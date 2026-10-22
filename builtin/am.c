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
static 整数 linelen(const char *msg)
{
	返回 strchrnul(msg, '\n') - msg;
}

/**
 * Returns true if `str` consists of only whitespace, false otherwise.
 */
static 整数 str_isspace(const char *str)
{
	为 (; *str; str++)
		如果 (!isspace(*str))
			返回 0;

	返回 1;
}

enum patch_format {
	PATCH_FORMAT_UNKNOWN = 0,
	PATCH_FORMAT_MBOX,
	PATCH_FORMAT_STGIT,
	PATCH_FORMAT_STGIT_SERIES,
	PATCH_FORMAT_HG,
	PATCH_FORMAT_MBOXRD
};

enum keep_type {
	KEEP_FALSE = 0,
	KEEP_TRUE,      /* pass -k flag to git-mailinfo */
	KEEP_NON_PATCH  /* pass -b flag to git-mailinfo */
};

enum scissors_type {
	SCISSORS_UNSET = -1,
	SCISSORS_FALSE = 0,  /* pass --no-scissors to git-mailinfo */
	SCISSORS_TRUE        /* pass --scissors to git-mailinfo */
};

enum signoff_type {
	SIGNOFF_FALSE = 0,
	SIGNOFF_TRUE = 1,
	SIGNOFF_EXPLICIT /* --signoff was set on the command-line */
};

enum resume_type {
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

enum empty_action {
	STOP_ON_EMPTY_COMMIT = 0,  /* output errors and stop in the middle of an am session */
	DROP_EMPTY_COMMIT,         /* skip with a notice message, unless "--quiet" has been passed */
	KEEP_EMPTY_COMMIT,         /* keep recording as empty commits */
};

struct am_state {
	/* state directory path */
	char *dir;

	/* current and last patch numbers, 1-indexed */
	整数 cur;
	整数 last;

	/* commit metadata and message */
	char *author_name;
	char *author_email;
	char *author_date;
	char *msg;
	size_t msg_len;

	/* when --rebasing, records the original commit the patch came from */
	struct object_id orig_commit;

	/* number of digits in patch filename */
	整数 prec;

	/* various operating modes and command line options */
	整数 interactive;
	整数 no_verify;
	整数 threeway;
	整数 quiet;
	整数 signoff; /* enum signoff_type */
	整数 utf8;
	整数 keep; /* enum keep_type */
	整数 message_id;
	整数 scissors; /* enum scissors_type */
	整数 quoted_cr; /* enum quoted_cr_action */
	整数 empty_type; /* enum empty_action */
	struct strvec git_apply_opts;
	const char *resolvemsg;
	整数 committer_date_is_author_date;
	整数 ignore_date;
	整数 allow_rerere_autoupdate;
	const char *sign_commit;
	整数 rebasing;
};

/**
 * Initializes am_state with the default values.
 */
static void am_state_init(struct am_state *state)
{
	整数 gpgsign;

	memset(state, 0, sizeof(*state));

	state->dir = repo_git_path(the_repository, "rebase-apply");

	state->prec = 4;

	repo_config_get_bool(the_repository, "am.threeway", &state->threeway);

	state->utf8 = 1;

	repo_config_get_bool(the_repository, "am.messageid", &state->message_id);

	state->scissors = SCISSORS_UNSET;
	state->quoted_cr = quoted_cr_unset;

	strvec_init(&state->git_apply_opts);

	如果 (!repo_config_get_bool(the_repository, "commit.gpgsign", &gpgsign))
		state->sign_commit = gpgsign ? "" : NULL;
}

/**
 * Releases memory allocated by an am_state.
 */
static void am_state_release(struct am_state *state)
{
	free(state->dir);
	free(state->author_name);
	free(state->author_email);
	free(state->author_date);
	free(state->msg);
	strvec_clear(&state->git_apply_opts);
}

static 整数 am_option_parse_quoted_cr(const struct option *opt,
				     const char *arg, 整数 unset)
{
	BUG_ON_OPT_NEG(unset);

	如果 (mailinfo_parse_quoted_cr_action(arg, opt->value) != 0)
		返回 error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	返回 0;
}

static 整数 am_option_parse_empty(const struct option *opt,
				     const char *arg, 整数 unset)
{
	整数 *opt_value = opt->value;

	BUG_ON_OPT_NEG(unset);

	如果 (!strcmp(arg, "stop"))
		*opt_value = STOP_ON_EMPTY_COMMIT;
	否则 如果 (!strcmp(arg, "drop"))
		*opt_value = DROP_EMPTY_COMMIT;
	否则 如果 (!strcmp(arg, "keep"))
		*opt_value = KEEP_EMPTY_COMMIT;
	否则
		返回 error(_("invalid value for '%s': '%s'"), "--empty", arg);

	返回 0;
}

/**
 * Returns path relative to the am_state directory.
 */
static inline const char *am_path(const struct am_state *state, const char *path)
{
	返回 mkpath("%s/%s", state->dir, path);
}

/**
 * For convenience to call write_file()
 */
static void write_state_text(const struct am_state *state,
			     const char *name, const char *string)
{
	write_file(am_path(state, name), "%s", string);
}

static void write_state_count(const struct am_state *state,
			      const char *name, 整数 value)
{
	write_file(am_path(state, name), "%d", value);
}

static void write_state_bool(const struct am_state *state,
			     const char *name, 整数 value)
{
	write_state_text(state, name, value ? "t" : "f");
}

/**
 * If state->quiet is false, calls fprintf(fp, fmt, ...), and appends a newline
 * at the end.
 */
__attribute__((format (printf, 3, 4)))
static void say(const struct am_state *state, FILE *fp, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	如果 (!state->quiet) {
		vfprintf(fp, fmt, ap);
		putc('\n', fp);
	}
	va_end(ap);
}

/**
 * Returns 1 if there is an am session in progress, 0 otherwise.
 */
static 整数 am_in_progress(const struct am_state *state)
{
	struct stat st;

	如果 (lstat(state->dir, &st) < 0 || !S_ISDIR(st.st_mode))
		返回 0;
	如果 (lstat(am_path(state, "last"), &st) || !S_ISREG(st.st_mode))
		返回 0;
	如果 (lstat(am_path(state, "next"), &st) || !S_ISREG(st.st_mode))
		返回 0;
	返回 1;
}

/**
 * Reads the contents of `file` in the `state` directory into `sb`. Returns the
 * number of bytes read on success, -1 if the file does not exist. If `trim` is
 * set, trailing whitespace will be removed.
 */
static 整数 read_state_file(struct strbuf *sb, const struct am_state *state,
			const char *file, 整数 trim)
{
	strbuf_reset(sb);

	如果 (strbuf_read_file(sb, am_path(state, file), 0) >= 0) {
		如果 (trim)
			strbuf_trim(sb);

		返回 sb->len;
	}

	如果 (errno == ENOENT)
		返回 -1;

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
static 整数 read_am_author_script(struct am_state *state)
{
	const char *filename = am_path(state, "author-script");

	assert(!state->author_name);
	assert(!state->author_email);
	assert(!state->author_date);

	返回 read_author_script(filename, &state->author_name,
				  &state->author_email, &state->author_date, 1);
}

/**
 * Saves state->author_name, state->author_email and state->author_date in the
 * state directory's "author-script" file.
 */
static void write_author_script(const struct am_state *state)
{
	struct strbuf sb = STRBUF_INIT;

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
static 整数 read_commit_msg(struct am_state *state)
{
	struct strbuf sb = STRBUF_INIT;

	assert(!state->msg);

	如果 (read_state_file(&sb, state, "final-commit", 0) < 0) {
		strbuf_release(&sb);
		返回 -1;
	}

	state->msg = strbuf_detach(&sb, &state->msg_len);
	返回 0;
}

/**
 * Saves state->msg in the state directory's "final-commit" file.
 */
static void write_commit_msg(const struct am_state *state)
{
	const char *filename = am_path(state, "final-commit");
	write_file_buf(filename, state->msg, state->msg_len);
}

/**
 * Loads state from disk.
 */
static void am_load(struct am_state *state)
{
	struct strbuf sb = STRBUF_INIT;

	如果 (read_state_file(&sb, state, "next", 1) < 0)
		BUG("state file 'next' does not exist");
	state->cur = strtol(sb.buf, NULL, 10);

	如果 (read_state_file(&sb, state, "last", 1) < 0)
		BUG("state file 'last' does not exist");
	state->last = strtol(sb.buf, NULL, 10);

	如果 (read_am_author_script(state) < 0)
		die(_("could not parse author script"));

	read_commit_msg(state);

	如果 (read_state_file(&sb, state, "original-commit", 1) < 0)
		oidclr(&state->orig_commit, the_repository->hash_algo);
	否则 如果 (get_oid_hex(sb.buf, &state->orig_commit) < 0)
		die(_("could not parse %s"), am_path(state, "original-commit"));

	read_state_file(&sb, state, "threeway", 1);
	state->threeway = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "quiet", 1);
	state->quiet = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "sign", 1);
	state->signoff = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "utf8", 1);
	state->utf8 = !strcmp(sb.buf, "t");

