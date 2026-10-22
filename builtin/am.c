/*
 * Builtin "git am"
 *
 * Based on git-am.sh by Junio C Hamano.
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE

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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ linelen(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ)
{
	ಹಿಂತಿರುಗಿ strchrnul(ಸಂದೇಶ, '\n') - ಸಂದೇಶ;
}

/**
 * Returns true if `str` consists of only whitespace, false otherwise.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ str_isspace(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str)
{
	ಗಾಗಿ (; *str; str++)
		ಆದರೆ (!ಖಾಲಿಯೇ(*str))
			ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ 1;
}

ಎಣಿಕೆ patch_format {
	PATCH_FORMAT_UNKNOWN = 0,
	PATCH_FORMAT_MBOX,
	PATCH_FORMAT_STGIT,
	PATCH_FORMAT_STGIT_SERIES,
	PATCH_FORMAT_HG,
	PATCH_FORMAT_MBOXRD
};

ಎಣಿಕೆ keep_type {
	KEEP_FALSE = 0,
	KEEP_TRUE,      /* pass -k flag to git-mailinfo */
	KEEP_NON_PATCH  /* pass -b flag to git-mailinfo */
};

ಎಣಿಕೆ scissors_type {
	SCISSORS_UNSET = -1,
	SCISSORS_FALSE = 0,  /* pass --no-scissors to git-mailinfo */
	SCISSORS_TRUE        /* pass --scissors to git-mailinfo */
};

ಎಣಿಕೆ signoff_type {
	SIGNOFF_FALSE = 0,
	SIGNOFF_TRUE = 1,
	SIGNOFF_EXPLICIT /* --signoff was set on the command-line */
};

ಎಣಿಕೆ resume_type {
	RESUME_FALSE = 0,
	RESUME_APPLY,
	RESUME_RESOLVED,
	RESUME_SKIP,
	RESUME_ABORT,
	RESUME_QUIT,
	RESUME_SHOW_PATCH_RAW,
	RESUME_SHOW_PATCH_DIFF,
	RESUME_ALLOW_ಖಾಲಿಗಳು,
};

ಎಣಿಕೆ ಖಾಲಿಗಳು_action {
	STOP_ON_ಖಾಲಿಗಳು_COMMIT = 0,  /* output errors and stop in the middle of an am session */
	DROP_ಖಾಲಿಗಳು_COMMIT,         /* skip with a notice message, unless "--quiet" has been passed */
	KEEP_ಖಾಲಿಗಳು_COMMIT,         /* keep recording as empty commits */
};

ರಚನೆ am_state {
	/* state directory path */
	ಅಕ್ಷರ *dir;

	/* current and last patch numbers, 1-indexed */
	ಪೂರ್ಣಾಂಕ cur;
	ಪೂರ್ಣಾಂಕ ಕೊನೆಯ;

	/* commit metadata and message */
	ಅಕ್ಷರ *author_name;
	ಅಕ್ಷರ *author_email;
	ಅಕ್ಷರ *author_date;
	ಅಕ್ಷರ *ಸಂದೇಶ;
	ಗಾತ್ರ_t ಸಂದೇಶ_len;

	/* when --rebasing, records the original commit the patch came from */
	ರಚನೆ object_id orig_commit;

	/* number of digits in patch filename */
	ಪೂರ್ಣಾಂಕ prec;

	/* various operating modes and command line options */
	ಪೂರ್ಣಾಂಕ interactive;
	ಪೂರ್ಣಾಂಕ no_verify;
	ಪೂರ್ಣಾಂಕ threeway;
	ಪೂರ್ಣಾಂಕ quiet;
	ಪೂರ್ಣಾಂಕ signoff; /* enum signoff_type */
	ಪೂರ್ಣಾಂಕ utf8;
	ಪೂರ್ಣಾಂಕ keep; /* enum keep_type */
	ಪೂರ್ಣಾಂಕ ಸಂದೇಶ_id;
	ಪೂರ್ಣಾಂಕ scissors; /* enum scissors_type */
	ಪೂರ್ಣಾಂಕ quoted_cr; /* enum quoted_cr_action */
	ಪೂರ್ಣಾಂಕ ಖಾಲಿಗಳು_type; /* enum empty_action */
	ರಚನೆ strvec git_apply_opts;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *resolvemsg;
	ಪೂರ್ಣಾಂಕ committer_date_is_author_date;
	ಪೂರ್ಣಾಂಕ ignore_date;
	ಪೂರ್ಣಾಂಕ allow_rerere_autoupdate;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *sign_commit;
	ಪೂರ್ಣಾಂಕ rebasing;
};

/**
 * Initializes am_state with the default values.
 */
ಸ್ಥಿರ ಶೂನ್ಯ am_state_init(ರಚನೆ am_state *state)
{
	ಪೂರ್ಣಾಂಕ gpgsign;

	memset(state, 0, ಗಾತ್ರ(*state));

	state->dir = repo_git_ಮಾರ್ಗ(the_repository, "rebase-apply");

	state->prec = 4;

	repo_config_get_bool(the_repository, "am.threeway", &state->threeway);

	state->utf8 = 1;

	repo_config_get_bool(the_repository, "am.messageid", &state->ಸಂದೇಶ_id);

	state->scissors = SCISSORS_UNSET;
	state->quoted_cr = quoted_cr_unset;

	strvec_init(&state->git_apply_opts);

	ಆದರೆ (!repo_config_get_bool(the_repository, "commit.gpgsign", &gpgsign))
		state->sign_commit = gpgsign ? "" : ಖಾಲಿ;
}

/**
 * Releases memory allocated by an am_state.
 */
ಸ್ಥಿರ ಶೂನ್ಯ am_state_release(ರಚನೆ am_state *state)
{
	ಬಿಡುಗಡೆ(state->dir);
	ಬಿಡುಗಡೆ(state->author_name);
	ಬಿಡುಗಡೆ(state->author_email);
	ಬಿಡುಗಡೆ(state->author_date);
	ಬಿಡುಗಡೆ(state->ಸಂದೇಶ);
	strvec_clear(&state->git_apply_opts);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ am_option_parse_quoted_cr(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	BUG_ON_OPT_NEG(unset);

	ಆದರೆ (mailinfo_parse_quoted_cr_action(arg, opt->ಮೌಲ್ಯ) != 0)
		ಹಿಂತಿರುಗಿ error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ am_option_parse_ಖಾಲಿಗಳು(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಪೂರ್ಣಾಂಕ *opt_ಮೌಲ್ಯ = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	ಆದರೆ (!strcmp(arg, "stop"))
		*opt_ಮೌಲ್ಯ = STOP_ON_ಖಾಲಿಗಳು_COMMIT;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "drop"))
		*opt_ಮೌಲ್ಯ = DROP_ಖಾಲಿಗಳು_COMMIT;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "keep"))
		*opt_ಮೌಲ್ಯ = KEEP_ಖಾಲಿಗಳು_COMMIT;
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ error(_("invalid value for '%s': '%s'"), "--empty", arg);

	ಹಿಂತಿರುಗಿ 0;
}

/**
 * Returns path relative to the am_state directory.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *am_ಮಾರ್ಗ(ಸ್ಥಿರಾಂಕ ರಚನೆ am_state *state, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಹಿಂತಿರುಗಿ mkpath("%s/%s", state->dir, ಮಾರ್ಗ);
}

/**
 * For convenience to call write_file()
 */
ಸ್ಥಿರ ಶೂನ್ಯ write_state_ಪಠ್ಯ(ಸ್ಥಿರಾಂಕ ರಚನೆ am_state *state,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string)
{
	write_file(am_ಮಾರ್ಗ(state, name), "%s", string);
}

ಸ್ಥಿರ ಶೂನ್ಯ write_state_ಎಣಿಕೆ(ಸ್ಥಿರಾಂಕ ರಚನೆ am_state *state,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ)
{
	write_file(am_ಮಾರ್ಗ(state, name), "%d", ಮೌಲ್ಯ);
}

ಸ್ಥಿರ ಶೂನ್ಯ write_state_bool(ಸ್ಥಿರಾಂಕ ರಚನೆ am_state *state,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ)
{
	write_state_ಪಠ್ಯ(state, name, ಮೌಲ್ಯ ? "t" : "f");
}

/**
 * If state->quiet is false, calls fprintf(fp, fmt, ...), and appends a newline
 * at the end.
 */
__attribute__((format (ಬರೆ, 3, 4)))
ಸ್ಥಿರ ಶೂನ್ಯ say(ಸ್ಥಿರಾಂಕ ರಚನೆ am_state *state, FILE *fp, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...)
{
	va_list ap;

	va_ಆರಂಭ(ap, fmt);
	ಆದರೆ (!state->quiet) {
		vfprintf(fp, fmt, ap);
		putc('\n', fp);
	}
	va_ಕೊನೆ(ap);
}

/**
 * Returns 1 if there is an am session in progress, 0 otherwise.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ am_ಒಳ_ಪ್ರಗತಿ(ಸ್ಥಿರಾಂಕ ರಚನೆ am_state *state)
{
	ರಚನೆ stat st;

	ಆದರೆ (lstat(state->dir, &st) < 0 || !S_ISDIR(st.st_ಬಹುಲಕ))
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (lstat(am_ಮಾರ್ಗ(state, "last"), &st) || !S_ISREG(st.st_ಬಹುಲಕ))
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (lstat(am_ಮಾರ್ಗ(state, "next"), &st) || !S_ISREG(st.st_ಬಹುಲಕ))
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ 1;
}

/**
 * Reads the contents of `file` in the `state` directory into `sb`. Returns the
 * number of bytes read on success, -1 if the file does not exist. If `trim` is
 * set, trailing whitespace will be removed.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ read_state_file(ರಚನೆ strbuf *sb, ಸ್ಥಿರಾಂಕ ರಚನೆ am_state *state,
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, ಪೂರ್ಣಾಂಕ trim)
{
	strbuf_reset(sb);

	ಆದರೆ (strbuf_read_file(sb, am_ಮಾರ್ಗ(state, file), 0) >= 0) {
		ಆದರೆ (trim)
			strbuf_trim(sb);

		ಹಿಂತಿರುಗಿ sb->len;
	}

	ಆದರೆ (errno == ENOENT)
		ಹಿಂತಿರುಗಿ -1;

	die_errno(_("could not read '%s'"), am_ಮಾರ್ಗ(state, file));
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ read_am_author_script(ರಚನೆ am_state *state)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename = am_ಮಾರ್ಗ(state, "author-script");

	ಪ್ರತಿಜ್ಞಾ(!state->author_name);
	ಪ್ರತಿಜ್ಞಾ(!state->author_email);
	ಪ್ರತಿಜ್ಞಾ(!state->author_date);

	ಹಿಂತಿರುಗಿ read_author_script(filename, &state->author_name,
				  &state->author_email, &state->author_date, 1);
}

/**
 * Saves state->author_name, state->author_email and state->author_date in the
 * state directory's "author-script" file.
 */
ಸ್ಥಿರ ಶೂನ್ಯ write_author_script(ಸ್ಥಿರಾಂಕ ರಚನೆ am_state *state)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;

	strbuf_addstr(&sb, "GIT_AUTHOR_NAME=");
	sq_quote_buf(&sb, state->author_name);
	strbuf_addch(&sb, '\n');

	strbuf_addstr(&sb, "GIT_AUTHOR_EMAIL=");
	sq_quote_buf(&sb, state->author_email);
	strbuf_addch(&sb, '\n');

	strbuf_addstr(&sb, "GIT_AUTHOR_DATE=");
	sq_quote_buf(&sb, state->author_date);
	strbuf_addch(&sb, '\n');

	write_state_ಪಠ್ಯ(state, "author-script", sb.buf);

	strbuf_release(&sb);
}

/**
 * Reads the commit message from the state directory's "final-commit" file,
 * setting state->msg to its contents and state->msg_len to the length of its
 * contents in bytes.
 *
 * Returns 0 on success, -1 if the file does not exist.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ read_commit_ಸಂದೇಶ(ರಚನೆ am_state *state)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;

	ಪ್ರತಿಜ್ಞಾ(!state->ಸಂದೇಶ);

	ಆದರೆ (read_state_file(&sb, state, "final-commit", 0) < 0) {
		strbuf_release(&sb);
		ಹಿಂತಿರುಗಿ -1;
	}

	state->ಸಂದೇಶ = strbuf_detach(&sb, &state->ಸಂದೇಶ_len);
	ಹಿಂತಿರುಗಿ 0;
}

/**
 * Saves state->msg in the state directory's "final-commit" file.
 */
ಸ್ಥಿರ ಶೂನ್ಯ write_commit_ಸಂದೇಶ(ಸ್ಥಿರಾಂಕ ರಚನೆ am_state *state)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename = am_ಮಾರ್ಗ(state, "final-commit");
	write_file_buf(filename, state->ಸಂದೇಶ, state->ಸಂದೇಶ_len);
}

/**
 * Loads state from disk.
 */
