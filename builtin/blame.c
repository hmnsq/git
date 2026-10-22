/*
 * Blame
 *
 * Copyright (c) 2006, 2014 by its authors
 * See COPYING for licensing conditions
 */

#تعریف_کړه USE_THE_REPOSITORY_VARIABLE

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
ساكن صحيح show_ریښه;
ساكن صحيح سرچپه;
ساكن صحيح blank_boundary;
ساكن صحيح incremental;
ساكن صحيح xdl_opts;
ساكن صحيح abbrev = -1;
ساكن صحيح no_ټولیز_file_rename;
ساكن صحيح show_پرمختګ;
ساكن حرف repeated_meta_color[COLOR_MAXLEN];
ساكن صحيح coloring_ډېرپېښ;
ساكن بنية string_list ignore_revs_file_list = STRING_LIST_INIT_DUP;
ساكن صحيح mark_unblamable_کرښې;
ساكن صحيح mark_ignored_کرښې;

ساكن بنية date_ډېرپېښ blame_date_ډېرپېښ = { DATE_ISO8601 };
ساكن اندازه_t blame_date_پلنوالی;

ساكن بنية string_list mailmap = STRING_LIST_INIT_NODUP;

#که_نه_تعریف_شوی DEBUG_BLAME
#تعریف_کړه DEBUG_BLAME 0
#د_که_پای

ساكن بینښه blame_move_score;
ساكن بینښه blame_copy_score;

/* Remember to update object flag allocation in object.h */
#تعریف_کړه METAINFO_SHOWN		(1u<<12)
#تعریف_کړه نور_THAN_ONE_لار	(1u<<13)

بنية پرمختګ_info {
	بنية پرمختګ *پرمختګ;
	صحيح blamed_کرښې;
};

ساكن ثابت حرف *nth_کرښه_cb(فراغ *معلومات, اوږد lno)
{
	أرجع blame_nth_کرښه((بنية blame_scoreboard *)معلومات, lno);
}

/*
 * Information on commits, used for output.
 */
بنية commit_info {
	بنية strbuf author;
	بنية strbuf author_mail;
	timestamp_t author_وخت;
	بنية strbuf author_tz;

	/* filled only when asked for details */
	بنية strbuf committer;
	بنية strbuf committer_mail;
	timestamp_t committer_وخت;
	بنية strbuf committer_tz;

	بنية strbuf summary;
};

#تعریف_کړه COMMIT_INFO_INIT { \
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
ساكن فراغ get_ac_کرښه(ثابت حرف *inbuf, ثابت حرف *what,
	بنية strbuf *name, بنية strbuf *mail,
	timestamp_t *وخت, بنية strbuf *tz)
{
	بنية ident_وېشل ident;
	اندازه_t len, maillen, namelen;
	ثابت حرف *tmp, *endp;
	ثابت حرف *namebuf, *mailbuf;

	tmp = مزي_پلټنه(inbuf, what);
	إذا (!tmp)
		اذهب error_بهر;
	tmp += مزي_اوږدوالی(what);
	endp = strchr(tmp, '\n');
	إذا (!endp)
		len = مزي_اوږدوالی(tmp);
	وإلا
		len = endp - tmp;

	إذا (وېشل_ident_کرښه(&ident, tmp, len)) {
	error_بهر:
		/* Ugh */
		tmp = "(unknown)";
		strbuf_addstr(name, tmp);
		strbuf_addstr(mail, tmp);
		strbuf_addstr(tz, tmp);
		*وخت = 0;
		أرجع;
	}

	namelen = ident.name_پای_ټکی - ident.name_begin;
	namebuf = ident.name_begin;

	maillen = ident.mail_پای_ټکی - ident.mail_begin;
	mailbuf = ident.mail_begin;

	إذا (ident.date_begin && ident.date_پای_ټکی)
		*وخت = strtoul(ident.date_begin, عدم, 10);
	وإلا
		*وخت = 0;

	إذا (ident.tz_begin && ident.tz_پای_ټکی)
		strbuf_جمع_کړه(tz, ident.tz_begin, ident.tz_پای_ټکی - ident.tz_begin);
	وإلا
		strbuf_addstr(tz, "(unknown)");

	/*
	 * Now, convert both name and e-mail using mailmap
	 */
	map_user(&mailmap, &mailbuf, &maillen,
		 &namebuf, &namelen);

	strbuf_addf(mail, "<%.*s>", (صحيح)maillen, mailbuf);
	strbuf_جمع_کړه(name, namebuf, namelen);
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
	ثابت حرف *پیغام_2;

	encoding = get_log_output_encoding();
	پیغام_2 = repo_logmsg_reencode(the_repository, commit, عدم, encoding);
	get_ac_کرښه(پیغام_2, "\nauthor ",
		    &ret->author, &ret->author_mail,
		    &ret->author_وخت, &ret->author_tz);

	get_ac_کرښه(پیغام_2, "\ncommitter ",
		    &ret->committer, &ret->committer_mail,
		    &ret->committer_وخت, &ret->committer_tz);

	len = پیدا_کړه_commit_subject(پیغام_2, &subject);
	إذا (len)
		strbuf_جمع_کړه(&ret->summary, subject, len);
	وإلا
		strbuf_addf(&ret->summary, "(%s)", oid_منزل_hex(&commit->object.oid));

	repo_unuse_commit_بفر(the_repository, commit, پیغام_2);
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
	إذا (suspect->پخوانی) {
		بنية blame_origin *prev = suspect->پخوانی;
		چاپ_کړه("previous %s ", oid_منزل_hex(&prev->commit->object.oid));
		write_name_quoted(prev->لار, stdout, '\n');
	}
	چاپ_کړه("filename ");
	write_name_quoted(suspect->لار, stdout, '\n');
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
	چاپ_کړه("author %s\n", ci.author.buf);
	چاپ_کړه("author-mail %s\n", ci.author_mail.buf);
	چاپ_کړه("author-time %"PRItime"\n", ci.author_وخت);
	چاپ_کړه("author-tz %s\n", ci.author_tz.buf);
	چاپ_کړه("committer %s\n", ci.committer.buf);
	چاپ_کړه("committer-mail %s\n", ci.committer_mail.buf);
	چاپ_کړه("committer-time %"PRItime"\n", ci.committer_وخت);
	چاپ_کړه("committer-tz %s\n", ci.committer_tz.buf);
	چاپ_کړه("summary %s\n", ci.summary.buf);
	إذا (suspect->commit->object.flags & UNINTERESTING)
		چاپ_کړه("boundary\n");

	commit_info_destroy(&ci);

	أرجع 1;
}

