/*
 * Builtin "git am"
 *
 * Based on git-am.sh by Junio C Hamano.
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "advice.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "editor.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hook.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tempfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lockfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cache-tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unpack-trees.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "branch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "preload-index.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sequencer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "merge-ort-wrappers.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "log-tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "notes-utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "rerere.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mailinfo.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "apply.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pager.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pretty.h"

/**
 * Returns the length of the first line of msg.
 */
ਸਥਿਰ ਪੂਰਨ linelen(ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ)
{
	ਵਾਪਸ strchrnul(ਸੁਨੇਹਾ, '\n') - ਸੁਨੇਹਾ;
}

/**
 * Returns true if `str` consists of only whitespace, false otherwise.
 */
ਸਥਿਰ ਪੂਰਨ str_isspace(ਅਚਲ ਅੱਖਰ *str)
{
	ਲਈ (; *str; str++)
		ਜੇ (!ਖਾਲੀ_ਥਾਂ_ਹੈ(*str))
			ਵਾਪਸ 0;

	ਵਾਪਸ 1;
}

ਗਿਣਤੀ patch_format {
	PATCH_FORMAT_UNKNOWN = 0,
	PATCH_FORMAT_MBOX,
	PATCH_FORMAT_STGIT,
	PATCH_FORMAT_STGIT_SERIES,
	PATCH_FORMAT_HG,
	PATCH_FORMAT_MBOXRD
};

ਗਿਣਤੀ keep_type {
	KEEP_FALSE = 0,
	KEEP_TRUE,      /* pass -k flag to git-mailinfo */
	KEEP_NON_PATCH  /* pass -b flag to git-mailinfo */
};

ਗਿਣਤੀ scissors_type {
	SCISSORS_UNSET = -1,
	SCISSORS_FALSE = 0,  /* pass --no-scissors to git-mailinfo */
	SCISSORS_TRUE        /* pass --scissors to git-mailinfo */
};

ਗਿਣਤੀ signoff_type {
	SIGNOFF_FALSE = 0,
	SIGNOFF_TRUE = 1,
	SIGNOFF_EXPLICIT /* --signoff was set on the command-line */
};

ਗਿਣਤੀ resume_type {
	RESUME_FALSE = 0,
	RESUME_APPLY,
	RESUME_RESOLVED,
	RESUME_SKIP,
	RESUME_ABORT,
	RESUME_QUIT,
	RESUME_SHOW_PATCH_RAW,
	RESUME_SHOW_PATCH_DIFF,
	RESUME_ALLOW_ਖਾਲੀ_ਥਾਵਾਂ,
};

ਗਿਣਤੀ ਖਾਲੀ_ਥਾਵਾਂ_action {
	STOP_ON_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT = 0,  /* output errors and stop in the middle of an am session */
	DROP_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT,         /* skip with a notice message, unless "--quiet" has been passed */
	KEEP_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT,         /* keep recording as empty commits */
};

ਬਣਤਰ am_state {
	/* state directory path */
	ਅੱਖਰ *dir;

	/* current and last patch numbers, 1-indexed */
	ਪੂਰਨ cur;
	ਪੂਰਨ ਆਖਰੀ;

	/* commit metadata and message */
	ਅੱਖਰ *author_name;
	ਅੱਖਰ *author_email;
	ਅੱਖਰ *author_date;
	ਅੱਖਰ *ਸੁਨੇਹਾ;
	ਮਾਪ_t ਸੁਨੇਹਾ_len;

	/* when --rebasing, records the original commit the patch came from */
	ਬਣਤਰ object_id orig_commit;

	/* number of digits in patch filename */
	ਪੂਰਨ prec;

	/* various operating modes and command line options */
	ਪੂਰਨ interactive;
	ਪੂਰਨ no_verify;
	ਪੂਰਨ threeway;
	ਪੂਰਨ quiet;
	ਪੂਰਨ signoff; /* enum signoff_type */
	ਪੂਰਨ utf8;
	ਪੂਰਨ keep; /* enum keep_type */
	ਪੂਰਨ ਸੁਨੇਹਾ_id;
	ਪੂਰਨ scissors; /* enum scissors_type */
	ਪੂਰਨ quoted_cr; /* enum quoted_cr_action */
	ਪੂਰਨ ਖਾਲੀ_ਥਾਵਾਂ_type; /* enum empty_action */
	ਬਣਤਰ strvec git_apply_opts;
	ਅਚਲ ਅੱਖਰ *resolvemsg;
	ਪੂਰਨ committer_date_is_author_date;
	ਪੂਰਨ ignore_date;
	ਪੂਰਨ allow_rerere_autoupdate;
	ਅਚਲ ਅੱਖਰ *sign_commit;
	ਪੂਰਨ rebasing;
};

/**
 * Initializes am_state with the default values.
 */
ਸਥਿਰ ਖਾਲੀ am_state_init(ਬਣਤਰ am_state *state)
{
	ਪੂਰਨ gpgsign;

	memset(state, 0, ਆਕਾਰ(*state));

	state->dir = repo_git_ਰਸਤਾ(the_repository, "rebase-apply");

	state->prec = 4;

	repo_config_get_bool(the_repository, "am.threeway", &state->threeway);

	state->utf8 = 1;

	repo_config_get_bool(the_repository, "am.messageid", &state->ਸੁਨੇਹਾ_id);

	state->scissors = SCISSORS_UNSET;
	state->quoted_cr = quoted_cr_unset;

	strvec_init(&state->git_apply_opts);

	ਜੇ (!repo_config_get_bool(the_repository, "commit.gpgsign", &gpgsign))
		state->sign_commit = gpgsign ? "" : ਨਿਰਾ;
}

/**
 * Releases memory allocated by an am_state.
 */
ਸਥਿਰ ਖਾਲੀ am_state_release(ਬਣਤਰ am_state *state)
{
	ਮੁਕਤ(state->dir);
	ਮੁਕਤ(state->author_name);
	ਮੁਕਤ(state->author_email);
	ਮੁਕਤ(state->author_date);
	ਮੁਕਤ(state->ਸੁਨੇਹਾ);
	strvec_clear(&state->git_apply_opts);
}

ਸਥਿਰ ਪੂਰਨ am_option_parse_quoted_cr(ਅਚਲ ਬਣਤਰ option *opt,
				     ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	BUG_ON_OPT_NEG(unset);

	ਜੇ (mailinfo_parse_quoted_cr_action(arg, opt->ਮੁੱਲ) != 0)
		ਵਾਪਸ error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ am_option_parse_ਖਾਲੀ_ਥਾਵਾਂ(ਅਚਲ ਬਣਤਰ option *opt,
				     ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਪੂਰਨ *opt_ਮੁੱਲ = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);

	ਜੇ (!strcmp(arg, "stop"))
		*opt_ਮੁੱਲ = STOP_ON_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "drop"))
		*opt_ਮੁੱਲ = DROP_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "keep"))
		*opt_ਮੁੱਲ = KEEP_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ error(_("invalid value for '%s': '%s'"), "--empty", arg);

	ਵਾਪਸ 0;
}

/**
 * Returns path relative to the am_state directory.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ ਅੱਖਰ *am_ਰਸਤਾ(ਅਚਲ ਬਣਤਰ am_state *state, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਵਾਪਸ mkpath("%s/%s", state->dir, ਰਸਤਾ);
}

/**
 * For convenience to call write_file()
 */
ਸਥਿਰ ਖਾਲੀ write_state_ਪਾਠ(ਅਚਲ ਬਣਤਰ am_state *state,
			     ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *string)
{
	write_file(am_ਰਸਤਾ(state, name), "%s", string);
}

ਸਥਿਰ ਖਾਲੀ write_state_ਗਿਣਤੀ(ਅਚਲ ਬਣਤਰ am_state *state,
			      ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ ਮੁੱਲ)
{
	write_file(am_ਰਸਤਾ(state, name), "%d", ਮੁੱਲ);
}

ਸਥਿਰ ਖਾਲੀ write_state_bool(ਅਚਲ ਬਣਤਰ am_state *state,
			     ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ ਮੁੱਲ)
{
	write_state_ਪਾਠ(state, name, ਮੁੱਲ ? "t" : "f");
}

/**
 * If state->quiet is false, calls fprintf(fp, fmt, ...), and appends a newline
 * at the end.
 */
__attribute__((format (ਲਿਖੋ, 3, 4)))
ਸਥਿਰ ਖਾਲੀ say(ਅਚਲ ਬਣਤਰ am_state *state, FILE *fp, ਅਚਲ ਅੱਖਰ *fmt, ...)
{
	va_list ap;

	va_ਸ਼ੁਰੂ(ap, fmt);
	ਜੇ (!state->quiet) {
		vfprintf(fp, fmt, ap);
		putc('\n', fp);
	}
	va_ਸਿਰਾ(ap);
}

/**
 * Returns 1 if there is an am session in progress, 0 otherwise.
 */
ਸਥਿਰ ਪੂਰਨ am_ਵਿੱਚ_ਤਰੱਕੀ(ਅਚਲ ਬਣਤਰ am_state *state)
{
	ਬਣਤਰ stat st;

	ਜੇ (lstat(state->dir, &st) < 0 || !S_ISDIR(st.st_ਬਹੁਲਕ))
		ਵਾਪਸ 0;
	ਜੇ (lstat(am_ਰਸਤਾ(state, "last"), &st) || !S_ISREG(st.st_ਬਹੁਲਕ))
		ਵਾਪਸ 0;
	ਜੇ (lstat(am_ਰਸਤਾ(state, "next"), &st) || !S_ISREG(st.st_ਬਹੁਲਕ))
		ਵਾਪਸ 0;
	ਵਾਪਸ 1;
}

/**
 * Reads the contents of `file` in the `state` directory into `sb`. Returns the
 * number of bytes read on success, -1 if the file does not exist. If `trim` is
 * set, trailing whitespace will be removed.
 */
ਸਥਿਰ ਪੂਰਨ read_state_file(ਬਣਤਰ strbuf *sb, ਅਚਲ ਬਣਤਰ am_state *state,
			ਅਚਲ ਅੱਖਰ *file, ਪੂਰਨ trim)
{
	strbuf_reset(sb);

	ਜੇ (strbuf_read_file(sb, am_ਰਸਤਾ(state, file), 0) >= 0) {
		ਜੇ (trim)
			strbuf_trim(sb);

		ਵਾਪਸ sb->len;
	}

	ਜੇ (errno == ENOENT)
		ਵਾਪਸ -1;

	die_errno(_("could not read '%s'"), am_ਰਸਤਾ(state, file));
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
ਸਥਿਰ ਪੂਰਨ read_am_author_script(ਬਣਤਰ am_state *state)
{
	ਅਚਲ ਅੱਖਰ *filename = am_ਰਸਤਾ(state, "author-script");

	ਪ੍ਰਤਿਜ੍ਞਾ(!state->author_name);
	ਪ੍ਰਤਿਜ੍ਞਾ(!state->author_email);
	ਪ੍ਰਤਿਜ੍ਞਾ(!state->author_date);

	ਵਾਪਸ read_author_script(filename, &state->author_name,
				  &state->author_email, &state->author_date, 1);
}

/**
 * Saves state->author_name, state->author_email and state->author_date in the
 * state directory's "author-script" file.
 */
ਸਥਿਰ ਖਾਲੀ write_author_script(ਅਚਲ ਬਣਤਰ am_state *state)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;

	strbuf_addstr(&sb, "GIT_AUTHOR_NAME=");
	sq_quote_buf(&sb, state->author_name);
	strbuf_addch(&sb, '\n');

	strbuf_addstr(&sb, "GIT_AUTHOR_EMAIL=");
	sq_quote_buf(&sb, state->author_email);
	strbuf_addch(&sb, '\n');

	strbuf_addstr(&sb, "GIT_AUTHOR_DATE=");
	sq_quote_buf(&sb, state->author_date);
	strbuf_addch(&sb, '\n');

	write_state_ਪਾਠ(state, "author-script", sb.buf);

	strbuf_release(&sb);
}

/**
 * Reads the commit message from the state directory's "final-commit" file,
 * setting state->msg to its contents and state->msg_len to the length of its
 * contents in bytes.
 *
 * Returns 0 on success, -1 if the file does not exist.
 */
ਸਥਿਰ ਪੂਰਨ read_commit_ਸੁਨੇਹਾ(ਬਣਤਰ am_state *state)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;

	ਪ੍ਰਤਿਜ੍ਞਾ(!state->ਸੁਨੇਹਾ);

	ਜੇ (read_state_file(&sb, state, "final-commit", 0) < 0) {
		strbuf_release(&sb);
		ਵਾਪਸ -1;
	}

	state->ਸੁਨੇਹਾ = strbuf_detach(&sb, &state->ਸੁਨੇਹਾ_len);
	ਵਾਪਸ 0;
}

/**
 * Saves state->msg in the state directory's "final-commit" file.
 */
ਸਥਿਰ ਖਾਲੀ write_commit_ਸੁਨੇਹਾ(ਅਚਲ ਬਣਤਰ am_state *state)
{
	ਅਚਲ ਅੱਖਰ *filename = am_ਰਸਤਾ(state, "final-commit");
	write_file_buf(filename, state->ਸੁਨੇਹਾ, state->ਸੁਨੇਹਾ_len);
}

/**
 * Loads state from disk.
 */