ಸ್ಥಿರ ಶೂನ್ಯ am_ತುಂಬು(ರಚನೆ am_state *state)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;

	ಆದರೆ (read_state_file(&sb, state, "next", 1) < 0)
		BUG("state file 'next' does not exist");
	state->cur = strtol(sb.buf, ಖಾಲಿ, 10);

	ಆದರೆ (read_state_file(&sb, state, "last", 1) < 0)
		BUG("state file 'last' does not exist");
	state->ಕೊನೆಯ = strtol(sb.buf, ಖಾಲಿ, 10);

	ಆದರೆ (read_am_author_script(state) < 0)
		die(_("could not parse author script"));

	read_commit_ಸಂದೇಶ(state);

	ಆದರೆ (read_state_file(&sb, state, "original-commit", 1) < 0)
		oidclr(&state->orig_commit, the_repository->hash_algo);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (get_oid_hex(sb.buf, &state->orig_commit) < 0)
		die(_("could not parse %s"), am_ಮಾರ್ಗ(state, "original-commit"));

	read_state_file(&sb, state, "threeway", 1);
	state->threeway = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "quiet", 1);
	state->quiet = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "sign", 1);
	state->signoff = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "utf8", 1);
	state->utf8 = !strcmp(sb.buf, "t");

	ಆದರೆ (file_exists(am_ಮಾರ್ಗ(state, "rerere-autoupdate"))) {
		read_state_file(&sb, state, "rerere-autoupdate", 1);
		state->allow_rerere_autoupdate = strcmp(sb.buf, "t") ?
			RERERE_NOAUTOUPDATE : RERERE_AUTOUPDATE;
	} ಇಲ್ಲದಿದ್ದರೆ {
		state->allow_rerere_autoupdate = 0;
	}

	read_state_file(&sb, state, "keep", 1);
	ಆದರೆ (!strcmp(sb.buf, "t"))
		state->keep = KEEP_TRUE;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(sb.buf, "b"))
		state->keep = KEEP_NON_PATCH;
	ಇಲ್ಲದಿದ್ದರೆ
		state->keep = KEEP_FALSE;

	read_state_file(&sb, state, "messageid", 1);
	state->ಸಂದೇಶ_id = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "scissors", 1);
	ಆದರೆ (!strcmp(sb.buf, "t"))
		state->scissors = SCISSORS_TRUE;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(sb.buf, "f"))
		state->scissors = SCISSORS_FALSE;
	ಇಲ್ಲದಿದ್ದರೆ
		state->scissors = SCISSORS_UNSET;

	read_state_file(&sb, state, "quoted-cr", 1);
	ಆದರೆ (!*sb.buf)
		state->quoted_cr = quoted_cr_unset;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (mailinfo_parse_quoted_cr_action(sb.buf, &state->quoted_cr) != 0)
		die(_("could not parse %s"), am_ಮಾರ್ಗ(state, "quoted-cr"));

	read_state_file(&sb, state, "apply-opt", 1);
	strvec_clear(&state->git_apply_opts);
	ಆದರೆ (sq_dequote_ಗಮ್ಯ_strvec(sb.buf, &state->git_apply_opts) < 0)
		die(_("could not parse %s"), am_ಮಾರ್ಗ(state, "apply-opt"));

	state->rebasing = !!file_exists(am_ಮಾರ್ಗ(state, "rebasing"));

	strbuf_release(&sb);
}

/**
 * Removes the am_state directory, forcefully terminating the current am
 * session.
 */
ಸ್ಥಿರ ಶೂನ್ಯ am_destroy(ಸ್ಥಿರಾಂಕ ರಚನೆ am_state *state)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;

	strbuf_addstr(&sb, state->dir);
	remove_dir_recursively(&sb, 0);
	strbuf_release(&sb);
}

/**
 * Runs applypatch-msg hook. Returns its exit code.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸತತ_applypatch_ಸಂದೇಶ_hook(ರಚನೆ am_state *state)
{
	ಪೂರ್ಣಾಂಕ ret = 0;

	ಪ್ರತಿಜ್ಞಾ(state->ಸಂದೇಶ);

	ಆದರೆ (!state->no_verify) {
		ರಚನೆ ಸತತ_hooks_opt opt = ಸತತ_HOOKS_OPT_INIT_FORCE_SERIAL;
		strvec_ತಳ್ಳು(&opt.ವಾದಗಳು, am_ಮಾರ್ಗ(state, "final-commit"));
		ret = ಸತತ_hooks_opt(the_repository, "applypatch-msg", &opt);
	}

	ಆದರೆ (!ret) {
		FREE_AND_NULL(state->ಸಂದೇಶ);
		ಆದರೆ (read_commit_ಸಂದೇಶ(state) < 0)
			die(_("'%s' was deleted by the applypatch-msg hook"),
				am_ಮಾರ್ಗ(state, "final-commit"));
	}

	ಹಿಂತಿರುಗಿ ret;
}

/**
 * Runs post-rewrite hook. Returns it exit code.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸತತ_post_rewrite_hook(ಸ್ಥಿರಾಂಕ ರಚನೆ am_state *state)
{
	ರಚನೆ ಸತತ_hooks_opt opt = ಸತತ_HOOKS_OPT_INIT;

	strvec_ತಳ್ಳು(&opt.ವಾದಗಳು, "rebase");
	opt.ಮಾರ್ಗ_ಗಮ್ಯ_stdin = am_ಮಾರ್ಗ(state, "rewritten");

	ಹಿಂತಿರುಗಿ ಸತತ_hooks_opt(the_repository, "post-rewrite", &opt);
}

/**
 * Reads the state directory's "rewritten" file, and copies notes from the old
 * commits listed in the file to their rewritten commits.
 *
 * Returns 0 on success, -1 on failure.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ copy_notes_for_rebase(ಸ್ಥಿರಾಂಕ ರಚನೆ am_state *state)
{
	ರಚನೆ notes_rewrite_cfg *c;
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *invalid_ಸಾಲು = _("Malformed input line: '%s'.");
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ = "Notes added by 'git rebase'";
	FILE *fp;
	ಪೂರ್ಣಾಂಕ ret = 0;

	ಪ್ರತಿಜ್ಞಾ(state->rebasing);

	c = init_copy_notes_for_rewrite("rebase");
	ಆದರೆ (!c)
		ಹಿಂತಿರುಗಿ 0;

	fp = xfopen(am_ಮಾರ್ಗ(state, "rewritten"), "r");

	ಆಗ (!strbuf_getline_lf(&sb, fp)) {
		ರಚನೆ object_id from_obj, ಗಮ್ಯ_obj;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p;

		ಆದರೆ (sb.len != the_hash_algo->hexsz * 2 + 1) {
			ret = error(invalid_ಸಾಲು, sb.buf);
			ಹೋಗು ಮುಕ್ತಾಯ;
		}

		ಆದರೆ (parse_oid_hex(sb.buf, &from_obj, &p)) {
			ret = error(invalid_ಸಾಲು, sb.buf);
			ಹೋಗು ಮುಕ್ತಾಯ;
		}

		ಆದರೆ (*p != ' ') {
			ret = error(invalid_ಸಾಲು, sb.buf);
			ಹೋಗು ಮುಕ್ತಾಯ;
		}

		ಆದರೆ (get_oid_hex(p + 1, &ಗಮ್ಯ_obj)) {
			ret = error(invalid_ಸಾಲು, sb.buf);
			ಹೋಗು ಮುಕ್ತಾಯ;
		}

		ಆದರೆ (copy_note_for_rewrite(c, &from_obj, &ಗಮ್ಯ_obj))
			ret = error(_("Failed to copy notes from '%s' to '%s'"),
					oid_ಗಮ್ಯ_hex(&from_obj), oid_ಗಮ್ಯ_hex(&ಗಮ್ಯ_obj));
	}

ಮುಕ್ತಾಯ:
	ಮುಕ್ತಾಯ_copy_notes_for_rewrite(the_repository, c, ಸಂದೇಶ);
	fclose(fp);
	strbuf_release(&sb);
	ಹಿಂತಿರುಗಿ ret;
}

/**
 * Determines if the file looks like a piece of RFC2822 mail by grabbing all
 * non-indented lines and checking if they look like they begin with valid
 * header field names.
 *
 * Returns 1 if the file looks like a piece of mail, 0 otherwise.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ is_mail(FILE *fp)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಶೀರ್ಷಿಕೆ_regex = "^[!-9;-~]+:";
	ರಚನೆ strbuf sb = STRBUF_INIT;
	regex_t regex;
	ಪೂರ್ಣಾಂಕ ret = 1;

	ಆದರೆ (fseek(fp, 0L, SEEK_ಗಣ))
		die_errno(_("fseek failed"));

	ಆದರೆ (regcomp(&regex, ಶೀರ್ಷಿಕೆ_regex, REG_NOSUB | REG_EXTENDED))
		die("invalid pattern: %s", ಶೀರ್ಷಿಕೆ_regex);

	ಆಗ (!strbuf_getline(&sb, fp)) {
		ಆದರೆ (!sb.len)
			ನಿಲ್ಲಿಸು; /* End of header */

		/* Ignore indented folded lines */
		ಆದರೆ (*sb.buf == '\t' || *sb.buf == ' ')
			ಮುಂದುವರಿ;

		/* It's a header if it matches header_regex */
		ಆದರೆ (regexec(&regex, sb.buf, 0, ಖಾಲಿ, 0)) {
			ret = 0;
			ಹೋಗು ಮುಗಿದ;
		}
	}

ಮುಗಿದ:
	regfree(&regex);
	strbuf_release(&sb);
	ಹಿಂತಿರುಗಿ ret;
}

