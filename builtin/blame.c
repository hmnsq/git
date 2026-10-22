/*
 * Blame
 *
 * Copyright (c) 2006, 2014 by its authors
 * See COPYING for licensing conditions
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "color.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "mailmap.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "prio-queue.h"
#అన్తర్భావ "utf8.h"
#అన్తర్భావ "userdiff.h"
#అన్తర్భావ "line-range.h"
#అన్తర్భావ "line-log.h"
#అన్తర్భావ "progress.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "pager.h"
#అన్తర్భావ "blame.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "tag.h"
#అన్తర్భావ "write-or-die.h"

స్థిర స్థిరరాశి అక్షరం blame_usage[] = N_("git blame [<options>] [<rev-opts>] [<rev>] [--] <file>");
స్థిర స్థిరరాశి అక్షరం annotate_usage[] = N_("git annotate [<options>] [<rev-opts>] [<rev>] [--] <file>");

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి blame_opt_usage[] = {
	blame_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి annotate_opt_usage[] = {
	annotate_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	ఖాళీ
};

స్థిర పూర్ణాంకం longest_file;
స్థిర పూర్ణాంకం longest_author;
స్థిర పూర్ణాంకం max_orig_digits;
స్థిర పూర్ణాంకం max_digits;
స్థిర పూర్ణాంకం max_score_digits;
స్థిర పూర్ణాంకం show_మూలం;
స్థిర పూర్ణాంకం తిరగేయి;
స్థిర పూర్ణాంకం blank_boundary;
స్థిర పూర్ణాంకం incremental;
స్థిర పూర్ణాంకం xdl_opts;
స్థిర పూర్ణాంకం abbrev = -1;
స్థిర పూర్ణాంకం no_సంపూర్ణ_file_rename;
స్థిర పూర్ణాంకం show_పురోగతి;
స్థిర అక్షరం repeated_meta_color[COLOR_MAXLEN];
స్థిర పూర్ణాంకం coloring_బహుళకం;
స్థిర నిర్మాణం string_list ignore_revs_file_list = STRING_LIST_INIT_DUP;
స్థిర పూర్ణాంకం mark_unblamable_పంక్తులు;
స్థిర పూర్ణాంకం mark_ignored_పంక్తులు;

స్థిర నిర్మాణం date_బహుళకం blame_date_బహుళకం = { DATE_ISO8601 };
స్థిర కొలత_t blame_date_వెడల్పు;

స్థిర నిర్మాణం string_list mailmap = STRING_LIST_INIT_NODUP;

#నిర్వచితం_కాకపోతే DEBUG_BLAME
#నిర్వచించు DEBUG_BLAME 0
#అయితే_ముగింపు

స్థిర గుర్తులేని blame_move_score;
స్థిర గుర్తులేని blame_copy_score;

/* Remember to update object flag allocation in object.h */
#నిర్వచించు METAINFO_SHOWN		(1u<<12)
#నిర్వచించు మరిన్ని_THAN_ONE_మార్గం	(1u<<13)

నిర్మాణం పురోగతి_info {
	నిర్మాణం పురోగతి *పురోగతి;
	పూర్ణాంకం blamed_పంక్తులు;
};

స్థిర స్థిరరాశి అక్షరం *nth_పంక్తి_cb(శూన్య *దత్తాంశం, దీర్ఘ lno)
{
	తిరిగి blame_nth_పంక్తి((నిర్మాణం blame_scoreboard *)దత్తాంశం, lno);
}

/*
 * Information on commits, used for output.
 */
నిర్మాణం commit_info {
	నిర్మాణం strbuf author;
	నిర్మాణం strbuf author_mail;
	timestamp_t author_సమయం;
	నిర్మాణం strbuf author_tz;

	/* filled only when asked for details */
	నిర్మాణం strbuf committer;
	నిర్మాణం strbuf committer_mail;
	timestamp_t committer_సమయం;
	నిర్మాణం strbuf committer_tz;

	నిర్మాణం strbuf summary;
};

#నిర్వచించు COMMIT_INFO_INIT { \
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
స్థిర శూన్య get_ac_పంక్తి(స్థిరరాశి అక్షరం *inbuf, స్థిరరాశి అక్షరం *what,
	నిర్మాణం strbuf *name, నిర్మాణం strbuf *mail,
	timestamp_t *సమయం, నిర్మాణం strbuf *tz)
{
	నిర్మాణం ident_విభజించు ident;
	కొలత_t len, maillen, namelen;
	స్థిరరాశి అక్షరం *tmp, *endp;
	స్థిరరాశి అక్షరం *namebuf, *mailbuf;

	tmp = అక్షరశ్రేణి_శోధన(inbuf, what);
	ఒకవేళ (!tmp)
		వెళ్ళు error_బహిర్గతం;
	tmp += అక్షరశ్రేణి_పొడవు(what);
	endp = strchr(tmp, '\n');
	ఒకవేళ (!endp)
		len = అక్షరశ్రేణి_పొడవు(tmp);
	లేకపోతే
		len = endp - tmp;

	ఒకవేళ (విభజించు_ident_పంక్తి(&ident, tmp, len)) {
	error_బహిర్గతం:
		/* Ugh */
		tmp = "(unknown)";
		strbuf_addstr(name, tmp);
		strbuf_addstr(mail, tmp);
		strbuf_addstr(tz, tmp);
		*సమయం = 0;
		తిరిగి;
	}

	namelen = ident.name_చివర - ident.name_begin;
	namebuf = ident.name_begin;

	maillen = ident.mail_చివర - ident.mail_begin;
	mailbuf = ident.mail_begin;

	ఒకవేళ (ident.date_begin && ident.date_చివర)
		*సమయం = strtoul(ident.date_begin, ఖాళీ, 10);
	లేకపోతే
		*సమయం = 0;

	ఒకవేళ (ident.tz_begin && ident.tz_చివర)
		strbuf_కూడు(tz, ident.tz_begin, ident.tz_చివర - ident.tz_begin);
	లేకపోతే
		strbuf_addstr(tz, "(unknown)");

	/*
	 * Now, convert both name and e-mail using mailmap
	 */
	map_user(&mailmap, &mailbuf, &maillen,
		 &namebuf, &namelen);

	strbuf_addf(mail, "<%.*s>", (పూర్ణాంకం)maillen, mailbuf);
	strbuf_కూడు(name, namebuf, namelen);
}

స్థిర శూన్య commit_info_destroy(నిర్మాణం commit_info *ci)
{

	strbuf_release(&ci->author);
	strbuf_release(&ci->author_mail);
	strbuf_release(&ci->author_tz);
	strbuf_release(&ci->committer);
	strbuf_release(&ci->committer_mail);
	strbuf_release(&ci->committer_tz);
	strbuf_release(&ci->summary);
}