ਸਥਿਰ ਖਾਲੀ am_ਲੱਦੋ(ਬਣਤਰ am_state *state)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;

	ਜੇ (read_state_file(&sb, state, "next", 1) < 0)
		BUG("state file 'next' does not exist");
	state->cur = strtol(sb.buf, ਨਿਰਾ, 10);

	ਜੇ (read_state_file(&sb, state, "last", 1) < 0)
		BUG("state file 'last' does not exist");
	state->ਆਖਰੀ = strtol(sb.buf, ਨਿਰਾ, 10);

	ਜੇ (read_am_author_script(state) < 0)
		die(_("could not parse author script"));

	read_commit_ਸੁਨੇਹਾ(state);

	ਜੇ (read_state_file(&sb, state, "original-commit", 1) < 0)
		oidclr(&state->orig_commit, the_repository->hash_algo);
	ਨਹੀਂਤਾਂ ਜੇ (get_oid_hex(sb.buf, &state->orig_commit) < 0)
		die(_("could not parse %s"), am_ਰਸਤਾ(state, "original-commit"));

	read_state_file(&sb, state, "threeway", 1);
	state->threeway = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "quiet", 1);
	state->quiet = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "sign", 1);
	state->signoff = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "utf8", 1);
	state->utf8 = !strcmp(sb.buf, "t");

	ਜੇ (file_exists(am_ਰਸਤਾ(state, "rerere-autoupdate"))) {
		read_state_file(&sb, state, "rerere-autoupdate", 1);
		state->allow_rerere_autoupdate = strcmp(sb.buf, "t") ?
			RERERE_NOAUTOUPDATE : RERERE_AUTOUPDATE;
	} ਨਹੀਂਤਾਂ {
		state->allow_rerere_autoupdate = 0;
	}

	read_state_file(&sb, state, "keep", 1);
	ਜੇ (!strcmp(sb.buf, "t"))
		state->keep = KEEP_TRUE;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(sb.buf, "b"))
		state->keep = KEEP_NON_PATCH;
	ਨਹੀਂਤਾਂ
		state->keep = KEEP_FALSE;

	read_state_file(&sb, state, "messageid", 1);
	state->ਸੁਨੇਹਾ_id = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "scissors", 1);
	ਜੇ (!strcmp(sb.buf, "t"))
		state->scissors = SCISSORS_TRUE;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(sb.buf, "f"))
		state->scissors = SCISSORS_FALSE;
	ਨਹੀਂਤਾਂ
		state->scissors = SCISSORS_UNSET;

	read_state_file(&sb, state, "quoted-cr", 1);
	ਜੇ (!*sb.buf)
		state->quoted_cr = quoted_cr_unset;
	ਨਹੀਂਤਾਂ ਜੇ (mailinfo_parse_quoted_cr_action(sb.buf, &state->quoted_cr) != 0)
		die(_("could not parse %s"), am_ਰਸਤਾ(state, "quoted-cr"));

	read_state_file(&sb, state, "apply-opt", 1);
	strvec_clear(&state->git_apply_opts);
	ਜੇ (sq_dequote_ਮੰਜ਼ਿਲ_strvec(sb.buf, &state->git_apply_opts) < 0)
		die(_("could not parse %s"), am_ਰਸਤਾ(state, "apply-opt"));

	state->rebasing = !!file_exists(am_ਰਸਤਾ(state, "rebasing"));

	strbuf_release(&sb);
}

/**
 * Removes the am_state directory, forcefully terminating the current am
 * session.
 */
ਸਥਿਰ ਖਾਲੀ am_destroy(ਅਚਲ ਬਣਤਰ am_state *state)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;

	strbuf_addstr(&sb, state->dir);
	remove_dir_recursively(&sb, 0);
	strbuf_release(&sb);
}

/**
 * Runs applypatch-msg hook. Returns its exit code.
 */
ਸਥਿਰ ਪੂਰਨ ਲੜੀਵਾਰ_applypatch_ਸੁਨੇਹਾ_hook(ਬਣਤਰ am_state *state)
{
	ਪੂਰਨ ret = 0;

	ਪ੍ਰਤਿਜ੍ਞਾ(state->ਸੁਨੇਹਾ);

	ਜੇ (!state->no_verify) {
		ਬਣਤਰ ਲੜੀਵਾਰ_hooks_opt opt = ਲੜੀਵਾਰ_HOOKS_OPT_INIT_FORCE_SERIAL;
		strvec_ਧੱਕੋ(&opt.ਦਲੀਲਾਂ, am_ਰਸਤਾ(state, "final-commit"));
		ret = ਲੜੀਵਾਰ_hooks_opt(the_repository, "applypatch-msg", &opt);
	}

	ਜੇ (!ret) {
		FREE_AND_NULL(state->ਸੁਨੇਹਾ);
		ਜੇ (read_commit_ਸੁਨੇਹਾ(state) < 0)
			die(_("'%s' was deleted by the applypatch-msg hook"),
				am_ਰਸਤਾ(state, "final-commit"));
	}

	ਵਾਪਸ ret;
}

/**
 * Runs post-rewrite hook. Returns it exit code.
 */
ਸਥਿਰ ਪੂਰਨ ਲੜੀਵਾਰ_post_rewrite_hook(ਅਚਲ ਬਣਤਰ am_state *state)
{
	ਬਣਤਰ ਲੜੀਵਾਰ_hooks_opt opt = ਲੜੀਵਾਰ_HOOKS_OPT_INIT;

	strvec_ਧੱਕੋ(&opt.ਦਲੀਲਾਂ, "rebase");
	opt.ਰਸਤਾ_ਮੰਜ਼ਿਲ_stdin = am_ਰਸਤਾ(state, "rewritten");

	ਵਾਪਸ ਲੜੀਵਾਰ_hooks_opt(the_repository, "post-rewrite", &opt);
}

/**
 * Reads the state directory's "rewritten" file, and copies notes from the old
 * commits listed in the file to their rewritten commits.
 *
 * Returns 0 on success, -1 on failure.
 */
ਸਥਿਰ ਪੂਰਨ copy_notes_for_rebase(ਅਚਲ ਬਣਤਰ am_state *state)
{
	ਬਣਤਰ notes_rewrite_cfg *c;
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *invalid_ਸਤਰ = _("Malformed input line: '%s'.");
	ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ = "Notes added by 'git rebase'";
	FILE *fp;
	ਪੂਰਨ ret = 0;

	ਪ੍ਰਤਿਜ੍ਞਾ(state->rebasing);

	c = init_copy_notes_for_rewrite("rebase");
	ਜੇ (!c)
		ਵਾਪਸ 0;

	fp = xfopen(am_ਰਸਤਾ(state, "rewritten"), "r");

	ਜਦੋਂ (!strbuf_getline_lf(&sb, fp)) {
		ਬਣਤਰ object_id from_obj, ਮੰਜ਼ਿਲ_obj;
		ਅਚਲ ਅੱਖਰ *p;

		ਜੇ (sb.len != the_hash_algo->hexsz * 2 + 1) {
			ret = error(invalid_ਸਤਰ, sb.buf);
			ਜਾਓ ਅੰਤ;
		}

		ਜੇ (parse_oid_hex(sb.buf, &from_obj, &p)) {
			ret = error(invalid_ਸਤਰ, sb.buf);
			ਜਾਓ ਅੰਤ;
		}

		ਜੇ (*p != ' ') {
			ret = error(invalid_ਸਤਰ, sb.buf);
			ਜਾਓ ਅੰਤ;
		}

		ਜੇ (get_oid_hex(p + 1, &ਮੰਜ਼ਿਲ_obj)) {
			ret = error(invalid_ਸਤਰ, sb.buf);
			ਜਾਓ ਅੰਤ;
		}

		ਜੇ (copy_note_for_rewrite(c, &from_obj, &ਮੰਜ਼ਿਲ_obj))
			ret = error(_("Failed to copy notes from '%s' to '%s'"),
					oid_ਮੰਜ਼ਿਲ_hex(&from_obj), oid_ਮੰਜ਼ਿਲ_hex(&ਮੰਜ਼ਿਲ_obj));
	}

ਅੰਤ:
	ਅੰਤ_copy_notes_for_rewrite(the_repository, c, ਸੁਨੇਹਾ);
	fclose(fp);
	strbuf_release(&sb);
	ਵਾਪਸ ret;
}

/**
 * Determines if the file looks like a piece of RFC2822 mail by grabbing all
 * non-indented lines and checking if they look like they begin with valid
 * header field names.
 *
 * Returns 1 if the file looks like a piece of mail, 0 otherwise.
 */
ਸਥਿਰ ਪੂਰਨ is_mail(FILE *fp)
{
	ਅਚਲ ਅੱਖਰ *ਸਿਰਲੇਖ_regex = "^[!-9;-~]+:";
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	regex_t regex;
	ਪੂਰਨ ret = 1;

	ਜੇ (fseek(fp, 0L, SEEK_ਸੈੱਟ))
		die_errno(_("fseek failed"));

	ਜੇ (regcomp(&regex, ਸਿਰਲੇਖ_regex, REG_NOSUB | REG_EXTENDED))
		die("invalid pattern: %s", ਸਿਰਲੇਖ_regex);

	ਜਦੋਂ (!strbuf_getline(&sb, fp)) {
		ਜੇ (!sb.len)
			ਰੋਕੋ; /* End of header */

		/* Ignore indented folded lines */
		ਜੇ (*sb.buf == '\t' || *sb.buf == ' ')
			ਜਾਰੀ;

		/* It's a header if it matches header_regex */
		ਜੇ (regexec(&regex, sb.buf, 0, ਨਿਰਾ, 0)) {
			ret = 0;
			ਜਾਓ ਪੂਰਾ;
		}
	}

ਪੂਰਾ:
	regfree(&regex);
	strbuf_release(&sb);
	ਵਾਪਸ ret;
}

/**
 * Attempts to detect the patch_format of the patches contained in `paths`,
 * returning the PATCH_FORMAT_* enum value. Returns PATCH_FORMAT_UNKNOWN if
 * detection fails.
 */
ਸਥਿਰ ਪੂਰਨ detect_patch_format(ਅਚਲ ਅੱਖਰ **paths)
{
	ਗਿਣਤੀ patch_format ret = PATCH_FORMAT_UNKNOWN;
	ਬਣਤਰ strbuf l1 = STRBUF_INIT;
	ਬਣਤਰ strbuf l2 = STRBUF_INIT;
	ਬਣਤਰ strbuf l3 = STRBUF_INIT;
	FILE *fp;

	/*
	 * We default to mbox format if input is from stdin and for directories
	 */
	ਜੇ (!*paths || !strcmp(*paths, "-") || is_directory(*paths))
		ਵਾਪਸ PATCH_FORMAT_MBOX;

	/*
	 * Otherwise, check the first few lines of the first patch, starting
	 * from the first non-blank line, to try to detect its format.
	 */

	fp = xfopen(*paths, "r");

	ਜਦੋਂ (!strbuf_getline(&l1, fp)) {
		ਜੇ (l1.len)
			ਰੋਕੋ;
	}

	ਜੇ (starts_with(l1.buf, "From ") || starts_with(l1.buf, "From: ")) {
		ret = PATCH_FORMAT_MBOX;
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (starts_with(l1.buf, "# This series applies on GIT commit")) {
		ret = PATCH_FORMAT_STGIT_SERIES;
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (!strcmp(l1.buf, "# HG changeset patch")) {
		ret = PATCH_FORMAT_HG;
		ਜਾਓ ਪੂਰਾ;
	}

	strbuf_getline(&l2, fp);
	strbuf_getline(&l3, fp);

	/*
	 * If the second line is empty and the third is a From, Author or Date
	 * entry, this is likely an StGit patch.
	 */
	ਜੇ (l1.len && !l2.len &&
		(starts_with(l3.buf, "From:") ||
		 starts_with(l3.buf, "Author:") ||
		 starts_with(l3.buf, "Date:"))) {
		ret = PATCH_FORMAT_STGIT;
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (l1.len && is_mail(fp)) {
		ret = PATCH_FORMAT_MBOX;
		ਜਾਓ ਪੂਰਾ;
	}

ਪੂਰਾ:
	fclose(fp);
	strbuf_release(&l1);
	strbuf_release(&l2);
	strbuf_release(&l3);
	ਵਾਪਸ ret;
}

/**
 * Splits out individual email patches from `paths`, where each path is either
 * a mbox file or a Maildir. Returns 0 on success, -1 on failure.
 */
ਸਥਿਰ ਪੂਰਨ ਵੰਡੋ_mail_mbox(ਬਣਤਰ am_state *state, ਅਚਲ ਅੱਖਰ **paths,
				ਪੂਰਨ keep_cr, ਪੂਰਨ mboxrd)
{
	ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ strbuf ਆਖਰੀ = STRBUF_INIT;
	ਪੂਰਨ ret;

	cp.git_cmd = 1;
	strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "mailsplit");
	strvec_pushf(&cp.ਦਲੀਲਾਂ, "-d%d", state->prec);
	strvec_pushf(&cp.ਦਲੀਲਾਂ, "-o%s", state->dir);
	strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "-b");
	ਜੇ (keep_cr)
		strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "--keep-cr");
	ਜੇ (mboxrd)
		strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "--mboxrd");
	strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "--");
	strvec_pushv(&cp.ਦਲੀਲਾਂ, paths);

	ret = capture_command(&cp, &ਆਖਰੀ, 8);
	ਜੇ (ret)
		ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;

	state->cur = 1;
	state->ਆਖਰੀ = strtol(ਆਖਰੀ.buf, ਨਿਰਾ, 10);

ਬਾਹਰ_ਨਿਕਲੋ:
	strbuf_release(&ਆਖਰੀ);
	ਵਾਪਸ ret ? -1 : 0;
}

