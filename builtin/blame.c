/*
 * Blame
 *
 * Copyright (c) 2006, 2014 by its authors
 * See COPYING for licensing conditions
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

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

ساکن ثابت حرف blame_usage[] = N_("git blame [<options>] [<rev-opts>] [<rev>] [--] <file>");
ساکن ثابت حرف annotate_usage[] = N_("git annotate [<options>] [<rev-opts>] [<rev>] [--] <file>");

ساکن ثابت حرف *ثابت blame_opt_usage[] = {
	blame_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	تہی
};

ساکن ثابت حرف *ثابت annotate_opt_usage[] = {
	annotate_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	تہی
};

ساکن صحیح longest_file;
ساکن صحیح longest_author;
ساکن صحیح max_orig_digits;
ساکن صحیح max_digits;
ساکن صحیح max_score_digits;
ساکن صحیح show_جڑ;
ساکن صحیح الٹ;
ساکن صحیح blank_boundary;
ساکن صحیح incremental;
ساکن صحیح xdl_opts;
ساکن صحیح abbrev = -1;
ساکن صحیح no_پورا_file_rename;
ساکن صحیح show_پیش_رفت;
ساکن حرف repeated_meta_color[COLOR_MAXLEN];
ساکن صحیح coloring_منوال;
ساکن ساخت string_list ignore_revs_file_list = STRING_LIST_INIT_DUP;
ساکن صحیح mark_unblamable_سطریں;
ساکن صحیح mark_ignored_سطریں;

ساکن ساخت date_منوال blame_date_منوال = { DATE_ISO8601 };
ساکن سائز_t blame_date_چوڑائی;

ساکن ساخت string_list mailmap = STRING_LIST_INIT_NODUP;

#اگر_غیر_تعریف_شدہ DEBUG_BLAME
#تعریف_کرو DEBUG_BLAME 0
#اگر_ختم

ساکن غیرعلامتی blame_move_score;
ساکن غیرعلامتی blame_copy_score;

/* Remember to update object flag allocation in object.h */
#تعریف_کرو METAINFO_SHOWN		(1u<<12)
#تعریف_کرو مزید_THAN_ONE_راستہ	(1u<<13)

ساخت پیش_رفت_info {
	ساخت پیش_رفت *پیش_رفت;
	صحیح blamed_سطریں;
};

ساکن ثابت حرف *nth_سطر_cb(خالی *ڈیٹا, طویل lno)
{
	واپس blame_nth_سطر((ساخت blame_scoreboard *)ڈیٹا, lno);
}

/*
 * Information on commits, used for output.
 */
ساخت commit_info {
	ساخت strbuf author;
	ساخت strbuf author_mail;
	timestamp_t author_وقت;
	ساخت strbuf author_tz;

	/* filled only when asked for details */
	ساخت strbuf committer;
	ساخت strbuf committer_mail;
	timestamp_t committer_وقت;
	ساخت strbuf committer_tz;

	ساخت strbuf summary;
};

#تعریف_کرو COMMIT_INFO_INIT { \
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
ساکن خالی get_ac_سطر(ثابت حرف *inbuf, ثابت حرف *what,
	ساخت strbuf *name, ساخت strbuf *mail,
	timestamp_t *وقت, ساخت strbuf *tz)
{
	ساخت ident_بانٹو ident;
	سائز_t len, maillen, namelen;
	ثابت حرف *tmp, *endp;
	ثابت حرف *namebuf, *mailbuf;

	tmp = لڑی_تلاش(inbuf, what);
	اگر (!tmp)
		جاؤ error_برآمد;
	tmp += لڑی_لمبائی(what);
	endp = strchr(tmp, '\n');
	اگر (!endp)
		len = لڑی_لمبائی(tmp);
	ورنہ
		len = endp - tmp;

	اگر (بانٹو_ident_سطر(&ident, tmp, len)) {
	error_برآمد:
		/* Ugh */
		tmp = "(unknown)";
		strbuf_addstr(name, tmp);
		strbuf_addstr(mail, tmp);
		strbuf_addstr(tz, tmp);
		*وقت = 0;
		واپس;
	}

	namelen = ident.name_انتہا - ident.name_begin;
	namebuf = ident.name_begin;

	maillen = ident.mail_انتہا - ident.mail_begin;
	mailbuf = ident.mail_begin;

	اگر (ident.date_begin && ident.date_انتہا)
		*وقت = strtoul(ident.date_begin, تہی, 10);
	ورنہ
		*وقت = 0;

	اگر (ident.tz_begin && ident.tz_انتہا)
		strbuf_جوڑو(tz, ident.tz_begin, ident.tz_انتہا - ident.tz_begin);
	ورنہ
		strbuf_addstr(tz, "(unknown)");

	/*
	 * Now, convert both name and e-mail using mailmap
	 */
	map_user(&mailmap, &mailbuf, &maillen,
		 &namebuf, &namelen);

	strbuf_addf(mail, "<%.*s>", (صحیح)maillen, mailbuf);
	strbuf_جوڑو(name, namebuf, namelen);
}

ساکن خالی commit_info_destroy(ساخت commit_info *ci)
{

	strbuf_release(&ci->author);
	strbuf_release(&ci->author_mail);
	strbuf_release(&ci->author_tz);
	strbuf_release(&ci->committer);
	strbuf_release(&ci->committer_mail);
	strbuf_release(&ci->committer_tz);
	strbuf_release(&ci->summary);
}

