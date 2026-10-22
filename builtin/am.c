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
स्थिर पूर्णाङ्क linelen(ध्रुव वर्ण *सन्देशः)
{
	प्रत्यागम strchrnul(सन्देशः, '\n') - सन्देशः;
}

/**
 * Returns true if `str` consists of only whitespace, false otherwise.
 */
स्थिर पूर्णाङ्क str_isspace(ध्रुव वर्ण *str)
{
	प्रति (; *str; str++)
		जइ (!isspace(*str))
			प्रत्यागम 0;

	प्रत्यागम 1;
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
	वर्ण *dir;

	/* current and last patch numbers, 1-indexed */
	पूर्णाङ्क cur;
	पूर्णाङ्क last;

	/* commit metadata and message */
	वर्ण *author_name;
	वर्ण *author_email;
	वर्ण *author_date;
	वर्ण *सन्देशः;
	size_t सन्देशः_len;

	/* when --rebasing, records the original commit the patch came from */
	संरचना object_id orig_commit;

	/* number of digits in patch filename */
	पूर्णाङ्क prec;

	/* various operating modes and command line options */
	पूर्णाङ्क interactive;
	पूर्णाङ्क no_verify;
	पूर्णाङ्क threeway;
	पूर्णाङ्क quiet;
	पूर्णाङ्क signoff; /* enum signoff_type */
	पूर्णाङ्क utf8;
	पूर्णाङ्क keep; /* enum keep_type */
	पूर्णाङ्क message_id;
	पूर्णाङ्क scissors; /* enum scissors_type */
	पूर्णाङ्क quoted_cr; /* enum quoted_cr_action */
	पूर्णाङ्क empty_type; /* enum empty_action */
	संरचना strvec git_apply_opts;
	ध्रुव वर्ण *resolvemsg;
	पूर्णाङ्क committer_date_is_author_date;
	पूर्णाङ्क ignore_date;
	पूर्णाङ्क allow_rerere_autoupdate;
	ध्रुव वर्ण *sign_commit;
	पूर्णाङ्क rebasing;
};

/**
 * Initializes am_state with the default values.
 */
स्थिर शून्य am_state_init(संरचना am_state *state)
{
	पूर्णाङ्क gpgsign;

	memset(state, 0, परिमाण(*state));

	state->dir = repo_git_path(the_repository, "rebase-apply");

	state->prec = 4;

	repo_config_get_bool(the_repository, "am.threeway", &state->threeway);

	state->utf8 = 1;

	repo_config_get_bool(the_repository, "am.messageid", &state->message_id);

	state->scissors = SCISSORS_UNSET;
	state->quoted_cr = quoted_cr_unset;

	strvec_init(&state->git_apply_opts);

	जइ (!repo_config_get_bool(the_repository, "commit.gpgsign", &gpgsign))
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
	मुक्त(state->सन्देशः);
	strvec_clear(&state->git_apply_opts);
}

स्थिर पूर्णाङ्क am_option_parse_quoted_cr(ध्रुव संरचना option *opt,
				     ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);

	जइ (mailinfo_parse_quoted_cr_action(arg, opt->मानम्) != 0)
		प्रत्यागम error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क am_option_parse_empty(ध्रुव संरचना option *opt,
				     ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *opt_मानम् = opt->मानम्;

	BUG_ON_OPT_NEG(unset);

	जइ (!strcmp(arg, "stop"))
		*opt_मानम् = STOP_ON_EMPTY_COMMIT;
	अण्णहा जइ (!strcmp(arg, "drop"))
		*opt_मानम् = DROP_EMPTY_COMMIT;
	अण्णहा जइ (!strcmp(arg, "keep"))
		*opt_मानम् = KEEP_EMPTY_COMMIT;
	अण्णहा
		प्रत्यागम error(_("invalid value for '%s': '%s'"), "--empty", arg);

	प्रत्यागम 0;
}

/**
 * Returns path relative to the am_state directory.
 */
स्थिर अन्तःस्थ ध्रुव वर्ण *am_path(ध्रुव संरचना am_state *state, ध्रुव वर्ण *path)
{
	प्रत्यागम mkpath("%s/%s", state->dir, path);
}

/**
 * For convenience to call write_file()
 */
स्थिर शून्य write_state_text(ध्रुव संरचना am_state *state,
			     ध्रुव वर्ण *name, ध्रुव वर्ण *string)
{
	write_file(am_path(state, name), "%s", string);
}

स्थिर शून्य write_state_गणनम्(ध्रुव संरचना am_state *state,
			      ध्रुव वर्ण *name, पूर्णाङ्क मानम्)
{
	write_file(am_path(state, name), "%d", मानम्);
}

स्थिर शून्य write_state_bool(ध्रुव संरचना am_state *state,
			     ध्रुव वर्ण *name, पूर्णाङ्क मानम्)
{
	write_state_text(state, name, मानम् ? "t" : "f");
}

/**
 * If state->quiet is false, calls fprintf(fp, fmt, ...), and appends a newline
 * at the end.
 */
__attribute__((format (printf, 3, 4)))
स्थिर शून्य say(ध्रुव संरचना am_state *state, FILE *fp, ध्रुव वर्ण *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	जइ (!state->quiet) {
		vfprintf(fp, fmt, ap);
		putc('\n', fp);
	}
	va_end(ap);
}

/**
 * Returns 1 if there is an am session in progress, 0 otherwise.
 */
स्थिर पूर्णाङ्क am_in_progress(ध्रुव संरचना am_state *state)
{
	संरचना stat st;

	जइ (lstat(state->dir, &st) < 0 || !S_ISDIR(st.st_mode))
		प्रत्यागम 0;
	जइ (lstat(am_path(state, "last"), &st) || !S_ISREG(st.st_mode))
		प्रत्यागम 0;
	जइ (lstat(am_path(state, "next"), &st) || !S_ISREG(st.st_mode))
		प्रत्यागम 0;
	प्रत्यागम 1;
}

/**
 * Reads the contents of `file` in the `state` directory into `sb`. Returns the
 * number of bytes read on success, -1 if the file does not exist. If `trim` is
 * set, trailing whitespace will be removed.
 */
स्थिर पूर्णाङ्क read_state_file(संरचना strbuf *sb, ध्रुव संरचना am_state *state,
			ध्रुव वर्ण *file, पूर्णाङ्क trim)
{
	strbuf_reset(sb);

	जइ (strbuf_read_file(sb, am_path(state, file), 0) >= 0) {
		जइ (trim)
			strbuf_trim(sb);

		प्रत्यागम sb->len;
	}

	जइ (errno == ENOENT)
		प्रत्यागम -1;

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
स्थिर पूर्णाङ्क read_am_author_script(संरचना am_state *state)
{
	ध्रुव वर्ण *filename = am_path(state, "author-script");

	प्रतिज्ञा(!state->author_name);
	प्रतिज्ञा(!state->author_email);
	प्रतिज्ञा(!state->author_date);

	प्रत्यागम read_author_script(filename, &state->author_name,
				  &state->author_email, &state->author_date, 1);
}

/**
 * Saves state->author_name, state->author_email and state->author_date in the
 * state directory's "author-script" file.
 */
स्थिर शून्य write_author_script(ध्रुव संरचना am_state *state)
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
स्थिर पूर्णाङ्क read_commit_सन्देशः(संरचना am_state *state)
{
	संरचना strbuf sb = STRBUF_INIT;

	प्रतिज्ञा(!state->सन्देशः);

	जइ (read_state_file(&sb, state, "final-commit", 0) < 0) {
		strbuf_release(&sb);
		प्रत्यागम -1;
	}

	state->सन्देशः = strbuf_detach(&sb, &state->सन्देशः_len);
	प्रत्यागम 0;
}

/**
 * Saves state->msg in the state directory's "final-commit" file.
 */
स्थिर शून्य write_commit_सन्देशः(ध्रुव संरचना am_state *state)
{
	ध्रुव वर्ण *filename = am_path(state, "final-commit");
	write_file_buf(filename, state->सन्देशः, state->सन्देशः_len);
}

/**
 * Loads state from disk.
 */
स्थिर शून्य am_load(संरचना am_state *state)
{
	संरचना strbuf sb = STRBUF_INIT;

	जइ (read_state_file(&sb, state, "next", 1) < 0)
		BUG("state file 'next' does not exist");
	state->cur = strtol(sb.buf, रिक्त, 10);

	जइ (read_state_file(&sb, state, "last", 1) < 0)
		BUG("state file 'last' does not exist");
	state->last = strtol(sb.buf, रिक्त, 10);

	जइ (read_am_author_script(state) < 0)
		die(_("could not parse author script"));

	read_commit_सन्देशः(state);

	जइ (read_state_file(&sb, state, "original-commit", 1) < 0)
		oidclr(&state->orig_commit, the_repository->hash_algo);
	अण्णहा जइ (get_oid_hex(sb.buf, &state->orig_commit) < 0)
		die(_("could not parse %s"), am_path(state, "original-commit"));

	read_state_file(&sb, state, "threeway", 1);
	state->threeway = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "quiet", 1);
	state->quiet = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "sign", 1);
	state->signoff = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "utf8", 1);
	state->utf8 = !strcmp(sb.buf, "t");

	जइ (file_exists(am_path(state, "rerere-autoupdate"))) {
		read_state_file(&sb, state, "rerere-autoupdate", 1);
		state->allow_rerere_autoupdate = strcmp(sb.buf, "t") ?
			RERERE_NOAUTOUPDATE : RERERE_AUTOUPDATE;
	} अण्णहा {
		state->allow_rerere_autoupdate = 0;
	}

	read_state_file(&sb, state, "keep", 1);
	जइ (!strcmp(sb.buf, "t"))
		state->keep = KEEP_TRUE;
	अण्णहा जइ (!strcmp(sb.buf, "b"))
		state->keep = KEEP_NON_PATCH;
	अण्णहा
		state->keep = KEEP_FALSE;

	read_state_file(&sb, state, "messageid", 1);
	state->message_id = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "scissors", 1);
	जइ (!strcmp(sb.buf, "t"))
		state->scissors = SCISSORS_TRUE;
	अण्णहा जइ (!strcmp(sb.buf, "f"))
		state->scissors = SCISSORS_FALSE;
	अण्णहा
		state->scissors = SCISSORS_UNSET;

	read_state_file(&sb, state, "quoted-cr", 1);
	जइ (!*sb.buf)
		state->quoted_cr = quoted_cr_unset;
	अण्णहा जइ (mailinfo_parse_quoted_cr_action(sb.buf, &state->quoted_cr) != 0)
		die(_("could not parse %s"), am_path(state, "quoted-cr"));

	read_state_file(&sb, state, "apply-opt", 1);
	strvec_clear(&state->git_apply_opts);
	जइ (sq_dequote_to_strvec(sb.buf, &state->git_apply_opts) < 0)
		die(_("could not parse %s"), am_path(state, "apply-opt"));

	state->rebasing = !!file_exists(am_path(state, "rebasing"));

	strbuf_release(&sb);
}