/**
 * Callback signature for split_mail_conv(). The foreign patch should be
 * read from `in`, and the converted patch (in RFC2822 mail format) should be
 * written to `out`. Return 0 on success, or -1 on failure.
 */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ (*mail_conv_fn)(FILE *ਬਾਹਰਲਾ, FILE *ਵਿੱਚ, ਪੂਰਨ keep_cr);

/**
 * Calls `fn` for each file in `paths` to convert the foreign patch to the
 * RFC2822 mail format suitable for parsing with git-mailinfo.
 *
 * Returns 0 on success, -1 on failure.
 */
ਸਥਿਰ ਪੂਰਨ ਵੰਡੋ_mail_conv(mail_conv_fn fn, ਬਣਤਰ am_state *state,
			ਅਚਲ ਅੱਖਰ **paths, ਪੂਰਨ keep_cr)
{
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ *stdin_only[] = {"-", ਨਿਰਾ};
	ਪੂਰਨ i;

	ਜੇ (!*paths)
		paths = stdin_only;

	ਲਈ (i = 0; *paths; paths++, i++) {
		FILE *ਵਿੱਚ, *ਬਾਹਰਲਾ;
		ਅਚਲ ਅੱਖਰ *mail;
		ਪੂਰਨ ret;

		ਜੇ (!strcmp(*paths, "-"))
			ਵਿੱਚ = stdin;
		ਨਹੀਂਤਾਂ
			ਵਿੱਚ = fopen(*paths, "r");

		ਜੇ (!ਵਿੱਚ)
			ਵਾਪਸ error_errno(_("could not open '%s' for reading"),
					   *paths);

		mail = mkpath("%s/%0*d", state->dir, state->prec, i + 1);

		ਬਾਹਰਲਾ = fopen(mail, "w");
		ਜੇ (!ਬਾਹਰਲਾ) {
			ਜੇ (ਵਿੱਚ != stdin)
				fclose(ਵਿੱਚ);
			ਵਾਪਸ error_errno(_("could not open '%s' for writing"),
					   mail);
		}

		ret = fn(ਬਾਹਰਲਾ, ਵਿੱਚ, keep_cr);

		fclose(ਬਾਹਰਲਾ);
		ਜੇ (ਵਿੱਚ != stdin)
			fclose(ਵਿੱਚ);

		ਜੇ (ret)
			ਵਾਪਸ error(_("could not parse patch '%s'"), *paths);
	}

	state->cur = 1;
	state->ਆਖਰੀ = i;
	ਵਾਪਸ 0;
}

/**
 * A split_mail_conv() callback that converts an StGit patch to an RFC2822
 * message suitable for parsing with git-mailinfo.
 */
ਸਥਿਰ ਪੂਰਨ stgit_patch_ਮੰਜ਼ਿਲ_mail(FILE *ਬਾਹਰਲਾ, FILE *ਵਿੱਚ, ਪੂਰਨ keep_cr ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਪੂਰਨ subject_printed = 0;

	ਜਦੋਂ (!strbuf_getline_lf(&sb, ਵਿੱਚ)) {
		ਅਚਲ ਅੱਖਰ *str;

		ਜੇ (str_isspace(sb.buf))
			ਜਾਰੀ;
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(sb.buf, "Author:", &str))
			fprintf(ਬਾਹਰਲਾ, "From:%s\n", str);
		ਨਹੀਂਤਾਂ ਜੇ (starts_with(sb.buf, "From") || starts_with(sb.buf, "Date"))
			fprintf(ਬਾਹਰਲਾ, "%s\n", sb.buf);
		ਨਹੀਂਤਾਂ ਜੇ (!subject_printed) {
			fprintf(ਬਾਹਰਲਾ, "Subject: %s\n", sb.buf);
			subject_printed = 1;
		} ਨਹੀਂਤਾਂ {
			fprintf(ਬਾਹਰਲਾ, "\n%s\n", sb.buf);
			ਰੋਕੋ;
		}
	}

	strbuf_reset(&sb);
	ਜਦੋਂ (strbuf_fread(&sb, 8192, ਵਿੱਚ) > 0) {
		fwrite(sb.buf, 1, sb.len, ਬਾਹਰਲਾ);
		strbuf_reset(&sb);
	}

	strbuf_release(&sb);
	ਵਾਪਸ 0;
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
ਸਥਿਰ ਪੂਰਨ ਵੰਡੋ_mail_stgit_series(ਬਣਤਰ am_state *state, ਅਚਲ ਅੱਖਰ **paths,
					ਪੂਰਨ keep_cr)
{
	ਅਚਲ ਅੱਖਰ *series_dir;
	ਅੱਖਰ *series_dir_buf;
	FILE *fp;
	ਬਣਤਰ strvec patches = STRVEC_INIT;
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਪੂਰਨ ret;

	ਜੇ (!paths[0] || paths[1])
		ਵਾਪਸ error(_("Only one StGIT patch series can be applied at once"));

	series_dir_buf = xstrdup(*paths);
	series_dir = dirname(series_dir_buf);

	fp = fopen(*paths, "r");
	ਜੇ (!fp) {
		ਮੁਕਤ(series_dir_buf);
		ਵਾਪਸ error_errno(_("could not open '%s' for reading"), *paths);
	}

	ਜਦੋਂ (!strbuf_getline_lf(&sb, fp)) {
		ਜੇ (*sb.buf == '#')
			ਜਾਰੀ; /* skip comment lines */

		strvec_ਧੱਕੋ(&patches, mkpath("%s/%s", series_dir, sb.buf));
	}

	fclose(fp);
	strbuf_release(&sb);
	ਮੁਕਤ(series_dir_buf);

	ret = ਵੰਡੋ_mail_conv(stgit_patch_ਮੰਜ਼ਿਲ_mail, state, patches.v, keep_cr);

	strvec_clear(&patches);
	ਵਾਪਸ ret;
}

/**
 * A split_patches_conv() callback that converts a mercurial patch to a RFC2822
 * message suitable for parsing with git-mailinfo.
 */
ਸਥਿਰ ਪੂਰਨ hg_patch_ਮੰਜ਼ਿਲ_mail(FILE *ਬਾਹਰਲਾ, FILE *ਵਿੱਚ, ਪੂਰਨ keep_cr ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਪੂਰਨ rc = 0;

	ਜਦੋਂ (!strbuf_getline_lf(&sb, ਵਿੱਚ)) {
		ਅਚਲ ਅੱਖਰ *str;

		ਜੇ (skip_ਅਗੇਤਰ(sb.buf, "# User ", &str))
			fprintf(ਬਾਹਰਲਾ, "From: %s\n", str);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(sb.buf, "# Date ", &str)) {
			timestamp_t timestamp;
			ਲੰਮਾ tz, tz2;
			ਅੱਖਰ *ਸਿਰਾ;

			errno = 0;
			timestamp = parse_timestamp(str, &ਸਿਰਾ, 10);
			ਜੇ (errno) {
				rc = error(_("invalid timestamp"));
				ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
			}

			ਜੇ (!skip_ਅਗੇਤਰ(ਸਿਰਾ, " ", &str)) {
				rc = error(_("invalid Date line"));
				ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
			}

			errno = 0;
			tz = strtol(str, &ਸਿਰਾ, 10);
			ਜੇ (errno) {
				rc = error(_("invalid timezone offset"));
				ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
			}

			ਜੇ (*ਸਿਰਾ) {
				rc = error(_("invalid Date line"));
				ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
			}

			/*
			 * mercurial's timezone is in seconds west of UTC,
			 * however git's timezone is in hours + minutes east of
			 * UTC. Convert it.
			 */
			tz2 = labs(tz) / 3600 * 100 + labs(tz) % 3600 / 60;
			ਜੇ (tz > 0)
				tz2 = -tz2;

			fprintf(ਬਾਹਰਲਾ, "Date: %s\n", show_date(timestamp, tz2, DATE_ਬਹੁਲਕ(RFC2822)));
		} ਨਹੀਂਤਾਂ ਜੇ (starts_with(sb.buf, "# ")) {
			ਜਾਰੀ;
		} ਨਹੀਂਤਾਂ {
			fprintf(ਬਾਹਰਲਾ, "\n%s\n", sb.buf);
			ਰੋਕੋ;
		}
	}

	strbuf_reset(&sb);
	ਜਦੋਂ (strbuf_fread(&sb, 8192, ਵਿੱਚ) > 0) {
		fwrite(sb.buf, 1, sb.len, ਬਾਹਰਲਾ);
		strbuf_reset(&sb);
	}
ਬਾਹਰ_ਨਿਕਲੋ:
	strbuf_release(&sb);
	ਵਾਪਸ rc;
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
ਸਥਿਰ ਪੂਰਨ ਵੰਡੋ_mail(ਬਣਤਰ am_state *state, ਗਿਣਤੀ patch_format patch_format,
			ਅਚਲ ਅੱਖਰ **paths, ਪੂਰਨ keep_cr)
{
	ਜੇ (keep_cr < 0) {
		keep_cr = 0;
		repo_config_get_bool(the_repository, "am.keepcr", &keep_cr);
	}

	ਚੋਣ (patch_format) {
	ਹਾਲਤ PATCH_FORMAT_MBOX:
		ਵਾਪਸ ਵੰਡੋ_mail_mbox(state, paths, keep_cr, 0);
	ਹਾਲਤ PATCH_FORMAT_STGIT:
		ਵਾਪਸ ਵੰਡੋ_mail_conv(stgit_patch_ਮੰਜ਼ਿਲ_mail, state, paths, keep_cr);
	ਹਾਲਤ PATCH_FORMAT_STGIT_SERIES:
		ਵਾਪਸ ਵੰਡੋ_mail_stgit_series(state, paths, keep_cr);
	ਹਾਲਤ PATCH_FORMAT_HG:
		ਵਾਪਸ ਵੰਡੋ_mail_conv(hg_patch_ਮੰਜ਼ਿਲ_mail, state, paths, keep_cr);
	ਹਾਲਤ PATCH_FORMAT_MBOXRD:
		ਵਾਪਸ ਵੰਡੋ_mail_mbox(state, paths, keep_cr, 1);
	ਮੂਲ:
		BUG("invalid patch_format");
	}
	ਵਾਪਸ -1;
}

/**
 * Setup a new am session for applying patches
 */
ਸਥਿਰ ਖਾਲੀ am_ਤਿਆਰੀ(ਬਣਤਰ am_state *state, ਗਿਣਤੀ patch_format patch_format,
			ਅਚਲ ਅੱਖਰ **paths, ਪੂਰਨ keep_cr)
{
	ਬਣਤਰ object_id curr_ਸਿਰ;
	ਅਚਲ ਅੱਖਰ *str;
	ਬਣਤਰ strbuf sb = STRBUF_INIT;

	ਜੇ (!patch_format)
		patch_format = detect_patch_format(paths);

	ਜੇ (!patch_format) {
		fprintf_ln(stderr, _("Patch format detection failed."));
		die(ਨਿਰਾ);
	}

	ਜੇ (mkdir(state->dir, 0777) < 0 && errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), state->dir);
	refs_delete_ref(get_main_ref_store(the_repository), ਨਿਰਾ,
			"REBASE_HEAD", ਨਿਰਾ, REF_NO_DEREF);

	ਜੇ (ਵੰਡੋ_mail(state, patch_format, paths, keep_cr) < 0) {
		am_destroy(state);
		die(_("Failed to split patches."));
	}

	ਜੇ (state->rebasing)
		state->threeway = 1;

	write_state_bool(state, "threeway", state->threeway);
	write_state_bool(state, "quiet", state->quiet);
	write_state_bool(state, "sign", state->signoff);
	write_state_bool(state, "utf8", state->utf8);

	ਜੇ (state->allow_rerere_autoupdate)
		write_state_bool(state, "rerere-autoupdate",
			 state->allow_rerere_autoupdate == RERERE_AUTOUPDATE);

	ਚੋਣ (state->keep) {
	ਹਾਲਤ KEEP_FALSE:
		str = "f";
		ਰੋਕੋ;
	ਹਾਲਤ KEEP_TRUE:
		str = "t";
		ਰੋਕੋ;
	ਹਾਲਤ KEEP_NON_PATCH:
		str = "b";
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("invalid value for state->keep");
	}

	write_state_ਪਾਠ(state, "keep", str);
	write_state_bool(state, "messageid", state->ਸੁਨੇਹਾ_id);

	ਚੋਣ (state->scissors) {
	ਹਾਲਤ SCISSORS_UNSET:
		str = "";
		ਰੋਕੋ;
	ਹਾਲਤ SCISSORS_FALSE:
		str = "f";
		ਰੋਕੋ;
	ਹਾਲਤ SCISSORS_TRUE:
		str = "t";
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("invalid value for state->scissors");
	}
	write_state_ਪਾਠ(state, "scissors", str);

	ਚੋਣ (state->quoted_cr) {
	ਹਾਲਤ quoted_cr_unset:
		str = "";
		ਰੋਕੋ;
	ਹਾਲਤ quoted_cr_nowarn:
		str = "nowarn";
		ਰੋਕੋ;
	ਹਾਲਤ quoted_cr_warn:
		str = "warn";
		ਰੋਕੋ;
	ਹਾਲਤ quoted_cr_strip:
		str = "strip";
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("invalid value for state->quoted_cr");
	}
	write_state_ਪਾਠ(state, "quoted-cr", str);

	sq_quote_argv(&sb, state->git_apply_opts.v);
	write_state_ਪਾਠ(state, "apply-opt", sb.buf);

	ਜੇ (state->rebasing)
		write_state_ਪਾਠ(state, "rebasing", "");
	ਨਹੀਂਤਾਂ
		write_state_ਪਾਠ(state, "applying", "");

	ਜੇ (!repo_get_oid(the_repository, "HEAD", &curr_ਸਿਰ)) {
		write_state_ਪਾਠ(state, "abort-safety", oid_ਮੰਜ਼ਿਲ_hex(&curr_ਸਿਰ));
		ਜੇ (!state->rebasing)
			refs_update_ref(get_main_ref_store(the_repository),
					"am", "ORIG_HEAD", &curr_ਸਿਰ, ਨਿਰਾ,
					0,
					UPDATE_REFS_DIE_ON_ERR);
	} ਨਹੀਂਤਾਂ {
		write_state_ਪਾਠ(state, "abort-safety", "");
		ਜੇ (!state->rebasing)
			refs_delete_ref(get_main_ref_store(the_repository),
					ਨਿਰਾ, "ORIG_HEAD", ਨਿਰਾ, 0);
	}

	/*
	 * NOTE: Since the "next" and "last" files determine if an am_state
	 * session is in progress, they should be written last.
	 */

	write_state_ਗਿਣਤੀ(state, "next", state->cur);
	write_state_ਗਿਣਤੀ(state, "last", state->ਆਖਰੀ);

	strbuf_release(&sb);
}

