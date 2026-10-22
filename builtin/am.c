/*
 * Builtin "git am"
 *
 * Based on git-am.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"
#include "abspath.h"
#include "advice.h"
#include "config.h"
#include "editor.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "parse-options.h"
#include "dir.h"
#include "run-command.h"
#include "hook.h"
#include "quote.h"
#include "tempfile.h"
#include "lockfile.h"
#include "cache-tree.h"
#include "refs.h"
#include "commit.h"
#include "diff.h"
#include "unpack-trees.h"
#include "branch.h"
#include "object-name.h"
#include "preload-index.h"
#include "sequencer.h"
#include "revision.h"
#include "merge-ort-wrappers.h"
#include "log-tree.h"
#include "notes-utils.h"
#include "rerere.h"
#include "mailinfo.h"
#include "apply.h"
#include "string-list.h"
#include "pager.h"
#include "path.h"
#include "pretty.h"

/**
 * Returns the length of the first line of msg.
 */
static ចំនួនគត់ linelen(const var_na *msg)
{
	ត្រឡប់ strchrnul(msg, '\n') - msg;
}

/**
 * Returns true if `str` consists of only whitespace, false otherwise.
 */
static ចំនួនគត់ str_isspace(const var_na *str)
{
	សម្រាប់ (; *str; str++)
		ប្រសិនបើ (!isspace(*str))
			ត្រឡប់ 0;

	ត្រឡប់ 1;
}

ga_nanaa patch_format {
	PATCH_FORMAT_UNKNOWN = 0,
	PATCH_FORMAT_MBOX,
	PATCH_FORMAT_STGIT,
	PATCH_FORMAT_STGIT_SERIES,
	PATCH_FORMAT_HG,
	PATCH_FORMAT_MBOXRD
};

ga_nanaa keep_type {
	KEEP_FALSE = 0,
	KEEP_TRUE,      /* pass -k flag to git-mailinfo */
	KEEP_NON_PATCH  /* pass -b flag to git-mailinfo */
};

ga_nanaa scissors_type {
	SCISSORS_UNSET = -1,
	SCISSORS_FALSE = 0,  /* pass --no-scissors to git-mailinfo */
	SCISSORS_TRUE        /* pass --scissors to git-mailinfo */
};

ga_nanaa signoff_type {
	SIGNOFF_FALSE = 0,
	SIGNOFF_TRUE = 1,
	SIGNOFF_EXPLICIT /* --signoff was set on the command-line */
};

ga_nanaa resume_type {
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

ga_nanaa empty_action {
	STOP_ON_EMPTY_COMMIT = 0,  /* output errors and stop in the middle of an am session */
	DROP_EMPTY_COMMIT,         /* skip with a notice message, unless "--quiet" has been passed */
	KEEP_EMPTY_COMMIT,         /* keep recording as empty commits */
};

sa_nracanaa am_state {
	/* state directory path */
	var_na *dir;

	/* current and last patch numbers, 1-indexed */
	ចំនួនគត់ cur;
	ចំនួនគត់ last;

	/* commit metadata and message */
	var_na *author_name;
	var_na *author_email;
	var_na *author_date;
	var_na *msg;
	size_t msg_len;

	/* when --rebasing, records the original commit the patch came from */
	sa_nracanaa object_id orig_commit;

	/* number of digits in patch filename */
	ចំនួនគត់ prec;

	/* various operating modes and command line options */
	ចំនួនគត់ interactive;
	ចំនួនគត់ no_verify;
	ចំនួនគត់ threeway;
	ចំនួនគត់ quiet;
	ចំនួនគត់ signoff; /* enum signoff_type */
	ចំនួនគត់ utf8;
	ចំនួនគត់ keep; /* enum keep_type */
	ចំនួនគត់ message_id;
	ចំនួនគត់ scissors; /* enum scissors_type */
	ចំនួនគត់ quoted_cr; /* enum quoted_cr_action */
	ចំនួនគត់ empty_type; /* enum empty_action */
	sa_nracanaa strvec git_apply_opts;
	const var_na *resolvemsg;
	ចំនួនគត់ committer_date_is_author_date;
	ចំនួនគត់ ignore_date;
	ចំនួនគត់ allow_rerere_autoupdate;
	const var_na *sign_commit;
	ចំនួនគត់ rebasing;
};

/**
 * Initializes am_state with the default values.
 */
static void am_state_init(sa_nracanaa am_state *state)
{
	ចំនួនគត់ gpgsign;

	memset(state, 0, parimaa_na(*state));

	state->dir = repo_git_path(the_repository, "rebase-apply");

	state->prec = 4;

	repo_config_get_bool(the_repository, "am.threeway", &state->threeway);

	state->utf8 = 1;

	repo_config_get_bool(the_repository, "am.messageid", &state->message_id);

	state->scissors = SCISSORS_UNSET;
	state->quoted_cr = quoted_cr_unset;

	strvec_init(&state->git_apply_opts);

	ប្រសិនបើ (!repo_config_get_bool(the_repository, "commit.gpgsign", &gpgsign))
		state->sign_commit = gpgsign ? "" : NULL;
}

/**
 * Releases memory allocated by an am_state.
 */
static void am_state_release(sa_nracanaa am_state *state)
{
	free(state->dir);
	free(state->author_name);
	free(state->author_email);
	free(state->author_date);
	free(state->msg);
	strvec_clear(&state->git_apply_opts);
}

static ចំនួនគត់ am_option_parse_quoted_cr(const sa_nracanaa option *opt,
				     const var_na *arg, ចំនួនគត់ unset)
{
	BUG_ON_OPT_NEG(unset);

	ប្រសិនបើ (mailinfo_parse_quoted_cr_action(arg, opt->value) != 0)
		ត្រឡប់ error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	ត្រឡប់ 0;
}

static ចំនួនគត់ am_option_parse_empty(const sa_nracanaa option *opt,
				     const var_na *arg, ចំនួនគត់ unset)
{
	ចំនួនគត់ *opt_value = opt->value;

	BUG_ON_OPT_NEG(unset);

	ប្រសិនបើ (!strcmp(arg, "stop"))
		*opt_value = STOP_ON_EMPTY_COMMIT;
	បើមិនដូច្នេះ ប្រសិនបើ (!strcmp(arg, "drop"))
		*opt_value = DROP_EMPTY_COMMIT;
	បើមិនដូច្នេះ ប្រសិនបើ (!strcmp(arg, "keep"))
		*opt_value = KEEP_EMPTY_COMMIT;
	បើមិនដូច្នេះ
		ត្រឡប់ error(_("invalid value for '%s': '%s'"), "--empty", arg);

	ត្រឡប់ 0;
}

/**
 * Returns path relative to the am_state directory.
 */
static anta_hstha const var_na *am_path(const sa_nracanaa am_state *state, const var_na *path)
{
	ត្រឡប់ mkpath("%s/%s", state->dir, path);
}

/**
 * For convenience to call write_file()
 */
static void write_state_text(const sa_nracanaa am_state *state,
			     const var_na *name, const var_na *string)
{
	write_file(am_path(state, name), "%s", string);
}

static void write_state_count(const sa_nracanaa am_state *state,
			      const var_na *name, ចំនួនគត់ value)
{
	write_file(am_path(state, name), "%d", value);
}

static void write_state_bool(const sa_nracanaa am_state *state,
			     const var_na *name, ចំនួនគត់ value)
{
	write_state_text(state, name, value ? "t" : "f");
}

/**
 * If state->quiet is false, calls fprintf(fp, fmt, ...), and appends a newline
 * at the end.
 */
__attribute__((format (printf, 3, 4)))
static void say(const sa_nracanaa am_state *state, FILE *fp, const var_na *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	ប្រសិនបើ (!state->quiet) {
		vfprintf(fp, fmt, ap);
		putc('\n', fp);
	}
	va_end(ap);
}

/**
 * Returns 1 if there is an am session in progress, 0 otherwise.
 */
static ចំនួនគត់ am_in_progress(const sa_nracanaa am_state *state)
{
	sa_nracanaa stat st;

	ប្រសិនបើ (lstat(state->dir, &st) < 0 || !S_ISDIR(st.st_mode))
		ត្រឡប់ 0;
	ប្រសិនបើ (lstat(am_path(state, "last"), &st) || !S_ISREG(st.st_mode))
		ត្រឡប់ 0;
	ប្រសិនបើ (lstat(am_path(state, "next"), &st) || !S_ISREG(st.st_mode))
		ត្រឡប់ 0;
	ត្រឡប់ 1;
}

/**
 * Reads the contents of `file` in the `state` directory into `sb`. Returns the
 * number of bytes read on success, -1 if the file does not exist. If `trim` is
 * set, trailing whitespace will be removed.
 */
static ចំនួនគត់ read_state_file(sa_nracanaa strbuf *sb, const sa_nracanaa am_state *state,
			const var_na *file, ចំនួនគត់ trim)
{
	strbuf_reset(sb);

	ប្រសិនបើ (strbuf_read_file(sb, am_path(state, file), 0) >= 0) {
		ប្រសិនបើ (trim)
			strbuf_trim(sb);

		ត្រឡប់ sb->len;
	}

	ប្រសិនបើ (errno == ENOENT)
		ត្រឡប់ -1;

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
static ចំនួនគត់ read_am_author_script(sa_nracanaa am_state *state)
{
	const var_na *filename = am_path(state, "author-script");

	assert(!state->author_name);
	assert(!state->author_email);
	assert(!state->author_date);

	ត្រឡប់ read_author_script(filename, &state->author_name,
				  &state->author_email, &state->author_date, 1);
}

/**
 * Saves state->author_name, state->author_email and state->author_date in the
 * state directory's "author-script" file.
 */
static void write_author_script(const sa_nracanaa am_state *state)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;

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
static ចំនួនគត់ read_commit_msg(sa_nracanaa am_state *state)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;

	assert(!state->msg);

	ប្រសិនបើ (read_state_file(&sb, state, "final-commit", 0) < 0) {
		strbuf_release(&sb);
		ត្រឡប់ -1;
	}

	state->msg = strbuf_detach(&sb, &state->msg_len);
	ត្រឡប់ 0;
}

/**
 * Saves state->msg in the state directory's "final-commit" file.
 */
static void write_commit_msg(const sa_nracanaa am_state *state)
{
	const var_na *filename = am_path(state, "final-commit");
	write_file_buf(filename, state->msg, state->msg_len);
}

/**
 * Loads state from disk.
 */
static void am_load(sa_nracanaa am_state *state)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;

	ប្រសិនបើ (read_state_file(&sb, state, "next", 1) < 0)
		BUG("state file 'next' does not exist");
	state->cur = strtol(sb.buf, NULL, 10);

	ប្រសិនបើ (read_state_file(&sb, state, "last", 1) < 0)
		BUG("state file 'last' does not exist");
	state->last = strtol(sb.buf, NULL, 10);

	ប្រសិនបើ (read_am_author_script(state) < 0)
		die(_("could not parse author script"));

	read_commit_msg(state);

	ប្រសិនបើ (read_state_file(&sb, state, "original-commit", 1) < 0)
		oidclr(&state->orig_commit, the_repository->hash_algo);
	បើមិនដូច្នេះ ប្រសិនបើ (get_oid_hex(sb.buf, &state->orig_commit) < 0)
		die(_("could not parse %s"), am_path(state, "original-commit"));

	read_state_file(&sb, state, "threeway", 1);
	state->threeway = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "quiet", 1);
	state->quiet = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "sign", 1);
	state->signoff = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "utf8", 1);
	state->utf8 = !strcmp(sb.buf, "t");

	ប្រសិនបើ (file_exists(am_path(state, "rerere-autoupdate"))) {
		read_state_file(&sb, state, "rerere-autoupdate", 1);
		state->allow_rerere_autoupdate = strcmp(sb.buf, "t") ?
			RERERE_NOAUTOUPDATE : RERERE_AUTOUPDATE;
	} បើមិនដូច្នេះ {
		state->allow_rerere_autoupdate = 0;
	}

	read_state_file(&sb, state, "keep", 1);
	ប្រសិនបើ (!strcmp(sb.buf, "t"))
		state->keep = KEEP_TRUE;
	បើមិនដូច្នេះ ប្រសិនបើ (!strcmp(sb.buf, "b"))
		state->keep = KEEP_NON_PATCH;
	បើមិនដូច្នេះ
		state->keep = KEEP_FALSE;

	read_state_file(&sb, state, "messageid", 1);
	state->message_id = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "scissors", 1);
	ប្រសិនបើ (!strcmp(sb.buf, "t"))
		state->scissors = SCISSORS_TRUE;
	បើមិនដូច្នេះ ប្រសិនបើ (!strcmp(sb.buf, "f"))
		state->scissors = SCISSORS_FALSE;
	បើមិនដូច្នេះ
		state->scissors = SCISSORS_UNSET;

	read_state_file(&sb, state, "quoted-cr", 1);
	ប្រសិនបើ (!*sb.buf)
		state->quoted_cr = quoted_cr_unset;
	បើមិនដូច្នេះ ប្រសិនបើ (mailinfo_parse_quoted_cr_action(sb.buf, &state->quoted_cr) != 0)
		die(_("could not parse %s"), am_path(state, "quoted-cr"));

	read_state_file(&sb, state, "apply-opt", 1);
	strvec_clear(&state->git_apply_opts);
	ប្រសិនបើ (sq_dequote_to_strvec(sb.buf, &state->git_apply_opts) < 0)
		die(_("could not parse %s"), am_path(state, "apply-opt"));

	state->rebasing = !!file_exists(am_path(state, "rebasing"));

	strbuf_release(&sb);
}

