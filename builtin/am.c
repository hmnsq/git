/*
 * Builtin "git am"
 *
 * Based on git-am.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "advice.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "editor.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "hook.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "tempfile.h"
#அந்தர்பாவ "lockfile.h"
#அந்தர்பாவ "cache-tree.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "unpack-trees.h"
#அந்தர்பாவ "branch.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "preload-index.h"
#அந்தர்பாவ "sequencer.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "merge-ort-wrappers.h"
#அந்தர்பாவ "log-tree.h"
#அந்தர்பாவ "notes-utils.h"
#அந்தர்பாவ "rerere.h"
#அந்தர்பாவ "mailinfo.h"
#அந்தர்பாவ "apply.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "pager.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "pretty.h"

/**
 * Returns the length of the first line of msg.
 */
நிலையான முழு linelen(மாறிலி எழுத்து *செய்தி)
{
	திரும்பு strchrnul(செய்தி, '\n') - செய்தி;
}

/**
 * Returns true if `str` consists of only whitespace, false otherwise.
 */
நிலையான முழு str_isspace(மாறிலி எழுத்து *str)
{
	க்கு (; *str; str++)
		எனில் (!isspace(*str))
			திரும்பு 0;

	திரும்பு 1;
}

எண்ணிக்கை patch_format {
	PATCH_FORMAT_UNKNOWN = 0,
	PATCH_FORMAT_MBOX,
	PATCH_FORMAT_STGIT,
	PATCH_FORMAT_STGIT_SERIES,
	PATCH_FORMAT_HG,
	PATCH_FORMAT_MBOXRD
};

எண்ணிக்கை keep_type {
	KEEP_FALSE = 0,
	KEEP_TRUE,      /* pass -k flag to git-mailinfo */
	KEEP_NON_PATCH  /* pass -b flag to git-mailinfo */
};

எண்ணிக்கை scissors_type {
	SCISSORS_UNSET = -1,
	SCISSORS_FALSE = 0,  /* pass --no-scissors to git-mailinfo */
	SCISSORS_TRUE        /* pass --scissors to git-mailinfo */
};

எண்ணிக்கை signoff_type {
	SIGNOFF_FALSE = 0,
	SIGNOFF_TRUE = 1,
	SIGNOFF_EXPLICIT /* --signoff was set on the command-line */
};

எண்ணிக்கை resume_type {
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

எண்ணிக்கை empty_action {
	STOP_ON_EMPTY_COMMIT = 0,  /* output errors and stop in the middle of an am session */
	DROP_EMPTY_COMMIT,         /* skip with a notice message, unless "--quiet" has been passed */
	KEEP_EMPTY_COMMIT,         /* keep recording as empty commits */
};

கட்டமைப்பு am_state {
	/* state directory path */
	எழுத்து *dir;

	/* current and last patch numbers, 1-indexed */
	முழு cur;
	முழு last;

	/* commit metadata and message */
	எழுத்து *author_name;
	எழுத்து *author_email;
	எழுத்து *author_date;
	எழுத்து *செய்தி;
	size_t செய்தி_len;

	/* when --rebasing, records the original commit the patch came from */
	கட்டமைப்பு object_id orig_commit;

	/* number of digits in patch filename */
	முழு prec;

	/* various operating modes and command line options */
	முழு interactive;
	முழு no_verify;
	முழு threeway;
	முழு quiet;
	முழு signoff; /* enum signoff_type */
	முழு utf8;
	முழு keep; /* enum keep_type */
	முழு message_id;
	முழு scissors; /* enum scissors_type */
	முழு quoted_cr; /* enum quoted_cr_action */
	முழு empty_type; /* enum empty_action */
	கட்டமைப்பு strvec git_apply_opts;
	மாறிலி எழுத்து *resolvemsg;
	முழு committer_date_is_author_date;
	முழு ignore_date;
	முழு allow_rerere_autoupdate;
	மாறிலி எழுத்து *sign_commit;
	முழு rebasing;
};

/**
 * Initializes am_state with the default values.
 */
நிலையான வெறுமை am_state_init(கட்டமைப்பு am_state *state)
{
	முழு gpgsign;

	memset(state, 0, அளவு(*state));

	state->dir = repo_git_path(the_repository, "rebase-apply");

	state->prec = 4;

	repo_config_get_bool(the_repository, "am.threeway", &state->threeway);

	state->utf8 = 1;

	repo_config_get_bool(the_repository, "am.messageid", &state->message_id);

	state->scissors = SCISSORS_UNSET;
	state->quoted_cr = quoted_cr_unset;

	strvec_init(&state->git_apply_opts);

	எனில் (!repo_config_get_bool(the_repository, "commit.gpgsign", &gpgsign))
		state->sign_commit = gpgsign ? "" : வெற்று;
}

/**
 * Releases memory allocated by an am_state.
 */
நிலையான வெறுமை am_state_release(கட்டமைப்பு am_state *state)
{
	விடுவி(state->dir);
	விடுவி(state->author_name);
	விடுவி(state->author_email);
	விடுவி(state->author_date);
	விடுவி(state->செய்தி);
	strvec_clear(&state->git_apply_opts);
}

நிலையான முழு am_option_parse_quoted_cr(மாறிலி கட்டமைப்பு option *opt,
				     மாறிலி எழுத்து *arg, முழு unset)
{
	BUG_ON_OPT_NEG(unset);

	எனில் (mailinfo_parse_quoted_cr_action(arg, opt->மதிப்பு) != 0)
		திரும்பு error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	திரும்பு 0;
}

நிலையான முழு am_option_parse_empty(மாறிலி கட்டமைப்பு option *opt,
				     மாறிலி எழுத்து *arg, முழு unset)
{
	முழு *opt_மதிப்பு = opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);

	எனில் (!strcmp(arg, "stop"))
		*opt_மதிப்பு = STOP_ON_EMPTY_COMMIT;
	இல்லையெனில் எனில் (!strcmp(arg, "drop"))
		*opt_மதிப்பு = DROP_EMPTY_COMMIT;
	இல்லையெனில் எனில் (!strcmp(arg, "keep"))
		*opt_மதிப்பு = KEEP_EMPTY_COMMIT;
	இல்லையெனில்
		திரும்பு error(_("invalid value for '%s': '%s'"), "--empty", arg);

	திரும்பு 0;
}

/**
 * Returns path relative to the am_state directory.
 */
நிலையான உள்வரி மாறிலி எழுத்து *am_path(மாறிலி கட்டமைப்பு am_state *state, மாறிலி எழுத்து *path)
{
	திரும்பு mkpath("%s/%s", state->dir, path);
}

/**
 * For convenience to call write_file()
 */
நிலையான வெறுமை write_state_text(மாறிலி கட்டமைப்பு am_state *state,
			     மாறிலி எழுத்து *name, மாறிலி எழுத்து *string)
{
	write_file(am_path(state, name), "%s", string);
}

நிலையான வெறுமை write_state_மொத்தம்(மாறிலி கட்டமைப்பு am_state *state,
			      மாறிலி எழுத்து *name, முழு மதிப்பு)
{
	write_file(am_path(state, name), "%d", மதிப்பு);
}

நிலையான வெறுமை write_state_bool(மாறிலி கட்டமைப்பு am_state *state,
			     மாறிலி எழுத்து *name, முழு மதிப்பு)
{
	write_state_text(state, name, மதிப்பு ? "t" : "f");
}

/**
 * If state->quiet is false, calls fprintf(fp, fmt, ...), and appends a newline
 * at the end.
 */
__attribute__((format (printf, 3, 4)))
நிலையான வெறுமை say(மாறிலி கட்டமைப்பு am_state *state, FILE *fp, மாறிலி எழுத்து *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	எனில் (!state->quiet) {
		vfprintf(fp, fmt, ap);
		putc('\n', fp);
	}
	va_end(ap);
}

/**
 * Returns 1 if there is an am session in progress, 0 otherwise.
 */
நிலையான முழு am_in_progress(மாறிலி கட்டமைப்பு am_state *state)
{
	கட்டமைப்பு stat st;

	எனில் (lstat(state->dir, &st) < 0 || !S_ISDIR(st.st_mode))
		திரும்பு 0;
	எனில் (lstat(am_path(state, "last"), &st) || !S_ISREG(st.st_mode))
		திரும்பு 0;
	எனில் (lstat(am_path(state, "next"), &st) || !S_ISREG(st.st_mode))
		திரும்பு 0;
	திரும்பு 1;
}

/**
 * Reads the contents of `file` in the `state` directory into `sb`. Returns the
 * number of bytes read on success, -1 if the file does not exist. If `trim` is
 * set, trailing whitespace will be removed.
 */
நிலையான முழு read_state_file(கட்டமைப்பு strbuf *sb, மாறிலி கட்டமைப்பு am_state *state,
			மாறிலி எழுத்து *file, முழு trim)
{
	strbuf_reset(sb);

	எனில் (strbuf_read_file(sb, am_path(state, file), 0) >= 0) {
		எனில் (trim)
			strbuf_trim(sb);

		திரும்பு sb->len;
	}

	எனில் (errno == ENOENT)
		திரும்பு -1;

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
நிலையான முழு read_am_author_script(கட்டமைப்பு am_state *state)
{
	மாறிலி எழுத்து *filename = am_path(state, "author-script");

	ப்ரதிஜ்ஞா(!state->author_name);
	ப்ரதிஜ்ஞா(!state->author_email);
	ப்ரதிஜ்ஞா(!state->author_date);

	திரும்பு read_author_script(filename, &state->author_name,
				  &state->author_email, &state->author_date, 1);
}

/**
 * Saves state->author_name, state->author_email and state->author_date in the
 * state directory's "author-script" file.
 */
நிலையான வெறுமை write_author_script(மாறிலி கட்டமைப்பு am_state *state)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;

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
நிலையான முழு read_commit_செய்தி(கட்டமைப்பு am_state *state)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;

	ப்ரதிஜ்ஞா(!state->செய்தி);

	எனில் (read_state_file(&sb, state, "final-commit", 0) < 0) {
		strbuf_release(&sb);
		திரும்பு -1;
	}

	state->செய்தி = strbuf_detach(&sb, &state->செய்தி_len);
	திரும்பு 0;
}

