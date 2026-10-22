/*
 * Builtin "git am"
 *
 * Based on git-am.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "abspath.h"
#അന്തര്ഭാവ "advice.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "editor.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "hook.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "tempfile.h"
#അന്തര്ഭാവ "lockfile.h"
#അന്തര്ഭാവ "cache-tree.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "diff.h"
#അന്തര്ഭാവ "unpack-trees.h"
#അന്തര്ഭാവ "branch.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "preload-index.h"
#അന്തര്ഭാവ "sequencer.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "merge-ort-wrappers.h"
#അന്തര്ഭാവ "log-tree.h"
#അന്തര്ഭാവ "notes-utils.h"
#അന്തര്ഭാവ "rerere.h"
#അന്തര്ഭാവ "mailinfo.h"
#അന്തര്ഭാവ "apply.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "pager.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "pretty.h"

/**
 * Returns the length of the first line of msg.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ linelen(സ്ഥിരം അക്ഷരം *സന്ദേശം)
{
	തിരികെ strchrnul(സന്ദേശം, '\n') - സന്ദേശം;
}

/**
 * Returns true if `str` consists of only whitespace, false otherwise.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ str_isspace(സ്ഥിരം അക്ഷരം *str)
{
	വേണ്ടി (; *str; str++)
		എങ്കിൽ (!isspace(*str))
			തിരികെ 0;

	തിരികെ 1;
}

എണ്ണൽ patch_format {
	PATCH_FORMAT_UNKNOWN = 0,
	PATCH_FORMAT_MBOX,
	PATCH_FORMAT_STGIT,
	PATCH_FORMAT_STGIT_SERIES,
	PATCH_FORMAT_HG,
	PATCH_FORMAT_MBOXRD
};

എണ്ണൽ keep_type {
	KEEP_FALSE = 0,
	KEEP_TRUE,      /* pass -k flag to git-mailinfo */
	KEEP_NON_PATCH  /* pass -b flag to git-mailinfo */
};

എണ്ണൽ scissors_type {
	SCISSORS_UNSET = -1,
	SCISSORS_FALSE = 0,  /* pass --no-scissors to git-mailinfo */
	SCISSORS_TRUE        /* pass --scissors to git-mailinfo */
};

എണ്ണൽ signoff_type {
	SIGNOFF_FALSE = 0,
	SIGNOFF_TRUE = 1,
	SIGNOFF_EXPLICIT /* --signoff was set on the command-line */
};

എണ്ണൽ resume_type {
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

എണ്ണൽ empty_action {
	STOP_ON_EMPTY_COMMIT = 0,  /* output errors and stop in the middle of an am session */
	DROP_EMPTY_COMMIT,         /* skip with a notice message, unless "--quiet" has been passed */
	KEEP_EMPTY_COMMIT,         /* keep recording as empty commits */
};

ഘടന am_state {
	/* state directory path */
	അക്ഷരം *dir;

	/* current and last patch numbers, 1-indexed */
	പൂർണ്ണസംഖ്യ cur;
	പൂർണ്ണസംഖ്യ last;

	/* commit metadata and message */
	അക്ഷരം *author_name;
	അക്ഷരം *author_email;
	അക്ഷരം *author_date;
	അക്ഷരം *സന്ദേശം;
	size_t സന്ദേശം_len;

	/* when --rebasing, records the original commit the patch came from */
	ഘടന object_id orig_commit;

	/* number of digits in patch filename */
	പൂർണ്ണസംഖ്യ prec;

	/* various operating modes and command line options */
	പൂർണ്ണസംഖ്യ interactive;
	പൂർണ്ണസംഖ്യ no_verify;
	പൂർണ്ണസംഖ്യ threeway;
	പൂർണ്ണസംഖ്യ quiet;
	പൂർണ്ണസംഖ്യ signoff; /* enum signoff_type */
	പൂർണ്ണസംഖ്യ utf8;
	പൂർണ്ണസംഖ്യ keep; /* enum keep_type */
	പൂർണ്ണസംഖ്യ message_id;
	പൂർണ്ണസംഖ്യ scissors; /* enum scissors_type */
	പൂർണ്ണസംഖ്യ quoted_cr; /* enum quoted_cr_action */
	പൂർണ്ണസംഖ്യ empty_type; /* enum empty_action */
	ഘടന strvec git_apply_opts;
	സ്ഥിരം അക്ഷരം *resolvemsg;
	പൂർണ്ണസംഖ്യ committer_date_is_author_date;
	പൂർണ്ണസംഖ്യ ignore_date;
	പൂർണ്ണസംഖ്യ allow_rerere_autoupdate;
	സ്ഥിരം അക്ഷരം *sign_commit;
	പൂർണ്ണസംഖ്യ rebasing;
};

/**
 * Initializes am_state with the default values.
 */
സ്ഥിതം ശൂന്യം am_state_init(ഘടന am_state *state)
{
	പൂർണ്ണസംഖ്യ gpgsign;

	memset(state, 0, വലുപ്പം(*state));

	state->dir = repo_git_path(the_repository, "rebase-apply");

	state->prec = 4;

	repo_config_get_bool(the_repository, "am.threeway", &state->threeway);

	state->utf8 = 1;

	repo_config_get_bool(the_repository, "am.messageid", &state->message_id);

	state->scissors = SCISSORS_UNSET;
	state->quoted_cr = quoted_cr_unset;

	strvec_init(&state->git_apply_opts);

	എങ്കിൽ (!repo_config_get_bool(the_repository, "commit.gpgsign", &gpgsign))
		state->sign_commit = gpgsign ? "" : ശൂന്യ;
}

/**
 * Releases memory allocated by an am_state.
 */
സ്ഥിതം ശൂന്യം am_state_release(ഘടന am_state *state)
{
	സ്വതന്ത്രം(state->dir);
	സ്വതന്ത്രം(state->author_name);
	സ്വതന്ത്രം(state->author_email);
	സ്വതന്ത്രം(state->author_date);
	സ്വതന്ത്രം(state->സന്ദേശം);
	strvec_clear(&state->git_apply_opts);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ am_option_parse_quoted_cr(സ്ഥിരം ഘടന option *opt,
				     സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	BUG_ON_OPT_NEG(unset);

	എങ്കിൽ (mailinfo_parse_quoted_cr_action(arg, opt->മൂല്യം) != 0)
		തിരികെ error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ am_option_parse_empty(സ്ഥിരം ഘടന option *opt,
				     സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	പൂർണ്ണസംഖ്യ *opt_മൂല്യം = opt->മൂല്യം;

	BUG_ON_OPT_NEG(unset);

	എങ്കിൽ (!strcmp(arg, "stop"))
		*opt_മൂല്യം = STOP_ON_EMPTY_COMMIT;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "drop"))
		*opt_മൂല്യം = DROP_EMPTY_COMMIT;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "keep"))
		*opt_മൂല്യം = KEEP_EMPTY_COMMIT;
	അല്ലെങ്കിൽ
		തിരികെ error(_("invalid value for '%s': '%s'"), "--empty", arg);

	തിരികെ 0;
}

/**
 * Returns path relative to the am_state directory.
 */
സ്ഥിതം വരിയിൽ സ്ഥിരം അക്ഷരം *am_path(സ്ഥിരം ഘടന am_state *state, സ്ഥിരം അക്ഷരം *path)
{
	തിരികെ mkpath("%s/%s", state->dir, path);
}

/**
 * For convenience to call write_file()
 */
സ്ഥിതം ശൂന്യം write_state_text(സ്ഥിരം ഘടന am_state *state,
			     സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *string)
{
	write_file(am_path(state, name), "%s", string);
}

സ്ഥിതം ശൂന്യം write_state_എണ്ണം(സ്ഥിരം ഘടന am_state *state,
			      സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ മൂല്യം)
{
	write_file(am_path(state, name), "%d", മൂല്യം);
}

സ്ഥിതം ശൂന്യം write_state_bool(സ്ഥിരം ഘടന am_state *state,
			     സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ മൂല്യം)
{
	write_state_text(state, name, മൂല്യം ? "t" : "f");
}

/**
 * If state->quiet is false, calls fprintf(fp, fmt, ...), and appends a newline
 * at the end.
 */
__attribute__((format (printf, 3, 4)))
സ്ഥിതം ശൂന്യം say(സ്ഥിരം ഘടന am_state *state, FILE *fp, സ്ഥിരം അക്ഷരം *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	എങ്കിൽ (!state->quiet) {
		vfprintf(fp, fmt, ap);
		putc('\n', fp);
	}
	va_end(ap);
}

/**
 * Returns 1 if there is an am session in progress, 0 otherwise.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ am_in_progress(സ്ഥിരം ഘടന am_state *state)
{
	ഘടന stat st;

	എങ്കിൽ (lstat(state->dir, &st) < 0 || !S_ISDIR(st.st_mode))
		തിരികെ 0;
	എങ്കിൽ (lstat(am_path(state, "last"), &st) || !S_ISREG(st.st_mode))
		തിരികെ 0;
	എങ്കിൽ (lstat(am_path(state, "next"), &st) || !S_ISREG(st.st_mode))
		തിരികെ 0;
	തിരികെ 1;
}

/**
 * Reads the contents of `file` in the `state` directory into `sb`. Returns the
 * number of bytes read on success, -1 if the file does not exist. If `trim` is
 * set, trailing whitespace will be removed.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ read_state_file(ഘടന strbuf *sb, സ്ഥിരം ഘടന am_state *state,
			സ്ഥിരം അക്ഷരം *file, പൂർണ്ണസംഖ്യ trim)
{
	strbuf_reset(sb);

	എങ്കിൽ (strbuf_read_file(sb, am_path(state, file), 0) >= 0) {
		എങ്കിൽ (trim)
			strbuf_trim(sb);

		തിരികെ sb->len;
	}

	എങ്കിൽ (errno == ENOENT)
		തിരികെ -1;

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
സ്ഥിതം പൂർണ്ണസംഖ്യ read_am_author_script(ഘടന am_state *state)
{
	സ്ഥിരം അക്ഷരം *filename = am_path(state, "author-script");

	പ്രതിജ്ഞാ(!state->author_name);
	പ്രതിജ്ഞാ(!state->author_email);
	പ്രതിജ്ഞാ(!state->author_date);

	തിരികെ read_author_script(filename, &state->author_name,
				  &state->author_email, &state->author_date, 1);
}

/**
 * Saves state->author_name, state->author_email and state->author_date in the
 * state directory's "author-script" file.
 */