స్థిర శూన్య get_commit_info(నిర్మాణం commit *commit, నిర్మాణం commit_info *ret)
{
	పూర్ణాంకం len;
	స్థిరరాశి అక్షరం *subject, *encoding;
	స్థిరరాశి అక్షరం *సందేశం_2;

	encoding = get_log_output_encoding();
	సందేశం_2 = repo_logmsg_reencode(the_repository, commit, ఖాళీ, encoding);
	get_ac_పంక్తి(సందేశం_2, "\nauthor ",
		    &ret->author, &ret->author_mail,
		    &ret->author_సమయం, &ret->author_tz);

	get_ac_పంక్తి(సందేశం_2, "\ncommitter ",
		    &ret->committer, &ret->committer_mail,
		    &ret->committer_సమయం, &ret->committer_tz);

	len = కనుగొను_commit_subject(సందేశం_2, &subject);
	ఒకవేళ (len)
		strbuf_కూడు(&ret->summary, subject, len);
	లేకపోతే
		strbuf_addf(&ret->summary, "(%s)", oid_గమ్యం_hex(&commit->object.oid));

	repo_unuse_commit_బఫరు(the_repository, commit, సందేశం_2);
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
స్థిర శూన్య write_filename_info(నిర్మాణం blame_origin *suspect)
{
	ఒకవేళ (suspect->మునుపటి) {
		నిర్మాణం blame_origin *prev = suspect->మునుపటి;
		రాయి("previous %s ", oid_గమ్యం_hex(&prev->commit->object.oid));
		write_name_quoted(prev->మార్గం, stdout, '\n');
	}
	రాయి("filename ");
	write_name_quoted(suspect->మార్గం, stdout, '\n');
}

/*
 * Porcelain/Incremental format wants to show a lot of details per
 * commit.  Instead of repeating this every line, emit it only once,
 * the first time each commit appears in the output (unless the
 * user has specifically asked for us to repeat).
 */
స్థిర పూర్ణాంకం emit_one_suspect_detail(నిర్మాణం blame_origin *suspect, పూర్ణాంకం repeat)
{
	నిర్మాణం commit_info ci = COMMIT_INFO_INIT;

	ఒకవేళ (!repeat && (suspect->commit->object.flags & METAINFO_SHOWN))
		తిరిగి 0;

	suspect->commit->object.flags |= METAINFO_SHOWN;
	get_commit_info(suspect->commit, &ci);
	రాయి("author %s\n", ci.author.buf);
	రాయి("author-mail %s\n", ci.author_mail.buf);
	రాయి("author-time %"PRItime"\n", ci.author_సమయం);
	రాయి("author-tz %s\n", ci.author_tz.buf);
	రాయి("committer %s\n", ci.committer.buf);
	రాయి("committer-mail %s\n", ci.committer_mail.buf);
	రాయి("committer-time %"PRItime"\n", ci.committer_సమయం);
	రాయి("committer-tz %s\n", ci.committer_tz.buf);
	రాయి("summary %s\n", ci.summary.buf);
	ఒకవేళ (suspect->commit->object.flags & UNINTERESTING)
		రాయి("boundary\n");

	commit_info_destroy(&ci);

	తిరిగి 1;
}

/*
 * The blame_entry is found to be guilty for the range.
 * Show it in incremental output.
 */
స్థిర శూన్య దొరికింది_guilty_నమోదు(నిర్మాణం blame_నమోదు *ent, శూన్య *దత్తాంశం)
{
	నిర్మాణం పురోగతి_info *pi = (నిర్మాణం పురోగతి_info *)దత్తాంశం;

	ఒకవేళ (incremental) {
		నిర్మాణం blame_origin *suspect = ent->suspect;

		రాయి("%s %d %d %d\n",
		       oid_గమ్యం_hex(&suspect->commit->object.oid),
		       ent->s_lno + 1, ent->lno + 1, ent->అంకె_పంక్తులు);
		emit_one_suspect_detail(suspect, 0);
		write_filename_info(suspect);
		maybe_flush_or_die(stdout, "stdout");
	}
	pi->blamed_పంక్తులు += ent->అంకె_పంక్తులు;
	display_పురోగతి(pi->పురోగతి, pi->blamed_పంక్తులు);
}

స్థిర స్థిరరాశి అక్షరం *format_సమయం(timestamp_t సమయం, స్థిరరాశి అక్షరం *tz_str,
			       పూర్ణాంకం show_raw_సమయం)
{
	స్థిర నిర్మాణం strbuf సమయం_buf = STRBUF_INIT;

	strbuf_reset(&సమయం_buf);
	ఒకవేళ (show_raw_సమయం) {
		strbuf_addf(&సమయం_buf, "%"PRItime" %s", సమయం, tz_str);
	}
	లేకపోతే {
		స్థిరరాశి అక్షరం *సమయం_str;
		కొలత_t సమయం_వెడల్పు;
		పూర్ణాంకం tz;
		tz = atoi(tz_str);
		సమయం_str = show_date(సమయం, tz, blame_date_బహుళకం);
		strbuf_addstr(&సమయం_buf, సమయం_str);
		/*
		 * Add space paddings to time_buf to display a fixed width
		 * string, and use time_width for display width calibration.
		 */
		కొరకు (సమయం_వెడల్పు = utf8_strwidth(సమయం_str);
		     సమయం_వెడల్పు < blame_date_వెడల్పు;
		     సమయం_వెడల్పు++)
			strbuf_addch(&సమయం_buf, ' ');
	}
	తిరిగి సమయం_buf.buf;
}

#నిర్వచించు OUTPUT_ANNOTATE_COMPAT      (1U<<0)
#నిర్వచించు OUTPUT_LONG_OBJECT_NAME     (1U<<1)
#నిర్వచించు OUTPUT_RAW_TIMESTAMP        (1U<<2)
#నిర్వచించు OUTPUT_PORCELAIN            (1U<<3)
#నిర్వచించు OUTPUT_SHOW_NAME            (1U<<4)
#నిర్వచించు OUTPUT_SHOW_సంఖ్య          (1U<<5)
#నిర్వచించు OUTPUT_SHOW_SCORE           (1U<<6)
#నిర్వచించు OUTPUT_NO_AUTHOR            (1U<<7)
#నిర్వచించు OUTPUT_SHOW_EMAIL           (1U<<8)
#నిర్వచించు OUTPUT_పంక్తి_PORCELAIN       (1U<<9)
#నిర్వచించు OUTPUT_COLOR_పంక్తి           (1U<<10)
#నిర్వచించు OUTPUT_SHOW_AGE_WITH_COLOR  (1U<<11)

స్థిర శూన్య emit_porcelain_details(నిర్మాణం blame_origin *suspect, పూర్ణాంకం repeat)
{
	ఒకవేళ (emit_one_suspect_detail(suspect, repeat) ||
	    (suspect->commit->object.flags & మరిన్ని_THAN_ONE_మార్గం))
		write_filename_info(suspect);
}

/*
 * Information which needs to be printed per-line goes here. Any
 * information which can be clubbed on a commit/file level, should
 * be printed via 'emit_one_suspect_detail()'.
 */
స్థిర శూన్య emit_porcelain_చొప్పున_పంక్తి_details(నిర్మాణం blame_నమోదు *ent)
{
	ఒకవేళ (mark_unblamable_పంక్తులు && ent->unblamable)
		puts("unblamable");
	ఒకవేళ (mark_ignored_పంక్తులు && ent->ignored)
		puts("ignored");
}

స్థిర శూన్య emit_porcelain(నిర్మాణం blame_scoreboard *sb, నిర్మాణం blame_నమోదు *ent,
			   పూర్ణాంకం opt)
{
	పూర్ణాంకం repeat = opt & OUTPUT_పంక్తి_PORCELAIN;
	పూర్ణాంకం cnt;
	స్థిరరాశి అక్షరం *cp;
	నిర్మాణం blame_origin *suspect = ent->suspect;
	అక్షరం hex[GIT_MAX_HEXSZ + 1];

	oid_గమ్యం_hex_r(hex, &suspect->commit->object.oid);
	రాయి("%s %d %d %d\n",
	       hex,
	       ent->s_lno + 1,
	       ent->lno + 1,
	       ent->అంకె_పంక్తులు);
	emit_porcelain_details(suspect, repeat);
	emit_porcelain_చొప్పున_పంక్తి_details(ent);

	cp = blame_nth_పంక్తి(sb, ent->lno);
	కొరకు (cnt = 0; cnt < ent->అంకె_పంక్తులు; cnt++) {
		అక్షరం ch;
		ఒకవేళ (cnt) {
			రాయి("%s %d %d\n", hex,
			       ent->s_lno + 1 + cnt,
			       ent->lno + 1 + cnt);
			ఒకవేళ (repeat)
				emit_porcelain_details(suspect, 1);
			emit_porcelain_చొప్పున_పంక్తి_details(ent);
		}
		అక్షరం_రాయి('\t');
		చేయి {
			ch = *cp++;
			అక్షరం_రాయి(ch);
		} అయితే (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_కొలత);
	}

	ఒకవేళ (sb->final_buf_కొలత && cp[-1] != '\n')
		అక్షరం_రాయి('\n');
}

స్థిర నిర్మాణం color_క్షేత్రం {
	timestamp_t hop;
	అక్షరం నిలువు[COLOR_MAXLEN];
} *colorfield;
స్థిర పూర్ణాంకం colorfield_nr, colorfield_alloc;

స్థిర శూన్య parse_color_fields(స్థిరరాశి అక్షరం *s)
{
	నిర్మాణం string_list l = STRING_LIST_INIT_DUP;
	నిర్మాణం string_list_అంశం *అంశం;
	గణన { EXPECT_DATE, EXPECT_COLOR } తదుపరి = EXPECT_COLOR;

	colorfield_nr = 0;

	/* Ideally this would be stripped and split at the same time? */
	string_list_విభజించు(&l, s, ",", -1);
	ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);

	for_each_string_list_అంశం(అంశం, &l) {
		ఎంపిక (తదుపరి) {
		స్థితి EXPECT_DATE:
			colorfield[colorfield_nr].hop = approxidate(అంశం->string);
			తదుపరి = EXPECT_COLOR;
			colorfield_nr++;
			ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);
			ఆపు;
		స్థితి EXPECT_COLOR:
			ఒకవేళ (color_parse(అంశం->string, colorfield[colorfield_nr].నిలువు))
				die(_("expecting a color: %s"), అంశం->string);
			తదుపరి = EXPECT_DATE;
			ఆపు;
		}
	}

	ఒకవేళ (తదుపరి == EXPECT_COLOR)
		die(_("must end with a color"));

	colorfield[colorfield_nr].hop = సమయం_MAX;
	string_list_clear(&l, 0);
}

