/*
 * Builtin "git am"
 *
 * Based on git-am.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "abspath.h"
#অন্তর্ভাব "advice.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "editor.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "dir.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "hook.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "tempfile.h"
#অন্তর্ভাব "lockfile.h"
#অন্তর্ভাব "cache-tree.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "diff.h"
#অন্তর্ভাব "unpack-trees.h"
#অন্তর্ভাব "branch.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "preload-index.h"
#অন্তর্ভাব "sequencer.h"
#অন্তর্ভাব "revision.h"
#অন্তর্ভাব "merge-ort-wrappers.h"
#অন্তর্ভাব "log-tree.h"
#অন্তর্ভাব "notes-utils.h"
#অন্তর্ভাব "rerere.h"
#অন্তর্ভাব "mailinfo.h"
#অন্তর্ভাব "apply.h"
#অন্তর্ভাব "string-list.h"
#অন্তর্ভাব "pager.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "pretty.h"

/**
 * Returns the length of the first line of msg.
 */
স্থির পূর্ণসংখ্যা linelen(ধ্রুবক অক্ষর *বার্তা)
{
	ফেরত strchrnul(বার্তা, '\n') - বার্তা;
}

/**
 * Returns true if `str` consists of only whitespace, false otherwise.
 */
স্থির পূর্ণসংখ্যা str_isspace(ধ্রুবক অক্ষর *str)
{
	জন্য (; *str; str++)
		যদি (!isspace(*str))
			ফেরত 0;

	ফেরত 1;
}

গণনা patch_format {
	PATCH_FORMAT_UNKNOWN = 0,
	PATCH_FORMAT_MBOX,
	PATCH_FORMAT_STGIT,
	PATCH_FORMAT_STGIT_SERIES,
	PATCH_FORMAT_HG,
	PATCH_FORMAT_MBOXRD
};

গণনা keep_type {
	KEEP_FALSE = 0,
	KEEP_TRUE,      /* pass -k flag to git-mailinfo */
	KEEP_NON_PATCH  /* pass -b flag to git-mailinfo */
};

গণনা scissors_type {
	SCISSORS_UNSET = -1,
	SCISSORS_FALSE = 0,  /* pass --no-scissors to git-mailinfo */
	SCISSORS_TRUE        /* pass --scissors to git-mailinfo */
};

গণনা signoff_type {
	SIGNOFF_FALSE = 0,
	SIGNOFF_TRUE = 1,
	SIGNOFF_EXPLICIT /* --signoff was set on the command-line */
};

গণনা resume_type {
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

গণনা empty_action {
	STOP_ON_EMPTY_COMMIT = 0,  /* output errors and stop in the middle of an am session */
	DROP_EMPTY_COMMIT,         /* skip with a notice message, unless "--quiet" has been passed */
	KEEP_EMPTY_COMMIT,         /* keep recording as empty commits */
};

কাঠামো am_state {
	/* state directory path */
	অক্ষর *dir;

	/* current and last patch numbers, 1-indexed */
	পূর্ণসংখ্যা cur;
	পূর্ণসংখ্যা last;

	/* commit metadata and message */
	অক্ষর *author_name;
	অক্ষর *author_email;
	অক্ষর *author_date;
	অক্ষর *বার্তা;
	আকার_t বার্তা_len;

	/* when --rebasing, records the original commit the patch came from */
	কাঠামো object_id orig_commit;

	/* number of digits in patch filename */
	পূর্ণসংখ্যা prec;

	/* various operating modes and command line options */
	পূর্ণসংখ্যা interactive;
	পূর্ণসংখ্যা no_verify;
	পূর্ণসংখ্যা threeway;
	পূর্ণসংখ্যা quiet;
	পূর্ণসংখ্যা signoff; /* enum signoff_type */
	পূর্ণসংখ্যা utf8;
	পূর্ণসংখ্যা keep; /* enum keep_type */
	পূর্ণসংখ্যা message_id;
	পূর্ণসংখ্যা scissors; /* enum scissors_type */
	পূর্ণসংখ্যা quoted_cr; /* enum quoted_cr_action */
	পূর্ণসংখ্যা empty_type; /* enum empty_action */
	কাঠামো strvec git_apply_opts;
	ধ্রুবক অক্ষর *resolvemsg;
	পূর্ণসংখ্যা committer_date_is_author_date;
	পূর্ণসংখ্যা ignore_date;
	পূর্ণসংখ্যা allow_rerere_autoupdate;
	ধ্রুবক অক্ষর *sign_commit;
	পূর্ণসংখ্যা rebasing;
};

/**
 * Initializes am_state with the default values.
 */
স্থির শূন্য am_state_init(কাঠামো am_state *state)
{
	পূর্ণসংখ্যা gpgsign;

	memset(state, 0, আকার(*state));

	state->dir = repo_git_path(the_repository, "rebase-apply");

	state->prec = 4;

	repo_config_get_bool(the_repository, "am.threeway", &state->threeway);

	state->utf8 = 1;

	repo_config_get_bool(the_repository, "am.messageid", &state->message_id);

	state->scissors = SCISSORS_UNSET;
	state->quoted_cr = quoted_cr_unset;

	strvec_init(&state->git_apply_opts);

	যদি (!repo_config_get_bool(the_repository, "commit.gpgsign", &gpgsign))
		state->sign_commit = gpgsign ? "" : ফাঁকা;
}

/**
 * Releases memory allocated by an am_state.
 */
স্থির শূন্য am_state_release(কাঠামো am_state *state)
{
	মুক্ত(state->dir);
	মুক্ত(state->author_name);
	মুক্ত(state->author_email);
	মুক্ত(state->author_date);
	মুক্ত(state->বার্তা);
	strvec_clear(&state->git_apply_opts);
}

স্থির পূর্ণসংখ্যা am_option_parse_quoted_cr(ধ্রুবক কাঠামো option *opt,
				     ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	BUG_ON_OPT_NEG(unset);

	যদি (mailinfo_parse_quoted_cr_action(arg, opt->মান) != 0)
		ফেরত error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা am_option_parse_empty(ধ্রুবক কাঠামো option *opt,
				     ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	পূর্ণসংখ্যা *opt_মান = opt->মান;

	BUG_ON_OPT_NEG(unset);

	যদি (!strcmp(arg, "stop"))
		*opt_মান = STOP_ON_EMPTY_COMMIT;
	অন্যথা যদি (!strcmp(arg, "drop"))
		*opt_মান = DROP_EMPTY_COMMIT;
	অন্যথা যদি (!strcmp(arg, "keep"))
		*opt_মান = KEEP_EMPTY_COMMIT;
	অন্যথা
		ফেরত error(_("invalid value for '%s': '%s'"), "--empty", arg);

	ফেরত 0;
}

/**
 * Returns path relative to the am_state directory.
 */
স্থির অন্তঃরেখ ধ্রুবক অক্ষর *am_path(ধ্রুবক কাঠামো am_state *state, ধ্রুবক অক্ষর *path)
{
	ফেরত mkpath("%s/%s", state->dir, path);
}

/**
 * For convenience to call write_file()
 */
স্থির শূন্য write_state_text(ধ্রুবক কাঠামো am_state *state,
			     ধ্রুবক অক্ষর *name, ধ্রুবক অক্ষর *string)
{
	write_file(am_path(state, name), "%s", string);
}

স্থির শূন্য write_state_গুনতি(ধ্রুবক কাঠামো am_state *state,
			      ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা মান)
{
	write_file(am_path(state, name), "%d", মান);
}

স্থির শূন্য write_state_bool(ধ্রুবক কাঠামো am_state *state,
			     ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা মান)
{
	write_state_text(state, name, মান ? "t" : "f");
}

/**
 * If state->quiet is false, calls fprintf(fp, fmt, ...), and appends a newline
 * at the end.
 */
__attribute__((format (printf, 3, 4)))
স্থির শূন্য say(ধ্রুবক কাঠামো am_state *state, FILE *fp, ধ্রুবক অক্ষর *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	যদি (!state->quiet) {
		vfprintf(fp, fmt, ap);
		putc('\n', fp);
	}
	va_end(ap);
}

/**
 * Returns 1 if there is an am session in progress, 0 otherwise.
 */
স্থির পূর্ণসংখ্যা am_in_progress(ধ্রুবক কাঠামো am_state *state)
{
	কাঠামো stat st;

	যদি (lstat(state->dir, &st) < 0 || !S_ISDIR(st.st_mode))
		ফেরত 0;
	যদি (lstat(am_path(state, "last"), &st) || !S_ISREG(st.st_mode))
		ফেরত 0;
	যদি (lstat(am_path(state, "next"), &st) || !S_ISREG(st.st_mode))
		ফেরত 0;
	ফেরত 1;
}

/**
 * Reads the contents of `file` in the `state` directory into `sb`. Returns the
 * number of bytes read on success, -1 if the file does not exist. If `trim` is
 * set, trailing whitespace will be removed.
 */
স্থির পূর্ণসংখ্যা read_state_file(কাঠামো strbuf *sb, ধ্রুবক কাঠামো am_state *state,
			ধ্রুবক অক্ষর *file, পূর্ণসংখ্যা trim)
{
	strbuf_reset(sb);

	যদি (strbuf_read_file(sb, am_path(state, file), 0) >= 0) {
		যদি (trim)
			strbuf_trim(sb);

		ফেরত sb->len;
	}

	যদি (errno == ENOENT)
		ফেরত -1;

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
স্থির পূর্ণসংখ্যা read_am_author_script(কাঠামো am_state *state)
{
	ধ্রুবক অক্ষর *filename = am_path(state, "author-script");

	প্রতিজ্ঞা(!state->author_name);
	প্রতিজ্ঞা(!state->author_email);
	প্রতিজ্ঞা(!state->author_date);

	ফেরত read_author_script(filename, &state->author_name,
				  &state->author_email, &state->author_date, 1);
}

/**
 * Saves state->author_name, state->author_email and state->author_date in the
 * state directory's "author-script" file.
 */
স্থির শূন্য write_author_script(ধ্রুবক কাঠামো am_state *state)
{
	কাঠামো strbuf sb = STRBUF_INIT;

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
স্থির পূর্ণসংখ্যা read_commit_বার্তা(কাঠামো am_state *state)
{
	কাঠামো strbuf sb = STRBUF_INIT;

	প্রতিজ্ঞা(!state->বার্তা);

	যদি (read_state_file(&sb, state, "final-commit", 0) < 0) {
		strbuf_release(&sb);
		ফেরত -1;
	}

	state->বার্তা = strbuf_detach(&sb, &state->বার্তা_len);
	ফেরত 0;
}

/**
 * Saves state->msg in the state directory's "final-commit" file.
 */
স্থির শূন্য write_commit_বার্তা(ধ্রুবক কাঠামো am_state *state)
{
	ধ্রুবক অক্ষর *filename = am_path(state, "final-commit");
	write_file_buf(filename, state->বার্তা, state->বার্তা_len);
}

/**
 * Loads state from disk.
 */
স্থির শূন্য am_load(কাঠামো am_state *state)
{
	কাঠামো strbuf sb = STRBUF_INIT;

	যদি (read_state_file(&sb, state, "next", 1) < 0)
		BUG("state file 'next' does not exist");
	state->cur = strtol(sb.buf, ফাঁকা, 10);

	যদি (read_state_file(&sb, state, "last", 1) < 0)
		BUG("state file 'last' does not exist");
	state->last = strtol(sb.buf, ফাঁকা, 10);

	যদি (read_am_author_script(state) < 0)
		die(_("could not parse author script"));

	read_commit_বার্তা(state);

	যদি (read_state_file(&sb, state, "original-commit", 1) < 0)
		oidclr(&state->orig_commit, the_repository->hash_algo);
	অন্যথা যদি (get_oid_hex(sb.buf, &state->orig_commit) < 0)
		die(_("could not parse %s"), am_path(state, "original-commit"));

	read_state_file(&sb, state, "threeway", 1);
	state->threeway = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "quiet", 1);
	state->quiet = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "sign", 1);
	state->signoff = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "utf8", 1);
	state->utf8 = !strcmp(sb.buf, "t");

	যদি (file_exists(am_path(state, "rerere-autoupdate"))) {
		read_state_file(&sb, state, "rerere-autoupdate", 1);
		state->allow_rerere_autoupdate = strcmp(sb.buf, "t") ?
			RERERE_NOAUTOUPDATE : RERERE_AUTOUPDATE;
	} অন্যথা {
		state->allow_rerere_autoupdate = 0;
	}

	read_state_file(&sb, state, "keep", 1);
	যদি (!strcmp(sb.buf, "t"))
		state->keep = KEEP_TRUE;
	অন্যথা যদি (!strcmp(sb.buf, "b"))
		state->keep = KEEP_NON_PATCH;
	অন্যথা
		state->keep = KEEP_FALSE;

	read_state_file(&sb, state, "messageid", 1);
	state->message_id = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "scissors", 1);
	যদি (!strcmp(sb.buf, "t"))
		state->scissors = SCISSORS_TRUE;
	অন্যথা যদি (!strcmp(sb.buf, "f"))
		state->scissors = SCISSORS_FALSE;
	অন্যথা
		state->scissors = SCISSORS_UNSET;

	read_state_file(&sb, state, "quoted-cr", 1);
	যদি (!*sb.buf)
		state->quoted_cr = quoted_cr_unset;
	অন্যথা যদি (mailinfo_parse_quoted_cr_action(sb.buf, &state->quoted_cr) != 0)
		die(_("could not parse %s"), am_path(state, "quoted-cr"));

	read_state_file(&sb, state, "apply-opt", 1);
	strvec_clear(&state->git_apply_opts);
	যদি (sq_dequote_to_strvec(sb.buf, &state->git_apply_opts) < 0)
		die(_("could not parse %s"), am_path(state, "apply-opt"));

	state->rebasing = !!file_exists(am_path(state, "rebasing"));

	strbuf_release(&sb);
}

