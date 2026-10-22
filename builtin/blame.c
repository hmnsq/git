/*
 * Blame
 *
 * Copyright (c) 2006, 2014 by its authors
 * See COPYING for licensing conditions
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

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
ساكن صحيح show_جڑ;
ساكن صحيح الٹ;
ساكن صحيح blank_boundary;
ساكن صحيح incremental;
ساكن صحيح xdl_opts;
ساكن صحيح abbrev = -1;
ساكن صحيح no_سارا_file_rename;
ساكن صحيح show_ترقی;
ساكن حرف repeated_meta_color[COLOR_MAXLEN];
ساكن صحيح coloring_منوال;
ساكن بنية string_list ignore_revs_file_list = STRING_LIST_INIT_DUP;
ساكن صحيح mark_unblamable_سطریں;
ساكن صحيح mark_ignored_سطریں;

ساكن بنية date_منوال blame_date_منوال = { DATE_ISO8601 };
ساكن ناپ_t blame_date_چوڑائی;

ساكن بنية string_list mailmap = STRING_LIST_INIT_NODUP;

#جے_بنا_تعریف DEBUG_BLAME
#تعریف_کرو DEBUG_BLAME 0
#جے_انت

ساكن بنانشانی blame_move_score;
ساكن بنانشانی blame_copy_score;

/* Remember to update object flag allocation in object.h */
#تعریف_کرو METAINFO_SHOWN		(1u<<12)
#تعریف_کرو ہور_THAN_ONE_رستہ	(1u<<13)

بنية ترقی_info {
	بنية ترقی *ترقی;
	صحيح blamed_سطریں;
};

ساكن ثابت حرف *nth_سطر_cb(فراغ *انکڑے, لما lno)
{
	أرجع blame_nth_سطر((بنية blame_scoreboard *)انکڑے, lno);
}

/*
 * Information on commits, used for output.
 */
