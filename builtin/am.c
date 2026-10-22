/*
 * Builtin "git am"
 *
 * Based on git-am.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "abspath.h"
#ଅନ୍ତର୍ଭାଵ "advice.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "editor.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "hook.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "tempfile.h"
#ଅନ୍ତର୍ଭାଵ "lockfile.h"
#ଅନ୍ତର୍ଭାଵ "cache-tree.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "diff.h"
#ଅନ୍ତର୍ଭାଵ "unpack-trees.h"
#ଅନ୍ତର୍ଭାଵ "branch.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "preload-index.h"
#ଅନ୍ତର୍ଭାଵ "sequencer.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"
#ଅନ୍ତର୍ଭାଵ "merge-ort-wrappers.h"
#ଅନ୍ତର୍ଭାଵ "log-tree.h"
#ଅନ୍ତର୍ଭାଵ "notes-utils.h"
#ଅନ୍ତର୍ଭାଵ "rerere.h"
#ଅନ୍ତର୍ଭାଵ "mailinfo.h"
#ଅନ୍ତର୍ଭାଵ "apply.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "pager.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "pretty.h"

/**
 * Returns the length of the first line of msg.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା linelen(ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ)
{
	ଫେରନ୍ତୁ strchrnul(ସନ୍ଦେଶ, '\n') - ସନ୍ଦେଶ;
}

/**
 * Returns true if `str` consists of only whitespace, false otherwise.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା str_isspace(ଅଚଳ ଅକ୍ଷର *str)
{
	ପାଇଁ (; *str; str++)
		ଯଦି (!isspace(*str))
			ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ 1;
}

ଗଣନା patch_format {
	PATCH_FORMAT_UNKNOWN = 0,
	PATCH_FORMAT_MBOX,
	PATCH_FORMAT_STGIT,
	PATCH_FORMAT_STGIT_SERIES,
	PATCH_FORMAT_HG,
	PATCH_FORMAT_MBOXRD
};

ଗଣନା keep_type {
	KEEP_FALSE = 0,
	KEEP_TRUE,      /* pass -k flag to git-mailinfo */
	KEEP_NON_PATCH  /* pass -b flag to git-mailinfo */
};

ଗଣନା scissors_type {
	SCISSORS_UNSET = -1,
	SCISSORS_FALSE = 0,  /* pass --no-scissors to git-mailinfo */
	SCISSORS_TRUE        /* pass --scissors to git-mailinfo */
};

ଗଣନା signoff_type {
	SIGNOFF_FALSE = 0,
	SIGNOFF_TRUE = 1,
	SIGNOFF_EXPLICIT /* --signoff was set on the command-line */
};

ଗଣନା resume_type {
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

ଗଣନା empty_action {
	STOP_ON_EMPTY_COMMIT = 0,  /* output errors and stop in the middle of an am session */
	DROP_EMPTY_COMMIT,         /* skip with a notice message, unless "--quiet" has been passed */
	KEEP_EMPTY_COMMIT,         /* keep recording as empty commits */
};

ସଂରଚନା am_state {
	/* state directory path */
	ଅକ୍ଷର *dir;

	/* current and last patch numbers, 1-indexed */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cur;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା last;

	/* commit metadata and message */
	ଅକ୍ଷର *author_name;
	ଅକ୍ଷର *author_email;
	ଅକ୍ଷର *author_date;
	ଅକ୍ଷର *ସନ୍ଦେଶ;
	size_t ସନ୍ଦେଶ_len;

	/* when --rebasing, records the original commit the patch came from */
	ସଂରଚନା object_id orig_commit;

	/* number of digits in patch filename */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା prec;

	/* various operating modes and command line options */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା interactive;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା no_verify;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା threeway;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା signoff; /* enum signoff_type */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା utf8;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep; /* enum keep_type */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା message_id;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା scissors; /* enum scissors_type */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା quoted_cr; /* enum quoted_cr_action */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା empty_type; /* enum empty_action */
	ସଂରଚନା strvec git_apply_opts;
	ଅଚଳ ଅକ୍ଷର *resolvemsg;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା committer_date_is_author_date;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ignore_date;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା allow_rerere_autoupdate;
	ଅଚଳ ଅକ୍ଷର *sign_commit;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା rebasing;
};

/**
 * Initializes am_state with the default values.
 */
ସ୍ଥିର ଶୂନ୍ୟ am_state_init(ସଂରଚନା am_state *state)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା gpgsign;

	memset(state, 0, ଆକାର(*state));

	state->dir = repo_git_path(the_repository, "rebase-apply");

	state->prec = 4;

	repo_config_get_bool(the_repository, "am.threeway", &state->threeway);

	state->utf8 = 1;

	repo_config_get_bool(the_repository, "am.messageid", &state->message_id);

	state->scissors = SCISSORS_UNSET;
	state->quoted_cr = quoted_cr_unset;

	strvec_init(&state->git_apply_opts);

	ଯଦି (!repo_config_get_bool(the_repository, "commit.gpgsign", &gpgsign))
		state->sign_commit = gpgsign ? "" : ଖାଲି;
}

/**
 * Releases memory allocated by an am_state.
 */
ସ୍ଥିର ଶୂନ୍ୟ am_state_release(ସଂରଚନା am_state *state)
{
	ମୁକ୍ତ(state->dir);
	ମୁକ୍ତ(state->author_name);
	ମୁକ୍ତ(state->author_email);
	ମୁକ୍ତ(state->author_date);
	ମୁକ୍ତ(state->ସନ୍ଦେଶ);
	strvec_clear(&state->git_apply_opts);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା am_option_parse_quoted_cr(ଅଚଳ ସଂରଚନା option *opt,
				     ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	BUG_ON_OPT_NEG(unset);

	ଯଦି (mailinfo_parse_quoted_cr_action(arg, opt->ମୂଲ୍ୟ) != 0)
		ଫେରନ୍ତୁ error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା am_option_parse_empty(ଅଚଳ ସଂରଚନା option *opt,
				     ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *opt_ମୂଲ୍ୟ = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG(unset);

	ଯଦି (!strcmp(arg, "stop"))
		*opt_ମୂଲ୍ୟ = STOP_ON_EMPTY_COMMIT;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "drop"))
		*opt_ମୂଲ୍ୟ = DROP_EMPTY_COMMIT;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "keep"))
		*opt_ମୂଲ୍ୟ = KEEP_EMPTY_COMMIT;
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ error(_("invalid value for '%s': '%s'"), "--empty", arg);

	ଫେରନ୍ତୁ 0;
}

/**
 * Returns path relative to the am_state directory.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ ଅକ୍ଷର *am_path(ଅଚଳ ସଂରଚନା am_state *state, ଅଚଳ ଅକ୍ଷର *path)
{
	ଫେରନ୍ତୁ mkpath("%s/%s", state->dir, path);
}

/**
 * For convenience to call write_file()
 */
ସ୍ଥିର ଶୂନ୍ୟ write_state_text(ଅଚଳ ସଂରଚନା am_state *state,
			     ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *string)
{
	write_file(am_path(state, name), "%s", string);
}

ସ୍ଥିର ଶୂନ୍ୟ write_state_ଗଣନା(ଅଚଳ ସଂରଚନା am_state *state,
			      ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ)
{
	write_file(am_path(state, name), "%d", ମୂଲ୍ୟ);
}

ସ୍ଥିର ଶୂନ୍ୟ write_state_bool(ଅଚଳ ସଂରଚନା am_state *state,
			     ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ)
{
	write_state_text(state, name, ମୂଲ୍ୟ ? "t" : "f");
}

/**
 * If state->quiet is false, calls fprintf(fp, fmt, ...), and appends a newline
 * at the end.
 */
__attribute__((format (printf, 3, 4)))
ସ୍ଥିର ଶୂନ୍ୟ say(ଅଚଳ ସଂରଚନା am_state *state, FILE *fp, ଅଚଳ ଅକ୍ଷର *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	ଯଦି (!state->quiet) {
		vfprintf(fp, fmt, ap);
		putc('\n', fp);
	}
	va_end(ap);
}

/**
 * Returns 1 if there is an am session in progress, 0 otherwise.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା am_in_progress(ଅଚଳ ସଂରଚନା am_state *state)
{
	ସଂରଚନା stat st;

	ଯଦି (lstat(state->dir, &st) < 0 || !S_ISDIR(st.st_mode))
		ଫେରନ୍ତୁ 0;
	ଯଦି (lstat(am_path(state, "last"), &st) || !S_ISREG(st.st_mode))
		ଫେରନ୍ତୁ 0;
	ଯଦି (lstat(am_path(state, "next"), &st) || !S_ISREG(st.st_mode))
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ 1;
}

/**
 * Reads the contents of `file` in the `state` directory into `sb`. Returns the
 * number of bytes read on success, -1 if the file does not exist. If `trim` is
 * set, trailing whitespace will be removed.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_state_file(ସଂରଚନା strbuf *sb, ଅଚଳ ସଂରଚନା am_state *state,
			ଅଚଳ ଅକ୍ଷର *file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା trim)
{
	strbuf_reset(sb);

	ଯଦି (strbuf_read_file(sb, am_path(state, file), 0) >= 0) {
		ଯଦି (trim)
			strbuf_trim(sb);

		ଫେରନ୍ତୁ sb->len;
	}

	ଯଦି (errno == ENOENT)
		ଫେରନ୍ତୁ -1;

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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_am_author_script(ସଂରଚନା am_state *state)
{
	ଅଚଳ ଅକ୍ଷର *filename = am_path(state, "author-script");

	ପ୍ରତିଜ୍ଞା(!state->author_name);
	ପ୍ରତିଜ୍ଞା(!state->author_email);
	ପ୍ରତିଜ୍ଞା(!state->author_date);

	ଫେରନ୍ତୁ read_author_script(filename, &state->author_name,
				  &state->author_email, &state->author_date, 1);
}

/**
 * Saves state->author_name, state->author_email and state->author_date in the
 * state directory's "author-script" file.
 */
ସ୍ଥିର ଶୂନ୍ୟ write_author_script(ଅଚଳ ସଂରଚନା am_state *state)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;

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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_commit_ସନ୍ଦେଶ(ସଂରଚନା am_state *state)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;

	ପ୍ରତିଜ୍ଞା(!state->ସନ୍ଦେଶ);

	ଯଦି (read_state_file(&sb, state, "final-commit", 0) < 0) {
		strbuf_release(&sb);
		ଫେରନ୍ତୁ -1;
	}

	state->ସନ୍ଦେଶ = strbuf_detach(&sb, &state->ସନ୍ଦେଶ_len);
	ଫେରନ୍ତୁ 0;
}