/**
 * Removes the am_state directory, forcefully terminating the current am
 * session.
 */
স্থির শূন্য am_destroy(ধ্রুবক কাঠামো am_state *state)
{
	কাঠামো strbuf sb = STRBUF_INIT;

	strbuf_addstr(&sb, state->dir);
	remove_dir_recursively(&sb, 0);
	strbuf_release(&sb);
}

/**
 * Runs applypatch-msg hook. Returns its exit code.
 */
স্থির পূর্ণসংখ্যা run_applypatch_বার্তা_hook(কাঠামো am_state *state)
{
	পূর্ণসংখ্যা ret = 0;

	প্রতিজ্ঞা(state->বার্তা);

	যদি (!state->no_verify) {
		কাঠামো run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;
		strvec_ঠেলো(&opt.args, am_path(state, "final-commit"));
		ret = run_hooks_opt(the_repository, "applypatch-msg", &opt);
	}

	যদি (!ret) {
		FREE_AND_NULL(state->বার্তা);
		যদি (read_commit_বার্তা(state) < 0)
			die(_("'%s' was deleted by the applypatch-msg hook"),
				am_path(state, "final-commit"));
	}

	ফেরত ret;
}

/**
 * Runs post-rewrite hook. Returns it exit code.
 */
স্থির পূর্ণসংখ্যা run_post_rewrite_hook(ধ্রুবক কাঠামো am_state *state)
{
	কাঠামো run_hooks_opt opt = RUN_HOOKS_OPT_INIT;

	strvec_ঠেলো(&opt.args, "rebase");
	opt.path_to_stdin = am_path(state, "rewritten");

	ফেরত run_hooks_opt(the_repository, "post-rewrite", &opt);
}

/**
 * Reads the state directory's "rewritten" file, and copies notes from the old
 * commits listed in the file to their rewritten commits.
 *
 * Returns 0 on success, -1 on failure.
 */
স্থির পূর্ণসংখ্যা copy_notes_for_rebase(ধ্রুবক কাঠামো am_state *state)
{
	কাঠামো notes_rewrite_cfg *c;
	কাঠামো strbuf sb = STRBUF_INIT;
	ধ্রুবক অক্ষর *invalid_সারি = _("Malformed input line: '%s'.");
	ধ্রুবক অক্ষর *বার্তা = "Notes added by 'git rebase'";
	FILE *fp;
	পূর্ণসংখ্যা ret = 0;

	প্রতিজ্ঞা(state->rebasing);

	c = init_copy_notes_for_rewrite("rebase");
	যদি (!c)
		ফেরত 0;

	fp = xfopen(am_path(state, "rewritten"), "r");

	যখন (!strbuf_getline_lf(&sb, fp)) {
		কাঠামো object_id from_obj, to_obj;
		ধ্রুবক অক্ষর *p;

		যদি (sb.len != the_hash_algo->hexsz * 2 + 1) {
			ret = error(invalid_সারি, sb.buf);
			যাও finish;
		}

		যদি (parse_oid_hex(sb.buf, &from_obj, &p)) {
			ret = error(invalid_সারি, sb.buf);
			যাও finish;
		}

		যদি (*p != ' ') {
			ret = error(invalid_সারি, sb.buf);
			যাও finish;
		}

		যদি (get_oid_hex(p + 1, &to_obj)) {
			ret = error(invalid_সারি, sb.buf);
			যাও finish;
		}

		যদি (copy_note_for_rewrite(c, &from_obj, &to_obj))
			ret = error(_("Failed to copy notes from '%s' to '%s'"),
					oid_to_hex(&from_obj), oid_to_hex(&to_obj));
	}

finish:
	finish_copy_notes_for_rewrite(the_repository, c, বার্তা);
	fclose(fp);
	strbuf_release(&sb);
	ফেরত ret;
}

/**
 * Determines if the file looks like a piece of RFC2822 mail by grabbing all
 * non-indented lines and checking if they look like they begin with valid
 * header field names.
 *
 * Returns 1 if the file looks like a piece of mail, 0 otherwise.
 */
স্থির পূর্ণসংখ্যা is_mail(FILE *fp)
{
	ধ্রুবক অক্ষর *header_regex = "^[!-9;-~]+:";
	কাঠামো strbuf sb = STRBUF_INIT;
	regex_t regex;
	পূর্ণসংখ্যা ret = 1;

	যদি (fseek(fp, 0L, SEEK_SET))
		die_errno(_("fseek failed"));

	যদি (regcomp(&regex, header_regex, REG_NOSUB | REG_EXTENDED))
		die("invalid pattern: %s", header_regex);

	যখন (!strbuf_getline(&sb, fp)) {
		যদি (!sb.len)
			বিরতি; /* End of header */

		/* Ignore indented folded lines */
		যদি (*sb.buf == '\t' || *sb.buf == ' ')
			চালু;

		/* It's a header if it matches header_regex */
		যদি (regexec(&regex, sb.buf, 0, ফাঁকা, 0)) {
			ret = 0;
			যাও done;
		}
	}

done:
	regfree(&regex);
	strbuf_release(&sb);
	ফেরত ret;
}

/**
 * Attempts to detect the patch_format of the patches contained in `paths`,
 * returning the PATCH_FORMAT_* enum value. Returns PATCH_FORMAT_UNKNOWN if
 * detection fails.
 */
স্থির পূর্ণসংখ্যা detect_patch_format(ধ্রুবক অক্ষর **paths)
{
	গণনা patch_format ret = PATCH_FORMAT_UNKNOWN;
	কাঠামো strbuf l1 = STRBUF_INIT;
	কাঠামো strbuf l2 = STRBUF_INIT;
	কাঠামো strbuf l3 = STRBUF_INIT;
	FILE *fp;

	/*
	 * We default to mbox format if input is from stdin and for directories
	 */
	যদি (!*paths || !strcmp(*paths, "-") || is_directory(*paths))
		ফেরত PATCH_FORMAT_MBOX;

	/*
	 * Otherwise, check the first few lines of the first patch, starting
	 * from the first non-blank line, to try to detect its format.
	 */

	fp = xfopen(*paths, "r");

	যখন (!strbuf_getline(&l1, fp)) {
		যদি (l1.len)
			বিরতি;
	}

	যদি (starts_with(l1.buf, "From ") || starts_with(l1.buf, "From: ")) {
		ret = PATCH_FORMAT_MBOX;
		যাও done;
	}

	যদি (starts_with(l1.buf, "# This series applies on GIT commit")) {
		ret = PATCH_FORMAT_STGIT_SERIES;
		যাও done;
	}

	যদি (!strcmp(l1.buf, "# HG changeset patch")) {
		ret = PATCH_FORMAT_HG;
		যাও done;
	}

	strbuf_getline(&l2, fp);
	strbuf_getline(&l3, fp);

	/*
	 * If the second line is empty and the third is a From, Author or Date
	 * entry, this is likely an StGit patch.
	 */
	যদি (l1.len && !l2.len &&
		(starts_with(l3.buf, "From:") ||
		 starts_with(l3.buf, "Author:") ||
		 starts_with(l3.buf, "Date:"))) {
		ret = PATCH_FORMAT_STGIT;
		যাও done;
	}

	যদি (l1.len && is_mail(fp)) {
		ret = PATCH_FORMAT_MBOX;
		যাও done;
	}

done:
	fclose(fp);
	strbuf_release(&l1);
	strbuf_release(&l2);
	strbuf_release(&l3);
	ফেরত ret;
}