స్థిర శూన్య అమరిక_default_color_by_age(శూన్య)
{
	parse_color_fields("blue,12 month ago,white,1 month ago,red");
}

స్థిర శూన్య determine_పంక్తి_heat(నిర్మాణం commit_info *ci, స్థిరరాశి అక్షరం **dest_color)
{
	పూర్ణాంకం i = 0;

	అయితే (i < colorfield_nr && ci->author_సమయం > colorfield[i].hop)
		i++;

	*dest_color = colorfield[i].నిలువు;
}

స్థిర అంతఃవరుస పూర్ణాంకం maybe_putc(పూర్ణాంకం c, FILE *బహిర్గతం)
{
	తిరిగి బహిర్గతం ? putc(c, బహిర్గతం) : 0;
}

స్థిర కొలత_t print_marks(FILE *బహిర్గతం, స్థిరరాశి నిర్మాణం blame_నమోదు *ent, పూర్ణాంకం opt)
{
	కొలత_t len = 0;

	ఒకవేళ ((ent->suspect->commit->object.flags & UNINTERESTING) &&
	    !blank_boundary && !(opt & OUTPUT_ANNOTATE_COMPAT)) {
		maybe_putc('^', బహిర్గతం);
		len++;
	}
	ఒకవేళ (mark_unblamable_పంక్తులు && ent->unblamable) {
		maybe_putc('*', బహిర్గతం);
		len++;
	}
	ఒకవేళ (mark_ignored_పంక్తులు && ent->ignored) {
		maybe_putc('?', బహిర్గతం);
		len++;
	}
	తిరిగి len;
}

స్థిర కొలత_t లెక్క_marks(స్థిరరాశి నిర్మాణం blame_నమోదు *ent, పూర్ణాంకం opt)
{
	తిరిగి print_marks(ఖాళీ, ent, opt);
}