/**
 * Attempts to detect the patch_format of the patches contained in `paths`,
 * returning the PATCH_FORMAT_* enum value. Returns PATCH_FORMAT_UNKNOWN if
 * detection fails.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ detect_patch_format(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **paths)
{
	ಎಣಿಕೆ patch_format ret = PATCH_FORMAT_UNKNOWN;
	ರಚನೆ strbuf l1 = STRBUF_INIT;
	ರಚನೆ strbuf l2 = STRBUF_INIT;
	ರಚನೆ strbuf l3 = STRBUF_INIT;
	FILE *fp;

	/*
	 * We default to mbox format if input is from stdin and for directories
	 */
	ಆದರೆ (!*paths || !strcmp(*paths, "-") || is_directory(*paths))
		ಹಿಂತಿರುಗಿ PATCH_FORMAT_MBOX;

	/*
	 * Otherwise, check the first few lines of the first patch, starting
	 * from the first non-blank line, to try to detect its format.
	 */

	fp = xfopen(*paths, "r");

	ಆಗ (!strbuf_getline(&l1, fp)) {
		ಆದರೆ (l1.len)
			ನಿಲ್ಲಿಸು;
	}

	ಆದರೆ (starts_with(l1.buf, "From ") || starts_with(l1.buf, "From: ")) {
		ret = PATCH_FORMAT_MBOX;
		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (starts_with(l1.buf, "# This series applies on GIT commit")) {
		ret = PATCH_FORMAT_STGIT_SERIES;
		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (!strcmp(l1.buf, "# HG changeset patch")) {
		ret = PATCH_FORMAT_HG;
		ಹೋಗು ಮುಗಿದ;
	}

	strbuf_getline(&l2, fp);
	strbuf_getline(&l3, fp);

	/*
	 * If the second line is empty and the third is a From, Author or Date
	 * entry, this is likely an StGit patch.
	 */
	ಆದರೆ (l1.len && !l2.len &&
		(starts_with(l3.buf, "From:") ||
		 starts_with(l3.buf, "Author:") ||
		 starts_with(l3.buf, "Date:"))) {
		ret = PATCH_FORMAT_STGIT;
		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (l1.len && is_mail(fp)) {
		ret = PATCH_FORMAT_MBOX;
		ಹೋಗು ಮುಗಿದ;
	}

ಮುಗಿದ:
	fclose(fp);
	strbuf_release(&l1);
	strbuf_release(&l2);
	strbuf_release(&l3);
	ಹಿಂತಿರುಗಿ ret;
}

/**
 * Splits out individual email patches from `paths`, where each path is either
 * a mbox file or a Maildir. Returns 0 on success, -1 on failure.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ವಿಭಜಿಸು_mail_mbox(ರಚನೆ am_state *state, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **paths,
				ಪೂರ್ಣಾಂಕ keep_cr, ಪೂರ್ಣಾಂಕ mboxrd)
{
	ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;
	ರಚನೆ strbuf ಕೊನೆಯ = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ ret;

	cp.git_cmd = 1;
	strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "mailsplit");
	strvec_pushf(&cp.ವಾದಗಳು, "-d%d", state->prec);
	strvec_pushf(&cp.ವಾದಗಳು, "-o%s", state->dir);
	strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "-b");
	ಆದರೆ (keep_cr)
		strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "--keep-cr");
	ಆದರೆ (mboxrd)
		strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "--mboxrd");
	strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "--");
	strvec_pushv(&cp.ವಾದಗಳು, paths);

	ret = capture_command(&cp, &ಕೊನೆಯ, 8);
	ಆದರೆ (ret)
		ಹೋಗು ನಿರ್ಗಮಿಸು;

	state->cur = 1;
	state->ಕೊನೆಯ = strtol(ಕೊನೆಯ.buf, ಖಾಲಿ, 10);

ನಿರ್ಗಮಿಸು:
	strbuf_release(&ಕೊನೆಯ);
	ಹಿಂತಿರುಗಿ ret ? -1 : 0;
}

/**
 * Callback signature for split_mail_conv(). The foreign patch should be
 * read from `in`, and the converted patch (in RFC2822 mail format) should be
 * written to `out`. Return 0 on success, or -1 on failure.
 */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*mail_conv_fn)(FILE *ಹೊರಹೋಗು, FILE *ಒಳ, ಪೂರ್ಣಾಂಕ keep_cr);

/**
 * Calls `fn` for each file in `paths` to convert the foreign patch to the
 * RFC2822 mail format suitable for parsing with git-mailinfo.
 *
 * Returns 0 on success, -1 on failure.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ವಿಭಜಿಸು_mail_conv(mail_conv_fn fn, ರಚನೆ am_state *state,
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ keep_cr)
{
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *stdin_only[] = {"-", ಖಾಲಿ};
	ಪೂರ್ಣಾಂಕ i;

	ಆದರೆ (!*paths)
		paths = stdin_only;

	ಗಾಗಿ (i = 0; *paths; paths++, i++) {
		FILE *ಒಳ, *ಹೊರಹೋಗು;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *mail;
		ಪೂರ್ಣಾಂಕ ret;

		ಆದರೆ (!strcmp(*paths, "-"))
			ಒಳ = stdin;
		ಇಲ್ಲದಿದ್ದರೆ
			ಒಳ = fopen(*paths, "r");

		ಆದರೆ (!ಒಳ)
			ಹಿಂತಿರುಗಿ error_errno(_("could not open '%s' for reading"),
					   *paths);

		mail = mkpath("%s/%0*d", state->dir, state->prec, i + 1);

		ಹೊರಹೋಗು = fopen(mail, "w");
		ಆದರೆ (!ಹೊರಹೋಗು) {
			ಆದರೆ (ಒಳ != stdin)
				fclose(ಒಳ);
			ಹಿಂತಿರುಗಿ error_errno(_("could not open '%s' for writing"),
					   mail);
		}

		ret = fn(ಹೊರಹೋಗು, ಒಳ, keep_cr);

		fclose(ಹೊರಹೋಗು);
		ಆದರೆ (ಒಳ != stdin)
			fclose(ಒಳ);

		ಆದರೆ (ret)
			ಹಿಂತಿರುಗಿ error(_("could not parse patch '%s'"), *paths);
	}

	state->cur = 1;
	state->ಕೊನೆಯ = i;
	ಹಿಂತಿರುಗಿ 0;
}

/**
 * A split_mail_conv() callback that converts an StGit patch to an RFC2822
 * message suitable for parsing with git-mailinfo.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ stgit_patch_ಗಮ್ಯ_mail(FILE *ಹೊರಹೋಗು, FILE *ಒಳ, ಪೂರ್ಣಾಂಕ keep_cr ಬಳಸದ)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ subject_printed = 0;

	ಆಗ (!strbuf_getline_lf(&sb, ಒಳ)) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str;

		ಆದರೆ (str_isspace(sb.buf))
			ಮುಂದುವರಿ;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(sb.buf, "Author:", &str))
			fprintf(ಹೊರಹೋಗು, "From:%s\n", str);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (starts_with(sb.buf, "From") || starts_with(sb.buf, "Date"))
			fprintf(ಹೊರಹೋಗು, "%s\n", sb.buf);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!subject_printed) {
			fprintf(ಹೊರಹೋಗು, "Subject: %s\n", sb.buf);
			subject_printed = 1;
		} ಇಲ್ಲದಿದ್ದರೆ {
			fprintf(ಹೊರಹೋಗು, "\n%s\n", sb.buf);
			ನಿಲ್ಲಿಸು;
		}
	}

	strbuf_reset(&sb);
	ಆಗ (strbuf_fread(&sb, 8192, ಒಳ) > 0) {
		fwrite(sb.buf, 1, sb.len, ಹೊರಹೋಗು);
		strbuf_reset(&sb);
	}

	strbuf_release(&sb);
	ಹಿಂತಿರುಗಿ 0;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ವಿಭಜಿಸು_mail_stgit_series(ರಚನೆ am_state *state, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **paths,
					ಪೂರ್ಣಾಂಕ keep_cr)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *series_dir;
	ಅಕ್ಷರ *series_dir_buf;
	FILE *fp;
	ರಚನೆ strvec patches = STRVEC_INIT;
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (!paths[0] || paths[1])
		ಹಿಂತಿರುಗಿ error(_("Only one StGIT patch series can be applied at once"));

	series_dir_buf = xstrdup(*paths);
	series_dir = dirname(series_dir_buf);

	fp = fopen(*paths, "r");
	ಆದರೆ (!fp) {
		ಬಿಡುಗಡೆ(series_dir_buf);
		ಹಿಂತಿರುಗಿ error_errno(_("could not open '%s' for reading"), *paths);
	}

	ಆಗ (!strbuf_getline_lf(&sb, fp)) {
		ಆದರೆ (*sb.buf == '#')
			ಮುಂದುವರಿ; /* skip comment lines */

		strvec_ತಳ್ಳು(&patches, mkpath("%s/%s", series_dir, sb.buf));
	}

	fclose(fp);
	strbuf_release(&sb);
	ಬಿಡುಗಡೆ(series_dir_buf);

	ret = ವಿಭಜಿಸು_mail_conv(stgit_patch_ಗಮ್ಯ_mail, state, patches.v, keep_cr);

	strvec_clear(&patches);
	ಹಿಂತಿರುಗಿ ret;
}

/**
 * A split_patches_conv() callback that converts a mercurial patch to a RFC2822
 * message suitable for parsing with git-mailinfo.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ hg_patch_ಗಮ್ಯ_mail(FILE *ಹೊರಹೋಗು, FILE *ಒಳ, ಪೂರ್ಣಾಂಕ keep_cr ಬಳಸದ)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ rc = 0;

	ಆಗ (!strbuf_getline_lf(&sb, ಒಳ)) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str;

		ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(sb.buf, "# User ", &str))
			fprintf(ಹೊರಹೋಗು, "From: %s\n", str);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(sb.buf, "# Date ", &str)) {
			timestamp_t timestamp;
			ದೀರ್ಘ tz, tz2;
			ಅಕ್ಷರ *ಕೊನೆ;

			errno = 0;
			timestamp = parse_timestamp(str, &ಕೊನೆ, 10);
			ಆದರೆ (errno) {
				rc = error(_("invalid timestamp"));
				ಹೋಗು ನಿರ್ಗಮಿಸು;
			}

			ಆದರೆ (!skip_ಪೂರ್ವಪ್ರತ್ಯಯ(ಕೊನೆ, " ", &str)) {
				rc = error(_("invalid Date line"));
				ಹೋಗು ನಿರ್ಗಮಿಸು;
			}

			errno = 0;
			tz = strtol(str, &ಕೊನೆ, 10);
			ಆದರೆ (errno) {
				rc = error(_("invalid timezone offset"));
				ಹೋಗು ನಿರ್ಗಮಿಸು;
			}

			ಆದರೆ (*ಕೊನೆ) {
				rc = error(_("invalid Date line"));
				ಹೋಗು ನಿರ್ಗಮಿಸು;
			}

			/*
			 * mercurial's timezone is in seconds west of UTC,
			 * however git's timezone is in hours + minutes east of
			 * UTC. Convert it.
			 */
			tz2 = labs(tz) / 3600 * 100 + labs(tz) % 3600 / 60;
			ಆದರೆ (tz > 0)
				tz2 = -tz2;

			fprintf(ಹೊರಹೋಗು, "Date: %s\n", show_date(timestamp, tz2, DATE_ಬಹುಲಕ(RFC2822)));
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (starts_with(sb.buf, "# ")) {
			ಮುಂದುವರಿ;
		} ಇಲ್ಲದಿದ್ದರೆ {
			fprintf(ಹೊರಹೋಗು, "\n%s\n", sb.buf);
			ನಿಲ್ಲಿಸು;
		}
	}

	strbuf_reset(&sb);
	ಆಗ (strbuf_fread(&sb, 8192, ಒಳ) > 0) {
		fwrite(sb.buf, 1, sb.len, ಹೊರಹೋಗು);
		strbuf_reset(&sb);
	}
ನಿರ್ಗಮಿಸು:
	strbuf_release(&sb);
	ಹಿಂತಿರುಗಿ rc;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ವಿಭಜಿಸು_mail(ರಚನೆ am_state *state, ಎಣಿಕೆ patch_format patch_format,
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ keep_cr)
{
	ಆದರೆ (keep_cr < 0) {
		keep_cr = 0;
		repo_config_get_bool(the_repository, "am.keepcr", &keep_cr);
	}

	ಆಯ್ಕೆ (patch_format) {
	ಸ್ಥಿತಿ PATCH_FORMAT_MBOX:
		ಹಿಂತಿರುಗಿ ವಿಭಜಿಸು_mail_mbox(state, paths, keep_cr, 0);
	ಸ್ಥಿತಿ PATCH_FORMAT_STGIT:
		ಹಿಂತಿರುಗಿ ವಿಭಜಿಸು_mail_conv(stgit_patch_ಗಮ್ಯ_mail, state, paths, keep_cr);
	ಸ್ಥಿತಿ PATCH_FORMAT_STGIT_SERIES:
		ಹಿಂತಿರುಗಿ ವಿಭಜಿಸು_mail_stgit_series(state, paths, keep_cr);
	ಸ್ಥಿತಿ PATCH_FORMAT_HG:
		ಹಿಂತಿರುಗಿ ವಿಭಜಿಸು_mail_conv(hg_patch_ಗಮ್ಯ_mail, state, paths, keep_cr);
	ಸ್ಥಿತಿ PATCH_FORMAT_MBOXRD:
		ಹಿಂತಿರುಗಿ ವಿಭಜಿಸು_mail_mbox(state, paths, keep_cr, 1);
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("invalid patch_format");
	}
	ಹಿಂತಿರುಗಿ -1;
}

/**
 * Setup a new am session for applying patches
 */
ಸ್ಥಿರ ಶೂನ್ಯ am_ಸಿದ್ಧತೆ(ರಚನೆ am_state *state, ಎಣಿಕೆ patch_format patch_format,
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ keep_cr)
{
	ರಚನೆ object_id curr_ತಲೆ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str;
	ರಚನೆ strbuf sb = STRBUF_INIT;

	ಆದರೆ (!patch_format)
		patch_format = detect_patch_format(paths);

	ಆದರೆ (!patch_format) {
		fprintf_ln(stderr, _("Patch format detection failed."));
		die(ಖಾಲಿ);
	}

	ಆದರೆ (mkdir(state->dir, 0777) < 0 && errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), state->dir);
	refs_delete_ref(get_main_ref_store(the_repository), ಖಾಲಿ,
			"REBASE_HEAD", ಖಾಲಿ, REF_NO_DEREF);

	ಆದರೆ (ವಿಭಜಿಸು_mail(state, patch_format, paths, keep_cr) < 0) {
		am_destroy(state);
		die(_("Failed to split patches."));
	}

	ಆದರೆ (state->rebasing)
		state->threeway = 1;

	write_state_bool(state, "threeway", state->threeway);
	write_state_bool(state, "quiet", state->quiet);
	write_state_bool(state, "sign", state->signoff);
	write_state_bool(state, "utf8", state->utf8);

	ಆದರೆ (state->allow_rerere_autoupdate)
		write_state_bool(state, "rerere-autoupdate",
			 state->allow_rerere_autoupdate == RERERE_AUTOUPDATE);

	ಆಯ್ಕೆ (state->keep) {
	ಸ್ಥಿತಿ KEEP_FALSE:
		str = "f";
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ KEEP_TRUE:
		str = "t";
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ KEEP_NON_PATCH:
		str = "b";
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("invalid value for state->keep");
	}

	write_state_ಪಠ್ಯ(state, "keep", str);
	write_state_bool(state, "messageid", state->ಸಂದೇಶ_id);

	ಆಯ್ಕೆ (state->scissors) {
	ಸ್ಥಿತಿ SCISSORS_UNSET:
		str = "";
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SCISSORS_FALSE:
		str = "f";
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SCISSORS_TRUE:
		str = "t";
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("invalid value for state->scissors");
	}
	write_state_ಪಠ್ಯ(state, "scissors", str);

	ಆಯ್ಕೆ (state->quoted_cr) {
	ಸ್ಥಿತಿ quoted_cr_unset:
		str = "";
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ quoted_cr_nowarn:
		str = "nowarn";
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ quoted_cr_warn:
		str = "warn";
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ quoted_cr_strip:
		str = "strip";
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("invalid value for state->quoted_cr");
	}
	write_state_ಪಠ್ಯ(state, "quoted-cr", str);

	sq_quote_argv(&sb, state->git_apply_opts.v);
	write_state_ಪಠ್ಯ(state, "apply-opt", sb.buf);

	ಆದರೆ (state->rebasing)
		write_state_ಪಠ್ಯ(state, "rebasing", "");
	ಇಲ್ಲದಿದ್ದರೆ
		write_state_ಪಠ್ಯ(state, "applying", "");

	ಆದರೆ (!repo_get_oid(the_repository, "HEAD", &curr_ತಲೆ)) {
		write_state_ಪಠ್ಯ(state, "abort-safety", oid_ಗಮ್ಯ_hex(&curr_ತಲೆ));
		ಆದರೆ (!state->rebasing)
			refs_update_ref(get_main_ref_store(the_repository),
					"am", "ORIG_HEAD", &curr_ತಲೆ, ಖಾಲಿ,
					0,
					UPDATE_REFS_DIE_ON_ERR);
	} ಇಲ್ಲದಿದ್ದರೆ {
		write_state_ಪಠ್ಯ(state, "abort-safety", "");
		ಆದರೆ (!state->rebasing)
			refs_delete_ref(get_main_ref_store(the_repository),
					ಖಾಲಿ, "ORIG_HEAD", ಖಾಲಿ, 0);
	}

	/*
	 * NOTE: Since the "next" and "last" files determine if an am_state
	 * session is in progress, they should be written last.
	 */

	write_state_ಎಣಿಕೆ(state, "next", state->cur);
	write_state_ಎಣಿಕೆ(state, "last", state->ಕೊನೆಯ);

	strbuf_release(&sb);
}