/**
 * Increments the patch pointer, and cleans am_state for the application of the
 * next patch.
 */
ਸਥਿਰ ਖਾਲੀ am_ਅਗਲਾ(ਬਣਤਰ am_state *state)
{
	ਬਣਤਰ object_id ਸਿਰ;

	FREE_AND_NULL(state->author_name);
	FREE_AND_NULL(state->author_email);
	FREE_AND_NULL(state->author_date);
	FREE_AND_NULL(state->ਸੁਨੇਹਾ);
	state->ਸੁਨੇਹਾ_len = 0;

	unlink(am_ਰਸਤਾ(state, "author-script"));
	unlink(am_ਰਸਤਾ(state, "final-commit"));

	oidclr(&state->orig_commit, the_repository->hash_algo);
	unlink(am_ਰਸਤਾ(state, "original-commit"));
	refs_delete_ref(get_main_ref_store(the_repository), ਨਿਰਾ,
			"REBASE_HEAD", ਨਿਰਾ, REF_NO_DEREF);

	ਜੇ (!repo_get_oid(the_repository, "HEAD", &ਸਿਰ))
		write_state_ਪਾਠ(state, "abort-safety", oid_ਮੰਜ਼ਿਲ_hex(&ਸਿਰ));
	ਨਹੀਂਤਾਂ
		write_state_ਪਾਠ(state, "abort-safety", "");

	state->cur++;
	write_state_ਗਿਣਤੀ(state, "next", state->cur);
}

/**
 * Returns the filename of the current patch email.
 */
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *msgnum(ਅਚਲ ਬਣਤਰ am_state *state)
{
	ਸਥਿਰ ਬਣਤਰ strbuf sb = STRBUF_INIT;

	strbuf_reset(&sb);
	strbuf_addf(&sb, "%0*d", state->prec, state->cur);

	ਵਾਪਸ sb.buf;
}

/**
 * Dies with a user-friendly message on how to proceed after resolving the
 * problem. This message can be overridden with state->resolvemsg.
 */
ਸਥਿਰ ਖਾਲੀ NORETURN die_user_resolve(ਅਚਲ ਬਣਤਰ am_state *state)
{
	ਜੇ (state->resolvemsg) {
		advise_if_enabled(ADVICE_ਮਿਲਾਓ_CONFLICT, "%s", state->resolvemsg);
	} ਨਹੀਂਤਾਂ {
		ਅਚਲ ਅੱਖਰ *cmdline = state->interactive ? "git am -i" : "git am";
		ਬਣਤਰ strbuf sb = STRBUF_INIT;

		strbuf_addf(&sb, _("When you have resolved this problem, run \"%s --continue\".\n"), cmdline);
		strbuf_addf(&sb, _("If you prefer to skip this patch, run \"%s --skip\" instead.\n"), cmdline);

		ਜੇ (advice_enabled(ADVICE_AM_WORK_DIR) &&
		    is_ਖਾਲੀ_ਥਾਵਾਂ_or_missing_file(am_ਰਸਤਾ(state, "patch")) &&
		    !repo_ਸੂਚਕਾਂਕ_has_changes(the_repository, ਨਿਰਾ, ਨਿਰਾ))
			strbuf_addf(&sb, _("To record the empty patch as an empty commit, run \"%s --allow-empty\".\n"), cmdline);

		strbuf_addf(&sb, _("To restore the original branch and stop patching, run \"%s --abort\"."), cmdline);

		advise_if_enabled(ADVICE_ਮਿਲਾਓ_CONFLICT, "%s", sb.buf);
		strbuf_release(&sb);
	}

	die(ਨਿਰਾ);
}

/**
 * Appends signoff to the "msg" field of the am_state.
 */
ਸਥਿਰ ਖਾਲੀ am_append_signoff(ਬਣਤਰ am_state *state)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;

	strbuf_attach(&sb, state->ਸੁਨੇਹਾ, state->ਸੁਨੇਹਾ_len, state->ਸੁਨੇਹਾ_len + 1);
	append_signoff(&sb, 0, 0);
	state->ਸੁਨੇਹਾ = strbuf_detach(&sb, &state->ਸੁਨੇਹਾ_len);
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
ਸਥਿਰ ਪੂਰਨ parse_mail(ਬਣਤਰ am_state *state, ਅਚਲ ਅੱਖਰ *mail)
{
	FILE *fp;
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਬਣਤਰ strbuf ਸੁਨੇਹਾ = STRBUF_INIT;
	ਬਣਤਰ strbuf author_name = STRBUF_INIT;
	ਬਣਤਰ strbuf author_date = STRBUF_INIT;
	ਬਣਤਰ strbuf author_email = STRBUF_INIT;
	ਪੂਰਨ ret = 0;
	ਬਣਤਰ mailinfo mi;

	ਤਿਆਰੀ_mailinfo(the_repository, &mi);

	ਜੇ (state->utf8)
		mi.metainfo_charset = get_commit_output_encoding();
	ਨਹੀਂਤਾਂ
		mi.metainfo_charset = ਨਿਰਾ;

	ਚੋਣ (state->keep) {
	ਹਾਲਤ KEEP_FALSE:
		ਰੋਕੋ;
	ਹਾਲਤ KEEP_TRUE:
		mi.keep_subject = 1;
		ਰੋਕੋ;
	ਹਾਲਤ KEEP_NON_PATCH:
		mi.keep_non_patch_brackets_ਵਿੱਚ_subject = 1;
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("invalid value for state->keep");
	}

	ਜੇ (state->ਸੁਨੇਹਾ_id)
		mi.ਜੋੜੋ_ਸੁਨੇਹਾ_id = 1;

	ਚੋਣ (state->scissors) {
	ਹਾਲਤ SCISSORS_UNSET:
		ਰੋਕੋ;
	ਹਾਲਤ SCISSORS_FALSE:
		mi.use_scissors = 0;
		ਰੋਕੋ;
	ਹਾਲਤ SCISSORS_TRUE:
		mi.use_scissors = 1;
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("invalid value for state->scissors");
	}

	ਚੋਣ (state->quoted_cr) {
	ਹਾਲਤ quoted_cr_unset:
		ਰੋਕੋ;
	ਹਾਲਤ quoted_cr_nowarn:
	ਹਾਲਤ quoted_cr_warn:
	ਹਾਲਤ quoted_cr_strip:
		mi.quoted_cr = state->quoted_cr;
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("invalid value for state->quoted_cr");
	}

	mi.ਨਿਵੇਸ਼ = xfopen(mail, "r");
	mi.output = xfopen(am_ਰਸਤਾ(state, "info"), "w");
	ਜੇ (mailinfo(&mi, am_ਰਸਤਾ(state, "msg"), am_ਰਸਤਾ(state, "patch")))
		die("could not parse patch");

	fclose(mi.ਨਿਵੇਸ਼);
	fclose(mi.output);

	ਜੇ (mi.format_flowed)
		warning(_("Patch sent with format=flowed; "
			  "space at the end of lines might be lost."));

	/* Extract message and author information */
	fp = xfopen(am_ਰਸਤਾ(state, "info"), "r");
	ਜਦੋਂ (!strbuf_getline_lf(&sb, fp)) {
		ਅਚਲ ਅੱਖਰ *x;

		ਜੇ (skip_ਅਗੇਤਰ(sb.buf, "Subject: ", &x)) {
			ਜੇ (ਸੁਨੇਹਾ.len)
				strbuf_addch(&ਸੁਨੇਹਾ, '\n');
			strbuf_addstr(&ਸੁਨੇਹਾ, x);
		} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(sb.buf, "Author: ", &x))
			strbuf_addstr(&author_name, x);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(sb.buf, "Email: ", &x))
			strbuf_addstr(&author_email, x);
		ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(sb.buf, "Date: ", &x))
			strbuf_addstr(&author_date, x);
	}
	fclose(fp);

	/* Skip pine's internal folder data */
	ਜੇ (!strcmp(author_name.buf, "Mail System Internal Data")) {
		ret = 1;
		ਜਾਓ ਅੰਤ;
	}

	strbuf_addstr(&ਸੁਨੇਹਾ, "\n\n");
	strbuf_addbuf(&ਸੁਨੇਹਾ, &mi.log_ਸੁਨੇਹਾ);
	strbuf_stripspace(&ਸੁਨੇਹਾ, ਨਿਰਾ);

	ਪ੍ਰਤਿਜ੍ਞਾ(!state->author_name);
	state->author_name = strbuf_detach(&author_name, ਨਿਰਾ);

	ਪ੍ਰਤਿਜ੍ਞਾ(!state->author_email);
	state->author_email = strbuf_detach(&author_email, ਨਿਰਾ);

	ਪ੍ਰਤਿਜ੍ਞਾ(!state->author_date);
	state->author_date = strbuf_detach(&author_date, ਨਿਰਾ);

	ਪ੍ਰਤਿਜ੍ਞਾ(!state->ਸੁਨੇਹਾ);
	state->ਸੁਨੇਹਾ = strbuf_detach(&ਸੁਨੇਹਾ, &state->ਸੁਨੇਹਾ_len);

ਅੰਤ:
	strbuf_release(&ਸੁਨੇਹਾ);
	strbuf_release(&author_date);
	strbuf_release(&author_email);
	strbuf_release(&author_name);
	strbuf_release(&sb);
	clear_mailinfo(&mi);
	ਵਾਪਸ ret;
}

/**
 * Sets commit_id to the commit hash where the mail was generated from.
 * Returns 0 on success, -1 on failure.
 */
ਸਥਿਰ ਪੂਰਨ get_mail_commit_oid(ਬਣਤਰ object_id *commit_id, ਅਚਲ ਅੱਖਰ *mail)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	FILE *fp = xfopen(mail, "r");
	ਅਚਲ ਅੱਖਰ *x;
	ਪੂਰਨ ret = 0;

	ਜੇ (strbuf_getline_lf(&sb, fp) ||
	    !skip_ਅਗੇਤਰ(sb.buf, "From ", &x) ||
	    get_oid_hex(x, commit_id) < 0)
		ret = -1;

	strbuf_release(&sb);
	fclose(fp);
	ਵਾਪਸ ret;
}

/**
 * Sets state->msg, state->author_name, state->author_email, state->author_date
 * to the commit's respective info.
 */
ਸਥਿਰ ਖਾਲੀ get_commit_info(ਬਣਤਰ am_state *state, ਬਣਤਰ commit *commit)
{
	ਅਚਲ ਅੱਖਰ *ਬਫਰ, *ident_ਸਤਰ, *ਸੁਨੇਹਾ;
	ਮਾਪ_t ident_len;
	ਬਣਤਰ ident_ਵੰਡੋ id;

	ਬਫਰ = repo_logmsg_reencode(the_repository, commit, ਨਿਰਾ,
				      get_commit_output_encoding());

	ident_ਸਤਰ = ਲੱਭੋ_commit_ਸਿਰਲੇਖ(ਬਫਰ, "author", &ident_len);
	ਜੇ (!ident_ਸਤਰ)
		die(_("missing author line in commit %s"),
		      oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid));
	ਜੇ (ਵੰਡੋ_ident_ਸਤਰ(&id, ident_ਸਤਰ, ident_len) < 0)
		die(_("invalid ident line: %.*s"), (ਪੂਰਨ)ident_len, ident_ਸਤਰ);

	ਪ੍ਰਤਿਜ੍ਞਾ(!state->author_name);
	ਜੇ (id.name_begin)
		state->author_name =
			xmemdupz(id.name_begin, id.name_ਸਿਰਾ - id.name_begin);
	ਨਹੀਂਤਾਂ
		state->author_name = xstrdup("");

	ਪ੍ਰਤਿਜ੍ਞਾ(!state->author_email);
	ਜੇ (id.mail_begin)
		state->author_email =
			xmemdupz(id.mail_begin, id.mail_ਸਿਰਾ - id.mail_begin);
	ਨਹੀਂਤਾਂ
		state->author_email = xstrdup("");

	ਪ੍ਰਤਿਜ੍ਞਾ(!state->author_date);
	state->author_date = xstrdup(show_ident_date(&id, DATE_ਬਹੁਲਕ(ਸਾਧਾਰਨ)));

	ਪ੍ਰਤਿਜ੍ਞਾ(!state->ਸੁਨੇਹਾ);
	ਸੁਨੇਹਾ = ਲੜੀ_ਖੋਜ(ਬਫਰ, "\n\n");
	ਜੇ (!ਸੁਨੇਹਾ)
		die(_("unable to parse commit %s"), oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid));
	state->ਸੁਨੇਹਾ = xstrdup(ਸੁਨੇਹਾ + 2);
	state->ਸੁਨੇਹਾ_len = ਲੜੀ_ਲੰਬਾਈ(state->ਸੁਨੇਹਾ);
	repo_unuse_commit_ਬਫਰ(the_repository, commit, ਬਫਰ);
}