بنية commit_info {
	بنية strbuf author;
	بنية strbuf author_mail;
	timestamp_t author_ویلا;
	بنية strbuf author_tz;

	/* filled only when asked for details */
	بنية strbuf committer;
	بنية strbuf committer_mail;
	timestamp_t committer_ویلا;
	بنية strbuf committer_tz;

	بنية strbuf summary;
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
ساكن فراغ get_ac_سطر(ثابت حرف *inbuf, ثابت حرف *what,
	بنية strbuf *name, بنية strbuf *mail,
	timestamp_t *ویلا, بنية strbuf *tz)
{
	بنية ident_ونڈو ident;
	ناپ_t len, maillen, namelen;
	ثابت حرف *tmp, *endp;
	ثابت حرف *namebuf, *mailbuf;

	tmp = لڑی_کھوج(inbuf, what);
	إذا (!tmp)
		اذهب error_باہرلا;
	tmp += لڑی_لمبائی(what);
	endp = strchr(tmp, '\n');
	إذا (!endp)
		len = لڑی_لمبائی(tmp);
	وإلا
		len = endp - tmp;

	إذا (ونڈو_ident_سطر(&ident, tmp, len)) {
	error_باہرلا:
		/* Ugh */
		tmp = "(unknown)";
		strbuf_addstr(name, tmp);
		strbuf_addstr(mail, tmp);
		strbuf_addstr(tz, tmp);
		*ویلا = 0;
		أرجع;
	}

	namelen = ident.name_سرا - ident.name_begin;
	namebuf = ident.name_begin;

	maillen = ident.mail_سرا - ident.mail_begin;
	mailbuf = ident.mail_begin;

	إذا (ident.date_begin && ident.date_سرا)
		*ویلا = strtoul(ident.date_begin, عدم, 10);
	وإلا
		*ویلا = 0;

	إذا (ident.tz_begin && ident.tz_سرا)
		strbuf_جوڑو(tz, ident.tz_begin, ident.tz_سرا - ident.tz_begin);
	وإلا
		strbuf_addstr(tz, "(unknown)");

	/*
	 * Now, convert both name and e-mail using mailmap
	 */
	map_user(&mailmap, &mailbuf, &maillen,
		 &namebuf, &namelen);

	strbuf_addf(mail, "<%.*s>", (صحيح)maillen, mailbuf);
	strbuf_جوڑو(name, namebuf, namelen);
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
	ثابت حرف *سنیہا;

	encoding = get_log_output_encoding();
	سنیہا = repo_logmsg_reencode(the_repository, commit, عدم, encoding);
	get_ac_سطر(سنیہا, "\nauthor ",
		    &ret->author, &ret->author_mail,
		    &ret->author_ویلا, &ret->author_tz);

	get_ac_سطر(سنیہا, "\ncommitter ",
		    &ret->committer, &ret->committer_mail,
		    &ret->committer_ویلا, &ret->committer_tz);

	len = لبھو_commit_subject(سنیہا, &subject);
	إذا (len)
		strbuf_جوڑو(&ret->summary, subject, len);
	وإلا
		strbuf_addf(&ret->summary, "(%s)", oid_منزل_hex(&commit->object.oid));

	repo_unuse_commit_بفر(the_repository, commit, سنیہا);
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
	إذا (suspect->پچھلا) {
		بنية blame_origin *prev = suspect->پچھلا;
		لکھو("previous %s ", oid_منزل_hex(&prev->commit->object.oid));
		write_name_quoted(prev->رستہ, stdout, '\n');
	}
	لکھو("filename ");
	write_name_quoted(suspect->رستہ, stdout, '\n');
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
	لکھو("author %s\n", ci.author.buf);
	لکھو("author-mail %s\n", ci.author_mail.buf);
	لکھو("author-time %"PRItime"\n", ci.author_ویلا);
	لکھو("author-tz %s\n", ci.author_tz.buf);
	لکھو("committer %s\n", ci.committer.buf);
	لکھو("committer-mail %s\n", ci.committer_mail.buf);
	لکھو("committer-time %"PRItime"\n", ci.committer_ویلا);
	لکھو("committer-tz %s\n", ci.committer_tz.buf);
	لکھو("summary %s\n", ci.summary.buf);
	إذا (suspect->commit->object.flags & UNINTERESTING)
		لکھو("boundary\n");

	commit_info_destroy(&ci);

	أرجع 1;
}

/*
 * The blame_entry is found to be guilty for the range.
 * Show it in incremental output.
 */
ساكن فراغ لبھیا_guilty_اندراج(بنية blame_اندراج *ent, فراغ *انکڑے)
{
	بنية ترقی_info *pi = (بنية ترقی_info *)انکڑے;

	إذا (incremental) {
		بنية blame_origin *suspect = ent->suspect;

		لکھو("%s %d %d %d\n",
		       oid_منزل_hex(&suspect->commit->object.oid),
		       ent->s_lno + 1, ent->lno + 1, ent->عدد_سطریں);
		emit_one_suspect_detail(suspect, 0);
		write_filename_info(suspect);
		maybe_flush_or_die(stdout, "stdout");
	}
	pi->blamed_سطریں += ent->عدد_سطریں;
	display_ترقی(pi->ترقی, pi->blamed_سطریں);
}

ساكن ثابت حرف *format_ویلا(timestamp_t ویلا, ثابت حرف *tz_str,
			       صحيح show_raw_ویلا)
{
	ساكن بنية strbuf ویلا_buf = STRBUF_INIT;

	strbuf_reset(&ویلا_buf);
	إذا (show_raw_ویلا) {
		strbuf_addf(&ویلا_buf, "%"PRItime" %s", ویلا, tz_str);
	}
	وإلا {
		ثابت حرف *ویلا_str;
		ناپ_t ویلا_چوڑائی;
		صحيح tz;
		tz = atoi(tz_str);
		ویلا_str = show_date(ویلا, tz, blame_date_منوال);
		strbuf_addstr(&ویلا_buf, ویلا_str);
		/*
		 * Add space paddings to time_buf to display a fixed width
		 * string, and use time_width for display width calibration.
		 */
		لكل (ویلا_چوڑائی = utf8_strwidth(ویلا_str);
		     ویلا_چوڑائی < blame_date_چوڑائی;
		     ویلا_چوڑائی++)
			strbuf_addch(&ویلا_buf, ' ');
	}
	أرجع ویلا_buf.buf;
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

ساكن فراغ emit_porcelain_details(بنية blame_origin *suspect, صحيح repeat)
{
	إذا (emit_one_suspect_detail(suspect, repeat) ||
	    (suspect->commit->object.flags & ہور_THAN_ONE_رستہ))
		write_filename_info(suspect);
}

/*
 * Information which needs to be printed per-line goes here. Any
 * information which can be clubbed on a commit/file level, should
 * be printed via 'emit_one_suspect_detail()'.
 */
ساكن فراغ emit_porcelain_فی_سطر_details(بنية blame_اندراج *ent)
{
	إذا (mark_unblamable_سطریں && ent->unblamable)
		puts("unblamable");
	إذا (mark_ignored_سطریں && ent->ignored)
		puts("ignored");
}

ساكن فراغ emit_porcelain(بنية blame_scoreboard *sb, بنية blame_اندراج *ent,
			   صحيح opt)
{
	صحيح repeat = opt & OUTPUT_سطر_PORCELAIN;
	صحيح cnt;
	ثابت حرف *cp;
	بنية blame_origin *suspect = ent->suspect;
	حرف hex[GIT_MAX_HEXSZ + 1];

	oid_منزل_hex_r(hex, &suspect->commit->object.oid);
	لکھو("%s %d %d %d\n",
	       hex,
	       ent->s_lno + 1,
	       ent->lno + 1,
	       ent->عدد_سطریں);
	emit_porcelain_details(suspect, repeat);
	emit_porcelain_فی_سطر_details(ent);

	cp = blame_nth_سطر(sb, ent->lno);
	لكل (cnt = 0; cnt < ent->عدد_سطریں; cnt++) {
		حرف ch;
		إذا (cnt) {
			لکھو("%s %d %d\n", hex,
			       ent->s_lno + 1 + cnt,
			       ent->lno + 1 + cnt);
			إذا (repeat)
				emit_porcelain_details(suspect, 1);
			emit_porcelain_فی_سطر_details(ent);
		}
		حرف_لکھو('\t');
		افعل {
			ch = *cp++;
			حرف_لکھو(ch);
		} طالما (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_ناپ);
	}

	إذا (sb->final_buf_ناپ && cp[-1] != '\n')
		حرف_لکھو('\n');
}

ساكن بنية color_میدان {
	timestamp_t hop;
	حرف خانہ[COLOR_MAXLEN];
} *colorfield;
ساكن صحيح colorfield_nr, colorfield_alloc;

ساكن فراغ parse_color_fields(ثابت حرف *s)
{
	بنية string_list l = STRING_LIST_INIT_DUP;
	بنية string_list_شے *شے;
	تعداد { EXPECT_DATE, EXPECT_COLOR } اگلا = EXPECT_COLOR;

	colorfield_nr = 0;

	/* Ideally this would be stripped and split at the same time? */
	string_list_ونڈو(&l, s, ",", -1);
	ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);

	for_each_string_list_شے(شے, &l) {
		بدل (اگلا) {
		حالة EXPECT_DATE:
			colorfield[colorfield_nr].hop = approxidate(شے->string);
			اگلا = EXPECT_COLOR;
			colorfield_nr++;
			ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);
			اكسر;
		حالة EXPECT_COLOR:
			إذا (color_parse(شے->string, colorfield[colorfield_nr].خانہ))
				die(_("expecting a color: %s"), شے->string);
			اگلا = EXPECT_DATE;
			اكسر;
		}
	}

	إذا (اگلا == EXPECT_COLOR)
		die(_("must end with a color"));

	colorfield[colorfield_nr].hop = ویلا_MAX;
	string_list_clear(&l, 0);
}