/**
 * Removes the am_state directory, forcefully terminating the current am
 * session.
 */
स्थिर शून्य am_destroy(ध्रुव संरचना am_state *state)
{
	संरचना strbuf sb = STRBUF_INIT;

	strbuf_addstr(&sb, state->dir);
	remove_dir_recursively(&sb, 0);
	strbuf_release(&sb);
}

/**
 * Runs applypatch-msg hook. Returns its exit code.
 */
स्थिर पूर्णाङ्क run_applypatch_सन्देशः_hook(संरचना am_state *state)
{
	पूर्णाङ्क ret = 0;

	प्रतिज्ञा(state->सन्देशः);

	जइ (!state->no_verify) {
		संरचना run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;
		strvec_प्रक्षिप(&opt.args, am_path(state, "final-commit"));
		ret = run_hooks_opt(the_repository, "applypatch-msg", &opt);
	}

	जइ (!ret) {
		FREE_AND_NULL(state->सन्देशः);
		जइ (read_commit_सन्देशः(state) < 0)
			die(_("'%s' was deleted by the applypatch-msg hook"),
				am_path(state, "final-commit"));
	}

	प्रत्यागम ret;
}

/**
 * Runs post-rewrite hook. Returns it exit code.
 */
स्थिर पूर्णाङ्क run_post_rewrite_hook(ध्रुव संरचना am_state *state)
{
	संरचना run_hooks_opt opt = RUN_HOOKS_OPT_INIT;

	strvec_प्रक्षिप(&opt.args, "rebase");
	opt.path_to_stdin = am_path(state, "rewritten");

	प्रत्यागम run_hooks_opt(the_repository, "post-rewrite", &opt);
}

/**
 * Reads the state directory's "rewritten" file, and copies notes from the old
 * commits listed in the file to their rewritten commits.
 *
 * Returns 0 on success, -1 on failure.
 */
स्थिर पूर्णाङ्क copy_notes_for_rebase(ध्रुव संरचना am_state *state)
{
	संरचना notes_rewrite_cfg *c;
	संरचना strbuf sb = STRBUF_INIT;
	ध्रुव वर्ण *invalid_पङ्क्तिः = _("Malformed input line: '%s'.");
	ध्रुव वर्ण *सन्देशः = "Notes added by 'git rebase'";
	FILE *fp;
	पूर्णाङ्क ret = 0;

	प्रतिज्ञा(state->rebasing);

	c = init_copy_notes_for_rewrite("rebase");
	जइ (!c)
		प्रत्यागम 0;

	fp = xfopen(am_path(state, "rewritten"), "r");

	जाव (!strbuf_getline_lf(&sb, fp)) {
		संरचना object_id from_obj, to_obj;
		ध्रुव वर्ण *p;

		जइ (sb.len != the_hash_algo->hexsz * 2 + 1) {
			ret = error(invalid_पङ्क्तिः, sb.buf);
			गच्छ finish;
		}

		जइ (parse_oid_hex(sb.buf, &from_obj, &p)) {
			ret = error(invalid_पङ्क्तिः, sb.buf);
			गच्छ finish;
		}

		जइ (*p != ' ') {
			ret = error(invalid_पङ्क्तिः, sb.buf);
			गच्छ finish;
		}

		जइ (get_oid_hex(p + 1, &to_obj)) {
			ret = error(invalid_पङ्क्तिः, sb.buf);
			गच्छ finish;
		}

		जइ (copy_note_for_rewrite(c, &from_obj, &to_obj))
			ret = error(_("Failed to copy notes from '%s' to '%s'"),
					oid_to_hex(&from_obj), oid_to_hex(&to_obj));
	}

finish:
	finish_copy_notes_for_rewrite(the_repository, c, सन्देशः);
	fclose(fp);
	strbuf_release(&sb);
	प्रत्यागम ret;
}

/**
 * Determines if the file looks like a piece of RFC2822 mail by grabbing all
 * non-indented lines and checking if they look like they begin with valid
 * header field names.
 *
 * Returns 1 if the file looks like a piece of mail, 0 otherwise.
 */
स्थिर पूर्णाङ्क is_mail(FILE *fp)
{
	ध्रुव वर्ण *header_regex = "^[!-9;-~]+:";
	संरचना strbuf sb = STRBUF_INIT;
	regex_t regex;
	पूर्णाङ्क ret = 1;

	जइ (fseek(fp, 0L, SEEK_SET))
		die_errno(_("fseek failed"));

	जइ (regcomp(&regex, header_regex, REG_NOSUB | REG_EXTENDED))
		die("invalid pattern: %s", header_regex);

	जाव (!strbuf_getline(&sb, fp)) {
		जइ (!sb.len)
			भङ्ग; /* End of header */

		/* Ignore indented folded lines */
		जइ (*sb.buf == '\t' || *sb.buf == ' ')
			अनुवर्त;

		/* It's a header if it matches header_regex */
		जइ (regexec(&regex, sb.buf, 0, रिक्त, 0)) {
			ret = 0;
			गच्छ done;
		}
	}

done:
	regfree(&regex);
	strbuf_release(&sb);
	प्रत्यागम ret;
}

/**
 * Attempts to detect the patch_format of the patches contained in `paths`,
 * returning the PATCH_FORMAT_* enum value. Returns PATCH_FORMAT_UNKNOWN if
 * detection fails.
 */
स्थिर पूर्णाङ्क detect_patch_format(ध्रुव वर्ण **paths)
{
	गणना patch_format ret = PATCH_FORMAT_UNKNOWN;
	संरचना strbuf l1 = STRBUF_INIT;
	संरचना strbuf l2 = STRBUF_INIT;
	संरचना strbuf l3 = STRBUF_INIT;
	FILE *fp;

	/*
	 * We default to mbox format if input is from stdin and for directories
	 */
	जइ (!*paths || !strcmp(*paths, "-") || is_directory(*paths))
		प्रत्यागम PATCH_FORMAT_MBOX;

	/*
	 * Otherwise, check the first few lines of the first patch, starting
	 * from the first non-blank line, to try to detect its format.
	 */

	fp = xfopen(*paths, "r");

	जाव (!strbuf_getline(&l1, fp)) {
		जइ (l1.len)
			भङ्ग;
	}

	जइ (starts_with(l1.buf, "From ") || starts_with(l1.buf, "From: ")) {
		ret = PATCH_FORMAT_MBOX;
		गच्छ done;
	}

	जइ (starts_with(l1.buf, "# This series applies on GIT commit")) {
		ret = PATCH_FORMAT_STGIT_SERIES;
		गच्छ done;
	}

	जइ (!strcmp(l1.buf, "# HG changeset patch")) {
		ret = PATCH_FORMAT_HG;
		गच्छ done;
	}

	strbuf_getline(&l2, fp);
	strbuf_getline(&l3, fp);

	/*
	 * If the second line is empty and the third is a From, Author or Date
	 * entry, this is likely an StGit patch.
	 */
	जइ (l1.len && !l2.len &&
		(starts_with(l3.buf, "From:") ||
		 starts_with(l3.buf, "Author:") ||
		 starts_with(l3.buf, "Date:"))) {
		ret = PATCH_FORMAT_STGIT;
		गच्छ done;
	}

	जइ (l1.len && is_mail(fp)) {
		ret = PATCH_FORMAT_MBOX;
		गच्छ done;
	}

done:
	fclose(fp);
	strbuf_release(&l1);
	strbuf_release(&l2);
	strbuf_release(&l3);
	प्रत्यागम ret;
}