/**
 * Saves state->msg in the state directory's "final-commit" file.
 */
நிலையான வெறுமை write_commit_செய்தி(மாறிலி கட்டமைப்பு am_state *state)
{
	மாறிலி எழுத்து *filename = am_path(state, "final-commit");
	write_file_buf(filename, state->செய்தி, state->செய்தி_len);
}

/**
 * Loads state from disk.
 */
நிலையான வெறுமை am_load(கட்டமைப்பு am_state *state)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;

	எனில் (read_state_file(&sb, state, "next", 1) < 0)
		BUG("state file 'next' does not exist");
	state->cur = strtol(sb.buf, வெற்று, 10);

	எனில் (read_state_file(&sb, state, "last", 1) < 0)
		BUG("state file 'last' does not exist");
	state->last = strtol(sb.buf, வெற்று, 10);

	எனில் (read_am_author_script(state) < 0)
		die(_("could not parse author script"));

	read_commit_செய்தி(state);

	எனில் (read_state_file(&sb, state, "original-commit", 1) < 0)
		oidclr(&state->orig_commit, the_repository->hash_algo);
	இல்லையெனில் எனில் (get_oid_hex(sb.buf, &state->orig_commit) < 0)
		die(_("could not parse %s"), am_path(state, "original-commit"));

	read_state_file(&sb, state, "threeway", 1);
	state->threeway = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "quiet", 1);
	state->quiet = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "sign", 1);
	state->signoff = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "utf8", 1);
	state->utf8 = !strcmp(sb.buf, "t");

	எனில் (file_exists(am_path(state, "rerere-autoupdate"))) {
		read_state_file(&sb, state, "rerere-autoupdate", 1);
		state->allow_rerere_autoupdate = strcmp(sb.buf, "t") ?
			RERERE_NOAUTOUPDATE : RERERE_AUTOUPDATE;
	} இல்லையெனில் {
		state->allow_rerere_autoupdate = 0;
	}

	read_state_file(&sb, state, "keep", 1);
	எனில் (!strcmp(sb.buf, "t"))
		state->keep = KEEP_TRUE;
	இல்லையெனில் எனில் (!strcmp(sb.buf, "b"))
		state->keep = KEEP_NON_PATCH;
	இல்லையெனில்
		state->keep = KEEP_FALSE;

	read_state_file(&sb, state, "messageid", 1);
	state->message_id = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "scissors", 1);
	எனில் (!strcmp(sb.buf, "t"))
		state->scissors = SCISSORS_TRUE;
	இல்லையெனில் எனில் (!strcmp(sb.buf, "f"))
		state->scissors = SCISSORS_FALSE;
	இல்லையெனில்
		state->scissors = SCISSORS_UNSET;

	read_state_file(&sb, state, "quoted-cr", 1);
	எனில் (!*sb.buf)
		state->quoted_cr = quoted_cr_unset;
	இல்லையெனில் எனில் (mailinfo_parse_quoted_cr_action(sb.buf, &state->quoted_cr) != 0)
		die(_("could not parse %s"), am_path(state, "quoted-cr"));

	read_state_file(&sb, state, "apply-opt", 1);
	strvec_clear(&state->git_apply_opts);
	எனில் (sq_dequote_to_strvec(sb.buf, &state->git_apply_opts) < 0)
		die(_("could not parse %s"), am_path(state, "apply-opt"));

	state->rebasing = !!file_exists(am_path(state, "rebasing"));

	strbuf_release(&sb);
}

/**
 * Removes the am_state directory, forcefully terminating the current am
 * session.
 */
நிலையான வெறுமை am_destroy(மாறிலி கட்டமைப்பு am_state *state)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;

	strbuf_addstr(&sb, state->dir);
	remove_dir_recursively(&sb, 0);
	strbuf_release(&sb);
}

/**
 * Runs applypatch-msg hook. Returns its exit code.
 */
நிலையான முழு run_applypatch_செய்தி_hook(கட்டமைப்பு am_state *state)
{
	முழு ret = 0;

	ப்ரதிஜ்ஞா(state->செய்தி);

	எனில் (!state->no_verify) {
		கட்டமைப்பு run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;
		strvec_தள்ளு(&opt.args, am_path(state, "final-commit"));
		ret = run_hooks_opt(the_repository, "applypatch-msg", &opt);
	}

	எனில் (!ret) {
		FREE_AND_NULL(state->செய்தி);
		எனில் (read_commit_செய்தி(state) < 0)
			die(_("'%s' was deleted by the applypatch-msg hook"),
				am_path(state, "final-commit"));
	}

	திரும்பு ret;
}

/**
 * Runs post-rewrite hook. Returns it exit code.
 */
நிலையான முழு run_post_rewrite_hook(மாறிலி கட்டமைப்பு am_state *state)
{
	கட்டமைப்பு run_hooks_opt opt = RUN_HOOKS_OPT_INIT;

	strvec_தள்ளு(&opt.args, "rebase");
	opt.path_to_stdin = am_path(state, "rewritten");

	திரும்பு run_hooks_opt(the_repository, "post-rewrite", &opt);
}

/**
 * Reads the state directory's "rewritten" file, and copies notes from the old
 * commits listed in the file to their rewritten commits.
 *
 * Returns 0 on success, -1 on failure.
 */
நிலையான முழு copy_notes_for_rebase(மாறிலி கட்டமைப்பு am_state *state)
{
	கட்டமைப்பு notes_rewrite_cfg *c;
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	மாறிலி எழுத்து *invalid_வரி = _("Malformed input line: '%s'.");
	மாறிலி எழுத்து *செய்தி = "Notes added by 'git rebase'";
	FILE *fp;
	முழு ret = 0;

	ப்ரதிஜ்ஞா(state->rebasing);

	c = init_copy_notes_for_rewrite("rebase");
	எனில் (!c)
		திரும்பு 0;

	fp = xfopen(am_path(state, "rewritten"), "r");

	போது (!strbuf_getline_lf(&sb, fp)) {
		கட்டமைப்பு object_id from_obj, to_obj;
		மாறிலி எழுத்து *p;

		எனில் (sb.len != the_hash_algo->hexsz * 2 + 1) {
			ret = error(invalid_வரி, sb.buf);
			செல் finish;
		}

		எனில் (parse_oid_hex(sb.buf, &from_obj, &p)) {
			ret = error(invalid_வரி, sb.buf);
			செல் finish;
		}

		எனில் (*p != ' ') {
			ret = error(invalid_வரி, sb.buf);
			செல் finish;
		}

		எனில் (get_oid_hex(p + 1, &to_obj)) {
			ret = error(invalid_வரி, sb.buf);
			செல் finish;
		}

		எனில் (copy_note_for_rewrite(c, &from_obj, &to_obj))
			ret = error(_("Failed to copy notes from '%s' to '%s'"),
					oid_to_hex(&from_obj), oid_to_hex(&to_obj));
	}

finish:
	finish_copy_notes_for_rewrite(the_repository, c, செய்தி);
	fclose(fp);
	strbuf_release(&sb);
	திரும்பு ret;
}

/**
 * Determines if the file looks like a piece of RFC2822 mail by grabbing all
 * non-indented lines and checking if they look like they begin with valid
 * header field names.
 *
 * Returns 1 if the file looks like a piece of mail, 0 otherwise.
 */
நிலையான முழு is_mail(FILE *fp)
{
	மாறிலி எழுத்து *header_regex = "^[!-9;-~]+:";
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	regex_t regex;
	முழு ret = 1;

	எனில் (fseek(fp, 0L, SEEK_SET))
		die_errno(_("fseek failed"));

	எனில் (regcomp(&regex, header_regex, REG_NOSUB | REG_EXTENDED))
		die("invalid pattern: %s", header_regex);

	போது (!strbuf_getline(&sb, fp)) {
		எனில் (!sb.len)
			நிறுத்து; /* End of header */

		/* Ignore indented folded lines */
		எனில் (*sb.buf == '\t' || *sb.buf == ' ')
			தொடர்;

		/* It's a header if it matches header_regex */
		எனில் (regexec(&regex, sb.buf, 0, வெற்று, 0)) {
			ret = 0;
			செல் done;
		}
	}

done:
	regfree(&regex);
	strbuf_release(&sb);
	திரும்பு ret;
}

/**
 * Attempts to detect the patch_format of the patches contained in `paths`,
 * returning the PATCH_FORMAT_* enum value. Returns PATCH_FORMAT_UNKNOWN if
 * detection fails.
 */
