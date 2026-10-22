/*
 * Builtin "git am"
 *
 * Based on git-am.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "advice.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "editor.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "hook.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "tempfile.h"
#ಅನ್ತರ್ಭಾವ "lockfile.h"
#ಅನ್ತರ್ಭಾವ "cache-tree.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "unpack-trees.h"
#ಅನ್ತರ್ಭಾವ "branch.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "preload-index.h"
#ಅನ್ತರ್ಭಾವ "sequencer.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "merge-ort-wrappers.h"
#ಅನ್ತರ್ಭಾವ "log-tree.h"
#ಅನ್ತರ್ಭಾವ "notes-utils.h"
#ಅನ್ತರ್ಭಾವ "rerere.h"
#ಅನ್ತರ್ಭಾವ "mailinfo.h"
#ಅನ್ತರ್ಭಾವ "apply.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "pager.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "pretty.h"

/**
 * Returns the length of the first line of msg.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ linelen(ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ)
{
	ಪ್ರತ್ಯಾಗಮ strchrnul(ಸಂದೇಶ, '\n') - ಸಂದೇಶ;
}

/**
 * Returns true if `str` consists of only whitespace, false otherwise.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ str_isspace(ಧ್ರುವ ವರ್ಣ *str)
{
	ಪ್ರತಿ (; *str; str++)
		ಯದಿ (!isspace(*str))
			ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತ್ಯಾಗಮ 1;
}

ಗಣನಾ patch_format {
	PATCH_FORMAT_UNKNOWN = 0,
	PATCH_FORMAT_MBOX,
	PATCH_FORMAT_STGIT,
	PATCH_FORMAT_STGIT_SERIES,
	PATCH_FORMAT_HG,
	PATCH_FORMAT_MBOXRD
};

ಗಣನಾ keep_type {
	KEEP_FALSE = 0,
	KEEP_TRUE,      /* pass -k flag to git-mailinfo */
	KEEP_NON_PATCH  /* pass -b flag to git-mailinfo */
};

ಗಣನಾ scissors_type {
	SCISSORS_UNSET = -1,
	SCISSORS_FALSE = 0,  /* pass --no-scissors to git-mailinfo */
	SCISSORS_TRUE        /* pass --scissors to git-mailinfo */
};

ಗಣನಾ signoff_type {
	SIGNOFF_FALSE = 0,
	SIGNOFF_TRUE = 1,
	SIGNOFF_EXPLICIT /* --signoff was set on the command-line */
};

ಗಣನಾ resume_type {
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

ಗಣನಾ empty_action {
	STOP_ON_EMPTY_COMMIT = 0,  /* output errors and stop in the middle of an am session */
	DROP_EMPTY_COMMIT,         /* skip with a notice message, unless "--quiet" has been passed */
	KEEP_EMPTY_COMMIT,         /* keep recording as empty commits */
};

ಸಂರಚನಾ am_state {
	/* state directory path */
	ವರ್ಣ *dir;

	/* current and last patch numbers, 1-indexed */
	ಪೂರ್ಣಾಙ್ಕ cur;
	ಪೂರ್ಣಾಙ್ಕ last;

	/* commit metadata and message */
	ವರ್ಣ *author_name;
	ವರ್ಣ *author_email;
	ವರ್ಣ *author_date;
	ವರ್ಣ *ಸಂದೇಶ;
	size_t ಸಂದೇಶ_len;

	/* when --rebasing, records the original commit the patch came from */
	ಸಂರಚನಾ object_id orig_commit;

	/* number of digits in patch filename */
	ಪೂರ್ಣಾಙ್ಕ prec;

	/* various operating modes and command line options */
	ಪೂರ್ಣಾಙ್ಕ interactive;
	ಪೂರ್ಣಾಙ್ಕ no_verify;
	ಪೂರ್ಣಾಙ್ಕ threeway;
	ಪೂರ್ಣಾಙ್ಕ quiet;
	ಪೂರ್ಣಾಙ್ಕ signoff; /* enum signoff_type */
	ಪೂರ್ಣಾಙ್ಕ utf8;
	ಪೂರ್ಣಾಙ್ಕ keep; /* enum keep_type */
	ಪೂರ್ಣಾಙ್ಕ message_id;
	ಪೂರ್ಣಾಙ್ಕ scissors; /* enum scissors_type */
	ಪೂರ್ಣಾಙ್ಕ quoted_cr; /* enum quoted_cr_action */
	ಪೂರ್ಣಾಙ್ಕ empty_type; /* enum empty_action */
	ಸಂರಚನಾ strvec git_apply_opts;
	ಧ್ರುವ ವರ್ಣ *resolvemsg;
	ಪೂರ್ಣಾಙ್ಕ committer_date_is_author_date;
	ಪೂರ್ಣಾಙ್ಕ ignore_date;
	ಪೂರ್ಣಾಙ್ಕ allow_rerere_autoupdate;
	ಧ್ರುವ ವರ್ಣ *sign_commit;
	ಪೂರ್ಣಾಙ್ಕ rebasing;
};

/**
 * Initializes am_state with the default values.
 */
ಸ್ಥಿರ ಶೂನ್ಯ am_state_init(ಸಂರಚನಾ am_state *state)
{
	ಪೂರ್ಣಾಙ್ಕ gpgsign;

	memset(state, 0, ಪರಿಮಾಣ(*state));

	state->dir = repo_git_path(the_repository, "rebase-apply");

	state->prec = 4;

	repo_config_get_bool(the_repository, "am.threeway", &state->threeway);

	state->utf8 = 1;

	repo_config_get_bool(the_repository, "am.messageid", &state->message_id);

	state->scissors = SCISSORS_UNSET;
	state->quoted_cr = quoted_cr_unset;

	strvec_init(&state->git_apply_opts);

	ಯದಿ (!repo_config_get_bool(the_repository, "commit.gpgsign", &gpgsign))
		state->sign_commit = gpgsign ? "" : ರಿಕ್ತ;
}

/**
 * Releases memory allocated by an am_state.
 */
ಸ್ಥಿರ ಶೂನ್ಯ am_state_release(ಸಂರಚನಾ am_state *state)
{
	ಮುಕ್ತ(state->dir);
	ಮುಕ್ತ(state->author_name);
	ಮುಕ್ತ(state->author_email);
	ಮುಕ್ತ(state->author_date);
	ಮುಕ್ತ(state->ಸಂದೇಶ);
	strvec_clear(&state->git_apply_opts);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ am_option_parse_quoted_cr(ಧ್ರುವ ಸಂರಚನಾ option *opt,
				     ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	BUG_ON_OPT_NEG(unset);

	ಯದಿ (mailinfo_parse_quoted_cr_action(arg, opt->ಮೌಲ್ಯ) != 0)
		ಪ್ರತ್ಯಾಗಮ error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ am_option_parse_empty(ಧ್ರುವ ಸಂರಚನಾ option *opt,
				     ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಪೂರ್ಣಾಙ್ಕ *opt_ಮೌಲ್ಯ = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	ಯದಿ (!strcmp(arg, "stop"))
		*opt_ಮೌಲ್ಯ = STOP_ON_EMPTY_COMMIT;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "drop"))
		*opt_ಮೌಲ್ಯ = DROP_EMPTY_COMMIT;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "keep"))
		*opt_ಮೌಲ್ಯ = KEEP_EMPTY_COMMIT;
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ error(_("invalid value for '%s': '%s'"), "--empty", arg);

	ಪ್ರತ್ಯಾಗಮ 0;
}

/**
 * Returns path relative to the am_state directory.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ ವರ್ಣ *am_path(ಧ್ರುವ ಸಂರಚನಾ am_state *state, ಧ್ರುವ ವರ್ಣ *path)
{
	ಪ್ರತ್ಯಾಗಮ mkpath("%s/%s", state->dir, path);
}

/**
 * For convenience to call write_file()
 */
ಸ್ಥಿರ ಶೂನ್ಯ write_state_text(ಧ್ರುವ ಸಂರಚನಾ am_state *state,
			     ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *string)
{
	write_file(am_path(state, name), "%s", string);
}

ಸ್ಥಿರ ಶೂನ್ಯ write_state_ಎಣಿಕೆ(ಧ್ರುವ ಸಂರಚನಾ am_state *state,
			      ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ)
{
	write_file(am_path(state, name), "%d", ಮೌಲ್ಯ);
}

ಸ್ಥಿರ ಶೂನ್ಯ write_state_bool(ಧ್ರುವ ಸಂರಚನಾ am_state *state,
			     ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ)
{
	write_state_text(state, name, ಮೌಲ್ಯ ? "t" : "f");
}

/**
 * If state->quiet is false, calls fprintf(fp, fmt, ...), and appends a newline
 * at the end.
 */
__attribute__((format (printf, 3, 4)))
ಸ್ಥಿರ ಶೂನ್ಯ say(ಧ್ರುವ ಸಂರಚನಾ am_state *state, FILE *fp, ಧ್ರುವ ವರ್ಣ *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	ಯದಿ (!state->quiet) {
		vfprintf(fp, fmt, ap);
		putc('\n', fp);
	}
	va_end(ap);
}

/**
 * Returns 1 if there is an am session in progress, 0 otherwise.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ am_in_progress(ಧ್ರುವ ಸಂರಚನಾ am_state *state)
{
	ಸಂರಚನಾ stat st;

	ಯದಿ (lstat(state->dir, &st) < 0 || !S_ISDIR(st.st_mode))
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (lstat(am_path(state, "last"), &st) || !S_ISREG(st.st_mode))
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (lstat(am_path(state, "next"), &st) || !S_ISREG(st.st_mode))
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ 1;
}

/**
 * Reads the contents of `file` in the `state` directory into `sb`. Returns the
 * number of bytes read on success, -1 if the file does not exist. If `trim` is
 * set, trailing whitespace will be removed.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ read_state_file(ಸಂರಚನಾ strbuf *sb, ಧ್ರುವ ಸಂರಚನಾ am_state *state,
			ಧ್ರುವ ವರ್ಣ *file, ಪೂರ್ಣಾಙ್ಕ trim)
{
	strbuf_reset(sb);

	ಯದಿ (strbuf_read_file(sb, am_path(state, file), 0) >= 0) {
		ಯದಿ (trim)
			strbuf_trim(sb);

		ಪ್ರತ್ಯಾಗಮ sb->len;
	}

	ಯದಿ (errno == ENOENT)
		ಪ್ರತ್ಯಾಗಮ -1;

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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ read_am_author_script(ಸಂರಚನಾ am_state *state)
{
	ಧ್ರುವ ವರ್ಣ *filename = am_path(state, "author-script");

	ಪ್ರತಿಜ್ಞಾ(!state->author_name);
	ಪ್ರತಿಜ್ಞಾ(!state->author_email);
	ಪ್ರತಿಜ್ಞಾ(!state->author_date);

	ಪ್ರತ್ಯಾಗಮ read_author_script(filename, &state->author_name,
				  &state->author_email, &state->author_date, 1);
}

/**
 * Saves state->author_name, state->author_email and state->author_date in the
 * state directory's "author-script" file.
 */