/**
 * Increments the patch pointer, and cleans am_state for the application of the
 * next patch.
 */
ಸ್ಥಿರ ಶೂನ್ಯ am_ಮುಂದಿನ(ರಚನೆ am_state *state)
{
	ರಚನೆ object_id ತಲೆ;

	FREE_AND_NULL(state->author_name);
	FREE_AND_NULL(state->author_email);
	FREE_AND_NULL(state->author_date);
	FREE_AND_NULL(state->ಸಂದೇಶ);
	state->ಸಂದೇಶ_len = 0;

	unlink(am_ಮಾರ್ಗ(state, "author-script"));
	unlink(am_ಮಾರ್ಗ(state, "final-commit"));

	oidclr(&state->orig_commit, the_repository->hash_algo);
	unlink(am_ಮಾರ್ಗ(state, "original-commit"));
	refs_delete_ref(get_main_ref_store(the_repository), ಖಾಲಿ,
			"REBASE_HEAD", ಖಾಲಿ, REF_NO_DEREF);

	ಆದರೆ (!repo_get_oid(the_repository, "HEAD", &ತಲೆ))
		write_state_ಪಠ್ಯ(state, "abort-safety", oid_ಗಮ್ಯ_hex(&ತಲೆ));
	ಇಲ್ಲದಿದ್ದರೆ
		write_state_ಪಠ್ಯ(state, "abort-safety", "");

	state->cur++;
	write_state_ಎಣಿಕೆ(state, "next", state->cur);
}

/**
 * Returns the filename of the current patch email.
 */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *msgnum(ಸ್ಥಿರಾಂಕ ರಚನೆ am_state *state)
{
	ಸ್ಥಿರ ರಚನೆ strbuf sb = STRBUF_INIT;

	strbuf_reset(&sb);
	strbuf_addf(&sb, "%0*d", state->prec, state->cur);

	ಹಿಂತಿರುಗಿ sb.buf;
}

/**
 * Dies with a user-friendly message on how to proceed after resolving the
 * problem. This message can be overridden with state->resolvemsg.
 */
ಸ್ಥಿರ ಶೂನ್ಯ NORETURN die_user_resolve(ಸ್ಥಿರಾಂಕ ರಚನೆ am_state *state)
{
	ಆದರೆ (state->resolvemsg) {
		advise_if_enabled(ADVICE_ವಿಲೀನ_CONFLICT, "%s", state->resolvemsg);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cmdline = state->interactive ? "git am -i" : "git am";
		ರಚನೆ strbuf sb = STRBUF_INIT;

		strbuf_addf(&sb, _("When you have resolved this problem, run \"%s --continue\".\n"), cmdline);
		strbuf_addf(&sb, _("If you prefer to skip this patch, run \"%s --skip\" instead.\n"), cmdline);

		ಆದರೆ (advice_enabled(ADVICE_AM_WORK_DIR) &&
		    is_ಖಾಲಿಗಳು_or_missing_file(am_ಮಾರ್ಗ(state, "patch")) &&
		    !repo_ಸೂಚ್ಯಂಕ_has_changes(the_repository, ಖಾಲಿ, ಖಾಲಿ))
			strbuf_addf(&sb, _("To record the empty patch as an empty commit, run \"%s --allow-empty\".\n"), cmdline);

		strbuf_addf(&sb, _("To restore the original branch and stop patching, run \"%s --abort\"."), cmdline);

		advise_if_enabled(ADVICE_ವಿಲೀನ_CONFLICT, "%s", sb.buf);
		strbuf_release(&sb);
	}

	die(ಖಾಲಿ);
}

/**
 * Appends signoff to the "msg" field of the am_state.
 */
ಸ್ಥಿರ ಶೂನ್ಯ am_append_signoff(ರಚನೆ am_state *state)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;

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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_mail(ರಚನೆ am_state *state, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *mail)
{
	FILE *fp;
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ರಚನೆ strbuf ಸಂದೇಶ = STRBUF_INIT;
	ರಚನೆ strbuf author_name = STRBUF_INIT;
	ರಚನೆ strbuf author_date = STRBUF_INIT;
	ರಚನೆ strbuf author_email = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ ret = 0;
	ರಚನೆ mailinfo mi;

	ಸಿದ್ಧತೆ_mailinfo(the_repository, &mi);

	ಆದರೆ (state->utf8)
		mi.metainfo_charset = get_commit_output_encoding();
	ಇಲ್ಲದಿದ್ದರೆ
		mi.metainfo_charset = ಖಾಲಿ;

	ಆಯ್ಕೆ (state->keep) {
	ಸ್ಥಿತಿ KEEP_FALSE:
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ KEEP_TRUE:
		mi.keep_subject = 1;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ KEEP_NON_PATCH:
		mi.keep_non_patch_brackets_ಒಳ_subject = 1;
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("invalid value for state->keep");
	}

	ಆದರೆ (state->ಸಂದೇಶ_id)
		mi.ಕೂಡಿಸು_ಸಂದೇಶ_id = 1;

	ಆಯ್ಕೆ (state->scissors) {
	ಸ್ಥಿತಿ SCISSORS_UNSET:
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SCISSORS_FALSE:
		mi.use_scissors = 0;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SCISSORS_TRUE:
		mi.use_scissors = 1;
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("invalid value for state->scissors");
	}

	ಆಯ್ಕೆ (state->quoted_cr) {
	ಸ್ಥಿತಿ quoted_cr_unset:
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ quoted_cr_nowarn:
	ಸ್ಥಿತಿ quoted_cr_warn:
	ಸ್ಥಿತಿ quoted_cr_strip:
		mi.quoted_cr = state->quoted_cr;
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("invalid value for state->quoted_cr");
	}

	mi.ಒಳಹರಿವು = xfopen(mail, "r");
	mi.output = xfopen(am_ಮಾರ್ಗ(state, "info"), "w");
	ಆದರೆ (mailinfo(&mi, am_ಮಾರ್ಗ(state, "msg"), am_ಮಾರ್ಗ(state, "patch")))
		die("could not parse patch");

	fclose(mi.ಒಳಹರಿವು);
	fclose(mi.output);

	ಆದರೆ (mi.format_flowed)
		warning(_("Patch sent with format=flowed; "
			  "space at the end of lines might be lost."));

	/* Extract message and author information */
	fp = xfopen(am_ಮಾರ್ಗ(state, "info"), "r");
	ಆಗ (!strbuf_getline_lf(&sb, fp)) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *x;

		ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(sb.buf, "Subject: ", &x)) {
			ಆದರೆ (ಸಂದೇಶ.len)
				strbuf_addch(&ಸಂದೇಶ, '\n');
			strbuf_addstr(&ಸಂದೇಶ, x);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(sb.buf, "Author: ", &x))
			strbuf_addstr(&author_name, x);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(sb.buf, "Email: ", &x))
			strbuf_addstr(&author_email, x);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(sb.buf, "Date: ", &x))
			strbuf_addstr(&author_date, x);
	}
	fclose(fp);

	/* Skip pine's internal folder data */
	ಆದರೆ (!strcmp(author_name.buf, "Mail System Internal Data")) {
		ret = 1;
		ಹೋಗು ಮುಕ್ತಾಯ;
	}

	strbuf_addstr(&ಸಂದೇಶ, "\n\n");
	strbuf_addbuf(&ಸಂದೇಶ, &mi.log_ಸಂದೇಶ);
	strbuf_stripspace(&ಸಂದೇಶ, ಖಾಲಿ);

	ಪ್ರತಿಜ್ಞಾ(!state->author_name);
	state->author_name = strbuf_detach(&author_name, ಖಾಲಿ);

	ಪ್ರತಿಜ್ಞಾ(!state->author_email);
	state->author_email = strbuf_detach(&author_email, ಖಾಲಿ);

	ಪ್ರತಿಜ್ಞಾ(!state->author_date);
	state->author_date = strbuf_detach(&author_date, ಖಾಲಿ);

	ಪ್ರತಿಜ್ಞಾ(!state->ಸಂದೇಶ);
	state->ಸಂದೇಶ = strbuf_detach(&ಸಂದೇಶ, &state->ಸಂದೇಶ_len);

ಮುಕ್ತಾಯ:
	strbuf_release(&ಸಂದೇಶ);
	strbuf_release(&author_date);
	strbuf_release(&author_email);
	strbuf_release(&author_name);
	strbuf_release(&sb);
	clear_mailinfo(&mi);
	ಹಿಂತಿರುಗಿ ret;
}

/**
 * Sets commit_id to the commit hash where the mail was generated from.
 * Returns 0 on success, -1 on failure.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_mail_commit_oid(ರಚನೆ object_id *commit_id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *mail)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;
	FILE *fp = xfopen(mail, "r");
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *x;
	ಪೂರ್ಣಾಂಕ ret = 0;

	ಆದರೆ (strbuf_getline_lf(&sb, fp) ||
	    !skip_ಪೂರ್ವಪ್ರತ್ಯಯ(sb.buf, "From ", &x) ||
	    get_oid_hex(x, commit_id) < 0)
		ret = -1;

	strbuf_release(&sb);
	fclose(fp);
	ಹಿಂತಿರುಗಿ ret;
}

/**
 * Sets state->msg, state->author_name, state->author_email, state->author_date
 * to the commit's respective info.
 */
ಸ್ಥಿರ ಶೂನ್ಯ get_commit_info(ರಚನೆ am_state *state, ರಚನೆ commit *commit)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಫರ್, *ident_ಸಾಲು, *ಸಂದೇಶ;
	ಗಾತ್ರ_t ident_len;
	ರಚನೆ ident_ವಿಭಜಿಸು id;

	ಬಫರ್ = repo_logmsg_reencode(the_repository, commit, ಖಾಲಿ,
				      get_commit_output_encoding());

	ident_ಸಾಲು = ಕಂಡುಹಿಡಿ_commit_ಶೀರ್ಷಿಕೆ(ಬಫರ್, "author", &ident_len);
	ಆದರೆ (!ident_ಸಾಲು)
		die(_("missing author line in commit %s"),
		      oid_ಗಮ್ಯ_hex(&commit->object.oid));
	ಆದರೆ (ವಿಭಜಿಸು_ident_ಸಾಲು(&id, ident_ಸಾಲು, ident_len) < 0)
		die(_("invalid ident line: %.*s"), (ಪೂರ್ಣಾಂಕ)ident_len, ident_ಸಾಲು);

	ಪ್ರತಿಜ್ಞಾ(!state->author_name);
	ಆದರೆ (id.name_begin)
		state->author_name =
			xmemdupz(id.name_begin, id.name_ಕೊನೆ - id.name_begin);
	ಇಲ್ಲದಿದ್ದರೆ
		state->author_name = xstrdup("");

	ಪ್ರತಿಜ್ಞಾ(!state->author_email);
	ಆದರೆ (id.mail_begin)
		state->author_email =
			xmemdupz(id.mail_begin, id.mail_ಕೊನೆ - id.mail_begin);
	ಇಲ್ಲದಿದ್ದರೆ
		state->author_email = xstrdup("");

	ಪ್ರತಿಜ್ಞಾ(!state->author_date);
	state->author_date = xstrdup(show_ident_date(&id, DATE_ಬಹುಲಕ(ಸಾಮಾನ್ಯ)));

	ಪ್ರತಿಜ್ಞಾ(!state->ಸಂದೇಶ);
	ಸಂದೇಶ = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(ಬಫರ್, "\n\n");
	ಆದರೆ (!ಸಂದೇಶ)
		die(_("unable to parse commit %s"), oid_ಗಮ್ಯ_hex(&commit->object.oid));
	state->ಸಂದೇಶ = xstrdup(ಸಂದೇಶ + 2);
	state->ಸಂದೇಶ_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(state->ಸಂದೇಶ);
	repo_unuse_commit_ಬಫರ್(the_repository, commit, ಬಫರ್);
}