/*
 * The blame_entry is found to be guilty for the range.
 * Show it in incremental output.
 */
ساكن فراغ وموندل_guilty_ثبت(بنية blame_ثبت *ent, فراغ *معلومات)
{
	بنية پرمختګ_info *pi = (بنية پرمختګ_info *)معلومات;

	إذا (incremental) {
		بنية blame_origin *suspect = ent->suspect;

		چاپ_کړه("%s %d %d %d\n",
		       oid_منزل_hex(&suspect->commit->object.oid),
		       ent->s_lno + 1, ent->lno + 1, ent->عدد_کرښې);
		emit_one_suspect_detail(suspect, 0);
		write_filename_info(suspect);
		maybe_flush_or_die(stdout, "stdout");
	}
	pi->blamed_کرښې += ent->عدد_کرښې;
	display_پرمختګ(pi->پرمختګ, pi->blamed_کرښې);
}

ساكن ثابت حرف *format_وخت(timestamp_t وخت, ثابت حرف *tz_str,
			       صحيح show_raw_وخت)
{
	ساكن بنية strbuf وخت_buf = STRBUF_INIT;

	strbuf_reset(&وخت_buf);
	إذا (show_raw_وخت) {
		strbuf_addf(&وخت_buf, "%"PRItime" %s", وخت, tz_str);
	}
	وإلا {
		ثابت حرف *وخت_str;
		اندازه_t وخت_پلنوالی;
		صحيح tz;
		tz = atoi(tz_str);
		وخت_str = show_date(وخت, tz, blame_date_ډېرپېښ);
		strbuf_addstr(&وخت_buf, وخت_str);
		/*
		 * Add space paddings to time_buf to display a fixed width
		 * string, and use time_width for display width calibration.
		 */
		لكل (وخت_پلنوالی = utf8_strwidth(وخت_str);
		     وخت_پلنوالی < blame_date_پلنوالی;
		     وخت_پلنوالی++)
			strbuf_addch(&وخت_buf, ' ');
	}
	أرجع وخت_buf.buf;
}

#تعریف_کړه OUTPUT_ANNOTATE_COMPAT      (1U<<0)
#تعریف_کړه OUTPUT_LONG_OBJECT_NAME     (1U<<1)
#تعریف_کړه OUTPUT_RAW_TIMESTAMP        (1U<<2)
#تعریف_کړه OUTPUT_PORCELAIN            (1U<<3)
#تعریف_کړه OUTPUT_SHOW_NAME            (1U<<4)
#تعریف_کړه OUTPUT_SHOW_شمېره          (1U<<5)
#تعریف_کړه OUTPUT_SHOW_SCORE           (1U<<6)
#تعریف_کړه OUTPUT_NO_AUTHOR            (1U<<7)
#تعریف_کړه OUTPUT_SHOW_EMAIL           (1U<<8)
#تعریف_کړه OUTPUT_کرښه_PORCELAIN       (1U<<9)
#تعریف_کړه OUTPUT_COLOR_کرښه           (1U<<10)
#تعریف_کړه OUTPUT_SHOW_AGE_WITH_COLOR  (1U<<11)

ساكن فراغ emit_porcelain_details(بنية blame_origin *suspect, صحيح repeat)
{
	إذا (emit_one_suspect_detail(suspect, repeat) ||
	    (suspect->commit->object.flags & نور_THAN_ONE_لار))
		write_filename_info(suspect);
}

/*
 * Information which needs to be printed per-line goes here. Any
 * information which can be clubbed on a commit/file level, should
 * be printed via 'emit_one_suspect_detail()'.
 */
ساكن فراغ emit_porcelain_فی_کرښه_details(بنية blame_ثبت *ent)
{
	إذا (mark_unblamable_کرښې && ent->unblamable)
		puts("unblamable");
	إذا (mark_ignored_کرښې && ent->ignored)
		puts("ignored");
}

ساكن فراغ emit_porcelain(بنية blame_scoreboard *sb, بنية blame_ثبت *ent,
			   صحيح opt)
{
	صحيح repeat = opt & OUTPUT_کرښه_PORCELAIN;
	صحيح cnt;
	ثابت حرف *cp;
	بنية blame_origin *suspect = ent->suspect;
	حرف hex[GIT_MAX_HEXSZ + 1];

	oid_منزل_hex_r(hex, &suspect->commit->object.oid);
	چاپ_کړه("%s %d %d %d\n",
	       hex,
	       ent->s_lno + 1,
	       ent->lno + 1,
	       ent->عدد_کرښې);
	emit_porcelain_details(suspect, repeat);
	emit_porcelain_فی_کرښه_details(ent);

	cp = blame_nth_کرښه(sb, ent->lno);
	لكل (cnt = 0; cnt < ent->عدد_کرښې; cnt++) {
		حرف ch;
		إذا (cnt) {
			چاپ_کړه("%s %d %d\n", hex,
			       ent->s_lno + 1 + cnt,
			       ent->lno + 1 + cnt);
			إذا (repeat)
				emit_porcelain_details(suspect, 1);
			emit_porcelain_فی_کرښه_details(ent);
		}
		توری_چاپ_کړه('\t');
		افعل {
			ch = *cp++;
			توری_چاپ_کړه(ch);
		} طالما (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_اندازه);
	}

	إذا (sb->final_buf_اندازه && cp[-1] != '\n')
		توری_چاپ_کړه('\n');
}