/**
 * Writes `commit` as a patch to the state directory's "patch" file.
 */
ਸਥਿਰ ਖਾਲੀ write_commit_patch(ਅਚਲ ਬਣਤਰ am_state *state, ਬਣਤਰ commit *commit)
{
	ਬਣਤਰ rev_info rev_info;
	FILE *fp;

	fp = xfopen(am_ਰਸਤਾ(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, ਨਿਰਾ);
	rev_info.diff = 1;
	rev_info.abbrev = 0;
	rev_info.disable_stdin = 1;
	rev_info.show_ਜੜ੍ਹ_diff = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.flags.ਦੁਆਧਾਰੀ = 1;
	rev_info.diffopt.flags.ਭਰਿਆ_ਸੂਚਕਾਂਕ = 1;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	ਜੋੜੋ_pending_object(&rev_info, &commit->object, "");
	diff_ਤਿਆਰੀ_ਪੂਰਾ(&rev_info.diffopt);
	log_ਰੁੱਖ_commit(&rev_info, commit);
	release_revisions(&rev_info);
}

/**
 * Writes the diff of the index against HEAD as a patch to the state
 * directory's "patch" file.
 */
ਸਥਿਰ ਖਾਲੀ write_ਸੂਚਕਾਂਕ_patch(ਅਚਲ ਬਣਤਰ am_state *state)
{
	ਬਣਤਰ ਰੁੱਖ *ਰੁੱਖ;
	ਬਣਤਰ object_id ਸਿਰ;
	ਬਣਤਰ rev_info rev_info;
	FILE *fp;

	ਜੇ (!repo_get_oid(the_repository, "HEAD", &ਸਿਰ)) {
		ਬਣਤਰ commit *commit = lookup_commit_or_die(&ਸਿਰ, "HEAD");
		ਰੁੱਖ = repo_get_commit_ਰੁੱਖ(the_repository, commit);
	} ਨਹੀਂਤਾਂ
		ਰੁੱਖ = lookup_ਰੁੱਖ(the_repository,
				   the_repository->hash_algo->ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ);

	fp = xfopen(am_ਰਸਤਾ(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, ਨਿਰਾ);
	rev_info.diff = 1;
	rev_info.disable_stdin = 1;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	ਜੋੜੋ_pending_object(&rev_info, &ਰੁੱਖ->object, "");
	diff_ਤਿਆਰੀ_ਪੂਰਾ(&rev_info.diffopt);
	ਲੜੀਵਾਰ_diff_ਸੂਚਕਾਂਕ(&rev_info, DIFF_ਸੂਚਕਾਂਕ_CACHED);
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
ਸਥਿਰ ਪੂਰਨ parse_mail_rebase(ਬਣਤਰ am_state *state, ਅਚਲ ਅੱਖਰ *mail)
{
	ਬਣਤਰ commit *commit;
	ਬਣਤਰ object_id commit_oid;

	ਜੇ (get_mail_commit_oid(&commit_oid, mail) < 0)
		die(_("could not parse %s"), mail);

	commit = lookup_commit_or_die(&commit_oid, mail);

	get_commit_info(state, commit);

	write_commit_patch(state, commit);

	oidcpy(&state->orig_commit, &commit_oid);
	write_state_ਪਾਠ(state, "original-commit", oid_ਮੰਜ਼ਿਲ_hex(&commit_oid));
	refs_update_ref(get_main_ref_store(the_repository), "am",
			"REBASE_HEAD", &commit_oid,
			ਨਿਰਾ, REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);

	ਵਾਪਸ 0;
}

/**
 * Applies current patch with git-apply. Returns 0 on success, -1 otherwise. If
 * `index_file` is not NULL, the patch will be applied to that index.
 */
ਸਥਿਰ ਪੂਰਨ ਲੜੀਵਾਰ_apply(ਅਚਲ ਬਣਤਰ am_state *state, ਅਚਲ ਅੱਖਰ *ਸੂਚਕਾਂਕ_file)
{
	ਬਣਤਰ strvec apply_paths = STRVEC_INIT;
	ਬਣਤਰ strvec apply_opts = STRVEC_INIT;
	ਬਣਤਰ apply_state apply_state;
	ਪੂਰਨ res, opts_ਖੱਬਾ;
	ਪੂਰਨ force_apply = 0;
	ਪੂਰਨ options = 0;
	ਅਚਲ ਅੱਖਰ **apply_argv;

	ਜੇ (init_apply_state(&apply_state, the_repository, ਨਿਰਾ))
		BUG("init_apply_state() failed");

	strvec_ਧੱਕੋ(&apply_opts, "apply");
	strvec_pushv(&apply_opts, state->git_apply_opts.v);

	/*
	 * Build a copy that apply_parse_options() can rearrange.
	 * apply_opts.v keeps referencing the allocated strings for
	 * strvec_clear() to release.
	 */
	DUP_ARRAY(apply_argv, apply_opts.v, apply_opts.nr);

	opts_ਖੱਬਾ = apply_parse_options(apply_opts.nr, apply_argv,
					&apply_state, &force_apply, &options,
					ਨਿਰਾ);

	ਜੇ (opts_ਖੱਬਾ != 0)
		die("unknown option passed through to git apply");

	ਜੇ (ਸੂਚਕਾਂਕ_file) {
		apply_state.ਸੂਚਕਾਂਕ_file = ਸੂਚਕਾਂਕ_file;
		apply_state.cached = 1;
	} ਨਹੀਂਤਾਂ
		apply_state.check_ਸੂਚਕਾਂਕ = 1;

	/*
	 * If we are allowed to fall back on 3-way merge, don't give false
	 * errors during the initial attempt.
	 */
	ਜੇ (state->threeway && !ਸੂਚਕਾਂਕ_file)
		apply_state.apply_verbosity = verbosity_silent;

	ਜੇ (check_apply_state(&apply_state, force_apply))
		BUG("check_apply_state() failed");

	strvec_ਧੱਕੋ(&apply_paths, am_ਰਸਤਾ(state, "patch"));

	res = apply_all_patches(&apply_state, apply_paths.nr, apply_paths.v, options);

	strvec_clear(&apply_paths);
	strvec_clear(&apply_opts);
	clear_apply_state(&apply_state);
	ਮੁਕਤ(apply_argv);

	ਜੇ (res)
		ਵਾਪਸ res;

	ਜੇ (ਸੂਚਕਾਂਕ_file) {
		/* Reload index as apply_all_patches() will have modified it. */
		discard_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);
		read_ਸੂਚਕਾਂਕ_from(the_repository->ਸੂਚਕਾਂਕ, ਸੂਚਕਾਂਕ_file,
				repo_get_git_dir(the_repository));
	}

	ਵਾਪਸ 0;
}

/**
 * Builds an index that contains just the blobs needed for a 3way merge.
 */
ਸਥਿਰ ਪੂਰਨ build_fake_ancestor(ਅਚਲ ਬਣਤਰ am_state *state, ਅਚਲ ਅੱਖਰ *ਸੂਚਕਾਂਕ_file)
{
	ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;

	cp.git_cmd = 1;
	strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, "apply");
	strvec_pushv(&cp.ਦਲੀਲਾਂ, state->git_apply_opts.v);
	strvec_pushf(&cp.ਦਲੀਲਾਂ, "--build-fake-ancestor=%s", ਸੂਚਕਾਂਕ_file);
	strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, am_ਰਸਤਾ(state, "patch"));

	ਜੇ (ਲੜੀਵਾਰ_command(&cp))
		ਵਾਪਸ -1;

	ਵਾਪਸ 0;
}

/**
 * Attempt a threeway merge, using index_path as the temporary index.
 */
ਸਥਿਰ ਪੂਰਨ fall_ਪਿੱਠ_threeway(ਅਚਲ ਬਣਤਰ am_state *state, ਅਚਲ ਅੱਖਰ *ਸੂਚਕਾਂਕ_ਰਸਤਾ)
{
	ਬਣਤਰ object_id their_ਰੁੱਖ, our_ਰੁੱਖ;
	ਬਣਤਰ object_id bases[1] = { 0 };
	ਬਣਤਰ ਮਿਲਾਓ_options o;
	ਬਣਤਰ commit *ਨਤੀਜਾ;
	ਅੱਖਰ *their_ਰੁੱਖ_name;

	ਜੇ (repo_get_oid(the_repository, "HEAD", &our_ਰੁੱਖ) < 0)
		oidcpy(&our_ਰੁੱਖ, the_hash_algo->ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ);

	ਜੇ (build_fake_ancestor(state, ਸੂਚਕਾਂਕ_ਰਸਤਾ))
		ਵਾਪਸ error("could not build fake ancestor");

	discard_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);
	read_ਸੂਚਕਾਂਕ_from(the_repository->ਸੂਚਕਾਂਕ, ਸੂਚਕਾਂਕ_ਰਸਤਾ, repo_get_git_dir(the_repository));

	ਜੇ (write_ਸੂਚਕਾਂਕ_as_ਰੁੱਖ(&bases[0], the_repository->ਸੂਚਕਾਂਕ, ਸੂਚਕਾਂਕ_ਰਸਤਾ, 0, ਨਿਰਾ))
		ਵਾਪਸ error(_("Repository lacks necessary blobs to fall back on 3-way merge."));

	say(state, stdout, _("Using index info to reconstruct a base tree..."));

	ਜੇ (!state->quiet) {
		/*
		 * List paths that needed 3-way fallback, so that the user can
		 * review them with extra care to spot mismerges.
		 */
		ਬਣਤਰ rev_info rev_info;

		repo_init_revisions(the_repository, &rev_info, ਨਿਰਾ);
		rev_info.diffopt.output_format = DIFF_FORMAT_NAME_ਸਥਿਤੀ;
		rev_info.diffopt.filter |= diff_filter_ਦੋਆਧਾਰੀ_ਅੰਕ('A');
		rev_info.diffopt.filter |= diff_filter_ਦੋਆਧਾਰੀ_ਅੰਕ('M');
		ਜੋੜੋ_pending_oid(&rev_info, "HEAD", &our_ਰੁੱਖ, 0);
		diff_ਤਿਆਰੀ_ਪੂਰਾ(&rev_info.diffopt);
		ਲੜੀਵਾਰ_diff_ਸੂਚਕਾਂਕ(&rev_info, DIFF_ਸੂਚਕਾਂਕ_CACHED);
		release_revisions(&rev_info);
	}

	ਜੇ (ਲੜੀਵਾਰ_apply(state, ਸੂਚਕਾਂਕ_ਰਸਤਾ))
		ਵਾਪਸ error(_("Did you hand edit your patch?\n"
				"It does not apply to blobs recorded in its index."));

	ਜੇ (write_ਸੂਚਕਾਂਕ_as_ਰੁੱਖ(&their_ਰੁੱਖ, the_repository->ਸੂਚਕਾਂਕ, ਸੂਚਕਾਂਕ_ਰਸਤਾ, 0, ਨਿਰਾ))
		ਵਾਪਸ error("could not write tree");

	say(state, stdout, _("Falling back to patching base and 3-way merge..."));

	discard_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ);
	repo_read_ਸੂਚਕਾਂਕ(the_repository);

	/*
	 * This is not so wrong. Depending on which base we picked, orig_tree
	 * may be wildly different from ours, but their_tree has the same set of
	 * wildly different changes in parts the patch did not touch, so
	 * recursive ends up canceling them, saying that we reverted all those
	 * changes.
	 */

	init_ui_ਮਿਲਾਓ_options(&o, the_repository);

	o.branch1 = "HEAD";
	their_ਰੁੱਖ_name = xstrfmt("%.*s", linelen(state->ਸੁਨੇਹਾ), state->ਸੁਨੇਹਾ);
	o.branch2 = their_ਰੁੱਖ_name;
	o.ancestor = "constructed fake ancestor";
	o.detect_directory_renames = ਮਿਲਾਓ_DIRECTORY_RENAMES_NONE;

	ਜੇ (state->quiet)
		o.verbosity = 0;

	ਜੇ (ਮਿਲਾਓ_ort_generic(&o, &our_ਰੁੱਖ, &their_ਰੁੱਖ, 1, bases, &ਨਤੀਜਾ)) {
		repo_rerere(the_repository, state->allow_rerere_autoupdate);
		ਮੁਕਤ(their_ਰੁੱਖ_name);
		ਵਾਪਸ error(_("Failed to merge in the changes."));
	}

	ਮੁਕਤ(their_ਰੁੱਖ_name);
	ਵਾਪਸ 0;
}

/**
 * Commits the current index with state->msg as the commit message and
 * state->author_name, state->author_email and state->author_date as the author
 * information.
 */
