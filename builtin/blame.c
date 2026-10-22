/*
 * Blame
 *
 * Copyright (c) 2006, 2014 by its authors
 * See COPYING for licensing conditions
 */

#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE

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
ساكن صحيح show_پاڙ;
ساكن صحيح ابتو;
ساكن صحيح blank_boundary;
ساكن صحيح incremental;
ساكن صحيح xdl_opts;
ساكن صحيح abbrev = -1;
ساكن صحيح no_سڄو_file_rename;
ساكن صحيح show_ترقي;
ساكن حرف repeated_meta_color[COLOR_MAXLEN];
ساكن صحيح coloring_گهڻائي;
ساكن بنية string_list ignore_revs_file_list = STRING_LIST_INIT_DUP;
ساكن صحيح mark_unblamable_سٽون;
ساكن صحيح mark_ignored_سٽون;

ساكن بنية date_گهڻائي blame_date_گهڻائي = { DATE_ISO8601 };
ساكن سائيز_t blame_date_ويڪر;

ساكن بنية string_list mailmap = STRING_LIST_INIT_NODUP;

#جيڪڏهن_وصف_نه_ٿيل DEBUG_BLAME
#وصف_ڪريو DEBUG_BLAME 0
#جيڪڏهن_پورو

ساكن بنانشان blame_move_score;
ساكن بنانشان blame_copy_score;

/* Remember to update object flag allocation in object.h */
#وصف_ڪريو METAINFO_SHOWN		(1u<<12)
#وصف_ڪريو وڌيڪ_THAN_ONE_رستو	(1u<<13)

بنية ترقي_info {
	بنية ترقي *ترقي;
	صحيح blamed_سٽون;
};

ساكن ثابت حرف *nth_سٽ_cb(فراغ *معلومات, ڊگهو lno)
{
	أرجع blame_nth_سٽ((بنية blame_scoreboard *)معلومات, lno);
}

/*
 * Information on commits, used for output.
 */
بنية commit_info {
	بنية strbuf author;
	بنية strbuf author_mail;
	timestamp_t author_وقت;
	بنية strbuf author_tz;

	/* filled only when asked for details */
	بنية strbuf committer;
	بنية strbuf committer_mail;
	timestamp_t committer_وقت;
	بنية strbuf committer_tz;

	بنية strbuf summary;
};

#وصف_ڪريو COMMIT_INFO_INIT { \
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
ساكن فراغ get_ac_سٽ(ثابت حرف *inbuf, ثابت حرف *what,
	بنية strbuf *name, بنية strbuf *mail,
	timestamp_t *وقت, بنية strbuf *tz)
{
	بنية ident_ورهايو ident;
	سائيز_t len, maillen, namelen;
	ثابت حرف *tmp, *endp;
	ثابت حرف *namebuf, *mailbuf;

	tmp = لڙي_ڳولا(inbuf, what);
	إذا (!tmp)
		اذهب error_ٻاهريون;
	tmp += لڙي_ڊيگهه(what);
	endp = strchr(tmp, '\n');
	إذا (!endp)
		len = لڙي_ڊيگهه(tmp);
	وإلا
		len = endp - tmp;

	إذا (ورهايو_ident_سٽ(&ident, tmp, len)) {
	error_ٻاهريون:
		/* Ugh */
		tmp = "(unknown)";
		strbuf_addstr(name, tmp);
		strbuf_addstr(mail, tmp);
		strbuf_addstr(tz, tmp);
		*وقت = 0;
		أرجع;
	}

	namelen = ident.name_پڇاڙي - ident.name_begin;
	namebuf = ident.name_begin;

	maillen = ident.mail_پڇاڙي - ident.mail_begin;
	mailbuf = ident.mail_begin;

	إذا (ident.date_begin && ident.date_پڇاڙي)
		*وقت = strtoul(ident.date_begin, عدم, 10);
	وإلا
		*وقت = 0;

	إذا (ident.tz_begin && ident.tz_پڇاڙي)
		strbuf_جوڙيو(tz, ident.tz_begin, ident.tz_پڇاڙي - ident.tz_begin);
	وإلا
		strbuf_addstr(tz, "(unknown)");

	/*
	 * Now, convert both name and e-mail using mailmap
	 */
	map_user(&mailmap, &mailbuf, &maillen,
		 &namebuf, &namelen);

	strbuf_addf(mail, "<%.*s>", (صحيح)maillen, mailbuf);
	strbuf_جوڙيو(name, namebuf, namelen);
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
	ثابت حرف *پيغام;

	encoding = get_log_output_encoding();
	پيغام = repo_logmsg_reencode(the_repository, commit, عدم, encoding);
	get_ac_سٽ(پيغام, "\nauthor ",
		    &ret->author, &ret->author_mail,
		    &ret->author_وقت, &ret->author_tz);

	get_ac_سٽ(پيغام, "\ncommitter ",
		    &ret->committer, &ret->committer_mail,
		    &ret->committer_وقت, &ret->committer_tz);

	len = ڳوليو_commit_subject(پيغام, &subject);
	إذا (len)
		strbuf_جوڙيو(&ret->summary, subject, len);
	وإلا
		strbuf_addf(&ret->summary, "(%s)", oid_منزل_hex(&commit->object.oid));

	repo_unuse_commit_بفر(the_repository, commit, پيغام);
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
	إذا (suspect->اڳيون) {
		بنية blame_origin *prev = suspect->اڳيون;
		لکو("previous %s ", oid_منزل_hex(&prev->commit->object.oid));
		write_name_quoted(prev->رستو, stdout, '\n');
	}
	لکو("filename ");
	write_name_quoted(suspect->رستو, stdout, '\n');
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
	لکو("author %s\n", ci.author.buf);
	لکو("author-mail %s\n", ci.author_mail.buf);
	لکو("author-time %"PRItime"\n", ci.author_وقت);
	لکو("author-tz %s\n", ci.author_tz.buf);
	لکو("committer %s\n", ci.committer.buf);
	لکو("committer-mail %s\n", ci.committer_mail.buf);
	لکو("committer-time %"PRItime"\n", ci.committer_وقت);
	لکو("committer-tz %s\n", ci.committer_tz.buf);
	لکو("summary %s\n", ci.summary.buf);
	إذا (suspect->commit->object.flags & UNINTERESTING)
		لکو("boundary\n");

	commit_info_destroy(&ci);

	أرجع 1;
}