நிலையான முழு detect_patch_format(மாறிலி எழுத்து **paths)
{
	எண்ணிக்கை patch_format ret = PATCH_FORMAT_UNKNOWN;
	கட்டமைப்பு strbuf l1 = STRBUF_INIT;
	கட்டமைப்பு strbuf l2 = STRBUF_INIT;
	கட்டமைப்பு strbuf l3 = STRBUF_INIT;
	FILE *fp;

	/*
	 * We default to mbox format if input is from stdin and for directories
	 */
	எனில் (!*paths || !strcmp(*paths, "-") || is_directory(*paths))
		திரும்பு PATCH_FORMAT_MBOX;

	/*
	 * Otherwise, check the first few lines of the first patch, starting
	 * from the first non-blank line, to try to detect its format.
	 */

	fp = xfopen(*paths, "r");

	போது (!strbuf_getline(&l1, fp)) {
		எனில் (l1.len)
			நிறுத்து;
	}

	எனில் (starts_with(l1.buf, "From ") || starts_with(l1.buf, "From: ")) {
		ret = PATCH_FORMAT_MBOX;
		செல் done;
	}

	எனில் (starts_with(l1.buf, "# This series applies on GIT commit")) {
		ret = PATCH_FORMAT_STGIT_SERIES;
		செல் done;
	}

	எனில் (!strcmp(l1.buf, "# HG changeset patch")) {
		ret = PATCH_FORMAT_HG;
		செல் done;
	}

	strbuf_getline(&l2, fp);
	strbuf_getline(&l3, fp);

	/*
	 * If the second line is empty and the third is a From, Author or Date
	 * entry, this is likely an StGit patch.
	 */
	எனில் (l1.len && !l2.len &&
		(starts_with(l3.buf, "From:") ||
		 starts_with(l3.buf, "Author:") ||
		 starts_with(l3.buf, "Date:"))) {
		ret = PATCH_FORMAT_STGIT;
		செல் done;
	}

	எனில் (l1.len && is_mail(fp)) {
		ret = PATCH_FORMAT_MBOX;
		செல் done;
	}

done:
	fclose(fp);
	strbuf_release(&l1);
	strbuf_release(&l2);
	strbuf_release(&l3);
	திரும்பு ret;
}

/**
 * Splits out individual email patches from `paths`, where each path is either
 * a mbox file or a Maildir. Returns 0 on success, -1 on failure.
 */
நிலையான முழு split_mail_mbox(கட்டமைப்பு am_state *state, மாறிலி எழுத்து **paths,
				முழு keep_cr, முழு mboxrd)
{
	கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;
	கட்டமைப்பு strbuf last = STRBUF_INIT;
	முழு ret;

	cp.git_cmd = 1;
	strvec_தள்ளு(&cp.args, "mailsplit");
	strvec_pushf(&cp.args, "-d%d", state->prec);
	strvec_pushf(&cp.args, "-o%s", state->dir);
	strvec_தள்ளு(&cp.args, "-b");
	எனில் (keep_cr)
		strvec_தள்ளு(&cp.args, "--keep-cr");
	எனில் (mboxrd)
		strvec_தள்ளு(&cp.args, "--mboxrd");
	strvec_தள்ளு(&cp.args, "--");
	strvec_pushv(&cp.args, paths);

	ret = capture_command(&cp, &last, 8);
	எனில் (ret)
		செல் exit;

	state->cur = 1;
	state->last = strtol(last.buf, வெற்று, 10);

exit:
	strbuf_release(&last);
	திரும்பு ret ? -1 : 0;
}

/**
 * Callback signature for split_mail_conv(). The foreign patch should be
 * read from `in`, and the converted patch (in RFC2822 mail format) should be
 * written to `out`. Return 0 on success, or -1 on failure.
 */
வகைவரை முழு (*mail_conv_fn)(FILE *out, FILE *in, முழு keep_cr);

/**
 * Calls `fn` for each file in `paths` to convert the foreign patch to the
 * RFC2822 mail format suitable for parsing with git-mailinfo.
 *
 * Returns 0 on success, -1 on failure.
 */
நிலையான முழு split_mail_conv(mail_conv_fn fn, கட்டமைப்பு am_state *state,
			மாறிலி எழுத்து **paths, முழு keep_cr)
{
	நிலையான மாறிலி எழுத்து *stdin_only[] = {"-", வெற்று};
	முழு i;

	எனில் (!*paths)
		paths = stdin_only;

	க்கு (i = 0; *paths; paths++, i++) {
		FILE *in, *out;
		மாறிலி எழுத்து *mail;
		முழு ret;

		எனில் (!strcmp(*paths, "-"))
			in = stdin;
		இல்லையெனில்
			in = fopen(*paths, "r");

		எனில் (!in)
			திரும்பு error_errno(_("could not open '%s' for reading"),
					   *paths);

		mail = mkpath("%s/%0*d", state->dir, state->prec, i + 1);

		out = fopen(mail, "w");
		எனில் (!out) {
			எனில் (in != stdin)
				fclose(in);
			திரும்பு error_errno(_("could not open '%s' for writing"),
					   mail);
		}

		ret = fn(out, in, keep_cr);

		fclose(out);
		எனில் (in != stdin)
			fclose(in);

		எனில் (ret)
			திரும்பு error(_("could not parse patch '%s'"), *paths);
	}

	state->cur = 1;
	state->last = i;
	திரும்பு 0;
}

/**
 * A split_mail_conv() callback that converts an StGit patch to an RFC2822
 * message suitable for parsing with git-mailinfo.
 */
நிலையான முழு stgit_patch_to_mail(FILE *out, FILE *in, முழு keep_cr UNUSED)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	முழு subject_printed = 0;

	போது (!strbuf_getline_lf(&sb, in)) {
		மாறிலி எழுத்து *str;

		எனில் (str_isspace(sb.buf))
			தொடர்;
		இல்லையெனில் எனில் (skip_prefix(sb.buf, "Author:", &str))
			fprintf(out, "From:%s\n", str);
		இல்லையெனில் எனில் (starts_with(sb.buf, "From") || starts_with(sb.buf, "Date"))
			fprintf(out, "%s\n", sb.buf);
		இல்லையெனில் எனில் (!subject_printed) {
			fprintf(out, "Subject: %s\n", sb.buf);
			subject_printed = 1;
		} இல்லையெனில் {
			fprintf(out, "\n%s\n", sb.buf);
			நிறுத்து;
		}
	}

	strbuf_reset(&sb);
	போது (strbuf_fread(&sb, 8192, in) > 0) {
		fwrite(sb.buf, 1, sb.len, out);
		strbuf_reset(&sb);
	}

	strbuf_release(&sb);
	திரும்பு 0;
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
நிலையான முழு split_mail_stgit_series(கட்டமைப்பு am_state *state, மாறிலி எழுத்து **paths,
					முழு keep_cr)
{
	மாறிலி எழுத்து *series_dir;
	எழுத்து *series_dir_buf;
	FILE *fp;
	கட்டமைப்பு strvec patches = STRVEC_INIT;
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	முழு ret;

	எனில் (!paths[0] || paths[1])
		திரும்பு error(_("Only one StGIT patch series can be applied at once"));

	series_dir_buf = xstrdup(*paths);
	series_dir = dirname(series_dir_buf);

	fp = fopen(*paths, "r");
	எனில் (!fp) {
		விடுவி(series_dir_buf);
		திரும்பு error_errno(_("could not open '%s' for reading"), *paths);
	}

	போது (!strbuf_getline_lf(&sb, fp)) {
		எனில் (*sb.buf == '#')
			தொடர்; /* skip comment lines */

		strvec_தள்ளு(&patches, mkpath("%s/%s", series_dir, sb.buf));
	}

	fclose(fp);
	strbuf_release(&sb);
	விடுவி(series_dir_buf);

	ret = split_mail_conv(stgit_patch_to_mail, state, patches.v, keep_cr);

	strvec_clear(&patches);
	திரும்பு ret;
}

/**
 * A split_patches_conv() callback that converts a mercurial patch to a RFC2822
 * message suitable for parsing with git-mailinfo.
 */
நிலையான முழு hg_patch_to_mail(FILE *out, FILE *in, முழு keep_cr UNUSED)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	முழு rc = 0;

	போது (!strbuf_getline_lf(&sb, in)) {
		மாறிலி எழுத்து *str;

		எனில் (skip_prefix(sb.buf, "# User ", &str))
			fprintf(out, "From: %s\n", str);
		இல்லையெனில் எனில் (skip_prefix(sb.buf, "# Date ", &str)) {
			timestamp_t timestamp;
			long tz, tz2;
			எழுத்து *end;

			errno = 0;
			timestamp = parse_timestamp(str, &end, 10);
			எனில் (errno) {
				rc = error(_("invalid timestamp"));
				செல் exit;
			}

			எனில் (!skip_prefix(end, " ", &str)) {
				rc = error(_("invalid Date line"));
				செல் exit;
			}

			errno = 0;
			tz = strtol(str, &end, 10);
			எனில் (errno) {
				rc = error(_("invalid timezone offset"));
				செல் exit;
			}

			எனில் (*end) {
				rc = error(_("invalid Date line"));
				செல் exit;
			}

			/*
			 * mercurial's timezone is in seconds west of UTC,
			 * however git's timezone is in hours + minutes east of
			 * UTC. Convert it.
			 */
			tz2 = labs(tz) / 3600 * 100 + labs(tz) % 3600 / 60;
			எனில் (tz > 0)
				tz2 = -tz2;

			fprintf(out, "Date: %s\n", show_date(timestamp, tz2, DATE_MODE(RFC2822)));
		} இல்லையெனில் எனில் (starts_with(sb.buf, "# ")) {
			தொடர்;
		} இல்லையெனில் {
			fprintf(out, "\n%s\n", sb.buf);
			நிறுத்து;
		}
	}

	strbuf_reset(&sb);
	போது (strbuf_fread(&sb, 8192, in) > 0) {
		fwrite(sb.buf, 1, sb.len, out);
		strbuf_reset(&sb);
	}
exit:
	strbuf_release(&sb);
	திரும்பு rc;
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
நிலையான முழு split_mail(கட்டமைப்பு am_state *state, எண்ணிக்கை patch_format patch_format,
			மாறிலி எழுத்து **paths, முழு keep_cr)
{
	எனில் (keep_cr < 0) {
		keep_cr = 0;
		repo_config_get_bool(the_repository, "am.keepcr", &keep_cr);
	}

	தேர்வு (patch_format) {
	நிலை PATCH_FORMAT_MBOX:
		திரும்பு split_mail_mbox(state, paths, keep_cr, 0);
	நிலை PATCH_FORMAT_STGIT:
		திரும்பு split_mail_conv(stgit_patch_to_mail, state, paths, keep_cr);
	நிலை PATCH_FORMAT_STGIT_SERIES:
		திரும்பு split_mail_stgit_series(state, paths, keep_cr);
	நிலை PATCH_FORMAT_HG:
		திரும்பு split_mail_conv(hg_patch_to_mail, state, paths, keep_cr);
	நிலை PATCH_FORMAT_MBOXRD:
		திரும்பு split_mail_mbox(state, paths, keep_cr, 1);
	இயல்பு:
		BUG("invalid patch_format");
	}
	திரும்பு -1;
}

