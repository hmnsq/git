/*
 * Blame
 *
 * Copyright (c) 2006, 2014 by its authors
 * See COPYING for licensing conditions
 */

#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"
#include "config.h"
#include "color.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "commit.h"
#include "diff.h"
#include "revision.h"
#include "quote.h"
#include "string-list.h"
#include "mailmap.h"
#include "parse-options.h"
#include "prio-queue.h"
#include "utf8.h"
#include "userdiff.h"
#include "line-range.h"
#include "line-log.h"
#include "progress.h"
#include "object-name.h"
#include "odb.h"
#include "pager.h"
#include "blame.h"
#include "refs.h"
#include "setup.h"
#include "tag.h"
#include "write-or-die.h"

static const char blame_usage[] = N_("git blame [<options>] [<rev-opts>] [<rev>] [--] <file>");
static const char annotate_usage[] = N_("git annotate [<options>] [<rev-opts>] [<rev>] [--] <file>");

static const char *const blame_opt_usage[] = {
	blame_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	NULL
};

static const char *const annotate_opt_usage[] = {
	annotate_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	NULL
};

static целое longest_file;
static целое longest_author;
static целое max_orig_digits;
static целое max_digits;
static целое max_score_digits;
static целое show_root;
static целое reverse;
static целое blank_boundary;
static целое incremental;
static целое xdl_opts;
static целое abbrev = -1;
static целое no_whole_file_rename;
static целое show_progress;
static char repeated_meta_color[COLOR_MAXLEN];
static целое coloring_mode;
static struct string_list ignore_revs_file_list = STRING_LIST_INIT_DUP;
static целое mark_unblamable_lines;
static целое mark_ignored_lines;

static struct date_mode blame_date_mode = { DATE_ISO8601 };
static size_t blame_date_width;

static struct string_list mailmap = STRING_LIST_INIT_NODUP;

#ifndef DEBUG_BLAME
#define DEBUG_BLAME 0
#endif

static unsigned blame_move_score;
static unsigned blame_copy_score;

/* Remember to update object flag allocation in object.h */
#define METAINFO_SHOWN		(1u<<12)
#define MORE_THAN_ONE_PATH	(1u<<13)

struct progress_info {
	struct progress *progress;
	целое blamed_lines;
};

static const char *nth_line_cb(void *data, long lno)
{
	вернуть blame_nth_line((struct blame_scoreboard *)data, lno);
}

/*
 * Information on commits, used for output.
 */
struct commit_info {
	struct strbuf author;
	struct strbuf author_mail;
	timestamp_t author_time;
	struct strbuf author_tz;

	/* filled only when asked for details */
	struct strbuf committer;
	struct strbuf committer_mail;
	timestamp_t committer_time;
	struct strbuf committer_tz;