/*
 * The blame_entry is found to be guilty for the range.
 * Show it in incremental output.
 */
ساكن فراغ مليو_guilty_داخلا(بنية blame_داخلا *ent, فراغ *معلومات)
{
	بنية ترقي_info *pi = (بنية ترقي_info *)معلومات;

	إذا (incremental) {
		بنية blame_origin *suspect = ent->suspect;

		لکو("%s %d %d %d\n",
		       oid_منزل_hex(&suspect->commit->object.oid),
		       ent->s_lno + 1, ent->lno + 1, ent->انگ_سٽون);
		emit_one_suspect_detail(suspect, 0);
		write_filename_info(suspect);
		maybe_flush_or_die(stdout, "stdout");
	}
	pi->blamed_سٽون += ent->انگ_سٽون;
	display_ترقي(pi->ترقي, pi->blamed_سٽون);
}

ساكن ثابت حرف *format_وقت(timestamp_t وقت, ثابت حرف *tz_str,
			       صحيح show_raw_وقت)
{
	ساكن بنية strbuf وقت_buf = STRBUF_INIT;

	strbuf_reset(&وقت_buf);
	إذا (show_raw_وقت) {
		strbuf_addf(&وقت_buf, "%"PRItime" %s", وقت, tz_str);
	}
	وإلا {
		ثابت حرف *وقت_str;
		سائيز_t وقت_ويڪر;
		صحيح tz;
		tz = atoi(tz_str);
		وقت_str = show_date(وقت, tz, blame_date_گهڻائي);
		strbuf_addstr(&وقت_buf, وقت_str);
		/*
		 * Add space paddings to time_buf to display a fixed width
		 * string, and use time_width for display width calibration.
		 */
		لكل (وقت_ويڪر = utf8_strwidth(وقت_str);
		     وقت_ويڪر < blame_date_ويڪر;
		     وقت_ويڪر++)
			strbuf_addch(&وقت_buf, ' ');
	}
	أرجع وقت_buf.buf;
}

#وصف_ڪريو OUTPUT_ANNOTATE_COMPAT      (1U<<0)
#وصف_ڪريو OUTPUT_LONG_OBJECT_NAME     (1U<<1)
#وصف_ڪريو OUTPUT_RAW_TIMESTAMP        (1U<<2)
#وصف_ڪريو OUTPUT_PORCELAIN            (1U<<3)
#وصف_ڪريو OUTPUT_SHOW_NAME            (1U<<4)
#وصف_ڪريو OUTPUT_SHOW_عدد          (1U<<5)
#وصف_ڪريو OUTPUT_SHOW_SCORE           (1U<<6)
#وصف_ڪريو OUTPUT_NO_AUTHOR            (1U<<7)
#وصف_ڪريو OUTPUT_SHOW_EMAIL           (1U<<8)
#وصف_ڪريو OUTPUT_سٽ_PORCELAIN       (1U<<9)
#وصف_ڪريو OUTPUT_COLOR_سٽ           (1U<<10)
#وصف_ڪريو OUTPUT_SHOW_AGE_WITH_COLOR  (1U<<11)

ساكن فراغ emit_porcelain_details(بنية blame_origin *suspect, صحيح repeat)
{
	إذا (emit_one_suspect_detail(suspect, repeat) ||
	    (suspect->commit->object.flags & وڌيڪ_THAN_ONE_رستو))
		write_filename_info(suspect);
}

/*
 * Information which needs to be printed per-line goes here. Any
 * information which can be clubbed on a commit/file level, should
 * be printed via 'emit_one_suspect_detail()'.
 */
ساكن فراغ emit_porcelain_في_سٽ_details(بنية blame_داخلا *ent)
{
	إذا (mark_unblamable_سٽون && ent->unblamable)
		puts("unblamable");
	إذا (mark_ignored_سٽون && ent->ignored)
		puts("ignored");
}