ساكن فراغ تیاری_default_color_by_age(فراغ)
{
	parse_color_fields("blue,12 month ago,white,1 month ago,red");
}

ساكن فراغ determine_سطر_heat(بنية commit_info *ci, ثابت حرف **dest_color)
{
	صحيح i = 0;

	طالما (i < colorfield_nr && ci->author_ویلا > colorfield[i].hop)
		i++;

	*dest_color = colorfield[i].خانہ;
}

ساكن مضمن صحيح maybe_putc(صحيح c, FILE *باہرلا)
{
	أرجع باہرلا ? putc(c, باہرلا) : 0;
}

ساكن ناپ_t print_marks(FILE *باہرلا, ثابت بنية blame_اندراج *ent, صحيح opt)
{
	ناپ_t len = 0;

	إذا ((ent->suspect->commit->object.flags & UNINTERESTING) &&
	    !blank_boundary && !(opt & OUTPUT_ANNOTATE_COMPAT)) {
		maybe_putc('^', باہرلا);
		len++;
	}
	إذا (mark_unblamable_سطریں && ent->unblamable) {
		maybe_putc('*', باہرلا);
		len++;
	}
	إذا (mark_ignored_سطریں && ent->ignored) {
		maybe_putc('?', باہرلا);
		len++;
	}
	أرجع len;
}

ساكن ناپ_t گنتی_marks(ثابت بنية blame_اندراج *ent, صحيح opt)
{
	أرجع print_marks(عدم, ent, opt);
}

ساكن فراغ emit_other(بنية blame_scoreboard *sb, بنية blame_اندراج *ent,
		       صحيح opt, بنية blame_اندراج *prev_ent)
{
	صحيح cnt;
	ثابت حرف *cp;
	بنية blame_origin *suspect = ent->suspect;
	بنية commit_info ci = COMMIT_INFO_INIT;
	حرف hex[GIT_MAX_HEXSZ + 1];
	صحيح show_raw_ویلا = !!(opt & OUTPUT_RAW_TIMESTAMP);
	ثابت حرف *default_color = عدم, *color = عدم, *reset = عدم;

	get_commit_info(suspect->commit, &ci);
	oid_منزل_hex_r(hex, &suspect->commit->object.oid);

	cp = blame_nth_سطر(sb, ent->lno);

	إذا (opt & OUTPUT_SHOW_AGE_WITH_COLOR) {
		determine_سطر_heat(&ci, &default_color);
		color = default_color;
		reset = GIT_COLOR_RESET;
	}

	إذا (abbrev < MINIMUM_ABBREV)
		BUG("abbreviation is smaller than minimum length: %d < %d",
		    abbrev, MINIMUM_ABBREV);

	لكل (cnt = 0; cnt < ent->عدد_سطریں; cnt++) {
		حرف ch;
		ناپ_t لمائی = (opt & OUTPUT_LONG_OBJECT_NAME) ?
			the_hash_algo->hexsz : (ناپ_t) abbrev;

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
			memset(hex, ' ', لڑی_لمبائی(hex));
		لمائی -= print_marks(stdout, ent, opt);

		لکھو("%.*s", (صحيح)(لمائی < GIT_MAX_HEXSZ ? لمائی : GIT_MAX_HEXSZ), hex);
		إذا (opt & OUTPUT_ANNOTATE_COMPAT) {
			ثابت حرف *name;
			إذا (opt & OUTPUT_SHOW_EMAIL)
				name = ci.author_mail.buf;
			وإلا
				name = ci.author.buf;
			لکھو("\t(%10s\t%10s\t%d)", name,
			       format_ویلا(ci.author_ویلا, ci.author_tz.buf,
					   show_raw_ویلا),
			       ent->lno + 1 + cnt);
		} وإلا {
			إذا (opt & OUTPUT_SHOW_SCORE)
				لکھو(" %*d %02d",
				       max_score_digits, ent->score,
				       ent->suspect->refcnt);
			إذا (opt & OUTPUT_SHOW_NAME)
				لکھو(" %-*.*s", longest_file, longest_file,
				       suspect->رستہ);
			إذا (opt & OUTPUT_SHOW_نمبر)
				لکھو(" %*d", max_orig_digits,
				       ent->s_lno + 1 + cnt);

			إذا (!(opt & OUTPUT_NO_AUTHOR)) {
				ثابت حرف *name;
				صحيح pad;
				إذا (opt & OUTPUT_SHOW_EMAIL)
					name = ci.author_mail.buf;
				وإلا
					name = ci.author.buf;
				pad = longest_author - utf8_strwidth(name);
				لکھو(" (%s%*s %10s",
				       name, pad, "",
				       format_ویلا(ci.author_ویلا,
						   ci.author_tz.buf,
						   show_raw_ویلا));
			}
			لکھو(" %*d) ",
			       max_digits, ent->lno + 1 + cnt);
		}
		إذا (reset)
			fputs(reset, stdout);
		افعل {
			ch = *cp++;
			حرف_لکھو(ch);
		} طالما (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_ناپ);
	}

	إذا (sb->final_buf_ناپ && cp[-1] != '\n')
		حرف_لکھو('\n');

	commit_info_destroy(&ci);
}