/**
 * Removes the am_state directory, forcefully terminating the current am
 * session.
 */
static void am_destroy(const sa_nracanaa am_state *state)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;

	strbuf_addstr(&sb, state->dir);
	remove_dir_recursively(&sb, 0);
	strbuf_release(&sb);
}

/**
 * Runs applypatch-msg hook. Returns its exit code.
 */
static ចំនួនគត់ run_applypatch_msg_hook(sa_nracanaa am_state *state)
{
	ចំនួនគត់ ret = 0;

	assert(state->msg);

	ប្រសិនបើ (!state->no_verify) {
		sa_nracanaa run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;
		strvec_push(&opt.args, am_path(state, "final-commit"));
		ret = run_hooks_opt(the_repository, "applypatch-msg", &opt);
	}

	ប្រសិនបើ (!ret) {
		FREE_AND_NULL(state->msg);
		ប្រសិនបើ (read_commit_msg(state) < 0)
			die(_("'%s' was deleted by the applypatch-msg hook"),
				am_path(state, "final-commit"));
	}

	ត្រឡប់ ret;
}

/**
 * Runs post-rewrite hook. Returns it exit code.
 */
static ចំនួនគត់ run_post_rewrite_hook(const sa_nracanaa am_state *state)
{
	sa_nracanaa run_hooks_opt opt = RUN_HOOKS_OPT_INIT;

	strvec_push(&opt.args, "rebase");
	opt.path_to_stdin = am_path(state, "rewritten");

	ត្រឡប់ run_hooks_opt(the_repository, "post-rewrite", &opt);
}

/**
 * Reads the state directory's "rewritten" file, and copies notes from the old
 * commits listed in the file to their rewritten commits.
 *
 * Returns 0 on success, -1 on failure.
 */
static ចំនួនគត់ copy_notes_for_rebase(const sa_nracanaa am_state *state)
{
	sa_nracanaa notes_rewrite_cfg *c;
	sa_nracanaa strbuf sb = STRBUF_INIT;
	const var_na *invalid_line = _("Malformed input line: '%s'.");
	const var_na *msg = "Notes added by 'git rebase'";
	FILE *fp;
	ចំនួនគត់ ret = 0;

	assert(state->rebasing);

	c = init_copy_notes_for_rewrite("rebase");
	ប្រសិនបើ (!c)
		ត្រឡប់ 0;

	fp = xfopen(am_path(state, "rewritten"), "r");

	ខណៈពេល (!strbuf_getline_lf(&sb, fp)) {
		sa_nracanaa object_id from_obj, to_obj;
		const var_na *p;

		ប្រសិនបើ (sb.len != the_hash_algo->hexsz * 2 + 1) {
			ret = error(invalid_line, sb.buf);
			goto finish;
		}

		ប្រសិនបើ (parse_oid_hex(sb.buf, &from_obj, &p)) {
			ret = error(invalid_line, sb.buf);
			goto finish;
		}

		ប្រសិនបើ (*p != ' ') {
			ret = error(invalid_line, sb.buf);
			goto finish;
		}

		ប្រសិនបើ (get_oid_hex(p + 1, &to_obj)) {
			ret = error(invalid_line, sb.buf);
			goto finish;
		}

		ប្រសិនបើ (copy_note_for_rewrite(c, &from_obj, &to_obj))
			ret = error(_("Failed to copy notes from '%s' to '%s'"),
					oid_to_hex(&from_obj), oid_to_hex(&to_obj));
	}

finish:
	finish_copy_notes_for_rewrite(the_repository, c, msg);
	fclose(fp);
	strbuf_release(&sb);
	ត្រឡប់ ret;
}

/**
 * Determines if the file looks like a piece of RFC2822 mail by grabbing all
 * non-indented lines and checking if they look like they begin with valid
 * header field names.
 *
 * Returns 1 if the file looks like a piece of mail, 0 otherwise.
 */
static ចំនួនគត់ is_mail(FILE *fp)
{
	const var_na *header_regex = "^[!-9;-~]+:";
	sa_nracanaa strbuf sb = STRBUF_INIT;
	regex_t regex;
	ចំនួនគត់ ret = 1;

	ប្រសិនបើ (fseek(fp, 0L, SEEK_SET))
		die_errno(_("fseek failed"));

	ប្រសិនបើ (regcomp(&regex, header_regex, REG_NOSUB | REG_EXTENDED))
		die("invalid pattern: %s", header_regex);

	ខណៈពេល (!strbuf_getline(&sb, fp)) {
		ប្រសិនបើ (!sb.len)
			bha_nga; /* End of header */

		/* Ignore indented folded lines */
		ប្រសិនបើ (*sb.buf == '\t' || *sb.buf == ' ')
			continue;

		/* It's a header if it matches header_regex */
		ប្រសិនបើ (regexec(&regex, sb.buf, 0, NULL, 0)) {
			ret = 0;
			goto done;
		}
	}

done:
	regfree(&regex);
	strbuf_release(&sb);
	ត្រឡប់ ret;
}

/**
 * Attempts to detect the patch_format of the patches contained in `paths`,
 * returning the PATCH_FORMAT_* enum value. Returns PATCH_FORMAT_UNKNOWN if
 * detection fails.
 */