ساکن خالی get_commit_info(ساخت commit *commit, ساخت commit_info *ret)
{
	صحیح len;
	ثابت حرف *subject, *encoding;
	ثابت حرف *پیغام_2;

	encoding = get_log_output_encoding();
	پیغام_2 = repo_logmsg_reencode(the_repository, commit, تہی, encoding);
	get_ac_سطر(پیغام_2, "\nauthor ",
		    &ret->author, &ret->author_mail,
		    &ret->author_وقت, &ret->author_tz);

	get_ac_سطر(پیغام_2, "\ncommitter ",
		    &ret->committer, &ret->committer_mail,
		    &ret->committer_وقت, &ret->committer_tz);

	len = ڈھونڈو_commit_subject(پیغام_2, &subject);
	اگر (len)
		strbuf_جوڑو(&ret->summary, subject, len);
	ورنہ
		strbuf_addf(&ret->summary, "(%s)", oid_مقصد_hex(&commit->object.oid));

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
ساکن خالی write_filename_info(ساخت blame_origin *suspect)
{
	اگر (suspect->پچھلا) {
		ساخت blame_origin *پچھلا_2 = suspect->پچھلا;
		لکھو("previous %s ", oid_مقصد_hex(&پچھلا_2->commit->object.oid));
		write_name_quoted(پچھلا_2->راستہ, stdout, '\n');
	}
	لکھو("filename ");
	write_name_quoted(suspect->راستہ, stdout, '\n');
}

/*
 * Porcelain/Incremental format wants to show a lot of details per
 * commit.  Instead of repeating this every line, emit it only once,
 * the first time each commit appears in the output (unless the
 * user has specifically asked for us to repeat).
 */
ساکن صحیح emit_one_suspect_detail(ساخت blame_origin *suspect, صحیح repeat)
{
	ساخت commit_info ci = COMMIT_INFO_INIT;

	اگر (!repeat && (suspect->commit->object.flags & METAINFO_SHOWN))
		واپس 0;

	suspect->commit->object.flags |= METAINFO_SHOWN;
	get_commit_info(suspect->commit, &ci);
	لکھو("author %s\n", ci.author.buf);
	لکھو("author-mail %s\n", ci.author_mail.buf);
	لکھو("author-time %"PRItime"\n", ci.author_وقت);
	لکھو("author-tz %s\n", ci.author_tz.buf);
	لکھو("committer %s\n", ci.committer.buf);
	لکھو("committer-mail %s\n", ci.committer_mail.buf);
	لکھو("committer-time %"PRItime"\n", ci.committer_وقت);
	لکھو("committer-tz %s\n", ci.committer_tz.buf);
	لکھو("summary %s\n", ci.summary.buf);
	اگر (suspect->commit->object.flags & UNINTERESTING)
		لکھو("boundary\n");

	commit_info_destroy(&ci);

	واپس 1;
}

/*
 * The blame_entry is found to be guilty for the range.
 * Show it in incremental output.
 */
ساکن خالی ملا_guilty_اندراج(ساخت blame_اندراج *ent, خالی *ڈیٹا)
{
	ساخت پیش_رفت_info *pi = (ساخت پیش_رفت_info *)ڈیٹا;

	اگر (incremental) {
		ساخت blame_origin *suspect = ent->suspect;

		لکھو("%s %d %d %d\n",
		       oid_مقصد_hex(&suspect->commit->object.oid),
		       ent->s_lno + 1, ent->lno + 1, ent->عدد_سطریں);
		emit_one_suspect_detail(suspect, 0);
		write_filename_info(suspect);
		maybe_flush_or_die(stdout, "stdout");
	}
	pi->blamed_سطریں += ent->عدد_سطریں;
	display_پیش_رفت(pi->پیش_رفت, pi->blamed_سطریں);
}

ساکن ثابت حرف *format_وقت(timestamp_t وقت, ثابت حرف *tz_str,
			       صحیح show_raw_وقت)
{
	ساکن ساخت strbuf وقت_buf = STRBUF_INIT;

	strbuf_reset(&وقت_buf);
	اگر (show_raw_وقت) {
		strbuf_addf(&وقت_buf, "%"PRItime" %s", وقت, tz_str);
	}
	ورنہ {
		ثابت حرف *وقت_str;
		سائز_t وقت_چوڑائی;
		صحیح tz;
		tz = atoi(tz_str);
		وقت_str = show_date(وقت, tz, blame_date_منوال);
		strbuf_addstr(&وقت_buf, وقت_str);
		/*
		 * Add space paddings to time_buf to display a fixed width
		 * string, and use time_width for display width calibration.
		 */
		برائے (وقت_چوڑائی = utf8_strwidth(وقت_str);
		     وقت_چوڑائی < blame_date_چوڑائی;
		     وقت_چوڑائی++)
			strbuf_addch(&وقت_buf, ' ');
	}
	واپس وقت_buf.buf;
}

#تعریف_کرو OUTPUT_ANNOTATE_COMPAT      (1U<<0)
#تعریف_کرو OUTPUT_LONG_OBJECT_NAME     (1U<<1)
#تعریف_کرو OUTPUT_RAW_TIMESTAMP        (1U<<2)
#تعریف_کرو OUTPUT_PORCELAIN            (1U<<3)
#تعریف_کرو OUTPUT_SHOW_NAME            (1U<<4)
#تعریف_کرو OUTPUT_SHOW_نمبر          (1U<<5)
#تعریف_کرو OUTPUT_SHOW_SCORE           (1U<<6)
#تعریف_کرو OUTPUT_NO_AUTHOR            (1U<<7)
#تعریف_کرو OUTPUT_SHOW_EMAIL           (1U<<8)
#تعریف_کرو OUTPUT_سطر_PORCELAIN       (1U<<9)
#تعریف_کرو OUTPUT_COLOR_سطر           (1U<<10)
#تعریف_کرو OUTPUT_SHOW_AGE_WITH_COLOR  (1U<<11)

ساکن خالی emit_porcelain_details(ساخت blame_origin *suspect, صحیح repeat)
{
	اگر (emit_one_suspect_detail(suspect, repeat) ||
	    (suspect->commit->object.flags & مزید_THAN_ONE_راستہ))
		write_filename_info(suspect);
}

/*
 * Information which needs to be printed per-line goes here. Any
 * information which can be clubbed on a commit/file level, should
 * be printed via 'emit_one_suspect_detail()'.
 */
ساکن خالی emit_porcelain_فی_سطر_details(ساخت blame_اندراج *ent)
{
	اگر (mark_unblamable_سطریں && ent->unblamable)
		puts("unblamable");
	اگر (mark_ignored_سطریں && ent->ignored)
		puts("ignored");
}

ساکن خالی emit_porcelain(ساخت blame_scoreboard *sb, ساخت blame_اندراج *ent,
			   صحیح opt)
{
	صحیح repeat = opt & OUTPUT_سطر_PORCELAIN;
	صحیح cnt;
	ثابت حرف *cp;
	ساخت blame_origin *suspect = ent->suspect;
	حرف hex[GIT_MAX_HEXSZ + 1];

	oid_مقصد_hex_r(hex, &suspect->commit->object.oid);
	لکھو("%s %d %d %d\n",
	       hex,
	       ent->s_lno + 1,
	       ent->lno + 1,
	       ent->عدد_سطریں);
	emit_porcelain_details(suspect, repeat);
	emit_porcelain_فی_سطر_details(ent);

	cp = blame_nth_سطر(sb, ent->lno);
	برائے (cnt = 0; cnt < ent->عدد_سطریں; cnt++) {
		حرف ch;
		اگر (cnt) {
			لکھو("%s %d %d\n", hex,
			       ent->s_lno + 1 + cnt,
			       ent->lno + 1 + cnt);
			اگر (repeat)
				emit_porcelain_details(suspect, 1);
			emit_porcelain_فی_سطر_details(ent);
		}
		حرف_لکھو('\t');
		کرو {
			ch = *cp++;
			حرف_لکھو(ch);
		} جب (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_سائز);
	}

	اگر (sb->final_buf_سائز && cp[-1] != '\n')
		حرف_لکھو('\n');
}

ساکن ساخت color_میدان {
	timestamp_t hop;
	حرف ستون[COLOR_MAXLEN];
} *colorfield;
ساکن صحیح colorfield_nr, colorfield_alloc;

ساکن خالی parse_color_fields(ثابت حرف *s)
{
	ساخت string_list l = STRING_LIST_INIT_DUP;
	ساخت string_list_شے *شے;
	شمار { EXPECT_DATE, EXPECT_COLOR } اگلا = EXPECT_COLOR;

	colorfield_nr = 0;

	/* Ideally this would be stripped and split at the same time? */
	string_list_بانٹو(&l, s, ",", -1);
	ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);

	for_each_string_list_شے(شے, &l) {
		انتخاب (اگلا) {
		حالت EXPECT_DATE:
			colorfield[colorfield_nr].hop = approxidate(شے->string);
			اگلا = EXPECT_COLOR;
			colorfield_nr++;
			ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);
			توڑو;
		حالت EXPECT_COLOR:
			اگر (color_parse(شے->string, colorfield[colorfield_nr].ستون))
				die(_("expecting a color: %s"), شے->string);
			اگلا = EXPECT_DATE;
			توڑو;
		}
	}

	اگر (اگلا == EXPECT_COLOR)
		die(_("must end with a color"));

	colorfield[colorfield_nr].hop = وقت_MAX;
	string_list_clear(&l, 0);
}