/**
 * Splits out individual email patches from `paths`, where each path is either
 * a mbox file or a Maildir. Returns 0 on success, -1 on failure.
 */
स्थिर पूर्णाङ्क split_mail_mbox(संरचना am_state *state, ध्रुव वर्ण **paths,
				पूर्णाङ्क keep_cr, पूर्णाङ्क mboxrd)
{
	संरचना child_process cp = CHILD_PROCESS_INIT;
	संरचना strbuf last = STRBUF_INIT;
	पूर्णाङ्क ret;

	cp.git_cmd = 1;
	strvec_प्रक्षिप(&cp.args, "mailsplit");
	strvec_pushf(&cp.args, "-d%d", state->prec);
	strvec_pushf(&cp.args, "-o%s", state->dir);
	strvec_प्रक्षिप(&cp.args, "-b");
	जइ (keep_cr)
		strvec_प्रक्षिप(&cp.args, "--keep-cr");
	जइ (mboxrd)
		strvec_प्रक्षिप(&cp.args, "--mboxrd");
	strvec_प्रक्षिप(&cp.args, "--");
	strvec_pushv(&cp.args, paths);

	ret = capture_command(&cp, &last, 8);
	जइ (ret)
		गच्छ exit;

	state->cur = 1;
	state->last = strtol(last.buf, रिक्त, 10);

exit:
	strbuf_release(&last);
	प्रत्यागम ret ? -1 : 0;
}

/**
 * Callback signature for split_mail_conv(). The foreign patch should be
 * read from `in`, and the converted patch (in RFC2822 mail format) should be
 * written to `out`. Return 0 on success, or -1 on failure.
 */
प्रकारनिर्देश पूर्णाङ्क (*mail_conv_fn)(FILE *out, FILE *in, पूर्णाङ्क keep_cr);

/**
 * Calls `fn` for each file in `paths` to convert the foreign patch to the
 * RFC2822 mail format suitable for parsing with git-mailinfo.
 *
 * Returns 0 on success, -1 on failure.
 */
स्थिर पूर्णाङ्क split_mail_conv(mail_conv_fn fn, संरचना am_state *state,
			ध्रुव वर्ण **paths, पूर्णाङ्क keep_cr)
{
	स्थिर ध्रुव वर्ण *stdin_only[] = {"-", रिक्त};
	पूर्णाङ्क i;

	जइ (!*paths)
		paths = stdin_only;

	प्रति (i = 0; *paths; paths++, i++) {
		FILE *in, *out;
		ध्रुव वर्ण *mail;
		पूर्णाङ्क ret;

		जइ (!strcmp(*paths, "-"))
			in = stdin;
		अण्णहा
			in = fopen(*paths, "r");

		जइ (!in)
			प्रत्यागम error_errno(_("could not open '%s' for reading"),
					   *paths);

		mail = mkpath("%s/%0*d", state->dir, state->prec, i + 1);

		out = fopen(mail, "w");
		जइ (!out) {
			जइ (in != stdin)
				fclose(in);
			प्रत्यागम error_errno(_("could not open '%s' for writing"),
					   mail);
		}

		ret = fn(out, in, keep_cr);

		fclose(out);
		जइ (in != stdin)
			fclose(in);

		जइ (ret)
			प्रत्यागम error(_("could not parse patch '%s'"), *paths);
	}

	state->cur = 1;
	state->last = i;
	प्रत्यागम 0;
}

/**
 * A split_mail_conv() callback that converts an StGit patch to an RFC2822
 * message suitable for parsing with git-mailinfo.
 */
