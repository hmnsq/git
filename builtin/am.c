/*
 * Builtin "git am"
 *
 * Based on git-am.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "abspath.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "editor.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "hook.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "tempfile.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "cache-tree.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "unpack-trees.h"
#अन्तर्भाव "branch.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "preload-index.h"
#अन्तर्भाव "sequencer.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "merge-ort-wrappers.h"
#अन्तर्भाव "log-tree.h"
#अन्तर्भाव "notes-utils.h"
#अन्तर्भाव "rerere.h"
#अन्तर्भाव "mailinfo.h"
#अन्तर्भाव "apply.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "pager.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "pretty.h"

/**
 * Returns the length of the first line of msg.
 */
स्थिर पूर्णांक linelen(स्थिरांक अक्षर *संदेश)
{
	परत strchrnul(संदेश, '\n') - संदेश;
}

/**
 * Returns true if `str` consists of only whitespace, false otherwise.
 */
स्थिर पूर्णांक str_isspace(स्थिरांक अक्षर *str)
{
	साठी (; *str; str++)
		जर (!है_खाली(*str))
			परत 0;

	परत 1;
}

गणना patch_format {
	PATCH_FORMAT_UNKNOWN = 0,
	PATCH_FORMAT_MBOX,
	PATCH_FORMAT_STGIT,
	PATCH_FORMAT_STGIT_SERIES,
	PATCH_FORMAT_HG,
	PATCH_FORMAT_MBOXRD
};

गणना keep_type {
	KEEP_FALSE = 0,
	KEEP_TRUE,      /* pass -k flag to git-mailinfo */
	KEEP_NON_PATCH  /* pass -b flag to git-mailinfo */
};

गणना scissors_type {
	SCISSORS_UNSET = -1,
	SCISSORS_FALSE = 0,  /* pass --no-scissors to git-mailinfo */
	SCISSORS_TRUE        /* pass --scissors to git-mailinfo */
};

गणना signoff_type {
	SIGNOFF_FALSE = 0,
	SIGNOFF_TRUE = 1,
	SIGNOFF_EXPLICIT /* --signoff was set on the command-line */
};