/**
 * Setup a new am session for applying patches
 */
நிலையான வெறுமை am_setup(கட்டமைப்பு am_state *state, எண்ணிக்கை patch_format patch_format,
			மாறிலி எழுத்து **paths, முழு keep_cr)
{
	கட்டமைப்பு object_id curr_head;
	மாறிலி எழுத்து *str;
	கட்டமைப்பு strbuf sb = STRBUF_INIT;

	எனில் (!patch_format)
		patch_format = detect_patch_format(paths);

	எனில் (!patch_format) {
		fprintf_ln(stderr, _("Patch format detection failed."));
		die(வெற்று);
	}

	எனில் (mkdir(state->dir, 0777) < 0 && errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), state->dir);
	refs_delete_ref(get_main_ref_store(the_repository), வெற்று,
			"REBASE_HEAD", வெற்று, REF_NO_DEREF);

	எனில் (split_mail(state, patch_format, paths, keep_cr) < 0) {
		am_destroy(state);
		die(_("Failed to split patches."));
	}

	எனில் (state->rebasing)
		state->threeway = 1;

	write_state_bool(state, "threeway", state->threeway);
	write_state_bool(state, "quiet", state->quiet);
	write_state_bool(state, "sign", state->signoff);
	write_state_bool(state, "utf8", state->utf8);

	எனில் (state->allow_rerere_autoupdate)
		write_state_bool(state, "rerere-autoupdate",
			 state->allow_rerere_autoupdate == RERERE_AUTOUPDATE);

	தேர்வு (state->keep) {
	நிலை KEEP_FALSE:
		str = "f";
		நிறுத்து;
	நிலை KEEP_TRUE:
		str = "t";
		நிறுத்து;
	நிலை KEEP_NON_PATCH:
		str = "b";
		நிறுத்து;
	இயல்பு:
		BUG("invalid value for state->keep");
	}

	write_state_text(state, "keep", str);
	write_state_bool(state, "messageid", state->message_id);

	தேர்வு (state->scissors) {
	நிலை SCISSORS_UNSET:
		str = "";
		நிறுத்து;
	நிலை SCISSORS_FALSE:
		str = "f";
		நிறுத்து;
	நிலை SCISSORS_TRUE:
		str = "t";
		நிறுத்து;
	இயல்பு:
		BUG("invalid value for state->scissors");
	}
	write_state_text(state, "scissors", str);

	தேர்வு (state->quoted_cr) {
	நிலை quoted_cr_unset:
		str = "";
		நிறுத்து;
	நிலை quoted_cr_nowarn:
		str = "nowarn";
		நிறுத்து;
	நிலை quoted_cr_warn:
		str = "warn";
		நிறுத்து;
	நிலை quoted_cr_strip:
		str = "strip";
		நிறுத்து;
	இயல்பு:
		BUG("invalid value for state->quoted_cr");
	}
	write_state_text(state, "quoted-cr", str);

	sq_quote_argv(&sb, state->git_apply_opts.v);
	write_state_text(state, "apply-opt", sb.buf);

	எனில் (state->rebasing)
		write_state_text(state, "rebasing", "");
	இல்லையெனில்
		write_state_text(state, "applying", "");

	எனில் (!repo_get_oid(the_repository, "HEAD", &curr_head)) {
		write_state_text(state, "abort-safety", oid_to_hex(&curr_head));
		எனில் (!state->rebasing)
			refs_update_ref(get_main_ref_store(the_repository),
					"am", "ORIG_HEAD", &curr_head, வெற்று,
					0,
					UPDATE_REFS_DIE_ON_ERR);
	} இல்லையெனில் {
		write_state_text(state, "abort-safety", "");
		எனில் (!state->rebasing)
			refs_delete_ref(get_main_ref_store(the_repository),
					வெற்று, "ORIG_HEAD", வெற்று, 0);
	}

	/*
	 * NOTE: Since the "next" and "last" files determine if an am_state
	 * session is in progress, they should be written last.
	 */

	write_state_மொத்தம்(state, "next", state->cur);
	write_state_மொத்தம்(state, "last", state->last);

	strbuf_release(&sb);
}

/**
 * Increments the patch pointer, and cleans am_state for the application of the
 * next patch.
 */
நிலையான வெறுமை am_next(கட்டமைப்பு am_state *state)
{
	கட்டமைப்பு object_id head;

	FREE_AND_NULL(state->author_name);
	FREE_AND_NULL(state->author_email);
	FREE_AND_NULL(state->author_date);
	FREE_AND_NULL(state->செய்தி);
	state->செய்தி_len = 0;

	unlink(am_path(state, "author-script"));
	unlink(am_path(state, "final-commit"));

	oidclr(&state->orig_commit, the_repository->hash_algo);
	unlink(am_path(state, "original-commit"));
	refs_delete_ref(get_main_ref_store(the_repository), வெற்று,
			"REBASE_HEAD", வெற்று, REF_NO_DEREF);

	எனில் (!repo_get_oid(the_repository, "HEAD", &head))
		write_state_text(state, "abort-safety", oid_to_hex(&head));
	இல்லையெனில்
		write_state_text(state, "abort-safety", "");

	state->cur++;
	write_state_மொத்தம்(state, "next", state->cur);
}

/**
 * Returns the filename of the current patch email.
 */
நிலையான மாறிலி எழுத்து *msgnum(மாறிலி கட்டமைப்பு am_state *state)
{
	நிலையான கட்டமைப்பு strbuf sb = STRBUF_INIT;

	strbuf_reset(&sb);
	strbuf_addf(&sb, "%0*d", state->prec, state->cur);

	திரும்பு sb.buf;
}

/**
 * Dies with a user-friendly message on how to proceed after resolving the
 * problem. This message can be overridden with state->resolvemsg.
 */
நிலையான வெறுமை NORETURN die_user_resolve(மாறிலி கட்டமைப்பு am_state *state)
{
	எனில் (state->resolvemsg) {
		advise_if_enabled(ADVICE_MERGE_CONFLICT, "%s", state->resolvemsg);
	} இல்லையெனில் {
		மாறிலி எழுத்து *cmdline = state->interactive ? "git am -i" : "git am";
		கட்டமைப்பு strbuf sb = STRBUF_INIT;

		strbuf_addf(&sb, _("When you have resolved this problem, run \"%s --continue\".\n"), cmdline);
		strbuf_addf(&sb, _("If you prefer to skip this patch, run \"%s --skip\" instead.\n"), cmdline);

		எனில் (advice_enabled(ADVICE_AM_WORK_DIR) &&
		    is_empty_or_missing_file(am_path(state, "patch")) &&
		    !repo_index_has_changes(the_repository, வெற்று, வெற்று))
			strbuf_addf(&sb, _("To record the empty patch as an empty commit, run \"%s --allow-empty\".\n"), cmdline);

		strbuf_addf(&sb, _("To restore the original branch and stop patching, run \"%s --abort\"."), cmdline);

		advise_if_enabled(ADVICE_MERGE_CONFLICT, "%s", sb.buf);
		strbuf_release(&sb);
	}

	die(வெற்று);
}

/**
 * Appends signoff to the "msg" field of the am_state.
 */