ಸ್ಥಿರ ಶೂನ್ಯ write_author_script(ಧ್ರುವ ಸಂರಚನಾ am_state *state)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ read_commit_ಸಂದೇಶ(ಸಂರಚನಾ am_state *state)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

	ಪ್ರತಿಜ್ಞಾ(!state->ಸಂದೇಶ);

	ಯದಿ (read_state_file(&sb, state, "final-commit", 0) < 0) {
		strbuf_release(&sb);
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	state->ಸಂದೇಶ = strbuf_detach(&sb, &state->ಸಂದೇಶ_len);
	ಪ್ರತ್ಯಾಗಮ 0;
}

/**
 * Saves state->msg in the state directory's "final-commit" file.
 */
ಸ್ಥಿರ ಶೂನ್ಯ write_commit_ಸಂದೇಶ(ಧ್ರುವ ಸಂರಚನಾ am_state *state)
{
	ಧ್ರುವ ವರ್ಣ *filename = am_path(state, "final-commit");
	write_file_buf(filename, state->ಸಂದೇಶ, state->ಸಂದೇಶ_len);
}

/**
 * Loads state from disk.
 */
ಸ್ಥಿರ ಶೂನ್ಯ am_load(ಸಂರಚನಾ am_state *state)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

	ಯದಿ (read_state_file(&sb, state, "next", 1) < 0)
		BUG("state file 'next' does not exist");
	state->cur = strtol(sb.buf, ರಿಕ್ತ, 10);

	ಯದಿ (read_state_file(&sb, state, "last", 1) < 0)
		BUG("state file 'last' does not exist");
	state->last = strtol(sb.buf, ರಿಕ್ತ, 10);

	ಯದಿ (read_am_author_script(state) < 0)
		die(_("could not parse author script"));

	read_commit_ಸಂದೇಶ(state);

	ಯದಿ (read_state_file(&sb, state, "original-commit", 1) < 0)
		oidclr(&state->orig_commit, the_repository->hash_algo);
	ಅನ್ಯಥಾ ಯದಿ (get_oid_hex(sb.buf, &state->orig_commit) < 0)
		die(_("could not parse %s"), am_path(state, "original-commit"));

	read_state_file(&sb, state, "threeway", 1);
	state->threeway = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "quiet", 1);
	state->quiet = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "sign", 1);
	state->signoff = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "utf8", 1);
	state->utf8 = !strcmp(sb.buf, "t");

	ಯದಿ (file_exists(am_path(state, "rerere-autoupdate"))) {
		read_state_file(&sb, state, "rerere-autoupdate", 1);
		state->allow_rerere_autoupdate = strcmp(sb.buf, "t") ?
			RERERE_NOAUTOUPDATE : RERERE_AUTOUPDATE;
	} ಅನ್ಯಥಾ {
		state->allow_rerere_autoupdate = 0;
	}

	read_state_file(&sb, state, "keep", 1);
	ಯದಿ (!strcmp(sb.buf, "t"))
		state->keep = KEEP_TRUE;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(sb.buf, "b"))
		state->keep = KEEP_NON_PATCH;
	ಅನ್ಯಥಾ
		state->keep = KEEP_FALSE;

	read_state_file(&sb, state, "messageid", 1);
	state->message_id = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "scissors", 1);
	ಯದಿ (!strcmp(sb.buf, "t"))
		state->scissors = SCISSORS_TRUE;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(sb.buf, "f"))
		state->scissors = SCISSORS_FALSE;
	ಅನ್ಯಥಾ
		state->scissors = SCISSORS_UNSET;

	read_state_file(&sb, state, "quoted-cr", 1);
	ಯದಿ (!*sb.buf)
		state->quoted_cr = quoted_cr_unset;
	ಅನ್ಯಥಾ ಯದಿ (mailinfo_parse_quoted_cr_action(sb.buf, &state->quoted_cr) != 0)
		die(_("could not parse %s"), am_path(state, "quoted-cr"));

	read_state_file(&sb, state, "apply-opt", 1);
	strvec_clear(&state->git_apply_opts);
	ಯದಿ (sq_dequote_to_strvec(sb.buf, &state->git_apply_opts) < 0)
		die(_("could not parse %s"), am_path(state, "apply-opt"));

	state->rebasing = !!file_exists(am_path(state, "rebasing"));

	strbuf_release(&sb);
}

/**
 * Removes the am_state directory, forcefully terminating the current am
 * session.
 */
ಸ್ಥಿರ ಶೂನ್ಯ am_destroy(ಧ್ರುವ ಸಂರಚನಾ am_state *state)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

	strbuf_addstr(&sb, state->dir);
	remove_dir_recursively(&sb, 0);
	strbuf_release(&sb);
}

/**
 * Runs applypatch-msg hook. Returns its exit code.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ run_applypatch_ಸಂದೇಶ_hook(ಸಂರಚನಾ am_state *state)
{
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	ಪ್ರತಿಜ್ಞಾ(state->ಸಂದೇಶ);

	ಯದಿ (!state->no_verify) {
		ಸಂರಚನಾ run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;
		strvec_ತಳ್ಳು(&opt.args, am_path(state, "final-commit"));
		ret = run_hooks_opt(the_repository, "applypatch-msg", &opt);
	}

	ಯದಿ (!ret) {
		FREE_AND_NULL(state->ಸಂದೇಶ);
		ಯದಿ (read_commit_ಸಂದೇಶ(state) < 0)
			die(_("'%s' was deleted by the applypatch-msg hook"),
				am_path(state, "final-commit"));
	}

	ಪ್ರತ್ಯಾಗಮ ret;
}

/**
 * Runs post-rewrite hook. Returns it exit code.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ run_post_rewrite_hook(ಧ್ರುವ ಸಂರಚನಾ am_state *state)
{
	ಸಂರಚನಾ run_hooks_opt opt = RUN_HOOKS_OPT_INIT;

	strvec_ತಳ್ಳು(&opt.args, "rebase");
	opt.path_to_stdin = am_path(state, "rewritten");

	ಪ್ರತ್ಯಾಗಮ run_hooks_opt(the_repository, "post-rewrite", &opt);
}

/**
 * Reads the state directory's "rewritten" file, and copies notes from the old
 * commits listed in the file to their rewritten commits.
 *
 * Returns 0 on success, -1 on failure.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ copy_notes_for_rebase(ಧ್ರುವ ಸಂರಚನಾ am_state *state)
{
	ಸಂರಚನಾ notes_rewrite_cfg *c;
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *invalid_ಸಾಲು = _("Malformed input line: '%s'.");
	ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ = "Notes added by 'git rebase'";
	FILE *fp;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	ಪ್ರತಿಜ್ಞಾ(state->rebasing);

	c = init_copy_notes_for_rewrite("rebase");
	ಯದಿ (!c)
		ಪ್ರತ್ಯಾಗಮ 0;

	fp = xfopen(am_path(state, "rewritten"), "r");

	ಯಾವತ್ (!strbuf_getline_lf(&sb, fp)) {
		ಸಂರಚನಾ object_id from_obj, to_obj;
		ಧ್ರುವ ವರ್ಣ *p;

		ಯದಿ (sb.len != the_hash_algo->hexsz * 2 + 1) {
			ret = error(invalid_ಸಾಲು, sb.buf);
			ಗಚ್ಛ finish;
		}

		ಯದಿ (parse_oid_hex(sb.buf, &from_obj, &p)) {
			ret = error(invalid_ಸಾಲು, sb.buf);
			ಗಚ್ಛ finish;
		}

		ಯದಿ (*p != ' ') {
			ret = error(invalid_ಸಾಲು, sb.buf);
			ಗಚ್ಛ finish;
		}

		ಯದಿ (get_oid_hex(p + 1, &to_obj)) {
			ret = error(invalid_ಸಾಲು, sb.buf);
			ಗಚ್ಛ finish;
		}

		ಯದಿ (copy_note_for_rewrite(c, &from_obj, &to_obj))
			ret = error(_("Failed to copy notes from '%s' to '%s'"),
					oid_to_hex(&from_obj), oid_to_hex(&to_obj));
	}

finish:
	finish_copy_notes_for_rewrite(the_repository, c, ಸಂದೇಶ);
	fclose(fp);
	strbuf_release(&sb);
	ಪ್ರತ್ಯಾಗಮ ret;
}

/**
 * Determines if the file looks like a piece of RFC2822 mail by grabbing all
 * non-indented lines and checking if they look like they begin with valid
 * header field names.
 *
 * Returns 1 if the file looks like a piece of mail, 0 otherwise.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ is_mail(FILE *fp)
{
	ಧ್ರುವ ವರ್ಣ *header_regex = "^[!-9;-~]+:";
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	regex_t regex;
	ಪೂರ್ಣಾಙ್ಕ ret = 1;

	ಯದಿ (fseek(fp, 0L, SEEK_SET))
		die_errno(_("fseek failed"));

	ಯದಿ (regcomp(&regex, header_regex, REG_NOSUB | REG_EXTENDED))
		die("invalid pattern: %s", header_regex);

	ಯಾವತ್ (!strbuf_getline(&sb, fp)) {
		ಯದಿ (!sb.len)
			ಭಙ್ಗ; /* End of header */

		/* Ignore indented folded lines */
		ಯದಿ (*sb.buf == '\t' || *sb.buf == ' ')
			ಅನುವರ್ತ;

		/* It's a header if it matches header_regex */
		ಯದಿ (regexec(&regex, sb.buf, 0, ರಿಕ್ತ, 0)) {
			ret = 0;
			ಗಚ್ಛ done;
		}
	}

done:
	regfree(&regex);
	strbuf_release(&sb);
	ಪ್ರತ್ಯಾಗಮ ret;
}

/**
 * Attempts to detect the patch_format of the patches contained in `paths`,
 * returning the PATCH_FORMAT_* enum value. Returns PATCH_FORMAT_UNKNOWN if
 * detection fails.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ detect_patch_format(ಧ್ರುವ ವರ್ಣ **paths)
{
	ಗಣನಾ patch_format ret = PATCH_FORMAT_UNKNOWN;
	ಸಂರಚನಾ strbuf l1 = STRBUF_INIT;
	ಸಂರಚನಾ strbuf l2 = STRBUF_INIT;
	ಸಂರಚನಾ strbuf l3 = STRBUF_INIT;
	FILE *fp;

	/*
	 * We default to mbox format if input is from stdin and for directories
	 */
	ಯದಿ (!*paths || !strcmp(*paths, "-") || is_directory(*paths))
		ಪ್ರತ್ಯಾಗಮ PATCH_FORMAT_MBOX;

	/*
	 * Otherwise, check the first few lines of the first patch, starting
	 * from the first non-blank line, to try to detect its format.
	 */

	fp = xfopen(*paths, "r");

	ಯಾವತ್ (!strbuf_getline(&l1, fp)) {
		ಯದಿ (l1.len)
			ಭಙ್ಗ;
	}

	ಯದಿ (starts_with(l1.buf, "From ") || starts_with(l1.buf, "From: ")) {
		ret = PATCH_FORMAT_MBOX;
		ಗಚ್ಛ done;
	}

	ಯದಿ (starts_with(l1.buf, "# This series applies on GIT commit")) {
		ret = PATCH_FORMAT_STGIT_SERIES;
		ಗಚ್ಛ done;
	}

	ಯದಿ (!strcmp(l1.buf, "# HG changeset patch")) {
		ret = PATCH_FORMAT_HG;
		ಗಚ್ಛ done;
	}

	strbuf_getline(&l2, fp);
	strbuf_getline(&l3, fp);

	/*
	 * If the second line is empty and the third is a From, Author or Date
	 * entry, this is likely an StGit patch.
	 */
	ಯದಿ (l1.len && !l2.len &&
		(starts_with(l3.buf, "From:") ||
		 starts_with(l3.buf, "Author:") ||
		 starts_with(l3.buf, "Date:"))) {
		ret = PATCH_FORMAT_STGIT;
		ಗಚ್ಛ done;
	}

	ಯದಿ (l1.len && is_mail(fp)) {
		ret = PATCH_FORMAT_MBOX;
		ಗಚ್ಛ done;
	}