/**
 * Writes `commit` as a patch to the state directory's "patch" file.
 */
ಸ್ಥಿರ ಶೂನ್ಯ write_commit_patch(ಸ್ಥಿರಾಂಕ ರಚನೆ am_state *state, ರಚನೆ commit *commit)
{
	ರಚನೆ rev_info rev_info;
	FILE *fp;

	fp = xfopen(am_ಮಾರ್ಗ(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, ಖಾಲಿ);
	rev_info.diff = 1;
	rev_info.abbrev = 0;
	rev_info.disable_stdin = 1;
	rev_info.show_ಬೇರು_diff = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.flags.ದ್ವಿಮಾನ = 1;
	rev_info.diffopt.flags.ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 1;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	ಕೂಡಿಸು_pending_object(&rev_info, &commit->object, "");
	diff_ಸಿದ್ಧತೆ_ಮುಗಿದ(&rev_info.diffopt);
	log_ವೃಕ್ಷ_commit(&rev_info, commit);
	release_revisions(&rev_info);
}

/**
 * Writes the diff of the index against HEAD as a patch to the state
 * directory's "patch" file.
 */
ಸ್ಥಿರ ಶೂನ್ಯ write_ಸೂಚ್ಯಂಕ_patch(ಸ್ಥಿರಾಂಕ ರಚನೆ am_state *state)
{
	ರಚನೆ ವೃಕ್ಷ *ವೃಕ್ಷ;
	ರಚನೆ object_id ತಲೆ;
	ರಚನೆ rev_info rev_info;
	FILE *fp;

	ಆದರೆ (!repo_get_oid(the_repository, "HEAD", &ತಲೆ)) {
		ರಚನೆ commit *commit = lookup_commit_or_die(&ತಲೆ, "HEAD");
		ವೃಕ್ಷ = repo_get_commit_ವೃಕ್ಷ(the_repository, commit);
	} ಇಲ್ಲದಿದ್ದರೆ
		ವೃಕ್ಷ = lookup_ವೃಕ್ಷ(the_repository,
				   the_repository->hash_algo->ಖಾಲಿಗಳು_ವೃಕ್ಷ);

	fp = xfopen(am_ಮಾರ್ಗ(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, ಖಾಲಿ);
	rev_info.diff = 1;
	rev_info.disable_stdin = 1;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	ಕೂಡಿಸು_pending_object(&rev_info, &ವೃಕ್ಷ->object, "");
	diff_ಸಿದ್ಧತೆ_ಮುಗಿದ(&rev_info.diffopt);
	ಸತತ_diff_ಸೂಚ್ಯಂಕ(&rev_info, DIFF_ಸೂಚ್ಯಂಕ_CACHED);
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_mail_rebase(ರಚನೆ am_state *state, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *mail)
{
	ರಚನೆ commit *commit;
	ರಚನೆ object_id commit_oid;

	ಆದರೆ (get_mail_commit_oid(&commit_oid, mail) < 0)
		die(_("could not parse %s"), mail);

	commit = lookup_commit_or_die(&commit_oid, mail);

	get_commit_info(state, commit);

	write_commit_patch(state, commit);

	oidcpy(&state->orig_commit, &commit_oid);
	write_state_ಪಠ್ಯ(state, "original-commit", oid_ಗಮ್ಯ_hex(&commit_oid));
	refs_update_ref(get_main_ref_store(the_repository), "am",
			"REBASE_HEAD", &commit_oid,
			ಖಾಲಿ, REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);

	ಹಿಂತಿರುಗಿ 0;
}

/**
 * Applies current patch with git-apply. Returns 0 on success, -1 otherwise. If
 * `index_file` is not NULL, the patch will be applied to that index.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸತತ_apply(ಸ್ಥಿರಾಂಕ ರಚನೆ am_state *state, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸೂಚ್ಯಂಕ_file)
{
	ರಚನೆ strvec apply_paths = STRVEC_INIT;
	ರಚನೆ strvec apply_opts = STRVEC_INIT;
	ರಚನೆ apply_state apply_state;
	ಪೂರ್ಣಾಂಕ res, opts_ಎಡ;
	ಪೂರ್ಣಾಂಕ force_apply = 0;
	ಪೂರ್ಣಾಂಕ options = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **apply_argv;

	ಆದರೆ (init_apply_state(&apply_state, the_repository, ಖಾಲಿ))
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
					ಖಾಲಿ);

	ಆದರೆ (opts_ಎಡ != 0)
		die("unknown option passed through to git apply");

	ಆದರೆ (ಸೂಚ್ಯಂಕ_file) {
		apply_state.ಸೂಚ್ಯಂಕ_file = ಸೂಚ್ಯಂಕ_file;
		apply_state.cached = 1;
	} ಇಲ್ಲದಿದ್ದರೆ
		apply_state.check_ಸೂಚ್ಯಂಕ = 1;

	/*
	 * If we are allowed to fall back on 3-way merge, don't give false
	 * errors during the initial attempt.
	 */
	ಆದರೆ (state->threeway && !ಸೂಚ್ಯಂಕ_file)
		apply_state.apply_verbosity = verbosity_silent;

	ಆದರೆ (check_apply_state(&apply_state, force_apply))
		BUG("check_apply_state() failed");

	strvec_ತಳ್ಳು(&apply_paths, am_ಮಾರ್ಗ(state, "patch"));

	res = apply_all_patches(&apply_state, apply_paths.nr, apply_paths.v, options);

	strvec_clear(&apply_paths);
	strvec_clear(&apply_opts);
	clear_apply_state(&apply_state);
	ಬಿಡುಗಡೆ(apply_argv);

	ಆದರೆ (res)
		ಹಿಂತಿರುಗಿ res;

	ಆದರೆ (ಸೂಚ್ಯಂಕ_file) {
		/* Reload index as apply_all_patches() will have modified it. */
		discard_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);
		read_ಸೂಚ್ಯಂಕ_from(the_repository->ಸೂಚ್ಯಂಕ, ಸೂಚ್ಯಂಕ_file,
				repo_get_git_dir(the_repository));
	}

	ಹಿಂತಿರುಗಿ 0;
}

/**
 * Builds an index that contains just the blobs needed for a 3way merge.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ build_fake_ancestor(ಸ್ಥಿರಾಂಕ ರಚನೆ am_state *state, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸೂಚ್ಯಂಕ_file)
{
	ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;

	cp.git_cmd = 1;
	strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, "apply");
	strvec_pushv(&cp.ವಾದಗಳು, state->git_apply_opts.v);
	strvec_pushf(&cp.ವಾದಗಳು, "--build-fake-ancestor=%s", ಸೂಚ್ಯಂಕ_file);
	strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, am_ಮಾರ್ಗ(state, "patch"));

	ಆದರೆ (ಸತತ_command(&cp))
		ಹಿಂತಿರುಗಿ -1;

	ಹಿಂತಿರುಗಿ 0;
}

/**
 * Attempt a threeway merge, using index_path as the temporary index.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fall_ಹಿಂಭಾಗ_threeway(ಸ್ಥಿರಾಂಕ ರಚನೆ am_state *state, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ)
{
	ರಚನೆ object_id their_ವೃಕ್ಷ, our_ವೃಕ್ಷ;
	ರಚನೆ object_id bases[1] = { 0 };
	ರಚನೆ ವಿಲೀನ_options o;
	ರಚನೆ commit *ಫಲಿತಾಂಶ;
	ಅಕ್ಷರ *their_ವೃಕ್ಷ_name;

	ಆದರೆ (repo_get_oid(the_repository, "HEAD", &our_ವೃಕ್ಷ) < 0)
		oidcpy(&our_ವೃಕ್ಷ, the_hash_algo->ಖಾಲಿಗಳು_ವೃಕ್ಷ);

	ಆದರೆ (build_fake_ancestor(state, ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ))
		ಹಿಂತಿರುಗಿ error("could not build fake ancestor");

	discard_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);
	read_ಸೂಚ್ಯಂಕ_from(the_repository->ಸೂಚ್ಯಂಕ, ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ, repo_get_git_dir(the_repository));

	ಆದರೆ (write_ಸೂಚ್ಯಂಕ_as_ವೃಕ್ಷ(&bases[0], the_repository->ಸೂಚ್ಯಂಕ, ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ, 0, ಖಾಲಿ))
		ಹಿಂತಿರುಗಿ error(_("Repository lacks necessary blobs to fall back on 3-way merge."));

	say(state, stdout, _("Using index info to reconstruct a base tree..."));

	ಆದರೆ (!state->quiet) {
		/*
		 * List paths that needed 3-way fallback, so that the user can
		 * review them with extra care to spot mismerges.
		 */
		ರಚನೆ rev_info rev_info;

		repo_init_revisions(the_repository, &rev_info, ಖಾಲಿ);
		rev_info.diffopt.output_format = DIFF_FORMAT_NAME_ಸ್ಥಿತಿ;
		rev_info.diffopt.filter |= diff_filter_ಬಿಟ್('A');
		rev_info.diffopt.filter |= diff_filter_ಬಿಟ್('M');
		ಕೂಡಿಸು_pending_oid(&rev_info, "HEAD", &our_ವೃಕ್ಷ, 0);
		diff_ಸಿದ್ಧತೆ_ಮುಗಿದ(&rev_info.diffopt);
		ಸತತ_diff_ಸೂಚ್ಯಂಕ(&rev_info, DIFF_ಸೂಚ್ಯಂಕ_CACHED);
		release_revisions(&rev_info);
	}

	ಆದರೆ (ಸತತ_apply(state, ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ))
		ಹಿಂತಿರುಗಿ error(_("Did you hand edit your patch?\n"
				"It does not apply to blobs recorded in its index."));

	ಆದರೆ (write_ಸೂಚ್ಯಂಕ_as_ವೃಕ್ಷ(&their_ವೃಕ್ಷ, the_repository->ಸೂಚ್ಯಂಕ, ಸೂಚ್ಯಂಕ_ಮಾರ್ಗ, 0, ಖಾಲಿ))
		ಹಿಂತಿರುಗಿ error("could not write tree");

	say(state, stdout, _("Falling back to patching base and 3-way merge..."));

	discard_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ);
	repo_read_ಸೂಚ್ಯಂಕ(the_repository);

	/*
	 * This is not so wrong. Depending on which base we picked, orig_tree
	 * may be wildly different from ours, but their_tree has the same set of
	 * wildly different changes in parts the patch did not touch, so
	 * recursive ends up canceling them, saying that we reverted all those
	 * changes.
	 */

	init_ui_ವಿಲೀನ_options(&o, the_repository);

	o.branch1 = "HEAD";
	their_ವೃಕ್ಷ_name = xstrfmt("%.*s", linelen(state->ಸಂದೇಶ), state->ಸಂದೇಶ);
	o.branch2 = their_ವೃಕ್ಷ_name;
	o.ancestor = "constructed fake ancestor";
	o.detect_directory_renames = ವಿಲೀನ_DIRECTORY_RENAMES_NONE;

	ಆದರೆ (state->quiet)
		o.verbosity = 0;

	ಆದರೆ (ವಿಲೀನ_ort_generic(&o, &our_ವೃಕ್ಷ, &their_ವೃಕ್ಷ, 1, bases, &ಫಲಿತಾಂಶ)) {
		repo_rerere(the_repository, state->allow_rerere_autoupdate);
		ಬಿಡುಗಡೆ(their_ವೃಕ್ಷ_name);
		ಹಿಂತಿರುಗಿ error(_("Failed to merge in the changes."));
	}

	ಬಿಡುಗಡೆ(their_ವೃಕ್ಷ_name);
	ಹಿಂತಿರುಗಿ 0;
}

/**
 * Commits the current index with state->msg as the commit message and
 * state->author_name, state->author_email and state->author_date as the author
 * information.
 */