ساكن بنية color_ډګر {
	timestamp_t hop;
	حرف ستون[COLOR_MAXLEN];
} *colorfield;
ساكن صحيح colorfield_nr, colorfield_alloc;

ساكن فراغ parse_color_fields(ثابت حرف *s)
{
	بنية string_list l = STRING_LIST_INIT_DUP;
	بنية string_list_توکی *توکی;
	تعداد { EXPECT_DATE, EXPECT_COLOR } بل = EXPECT_COLOR;

	colorfield_nr = 0;

	/* Ideally this would be stripped and split at the same time? */
	string_list_وېشل(&l, s, ",", -1);
	ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);

	for_each_string_list_توکی(توکی, &l) {
		بدل (بل) {
		حالة EXPECT_DATE:
			colorfield[colorfield_nr].hop = approxidate(توکی->string);
			بل = EXPECT_COLOR;
			colorfield_nr++;
			ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);
			اكسر;
		حالة EXPECT_COLOR:
			إذا (color_parse(توکی->string, colorfield[colorfield_nr].ستون))
				die(_("expecting a color: %s"), توکی->string);
			بل = EXPECT_DATE;
			اكسر;
		}
	}

	إذا (بل == EXPECT_COLOR)
		die(_("must end with a color"));

	colorfield[colorfield_nr].hop = وخت_MAX;
	string_list_clear(&l, 0);
}

ساكن فراغ تیاری_default_color_by_age(فراغ)
{
	parse_color_fields("blue,12 month ago,white,1 month ago,red");
}

ساكن فراغ determine_کرښه_heat(بنية commit_info *ci, ثابت حرف **dest_color)
{
	صحيح i = 0;

	طالما (i < colorfield_nr && ci->author_وخت > colorfield[i].hop)
		i++;

	*dest_color = colorfield[i].ستون;
}

ساكن مضمن صحيح maybe_putc(صحيح c, FILE *بهر)
{
	أرجع بهر ? putc(c, بهر) : 0;
}

ساكن اندازه_t print_marks(FILE *بهر, ثابت بنية blame_ثبت *ent, صحيح opt)
{
	اندازه_t len = 0;

	إذا ((ent->suspect->commit->object.flags & UNINTERESTING) &&
	    !blank_boundary && !(opt & OUTPUT_ANNOTATE_COMPAT)) {
		maybe_putc('^', بهر);
		len++;
	}
	إذا (mark_unblamable_کرښې && ent->unblamable) {
		maybe_putc('*', بهر);
		len++;
	}
	إذا (mark_ignored_کرښې && ent->ignored) {
		maybe_putc('?', بهر);
		len++;
	}
	أرجع len;
}

ساكن اندازه_t شمېر_marks(ثابت بنية blame_ثبت *ent, صحيح opt)
{
	أرجع print_marks(عدم, ent, opt);
}