गणना resume_type {
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

गणना empty_action {
	STOP_ON_EMPTY_COMMIT = 0,  /* output errors and stop in the middle of an am session */
	DROP_EMPTY_COMMIT,         /* skip with a notice message, unless "--quiet" has been passed */
	KEEP_EMPTY_COMMIT,         /* keep recording as empty commits */
};

संरचना am_state {
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
	संरचना object_id orig_commit;

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
	संरचना strvec git_apply_opts;
	स्थिरांक अक्षर *resolvemsg;
	पूर्णांक committer_date_is_author_date;
	पूर्णांक ignore_date;
	पूर्णांक allow_rerere_autoupdate;
	स्थिरांक अक्षर *sign_commit;
	पूर्णांक rebasing;
};

/**
 * Initializes am_state with the default values.
 */
स्थिर शून्य am_state_init(संरचना am_state *state)
{
	पूर्णांक gpgsign;

	स_रखो(state, 0, आकार(*state));

	state->dir = repo_git_path(the_repository, "rebase-apply");

	state->prec = 4;

	repo_config_get_bool(the_repository, "am.threeway", &state->threeway);

	state->utf8 = 1;

	repo_config_get_bool(the_repository, "am.messageid", &state->message_id);

	state->scissors = SCISSORS_UNSET;
	state->quoted_cr = quoted_cr_unset;

	strvec_init(&state->git_apply_opts);

	जर (!repo_config_get_bool(the_repository, "commit.gpgsign", &gpgsign))
		state->sign_commit = gpgsign ? "" : रिक्त;
}

/**
 * Releases memory allocated by an am_state.
 */
स्थिर शून्य am_state_release(संरचना am_state *state)
{
	मुक्त(state->dir);
	मुक्त(state->author_name);
	मुक्त(state->author_email);
	मुक्त(state->author_date);
	मुक्त(state->संदेश);
	strvec_clear(&state->git_apply_opts);
}

स्थिर पूर्णांक am_option_parse_quoted_cr(स्थिरांक संरचना option *opt,
				     स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	BUG_ON_OPT_NEG(unset);

	जर (mailinfo_parse_quoted_cr_action(arg, opt->मूल्य) != 0)
		परत error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	परत 0;
}

स्थिर पूर्णांक am_option_parse_empty(स्थिरांक संरचना option *opt,
				     स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	पूर्णांक *opt_मूल्य = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	जर (!म_भेद(arg, "stop"))
		*opt_मूल्य = STOP_ON_EMPTY_COMMIT;
	अन्यथा जर (!म_भेद(arg, "drop"))
		*opt_मूल्य = DROP_EMPTY_COMMIT;
	अन्यथा जर (!म_भेद(arg, "keep"))
		*opt_मूल्य = KEEP_EMPTY_COMMIT;
	अन्यथा
		परत error(_("invalid value for '%s': '%s'"), "--empty", arg);

	परत 0;
}

/**
 * Returns path relative to the am_state directory.
 */
स्थिर ओळीत स्थिरांक अक्षर *am_path(स्थिरांक संरचना am_state *state, स्थिरांक अक्षर *path)
{
	परत mkpath("%s/%s", state->dir, path);
}

/**
 * For convenience to call write_file()
 */
स्थिर शून्य write_state_text(स्थिरांक संरचना am_state *state,
			     स्थिरांक अक्षर *name, स्थिरांक अक्षर *string)
{
	write_file(am_path(state, name), "%s", string);
}

स्थिर शून्य write_state_मोजणी(स्थिरांक संरचना am_state *state,
			      स्थिरांक अक्षर *name, पूर्णांक मूल्य)
{
	write_file(am_path(state, name), "%d", मूल्य);
}

स्थिर शून्य write_state_bool(स्थिरांक संरचना am_state *state,
			     स्थिरांक अक्षर *name, पूर्णांक मूल्य)
{
	write_state_text(state, name, मूल्य ? "t" : "f");
}

/**
 * If state->quiet is false, calls fprintf(fp, fmt, ...), and appends a newline
 * at the end.
 */
__attribute__((format (म_लिखो, 3, 4)))
स्थिर शून्य say(स्थिरांक संरचना am_state *state, खाता *fp, स्थिरांक अक्षर *fmt, ...)
{
	बहु_सूची ap;

	बहु_शुरू(ap, fmt);
	जर (!state->quiet) {
		भख_लिखो(fp, fmt, ap);
		अ_दो('\n', fp);
	}
	बहु_पूर्ण(ap);
}

/**
 * Returns 1 if there is an am session in progress, 0 otherwise.
 */
स्थिर पूर्णांक am_in_progress(स्थिरांक संरचना am_state *state)
{
	संरचना stat st;

	जर (lstat(state->dir, &st) < 0 || !S_ISDIR(st.st_mode))
		परत 0;
	जर (lstat(am_path(state, "last"), &st) || !S_ISREG(st.st_mode))
		परत 0;
	जर (lstat(am_path(state, "next"), &st) || !S_ISREG(st.st_mode))
		परत 0;
	परत 1;
}

/**
 * Reads the contents of `file` in the `state` directory into `sb`. Returns the
 * number of bytes read on success, -1 if the file does not exist. If `trim` is
 * set, trailing whitespace will be removed.
 */
स्थिर पूर्णांक read_state_file(संरचना strbuf *sb, स्थिरांक संरचना am_state *state,
			स्थिरांक अक्षर *file, पूर्णांक trim)
{
	strbuf_reset(sb);

	जर (strbuf_read_file(sb, am_path(state, file), 0) >= 0) {
		जर (trim)
			strbuf_trim(sb);

		परत sb->len;
	}

	जर (त्रुटि_सं == ENOENT)
		परत -1;

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
स्थिर पूर्णांक read_am_author_script(संरचना am_state *state)
{
	स्थिरांक अक्षर *filename = am_path(state, "author-script");

	प्रतिज्ञा(!state->author_name);
	प्रतिज्ञा(!state->author_email);
	प्रतिज्ञा(!state->author_date);

	परत read_author_script(filename, &state->author_name,
				  &state->author_email, &state->author_date, 1);
}

/**
 * Saves state->author_name, state->author_email and state->author_date in the
 * state directory's "author-script" file.
 */
स्थिर शून्य write_author_script(स्थिरांक संरचना am_state *state)
{
	संरचना strbuf sb = STRBUF_INIT;

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
स्थिर पूर्णांक read_commit_संदेश(संरचना am_state *state)
{
	संरचना strbuf sb = STRBUF_INIT;

	प्रतिज्ञा(!state->संदेश);

	जर (read_state_file(&sb, state, "final-commit", 0) < 0) {
		strbuf_release(&sb);
		परत -1;
	}

	state->संदेश = strbuf_detach(&sb, &state->संदेश_len);
	परत 0;
}

/**
 * Saves state->msg in the state directory's "final-commit" file.
 */
स्थिर शून्य write_commit_संदेश(स्थिरांक संरचना am_state *state)
{
	स्थिरांक अक्षर *filename = am_path(state, "final-commit");
	write_file_buf(filename, state->संदेश, state->संदेश_len);
}

/**
 * Loads state from disk.
 */
स्थिर शून्य am_load(संरचना am_state *state)
{
	संरचना strbuf sb = STRBUF_INIT;

	जर (read_state_file(&sb, state, "next", 1) < 0)
		BUG("state file 'next' does not exist");
	state->cur = म_से_दीर्घ(sb.buf, रिक्त, 10);

	जर (read_state_file(&sb, state, "last", 1) < 0)
		BUG("state file 'last' does not exist");
	state->last = म_से_दीर्घ(sb.buf, रिक्त, 10);

	जर (read_am_author_script(state) < 0)
		die(_("could not parse author script"));

	read_commit_संदेश(state);

	जर (read_state_file(&sb, state, "original-commit", 1) < 0)
		oidclr(&state->orig_commit, the_repository->hash_algo);
	अन्यथा जर (get_oid_hex(sb.buf, &state->orig_commit) < 0)
		die(_("could not parse %s"), am_path(state, "original-commit"));

	read_state_file(&sb, state, "threeway", 1);
	state->threeway = !म_भेद(sb.buf, "t");

	read_state_file(&sb, state, "quiet", 1);
	state->quiet = !म_भेद(sb.buf, "t");

	read_state_file(&sb, state, "sign", 1);
	state->signoff = !म_भेद(sb.buf, "t");

	read_state_file(&sb, state, "utf8", 1);
	state->utf8 = !म_भेद(sb.buf, "t");

	जर (file_exists(am_path(state, "rerere-autoupdate"))) {
		read_state_file(&sb, state, "rerere-autoupdate", 1);
		state->allow_rerere_autoupdate = म_भेद(sb.buf, "t") ?
			RERERE_NOAUTOUPDATE : RERERE_AUTOUPDATE;
	} अन्यथा {
		state->allow_rerere_autoupdate = 0;
	}

	read_state_file(&sb, state, "keep", 1);
	जर (!म_भेद(sb.buf, "t"))
		state->keep = KEEP_TRUE;
	अन्यथा जर (!म_भेद(sb.buf, "b"))
		state->keep = KEEP_NON_PATCH;
	अन्यथा
		state->keep = KEEP_FALSE;

	read_state_file(&sb, state, "messageid", 1);
	state->message_id = !म_भेद(sb.buf, "t");

	read_state_file(&sb, state, "scissors", 1);
	जर (!म_भेद(sb.buf, "t"))
		state->scissors = SCISSORS_TRUE;
	अन्यथा जर (!म_भेद(sb.buf, "f"))
		state->scissors = SCISSORS_FALSE;
	अन्यथा
		state->scissors = SCISSORS_UNSET;

	read_state_file(&sb, state, "quoted-cr", 1);
	जर (!*sb.buf)
		state->quoted_cr = quoted_cr_unset;
	अन्यथा जर (mailinfo_parse_quoted_cr_action(sb.buf, &state->quoted_cr) != 0)
		die(_("could not parse %s"), am_path(state, "quoted-cr"));

	read_state_file(&sb, state, "apply-opt", 1);
	strvec_clear(&state->git_apply_opts);
	जर (sq_dequote_to_strvec(sb.buf, &state->git_apply_opts) < 0)
		die(_("could not parse %s"), am_path(state, "apply-opt"));

	state->rebasing = !!file_exists(am_path(state, "rebasing"));

	strbuf_release(&sb);
}

/**
 * Removes the am_state directory, forcefully terminating the current am
 * session.
 */
स्थिर शून्य am_destroy(स्थिरांक संरचना am_state *state)
{
	संरचना strbuf sb = STRBUF_INIT;

	strbuf_addstr(&sb, state->dir);
	remove_dir_recursively(&sb, 0);
	strbuf_release(&sb);
}

/**
 * Runs applypatch-msg hook. Returns its exit code.
 */
स्थिर पूर्णांक run_applypatch_संदेश_hook(संरचना am_state *state)
{
	पूर्णांक ret = 0;

	प्रतिज्ञा(state->संदेश);

	जर (!state->no_verify) {
		संरचना run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;
		strvec_ढकला(&opt.args, am_path(state, "final-commit"));
		ret = run_hooks_opt(the_repository, "applypatch-msg", &opt);
	}

	जर (!ret) {
		FREE_AND_NULL(state->संदेश);
		जर (read_commit_संदेश(state) < 0)
			die(_("'%s' was deleted by the applypatch-msg hook"),
				am_path(state, "final-commit"));
	}

	परत ret;
}

/**
 * Runs post-rewrite hook. Returns it exit code.
 */
स्थिर पूर्णांक run_post_rewrite_hook(स्थिरांक संरचना am_state *state)
{
	संरचना run_hooks_opt opt = RUN_HOOKS_OPT_INIT;

	strvec_ढकला(&opt.args, "rebase");
	opt.path_to_stdin = am_path(state, "rewritten");

	परत run_hooks_opt(the_repository, "post-rewrite", &opt);
}

/**
 * Reads the state directory's "rewritten" file, and copies notes from the old
 * commits listed in the file to their rewritten commits.
 *
 * Returns 0 on success, -1 on failure.
 */
स्थिर पूर्णांक copy_notes_for_rebase(स्थिरांक संरचना am_state *state)
{
	संरचना notes_rewrite_cfg *c;
	संरचना strbuf sb = STRBUF_INIT;
	स्थिरांक अक्षर *invalid_ओळ = _("Malformed input line: '%s'.");
	स्थिरांक अक्षर *संदेश = "Notes added by 'git rebase'";
	खाता *fp;
	पूर्णांक ret = 0;

	प्रतिज्ञा(state->rebasing);

	c = init_copy_notes_for_rewrite("rebase");
	जर (!c)
		परत 0;

	fp = xfopen(am_path(state, "rewritten"), "r");

	पर्यंत (!strbuf_getline_lf(&sb, fp)) {
		संरचना object_id from_obj, to_obj;
		स्थिरांक अक्षर *p;

		जर (sb.len != the_hash_algo->hexsz * 2 + 1) {
			ret = error(invalid_ओळ, sb.buf);
			जा finish;
		}

		जर (parse_oid_hex(sb.buf, &from_obj, &p)) {
			ret = error(invalid_ओळ, sb.buf);
			जा finish;
		}

		जर (*p != ' ') {
			ret = error(invalid_ओळ, sb.buf);
			जा finish;
		}

		जर (get_oid_hex(p + 1, &to_obj)) {
			ret = error(invalid_ओळ, sb.buf);
			जा finish;
		}

		जर (copy_note_for_rewrite(c, &from_obj, &to_obj))
			ret = error(_("Failed to copy notes from '%s' to '%s'"),
					oid_to_hex(&from_obj), oid_to_hex(&to_obj));
	}

finish:
	finish_copy_notes_for_rewrite(the_repository, c, संदेश);
	ख_बंद(fp);
	strbuf_release(&sb);
	परत ret;
}

/**
 * Determines if the file looks like a piece of RFC2822 mail by grabbing all
 * non-indented lines and checking if they look like they begin with valid
 * header field names.
 *
 * Returns 1 if the file looks like a piece of mail, 0 otherwise.
 */
स्थिर पूर्णांक is_mail(खाता *fp)
{
	स्थिरांक अक्षर *header_regex = "^[!-9;-~]+:";
	संरचना strbuf sb = STRBUF_INIT;
	regex_t regex;
	पूर्णांक ret = 1;

	जर (ख_जाओ(fp, 0L, शुरू_से))
		die_errno(_("fseek failed"));

	जर (regcomp(&regex, header_regex, REG_NOSUB | REG_EXTENDED))
		die("invalid pattern: %s", header_regex);

	पर्यंत (!strbuf_getline(&sb, fp)) {
		जर (!sb.len)
			थांबा; /* End of header */

		/* Ignore indented folded lines */
		जर (*sb.buf == '\t' || *sb.buf == ' ')
			सुरूठेवा;

		/* It's a header if it matches header_regex */
		जर (regexec(&regex, sb.buf, 0, रिक्त, 0)) {
			ret = 0;
			जा done;
		}
	}

done:
	regfree(&regex);
	strbuf_release(&sb);
	परत ret;
}

/**
 * Attempts to detect the patch_format of the patches contained in `paths`,
 * returning the PATCH_FORMAT_* enum value. Returns PATCH_FORMAT_UNKNOWN if
 * detection fails.
 */
स्थिर पूर्णांक detect_patch_format(स्थिरांक अक्षर **paths)
{
	गणना patch_format ret = PATCH_FORMAT_UNKNOWN;
	संरचना strbuf l1 = STRBUF_INIT;
	संरचना strbuf l2 = STRBUF_INIT;
	संरचना strbuf l3 = STRBUF_INIT;
	खाता *fp;

	/*
	 * We default to mbox format if input is from stdin and for directories
	 */
	जर (!*paths || !म_भेद(*paths, "-") || is_directory(*paths))
		परत PATCH_FORMAT_MBOX;

	/*
	 * Otherwise, check the first few lines of the first patch, starting
	 * from the first non-blank line, to try to detect its format.
	 */

	fp = xfopen(*paths, "r");

	पर्यंत (!strbuf_getline(&l1, fp)) {
		जर (l1.len)
			थांबा;
	}

	जर (starts_with(l1.buf, "From ") || starts_with(l1.buf, "From: ")) {
		ret = PATCH_FORMAT_MBOX;
		जा done;
	}

	जर (starts_with(l1.buf, "# This series applies on GIT commit")) {
		ret = PATCH_FORMAT_STGIT_SERIES;
		जा done;
	}

	जर (!म_भेद(l1.buf, "# HG changeset patch")) {
		ret = PATCH_FORMAT_HG;
		जा done;
	}

	strbuf_getline(&l2, fp);
	strbuf_getline(&l3, fp);

	/*
	 * If the second line is empty and the third is a From, Author or Date
	 * entry, this is likely an StGit patch.
	 */
	जर (l1.len && !l2.len &&
		(starts_with(l3.buf, "From:") ||
		 starts_with(l3.buf, "Author:") ||
		 starts_with(l3.buf, "Date:"))) {
		ret = PATCH_FORMAT_STGIT;
		जा done;
	}

	जर (l1.len && is_mail(fp)) {
		ret = PATCH_FORMAT_MBOX;
		जा done;
	}

done:
	ख_बंद(fp);
	strbuf_release(&l1);
	strbuf_release(&l2);
	strbuf_release(&l3);
	परत ret;
}

/**
 * Splits out individual email patches from `paths`, where each path is either
 * a mbox file or a Maildir. Returns 0 on success, -1 on failure.
 */
स्थिर पूर्णांक split_mail_mbox(संरचना am_state *state, स्थिरांक अक्षर **paths,
				पूर्णांक keep_cr, पूर्णांक mboxrd)
{
	संरचना child_process cp = CHILD_PROCESS_INIT;
	संरचना strbuf last = STRBUF_INIT;
	पूर्णांक ret;

	cp.git_cmd = 1;
	strvec_ढकला(&cp.args, "mailsplit");
	strvec_pushf(&cp.args, "-d%d", state->prec);
	strvec_pushf(&cp.args, "-o%s", state->dir);
	strvec_ढकला(&cp.args, "-b");
	जर (keep_cr)
		strvec_ढकला(&cp.args, "--keep-cr");
	जर (mboxrd)
		strvec_ढकला(&cp.args, "--mboxrd");
	strvec_ढकला(&cp.args, "--");
	strvec_pushv(&cp.args, paths);

	ret = capture_command(&cp, &last, 8);
	जर (ret)
		जा निकास;

	state->cur = 1;
	state->last = म_से_दीर्घ(last.buf, रिक्त, 10);

निकास:
	strbuf_release(&last);
	परत ret ? -1 : 0;
}

/**
 * Callback signature for split_mail_conv(). The foreign patch should be
 * read from `in`, and the converted patch (in RFC2822 mail format) should be
 * written to `out`. Return 0 on success, or -1 on failure.
 */
प्रकारव्याख्या पूर्णांक (*mail_conv_fn)(खाता *out, खाता *in, पूर्णांक keep_cr);

/**
 * Calls `fn` for each file in `paths` to convert the foreign patch to the
 * RFC2822 mail format suitable for parsing with git-mailinfo.
 *
 * Returns 0 on success, -1 on failure.
 */
स्थिर पूर्णांक split_mail_conv(mail_conv_fn fn, संरचना am_state *state,
			स्थिरांक अक्षर **paths, पूर्णांक keep_cr)
{
	स्थिर स्थिरांक अक्षर *stdin_only[] = {"-", रिक्त};
	पूर्णांक i;

	जर (!*paths)
		paths = stdin_only;

	साठी (i = 0; *paths; paths++, i++) {
		खाता *in, *out;
		स्थिरांक अक्षर *mail;
		पूर्णांक ret;

		जर (!म_भेद(*paths, "-"))
			in = मानक_निवेश;
		अन्यथा
			in = ख_खोलो(*paths, "r");

		जर (!in)
			परत error_errno(_("could not open '%s' for reading"),
					   *paths);

		mail = mkpath("%s/%0*d", state->dir, state->prec, i + 1);

		out = ख_खोलो(mail, "w");
		जर (!out) {
			जर (in != मानक_निवेश)
				ख_बंद(in);
			परत error_errno(_("could not open '%s' for writing"),
					   mail);
		}

		ret = fn(out, in, keep_cr);

		ख_बंद(out);
		जर (in != मानक_निवेश)
			ख_बंद(in);

		जर (ret)
			परत error(_("could not parse patch '%s'"), *paths);
	}

	state->cur = 1;
	state->last = i;
	परत 0;
}

/**
 * A split_mail_conv() callback that converts an StGit patch to an RFC2822
 * message suitable for parsing with git-mailinfo.
 */
स्थिर पूर्णांक stgit_patch_to_mail(खाता *out, खाता *in, पूर्णांक keep_cr UNUSED)
{
	संरचना strbuf sb = STRBUF_INIT;
	पूर्णांक subject_printed = 0;

	पर्यंत (!strbuf_getline_lf(&sb, in)) {
		स्थिरांक अक्षर *str;

		जर (str_isspace(sb.buf))
			सुरूठेवा;
		अन्यथा जर (skip_prefix(sb.buf, "Author:", &str))
			ख_लिखो(out, "From:%s\n", str);
		अन्यथा जर (starts_with(sb.buf, "From") || starts_with(sb.buf, "Date"))
			ख_लिखो(out, "%s\n", sb.buf);
		अन्यथा जर (!subject_printed) {
			ख_लिखो(out, "Subject: %s\n", sb.buf);
			subject_printed = 1;
		} अन्यथा {
			ख_लिखो(out, "\n%s\n", sb.buf);
			थांबा;
		}
	}

	strbuf_reset(&sb);
	पर्यंत (strbuf_fread(&sb, 8192, in) > 0) {
		ख_डालो(sb.buf, 1, sb.len, out);
		strbuf_reset(&sb);
	}

	strbuf_release(&sb);
	परत 0;
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
स्थिर पूर्णांक split_mail_stgit_series(संरचना am_state *state, स्थिरांक अक्षर **paths,
					पूर्णांक keep_cr)
{
	स्थिरांक अक्षर *series_dir;
	अक्षर *series_dir_buf;
	खाता *fp;
	संरचना strvec patches = STRVEC_INIT;
	संरचना strbuf sb = STRBUF_INIT;
	पूर्णांक ret;

	जर (!paths[0] || paths[1])
		परत error(_("Only one StGIT patch series can be applied at once"));

	series_dir_buf = xstrdup(*paths);
	series_dir = स_नाम(series_dir_buf);

	fp = ख_खोलो(*paths, "r");
	जर (!fp) {
		मुक्त(series_dir_buf);
		परत error_errno(_("could not open '%s' for reading"), *paths);
	}

	पर्यंत (!strbuf_getline_lf(&sb, fp)) {
		जर (*sb.buf == '#')
			सुरूठेवा; /* skip comment lines */

		strvec_ढकला(&patches, mkpath("%s/%s", series_dir, sb.buf));
	}

	ख_बंद(fp);
	strbuf_release(&sb);
	मुक्त(series_dir_buf);

	ret = split_mail_conv(stgit_patch_to_mail, state, patches.v, keep_cr);

	strvec_clear(&patches);
	परत ret;
}

/**
 * A split_patches_conv() callback that converts a mercurial patch to a RFC2822
 * message suitable for parsing with git-mailinfo.
 */
स्थिर पूर्णांक hg_patch_to_mail(खाता *out, खाता *in, पूर्णांक keep_cr UNUSED)
{
	संरचना strbuf sb = STRBUF_INIT;
	पूर्णांक rc = 0;

	पर्यंत (!strbuf_getline_lf(&sb, in)) {
		स्थिरांक अक्षर *str;

		जर (skip_prefix(sb.buf, "# User ", &str))
			ख_लिखो(out, "From: %s\n", str);
		अन्यथा जर (skip_prefix(sb.buf, "# Date ", &str)) {
			timestamp_t timestamp;
			दीर्घ tz, tz2;
			अक्षर *end;

			त्रुटि_सं = 0;
			timestamp = parse_timestamp(str, &end, 10);
			जर (त्रुटि_सं) {
				rc = error(_("invalid timestamp"));
				जा निकास;
			}

			जर (!skip_prefix(end, " ", &str)) {
				rc = error(_("invalid Date line"));
				जा निकास;
			}

			त्रुटि_सं = 0;
			tz = म_से_दीर्घ(str, &end, 10);
			जर (त्रुटि_सं) {
				rc = error(_("invalid timezone offset"));
				जा निकास;
			}

			जर (*end) {
				rc = error(_("invalid Date line"));
				जा निकास;
			}

			/*
			 * mercurial's timezone is in seconds west of UTC,
			 * however git's timezone is in hours + minutes east of
			 * UTC. Convert it.
			 */
			tz2 = द_असल(tz) / 3600 * 100 + द_असल(tz) % 3600 / 60;
			जर (tz > 0)
				tz2 = -tz2;

			ख_लिखो(out, "Date: %s\n", show_date(timestamp, tz2, DATE_MODE(RFC2822)));
		} अन्यथा जर (starts_with(sb.buf, "# ")) {
			सुरूठेवा;
		} अन्यथा {
			ख_लिखो(out, "\n%s\n", sb.buf);
			थांबा;
		}
	}

	strbuf_reset(&sb);
	पर्यंत (strbuf_fread(&sb, 8192, in) > 0) {
		ख_डालो(sb.buf, 1, sb.len, out);
		strbuf_reset(&sb);
	}
निकास:
	strbuf_release(&sb);
	परत rc;
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
स्थिर पूर्णांक split_mail(संरचना am_state *state, गणना patch_format patch_format,
			स्थिरांक अक्षर **paths, पूर्णांक keep_cr)
{
	जर (keep_cr < 0) {
		keep_cr = 0;
		repo_config_get_bool(the_repository, "am.keepcr", &keep_cr);
	}

	निवड (patch_format) {
	स्थिती PATCH_FORMAT_MBOX:
		परत split_mail_mbox(state, paths, keep_cr, 0);
	स्थिती PATCH_FORMAT_STGIT:
		परत split_mail_conv(stgit_patch_to_mail, state, paths, keep_cr);
	स्थिती PATCH_FORMAT_STGIT_SERIES:
		परत split_mail_stgit_series(state, paths, keep_cr);
	स्थिती PATCH_FORMAT_HG:
		परत split_mail_conv(hg_patch_to_mail, state, paths, keep_cr);
	स्थिती PATCH_FORMAT_MBOXRD:
		परत split_mail_mbox(state, paths, keep_cr, 1);
	पूर्वनिर्धारित:
		BUG("invalid patch_format");
	}
	परत -1;
}

/**
 * Setup a new am session for applying patches
 */
स्थिर शून्य am_setup(संरचना am_state *state, गणना patch_format patch_format,
			स्थिरांक अक्षर **paths, पूर्णांक keep_cr)
{
	संरचना object_id curr_head;
	स्थिरांक अक्षर *str;
	संरचना strbuf sb = STRBUF_INIT;

	जर (!patch_format)
		patch_format = detect_patch_format(paths);

	जर (!patch_format) {
		fprintf_ln(मानक_त्रुटि, _("Patch format detection failed."));
		die(रिक्त);
	}

	जर (सूची_गढ़ो(state->dir, 0777) < 0 && त्रुटि_सं != EEXIST)
		die_errno(_("failed to create directory '%s'"), state->dir);
	refs_delete_ref(get_main_ref_store(the_repository), रिक्त,
			"REBASE_HEAD", रिक्त, REF_NO_DEREF);

	जर (split_mail(state, patch_format, paths, keep_cr) < 0) {
		am_destroy(state);
		die(_("Failed to split patches."));
	}

	जर (state->rebasing)
		state->threeway = 1;

	write_state_bool(state, "threeway", state->threeway);
	write_state_bool(state, "quiet", state->quiet);
	write_state_bool(state, "sign", state->signoff);
	write_state_bool(state, "utf8", state->utf8);

	जर (state->allow_rerere_autoupdate)
		write_state_bool(state, "rerere-autoupdate",
			 state->allow_rerere_autoupdate == RERERE_AUTOUPDATE);

	निवड (state->keep) {
	स्थिती KEEP_FALSE:
		str = "f";
		थांबा;
	स्थिती KEEP_TRUE:
		str = "t";
		थांबा;
	स्थिती KEEP_NON_PATCH:
		str = "b";
		थांबा;
	पूर्वनिर्धारित:
		BUG("invalid value for state->keep");
	}

	write_state_text(state, "keep", str);
	write_state_bool(state, "messageid", state->message_id);

	निवड (state->scissors) {
	स्थिती SCISSORS_UNSET:
		str = "";
		थांबा;
	स्थिती SCISSORS_FALSE:
		str = "f";
		थांबा;
	स्थिती SCISSORS_TRUE:
		str = "t";
		थांबा;
	पूर्वनिर्धारित:
		BUG("invalid value for state->scissors");
	}
	write_state_text(state, "scissors", str);

	निवड (state->quoted_cr) {
	स्थिती quoted_cr_unset:
		str = "";
		थांबा;
	स्थिती quoted_cr_nowarn:
		str = "nowarn";
		थांबा;
	स्थिती quoted_cr_warn:
		str = "warn";
		थांबा;
	स्थिती quoted_cr_strip:
		str = "strip";
		थांबा;
	पूर्वनिर्धारित:
		BUG("invalid value for state->quoted_cr");
	}
	write_state_text(state, "quoted-cr", str);

	sq_quote_argv(&sb, state->git_apply_opts.v);
	write_state_text(state, "apply-opt", sb.buf);

	जर (state->rebasing)
		write_state_text(state, "rebasing", "");
	अन्यथा
		write_state_text(state, "applying", "");

	जर (!repo_get_oid(the_repository, "HEAD", &curr_head)) {
		write_state_text(state, "abort-safety", oid_to_hex(&curr_head));
		जर (!state->rebasing)
			refs_update_ref(get_main_ref_store(the_repository),
					"am", "ORIG_HEAD", &curr_head, रिक्त,
					0,
					UPDATE_REFS_DIE_ON_ERR);
	} अन्यथा {
		write_state_text(state, "abort-safety", "");
		जर (!state->rebasing)
			refs_delete_ref(get_main_ref_store(the_repository),
					रिक्त, "ORIG_HEAD", रिक्त, 0);
	}

	/*
	 * NOTE: Since the "next" and "last" files determine if an am_state
	 * session is in progress, they should be written last.
	 */

	write_state_मोजणी(state, "next", state->cur);
	write_state_मोजणी(state, "last", state->last);

	strbuf_release(&sb);
}

/**
 * Increments the patch pointer, and cleans am_state for the application of the
 * next patch.
 */
स्थिर शून्य am_next(संरचना am_state *state)
{
	संरचना object_id head;

	FREE_AND_NULL(state->author_name);
	FREE_AND_NULL(state->author_email);
	FREE_AND_NULL(state->author_date);
	FREE_AND_NULL(state->संदेश);
	state->संदेश_len = 0;

	unlink(am_path(state, "author-script"));
	unlink(am_path(state, "final-commit"));

	oidclr(&state->orig_commit, the_repository->hash_algo);
	unlink(am_path(state, "original-commit"));
	refs_delete_ref(get_main_ref_store(the_repository), रिक्त,
			"REBASE_HEAD", रिक्त, REF_NO_DEREF);

	जर (!repo_get_oid(the_repository, "HEAD", &head))
		write_state_text(state, "abort-safety", oid_to_hex(&head));
	अन्यथा
		write_state_text(state, "abort-safety", "");

	state->cur++;
	write_state_मोजणी(state, "next", state->cur);
}

/**
 * Returns the filename of the current patch email.
 */
स्थिर स्थिरांक अक्षर *msgnum(स्थिरांक संरचना am_state *state)
{
	स्थिर संरचना strbuf sb = STRBUF_INIT;

	strbuf_reset(&sb);
	strbuf_addf(&sb, "%0*d", state->prec, state->cur);

	परत sb.buf;
}

/**
 * Dies with a user-friendly message on how to proceed after resolving the
 * problem. This message can be overridden with state->resolvemsg.
 */
स्थिर शून्य NORETURN die_user_resolve(स्थिरांक संरचना am_state *state)
{
	जर (state->resolvemsg) {
		advise_if_enabled(ADVICE_MERGE_CONFLICT, "%s", state->resolvemsg);
	} अन्यथा {
		स्थिरांक अक्षर *cmdline = state->interactive ? "git am -i" : "git am";
		संरचना strbuf sb = STRBUF_INIT;

		strbuf_addf(&sb, _("When you have resolved this problem, run \"%s --continue\".\n"), cmdline);
		strbuf_addf(&sb, _("If you prefer to skip this patch, run \"%s --skip\" instead.\n"), cmdline);

		जर (advice_enabled(ADVICE_AM_WORK_DIR) &&
		    is_empty_or_missing_file(am_path(state, "patch")) &&
		    !repo_index_has_changes(the_repository, रिक्त, रिक्त))
			strbuf_addf(&sb, _("To record the empty patch as an empty commit, run \"%s --allow-empty\".\n"), cmdline);

		strbuf_addf(&sb, _("To restore the original branch and stop patching, run \"%s --abort\"."), cmdline);

		advise_if_enabled(ADVICE_MERGE_CONFLICT, "%s", sb.buf);
		strbuf_release(&sb);
	}

	die(रिक्त);
}

/**
 * Appends signoff to the "msg" field of the am_state.
 */
स्थिर शून्य am_append_signoff(संरचना am_state *state)
{
	संरचना strbuf sb = STRBUF_INIT;

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
स्थिर पूर्णांक parse_mail(संरचना am_state *state, स्थिरांक अक्षर *mail)
{
	खाता *fp;
	संरचना strbuf sb = STRBUF_INIT;
	संरचना strbuf संदेश = STRBUF_INIT;
	संरचना strbuf author_name = STRBUF_INIT;
	संरचना strbuf author_date = STRBUF_INIT;
	संरचना strbuf author_email = STRBUF_INIT;
	पूर्णांक ret = 0;
	संरचना mailinfo mi;

	setup_mailinfo(the_repository, &mi);

	जर (state->utf8)
		mi.metainfo_charset = get_commit_output_encoding();
	अन्यथा
		mi.metainfo_charset = रिक्त;

	निवड (state->keep) {
	स्थिती KEEP_FALSE:
		थांबा;
	स्थिती KEEP_TRUE:
		mi.keep_subject = 1;
		थांबा;
	स्थिती KEEP_NON_PATCH:
		mi.keep_non_patch_brackets_in_subject = 1;
		थांबा;
	पूर्वनिर्धारित:
		BUG("invalid value for state->keep");
	}

	जर (state->message_id)
		mi.add_message_id = 1;

	निवड (state->scissors) {
	स्थिती SCISSORS_UNSET:
		थांबा;
	स्थिती SCISSORS_FALSE:
		mi.use_scissors = 0;
		थांबा;
	स्थिती SCISSORS_TRUE:
		mi.use_scissors = 1;
		थांबा;
	पूर्वनिर्धारित:
		BUG("invalid value for state->scissors");
	}

	निवड (state->quoted_cr) {
	स्थिती quoted_cr_unset:
		थांबा;
	स्थिती quoted_cr_nowarn:
	स्थिती quoted_cr_warn:
	स्थिती quoted_cr_strip:
		mi.quoted_cr = state->quoted_cr;
		थांबा;
	पूर्वनिर्धारित:
		BUG("invalid value for state->quoted_cr");
	}

	mi.आदान = xfopen(mail, "r");
	mi.output = xfopen(am_path(state, "info"), "w");
	जर (mailinfo(&mi, am_path(state, "msg"), am_path(state, "patch")))
		die("could not parse patch");

	ख_बंद(mi.आदान);
	ख_बंद(mi.output);

	जर (mi.format_flowed)
		warning(_("Patch sent with format=flowed; "
			  "space at the end of lines might be lost."));

	/* Extract message and author information */
	fp = xfopen(am_path(state, "info"), "r");
	पर्यंत (!strbuf_getline_lf(&sb, fp)) {
		स्थिरांक अक्षर *x;

		जर (skip_prefix(sb.buf, "Subject: ", &x)) {
			जर (संदेश.len)
				strbuf_addch(&संदेश, '\n');
			strbuf_addstr(&संदेश, x);
		} अन्यथा जर (skip_prefix(sb.buf, "Author: ", &x))
			strbuf_addstr(&author_name, x);
		अन्यथा जर (skip_prefix(sb.buf, "Email: ", &x))
			strbuf_addstr(&author_email, x);
		अन्यथा जर (skip_prefix(sb.buf, "Date: ", &x))
			strbuf_addstr(&author_date, x);
	}
	ख_बंद(fp);

	/* Skip pine's internal folder data */
	जर (!म_भेद(author_name.buf, "Mail System Internal Data")) {
		ret = 1;
		जा finish;
	}

	strbuf_addstr(&संदेश, "\n\n");
	strbuf_addbuf(&संदेश, &mi.log_message);
	strbuf_stripspace(&संदेश, रिक्त);

	प्रतिज्ञा(!state->author_name);
	state->author_name = strbuf_detach(&author_name, रिक्त);

	प्रतिज्ञा(!state->author_email);
	state->author_email = strbuf_detach(&author_email, रिक्त);

	प्रतिज्ञा(!state->author_date);
	state->author_date = strbuf_detach(&author_date, रिक्त);

	प्रतिज्ञा(!state->संदेश);
	state->संदेश = strbuf_detach(&संदेश, &state->संदेश_len);

finish:
	strbuf_release(&संदेश);
	strbuf_release(&author_date);
	strbuf_release(&author_email);
	strbuf_release(&author_name);
	strbuf_release(&sb);
	clear_mailinfo(&mi);
	परत ret;
}

/**
 * Sets commit_id to the commit hash where the mail was generated from.
 * Returns 0 on success, -1 on failure.
 */
स्थिर पूर्णांक get_mail_commit_oid(संरचना object_id *commit_id, स्थिरांक अक्षर *mail)
{
	संरचना strbuf sb = STRBUF_INIT;
	खाता *fp = xfopen(mail, "r");
	स्थिरांक अक्षर *x;
	पूर्णांक ret = 0;

	जर (strbuf_getline_lf(&sb, fp) ||
	    !skip_prefix(sb.buf, "From ", &x) ||
	    get_oid_hex(x, commit_id) < 0)
		ret = -1;

	strbuf_release(&sb);
	ख_बंद(fp);
	परत ret;
}

/**
 * Sets state->msg, state->author_name, state->author_email, state->author_date
 * to the commit's respective info.
 */
स्थिर शून्य get_commit_info(संरचना am_state *state, संरचना commit *commit)
{
	स्थिरांक अक्षर *buffer, *ident_ओळ, *संदेश;
	माप_प्रकार ident_len;
	संरचना ident_split id;

	buffer = repo_logmsg_reencode(the_repository, commit, रिक्त,
				      get_commit_output_encoding());

	ident_ओळ = find_commit_header(buffer, "author", &ident_len);
	जर (!ident_ओळ)
		die(_("missing author line in commit %s"),
		      oid_to_hex(&commit->object.oid));
	जर (split_ident_ओळ(&id, ident_ओळ, ident_len) < 0)
		die(_("invalid ident line: %.*s"), (पूर्णांक)ident_len, ident_ओळ);

	प्रतिज्ञा(!state->author_name);
	जर (id.name_begin)
		state->author_name =
			xmemdupz(id.name_begin, id.name_end - id.name_begin);
	अन्यथा
		state->author_name = xstrdup("");

	प्रतिज्ञा(!state->author_email);
	जर (id.mail_begin)
		state->author_email =
			xmemdupz(id.mail_begin, id.mail_end - id.mail_begin);
	अन्यथा
		state->author_email = xstrdup("");

	प्रतिज्ञा(!state->author_date);
	state->author_date = xstrdup(show_ident_date(&id, DATE_MODE(NORMAL)));

	प्रतिज्ञा(!state->संदेश);
	संदेश = म_माला(buffer, "\n\n");
	जर (!संदेश)
		die(_("unable to parse commit %s"), oid_to_hex(&commit->object.oid));
	state->संदेश = xstrdup(संदेश + 2);
	state->संदेश_len = म_माप(state->संदेश);
	repo_unuse_commit_buffer(the_repository, commit, buffer);
}

/**
 * Writes `commit` as a patch to the state directory's "patch" file.
 */
स्थिर शून्य write_commit_patch(स्थिरांक संरचना am_state *state, संरचना commit *commit)
{
	संरचना rev_info rev_info;
	खाता *fp;

	fp = xfopen(am_path(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, रिक्त);
	rev_info.diff = 1;
	rev_info.abbrev = 0;
	rev_info.disable_stdin = 1;
	rev_info.show_मूळ_diff = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.flags.द्विमान = 1;
	rev_info.diffopt.flags.full_index = 1;
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
स्थिर शून्य write_index_patch(स्थिरांक संरचना am_state *state)
{
	संरचना वृक्ष *वृक्ष;
	संरचना object_id head;
	संरचना rev_info rev_info;
	खाता *fp;

	जर (!repo_get_oid(the_repository, "HEAD", &head)) {
		संरचना commit *commit = lookup_commit_or_die(&head, "HEAD");
		वृक्ष = repo_get_commit_वृक्ष(the_repository, commit);
	} अन्यथा
		वृक्ष = lookup_वृक्ष(the_repository,
				   the_repository->hash_algo->empty_वृक्ष);

	fp = xfopen(am_path(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, रिक्त);
	rev_info.diff = 1;
	rev_info.disable_stdin = 1;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	add_pending_object(&rev_info, &वृक्ष->object, "");
	diff_setup_done(&rev_info.diffopt);
	run_diff_index(&rev_info, DIFF_INDEX_CACHED);
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
स्थिर पूर्णांक parse_mail_rebase(संरचना am_state *state, स्थिरांक अक्षर *mail)
{
	संरचना commit *commit;
	संरचना object_id commit_oid;

	जर (get_mail_commit_oid(&commit_oid, mail) < 0)
		die(_("could not parse %s"), mail);

	commit = lookup_commit_or_die(&commit_oid, mail);

	get_commit_info(state, commit);

	write_commit_patch(state, commit);

	oidcpy(&state->orig_commit, &commit_oid);
	write_state_text(state, "original-commit", oid_to_hex(&commit_oid));
	refs_update_ref(get_main_ref_store(the_repository), "am",
			"REBASE_HEAD", &commit_oid,
			रिक्त, REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);

	परत 0;
}

/**
 * Applies current patch with git-apply. Returns 0 on success, -1 otherwise. If
 * `index_file` is not NULL, the patch will be applied to that index.
 */
स्थिर पूर्णांक run_apply(स्थिरांक संरचना am_state *state, स्थिरांक अक्षर *index_file)
{
	संरचना strvec apply_paths = STRVEC_INIT;
	संरचना strvec apply_opts = STRVEC_INIT;
	संरचना apply_state apply_state;
	पूर्णांक res, opts_डावा;
	पूर्णांक force_apply = 0;
	पूर्णांक options = 0;
	स्थिरांक अक्षर **apply_argv;

	जर (init_apply_state(&apply_state, the_repository, रिक्त))
		BUG("init_apply_state() failed");

	strvec_ढकला(&apply_opts, "apply");
	strvec_pushv(&apply_opts, state->git_apply_opts.v);

	/*
	 * Build a copy that apply_parse_options() can rearrange.
	 * apply_opts.v keeps referencing the allocated strings for
	 * strvec_clear() to release.
	 */
	DUP_ARRAY(apply_argv, apply_opts.v, apply_opts.nr);

	opts_डावा = apply_parse_options(apply_opts.nr, apply_argv,
					&apply_state, &force_apply, &options,
					रिक्त);

	जर (opts_डावा != 0)
		die("unknown option passed through to git apply");

	जर (index_file) {
		apply_state.index_file = index_file;
		apply_state.cached = 1;
	} अन्यथा
		apply_state.check_index = 1;

	/*
	 * If we are allowed to fall back on 3-way merge, don't give false
	 * errors during the initial attempt.
	 */
	जर (state->threeway && !index_file)
		apply_state.apply_verbosity = verbosity_silent;

	जर (check_apply_state(&apply_state, force_apply))
		BUG("check_apply_state() failed");

	strvec_ढकला(&apply_paths, am_path(state, "patch"));

	res = apply_all_patches(&apply_state, apply_paths.nr, apply_paths.v, options);

	strvec_clear(&apply_paths);
	strvec_clear(&apply_opts);
	clear_apply_state(&apply_state);
	मुक्त(apply_argv);

	जर (res)
		परत res;

	जर (index_file) {
		/* Reload index as apply_all_patches() will have modified it. */
		discard_index(the_repository->index);
		read_index_from(the_repository->index, index_file,
				repo_get_git_dir(the_repository));
	}

	परत 0;
}

/**
 * Builds an index that contains just the blobs needed for a 3way merge.
 */
स्थिर पूर्णांक build_fake_ancestor(स्थिरांक संरचना am_state *state, स्थिरांक अक्षर *index_file)
{
	संरचना child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	strvec_ढकला(&cp.args, "apply");
	strvec_pushv(&cp.args, state->git_apply_opts.v);
	strvec_pushf(&cp.args, "--build-fake-ancestor=%s", index_file);
	strvec_ढकला(&cp.args, am_path(state, "patch"));

	जर (run_command(&cp))
		परत -1;

	परत 0;
}

/**
 * Attempt a threeway merge, using index_path as the temporary index.
 */
स्थिर पूर्णांक fall_back_threeway(स्थिरांक संरचना am_state *state, स्थिरांक अक्षर *index_path)
{
	संरचना object_id their_वृक्ष, our_वृक्ष;
	संरचना object_id bases[1] = { 0 };
	संरचना merge_options o;
	संरचना commit *result;
	अक्षर *their_वृक्ष_name;

	जर (repo_get_oid(the_repository, "HEAD", &our_वृक्ष) < 0)
		oidcpy(&our_वृक्ष, the_hash_algo->empty_वृक्ष);

	जर (build_fake_ancestor(state, index_path))
		परत error("could not build fake ancestor");

	discard_index(the_repository->index);
	read_index_from(the_repository->index, index_path, repo_get_git_dir(the_repository));

	जर (write_index_as_वृक्ष(&bases[0], the_repository->index, index_path, 0, रिक्त))
		परत error(_("Repository lacks necessary blobs to fall back on 3-way merge."));

	say(state, मानक_निकास, _("Using index info to reconstruct a base tree..."));

	जर (!state->quiet) {
		/*
		 * List paths that needed 3-way fallback, so that the user can
		 * review them with extra care to spot mismerges.
		 */
		संरचना rev_info rev_info;

		repo_init_revisions(the_repository, &rev_info, रिक्त);
		rev_info.diffopt.output_format = DIFF_FORMAT_NAME_STATUS;
		rev_info.diffopt.filter |= diff_filter_bit('A');
		rev_info.diffopt.filter |= diff_filter_bit('M');
		add_pending_oid(&rev_info, "HEAD", &our_वृक्ष, 0);
		diff_setup_done(&rev_info.diffopt);
		run_diff_index(&rev_info, DIFF_INDEX_CACHED);
		release_revisions(&rev_info);
	}

	जर (run_apply(state, index_path))
		परत error(_("Did you hand edit your patch?\n"
				"It does not apply to blobs recorded in its index."));

	जर (write_index_as_वृक्ष(&their_वृक्ष, the_repository->index, index_path, 0, रिक्त))
		परत error("could not write tree");

	say(state, मानक_निकास, _("Falling back to patching base and 3-way merge..."));

	discard_index(the_repository->index);
	repo_read_index(the_repository);

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

	जर (state->quiet)
		o.verbosity = 0;

	जर (merge_ort_generic(&o, &our_वृक्ष, &their_वृक्ष, 1, bases, &result)) {
		repo_rerere(the_repository, state->allow_rerere_autoupdate);
		मुक्त(their_वृक्ष_name);
		परत error(_("Failed to merge in the changes."));
	}

	मुक्त(their_वृक्ष_name);
	परत 0;
}

/**
 * Commits the current index with state->msg as the commit message and
 * state->author_name, state->author_email and state->author_date as the author
 * information.
 */
स्थिर शून्य do_commit(स्थिरांक संरचना am_state *state)
{
	संरचना object_id वृक्ष, parent, commit;
	स्थिरांक संरचना object_id *old_oid;
	संरचना commit_list *parents = रिक्त;
	स्थिरांक अक्षर *reflog_संदेश, *author, *committer = रिक्त;
	संरचना strbuf sb = STRBUF_INIT;

	जर (!state->no_verify && run_hooks(the_repository, "pre-applypatch"))
		निकास(1);

	जर (write_index_as_वृक्ष(&वृक्ष, the_repository->index,
				repo_get_index_file(the_repository),
				0, रिक्त))
		die(_("git write-tree failed to write a tree"));

	जर (!repo_get_oid_commit(the_repository, "HEAD", &parent)) {
		old_oid = &parent;
		commit_list_घाला(lookup_commit(the_repository, &parent),
				   &parents);
	} अन्यथा {
		old_oid = रिक्त;
		say(state, मानक_त्रुटि, _("applying to an empty history"));
	}

	author = fmt_ident(state->author_name, state->author_email,
		WANT_AUTHOR_IDENT,
			state->ignore_date ? रिक्त : state->author_date,
			IDENT_STRICT);

	जर (state->committer_date_is_author_date)
		committer = fmt_ident(दो_पर्या("GIT_COMMITTER_NAME"),
				      दो_पर्या("GIT_COMMITTER_EMAIL"),
				      WANT_COMMITTER_IDENT,
				      state->ignore_date ? रिक्त
							 : state->author_date,
				      IDENT_STRICT);

	जर (commit_वृक्ष_extended(state->संदेश, state->संदेश_len, &वृक्ष, parents,
				 &commit, author, committer, state->sign_commit,
				 रिक्त))
		die(_("failed to write commit object"));

	reflog_संदेश = दो_पर्या("GIT_REFLOG_ACTION");
	जर (!reflog_संदेश)
		reflog_संदेश = "am";

	strbuf_addf(&sb, "%s: %.*s", reflog_संदेश, linelen(state->संदेश),
			state->संदेश);

	refs_update_ref(get_main_ref_store(the_repository), sb.buf, "HEAD",
			&commit, old_oid, 0,
			UPDATE_REFS_DIE_ON_ERR);

	जर (state->rebasing) {
		खाता *fp = xfopen(am_path(state, "rewritten"), "a");

		प्रतिज्ञा(!is_null_oid(&state->orig_commit));
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
स्थिर शून्य validate_resume_state(स्थिरांक संरचना am_state *state)
{
	जर (!state->संदेश)
		die(_("cannot resume: %s does not exist."),
			am_path(state, "final-commit"));

	जर (!state->author_name || !state->author_email || !state->author_date)
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
स्थिर पूर्णांक do_interactive(संरचना am_state *state)
{
	प्रतिज्ञा(state->संदेश);

	साठी (;;) {
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
		जर (!ख_माला_लो(reply, आकार(reply), मानक_निवेश))
			die("unable to read from stdin; aborting");

		जर (*reply == 'y' || *reply == 'Y') {
			परत 0;
		} अन्यथा जर (*reply == 'a' || *reply == 'A') {
			state->interactive = 0;
			परत 0;
		} अन्यथा जर (*reply == 'n' || *reply == 'N') {
			परत 1;
		} अन्यथा जर (*reply == 'e' || *reply == 'E') {
			संरचना strbuf संदेश = STRBUF_INIT;

			जर (!launch_editor(am_path(state, "final-commit"), &संदेश, रिक्त)) {
				मुक्त(state->संदेश);
				state->संदेश = strbuf_detach(&संदेश, &state->संदेश_len);
			}
			strbuf_release(&संदेश);
		} अन्यथा जर (*reply == 'v' || *reply == 'V') {
			स्थिरांक अक्षर *pager = git_pager(the_repository, 1);
			संरचना child_process cp = CHILD_PROCESS_INIT;

			जर (!pager)
				pager = "cat";
			prepare_pager_args(&cp, pager);
			strvec_ढकला(&cp.args, am_path(state, "patch"));
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
स्थिर शून्य am_run(संरचना am_state *state, पूर्णांक resume)
{
	संरचना strbuf sb = STRBUF_INIT;

	unlink(am_path(state, "dirtyindex"));

	जर (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 रिक्त, रिक्त, रिक्त) < 0)
		die(_("unable to write index file"));

	जर (repo_index_has_changes(the_repository, रिक्त, &sb)) {
		write_state_bool(state, "dirtyindex", 1);
		die(_("Dirty index: cannot apply patches (dirty: %s)"), sb.buf);
	}

	strbuf_release(&sb);

	पर्यंत (state->cur <= state->last) {
		स्थिरांक अक्षर *mail = am_path(state, msgnum(state));
		पूर्णांक apply_status;
		पूर्णांक to_keep;

		reset_ident_date();

		जर (!file_exists(mail))
			जा next;

		जर (resume) {
			validate_resume_state(state);
		} अन्यथा {
			पूर्णांक skip;

			जर (state->rebasing)
				skip = parse_mail_rebase(state, mail);
			अन्यथा
				skip = parse_mail(state, mail);

			जर (skip)
				जा next; /* mail should be skipped */

			जर (state->signoff)
				am_append_signoff(state);

			write_author_script(state);
			write_commit_संदेश(state);
		}

		जर (state->interactive && do_interactive(state))
			जा next;

		to_keep = 0;
		जर (is_empty_or_missing_file(am_path(state, "patch"))) {
			निवड (state->empty_type) {
			स्थिती DROP_EMPTY_COMMIT:
				say(state, मानक_निकास, _("Skipping: %.*s"), linelen(state->संदेश), state->संदेश);
				जा next;
				थांबा;
			स्थिती KEEP_EMPTY_COMMIT:
				to_keep = 1;
				say(state, मानक_निकास, _("Creating an empty commit: %.*s"),
					linelen(state->संदेश), state->संदेश);
				थांबा;
			स्थिती STOP_ON_EMPTY_COMMIT:
				printf_ln(_("Patch is empty."));
				die_user_resolve(state);
				थांबा;
			}
		}

		जर (run_applypatch_संदेश_hook(state))
			निकास(1);
		जर (to_keep)
			जा commit;

		say(state, मानक_निकास, _("Applying: %.*s"), linelen(state->संदेश), state->संदेश);

		apply_status = run_apply(state, रिक्त);

		जर (apply_status && state->threeway) {
			संरचना strbuf sb = STRBUF_INIT;

			strbuf_addstr(&sb, am_path(state, "patch-merge-index"));
			apply_status = fall_back_threeway(state, sb.buf);
			strbuf_release(&sb);

			/*
			 * Applying the patch to an earlier tree and merging
			 * the result may have produced the same tree as ours.
			 */
			जर (!apply_status &&
			    !repo_index_has_changes(the_repository, रिक्त, रिक्त)) {
				say(state, मानक_निकास, _("No changes -- Patch already applied."));
				जा next;
			}
		}

		जर (apply_status) {
			printf_ln(_("Patch failed at %s %.*s"), msgnum(state),
				linelen(state->संदेश), state->संदेश);

			जर (advice_enabled(ADVICE_AM_WORK_DIR))
				advise(_("Use 'git am --show-current-patch=diff' to see the failed patch"));

			die_user_resolve(state);
		}

commit:
		do_commit(state);

next:
		am_next(state);

		जर (resume)
			am_load(state);
		resume = 0;
	}

	जर (!is_empty_or_missing_file(am_path(state, "rewritten"))) {
		प्रतिज्ञा(state->rebasing);
		copy_notes_for_rebase(state);
		run_post_rewrite_hook(state);
	}

	/*
	 * In rebasing mode, it's up to the caller to take care of
	 * housekeeping.
	 */
	जर (!state->rebasing) {
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
स्थिर शून्य am_resolve(संरचना am_state *state, पूर्णांक allow_empty)
{
	validate_resume_state(state);

	say(state, मानक_निकास, _("Applying: %.*s"), linelen(state->संदेश), state->संदेश);

	जर (!repo_index_has_changes(the_repository, रिक्त, रिक्त)) {
		जर (allow_empty && is_empty_or_missing_file(am_path(state, "patch"))) {
			printf_ln(_("No changes - recorded it as an empty commit."));
		} अन्यथा {
			printf_ln(_("No changes - did you forget to use 'git add'?\n"
				    "If there is nothing left to stage, chances are that something else\n"
				    "already introduced the same changes; you might want to skip this patch."));
			die_user_resolve(state);
		}
	}

	जर (unmerged_index(the_repository->index)) {
		printf_ln(_("You still have unmerged paths in your index.\n"
			"You should 'git add' each file with resolved conflicts to mark them as such.\n"
			"You might run `git rm` on a file to accept \"deleted by them\" for it."));
		die_user_resolve(state);
	}

	जर (state->interactive) {
		write_index_patch(state);
		जर (do_interactive(state))
			जा next;
	}

	repo_rerere(the_repository, 0);

	do_commit(state);

next:
	am_next(state);
	am_load(state);
	am_run(state, 0);
}

/**
 * Performs a checkout fast-forward from `head` to `remote`. If `reset` is
 * true, any unmerged entries will be discarded. Returns 0 on success, -1 on
 * failure.
 */
स्थिर पूर्णांक fast_forward_to(संरचना वृक्ष *head, संरचना वृक्ष *remote, पूर्णांक reset)
{
	संरचना lock_file lock_file = LOCK_INIT;
	संरचना unpack_trees_options opts;
	संरचना वृक्ष_desc t[2];

	जर (repo_parse_वृक्ष(the_repository, head) || repo_parse_वृक्ष(the_repository, remote))
		परत -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	refresh_index(the_repository->index, REFRESH_QUIET, रिक्त, रिक्त, रिक्त);

	स_रखो(&opts, 0, आकार(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.update = 1;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = twoway_merge;
	init_वृक्ष_desc(&t[0], &head->object.oid, head->buffer, head->size);
	init_वृक्ष_desc(&t[1], &remote->object.oid, remote->buffer, remote->size);

	जर (unpack_trees(2, t, &opts)) {
		rollback_lock_file(&lock_file);
		परत -1;
	}

	जर (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	परत 0;
}

/**
 * Merges a tree into the index. The index's stat info will take precedence
 * over the merged tree's. Returns 0 on success, -1 on failure.
 */
स्थिर पूर्णांक merge_वृक्ष(संरचना वृक्ष *वृक्ष)
{
	संरचना lock_file lock_file = LOCK_INIT;
	संरचना unpack_trees_options opts;
	संरचना वृक्ष_desc t[1];

	जर (repo_parse_वृक्ष(the_repository, वृक्ष))
		परत -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	स_रखो(&opts, 0, आकार(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.merge = 1;
	opts.fn = oneway_merge;
	init_वृक्ष_desc(&t[0], &वृक्ष->object.oid, वृक्ष->buffer, वृक्ष->size);

	जर (unpack_trees(1, t, &opts)) {
		rollback_lock_file(&lock_file);
		परत -1;
	}

	जर (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	परत 0;
}

/**
 * Clean the index without touching entries that are not modified between
 * `head` and `remote`.
 */
स्थिर पूर्णांक clean_index(स्थिरांक संरचना object_id *head, स्थिरांक संरचना object_id *remote)
{
	संरचना वृक्ष *head_वृक्ष, *remote_वृक्ष, *index_वृक्ष;
	संरचना object_id index;

	head_वृक्ष = repo_parse_वृक्ष_indirect(the_repository, head);
	जर (!head_वृक्ष)
		परत error(_("Could not parse object '%s'."), oid_to_hex(head));

	remote_वृक्ष = repo_parse_वृक्ष_indirect(the_repository, remote);
	जर (!remote_वृक्ष)
		परत error(_("Could not parse object '%s'."), oid_to_hex(remote));

	repo_read_index_unmerged(the_repository);

	जर (fast_forward_to(head_वृक्ष, head_वृक्ष, 1))
		परत -1;

	जर (write_index_as_वृक्ष(&index, the_repository->index,
				repo_get_index_file(the_repository),
				0, रिक्त))
		परत -1;

	index_वृक्ष = repo_parse_वृक्ष_indirect(the_repository, &index);
	जर (!index_वृक्ष)
		परत error(_("Could not parse object '%s'."), oid_to_hex(&index));

	जर (fast_forward_to(index_वृक्ष, remote_वृक्ष, 0))
		परत -1;

	जर (merge_वृक्ष(remote_वृक्ष))
		परत -1;

	remove_branch_state(the_repository, 0);

	परत 0;
}

/**
 * Resets rerere's merge resolution metadata.
 */
स्थिर शून्य am_rerere_clear(शून्य)
{
	संरचना string_list merge_rr = STRING_LIST_INIT_DUP;
	rerere_clear(the_repository, &merge_rr);
	string_list_clear(&merge_rr, 1);
}

/**
 * Resume the current am session by skipping the current patch.
 */
स्थिर शून्य am_skip(संरचना am_state *state)
{
	संरचना object_id head;

	am_rerere_clear();

	जर (repo_get_oid(the_repository, "HEAD", &head))
		oidcpy(&head, the_hash_algo->empty_वृक्ष);

	जर (clean_index(&head, &head))
		die(_("failed to clean index"));

	जर (state->rebasing) {
		खाता *fp = xfopen(am_path(state, "rewritten"), "a");

		प्रतिज्ञा(!is_null_oid(&state->orig_commit));
		ख_लिखो(fp, "%s ", oid_to_hex(&state->orig_commit));
		ख_लिखो(fp, "%s\n", oid_to_hex(&head));
		ख_बंद(fp);
	}

	am_next(state);
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
स्थिर पूर्णांक safe_to_abort(स्थिरांक संरचना am_state *state)
{
	संरचना strbuf sb = STRBUF_INIT;
	संरचना object_id abort_safety, head;

	जर (file_exists(am_path(state, "dirtyindex")))
		परत 0;

	जर (read_state_file(&sb, state, "abort-safety", 1) > 0) {
		जर (get_oid_hex(sb.buf, &abort_safety))
			die(_("could not parse %s"), am_path(state, "abort-safety"));
	} अन्यथा
		oidclr(&abort_safety, the_repository->hash_algo);
	strbuf_release(&sb);

	जर (repo_get_oid(the_repository, "HEAD", &head))
		oidclr(&head, the_repository->hash_algo);

	जर (oideq(&head, &abort_safety))
		परत 1;

	warning(_("You seem to have moved HEAD since the last 'am' failure.\n"
		"Not rewinding to ORIG_HEAD"));

	परत 0;
}

/**
 * Aborts the current am session if it is safe to do so.
 */
स्थिर शून्य am_abort(संरचना am_state *state)
{
	संरचना object_id curr_head, orig_head;
	पूर्णांक has_curr_head, has_orig_head;
	अक्षर *curr_branch;

	जर (!safe_to_abort(state)) {
		am_destroy(state);
		परत;
	}

	am_rerere_clear();

	curr_branch = refs_resolve_refdup(get_main_ref_store(the_repository),
					  "HEAD", 0, &curr_head, रिक्त);
	has_curr_head = curr_branch && !is_null_oid(&curr_head);
	जर (!has_curr_head)
		oidcpy(&curr_head, the_hash_algo->empty_वृक्ष);

	has_orig_head = !repo_get_oid(the_repository, "ORIG_HEAD", &orig_head);
	जर (!has_orig_head)
		oidcpy(&orig_head, the_hash_algo->empty_वृक्ष);

	जर (clean_index(&curr_head, &orig_head))
		die(_("failed to clean index"));

	जर (has_orig_head)
		refs_update_ref(get_main_ref_store(the_repository),
				"am --abort", "HEAD", &orig_head,
				has_curr_head ? &curr_head : रिक्त, 0,
				UPDATE_REFS_DIE_ON_ERR);
	अन्यथा जर (curr_branch)
		refs_delete_ref(get_main_ref_store(the_repository), रिक्त,
				curr_branch, रिक्त, REF_NO_DEREF);

	मुक्त(curr_branch);
	am_destroy(state);
}

स्थिर पूर्णांक show_patch(संरचना am_state *state, गणना resume_type resume_mode)
{
	संरचना strbuf sb = STRBUF_INIT;
	स्थिरांक अक्षर *patch_path;
	पूर्णांक len;

	जर (!is_null_oid(&state->orig_commit)) {
		संरचना child_process cmd = CHILD_PROCESS_INIT;

		strvec_pushl(&cmd.args, "show", oid_to_hex(&state->orig_commit),
			     "--", रिक्त);
		cmd.git_cmd = 1;
		परत run_command(&cmd);
	}

	निवड (resume_mode) {
	स्थिती RESUME_SHOW_PATCH_RAW:
		patch_path = am_path(state, msgnum(state));
		थांबा;
	स्थिती RESUME_SHOW_PATCH_DIFF:
		patch_path = am_path(state, "patch");
		थांबा;
	पूर्वनिर्धारित:
		BUG("invalid mode for --show-current-patch");
	}

	len = strbuf_read_file(&sb, patch_path, 0);
	जर (len < 0)
		die_errno(_("failed to read '%s'"), patch_path);

	setup_pager(the_repository);
	write_in_full(1, sb.buf, sb.len);
	strbuf_release(&sb);
	परत 0;
}

/**
 * parse_options() callback that validates and sets opt->value to the
 * PATCH_FORMAT_* enum value corresponding to `arg`.
 */
स्थिर पूर्णांक parse_opt_patchformat(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	पूर्णांक *opt_मूल्य = opt->मूल्य;

	जर (unset)
		*opt_मूल्य = PATCH_FORMAT_UNKNOWN;
	अन्यथा जर (!म_भेद(arg, "mbox"))
		*opt_मूल्य = PATCH_FORMAT_MBOX;
	अन्यथा जर (!म_भेद(arg, "stgit"))
		*opt_मूल्य = PATCH_FORMAT_STGIT;
	अन्यथा जर (!म_भेद(arg, "stgit-series"))
		*opt_मूल्य = PATCH_FORMAT_STGIT_SERIES;
	अन्यथा जर (!म_भेद(arg, "hg"))
		*opt_मूल्य = PATCH_FORMAT_HG;
	अन्यथा जर (!म_भेद(arg, "mboxrd"))
		*opt_मूल्य = PATCH_FORMAT_MBOXRD;
	/*
	 * Please update $__git_patchformat in git-completion.bash
	 * when you add new options
	 */
	अन्यथा
		परत error(_("invalid value for '%s': '%s'"),
			     "--patch-format", arg);
	परत 0;
}

स्थिर पूर्णांक parse_opt_show_current_patch(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	पूर्णांक *opt_मूल्य = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	जर (!arg)
		*opt_मूल्य = opt->defval;
	अन्यथा जर (!म_भेद(arg, "raw"))
		*opt_मूल्य = RESUME_SHOW_PATCH_RAW;
	अन्यथा जर (!म_भेद(arg, "diff"))
		*opt_मूल्य = RESUME_SHOW_PATCH_DIFF;
	/*
	 * Please update $__git_showcurrentpatch in git-completion.bash
	 * when you add new options
	 */
	अन्यथा
		परत error(_("invalid value for '%s': '%s'"),
			     "--show-current-patch", arg);
	परत 0;
}

पूर्णांक cmd_am(पूर्णांक argc,
	   स्थिरांक अक्षर **argv,
	   स्थिरांक अक्षर *prefix,
	   संरचना repository *repo UNUSED)
{
	संरचना am_state state;
	पूर्णांक द्विमान = -1;
	पूर्णांक keep_cr = -1;
	पूर्णांक patch_format = PATCH_FORMAT_UNKNOWN;
	गणना resume_type resume_mode = RESUME_FALSE;
	पूर्णांक in_progress;
	पूर्णांक ret = 0;

	स्थिरांक अक्षर * स्थिरांक usage[] = {
		N_("git am [<options>] [(<mbox> | <Maildir>)...]"),
		N_("git am [<options>] (--continue | --skip | --abort)"),
		रिक्त
	};

	संरचना option options[] = {
		OPT_BOOL('i', "interactive", &state.interactive,
			N_("run interactively")),
		OPT_BOOL('n', "no-verify", &state.no_verify,
			N_("bypass pre-applypatch and applypatch-msg hooks")),
		OPT_HIDDEN_BOOL('b', "binary", &द्विमान,
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
		OPT_PASSTHRU_ARGV(0, "ignore-space-change", &state.git_apply_opts, रिक्त,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU_ARGV(0, "ignore-whitespace", &state.git_apply_opts, रिक्त,
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
		OPT_PASSTHRU_ARGV('C', रिक्त, &state.git_apply_opts, N_("n"),
			N_("pass it through git-apply"),
			0),
		OPT_PASSTHRU_ARGV('p', रिक्त, &state.git_apply_opts, N_("num"),
			N_("pass it through git-apply"),
			0),
		OPT_CALLBACK(0, "patch-format", &patch_format, N_("format"),
			N_("format the patch(es) are in"),
			parse_opt_patchformat),
		OPT_PASSTHRU_ARGV(0, "reject", &state.git_apply_opts, रिक्त,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_STRING(0, "resolvemsg", &state.resolvemsg, रिक्त,
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
			.precision = आकार(resume_mode),
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

	repo_config(the_repository, git_default_config, रिक्त);

	am_state_init(&state);

	in_progress = am_in_progress(&state);
	जर (in_progress)
		am_load(&state);

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	जर (द्विमान >= 0)
		fprintf_ln(मानक_त्रुटि, _("The -b/--binary option has been a no-op for long time, and\n"
				"it will be removed. Please do not use it anymore."));

	/* Ensure a valid committer ident can be constructed */
	git_committer_info(IDENT_STRICT);

	जर (repo_read_index_preload(the_repository, रिक्त, 0) < 0)
		die(_("failed to read the index"));

	जर (in_progress) {
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
		जर (argc || (resume_mode == RESUME_FALSE && !isatty(0)))
			die(_("previous rebase directory %s still exists but mbox given."),
				state.dir);

		जर (resume_mode == RESUME_FALSE)
			resume_mode = RESUME_APPLY;

		जर (state.signoff == SIGNOFF_EXPLICIT)
			am_append_signoff(&state);
	} अन्यथा {
		संरचना strvec paths = STRVEC_INIT;
		पूर्णांक i;

		/*
		 * Handle stray state directory in the independent-run case. In
		 * the --rebasing case, it is up to the caller to take care of
		 * stray directories.
		 */
		जर (file_exists(state.dir) && !state.rebasing) {
			जर (resume_mode == RESUME_ABORT || resume_mode == RESUME_QUIT) {
				am_destroy(&state);
				am_state_release(&state);
				परत 0;
			}

			die(_("Stray %s directory found.\n"
				"Use \"git am --abort\" to remove it."),
				state.dir);
		}

		जर (resume_mode)
			die(_("Resolve operation not in progress, we are not resuming."));

		साठी (i = 0; i < argc; i++) {
			जर (is_absolute_path(argv[i]) || !prefix)
				strvec_ढकला(&paths, argv[i]);
			अन्यथा
				strvec_ढकला(&paths, mkpath("%s/%s", prefix, argv[i]));
		}

		जर (state.interactive && !paths.nr)
			die(_("interactive mode requires patches on the command line"));

		am_setup(&state, patch_format, paths.v, keep_cr);

		strvec_clear(&paths);
	}

	निवड (resume_mode) {
	स्थिती RESUME_FALSE:
		am_run(&state, 0);
		थांबा;
	स्थिती RESUME_APPLY:
		am_run(&state, 1);
		थांबा;
	स्थिती RESUME_RESOLVED:
	स्थिती RESUME_ALLOW_EMPTY:
		am_resolve(&state, resume_mode == RESUME_ALLOW_EMPTY ? 1 : 0);
		थांबा;
	स्थिती RESUME_SKIP:
		am_skip(&state);
		थांबा;
	स्थिती RESUME_ABORT:
		am_abort(&state);
		थांबा;
	स्थिती RESUME_QUIT:
		am_rerere_clear();
		am_destroy(&state);
		थांबा;
	स्थिती RESUME_SHOW_PATCH_RAW:
	स्थिती RESUME_SHOW_PATCH_DIFF:
		ret = show_patch(&state, resume_mode);
		थांबा;
	पूर्वनिर्धारित:
		BUG("invalid resume value");
	}

	am_state_release(&state);

	परत ret;
}