	struct strbuf summary;
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
static void get_ac_line(const char *inbuf, const char *what,
	struct strbuf *name, struct strbuf *mail,
	timestamp_t *time, struct strbuf *tz)
{
	struct ident_split ident;
	size_t len, maillen, namelen;
	const char *tmp, *endp;
	const char *namebuf, *mailbuf;

	tmp = strstr(inbuf, what);
	если (!tmp)
		goto error_out;
	tmp += strlen(what);
	endp = strchr(tmp, '\n');
	если (!endp)
		len = strlen(tmp);
	иначе
		len = endp - tmp;

	если (split_ident_line(&ident, tmp, len)) {
	error_out:
		/* Ugh */
		tmp = "(unknown)";
		strbuf_addstr(name, tmp);
		strbuf_addstr(mail, tmp);
		strbuf_addstr(tz, tmp);
		*time = 0;
		вернуть;
	}

	namelen = ident.name_end - ident.name_begin;
	namebuf = ident.name_begin;

	maillen = ident.mail_end - ident.mail_begin;
	mailbuf = ident.mail_begin;

	если (ident.date_begin && ident.date_end)
		*time = strtoul(ident.date_begin, NULL, 10);
	иначе
		*time = 0;

	если (ident.tz_begin && ident.tz_end)
		strbuf_add(tz, ident.tz_begin, ident.tz_end - ident.tz_begin);
	иначе
		strbuf_addstr(tz, "(unknown)");

	/*
	 * Now, convert both name and e-mail using mailmap
	 */
	map_user(&mailmap, &mailbuf, &maillen,
		 &namebuf, &namelen);

	strbuf_addf(mail, "<%.*s>", (целое)maillen, mailbuf);
	strbuf_add(name, namebuf, namelen);
}

static void commit_info_destroy(struct commit_info *ci)
{

	strbuf_release(&ci->author);
	strbuf_release(&ci->author_mail);
	strbuf_release(&ci->author_tz);
	strbuf_release(&ci->committer);
	strbuf_release(&ci->committer_mail);
	strbuf_release(&ci->committer_tz);
	strbuf_release(&ci->summary);
}

static void get_commit_info(struct commit *commit, struct commit_info *ret)
{
	целое len;
	const char *subject, *encoding;
	const char *message;

	encoding = get_log_output_encoding();
	message = repo_logmsg_reencode(the_repository, commit, NULL, encoding);
	get_ac_line(message, "\nauthor ",
		    &ret->author, &ret->author_mail,
		    &ret->author_time, &ret->author_tz);

	get_ac_line(message, "\ncommitter ",
		    &ret->committer, &ret->committer_mail,
		    &ret->committer_time, &ret->committer_tz);

	len = find_commit_subject(message, &subject);
	если (len)
		strbuf_add(&ret->summary, subject, len);
	иначе
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
static void write_filename_info(struct blame_origin *suspect)
{
	если (suspect->previous) {
		struct blame_origin *prev = suspect->previous;
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
static целое emit_one_suspect_detail(struct blame_origin *suspect, целое repeat)
{
	struct commit_info ci = COMMIT_INFO_INIT;

	если (!repeat && (suspect->commit->object.flags & METAINFO_SHOWN))
		вернуть 0;

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
	если (suspect->commit->object.flags & UNINTERESTING)
		printf("boundary\n");

	commit_info_destroy(&ci);

	вернуть 1;
}

/*
 * The blame_entry is found to be guilty for the range.
 * Show it in incremental output.
 */
static void found_guilty_entry(struct blame_entry *ent, void *data)
{
	struct progress_info *pi = (struct progress_info *)data;

	если (incremental) {
		struct blame_origin *suspect = ent->suspect;

		printf("%s %d %d %d\n",
		       oid_to_hex(&suspect->commit->object.oid),
		       ent->s_lno + 1, ent->lno + 1, ent->num_lines);
		emit_one_suspect_detail(suspect, 0);
		write_filename_info(suspect);
		maybe_flush_or_die(stdout, "stdout");
	}
	pi->blamed_lines += ent->num_lines;
	display_progress(pi->progress, pi->blamed_lines);
}

static const char *format_time(timestamp_t time, const char *tz_str,
			       целое show_raw_time)
{
	static struct strbuf time_buf = STRBUF_INIT;

	strbuf_reset(&time_buf);
	если (show_raw_time) {
		strbuf_addf(&time_buf, "%"PRItime" %s", time, tz_str);
	}
	иначе {
		const char *time_str;
		size_t time_width;
		целое tz;
		tz = atoi(tz_str);
		time_str = show_date(time, tz, blame_date_mode);
		strbuf_addstr(&time_buf, time_str);
		/*
		 * Add space paddings to time_buf to display a fixed width
		 * string, and use time_width for display width calibration.
		 */
		для (time_width = utf8_strwidth(time_str);
		     time_width < blame_date_width;
		     time_width++)
			strbuf_addch(&time_buf, ' ');
	}
	вернуть time_buf.buf;
}

#define OUTPUT_ANNOTATE_COMPAT      (1U<<0)
#define OUTPUT_LONG_OBJECT_NAME     (1U<<1)
#define OUTPUT_RAW_TIMESTAMP        (1U<<2)
#define OUTPUT_PORCELAIN            (1U<<3)
#define OUTPUT_SHOW_NAME            (1U<<4)
#define OUTPUT_SHOW_NUMBER          (1U<<5)
#define OUTPUT_SHOW_SCORE           (1U<<6)
#define OUTPUT_NO_AUTHOR            (1U<<7)
#define OUTPUT_SHOW_EMAIL           (1U<<8)
#define OUTPUT_LINE_PORCELAIN       (1U<<9)
#define OUTPUT_COLOR_LINE           (1U<<10)
#define OUTPUT_SHOW_AGE_WITH_COLOR  (1U<<11)

static void emit_porcelain_details(struct blame_origin *suspect, целое repeat)
{
	если (emit_one_suspect_detail(suspect, repeat) ||
	    (suspect->commit->object.flags & MORE_THAN_ONE_PATH))
		write_filename_info(suspect);
}

/*
 * Information which needs to be printed per-line goes here. Any
 * information which can be clubbed on a commit/file level, should
 * be printed via 'emit_one_suspect_detail()'.
 */
static void emit_porcelain_per_line_details(struct blame_entry *ent)
{
	если (mark_unblamable_lines && ent->unblamable)
		puts("unblamable");
	если (mark_ignored_lines && ent->ignored)
		puts("ignored");
}

static void emit_porcelain(struct blame_scoreboard *sb, struct blame_entry *ent,
			   целое opt)
{
	целое repeat = opt & OUTPUT_LINE_PORCELAIN;
	целое cnt;
	const char *cp;
	struct blame_origin *suspect = ent->suspect;
	char hex[GIT_MAX_HEXSZ + 1];

	oid_to_hex_r(hex, &suspect->commit->object.oid);
	printf("%s %d %d %d\n",
	       hex,
	       ent->s_lno + 1,
	       ent->lno + 1,
	       ent->num_lines);
	emit_porcelain_details(suspect, repeat);
	emit_porcelain_per_line_details(ent);

	cp = blame_nth_line(sb, ent->lno);
	для (cnt = 0; cnt < ent->num_lines; cnt++) {
		char ch;
		если (cnt) {
			printf("%s %d %d\n", hex,
			       ent->s_lno + 1 + cnt,
			       ent->lno + 1 + cnt);
			если (repeat)
				emit_porcelain_details(suspect, 1);
			emit_porcelain_per_line_details(ent);
		}
		putchar('\t');
		do {
			ch = *cp++;
			putchar(ch);
		} пока (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_size);
	}

	если (sb->final_buf_size && cp[-1] != '\n')
		putchar('\n');
}

static struct color_field {
	timestamp_t hop;
	char col[COLOR_MAXLEN];
} *colorfield;
static целое colorfield_nr, colorfield_alloc;

static void parse_color_fields(const char *s)
{
	struct string_list l = STRING_LIST_INIT_DUP;
	struct string_list_item *item;
	enum { EXPECT_DATE, EXPECT_COLOR } next = EXPECT_COLOR;

	colorfield_nr = 0;

	/* Ideally this would be stripped and split at the same time? */
	string_list_split(&l, s, ",", -1);
	ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);

	for_each_string_list_item(item, &l) {
		switch (next) {
		case EXPECT_DATE:
			colorfield[colorfield_nr].hop = approxidate(item->string);
			next = EXPECT_COLOR;
			colorfield_nr++;
			ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);
			break;
		case EXPECT_COLOR:
			если (color_parse(item->string, colorfield[colorfield_nr].col))
				die(_("expecting a color: %s"), item->string);
			next = EXPECT_DATE;
			break;
		}
	}

	если (next == EXPECT_COLOR)
		die(_("must end with a color"));

	colorfield[colorfield_nr].hop = TIME_MAX;
	string_list_clear(&l, 0);
}

static void setup_default_color_by_age(void)
{
	parse_color_fields("blue,12 month ago,white,1 month ago,red");
}

static void determine_line_heat(struct commit_info *ci, const char **dest_color)
{
	целое i = 0;

	пока (i < colorfield_nr && ci->author_time > colorfield[i].hop)
		i++;

	*dest_color = colorfield[i].col;
}

static inline целое maybe_putc(целое c, FILE *out)
{
	вернуть out ? putc(c, out) : 0;
}

static size_t print_marks(FILE *out, const struct blame_entry *ent, целое opt)
{
	size_t len = 0;

	если ((ent->suspect->commit->object.flags & UNINTERESTING) &&
	    !blank_boundary && !(opt & OUTPUT_ANNOTATE_COMPAT)) {
		maybe_putc('^', out);
		len++;
	}
	если (mark_unblamable_lines && ent->unblamable) {
		maybe_putc('*', out);
		len++;
	}
	если (mark_ignored_lines && ent->ignored) {
		maybe_putc('?', out);
		len++;
	}
	вернуть len;
}

static size_t count_marks(const struct blame_entry *ent, целое opt)
{
	вернуть print_marks(NULL, ent, opt);
}

static void emit_other(struct blame_scoreboard *sb, struct blame_entry *ent,
		       целое opt, struct blame_entry *prev_ent)
{
	целое cnt;
	const char *cp;
	struct blame_origin *suspect = ent->suspect;
	struct commit_info ci = COMMIT_INFO_INIT;
	char hex[GIT_MAX_HEXSZ + 1];
	целое show_raw_time = !!(opt & OUTPUT_RAW_TIMESTAMP);
	const char *default_color = NULL, *color = NULL, *reset = NULL;

	get_commit_info(suspect->commit, &ci);
	oid_to_hex_r(hex, &suspect->commit->object.oid);

	cp = blame_nth_line(sb, ent->lno);

	если (opt & OUTPUT_SHOW_AGE_WITH_COLOR) {
		determine_line_heat(&ci, &default_color);
		color = default_color;
		reset = GIT_COLOR_RESET;
	}

	если (abbrev < MINIMUM_ABBREV)
		BUG("abbreviation is smaller than minimum length: %d < %d",
		    abbrev, MINIMUM_ABBREV);

	для (cnt = 0; cnt < ent->num_lines; cnt++) {
		char ch;
		size_t length = (opt & OUTPUT_LONG_OBJECT_NAME) ?
			the_hash_algo->hexsz : (size_t) abbrev;

		если (opt & OUTPUT_COLOR_LINE) {
			если (cnt > 0 ||
			    (prev_ent &&
			     oideq(&suspect->commit->object.oid,
				   &prev_ent->suspect->commit->object.oid))) {
				color = repeated_meta_color;
				reset = GIT_COLOR_RESET;
			} иначе  {
				color = default_color ? default_color : NULL;
				reset = default_color ? GIT_COLOR_RESET : NULL;
			}
		}
		если (color)
			fputs(color, stdout);

		если ((suspect->commit->object.flags & UNINTERESTING) &&
		    blank_boundary)
			memset(hex, ' ', strlen(hex));
		length -= print_marks(stdout, ent, opt);

		printf("%.*s", (целое)(length < GIT_MAX_HEXSZ ? length : GIT_MAX_HEXSZ), hex);
		если (opt & OUTPUT_ANNOTATE_COMPAT) {
			const char *name;
			если (opt & OUTPUT_SHOW_EMAIL)
				name = ci.author_mail.buf;
			иначе
				name = ci.author.buf;
			printf("\t(%10s\t%10s\t%d)", name,
			       format_time(ci.author_time, ci.author_tz.buf,
					   show_raw_time),
			       ent->lno + 1 + cnt);
		} иначе {
			если (opt & OUTPUT_SHOW_SCORE)
				printf(" %*d %02d",
				       max_score_digits, ent->score,
				       ent->suspect->refcnt);
			если (opt & OUTPUT_SHOW_NAME)
				printf(" %-*.*s", longest_file, longest_file,
				       suspect->path);
			если (opt & OUTPUT_SHOW_NUMBER)
				printf(" %*d", max_orig_digits,
				       ent->s_lno + 1 + cnt);

			если (!(opt & OUTPUT_NO_AUTHOR)) {
				const char *name;
				целое pad;
				если (opt & OUTPUT_SHOW_EMAIL)
					name = ci.author_mail.buf;
				иначе
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
		если (reset)
			fputs(reset, stdout);
		do {
			ch = *cp++;
			putchar(ch);
		} пока (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_size);
	}

	если (sb->final_buf_size && cp[-1] != '\n')
		putchar('\n');

	commit_info_destroy(&ci);
}

static void output(struct blame_scoreboard *sb, целое option)
{
	struct blame_entry *ent, *prev_ent = NULL;

	если (option & OUTPUT_PORCELAIN) {
		для (ent = sb->ent; ent; ent = ent->next) {
			целое count = 0;
			struct blame_origin *suspect;
			struct commit *commit = ent->suspect->commit;
			если (commit->object.flags & MORE_THAN_ONE_PATH)
				continue;
			для (suspect = get_blame_suspects(commit); suspect; suspect = suspect->next) {
				если (suspect->guilty && count++) {
					commit->object.flags |= MORE_THAN_ONE_PATH;
					break;
				}
			}
		}
	}

	для (ent = sb->ent; ent; ent = ent->next) {
		если (option & OUTPUT_PORCELAIN)
			emit_porcelain(sb, ent, option);
		иначе {
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
static целое read_ancestry(const char *graft_file)
{
	FILE *fp = fopen_or_warn(graft_file, "r");
	struct strbuf buf = STRBUF_INIT;
	если (!fp)
		вернуть -1;
	пока (!strbuf_getwholeline(&buf, fp, '\n')) {
		/* The format is just "Commit Parent1 Parent2 ...\n" */
		struct commit_graft *graft = read_graft_line(&buf);
		если (graft)
			register_commit_graft(the_repository, graft, 0);
	}
	fclose(fp);
	strbuf_release(&buf);
	вернуть 0;
}

static целое update_auto_abbrev(целое auto_abbrev, struct blame_origin *suspect)
{
	const char *uniq = repo_find_unique_abbrev(the_repository,
						   &suspect->commit->object.oid,
						   auto_abbrev);
	целое len = strlen(uniq);
	если (auto_abbrev < len)
		вернуть len;
	вернуть auto_abbrev;
}

/*
 * How many columns do we need to show line numbers, authors,
 * and filenames?
 */
static void find_alignment(struct blame_scoreboard *sb, целое *option)
{
	целое longest_src_lines = 0;
	целое longest_dst_lines = 0;
	unsigned largest_score = 0;
	struct blame_entry *e;
	целое compute_auto_abbrev = (abbrev < 0);
	целое auto_abbrev = DEFAULT_ABBREV;
	size_t max_marks_count = 0;

	для (e = sb->ent; e; e = e->next) {
		struct blame_origin *suspect = e->suspect;
		целое num;
		size_t marks_count = count_marks(e, *option);

		если (max_marks_count < marks_count)
			max_marks_count = marks_count;
		если (compute_auto_abbrev)
			auto_abbrev = update_auto_abbrev(auto_abbrev, suspect);
		если (strcmp(suspect->path, sb->path))
			*option |= OUTPUT_SHOW_NAME;
		num = strlen(suspect->path);
		если (longest_file < num)
			longest_file = num;
		если (!(suspect->commit->object.flags & METAINFO_SHOWN)) {
			struct commit_info ci = COMMIT_INFO_INIT;
			suspect->commit->object.flags |= METAINFO_SHOWN;
			get_commit_info(suspect->commit, &ci);
			если (*option & OUTPUT_SHOW_EMAIL)
				num = utf8_strwidth(ci.author_mail.buf);
			иначе
				num = utf8_strwidth(ci.author.buf);
			если (longest_author < num)
				longest_author = num;
			commit_info_destroy(&ci);
		}
		num = e->s_lno + e->num_lines;
		если (longest_src_lines < num)
			longest_src_lines = num;
		num = e->lno + e->num_lines;
		если (longest_dst_lines < num)
			longest_dst_lines = num;
		если (largest_score < blame_entry_score(sb, e))
			largest_score = blame_entry_score(sb, e);
	}
	max_orig_digits = decimal_width(longest_src_lines);
	max_digits = decimal_width(longest_dst_lines);
	max_score_digits = decimal_width(largest_score);

	если (compute_auto_abbrev)
		abbrev = auto_abbrev;
	если (abbrev < (целое)the_hash_algo->hexsz) {
		abbrev += max_marks_count;
		если (abbrev > (целое)the_hash_algo->hexsz)
			abbrev = the_hash_algo->hexsz;
	}
}

static void sanity_check_on_fail(struct blame_scoreboard *sb, целое baa)
{
	целое opt = OUTPUT_SHOW_SCORE | OUTPUT_SHOW_NUMBER | OUTPUT_SHOW_NAME;
	find_alignment(sb, &opt);
	output(sb, opt);
	die("Baa %d!", baa);
}

static unsigned parse_score(const char *arg)
{
	char *end;
	unsigned long score = strtoul(arg, &end, 10);
	если (*end)
		вернуть 0;
	вернуть score;
}

static char *add_prefix(const char *prefix, const char *path)
{
	вернуть prefix_path(the_repository, prefix, prefix ? strlen(prefix) : 0, path);
}

static целое git_blame_config(const char *var, const char *value,
			    const struct config_context *ctx, void *cb)
{
	если (!strcmp(var, "blame.showroot")) {
		show_root = git_config_bool(var, value);
		вернуть 0;
	}
	если (!strcmp(var, "blame.blankboundary")) {
		blank_boundary = git_config_bool(var, value);
		вернуть 0;
	}
	если (!strcmp(var, "blame.showemail")) {
		целое *output_option = cb;
		если (git_config_bool(var, value))
			*output_option |= OUTPUT_SHOW_EMAIL;
		иначе
			*output_option &= ~OUTPUT_SHOW_EMAIL;
		вернуть 0;
	}
	если (!strcmp(var, "blame.date")) {
		если (!value)
			вернуть config_error_nonbool(var);
		parse_date_format(value, &blame_date_mode);
		вернуть 0;
	}
	если (!strcmp(var, "blame.ignorerevsfile")) {
		char *str;
		целое ret;

		ret = git_config_pathname(&str, var, value);
		если (ret)
			вернуть ret;
		если (str)
			string_list_insert(&ignore_revs_file_list, str);
		free(str);
		вернуть 0;
	}
	если (!strcmp(var, "blame.markunblamablelines")) {
		mark_unblamable_lines = git_config_bool(var, value);
		вернуть 0;
	}
	если (!strcmp(var, "blame.markignoredlines")) {
		mark_ignored_lines = git_config_bool(var, value);
		вернуть 0;
	}
	если (!strcmp(var, "color.blame.repeatedlines")) {
		если (color_parse_mem(value, strlen(value), repeated_meta_color))
			warning(_("invalid value for '%s': '%s'"),
				"color.blame.repeatedLines", value);
		вернуть 0;
	}
	если (!strcmp(var, "color.blame.highlightrecent")) {
		parse_color_fields(value);
		вернуть 0;
	}

	если (!strcmp(var, "blame.coloring")) {
		если (!value)
			вернуть config_error_nonbool(var);
		если (!strcmp(value, "repeatedLines")) {
			coloring_mode |= OUTPUT_COLOR_LINE;
		} иначе если (!strcmp(value, "highlightRecent")) {
			coloring_mode |= OUTPUT_SHOW_AGE_WITH_COLOR;
		} иначе если (!strcmp(value, "none")) {
			coloring_mode &= ~(OUTPUT_COLOR_LINE |
					    OUTPUT_SHOW_AGE_WITH_COLOR);
		} иначе {
			warning(_("invalid value for '%s': '%s'"),
				"blame.coloring", value);
			вернуть 0;
		}
	}

	если (!strcmp(var, "diff.algorithm")) {
		long diff_algorithm;
		если (!value)
			вернуть config_error_nonbool(var);
		diff_algorithm = parse_algorithm_value(value);
		если (diff_algorithm < 0)
			вернуть error(_("unknown value for config '%s': %s"),
				     var, value);
		xdl_opts &= ~XDF_DIFF_ALGORITHM_MASK;
		xdl_opts |= diff_algorithm;
		вернуть 0;
	}

	если (git_diff_heuristic_config(var, value, cb) < 0)
		вернуть -1;
	если (userdiff_config(var, value) < 0)
		вернуть -1;

	вернуть git_default_config(var, value, ctx, cb);
}

static целое blame_copy_callback(const struct option *option, const char *arg, целое unset)
{
	целое *opt = option->value;

	BUG_ON_OPT_NEG(unset);

	/*
	 * -C enables copy from removed files;
	 * -C -C enables copy from existing files, but only
	 *       when blaming a new file;
	 * -C -C -C enables copy from existing files for
	 *          everybody
	 */
	если (*opt & PICKAXE_BLAME_COPY_HARDER)
		*opt |= PICKAXE_BLAME_COPY_HARDEST;
	если (*opt & PICKAXE_BLAME_COPY)
		*opt |= PICKAXE_BLAME_COPY_HARDER;
	*opt |= PICKAXE_BLAME_COPY | PICKAXE_BLAME_MOVE;

	если (arg)
		blame_copy_score = parse_score(arg);
	вернуть 0;
}

static целое blame_move_callback(const struct option *option, const char *arg, целое unset)
{
	целое *opt = option->value;

	BUG_ON_OPT_NEG(unset);

	*opt |= PICKAXE_BLAME_MOVE;

	если (arg)
		blame_move_score = parse_score(arg);
	вернуть 0;
}

static целое blame_diff_algorithm_minimal(const struct option *option,
					const char *arg, целое unset)
{
	целое *opt = option->value;

	BUG_ON_OPT_ARG(arg);

	*opt &= ~XDF_DIFF_ALGORITHM_MASK;
	если (!unset)
		*opt |= XDF_NEED_MINIMAL;

	вернуть 0;
}

static целое blame_diff_algorithm_callback(const struct option *option,
					 const char *arg, целое unset)
{
	целое *opt = option->value;
	long value = parse_algorithm_value(arg);

	BUG_ON_OPT_NEG(unset);

	если (value < 0)
		вернуть error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	*opt &= ~XDF_DIFF_ALGORITHM_MASK;
	*opt |= value;

	вернуть 0;
}

static целое is_a_rev(const char *name)
{
	struct object_id oid;

	если (repo_get_oid(the_repository, name, &oid))
		вернуть 0;
	вернуть OBJ_NONE < odb_read_object_info(the_repository->objects, &oid, NULL);
}

static целое peel_to_commit_oid(struct object_id *oid_ret, void *cbdata)
{
	struct repository *r = ((struct blame_scoreboard *)cbdata)->repo;
	struct object_id oid;

	oidcpy(&oid, oid_ret);
	пока (1) {
		struct object *obj;
		целое kind = odb_read_object_info(r->objects, &oid, NULL);
		если (kind == OBJ_COMMIT) {
			oidcpy(oid_ret, &oid);
			вернуть 0;
		}
		если (kind != OBJ_TAG)
			вернуть -1;
		obj = deref_tag(r, parse_object(r, &oid), NULL, 0);
		если (!obj)
			вернуть -1;
		oidcpy(&oid, &obj->oid);
	}
}

static void build_ignorelist(struct blame_scoreboard *sb,
			     struct string_list *ignore_revs_file_list,
			     struct string_list *ignore_rev_list)
{
	struct string_list_item *i;
	struct object_id oid;

	oidset_init(&sb->ignore_list, 0);
	for_each_string_list_item(i, ignore_revs_file_list) {
		если (!strcmp(i->string, ""))
			oidset_clear(&sb->ignore_list);
		иначе
			oidset_parse_file_carefully(&sb->ignore_list, i->string,
						    the_repository->hash_algo,
						    peel_to_commit_oid, sb);
	}
	for_each_string_list_item(i, ignore_rev_list) {
		если (repo_get_oid_committish(the_repository, i->string, &oid) ||
		    peel_to_commit_oid(&oid, sb))
			die(_("cannot find revision %s to ignore"), i->string);
		oidset_insert(&sb->ignore_list, &oid);
	}
}

целое cmd_blame(целое argc,
	      const char **argv,
	      const char *prefix,
	      struct repository *repo UNUSED)
{
	struct rev_info revs;
	char *path = NULL;
	struct blame_scoreboard sb;
	struct blame_origin *o;
	struct blame_entry *ent = NULL;
	long dashdash_pos, lno;
	struct progress_info pi = { NULL, 0 };

	struct string_list range_list = STRING_LIST_INIT_NODUP;
	struct string_list ignore_rev_list = STRING_LIST_INIT_NODUP;
	целое output_option = 0, opt = 0;
	целое show_stats = 0;
	const char *revs_file = NULL;
	const char *contents_from = NULL;
	const struct option options[] = {
		OPT_BOOL(0, "incremental", &incremental, N_("show blame entries as we find them, incrementally")),
		OPT_BOOL('b', NULL, &blank_boundary, N_("do not show object names of boundary commits (Default: off)")),
		OPT_BOOL(0, "root", &show_root, N_("do not treat root commits as boundaries (Default: off)")),
		OPT_BOOL(0, "show-stats", &show_stats, N_("show work cost statistics")),
		OPT_BOOL(0, "progress", &show_progress, N_("force progress reporting")),
		OPT_BIT(0, "score-debug", &output_option, N_("show output score for blame entries"), OUTPUT_SHOW_SCORE),
		OPT_BIT('f', "show-name", &output_option, N_("show original filename (Default: auto)"), OUTPUT_SHOW_NAME),
		OPT_BIT('n', "show-number", &output_option, N_("show original linenumber (Default: off)"), OUTPUT_SHOW_NUMBER),
		OPT_BIT('p', "porcelain", &output_option, N_("show in a format designed for machine consumption"), OUTPUT_PORCELAIN),
		OPT_BIT(0, "line-porcelain", &output_option, N_("show porcelain format with per-line commit information"), OUTPUT_PORCELAIN|OUTPUT_LINE_PORCELAIN),
		OPT_BIT('c', NULL, &output_option, N_("use the same output mode as git-annotate (Default: off)"), OUTPUT_ANNOTATE_COMPAT),
		OPT_BIT('t', NULL, &output_option, N_("show raw timestamp (Default: off)"), OUTPUT_RAW_TIMESTAMP),
		OPT_BIT('l', NULL, &output_option, N_("show long commit SHA1 (Default: off)"), OUTPUT_LONG_OBJECT_NAME),
		OPT_BIT('s', NULL, &output_option, N_("suppress author name and timestamp (Default: off)"), OUTPUT_NO_AUTHOR),
		OPT_BIT('e', "show-email", &output_option, N_("show author email instead of name (Default: off)"), OUTPUT_SHOW_EMAIL),
		OPT_BIT('w', NULL, &xdl_opts, N_("ignore whitespace differences"), XDF_IGNORE_WHITESPACE),
		OPT_CALLBACK_F(0, "diff-algorithm", &xdl_opts, N_("<algorithm>"),
			       N_("choose a diff algorithm"),
			       PARSE_OPT_NONEG, blame_diff_algorithm_callback),
		OPT_STRING_LIST(0, "ignore-rev", &ignore_rev_list, N_("rev"), N_("ignore <rev> when blaming")),
		OPT_STRING_LIST(0, "ignore-revs-file", &ignore_revs_file_list, N_("file"), N_("ignore revisions from <file>")),
		OPT_BIT(0, "color-lines", &output_option, N_("color redundant metadata from previous line differently"), OUTPUT_COLOR_LINE),
		OPT_BIT(0, "color-by-age", &output_option, N_("color lines by age"), OUTPUT_SHOW_AGE_WITH_COLOR),
		OPT_CALLBACK_F(0, "minimal", &xdl_opts, NULL,
			       N_("spend extra cycles to find a better match"),
			       PARSE_OPT_NOARG | PARSE_OPT_HIDDEN, blame_diff_algorithm_minimal),
		OPT_STRING('S', NULL, &revs_file, N_("file"), N_("use revisions from <file> instead of calling git-rev-list")),
		OPT_STRING(0, "contents", &contents_from, N_("file"), N_("use <file>'s contents as the final image")),
		OPT_CALLBACK_F('C', NULL, &opt, N_("score"), N_("find line copies within and across files"), PARSE_OPT_OPTARG, blame_copy_callback),
		OPT_CALLBACK_F('M', NULL, &opt, N_("score"), N_("find line movements within and across files"), PARSE_OPT_OPTARG, blame_move_callback),
		OPT_STRING_LIST('L', NULL, &range_list, N_("range"),
				N_("process only line range <start>,<end> or function :<funcname>")),
		OPT__ABBREV(&abbrev),
		OPT_END()
	};

	struct parse_opt_ctx_t ctx;
	целое cmd_is_annotate = !strcmp(argv[0], "annotate");
	struct range_set ranges;
	unsigned целое range_i;
	long anchor;
	long num_lines = 0;
	const char *str_usage = cmd_is_annotate ? annotate_usage : blame_usage;
	const char *const *opt_usage = cmd_is_annotate ? annotate_opt_usage : blame_opt_usage;

	setup_default_color_by_age();
	repo_config(the_repository, git_blame_config, &output_option);
	repo_init_revisions(the_repository, &revs, NULL);
	revs.date_mode = blame_date_mode;
	revs.diffopt.flags.allow_textconv = 1;
	revs.diffopt.flags.follow_renames = 1;

	save_commit_buffer = 0;
	dashdash_pos = 0;
	show_progress = -1;

	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);
	для (;;) {
		switch (parse_options_step(&ctx, options, opt_usage)) {
		case PARSE_OPT_NON_OPTION:
		case PARSE_OPT_UNKNOWN:
			break;
		case PARSE_OPT_HELP:
			exit(0);
		case PARSE_OPT_HELP_ERROR:
		case PARSE_OPT_ERROR:
		case PARSE_OPT_SUBCOMMAND:
			exit(129);
		case PARSE_OPT_COMPLETE:
			exit(0);
		case PARSE_OPT_DONE:
			если (ctx.argv[0])
				dashdash_pos = ctx.cpidx;
			goto parse_done;
		}

		если (!strcmp(ctx.argv[0], "--reverse")) {
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

	если (incremental || (output_option & OUTPUT_PORCELAIN)) {
		если (show_progress > 0)
			die(_("--progress can't be used with --incremental or porcelain formats"));
		show_progress = 0;
	} иначе если (show_progress < 0)
		show_progress = isatty(2);

	если (!abbrev)
		abbrev = the_hash_algo->hexsz;

	если (revs_file && read_ancestry(revs_file))
		die_errno("reading graft file '%s' failed", revs_file);

	если (cmd_is_annotate) {
		output_option |= OUTPUT_ANNOTATE_COMPAT;
		blame_date_mode.type = DATE_ISO8601;
	} иначе {
		blame_date_mode = revs.date_mode;
	}

	/* The maximum width used to show the dates */
	switch (blame_date_mode.type) {
	case DATE_RFC2822:
		blame_date_width = sizeof("Thu, 19 Oct 2006 16:00:04 -0700");
		break;
	case DATE_ISO8601_STRICT:
		blame_date_width = sizeof("2006-10-19T16:00:04-07:00");
		break;
	case DATE_ISO8601:
		blame_date_width = sizeof("2006-10-19 16:00:04 -0700");
		break;
	case DATE_RAW:
		blame_date_width = sizeof("1161298804 -0700");
		break;
	case DATE_UNIX:
		blame_date_width = sizeof("1161298804");
		break;
	case DATE_SHORT:
		blame_date_width = sizeof("2006-10-19");
		break;
	case DATE_RELATIVE:
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
		break;
	case DATE_HUMAN:
		/* If the year is shown, no time is shown */
		blame_date_width = sizeof("Thu Oct 19 16:00");
		break;
	case DATE_NORMAL:
		blame_date_width = sizeof("Thu Oct 19 16:00:04 2006 -0700");
		break;
	case DATE_STRFTIME:
		blame_date_width = strlen(show_date(0, 0, blame_date_mode)) + 1; /* add the null */
		break;
	}
	blame_date_width -= 1; /* strip the null */

	если (revs.diffopt.flags.find_copies_harder)
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
	если (dashdash_pos) {
		switch (argc - dashdash_pos - 1) {
		case 2: /* (1b) */
			если (argc != 4)
				usage_with_options(opt_usage, options);
			/* reorder for the new way: <rev> -- <path> */
			argv[1] = argv[3];
			argv[3] = argv[2];
			argv[2] = "--";
			/* FALLTHROUGH */
		case 1: /* (1a) */
			path = add_prefix(prefix, argv[--argc]);
			argv[argc] = NULL;
			break;
		default:
			usage_with_options(opt_usage, options);
		}
	} иначе {
		если (argc < 2)
			usage_with_options(opt_usage, options);
		если (argc == 3 && is_a_rev(argv[argc - 1])) { /* (2b) */
			path = add_prefix(prefix, argv[1]);
			argv[1] = argv[2];
		} иначе {	/* (2a) */
			если (argc == 2 && is_a_rev(argv[1]) && !repo_get_work_tree(the_repository))
				die("missing <path> to blame");
			path = add_prefix(prefix, argv[argc - 1]);
		}
		argv[argc - 1] = "--";
	}

	revs.disable_stdin = 1;
	setup_revisions(argc, argv, &revs, NULL);
	если (!revs.pending.nr && is_bare_repository(the_repository)) {
		struct commit *head_commit;
		struct object_id head_oid;

		если (!refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", RESOLVE_REF_READING,
					     &head_oid, NULL) ||
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
	если (!(opt & PICKAXE_BLAME_COPY))
		setup_blame_bloom_data(&sb);

	lno = sb.num_lines;

	если (lno && !range_list.nr)
		string_list_append(&range_list, "1");

	anchor = 1;
	range_set_init(&ranges, range_list.nr);
	для (range_i = 0; range_i < range_list.nr; ++range_i) {
		long bottom, top;
		если (parse_range_arg(range_list.items[range_i].string,
				    nth_line_cb, &sb, lno, anchor,
				    &bottom, &top, sb.path,
				    the_repository->index))
			usage(str_usage);
		если ((!lno && (top || bottom)) || lno < bottom)
			die(Q_("file %s has only %lu line",
			       "file %s has only %lu lines",
			       lno), sb.path, lno);
		если (bottom < 1)
			bottom = 1;
		если (top < 1 || lno < top)
			top = lno;
		bottom--;
		range_set_append_unsafe(&ranges, bottom, top);
		anchor = top + 1;
	}
	sort_and_merge_range_set(&ranges);

	для (range_i = ranges.nr; range_i > 0; --range_i) {
		const struct range *r = &ranges.ranges[range_i - 1];
		ent = blame_entry_prepend(ent, r->start, r->end, o);
		num_lines += (r->end - r->start);
	}
	если (!num_lines)
		num_lines = sb.num_lines;

	o->suspects = ent;
	prio_queue_put(&sb.commits, o->commit);

	blame_origin_decref(o);

	range_set_release(&ranges);
	string_list_clear(&range_list, 0);

	sb.ent = NULL;

	если (blame_move_score)
		sb.move_score = blame_move_score;
	если (blame_copy_score)
		sb.copy_score = blame_copy_score;

	sb.debug = DEBUG_BLAME;
	sb.on_sanity_fail = &sanity_check_on_fail;

	sb.show_root = show_root;
	sb.xdl_opts = xdl_opts;
	sb.no_whole_file_rename = no_whole_file_rename;

	read_mailmap(the_repository, &mailmap);

	sb.found_guilty_entry = &found_guilty_entry;
	sb.found_guilty_entry_data = &pi;
	если (show_progress)
		pi.progress = start_delayed_progress(the_repository,
						     _("Blaming lines"),
						     num_lines);

	assign_blame(&sb, opt);

	stop_progress(&pi.progress);

	если (!incremental)
		setup_pager(the_repository);
	иначе
		goto cleanup;

	blame_sort_final(&sb);

	blame_coalesce(&sb);

	если (!(output_option & (OUTPUT_COLOR_LINE | OUTPUT_SHOW_AGE_WITH_COLOR)))
		output_option |= coloring_mode;

	если (!(output_option & OUTPUT_PORCELAIN)) {
		find_alignment(&sb, &output_option);
		если (!*repeated_meta_color &&
		    (output_option & OUTPUT_COLOR_LINE))
			xsnprintf(repeated_meta_color,
				  sizeof(repeated_meta_color),
				  "%s", GIT_COLOR_CYAN);
	}
	если (output_option & OUTPUT_ANNOTATE_COMPAT)
		output_option &= ~(OUTPUT_COLOR_LINE | OUTPUT_SHOW_AGE_WITH_COLOR);

	output(&sb, output_option);

	если (show_stats) {
		printf("num read blob: %d\n", sb.num_read_blob);
		printf("num get patch: %d\n", sb.num_get_patch);
		printf("num commits: %d\n", sb.num_commits);
	}

cleanup:
	для (ent = sb.ent; ent; ) {
		struct blame_entry *e = ent->next;
		free(ent);
		ent = e;
	}

	free(path);
	cleanup_scoreboard(&sb);
	release_revisions(&revs);
	вернуть 0;
}
