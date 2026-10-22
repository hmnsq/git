/*
 * Blame
 *
 * Copyright (c) 2006, 2014 by its authors
 * See COPYING for licensing conditions
 */

#define USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "color.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "commit.h"
#ضمن "diff.h"
#ضمن "revision.h"
#ضمن "quote.h"
#ضمن "string-list.h"
#ضمن "mailmap.h"
#ضمن "parse-options.h"
#ضمن "prio-queue.h"
#ضمن "utf8.h"
#ضمن "userdiff.h"
#ضمن "line-range.h"
#ضمن "line-log.h"
#ضمن "progress.h"
#ضمن "object-name.h"
#ضمن "odb.h"
#ضمن "pager.h"
#ضمن "blame.h"
#ضمن "refs.h"
#ضمن "setup.h"
#ضمن "tag.h"
#ضمن "write-or-die.h"

ساكن ثابت حرف blame_usage[] = N_("git blame [<options>] [<rev-opts>] [<rev>] [--] <file>");
ساكن ثابت حرف annotate_usage[] = N_("git annotate [<options>] [<rev-opts>] [<rev>] [--] <file>");

ساكن ثابت حرف *ثابت blame_opt_usage[] = {
	blame_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	عدم
};

ساكن ثابت حرف *ثابت annotate_opt_usage[] = {
	annotate_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	عدم
};

ساكن صحيح longest_file;
ساكن صحيح longest_author;
ساكن صحيح max_orig_digits;
ساكن صحيح max_digits;
ساكن صحيح max_score_digits;
ساكن صحيح show_يىلتىز;
ساكن صحيح reverse;
ساكن صحيح blank_boundary;
ساكن صحيح incremental;
ساكن صحيح xdl_opts;
ساكن صحيح abbrev = -1;
ساكن صحيح no_whole_file_rename;
ساكن صحيح show_progress;
ساكن حرف repeated_meta_color[COLOR_MAXLEN];
ساكن صحيح coloring_mode;
ساكن بنية string_list ignore_revs_file_list = STRING_LIST_INIT_DUP;
ساكن صحيح mark_unblamable_قۇرلار;
ساكن صحيح mark_ignored_قۇرلار;

ساكن بنية date_mode blame_date_mode = { DATE_ISO8601 };
ساكن size_t blame_date_width;

ساكن بنية string_list mailmap = STRING_LIST_INIT_NODUP;

#ifndef DEBUG_BLAME
#define DEBUG_BLAME 0
#endif

ساكن unsigned blame_move_score;
ساكن unsigned blame_copy_score;

/* Remember to update object flag allocation in object.h */
#define METAINFO_SHOWN		(1u<<12)
#define MORE_THAN_ONE_PATH	(1u<<13)

بنية progress_info {
	بنية progress *progress;
	صحيح blamed_قۇرلار;
};

ساكن ثابت حرف *nth_قۇر_cb(فراغ *data, long lno)
{
	أرجع blame_nth_قۇر((بنية blame_scoreboard *)data, lno);
}

/*
 * Information on commits, used for output.
 */
