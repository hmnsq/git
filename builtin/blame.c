/*
 * Blame
 *
 * Copyright (c) 2006, 2014 by its authors
 * See COPYING for licensing conditions
 */

#تعریف_کن USE_THE_REPOSITORY_VARIABLE

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
ساكن صحيح show_ریشه;
ساكن صحيح معکوس;
ساكن صحيح blank_boundary;
ساكن صحيح incremental;
ساكن صحيح xdl_opts;
ساكن صحيح abbrev = -1;
ساكن صحيح no_کامل_file_rename;
ساكن صحيح show_پیشرفت;
ساكن حرف repeated_meta_color[COLOR_MAXLEN];
ساكن صحيح coloring_نما;
ساكن بنية string_list ignore_revs_file_list = STRING_LIST_INIT_DUP;
ساكن صحيح mark_unblamable_سطرها;
ساكن صحيح mark_ignored_سطرها;

ساكن بنية date_نما blame_date_نما = { DATE_ISO8601 };
ساكن اندازه_t blame_date_عرض;

ساكن بنية string_list mailmap = STRING_LIST_INIT_NODUP;

#اگر_تعریف_نشده DEBUG_BLAME
#تعریف_کن DEBUG_BLAME 0
#پایان_اگر

ساكن بیعلامت blame_move_score;
ساكن بیعلامت blame_copy_score;

/* Remember to update object flag allocation in object.h */
#تعریف_کن METAINFO_SHOWN		(1u<<12)
#تعریف_کن بیشتر_THAN_ONE_مسیر	(1u<<13)

بنية پیشرفت_info {
	بنية پیشرفت *پیشرفت;
	صحيح blamed_سطرها;
};

ساكن ثابت حرف *nth_سطر_cb(فراغ *داده, دراز lno)
{
	أرجع blame_nth_سطر((بنية blame_scoreboard *)داده, lno);
}

/*
 * Information on commits, used for output.
 */
بنية commit_info {
	بنية strbuf author;
	بنية strbuf author_mail;
	timestamp_t author_زمان;
	بنية strbuf author_tz;

	/* filled only when asked for details */
	بنية strbuf committer;
	بنية strbuf committer_mail;
	timestamp_t committer_زمان;
	بنية strbuf committer_tz;

	بنية strbuf summary;
};

#تعریف_کن COMMIT_INFO_INIT { \
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
ساكن فراغ get_ac_سطر(ثابت حرف *inbuf, ثابت حرف *what,
	بنية strbuf *name, بنية strbuf *mail,
	timestamp_t *زمان, بنية strbuf *tz)
{
	بنية ident_تقسیم ident;
	اندازه_t len, maillen, namelen;
	ثابت حرف *tmp, *endp;
	ثابت حرف *namebuf, *mailbuf;

	tmp = جستجوی_رشته(inbuf, what);
	إذا (!tmp)
		اذهب error_بیرون;
	tmp += طول_رشته(what);
	endp = strchr(tmp, '\n');
	إذا (!endp)
		len = طول_رشته(tmp);
	وإلا
		len = endp - tmp;

	إذا (تقسیم_ident_سطر(&ident, tmp, len)) {
	error_بیرون:
		/* Ugh */
		tmp = "(unknown)";
		strbuf_addstr(name, tmp);
		strbuf_addstr(mail, tmp);
		strbuf_addstr(tz, tmp);
		*زمان = 0;
		أرجع;
	}

	namelen = ident.name_انتها - ident.name_begin;
	namebuf = ident.name_begin;

	maillen = ident.mail_انتها - ident.mail_begin;
	mailbuf = ident.mail_begin;

	إذا (ident.date_begin && ident.date_انتها)
		*زمان = strtoul(ident.date_begin, عدم, 10);
	وإلا
		*زمان = 0;

	إذا (ident.tz_begin && ident.tz_انتها)
		strbuf_جمع_کن(tz, ident.tz_begin, ident.tz_انتها - ident.tz_begin);
	وإلا
		strbuf_addstr(tz, "(unknown)");

	/*
	 * Now, convert both name and e-mail using mailmap
	 */
	map_user(&mailmap, &mailbuf, &maillen,
		 &namebuf, &namelen);

	strbuf_addf(mail, "<%.*s>", (صحيح)maillen, mailbuf);
	strbuf_جمع_کن(name, namebuf, namelen);
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
	ثابت حرف *پیام_2;

	encoding = get_log_output_encoding();
	پیام_2 = repo_logmsg_reencode(the_repository, commit, عدم, encoding);
	get_ac_سطر(پیام_2, "\nauthor ",
		    &ret->author, &ret->author_mail,
		    &ret->author_زمان, &ret->author_tz);

	get_ac_سطر(پیام_2, "\ncommitter ",
		    &ret->committer, &ret->committer_mail,
		    &ret->committer_زمان, &ret->committer_tz);

	len = پیدا_کن_commit_subject(پیام_2, &subject);
	إذا (len)
		strbuf_جمع_کن(&ret->summary, subject, len);
	وإلا
		strbuf_addf(&ret->summary, "(%s)", oid_مقصد_hex(&commit->object.oid));

	repo_unuse_commit_میانگیر(the_repository, commit, پیام_2);
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
	إذا (suspect->قبلی) {
		بنية blame_origin *prev = suspect->قبلی;
		چاپ_کن("previous %s ", oid_مقصد_hex(&prev->commit->object.oid));
		write_name_quoted(prev->مسیر, stdout, '\n');
	}
	چاپ_کن("filename ");
	write_name_quoted(suspect->مسیر, stdout, '\n');
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
	چاپ_کن("author %s\n", ci.author.buf);
	چاپ_کن("author-mail %s\n", ci.author_mail.buf);
	چاپ_کن("author-time %"PRItime"\n", ci.author_زمان);
	چاپ_کن("author-tz %s\n", ci.author_tz.buf);
	چاپ_کن("committer %s\n", ci.committer.buf);
	چاپ_کن("committer-mail %s\n", ci.committer_mail.buf);
	چاپ_کن("committer-time %"PRItime"\n", ci.committer_زمان);
	چاپ_کن("committer-tz %s\n", ci.committer_tz.buf);
	چاپ_کن("summary %s\n", ci.summary.buf);
	إذا (suspect->commit->object.flags & UNINTERESTING)
		چاپ_کن("boundary\n");

	commit_info_destroy(&ci);

	أرجع 1;
}