ساکن خالی تیاری_default_color_by_age(خالی)
{
	parse_color_fields("blue,12 month ago,white,1 month ago,red");
}

ساکن خالی determine_سطر_heat(ساخت commit_info *ci, ثابت حرف **dest_color)
{
	صحیح i = 0;

	جب (i < colorfield_nr && ci->author_وقت > colorfield[i].hop)
		i++;

	*dest_color = colorfield[i].ستون;
}

ساکن اندرونی صحیح maybe_putc(صحیح c, FILE *برآمد)
{
	واپس برآمد ? putc(c, برآمد) : 0;
}

ساکن سائز_t print_marks(FILE *برآمد, ثابت ساخت blame_اندراج *ent, صحیح opt)
{
	سائز_t len = 0;

	اگر ((ent->suspect->commit->object.flags & UNINTERESTING) &&
	    !blank_boundary && !(opt & OUTPUT_ANNOTATE_COMPAT)) {
		maybe_putc('^', برآمد);
		len++;
	}
	اگر (mark_unblamable_سطریں && ent->unblamable) {
		maybe_putc('*', برآمد);
		len++;
	}
	اگر (mark_ignored_سطریں && ent->ignored) {
		maybe_putc('?', برآمد);
		len++;
	}
	واپس len;
}

ساکن سائز_t گنتی_marks(ثابت ساخت blame_اندراج *ent, صحیح opt)
{
	واپس print_marks(تہی, ent, opt);
}

ساکن خالی emit_other(ساخت blame_scoreboard *sb, ساخت blame_اندراج *ent,
		       صحیح opt, ساخت blame_اندراج *پچھلا_ent)
{
	صحیح cnt;
	ثابت حرف *cp;
	ساخت blame_origin *suspect = ent->suspect;
	ساخت commit_info ci = COMMIT_INFO_INIT;
	حرف hex[GIT_MAX_HEXSZ + 1];
	صحیح show_raw_وقت = !!(opt & OUTPUT_RAW_TIMESTAMP);
	ثابت حرف *default_color = تہی, *color = تہی, *reset = تہی;

	get_commit_info(suspect->commit, &ci);
	oid_مقصد_hex_r(hex, &suspect->commit->object.oid);

	cp = blame_nth_سطر(sb, ent->lno);

	اگر (opt & OUTPUT_SHOW_AGE_WITH_COLOR) {
		determine_سطر_heat(&ci, &default_color);
		color = default_color;
		reset = GIT_COLOR_RESET;
	}

	اگر (abbrev < MINIMUM_ABBREV)
		BUG("abbreviation is smaller than minimum length: %d < %d",
		    abbrev, MINIMUM_ABBREV);

	برائے (cnt = 0; cnt < ent->عدد_سطریں; cnt++) {
		حرف ch;
		سائز_t لمبائی = (opt & OUTPUT_LONG_OBJECT_NAME) ?
			the_hash_algo->hexsz : (سائز_t) abbrev;

		اگر (opt & OUTPUT_COLOR_سطر) {
			اگر (cnt > 0 ||
			    (پچھلا_ent &&
			     oideq(&suspect->commit->object.oid,
				   &پچھلا_ent->suspect->commit->object.oid))) {
				color = repeated_meta_color;
				reset = GIT_COLOR_RESET;
			} ورنہ  {
				color = default_color ? default_color : تہی;
				reset = default_color ? GIT_COLOR_RESET : تہی;
			}
		}
		اگر (color)
			fputs(color, stdout);

		اگر ((suspect->commit->object.flags & UNINTERESTING) &&
		    blank_boundary)
			memset(hex, ' ', لڑی_لمبائی(hex));
		لمبائی -= print_marks(stdout, ent, opt);

		لکھو("%.*s", (صحیح)(لمبائی < GIT_MAX_HEXSZ ? لمبائی : GIT_MAX_HEXSZ), hex);
		اگر (opt & OUTPUT_ANNOTATE_COMPAT) {
			ثابت حرف *name;
			اگر (opt & OUTPUT_SHOW_EMAIL)
				name = ci.author_mail.buf;
			ورنہ
				name = ci.author.buf;
			لکھو("\t(%10s\t%10s\t%d)", name,
			       format_وقت(ci.author_وقت, ci.author_tz.buf,
					   show_raw_وقت),
			       ent->lno + 1 + cnt);
		} ورنہ {
			اگر (opt & OUTPUT_SHOW_SCORE)
				لکھو(" %*d %02d",
				       max_score_digits, ent->score,
				       ent->suspect->refcnt);
			اگر (opt & OUTPUT_SHOW_NAME)
				لکھو(" %-*.*s", longest_file, longest_file,
				       suspect->راستہ);
			اگر (opt & OUTPUT_SHOW_نمبر)
				لکھو(" %*d", max_orig_digits,
				       ent->s_lno + 1 + cnt);

			اگر (!(opt & OUTPUT_NO_AUTHOR)) {
				ثابت حرف *name;
				صحیح pad;
				اگر (opt & OUTPUT_SHOW_EMAIL)
					name = ci.author_mail.buf;
				ورنہ
					name = ci.author.buf;
				pad = longest_author - utf8_strwidth(name);
				لکھو(" (%s%*s %10s",
				       name, pad, "",
				       format_وقت(ci.author_وقت,
						   ci.author_tz.buf,
						   show_raw_وقت));
			}
			لکھو(" %*d) ",
			       max_digits, ent->lno + 1 + cnt);
		}
		اگر (reset)
			fputs(reset, stdout);
		کرو {
			ch = *cp++;
			حرف_لکھو(ch);
		} جب (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_سائز);
	}

	اگر (sb->final_buf_سائز && cp[-1] != '\n')
		حرف_لکھو('\n');

	commit_info_destroy(&ci);
}

