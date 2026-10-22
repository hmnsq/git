/*
 * Builtin "git am"
 *
 * Based on git-am.sh by Junio C Hamano.
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "builtin.h"
#समावेश "abspath.h"
#समावेश "advice.h"
#समावेश "config.h"
#समावेश "editor.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "parse-options.h"
#समावेश "dir.h"
#समावेश "run-command.h"
#समावेश "hook.h"
#समावेश "quote.h"
#समावेश "tempfile.h"
#समावेश "lockfile.h"
#समावेश "cache-tree.h"
#समावेश "refs.h"
#समावेश "commit.h"
#समावेश "diff.h"
#समावेश "unpack-trees.h"
#समावेश "branch.h"
#समावेश "object-name.h"
#समावेश "preload-index.h"
#समावेश "sequencer.h"
#समावेश "revision.h"
#समावेश "merge-ort-wrappers.h"
#समावेश "log-tree.h"
#समावेश "notes-utils.h"
#समावेश "rerere.h"
#समावेश "mailinfo.h"
#समावेश "apply.h"
#समावेश "string-list.h"
#समावेश "pager.h"
#समावेश "path.h"
#समावेश "pretty.h"

/**
 * Returns the length of the first line of msg.
 */
अटल पूर्णांक linelen(स्थिर अक्षर *संदेश)
{
	वापस strchrnul(संदेश, '\n') - संदेश;
}

/**
 * Returns true if `str` consists of only whitespace, false otherwise.
 */
अटल पूर्णांक str_isspace(स्थिर अक्षर *str)
{
	क्रम (; *str; str++)
		अगर (!है_खाली(*str))
			वापस 0;

	वापस 1;
}

क्रमागत patch_format {
	PATCH_FORMAT_UNKNOWN = 0,
	PATCH_FORMAT_MBOX,
	PATCH_FORMAT_STGIT,
	PATCH_FORMAT_STGIT_SERIES,
	PATCH_FORMAT_HG,
	PATCH_FORMAT_MBOXRD
};

क्रमागत keep_type {
	KEEP_FALSE = 0,
	KEEP_TRUE,      /* pass -k flag to git-mailinfo */
	KEEP_NON_PATCH  /* pass -b flag to git-mailinfo */
};

क्रमागत scissors_type {
	SCISSORS_UNSET = -1,
	SCISSORS_FALSE = 0,  /* pass --no-scissors to git-mailinfo */
	SCISSORS_TRUE        /* pass --scissors to git-mailinfo */
};

क्रमागत signoff_type {
	SIGNOFF_FALSE = 0,
	SIGNOFF_TRUE = 1,
	SIGNOFF_EXPLICIT /* --signoff was set on the command-line */
};

क्रमागत resume_type {
	RESUME_FALSE = 0,
	RESUME_APPLY,
	RESUME_RESOLVED,
	RESUME_SKIP,
	RESUME_ABORT,
	RESUME_QUIT,
	RESUME_SHOW_PATCH_RAW,
	RESUME_SHOW_PATCH_DIFF,
	RESUME_ALLOW_EMPTY,
};

क्रमागत empty_action {
	STOP_ON_EMPTY_COMMIT = 0,  /* output errors and stop in the middle of an am session */
	DROP_EMPTY_COMMIT,         /* skip with a notice message, unless "--quiet" has been passed */
	KEEP_EMPTY_COMMIT,         /* keep recording as empty commits */
};

काष्ठा am_state {
	/* state directory path */
	अक्षर *dir;

	/* current and last patch numbers, 1-indexed */
	पूर्णांक cur;
	पूर्णांक last;

	/* commit metadata and message */
	अक्षर *author_name;
	अक्षर *author_email;
	अक्षर *author_date;
	अक्षर *संदेश;
	माप_प्रकार संदेश_len;

	/* when --rebasing, records the original commit the patch came from */
	काष्ठा object_id orig_commit;

	/* number of digits in patch filename */
	पूर्णांक prec;

	/* various operating modes and command line options */
	पूर्णांक interactive;
	पूर्णांक no_verify;
	पूर्णांक threeway;
	पूर्णांक quiet;
	पूर्णांक signoff; /* enum signoff_type */
	पूर्णांक utf8;
	पूर्णांक keep; /* enum keep_type */
	पूर्णांक message_id;
	पूर्णांक scissors; /* enum scissors_type */
	पूर्णांक quoted_cr; /* enum quoted_cr_action */
	पूर्णांक empty_type; /* enum empty_action */
	काष्ठा strvec git_apply_opts;
	स्थिर अक्षर *resolvemsg;
	पूर्णांक committer_date_is_author_date;
	पूर्णांक ignore_date;
	पूर्णांक allow_rerere_autoupdate;
	स्थिर अक्षर *sign_commit;
	पूर्णांक rebasing;
};

/**
 * Initializes am_state with the default values.
 */
अटल व्योम am_state_init(काष्ठा am_state *state)
{
	पूर्णांक gpgsign;

	स_रखो(state, 0, माप(*state));

	state->dir = repo_git_path(the_repository, "rebase-apply");

	state->prec = 4;

	repo_config_get_bool(the_repository, "am.threeway", &state->threeway);

	state->utf8 = 1;

	repo_config_get_bool(the_repository, "am.messageid", &state->message_id);

	state->scissors = SCISSORS_UNSET;
	state->quoted_cr = quoted_cr_unset;

	strvec_init(&state->git_apply_opts);

	अगर (!repo_config_get_bool(the_repository, "commit.gpgsign", &gpgsign))
		state->sign_commit = gpgsign ? "" : शून्य;
}

/**
 * Releases memory allocated by an am_state.
 */
अटल व्योम am_state_release(काष्ठा am_state *state)
{
	मुक्त(state->dir);
	मुक्त(state->author_name);
	मुक्त(state->author_email);
	मुक्त(state->author_date);
	मुक्त(state->संदेश);
	strvec_clear(&state->git_apply_opts);
}

अटल पूर्णांक am_option_parse_quoted_cr(स्थिर काष्ठा option *opt,
				     स्थिर अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);

	अगर (mailinfo_parse_quoted_cr_action(arg, opt->मूल्य) != 0)
		वापस error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	वापस 0;
}

अटल पूर्णांक am_option_parse_empty(स्थिर काष्ठा option *opt,
				     स्थिर अक्षर *arg, पूर्णांक unset)
{
	पूर्णांक *opt_मूल्य = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	अगर (!म_भेद(arg, "stop"))
		*opt_मूल्य = STOP_ON_EMPTY_COMMIT;
	अन्यथा अगर (!म_भेद(arg, "drop"))
		*opt_मूल्य = DROP_EMPTY_COMMIT;
	अन्यथा अगर (!म_भेद(arg, "keep"))
		*opt_मूल्य = KEEP_EMPTY_COMMIT;
	अन्यथा
		वापस error(_("invalid value for '%s': '%s'"), "--empty", arg);

	वापस 0;
}

/**
 * Returns path relative to the am_state directory.
 */
अटल अंतरभूत स्थिर अक्षर *am_path(स्थिर काष्ठा am_state *state, स्थिर अक्षर *path)
{
	वापस mkpath("%s/%s", state->dir, path);
}

/**
 * For convenience to call write_file()
 */
अटल व्योम write_state_text(स्थिर काष्ठा am_state *state,
			     स्थिर अक्षर *name, स्थिर अक्षर *string)
{
	write_file(am_path(state, name), "%s", string);
}

अटल व्योम write_state_गिनती(स्थिर काष्ठा am_state *state,
			      स्थिर अक्षर *name, पूर्णांक मूल्य)
{
	write_file(am_path(state, name), "%d", मूल्य);
}

अटल व्योम write_state_bool(स्थिर काष्ठा am_state *state,
			     स्थिर अक्षर *name, पूर्णांक मूल्य)
{
	write_state_text(state, name, मूल्य ? "t" : "f");
}

/**
 * If state->quiet is false, calls fprintf(fp, fmt, ...), and appends a newline
 * at the end.
 */
__attribute__((format (म_लिखो, 3, 4)))
अटल व्योम say(स्थिर काष्ठा am_state *state, खाता *fp, स्थिर अक्षर *fmt, ...)
{
	बहु_सूची ap;

	बहु_शुरू(ap, fmt);
	अगर (!state->quiet) {
		भख_लिखो(fp, fmt, ap);
		अ_दो('\n', fp);
	}
	बहु_पूर्ण(ap);
}

/**
 * Returns 1 if there is an am session in progress, 0 otherwise.
 */
अटल पूर्णांक am_in_progress(स्थिर काष्ठा am_state *state)
{
	काष्ठा stat st;

	अगर (lstat(state->dir, &st) < 0 || !S_ISDIR(st.st_mode))
		वापस 0;
	अगर (lstat(am_path(state, "last"), &st) || !S_ISREG(st.st_mode))
		वापस 0;
	अगर (lstat(am_path(state, "next"), &st) || !S_ISREG(st.st_mode))
		वापस 0;
	वापस 1;
}

/**
 * Reads the contents of `file` in the `state` directory into `sb`. Returns the
 * number of bytes read on success, -1 if the file does not exist. If `trim` is
 * set, trailing whitespace will be removed.
 */
अटल पूर्णांक read_state_file(काष्ठा strbuf *sb, स्थिर काष्ठा am_state *state,
			स्थिर अक्षर *file, पूर्णांक trim)
{
	strbuf_reset(sb);

	अगर (strbuf_read_file(sb, am_path(state, file), 0) >= 0) {
		अगर (trim)
			strbuf_trim(sb);

		वापस sb->len;
	}

	अगर (त्रुटि_सं == ENOENT)
		वापस -1;

	die_errno(_("could not read '%s'"), am_path(state, file));
}

/**
 * Reads and parses the state directory's "author-script" file, and sets
 * state->author_name, state->author_email and state->author_date accordingly.
 * Returns 0 on success, -1 if the file could not be parsed.
 *
 * The author script is of the format:
 *
 *	GIT_AUTHOR_NAME='$author_name'
 *	GIT_AUTHOR_EMAIL='$author_email'
 *	GIT_AUTHOR_DATE='$author_date'
 *
 * where $author_name, $author_email and $author_date are quoted. We are strict
 * with our parsing, as the file was meant to be eval'd in the old git-am.sh
 * script, and thus if the file differs from what this function expects, it is
 * better to bail out than to do something that the user does not expect.
 */
अटल पूर्णांक read_am_author_script(काष्ठा am_state *state)
{
	स्थिर अक्षर *filename = am_path(state, "author-script");

	निश्चित(!state->author_name);
	निश्चित(!state->author_email);
	निश्चित(!state->author_date);

	वापस read_author_script(filename, &state->author_name,
				  &state->author_email, &state->author_date, 1);
}

/**
 * Saves state->author_name, state->author_email and state->author_date in the
 * state directory's "author-script" file.
 */
अटल व्योम write_author_script(स्थिर काष्ठा am_state *state)
{
	काष्ठा strbuf sb = STRBUF_INIT;

	strbuf_addstr(&sb, "GIT_AUTHOR_NAME=");
	sq_quote_buf(&sb, state->author_name);
	strbuf_addch(&sb, '\n');

	strbuf_addstr(&sb, "GIT_AUTHOR_EMAIL=");
	sq_quote_buf(&sb, state->author_email);
	strbuf_addch(&sb, '\n');

	strbuf_addstr(&sb, "GIT_AUTHOR_DATE=");
	sq_quote_buf(&sb, state->author_date);
	strbuf_addch(&sb, '\n');

	write_state_text(state, "author-script", sb.buf);

	strbuf_release(&sb);
}