/*
 * The blame_entry is found to be guilty for the range.
 * Show it in incremental output.
 */
ساكن فراغ یافت_guilty_مدخل(بنية blame_مدخل *ent, فراغ *داده)
{
	بنية پیشرفت_info *pi = (بنية پیشرفت_info *)داده;

	إذا (incremental) {
		بنية blame_origin *suspect = ent->suspect;

		چاپ_کن("%s %d %d %d\n",
		       oid_مقصد_hex(&suspect->commit->object.oid),
		       ent->s_lno + 1, ent->lno + 1, ent->رقم_سطرها);
		emit_one_suspect_detail(suspect, 0);
		write_filename_info(suspect);
		maybe_flush_or_die(stdout, "stdout");
	}
	pi->blamed_سطرها += ent->رقم_سطرها;
	display_پیشرفت(pi->پیشرفت, pi->blamed_سطرها);
}

ساكن ثابت حرف *format_زمان(timestamp_t زمان, ثابت حرف *tz_str,
			       صحيح show_raw_زمان)
{
	ساكن بنية strbuf زمان_buf = STRBUF_INIT;

	strbuf_reset(&زمان_buf);
	إذا (show_raw_زمان) {
		strbuf_addf(&زمان_buf, "%"PRItime" %s", زمان, tz_str);
	}
	وإلا {
		ثابت حرف *زمان_str;
		اندازه_t زمان_عرض;
		صحيح tz;
		tz = atoi(tz_str);
		زمان_str = show_date(زمان, tz, blame_date_نما);
		strbuf_addstr(&زمان_buf, زمان_str);
		/*
		 * Add space paddings to time_buf to display a fixed width
		 * string, and use time_width for display width calibration.
		 */
		لكل (زمان_عرض = utf8_strwidth(زمان_str);
		     زمان_عرض < blame_date_عرض;
		     زمان_عرض++)
			strbuf_addch(&زمان_buf, ' ');
	}
	أرجع زمان_buf.buf;
}

#تعریف_کن OUTPUT_ANNOTATE_COMPAT      (1U<<0)
#تعریف_کن OUTPUT_LONG_OBJECT_NAME     (1U<<1)
#تعریف_کن OUTPUT_RAW_TIMESTAMP        (1U<<2)
#تعریف_کن OUTPUT_PORCELAIN            (1U<<3)
#تعریف_کن OUTPUT_SHOW_NAME            (1U<<4)
#تعریف_کن OUTPUT_SHOW_عدد          (1U<<5)
#تعریف_کن OUTPUT_SHOW_SCORE           (1U<<6)
#تعریف_کن OUTPUT_NO_AUTHOR            (1U<<7)
#تعریف_کن OUTPUT_SHOW_EMAIL           (1U<<8)
#تعریف_کن OUTPUT_سطر_PORCELAIN       (1U<<9)
#تعریف_کن OUTPUT_COLOR_سطر           (1U<<10)
#تعریف_کن OUTPUT_SHOW_AGE_WITH_COLOR  (1U<<11)

ساكن فراغ emit_porcelain_details(بنية blame_origin *suspect, صحيح repeat)
{
	إذا (emit_one_suspect_detail(suspect, repeat) ||
	    (suspect->commit->object.flags & بیشتر_THAN_ONE_مسیر))
		write_filename_info(suspect);
}

/*
 * Information which needs to be printed per-line goes here. Any
 * information which can be clubbed on a commit/file level, should
 * be printed via 'emit_one_suspect_detail()'.
 */
ساكن فراغ emit_porcelain_در_هر_سطر_details(بنية blame_مدخل *ent)
{
	إذا (mark_unblamable_سطرها && ent->unblamable)
		puts("unblamable");
	إذا (mark_ignored_سطرها && ent->ignored)
		puts("ignored");
}

ساكن فراغ emit_porcelain(بنية blame_scoreboard *sb, بنية blame_مدخل *ent,
			   صحيح opt)
{
	صحيح repeat = opt & OUTPUT_سطر_PORCELAIN;
	صحيح cnt;
	ثابت حرف *cp;
	بنية blame_origin *suspect = ent->suspect;
	حرف hex[GIT_MAX_HEXSZ + 1];

	oid_مقصد_hex_r(hex, &suspect->commit->object.oid);
	چاپ_کن("%s %d %d %d\n",
	       hex,
	       ent->s_lno + 1,
	       ent->lno + 1,
	       ent->رقم_سطرها);
	emit_porcelain_details(suspect, repeat);
	emit_porcelain_در_هر_سطر_details(ent);

	cp = blame_nth_سطر(sb, ent->lno);
	لكل (cnt = 0; cnt < ent->رقم_سطرها; cnt++) {
		حرف ch;
		إذا (cnt) {
			چاپ_کن("%s %d %d\n", hex,
			       ent->s_lno + 1 + cnt,
			       ent->lno + 1 + cnt);
			إذا (repeat)
				emit_porcelain_details(suspect, 1);
			emit_porcelain_در_هر_سطر_details(ent);
		}
		حرف_چاپ_کن('\t');
		افعل {
			ch = *cp++;
			حرف_چاپ_کن(ch);
		} طالما (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_اندازه);
	}

	إذا (sb->final_buf_اندازه && cp[-1] != '\n')
		حرف_چاپ_کن('\n');
}