ساكن فراغ emit_porcelain(بنية blame_scoreboard *sb, بنية blame_داخلا *ent,
			   صحيح opt)
{
	صحيح repeat = opt & OUTPUT_سٽ_PORCELAIN;
	صحيح cnt;
	ثابت حرف *cp;
	بنية blame_origin *suspect = ent->suspect;
	حرف hex[GIT_MAX_HEXSZ + 1];

	oid_منزل_hex_r(hex, &suspect->commit->object.oid);
	لکو("%s %d %d %d\n",
	       hex,
	       ent->s_lno + 1,
	       ent->lno + 1,
	       ent->انگ_سٽون);
	emit_porcelain_details(suspect, repeat);
	emit_porcelain_في_سٽ_details(ent);

	cp = blame_nth_سٽ(sb, ent->lno);
	لكل (cnt = 0; cnt < ent->انگ_سٽون; cnt++) {
		حرف ch;
		إذا (cnt) {
			لکو("%s %d %d\n", hex,
			       ent->s_lno + 1 + cnt,
			       ent->lno + 1 + cnt);
			إذا (repeat)
				emit_porcelain_details(suspect, 1);
			emit_porcelain_في_سٽ_details(ent);
		}
		اکر_لکو('\t');
		افعل {
			ch = *cp++;
			اکر_لکو(ch);
		} طالما (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_سائيز);
	}

	إذا (sb->final_buf_سائيز && cp[-1] != '\n')
		اکر_لکو('\n');
}

ساكن بنية color_ميدان {
	timestamp_t hop;
	حرف کانو[COLOR_MAXLEN];
} *colorfield;
ساكن صحيح colorfield_nr, colorfield_alloc;

ساكن فراغ parse_color_fields(ثابت حرف *s)
{
	بنية string_list l = STRING_LIST_INIT_DUP;
	بنية string_list_شي *شي;
	تعداد { EXPECT_DATE, EXPECT_COLOR } ايندڙ = EXPECT_COLOR;

	colorfield_nr = 0;

	/* Ideally this would be stripped and split at the same time? */
	string_list_ورهايو(&l, s, ",", -1);
	ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);

	for_each_string_list_شي(شي, &l) {
		بدل (ايندڙ) {
		حالة EXPECT_DATE:
			colorfield[colorfield_nr].hop = approxidate(شي->string);
			ايندڙ = EXPECT_COLOR;
			colorfield_nr++;
			ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);
			اكسر;
		حالة EXPECT_COLOR:
			إذا (color_parse(شي->string, colorfield[colorfield_nr].کانو))
				die(_("expecting a color: %s"), شي->string);
			ايندڙ = EXPECT_DATE;
			اكسر;
		}
	}

	إذا (ايندڙ == EXPECT_COLOR)
		die(_("must end with a color"));

	colorfield[colorfield_nr].hop = وقت_MAX;
	string_list_clear(&l, 0);
}

ساكن فراغ تياري_default_color_by_age(فراغ)
{
	parse_color_fields("blue,12 month ago,white,1 month ago,red");
}

ساكن فراغ determine_سٽ_heat(بنية commit_info *ci, ثابت حرف **dest_color)
{
	صحيح i = 0;

	طالما (i < colorfield_nr && ci->author_وقت > colorfield[i].hop)
		i++;

	*dest_color = colorfield[i].کانو;
}

ساكن مضمن صحيح maybe_putc(صحيح c, FILE *ٻاهريون)
{
	أرجع ٻاهريون ? putc(c, ٻاهريون) : 0;
}

ساكن سائيز_t print_marks(FILE *ٻاهريون, ثابت بنية blame_داخلا *ent, صحيح opt)
{
	سائيز_t len = 0;

	إذا ((ent->suspect->commit->object.flags & UNINTERESTING) &&
	    !blank_boundary && !(opt & OUTPUT_ANNOTATE_COMPAT)) {
		maybe_putc('^', ٻاهريون);
		len++;
	}
	إذا (mark_unblamable_سٽون && ent->unblamable) {
		maybe_putc('*', ٻاهريون);
		len++;
	}
	إذا (mark_ignored_سٽون && ent->ignored) {
		maybe_putc('?', ٻاهريون);
		len++;
	}
	أرجع len;
}

ساكن سائيز_t ڳڻپ_marks(ثابت بنية blame_داخلا *ent, صحيح opt)
{
	أرجع print_marks(عدم, ent, opt);
}

