/*
 * Blame
 *
 * Copyright (c) 2006, 2014 by its authors
 * See COPYING for licensing conditions
 */

#define USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "config.h"
#شامل "color.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "commit.h"
#شامل "diff.h"
#شامل "revision.h"
#شامل "quote.h"
#شامل "string-list.h"
#شامل "mailmap.h"
#شامل "parse-options.h"
#شامل "prio-queue.h"
#شامل "utf8.h"
#شامل "userdiff.h"
#شامل "line-range.h"
#شامل "line-log.h"
#شامل "progress.h"
#شامل "object-name.h"
#شامل "odb.h"
#شامل "pager.h"
#شامل "blame.h"
#شامل "refs.h"
#شامل "setup.h"
#شامل "tag.h"
#شامل "write-or-die.h"

ایستا ثابت نویسه blame_usage[] = N_("git blame [<options>] [<rev-opts>] [<rev>] [--] <file>");
ایستا ثابت نویسه annotate_usage[] = N_("git annotate [<options>] [<rev-opts>] [<rev>] [--] <file>");

ایستا ثابت نویسه *ثابت blame_opt_usage[] = {
	blame_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	پوچ
};

ایستا ثابت نویسه *ثابت annotate_opt_usage[] = {
	annotate_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	پوچ
};

ایستا صحیح longest_file;
ایستا صحیح longest_author;
ایستا صحیح max_orig_digits;
ایستا صحیح max_digits;
ایستا صحیح max_score_digits;
ایستا صحیح show_ریشه;
ایستا صحیح reverse;
ایستا صحیح blank_boundary;
ایستا صحیح incremental;
ایستا صحیح xdl_opts;
ایستا صحیح abbrev = -1;
ایستا صحیح no_whole_file_rename;
ایستا صحیح show_progress;
ایستا نویسه repeated_meta_color[COLOR_MAXLEN];
ایستا صحیح coloring_mode;
ایستا ساختار string_list ignore_revs_file_list = STRING_LIST_INIT_DUP;
ایستا صحیح mark_unblamable_سطرها;
ایستا صحیح mark_ignored_سطرها;

ایستا ساختار date_mode blame_date_mode = { DATE_ISO8601 };
ایستا size_t blame_date_width;

ایستا ساختار string_list mailmap = STRING_LIST_INIT_NODUP;

#ifndef DEBUG_BLAME
#define DEBUG_BLAME 0
#endif

ایستا unsigned blame_move_score;
ایستا unsigned blame_copy_score;

/* Remember to update object flag allocation in object.h */
#define METAINFO_SHOWN		(1u<<12)
#define MORE_THAN_ONE_PATH	(1u<<13)

ساختار progress_info {
	ساختار progress *progress;
	صحیح blamed_سطرها;
};

ایستا ثابت نویسه *nth_سطر_cb(تهی *data, long lno)
{
	بازگشت blame_nth_سطر((ساختار blame_scoreboard *)data, lno);
}

/*
 * Information on commits, used for output.
 */