ಸ್ಥಿರ ಶೂನ್ಯ do_commit(ಸ್ಥಿರಾಂಕ ರಚನೆ am_state *state)
{
	ರಚನೆ object_id ವೃಕ್ಷ, ಪೋಷಕ, commit;
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *old_oid;
	ರಚನೆ commit_list *parents = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *reflog_ಸಂದೇಶ, *author, *committer = ಖಾಲಿ;
	ರಚನೆ strbuf sb = STRBUF_INIT;

	ಆದರೆ (!state->no_verify && ಸತತ_hooks(the_repository, "pre-applypatch"))
		ನಿರ್ಗಮಿಸು(1);

	ಆದರೆ (write_ಸೂಚ್ಯಂಕ_as_ವೃಕ್ಷ(&ವೃಕ್ಷ, the_repository->ಸೂಚ್ಯಂಕ,
				repo_get_ಸೂಚ್ಯಂಕ_file(the_repository),
				0, ಖಾಲಿ))
		die(_("git write-tree failed to write a tree"));

	ಆದರೆ (!repo_get_oid_commit(the_repository, "HEAD", &ಪೋಷಕ)) {
		old_oid = &ಪೋಷಕ;
		commit_list_ಸೇರಿಸು(lookup_commit(the_repository, &ಪೋಷಕ),
				   &parents);
	} ಇಲ್ಲದಿದ್ದರೆ {
		old_oid = ಖಾಲಿ;
		say(state, stderr, _("applying to an empty history"));
	}

	author = fmt_ident(state->author_name, state->author_email,
		WANT_AUTHOR_IDENT,
			state->ignore_date ? ಖಾಲಿ : state->author_date,
			IDENT_STRICT);

	ಆದರೆ (state->committer_date_is_author_date)
		committer = fmt_ident(getenv("GIT_COMMITTER_NAME"),
				      getenv("GIT_COMMITTER_EMAIL"),
				      WANT_COMMITTER_IDENT,
				      state->ignore_date ? ಖಾಲಿ
							 : state->author_date,
				      IDENT_STRICT);

	ಆದರೆ (commit_ವೃಕ್ಷ_extended(state->ಸಂದೇಶ, state->ಸಂದೇಶ_len, &ವೃಕ್ಷ, parents,
				 &commit, author, committer, state->sign_commit,
				 ಖಾಲಿ))
		die(_("failed to write commit object"));

	reflog_ಸಂದೇಶ = getenv("GIT_REFLOG_ACTION");
	ಆದರೆ (!reflog_ಸಂದೇಶ)
		reflog_ಸಂದೇಶ = "am";

	strbuf_addf(&sb, "%s: %.*s", reflog_ಸಂದೇಶ, linelen(state->ಸಂದೇಶ),
			state->ಸಂದೇಶ);

	refs_update_ref(get_main_ref_store(the_repository), sb.buf, "HEAD",
			&commit, old_oid, 0,
			UPDATE_REFS_DIE_ON_ERR);

	ಆದರೆ (state->rebasing) {
		FILE *fp = xfopen(am_ಮಾರ್ಗ(state, "rewritten"), "a");

		ಪ್ರತಿಜ್ಞಾ(!is_null_oid(&state->orig_commit));
		fprintf(fp, "%s ", oid_ಗಮ್ಯ_hex(&state->orig_commit));
		fprintf(fp, "%s\n", oid_ಗಮ್ಯ_hex(&commit));
		fclose(fp);
	}

	ಸತತ_hooks(the_repository, "post-applypatch");

	commit_list_free(parents);
	strbuf_release(&sb);
}

/**
 * Validates the am_state for resuming -- the "msg" and authorship fields must
 * be filled up.
 */
ಸ್ಥಿರ ಶೂನ್ಯ validate_resume_state(ಸ್ಥಿರಾಂಕ ರಚನೆ am_state *state)
{
	ಆದರೆ (!state->ಸಂದೇಶ)
		die(_("cannot resume: %s does not exist."),
			am_ಮಾರ್ಗ(state, "final-commit"));

	ಆದರೆ (!state->author_name || !state->author_email || !state->author_date)
		die(_("cannot resume: %s does not exist."),
			am_ಮಾರ್ಗ(state, "author-script"));
}

/**
 * Interactively prompt the user on whether the current patch should be
 * applied.
 *
 * Returns 0 if the user chooses to apply the patch, 1 if the user chooses to
 * skip it.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_interactive(ರಚನೆ am_state *state)
{
	ಪ್ರತಿಜ್ಞಾ(state->ಸಂದೇಶ);

	ಗಾಗಿ (;;) {
		ಅಕ್ಷರ ಉತ್ತರ[64];

		puts(_("Commit Body is:"));
		puts("--------------------------");
		ಬರೆ("%s", state->ಸಂದೇಶ);
		puts("--------------------------");

		/*
		 * TRANSLATORS: Make sure to include [y], [n], [e], [v] and [a]
		 * in your translation. The program will only accept English
		 * input at this point.
		 */
		ಬರೆ(_("Apply? [y]es/[n]o/[e]dit/[v]iew patch/[a]ccept all: "));
		ಆದರೆ (!fgets(ಉತ್ತರ, ಗಾತ್ರ(ಉತ್ತರ), stdin))
			die("unable to read from stdin; aborting");

		ಆದರೆ (*ಉತ್ತರ == 'y' || *ಉತ್ತರ == 'Y') {
			ಹಿಂತಿರುಗಿ 0;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*ಉತ್ತರ == 'a' || *ಉತ್ತರ == 'A') {
			state->interactive = 0;
			ಹಿಂತಿರುಗಿ 0;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*ಉತ್ತರ == 'n' || *ಉತ್ತರ == 'N') {
			ಹಿಂತಿರುಗಿ 1;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*ಉತ್ತರ == 'e' || *ಉತ್ತರ == 'E') {
			ರಚನೆ strbuf ಸಂದೇಶ = STRBUF_INIT;

			ಆದರೆ (!launch_editor(am_ಮಾರ್ಗ(state, "final-commit"), &ಸಂದೇಶ, ಖಾಲಿ)) {
				ಬಿಡುಗಡೆ(state->ಸಂದೇಶ);
				state->ಸಂದೇಶ = strbuf_detach(&ಸಂದೇಶ, &state->ಸಂದೇಶ_len);
			}
			strbuf_release(&ಸಂದೇಶ);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*ಉತ್ತರ == 'v' || *ಉತ್ತರ == 'V') {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pager = git_pager(the_repository, 1);
			ರಚನೆ ಮಗು_process cp = ಮಗು_PROCESS_INIT;

			ಆದರೆ (!pager)
				pager = "cat";
			prepare_pager_ವಾದಗಳು(&cp, pager);
			strvec_ತಳ್ಳು(&cp.ವಾದಗಳು, am_ಮಾರ್ಗ(state, "patch"));
			ಸತತ_command(&cp);
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
ಸ್ಥಿರ ಶೂನ್ಯ am_ಸತತ(ರಚನೆ am_state *state, ಪೂರ್ಣಾಂಕ resume)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;

	unlink(am_ಮಾರ್ಗ(state, "dirtyindex"));

	ಆದರೆ (repo_refresh_and_write_ಸೂಚ್ಯಂಕ(the_repository, REFRESH_QUIET, 0, 0,
					 ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ) < 0)
		die(_("unable to write index file"));

	ಆದರೆ (repo_ಸೂಚ್ಯಂಕ_has_changes(the_repository, ಖಾಲಿ, &sb)) {
		write_state_bool(state, "dirtyindex", 1);
		die(_("Dirty index: cannot apply patches (dirty: %s)"), sb.buf);
	}

	strbuf_release(&sb);

	ಆಗ (state->cur <= state->ಕೊನೆಯ) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *mail = am_ಮಾರ್ಗ(state, msgnum(state));
		ಪೂರ್ಣಾಂಕ apply_ಸ್ಥಿತಿ;
		ಪೂರ್ಣಾಂಕ ಗಮ್ಯ_keep;

		reset_ident_date();

		ಆದರೆ (!file_exists(mail))
			ಹೋಗು ಮುಂದಿನ;

		ಆದರೆ (resume) {
			validate_resume_state(state);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಪೂರ್ಣಾಂಕ skip;

			ಆದರೆ (state->rebasing)
				skip = parse_mail_rebase(state, mail);
			ಇಲ್ಲದಿದ್ದರೆ
				skip = parse_mail(state, mail);

			ಆದರೆ (skip)
				ಹೋಗು ಮುಂದಿನ; /* mail should be skipped */

			ಆದರೆ (state->signoff)
				am_append_signoff(state);

			write_author_script(state);
			write_commit_ಸಂದೇಶ(state);
		}

		ಆದರೆ (state->interactive && do_interactive(state))
			ಹೋಗು ಮುಂದಿನ;

		ಗಮ್ಯ_keep = 0;
		ಆದರೆ (is_ಖಾಲಿಗಳು_or_missing_file(am_ಮಾರ್ಗ(state, "patch"))) {
			ಆಯ್ಕೆ (state->ಖಾಲಿಗಳು_type) {
			ಸ್ಥಿತಿ DROP_ಖಾಲಿಗಳು_COMMIT:
				say(state, stdout, _("Skipping: %.*s"), linelen(state->ಸಂದೇಶ), state->ಸಂದೇಶ);
				ಹೋಗು ಮುಂದಿನ;
				ನಿಲ್ಲಿಸು;
			ಸ್ಥಿತಿ KEEP_ಖಾಲಿಗಳು_COMMIT:
				ಗಮ್ಯ_keep = 1;
				say(state, stdout, _("Creating an empty commit: %.*s"),
					linelen(state->ಸಂದೇಶ), state->ಸಂದೇಶ);
				ನಿಲ್ಲಿಸು;
			ಸ್ಥಿತಿ STOP_ON_ಖಾಲಿಗಳು_COMMIT:
				printf_ln(_("Patch is empty."));
				die_user_resolve(state);
				ನಿಲ್ಲಿಸು;
			}
		}

		ಆದರೆ (ಸತತ_applypatch_ಸಂದೇಶ_hook(state))
			ನಿರ್ಗಮಿಸು(1);
		ಆದರೆ (ಗಮ್ಯ_keep)
			ಹೋಗು commit;

		say(state, stdout, _("Applying: %.*s"), linelen(state->ಸಂದೇಶ), state->ಸಂದೇಶ);

		apply_ಸ್ಥಿತಿ = ಸತತ_apply(state, ಖಾಲಿ);

		ಆದರೆ (apply_ಸ್ಥಿತಿ && state->threeway) {
			ರಚನೆ strbuf sb = STRBUF_INIT;

			strbuf_addstr(&sb, am_ಮಾರ್ಗ(state, "patch-merge-index"));
			apply_ಸ್ಥಿತಿ = fall_ಹಿಂಭಾಗ_threeway(state, sb.buf);
			strbuf_release(&sb);

			/*
			 * Applying the patch to an earlier tree and merging
			 * the result may have produced the same tree as ours.
			 */
			ಆದರೆ (!apply_ಸ್ಥಿತಿ &&
			    !repo_ಸೂಚ್ಯಂಕ_has_changes(the_repository, ಖಾಲಿ, ಖಾಲಿ)) {
				say(state, stdout, _("No changes -- Patch already applied."));
				ಹೋಗು ಮುಂದಿನ;
			}
		}

		ಆದರೆ (apply_ಸ್ಥಿತಿ) {
			printf_ln(_("Patch failed at %s %.*s"), msgnum(state),
				linelen(state->ಸಂದೇಶ), state->ಸಂದೇಶ);

			ಆದರೆ (advice_enabled(ADVICE_AM_WORK_DIR))
				advise(_("Use 'git am --show-current-patch=diff' to see the failed patch"));

			die_user_resolve(state);
		}

commit:
		do_commit(state);

ಮುಂದಿನ:
		am_ಮುಂದಿನ(state);

		ಆದರೆ (resume)
			am_ತುಂಬು(state);
		resume = 0;
	}

	ಆದರೆ (!is_ಖಾಲಿಗಳು_or_missing_file(am_ಮಾರ್ಗ(state, "rewritten"))) {
		ಪ್ರತಿಜ್ಞಾ(state->rebasing);
		copy_notes_for_rebase(state);
		ಸತತ_post_rewrite_hook(state);
	}

	/*
	 * In rebasing mode, it's up to the caller to take care of
	 * housekeeping.
	 */
	ಆದರೆ (!state->rebasing) {
		am_destroy(state);
		ಸತತ_auto_maintenance(the_repository, state->quiet);
	}
}

/**
 * Resume the current am session after patch application failure. The user did
 * all the hard work, and we do not have to do any patch application. Just
 * trust and commit what the user has in the index and working tree. If `allow_empty`
 * is true, commit as an empty commit when index has not changed and lacking a patch.
 */
ಸ್ಥಿರ ಶೂನ್ಯ am_resolve(ರಚನೆ am_state *state, ಪೂರ್ಣಾಂಕ allow_ಖಾಲಿಗಳು)
{
	validate_resume_state(state);

	say(state, stdout, _("Applying: %.*s"), linelen(state->ಸಂದೇಶ), state->ಸಂದೇಶ);

	ಆದರೆ (!repo_ಸೂಚ್ಯಂಕ_has_changes(the_repository, ಖಾಲಿ, ಖಾಲಿ)) {
		ಆದರೆ (allow_ಖಾಲಿಗಳು && is_ಖಾಲಿಗಳು_or_missing_file(am_ಮಾರ್ಗ(state, "patch"))) {
			printf_ln(_("No changes - recorded it as an empty commit."));
		} ಇಲ್ಲದಿದ್ದರೆ {
			printf_ln(_("No changes - did you forget to use 'git add'?\n"
				    "If there is nothing left to stage, chances are that something else\n"
				    "already introduced the same changes; you might want to skip this patch."));
			die_user_resolve(state);
		}
	}

	ಆದರೆ (unmerged_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ)) {
		printf_ln(_("You still have unmerged paths in your index.\n"
			"You should 'git add' each file with resolved conflicts to mark them as such.\n"
			"You might run `git rm` on a file to accept \"deleted by them\" for it."));
		die_user_resolve(state);
	}

	ಆದರೆ (state->interactive) {
		write_ಸೂಚ್ಯಂಕ_patch(state);
		ಆದರೆ (do_interactive(state))
			ಹೋಗು ಮುಂದಿನ;
	}

	repo_rerere(the_repository, 0);

	do_commit(state);