நிலையான வெறுமை am_append_signoff(கட்டமைப்பு am_state *state)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;

	strbuf_attach(&sb, state->செய்தி, state->செய்தி_len, state->செய்தி_len + 1);
	append_signoff(&sb, 0, 0);
	state->செய்தி = strbuf_detach(&sb, &state->செய்தி_len);
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
நிலையான முழு parse_mail(கட்டமைப்பு am_state *state, மாறிலி எழுத்து *mail)
{
	FILE *fp;
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	கட்டமைப்பு strbuf செய்தி = STRBUF_INIT;
	கட்டமைப்பு strbuf author_name = STRBUF_INIT;
	கட்டமைப்பு strbuf author_date = STRBUF_INIT;
	கட்டமைப்பு strbuf author_email = STRBUF_INIT;
	முழு ret = 0;
	கட்டமைப்பு mailinfo mi;

	setup_mailinfo(the_repository, &mi);

	எனில் (state->utf8)
		mi.metainfo_charset = get_commit_output_encoding();
	இல்லையெனில்
		mi.metainfo_charset = வெற்று;

	தேர்வு (state->keep) {
	நிலை KEEP_FALSE:
		நிறுத்து;
	நிலை KEEP_TRUE:
		mi.keep_subject = 1;
		நிறுத்து;
	நிலை KEEP_NON_PATCH:
		mi.keep_non_patch_brackets_in_subject = 1;
		நிறுத்து;
	இயல்பு:
		BUG("invalid value for state->keep");
	}

	எனில் (state->message_id)
		mi.add_message_id = 1;

	தேர்வு (state->scissors) {
	நிலை SCISSORS_UNSET:
		நிறுத்து;
	நிலை SCISSORS_FALSE:
		mi.use_scissors = 0;
		நிறுத்து;
	நிலை SCISSORS_TRUE:
		mi.use_scissors = 1;
		நிறுத்து;
	இயல்பு:
		BUG("invalid value for state->scissors");
	}

	தேர்வு (state->quoted_cr) {
	நிலை quoted_cr_unset:
		நிறுத்து;
	நிலை quoted_cr_nowarn:
	நிலை quoted_cr_warn:
	நிலை quoted_cr_strip:
		mi.quoted_cr = state->quoted_cr;
		நிறுத்து;
	இயல்பு:
		BUG("invalid value for state->quoted_cr");
	}

	mi.உள்ளீடு = xfopen(mail, "r");
	mi.output = xfopen(am_path(state, "info"), "w");
	எனில் (mailinfo(&mi, am_path(state, "msg"), am_path(state, "patch")))
		die("could not parse patch");

	fclose(mi.உள்ளீடு);
	fclose(mi.output);

	எனில் (mi.format_flowed)
		warning(_("Patch sent with format=flowed; "
			  "space at the end of lines might be lost."));

	/* Extract message and author information */
	fp = xfopen(am_path(state, "info"), "r");
	போது (!strbuf_getline_lf(&sb, fp)) {
		மாறிலி எழுத்து *x;

		எனில் (skip_prefix(sb.buf, "Subject: ", &x)) {
			எனில் (செய்தி.len)
				strbuf_addch(&செய்தி, '\n');
			strbuf_addstr(&செய்தி, x);
		} இல்லையெனில் எனில் (skip_prefix(sb.buf, "Author: ", &x))
			strbuf_addstr(&author_name, x);
		இல்லையெனில் எனில் (skip_prefix(sb.buf, "Email: ", &x))
			strbuf_addstr(&author_email, x);
		இல்லையெனில் எனில் (skip_prefix(sb.buf, "Date: ", &x))
			strbuf_addstr(&author_date, x);
	}
	fclose(fp);

	/* Skip pine's internal folder data */
	எனில் (!strcmp(author_name.buf, "Mail System Internal Data")) {
		ret = 1;
		செல் finish;
	}

	strbuf_addstr(&செய்தி, "\n\n");
	strbuf_addbuf(&செய்தி, &mi.log_message);
	strbuf_stripspace(&செய்தி, வெற்று);

	ப்ரதிஜ்ஞா(!state->author_name);
	state->author_name = strbuf_detach(&author_name, வெற்று);

	ப்ரதிஜ்ஞா(!state->author_email);
	state->author_email = strbuf_detach(&author_email, வெற்று);

	ப்ரதிஜ்ஞா(!state->author_date);
	state->author_date = strbuf_detach(&author_date, வெற்று);

	ப்ரதிஜ்ஞா(!state->செய்தி);
	state->செய்தி = strbuf_detach(&செய்தி, &state->செய்தி_len);

finish:
	strbuf_release(&செய்தி);
	strbuf_release(&author_date);
	strbuf_release(&author_email);
	strbuf_release(&author_name);
	strbuf_release(&sb);
	clear_mailinfo(&mi);
	திரும்பு ret;
}

/**
 * Sets commit_id to the commit hash where the mail was generated from.
 * Returns 0 on success, -1 on failure.
 */
நிலையான முழு get_mail_commit_oid(கட்டமைப்பு object_id *commit_id, மாறிலி எழுத்து *mail)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	FILE *fp = xfopen(mail, "r");
	மாறிலி எழுத்து *x;
	முழு ret = 0;

	எனில் (strbuf_getline_lf(&sb, fp) ||
	    !skip_prefix(sb.buf, "From ", &x) ||
	    get_oid_hex(x, commit_id) < 0)
		ret = -1;

	strbuf_release(&sb);
	fclose(fp);
	திரும்பு ret;
}

/**
 * Sets state->msg, state->author_name, state->author_email, state->author_date
 * to the commit's respective info.
 */
நிலையான வெறுமை get_commit_info(கட்டமைப்பு am_state *state, கட்டமைப்பு commit *commit)
{
	மாறிலி எழுத்து *buffer, *ident_வரி, *செய்தி;
	size_t ident_len;
	கட்டமைப்பு ident_split id;

	buffer = repo_logmsg_reencode(the_repository, commit, வெற்று,
				      get_commit_output_encoding());

	ident_வரி = find_commit_header(buffer, "author", &ident_len);
	எனில் (!ident_வரி)
		die(_("missing author line in commit %s"),
		      oid_to_hex(&commit->object.oid));
	எனில் (split_ident_வரி(&id, ident_வரி, ident_len) < 0)
		die(_("invalid ident line: %.*s"), (முழு)ident_len, ident_வரி);

	ப்ரதிஜ்ஞா(!state->author_name);
	எனில் (id.name_begin)
		state->author_name =
			xmemdupz(id.name_begin, id.name_end - id.name_begin);
	இல்லையெனில்
		state->author_name = xstrdup("");

	ப்ரதிஜ்ஞா(!state->author_email);
	எனில் (id.mail_begin)
		state->author_email =
			xmemdupz(id.mail_begin, id.mail_end - id.mail_begin);
	இல்லையெனில்
		state->author_email = xstrdup("");

	ப்ரதிஜ்ஞா(!state->author_date);
	state->author_date = xstrdup(show_ident_date(&id, DATE_MODE(NORMAL)));

	ப்ரதிஜ்ஞா(!state->செய்தி);
	செய்தி = strstr(buffer, "\n\n");
	எனில் (!செய்தி)
		die(_("unable to parse commit %s"), oid_to_hex(&commit->object.oid));
	state->செய்தி = xstrdup(செய்தி + 2);
	state->செய்தி_len = strlen(state->செய்தி);
	repo_unuse_commit_buffer(the_repository, commit, buffer);
}

/**
 * Writes `commit` as a patch to the state directory's "patch" file.
 */