/**
 * Reads the commit message from the state directory's "final-commit" file,
 * setting state->msg to its contents and state->msg_len to the length of its
 * contents in bytes.
 *
 * Returns 0 on success, -1 if the file does not exist.
 */
अटल पूर्णांक read_commit_संदेश(काष्ठा am_state *state)
{
	काष्ठा strbuf sb = STRBUF_INIT;

	निश्चित(!state->संदेश);

	अगर (read_state_file(&sb, state, "final-commit", 0) < 0) {
		strbuf_release(&sb);
		वापस -1;
	}

	state->संदेश = strbuf_detach(&sb, &state->संदेश_len);
	वापस 0;
}

/**
 * Saves state->msg in the state directory's "final-commit" file.
 */
अटल व्योम write_commit_संदेश(स्थिर काष्ठा am_state *state)
{
	स्थिर अक्षर *filename = am_path(state, "final-commit");
	write_file_buf(filename, state->संदेश, state->संदेश_len);
}

/**
 * Loads state from disk.
 */
अटल व्योम am_load(काष्ठा am_state *state)
{
	काष्ठा strbuf sb = STRBUF_INIT;

	अगर (read_state_file(&sb, state, "next", 1) < 0)
		BUG("state file 'next' does not exist");
	state->cur = म_से_दीर्घ(sb.buf, शून्य, 10);

	अगर (read_state_file(&sb, state, "last", 1) < 0)
		BUG("state file 'last' does not exist");
	state->last = म_से_दीर्घ(sb.buf, शून्य, 10);

	अगर (read_am_author_script(state) < 0)
		die(_("could not parse author script"));

	read_commit_संदेश(state);

	अगर (read_state_file(&sb, state, "original-commit", 1) < 0)
		oidclr(&state->orig_commit, the_repository->hash_algo);
	अन्यथा अगर (get_oid_hex(sb.buf, &state->orig_commit) < 0)
		die(_("could not parse %s"), am_path(state, "original-commit"));

	read_state_file(&sb, state, "threeway", 1);
	state->threeway = !म_भेद(sb.buf, "t");

	read_state_file(&sb, state, "quiet", 1);
	state->quiet = !म_भेद(sb.buf, "t");

	read_state_file(&sb, state, "sign", 1);
	state->signoff = !म_भेद(sb.buf, "t");

	read_state_file(&sb, state, "utf8", 1);
	state->utf8 = !म_भेद(sb.buf, "t");

	अगर (file_exists(am_path(state, "rerere-autoupdate"))) {
		read_state_file(&sb, state, "rerere-autoupdate", 1);
		state->allow_rerere_autoupdate = म_भेद(sb.buf, "t") ?
			RERERE_NOAUTOUPDATE : RERERE_AUTOUPDATE;
	} अन्यथा {
		state->allow_rerere_autoupdate = 0;
	}

	read_state_file(&sb, state, "keep", 1);
	अगर (!म_भेद(sb.buf, "t"))
		state->keep = KEEP_TRUE;
	अन्यथा अगर (!म_भेद(sb.buf, "b"))
		state->keep = KEEP_NON_PATCH;
	अन्यथा
		state->keep = KEEP_FALSE;

	read_state_file(&sb, state, "messageid", 1);
	state->message_id = !म_भेद(sb.buf, "t");

	read_state_file(&sb, state, "scissors", 1);
	अगर (!म_भेद(sb.buf, "t"))
		state->scissors = SCISSORS_TRUE;
	अन्यथा अगर (!म_भेद(sb.buf, "f"))
		state->scissors = SCISSORS_FALSE;
	अन्यथा
		state->scissors = SCISSORS_UNSET;

	read_state_file(&sb, state, "quoted-cr", 1);
	अगर (!*sb.buf)
		state->quoted_cr = quoted_cr_unset;
	अन्यथा अगर (mailinfo_parse_quoted_cr_action(sb.buf, &state->quoted_cr) != 0)
		die(_("could not parse %s"), am_path(state, "quoted-cr"));

	read_state_file(&sb, state, "apply-opt", 1);
	strvec_clear(&state->git_apply_opts);
	अगर (sq_dequote_to_strvec(sb.buf, &state->git_apply_opts) < 0)
		die(_("could not parse %s"), am_path(state, "apply-opt"));

	state->rebasing = !!file_exists(am_path(state, "rebasing"));

	strbuf_release(&sb);
}

/**
 * Removes the am_state directory, forcefully terminating the current am
 * session.
 */
अटल व्योम am_destroy(स्थिर काष्ठा am_state *state)
{
	काष्ठा strbuf sb = STRBUF_INIT;

	strbuf_addstr(&sb, state->dir);
	remove_dir_recursively(&sb, 0);
	strbuf_release(&sb);
}

/**
 * Runs applypatch-msg hook. Returns its exit code.
 */
अटल पूर्णांक run_applypatch_संदेश_hook(काष्ठा am_state *state)
{
	पूर्णांक ret = 0;

	निश्चित(state->संदेश);

	अगर (!state->no_verify) {
		काष्ठा run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;
		strvec_धकेलो(&opt.args, am_path(state, "final-commit"));
		ret = run_hooks_opt(the_repository, "applypatch-msg", &opt);
	}

	अगर (!ret) {
		FREE_AND_NULL(state->संदेश);
		अगर (read_commit_संदेश(state) < 0)
			die(_("'%s' was deleted by the applypatch-msg hook"),
				am_path(state, "final-commit"));
	}

	वापस ret;
}

/**
 * Runs post-rewrite hook. Returns it exit code.
 */
अटल पूर्णांक run_post_rewrite_hook(स्थिर काष्ठा am_state *state)
{
	काष्ठा run_hooks_opt opt = RUN_HOOKS_OPT_INIT;

	strvec_धकेलो(&opt.args, "rebase");
	opt.path_to_stdin = am_path(state, "rewritten");

	वापस run_hooks_opt(the_repository, "post-rewrite", &opt);
}

/**
 * Reads the state directory's "rewritten" file, and copies notes from the old
 * commits listed in the file to their rewritten commits.
 *
 * Returns 0 on success, -1 on failure.
 */
अटल पूर्णांक copy_notes_for_rebase(स्थिर काष्ठा am_state *state)
{
	काष्ठा notes_rewrite_cfg *c;
	काष्ठा strbuf sb = STRBUF_INIT;
	स्थिर अक्षर *invalid_पंक्ति = _("Malformed input line: '%s'.");
	स्थिर अक्षर *संदेश = "Notes added by 'git rebase'";
	खाता *fp;
	पूर्णांक ret = 0;

	निश्चित(state->rebasing);

	c = init_copy_notes_for_rewrite("rebase");
	अगर (!c)
		वापस 0;

	fp = xfopen(am_path(state, "rewritten"), "r");

	जबतक (!strbuf_getline_lf(&sb, fp)) {
		काष्ठा object_id from_obj, to_obj;
		स्थिर अक्षर *p;

		अगर (sb.len != the_hash_algo->hexsz * 2 + 1) {
			ret = error(invalid_पंक्ति, sb.buf);
			जाओ finish;
		}

		अगर (parse_oid_hex(sb.buf, &from_obj, &p)) {
			ret = error(invalid_पंक्ति, sb.buf);
			जाओ finish;
		}

		अगर (*p != ' ') {
			ret = error(invalid_पंक्ति, sb.buf);
			जाओ finish;
		}

		अगर (get_oid_hex(p + 1, &to_obj)) {
			ret = error(invalid_पंक्ति, sb.buf);
			जाओ finish;
		}

		अगर (copy_note_for_rewrite(c, &from_obj, &to_obj))
			ret = error(_("Failed to copy notes from '%s' to '%s'"),
					oid_to_hex(&from_obj), oid_to_hex(&to_obj));
	}

finish:
	finish_copy_notes_for_rewrite(the_repository, c, संदेश);
	ख_बंद(fp);
	strbuf_release(&sb);
	वापस ret;
}

/**
 * Determines if the file looks like a piece of RFC2822 mail by grabbing all
 * non-indented lines and checking if they look like they begin with valid
 * header field names.
 *
 * Returns 1 if the file looks like a piece of mail, 0 otherwise.
 */
अटल पूर्णांक is_mail(खाता *fp)
{
	स्थिर अक्षर *header_regex = "^[!-9;-~]+:";
	काष्ठा strbuf sb = STRBUF_INIT;
	regex_t regex;
	पूर्णांक ret = 1;

	अगर (ख_जाओ(fp, 0L, शुरू_से))
		die_errno(_("fseek failed"));

	अगर (regcomp(&regex, header_regex, REG_NOSUB | REG_EXTENDED))
		die("invalid pattern: %s", header_regex);

	जबतक (!strbuf_getline(&sb, fp)) {
		अगर (!sb.len)
			अवरोध; /* End of header */

		/* Ignore indented folded lines */
		अगर (*sb.buf == '\t' || *sb.buf == ' ')
			जारी;

		/* It's a header if it matches header_regex */
		अगर (regexec(&regex, sb.buf, 0, शून्य, 0)) {
			ret = 0;
			जाओ done;
		}
	}

done:
	regfree(&regex);
	strbuf_release(&sb);
	वापस ret;
}

/**
 * Attempts to detect the patch_format of the patches contained in `paths`,
 * returning the PATCH_FORMAT_* enum value. Returns PATCH_FORMAT_UNKNOWN if
 * detection fails.
 */
अटल पूर्णांक detect_patch_format(स्थिर अक्षर **paths)
{
	क्रमागत patch_format ret = PATCH_FORMAT_UNKNOWN;
	काष्ठा strbuf l1 = STRBUF_INIT;
	काष्ठा strbuf l2 = STRBUF_INIT;
	काष्ठा strbuf l3 = STRBUF_INIT;
	खाता *fp;

	/*
	 * We default to mbox format if input is from stdin and for directories
	 */
	अगर (!*paths || !म_भेद(*paths, "-") || is_directory(*paths))
		वापस PATCH_FORMAT_MBOX;

	/*
	 * Otherwise, check the first few lines of the first patch, starting
	 * from the first non-blank line, to try to detect its format.
	 */

	fp = xfopen(*paths, "r");

	जबतक (!strbuf_getline(&l1, fp)) {
		अगर (l1.len)
			अवरोध;
	}

	अगर (starts_with(l1.buf, "From ") || starts_with(l1.buf, "From: ")) {
		ret = PATCH_FORMAT_MBOX;
		जाओ done;
	}

	अगर (starts_with(l1.buf, "# This series applies on GIT commit")) {
		ret = PATCH_FORMAT_STGIT_SERIES;
		जाओ done;
	}

	अगर (!म_भेद(l1.buf, "# HG changeset patch")) {
		ret = PATCH_FORMAT_HG;
		जाओ done;
	}

	strbuf_getline(&l2, fp);
	strbuf_getline(&l3, fp);

	/*
	 * If the second line is empty and the third is a From, Author or Date
	 * entry, this is likely an StGit patch.
	 */
	अगर (l1.len && !l2.len &&
		(starts_with(l3.buf, "From:") ||
		 starts_with(l3.buf, "Author:") ||
		 starts_with(l3.buf, "Date:"))) {
		ret = PATCH_FORMAT_STGIT;
		जाओ done;
	}

	अगर (l1.len && is_mail(fp)) {
		ret = PATCH_FORMAT_MBOX;
		जाओ done;
	}

done:
	ख_बंद(fp);
	strbuf_release(&l1);
	strbuf_release(&l2);
	strbuf_release(&l3);
	वापस ret;
}