ساختار commit_info {
	ساختار strbuf author;
	ساختار strbuf author_mail;
	timestamp_t author_time;
	ساختار strbuf author_tz;

	/* filled only when asked for details */
	ساختار strbuf committer;
	ساختار strbuf committer_mail;
	timestamp_t committer_time;
	ساختار strbuf committer_tz;

	ساختار strbuf summary;
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
ایستا تهی get_ac_سطر(ثابت نویسه *inbuf, ثابت نویسه *what,
	ساختار strbuf *name, ساختار strbuf *mail,
	timestamp_t *time, ساختار strbuf *tz)
{
	ساختار ident_split ident;
	size_t len, maillen, namelen;
	ثابت نویسه *tmp, *endp;
	ثابت نویسه *namebuf, *mailbuf;

	tmp = strstr(inbuf, what);
	اگر (!tmp)
		برو error_out;
	tmp += strlen(what);
	endp = strchr(tmp, '\n');
	اگر (!endp)
		len = strlen(tmp);
	وگرنه
		len = endp - tmp;

	اگر (split_ident_سطر(&ident, tmp, len)) {
	error_out:
		/* Ugh */
		tmp = "(unknown)";
		strbuf_addstr(name, tmp);
		strbuf_addstr(mail, tmp);
		strbuf_addstr(tz, tmp);
		*time = 0;
		بازگشت;
	}

	namelen = ident.name_end - ident.name_begin;
	namebuf = ident.name_begin;

	maillen = ident.mail_end - ident.mail_begin;
	mailbuf = ident.mail_begin;

	اگر (ident.date_begin && ident.date_end)
		*time = strtoul(ident.date_begin, پوچ, 10);
	وگرنه
		*time = 0;

	اگر (ident.tz_begin && ident.tz_end)
		strbuf_add(tz, ident.tz_begin, ident.tz_end - ident.tz_begin);
	وگرنه
		strbuf_addstr(tz, "(unknown)");

	/*
	 * Now, convert both name and e-mail using mailmap
	 */
	map_user(&mailmap, &mailbuf, &maillen,
		 &namebuf, &namelen);

	strbuf_addf(mail, "<%.*s>", (صحیح)maillen, mailbuf);
	strbuf_add(name, namebuf, namelen);
}

ایستا تهی commit_info_destroy(ساختار commit_info *ci)
{

	strbuf_release(&ci->author);
	strbuf_release(&ci->author_mail);
	strbuf_release(&ci->author_tz);
	strbuf_release(&ci->committer);
	strbuf_release(&ci->committer_mail);
	strbuf_release(&ci->committer_tz);
	strbuf_release(&ci->summary);
}

ایستا تهی get_commit_info(ساختار commit *commit, ساختار commit_info *ret)
{
	صحیح len;
	ثابت نویسه *subject, *encoding;
	ثابت نویسه *message;

	encoding = get_log_output_encoding();
	message = repo_logmsg_reencode(the_repository, commit, پوچ, encoding);
	get_ac_سطر(message, "\nauthor ",
		    &ret->author, &ret->author_mail,
		    &ret->author_time, &ret->author_tz);

	get_ac_سطر(message, "\ncommitter ",
		    &ret->committer, &ret->committer_mail,
		    &ret->committer_time, &ret->committer_tz);

	len = find_commit_subject(message, &subject);
	اگر (len)
		strbuf_add(&ret->summary, subject, len);
	وگرنه
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
ایستا تهی write_filename_info(ساختار blame_origin *suspect)
{
	اگر (suspect->previous) {
		ساختار blame_origin *prev = suspect->previous;
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
ایستا صحیح emit_one_suspect_detail(ساختار blame_origin *suspect, صحیح repeat)
{
	ساختار commit_info ci = COMMIT_INFO_INIT;

	اگر (!repeat && (suspect->commit->object.flags & METAINFO_SHOWN))
		بازگشت 0;

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
	اگر (suspect->commit->object.flags & UNINTERESTING)
		printf("boundary\n");

	commit_info_destroy(&ci);

	بازگشت 1;
}

/*
 * The blame_entry is found to be guilty for the range.
 * Show it in incremental output.
 */
ایستا تهی found_guilty_entry(ساختار blame_entry *ent, تهی *data)
{
	ساختار progress_info *pi = (ساختار progress_info *)data;

	اگر (incremental) {
		ساختار blame_origin *suspect = ent->suspect;

		printf("%s %d %d %d\n",
		       oid_to_hex(&suspect->commit->object.oid),
		       ent->s_lno + 1, ent->lno + 1, ent->رقم_سطرها);
		emit_one_suspect_detail(suspect, 0);
		write_filename_info(suspect);
		maybe_flush_or_die(stdout, "stdout");
	}
	pi->blamed_سطرها += ent->رقم_سطرها;
	display_progress(pi->progress, pi->blamed_سطرها);
}

ایستا ثابت نویسه *format_time(timestamp_t time, ثابت نویسه *tz_str,
			       صحیح show_raw_time)
{
	ایستا ساختار strbuf time_buf = STRBUF_INIT;

	strbuf_reset(&time_buf);
	اگر (show_raw_time) {
		strbuf_addf(&time_buf, "%"PRItime" %s", time, tz_str);
	}
	وگرنه {
		ثابت نویسه *time_str;
		size_t time_width;
		صحیح tz;
		tz = atoi(tz_str);
		time_str = show_date(time, tz, blame_date_mode);
		strbuf_addstr(&time_buf, time_str);
		/*
		 * Add space paddings to time_buf to display a fixed width
		 * string, and use time_width for display width calibration.
		 */
		برای (time_width = utf8_strwidth(time_str);
		     time_width < blame_date_width;
		     time_width++)
			strbuf_addch(&time_buf, ' ');
	}
	بازگشت time_buf.buf;
}

#define OUTPUT_ANNOTATE_COMPAT      (1U<<0)
#define OUTPUT_LONG_OBJECT_NAME     (1U<<1)
#define OUTPUT_RAW_TIMESTAMP        (1U<<2)
#define OUTPUT_PORCELAIN            (1U<<3)
#define OUTPUT_SHOW_NAME            (1U<<4)
#define OUTPUT_SHOW_عدد          (1U<<5)
#define OUTPUT_SHOW_SCORE           (1U<<6)
#define OUTPUT_NO_AUTHOR            (1U<<7)
#define OUTPUT_SHOW_EMAIL           (1U<<8)
#define OUTPUT_سطر_PORCELAIN       (1U<<9)
#define OUTPUT_COLOR_سطر           (1U<<10)
#define OUTPUT_SHOW_AGE_WITH_COLOR  (1U<<11)

ایستا تهی emit_porcelain_details(ساختار blame_origin *suspect, صحیح repeat)
{
	اگر (emit_one_suspect_detail(suspect, repeat) ||
	    (suspect->commit->object.flags & MORE_THAN_ONE_PATH))
		write_filename_info(suspect);
}

/*
 * Information which needs to be printed per-line goes here. Any
 * information which can be clubbed on a commit/file level, should
 * be printed via 'emit_one_suspect_detail()'.
 */
ایستا تهی emit_porcelain_per_سطر_details(ساختار blame_entry *ent)
{
	اگر (mark_unblamable_سطرها && ent->unblamable)
		puts("unblamable");
	اگر (mark_ignored_سطرها && ent->ignored)
		puts("ignored");
}

ایستا تهی emit_porcelain(ساختار blame_scoreboard *sb, ساختار blame_entry *ent,
			   صحیح opt)
{
	صحیح repeat = opt & OUTPUT_سطر_PORCELAIN;
	صحیح cnt;
	ثابت نویسه *cp;
	ساختار blame_origin *suspect = ent->suspect;
	نویسه hex[GIT_MAX_HEXSZ + 1];

	oid_to_hex_r(hex, &suspect->commit->object.oid);
	printf("%s %d %d %d\n",
	       hex,
	       ent->s_lno + 1,
	       ent->lno + 1,
	       ent->رقم_سطرها);
	emit_porcelain_details(suspect, repeat);
	emit_porcelain_per_سطر_details(ent);

	cp = blame_nth_سطر(sb, ent->lno);
	برای (cnt = 0; cnt < ent->رقم_سطرها; cnt++) {
		نویسه ch;
		اگر (cnt) {
			printf("%s %d %d\n", hex,
			       ent->s_lno + 1 + cnt,
			       ent->lno + 1 + cnt);
			اگر (repeat)
				emit_porcelain_details(suspect, 1);
			emit_porcelain_per_سطر_details(ent);
		}
		putchar('\t');
		انجامبده {
			ch = *cp++;
			putchar(ch);
		} وقتی (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_size);
	}

	اگر (sb->final_buf_size && cp[-1] != '\n')
		putchar('\n');
}

ایستا ساختار color_field {
	timestamp_t hop;
	نویسه col[COLOR_MAXLEN];
} *colorfield;
ایستا صحیح colorfield_nr, colorfield_alloc;

ایستا تهی parse_color_fields(ثابت نویسه *s)
{
	ساختار string_list l = STRING_LIST_INIT_DUP;
	ساختار string_list_item *item;
	شمارش { EXPECT_DATE, EXPECT_COLOR } next = EXPECT_COLOR;

	colorfield_nr = 0;

	/* Ideally this would be stripped and split at the same time? */
	string_list_split(&l, s, ",", -1);
	ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);

	for_each_string_list_item(item, &l) {
		انتخاب (next) {
		حالت EXPECT_DATE:
			colorfield[colorfield_nr].hop = approxidate(item->string);
			next = EXPECT_COLOR;
			colorfield_nr++;
			ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);
			بشکن;
		حالت EXPECT_COLOR:
			اگر (color_parse(item->string, colorfield[colorfield_nr].col))
				die(_("expecting a color: %s"), item->string);
			next = EXPECT_DATE;
			بشکن;
		}
	}

	اگر (next == EXPECT_COLOR)
		die(_("must end with a color"));

	colorfield[colorfield_nr].hop = TIME_MAX;
	string_list_clear(&l, 0);
}

ایستا تهی setup_default_color_by_age(تهی)
{
	parse_color_fields("blue,12 month ago,white,1 month ago,red");
}

ایستا تهی determine_سطر_heat(ساختار commit_info *ci, ثابت نویسه **dest_color)
{
	صحیح i = 0;

	وقتی (i < colorfield_nr && ci->author_time > colorfield[i].hop)
		i++;

	*dest_color = colorfield[i].col;
}

ایستا درونخطی صحیح maybe_putc(صحیح c, FILE *out)
{
	بازگشت out ? putc(c, out) : 0;
}

ایستا size_t print_marks(FILE *out, ثابت ساختار blame_entry *ent, صحیح opt)
{
	size_t len = 0;

	اگر ((ent->suspect->commit->object.flags & UNINTERESTING) &&
	    !blank_boundary && !(opt & OUTPUT_ANNOTATE_COMPAT)) {
		maybe_putc('^', out);
		len++;
	}
	اگر (mark_unblamable_سطرها && ent->unblamable) {
		maybe_putc('*', out);
		len++;
	}
	اگر (mark_ignored_سطرها && ent->ignored) {
		maybe_putc('?', out);
		len++;
	}
	بازگشت len;
}

ایستا size_t شمار_marks(ثابت ساختار blame_entry *ent, صحیح opt)
{
	بازگشت print_marks(پوچ, ent, opt);
}

ایستا تهی emit_other(ساختار blame_scoreboard *sb, ساختار blame_entry *ent,
		       صحیح opt, ساختار blame_entry *prev_ent)
{
	صحیح cnt;
	ثابت نویسه *cp;
	ساختار blame_origin *suspect = ent->suspect;
	ساختار commit_info ci = COMMIT_INFO_INIT;
	نویسه hex[GIT_MAX_HEXSZ + 1];
	صحیح show_raw_time = !!(opt & OUTPUT_RAW_TIMESTAMP);
	ثابت نویسه *default_color = پوچ, *color = پوچ, *reset = پوچ;

	get_commit_info(suspect->commit, &ci);
	oid_to_hex_r(hex, &suspect->commit->object.oid);

	cp = blame_nth_سطر(sb, ent->lno);

	اگر (opt & OUTPUT_SHOW_AGE_WITH_COLOR) {
		determine_سطر_heat(&ci, &default_color);
		color = default_color;
		reset = GIT_COLOR_RESET;
	}

	اگر (abbrev < MINIMUM_ABBREV)
		BUG("abbreviation is smaller than minimum length: %d < %d",
		    abbrev, MINIMUM_ABBREV);

	برای (cnt = 0; cnt < ent->رقم_سطرها; cnt++) {
		نویسه ch;
		size_t length = (opt & OUTPUT_LONG_OBJECT_NAME) ?
			the_hash_algo->hexsz : (size_t) abbrev;

		اگر (opt & OUTPUT_COLOR_سطر) {
			اگر (cnt > 0 ||
			    (prev_ent &&
			     oideq(&suspect->commit->object.oid,
				   &prev_ent->suspect->commit->object.oid))) {
				color = repeated_meta_color;
				reset = GIT_COLOR_RESET;
			} وگرنه  {
				color = default_color ? default_color : پوچ;
				reset = default_color ? GIT_COLOR_RESET : پوچ;
			}
		}
		اگر (color)
			fputs(color, stdout);

		اگر ((suspect->commit->object.flags & UNINTERESTING) &&
		    blank_boundary)
			memset(hex, ' ', strlen(hex));
		length -= print_marks(stdout, ent, opt);

		printf("%.*s", (صحیح)(length < GIT_MAX_HEXSZ ? length : GIT_MAX_HEXSZ), hex);
		اگر (opt & OUTPUT_ANNOTATE_COMPAT) {
			ثابت نویسه *name;
			اگر (opt & OUTPUT_SHOW_EMAIL)
				name = ci.author_mail.buf;
			وگرنه
				name = ci.author.buf;
			printf("\t(%10s\t%10s\t%d)", name,
			       format_time(ci.author_time, ci.author_tz.buf,
					   show_raw_time),
			       ent->lno + 1 + cnt);
		} وگرنه {
			اگر (opt & OUTPUT_SHOW_SCORE)
				printf(" %*d %02d",
				       max_score_digits, ent->score,
				       ent->suspect->refcnt);
			اگر (opt & OUTPUT_SHOW_NAME)
				printf(" %-*.*s", longest_file, longest_file,
				       suspect->path);
			اگر (opt & OUTPUT_SHOW_عدد)
				printf(" %*d", max_orig_digits,
				       ent->s_lno + 1 + cnt);

			اگر (!(opt & OUTPUT_NO_AUTHOR)) {
				ثابت نویسه *name;
				صحیح pad;
				اگر (opt & OUTPUT_SHOW_EMAIL)
					name = ci.author_mail.buf;
				وگرنه
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
		اگر (reset)
			fputs(reset, stdout);
		انجامبده {
			ch = *cp++;
			putchar(ch);
		} وقتی (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_size);
	}

	اگر (sb->final_buf_size && cp[-1] != '\n')
		putchar('\n');

	commit_info_destroy(&ci);
}

ایستا تهی output(ساختار blame_scoreboard *sb, صحیح option)
{
	ساختار blame_entry *ent, *prev_ent = پوچ;

	اگر (option & OUTPUT_PORCELAIN) {
		برای (ent = sb->ent; ent; ent = ent->next) {
			صحیح شمار = 0;
			ساختار blame_origin *suspect;
			ساختار commit *commit = ent->suspect->commit;
			اگر (commit->object.flags & MORE_THAN_ONE_PATH)
				ادامه;
			برای (suspect = get_blame_suspects(commit); suspect; suspect = suspect->next) {
				اگر (suspect->guilty && شمار++) {
					commit->object.flags |= MORE_THAN_ONE_PATH;
					بشکن;
				}
			}
		}
	}

	برای (ent = sb->ent; ent; ent = ent->next) {
		اگر (option & OUTPUT_PORCELAIN)
			emit_porcelain(sb, ent, option);
		وگرنه {
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
ایستا صحیح read_ancestry(ثابت نویسه *graft_file)
{
	FILE *fp = fopen_or_warn(graft_file, "r");
	ساختار strbuf buf = STRBUF_INIT;
	اگر (!fp)
		بازگشت -1;
	وقتی (!strbuf_getwholeline(&buf, fp, '\n')) {
		/* The format is just "Commit Parent1 Parent2 ...\n" */
		ساختار commit_graft *graft = read_graft_سطر(&buf);
		اگر (graft)
			register_commit_graft(the_repository, graft, 0);
	}
	fclose(fp);
	strbuf_release(&buf);
	بازگشت 0;
}

ایستا صحیح update_auto_abbrev(صحیح auto_abbrev, ساختار blame_origin *suspect)
{
	ثابت نویسه *uniq = repo_find_unique_abbrev(the_repository,
						   &suspect->commit->object.oid,
						   auto_abbrev);
	صحیح len = strlen(uniq);
	اگر (auto_abbrev < len)
		بازگشت len;
	بازگشت auto_abbrev;
}

/*
 * How many columns do we need to show line numbers, authors,
 * and filenames?
 */
ایستا تهی find_alignment(ساختار blame_scoreboard *sb, صحیح *option)
{
	صحیح longest_src_سطرها = 0;
	صحیح longest_dst_سطرها = 0;
	unsigned largest_score = 0;
	ساختار blame_entry *e;
	صحیح compute_auto_abbrev = (abbrev < 0);
	صحیح auto_abbrev = DEFAULT_ABBREV;
	size_t max_marks_شمار = 0;

	برای (e = sb->ent; e; e = e->next) {
		ساختار blame_origin *suspect = e->suspect;
		صحیح رقم;
		size_t marks_شمار = شمار_marks(e, *option);

		اگر (max_marks_شمار < marks_شمار)
			max_marks_شمار = marks_شمار;
		اگر (compute_auto_abbrev)
			auto_abbrev = update_auto_abbrev(auto_abbrev, suspect);
		اگر (strcmp(suspect->path, sb->path))
			*option |= OUTPUT_SHOW_NAME;
		رقم = strlen(suspect->path);
		اگر (longest_file < رقم)
			longest_file = رقم;
		اگر (!(suspect->commit->object.flags & METAINFO_SHOWN)) {
			ساختار commit_info ci = COMMIT_INFO_INIT;
			suspect->commit->object.flags |= METAINFO_SHOWN;
			get_commit_info(suspect->commit, &ci);
			اگر (*option & OUTPUT_SHOW_EMAIL)
				رقم = utf8_strwidth(ci.author_mail.buf);
			وگرنه
				رقم = utf8_strwidth(ci.author.buf);
			اگر (longest_author < رقم)
				longest_author = رقم;
			commit_info_destroy(&ci);
		}
		رقم = e->s_lno + e->رقم_سطرها;
		اگر (longest_src_سطرها < رقم)
			longest_src_سطرها = رقم;
		رقم = e->lno + e->رقم_سطرها;
		اگر (longest_dst_سطرها < رقم)
			longest_dst_سطرها = رقم;
		اگر (largest_score < blame_entry_score(sb, e))
			largest_score = blame_entry_score(sb, e);
	}
	max_orig_digits = decimal_width(longest_src_سطرها);
	max_digits = decimal_width(longest_dst_سطرها);
	max_score_digits = decimal_width(largest_score);

	اگر (compute_auto_abbrev)
		abbrev = auto_abbrev;
	اگر (abbrev < (صحیح)the_hash_algo->hexsz) {
		abbrev += max_marks_شمار;
		اگر (abbrev > (صحیح)the_hash_algo->hexsz)
			abbrev = the_hash_algo->hexsz;
	}
}

ایستا تهی sanity_check_on_fail(ساختار blame_scoreboard *sb, صحیح baa)
{
	صحیح opt = OUTPUT_SHOW_SCORE | OUTPUT_SHOW_عدد | OUTPUT_SHOW_NAME;
	find_alignment(sb, &opt);
	output(sb, opt);
	die("Baa %d!", baa);
}

ایستا unsigned parse_score(ثابت نویسه *arg)
{
	نویسه *end;
	unsigned long score = strtoul(arg, &end, 10);
	اگر (*end)
		بازگشت 0;
	بازگشت score;
}

ایستا نویسه *add_prefix(ثابت نویسه *prefix, ثابت نویسه *path)
{
	بازگشت prefix_path(the_repository, prefix, prefix ? strlen(prefix) : 0, path);
}

ایستا صحیح git_blame_config(ثابت نویسه *var, ثابت نویسه *مقدار,
			    ثابت ساختار config_context *ctx, تهی *cb)
{
	اگر (!strcmp(var, "blame.showroot")) {
		show_ریشه = git_config_bool(var, مقدار);
		بازگشت 0;
	}
	اگر (!strcmp(var, "blame.blankboundary")) {
		blank_boundary = git_config_bool(var, مقدار);
		بازگشت 0;
	}
	اگر (!strcmp(var, "blame.showemail")) {
		صحیح *output_option = cb;
		اگر (git_config_bool(var, مقدار))
			*output_option |= OUTPUT_SHOW_EMAIL;
		وگرنه
			*output_option &= ~OUTPUT_SHOW_EMAIL;
		بازگشت 0;
	}
	اگر (!strcmp(var, "blame.date")) {
		اگر (!مقدار)
			بازگشت config_error_nonbool(var);
		parse_date_format(مقدار, &blame_date_mode);
		بازگشت 0;
	}
	اگر (!strcmp(var, "blame.ignorerevsfile")) {
		نویسه *str;
		صحیح ret;

		ret = git_config_pathname(&str, var, مقدار);
		اگر (ret)
			بازگشت ret;
		اگر (str)
			string_list_درج(&ignore_revs_file_list, str);
		آزاد(str);
		بازگشت 0;
	}
	اگر (!strcmp(var, "blame.markunblamablelines")) {
		mark_unblamable_سطرها = git_config_bool(var, مقدار);
		بازگشت 0;
	}
	اگر (!strcmp(var, "blame.markignoredlines")) {
		mark_ignored_سطرها = git_config_bool(var, مقدار);
		بازگشت 0;
	}
	اگر (!strcmp(var, "color.blame.repeatedlines")) {
		اگر (color_parse_mem(مقدار, strlen(مقدار), repeated_meta_color))
			warning(_("invalid value for '%s': '%s'"),
				"color.blame.repeatedLines", مقدار);
		بازگشت 0;
	}
	اگر (!strcmp(var, "color.blame.highlightrecent")) {
		parse_color_fields(مقدار);
		بازگشت 0;
	}

	اگر (!strcmp(var, "blame.coloring")) {
		اگر (!مقدار)
			بازگشت config_error_nonbool(var);
		اگر (!strcmp(مقدار, "repeatedLines")) {
			coloring_mode |= OUTPUT_COLOR_سطر;
		} وگرنه اگر (!strcmp(مقدار, "highlightRecent")) {
			coloring_mode |= OUTPUT_SHOW_AGE_WITH_COLOR;
		} وگرنه اگر (!strcmp(مقدار, "none")) {
			coloring_mode &= ~(OUTPUT_COLOR_سطر |
					    OUTPUT_SHOW_AGE_WITH_COLOR);
		} وگرنه {
			warning(_("invalid value for '%s': '%s'"),
				"blame.coloring", مقدار);
			بازگشت 0;
		}
	}

	اگر (!strcmp(var, "diff.algorithm")) {
		long diff_algorithm;
		اگر (!مقدار)
			بازگشت config_error_nonbool(var);
		diff_algorithm = parse_algorithm_مقدار(مقدار);
		اگر (diff_algorithm < 0)
			بازگشت error(_("unknown value for config '%s': %s"),
				     var, مقدار);
		xdl_opts &= ~XDF_DIFF_ALGORITHM_MASK;
		xdl_opts |= diff_algorithm;
		بازگشت 0;
	}

	اگر (git_diff_heuristic_config(var, مقدار, cb) < 0)
		بازگشت -1;
	اگر (userdiff_config(var, مقدار) < 0)
		بازگشت -1;

	بازگشت git_default_config(var, مقدار, ctx, cb);
}

ایستا صحیح blame_copy_callback(ثابت ساختار option *option, ثابت نویسه *arg, صحیح unset)
{
	صحیح *opt = option->مقدار;

	BUG_ON_OPT_NEG(unset);

	/*
	 * -C enables copy from removed files;
	 * -C -C enables copy from existing files, but only
	 *       when blaming a new file;
	 * -C -C -C enables copy from existing files for
	 *          everybody
	 */
	اگر (*opt & PICKAXE_BLAME_COPY_HARDER)
		*opt |= PICKAXE_BLAME_COPY_HARDEST;
	اگر (*opt & PICKAXE_BLAME_COPY)
		*opt |= PICKAXE_BLAME_COPY_HARDER;
	*opt |= PICKAXE_BLAME_COPY | PICKAXE_BLAME_MOVE;

	اگر (arg)
		blame_copy_score = parse_score(arg);
	بازگشت 0;
}

ایستا صحیح blame_move_callback(ثابت ساختار option *option, ثابت نویسه *arg, صحیح unset)
{
	صحیح *opt = option->مقدار;

	BUG_ON_OPT_NEG(unset);

	*opt |= PICKAXE_BLAME_MOVE;

	اگر (arg)
		blame_move_score = parse_score(arg);
	بازگشت 0;
}

ایستا صحیح blame_diff_algorithm_minimal(ثابت ساختار option *option,
					ثابت نویسه *arg, صحیح unset)
{
	صحیح *opt = option->مقدار;

	BUG_ON_OPT_ARG(arg);

	*opt &= ~XDF_DIFF_ALGORITHM_MASK;
	اگر (!unset)
		*opt |= XDF_NEED_MINIMAL;

	بازگشت 0;
}

ایستا صحیح blame_diff_algorithm_callback(ثابت ساختار option *option,
					 ثابت نویسه *arg, صحیح unset)
{
	صحیح *opt = option->مقدار;
	long مقدار = parse_algorithm_مقدار(arg);

	BUG_ON_OPT_NEG(unset);

	اگر (مقدار < 0)
		بازگشت error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	*opt &= ~XDF_DIFF_ALGORITHM_MASK;
	*opt |= مقدار;

	بازگشت 0;
}

ایستا صحیح is_a_rev(ثابت نویسه *name)
{
	ساختار object_id oid;

	اگر (repo_get_oid(the_repository, name, &oid))
		بازگشت 0;
	بازگشت OBJ_NONE < odb_read_object_info(the_repository->objects, &oid, پوچ);
}

ایستا صحیح peel_to_commit_oid(ساختار object_id *oid_ret, تهی *cbdata)
{
	ساختار repository *r = ((ساختار blame_scoreboard *)cbdata)->repo;
	ساختار object_id oid;

	oidcpy(&oid, oid_ret);
	وقتی (1) {
		ساختار object *obj;
		صحیح kind = odb_read_object_info(r->objects, &oid, پوچ);
		اگر (kind == OBJ_COMMIT) {
			oidcpy(oid_ret, &oid);
			بازگشت 0;
		}
		اگر (kind != OBJ_TAG)
			بازگشت -1;
		obj = deref_tag(r, parse_object(r, &oid), پوچ, 0);
		اگر (!obj)
			بازگشت -1;
		oidcpy(&oid, &obj->oid);
	}
}

ایستا تهی build_ignorelist(ساختار blame_scoreboard *sb,
			     ساختار string_list *ignore_revs_file_list,
			     ساختار string_list *ignore_rev_list)
{
	ساختار string_list_item *i;
	ساختار object_id oid;

	oidset_init(&sb->ignore_list, 0);
	for_each_string_list_item(i, ignore_revs_file_list) {
		اگر (!strcmp(i->string, ""))
			oidset_clear(&sb->ignore_list);
		وگرنه
			oidset_parse_file_carefully(&sb->ignore_list, i->string,
						    the_repository->hash_algo,
						    peel_to_commit_oid, sb);
	}
	for_each_string_list_item(i, ignore_rev_list) {
		اگر (repo_get_oid_committish(the_repository, i->string, &oid) ||
		    peel_to_commit_oid(&oid, sb))
			die(_("cannot find revision %s to ignore"), i->string);
		oidset_درج(&sb->ignore_list, &oid);
	}
}

صحیح cmd_blame(صحیح argc,
	      ثابت نویسه **argv,
	      ثابت نویسه *prefix,
	      ساختار repository *repo UNUSED)
{
	ساختار rev_info revs;
	نویسه *path = پوچ;
	ساختار blame_scoreboard sb;
	ساختار blame_origin *o;
	ساختار blame_entry *ent = پوچ;
	long dashdash_pos, lno;
	ساختار progress_info pi = { پوچ, 0 };

	ساختار string_list range_list = STRING_LIST_INIT_NODUP;
	ساختار string_list ignore_rev_list = STRING_LIST_INIT_NODUP;
	صحیح output_option = 0, opt = 0;
	صحیح show_stats = 0;
	ثابت نویسه *revs_file = پوچ;
	ثابت نویسه *contents_from = پوچ;
	ثابت ساختار option options[] = {
		OPT_BOOL(0, "incremental", &incremental, N_("show blame entries as we find them, incrementally")),
		OPT_BOOL('b', پوچ, &blank_boundary, N_("do not show object names of boundary commits (Default: off)")),
		OPT_BOOL(0, "root", &show_ریشه, N_("do not treat root commits as boundaries (Default: off)")),
		OPT_BOOL(0, "show-stats", &show_stats, N_("show work cost statistics")),
		OPT_BOOL(0, "progress", &show_progress, N_("force progress reporting")),
		OPT_BIT(0, "score-debug", &output_option, N_("show output score for blame entries"), OUTPUT_SHOW_SCORE),
		OPT_BIT('f', "show-name", &output_option, N_("show original filename (Default: auto)"), OUTPUT_SHOW_NAME),
		OPT_BIT('n', "show-number", &output_option, N_("show original linenumber (Default: off)"), OUTPUT_SHOW_عدد),
		OPT_BIT('p', "porcelain", &output_option, N_("show in a format designed for machine consumption"), OUTPUT_PORCELAIN),
		OPT_BIT(0, "line-porcelain", &output_option, N_("show porcelain format with per-line commit information"), OUTPUT_PORCELAIN|OUTPUT_سطر_PORCELAIN),
		OPT_BIT('c', پوچ, &output_option, N_("use the same output mode as git-annotate (Default: off)"), OUTPUT_ANNOTATE_COMPAT),
		OPT_BIT('t', پوچ, &output_option, N_("show raw timestamp (Default: off)"), OUTPUT_RAW_TIMESTAMP),
		OPT_BIT('l', پوچ, &output_option, N_("show long commit SHA1 (Default: off)"), OUTPUT_LONG_OBJECT_NAME),
		OPT_BIT('s', پوچ, &output_option, N_("suppress author name and timestamp (Default: off)"), OUTPUT_NO_AUTHOR),
		OPT_BIT('e', "show-email", &output_option, N_("show author email instead of name (Default: off)"), OUTPUT_SHOW_EMAIL),
		OPT_BIT('w', پوچ, &xdl_opts, N_("ignore whitespace differences"), XDF_IGNORE_WHITESPACE),
		OPT_CALLBACK_F(0, "diff-algorithm", &xdl_opts, N_("<algorithm>"),
			       N_("choose a diff algorithm"),
			       PARSE_OPT_NONEG, blame_diff_algorithm_callback),
		OPT_STRING_LIST(0, "ignore-rev", &ignore_rev_list, N_("rev"), N_("ignore <rev> when blaming")),
		OPT_STRING_LIST(0, "ignore-revs-file", &ignore_revs_file_list, N_("file"), N_("ignore revisions from <file>")),
		OPT_BIT(0, "color-lines", &output_option, N_("color redundant metadata from previous line differently"), OUTPUT_COLOR_سطر),
		OPT_BIT(0, "color-by-age", &output_option, N_("color lines by age"), OUTPUT_SHOW_AGE_WITH_COLOR),
		OPT_CALLBACK_F(0, "minimal", &xdl_opts, پوچ,
			       N_("spend extra cycles to find a better match"),
			       PARSE_OPT_NOARG | PARSE_OPT_HIDDEN, blame_diff_algorithm_minimal),
		OPT_STRING('S', پوچ, &revs_file, N_("file"), N_("use revisions from <file> instead of calling git-rev-list")),
		OPT_STRING(0, "contents", &contents_from, N_("file"), N_("use <file>'s contents as the final image")),
		OPT_CALLBACK_F('C', پوچ, &opt, N_("score"), N_("find line copies within and across files"), PARSE_OPT_OPTARG, blame_copy_callback),
		OPT_CALLBACK_F('M', پوچ, &opt, N_("score"), N_("find line movements within and across files"), PARSE_OPT_OPTARG, blame_move_callback),
		OPT_STRING_LIST('L', پوچ, &range_list, N_("range"),
				N_("process only line range <start>,<end> or function :<funcname>")),
		OPT__ABBREV(&abbrev),
		OPT_END()
	};

	ساختار parse_opt_ctx_t ctx;
	صحیح cmd_is_annotate = !strcmp(argv[0], "annotate");
	ساختار range_set ranges;
	unsigned صحیح range_i;
	long anchor;
	long رقم_سطرها = 0;
	ثابت نویسه *str_usage = cmd_is_annotate ? annotate_usage : blame_usage;
	ثابت نویسه *ثابت *opt_usage = cmd_is_annotate ? annotate_opt_usage : blame_opt_usage;

	setup_default_color_by_age();
	repo_config(the_repository, git_blame_config, &output_option);
	repo_init_revisions(the_repository, &revs, پوچ);
	revs.date_mode = blame_date_mode;
	revs.diffopt.flags.allow_textconv = 1;
	revs.diffopt.flags.follow_renames = 1;

	save_commit_buffer = 0;
	dashdash_pos = 0;
	show_progress = -1;

	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);
	برای (;;) {
		انتخاب (parse_options_step(&ctx, options, opt_usage)) {
		حالت PARSE_OPT_NON_OPTION:
		حالت PARSE_OPT_UNKNOWN:
			بشکن;
		حالت PARSE_OPT_HELP:
			exit(0);
		حالت PARSE_OPT_HELP_ERROR:
		حالت PARSE_OPT_ERROR:
		حالت PARSE_OPT_SUBCOMMAND:
			exit(129);
		حالت PARSE_OPT_COMPLETE:
			exit(0);
		حالت PARSE_OPT_DONE:
			اگر (ctx.argv[0])
				dashdash_pos = ctx.cpidx;
			برو parse_done;
		}

		اگر (!strcmp(ctx.argv[0], "--reverse")) {
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

	اگر (incremental || (output_option & OUTPUT_PORCELAIN)) {
		اگر (show_progress > 0)
			die(_("--progress can't be used with --incremental or porcelain formats"));
		show_progress = 0;
	} وگرنه اگر (show_progress < 0)
		show_progress = isatty(2);

	اگر (!abbrev)
		abbrev = the_hash_algo->hexsz;

	اگر (revs_file && read_ancestry(revs_file))
		die_errno("reading graft file '%s' failed", revs_file);

	اگر (cmd_is_annotate) {
		output_option |= OUTPUT_ANNOTATE_COMPAT;
		blame_date_mode.type = DATE_ISO8601;
	} وگرنه {
		blame_date_mode = revs.date_mode;
	}

	/* The maximum width used to show the dates */
	انتخاب (blame_date_mode.type) {
	حالت DATE_RFC2822:
		blame_date_width = اندازه("Thu, 19 Oct 2006 16:00:04 -0700");
		بشکن;
	حالت DATE_ISO8601_STRICT:
		blame_date_width = اندازه("2006-10-19T16:00:04-07:00");
		بشکن;
	حالت DATE_ISO8601:
		blame_date_width = اندازه("2006-10-19 16:00:04 -0700");
		بشکن;
	حالت DATE_RAW:
		blame_date_width = اندازه("1161298804 -0700");
		بشکن;
	حالت DATE_UNIX:
		blame_date_width = اندازه("1161298804");
		بشکن;
	حالت DATE_SHORT:
		blame_date_width = اندازه("2006-10-19");
		بشکن;
	حالت DATE_RELATIVE:
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
		بشکن;
	حالت DATE_HUMAN:
		/* If the year is shown, no time is shown */
		blame_date_width = اندازه("Thu Oct 19 16:00");
		بشکن;
	حالت DATE_NORMAL:
		blame_date_width = اندازه("Thu Oct 19 16:00:04 2006 -0700");
		بشکن;
	حالت DATE_STRFTIME:
		blame_date_width = strlen(show_date(0, 0, blame_date_mode)) + 1; /* add the null */
		بشکن;
	}
	blame_date_width -= 1; /* strip the null */

	اگر (revs.diffopt.flags.find_copies_harder)
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
	اگر (dashdash_pos) {
		انتخاب (argc - dashdash_pos - 1) {
		حالت 2: /* (1b) */
			اگر (argc != 4)
				usage_with_options(opt_usage, options);
			/* reorder for the new way: <rev> -- <path> */
			argv[1] = argv[3];
			argv[3] = argv[2];
			argv[2] = "--";
			/* FALLTHROUGH */
		حالت 1: /* (1a) */
			path = add_prefix(prefix, argv[--argc]);
			argv[argc] = پوچ;
			بشکن;
		پیشفرض:
			usage_with_options(opt_usage, options);
		}
	} وگرنه {
		اگر (argc < 2)
			usage_with_options(opt_usage, options);
		اگر (argc == 3 && is_a_rev(argv[argc - 1])) { /* (2b) */
			path = add_prefix(prefix, argv[1]);
			argv[1] = argv[2];
		} وگرنه {	/* (2a) */
			اگر (argc == 2 && is_a_rev(argv[1]) && !repo_get_work_درخت(the_repository))
				die("missing <path> to blame");
			path = add_prefix(prefix, argv[argc - 1]);
		}
		argv[argc - 1] = "--";
	}

	revs.disable_stdin = 1;
	setup_revisions(argc, argv, &revs, پوچ);
	اگر (!revs.pending.nr && is_bare_repository(the_repository)) {
		ساختار commit *head_commit;
		ساختار object_id head_oid;

		اگر (!refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", RESOLVE_REF_READING,
					     &head_oid, پوچ) ||
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
	اگر (!(opt & PICKAXE_BLAME_COPY))
		setup_blame_bloom_data(&sb);

	lno = sb.رقم_سطرها;

	اگر (lno && !range_list.nr)
		string_list_append(&range_list, "1");

	anchor = 1;
	range_set_init(&ranges, range_list.nr);
	برای (range_i = 0; range_i < range_list.nr; ++range_i) {
		long bottom, بالاترین;
		اگر (parse_range_arg(range_list.اقلام[range_i].string,
				    nth_سطر_cb, &sb, lno, anchor,
				    &bottom, &بالاترین, sb.path,
				    the_repository->index))
			usage(str_usage);
		اگر ((!lno && (بالاترین || bottom)) || lno < bottom)
			die(Q_("file %s has only %lu line",
			       "file %s has only %lu lines",
			       lno), sb.path, lno);
		اگر (bottom < 1)
			bottom = 1;
		اگر (بالاترین < 1 || lno < بالاترین)
			بالاترین = lno;
		bottom--;
		range_set_append_unsafe(&ranges, bottom, بالاترین);
		anchor = بالاترین + 1;
	}
	sort_and_merge_range_set(&ranges);

	برای (range_i = ranges.nr; range_i > 0; --range_i) {
		ثابت ساختار range *r = &ranges.ranges[range_i - 1];
		ent = blame_entry_prepend(ent, r->start, r->end, o);
		رقم_سطرها += (r->end - r->start);
	}
	اگر (!رقم_سطرها)
		رقم_سطرها = sb.رقم_سطرها;

	o->suspects = ent;
	prio_queue_put(&sb.commits, o->commit);

	blame_origin_decref(o);

	range_set_release(&ranges);
	string_list_clear(&range_list, 0);

	sb.ent = پوچ;

	اگر (blame_move_score)
		sb.move_score = blame_move_score;
	اگر (blame_copy_score)
		sb.copy_score = blame_copy_score;

	sb.debug = DEBUG_BLAME;
	sb.on_sanity_fail = &sanity_check_on_fail;

	sb.show_ریشه = show_ریشه;
	sb.xdl_opts = xdl_opts;
	sb.no_whole_file_rename = no_whole_file_rename;

	read_mailmap(the_repository, &mailmap);

	sb.found_guilty_entry = &found_guilty_entry;
	sb.found_guilty_entry_data = &pi;
	اگر (show_progress)
		pi.progress = start_delayed_progress(the_repository,
						     _("Blaming lines"),
						     رقم_سطرها);

	assign_blame(&sb, opt);

	stop_progress(&pi.progress);

	اگر (!incremental)
		setup_pager(the_repository);
	وگرنه
		برو cleanup;

	blame_sort_final(&sb);

	blame_coalesce(&sb);

	اگر (!(output_option & (OUTPUT_COLOR_سطر | OUTPUT_SHOW_AGE_WITH_COLOR)))
		output_option |= coloring_mode;

	اگر (!(output_option & OUTPUT_PORCELAIN)) {
		find_alignment(&sb, &output_option);
		اگر (!*repeated_meta_color &&
		    (output_option & OUTPUT_COLOR_سطر))
			xsnprintf(repeated_meta_color,
				  اندازه(repeated_meta_color),
				  "%s", GIT_COLOR_CYAN);
	}
	اگر (output_option & OUTPUT_ANNOTATE_COMPAT)
		output_option &= ~(OUTPUT_COLOR_سطر | OUTPUT_SHOW_AGE_WITH_COLOR);

	output(&sb, output_option);

	اگر (show_stats) {
		printf("num read blob: %d\n", sb.رقم_read_blob);
		printf("num get patch: %d\n", sb.رقم_get_patch);
		printf("num commits: %d\n", sb.رقم_commits);
	}

cleanup:
	برای (ent = sb.ent; ent; ) {
		ساختار blame_entry *e = ent->next;
		آزاد(ent);
		ent = e;
	}

	آزاد(path);
	cleanup_scoreboard(&sb);
	release_revisions(&revs);
	بازگشت 0;
}