/**
 * Splits out individual email patches from `paths`, where each path is either
 * a mbox file or a Maildir. Returns 0 on success, -1 on failure.
 */
স্থির পূর্ণসংখ্যা split_mail_mbox(কাঠামো am_state *state, ধ্রুবক অক্ষর **paths,
				পূর্ণসংখ্যা keep_cr, পূর্ণসংখ্যা mboxrd)
{
	কাঠামো child_process cp = CHILD_PROCESS_INIT;
	কাঠামো strbuf last = STRBUF_INIT;
	পূর্ণসংখ্যা ret;

	cp.git_cmd = 1;
	strvec_ঠেলো(&cp.args, "mailsplit");
	strvec_pushf(&cp.args, "-d%d", state->prec);
	strvec_pushf(&cp.args, "-o%s", state->dir);
	strvec_ঠেলো(&cp.args, "-b");
	যদি (keep_cr)
		strvec_ঠেলো(&cp.args, "--keep-cr");
	যদি (mboxrd)
		strvec_ঠেলো(&cp.args, "--mboxrd");
	strvec_ঠেলো(&cp.args, "--");
	strvec_pushv(&cp.args, paths);

	ret = capture_command(&cp, &last, 8);
	যদি (ret)
		যাও exit;

	state->cur = 1;
	state->last = strtol(last.buf, ফাঁকা, 10);

exit:
	strbuf_release(&last);
	ফেরত ret ? -1 : 0;
}

/**
 * Callback signature for split_mail_conv(). The foreign patch should be
 * read from `in`, and the converted patch (in RFC2822 mail format) should be
 * written to `out`. Return 0 on success, or -1 on failure.
 */
প্রকারলেখ পূর্ণসংখ্যা (*mail_conv_fn)(FILE *out, FILE *in, পূর্ণসংখ্যা keep_cr);

/**
 * Calls `fn` for each file in `paths` to convert the foreign patch to the
 * RFC2822 mail format suitable for parsing with git-mailinfo.
 *
 * Returns 0 on success, -1 on failure.
 */
স্থির পূর্ণসংখ্যা split_mail_conv(mail_conv_fn fn, কাঠামো am_state *state,
			ধ্রুবক অক্ষর **paths, পূর্ণসংখ্যা keep_cr)
{
	স্থির ধ্রুবক অক্ষর *stdin_only[] = {"-", ফাঁকা};
	পূর্ণসংখ্যা i;

	যদি (!*paths)
		paths = stdin_only;

	জন্য (i = 0; *paths; paths++, i++) {
		FILE *in, *out;
		ধ্রুবক অক্ষর *mail;
		পূর্ণসংখ্যা ret;

		যদি (!strcmp(*paths, "-"))
			in = stdin;
		অন্যথা
			in = fopen(*paths, "r");

		যদি (!in)
			ফেরত error_errno(_("could not open '%s' for reading"),
					   *paths);

		mail = mkpath("%s/%0*d", state->dir, state->prec, i + 1);

		out = fopen(mail, "w");
		যদি (!out) {
			যদি (in != stdin)
				fclose(in);
			ফেরত error_errno(_("could not open '%s' for writing"),
					   mail);
		}

		ret = fn(out, in, keep_cr);

		fclose(out);
		যদি (in != stdin)
			fclose(in);

		যদি (ret)
			ফেরত error(_("could not parse patch '%s'"), *paths);
	}

	state->cur = 1;
	state->last = i;
	ফেরত 0;
}

/**
 * A split_mail_conv() callback that converts an StGit patch to an RFC2822
 * message suitable for parsing with git-mailinfo.
 */