സ്ഥിതം ശൂന്യം write_author_script(സ്ഥിരം ഘടന am_state *state)
{
	ഘടന strbuf sb = STRBUF_INIT;

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
സ്ഥിതം പൂർണ്ണസംഖ്യ read_commit_സന്ദേശം(ഘടന am_state *state)
{
	ഘടന strbuf sb = STRBUF_INIT;

	പ്രതിജ്ഞാ(!state->സന്ദേശം);

	എങ്കിൽ (read_state_file(&sb, state, "final-commit", 0) < 0) {
		strbuf_release(&sb);
		തിരികെ -1;
	}

	state->സന്ദേശം = strbuf_detach(&sb, &state->സന്ദേശം_len);
	തിരികെ 0;
}

/**
 * Saves state->msg in the state directory's "final-commit" file.
 */
സ്ഥിതം ശൂന്യം write_commit_സന്ദേശം(സ്ഥിരം ഘടന am_state *state)
{
	സ്ഥിരം അക്ഷരം *filename = am_path(state, "final-commit");
	write_file_buf(filename, state->സന്ദേശം, state->സന്ദേശം_len);
}

/**
 * Loads state from disk.
 */
സ്ഥിതം ശൂന്യം am_load(ഘടന am_state *state)
{
	ഘടന strbuf sb = STRBUF_INIT;

	എങ്കിൽ (read_state_file(&sb, state, "next", 1) < 0)
		BUG("state file 'next' does not exist");
	state->cur = strtol(sb.buf, ശൂന്യ, 10);

	എങ്കിൽ (read_state_file(&sb, state, "last", 1) < 0)
		BUG("state file 'last' does not exist");
	state->last = strtol(sb.buf, ശൂന്യ, 10);

	എങ്കിൽ (read_am_author_script(state) < 0)
		die(_("could not parse author script"));

	read_commit_സന്ദേശം(state);

	എങ്കിൽ (read_state_file(&sb, state, "original-commit", 1) < 0)
		oidclr(&state->orig_commit, the_repository->hash_algo);
	അല്ലെങ്കിൽ എങ്കിൽ (get_oid_hex(sb.buf, &state->orig_commit) < 0)
		die(_("could not parse %s"), am_path(state, "original-commit"));

	read_state_file(&sb, state, "threeway", 1);
	state->threeway = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "quiet", 1);
	state->quiet = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "sign", 1);
	state->signoff = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "utf8", 1);
	state->utf8 = !strcmp(sb.buf, "t");

	എങ്കിൽ (file_exists(am_path(state, "rerere-autoupdate"))) {
		read_state_file(&sb, state, "rerere-autoupdate", 1);
		state->allow_rerere_autoupdate = strcmp(sb.buf, "t") ?
			RERERE_NOAUTOUPDATE : RERERE_AUTOUPDATE;
	} അല്ലെങ്കിൽ {
		state->allow_rerere_autoupdate = 0;
	}

	read_state_file(&sb, state, "keep", 1);
	എങ്കിൽ (!strcmp(sb.buf, "t"))
		state->keep = KEEP_TRUE;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(sb.buf, "b"))
		state->keep = KEEP_NON_PATCH;
	അല്ലെങ്കിൽ
		state->keep = KEEP_FALSE;

	read_state_file(&sb, state, "messageid", 1);
	state->message_id = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "scissors", 1);
	എങ്കിൽ (!strcmp(sb.buf, "t"))
		state->scissors = SCISSORS_TRUE;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(sb.buf, "f"))
		state->scissors = SCISSORS_FALSE;
	അല്ലെങ്കിൽ
		state->scissors = SCISSORS_UNSET;

	read_state_file(&sb, state, "quoted-cr", 1);
	എങ്കിൽ (!*sb.buf)
		state->quoted_cr = quoted_cr_unset;
	അല്ലെങ്കിൽ എങ്കിൽ (mailinfo_parse_quoted_cr_action(sb.buf, &state->quoted_cr) != 0)
		die(_("could not parse %s"), am_path(state, "quoted-cr"));

	read_state_file(&sb, state, "apply-opt", 1);
	strvec_clear(&state->git_apply_opts);
	എങ്കിൽ (sq_dequote_to_strvec(sb.buf, &state->git_apply_opts) < 0)
		die(_("could not parse %s"), am_path(state, "apply-opt"));

	state->rebasing = !!file_exists(am_path(state, "rebasing"));

	strbuf_release(&sb);
}

/**
 * Removes the am_state directory, forcefully terminating the current am
 * session.
 */
സ്ഥിതം ശൂന്യം am_destroy(സ്ഥിരം ഘടന am_state *state)
{
	ഘടന strbuf sb = STRBUF_INIT;

	strbuf_addstr(&sb, state->dir);
	remove_dir_recursively(&sb, 0);
	strbuf_release(&sb);
}

/**
 * Runs applypatch-msg hook. Returns its exit code.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ run_applypatch_സന്ദേശം_hook(ഘടന am_state *state)
{
	പൂർണ്ണസംഖ്യ ret = 0;

	പ്രതിജ്ഞാ(state->സന്ദേശം);

	എങ്കിൽ (!state->no_verify) {
		ഘടന run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;
		strvec_തള്ളുക(&opt.args, am_path(state, "final-commit"));
		ret = run_hooks_opt(the_repository, "applypatch-msg", &opt);
	}

	എങ്കിൽ (!ret) {
		FREE_AND_NULL(state->സന്ദേശം);
		എങ്കിൽ (read_commit_സന്ദേശം(state) < 0)
			die(_("'%s' was deleted by the applypatch-msg hook"),
				am_path(state, "final-commit"));
	}

	തിരികെ ret;
}

/**
 * Runs post-rewrite hook. Returns it exit code.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ run_post_rewrite_hook(സ്ഥിരം ഘടന am_state *state)
{
	ഘടന run_hooks_opt opt = RUN_HOOKS_OPT_INIT;

	strvec_തള്ളുക(&opt.args, "rebase");
	opt.path_to_stdin = am_path(state, "rewritten");

	തിരികെ run_hooks_opt(the_repository, "post-rewrite", &opt);
}

/**
 * Reads the state directory's "rewritten" file, and copies notes from the old
 * commits listed in the file to their rewritten commits.
 *
 * Returns 0 on success, -1 on failure.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ copy_notes_for_rebase(സ്ഥിരം ഘടന am_state *state)
{
	ഘടന notes_rewrite_cfg *c;
	ഘടന strbuf sb = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *invalid_വരി = _("Malformed input line: '%s'.");
	സ്ഥിരം അക്ഷരം *സന്ദേശം = "Notes added by 'git rebase'";
	FILE *fp;
	പൂർണ്ണസംഖ്യ ret = 0;

	പ്രതിജ്ഞാ(state->rebasing);

	c = init_copy_notes_for_rewrite("rebase");
	എങ്കിൽ (!c)
		തിരികെ 0;

	fp = xfopen(am_path(state, "rewritten"), "r");

	ആകുമ്പോൾ (!strbuf_getline_lf(&sb, fp)) {
		ഘടന object_id from_obj, to_obj;
		സ്ഥിരം അക്ഷരം *p;

		എങ്കിൽ (sb.len != the_hash_algo->hexsz * 2 + 1) {
			ret = error(invalid_വരി, sb.buf);
			പോകുക finish;
		}

		എങ്കിൽ (parse_oid_hex(sb.buf, &from_obj, &p)) {
			ret = error(invalid_വരി, sb.buf);
			പോകുക finish;
		}

		എങ്കിൽ (*p != ' ') {
			ret = error(invalid_വരി, sb.buf);
			പോകുക finish;
		}

		എങ്കിൽ (get_oid_hex(p + 1, &to_obj)) {
			ret = error(invalid_വരി, sb.buf);
			പോകുക finish;
		}

		എങ്കിൽ (copy_note_for_rewrite(c, &from_obj, &to_obj))
			ret = error(_("Failed to copy notes from '%s' to '%s'"),
					oid_to_hex(&from_obj), oid_to_hex(&to_obj));
	}

finish:
	finish_copy_notes_for_rewrite(the_repository, c, സന്ദേശം);
	fclose(fp);
	strbuf_release(&sb);
	തിരികെ ret;
}

/**
 * Determines if the file looks like a piece of RFC2822 mail by grabbing all
 * non-indented lines and checking if they look like they begin with valid
 * header field names.
 *
 * Returns 1 if the file looks like a piece of mail, 0 otherwise.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ is_mail(FILE *fp)
{
	സ്ഥിരം അക്ഷരം *header_regex = "^[!-9;-~]+:";
	ഘടന strbuf sb = STRBUF_INIT;
	regex_t regex;
	പൂർണ്ണസംഖ്യ ret = 1;

	എങ്കിൽ (fseek(fp, 0L, SEEK_SET))
		die_errno(_("fseek failed"));

	എങ്കിൽ (regcomp(&regex, header_regex, REG_NOSUB | REG_EXTENDED))
		die("invalid pattern: %s", header_regex);

	ആകുമ്പോൾ (!strbuf_getline(&sb, fp)) {
		എങ്കിൽ (!sb.len)
			നിർത്തുക; /* End of header */

		/* Ignore indented folded lines */
		എങ്കിൽ (*sb.buf == '\t' || *sb.buf == ' ')
			തുടരുക;

		/* It's a header if it matches header_regex */
		എങ്കിൽ (regexec(&regex, sb.buf, 0, ശൂന്യ, 0)) {
			ret = 0;
			പോകുക done;
		}
	}

done:
	regfree(&regex);
	strbuf_release(&sb);
	തിരികെ ret;
}

/**
 * Attempts to detect the patch_format of the patches contained in `paths`,
 * returning the PATCH_FORMAT_* enum value. Returns PATCH_FORMAT_UNKNOWN if
 * detection fails.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ detect_patch_format(സ്ഥിരം അക്ഷരം **paths)
{
	എണ്ണൽ patch_format ret = PATCH_FORMAT_UNKNOWN;
	ഘടന strbuf l1 = STRBUF_INIT;
	ഘടന strbuf l2 = STRBUF_INIT;
	ഘടന strbuf l3 = STRBUF_INIT;
	FILE *fp;

	/*
	 * We default to mbox format if input is from stdin and for directories
	 */
	എങ്കിൽ (!*paths || !strcmp(*paths, "-") || is_directory(*paths))
		തിരികെ PATCH_FORMAT_MBOX;

	/*
	 * Otherwise, check the first few lines of the first patch, starting
	 * from the first non-blank line, to try to detect its format.
	 */

	fp = xfopen(*paths, "r");

	ആകുമ്പോൾ (!strbuf_getline(&l1, fp)) {
		എങ്കിൽ (l1.len)
			നിർത്തുക;
	}

	എങ്കിൽ (starts_with(l1.buf, "From ") || starts_with(l1.buf, "From: ")) {
		ret = PATCH_FORMAT_MBOX;
		പോകുക done;
	}

	എങ്കിൽ (starts_with(l1.buf, "# This series applies on GIT commit")) {
		ret = PATCH_FORMAT_STGIT_SERIES;
		പോകുക done;
	}

	എങ്കിൽ (!strcmp(l1.buf, "# HG changeset patch")) {
		ret = PATCH_FORMAT_HG;
		പോകുക done;
	}

	strbuf_getline(&l2, fp);
	strbuf_getline(&l3, fp);

	/*
	 * If the second line is empty and the third is a From, Author or Date
	 * entry, this is likely an StGit patch.
	 */
	എങ്കിൽ (l1.len && !l2.len &&
		(starts_with(l3.buf, "From:") ||
		 starts_with(l3.buf, "Author:") ||
		 starts_with(l3.buf, "Date:"))) {
		ret = PATCH_FORMAT_STGIT;
		പോകുക done;
	}

	എങ്കിൽ (l1.len && is_mail(fp)) {
		ret = PATCH_FORMAT_MBOX;
		പോകുക done;
	}