ਸਥਿਰ ਖਾਲੀ do_commit(ਅਚਲ ਬਣਤਰ am_state *state)
{
	ਬਣਤਰ object_id ਰੁੱਖ, ਮਾਪਾ, commit;
	ਅਚਲ ਬਣਤਰ object_id *old_oid;
	ਬਣਤਰ commit_list *parents = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *reflog_ਸੁਨੇਹਾ, *author, *committer = ਨਿਰਾ;
	ਬਣਤਰ strbuf sb = STRBUF_INIT;

	ਜੇ (!state->no_verify && ਲੜੀਵਾਰ_hooks(the_repository, "pre-applypatch"))
		ਬਾਹਰ_ਨਿਕਲੋ(1);

	ਜੇ (write_ਸੂਚਕਾਂਕ_as_ਰੁੱਖ(&ਰੁੱਖ, the_repository->ਸੂਚਕਾਂਕ,
				repo_get_ਸੂਚਕਾਂਕ_file(the_repository),
				0, ਨਿਰਾ))
		die(_("git write-tree failed to write a tree"));

	ਜੇ (!repo_get_oid_commit(the_repository, "HEAD", &ਮਾਪਾ)) {
		old_oid = &ਮਾਪਾ;
		commit_list_ਪਾਓ(lookup_commit(the_repository, &ਮਾਪਾ),
				   &parents);
	} ਨਹੀਂਤਾਂ {
		old_oid = ਨਿਰਾ;
		say(state, stderr, _("applying to an empty history"));
	}

	author = fmt_ident(state->author_name, state->author_email,
		WANT_AUTHOR_IDENT,
			state->ignore_date ? ਨਿਰਾ : state->author_date,
			IDENT_STRICT);

	ਜੇ (state->committer_date_is_author_date)
		committer = fmt_ident(getenv("GIT_COMMITTER_NAME"),
				      getenv("GIT_COMMITTER_EMAIL"),
				      WANT_COMMITTER_IDENT,
				      state->ignore_date ? ਨਿਰਾ
							 : state->author_date,
				      IDENT_STRICT);

	ਜੇ (commit_ਰੁੱਖ_extended(state->ਸੁਨੇਹਾ, state->ਸੁਨੇਹਾ_len, &ਰੁੱਖ, parents,
				 &commit, author, committer, state->sign_commit,
				 ਨਿਰਾ))
		die(_("failed to write commit object"));

	reflog_ਸੁਨੇਹਾ = getenv("GIT_REFLOG_ACTION");
	ਜੇ (!reflog_ਸੁਨੇਹਾ)
		reflog_ਸੁਨੇਹਾ = "am";

	strbuf_addf(&sb, "%s: %.*s", reflog_ਸੁਨੇਹਾ, linelen(state->ਸੁਨੇਹਾ),
			state->ਸੁਨੇਹਾ);

	refs_update_ref(get_main_ref_store(the_repository), sb.buf, "HEAD",
			&commit, old_oid, 0,
			UPDATE_REFS_DIE_ON_ERR);

	ਜੇ (state->rebasing) {
		FILE *fp = xfopen(am_ਰਸਤਾ(state, "rewritten"), "a");

		ਪ੍ਰਤਿਜ੍ਞਾ(!is_null_oid(&state->orig_commit));
		fprintf(fp, "%s ", oid_ਮੰਜ਼ਿਲ_hex(&state->orig_commit));
		fprintf(fp, "%s\n", oid_ਮੰਜ਼ਿਲ_hex(&commit));
		fclose(fp);
	}

	ਲੜੀਵਾਰ_hooks(the_repository, "post-applypatch");

	commit_list_free(parents);
	strbuf_release(&sb);
}

/**
 * Validates the am_state for resuming -- the "msg" and authorship fields must
 * be filled up.
 */
ਸਥਿਰ ਖਾਲੀ validate_resume_state(ਅਚਲ ਬਣਤਰ am_state *state)
{
	ਜੇ (!state->ਸੁਨੇਹਾ)
		die(_("cannot resume: %s does not exist."),
			am_ਰਸਤਾ(state, "final-commit"));

	ਜੇ (!state->author_name || !state->author_email || !state->author_date)
		die(_("cannot resume: %s does not exist."),
			am_ਰਸਤਾ(state, "author-script"));
}

/**
 * Interactively prompt the user on whether the current patch should be
 * applied.
 *
 * Returns 0 if the user chooses to apply the patch, 1 if the user chooses to
 * skip it.
 */
ਸਥਿਰ ਪੂਰਨ do_interactive(ਬਣਤਰ am_state *state)
{
	ਪ੍ਰਤਿਜ੍ਞਾ(state->ਸੁਨੇਹਾ);

	ਲਈ (;;) {
		ਅੱਖਰ ਜਵਾਬ[64];

		puts(_("Commit Body is:"));
		puts("--------------------------");
		ਲਿਖੋ("%s", state->ਸੁਨੇਹਾ);
		puts("--------------------------");

		/*
		 * TRANSLATORS: Make sure to include [y], [n], [e], [v] and [a]
		 * in your translation. The program will only accept English
		 * input at this point.
		 */
		ਲਿਖੋ(_("Apply? [y]es/[n]o/[e]dit/[v]iew patch/[a]ccept all: "));
		ਜੇ (!fgets(ਜਵਾਬ, ਆਕਾਰ(ਜਵਾਬ), stdin))
			die("unable to read from stdin; aborting");

		ਜੇ (*ਜਵਾਬ == 'y' || *ਜਵਾਬ == 'Y') {
			ਵਾਪਸ 0;
		} ਨਹੀਂਤਾਂ ਜੇ (*ਜਵਾਬ == 'a' || *ਜਵਾਬ == 'A') {
			state->interactive = 0;
			ਵਾਪਸ 0;
		} ਨਹੀਂਤਾਂ ਜੇ (*ਜਵਾਬ == 'n' || *ਜਵਾਬ == 'N') {
			ਵਾਪਸ 1;
		} ਨਹੀਂਤਾਂ ਜੇ (*ਜਵਾਬ == 'e' || *ਜਵਾਬ == 'E') {
			ਬਣਤਰ strbuf ਸੁਨੇਹਾ = STRBUF_INIT;

			ਜੇ (!launch_editor(am_ਰਸਤਾ(state, "final-commit"), &ਸੁਨੇਹਾ, ਨਿਰਾ)) {
				ਮੁਕਤ(state->ਸੁਨੇਹਾ);
				state->ਸੁਨੇਹਾ = strbuf_detach(&ਸੁਨੇਹਾ, &state->ਸੁਨੇਹਾ_len);
			}
			strbuf_release(&ਸੁਨੇਹਾ);
		} ਨਹੀਂਤਾਂ ਜੇ (*ਜਵਾਬ == 'v' || *ਜਵਾਬ == 'V') {
			ਅਚਲ ਅੱਖਰ *pager = git_pager(the_repository, 1);
			ਬਣਤਰ ਬੱਚਾ_process cp = ਬੱਚਾ_PROCESS_INIT;

			ਜੇ (!pager)
				pager = "cat";
			prepare_pager_ਦਲੀਲਾਂ(&cp, pager);
			strvec_ਧੱਕੋ(&cp.ਦਲੀਲਾਂ, am_ਰਸਤਾ(state, "patch"));
			ਲੜੀਵਾਰ_command(&cp);
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
ਸਥਿਰ ਖਾਲੀ am_ਲੜੀਵਾਰ(ਬਣਤਰ am_state *state, ਪੂਰਨ resume)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;

	unlink(am_ਰਸਤਾ(state, "dirtyindex"));

	ਜੇ (repo_refresh_and_write_ਸੂਚਕਾਂਕ(the_repository, REFRESH_QUIET, 0, 0,
					 ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ) < 0)
		die(_("unable to write index file"));

	ਜੇ (repo_ਸੂਚਕਾਂਕ_has_changes(the_repository, ਨਿਰਾ, &sb)) {
		write_state_bool(state, "dirtyindex", 1);
		die(_("Dirty index: cannot apply patches (dirty: %s)"), sb.buf);
	}

	strbuf_release(&sb);

	ਜਦੋਂ (state->cur <= state->ਆਖਰੀ) {
		ਅਚਲ ਅੱਖਰ *mail = am_ਰਸਤਾ(state, msgnum(state));
		ਪੂਰਨ apply_ਸਥਿਤੀ;
		ਪੂਰਨ ਮੰਜ਼ਿਲ_keep;

		reset_ident_date();

		ਜੇ (!file_exists(mail))
			ਜਾਓ ਅਗਲਾ;

		ਜੇ (resume) {
			validate_resume_state(state);
		} ਨਹੀਂਤਾਂ {
			ਪੂਰਨ skip;

			ਜੇ (state->rebasing)
				skip = parse_mail_rebase(state, mail);
			ਨਹੀਂਤਾਂ
				skip = parse_mail(state, mail);

			ਜੇ (skip)
				ਜਾਓ ਅਗਲਾ; /* mail should be skipped */

			ਜੇ (state->signoff)
				am_append_signoff(state);

			write_author_script(state);
			write_commit_ਸੁਨੇਹਾ(state);
		}

		ਜੇ (state->interactive && do_interactive(state))
			ਜਾਓ ਅਗਲਾ;

		ਮੰਜ਼ਿਲ_keep = 0;
		ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_or_missing_file(am_ਰਸਤਾ(state, "patch"))) {
			ਚੋਣ (state->ਖਾਲੀ_ਥਾਵਾਂ_type) {
			ਹਾਲਤ DROP_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT:
				say(state, stdout, _("Skipping: %.*s"), linelen(state->ਸੁਨੇਹਾ), state->ਸੁਨੇਹਾ);
				ਜਾਓ ਅਗਲਾ;
				ਰੋਕੋ;
			ਹਾਲਤ KEEP_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT:
				ਮੰਜ਼ਿਲ_keep = 1;
				say(state, stdout, _("Creating an empty commit: %.*s"),
					linelen(state->ਸੁਨੇਹਾ), state->ਸੁਨੇਹਾ);
				ਰੋਕੋ;
			ਹਾਲਤ STOP_ON_ਖਾਲੀ_ਥਾਵਾਂ_COMMIT:
				printf_ln(_("Patch is empty."));
				die_user_resolve(state);
				ਰੋਕੋ;
			}
		}

		ਜੇ (ਲੜੀਵਾਰ_applypatch_ਸੁਨੇਹਾ_hook(state))
			ਬਾਹਰ_ਨਿਕਲੋ(1);
		ਜੇ (ਮੰਜ਼ਿਲ_keep)
			ਜਾਓ commit;

		say(state, stdout, _("Applying: %.*s"), linelen(state->ਸੁਨੇਹਾ), state->ਸੁਨੇਹਾ);

		apply_ਸਥਿਤੀ = ਲੜੀਵਾਰ_apply(state, ਨਿਰਾ);

		ਜੇ (apply_ਸਥਿਤੀ && state->threeway) {
			ਬਣਤਰ strbuf sb = STRBUF_INIT;

			strbuf_addstr(&sb, am_ਰਸਤਾ(state, "patch-merge-index"));
			apply_ਸਥਿਤੀ = fall_ਪਿੱਠ_threeway(state, sb.buf);
			strbuf_release(&sb);

			/*
			 * Applying the patch to an earlier tree and merging
			 * the result may have produced the same tree as ours.
			 */
			ਜੇ (!apply_ਸਥਿਤੀ &&
			    !repo_ਸੂਚਕਾਂਕ_has_changes(the_repository, ਨਿਰਾ, ਨਿਰਾ)) {
				say(state, stdout, _("No changes -- Patch already applied."));
				ਜਾਓ ਅਗਲਾ;
			}
		}

		ਜੇ (apply_ਸਥਿਤੀ) {
			printf_ln(_("Patch failed at %s %.*s"), msgnum(state),
				linelen(state->ਸੁਨੇਹਾ), state->ਸੁਨੇਹਾ);

			ਜੇ (advice_enabled(ADVICE_AM_WORK_DIR))
				advise(_("Use 'git am --show-current-patch=diff' to see the failed patch"));

			die_user_resolve(state);
		}

commit:
		do_commit(state);

ਅਗਲਾ:
		am_ਅਗਲਾ(state);

		ਜੇ (resume)
			am_ਲੱਦੋ(state);
		resume = 0;
	}

	ਜੇ (!is_ਖਾਲੀ_ਥਾਵਾਂ_or_missing_file(am_ਰਸਤਾ(state, "rewritten"))) {
		ਪ੍ਰਤਿਜ੍ਞਾ(state->rebasing);
		copy_notes_for_rebase(state);
		ਲੜੀਵਾਰ_post_rewrite_hook(state);
	}

	/*
	 * In rebasing mode, it's up to the caller to take care of
	 * housekeeping.
	 */
	ਜੇ (!state->rebasing) {
		am_destroy(state);
		ਲੜੀਵਾਰ_auto_maintenance(the_repository, state->quiet);
	}
}

/**
 * Resume the current am session after patch application failure. The user did
 * all the hard work, and we do not have to do any patch application. Just
 * trust and commit what the user has in the index and working tree. If `allow_empty`
 * is true, commit as an empty commit when index has not changed and lacking a patch.
 */
ਸਥਿਰ ਖਾਲੀ am_resolve(ਬਣਤਰ am_state *state, ਪੂਰਨ allow_ਖਾਲੀ_ਥਾਵਾਂ)
{
	validate_resume_state(state);

	say(state, stdout, _("Applying: %.*s"), linelen(state->ਸੁਨੇਹਾ), state->ਸੁਨੇਹਾ);

	ਜੇ (!repo_ਸੂਚਕਾਂਕ_has_changes(the_repository, ਨਿਰਾ, ਨਿਰਾ)) {
		ਜੇ (allow_ਖਾਲੀ_ਥਾਵਾਂ && is_ਖਾਲੀ_ਥਾਵਾਂ_or_missing_file(am_ਰਸਤਾ(state, "patch"))) {
			printf_ln(_("No changes - recorded it as an empty commit."));
		} ਨਹੀਂਤਾਂ {
			printf_ln(_("No changes - did you forget to use 'git add'?\n"
				    "If there is nothing left to stage, chances are that something else\n"
				    "already introduced the same changes; you might want to skip this patch."));
			die_user_resolve(state);
		}
	}

	ਜੇ (unmerged_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ)) {
		printf_ln(_("You still have unmerged paths in your index.\n"
			"You should 'git add' each file with resolved conflicts to mark them as such.\n"
			"You might run `git rm` on a file to accept \"deleted by them\" for it."));
		die_user_resolve(state);
	}

	ਜੇ (state->interactive) {
		write_ਸੂਚਕਾਂਕ_patch(state);
		ਜੇ (do_interactive(state))
			ਜਾਓ ਅਗਲਾ;
	}

	repo_rerere(the_repository, 0);

	do_commit(state);

ਅਗਲਾ:
	am_ਅਗਲਾ(state);
	am_ਲੱਦੋ(state);
	am_ਲੜੀਵਾਰ(state, 0);
}