	如果 (file_exists(am_path(state, "rerere-autoupdate"))) {
		read_state_file(&sb, state, "rerere-autoupdate", 1);
		state->allow_rerere_autoupdate = strcmp(sb.buf, "t") ?
			RERERE_NOAUTOUPDATE : RERERE_AUTOUPDATE;
	} 否则 {
		state->allow_rerere_autoupdate = 0;
	}

	read_state_file(&sb, state, "keep", 1);
	如果 (!strcmp(sb.buf, "t"))
		state->keep = KEEP_TRUE;
	否则 如果 (!strcmp(sb.buf, "b"))
		state->keep = KEEP_NON_PATCH;
	否则
		state->keep = KEEP_FALSE;

	read_state_file(&sb, state, "messageid", 1);
	state->message_id = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "scissors", 1);
	如果 (!strcmp(sb.buf, "t"))
		state->scissors = SCISSORS_TRUE;
	否则 如果 (!strcmp(sb.buf, "f"))
		state->scissors = SCISSORS_FALSE;
	否则
		state->scissors = SCISSORS_UNSET;

	read_state_file(&sb, state, "quoted-cr", 1);
	如果 (!*sb.buf)
		state->quoted_cr = quoted_cr_unset;
	否则 如果 (mailinfo_parse_quoted_cr_action(sb.buf, &state->quoted_cr) != 0)
		die(_("could not parse %s"), am_path(state, "quoted-cr"));

	read_state_file(&sb, state, "apply-opt", 1);
	strvec_clear(&state->git_apply_opts);
	如果 (sq_dequote_to_strvec(sb.buf, &state->git_apply_opts) < 0)
		die(_("could not parse %s"), am_path(state, "apply-opt"));

	state->rebasing = !!file_exists(am_path(state, "rebasing"));

	strbuf_release(&sb);
}

/**
 * Removes the am_state directory, forcefully terminating the current am
 * session.
 */
static void am_destroy(const struct am_state *state)
{
	struct strbuf sb = STRBUF_INIT;

	strbuf_addstr(&sb, state->dir);
	remove_dir_recursively(&sb, 0);
	strbuf_release(&sb);
}

/**
 * Runs applypatch-msg hook. Returns its exit code.
 */
static 整数 run_applypatch_msg_hook(struct am_state *state)
{
	整数 ret = 0;

	assert(state->msg);

	如果 (!state->no_verify) {
		struct run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;
		strvec_push(&opt.args, am_path(state, "final-commit"));
		ret = run_hooks_opt(the_repository, "applypatch-msg", &opt);
	}

	如果 (!ret) {
		FREE_AND_NULL(state->msg);
		如果 (read_commit_msg(state) < 0)
			die(_("'%s' was deleted by the applypatch-msg hook"),
				am_path(state, "final-commit"));
	}

	返回 ret;
}

/**
 * Runs post-rewrite hook. Returns it exit code.
 */
static 整数 run_post_rewrite_hook(const struct am_state *state)
{
	struct run_hooks_opt opt = RUN_HOOKS_OPT_INIT;

	strvec_push(&opt.args, "rebase");
	opt.path_to_stdin = am_path(state, "rewritten");

	返回 run_hooks_opt(the_repository, "post-rewrite", &opt);
}

/**
 * Reads the state directory's "rewritten" file, and copies notes from the old
 * commits listed in the file to their rewritten commits.
 *
 * Returns 0 on success, -1 on failure.
 */
static 整数 copy_notes_for_rebase(const struct am_state *state)
{
	struct notes_rewrite_cfg *c;
	struct strbuf sb = STRBUF_INIT;
	const char *invalid_line = _("Malformed input line: '%s'.");
	const char *msg = "Notes added by 'git rebase'";
	FILE *fp;
	整数 ret = 0;

	assert(state->rebasing);

	c = init_copy_notes_for_rewrite("rebase");
	如果 (!c)
		返回 0;

	fp = xfopen(am_path(state, "rewritten"), "r");

	当 (!strbuf_getline_lf(&sb, fp)) {
		struct object_id from_obj, to_obj;
		const char *p;

		如果 (sb.len != the_hash_algo->hexsz * 2 + 1) {
			ret = error(invalid_line, sb.buf);
			goto finish;
		}

		如果 (parse_oid_hex(sb.buf, &from_obj, &p)) {
			ret = error(invalid_line, sb.buf);
			goto finish;
		}

		如果 (*p != ' ') {
			ret = error(invalid_line, sb.buf);
			goto finish;
		}

		如果 (get_oid_hex(p + 1, &to_obj)) {
			ret = error(invalid_line, sb.buf);
			goto finish;
		}

		如果 (copy_note_for_rewrite(c, &from_obj, &to_obj))
			ret = error(_("Failed to copy notes from '%s' to '%s'"),
					oid_to_hex(&from_obj), oid_to_hex(&to_obj));
	}

finish:
	finish_copy_notes_for_rewrite(the_repository, c, msg);
	fclose(fp);
	strbuf_release(&sb);
	返回 ret;
}

/**
 * Determines if the file looks like a piece of RFC2822 mail by grabbing all
 * non-indented lines and checking if they look like they begin with valid
 * header field names.
 *
 * Returns 1 if the file looks like a piece of mail, 0 otherwise.
 */
static 整数 is_mail(FILE *fp)
{
	const char *header_regex = "^[!-9;-~]+:";
	struct strbuf sb = STRBUF_INIT;
	regex_t regex;
	整数 ret = 1;

	如果 (fseek(fp, 0L, SEEK_SET))
		die_errno(_("fseek failed"));

	如果 (regcomp(&regex, header_regex, REG_NOSUB | REG_EXTENDED))
		die("invalid pattern: %s", header_regex);

	当 (!strbuf_getline(&sb, fp)) {
		如果 (!sb.len)
			break; /* End of header */

		/* Ignore indented folded lines */
		如果 (*sb.buf == '\t' || *sb.buf == ' ')
			continue;

		/* It's a header if it matches header_regex */
		如果 (regexec(&regex, sb.buf, 0, NULL, 0)) {
			ret = 0;
			goto done;
		}
	}

done:
	regfree(&regex);
	strbuf_release(&sb);
	返回 ret;
}

/**
 * Attempts to detect the patch_format of the patches contained in `paths`,
 * returning the PATCH_FORMAT_* enum value. Returns PATCH_FORMAT_UNKNOWN if
 * detection fails.
 */