/**
 * Splits out individual email patches from `paths`, where each path is either
 * a mbox file or a Maildir. Returns 0 on success, -1 on failure.
 */
अटल पूर्णांक split_mail_mbox(काष्ठा am_state *state, स्थिर अक्षर **paths,
				पूर्णांक keep_cr, पूर्णांक mboxrd)
{
	काष्ठा child_process cp = CHILD_PROCESS_INIT;
	काष्ठा strbuf last = STRBUF_INIT;
	पूर्णांक ret;

	cp.git_cmd = 1;
	strvec_धकेलो(&cp.args, "mailsplit");
	strvec_pushf(&cp.args, "-d%d", state->prec);
	strvec_pushf(&cp.args, "-o%s", state->dir);
	strvec_धकेलो(&cp.args, "-b");
	अगर (keep_cr)
		strvec_धकेलो(&cp.args, "--keep-cr");
	अगर (mboxrd)
		strvec_धकेलो(&cp.args, "--mboxrd");
	strvec_धकेलो(&cp.args, "--");
	strvec_pushv(&cp.args, paths);

	ret = capture_command(&cp, &last, 8);
	अगर (ret)
		जाओ निकास;

	state->cur = 1;
	state->last = म_से_दीर्घ(last.buf, शून्य, 10);

निकास:
	strbuf_release(&last);
	वापस ret ? -1 : 0;
}

/**
 * Callback signature for split_mail_conv(). The foreign patch should be
 * read from `in`, and the converted patch (in RFC2822 mail format) should be
 * written to `out`. Return 0 on success, or -1 on failure.
 */
प्रकार पूर्णांक (*mail_conv_fn)(खाता *out, खाता *in, पूर्णांक keep_cr);

/**
 * Calls `fn` for each file in `paths` to convert the foreign patch to the
 * RFC2822 mail format suitable for parsing with git-mailinfo.
 *
 * Returns 0 on success, -1 on failure.
 */
अटल पूर्णांक split_mail_conv(mail_conv_fn fn, काष्ठा am_state *state,
			स्थिर अक्षर **paths, पूर्णांक keep_cr)
{
	अटल स्थिर अक्षर *stdin_only[] = {"-", शून्य};
	पूर्णांक i;

	अगर (!*paths)
		paths = stdin_only;

	क्रम (i = 0; *paths; paths++, i++) {
		खाता *in, *out;
		स्थिर अक्षर *mail;
		पूर्णांक ret;

		अगर (!म_भेद(*paths, "-"))
			in = मानक_निवेश;
		अन्यथा
			in = ख_खोलो(*paths, "r");

		अगर (!in)
			वापस error_errno(_("could not open '%s' for reading"),
					   *paths);

		mail = mkpath("%s/%0*d", state->dir, state->prec, i + 1);

		out = ख_खोलो(mail, "w");
		अगर (!out) {
			अगर (in != मानक_निवेश)
				ख_बंद(in);
			वापस error_errno(_("could not open '%s' for writing"),
					   mail);
		}

		ret = fn(out, in, keep_cr);

		ख_बंद(out);
		अगर (in != मानक_निवेश)
			ख_बंद(in);

		अगर (ret)
			वापस error(_("could not parse patch '%s'"), *paths);
	}

	state->cur = 1;
	state->last = i;
	वापस 0;
}

/**
 * A split_mail_conv() callback that converts an StGit patch to an RFC2822
 * message suitable for parsing with git-mailinfo.
 */
अटल पूर्णांक stgit_patch_to_mail(खाता *out, खाता *in, पूर्णांक keep_cr UNUSED)
{
	काष्ठा strbuf sb = STRBUF_INIT;
	पूर्णांक subject_printed = 0;

	जबतक (!strbuf_getline_lf(&sb, in)) {
		स्थिर अक्षर *str;

		अगर (str_isspace(sb.buf))
			जारी;
		अन्यथा अगर (skip_prefix(sb.buf, "Author:", &str))
			ख_लिखो(out, "From:%s\n", str);
		अन्यथा अगर (starts_with(sb.buf, "From") || starts_with(sb.buf, "Date"))
			ख_लिखो(out, "%s\n", sb.buf);
		अन्यथा अगर (!subject_printed) {
			ख_लिखो(out, "Subject: %s\n", sb.buf);
			subject_printed = 1;
		} अन्यथा {
			ख_लिखो(out, "\n%s\n", sb.buf);
			अवरोध;
		}
	}

	strbuf_reset(&sb);
	जबतक (strbuf_fread(&sb, 8192, in) > 0) {
		ख_डालो(sb.buf, 1, sb.len, out);
		strbuf_reset(&sb);
	}

	strbuf_release(&sb);
	वापस 0;
}

/**
 * This function only supports a single StGit series file in `paths`.
 *
 * Given an StGit series file, converts the StGit patches in the series into
 * RFC2822 messages suitable for parsing with git-mailinfo, and queues them in
 * the state directory.
 *
 * Returns 0 on success, -1 on failure.
 */
अटल पूर्णांक split_mail_stgit_series(काष्ठा am_state *state, स्थिर अक्षर **paths,
					पूर्णांक keep_cr)
{
	स्थिर अक्षर *series_dir;
	अक्षर *series_dir_buf;
	खाता *fp;
	काष्ठा strvec patches = STRVEC_INIT;
	काष्ठा strbuf sb = STRBUF_INIT;
	पूर्णांक ret;

	अगर (!paths[0] || paths[1])
		वापस error(_("Only one StGIT patch series can be applied at once"));

	series_dir_buf = xstrdup(*paths);
	series_dir = स_नाम(series_dir_buf);

	fp = ख_खोलो(*paths, "r");
	अगर (!fp) {
		मुक्त(series_dir_buf);
		वापस error_errno(_("could not open '%s' for reading"), *paths);
	}

	जबतक (!strbuf_getline_lf(&sb, fp)) {
		अगर (*sb.buf == '#')
			जारी; /* skip comment lines */

		strvec_धकेलो(&patches, mkpath("%s/%s", series_dir, sb.buf));
	}

	ख_बंद(fp);
	strbuf_release(&sb);
	मुक्त(series_dir_buf);

	ret = split_mail_conv(stgit_patch_to_mail, state, patches.v, keep_cr);

	strvec_clear(&patches);
	वापस ret;
}

/**
 * A split_patches_conv() callback that converts a mercurial patch to a RFC2822
 * message suitable for parsing with git-mailinfo.
 */
अटल पूर्णांक hg_patch_to_mail(खाता *out, खाता *in, पूर्णांक keep_cr UNUSED)
{
	काष्ठा strbuf sb = STRBUF_INIT;
	पूर्णांक rc = 0;

	जबतक (!strbuf_getline_lf(&sb, in)) {
		स्थिर अक्षर *str;

		अगर (skip_prefix(sb.buf, "# User ", &str))
			ख_लिखो(out, "From: %s\n", str);
		अन्यथा अगर (skip_prefix(sb.buf, "# Date ", &str)) {
			timestamp_t timestamp;
			दीर्घ tz, tz2;
			अक्षर *end;

			त्रुटि_सं = 0;
			timestamp = parse_timestamp(str, &end, 10);
			अगर (त्रुटि_सं) {
				rc = error(_("invalid timestamp"));
				जाओ निकास;
			}

			अगर (!skip_prefix(end, " ", &str)) {
				rc = error(_("invalid Date line"));
				जाओ निकास;
			}

			त्रुटि_सं = 0;
			tz = म_से_दीर्घ(str, &end, 10);
			अगर (त्रुटि_सं) {
				rc = error(_("invalid timezone offset"));
				जाओ निकास;
			}

			अगर (*end) {
				rc = error(_("invalid Date line"));
				जाओ निकास;
			}

			/*
			 * mercurial's timezone is in seconds west of UTC,
			 * however git's timezone is in hours + minutes east of
			 * UTC. Convert it.
			 */
			tz2 = द_असल(tz) / 3600 * 100 + द_असल(tz) % 3600 / 60;
			अगर (tz > 0)
				tz2 = -tz2;

			ख_लिखो(out, "Date: %s\n", show_date(timestamp, tz2, DATE_MODE(RFC2822)));
		} अन्यथा अगर (starts_with(sb.buf, "# ")) {
			जारी;
		} अन्यथा {
			ख_लिखो(out, "\n%s\n", sb.buf);
			अवरोध;
		}
	}

	strbuf_reset(&sb);
	जबतक (strbuf_fread(&sb, 8192, in) > 0) {
		ख_डालो(sb.buf, 1, sb.len, out);
		strbuf_reset(&sb);
	}
निकास:
	strbuf_release(&sb);
	वापस rc;
}

/**
 * Splits a list of files/directories into individual email patches. Each path
 * in `paths` must be a file/directory that is formatted according to
 * `patch_format`.
 *
 * Once split out, the individual email patches will be stored in the state
 * directory, with each patch's filename being its index, padded to state->prec
 * digits.
 *
 * state->cur will be set to the index of the first mail, and state->last will
 * be set to the index of the last mail.
 *
 * Set keep_cr to 0 to convert all lines ending with \r\n to end with \n, 1
 * to disable this behavior, -1 to use the default configured setting.
 *
 * Returns 0 on success, -1 on failure.
 */
अटल पूर्णांक split_mail(काष्ठा am_state *state, क्रमागत patch_format patch_format,
			स्थिर अक्षर **paths, पूर्णांक keep_cr)
{
	अगर (keep_cr < 0) {
		keep_cr = 0;
		repo_config_get_bool(the_repository, "am.keepcr", &keep_cr);
	}

	चयन (patch_format) {
	हाल PATCH_FORMAT_MBOX:
		वापस split_mail_mbox(state, paths, keep_cr, 0);
	हाल PATCH_FORMAT_STGIT:
		वापस split_mail_conv(stgit_patch_to_mail, state, paths, keep_cr);
	हाल PATCH_FORMAT_STGIT_SERIES:
		वापस split_mail_stgit_series(state, paths, keep_cr);
	हाल PATCH_FORMAT_HG:
		वापस split_mail_conv(hg_patch_to_mail, state, paths, keep_cr);
	हाल PATCH_FORMAT_MBOXRD:
		वापस split_mail_mbox(state, paths, keep_cr, 1);
	शेष:
		BUG("invalid patch_format");
	}
	वापस -1;
}

/**
 * Setup a new am session for applying patches
 */
