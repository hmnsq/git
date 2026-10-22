/*
 * Blame
 *
 * Copyright (c) 2006, 2014 by its authors
 * See COPYING for licensing conditions
 */

#define USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "color.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "mailmap.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "prio-queue.h"
#अन्तर्भाव "utf8.h"
#अन्तर्भाव "userdiff.h"
#अन्तर्भाव "line-range.h"
#अन्तर्भाव "line-log.h"
#अन्तर्भाव "progress.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "pager.h"
#अन्तर्भाव "blame.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "tag.h"
#अन्तर्भाव "write-or-die.h"

स्थिर अचर वर्ण blame_usage[] = N_("git blame [<options>] [<rev-opts>] [<rev>] [--] <file>");
स्थिर अचर वर्ण annotate_usage[] = N_("git annotate [<options>] [<rev-opts>] [<rev>] [--] <file>");

स्थिर अचर वर्ण *अचर blame_opt_usage[] = {
	blame_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	रिक्त
};

स्थिर अचर वर्ण *अचर annotate_opt_usage[] = {
	annotate_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	रिक्त
};

स्थिर पूर्णाङ्क longest_file;
स्थिर पूर्णाङ्क longest_author;
स्थिर पूर्णाङ्क max_orig_digits;
स्थिर पूर्णाङ्क max_digits;
स्थिर पूर्णाङ्क max_score_digits;
स्थिर पूर्णाङ्क show_जरा;
स्थिर पूर्णाङ्क reverse;
स्थिर पूर्णाङ्क blank_boundary;
स्थिर पूर्णाङ्क incremental;
स्थिर पूर्णाङ्क xdl_opts;
स्थिर पूर्णाङ्क abbrev = -1;
स्थिर पूर्णाङ्क no_whole_file_rename;
स्थिर पूर्णाङ्क show_progress;
स्थिर वर्ण repeated_meta_color[COLOR_MAXLEN];
स्थिर पूर्णाङ्क coloring_mode;
स्थिर संरचना string_list ignore_revs_file_list = STRING_LIST_INIT_DUP;
स्थिर पूर्णाङ्क mark_unblamable_पङ्क्तिहरू;
स्थिर पूर्णाङ्क mark_ignored_पङ्क्तिहरू;

स्थिर संरचना date_mode blame_date_mode = { DATE_ISO8601 };
स्थिर माप_प्रकार blame_date_width;

स्थिर संरचना string_list mailmap = STRING_LIST_INIT_NODUP;

#ifndef DEBUG_BLAME
#define DEBUG_BLAME 0
#endif

स्थिर अचिन्हित blame_move_score;
स्थिर अचिन्हित blame_copy_score;

/* Remember to update object flag allocation in object.h */
#define METAINFO_SHOWN		(1u<<12)
#define MORE_THAN_ONE_PATH	(1u<<13)

संरचना progress_info {
	संरचना progress *progress;
	पूर्णाङ्क blamed_पङ्क्तिहरू;
};

स्थिर अचर वर्ण *nth_पङ्क्ति_cb(शून्य *data, दीर्घ lno)
{
	फर्काऊ blame_nth_पङ्क्ति((संरचना blame_scoreboard *)data, lno);
}

/*
 * Information on commits, used for output.
 */