/**
 * Saves state->msg in the state directory's "final-commit" file.
 */
ସ୍ଥିର ଶୂନ୍ୟ write_commit_ସନ୍ଦେଶ(ଅଚଳ ସଂରଚନା am_state *state)
{
	ଅଚଳ ଅକ୍ଷର *filename = am_path(state, "final-commit");
	write_file_buf(filename, state->ସନ୍ଦେଶ, state->ସନ୍ଦେଶ_len);
}

/**
 * Loads state from disk.
 */
ସ୍ଥିର ଶୂନ୍ୟ am_load(ସଂରଚନା am_state *state)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;

	ଯଦି (read_state_file(&sb, state, "next", 1) < 0)
		BUG("state file 'next' does not exist");
	state->cur = strtol(sb.buf, ଖାଲି, 10);

	ଯଦି (read_state_file(&sb, state, "last", 1) < 0)
		BUG("state file 'last' does not exist");
	state->last = strtol(sb.buf, ଖାଲି, 10);

	ଯଦି (read_am_author_script(state) < 0)
		die(_("could not parse author script"));

	read_commit_ସନ୍ଦେଶ(state);

	ଯଦି (read_state_file(&sb, state, "original-commit", 1) < 0)
		oidclr(&state->orig_commit, the_repository->hash_algo);
	ଅନ୍ୟଥା ଯଦି (get_oid_hex(sb.buf, &state->orig_commit) < 0)
		die(_("could not parse %s"), am_path(state, "original-commit"));

	read_state_file(&sb, state, "threeway", 1);
	state->threeway = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "quiet", 1);
	state->quiet = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "sign", 1);
	state->signoff = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "utf8", 1);
	state->utf8 = !strcmp(sb.buf, "t");

	ଯଦି (file_exists(am_path(state, "rerere-autoupdate"))) {
		read_state_file(&sb, state, "rerere-autoupdate", 1);
		state->allow_rerere_autoupdate = strcmp(sb.buf, "t") ?
			RERERE_NOAUTOUPDATE : RERERE_AUTOUPDATE;
	} ଅନ୍ୟଥା {
		state->allow_rerere_autoupdate = 0;
	}

	read_state_file(&sb, state, "keep", 1);
	ଯଦି (!strcmp(sb.buf, "t"))
		state->keep = KEEP_TRUE;
	ଅନ୍ୟଥା ଯଦି (!strcmp(sb.buf, "b"))
		state->keep = KEEP_NON_PATCH;
	ଅନ୍ୟଥା
		state->keep = KEEP_FALSE;

	read_state_file(&sb, state, "messageid", 1);
	state->message_id = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "scissors", 1);
	ଯଦି (!strcmp(sb.buf, "t"))
		state->scissors = SCISSORS_TRUE;
	ଅନ୍ୟଥା ଯଦି (!strcmp(sb.buf, "f"))
		state->scissors = SCISSORS_FALSE;
	ଅନ୍ୟଥା
		state->scissors = SCISSORS_UNSET;

	read_state_file(&sb, state, "quoted-cr", 1);
	ଯଦି (!*sb.buf)
		state->quoted_cr = quoted_cr_unset;
	ଅନ୍ୟଥା ଯଦି (mailinfo_parse_quoted_cr_action(sb.buf, &state->quoted_cr) != 0)
		die(_("could not parse %s"), am_path(state, "quoted-cr"));

	read_state_file(&sb, state, "apply-opt", 1);
	strvec_clear(&state->git_apply_opts);
	ଯଦି (sq_dequote_to_strvec(sb.buf, &state->git_apply_opts) < 0)
		die(_("could not parse %s"), am_path(state, "apply-opt"));

	state->rebasing = !!file_exists(am_path(state, "rebasing"));

	strbuf_release(&sb);
}

/**
 * Removes the am_state directory, forcefully terminating the current am
 * session.
 */
ସ୍ଥିର ଶୂନ୍ୟ am_destroy(ଅଚଳ ସଂରଚନା am_state *state)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;

	strbuf_addstr(&sb, state->dir);
	remove_dir_recursively(&sb, 0);
	strbuf_release(&sb);
}

/**
 * Runs applypatch-msg hook. Returns its exit code.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_applypatch_ସନ୍ଦେଶ_hook(ସଂରଚନା am_state *state)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	ପ୍ରତିଜ୍ଞା(state->ସନ୍ଦେଶ);

	ଯଦି (!state->no_verify) {
		ସଂରଚନା run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;
		strvec_ଠେଲ(&opt.args, am_path(state, "final-commit"));
		ret = run_hooks_opt(the_repository, "applypatch-msg", &opt);
	}

	ଯଦି (!ret) {
		FREE_AND_NULL(state->ସନ୍ଦେଶ);
		ଯଦି (read_commit_ସନ୍ଦେଶ(state) < 0)
			die(_("'%s' was deleted by the applypatch-msg hook"),
				am_path(state, "final-commit"));
	}

	ଫେରନ୍ତୁ ret;
}

/**
 * Runs post-rewrite hook. Returns it exit code.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_post_rewrite_hook(ଅଚଳ ସଂରଚନା am_state *state)
{
	ସଂରଚନା run_hooks_opt opt = RUN_HOOKS_OPT_INIT;

	strvec_ଠେଲ(&opt.args, "rebase");
	opt.path_to_stdin = am_path(state, "rewritten");

	ଫେରନ୍ତୁ run_hooks_opt(the_repository, "post-rewrite", &opt);
}

/**
 * Reads the state directory's "rewritten" file, and copies notes from the old
 * commits listed in the file to their rewritten commits.
 *
 * Returns 0 on success, -1 on failure.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା copy_notes_for_rebase(ଅଚଳ ସଂରଚନା am_state *state)
{
	ସଂରଚନା notes_rewrite_cfg *c;
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *invalid_ଧାଡ଼ି = _("Malformed input line: '%s'.");
	ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ = "Notes added by 'git rebase'";
	FILE *fp;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	ପ୍ରତିଜ୍ଞା(state->rebasing);

	c = init_copy_notes_for_rewrite("rebase");
	ଯଦି (!c)
		ଫେରନ୍ତୁ 0;

	fp = xfopen(am_path(state, "rewritten"), "r");

	ଯେତେବେଳେ (!strbuf_getline_lf(&sb, fp)) {
		ସଂରଚନା object_id from_obj, to_obj;
		ଅଚଳ ଅକ୍ଷର *p;

		ଯଦି (sb.len != the_hash_algo->hexsz * 2 + 1) {
			ret = error(invalid_ଧାଡ଼ି, sb.buf);
			ଯାଅ finish;
		}

		ଯଦି (parse_oid_hex(sb.buf, &from_obj, &p)) {
			ret = error(invalid_ଧାଡ଼ି, sb.buf);
			ଯାଅ finish;
		}

		ଯଦି (*p != ' ') {
			ret = error(invalid_ଧାଡ଼ି, sb.buf);
			ଯାଅ finish;
		}

		ଯଦି (get_oid_hex(p + 1, &to_obj)) {
			ret = error(invalid_ଧାଡ଼ି, sb.buf);
			ଯାଅ finish;
		}

		ଯଦି (copy_note_for_rewrite(c, &from_obj, &to_obj))
			ret = error(_("Failed to copy notes from '%s' to '%s'"),
					oid_to_hex(&from_obj), oid_to_hex(&to_obj));
	}

finish:
	finish_copy_notes_for_rewrite(the_repository, c, ସନ୍ଦେଶ);
	fclose(fp);
	strbuf_release(&sb);
	ଫେରନ୍ତୁ ret;
}

/**
 * Determines if the file looks like a piece of RFC2822 mail by grabbing all
 * non-indented lines and checking if they look like they begin with valid
 * header field names.
 *
 * Returns 1 if the file looks like a piece of mail, 0 otherwise.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_mail(FILE *fp)
{
	ଅଚଳ ଅକ୍ଷର *header_regex = "^[!-9;-~]+:";
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	regex_t regex;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 1;

	ଯଦି (fseek(fp, 0L, SEEK_SET))
		die_errno(_("fseek failed"));

	ଯଦି (regcomp(&regex, header_regex, REG_NOSUB | REG_EXTENDED))
		die("invalid pattern: %s", header_regex);

	ଯେତେବେଳେ (!strbuf_getline(&sb, fp)) {
		ଯଦି (!sb.len)
			ଭାଙ୍ଗ; /* End of header */

		/* Ignore indented folded lines */
		ଯଦି (*sb.buf == '\t' || *sb.buf == ' ')
			ଜାରି;

		/* It's a header if it matches header_regex */
		ଯଦି (regexec(&regex, sb.buf, 0, ଖାଲି, 0)) {
			ret = 0;
			ଯାଅ done;
		}
	}

done:
	regfree(&regex);
	strbuf_release(&sb);
	ଫେରନ୍ତୁ ret;
}

/**
 * Attempts to detect the patch_format of the patches contained in `paths`,
 * returning the PATCH_FORMAT_* enum value. Returns PATCH_FORMAT_UNKNOWN if
 * detection fails.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା detect_patch_format(ଅଚଳ ଅକ୍ଷର **paths)
{
	ଗଣନା patch_format ret = PATCH_FORMAT_UNKNOWN;
	ସଂରଚନା strbuf l1 = STRBUF_INIT;
	ସଂରଚନା strbuf l2 = STRBUF_INIT;
	ସଂରଚନା strbuf l3 = STRBUF_INIT;
	FILE *fp;

	/*
	 * We default to mbox format if input is from stdin and for directories
	 */
	ଯଦି (!*paths || !strcmp(*paths, "-") || is_directory(*paths))
		ଫେରନ୍ତୁ PATCH_FORMAT_MBOX;

	/*
	 * Otherwise, check the first few lines of the first patch, starting
	 * from the first non-blank line, to try to detect its format.
	 */

	fp = xfopen(*paths, "r");

	ଯେତେବେଳେ (!strbuf_getline(&l1, fp)) {
		ଯଦି (l1.len)
			ଭାଙ୍ଗ;
	}

	ଯଦି (starts_with(l1.buf, "From ") || starts_with(l1.buf, "From: ")) {
		ret = PATCH_FORMAT_MBOX;
		ଯାଅ done;
	}

	ଯଦି (starts_with(l1.buf, "# This series applies on GIT commit")) {
		ret = PATCH_FORMAT_STGIT_SERIES;
		ଯାଅ done;
	}

	ଯଦି (!strcmp(l1.buf, "# HG changeset patch")) {
		ret = PATCH_FORMAT_HG;
		ଯାଅ done;
	}

	strbuf_getline(&l2, fp);
	strbuf_getline(&l3, fp);

	/*
	 * If the second line is empty and the third is a From, Author or Date
	 * entry, this is likely an StGit patch.
	 */
	ଯଦି (l1.len && !l2.len &&
		(starts_with(l3.buf, "From:") ||
		 starts_with(l3.buf, "Author:") ||
		 starts_with(l3.buf, "Date:"))) {
		ret = PATCH_FORMAT_STGIT;
		ଯାଅ done;
	}

	ଯଦି (l1.len && is_mail(fp)) {
		ret = PATCH_FORMAT_MBOX;
		ଯାଅ done;
	}