अटल व्योम am_setup(काष्ठा am_state *state, क्रमागत patch_format patch_format,
			स्थिर अक्षर **paths, पूर्णांक keep_cr)
{
	काष्ठा object_id curr_शीर्ष;
	स्थिर अक्षर *str;
	काष्ठा strbuf sb = STRBUF_INIT;

	अगर (!patch_format)
		patch_format = detect_patch_format(paths);

	अगर (!patch_format) {
		fprintf_ln(मानक_त्रुटि, _("Patch format detection failed."));
		die(शून्य);
	}

	अगर (सूची_गढ़ो(state->dir, 0777) < 0 && त्रुटि_सं != EEXIST)
		die_errno(_("failed to create directory '%s'"), state->dir);
	refs_delete_ref(get_main_ref_store(the_repository), शून्य,
			"REBASE_HEAD", शून्य, REF_NO_DEREF);

	अगर (split_mail(state, patch_format, paths, keep_cr) < 0) {
		am_destroy(state);
		die(_("Failed to split patches."));
	}

	अगर (state->rebasing)
		state->threeway = 1;

	write_state_bool(state, "threeway", state->threeway);
	write_state_bool(state, "quiet", state->quiet);
	write_state_bool(state, "sign", state->signoff);
	write_state_bool(state, "utf8", state->utf8);

	अगर (state->allow_rerere_autoupdate)
		write_state_bool(state, "rerere-autoupdate",
			 state->allow_rerere_autoupdate == RERERE_AUTOUPDATE);

	चयन (state->keep) {
	हाल KEEP_FALSE:
		str = "f";
		अवरोध;
	हाल KEEP_TRUE:
		str = "t";
		अवरोध;
	हाल KEEP_NON_PATCH:
		str = "b";
		अवरोध;
	शेष:
		BUG("invalid value for state->keep");
	}

	write_state_text(state, "keep", str);
	write_state_bool(state, "messageid", state->message_id);

	चयन (state->scissors) {
	हाल SCISSORS_UNSET:
		str = "";
		अवरोध;
	हाल SCISSORS_FALSE:
		str = "f";
		अवरोध;
	हाल SCISSORS_TRUE:
		str = "t";
		अवरोध;
	शेष:
		BUG("invalid value for state->scissors");
	}
	write_state_text(state, "scissors", str);

	चयन (state->quoted_cr) {
	हाल quoted_cr_unset:
		str = "";
		अवरोध;
	हाल quoted_cr_nowarn:
		str = "nowarn";
		अवरोध;
	हाल quoted_cr_warn:
		str = "warn";
		अवरोध;
	हाल quoted_cr_strip:
		str = "strip";
		अवरोध;
	शेष:
		BUG("invalid value for state->quoted_cr");
	}
	write_state_text(state, "quoted-cr", str);

	sq_quote_argv(&sb, state->git_apply_opts.v);
	write_state_text(state, "apply-opt", sb.buf);

	अगर (state->rebasing)
		write_state_text(state, "rebasing", "");
	अन्यथा
		write_state_text(state, "applying", "");

	अगर (!repo_get_oid(the_repository, "HEAD", &curr_शीर्ष)) {
		write_state_text(state, "abort-safety", oid_to_hex(&curr_शीर्ष));
		अगर (!state->rebasing)
			refs_update_ref(get_main_ref_store(the_repository),
					"am", "ORIG_HEAD", &curr_शीर्ष, शून्य,
					0,
					UPDATE_REFS_DIE_ON_ERR);
	} अन्यथा {
		write_state_text(state, "abort-safety", "");
		अगर (!state->rebasing)
			refs_delete_ref(get_main_ref_store(the_repository),
					शून्य, "ORIG_HEAD", शून्य, 0);
	}

	/*
	 * NOTE: Since the "next" and "last" files determine if an am_state
	 * session is in progress, they should be written last.
	 */

	write_state_गिनती(state, "next", state->cur);
	write_state_गिनती(state, "last", state->last);

	strbuf_release(&sb);
}

/**
 * Increments the patch pointer, and cleans am_state for the application of the
 * next patch.
 */
अटल व्योम am_अग्र(काष्ठा am_state *state)
{
	काष्ठा object_id शीर्ष;

	FREE_AND_NULL(state->author_name);
	FREE_AND_NULL(state->author_email);
	FREE_AND_NULL(state->author_date);
	FREE_AND_NULL(state->संदेश);
	state->संदेश_len = 0;

	unlink(am_path(state, "author-script"));
	unlink(am_path(state, "final-commit"));

	oidclr(&state->orig_commit, the_repository->hash_algo);
	unlink(am_path(state, "original-commit"));
	refs_delete_ref(get_main_ref_store(the_repository), शून्य,
			"REBASE_HEAD", शून्य, REF_NO_DEREF);

	अगर (!repo_get_oid(the_repository, "HEAD", &शीर्ष))
		write_state_text(state, "abort-safety", oid_to_hex(&शीर्ष));
	अन्यथा
		write_state_text(state, "abort-safety", "");

	state->cur++;
	write_state_गिनती(state, "next", state->cur);
}

/**
 * Returns the filename of the current patch email.
 */
अटल स्थिर अक्षर *msgnum(स्थिर काष्ठा am_state *state)
{
	अटल काष्ठा strbuf sb = STRBUF_INIT;

	strbuf_reset(&sb);
	strbuf_addf(&sb, "%0*d", state->prec, state->cur);

	वापस sb.buf;
}

/**
 * Dies with a user-friendly message on how to proceed after resolving the
 * problem. This message can be overridden with state->resolvemsg.
 */
अटल व्योम NORETURN die_user_resolve(स्थिर काष्ठा am_state *state)
{
	अगर (state->resolvemsg) {
		advise_if_enabled(ADVICE_MERGE_CONFLICT, "%s", state->resolvemsg);
	} अन्यथा {
		स्थिर अक्षर *cmdline = state->interactive ? "git am -i" : "git am";
		काष्ठा strbuf sb = STRBUF_INIT;

		strbuf_addf(&sb, _("When you have resolved this problem, run \"%s --continue\".\n"), cmdline);
		strbuf_addf(&sb, _("If you prefer to skip this patch, run \"%s --skip\" instead.\n"), cmdline);

		अगर (advice_enabled(ADVICE_AM_WORK_DIR) &&
		    is_empty_or_missing_file(am_path(state, "patch")) &&
		    !repo_सूचकांक_has_changes(the_repository, शून्य, शून्य))
			strbuf_addf(&sb, _("To record the empty patch as an empty commit, run \"%s --allow-empty\".\n"), cmdline);

		strbuf_addf(&sb, _("To restore the original branch and stop patching, run \"%s --abort\"."), cmdline);

		advise_if_enabled(ADVICE_MERGE_CONFLICT, "%s", sb.buf);
		strbuf_release(&sb);
	}

	die(शून्य);
}

/**
 * Appends signoff to the "msg" field of the am_state.
 */
अटल व्योम am_append_signoff(काष्ठा am_state *state)
{
	काष्ठा strbuf sb = STRBUF_INIT;

	strbuf_attach(&sb, state->संदेश, state->संदेश_len, state->संदेश_len + 1);
	append_signoff(&sb, 0, 0);
	state->संदेश = strbuf_detach(&sb, &state->संदेश_len);
}

/**
 * Parses `mail` using git-mailinfo, extracting its patch and authorship info.
 * state->msg will be set to the patch message. state->author_name,
 * state->author_email and state->author_date will be set to the patch author's
 * name, email and date respectively. The patch body will be written to the
 * state directory's "patch" file.
 *
 * Returns 1 if the patch should be skipped, 0 otherwise.
 */
अटल पूर्णांक parse_mail(काष्ठा am_state *state, स्थिर अक्षर *mail)
{
	खाता *fp;
	काष्ठा strbuf sb = STRBUF_INIT;
	काष्ठा strbuf संदेश = STRBUF_INIT;
	काष्ठा strbuf author_name = STRBUF_INIT;
	काष्ठा strbuf author_date = STRBUF_INIT;
	काष्ठा strbuf author_email = STRBUF_INIT;
	पूर्णांक ret = 0;
	काष्ठा mailinfo mi;

	setup_mailinfo(the_repository, &mi);

	अगर (state->utf8)
		mi.metainfo_charset = get_commit_output_encoding();
	अन्यथा
		mi.metainfo_charset = शून्य;

	चयन (state->keep) {
	हाल KEEP_FALSE:
		अवरोध;
	हाल KEEP_TRUE:
		mi.keep_subject = 1;
		अवरोध;
	हाल KEEP_NON_PATCH:
		mi.keep_non_patch_brackets_in_subject = 1;
		अवरोध;
	शेष:
		BUG("invalid value for state->keep");
	}

	अगर (state->message_id)
		mi.add_message_id = 1;

	चयन (state->scissors) {
	हाल SCISSORS_UNSET:
		अवरोध;
	हाल SCISSORS_FALSE:
		mi.use_scissors = 0;
		अवरोध;
	हाल SCISSORS_TRUE:
		mi.use_scissors = 1;
		अवरोध;
	शेष:
		BUG("invalid value for state->scissors");
	}

	चयन (state->quoted_cr) {
	हाल quoted_cr_unset:
		अवरोध;
	हाल quoted_cr_nowarn:
	हाल quoted_cr_warn:
	हाल quoted_cr_strip:
		mi.quoted_cr = state->quoted_cr;
		अवरोध;
	शेष:
		BUG("invalid value for state->quoted_cr");
	}

	mi.निवेश = xfopen(mail, "r");
	mi.output = xfopen(am_path(state, "info"), "w");
	अगर (mailinfo(&mi, am_path(state, "msg"), am_path(state, "patch")))
		die("could not parse patch");

	ख_बंद(mi.निवेश);
	ख_बंद(mi.output);

	अगर (mi.format_flowed)
		warning(_("Patch sent with format=flowed; "
			  "space at the end of lines might be lost."));

	/* Extract message and author information */
	fp = xfopen(am_path(state, "info"), "r");
	जबतक (!strbuf_getline_lf(&sb, fp)) {
		स्थिर अक्षर *x;

		अगर (skip_prefix(sb.buf, "Subject: ", &x)) {
			अगर (संदेश.len)
				strbuf_addch(&संदेश, '\n');
			strbuf_addstr(&संदेश, x);
		} अन्यथा अगर (skip_prefix(sb.buf, "Author: ", &x))
			strbuf_addstr(&author_name, x);
		अन्यथा अगर (skip_prefix(sb.buf, "Email: ", &x))
			strbuf_addstr(&author_email, x);
		अन्यथा अगर (skip_prefix(sb.buf, "Date: ", &x))
			strbuf_addstr(&author_date, x);
	}
	ख_बंद(fp);

	/* Skip pine's internal folder data */
	अगर (!म_भेद(author_name.buf, "Mail System Internal Data")) {
		ret = 1;
		जाओ finish;
	}

	strbuf_addstr(&संदेश, "\n\n");
	strbuf_addbuf(&संदेश, &mi.log_message);
	strbuf_stripspace(&संदेश, शून्य);

	निश्चित(!state->author_name);
	state->author_name = strbuf_detach(&author_name, शून्य);

	निश्चित(!state->author_email);
	state->author_email = strbuf_detach(&author_email, शून्य);

	निश्चित(!state->author_date);
	state->author_date = strbuf_detach(&author_date, शून्य);

	निश्चित(!state->संदेश);
	state->संदेश = strbuf_detach(&संदेश, &state->संदेश_len);

finish:
	strbuf_release(&संदेश);
	strbuf_release(&author_date);
	strbuf_release(&author_email);
	strbuf_release(&author_name);
	strbuf_release(&sb);
	clear_mailinfo(&mi);
	वापस ret;
}

/**
 * Sets commit_id to the commit hash where the mail was generated from.
 * Returns 0 on success, -1 on failure.
 */