ساكن بنية color_فیلد {
	timestamp_t hop;
	حرف ستون[COLOR_MAXLEN];
} *colorfield;
ساكن صحيح colorfield_nr, colorfield_alloc;

ساكن فراغ parse_color_fields(ثابت حرف *s)
{
	بنية string_list l = STRING_LIST_INIT_DUP;
	بنية string_list_قلم *قلم;
	تعداد { EXPECT_DATE, EXPECT_COLOR } بعدی = EXPECT_COLOR;

	colorfield_nr = 0;

	/* Ideally this would be stripped and split at the same time? */
	string_list_تقسیم(&l, s, ",", -1);
	ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);

	for_each_string_list_قلم(قلم, &l) {
		بدل (بعدی) {
		حالة EXPECT_DATE:
			colorfield[colorfield_nr].hop = approxidate(قلم->string);
			بعدی = EXPECT_COLOR;
			colorfield_nr++;
			ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);
			اكسر;
		حالة EXPECT_COLOR:
			إذا (color_parse(قلم->string, colorfield[colorfield_nr].ستون))
				die(_("expecting a color: %s"), قلم->string);
			بعدی = EXPECT_DATE;
			اكسر;
		}
	}

	إذا (بعدی == EXPECT_COLOR)
		die(_("must end with a color"));

	colorfield[colorfield_nr].hop = زمان_MAX;
	string_list_clear(&l, 0);
}

ساكن فراغ راهاندازی_default_color_by_age(فراغ)
{
	parse_color_fields("blue,12 month ago,white,1 month ago,red");
}

ساكن فراغ determine_سطر_heat(بنية commit_info *ci, ثابت حرف **dest_color)
{
	صحيح i = 0;

	طالما (i < colorfield_nr && ci->author_زمان > colorfield[i].hop)
		i++;

	*dest_color = colorfield[i].ستون;
}

ساكن مضمن صحيح maybe_putc(صحيح c, FILE *بیرون)
{
	أرجع بیرون ? putc(c, بیرون) : 0;
}

ساكن اندازه_t print_marks(FILE *بیرون, ثابت بنية blame_مدخل *ent, صحيح opt)
{
	اندازه_t len = 0;

	إذا ((ent->suspect->commit->object.flags & UNINTERESTING) &&
	    !blank_boundary && !(opt & OUTPUT_ANNOTATE_COMPAT)) {
		maybe_putc('^', بیرون);
		len++;
	}
	إذا (mark_unblamable_سطرها && ent->unblamable) {
		maybe_putc('*', بیرون);
		len++;
	}
	إذا (mark_ignored_سطرها && ent->ignored) {
		maybe_putc('?', بیرون);
		len++;
	}
	أرجع len;
}

ساكن اندازه_t شمار_marks(ثابت بنية blame_مدخل *ent, صحيح opt)
{
	أرجع print_marks(عدم, ent, opt);
}