స్థిర శూన్య emit_other(నిర్మాణం blame_scoreboard *sb, నిర్మాణం blame_నమోదు *ent,
		       పూర్ణాంకం opt, నిర్మాణం blame_నమోదు *prev_ent)
{
	పూర్ణాంకం cnt;
	స్థిరరాశి అక్షరం *cp;
	నిర్మాణం blame_origin *suspect = ent->suspect;
	నిర్మాణం commit_info ci = COMMIT_INFO_INIT;
	అక్షరం hex[GIT_MAX_HEXSZ + 1];
	పూర్ణాంకం show_raw_సమయం = !!(opt & OUTPUT_RAW_TIMESTAMP);
	స్థిరరాశి అక్షరం *default_color = ఖాళీ, *color = ఖాళీ, *reset = ఖాళీ;

	get_commit_info(suspect->commit, &ci);
	oid_గమ్యం_hex_r(hex, &suspect->commit->object.oid);

	cp = blame_nth_పంక్తి(sb, ent->lno);

	ఒకవేళ (opt & OUTPUT_SHOW_AGE_WITH_COLOR) {
		determine_పంక్తి_heat(&ci, &default_color);
		color = default_color;
		reset = GIT_COLOR_RESET;
	}

	ఒకవేళ (abbrev < MINIMUM_ABBREV)
		BUG("abbreviation is smaller than minimum length: %d < %d",
		    abbrev, MINIMUM_ABBREV);

	కొరకు (cnt = 0; cnt < ent->అంకె_పంక్తులు; cnt++) {
		అక్షరం ch;
		కొలత_t నిడివి = (opt & OUTPUT_LONG_OBJECT_NAME) ?
			the_hash_algo->hexsz : (కొలత_t) abbrev;

		ఒకవేళ (opt & OUTPUT_COLOR_పంక్తి) {
			ఒకవేళ (cnt > 0 ||
			    (prev_ent &&
			     oideq(&suspect->commit->object.oid,
				   &prev_ent->suspect->commit->object.oid))) {
				color = repeated_meta_color;
				reset = GIT_COLOR_RESET;
			} లేకపోతే  {
				color = default_color ? default_color : ఖాళీ;
				reset = default_color ? GIT_COLOR_RESET : ఖాళీ;
			}
		}
		ఒకవేళ (color)
			fputs(color, stdout);

		ఒకవేళ ((suspect->commit->object.flags & UNINTERESTING) &&
		    blank_boundary)
			memset(hex, ' ', అక్షరశ్రేణి_పొడవు(hex));
		నిడివి -= print_marks(stdout, ent, opt);

		రాయి("%.*s", (పూర్ణాంకం)(నిడివి < GIT_MAX_HEXSZ ? నిడివి : GIT_MAX_HEXSZ), hex);
		ఒకవేళ (opt & OUTPUT_ANNOTATE_COMPAT) {
			స్థిరరాశి అక్షరం *name;
			ఒకవేళ (opt & OUTPUT_SHOW_EMAIL)
				name = ci.author_mail.buf;
			లేకపోతే
				name = ci.author.buf;
			రాయి("\t(%10s\t%10s\t%d)", name,
			       format_సమయం(ci.author_సమయం, ci.author_tz.buf,
					   show_raw_సమయం),
			       ent->lno + 1 + cnt);
		} లేకపోతే {
			ఒకవేళ (opt & OUTPUT_SHOW_SCORE)
				రాయి(" %*d %02d",
				       max_score_digits, ent->score,
				       ent->suspect->refcnt);
			ఒకవేళ (opt & OUTPUT_SHOW_NAME)
				రాయి(" %-*.*s", longest_file, longest_file,
				       suspect->మార్గం);
			ఒకవేళ (opt & OUTPUT_SHOW_సంఖ్య)
				రాయి(" %*d", max_orig_digits,
				       ent->s_lno + 1 + cnt);

			ఒకవేళ (!(opt & OUTPUT_NO_AUTHOR)) {
				స్థిరరాశి అక్షరం *name;
				పూర్ణాంకం pad;
				ఒకవేళ (opt & OUTPUT_SHOW_EMAIL)
					name = ci.author_mail.buf;
				లేకపోతే
					name = ci.author.buf;
				pad = longest_author - utf8_strwidth(name);
				రాయి(" (%s%*s %10s",
				       name, pad, "",
				       format_సమయం(ci.author_సమయం,
						   ci.author_tz.buf,
						   show_raw_సమయం));
			}
			రాయి(" %*d) ",
			       max_digits, ent->lno + 1 + cnt);
		}
		ఒకవేళ (reset)
			fputs(reset, stdout);
		చేయి {
			ch = *cp++;
			అక్షరం_రాయి(ch);
		} అయితే (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_కొలత);
	}

	ఒకవేళ (sb->final_buf_కొలత && cp[-1] != '\n')
		అక్షరం_రాయి('\n');

	commit_info_destroy(&ci);
}