done:
	fclose(fp);
	strbuf_release(&l1);
	strbuf_release(&l2);
	strbuf_release(&l3);
	ಪ್ರತ್ಯಾಗಮ ret;
}

/**
 * Splits out individual email patches from `paths`, where each path is either
 * a mbox file or a Maildir. Returns 0 on success, -1 on failure.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ split_mail_mbox(ಸಂರಚನಾ am_state *state, ಧ್ರುವ ವರ್ಣ **paths,
				ಪೂರ್ಣಾಙ್ಕ keep_cr, ಪೂರ್ಣಾಙ್ಕ mboxrd)
{
	ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ strbuf last = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ ret;

	cp.git_cmd = 1;
	strvec_ತಳ್ಳು(&cp.args, "mailsplit");
	strvec_pushf(&cp.args, "-d%d", state->prec);
	strvec_pushf(&cp.args, "-o%s", state->dir);
	strvec_ತಳ್ಳು(&cp.args, "-b");
	ಯದಿ (keep_cr)
		strvec_ತಳ್ಳು(&cp.args, "--keep-cr");
	ಯದಿ (mboxrd)
		strvec_ತಳ್ಳು(&cp.args, "--mboxrd");
	strvec_ತಳ್ಳು(&cp.args, "--");
	strvec_pushv(&cp.args, paths);

	ret = capture_command(&cp, &last, 8);
	ಯದಿ (ret)
		ಗಚ್ಛ exit;

	state->cur = 1;
	state->last = strtol(last.buf, ರಿಕ್ತ, 10);

exit:
	strbuf_release(&last);
	ಪ್ರತ್ಯಾಗಮ ret ? -1 : 0;
}

/**
 * Callback signature for split_mail_conv(). The foreign patch should be
 * read from `in`, and the converted patch (in RFC2822 mail format) should be
 * written to `out`. Return 0 on success, or -1 on failure.
 */
ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ (*mail_conv_fn)(FILE *out, FILE *in, ಪೂರ್ಣಾಙ್ಕ keep_cr);

/**
 * Calls `fn` for each file in `paths` to convert the foreign patch to the
 * RFC2822 mail format suitable for parsing with git-mailinfo.
 *
 * Returns 0 on success, -1 on failure.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ split_mail_conv(mail_conv_fn fn, ಸಂರಚನಾ am_state *state,
			ಧ್ರುವ ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ keep_cr)
{
	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *stdin_only[] = {"-", ರಿಕ್ತ};
	ಪೂರ್ಣಾಙ್ಕ i;

	ಯದಿ (!*paths)
		paths = stdin_only;

	ಪ್ರತಿ (i = 0; *paths; paths++, i++) {
		FILE *in, *out;
		ಧ್ರುವ ವರ್ಣ *mail;
		ಪೂರ್ಣಾಙ್ಕ ret;

		ಯದಿ (!strcmp(*paths, "-"))
			in = stdin;
		ಅನ್ಯಥಾ
			in = fopen(*paths, "r");

		ಯದಿ (!in)
			ಪ್ರತ್ಯಾಗಮ error_errno(_("could not open '%s' for reading"),
					   *paths);

		mail = mkpath("%s/%0*d", state->dir, state->prec, i + 1);

		out = fopen(mail, "w");
		ಯದಿ (!out) {
			ಯದಿ (in != stdin)
				fclose(in);
			ಪ್ರತ್ಯಾಗಮ error_errno(_("could not open '%s' for writing"),
					   mail);
		}

		ret = fn(out, in, keep_cr);

		fclose(out);
		ಯದಿ (in != stdin)
			fclose(in);

		ಯದಿ (ret)
			ಪ್ರತ್ಯಾಗಮ error(_("could not parse patch '%s'"), *paths);
	}

	state->cur = 1;
	state->last = i;
	ಪ್ರತ್ಯಾಗಮ 0;
}

/**
 * A split_mail_conv() callback that converts an StGit patch to an RFC2822
 * message suitable for parsing with git-mailinfo.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ stgit_patch_to_mail(FILE *out, FILE *in, ಪೂರ್ಣಾಙ್ಕ keep_cr UNUSED)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ subject_printed = 0;

	ಯಾವತ್ (!strbuf_getline_lf(&sb, in)) {
		ಧ್ರುವ ವರ್ಣ *str;

		ಯದಿ (str_isspace(sb.buf))
			ಅನುವರ್ತ;
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(sb.buf, "Author:", &str))
			fprintf(out, "From:%s\n", str);
		ಅನ್ಯಥಾ ಯದಿ (starts_with(sb.buf, "From") || starts_with(sb.buf, "Date"))
			fprintf(out, "%s\n", sb.buf);
		ಅನ್ಯಥಾ ಯದಿ (!subject_printed) {
			fprintf(out, "Subject: %s\n", sb.buf);
			subject_printed = 1;
		} ಅನ್ಯಥಾ {
			fprintf(out, "\n%s\n", sb.buf);
			ಭಙ್ಗ;
		}
	}

	strbuf_reset(&sb);
	ಯಾವತ್ (strbuf_fread(&sb, 8192, in) > 0) {
		fwrite(sb.buf, 1, sb.len, out);
		strbuf_reset(&sb);
	}

	strbuf_release(&sb);
	ಪ್ರತ್ಯಾಗಮ 0;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ split_mail_stgit_series(ಸಂರಚನಾ am_state *state, ಧ್ರುವ ವರ್ಣ **paths,
					ಪೂರ್ಣಾಙ್ಕ keep_cr)
{
	ಧ್ರುವ ವರ್ಣ *series_dir;
	ವರ್ಣ *series_dir_buf;
	FILE *fp;
	ಸಂರಚನಾ strvec patches = STRVEC_INIT;
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಯದಿ (!paths[0] || paths[1])
		ಪ್ರತ್ಯಾಗಮ error(_("Only one StGIT patch series can be applied at once"));

	series_dir_buf = xstrdup(*paths);
	series_dir = dirname(series_dir_buf);

	fp = fopen(*paths, "r");
	ಯದಿ (!fp) {
		ಮುಕ್ತ(series_dir_buf);
		ಪ್ರತ್ಯಾಗಮ error_errno(_("could not open '%s' for reading"), *paths);
	}

	ಯಾವತ್ (!strbuf_getline_lf(&sb, fp)) {
		ಯದಿ (*sb.buf == '#')
			ಅನುವರ್ತ; /* skip comment lines */

		strvec_ತಳ್ಳು(&patches, mkpath("%s/%s", series_dir, sb.buf));
	}

	fclose(fp);
	strbuf_release(&sb);
	ಮುಕ್ತ(series_dir_buf);

	ret = split_mail_conv(stgit_patch_to_mail, state, patches.v, keep_cr);

	strvec_clear(&patches);
	ಪ್ರತ್ಯಾಗಮ ret;
}

/**
 * A split_patches_conv() callback that converts a mercurial patch to a RFC2822
 * message suitable for parsing with git-mailinfo.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ hg_patch_to_mail(FILE *out, FILE *in, ಪೂರ್ಣಾಙ್ಕ keep_cr UNUSED)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ rc = 0;

	ಯಾವತ್ (!strbuf_getline_lf(&sb, in)) {
		ಧ್ರುವ ವರ್ಣ *str;

		ಯದಿ (skip_prefix(sb.buf, "# User ", &str))
			fprintf(out, "From: %s\n", str);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(sb.buf, "# Date ", &str)) {
			timestamp_t timestamp;
			long tz, tz2;
			ವರ್ಣ *end;

			errno = 0;
			timestamp = parse_timestamp(str, &end, 10);
			ಯದಿ (errno) {
				rc = error(_("invalid timestamp"));
				ಗಚ್ಛ exit;
			}

			ಯದಿ (!skip_prefix(end, " ", &str)) {
				rc = error(_("invalid Date line"));
				ಗಚ್ಛ exit;
			}

			errno = 0;
			tz = strtol(str, &end, 10);
			ಯದಿ (errno) {
				rc = error(_("invalid timezone offset"));
				ಗಚ್ಛ exit;
			}

			ಯದಿ (*end) {
				rc = error(_("invalid Date line"));
				ಗಚ್ಛ exit;
			}

			/*
			 * mercurial's timezone is in seconds west of UTC,
			 * however git's timezone is in hours + minutes east of
			 * UTC. Convert it.
			 */
			tz2 = labs(tz) / 3600 * 100 + labs(tz) % 3600 / 60;
			ಯದಿ (tz > 0)
				tz2 = -tz2;

			fprintf(out, "Date: %s\n", show_date(timestamp, tz2, DATE_MODE(RFC2822)));
		} ಅನ್ಯಥಾ ಯದಿ (starts_with(sb.buf, "# ")) {
			ಅನುವರ್ತ;
		} ಅನ್ಯಥಾ {
			fprintf(out, "\n%s\n", sb.buf);
			ಭಙ್ಗ;
		}
	}

	strbuf_reset(&sb);
	ಯಾವತ್ (strbuf_fread(&sb, 8192, in) > 0) {
		fwrite(sb.buf, 1, sb.len, out);
		strbuf_reset(&sb);
	}
exit:
	strbuf_release(&sb);
	ಪ್ರತ್ಯಾಗಮ rc;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ split_mail(ಸಂರಚನಾ am_state *state, ಗಣನಾ patch_format patch_format,
			ಧ್ರುವ ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ keep_cr)
{
	ಯದಿ (keep_cr < 0) {
		keep_cr = 0;
		repo_config_get_bool(the_repository, "am.keepcr", &keep_cr);
	}

	ವಿಕಲ್ಪ (patch_format) {
	ಪಕ್ಷ PATCH_FORMAT_MBOX:
		ಪ್ರತ್ಯಾಗಮ split_mail_mbox(state, paths, keep_cr, 0);
	ಪಕ್ಷ PATCH_FORMAT_STGIT:
		ಪ್ರತ್ಯಾಗಮ split_mail_conv(stgit_patch_to_mail, state, paths, keep_cr);
	ಪಕ್ಷ PATCH_FORMAT_STGIT_SERIES:
		ಪ್ರತ್ಯಾಗಮ split_mail_stgit_series(state, paths, keep_cr);
	ಪಕ್ಷ PATCH_FORMAT_HG:
		ಪ್ರತ್ಯಾಗಮ split_mail_conv(hg_patch_to_mail, state, paths, keep_cr);
	ಪಕ್ಷ PATCH_FORMAT_MBOXRD:
		ಪ್ರತ್ಯಾಗಮ split_mail_mbox(state, paths, keep_cr, 1);
	ಸ್ವಭಾವ:
		BUG("invalid patch_format");
	}
	ಪ್ರತ್ಯಾಗಮ -1;
}