ساكن فراغ output(بنية blame_scoreboard *sb, صحيح option)
{
	بنية blame_اندراج *ent, *prev_ent = عدم;

	إذا (option & OUTPUT_PORCELAIN) {
		لكل (ent = sb->ent; ent; ent = ent->اگلا) {
			صحيح گنتی = 0;
			بنية blame_origin *suspect;
			بنية commit *commit = ent->suspect->commit;
			إذا (commit->object.flags & ہور_THAN_ONE_رستہ)
				استمر;
			لكل (suspect = get_blame_suspects(commit); suspect; suspect = suspect->اگلا) {
				إذا (suspect->guilty && گنتی++) {
					commit->object.flags |= ہور_THAN_ONE_رستہ;
					اكسر;
				}
			}
		}
	}

	لكل (ent = sb->ent; ent; ent = ent->اگلا) {
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
			رجسٹر_commit_graft(the_repository, graft, 0);
	}
	fclose(fp);
	strbuf_release(&buf);
	أرجع 0;
}

ساكن صحيح update_auto_abbrev(صحيح auto_abbrev, بنية blame_origin *suspect)
{
	ثابت حرف *uniq = repo_لبھو_unique_abbrev(the_repository,
						   &suspect->commit->object.oid,
						   auto_abbrev);
	صحيح len = لڑی_لمبائی(uniq);
	إذا (auto_abbrev < len)
		أرجع len;
	أرجع auto_abbrev;
}

/*
 * How many columns do we need to show line numbers, authors,
 * and filenames?
 */
ساكن فراغ لبھو_alignment(بنية blame_scoreboard *sb, صحيح *option)
{
	صحيح longest_src_سطریں = 0;
	صحيح longest_dst_سطریں = 0;
	بنانشانی سبھ_توں_وڈا_score = 0;
	بنية blame_اندراج *e;
	صحيح compute_auto_abbrev = (abbrev < 0);
	صحيح auto_abbrev = DEFAULT_ABBREV;
	ناپ_t max_marks_گنتی = 0;

	لكل (e = sb->ent; e; e = e->اگلا) {
		بنية blame_origin *suspect = e->suspect;
		صحيح عدد;
		ناپ_t marks_گنتی = گنتی_marks(e, *option);

		إذا (max_marks_گنتی < marks_گنتی)
			max_marks_گنتی = marks_گنتی;
		إذا (compute_auto_abbrev)
			auto_abbrev = update_auto_abbrev(auto_abbrev, suspect);
		إذا (strcmp(suspect->رستہ, sb->رستہ))
			*option |= OUTPUT_SHOW_NAME;
		عدد = لڑی_لمبائی(suspect->رستہ);
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
		عدد = e->s_lno + e->عدد_سطریں;
		إذا (longest_src_سطریں < عدد)
			longest_src_سطریں = عدد;
		عدد = e->lno + e->عدد_سطریں;
		إذا (longest_dst_سطریں < عدد)
			longest_dst_سطریں = عدد;
		إذا (سبھ_توں_وڈا_score < blame_اندراج_score(sb, e))
			سبھ_توں_وڈا_score = blame_اندراج_score(sb, e);
	}
	max_orig_digits = decimal_چوڑائی(longest_src_سطریں);
	max_digits = decimal_چوڑائی(longest_dst_سطریں);
	max_score_digits = decimal_چوڑائی(سبھ_توں_وڈا_score);

	إذا (compute_auto_abbrev)
		abbrev = auto_abbrev;
	إذا (abbrev < (صحيح)the_hash_algo->hexsz) {
		abbrev += max_marks_گنتی;
		إذا (abbrev > (صحيح)the_hash_algo->hexsz)
			abbrev = the_hash_algo->hexsz;
	}
}

ساكن فراغ sanity_check_on_fail(بنية blame_scoreboard *sb, صحيح baa)
{
	صحيح opt = OUTPUT_SHOW_SCORE | OUTPUT_SHOW_نمبر | OUTPUT_SHOW_NAME;
	لبھو_alignment(sb, &opt);
	output(sb, opt);
	die("Baa %d!", baa);
}

ساكن بنانشانی parse_score(ثابت حرف *arg)
{
	حرف *سرا;
	بنانشانی لما score = strtoul(arg, &سرا, 10);
	إذا (*سرا)
		أرجع 0;
	أرجع score;
}