स्थिर पूर्णाङ्क stgit_patch_to_mail(FILE *out, FILE *in, पूर्णाङ्क keep_cr UNUSED)
{
	संरचना strbuf sb = STRBUF_INIT;
	पूर्णाङ्क subject_printed = 0;

	जाव (!strbuf_getline_lf(&sb, in)) {
		ध्रुव वर्ण *str;

		जइ (str_isspace(sb.buf))
			अनुवर्त;
		अण्णहा जइ (skip_prefix(sb.buf, "Author:", &str))
			fprintf(out, "From:%s\n", str);
		अण्णहा जइ (starts_with(sb.buf, "From") || starts_with(sb.buf, "Date"))
			fprintf(out, "%s\n", sb.buf);
		अण्णहा जइ (!subject_printed) {
			fprintf(out, "Subject: %s\n", sb.buf);
			subject_printed = 1;
		} अण्णहा {
			fprintf(out, "\n%s\n", sb.buf);
			भङ्ग;
		}
	}

	strbuf_reset(&sb);
	जाव (strbuf_fread(&sb, 8192, in) > 0) {
		fwrite(sb.buf, 1, sb.len, out);
		strbuf_reset(&sb);
	}

	strbuf_release(&sb);
	प्रत्यागम 0;
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
स्थिर पूर्णाङ्क split_mail_stgit_series(संरचना am_state *state, ध्रुव वर्ण **paths,
					पूर्णाङ्क keep_cr)
{
	ध्रुव वर्ण *series_dir;
	वर्ण *series_dir_buf;
	FILE *fp;
	संरचना strvec patches = STRVEC_INIT;
	संरचना strbuf sb = STRBUF_INIT;
	पूर्णाङ्क ret;

	जइ (!paths[0] || paths[1])
		प्रत्यागम error(_("Only one StGIT patch series can be applied at once"));

	series_dir_buf = xstrdup(*paths);
	series_dir = dirname(series_dir_buf);

	fp = fopen(*paths, "r");
	जइ (!fp) {
		मुक्त(series_dir_buf);
		प्रत्यागम error_errno(_("could not open '%s' for reading"), *paths);
	}

	जाव (!strbuf_getline_lf(&sb, fp)) {
		जइ (*sb.buf == '#')
			अनुवर्त; /* skip comment lines */

		strvec_प्रक्षिप(&patches, mkpath("%s/%s", series_dir, sb.buf));
	}

	fclose(fp);
	strbuf_release(&sb);
	मुक्त(series_dir_buf);

	ret = split_mail_conv(stgit_patch_to_mail, state, patches.v, keep_cr);

	strvec_clear(&patches);
	प्रत्यागम ret;
}

/**
 * A split_patches_conv() callback that converts a mercurial patch to a RFC2822
 * message suitable for parsing with git-mailinfo.
 */
स्थिर पूर्णाङ्क hg_patch_to_mail(FILE *out, FILE *in, पूर्णाङ्क keep_cr UNUSED)
{
	संरचना strbuf sb = STRBUF_INIT;
	पूर्णाङ्क rc = 0;

	जाव (!strbuf_getline_lf(&sb, in)) {
		ध्रुव वर्ण *str;

		जइ (skip_prefix(sb.buf, "# User ", &str))
			fprintf(out, "From: %s\n", str);
		अण्णहा जइ (skip_prefix(sb.buf, "# Date ", &str)) {
			timestamp_t timestamp;
			long tz, tz2;
			वर्ण *end;

			errno = 0;
			timestamp = parse_timestamp(str, &end, 10);
			जइ (errno) {
				rc = error(_("invalid timestamp"));
				गच्छ exit;
			}

			जइ (!skip_prefix(end, " ", &str)) {
				rc = error(_("invalid Date line"));
				गच्छ exit;
			}

			errno = 0;
			tz = strtol(str, &end, 10);
			जइ (errno) {
				rc = error(_("invalid timezone offset"));
				गच्छ exit;
			}

			जइ (*end) {
				rc = error(_("invalid Date line"));
				गच्छ exit;
			}

			/*
			 * mercurial's timezone is in seconds west of UTC,
			 * however git's timezone is in hours + minutes east of
			 * UTC. Convert it.
			 */
			tz2 = labs(tz) / 3600 * 100 + labs(tz) % 3600 / 60;
			जइ (tz > 0)
				tz2 = -tz2;

			fprintf(out, "Date: %s\n", show_date(timestamp, tz2, DATE_MODE(RFC2822)));
		} अण्णहा जइ (starts_with(sb.buf, "# ")) {
			अनुवर्त;
		} अण्णहा {
			fprintf(out, "\n%s\n", sb.buf);
			भङ्ग;
		}
	}

	strbuf_reset(&sb);
	जाव (strbuf_fread(&sb, 8192, in) > 0) {
		fwrite(sb.buf, 1, sb.len, out);
		strbuf_reset(&sb);
	}
exit:
	strbuf_release(&sb);
	प्रत्यागम rc;
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
स्थिर पूर्णाङ्क split_mail(संरचना am_state *state, गणना patch_format patch_format,
			ध्रुव वर्ण **paths, पूर्णाङ्क keep_cr)
{
	जइ (keep_cr < 0) {
		keep_cr = 0;
		repo_config_get_bool(the_repository, "am.keepcr", &keep_cr);
	}

	विकल्प (patch_format) {
	पक्ष PATCH_FORMAT_MBOX:
		प्रत्यागम split_mail_mbox(state, paths, keep_cr, 0);
	पक्ष PATCH_FORMAT_STGIT:
		प्रत्यागम split_mail_conv(stgit_patch_to_mail, state, paths, keep_cr);
	पक्ष PATCH_FORMAT_STGIT_SERIES:
		प्रत्यागम split_mail_stgit_series(state, paths, keep_cr);
	पक्ष PATCH_FORMAT_HG:
		प्रत्यागम split_mail_conv(hg_patch_to_mail, state, paths, keep_cr);
	पक्ष PATCH_FORMAT_MBOXRD:
		प्रत्यागम split_mail_mbox(state, paths, keep_cr, 1);
	स्वभाव:
		BUG("invalid patch_format");
	}
	प्रत्यागम -1;
}

/**
 * Setup a new am session for applying patches
 */
स्थिर शून्य am_setup(संरचना am_state *state, गणना patch_format patch_format,
			ध्रुव वर्ण **paths, पूर्णाङ्क keep_cr)
{
	संरचना object_id curr_head;
	ध्रुव वर्ण *str;
	संरचना strbuf sb = STRBUF_INIT;

	जइ (!patch_format)
		patch_format = detect_patch_format(paths);

	जइ (!patch_format) {
		fprintf_ln(stderr, _("Patch format detection failed."));
		die(रिक्त);
	}

	जइ (mkdir(state->dir, 0777) < 0 && errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), state->dir);
	refs_delete_ref(get_main_ref_store(the_repository), रिक्त,
			"REBASE_HEAD", रिक्त, REF_NO_DEREF);

	जइ (split_mail(state, patch_format, paths, keep_cr) < 0) {
		am_destroy(state);
		die(_("Failed to split patches."));
	}

	जइ (state->rebasing)
		state->threeway = 1;

	write_state_bool(state, "threeway", state->threeway);
	write_state_bool(state, "quiet", state->quiet);
	write_state_bool(state, "sign", state->signoff);
	write_state_bool(state, "utf8", state->utf8);

	जइ (state->allow_rerere_autoupdate)
		write_state_bool(state, "rerere-autoupdate",
			 state->allow_rerere_autoupdate == RERERE_AUTOUPDATE);

	विकल्प (state->keep) {
	पक्ष KEEP_FALSE:
		str = "f";
		भङ्ग;
	पक्ष KEEP_TRUE:
		str = "t";
		भङ्ग;
	पक्ष KEEP_NON_PATCH:
		str = "b";
		भङ्ग;
	स्वभाव:
		BUG("invalid value for state->keep");
	}

	write_state_text(state, "keep", str);
	write_state_bool(state, "messageid", state->message_id);

	विकल्प (state->scissors) {
	पक्ष SCISSORS_UNSET:
		str = "";
		भङ्ग;
	पक्ष SCISSORS_FALSE:
		str = "f";
		भङ्ग;
	पक्ष SCISSORS_TRUE:
		str = "t";
		भङ्ग;
	स्वभाव:
		BUG("invalid value for state->scissors");
	}
	write_state_text(state, "scissors", str);

	विकल्प (state->quoted_cr) {
	पक्ष quoted_cr_unset:
		str = "";
		भङ्ग;
	पक्ष quoted_cr_nowarn:
		str = "nowarn";
		भङ्ग;
	पक्ष quoted_cr_warn:
		str = "warn";
		भङ्ग;
	पक्ष quoted_cr_strip:
		str = "strip";
		भङ्ग;
	स्वभाव:
		BUG("invalid value for state->quoted_cr");
	}
	write_state_text(state, "quoted-cr", str);

	sq_quote_argv(&sb, state->git_apply_opts.v);
	write_state_text(state, "apply-opt", sb.buf);

	जइ (state->rebasing)
		write_state_text(state, "rebasing", "");
	अण्णहा
		write_state_text(state, "applying", "");

	जइ (!repo_get_oid(the_repository, "HEAD", &curr_head)) {
		write_state_text(state, "abort-safety", oid_to_hex(&curr_head));
		जइ (!state->rebasing)
			refs_update_ref(get_main_ref_store(the_repository),
					"am", "ORIG_HEAD", &curr_head, रिक्त,
					0,
					UPDATE_REFS_DIE_ON_ERR);
	} अण्णहा {
		write_state_text(state, "abort-safety", "");
		जइ (!state->rebasing)
			refs_delete_ref(get_main_ref_store(the_repository),
					रिक्त, "ORIG_HEAD", रिक्त, 0);
	}

	/*
	 * NOTE: Since the "next" and "last" files determine if an am_state
	 * session is in progress, they should be written last.
	 */

	write_state_गणनम्(state, "next", state->cur);
	write_state_गणनम्(state, "last", state->last);

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
	FREE_AND_NULL(state->सन्देशः);
	state->सन्देशः_len = 0;

	unlink(am_path(state, "author-script"));
	unlink(am_path(state, "final-commit"));

	oidclr(&state->orig_commit, the_repository->hash_algo);
	unlink(am_path(state, "original-commit"));
	refs_delete_ref(get_main_ref_store(the_repository), रिक्त,
			"REBASE_HEAD", रिक्त, REF_NO_DEREF);

	जइ (!repo_get_oid(the_repository, "HEAD", &head))
		write_state_text(state, "abort-safety", oid_to_hex(&head));
	अण्णहा
		write_state_text(state, "abort-safety", "");

	state->cur++;
	write_state_गणनम्(state, "next", state->cur);
}

/**
 * Returns the filename of the current patch email.
 */
स्थिर ध्रुव वर्ण *msgnum(ध्रुव संरचना am_state *state)
{
	स्थिर संरचना strbuf sb = STRBUF_INIT;

	strbuf_reset(&sb);
	strbuf_addf(&sb, "%0*d", state->prec, state->cur);

	प्रत्यागम sb.buf;
}

/**
 * Dies with a user-friendly message on how to proceed after resolving the
 * problem. This message can be overridden with state->resolvemsg.
 */
स्थिर शून्य NORETURN die_user_resolve(ध्रुव संरचना am_state *state)
{
	जइ (state->resolvemsg) {
		advise_if_enabled(ADVICE_MERGE_CONFLICT, "%s", state->resolvemsg);
	} अण्णहा {
		ध्रुव वर्ण *cmdline = state->interactive ? "git am -i" : "git am";
		संरचना strbuf sb = STRBUF_INIT;

		strbuf_addf(&sb, _("When you have resolved this problem, run \"%s --continue\".\n"), cmdline);
		strbuf_addf(&sb, _("If you prefer to skip this patch, run \"%s --skip\" instead.\n"), cmdline);

		जइ (advice_enabled(ADVICE_AM_WORK_DIR) &&
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

	strbuf_attach(&sb, state->सन्देशः, state->सन्देशः_len, state->सन्देशः_len + 1);
	append_signoff(&sb, 0, 0);
	state->सन्देशः = strbuf_detach(&sb, &state->सन्देशः_len);
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
स्थिर पूर्णाङ्क parse_mail(संरचना am_state *state, ध्रुव वर्ण *mail)
{
	FILE *fp;
	संरचना strbuf sb = STRBUF_INIT;
	संरचना strbuf सन्देशः = STRBUF_INIT;
	संरचना strbuf author_name = STRBUF_INIT;
	संरचना strbuf author_date = STRBUF_INIT;
	संरचना strbuf author_email = STRBUF_INIT;
	पूर्णाङ्क ret = 0;
	संरचना mailinfo mi;

	setup_mailinfo(the_repository, &mi);

	जइ (state->utf8)
		mi.metainfo_charset = get_commit_output_encoding();
	अण्णहा
		mi.metainfo_charset = रिक्त;

	विकल्प (state->keep) {
	पक्ष KEEP_FALSE:
		भङ्ग;
	पक्ष KEEP_TRUE:
		mi.keep_subject = 1;
		भङ्ग;
	पक्ष KEEP_NON_PATCH:
		mi.keep_non_patch_brackets_in_subject = 1;
		भङ्ग;
	स्वभाव:
		BUG("invalid value for state->keep");
	}

	जइ (state->message_id)
		mi.add_message_id = 1;

	विकल्प (state->scissors) {
	पक्ष SCISSORS_UNSET:
		भङ्ग;
	पक्ष SCISSORS_FALSE:
		mi.use_scissors = 0;
		भङ्ग;
	पक्ष SCISSORS_TRUE:
		mi.use_scissors = 1;
		भङ्ग;
	स्वभाव:
		BUG("invalid value for state->scissors");
	}

	विकल्प (state->quoted_cr) {
	पक्ष quoted_cr_unset:
		भङ्ग;
	पक्ष quoted_cr_nowarn:
	पक्ष quoted_cr_warn:
	पक्ष quoted_cr_strip:
		mi.quoted_cr = state->quoted_cr;
		भङ्ग;
	स्वभाव:
		BUG("invalid value for state->quoted_cr");
	}

	mi.निवेशः = xfopen(mail, "r");
	mi.output = xfopen(am_path(state, "info"), "w");
	जइ (mailinfo(&mi, am_path(state, "msg"), am_path(state, "patch")))
		die("could not parse patch");

	fclose(mi.निवेशः);
	fclose(mi.output);

	जइ (mi.format_flowed)
		warning(_("Patch sent with format=flowed; "
			  "space at the end of lines might be lost."));

	/* Extract message and author information */
	fp = xfopen(am_path(state, "info"), "r");
	जाव (!strbuf_getline_lf(&sb, fp)) {
		ध्रुव वर्ण *x;

		जइ (skip_prefix(sb.buf, "Subject: ", &x)) {
			जइ (सन्देशः.len)
				strbuf_addch(&सन्देशः, '\n');
			strbuf_addstr(&सन्देशः, x);
		} अण्णहा जइ (skip_prefix(sb.buf, "Author: ", &x))
			strbuf_addstr(&author_name, x);
		अण्णहा जइ (skip_prefix(sb.buf, "Email: ", &x))
			strbuf_addstr(&author_email, x);
		अण्णहा जइ (skip_prefix(sb.buf, "Date: ", &x))
			strbuf_addstr(&author_date, x);
	}
	fclose(fp);

	/* Skip pine's internal folder data */
	जइ (!strcmp(author_name.buf, "Mail System Internal Data")) {
		ret = 1;
		गच्छ finish;
	}

	strbuf_addstr(&सन्देशः, "\n\n");
	strbuf_addbuf(&सन्देशः, &mi.log_message);
	strbuf_stripspace(&सन्देशः, रिक्त);

	प्रतिज्ञा(!state->author_name);
	state->author_name = strbuf_detach(&author_name, रिक्त);

	प्रतिज्ञा(!state->author_email);
	state->author_email = strbuf_detach(&author_email, रिक्त);

	प्रतिज्ञा(!state->author_date);
	state->author_date = strbuf_detach(&author_date, रिक्त);

	प्रतिज्ञा(!state->सन्देशः);
	state->सन्देशः = strbuf_detach(&सन्देशः, &state->सन्देशः_len);

finish:
	strbuf_release(&सन्देशः);
	strbuf_release(&author_date);
	strbuf_release(&author_email);
	strbuf_release(&author_name);
	strbuf_release(&sb);
	clear_mailinfo(&mi);
	प्रत्यागम ret;
}

/**
 * Sets commit_id to the commit hash where the mail was generated from.
 * Returns 0 on success, -1 on failure.
 */
स्थिर पूर्णाङ्क get_mail_commit_oid(संरचना object_id *commit_id, ध्रुव वर्ण *mail)
{
	संरचना strbuf sb = STRBUF_INIT;
	FILE *fp = xfopen(mail, "r");
	ध्रुव वर्ण *x;
	पूर्णाङ्क ret = 0;

	जइ (strbuf_getline_lf(&sb, fp) ||
	    !skip_prefix(sb.buf, "From ", &x) ||
	    get_oid_hex(x, commit_id) < 0)
		ret = -1;

	strbuf_release(&sb);
	fclose(fp);
	प्रत्यागम ret;
}

/**
 * Sets state->msg, state->author_name, state->author_email, state->author_date
 * to the commit's respective info.
 */
स्थिर शून्य get_commit_info(संरचना am_state *state, संरचना commit *commit)
{
	ध्रुव वर्ण *buffer, *ident_पङ्क्तिः, *सन्देशः;
	size_t ident_len;
	संरचना ident_split id;

	buffer = repo_logmsg_reencode(the_repository, commit, रिक्त,
				      get_commit_output_encoding());

	ident_पङ्क्तिः = find_commit_header(buffer, "author", &ident_len);
	जइ (!ident_पङ्क्तिः)
		die(_("missing author line in commit %s"),
		      oid_to_hex(&commit->object.oid));
	जइ (split_ident_पङ्क्तिः(&id, ident_पङ्क्तिः, ident_len) < 0)
		die(_("invalid ident line: %.*s"), (पूर्णाङ्क)ident_len, ident_पङ्क्तिः);

	प्रतिज्ञा(!state->author_name);
	जइ (id.name_begin)
		state->author_name =
			xmemdupz(id.name_begin, id.name_end - id.name_begin);
	अण्णहा
		state->author_name = xstrdup("");

	प्रतिज्ञा(!state->author_email);
	जइ (id.mail_begin)
		state->author_email =
			xmemdupz(id.mail_begin, id.mail_end - id.mail_begin);
	अण्णहा
		state->author_email = xstrdup("");

	प्रतिज्ञा(!state->author_date);
	state->author_date = xstrdup(show_ident_date(&id, DATE_MODE(NORMAL)));

	प्रतिज्ञा(!state->सन्देशः);
	सन्देशः = strstr(buffer, "\n\n");
	जइ (!सन्देशः)
		die(_("unable to parse commit %s"), oid_to_hex(&commit->object.oid));
	state->सन्देशः = xstrdup(सन्देशः + 2);
	state->सन्देशः_len = strlen(state->सन्देशः);
	repo_unuse_commit_buffer(the_repository, commit, buffer);
}

/**
 * Writes `commit` as a patch to the state directory's "patch" file.
 */
स्थिर शून्य write_commit_patch(ध्रुव संरचना am_state *state, संरचना commit *commit)
{
	संरचना rev_info rev_info;
	FILE *fp;

	fp = xfopen(am_path(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, रिक्त);
	rev_info.diff = 1;
	rev_info.abbrev = 0;
	rev_info.disable_stdin = 1;
	rev_info.show_मूलम्_diff = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.flags.द्विमान = 1;
	rev_info.diffopt.flags.full_index = 1;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	add_pending_object(&rev_info, &commit->object, "");
	diff_setup_done(&rev_info.diffopt);
	log_वृक्षः_commit(&rev_info, commit);
	release_revisions(&rev_info);
}

/**
 * Writes the diff of the index against HEAD as a patch to the state
 * directory's "patch" file.
 */
स्थिर शून्य write_index_patch(ध्रुव संरचना am_state *state)
{
	संरचना वृक्षः *वृक्षः;
	संरचना object_id head;
	संरचना rev_info rev_info;
	FILE *fp;

	जइ (!repo_get_oid(the_repository, "HEAD", &head)) {
		संरचना commit *commit = lookup_commit_or_die(&head, "HEAD");
		वृक्षः = repo_get_commit_वृक्षः(the_repository, commit);
	} अण्णहा
		वृक्षः = lookup_वृक्षः(the_repository,
				   the_repository->hash_algo->empty_वृक्षः);

	fp = xfopen(am_path(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, रिक्त);
	rev_info.diff = 1;
	rev_info.disable_stdin = 1;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	add_pending_object(&rev_info, &वृक्षः->object, "");
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
स्थिर पूर्णाङ्क parse_mail_rebase(संरचना am_state *state, ध्रुव वर्ण *mail)
{
	संरचना commit *commit;
	संरचना object_id commit_oid;

	जइ (get_mail_commit_oid(&commit_oid, mail) < 0)
		die(_("could not parse %s"), mail);

	commit = lookup_commit_or_die(&commit_oid, mail);

	get_commit_info(state, commit);

	write_commit_patch(state, commit);

	oidcpy(&state->orig_commit, &commit_oid);
	write_state_text(state, "original-commit", oid_to_hex(&commit_oid));
	refs_update_ref(get_main_ref_store(the_repository), "am",
			"REBASE_HEAD", &commit_oid,
			रिक्त, REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);

	प्रत्यागम 0;
}

/**
 * Applies current patch with git-apply. Returns 0 on success, -1 otherwise. If
 * `index_file` is not NULL, the patch will be applied to that index.
 */
स्थिर पूर्णाङ्क run_apply(ध्रुव संरचना am_state *state, ध्रुव वर्ण *index_file)
{
	संरचना strvec apply_paths = STRVEC_INIT;
	संरचना strvec apply_opts = STRVEC_INIT;
	संरचना apply_state apply_state;
	पूर्णाङ्क res, opts_वामः;
	पूर्णाङ्क force_apply = 0;
	पूर्णाङ्क options = 0;
	ध्रुव वर्ण **apply_argv;

	जइ (init_apply_state(&apply_state, the_repository, रिक्त))
		BUG("init_apply_state() failed");

	strvec_प्रक्षिप(&apply_opts, "apply");
	strvec_pushv(&apply_opts, state->git_apply_opts.v);

	/*
	 * Build a copy that apply_parse_options() can rearrange.
	 * apply_opts.v keeps referencing the allocated strings for
	 * strvec_clear() to release.
	 */
	DUP_ARRAY(apply_argv, apply_opts.v, apply_opts.nr);

	opts_वामः = apply_parse_options(apply_opts.nr, apply_argv,
					&apply_state, &force_apply, &options,
					रिक्त);

	जइ (opts_वामः != 0)
		die("unknown option passed through to git apply");

	जइ (index_file) {
		apply_state.index_file = index_file;
		apply_state.cached = 1;
	} अण्णहा
		apply_state.check_index = 1;

	/*
	 * If we are allowed to fall back on 3-way merge, don't give false
	 * errors during the initial attempt.
	 */
	जइ (state->threeway && !index_file)
		apply_state.apply_verbosity = verbosity_silent;

	जइ (check_apply_state(&apply_state, force_apply))
		BUG("check_apply_state() failed");

	strvec_प्रक्षिप(&apply_paths, am_path(state, "patch"));

	res = apply_all_patches(&apply_state, apply_paths.nr, apply_paths.v, options);

	strvec_clear(&apply_paths);
	strvec_clear(&apply_opts);
	clear_apply_state(&apply_state);
	मुक्त(apply_argv);

	जइ (res)
		प्रत्यागम res;

	जइ (index_file) {
		/* Reload index as apply_all_patches() will have modified it. */
		discard_index(the_repository->index);
		read_index_from(the_repository->index, index_file,
				repo_get_git_dir(the_repository));
	}

	प्रत्यागम 0;
}

/**
 * Builds an index that contains just the blobs needed for a 3way merge.
 */
स्थिर पूर्णाङ्क build_fake_ancestor(ध्रुव संरचना am_state *state, ध्रुव वर्ण *index_file)
{
	संरचना child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	strvec_प्रक्षिप(&cp.args, "apply");
	strvec_pushv(&cp.args, state->git_apply_opts.v);
	strvec_pushf(&cp.args, "--build-fake-ancestor=%s", index_file);
	strvec_प्रक्षिप(&cp.args, am_path(state, "patch"));

	जइ (run_command(&cp))
		प्रत्यागम -1;

	प्रत्यागम 0;
}

/**
 * Attempt a threeway merge, using index_path as the temporary index.
 */
स्थिर पूर्णाङ्क fall_back_threeway(ध्रुव संरचना am_state *state, ध्रुव वर्ण *index_path)
{
	संरचना object_id their_वृक्षः, our_वृक्षः;
	संरचना object_id bases[1] = { 0 };
	संरचना merge_options o;
	संरचना commit *result;
	वर्ण *their_वृक्षः_name;

	जइ (repo_get_oid(the_repository, "HEAD", &our_वृक्षः) < 0)
		oidcpy(&our_वृक्षः, the_hash_algo->empty_वृक्षः);

	जइ (build_fake_ancestor(state, index_path))
		प्रत्यागम error("could not build fake ancestor");

	discard_index(the_repository->index);
	read_index_from(the_repository->index, index_path, repo_get_git_dir(the_repository));

	जइ (write_index_as_वृक्षः(&bases[0], the_repository->index, index_path, 0, रिक्त))
		प्रत्यागम error(_("Repository lacks necessary blobs to fall back on 3-way merge."));

	say(state, stdout, _("Using index info to reconstruct a base tree..."));

	जइ (!state->quiet) {
		/*
		 * List paths that needed 3-way fallback, so that the user can
		 * review them with extra care to spot mismerges.
		 */
		संरचना rev_info rev_info;

		repo_init_revisions(the_repository, &rev_info, रिक्त);
		rev_info.diffopt.output_format = DIFF_FORMAT_NAME_STATUS;
		rev_info.diffopt.filter |= diff_filter_bit('A');
		rev_info.diffopt.filter |= diff_filter_bit('M');
		add_pending_oid(&rev_info, "HEAD", &our_वृक्षः, 0);
		diff_setup_done(&rev_info.diffopt);
		run_diff_index(&rev_info, DIFF_INDEX_CACHED);
		release_revisions(&rev_info);
	}

	जइ (run_apply(state, index_path))
		प्रत्यागम error(_("Did you hand edit your patch?\n"
				"It does not apply to blobs recorded in its index."));

	जइ (write_index_as_वृक्षः(&their_वृक्षः, the_repository->index, index_path, 0, रिक्त))
		प्रत्यागम error("could not write tree");

	say(state, stdout, _("Falling back to patching base and 3-way merge..."));

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
	their_वृक्षः_name = xstrfmt("%.*s", linelen(state->सन्देशः), state->सन्देशः);
	o.branch2 = their_वृक्षः_name;
	o.ancestor = "constructed fake ancestor";
	o.detect_directory_renames = MERGE_DIRECTORY_RENAMES_NONE;

	जइ (state->quiet)
		o.verbosity = 0;

	जइ (merge_ort_generic(&o, &our_वृक्षः, &their_वृक्षः, 1, bases, &result)) {
		repo_rerere(the_repository, state->allow_rerere_autoupdate);
		मुक्त(their_वृक्षः_name);
		प्रत्यागम error(_("Failed to merge in the changes."));
	}

	मुक्त(their_वृक्षः_name);
	प्रत्यागम 0;
}

/**
 * Commits the current index with state->msg as the commit message and
 * state->author_name, state->author_email and state->author_date as the author
 * information.
 */
स्थिर शून्य do_commit(ध्रुव संरचना am_state *state)
{
	संरचना object_id वृक्षः, parent, commit;
	ध्रुव संरचना object_id *old_oid;
	संरचना commit_list *parents = रिक्त;
	ध्रुव वर्ण *reflog_सन्देशः, *author, *committer = रिक्त;
	संरचना strbuf sb = STRBUF_INIT;

	जइ (!state->no_verify && run_hooks(the_repository, "pre-applypatch"))
		exit(1);

	जइ (write_index_as_वृक्षः(&वृक्षः, the_repository->index,
				repo_get_index_file(the_repository),
				0, रिक्त))
		die(_("git write-tree failed to write a tree"));

	जइ (!repo_get_oid_commit(the_repository, "HEAD", &parent)) {
		old_oid = &parent;
		commit_list_निवेशय(lookup_commit(the_repository, &parent),
				   &parents);
	} अण्णहा {
		old_oid = रिक्त;
		say(state, stderr, _("applying to an empty history"));
	}

	author = fmt_ident(state->author_name, state->author_email,
		WANT_AUTHOR_IDENT,
			state->ignore_date ? रिक्त : state->author_date,
			IDENT_STRICT);

	जइ (state->committer_date_is_author_date)
		committer = fmt_ident(getenv("GIT_COMMITTER_NAME"),
				      getenv("GIT_COMMITTER_EMAIL"),
				      WANT_COMMITTER_IDENT,
				      state->ignore_date ? रिक्त
							 : state->author_date,
				      IDENT_STRICT);

	जइ (commit_वृक्षः_extended(state->सन्देशः, state->सन्देशः_len, &वृक्षः, parents,
				 &commit, author, committer, state->sign_commit,
				 रिक्त))
		die(_("failed to write commit object"));

	reflog_सन्देशः = getenv("GIT_REFLOG_ACTION");
	जइ (!reflog_सन्देशः)
		reflog_सन्देशः = "am";

	strbuf_addf(&sb, "%s: %.*s", reflog_सन्देशः, linelen(state->सन्देशः),
			state->सन्देशः);

	refs_update_ref(get_main_ref_store(the_repository), sb.buf, "HEAD",
			&commit, old_oid, 0,
			UPDATE_REFS_DIE_ON_ERR);

	जइ (state->rebasing) {
		FILE *fp = xfopen(am_path(state, "rewritten"), "a");

		प्रतिज्ञा(!is_null_oid(&state->orig_commit));
		fprintf(fp, "%s ", oid_to_hex(&state->orig_commit));
		fprintf(fp, "%s\n", oid_to_hex(&commit));
		fclose(fp);
	}

	run_hooks(the_repository, "post-applypatch");

	commit_list_free(parents);
	strbuf_release(&sb);
}

/**
 * Validates the am_state for resuming -- the "msg" and authorship fields must
 * be filled up.
 */
स्थिर शून्य validate_resume_state(ध्रुव संरचना am_state *state)
{
	जइ (!state->सन्देशः)
		die(_("cannot resume: %s does not exist."),
			am_path(state, "final-commit"));

	जइ (!state->author_name || !state->author_email || !state->author_date)
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
स्थिर पूर्णाङ्क do_interactive(संरचना am_state *state)
{
	प्रतिज्ञा(state->सन्देशः);

	प्रति (;;) {
		वर्ण reply[64];

		puts(_("Commit Body is:"));
		puts("--------------------------");
		printf("%s", state->सन्देशः);
		puts("--------------------------");

		/*
		 * TRANSLATORS: Make sure to include [y], [n], [e], [v] and [a]
		 * in your translation. The program will only accept English
		 * input at this point.
		 */
		printf(_("Apply? [y]es/[n]o/[e]dit/[v]iew patch/[a]ccept all: "));
		जइ (!fgets(reply, परिमाण(reply), stdin))
			die("unable to read from stdin; aborting");

		जइ (*reply == 'y' || *reply == 'Y') {
			प्रत्यागम 0;
		} अण्णहा जइ (*reply == 'a' || *reply == 'A') {
			state->interactive = 0;
			प्रत्यागम 0;
		} अण्णहा जइ (*reply == 'n' || *reply == 'N') {
			प्रत्यागम 1;
		} अण्णहा जइ (*reply == 'e' || *reply == 'E') {
			संरचना strbuf सन्देशः = STRBUF_INIT;

			जइ (!launch_editor(am_path(state, "final-commit"), &सन्देशः, रिक्त)) {
				मुक्त(state->सन्देशः);
				state->सन्देशः = strbuf_detach(&सन्देशः, &state->सन्देशः_len);
			}
			strbuf_release(&सन्देशः);
		} अण्णहा जइ (*reply == 'v' || *reply == 'V') {
			ध्रुव वर्ण *pager = git_pager(the_repository, 1);
			संरचना child_process cp = CHILD_PROCESS_INIT;

			जइ (!pager)
				pager = "cat";
			prepare_pager_args(&cp, pager);
			strvec_प्रक्षिप(&cp.args, am_path(state, "patch"));
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
स्थिर शून्य am_run(संरचना am_state *state, पूर्णाङ्क resume)
{
	संरचना strbuf sb = STRBUF_INIT;

	unlink(am_path(state, "dirtyindex"));

	जइ (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 रिक्त, रिक्त, रिक्त) < 0)
		die(_("unable to write index file"));

	जइ (repo_index_has_changes(the_repository, रिक्त, &sb)) {
		write_state_bool(state, "dirtyindex", 1);
		die(_("Dirty index: cannot apply patches (dirty: %s)"), sb.buf);
	}

	strbuf_release(&sb);

	जाव (state->cur <= state->last) {
		ध्रुव वर्ण *mail = am_path(state, msgnum(state));
		पूर्णाङ्क apply_status;
		पूर्णाङ्क to_keep;

		reset_ident_date();

		जइ (!file_exists(mail))
			गच्छ next;

		जइ (resume) {
			validate_resume_state(state);
		} अण्णहा {
			पूर्णाङ्क skip;

			जइ (state->rebasing)
				skip = parse_mail_rebase(state, mail);
			अण्णहा
				skip = parse_mail(state, mail);

			जइ (skip)
				गच्छ next; /* mail should be skipped */

			जइ (state->signoff)
				am_append_signoff(state);

			write_author_script(state);
			write_commit_सन्देशः(state);
		}

		जइ (state->interactive && do_interactive(state))
			गच्छ next;

		to_keep = 0;
		जइ (is_empty_or_missing_file(am_path(state, "patch"))) {
			विकल्प (state->empty_type) {
			पक्ष DROP_EMPTY_COMMIT:
				say(state, stdout, _("Skipping: %.*s"), linelen(state->सन्देशः), state->सन्देशः);
				गच्छ next;
				भङ्ग;
			पक्ष KEEP_EMPTY_COMMIT:
				to_keep = 1;
				say(state, stdout, _("Creating an empty commit: %.*s"),
					linelen(state->सन्देशः), state->सन्देशः);
				भङ्ग;
			पक्ष STOP_ON_EMPTY_COMMIT:
				printf_ln(_("Patch is empty."));
				die_user_resolve(state);
				भङ्ग;
			}
		}

		जइ (run_applypatch_सन्देशः_hook(state))
			exit(1);
		जइ (to_keep)
			गच्छ commit;

		say(state, stdout, _("Applying: %.*s"), linelen(state->सन्देशः), state->सन्देशः);

		apply_status = run_apply(state, रिक्त);

		जइ (apply_status && state->threeway) {
			संरचना strbuf sb = STRBUF_INIT;

			strbuf_addstr(&sb, am_path(state, "patch-merge-index"));
			apply_status = fall_back_threeway(state, sb.buf);
			strbuf_release(&sb);

			/*
			 * Applying the patch to an earlier tree and merging
			 * the result may have produced the same tree as ours.
			 */
			जइ (!apply_status &&
			    !repo_index_has_changes(the_repository, रिक्त, रिक्त)) {
				say(state, stdout, _("No changes -- Patch already applied."));
				गच्छ next;
			}
		}

		जइ (apply_status) {
			printf_ln(_("Patch failed at %s %.*s"), msgnum(state),
				linelen(state->सन्देशः), state->सन्देशः);

			जइ (advice_enabled(ADVICE_AM_WORK_DIR))
				advise(_("Use 'git am --show-current-patch=diff' to see the failed patch"));

			die_user_resolve(state);
		}

commit:
		do_commit(state);

next:
		am_next(state);

		जइ (resume)
			am_load(state);
		resume = 0;
	}

	जइ (!is_empty_or_missing_file(am_path(state, "rewritten"))) {
		प्रतिज्ञा(state->rebasing);
		copy_notes_for_rebase(state);
		run_post_rewrite_hook(state);
	}

	/*
	 * In rebasing mode, it's up to the caller to take care of
	 * housekeeping.
	 */
	जइ (!state->rebasing) {
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
स्थिर शून्य am_resolve(संरचना am_state *state, पूर्णाङ्क allow_empty)
{
	validate_resume_state(state);

	say(state, stdout, _("Applying: %.*s"), linelen(state->सन्देशः), state->सन्देशः);

	जइ (!repo_index_has_changes(the_repository, रिक्त, रिक्त)) {
		जइ (allow_empty && is_empty_or_missing_file(am_path(state, "patch"))) {
			printf_ln(_("No changes - recorded it as an empty commit."));
		} अण्णहा {
			printf_ln(_("No changes - did you forget to use 'git add'?\n"
				    "If there is nothing left to stage, chances are that something else\n"
				    "already introduced the same changes; you might want to skip this patch."));
			die_user_resolve(state);
		}
	}

	जइ (unmerged_index(the_repository->index)) {
		printf_ln(_("You still have unmerged paths in your index.\n"
			"You should 'git add' each file with resolved conflicts to mark them as such.\n"
			"You might run `git rm` on a file to accept \"deleted by them\" for it."));
		die_user_resolve(state);
	}

	जइ (state->interactive) {
		write_index_patch(state);
		जइ (do_interactive(state))
			गच्छ next;
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
स्थिर पूर्णाङ्क fast_forward_to(संरचना वृक्षः *head, संरचना वृक्षः *remote, पूर्णाङ्क reset)
{
	संरचना lock_file lock_file = LOCK_INIT;
	संरचना unpack_trees_options opts;
	संरचना वृक्षः_desc t[2];

	जइ (repo_parse_वृक्षः(the_repository, head) || repo_parse_वृक्षः(the_repository, remote))
		प्रत्यागम -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	refresh_index(the_repository->index, REFRESH_QUIET, रिक्त, रिक्त, रिक्त);

	memset(&opts, 0, परिमाण(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.update = 1;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = twoway_merge;
	init_वृक्षः_desc(&t[0], &head->object.oid, head->buffer, head->size);
	init_वृक्षः_desc(&t[1], &remote->object.oid, remote->buffer, remote->size);

	जइ (unpack_trees(2, t, &opts)) {
		rollback_lock_file(&lock_file);
		प्रत्यागम -1;
	}

	जइ (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	प्रत्यागम 0;
}

/**
 * Merges a tree into the index. The index's stat info will take precedence
 * over the merged tree's. Returns 0 on success, -1 on failure.
 */
स्थिर पूर्णाङ्क merge_वृक्षः(संरचना वृक्षः *वृक्षः)
{
	संरचना lock_file lock_file = LOCK_INIT;
	संरचना unpack_trees_options opts;
	संरचना वृक्षः_desc t[1];

	जइ (repo_parse_वृक्षः(the_repository, वृक्षः))
		प्रत्यागम -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, परिमाण(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.merge = 1;
	opts.fn = oneway_merge;
	init_वृक्षः_desc(&t[0], &वृक्षः->object.oid, वृक्षः->buffer, वृक्षः->size);

	जइ (unpack_trees(1, t, &opts)) {
		rollback_lock_file(&lock_file);
		प्रत्यागम -1;
	}

	जइ (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	प्रत्यागम 0;
}

/**
 * Clean the index without touching entries that are not modified between
 * `head` and `remote`.
 */
स्थिर पूर्णाङ्क clean_index(ध्रुव संरचना object_id *head, ध्रुव संरचना object_id *remote)
{
	संरचना वृक्षः *head_वृक्षः, *remote_वृक्षः, *index_वृक्षः;
	संरचना object_id index;

	head_वृक्षः = repo_parse_वृक्षः_indirect(the_repository, head);
	जइ (!head_वृक्षः)
		प्रत्यागम error(_("Could not parse object '%s'."), oid_to_hex(head));

	remote_वृक्षः = repo_parse_वृक्षः_indirect(the_repository, remote);
	जइ (!remote_वृक्षः)
		प्रत्यागम error(_("Could not parse object '%s'."), oid_to_hex(remote));

	repo_read_index_unmerged(the_repository);

	जइ (fast_forward_to(head_वृक्षः, head_वृक्षः, 1))
		प्रत्यागम -1;

	जइ (write_index_as_वृक्षः(&index, the_repository->index,
				repo_get_index_file(the_repository),
				0, रिक्त))
		प्रत्यागम -1;

	index_वृक्षः = repo_parse_वृक्षः_indirect(the_repository, &index);
	जइ (!index_वृक्षः)
		प्रत्यागम error(_("Could not parse object '%s'."), oid_to_hex(&index));

	जइ (fast_forward_to(index_वृक्षः, remote_वृक्षः, 0))
		प्रत्यागम -1;

	जइ (merge_वृक्षः(remote_वृक्षः))
		प्रत्यागम -1;

	remove_branch_state(the_repository, 0);

	प्रत्यागम 0;
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

	जइ (repo_get_oid(the_repository, "HEAD", &head))
		oidcpy(&head, the_hash_algo->empty_वृक्षः);

	जइ (clean_index(&head, &head))
		die(_("failed to clean index"));

	जइ (state->rebasing) {
		FILE *fp = xfopen(am_path(state, "rewritten"), "a");

		प्रतिज्ञा(!is_null_oid(&state->orig_commit));
		fprintf(fp, "%s ", oid_to_hex(&state->orig_commit));
		fprintf(fp, "%s\n", oid_to_hex(&head));
		fclose(fp);
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
स्थिर पूर्णाङ्क safe_to_abort(ध्रुव संरचना am_state *state)
{
	संरचना strbuf sb = STRBUF_INIT;
	संरचना object_id abort_safety, head;

	जइ (file_exists(am_path(state, "dirtyindex")))
		प्रत्यागम 0;

	जइ (read_state_file(&sb, state, "abort-safety", 1) > 0) {
		जइ (get_oid_hex(sb.buf, &abort_safety))
			die(_("could not parse %s"), am_path(state, "abort-safety"));
	} अण्णहा
		oidclr(&abort_safety, the_repository->hash_algo);
	strbuf_release(&sb);

	जइ (repo_get_oid(the_repository, "HEAD", &head))
		oidclr(&head, the_repository->hash_algo);

	जइ (oideq(&head, &abort_safety))
		प्रत्यागम 1;

	warning(_("You seem to have moved HEAD since the last 'am' failure.\n"
		"Not rewinding to ORIG_HEAD"));

	प्रत्यागम 0;
}

/**
 * Aborts the current am session if it is safe to do so.
 */
स्थिर शून्य am_abort(संरचना am_state *state)
{
	संरचना object_id curr_head, orig_head;
	पूर्णाङ्क has_curr_head, has_orig_head;
	वर्ण *curr_branch;

	जइ (!safe_to_abort(state)) {
		am_destroy(state);
		प्रत्यागम;
	}

	am_rerere_clear();

	curr_branch = refs_resolve_refdup(get_main_ref_store(the_repository),
					  "HEAD", 0, &curr_head, रिक्त);
	has_curr_head = curr_branch && !is_null_oid(&curr_head);
	जइ (!has_curr_head)
		oidcpy(&curr_head, the_hash_algo->empty_वृक्षः);

	has_orig_head = !repo_get_oid(the_repository, "ORIG_HEAD", &orig_head);
	जइ (!has_orig_head)
		oidcpy(&orig_head, the_hash_algo->empty_वृक्षः);

	जइ (clean_index(&curr_head, &orig_head))
		die(_("failed to clean index"));

	जइ (has_orig_head)
		refs_update_ref(get_main_ref_store(the_repository),
				"am --abort", "HEAD", &orig_head,
				has_curr_head ? &curr_head : रिक्त, 0,
				UPDATE_REFS_DIE_ON_ERR);
	अण्णहा जइ (curr_branch)
		refs_delete_ref(get_main_ref_store(the_repository), रिक्त,
				curr_branch, रिक्त, REF_NO_DEREF);

	मुक्त(curr_branch);
	am_destroy(state);
}

स्थिर पूर्णाङ्क show_patch(संरचना am_state *state, गणना resume_type resume_mode)
{
	संरचना strbuf sb = STRBUF_INIT;
	ध्रुव वर्ण *patch_path;
	पूर्णाङ्क len;

	जइ (!is_null_oid(&state->orig_commit)) {
		संरचना child_process cmd = CHILD_PROCESS_INIT;

		strvec_pushl(&cmd.args, "show", oid_to_hex(&state->orig_commit),
			     "--", रिक्त);
		cmd.git_cmd = 1;
		प्रत्यागम run_command(&cmd);
	}

	विकल्प (resume_mode) {
	पक्ष RESUME_SHOW_PATCH_RAW:
		patch_path = am_path(state, msgnum(state));
		भङ्ग;
	पक्ष RESUME_SHOW_PATCH_DIFF:
		patch_path = am_path(state, "patch");
		भङ्ग;
	स्वभाव:
		BUG("invalid mode for --show-current-patch");
	}

	len = strbuf_read_file(&sb, patch_path, 0);
	जइ (len < 0)
		die_errno(_("failed to read '%s'"), patch_path);

	setup_pager(the_repository);
	write_in_full(1, sb.buf, sb.len);
	strbuf_release(&sb);
	प्रत्यागम 0;
}

/**
 * parse_options() callback that validates and sets opt->value to the
 * PATCH_FORMAT_* enum value corresponding to `arg`.
 */
स्थिर पूर्णाङ्क parse_opt_patchformat(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *opt_मानम् = opt->मानम्;

	जइ (unset)
		*opt_मानम् = PATCH_FORMAT_UNKNOWN;
	अण्णहा जइ (!strcmp(arg, "mbox"))
		*opt_मानम् = PATCH_FORMAT_MBOX;
	अण्णहा जइ (!strcmp(arg, "stgit"))
		*opt_मानम् = PATCH_FORMAT_STGIT;
	अण्णहा जइ (!strcmp(arg, "stgit-series"))
		*opt_मानम् = PATCH_FORMAT_STGIT_SERIES;
	अण्णहा जइ (!strcmp(arg, "hg"))
		*opt_मानम् = PATCH_FORMAT_HG;
	अण्णहा जइ (!strcmp(arg, "mboxrd"))
		*opt_मानम् = PATCH_FORMAT_MBOXRD;
	/*
	 * Please update $__git_patchformat in git-completion.bash
	 * when you add new options
	 */
	अण्णहा
		प्रत्यागम error(_("invalid value for '%s': '%s'"),
			     "--patch-format", arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क parse_opt_show_current_patch(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *opt_मानम् = opt->मानम्;

	BUG_ON_OPT_NEG(unset);

	जइ (!arg)
		*opt_मानम् = opt->defval;
	अण्णहा जइ (!strcmp(arg, "raw"))
		*opt_मानम् = RESUME_SHOW_PATCH_RAW;
	अण्णहा जइ (!strcmp(arg, "diff"))
		*opt_मानम् = RESUME_SHOW_PATCH_DIFF;
	/*
	 * Please update $__git_showcurrentpatch in git-completion.bash
	 * when you add new options
	 */
	अण्णहा
		प्रत्यागम error(_("invalid value for '%s': '%s'"),
			     "--show-current-patch", arg);
	प्रत्यागम 0;
}

पूर्णाङ्क cmd_am(पूर्णाङ्क argc,
	   ध्रुव वर्ण **argv,
	   ध्रुव वर्ण *prefix,
	   संरचना repository *repo UNUSED)
{
	संरचना am_state state;
	पूर्णाङ्क द्विमान = -1;
	पूर्णाङ्क keep_cr = -1;
	पूर्णाङ्क patch_format = PATCH_FORMAT_UNKNOWN;
	गणना resume_type resume_mode = RESUME_FALSE;
	पूर्णाङ्क in_progress;
	पूर्णाङ्क ret = 0;

	ध्रुव वर्ण * ध्रुव usage[] = {
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
			.मानम् = &resume_mode,
			.precision = परिमाण(resume_mode),
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
			.मानम् = &state.sign_commit,
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
	जइ (in_progress)
		am_load(&state);

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	जइ (द्विमान >= 0)
		fprintf_ln(stderr, _("The -b/--binary option has been a no-op for long time, and\n"
				"it will be removed. Please do not use it anymore."));

	/* Ensure a valid committer ident can be constructed */
	git_committer_info(IDENT_STRICT);

	जइ (repo_read_index_preload(the_repository, रिक्त, 0) < 0)
		die(_("failed to read the index"));

	जइ (in_progress) {
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
		जइ (argc || (resume_mode == RESUME_FALSE && !isatty(0)))
			die(_("previous rebase directory %s still exists but mbox given."),
				state.dir);

		जइ (resume_mode == RESUME_FALSE)
			resume_mode = RESUME_APPLY;

		जइ (state.signoff == SIGNOFF_EXPLICIT)
			am_append_signoff(&state);
	} अण्णहा {
		संरचना strvec paths = STRVEC_INIT;
		पूर्णाङ्क i;

		/*
		 * Handle stray state directory in the independent-run case. In
		 * the --rebasing case, it is up to the caller to take care of
		 * stray directories.
		 */
		जइ (file_exists(state.dir) && !state.rebasing) {
			जइ (resume_mode == RESUME_ABORT || resume_mode == RESUME_QUIT) {
				am_destroy(&state);
				am_state_release(&state);
				प्रत्यागम 0;
			}

			die(_("Stray %s directory found.\n"
				"Use \"git am --abort\" to remove it."),
				state.dir);
		}

		जइ (resume_mode)
			die(_("Resolve operation not in progress, we are not resuming."));

		प्रति (i = 0; i < argc; i++) {
			जइ (is_absolute_path(argv[i]) || !prefix)
				strvec_प्रक्षिप(&paths, argv[i]);
			अण्णहा
				strvec_प्रक्षिप(&paths, mkpath("%s/%s", prefix, argv[i]));
		}

		जइ (state.interactive && !paths.nr)
			die(_("interactive mode requires patches on the command line"));

		am_setup(&state, patch_format, paths.v, keep_cr);

		strvec_clear(&paths);
	}

	विकल्प (resume_mode) {
	पक्ष RESUME_FALSE:
		am_run(&state, 0);
		भङ्ग;
	पक्ष RESUME_APPLY:
		am_run(&state, 1);
		भङ्ग;
	पक्ष RESUME_RESOLVED:
	पक्ष RESUME_ALLOW_EMPTY:
		am_resolve(&state, resume_mode == RESUME_ALLOW_EMPTY ? 1 : 0);
		भङ्ग;
	पक्ष RESUME_SKIP:
		am_skip(&state);
		भङ्ग;
	पक्ष RESUME_ABORT:
		am_abort(&state);
		भङ्ग;
	पक्ष RESUME_QUIT:
		am_rerere_clear();
		am_destroy(&state);
		भङ्ग;
	पक्ष RESUME_SHOW_PATCH_RAW:
	पक्ष RESUME_SHOW_PATCH_DIFF:
		ret = show_patch(&state, resume_mode);
		भङ्ग;
	स्वभाव:
		BUG("invalid resume value");
	}

	am_state_release(&state);

	प्रत्यागम ret;
}