ساکن خالی output(ساخت blame_scoreboard *sb, صحیح option)
{
	ساخت blame_اندراج *ent, *پچھلا_ent = تہی;

	اگر (option & OUTPUT_PORCELAIN) {
		برائے (ent = sb->ent; ent; ent = ent->اگلا) {
			صحیح گنتی = 0;
			ساخت blame_origin *suspect;
			ساخت commit *commit = ent->suspect->commit;
			اگر (commit->object.flags & مزید_THAN_ONE_راستہ)
				جاری;
			برائے (suspect = get_blame_suspects(commit); suspect; suspect = suspect->اگلا) {
				اگر (suspect->guilty && گنتی++) {
					commit->object.flags |= مزید_THAN_ONE_راستہ;
					توڑو;
				}
			}
		}
	}

	برائے (ent = sb->ent; ent; ent = ent->اگلا) {
		اگر (option & OUTPUT_PORCELAIN)
			emit_porcelain(sb, ent, option);
		ورنہ {
			emit_other(sb, ent, option, پچھلا_ent);
			پچھلا_ent = ent;
		}
	}
}

/*
 * Add phony grafts for use with -S; this is primarily to
 * support git's cvsserver that wants to give a linear history
 * to its clients.
 */
ساکن صحیح read_ancestry(ثابت حرف *graft_file)
{
	FILE *fp = fopen_or_warn(graft_file, "r");
	ساخت strbuf buf = STRBUF_INIT;
	اگر (!fp)
		واپس -1;
	جب (!strbuf_getwholeline(&buf, fp, '\n')) {
		/* The format is just "Commit Parent1 Parent2 ...\n" */
		ساخت commit_graft *graft = read_graft_سطر(&buf);
		اگر (graft)
			رجسٹر_commit_graft(the_repository, graft, 0);
	}
	fclose(fp);
	strbuf_release(&buf);
	واپس 0;
}

ساکن صحیح update_auto_abbrev(صحیح auto_abbrev, ساخت blame_origin *suspect)
{
	ثابت حرف *uniq = repo_ڈھونڈو_unique_abbrev(the_repository,
						   &suspect->commit->object.oid,
						   auto_abbrev);
	صحیح len = لڑی_لمبائی(uniq);
	اگر (auto_abbrev < len)
		واپس len;
	واپس auto_abbrev;
}

/*
 * How many columns do we need to show line numbers, authors,
 * and filenames?
 */
ساکن خالی ڈھونڈو_alignment(ساخت blame_scoreboard *sb, صحیح *option)
{
	صحیح longest_src_سطریں = 0;
	صحیح longest_dst_سطریں = 0;
	غیرعلامتی سب_سے_بڑا_score = 0;
	ساخت blame_اندراج *e;
	صحیح compute_auto_abbrev = (abbrev < 0);
	صحیح auto_abbrev = DEFAULT_ABBREV;
	سائز_t max_marks_گنتی = 0;

	برائے (e = sb->ent; e; e = e->اگلا) {
		ساخت blame_origin *suspect = e->suspect;
		صحیح عدد;
		سائز_t marks_گنتی = گنتی_marks(e, *option);

		اگر (max_marks_گنتی < marks_گنتی)
			max_marks_گنتی = marks_گنتی;
		اگر (compute_auto_abbrev)
			auto_abbrev = update_auto_abbrev(auto_abbrev, suspect);
		اگر (strcmp(suspect->راستہ, sb->راستہ))
			*option |= OUTPUT_SHOW_NAME;
		عدد = لڑی_لمبائی(suspect->راستہ);
		اگر (longest_file < عدد)
			longest_file = عدد;
		اگر (!(suspect->commit->object.flags & METAINFO_SHOWN)) {
			ساخت commit_info ci = COMMIT_INFO_INIT;
			suspect->commit->object.flags |= METAINFO_SHOWN;
			get_commit_info(suspect->commit, &ci);
			اگر (*option & OUTPUT_SHOW_EMAIL)
				عدد = utf8_strwidth(ci.author_mail.buf);
			ورنہ
				عدد = utf8_strwidth(ci.author.buf);
			اگر (longest_author < عدد)
				longest_author = عدد;
			commit_info_destroy(&ci);
		}
		عدد = e->s_lno + e->عدد_سطریں;
		اگر (longest_src_سطریں < عدد)
			longest_src_سطریں = عدد;
		عدد = e->lno + e->عدد_سطریں;
		اگر (longest_dst_سطریں < عدد)
			longest_dst_سطریں = عدد;
		اگر (سب_سے_بڑا_score < blame_اندراج_score(sb, e))
			سب_سے_بڑا_score = blame_اندراج_score(sb, e);
	}
	max_orig_digits = decimal_چوڑائی(longest_src_سطریں);
	max_digits = decimal_چوڑائی(longest_dst_سطریں);
	max_score_digits = decimal_چوڑائی(سب_سے_بڑا_score);

	اگر (compute_auto_abbrev)
		abbrev = auto_abbrev;
	اگر (abbrev < (صحیح)the_hash_algo->hexsz) {
		abbrev += max_marks_گنتی;
		اگر (abbrev > (صحیح)the_hash_algo->hexsz)
			abbrev = the_hash_algo->hexsz;
	}
}