done:
	fclose(fp);
	strbuf_release(&l1);
	strbuf_release(&l2);
	strbuf_release(&l3);
	ଫେରନ୍ତୁ ret;
}

/**
 * Splits out individual email patches from `paths`, where each path is either
 * a mbox file or a Maildir. Returns 0 on success, -1 on failure.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା split_mail_mbox(ସଂରଚନା am_state *state, ଅଚଳ ଅକ୍ଷର **paths,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep_cr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mboxrd)
{
	ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;
	ସଂରଚନା strbuf last = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	cp.git_cmd = 1;
	strvec_ଠେଲ(&cp.args, "mailsplit");
	strvec_pushf(&cp.args, "-d%d", state->prec);
	strvec_pushf(&cp.args, "-o%s", state->dir);
	strvec_ଠେଲ(&cp.args, "-b");
	ଯଦି (keep_cr)
		strvec_ଠେଲ(&cp.args, "--keep-cr");
	ଯଦି (mboxrd)
		strvec_ଠେଲ(&cp.args, "--mboxrd");
	strvec_ଠେଲ(&cp.args, "--");
	strvec_pushv(&cp.args, paths);

	ret = capture_command(&cp, &last, 8);
	ଯଦି (ret)
		ଯାଅ exit;

	state->cur = 1;
	state->last = strtol(last.buf, ଖାଲି, 10);

exit:
	strbuf_release(&last);
	ଫେରନ୍ତୁ ret ? -1 : 0;
}

/**
 * Callback signature for split_mail_conv(). The foreign patch should be
 * read from `in`, and the converted patch (in RFC2822 mail format) should be
 * written to `out`. Return 0 on success, or -1 on failure.
 */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*mail_conv_fn)(FILE *out, FILE *in, ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep_cr);

/**
 * Calls `fn` for each file in `paths` to convert the foreign patch to the
 * RFC2822 mail format suitable for parsing with git-mailinfo.
 *
 * Returns 0 on success, -1 on failure.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା split_mail_conv(mail_conv_fn fn, ସଂରଚନା am_state *state,
			ଅଚଳ ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep_cr)
{
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *stdin_only[] = {"-", ଖାଲି};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ଯଦି (!*paths)
		paths = stdin_only;

	ପାଇଁ (i = 0; *paths; paths++, i++) {
		FILE *in, *out;
		ଅଚଳ ଅକ୍ଷର *mail;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

		ଯଦି (!strcmp(*paths, "-"))
			in = stdin;
		ଅନ୍ୟଥା
			in = fopen(*paths, "r");

		ଯଦି (!in)
			ଫେରନ୍ତୁ error_errno(_("could not open '%s' for reading"),
					   *paths);

		mail = mkpath("%s/%0*d", state->dir, state->prec, i + 1);

		out = fopen(mail, "w");
		ଯଦି (!out) {
			ଯଦି (in != stdin)
				fclose(in);
			ଫେରନ୍ତୁ error_errno(_("could not open '%s' for writing"),
					   mail);
		}

		ret = fn(out, in, keep_cr);

		fclose(out);
		ଯଦି (in != stdin)
			fclose(in);

		ଯଦି (ret)
			ଫେରନ୍ତୁ error(_("could not parse patch '%s'"), *paths);
	}

	state->cur = 1;
	state->last = i;
	ଫେରନ୍ତୁ 0;
}

/**
 * A split_mail_conv() callback that converts an StGit patch to an RFC2822
 * message suitable for parsing with git-mailinfo.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା stgit_patch_to_mail(FILE *out, FILE *in, ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep_cr UNUSED)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା subject_printed = 0;

	ଯେତେବେଳେ (!strbuf_getline_lf(&sb, in)) {
		ଅଚଳ ଅକ୍ଷର *str;

		ଯଦି (str_isspace(sb.buf))
			ଜାରି;
		ଅନ୍ୟଥା ଯଦି (skip_prefix(sb.buf, "Author:", &str))
			fprintf(out, "From:%s\n", str);
		ଅନ୍ୟଥା ଯଦି (starts_with(sb.buf, "From") || starts_with(sb.buf, "Date"))
			fprintf(out, "%s\n", sb.buf);
		ଅନ୍ୟଥା ଯଦି (!subject_printed) {
			fprintf(out, "Subject: %s\n", sb.buf);
			subject_printed = 1;
		} ଅନ୍ୟଥା {
			fprintf(out, "\n%s\n", sb.buf);
			ଭାଙ୍ଗ;
		}
	}

	strbuf_reset(&sb);
	ଯେତେବେଳେ (strbuf_fread(&sb, 8192, in) > 0) {
		fwrite(sb.buf, 1, sb.len, out);
		strbuf_reset(&sb);
	}

	strbuf_release(&sb);
	ଫେରନ୍ତୁ 0;
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା split_mail_stgit_series(ସଂରଚନା am_state *state, ଅଚଳ ଅକ୍ଷର **paths,
					ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep_cr)
{
	ଅଚଳ ଅକ୍ଷର *series_dir;
	ଅକ୍ଷର *series_dir_buf;
	FILE *fp;
	ସଂରଚନା strvec patches = STRVEC_INIT;
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ଯଦି (!paths[0] || paths[1])
		ଫେରନ୍ତୁ error(_("Only one StGIT patch series can be applied at once"));

	series_dir_buf = xstrdup(*paths);
	series_dir = dirname(series_dir_buf);

	fp = fopen(*paths, "r");
	ଯଦି (!fp) {
		ମୁକ୍ତ(series_dir_buf);
		ଫେରନ୍ତୁ error_errno(_("could not open '%s' for reading"), *paths);
	}

	ଯେତେବେଳେ (!strbuf_getline_lf(&sb, fp)) {
		ଯଦି (*sb.buf == '#')
			ଜାରି; /* skip comment lines */

		strvec_ଠେଲ(&patches, mkpath("%s/%s", series_dir, sb.buf));
	}

	fclose(fp);
	strbuf_release(&sb);
	ମୁକ୍ତ(series_dir_buf);

	ret = split_mail_conv(stgit_patch_to_mail, state, patches.v, keep_cr);

	strvec_clear(&patches);
	ଫେରନ୍ତୁ ret;
}

/**
 * A split_patches_conv() callback that converts a mercurial patch to a RFC2822
 * message suitable for parsing with git-mailinfo.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା hg_patch_to_mail(FILE *out, FILE *in, ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep_cr UNUSED)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା rc = 0;

	ଯେତେବେଳେ (!strbuf_getline_lf(&sb, in)) {
		ଅଚଳ ଅକ୍ଷର *str;

		ଯଦି (skip_prefix(sb.buf, "# User ", &str))
			fprintf(out, "From: %s\n", str);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(sb.buf, "# Date ", &str)) {
			timestamp_t timestamp;
			long tz, tz2;
			ଅକ୍ଷର *end;

			errno = 0;
			timestamp = parse_timestamp(str, &end, 10);
			ଯଦି (errno) {
				rc = error(_("invalid timestamp"));
				ଯାଅ exit;
			}

			ଯଦି (!skip_prefix(end, " ", &str)) {
				rc = error(_("invalid Date line"));
				ଯାଅ exit;
			}

			errno = 0;
			tz = strtol(str, &end, 10);
			ଯଦି (errno) {
				rc = error(_("invalid timezone offset"));
				ଯାଅ exit;
			}

			ଯଦି (*end) {
				rc = error(_("invalid Date line"));
				ଯାଅ exit;
			}

			/*
			 * mercurial's timezone is in seconds west of UTC,
			 * however git's timezone is in hours + minutes east of
			 * UTC. Convert it.
			 */
			tz2 = labs(tz) / 3600 * 100 + labs(tz) % 3600 / 60;
			ଯଦି (tz > 0)
				tz2 = -tz2;

			fprintf(out, "Date: %s\n", show_date(timestamp, tz2, DATE_MODE(RFC2822)));
		} ଅନ୍ୟଥା ଯଦି (starts_with(sb.buf, "# ")) {
			ଜାରି;
		} ଅନ୍ୟଥା {
			fprintf(out, "\n%s\n", sb.buf);
			ଭାଙ୍ଗ;
		}
	}

	strbuf_reset(&sb);
	ଯେତେବେଳେ (strbuf_fread(&sb, 8192, in) > 0) {
		fwrite(sb.buf, 1, sb.len, out);
		strbuf_reset(&sb);
	}
exit:
	strbuf_release(&sb);
	ଫେରନ୍ତୁ rc;
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା split_mail(ସଂରଚନା am_state *state, ଗଣନା patch_format patch_format,
			ଅଚଳ ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep_cr)
{
	ଯଦି (keep_cr < 0) {
		keep_cr = 0;
		repo_config_get_bool(the_repository, "am.keepcr", &keep_cr);
	}

	ବାଛ (patch_format) {
	ସ୍ଥିତି PATCH_FORMAT_MBOX:
		ଫେରନ୍ତୁ split_mail_mbox(state, paths, keep_cr, 0);
	ସ୍ଥିତି PATCH_FORMAT_STGIT:
		ଫେରନ୍ତୁ split_mail_conv(stgit_patch_to_mail, state, paths, keep_cr);
	ସ୍ଥିତି PATCH_FORMAT_STGIT_SERIES:
		ଫେରନ୍ତୁ split_mail_stgit_series(state, paths, keep_cr);
	ସ୍ଥିତି PATCH_FORMAT_HG:
		ଫେରନ୍ତୁ split_mail_conv(hg_patch_to_mail, state, paths, keep_cr);
	ସ୍ଥିତି PATCH_FORMAT_MBOXRD:
		ଫେରନ୍ତୁ split_mail_mbox(state, paths, keep_cr, 1);
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("invalid patch_format");
	}
	ଫେରନ୍ତୁ -1;
}

/**
 * Setup a new am session for applying patches
 */