/**
 * Setup a new am session for applying patches
 */
ಸ್ಥಿರ ಶೂನ್ಯ am_setup(ಸಂರಚನಾ am_state *state, ಗಣನಾ patch_format patch_format,
			ಧ್ರುವ ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ keep_cr)
{
	ಸಂರಚನಾ object_id curr_head;
	ಧ್ರುವ ವರ್ಣ *str;
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

	ಯದಿ (!patch_format)
		patch_format = detect_patch_format(paths);

	ಯದಿ (!patch_format) {
		fprintf_ln(stderr, _("Patch format detection failed."));
		die(ರಿಕ್ತ);
	}

	ಯದಿ (mkdir(state->dir, 0777) < 0 && errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), state->dir);
	refs_delete_ref(get_main_ref_store(the_repository), ರಿಕ್ತ,
			"REBASE_HEAD", ರಿಕ್ತ, REF_NO_DEREF);

	ಯದಿ (split_mail(state, patch_format, paths, keep_cr) < 0) {
		am_destroy(state);
		die(_("Failed to split patches."));
	}

	ಯದಿ (state->rebasing)
		state->threeway = 1;

	write_state_bool(state, "threeway", state->threeway);
	write_state_bool(state, "quiet", state->quiet);
	write_state_bool(state, "sign", state->signoff);
	write_state_bool(state, "utf8", state->utf8);

	ಯದಿ (state->allow_rerere_autoupdate)
		write_state_bool(state, "rerere-autoupdate",
			 state->allow_rerere_autoupdate == RERERE_AUTOUPDATE);

	ವಿಕಲ್ಪ (state->keep) {
	ಪಕ್ಷ KEEP_FALSE:
		str = "f";
		ಭಙ್ಗ;
	ಪಕ್ಷ KEEP_TRUE:
		str = "t";
		ಭಙ್ಗ;
	ಪಕ್ಷ KEEP_NON_PATCH:
		str = "b";
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		BUG("invalid value for state->keep");
	}

	write_state_text(state, "keep", str);
	write_state_bool(state, "messageid", state->message_id);

	ವಿಕಲ್ಪ (state->scissors) {
	ಪಕ್ಷ SCISSORS_UNSET:
		str = "";
		ಭಙ್ಗ;
	ಪಕ್ಷ SCISSORS_FALSE:
		str = "f";
		ಭಙ್ಗ;
	ಪಕ್ಷ SCISSORS_TRUE:
		str = "t";
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		BUG("invalid value for state->scissors");
	}
	write_state_text(state, "scissors", str);

	ವಿಕಲ್ಪ (state->quoted_cr) {
	ಪಕ್ಷ quoted_cr_unset:
		str = "";
		ಭಙ್ಗ;
	ಪಕ್ಷ quoted_cr_nowarn:
		str = "nowarn";
		ಭಙ್ಗ;
	ಪಕ್ಷ quoted_cr_warn:
		str = "warn";
		ಭಙ್ಗ;
	ಪಕ್ಷ quoted_cr_strip:
		str = "strip";
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		BUG("invalid value for state->quoted_cr");
	}
	write_state_text(state, "quoted-cr", str);

	sq_quote_argv(&sb, state->git_apply_opts.v);
	write_state_text(state, "apply-opt", sb.buf);

	ಯದಿ (state->rebasing)
		write_state_text(state, "rebasing", "");
	ಅನ್ಯಥಾ
		write_state_text(state, "applying", "");

	ಯದಿ (!repo_get_oid(the_repository, "HEAD", &curr_head)) {
		write_state_text(state, "abort-safety", oid_to_hex(&curr_head));
		ಯದಿ (!state->rebasing)
			refs_update_ref(get_main_ref_store(the_repository),
					"am", "ORIG_HEAD", &curr_head, ರಿಕ್ತ,
					0,
					UPDATE_REFS_DIE_ON_ERR);
	} ಅನ್ಯಥಾ {
		write_state_text(state, "abort-safety", "");
		ಯದಿ (!state->rebasing)
			refs_delete_ref(get_main_ref_store(the_repository),
					ರಿಕ್ತ, "ORIG_HEAD", ರಿಕ್ತ, 0);
	}

	/*
	 * NOTE: Since the "next" and "last" files determine if an am_state
	 * session is in progress, they should be written last.
	 */

	write_state_ಎಣಿಕೆ(state, "next", state->cur);
	write_state_ಎಣಿಕೆ(state, "last", state->last);

	strbuf_release(&sb);
}

/**
 * Increments the patch pointer, and cleans am_state for the application of the
 * next patch.
 */
ಸ್ಥಿರ ಶೂನ್ಯ am_next(ಸಂರಚನಾ am_state *state)
{
	ಸಂರಚನಾ object_id head;

	FREE_AND_NULL(state->author_name);
	FREE_AND_NULL(state->author_email);
	FREE_AND_NULL(state->author_date);
	FREE_AND_NULL(state->ಸಂದೇಶ);
	state->ಸಂದೇಶ_len = 0;

	unlink(am_path(state, "author-script"));
	unlink(am_path(state, "final-commit"));

	oidclr(&state->orig_commit, the_repository->hash_algo);
	unlink(am_path(state, "original-commit"));
	refs_delete_ref(get_main_ref_store(the_repository), ರಿಕ್ತ,
			"REBASE_HEAD", ರಿಕ್ತ, REF_NO_DEREF);

	ಯದಿ (!repo_get_oid(the_repository, "HEAD", &head))
		write_state_text(state, "abort-safety", oid_to_hex(&head));
	ಅನ್ಯಥಾ
		write_state_text(state, "abort-safety", "");

	state->cur++;
	write_state_ಎಣಿಕೆ(state, "next", state->cur);
}

/**
 * Returns the filename of the current patch email.
 */
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *msgnum(ಧ್ರುವ ಸಂರಚನಾ am_state *state)
{
	ಸ್ಥಿರ ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

	strbuf_reset(&sb);
	strbuf_addf(&sb, "%0*d", state->prec, state->cur);

	ಪ್ರತ್ಯಾಗಮ sb.buf;
}

/**
 * Dies with a user-friendly message on how to proceed after resolving the
 * problem. This message can be overridden with state->resolvemsg.
 */
ಸ್ಥಿರ ಶೂನ್ಯ NORETURN die_user_resolve(ಧ್ರುವ ಸಂರಚನಾ am_state *state)
{
	ಯದಿ (state->resolvemsg) {
		advise_if_enabled(ADVICE_MERGE_CONFLICT, "%s", state->resolvemsg);
	} ಅನ್ಯಥಾ {
		ಧ್ರುವ ವರ್ಣ *cmdline = state->interactive ? "git am -i" : "git am";
		ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

		strbuf_addf(&sb, _("When you have resolved this problem, run \"%s --continue\".\n"), cmdline);
		strbuf_addf(&sb, _("If you prefer to skip this patch, run \"%s --skip\" instead.\n"), cmdline);

		ಯದಿ (advice_enabled(ADVICE_AM_WORK_DIR) &&
		    is_empty_or_missing_file(am_path(state, "patch")) &&
		    !repo_index_has_changes(the_repository, ರಿಕ್ತ, ರಿಕ್ತ))
			strbuf_addf(&sb, _("To record the empty patch as an empty commit, run \"%s --allow-empty\".\n"), cmdline);

		strbuf_addf(&sb, _("To restore the original branch and stop patching, run \"%s --abort\"."), cmdline);

		advise_if_enabled(ADVICE_MERGE_CONFLICT, "%s", sb.buf);
		strbuf_release(&sb);
	}

	die(ರಿಕ್ತ);
}

/**
 * Appends signoff to the "msg" field of the am_state.
 */
ಸ್ಥಿರ ಶೂನ್ಯ am_append_signoff(ಸಂರಚನಾ am_state *state)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

	strbuf_attach(&sb, state->ಸಂದೇಶ, state->ಸಂದೇಶ_len, state->ಸಂದೇಶ_len + 1);
	append_signoff(&sb, 0, 0);
	state->ಸಂದೇಶ = strbuf_detach(&sb, &state->ಸಂದೇಶ_len);
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_mail(ಸಂರಚನಾ am_state *state, ಧ್ರುವ ವರ್ಣ *mail)
{
	FILE *fp;
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಸಂರಚನಾ strbuf ಸಂದೇಶ = STRBUF_INIT;
	ಸಂರಚನಾ strbuf author_name = STRBUF_INIT;
	ಸಂರಚನಾ strbuf author_date = STRBUF_INIT;
	ಸಂರಚನಾ strbuf author_email = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	ಸಂರಚನಾ mailinfo mi;

	setup_mailinfo(the_repository, &mi);

	ಯದಿ (state->utf8)
		mi.metainfo_charset = get_commit_output_encoding();
	ಅನ್ಯಥಾ
		mi.metainfo_charset = ರಿಕ್ತ;

	ವಿಕಲ್ಪ (state->keep) {
	ಪಕ್ಷ KEEP_FALSE:
		ಭಙ್ಗ;
	ಪಕ್ಷ KEEP_TRUE:
		mi.keep_subject = 1;
		ಭಙ್ಗ;
	ಪಕ್ಷ KEEP_NON_PATCH:
		mi.keep_non_patch_brackets_in_subject = 1;
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		BUG("invalid value for state->keep");
	}

	ಯದಿ (state->message_id)
		mi.add_message_id = 1;

	ವಿಕಲ್ಪ (state->scissors) {
	ಪಕ್ಷ SCISSORS_UNSET:
		ಭಙ್ಗ;
	ಪಕ್ಷ SCISSORS_FALSE:
		mi.use_scissors = 0;
		ಭಙ್ಗ;
	ಪಕ್ಷ SCISSORS_TRUE:
		mi.use_scissors = 1;
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		BUG("invalid value for state->scissors");
	}

	ವಿಕಲ್ಪ (state->quoted_cr) {
	ಪಕ್ಷ quoted_cr_unset:
		ಭಙ್ಗ;
	ಪಕ್ಷ quoted_cr_nowarn:
	ಪಕ್ಷ quoted_cr_warn:
	ಪಕ್ಷ quoted_cr_strip:
		mi.quoted_cr = state->quoted_cr;
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		BUG("invalid value for state->quoted_cr");
	}

	mi.ಒಳಹರಿವು = xfopen(mail, "r");
	mi.output = xfopen(am_path(state, "info"), "w");
	ಯದಿ (mailinfo(&mi, am_path(state, "msg"), am_path(state, "patch")))
		die("could not parse patch");

	fclose(mi.ಒಳಹರಿವು);
	fclose(mi.output);

	ಯದಿ (mi.format_flowed)
		warning(_("Patch sent with format=flowed; "
			  "space at the end of lines might be lost."));

	/* Extract message and author information */
	fp = xfopen(am_path(state, "info"), "r");
	ಯಾವತ್ (!strbuf_getline_lf(&sb, fp)) {
		ಧ್ರುವ ವರ್ಣ *x;

		ಯದಿ (skip_prefix(sb.buf, "Subject: ", &x)) {
			ಯದಿ (ಸಂದೇಶ.len)
				strbuf_addch(&ಸಂದೇಶ, '\n');
			strbuf_addstr(&ಸಂದೇಶ, x);
		} ಅನ್ಯಥಾ ಯದಿ (skip_prefix(sb.buf, "Author: ", &x))
			strbuf_addstr(&author_name, x);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(sb.buf, "Email: ", &x))
			strbuf_addstr(&author_email, x);
		ಅನ್ಯಥಾ ಯದಿ (skip_prefix(sb.buf, "Date: ", &x))
			strbuf_addstr(&author_date, x);
	}
	fclose(fp);

	/* Skip pine's internal folder data */
	ಯದಿ (!strcmp(author_name.buf, "Mail System Internal Data")) {
		ret = 1;
		ಗಚ್ಛ finish;
	}

	strbuf_addstr(&ಸಂದೇಶ, "\n\n");
	strbuf_addbuf(&ಸಂದೇಶ, &mi.log_message);
	strbuf_stripspace(&ಸಂದೇಶ, ರಿಕ್ತ);

	ಪ್ರತಿಜ್ಞಾ(!state->author_name);
	state->author_name = strbuf_detach(&author_name, ರಿಕ್ತ);

	ಪ್ರತಿಜ್ಞಾ(!state->author_email);
	state->author_email = strbuf_detach(&author_email, ರಿಕ್ತ);

	ಪ್ರತಿಜ್ಞಾ(!state->author_date);
	state->author_date = strbuf_detach(&author_date, ರಿಕ್ತ);

	ಪ್ರತಿಜ್ಞಾ(!state->ಸಂದೇಶ);
	state->ಸಂದೇಶ = strbuf_detach(&ಸಂದೇಶ, &state->ಸಂದೇಶ_len);