ساكن فراغ emit_other(بنية blame_scoreboard *sb, بنية blame_مدخل *ent,
		       صحيح opt, بنية blame_مدخل *prev_ent)
{
	صحيح cnt;
	ثابت حرف *cp;
	بنية blame_origin *suspect = ent->suspect;
	بنية commit_info ci = COMMIT_INFO_INIT;
	حرف hex[GIT_MAX_HEXSZ + 1];
	صحيح show_raw_زمان = !!(opt & OUTPUT_RAW_TIMESTAMP);
	ثابت حرف *default_color = عدم, *color = عدم, *reset = عدم;

	get_commit_info(suspect->commit, &ci);
	oid_مقصد_hex_r(hex, &suspect->commit->object.oid);

	cp = blame_nth_سطر(sb, ent->lno);

	إذا (opt & OUTPUT_SHOW_AGE_WITH_COLOR) {
		determine_سطر_heat(&ci, &default_color);
		color = default_color;
		reset = GIT_COLOR_RESET;
	}

	إذا (abbrev < MINIMUM_ABBREV)
		BUG("abbreviation is smaller than minimum length: %d < %d",
		    abbrev, MINIMUM_ABBREV);

	لكل (cnt = 0; cnt < ent->رقم_سطرها; cnt++) {
		حرف ch;
		اندازه_t درازی = (opt & OUTPUT_LONG_OBJECT_NAME) ?
			the_hash_algo->hexsz : (اندازه_t) abbrev;

		إذا (opt & OUTPUT_COLOR_سطر) {
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
			memset(hex, ' ', طول_رشته(hex));
		درازی -= print_marks(stdout, ent, opt);

		چاپ_کن("%.*s", (صحيح)(درازی < GIT_MAX_HEXSZ ? درازی : GIT_MAX_HEXSZ), hex);
		إذا (opt & OUTPUT_ANNOTATE_COMPAT) {
			ثابت حرف *name;
			إذا (opt & OUTPUT_SHOW_EMAIL)
				name = ci.author_mail.buf;
			وإلا
				name = ci.author.buf;
			چاپ_کن("\t(%10s\t%10s\t%d)", name,
			       format_زمان(ci.author_زمان, ci.author_tz.buf,
					   show_raw_زمان),
			       ent->lno + 1 + cnt);
		} وإلا {
			إذا (opt & OUTPUT_SHOW_SCORE)
				چاپ_کن(" %*d %02d",
				       max_score_digits, ent->score,
				       ent->suspect->refcnt);
			إذا (opt & OUTPUT_SHOW_NAME)
				چاپ_کن(" %-*.*s", longest_file, longest_file,
				       suspect->مسیر);
			إذا (opt & OUTPUT_SHOW_عدد)
				چاپ_کن(" %*d", max_orig_digits,
				       ent->s_lno + 1 + cnt);

			إذا (!(opt & OUTPUT_NO_AUTHOR)) {
				ثابت حرف *name;
				صحيح pad;
				إذا (opt & OUTPUT_SHOW_EMAIL)
					name = ci.author_mail.buf;
				وإلا
					name = ci.author.buf;
				pad = longest_author - utf8_strwidth(name);
				چاپ_کن(" (%s%*s %10s",
				       name, pad, "",
				       format_زمان(ci.author_زمان,
						   ci.author_tz.buf,
						   show_raw_زمان));
			}
			چاپ_کن(" %*d) ",
			       max_digits, ent->lno + 1 + cnt);
		}
		إذا (reset)
			fputs(reset, stdout);
		افعل {
			ch = *cp++;
			حرف_چاپ_کن(ch);
		} طالما (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_اندازه);
	}

	إذا (sb->final_buf_اندازه && cp[-1] != '\n')
		حرف_چاپ_کن('\n');

	commit_info_destroy(&ci);
}

ساكن فراغ output(بنية blame_scoreboard *sb, صحيح option)
{
	بنية blame_مدخل *ent, *prev_ent = عدم;

	إذا (option & OUTPUT_PORCELAIN) {
		لكل (ent = sb->ent; ent; ent = ent->بعدی) {
			صحيح شمار = 0;
			بنية blame_origin *suspect;
			بنية commit *commit = ent->suspect->commit;
			إذا (commit->object.flags & بیشتر_THAN_ONE_مسیر)
				استمر;
			لكل (suspect = get_blame_suspects(commit); suspect; suspect = suspect->بعدی) {
				إذا (suspect->guilty && شمار++) {
					commit->object.flags |= بیشتر_THAN_ONE_مسیر;
					اكسر;
				}
			}
		}
	}

	لكل (ent = sb->ent; ent; ent = ent->بعدی) {
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
		بنية commit_graft *graft = read_graft_سطر(&buf);
		إذا (graft)
			ثبات_commit_graft(the_repository, graft, 0);
	}
	fclose(fp);
	strbuf_release(&buf);
	أرجع 0;
}

ساكن صحيح update_auto_abbrev(صحيح auto_abbrev, بنية blame_origin *suspect)
{
	ثابت حرف *uniq = repo_پیدا_کن_unique_abbrev(the_repository,
						   &suspect->commit->object.oid,
						   auto_abbrev);
	صحيح len = طول_رشته(uniq);
	إذا (auto_abbrev < len)
		أرجع len;
	أرجع auto_abbrev;
}

/*
 * How many columns do we need to show line numbers, authors,
 * and filenames?
 */
ساكن فراغ پیدا_کن_alignment(بنية blame_scoreboard *sb, صحيح *option)
{
	صحيح longest_src_سطرها = 0;
	صحيح longest_dst_سطرها = 0;
	بیعلامت بزرگترین_score = 0;
	بنية blame_مدخل *e;
	صحيح compute_auto_abbrev = (abbrev < 0);
	صحيح auto_abbrev = DEFAULT_ABBREV;
	اندازه_t max_marks_شمار = 0;

	لكل (e = sb->ent; e; e = e->بعدی) {
		بنية blame_origin *suspect = e->suspect;
		صحيح رقم;
		اندازه_t marks_شمار = شمار_marks(e, *option);

		إذا (max_marks_شمار < marks_شمار)
			max_marks_شمار = marks_شمار;
		إذا (compute_auto_abbrev)
			auto_abbrev = update_auto_abbrev(auto_abbrev, suspect);
		إذا (strcmp(suspect->مسیر, sb->مسیر))
			*option |= OUTPUT_SHOW_NAME;
		رقم = طول_رشته(suspect->مسیر);
		إذا (longest_file < رقم)
			longest_file = رقم;
		إذا (!(suspect->commit->object.flags & METAINFO_SHOWN)) {
			بنية commit_info ci = COMMIT_INFO_INIT;
			suspect->commit->object.flags |= METAINFO_SHOWN;
			get_commit_info(suspect->commit, &ci);
			إذا (*option & OUTPUT_SHOW_EMAIL)
				رقم = utf8_strwidth(ci.author_mail.buf);
			وإلا
				رقم = utf8_strwidth(ci.author.buf);
			إذا (longest_author < رقم)
				longest_author = رقم;
			commit_info_destroy(&ci);
		}
		رقم = e->s_lno + e->رقم_سطرها;
		إذا (longest_src_سطرها < رقم)
			longest_src_سطرها = رقم;
		رقم = e->lno + e->رقم_سطرها;
		إذا (longest_dst_سطرها < رقم)
			longest_dst_سطرها = رقم;
		إذا (بزرگترین_score < blame_مدخل_score(sb, e))
			بزرگترین_score = blame_مدخل_score(sb, e);
	}
	max_orig_digits = decimal_عرض(longest_src_سطرها);
	max_digits = decimal_عرض(longest_dst_سطرها);
	max_score_digits = decimal_عرض(بزرگترین_score);

	إذا (compute_auto_abbrev)
		abbrev = auto_abbrev;
	إذا (abbrev < (صحيح)the_hash_algo->hexsz) {
		abbrev += max_marks_شمار;
		إذا (abbrev > (صحيح)the_hash_algo->hexsz)
			abbrev = the_hash_algo->hexsz;
	}
}

ساكن فراغ sanity_check_on_fail(بنية blame_scoreboard *sb, صحيح baa)
{
	صحيح opt = OUTPUT_SHOW_SCORE | OUTPUT_SHOW_عدد | OUTPUT_SHOW_NAME;
	پیدا_کن_alignment(sb, &opt);
	output(sb, opt);
	die("Baa %d!", baa);
}

ساكن بیعلامت parse_score(ثابت حرف *arg)
{
	حرف *انتها;
	بیعلامت دراز score = strtoul(arg, &انتها, 10);
	إذا (*انتها)
		أرجع 0;
	أرجع score;
}

ساكن حرف *جمع_کن_پیشوند(ثابت حرف *پیشوند, ثابت حرف *مسیر)
{
	أرجع پیشوند_مسیر(the_repository, پیشوند, پیشوند ? طول_رشته(پیشوند) : 0, مسیر);
}

ساكن صحيح git_blame_config(ثابت حرف *var, ثابت حرف *مقدار,
			    ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, "blame.showroot")) {
		show_ریشه = git_config_bool(var, مقدار);
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.blankboundary")) {
		blank_boundary = git_config_bool(var, مقدار);
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.showemail")) {
		صحيح *output_option = cb;
		إذا (git_config_bool(var, مقدار))
			*output_option |= OUTPUT_SHOW_EMAIL;
		وإلا
			*output_option &= ~OUTPUT_SHOW_EMAIL;
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.date")) {
		إذا (!مقدار)
			أرجع config_error_nonbool(var);
		parse_date_format(مقدار, &blame_date_نما);
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.ignorerevsfile")) {
		حرف *str;
		صحيح ret;

		ret = git_config_pathname(&str, var, مقدار);
		إذا (ret)
			أرجع ret;
		إذا (str)
			string_list_درج(&ignore_revs_file_list, str);
		حرر(str);
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.markunblamablelines")) {
		mark_unblamable_سطرها = git_config_bool(var, مقدار);
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.markignoredlines")) {
		mark_ignored_سطرها = git_config_bool(var, مقدار);
		أرجع 0;
	}
	إذا (!strcmp(var, "color.blame.repeatedlines")) {
		إذا (color_parse_mem(مقدار, طول_رشته(مقدار), repeated_meta_color))
			warning(_("invalid value for '%s': '%s'"),
				"color.blame.repeatedLines", مقدار);
		أرجع 0;
	}
	إذا (!strcmp(var, "color.blame.highlightrecent")) {
		parse_color_fields(مقدار);
		أرجع 0;
	}

	إذا (!strcmp(var, "blame.coloring")) {
		إذا (!مقدار)
			أرجع config_error_nonbool(var);
		إذا (!strcmp(مقدار, "repeatedLines")) {
			coloring_نما |= OUTPUT_COLOR_سطر;
		} وإلا إذا (!strcmp(مقدار, "highlightRecent")) {
			coloring_نما |= OUTPUT_SHOW_AGE_WITH_COLOR;
		} وإلا إذا (!strcmp(مقدار, "none")) {
			coloring_نما &= ~(OUTPUT_COLOR_سطر |
					    OUTPUT_SHOW_AGE_WITH_COLOR);
		} وإلا {
			warning(_("invalid value for '%s': '%s'"),
				"blame.coloring", مقدار);
			أرجع 0;
		}
	}

	إذا (!strcmp(var, "diff.algorithm")) {
		دراز diff_algorithm;
		إذا (!مقدار)
			أرجع config_error_nonbool(var);
		diff_algorithm = parse_algorithm_مقدار(مقدار);
		إذا (diff_algorithm < 0)
			أرجع error(_("unknown value for config '%s': %s"),
				     var, مقدار);
		xdl_opts &= ~XDF_DIFF_ALGORITHM_ماسک;
		xdl_opts |= diff_algorithm;
		أرجع 0;
	}

	إذا (git_diff_heuristic_config(var, مقدار, cb) < 0)
		أرجع -1;
	إذا (userdiff_config(var, مقدار) < 0)
		أرجع -1;

	أرجع git_default_config(var, مقدار, ctx, cb);
}