ସ୍ଥିର ଶୂନ୍ୟ am_setup(ସଂରଚନା am_state *state, ଗଣନା patch_format patch_format,
			ଅଚଳ ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep_cr)
{
	ସଂରଚନା object_id curr_head;
	ଅଚଳ ଅକ୍ଷର *str;
	ସଂରଚନା strbuf sb = STRBUF_INIT;

	ଯଦି (!patch_format)
		patch_format = detect_patch_format(paths);

	ଯଦି (!patch_format) {
		fprintf_ln(stderr, _("Patch format detection failed."));
		die(ଖାଲି);
	}

	ଯଦି (mkdir(state->dir, 0777) < 0 && errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), state->dir);
	refs_delete_ref(get_main_ref_store(the_repository), ଖାଲି,
			"REBASE_HEAD", ଖାଲି, REF_NO_DEREF);

	ଯଦି (split_mail(state, patch_format, paths, keep_cr) < 0) {
		am_destroy(state);
		die(_("Failed to split patches."));
	}

	ଯଦି (state->rebasing)
		state->threeway = 1;

	write_state_bool(state, "threeway", state->threeway);
	write_state_bool(state, "quiet", state->quiet);
	write_state_bool(state, "sign", state->signoff);
	write_state_bool(state, "utf8", state->utf8);

	ଯଦି (state->allow_rerere_autoupdate)
		write_state_bool(state, "rerere-autoupdate",
			 state->allow_rerere_autoupdate == RERERE_AUTOUPDATE);

	ବାଛ (state->keep) {
	ସ୍ଥିତି KEEP_FALSE:
		str = "f";
		ଭାଙ୍ଗ;
	ସ୍ଥିତି KEEP_TRUE:
		str = "t";
		ଭାଙ୍ଗ;
	ସ୍ଥିତି KEEP_NON_PATCH:
		str = "b";
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("invalid value for state->keep");
	}

	write_state_text(state, "keep", str);
	write_state_bool(state, "messageid", state->message_id);

	ବାଛ (state->scissors) {
	ସ୍ଥିତି SCISSORS_UNSET:
		str = "";
		ଭାଙ୍ଗ;
	ସ୍ଥିତି SCISSORS_FALSE:
		str = "f";
		ଭାଙ୍ଗ;
	ସ୍ଥିତି SCISSORS_TRUE:
		str = "t";
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("invalid value for state->scissors");
	}
	write_state_text(state, "scissors", str);

	ବାଛ (state->quoted_cr) {
	ସ୍ଥିତି quoted_cr_unset:
		str = "";
		ଭାଙ୍ଗ;
	ସ୍ଥିତି quoted_cr_nowarn:
		str = "nowarn";
		ଭାଙ୍ଗ;
	ସ୍ଥିତି quoted_cr_warn:
		str = "warn";
		ଭାଙ୍ଗ;
	ସ୍ଥିତି quoted_cr_strip:
		str = "strip";
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("invalid value for state->quoted_cr");
	}
	write_state_text(state, "quoted-cr", str);

	sq_quote_argv(&sb, state->git_apply_opts.v);
	write_state_text(state, "apply-opt", sb.buf);

	ଯଦି (state->rebasing)
		write_state_text(state, "rebasing", "");
	ଅନ୍ୟଥା
		write_state_text(state, "applying", "");

	ଯଦି (!repo_get_oid(the_repository, "HEAD", &curr_head)) {
		write_state_text(state, "abort-safety", oid_to_hex(&curr_head));
		ଯଦି (!state->rebasing)
			refs_update_ref(get_main_ref_store(the_repository),
					"am", "ORIG_HEAD", &curr_head, ଖାଲି,
					0,
					UPDATE_REFS_DIE_ON_ERR);
	} ଅନ୍ୟଥା {
		write_state_text(state, "abort-safety", "");
		ଯଦି (!state->rebasing)
			refs_delete_ref(get_main_ref_store(the_repository),
					ଖାଲି, "ORIG_HEAD", ଖାଲି, 0);
	}

	/*
	 * NOTE: Since the "next" and "last" files determine if an am_state
	 * session is in progress, they should be written last.
	 */

	write_state_ଗଣନା(state, "next", state->cur);
	write_state_ଗଣନା(state, "last", state->last);

	strbuf_release(&sb);
}

/**
 * Increments the patch pointer, and cleans am_state for the application of the
 * next patch.
 */
ସ୍ଥିର ଶୂନ୍ୟ am_next(ସଂରଚନା am_state *state)
{
	ସଂରଚନା object_id head;

	FREE_AND_NULL(state->author_name);
	FREE_AND_NULL(state->author_email);
	FREE_AND_NULL(state->author_date);
	FREE_AND_NULL(state->ସନ୍ଦେଶ);
	state->ସନ୍ଦେଶ_len = 0;

	unlink(am_path(state, "author-script"));
	unlink(am_path(state, "final-commit"));

	oidclr(&state->orig_commit, the_repository->hash_algo);
	unlink(am_path(state, "original-commit"));
	refs_delete_ref(get_main_ref_store(the_repository), ଖାଲି,
			"REBASE_HEAD", ଖାଲି, REF_NO_DEREF);

	ଯଦି (!repo_get_oid(the_repository, "HEAD", &head))
		write_state_text(state, "abort-safety", oid_to_hex(&head));
	ଅନ୍ୟଥା
		write_state_text(state, "abort-safety", "");

	state->cur++;
	write_state_ଗଣନା(state, "next", state->cur);
}

/**
 * Returns the filename of the current patch email.
 */
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *msgnum(ଅଚଳ ସଂରଚନା am_state *state)
{
	ସ୍ଥିର ସଂରଚନା strbuf sb = STRBUF_INIT;

	strbuf_reset(&sb);
	strbuf_addf(&sb, "%0*d", state->prec, state->cur);

	ଫେରନ୍ତୁ sb.buf;
}

/**
 * Dies with a user-friendly message on how to proceed after resolving the
 * problem. This message can be overridden with state->resolvemsg.
 */
ସ୍ଥିର ଶୂନ୍ୟ NORETURN die_user_resolve(ଅଚଳ ସଂରଚନା am_state *state)
{
	ଯଦି (state->resolvemsg) {
		advise_if_enabled(ADVICE_MERGE_CONFLICT, "%s", state->resolvemsg);
	} ଅନ୍ୟଥା {
		ଅଚଳ ଅକ୍ଷର *cmdline = state->interactive ? "git am -i" : "git am";
		ସଂରଚନା strbuf sb = STRBUF_INIT;

		strbuf_addf(&sb, _("When you have resolved this problem, run \"%s --continue\".\n"), cmdline);
		strbuf_addf(&sb, _("If you prefer to skip this patch, run \"%s --skip\" instead.\n"), cmdline);

		ଯଦି (advice_enabled(ADVICE_AM_WORK_DIR) &&
		    is_empty_or_missing_file(am_path(state, "patch")) &&
		    !repo_index_has_changes(the_repository, ଖାଲି, ଖାଲି))
			strbuf_addf(&sb, _("To record the empty patch as an empty commit, run \"%s --allow-empty\".\n"), cmdline);

		strbuf_addf(&sb, _("To restore the original branch and stop patching, run \"%s --abort\"."), cmdline);

		advise_if_enabled(ADVICE_MERGE_CONFLICT, "%s", sb.buf);
		strbuf_release(&sb);
	}

	die(ଖାଲି);
}

/**
 * Appends signoff to the "msg" field of the am_state.
 */
ସ୍ଥିର ଶୂନ୍ୟ am_append_signoff(ସଂରଚନା am_state *state)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;

	strbuf_attach(&sb, state->ସନ୍ଦେଶ, state->ସନ୍ଦେଶ_len, state->ସନ୍ଦେଶ_len + 1);
	append_signoff(&sb, 0, 0);
	state->ସନ୍ଦେଶ = strbuf_detach(&sb, &state->ସନ୍ଦେଶ_len);
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_mail(ସଂରଚନା am_state *state, ଅଚଳ ଅକ୍ଷର *mail)
{
	FILE *fp;
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ସଂରଚନା strbuf ସନ୍ଦେଶ = STRBUF_INIT;
	ସଂରଚନା strbuf author_name = STRBUF_INIT;
	ସଂରଚନା strbuf author_date = STRBUF_INIT;
	ସଂରଚନା strbuf author_email = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ସଂରଚନା mailinfo mi;

	setup_mailinfo(the_repository, &mi);

	ଯଦି (state->utf8)
		mi.metainfo_charset = get_commit_output_encoding();
	ଅନ୍ୟଥା
		mi.metainfo_charset = ଖାଲି;

	ବାଛ (state->keep) {
	ସ୍ଥିତି KEEP_FALSE:
		ଭାଙ୍ଗ;
	ସ୍ଥିତି KEEP_TRUE:
		mi.keep_subject = 1;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି KEEP_NON_PATCH:
		mi.keep_non_patch_brackets_in_subject = 1;
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("invalid value for state->keep");
	}

	ଯଦି (state->message_id)
		mi.add_message_id = 1;

	ବାଛ (state->scissors) {
	ସ୍ଥିତି SCISSORS_UNSET:
		ଭାଙ୍ଗ;
	ସ୍ଥିତି SCISSORS_FALSE:
		mi.use_scissors = 0;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି SCISSORS_TRUE:
		mi.use_scissors = 1;
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("invalid value for state->scissors");
	}

	ବାଛ (state->quoted_cr) {
	ସ୍ଥିତି quoted_cr_unset:
		ଭାଙ୍ଗ;
	ସ୍ଥିତି quoted_cr_nowarn:
	ସ୍ଥିତି quoted_cr_warn:
	ସ୍ଥିତି quoted_cr_strip:
		mi.quoted_cr = state->quoted_cr;
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("invalid value for state->quoted_cr");
	}

	mi.ନିବେଶ = xfopen(mail, "r");
	mi.output = xfopen(am_path(state, "info"), "w");
	ଯଦି (mailinfo(&mi, am_path(state, "msg"), am_path(state, "patch")))
		die("could not parse patch");

	fclose(mi.ନିବେଶ);
	fclose(mi.output);

	ଯଦି (mi.format_flowed)
		warning(_("Patch sent with format=flowed; "
			  "space at the end of lines might be lost."));

	/* Extract message and author information */
	fp = xfopen(am_path(state, "info"), "r");
	ଯେତେବେଳେ (!strbuf_getline_lf(&sb, fp)) {
		ଅଚଳ ଅକ୍ଷର *x;

		ଯଦି (skip_prefix(sb.buf, "Subject: ", &x)) {
			ଯଦି (ସନ୍ଦେଶ.len)
				strbuf_addch(&ସନ୍ଦେଶ, '\n');
			strbuf_addstr(&ସନ୍ଦେଶ, x);
		} ଅନ୍ୟଥା ଯଦି (skip_prefix(sb.buf, "Author: ", &x))
			strbuf_addstr(&author_name, x);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(sb.buf, "Email: ", &x))
			strbuf_addstr(&author_email, x);
		ଅନ୍ୟଥା ଯଦି (skip_prefix(sb.buf, "Date: ", &x))
			strbuf_addstr(&author_date, x);
	}
	fclose(fp);

	/* Skip pine's internal folder data */
	ଯଦି (!strcmp(author_name.buf, "Mail System Internal Data")) {
		ret = 1;
		ଯାଅ finish;
	}

	strbuf_addstr(&ସନ୍ଦେଶ, "\n\n");
	strbuf_addbuf(&ସନ୍ଦେଶ, &mi.log_message);
	strbuf_stripspace(&ସନ୍ଦେଶ, ଖାଲି);

	ପ୍ରତିଜ୍ଞା(!state->author_name);
	state->author_name = strbuf_detach(&author_name, ଖାଲି);

	ପ୍ରତିଜ୍ଞା(!state->author_email);
	state->author_email = strbuf_detach(&author_email, ଖାଲି);

	ପ୍ରତିଜ୍ଞା(!state->author_date);
	state->author_date = strbuf_detach(&author_date, ଖାଲି);

	ପ୍ରତିଜ୍ଞା(!state->ସନ୍ଦେଶ);
	state->ସନ୍ଦେଶ = strbuf_detach(&ସନ୍ଦେଶ, &state->ସନ୍ଦେଶ_len);