/**
 * Performs a checkout fast-forward from `head` to `remote`. If `reset` is
 * true, any unmerged entries will be discarded. Returns 0 on success, -1 on
 * failure.
 */
ਸਥਿਰ ਪੂਰਨ fast_forward_ਮੰਜ਼ਿਲ(ਬਣਤਰ ਰੁੱਖ *ਸਿਰ, ਬਣਤਰ ਰੁੱਖ *remote, ਪੂਰਨ reset)
{
	ਬਣਤਰ ਤਾਲਾ_file ਤਾਲਾ_file = ਤਾਲਾ_INIT;
	ਬਣਤਰ unpack_trees_options opts;
	ਬਣਤਰ ਰੁੱਖ_desc t[2];

	ਜੇ (repo_parse_ਰੁੱਖ(the_repository, ਸਿਰ) || repo_parse_ਰੁੱਖ(the_repository, remote))
		ਵਾਪਸ -1;

	repo_hold_locked_ਸੂਚਕਾਂਕ(the_repository, &ਤਾਲਾ_file, ਤਾਲਾ_DIE_ON_ERROR);

	refresh_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, REFRESH_QUIET, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ);

	memset(&opts, 0, ਆਕਾਰ(opts));
	opts.ਸਿਰ_idx = 1;
	opts.src_ਸੂਚਕਾਂਕ = the_repository->ਸੂਚਕਾਂਕ;
	opts.dst_ਸੂਚਕਾਂਕ = the_repository->ਸੂਚਕਾਂਕ;
	opts.update = 1;
	opts.ਮਿਲਾਓ = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = twoway_ਮਿਲਾਓ;
	init_ਰੁੱਖ_desc(&t[0], &ਸਿਰ->object.oid, ਸਿਰ->ਬਫਰ, ਸਿਰ->ਮਾਪ);
	init_ਰੁੱਖ_desc(&t[1], &remote->object.oid, remote->ਬਫਰ, remote->ਮਾਪ);

	ਜੇ (unpack_trees(2, t, &opts)) {
		rollback_ਤਾਲਾ_file(&ਤਾਲਾ_file);
		ਵਾਪਸ -1;
	}

	ਜੇ (write_locked_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, &ਤਾਲਾ_file, COMMIT_ਤਾਲਾ))
		die(_("unable to write new index file"));

	ਵਾਪਸ 0;
}

/**
 * Merges a tree into the index. The index's stat info will take precedence
 * over the merged tree's. Returns 0 on success, -1 on failure.
 */
ਸਥਿਰ ਪੂਰਨ ਮਿਲਾਓ_ਰੁੱਖ(ਬਣਤਰ ਰੁੱਖ *ਰੁੱਖ)
{
	ਬਣਤਰ ਤਾਲਾ_file ਤਾਲਾ_file = ਤਾਲਾ_INIT;
	ਬਣਤਰ unpack_trees_options opts;
	ਬਣਤਰ ਰੁੱਖ_desc t[1];

	ਜੇ (repo_parse_ਰੁੱਖ(the_repository, ਰੁੱਖ))
		ਵਾਪਸ -1;

	repo_hold_locked_ਸੂਚਕਾਂਕ(the_repository, &ਤਾਲਾ_file, ਤਾਲਾ_DIE_ON_ERROR);

	memset(&opts, 0, ਆਕਾਰ(opts));
	opts.ਸਿਰ_idx = 1;
	opts.src_ਸੂਚਕਾਂਕ = the_repository->ਸੂਚਕਾਂਕ;
	opts.dst_ਸੂਚਕਾਂਕ = the_repository->ਸੂਚਕਾਂਕ;
	opts.ਮਿਲਾਓ = 1;
	opts.fn = oneway_ਮਿਲਾਓ;
	init_ਰੁੱਖ_desc(&t[0], &ਰੁੱਖ->object.oid, ਰੁੱਖ->ਬਫਰ, ਰੁੱਖ->ਮਾਪ);

	ਜੇ (unpack_trees(1, t, &opts)) {
		rollback_ਤਾਲਾ_file(&ਤਾਲਾ_file);
		ਵਾਪਸ -1;
	}

	ਜੇ (write_locked_ਸੂਚਕਾਂਕ(the_repository->ਸੂਚਕਾਂਕ, &ਤਾਲਾ_file, COMMIT_ਤਾਲਾ))
		die(_("unable to write new index file"));

	ਵਾਪਸ 0;
}

/**
 * Clean the index without touching entries that are not modified between
 * `head` and `remote`.
 */
ਸਥਿਰ ਪੂਰਨ clean_ਸੂਚਕਾਂਕ(ਅਚਲ ਬਣਤਰ object_id *ਸਿਰ, ਅਚਲ ਬਣਤਰ object_id *remote)
{
	ਬਣਤਰ ਰੁੱਖ *ਸਿਰ_ਰੁੱਖ, *remote_ਰੁੱਖ, *ਸੂਚਕਾਂਕ_ਰੁੱਖ;
	ਬਣਤਰ object_id ਸੂਚਕਾਂਕ;

	ਸਿਰ_ਰੁੱਖ = repo_parse_ਰੁੱਖ_ਅਸਿੱਧਾ(the_repository, ਸਿਰ);
	ਜੇ (!ਸਿਰ_ਰੁੱਖ)
		ਵਾਪਸ error(_("Could not parse object '%s'."), oid_ਮੰਜ਼ਿਲ_hex(ਸਿਰ));

	remote_ਰੁੱਖ = repo_parse_ਰੁੱਖ_ਅਸਿੱਧਾ(the_repository, remote);
	ਜੇ (!remote_ਰੁੱਖ)
		ਵਾਪਸ error(_("Could not parse object '%s'."), oid_ਮੰਜ਼ਿਲ_hex(remote));

	repo_read_ਸੂਚਕਾਂਕ_unmerged(the_repository);

	ਜੇ (fast_forward_ਮੰਜ਼ਿਲ(ਸਿਰ_ਰੁੱਖ, ਸਿਰ_ਰੁੱਖ, 1))
		ਵਾਪਸ -1;

	ਜੇ (write_ਸੂਚਕਾਂਕ_as_ਰੁੱਖ(&ਸੂਚਕਾਂਕ, the_repository->ਸੂਚਕਾਂਕ,
				repo_get_ਸੂਚਕਾਂਕ_file(the_repository),
				0, ਨਿਰਾ))
		ਵਾਪਸ -1;

	ਸੂਚਕਾਂਕ_ਰੁੱਖ = repo_parse_ਰੁੱਖ_ਅਸਿੱਧਾ(the_repository, &ਸੂਚਕਾਂਕ);
	ਜੇ (!ਸੂਚਕਾਂਕ_ਰੁੱਖ)
		ਵਾਪਸ error(_("Could not parse object '%s'."), oid_ਮੰਜ਼ਿਲ_hex(&ਸੂਚਕਾਂਕ));

	ਜੇ (fast_forward_ਮੰਜ਼ਿਲ(ਸੂਚਕਾਂਕ_ਰੁੱਖ, remote_ਰੁੱਖ, 0))
		ਵਾਪਸ -1;

	ਜੇ (ਮਿਲਾਓ_ਰੁੱਖ(remote_ਰੁੱਖ))
		ਵਾਪਸ -1;

	remove_branch_state(the_repository, 0);

	ਵਾਪਸ 0;
}

/**
 * Resets rerere's merge resolution metadata.
 */
ਸਥਿਰ ਖਾਲੀ am_rerere_clear(ਖਾਲੀ)
{
	ਬਣਤਰ string_list ਮਿਲਾਓ_rr = STRING_LIST_INIT_DUP;
	rerere_clear(the_repository, &ਮਿਲਾਓ_rr);
	string_list_clear(&ਮਿਲਾਓ_rr, 1);
}

/**
 * Resume the current am session by skipping the current patch.
 */
ਸਥਿਰ ਖਾਲੀ am_skip(ਬਣਤਰ am_state *state)
{
	ਬਣਤਰ object_id ਸਿਰ;

	am_rerere_clear();

	ਜੇ (repo_get_oid(the_repository, "HEAD", &ਸਿਰ))
		oidcpy(&ਸਿਰ, the_hash_algo->ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ);

	ਜੇ (clean_ਸੂਚਕਾਂਕ(&ਸਿਰ, &ਸਿਰ))
		die(_("failed to clean index"));

	ਜੇ (state->rebasing) {
		FILE *fp = xfopen(am_ਰਸਤਾ(state, "rewritten"), "a");

		ਪ੍ਰਤਿਜ੍ਞਾ(!is_null_oid(&state->orig_commit));
		fprintf(fp, "%s ", oid_ਮੰਜ਼ਿਲ_hex(&state->orig_commit));
		fprintf(fp, "%s\n", oid_ਮੰਜ਼ਿਲ_hex(&ਸਿਰ));
		fclose(fp);
	}

	am_ਅਗਲਾ(state);
	am_ਲੱਦੋ(state);
	am_ਲੜੀਵਾਰ(state, 0);
}

/**
 * Returns true if it is safe to reset HEAD to the ORIG_HEAD, false otherwise.
 *
 * It is not safe to reset HEAD when:
 * 1. git-am previously failed because the index was dirty.
 * 2. HEAD has moved since git-am previously failed.
 */
ਸਥਿਰ ਪੂਰਨ ਸੁਰੱਖਿਅਤ_ਮੰਜ਼ਿਲ_abort(ਅਚਲ ਬਣਤਰ am_state *state)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਬਣਤਰ object_id abort_safety, ਸਿਰ;

	ਜੇ (file_exists(am_ਰਸਤਾ(state, "dirtyindex")))
		ਵਾਪਸ 0;

	ਜੇ (read_state_file(&sb, state, "abort-safety", 1) > 0) {
		ਜੇ (get_oid_hex(sb.buf, &abort_safety))
			die(_("could not parse %s"), am_ਰਸਤਾ(state, "abort-safety"));
	} ਨਹੀਂਤਾਂ
		oidclr(&abort_safety, the_repository->hash_algo);
	strbuf_release(&sb);

	ਜੇ (repo_get_oid(the_repository, "HEAD", &ਸਿਰ))
		oidclr(&ਸਿਰ, the_repository->hash_algo);

	ਜੇ (oideq(&ਸਿਰ, &abort_safety))
		ਵਾਪਸ 1;

	warning(_("You seem to have moved HEAD since the last 'am' failure.\n"
		"Not rewinding to ORIG_HEAD"));

	ਵਾਪਸ 0;
}

/**
 * Aborts the current am session if it is safe to do so.
 */
ਸਥਿਰ ਖਾਲੀ am_abort(ਬਣਤਰ am_state *state)
{
	ਬਣਤਰ object_id curr_ਸਿਰ, orig_ਸਿਰ;
	ਪੂਰਨ has_curr_ਸਿਰ, has_orig_ਸਿਰ;
	ਅੱਖਰ *curr_branch;

	ਜੇ (!ਸੁਰੱਖਿਅਤ_ਮੰਜ਼ਿਲ_abort(state)) {
		am_destroy(state);
		ਵਾਪਸ;
	}

	am_rerere_clear();

	curr_branch = refs_resolve_refdup(get_main_ref_store(the_repository),
					  "HEAD", 0, &curr_ਸਿਰ, ਨਿਰਾ);
	has_curr_ਸਿਰ = curr_branch && !is_null_oid(&curr_ਸਿਰ);
	ਜੇ (!has_curr_ਸਿਰ)
		oidcpy(&curr_ਸਿਰ, the_hash_algo->ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ);

	has_orig_ਸਿਰ = !repo_get_oid(the_repository, "ORIG_HEAD", &orig_ਸਿਰ);
	ਜੇ (!has_orig_ਸਿਰ)
		oidcpy(&orig_ਸਿਰ, the_hash_algo->ਖਾਲੀ_ਥਾਵਾਂ_ਰੁੱਖ);

	ਜੇ (clean_ਸੂਚਕਾਂਕ(&curr_ਸਿਰ, &orig_ਸਿਰ))
		die(_("failed to clean index"));

	ਜੇ (has_orig_ਸਿਰ)
		refs_update_ref(get_main_ref_store(the_repository),
				"am --abort", "HEAD", &orig_ਸਿਰ,
				has_curr_ਸਿਰ ? &curr_ਸਿਰ : ਨਿਰਾ, 0,
				UPDATE_REFS_DIE_ON_ERR);
	ਨਹੀਂਤਾਂ ਜੇ (curr_branch)
		refs_delete_ref(get_main_ref_store(the_repository), ਨਿਰਾ,
				curr_branch, ਨਿਰਾ, REF_NO_DEREF);

	ਮੁਕਤ(curr_branch);
	am_destroy(state);
}