நிலையான வெறுமை write_commit_patch(மாறிலி கட்டமைப்பு am_state *state, கட்டமைப்பு commit *commit)
{
	கட்டமைப்பு rev_info rev_info;
	FILE *fp;

	fp = xfopen(am_path(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, வெற்று);
	rev_info.diff = 1;
	rev_info.abbrev = 0;
	rev_info.disable_stdin = 1;
	rev_info.show_வேர்_diff = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.flags.இருமம் = 1;
	rev_info.diffopt.flags.full_index = 1;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	add_pending_object(&rev_info, &commit->object, "");
	diff_setup_done(&rev_info.diffopt);
	log_மரம்_commit(&rev_info, commit);
	release_revisions(&rev_info);
}

/**
 * Writes the diff of the index against HEAD as a patch to the state
 * directory's "patch" file.
 */
நிலையான வெறுமை write_index_patch(மாறிலி கட்டமைப்பு am_state *state)
{
	கட்டமைப்பு மரம் *மரம்;
	கட்டமைப்பு object_id head;
	கட்டமைப்பு rev_info rev_info;
	FILE *fp;

	எனில் (!repo_get_oid(the_repository, "HEAD", &head)) {
		கட்டமைப்பு commit *commit = lookup_commit_or_die(&head, "HEAD");
		மரம் = repo_get_commit_மரம்(the_repository, commit);
	} இல்லையெனில்
		மரம் = lookup_மரம்(the_repository,
				   the_repository->hash_algo->empty_மரம்);

	fp = xfopen(am_path(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, வெற்று);
	rev_info.diff = 1;
	rev_info.disable_stdin = 1;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	add_pending_object(&rev_info, &மரம்->object, "");
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
நிலையான முழு parse_mail_rebase(கட்டமைப்பு am_state *state, மாறிலி எழுத்து *mail)
{
	கட்டமைப்பு commit *commit;
	கட்டமைப்பு object_id commit_oid;

	எனில் (get_mail_commit_oid(&commit_oid, mail) < 0)
		die(_("could not parse %s"), mail);

	commit = lookup_commit_or_die(&commit_oid, mail);

	get_commit_info(state, commit);

	write_commit_patch(state, commit);

	oidcpy(&state->orig_commit, &commit_oid);
	write_state_text(state, "original-commit", oid_to_hex(&commit_oid));
	refs_update_ref(get_main_ref_store(the_repository), "am",
			"REBASE_HEAD", &commit_oid,
			வெற்று, REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);

	திரும்பு 0;
}

/**
 * Applies current patch with git-apply. Returns 0 on success, -1 otherwise. If
 * `index_file` is not NULL, the patch will be applied to that index.
 */
நிலையான முழு run_apply(மாறிலி கட்டமைப்பு am_state *state, மாறிலி எழுத்து *index_file)
{
	கட்டமைப்பு strvec apply_paths = STRVEC_INIT;
	கட்டமைப்பு strvec apply_opts = STRVEC_INIT;
	கட்டமைப்பு apply_state apply_state;
	முழு res, opts_இடது;
	முழு force_apply = 0;
	முழு options = 0;
	மாறிலி எழுத்து **apply_argv;

	எனில் (init_apply_state(&apply_state, the_repository, வெற்று))
		BUG("init_apply_state() failed");

	strvec_தள்ளு(&apply_opts, "apply");
	strvec_pushv(&apply_opts, state->git_apply_opts.v);

	/*
	 * Build a copy that apply_parse_options() can rearrange.
	 * apply_opts.v keeps referencing the allocated strings for
	 * strvec_clear() to release.
	 */
	DUP_ARRAY(apply_argv, apply_opts.v, apply_opts.nr);

	opts_இடது = apply_parse_options(apply_opts.nr, apply_argv,
					&apply_state, &force_apply, &options,
					வெற்று);

	எனில் (opts_இடது != 0)
		die("unknown option passed through to git apply");

	எனில் (index_file) {
		apply_state.index_file = index_file;
		apply_state.cached = 1;
	} இல்லையெனில்
		apply_state.check_index = 1;

	/*
	 * If we are allowed to fall back on 3-way merge, don't give false
	 * errors during the initial attempt.
	 */
	எனில் (state->threeway && !index_file)
		apply_state.apply_verbosity = verbosity_silent;

	எனில் (check_apply_state(&apply_state, force_apply))
		BUG("check_apply_state() failed");

	strvec_தள்ளு(&apply_paths, am_path(state, "patch"));

	res = apply_all_patches(&apply_state, apply_paths.nr, apply_paths.v, options);

	strvec_clear(&apply_paths);
	strvec_clear(&apply_opts);
	clear_apply_state(&apply_state);
	விடுவி(apply_argv);

	எனில் (res)
		திரும்பு res;

	எனில் (index_file) {
		/* Reload index as apply_all_patches() will have modified it. */
		discard_index(the_repository->index);
		read_index_from(the_repository->index, index_file,
				repo_get_git_dir(the_repository));
	}

	திரும்பு 0;
}

/**
 * Builds an index that contains just the blobs needed for a 3way merge.
 */
நிலையான முழு build_fake_ancestor(மாறிலி கட்டமைப்பு am_state *state, மாறிலி எழுத்து *index_file)
{
	கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	strvec_தள்ளு(&cp.args, "apply");
	strvec_pushv(&cp.args, state->git_apply_opts.v);
	strvec_pushf(&cp.args, "--build-fake-ancestor=%s", index_file);
	strvec_தள்ளு(&cp.args, am_path(state, "patch"));

	எனில் (run_command(&cp))
		திரும்பு -1;

	திரும்பு 0;
}

/**
 * Attempt a threeway merge, using index_path as the temporary index.
 */
நிலையான முழு fall_back_threeway(மாறிலி கட்டமைப்பு am_state *state, மாறிலி எழுத்து *index_path)
{
	கட்டமைப்பு object_id their_மரம், our_மரம்;
	கட்டமைப்பு object_id bases[1] = { 0 };
	கட்டமைப்பு merge_options o;
	கட்டமைப்பு commit *result;
	எழுத்து *their_மரம்_name;

	எனில் (repo_get_oid(the_repository, "HEAD", &our_மரம்) < 0)
		oidcpy(&our_மரம், the_hash_algo->empty_மரம்);

	எனில் (build_fake_ancestor(state, index_path))
		திரும்பு error("could not build fake ancestor");

	discard_index(the_repository->index);
	read_index_from(the_repository->index, index_path, repo_get_git_dir(the_repository));

	எனில் (write_index_as_மரம்(&bases[0], the_repository->index, index_path, 0, வெற்று))
		திரும்பு error(_("Repository lacks necessary blobs to fall back on 3-way merge."));

	say(state, stdout, _("Using index info to reconstruct a base tree..."));

	எனில் (!state->quiet) {
		/*
		 * List paths that needed 3-way fallback, so that the user can
		 * review them with extra care to spot mismerges.
		 */
		கட்டமைப்பு rev_info rev_info;

		repo_init_revisions(the_repository, &rev_info, வெற்று);
		rev_info.diffopt.output_format = DIFF_FORMAT_NAME_STATUS;
		rev_info.diffopt.filter |= diff_filter_bit('A');
		rev_info.diffopt.filter |= diff_filter_bit('M');
		add_pending_oid(&rev_info, "HEAD", &our_மரம், 0);
		diff_setup_done(&rev_info.diffopt);
		run_diff_index(&rev_info, DIFF_INDEX_CACHED);
		release_revisions(&rev_info);
	}

	எனில் (run_apply(state, index_path))
		திரும்பு error(_("Did you hand edit your patch?\n"
				"It does not apply to blobs recorded in its index."));

	எனில் (write_index_as_மரம்(&their_மரம், the_repository->index, index_path, 0, வெற்று))
		திரும்பு error("could not write tree");

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
	their_மரம்_name = xstrfmt("%.*s", linelen(state->செய்தி), state->செய்தி);
	o.branch2 = their_மரம்_name;
	o.ancestor = "constructed fake ancestor";
	o.detect_directory_renames = MERGE_DIRECTORY_RENAMES_NONE;

	எனில் (state->quiet)
		o.verbosity = 0;

	எனில் (merge_ort_generic(&o, &our_மரம், &their_மரம், 1, bases, &result)) {
		repo_rerere(the_repository, state->allow_rerere_autoupdate);
		விடுவி(their_மரம்_name);
		திரும்பு error(_("Failed to merge in the changes."));
	}

	விடுவி(their_மரம்_name);
	திரும்பு 0;
}

/**
 * Commits the current index with state->msg as the commit message and
 * state->author_name, state->author_email and state->author_date as the author
 * information.
 */
நிலையான வெறுமை do_commit(மாறிலி கட்டமைப்பு am_state *state)
{
	கட்டமைப்பு object_id மரம், parent, commit;
	மாறிலி கட்டமைப்பு object_id *old_oid;
	கட்டமைப்பு commit_list *parents = வெற்று;
	மாறிலி எழுத்து *reflog_செய்தி, *author, *committer = வெற்று;
	கட்டமைப்பு strbuf sb = STRBUF_INIT;

	எனில் (!state->no_verify && run_hooks(the_repository, "pre-applypatch"))
		exit(1);

	எனில் (write_index_as_மரம்(&மரம், the_repository->index,
				repo_get_index_file(the_repository),
				0, வெற்று))
		die(_("git write-tree failed to write a tree"));

	எனில் (!repo_get_oid_commit(the_repository, "HEAD", &parent)) {
		old_oid = &parent;
		commit_list_செருகு(lookup_commit(the_repository, &parent),
				   &parents);
	} இல்லையெனில் {
		old_oid = வெற்று;
		say(state, stderr, _("applying to an empty history"));
	}

	author = fmt_ident(state->author_name, state->author_email,
		WANT_AUTHOR_IDENT,
			state->ignore_date ? வெற்று : state->author_date,
			IDENT_STRICT);

	எனில் (state->committer_date_is_author_date)
		committer = fmt_ident(getenv("GIT_COMMITTER_NAME"),
				      getenv("GIT_COMMITTER_EMAIL"),
				      WANT_COMMITTER_IDENT,
				      state->ignore_date ? வெற்று
							 : state->author_date,
				      IDENT_STRICT);

	எனில் (commit_மரம்_extended(state->செய்தி, state->செய்தி_len, &மரம், parents,
				 &commit, author, committer, state->sign_commit,
				 வெற்று))
		die(_("failed to write commit object"));

	reflog_செய்தி = getenv("GIT_REFLOG_ACTION");
	எனில் (!reflog_செய்தி)
		reflog_செய்தி = "am";

	strbuf_addf(&sb, "%s: %.*s", reflog_செய்தி, linelen(state->செய்தி),
			state->செய்தி);

	refs_update_ref(get_main_ref_store(the_repository), sb.buf, "HEAD",
			&commit, old_oid, 0,
			UPDATE_REFS_DIE_ON_ERR);

	எனில் (state->rebasing) {
		FILE *fp = xfopen(am_path(state, "rewritten"), "a");

		ப்ரதிஜ்ஞா(!is_null_oid(&state->orig_commit));
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
நிலையான வெறுமை validate_resume_state(மாறிலி கட்டமைப்பு am_state *state)
{
	எனில் (!state->செய்தி)
		die(_("cannot resume: %s does not exist."),
			am_path(state, "final-commit"));

	எனில் (!state->author_name || !state->author_email || !state->author_date)
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
நிலையான முழு do_interactive(கட்டமைப்பு am_state *state)
{
	ப்ரதிஜ்ஞா(state->செய்தி);

	க்கு (;;) {
		எழுத்து reply[64];

		puts(_("Commit Body is:"));
		puts("--------------------------");
		printf("%s", state->செய்தி);
		puts("--------------------------");

		/*
		 * TRANSLATORS: Make sure to include [y], [n], [e], [v] and [a]
		 * in your translation. The program will only accept English
		 * input at this point.
		 */
		printf(_("Apply? [y]es/[n]o/[e]dit/[v]iew patch/[a]ccept all: "));
		எனில் (!fgets(reply, அளவு(reply), stdin))
			die("unable to read from stdin; aborting");

		எனில் (*reply == 'y' || *reply == 'Y') {
			திரும்பு 0;
		} இல்லையெனில் எனில் (*reply == 'a' || *reply == 'A') {
			state->interactive = 0;
			திரும்பு 0;
		} இல்லையெனில் எனில் (*reply == 'n' || *reply == 'N') {
			திரும்பு 1;
		} இல்லையெனில் எனில் (*reply == 'e' || *reply == 'E') {
			கட்டமைப்பு strbuf செய்தி = STRBUF_INIT;

			எனில் (!launch_editor(am_path(state, "final-commit"), &செய்தி, வெற்று)) {
				விடுவி(state->செய்தி);
				state->செய்தி = strbuf_detach(&செய்தி, &state->செய்தி_len);
			}
			strbuf_release(&செய்தி);
		} இல்லையெனில் எனில் (*reply == 'v' || *reply == 'V') {
			மாறிலி எழுத்து *pager = git_pager(the_repository, 1);
			கட்டமைப்பு child_process cp = CHILD_PROCESS_INIT;

			எனில் (!pager)
				pager = "cat";
			prepare_pager_args(&cp, pager);
			strvec_தள்ளு(&cp.args, am_path(state, "patch"));
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
நிலையான வெறுமை am_run(கட்டமைப்பு am_state *state, முழு resume)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;

	unlink(am_path(state, "dirtyindex"));

	எனில் (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 வெற்று, வெற்று, வெற்று) < 0)
		die(_("unable to write index file"));

	எனில் (repo_index_has_changes(the_repository, வெற்று, &sb)) {
		write_state_bool(state, "dirtyindex", 1);
		die(_("Dirty index: cannot apply patches (dirty: %s)"), sb.buf);
	}

	strbuf_release(&sb);

	போது (state->cur <= state->last) {
		மாறிலி எழுத்து *mail = am_path(state, msgnum(state));
		முழு apply_status;
		முழு to_keep;

		reset_ident_date();

		எனில் (!file_exists(mail))
			செல் next;

		எனில் (resume) {
			validate_resume_state(state);
		} இல்லையெனில் {
			முழு skip;

			எனில் (state->rebasing)
				skip = parse_mail_rebase(state, mail);
			இல்லையெனில்
				skip = parse_mail(state, mail);

			எனில் (skip)
				செல் next; /* mail should be skipped */

			எனில் (state->signoff)
				am_append_signoff(state);

			write_author_script(state);
			write_commit_செய்தி(state);
		}

		எனில் (state->interactive && do_interactive(state))
			செல் next;

		to_keep = 0;
		எனில் (is_empty_or_missing_file(am_path(state, "patch"))) {
			தேர்வு (state->empty_type) {
			நிலை DROP_EMPTY_COMMIT:
				say(state, stdout, _("Skipping: %.*s"), linelen(state->செய்தி), state->செய்தி);
				செல் next;
				நிறுத்து;
			நிலை KEEP_EMPTY_COMMIT:
				to_keep = 1;
				say(state, stdout, _("Creating an empty commit: %.*s"),
					linelen(state->செய்தி), state->செய்தி);
				நிறுத்து;
			நிலை STOP_ON_EMPTY_COMMIT:
				printf_ln(_("Patch is empty."));
				die_user_resolve(state);
				நிறுத்து;
			}
		}

		எனில் (run_applypatch_செய்தி_hook(state))
			exit(1);
		எனில் (to_keep)
			செல் commit;

		say(state, stdout, _("Applying: %.*s"), linelen(state->செய்தி), state->செய்தி);

		apply_status = run_apply(state, வெற்று);

		எனில் (apply_status && state->threeway) {
			கட்டமைப்பு strbuf sb = STRBUF_INIT;

			strbuf_addstr(&sb, am_path(state, "patch-merge-index"));
			apply_status = fall_back_threeway(state, sb.buf);
			strbuf_release(&sb);

			/*
			 * Applying the patch to an earlier tree and merging
			 * the result may have produced the same tree as ours.
			 */
			எனில் (!apply_status &&
			    !repo_index_has_changes(the_repository, வெற்று, வெற்று)) {
				say(state, stdout, _("No changes -- Patch already applied."));
				செல் next;
			}
		}

		எனில் (apply_status) {
			printf_ln(_("Patch failed at %s %.*s"), msgnum(state),
				linelen(state->செய்தி), state->செய்தி);

			எனில் (advice_enabled(ADVICE_AM_WORK_DIR))
				advise(_("Use 'git am --show-current-patch=diff' to see the failed patch"));

			die_user_resolve(state);
		}

commit:
		do_commit(state);

next:
		am_next(state);

		எனில் (resume)
			am_load(state);
		resume = 0;
	}

	எனில் (!is_empty_or_missing_file(am_path(state, "rewritten"))) {
		ப்ரதிஜ்ஞா(state->rebasing);
		copy_notes_for_rebase(state);
		run_post_rewrite_hook(state);
	}

	/*
	 * In rebasing mode, it's up to the caller to take care of
	 * housekeeping.
	 */
	எனில் (!state->rebasing) {
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
நிலையான வெறுமை am_resolve(கட்டமைப்பு am_state *state, முழு allow_empty)
{
	validate_resume_state(state);

	say(state, stdout, _("Applying: %.*s"), linelen(state->செய்தி), state->செய்தி);

	எனில் (!repo_index_has_changes(the_repository, வெற்று, வெற்று)) {
		எனில் (allow_empty && is_empty_or_missing_file(am_path(state, "patch"))) {
			printf_ln(_("No changes - recorded it as an empty commit."));
		} இல்லையெனில் {
			printf_ln(_("No changes - did you forget to use 'git add'?\n"
				    "If there is nothing left to stage, chances are that something else\n"
				    "already introduced the same changes; you might want to skip this patch."));
			die_user_resolve(state);
		}
	}

	எனில் (unmerged_index(the_repository->index)) {
		printf_ln(_("You still have unmerged paths in your index.\n"
			"You should 'git add' each file with resolved conflicts to mark them as such.\n"
			"You might run `git rm` on a file to accept \"deleted by them\" for it."));
		die_user_resolve(state);
	}

	எனில் (state->interactive) {
		write_index_patch(state);
		எனில் (do_interactive(state))
			செல் next;
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
நிலையான முழு fast_forward_to(கட்டமைப்பு மரம் *head, கட்டமைப்பு மரம் *remote, முழு reset)
{
	கட்டமைப்பு lock_file lock_file = LOCK_INIT;
	கட்டமைப்பு unpack_trees_options opts;
	கட்டமைப்பு மரம்_desc t[2];

	எனில் (repo_parse_மரம்(the_repository, head) || repo_parse_மரம்(the_repository, remote))
		திரும்பு -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	refresh_index(the_repository->index, REFRESH_QUIET, வெற்று, வெற்று, வெற்று);

	memset(&opts, 0, அளவு(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.update = 1;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = twoway_merge;
	init_மரம்_desc(&t[0], &head->object.oid, head->buffer, head->size);
	init_மரம்_desc(&t[1], &remote->object.oid, remote->buffer, remote->size);

	எனில் (unpack_trees(2, t, &opts)) {
		rollback_lock_file(&lock_file);
		திரும்பு -1;
	}

	எனில் (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	திரும்பு 0;
}

/**
 * Merges a tree into the index. The index's stat info will take precedence
 * over the merged tree's. Returns 0 on success, -1 on failure.
 */
நிலையான முழு merge_மரம்(கட்டமைப்பு மரம் *மரம்)
{
	கட்டமைப்பு lock_file lock_file = LOCK_INIT;
	கட்டமைப்பு unpack_trees_options opts;
	கட்டமைப்பு மரம்_desc t[1];

	எனில் (repo_parse_மரம்(the_repository, மரம்))
		திரும்பு -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, அளவு(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.merge = 1;
	opts.fn = oneway_merge;
	init_மரம்_desc(&t[0], &மரம்->object.oid, மரம்->buffer, மரம்->size);

	எனில் (unpack_trees(1, t, &opts)) {
		rollback_lock_file(&lock_file);
		திரும்பு -1;
	}

	எனில் (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	திரும்பு 0;
}

/**
 * Clean the index without touching entries that are not modified between
 * `head` and `remote`.
 */
நிலையான முழு clean_index(மாறிலி கட்டமைப்பு object_id *head, மாறிலி கட்டமைப்பு object_id *remote)
{
	கட்டமைப்பு மரம் *head_மரம், *remote_மரம், *index_மரம்;
	கட்டமைப்பு object_id index;

	head_மரம் = repo_parse_மரம்_indirect(the_repository, head);
	எனில் (!head_மரம்)
		திரும்பு error(_("Could not parse object '%s'."), oid_to_hex(head));

	remote_மரம் = repo_parse_மரம்_indirect(the_repository, remote);
	எனில் (!remote_மரம்)
		திரும்பு error(_("Could not parse object '%s'."), oid_to_hex(remote));

	repo_read_index_unmerged(the_repository);

	எனில் (fast_forward_to(head_மரம், head_மரம், 1))
		திரும்பு -1;

	எனில் (write_index_as_மரம்(&index, the_repository->index,
				repo_get_index_file(the_repository),
				0, வெற்று))
		திரும்பு -1;

	index_மரம் = repo_parse_மரம்_indirect(the_repository, &index);
	எனில் (!index_மரம்)
		திரும்பு error(_("Could not parse object '%s'."), oid_to_hex(&index));

	எனில் (fast_forward_to(index_மரம், remote_மரம், 0))
		திரும்பு -1;

	எனில் (merge_மரம்(remote_மரம்))
		திரும்பு -1;

	remove_branch_state(the_repository, 0);

	திரும்பு 0;
}

/**
 * Resets rerere's merge resolution metadata.
 */
நிலையான வெறுமை am_rerere_clear(வெறுமை)
{
	கட்டமைப்பு string_list merge_rr = STRING_LIST_INIT_DUP;
	rerere_clear(the_repository, &merge_rr);
	string_list_clear(&merge_rr, 1);
}

/**
 * Resume the current am session by skipping the current patch.
 */
நிலையான வெறுமை am_skip(கட்டமைப்பு am_state *state)
{
	கட்டமைப்பு object_id head;

	am_rerere_clear();

	எனில் (repo_get_oid(the_repository, "HEAD", &head))
		oidcpy(&head, the_hash_algo->empty_மரம்);

	எனில் (clean_index(&head, &head))
		die(_("failed to clean index"));

	எனில் (state->rebasing) {
		FILE *fp = xfopen(am_path(state, "rewritten"), "a");

		ப்ரதிஜ்ஞா(!is_null_oid(&state->orig_commit));
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
நிலையான முழு safe_to_abort(மாறிலி கட்டமைப்பு am_state *state)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	கட்டமைப்பு object_id abort_safety, head;

	எனில் (file_exists(am_path(state, "dirtyindex")))
		திரும்பு 0;

	எனில் (read_state_file(&sb, state, "abort-safety", 1) > 0) {
		எனில் (get_oid_hex(sb.buf, &abort_safety))
			die(_("could not parse %s"), am_path(state, "abort-safety"));
	} இல்லையெனில்
		oidclr(&abort_safety, the_repository->hash_algo);
	strbuf_release(&sb);

	எனில் (repo_get_oid(the_repository, "HEAD", &head))
		oidclr(&head, the_repository->hash_algo);

	எனில் (oideq(&head, &abort_safety))
		திரும்பு 1;

	warning(_("You seem to have moved HEAD since the last 'am' failure.\n"
		"Not rewinding to ORIG_HEAD"));

	திரும்பு 0;
}

/**
 * Aborts the current am session if it is safe to do so.
 */
நிலையான வெறுமை am_abort(கட்டமைப்பு am_state *state)
{
	கட்டமைப்பு object_id curr_head, orig_head;
	முழு has_curr_head, has_orig_head;
	எழுத்து *curr_branch;

	எனில் (!safe_to_abort(state)) {
		am_destroy(state);
		திரும்பு;
	}

	am_rerere_clear();

	curr_branch = refs_resolve_refdup(get_main_ref_store(the_repository),
					  "HEAD", 0, &curr_head, வெற்று);
	has_curr_head = curr_branch && !is_null_oid(&curr_head);
	எனில் (!has_curr_head)
		oidcpy(&curr_head, the_hash_algo->empty_மரம்);

	has_orig_head = !repo_get_oid(the_repository, "ORIG_HEAD", &orig_head);
	எனில் (!has_orig_head)
		oidcpy(&orig_head, the_hash_algo->empty_மரம்);

	எனில் (clean_index(&curr_head, &orig_head))
		die(_("failed to clean index"));

	எனில் (has_orig_head)
		refs_update_ref(get_main_ref_store(the_repository),
				"am --abort", "HEAD", &orig_head,
				has_curr_head ? &curr_head : வெற்று, 0,
				UPDATE_REFS_DIE_ON_ERR);
	இல்லையெனில் எனில் (curr_branch)
		refs_delete_ref(get_main_ref_store(the_repository), வெற்று,
				curr_branch, வெற்று, REF_NO_DEREF);

	விடுவி(curr_branch);
	am_destroy(state);
}

நிலையான முழு show_patch(கட்டமைப்பு am_state *state, எண்ணிக்கை resume_type resume_mode)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	மாறிலி எழுத்து *patch_path;
	முழு len;

	எனில் (!is_null_oid(&state->orig_commit)) {
		கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;

		strvec_pushl(&cmd.args, "show", oid_to_hex(&state->orig_commit),
			     "--", வெற்று);
		cmd.git_cmd = 1;
		திரும்பு run_command(&cmd);
	}

	தேர்வு (resume_mode) {
	நிலை RESUME_SHOW_PATCH_RAW:
		patch_path = am_path(state, msgnum(state));
		நிறுத்து;
	நிலை RESUME_SHOW_PATCH_DIFF:
		patch_path = am_path(state, "patch");
		நிறுத்து;
	இயல்பு:
		BUG("invalid mode for --show-current-patch");
	}

	len = strbuf_read_file(&sb, patch_path, 0);
	எனில் (len < 0)
		die_errno(_("failed to read '%s'"), patch_path);

	setup_pager(the_repository);
	write_in_full(1, sb.buf, sb.len);
	strbuf_release(&sb);
	திரும்பு 0;
}

/**
 * parse_options() callback that validates and sets opt->value to the
 * PATCH_FORMAT_* enum value corresponding to `arg`.
 */
நிலையான முழு parse_opt_patchformat(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	முழு *opt_மதிப்பு = opt->மதிப்பு;

	எனில் (unset)
		*opt_மதிப்பு = PATCH_FORMAT_UNKNOWN;
	இல்லையெனில் எனில் (!strcmp(arg, "mbox"))
		*opt_மதிப்பு = PATCH_FORMAT_MBOX;
	இல்லையெனில் எனில் (!strcmp(arg, "stgit"))
		*opt_மதிப்பு = PATCH_FORMAT_STGIT;
	இல்லையெனில் எனில் (!strcmp(arg, "stgit-series"))
		*opt_மதிப்பு = PATCH_FORMAT_STGIT_SERIES;
	இல்லையெனில் எனில் (!strcmp(arg, "hg"))
		*opt_மதிப்பு = PATCH_FORMAT_HG;
	இல்லையெனில் எனில் (!strcmp(arg, "mboxrd"))
		*opt_மதிப்பு = PATCH_FORMAT_MBOXRD;
	/*
	 * Please update $__git_patchformat in git-completion.bash
	 * when you add new options
	 */
	இல்லையெனில்
		திரும்பு error(_("invalid value for '%s': '%s'"),
			     "--patch-format", arg);
	திரும்பு 0;
}

நிலையான முழு parse_opt_show_current_patch(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	முழு *opt_மதிப்பு = opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);

	எனில் (!arg)
		*opt_மதிப்பு = opt->defval;
	இல்லையெனில் எனில் (!strcmp(arg, "raw"))
		*opt_மதிப்பு = RESUME_SHOW_PATCH_RAW;
	இல்லையெனில் எனில் (!strcmp(arg, "diff"))
		*opt_மதிப்பு = RESUME_SHOW_PATCH_DIFF;
	/*
	 * Please update $__git_showcurrentpatch in git-completion.bash
	 * when you add new options
	 */
	இல்லையெனில்
		திரும்பு error(_("invalid value for '%s': '%s'"),
			     "--show-current-patch", arg);
	திரும்பு 0;
}

முழு cmd_am(முழு argc,
	   மாறிலி எழுத்து **argv,
	   மாறிலி எழுத்து *prefix,
	   கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு am_state state;
	முழு இருமம் = -1;
	முழு keep_cr = -1;
	முழு patch_format = PATCH_FORMAT_UNKNOWN;
	எண்ணிக்கை resume_type resume_mode = RESUME_FALSE;
	முழு in_progress;
	முழு ret = 0;

	மாறிலி எழுத்து * மாறிலி usage[] = {
		N_("git am [<options>] [(<mbox> | <Maildir>)...]"),
		N_("git am [<options>] (--continue | --skip | --abort)"),
		வெற்று
	};

	கட்டமைப்பு option options[] = {
		OPT_BOOL('i', "interactive", &state.interactive,
			N_("run interactively")),
		OPT_BOOL('n', "no-verify", &state.no_verify,
			N_("bypass pre-applypatch and applypatch-msg hooks")),
		OPT_HIDDEN_BOOL('b', "binary", &இருமம்,
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
		OPT_PASSTHRU_ARGV(0, "ignore-space-change", &state.git_apply_opts, வெற்று,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU_ARGV(0, "ignore-whitespace", &state.git_apply_opts, வெற்று,
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
		OPT_PASSTHRU_ARGV('C', வெற்று, &state.git_apply_opts, N_("n"),
			N_("pass it through git-apply"),
			0),
		OPT_PASSTHRU_ARGV('p', வெற்று, &state.git_apply_opts, N_("num"),
			N_("pass it through git-apply"),
			0),
		OPT_CALLBACK(0, "patch-format", &patch_format, N_("format"),
			N_("format the patch(es) are in"),
			parse_opt_patchformat),
		OPT_PASSTHRU_ARGV(0, "reject", &state.git_apply_opts, வெற்று,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_STRING(0, "resolvemsg", &state.resolvemsg, வெற்று,
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
			.மதிப்பு = &resume_mode,
			.precision = அளவு(resume_mode),
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
			.மதிப்பு = &state.sign_commit,
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

	repo_config(the_repository, git_default_config, வெற்று);

	am_state_init(&state);

	in_progress = am_in_progress(&state);
	எனில் (in_progress)
		am_load(&state);

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	எனில் (இருமம் >= 0)
		fprintf_ln(stderr, _("The -b/--binary option has been a no-op for long time, and\n"
				"it will be removed. Please do not use it anymore."));

	/* Ensure a valid committer ident can be constructed */
	git_committer_info(IDENT_STRICT);

	எனில் (repo_read_index_preload(the_repository, வெற்று, 0) < 0)
		die(_("failed to read the index"));

	எனில் (in_progress) {
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
		எனில் (argc || (resume_mode == RESUME_FALSE && !isatty(0)))
			die(_("previous rebase directory %s still exists but mbox given."),
				state.dir);

		எனில் (resume_mode == RESUME_FALSE)
			resume_mode = RESUME_APPLY;

		எனில் (state.signoff == SIGNOFF_EXPLICIT)
			am_append_signoff(&state);
	} இல்லையெனில் {
		கட்டமைப்பு strvec paths = STRVEC_INIT;
		முழு i;

		/*
		 * Handle stray state directory in the independent-run case. In
		 * the --rebasing case, it is up to the caller to take care of
		 * stray directories.
		 */
		எனில் (file_exists(state.dir) && !state.rebasing) {
			எனில் (resume_mode == RESUME_ABORT || resume_mode == RESUME_QUIT) {
				am_destroy(&state);
				am_state_release(&state);
				திரும்பு 0;
			}

			die(_("Stray %s directory found.\n"
				"Use \"git am --abort\" to remove it."),
				state.dir);
		}

		எனில் (resume_mode)
			die(_("Resolve operation not in progress, we are not resuming."));

		க்கு (i = 0; i < argc; i++) {
			எனில் (is_absolute_path(argv[i]) || !prefix)
				strvec_தள்ளு(&paths, argv[i]);
			இல்லையெனில்
				strvec_தள்ளு(&paths, mkpath("%s/%s", prefix, argv[i]));
		}

		எனில் (state.interactive && !paths.nr)
			die(_("interactive mode requires patches on the command line"));

		am_setup(&state, patch_format, paths.v, keep_cr);

		strvec_clear(&paths);
	}

	தேர்வு (resume_mode) {
	நிலை RESUME_FALSE:
		am_run(&state, 0);
		நிறுத்து;
	நிலை RESUME_APPLY:
		am_run(&state, 1);
		நிறுத்து;
	நிலை RESUME_RESOLVED:
	நிலை RESUME_ALLOW_EMPTY:
		am_resolve(&state, resume_mode == RESUME_ALLOW_EMPTY ? 1 : 0);
		நிறுத்து;
	நிலை RESUME_SKIP:
		am_skip(&state);
		நிறுத்து;
	நிலை RESUME_ABORT:
		am_abort(&state);
		நிறுத்து;
	நிலை RESUME_QUIT:
		am_rerere_clear();
		am_destroy(&state);
		நிறுத்து;
	நிலை RESUME_SHOW_PATCH_RAW:
	நிலை RESUME_SHOW_PATCH_DIFF:
		ret = show_patch(&state, resume_mode);
		நிறுத்து;
	இயல்பு:
		BUG("invalid resume value");
	}

	am_state_release(&state);

	திரும்பு ret;
}