ಮುಂದಿನ:
	am_ಮುಂದಿನ(state);
	am_ತುಂಬು(state);
	am_ಸತತ(state, 0);
}

/**
 * Performs a checkout fast-forward from `head` to `remote`. If `reset` is
 * true, any unmerged entries will be discarded. Returns 0 on success, -1 on
 * failure.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ fast_forward_ಗಮ್ಯ(ರಚನೆ ವೃಕ್ಷ *ತಲೆ, ರಚನೆ ವೃಕ್ಷ *remote, ಪೂರ್ಣಾಂಕ reset)
{
	ರಚನೆ ಬೀಗ_file ಬೀಗ_file = ಬೀಗ_INIT;
	ರಚನೆ unpack_trees_options opts;
	ರಚನೆ ವೃಕ್ಷ_desc t[2];

	ಆದರೆ (repo_parse_ವೃಕ್ಷ(the_repository, ತಲೆ) || repo_parse_ವೃಕ್ಷ(the_repository, remote))
		ಹಿಂತಿರುಗಿ -1;

	repo_hold_locked_ಸೂಚ್ಯಂಕ(the_repository, &ಬೀಗ_file, ಬೀಗ_DIE_ON_ERROR);

	refresh_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, REFRESH_QUIET, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ);

	memset(&opts, 0, ಗಾತ್ರ(opts));
	opts.ತಲೆ_idx = 1;
	opts.src_ಸೂಚ್ಯಂಕ = the_repository->ಸೂಚ್ಯಂಕ;
	opts.dst_ಸೂಚ್ಯಂಕ = the_repository->ಸೂಚ್ಯಂಕ;
	opts.update = 1;
	opts.ವಿಲೀನ = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = twoway_ವಿಲೀನ;
	init_ವೃಕ್ಷ_desc(&t[0], &ತಲೆ->object.oid, ತಲೆ->ಬಫರ್, ತಲೆ->ಗಾತ್ರ_2);
	init_ವೃಕ್ಷ_desc(&t[1], &remote->object.oid, remote->ಬಫರ್, remote->ಗಾತ್ರ_2);

	ಆದರೆ (unpack_trees(2, t, &opts)) {
		rollback_ಬೀಗ_file(&ಬೀಗ_file);
		ಹಿಂತಿರುಗಿ -1;
	}

	ಆದರೆ (write_locked_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, &ಬೀಗ_file, COMMIT_ಬೀಗ))
		die(_("unable to write new index file"));

	ಹಿಂತಿರುಗಿ 0;
}

/**
 * Merges a tree into the index. The index's stat info will take precedence
 * over the merged tree's. Returns 0 on success, -1 on failure.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ವಿಲೀನ_ವೃಕ್ಷ(ರಚನೆ ವೃಕ್ಷ *ವೃಕ್ಷ)
{
	ರಚನೆ ಬೀಗ_file ಬೀಗ_file = ಬೀಗ_INIT;
	ರಚನೆ unpack_trees_options opts;
	ರಚನೆ ವೃಕ್ಷ_desc t[1];

	ಆದರೆ (repo_parse_ವೃಕ್ಷ(the_repository, ವೃಕ್ಷ))
		ಹಿಂತಿರುಗಿ -1;

	repo_hold_locked_ಸೂಚ್ಯಂಕ(the_repository, &ಬೀಗ_file, ಬೀಗ_DIE_ON_ERROR);

	memset(&opts, 0, ಗಾತ್ರ(opts));
	opts.ತಲೆ_idx = 1;
	opts.src_ಸೂಚ್ಯಂಕ = the_repository->ಸೂಚ್ಯಂಕ;
	opts.dst_ಸೂಚ್ಯಂಕ = the_repository->ಸೂಚ್ಯಂಕ;
	opts.ವಿಲೀನ = 1;
	opts.fn = oneway_ವಿಲೀನ;
	init_ವೃಕ್ಷ_desc(&t[0], &ವೃಕ್ಷ->object.oid, ವೃಕ್ಷ->ಬಫರ್, ವೃಕ್ಷ->ಗಾತ್ರ_2);

	ಆದರೆ (unpack_trees(1, t, &opts)) {
		rollback_ಬೀಗ_file(&ಬೀಗ_file);
		ಹಿಂತಿರುಗಿ -1;
	}

	ಆದರೆ (write_locked_ಸೂಚ್ಯಂಕ(the_repository->ಸೂಚ್ಯಂಕ, &ಬೀಗ_file, COMMIT_ಬೀಗ))
		die(_("unable to write new index file"));

	ಹಿಂತಿರುಗಿ 0;
}

/**
 * Clean the index without touching entries that are not modified between
 * `head` and `remote`.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ clean_ಸೂಚ್ಯಂಕ(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *ತಲೆ, ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *remote)
{
	ರಚನೆ ವೃಕ್ಷ *ತಲೆ_ವೃಕ್ಷ, *remote_ವೃಕ್ಷ, *ಸೂಚ್ಯಂಕ_ವೃಕ್ಷ;
	ರಚನೆ object_id ಸೂಚ್ಯಂಕ;

	ತಲೆ_ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_ಪರೋಕ್ಷ(the_repository, ತಲೆ);
	ಆದರೆ (!ತಲೆ_ವೃಕ್ಷ)
		ಹಿಂತಿರುಗಿ error(_("Could not parse object '%s'."), oid_ಗಮ್ಯ_hex(ತಲೆ));

	remote_ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_ಪರೋಕ್ಷ(the_repository, remote);
	ಆದರೆ (!remote_ವೃಕ್ಷ)
		ಹಿಂತಿರುಗಿ error(_("Could not parse object '%s'."), oid_ಗಮ್ಯ_hex(remote));

	repo_read_ಸೂಚ್ಯಂಕ_unmerged(the_repository);

	ಆದರೆ (fast_forward_ಗಮ್ಯ(ತಲೆ_ವೃಕ್ಷ, ತಲೆ_ವೃಕ್ಷ, 1))
		ಹಿಂತಿರುಗಿ -1;

	ಆದರೆ (write_ಸೂಚ್ಯಂಕ_as_ವೃಕ್ಷ(&ಸೂಚ್ಯಂಕ, the_repository->ಸೂಚ್ಯಂಕ,
				repo_get_ಸೂಚ್ಯಂಕ_file(the_repository),
				0, ಖಾಲಿ))
		ಹಿಂತಿರುಗಿ -1;

	ಸೂಚ್ಯಂಕ_ವೃಕ್ಷ = repo_parse_ವೃಕ್ಷ_ಪರೋಕ್ಷ(the_repository, &ಸೂಚ್ಯಂಕ);
	ಆದರೆ (!ಸೂಚ್ಯಂಕ_ವೃಕ್ಷ)
		ಹಿಂತಿರುಗಿ error(_("Could not parse object '%s'."), oid_ಗಮ್ಯ_hex(&ಸೂಚ್ಯಂಕ));

	ಆದರೆ (fast_forward_ಗಮ್ಯ(ಸೂಚ್ಯಂಕ_ವೃಕ್ಷ, remote_ವೃಕ್ಷ, 0))
		ಹಿಂತಿರುಗಿ -1;

	ಆದರೆ (ವಿಲೀನ_ವೃಕ್ಷ(remote_ವೃಕ್ಷ))
		ಹಿಂತಿರುಗಿ -1;

	remove_branch_state(the_repository, 0);

	ಹಿಂತಿರುಗಿ 0;
}

/**
 * Resets rerere's merge resolution metadata.
 */
ಸ್ಥಿರ ಶೂನ್ಯ am_rerere_clear(ಶೂನ್ಯ)
{
	ರಚನೆ string_list ವಿಲೀನ_rr = STRING_LIST_INIT_DUP;
	rerere_clear(the_repository, &ವಿಲೀನ_rr);
	string_list_clear(&ವಿಲೀನ_rr, 1);
}

/**
 * Resume the current am session by skipping the current patch.
 */
ಸ್ಥಿರ ಶೂನ್ಯ am_skip(ರಚನೆ am_state *state)
{
	ರಚನೆ object_id ತಲೆ;

	am_rerere_clear();

	ಆದರೆ (repo_get_oid(the_repository, "HEAD", &ತಲೆ))
		oidcpy(&ತಲೆ, the_hash_algo->ಖಾಲಿಗಳು_ವೃಕ್ಷ);

	ಆದರೆ (clean_ಸೂಚ್ಯಂಕ(&ತಲೆ, &ತಲೆ))
		die(_("failed to clean index"));

	ಆದರೆ (state->rebasing) {
		FILE *fp = xfopen(am_ಮಾರ್ಗ(state, "rewritten"), "a");

		ಪ್ರತಿಜ್ಞಾ(!is_null_oid(&state->orig_commit));
		fprintf(fp, "%s ", oid_ಗಮ್ಯ_hex(&state->orig_commit));
		fprintf(fp, "%s\n", oid_ಗಮ್ಯ_hex(&ತಲೆ));
		fclose(fp);
	}

	am_ಮುಂದಿನ(state);
	am_ತುಂಬು(state);
	am_ಸತತ(state, 0);
}

/**
 * Returns true if it is safe to reset HEAD to the ORIG_HEAD, false otherwise.
 *
 * It is not safe to reset HEAD when:
 * 1. git-am previously failed because the index was dirty.
 * 2. HEAD has moved since git-am previously failed.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸುರಕ್ಷಿತ_ಗಮ್ಯ_abort(ಸ್ಥಿರಾಂಕ ರಚನೆ am_state *state)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ರಚನೆ object_id abort_safety, ತಲೆ;

	ಆದರೆ (file_exists(am_ಮಾರ್ಗ(state, "dirtyindex")))
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (read_state_file(&sb, state, "abort-safety", 1) > 0) {
		ಆದರೆ (get_oid_hex(sb.buf, &abort_safety))
			die(_("could not parse %s"), am_ಮಾರ್ಗ(state, "abort-safety"));
	} ಇಲ್ಲದಿದ್ದರೆ
		oidclr(&abort_safety, the_repository->hash_algo);
	strbuf_release(&sb);

	ಆದರೆ (repo_get_oid(the_repository, "HEAD", &ತಲೆ))
		oidclr(&ತಲೆ, the_repository->hash_algo);

	ಆದರೆ (oideq(&ತಲೆ, &abort_safety))
		ಹಿಂತಿರುಗಿ 1;

	warning(_("You seem to have moved HEAD since the last 'am' failure.\n"
		"Not rewinding to ORIG_HEAD"));

	ಹಿಂತಿರುಗಿ 0;
}

/**
 * Aborts the current am session if it is safe to do so.
 */