ساکن خالی sanity_check_on_fail(ساخت blame_scoreboard *sb, صحیح baa)
{
	صحیح opt = OUTPUT_SHOW_SCORE | OUTPUT_SHOW_نمبر | OUTPUT_SHOW_NAME;
	ڈھونڈو_alignment(sb, &opt);
	output(sb, opt);
	die("Baa %d!", baa);
}

ساکن غیرعلامتی parse_score(ثابت حرف *arg)
{
	حرف *انتہا;
	غیرعلامتی طویل score = strtoul(arg, &انتہا, 10);
	اگر (*انتہا)
		واپس 0;
	واپس score;
}

ساکن حرف *جوڑو_سابقہ(ثابت حرف *سابقہ, ثابت حرف *راستہ)
{
	واپس سابقہ_راستہ(the_repository, سابقہ, سابقہ ? لڑی_لمبائی(سابقہ) : 0, راستہ);
}

ساکن صحیح git_blame_config(ثابت حرف *var, ثابت حرف *قدر,
			    ثابت ساخت config_context *ctx, خالی *cb)
{
	اگر (!strcmp(var, "blame.showroot")) {
		show_جڑ = git_config_bool(var, قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "blame.blankboundary")) {
		blank_boundary = git_config_bool(var, قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "blame.showemail")) {
		صحیح *output_option = cb;
		اگر (git_config_bool(var, قدر))
			*output_option |= OUTPUT_SHOW_EMAIL;
		ورنہ
			*output_option &= ~OUTPUT_SHOW_EMAIL;
		واپس 0;
	}
	اگر (!strcmp(var, "blame.date")) {
		اگر (!قدر)
			واپس config_error_nonbool(var);
		parse_date_format(قدر, &blame_date_منوال);
		واپس 0;
	}
	اگر (!strcmp(var, "blame.ignorerevsfile")) {
		حرف *str;
		صحیح ret;

		ret = git_config_pathname(&str, var, قدر);
		اگر (ret)
			واپس ret;
		اگر (str)
			string_list_داخل_کرو(&ignore_revs_file_list, str);
		آزاد(str);
		واپس 0;
	}
	اگر (!strcmp(var, "blame.markunblamablelines")) {
		mark_unblamable_سطریں = git_config_bool(var, قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "blame.markignoredlines")) {
		mark_ignored_سطریں = git_config_bool(var, قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "color.blame.repeatedlines")) {
		اگر (color_parse_mem(قدر, لڑی_لمبائی(قدر), repeated_meta_color))
			warning(_("invalid value for '%s': '%s'"),
				"color.blame.repeatedLines", قدر);
		واپس 0;
	}
	اگر (!strcmp(var, "color.blame.highlightrecent")) {
		parse_color_fields(قدر);
		واپس 0;
	}

	اگر (!strcmp(var, "blame.coloring")) {
		اگر (!قدر)
			واپس config_error_nonbool(var);
		اگر (!strcmp(قدر, "repeatedLines")) {
			coloring_منوال |= OUTPUT_COLOR_سطر;
		} ورنہ اگر (!strcmp(قدر, "highlightRecent")) {
			coloring_منوال |= OUTPUT_SHOW_AGE_WITH_COLOR;
		} ورنہ اگر (!strcmp(قدر, "none")) {
			coloring_منوال &= ~(OUTPUT_COLOR_سطر |
					    OUTPUT_SHOW_AGE_WITH_COLOR);
		} ورنہ {
			warning(_("invalid value for '%s': '%s'"),
				"blame.coloring", قدر);
			واپس 0;
		}
	}

	اگر (!strcmp(var, "diff.algorithm")) {
		طویل diff_algorithm;
		اگر (!قدر)
			واپس config_error_nonbool(var);
		diff_algorithm = parse_algorithm_قدر(قدر);
		اگر (diff_algorithm < 0)
			واپس error(_("unknown value for config '%s': %s"),
				     var, قدر);
		xdl_opts &= ~XDF_DIFF_ALGORITHM_نقاب;
		xdl_opts |= diff_algorithm;
		واپس 0;
	}

	اگر (git_diff_heuristic_config(var, قدر, cb) < 0)
		واپس -1;
	اگر (userdiff_config(var, قدر) < 0)
		واپس -1;

	واپس git_default_config(var, قدر, ctx, cb);
}

ساکن صحیح blame_copy_callback(ثابت ساخت option *option, ثابت حرف *arg, صحیح unset)
{
	صحیح *opt = option->قدر;

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
	واپس 0;
}

ساکن صحیح blame_move_callback(ثابت ساخت option *option, ثابت حرف *arg, صحیح unset)
{
	صحیح *opt = option->قدر;

	BUG_ON_OPT_NEG(unset);

	*opt |= PICKAXE_BLAME_MOVE;

	اگر (arg)
		blame_move_score = parse_score(arg);
	واپس 0;
}

ساکن صحیح blame_diff_algorithm_اقل(ثابت ساخت option *option,
					ثابت حرف *arg, صحیح unset)
{
	صحیح *opt = option->قدر;

	BUG_ON_OPT_ARG(arg);

	*opt &= ~XDF_DIFF_ALGORITHM_نقاب;
	اگر (!unset)
		*opt |= XDF_NEED_اقل;

	واپس 0;
}