static ចំនួនគត់ detect_patch_format(const var_na **paths)
{
	ga_nanaa patch_format ret = PATCH_FORMAT_UNKNOWN;
	sa_nracanaa strbuf l1 = STRBUF_INIT;
	sa_nracanaa strbuf l2 = STRBUF_INIT;
	sa_nracanaa strbuf l3 = STRBUF_INIT;
	FILE *fp;

	/*
	 * We default to mbox format if input is from stdin and for directories
	 */
	ប្រសិនបើ (!*paths || !strcmp(*paths, "-") || is_directory(*paths))
		ត្រឡប់ PATCH_FORMAT_MBOX;

	/*
	 * Otherwise, check the first few lines of the first patch, starting
	 * from the first non-blank line, to try to detect its format.
	 */

	fp = xfopen(*paths, "r");

	ខណៈពេល (!strbuf_getline(&l1, fp)) {
		ប្រសិនបើ (l1.len)
			bha_nga;
	}

	ប្រសិនបើ (starts_with(l1.buf, "From ") || starts_with(l1.buf, "From: ")) {
		ret = PATCH_FORMAT_MBOX;
		goto done;
	}

	ប្រសិនបើ (starts_with(l1.buf, "# This series applies on GIT commit")) {
		ret = PATCH_FORMAT_STGIT_SERIES;
		goto done;
	}

	ប្រសិនបើ (!strcmp(l1.buf, "# HG changeset patch")) {
		ret = PATCH_FORMAT_HG;
		goto done;
	}

	strbuf_getline(&l2, fp);
	strbuf_getline(&l3, fp);

	/*
	 * If the second line is empty and the third is a From, Author or Date
	 * entry, this is likely an StGit patch.
	 */
	ប្រសិនបើ (l1.len && !l2.len &&
		(starts_with(l3.buf, "From:") ||
		 starts_with(l3.buf, "Author:") ||
		 starts_with(l3.buf, "Date:"))) {
		ret = PATCH_FORMAT_STGIT;
		goto done;
	}

	ប្រសិនបើ (l1.len && is_mail(fp)) {
		ret = PATCH_FORMAT_MBOX;
		goto done;
	}

done:
	fclose(fp);
	strbuf_release(&l1);
	strbuf_release(&l2);
	strbuf_release(&l3);
	ត្រឡប់ ret;
}

/**
 * Splits out individual email patches from `paths`, where each path is either
 * a mbox file or a Maildir. Returns 0 on success, -1 on failure.
 */
static ចំនួនគត់ split_mail_mbox(sa_nracanaa am_state *state, const var_na **paths,
				ចំនួនគត់ keep_cr, ចំនួនគត់ mboxrd)
{
	sa_nracanaa child_process cp = CHILD_PROCESS_INIT;
	sa_nracanaa strbuf last = STRBUF_INIT;
	ចំនួនគត់ ret;

	cp.git_cmd = 1;
	strvec_push(&cp.args, "mailsplit");
	strvec_pushf(&cp.args, "-d%d", state->prec);
	strvec_pushf(&cp.args, "-o%s", state->dir);
	strvec_push(&cp.args, "-b");
	ប្រសិនបើ (keep_cr)
		strvec_push(&cp.args, "--keep-cr");
	ប្រសិនបើ (mboxrd)
		strvec_push(&cp.args, "--mboxrd");
	strvec_push(&cp.args, "--");
	strvec_pushv(&cp.args, paths);

	ret = capture_command(&cp, &last, 8);
	ប្រសិនបើ (ret)
		goto exit;

	state->cur = 1;
	state->last = strtol(last.buf, NULL, 10);

exit:
	strbuf_release(&last);
	ត្រឡប់ ret ? -1 : 0;
}

/**
 * Callback signature for split_mail_conv(). The foreign patch should be
 * read from `in`, and the converted patch (in RFC2822 mail format) should be
 * written to `out`. Return 0 on success, or -1 on failure.
 */
typedef ចំនួនគត់ (*mail_conv_fn)(FILE *out, FILE *in, ចំនួនគត់ keep_cr);

/**
 * Calls `fn` for each file in `paths` to convert the foreign patch to the
 * RFC2822 mail format suitable for parsing with git-mailinfo.
 *
 * Returns 0 on success, -1 on failure.
 */
static ចំនួនគត់ split_mail_conv(mail_conv_fn fn, sa_nracanaa am_state *state,
			const var_na **paths, ចំនួនគត់ keep_cr)
{
	static const var_na *stdin_only[] = {"-", NULL};
	ចំនួនគត់ i;

	ប្រសិនបើ (!*paths)
		paths = stdin_only;

	សម្រាប់ (i = 0; *paths; paths++, i++) {
		FILE *in, *out;
		const var_na *mail;
		ចំនួនគត់ ret;

		ប្រសិនបើ (!strcmp(*paths, "-"))
			in = stdin;
		បើមិនដូច្នេះ
			in = fopen(*paths, "r");

		ប្រសិនបើ (!in)
			ត្រឡប់ error_errno(_("could not open '%s' for reading"),
					   *paths);

		mail = mkpath("%s/%0*d", state->dir, state->prec, i + 1);

		out = fopen(mail, "w");
		ប្រសិនបើ (!out) {
			ប្រសិនបើ (in != stdin)
				fclose(in);
			ត្រឡប់ error_errno(_("could not open '%s' for writing"),
					   mail);
		}

		ret = fn(out, in, keep_cr);

		fclose(out);
		ប្រសិនបើ (in != stdin)
			fclose(in);

		ប្រសិនបើ (ret)
			ត្រឡប់ error(_("could not parse patch '%s'"), *paths);
	}

	state->cur = 1;
	state->last = i;
	ត្រឡប់ 0;
}

/**
 * A split_mail_conv() callback that converts an StGit patch to an RFC2822
 * message suitable for parsing with git-mailinfo.
 */
static ចំនួនគត់ stgit_patch_to_mail(FILE *out, FILE *in, ចំនួនគត់ keep_cr UNUSED)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	ចំនួនគត់ subject_printed = 0;

	ខណៈពេល (!strbuf_getline_lf(&sb, in)) {
		const var_na *str;

		ប្រសិនបើ (str_isspace(sb.buf))
			continue;
		បើមិនដូច្នេះ ប្រសិនបើ (skip_prefix(sb.buf, "Author:", &str))
			fprintf(out, "From:%s\n", str);
		បើមិនដូច្នេះ ប្រសិនបើ (starts_with(sb.buf, "From") || starts_with(sb.buf, "Date"))
			fprintf(out, "%s\n", sb.buf);
		បើមិនដូច្នេះ ប្រសិនបើ (!subject_printed) {
			fprintf(out, "Subject: %s\n", sb.buf);
			subject_printed = 1;
		} បើមិនដូច្នេះ {
			fprintf(out, "\n%s\n", sb.buf);
			bha_nga;
		}
	}

	strbuf_reset(&sb);
	ខណៈពេល (strbuf_fread(&sb, 8192, in) > 0) {
		fwrite(sb.buf, 1, sb.len, out);
		strbuf_reset(&sb);
	}

	strbuf_release(&sb);
	ត្រឡប់ 0;
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
static ចំនួនគត់ split_mail_stgit_series(sa_nracanaa am_state *state, const var_na **paths,
					ចំនួនគត់ keep_cr)
{
	const var_na *series_dir;
	var_na *series_dir_buf;
	FILE *fp;
	sa_nracanaa strvec patches = STRVEC_INIT;
	sa_nracanaa strbuf sb = STRBUF_INIT;
	ចំនួនគត់ ret;

	ប្រសិនបើ (!paths[0] || paths[1])
		ត្រឡប់ error(_("Only one StGIT patch series can be applied at once"));

	series_dir_buf = xstrdup(*paths);
	series_dir = dirname(series_dir_buf);

	fp = fopen(*paths, "r");
	ប្រសិនបើ (!fp) {
		free(series_dir_buf);
		ត្រឡប់ error_errno(_("could not open '%s' for reading"), *paths);
	}

	ខណៈពេល (!strbuf_getline_lf(&sb, fp)) {
		ប្រសិនបើ (*sb.buf == '#')
			continue; /* skip comment lines */

		strvec_push(&patches, mkpath("%s/%s", series_dir, sb.buf));
	}

	fclose(fp);
	strbuf_release(&sb);
	free(series_dir_buf);

	ret = split_mail_conv(stgit_patch_to_mail, state, patches.v, keep_cr);

	strvec_clear(&patches);
	ត្រឡប់ ret;
}

/**
 * A split_patches_conv() callback that converts a mercurial patch to a RFC2822
 * message suitable for parsing with git-mailinfo.
 */
static ចំនួនគត់ hg_patch_to_mail(FILE *out, FILE *in, ចំនួនគត់ keep_cr UNUSED)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	ចំនួនគត់ rc = 0;

	ខណៈពេល (!strbuf_getline_lf(&sb, in)) {
		const var_na *str;

		ប្រសិនបើ (skip_prefix(sb.buf, "# User ", &str))
			fprintf(out, "From: %s\n", str);
		បើមិនដូច្នេះ ប្រសិនបើ (skip_prefix(sb.buf, "# Date ", &str)) {
			timestamp_t timestamp;
			long tz, tz2;
			var_na *end;

			errno = 0;
			timestamp = parse_timestamp(str, &end, 10);
			ប្រសិនបើ (errno) {
				rc = error(_("invalid timestamp"));
				goto exit;
			}

			ប្រសិនបើ (!skip_prefix(end, " ", &str)) {
				rc = error(_("invalid Date line"));
				goto exit;
			}

			errno = 0;
			tz = strtol(str, &end, 10);
			ប្រសិនបើ (errno) {
				rc = error(_("invalid timezone offset"));
				goto exit;
			}

			ប្រសិនបើ (*end) {
				rc = error(_("invalid Date line"));
				goto exit;
			}

			/*
			 * mercurial's timezone is in seconds west of UTC,
			 * however git's timezone is in hours + minutes east of
			 * UTC. Convert it.
			 */
			tz2 = labs(tz) / 3600 * 100 + labs(tz) % 3600 / 60;
			ប្រសិនបើ (tz > 0)
				tz2 = -tz2;

			fprintf(out, "Date: %s\n", show_date(timestamp, tz2, DATE_MODE(RFC2822)));
		} បើមិនដូច្នេះ ប្រសិនបើ (starts_with(sb.buf, "# ")) {
			continue;
		} បើមិនដូច្នេះ {
			fprintf(out, "\n%s\n", sb.buf);
			bha_nga;
		}
	}

	strbuf_reset(&sb);
	ខណៈពេល (strbuf_fread(&sb, 8192, in) > 0) {
		fwrite(sb.buf, 1, sb.len, out);
		strbuf_reset(&sb);
	}