static 整数 detect_patch_format(const char **paths)
{
	enum patch_format ret = PATCH_FORMAT_UNKNOWN;
	struct strbuf l1 = STRBUF_INIT;
	struct strbuf l2 = STRBUF_INIT;
	struct strbuf l3 = STRBUF_INIT;
	FILE *fp;

	/*
	 * We default to mbox format if input is from stdin and for directories
	 */
	如果 (!*paths || !strcmp(*paths, "-") || is_directory(*paths))
		返回 PATCH_FORMAT_MBOX;

	/*
	 * Otherwise, check the first few lines of the first patch, starting
	 * from the first non-blank line, to try to detect its format.
	 */

	fp = xfopen(*paths, "r");

	当 (!strbuf_getline(&l1, fp)) {
		如果 (l1.len)
			break;
	}

	如果 (starts_with(l1.buf, "From ") || starts_with(l1.buf, "From: ")) {
		ret = PATCH_FORMAT_MBOX;
		goto done;
	}

	如果 (starts_with(l1.buf, "# This series applies on GIT commit")) {
		ret = PATCH_FORMAT_STGIT_SERIES;
		goto done;
	}

	如果 (!strcmp(l1.buf, "# HG changeset patch")) {
		ret = PATCH_FORMAT_HG;
		goto done;
	}

	strbuf_getline(&l2, fp);
	strbuf_getline(&l3, fp);

	/*
	 * If the second line is empty and the third is a From, Author or Date
	 * entry, this is likely an StGit patch.
	 */
	如果 (l1.len && !l2.len &&
		(starts_with(l3.buf, "From:") ||
		 starts_with(l3.buf, "Author:") ||
		 starts_with(l3.buf, "Date:"))) {
		ret = PATCH_FORMAT_STGIT;
		goto done;
	}

	如果 (l1.len && is_mail(fp)) {
		ret = PATCH_FORMAT_MBOX;
		goto done;
	}

done:
	fclose(fp);
	strbuf_release(&l1);
	strbuf_release(&l2);
	strbuf_release(&l3);
	返回 ret;
}

/**
 * Splits out individual email patches from `paths`, where each path is either
 * a mbox file or a Maildir. Returns 0 on success, -1 on failure.
 */
static 整数 split_mail_mbox(struct am_state *state, const char **paths,
				整数 keep_cr, 整数 mboxrd)
{
	struct child_process cp = CHILD_PROCESS_INIT;
	struct strbuf last = STRBUF_INIT;
	整数 ret;

	cp.git_cmd = 1;
	strvec_push(&cp.args, "mailsplit");
	strvec_pushf(&cp.args, "-d%d", state->prec);
	strvec_pushf(&cp.args, "-o%s", state->dir);
	strvec_push(&cp.args, "-b");
	如果 (keep_cr)
		strvec_push(&cp.args, "--keep-cr");
	如果 (mboxrd)
		strvec_push(&cp.args, "--mboxrd");
	strvec_push(&cp.args, "--");
	strvec_pushv(&cp.args, paths);

	ret = capture_command(&cp, &last, 8);
	如果 (ret)
		goto exit;

	state->cur = 1;
	state->last = strtol(last.buf, NULL, 10);

exit:
	strbuf_release(&last);
	返回 ret ? -1 : 0;
}

/**
 * Callback signature for split_mail_conv(). The foreign patch should be
 * read from `in`, and the converted patch (in RFC2822 mail format) should be
 * written to `out`. Return 0 on success, or -1 on failure.
 */
typedef 整数 (*mail_conv_fn)(FILE *out, FILE *in, 整数 keep_cr);

/**
 * Calls `fn` for each file in `paths` to convert the foreign patch to the
 * RFC2822 mail format suitable for parsing with git-mailinfo.
 *
 * Returns 0 on success, -1 on failure.
 */
static 整数 split_mail_conv(mail_conv_fn fn, struct am_state *state,
			const char **paths, 整数 keep_cr)
{
	static const char *stdin_only[] = {"-", NULL};
	整数 i;

	如果 (!*paths)
		paths = stdin_only;

	为 (i = 0; *paths; paths++, i++) {
		FILE *in, *out;
		const char *mail;
		整数 ret;

		如果 (!strcmp(*paths, "-"))
			in = stdin;
		否则
			in = fopen(*paths, "r");

		如果 (!in)
			返回 error_errno(_("could not open '%s' for reading"),
					   *paths);

		mail = mkpath("%s/%0*d", state->dir, state->prec, i + 1);

		out = fopen(mail, "w");
		如果 (!out) {
			如果 (in != stdin)
				fclose(in);
			返回 error_errno(_("could not open '%s' for writing"),
					   mail);
		}

		ret = fn(out, in, keep_cr);

		fclose(out);
		如果 (in != stdin)
			fclose(in);

		如果 (ret)
			返回 error(_("could not parse patch '%s'"), *paths);
	}

	state->cur = 1;
	state->last = i;
	返回 0;
}

/**
 * A split_mail_conv() callback that converts an StGit patch to an RFC2822
 * message suitable for parsing with git-mailinfo.
 */