संरचना commit_info {
	संरचना strbuf author;
	संरचना strbuf author_mail;
	timestamp_t author_time;
	संरचना strbuf author_tz;

	/* filled only when asked for details */
	संरचना strbuf committer;
	संरचना strbuf committer_mail;
	timestamp_t committer_time;
	संरचना strbuf committer_tz;

	संरचना strbuf summary;
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
स्थिर शून्य get_ac_पङ्क्ति(अचर वर्ण *inbuf, अचर वर्ण *what,
	संरचना strbuf *name, संरचना strbuf *mail,
	timestamp_t *समय, संरचना strbuf *tz)
{
	संरचना ident_split ident;
	माप_प्रकार len, maillen, namelen;
	अचर वर्ण *tmp, *endp;
	अचर वर्ण *namebuf, *mailbuf;

	tmp = म_माला(inbuf, what);
	यदि (!tmp)
		गच्छ error_out;
	tmp += म_माप(what);
	endp = म_अक्षर(tmp, '\n');
	यदि (!endp)
		len = म_माप(tmp);
	अन्यथा
		len = endp - tmp;

	यदि (split_ident_पङ्क्ति(&ident, tmp, len)) {
	error_out:
		/* Ugh */
		tmp = "(unknown)";
		strbuf_addstr(name, tmp);
		strbuf_addstr(mail, tmp);
		strbuf_addstr(tz, tmp);
		*समय = 0;
		फर्काऊ;
	}

	namelen = ident.name_end - ident.name_begin;
	namebuf = ident.name_begin;

	maillen = ident.mail_end - ident.mail_begin;
	mailbuf = ident.mail_begin;

	यदि (ident.date_begin && ident.date_end)
		*समय = म_से_अदीर्घ(ident.date_begin, रिक्त, 10);
	अन्यथा
		*समय = 0;

	यदि (ident.tz_begin && ident.tz_end)
		strbuf_add(tz, ident.tz_begin, ident.tz_end - ident.tz_begin);
	अन्यथा
		strbuf_addstr(tz, "(unknown)");

	/*
	 * Now, convert both name and e-mail using mailmap
	 */
	map_user(&mailmap, &mailbuf, &maillen,
		 &namebuf, &namelen);

	strbuf_addf(mail, "<%.*s>", (पूर्णाङ्क)maillen, mailbuf);
	strbuf_add(name, namebuf, namelen);
}

स्थिर शून्य commit_info_destroy(संरचना commit_info *ci)
{

	strbuf_release(&ci->author);
	strbuf_release(&ci->author_mail);
	strbuf_release(&ci->author_tz);
	strbuf_release(&ci->committer);
	strbuf_release(&ci->committer_mail);
	strbuf_release(&ci->committer_tz);
	strbuf_release(&ci->summary);
}

स्थिर शून्य get_commit_info(संरचना commit *commit, संरचना commit_info *ret)
{
	पूर्णाङ्क len;
	अचर वर्ण *subject, *encoding;
	अचर वर्ण *message;

	encoding = get_log_output_encoding();
	message = repo_logmsg_reencode(the_repository, commit, रिक्त, encoding);
	get_ac_पङ्क्ति(message, "\nauthor ",
		    &ret->author, &ret->author_mail,
		    &ret->author_time, &ret->author_tz);

	get_ac_पङ्क्ति(message, "\ncommitter ",
		    &ret->committer, &ret->committer_mail,
		    &ret->committer_time, &ret->committer_tz);

	len = find_commit_subject(message, &subject);
	यदि (len)
		strbuf_add(&ret->summary, subject, len);
	अन्यथा
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
स्थिर शून्य write_filename_info(संरचना blame_origin *suspect)
{
	यदि (suspect->previous) {
		संरचना blame_origin *prev = suspect->previous;
		म_लिखो("previous %s ", oid_to_hex(&prev->commit->object.oid));
		write_name_quoted(prev->path, मानक_निकास, '\n');
	}
	म_लिखो("filename ");
	write_name_quoted(suspect->path, मानक_निकास, '\n');
}

/*
 * Porcelain/Incremental format wants to show a lot of details per
 * commit.  Instead of repeating this every line, emit it only once,
 * the first time each commit appears in the output (unless the
 * user has specifically asked for us to repeat).
 */
स्थिर पूर्णाङ्क emit_one_suspect_detail(संरचना blame_origin *suspect, पूर्णाङ्क repeat)
{
	संरचना commit_info ci = COMMIT_INFO_INIT;

	यदि (!repeat && (suspect->commit->object.flags & METAINFO_SHOWN))
		फर्काऊ 0;

	suspect->commit->object.flags |= METAINFO_SHOWN;
	get_commit_info(suspect->commit, &ci);
	म_लिखो("author %s\n", ci.author.buf);
	म_लिखो("author-mail %s\n", ci.author_mail.buf);
	म_लिखो("author-time %"PRItime"\n", ci.author_time);
	म_लिखो("author-tz %s\n", ci.author_tz.buf);
	म_लिखो("committer %s\n", ci.committer.buf);
	म_लिखो("committer-mail %s\n", ci.committer_mail.buf);
	म_लिखो("committer-time %"PRItime"\n", ci.committer_time);
	म_लिखो("committer-tz %s\n", ci.committer_tz.buf);
	म_लिखो("summary %s\n", ci.summary.buf);
	यदि (suspect->commit->object.flags & UNINTERESTING)
		म_लिखो("boundary\n");

	commit_info_destroy(&ci);

	फर्काऊ 1;
}

/*
 * The blame_entry is found to be guilty for the range.
 * Show it in incremental output.
 */
स्थिर शून्य found_guilty_entry(संरचना blame_entry *ent, शून्य *data)
{
	संरचना progress_info *pi = (संरचना progress_info *)data;

	यदि (incremental) {
		संरचना blame_origin *suspect = ent->suspect;

		म_लिखो("%s %d %d %d\n",
		       oid_to_hex(&suspect->commit->object.oid),
		       ent->s_lno + 1, ent->lno + 1, ent->अङ्क_पङ्क्तिहरू);
		emit_one_suspect_detail(suspect, 0);
		write_filename_info(suspect);
		maybe_flush_or_die(मानक_निकास, "stdout");
	}
	pi->blamed_पङ्क्तिहरू += ent->अङ्क_पङ्क्तिहरू;
	display_progress(pi->progress, pi->blamed_पङ्क्तिहरू);
}

स्थिर अचर वर्ण *format_time(timestamp_t समय, अचर वर्ण *tz_str,
			       पूर्णाङ्क show_raw_time)
{
	स्थिर संरचना strbuf time_buf = STRBUF_INIT;

	strbuf_reset(&time_buf);
	यदि (show_raw_time) {
		strbuf_addf(&time_buf, "%"PRItime" %s", समय, tz_str);
	}
	अन्यथा {
		अचर वर्ण *time_str;
		माप_प्रकार time_width;
		पूर्णाङ्क tz;
		tz = म_से_प(tz_str);
		time_str = show_date(समय, tz, blame_date_mode);
		strbuf_addstr(&time_buf, time_str);
		/*
		 * Add space paddings to time_buf to display a fixed width
		 * string, and use time_width for display width calibration.
		 */
		लागि (time_width = utf8_strwidth(time_str);
		     time_width < blame_date_width;
		     time_width++)
			strbuf_addch(&time_buf, ' ');
	}
	फर्काऊ time_buf.buf;
}

#define OUTPUT_ANNOTATE_COMPAT      (1U<<0)
#define OUTPUT_LONG_OBJECT_NAME     (1U<<1)
#define OUTPUT_RAW_TIMESTAMP        (1U<<2)
#define OUTPUT_PORCELAIN            (1U<<3)
#define OUTPUT_SHOW_NAME            (1U<<4)
#define OUTPUT_SHOW_सङ्ख्या          (1U<<5)
#define OUTPUT_SHOW_SCORE           (1U<<6)
#define OUTPUT_NO_AUTHOR            (1U<<7)
#define OUTPUT_SHOW_EMAIL           (1U<<8)
#define OUTPUT_पङ्क्ति_PORCELAIN       (1U<<9)
#define OUTPUT_COLOR_पङ्क्ति           (1U<<10)
#define OUTPUT_SHOW_AGE_WITH_COLOR  (1U<<11)

स्थिर शून्य emit_porcelain_details(संरचना blame_origin *suspect, पूर्णाङ्क repeat)
{
	यदि (emit_one_suspect_detail(suspect, repeat) ||
	    (suspect->commit->object.flags & MORE_THAN_ONE_PATH))
		write_filename_info(suspect);
}

/*
 * Information which needs to be printed per-line goes here. Any
 * information which can be clubbed on a commit/file level, should
 * be printed via 'emit_one_suspect_detail()'.
 */
स्थिर शून्य emit_porcelain_per_पङ्क्ति_details(संरचना blame_entry *ent)
{
	यदि (mark_unblamable_पङ्क्तिहरू && ent->unblamable)
		माला_दो("unblamable");
	यदि (mark_ignored_पङ्क्तिहरू && ent->ignored)
		माला_दो("ignored");
}

स्थिर शून्य emit_porcelain(संरचना blame_scoreboard *sb, संरचना blame_entry *ent,
			   पूर्णाङ्क opt)
{
	पूर्णाङ्क repeat = opt & OUTPUT_पङ्क्ति_PORCELAIN;
	पूर्णाङ्क cnt;
	अचर वर्ण *cp;
	संरचना blame_origin *suspect = ent->suspect;
	वर्ण hex[GIT_MAX_HEXSZ + 1];

	oid_to_hex_r(hex, &suspect->commit->object.oid);
	म_लिखो("%s %d %d %d\n",
	       hex,
	       ent->s_lno + 1,
	       ent->lno + 1,
	       ent->अङ्क_पङ्क्तिहरू);
	emit_porcelain_details(suspect, repeat);
	emit_porcelain_per_पङ्क्ति_details(ent);

	cp = blame_nth_पङ्क्ति(sb, ent->lno);
	लागि (cnt = 0; cnt < ent->अङ्क_पङ्क्तिहरू; cnt++) {
		वर्ण ch;
		यदि (cnt) {
			म_लिखो("%s %d %d\n", hex,
			       ent->s_lno + 1 + cnt,
			       ent->lno + 1 + cnt);
			यदि (repeat)
				emit_porcelain_details(suspect, 1);
			emit_porcelain_per_पङ्क्ति_details(ent);
		}
		अक्षर_दो('\t');
		गर {
			ch = *cp++;
			अक्षर_दो(ch);
		} जबसम्म (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_size);
	}

	यदि (sb->final_buf_size && cp[-1] != '\n')
		अक्षर_दो('\n');
}

स्थिर संरचना color_field {
	timestamp_t hop;
	वर्ण col[COLOR_MAXLEN];
} *colorfield;
स्थिर पूर्णाङ्क colorfield_nr, colorfield_alloc;

स्थिर शून्य parse_color_fields(अचर वर्ण *s)
{
	संरचना string_list l = STRING_LIST_INIT_DUP;
	संरचना string_list_item *item;
	गणना { EXPECT_DATE, EXPECT_COLOR } next = EXPECT_COLOR;

	colorfield_nr = 0;

	/* Ideally this would be stripped and split at the same time? */
	string_list_split(&l, s, ",", -1);
	ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);

	for_each_string_list_item(item, &l) {
		विकल्प (next) {
		पक्ष EXPECT_DATE:
			colorfield[colorfield_nr].hop = approxidate(item->string);
			next = EXPECT_COLOR;
			colorfield_nr++;
			ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);
			रोक;
		पक्ष EXPECT_COLOR:
			यदि (color_parse(item->string, colorfield[colorfield_nr].col))
				die(_("expecting a color: %s"), item->string);
			next = EXPECT_DATE;
			रोक;
		}
	}

	यदि (next == EXPECT_COLOR)
		die(_("must end with a color"));

	colorfield[colorfield_nr].hop = TIME_MAX;
	string_list_clear(&l, 0);
}