done:
	fclose(fp);
	strbuf_release(&l1);
	strbuf_release(&l2);
	strbuf_release(&l3);
	തിരികെ ret;
}

/**
 * Splits out individual email patches from `paths`, where each path is either
 * a mbox file or a Maildir. Returns 0 on success, -1 on failure.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ split_mail_mbox(ഘടന am_state *state, സ്ഥിരം അക്ഷരം **paths,
				പൂർണ്ണസംഖ്യ keep_cr, പൂർണ്ണസംഖ്യ mboxrd)
{
	ഘടന child_process cp = CHILD_PROCESS_INIT;
	ഘടന strbuf last = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ ret;

	cp.git_cmd = 1;
	strvec_തള്ളുക(&cp.args, "mailsplit");
	strvec_pushf(&cp.args, "-d%d", state->prec);
	strvec_pushf(&cp.args, "-o%s", state->dir);
	strvec_തള്ളുക(&cp.args, "-b");
	എങ്കിൽ (keep_cr)
		strvec_തള്ളുക(&cp.args, "--keep-cr");
	എങ്കിൽ (mboxrd)
		strvec_തള്ളുക(&cp.args, "--mboxrd");
	strvec_തള്ളുക(&cp.args, "--");
	strvec_pushv(&cp.args, paths);

	ret = capture_command(&cp, &last, 8);
	എങ്കിൽ (ret)
		പോകുക exit;

	state->cur = 1;
	state->last = strtol(last.buf, ശൂന്യ, 10);

exit:
	strbuf_release(&last);
	തിരികെ ret ? -1 : 0;
}

/**
 * Callback signature for split_mail_conv(). The foreign patch should be
 * read from `in`, and the converted patch (in RFC2822 mail format) should be
 * written to `out`. Return 0 on success, or -1 on failure.
 */
തരനിർവചനം പൂർണ്ണസംഖ്യ (*mail_conv_fn)(FILE *out, FILE *in, പൂർണ്ണസംഖ്യ keep_cr);

/**
 * Calls `fn` for each file in `paths` to convert the foreign patch to the
 * RFC2822 mail format suitable for parsing with git-mailinfo.
 *
 * Returns 0 on success, -1 on failure.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ split_mail_conv(mail_conv_fn fn, ഘടന am_state *state,
			സ്ഥിരം അക്ഷരം **paths, പൂർണ്ണസംഖ്യ keep_cr)
{
	സ്ഥിതം സ്ഥിരം അക്ഷരം *stdin_only[] = {"-", ശൂന്യ};
	പൂർണ്ണസംഖ്യ i;

	എങ്കിൽ (!*paths)
		paths = stdin_only;

	വേണ്ടി (i = 0; *paths; paths++, i++) {
		FILE *in, *out;
		സ്ഥിരം അക്ഷരം *mail;
		പൂർണ്ണസംഖ്യ ret;

		എങ്കിൽ (!strcmp(*paths, "-"))
			in = stdin;
		അല്ലെങ്കിൽ
			in = fopen(*paths, "r");

		എങ്കിൽ (!in)
			തിരികെ error_errno(_("could not open '%s' for reading"),
					   *paths);

		mail = mkpath("%s/%0*d", state->dir, state->prec, i + 1);

		out = fopen(mail, "w");
		എങ്കിൽ (!out) {
			എങ്കിൽ (in != stdin)
				fclose(in);
			തിരികെ error_errno(_("could not open '%s' for writing"),
					   mail);
		}

		ret = fn(out, in, keep_cr);

		fclose(out);
		എങ്കിൽ (in != stdin)
			fclose(in);

		എങ്കിൽ (ret)
			തിരികെ error(_("could not parse patch '%s'"), *paths);
	}

	state->cur = 1;
	state->last = i;
	തിരികെ 0;
}

/**
 * A split_mail_conv() callback that converts an StGit patch to an RFC2822
 * message suitable for parsing with git-mailinfo.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ stgit_patch_to_mail(FILE *out, FILE *in, പൂർണ്ണസംഖ്യ keep_cr UNUSED)
{
	ഘടന strbuf sb = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ subject_printed = 0;

	ആകുമ്പോൾ (!strbuf_getline_lf(&sb, in)) {
		സ്ഥിരം അക്ഷരം *str;

		എങ്കിൽ (str_isspace(sb.buf))
			തുടരുക;
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(sb.buf, "Author:", &str))
			fprintf(out, "From:%s\n", str);
		അല്ലെങ്കിൽ എങ്കിൽ (starts_with(sb.buf, "From") || starts_with(sb.buf, "Date"))
			fprintf(out, "%s\n", sb.buf);
		അല്ലെങ്കിൽ എങ്കിൽ (!subject_printed) {
			fprintf(out, "Subject: %s\n", sb.buf);
			subject_printed = 1;
		} അല്ലെങ്കിൽ {
			fprintf(out, "\n%s\n", sb.buf);
			നിർത്തുക;
		}
	}

	strbuf_reset(&sb);
	ആകുമ്പോൾ (strbuf_fread(&sb, 8192, in) > 0) {
		fwrite(sb.buf, 1, sb.len, out);
		strbuf_reset(&sb);
	}

	strbuf_release(&sb);
	തിരികെ 0;
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
സ്ഥിതം പൂർണ്ണസംഖ്യ split_mail_stgit_series(ഘടന am_state *state, സ്ഥിരം അക്ഷരം **paths,
					പൂർണ്ണസംഖ്യ keep_cr)
{
	സ്ഥിരം അക്ഷരം *series_dir;
	അക്ഷരം *series_dir_buf;
	FILE *fp;
	ഘടന strvec patches = STRVEC_INIT;
	ഘടന strbuf sb = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ ret;

	എങ്കിൽ (!paths[0] || paths[1])
		തിരികെ error(_("Only one StGIT patch series can be applied at once"));

	series_dir_buf = xstrdup(*paths);
	series_dir = dirname(series_dir_buf);

	fp = fopen(*paths, "r");
	എങ്കിൽ (!fp) {
		സ്വതന്ത്രം(series_dir_buf);
		തിരികെ error_errno(_("could not open '%s' for reading"), *paths);
	}

	ആകുമ്പോൾ (!strbuf_getline_lf(&sb, fp)) {
		എങ്കിൽ (*sb.buf == '#')
			തുടരുക; /* skip comment lines */

		strvec_തള്ളുക(&patches, mkpath("%s/%s", series_dir, sb.buf));
	}

	fclose(fp);
	strbuf_release(&sb);
	സ്വതന്ത്രം(series_dir_buf);

	ret = split_mail_conv(stgit_patch_to_mail, state, patches.v, keep_cr);

	strvec_clear(&patches);
	തിരികെ ret;
}

/**
 * A split_patches_conv() callback that converts a mercurial patch to a RFC2822
 * message suitable for parsing with git-mailinfo.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ hg_patch_to_mail(FILE *out, FILE *in, പൂർണ്ണസംഖ്യ keep_cr UNUSED)
{
	ഘടന strbuf sb = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ rc = 0;

	ആകുമ്പോൾ (!strbuf_getline_lf(&sb, in)) {
		സ്ഥിരം അക്ഷരം *str;

		എങ്കിൽ (skip_prefix(sb.buf, "# User ", &str))
			fprintf(out, "From: %s\n", str);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(sb.buf, "# Date ", &str)) {
			timestamp_t timestamp;
			long tz, tz2;
			അക്ഷരം *end;

			errno = 0;
			timestamp = parse_timestamp(str, &end, 10);
			എങ്കിൽ (errno) {
				rc = error(_("invalid timestamp"));
				പോകുക exit;
			}

			എങ്കിൽ (!skip_prefix(end, " ", &str)) {
				rc = error(_("invalid Date line"));
				പോകുക exit;
			}

			errno = 0;
			tz = strtol(str, &end, 10);
			എങ്കിൽ (errno) {
				rc = error(_("invalid timezone offset"));
				പോകുക exit;
			}

			എങ്കിൽ (*end) {
				rc = error(_("invalid Date line"));
				പോകുക exit;
			}

			/*
			 * mercurial's timezone is in seconds west of UTC,
			 * however git's timezone is in hours + minutes east of
			 * UTC. Convert it.
			 */
			tz2 = labs(tz) / 3600 * 100 + labs(tz) % 3600 / 60;
			എങ്കിൽ (tz > 0)
				tz2 = -tz2;

			fprintf(out, "Date: %s\n", show_date(timestamp, tz2, DATE_MODE(RFC2822)));
		} അല്ലെങ്കിൽ എങ്കിൽ (starts_with(sb.buf, "# ")) {
			തുടരുക;
		} അല്ലെങ്കിൽ {
			fprintf(out, "\n%s\n", sb.buf);
			നിർത്തുക;
		}
	}

	strbuf_reset(&sb);
	ആകുമ്പോൾ (strbuf_fread(&sb, 8192, in) > 0) {
		fwrite(sb.buf, 1, sb.len, out);
		strbuf_reset(&sb);
	}
exit:
	strbuf_release(&sb);
	തിരികെ rc;
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
സ്ഥിതം പൂർണ്ണസംഖ്യ split_mail(ഘടന am_state *state, എണ്ണൽ patch_format patch_format,
			സ്ഥിരം അക്ഷരം **paths, പൂർണ്ണസംഖ്യ keep_cr)
{
	എങ്കിൽ (keep_cr < 0) {
		keep_cr = 0;
		repo_config_get_bool(the_repository, "am.keepcr", &keep_cr);
	}

	തിരഞ്ഞെടുപ്പ് (patch_format) {
	അവസ്ഥ PATCH_FORMAT_MBOX:
		തിരികെ split_mail_mbox(state, paths, keep_cr, 0);
	അവസ്ഥ PATCH_FORMAT_STGIT:
		തിരികെ split_mail_conv(stgit_patch_to_mail, state, paths, keep_cr);
	അവസ്ഥ PATCH_FORMAT_STGIT_SERIES:
		തിരികെ split_mail_stgit_series(state, paths, keep_cr);
	അവസ്ഥ PATCH_FORMAT_HG:
		തിരികെ split_mail_conv(hg_patch_to_mail, state, paths, keep_cr);
	അവസ്ഥ PATCH_FORMAT_MBOXRD:
		തിരികെ split_mail_mbox(state, paths, keep_cr, 1);
	സ്ഥിരസ്ഥിതി:
		BUG("invalid patch_format");
	}
	തിരികെ -1;
}