अटल पूर्णांक get_mail_commit_oid(काष्ठा object_id *commit_id, स्थिर अक्षर *mail)
{
	काष्ठा strbuf sb = STRBUF_INIT;
	खाता *fp = xfopen(mail, "r");
	स्थिर अक्षर *x;
	पूर्णांक ret = 0;

	अगर (strbuf_getline_lf(&sb, fp) ||
	    !skip_prefix(sb.buf, "From ", &x) ||
	    get_oid_hex(x, commit_id) < 0)
		ret = -1;

	strbuf_release(&sb);
	ख_बंद(fp);
	वापस ret;
}

/**
 * Sets state->msg, state->author_name, state->author_email, state->author_date
 * to the commit's respective info.
 */
अटल व्योम get_commit_info(काष्ठा am_state *state, काष्ठा commit *commit)
{
	स्थिर अक्षर *बफर, *ident_पंक्ति, *संदेश;
	माप_प्रकार ident_len;
	काष्ठा ident_split id;

	बफर = repo_logmsg_reencode(the_repository, commit, शून्य,
				      get_commit_output_encoding());

	ident_पंक्ति = find_commit_header(बफर, "author", &ident_len);
	अगर (!ident_पंक्ति)
		die(_("missing author line in commit %s"),
		      oid_to_hex(&commit->object.oid));
	अगर (split_ident_पंक्ति(&id, ident_पंक्ति, ident_len) < 0)
		die(_("invalid ident line: %.*s"), (पूर्णांक)ident_len, ident_पंक्ति);

	निश्चित(!state->author_name);
	अगर (id.name_begin)
		state->author_name =
			xmemdupz(id.name_begin, id.name_end - id.name_begin);
	अन्यथा
		state->author_name = xstrdup("");

	निश्चित(!state->author_email);
	अगर (id.mail_begin)
		state->author_email =
			xmemdupz(id.mail_begin, id.mail_end - id.mail_begin);
	अन्यथा
		state->author_email = xstrdup("");

	निश्चित(!state->author_date);
	state->author_date = xstrdup(show_ident_date(&id, DATE_MODE(NORMAL)));

	निश्चित(!state->संदेश);
	संदेश = म_माला(बफर, "\n\n");
	अगर (!संदेश)
		die(_("unable to parse commit %s"), oid_to_hex(&commit->object.oid));
	state->संदेश = xstrdup(संदेश + 2);
	state->संदेश_len = म_माप(state->संदेश);
	repo_unuse_commit_बफर(the_repository, commit, बफर);
}

/**
 * Writes `commit` as a patch to the state directory's "patch" file.
 */