ساكن صحيح blame_copy_callback(ثابت بنية option *option, ثابت حرف *arg, صحيح unset)
{
	صحيح *opt = option->مقدار;

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
	صحيح *opt = option->مقدار;

	BUG_ON_OPT_NEG(unset);

	*opt |= PICKAXE_BLAME_MOVE;

	إذا (arg)
		blame_move_score = parse_score(arg);
	أرجع 0;
}

ساكن صحيح blame_diff_algorithm_کمینه(ثابت بنية option *option,
					ثابت حرف *arg, صحيح unset)
{
	صحيح *opt = option->مقدار;

	BUG_ON_OPT_ARG(arg);

	*opt &= ~XDF_DIFF_ALGORITHM_ماسک;
	إذا (!unset)
		*opt |= XDF_NEED_کمینه;

	أرجع 0;
}

ساكن صحيح blame_diff_algorithm_callback(ثابت بنية option *option,
					 ثابت حرف *arg, صحيح unset)
{
	صحيح *opt = option->مقدار;
	دراز مقدار = parse_algorithm_مقدار(arg);

	BUG_ON_OPT_NEG(unset);

	إذا (مقدار < 0)
		أرجع error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	*opt &= ~XDF_DIFF_ALGORITHM_ماسک;
	*opt |= مقدار;

	أرجع 0;
}

ساكن صحيح is_a_rev(ثابت حرف *name)
{
	بنية object_id oid;

	إذا (repo_get_oid(the_repository, name, &oid))
		أرجع 0;
	أرجع OBJ_NONE < odb_read_object_info(the_repository->objects, &oid, عدم);
}

ساكن صحيح peel_مقصد_commit_oid(بنية object_id *oid_ret, فراغ *cbdata)
{
	بنية repository *r = ((بنية blame_scoreboard *)cbdata)->repo;
	بنية object_id oid;

	oidcpy(&oid, oid_ret);
	طالما (1) {
		بنية object *obj;
		صحيح نوع = odb_read_object_info(r->objects, &oid, عدم);
		إذا (نوع == OBJ_COMMIT) {
			oidcpy(oid_ret, &oid);
			أرجع 0;
		}
		إذا (نوع != OBJ_برچسب)
			أرجع -1;
		obj = deref_برچسب(r, parse_object(r, &oid), عدم, 0);
		إذا (!obj)
			أرجع -1;
		oidcpy(&oid, &obj->oid);
	}
}