स्थिर शून्य setup_default_color_by_age(शून्य)
{
	parse_color_fields("blue,12 month ago,white,1 month ago,red");
}

स्थिर शून्य determine_पङ्क्ति_heat(संरचना commit_info *ci, अचर वर्ण **dest_color)
{
	पूर्णाङ्क i = 0;

	जबसम्म (i < colorfield_nr && ci->author_time > colorfield[i].hop)
		i++;

	*dest_color = colorfield[i].col;
}

स्थिर अन्तःस्थ पूर्णाङ्क maybe_putc(पूर्णाङ्क c, खाता *out)
{
	फर्काऊ out ? अ_दो(c, out) : 0;
}

स्थिर माप_प्रकार print_marks(खाता *out, अचर संरचना blame_entry *ent, पूर्णाङ्क opt)
{
	माप_प्रकार len = 0;

	यदि ((ent->suspect->commit->object.flags & UNINTERESTING) &&
	    !blank_boundary && !(opt & OUTPUT_ANNOTATE_COMPAT)) {
		maybe_putc('^', out);
		len++;
	}
	यदि (mark_unblamable_पङ्क्तिहरू && ent->unblamable) {
		maybe_putc('*', out);
		len++;
	}
	यदि (mark_ignored_पङ्क्तिहरू && ent->ignored) {
		maybe_putc('?', out);
		len++;
	}
	फर्काऊ len;
}

स्थिर माप_प्रकार गन्ती_marks(अचर संरचना blame_entry *ent, पूर्णाङ्क opt)
{
	फर्काऊ print_marks(रिक्त, ent, opt);
}