/**
 * Setup a new am session for applying patches
 */
സ്ഥിതം ശൂന്യം am_setup(ഘടന am_state *state, എണ്ണൽ patch_format patch_format,
			സ്ഥിരം അക്ഷരം **paths, പൂർണ്ണസംഖ്യ keep_cr)
{
	ഘടന object_id curr_head;
	സ്ഥിരം അക്ഷരം *str;
	ഘടന strbuf sb = STRBUF_INIT;

	എങ്കിൽ (!patch_format)
		patch_format = detect_patch_format(paths);

	എങ്കിൽ (!patch_format) {
		fprintf_ln(stderr, _("Patch format detection failed."));
		die(ശൂന്യ);
	}

	എങ്കിൽ (mkdir(state->dir, 0777) < 0 && errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), state->dir);
	refs_delete_ref(get_main_ref_store(the_repository), ശൂന്യ,
			"REBASE_HEAD", ശൂന്യ, REF_NO_DEREF);

	എങ്കിൽ (split_mail(state, patch_format, paths, keep_cr) < 0) {
		am_destroy(state);
		die(_("Failed to split patches."));
	}

	എങ്കിൽ (state->rebasing)
		state->threeway = 1;

	write_state_bool(state, "threeway", state->threeway);
	write_state_bool(state, "quiet", state->quiet);
	write_state_bool(state, "sign", state->signoff);
	write_state_bool(state, "utf8", state->utf8);

	എങ്കിൽ (state->allow_rerere_autoupdate)
		write_state_bool(state, "rerere-autoupdate",
			 state->allow_rerere_autoupdate == RERERE_AUTOUPDATE);

	തിരഞ്ഞെടുപ്പ് (state->keep) {
	അവസ്ഥ KEEP_FALSE:
		str = "f";
		നിർത്തുക;
	അവസ്ഥ KEEP_TRUE:
		str = "t";
		നിർത്തുക;
	അവസ്ഥ KEEP_NON_PATCH:
		str = "b";
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("invalid value for state->keep");
	}

	write_state_text(state, "keep", str);
	write_state_bool(state, "messageid", state->message_id);

	തിരഞ്ഞെടുപ്പ് (state->scissors) {
	അവസ്ഥ SCISSORS_UNSET:
		str = "";
		നിർത്തുക;
	അവസ്ഥ SCISSORS_FALSE:
		str = "f";
		നിർത്തുക;
	അവസ്ഥ SCISSORS_TRUE:
		str = "t";
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("invalid value for state->scissors");
	}
	write_state_text(state, "scissors", str);

	തിരഞ്ഞെടുപ്പ് (state->quoted_cr) {
	അവസ്ഥ quoted_cr_unset:
		str = "";
		നിർത്തുക;
	അവസ്ഥ quoted_cr_nowarn:
		str = "nowarn";
		നിർത്തുക;
	അവസ്ഥ quoted_cr_warn:
		str = "warn";
		നിർത്തുക;
	അവസ്ഥ quoted_cr_strip:
		str = "strip";
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("invalid value for state->quoted_cr");
	}
	write_state_text(state, "quoted-cr", str);

	sq_quote_argv(&sb, state->git_apply_opts.v);
	write_state_text(state, "apply-opt", sb.buf);

	എങ്കിൽ (state->rebasing)
		write_state_text(state, "rebasing", "");
	അല്ലെങ്കിൽ
		write_state_text(state, "applying", "");

	എങ്കിൽ (!repo_get_oid(the_repository, "HEAD", &curr_head)) {
		write_state_text(state, "abort-safety", oid_to_hex(&curr_head));
		എങ്കിൽ (!state->rebasing)
			refs_update_ref(get_main_ref_store(the_repository),
					"am", "ORIG_HEAD", &curr_head, ശൂന്യ,
					0,
					UPDATE_REFS_DIE_ON_ERR);
	} അല്ലെങ്കിൽ {
		write_state_text(state, "abort-safety", "");
		എങ്കിൽ (!state->rebasing)
			refs_delete_ref(get_main_ref_store(the_repository),
					ശൂന്യ, "ORIG_HEAD", ശൂന്യ, 0);
	}

	/*
	 * NOTE: Since the "next" and "last" files determine if an am_state
	 * session is in progress, they should be written last.
	 */

	write_state_എണ്ണം(state, "next", state->cur);
	write_state_എണ്ണം(state, "last", state->last);

	strbuf_release(&sb);
}

/**
 * Increments the patch pointer, and cleans am_state for the application of the
 * next patch.
 */
സ്ഥിതം ശൂന്യം am_next(ഘടന am_state *state)
{
	ഘടന object_id head;

	FREE_AND_NULL(state->author_name);
	FREE_AND_NULL(state->author_email);
	FREE_AND_NULL(state->author_date);
	FREE_AND_NULL(state->സന്ദേശം);
	state->സന്ദേശം_len = 0;

	unlink(am_path(state, "author-script"));
	unlink(am_path(state, "final-commit"));

	oidclr(&state->orig_commit, the_repository->hash_algo);
	unlink(am_path(state, "original-commit"));
	refs_delete_ref(get_main_ref_store(the_repository), ശൂന്യ,
			"REBASE_HEAD", ശൂന്യ, REF_NO_DEREF);

	എങ്കിൽ (!repo_get_oid(the_repository, "HEAD", &head))
		write_state_text(state, "abort-safety", oid_to_hex(&head));
	അല്ലെങ്കിൽ
		write_state_text(state, "abort-safety", "");

	state->cur++;
	write_state_എണ്ണം(state, "next", state->cur);
}

/**
 * Returns the filename of the current patch email.
 */
സ്ഥിതം സ്ഥിരം അക്ഷരം *msgnum(സ്ഥിരം ഘടന am_state *state)
{
	സ്ഥിതം ഘടന strbuf sb = STRBUF_INIT;

	strbuf_reset(&sb);
	strbuf_addf(&sb, "%0*d", state->prec, state->cur);

	തിരികെ sb.buf;
}

/**
 * Dies with a user-friendly message on how to proceed after resolving the
 * problem. This message can be overridden with state->resolvemsg.
 */
സ്ഥിതം ശൂന്യം NORETURN die_user_resolve(സ്ഥിരം ഘടന am_state *state)
{
	എങ്കിൽ (state->resolvemsg) {
		advise_if_enabled(ADVICE_MERGE_CONFLICT, "%s", state->resolvemsg);
	} അല്ലെങ്കിൽ {
		സ്ഥിരം അക്ഷരം *cmdline = state->interactive ? "git am -i" : "git am";
		ഘടന strbuf sb = STRBUF_INIT;

		strbuf_addf(&sb, _("When you have resolved this problem, run \"%s --continue\".\n"), cmdline);
		strbuf_addf(&sb, _("If you prefer to skip this patch, run \"%s --skip\" instead.\n"), cmdline);

		എങ്കിൽ (advice_enabled(ADVICE_AM_WORK_DIR) &&
		    is_empty_or_missing_file(am_path(state, "patch")) &&
		    !repo_index_has_changes(the_repository, ശൂന്യ, ശൂന്യ))
			strbuf_addf(&sb, _("To record the empty patch as an empty commit, run \"%s --allow-empty\".\n"), cmdline);

		strbuf_addf(&sb, _("To restore the original branch and stop patching, run \"%s --abort\"."), cmdline);

		advise_if_enabled(ADVICE_MERGE_CONFLICT, "%s", sb.buf);
		strbuf_release(&sb);
	}

	die(ശൂന്യ);
}

/**
 * Appends signoff to the "msg" field of the am_state.
 */
സ്ഥിതം ശൂന്യം am_append_signoff(ഘടന am_state *state)
{
	ഘടന strbuf sb = STRBUF_INIT;

	strbuf_attach(&sb, state->സന്ദേശം, state->സന്ദേശം_len, state->സന്ദേശം_len + 1);
	append_signoff(&sb, 0, 0);
	state->സന്ദേശം = strbuf_detach(&sb, &state->സന്ദേശം_len);
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
സ്ഥിതം പൂർണ്ണസംഖ്യ parse_mail(ഘടന am_state *state, സ്ഥിരം അക്ഷരം *mail)
{
	FILE *fp;
	ഘടന strbuf sb = STRBUF_INIT;
	ഘടന strbuf സന്ദേശം = STRBUF_INIT;
	ഘടന strbuf author_name = STRBUF_INIT;
	ഘടന strbuf author_date = STRBUF_INIT;
	ഘടന strbuf author_email = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ ret = 0;
	ഘടന mailinfo mi;

	setup_mailinfo(the_repository, &mi);

	എങ്കിൽ (state->utf8)
		mi.metainfo_charset = get_commit_output_encoding();
	അല്ലെങ്കിൽ
		mi.metainfo_charset = ശൂന്യ;

	തിരഞ്ഞെടുപ്പ് (state->keep) {
	അവസ്ഥ KEEP_FALSE:
		നിർത്തുക;
	അവസ്ഥ KEEP_TRUE:
		mi.keep_subject = 1;
		നിർത്തുക;
	അവസ്ഥ KEEP_NON_PATCH:
		mi.keep_non_patch_brackets_in_subject = 1;
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("invalid value for state->keep");
	}

	എങ്കിൽ (state->message_id)
		mi.add_message_id = 1;

	തിരഞ്ഞെടുപ്പ് (state->scissors) {
	അവസ്ഥ SCISSORS_UNSET:
		നിർത്തുക;
	അവസ്ഥ SCISSORS_FALSE:
		mi.use_scissors = 0;
		നിർത്തുക;
	അവസ്ഥ SCISSORS_TRUE:
		mi.use_scissors = 1;
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("invalid value for state->scissors");
	}

	തിരഞ്ഞെടുപ്പ് (state->quoted_cr) {
	അവസ്ഥ quoted_cr_unset:
		നിർത്തുക;
	അവസ്ഥ quoted_cr_nowarn:
	അവസ്ഥ quoted_cr_warn:
	അവസ്ഥ quoted_cr_strip:
		mi.quoted_cr = state->quoted_cr;
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("invalid value for state->quoted_cr");
	}

	mi.നിവേശം = xfopen(mail, "r");
	mi.output = xfopen(am_path(state, "info"), "w");
	എങ്കിൽ (mailinfo(&mi, am_path(state, "msg"), am_path(state, "patch")))
		die("could not parse patch");

	fclose(mi.നിവേശം);
	fclose(mi.output);

	എങ്കിൽ (mi.format_flowed)
		warning(_("Patch sent with format=flowed; "
			  "space at the end of lines might be lost."));

	/* Extract message and author information */
	fp = xfopen(am_path(state, "info"), "r");
	ആകുമ്പോൾ (!strbuf_getline_lf(&sb, fp)) {
		സ്ഥിരം അക്ഷരം *x;

		എങ്കിൽ (skip_prefix(sb.buf, "Subject: ", &x)) {
			എങ്കിൽ (സന്ദേശം.len)
				strbuf_addch(&സന്ദേശം, '\n');
			strbuf_addstr(&സന്ദേശം, x);
		} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(sb.buf, "Author: ", &x))
			strbuf_addstr(&author_name, x);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(sb.buf, "Email: ", &x))
			strbuf_addstr(&author_email, x);
		അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(sb.buf, "Date: ", &x))
			strbuf_addstr(&author_date, x);
	}
	fclose(fp);

	/* Skip pine's internal folder data */
	എങ്കിൽ (!strcmp(author_name.buf, "Mail System Internal Data")) {
		ret = 1;
		പോകുക finish;
	}

	strbuf_addstr(&സന്ദേശം, "\n\n");
	strbuf_addbuf(&സന്ദേശം, &mi.log_message);
	strbuf_stripspace(&സന്ദേശം, ശൂന്യ);

	പ്രതിജ്ഞാ(!state->author_name);
	state->author_name = strbuf_detach(&author_name, ശൂന്യ);

	പ്രതിജ്ഞാ(!state->author_email);
	state->author_email = strbuf_detach(&author_email, ശൂന്യ);

	പ്രതിജ്ഞാ(!state->author_date);
	state->author_date = strbuf_detach(&author_date, ശൂന്യ);

	പ്രതിജ്ഞാ(!state->സന്ദേശം);
	state->സന്ദേശം = strbuf_detach(&സന്ദേശം, &state->സന്ദേശം_len);