finish:
	strbuf_release(&ಸಂದೇಶ);
	strbuf_release(&author_date);
	strbuf_release(&author_email);
	strbuf_release(&author_name);
	strbuf_release(&sb);
	clear_mailinfo(&mi);
	ಪ್ರತ್ಯಾಗಮ ret;
}

/**
 * Sets commit_id to the commit hash where the mail was generated from.
 * Returns 0 on success, -1 on failure.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_mail_commit_oid(ಸಂರಚನಾ object_id *commit_id, ಧ್ರುವ ವರ್ಣ *mail)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	FILE *fp = xfopen(mail, "r");
	ಧ್ರುವ ವರ್ಣ *x;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	ಯದಿ (strbuf_getline_lf(&sb, fp) ||
	    !skip_prefix(sb.buf, "From ", &x) ||
	    get_oid_hex(x, commit_id) < 0)
		ret = -1;

	strbuf_release(&sb);
	fclose(fp);
	ಪ್ರತ್ಯಾಗಮ ret;
}

/**
 * Sets state->msg, state->author_name, state->author_email, state->author_date
 * to the commit's respective info.
 */
ಸ್ಥಿರ ಶೂನ್ಯ get_commit_info(ಸಂರಚನಾ am_state *state, ಸಂರಚನಾ commit *commit)
{
	ಧ್ರುವ ವರ್ಣ *buffer, *ident_ಸಾಲು, *ಸಂದೇಶ;
	size_t ident_len;
	ಸಂರಚನಾ ident_split id;

	buffer = repo_logmsg_reencode(the_repository, commit, ರಿಕ್ತ,
				      get_commit_output_encoding());

	ident_ಸಾಲು = find_commit_header(buffer, "author", &ident_len);
	ಯದಿ (!ident_ಸಾಲು)
		die(_("missing author line in commit %s"),
		      oid_to_hex(&commit->object.oid));
	ಯದಿ (split_ident_ಸಾಲು(&id, ident_ಸಾಲು, ident_len) < 0)
		die(_("invalid ident line: %.*s"), (ಪೂರ್ಣಾಙ್ಕ)ident_len, ident_ಸಾಲು);

	ಪ್ರತಿಜ್ಞಾ(!state->author_name);
	ಯದಿ (id.name_begin)
		state->author_name =
			xmemdupz(id.name_begin, id.name_end - id.name_begin);
	ಅನ್ಯಥಾ
		state->author_name = xstrdup("");

	ಪ್ರತಿಜ್ಞಾ(!state->author_email);
	ಯದಿ (id.mail_begin)
		state->author_email =
			xmemdupz(id.mail_begin, id.mail_end - id.mail_begin);
	ಅನ್ಯಥಾ
		state->author_email = xstrdup("");

	ಪ್ರತಿಜ್ಞಾ(!state->author_date);
	state->author_date = xstrdup(show_ident_date(&id, DATE_MODE(NORMAL)));

	ಪ್ರತಿಜ್ಞಾ(!state->ಸಂದೇಶ);
	ಸಂದೇಶ = strstr(buffer, "\n\n");
	ಯದಿ (!ಸಂದೇಶ)
		die(_("unable to parse commit %s"), oid_to_hex(&commit->object.oid));
	state->ಸಂದೇಶ = xstrdup(ಸಂದೇಶ + 2);
	state->ಸಂದೇಶ_len = strlen(state->ಸಂದೇಶ);
	repo_unuse_commit_buffer(the_repository, commit, buffer);
}

/**
 * Writes `commit` as a patch to the state directory's "patch" file.
 */