स्थिर शून्य emit_other(संरचना blame_scoreboard *sb, संरचना blame_entry *ent,
		       पूर्णाङ्क opt, संरचना blame_entry *prev_ent)
{
	पूर्णाङ्क cnt;
	अचर वर्ण *cp;
	संरचना blame_origin *suspect = ent->suspect;
	संरचना commit_info ci = COMMIT_INFO_INIT;
	वर्ण hex[GIT_MAX_HEXSZ + 1];
	पूर्णाङ्क show_raw_time = !!(opt & OUTPUT_RAW_TIMESTAMP);
	अचर वर्ण *default_color = रिक्त, *color = रिक्त, *reset = रिक्त;

	get_commit_info(suspect->commit, &ci);
	oid_to_hex_r(hex, &suspect->commit->object.oid);

	cp = blame_nth_पङ्क्ति(sb, ent->lno);

	यदि (opt & OUTPUT_SHOW_AGE_WITH_COLOR) {
		determine_पङ्क्ति_heat(&ci, &default_color);
		color = default_color;
		reset = GIT_COLOR_RESET;
	}

	यदि (abbrev < MINIMUM_ABBREV)
		BUG("abbreviation is smaller than minimum length: %d < %d",
		    abbrev, MINIMUM_ABBREV);

	लागि (cnt = 0; cnt < ent->अङ्क_पङ्क्तिहरू; cnt++) {
		वर्ण ch;
		माप_प्रकार length = (opt & OUTPUT_LONG_OBJECT_NAME) ?
			the_hash_algo->hexsz : (माप_प्रकार) abbrev;

		यदि (opt & OUTPUT_COLOR_पङ्क्ति) {
			यदि (cnt > 0 ||
			    (prev_ent &&
			     oideq(&suspect->commit->object.oid,
				   &prev_ent->suspect->commit->object.oid))) {
				color = repeated_meta_color;
				reset = GIT_COLOR_RESET;
			} अन्यथा  {
				color = default_color ? default_color : रिक्त;
				reset = default_color ? GIT_COLOR_RESET : रिक्त;
			}
		}
		यदि (color)
			ख_माला_दो(color, मानक_निकास);

		यदि ((suspect->commit->object.flags & UNINTERESTING) &&
		    blank_boundary)
			स_रखो(hex, ' ', म_माप(hex));
		length -= print_marks(मानक_निकास, ent, opt);

		म_लिखो("%.*s", (पूर्णाङ्क)(length < GIT_MAX_HEXSZ ? length : GIT_MAX_HEXSZ), hex);
		यदि (opt & OUTPUT_ANNOTATE_COMPAT) {
			अचर वर्ण *name;
			यदि (opt & OUTPUT_SHOW_EMAIL)
				name = ci.author_mail.buf;
			अन्यथा
				name = ci.author.buf;
			म_लिखो("\t(%10s\t%10s\t%d)", name,
			       format_time(ci.author_time, ci.author_tz.buf,
					   show_raw_time),
			       ent->lno + 1 + cnt);
		} अन्यथा {
			यदि (opt & OUTPUT_SHOW_SCORE)
				म_लिखो(" %*d %02d",
				       max_score_digits, ent->score,
				       ent->suspect->refcnt);
			यदि (opt & OUTPUT_SHOW_NAME)
				म_लिखो(" %-*.*s", longest_file, longest_file,
				       suspect->path);
			यदि (opt & OUTPUT_SHOW_सङ्ख्या)
				म_लिखो(" %*d", max_orig_digits,
				       ent->s_lno + 1 + cnt);

			यदि (!(opt & OUTPUT_NO_AUTHOR)) {
				अचर वर्ण *name;
				पूर्णाङ्क pad;
				यदि (opt & OUTPUT_SHOW_EMAIL)
					name = ci.author_mail.buf;
				अन्यथा
					name = ci.author.buf;
				pad = longest_author - utf8_strwidth(name);
				म_लिखो(" (%s%*s %10s",
				       name, pad, "",
				       format_time(ci.author_time,
						   ci.author_tz.buf,
						   show_raw_time));
			}
			म_लिखो(" %*d) ",
			       max_digits, ent->lno + 1 + cnt);
		}
		यदि (reset)
			ख_माला_दो(reset, मानक_निकास);
		गर {
			ch = *cp++;
			अक्षर_दो(ch);
		} जबसम्म (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_size);
	}

	यदि (sb->final_buf_size && cp[-1] != '\n')
		अक्षर_दो('\n');

	commit_info_destroy(&ci);
}