ساكن فراغ emit_other(بنية blame_scoreboard *sb, بنية blame_ثبت *ent,
		       صحيح opt, بنية blame_ثبت *prev_ent)
{
	صحيح cnt;
	ثابت حرف *cp;
	بنية blame_origin *suspect = ent->suspect;
	بنية commit_info ci = COMMIT_INFO_INIT;
	حرف hex[GIT_MAX_HEXSZ + 1];
	صحيح show_raw_وخت = !!(opt & OUTPUT_RAW_TIMESTAMP);
	ثابت حرف *default_color = عدم, *color = عدم, *reset = عدم;

	get_commit_info(suspect->commit, &ci);
	oid_منزل_hex_r(hex, &suspect->commit->object.oid);

	cp = blame_nth_کرښه(sb, ent->lno);

	إذا (opt & OUTPUT_SHOW_AGE_WITH_COLOR) {
		determine_کرښه_heat(&ci, &default_color);
		color = default_color;
		reset = GIT_COLOR_RESET;
	}

	إذا (abbrev < MINIMUM_ABBREV)
		BUG("abbreviation is smaller than minimum length: %d < %d",
		    abbrev, MINIMUM_ABBREV);

	لكل (cnt = 0; cnt < ent->عدد_کرښې; cnt++) {
		حرف ch;
		اندازه_t اوږدتیا = (opt & OUTPUT_LONG_OBJECT_NAME) ?
			the_hash_algo->hexsz : (اندازه_t) abbrev;

		إذا (opt & OUTPUT_COLOR_کرښه) {
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
			memset(hex, ' ', مزي_اوږدوالی(hex));
		اوږدتیا -= print_marks(stdout, ent, opt);

		چاپ_کړه("%.*s", (صحيح)(اوږدتیا < GIT_MAX_HEXSZ ? اوږدتیا : GIT_MAX_HEXSZ), hex);
		إذا (opt & OUTPUT_ANNOTATE_COMPAT) {
			ثابت حرف *name;
			إذا (opt & OUTPUT_SHOW_EMAIL)
				name = ci.author_mail.buf;
			وإلا
				name = ci.author.buf;
			چاپ_کړه("\t(%10s\t%10s\t%d)", name,
			       format_وخت(ci.author_وخت, ci.author_tz.buf,
					   show_raw_وخت),
			       ent->lno + 1 + cnt);
		} وإلا {
			إذا (opt & OUTPUT_SHOW_SCORE)
				چاپ_کړه(" %*d %02d",
				       max_score_digits, ent->score,
				       ent->suspect->refcnt);
			إذا (opt & OUTPUT_SHOW_NAME)
				چاپ_کړه(" %-*.*s", longest_file, longest_file,
				       suspect->لار);
			إذا (opt & OUTPUT_SHOW_شمېره)
				چاپ_کړه(" %*d", max_orig_digits,
				       ent->s_lno + 1 + cnt);

			إذا (!(opt & OUTPUT_NO_AUTHOR)) {
				ثابت حرف *name;
				صحيح pad;
				إذا (opt & OUTPUT_SHOW_EMAIL)
					name = ci.author_mail.buf;
				وإلا
					name = ci.author.buf;
				pad = longest_author - utf8_strwidth(name);
				چاپ_کړه(" (%s%*s %10s",
				       name, pad, "",
				       format_وخت(ci.author_وخت,
						   ci.author_tz.buf,
						   show_raw_وخت));
			}
			چاپ_کړه(" %*d) ",
			       max_digits, ent->lno + 1 + cnt);
		}
		إذا (reset)
			fputs(reset, stdout);
		افعل {
			ch = *cp++;
			توری_چاپ_کړه(ch);
		} طالما (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_اندازه);
	}

	إذا (sb->final_buf_اندازه && cp[-1] != '\n')
		توری_چاپ_کړه('\n');

	commit_info_destroy(&ci);
}

ساكن فراغ output(بنية blame_scoreboard *sb, صحيح option)
{
	بنية blame_ثبت *ent, *prev_ent = عدم;

	إذا (option & OUTPUT_PORCELAIN) {
		لكل (ent = sb->ent; ent; ent = ent->بل) {
			صحيح شمېر = 0;
			بنية blame_origin *suspect;
			بنية commit *commit = ent->suspect->commit;
			إذا (commit->object.flags & نور_THAN_ONE_لار)
				استمر;
			لكل (suspect = get_blame_suspects(commit); suspect; suspect = suspect->بل) {
				إذا (suspect->guilty && شمېر++) {
					commit->object.flags |= نور_THAN_ONE_لار;
					اكسر;
				}
			}
		}
	}

	لكل (ent = sb->ent; ent; ent = ent->بل) {
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
		بنية commit_graft *graft = read_graft_کرښه(&buf);
		إذا (graft)
			ثبتوونکی_commit_graft(the_repository, graft, 0);
	}
	fclose(fp);
	strbuf_release(&buf);
	أرجع 0;
}

ساكن صحيح update_auto_abbrev(صحيح auto_abbrev, بنية blame_origin *suspect)
{
	ثابت حرف *uniq = repo_پیدا_کړه_unique_abbrev(the_repository,
						   &suspect->commit->object.oid,
						   auto_abbrev);
	صحيح len = مزي_اوږدوالی(uniq);
	إذا (auto_abbrev < len)
		أرجع len;
	أرجع auto_abbrev;
}

/*
 * How many columns do we need to show line numbers, authors,
 * and filenames?
 */
ساكن فراغ پیدا_کړه_alignment(بنية blame_scoreboard *sb, صحيح *option)
{
	صحيح longest_src_کرښې = 0;
	صحيح longest_dst_کرښې = 0;
	بینښه تر_ټولو_لوی_score = 0;
	بنية blame_ثبت *e;
	صحيح compute_auto_abbrev = (abbrev < 0);
	صحيح auto_abbrev = DEFAULT_ABBREV;
	اندازه_t max_marks_شمېر = 0;

	لكل (e = sb->ent; e; e = e->بل) {
		بنية blame_origin *suspect = e->suspect;
		صحيح عدد;
		اندازه_t marks_شمېر = شمېر_marks(e, *option);

		إذا (max_marks_شمېر < marks_شمېر)
			max_marks_شمېر = marks_شمېر;
		إذا (compute_auto_abbrev)
			auto_abbrev = update_auto_abbrev(auto_abbrev, suspect);
		إذا (strcmp(suspect->لار, sb->لار))
			*option |= OUTPUT_SHOW_NAME;
		عدد = مزي_اوږدوالی(suspect->لار);
		إذا (longest_file < عدد)
			longest_file = عدد;
		إذا (!(suspect->commit->object.flags & METAINFO_SHOWN)) {
			بنية commit_info ci = COMMIT_INFO_INIT;
			suspect->commit->object.flags |= METAINFO_SHOWN;
			get_commit_info(suspect->commit, &ci);
			إذا (*option & OUTPUT_SHOW_EMAIL)
				عدد = utf8_strwidth(ci.author_mail.buf);
			وإلا
				عدد = utf8_strwidth(ci.author.buf);
			إذا (longest_author < عدد)
				longest_author = عدد;
			commit_info_destroy(&ci);
		}
		عدد = e->s_lno + e->عدد_کرښې;
		إذا (longest_src_کرښې < عدد)
			longest_src_کرښې = عدد;
		عدد = e->lno + e->عدد_کرښې;
		إذا (longest_dst_کرښې < عدد)
			longest_dst_کرښې = عدد;
		إذا (تر_ټولو_لوی_score < blame_ثبت_score(sb, e))
			تر_ټولو_لوی_score = blame_ثبت_score(sb, e);
	}
	max_orig_digits = decimal_پلنوالی(longest_src_کرښې);
	max_digits = decimal_پلنوالی(longest_dst_کرښې);
	max_score_digits = decimal_پلنوالی(تر_ټولو_لوی_score);

	إذا (compute_auto_abbrev)
		abbrev = auto_abbrev;
	إذا (abbrev < (صحيح)the_hash_algo->hexsz) {
		abbrev += max_marks_شمېر;
		إذا (abbrev > (صحيح)the_hash_algo->hexsz)
			abbrev = the_hash_algo->hexsz;
	}
}

ساكن فراغ sanity_check_on_fail(بنية blame_scoreboard *sb, صحيح baa)
{
	صحيح opt = OUTPUT_SHOW_SCORE | OUTPUT_SHOW_شمېره | OUTPUT_SHOW_NAME;
	پیدا_کړه_alignment(sb, &opt);
	output(sb, opt);
	die("Baa %d!", baa);
}

ساكن بینښه parse_score(ثابت حرف *arg)
{
	حرف *پای_ټکی;
	بینښه اوږد score = strtoul(arg, &پای_ټکی, 10);
	إذا (*پای_ټکی)
		أرجع 0;
	أرجع score;
}

ساكن حرف *جمع_کړه_مختاړی(ثابت حرف *مختاړی, ثابت حرف *لار)
{
	أرجع مختاړی_لار(the_repository, مختاړی, مختاړی ? مزي_اوږدوالی(مختاړی) : 0, لار);
}

ساكن صحيح git_blame_config(ثابت حرف *var, ثابت حرف *ارزښت,
			    ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, "blame.showroot")) {
		show_ریښه = git_config_bool(var, ارزښت);
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.blankboundary")) {
		blank_boundary = git_config_bool(var, ارزښت);
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.showemail")) {
		صحيح *output_option = cb;
		إذا (git_config_bool(var, ارزښت))
			*output_option |= OUTPUT_SHOW_EMAIL;
		وإلا
			*output_option &= ~OUTPUT_SHOW_EMAIL;
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.date")) {
		إذا (!ارزښت)
			أرجع config_error_nonbool(var);
		parse_date_format(ارزښت, &blame_date_ډېرپېښ);
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.ignorerevsfile")) {
		حرف *str;
		صحيح ret;

		ret = git_config_pathname(&str, var, ارزښت);
		إذا (ret)
			أرجع ret;
		إذا (str)
			string_list_ورننباسه(&ignore_revs_file_list, str);
		حرر(str);
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.markunblamablelines")) {
		mark_unblamable_کرښې = git_config_bool(var, ارزښت);
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.markignoredlines")) {
		mark_ignored_کرښې = git_config_bool(var, ارزښت);
		أرجع 0;
	}
	إذا (!strcmp(var, "color.blame.repeatedlines")) {
		إذا (color_parse_mem(ارزښت, مزي_اوږدوالی(ارزښت), repeated_meta_color))
			warning(_("invalid value for '%s': '%s'"),
				"color.blame.repeatedLines", ارزښت);
		أرجع 0;
	}
	إذا (!strcmp(var, "color.blame.highlightrecent")) {
		parse_color_fields(ارزښت);
		أرجع 0;
	}

	إذا (!strcmp(var, "blame.coloring")) {
		إذا (!ارزښت)
			أرجع config_error_nonbool(var);
		إذا (!strcmp(ارزښت, "repeatedLines")) {
			coloring_ډېرپېښ |= OUTPUT_COLOR_کرښه;
		} وإلا إذا (!strcmp(ارزښت, "highlightRecent")) {
			coloring_ډېرپېښ |= OUTPUT_SHOW_AGE_WITH_COLOR;
		} وإلا إذا (!strcmp(ارزښت, "none")) {
			coloring_ډېرپېښ &= ~(OUTPUT_COLOR_کرښه |
					    OUTPUT_SHOW_AGE_WITH_COLOR);
		} وإلا {
			warning(_("invalid value for '%s': '%s'"),
				"blame.coloring", ارزښت);
			أرجع 0;
		}
	}

	إذا (!strcmp(var, "diff.algorithm")) {
		اوږد diff_algorithm;
		إذا (!ارزښت)
			أرجع config_error_nonbool(var);
		diff_algorithm = parse_algorithm_ارزښت(ارزښت);
		إذا (diff_algorithm < 0)
			أرجع error(_("unknown value for config '%s': %s"),
				     var, ارزښت);
		xdl_opts &= ~XDF_DIFF_ALGORITHM_ماسک;
		xdl_opts |= diff_algorithm;
		أرجع 0;
	}

	إذا (git_diff_heuristic_config(var, ارزښت, cb) < 0)
		أرجع -1;
	إذا (userdiff_config(var, ارزښت) < 0)
		أرجع -1;

	أرجع git_default_config(var, ارزښت, ctx, cb);
}