ساكن فراغ emit_other(بنية blame_scoreboard *sb, بنية blame_داخلا *ent,
		       صحيح opt, بنية blame_داخلا *prev_ent)
{
	صحيح cnt;
	ثابت حرف *cp;
	بنية blame_origin *suspect = ent->suspect;
	بنية commit_info ci = COMMIT_INFO_INIT;
	حرف hex[GIT_MAX_HEXSZ + 1];
	صحيح show_raw_وقت = !!(opt & OUTPUT_RAW_TIMESTAMP);
	ثابت حرف *default_color = عدم, *color = عدم, *reset = عدم;

	get_commit_info(suspect->commit, &ci);
	oid_منزل_hex_r(hex, &suspect->commit->object.oid);

	cp = blame_nth_سٽ(sb, ent->lno);

	إذا (opt & OUTPUT_SHOW_AGE_WITH_COLOR) {
		determine_سٽ_heat(&ci, &default_color);
		color = default_color;
		reset = GIT_COLOR_RESET;
	}

	إذا (abbrev < MINIMUM_ABBREV)
		BUG("abbreviation is smaller than minimum length: %d < %d",
		    abbrev, MINIMUM_ABBREV);

	لكل (cnt = 0; cnt < ent->انگ_سٽون; cnt++) {
		حرف ch;
		سائيز_t ڊيگهائي = (opt & OUTPUT_LONG_OBJECT_NAME) ?
			the_hash_algo->hexsz : (سائيز_t) abbrev;

		إذا (opt & OUTPUT_COLOR_سٽ) {
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
			memset(hex, ' ', لڙي_ڊيگهه(hex));
		ڊيگهائي -= print_marks(stdout, ent, opt);

		لکو("%.*s", (صحيح)(ڊيگهائي < GIT_MAX_HEXSZ ? ڊيگهائي : GIT_MAX_HEXSZ), hex);
		إذا (opt & OUTPUT_ANNOTATE_COMPAT) {
			ثابت حرف *name;
			إذا (opt & OUTPUT_SHOW_EMAIL)
				name = ci.author_mail.buf;
			وإلا
				name = ci.author.buf;
			لکو("\t(%10s\t%10s\t%d)", name,
			       format_وقت(ci.author_وقت, ci.author_tz.buf,
					   show_raw_وقت),
			       ent->lno + 1 + cnt);
		} وإلا {
			إذا (opt & OUTPUT_SHOW_SCORE)
				لکو(" %*d %02d",
				       max_score_digits, ent->score,
				       ent->suspect->refcnt);
			إذا (opt & OUTPUT_SHOW_NAME)
				لکو(" %-*.*s", longest_file, longest_file,
				       suspect->رستو);
			إذا (opt & OUTPUT_SHOW_عدد)
				لکو(" %*d", max_orig_digits,
				       ent->s_lno + 1 + cnt);

			إذا (!(opt & OUTPUT_NO_AUTHOR)) {
				ثابت حرف *name;
				صحيح pad;
				إذا (opt & OUTPUT_SHOW_EMAIL)
					name = ci.author_mail.buf;
				وإلا
					name = ci.author.buf;
				pad = longest_author - utf8_strwidth(name);
				لکو(" (%s%*s %10s",
				       name, pad, "",
				       format_وقت(ci.author_وقت,
						   ci.author_tz.buf,
						   show_raw_وقت));
			}
			لکو(" %*d) ",
			       max_digits, ent->lno + 1 + cnt);
		}
		إذا (reset)
			fputs(reset, stdout);
		افعل {
			ch = *cp++;
			اکر_لکو(ch);
		} طالما (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_سائيز);
	}

	إذا (sb->final_buf_سائيز && cp[-1] != '\n')
		اکر_لکو('\n');

	commit_info_destroy(&ci);
}

ساكن فراغ output(بنية blame_scoreboard *sb, صحيح option)
{
	بنية blame_داخلا *ent, *prev_ent = عدم;

	إذا (option & OUTPUT_PORCELAIN) {
		لكل (ent = sb->ent; ent; ent = ent->ايندڙ) {
			صحيح ڳڻپ = 0;
			بنية blame_origin *suspect;
			بنية commit *commit = ent->suspect->commit;
			إذا (commit->object.flags & وڌيڪ_THAN_ONE_رستو)
				استمر;
			لكل (suspect = get_blame_suspects(commit); suspect; suspect = suspect->ايندڙ) {
				إذا (suspect->guilty && ڳڻپ++) {
					commit->object.flags |= وڌيڪ_THAN_ONE_رستو;
					اكسر;
				}
			}
		}
	}

	لكل (ent = sb->ent; ent; ent = ent->ايندڙ) {
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
		بنية commit_graft *graft = read_graft_سٽ(&buf);
		إذا (graft)
			رجسٽر_commit_graft(the_repository, graft, 0);
	}
	fclose(fp);
	strbuf_release(&buf);
	أرجع 0;
}

ساكن صحيح update_auto_abbrev(صحيح auto_abbrev, بنية blame_origin *suspect)
{
	ثابت حرف *uniq = repo_ڳوليو_unique_abbrev(the_repository,
						   &suspect->commit->object.oid,
						   auto_abbrev);
	صحيح len = لڙي_ڊيگهه(uniq);
	إذا (auto_abbrev < len)
		أرجع len;
	أرجع auto_abbrev;
}

/*
 * How many columns do we need to show line numbers, authors,
 * and filenames?
 */
ساكن فراغ ڳوليو_alignment(بنية blame_scoreboard *sb, صحيح *option)
{
	صحيح longest_src_سٽون = 0;
	صحيح longest_dst_سٽون = 0;
	بنانشان سڀ_کان_وڏو_score = 0;
	بنية blame_داخلا *e;
	صحيح compute_auto_abbrev = (abbrev < 0);
	صحيح auto_abbrev = DEFAULT_ABBREV;
	سائيز_t max_marks_ڳڻپ = 0;

	لكل (e = sb->ent; e; e = e->ايندڙ) {
		بنية blame_origin *suspect = e->suspect;
		صحيح انگ;
		سائيز_t marks_ڳڻپ = ڳڻپ_marks(e, *option);

		إذا (max_marks_ڳڻپ < marks_ڳڻپ)
			max_marks_ڳڻپ = marks_ڳڻپ;
		إذا (compute_auto_abbrev)
			auto_abbrev = update_auto_abbrev(auto_abbrev, suspect);
		إذا (strcmp(suspect->رستو, sb->رستو))
			*option |= OUTPUT_SHOW_NAME;
		انگ = لڙي_ڊيگهه(suspect->رستو);
		إذا (longest_file < انگ)
			longest_file = انگ;
		إذا (!(suspect->commit->object.flags & METAINFO_SHOWN)) {
			بنية commit_info ci = COMMIT_INFO_INIT;
			suspect->commit->object.flags |= METAINFO_SHOWN;
			get_commit_info(suspect->commit, &ci);
			إذا (*option & OUTPUT_SHOW_EMAIL)
				انگ = utf8_strwidth(ci.author_mail.buf);
			وإلا
				انگ = utf8_strwidth(ci.author.buf);
			إذا (longest_author < انگ)
				longest_author = انگ;
			commit_info_destroy(&ci);
		}
		انگ = e->s_lno + e->انگ_سٽون;
		إذا (longest_src_سٽون < انگ)
			longest_src_سٽون = انگ;
		انگ = e->lno + e->انگ_سٽون;
		إذا (longest_dst_سٽون < انگ)
			longest_dst_سٽون = انگ;
		إذا (سڀ_کان_وڏو_score < blame_داخلا_score(sb, e))
			سڀ_کان_وڏو_score = blame_داخلا_score(sb, e);
	}
	max_orig_digits = decimal_ويڪر(longest_src_سٽون);
	max_digits = decimal_ويڪر(longest_dst_سٽون);
	max_score_digits = decimal_ويڪر(سڀ_کان_وڏو_score);

	إذا (compute_auto_abbrev)
		abbrev = auto_abbrev;
	إذا (abbrev < (صحيح)the_hash_algo->hexsz) {
		abbrev += max_marks_ڳڻپ;
		إذا (abbrev > (صحيح)the_hash_algo->hexsz)
			abbrev = the_hash_algo->hexsz;
	}
}

ساكن فراغ sanity_check_on_fail(بنية blame_scoreboard *sb, صحيح baa)
{
	صحيح opt = OUTPUT_SHOW_SCORE | OUTPUT_SHOW_عدد | OUTPUT_SHOW_NAME;
	ڳوليو_alignment(sb, &opt);
	output(sb, opt);
	die("Baa %d!", baa);
}

ساكن بنانشان parse_score(ثابت حرف *arg)
{
	حرف *پڇاڙي;
	بنانشان ڊگهو score = strtoul(arg, &پڇاڙي, 10);
	إذا (*پڇاڙي)
		أرجع 0;
	أرجع score;
}

ساكن حرف *جوڙيو_اڳياڙي(ثابت حرف *اڳياڙي, ثابت حرف *رستو)
{
	أرجع اڳياڙي_رستو(the_repository, اڳياڙي, اڳياڙي ? لڙي_ڊيگهه(اڳياڙي) : 0, رستو);
}

ساكن صحيح git_blame_config(ثابت حرف *var, ثابت حرف *قدر,
			    ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, "blame.showroot")) {
		show_پاڙ = git_config_bool(var, قدر);
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
		parse_date_format(قدر, &blame_date_گهڻائي);
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.ignorerevsfile")) {
		حرف *str;
		صحيح ret;

		ret = git_config_pathname(&str, var, قدر);
		إذا (ret)
			أرجع ret;
		إذا (str)
			string_list_وجهو(&ignore_revs_file_list, str);
		حرر(str);
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.markunblamablelines")) {
		mark_unblamable_سٽون = git_config_bool(var, قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "blame.markignoredlines")) {
		mark_ignored_سٽون = git_config_bool(var, قدر);
		أرجع 0;
	}
	إذا (!strcmp(var, "color.blame.repeatedlines")) {
		إذا (color_parse_mem(قدر, لڙي_ڊيگهه(قدر), repeated_meta_color))
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
			coloring_گهڻائي |= OUTPUT_COLOR_سٽ;
		} وإلا إذا (!strcmp(قدر, "highlightRecent")) {
			coloring_گهڻائي |= OUTPUT_SHOW_AGE_WITH_COLOR;
		} وإلا إذا (!strcmp(قدر, "none")) {
			coloring_گهڻائي &= ~(OUTPUT_COLOR_سٽ |
					    OUTPUT_SHOW_AGE_WITH_COLOR);
		} وإلا {
			warning(_("invalid value for '%s': '%s'"),
				"blame.coloring", قدر);
			أرجع 0;
		}
	}

	إذا (!strcmp(var, "diff.algorithm")) {
		ڊگهو diff_algorithm;
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

ساكن صحيح blame_diff_algorithm_گهٽتر(ثابت بنية option *option,
					ثابت حرف *arg, صحيح unset)
{
	صحيح *opt = option->قدر;

	BUG_ON_OPT_ARG(arg);

	*opt &= ~XDF_DIFF_ALGORITHM_نقاب;
	إذا (!unset)
		*opt |= XDF_NEED_گهٽتر;

	أرجع 0;
}

ساكن صحيح blame_diff_algorithm_callback(ثابت بنية option *option,
					 ثابت حرف *arg, صحيح unset)
{
	صحيح *opt = option->قدر;
	ڊگهو قدر = parse_algorithm_قدر(arg);

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
		إذا (قسم != OBJ_ٽيگ)
			أرجع -1;
		obj = deref_ٽيگ(r, parse_object(r, &oid), عدم, 0);
		إذا (!obj)
			أرجع -1;
		oidcpy(&oid, &obj->oid);
	}
}