अटल व्योम write_commit_patch(स्थिर काष्ठा am_state *state, काष्ठा commit *commit)
{
	काष्ठा rev_info rev_info;
	खाता *fp;

	fp = xfopen(am_path(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, शून्य);
	rev_info.diff = 1;
	rev_info.abbrev = 0;
	rev_info.disable_stdin = 1;
	rev_info.show_मूल_diff = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.flags.द्विआधारी = 1;
	rev_info.diffopt.flags.full_सूचकांक = 1;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	add_pending_object(&rev_info, &commit->object, "");
	diff_setup_done(&rev_info.diffopt);
	log_वृक्ष_commit(&rev_info, commit);
	release_revisions(&rev_info);
}

/**
 * Writes the diff of the index against HEAD as a patch to the state
 * directory's "patch" file.
 */
अटल व्योम write_सूचकांक_patch(स्थिर काष्ठा am_state *state)
{
	काष्ठा वृक्ष *वृक्ष;
	काष्ठा object_id शीर्ष;
	काष्ठा rev_info rev_info;
	खाता *fp;

	अगर (!repo_get_oid(the_repository, "HEAD", &शीर्ष)) {
		काष्ठा commit *commit = lookup_commit_or_die(&शीर्ष, "HEAD");
		वृक्ष = repo_get_commit_वृक्ष(the_repository, commit);
	} अन्यथा
		वृक्ष = lookup_वृक्ष(the_repository,
				   the_repository->hash_algo->empty_वृक्ष);

	fp = xfopen(am_path(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, शून्य);
	rev_info.diff = 1;
	rev_info.disable_stdin = 1;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	add_pending_object(&rev_info, &वृक्ष->object, "");
	diff_setup_done(&rev_info.diffopt);
	run_diff_सूचकांक(&rev_info, DIFF_सूचकांक_CACHED);
	release_revisions(&rev_info);
}

/**
 * Like parse_mail(), but parses the mail by looking up its commit ID
 * directly. This is used in --rebasing mode to bypass git-mailinfo's munging
 * of patches.
 *
 * state->orig_commit will be set to the original commit ID.
 *
 * Will always return 0 as the patch should never be skipped.
 */
अटल पूर्णांक parse_mail_rebase(काष्ठा am_state *state, स्थिर अक्षर *mail)
{
	काष्ठा commit *commit;
	काष्ठा object_id commit_oid;

	अगर (get_mail_commit_oid(&commit_oid, mail) < 0)
		die(_("could not parse %s"), mail);

	commit = lookup_commit_or_die(&commit_oid, mail);

	get_commit_info(state, commit);

	write_commit_patch(state, commit);

	oidcpy(&state->orig_commit, &commit_oid);
	write_state_text(state, "original-commit", oid_to_hex(&commit_oid));
	refs_update_ref(get_main_ref_store(the_repository), "am",
			"REBASE_HEAD", &commit_oid,
			शून्य, REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);

	वापस 0;
}

/**
 * Applies current patch with git-apply. Returns 0 on success, -1 otherwise. If
 * `index_file` is not NULL, the patch will be applied to that index.
 */
अटल पूर्णांक run_apply(स्थिर काष्ठा am_state *state, स्थिर अक्षर *सूचकांक_file)
{
	काष्ठा strvec apply_paths = STRVEC_INIT;
	काष्ठा strvec apply_opts = STRVEC_INIT;
	काष्ठा apply_state apply_state;
	पूर्णांक res, opts_बायाँ;
	पूर्णांक force_apply = 0;
	पूर्णांक options = 0;
	स्थिर अक्षर **apply_argv;

	अगर (init_apply_state(&apply_state, the_repository, शून्य))
		BUG("init_apply_state() failed");

	strvec_धकेलो(&apply_opts, "apply");
	strvec_pushv(&apply_opts, state->git_apply_opts.v);

	/*
	 * Build a copy that apply_parse_options() can rearrange.
	 * apply_opts.v keeps referencing the allocated strings for
	 * strvec_clear() to release.
	 */
	DUP_ARRAY(apply_argv, apply_opts.v, apply_opts.nr);

	opts_बायाँ = apply_parse_options(apply_opts.nr, apply_argv,
					&apply_state, &force_apply, &options,
					शून्य);

	अगर (opts_बायाँ != 0)
		die("unknown option passed through to git apply");

	अगर (सूचकांक_file) {
		apply_state.सूचकांक_file = सूचकांक_file;
		apply_state.cached = 1;
	} अन्यथा
		apply_state.check_सूचकांक = 1;

	/*
	 * If we are allowed to fall back on 3-way merge, don't give false
	 * errors during the initial attempt.
	 */
	अगर (state->threeway && !सूचकांक_file)
		apply_state.apply_verbosity = verbosity_silent;

	अगर (check_apply_state(&apply_state, force_apply))
		BUG("check_apply_state() failed");

	strvec_धकेलो(&apply_paths, am_path(state, "patch"));

	res = apply_all_patches(&apply_state, apply_paths.nr, apply_paths.v, options);

	strvec_clear(&apply_paths);
	strvec_clear(&apply_opts);
	clear_apply_state(&apply_state);
	मुक्त(apply_argv);

	अगर (res)
		वापस res;

	अगर (सूचकांक_file) {
		/* Reload index as apply_all_patches() will have modified it. */
		discard_सूचकांक(the_repository->सूचकांक);
		read_सूचकांक_from(the_repository->सूचकांक, सूचकांक_file,
				repo_get_git_dir(the_repository));
	}

	वापस 0;
}

/**
 * Builds an index that contains just the blobs needed for a 3way merge.
 */
अटल पूर्णांक build_fake_ancestor(स्थिर काष्ठा am_state *state, स्थिर अक्षर *सूचकांक_file)
{
	काष्ठा child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	strvec_धकेलो(&cp.args, "apply");
	strvec_pushv(&cp.args, state->git_apply_opts.v);
	strvec_pushf(&cp.args, "--build-fake-ancestor=%s", सूचकांक_file);
	strvec_धकेलो(&cp.args, am_path(state, "patch"));

	अगर (run_command(&cp))
		वापस -1;

	वापस 0;
}

/**
 * Attempt a threeway merge, using index_path as the temporary index.
 */
अटल पूर्णांक fall_back_threeway(स्थिर काष्ठा am_state *state, स्थिर अक्षर *सूचकांक_path)
{
	काष्ठा object_id their_वृक्ष, our_वृक्ष;
	काष्ठा object_id bases[1] = { 0 };
	काष्ठा merge_options o;
	काष्ठा commit *परिणाम;
	अक्षर *their_वृक्ष_name;

	अगर (repo_get_oid(the_repository, "HEAD", &our_वृक्ष) < 0)
		oidcpy(&our_वृक्ष, the_hash_algo->empty_वृक्ष);

	अगर (build_fake_ancestor(state, सूचकांक_path))
		वापस error("could not build fake ancestor");

	discard_सूचकांक(the_repository->सूचकांक);
	read_सूचकांक_from(the_repository->सूचकांक, सूचकांक_path, repo_get_git_dir(the_repository));

	अगर (write_सूचकांक_as_वृक्ष(&bases[0], the_repository->सूचकांक, सूचकांक_path, 0, शून्य))
		वापस error(_("Repository lacks necessary blobs to fall back on 3-way merge."));

	say(state, मानक_निकास, _("Using index info to reconstruct a base tree..."));

	अगर (!state->quiet) {
		/*
		 * List paths that needed 3-way fallback, so that the user can
		 * review them with extra care to spot mismerges.
		 */
		काष्ठा rev_info rev_info;

		repo_init_revisions(the_repository, &rev_info, शून्य);
		rev_info.diffopt.output_format = DIFF_FORMAT_NAME_STATUS;
		rev_info.diffopt.filter |= diff_filter_bit('A');
		rev_info.diffopt.filter |= diff_filter_bit('M');
		add_pending_oid(&rev_info, "HEAD", &our_वृक्ष, 0);
		diff_setup_done(&rev_info.diffopt);
		run_diff_सूचकांक(&rev_info, DIFF_सूचकांक_CACHED);
		release_revisions(&rev_info);
	}

	अगर (run_apply(state, सूचकांक_path))
		वापस error(_("Did you hand edit your patch?\n"
				"It does not apply to blobs recorded in its index."));

	अगर (write_सूचकांक_as_वृक्ष(&their_वृक्ष, the_repository->सूचकांक, सूचकांक_path, 0, शून्य))
		वापस error("could not write tree");

	say(state, मानक_निकास, _("Falling back to patching base and 3-way merge..."));

	discard_सूचकांक(the_repository->सूचकांक);
	repo_read_सूचकांक(the_repository);

	/*
	 * This is not so wrong. Depending on which base we picked, orig_tree
	 * may be wildly different from ours, but their_tree has the same set of
	 * wildly different changes in parts the patch did not touch, so
	 * recursive ends up canceling them, saying that we reverted all those
	 * changes.
	 */

	init_ui_merge_options(&o, the_repository);

	o.branch1 = "HEAD";
	their_वृक्ष_name = xstrfmt("%.*s", linelen(state->संदेश), state->संदेश);
	o.branch2 = their_वृक्ष_name;
	o.ancestor = "constructed fake ancestor";
	o.detect_directory_renames = MERGE_DIRECTORY_RENAMES_NONE;

	अगर (state->quiet)
		o.verbosity = 0;

	अगर (merge_ort_generic(&o, &our_वृक्ष, &their_वृक्ष, 1, bases, &परिणाम)) {
		repo_rerere(the_repository, state->allow_rerere_autoupdate);
		मुक्त(their_वृक्ष_name);
		वापस error(_("Failed to merge in the changes."));
	}

	मुक्त(their_वृक्ष_name);
	वापस 0;
}

/**
 * Commits the current index with state->msg as the commit message and
 * state->author_name, state->author_email and state->author_date as the author
 * information.
 */
अटल व्योम do_commit(स्थिर काष्ठा am_state *state)
{
	काष्ठा object_id वृक्ष, parent, commit;
	स्थिर काष्ठा object_id *old_oid;
	काष्ठा commit_list *parents = शून्य;
	स्थिर अक्षर *reflog_संदेश, *author, *committer = शून्य;
	काष्ठा strbuf sb = STRBUF_INIT;

	अगर (!state->no_verify && run_hooks(the_repository, "pre-applypatch"))
		निकास(1);

	अगर (write_सूचकांक_as_वृक्ष(&वृक्ष, the_repository->सूचकांक,
				repo_get_सूचकांक_file(the_repository),
				0, शून्य))
		die(_("git write-tree failed to write a tree"));

	अगर (!repo_get_oid_commit(the_repository, "HEAD", &parent)) {
		old_oid = &parent;
		commit_list_जोड़ो(lookup_commit(the_repository, &parent),
				   &parents);
	} अन्यथा {
		old_oid = शून्य;
		say(state, मानक_त्रुटि, _("applying to an empty history"));
	}

	author = fmt_ident(state->author_name, state->author_email,
		WANT_AUTHOR_IDENT,
			state->ignore_date ? शून्य : state->author_date,
			IDENT_STRICT);

	अगर (state->committer_date_is_author_date)
		committer = fmt_ident(दो_पर्या("GIT_COMMITTER_NAME"),
				      दो_पर्या("GIT_COMMITTER_EMAIL"),
				      WANT_COMMITTER_IDENT,
				      state->ignore_date ? शून्य
							 : state->author_date,
				      IDENT_STRICT);

	अगर (commit_वृक्ष_extended(state->संदेश, state->संदेश_len, &वृक्ष, parents,
				 &commit, author, committer, state->sign_commit,
				 शून्य))
		die(_("failed to write commit object"));

	reflog_संदेश = दो_पर्या("GIT_REFLOG_ACTION");
	अगर (!reflog_संदेश)
		reflog_संदेश = "am";

	strbuf_addf(&sb, "%s: %.*s", reflog_संदेश, linelen(state->संदेश),
			state->संदेश);

	refs_update_ref(get_main_ref_store(the_repository), sb.buf, "HEAD",
			&commit, old_oid, 0,
			UPDATE_REFS_DIE_ON_ERR);

	अगर (state->rebasing) {
		खाता *fp = xfopen(am_path(state, "rewritten"), "a");

		निश्चित(!is_null_oid(&state->orig_commit));
		ख_लिखो(fp, "%s ", oid_to_hex(&state->orig_commit));
		ख_लिखो(fp, "%s\n", oid_to_hex(&commit));
		ख_बंद(fp);
	}

	run_hooks(the_repository, "post-applypatch");

	commit_list_free(parents);
	strbuf_release(&sb);
}

/**
 * Validates the am_state for resuming -- the "msg" and authorship fields must
 * be filled up.
 */
अटल व्योम validate_resume_state(स्थिर काष्ठा am_state *state)
{
	अगर (!state->संदेश)
		die(_("cannot resume: %s does not exist."),
			am_path(state, "final-commit"));

	अगर (!state->author_name || !state->author_email || !state->author_date)
		die(_("cannot resume: %s does not exist."),
			am_path(state, "author-script"));
}

/**
 * Interactively prompt the user on whether the current patch should be
 * applied.
 *
 * Returns 0 if the user chooses to apply the patch, 1 if the user chooses to
 * skip it.
 */
अटल पूर्णांक do_interactive(काष्ठा am_state *state)
{
	निश्चित(state->संदेश);

	क्रम (;;) {
		अक्षर reply[64];

		माला_दो(_("Commit Body is:"));
		माला_दो("--------------------------");
		म_लिखो("%s", state->संदेश);
		माला_दो("--------------------------");

		/*
		 * TRANSLATORS: Make sure to include [y], [n], [e], [v] and [a]
		 * in your translation. The program will only accept English
		 * input at this point.
		 */
		म_लिखो(_("Apply? [y]es/[n]o/[e]dit/[v]iew patch/[a]ccept all: "));
		अगर (!ख_माला_लो(reply, माप(reply), मानक_निवेश))
			die("unable to read from stdin; aborting");

		अगर (*reply == 'y' || *reply == 'Y') {
			वापस 0;
		} अन्यथा अगर (*reply == 'a' || *reply == 'A') {
			state->interactive = 0;
			वापस 0;
		} अन्यथा अगर (*reply == 'n' || *reply == 'N') {
			वापस 1;
		} अन्यथा अगर (*reply == 'e' || *reply == 'E') {
			काष्ठा strbuf संदेश = STRBUF_INIT;

			अगर (!launch_editor(am_path(state, "final-commit"), &संदेश, शून्य)) {
				मुक्त(state->संदेश);
				state->संदेश = strbuf_detach(&संदेश, &state->संदेश_len);
			}
			strbuf_release(&संदेश);
		} अन्यथा अगर (*reply == 'v' || *reply == 'V') {
			स्थिर अक्षर *pager = git_pager(the_repository, 1);
			काष्ठा child_process cp = CHILD_PROCESS_INIT;

			अगर (!pager)
				pager = "cat";
			prepare_pager_args(&cp, pager);
			strvec_धकेलो(&cp.args, am_path(state, "patch"));
			run_command(&cp);
		}
	}
}

/**
 * Applies all queued mail.
 *
 * If `resume` is true, we are "resuming". The "msg" and authorship fields, as
 * well as the state directory's "patch" file is used as-is for applying the
 * patch and committing it.
 */
अटल व्योम am_run(काष्ठा am_state *state, पूर्णांक resume)
{
	काष्ठा strbuf sb = STRBUF_INIT;

	unlink(am_path(state, "dirtyindex"));

	अगर (repo_refresh_and_write_सूचकांक(the_repository, REFRESH_QUIET, 0, 0,
					 शून्य, शून्य, शून्य) < 0)
		die(_("unable to write index file"));

	अगर (repo_सूचकांक_has_changes(the_repository, शून्य, &sb)) {
		write_state_bool(state, "dirtyindex", 1);
		die(_("Dirty index: cannot apply patches (dirty: %s)"), sb.buf);
	}

	strbuf_release(&sb);

	जबतक (state->cur <= state->last) {
		स्थिर अक्षर *mail = am_path(state, msgnum(state));
		पूर्णांक apply_status;
		पूर्णांक to_keep;

		reset_ident_date();

		अगर (!file_exists(mail))
			जाओ अग्र;

		अगर (resume) {
			validate_resume_state(state);
		} अन्यथा {
			पूर्णांक skip;

			अगर (state->rebasing)
				skip = parse_mail_rebase(state, mail);
			अन्यथा
				skip = parse_mail(state, mail);

			अगर (skip)
				जाओ अग्र; /* mail should be skipped */

			अगर (state->signoff)
				am_append_signoff(state);

			write_author_script(state);
			write_commit_संदेश(state);
		}

		अगर (state->interactive && do_interactive(state))
			जाओ अग्र;

		to_keep = 0;
		अगर (is_empty_or_missing_file(am_path(state, "patch"))) {
			चयन (state->empty_type) {
			हाल DROP_EMPTY_COMMIT:
				say(state, मानक_निकास, _("Skipping: %.*s"), linelen(state->संदेश), state->संदेश);
				जाओ अग्र;
				अवरोध;
			हाल KEEP_EMPTY_COMMIT:
				to_keep = 1;
				say(state, मानक_निकास, _("Creating an empty commit: %.*s"),
					linelen(state->संदेश), state->संदेश);
				अवरोध;
			हाल STOP_ON_EMPTY_COMMIT:
				printf_ln(_("Patch is empty."));
				die_user_resolve(state);
				अवरोध;
			}
		}

		अगर (run_applypatch_संदेश_hook(state))
			निकास(1);
		अगर (to_keep)
			जाओ commit;

		say(state, मानक_निकास, _("Applying: %.*s"), linelen(state->संदेश), state->संदेश);

		apply_status = run_apply(state, शून्य);

		अगर (apply_status && state->threeway) {
			काष्ठा strbuf sb = STRBUF_INIT;

			strbuf_addstr(&sb, am_path(state, "patch-merge-index"));
			apply_status = fall_back_threeway(state, sb.buf);
			strbuf_release(&sb);

			/*
			 * Applying the patch to an earlier tree and merging
			 * the result may have produced the same tree as ours.
			 */
			अगर (!apply_status &&
			    !repo_सूचकांक_has_changes(the_repository, शून्य, शून्य)) {
				say(state, मानक_निकास, _("No changes -- Patch already applied."));
				जाओ अग्र;
			}
		}

		अगर (apply_status) {
			printf_ln(_("Patch failed at %s %.*s"), msgnum(state),
				linelen(state->संदेश), state->संदेश);

			अगर (advice_enabled(ADVICE_AM_WORK_DIR))
				advise(_("Use 'git am --show-current-patch=diff' to see the failed patch"));

			die_user_resolve(state);
		}

commit:
		do_commit(state);

अग्र:
		am_अग्र(state);

		अगर (resume)
			am_load(state);
		resume = 0;
	}

	अगर (!is_empty_or_missing_file(am_path(state, "rewritten"))) {
		निश्चित(state->rebasing);
		copy_notes_for_rebase(state);
		run_post_rewrite_hook(state);
	}

	/*
	 * In rebasing mode, it's up to the caller to take care of
	 * housekeeping.
	 */
	अगर (!state->rebasing) {
		am_destroy(state);
		run_auto_maintenance(the_repository, state->quiet);
	}
}

/**
 * Resume the current am session after patch application failure. The user did
 * all the hard work, and we do not have to do any patch application. Just
 * trust and commit what the user has in the index and working tree. If `allow_empty`
 * is true, commit as an empty commit when index has not changed and lacking a patch.
 */
अटल व्योम am_resolve(काष्ठा am_state *state, पूर्णांक allow_empty)
{
	validate_resume_state(state);

	say(state, मानक_निकास, _("Applying: %.*s"), linelen(state->संदेश), state->संदेश);

	अगर (!repo_सूचकांक_has_changes(the_repository, शून्य, शून्य)) {
		अगर (allow_empty && is_empty_or_missing_file(am_path(state, "patch"))) {
			printf_ln(_("No changes - recorded it as an empty commit."));
		} अन्यथा {
			printf_ln(_("No changes - did you forget to use 'git add'?\n"
				    "If there is nothing left to stage, chances are that something else\n"
				    "already introduced the same changes; you might want to skip this patch."));
			die_user_resolve(state);
		}
	}

	अगर (unmerged_सूचकांक(the_repository->सूचकांक)) {
		printf_ln(_("You still have unmerged paths in your index.\n"
			"You should 'git add' each file with resolved conflicts to mark them as such.\n"
			"You might run `git rm` on a file to accept \"deleted by them\" for it."));
		die_user_resolve(state);
	}

	अगर (state->interactive) {
		write_सूचकांक_patch(state);
		अगर (do_interactive(state))
			जाओ अग्र;
	}

	repo_rerere(the_repository, 0);

	do_commit(state);

अग्र:
	am_अग्र(state);
	am_load(state);
	am_run(state, 0);
}

/**
 * Performs a checkout fast-forward from `head` to `remote`. If `reset` is
 * true, any unmerged entries will be discarded. Returns 0 on success, -1 on
 * failure.
 */
अटल पूर्णांक fast_forward_to(काष्ठा वृक्ष *शीर्ष, काष्ठा वृक्ष *remote, पूर्णांक reset)
{
	काष्ठा lock_file lock_file = LOCK_INIT;
	काष्ठा unpack_trees_options opts;
	काष्ठा वृक्ष_desc t[2];

	अगर (repo_parse_वृक्ष(the_repository, शीर्ष) || repo_parse_वृक्ष(the_repository, remote))
		वापस -1;

	repo_hold_locked_सूचकांक(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	refresh_सूचकांक(the_repository->सूचकांक, REFRESH_QUIET, शून्य, शून्य, शून्य);

	स_रखो(&opts, 0, माप(opts));
	opts.शीर्ष_idx = 1;
	opts.src_सूचकांक = the_repository->सूचकांक;
	opts.dst_सूचकांक = the_repository->सूचकांक;
	opts.update = 1;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = twoway_merge;
	init_वृक्ष_desc(&t[0], &शीर्ष->object.oid, शीर्ष->बफर, शीर्ष->आकार);
	init_वृक्ष_desc(&t[1], &remote->object.oid, remote->बफर, remote->आकार);

	अगर (unpack_trees(2, t, &opts)) {
		rollback_lock_file(&lock_file);
		वापस -1;
	}

	अगर (write_locked_सूचकांक(the_repository->सूचकांक, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	वापस 0;
}

/**
 * Merges a tree into the index. The index's stat info will take precedence
 * over the merged tree's. Returns 0 on success, -1 on failure.
 */
अटल पूर्णांक merge_वृक्ष(काष्ठा वृक्ष *वृक्ष)
{
	काष्ठा lock_file lock_file = LOCK_INIT;
	काष्ठा unpack_trees_options opts;
	काष्ठा वृक्ष_desc t[1];

	अगर (repo_parse_वृक्ष(the_repository, वृक्ष))
		वापस -1;

	repo_hold_locked_सूचकांक(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	स_रखो(&opts, 0, माप(opts));
	opts.शीर्ष_idx = 1;
	opts.src_सूचकांक = the_repository->सूचकांक;
	opts.dst_सूचकांक = the_repository->सूचकांक;
	opts.merge = 1;
	opts.fn = oneway_merge;
	init_वृक्ष_desc(&t[0], &वृक्ष->object.oid, वृक्ष->बफर, वृक्ष->आकार);

	अगर (unpack_trees(1, t, &opts)) {
		rollback_lock_file(&lock_file);
		वापस -1;
	}

	अगर (write_locked_सूचकांक(the_repository->सूचकांक, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	वापस 0;
}

/**
 * Clean the index without touching entries that are not modified between
 * `head` and `remote`.
 */
अटल पूर्णांक clean_सूचकांक(स्थिर काष्ठा object_id *शीर्ष, स्थिर काष्ठा object_id *remote)
{
	काष्ठा वृक्ष *शीर्ष_वृक्ष, *remote_वृक्ष, *सूचकांक_वृक्ष;
	काष्ठा object_id सूचकांक;

	शीर्ष_वृक्ष = repo_parse_वृक्ष_indirect(the_repository, शीर्ष);
	अगर (!शीर्ष_वृक्ष)
		वापस error(_("Could not parse object '%s'."), oid_to_hex(शीर्ष));

	remote_वृक्ष = repo_parse_वृक्ष_indirect(the_repository, remote);
	अगर (!remote_वृक्ष)
		वापस error(_("Could not parse object '%s'."), oid_to_hex(remote));

	repo_read_सूचकांक_unmerged(the_repository);

	अगर (fast_forward_to(शीर्ष_वृक्ष, शीर्ष_वृक्ष, 1))
		वापस -1;

	अगर (write_सूचकांक_as_वृक्ष(&सूचकांक, the_repository->सूचकांक,
				repo_get_सूचकांक_file(the_repository),
				0, शून्य))
		वापस -1;

	सूचकांक_वृक्ष = repo_parse_वृक्ष_indirect(the_repository, &सूचकांक);
	अगर (!सूचकांक_वृक्ष)
		वापस error(_("Could not parse object '%s'."), oid_to_hex(&सूचकांक));

	अगर (fast_forward_to(सूचकांक_वृक्ष, remote_वृक्ष, 0))
		वापस -1;

	अगर (merge_वृक्ष(remote_वृक्ष))
		वापस -1;

	remove_branch_state(the_repository, 0);

	वापस 0;
}

/**
 * Resets rerere's merge resolution metadata.
 */
अटल व्योम am_rerere_clear(व्योम)
{
	काष्ठा string_list merge_rr = STRING_LIST_INIT_DUP;
	rerere_clear(the_repository, &merge_rr);
	string_list_clear(&merge_rr, 1);
}

/**
 * Resume the current am session by skipping the current patch.
 */
अटल व्योम am_skip(काष्ठा am_state *state)
{
	काष्ठा object_id शीर्ष;

	am_rerere_clear();

	अगर (repo_get_oid(the_repository, "HEAD", &शीर्ष))
		oidcpy(&शीर्ष, the_hash_algo->empty_वृक्ष);

	अगर (clean_सूचकांक(&शीर्ष, &शीर्ष))
		die(_("failed to clean index"));

	अगर (state->rebasing) {
		खाता *fp = xfopen(am_path(state, "rewritten"), "a");

		निश्चित(!is_null_oid(&state->orig_commit));
		ख_लिखो(fp, "%s ", oid_to_hex(&state->orig_commit));
		ख_लिखो(fp, "%s\n", oid_to_hex(&शीर्ष));
		ख_बंद(fp);
	}

	am_अग्र(state);
	am_load(state);
	am_run(state, 0);
}

/**
 * Returns true if it is safe to reset HEAD to the ORIG_HEAD, false otherwise.
 *
 * It is not safe to reset HEAD when:
 * 1. git-am previously failed because the index was dirty.
 * 2. HEAD has moved since git-am previously failed.
 */
अटल पूर्णांक safe_to_abort(स्थिर काष्ठा am_state *state)
{
	काष्ठा strbuf sb = STRBUF_INIT;
	काष्ठा object_id abort_safety, शीर्ष;

	अगर (file_exists(am_path(state, "dirtyindex")))
		वापस 0;

	अगर (read_state_file(&sb, state, "abort-safety", 1) > 0) {
		अगर (get_oid_hex(sb.buf, &abort_safety))
			die(_("could not parse %s"), am_path(state, "abort-safety"));
	} अन्यथा
		oidclr(&abort_safety, the_repository->hash_algo);
	strbuf_release(&sb);

	अगर (repo_get_oid(the_repository, "HEAD", &शीर्ष))
		oidclr(&शीर्ष, the_repository->hash_algo);

	अगर (oideq(&शीर्ष, &abort_safety))
		वापस 1;

	warning(_("You seem to have moved HEAD since the last 'am' failure.\n"
		"Not rewinding to ORIG_HEAD"));

	वापस 0;
}

/**
 * Aborts the current am session if it is safe to do so.
 */
अटल व्योम am_abort(काष्ठा am_state *state)
{
	काष्ठा object_id curr_शीर्ष, orig_शीर्ष;
	पूर्णांक has_curr_शीर्ष, has_orig_शीर्ष;
	अक्षर *curr_branch;

	अगर (!safe_to_abort(state)) {
		am_destroy(state);
		वापस;
	}

	am_rerere_clear();

	curr_branch = refs_resolve_refdup(get_main_ref_store(the_repository),
					  "HEAD", 0, &curr_शीर्ष, शून्य);
	has_curr_शीर्ष = curr_branch && !is_null_oid(&curr_शीर्ष);
	अगर (!has_curr_शीर्ष)
		oidcpy(&curr_शीर्ष, the_hash_algo->empty_वृक्ष);

	has_orig_शीर्ष = !repo_get_oid(the_repository, "ORIG_HEAD", &orig_शीर्ष);
	अगर (!has_orig_शीर्ष)
		oidcpy(&orig_शीर्ष, the_hash_algo->empty_वृक्ष);

	अगर (clean_सूचकांक(&curr_शीर्ष, &orig_शीर्ष))
		die(_("failed to clean index"));

	अगर (has_orig_शीर्ष)
		refs_update_ref(get_main_ref_store(the_repository),
				"am --abort", "HEAD", &orig_शीर्ष,
				has_curr_शीर्ष ? &curr_शीर्ष : शून्य, 0,
				UPDATE_REFS_DIE_ON_ERR);
	अन्यथा अगर (curr_branch)
		refs_delete_ref(get_main_ref_store(the_repository), शून्य,
				curr_branch, शून्य, REF_NO_DEREF);

	मुक्त(curr_branch);
	am_destroy(state);
}

अटल पूर्णांक show_patch(काष्ठा am_state *state, क्रमागत resume_type resume_mode)
{
	काष्ठा strbuf sb = STRBUF_INIT;
	स्थिर अक्षर *patch_path;
	पूर्णांक len;

	अगर (!is_null_oid(&state->orig_commit)) {
		काष्ठा child_process cmd = CHILD_PROCESS_INIT;

		strvec_pushl(&cmd.args, "show", oid_to_hex(&state->orig_commit),
			     "--", शून्य);
		cmd.git_cmd = 1;
		वापस run_command(&cmd);
	}

	चयन (resume_mode) {
	हाल RESUME_SHOW_PATCH_RAW:
		patch_path = am_path(state, msgnum(state));
		अवरोध;
	हाल RESUME_SHOW_PATCH_DIFF:
		patch_path = am_path(state, "patch");
		अवरोध;
	शेष:
		BUG("invalid mode for --show-current-patch");
	}

	len = strbuf_read_file(&sb, patch_path, 0);
	अगर (len < 0)
		die_errno(_("failed to read '%s'"), patch_path);

	setup_pager(the_repository);
	write_in_full(1, sb.buf, sb.len);
	strbuf_release(&sb);
	वापस 0;
}

/**
 * parse_options() callback that validates and sets opt->value to the
 * PATCH_FORMAT_* enum value corresponding to `arg`.
 */
अटल पूर्णांक parse_opt_patchformat(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	पूर्णांक *opt_मूल्य = opt->मूल्य;

	अगर (unset)
		*opt_मूल्य = PATCH_FORMAT_UNKNOWN;
	अन्यथा अगर (!म_भेद(arg, "mbox"))
		*opt_मूल्य = PATCH_FORMAT_MBOX;
	अन्यथा अगर (!म_भेद(arg, "stgit"))
		*opt_मूल्य = PATCH_FORMAT_STGIT;
	अन्यथा अगर (!म_भेद(arg, "stgit-series"))
		*opt_मूल्य = PATCH_FORMAT_STGIT_SERIES;
	अन्यथा अगर (!म_भेद(arg, "hg"))
		*opt_मूल्य = PATCH_FORMAT_HG;
	अन्यथा अगर (!म_भेद(arg, "mboxrd"))
		*opt_मूल्य = PATCH_FORMAT_MBOXRD;
	/*
	 * Please update $__git_patchformat in git-completion.bash
	 * when you add new options
	 */
	अन्यथा
		वापस error(_("invalid value for '%s': '%s'"),
			     "--patch-format", arg);
	वापस 0;
}

अटल पूर्णांक parse_opt_show_current_patch(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	पूर्णांक *opt_मूल्य = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	अगर (!arg)
		*opt_मूल्य = opt->defval;
	अन्यथा अगर (!म_भेद(arg, "raw"))
		*opt_मूल्य = RESUME_SHOW_PATCH_RAW;
	अन्यथा अगर (!म_भेद(arg, "diff"))
		*opt_मूल्य = RESUME_SHOW_PATCH_DIFF;
	/*
	 * Please update $__git_showcurrentpatch in git-completion.bash
	 * when you add new options
	 */
	अन्यथा
		वापस error(_("invalid value for '%s': '%s'"),
			     "--show-current-patch", arg);
	वापस 0;
}

पूर्णांक cmd_am(पूर्णांक argc,
	   स्थिर अक्षर **argv,
	   स्थिर अक्षर *prefix,
	   काष्ठा repository *repo UNUSED)
{
	काष्ठा am_state state;
	पूर्णांक द्विआधारी = -1;
	पूर्णांक keep_cr = -1;
	पूर्णांक patch_format = PATCH_FORMAT_UNKNOWN;
	क्रमागत resume_type resume_mode = RESUME_FALSE;
	पूर्णांक in_progress;
	पूर्णांक ret = 0;

	स्थिर अक्षर * स्थिर usage[] = {
		N_("git am [<options>] [(<mbox> | <Maildir>)...]"),
		N_("git am [<options>] (--continue | --skip | --abort)"),
		शून्य
	};

	काष्ठा option options[] = {
		OPT_BOOL('i', "interactive", &state.interactive,
			N_("run interactively")),
		OPT_BOOL('n', "no-verify", &state.no_verify,
			N_("bypass pre-applypatch and applypatch-msg hooks")),
		OPT_HIDDEN_BOOL('b', "binary", &द्विआधारी,
			N_("historical option -- no-op")),
		OPT_BOOL('3', "3way", &state.threeway,
			N_("allow fall back on 3way merging if needed")),
		OPT__QUIET(&state.quiet, N_("be quiet")),
		OPT_SET_INT('s', "signoff", &state.signoff,
			N_("add a Signed-off-by trailer to the commit message"),
			SIGNOFF_EXPLICIT),
		OPT_BOOL('u', "utf8", &state.utf8,
			N_("recode into utf8 (default)")),
		OPT_SET_INT('k', "keep", &state.keep,
			N_("pass -k flag to git-mailinfo"), KEEP_TRUE),
		OPT_SET_INT(0, "keep-non-patch", &state.keep,
			N_("pass -b flag to git-mailinfo"), KEEP_NON_PATCH),
		OPT_BOOL('m', "message-id", &state.message_id,
			N_("pass -m flag to git-mailinfo")),
		OPT_SET_INT(0, "keep-cr", &keep_cr,
			    N_("pass --keep-cr flag to git-mailsplit for mbox format"),
			    1),
		OPT_BOOL('c', "scissors", &state.scissors,
			N_("strip everything before a scissors line")),
		OPT_CALLBACK_F(0, "quoted-cr", &state.quoted_cr, N_("action"),
			       N_("pass it through git-mailinfo"),
			       PARSE_OPT_NONEG, am_option_parse_quoted_cr),
		OPT_PASSTHRU_ARGV(0, "whitespace", &state.git_apply_opts, N_("action"),
			N_("pass it through git-apply"),
			0),
		OPT_PASSTHRU_ARGV(0, "ignore-space-change", &state.git_apply_opts, शून्य,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU_ARGV(0, "ignore-whitespace", &state.git_apply_opts, शून्य,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU_ARGV(0, "directory", &state.git_apply_opts, N_("root"),
			N_("pass it through git-apply"),
			0),
		OPT_PASSTHRU_ARGV(0, "exclude", &state.git_apply_opts, N_("path"),
			N_("pass it through git-apply"),
			0),
		OPT_PASSTHRU_ARGV(0, "include", &state.git_apply_opts, N_("path"),
			N_("pass it through git-apply"),
			0),
		OPT_PASSTHRU_ARGV('C', शून्य, &state.git_apply_opts, N_("n"),
			N_("pass it through git-apply"),
			0),
		OPT_PASSTHRU_ARGV('p', शून्य, &state.git_apply_opts, N_("num"),
			N_("pass it through git-apply"),
			0),
		OPT_CALLBACK(0, "patch-format", &patch_format, N_("format"),
			N_("format the patch(es) are in"),
			parse_opt_patchformat),
		OPT_PASSTHRU_ARGV(0, "reject", &state.git_apply_opts, शून्य,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_STRING(0, "resolvemsg", &state.resolvemsg, शून्य,
			N_("override error message when patch failure occurs")),
		OPT_CMDMODE(0, "continue", &resume_mode,
			N_("continue applying patches after resolving a conflict"),
			RESUME_RESOLVED),
		OPT_CMDMODE('r', "resolved", &resume_mode,
			N_("synonyms for --continue"),
			RESUME_RESOLVED),
		OPT_CMDMODE(0, "skip", &resume_mode,
			N_("skip the current patch"),
			RESUME_SKIP),
		OPT_CMDMODE(0, "abort", &resume_mode,
			N_("restore the original branch and abort the patching operation"),
			RESUME_ABORT),
		OPT_CMDMODE(0, "quit", &resume_mode,
			N_("abort the patching operation but keep HEAD where it is"),
			RESUME_QUIT),
		{
			.type = OPTION_CALLBACK,
			.long_name = "show-current-patch",
			.मूल्य = &resume_mode,
			.precision = माप(resume_mode),
			.argh = "(diff|raw)",
			.help = N_("show the patch being applied"),
			.flags = PARSE_OPT_CMDMODE | PARSE_OPT_OPTARG | PARSE_OPT_NONEG | PARSE_OPT_LITERAL_ARGHELP,
			.callback = parse_opt_show_current_patch,
			.defval = RESUME_SHOW_PATCH_RAW,
		},
		OPT_CMDMODE(0, "retry", &resume_mode,
			N_("try to apply current patch again"),
			RESUME_APPLY),
		OPT_CMDMODE(0, "allow-empty", &resume_mode,
			N_("record the empty patch as an empty commit"),
			RESUME_ALLOW_EMPTY),
		OPT_BOOL(0, "committer-date-is-author-date",
			&state.committer_date_is_author_date,
			N_("lie about committer date")),
		OPT_BOOL(0, "ignore-date", &state.ignore_date,
			N_("use current timestamp for author date")),
		OPT_RERERE_AUTOUPDATE(&state.allow_rerere_autoupdate),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.मूल्य = &state.sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG-sign commits"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_CALLBACK_F(0, "empty", &state.empty_type, "(stop|drop|keep)",
		  N_("how to handle empty patches"),
		  PARSE_OPT_NONEG, am_option_parse_empty),
		OPT_HIDDEN_BOOL(0, "rebasing", &state.rebasing,
			N_("(internal use for git-rebase)")),
		OPT_END()
	};

	show_usage_with_options_if_asked(argc, argv, usage, options);

	repo_config(the_repository, git_default_config, शून्य);

	am_state_init(&state);

	in_progress = am_in_progress(&state);
	अगर (in_progress)
		am_load(&state);

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	अगर (द्विआधारी >= 0)
		fprintf_ln(मानक_त्रुटि, _("The -b/--binary option has been a no-op for long time, and\n"
				"it will be removed. Please do not use it anymore."));

	/* Ensure a valid committer ident can be constructed */
	git_committer_info(IDENT_STRICT);

	अगर (repo_read_सूचकांक_preload(the_repository, शून्य, 0) < 0)
		die(_("failed to read the index"));

	अगर (in_progress) {
		/*
		 * Catch user error to feed us patches when there is a session
		 * in progress:
		 *
		 * 1. mbox path(s) are provided on the command-line.
		 * 2. stdin is not a tty: the user is trying to feed us a patch
		 *    from standard input. This is somewhat unreliable -- stdin
		 *    could be /dev/null for example and the caller did not
		 *    intend to feed us a patch but wanted to continue
		 *    unattended.
		 */
		अगर (argc || (resume_mode == RESUME_FALSE && !isatty(0)))
			die(_("previous rebase directory %s still exists but mbox given."),
				state.dir);

		अगर (resume_mode == RESUME_FALSE)
			resume_mode = RESUME_APPLY;

		अगर (state.signoff == SIGNOFF_EXPLICIT)
			am_append_signoff(&state);
	} अन्यथा {
		काष्ठा strvec paths = STRVEC_INIT;
		पूर्णांक i;

		/*
		 * Handle stray state directory in the independent-run case. In
		 * the --rebasing case, it is up to the caller to take care of
		 * stray directories.
		 */
		अगर (file_exists(state.dir) && !state.rebasing) {
			अगर (resume_mode == RESUME_ABORT || resume_mode == RESUME_QUIT) {
				am_destroy(&state);
				am_state_release(&state);
				वापस 0;
			}

			die(_("Stray %s directory found.\n"
				"Use \"git am --abort\" to remove it."),
				state.dir);
		}

		अगर (resume_mode)
			die(_("Resolve operation not in progress, we are not resuming."));

		क्रम (i = 0; i < argc; i++) {
			अगर (is_absolute_path(argv[i]) || !prefix)
				strvec_धकेलो(&paths, argv[i]);
			अन्यथा
				strvec_धकेलो(&paths, mkpath("%s/%s", prefix, argv[i]));
		}

		अगर (state.interactive && !paths.nr)
			die(_("interactive mode requires patches on the command line"));

		am_setup(&state, patch_format, paths.v, keep_cr);

		strvec_clear(&paths);
	}

	चयन (resume_mode) {
	हाल RESUME_FALSE:
		am_run(&state, 0);
		अवरोध;
	हाल RESUME_APPLY:
		am_run(&state, 1);
		अवरोध;
	हाल RESUME_RESOLVED:
	हाल RESUME_ALLOW_EMPTY:
		am_resolve(&state, resume_mode == RESUME_ALLOW_EMPTY ? 1 : 0);
		अवरोध;
	हाल RESUME_SKIP:
		am_skip(&state);
		अवरोध;
	हाल RESUME_ABORT:
		am_abort(&state);
		अवरोध;
	हाल RESUME_QUIT:
		am_rerere_clear();
		am_destroy(&state);
		अवरोध;
	हाल RESUME_SHOW_PATCH_RAW:
	हाल RESUME_SHOW_PATCH_DIFF:
		ret = show_patch(&state, resume_mode);
		अवरोध;
	शेष:
		BUG("invalid resume value");
	}

	am_state_release(&state);

	वापस ret;
}