finish:
	strbuf_release(&ସନ୍ଦେଶ);
	strbuf_release(&author_date);
	strbuf_release(&author_email);
	strbuf_release(&author_name);
	strbuf_release(&sb);
	clear_mailinfo(&mi);
	ଫେରନ୍ତୁ ret;
}

/**
 * Sets commit_id to the commit hash where the mail was generated from.
 * Returns 0 on success, -1 on failure.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_mail_commit_oid(ସଂରଚନା object_id *commit_id, ଅଚଳ ଅକ୍ଷର *mail)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	FILE *fp = xfopen(mail, "r");
	ଅଚଳ ଅକ୍ଷର *x;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	ଯଦି (strbuf_getline_lf(&sb, fp) ||
	    !skip_prefix(sb.buf, "From ", &x) ||
	    get_oid_hex(x, commit_id) < 0)
		ret = -1;

	strbuf_release(&sb);
	fclose(fp);
	ଫେରନ୍ତୁ ret;
}

/**
 * Sets state->msg, state->author_name, state->author_email, state->author_date
 * to the commit's respective info.
 */
ସ୍ଥିର ଶୂନ୍ୟ get_commit_info(ସଂରଚନା am_state *state, ସଂରଚନା commit *commit)
{
	ଅଚଳ ଅକ୍ଷର *buffer, *ident_ଧାଡ଼ି, *ସନ୍ଦେଶ;
	size_t ident_len;
	ସଂରଚନା ident_split id;

	buffer = repo_logmsg_reencode(the_repository, commit, ଖାଲି,
				      get_commit_output_encoding());

	ident_ଧାଡ଼ି = find_commit_header(buffer, "author", &ident_len);
	ଯଦି (!ident_ଧାଡ଼ି)
		die(_("missing author line in commit %s"),
		      oid_to_hex(&commit->object.oid));
	ଯଦି (split_ident_ଧାଡ଼ି(&id, ident_ଧାଡ଼ି, ident_len) < 0)
		die(_("invalid ident line: %.*s"), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ident_len, ident_ଧାଡ଼ି);

	ପ୍ରତିଜ୍ଞା(!state->author_name);
	ଯଦି (id.name_begin)
		state->author_name =
			xmemdupz(id.name_begin, id.name_end - id.name_begin);
	ଅନ୍ୟଥା
		state->author_name = xstrdup("");

	ପ୍ରତିଜ୍ଞା(!state->author_email);
	ଯଦି (id.mail_begin)
		state->author_email =
			xmemdupz(id.mail_begin, id.mail_end - id.mail_begin);
	ଅନ୍ୟଥା
		state->author_email = xstrdup("");

	ପ୍ରତିଜ୍ଞା(!state->author_date);
	state->author_date = xstrdup(show_ident_date(&id, DATE_MODE(NORMAL)));

	ପ୍ରତିଜ୍ଞା(!state->ସନ୍ଦେଶ);
	ସନ୍ଦେଶ = strstr(buffer, "\n\n");
	ଯଦି (!ସନ୍ଦେଶ)
		die(_("unable to parse commit %s"), oid_to_hex(&commit->object.oid));
	state->ସନ୍ଦେଶ = xstrdup(ସନ୍ଦେଶ + 2);
	state->ସନ୍ଦେଶ_len = strlen(state->ସନ୍ଦେଶ);
	repo_unuse_commit_buffer(the_repository, commit, buffer);
}

/**
 * Writes `commit` as a patch to the state directory's "patch" file.
 */
ସ୍ଥିର ଶୂନ୍ୟ write_commit_patch(ଅଚଳ ସଂରଚନା am_state *state, ସଂରଚନା commit *commit)
{
	ସଂରଚନା rev_info rev_info;
	FILE *fp;

	fp = xfopen(am_path(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, ଖାଲି);
	rev_info.diff = 1;
	rev_info.abbrev = 0;
	rev_info.disable_stdin = 1;
	rev_info.show_ମୂଳ_diff = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.flags.ଦ୍ୱିମିକ = 1;
	rev_info.diffopt.flags.full_index = 1;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	add_pending_object(&rev_info, &commit->object, "");
	diff_setup_done(&rev_info.diffopt);
	log_ବୃକ୍ଷ_commit(&rev_info, commit);
	release_revisions(&rev_info);
}

/**
 * Writes the diff of the index against HEAD as a patch to the state
 * directory's "patch" file.
 */
ସ୍ଥିର ଶୂନ୍ୟ write_index_patch(ଅଚଳ ସଂରଚନା am_state *state)
{
	ସଂରଚନା ବୃକ୍ଷ *ବୃକ୍ଷ;
	ସଂରଚନା object_id head;
	ସଂରଚନା rev_info rev_info;
	FILE *fp;

	ଯଦି (!repo_get_oid(the_repository, "HEAD", &head)) {
		ସଂରଚନା commit *commit = lookup_commit_or_die(&head, "HEAD");
		ବୃକ୍ଷ = repo_get_commit_ବୃକ୍ଷ(the_repository, commit);
	} ଅନ୍ୟଥା
		ବୃକ୍ଷ = lookup_ବୃକ୍ଷ(the_repository,
				   the_repository->hash_algo->empty_ବୃକ୍ଷ);

	fp = xfopen(am_path(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, ଖାଲି);
	rev_info.diff = 1;
	rev_info.disable_stdin = 1;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	add_pending_object(&rev_info, &ବୃକ୍ଷ->object, "");
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_mail_rebase(ସଂରଚନା am_state *state, ଅଚଳ ଅକ୍ଷର *mail)
{
	ସଂରଚନା commit *commit;
	ସଂରଚନା object_id commit_oid;

	ଯଦି (get_mail_commit_oid(&commit_oid, mail) < 0)
		die(_("could not parse %s"), mail);

	commit = lookup_commit_or_die(&commit_oid, mail);

	get_commit_info(state, commit);

	write_commit_patch(state, commit);

	oidcpy(&state->orig_commit, &commit_oid);
	write_state_text(state, "original-commit", oid_to_hex(&commit_oid));
	refs_update_ref(get_main_ref_store(the_repository), "am",
			"REBASE_HEAD", &commit_oid,
			ଖାଲି, REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);

	ଫେରନ୍ତୁ 0;
}

/**
 * Applies current patch with git-apply. Returns 0 on success, -1 otherwise. If
 * `index_file` is not NULL, the patch will be applied to that index.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା run_apply(ଅଚଳ ସଂରଚନା am_state *state, ଅଚଳ ଅକ୍ଷର *index_file)
{
	ସଂରଚନା strvec apply_paths = STRVEC_INIT;
	ସଂରଚନା strvec apply_opts = STRVEC_INIT;
	ସଂରଚନା apply_state apply_state;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res, opts_ବାମ;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା force_apply = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା options = 0;
	ଅଚଳ ଅକ୍ଷର **apply_argv;

	ଯଦି (init_apply_state(&apply_state, the_repository, ଖାଲି))
		BUG("init_apply_state() failed");

	strvec_ଠେଲ(&apply_opts, "apply");
	strvec_pushv(&apply_opts, state->git_apply_opts.v);

	/*
	 * Build a copy that apply_parse_options() can rearrange.
	 * apply_opts.v keeps referencing the allocated strings for
	 * strvec_clear() to release.
	 */
	DUP_ARRAY(apply_argv, apply_opts.v, apply_opts.nr);

	opts_ବାମ = apply_parse_options(apply_opts.nr, apply_argv,
					&apply_state, &force_apply, &options,
					ଖାଲି);

	ଯଦି (opts_ବାମ != 0)
		die("unknown option passed through to git apply");

	ଯଦି (index_file) {
		apply_state.index_file = index_file;
		apply_state.cached = 1;
	} ଅନ୍ୟଥା
		apply_state.check_index = 1;

	/*
	 * If we are allowed to fall back on 3-way merge, don't give false
	 * errors during the initial attempt.
	 */
	ଯଦି (state->threeway && !index_file)
		apply_state.apply_verbosity = verbosity_silent;

	ଯଦି (check_apply_state(&apply_state, force_apply))
		BUG("check_apply_state() failed");

	strvec_ଠେଲ(&apply_paths, am_path(state, "patch"));

	res = apply_all_patches(&apply_state, apply_paths.nr, apply_paths.v, options);

	strvec_clear(&apply_paths);
	strvec_clear(&apply_opts);
	clear_apply_state(&apply_state);
	ମୁକ୍ତ(apply_argv);

	ଯଦି (res)
		ଫେରନ୍ତୁ res;

	ଯଦି (index_file) {
		/* Reload index as apply_all_patches() will have modified it. */
		discard_index(the_repository->index);
		read_index_from(the_repository->index, index_file,
				repo_get_git_dir(the_repository));
	}

	ଫେରନ୍ତୁ 0;
}

/**
 * Builds an index that contains just the blobs needed for a 3way merge.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା build_fake_ancestor(ଅଚଳ ସଂରଚନା am_state *state, ଅଚଳ ଅକ୍ଷର *index_file)
{
	ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	strvec_ଠେଲ(&cp.args, "apply");
	strvec_pushv(&cp.args, state->git_apply_opts.v);
	strvec_pushf(&cp.args, "--build-fake-ancestor=%s", index_file);
	strvec_ଠେଲ(&cp.args, am_path(state, "patch"));

	ଯଦି (run_command(&cp))
		ଫେରନ୍ତୁ -1;

	ଫେରନ୍ତୁ 0;
}

/**
 * Attempt a threeway merge, using index_path as the temporary index.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fall_back_threeway(ଅଚଳ ସଂରଚନା am_state *state, ଅଚଳ ଅକ୍ଷର *index_path)
{
	ସଂରଚନା object_id their_ବୃକ୍ଷ, our_ବୃକ୍ଷ;
	ସଂରଚନା object_id bases[1] = { 0 };
	ସଂରଚନା merge_options o;
	ସଂରଚନା commit *result;
	ଅକ୍ଷର *their_ବୃକ୍ଷ_name;

	ଯଦି (repo_get_oid(the_repository, "HEAD", &our_ବୃକ୍ଷ) < 0)
		oidcpy(&our_ବୃକ୍ଷ, the_hash_algo->empty_ବୃକ୍ଷ);

	ଯଦି (build_fake_ancestor(state, index_path))
		ଫେରନ୍ତୁ error("could not build fake ancestor");

	discard_index(the_repository->index);
	read_index_from(the_repository->index, index_path, repo_get_git_dir(the_repository));

	ଯଦି (write_index_as_ବୃକ୍ଷ(&bases[0], the_repository->index, index_path, 0, ଖାଲି))
		ଫେରନ୍ତୁ error(_("Repository lacks necessary blobs to fall back on 3-way merge."));

	say(state, stdout, _("Using index info to reconstruct a base tree..."));

	ଯଦି (!state->quiet) {
		/*
		 * List paths that needed 3-way fallback, so that the user can
		 * review them with extra care to spot mismerges.
		 */
		ସଂରଚନା rev_info rev_info;

		repo_init_revisions(the_repository, &rev_info, ଖାଲି);
		rev_info.diffopt.output_format = DIFF_FORMAT_NAME_STATUS;
		rev_info.diffopt.filter |= diff_filter_bit('A');
		rev_info.diffopt.filter |= diff_filter_bit('M');
		add_pending_oid(&rev_info, "HEAD", &our_ବୃକ୍ଷ, 0);
		diff_setup_done(&rev_info.diffopt);
		run_diff_index(&rev_info, DIFF_INDEX_CACHED);
		release_revisions(&rev_info);
	}

	ଯଦି (run_apply(state, index_path))
		ଫେରନ୍ତୁ error(_("Did you hand edit your patch?\n"
				"It does not apply to blobs recorded in its index."));

	ଯଦି (write_index_as_ବୃକ୍ଷ(&their_ବୃକ୍ଷ, the_repository->index, index_path, 0, ଖାଲି))
		ଫେରନ୍ତୁ error("could not write tree");

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
	their_ବୃକ୍ଷ_name = xstrfmt("%.*s", linelen(state->ସନ୍ଦେଶ), state->ସନ୍ଦେଶ);
	o.branch2 = their_ବୃକ୍ଷ_name;
	o.ancestor = "constructed fake ancestor";
	o.detect_directory_renames = MERGE_DIRECTORY_RENAMES_NONE;

	ଯଦି (state->quiet)
		o.verbosity = 0;

	ଯଦି (merge_ort_generic(&o, &our_ବୃକ୍ଷ, &their_ବୃକ୍ଷ, 1, bases, &result)) {
		repo_rerere(the_repository, state->allow_rerere_autoupdate);
		ମୁକ୍ତ(their_ବୃକ୍ଷ_name);
		ଫେରନ୍ତୁ error(_("Failed to merge in the changes."));
	}

	ମୁକ୍ତ(their_ବୃକ୍ଷ_name);
	ଫେରନ୍ତୁ 0;
}