finish:
	strbuf_release(&സന്ദേശം);
	strbuf_release(&author_date);
	strbuf_release(&author_email);
	strbuf_release(&author_name);
	strbuf_release(&sb);
	clear_mailinfo(&mi);
	തിരികെ ret;
}

/**
 * Sets commit_id to the commit hash where the mail was generated from.
 * Returns 0 on success, -1 on failure.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ get_mail_commit_oid(ഘടന object_id *commit_id, സ്ഥിരം അക്ഷരം *mail)
{
	ഘടന strbuf sb = STRBUF_INIT;
	FILE *fp = xfopen(mail, "r");
	സ്ഥിരം അക്ഷരം *x;
	പൂർണ്ണസംഖ്യ ret = 0;

	എങ്കിൽ (strbuf_getline_lf(&sb, fp) ||
	    !skip_prefix(sb.buf, "From ", &x) ||
	    get_oid_hex(x, commit_id) < 0)
		ret = -1;

	strbuf_release(&sb);
	fclose(fp);
	തിരികെ ret;
}

/**
 * Sets state->msg, state->author_name, state->author_email, state->author_date
 * to the commit's respective info.
 */
സ്ഥിതം ശൂന്യം get_commit_info(ഘടന am_state *state, ഘടന commit *commit)
{
	സ്ഥിരം അക്ഷരം *buffer, *ident_വരി, *സന്ദേശം;
	size_t ident_len;
	ഘടന ident_split id;

	buffer = repo_logmsg_reencode(the_repository, commit, ശൂന്യ,
				      get_commit_output_encoding());

	ident_വരി = find_commit_header(buffer, "author", &ident_len);
	എങ്കിൽ (!ident_വരി)
		die(_("missing author line in commit %s"),
		      oid_to_hex(&commit->object.oid));
	എങ്കിൽ (split_ident_വരി(&id, ident_വരി, ident_len) < 0)
		die(_("invalid ident line: %.*s"), (പൂർണ്ണസംഖ്യ)ident_len, ident_വരി);

	പ്രതിജ്ഞാ(!state->author_name);
	എങ്കിൽ (id.name_begin)
		state->author_name =
			xmemdupz(id.name_begin, id.name_end - id.name_begin);
	അല്ലെങ്കിൽ
		state->author_name = xstrdup("");

	പ്രതിജ്ഞാ(!state->author_email);
	എങ്കിൽ (id.mail_begin)
		state->author_email =
			xmemdupz(id.mail_begin, id.mail_end - id.mail_begin);
	അല്ലെങ്കിൽ
		state->author_email = xstrdup("");

	പ്രതിജ്ഞാ(!state->author_date);
	state->author_date = xstrdup(show_ident_date(&id, DATE_MODE(NORMAL)));

	പ്രതിജ്ഞാ(!state->സന്ദേശം);
	സന്ദേശം = strstr(buffer, "\n\n");
	എങ്കിൽ (!സന്ദേശം)
		die(_("unable to parse commit %s"), oid_to_hex(&commit->object.oid));
	state->സന്ദേശം = xstrdup(സന്ദേശം + 2);
	state->സന്ദേശം_len = strlen(state->സന്ദേശം);
	repo_unuse_commit_buffer(the_repository, commit, buffer);
}

/**
 * Writes `commit` as a patch to the state directory's "patch" file.
 */
സ്ഥിതം ശൂന്യം write_commit_patch(സ്ഥിരം ഘടന am_state *state, ഘടന commit *commit)
{
	ഘടന rev_info rev_info;
	FILE *fp;

	fp = xfopen(am_path(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, ശൂന്യ);
	rev_info.diff = 1;
	rev_info.abbrev = 0;
	rev_info.disable_stdin = 1;
	rev_info.show_വേര്_diff = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.flags.ദ്വിമാന = 1;
	rev_info.diffopt.flags.full_index = 1;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	add_pending_object(&rev_info, &commit->object, "");
	diff_setup_done(&rev_info.diffopt);
	log_വൃക്ഷം_commit(&rev_info, commit);
	release_revisions(&rev_info);
}

/**
 * Writes the diff of the index against HEAD as a patch to the state
 * directory's "patch" file.
 */
സ്ഥിതം ശൂന്യം write_index_patch(സ്ഥിരം ഘടന am_state *state)
{
	ഘടന വൃക്ഷം *വൃക്ഷം;
	ഘടന object_id head;
	ഘടന rev_info rev_info;
	FILE *fp;

	എങ്കിൽ (!repo_get_oid(the_repository, "HEAD", &head)) {
		ഘടന commit *commit = lookup_commit_or_die(&head, "HEAD");
		വൃക്ഷം = repo_get_commit_വൃക്ഷം(the_repository, commit);
	} അല്ലെങ്കിൽ
		വൃക്ഷം = lookup_വൃക്ഷം(the_repository,
				   the_repository->hash_algo->empty_വൃക്ഷം);

	fp = xfopen(am_path(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, ശൂന്യ);
	rev_info.diff = 1;
	rev_info.disable_stdin = 1;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	add_pending_object(&rev_info, &വൃക്ഷം->object, "");
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
സ്ഥിതം പൂർണ്ണസംഖ്യ parse_mail_rebase(ഘടന am_state *state, സ്ഥിരം അക്ഷരം *mail)
{
	ഘടന commit *commit;
	ഘടന object_id commit_oid;

	എങ്കിൽ (get_mail_commit_oid(&commit_oid, mail) < 0)
		die(_("could not parse %s"), mail);

	commit = lookup_commit_or_die(&commit_oid, mail);

	get_commit_info(state, commit);

	write_commit_patch(state, commit);

	oidcpy(&state->orig_commit, &commit_oid);
	write_state_text(state, "original-commit", oid_to_hex(&commit_oid));
	refs_update_ref(get_main_ref_store(the_repository), "am",
			"REBASE_HEAD", &commit_oid,
			ശൂന്യ, REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);

	തിരികെ 0;
}

/**
 * Applies current patch with git-apply. Returns 0 on success, -1 otherwise. If
 * `index_file` is not NULL, the patch will be applied to that index.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ run_apply(സ്ഥിരം ഘടന am_state *state, സ്ഥിരം അക്ഷരം *index_file)
{
	ഘടന strvec apply_paths = STRVEC_INIT;
	ഘടന strvec apply_opts = STRVEC_INIT;
	ഘടന apply_state apply_state;
	പൂർണ്ണസംഖ്യ res, opts_ഇടത്;
	പൂർണ്ണസംഖ്യ force_apply = 0;
	പൂർണ്ണസംഖ്യ options = 0;
	സ്ഥിരം അക്ഷരം **apply_argv;

	എങ്കിൽ (init_apply_state(&apply_state, the_repository, ശൂന്യ))
		BUG("init_apply_state() failed");

	strvec_തള്ളുക(&apply_opts, "apply");
	strvec_pushv(&apply_opts, state->git_apply_opts.v);

	/*
	 * Build a copy that apply_parse_options() can rearrange.
	 * apply_opts.v keeps referencing the allocated strings for
	 * strvec_clear() to release.
	 */
	DUP_ARRAY(apply_argv, apply_opts.v, apply_opts.nr);

	opts_ഇടത് = apply_parse_options(apply_opts.nr, apply_argv,
					&apply_state, &force_apply, &options,
					ശൂന്യ);

	എങ്കിൽ (opts_ഇടത് != 0)
		die("unknown option passed through to git apply");

	എങ്കിൽ (index_file) {
		apply_state.index_file = index_file;
		apply_state.cached = 1;
	} അല്ലെങ്കിൽ
		apply_state.check_index = 1;

	/*
	 * If we are allowed to fall back on 3-way merge, don't give false
	 * errors during the initial attempt.
	 */
	എങ്കിൽ (state->threeway && !index_file)
		apply_state.apply_verbosity = verbosity_silent;

	എങ്കിൽ (check_apply_state(&apply_state, force_apply))
		BUG("check_apply_state() failed");

	strvec_തള്ളുക(&apply_paths, am_path(state, "patch"));

	res = apply_all_patches(&apply_state, apply_paths.nr, apply_paths.v, options);

	strvec_clear(&apply_paths);
	strvec_clear(&apply_opts);
	clear_apply_state(&apply_state);
	സ്വതന്ത്രം(apply_argv);

	എങ്കിൽ (res)
		തിരികെ res;

	എങ്കിൽ (index_file) {
		/* Reload index as apply_all_patches() will have modified it. */
		discard_index(the_repository->index);
		read_index_from(the_repository->index, index_file,
				repo_get_git_dir(the_repository));
	}

	തിരികെ 0;
}

/**
 * Builds an index that contains just the blobs needed for a 3way merge.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ build_fake_ancestor(സ്ഥിരം ഘടന am_state *state, സ്ഥിരം അക്ഷരം *index_file)
{
	ഘടന child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	strvec_തള്ളുക(&cp.args, "apply");
	strvec_pushv(&cp.args, state->git_apply_opts.v);
	strvec_pushf(&cp.args, "--build-fake-ancestor=%s", index_file);
	strvec_തള്ളുക(&cp.args, am_path(state, "patch"));

	എങ്കിൽ (run_command(&cp))
		തിരികെ -1;

	തിരികെ 0;
}

/**
 * Attempt a threeway merge, using index_path as the temporary index.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ fall_back_threeway(സ്ഥിരം ഘടന am_state *state, സ്ഥിരം അക്ഷരം *index_path)
{
	ഘടന object_id their_വൃക്ഷം, our_വൃക്ഷം;
	ഘടന object_id bases[1] = { 0 };
	ഘടന merge_options o;
	ഘടന commit *result;
	അക്ഷരം *their_വൃക്ഷം_name;

	എങ്കിൽ (repo_get_oid(the_repository, "HEAD", &our_വൃക്ഷം) < 0)
		oidcpy(&our_വൃക്ഷം, the_hash_algo->empty_വൃക്ഷം);

	എങ്കിൽ (build_fake_ancestor(state, index_path))
		തിരികെ error("could not build fake ancestor");

	discard_index(the_repository->index);
	read_index_from(the_repository->index, index_path, repo_get_git_dir(the_repository));

	എങ്കിൽ (write_index_as_വൃക്ഷം(&bases[0], the_repository->index, index_path, 0, ശൂന്യ))
		തിരികെ error(_("Repository lacks necessary blobs to fall back on 3-way merge."));

	say(state, stdout, _("Using index info to reconstruct a base tree..."));

	എങ്കിൽ (!state->quiet) {
		/*
		 * List paths that needed 3-way fallback, so that the user can
		 * review them with extra care to spot mismerges.
		 */
		ഘടന rev_info rev_info;

		repo_init_revisions(the_repository, &rev_info, ശൂന്യ);
		rev_info.diffopt.output_format = DIFF_FORMAT_NAME_STATUS;
		rev_info.diffopt.filter |= diff_filter_bit('A');
		rev_info.diffopt.filter |= diff_filter_bit('M');
		add_pending_oid(&rev_info, "HEAD", &our_വൃക്ഷം, 0);
		diff_setup_done(&rev_info.diffopt);
		run_diff_index(&rev_info, DIFF_INDEX_CACHED);
		release_revisions(&rev_info);
	}

	എങ്കിൽ (run_apply(state, index_path))
		തിരികെ error(_("Did you hand edit your patch?\n"
				"It does not apply to blobs recorded in its index."));

	എങ്കിൽ (write_index_as_വൃക്ഷം(&their_വൃക്ഷം, the_repository->index, index_path, 0, ശൂന്യ))
		തിരികെ error("could not write tree");

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
	their_വൃക്ഷം_name = xstrfmt("%.*s", linelen(state->സന്ദേശം), state->സന്ദേശം);
	o.branch2 = their_വൃക്ഷം_name;
	o.ancestor = "constructed fake ancestor";
	o.detect_directory_renames = MERGE_DIRECTORY_RENAMES_NONE;

	എങ്കിൽ (state->quiet)
		o.verbosity = 0;

	എങ്കിൽ (merge_ort_generic(&o, &our_വൃക്ഷം, &their_വൃക്ഷം, 1, bases, &result)) {
		repo_rerere(the_repository, state->allow_rerere_autoupdate);
		സ്വതന്ത്രം(their_വൃക്ഷം_name);
		തിരികെ error(_("Failed to merge in the changes."));
	}

	സ്വതന്ത്രം(their_വൃക്ഷം_name);
	തിരികെ 0;
}