exit:
	strbuf_release(&sb);
	ត្រឡប់ rc;
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
static ចំនួនគត់ split_mail(sa_nracanaa am_state *state, ga_nanaa patch_format patch_format,
			const var_na **paths, ចំនួនគត់ keep_cr)
{
	ប្រសិនបើ (keep_cr < 0) {
		keep_cr = 0;
		repo_config_get_bool(the_repository, "am.keepcr", &keep_cr);
	}

	switch (patch_format) {
	pak_sa PATCH_FORMAT_MBOX:
		ត្រឡប់ split_mail_mbox(state, paths, keep_cr, 0);
	pak_sa PATCH_FORMAT_STGIT:
		ត្រឡប់ split_mail_conv(stgit_patch_to_mail, state, paths, keep_cr);
	pak_sa PATCH_FORMAT_STGIT_SERIES:
		ត្រឡប់ split_mail_stgit_series(state, paths, keep_cr);
	pak_sa PATCH_FORMAT_HG:
		ត្រឡប់ split_mail_conv(hg_patch_to_mail, state, paths, keep_cr);
	pak_sa PATCH_FORMAT_MBOXRD:
		ត្រឡប់ split_mail_mbox(state, paths, keep_cr, 1);
	default:
		BUG("invalid patch_format");
	}
	ត្រឡប់ -1;
}

/**
 * Setup a new am session for applying patches
 */
static void am_setup(sa_nracanaa am_state *state, ga_nanaa patch_format patch_format,
			const var_na **paths, ចំនួនគត់ keep_cr)
{
	sa_nracanaa object_id curr_head;
	const var_na *str;
	sa_nracanaa strbuf sb = STRBUF_INIT;

	ប្រសិនបើ (!patch_format)
		patch_format = detect_patch_format(paths);

	ប្រសិនបើ (!patch_format) {
		fprintf_ln(stderr, _("Patch format detection failed."));
		die(NULL);
	}

	ប្រសិនបើ (mkdir(state->dir, 0777) < 0 && errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), state->dir);
	refs_delete_ref(get_main_ref_store(the_repository), NULL,
			"REBASE_HEAD", NULL, REF_NO_DEREF);

	ប្រសិនបើ (split_mail(state, patch_format, paths, keep_cr) < 0) {
		am_destroy(state);
		die(_("Failed to split patches."));
	}

	ប្រសិនបើ (state->rebasing)
		state->threeway = 1;

	write_state_bool(state, "threeway", state->threeway);
	write_state_bool(state, "quiet", state->quiet);
	write_state_bool(state, "sign", state->signoff);
	write_state_bool(state, "utf8", state->utf8);

	ប្រសិនបើ (state->allow_rerere_autoupdate)
		write_state_bool(state, "rerere-autoupdate",
			 state->allow_rerere_autoupdate == RERERE_AUTOUPDATE);

	switch (state->keep) {
	pak_sa KEEP_FALSE:
		str = "f";
		bha_nga;
	pak_sa KEEP_TRUE:
		str = "t";
		bha_nga;
	pak_sa KEEP_NON_PATCH:
		str = "b";
		bha_nga;
	default:
		BUG("invalid value for state->keep");
	}

	write_state_text(state, "keep", str);
	write_state_bool(state, "messageid", state->message_id);

	switch (state->scissors) {
	pak_sa SCISSORS_UNSET:
		str = "";
		bha_nga;
	pak_sa SCISSORS_FALSE:
		str = "f";
		bha_nga;
	pak_sa SCISSORS_TRUE:
		str = "t";
		bha_nga;
	default:
		BUG("invalid value for state->scissors");
	}
	write_state_text(state, "scissors", str);

	switch (state->quoted_cr) {
	pak_sa quoted_cr_unset:
		str = "";
		bha_nga;
	pak_sa quoted_cr_nowarn:
		str = "nowarn";
		bha_nga;
	pak_sa quoted_cr_warn:
		str = "warn";
		bha_nga;
	pak_sa quoted_cr_strip:
		str = "strip";
		bha_nga;
	default:
		BUG("invalid value for state->quoted_cr");
	}
	write_state_text(state, "quoted-cr", str);

	sq_quote_argv(&sb, state->git_apply_opts.v);
	write_state_text(state, "apply-opt", sb.buf);

	ប្រសិនបើ (state->rebasing)
		write_state_text(state, "rebasing", "");
	បើមិនដូច្នេះ
		write_state_text(state, "applying", "");

	ប្រសិនបើ (!repo_get_oid(the_repository, "HEAD", &curr_head)) {
		write_state_text(state, "abort-safety", oid_to_hex(&curr_head));
		ប្រសិនបើ (!state->rebasing)
			refs_update_ref(get_main_ref_store(the_repository),
					"am", "ORIG_HEAD", &curr_head, NULL,
					0,
					UPDATE_REFS_DIE_ON_ERR);
	} បើមិនដូច្នេះ {
		write_state_text(state, "abort-safety", "");
		ប្រសិនបើ (!state->rebasing)
			refs_delete_ref(get_main_ref_store(the_repository),
					NULL, "ORIG_HEAD", NULL, 0);
	}

	/*
	 * NOTE: Since the "next" and "last" files determine if an am_state
	 * session is in progress, they should be written last.
	 */

	write_state_count(state, "next", state->cur);
	write_state_count(state, "last", state->last);

	strbuf_release(&sb);
}

/**
 * Increments the patch pointer, and cleans am_state for the application of the
 * next patch.
 */
static void am_next(sa_nracanaa am_state *state)
{
	sa_nracanaa object_id head;

	FREE_AND_NULL(state->author_name);
	FREE_AND_NULL(state->author_email);
	FREE_AND_NULL(state->author_date);
	FREE_AND_NULL(state->msg);
	state->msg_len = 0;

	unlink(am_path(state, "author-script"));
	unlink(am_path(state, "final-commit"));

	oidclr(&state->orig_commit, the_repository->hash_algo);
	unlink(am_path(state, "original-commit"));
	refs_delete_ref(get_main_ref_store(the_repository), NULL,
			"REBASE_HEAD", NULL, REF_NO_DEREF);

	ប្រសិនបើ (!repo_get_oid(the_repository, "HEAD", &head))
		write_state_text(state, "abort-safety", oid_to_hex(&head));
	បើមិនដូច្នេះ
		write_state_text(state, "abort-safety", "");

	state->cur++;
	write_state_count(state, "next", state->cur);
}

/**
 * Returns the filename of the current patch email.
 */
static const var_na *msgnum(const sa_nracanaa am_state *state)
{
	static sa_nracanaa strbuf sb = STRBUF_INIT;

	strbuf_reset(&sb);
	strbuf_addf(&sb, "%0*d", state->prec, state->cur);

	ត្រឡប់ sb.buf;
}

/**
 * Dies with a user-friendly message on how to proceed after resolving the
 * problem. This message can be overridden with state->resolvemsg.
 */
static void NORETURN die_user_resolve(const sa_nracanaa am_state *state)
{
	ប្រសិនបើ (state->resolvemsg) {
		advise_if_enabled(ADVICE_MERGE_CONFLICT, "%s", state->resolvemsg);
	} បើមិនដូច្នេះ {
		const var_na *cmdline = state->interactive ? "git am -i" : "git am";
		sa_nracanaa strbuf sb = STRBUF_INIT;

		strbuf_addf(&sb, _("When you have resolved this problem, run \"%s --continue\".\n"), cmdline);
		strbuf_addf(&sb, _("If you prefer to skip this patch, run \"%s --skip\" instead.\n"), cmdline);

		ប្រសិនបើ (advice_enabled(ADVICE_AM_WORK_DIR) &&
		    is_empty_or_missing_file(am_path(state, "patch")) &&
		    !repo_index_has_changes(the_repository, NULL, NULL))
			strbuf_addf(&sb, _("To record the empty patch as an empty commit, run \"%s --allow-empty\".\n"), cmdline);

		strbuf_addf(&sb, _("To restore the original branch and stop patching, run \"%s --abort\"."), cmdline);

		advise_if_enabled(ADVICE_MERGE_CONFLICT, "%s", sb.buf);
		strbuf_release(&sb);
	}

	die(NULL);
}

/**
 * Appends signoff to the "msg" field of the am_state.
 */