/**
 * Commits the current index with state->msg as the commit message and
 * state->author_name, state->author_email and state->author_date as the author
 * information.
 */
ସ୍ଥିର ଶୂନ୍ୟ do_commit(ଅଚଳ ସଂରଚନା am_state *state)
{
	ସଂରଚନା object_id ବୃକ୍ଷ, parent, commit;
	ଅଚଳ ସଂରଚନା object_id *old_oid;
	ସଂରଚନା commit_list *parents = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *reflog_ସନ୍ଦେଶ, *author, *committer = ଖାଲି;
	ସଂରଚନା strbuf sb = STRBUF_INIT;

	ଯଦି (!state->no_verify && run_hooks(the_repository, "pre-applypatch"))
		exit(1);

	ଯଦି (write_index_as_ବୃକ୍ଷ(&ବୃକ୍ଷ, the_repository->index,
				repo_get_index_file(the_repository),
				0, ଖାଲି))
		die(_("git write-tree failed to write a tree"));

	ଯଦି (!repo_get_oid_commit(the_repository, "HEAD", &parent)) {
		old_oid = &parent;
		commit_list_ଭର୍ତ୍ତି(lookup_commit(the_repository, &parent),
				   &parents);
	} ଅନ୍ୟଥା {
		old_oid = ଖାଲି;
		say(state, stderr, _("applying to an empty history"));
	}

	author = fmt_ident(state->author_name, state->author_email,
		WANT_AUTHOR_IDENT,
			state->ignore_date ? ଖାଲି : state->author_date,
			IDENT_STRICT);

	ଯଦି (state->committer_date_is_author_date)
		committer = fmt_ident(getenv("GIT_COMMITTER_NAME"),
				      getenv("GIT_COMMITTER_EMAIL"),
				      WANT_COMMITTER_IDENT,
				      state->ignore_date ? ଖାଲି
							 : state->author_date,
				      IDENT_STRICT);

	ଯଦି (commit_ବୃକ୍ଷ_extended(state->ସନ୍ଦେଶ, state->ସନ୍ଦେଶ_len, &ବୃକ୍ଷ, parents,
				 &commit, author, committer, state->sign_commit,
				 ଖାଲି))
		die(_("failed to write commit object"));

	reflog_ସନ୍ଦେଶ = getenv("GIT_REFLOG_ACTION");
	ଯଦି (!reflog_ସନ୍ଦେଶ)
		reflog_ସନ୍ଦେଶ = "am";

	strbuf_addf(&sb, "%s: %.*s", reflog_ସନ୍ଦେଶ, linelen(state->ସନ୍ଦେଶ),
			state->ସନ୍ଦେଶ);

	refs_update_ref(get_main_ref_store(the_repository), sb.buf, "HEAD",
			&commit, old_oid, 0,
			UPDATE_REFS_DIE_ON_ERR);

	ଯଦି (state->rebasing) {
		FILE *fp = xfopen(am_path(state, "rewritten"), "a");

		ପ୍ରତିଜ୍ଞା(!is_null_oid(&state->orig_commit));
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
ସ୍ଥିର ଶୂନ୍ୟ validate_resume_state(ଅଚଳ ସଂରଚନା am_state *state)
{
	ଯଦି (!state->ସନ୍ଦେଶ)
		die(_("cannot resume: %s does not exist."),
			am_path(state, "final-commit"));

	ଯଦି (!state->author_name || !state->author_email || !state->author_date)
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_interactive(ସଂରଚନା am_state *state)
{
	ପ୍ରତିଜ୍ଞା(state->ସନ୍ଦେଶ);

	ପାଇଁ (;;) {
		ଅକ୍ଷର reply[64];

		puts(_("Commit Body is:"));
		puts("--------------------------");
		printf("%s", state->ସନ୍ଦେଶ);
		puts("--------------------------");

		/*
		 * TRANSLATORS: Make sure to include [y], [n], [e], [v] and [a]
		 * in your translation. The program will only accept English
		 * input at this point.
		 */
		printf(_("Apply? [y]es/[n]o/[e]dit/[v]iew patch/[a]ccept all: "));
		ଯଦି (!fgets(reply, ଆକାର(reply), stdin))
			die("unable to read from stdin; aborting");

		ଯଦି (*reply == 'y' || *reply == 'Y') {
			ଫେରନ୍ତୁ 0;
		} ଅନ୍ୟଥା ଯଦି (*reply == 'a' || *reply == 'A') {
			state->interactive = 0;
			ଫେରନ୍ତୁ 0;
		} ଅନ୍ୟଥା ଯଦି (*reply == 'n' || *reply == 'N') {
			ଫେରନ୍ତୁ 1;
		} ଅନ୍ୟଥା ଯଦି (*reply == 'e' || *reply == 'E') {
			ସଂରଚନା strbuf ସନ୍ଦେଶ = STRBUF_INIT;

			ଯଦି (!launch_editor(am_path(state, "final-commit"), &ସନ୍ଦେଶ, ଖାଲି)) {
				ମୁକ୍ତ(state->ସନ୍ଦେଶ);
				state->ସନ୍ଦେଶ = strbuf_detach(&ସନ୍ଦେଶ, &state->ସନ୍ଦେଶ_len);
			}
			strbuf_release(&ସନ୍ଦେଶ);
		} ଅନ୍ୟଥା ଯଦି (*reply == 'v' || *reply == 'V') {
			ଅଚଳ ଅକ୍ଷର *pager = git_pager(the_repository, 1);
			ସଂରଚନା child_process cp = CHILD_PROCESS_INIT;

			ଯଦି (!pager)
				pager = "cat";
			prepare_pager_args(&cp, pager);
			strvec_ଠେଲ(&cp.args, am_path(state, "patch"));
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
ସ୍ଥିର ଶୂନ୍ୟ am_run(ସଂରଚନା am_state *state, ପୂର୍ଣ୍ଣସଂଖ୍ୟା resume)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;

	unlink(am_path(state, "dirtyindex"));

	ଯଦି (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 ଖାଲି, ଖାଲି, ଖାଲି) < 0)
		die(_("unable to write index file"));

	ଯଦି (repo_index_has_changes(the_repository, ଖାଲି, &sb)) {
		write_state_bool(state, "dirtyindex", 1);
		die(_("Dirty index: cannot apply patches (dirty: %s)"), sb.buf);
	}

	strbuf_release(&sb);

	ଯେତେବେଳେ (state->cur <= state->last) {
		ଅଚଳ ଅକ୍ଷର *mail = am_path(state, msgnum(state));
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା apply_status;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା to_keep;

		reset_ident_date();

		ଯଦି (!file_exists(mail))
			ଯାଅ next;

		ଯଦି (resume) {
			validate_resume_state(state);
		} ଅନ୍ୟଥା {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା skip;

			ଯଦି (state->rebasing)
				skip = parse_mail_rebase(state, mail);
			ଅନ୍ୟଥା
				skip = parse_mail(state, mail);

			ଯଦି (skip)
				ଯାଅ next; /* mail should be skipped */

			ଯଦି (state->signoff)
				am_append_signoff(state);

			write_author_script(state);
			write_commit_ସନ୍ଦେଶ(state);
		}

		ଯଦି (state->interactive && do_interactive(state))
			ଯାଅ next;

		to_keep = 0;
		ଯଦି (is_empty_or_missing_file(am_path(state, "patch"))) {
			ବାଛ (state->empty_type) {
			ସ୍ଥିତି DROP_EMPTY_COMMIT:
				say(state, stdout, _("Skipping: %.*s"), linelen(state->ସନ୍ଦେଶ), state->ସନ୍ଦେଶ);
				ଯାଅ next;
				ଭାଙ୍ଗ;
			ସ୍ଥିତି KEEP_EMPTY_COMMIT:
				to_keep = 1;
				say(state, stdout, _("Creating an empty commit: %.*s"),
					linelen(state->ସନ୍ଦେଶ), state->ସନ୍ଦେଶ);
				ଭାଙ୍ଗ;
			ସ୍ଥିତି STOP_ON_EMPTY_COMMIT:
				printf_ln(_("Patch is empty."));
				die_user_resolve(state);
				ଭାଙ୍ଗ;
			}
		}

		ଯଦି (run_applypatch_ସନ୍ଦେଶ_hook(state))
			exit(1);
		ଯଦି (to_keep)
			ଯାଅ commit;

		say(state, stdout, _("Applying: %.*s"), linelen(state->ସନ୍ଦେଶ), state->ସନ୍ଦେଶ);

		apply_status = run_apply(state, ଖାଲି);

		ଯଦି (apply_status && state->threeway) {
			ସଂରଚନା strbuf sb = STRBUF_INIT;

			strbuf_addstr(&sb, am_path(state, "patch-merge-index"));
			apply_status = fall_back_threeway(state, sb.buf);
			strbuf_release(&sb);

			/*
			 * Applying the patch to an earlier tree and merging
			 * the result may have produced the same tree as ours.
			 */
			ଯଦି (!apply_status &&
			    !repo_index_has_changes(the_repository, ଖାଲି, ଖାଲି)) {
				say(state, stdout, _("No changes -- Patch already applied."));
				ଯାଅ next;
			}
		}

		ଯଦି (apply_status) {
			printf_ln(_("Patch failed at %s %.*s"), msgnum(state),
				linelen(state->ସନ୍ଦେଶ), state->ସନ୍ଦେଶ);

			ଯଦି (advice_enabled(ADVICE_AM_WORK_DIR))
				advise(_("Use 'git am --show-current-patch=diff' to see the failed patch"));

			die_user_resolve(state);
		}

commit:
		do_commit(state);

next:
		am_next(state);

		ଯଦି (resume)
			am_load(state);
		resume = 0;
	}

	ଯଦି (!is_empty_or_missing_file(am_path(state, "rewritten"))) {
		ପ୍ରତିଜ୍ଞା(state->rebasing);
		copy_notes_for_rebase(state);
		run_post_rewrite_hook(state);
	}

	/*
	 * In rebasing mode, it's up to the caller to take care of
	 * housekeeping.
	 */
	ଯଦି (!state->rebasing) {
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
ସ୍ଥିର ଶୂନ୍ୟ am_resolve(ସଂରଚନା am_state *state, ପୂର୍ଣ୍ଣସଂଖ୍ୟା allow_empty)
{
	validate_resume_state(state);

	say(state, stdout, _("Applying: %.*s"), linelen(state->ସନ୍ଦେଶ), state->ସନ୍ଦେଶ);

	ଯଦି (!repo_index_has_changes(the_repository, ଖାଲି, ଖାଲି)) {
		ଯଦି (allow_empty && is_empty_or_missing_file(am_path(state, "patch"))) {
			printf_ln(_("No changes - recorded it as an empty commit."));
		} ଅନ୍ୟଥା {
			printf_ln(_("No changes - did you forget to use 'git add'?\n"
				    "If there is nothing left to stage, chances are that something else\n"
				    "already introduced the same changes; you might want to skip this patch."));
			die_user_resolve(state);
		}
	}

	ଯଦି (unmerged_index(the_repository->index)) {
		printf_ln(_("You still have unmerged paths in your index.\n"
			"You should 'git add' each file with resolved conflicts to mark them as such.\n"
			"You might run `git rm` on a file to accept \"deleted by them\" for it."));
		die_user_resolve(state);
	}

	ଯଦି (state->interactive) {
		write_index_patch(state);
		ଯଦି (do_interactive(state))
			ଯାଅ next;
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fast_forward_to(ସଂରଚନା ବୃକ୍ଷ *head, ସଂରଚନା ବୃକ୍ଷ *remote, ପୂର୍ଣ୍ଣସଂଖ୍ୟା reset)
{
	ସଂରଚନା lock_file lock_file = LOCK_INIT;
	ସଂରଚନା unpack_trees_options opts;
	ସଂରଚନା ବୃକ୍ଷ_desc t[2];

	ଯଦି (repo_parse_ବୃକ୍ଷ(the_repository, head) || repo_parse_ବୃକ୍ଷ(the_repository, remote))
		ଫେରନ୍ତୁ -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	refresh_index(the_repository->index, REFRESH_QUIET, ଖାଲି, ଖାଲି, ଖାଲି);

	memset(&opts, 0, ଆକାର(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.update = 1;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = twoway_merge;
	init_ବୃକ୍ଷ_desc(&t[0], &head->object.oid, head->buffer, head->size);
	init_ବୃକ୍ଷ_desc(&t[1], &remote->object.oid, remote->buffer, remote->size);

	ଯଦି (unpack_trees(2, t, &opts)) {
		rollback_lock_file(&lock_file);
		ଫେରନ୍ତୁ -1;
	}

	ଯଦି (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	ଫେରନ୍ତୁ 0;
}

/**
 * Merges a tree into the index. The index's stat info will take precedence
 * over the merged tree's. Returns 0 on success, -1 on failure.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା merge_ବୃକ୍ଷ(ସଂରଚନା ବୃକ୍ଷ *ବୃକ୍ଷ)
{
	ସଂରଚନା lock_file lock_file = LOCK_INIT;
	ସଂରଚନା unpack_trees_options opts;
	ସଂରଚନା ବୃକ୍ଷ_desc t[1];

	ଯଦି (repo_parse_ବୃକ୍ଷ(the_repository, ବୃକ୍ଷ))
		ଫେରନ୍ତୁ -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, ଆକାର(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.merge = 1;
	opts.fn = oneway_merge;
	init_ବୃକ୍ଷ_desc(&t[0], &ବୃକ୍ଷ->object.oid, ବୃକ୍ଷ->buffer, ବୃକ୍ଷ->size);

	ଯଦି (unpack_trees(1, t, &opts)) {
		rollback_lock_file(&lock_file);
		ଫେରନ୍ତୁ -1;
	}

	ଯଦି (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	ଫେରନ୍ତୁ 0;
}

/**
 * Clean the index without touching entries that are not modified between
 * `head` and `remote`.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା clean_index(ଅଚଳ ସଂରଚନା object_id *head, ଅଚଳ ସଂରଚନା object_id *remote)
{
	ସଂରଚନା ବୃକ୍ଷ *head_ବୃକ୍ଷ, *remote_ବୃକ୍ଷ, *index_ବୃକ୍ଷ;
	ସଂରଚନା object_id index;

	head_ବୃକ୍ଷ = repo_parse_ବୃକ୍ଷ_indirect(the_repository, head);
	ଯଦି (!head_ବୃକ୍ଷ)
		ଫେରନ୍ତୁ error(_("Could not parse object '%s'."), oid_to_hex(head));

	remote_ବୃକ୍ଷ = repo_parse_ବୃକ୍ଷ_indirect(the_repository, remote);
	ଯଦି (!remote_ବୃକ୍ଷ)
		ଫେରନ୍ତୁ error(_("Could not parse object '%s'."), oid_to_hex(remote));

	repo_read_index_unmerged(the_repository);

	ଯଦି (fast_forward_to(head_ବୃକ୍ଷ, head_ବୃକ୍ଷ, 1))
		ଫେରନ୍ତୁ -1;

	ଯଦି (write_index_as_ବୃକ୍ଷ(&index, the_repository->index,
				repo_get_index_file(the_repository),
				0, ଖାଲି))
		ଫେରନ୍ତୁ -1;

	index_ବୃକ୍ଷ = repo_parse_ବୃକ୍ଷ_indirect(the_repository, &index);
	ଯଦି (!index_ବୃକ୍ଷ)
		ଫେରନ୍ତୁ error(_("Could not parse object '%s'."), oid_to_hex(&index));

	ଯଦି (fast_forward_to(index_ବୃକ୍ଷ, remote_ବୃକ୍ଷ, 0))
		ଫେରନ୍ତୁ -1;

	ଯଦି (merge_ବୃକ୍ଷ(remote_ବୃକ୍ଷ))
		ଫେରନ୍ତୁ -1;

	remove_branch_state(the_repository, 0);

	ଫେରନ୍ତୁ 0;
}

/**
 * Resets rerere's merge resolution metadata.
 */
ସ୍ଥିର ଶୂନ୍ୟ am_rerere_clear(ଶୂନ୍ୟ)
{
	ସଂରଚନା string_list merge_rr = STRING_LIST_INIT_DUP;
	rerere_clear(the_repository, &merge_rr);
	string_list_clear(&merge_rr, 1);
}

/**
 * Resume the current am session by skipping the current patch.
 */
ସ୍ଥିର ଶୂନ୍ୟ am_skip(ସଂରଚନା am_state *state)
{
	ସଂରଚନା object_id head;

	am_rerere_clear();

	ଯଦି (repo_get_oid(the_repository, "HEAD", &head))
		oidcpy(&head, the_hash_algo->empty_ବୃକ୍ଷ);

	ଯଦି (clean_index(&head, &head))
		die(_("failed to clean index"));

	ଯଦି (state->rebasing) {
		FILE *fp = xfopen(am_path(state, "rewritten"), "a");

		ପ୍ରତିଜ୍ଞା(!is_null_oid(&state->orig_commit));
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା safe_to_abort(ଅଚଳ ସଂରଚନା am_state *state)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ସଂରଚନା object_id abort_safety, head;

	ଯଦି (file_exists(am_path(state, "dirtyindex")))
		ଫେରନ୍ତୁ 0;

	ଯଦି (read_state_file(&sb, state, "abort-safety", 1) > 0) {
		ଯଦି (get_oid_hex(sb.buf, &abort_safety))
			die(_("could not parse %s"), am_path(state, "abort-safety"));
	} ଅନ୍ୟଥା
		oidclr(&abort_safety, the_repository->hash_algo);
	strbuf_release(&sb);

	ଯଦି (repo_get_oid(the_repository, "HEAD", &head))
		oidclr(&head, the_repository->hash_algo);

	ଯଦି (oideq(&head, &abort_safety))
		ଫେରନ୍ତୁ 1;

	warning(_("You seem to have moved HEAD since the last 'am' failure.\n"
		"Not rewinding to ORIG_HEAD"));

	ଫେରନ୍ତୁ 0;
}

/**
 * Aborts the current am session if it is safe to do so.
 */
ସ୍ଥିର ଶୂନ୍ୟ am_abort(ସଂରଚନା am_state *state)
{
	ସଂରଚନା object_id curr_head, orig_head;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_curr_head, has_orig_head;
	ଅକ୍ଷର *curr_branch;

	ଯଦି (!safe_to_abort(state)) {
		am_destroy(state);
		ଫେରନ୍ତୁ;
	}

	am_rerere_clear();

	curr_branch = refs_resolve_refdup(get_main_ref_store(the_repository),
					  "HEAD", 0, &curr_head, ଖାଲି);
	has_curr_head = curr_branch && !is_null_oid(&curr_head);
	ଯଦି (!has_curr_head)
		oidcpy(&curr_head, the_hash_algo->empty_ବୃକ୍ଷ);

	has_orig_head = !repo_get_oid(the_repository, "ORIG_HEAD", &orig_head);
	ଯଦି (!has_orig_head)
		oidcpy(&orig_head, the_hash_algo->empty_ବୃକ୍ଷ);

	ଯଦି (clean_index(&curr_head, &orig_head))
		die(_("failed to clean index"));

	ଯଦି (has_orig_head)
		refs_update_ref(get_main_ref_store(the_repository),
				"am --abort", "HEAD", &orig_head,
				has_curr_head ? &curr_head : ଖାଲି, 0,
				UPDATE_REFS_DIE_ON_ERR);
	ଅନ୍ୟଥା ଯଦି (curr_branch)
		refs_delete_ref(get_main_ref_store(the_repository), ଖାଲି,
				curr_branch, ଖାଲି, REF_NO_DEREF);

	ମୁକ୍ତ(curr_branch);
	am_destroy(state);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_patch(ସଂରଚନା am_state *state, ଗଣନା resume_type resume_mode)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *patch_path;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;

	ଯଦି (!is_null_oid(&state->orig_commit)) {
		ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;

		strvec_pushl(&cmd.args, "show", oid_to_hex(&state->orig_commit),
			     "--", ଖାଲି);
		cmd.git_cmd = 1;
		ଫେରନ୍ତୁ run_command(&cmd);
	}

	ବାଛ (resume_mode) {
	ସ୍ଥିତି RESUME_SHOW_PATCH_RAW:
		patch_path = am_path(state, msgnum(state));
		ଭାଙ୍ଗ;
	ସ୍ଥିତି RESUME_SHOW_PATCH_DIFF:
		patch_path = am_path(state, "patch");
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("invalid mode for --show-current-patch");
	}

	len = strbuf_read_file(&sb, patch_path, 0);
	ଯଦି (len < 0)
		die_errno(_("failed to read '%s'"), patch_path);

	setup_pager(the_repository);
	write_in_full(1, sb.buf, sb.len);
	strbuf_release(&sb);
	ଫେରନ୍ତୁ 0;
}

/**
 * parse_options() callback that validates and sets opt->value to the
 * PATCH_FORMAT_* enum value corresponding to `arg`.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_opt_patchformat(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *opt_ମୂଲ୍ୟ = opt->ମୂଲ୍ୟ;

	ଯଦି (unset)
		*opt_ମୂଲ୍ୟ = PATCH_FORMAT_UNKNOWN;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "mbox"))
		*opt_ମୂଲ୍ୟ = PATCH_FORMAT_MBOX;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "stgit"))
		*opt_ମୂଲ୍ୟ = PATCH_FORMAT_STGIT;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "stgit-series"))
		*opt_ମୂଲ୍ୟ = PATCH_FORMAT_STGIT_SERIES;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "hg"))
		*opt_ମୂଲ୍ୟ = PATCH_FORMAT_HG;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "mboxrd"))
		*opt_ମୂଲ୍ୟ = PATCH_FORMAT_MBOXRD;
	/*
	 * Please update $__git_patchformat in git-completion.bash
	 * when you add new options
	 */
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ error(_("invalid value for '%s': '%s'"),
			     "--patch-format", arg);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_opt_show_current_patch(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *opt_ମୂଲ୍ୟ = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG(unset);

	ଯଦି (!arg)
		*opt_ମୂଲ୍ୟ = opt->defval;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "raw"))
		*opt_ମୂଲ୍ୟ = RESUME_SHOW_PATCH_RAW;
	ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "diff"))
		*opt_ମୂଲ୍ୟ = RESUME_SHOW_PATCH_DIFF;
	/*
	 * Please update $__git_showcurrentpatch in git-completion.bash
	 * when you add new options
	 */
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ error(_("invalid value for '%s': '%s'"),
			     "--show-current-patch", arg);
	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_am(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	   ଅଚଳ ଅକ୍ଷର **argv,
	   ଅଚଳ ଅକ୍ଷର *prefix,
	   ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା am_state state;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଦ୍ୱିମିକ = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା keep_cr = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା patch_format = PATCH_FORMAT_UNKNOWN;
	ଗଣନା resume_type resume_mode = RESUME_FALSE;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା in_progress;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	ଅଚଳ ଅକ୍ଷର * ଅଚଳ usage[] = {
		N_("git am [<options>] [(<mbox> | <Maildir>)...]"),
		N_("git am [<options>] (--continue | --skip | --abort)"),
		ଖାଲି
	};

	ସଂରଚନା option options[] = {
		OPT_BOOL('i', "interactive", &state.interactive,
			N_("run interactively")),
		OPT_BOOL('n', "no-verify", &state.no_verify,
			N_("bypass pre-applypatch and applypatch-msg hooks")),
		OPT_HIDDEN_BOOL('b', "binary", &ଦ୍ୱିମିକ,
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
		OPT_PASSTHRU_ARGV(0, "ignore-space-change", &state.git_apply_opts, ଖାଲି,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU_ARGV(0, "ignore-whitespace", &state.git_apply_opts, ଖାଲି,
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
		OPT_PASSTHRU_ARGV('C', ଖାଲି, &state.git_apply_opts, N_("n"),
			N_("pass it through git-apply"),
			0),
		OPT_PASSTHRU_ARGV('p', ଖାଲି, &state.git_apply_opts, N_("num"),
			N_("pass it through git-apply"),
			0),
		OPT_CALLBACK(0, "patch-format", &patch_format, N_("format"),
			N_("format the patch(es) are in"),
			parse_opt_patchformat),
		OPT_PASSTHRU_ARGV(0, "reject", &state.git_apply_opts, ଖାଲି,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_STRING(0, "resolvemsg", &state.resolvemsg, ଖାଲି,
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
			.ମୂଲ୍ୟ = &resume_mode,
			.precision = ଆକାର(resume_mode),
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
			.ମୂଲ୍ୟ = &state.sign_commit,
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

	repo_config(the_repository, git_default_config, ଖାଲି);

	am_state_init(&state);

	in_progress = am_in_progress(&state);
	ଯଦି (in_progress)
		am_load(&state);

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	ଯଦି (ଦ୍ୱିମିକ >= 0)
		fprintf_ln(stderr, _("The -b/--binary option has been a no-op for long time, and\n"
				"it will be removed. Please do not use it anymore."));

	/* Ensure a valid committer ident can be constructed */
	git_committer_info(IDENT_STRICT);

	ଯଦି (repo_read_index_preload(the_repository, ଖାଲି, 0) < 0)
		die(_("failed to read the index"));

	ଯଦି (in_progress) {
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
		ଯଦି (argc || (resume_mode == RESUME_FALSE && !isatty(0)))
			die(_("previous rebase directory %s still exists but mbox given."),
				state.dir);

		ଯଦି (resume_mode == RESUME_FALSE)
			resume_mode = RESUME_APPLY;

		ଯଦି (state.signoff == SIGNOFF_EXPLICIT)
			am_append_signoff(&state);
	} ଅନ୍ୟଥା {
		ସଂରଚନା strvec paths = STRVEC_INIT;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

		/*
		 * Handle stray state directory in the independent-run case. In
		 * the --rebasing case, it is up to the caller to take care of
		 * stray directories.
		 */
		ଯଦି (file_exists(state.dir) && !state.rebasing) {
			ଯଦି (resume_mode == RESUME_ABORT || resume_mode == RESUME_QUIT) {
				am_destroy(&state);
				am_state_release(&state);
				ଫେରନ୍ତୁ 0;
			}

			die(_("Stray %s directory found.\n"
				"Use \"git am --abort\" to remove it."),
				state.dir);
		}

		ଯଦି (resume_mode)
			die(_("Resolve operation not in progress, we are not resuming."));

		ପାଇଁ (i = 0; i < argc; i++) {
			ଯଦି (is_absolute_path(argv[i]) || !prefix)
				strvec_ଠେଲ(&paths, argv[i]);
			ଅନ୍ୟଥା
				strvec_ଠେଲ(&paths, mkpath("%s/%s", prefix, argv[i]));
		}

		ଯଦି (state.interactive && !paths.nr)
			die(_("interactive mode requires patches on the command line"));

		am_setup(&state, patch_format, paths.v, keep_cr);

		strvec_clear(&paths);
	}

	ବାଛ (resume_mode) {
	ସ୍ଥିତି RESUME_FALSE:
		am_run(&state, 0);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି RESUME_APPLY:
		am_run(&state, 1);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି RESUME_RESOLVED:
	ସ୍ଥିତି RESUME_ALLOW_EMPTY:
		am_resolve(&state, resume_mode == RESUME_ALLOW_EMPTY ? 1 : 0);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି RESUME_SKIP:
		am_skip(&state);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି RESUME_ABORT:
		am_abort(&state);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି RESUME_QUIT:
		am_rerere_clear();
		am_destroy(&state);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି RESUME_SHOW_PATCH_RAW:
	ସ୍ଥିତି RESUME_SHOW_PATCH_DIFF:
		ret = show_patch(&state, resume_mode);
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("invalid resume value");
	}

	am_state_release(&state);

	ଫେରନ୍ତୁ ret;
}