ساكن صحيح blame_copy_callback(ثابت بنية option *option, ثابت حرف *arg, صحيح unset)
{
	صحيح *opt = option->ارزښت;

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
	صحيح *opt = option->ارزښت;

	BUG_ON_OPT_NEG(unset);

	*opt |= PICKAXE_BLAME_MOVE;

	إذا (arg)
		blame_move_score = parse_score(arg);
	أرجع 0;
}

ساكن صحيح blame_diff_algorithm_اصغري(ثابت بنية option *option,
					ثابت حرف *arg, صحيح unset)
{
	صحيح *opt = option->ارزښت;

	BUG_ON_OPT_ARG(arg);

	*opt &= ~XDF_DIFF_ALGORITHM_ماسک;
	إذا (!unset)
		*opt |= XDF_NEED_اصغري;

	أرجع 0;
}

ساكن صحيح blame_diff_algorithm_callback(ثابت بنية option *option,
					 ثابت حرف *arg, صحيح unset)
{
	صحيح *opt = option->ارزښت;
	اوږد ارزښت = parse_algorithm_ارزښت(arg);

	BUG_ON_OPT_NEG(unset);

	إذا (ارزښت < 0)
		أرجع error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	*opt &= ~XDF_DIFF_ALGORITHM_ماسک;
	*opt |= ارزښت;

	أرجع 0;
}

ساكن صحيح is_a_rev(ثابت حرف *name)
{
	بنية object_id oid;

	إذا (repo_get_oid(the_repository, name, &oid))
		أرجع 0;
	أرجع OBJ_NONE < odb_read_object_info(the_repository->objects, &oid, عدم);
}