static void am_append_signoff(sa_nracanaa am_state *state)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;

	strbuf_attach(&sb, state->msg, state->msg_len, state->msg_len + 1);
	append_signoff(&sb, 0, 0);
	state->msg = strbuf_detach(&sb, &state->msg_len);
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
static ចំនួនគត់ parse_mail(sa_nracanaa am_state *state, const var_na *mail)
{
	FILE *fp;
	sa_nracanaa strbuf sb = STRBUF_INIT;
	sa_nracanaa strbuf msg = STRBUF_INIT;
	sa_nracanaa strbuf author_name = STRBUF_INIT;
	sa_nracanaa strbuf author_date = STRBUF_INIT;
	sa_nracanaa strbuf author_email = STRBUF_INIT;
	ចំនួនគត់ ret = 0;
	sa_nracanaa mailinfo mi;

	setup_mailinfo(the_repository, &mi);

	ប្រសិនបើ (state->utf8)
		mi.metainfo_charset = get_commit_output_encoding();
	បើមិនដូច្នេះ
		mi.metainfo_charset = NULL;

	switch (state->keep) {
	pak_sa KEEP_FALSE:
		bha_nga;
	pak_sa KEEP_TRUE:
		mi.keep_subject = 1;
		bha_nga;
	pak_sa KEEP_NON_PATCH:
		mi.keep_non_patch_brackets_in_subject = 1;
		bha_nga;
	default:
		BUG("invalid value for state->keep");
	}

	ប្រសិនបើ (state->message_id)
		mi.add_message_id = 1;

	switch (state->scissors) {
	pak_sa SCISSORS_UNSET:
		bha_nga;
	pak_sa SCISSORS_FALSE:
		mi.use_scissors = 0;
		bha_nga;
	pak_sa SCISSORS_TRUE:
		mi.use_scissors = 1;
		bha_nga;
	default:
		BUG("invalid value for state->scissors");
	}

	switch (state->quoted_cr) {
	pak_sa quoted_cr_unset:
		bha_nga;
	pak_sa quoted_cr_nowarn:
	pak_sa quoted_cr_warn:
	pak_sa quoted_cr_strip:
		mi.quoted_cr = state->quoted_cr;
		bha_nga;
	default:
		BUG("invalid value for state->quoted_cr");
	}

	mi.input = xfopen(mail, "r");
	mi.output = xfopen(am_path(state, "info"), "w");
	ប្រសិនបើ (mailinfo(&mi, am_path(state, "msg"), am_path(state, "patch")))
		die("could not parse patch");

	fclose(mi.input);
	fclose(mi.output);

	ប្រសិនបើ (mi.format_flowed)
		warning(_("Patch sent with format=flowed; "
			  "space at the end of lines might be lost."));

	/* Extract message and author information */
	fp = xfopen(am_path(state, "info"), "r");
	ខណៈពេល (!strbuf_getline_lf(&sb, fp)) {
		const var_na *x;

		ប្រសិនបើ (skip_prefix(sb.buf, "Subject: ", &x)) {
			ប្រសិនបើ (msg.len)
				strbuf_addch(&msg, '\n');
			strbuf_addstr(&msg, x);
		} បើមិនដូច្នេះ ប្រសិនបើ (skip_prefix(sb.buf, "Author: ", &x))
			strbuf_addstr(&author_name, x);
		បើមិនដូច្នេះ ប្រសិនបើ (skip_prefix(sb.buf, "Email: ", &x))
			strbuf_addstr(&author_email, x);
		បើមិនដូច្នេះ ប្រសិនបើ (skip_prefix(sb.buf, "Date: ", &x))
			strbuf_addstr(&author_date, x);
	}
	fclose(fp);

	/* Skip pine's internal folder data */
	ប្រសិនបើ (!strcmp(author_name.buf, "Mail System Internal Data")) {
		ret = 1;
		goto finish;
	}

	strbuf_addstr(&msg, "\n\n");
	strbuf_addbuf(&msg, &mi.log_message);
	strbuf_stripspace(&msg, NULL);

	assert(!state->author_name);
	state->author_name = strbuf_detach(&author_name, NULL);

	assert(!state->author_email);
	state->author_email = strbuf_detach(&author_email, NULL);

	assert(!state->author_date);
	state->author_date = strbuf_detach(&author_date, NULL);

	assert(!state->msg);
	state->msg = strbuf_detach(&msg, &state->msg_len);

finish:
	strbuf_release(&msg);
	strbuf_release(&author_date);
	strbuf_release(&author_email);
	strbuf_release(&author_name);
	strbuf_release(&sb);
	clear_mailinfo(&mi);
	ត្រឡប់ ret;
}

/**
 * Sets commit_id to the commit hash where the mail was generated from.
 * Returns 0 on success, -1 on failure.
 */
static ចំនួនគត់ get_mail_commit_oid(sa_nracanaa object_id *commit_id, const var_na *mail)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	FILE *fp = xfopen(mail, "r");
	const var_na *x;
	ចំនួនគត់ ret = 0;

	ប្រសិនបើ (strbuf_getline_lf(&sb, fp) ||
	    !skip_prefix(sb.buf, "From ", &x) ||
	    get_oid_hex(x, commit_id) < 0)
		ret = -1;

	strbuf_release(&sb);
	fclose(fp);
	ត្រឡប់ ret;
}

/**
 * Sets state->msg, state->author_name, state->author_email, state->author_date
 * to the commit's respective info.
 */
static void get_commit_info(sa_nracanaa am_state *state, sa_nracanaa commit *commit)
{
	const var_na *buffer, *ident_line, *msg;
	size_t ident_len;
	sa_nracanaa ident_split id;

	buffer = repo_logmsg_reencode(the_repository, commit, NULL,
				      get_commit_output_encoding());

	ident_line = find_commit_header(buffer, "author", &ident_len);
	ប្រសិនបើ (!ident_line)
		die(_("missing author line in commit %s"),
		      oid_to_hex(&commit->object.oid));
	ប្រសិនបើ (split_ident_line(&id, ident_line, ident_len) < 0)
		die(_("invalid ident line: %.*s"), (ចំនួនគត់)ident_len, ident_line);

	assert(!state->author_name);
	ប្រសិនបើ (id.name_begin)
		state->author_name =
			xmemdupz(id.name_begin, id.name_end - id.name_begin);
	បើមិនដូច្នេះ
		state->author_name = xstrdup("");

	assert(!state->author_email);
	ប្រសិនបើ (id.mail_begin)
		state->author_email =
			xmemdupz(id.mail_begin, id.mail_end - id.mail_begin);
	បើមិនដូច្នេះ
		state->author_email = xstrdup("");

	assert(!state->author_date);
	state->author_date = xstrdup(show_ident_date(&id, DATE_MODE(NORMAL)));

	assert(!state->msg);
	msg = strstr(buffer, "\n\n");
	ប្រសិនបើ (!msg)
		die(_("unable to parse commit %s"), oid_to_hex(&commit->object.oid));
	state->msg = xstrdup(msg + 2);
	state->msg_len = strlen(state->msg);
	repo_unuse_commit_buffer(the_repository, commit, buffer);
}

/**
 * Writes `commit` as a patch to the state directory's "patch" file.
 */