ਸਥਿਰ ਪੂਰਨ show_patch(ਬਣਤਰ am_state *state, ਗਿਣਤੀ resume_type resume_ਬਹੁਲਕ)
{
	ਬਣਤਰ strbuf sb = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *patch_ਰਸਤਾ;
	ਪੂਰਨ len;

	ਜੇ (!is_null_oid(&state->orig_commit)) {
		ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;

		strvec_pushl(&cmd.ਦਲੀਲਾਂ, "show", oid_ਮੰਜ਼ਿਲ_hex(&state->orig_commit),
			     "--", ਨਿਰਾ);
		cmd.git_cmd = 1;
		ਵਾਪਸ ਲੜੀਵਾਰ_command(&cmd);
	}

	ਚੋਣ (resume_ਬਹੁਲਕ) {
	ਹਾਲਤ RESUME_SHOW_PATCH_RAW:
		patch_ਰਸਤਾ = am_ਰਸਤਾ(state, msgnum(state));
		ਰੋਕੋ;
	ਹਾਲਤ RESUME_SHOW_PATCH_DIFF:
		patch_ਰਸਤਾ = am_ਰਸਤਾ(state, "patch");
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("invalid mode for --show-current-patch");
	}

	len = strbuf_read_file(&sb, patch_ਰਸਤਾ, 0);
	ਜੇ (len < 0)
		die_errno(_("failed to read '%s'"), patch_ਰਸਤਾ);

	ਤਿਆਰੀ_pager(the_repository);
	write_ਵਿੱਚ_ਭਰਿਆ(1, sb.buf, sb.len);
	strbuf_release(&sb);
	ਵਾਪਸ 0;
}

/**
 * parse_options() callback that validates and sets opt->value to the
 * PATCH_FORMAT_* enum value corresponding to `arg`.
 */
ਸਥਿਰ ਪੂਰਨ parse_opt_patchformat(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਪੂਰਨ *opt_ਮੁੱਲ = opt->ਮੁੱਲ;

	ਜੇ (unset)
		*opt_ਮੁੱਲ = PATCH_FORMAT_UNKNOWN;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "mbox"))
		*opt_ਮੁੱਲ = PATCH_FORMAT_MBOX;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "stgit"))
		*opt_ਮੁੱਲ = PATCH_FORMAT_STGIT;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "stgit-series"))
		*opt_ਮੁੱਲ = PATCH_FORMAT_STGIT_SERIES;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "hg"))
		*opt_ਮੁੱਲ = PATCH_FORMAT_HG;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "mboxrd"))
		*opt_ਮੁੱਲ = PATCH_FORMAT_MBOXRD;
	/*
	 * Please update $__git_patchformat in git-completion.bash
	 * when you add new options
	 */
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ error(_("invalid value for '%s': '%s'"),
			     "--patch-format", arg);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ parse_opt_show_current_patch(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਪੂਰਨ *opt_ਮੁੱਲ = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);

	ਜੇ (!arg)
		*opt_ਮੁੱਲ = opt->defval;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "raw"))
		*opt_ਮੁੱਲ = RESUME_SHOW_PATCH_RAW;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "diff"))
		*opt_ਮੁੱਲ = RESUME_SHOW_PATCH_DIFF;
	/*
	 * Please update $__git_showcurrentpatch in git-completion.bash
	 * when you add new options
	 */
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ error(_("invalid value for '%s': '%s'"),
			     "--show-current-patch", arg);
	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_am(ਪੂਰਨ argc,
	   ਅਚਲ ਅੱਖਰ **argv,
	   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ am_state state;
	ਪੂਰਨ ਦੁਆਧਾਰੀ = -1;
	ਪੂਰਨ keep_cr = -1;
	ਪੂਰਨ patch_format = PATCH_FORMAT_UNKNOWN;
	ਗਿਣਤੀ resume_type resume_ਬਹੁਲਕ = RESUME_FALSE;
	ਪੂਰਨ ਵਿੱਚ_ਤਰੱਕੀ;
	ਪੂਰਨ ret = 0;

	ਅਚਲ ਅੱਖਰ * ਅਚਲ usage[] = {
		N_("git am [<options>] [(<mbox> | <Maildir>)...]"),
		N_("git am [<options>] (--continue | --skip | --abort)"),
		ਨਿਰਾ
	};

	ਬਣਤਰ option options[] = {
		OPT_BOOL('i', "interactive", &state.interactive,
			N_("run interactively")),
		OPT_BOOL('n', "no-verify", &state.no_verify,
			N_("bypass pre-applypatch and applypatch-msg hooks")),
		OPT_HIDDEN_BOOL('b', "binary", &ਦੁਆਧਾਰੀ,
			N_("historical option -- no-op")),
		OPT_BOOL('3', "3way", &state.threeway,
			N_("allow fall back on 3way merging if needed")),
		OPT__QUIET(&state.quiet, N_("be quiet")),
		OPT_ਸੈੱਟ_INT('s', "signoff", &state.signoff,
			N_("add a Signed-off-by trailer to the commit message"),
			SIGNOFF_EXPLICIT),
		OPT_BOOL('u', "utf8", &state.utf8,
			N_("recode into utf8 (default)")),
		OPT_ਸੈੱਟ_INT('k', "keep", &state.keep,
			N_("pass -k flag to git-mailinfo"), KEEP_TRUE),
		OPT_ਸੈੱਟ_INT(0, "keep-non-patch", &state.keep,
			N_("pass -b flag to git-mailinfo"), KEEP_NON_PATCH),
		OPT_BOOL('m', "message-id", &state.ਸੁਨੇਹਾ_id,
			N_("pass -m flag to git-mailinfo")),
		OPT_ਸੈੱਟ_INT(0, "keep-cr", &keep_cr,
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
		OPT_PASSTHRU_ARGV(0, "ignore-space-change", &state.git_apply_opts, ਨਿਰਾ,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU_ARGV(0, "ignore-whitespace", &state.git_apply_opts, ਨਿਰਾ,
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
		OPT_PASSTHRU_ARGV('C', ਨਿਰਾ, &state.git_apply_opts, N_("n"),
			N_("pass it through git-apply"),
			0),
		OPT_PASSTHRU_ARGV('p', ਨਿਰਾ, &state.git_apply_opts, N_("num"),
			N_("pass it through git-apply"),
			0),
		OPT_CALLBACK(0, "patch-format", &patch_format, N_("format"),
			N_("format the patch(es) are in"),
			parse_opt_patchformat),
		OPT_PASSTHRU_ARGV(0, "reject", &state.git_apply_opts, ਨਿਰਾ,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_STRING(0, "resolvemsg", &state.resolvemsg, ਨਿਰਾ,
			N_("override error message when patch failure occurs")),
		OPT_CMDMODE(0, "continue", &resume_ਬਹੁਲਕ,
			N_("continue applying patches after resolving a conflict"),
			RESUME_RESOLVED),
		OPT_CMDMODE('r', "resolved", &resume_ਬਹੁਲਕ,
			N_("synonyms for --continue"),
			RESUME_RESOLVED),
		OPT_CMDMODE(0, "skip", &resume_ਬਹੁਲਕ,
			N_("skip the current patch"),
			RESUME_SKIP),
		OPT_CMDMODE(0, "abort", &resume_ਬਹੁਲਕ,
			N_("restore the original branch and abort the patching operation"),
			RESUME_ABORT),
		OPT_CMDMODE(0, "quit", &resume_ਬਹੁਲਕ,
			N_("abort the patching operation but keep HEAD where it is"),
			RESUME_QUIT),
		{
			.type = OPTION_CALLBACK,
			.long_name = "show-current-patch",
			.ਮੁੱਲ = &resume_ਬਹੁਲਕ,
			.precision = ਆਕਾਰ(resume_ਬਹੁਲਕ),
			.argh = "(diff|raw)",
			.help = N_("show the patch being applied"),
			.flags = PARSE_OPT_CMDMODE | PARSE_OPT_OPTARG | PARSE_OPT_NONEG | PARSE_OPT_LITERAL_ARGHELP,
			.callback = parse_opt_show_current_patch,
			.defval = RESUME_SHOW_PATCH_RAW,
		},
		OPT_CMDMODE(0, "retry", &resume_ਬਹੁਲਕ,
			N_("try to apply current patch again"),
			RESUME_APPLY),
		OPT_CMDMODE(0, "allow-empty", &resume_ਬਹੁਲਕ,
			N_("record the empty patch as an empty commit"),
			RESUME_ALLOW_ਖਾਲੀ_ਥਾਵਾਂ),
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
			.ਮੁੱਲ = &state.sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG-sign commits"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_CALLBACK_F(0, "empty", &state.ਖਾਲੀ_ਥਾਵਾਂ_type, "(stop|drop|keep)",
		  N_("how to handle empty patches"),
		  PARSE_OPT_NONEG, am_option_parse_ਖਾਲੀ_ਥਾਵਾਂ),
		OPT_HIDDEN_BOOL(0, "rebasing", &state.rebasing,
			N_("(internal use for git-rebase)")),
		OPT_ਸਿਰਾ()
	};

	show_usage_with_options_if_asked(argc, argv, usage, options);

	repo_config(the_repository, git_default_config, ਨਿਰਾ);

	am_state_init(&state);

	ਵਿੱਚ_ਤਰੱਕੀ = am_ਵਿੱਚ_ਤਰੱਕੀ(&state);
	ਜੇ (ਵਿੱਚ_ਤਰੱਕੀ)
		am_ਲੱਦੋ(&state);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, usage, 0);

	ਜੇ (ਦੁਆਧਾਰੀ >= 0)
		fprintf_ln(stderr, _("The -b/--binary option has been a no-op for long time, and\n"
				"it will be removed. Please do not use it anymore."));

	/* Ensure a valid committer ident can be constructed */
	git_committer_info(IDENT_STRICT);

	ਜੇ (repo_read_ਸੂਚਕਾਂਕ_preload(the_repository, ਨਿਰਾ, 0) < 0)
		die(_("failed to read the index"));

	ਜੇ (ਵਿੱਚ_ਤਰੱਕੀ) {
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
		ਜੇ (argc || (resume_ਬਹੁਲਕ == RESUME_FALSE && !isatty(0)))
			die(_("previous rebase directory %s still exists but mbox given."),
				state.dir);

		ਜੇ (resume_ਬਹੁਲਕ == RESUME_FALSE)
			resume_ਬਹੁਲਕ = RESUME_APPLY;

		ਜੇ (state.signoff == SIGNOFF_EXPLICIT)
			am_append_signoff(&state);
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ strvec paths = STRVEC_INIT;
		ਪੂਰਨ i;

		/*
		 * Handle stray state directory in the independent-run case. In
		 * the --rebasing case, it is up to the caller to take care of
		 * stray directories.
		 */
		ਜੇ (file_exists(state.dir) && !state.rebasing) {
			ਜੇ (resume_ਬਹੁਲਕ == RESUME_ABORT || resume_ਬਹੁਲਕ == RESUME_QUIT) {
				am_destroy(&state);
				am_state_release(&state);
				ਵਾਪਸ 0;
			}

			die(_("Stray %s directory found.\n"
				"Use \"git am --abort\" to remove it."),
				state.dir);
		}

		ਜੇ (resume_ਬਹੁਲਕ)
			die(_("Resolve operation not in progress, we are not resuming."));

		ਲਈ (i = 0; i < argc; i++) {
			ਜੇ (is_absolute_ਰਸਤਾ(argv[i]) || !ਅਗੇਤਰ)
				strvec_ਧੱਕੋ(&paths, argv[i]);
			ਨਹੀਂਤਾਂ
				strvec_ਧੱਕੋ(&paths, mkpath("%s/%s", ਅਗੇਤਰ, argv[i]));
		}

		ਜੇ (state.interactive && !paths.nr)
			die(_("interactive mode requires patches on the command line"));

		am_ਤਿਆਰੀ(&state, patch_format, paths.v, keep_cr);

		strvec_clear(&paths);
	}

	ਚੋਣ (resume_ਬਹੁਲਕ) {
	ਹਾਲਤ RESUME_FALSE:
		am_ਲੜੀਵਾਰ(&state, 0);
		ਰੋਕੋ;
	ਹਾਲਤ RESUME_APPLY:
		am_ਲੜੀਵਾਰ(&state, 1);
		ਰੋਕੋ;
	ਹਾਲਤ RESUME_RESOLVED:
	ਹਾਲਤ RESUME_ALLOW_ਖਾਲੀ_ਥਾਵਾਂ:
		am_resolve(&state, resume_ਬਹੁਲਕ == RESUME_ALLOW_ਖਾਲੀ_ਥਾਵਾਂ ? 1 : 0);
		ਰੋਕੋ;
	ਹਾਲਤ RESUME_SKIP:
		am_skip(&state);
		ਰੋਕੋ;
	ਹਾਲਤ RESUME_ABORT:
		am_abort(&state);
		ਰੋਕੋ;
	ਹਾਲਤ RESUME_QUIT:
		am_rerere_clear();
		am_destroy(&state);
		ਰੋਕੋ;
	ਹਾਲਤ RESUME_SHOW_PATCH_RAW:
	ਹਾਲਤ RESUME_SHOW_PATCH_DIFF:
		ret = show_patch(&state, resume_ਬਹੁਲਕ);
		ਰੋਕੋ;
	ਮੂਲ:
		BUG("invalid resume value");
	}

	am_state_release(&state);

	ਵਾਪਸ ret;
}