ساكن صحيح peel_منزل_commit_oid(بنية object_id *oid_ret, فراغ *cbdata)
{
	بنية repository *r = ((بنية blame_scoreboard *)cbdata)->repo;
	بنية object_id oid;

	oidcpy(&oid, oid_ret);
	طالما (1) {
		بنية object *obj;
		صحيح ډول = odb_read_object_info(r->objects, &oid, عدم);
		إذا (ډول == OBJ_COMMIT) {
			oidcpy(oid_ret, &oid);
			أرجع 0;
		}
		إذا (ډول != OBJ_نښان)
			أرجع -1;
		obj = deref_نښان(r, parse_object(r, &oid), عدم, 0);
		إذا (!obj)
			أرجع -1;
		oidcpy(&oid, &obj->oid);
	}
}

ساكن فراغ build_ignorelist(بنية blame_scoreboard *sb,
			     بنية string_list *ignore_revs_file_list,
			     بنية string_list *ignore_rev_list)
{
	بنية string_list_توکی *i;
	بنية object_id oid;

	oidset_init(&sb->ignore_list, 0);
	for_each_string_list_توکی(i, ignore_revs_file_list) {
		إذا (!strcmp(i->string, ""))
			oidset_clear(&sb->ignore_list);
		وإلا
			oidset_parse_file_carefully(&sb->ignore_list, i->string,
						    the_repository->hash_algo,
						    peel_منزل_commit_oid, sb);
	}
	for_each_string_list_توکی(i, ignore_rev_list) {
		إذا (repo_get_oid_committish(the_repository, i->string, &oid) ||
		    peel_منزل_commit_oid(&oid, sb))
			die(_("cannot find revision %s to ignore"), i->string);
		oidset_ورننباسه(&sb->ignore_list, &oid);
	}
}