static void write_commit_patch(const sa_nracanaa am_state *state, sa_nracanaa commit *commit)
{
	sa_nracanaa rev_info rev_info;
	FILE *fp;

	fp = xfopen(am_path(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, NULL);
	rev_info.diff = 1;
	rev_info.abbrev = 0;
	rev_info.disable_stdin = 1;
	rev_info.show_root_diff = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.flags.binary = 1;
	rev_info.diffopt.flags.full_index = 1;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	add_pending_object(&rev_info, &commit->object, "");
	diff_setup_done(&rev_info.diffopt);
	log_tree_commit(&rev_info, commit);
	release_revisions(&rev_info);
}

/**
 * Writes the diff of the index against HEAD as a patch to the state
 * directory's "patch" file.
 */
static void write_index_patch(const sa_nracanaa am_state *state)
{
	sa_nracanaa tree *tree;
	sa_nracanaa object_id head;
	sa_nracanaa rev_info rev_info;
	FILE *fp;

	ប្រសិនបើ (!repo_get_oid(the_repository, "HEAD", &head)) {
		sa_nracanaa commit *commit = lookup_commit_or_die(&head, "HEAD");
		tree = repo_get_commit_tree(the_repository, commit);
	} បើមិនដូច្នេះ
		tree = lookup_tree(the_repository,
				   the_repository->hash_algo->empty_tree);

	fp = xfopen(am_path(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, NULL);
	rev_info.diff = 1;
	rev_info.disable_stdin = 1;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	add_pending_object(&rev_info, &tree->object, "");
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
static ចំនួនគត់ parse_mail_rebase(sa_nracanaa am_state *state, const var_na *mail)
{
	sa_nracanaa commit *commit;
	sa_nracanaa object_id commit_oid;

	ប្រសិនបើ (get_mail_commit_oid(&commit_oid, mail) < 0)
		die(_("could not parse %s"), mail);

	commit = lookup_commit_or_die(&commit_oid, mail);

	get_commit_info(state, commit);

	write_commit_patch(state, commit);

	oidcpy(&state->orig_commit, &commit_oid);
	write_state_text(state, "original-commit", oid_to_hex(&commit_oid));
	refs_update_ref(get_main_ref_store(the_repository), "am",
			"REBASE_HEAD", &commit_oid,
			NULL, REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);

	ត្រឡប់ 0;
}

/**
 * Applies current patch with git-apply. Returns 0 on success, -1 otherwise. If
 * `index_file` is not NULL, the patch will be applied to that index.
 */
static ចំនួនគត់ run_apply(const sa_nracanaa am_state *state, const var_na *index_file)
{
	sa_nracanaa strvec apply_paths = STRVEC_INIT;
	sa_nracanaa strvec apply_opts = STRVEC_INIT;
	sa_nracanaa apply_state apply_state;
	ចំនួនគត់ res, opts_left;
	ចំនួនគត់ force_apply = 0;
	ចំនួនគត់ options = 0;
	const var_na **apply_argv;

	ប្រសិនបើ (init_apply_state(&apply_state, the_repository, NULL))
		BUG("init_apply_state() failed");

	strvec_push(&apply_opts, "apply");
	strvec_pushv(&apply_opts, state->git_apply_opts.v);

	/*
	 * Build a copy that apply_parse_options() can rearrange.
	 * apply_opts.v keeps referencing the allocated strings for
	 * strvec_clear() to release.
	 */
	DUP_ARRAY(apply_argv, apply_opts.v, apply_opts.nr);

	opts_left = apply_parse_options(apply_opts.nr, apply_argv,
					&apply_state, &force_apply, &options,
					NULL);

	ប្រសិនបើ (opts_left != 0)
		die("unknown option passed through to git apply");

	ប្រសិនបើ (index_file) {
		apply_state.index_file = index_file;
		apply_state.cached = 1;
	} បើមិនដូច្នេះ
		apply_state.check_index = 1;

	/*
	 * If we are allowed to fall back on 3-way merge, don't give false
	 * errors during the initial attempt.
	 */
	ប្រសិនបើ (state->threeway && !index_file)
		apply_state.apply_verbosity = verbosity_silent;

	ប្រសិនបើ (check_apply_state(&apply_state, force_apply))
		BUG("check_apply_state() failed");

	strvec_push(&apply_paths, am_path(state, "patch"));

	res = apply_all_patches(&apply_state, apply_paths.nr, apply_paths.v, options);

	strvec_clear(&apply_paths);
	strvec_clear(&apply_opts);
	clear_apply_state(&apply_state);
	free(apply_argv);

	ប្រសិនបើ (res)
		ត្រឡប់ res;

	ប្រសិនបើ (index_file) {
		/* Reload index as apply_all_patches() will have modified it. */
		discard_index(the_repository->index);
		read_index_from(the_repository->index, index_file,
				repo_get_git_dir(the_repository));
	}

	ត្រឡប់ 0;
}

/**
 * Builds an index that contains just the blobs needed for a 3way merge.
 */
static ចំនួនគត់ build_fake_ancestor(const sa_nracanaa am_state *state, const var_na *index_file)
{
	sa_nracanaa child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	strvec_push(&cp.args, "apply");
	strvec_pushv(&cp.args, state->git_apply_opts.v);
	strvec_pushf(&cp.args, "--build-fake-ancestor=%s", index_file);
	strvec_push(&cp.args, am_path(state, "patch"));

	ប្រសិនបើ (run_command(&cp))
		ត្រឡប់ -1;

	ត្រឡប់ 0;
}

/**
 * Attempt a threeway merge, using index_path as the temporary index.
 */
static ចំនួនគត់ fall_back_threeway(const sa_nracanaa am_state *state, const var_na *index_path)
{
	sa_nracanaa object_id their_tree, our_tree;
	sa_nracanaa object_id bases[1] = { 0 };
	sa_nracanaa merge_options o;
	sa_nracanaa commit *result;
	var_na *their_tree_name;

	ប្រសិនបើ (repo_get_oid(the_repository, "HEAD", &our_tree) < 0)
		oidcpy(&our_tree, the_hash_algo->empty_tree);

	ប្រសិនបើ (build_fake_ancestor(state, index_path))
		ត្រឡប់ error("could not build fake ancestor");

	discard_index(the_repository->index);
	read_index_from(the_repository->index, index_path, repo_get_git_dir(the_repository));

	ប្រសិនបើ (write_index_as_tree(&bases[0], the_repository->index, index_path, 0, NULL))
		ត្រឡប់ error(_("Repository lacks necessary blobs to fall back on 3-way merge."));

	say(state, stdout, _("Using index info to reconstruct a base tree..."));

	ប្រសិនបើ (!state->quiet) {
		/*
		 * List paths that needed 3-way fallback, so that the user can
		 * review them with extra care to spot mismerges.
		 */
		sa_nracanaa rev_info rev_info;

		repo_init_revisions(the_repository, &rev_info, NULL);
		rev_info.diffopt.output_format = DIFF_FORMAT_NAME_STATUS;
		rev_info.diffopt.filter |= diff_filter_bit('A');
		rev_info.diffopt.filter |= diff_filter_bit('M');
		add_pending_oid(&rev_info, "HEAD", &our_tree, 0);
		diff_setup_done(&rev_info.diffopt);
		run_diff_index(&rev_info, DIFF_INDEX_CACHED);
		release_revisions(&rev_info);
	}

	ប្រសិនបើ (run_apply(state, index_path))
		ត្រឡប់ error(_("Did you hand edit your patch?\n"
				"It does not apply to blobs recorded in its index."));

	ប្រសិនបើ (write_index_as_tree(&their_tree, the_repository->index, index_path, 0, NULL))
		ត្រឡប់ error("could not write tree");

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
	their_tree_name = xstrfmt("%.*s", linelen(state->msg), state->msg);
	o.branch2 = their_tree_name;
	o.ancestor = "constructed fake ancestor";
	o.detect_directory_renames = MERGE_DIRECTORY_RENAMES_NONE;

	ប្រសិនបើ (state->quiet)
		o.verbosity = 0;

	ប្រសិនបើ (merge_ort_generic(&o, &our_tree, &their_tree, 1, bases, &result)) {
		repo_rerere(the_repository, state->allow_rerere_autoupdate);
		free(their_tree_name);
		ត្រឡប់ error(_("Failed to merge in the changes."));
	}

	free(their_tree_name);
	ត្រឡប់ 0;
}

/**
 * Commits the current index with state->msg as the commit message and
 * state->author_name, state->author_email and state->author_date as the author
 * information.
 */
static void do_commit(const sa_nracanaa am_state *state)
{
	sa_nracanaa object_id tree, parent, commit;
	const sa_nracanaa object_id *old_oid;
	sa_nracanaa commit_list *parents = NULL;
	const var_na *reflog_msg, *author, *committer = NULL;
	sa_nracanaa strbuf sb = STRBUF_INIT;

	ប្រសិនបើ (!state->no_verify && run_hooks(the_repository, "pre-applypatch"))
		exit(1);

	ប្រសិនបើ (write_index_as_tree(&tree, the_repository->index,
				repo_get_index_file(the_repository),
				0, NULL))
		die(_("git write-tree failed to write a tree"));

	ប្រសិនបើ (!repo_get_oid_commit(the_repository, "HEAD", &parent)) {
		old_oid = &parent;
		commit_list_insert(lookup_commit(the_repository, &parent),
				   &parents);
	} បើមិនដូច្នេះ {
		old_oid = NULL;
		say(state, stderr, _("applying to an empty history"));
	}

	author = fmt_ident(state->author_name, state->author_email,
		WANT_AUTHOR_IDENT,
			state->ignore_date ? NULL : state->author_date,
			IDENT_STRICT);

	ប្រសិនបើ (state->committer_date_is_author_date)
		committer = fmt_ident(getenv("GIT_COMMITTER_NAME"),
				      getenv("GIT_COMMITTER_EMAIL"),
				      WANT_COMMITTER_IDENT,
				      state->ignore_date ? NULL
							 : state->author_date,
				      IDENT_STRICT);

	ប្រសិនបើ (commit_tree_extended(state->msg, state->msg_len, &tree, parents,
				 &commit, author, committer, state->sign_commit,
				 NULL))
		die(_("failed to write commit object"));

	reflog_msg = getenv("GIT_REFLOG_ACTION");
	ប្រសិនបើ (!reflog_msg)
		reflog_msg = "am";

	strbuf_addf(&sb, "%s: %.*s", reflog_msg, linelen(state->msg),
			state->msg);

	refs_update_ref(get_main_ref_store(the_repository), sb.buf, "HEAD",
			&commit, old_oid, 0,
			UPDATE_REFS_DIE_ON_ERR);

	ប្រសិនបើ (state->rebasing) {
		FILE *fp = xfopen(am_path(state, "rewritten"), "a");

		assert(!is_null_oid(&state->orig_commit));
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
static void validate_resume_state(const sa_nracanaa am_state *state)
{
	ប្រសិនបើ (!state->msg)
		die(_("cannot resume: %s does not exist."),
			am_path(state, "final-commit"));

	ប្រសិនបើ (!state->author_name || !state->author_email || !state->author_date)
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
static ចំនួនគត់ do_interactive(sa_nracanaa am_state *state)
{
	assert(state->msg);

	សម្រាប់ (;;) {
		var_na reply[64];

		puts(_("Commit Body is:"));
		puts("--------------------------");
		printf("%s", state->msg);
		puts("--------------------------");

		/*
		 * TRANSLATORS: Make sure to include [y], [n], [e], [v] and [a]
		 * in your translation. The program will only accept English
		 * input at this point.
		 */
		printf(_("Apply? [y]es/[n]o/[e]dit/[v]iew patch/[a]ccept all: "));
		ប្រសិនបើ (!fgets(reply, parimaa_na(reply), stdin))
			die("unable to read from stdin; aborting");

		ប្រសិនបើ (*reply == 'y' || *reply == 'Y') {
			ត្រឡប់ 0;
		} បើមិនដូច្នេះ ប្រសិនបើ (*reply == 'a' || *reply == 'A') {
			state->interactive = 0;
			ត្រឡប់ 0;
		} បើមិនដូច្នេះ ប្រសិនបើ (*reply == 'n' || *reply == 'N') {
			ត្រឡប់ 1;
		} បើមិនដូច្នេះ ប្រសិនបើ (*reply == 'e' || *reply == 'E') {
			sa_nracanaa strbuf msg = STRBUF_INIT;

			ប្រសិនបើ (!launch_editor(am_path(state, "final-commit"), &msg, NULL)) {
				free(state->msg);
				state->msg = strbuf_detach(&msg, &state->msg_len);
			}
			strbuf_release(&msg);
		} បើមិនដូច្នេះ ប្រសិនបើ (*reply == 'v' || *reply == 'V') {
			const var_na *pager = git_pager(the_repository, 1);
			sa_nracanaa child_process cp = CHILD_PROCESS_INIT;

			ប្រសិនបើ (!pager)
				pager = "cat";
			prepare_pager_args(&cp, pager);
			strvec_push(&cp.args, am_path(state, "patch"));
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
static void am_run(sa_nracanaa am_state *state, ចំនួនគត់ resume)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;

	unlink(am_path(state, "dirtyindex"));

	ប្រសិនបើ (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 NULL, NULL, NULL) < 0)
		die(_("unable to write index file"));

	ប្រសិនបើ (repo_index_has_changes(the_repository, NULL, &sb)) {
		write_state_bool(state, "dirtyindex", 1);
		die(_("Dirty index: cannot apply patches (dirty: %s)"), sb.buf);
	}

	strbuf_release(&sb);

	ខណៈពេល (state->cur <= state->last) {
		const var_na *mail = am_path(state, msgnum(state));
		ចំនួនគត់ apply_status;
		ចំនួនគត់ to_keep;

		reset_ident_date();

		ប្រសិនបើ (!file_exists(mail))
			goto next;

		ប្រសិនបើ (resume) {
			validate_resume_state(state);
		} បើមិនដូច្នេះ {
			ចំនួនគត់ skip;

			ប្រសិនបើ (state->rebasing)
				skip = parse_mail_rebase(state, mail);
			បើមិនដូច្នេះ
				skip = parse_mail(state, mail);

			ប្រសិនបើ (skip)
				goto next; /* mail should be skipped */

			ប្រសិនបើ (state->signoff)
				am_append_signoff(state);

			write_author_script(state);
			write_commit_msg(state);
		}

		ប្រសិនបើ (state->interactive && do_interactive(state))
			goto next;

		to_keep = 0;
		ប្រសិនបើ (is_empty_or_missing_file(am_path(state, "patch"))) {
			switch (state->empty_type) {
			pak_sa DROP_EMPTY_COMMIT:
				say(state, stdout, _("Skipping: %.*s"), linelen(state->msg), state->msg);
				goto next;
				bha_nga;
			pak_sa KEEP_EMPTY_COMMIT:
				to_keep = 1;
				say(state, stdout, _("Creating an empty commit: %.*s"),
					linelen(state->msg), state->msg);
				bha_nga;
			pak_sa STOP_ON_EMPTY_COMMIT:
				printf_ln(_("Patch is empty."));
				die_user_resolve(state);
				bha_nga;
			}
		}

		ប្រសិនបើ (run_applypatch_msg_hook(state))
			exit(1);
		ប្រសិនបើ (to_keep)
			goto commit;

		say(state, stdout, _("Applying: %.*s"), linelen(state->msg), state->msg);

		apply_status = run_apply(state, NULL);

		ប្រសិនបើ (apply_status && state->threeway) {
			sa_nracanaa strbuf sb = STRBUF_INIT;

			strbuf_addstr(&sb, am_path(state, "patch-merge-index"));
			apply_status = fall_back_threeway(state, sb.buf);
			strbuf_release(&sb);

			/*
			 * Applying the patch to an earlier tree and merging
			 * the result may have produced the same tree as ours.
			 */
			ប្រសិនបើ (!apply_status &&
			    !repo_index_has_changes(the_repository, NULL, NULL)) {
				say(state, stdout, _("No changes -- Patch already applied."));
				goto next;
			}
		}

		ប្រសិនបើ (apply_status) {
			printf_ln(_("Patch failed at %s %.*s"), msgnum(state),
				linelen(state->msg), state->msg);

			ប្រសិនបើ (advice_enabled(ADVICE_AM_WORK_DIR))
				advise(_("Use 'git am --show-current-patch=diff' to see the failed patch"));

			die_user_resolve(state);
		}

commit:
		do_commit(state);

next:
		am_next(state);

		ប្រសិនបើ (resume)
			am_load(state);
		resume = 0;
	}

	ប្រសិនបើ (!is_empty_or_missing_file(am_path(state, "rewritten"))) {
		assert(state->rebasing);
		copy_notes_for_rebase(state);
		run_post_rewrite_hook(state);
	}

	/*
	 * In rebasing mode, it's up to the caller to take care of
	 * housekeeping.
	 */
	ប្រសិនបើ (!state->rebasing) {
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
static void am_resolve(sa_nracanaa am_state *state, ចំនួនគត់ allow_empty)
{
	validate_resume_state(state);

	say(state, stdout, _("Applying: %.*s"), linelen(state->msg), state->msg);

	ប្រសិនបើ (!repo_index_has_changes(the_repository, NULL, NULL)) {
		ប្រសិនបើ (allow_empty && is_empty_or_missing_file(am_path(state, "patch"))) {
			printf_ln(_("No changes - recorded it as an empty commit."));
		} បើមិនដូច្នេះ {
			printf_ln(_("No changes - did you forget to use 'git add'?\n"
				    "If there is nothing left to stage, chances are that something else\n"
				    "already introduced the same changes; you might want to skip this patch."));
			die_user_resolve(state);
		}
	}

	ប្រសិនបើ (unmerged_index(the_repository->index)) {
		printf_ln(_("You still have unmerged paths in your index.\n"
			"You should 'git add' each file with resolved conflicts to mark them as such.\n"
			"You might run `git rm` on a file to accept \"deleted by them\" for it."));
		die_user_resolve(state);
	}

	ប្រសិនបើ (state->interactive) {
		write_index_patch(state);
		ប្រសិនបើ (do_interactive(state))
			goto next;
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
static ចំនួនគត់ fast_forward_to(sa_nracanaa tree *head, sa_nracanaa tree *remote, ចំនួនគត់ reset)
{
	sa_nracanaa lock_file lock_file = LOCK_INIT;
	sa_nracanaa unpack_trees_options opts;
	sa_nracanaa tree_desc t[2];

	ប្រសិនបើ (repo_parse_tree(the_repository, head) || repo_parse_tree(the_repository, remote))
		ត្រឡប់ -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	refresh_index(the_repository->index, REFRESH_QUIET, NULL, NULL, NULL);

	memset(&opts, 0, parimaa_na(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.update = 1;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = twoway_merge;
	init_tree_desc(&t[0], &head->object.oid, head->buffer, head->size);
	init_tree_desc(&t[1], &remote->object.oid, remote->buffer, remote->size);

	ប្រសិនបើ (unpack_trees(2, t, &opts)) {
		rollback_lock_file(&lock_file);
		ត្រឡប់ -1;
	}

	ប្រសិនបើ (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	ត្រឡប់ 0;
}

/**
 * Merges a tree into the index. The index's stat info will take precedence
 * over the merged tree's. Returns 0 on success, -1 on failure.
 */
static ចំនួនគត់ merge_tree(sa_nracanaa tree *tree)
{
	sa_nracanaa lock_file lock_file = LOCK_INIT;
	sa_nracanaa unpack_trees_options opts;
	sa_nracanaa tree_desc t[1];

	ប្រសិនបើ (repo_parse_tree(the_repository, tree))
		ត្រឡប់ -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, parimaa_na(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.merge = 1;
	opts.fn = oneway_merge;
	init_tree_desc(&t[0], &tree->object.oid, tree->buffer, tree->size);

	ប្រសិនបើ (unpack_trees(1, t, &opts)) {
		rollback_lock_file(&lock_file);
		ត្រឡប់ -1;
	}

	ប្រសិនបើ (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	ត្រឡប់ 0;
}

/**
 * Clean the index without touching entries that are not modified between
 * `head` and `remote`.
 */
static ចំនួនគត់ clean_index(const sa_nracanaa object_id *head, const sa_nracanaa object_id *remote)
{
	sa_nracanaa tree *head_tree, *remote_tree, *index_tree;
	sa_nracanaa object_id index;

	head_tree = repo_parse_tree_indirect(the_repository, head);
	ប្រសិនបើ (!head_tree)
		ត្រឡប់ error(_("Could not parse object '%s'."), oid_to_hex(head));

	remote_tree = repo_parse_tree_indirect(the_repository, remote);
	ប្រសិនបើ (!remote_tree)
		ត្រឡប់ error(_("Could not parse object '%s'."), oid_to_hex(remote));

	repo_read_index_unmerged(the_repository);

	ប្រសិនបើ (fast_forward_to(head_tree, head_tree, 1))
		ត្រឡប់ -1;

	ប្រសិនបើ (write_index_as_tree(&index, the_repository->index,
				repo_get_index_file(the_repository),
				0, NULL))
		ត្រឡប់ -1;

	index_tree = repo_parse_tree_indirect(the_repository, &index);
	ប្រសិនបើ (!index_tree)
		ត្រឡប់ error(_("Could not parse object '%s'."), oid_to_hex(&index));

	ប្រសិនបើ (fast_forward_to(index_tree, remote_tree, 0))
		ត្រឡប់ -1;

	ប្រសិនបើ (merge_tree(remote_tree))
		ត្រឡប់ -1;

	remove_branch_state(the_repository, 0);

	ត្រឡប់ 0;
}

/**
 * Resets rerere's merge resolution metadata.
 */
static void am_rerere_clear(void)
{
	sa_nracanaa string_list merge_rr = STRING_LIST_INIT_DUP;
	rerere_clear(the_repository, &merge_rr);
	string_list_clear(&merge_rr, 1);
}

/**
 * Resume the current am session by skipping the current patch.
 */
static void am_skip(sa_nracanaa am_state *state)
{
	sa_nracanaa object_id head;

	am_rerere_clear();

	ប្រសិនបើ (repo_get_oid(the_repository, "HEAD", &head))
		oidcpy(&head, the_hash_algo->empty_tree);

	ប្រសិនបើ (clean_index(&head, &head))
		die(_("failed to clean index"));

	ប្រសិនបើ (state->rebasing) {
		FILE *fp = xfopen(am_path(state, "rewritten"), "a");

		assert(!is_null_oid(&state->orig_commit));
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
static ចំនួនគត់ safe_to_abort(const sa_nracanaa am_state *state)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	sa_nracanaa object_id abort_safety, head;

	ប្រសិនបើ (file_exists(am_path(state, "dirtyindex")))
		ត្រឡប់ 0;

	ប្រសិនបើ (read_state_file(&sb, state, "abort-safety", 1) > 0) {
		ប្រសិនបើ (get_oid_hex(sb.buf, &abort_safety))
			die(_("could not parse %s"), am_path(state, "abort-safety"));
	} បើមិនដូច្នេះ
		oidclr(&abort_safety, the_repository->hash_algo);
	strbuf_release(&sb);

	ប្រសិនបើ (repo_get_oid(the_repository, "HEAD", &head))
		oidclr(&head, the_repository->hash_algo);

	ប្រសិនបើ (oideq(&head, &abort_safety))
		ត្រឡប់ 1;

	warning(_("You seem to have moved HEAD since the last 'am' failure.\n"
		"Not rewinding to ORIG_HEAD"));

	ត្រឡប់ 0;
}

/**
 * Aborts the current am session if it is safe to do so.
 */
static void am_abort(sa_nracanaa am_state *state)
{
	sa_nracanaa object_id curr_head, orig_head;
	ចំនួនគត់ has_curr_head, has_orig_head;
	var_na *curr_branch;

	ប្រសិនបើ (!safe_to_abort(state)) {
		am_destroy(state);
		ត្រឡប់;
	}

	am_rerere_clear();

	curr_branch = refs_resolve_refdup(get_main_ref_store(the_repository),
					  "HEAD", 0, &curr_head, NULL);
	has_curr_head = curr_branch && !is_null_oid(&curr_head);
	ប្រសិនបើ (!has_curr_head)
		oidcpy(&curr_head, the_hash_algo->empty_tree);

	has_orig_head = !repo_get_oid(the_repository, "ORIG_HEAD", &orig_head);
	ប្រសិនបើ (!has_orig_head)
		oidcpy(&orig_head, the_hash_algo->empty_tree);

	ប្រសិនបើ (clean_index(&curr_head, &orig_head))
		die(_("failed to clean index"));

	ប្រសិនបើ (has_orig_head)
		refs_update_ref(get_main_ref_store(the_repository),
				"am --abort", "HEAD", &orig_head,
				has_curr_head ? &curr_head : NULL, 0,
				UPDATE_REFS_DIE_ON_ERR);
	បើមិនដូច្នេះ ប្រសិនបើ (curr_branch)
		refs_delete_ref(get_main_ref_store(the_repository), NULL,
				curr_branch, NULL, REF_NO_DEREF);

	free(curr_branch);
	am_destroy(state);
}

static ចំនួនគត់ show_patch(sa_nracanaa am_state *state, ga_nanaa resume_type resume_mode)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	const var_na *patch_path;
	ចំនួនគត់ len;

	ប្រសិនបើ (!is_null_oid(&state->orig_commit)) {
		sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

		strvec_pushl(&cmd.args, "show", oid_to_hex(&state->orig_commit),
			     "--", NULL);
		cmd.git_cmd = 1;
		ត្រឡប់ run_command(&cmd);
	}

	switch (resume_mode) {
	pak_sa RESUME_SHOW_PATCH_RAW:
		patch_path = am_path(state, msgnum(state));
		bha_nga;
	pak_sa RESUME_SHOW_PATCH_DIFF:
		patch_path = am_path(state, "patch");
		bha_nga;
	default:
		BUG("invalid mode for --show-current-patch");
	}

	len = strbuf_read_file(&sb, patch_path, 0);
	ប្រសិនបើ (len < 0)
		die_errno(_("failed to read '%s'"), patch_path);

	setup_pager(the_repository);
	write_in_full(1, sb.buf, sb.len);
	strbuf_release(&sb);
	ត្រឡប់ 0;
}

/**
 * parse_options() callback that validates and sets opt->value to the
 * PATCH_FORMAT_* enum value corresponding to `arg`.
 */
static ចំនួនគត់ parse_opt_patchformat(const sa_nracanaa option *opt, const var_na *arg, ចំនួនគត់ unset)
{
	ចំនួនគត់ *opt_value = opt->value;

	ប្រសិនបើ (unset)
		*opt_value = PATCH_FORMAT_UNKNOWN;
	បើមិនដូច្នេះ ប្រសិនបើ (!strcmp(arg, "mbox"))
		*opt_value = PATCH_FORMAT_MBOX;
	បើមិនដូច្នេះ ប្រសិនបើ (!strcmp(arg, "stgit"))
		*opt_value = PATCH_FORMAT_STGIT;
	បើមិនដូច្នេះ ប្រសិនបើ (!strcmp(arg, "stgit-series"))
		*opt_value = PATCH_FORMAT_STGIT_SERIES;
	បើមិនដូច្នេះ ប្រសិនបើ (!strcmp(arg, "hg"))
		*opt_value = PATCH_FORMAT_HG;
	បើមិនដូច្នេះ ប្រសិនបើ (!strcmp(arg, "mboxrd"))
		*opt_value = PATCH_FORMAT_MBOXRD;
	/*
	 * Please update $__git_patchformat in git-completion.bash
	 * when you add new options
	 */
	បើមិនដូច្នេះ
		ត្រឡប់ error(_("invalid value for '%s': '%s'"),
			     "--patch-format", arg);
	ត្រឡប់ 0;
}

static ចំនួនគត់ parse_opt_show_current_patch(const sa_nracanaa option *opt, const var_na *arg, ចំនួនគត់ unset)
{
	ចំនួនគត់ *opt_value = opt->value;

	BUG_ON_OPT_NEG(unset);

	ប្រសិនបើ (!arg)
		*opt_value = opt->defval;
	បើមិនដូច្នេះ ប្រសិនបើ (!strcmp(arg, "raw"))
		*opt_value = RESUME_SHOW_PATCH_RAW;
	បើមិនដូច្នេះ ប្រសិនបើ (!strcmp(arg, "diff"))
		*opt_value = RESUME_SHOW_PATCH_DIFF;
	/*
	 * Please update $__git_showcurrentpatch in git-completion.bash
	 * when you add new options
	 */
	បើមិនដូច្នេះ
		ត្រឡប់ error(_("invalid value for '%s': '%s'"),
			     "--show-current-patch", arg);
	ត្រឡប់ 0;
}

ចំនួនគត់ cmd_am(ចំនួនគត់ argc,
	   const var_na **argv,
	   const var_na *prefix,
	   sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa am_state state;
	ចំនួនគត់ binary = -1;
	ចំនួនគត់ keep_cr = -1;
	ចំនួនគត់ patch_format = PATCH_FORMAT_UNKNOWN;
	ga_nanaa resume_type resume_mode = RESUME_FALSE;
	ចំនួនគត់ in_progress;
	ចំនួនគត់ ret = 0;

	const var_na * const usage[] = {
		N_("git am [<options>] [(<mbox> | <Maildir>)...]"),
		N_("git am [<options>] (--continue | --skip | --abort)"),
		NULL
	};

	sa_nracanaa option options[] = {
		OPT_BOOL('i', "interactive", &state.interactive,
			N_("run interactively")),
		OPT_BOOL('n', "no-verify", &state.no_verify,
			N_("bypass pre-applypatch and applypatch-msg hooks")),
		OPT_HIDDEN_BOOL('b', "binary", &binary,
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
		OPT_PASSTHRU_ARGV(0, "ignore-space-change", &state.git_apply_opts, NULL,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU_ARGV(0, "ignore-whitespace", &state.git_apply_opts, NULL,
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
		OPT_PASSTHRU_ARGV('C', NULL, &state.git_apply_opts, N_("n"),
			N_("pass it through git-apply"),
			0),
		OPT_PASSTHRU_ARGV('p', NULL, &state.git_apply_opts, N_("num"),
			N_("pass it through git-apply"),
			0),
		OPT_CALLBACK(0, "patch-format", &patch_format, N_("format"),
			N_("format the patch(es) are in"),
			parse_opt_patchformat),
		OPT_PASSTHRU_ARGV(0, "reject", &state.git_apply_opts, NULL,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_STRING(0, "resolvemsg", &state.resolvemsg, NULL,
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
			.value = &resume_mode,
			.precision = parimaa_na(resume_mode),
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
			.value = &state.sign_commit,
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

	repo_config(the_repository, git_default_config, NULL);

	am_state_init(&state);

	in_progress = am_in_progress(&state);
	ប្រសិនបើ (in_progress)
		am_load(&state);

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	ប្រសិនបើ (binary >= 0)
		fprintf_ln(stderr, _("The -b/--binary option has been a no-op for long time, and\n"
				"it will be removed. Please do not use it anymore."));

	/* Ensure a valid committer ident can be constructed */
	git_committer_info(IDENT_STRICT);

	ប្រសិនបើ (repo_read_index_preload(the_repository, NULL, 0) < 0)
		die(_("failed to read the index"));

	ប្រសិនបើ (in_progress) {
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
		ប្រសិនបើ (argc || (resume_mode == RESUME_FALSE && !isatty(0)))
			die(_("previous rebase directory %s still exists but mbox given."),
				state.dir);

		ប្រសិនបើ (resume_mode == RESUME_FALSE)
			resume_mode = RESUME_APPLY;

		ប្រសិនបើ (state.signoff == SIGNOFF_EXPLICIT)
			am_append_signoff(&state);
	} បើមិនដូច្នេះ {
		sa_nracanaa strvec paths = STRVEC_INIT;
		ចំនួនគត់ i;

		/*
		 * Handle stray state directory in the independent-run case. In
		 * the --rebasing case, it is up to the caller to take care of
		 * stray directories.
		 */
		ប្រសិនបើ (file_exists(state.dir) && !state.rebasing) {
			ប្រសិនបើ (resume_mode == RESUME_ABORT || resume_mode == RESUME_QUIT) {
				am_destroy(&state);
				am_state_release(&state);
				ត្រឡប់ 0;
			}

			die(_("Stray %s directory found.\n"
				"Use \"git am --abort\" to remove it."),
				state.dir);
		}

		ប្រសិនបើ (resume_mode)
			die(_("Resolve operation not in progress, we are not resuming."));

		សម្រាប់ (i = 0; i < argc; i++) {
			ប្រសិនបើ (is_absolute_path(argv[i]) || !prefix)
				strvec_push(&paths, argv[i]);
			បើមិនដូច្នេះ
				strvec_push(&paths, mkpath("%s/%s", prefix, argv[i]));
		}

		ប្រសិនបើ (state.interactive && !paths.nr)
			die(_("interactive mode requires patches on the command line"));

		am_setup(&state, patch_format, paths.v, keep_cr);

		strvec_clear(&paths);
	}

	switch (resume_mode) {
	pak_sa RESUME_FALSE:
		am_run(&state, 0);
		bha_nga;
	pak_sa RESUME_APPLY:
		am_run(&state, 1);
		bha_nga;
	pak_sa RESUME_RESOLVED:
	pak_sa RESUME_ALLOW_EMPTY:
		am_resolve(&state, resume_mode == RESUME_ALLOW_EMPTY ? 1 : 0);
		bha_nga;
	pak_sa RESUME_SKIP:
		am_skip(&state);
		bha_nga;
	pak_sa RESUME_ABORT:
		am_abort(&state);
		bha_nga;
	pak_sa RESUME_QUIT:
		am_rerere_clear();
		am_destroy(&state);
		bha_nga;
	pak_sa RESUME_SHOW_PATCH_RAW:
	pak_sa RESUME_SHOW_PATCH_DIFF:
		ret = show_patch(&state, resume_mode);
		bha_nga;
	default:
		BUG("invalid resume value");
	}

	am_state_release(&state);

	ត្រឡប់ ret;
}