स्थिर शून्य output(संरचना blame_scoreboard *sb, पूर्णाङ्क option)
{
	संरचना blame_entry *ent, *prev_ent = रिक्त;

	यदि (option & OUTPUT_PORCELAIN) {
		लागि (ent = sb->ent; ent; ent = ent->next) {
			पूर्णाङ्क गन्ती = 0;
			संरचना blame_origin *suspect;
			संरचना commit *commit = ent->suspect->commit;
			यदि (commit->object.flags & MORE_THAN_ONE_PATH)
				जारी;
			लागि (suspect = get_blame_suspects(commit); suspect; suspect = suspect->next) {
				यदि (suspect->guilty && गन्ती++) {
					commit->object.flags |= MORE_THAN_ONE_PATH;
					रोक;
				}
			}
		}
	}

	लागि (ent = sb->ent; ent; ent = ent->next) {
		यदि (option & OUTPUT_PORCELAIN)
			emit_porcelain(sb, ent, option);
		अन्यथा {
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
स्थिर पूर्णाङ्क read_ancestry(अचर वर्ण *graft_file)
{
	खाता *fp = fopen_or_warn(graft_file, "r");
	संरचना strbuf buf = STRBUF_INIT;
	यदि (!fp)
		फर्काऊ -1;
	जबसम्म (!strbuf_getwholeline(&buf, fp, '\n')) {
		/* The format is just "Commit Parent1 Parent2 ...\n" */
		संरचना commit_graft *graft = read_graft_पङ्क्ति(&buf);
		यदि (graft)
			register_commit_graft(the_repository, graft, 0);
	}
	ख_बंद(fp);
	strbuf_release(&buf);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क update_auto_abbrev(पूर्णाङ्क auto_abbrev, संरचना blame_origin *suspect)
{
	अचर वर्ण *uniq = repo_find_unique_abbrev(the_repository,
						   &suspect->commit->object.oid,
						   auto_abbrev);
	पूर्णाङ्क len = म_माप(uniq);
	यदि (auto_abbrev < len)
		फर्काऊ len;
	फर्काऊ auto_abbrev;
}

/*
 * How many columns do we need to show line numbers, authors,
 * and filenames?
 */
स्थिर शून्य find_alignment(संरचना blame_scoreboard *sb, पूर्णाङ्क *option)
{
	पूर्णाङ्क longest_src_पङ्क्तिहरू = 0;
	पूर्णाङ्क longest_dst_पङ्क्तिहरू = 0;
	अचिन्हित largest_score = 0;
	संरचना blame_entry *e;
	पूर्णाङ्क compute_auto_abbrev = (abbrev < 0);
	पूर्णाङ्क auto_abbrev = DEFAULT_ABBREV;
	माप_प्रकार max_marks_गन्ती = 0;

	लागि (e = sb->ent; e; e = e->next) {
		संरचना blame_origin *suspect = e->suspect;
		पूर्णाङ्क अङ्क;
		माप_प्रकार marks_गन्ती = गन्ती_marks(e, *option);

		यदि (max_marks_गन्ती < marks_गन्ती)
			max_marks_गन्ती = marks_गन्ती;
		यदि (compute_auto_abbrev)
			auto_abbrev = update_auto_abbrev(auto_abbrev, suspect);
		यदि (म_भेद(suspect->path, sb->path))
			*option |= OUTPUT_SHOW_NAME;
		अङ्क = म_माप(suspect->path);
		यदि (longest_file < अङ्क)
			longest_file = अङ्क;
		यदि (!(suspect->commit->object.flags & METAINFO_SHOWN)) {
			संरचना commit_info ci = COMMIT_INFO_INIT;
			suspect->commit->object.flags |= METAINFO_SHOWN;
			get_commit_info(suspect->commit, &ci);
			यदि (*option & OUTPUT_SHOW_EMAIL)
				अङ्क = utf8_strwidth(ci.author_mail.buf);
			अन्यथा
				अङ्क = utf8_strwidth(ci.author.buf);
			यदि (longest_author < अङ्क)
				longest_author = अङ्क;
			commit_info_destroy(&ci);
		}
		अङ्क = e->s_lno + e->अङ्क_पङ्क्तिहरू;
		यदि (longest_src_पङ्क्तिहरू < अङ्क)
			longest_src_पङ्क्तिहरू = अङ्क;
		अङ्क = e->lno + e->अङ्क_पङ्क्तिहरू;
		यदि (longest_dst_पङ्क्तिहरू < अङ्क)
			longest_dst_पङ्क्तिहरू = अङ्क;
		यदि (largest_score < blame_entry_score(sb, e))
			largest_score = blame_entry_score(sb, e);
	}
	max_orig_digits = decimal_width(longest_src_पङ्क्तिहरू);
	max_digits = decimal_width(longest_dst_पङ्क्तिहरू);
	max_score_digits = decimal_width(largest_score);

	यदि (compute_auto_abbrev)
		abbrev = auto_abbrev;
	यदि (abbrev < (पूर्णाङ्क)the_hash_algo->hexsz) {
		abbrev += max_marks_गन्ती;
		यदि (abbrev > (पूर्णाङ्क)the_hash_algo->hexsz)
			abbrev = the_hash_algo->hexsz;
	}
}

स्थिर शून्य sanity_check_on_fail(संरचना blame_scoreboard *sb, पूर्णाङ्क baa)
{
	पूर्णाङ्क opt = OUTPUT_SHOW_SCORE | OUTPUT_SHOW_सङ्ख्या | OUTPUT_SHOW_NAME;
	find_alignment(sb, &opt);
	output(sb, opt);
	die("Baa %d!", baa);
}

स्थिर अचिन्हित parse_score(अचर वर्ण *arg)
{
	वर्ण *end;
	अचिन्हित दीर्घ score = म_से_अदीर्घ(arg, &end, 10);
	यदि (*end)
		फर्काऊ 0;
	फर्काऊ score;
}

स्थिर वर्ण *add_prefix(अचर वर्ण *prefix, अचर वर्ण *path)
{
	फर्काऊ prefix_path(the_repository, prefix, prefix ? म_माप(prefix) : 0, path);
}

स्थिर पूर्णाङ्क git_blame_config(अचर वर्ण *var, अचर वर्ण *मान,
			    अचर संरचना config_context *ctx, शून्य *cb)
{
	यदि (!म_भेद(var, "blame.showroot")) {
		show_जरा = git_config_bool(var, मान);
		फर्काऊ 0;
	}
	यदि (!म_भेद(var, "blame.blankboundary")) {
		blank_boundary = git_config_bool(var, मान);
		फर्काऊ 0;
	}
	यदि (!म_भेद(var, "blame.showemail")) {
		पूर्णाङ्क *output_option = cb;
		यदि (git_config_bool(var, मान))
			*output_option |= OUTPUT_SHOW_EMAIL;
		अन्यथा
			*output_option &= ~OUTPUT_SHOW_EMAIL;
		फर्काऊ 0;
	}
	यदि (!म_भेद(var, "blame.date")) {
		यदि (!मान)
			फर्काऊ config_error_nonbool(var);
		parse_date_format(मान, &blame_date_mode);
		फर्काऊ 0;
	}
	यदि (!म_भेद(var, "blame.ignorerevsfile")) {
		वर्ण *str;
		पूर्णाङ्क ret;

		ret = git_config_pathname(&str, var, मान);
		यदि (ret)
			फर्काऊ ret;
		यदि (str)
			string_list_घुसाउ(&ignore_revs_file_list, str);
		मुक्त(str);
		फर्काऊ 0;
	}
	यदि (!म_भेद(var, "blame.markunblamablelines")) {
		mark_unblamable_पङ्क्तिहरू = git_config_bool(var, मान);
		फर्काऊ 0;
	}
	यदि (!म_भेद(var, "blame.markignoredlines")) {
		mark_ignored_पङ्क्तिहरू = git_config_bool(var, मान);
		फर्काऊ 0;
	}
	यदि (!म_भेद(var, "color.blame.repeatedlines")) {
		यदि (color_parse_mem(मान, म_माप(मान), repeated_meta_color))
			warning(_("invalid value for '%s': '%s'"),
				"color.blame.repeatedLines", मान);
		फर्काऊ 0;
	}
	यदि (!म_भेद(var, "color.blame.highlightrecent")) {
		parse_color_fields(मान);
		फर्काऊ 0;
	}

	यदि (!म_भेद(var, "blame.coloring")) {
		यदि (!मान)
			फर्काऊ config_error_nonbool(var);
		यदि (!म_भेद(मान, "repeatedLines")) {
			coloring_mode |= OUTPUT_COLOR_पङ्क्ति;
		} अन्यथा यदि (!म_भेद(मान, "highlightRecent")) {
			coloring_mode |= OUTPUT_SHOW_AGE_WITH_COLOR;
		} अन्यथा यदि (!म_भेद(मान, "none")) {
			coloring_mode &= ~(OUTPUT_COLOR_पङ्क्ति |
					    OUTPUT_SHOW_AGE_WITH_COLOR);
		} अन्यथा {
			warning(_("invalid value for '%s': '%s'"),
				"blame.coloring", मान);
			फर्काऊ 0;
		}
	}

	यदि (!म_भेद(var, "diff.algorithm")) {
		दीर्घ diff_algorithm;
		यदि (!मान)
			फर्काऊ config_error_nonbool(var);
		diff_algorithm = parse_algorithm_मान(मान);
		यदि (diff_algorithm < 0)
			फर्काऊ error(_("unknown value for config '%s': %s"),
				     var, मान);
		xdl_opts &= ~XDF_DIFF_ALGORITHM_MASK;
		xdl_opts |= diff_algorithm;
		फर्काऊ 0;
	}

	यदि (git_diff_heuristic_config(var, मान, cb) < 0)
		फर्काऊ -1;
	यदि (userdiff_config(var, मान) < 0)
		फर्काऊ -1;

	फर्काऊ git_default_config(var, मान, ctx, cb);
}

स्थिर पूर्णाङ्क blame_copy_callback(अचर संरचना option *option, अचर वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *opt = option->मान;

	BUG_ON_OPT_NEG(unset);

	/*
	 * -C enables copy from removed files;
	 * -C -C enables copy from existing files, but only
	 *       when blaming a new file;
	 * -C -C -C enables copy from existing files for
	 *          everybody
	 */
	यदि (*opt & PICKAXE_BLAME_COPY_HARDER)
		*opt |= PICKAXE_BLAME_COPY_HARDEST;
	यदि (*opt & PICKAXE_BLAME_COPY)
		*opt |= PICKAXE_BLAME_COPY_HARDER;
	*opt |= PICKAXE_BLAME_COPY | PICKAXE_BLAME_MOVE;

	यदि (arg)
		blame_copy_score = parse_score(arg);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क blame_move_callback(अचर संरचना option *option, अचर वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *opt = option->मान;

	BUG_ON_OPT_NEG(unset);

	*opt |= PICKAXE_BLAME_MOVE;

	यदि (arg)
		blame_move_score = parse_score(arg);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क blame_diff_algorithm_minimal(अचर संरचना option *option,
					अचर वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *opt = option->मान;

	BUG_ON_OPT_ARG(arg);

	*opt &= ~XDF_DIFF_ALGORITHM_MASK;
	यदि (!unset)
		*opt |= XDF_NEED_MINIMAL;

	फर्काऊ 0;
}

स्थिर पूर्णाङ्क blame_diff_algorithm_callback(अचर संरचना option *option,
					 अचर वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *opt = option->मान;
	दीर्घ मान = parse_algorithm_मान(arg);

	BUG_ON_OPT_NEG(unset);

	यदि (मान < 0)
		फर्काऊ error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	*opt &= ~XDF_DIFF_ALGORITHM_MASK;
	*opt |= मान;

	फर्काऊ 0;
}

स्थिर पूर्णाङ्क is_a_rev(अचर वर्ण *name)
{
	संरचना object_id oid;

	यदि (repo_get_oid(the_repository, name, &oid))
		फर्काऊ 0;
	फर्काऊ OBJ_NONE < odb_read_object_info(the_repository->objects, &oid, रिक्त);
}

स्थिर पूर्णाङ्क peel_to_commit_oid(संरचना object_id *oid_ret, शून्य *cbdata)
{
	संरचना repository *r = ((संरचना blame_scoreboard *)cbdata)->repo;
	संरचना object_id oid;

	oidcpy(&oid, oid_ret);
	जबसम्म (1) {
		संरचना object *obj;
		पूर्णाङ्क kind = odb_read_object_info(r->objects, &oid, रिक्त);
		यदि (kind == OBJ_COMMIT) {
			oidcpy(oid_ret, &oid);
			फर्काऊ 0;
		}
		यदि (kind != OBJ_TAG)
			फर्काऊ -1;
		obj = deref_tag(r, parse_object(r, &oid), रिक्त, 0);
		यदि (!obj)
			फर्काऊ -1;
		oidcpy(&oid, &obj->oid);
	}
}

स्थिर शून्य build_ignorelist(संरचना blame_scoreboard *sb,
			     संरचना string_list *ignore_revs_file_list,
			     संरचना string_list *ignore_rev_list)
{
	संरचना string_list_item *i;
	संरचना object_id oid;

	oidset_init(&sb->ignore_list, 0);
	for_each_string_list_item(i, ignore_revs_file_list) {
		यदि (!म_भेद(i->string, ""))
			oidset_clear(&sb->ignore_list);
		अन्यथा
			oidset_parse_file_carefully(&sb->ignore_list, i->string,
						    the_repository->hash_algo,
						    peel_to_commit_oid, sb);
	}
	for_each_string_list_item(i, ignore_rev_list) {
		यदि (repo_get_oid_committish(the_repository, i->string, &oid) ||
		    peel_to_commit_oid(&oid, sb))
			die(_("cannot find revision %s to ignore"), i->string);
		oidset_घुसाउ(&sb->ignore_list, &oid);
	}
}

पूर्णाङ्क cmd_blame(पूर्णाङ्क argc,
	      अचर वर्ण **argv,
	      अचर वर्ण *prefix,
	      संरचना repository *repo UNUSED)
{
	संरचना rev_info revs;
	वर्ण *path = रिक्त;
	संरचना blame_scoreboard sb;
	संरचना blame_origin *o;
	संरचना blame_entry *ent = रिक्त;
	दीर्घ dashdash_pos, lno;
	संरचना progress_info pi = { रिक्त, 0 };

	संरचना string_list range_list = STRING_LIST_INIT_NODUP;
	संरचना string_list ignore_rev_list = STRING_LIST_INIT_NODUP;
	पूर्णाङ्क output_option = 0, opt = 0;
	पूर्णाङ्क show_stats = 0;
	अचर वर्ण *revs_file = रिक्त;
	अचर वर्ण *contents_from = रिक्त;
	अचर संरचना option options[] = {
		OPT_BOOL(0, "incremental", &incremental, N_("show blame entries as we find them, incrementally")),
		OPT_BOOL('b', रिक्त, &blank_boundary, N_("do not show object names of boundary commits (Default: off)")),
		OPT_BOOL(0, "root", &show_जरा, N_("do not treat root commits as boundaries (Default: off)")),
		OPT_BOOL(0, "show-stats", &show_stats, N_("show work cost statistics")),
		OPT_BOOL(0, "progress", &show_progress, N_("force progress reporting")),
		OPT_BIT(0, "score-debug", &output_option, N_("show output score for blame entries"), OUTPUT_SHOW_SCORE),
		OPT_BIT('f', "show-name", &output_option, N_("show original filename (Default: auto)"), OUTPUT_SHOW_NAME),
		OPT_BIT('n', "show-number", &output_option, N_("show original linenumber (Default: off)"), OUTPUT_SHOW_सङ्ख्या),
		OPT_BIT('p', "porcelain", &output_option, N_("show in a format designed for machine consumption"), OUTPUT_PORCELAIN),
		OPT_BIT(0, "line-porcelain", &output_option, N_("show porcelain format with per-line commit information"), OUTPUT_PORCELAIN|OUTPUT_पङ्क्ति_PORCELAIN),
		OPT_BIT('c', रिक्त, &output_option, N_("use the same output mode as git-annotate (Default: off)"), OUTPUT_ANNOTATE_COMPAT),
		OPT_BIT('t', रिक्त, &output_option, N_("show raw timestamp (Default: off)"), OUTPUT_RAW_TIMESTAMP),
		OPT_BIT('l', रिक्त, &output_option, N_("show long commit SHA1 (Default: off)"), OUTPUT_LONG_OBJECT_NAME),
		OPT_BIT('s', रिक्त, &output_option, N_("suppress author name and timestamp (Default: off)"), OUTPUT_NO_AUTHOR),
		OPT_BIT('e', "show-email", &output_option, N_("show author email instead of name (Default: off)"), OUTPUT_SHOW_EMAIL),
		OPT_BIT('w', रिक्त, &xdl_opts, N_("ignore whitespace differences"), XDF_IGNORE_WHITESPACE),
		OPT_CALLBACK_F(0, "diff-algorithm", &xdl_opts, N_("<algorithm>"),
			       N_("choose a diff algorithm"),
			       PARSE_OPT_NONEG, blame_diff_algorithm_callback),
		OPT_STRING_LIST(0, "ignore-rev", &ignore_rev_list, N_("rev"), N_("ignore <rev> when blaming")),
		OPT_STRING_LIST(0, "ignore-revs-file", &ignore_revs_file_list, N_("file"), N_("ignore revisions from <file>")),
		OPT_BIT(0, "color-lines", &output_option, N_("color redundant metadata from previous line differently"), OUTPUT_COLOR_पङ्क्ति),
		OPT_BIT(0, "color-by-age", &output_option, N_("color lines by age"), OUTPUT_SHOW_AGE_WITH_COLOR),
		OPT_CALLBACK_F(0, "minimal", &xdl_opts, रिक्त,
			       N_("spend extra cycles to find a better match"),
			       PARSE_OPT_NOARG | PARSE_OPT_HIDDEN, blame_diff_algorithm_minimal),
		OPT_STRING('S', रिक्त, &revs_file, N_("file"), N_("use revisions from <file> instead of calling git-rev-list")),
		OPT_STRING(0, "contents", &contents_from, N_("file"), N_("use <file>'s contents as the final image")),
		OPT_CALLBACK_F('C', रिक्त, &opt, N_("score"), N_("find line copies within and across files"), PARSE_OPT_OPTARG, blame_copy_callback),
		OPT_CALLBACK_F('M', रिक्त, &opt, N_("score"), N_("find line movements within and across files"), PARSE_OPT_OPTARG, blame_move_callback),
		OPT_STRING_LIST('L', रिक्त, &range_list, N_("range"),
				N_("process only line range <start>,<end> or function :<funcname>")),
		OPT__ABBREV(&abbrev),
		OPT_END()
	};

	संरचना parse_opt_ctx_t ctx;
	पूर्णाङ्क cmd_is_annotate = !म_भेद(argv[0], "annotate");
	संरचना range_set ranges;
	अचिन्हित पूर्णाङ्क range_i;
	दीर्घ anchor;
	दीर्घ अङ्क_पङ्क्तिहरू = 0;
	अचर वर्ण *str_usage = cmd_is_annotate ? annotate_usage : blame_usage;
	अचर वर्ण *अचर *opt_usage = cmd_is_annotate ? annotate_opt_usage : blame_opt_usage;

	setup_default_color_by_age();
	repo_config(the_repository, git_blame_config, &output_option);
	repo_init_revisions(the_repository, &revs, रिक्त);
	revs.date_mode = blame_date_mode;
	revs.diffopt.flags.allow_textconv = 1;
	revs.diffopt.flags.follow_renames = 1;

	save_commit_buffer = 0;
	dashdash_pos = 0;
	show_progress = -1;

	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);
	लागि (;;) {
		विकल्प (parse_options_step(&ctx, options, opt_usage)) {
		पक्ष PARSE_OPT_NON_OPTION:
		पक्ष PARSE_OPT_UNKNOWN:
			रोक;
		पक्ष PARSE_OPT_HELP:
			निकास(0);
		पक्ष PARSE_OPT_HELP_ERROR:
		पक्ष PARSE_OPT_ERROR:
		पक्ष PARSE_OPT_SUBCOMMAND:
			निकास(129);
		पक्ष PARSE_OPT_COMPLETE:
			निकास(0);
		पक्ष PARSE_OPT_DONE:
			यदि (ctx.argv[0])
				dashdash_pos = ctx.cpidx;
			गच्छ parse_done;
		}

		यदि (!म_भेद(ctx.argv[0], "--reverse")) {
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

	यदि (incremental || (output_option & OUTPUT_PORCELAIN)) {
		यदि (show_progress > 0)
			die(_("--progress can't be used with --incremental or porcelain formats"));
		show_progress = 0;
	} अन्यथा यदि (show_progress < 0)
		show_progress = isatty(2);

	यदि (!abbrev)
		abbrev = the_hash_algo->hexsz;

	यदि (revs_file && read_ancestry(revs_file))
		die_errno("reading graft file '%s' failed", revs_file);

	यदि (cmd_is_annotate) {
		output_option |= OUTPUT_ANNOTATE_COMPAT;
		blame_date_mode.type = DATE_ISO8601;
	} अन्यथा {
		blame_date_mode = revs.date_mode;
	}

	/* The maximum width used to show the dates */
	विकल्प (blame_date_mode.type) {
	पक्ष DATE_RFC2822:
		blame_date_width = परिमाण("Thu, 19 Oct 2006 16:00:04 -0700");
		रोक;
	पक्ष DATE_ISO8601_STRICT:
		blame_date_width = परिमाण("2006-10-19T16:00:04-07:00");
		रोक;
	पक्ष DATE_ISO8601:
		blame_date_width = परिमाण("2006-10-19 16:00:04 -0700");
		रोक;
	पक्ष DATE_RAW:
		blame_date_width = परिमाण("1161298804 -0700");
		रोक;
	पक्ष DATE_UNIX:
		blame_date_width = परिमाण("1161298804");
		रोक;
	पक्ष DATE_SHORT:
		blame_date_width = परिमाण("2006-10-19");
		रोक;
	पक्ष DATE_RELATIVE:
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
		रोक;
	पक्ष DATE_HUMAN:
		/* If the year is shown, no time is shown */
		blame_date_width = परिमाण("Thu Oct 19 16:00");
		रोक;
	पक्ष DATE_NORMAL:
		blame_date_width = परिमाण("Thu Oct 19 16:00:04 2006 -0700");
		रोक;
	पक्ष DATE_STRFTIME:
		blame_date_width = म_माप(show_date(0, 0, blame_date_mode)) + 1; /* add the null */
		रोक;
	}
	blame_date_width -= 1; /* strip the null */

	यदि (revs.diffopt.flags.find_copies_harder)
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
	यदि (dashdash_pos) {
		विकल्प (argc - dashdash_pos - 1) {
		पक्ष 2: /* (1b) */
			यदि (argc != 4)
				usage_with_options(opt_usage, options);
			/* reorder for the new way: <rev> -- <path> */
			argv[1] = argv[3];
			argv[3] = argv[2];
			argv[2] = "--";
			/* FALLTHROUGH */
		पक्ष 1: /* (1a) */
			path = add_prefix(prefix, argv[--argc]);
			argv[argc] = रिक्त;
			रोक;
		स्वभाव:
			usage_with_options(opt_usage, options);
		}
	} अन्यथा {
		यदि (argc < 2)
			usage_with_options(opt_usage, options);
		यदि (argc == 3 && is_a_rev(argv[argc - 1])) { /* (2b) */
			path = add_prefix(prefix, argv[1]);
			argv[1] = argv[2];
		} अन्यथा {	/* (2a) */
			यदि (argc == 2 && is_a_rev(argv[1]) && !repo_get_work_रूख(the_repository))
				die("missing <path> to blame");
			path = add_prefix(prefix, argv[argc - 1]);
		}
		argv[argc - 1] = "--";
	}

	revs.disable_stdin = 1;
	setup_revisions(argc, argv, &revs, रिक्त);
	यदि (!revs.pending.nr && is_bare_repository(the_repository)) {
		संरचना commit *head_commit;
		संरचना object_id head_oid;

		यदि (!refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", RESOLVE_REF_READING,
					     &head_oid, रिक्त) ||
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
	यदि (!(opt & PICKAXE_BLAME_COPY))
		setup_blame_bloom_data(&sb);

	lno = sb.अङ्क_पङ्क्तिहरू;

	यदि (lno && !range_list.nr)
		string_list_append(&range_list, "1");

	anchor = 1;
	range_set_init(&ranges, range_list.nr);
	लागि (range_i = 0; range_i < range_list.nr; ++range_i) {
		दीर्घ bottom, शीर्ष;
		यदि (parse_range_arg(range_list.वस्तुहरू[range_i].string,
				    nth_पङ्क्ति_cb, &sb, lno, anchor,
				    &bottom, &शीर्ष, sb.path,
				    the_repository->index))
			usage(str_usage);
		यदि ((!lno && (शीर्ष || bottom)) || lno < bottom)
			die(Q_("file %s has only %lu line",
			       "file %s has only %lu lines",
			       lno), sb.path, lno);
		यदि (bottom < 1)
			bottom = 1;
		यदि (शीर्ष < 1 || lno < शीर्ष)
			शीर्ष = lno;
		bottom--;
		range_set_append_unsafe(&ranges, bottom, शीर्ष);
		anchor = शीर्ष + 1;
	}
	sort_and_merge_range_set(&ranges);

	लागि (range_i = ranges.nr; range_i > 0; --range_i) {
		अचर संरचना range *r = &ranges.ranges[range_i - 1];
		ent = blame_entry_prepend(ent, r->start, r->end, o);
		अङ्क_पङ्क्तिहरू += (r->end - r->start);
	}
	यदि (!अङ्क_पङ्क्तिहरू)
		अङ्क_पङ्क्तिहरू = sb.अङ्क_पङ्क्तिहरू;

	o->suspects = ent;
	prio_queue_put(&sb.commits, o->commit);

	blame_origin_decref(o);

	range_set_release(&ranges);
	string_list_clear(&range_list, 0);

	sb.ent = रिक्त;

	यदि (blame_move_score)
		sb.move_score = blame_move_score;
	यदि (blame_copy_score)
		sb.copy_score = blame_copy_score;

	sb.debug = DEBUG_BLAME;
	sb.on_sanity_fail = &sanity_check_on_fail;

	sb.show_जरा = show_जरा;
	sb.xdl_opts = xdl_opts;
	sb.no_whole_file_rename = no_whole_file_rename;

	read_mailmap(the_repository, &mailmap);

	sb.found_guilty_entry = &found_guilty_entry;
	sb.found_guilty_entry_data = &pi;
	यदि (show_progress)
		pi.progress = start_delayed_progress(the_repository,
						     _("Blaming lines"),
						     अङ्क_पङ्क्तिहरू);

	assign_blame(&sb, opt);

	stop_progress(&pi.progress);

	यदि (!incremental)
		setup_pager(the_repository);
	अन्यथा
		गच्छ cleanup;

	blame_sort_final(&sb);

	blame_coalesce(&sb);

	यदि (!(output_option & (OUTPUT_COLOR_पङ्क्ति | OUTPUT_SHOW_AGE_WITH_COLOR)))
		output_option |= coloring_mode;

	यदि (!(output_option & OUTPUT_PORCELAIN)) {
		find_alignment(&sb, &output_option);
		यदि (!*repeated_meta_color &&
		    (output_option & OUTPUT_COLOR_पङ्क्ति))
			xsnprintf(repeated_meta_color,
				  परिमाण(repeated_meta_color),
				  "%s", GIT_COLOR_CYAN);
	}
	यदि (output_option & OUTPUT_ANNOTATE_COMPAT)
		output_option &= ~(OUTPUT_COLOR_पङ्क्ति | OUTPUT_SHOW_AGE_WITH_COLOR);

	output(&sb, output_option);

	यदि (show_stats) {
		म_लिखो("num read blob: %d\n", sb.अङ्क_read_blob);
		म_लिखो("num get patch: %d\n", sb.अङ्क_get_patch);
		म_लिखो("num commits: %d\n", sb.अङ्क_commits);
	}

cleanup:
	लागि (ent = sb.ent; ent; ) {
		संरचना blame_entry *e = ent->next;
		मुक्त(ent);
		ent = e;
	}

	मुक्त(path);
	cleanup_scoreboard(&sb);
	release_revisions(&revs);
	फर्काऊ 0;
}