ساکن صحیح blame_diff_algorithm_callback(ثابت ساخت option *option,
					 ثابت حرف *arg, صحیح unset)
{
	صحیح *opt = option->قدر;
	طویل قدر = parse_algorithm_قدر(arg);

	BUG_ON_OPT_NEG(unset);

	اگر (قدر < 0)
		واپس error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	*opt &= ~XDF_DIFF_ALGORITHM_نقاب;
	*opt |= قدر;

	واپس 0;
}

ساکن صحیح is_a_rev(ثابت حرف *name)
{
	ساخت object_id oid;

	اگر (repo_get_oid(the_repository, name, &oid))
		واپس 0;
	واپس OBJ_NONE < odb_read_object_info(the_repository->objects, &oid, تہی);
}

ساکن صحیح peel_مقصد_commit_oid(ساخت object_id *oid_ret, خالی *cbdata)
{
	ساخت repository *r = ((ساخت blame_scoreboard *)cbdata)->repo;
	ساخت object_id oid;

	oidcpy(&oid, oid_ret);
	جب (1) {
		ساخت object *obj;
		صحیح قسم = odb_read_object_info(r->objects, &oid, تہی);
		اگر (قسم == OBJ_COMMIT) {
			oidcpy(oid_ret, &oid);
			واپس 0;
		}
		اگر (قسم != OBJ_لیبل)
			واپس -1;
		obj = deref_لیبل(r, parse_object(r, &oid), تہی, 0);
		اگر (!obj)
			واپس -1;
		oidcpy(&oid, &obj->oid);
	}
}

ساکن خالی build_ignorelist(ساخت blame_scoreboard *sb,
			     ساخت string_list *ignore_revs_file_list,
			     ساخت string_list *ignore_rev_list)
{
	ساخت string_list_شے *i;
	ساخت object_id oid;

	oidset_init(&sb->ignore_list, 0);
	for_each_string_list_شے(i, ignore_revs_file_list) {
		اگر (!strcmp(i->string, ""))
			oidset_clear(&sb->ignore_list);
		ورنہ
			oidset_parse_file_carefully(&sb->ignore_list, i->string,
						    the_repository->hash_algo,
						    peel_مقصد_commit_oid, sb);
	}
	for_each_string_list_شے(i, ignore_rev_list) {
		اگر (repo_get_oid_committish(the_repository, i->string, &oid) ||
		    peel_مقصد_commit_oid(&oid, sb))
			die(_("cannot find revision %s to ignore"), i->string);
		oidset_داخل_کرو(&sb->ignore_list, &oid);
	}
}