بنية commit_info {
	بنية strbuf author;
	بنية strbuf author_mail;
	timestamp_t author_time;
	بنية strbuf author_tz;

	/* filled only when asked for details */
	بنية strbuf committer;
	بنية strbuf committer_mail;
	timestamp_t committer_time;
	بنية strbuf committer_tz;

	بنية strbuf summary;
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
ساكن فراغ get_ac_قۇر(ثابت حرف *inbuf, ثابت حرف *what,
	بنية strbuf *name, بنية strbuf *mail,
	timestamp_t *time, بنية strbuf *tz)
{
	بنية ident_split ident;
	size_t len, maillen, namelen;
	ثابت حرف *tmp, *endp;
	ثابت حرف *namebuf, *mailbuf;

	tmp = strstr(inbuf, what);
	إذا (!tmp)
		اذهب error_out;
	tmp += strlen(what);
	endp = strchr(tmp, '\n');
	إذا (!endp)
		len = strlen(tmp);
	وإلا
		len = endp - tmp;

	إذا (split_ident_قۇر(&ident, tmp, len)) {
	error_out:
		/* Ugh */
		tmp = "(unknown)";
		strbuf_addstr(name, tmp);
		strbuf_addstr(mail, tmp);
		strbuf_addstr(tz, tmp);
		*time = 0;
		أرجع;
	}

	namelen = ident.name_end - ident.name_begin;
	namebuf = ident.name_begin;

	maillen = ident.mail_end - ident.mail_begin;
	mailbuf = ident.mail_begin;

	إذا (ident.date_begin && ident.date_end)
		*time = strtoul(ident.date_begin, عدم, 10);
	وإلا
		*time = 0;

	إذا (ident.tz_begin && ident.tz_end)
		strbuf_add(tz, ident.tz_begin, ident.tz_end - ident.tz_begin);
	وإلا
		strbuf_addstr(tz, "(unknown)");

	/*
	 * Now, convert both name and e-mail using mailmap
	 */
	map_user(&mailmap, &mailbuf, &maillen,
		 &namebuf, &namelen);

	strbuf_addf(mail, "<%.*s>", (صحيح)maillen, mailbuf);
	strbuf_add(name, namebuf, namelen);
}

ساكن فراغ commit_info_destroy(بنية commit_info *ci)
{

	strbuf_release(&ci->author);
	strbuf_release(&ci->author_mail);
	strbuf_release(&ci->author_tz);
	strbuf_release(&ci->committer);
	strbuf_release(&ci->committer_mail);
	strbuf_release(&ci->committer_tz);
	strbuf_release(&ci->summary);
}

ساكن فراغ get_commit_info(بنية commit *commit, بنية commit_info *ret)
{
	صحيح len;
	ثابت حرف *subject, *encoding;
	ثابت حرف *message;

	encoding = get_log_output_encoding();
	message = repo_logmsg_reencode(the_repository, commit, عدم, encoding);
	get_ac_قۇر(message, "\nauthor ",
		    &ret->author, &ret->author_mail,
		    &ret->author_time, &ret->author_tz);

	get_ac_قۇر(message, "\ncommitter ",
		    &ret->committer, &ret->committer_mail,
		    &ret->committer_time, &ret->committer_tz);

	len = find_commit_subject(message, &subject);
	إذا (len)
		strbuf_add(&ret->summary, subject, len);
	وإلا
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
ساكن فراغ write_filename_info(بنية blame_origin *suspect)
{
	إذا (suspect->previous) {
		بنية blame_origin *prev = suspect->previous;
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
ساكن صحيح emit_one_suspect_detail(بنية blame_origin *suspect, صحيح repeat)
{
	بنية commit_info ci = COMMIT_INFO_INIT;

	إذا (!repeat && (suspect->commit->object.flags & METAINFO_SHOWN))
		أرجع 0;

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
	إذا (suspect->commit->object.flags & UNINTERESTING)
		printf("boundary\n");

	commit_info_destroy(&ci);

	أرجع 1;
}

/*
 * The blame_entry is found to be guilty for the range.
 * Show it in incremental output.
 */
ساكن فراغ found_guilty_entry(بنية blame_entry *ent, فراغ *data)
{
	بنية progress_info *pi = (بنية progress_info *)data;

	إذا (incremental) {
		بنية blame_origin *suspect = ent->suspect;

		printf("%s %d %d %d\n",
		       oid_to_hex(&suspect->commit->object.oid),
		       ent->s_lno + 1, ent->lno + 1, ent->رەقەم_قۇرلار);
		emit_one_suspect_detail(suspect, 0);
		write_filename_info(suspect);
		maybe_flush_or_die(stdout, "stdout");
	}
	pi->blamed_قۇرلار += ent->رەقەم_قۇرلار;
	display_progress(pi->progress, pi->blamed_قۇرلار);
}

ساكن ثابت حرف *format_time(timestamp_t time, ثابت حرف *tz_str,
			       صحيح show_raw_time)
{
	ساكن بنية strbuf time_buf = STRBUF_INIT;

	strbuf_reset(&time_buf);
	إذا (show_raw_time) {
		strbuf_addf(&time_buf, "%"PRItime" %s", time, tz_str);
	}
	وإلا {
		ثابت حرف *time_str;
		size_t time_width;
		صحيح tz;
		tz = atoi(tz_str);
		time_str = show_date(time, tz, blame_date_mode);
		strbuf_addstr(&time_buf, time_str);
		/*
		 * Add space paddings to time_buf to display a fixed width
		 * string, and use time_width for display width calibration.
		 */
		لكل (time_width = utf8_strwidth(time_str);
		     time_width < blame_date_width;
		     time_width++)
			strbuf_addch(&time_buf, ' ');
	}
	أرجع time_buf.buf;
}

#define OUTPUT_ANNOTATE_COMPAT      (1U<<0)
#define OUTPUT_LONG_OBJECT_NAME     (1U<<1)
#define OUTPUT_RAW_TIMESTAMP        (1U<<2)
#define OUTPUT_PORCELAIN            (1U<<3)
#define OUTPUT_SHOW_NAME            (1U<<4)
#define OUTPUT_SHOW_سان          (1U<<5)
#define OUTPUT_SHOW_SCORE           (1U<<6)
#define OUTPUT_NO_AUTHOR            (1U<<7)
#define OUTPUT_SHOW_EMAIL           (1U<<8)
#define OUTPUT_قۇر_PORCELAIN       (1U<<9)
#define OUTPUT_COLOR_قۇر           (1U<<10)
#define OUTPUT_SHOW_AGE_WITH_COLOR  (1U<<11)

ساكن فراغ emit_porcelain_details(بنية blame_origin *suspect, صحيح repeat)
{
	إذا (emit_one_suspect_detail(suspect, repeat) ||
	    (suspect->commit->object.flags & MORE_THAN_ONE_PATH))
		write_filename_info(suspect);
}

/*
 * Information which needs to be printed per-line goes here. Any
 * information which can be clubbed on a commit/file level, should
 * be printed via 'emit_one_suspect_detail()'.
 */
ساكن فراغ emit_porcelain_per_قۇر_details(بنية blame_entry *ent)
{
	إذا (mark_unblamable_قۇرلار && ent->unblamable)
		puts("unblamable");
	إذا (mark_ignored_قۇرلار && ent->ignored)
		puts("ignored");
}

ساكن فراغ emit_porcelain(بنية blame_scoreboard *sb, بنية blame_entry *ent,
			   صحيح opt)
{
	صحيح repeat = opt & OUTPUT_قۇر_PORCELAIN;
	صحيح cnt;
	ثابت حرف *cp;
	بنية blame_origin *suspect = ent->suspect;
	حرف hex[GIT_MAX_HEXSZ + 1];

	oid_to_hex_r(hex, &suspect->commit->object.oid);
	printf("%s %d %d %d\n",
	       hex,
	       ent->s_lno + 1,
	       ent->lno + 1,
	       ent->رەقەم_قۇرلار);
	emit_porcelain_details(suspect, repeat);
	emit_porcelain_per_قۇر_details(ent);

	cp = blame_nth_قۇر(sb, ent->lno);
	لكل (cnt = 0; cnt < ent->رەقەم_قۇرلار; cnt++) {
		حرف ch;
		إذا (cnt) {
			printf("%s %d %d\n", hex,
			       ent->s_lno + 1 + cnt,
			       ent->lno + 1 + cnt);
			إذا (repeat)
				emit_porcelain_details(suspect, 1);
			emit_porcelain_per_قۇر_details(ent);
		}
		putchar('\t');
		افعل {
			ch = *cp++;
			putchar(ch);
		} طالما (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_size);
	}

	إذا (sb->final_buf_size && cp[-1] != '\n')
		putchar('\n');
}

ساكن بنية color_field {
	timestamp_t hop;
	حرف col[COLOR_MAXLEN];
} *colorfield;
ساكن صحيح colorfield_nr, colorfield_alloc;

ساكن فراغ parse_color_fields(ثابت حرف *s)
{
	بنية string_list l = STRING_LIST_INIT_DUP;
	بنية string_list_item *item;
	تعداد { EXPECT_DATE, EXPECT_COLOR } next = EXPECT_COLOR;

	colorfield_nr = 0;

	/* Ideally this would be stripped and split at the same time? */
	string_list_split(&l, s, ",", -1);
	ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);

	for_each_string_list_item(item, &l) {
		بدل (next) {
		حالة EXPECT_DATE:
			colorfield[colorfield_nr].hop = approxidate(item->string);
			next = EXPECT_COLOR;
			colorfield_nr++;
			ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);
			اكسر;
		حالة EXPECT_COLOR:
			إذا (color_parse(item->string, colorfield[colorfield_nr].col))
				die(_("expecting a color: %s"), item->string);
			next = EXPECT_DATE;
			اكسر;
		}
	}

	إذا (next == EXPECT_COLOR)
		die(_("must end with a color"));

	colorfield[colorfield_nr].hop = TIME_MAX;
	string_list_clear(&l, 0);
}

ساكن فراغ setup_default_color_by_age(فراغ)
{
	parse_color_fields("blue,12 month ago,white,1 month ago,red");
}

ساكن فراغ determine_قۇر_heat(بنية commit_info *ci, ثابت حرف **dest_color)
{
	صحيح i = 0;

	طالما (i < colorfield_nr && ci->author_time > colorfield[i].hop)
		i++;

	*dest_color = colorfield[i].col;
}

ساكن مضمن صحيح maybe_putc(صحيح c, FILE *out)
{
	أرجع out ? putc(c, out) : 0;
}

ساكن size_t print_marks(FILE *out, ثابت بنية blame_entry *ent, صحيح opt)
{
	size_t len = 0;

	إذا ((ent->suspect->commit->object.flags & UNINTERESTING) &&
	    !blank_boundary && !(opt & OUTPUT_ANNOTATE_COMPAT)) {
		maybe_putc('^', out);
		len++;
	}
	إذا (mark_unblamable_قۇرلار && ent->unblamable) {
		maybe_putc('*', out);
		len++;
	}
	إذا (mark_ignored_قۇرلار && ent->ignored) {
		maybe_putc('?', out);
		len++;
	}
	أرجع len;
}

ساكن size_t سانى_marks(ثابت بنية blame_entry *ent, صحيح opt)
{
	أرجع print_marks(عدم, ent, opt);
}

ساكن فراغ emit_other(بنية blame_scoreboard *sb, بنية blame_entry *ent,
		       صحيح opt, بنية blame_entry *prev_ent)
{
	صحيح cnt;
	ثابت حرف *cp;
	بنية blame_origin *suspect = ent->suspect;
	بنية commit_info ci = COMMIT_INFO_INIT;
	حرف hex[GIT_MAX_HEXSZ + 1];
	صحيح show_raw_time = !!(opt & OUTPUT_RAW_TIMESTAMP);
	ثابت حرف *default_color = عدم, *color = عدم, *reset = عدم;

	get_commit_info(suspect->commit, &ci);
	oid_to_hex_r(hex, &suspect->commit->object.oid);

	cp = blame_nth_قۇر(sb, ent->lno);

	إذا (opt & OUTPUT_SHOW_AGE_WITH_COLOR) {
		determine_قۇر_heat(&ci, &default_color);
		color = default_color;
		reset = GIT_COLOR_RESET;
	}

	إذا (abbrev < MINIMUM_ABBREV)
		BUG("abbreviation is smaller than minimum length: %d < %d",
		    abbrev, MINIMUM_ABBREV);

	لكل (cnt = 0; cnt < ent->رەقەم_قۇرلار; cnt++) {
		حرف ch;
		size_t length = (opt & OUTPUT_LONG_OBJECT_NAME) ?
			the_hash_algo->hexsz : (size_t) abbrev;

		إذا (opt & OUTPUT_COLOR_قۇر) {
			إذا (cnt > 0 ||
			    (prev_ent &&
			     oideq(&suspect->commit->object.oid,
				   &prev_ent->suspect->commit->object.oid))) {
				color = repeated_meta_color;
				reset = GIT_COLOR_RESET;
			} وإلا  {
				color = default_color ? default_color : عدم;
				reset = default_color ? GIT_COLOR_RESET : عدم;
			}
		}
		إذا (color)
			fputs(color, stdout);

		إذا ((suspect->commit->object.flags & UNINTERESTING) &&
		    blank_boundary)
			memset(hex, ' ', strlen(hex));
		length -= print_marks(stdout, ent, opt);

		printf("%.*s", (صحيح)(length < GIT_MAX_HEXSZ ? length : GIT_MAX_HEXSZ), hex);
		إذا (opt & OUTPUT_ANNOTATE_COMPAT) {
			ثابت حرف *name;
			إذا (opt & OUTPUT_SHOW_EMAIL)
				name = ci.author_mail.buf;
			وإلا
				name = ci.author.buf;
			printf("\t(%10s\t%10s\t%d)", name,
			       format_time(ci.author_time, ci.author_tz.buf,
					   show_raw_time),
			       ent->lno + 1 + cnt);
		} وإلا {
			إذا (opt & OUTPUT_SHOW_SCORE)
				printf(" %*d %02d",
				       max_score_digits, ent->score,
				       ent->suspect->refcnt);
			إذا (opt & OUTPUT_SHOW_NAME)
				printf(" %-*.*s", longest_file, longest_file,
				       suspect->path);
			إذا (opt & OUTPUT_SHOW_سان)
				printf(" %*d", max_orig_digits,
				       ent->s_lno + 1 + cnt);

			إذا (!(opt & OUTPUT_NO_AUTHOR)) {
				ثابت حرف *name;
				صحيح pad;
				إذا (opt & OUTPUT_SHOW_EMAIL)
					name = ci.author_mail.buf;
				وإلا
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
		إذا (reset)
			fputs(reset, stdout);
		افعل {
			ch = *cp++;
			putchar(ch);
		} طالما (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_size);
	}

	إذا (sb->final_buf_size && cp[-1] != '\n')
		putchar('\n');

	commit_info_destroy(&ci);
}

ساكن فراغ output(بنية blame_scoreboard *sb, صحيح option)
{
	بنية blame_entry *ent, *prev_ent = عدم;

	إذا (option & OUTPUT_PORCELAIN) {
		لكل (ent = sb->ent; ent; ent = ent->next) {
			صحيح سانى = 0;
			بنية blame_origin *suspect;
			بنية commit *commit = ent->suspect->commit;
			إذا (commit->object.flags & MORE_THAN_ONE_PATH)
				استمر;
			لكل (suspect = get_blame_suspects(commit); suspect; suspect = suspect->next) {
				إذا (suspect->guilty && سانى++) {
					commit->object.flags |= MORE_THAN_ONE_PATH;
					اكسر;
				}
			}
		}
	}

	لكل (ent = sb->ent; ent; ent = ent->next) {
		إذا (option & OUTPUT_PORCELAIN)
			emit_porcelain(sb, ent, option);
		وإلا {
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
ساكن صحيح read_ancestry(ثابت حرف *graft_file)
{
	FILE *fp = fopen_or_warn(graft_file, "r");
	بنية strbuf buf = STRBUF_INIT;
	إذا (!fp)
		أرجع -1;
	طالما (!strbuf_getwholeline(&buf, fp, '\n')) {
		/* The format is just "Commit Parent1 Parent2 ...\n" */
		بنية commit_graft *graft = read_graft_قۇر(&buf);
		إذا (graft)
			register_commit_graft(the_repository, graft, 0);
	}
	fclose(fp);
	strbuf_release(&buf);
	أرجع 0;
}

ساكن صحيح update_auto_abbrev(صحيح auto_abbrev, بنية blame_origin *suspect)
{
	ثابت حرف *uniq = repo_find_unique_abbrev(the_repository,
						   &suspect->commit->object.oid,
						   auto_abbrev);
	صحيح len = strlen(uniq);
	إذا (auto_abbrev < len)
		أرجع len;
	أرجع auto_abbrev;
}

/*
 * How many columns do we need to show line numbers, authors,
 * and filenames?
 */
ساكن فراغ find_alignment(بنية blame_scoreboard *sb, صحيح *option)
{
	صحيح longest_src_قۇرلار = 0;
	صحيح longest_dst_قۇرلار = 0;
	unsigned largest_score = 0;
	بنية blame_entry *e;
	صحيح compute_auto_abbrev = (abbrev < 0);
	صحيح auto_abbrev = DEFAULT_ABBREV;
	size_t max_marks_سانى = 0;

	لكل (e = sb->ent; e; e = e->next) {
		بنية blame_origin *suspect = e->suspect;
		صحيح رەقەم;
		size_t marks_سانى = سانى_marks(e, *option);

		إذا (max_marks_سانى < marks_سانى)
			max_marks_سانى = marks_سانى;
		إذا (compute_auto_abbrev)
			auto_abbrev = update_auto_abbrev(auto_abbrev, suspect);
		إذا (strcmp(suspect->path, sb->path))
			*option |= OUTPUT_SHOW_NAME;
		رەقەم = strlen(suspect->path);
		إذا (longest_file < رەقەم)
			longest_file = رەقەم;
		إذا (!(suspect->commit->object.flags & METAINFO_SHOWN)) {
			بنية commit_info ci = COMMIT_INFO_INIT;
			suspect->commit->object.flags |= METAINFO_SHOWN;
			get_commit_info(suspect->commit, &ci);
			إذا (*option & OUTPUT_SHOW_EMAIL)
				رەقەم = utf8_strwidth(ci.author_mail.buf);
			وإلا
				رەقەم = utf8_strwidth(ci.author.buf);
			إذا (longest_author < رەقەم)
				longest_author = رەقەم;
			commit_info_destroy(&ci);
		}
		رەقەم = e->s_lno + e->رەقەم_قۇرلار;
		إذا (longest_src_قۇرلار < رەقەم)
			longest_src_قۇرلار = رەقەم;
		رەقەم = e->lno + e->رەقەم_قۇرلار;
		إذا (longest_dst_قۇرلار < رەقەم)
			longest_dst_قۇرلار = رەقەم;
		إذا (largest_score < blame_entry_score(sb, e))
			largest_score = blame_entry_score(sb, e);
	}
	max_orig_digits = decimal_width(longest_src_قۇرلار);
	max_digits = decimal_width(longest_dst_قۇرلار);
	max_score_digits = decimal_width(largest_score);

	إذا (compute_auto_abbrev)
		abbrev = auto_abbrev;
	إذا (abbrev < (صحيح)the_hash_algo->hexsz) {
		abbrev += max_marks_سانى;
		إذا (abbrev > (صحيح)the_hash_algo->hexsz)
			abbrev = the_hash_algo->hexsz;
	}
}

ساكن فراغ sanity_check_on_fail(بنية blame_scoreboard *sb, صحيح baa)
{
	صحيح opt = OUTPUT_SHOW_SCORE | OUTPUT_SHOW_سان | OUTPUT_SHOW_NAME;
	find_alignment(sb, &opt);
	output(sb, opt);
	die("Baa %d!", baa);
}

ساكن unsigned parse_score(ثابت حرف *arg)
{
	حرف *end;
	unsigned long score = strtoul(arg, &end, 10);
	إذا (*end)
		أرجع 0;
	أرجع score;
}

ساكن حرف *add_prefix(ثابت حرف *prefix, ثابت حرف *path)
{
	أرجع prefix_path(the_repository, prefix, prefix ? strlen(prefix) : 0, path);
}

ساكن صحيح git_blame_config(ثابت حرف *var, ثابت حرف *قىممەت,
			    ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, "blame.showroot")) {
		show_يىلتىز = git_config_bool(var, قىممەت);
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.blankboundary")) {
		blank_boundary = git_config_bool(var, قىممەت);
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.showemail")) {
		صحيح *output_option = cb;
		إذا (git_config_bool(var, قىممەت))
			*output_option |= OUTPUT_SHOW_EMAIL;
		وإلا
			*output_option &= ~OUTPUT_SHOW_EMAIL;
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.date")) {
		إذا (!قىممەت)
			أرجع config_error_nonbool(var);
		parse_date_format(قىممەت, &blame_date_mode);
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.ignorerevsfile")) {
		حرف *str;
		صحيح ret;

		ret = git_config_pathname(&str, var, قىممەت);
		إذا (ret)
			أرجع ret;
		إذا (str)
			string_list_قىستۇرۇش(&ignore_revs_file_list, str);
		حرر(str);
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.markunblamablelines")) {
		mark_unblamable_قۇرلار = git_config_bool(var, قىممەت);
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.markignoredlines")) {
		mark_ignored_قۇرلار = git_config_bool(var, قىممەت);
		أرجع 0;
	}
	إذا (!strcmp(var, "color.blame.repeatedlines")) {
		إذا (color_parse_mem(قىممەت, strlen(قىممەت), repeated_meta_color))
			warning(_("invalid value for '%s': '%s'"),
				"color.blame.repeatedLines", قىممەت);
		أرجع 0;
	}
	إذا (!strcmp(var, "color.blame.highlightrecent")) {
		parse_color_fields(قىممەت);
		أرجع 0;
	}

	إذا (!strcmp(var, "blame.coloring")) {
		إذا (!قىممەت)
			أرجع config_error_nonbool(var);
		إذا (!strcmp(قىممەت, "repeatedLines")) {
			coloring_mode |= OUTPUT_COLOR_قۇر;
		} وإلا إذا (!strcmp(قىممەت, "highlightRecent")) {
			coloring_mode |= OUTPUT_SHOW_AGE_WITH_COLOR;
		} وإلا إذا (!strcmp(قىممەت, "none")) {
			coloring_mode &= ~(OUTPUT_COLOR_قۇر |
					    OUTPUT_SHOW_AGE_WITH_COLOR);
		} وإلا {
			warning(_("invalid value for '%s': '%s'"),
				"blame.coloring", قىممەت);
			أرجع 0;
		}
	}

	إذا (!strcmp(var, "diff.algorithm")) {
		long diff_algorithm;
		إذا (!قىممەت)
			أرجع config_error_nonbool(var);
		diff_algorithm = parse_algorithm_قىممەت(قىممەت);
		إذا (diff_algorithm < 0)
			أرجع error(_("unknown value for config '%s': %s"),
				     var, قىممەت);
		xdl_opts &= ~XDF_DIFF_ALGORITHM_MASK;
		xdl_opts |= diff_algorithm;
		أرجع 0;
	}

	إذا (git_diff_heuristic_config(var, قىممەت, cb) < 0)
		أرجع -1;
	إذا (userdiff_config(var, قىممەت) < 0)
		أرجع -1;

	أرجع git_default_config(var, قىممەت, ctx, cb);
}

ساكن صحيح blame_copy_callback(ثابت بنية option *option, ثابت حرف *arg, صحيح unset)
{
	صحيح *opt = option->قىممەت;

	BUG_ON_OPT_NEG(unset);

	/*
	 * -C enables copy from removed files;
	 * -C -C enables copy from existing files, but only
	 *       when blaming a new file;
	 * -C -C -C enables copy from existing files for
	 *          everybody
	 */
	إذا (*opt & PICKAXE_BLAME_COPY_HARDER)
		*opt |= PICKAXE_BLAME_COPY_HARDEST;
	إذا (*opt & PICKAXE_BLAME_COPY)
		*opt |= PICKAXE_BLAME_COPY_HARDER;
	*opt |= PICKAXE_BLAME_COPY | PICKAXE_BLAME_MOVE;

	إذا (arg)
		blame_copy_score = parse_score(arg);
	أرجع 0;
}

ساكن صحيح blame_move_callback(ثابت بنية option *option, ثابت حرف *arg, صحيح unset)
{
	صحيح *opt = option->قىممەت;

	BUG_ON_OPT_NEG(unset);

	*opt |= PICKAXE_BLAME_MOVE;

	إذا (arg)
		blame_move_score = parse_score(arg);
	أرجع 0;
}

ساكن صحيح blame_diff_algorithm_minimal(ثابت بنية option *option,
					ثابت حرف *arg, صحيح unset)
{
	صحيح *opt = option->قىممەت;

	BUG_ON_OPT_ARG(arg);

	*opt &= ~XDF_DIFF_ALGORITHM_MASK;
	إذا (!unset)
		*opt |= XDF_NEED_MINIMAL;

	أرجع 0;
}

ساكن صحيح blame_diff_algorithm_callback(ثابت بنية option *option,
					 ثابت حرف *arg, صحيح unset)
{
	صحيح *opt = option->قىممەت;
	long قىممەت = parse_algorithm_قىممەت(arg);

	BUG_ON_OPT_NEG(unset);

	إذا (قىممەت < 0)
		أرجع error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	*opt &= ~XDF_DIFF_ALGORITHM_MASK;
	*opt |= قىممەت;

	أرجع 0;
}

ساكن صحيح is_a_rev(ثابت حرف *name)
{
	بنية object_id oid;

	إذا (repo_get_oid(the_repository, name, &oid))
		أرجع 0;
	أرجع OBJ_NONE < odb_read_object_info(the_repository->objects, &oid, عدم);
}

ساكن صحيح peel_to_commit_oid(بنية object_id *oid_ret, فراغ *cbdata)
{
	بنية repository *r = ((بنية blame_scoreboard *)cbdata)->repo;
	بنية object_id oid;

	oidcpy(&oid, oid_ret);
	طالما (1) {
		بنية object *obj;
		صحيح kind = odb_read_object_info(r->objects, &oid, عدم);
		إذا (kind == OBJ_COMMIT) {
			oidcpy(oid_ret, &oid);
			أرجع 0;
		}
		إذا (kind != OBJ_TAG)
			أرجع -1;
		obj = deref_tag(r, parse_object(r, &oid), عدم, 0);
		إذا (!obj)
			أرجع -1;
		oidcpy(&oid, &obj->oid);
	}
}

ساكن فراغ build_ignorelist(بنية blame_scoreboard *sb,
			     بنية string_list *ignore_revs_file_list,
			     بنية string_list *ignore_rev_list)
{
	بنية string_list_item *i;
	بنية object_id oid;

	oidset_init(&sb->ignore_list, 0);
	for_each_string_list_item(i, ignore_revs_file_list) {
		إذا (!strcmp(i->string, ""))
			oidset_clear(&sb->ignore_list);
		وإلا
			oidset_parse_file_carefully(&sb->ignore_list, i->string,
						    the_repository->hash_algo,
						    peel_to_commit_oid, sb);
	}
	for_each_string_list_item(i, ignore_rev_list) {
		إذا (repo_get_oid_committish(the_repository, i->string, &oid) ||
		    peel_to_commit_oid(&oid, sb))
			die(_("cannot find revision %s to ignore"), i->string);
		oidset_قىستۇرۇش(&sb->ignore_list, &oid);
	}
}

صحيح cmd_blame(صحيح argc,
	      ثابت حرف **argv,
	      ثابت حرف *prefix,
	      بنية repository *repo UNUSED)
{
	بنية rev_info revs;
	حرف *path = عدم;
	بنية blame_scoreboard sb;
	بنية blame_origin *o;
	بنية blame_entry *ent = عدم;
	long dashdash_pos, lno;
	بنية progress_info pi = { عدم, 0 };

	بنية string_list range_list = STRING_LIST_INIT_NODUP;
	بنية string_list ignore_rev_list = STRING_LIST_INIT_NODUP;
	صحيح output_option = 0, opt = 0;
	صحيح show_stats = 0;
	ثابت حرف *revs_file = عدم;
	ثابت حرف *contents_from = عدم;
	ثابت بنية option options[] = {
		OPT_BOOL(0, "incremental", &incremental, N_("show blame entries as we find them, incrementally")),
		OPT_BOOL('b', عدم, &blank_boundary, N_("do not show object names of boundary commits (Default: off)")),
		OPT_BOOL(0, "root", &show_يىلتىز, N_("do not treat root commits as boundaries (Default: off)")),
		OPT_BOOL(0, "show-stats", &show_stats, N_("show work cost statistics")),
		OPT_BOOL(0, "progress", &show_progress, N_("force progress reporting")),
		OPT_BIT(0, "score-debug", &output_option, N_("show output score for blame entries"), OUTPUT_SHOW_SCORE),
		OPT_BIT('f', "show-name", &output_option, N_("show original filename (Default: auto)"), OUTPUT_SHOW_NAME),
		OPT_BIT('n', "show-number", &output_option, N_("show original linenumber (Default: off)"), OUTPUT_SHOW_سان),
		OPT_BIT('p', "porcelain", &output_option, N_("show in a format designed for machine consumption"), OUTPUT_PORCELAIN),
		OPT_BIT(0, "line-porcelain", &output_option, N_("show porcelain format with per-line commit information"), OUTPUT_PORCELAIN|OUTPUT_قۇر_PORCELAIN),
		OPT_BIT('c', عدم, &output_option, N_("use the same output mode as git-annotate (Default: off)"), OUTPUT_ANNOTATE_COMPAT),
		OPT_BIT('t', عدم, &output_option, N_("show raw timestamp (Default: off)"), OUTPUT_RAW_TIMESTAMP),
		OPT_BIT('l', عدم, &output_option, N_("show long commit SHA1 (Default: off)"), OUTPUT_LONG_OBJECT_NAME),
		OPT_BIT('s', عدم, &output_option, N_("suppress author name and timestamp (Default: off)"), OUTPUT_NO_AUTHOR),
		OPT_BIT('e', "show-email", &output_option, N_("show author email instead of name (Default: off)"), OUTPUT_SHOW_EMAIL),
		OPT_BIT('w', عدم, &xdl_opts, N_("ignore whitespace differences"), XDF_IGNORE_WHITESPACE),
		OPT_CALLBACK_F(0, "diff-algorithm", &xdl_opts, N_("<algorithm>"),
			       N_("choose a diff algorithm"),
			       PARSE_OPT_NONEG, blame_diff_algorithm_callback),
		OPT_STRING_LIST(0, "ignore-rev", &ignore_rev_list, N_("rev"), N_("ignore <rev> when blaming")),
		OPT_STRING_LIST(0, "ignore-revs-file", &ignore_revs_file_list, N_("file"), N_("ignore revisions from <file>")),
		OPT_BIT(0, "color-lines", &output_option, N_("color redundant metadata from previous line differently"), OUTPUT_COLOR_قۇر),
		OPT_BIT(0, "color-by-age", &output_option, N_("color lines by age"), OUTPUT_SHOW_AGE_WITH_COLOR),
		OPT_CALLBACK_F(0, "minimal", &xdl_opts, عدم,
			       N_("spend extra cycles to find a better match"),
			       PARSE_OPT_NOARG | PARSE_OPT_HIDDEN, blame_diff_algorithm_minimal),
		OPT_STRING('S', عدم, &revs_file, N_("file"), N_("use revisions from <file> instead of calling git-rev-list")),
		OPT_STRING(0, "contents", &contents_from, N_("file"), N_("use <file>'s contents as the final image")),
		OPT_CALLBACK_F('C', عدم, &opt, N_("score"), N_("find line copies within and across files"), PARSE_OPT_OPTARG, blame_copy_callback),
		OPT_CALLBACK_F('M', عدم, &opt, N_("score"), N_("find line movements within and across files"), PARSE_OPT_OPTARG, blame_move_callback),
		OPT_STRING_LIST('L', عدم, &range_list, N_("range"),
				N_("process only line range <start>,<end> or function :<funcname>")),
		OPT__ABBREV(&abbrev),
		OPT_END()
	};

	بنية parse_opt_ctx_t ctx;
	صحيح cmd_is_annotate = !strcmp(argv[0], "annotate");
	بنية range_set ranges;
	unsigned صحيح range_i;
	long anchor;
	long رەقەم_قۇرلار = 0;
	ثابت حرف *str_usage = cmd_is_annotate ? annotate_usage : blame_usage;
	ثابت حرف *ثابت *opt_usage = cmd_is_annotate ? annotate_opt_usage : blame_opt_usage;

	setup_default_color_by_age();
	repo_config(the_repository, git_blame_config, &output_option);
	repo_init_revisions(the_repository, &revs, عدم);
	revs.date_mode = blame_date_mode;
	revs.diffopt.flags.allow_textconv = 1;
	revs.diffopt.flags.follow_renames = 1;

	save_commit_buffer = 0;
	dashdash_pos = 0;
	show_progress = -1;

	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);
	لكل (;;) {
		بدل (parse_options_step(&ctx, options, opt_usage)) {
		حالة PARSE_OPT_NON_OPTION:
		حالة PARSE_OPT_UNKNOWN:
			اكسر;
		حالة PARSE_OPT_HELP:
			exit(0);
		حالة PARSE_OPT_HELP_ERROR:
		حالة PARSE_OPT_ERROR:
		حالة PARSE_OPT_SUBCOMMAND:
			exit(129);
		حالة PARSE_OPT_COMPLETE:
			exit(0);
		حالة PARSE_OPT_DONE:
			إذا (ctx.argv[0])
				dashdash_pos = ctx.cpidx;
			اذهب parse_done;
		}

		إذا (!strcmp(ctx.argv[0], "--reverse")) {
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

	إذا (incremental || (output_option & OUTPUT_PORCELAIN)) {
		إذا (show_progress > 0)
			die(_("--progress can't be used with --incremental or porcelain formats"));
		show_progress = 0;
	} وإلا إذا (show_progress < 0)
		show_progress = isatty(2);

	إذا (!abbrev)
		abbrev = the_hash_algo->hexsz;

	إذا (revs_file && read_ancestry(revs_file))
		die_errno("reading graft file '%s' failed", revs_file);

	إذا (cmd_is_annotate) {
		output_option |= OUTPUT_ANNOTATE_COMPAT;
		blame_date_mode.type = DATE_ISO8601;
	} وإلا {
		blame_date_mode = revs.date_mode;
	}

	/* The maximum width used to show the dates */
	بدل (blame_date_mode.type) {
	حالة DATE_RFC2822:
		blame_date_width = حجم("Thu, 19 Oct 2006 16:00:04 -0700");
		اكسر;
	حالة DATE_ISO8601_STRICT:
		blame_date_width = حجم("2006-10-19T16:00:04-07:00");
		اكسر;
	حالة DATE_ISO8601:
		blame_date_width = حجم("2006-10-19 16:00:04 -0700");
		اكسر;
	حالة DATE_RAW:
		blame_date_width = حجم("1161298804 -0700");
		اكسر;
	حالة DATE_UNIX:
		blame_date_width = حجم("1161298804");
		اكسر;
	حالة DATE_SHORT:
		blame_date_width = حجم("2006-10-19");
		اكسر;
	حالة DATE_RELATIVE:
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
		اكسر;
	حالة DATE_HUMAN:
		/* If the year is shown, no time is shown */
		blame_date_width = حجم("Thu Oct 19 16:00");
		اكسر;
	حالة DATE_NORMAL:
		blame_date_width = حجم("Thu Oct 19 16:00:04 2006 -0700");
		اكسر;
	حالة DATE_STRFTIME:
		blame_date_width = strlen(show_date(0, 0, blame_date_mode)) + 1; /* add the null */
		اكسر;
	}
	blame_date_width -= 1; /* strip the null */

	إذا (revs.diffopt.flags.find_copies_harder)
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
	إذا (dashdash_pos) {
		بدل (argc - dashdash_pos - 1) {
		حالة 2: /* (1b) */
			إذا (argc != 4)
				usage_with_options(opt_usage, options);
			/* reorder for the new way: <rev> -- <path> */
			argv[1] = argv[3];
			argv[3] = argv[2];
			argv[2] = "--";
			/* FALLTHROUGH */
		حالة 1: /* (1a) */
			path = add_prefix(prefix, argv[--argc]);
			argv[argc] = عدم;
			اكسر;
		افتراضي:
			usage_with_options(opt_usage, options);
		}
	} وإلا {
		إذا (argc < 2)
			usage_with_options(opt_usage, options);
		إذا (argc == 3 && is_a_rev(argv[argc - 1])) { /* (2b) */
			path = add_prefix(prefix, argv[1]);
			argv[1] = argv[2];
		} وإلا {	/* (2a) */
			إذا (argc == 2 && is_a_rev(argv[1]) && !repo_get_work_دەرەخ(the_repository))
				die("missing <path> to blame");
			path = add_prefix(prefix, argv[argc - 1]);
		}
		argv[argc - 1] = "--";
	}

	revs.disable_stdin = 1;
	setup_revisions(argc, argv, &revs, عدم);
	إذا (!revs.pending.nr && is_bare_repository(the_repository)) {
		بنية commit *head_commit;
		بنية object_id head_oid;

		إذا (!refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", RESOLVE_REF_READING,
					     &head_oid, عدم) ||
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
	إذا (!(opt & PICKAXE_BLAME_COPY))
		setup_blame_bloom_data(&sb);

	lno = sb.رەقەم_قۇرلار;

	إذا (lno && !range_list.nr)
		string_list_append(&range_list, "1");

	anchor = 1;
	range_set_init(&ranges, range_list.nr);
	لكل (range_i = 0; range_i < range_list.nr; ++range_i) {
		long bottom, چوققا;
		إذا (parse_range_arg(range_list.بۇيۇملار[range_i].string,
				    nth_قۇر_cb, &sb, lno, anchor,
				    &bottom, &چوققا, sb.path,
				    the_repository->index))
			usage(str_usage);
		إذا ((!lno && (چوققا || bottom)) || lno < bottom)
			die(Q_("file %s has only %lu line",
			       "file %s has only %lu lines",
			       lno), sb.path, lno);
		إذا (bottom < 1)
			bottom = 1;
		إذا (چوققا < 1 || lno < چوققا)
			چوققا = lno;
		bottom--;
		range_set_append_unsafe(&ranges, bottom, چوققا);
		anchor = چوققا + 1;
	}
	sort_and_merge_range_set(&ranges);

	لكل (range_i = ranges.nr; range_i > 0; --range_i) {
		ثابت بنية range *r = &ranges.ranges[range_i - 1];
		ent = blame_entry_prepend(ent, r->start, r->end, o);
		رەقەم_قۇرلار += (r->end - r->start);
	}
	إذا (!رەقەم_قۇرلار)
		رەقەم_قۇرلار = sb.رەقەم_قۇرلار;

	o->suspects = ent;
	prio_queue_put(&sb.commits, o->commit);

	blame_origin_decref(o);

	range_set_release(&ranges);
	string_list_clear(&range_list, 0);

	sb.ent = عدم;

	إذا (blame_move_score)
		sb.move_score = blame_move_score;
	إذا (blame_copy_score)
		sb.copy_score = blame_copy_score;

	sb.debug = DEBUG_BLAME;
	sb.on_sanity_fail = &sanity_check_on_fail;

	sb.show_يىلتىز = show_يىلتىز;
	sb.xdl_opts = xdl_opts;
	sb.no_whole_file_rename = no_whole_file_rename;

	read_mailmap(the_repository, &mailmap);

	sb.found_guilty_entry = &found_guilty_entry;
	sb.found_guilty_entry_data = &pi;
	إذا (show_progress)
		pi.progress = start_delayed_progress(the_repository,
						     _("Blaming lines"),
						     رەقەم_قۇرلار);

	assign_blame(&sb, opt);

	stop_progress(&pi.progress);

	إذا (!incremental)
		setup_pager(the_repository);
	وإلا
		اذهب cleanup;

	blame_sort_final(&sb);

	blame_coalesce(&sb);

	إذا (!(output_option & (OUTPUT_COLOR_قۇر | OUTPUT_SHOW_AGE_WITH_COLOR)))
		output_option |= coloring_mode;

	إذا (!(output_option & OUTPUT_PORCELAIN)) {
		find_alignment(&sb, &output_option);
		إذا (!*repeated_meta_color &&
		    (output_option & OUTPUT_COLOR_قۇر))
			xsnprintf(repeated_meta_color,
				  حجم(repeated_meta_color),
				  "%s", GIT_COLOR_CYAN);
	}
	إذا (output_option & OUTPUT_ANNOTATE_COMPAT)
		output_option &= ~(OUTPUT_COLOR_قۇر | OUTPUT_SHOW_AGE_WITH_COLOR);

	output(&sb, output_option);

	إذا (show_stats) {
		printf("num read blob: %d\n", sb.رەقەم_read_blob);
		printf("num get patch: %d\n", sb.رەقەم_get_patch);
		printf("num commits: %d\n", sb.رەقەم_commits);
	}

cleanup:
	لكل (ent = sb.ent; ent; ) {
		بنية blame_entry *e = ent->next;
		حرر(ent);
		ent = e;
	}

	حرر(path);
	cleanup_scoreboard(&sb);
	release_revisions(&revs);
	أرجع 0;
}