ಸ್ಥಿರ ಶೂನ್ಯ am_abort(ರಚನೆ am_state *state)
{
	ರಚನೆ object_id curr_ತಲೆ, orig_ತಲೆ;
	ಪೂರ್ಣಾಂಕ has_curr_ತಲೆ, has_orig_ತಲೆ;
	ಅಕ್ಷರ *curr_branch;

	ಆದರೆ (!ಸುರಕ್ಷಿತ_ಗಮ್ಯ_abort(state)) {
		am_destroy(state);
		ಹಿಂತಿರುಗಿ;
	}

	am_rerere_clear();

	curr_branch = refs_resolve_refdup(get_main_ref_store(the_repository),
					  "HEAD", 0, &curr_ತಲೆ, ಖಾಲಿ);
	has_curr_ತಲೆ = curr_branch && !is_null_oid(&curr_ತಲೆ);
	ಆದರೆ (!has_curr_ತಲೆ)
		oidcpy(&curr_ತಲೆ, the_hash_algo->ಖಾಲಿಗಳು_ವೃಕ್ಷ);

	has_orig_ತಲೆ = !repo_get_oid(the_repository, "ORIG_HEAD", &orig_ತಲೆ);
	ಆದರೆ (!has_orig_ತಲೆ)
		oidcpy(&orig_ತಲೆ, the_hash_algo->ಖಾಲಿಗಳು_ವೃಕ್ಷ);

	ಆದರೆ (clean_ಸೂಚ್ಯಂಕ(&curr_ತಲೆ, &orig_ತಲೆ))
		die(_("failed to clean index"));

	ಆದರೆ (has_orig_ತಲೆ)
		refs_update_ref(get_main_ref_store(the_repository),
				"am --abort", "HEAD", &orig_ತಲೆ,
				has_curr_ತಲೆ ? &curr_ತಲೆ : ಖಾಲಿ, 0,
				UPDATE_REFS_DIE_ON_ERR);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (curr_branch)
		refs_delete_ref(get_main_ref_store(the_repository), ಖಾಲಿ,
				curr_branch, ಖಾಲಿ, REF_NO_DEREF);

	ಬಿಡುಗಡೆ(curr_branch);
	am_destroy(state);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_patch(ರಚನೆ am_state *state, ಎಣಿಕೆ resume_type resume_ಬಹುಲಕ)
{
	ರಚನೆ strbuf sb = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *patch_ಮಾರ್ಗ;
	ಪೂರ್ಣಾಂಕ len;

	ಆದರೆ (!is_null_oid(&state->orig_commit)) {
		ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;

		strvec_pushl(&cmd.ವಾದಗಳು, "show", oid_ಗಮ್ಯ_hex(&state->orig_commit),
			     "--", ಖಾಲಿ);
		cmd.git_cmd = 1;
		ಹಿಂತಿರುಗಿ ಸತತ_command(&cmd);
	}

	ಆಯ್ಕೆ (resume_ಬಹುಲಕ) {
	ಸ್ಥಿತಿ RESUME_SHOW_PATCH_RAW:
		patch_ಮಾರ್ಗ = am_ಮಾರ್ಗ(state, msgnum(state));
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ RESUME_SHOW_PATCH_DIFF:
		patch_ಮಾರ್ಗ = am_ಮಾರ್ಗ(state, "patch");
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("invalid mode for --show-current-patch");
	}

	len = strbuf_read_file(&sb, patch_ಮಾರ್ಗ, 0);
	ಆದರೆ (len < 0)
		die_errno(_("failed to read '%s'"), patch_ಮಾರ್ಗ);

	ಸಿದ್ಧತೆ_pager(the_repository);
	write_ಒಳ_ತುಂಬಿದ(1, sb.buf, sb.len);
	strbuf_release(&sb);
	ಹಿಂತಿರುಗಿ 0;
}

/**
 * parse_options() callback that validates and sets opt->value to the
 * PATCH_FORMAT_* enum value corresponding to `arg`.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_opt_patchformat(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಪೂರ್ಣಾಂಕ *opt_ಮೌಲ್ಯ = opt->ಮೌಲ್ಯ;

	ಆದರೆ (unset)
		*opt_ಮೌಲ್ಯ = PATCH_FORMAT_UNKNOWN;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "mbox"))
		*opt_ಮೌಲ್ಯ = PATCH_FORMAT_MBOX;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "stgit"))
		*opt_ಮೌಲ್ಯ = PATCH_FORMAT_STGIT;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "stgit-series"))
		*opt_ಮೌಲ್ಯ = PATCH_FORMAT_STGIT_SERIES;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "hg"))
		*opt_ಮೌಲ್ಯ = PATCH_FORMAT_HG;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "mboxrd"))
		*opt_ಮೌಲ್ಯ = PATCH_FORMAT_MBOXRD;
	/*
	 * Please update $__git_patchformat in git-completion.bash
	 * when you add new options
	 */
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ error(_("invalid value for '%s': '%s'"),
			     "--patch-format", arg);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_opt_show_current_patch(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಪೂರ್ಣಾಂಕ *opt_ಮೌಲ್ಯ = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	ಆದರೆ (!arg)
		*opt_ಮೌಲ್ಯ = opt->defval;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "raw"))
		*opt_ಮೌಲ್ಯ = RESUME_SHOW_PATCH_RAW;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "diff"))
		*opt_ಮೌಲ್ಯ = RESUME_SHOW_PATCH_DIFF;
	/*
	 * Please update $__git_showcurrentpatch in git-completion.bash
	 * when you add new options
	 */
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ error(_("invalid value for '%s': '%s'"),
			     "--show-current-patch", arg);
	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_am(ಪೂರ್ಣಾಂಕ argc,
	   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	   ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ am_state state;
	ಪೂರ್ಣಾಂಕ ದ್ವಿಮಾನ = -1;
	ಪೂರ್ಣಾಂಕ keep_cr = -1;
	ಪೂರ್ಣಾಂಕ patch_format = PATCH_FORMAT_UNKNOWN;
	ಎಣಿಕೆ resume_type resume_ಬಹುಲಕ = RESUME_FALSE;
	ಪೂರ್ಣಾಂಕ ಒಳ_ಪ್ರಗತಿ;
	ಪೂರ್ಣಾಂಕ ret = 0;

	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ usage[] = {
		N_("git am [<options>] [(<mbox> | <Maildir>)...]"),
		N_("git am [<options>] (--continue | --skip | --abort)"),
		ಖಾಲಿ
	};

	ರಚನೆ option options[] = {
		OPT_BOOL('i', "interactive", &state.interactive,
			N_("run interactively")),
		OPT_BOOL('n', "no-verify", &state.no_verify,
			N_("bypass pre-applypatch and applypatch-msg hooks")),
		OPT_HIDDEN_BOOL('b', "binary", &ದ್ವಿಮಾನ,
			N_("historical option -- no-op")),
		OPT_BOOL('3', "3way", &state.threeway,
			N_("allow fall back on 3way merging if needed")),
		OPT__QUIET(&state.quiet, N_("be quiet")),
		OPT_ಗಣ_INT('s', "signoff", &state.signoff,
			N_("add a Signed-off-by trailer to the commit message"),
			SIGNOFF_EXPLICIT),
		OPT_BOOL('u', "utf8", &state.utf8,
			N_("recode into utf8 (default)")),
		OPT_ಗಣ_INT('k', "keep", &state.keep,
			N_("pass -k flag to git-mailinfo"), KEEP_TRUE),
		OPT_ಗಣ_INT(0, "keep-non-patch", &state.keep,
			N_("pass -b flag to git-mailinfo"), KEEP_NON_PATCH),
		OPT_BOOL('m', "message-id", &state.ಸಂದೇಶ_id,
			N_("pass -m flag to git-mailinfo")),
		OPT_ಗಣ_INT(0, "keep-cr", &keep_cr,
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
		OPT_PASSTHRU_ARGV(0, "ignore-space-change", &state.git_apply_opts, ಖಾಲಿ,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU_ARGV(0, "ignore-whitespace", &state.git_apply_opts, ಖಾಲಿ,
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
		OPT_PASSTHRU_ARGV('C', ಖಾಲಿ, &state.git_apply_opts, N_("n"),
			N_("pass it through git-apply"),
			0),
		OPT_PASSTHRU_ARGV('p', ಖಾಲಿ, &state.git_apply_opts, N_("num"),
			N_("pass it through git-apply"),
			0),
		OPT_CALLBACK(0, "patch-format", &patch_format, N_("format"),
			N_("format the patch(es) are in"),
			parse_opt_patchformat),
		OPT_PASSTHRU_ARGV(0, "reject", &state.git_apply_opts, ಖಾಲಿ,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_STRING(0, "resolvemsg", &state.resolvemsg, ಖಾಲಿ,
			N_("override error message when patch failure occurs")),
		OPT_CMDMODE(0, "continue", &resume_ಬಹುಲಕ,
			N_("continue applying patches after resolving a conflict"),
			RESUME_RESOLVED),
		OPT_CMDMODE('r', "resolved", &resume_ಬಹುಲಕ,
			N_("synonyms for --continue"),
			RESUME_RESOLVED),
		OPT_CMDMODE(0, "skip", &resume_ಬಹುಲಕ,
			N_("skip the current patch"),
			RESUME_SKIP),
		OPT_CMDMODE(0, "abort", &resume_ಬಹುಲಕ,
			N_("restore the original branch and abort the patching operation"),
			RESUME_ABORT),
		OPT_CMDMODE(0, "quit", &resume_ಬಹುಲಕ,
			N_("abort the patching operation but keep HEAD where it is"),
			RESUME_QUIT),
		{
			.type = OPTION_CALLBACK,
			.long_name = "show-current-patch",
			.ಮೌಲ್ಯ = &resume_ಬಹುಲಕ,
			.precision = ಗಾತ್ರ(resume_ಬಹುಲಕ),
			.argh = "(diff|raw)",
			.help = N_("show the patch being applied"),
			.flags = PARSE_OPT_CMDMODE | PARSE_OPT_OPTARG | PARSE_OPT_NONEG | PARSE_OPT_LITERAL_ARGHELP,
			.callback = parse_opt_show_current_patch,
			.defval = RESUME_SHOW_PATCH_RAW,
		},
		OPT_CMDMODE(0, "retry", &resume_ಬಹುಲಕ,
			N_("try to apply current patch again"),
			RESUME_APPLY),
		OPT_CMDMODE(0, "allow-empty", &resume_ಬಹುಲಕ,
			N_("record the empty patch as an empty commit"),
			RESUME_ALLOW_ಖಾಲಿಗಳು),
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
		OPT_CALLBACK_F(0, "empty", &state.ಖಾಲಿಗಳು_type, "(stop|drop|keep)",
		  N_("how to handle empty patches"),
		  PARSE_OPT_NONEG, am_option_parse_ಖಾಲಿಗಳು),
		OPT_HIDDEN_BOOL(0, "rebasing", &state.rebasing,
			N_("(internal use for git-rebase)")),
		OPT_ಕೊನೆ()
	};

	show_usage_with_options_if_asked(argc, argv, usage, options);

	repo_config(the_repository, git_default_config, ಖಾಲಿ);

	am_state_init(&state);

	ಒಳ_ಪ್ರಗತಿ = am_ಒಳ_ಪ್ರಗತಿ(&state);
	ಆದರೆ (ಒಳ_ಪ್ರಗತಿ)
		am_ತುಂಬು(&state);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, usage, 0);

	ಆದರೆ (ದ್ವಿಮಾನ >= 0)
		fprintf_ln(stderr, _("The -b/--binary option has been a no-op for long time, and\n"
				"it will be removed. Please do not use it anymore."));

	/* Ensure a valid committer ident can be constructed */
	git_committer_info(IDENT_STRICT);

	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ_preload(the_repository, ಖಾಲಿ, 0) < 0)
		die(_("failed to read the index"));

	ಆದರೆ (ಒಳ_ಪ್ರಗತಿ) {
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
		ಆದರೆ (argc || (resume_ಬಹುಲಕ == RESUME_FALSE && !isatty(0)))
			die(_("previous rebase directory %s still exists but mbox given."),
				state.dir);

		ಆದರೆ (resume_ಬಹುಲಕ == RESUME_FALSE)
			resume_ಬಹುಲಕ = RESUME_APPLY;

		ಆದರೆ (state.signoff == SIGNOFF_EXPLICIT)
			am_append_signoff(&state);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ strvec paths = STRVEC_INIT;
		ಪೂರ್ಣಾಂಕ i;

		/*
		 * Handle stray state directory in the independent-run case. In
		 * the --rebasing case, it is up to the caller to take care of
		 * stray directories.
		 */
		ಆದರೆ (file_exists(state.dir) && !state.rebasing) {
			ಆದರೆ (resume_ಬಹುಲಕ == RESUME_ABORT || resume_ಬಹುಲಕ == RESUME_QUIT) {
				am_destroy(&state);
				am_state_release(&state);
				ಹಿಂತಿರುಗಿ 0;
			}

			die(_("Stray %s directory found.\n"
				"Use \"git am --abort\" to remove it."),
				state.dir);
		}

		ಆದರೆ (resume_ಬಹುಲಕ)
			die(_("Resolve operation not in progress, we are not resuming."));

		ಗಾಗಿ (i = 0; i < argc; i++) {
			ಆದರೆ (is_absolute_ಮಾರ್ಗ(argv[i]) || !ಪೂರ್ವಪ್ರತ್ಯಯ)
				strvec_ತಳ್ಳು(&paths, argv[i]);
			ಇಲ್ಲದಿದ್ದರೆ
				strvec_ತಳ್ಳು(&paths, mkpath("%s/%s", ಪೂರ್ವಪ್ರತ್ಯಯ, argv[i]));
		}

		ಆದರೆ (state.interactive && !paths.nr)
			die(_("interactive mode requires patches on the command line"));

		am_ಸಿದ್ಧತೆ(&state, patch_format, paths.v, keep_cr);

		strvec_clear(&paths);
	}

	ಆಯ್ಕೆ (resume_ಬಹುಲಕ) {
	ಸ್ಥಿತಿ RESUME_FALSE:
		am_ಸತತ(&state, 0);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ RESUME_APPLY:
		am_ಸತತ(&state, 1);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ RESUME_RESOLVED:
	ಸ್ಥಿತಿ RESUME_ALLOW_ಖಾಲಿಗಳು:
		am_resolve(&state, resume_ಬಹುಲಕ == RESUME_ALLOW_ಖಾಲಿಗಳು ? 1 : 0);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ RESUME_SKIP:
		am_skip(&state);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ RESUME_ABORT:
		am_abort(&state);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ RESUME_QUIT:
		am_rerere_clear();
		am_destroy(&state);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ RESUME_SHOW_PATCH_RAW:
	ಸ್ಥಿತಿ RESUME_SHOW_PATCH_DIFF:
		ret = show_patch(&state, resume_ಬಹುಲಕ);
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("invalid resume value");
	}

	am_state_release(&state);

	ಹಿಂತಿರುಗಿ ret;
}