/**
 * Commits the current index with state->msg as the commit message and
 * state->author_name, state->author_email and state->author_date as the author
 * information.
 */
സ്ഥിതം ശൂന്യം do_commit(സ്ഥിരം ഘടന am_state *state)
{
	ഘടന object_id വൃക്ഷം, parent, commit;
	സ്ഥിരം ഘടന object_id *old_oid;
	ഘടന commit_list *parents = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *reflog_സന്ദേശം, *author, *committer = ശൂന്യ;
	ഘടന strbuf sb = STRBUF_INIT;

	എങ്കിൽ (!state->no_verify && run_hooks(the_repository, "pre-applypatch"))
		exit(1);

	എങ്കിൽ (write_index_as_വൃക്ഷം(&വൃക്ഷം, the_repository->index,
				repo_get_index_file(the_repository),
				0, ശൂന്യ))
		die(_("git write-tree failed to write a tree"));

	എങ്കിൽ (!repo_get_oid_commit(the_repository, "HEAD", &parent)) {
		old_oid = &parent;
		commit_list_തിരുകുക(lookup_commit(the_repository, &parent),
				   &parents);
	} അല്ലെങ്കിൽ {
		old_oid = ശൂന്യ;
		say(state, stderr, _("applying to an empty history"));
	}

	author = fmt_ident(state->author_name, state->author_email,
		WANT_AUTHOR_IDENT,
			state->ignore_date ? ശൂന്യ : state->author_date,
			IDENT_STRICT);

	എങ്കിൽ (state->committer_date_is_author_date)
		committer = fmt_ident(getenv("GIT_COMMITTER_NAME"),
				      getenv("GIT_COMMITTER_EMAIL"),
				      WANT_COMMITTER_IDENT,
				      state->ignore_date ? ശൂന്യ
							 : state->author_date,
				      IDENT_STRICT);

	എങ്കിൽ (commit_വൃക്ഷം_extended(state->സന്ദേശം, state->സന്ദേശം_len, &വൃക്ഷം, parents,
				 &commit, author, committer, state->sign_commit,
				 ശൂന്യ))
		die(_("failed to write commit object"));

	reflog_സന്ദേശം = getenv("GIT_REFLOG_ACTION");
	എങ്കിൽ (!reflog_സന്ദേശം)
		reflog_സന്ദേശം = "am";

	strbuf_addf(&sb, "%s: %.*s", reflog_സന്ദേശം, linelen(state->സന്ദേശം),
			state->സന്ദേശം);

	refs_update_ref(get_main_ref_store(the_repository), sb.buf, "HEAD",
			&commit, old_oid, 0,
			UPDATE_REFS_DIE_ON_ERR);

	എങ്കിൽ (state->rebasing) {
		FILE *fp = xfopen(am_path(state, "rewritten"), "a");

		പ്രതിജ്ഞാ(!is_null_oid(&state->orig_commit));
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
സ്ഥിതം ശൂന്യം validate_resume_state(സ്ഥിരം ഘടന am_state *state)
{
	എങ്കിൽ (!state->സന്ദേശം)
		die(_("cannot resume: %s does not exist."),
			am_path(state, "final-commit"));

	എങ്കിൽ (!state->author_name || !state->author_email || !state->author_date)
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
സ്ഥിതം പൂർണ്ണസംഖ്യ do_interactive(ഘടന am_state *state)
{
	പ്രതിജ്ഞാ(state->സന്ദേശം);

	വേണ്ടി (;;) {
		അക്ഷരം reply[64];

		puts(_("Commit Body is:"));
		puts("--------------------------");
		printf("%s", state->സന്ദേശം);
		puts("--------------------------");

		/*
		 * TRANSLATORS: Make sure to include [y], [n], [e], [v] and [a]
		 * in your translation. The program will only accept English
		 * input at this point.
		 */
		printf(_("Apply? [y]es/[n]o/[e]dit/[v]iew patch/[a]ccept all: "));
		എങ്കിൽ (!fgets(reply, വലുപ്പം(reply), stdin))
			die("unable to read from stdin; aborting");

		എങ്കിൽ (*reply == 'y' || *reply == 'Y') {
			തിരികെ 0;
		} അല്ലെങ്കിൽ എങ്കിൽ (*reply == 'a' || *reply == 'A') {
			state->interactive = 0;
			തിരികെ 0;
		} അല്ലെങ്കിൽ എങ്കിൽ (*reply == 'n' || *reply == 'N') {
			തിരികെ 1;
		} അല്ലെങ്കിൽ എങ്കിൽ (*reply == 'e' || *reply == 'E') {
			ഘടന strbuf സന്ദേശം = STRBUF_INIT;

			എങ്കിൽ (!launch_editor(am_path(state, "final-commit"), &സന്ദേശം, ശൂന്യ)) {
				സ്വതന്ത്രം(state->സന്ദേശം);
				state->സന്ദേശം = strbuf_detach(&സന്ദേശം, &state->സന്ദേശം_len);
			}
			strbuf_release(&സന്ദേശം);
		} അല്ലെങ്കിൽ എങ്കിൽ (*reply == 'v' || *reply == 'V') {
			സ്ഥിരം അക്ഷരം *pager = git_pager(the_repository, 1);
			ഘടന child_process cp = CHILD_PROCESS_INIT;

			എങ്കിൽ (!pager)
				pager = "cat";
			prepare_pager_args(&cp, pager);
			strvec_തള്ളുക(&cp.args, am_path(state, "patch"));
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
സ്ഥിതം ശൂന്യം am_run(ഘടന am_state *state, പൂർണ്ണസംഖ്യ resume)
{
	ഘടന strbuf sb = STRBUF_INIT;

	unlink(am_path(state, "dirtyindex"));

	എങ്കിൽ (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 ശൂന്യ, ശൂന്യ, ശൂന്യ) < 0)
		die(_("unable to write index file"));

	എങ്കിൽ (repo_index_has_changes(the_repository, ശൂന്യ, &sb)) {
		write_state_bool(state, "dirtyindex", 1);
		die(_("Dirty index: cannot apply patches (dirty: %s)"), sb.buf);
	}

	strbuf_release(&sb);

	ആകുമ്പോൾ (state->cur <= state->last) {
		സ്ഥിരം അക്ഷരം *mail = am_path(state, msgnum(state));
		പൂർണ്ണസംഖ്യ apply_status;
		പൂർണ്ണസംഖ്യ to_keep;

		reset_ident_date();

		എങ്കിൽ (!file_exists(mail))
			പോകുക next;

		എങ്കിൽ (resume) {
			validate_resume_state(state);
		} അല്ലെങ്കിൽ {
			പൂർണ്ണസംഖ്യ skip;

			എങ്കിൽ (state->rebasing)
				skip = parse_mail_rebase(state, mail);
			അല്ലെങ്കിൽ
				skip = parse_mail(state, mail);

			എങ്കിൽ (skip)
				പോകുക next; /* mail should be skipped */

			എങ്കിൽ (state->signoff)
				am_append_signoff(state);

			write_author_script(state);
			write_commit_സന്ദേശം(state);
		}

		എങ്കിൽ (state->interactive && do_interactive(state))
			പോകുക next;

		to_keep = 0;
		എങ്കിൽ (is_empty_or_missing_file(am_path(state, "patch"))) {
			തിരഞ്ഞെടുപ്പ് (state->empty_type) {
			അവസ്ഥ DROP_EMPTY_COMMIT:
				say(state, stdout, _("Skipping: %.*s"), linelen(state->സന്ദേശം), state->സന്ദേശം);
				പോകുക next;
				നിർത്തുക;
			അവസ്ഥ KEEP_EMPTY_COMMIT:
				to_keep = 1;
				say(state, stdout, _("Creating an empty commit: %.*s"),
					linelen(state->സന്ദേശം), state->സന്ദേശം);
				നിർത്തുക;
			അവസ്ഥ STOP_ON_EMPTY_COMMIT:
				printf_ln(_("Patch is empty."));
				die_user_resolve(state);
				നിർത്തുക;
			}
		}

		എങ്കിൽ (run_applypatch_സന്ദേശം_hook(state))
			exit(1);
		എങ്കിൽ (to_keep)
			പോകുക commit;

		say(state, stdout, _("Applying: %.*s"), linelen(state->സന്ദേശം), state->സന്ദേശം);

		apply_status = run_apply(state, ശൂന്യ);

		എങ്കിൽ (apply_status && state->threeway) {
			ഘടന strbuf sb = STRBUF_INIT;

			strbuf_addstr(&sb, am_path(state, "patch-merge-index"));
			apply_status = fall_back_threeway(state, sb.buf);
			strbuf_release(&sb);

			/*
			 * Applying the patch to an earlier tree and merging
			 * the result may have produced the same tree as ours.
			 */
			എങ്കിൽ (!apply_status &&
			    !repo_index_has_changes(the_repository, ശൂന്യ, ശൂന്യ)) {
				say(state, stdout, _("No changes -- Patch already applied."));
				പോകുക next;
			}
		}

		എങ്കിൽ (apply_status) {
			printf_ln(_("Patch failed at %s %.*s"), msgnum(state),
				linelen(state->സന്ദേശം), state->സന്ദേശം);

			എങ്കിൽ (advice_enabled(ADVICE_AM_WORK_DIR))
				advise(_("Use 'git am --show-current-patch=diff' to see the failed patch"));

			die_user_resolve(state);
		}

commit:
		do_commit(state);

next:
		am_next(state);

		എങ്കിൽ (resume)
			am_load(state);
		resume = 0;
	}

	എങ്കിൽ (!is_empty_or_missing_file(am_path(state, "rewritten"))) {
		പ്രതിജ്ഞാ(state->rebasing);
		copy_notes_for_rebase(state);
		run_post_rewrite_hook(state);
	}

	/*
	 * In rebasing mode, it's up to the caller to take care of
	 * housekeeping.
	 */
	എങ്കിൽ (!state->rebasing) {
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
സ്ഥിതം ശൂന്യം am_resolve(ഘടന am_state *state, പൂർണ്ണസംഖ്യ allow_empty)
{
	validate_resume_state(state);

	say(state, stdout, _("Applying: %.*s"), linelen(state->സന്ദേശം), state->സന്ദേശം);

	എങ്കിൽ (!repo_index_has_changes(the_repository, ശൂന്യ, ശൂന്യ)) {
		എങ്കിൽ (allow_empty && is_empty_or_missing_file(am_path(state, "patch"))) {
			printf_ln(_("No changes - recorded it as an empty commit."));
		} അല്ലെങ്കിൽ {
			printf_ln(_("No changes - did you forget to use 'git add'?\n"
				    "If there is nothing left to stage, chances are that something else\n"
				    "already introduced the same changes; you might want to skip this patch."));
			die_user_resolve(state);
		}
	}

	എങ്കിൽ (unmerged_index(the_repository->index)) {
		printf_ln(_("You still have unmerged paths in your index.\n"
			"You should 'git add' each file with resolved conflicts to mark them as such.\n"
			"You might run `git rm` on a file to accept \"deleted by them\" for it."));
		die_user_resolve(state);
	}

	എങ്കിൽ (state->interactive) {
		write_index_patch(state);
		എങ്കിൽ (do_interactive(state))
			പോകുക next;
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
സ്ഥിതം പൂർണ്ണസംഖ്യ fast_forward_to(ഘടന വൃക്ഷം *head, ഘടന വൃക്ഷം *remote, പൂർണ്ണസംഖ്യ reset)
{
	ഘടന lock_file lock_file = LOCK_INIT;
	ഘടന unpack_trees_options opts;
	ഘടന വൃക്ഷം_desc t[2];

	എങ്കിൽ (repo_parse_വൃക്ഷം(the_repository, head) || repo_parse_വൃക്ഷം(the_repository, remote))
		തിരികെ -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	refresh_index(the_repository->index, REFRESH_QUIET, ശൂന്യ, ശൂന്യ, ശൂന്യ);

	memset(&opts, 0, വലുപ്പം(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.update = 1;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = twoway_merge;
	init_വൃക്ഷം_desc(&t[0], &head->object.oid, head->buffer, head->size);
	init_വൃക്ഷം_desc(&t[1], &remote->object.oid, remote->buffer, remote->size);

	എങ്കിൽ (unpack_trees(2, t, &opts)) {
		rollback_lock_file(&lock_file);
		തിരികെ -1;
	}

	എങ്കിൽ (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	തിരികെ 0;
}

/**
 * Merges a tree into the index. The index's stat info will take precedence
 * over the merged tree's. Returns 0 on success, -1 on failure.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ merge_വൃക്ഷം(ഘടന വൃക്ഷം *വൃക്ഷം)
{
	ഘടന lock_file lock_file = LOCK_INIT;
	ഘടന unpack_trees_options opts;
	ഘടന വൃക്ഷം_desc t[1];

	എങ്കിൽ (repo_parse_വൃക്ഷം(the_repository, വൃക്ഷം))
		തിരികെ -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, വലുപ്പം(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.merge = 1;
	opts.fn = oneway_merge;
	init_വൃക്ഷം_desc(&t[0], &വൃക്ഷം->object.oid, വൃക്ഷം->buffer, വൃക്ഷം->size);

	എങ്കിൽ (unpack_trees(1, t, &opts)) {
		rollback_lock_file(&lock_file);
		തിരികെ -1;
	}

	എങ്കിൽ (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	തിരികെ 0;
}

/**
 * Clean the index without touching entries that are not modified between
 * `head` and `remote`.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ clean_index(സ്ഥിരം ഘടന object_id *head, സ്ഥിരം ഘടന object_id *remote)
{
	ഘടന വൃക്ഷം *head_വൃക്ഷം, *remote_വൃക്ഷം, *index_വൃക്ഷം;
	ഘടന object_id index;

	head_വൃക്ഷം = repo_parse_വൃക്ഷം_indirect(the_repository, head);
	എങ്കിൽ (!head_വൃക്ഷം)
		തിരികെ error(_("Could not parse object '%s'."), oid_to_hex(head));

	remote_വൃക്ഷം = repo_parse_വൃക്ഷം_indirect(the_repository, remote);
	എങ്കിൽ (!remote_വൃക്ഷം)
		തിരികെ error(_("Could not parse object '%s'."), oid_to_hex(remote));

	repo_read_index_unmerged(the_repository);

	എങ്കിൽ (fast_forward_to(head_വൃക്ഷം, head_വൃക്ഷം, 1))
		തിരികെ -1;

	എങ്കിൽ (write_index_as_വൃക്ഷം(&index, the_repository->index,
				repo_get_index_file(the_repository),
				0, ശൂന്യ))
		തിരികെ -1;

	index_വൃക്ഷം = repo_parse_വൃക്ഷം_indirect(the_repository, &index);
	എങ്കിൽ (!index_വൃക്ഷം)
		തിരികെ error(_("Could not parse object '%s'."), oid_to_hex(&index));

	എങ്കിൽ (fast_forward_to(index_വൃക്ഷം, remote_വൃക്ഷം, 0))
		തിരികെ -1;

	എങ്കിൽ (merge_വൃക്ഷം(remote_വൃക്ഷം))
		തിരികെ -1;

	remove_branch_state(the_repository, 0);

	തിരികെ 0;
}

/**
 * Resets rerere's merge resolution metadata.
 */
സ്ഥിതം ശൂന്യം am_rerere_clear(ശൂന്യം)
{
	ഘടന string_list merge_rr = STRING_LIST_INIT_DUP;
	rerere_clear(the_repository, &merge_rr);
	string_list_clear(&merge_rr, 1);
}

/**
 * Resume the current am session by skipping the current patch.
 */
സ്ഥിതം ശൂന്യം am_skip(ഘടന am_state *state)
{
	ഘടന object_id head;

	am_rerere_clear();

	എങ്കിൽ (repo_get_oid(the_repository, "HEAD", &head))
		oidcpy(&head, the_hash_algo->empty_വൃക്ഷം);

	എങ്കിൽ (clean_index(&head, &head))
		die(_("failed to clean index"));

	എങ്കിൽ (state->rebasing) {
		FILE *fp = xfopen(am_path(state, "rewritten"), "a");

		പ്രതിജ്ഞാ(!is_null_oid(&state->orig_commit));
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
സ്ഥിതം പൂർണ്ണസംഖ്യ safe_to_abort(സ്ഥിരം ഘടന am_state *state)
{
	ഘടന strbuf sb = STRBUF_INIT;
	ഘടന object_id abort_safety, head;

	എങ്കിൽ (file_exists(am_path(state, "dirtyindex")))
		തിരികെ 0;

	എങ്കിൽ (read_state_file(&sb, state, "abort-safety", 1) > 0) {
		എങ്കിൽ (get_oid_hex(sb.buf, &abort_safety))
			die(_("could not parse %s"), am_path(state, "abort-safety"));
	} അല്ലെങ്കിൽ
		oidclr(&abort_safety, the_repository->hash_algo);
	strbuf_release(&sb);

	എങ്കിൽ (repo_get_oid(the_repository, "HEAD", &head))
		oidclr(&head, the_repository->hash_algo);

	എങ്കിൽ (oideq(&head, &abort_safety))
		തിരികെ 1;

	warning(_("You seem to have moved HEAD since the last 'am' failure.\n"
		"Not rewinding to ORIG_HEAD"));

	തിരികെ 0;
}

/**
 * Aborts the current am session if it is safe to do so.
 */
സ്ഥിതം ശൂന്യം am_abort(ഘടന am_state *state)
{
	ഘടന object_id curr_head, orig_head;
	പൂർണ്ണസംഖ്യ has_curr_head, has_orig_head;
	അക്ഷരം *curr_branch;

	എങ്കിൽ (!safe_to_abort(state)) {
		am_destroy(state);
		തിരികെ;
	}

	am_rerere_clear();

	curr_branch = refs_resolve_refdup(get_main_ref_store(the_repository),
					  "HEAD", 0, &curr_head, ശൂന്യ);
	has_curr_head = curr_branch && !is_null_oid(&curr_head);
	എങ്കിൽ (!has_curr_head)
		oidcpy(&curr_head, the_hash_algo->empty_വൃക്ഷം);

	has_orig_head = !repo_get_oid(the_repository, "ORIG_HEAD", &orig_head);
	എങ്കിൽ (!has_orig_head)
		oidcpy(&orig_head, the_hash_algo->empty_വൃക്ഷം);

	എങ്കിൽ (clean_index(&curr_head, &orig_head))
		die(_("failed to clean index"));

	എങ്കിൽ (has_orig_head)
		refs_update_ref(get_main_ref_store(the_repository),
				"am --abort", "HEAD", &orig_head,
				has_curr_head ? &curr_head : ശൂന്യ, 0,
				UPDATE_REFS_DIE_ON_ERR);
	അല്ലെങ്കിൽ എങ്കിൽ (curr_branch)
		refs_delete_ref(get_main_ref_store(the_repository), ശൂന്യ,
				curr_branch, ശൂന്യ, REF_NO_DEREF);

	സ്വതന്ത്രം(curr_branch);
	am_destroy(state);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_patch(ഘടന am_state *state, എണ്ണൽ resume_type resume_mode)
{
	ഘടന strbuf sb = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *patch_path;
	പൂർണ്ണസംഖ്യ len;

	എങ്കിൽ (!is_null_oid(&state->orig_commit)) {
		ഘടന child_process cmd = CHILD_PROCESS_INIT;

		strvec_pushl(&cmd.args, "show", oid_to_hex(&state->orig_commit),
			     "--", ശൂന്യ);
		cmd.git_cmd = 1;
		തിരികെ run_command(&cmd);
	}

	തിരഞ്ഞെടുപ്പ് (resume_mode) {
	അവസ്ഥ RESUME_SHOW_PATCH_RAW:
		patch_path = am_path(state, msgnum(state));
		നിർത്തുക;
	അവസ്ഥ RESUME_SHOW_PATCH_DIFF:
		patch_path = am_path(state, "patch");
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("invalid mode for --show-current-patch");
	}

	len = strbuf_read_file(&sb, patch_path, 0);
	എങ്കിൽ (len < 0)
		die_errno(_("failed to read '%s'"), patch_path);

	setup_pager(the_repository);
	write_in_full(1, sb.buf, sb.len);
	strbuf_release(&sb);
	തിരികെ 0;
}

/**
 * parse_options() callback that validates and sets opt->value to the
 * PATCH_FORMAT_* enum value corresponding to `arg`.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ parse_opt_patchformat(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	പൂർണ്ണസംഖ്യ *opt_മൂല്യം = opt->മൂല്യം;

	എങ്കിൽ (unset)
		*opt_മൂല്യം = PATCH_FORMAT_UNKNOWN;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "mbox"))
		*opt_മൂല്യം = PATCH_FORMAT_MBOX;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "stgit"))
		*opt_മൂല്യം = PATCH_FORMAT_STGIT;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "stgit-series"))
		*opt_മൂല്യം = PATCH_FORMAT_STGIT_SERIES;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "hg"))
		*opt_മൂല്യം = PATCH_FORMAT_HG;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "mboxrd"))
		*opt_മൂല്യം = PATCH_FORMAT_MBOXRD;
	/*
	 * Please update $__git_patchformat in git-completion.bash
	 * when you add new options
	 */
	അല്ലെങ്കിൽ
		തിരികെ error(_("invalid value for '%s': '%s'"),
			     "--patch-format", arg);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_opt_show_current_patch(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	പൂർണ്ണസംഖ്യ *opt_മൂല്യം = opt->മൂല്യം;

	BUG_ON_OPT_NEG(unset);

	എങ്കിൽ (!arg)
		*opt_മൂല്യം = opt->defval;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "raw"))
		*opt_മൂല്യം = RESUME_SHOW_PATCH_RAW;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "diff"))
		*opt_മൂല്യം = RESUME_SHOW_PATCH_DIFF;
	/*
	 * Please update $__git_showcurrentpatch in git-completion.bash
	 * when you add new options
	 */
	അല്ലെങ്കിൽ
		തിരികെ error(_("invalid value for '%s': '%s'"),
			     "--show-current-patch", arg);
	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ cmd_am(പൂർണ്ണസംഖ്യ argc,
	   സ്ഥിരം അക്ഷരം **argv,
	   സ്ഥിരം അക്ഷരം *prefix,
	   ഘടന repository *repo UNUSED)
{
	ഘടന am_state state;
	പൂർണ്ണസംഖ്യ ദ്വിമാന = -1;
	പൂർണ്ണസംഖ്യ keep_cr = -1;
	പൂർണ്ണസംഖ്യ patch_format = PATCH_FORMAT_UNKNOWN;
	എണ്ണൽ resume_type resume_mode = RESUME_FALSE;
	പൂർണ്ണസംഖ്യ in_progress;
	പൂർണ്ണസംഖ്യ ret = 0;

	സ്ഥിരം അക്ഷരം * സ്ഥിരം usage[] = {
		N_("git am [<options>] [(<mbox> | <Maildir>)...]"),
		N_("git am [<options>] (--continue | --skip | --abort)"),
		ശൂന്യ
	};

	ഘടന option options[] = {
		OPT_BOOL('i', "interactive", &state.interactive,
			N_("run interactively")),
		OPT_BOOL('n', "no-verify", &state.no_verify,
			N_("bypass pre-applypatch and applypatch-msg hooks")),
		OPT_HIDDEN_BOOL('b', "binary", &ദ്വിമാന,
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
		OPT_PASSTHRU_ARGV(0, "ignore-space-change", &state.git_apply_opts, ശൂന്യ,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU_ARGV(0, "ignore-whitespace", &state.git_apply_opts, ശൂന്യ,
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
		OPT_PASSTHRU_ARGV('C', ശൂന്യ, &state.git_apply_opts, N_("n"),
			N_("pass it through git-apply"),
			0),
		OPT_PASSTHRU_ARGV('p', ശൂന്യ, &state.git_apply_opts, N_("num"),
			N_("pass it through git-apply"),
			0),
		OPT_CALLBACK(0, "patch-format", &patch_format, N_("format"),
			N_("format the patch(es) are in"),
			parse_opt_patchformat),
		OPT_PASSTHRU_ARGV(0, "reject", &state.git_apply_opts, ശൂന്യ,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_STRING(0, "resolvemsg", &state.resolvemsg, ശൂന്യ,
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
			.മൂല്യം = &resume_mode,
			.precision = വലുപ്പം(resume_mode),
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
			.മൂല്യം = &state.sign_commit,
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

	repo_config(the_repository, git_default_config, ശൂന്യ);

	am_state_init(&state);

	in_progress = am_in_progress(&state);
	എങ്കിൽ (in_progress)
		am_load(&state);

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	എങ്കിൽ (ദ്വിമാന >= 0)
		fprintf_ln(stderr, _("The -b/--binary option has been a no-op for long time, and\n"
				"it will be removed. Please do not use it anymore."));

	/* Ensure a valid committer ident can be constructed */
	git_committer_info(IDENT_STRICT);

	എങ്കിൽ (repo_read_index_preload(the_repository, ശൂന്യ, 0) < 0)
		die(_("failed to read the index"));

	എങ്കിൽ (in_progress) {
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
		എങ്കിൽ (argc || (resume_mode == RESUME_FALSE && !isatty(0)))
			die(_("previous rebase directory %s still exists but mbox given."),
				state.dir);

		എങ്കിൽ (resume_mode == RESUME_FALSE)
			resume_mode = RESUME_APPLY;

		എങ്കിൽ (state.signoff == SIGNOFF_EXPLICIT)
			am_append_signoff(&state);
	} അല്ലെങ്കിൽ {
		ഘടന strvec paths = STRVEC_INIT;
		പൂർണ്ണസംഖ്യ i;

		/*
		 * Handle stray state directory in the independent-run case. In
		 * the --rebasing case, it is up to the caller to take care of
		 * stray directories.
		 */
		എങ്കിൽ (file_exists(state.dir) && !state.rebasing) {
			എങ്കിൽ (resume_mode == RESUME_ABORT || resume_mode == RESUME_QUIT) {
				am_destroy(&state);
				am_state_release(&state);
				തിരികെ 0;
			}

			die(_("Stray %s directory found.\n"
				"Use \"git am --abort\" to remove it."),
				state.dir);
		}

		എങ്കിൽ (resume_mode)
			die(_("Resolve operation not in progress, we are not resuming."));

		വേണ്ടി (i = 0; i < argc; i++) {
			എങ്കിൽ (is_absolute_path(argv[i]) || !prefix)
				strvec_തള്ളുക(&paths, argv[i]);
			അല്ലെങ്കിൽ
				strvec_തള്ളുക(&paths, mkpath("%s/%s", prefix, argv[i]));
		}

		എങ്കിൽ (state.interactive && !paths.nr)
			die(_("interactive mode requires patches on the command line"));

		am_setup(&state, patch_format, paths.v, keep_cr);

		strvec_clear(&paths);
	}

	തിരഞ്ഞെടുപ്പ് (resume_mode) {
	അവസ്ഥ RESUME_FALSE:
		am_run(&state, 0);
		നിർത്തുക;
	അവസ്ഥ RESUME_APPLY:
		am_run(&state, 1);
		നിർത്തുക;
	അവസ്ഥ RESUME_RESOLVED:
	അവസ്ഥ RESUME_ALLOW_EMPTY:
		am_resolve(&state, resume_mode == RESUME_ALLOW_EMPTY ? 1 : 0);
		നിർത്തുക;
	അവസ്ഥ RESUME_SKIP:
		am_skip(&state);
		നിർത്തുക;
	അവസ്ഥ RESUME_ABORT:
		am_abort(&state);
		നിർത്തുക;
	അവസ്ഥ RESUME_QUIT:
		am_rerere_clear();
		am_destroy(&state);
		നിർത്തുക;
	അവസ്ഥ RESUME_SHOW_PATCH_RAW:
	അവസ്ഥ RESUME_SHOW_PATCH_DIFF:
		ret = show_patch(&state, resume_mode);
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		BUG("invalid resume value");
	}

	am_state_release(&state);

	തിരികെ ret;
}