ساكن حرف *جوڑو_اگیتر(ثابت حرف *اگیتر, ثابت حرف *رستہ)
{
	أرجع اگیتر_رستہ(the_repository, اگیتر, اگیتر ? لڑی_لمبائی(اگیتر) : 0, رستہ);
}

ساكن صحيح git_blame_config(ثابت حرف *var, ثابت حرف *قدر,
			    ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, "blame.showroot")) {
		show_جڑ = git_config_bool(var, قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.blankboundary")) {
		blank_boundary = git_config_bool(var, قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.showemail")) {
		صحيح *output_option = cb;
		إذا (git_config_bool(var, قدر))
			*output_option |= OUTPUT_SHOW_EMAIL;
		وإلا
			*output_option &= ~OUTPUT_SHOW_EMAIL;
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.date")) {
		إذا (!قدر)
			أرجع config_error_nonbool(var);
		parse_date_format(قدر, &blame_date_منوال);
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.ignorerevsfile")) {
		حرف *str;
		صحيح ret;

		ret = git_config_pathname(&str, var, قدر);
		إذا (ret)
			أرجع ret;
		إذا (str)
			string_list_داخل_کرو(&ignore_revs_file_list, str);
		حرر(str);
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.markunblamablelines")) {
		mark_unblamable_سطریں = git_config_bool(var, قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.markignoredlines")) {
		mark_ignored_سطریں = git_config_bool(var, قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "color.blame.repeatedlines")) {
		إذا (color_parse_mem(قدر, لڑی_لمبائی(قدر), repeated_meta_color))
			warning(_("invalid value for '%s': '%s'"),
				"color.blame.repeatedLines", قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "color.blame.highlightrecent")) {
		parse_color_fields(قدر);
		أرجع 0;
	}

	إذا (!strcmp(var, "blame.coloring")) {
		إذا (!قدر)
			أرجع config_error_nonbool(var);
		إذا (!strcmp(قدر, "repeatedLines")) {
			coloring_منوال |= OUTPUT_COLOR_سطر;
		} وإلا إذا (!strcmp(قدر, "highlightRecent")) {
			coloring_منوال |= OUTPUT_SHOW_AGE_WITH_COLOR;
		} وإلا إذا (!strcmp(قدر, "none")) {
			coloring_منوال &= ~(OUTPUT_COLOR_سطر |
					    OUTPUT_SHOW_AGE_WITH_COLOR);
		} وإلا {
			warning(_("invalid value for '%s': '%s'"),
				"blame.coloring", قدر);
			أرجع 0;
		}
	}

	إذا (!strcmp(var, "diff.algorithm")) {
		لما diff_algorithm;
		إذا (!قدر)
			أرجع config_error_nonbool(var);
		diff_algorithm = parse_algorithm_قدر(قدر);
		إذا (diff_algorithm < 0)
			أرجع error(_("unknown value for config '%s': %s"),
				     var, قدر);
		xdl_opts &= ~XDF_DIFF_ALGORITHM_نقاب;
		xdl_opts |= diff_algorithm;
		أرجع 0;
	}

	إذا (git_diff_heuristic_config(var, قدر, cb) < 0)
		أرجع -1;
	إذا (userdiff_config(var, قدر) < 0)
		أرجع -1;

	أرجع git_default_config(var, قدر, ctx, cb);
}

ساكن صحيح blame_copy_callback(ثابت بنية option *option, ثابت حرف *arg, صحيح unset)
{
	صحيح *opt = option->قدر;

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
	صحيح *opt = option->قدر;

	BUG_ON_OPT_NEG(unset);

	*opt |= PICKAXE_BLAME_MOVE;

	إذا (arg)
		blame_move_score = parse_score(arg);
	أرجع 0;
}

ساكن صحيح blame_diff_algorithm_اقل(ثابت بنية option *option,
					ثابت حرف *arg, صحيح unset)
{
	صحيح *opt = option->قدر;

	BUG_ON_OPT_ARG(arg);

	*opt &= ~XDF_DIFF_ALGORITHM_نقاب;
	إذا (!unset)
		*opt |= XDF_NEED_اقل;

	أرجع 0;
}

ساكن صحيح blame_diff_algorithm_callback(ثابت بنية option *option,
					 ثابت حرف *arg, صحيح unset)
{
	صحيح *opt = option->قدر;
	لما قدر = parse_algorithm_قدر(arg);

	BUG_ON_OPT_NEG(unset);

	إذا (قدر < 0)
		أرجع error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	*opt &= ~XDF_DIFF_ALGORITHM_نقاب;
	*opt |= قدر;

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
		صحيح قسم = odb_read_object_info(r->objects, &oid, عدم);
		إذا (قسم == OBJ_COMMIT) {
			oidcpy(oid_ret, &oid);
			أرجع 0;
		}
		إذا (قسم != OBJ_ٹیگ)
			أرجع -1;
		obj = deref_ٹیگ(r, parse_object(r, &oid), عدم, 0);
		إذا (!obj)
			أرجع -1;
		oidcpy(&oid, &obj->oid);
	}
}

ساكن فراغ build_ignorelist(بنية blame_scoreboard *sb,
			     بنية string_list *ignore_revs_file_list,
			     بنية string_list *ignore_rev_list)
{
	بنية string_list_شے *i;
	بنية object_id oid;

	oidset_init(&sb->ignore_list, 0);
	for_each_string_list_شے(i, ignore_revs_file_list) {
		إذا (!strcmp(i->string, ""))
			oidset_clear(&sb->ignore_list);
		وإلا
			oidset_parse_file_carefully(&sb->ignore_list, i->string,
						    the_repository->hash_algo,
						    peel_منزل_commit_oid, sb);
	}
	for_each_string_list_شے(i, ignore_rev_list) {
		إذا (repo_get_oid_committish(the_repository, i->string, &oid) ||
		    peel_منزل_commit_oid(&oid, sb))
			die(_("cannot find revision %s to ignore"), i->string);
		oidset_داخل_کرو(&sb->ignore_list, &oid);
	}
}

صحيح cmd_blame(صحيح argc,
	      ثابت حرف **argv,
	      ثابت حرف *اگیتر,
	      بنية repository *repo بن_ورتیا)
{
	بنية rev_info revs;
	حرف *رستہ = عدم;
	بنية blame_scoreboard sb;
	بنية blame_origin *o;
	بنية blame_اندراج *ent = عدم;
	لما dashdash_pos, lno;
	بنية ترقی_info pi = { عدم, 0 };

	بنية string_list range_list = STRING_LIST_INIT_NODUP;
	بنية string_list ignore_rev_list = STRING_LIST_INIT_NODUP;
	صحيح output_option = 0, opt = 0;
	صحيح show_stats = 0;
	ثابت حرف *revs_file = عدم;
	ثابت حرف *contents_from = عدم;
	ثابت بنية option options[] = {
		OPT_BOOL(0, "incremental", &incremental, N_("show blame entries as we find them, incrementally")),
		OPT_BOOL('b', عدم, &blank_boundary, N_("do not show object names of boundary commits (Default: off)")),
		OPT_BOOL(0, "root", &show_جڑ, N_("do not treat root commits as boundaries (Default: off)")),
		OPT_BOOL(0, "show-stats", &show_stats, N_("show work cost statistics")),
		OPT_BOOL(0, "progress", &show_ترقی, N_("force progress reporting")),
		OPT_ثنائی_ہندسہ(0, "score-debug", &output_option, N_("show output score for blame entries"), OUTPUT_SHOW_SCORE),
		OPT_ثنائی_ہندسہ('f', "show-name", &output_option, N_("show original filename (Default: auto)"), OUTPUT_SHOW_NAME),
		OPT_ثنائی_ہندسہ('n', "show-number", &output_option, N_("show original linenumber (Default: off)"), OUTPUT_SHOW_نمبر),
		OPT_ثنائی_ہندسہ('p', "porcelain", &output_option, N_("show in a format designed for machine consumption"), OUTPUT_PORCELAIN),
		OPT_ثنائی_ہندسہ(0, "line-porcelain", &output_option, N_("show porcelain format with per-line commit information"), OUTPUT_PORCELAIN|OUTPUT_سطر_PORCELAIN),
		OPT_ثنائی_ہندسہ('c', عدم, &output_option, N_("use the same output mode as git-annotate (Default: off)"), OUTPUT_ANNOTATE_COMPAT),
		OPT_ثنائی_ہندسہ('t', عدم, &output_option, N_("show raw timestamp (Default: off)"), OUTPUT_RAW_TIMESTAMP),
		OPT_ثنائی_ہندسہ('l', عدم, &output_option, N_("show long commit SHA1 (Default: off)"), OUTPUT_LONG_OBJECT_NAME),
		OPT_ثنائی_ہندسہ('s', عدم, &output_option, N_("suppress author name and timestamp (Default: off)"), OUTPUT_NO_AUTHOR),
		OPT_ثنائی_ہندسہ('e', "show-email", &output_option, N_("show author email instead of name (Default: off)"), OUTPUT_SHOW_EMAIL),
		OPT_ثنائی_ہندسہ('w', عدم, &xdl_opts, N_("ignore whitespace differences"), XDF_IGNORE_WHITESPACE),
		OPT_CALLBACK_F(0, "diff-algorithm", &xdl_opts, N_("<algorithm>"),
			       N_("choose a diff algorithm"),
			       PARSE_OPT_NONEG, blame_diff_algorithm_callback),
		OPT_STRING_LIST(0, "ignore-rev", &ignore_rev_list, N_("rev"), N_("ignore <rev> when blaming")),
		OPT_STRING_LIST(0, "ignore-revs-file", &ignore_revs_file_list, N_("file"), N_("ignore revisions from <file>")),
		OPT_ثنائی_ہندسہ(0, "color-lines", &output_option, N_("color redundant metadata from previous line differently"), OUTPUT_COLOR_سطر),
		OPT_ثنائی_ہندسہ(0, "color-by-age", &output_option, N_("color lines by age"), OUTPUT_SHOW_AGE_WITH_COLOR),
		OPT_CALLBACK_F(0, "minimal", &xdl_opts, عدم,
			       N_("spend extra cycles to find a better match"),
			       PARSE_OPT_NOARG | PARSE_OPT_HIDDEN, blame_diff_algorithm_اقل),
		OPT_STRING('S', عدم, &revs_file, N_("file"), N_("use revisions from <file> instead of calling git-rev-list")),
		OPT_STRING(0, "contents", &contents_from, N_("file"), N_("use <file>'s contents as the final image")),
		OPT_CALLBACK_F('C', عدم, &opt, N_("score"), N_("find line copies within and across files"), PARSE_OPT_OPTARG, blame_copy_callback),
		OPT_CALLBACK_F('M', عدم, &opt, N_("score"), N_("find line movements within and across files"), PARSE_OPT_OPTARG, blame_move_callback),
		OPT_STRING_LIST('L', عدم, &range_list, N_("range"),
				N_("process only line range <start>,<end> or function :<funcname>")),
		OPT__ABBREV(&abbrev),
		OPT_سرا()
	};

	بنية parse_opt_ctx_t ctx;
	صحيح cmd_is_annotate = !strcmp(argv[0], "annotate");
	بنية range_سیٹ ranges;
	بنانشانی صحيح range_i;
	لما anchor;
	لما عدد_سطریں = 0;
	ثابت حرف *str_usage = cmd_is_annotate ? annotate_usage : blame_usage;
	ثابت حرف *ثابت *opt_usage = cmd_is_annotate ? annotate_opt_usage : blame_opt_usage;

	تیاری_default_color_by_age();
	repo_config(the_repository, git_blame_config, &output_option);
	repo_init_revisions(the_repository, &revs, عدم);
	revs.date_منوال = blame_date_منوال;
	revs.diffopt.flags.allow_textconv = 1;
	revs.diffopt.flags.follow_renames = 1;

	save_commit_بفر = 0;
	dashdash_pos = 0;
	show_ترقی = -1;

	parse_options_شروع(&ctx, argc, argv, اگیتر, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);
	لكل (;;) {
		بدل (parse_options_قدم(&ctx, options, opt_usage)) {
		حالة PARSE_OPT_NON_OPTION:
		حالة PARSE_OPT_UNKNOWN:
			اكسر;
		حالة PARSE_OPT_HELP:
			باہر_نکلو(0);
		حالة PARSE_OPT_HELP_ERROR:
		حالة PARSE_OPT_ERROR:
		حالة PARSE_OPT_SUBCOMMAND:
			باہر_نکلو(129);
		حالة PARSE_OPT_COMPLETE:
			باہر_نکلو(0);
		حالة PARSE_OPT_پورا:
			إذا (ctx.argv[0])
				dashdash_pos = ctx.cpidx;
			اذهب parse_پورا;
		}

		إذا (!strcmp(ctx.argv[0], "--reverse")) {
			ctx.argv[0] = "--children";
			الٹ = 1;
		}
		parse_revision_opt(&revs, &ctx, options, opt_usage);
	}
parse_پورا:
	revision_opts_انت(&revs);
	no_سارا_file_rename = !revs.diffopt.flags.follow_renames;
	xdl_opts |= revs.diffopt.xdl_opts & XDF_INDENT_HEURISTIC;
	revs.diffopt.flags.follow_renames = 0;
	argc = parse_options_سرا(&ctx);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_بھریا_اشاریہ = 0;

	إذا (incremental || (output_option & OUTPUT_PORCELAIN)) {
		إذا (show_ترقی > 0)
			die(_("--progress can't be used with --incremental or porcelain formats"));
		show_ترقی = 0;
	} وإلا إذا (show_ترقی < 0)
		show_ترقی = isatty(2);

	إذا (!abbrev)
		abbrev = the_hash_algo->hexsz;

	إذا (revs_file && read_ancestry(revs_file))
		die_errno("reading graft file '%s' failed", revs_file);

	إذا (cmd_is_annotate) {
		output_option |= OUTPUT_ANNOTATE_COMPAT;
		blame_date_منوال.type = DATE_ISO8601;
	} وإلا {
		blame_date_منوال = revs.date_منوال;
	}

	/* The maximum width used to show the dates */
	بدل (blame_date_منوال.type) {
	حالة DATE_RFC2822:
		blame_date_چوڑائی = حجم("Thu, 19 Oct 2006 16:00:04 -0700");
		اكسر;
	حالة DATE_ISO8601_STRICT:
		blame_date_چوڑائی = حجم("2006-10-19T16:00:04-07:00");
		اكسر;
	حالة DATE_ISO8601:
		blame_date_چوڑائی = حجم("2006-10-19 16:00:04 -0700");
		اكسر;
	حالة DATE_RAW:
		blame_date_چوڑائی = حجم("1161298804 -0700");
		اكسر;
	حالة DATE_UNIX:
		blame_date_چوڑائی = حجم("1161298804");
		اكسر;
	حالة DATE_SHORT:
		blame_date_چوڑائی = حجم("2006-10-19");
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
		blame_date_چوڑائی = utf8_strwidth(_("4 years, 11 months ago")) + 1; /* add the null */
		اكسر;
	حالة DATE_HUMAN:
		/* If the year is shown, no time is shown */
		blame_date_چوڑائی = حجم("Thu Oct 19 16:00");
		اكسر;
	حالة DATE_عام:
		blame_date_چوڑائی = حجم("Thu Oct 19 16:00:04 2006 -0700");
		اكسر;
	حالة DATE_STRFTIME:
		blame_date_چوڑائی = لڑی_لمبائی(show_date(0, 0, blame_date_منوال)) + 1; /* add the null */
		اكسر;
	}
	blame_date_چوڑائی -= 1; /* strip the null */

	إذا (revs.diffopt.flags.لبھو_copies_harder)
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
			رستہ = جوڑو_اگیتر(اگیتر, argv[--argc]);
			argv[argc] = عدم;
			اكسر;
		افتراضي:
			usage_with_options(opt_usage, options);
		}
	} وإلا {
		إذا (argc < 2)
			usage_with_options(opt_usage, options);
		إذا (argc == 3 && is_a_rev(argv[argc - 1])) { /* (2b) */
			رستہ = جوڑو_اگیتر(اگیتر, argv[1]);
			argv[1] = argv[2];
		} وإلا {	/* (2a) */
			إذا (argc == 2 && is_a_rev(argv[1]) && !repo_get_work_درخت(the_repository))
				die("missing <path> to blame");
			رستہ = جوڑو_اگیتر(اگیتر, argv[argc - 1]);
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

		جوڑو_pending_object(&revs, &سر_commit->object, "HEAD");
	}

	init_scoreboard(&sb);
	sb.revs = &revs;
	sb.contents_from = contents_from;
	sb.الٹ = الٹ;
	sb.repo = the_repository;
	sb.رستہ = رستہ;
	build_ignorelist(&sb, &ignore_revs_file_list, &ignore_rev_list);
	string_list_clear(&ignore_revs_file_list, 0);
	string_list_clear(&ignore_rev_list, 0);
	تیاری_scoreboard(&sb, &o);

	/*
	 * Changed-path Bloom filters are disabled when looking
	 * for copies.
	 */
	إذا (!(opt & PICKAXE_BLAME_COPY))
		تیاری_blame_bloom_انکڑے(&sb);

	lno = sb.عدد_سطریں;

	إذا (lno && !range_list.nr)
		string_list_append(&range_list, "1");

	anchor = 1;
	range_سیٹ_init(&ranges, range_list.nr);
	لكل (range_i = 0; range_i < range_list.nr; ++range_i) {
		لما bottom, چوٹی;
		إذا (parse_range_arg(range_list.اشیاء[range_i].string,
				    nth_سطر_cb, &sb, lno, anchor,
				    &bottom, &چوٹی, sb.رستہ,
				    the_repository->اشاریہ))
			usage(str_usage);
		إذا ((!lno && (چوٹی || bottom)) || lno < bottom)
			die(Q_("file %s has only %lu line",
			       "file %s has only %lu lines",
			       lno), sb.رستہ, lno);
		إذا (bottom < 1)
			bottom = 1;
		إذا (چوٹی < 1 || lno < چوٹی)
			چوٹی = lno;
		bottom--;
		range_سیٹ_append_unsafe(&ranges, bottom, چوٹی);
		anchor = چوٹی + 1;
	}
	ترتیب_دیو_and_ملاؤ_range_سیٹ(&ranges);

	لكل (range_i = ranges.nr; range_i > 0; --range_i) {
		ثابت بنية range *r = &ranges.ranges[range_i - 1];
		ent = blame_اندراج_prepend(ent, r->شروع, r->سرا, o);
		عدد_سطریں += (r->سرا - r->شروع);
	}
	إذا (!عدد_سطریں)
		عدد_سطریں = sb.عدد_سطریں;

	o->suspects = ent;
	prio_قطار_put(&sb.commits, o->commit);

	blame_origin_decref(o);

	range_سیٹ_release(&ranges);
	string_list_clear(&range_list, 0);

	sb.ent = عدم;

	إذا (blame_move_score)
		sb.move_score = blame_move_score;
	إذا (blame_copy_score)
		sb.copy_score = blame_copy_score;

	sb.debug = DEBUG_BLAME;
	sb.on_sanity_fail = &sanity_check_on_fail;

	sb.show_جڑ = show_جڑ;
	sb.xdl_opts = xdl_opts;
	sb.no_سارا_file_rename = no_سارا_file_rename;

	read_mailmap(the_repository, &mailmap);

	sb.لبھیا_guilty_اندراج = &لبھیا_guilty_اندراج;
	sb.لبھیا_guilty_اندراج_انکڑے = &pi;
	إذا (show_ترقی)
		pi.ترقی = شروع_delayed_ترقی(the_repository,
						     _("Blaming lines"),
						     عدد_سطریں);

	سونپو_blame(&sb, opt);

	stop_ترقی(&pi.ترقی);

	إذا (!incremental)
		تیاری_pager(the_repository);
	وإلا
		اذهب cleanup;

	blame_ترتیب_دیو_final(&sb);

	blame_coalesce(&sb);

	إذا (!(output_option & (OUTPUT_COLOR_سطر | OUTPUT_SHOW_AGE_WITH_COLOR)))
		output_option |= coloring_منوال;

	إذا (!(output_option & OUTPUT_PORCELAIN)) {
		لبھو_alignment(&sb, &output_option);
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
		لکھو("num read blob: %d\n", sb.عدد_read_blob);
		لکھو("num get patch: %d\n", sb.عدد_get_patch);
		لکھو("num commits: %d\n", sb.عدد_commits);
	}

cleanup:
	لكل (ent = sb.ent; ent; ) {
		بنية blame_اندراج *e = ent->اگلا;
		حرر(ent);
		ent = e;
	}

	حرر(رستہ);
	cleanup_scoreboard(&sb);
	release_revisions(&revs);
	أرجع 0;
}
