/*
 * Blame
 *
 * Copyright (c) 2006, 2014 by its authors
 * See COPYING for licensing conditions
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "color.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "commit.h"
#समावेश "diff.h"
#समावेश "revision.h"
#समावेश "quote.h"
#समावेश "string-list.h"
#समावेश "mailmap.h"
#समावेश "parse-options.h"
#समावेश "prio-queue.h"
#समावेश "utf8.h"
#समावेश "userdiff.h"
#समावेश "line-range.h"
#समावेश "line-log.h"
#समावेश "progress.h"
#समावेश "object-name.h"
#समावेश "odb.h"
#समावेश "pager.h"
#समावेश "blame.h"
#समावेश "refs.h"
#समावेश "setup.h"
#समावेश "tag.h"
#समावेश "write-or-die.h"

अटल स्थिर अक्षर blame_usage[] = N_("git blame [<options>] [<rev-opts>] [<rev>] [--] <file>");
अटल स्थिर अक्षर annotate_usage[] = N_("git annotate [<options>] [<rev-opts>] [<rev>] [--] <file>");

अटल स्थिर अक्षर *स्थिर blame_opt_usage[] = {
	blame_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	शून्य
};

अटल स्थिर अक्षर *स्थिर annotate_opt_usage[] = {
	annotate_usage,
	"",
	N_("<rev-opts> are documented in git-rev-list(1)"),
	शून्य
};

अटल पूर्णांक longest_file;
अटल पूर्णांक longest_author;
अटल पूर्णांक max_orig_digits;
अटल पूर्णांक max_digits;
अटल पूर्णांक max_score_digits;
अटल पूर्णांक show_मूल;
अटल पूर्णांक reverse;
अटल पूर्णांक blank_boundary;
अटल पूर्णांक incremental;
अटल पूर्णांक xdl_opts;
अटल पूर्णांक abbrev = -1;
अटल पूर्णांक no_whole_file_rename;
अटल पूर्णांक show_progress;
अटल अक्षर repeated_meta_color[COLOR_MAXLEN];
अटल पूर्णांक coloring_mode;
अटल काष्ठा string_list ignore_revs_file_list = STRING_LIST_INIT_DUP;
अटल पूर्णांक mark_unblamable_पंक्तियाँ;
अटल पूर्णांक mark_ignored_पंक्तियाँ;

अटल काष्ठा date_mode blame_date_mode = { DATE_ISO8601 };
अटल माप_प्रकार blame_date_width;

अटल काष्ठा string_list mailmap = STRING_LIST_INIT_NODUP;

#अगर_अघोषित DEBUG_BLAME
#घोषणा DEBUG_BLAME 0
#पूर्ण_अगर

अटल अचिन्हित blame_move_score;
अटल अचिन्हित blame_copy_score;

/* Remember to update object flag allocation in object.h */
#घोषणा METAINFO_SHOWN		(1u<<12)
#घोषणा MORE_THAN_ONE_PATH	(1u<<13)

काष्ठा progress_info {
	काष्ठा progress *progress;
	पूर्णांक blamed_पंक्तियाँ;
};

अटल स्थिर अक्षर *nth_पंक्ति_cb(व्योम *तथ्य, दीर्घ lno)
{
	वापस blame_nth_पंक्ति((काष्ठा blame_scoreboard *)तथ्य, lno);
}

/*
 * Information on commits, used for output.
 */
काष्ठा commit_info {
	काष्ठा strbuf author;
	काष्ठा strbuf author_mail;
	timestamp_t author_time;
	काष्ठा strbuf author_tz;

	/* filled only when asked for details */
	काष्ठा strbuf committer;
	काष्ठा strbuf committer_mail;
	timestamp_t committer_time;
	काष्ठा strbuf committer_tz;

	काष्ठा strbuf summary;
};

#घोषणा COMMIT_INFO_INIT { \
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
अटल व्योम get_ac_पंक्ति(स्थिर अक्षर *inbuf, स्थिर अक्षर *what,
	काष्ठा strbuf *name, काष्ठा strbuf *mail,
	timestamp_t *समय, काष्ठा strbuf *tz)
{
	काष्ठा ident_split ident;
	माप_प्रकार len, maillen, namelen;
	स्थिर अक्षर *tmp, *endp;
	स्थिर अक्षर *namebuf, *mailbuf;

	tmp = म_माला(inbuf, what);
	अगर (!tmp)
		जाओ error_out;
	tmp += म_माप(what);
	endp = म_अक्षर(tmp, '\n');
	अगर (!endp)
		len = म_माप(tmp);
	अन्यथा
		len = endp - tmp;

	अगर (split_ident_पंक्ति(&ident, tmp, len)) {
	error_out:
		/* Ugh */
		tmp = "(unknown)";
		strbuf_addstr(name, tmp);
		strbuf_addstr(mail, tmp);
		strbuf_addstr(tz, tmp);
		*समय = 0;
		वापस;
	}

	namelen = ident.name_end - ident.name_begin;
	namebuf = ident.name_begin;

	maillen = ident.mail_end - ident.mail_begin;
	mailbuf = ident.mail_begin;

	अगर (ident.date_begin && ident.date_end)
		*समय = म_से_अदीर्घ(ident.date_begin, शून्य, 10);
	अन्यथा
		*समय = 0;

	अगर (ident.tz_begin && ident.tz_end)
		strbuf_add(tz, ident.tz_begin, ident.tz_end - ident.tz_begin);
	अन्यथा
		strbuf_addstr(tz, "(unknown)");

	/*
	 * Now, convert both name and e-mail using mailmap
	 */
	map_user(&mailmap, &mailbuf, &maillen,
		 &namebuf, &namelen);

	strbuf_addf(mail, "<%.*s>", (पूर्णांक)maillen, mailbuf);
	strbuf_add(name, namebuf, namelen);
}

अटल व्योम commit_info_destroy(काष्ठा commit_info *ci)
{

	strbuf_release(&ci->author);
	strbuf_release(&ci->author_mail);
	strbuf_release(&ci->author_tz);
	strbuf_release(&ci->committer);
	strbuf_release(&ci->committer_mail);
	strbuf_release(&ci->committer_tz);
	strbuf_release(&ci->summary);
}