ساكن فراغ build_ignorelist(بنية blame_scoreboard *sb,
			     بنية string_list *ignore_revs_file_list,
			     بنية string_list *ignore_rev_list)
{
	بنية string_list_قلم *i;
	بنية object_id oid;

	oidset_init(&sb->ignore_list, 0);
	for_each_string_list_قلم(i, ignore_revs_file_list) {
		إذا (!strcmp(i->string, ""))
			oidset_clear(&sb->ignore_list);
		وإلا
			oidset_parse_file_carefully(&sb->ignore_list, i->string,
						    the_repository->hash_algo,
						    peel_مقصد_commit_oid, sb);
	}
	for_each_string_list_قلم(i, ignore_rev_list) {
		إذا (repo_get_oid_committish(the_repository, i->string, &oid) ||
		    peel_مقصد_commit_oid(&oid, sb))
			die(_("cannot find revision %s to ignore"), i->string);
		oidset_درج(&sb->ignore_list, &oid);
	}
}

صحيح cmd_blame(صحيح argc,
	      ثابت حرف **argv,
	      ثابت حرف *پیشوند,
	      بنية repository *repo استفادهنشده)
{
	بنية rev_info revs;
	حرف *مسیر = عدم;
	بنية blame_scoreboard sb;
	بنية blame_origin *o;
	بنية blame_مدخل *ent = عدم;
	دراز dashdash_pos, lno;
	بنية پیشرفت_info pi = { عدم, 0 };

	بنية string_list range_list = STRING_LIST_INIT_NODUP;
	بنية string_list ignore_rev_list = STRING_LIST_INIT_NODUP;
	صحيح output_option = 0, opt = 0;
	صحيح show_stats = 0;
	ثابت حرف *revs_file = عدم;
	ثابت حرف *contents_from = عدم;
	ثابت بنية option options[] = {
		OPT_BOOL(0, "incremental", &incremental, N_("show blame entries as we find them, incrementally")),
		OPT_BOOL('b', عدم, &blank_boundary, N_("do not show object names of boundary commits (Default: off)")),
		OPT_BOOL(0, "root", &show_ریشه, N_("do not treat root commits as boundaries (Default: off)")),
		OPT_BOOL(0, "show-stats", &show_stats, N_("show work cost statistics")),
		OPT_BOOL(0, "progress", &show_پیشرفت, N_("force progress reporting")),
		OPT_بیت(0, "score-debug", &output_option, N_("show output score for blame entries"), OUTPUT_SHOW_SCORE),
		OPT_بیت('f', "show-name", &output_option, N_("show original filename (Default: auto)"), OUTPUT_SHOW_NAME),
		OPT_بیت('n', "show-number", &output_option, N_("show original linenumber (Default: off)"), OUTPUT_SHOW_عدد),
		OPT_بیت('p', "porcelain", &output_option, N_("show in a format designed for machine consumption"), OUTPUT_PORCELAIN),
		OPT_بیت(0, "line-porcelain", &output_option, N_("show porcelain format with per-line commit information"), OUTPUT_PORCELAIN|OUTPUT_سطر_PORCELAIN),
		OPT_بیت('c', عدم, &output_option, N_("use the same output mode as git-annotate (Default: off)"), OUTPUT_ANNOTATE_COMPAT),
		OPT_بیت('t', عدم, &output_option, N_("show raw timestamp (Default: off)"), OUTPUT_RAW_TIMESTAMP),
		OPT_بیت('l', عدم, &output_option, N_("show long commit SHA1 (Default: off)"), OUTPUT_LONG_OBJECT_NAME),
		OPT_بیت('s', عدم, &output_option, N_("suppress author name and timestamp (Default: off)"), OUTPUT_NO_AUTHOR),
		OPT_بیت('e', "show-email", &output_option, N_("show author email instead of name (Default: off)"), OUTPUT_SHOW_EMAIL),
		OPT_بیت('w', عدم, &xdl_opts, N_("ignore whitespace differences"), XDF_IGNORE_WHITESPACE),
		OPT_CALLBACK_F(0, "diff-algorithm", &xdl_opts, N_("<algorithm>"),
			       N_("choose a diff algorithm"),
			       PARSE_OPT_NONEG, blame_diff_algorithm_callback),
		OPT_STRING_LIST(0, "ignore-rev", &ignore_rev_list, N_("rev"), N_("ignore <rev> when blaming")),
		OPT_STRING_LIST(0, "ignore-revs-file", &ignore_revs_file_list, N_("file"), N_("ignore revisions from <file>")),
		OPT_بیت(0, "color-lines", &output_option, N_("color redundant metadata from previous line differently"), OUTPUT_COLOR_سطر),
		OPT_بیت(0, "color-by-age", &output_option, N_("color lines by age"), OUTPUT_SHOW_AGE_WITH_COLOR),
		OPT_CALLBACK_F(0, "minimal", &xdl_opts, عدم,
			       N_("spend extra cycles to find a better match"),
			       PARSE_OPT_NOARG | PARSE_OPT_HIDDEN, blame_diff_algorithm_کمینه),
		OPT_STRING('S', عدم, &revs_file, N_("file"), N_("use revisions from <file> instead of calling git-rev-list")),
		OPT_STRING(0, "contents", &contents_from, N_("file"), N_("use <file>'s contents as the final image")),
		OPT_CALLBACK_F('C', عدم, &opt, N_("score"), N_("find line copies within and across files"), PARSE_OPT_OPTARG, blame_copy_callback),
		OPT_CALLBACK_F('M', عدم, &opt, N_("score"), N_("find line movements within and across files"), PARSE_OPT_OPTARG, blame_move_callback),
		OPT_STRING_LIST('L', عدم, &range_list, N_("range"),
				N_("process only line range <start>,<end> or function :<funcname>")),
		OPT__ABBREV(&abbrev),
		OPT_انتها()
	};

	بنية parse_opt_ctx_t ctx;
	صحيح cmd_is_annotate = !strcmp(argv[0], "annotate");
	بنية range_مجموعه ranges;
	بیعلامت صحيح range_i;
	دراز anchor;
	دراز رقم_سطرها = 0;
	ثابت حرف *str_usage = cmd_is_annotate ? annotate_usage : blame_usage;
	ثابت حرف *ثابت *opt_usage = cmd_is_annotate ? annotate_opt_usage : blame_opt_usage;

	راهاندازی_default_color_by_age();
	repo_config(the_repository, git_blame_config, &output_option);
	repo_init_revisions(the_repository, &revs, عدم);
	revs.date_نما = blame_date_نما;
	revs.diffopt.flags.allow_textconv = 1;
	revs.diffopt.flags.follow_renames = 1;

	save_commit_میانگیر = 0;
	dashdash_pos = 0;
	show_پیشرفت = -1;

	parse_options_شروع(&ctx, argc, argv, پیشوند, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);
	لكل (;;) {
		بدل (parse_options_گام(&ctx, options, opt_usage)) {
		حالة PARSE_OPT_NON_OPTION:
		حالة PARSE_OPT_UNKNOWN:
			اكسر;
		حالة PARSE_OPT_HELP:
			خروج(0);
		حالة PARSE_OPT_HELP_ERROR:
		حالة PARSE_OPT_ERROR:
		حالة PARSE_OPT_SUBCOMMAND:
			خروج(129);
		حالة PARSE_OPT_COMPLETE:
			خروج(0);
		حالة PARSE_OPT_انجامشده:
			إذا (ctx.argv[0])
				dashdash_pos = ctx.cpidx;
			اذهب parse_انجامشده;
		}

		إذا (!strcmp(ctx.argv[0], "--reverse")) {
			ctx.argv[0] = "--children";
			معکوس = 1;
		}
		parse_revision_opt(&revs, &ctx, options, opt_usage);
	}
parse_انجامشده:
	revision_opts_پایان(&revs);
	no_کامل_file_rename = !revs.diffopt.flags.follow_renames;
	xdl_opts |= revs.diffopt.xdl_opts & XDF_INDENT_HEURISTIC;
	revs.diffopt.flags.follow_renames = 0;
	argc = parse_options_انتها(&ctx);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_پر_شاخص = 0;

	إذا (incremental || (output_option & OUTPUT_PORCELAIN)) {
		إذا (show_پیشرفت > 0)
			die(_("--progress can't be used with --incremental or porcelain formats"));
		show_پیشرفت = 0;
	} وإلا إذا (show_پیشرفت < 0)
		show_پیشرفت = isatty(2);

	إذا (!abbrev)
		abbrev = the_hash_algo->hexsz;

	إذا (revs_file && read_ancestry(revs_file))
		die_errno("reading graft file '%s' failed", revs_file);

	إذا (cmd_is_annotate) {
		output_option |= OUTPUT_ANNOTATE_COMPAT;
		blame_date_نما.type = DATE_ISO8601;
	} وإلا {
		blame_date_نما = revs.date_نما;
	}

	/* The maximum width used to show the dates */
	بدل (blame_date_نما.type) {
	حالة DATE_RFC2822:
		blame_date_عرض = حجم("Thu, 19 Oct 2006 16:00:04 -0700");
		اكسر;
	حالة DATE_ISO8601_STRICT:
		blame_date_عرض = حجم("2006-10-19T16:00:04-07:00");
		اكسر;
	حالة DATE_ISO8601:
		blame_date_عرض = حجم("2006-10-19 16:00:04 -0700");
		اكسر;
	حالة DATE_RAW:
		blame_date_عرض = حجم("1161298804 -0700");
		اكسر;
	حالة DATE_UNIX:
		blame_date_عرض = حجم("1161298804");
		اكسر;
	حالة DATE_SHORT:
		blame_date_عرض = حجم("2006-10-19");
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
		blame_date_عرض = utf8_strwidth(_("4 years, 11 months ago")) + 1; /* add the null */
		اكسر;
	حالة DATE_HUMAN:
		/* If the year is shown, no time is shown */
		blame_date_عرض = حجم("Thu Oct 19 16:00");
		اكسر;
	حالة DATE_نرمال:
		blame_date_عرض = حجم("Thu Oct 19 16:00:04 2006 -0700");
		اكسر;
	حالة DATE_STRFTIME:
		blame_date_عرض = طول_رشته(show_date(0, 0, blame_date_نما)) + 1; /* add the null */
		اكسر;
	}
	blame_date_عرض -= 1; /* strip the null */

	إذا (revs.diffopt.flags.پیدا_کن_copies_harder)
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
			مسیر = جمع_کن_پیشوند(پیشوند, argv[--argc]);
			argv[argc] = عدم;
			اكسر;
		افتراضي:
			usage_with_options(opt_usage, options);
		}
	} وإلا {
		إذا (argc < 2)
			usage_with_options(opt_usage, options);
		إذا (argc == 3 && is_a_rev(argv[argc - 1])) { /* (2b) */
			مسیر = جمع_کن_پیشوند(پیشوند, argv[1]);
			argv[1] = argv[2];
		} وإلا {	/* (2a) */
			إذا (argc == 2 && is_a_rev(argv[1]) && !repo_get_work_درخت(the_repository))
				die("missing <path> to blame");
			مسیر = جمع_کن_پیشوند(پیشوند, argv[argc - 1]);
		}
		argv[argc - 1] = "--";
	}

	revs.disable_stdin = 1;
	راهاندازی_revisions(argc, argv, &revs, عدم);
	إذا (!revs.pending.nr && is_bare_repository(the_repository)) {
		بنية commit *سر_commit;
		بنية object_id سر_oid;

		إذا (!refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", RESOLVE_REF_READING,
					     &سر_oid, عدم) ||
		    !(سر_commit = lookup_commit_reference_gently(revs.repo,
							     &سر_oid, 1)))
			die("no such ref: HEAD");

		جمع_کن_pending_object(&revs, &سر_commit->object, "HEAD");
	}

	init_scoreboard(&sb);
	sb.revs = &revs;
	sb.contents_from = contents_from;
	sb.معکوس = معکوس;
	sb.repo = the_repository;
	sb.مسیر = مسیر;
	build_ignorelist(&sb, &ignore_revs_file_list, &ignore_rev_list);
	string_list_clear(&ignore_revs_file_list, 0);
	string_list_clear(&ignore_rev_list, 0);
	راهاندازی_scoreboard(&sb, &o);

	/*
	 * Changed-path Bloom filters are disabled when looking
	 * for copies.
	 */
	إذا (!(opt & PICKAXE_BLAME_COPY))
		راهاندازی_blame_bloom_داده(&sb);

	lno = sb.رقم_سطرها;

	إذا (lno && !range_list.nr)
		string_list_append(&range_list, "1");

	anchor = 1;
	range_مجموعه_init(&ranges, range_list.nr);
	لكل (range_i = 0; range_i < range_list.nr; ++range_i) {
		دراز bottom, بالاترین;
		إذا (parse_range_arg(range_list.اقلام[range_i].string,
				    nth_سطر_cb, &sb, lno, anchor,
				    &bottom, &بالاترین, sb.مسیر,
				    the_repository->شاخص))
			usage(str_usage);
		إذا ((!lno && (بالاترین || bottom)) || lno < bottom)
			die(Q_("file %s has only %lu line",
			       "file %s has only %lu lines",
			       lno), sb.مسیر, lno);
		إذا (bottom < 1)
			bottom = 1;
		إذا (بالاترین < 1 || lno < بالاترین)
			بالاترین = lno;
		bottom--;
		range_مجموعه_append_unsafe(&ranges, bottom, بالاترین);
		anchor = بالاترین + 1;
	}
	مرتب_سازی_and_ادغام_range_مجموعه(&ranges);

	لكل (range_i = ranges.nr; range_i > 0; --range_i) {
		ثابت بنية range *r = &ranges.ranges[range_i - 1];
		ent = blame_مدخل_prepend(ent, r->شروع, r->انتها, o);
		رقم_سطرها += (r->انتها - r->شروع);
	}
	إذا (!رقم_سطرها)
		رقم_سطرها = sb.رقم_سطرها;

	o->suspects = ent;
	prio_صف_put(&sb.commits, o->commit);

	blame_origin_decref(o);

	range_مجموعه_release(&ranges);
	string_list_clear(&range_list, 0);

	sb.ent = عدم;

	إذا (blame_move_score)
		sb.move_score = blame_move_score;
	إذا (blame_copy_score)
		sb.copy_score = blame_copy_score;

	sb.debug = DEBUG_BLAME;
	sb.on_sanity_fail = &sanity_check_on_fail;

	sb.show_ریشه = show_ریشه;
	sb.xdl_opts = xdl_opts;
	sb.no_کامل_file_rename = no_کامل_file_rename;

	read_mailmap(the_repository, &mailmap);

	sb.یافت_guilty_مدخل = &یافت_guilty_مدخل;
	sb.یافت_guilty_مدخل_داده = &pi;
	إذا (show_پیشرفت)
		pi.پیشرفت = شروع_delayed_پیشرفت(the_repository,
						     _("Blaming lines"),
						     رقم_سطرها);

	تخصیص_بده_blame(&sb, opt);

	stop_پیشرفت(&pi.پیشرفت);

	إذا (!incremental)
		راهاندازی_pager(the_repository);
	وإلا
		اذهب cleanup;

	blame_مرتب_سازی_final(&sb);

	blame_coalesce(&sb);

	إذا (!(output_option & (OUTPUT_COLOR_سطر | OUTPUT_SHOW_AGE_WITH_COLOR)))
		output_option |= coloring_نما;

	إذا (!(output_option & OUTPUT_PORCELAIN)) {
		پیدا_کن_alignment(&sb, &output_option);
		إذا (!*repeated_meta_color &&
		    (output_option & OUTPUT_COLOR_سطر))
			xsnprintf(repeated_meta_color,
				  حجم(repeated_meta_color),
				  "%s", GIT_COLOR_CYAN);
	}
	إذا (output_option & OUTPUT_ANNOTATE_COMPAT)
		output_option &= ~(OUTPUT_COLOR_سطر | OUTPUT_SHOW_AGE_WITH_COLOR);

	output(&sb, output_option);

	إذا (show_stats) {
		چاپ_کن("num read blob: %d\n", sb.رقم_read_blob);
		چاپ_کن("num get patch: %d\n", sb.رقم_get_patch);
		چاپ_کن("num commits: %d\n", sb.رقم_commits);
	}

cleanup:
	لكل (ent = sb.ent; ent; ) {
		بنية blame_مدخل *e = ent->بعدی;
		حرر(ent);
		ent = e;
	}

	حرر(مسیر);
	cleanup_scoreboard(&sb);
	release_revisions(&revs);
	أرجع 0;
}