صحيح cmd_blame(صحيح argc,
	      ثابت حرف **argv,
	      ثابت حرف *مختاړی,
	      بنية repository *repo ناکارول_شوی)
{
	بنية rev_info revs;
	حرف *لار = عدم;
	بنية blame_scoreboard sb;
	بنية blame_origin *o;
	بنية blame_ثبت *ent = عدم;
	اوږد dashdash_pos, lno;
	بنية پرمختګ_info pi = { عدم, 0 };

	بنية string_list range_list = STRING_LIST_INIT_NODUP;
	بنية string_list ignore_rev_list = STRING_LIST_INIT_NODUP;
	صحيح output_option = 0, opt = 0;
	صحيح show_stats = 0;
	ثابت حرف *revs_file = عدم;
	ثابت حرف *contents_from = عدم;
	ثابت بنية option options[] = {
		OPT_BOOL(0, "incremental", &incremental, N_("show blame entries as we find them, incrementally")),
		OPT_BOOL('b', عدم, &blank_boundary, N_("do not show object names of boundary commits (Default: off)")),
		OPT_BOOL(0, "root", &show_ریښه, N_("do not treat root commits as boundaries (Default: off)")),
		OPT_BOOL(0, "show-stats", &show_stats, N_("show work cost statistics")),
		OPT_BOOL(0, "progress", &show_پرمختګ, N_("force progress reporting")),
		OPT_بټ(0, "score-debug", &output_option, N_("show output score for blame entries"), OUTPUT_SHOW_SCORE),
		OPT_بټ('f', "show-name", &output_option, N_("show original filename (Default: auto)"), OUTPUT_SHOW_NAME),
		OPT_بټ('n', "show-number", &output_option, N_("show original linenumber (Default: off)"), OUTPUT_SHOW_شمېره),
		OPT_بټ('p', "porcelain", &output_option, N_("show in a format designed for machine consumption"), OUTPUT_PORCELAIN),
		OPT_بټ(0, "line-porcelain", &output_option, N_("show porcelain format with per-line commit information"), OUTPUT_PORCELAIN|OUTPUT_کرښه_PORCELAIN),
		OPT_بټ('c', عدم, &output_option, N_("use the same output mode as git-annotate (Default: off)"), OUTPUT_ANNOTATE_COMPAT),
		OPT_بټ('t', عدم, &output_option, N_("show raw timestamp (Default: off)"), OUTPUT_RAW_TIMESTAMP),
		OPT_بټ('l', عدم, &output_option, N_("show long commit SHA1 (Default: off)"), OUTPUT_LONG_OBJECT_NAME),
		OPT_بټ('s', عدم, &output_option, N_("suppress author name and timestamp (Default: off)"), OUTPUT_NO_AUTHOR),
		OPT_بټ('e', "show-email", &output_option, N_("show author email instead of name (Default: off)"), OUTPUT_SHOW_EMAIL),
		OPT_بټ('w', عدم, &xdl_opts, N_("ignore whitespace differences"), XDF_IGNORE_WHITESPACE),
		OPT_CALLBACK_F(0, "diff-algorithm", &xdl_opts, N_("<algorithm>"),
			       N_("choose a diff algorithm"),
			       PARSE_OPT_NONEG, blame_diff_algorithm_callback),
		OPT_STRING_LIST(0, "ignore-rev", &ignore_rev_list, N_("rev"), N_("ignore <rev> when blaming")),
		OPT_STRING_LIST(0, "ignore-revs-file", &ignore_revs_file_list, N_("file"), N_("ignore revisions from <file>")),
		OPT_بټ(0, "color-lines", &output_option, N_("color redundant metadata from previous line differently"), OUTPUT_COLOR_کرښه),
		OPT_بټ(0, "color-by-age", &output_option, N_("color lines by age"), OUTPUT_SHOW_AGE_WITH_COLOR),
		OPT_CALLBACK_F(0, "minimal", &xdl_opts, عدم,
			       N_("spend extra cycles to find a better match"),
			       PARSE_OPT_NOARG | PARSE_OPT_HIDDEN, blame_diff_algorithm_اصغري),
		OPT_STRING('S', عدم, &revs_file, N_("file"), N_("use revisions from <file> instead of calling git-rev-list")),
		OPT_STRING(0, "contents", &contents_from, N_("file"), N_("use <file>'s contents as the final image")),
		OPT_CALLBACK_F('C', عدم, &opt, N_("score"), N_("find line copies within and across files"), PARSE_OPT_OPTARG, blame_copy_callback),
		OPT_CALLBACK_F('M', عدم, &opt, N_("score"), N_("find line movements within and across files"), PARSE_OPT_OPTARG, blame_move_callback),
		OPT_STRING_LIST('L', عدم, &range_list, N_("range"),
				N_("process only line range <start>,<end> or function :<funcname>")),
		OPT__ABBREV(&abbrev),
		OPT_پای_ټکی()
	};

	بنية parse_opt_ctx_t ctx;
	صحيح cmd_is_annotate = !strcmp(argv[0], "annotate");
	بنية range_سېټ ranges;
	بینښه صحيح range_i;
	اوږد anchor;
	اوږد عدد_کرښې = 0;
	ثابت حرف *str_usage = cmd_is_annotate ? annotate_usage : blame_usage;
	ثابت حرف *ثابت *opt_usage = cmd_is_annotate ? annotate_opt_usage : blame_opt_usage;

	تیاری_default_color_by_age();
	repo_config(the_repository, git_blame_config, &output_option);
	repo_init_revisions(the_repository, &revs, عدم);
	revs.date_ډېرپېښ = blame_date_ډېرپېښ;
	revs.diffopt.flags.allow_textconv = 1;
	revs.diffopt.flags.follow_renames = 1;

	save_commit_بفر = 0;
	dashdash_pos = 0;
	show_پرمختګ = -1;

	parse_options_پیل(&ctx, argc, argv, مختاړی, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);
	لكل (;;) {
		بدل (parse_options_ګام(&ctx, options, opt_usage)) {
		حالة PARSE_OPT_NON_OPTION:
		حالة PARSE_OPT_UNKNOWN:
			اكسر;
		حالة PARSE_OPT_HELP:
			وتل(0);
		حالة PARSE_OPT_HELP_ERROR:
		حالة PARSE_OPT_ERROR:
		حالة PARSE_OPT_SUBCOMMAND:
			وتل(129);
		حالة PARSE_OPT_COMPLETE:
			وتل(0);
		حالة PARSE_OPT_بشپړ:
			إذا (ctx.argv[0])
				dashdash_pos = ctx.cpidx;
			اذهب parse_بشپړ;
		}

		إذا (!strcmp(ctx.argv[0], "--reverse")) {
			ctx.argv[0] = "--children";
			سرچپه = 1;
		}
		parse_revision_opt(&revs, &ctx, options, opt_usage);
	}
parse_بشپړ:
	revision_opts_پای(&revs);
	no_ټولیز_file_rename = !revs.diffopt.flags.follow_renames;
	xdl_opts |= revs.diffopt.xdl_opts & XDF_INDENT_HEURISTIC;
	revs.diffopt.flags.follow_renames = 0;
	argc = parse_options_پای_ټکی(&ctx);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ډک_شاخص = 0;

	إذا (incremental || (output_option & OUTPUT_PORCELAIN)) {
		إذا (show_پرمختګ > 0)
			die(_("--progress can't be used with --incremental or porcelain formats"));
		show_پرمختګ = 0;
	} وإلا إذا (show_پرمختګ < 0)
		show_پرمختګ = isatty(2);

	إذا (!abbrev)
		abbrev = the_hash_algo->hexsz;

	إذا (revs_file && read_ancestry(revs_file))
		die_errno("reading graft file '%s' failed", revs_file);

	إذا (cmd_is_annotate) {
		output_option |= OUTPUT_ANNOTATE_COMPAT;
		blame_date_ډېرپېښ.type = DATE_ISO8601;
	} وإلا {
		blame_date_ډېرپېښ = revs.date_ډېرپېښ;
	}

	/* The maximum width used to show the dates */
	بدل (blame_date_ډېرپېښ.type) {
	حالة DATE_RFC2822:
		blame_date_پلنوالی = حجم("Thu, 19 Oct 2006 16:00:04 -0700");
		اكسر;
	حالة DATE_ISO8601_STRICT:
		blame_date_پلنوالی = حجم("2006-10-19T16:00:04-07:00");
		اكسر;
	حالة DATE_ISO8601:
		blame_date_پلنوالی = حجم("2006-10-19 16:00:04 -0700");
		اكسر;
	حالة DATE_RAW:
		blame_date_پلنوالی = حجم("1161298804 -0700");
		اكسر;
	حالة DATE_UNIX:
		blame_date_پلنوالی = حجم("1161298804");
		اكسر;
	حالة DATE_SHORT:
		blame_date_پلنوالی = حجم("2006-10-19");
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
		blame_date_پلنوالی = utf8_strwidth(_("4 years, 11 months ago")) + 1; /* add the null */
		اكسر;
	حالة DATE_HUMAN:
		/* If the year is shown, no time is shown */
		blame_date_پلنوالی = حجم("Thu Oct 19 16:00");
		اكسر;
	حالة DATE_نورمال:
		blame_date_پلنوالی = حجم("Thu Oct 19 16:00:04 2006 -0700");
		اكسر;
	حالة DATE_STRFTIME:
		blame_date_پلنوالی = مزي_اوږدوالی(show_date(0, 0, blame_date_ډېرپېښ)) + 1; /* add the null */
		اكسر;
	}
	blame_date_پلنوالی -= 1; /* strip the null */

	إذا (revs.diffopt.flags.پیدا_کړه_copies_harder)
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
			لار = جمع_کړه_مختاړی(مختاړی, argv[--argc]);
			argv[argc] = عدم;
			اكسر;
		افتراضي:
			usage_with_options(opt_usage, options);
		}
	} وإلا {
		إذا (argc < 2)
			usage_with_options(opt_usage, options);
		إذا (argc == 3 && is_a_rev(argv[argc - 1])) { /* (2b) */
			لار = جمع_کړه_مختاړی(مختاړی, argv[1]);
			argv[1] = argv[2];
		} وإلا {	/* (2a) */
			إذا (argc == 2 && is_a_rev(argv[1]) && !repo_get_work_ونه(the_repository))
				die("missing <path> to blame");
			لار = جمع_کړه_مختاړی(مختاړی, argv[argc - 1]);
		}
		argv[argc - 1] = "--";
	}

	revs.disable_stdin = 1;
	تیاری_revisions(argc, argv, &revs, عدم);
	إذا (!revs.pending.nr && is_bare_repository(the_repository)) {
		بنية commit *سر_commit;
		بنية object_id سر_oid;

		إذا (!refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", RESOLVE_REF_READING,
					     &سر_oid, عدم) ||
		    !(سر_commit = lookup_commit_reference_gently(revs.repo,
							     &سر_oid, 1)))
			die("no such ref: HEAD");

		جمع_کړه_pending_object(&revs, &سر_commit->object, "HEAD");
	}

	init_scoreboard(&sb);
	sb.revs = &revs;
	sb.contents_from = contents_from;
	sb.سرچپه = سرچپه;
	sb.repo = the_repository;
	sb.لار = لار;
	build_ignorelist(&sb, &ignore_revs_file_list, &ignore_rev_list);
	string_list_clear(&ignore_revs_file_list, 0);
	string_list_clear(&ignore_rev_list, 0);
	تیاری_scoreboard(&sb, &o);

	/*
	 * Changed-path Bloom filters are disabled when looking
	 * for copies.
	 */
	إذا (!(opt & PICKAXE_BLAME_COPY))
		تیاری_blame_bloom_معلومات(&sb);

	lno = sb.عدد_کرښې;

	إذا (lno && !range_list.nr)
		string_list_append(&range_list, "1");

	anchor = 1;
	range_سېټ_init(&ranges, range_list.nr);
	لكل (range_i = 0; range_i < range_list.nr; ++range_i) {
		اوږد bottom, سر_2;
		إذا (parse_range_arg(range_list.توکي[range_i].string,
				    nth_کرښه_cb, &sb, lno, anchor,
				    &bottom, &سر_2, sb.لار,
				    the_repository->شاخص))
			usage(str_usage);
		إذا ((!lno && (سر_2 || bottom)) || lno < bottom)
			die(Q_("file %s has only %lu line",
			       "file %s has only %lu lines",
			       lno), sb.لار, lno);
		إذا (bottom < 1)
			bottom = 1;
		إذا (سر_2 < 1 || lno < سر_2)
			سر_2 = lno;
		bottom--;
		range_سېټ_append_unsafe(&ranges, bottom, سر_2);
		anchor = سر_2 + 1;
	}
	ترتیبول_and_یوځای_range_سېټ(&ranges);

	لكل (range_i = ranges.nr; range_i > 0; --range_i) {
		ثابت بنية range *r = &ranges.ranges[range_i - 1];
		ent = blame_ثبت_prepend(ent, r->پیل, r->پای_ټکی, o);
		عدد_کرښې += (r->پای_ټکی - r->پیل);
	}
	إذا (!عدد_کرښې)
		عدد_کرښې = sb.عدد_کرښې;

	o->suspects = ent;
	prio_کتار_put(&sb.commits, o->commit);

	blame_origin_decref(o);

	range_سېټ_release(&ranges);
	string_list_clear(&range_list, 0);

	sb.ent = عدم;

	إذا (blame_move_score)
		sb.move_score = blame_move_score;
	إذا (blame_copy_score)
		sb.copy_score = blame_copy_score;

	sb.debug = DEBUG_BLAME;
	sb.on_sanity_fail = &sanity_check_on_fail;

	sb.show_ریښه = show_ریښه;
	sb.xdl_opts = xdl_opts;
	sb.no_ټولیز_file_rename = no_ټولیز_file_rename;

	read_mailmap(the_repository, &mailmap);

	sb.وموندل_guilty_ثبت = &وموندل_guilty_ثبت;
	sb.وموندل_guilty_ثبت_معلومات = &pi;
	إذا (show_پرمختګ)
		pi.پرمختګ = پیل_delayed_پرمختګ(the_repository,
						     _("Blaming lines"),
						     عدد_کرښې);

	ټاکل_blame(&sb, opt);

	stop_پرمختګ(&pi.پرمختګ);

	إذا (!incremental)
		تیاری_pager(the_repository);
	وإلا
		اذهب cleanup;

	blame_ترتیبول_final(&sb);

	blame_coalesce(&sb);

	إذا (!(output_option & (OUTPUT_COLOR_کرښه | OUTPUT_SHOW_AGE_WITH_COLOR)))
		output_option |= coloring_ډېرپېښ;

	إذا (!(output_option & OUTPUT_PORCELAIN)) {
		پیدا_کړه_alignment(&sb, &output_option);
		إذا (!*repeated_meta_color &&
		    (output_option & OUTPUT_COLOR_کرښه))
			xsnprintf(repeated_meta_color,
				  حجم(repeated_meta_color),
				  "%s", GIT_COLOR_CYAN);
	}
	إذا (output_option & OUTPUT_ANNOTATE_COMPAT)
		output_option &= ~(OUTPUT_COLOR_کرښه | OUTPUT_SHOW_AGE_WITH_COLOR);

	output(&sb, output_option);

	إذا (show_stats) {
		چاپ_کړه("num read blob: %d\n", sb.عدد_read_blob);
		چاپ_کړه("num get patch: %d\n", sb.عدد_get_patch);
		چاپ_کړه("num commits: %d\n", sb.عدد_commits);
	}

cleanup:
	لكل (ent = sb.ent; ent; ) {
		بنية blame_ثبت *e = ent->بل;
		حرر(ent);
		ent = e;
	}

	حرر(لار);
	cleanup_scoreboard(&sb);
	release_revisions(&revs);
	أرجع 0;
}