static 整数 stgit_patch_to_mail(FILE *out, FILE *in, 整数 keep_cr UNUSED)
{
	struct strbuf sb = STRBUF_INIT;
	整数 subject_printed = 0;

	当 (!strbuf_getline_lf(&sb, in)) {
		const char *str;

		如果 (str_isspace(sb.buf))
			continue;
		否则 如果 (skip_prefix(sb.buf, "Author:", &str))
			fprintf(out, "From:%s\n", str);
		否则 如果 (starts_with(sb.buf, "From") || starts_with(sb.buf, "Date"))
			fprintf(out, "%s\n", sb.buf);
		否则 如果 (!subject_printed) {
			fprintf(out, "Subject: %s\n", sb.buf);
			subject_printed = 1;
		} 否则 {
			fprintf(out, "\n%s\n", sb.buf);
			break;
		}
	}

	strbuf_reset(&sb);
	当 (strbuf_fread(&sb, 8192, in) > 0) {
		fwrite(sb.buf, 1, sb.len, out);
		strbuf_reset(&sb);
	}

	strbuf_release(&sb);
	返回 0;
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
static 整数 split_mail_stgit_series(struct am_state *state, const char **paths,
					整数 keep_cr)
{
	const char *series_dir;
	char *series_dir_buf;
	FILE *fp;
	struct strvec patches = STRVEC_INIT;
	struct strbuf sb = STRBUF_INIT;
	整数 ret;

	如果 (!paths[0] || paths[1])
		返回 error(_("Only one StGIT patch series can be applied at once"));

	series_dir_buf = xstrdup(*paths);
	series_dir = dirname(series_dir_buf);

	fp = fopen(*paths, "r");
	如果 (!fp) {
		free(series_dir_buf);
		返回 error_errno(_("could not open '%s' for reading"), *paths);
	}

	当 (!strbuf_getline_lf(&sb, fp)) {
		如果 (*sb.buf == '#')
			continue; /* skip comment lines */

		strvec_push(&patches, mkpath("%s/%s", series_dir, sb.buf));
	}

	fclose(fp);
	strbuf_release(&sb);
	free(series_dir_buf);

	ret = split_mail_conv(stgit_patch_to_mail, state, patches.v, keep_cr);

	strvec_clear(&patches);
	返回 ret;
}

/**
 * A split_patches_conv() callback that converts a mercurial patch to a RFC2822
 * message suitable for parsing with git-mailinfo.
 */
static 整数 hg_patch_to_mail(FILE *out, FILE *in, 整数 keep_cr UNUSED)
{
	struct strbuf sb = STRBUF_INIT;
	整数 rc = 0;

	当 (!strbuf_getline_lf(&sb, in)) {
		const char *str;

		如果 (skip_prefix(sb.buf, "# User ", &str))
			fprintf(out, "From: %s\n", str);
		否则 如果 (skip_prefix(sb.buf, "# Date ", &str)) {
			timestamp_t timestamp;
			long tz, tz2;
			char *end;

			errno = 0;
			timestamp = parse_timestamp(str, &end, 10);
			如果 (errno) {
				rc = error(_("invalid timestamp"));
				goto exit;
			}

			如果 (!skip_prefix(end, " ", &str)) {
				rc = error(_("invalid Date line"));
				goto exit;
			}

			errno = 0;
			tz = strtol(str, &end, 10);
			如果 (errno) {
				rc = error(_("invalid timezone offset"));
				goto exit;
			}

			如果 (*end) {
				rc = error(_("invalid Date line"));
				goto exit;
			}

			/*
			 * mercurial's timezone is in seconds west of UTC,
			 * however git's timezone is in hours + minutes east of
			 * UTC. Convert it.
			 */
			tz2 = labs(tz) / 3600 * 100 + labs(tz) % 3600 / 60;
			如果 (tz > 0)
				tz2 = -tz2;

			fprintf(out, "Date: %s\n", show_date(timestamp, tz2, DATE_MODE(RFC2822)));
		} 否则 如果 (starts_with(sb.buf, "# ")) {
			continue;
		} 否则 {
			fprintf(out, "\n%s\n", sb.buf);
			break;
		}
	}

	strbuf_reset(&sb);
	当 (strbuf_fread(&sb, 8192, in) > 0) {
		fwrite(sb.buf, 1, sb.len, out);
		strbuf_reset(&sb);
	}
exit:
	strbuf_release(&sb);
	返回 rc;
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
static 整数 split_mail(struct am_state *state, enum patch_format patch_format,
			const char **paths, 整数 keep_cr)
{
	如果 (keep_cr < 0) {
		keep_cr = 0;
		repo_config_get_bool(the_repository, "am.keepcr", &keep_cr);
	}

	switch (patch_format) {
	case PATCH_FORMAT_MBOX:
		返回 split_mail_mbox(state, paths, keep_cr, 0);
	case PATCH_FORMAT_STGIT:
		返回 split_mail_conv(stgit_patch_to_mail, state, paths, keep_cr);
	case PATCH_FORMAT_STGIT_SERIES:
		返回 split_mail_stgit_series(state, paths, keep_cr);
	case PATCH_FORMAT_HG:
		返回 split_mail_conv(hg_patch_to_mail, state, paths, keep_cr);
	case PATCH_FORMAT_MBOXRD:
		返回 split_mail_mbox(state, paths, keep_cr, 1);
	default:
		BUG("invalid patch_format");
	}
	返回 -1;
}

/**
 * Setup a new am session for applying patches
 */
static void am_setup(struct am_state *state, enum patch_format patch_format,
			const char **paths, 整数 keep_cr)
{
	struct object_id curr_head;
	const char *str;
	struct strbuf sb = STRBUF_INIT;

	如果 (!patch_format)
		patch_format = detect_patch_format(paths);

	如果 (!patch_format) {
		fprintf_ln(stderr, _("Patch format detection failed."));
		die(NULL);
	}

	如果 (mkdir(state->dir, 0777) < 0 && errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), state->dir);
	refs_delete_ref(get_main_ref_store(the_repository), NULL,
			"REBASE_HEAD", NULL, REF_NO_DEREF);

	如果 (split_mail(state, patch_format, paths, keep_cr) < 0) {
		am_destroy(state);
		die(_("Failed to split patches."));
	}

	如果 (state->rebasing)
		state->threeway = 1;

	write_state_bool(state, "threeway", state->threeway);
	write_state_bool(state, "quiet", state->quiet);
	write_state_bool(state, "sign", state->signoff);
	write_state_bool(state, "utf8", state->utf8);

	如果 (state->allow_rerere_autoupdate)
		write_state_bool(state, "rerere-autoupdate",
			 state->allow_rerere_autoupdate == RERERE_AUTOUPDATE);

	switch (state->keep) {
	case KEEP_FALSE:
		str = "f";
		break;
	case KEEP_TRUE:
		str = "t";
		break;
	case KEEP_NON_PATCH:
		str = "b";
		break;
	default:
		BUG("invalid value for state->keep");
	}

	write_state_text(state, "keep", str);
	write_state_bool(state, "messageid", state->message_id);

	switch (state->scissors) {
	case SCISSORS_UNSET:
		str = "";
		break;
	case SCISSORS_FALSE:
		str = "f";
		break;
	case SCISSORS_TRUE:
		str = "t";
		break;
	default:
		BUG("invalid value for state->scissors");
	}
	write_state_text(state, "scissors", str);

	switch (state->quoted_cr) {
	case quoted_cr_unset:
		str = "";
		break;
	case quoted_cr_nowarn:
		str = "nowarn";
		break;
	case quoted_cr_warn:
		str = "warn";
		break;
	case quoted_cr_strip:
		str = "strip";
		break;
	default:
		BUG("invalid value for state->quoted_cr");
	}
	write_state_text(state, "quoted-cr", str);

	sq_quote_argv(&sb, state->git_apply_opts.v);
	write_state_text(state, "apply-opt", sb.buf);

	如果 (state->rebasing)
		write_state_text(state, "rebasing", "");
	否则
		write_state_text(state, "applying", "");

	如果 (!repo_get_oid(the_repository, "HEAD", &curr_head)) {
		write_state_text(state, "abort-safety", oid_to_hex(&curr_head));
		如果 (!state->rebasing)
			refs_update_ref(get_main_ref_store(the_repository),
					"am", "ORIG_HEAD", &curr_head, NULL,
					0,
					UPDATE_REFS_DIE_ON_ERR);
	} 否则 {
		write_state_text(state, "abort-safety", "");
		如果 (!state->rebasing)
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
static void am_next(struct am_state *state)
{
	struct object_id head;

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

	如果 (!repo_get_oid(the_repository, "HEAD", &head))
		write_state_text(state, "abort-safety", oid_to_hex(&head));
	否则
		write_state_text(state, "abort-safety", "");

	state->cur++;
	write_state_count(state, "next", state->cur);
}

/**
 * Returns the filename of the current patch email.
 */
static const char *msgnum(const struct am_state *state)
{
	static struct strbuf sb = STRBUF_INIT;

	strbuf_reset(&sb);
	strbuf_addf(&sb, "%0*d", state->prec, state->cur);

	返回 sb.buf;
}

/**
 * Dies with a user-friendly message on how to proceed after resolving the
 * problem. This message can be overridden with state->resolvemsg.
 */
static void NORETURN die_user_resolve(const struct am_state *state)
{
	如果 (state->resolvemsg) {
		advise_if_enabled(ADVICE_MERGE_CONFLICT, "%s", state->resolvemsg);
	} 否则 {
		const char *cmdline = state->interactive ? "git am -i" : "git am";
		struct strbuf sb = STRBUF_INIT;

		strbuf_addf(&sb, _("When you have resolved this problem, run \"%s --continue\".\n"), cmdline);
		strbuf_addf(&sb, _("If you prefer to skip this patch, run \"%s --skip\" instead.\n"), cmdline);

		如果 (advice_enabled(ADVICE_AM_WORK_DIR) &&
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
static void am_append_signoff(struct am_state *state)
{
	struct strbuf sb = STRBUF_INIT;

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
static 整数 parse_mail(struct am_state *state, const char *mail)
{
	FILE *fp;
	struct strbuf sb = STRBUF_INIT;
	struct strbuf msg = STRBUF_INIT;
	struct strbuf author_name = STRBUF_INIT;
	struct strbuf author_date = STRBUF_INIT;
	struct strbuf author_email = STRBUF_INIT;
	整数 ret = 0;
	struct mailinfo mi;

	setup_mailinfo(the_repository, &mi);

	如果 (state->utf8)
		mi.metainfo_charset = get_commit_output_encoding();
	否则
		mi.metainfo_charset = NULL;

	switch (state->keep) {
	case KEEP_FALSE:
		break;
	case KEEP_TRUE:
		mi.keep_subject = 1;
		break;
	case KEEP_NON_PATCH:
		mi.keep_non_patch_brackets_in_subject = 1;
		break;
	default:
		BUG("invalid value for state->keep");
	}

	如果 (state->message_id)
		mi.add_message_id = 1;

	switch (state->scissors) {
	case SCISSORS_UNSET:
		break;
	case SCISSORS_FALSE:
		mi.use_scissors = 0;
		break;
	case SCISSORS_TRUE:
		mi.use_scissors = 1;
		break;
	default:
		BUG("invalid value for state->scissors");
	}

	switch (state->quoted_cr) {
	case quoted_cr_unset:
		break;
	case quoted_cr_nowarn:
	case quoted_cr_warn:
	case quoted_cr_strip:
		mi.quoted_cr = state->quoted_cr;
		break;
	default:
		BUG("invalid value for state->quoted_cr");
	}

	mi.input = xfopen(mail, "r");
	mi.output = xfopen(am_path(state, "info"), "w");
	如果 (mailinfo(&mi, am_path(state, "msg"), am_path(state, "patch")))
		die("could not parse patch");

	fclose(mi.input);
	fclose(mi.output);

	如果 (mi.format_flowed)
		warning(_("Patch sent with format=flowed; "
			  "space at the end of lines might be lost."));

	/* Extract message and author information */
	fp = xfopen(am_path(state, "info"), "r");
	当 (!strbuf_getline_lf(&sb, fp)) {
		const char *x;

		如果 (skip_prefix(sb.buf, "Subject: ", &x)) {
			如果 (msg.len)
				strbuf_addch(&msg, '\n');
			strbuf_addstr(&msg, x);
		} 否则 如果 (skip_prefix(sb.buf, "Author: ", &x))
			strbuf_addstr(&author_name, x);
		否则 如果 (skip_prefix(sb.buf, "Email: ", &x))
			strbuf_addstr(&author_email, x);
		否则 如果 (skip_prefix(sb.buf, "Date: ", &x))
			strbuf_addstr(&author_date, x);
	}
	fclose(fp);

	/* Skip pine's internal folder data */
	如果 (!strcmp(author_name.buf, "Mail System Internal Data")) {
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
	返回 ret;
}

/**
 * Sets commit_id to the commit hash where the mail was generated from.
 * Returns 0 on success, -1 on failure.
 */
static 整数 get_mail_commit_oid(struct object_id *commit_id, const char *mail)
{
	struct strbuf sb = STRBUF_INIT;
	FILE *fp = xfopen(mail, "r");
	const char *x;
	整数 ret = 0;

	如果 (strbuf_getline_lf(&sb, fp) ||
	    !skip_prefix(sb.buf, "From ", &x) ||
	    get_oid_hex(x, commit_id) < 0)
		ret = -1;

	strbuf_release(&sb);
	fclose(fp);
	返回 ret;
}

/**
 * Sets state->msg, state->author_name, state->author_email, state->author_date
 * to the commit's respective info.
 */
static void get_commit_info(struct am_state *state, struct commit *commit)
{
	const char *buffer, *ident_line, *msg;
	size_t ident_len;
	struct ident_split id;

	buffer = repo_logmsg_reencode(the_repository, commit, NULL,
				      get_commit_output_encoding());

	ident_line = find_commit_header(buffer, "author", &ident_len);
	如果 (!ident_line)
		die(_("missing author line in commit %s"),
		      oid_to_hex(&commit->object.oid));
	如果 (split_ident_line(&id, ident_line, ident_len) < 0)
		die(_("invalid ident line: %.*s"), (整数)ident_len, ident_line);

	assert(!state->author_name);
	如果 (id.name_begin)
		state->author_name =
			xmemdupz(id.name_begin, id.name_end - id.name_begin);
	否则
		state->author_name = xstrdup("");

	assert(!state->author_email);
	如果 (id.mail_begin)
		state->author_email =
			xmemdupz(id.mail_begin, id.mail_end - id.mail_begin);
	否则
		state->author_email = xstrdup("");

	assert(!state->author_date);
	state->author_date = xstrdup(show_ident_date(&id, DATE_MODE(NORMAL)));

	assert(!state->msg);
	msg = strstr(buffer, "\n\n");
	如果 (!msg)
		die(_("unable to parse commit %s"), oid_to_hex(&commit->object.oid));
	state->msg = xstrdup(msg + 2);
	state->msg_len = strlen(state->msg);
	repo_unuse_commit_buffer(the_repository, commit, buffer);
}

/**
 * Writes `commit` as a patch to the state directory's "patch" file.
 */
static void write_commit_patch(const struct am_state *state, struct commit *commit)
{
	struct rev_info rev_info;
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
static void write_index_patch(const struct am_state *state)
{
	struct tree *tree;
	struct object_id head;
	struct rev_info rev_info;
	FILE *fp;

	如果 (!repo_get_oid(the_repository, "HEAD", &head)) {
		struct commit *commit = lookup_commit_or_die(&head, "HEAD");
		tree = repo_get_commit_tree(the_repository, commit);
	} 否则
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
static 整数 parse_mail_rebase(struct am_state *state, const char *mail)
{
	struct commit *commit;
	struct object_id commit_oid;

	如果 (get_mail_commit_oid(&commit_oid, mail) < 0)
		die(_("could not parse %s"), mail);

	commit = lookup_commit_or_die(&commit_oid, mail);

	get_commit_info(state, commit);

	write_commit_patch(state, commit);

	oidcpy(&state->orig_commit, &commit_oid);
	write_state_text(state, "original-commit", oid_to_hex(&commit_oid));
	refs_update_ref(get_main_ref_store(the_repository), "am",
			"REBASE_HEAD", &commit_oid,
			NULL, REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);

	返回 0;
}

/**
 * Applies current patch with git-apply. Returns 0 on success, -1 otherwise. If
 * `index_file` is not NULL, the patch will be applied to that index.
 */
static 整数 run_apply(const struct am_state *state, const char *index_file)
{
	struct strvec apply_paths = STRVEC_INIT;
	struct strvec apply_opts = STRVEC_INIT;
	struct apply_state apply_state;
	整数 res, opts_left;
	整数 force_apply = 0;
	整数 options = 0;
	const char **apply_argv;

	如果 (init_apply_state(&apply_state, the_repository, NULL))
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

	如果 (opts_left != 0)
		die("unknown option passed through to git apply");

	如果 (index_file) {
		apply_state.index_file = index_file;
		apply_state.cached = 1;
	} 否则
		apply_state.check_index = 1;

	/*
	 * If we are allowed to fall back on 3-way merge, don't give false
	 * errors during the initial attempt.
	 */
	如果 (state->threeway && !index_file)
		apply_state.apply_verbosity = verbosity_silent;

	如果 (check_apply_state(&apply_state, force_apply))
		BUG("check_apply_state() failed");

	strvec_push(&apply_paths, am_path(state, "patch"));

	res = apply_all_patches(&apply_state, apply_paths.nr, apply_paths.v, options);

	strvec_clear(&apply_paths);
	strvec_clear(&apply_opts);
	clear_apply_state(&apply_state);
	free(apply_argv);

	如果 (res)
		返回 res;

	如果 (index_file) {
		/* Reload index as apply_all_patches() will have modified it. */
		discard_index(the_repository->index);
		read_index_from(the_repository->index, index_file,
				repo_get_git_dir(the_repository));
	}

	返回 0;
}

/**
 * Builds an index that contains just the blobs needed for a 3way merge.
 */
static 整数 build_fake_ancestor(const struct am_state *state, const char *index_file)
{
	struct child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	strvec_push(&cp.args, "apply");
	strvec_pushv(&cp.args, state->git_apply_opts.v);
	strvec_pushf(&cp.args, "--build-fake-ancestor=%s", index_file);
	strvec_push(&cp.args, am_path(state, "patch"));

	如果 (run_command(&cp))
		返回 -1;

	返回 0;
}

/**
 * Attempt a threeway merge, using index_path as the temporary index.
 */
static 整数 fall_back_threeway(const struct am_state *state, const char *index_path)
{
	struct object_id their_tree, our_tree;
	struct object_id bases[1] = { 0 };
	struct merge_options o;
	struct commit *result;
	char *their_tree_name;

	如果 (repo_get_oid(the_repository, "HEAD", &our_tree) < 0)
		oidcpy(&our_tree, the_hash_algo->empty_tree);

	如果 (build_fake_ancestor(state, index_path))
		返回 error("could not build fake ancestor");

	discard_index(the_repository->index);
	read_index_from(the_repository->index, index_path, repo_get_git_dir(the_repository));

	如果 (write_index_as_tree(&bases[0], the_repository->index, index_path, 0, NULL))
		返回 error(_("Repository lacks necessary blobs to fall back on 3-way merge."));

	say(state, stdout, _("Using index info to reconstruct a base tree..."));

	如果 (!state->quiet) {
		/*
		 * List paths that needed 3-way fallback, so that the user can
		 * review them with extra care to spot mismerges.
		 */
		struct rev_info rev_info;

		repo_init_revisions(the_repository, &rev_info, NULL);
		rev_info.diffopt.output_format = DIFF_FORMAT_NAME_STATUS;
		rev_info.diffopt.filter |= diff_filter_bit('A');
		rev_info.diffopt.filter |= diff_filter_bit('M');
		add_pending_oid(&rev_info, "HEAD", &our_tree, 0);
		diff_setup_done(&rev_info.diffopt);
		run_diff_index(&rev_info, DIFF_INDEX_CACHED);
		release_revisions(&rev_info);
	}

	如果 (run_apply(state, index_path))
		返回 error(_("Did you hand edit your patch?\n"
				"It does not apply to blobs recorded in its index."));

	如果 (write_index_as_tree(&their_tree, the_repository->index, index_path, 0, NULL))
		返回 error("could not write tree");

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

	如果 (state->quiet)
		o.verbosity = 0;

	如果 (merge_ort_generic(&o, &our_tree, &their_tree, 1, bases, &result)) {
		repo_rerere(the_repository, state->allow_rerere_autoupdate);
		free(their_tree_name);
		返回 error(_("Failed to merge in the changes."));
	}

	free(their_tree_name);
	返回 0;
}

/**
 * Commits the current index with state->msg as the commit message and
 * state->author_name, state->author_email and state->author_date as the author
 * information.
 */
static void do_commit(const struct am_state *state)
{
	struct object_id tree, parent, commit;
	const struct object_id *old_oid;
	struct commit_list *parents = NULL;
	const char *reflog_msg, *author, *committer = NULL;
	struct strbuf sb = STRBUF_INIT;

	如果 (!state->no_verify && run_hooks(the_repository, "pre-applypatch"))
		exit(1);

	如果 (write_index_as_tree(&tree, the_repository->index,
				repo_get_index_file(the_repository),
				0, NULL))
		die(_("git write-tree failed to write a tree"));

	如果 (!repo_get_oid_commit(the_repository, "HEAD", &parent)) {
		old_oid = &parent;
		commit_list_insert(lookup_commit(the_repository, &parent),
				   &parents);
	} 否则 {
		old_oid = NULL;
		say(state, stderr, _("applying to an empty history"));
	}

	author = fmt_ident(state->author_name, state->author_email,
		WANT_AUTHOR_IDENT,
			state->ignore_date ? NULL : state->author_date,
			IDENT_STRICT);

	如果 (state->committer_date_is_author_date)
		committer = fmt_ident(getenv("GIT_COMMITTER_NAME"),
				      getenv("GIT_COMMITTER_EMAIL"),
				      WANT_COMMITTER_IDENT,
				      state->ignore_date ? NULL
							 : state->author_date,
				      IDENT_STRICT);

	如果 (commit_tree_extended(state->msg, state->msg_len, &tree, parents,
				 &commit, author, committer, state->sign_commit,
				 NULL))
		die(_("failed to write commit object"));

	reflog_msg = getenv("GIT_REFLOG_ACTION");
	如果 (!reflog_msg)
		reflog_msg = "am";

	strbuf_addf(&sb, "%s: %.*s", reflog_msg, linelen(state->msg),
			state->msg);

	refs_update_ref(get_main_ref_store(the_repository), sb.buf, "HEAD",
			&commit, old_oid, 0,
			UPDATE_REFS_DIE_ON_ERR);

	如果 (state->rebasing) {
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
static void validate_resume_state(const struct am_state *state)
{
	如果 (!state->msg)
		die(_("cannot resume: %s does not exist."),
			am_path(state, "final-commit"));

	如果 (!state->author_name || !state->author_email || !state->author_date)
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
static 整数 do_interactive(struct am_state *state)
{
	assert(state->msg);

	为 (;;) {
		char reply[64];

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
		如果 (!fgets(reply, sizeof(reply), stdin))
			die("unable to read from stdin; aborting");

		如果 (*reply == 'y' || *reply == 'Y') {
			返回 0;
		} 否则 如果 (*reply == 'a' || *reply == 'A') {
			state->interactive = 0;
			返回 0;
		} 否则 如果 (*reply == 'n' || *reply == 'N') {
			返回 1;
		} 否则 如果 (*reply == 'e' || *reply == 'E') {
			struct strbuf msg = STRBUF_INIT;

			如果 (!launch_editor(am_path(state, "final-commit"), &msg, NULL)) {
				free(state->msg);
				state->msg = strbuf_detach(&msg, &state->msg_len);
			}
			strbuf_release(&msg);
		} 否则 如果 (*reply == 'v' || *reply == 'V') {
			const char *pager = git_pager(the_repository, 1);
			struct child_process cp = CHILD_PROCESS_INIT;

			如果 (!pager)
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
static void am_run(struct am_state *state, 整数 resume)
{
	struct strbuf sb = STRBUF_INIT;

	unlink(am_path(state, "dirtyindex"));

	如果 (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 NULL, NULL, NULL) < 0)
		die(_("unable to write index file"));

	如果 (repo_index_has_changes(the_repository, NULL, &sb)) {
		write_state_bool(state, "dirtyindex", 1);
		die(_("Dirty index: cannot apply patches (dirty: %s)"), sb.buf);
	}

	strbuf_release(&sb);

	当 (state->cur <= state->last) {
		const char *mail = am_path(state, msgnum(state));
		整数 apply_status;
		整数 to_keep;

		reset_ident_date();

		如果 (!file_exists(mail))
			goto next;

		如果 (resume) {
			validate_resume_state(state);
		} 否则 {
			整数 skip;

			如果 (state->rebasing)
				skip = parse_mail_rebase(state, mail);
			否则
				skip = parse_mail(state, mail);

			如果 (skip)
				goto next; /* mail should be skipped */

			如果 (state->signoff)
				am_append_signoff(state);

			write_author_script(state);
			write_commit_msg(state);
		}

		如果 (state->interactive && do_interactive(state))
			goto next;

		to_keep = 0;
		如果 (is_empty_or_missing_file(am_path(state, "patch"))) {
			switch (state->empty_type) {
			case DROP_EMPTY_COMMIT:
				say(state, stdout, _("Skipping: %.*s"), linelen(state->msg), state->msg);
				goto next;
				break;
			case KEEP_EMPTY_COMMIT:
				to_keep = 1;
				say(state, stdout, _("Creating an empty commit: %.*s"),
					linelen(state->msg), state->msg);
				break;
			case STOP_ON_EMPTY_COMMIT:
				printf_ln(_("Patch is empty."));
				die_user_resolve(state);
				break;
			}
		}

		如果 (run_applypatch_msg_hook(state))
			exit(1);
		如果 (to_keep)
			goto commit;

		say(state, stdout, _("Applying: %.*s"), linelen(state->msg), state->msg);

		apply_status = run_apply(state, NULL);

		如果 (apply_status && state->threeway) {
			struct strbuf sb = STRBUF_INIT;

			strbuf_addstr(&sb, am_path(state, "patch-merge-index"));
			apply_status = fall_back_threeway(state, sb.buf);
			strbuf_release(&sb);

			/*
			 * Applying the patch to an earlier tree and merging
			 * the result may have produced the same tree as ours.
			 */
			如果 (!apply_status &&
			    !repo_index_has_changes(the_repository, NULL, NULL)) {
				say(state, stdout, _("No changes -- Patch already applied."));
				goto next;
			}
		}

		如果 (apply_status) {
			printf_ln(_("Patch failed at %s %.*s"), msgnum(state),
				linelen(state->msg), state->msg);

			如果 (advice_enabled(ADVICE_AM_WORK_DIR))
				advise(_("Use 'git am --show-current-patch=diff' to see the failed patch"));

			die_user_resolve(state);
		}

commit:
		do_commit(state);

next:
		am_next(state);

		如果 (resume)
			am_load(state);
		resume = 0;
	}

	如果 (!is_empty_or_missing_file(am_path(state, "rewritten"))) {
		assert(state->rebasing);
		copy_notes_for_rebase(state);
		run_post_rewrite_hook(state);
	}

	/*
	 * In rebasing mode, it's up to the caller to take care of
	 * housekeeping.
	 */
	如果 (!state->rebasing) {
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
static void am_resolve(struct am_state *state, 整数 allow_empty)
{
	validate_resume_state(state);

	say(state, stdout, _("Applying: %.*s"), linelen(state->msg), state->msg);

	如果 (!repo_index_has_changes(the_repository, NULL, NULL)) {
		如果 (allow_empty && is_empty_or_missing_file(am_path(state, "patch"))) {
			printf_ln(_("No changes - recorded it as an empty commit."));
		} 否则 {
			printf_ln(_("No changes - did you forget to use 'git add'?\n"
				    "If there is nothing left to stage, chances are that something else\n"
				    "already introduced the same changes; you might want to skip this patch."));
			die_user_resolve(state);
		}
	}

	如果 (unmerged_index(the_repository->index)) {
		printf_ln(_("You still have unmerged paths in your index.\n"
			"You should 'git add' each file with resolved conflicts to mark them as such.\n"
			"You might run `git rm` on a file to accept \"deleted by them\" for it."));
		die_user_resolve(state);
	}

	如果 (state->interactive) {
		write_index_patch(state);
		如果 (do_interactive(state))
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
static 整数 fast_forward_to(struct tree *head, struct tree *remote, 整数 reset)
{
	struct lock_file lock_file = LOCK_INIT;
	struct unpack_trees_options opts;
	struct tree_desc t[2];

	如果 (repo_parse_tree(the_repository, head) || repo_parse_tree(the_repository, remote))
		返回 -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	refresh_index(the_repository->index, REFRESH_QUIET, NULL, NULL, NULL);

	memset(&opts, 0, sizeof(opts));
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

	如果 (unpack_trees(2, t, &opts)) {
		rollback_lock_file(&lock_file);
		返回 -1;
	}

	如果 (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	返回 0;
}

/**
 * Merges a tree into the index. The index's stat info will take precedence
 * over the merged tree's. Returns 0 on success, -1 on failure.
 */
static 整数 merge_tree(struct tree *tree)
{
	struct lock_file lock_file = LOCK_INIT;
	struct unpack_trees_options opts;
	struct tree_desc t[1];

	如果 (repo_parse_tree(the_repository, tree))
		返回 -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, sizeof(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.merge = 1;
	opts.fn = oneway_merge;
	init_tree_desc(&t[0], &tree->object.oid, tree->buffer, tree->size);

	如果 (unpack_trees(1, t, &opts)) {
		rollback_lock_file(&lock_file);
		返回 -1;
	}

	如果 (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	返回 0;
}

/**
 * Clean the index without touching entries that are not modified between
 * `head` and `remote`.
 */
static 整数 clean_index(const struct object_id *head, const struct object_id *remote)
{
	struct tree *head_tree, *remote_tree, *index_tree;
	struct object_id index;

	head_tree = repo_parse_tree_indirect(the_repository, head);
	如果 (!head_tree)
		返回 error(_("Could not parse object '%s'."), oid_to_hex(head));

	remote_tree = repo_parse_tree_indirect(the_repository, remote);
	如果 (!remote_tree)
		返回 error(_("Could not parse object '%s'."), oid_to_hex(remote));

	repo_read_index_unmerged(the_repository);

	如果 (fast_forward_to(head_tree, head_tree, 1))
		返回 -1;

	如果 (write_index_as_tree(&index, the_repository->index,
				repo_get_index_file(the_repository),
				0, NULL))
		返回 -1;

	index_tree = repo_parse_tree_indirect(the_repository, &index);
	如果 (!index_tree)
		返回 error(_("Could not parse object '%s'."), oid_to_hex(&index));

	如果 (fast_forward_to(index_tree, remote_tree, 0))
		返回 -1;

	如果 (merge_tree(remote_tree))
		返回 -1;

	remove_branch_state(the_repository, 0);

	返回 0;
}

/**
 * Resets rerere's merge resolution metadata.
 */
static void am_rerere_clear(void)
{
	struct string_list merge_rr = STRING_LIST_INIT_DUP;
	rerere_clear(the_repository, &merge_rr);
	string_list_clear(&merge_rr, 1);
}

/**
 * Resume the current am session by skipping the current patch.
 */
static void am_skip(struct am_state *state)
{
	struct object_id head;

	am_rerere_clear();

	如果 (repo_get_oid(the_repository, "HEAD", &head))
		oidcpy(&head, the_hash_algo->empty_tree);

	如果 (clean_index(&head, &head))
		die(_("failed to clean index"));

	如果 (state->rebasing) {
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
static 整数 safe_to_abort(const struct am_state *state)
{
	struct strbuf sb = STRBUF_INIT;
	struct object_id abort_safety, head;

	如果 (file_exists(am_path(state, "dirtyindex")))
		返回 0;

	如果 (read_state_file(&sb, state, "abort-safety", 1) > 0) {
		如果 (get_oid_hex(sb.buf, &abort_safety))
			die(_("could not parse %s"), am_path(state, "abort-safety"));
	} 否则
		oidclr(&abort_safety, the_repository->hash_algo);
	strbuf_release(&sb);

	如果 (repo_get_oid(the_repository, "HEAD", &head))
		oidclr(&head, the_repository->hash_algo);

	如果 (oideq(&head, &abort_safety))
		返回 1;

	warning(_("You seem to have moved HEAD since the last 'am' failure.\n"
		"Not rewinding to ORIG_HEAD"));

	返回 0;
}

/**
 * Aborts the current am session if it is safe to do so.
 */
static void am_abort(struct am_state *state)
{
	struct object_id curr_head, orig_head;
	整数 has_curr_head, has_orig_head;
	char *curr_branch;

	如果 (!safe_to_abort(state)) {
		am_destroy(state);
		返回;
	}

	am_rerere_clear();

	curr_branch = refs_resolve_refdup(get_main_ref_store(the_repository),
					  "HEAD", 0, &curr_head, NULL);
	has_curr_head = curr_branch && !is_null_oid(&curr_head);
	如果 (!has_curr_head)
		oidcpy(&curr_head, the_hash_algo->empty_tree);

	has_orig_head = !repo_get_oid(the_repository, "ORIG_HEAD", &orig_head);
	如果 (!has_orig_head)
		oidcpy(&orig_head, the_hash_algo->empty_tree);

	如果 (clean_index(&curr_head, &orig_head))
		die(_("failed to clean index"));

	如果 (has_orig_head)
		refs_update_ref(get_main_ref_store(the_repository),
				"am --abort", "HEAD", &orig_head,
				has_curr_head ? &curr_head : NULL, 0,
				UPDATE_REFS_DIE_ON_ERR);
	否则 如果 (curr_branch)
		refs_delete_ref(get_main_ref_store(the_repository), NULL,
				curr_branch, NULL, REF_NO_DEREF);

	free(curr_branch);
	am_destroy(state);
}

static 整数 show_patch(struct am_state *state, enum resume_type resume_mode)
{
	struct strbuf sb = STRBUF_INIT;
	const char *patch_path;
	整数 len;

	如果 (!is_null_oid(&state->orig_commit)) {
		struct child_process cmd = CHILD_PROCESS_INIT;

		strvec_pushl(&cmd.args, "show", oid_to_hex(&state->orig_commit),
			     "--", NULL);
		cmd.git_cmd = 1;
		返回 run_command(&cmd);
	}

	switch (resume_mode) {
	case RESUME_SHOW_PATCH_RAW:
		patch_path = am_path(state, msgnum(state));
		break;
	case RESUME_SHOW_PATCH_DIFF:
		patch_path = am_path(state, "patch");
		break;
	default:
		BUG("invalid mode for --show-current-patch");
	}

	len = strbuf_read_file(&sb, patch_path, 0);
	如果 (len < 0)
		die_errno(_("failed to read '%s'"), patch_path);

	setup_pager(the_repository);
	write_in_full(1, sb.buf, sb.len);
	strbuf_release(&sb);
	返回 0;
}

/**
 * parse_options() callback that validates and sets opt->value to the
 * PATCH_FORMAT_* enum value corresponding to `arg`.
 */
static 整数 parse_opt_patchformat(const struct option *opt, const char *arg, 整数 unset)
{
	整数 *opt_value = opt->value;

	如果 (unset)
		*opt_value = PATCH_FORMAT_UNKNOWN;
	否则 如果 (!strcmp(arg, "mbox"))
		*opt_value = PATCH_FORMAT_MBOX;
	否则 如果 (!strcmp(arg, "stgit"))
		*opt_value = PATCH_FORMAT_STGIT;
	否则 如果 (!strcmp(arg, "stgit-series"))
		*opt_value = PATCH_FORMAT_STGIT_SERIES;
	否则 如果 (!strcmp(arg, "hg"))
		*opt_value = PATCH_FORMAT_HG;
	否则 如果 (!strcmp(arg, "mboxrd"))
		*opt_value = PATCH_FORMAT_MBOXRD;
	/*
	 * Please update $__git_patchformat in git-completion.bash
	 * when you add new options
	 */
	否则
		返回 error(_("invalid value for '%s': '%s'"),
			     "--patch-format", arg);
	返回 0;
}

static 整数 parse_opt_show_current_patch(const struct option *opt, const char *arg, 整数 unset)
{
	整数 *opt_value = opt->value;

	BUG_ON_OPT_NEG(unset);

	如果 (!arg)
		*opt_value = opt->defval;
	否则 如果 (!strcmp(arg, "raw"))
		*opt_value = RESUME_SHOW_PATCH_RAW;
	否则 如果 (!strcmp(arg, "diff"))
		*opt_value = RESUME_SHOW_PATCH_DIFF;
	/*
	 * Please update $__git_showcurrentpatch in git-completion.bash
	 * when you add new options
	 */
	否则
		返回 error(_("invalid value for '%s': '%s'"),
			     "--show-current-patch", arg);
	返回 0;
}

整数 cmd_am(整数 argc,
	   const char **argv,
	   const char *prefix,
	   struct repository *repo UNUSED)
{
	struct am_state state;
	整数 binary = -1;
	整数 keep_cr = -1;
	整数 patch_format = PATCH_FORMAT_UNKNOWN;
	enum resume_type resume_mode = RESUME_FALSE;
	整数 in_progress;
	整数 ret = 0;

	const char * const usage[] = {
		N_("git am [<options>] [(<mbox> | <Maildir>)...]"),
		N_("git am [<options>] (--continue | --skip | --abort)"),
		NULL
	};

	struct option options[] = {
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
			.precision = sizeof(resume_mode),
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
	如果 (in_progress)
		am_load(&state);

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	如果 (binary >= 0)
		fprintf_ln(stderr, _("The -b/--binary option has been a no-op for long time, and\n"
				"it will be removed. Please do not use it anymore."));

	/* Ensure a valid committer ident can be constructed */
	git_committer_info(IDENT_STRICT);

	如果 (repo_read_index_preload(the_repository, NULL, 0) < 0)
		die(_("failed to read the index"));

	如果 (in_progress) {
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
		如果 (argc || (resume_mode == RESUME_FALSE && !isatty(0)))
			die(_("previous rebase directory %s still exists but mbox given."),
				state.dir);

		如果 (resume_mode == RESUME_FALSE)
			resume_mode = RESUME_APPLY;

		如果 (state.signoff == SIGNOFF_EXPLICIT)
			am_append_signoff(&state);
	} 否则 {
		struct strvec paths = STRVEC_INIT;
		整数 i;

		/*
		 * Handle stray state directory in the independent-run case. In
		 * the --rebasing case, it is up to the caller to take care of
		 * stray directories.
		 */
		如果 (file_exists(state.dir) && !state.rebasing) {
			如果 (resume_mode == RESUME_ABORT || resume_mode == RESUME_QUIT) {
				am_destroy(&state);
				am_state_release(&state);
				返回 0;
			}

			die(_("Stray %s directory found.\n"
				"Use \"git am --abort\" to remove it."),
				state.dir);
		}

		如果 (resume_mode)
			die(_("Resolve operation not in progress, we are not resuming."));

		为 (i = 0; i < argc; i++) {
			如果 (is_absolute_path(argv[i]) || !prefix)
				strvec_push(&paths, argv[i]);
			否则
				strvec_push(&paths, mkpath("%s/%s", prefix, argv[i]));
		}

		如果 (state.interactive && !paths.nr)
			die(_("interactive mode requires patches on the command line"));

		am_setup(&state, patch_format, paths.v, keep_cr);

		strvec_clear(&paths);
	}

	switch (resume_mode) {
	case RESUME_FALSE:
		am_run(&state, 0);
		break;
	case RESUME_APPLY:
		am_run(&state, 1);
		break;
	case RESUME_RESOLVED:
	case RESUME_ALLOW_EMPTY:
		am_resolve(&state, resume_mode == RESUME_ALLOW_EMPTY ? 1 : 0);
		break;
	case RESUME_SKIP:
		am_skip(&state);
		break;
	case RESUME_ABORT:
		am_abort(&state);
		break;
	case RESUME_QUIT:
		am_rerere_clear();
		am_destroy(&state);
		break;
	case RESUME_SHOW_PATCH_RAW:
	case RESUME_SHOW_PATCH_DIFF:
		ret = show_patch(&state, resume_mode);
		break;
	default:
		BUG("invalid resume value");
	}

	am_state_release(&state);

	返回 ret;
}