ساكن فراغ build_ignorelist(بنية blame_scoreboard *sb,
			     بنية string_list *ignore_revs_file_list,
			     بنية string_list *ignore_rev_list)
{
	بنية string_list_شي *i;
	بنية object_id oid;

	oidset_init(&sb->ignore_list, 0);
	for_each_string_list_شي(i, ignore_revs_file_list) {
		إذا (!strcmp(i->string, ""))
			oidset_clear(&sb->ignore_list);
		وإلا
			oidset_parse_file_carefully(&sb->ignore_list, i->string,
						    the_repository->hash_algo,
						    peel_منزل_commit_oid, sb);
	}
	for_each_string_list_شي(i, ignore_rev_list) {
		إذا (repo_get_oid_committish(the_repository, i->string, &oid) ||
		    peel_منزل_commit_oid(&oid, sb))
			die(_("cannot find revision %s to ignore"), i->string);
		oidset_وجهو(&sb->ignore_list, &oid);
	}
}

صحيح cmd_blame(صحيح argc,
	      ثابت حرف **argv,
	      ثابت حرف *اڳياڙي,
	      بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية rev_info revs;
	حرف *رستو = عدم;
	بنية blame_scoreboard sb;
	بنية blame_origin *o;
	بنية blame_داخلا *ent = عدم;
	ڊگهو dashdash_pos, lno;
	بنية ترقي_info pi = { عدم, 0 };

	بنية string_list range_list = STRING_LIST_INIT_NODUP;
	بنية string_list ignore_rev_list = STRING_LIST_INIT_NODUP;
	صحيح output_option = 0, opt = 0;
	صحيح show_stats = 0;
	ثابت حرف *revs_file = عدم;
	ثابت حرف *contents_from = عدم;
	ثابت بنية option options[] = {
		OPT_BOOL(0, "incremental", &incremental, N_("show blame entries as we find them, incrementally")),
		OPT_BOOL('b', عدم, &blank_boundary, N_("do not show object names of boundary commits (Default: off)")),
		OPT_BOOL(0, "root", &show_پاڙ, N_("do not treat root commits as boundaries (Default: off)")),
		OPT_BOOL(0, "show-stats", &show_stats, N_("show work cost statistics")),
		OPT_BOOL(0, "progress", &show_ترقي, N_("force progress reporting")),
		OPT_ٻٽو_انگ(0, "score-debug", &output_option, N_("show output score for blame entries"), OUTPUT_SHOW_SCORE),
		OPT_ٻٽو_انگ('f', "show-name", &output_option, N_("show original filename (Default: auto)"), OUTPUT_SHOW_NAME),
		OPT_ٻٽو_انگ('n', "show-number", &output_option, N_("show original linenumber (Default: off)"), OUTPUT_SHOW_عدد),
		OPT_ٻٽو_انگ('p', "porcelain", &output_option, N_("show in a format designed for machine consumption"), OUTPUT_PORCELAIN),
		OPT_ٻٽو_انگ(0, "line-porcelain", &output_option, N_("show porcelain format with per-line commit information"), OUTPUT_PORCELAIN|OUTPUT_سٽ_PORCELAIN),
		OPT_ٻٽو_انگ('c', عدم, &output_option, N_("use the same output mode as git-annotate (Default: off)"), OUTPUT_ANNOTATE_COMPAT),
		OPT_ٻٽو_انگ('t', عدم, &output_option, N_("show raw timestamp (Default: off)"), OUTPUT_RAW_TIMESTAMP),
		OPT_ٻٽو_انگ('l', عدم, &output_option, N_("show long commit SHA1 (Default: off)"), OUTPUT_LONG_OBJECT_NAME),
		OPT_ٻٽو_انگ('s', عدم, &output_option, N_("suppress author name and timestamp (Default: off)"), OUTPUT_NO_AUTHOR),
		OPT_ٻٽو_انگ('e', "show-email", &output_option, N_("show author email instead of name (Default: off)"), OUTPUT_SHOW_EMAIL),
		OPT_ٻٽو_انگ('w', عدم, &xdl_opts, N_("ignore whitespace differences"), XDF_IGNORE_WHITESPACE),
		OPT_CALLBACK_F(0, "diff-algorithm", &xdl_opts, N_("<algorithm>"),
			       N_("choose a diff algorithm"),
			       PARSE_OPT_NONEG, blame_diff_algorithm_callback),
		OPT_STRING_LIST(0, "ignore-rev", &ignore_rev_list, N_("rev"), N_("ignore <rev> when blaming")),
		OPT_STRING_LIST(0, "ignore-revs-file", &ignore_revs_file_list, N_("file"), N_("ignore revisions from <file>")),
		OPT_ٻٽو_انگ(0, "color-lines", &output_option, N_("color redundant metadata from previous line differently"), OUTPUT_COLOR_سٽ),
		OPT_ٻٽو_انگ(0, "color-by-age", &output_option, N_("color lines by age"), OUTPUT_SHOW_AGE_WITH_COLOR),
		OPT_CALLBACK_F(0, "minimal", &xdl_opts, عدم,
			       N_("spend extra cycles to find a better match"),
			       PARSE_OPT_NOARG | PARSE_OPT_HIDDEN, blame_diff_algorithm_گهٽتر),
		OPT_STRING('S', عدم, &revs_file, N_("file"), N_("use revisions from <file> instead of calling git-rev-list")),
		OPT_STRING(0, "contents", &contents_from, N_("file"), N_("use <file>'s contents as the final image")),
		OPT_CALLBACK_F('C', عدم, &opt, N_("score"), N_("find line copies within and across files"), PARSE_OPT_OPTARG, blame_copy_callback),
		OPT_CALLBACK_F('M', عدم, &opt, N_("score"), N_("find line movements within and across files"), PARSE_OPT_OPTARG, blame_move_callback),
		OPT_STRING_LIST('L', عدم, &range_list, N_("range"),
				N_("process only line range <start>,<end> or function :<funcname>")),
		OPT__ABBREV(&abbrev),
		OPT_پڇاڙي()
	};

	بنية parse_opt_ctx_t ctx;
	صحيح cmd_is_annotate = !strcmp(argv[0], "annotate");
	بنية range_سيٽ ranges;
	بنانشان صحيح range_i;
	ڊگهو anchor;
	ڊگهو انگ_سٽون = 0;
	ثابت حرف *str_usage = cmd_is_annotate ? annotate_usage : blame_usage;
	ثابت حرف *ثابت *opt_usage = cmd_is_annotate ? annotate_opt_usage : blame_opt_usage;

	تياري_default_color_by_age();
	repo_config(the_repository, git_blame_config, &output_option);
	repo_init_revisions(the_repository, &revs, عدم);
	revs.date_گهڻائي = blame_date_گهڻائي;
	revs.diffopt.flags.allow_textconv = 1;
	revs.diffopt.flags.follow_renames = 1;

	save_commit_بفر = 0;
	dashdash_pos = 0;
	show_ترقي = -1;

	parse_options_شروعات(&ctx, argc, argv, اڳياڙي, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);
	لكل (;;) {
		بدل (parse_options_قدم(&ctx, options, opt_usage)) {
		حالة PARSE_OPT_NON_OPTION:
		حالة PARSE_OPT_UNKNOWN:
			اكسر;
		حالة PARSE_OPT_HELP:
			ٻاهر_نڪرو(0);
		حالة PARSE_OPT_HELP_ERROR:
		حالة PARSE_OPT_ERROR:
		حالة PARSE_OPT_SUBCOMMAND:
			ٻاهر_نڪرو(129);
		حالة PARSE_OPT_COMPLETE:
			ٻاهر_نڪرو(0);
		حالة PARSE_OPT_پورو:
			إذا (ctx.argv[0])
				dashdash_pos = ctx.cpidx;
			اذهب parse_پورو;
		}

		إذا (!strcmp(ctx.argv[0], "--reverse")) {
			ctx.argv[0] = "--children";
			ابتو = 1;
		}
		parse_revision_opt(&revs, &ctx, options, opt_usage);
	}
parse_پورو:
	revision_opts_پڄاڻي(&revs);
	no_سڄو_file_rename = !revs.diffopt.flags.follow_renames;
	xdl_opts |= revs.diffopt.xdl_opts & XDF_INDENT_HEURISTIC;
	revs.diffopt.flags.follow_renames = 0;
	argc = parse_options_پڇاڙي(&ctx);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ڀريل_اشاريو = 0;

	إذا (incremental || (output_option & OUTPUT_PORCELAIN)) {
		إذا (show_ترقي > 0)
			die(_("--progress can't be used with --incremental or porcelain formats"));
		show_ترقي = 0;
	} وإلا إذا (show_ترقي < 0)
		show_ترقي = isatty(2);

	إذا (!abbrev)
		abbrev = the_hash_algo->hexsz;

	إذا (revs_file && read_ancestry(revs_file))
		die_errno("reading graft file '%s' failed", revs_file);

	إذا (cmd_is_annotate) {
		output_option |= OUTPUT_ANNOTATE_COMPAT;
		blame_date_گهڻائي.type = DATE_ISO8601;
	} وإلا {
		blame_date_گهڻائي = revs.date_گهڻائي;
	}

	/* The maximum width used to show the dates */
	بدل (blame_date_گهڻائي.type) {
	حالة DATE_RFC2822:
		blame_date_ويڪر = حجم("Thu, 19 Oct 2006 16:00:04 -0700");
		اكسر;
	حالة DATE_ISO8601_STRICT:
		blame_date_ويڪر = حجم("2006-10-19T16:00:04-07:00");
		اكسر;
	حالة DATE_ISO8601:
		blame_date_ويڪر = حجم("2006-10-19 16:00:04 -0700");
		اكسر;
	حالة DATE_RAW:
		blame_date_ويڪر = حجم("1161298804 -0700");
		اكسر;
	حالة DATE_UNIX:
		blame_date_ويڪر = حجم("1161298804");
		اكسر;
	حالة DATE_SHORT:
		blame_date_ويڪر = حجم("2006-10-19");
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
		blame_date_ويڪر = utf8_strwidth(_("4 years, 11 months ago")) + 1; /* add the null */
		اكسر;
	حالة DATE_HUMAN:
		/* If the year is shown, no time is shown */
		blame_date_ويڪر = حجم("Thu Oct 19 16:00");
		اكسر;
	حالة DATE_عام:
		blame_date_ويڪر = حجم("Thu Oct 19 16:00:04 2006 -0700");
		اكسر;
	حالة DATE_STRFTIME:
		blame_date_ويڪر = لڙي_ڊيگهه(show_date(0, 0, blame_date_گهڻائي)) + 1; /* add the null */
		اكسر;
	}
	blame_date_ويڪر -= 1; /* strip the null */

	إذا (revs.diffopt.flags.ڳوليو_copies_harder)
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
			رستو = جوڙيو_اڳياڙي(اڳياڙي, argv[--argc]);
			argv[argc] = عدم;
			اكسر;
		افتراضي:
			usage_with_options(opt_usage, options);
		}
	} وإلا {
		إذا (argc < 2)
			usage_with_options(opt_usage, options);
		إذا (argc == 3 && is_a_rev(argv[argc - 1])) { /* (2b) */
			رستو = جوڙيو_اڳياڙي(اڳياڙي, argv[1]);
			argv[1] = argv[2];
		} وإلا {	/* (2a) */
			إذا (argc == 2 && is_a_rev(argv[1]) && !repo_get_work_وڻ(the_repository))
				die("missing <path> to blame");
			رستو = جوڙيو_اڳياڙي(اڳياڙي, argv[argc - 1]);
		}
		argv[argc - 1] = "--";
	}

	revs.disable_stdin = 1;
	تياري_revisions(argc, argv, &revs, عدم);
	إذا (!revs.pending.nr && is_bare_repository(the_repository)) {
		بنية commit *سر_commit;
		بنية object_id سر_oid;

		إذا (!refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", RESOLVE_REF_READING,
					     &سر_oid, عدم) ||
		    !(سر_commit = lookup_commit_reference_gently(revs.repo,
							     &سر_oid, 1)))
			die("no such ref: HEAD");

		جوڙيو_pending_object(&revs, &سر_commit->object, "HEAD");
	}

	init_scoreboard(&sb);
	sb.revs = &revs;
	sb.contents_from = contents_from;
	sb.ابتو = ابتو;
	sb.repo = the_repository;
	sb.رستو = رستو;
	build_ignorelist(&sb, &ignore_revs_file_list, &ignore_rev_list);
	string_list_clear(&ignore_revs_file_list, 0);
	string_list_clear(&ignore_rev_list, 0);
	تياري_scoreboard(&sb, &o);

	/*
	 * Changed-path Bloom filters are disabled when looking
	 * for copies.
	 */
	إذا (!(opt & PICKAXE_BLAME_COPY))
		تياري_blame_bloom_معلومات(&sb);

	lno = sb.انگ_سٽون;

	إذا (lno && !range_list.nr)
		string_list_append(&range_list, "1");

	anchor = 1;
	range_سيٽ_init(&ranges, range_list.nr);
	لكل (range_i = 0; range_i < range_list.nr; ++range_i) {
		ڊگهو bottom, چوٽي;
		إذا (parse_range_arg(range_list.شيون[range_i].string,
				    nth_سٽ_cb, &sb, lno, anchor,
				    &bottom, &چوٽي, sb.رستو,
				    the_repository->اشاريو))
			usage(str_usage);
		إذا ((!lno && (چوٽي || bottom)) || lno < bottom)
			die(Q_("file %s has only %lu line",
			       "file %s has only %lu lines",
			       lno), sb.رستو, lno);
		إذا (bottom < 1)
			bottom = 1;
		إذا (چوٽي < 1 || lno < چوٽي)
			چوٽي = lno;
		bottom--;
		range_سيٽ_append_unsafe(&ranges, bottom, چوٽي);
		anchor = چوٽي + 1;
	}
	ترتيب_ڏيو_and_ملايو_range_سيٽ(&ranges);

	لكل (range_i = ranges.nr; range_i > 0; --range_i) {
		ثابت بنية range *r = &ranges.ranges[range_i - 1];
		ent = blame_داخلا_prepend(ent, r->شروعات, r->پڇاڙي, o);
		انگ_سٽون += (r->پڇاڙي - r->شروعات);
	}
	إذا (!انگ_سٽون)
		انگ_سٽون = sb.انگ_سٽون;

	o->suspects = ent;
	prio_قطار_put(&sb.commits, o->commit);

	blame_origin_decref(o);

	range_سيٽ_release(&ranges);
	string_list_clear(&range_list, 0);

	sb.ent = عدم;

	إذا (blame_move_score)
		sb.move_score = blame_move_score;
	إذا (blame_copy_score)
		sb.copy_score = blame_copy_score;

	sb.debug = DEBUG_BLAME;
	sb.on_sanity_fail = &sanity_check_on_fail;

	sb.show_پاڙ = show_پاڙ;
	sb.xdl_opts = xdl_opts;
	sb.no_سڄو_file_rename = no_سڄو_file_rename;

	read_mailmap(the_repository, &mailmap);

	sb.مليو_guilty_داخلا = &مليو_guilty_داخلا;
	sb.مليو_guilty_داخلا_معلومات = &pi;
	إذا (show_ترقي)
		pi.ترقي = شروعات_delayed_ترقي(the_repository,
						     _("Blaming lines"),
						     انگ_سٽون);

	سونپيو_blame(&sb, opt);

	stop_ترقي(&pi.ترقي);

	إذا (!incremental)
		تياري_pager(the_repository);
	وإلا
		اذهب cleanup;

	blame_ترتيب_ڏيو_final(&sb);

	blame_coalesce(&sb);

	إذا (!(output_option & (OUTPUT_COLOR_سٽ | OUTPUT_SHOW_AGE_WITH_COLOR)))
		output_option |= coloring_گهڻائي;

	إذا (!(output_option & OUTPUT_PORCELAIN)) {
		ڳوليو_alignment(&sb, &output_option);
		إذا (!*repeated_meta_color &&
		    (output_option & OUTPUT_COLOR_سٽ))
			xsnprintf(repeated_meta_color,
				  حجم(repeated_meta_color),
				  "%s", GIT_COLOR_CYAN);
	}
	إذا (output_option & OUTPUT_ANNOTATE_COMPAT)
		output_option &= ~(OUTPUT_COLOR_سٽ | OUTPUT_SHOW_AGE_WITH_COLOR);

	output(&sb, output_option);

	إذا (show_stats) {
		لکو("num read blob: %d\n", sb.انگ_read_blob);
		لکو("num get patch: %d\n", sb.انگ_get_patch);
		لکو("num commits: %d\n", sb.انگ_commits);
	}

cleanup:
	لكل (ent = sb.ent; ent; ) {
		بنية blame_داخلا *e = ent->ايندڙ;
		حرر(ent);
		ent = e;
	}

	حرر(رستو);
	cleanup_scoreboard(&sb);
	release_revisions(&revs);
	أرجع 0;
}