صحیح cmd_blame(صحیح argc,
	      ثابت حرف **argv,
	      ثابت حرف *سابقہ,
	      ساخت repository *repo غیر_مستعمل)
{
	ساخت rev_info revs;
	حرف *راستہ = تہی;
	ساخت blame_scoreboard sb;
	ساخت blame_origin *o;
	ساخت blame_اندراج *ent = تہی;
	طویل dashdash_pos, lno;
	ساخت پیش_رفت_info pi = { تہی, 0 };

	ساخت string_list range_list = STRING_LIST_INIT_NODUP;
	ساخت string_list ignore_rev_list = STRING_LIST_INIT_NODUP;
	صحیح output_option = 0, opt = 0;
	صحیح show_stats = 0;
	ثابت حرف *revs_file = تہی;
	ثابت حرف *contents_from = تہی;
	ثابت ساخت option options[] = {
		OPT_BOOL(0, "incremental", &incremental, N_("show blame entries as we find them, incrementally")),
		OPT_BOOL('b', تہی, &blank_boundary, N_("do not show object names of boundary commits (Default: off)")),
		OPT_BOOL(0, "root", &show_جڑ, N_("do not treat root commits as boundaries (Default: off)")),
		OPT_BOOL(0, "show-stats", &show_stats, N_("show work cost statistics")),
		OPT_BOOL(0, "progress", &show_پیش_رفت, N_("force progress reporting")),
		OPT_ثنائی_ہندسہ(0, "score-debug", &output_option, N_("show output score for blame entries"), OUTPUT_SHOW_SCORE),
		OPT_ثنائی_ہندسہ('f', "show-name", &output_option, N_("show original filename (Default: auto)"), OUTPUT_SHOW_NAME),
		OPT_ثنائی_ہندسہ('n', "show-number", &output_option, N_("show original linenumber (Default: off)"), OUTPUT_SHOW_نمبر),
		OPT_ثنائی_ہندسہ('p', "porcelain", &output_option, N_("show in a format designed for machine consumption"), OUTPUT_PORCELAIN),
		OPT_ثنائی_ہندسہ(0, "line-porcelain", &output_option, N_("show porcelain format with per-line commit information"), OUTPUT_PORCELAIN|OUTPUT_سطر_PORCELAIN),
		OPT_ثنائی_ہندسہ('c', تہی, &output_option, N_("use the same output mode as git-annotate (Default: off)"), OUTPUT_ANNOTATE_COMPAT),
		OPT_ثنائی_ہندسہ('t', تہی, &output_option, N_("show raw timestamp (Default: off)"), OUTPUT_RAW_TIMESTAMP),
		OPT_ثنائی_ہندسہ('l', تہی, &output_option, N_("show long commit SHA1 (Default: off)"), OUTPUT_LONG_OBJECT_NAME),
		OPT_ثنائی_ہندسہ('s', تہی, &output_option, N_("suppress author name and timestamp (Default: off)"), OUTPUT_NO_AUTHOR),
		OPT_ثنائی_ہندسہ('e', "show-email", &output_option, N_("show author email instead of name (Default: off)"), OUTPUT_SHOW_EMAIL),
		OPT_ثنائی_ہندسہ('w', تہی, &xdl_opts, N_("ignore whitespace differences"), XDF_IGNORE_WHITESPACE),
		OPT_CALLBACK_F(0, "diff-algorithm", &xdl_opts, N_("<algorithm>"),
			       N_("choose a diff algorithm"),
			       PARSE_OPT_NONEG, blame_diff_algorithm_callback),
		OPT_STRING_LIST(0, "ignore-rev", &ignore_rev_list, N_("rev"), N_("ignore <rev> when blaming")),
		OPT_STRING_LIST(0, "ignore-revs-file", &ignore_revs_file_list, N_("file"), N_("ignore revisions from <file>")),
		OPT_ثنائی_ہندسہ(0, "color-lines", &output_option, N_("color redundant metadata from previous line differently"), OUTPUT_COLOR_سطر),
		OPT_ثنائی_ہندسہ(0, "color-by-age", &output_option, N_("color lines by age"), OUTPUT_SHOW_AGE_WITH_COLOR),
		OPT_CALLBACK_F(0, "minimal", &xdl_opts, تہی,
			       N_("spend extra cycles to find a better match"),
			       PARSE_OPT_NOARG | PARSE_OPT_HIDDEN, blame_diff_algorithm_اقل),
		OPT_STRING('S', تہی, &revs_file, N_("file"), N_("use revisions from <file> instead of calling git-rev-list")),
		OPT_STRING(0, "contents", &contents_from, N_("file"), N_("use <file>'s contents as the final image")),
		OPT_CALLBACK_F('C', تہی, &opt, N_("score"), N_("find line copies within and across files"), PARSE_OPT_OPTARG, blame_copy_callback),
		OPT_CALLBACK_F('M', تہی, &opt, N_("score"), N_("find line movements within and across files"), PARSE_OPT_OPTARG, blame_move_callback),
		OPT_STRING_LIST('L', تہی, &range_list, N_("range"),
				N_("process only line range <start>,<end> or function :<funcname>")),
		OPT__ABBREV(&abbrev),
		OPT_انتہا()
	};

	ساخت parse_opt_ctx_t ctx;
	صحیح cmd_is_annotate = !strcmp(argv[0], "annotate");
	ساخت range_سیٹ ranges;
	غیرعلامتی صحیح range_i;
	طویل anchor;
	طویل عدد_سطریں = 0;
	ثابت حرف *str_usage = cmd_is_annotate ? annotate_usage : blame_usage;
	ثابت حرف *ثابت *opt_usage = cmd_is_annotate ? annotate_opt_usage : blame_opt_usage;

	تیاری_default_color_by_age();
	repo_config(the_repository, git_blame_config, &output_option);
	repo_init_revisions(the_repository, &revs, تہی);
	revs.date_منوال = blame_date_منوال;
	revs.diffopt.flags.allow_textconv = 1;
	revs.diffopt.flags.follow_renames = 1;

	save_commit_بفر = 0;
	dashdash_pos = 0;
	show_پیش_رفت = -1;

	parse_options_آغاز(&ctx, argc, argv, سابقہ, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);
	برائے (;;) {
		انتخاب (parse_options_قدم(&ctx, options, opt_usage)) {
		حالت PARSE_OPT_NON_OPTION:
		حالت PARSE_OPT_UNKNOWN:
			توڑو;
		حالت PARSE_OPT_HELP:
			باہر_نکلو(0);
		حالت PARSE_OPT_HELP_ERROR:
		حالت PARSE_OPT_ERROR:
		حالت PARSE_OPT_SUBCOMMAND:
			باہر_نکلو(129);
		حالت PARSE_OPT_COMPLETE:
			باہر_نکلو(0);
		حالت PARSE_OPT_مکمل:
			اگر (ctx.argv[0])
				dashdash_pos = ctx.cpidx;
			جاؤ parse_مکمل;
		}

		اگر (!strcmp(ctx.argv[0], "--reverse")) {
			ctx.argv[0] = "--children";
			الٹ = 1;
		}
		parse_revision_opt(&revs, &ctx, options, opt_usage);
	}
parse_مکمل:
	revision_opts_اختتام(&revs);
	no_پورا_file_rename = !revs.diffopt.flags.follow_renames;
	xdl_opts |= revs.diffopt.xdl_opts & XDF_INDENT_HEURISTIC;
	revs.diffopt.flags.follow_renames = 0;
	argc = parse_options_انتہا(&ctx);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_بھرا_اشاریہ = 0;

	اگر (incremental || (output_option & OUTPUT_PORCELAIN)) {
		اگر (show_پیش_رفت > 0)
			die(_("--progress can't be used with --incremental or porcelain formats"));
		show_پیش_رفت = 0;
	} ورنہ اگر (show_پیش_رفت < 0)
		show_پیش_رفت = isatty(2);

	اگر (!abbrev)
		abbrev = the_hash_algo->hexsz;

	اگر (revs_file && read_ancestry(revs_file))
		die_errno("reading graft file '%s' failed", revs_file);

	اگر (cmd_is_annotate) {
		output_option |= OUTPUT_ANNOTATE_COMPAT;
		blame_date_منوال.type = DATE_ISO8601;
	} ورنہ {
		blame_date_منوال = revs.date_منوال;
	}

	/* The maximum width used to show the dates */
	انتخاب (blame_date_منوال.type) {
	حالت DATE_RFC2822:
		blame_date_چوڑائی = حجم("Thu, 19 Oct 2006 16:00:04 -0700");
		توڑو;
	حالت DATE_ISO8601_STRICT:
		blame_date_چوڑائی = حجم("2006-10-19T16:00:04-07:00");
		توڑو;
	حالت DATE_ISO8601:
		blame_date_چوڑائی = حجم("2006-10-19 16:00:04 -0700");
		توڑو;
	حالت DATE_RAW:
		blame_date_چوڑائی = حجم("1161298804 -0700");
		توڑو;
	حالت DATE_UNIX:
		blame_date_چوڑائی = حجم("1161298804");
		توڑو;
	حالت DATE_SHORT:
		blame_date_چوڑائی = حجم("2006-10-19");
		توڑو;
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
		blame_date_چوڑائی = utf8_strwidth(_("4 years, 11 months ago")) + 1; /* add the null */
		توڑو;
	حالت DATE_HUMAN:
		/* If the year is shown, no time is shown */
		blame_date_چوڑائی = حجم("Thu Oct 19 16:00");
		توڑو;
	حالت DATE_معمول:
		blame_date_چوڑائی = حجم("Thu Oct 19 16:00:04 2006 -0700");
		توڑو;
	حالت DATE_STRFTIME:
		blame_date_چوڑائی = لڑی_لمبائی(show_date(0, 0, blame_date_منوال)) + 1; /* add the null */
		توڑو;
	}
	blame_date_چوڑائی -= 1; /* strip the null */

	اگر (revs.diffopt.flags.ڈھونڈو_copies_harder)
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
			راستہ = جوڑو_سابقہ(سابقہ, argv[--argc]);
			argv[argc] = تہی;
			توڑو;
		طےشدہ:
			usage_with_options(opt_usage, options);
		}
	} ورنہ {
		اگر (argc < 2)
			usage_with_options(opt_usage, options);
		اگر (argc == 3 && is_a_rev(argv[argc - 1])) { /* (2b) */
			راستہ = جوڑو_سابقہ(سابقہ, argv[1]);
			argv[1] = argv[2];
		} ورنہ {	/* (2a) */
			اگر (argc == 2 && is_a_rev(argv[1]) && !repo_get_work_درخت(the_repository))
				die("missing <path> to blame");
			راستہ = جوڑو_سابقہ(سابقہ, argv[argc - 1]);
		}
		argv[argc - 1] = "--";
	}

	revs.disable_stdin = 1;
	تیاری_revisions(argc, argv, &revs, تہی);
	اگر (!revs.pending.nr && is_bare_repository(the_repository)) {
		ساخت commit *سر_commit;
		ساخت object_id سر_oid;

		اگر (!refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", RESOLVE_REF_READING,
					     &سر_oid, تہی) ||
		    !(سر_commit = lookup_commit_reference_gently(revs.repo,
							     &سر_oid, 1)))
			die("no such ref: HEAD");

		جوڑو_pending_object(&revs, &سر_commit->object, "HEAD");
	}

	init_scoreboard(&sb);
	sb.revs = &revs;
	sb.contents_from = contents_from;
	sb.الٹ = الٹ;
	sb.repo = the_repository;
	sb.راستہ = راستہ;
	build_ignorelist(&sb, &ignore_revs_file_list, &ignore_rev_list);
	string_list_clear(&ignore_revs_file_list, 0);
	string_list_clear(&ignore_rev_list, 0);
	تیاری_scoreboard(&sb, &o);

	/*
	 * Changed-path Bloom filters are disabled when looking
	 * for copies.
	 */
	اگر (!(opt & PICKAXE_BLAME_COPY))
		تیاری_blame_bloom_ڈیٹا(&sb);

	lno = sb.عدد_سطریں;

	اگر (lno && !range_list.nr)
		string_list_append(&range_list, "1");

	anchor = 1;
	range_سیٹ_init(&ranges, range_list.nr);
	برائے (range_i = 0; range_i < range_list.nr; ++range_i) {
		طویل bottom, چوٹی;
		اگر (parse_range_arg(range_list.اشیاء[range_i].string,
				    nth_سطر_cb, &sb, lno, anchor,
				    &bottom, &چوٹی, sb.راستہ,
				    the_repository->اشاریہ))
			usage(str_usage);
		اگر ((!lno && (چوٹی || bottom)) || lno < bottom)
			die(Q_("file %s has only %lu line",
			       "file %s has only %lu lines",
			       lno), sb.راستہ, lno);
		اگر (bottom < 1)
			bottom = 1;
		اگر (چوٹی < 1 || lno < چوٹی)
			چوٹی = lno;
		bottom--;
		range_سیٹ_append_unsafe(&ranges, bottom, چوٹی);
		anchor = چوٹی + 1;
	}
	ترتیب_دو_and_ملاؤ_range_سیٹ(&ranges);

	برائے (range_i = ranges.nr; range_i > 0; --range_i) {
		ثابت ساخت range *r = &ranges.ranges[range_i - 1];
		ent = blame_اندراج_prepend(ent, r->آغاز, r->انتہا, o);
		عدد_سطریں += (r->انتہا - r->آغاز);
	}
	اگر (!عدد_سطریں)
		عدد_سطریں = sb.عدد_سطریں;

	o->suspects = ent;
	prio_قطار_put(&sb.commits, o->commit);

	blame_origin_decref(o);

	range_سیٹ_release(&ranges);
	string_list_clear(&range_list, 0);

	sb.ent = تہی;

	اگر (blame_move_score)
		sb.move_score = blame_move_score;
	اگر (blame_copy_score)
		sb.copy_score = blame_copy_score;

	sb.debug = DEBUG_BLAME;
	sb.on_sanity_fail = &sanity_check_on_fail;

	sb.show_جڑ = show_جڑ;
	sb.xdl_opts = xdl_opts;
	sb.no_پورا_file_rename = no_پورا_file_rename;

	read_mailmap(the_repository, &mailmap);

	sb.ملا_guilty_اندراج = &ملا_guilty_اندراج;
	sb.ملا_guilty_اندراج_ڈیٹا = &pi;
	اگر (show_پیش_رفت)
		pi.پیش_رفت = آغاز_delayed_پیش_رفت(the_repository,
						     _("Blaming lines"),
						     عدد_سطریں);

	تفویض_blame(&sb, opt);

	stop_پیش_رفت(&pi.پیش_رفت);

	اگر (!incremental)
		تیاری_pager(the_repository);
	ورنہ
		جاؤ cleanup;

	blame_ترتیب_دو_final(&sb);

	blame_coalesce(&sb);

	اگر (!(output_option & (OUTPUT_COLOR_سطر | OUTPUT_SHOW_AGE_WITH_COLOR)))
		output_option |= coloring_منوال;

	اگر (!(output_option & OUTPUT_PORCELAIN)) {
		ڈھونڈو_alignment(&sb, &output_option);
		اگر (!*repeated_meta_color &&
		    (output_option & OUTPUT_COLOR_سطر))
			xsnprintf(repeated_meta_color,
				  حجم(repeated_meta_color),
				  "%s", GIT_COLOR_CYAN);
	}
	اگر (output_option & OUTPUT_ANNOTATE_COMPAT)
		output_option &= ~(OUTPUT_COLOR_سطر | OUTPUT_SHOW_AGE_WITH_COLOR);

	output(&sb, output_option);

	اگر (show_stats) {
		لکھو("num read blob: %d\n", sb.عدد_read_blob);
		لکھو("num get patch: %d\n", sb.عدد_get_patch);
		لکھو("num commits: %d\n", sb.عدد_commits);
	}

cleanup:
	برائے (ent = sb.ent; ent; ) {
		ساخت blame_اندراج *e = ent->اگلا;
		آزاد(ent);
		ent = e;
	}

	آزاد(راستہ);
	cleanup_scoreboard(&sb);
	release_revisions(&revs);
	واپس 0;
}