స్థిర శూన్య output(నిర్మాణం blame_scoreboard *sb, పూర్ణాంకం option)
{
	నిర్మాణం blame_నమోదు *ent, *prev_ent = ఖాళీ;

	ఒకవేళ (option & OUTPUT_PORCELAIN) {
		కొరకు (ent = sb->ent; ent; ent = ent->తదుపరి) {
			పూర్ణాంకం లెక్క = 0;
			నిర్మాణం blame_origin *suspect;
			నిర్మాణం commit *commit = ent->suspect->commit;
			ఒకవేళ (commit->object.flags & మరిన్ని_THAN_ONE_మార్గం)
				కొనసాగు;
			కొరకు (suspect = get_blame_suspects(commit); suspect; suspect = suspect->తదుపరి) {
				ఒకవేళ (suspect->guilty && లెక్క++) {
					commit->object.flags |= మరిన్ని_THAN_ONE_మార్గం;
					ఆపు;
				}
			}
		}
	}

	కొరకు (ent = sb->ent; ent; ent = ent->తదుపరి) {
		ఒకవేళ (option & OUTPUT_PORCELAIN)
			emit_porcelain(sb, ent, option);
		లేకపోతే {
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
స్థిర పూర్ణాంకం read_ancestry(స్థిరరాశి అక్షరం *graft_file)
{
	FILE *fp = fopen_or_warn(graft_file, "r");
	నిర్మాణం strbuf buf = STRBUF_INIT;
	ఒకవేళ (!fp)
		తిరిగి -1;
	అయితే (!strbuf_getwholeline(&buf, fp, '\n')) {
		/* The format is just "Commit Parent1 Parent2 ...\n" */
		నిర్మాణం commit_graft *graft = read_graft_పంక్తి(&buf);
		ఒకవేళ (graft)
			రిజిస్టరు_commit_graft(the_repository, graft, 0);
	}
	fclose(fp);
	strbuf_release(&buf);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం update_auto_abbrev(పూర్ణాంకం auto_abbrev, నిర్మాణం blame_origin *suspect)
{
	స్థిరరాశి అక్షరం *uniq = repo_కనుగొను_unique_abbrev(the_repository,
						   &suspect->commit->object.oid,
						   auto_abbrev);
	పూర్ణాంకం len = అక్షరశ్రేణి_పొడవు(uniq);
	ఒకవేళ (auto_abbrev < len)
		తిరిగి len;
	తిరిగి auto_abbrev;
}

/*
 * How many columns do we need to show line numbers, authors,
 * and filenames?
 */
స్థిర శూన్య కనుగొను_alignment(నిర్మాణం blame_scoreboard *sb, పూర్ణాంకం *option)
{
	పూర్ణాంకం longest_src_పంక్తులు = 0;
	పూర్ణాంకం longest_dst_పంక్తులు = 0;
	గుర్తులేని అతిపెద్ద_score = 0;
	నిర్మాణం blame_నమోదు *e;
	పూర్ణాంకం compute_auto_abbrev = (abbrev < 0);
	పూర్ణాంకం auto_abbrev = DEFAULT_ABBREV;
	కొలత_t max_marks_లెక్క = 0;

	కొరకు (e = sb->ent; e; e = e->తదుపరి) {
		నిర్మాణం blame_origin *suspect = e->suspect;
		పూర్ణాంకం అంకె;
		కొలత_t marks_లెక్క = లెక్క_marks(e, *option);

		ఒకవేళ (max_marks_లెక్క < marks_లెక్క)
			max_marks_లెక్క = marks_లెక్క;
		ఒకవేళ (compute_auto_abbrev)
			auto_abbrev = update_auto_abbrev(auto_abbrev, suspect);
		ఒకవేళ (strcmp(suspect->మార్గం, sb->మార్గం))
			*option |= OUTPUT_SHOW_NAME;
		అంకె = అక్షరశ్రేణి_పొడవు(suspect->మార్గం);
		ఒకవేళ (longest_file < అంకె)
			longest_file = అంకె;
		ఒకవేళ (!(suspect->commit->object.flags & METAINFO_SHOWN)) {
			నిర్మాణం commit_info ci = COMMIT_INFO_INIT;
			suspect->commit->object.flags |= METAINFO_SHOWN;
			get_commit_info(suspect->commit, &ci);
			ఒకవేళ (*option & OUTPUT_SHOW_EMAIL)
				అంకె = utf8_strwidth(ci.author_mail.buf);
			లేకపోతే
				అంకె = utf8_strwidth(ci.author.buf);
			ఒకవేళ (longest_author < అంకె)
				longest_author = అంకె;
			commit_info_destroy(&ci);
		}
		అంకె = e->s_lno + e->అంకె_పంక్తులు;
		ఒకవేళ (longest_src_పంక్తులు < అంకె)
			longest_src_పంక్తులు = అంకె;
		అంకె = e->lno + e->అంకె_పంక్తులు;
		ఒకవేళ (longest_dst_పంక్తులు < అంకె)
			longest_dst_పంక్తులు = అంకె;
		ఒకవేళ (అతిపెద్ద_score < blame_నమోదు_score(sb, e))
			అతిపెద్ద_score = blame_నమోదు_score(sb, e);
	}
	max_orig_digits = decimal_వెడల్పు(longest_src_పంక్తులు);
	max_digits = decimal_వెడల్పు(longest_dst_పంక్తులు);
	max_score_digits = decimal_వెడల్పు(అతిపెద్ద_score);

	ఒకవేళ (compute_auto_abbrev)
		abbrev = auto_abbrev;
	ఒకవేళ (abbrev < (పూర్ణాంకం)the_hash_algo->hexsz) {
		abbrev += max_marks_లెక్క;
		ఒకవేళ (abbrev > (పూర్ణాంకం)the_hash_algo->hexsz)
			abbrev = the_hash_algo->hexsz;
	}
}

స్థిర శూన్య sanity_check_on_fail(నిర్మాణం blame_scoreboard *sb, పూర్ణాంకం baa)
{
	పూర్ణాంకం opt = OUTPUT_SHOW_SCORE | OUTPUT_SHOW_సంఖ్య | OUTPUT_SHOW_NAME;
	కనుగొను_alignment(sb, &opt);
	output(sb, opt);
	die("Baa %d!", baa);
}

స్థిర గుర్తులేని parse_score(స్థిరరాశి అక్షరం *arg)
{
	అక్షరం *చివర;
	గుర్తులేని దీర్ఘ score = strtoul(arg, &చివర, 10);
	ఒకవేళ (*చివర)
		తిరిగి 0;
	తిరిగి score;
}

స్థిర అక్షరం *కూడు_ఉపసర్గ(స్థిరరాశి అక్షరం *ఉపసర్గ, స్థిరరాశి అక్షరం *మార్గం)
{
	తిరిగి ఉపసర్గ_మార్గం(the_repository, ఉపసర్గ, ఉపసర్గ ? అక్షరశ్రేణి_పొడవు(ఉపసర్గ) : 0, మార్గం);
}

స్థిర పూర్ణాంకం git_blame_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
			    స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	ఒకవేళ (!strcmp(var, "blame.showroot")) {
		show_మూలం = git_config_bool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "blame.blankboundary")) {
		blank_boundary = git_config_bool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "blame.showemail")) {
		పూర్ణాంకం *output_option = cb;
		ఒకవేళ (git_config_bool(var, విలువ))
			*output_option |= OUTPUT_SHOW_EMAIL;
		లేకపోతే
			*output_option &= ~OUTPUT_SHOW_EMAIL;
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "blame.date")) {
		ఒకవేళ (!విలువ)
			తిరిగి config_error_nonbool(var);
		parse_date_format(విలువ, &blame_date_బహుళకం);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "blame.ignorerevsfile")) {
		అక్షరం *str;
		పూర్ణాంకం ret;

		ret = git_config_pathname(&str, var, విలువ);
		ఒకవేళ (ret)
			తిరిగి ret;
		ఒకవేళ (str)
			string_list_చొప్పించు(&ignore_revs_file_list, str);
		విడుదల(str);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "blame.markunblamablelines")) {
		mark_unblamable_పంక్తులు = git_config_bool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "blame.markignoredlines")) {
		mark_ignored_పంక్తులు = git_config_bool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "color.blame.repeatedlines")) {
		ఒకవేళ (color_parse_mem(విలువ, అక్షరశ్రేణి_పొడవు(విలువ), repeated_meta_color))
			warning(_("invalid value for '%s': '%s'"),
				"color.blame.repeatedLines", విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (!strcmp(var, "color.blame.highlightrecent")) {
		parse_color_fields(విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (!strcmp(var, "blame.coloring")) {
		ఒకవేళ (!విలువ)
			తిరిగి config_error_nonbool(var);
		ఒకవేళ (!strcmp(విలువ, "repeatedLines")) {
			coloring_బహుళకం |= OUTPUT_COLOR_పంక్తి;
		} లేకపోతే ఒకవేళ (!strcmp(విలువ, "highlightRecent")) {
			coloring_బహుళకం |= OUTPUT_SHOW_AGE_WITH_COLOR;
		} లేకపోతే ఒకవేళ (!strcmp(విలువ, "none")) {
			coloring_బహుళకం &= ~(OUTPUT_COLOR_పంక్తి |
					    OUTPUT_SHOW_AGE_WITH_COLOR);
		} లేకపోతే {
			warning(_("invalid value for '%s': '%s'"),
				"blame.coloring", విలువ);
			తిరిగి 0;
		}
	}

	ఒకవేళ (!strcmp(var, "diff.algorithm")) {
		దీర్ఘ diff_algorithm;
		ఒకవేళ (!విలువ)
			తిరిగి config_error_nonbool(var);
		diff_algorithm = parse_algorithm_విలువ(విలువ);
		ఒకవేళ (diff_algorithm < 0)
			తిరిగి error(_("unknown value for config '%s': %s"),
				     var, విలువ);
		xdl_opts &= ~XDF_DIFF_ALGORITHM_ముసుగు;
		xdl_opts |= diff_algorithm;
		తిరిగి 0;
	}

	ఒకవేళ (git_diff_heuristic_config(var, విలువ, cb) < 0)
		తిరిగి -1;
	ఒకవేళ (userdiff_config(var, విలువ) < 0)
		తిరిగి -1;

	తిరిగి git_default_config(var, విలువ, ctx, cb);
}

స్థిర పూర్ణాంకం blame_copy_callback(స్థిరరాశి నిర్మాణం option *option, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	పూర్ణాంకం *opt = option->విలువ;

	BUG_ON_OPT_NEG(unset);

	/*
	 * -C enables copy from removed files;
	 * -C -C enables copy from existing files, but only
	 *       when blaming a new file;
	 * -C -C -C enables copy from existing files for
	 *          everybody
	 */
	ఒకవేళ (*opt & PICKAXE_BLAME_COPY_HARDER)
		*opt |= PICKAXE_BLAME_COPY_HARDEST;
	ఒకవేళ (*opt & PICKAXE_BLAME_COPY)
		*opt |= PICKAXE_BLAME_COPY_HARDER;
	*opt |= PICKAXE_BLAME_COPY | PICKAXE_BLAME_MOVE;

	ఒకవేళ (arg)
		blame_copy_score = parse_score(arg);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం blame_move_callback(స్థిరరాశి నిర్మాణం option *option, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	పూర్ణాంకం *opt = option->విలువ;

	BUG_ON_OPT_NEG(unset);

	*opt |= PICKAXE_BLAME_MOVE;

	ఒకవేళ (arg)
		blame_move_score = parse_score(arg);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం blame_diff_algorithm_కనీస(స్థిరరాశి నిర్మాణం option *option,
					స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	పూర్ణాంకం *opt = option->విలువ;

	BUG_ON_OPT_ARG(arg);

	*opt &= ~XDF_DIFF_ALGORITHM_ముసుగు;
	ఒకవేళ (!unset)
		*opt |= XDF_NEED_కనీస;

	తిరిగి 0;
}

స్థిర పూర్ణాంకం blame_diff_algorithm_callback(స్థిరరాశి నిర్మాణం option *option,
					 స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	పూర్ణాంకం *opt = option->విలువ;
	దీర్ఘ విలువ = parse_algorithm_విలువ(arg);

	BUG_ON_OPT_NEG(unset);

	ఒకవేళ (విలువ < 0)
		తిరిగి error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	*opt &= ~XDF_DIFF_ALGORITHM_ముసుగు;
	*opt |= విలువ;

	తిరిగి 0;
}

స్థిర పూర్ణాంకం is_a_rev(స్థిరరాశి అక్షరం *name)
{
	నిర్మాణం object_id oid;

	ఒకవేళ (repo_get_oid(the_repository, name, &oid))
		తిరిగి 0;
	తిరిగి OBJ_NONE < odb_read_object_info(the_repository->objects, &oid, ఖాళీ);
}

స్థిర పూర్ణాంకం peel_గమ్యం_commit_oid(నిర్మాణం object_id *oid_ret, శూన్య *cbdata)
{
	నిర్మాణం repository *r = ((నిర్మాణం blame_scoreboard *)cbdata)->repo;
	నిర్మాణం object_id oid;

	oidcpy(&oid, oid_ret);
	అయితే (1) {
		నిర్మాణం object *obj;
		పూర్ణాంకం రకం = odb_read_object_info(r->objects, &oid, ఖాళీ);
		ఒకవేళ (రకం == OBJ_COMMIT) {
			oidcpy(oid_ret, &oid);
			తిరిగి 0;
		}
		ఒకవేళ (రకం != OBJ_ట్యాగ్)
			తిరిగి -1;
		obj = deref_ట్యాగ్(r, parse_object(r, &oid), ఖాళీ, 0);
		ఒకవేళ (!obj)
			తిరిగి -1;
		oidcpy(&oid, &obj->oid);
	}
}

స్థిర శూన్య build_ignorelist(నిర్మాణం blame_scoreboard *sb,
			     నిర్మాణం string_list *ignore_revs_file_list,
			     నిర్మాణం string_list *ignore_rev_list)
{
	నిర్మాణం string_list_అంశం *i;
	నిర్మాణం object_id oid;

	oidset_init(&sb->ignore_list, 0);
	for_each_string_list_అంశం(i, ignore_revs_file_list) {
		ఒకవేళ (!strcmp(i->string, ""))
			oidset_clear(&sb->ignore_list);
		లేకపోతే
			oidset_parse_file_carefully(&sb->ignore_list, i->string,
						    the_repository->hash_algo,
						    peel_గమ్యం_commit_oid, sb);
	}
	for_each_string_list_అంశం(i, ignore_rev_list) {
		ఒకవేళ (repo_get_oid_committish(the_repository, i->string, &oid) ||
		    peel_గమ్యం_commit_oid(&oid, sb))
			die(_("cannot find revision %s to ignore"), i->string);
		oidset_చొప్పించు(&sb->ignore_list, &oid);
	}
}

పూర్ణాంకం cmd_blame(పూర్ణాంకం argc,
	      స్థిరరాశి అక్షరం **argv,
	      స్థిరరాశి అక్షరం *ఉపసర్గ,
	      నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం rev_info revs;
	అక్షరం *మార్గం = ఖాళీ;
	నిర్మాణం blame_scoreboard sb;
	నిర్మాణం blame_origin *o;
	నిర్మాణం blame_నమోదు *ent = ఖాళీ;
	దీర్ఘ dashdash_pos, lno;
	నిర్మాణం పురోగతి_info pi = { ఖాళీ, 0 };

	నిర్మాణం string_list range_list = STRING_LIST_INIT_NODUP;
	నిర్మాణం string_list ignore_rev_list = STRING_LIST_INIT_NODUP;
	పూర్ణాంకం output_option = 0, opt = 0;
	పూర్ణాంకం show_stats = 0;
	స్థిరరాశి అక్షరం *revs_file = ఖాళీ;
	స్థిరరాశి అక్షరం *contents_from = ఖాళీ;
	స్థిరరాశి నిర్మాణం option options[] = {
		OPT_BOOL(0, "incremental", &incremental, N_("show blame entries as we find them, incrementally")),
		OPT_BOOL('b', ఖాళీ, &blank_boundary, N_("do not show object names of boundary commits (Default: off)")),
		OPT_BOOL(0, "root", &show_మూలం, N_("do not treat root commits as boundaries (Default: off)")),
		OPT_BOOL(0, "show-stats", &show_stats, N_("show work cost statistics")),
		OPT_BOOL(0, "progress", &show_పురోగతి, N_("force progress reporting")),
		OPT_బిట్టు(0, "score-debug", &output_option, N_("show output score for blame entries"), OUTPUT_SHOW_SCORE),
		OPT_బిట్టు('f', "show-name", &output_option, N_("show original filename (Default: auto)"), OUTPUT_SHOW_NAME),
		OPT_బిట్టు('n', "show-number", &output_option, N_("show original linenumber (Default: off)"), OUTPUT_SHOW_సంఖ్య),
		OPT_బిట్టు('p', "porcelain", &output_option, N_("show in a format designed for machine consumption"), OUTPUT_PORCELAIN),
		OPT_బిట్టు(0, "line-porcelain", &output_option, N_("show porcelain format with per-line commit information"), OUTPUT_PORCELAIN|OUTPUT_పంక్తి_PORCELAIN),
		OPT_బిట్టు('c', ఖాళీ, &output_option, N_("use the same output mode as git-annotate (Default: off)"), OUTPUT_ANNOTATE_COMPAT),
		OPT_బిట్టు('t', ఖాళీ, &output_option, N_("show raw timestamp (Default: off)"), OUTPUT_RAW_TIMESTAMP),
		OPT_బిట్టు('l', ఖాళీ, &output_option, N_("show long commit SHA1 (Default: off)"), OUTPUT_LONG_OBJECT_NAME),
		OPT_బిట్టు('s', ఖాళీ, &output_option, N_("suppress author name and timestamp (Default: off)"), OUTPUT_NO_AUTHOR),
		OPT_బిట్టు('e', "show-email", &output_option, N_("show author email instead of name (Default: off)"), OUTPUT_SHOW_EMAIL),
		OPT_బిట్టు('w', ఖాళీ, &xdl_opts, N_("ignore whitespace differences"), XDF_IGNORE_WHITESPACE),
		OPT_CALLBACK_F(0, "diff-algorithm", &xdl_opts, N_("<algorithm>"),
			       N_("choose a diff algorithm"),
			       PARSE_OPT_NONEG, blame_diff_algorithm_callback),
		OPT_STRING_LIST(0, "ignore-rev", &ignore_rev_list, N_("rev"), N_("ignore <rev> when blaming")),
		OPT_STRING_LIST(0, "ignore-revs-file", &ignore_revs_file_list, N_("file"), N_("ignore revisions from <file>")),
		OPT_బిట్టు(0, "color-lines", &output_option, N_("color redundant metadata from previous line differently"), OUTPUT_COLOR_పంక్తి),
		OPT_బిట్టు(0, "color-by-age", &output_option, N_("color lines by age"), OUTPUT_SHOW_AGE_WITH_COLOR),
		OPT_CALLBACK_F(0, "minimal", &xdl_opts, ఖాళీ,
			       N_("spend extra cycles to find a better match"),
			       PARSE_OPT_NOARG | PARSE_OPT_HIDDEN, blame_diff_algorithm_కనీస),
		OPT_STRING('S', ఖాళీ, &revs_file, N_("file"), N_("use revisions from <file> instead of calling git-rev-list")),
		OPT_STRING(0, "contents", &contents_from, N_("file"), N_("use <file>'s contents as the final image")),
		OPT_CALLBACK_F('C', ఖాళీ, &opt, N_("score"), N_("find line copies within and across files"), PARSE_OPT_OPTARG, blame_copy_callback),
		OPT_CALLBACK_F('M', ఖాళీ, &opt, N_("score"), N_("find line movements within and across files"), PARSE_OPT_OPTARG, blame_move_callback),
		OPT_STRING_LIST('L', ఖాళీ, &range_list, N_("range"),
				N_("process only line range <start>,<end> or function :<funcname>")),
		OPT__ABBREV(&abbrev),
		OPT_చివర()
	};

	నిర్మాణం parse_opt_ctx_t ctx;
	పూర్ణాంకం cmd_is_annotate = !strcmp(argv[0], "annotate");
	నిర్మాణం range_సమితి ranges;
	గుర్తులేని పూర్ణాంకం range_i;
	దీర్ఘ anchor;
	దీర్ఘ అంకె_పంక్తులు = 0;
	స్థిరరాశి అక్షరం *str_usage = cmd_is_annotate ? annotate_usage : blame_usage;
	స్థిరరాశి అక్షరం *స్థిరరాశి *opt_usage = cmd_is_annotate ? annotate_opt_usage : blame_opt_usage;

	అమరిక_default_color_by_age();
	repo_config(the_repository, git_blame_config, &output_option);
	repo_init_revisions(the_repository, &revs, ఖాళీ);
	revs.date_బహుళకం = blame_date_బహుళకం;
	revs.diffopt.flags.allow_textconv = 1;
	revs.diffopt.flags.follow_renames = 1;

	save_commit_బఫరు = 0;
	dashdash_pos = 0;
	show_పురోగతి = -1;

	parse_options_ప్రారంభం(&ctx, argc, argv, ఉపసర్గ, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);
	కొరకు (;;) {
		ఎంపిక (parse_options_అడుగు(&ctx, options, opt_usage)) {
		స్థితి PARSE_OPT_NON_OPTION:
		స్థితి PARSE_OPT_UNKNOWN:
			ఆపు;
		స్థితి PARSE_OPT_HELP:
			నిష్క్రమించు(0);
		స్థితి PARSE_OPT_HELP_ERROR:
		స్థితి PARSE_OPT_ERROR:
		స్థితి PARSE_OPT_SUBCOMMAND:
			నిష్క్రమించు(129);
		స్థితి PARSE_OPT_COMPLETE:
			నిష్క్రమించు(0);
		స్థితి PARSE_OPT_పూర్తి:
			ఒకవేళ (ctx.argv[0])
				dashdash_pos = ctx.cpidx;
			వెళ్ళు parse_పూర్తి;
		}

		ఒకవేళ (!strcmp(ctx.argv[0], "--reverse")) {
			ctx.argv[0] = "--children";
			తిరగేయి = 1;
		}
		parse_revision_opt(&revs, &ctx, options, opt_usage);
	}
parse_పూర్తి:
	revision_opts_ముగింపు(&revs);
	no_సంపూర్ణ_file_rename = !revs.diffopt.flags.follow_renames;
	xdl_opts |= revs.diffopt.xdl_opts & XDF_INDENT_HEURISTIC;
	revs.diffopt.flags.follow_renames = 0;
	argc = parse_options_చివర(&ctx);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_నిండిన_సూచీ = 0;

	ఒకవేళ (incremental || (output_option & OUTPUT_PORCELAIN)) {
		ఒకవేళ (show_పురోగతి > 0)
			die(_("--progress can't be used with --incremental or porcelain formats"));
		show_పురోగతి = 0;
	} లేకపోతే ఒకవేళ (show_పురోగతి < 0)
		show_పురోగతి = isatty(2);

	ఒకవేళ (!abbrev)
		abbrev = the_hash_algo->hexsz;

	ఒకవేళ (revs_file && read_ancestry(revs_file))
		die_errno("reading graft file '%s' failed", revs_file);

	ఒకవేళ (cmd_is_annotate) {
		output_option |= OUTPUT_ANNOTATE_COMPAT;
		blame_date_బహుళకం.type = DATE_ISO8601;
	} లేకపోతే {
		blame_date_బహుళకం = revs.date_బహుళకం;
	}

	/* The maximum width used to show the dates */
	ఎంపిక (blame_date_బహుళకం.type) {
	స్థితి DATE_RFC2822:
		blame_date_వెడల్పు = పరిమాణం("Thu, 19 Oct 2006 16:00:04 -0700");
		ఆపు;
	స్థితి DATE_ISO8601_STRICT:
		blame_date_వెడల్పు = పరిమాణం("2006-10-19T16:00:04-07:00");
		ఆపు;
	స్థితి DATE_ISO8601:
		blame_date_వెడల్పు = పరిమాణం("2006-10-19 16:00:04 -0700");
		ఆపు;
	స్థితి DATE_RAW:
		blame_date_వెడల్పు = పరిమాణం("1161298804 -0700");
		ఆపు;
	స్థితి DATE_UNIX:
		blame_date_వెడల్పు = పరిమాణం("1161298804");
		ఆపు;
	స్థితి DATE_SHORT:
		blame_date_వెడల్పు = పరిమాణం("2006-10-19");
		ఆపు;
	స్థితి DATE_RELATIVE:
		/*
		 * TRANSLATORS: This string is used to tell us the
		 * maximum display width for a relative timestamp in
		 * "git blame" output.  For C locale, "4 years, 11
		 * months ago", which takes 22 places, is the longest
		 * among various forms of relative timestamps, but
		 * your language may need more or fewer display
		 * columns.
		 */
		blame_date_వెడల్పు = utf8_strwidth(_("4 years, 11 months ago")) + 1; /* add the null */
		ఆపు;
	స్థితి DATE_HUMAN:
		/* If the year is shown, no time is shown */
		blame_date_వెడల్పు = పరిమాణం("Thu Oct 19 16:00");
		ఆపు;
	స్థితి DATE_సాధారణ:
		blame_date_వెడల్పు = పరిమాణం("Thu Oct 19 16:00:04 2006 -0700");
		ఆపు;
	స్థితి DATE_STRFTIME:
		blame_date_వెడల్పు = అక్షరశ్రేణి_పొడవు(show_date(0, 0, blame_date_బహుళకం)) + 1; /* add the null */
		ఆపు;
	}
	blame_date_వెడల్పు -= 1; /* strip the null */

	ఒకవేళ (revs.diffopt.flags.కనుగొను_copies_harder)
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
	ఒకవేళ (dashdash_pos) {
		ఎంపిక (argc - dashdash_pos - 1) {
		స్థితి 2: /* (1b) */
			ఒకవేళ (argc != 4)
				usage_with_options(opt_usage, options);
			/* reorder for the new way: <rev> -- <path> */
			argv[1] = argv[3];
			argv[3] = argv[2];
			argv[2] = "--";
			/* FALLTHROUGH */
		స్థితి 1: /* (1a) */
			మార్గం = కూడు_ఉపసర్గ(ఉపసర్గ, argv[--argc]);
			argv[argc] = ఖాళీ;
			ఆపు;
		అప్రమేయ:
			usage_with_options(opt_usage, options);
		}
	} లేకపోతే {
		ఒకవేళ (argc < 2)
			usage_with_options(opt_usage, options);
		ఒకవేళ (argc == 3 && is_a_rev(argv[argc - 1])) { /* (2b) */
			మార్గం = కూడు_ఉపసర్గ(ఉపసర్గ, argv[1]);
			argv[1] = argv[2];
		} లేకపోతే {	/* (2a) */
			ఒకవేళ (argc == 2 && is_a_rev(argv[1]) && !repo_get_work_వృక్షం(the_repository))
				die("missing <path> to blame");
			మార్గం = కూడు_ఉపసర్గ(ఉపసర్గ, argv[argc - 1]);
		}
		argv[argc - 1] = "--";
	}

	revs.disable_stdin = 1;
	అమరిక_revisions(argc, argv, &revs, ఖాళీ);
	ఒకవేళ (!revs.pending.nr && is_bare_repository(the_repository)) {
		నిర్మాణం commit *తల_commit;
		నిర్మాణం object_id తల_oid;

		ఒకవేళ (!refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", RESOLVE_REF_READING,
					     &తల_oid, ఖాళీ) ||
		    !(తల_commit = lookup_commit_reference_gently(revs.repo,
							     &తల_oid, 1)))
			die("no such ref: HEAD");

		కూడు_pending_object(&revs, &తల_commit->object, "HEAD");
	}

	init_scoreboard(&sb);
	sb.revs = &revs;
	sb.contents_from = contents_from;
	sb.తిరగేయి = తిరగేయి;
	sb.repo = the_repository;
	sb.మార్గం = మార్గం;
	build_ignorelist(&sb, &ignore_revs_file_list, &ignore_rev_list);
	string_list_clear(&ignore_revs_file_list, 0);
	string_list_clear(&ignore_rev_list, 0);
	అమరిక_scoreboard(&sb, &o);

	/*
	 * Changed-path Bloom filters are disabled when looking
	 * for copies.
	 */
	ఒకవేళ (!(opt & PICKAXE_BLAME_COPY))
		అమరిక_blame_bloom_దత్తాంశం(&sb);

	lno = sb.అంకె_పంక్తులు;

	ఒకవేళ (lno && !range_list.nr)
		string_list_append(&range_list, "1");

	anchor = 1;
	range_సమితి_init(&ranges, range_list.nr);
	కొరకు (range_i = 0; range_i < range_list.nr; ++range_i) {
		దీర్ఘ bottom, శిఖరం;
		ఒకవేళ (parse_range_arg(range_list.వస్తువులు[range_i].string,
				    nth_పంక్తి_cb, &sb, lno, anchor,
				    &bottom, &శిఖరం, sb.మార్గం,
				    the_repository->సూచీ))
			usage(str_usage);
		ఒకవేళ ((!lno && (శిఖరం || bottom)) || lno < bottom)
			die(Q_("file %s has only %lu line",
			       "file %s has only %lu lines",
			       lno), sb.మార్గం, lno);
		ఒకవేళ (bottom < 1)
			bottom = 1;
		ఒకవేళ (శిఖరం < 1 || lno < శిఖరం)
			శిఖరం = lno;
		bottom--;
		range_సమితి_append_unsafe(&ranges, bottom, శిఖరం);
		anchor = శిఖరం + 1;
	}
	క్రమీకరించు_and_విలీనం_range_సమితి(&ranges);

	కొరకు (range_i = ranges.nr; range_i > 0; --range_i) {
		స్థిరరాశి నిర్మాణం range *r = &ranges.ranges[range_i - 1];
		ent = blame_నమోదు_prepend(ent, r->ప్రారంభం, r->చివర, o);
		అంకె_పంక్తులు += (r->చివర - r->ప్రారంభం);
	}
	ఒకవేళ (!అంకె_పంక్తులు)
		అంకె_పంక్తులు = sb.అంకె_పంక్తులు;

	o->suspects = ent;
	prio_వరుస_put(&sb.commits, o->commit);

	blame_origin_decref(o);

	range_సమితి_release(&ranges);
	string_list_clear(&range_list, 0);

	sb.ent = ఖాళీ;

	ఒకవేళ (blame_move_score)
		sb.move_score = blame_move_score;
	ఒకవేళ (blame_copy_score)
		sb.copy_score = blame_copy_score;

	sb.debug = DEBUG_BLAME;
	sb.on_sanity_fail = &sanity_check_on_fail;

	sb.show_మూలం = show_మూలం;
	sb.xdl_opts = xdl_opts;
	sb.no_సంపూర్ణ_file_rename = no_సంపూర్ణ_file_rename;

	read_mailmap(the_repository, &mailmap);

	sb.దొరికింది_guilty_నమోదు = &దొరికింది_guilty_నమోదు;
	sb.దొరికింది_guilty_నమోదు_దత్తాంశం = &pi;
	ఒకవేళ (show_పురోగతి)
		pi.పురోగతి = ప్రారంభం_delayed_పురోగతి(the_repository,
						     _("Blaming lines"),
						     అంకె_పంక్తులు);

	కేటాయించు_blame(&sb, opt);

	stop_పురోగతి(&pi.పురోగతి);

	ఒకవేళ (!incremental)
		అమరిక_pager(the_repository);
	లేకపోతే
		వెళ్ళు cleanup;

	blame_క్రమీకరించు_final(&sb);

	blame_coalesce(&sb);

	ఒకవేళ (!(output_option & (OUTPUT_COLOR_పంక్తి | OUTPUT_SHOW_AGE_WITH_COLOR)))
		output_option |= coloring_బహుళకం;

	ఒకవేళ (!(output_option & OUTPUT_PORCELAIN)) {
		కనుగొను_alignment(&sb, &output_option);
		ఒకవేళ (!*repeated_meta_color &&
		    (output_option & OUTPUT_COLOR_పంక్తి))
			xsnprintf(repeated_meta_color,
				  పరిమాణం(repeated_meta_color),
				  "%s", GIT_COLOR_CYAN);
	}
	ఒకవేళ (output_option & OUTPUT_ANNOTATE_COMPAT)
		output_option &= ~(OUTPUT_COLOR_పంక్తి | OUTPUT_SHOW_AGE_WITH_COLOR);

	output(&sb, output_option);

	ఒకవేళ (show_stats) {
		రాయి("num read blob: %d\n", sb.అంకె_read_blob);
		రాయి("num get patch: %d\n", sb.అంకె_get_patch);
		రాయి("num commits: %d\n", sb.అంకె_commits);
	}

cleanup:
	కొరకు (ent = sb.ent; ent; ) {
		నిర్మాణం blame_నమోదు *e = ent->తదుపరి;
		విడుదల(ent);
		ent = e;
	}

	విడుదల(మార్గం);
	cleanup_scoreboard(&sb);
	release_revisions(&revs);
	తిరిగి 0;
}