ಸ್ಥಿರ ಶೂನ್ಯ write_commit_patch(ಧ್ರುವ ಸಂರಚನಾ am_state *state, ಸಂರಚನಾ commit *commit)
{
	ಸಂರಚನಾ rev_info rev_info;
	FILE *fp;

	fp = xfopen(am_path(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, ರಿಕ್ತ);
	rev_info.diff = 1;
	rev_info.abbrev = 0;
	rev_info.disable_stdin = 1;
	rev_info.show_ಬೇರು_diff = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.flags.ದ್ವಿಮಾನ = 1;
	rev_info.diffopt.flags.full_index = 1;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	add_pending_object(&rev_info, &commit->object, "");
	diff_setup_done(&rev_info.diffopt);
	log_ವೃಕ್ಷ_commit(&rev_info, commit);
	release_revisions(&rev_info);
}

/**
 * Writes the diff of the index against HEAD as a patch to the state
 * directory's "patch" file.
 */
ಸ್ಥಿರ ಶೂನ್ಯ write_index_patch(ಧ್ರುವ ಸಂರಚನಾ am_state *state)
{
	ಸಂರಚನಾ ವೃಕ್ಷ *ವೃಕ್ಷ;
	ಸಂರಚನಾ object_id head;
	ಸಂರಚನಾ rev_info rev_info;
	FILE *fp;

	ಯದಿ (!repo_get_oid(the_repository, "HEAD", &head)) {
		ಸಂರಚನಾ commit *commit = lookup_commit_or_die(&head, "HEAD");
		ವೃಕ್ಷ = repo_get_commit_ವೃಕ್ಷ(the_repository, commit);
	} ಅನ್ಯಥಾ
		ವೃಕ್ಷ = lookup_ವೃಕ್ಷ(the_repository,
				   the_repository->hash_algo->empty_ವೃಕ್ಷ);

	fp = xfopen(am_path(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, ರಿಕ್ತ);
	rev_info.diff = 1;
	rev_info.disable_stdin = 1;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	add_pending_object(&rev_info, &ವೃಕ್ಷ->object, "");
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_mail_rebase(ಸಂರಚನಾ am_state *state, ಧ್ರುವ ವರ್ಣ *mail)
{
	ಸಂರಚನಾ commit *commit;
	ಸಂರಚನಾ object_id commit_oid;

	ಯದಿ (get_mail_commit_oid(&commit_oid, mail) < 0)
		die(_("could not parse %s"), mail);

	commit = lookup_commit_or_die(&commit_oid, mail);

	get_commit_info(state, commit);

	write_commit_patch(state, commit);

	oidcpy(&state->orig_commit, &commit_oid);
	write_state_text(state, "original-commit", oid_to_hex(&commit_oid));
	refs_update_ref(get_main_ref_store(the_repository), "am",
			"REBASE_HEAD", &commit_oid,
			ರಿಕ್ತ, REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);

	ಪ್ರತ್ಯಾಗಮ 0;
}

/**
 * Applies current patch with git-apply. Returns 0 on success, -1 otherwise. If
 * `index_file` is not NULL, the patch will be applied to that index.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ run_apply(ಧ್ರುವ ಸಂರಚನಾ am_state *state, ಧ್ರುವ ವರ್ಣ *index_file)
{
	ಸಂರಚನಾ strvec apply_paths = STRVEC_INIT;
	ಸಂರಚನಾ strvec apply_opts = STRVEC_INIT;
	ಸಂರಚನಾ apply_state apply_state;
	ಪೂರ್ಣಾಙ್ಕ res, opts_ಎಡ;
	ಪೂರ್ಣಾಙ್ಕ force_apply = 0;
	ಪೂರ್ಣಾಙ್ಕ options = 0;
	ಧ್ರುವ ವರ್ಣ **apply_argv;

	ಯದಿ (init_apply_state(&apply_state, the_repository, ರಿಕ್ತ))
		BUG("init_apply_state() failed");

	strvec_ತಳ್ಳು(&apply_opts, "apply");
	strvec_pushv(&apply_opts, state->git_apply_opts.v);

	/*
	 * Build a copy that apply_parse_options() can rearrange.
	 * apply_opts.v keeps referencing the allocated strings for
	 * strvec_clear() to release.
	 */
	DUP_ARRAY(apply_argv, apply_opts.v, apply_opts.nr);

	opts_ಎಡ = apply_parse_options(apply_opts.nr, apply_argv,
					&apply_state, &force_apply, &options,
					ರಿಕ್ತ);

	ಯದಿ (opts_ಎಡ != 0)
		die("unknown option passed through to git apply");

	ಯದಿ (index_file) {
		apply_state.index_file = index_file;
		apply_state.cached = 1;
	} ಅನ್ಯಥಾ
		apply_state.check_index = 1;

	/*
	 * If we are allowed to fall back on 3-way merge, don't give false
	 * errors during the initial attempt.
	 */
	ಯದಿ (state->threeway && !index_file)
		apply_state.apply_verbosity = verbosity_silent;

	ಯದಿ (check_apply_state(&apply_state, force_apply))
		BUG("check_apply_state() failed");

	strvec_ತಳ್ಳು(&apply_paths, am_path(state, "patch"));

	res = apply_all_patches(&apply_state, apply_paths.nr, apply_paths.v, options);

	strvec_clear(&apply_paths);
	strvec_clear(&apply_opts);
	clear_apply_state(&apply_state);
	ಮುಕ್ತ(apply_argv);

	ಯದಿ (res)
		ಪ್ರತ್ಯಾಗಮ res;

	ಯದಿ (index_file) {
		/* Reload index as apply_all_patches() will have modified it. */
		discard_index(the_repository->index);
		read_index_from(the_repository->index, index_file,
				repo_get_git_dir(the_repository));
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

/**
 * Builds an index that contains just the blobs needed for a 3way merge.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ build_fake_ancestor(ಧ್ರುವ ಸಂರಚನಾ am_state *state, ಧ್ರುವ ವರ್ಣ *index_file)
{
	ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	strvec_ತಳ್ಳು(&cp.args, "apply");
	strvec_pushv(&cp.args, state->git_apply_opts.v);
	strvec_pushf(&cp.args, "--build-fake-ancestor=%s", index_file);
	strvec_ತಳ್ಳು(&cp.args, am_path(state, "patch"));

	ಯದಿ (run_command(&cp))
		ಪ್ರತ್ಯಾಗಮ -1;

	ಪ್ರತ್ಯಾಗಮ 0;
}

/**
 * Attempt a threeway merge, using index_path as the temporary index.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ fall_back_threeway(ಧ್ರುವ ಸಂರಚನಾ am_state *state, ಧ್ರುವ ವರ್ಣ *index_path)
{
	ಸಂರಚನಾ object_id their_ವೃಕ್ಷ, our_ವೃಕ್ಷ;
	ಸಂರಚನಾ object_id bases[1] = { 0 };
	ಸಂರಚನಾ merge_options o;
	ಸಂರಚನಾ commit *result;
	ವರ್ಣ *their_ವೃಕ್ಷ_name;

	ಯದಿ (repo_get_oid(the_repository, "HEAD", &our_ವೃಕ್ಷ) < 0)
		oidcpy(&our_ವೃಕ್ಷ, the_hash_algo->empty_ವೃಕ್ಷ);

	ಯದಿ (build_fake_ancestor(state, index_path))
		ಪ್ರತ್ಯಾಗಮ error("could not build fake ancestor");

	discard_index(the_repository->index);
	read_index_from(the_repository->index, index_path, repo_get_git_dir(the_repository));

	ಯದಿ (write_index_as_ವೃಕ್ಷ(&bases[0], the_repository->index, index_path, 0, ರಿಕ್ತ))
		ಪ್ರತ್ಯಾಗಮ error(_("Repository lacks necessary blobs to fall back on 3-way merge."));

	say(state, stdout, _("Using index info to reconstruct a base tree..."));

	ಯದಿ (!state->quiet) {
		/*
		 * List paths that needed 3-way fallback, so that the user can
		 * review them with extra care to spot mismerges.
		 */
		ಸಂರಚನಾ rev_info rev_info;

		repo_init_revisions(the_repository, &rev_info, ರಿಕ್ತ);
		rev_info.diffopt.output_format = DIFF_FORMAT_NAME_STATUS;
		rev_info.diffopt.filter |= diff_filter_bit('A');
		rev_info.diffopt.filter |= diff_filter_bit('M');
		add_pending_oid(&rev_info, "HEAD", &our_ವೃಕ್ಷ, 0);
		diff_setup_done(&rev_info.diffopt);
		run_diff_index(&rev_info, DIFF_INDEX_CACHED);
		release_revisions(&rev_info);
	}

	ಯದಿ (run_apply(state, index_path))
		ಪ್ರತ್ಯಾಗಮ error(_("Did you hand edit your patch?\n"
				"It does not apply to blobs recorded in its index."));

	ಯದಿ (write_index_as_ವೃಕ್ಷ(&their_ವೃಕ್ಷ, the_repository->index, index_path, 0, ರಿಕ್ತ))
		ಪ್ರತ್ಯಾಗಮ error("could not write tree");

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
	their_ವೃಕ್ಷ_name = xstrfmt("%.*s", linelen(state->ಸಂದೇಶ), state->ಸಂದೇಶ);
	o.branch2 = their_ವೃಕ್ಷ_name;
	o.ancestor = "constructed fake ancestor";
	o.detect_directory_renames = MERGE_DIRECTORY_RENAMES_NONE;

	ಯದಿ (state->quiet)
		o.verbosity = 0;

	ಯದಿ (merge_ort_generic(&o, &our_ವೃಕ್ಷ, &their_ವೃಕ್ಷ, 1, bases, &result)) {
		repo_rerere(the_repository, state->allow_rerere_autoupdate);
		ಮುಕ್ತ(their_ವೃಕ್ಷ_name);
		ಪ್ರತ್ಯಾಗಮ error(_("Failed to merge in the changes."));
	}

	ಮುಕ್ತ(their_ವೃಕ್ಷ_name);
	ಪ್ರತ್ಯಾಗಮ 0;
}

/**
 * Commits the current index with state->msg as the commit message and
 * state->author_name, state->author_email and state->author_date as the author
 * information.
 */
ಸ್ಥಿರ ಶೂನ್ಯ do_commit(ಧ್ರುವ ಸಂರಚನಾ am_state *state)
{
	ಸಂರಚನಾ object_id ವೃಕ್ಷ, parent, commit;
	ಧ್ರುವ ಸಂರಚನಾ object_id *old_oid;
	ಸಂರಚನಾ commit_list *parents = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *reflog_ಸಂದೇಶ, *author, *committer = ರಿಕ್ತ;
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

	ಯದಿ (!state->no_verify && run_hooks(the_repository, "pre-applypatch"))
		exit(1);

	ಯದಿ (write_index_as_ವೃಕ್ಷ(&ವೃಕ್ಷ, the_repository->index,
				repo_get_index_file(the_repository),
				0, ರಿಕ್ತ))
		die(_("git write-tree failed to write a tree"));

	ಯದಿ (!repo_get_oid_commit(the_repository, "HEAD", &parent)) {
		old_oid = &parent;
		commit_list_ಸೇರಿಸು(lookup_commit(the_repository, &parent),
				   &parents);
	} ಅನ್ಯಥಾ {
		old_oid = ರಿಕ್ತ;
		say(state, stderr, _("applying to an empty history"));
	}

	author = fmt_ident(state->author_name, state->author_email,
		WANT_AUTHOR_IDENT,
			state->ignore_date ? ರಿಕ್ತ : state->author_date,
			IDENT_STRICT);

	ಯದಿ (state->committer_date_is_author_date)
		committer = fmt_ident(getenv("GIT_COMMITTER_NAME"),
				      getenv("GIT_COMMITTER_EMAIL"),
				      WANT_COMMITTER_IDENT,
				      state->ignore_date ? ರಿಕ್ತ
							 : state->author_date,
				      IDENT_STRICT);

	ಯದಿ (commit_ವೃಕ್ಷ_extended(state->ಸಂದೇಶ, state->ಸಂದೇಶ_len, &ವೃಕ್ಷ, parents,
				 &commit, author, committer, state->sign_commit,
				 ರಿಕ್ತ))
		die(_("failed to write commit object"));

	reflog_ಸಂದೇಶ = getenv("GIT_REFLOG_ACTION");
	ಯದಿ (!reflog_ಸಂದೇಶ)
		reflog_ಸಂದೇಶ = "am";

	strbuf_addf(&sb, "%s: %.*s", reflog_ಸಂದೇಶ, linelen(state->ಸಂದೇಶ),
			state->ಸಂದೇಶ);

	refs_update_ref(get_main_ref_store(the_repository), sb.buf, "HEAD",
			&commit, old_oid, 0,
			UPDATE_REFS_DIE_ON_ERR);

	ಯದಿ (state->rebasing) {
		FILE *fp = xfopen(am_path(state, "rewritten"), "a");

		ಪ್ರತಿಜ್ಞಾ(!is_null_oid(&state->orig_commit));
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
ಸ್ಥಿರ ಶೂನ್ಯ validate_resume_state(ಧ್ರುವ ಸಂರಚನಾ am_state *state)
{
	ಯದಿ (!state->ಸಂದೇಶ)
		die(_("cannot resume: %s does not exist."),
			am_path(state, "final-commit"));

	ಯದಿ (!state->author_name || !state->author_email || !state->author_date)
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ do_interactive(ಸಂರಚನಾ am_state *state)
{
	ಪ್ರತಿಜ್ಞಾ(state->ಸಂದೇಶ);

	ಪ್ರತಿ (;;) {
		ವರ್ಣ reply[64];

		puts(_("Commit Body is:"));
		puts("--------------------------");
		printf("%s", state->ಸಂದೇಶ);
		puts("--------------------------");

		/*
		 * TRANSLATORS: Make sure to include [y], [n], [e], [v] and [a]
		 * in your translation. The program will only accept English
		 * input at this point.
		 */
		printf(_("Apply? [y]es/[n]o/[e]dit/[v]iew patch/[a]ccept all: "));
		ಯದಿ (!fgets(reply, ಪರಿಮಾಣ(reply), stdin))
			die("unable to read from stdin; aborting");

		ಯದಿ (*reply == 'y' || *reply == 'Y') {
			ಪ್ರತ್ಯಾಗಮ 0;
		} ಅನ್ಯಥಾ ಯದಿ (*reply == 'a' || *reply == 'A') {
			state->interactive = 0;
			ಪ್ರತ್ಯಾಗಮ 0;
		} ಅನ್ಯಥಾ ಯದಿ (*reply == 'n' || *reply == 'N') {
			ಪ್ರತ್ಯಾಗಮ 1;
		} ಅನ್ಯಥಾ ಯದಿ (*reply == 'e' || *reply == 'E') {
			ಸಂರಚನಾ strbuf ಸಂದೇಶ = STRBUF_INIT;

			ಯದಿ (!launch_editor(am_path(state, "final-commit"), &ಸಂದೇಶ, ರಿಕ್ತ)) {
				ಮುಕ್ತ(state->ಸಂದೇಶ);
				state->ಸಂದೇಶ = strbuf_detach(&ಸಂದೇಶ, &state->ಸಂದೇಶ_len);
			}
			strbuf_release(&ಸಂದೇಶ);
		} ಅನ್ಯಥಾ ಯದಿ (*reply == 'v' || *reply == 'V') {
			ಧ್ರುವ ವರ್ಣ *pager = git_pager(the_repository, 1);
			ಸಂರಚನಾ child_process cp = CHILD_PROCESS_INIT;

			ಯದಿ (!pager)
				pager = "cat";
			prepare_pager_args(&cp, pager);
			strvec_ತಳ್ಳು(&cp.args, am_path(state, "patch"));
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
ಸ್ಥಿರ ಶೂನ್ಯ am_run(ಸಂರಚನಾ am_state *state, ಪೂರ್ಣಾಙ್ಕ resume)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

	unlink(am_path(state, "dirtyindex"));

	ಯದಿ (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ) < 0)
		die(_("unable to write index file"));

	ಯದಿ (repo_index_has_changes(the_repository, ರಿಕ್ತ, &sb)) {
		write_state_bool(state, "dirtyindex", 1);
		die(_("Dirty index: cannot apply patches (dirty: %s)"), sb.buf);
	}

	strbuf_release(&sb);

	ಯಾವತ್ (state->cur <= state->last) {
		ಧ್ರುವ ವರ್ಣ *mail = am_path(state, msgnum(state));
		ಪೂರ್ಣಾಙ್ಕ apply_status;
		ಪೂರ್ಣಾಙ್ಕ to_keep;

		reset_ident_date();

		ಯದಿ (!file_exists(mail))
			ಗಚ್ಛ next;

		ಯದಿ (resume) {
			validate_resume_state(state);
		} ಅನ್ಯಥಾ {
			ಪೂರ್ಣಾಙ್ಕ skip;

			ಯದಿ (state->rebasing)
				skip = parse_mail_rebase(state, mail);
			ಅನ್ಯಥಾ
				skip = parse_mail(state, mail);

			ಯದಿ (skip)
				ಗಚ್ಛ next; /* mail should be skipped */

			ಯದಿ (state->signoff)
				am_append_signoff(state);

			write_author_script(state);
			write_commit_ಸಂದೇಶ(state);
		}

		ಯದಿ (state->interactive && do_interactive(state))
			ಗಚ್ಛ next;

		to_keep = 0;
		ಯದಿ (is_empty_or_missing_file(am_path(state, "patch"))) {
			ವಿಕಲ್ಪ (state->empty_type) {
			ಪಕ್ಷ DROP_EMPTY_COMMIT:
				say(state, stdout, _("Skipping: %.*s"), linelen(state->ಸಂದೇಶ), state->ಸಂದೇಶ);
				ಗಚ್ಛ next;
				ಭಙ್ಗ;
			ಪಕ್ಷ KEEP_EMPTY_COMMIT:
				to_keep = 1;
				say(state, stdout, _("Creating an empty commit: %.*s"),
					linelen(state->ಸಂದೇಶ), state->ಸಂದೇಶ);
				ಭಙ್ಗ;
			ಪಕ್ಷ STOP_ON_EMPTY_COMMIT:
				printf_ln(_("Patch is empty."));
				die_user_resolve(state);
				ಭಙ್ಗ;
			}
		}

		ಯದಿ (run_applypatch_ಸಂದೇಶ_hook(state))
			exit(1);
		ಯದಿ (to_keep)
			ಗಚ್ಛ commit;

		say(state, stdout, _("Applying: %.*s"), linelen(state->ಸಂದೇಶ), state->ಸಂದೇಶ);

		apply_status = run_apply(state, ರಿಕ್ತ);

		ಯದಿ (apply_status && state->threeway) {
			ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

			strbuf_addstr(&sb, am_path(state, "patch-merge-index"));
			apply_status = fall_back_threeway(state, sb.buf);
			strbuf_release(&sb);

			/*
			 * Applying the patch to an earlier tree and merging
			 * the result may have produced the same tree as ours.
			 */
			ಯದಿ (!apply_status &&
			    !repo_index_has_changes(the_repository, ರಿಕ್ತ, ರಿಕ್ತ)) {
				say(state, stdout, _("No changes -- Patch already applied."));
				ಗಚ್ಛ next;
			}
		}

		ಯದಿ (apply_status) {
			printf_ln(_("Patch failed at %s %.*s"), msgnum(state),
				linelen(state->ಸಂದೇಶ), state->ಸಂದೇಶ);

			ಯದಿ (advice_enabled(ADVICE_AM_WORK_DIR))
				advise(_("Use 'git am --show-current-patch=diff' to see the failed patch"));

			die_user_resolve(state);
		}

commit:
		do_commit(state);

next:
		am_next(state);

		ಯದಿ (resume)
			am_load(state);
		resume = 0;
	}

	ಯದಿ (!is_empty_or_missing_file(am_path(state, "rewritten"))) {
		ಪ್ರತಿಜ್ಞಾ(state->rebasing);
		copy_notes_for_rebase(state);
		run_post_rewrite_hook(state);
	}

	/*
	 * In rebasing mode, it's up to the caller to take care of
	 * housekeeping.
	 */
	ಯದಿ (!state->rebasing) {
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
ಸ್ಥಿರ ಶೂನ್ಯ am_resolve(ಸಂರಚನಾ am_state *state, ಪೂರ್ಣಾಙ್ಕ allow_empty)
{
	validate_resume_state(state);

	say(state, stdout, _("Applying: %.*s"), linelen(state->ಸಂದೇಶ), state->ಸಂದೇಶ);

	ಯದಿ (!repo_index_has_changes(the_repository, ರಿಕ್ತ, ರಿಕ್ತ)) {
		ಯದಿ (allow_empty && is_empty_or_missing_file(am_path(state, "patch"))) {
			printf_ln(_("No changes - recorded it as an empty commit."));
		} ಅನ್ಯಥಾ {
			printf_ln(_("No changes - did you forget to use 'git add'?\n"
				    "If there is nothing left to stage, chances are that something else\n"
				    "already introduced the same changes; you might want to skip this patch."));
			die_user_resolve(state);
		}
	}

	ಯದಿ (unmerged_index(the_repository->index)) {
		printf_ln(_("You still have unmerged paths in your index.\n"
			"You should 'git add' each file with resolved conflicts to mark them as such.\n"
			"You might run `git rm` on a file to accept \"deleted by them\" for it."));
		die_user_resolve(state);
	}

	ಯದಿ (state->interactive) {
		write_index_patch(state);
		ಯದಿ (do_interactive(state))
			ಗಚ್ಛ next;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ fast_forward_to(ಸಂರಚನಾ ವೃಕ್ಷ *head, ಸಂರಚನಾ ವೃಕ್ಷ *remote, ಪೂರ್ಣಾಙ್ಕ reset)
{
	ಸಂರಚನಾ lock_file lock_file = LOCK_INIT;
	ಸಂರಚನಾ unpack_trees_options opts;
	ಸಂರಚನಾ ವೃಕ್ಷ_desc t[2];

	ಯದಿ (repo_parse_ವೃಕ್ಷ(the_repository, head) || repo_parse_ವೃಕ್ಷ(the_repository, remote))
		ಪ್ರತ್ಯಾಗಮ -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	refresh_index(the_repository->index, REFRESH_QUIET, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ);

	memset(&opts, 0, ಪರಿಮಾಣ(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.update = 1;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = twoway_merge;
	init_ವೃಕ್ಷ_desc(&t[0], &head->object.oid, head->buffer, head->size);
	init_ವೃಕ್ಷ_desc(&t[1], &remote->object.oid, remote->buffer, remote->size);

	ಯದಿ (unpack_trees(2, t, &opts)) {
		rollback_lock_file(&lock_file);
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಯದಿ (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	ಪ್ರತ್ಯಾಗಮ 0;
}

/**
 * Merges a tree into the index. The index's stat info will take precedence
 * over the merged tree's. Returns 0 on success, -1 on failure.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ merge_ವೃಕ್ಷ(ಸಂರಚನಾ ವೃಕ್ಷ *ವೃಕ್ಷ)
{
	ಸಂರಚನಾ lock_file lock_file = LOCK_INIT;
	ಸಂರಚನಾ unpack_trees_options opts;
	ಸಂರಚನಾ ವೃಕ್ಷ_desc t[1];

	ಯದಿ (repo_parse_ವೃಕ್ಷ(the_repository, ವೃಕ್ಷ))
		ಪ್ರತ್ಯಾಗಮ -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, ಪರಿಮಾಣ(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.merge = 1;
	opts.fn = oneway_merge;
	init_ವೃಕ್ಷ_desc(&t[0], &ವೃಕ್ಷ->object.oid, ವೃಕ್ಷ->buffer, ವೃಕ್ಷ->size);

	ಯದಿ (unpack_trees(1, t, &opts)) {
		rollback_lock_file(&lock_file);
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಯದಿ (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	ಪ್ರತ್ಯಾಗಮ 0;
}

/**
 * Clean the index without touching entries that are not modified between
 * `head` and `remote`.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ clean_index(ಧ್ರುವ ಸಂರಚನಾ object_id *head, ಧ್ರುವ ಸಂರಚನಾ object_id *remote)
{
	ಸಂರಚನಾ ವೃಕ್ಷ *head_ವೃಕ್ಷ, *remote_ವೃಕ್ಷ, *index_ವೃಕ್ಷ;
	ಸಂರಚನಾ object_id index;

	head_ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_indirect(the_repository, head);
	ಯದಿ (!head_ವೃಕ್ಷ)
		ಪ್ರತ್ಯಾಗಮ error(_("Could not parse object '%s'."), oid_to_hex(head));

	remote_ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_indirect(the_repository, remote);
	ಯದಿ (!remote_ವೃಕ್ಷ)
		ಪ್ರತ್ಯಾಗಮ error(_("Could not parse object '%s'."), oid_to_hex(remote));

	repo_read_index_unmerged(the_repository);

	ಯದಿ (fast_forward_to(head_ವೃಕ್ಷ, head_ವೃಕ್ಷ, 1))
		ಪ್ರತ್ಯಾಗಮ -1;

	ಯದಿ (write_index_as_ವೃಕ್ಷ(&index, the_repository->index,
				repo_get_index_file(the_repository),
				0, ರಿಕ್ತ))
		ಪ್ರತ್ಯಾಗಮ -1;

	index_ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_indirect(the_repository, &index);
	ಯದಿ (!index_ವೃಕ್ಷ)
		ಪ್ರತ್ಯಾಗಮ error(_("Could not parse object '%s'."), oid_to_hex(&index));

	ಯದಿ (fast_forward_to(index_ವೃಕ್ಷ, remote_ವೃಕ್ಷ, 0))
		ಪ್ರತ್ಯಾಗಮ -1;

	ಯದಿ (merge_ವೃಕ್ಷ(remote_ವೃಕ್ಷ))
		ಪ್ರತ್ಯಾಗಮ -1;

	remove_branch_state(the_repository, 0);

	ಪ್ರತ್ಯಾಗಮ 0;
}

/**
 * Resets rerere's merge resolution metadata.
 */
ಸ್ಥಿರ ಶೂನ್ಯ am_rerere_clear(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ string_list merge_rr = STRING_LIST_INIT_DUP;
	rerere_clear(the_repository, &merge_rr);
	string_list_clear(&merge_rr, 1);
}

/**
 * Resume the current am session by skipping the current patch.
 */
ಸ್ಥಿರ ಶೂನ್ಯ am_skip(ಸಂರಚನಾ am_state *state)
{
	ಸಂರಚನಾ object_id head;

	am_rerere_clear();

	ಯದಿ (repo_get_oid(the_repository, "HEAD", &head))
		oidcpy(&head, the_hash_algo->empty_ವೃಕ್ಷ);

	ಯದಿ (clean_index(&head, &head))
		die(_("failed to clean index"));

	ಯದಿ (state->rebasing) {
		FILE *fp = xfopen(am_path(state, "rewritten"), "a");

		ಪ್ರತಿಜ್ಞಾ(!is_null_oid(&state->orig_commit));
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ safe_to_abort(ಧ್ರುವ ಸಂರಚನಾ am_state *state)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಸಂರಚನಾ object_id abort_safety, head;

	ಯದಿ (file_exists(am_path(state, "dirtyindex")))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (read_state_file(&sb, state, "abort-safety", 1) > 0) {
		ಯದಿ (get_oid_hex(sb.buf, &abort_safety))
			die(_("could not parse %s"), am_path(state, "abort-safety"));
	} ಅನ್ಯಥಾ
		oidclr(&abort_safety, the_repository->hash_algo);
	strbuf_release(&sb);

	ಯದಿ (repo_get_oid(the_repository, "HEAD", &head))
		oidclr(&head, the_repository->hash_algo);

	ಯದಿ (oideq(&head, &abort_safety))
		ಪ್ರತ್ಯಾಗಮ 1;

	warning(_("You seem to have moved HEAD since the last 'am' failure.\n"
		"Not rewinding to ORIG_HEAD"));

	ಪ್ರತ್ಯಾಗಮ 0;
}

/**
 * Aborts the current am session if it is safe to do so.
 */
ಸ್ಥಿರ ಶೂನ್ಯ am_abort(ಸಂರಚನಾ am_state *state)
{
	ಸಂರಚನಾ object_id curr_head, orig_head;
	ಪೂರ್ಣಾಙ್ಕ has_curr_head, has_orig_head;
	ವರ್ಣ *curr_branch;

	ಯದಿ (!safe_to_abort(state)) {
		am_destroy(state);
		ಪ್ರತ್ಯಾಗಮ;
	}

	am_rerere_clear();

	curr_branch = refs_resolve_refdup(get_main_ref_store(the_repository),
					  "HEAD", 0, &curr_head, ರಿಕ್ತ);
	has_curr_head = curr_branch && !is_null_oid(&curr_head);
	ಯದಿ (!has_curr_head)
		oidcpy(&curr_head, the_hash_algo->empty_ವೃಕ್ಷ);

	has_orig_head = !repo_get_oid(the_repository, "ORIG_HEAD", &orig_head);
	ಯದಿ (!has_orig_head)
		oidcpy(&orig_head, the_hash_algo->empty_ವೃಕ್ಷ);

	ಯದಿ (clean_index(&curr_head, &orig_head))
		die(_("failed to clean index"));

	ಯದಿ (has_orig_head)
		refs_update_ref(get_main_ref_store(the_repository),
				"am --abort", "HEAD", &orig_head,
				has_curr_head ? &curr_head : ರಿಕ್ತ, 0,
				UPDATE_REFS_DIE_ON_ERR);
	ಅನ್ಯಥಾ ಯದಿ (curr_branch)
		refs_delete_ref(get_main_ref_store(the_repository), ರಿಕ್ತ,
				curr_branch, ರಿಕ್ತ, REF_NO_DEREF);

	ಮುಕ್ತ(curr_branch);
	am_destroy(state);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_patch(ಸಂರಚನಾ am_state *state, ಗಣನಾ resume_type resume_mode)
{
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *patch_path;
	ಪೂರ್ಣಾಙ್ಕ len;

	ಯದಿ (!is_null_oid(&state->orig_commit)) {
		ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;

		strvec_pushl(&cmd.args, "show", oid_to_hex(&state->orig_commit),
			     "--", ರಿಕ್ತ);
		cmd.git_cmd = 1;
		ಪ್ರತ್ಯಾಗಮ run_command(&cmd);
	}

	ವಿಕಲ್ಪ (resume_mode) {
	ಪಕ್ಷ RESUME_SHOW_PATCH_RAW:
		patch_path = am_path(state, msgnum(state));
		ಭಙ್ಗ;
	ಪಕ್ಷ RESUME_SHOW_PATCH_DIFF:
		patch_path = am_path(state, "patch");
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		BUG("invalid mode for --show-current-patch");
	}

	len = strbuf_read_file(&sb, patch_path, 0);
	ಯದಿ (len < 0)
		die_errno(_("failed to read '%s'"), patch_path);

	setup_pager(the_repository);
	write_in_full(1, sb.buf, sb.len);
	strbuf_release(&sb);
	ಪ್ರತ್ಯಾಗಮ 0;
}

/**
 * parse_options() callback that validates and sets opt->value to the
 * PATCH_FORMAT_* enum value corresponding to `arg`.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_opt_patchformat(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಪೂರ್ಣಾಙ್ಕ *opt_ಮೌಲ್ಯ = opt->ಮೌಲ್ಯ;

	ಯದಿ (unset)
		*opt_ಮೌಲ್ಯ = PATCH_FORMAT_UNKNOWN;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "mbox"))
		*opt_ಮೌಲ್ಯ = PATCH_FORMAT_MBOX;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "stgit"))
		*opt_ಮೌಲ್ಯ = PATCH_FORMAT_STGIT;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "stgit-series"))
		*opt_ಮೌಲ್ಯ = PATCH_FORMAT_STGIT_SERIES;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "hg"))
		*opt_ಮೌಲ್ಯ = PATCH_FORMAT_HG;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "mboxrd"))
		*opt_ಮೌಲ್ಯ = PATCH_FORMAT_MBOXRD;
	/*
	 * Please update $__git_patchformat in git-completion.bash
	 * when you add new options
	 */
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ error(_("invalid value for '%s': '%s'"),
			     "--patch-format", arg);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_opt_show_current_patch(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಪೂರ್ಣಾಙ್ಕ *opt_ಮೌಲ್ಯ = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	ಯದಿ (!arg)
		*opt_ಮೌಲ್ಯ = opt->defval;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "raw"))
		*opt_ಮೌಲ್ಯ = RESUME_SHOW_PATCH_RAW;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "diff"))
		*opt_ಮೌಲ್ಯ = RESUME_SHOW_PATCH_DIFF;
	/*
	 * Please update $__git_showcurrentpatch in git-completion.bash
	 * when you add new options
	 */
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ error(_("invalid value for '%s': '%s'"),
			     "--show-current-patch", arg);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ cmd_am(ಪೂರ್ಣಾಙ್ಕ argc,
	   ಧ್ರುವ ವರ್ಣ **argv,
	   ಧ್ರುವ ವರ್ಣ *prefix,
	   ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ am_state state;
	ಪೂರ್ಣಾಙ್ಕ ದ್ವಿಮಾನ = -1;
	ಪೂರ್ಣಾಙ್ಕ keep_cr = -1;
	ಪೂರ್ಣಾಙ್ಕ patch_format = PATCH_FORMAT_UNKNOWN;
	ಗಣನಾ resume_type resume_mode = RESUME_FALSE;
	ಪೂರ್ಣಾಙ್ಕ in_progress;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ usage[] = {
		N_("git am [<options>] [(<mbox> | <Maildir>)...]"),
		N_("git am [<options>] (--continue | --skip | --abort)"),
		ರಿಕ್ತ
	};

	ಸಂರಚನಾ option options[] = {
		OPT_BOOL('i', "interactive", &state.interactive,
			N_("run interactively")),
		OPT_BOOL('n', "no-verify", &state.no_verify,
			N_("bypass pre-applypatch and applypatch-msg hooks")),
		OPT_HIDDEN_BOOL('b', "binary", &ದ್ವಿಮಾನ,
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
		OPT_PASSTHRU_ARGV(0, "ignore-space-change", &state.git_apply_opts, ರಿಕ್ತ,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU_ARGV(0, "ignore-whitespace", &state.git_apply_opts, ರಿಕ್ತ,
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
		OPT_PASSTHRU_ARGV('C', ರಿಕ್ತ, &state.git_apply_opts, N_("n"),
			N_("pass it through git-apply"),
			0),
		OPT_PASSTHRU_ARGV('p', ರಿಕ್ತ, &state.git_apply_opts, N_("num"),
			N_("pass it through git-apply"),
			0),
		OPT_CALLBACK(0, "patch-format", &patch_format, N_("format"),
			N_("format the patch(es) are in"),
			parse_opt_patchformat),
		OPT_PASSTHRU_ARGV(0, "reject", &state.git_apply_opts, ರಿಕ್ತ,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_STRING(0, "resolvemsg", &state.resolvemsg, ರಿಕ್ತ,
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
			.ಮೌಲ್ಯ = &resume_mode,
			.precision = ಪರಿಮಾಣ(resume_mode),
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
			.ಮೌಲ್ಯ = &state.sign_commit,
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

	repo_config(the_repository, git_default_config, ರಿಕ್ತ);

	am_state_init(&state);

	in_progress = am_in_progress(&state);
	ಯದಿ (in_progress)
		am_load(&state);

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	ಯದಿ (ದ್ವಿಮಾನ >= 0)
		fprintf_ln(stderr, _("The -b/--binary option has been a no-op for long time, and\n"
				"it will be removed. Please do not use it anymore."));

	/* Ensure a valid committer ident can be constructed */
	git_committer_info(IDENT_STRICT);

	ಯದಿ (repo_read_index_preload(the_repository, ರಿಕ್ತ, 0) < 0)
		die(_("failed to read the index"));

	ಯದಿ (in_progress) {
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
		ಯದಿ (argc || (resume_mode == RESUME_FALSE && !isatty(0)))
			die(_("previous rebase directory %s still exists but mbox given."),
				state.dir);

		ಯದಿ (resume_mode == RESUME_FALSE)
			resume_mode = RESUME_APPLY;

		ಯದಿ (state.signoff == SIGNOFF_EXPLICIT)
			am_append_signoff(&state);
	} ಅನ್ಯಥಾ {
		ಸಂರಚನಾ strvec paths = STRVEC_INIT;
		ಪೂರ್ಣಾಙ್ಕ i;

		/*
		 * Handle stray state directory in the independent-run case. In
		 * the --rebasing case, it is up to the caller to take care of
		 * stray directories.
		 */
		ಯದಿ (file_exists(state.dir) && !state.rebasing) {
			ಯದಿ (resume_mode == RESUME_ABORT || resume_mode == RESUME_QUIT) {
				am_destroy(&state);
				am_state_release(&state);
				ಪ್ರತ್ಯಾಗಮ 0;
			}

			die(_("Stray %s directory found.\n"
				"Use \"git am --abort\" to remove it."),
				state.dir);
		}

		ಯದಿ (resume_mode)
			die(_("Resolve operation not in progress, we are not resuming."));

		ಪ್ರತಿ (i = 0; i < argc; i++) {
			ಯದಿ (is_absolute_path(argv[i]) || !prefix)
				strvec_ತಳ್ಳು(&paths, argv[i]);
			ಅನ್ಯಥಾ
				strvec_ತಳ್ಳು(&paths, mkpath("%s/%s", prefix, argv[i]));
		}

		ಯದಿ (state.interactive && !paths.nr)
			die(_("interactive mode requires patches on the command line"));

		am_setup(&state, patch_format, paths.v, keep_cr);

		strvec_clear(&paths);
	}

	ವಿಕಲ್ಪ (resume_mode) {
	ಪಕ್ಷ RESUME_FALSE:
		am_run(&state, 0);
		ಭಙ್ಗ;
	ಪಕ್ಷ RESUME_APPLY:
		am_run(&state, 1);
		ಭಙ್ಗ;
	ಪಕ್ಷ RESUME_RESOLVED:
	ಪಕ್ಷ RESUME_ALLOW_EMPTY:
		am_resolve(&state, resume_mode == RESUME_ALLOW_EMPTY ? 1 : 0);
		ಭಙ್ಗ;
	ಪಕ್ಷ RESUME_SKIP:
		am_skip(&state);
		ಭಙ್ಗ;
	ಪಕ್ಷ RESUME_ABORT:
		am_abort(&state);
		ಭಙ್ಗ;
	ಪಕ್ಷ RESUME_QUIT:
		am_rerere_clear();
		am_destroy(&state);
		ಭಙ್ಗ;
	ಪಕ್ಷ RESUME_SHOW_PATCH_RAW:
	ಪಕ್ಷ RESUME_SHOW_PATCH_DIFF:
		ret = show_patch(&state, resume_mode);
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		BUG("invalid resume value");
	}

	am_state_release(&state);

	ಪ್ರತ್ಯಾಗಮ ret;
}