अटल व्योम get_commit_info(काष्ठा commit *commit, काष्ठा commit_info *ret)
{
	पूर्णांक len;
	स्थिर अक्षर *subject, *encoding;
	स्थिर अक्षर *message;

	encoding = get_log_output_encoding();
	message = repo_logmsg_reencode(the_repository, commit, शून्य, encoding);
	get_ac_पंक्ति(message, "\nauthor ",
		    &ret->author, &ret->author_mail,
		    &ret->author_time, &ret->author_tz);

	get_ac_पंक्ति(message, "\ncommitter ",
		    &ret->committer, &ret->committer_mail,
		    &ret->committer_time, &ret->committer_tz);

	len = find_commit_subject(message, &subject);
	अगर (len)
		strbuf_add(&ret->summary, subject, len);
	अन्यथा
		strbuf_addf(&ret->summary, "(%s)", oid_to_hex(&commit->object.oid));

	repo_unuse_commit_बफर(the_repository, commit, message);
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
अटल व्योम write_filename_info(काष्ठा blame_origin *suspect)
{
	अगर (suspect->previous) {
		काष्ठा blame_origin *पूर्व = suspect->previous;
		म_लिखो("previous %s ", oid_to_hex(&पूर्व->commit->object.oid));
		write_name_quoted(पूर्व->path, मानक_निकास, '\n');
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
अटल पूर्णांक emit_one_suspect_detail(काष्ठा blame_origin *suspect, पूर्णांक repeat)
{
	काष्ठा commit_info ci = COMMIT_INFO_INIT;

	अगर (!repeat && (suspect->commit->object.flags & METAINFO_SHOWN))
		वापस 0;

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
	अगर (suspect->commit->object.flags & UNINTERESTING)
		म_लिखो("boundary\n");

	commit_info_destroy(&ci);

	वापस 1;
}

/*
 * The blame_entry is found to be guilty for the range.
 * Show it in incremental output.
 */
अटल व्योम found_guilty_entry(काष्ठा blame_entry *ent, व्योम *तथ्य)
{
	काष्ठा progress_info *pi = (काष्ठा progress_info *)तथ्य;

	अगर (incremental) {
		काष्ठा blame_origin *suspect = ent->suspect;

		म_लिखो("%s %d %d %d\n",
		       oid_to_hex(&suspect->commit->object.oid),
		       ent->s_lno + 1, ent->lno + 1, ent->अंक_पंक्तियाँ);
		emit_one_suspect_detail(suspect, 0);
		write_filename_info(suspect);
		maybe_flush_or_die(मानक_निकास, "stdout");
	}
	pi->blamed_पंक्तियाँ += ent->अंक_पंक्तियाँ;
	display_progress(pi->progress, pi->blamed_पंक्तियाँ);
}

अटल स्थिर अक्षर *format_time(timestamp_t समय, स्थिर अक्षर *tz_str,
			       पूर्णांक show_raw_time)
{
	अटल काष्ठा strbuf time_buf = STRBUF_INIT;

	strbuf_reset(&time_buf);
	अगर (show_raw_time) {
		strbuf_addf(&time_buf, "%"PRItime" %s", समय, tz_str);
	}
	अन्यथा {
		स्थिर अक्षर *time_str;
		माप_प्रकार time_width;
		पूर्णांक tz;
		tz = म_से_प(tz_str);
		time_str = show_date(समय, tz, blame_date_mode);
		strbuf_addstr(&time_buf, time_str);
		/*
		 * Add space paddings to time_buf to display a fixed width
		 * string, and use time_width for display width calibration.
		 */
		क्रम (time_width = utf8_strwidth(time_str);
		     time_width < blame_date_width;
		     time_width++)
			strbuf_addch(&time_buf, ' ');
	}
	वापस time_buf.buf;
}

#घोषणा OUTPUT_ANNOTATE_COMPAT      (1U<<0)
#घोषणा OUTPUT_LONG_OBJECT_NAME     (1U<<1)
#घोषणा OUTPUT_RAW_TIMESTAMP        (1U<<2)
#घोषणा OUTPUT_PORCELAIN            (1U<<3)
#घोषणा OUTPUT_SHOW_NAME            (1U<<4)
#घोषणा OUTPUT_SHOW_संख्या          (1U<<5)
#घोषणा OUTPUT_SHOW_SCORE           (1U<<6)
#घोषणा OUTPUT_NO_AUTHOR            (1U<<7)
#घोषणा OUTPUT_SHOW_EMAIL           (1U<<8)
#घोषणा OUTPUT_पंक्ति_PORCELAIN       (1U<<9)
#घोषणा OUTPUT_COLOR_पंक्ति           (1U<<10)
#घोषणा OUTPUT_SHOW_AGE_WITH_COLOR  (1U<<11)

अटल व्योम emit_porcelain_details(काष्ठा blame_origin *suspect, पूर्णांक repeat)
{
	अगर (emit_one_suspect_detail(suspect, repeat) ||
	    (suspect->commit->object.flags & MORE_THAN_ONE_PATH))
		write_filename_info(suspect);
}

/*
 * Information which needs to be printed per-line goes here. Any
 * information which can be clubbed on a commit/file level, should
 * be printed via 'emit_one_suspect_detail()'.
 */
अटल व्योम emit_porcelain_per_पंक्ति_details(काष्ठा blame_entry *ent)
{
	अगर (mark_unblamable_पंक्तियाँ && ent->unblamable)
		माला_दो("unblamable");
	अगर (mark_ignored_पंक्तियाँ && ent->ignored)
		माला_दो("ignored");
}

अटल व्योम emit_porcelain(काष्ठा blame_scoreboard *sb, काष्ठा blame_entry *ent,
			   पूर्णांक opt)
{
	पूर्णांक repeat = opt & OUTPUT_पंक्ति_PORCELAIN;
	पूर्णांक cnt;
	स्थिर अक्षर *cp;
	काष्ठा blame_origin *suspect = ent->suspect;
	अक्षर hex[GIT_MAX_HEXSZ + 1];

	oid_to_hex_r(hex, &suspect->commit->object.oid);
	म_लिखो("%s %d %d %d\n",
	       hex,
	       ent->s_lno + 1,
	       ent->lno + 1,
	       ent->अंक_पंक्तियाँ);
	emit_porcelain_details(suspect, repeat);
	emit_porcelain_per_पंक्ति_details(ent);

	cp = blame_nth_पंक्ति(sb, ent->lno);
	क्रम (cnt = 0; cnt < ent->अंक_पंक्तियाँ; cnt++) {
		अक्षर ch;
		अगर (cnt) {
			म_लिखो("%s %d %d\n", hex,
			       ent->s_lno + 1 + cnt,
			       ent->lno + 1 + cnt);
			अगर (repeat)
				emit_porcelain_details(suspect, 1);
			emit_porcelain_per_पंक्ति_details(ent);
		}
		अक्षर_दो('\t');
		करो {
			ch = *cp++;
			अक्षर_दो(ch);
		} जबतक (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_आकार);
	}

	अगर (sb->final_buf_आकार && cp[-1] != '\n')
		अक्षर_दो('\n');
}

अटल काष्ठा color_field {
	timestamp_t hop;
	अक्षर col[COLOR_MAXLEN];
} *colorfield;
अटल पूर्णांक colorfield_nr, colorfield_alloc;

अटल व्योम parse_color_fields(स्थिर अक्षर *s)
{
	काष्ठा string_list l = STRING_LIST_INIT_DUP;
	काष्ठा string_list_item *item;
	क्रमागत { EXPECT_DATE, EXPECT_COLOR } अग्र = EXPECT_COLOR;

	colorfield_nr = 0;

	/* Ideally this would be stripped and split at the same time? */
	string_list_split(&l, s, ",", -1);
	ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);

	for_each_string_list_item(item, &l) {
		चयन (अग्र) {
		हाल EXPECT_DATE:
			colorfield[colorfield_nr].hop = approxidate(item->string);
			अग्र = EXPECT_COLOR;
			colorfield_nr++;
			ALLOC_GROW(colorfield, colorfield_nr + 1, colorfield_alloc);
			अवरोध;
		हाल EXPECT_COLOR:
			अगर (color_parse(item->string, colorfield[colorfield_nr].col))
				die(_("expecting a color: %s"), item->string);
			अग्र = EXPECT_DATE;
			अवरोध;
		}
	}

	अगर (अग्र == EXPECT_COLOR)
		die(_("must end with a color"));

	colorfield[colorfield_nr].hop = TIME_MAX;
	string_list_clear(&l, 0);
}

अटल व्योम setup_default_color_by_age(व्योम)
{
	parse_color_fields("blue,12 month ago,white,1 month ago,red");
}

अटल व्योम determine_पंक्ति_heat(काष्ठा commit_info *ci, स्थिर अक्षर **dest_color)
{
	पूर्णांक i = 0;

	जबतक (i < colorfield_nr && ci->author_time > colorfield[i].hop)
		i++;

	*dest_color = colorfield[i].col;
}

अटल अंतरभूत पूर्णांक maybe_putc(पूर्णांक c, खाता *out)
{
	वापस out ? अ_दो(c, out) : 0;
}

अटल माप_प्रकार print_marks(खाता *out, स्थिर काष्ठा blame_entry *ent, पूर्णांक opt)
{
	माप_प्रकार len = 0;

	अगर ((ent->suspect->commit->object.flags & UNINTERESTING) &&
	    !blank_boundary && !(opt & OUTPUT_ANNOTATE_COMPAT)) {
		maybe_putc('^', out);
		len++;
	}
	अगर (mark_unblamable_पंक्तियाँ && ent->unblamable) {
		maybe_putc('*', out);
		len++;
	}
	अगर (mark_ignored_पंक्तियाँ && ent->ignored) {
		maybe_putc('?', out);
		len++;
	}
	वापस len;
}

अटल माप_प्रकार गिनती_marks(स्थिर काष्ठा blame_entry *ent, पूर्णांक opt)
{
	वापस print_marks(शून्य, ent, opt);
}

अटल व्योम emit_other(काष्ठा blame_scoreboard *sb, काष्ठा blame_entry *ent,
		       पूर्णांक opt, काष्ठा blame_entry *पूर्व_ent)
{
	पूर्णांक cnt;
	स्थिर अक्षर *cp;
	काष्ठा blame_origin *suspect = ent->suspect;
	काष्ठा commit_info ci = COMMIT_INFO_INIT;
	अक्षर hex[GIT_MAX_HEXSZ + 1];
	पूर्णांक show_raw_time = !!(opt & OUTPUT_RAW_TIMESTAMP);
	स्थिर अक्षर *default_color = शून्य, *color = शून्य, *reset = शून्य;

	get_commit_info(suspect->commit, &ci);
	oid_to_hex_r(hex, &suspect->commit->object.oid);

	cp = blame_nth_पंक्ति(sb, ent->lno);

	अगर (opt & OUTPUT_SHOW_AGE_WITH_COLOR) {
		determine_पंक्ति_heat(&ci, &default_color);
		color = default_color;
		reset = GIT_COLOR_RESET;
	}

	अगर (abbrev < MINIMUM_ABBREV)
		BUG("abbreviation is smaller than minimum length: %d < %d",
		    abbrev, MINIMUM_ABBREV);

	क्रम (cnt = 0; cnt < ent->अंक_पंक्तियाँ; cnt++) {
		अक्षर ch;
		माप_प्रकार लंबाई = (opt & OUTPUT_LONG_OBJECT_NAME) ?
			the_hash_algo->hexsz : (माप_प्रकार) abbrev;

		अगर (opt & OUTPUT_COLOR_पंक्ति) {
			अगर (cnt > 0 ||
			    (पूर्व_ent &&
			     oideq(&suspect->commit->object.oid,
				   &पूर्व_ent->suspect->commit->object.oid))) {
				color = repeated_meta_color;
				reset = GIT_COLOR_RESET;
			} अन्यथा  {
				color = default_color ? default_color : शून्य;
				reset = default_color ? GIT_COLOR_RESET : शून्य;
			}
		}
		अगर (color)
			ख_माला_दो(color, मानक_निकास);

		अगर ((suspect->commit->object.flags & UNINTERESTING) &&
		    blank_boundary)
			स_रखो(hex, ' ', म_माप(hex));
		लंबाई -= print_marks(मानक_निकास, ent, opt);

		म_लिखो("%.*s", (पूर्णांक)(लंबाई < GIT_MAX_HEXSZ ? लंबाई : GIT_MAX_HEXSZ), hex);
		अगर (opt & OUTPUT_ANNOTATE_COMPAT) {
			स्थिर अक्षर *name;
			अगर (opt & OUTPUT_SHOW_EMAIL)
				name = ci.author_mail.buf;
			अन्यथा
				name = ci.author.buf;
			म_लिखो("\t(%10s\t%10s\t%d)", name,
			       format_time(ci.author_time, ci.author_tz.buf,
					   show_raw_time),
			       ent->lno + 1 + cnt);
		} अन्यथा {
			अगर (opt & OUTPUT_SHOW_SCORE)
				म_लिखो(" %*d %02d",
				       max_score_digits, ent->score,
				       ent->suspect->refcnt);
			अगर (opt & OUTPUT_SHOW_NAME)
				म_लिखो(" %-*.*s", longest_file, longest_file,
				       suspect->path);
			अगर (opt & OUTPUT_SHOW_संख्या)
				म_लिखो(" %*d", max_orig_digits,
				       ent->s_lno + 1 + cnt);

			अगर (!(opt & OUTPUT_NO_AUTHOR)) {
				स्थिर अक्षर *name;
				पूर्णांक pad;
				अगर (opt & OUTPUT_SHOW_EMAIL)
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
		अगर (reset)
			ख_माला_दो(reset, मानक_निकास);
		करो {
			ch = *cp++;
			अक्षर_दो(ch);
		} जबतक (ch != '\n' &&
			 cp < sb->final_buf + sb->final_buf_आकार);
	}

	अगर (sb->final_buf_आकार && cp[-1] != '\n')
		अक्षर_दो('\n');

	commit_info_destroy(&ci);
}

अटल व्योम output(काष्ठा blame_scoreboard *sb, पूर्णांक option)
{
	काष्ठा blame_entry *ent, *पूर्व_ent = शून्य;

	अगर (option & OUTPUT_PORCELAIN) {
		क्रम (ent = sb->ent; ent; ent = ent->अग्र) {
			पूर्णांक गिनती = 0;
			काष्ठा blame_origin *suspect;
			काष्ठा commit *commit = ent->suspect->commit;
			अगर (commit->object.flags & MORE_THAN_ONE_PATH)
				जारी;
			क्रम (suspect = get_blame_suspects(commit); suspect; suspect = suspect->अग्र) {
				अगर (suspect->guilty && गिनती++) {
					commit->object.flags |= MORE_THAN_ONE_PATH;
					अवरोध;
				}
			}
		}
	}

	क्रम (ent = sb->ent; ent; ent = ent->अग्र) {
		अगर (option & OUTPUT_PORCELAIN)
			emit_porcelain(sb, ent, option);
		अन्यथा {
			emit_other(sb, ent, option, पूर्व_ent);
			पूर्व_ent = ent;
		}
	}
}

/*
 * Add phony grafts for use with -S; this is primarily to
 * support git's cvsserver that wants to give a linear history
 * to its clients.
 */
अटल पूर्णांक read_ancestry(स्थिर अक्षर *graft_file)
{
	खाता *fp = fopen_or_warn(graft_file, "r");
	काष्ठा strbuf buf = STRBUF_INIT;
	अगर (!fp)
		वापस -1;
	जबतक (!strbuf_getwholeline(&buf, fp, '\n')) {
		/* The format is just "Commit Parent1 Parent2 ...\n" */
		काष्ठा commit_graft *graft = read_graft_पंक्ति(&buf);
		अगर (graft)
			register_commit_graft(the_repository, graft, 0);
	}
	ख_बंद(fp);
	strbuf_release(&buf);
	वापस 0;
}

अटल पूर्णांक update_auto_abbrev(पूर्णांक auto_abbrev, काष्ठा blame_origin *suspect)
{
	स्थिर अक्षर *uniq = repo_find_unique_abbrev(the_repository,
						   &suspect->commit->object.oid,
						   auto_abbrev);
	पूर्णांक len = म_माप(uniq);
	अगर (auto_abbrev < len)
		वापस len;
	वापस auto_abbrev;
}

/*
 * How many columns do we need to show line numbers, authors,
 * and filenames?
 */
अटल व्योम find_alignment(काष्ठा blame_scoreboard *sb, पूर्णांक *option)
{
	पूर्णांक longest_src_पंक्तियाँ = 0;
	पूर्णांक longest_dst_पंक्तियाँ = 0;
	अचिन्हित largest_score = 0;
	काष्ठा blame_entry *e;
	पूर्णांक compute_auto_abbrev = (abbrev < 0);
	पूर्णांक auto_abbrev = DEFAULT_ABBREV;
	माप_प्रकार max_marks_गिनती = 0;

	क्रम (e = sb->ent; e; e = e->अग्र) {
		काष्ठा blame_origin *suspect = e->suspect;
		पूर्णांक अंक;
		माप_प्रकार marks_गिनती = गिनती_marks(e, *option);

		अगर (max_marks_गिनती < marks_गिनती)
			max_marks_गिनती = marks_गिनती;
		अगर (compute_auto_abbrev)
			auto_abbrev = update_auto_abbrev(auto_abbrev, suspect);
		अगर (म_भेद(suspect->path, sb->path))
			*option |= OUTPUT_SHOW_NAME;
		अंक = म_माप(suspect->path);
		अगर (longest_file < अंक)
			longest_file = अंक;
		अगर (!(suspect->commit->object.flags & METAINFO_SHOWN)) {
			काष्ठा commit_info ci = COMMIT_INFO_INIT;
			suspect->commit->object.flags |= METAINFO_SHOWN;
			get_commit_info(suspect->commit, &ci);
			अगर (*option & OUTPUT_SHOW_EMAIL)
				अंक = utf8_strwidth(ci.author_mail.buf);
			अन्यथा
				अंक = utf8_strwidth(ci.author.buf);
			अगर (longest_author < अंक)
				longest_author = अंक;
			commit_info_destroy(&ci);
		}
		अंक = e->s_lno + e->अंक_पंक्तियाँ;
		अगर (longest_src_पंक्तियाँ < अंक)
			longest_src_पंक्तियाँ = अंक;
		अंक = e->lno + e->अंक_पंक्तियाँ;
		अगर (longest_dst_पंक्तियाँ < अंक)
			longest_dst_पंक्तियाँ = अंक;
		अगर (largest_score < blame_entry_score(sb, e))
			largest_score = blame_entry_score(sb, e);
	}
	max_orig_digits = decimal_width(longest_src_पंक्तियाँ);
	max_digits = decimal_width(longest_dst_पंक्तियाँ);
	max_score_digits = decimal_width(largest_score);

	अगर (compute_auto_abbrev)
		abbrev = auto_abbrev;
	अगर (abbrev < (पूर्णांक)the_hash_algo->hexsz) {
		abbrev += max_marks_गिनती;
		अगर (abbrev > (पूर्णांक)the_hash_algo->hexsz)
			abbrev = the_hash_algo->hexsz;
	}
}

अटल व्योम sanity_check_on_fail(काष्ठा blame_scoreboard *sb, पूर्णांक baa)
{
	पूर्णांक opt = OUTPUT_SHOW_SCORE | OUTPUT_SHOW_संख्या | OUTPUT_SHOW_NAME;
	find_alignment(sb, &opt);
	output(sb, opt);
	die("Baa %d!", baa);
}

अटल अचिन्हित parse_score(स्थिर अक्षर *arg)
{
	अक्षर *end;
	अचिन्हित दीर्घ score = म_से_अदीर्घ(arg, &end, 10);
	अगर (*end)
		वापस 0;
	वापस score;
}

अटल अक्षर *add_prefix(स्थिर अक्षर *prefix, स्थिर अक्षर *path)
{
	वापस prefix_path(the_repository, prefix, prefix ? म_माप(prefix) : 0, path);
}

अटल पूर्णांक git_blame_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
			    स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	अगर (!म_भेद(var, "blame.showroot")) {
		show_मूल = git_config_bool(var, मूल्य);
		वापस 0;
	}
	अगर (!म_भेद(var, "blame.blankboundary")) {
		blank_boundary = git_config_bool(var, मूल्य);
		वापस 0;
	}
	अगर (!म_भेद(var, "blame.showemail")) {
		पूर्णांक *output_option = cb;
		अगर (git_config_bool(var, मूल्य))
			*output_option |= OUTPUT_SHOW_EMAIL;
		अन्यथा
			*output_option &= ~OUTPUT_SHOW_EMAIL;
		वापस 0;
	}
	अगर (!म_भेद(var, "blame.date")) {
		अगर (!मूल्य)
			वापस config_error_nonbool(var);
		parse_date_format(मूल्य, &blame_date_mode);
		वापस 0;
	}
	अगर (!म_भेद(var, "blame.ignorerevsfile")) {
		अक्षर *str;
		पूर्णांक ret;

		ret = git_config_pathname(&str, var, मूल्य);
		अगर (ret)
			वापस ret;
		अगर (str)
			string_list_जोड़ो(&ignore_revs_file_list, str);
		मुक्त(str);
		वापस 0;
	}
	अगर (!म_भेद(var, "blame.markunblamablelines")) {
		mark_unblamable_पंक्तियाँ = git_config_bool(var, मूल्य);
		वापस 0;
	}
	अगर (!म_भेद(var, "blame.markignoredlines")) {
		mark_ignored_पंक्तियाँ = git_config_bool(var, मूल्य);
		वापस 0;
	}
	अगर (!म_भेद(var, "color.blame.repeatedlines")) {
		अगर (color_parse_mem(मूल्य, म_माप(मूल्य), repeated_meta_color))
			warning(_("invalid value for '%s': '%s'"),
				"color.blame.repeatedLines", मूल्य);
		वापस 0;
	}
	अगर (!म_भेद(var, "color.blame.highlightrecent")) {
		parse_color_fields(मूल्य);
		वापस 0;
	}

	अगर (!म_भेद(var, "blame.coloring")) {
		अगर (!मूल्य)
			वापस config_error_nonbool(var);
		अगर (!म_भेद(मूल्य, "repeatedLines")) {
			coloring_mode |= OUTPUT_COLOR_पंक्ति;
		} अन्यथा अगर (!म_भेद(मूल्य, "highlightRecent")) {
			coloring_mode |= OUTPUT_SHOW_AGE_WITH_COLOR;
		} अन्यथा अगर (!म_भेद(मूल्य, "none")) {
			coloring_mode &= ~(OUTPUT_COLOR_पंक्ति |
					    OUTPUT_SHOW_AGE_WITH_COLOR);
		} अन्यथा {
			warning(_("invalid value for '%s': '%s'"),
				"blame.coloring", मूल्य);
			वापस 0;
		}
	}

	अगर (!म_भेद(var, "diff.algorithm")) {
		दीर्घ diff_algorithm;
		अगर (!मूल्य)
			वापस config_error_nonbool(var);
		diff_algorithm = parse_algorithm_मूल्य(मूल्य);
		अगर (diff_algorithm < 0)
			वापस error(_("unknown value for config '%s': %s"),
				     var, मूल्य);
		xdl_opts &= ~XDF_DIFF_ALGORITHM_MASK;
		xdl_opts |= diff_algorithm;
		वापस 0;
	}

	अगर (git_diff_heuristic_config(var, मूल्य, cb) < 0)
		वापस -1;
	अगर (userdiff_config(var, मूल्य) < 0)
		वापस -1;

	वापस git_default_config(var, मूल्य, ctx, cb);
}

अटल पूर्णांक blame_copy_callback(स्थिर काष्ठा option *option, स्थिर अक्षर *arg, पूर्णांक unset)
{
	पूर्णांक *opt = option->मूल्य;

	BUG_ON_OPT_NEG(unset);

	/*
	 * -C enables copy from removed files;
	 * -C -C enables copy from existing files, but only
	 *       when blaming a new file;
	 * -C -C -C enables copy from existing files for
	 *          everybody
	 */
	अगर (*opt & PICKAXE_BLAME_COPY_HARDER)
		*opt |= PICKAXE_BLAME_COPY_HARDEST;
	अगर (*opt & PICKAXE_BLAME_COPY)
		*opt |= PICKAXE_BLAME_COPY_HARDER;
	*opt |= PICKAXE_BLAME_COPY | PICKAXE_BLAME_MOVE;

	अगर (arg)
		blame_copy_score = parse_score(arg);
	वापस 0;
}

अटल पूर्णांक blame_move_callback(स्थिर काष्ठा option *option, स्थिर अक्षर *arg, पूर्णांक unset)
{
	पूर्णांक *opt = option->मूल्य;

	BUG_ON_OPT_NEG(unset);

	*opt |= PICKAXE_BLAME_MOVE;

	अगर (arg)
		blame_move_score = parse_score(arg);
	वापस 0;
}

अटल पूर्णांक blame_diff_algorithm_minimal(स्थिर काष्ठा option *option,
					स्थिर अक्षर *arg, पूर्णांक unset)
{
	पूर्णांक *opt = option->मूल्य;

	BUG_ON_OPT_ARG(arg);

	*opt &= ~XDF_DIFF_ALGORITHM_MASK;
	अगर (!unset)
		*opt |= XDF_NEED_MINIMAL;

	वापस 0;
}

अटल पूर्णांक blame_diff_algorithm_callback(स्थिर काष्ठा option *option,
					 स्थिर अक्षर *arg, पूर्णांक unset)
{
	पूर्णांक *opt = option->मूल्य;
	दीर्घ मूल्य = parse_algorithm_मूल्य(arg);

	BUG_ON_OPT_NEG(unset);

	अगर (मूल्य < 0)
		वापस error(_("option diff-algorithm accepts \"myers\", "
			       "\"minimal\", \"patience\" and \"histogram\""));

	*opt &= ~XDF_DIFF_ALGORITHM_MASK;
	*opt |= मूल्य;

	वापस 0;
}

अटल पूर्णांक is_a_rev(स्थिर अक्षर *name)
{
	काष्ठा object_id oid;

	अगर (repo_get_oid(the_repository, name, &oid))
		वापस 0;
	वापस OBJ_NONE < odb_read_object_info(the_repository->objects, &oid, शून्य);
}

अटल पूर्णांक peel_to_commit_oid(काष्ठा object_id *oid_ret, व्योम *cbdata)
{
	काष्ठा repository *r = ((काष्ठा blame_scoreboard *)cbdata)->repo;
	काष्ठा object_id oid;

	oidcpy(&oid, oid_ret);
	जबतक (1) {
		काष्ठा object *obj;
		पूर्णांक kind = odb_read_object_info(r->objects, &oid, शून्य);
		अगर (kind == OBJ_COMMIT) {
			oidcpy(oid_ret, &oid);
			वापस 0;
		}
		अगर (kind != OBJ_TAG)
			वापस -1;
		obj = deref_tag(r, parse_object(r, &oid), शून्य, 0);
		अगर (!obj)
			वापस -1;
		oidcpy(&oid, &obj->oid);
	}
}

अटल व्योम build_ignorelist(काष्ठा blame_scoreboard *sb,
			     काष्ठा string_list *ignore_revs_file_list,
			     काष्ठा string_list *ignore_rev_list)
{
	काष्ठा string_list_item *i;
	काष्ठा object_id oid;

	oidset_init(&sb->ignore_list, 0);
	for_each_string_list_item(i, ignore_revs_file_list) {
		अगर (!म_भेद(i->string, ""))
			oidset_clear(&sb->ignore_list);
		अन्यथा
			oidset_parse_file_carefully(&sb->ignore_list, i->string,
						    the_repository->hash_algo,
						    peel_to_commit_oid, sb);
	}
	for_each_string_list_item(i, ignore_rev_list) {
		अगर (repo_get_oid_committish(the_repository, i->string, &oid) ||
		    peel_to_commit_oid(&oid, sb))
			die(_("cannot find revision %s to ignore"), i->string);
		oidset_जोड़ो(&sb->ignore_list, &oid);
	}
}

पूर्णांक cmd_blame(पूर्णांक argc,
	      स्थिर अक्षर **argv,
	      स्थिर अक्षर *prefix,
	      काष्ठा repository *repo UNUSED)
{
	काष्ठा rev_info revs;
	अक्षर *path = शून्य;
	काष्ठा blame_scoreboard sb;
	काष्ठा blame_origin *o;
	काष्ठा blame_entry *ent = शून्य;
	दीर्घ dashdash_pos, lno;
	काष्ठा progress_info pi = { शून्य, 0 };

	काष्ठा string_list range_list = STRING_LIST_INIT_NODUP;
	काष्ठा string_list ignore_rev_list = STRING_LIST_INIT_NODUP;
	पूर्णांक output_option = 0, opt = 0;
	पूर्णांक show_stats = 0;
	स्थिर अक्षर *revs_file = शून्य;
	स्थिर अक्षर *contents_from = शून्य;
	स्थिर काष्ठा option options[] = {
		OPT_BOOL(0, "incremental", &incremental, N_("show blame entries as we find them, incrementally")),
		OPT_BOOL('b', शून्य, &blank_boundary, N_("do not show object names of boundary commits (Default: off)")),
		OPT_BOOL(0, "root", &show_मूल, N_("do not treat root commits as boundaries (Default: off)")),
		OPT_BOOL(0, "show-stats", &show_stats, N_("show work cost statistics")),
		OPT_BOOL(0, "progress", &show_progress, N_("force progress reporting")),
		OPT_BIT(0, "score-debug", &output_option, N_("show output score for blame entries"), OUTPUT_SHOW_SCORE),
		OPT_BIT('f', "show-name", &output_option, N_("show original filename (Default: auto)"), OUTPUT_SHOW_NAME),
		OPT_BIT('n', "show-number", &output_option, N_("show original linenumber (Default: off)"), OUTPUT_SHOW_संख्या),
		OPT_BIT('p', "porcelain", &output_option, N_("show in a format designed for machine consumption"), OUTPUT_PORCELAIN),
		OPT_BIT(0, "line-porcelain", &output_option, N_("show porcelain format with per-line commit information"), OUTPUT_PORCELAIN|OUTPUT_पंक्ति_PORCELAIN),
		OPT_BIT('c', शून्य, &output_option, N_("use the same output mode as git-annotate (Default: off)"), OUTPUT_ANNOTATE_COMPAT),
		OPT_BIT('t', शून्य, &output_option, N_("show raw timestamp (Default: off)"), OUTPUT_RAW_TIMESTAMP),
		OPT_BIT('l', शून्य, &output_option, N_("show long commit SHA1 (Default: off)"), OUTPUT_LONG_OBJECT_NAME),
		OPT_BIT('s', शून्य, &output_option, N_("suppress author name and timestamp (Default: off)"), OUTPUT_NO_AUTHOR),
		OPT_BIT('e', "show-email", &output_option, N_("show author email instead of name (Default: off)"), OUTPUT_SHOW_EMAIL),
		OPT_BIT('w', शून्य, &xdl_opts, N_("ignore whitespace differences"), XDF_IGNORE_WHITESPACE),
		OPT_CALLBACK_F(0, "diff-algorithm", &xdl_opts, N_("<algorithm>"),
			       N_("choose a diff algorithm"),
			       PARSE_OPT_NONEG, blame_diff_algorithm_callback),
		OPT_STRING_LIST(0, "ignore-rev", &ignore_rev_list, N_("rev"), N_("ignore <rev> when blaming")),
		OPT_STRING_LIST(0, "ignore-revs-file", &ignore_revs_file_list, N_("file"), N_("ignore revisions from <file>")),
		OPT_BIT(0, "color-lines", &output_option, N_("color redundant metadata from previous line differently"), OUTPUT_COLOR_पंक्ति),
		OPT_BIT(0, "color-by-age", &output_option, N_("color lines by age"), OUTPUT_SHOW_AGE_WITH_COLOR),
		OPT_CALLBACK_F(0, "minimal", &xdl_opts, शून्य,
			       N_("spend extra cycles to find a better match"),
			       PARSE_OPT_NOARG | PARSE_OPT_HIDDEN, blame_diff_algorithm_minimal),
		OPT_STRING('S', शून्य, &revs_file, N_("file"), N_("use revisions from <file> instead of calling git-rev-list")),
		OPT_STRING(0, "contents", &contents_from, N_("file"), N_("use <file>'s contents as the final image")),
		OPT_CALLBACK_F('C', शून्य, &opt, N_("score"), N_("find line copies within and across files"), PARSE_OPT_OPTARG, blame_copy_callback),
		OPT_CALLBACK_F('M', शून्य, &opt, N_("score"), N_("find line movements within and across files"), PARSE_OPT_OPTARG, blame_move_callback),
		OPT_STRING_LIST('L', शून्य, &range_list, N_("range"),
				N_("process only line range <start>,<end> or function :<funcname>")),
		OPT__ABBREV(&abbrev),
		OPT_END()
	};

	काष्ठा parse_opt_ctx_t ctx;
	पूर्णांक cmd_is_annotate = !म_भेद(argv[0], "annotate");
	काष्ठा range_set ranges;
	अचिन्हित पूर्णांक range_i;
	दीर्घ anchor;
	दीर्घ अंक_पंक्तियाँ = 0;
	स्थिर अक्षर *str_usage = cmd_is_annotate ? annotate_usage : blame_usage;
	स्थिर अक्षर *स्थिर *opt_usage = cmd_is_annotate ? annotate_opt_usage : blame_opt_usage;

	setup_default_color_by_age();
	repo_config(the_repository, git_blame_config, &output_option);
	repo_init_revisions(the_repository, &revs, शून्य);
	revs.date_mode = blame_date_mode;
	revs.diffopt.flags.allow_textconv = 1;
	revs.diffopt.flags.follow_renames = 1;

	save_commit_बफर = 0;
	dashdash_pos = 0;
	show_progress = -1;

	parse_options_start(&ctx, argc, argv, prefix, options,
			    PARSE_OPT_KEEP_DASHDASH | PARSE_OPT_KEEP_ARGV0);
	क्रम (;;) {
		चयन (parse_options_step(&ctx, options, opt_usage)) {
		हाल PARSE_OPT_NON_OPTION:
		हाल PARSE_OPT_UNKNOWN:
			अवरोध;
		हाल PARSE_OPT_HELP:
			निकास(0);
		हाल PARSE_OPT_HELP_ERROR:
		हाल PARSE_OPT_ERROR:
		हाल PARSE_OPT_SUBCOMMAND:
			निकास(129);
		हाल PARSE_OPT_COMPLETE:
			निकास(0);
		हाल PARSE_OPT_DONE:
			अगर (ctx.argv[0])
				dashdash_pos = ctx.cpidx;
			जाओ parse_done;
		}

		अगर (!म_भेद(ctx.argv[0], "--reverse")) {
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
	the_repository->settings.command_requires_full_सूचकांक = 0;

	अगर (incremental || (output_option & OUTPUT_PORCELAIN)) {
		अगर (show_progress > 0)
			die(_("--progress can't be used with --incremental or porcelain formats"));
		show_progress = 0;
	} अन्यथा अगर (show_progress < 0)
		show_progress = isatty(2);

	अगर (!abbrev)
		abbrev = the_hash_algo->hexsz;

	अगर (revs_file && read_ancestry(revs_file))
		die_errno("reading graft file '%s' failed", revs_file);

	अगर (cmd_is_annotate) {
		output_option |= OUTPUT_ANNOTATE_COMPAT;
		blame_date_mode.type = DATE_ISO8601;
	} अन्यथा {
		blame_date_mode = revs.date_mode;
	}

	/* The maximum width used to show the dates */
	चयन (blame_date_mode.type) {
	हाल DATE_RFC2822:
		blame_date_width = माप("Thu, 19 Oct 2006 16:00:04 -0700");
		अवरोध;
	हाल DATE_ISO8601_STRICT:
		blame_date_width = माप("2006-10-19T16:00:04-07:00");
		अवरोध;
	हाल DATE_ISO8601:
		blame_date_width = माप("2006-10-19 16:00:04 -0700");
		अवरोध;
	हाल DATE_RAW:
		blame_date_width = माप("1161298804 -0700");
		अवरोध;
	हाल DATE_UNIX:
		blame_date_width = माप("1161298804");
		अवरोध;
	हाल DATE_SHORT:
		blame_date_width = माप("2006-10-19");
		अवरोध;
	हाल DATE_RELATIVE:
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
		अवरोध;
	हाल DATE_HUMAN:
		/* If the year is shown, no time is shown */
		blame_date_width = माप("Thu Oct 19 16:00");
		अवरोध;
	हाल DATE_NORMAL:
		blame_date_width = माप("Thu Oct 19 16:00:04 2006 -0700");
		अवरोध;
	हाल DATE_STRFTIME:
		blame_date_width = म_माप(show_date(0, 0, blame_date_mode)) + 1; /* add the null */
		अवरोध;
	}
	blame_date_width -= 1; /* strip the null */

	अगर (revs.diffopt.flags.find_copies_harder)
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
	अगर (dashdash_pos) {
		चयन (argc - dashdash_pos - 1) {
		हाल 2: /* (1b) */
			अगर (argc != 4)
				usage_with_options(opt_usage, options);
			/* reorder for the new way: <rev> -- <path> */
			argv[1] = argv[3];
			argv[3] = argv[2];
			argv[2] = "--";
			/* FALLTHROUGH */
		हाल 1: /* (1a) */
			path = add_prefix(prefix, argv[--argc]);
			argv[argc] = शून्य;
			अवरोध;
		शेष:
			usage_with_options(opt_usage, options);
		}
	} अन्यथा {
		अगर (argc < 2)
			usage_with_options(opt_usage, options);
		अगर (argc == 3 && is_a_rev(argv[argc - 1])) { /* (2b) */
			path = add_prefix(prefix, argv[1]);
			argv[1] = argv[2];
		} अन्यथा {	/* (2a) */
			अगर (argc == 2 && is_a_rev(argv[1]) && !repo_get_work_वृक्ष(the_repository))
				die("missing <path> to blame");
			path = add_prefix(prefix, argv[argc - 1]);
		}
		argv[argc - 1] = "--";
	}

	revs.disable_stdin = 1;
	setup_revisions(argc, argv, &revs, शून्य);
	अगर (!revs.pending.nr && is_bare_repository(the_repository)) {
		काष्ठा commit *शीर्ष_commit;
		काष्ठा object_id शीर्ष_oid;

		अगर (!refs_resolve_ref_unsafe(get_main_ref_store(the_repository), "HEAD", RESOLVE_REF_READING,
					     &शीर्ष_oid, शून्य) ||
		    !(शीर्ष_commit = lookup_commit_reference_gently(revs.repo,
							     &शीर्ष_oid, 1)))
			die("no such ref: HEAD");

		add_pending_object(&revs, &शीर्ष_commit->object, "HEAD");
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
	अगर (!(opt & PICKAXE_BLAME_COPY))
		setup_blame_bloom_तथ्य(&sb);

	lno = sb.अंक_पंक्तियाँ;

	अगर (lno && !range_list.nr)
		string_list_append(&range_list, "1");

	anchor = 1;
	range_set_init(&ranges, range_list.nr);
	क्रम (range_i = 0; range_i < range_list.nr; ++range_i) {
		दीर्घ bottom, शीर्ष_2;
		अगर (parse_range_arg(range_list.वस्तुएँ[range_i].string,
				    nth_पंक्ति_cb, &sb, lno, anchor,
				    &bottom, &शीर्ष_2, sb.path,
				    the_repository->सूचकांक))
			usage(str_usage);
		अगर ((!lno && (शीर्ष_2 || bottom)) || lno < bottom)
			die(Q_("file %s has only %lu line",
			       "file %s has only %lu lines",
			       lno), sb.path, lno);
		अगर (bottom < 1)
			bottom = 1;
		अगर (शीर्ष_2 < 1 || lno < शीर्ष_2)
			शीर्ष_2 = lno;
		bottom--;
		range_set_append_unsafe(&ranges, bottom, शीर्ष_2);
		anchor = शीर्ष_2 + 1;
	}
	sort_and_merge_range_set(&ranges);

	क्रम (range_i = ranges.nr; range_i > 0; --range_i) {
		स्थिर काष्ठा range *r = &ranges.ranges[range_i - 1];
		ent = blame_entry_prepend(ent, r->start, r->end, o);
		अंक_पंक्तियाँ += (r->end - r->start);
	}
	अगर (!अंक_पंक्तियाँ)
		अंक_पंक्तियाँ = sb.अंक_पंक्तियाँ;

	o->suspects = ent;
	prio_queue_put(&sb.commits, o->commit);

	blame_origin_decref(o);

	range_set_release(&ranges);
	string_list_clear(&range_list, 0);

	sb.ent = शून्य;

	अगर (blame_move_score)
		sb.move_score = blame_move_score;
	अगर (blame_copy_score)
		sb.copy_score = blame_copy_score;

	sb.debug = DEBUG_BLAME;
	sb.on_sanity_fail = &sanity_check_on_fail;

	sb.show_मूल = show_मूल;
	sb.xdl_opts = xdl_opts;
	sb.no_whole_file_rename = no_whole_file_rename;

	read_mailmap(the_repository, &mailmap);

	sb.found_guilty_entry = &found_guilty_entry;
	sb.found_guilty_entry_तथ्य = &pi;
	अगर (show_progress)
		pi.progress = start_delayed_progress(the_repository,
						     _("Blaming lines"),
						     अंक_पंक्तियाँ);

	assign_blame(&sb, opt);

	stop_progress(&pi.progress);

	अगर (!incremental)
		setup_pager(the_repository);
	अन्यथा
		जाओ cleanup;

	blame_sort_final(&sb);

	blame_coalesce(&sb);

	अगर (!(output_option & (OUTPUT_COLOR_पंक्ति | OUTPUT_SHOW_AGE_WITH_COLOR)))
		output_option |= coloring_mode;

	अगर (!(output_option & OUTPUT_PORCELAIN)) {
		find_alignment(&sb, &output_option);
		अगर (!*repeated_meta_color &&
		    (output_option & OUTPUT_COLOR_पंक्ति))
			xsnprintf(repeated_meta_color,
				  माप(repeated_meta_color),
				  "%s", GIT_COLOR_CYAN);
	}
	अगर (output_option & OUTPUT_ANNOTATE_COMPAT)
		output_option &= ~(OUTPUT_COLOR_पंक्ति | OUTPUT_SHOW_AGE_WITH_COLOR);

	output(&sb, output_option);

	अगर (show_stats) {
		म_लिखो("num read blob: %d\n", sb.अंक_read_blob);
		म_लिखो("num get patch: %d\n", sb.अंक_get_patch);
		म_लिखो("num commits: %d\n", sb.अंक_commits);
	}

cleanup:
	क्रम (ent = sb.ent; ent; ) {
		काष्ठा blame_entry *e = ent->अग्र;
		मुक्त(ent);
		ent = e;
	}

	मुक्त(path);
	cleanup_scoreboard(&sb);
	release_revisions(&revs);
	वापस 0;
}