স্থির পূর্ণসংখ্যা stgit_patch_to_mail(FILE *out, FILE *in, পূর্ণসংখ্যা keep_cr UNUSED)
{
	কাঠামো strbuf sb = STRBUF_INIT;
	পূর্ণসংখ্যা subject_printed = 0;

	যখন (!strbuf_getline_lf(&sb, in)) {
		ধ্রুবক অক্ষর *str;

		যদি (str_isspace(sb.buf))
			চালু;
		অন্যথা যদি (skip_prefix(sb.buf, "Author:", &str))
			fprintf(out, "From:%s\n", str);
		অন্যথা যদি (starts_with(sb.buf, "From") || starts_with(sb.buf, "Date"))
			fprintf(out, "%s\n", sb.buf);
		অন্যথা যদি (!subject_printed) {
			fprintf(out, "Subject: %s\n", sb.buf);
			subject_printed = 1;
		} অন্যথা {
			fprintf(out, "\n%s\n", sb.buf);
			বিরতি;
		}
	}

	strbuf_reset(&sb);
	যখন (strbuf_fread(&sb, 8192, in) > 0) {
		fwrite(sb.buf, 1, sb.len, out);
		strbuf_reset(&sb);
	}

	strbuf_release(&sb);
	ফেরত 0;
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
স্থির পূর্ণসংখ্যা split_mail_stgit_series(কাঠামো am_state *state, ধ্রুবক অক্ষর **paths,
					পূর্ণসংখ্যা keep_cr)
{
	ধ্রুবক অক্ষর *series_dir;
	অক্ষর *series_dir_buf;
	FILE *fp;
	কাঠামো strvec patches = STRVEC_INIT;
	কাঠামো strbuf sb = STRBUF_INIT;
	পূর্ণসংখ্যা ret;

	যদি (!paths[0] || paths[1])
		ফেরত error(_("Only one StGIT patch series can be applied at once"));

	series_dir_buf = xstrdup(*paths);
	series_dir = dirname(series_dir_buf);

	fp = fopen(*paths, "r");
	যদি (!fp) {
		মুক্ত(series_dir_buf);
		ফেরত error_errno(_("could not open '%s' for reading"), *paths);
	}

	যখন (!strbuf_getline_lf(&sb, fp)) {
		যদি (*sb.buf == '#')
			চালু; /* skip comment lines */

		strvec_ঠেলো(&patches, mkpath("%s/%s", series_dir, sb.buf));
	}

	fclose(fp);
	strbuf_release(&sb);
	মুক্ত(series_dir_buf);

	ret = split_mail_conv(stgit_patch_to_mail, state, patches.v, keep_cr);

	strvec_clear(&patches);
	ফেরত ret;
}

/**
 * A split_patches_conv() callback that converts a mercurial patch to a RFC2822
 * message suitable for parsing with git-mailinfo.
 */
স্থির পূর্ণসংখ্যা hg_patch_to_mail(FILE *out, FILE *in, পূর্ণসংখ্যা keep_cr UNUSED)
{
	কাঠামো strbuf sb = STRBUF_INIT;
	পূর্ণসংখ্যা rc = 0;

	যখন (!strbuf_getline_lf(&sb, in)) {
		ধ্রুবক অক্ষর *str;

		যদি (skip_prefix(sb.buf, "# User ", &str))
			fprintf(out, "From: %s\n", str);
		অন্যথা যদি (skip_prefix(sb.buf, "# Date ", &str)) {
			timestamp_t timestamp;
			long tz, tz2;
			অক্ষর *end;

			errno = 0;
			timestamp = parse_timestamp(str, &end, 10);
			যদি (errno) {
				rc = error(_("invalid timestamp"));
				যাও exit;
			}

			যদি (!skip_prefix(end, " ", &str)) {
				rc = error(_("invalid Date line"));
				যাও exit;
			}

			errno = 0;
			tz = strtol(str, &end, 10);
			যদি (errno) {
				rc = error(_("invalid timezone offset"));
				যাও exit;
			}

			যদি (*end) {
				rc = error(_("invalid Date line"));
				যাও exit;
			}

			/*
			 * mercurial's timezone is in seconds west of UTC,
			 * however git's timezone is in hours + minutes east of
			 * UTC. Convert it.
			 */
			tz2 = labs(tz) / 3600 * 100 + labs(tz) % 3600 / 60;
			যদি (tz > 0)
				tz2 = -tz2;

			fprintf(out, "Date: %s\n", show_date(timestamp, tz2, DATE_MODE(RFC2822)));
		} অন্যথা যদি (starts_with(sb.buf, "# ")) {
			চালু;
		} অন্যথা {
			fprintf(out, "\n%s\n", sb.buf);
			বিরতি;
		}
	}

	strbuf_reset(&sb);
	যখন (strbuf_fread(&sb, 8192, in) > 0) {
		fwrite(sb.buf, 1, sb.len, out);
		strbuf_reset(&sb);
	}
exit:
	strbuf_release(&sb);
	ফেরত rc;
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
স্থির পূর্ণসংখ্যা split_mail(কাঠামো am_state *state, গণনা patch_format patch_format,
			ধ্রুবক অক্ষর **paths, পূর্ণসংখ্যা keep_cr)
{
	যদি (keep_cr < 0) {
		keep_cr = 0;
		repo_config_get_bool(the_repository, "am.keepcr", &keep_cr);
	}

	নির্বাচন (patch_format) {
	ক্ষেত্র PATCH_FORMAT_MBOX:
		ফেরত split_mail_mbox(state, paths, keep_cr, 0);
	ক্ষেত্র PATCH_FORMAT_STGIT:
		ফেরত split_mail_conv(stgit_patch_to_mail, state, paths, keep_cr);
	ক্ষেত্র PATCH_FORMAT_STGIT_SERIES:
		ফেরত split_mail_stgit_series(state, paths, keep_cr);
	ক্ষেত্র PATCH_FORMAT_HG:
		ফেরত split_mail_conv(hg_patch_to_mail, state, paths, keep_cr);
	ক্ষেত্র PATCH_FORMAT_MBOXRD:
		ফেরত split_mail_mbox(state, paths, keep_cr, 1);
	পূর্বনির্ধারিত:
		BUG("invalid patch_format");
	}
	ফেরত -1;
}

/**
 * Setup a new am session for applying patches
 */
স্থির শূন্য am_setup(কাঠামো am_state *state, গণনা patch_format patch_format,
			ধ্রুবক অক্ষর **paths, পূর্ণসংখ্যা keep_cr)
{
	কাঠামো object_id curr_শীর্ষ;
	ধ্রুবক অক্ষর *str;
	কাঠামো strbuf sb = STRBUF_INIT;

	যদি (!patch_format)
		patch_format = detect_patch_format(paths);

	যদি (!patch_format) {
		fprintf_ln(stderr, _("Patch format detection failed."));
		die(ফাঁকা);
	}

	যদি (mkdir(state->dir, 0777) < 0 && errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), state->dir);
	refs_delete_ref(get_main_ref_store(the_repository), ফাঁকা,
			"REBASE_HEAD", ফাঁকা, REF_NO_DEREF);

	যদি (split_mail(state, patch_format, paths, keep_cr) < 0) {
		am_destroy(state);
		die(_("Failed to split patches."));
	}

	যদি (state->rebasing)
		state->threeway = 1;

	write_state_bool(state, "threeway", state->threeway);
	write_state_bool(state, "quiet", state->quiet);
	write_state_bool(state, "sign", state->signoff);
	write_state_bool(state, "utf8", state->utf8);

	যদি (state->allow_rerere_autoupdate)
		write_state_bool(state, "rerere-autoupdate",
			 state->allow_rerere_autoupdate == RERERE_AUTOUPDATE);

	নির্বাচন (state->keep) {
	ক্ষেত্র KEEP_FALSE:
		str = "f";
		বিরতি;
	ক্ষেত্র KEEP_TRUE:
		str = "t";
		বিরতি;
	ক্ষেত্র KEEP_NON_PATCH:
		str = "b";
		বিরতি;
	পূর্বনির্ধারিত:
		BUG("invalid value for state->keep");
	}

	write_state_text(state, "keep", str);
	write_state_bool(state, "messageid", state->message_id);

	নির্বাচন (state->scissors) {
	ক্ষেত্র SCISSORS_UNSET:
		str = "";
		বিরতি;
	ক্ষেত্র SCISSORS_FALSE:
		str = "f";
		বিরতি;
	ক্ষেত্র SCISSORS_TRUE:
		str = "t";
		বিরতি;
	পূর্বনির্ধারিত:
		BUG("invalid value for state->scissors");
	}
	write_state_text(state, "scissors", str);

	নির্বাচন (state->quoted_cr) {
	ক্ষেত্র quoted_cr_unset:
		str = "";
		বিরতি;
	ক্ষেত্র quoted_cr_nowarn:
		str = "nowarn";
		বিরতি;
	ক্ষেত্র quoted_cr_warn:
		str = "warn";
		বিরতি;
	ক্ষেত্র quoted_cr_strip:
		str = "strip";
		বিরতি;
	পূর্বনির্ধারিত:
		BUG("invalid value for state->quoted_cr");
	}
	write_state_text(state, "quoted-cr", str);

	sq_quote_argv(&sb, state->git_apply_opts.v);
	write_state_text(state, "apply-opt", sb.buf);

	যদি (state->rebasing)
		write_state_text(state, "rebasing", "");
	অন্যথা
		write_state_text(state, "applying", "");

	যদি (!repo_get_oid(the_repository, "HEAD", &curr_শীর্ষ)) {
		write_state_text(state, "abort-safety", oid_to_hex(&curr_শীর্ষ));
		যদি (!state->rebasing)
			refs_update_ref(get_main_ref_store(the_repository),
					"am", "ORIG_HEAD", &curr_শীর্ষ, ফাঁকা,
					0,
					UPDATE_REFS_DIE_ON_ERR);
	} অন্যথা {
		write_state_text(state, "abort-safety", "");
		যদি (!state->rebasing)
			refs_delete_ref(get_main_ref_store(the_repository),
					ফাঁকা, "ORIG_HEAD", ফাঁকা, 0);
	}

	/*
	 * NOTE: Since the "next" and "last" files determine if an am_state
	 * session is in progress, they should be written last.
	 */

	write_state_গুনতি(state, "next", state->cur);
	write_state_গুনতি(state, "last", state->last);

	strbuf_release(&sb);
}

/**
 * Increments the patch pointer, and cleans am_state for the application of the
 * next patch.
 */
স্থির শূন্য am_পরবর্তী(কাঠামো am_state *state)
{
	কাঠামো object_id শীর্ষ;

	FREE_AND_NULL(state->author_name);
	FREE_AND_NULL(state->author_email);
	FREE_AND_NULL(state->author_date);
	FREE_AND_NULL(state->বার্তা);
	state->বার্তা_len = 0;

	unlink(am_path(state, "author-script"));
	unlink(am_path(state, "final-commit"));

	oidclr(&state->orig_commit, the_repository->hash_algo);
	unlink(am_path(state, "original-commit"));
	refs_delete_ref(get_main_ref_store(the_repository), ফাঁকা,
			"REBASE_HEAD", ফাঁকা, REF_NO_DEREF);

	যদি (!repo_get_oid(the_repository, "HEAD", &শীর্ষ))
		write_state_text(state, "abort-safety", oid_to_hex(&শীর্ষ));
	অন্যথা
		write_state_text(state, "abort-safety", "");

	state->cur++;
	write_state_গুনতি(state, "next", state->cur);
}

/**
 * Returns the filename of the current patch email.
 */
স্থির ধ্রুবক অক্ষর *msgnum(ধ্রুবক কাঠামো am_state *state)
{
	স্থির কাঠামো strbuf sb = STRBUF_INIT;

	strbuf_reset(&sb);
	strbuf_addf(&sb, "%0*d", state->prec, state->cur);

	ফেরত sb.buf;
}

/**
 * Dies with a user-friendly message on how to proceed after resolving the
 * problem. This message can be overridden with state->resolvemsg.
 */
স্থির শূন্য NORETURN die_user_resolve(ধ্রুবক কাঠামো am_state *state)
{
	যদি (state->resolvemsg) {
		advise_if_enabled(ADVICE_MERGE_CONFLICT, "%s", state->resolvemsg);
	} অন্যথা {
		ধ্রুবক অক্ষর *cmdline = state->interactive ? "git am -i" : "git am";
		কাঠামো strbuf sb = STRBUF_INIT;

		strbuf_addf(&sb, _("When you have resolved this problem, run \"%s --continue\".\n"), cmdline);
		strbuf_addf(&sb, _("If you prefer to skip this patch, run \"%s --skip\" instead.\n"), cmdline);

		যদি (advice_enabled(ADVICE_AM_WORK_DIR) &&
		    is_empty_or_missing_file(am_path(state, "patch")) &&
		    !repo_সূচক_has_changes(the_repository, ফাঁকা, ফাঁকা))
			strbuf_addf(&sb, _("To record the empty patch as an empty commit, run \"%s --allow-empty\".\n"), cmdline);

		strbuf_addf(&sb, _("To restore the original branch and stop patching, run \"%s --abort\"."), cmdline);

		advise_if_enabled(ADVICE_MERGE_CONFLICT, "%s", sb.buf);
		strbuf_release(&sb);
	}

	die(ফাঁকা);
}

/**
 * Appends signoff to the "msg" field of the am_state.
 */
স্থির শূন্য am_append_signoff(কাঠামো am_state *state)
{
	কাঠামো strbuf sb = STRBUF_INIT;

	strbuf_attach(&sb, state->বার্তা, state->বার্তা_len, state->বার্তা_len + 1);
	append_signoff(&sb, 0, 0);
	state->বার্তা = strbuf_detach(&sb, &state->বার্তা_len);
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
স্থির পূর্ণসংখ্যা parse_mail(কাঠামো am_state *state, ধ্রুবক অক্ষর *mail)
{
	FILE *fp;
	কাঠামো strbuf sb = STRBUF_INIT;
	কাঠামো strbuf বার্তা = STRBUF_INIT;
	কাঠামো strbuf author_name = STRBUF_INIT;
	কাঠামো strbuf author_date = STRBUF_INIT;
	কাঠামো strbuf author_email = STRBUF_INIT;
	পূর্ণসংখ্যা ret = 0;
	কাঠামো mailinfo mi;

	setup_mailinfo(the_repository, &mi);

	যদি (state->utf8)
		mi.metainfo_charset = get_commit_output_encoding();
	অন্যথা
		mi.metainfo_charset = ফাঁকা;

	নির্বাচন (state->keep) {
	ক্ষেত্র KEEP_FALSE:
		বিরতি;
	ক্ষেত্র KEEP_TRUE:
		mi.keep_subject = 1;
		বিরতি;
	ক্ষেত্র KEEP_NON_PATCH:
		mi.keep_non_patch_brackets_in_subject = 1;
		বিরতি;
	পূর্বনির্ধারিত:
		BUG("invalid value for state->keep");
	}

	যদি (state->message_id)
		mi.add_message_id = 1;

	নির্বাচন (state->scissors) {
	ক্ষেত্র SCISSORS_UNSET:
		বিরতি;
	ক্ষেত্র SCISSORS_FALSE:
		mi.use_scissors = 0;
		বিরতি;
	ক্ষেত্র SCISSORS_TRUE:
		mi.use_scissors = 1;
		বিরতি;
	পূর্বনির্ধারিত:
		BUG("invalid value for state->scissors");
	}

	নির্বাচন (state->quoted_cr) {
	ক্ষেত্র quoted_cr_unset:
		বিরতি;
	ক্ষেত্র quoted_cr_nowarn:
	ক্ষেত্র quoted_cr_warn:
	ক্ষেত্র quoted_cr_strip:
		mi.quoted_cr = state->quoted_cr;
		বিরতি;
	পূর্বনির্ধারিত:
		BUG("invalid value for state->quoted_cr");
	}

	mi.প্রবেশ = xfopen(mail, "r");
	mi.output = xfopen(am_path(state, "info"), "w");
	যদি (mailinfo(&mi, am_path(state, "msg"), am_path(state, "patch")))
		die("could not parse patch");

	fclose(mi.প্রবেশ);
	fclose(mi.output);

	যদি (mi.format_flowed)
		warning(_("Patch sent with format=flowed; "
			  "space at the end of lines might be lost."));

	/* Extract message and author information */
	fp = xfopen(am_path(state, "info"), "r");
	যখন (!strbuf_getline_lf(&sb, fp)) {
		ধ্রুবক অক্ষর *x;

		যদি (skip_prefix(sb.buf, "Subject: ", &x)) {
			যদি (বার্তা.len)
				strbuf_addch(&বার্তা, '\n');
			strbuf_addstr(&বার্তা, x);
		} অন্যথা যদি (skip_prefix(sb.buf, "Author: ", &x))
			strbuf_addstr(&author_name, x);
		অন্যথা যদি (skip_prefix(sb.buf, "Email: ", &x))
			strbuf_addstr(&author_email, x);
		অন্যথা যদি (skip_prefix(sb.buf, "Date: ", &x))
			strbuf_addstr(&author_date, x);
	}
	fclose(fp);

	/* Skip pine's internal folder data */
	যদি (!strcmp(author_name.buf, "Mail System Internal Data")) {
		ret = 1;
		যাও finish;
	}

	strbuf_addstr(&বার্তা, "\n\n");
	strbuf_addbuf(&বার্তা, &mi.log_message);
	strbuf_stripspace(&বার্তা, ফাঁকা);

	প্রতিজ্ঞা(!state->author_name);
	state->author_name = strbuf_detach(&author_name, ফাঁকা);

	প্রতিজ্ঞা(!state->author_email);
	state->author_email = strbuf_detach(&author_email, ফাঁকা);

	প্রতিজ্ঞা(!state->author_date);
	state->author_date = strbuf_detach(&author_date, ফাঁকা);

	প্রতিজ্ঞা(!state->বার্তা);
	state->বার্তা = strbuf_detach(&বার্তা, &state->বার্তা_len);

finish:
	strbuf_release(&বার্তা);
	strbuf_release(&author_date);
	strbuf_release(&author_email);
	strbuf_release(&author_name);
	strbuf_release(&sb);
	clear_mailinfo(&mi);
	ফেরত ret;
}

/**
 * Sets commit_id to the commit hash where the mail was generated from.
 * Returns 0 on success, -1 on failure.
 */
স্থির পূর্ণসংখ্যা get_mail_commit_oid(কাঠামো object_id *commit_id, ধ্রুবক অক্ষর *mail)
{
	কাঠামো strbuf sb = STRBUF_INIT;
	FILE *fp = xfopen(mail, "r");
	ধ্রুবক অক্ষর *x;
	পূর্ণসংখ্যা ret = 0;

	যদি (strbuf_getline_lf(&sb, fp) ||
	    !skip_prefix(sb.buf, "From ", &x) ||
	    get_oid_hex(x, commit_id) < 0)
		ret = -1;

	strbuf_release(&sb);
	fclose(fp);
	ফেরত ret;
}

/**
 * Sets state->msg, state->author_name, state->author_email, state->author_date
 * to the commit's respective info.
 */
স্থির শূন্য get_commit_info(কাঠামো am_state *state, কাঠামো commit *commit)
{
	ধ্রুবক অক্ষর *বাফার, *ident_সারি, *বার্তা;
	আকার_t ident_len;
	কাঠামো ident_split id;

	বাফার = repo_logmsg_reencode(the_repository, commit, ফাঁকা,
				      get_commit_output_encoding());

	ident_সারি = find_commit_header(বাফার, "author", &ident_len);
	যদি (!ident_সারি)
		die(_("missing author line in commit %s"),
		      oid_to_hex(&commit->object.oid));
	যদি (split_ident_সারি(&id, ident_সারি, ident_len) < 0)
		die(_("invalid ident line: %.*s"), (পূর্ণসংখ্যা)ident_len, ident_সারি);

	প্রতিজ্ঞা(!state->author_name);
	যদি (id.name_begin)
		state->author_name =
			xmemdupz(id.name_begin, id.name_end - id.name_begin);
	অন্যথা
		state->author_name = xstrdup("");

	প্রতিজ্ঞা(!state->author_email);
	যদি (id.mail_begin)
		state->author_email =
			xmemdupz(id.mail_begin, id.mail_end - id.mail_begin);
	অন্যথা
		state->author_email = xstrdup("");

	প্রতিজ্ঞা(!state->author_date);
	state->author_date = xstrdup(show_ident_date(&id, DATE_MODE(NORMAL)));

	প্রতিজ্ঞা(!state->বার্তা);
	বার্তা = strstr(বাফার, "\n\n");
	যদি (!বার্তা)
		die(_("unable to parse commit %s"), oid_to_hex(&commit->object.oid));
	state->বার্তা = xstrdup(বার্তা + 2);
	state->বার্তা_len = strlen(state->বার্তা);
	repo_unuse_commit_বাফার(the_repository, commit, বাফার);
}

/**
 * Writes `commit` as a patch to the state directory's "patch" file.
 */
স্থির শূন্য write_commit_patch(ধ্রুবক কাঠামো am_state *state, কাঠামো commit *commit)
{
	কাঠামো rev_info rev_info;
	FILE *fp;

	fp = xfopen(am_path(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, ফাঁকা);
	rev_info.diff = 1;
	rev_info.abbrev = 0;
	rev_info.disable_stdin = 1;
	rev_info.show_মূল_diff = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.flags.দ্বিমিক = 1;
	rev_info.diffopt.flags.full_সূচক = 1;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	add_pending_object(&rev_info, &commit->object, "");
	diff_setup_done(&rev_info.diffopt);
	log_বৃক্ষ_commit(&rev_info, commit);
	release_revisions(&rev_info);
}

/**
 * Writes the diff of the index against HEAD as a patch to the state
 * directory's "patch" file.
 */
স্থির শূন্য write_সূচক_patch(ধ্রুবক কাঠামো am_state *state)
{
	কাঠামো বৃক্ষ *বৃক্ষ;
	কাঠামো object_id শীর্ষ;
	কাঠামো rev_info rev_info;
	FILE *fp;

	যদি (!repo_get_oid(the_repository, "HEAD", &শীর্ষ)) {
		কাঠামো commit *commit = lookup_commit_or_die(&শীর্ষ, "HEAD");
		বৃক্ষ = repo_get_commit_বৃক্ষ(the_repository, commit);
	} অন্যথা
		বৃক্ষ = lookup_বৃক্ষ(the_repository,
				   the_repository->hash_algo->empty_বৃক্ষ);

	fp = xfopen(am_path(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, ফাঁকা);
	rev_info.diff = 1;
	rev_info.disable_stdin = 1;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	add_pending_object(&rev_info, &বৃক্ষ->object, "");
	diff_setup_done(&rev_info.diffopt);
	run_diff_সূচক(&rev_info, DIFF_সূচক_CACHED);
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
স্থির পূর্ণসংখ্যা parse_mail_rebase(কাঠামো am_state *state, ধ্রুবক অক্ষর *mail)
{
	কাঠামো commit *commit;
	কাঠামো object_id commit_oid;

	যদি (get_mail_commit_oid(&commit_oid, mail) < 0)
		die(_("could not parse %s"), mail);

	commit = lookup_commit_or_die(&commit_oid, mail);

	get_commit_info(state, commit);

	write_commit_patch(state, commit);

	oidcpy(&state->orig_commit, &commit_oid);
	write_state_text(state, "original-commit", oid_to_hex(&commit_oid));
	refs_update_ref(get_main_ref_store(the_repository), "am",
			"REBASE_HEAD", &commit_oid,
			ফাঁকা, REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);

	ফেরত 0;
}

/**
 * Applies current patch with git-apply. Returns 0 on success, -1 otherwise. If
 * `index_file` is not NULL, the patch will be applied to that index.
 */
স্থির পূর্ণসংখ্যা run_apply(ধ্রুবক কাঠামো am_state *state, ধ্রুবক অক্ষর *সূচক_file)
{
	কাঠামো strvec apply_paths = STRVEC_INIT;
	কাঠামো strvec apply_opts = STRVEC_INIT;
	কাঠামো apply_state apply_state;
	পূর্ণসংখ্যা res, opts_বাম;
	পূর্ণসংখ্যা force_apply = 0;
	পূর্ণসংখ্যা options = 0;
	ধ্রুবক অক্ষর **apply_argv;

	যদি (init_apply_state(&apply_state, the_repository, ফাঁকা))
		BUG("init_apply_state() failed");

	strvec_ঠেলো(&apply_opts, "apply");
	strvec_pushv(&apply_opts, state->git_apply_opts.v);

	/*
	 * Build a copy that apply_parse_options() can rearrange.
	 * apply_opts.v keeps referencing the allocated strings for
	 * strvec_clear() to release.
	 */
	DUP_ARRAY(apply_argv, apply_opts.v, apply_opts.nr);

	opts_বাম = apply_parse_options(apply_opts.nr, apply_argv,
					&apply_state, &force_apply, &options,
					ফাঁকা);

	যদি (opts_বাম != 0)
		die("unknown option passed through to git apply");

	যদি (সূচক_file) {
		apply_state.সূচক_file = সূচক_file;
		apply_state.cached = 1;
	} অন্যথা
		apply_state.check_সূচক = 1;

	/*
	 * If we are allowed to fall back on 3-way merge, don't give false
	 * errors during the initial attempt.
	 */
	যদি (state->threeway && !সূচক_file)
		apply_state.apply_verbosity = verbosity_silent;

	যদি (check_apply_state(&apply_state, force_apply))
		BUG("check_apply_state() failed");

	strvec_ঠেলো(&apply_paths, am_path(state, "patch"));

	res = apply_all_patches(&apply_state, apply_paths.nr, apply_paths.v, options);

	strvec_clear(&apply_paths);
	strvec_clear(&apply_opts);
	clear_apply_state(&apply_state);
	মুক্ত(apply_argv);

	যদি (res)
		ফেরত res;

	যদি (সূচক_file) {
		/* Reload index as apply_all_patches() will have modified it. */
		discard_সূচক(the_repository->সূচক);
		read_সূচক_from(the_repository->সূচক, সূচক_file,
				repo_get_git_dir(the_repository));
	}

	ফেরত 0;
}

/**
 * Builds an index that contains just the blobs needed for a 3way merge.
 */
স্থির পূর্ণসংখ্যা build_fake_ancestor(ধ্রুবক কাঠামো am_state *state, ধ্রুবক অক্ষর *সূচক_file)
{
	কাঠামো child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	strvec_ঠেলো(&cp.args, "apply");
	strvec_pushv(&cp.args, state->git_apply_opts.v);
	strvec_pushf(&cp.args, "--build-fake-ancestor=%s", সূচক_file);
	strvec_ঠেলো(&cp.args, am_path(state, "patch"));

	যদি (run_command(&cp))
		ফেরত -1;

	ফেরত 0;
}

/**
 * Attempt a threeway merge, using index_path as the temporary index.
 */
স্থির পূর্ণসংখ্যা fall_back_threeway(ধ্রুবক কাঠামো am_state *state, ধ্রুবক অক্ষর *সূচক_path)
{
	কাঠামো object_id their_বৃক্ষ, our_বৃক্ষ;
	কাঠামো object_id bases[1] = { 0 };
	কাঠামো merge_options o;
	কাঠামো commit *ফলাফল;
	অক্ষর *their_বৃক্ষ_name;

	যদি (repo_get_oid(the_repository, "HEAD", &our_বৃক্ষ) < 0)
		oidcpy(&our_বৃক্ষ, the_hash_algo->empty_বৃক্ষ);

	যদি (build_fake_ancestor(state, সূচক_path))
		ফেরত error("could not build fake ancestor");

	discard_সূচক(the_repository->সূচক);
	read_সূচক_from(the_repository->সূচক, সূচক_path, repo_get_git_dir(the_repository));

	যদি (write_সূচক_as_বৃক্ষ(&bases[0], the_repository->সূচক, সূচক_path, 0, ফাঁকা))
		ফেরত error(_("Repository lacks necessary blobs to fall back on 3-way merge."));

	say(state, stdout, _("Using index info to reconstruct a base tree..."));

	যদি (!state->quiet) {
		/*
		 * List paths that needed 3-way fallback, so that the user can
		 * review them with extra care to spot mismerges.
		 */
		কাঠামো rev_info rev_info;

		repo_init_revisions(the_repository, &rev_info, ফাঁকা);
		rev_info.diffopt.output_format = DIFF_FORMAT_NAME_STATUS;
		rev_info.diffopt.filter |= diff_filter_bit('A');
		rev_info.diffopt.filter |= diff_filter_bit('M');
		add_pending_oid(&rev_info, "HEAD", &our_বৃক্ষ, 0);
		diff_setup_done(&rev_info.diffopt);
		run_diff_সূচক(&rev_info, DIFF_সূচক_CACHED);
		release_revisions(&rev_info);
	}

	যদি (run_apply(state, সূচক_path))
		ফেরত error(_("Did you hand edit your patch?\n"
				"It does not apply to blobs recorded in its index."));

	যদি (write_সূচক_as_বৃক্ষ(&their_বৃক্ষ, the_repository->সূচক, সূচক_path, 0, ফাঁকা))
		ফেরত error("could not write tree");

	say(state, stdout, _("Falling back to patching base and 3-way merge..."));

	discard_সূচক(the_repository->সূচক);
	repo_read_সূচক(the_repository);

	/*
	 * This is not so wrong. Depending on which base we picked, orig_tree
	 * may be wildly different from ours, but their_tree has the same set of
	 * wildly different changes in parts the patch did not touch, so
	 * recursive ends up canceling them, saying that we reverted all those
	 * changes.
	 */

	init_ui_merge_options(&o, the_repository);

	o.branch1 = "HEAD";
	their_বৃক্ষ_name = xstrfmt("%.*s", linelen(state->বার্তা), state->বার্তা);
	o.branch2 = their_বৃক্ষ_name;
	o.ancestor = "constructed fake ancestor";
	o.detect_directory_renames = MERGE_DIRECTORY_RENAMES_NONE;

	যদি (state->quiet)
		o.verbosity = 0;

	যদি (merge_ort_generic(&o, &our_বৃক্ষ, &their_বৃক্ষ, 1, bases, &ফলাফল)) {
		repo_rerere(the_repository, state->allow_rerere_autoupdate);
		মুক্ত(their_বৃক্ষ_name);
		ফেরত error(_("Failed to merge in the changes."));
	}

	মুক্ত(their_বৃক্ষ_name);
	ফেরত 0;
}

/**
 * Commits the current index with state->msg as the commit message and
 * state->author_name, state->author_email and state->author_date as the author
 * information.
 */
স্থির শূন্য do_commit(ধ্রুবক কাঠামো am_state *state)
{
	কাঠামো object_id বৃক্ষ, parent, commit;
	ধ্রুবক কাঠামো object_id *old_oid;
	কাঠামো commit_list *parents = ফাঁকা;
	ধ্রুবক অক্ষর *reflog_বার্তা, *author, *committer = ফাঁকা;
	কাঠামো strbuf sb = STRBUF_INIT;

	যদি (!state->no_verify && run_hooks(the_repository, "pre-applypatch"))
		exit(1);

	যদি (write_সূচক_as_বৃক্ষ(&বৃক্ষ, the_repository->সূচক,
				repo_get_সূচক_file(the_repository),
				0, ফাঁকা))
		die(_("git write-tree failed to write a tree"));

	যদি (!repo_get_oid_commit(the_repository, "HEAD", &parent)) {
		old_oid = &parent;
		commit_list_ঢোকাও(lookup_commit(the_repository, &parent),
				   &parents);
	} অন্যথা {
		old_oid = ফাঁকা;
		say(state, stderr, _("applying to an empty history"));
	}

	author = fmt_ident(state->author_name, state->author_email,
		WANT_AUTHOR_IDENT,
			state->ignore_date ? ফাঁকা : state->author_date,
			IDENT_STRICT);

	যদি (state->committer_date_is_author_date)
		committer = fmt_ident(getenv("GIT_COMMITTER_NAME"),
				      getenv("GIT_COMMITTER_EMAIL"),
				      WANT_COMMITTER_IDENT,
				      state->ignore_date ? ফাঁকা
							 : state->author_date,
				      IDENT_STRICT);

	যদি (commit_বৃক্ষ_extended(state->বার্তা, state->বার্তা_len, &বৃক্ষ, parents,
				 &commit, author, committer, state->sign_commit,
				 ফাঁকা))
		die(_("failed to write commit object"));

	reflog_বার্তা = getenv("GIT_REFLOG_ACTION");
	যদি (!reflog_বার্তা)
		reflog_বার্তা = "am";

	strbuf_addf(&sb, "%s: %.*s", reflog_বার্তা, linelen(state->বার্তা),
			state->বার্তা);

	refs_update_ref(get_main_ref_store(the_repository), sb.buf, "HEAD",
			&commit, old_oid, 0,
			UPDATE_REFS_DIE_ON_ERR);

	যদি (state->rebasing) {
		FILE *fp = xfopen(am_path(state, "rewritten"), "a");

		প্রতিজ্ঞা(!is_null_oid(&state->orig_commit));
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
স্থির শূন্য validate_resume_state(ধ্রুবক কাঠামো am_state *state)
{
	যদি (!state->বার্তা)
		die(_("cannot resume: %s does not exist."),
			am_path(state, "final-commit"));

	যদি (!state->author_name || !state->author_email || !state->author_date)
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
স্থির পূর্ণসংখ্যা do_interactive(কাঠামো am_state *state)
{
	প্রতিজ্ঞা(state->বার্তা);

	জন্য (;;) {
		অক্ষর reply[64];

		puts(_("Commit Body is:"));
		puts("--------------------------");
		printf("%s", state->বার্তা);
		puts("--------------------------");

		/*
		 * TRANSLATORS: Make sure to include [y], [n], [e], [v] and [a]
		 * in your translation. The program will only accept English
		 * input at this point.
		 */
		printf(_("Apply? [y]es/[n]o/[e]dit/[v]iew patch/[a]ccept all: "));
		যদি (!fgets(reply, আকার(reply), stdin))
			die("unable to read from stdin; aborting");

		যদি (*reply == 'y' || *reply == 'Y') {
			ফেরত 0;
		} অন্যথা যদি (*reply == 'a' || *reply == 'A') {
			state->interactive = 0;
			ফেরত 0;
		} অন্যথা যদি (*reply == 'n' || *reply == 'N') {
			ফেরত 1;
		} অন্যথা যদি (*reply == 'e' || *reply == 'E') {
			কাঠামো strbuf বার্তা = STRBUF_INIT;

			যদি (!launch_editor(am_path(state, "final-commit"), &বার্তা, ফাঁকা)) {
				মুক্ত(state->বার্তা);
				state->বার্তা = strbuf_detach(&বার্তা, &state->বার্তা_len);
			}
			strbuf_release(&বার্তা);
		} অন্যথা যদি (*reply == 'v' || *reply == 'V') {
			ধ্রুবক অক্ষর *pager = git_pager(the_repository, 1);
			কাঠামো child_process cp = CHILD_PROCESS_INIT;

			যদি (!pager)
				pager = "cat";
			prepare_pager_args(&cp, pager);
			strvec_ঠেলো(&cp.args, am_path(state, "patch"));
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
স্থির শূন্য am_run(কাঠামো am_state *state, পূর্ণসংখ্যা resume)
{
	কাঠামো strbuf sb = STRBUF_INIT;

	unlink(am_path(state, "dirtyindex"));

	যদি (repo_refresh_and_write_সূচক(the_repository, REFRESH_QUIET, 0, 0,
					 ফাঁকা, ফাঁকা, ফাঁকা) < 0)
		die(_("unable to write index file"));

	যদি (repo_সূচক_has_changes(the_repository, ফাঁকা, &sb)) {
		write_state_bool(state, "dirtyindex", 1);
		die(_("Dirty index: cannot apply patches (dirty: %s)"), sb.buf);
	}

	strbuf_release(&sb);

	যখন (state->cur <= state->last) {
		ধ্রুবক অক্ষর *mail = am_path(state, msgnum(state));
		পূর্ণসংখ্যা apply_status;
		পূর্ণসংখ্যা to_keep;

		reset_ident_date();

		যদি (!file_exists(mail))
			যাও পরবর্তী;

		যদি (resume) {
			validate_resume_state(state);
		} অন্যথা {
			পূর্ণসংখ্যা skip;

			যদি (state->rebasing)
				skip = parse_mail_rebase(state, mail);
			অন্যথা
				skip = parse_mail(state, mail);

			যদি (skip)
				যাও পরবর্তী; /* mail should be skipped */

			যদি (state->signoff)
				am_append_signoff(state);

			write_author_script(state);
			write_commit_বার্তা(state);
		}

		যদি (state->interactive && do_interactive(state))
			যাও পরবর্তী;

		to_keep = 0;
		যদি (is_empty_or_missing_file(am_path(state, "patch"))) {
			নির্বাচন (state->empty_type) {
			ক্ষেত্র DROP_EMPTY_COMMIT:
				say(state, stdout, _("Skipping: %.*s"), linelen(state->বার্তা), state->বার্তা);
				যাও পরবর্তী;
				বিরতি;
			ক্ষেত্র KEEP_EMPTY_COMMIT:
				to_keep = 1;
				say(state, stdout, _("Creating an empty commit: %.*s"),
					linelen(state->বার্তা), state->বার্তা);
				বিরতি;
			ক্ষেত্র STOP_ON_EMPTY_COMMIT:
				printf_ln(_("Patch is empty."));
				die_user_resolve(state);
				বিরতি;
			}
		}

		যদি (run_applypatch_বার্তা_hook(state))
			exit(1);
		যদি (to_keep)
			যাও commit;

		say(state, stdout, _("Applying: %.*s"), linelen(state->বার্তা), state->বার্তা);

		apply_status = run_apply(state, ফাঁকা);

		যদি (apply_status && state->threeway) {
			কাঠামো strbuf sb = STRBUF_INIT;

			strbuf_addstr(&sb, am_path(state, "patch-merge-index"));
			apply_status = fall_back_threeway(state, sb.buf);
			strbuf_release(&sb);

			/*
			 * Applying the patch to an earlier tree and merging
			 * the result may have produced the same tree as ours.
			 */
			যদি (!apply_status &&
			    !repo_সূচক_has_changes(the_repository, ফাঁকা, ফাঁকা)) {
				say(state, stdout, _("No changes -- Patch already applied."));
				যাও পরবর্তী;
			}
		}

		যদি (apply_status) {
			printf_ln(_("Patch failed at %s %.*s"), msgnum(state),
				linelen(state->বার্তা), state->বার্তা);

			যদি (advice_enabled(ADVICE_AM_WORK_DIR))
				advise(_("Use 'git am --show-current-patch=diff' to see the failed patch"));

			die_user_resolve(state);
		}

commit:
		do_commit(state);

পরবর্তী:
		am_পরবর্তী(state);

		যদি (resume)
			am_load(state);
		resume = 0;
	}

	যদি (!is_empty_or_missing_file(am_path(state, "rewritten"))) {
		প্রতিজ্ঞা(state->rebasing);
		copy_notes_for_rebase(state);
		run_post_rewrite_hook(state);
	}

	/*
	 * In rebasing mode, it's up to the caller to take care of
	 * housekeeping.
	 */
	যদি (!state->rebasing) {
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
স্থির শূন্য am_resolve(কাঠামো am_state *state, পূর্ণসংখ্যা allow_empty)
{
	validate_resume_state(state);

	say(state, stdout, _("Applying: %.*s"), linelen(state->বার্তা), state->বার্তা);

	যদি (!repo_সূচক_has_changes(the_repository, ফাঁকা, ফাঁকা)) {
		যদি (allow_empty && is_empty_or_missing_file(am_path(state, "patch"))) {
			printf_ln(_("No changes - recorded it as an empty commit."));
		} অন্যথা {
			printf_ln(_("No changes - did you forget to use 'git add'?\n"
				    "If there is nothing left to stage, chances are that something else\n"
				    "already introduced the same changes; you might want to skip this patch."));
			die_user_resolve(state);
		}
	}

	যদি (unmerged_সূচক(the_repository->সূচক)) {
		printf_ln(_("You still have unmerged paths in your index.\n"
			"You should 'git add' each file with resolved conflicts to mark them as such.\n"
			"You might run `git rm` on a file to accept \"deleted by them\" for it."));
		die_user_resolve(state);
	}

	যদি (state->interactive) {
		write_সূচক_patch(state);
		যদি (do_interactive(state))
			যাও পরবর্তী;
	}

	repo_rerere(the_repository, 0);

	do_commit(state);

পরবর্তী:
	am_পরবর্তী(state);
	am_load(state);
	am_run(state, 0);
}

/**
 * Performs a checkout fast-forward from `head` to `remote`. If `reset` is
 * true, any unmerged entries will be discarded. Returns 0 on success, -1 on
 * failure.
 */
স্থির পূর্ণসংখ্যা fast_forward_to(কাঠামো বৃক্ষ *শীর্ষ, কাঠামো বৃক্ষ *remote, পূর্ণসংখ্যা reset)
{
	কাঠামো lock_file lock_file = LOCK_INIT;
	কাঠামো unpack_trees_options opts;
	কাঠামো বৃক্ষ_desc t[2];

	যদি (repo_parse_বৃক্ষ(the_repository, শীর্ষ) || repo_parse_বৃক্ষ(the_repository, remote))
		ফেরত -1;

	repo_hold_locked_সূচক(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	refresh_সূচক(the_repository->সূচক, REFRESH_QUIET, ফাঁকা, ফাঁকা, ফাঁকা);

	memset(&opts, 0, আকার(opts));
	opts.শীর্ষ_idx = 1;
	opts.src_সূচক = the_repository->সূচক;
	opts.dst_সূচক = the_repository->সূচক;
	opts.update = 1;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = twoway_merge;
	init_বৃক্ষ_desc(&t[0], &শীর্ষ->object.oid, শীর্ষ->বাফার, শীর্ষ->আকার_2);
	init_বৃক্ষ_desc(&t[1], &remote->object.oid, remote->বাফার, remote->আকার_2);

	যদি (unpack_trees(2, t, &opts)) {
		rollback_lock_file(&lock_file);
		ফেরত -1;
	}

	যদি (write_locked_সূচক(the_repository->সূচক, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	ফেরত 0;
}

/**
 * Merges a tree into the index. The index's stat info will take precedence
 * over the merged tree's. Returns 0 on success, -1 on failure.
 */
স্থির পূর্ণসংখ্যা merge_বৃক্ষ(কাঠামো বৃক্ষ *বৃক্ষ)
{
	কাঠামো lock_file lock_file = LOCK_INIT;
	কাঠামো unpack_trees_options opts;
	কাঠামো বৃক্ষ_desc t[1];

	যদি (repo_parse_বৃক্ষ(the_repository, বৃক্ষ))
		ফেরত -1;

	repo_hold_locked_সূচক(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, আকার(opts));
	opts.শীর্ষ_idx = 1;
	opts.src_সূচক = the_repository->সূচক;
	opts.dst_সূচক = the_repository->সূচক;
	opts.merge = 1;
	opts.fn = oneway_merge;
	init_বৃক্ষ_desc(&t[0], &বৃক্ষ->object.oid, বৃক্ষ->বাফার, বৃক্ষ->আকার_2);

	যদি (unpack_trees(1, t, &opts)) {
		rollback_lock_file(&lock_file);
		ফেরত -1;
	}

	যদি (write_locked_সূচক(the_repository->সূচক, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	ফেরত 0;
}

/**
 * Clean the index without touching entries that are not modified between
 * `head` and `remote`.
 */
স্থির পূর্ণসংখ্যা clean_সূচক(ধ্রুবক কাঠামো object_id *শীর্ষ, ধ্রুবক কাঠামো object_id *remote)
{
	কাঠামো বৃক্ষ *শীর্ষ_বৃক্ষ, *remote_বৃক্ষ, *সূচক_বৃক্ষ;
	কাঠামো object_id সূচক;

	শীর্ষ_বৃক্ষ = repo_parse_বৃক্ষ_indirect(the_repository, শীর্ষ);
	যদি (!শীর্ষ_বৃক্ষ)
		ফেরত error(_("Could not parse object '%s'."), oid_to_hex(শীর্ষ));

	remote_বৃক্ষ = repo_parse_বৃক্ষ_indirect(the_repository, remote);
	যদি (!remote_বৃক্ষ)
		ফেরত error(_("Could not parse object '%s'."), oid_to_hex(remote));

	repo_read_সূচক_unmerged(the_repository);

	যদি (fast_forward_to(শীর্ষ_বৃক্ষ, শীর্ষ_বৃক্ষ, 1))
		ফেরত -1;

	যদি (write_সূচক_as_বৃক্ষ(&সূচক, the_repository->সূচক,
				repo_get_সূচক_file(the_repository),
				0, ফাঁকা))
		ফেরত -1;

	সূচক_বৃক্ষ = repo_parse_বৃক্ষ_indirect(the_repository, &সূচক);
	যদি (!সূচক_বৃক্ষ)
		ফেরত error(_("Could not parse object '%s'."), oid_to_hex(&সূচক));

	যদি (fast_forward_to(সূচক_বৃক্ষ, remote_বৃক্ষ, 0))
		ফেরত -1;

	যদি (merge_বৃক্ষ(remote_বৃক্ষ))
		ফেরত -1;

	remove_branch_state(the_repository, 0);

	ফেরত 0;
}

/**
 * Resets rerere's merge resolution metadata.
 */
স্থির শূন্য am_rerere_clear(শূন্য)
{
	কাঠামো string_list merge_rr = STRING_LIST_INIT_DUP;
	rerere_clear(the_repository, &merge_rr);
	string_list_clear(&merge_rr, 1);
}

/**
 * Resume the current am session by skipping the current patch.
 */
স্থির শূন্য am_skip(কাঠামো am_state *state)
{
	কাঠামো object_id শীর্ষ;

	am_rerere_clear();

	যদি (repo_get_oid(the_repository, "HEAD", &শীর্ষ))
		oidcpy(&শীর্ষ, the_hash_algo->empty_বৃক্ষ);

	যদি (clean_সূচক(&শীর্ষ, &শীর্ষ))
		die(_("failed to clean index"));

	যদি (state->rebasing) {
		FILE *fp = xfopen(am_path(state, "rewritten"), "a");

		প্রতিজ্ঞা(!is_null_oid(&state->orig_commit));
		fprintf(fp, "%s ", oid_to_hex(&state->orig_commit));
		fprintf(fp, "%s\n", oid_to_hex(&শীর্ষ));
		fclose(fp);
	}

	am_পরবর্তী(state);
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
স্থির পূর্ণসংখ্যা safe_to_abort(ধ্রুবক কাঠামো am_state *state)
{
	কাঠামো strbuf sb = STRBUF_INIT;
	কাঠামো object_id abort_safety, শীর্ষ;

	যদি (file_exists(am_path(state, "dirtyindex")))
		ফেরত 0;

	যদি (read_state_file(&sb, state, "abort-safety", 1) > 0) {
		যদি (get_oid_hex(sb.buf, &abort_safety))
			die(_("could not parse %s"), am_path(state, "abort-safety"));
	} অন্যথা
		oidclr(&abort_safety, the_repository->hash_algo);
	strbuf_release(&sb);

	যদি (repo_get_oid(the_repository, "HEAD", &শীর্ষ))
		oidclr(&শীর্ষ, the_repository->hash_algo);

	যদি (oideq(&শীর্ষ, &abort_safety))
		ফেরত 1;

	warning(_("You seem to have moved HEAD since the last 'am' failure.\n"
		"Not rewinding to ORIG_HEAD"));

	ফেরত 0;
}

/**
 * Aborts the current am session if it is safe to do so.
 */
স্থির শূন্য am_abort(কাঠামো am_state *state)
{
	কাঠামো object_id curr_শীর্ষ, orig_শীর্ষ;
	পূর্ণসংখ্যা has_curr_শীর্ষ, has_orig_শীর্ষ;
	অক্ষর *curr_branch;

	যদি (!safe_to_abort(state)) {
		am_destroy(state);
		ফেরত;
	}

	am_rerere_clear();

	curr_branch = refs_resolve_refdup(get_main_ref_store(the_repository),
					  "HEAD", 0, &curr_শীর্ষ, ফাঁকা);
	has_curr_শীর্ষ = curr_branch && !is_null_oid(&curr_শীর্ষ);
	যদি (!has_curr_শীর্ষ)
		oidcpy(&curr_শীর্ষ, the_hash_algo->empty_বৃক্ষ);

	has_orig_শীর্ষ = !repo_get_oid(the_repository, "ORIG_HEAD", &orig_শীর্ষ);
	যদি (!has_orig_শীর্ষ)
		oidcpy(&orig_শীর্ষ, the_hash_algo->empty_বৃক্ষ);

	যদি (clean_সূচক(&curr_শীর্ষ, &orig_শীর্ষ))
		die(_("failed to clean index"));

	যদি (has_orig_শীর্ষ)
		refs_update_ref(get_main_ref_store(the_repository),
				"am --abort", "HEAD", &orig_শীর্ষ,
				has_curr_শীর্ষ ? &curr_শীর্ষ : ফাঁকা, 0,
				UPDATE_REFS_DIE_ON_ERR);
	অন্যথা যদি (curr_branch)
		refs_delete_ref(get_main_ref_store(the_repository), ফাঁকা,
				curr_branch, ফাঁকা, REF_NO_DEREF);

	মুক্ত(curr_branch);
	am_destroy(state);
}

স্থির পূর্ণসংখ্যা show_patch(কাঠামো am_state *state, গণনা resume_type resume_mode)
{
	কাঠামো strbuf sb = STRBUF_INIT;
	ধ্রুবক অক্ষর *patch_path;
	পূর্ণসংখ্যা len;

	যদি (!is_null_oid(&state->orig_commit)) {
		কাঠামো child_process cmd = CHILD_PROCESS_INIT;

		strvec_pushl(&cmd.args, "show", oid_to_hex(&state->orig_commit),
			     "--", ফাঁকা);
		cmd.git_cmd = 1;
		ফেরত run_command(&cmd);
	}

	নির্বাচন (resume_mode) {
	ক্ষেত্র RESUME_SHOW_PATCH_RAW:
		patch_path = am_path(state, msgnum(state));
		বিরতি;
	ক্ষেত্র RESUME_SHOW_PATCH_DIFF:
		patch_path = am_path(state, "patch");
		বিরতি;
	পূর্বনির্ধারিত:
		BUG("invalid mode for --show-current-patch");
	}

	len = strbuf_read_file(&sb, patch_path, 0);
	যদি (len < 0)
		die_errno(_("failed to read '%s'"), patch_path);

	setup_pager(the_repository);
	write_in_full(1, sb.buf, sb.len);
	strbuf_release(&sb);
	ফেরত 0;
}

/**
 * parse_options() callback that validates and sets opt->value to the
 * PATCH_FORMAT_* enum value corresponding to `arg`.
 */
স্থির পূর্ণসংখ্যা parse_opt_patchformat(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	পূর্ণসংখ্যা *opt_মান = opt->মান;

	যদি (unset)
		*opt_মান = PATCH_FORMAT_UNKNOWN;
	অন্যথা যদি (!strcmp(arg, "mbox"))
		*opt_মান = PATCH_FORMAT_MBOX;
	অন্যথা যদি (!strcmp(arg, "stgit"))
		*opt_মান = PATCH_FORMAT_STGIT;
	অন্যথা যদি (!strcmp(arg, "stgit-series"))
		*opt_মান = PATCH_FORMAT_STGIT_SERIES;
	অন্যথা যদি (!strcmp(arg, "hg"))
		*opt_মান = PATCH_FORMAT_HG;
	অন্যথা যদি (!strcmp(arg, "mboxrd"))
		*opt_মান = PATCH_FORMAT_MBOXRD;
	/*
	 * Please update $__git_patchformat in git-completion.bash
	 * when you add new options
	 */
	অন্যথা
		ফেরত error(_("invalid value for '%s': '%s'"),
			     "--patch-format", arg);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা parse_opt_show_current_patch(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	পূর্ণসংখ্যা *opt_মান = opt->মান;

	BUG_ON_OPT_NEG(unset);

	যদি (!arg)
		*opt_মান = opt->defval;
	অন্যথা যদি (!strcmp(arg, "raw"))
		*opt_মান = RESUME_SHOW_PATCH_RAW;
	অন্যথা যদি (!strcmp(arg, "diff"))
		*opt_মান = RESUME_SHOW_PATCH_DIFF;
	/*
	 * Please update $__git_showcurrentpatch in git-completion.bash
	 * when you add new options
	 */
	অন্যথা
		ফেরত error(_("invalid value for '%s': '%s'"),
			     "--show-current-patch", arg);
	ফেরত 0;
}

পূর্ণসংখ্যা cmd_am(পূর্ণসংখ্যা argc,
	   ধ্রুবক অক্ষর **argv,
	   ধ্রুবক অক্ষর *prefix,
	   কাঠামো repository *repo UNUSED)
{
	কাঠামো am_state state;
	পূর্ণসংখ্যা দ্বিমিক = -1;
	পূর্ণসংখ্যা keep_cr = -1;
	পূর্ণসংখ্যা patch_format = PATCH_FORMAT_UNKNOWN;
	গণনা resume_type resume_mode = RESUME_FALSE;
	পূর্ণসংখ্যা in_progress;
	পূর্ণসংখ্যা ret = 0;

	ধ্রুবক অক্ষর * ধ্রুবক usage[] = {
		N_("git am [<options>] [(<mbox> | <Maildir>)...]"),
		N_("git am [<options>] (--continue | --skip | --abort)"),
		ফাঁকা
	};

	কাঠামো option options[] = {
		OPT_BOOL('i', "interactive", &state.interactive,
			N_("run interactively")),
		OPT_BOOL('n', "no-verify", &state.no_verify,
			N_("bypass pre-applypatch and applypatch-msg hooks")),
		OPT_HIDDEN_BOOL('b', "binary", &দ্বিমিক,
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
		OPT_PASSTHRU_ARGV(0, "ignore-space-change", &state.git_apply_opts, ফাঁকা,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU_ARGV(0, "ignore-whitespace", &state.git_apply_opts, ফাঁকা,
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
		OPT_PASSTHRU_ARGV('C', ফাঁকা, &state.git_apply_opts, N_("n"),
			N_("pass it through git-apply"),
			0),
		OPT_PASSTHRU_ARGV('p', ফাঁকা, &state.git_apply_opts, N_("num"),
			N_("pass it through git-apply"),
			0),
		OPT_CALLBACK(0, "patch-format", &patch_format, N_("format"),
			N_("format the patch(es) are in"),
			parse_opt_patchformat),
		OPT_PASSTHRU_ARGV(0, "reject", &state.git_apply_opts, ফাঁকা,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_STRING(0, "resolvemsg", &state.resolvemsg, ফাঁকা,
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
			.মান = &resume_mode,
			.precision = আকার(resume_mode),
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
			.মান = &state.sign_commit,
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

	repo_config(the_repository, git_default_config, ফাঁকা);

	am_state_init(&state);

	in_progress = am_in_progress(&state);
	যদি (in_progress)
		am_load(&state);

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	যদি (দ্বিমিক >= 0)
		fprintf_ln(stderr, _("The -b/--binary option has been a no-op for long time, and\n"
				"it will be removed. Please do not use it anymore."));

	/* Ensure a valid committer ident can be constructed */
	git_committer_info(IDENT_STRICT);

	যদি (repo_read_সূচক_preload(the_repository, ফাঁকা, 0) < 0)
		die(_("failed to read the index"));

	যদি (in_progress) {
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
		যদি (argc || (resume_mode == RESUME_FALSE && !isatty(0)))
			die(_("previous rebase directory %s still exists but mbox given."),
				state.dir);

		যদি (resume_mode == RESUME_FALSE)
			resume_mode = RESUME_APPLY;

		যদি (state.signoff == SIGNOFF_EXPLICIT)
			am_append_signoff(&state);
	} অন্যথা {
		কাঠামো strvec paths = STRVEC_INIT;
		পূর্ণসংখ্যা i;

		/*
		 * Handle stray state directory in the independent-run case. In
		 * the --rebasing case, it is up to the caller to take care of
		 * stray directories.
		 */
		যদি (file_exists(state.dir) && !state.rebasing) {
			যদি (resume_mode == RESUME_ABORT || resume_mode == RESUME_QUIT) {
				am_destroy(&state);
				am_state_release(&state);
				ফেরত 0;
			}

			die(_("Stray %s directory found.\n"
				"Use \"git am --abort\" to remove it."),
				state.dir);
		}

		যদি (resume_mode)
			die(_("Resolve operation not in progress, we are not resuming."));

		জন্য (i = 0; i < argc; i++) {
			যদি (is_absolute_path(argv[i]) || !prefix)
				strvec_ঠেলো(&paths, argv[i]);
			অন্যথা
				strvec_ঠেলো(&paths, mkpath("%s/%s", prefix, argv[i]));
		}

		যদি (state.interactive && !paths.nr)
			die(_("interactive mode requires patches on the command line"));

		am_setup(&state, patch_format, paths.v, keep_cr);

		strvec_clear(&paths);
	}

	নির্বাচন (resume_mode) {
	ক্ষেত্র RESUME_FALSE:
		am_run(&state, 0);
		বিরতি;
	ক্ষেত্র RESUME_APPLY:
		am_run(&state, 1);
		বিরতি;
	ক্ষেত্র RESUME_RESOLVED:
	ক্ষেত্র RESUME_ALLOW_EMPTY:
		am_resolve(&state, resume_mode == RESUME_ALLOW_EMPTY ? 1 : 0);
		বিরতি;
	ক্ষেত্র RESUME_SKIP:
		am_skip(&state);
		বিরতি;
	ক্ষেত্র RESUME_ABORT:
		am_abort(&state);
		বিরতি;
	ক্ষেত্র RESUME_QUIT:
		am_rerere_clear();
		am_destroy(&state);
		বিরতি;
	ক্ষেত্র RESUME_SHOW_PATCH_RAW:
	ক্ষেত্র RESUME_SHOW_PATCH_DIFF:
		ret = show_patch(&state, resume_mode);
		বিরতি;
	পূর্বনির্ধারিত:
		BUG("invalid resume value");
	}

	am_state_release(&state);

	ফেরত ret;
}
