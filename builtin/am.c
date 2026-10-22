/*
 * Builtin "git am"
 *
 * Based on git-am.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "abspath.h"
#شامل "advice.h"
#شامل "config.h"
#شامل "editor.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "parse-options.h"
#شامل "dir.h"
#شامل "run-command.h"
#شامل "hook.h"
#شامل "quote.h"
#شامل "tempfile.h"
#شامل "lockfile.h"
#شامل "cache-tree.h"
#شامل "refs.h"
#شامل "commit.h"
#شامل "diff.h"
#شامل "unpack-trees.h"
#شامل "branch.h"
#شامل "object-name.h"
#شامل "preload-index.h"
#شامل "sequencer.h"
#شامل "revision.h"
#شامل "merge-ort-wrappers.h"
#شامل "log-tree.h"
#شامل "notes-utils.h"
#شامل "rerere.h"
#شامل "mailinfo.h"
#شامل "apply.h"
#شامل "string-list.h"
#شامل "pager.h"
#شامل "path.h"
#شامل "pretty.h"

/**
 * Returns the length of the first line of msg.
 */
ایستا صحیح linelen(ثابت نویسه *پیام)
{
	بازگشت strchrnul(پیام, '\n') - پیام;
}

/**
 * Returns true if `str` consists of only whitespace, false otherwise.
 */
ایستا صحیح str_isspace(ثابت نویسه *str)
{
	برای (; *str; str++)
		اگر (!isspace(*str))
			بازگشت 0;

	بازگشت 1;
}

شمارش patch_format {
	PATCH_FORMAT_UNKNOWN = 0,
	PATCH_FORMAT_MBOX,
	PATCH_FORMAT_STGIT,
	PATCH_FORMAT_STGIT_SERIES,
	PATCH_FORMAT_HG,
	PATCH_FORMAT_MBOXRD
};

شمارش keep_type {
	KEEP_FALSE = 0,
	KEEP_TRUE,      /* pass -k flag to git-mailinfo */
	KEEP_NON_PATCH  /* pass -b flag to git-mailinfo */
};

شمارش scissors_type {
	SCISSORS_UNSET = -1,
	SCISSORS_FALSE = 0,  /* pass --no-scissors to git-mailinfo */
	SCISSORS_TRUE        /* pass --scissors to git-mailinfo */
};

شمارش signoff_type {
	SIGNOFF_FALSE = 0,
	SIGNOFF_TRUE = 1,
	SIGNOFF_EXPLICIT /* --signoff was set on the command-line */
};

شمارش resume_type {
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

شمارش empty_action {
	STOP_ON_EMPTY_COMMIT = 0,  /* output errors and stop in the middle of an am session */
	DROP_EMPTY_COMMIT,         /* skip with a notice message, unless "--quiet" has been passed */
	KEEP_EMPTY_COMMIT,         /* keep recording as empty commits */
};

ساختار am_state {
	/* state directory path */
	نویسه *dir;

	/* current and last patch numbers, 1-indexed */
	صحیح cur;
	صحیح last;

	/* commit metadata and message */
	نویسه *author_name;
	نویسه *author_email;
	نویسه *author_date;
	نویسه *پیام;
	size_t پیام_len;

	/* when --rebasing, records the original commit the patch came from */
	ساختار object_id orig_commit;

	/* number of digits in patch filename */
	صحیح prec;

	/* various operating modes and command line options */
	صحیح interactive;
	صحیح no_verify;
	صحیح threeway;
	صحیح quiet;
	صحیح signoff; /* enum signoff_type */
	صحیح utf8;
	صحیح keep; /* enum keep_type */
	صحیح message_id;
	صحیح scissors; /* enum scissors_type */
	صحیح quoted_cr; /* enum quoted_cr_action */
	صحیح empty_type; /* enum empty_action */
	ساختار strvec git_apply_opts;
	ثابت نویسه *resolvemsg;
	صحیح committer_date_is_author_date;
	صحیح ignore_date;
	صحیح allow_rerere_autoupdate;
	ثابت نویسه *sign_commit;
	صحیح rebasing;
};

/**
 * Initializes am_state with the default values.
 */
ایستا تهی am_state_init(ساختار am_state *state)
{
	صحیح gpgsign;

	memset(state, 0, اندازه(*state));

	state->dir = repo_git_path(the_repository, "rebase-apply");

	state->prec = 4;

	repo_config_get_bool(the_repository, "am.threeway", &state->threeway);

	state->utf8 = 1;

	repo_config_get_bool(the_repository, "am.messageid", &state->message_id);

	state->scissors = SCISSORS_UNSET;
	state->quoted_cr = quoted_cr_unset;

	strvec_init(&state->git_apply_opts);

	اگر (!repo_config_get_bool(the_repository, "commit.gpgsign", &gpgsign))
		state->sign_commit = gpgsign ? "" : پوچ;
}

/**
 * Releases memory allocated by an am_state.
 */
ایستا تهی am_state_release(ساختار am_state *state)
{
	آزاد(state->dir);
	آزاد(state->author_name);
	آزاد(state->author_email);
	آزاد(state->author_date);
	آزاد(state->پیام);
	strvec_clear(&state->git_apply_opts);
}

ایستا صحیح am_option_parse_quoted_cr(ثابت ساختار option *opt,
				     ثابت نویسه *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);

	اگر (mailinfo_parse_quoted_cr_action(arg, opt->مقدار) != 0)
		بازگشت error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	بازگشت 0;
}

ایستا صحیح am_option_parse_empty(ثابت ساختار option *opt,
				     ثابت نویسه *arg, صحیح unset)
{
	صحیح *opt_مقدار = opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	اگر (!strcmp(arg, "stop"))
		*opt_مقدار = STOP_ON_EMPTY_COMMIT;
	وگرنه اگر (!strcmp(arg, "drop"))
		*opt_مقدار = DROP_EMPTY_COMMIT;
	وگرنه اگر (!strcmp(arg, "keep"))
		*opt_مقدار = KEEP_EMPTY_COMMIT;
	وگرنه
		بازگشت error(_("invalid value for '%s': '%s'"), "--empty", arg);

	بازگشت 0;
}

/**
 * Returns path relative to the am_state directory.
 */
ایستا درونخطی ثابت نویسه *am_path(ثابت ساختار am_state *state, ثابت نویسه *path)
{
	بازگشت mkpath("%s/%s", state->dir, path);
}

/**
 * For convenience to call write_file()
 */
ایستا تهی write_state_text(ثابت ساختار am_state *state,
			     ثابت نویسه *name, ثابت نویسه *string)
{
	write_file(am_path(state, name), "%s", string);
}

ایستا تهی write_state_شمار(ثابت ساختار am_state *state,
			      ثابت نویسه *name, صحیح مقدار)
{
	write_file(am_path(state, name), "%d", مقدار);
}

ایستا تهی write_state_bool(ثابت ساختار am_state *state,
			     ثابت نویسه *name, صحیح مقدار)
{
	write_state_text(state, name, مقدار ? "t" : "f");
}

/**
 * If state->quiet is false, calls fprintf(fp, fmt, ...), and appends a newline
 * at the end.
 */
__attribute__((format (printf, 3, 4)))
ایستا تهی say(ثابت ساختار am_state *state, FILE *fp, ثابت نویسه *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	اگر (!state->quiet) {
		vfprintf(fp, fmt, ap);
		putc('\n', fp);
	}
	va_end(ap);
}

/**
 * Returns 1 if there is an am session in progress, 0 otherwise.
 */
ایستا صحیح am_in_progress(ثابت ساختار am_state *state)
{
	ساختار stat st;

	اگر (lstat(state->dir, &st) < 0 || !S_ISDIR(st.st_mode))
		بازگشت 0;
	اگر (lstat(am_path(state, "last"), &st) || !S_ISREG(st.st_mode))
		بازگشت 0;
	اگر (lstat(am_path(state, "next"), &st) || !S_ISREG(st.st_mode))
		بازگشت 0;
	بازگشت 1;
}

/**
 * Reads the contents of `file` in the `state` directory into `sb`. Returns the
 * number of bytes read on success, -1 if the file does not exist. If `trim` is
 * set, trailing whitespace will be removed.
 */
ایستا صحیح read_state_file(ساختار strbuf *sb, ثابت ساختار am_state *state,
			ثابت نویسه *file, صحیح trim)
{
	strbuf_reset(sb);

	اگر (strbuf_read_file(sb, am_path(state, file), 0) >= 0) {
		اگر (trim)
			strbuf_trim(sb);

		بازگشت sb->len;
	}

	اگر (errno == ENOENT)
		بازگشت -1;

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
ایستا صحیح read_am_author_script(ساختار am_state *state)
{
	ثابت نویسه *filename = am_path(state, "author-script");

	أكد(!state->author_name);
	أكد(!state->author_email);
	أكد(!state->author_date);

	بازگشت read_author_script(filename, &state->author_name,
				  &state->author_email, &state->author_date, 1);
}

/**
 * Saves state->author_name, state->author_email and state->author_date in the
 * state directory's "author-script" file.
 */
ایستا تهی write_author_script(ثابت ساختار am_state *state)
{
	ساختار strbuf sb = STRBUF_INIT;

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
ایستا صحیح read_commit_پیام(ساختار am_state *state)
{
	ساختار strbuf sb = STRBUF_INIT;

	أكد(!state->پیام);

	اگر (read_state_file(&sb, state, "final-commit", 0) < 0) {
		strbuf_release(&sb);
		بازگشت -1;
	}

	state->پیام = strbuf_detach(&sb, &state->پیام_len);
	بازگشت 0;
}

/**
 * Saves state->msg in the state directory's "final-commit" file.
 */
ایستا تهی write_commit_پیام(ثابت ساختار am_state *state)
{
	ثابت نویسه *filename = am_path(state, "final-commit");
	write_file_buf(filename, state->پیام, state->پیام_len);
}

/**
 * Loads state from disk.
 */
ایستا تهی am_load(ساختار am_state *state)
{
	ساختار strbuf sb = STRBUF_INIT;

	اگر (read_state_file(&sb, state, "next", 1) < 0)
		BUG("state file 'next' does not exist");
	state->cur = strtol(sb.buf, پوچ, 10);

	اگر (read_state_file(&sb, state, "last", 1) < 0)
		BUG("state file 'last' does not exist");
	state->last = strtol(sb.buf, پوچ, 10);

	اگر (read_am_author_script(state) < 0)
		die(_("could not parse author script"));

	read_commit_پیام(state);

	اگر (read_state_file(&sb, state, "original-commit", 1) < 0)
		oidclr(&state->orig_commit, the_repository->hash_algo);
	وگرنه اگر (get_oid_hex(sb.buf, &state->orig_commit) < 0)
		die(_("could not parse %s"), am_path(state, "original-commit"));

	read_state_file(&sb, state, "threeway", 1);
	state->threeway = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "quiet", 1);
	state->quiet = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "sign", 1);
	state->signoff = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "utf8", 1);
	state->utf8 = !strcmp(sb.buf, "t");

	اگر (file_exists(am_path(state, "rerere-autoupdate"))) {
		read_state_file(&sb, state, "rerere-autoupdate", 1);
		state->allow_rerere_autoupdate = strcmp(sb.buf, "t") ?
			RERERE_NOAUTOUPDATE : RERERE_AUTOUPDATE;
	} وگرنه {
		state->allow_rerere_autoupdate = 0;
	}

	read_state_file(&sb, state, "keep", 1);
	اگر (!strcmp(sb.buf, "t"))
		state->keep = KEEP_TRUE;
	وگرنه اگر (!strcmp(sb.buf, "b"))
		state->keep = KEEP_NON_PATCH;
	وگرنه
		state->keep = KEEP_FALSE;

	read_state_file(&sb, state, "messageid", 1);
	state->message_id = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "scissors", 1);
	اگر (!strcmp(sb.buf, "t"))
		state->scissors = SCISSORS_TRUE;
	وگرنه اگر (!strcmp(sb.buf, "f"))
		state->scissors = SCISSORS_FALSE;
	وگرنه
		state->scissors = SCISSORS_UNSET;

	read_state_file(&sb, state, "quoted-cr", 1);
	اگر (!*sb.buf)
		state->quoted_cr = quoted_cr_unset;
	وگرنه اگر (mailinfo_parse_quoted_cr_action(sb.buf, &state->quoted_cr) != 0)
		die(_("could not parse %s"), am_path(state, "quoted-cr"));

	read_state_file(&sb, state, "apply-opt", 1);
	strvec_clear(&state->git_apply_opts);
	اگر (sq_dequote_to_strvec(sb.buf, &state->git_apply_opts) < 0)
		die(_("could not parse %s"), am_path(state, "apply-opt"));

	state->rebasing = !!file_exists(am_path(state, "rebasing"));

	strbuf_release(&sb);
}

/**
 * Removes the am_state directory, forcefully terminating the current am
 * session.
 */
ایستا تهی am_destroy(ثابت ساختار am_state *state)
{
	ساختار strbuf sb = STRBUF_INIT;

	strbuf_addstr(&sb, state->dir);
	remove_dir_recursively(&sb, 0);
	strbuf_release(&sb);
}

/**
 * Runs applypatch-msg hook. Returns its exit code.
 */
ایستا صحیح run_applypatch_پیام_hook(ساختار am_state *state)
{
	صحیح ret = 0;

	أكد(state->پیام);

	اگر (!state->no_verify) {
		ساختار run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;
		strvec_افزودن(&opt.args, am_path(state, "final-commit"));
		ret = run_hooks_opt(the_repository, "applypatch-msg", &opt);
	}

	اگر (!ret) {
		FREE_AND_NULL(state->پیام);
		اگر (read_commit_پیام(state) < 0)
			die(_("'%s' was deleted by the applypatch-msg hook"),
				am_path(state, "final-commit"));
	}

	بازگشت ret;
}

/**
 * Runs post-rewrite hook. Returns it exit code.
 */
ایستا صحیح run_post_rewrite_hook(ثابت ساختار am_state *state)
{
	ساختار run_hooks_opt opt = RUN_HOOKS_OPT_INIT;

	strvec_افزودن(&opt.args, "rebase");
	opt.path_to_stdin = am_path(state, "rewritten");

	بازگشت run_hooks_opt(the_repository, "post-rewrite", &opt);
}

/**
 * Reads the state directory's "rewritten" file, and copies notes from the old
 * commits listed in the file to their rewritten commits.
 *
 * Returns 0 on success, -1 on failure.
 */
ایستا صحیح copy_notes_for_rebase(ثابت ساختار am_state *state)
{
	ساختار notes_rewrite_cfg *c;
	ساختار strbuf sb = STRBUF_INIT;
	ثابت نویسه *invalid_سطر = _("Malformed input line: '%s'.");
	ثابت نویسه *پیام = "Notes added by 'git rebase'";
	FILE *fp;
	صحیح ret = 0;

	أكد(state->rebasing);

	c = init_copy_notes_for_rewrite("rebase");
	اگر (!c)
		بازگشت 0;

	fp = xfopen(am_path(state, "rewritten"), "r");

	وقتی (!strbuf_getline_lf(&sb, fp)) {
		ساختار object_id from_obj, to_obj;
		ثابت نویسه *p;

		اگر (sb.len != the_hash_algo->hexsz * 2 + 1) {
			ret = error(invalid_سطر, sb.buf);
			برو finish;
		}

		اگر (parse_oid_hex(sb.buf, &from_obj, &p)) {
			ret = error(invalid_سطر, sb.buf);
			برو finish;
		}

		اگر (*p != ' ') {
			ret = error(invalid_سطر, sb.buf);
			برو finish;
		}

		اگر (get_oid_hex(p + 1, &to_obj)) {
			ret = error(invalid_سطر, sb.buf);
			برو finish;
		}

		اگر (copy_note_for_rewrite(c, &from_obj, &to_obj))
			ret = error(_("Failed to copy notes from '%s' to '%s'"),
					oid_to_hex(&from_obj), oid_to_hex(&to_obj));
	}

finish:
	finish_copy_notes_for_rewrite(the_repository, c, پیام);
	fclose(fp);
	strbuf_release(&sb);
	بازگشت ret;
}

/**
 * Determines if the file looks like a piece of RFC2822 mail by grabbing all
 * non-indented lines and checking if they look like they begin with valid
 * header field names.
 *
 * Returns 1 if the file looks like a piece of mail, 0 otherwise.
 */
ایستا صحیح is_mail(FILE *fp)
{
	ثابت نویسه *header_regex = "^[!-9;-~]+:";
	ساختار strbuf sb = STRBUF_INIT;
	regex_t regex;
	صحیح ret = 1;

	اگر (fseek(fp, 0L, SEEK_SET))
		die_errno(_("fseek failed"));

	اگر (regcomp(&regex, header_regex, REG_NOSUB | REG_EXTENDED))
		die("invalid pattern: %s", header_regex);

	وقتی (!strbuf_getline(&sb, fp)) {
		اگر (!sb.len)
			بشکن; /* End of header */

		/* Ignore indented folded lines */
		اگر (*sb.buf == '\t' || *sb.buf == ' ')
			ادامه;

		/* It's a header if it matches header_regex */
		اگر (regexec(&regex, sb.buf, 0, پوچ, 0)) {
			ret = 0;
			برو done;
		}
	}

done:
	regfree(&regex);
	strbuf_release(&sb);
	بازگشت ret;
}

/**
 * Attempts to detect the patch_format of the patches contained in `paths`,
 * returning the PATCH_FORMAT_* enum value. Returns PATCH_FORMAT_UNKNOWN if
 * detection fails.
 */
ایستا صحیح detect_patch_format(ثابت نویسه **paths)
{
	شمارش patch_format ret = PATCH_FORMAT_UNKNOWN;
	ساختار strbuf l1 = STRBUF_INIT;
	ساختار strbuf l2 = STRBUF_INIT;
	ساختار strbuf l3 = STRBUF_INIT;
	FILE *fp;

	/*
	 * We default to mbox format if input is from stdin and for directories
	 */
	اگر (!*paths || !strcmp(*paths, "-") || is_directory(*paths))
		بازگشت PATCH_FORMAT_MBOX;

	/*
	 * Otherwise, check the first few lines of the first patch, starting
	 * from the first non-blank line, to try to detect its format.
	 */

	fp = xfopen(*paths, "r");

	وقتی (!strbuf_getline(&l1, fp)) {
		اگر (l1.len)
			بشکن;
	}

	اگر (starts_with(l1.buf, "From ") || starts_with(l1.buf, "From: ")) {
		ret = PATCH_FORMAT_MBOX;
		برو done;
	}

	اگر (starts_with(l1.buf, "# This series applies on GIT commit")) {
		ret = PATCH_FORMAT_STGIT_SERIES;
		برو done;
	}

	اگر (!strcmp(l1.buf, "# HG changeset patch")) {
		ret = PATCH_FORMAT_HG;
		برو done;
	}

	strbuf_getline(&l2, fp);
	strbuf_getline(&l3, fp);

	/*
	 * If the second line is empty and the third is a From, Author or Date
	 * entry, this is likely an StGit patch.
	 */
	اگر (l1.len && !l2.len &&
		(starts_with(l3.buf, "From:") ||
		 starts_with(l3.buf, "Author:") ||
		 starts_with(l3.buf, "Date:"))) {
		ret = PATCH_FORMAT_STGIT;
		برو done;
	}

	اگر (l1.len && is_mail(fp)) {
		ret = PATCH_FORMAT_MBOX;
		برو done;
	}

done:
	fclose(fp);
	strbuf_release(&l1);
	strbuf_release(&l2);
	strbuf_release(&l3);
	بازگشت ret;
}

/**
 * Splits out individual email patches from `paths`, where each path is either
 * a mbox file or a Maildir. Returns 0 on success, -1 on failure.
 */
ایستا صحیح split_mail_mbox(ساختار am_state *state, ثابت نویسه **paths,
				صحیح keep_cr, صحیح mboxrd)
{
	ساختار child_process cp = CHILD_PROCESS_INIT;
	ساختار strbuf last = STRBUF_INIT;
	صحیح ret;

	cp.git_cmd = 1;
	strvec_افزودن(&cp.args, "mailsplit");
	strvec_pushf(&cp.args, "-d%d", state->prec);
	strvec_pushf(&cp.args, "-o%s", state->dir);
	strvec_افزودن(&cp.args, "-b");
	اگر (keep_cr)
		strvec_افزودن(&cp.args, "--keep-cr");
	اگر (mboxrd)
		strvec_افزودن(&cp.args, "--mboxrd");
	strvec_افزودن(&cp.args, "--");
	strvec_pushv(&cp.args, paths);

	ret = capture_command(&cp, &last, 8);
	اگر (ret)
		برو exit;

	state->cur = 1;
	state->last = strtol(last.buf, پوچ, 10);

exit:
	strbuf_release(&last);
	بازگشت ret ? -1 : 0;
}

/**
 * Callback signature for split_mail_conv(). The foreign patch should be
 * read from `in`, and the converted patch (in RFC2822 mail format) should be
 * written to `out`. Return 0 on success, or -1 on failure.
 */
تعریفنوع صحیح (*mail_conv_fn)(FILE *out, FILE *in, صحیح keep_cr);

/**
 * Calls `fn` for each file in `paths` to convert the foreign patch to the
 * RFC2822 mail format suitable for parsing with git-mailinfo.
 *
 * Returns 0 on success, -1 on failure.
 */
ایستا صحیح split_mail_conv(mail_conv_fn fn, ساختار am_state *state,
			ثابت نویسه **paths, صحیح keep_cr)
{
	ایستا ثابت نویسه *stdin_only[] = {"-", پوچ};
	صحیح i;

	اگر (!*paths)
		paths = stdin_only;

	برای (i = 0; *paths; paths++, i++) {
		FILE *in, *out;
		ثابت نویسه *mail;
		صحیح ret;

		اگر (!strcmp(*paths, "-"))
			in = stdin;
		وگرنه
			in = fopen(*paths, "r");

		اگر (!in)
			بازگشت error_errno(_("could not open '%s' for reading"),
					   *paths);

		mail = mkpath("%s/%0*d", state->dir, state->prec, i + 1);

		out = fopen(mail, "w");
		اگر (!out) {
			اگر (in != stdin)
				fclose(in);
			بازگشت error_errno(_("could not open '%s' for writing"),
					   mail);
		}

		ret = fn(out, in, keep_cr);

		fclose(out);
		اگر (in != stdin)
			fclose(in);

		اگر (ret)
			بازگشت error(_("could not parse patch '%s'"), *paths);
	}

	state->cur = 1;
	state->last = i;
	بازگشت 0;
}

/**
 * A split_mail_conv() callback that converts an StGit patch to an RFC2822
 * message suitable for parsing with git-mailinfo.
 */
ایستا صحیح stgit_patch_to_mail(FILE *out, FILE *in, صحیح keep_cr UNUSED)
{
	ساختار strbuf sb = STRBUF_INIT;
	صحیح subject_printed = 0;

	وقتی (!strbuf_getline_lf(&sb, in)) {
		ثابت نویسه *str;

		اگر (str_isspace(sb.buf))
			ادامه;
		وگرنه اگر (skip_prefix(sb.buf, "Author:", &str))
			fprintf(out, "From:%s\n", str);
		وگرنه اگر (starts_with(sb.buf, "From") || starts_with(sb.buf, "Date"))
			fprintf(out, "%s\n", sb.buf);
		وگرنه اگر (!subject_printed) {
			fprintf(out, "Subject: %s\n", sb.buf);
			subject_printed = 1;
		} وگرنه {
			fprintf(out, "\n%s\n", sb.buf);
			بشکن;
		}
	}

	strbuf_reset(&sb);
	وقتی (strbuf_fread(&sb, 8192, in) > 0) {
		fwrite(sb.buf, 1, sb.len, out);
		strbuf_reset(&sb);
	}

	strbuf_release(&sb);
	بازگشت 0;
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
ایستا صحیح split_mail_stgit_series(ساختار am_state *state, ثابت نویسه **paths,
					صحیح keep_cr)
{
	ثابت نویسه *series_dir;
	نویسه *series_dir_buf;
	FILE *fp;
	ساختار strvec patches = STRVEC_INIT;
	ساختار strbuf sb = STRBUF_INIT;
	صحیح ret;

	اگر (!paths[0] || paths[1])
		بازگشت error(_("Only one StGIT patch series can be applied at once"));

	series_dir_buf = xstrdup(*paths);
	series_dir = dirname(series_dir_buf);

	fp = fopen(*paths, "r");
	اگر (!fp) {
		آزاد(series_dir_buf);
		بازگشت error_errno(_("could not open '%s' for reading"), *paths);
	}

	وقتی (!strbuf_getline_lf(&sb, fp)) {
		اگر (*sb.buf == '#')
			ادامه; /* skip comment lines */

		strvec_افزودن(&patches, mkpath("%s/%s", series_dir, sb.buf));
	}

	fclose(fp);
	strbuf_release(&sb);
	آزاد(series_dir_buf);

	ret = split_mail_conv(stgit_patch_to_mail, state, patches.v, keep_cr);

	strvec_clear(&patches);
	بازگشت ret;
}

/**
 * A split_patches_conv() callback that converts a mercurial patch to a RFC2822
 * message suitable for parsing with git-mailinfo.
 */
ایستا صحیح hg_patch_to_mail(FILE *out, FILE *in, صحیح keep_cr UNUSED)
{
	ساختار strbuf sb = STRBUF_INIT;
	صحیح rc = 0;

	وقتی (!strbuf_getline_lf(&sb, in)) {
		ثابت نویسه *str;

		اگر (skip_prefix(sb.buf, "# User ", &str))
			fprintf(out, "From: %s\n", str);
		وگرنه اگر (skip_prefix(sb.buf, "# Date ", &str)) {
			timestamp_t timestamp;
			long tz, tz2;
			نویسه *end;

			errno = 0;
			timestamp = parse_timestamp(str, &end, 10);
			اگر (errno) {
				rc = error(_("invalid timestamp"));
				برو exit;
			}

			اگر (!skip_prefix(end, " ", &str)) {
				rc = error(_("invalid Date line"));
				برو exit;
			}

			errno = 0;
			tz = strtol(str, &end, 10);
			اگر (errno) {
				rc = error(_("invalid timezone offset"));
				برو exit;
			}

			اگر (*end) {
				rc = error(_("invalid Date line"));
				برو exit;
			}

			/*
			 * mercurial's timezone is in seconds west of UTC,
			 * however git's timezone is in hours + minutes east of
			 * UTC. Convert it.
			 */
			tz2 = labs(tz) / 3600 * 100 + labs(tz) % 3600 / 60;
			اگر (tz > 0)
				tz2 = -tz2;

			fprintf(out, "Date: %s\n", show_date(timestamp, tz2, DATE_MODE(RFC2822)));
		} وگرنه اگر (starts_with(sb.buf, "# ")) {
			ادامه;
		} وگرنه {
			fprintf(out, "\n%s\n", sb.buf);
			بشکن;
		}
	}

	strbuf_reset(&sb);
	وقتی (strbuf_fread(&sb, 8192, in) > 0) {
		fwrite(sb.buf, 1, sb.len, out);
		strbuf_reset(&sb);
	}
exit:
	strbuf_release(&sb);
	بازگشت rc;
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
ایستا صحیح split_mail(ساختار am_state *state, شمارش patch_format patch_format,
			ثابت نویسه **paths, صحیح keep_cr)
{
	اگر (keep_cr < 0) {
		keep_cr = 0;
		repo_config_get_bool(the_repository, "am.keepcr", &keep_cr);
	}

	انتخاب (patch_format) {
	حالت PATCH_FORMAT_MBOX:
		بازگشت split_mail_mbox(state, paths, keep_cr, 0);
	حالت PATCH_FORMAT_STGIT:
		بازگشت split_mail_conv(stgit_patch_to_mail, state, paths, keep_cr);
	حالت PATCH_FORMAT_STGIT_SERIES:
		بازگشت split_mail_stgit_series(state, paths, keep_cr);
	حالت PATCH_FORMAT_HG:
		بازگشت split_mail_conv(hg_patch_to_mail, state, paths, keep_cr);
	حالت PATCH_FORMAT_MBOXRD:
		بازگشت split_mail_mbox(state, paths, keep_cr, 1);
	پیشفرض:
		BUG("invalid patch_format");
	}
	بازگشت -1;
}

/**
 * Setup a new am session for applying patches
 */
ایستا تهی am_setup(ساختار am_state *state, شمارش patch_format patch_format,
			ثابت نویسه **paths, صحیح keep_cr)
{
	ساختار object_id curr_head;
	ثابت نویسه *str;
	ساختار strbuf sb = STRBUF_INIT;

	اگر (!patch_format)
		patch_format = detect_patch_format(paths);

	اگر (!patch_format) {
		fprintf_ln(stderr, _("Patch format detection failed."));
		die(پوچ);
	}

	اگر (mkdir(state->dir, 0777) < 0 && errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), state->dir);
	refs_delete_ref(get_main_ref_store(the_repository), پوچ,
			"REBASE_HEAD", پوچ, REF_NO_DEREF);

	اگر (split_mail(state, patch_format, paths, keep_cr) < 0) {
		am_destroy(state);
		die(_("Failed to split patches."));
	}

	اگر (state->rebasing)
		state->threeway = 1;

	write_state_bool(state, "threeway", state->threeway);
	write_state_bool(state, "quiet", state->quiet);
	write_state_bool(state, "sign", state->signoff);
	write_state_bool(state, "utf8", state->utf8);

	اگر (state->allow_rerere_autoupdate)
		write_state_bool(state, "rerere-autoupdate",
			 state->allow_rerere_autoupdate == RERERE_AUTOUPDATE);

	انتخاب (state->keep) {
	حالت KEEP_FALSE:
		str = "f";
		بشکن;
	حالت KEEP_TRUE:
		str = "t";
		بشکن;
	حالت KEEP_NON_PATCH:
		str = "b";
		بشکن;
	پیشفرض:
		BUG("invalid value for state->keep");
	}

	write_state_text(state, "keep", str);
	write_state_bool(state, "messageid", state->message_id);

	انتخاب (state->scissors) {
	حالت SCISSORS_UNSET:
		str = "";
		بشکن;
	حالت SCISSORS_FALSE:
		str = "f";
		بشکن;
	حالت SCISSORS_TRUE:
		str = "t";
		بشکن;
	پیشفرض:
		BUG("invalid value for state->scissors");
	}
	write_state_text(state, "scissors", str);

	انتخاب (state->quoted_cr) {
	حالت quoted_cr_unset:
		str = "";
		بشکن;
	حالت quoted_cr_nowarn:
		str = "nowarn";
		بشکن;
	حالت quoted_cr_warn:
		str = "warn";
		بشکن;
	حالت quoted_cr_strip:
		str = "strip";
		بشکن;
	پیشفرض:
		BUG("invalid value for state->quoted_cr");
	}
	write_state_text(state, "quoted-cr", str);

	sq_quote_argv(&sb, state->git_apply_opts.v);
	write_state_text(state, "apply-opt", sb.buf);

	اگر (state->rebasing)
		write_state_text(state, "rebasing", "");
	وگرنه
		write_state_text(state, "applying", "");

	اگر (!repo_get_oid(the_repository, "HEAD", &curr_head)) {
		write_state_text(state, "abort-safety", oid_to_hex(&curr_head));
		اگر (!state->rebasing)
			refs_update_ref(get_main_ref_store(the_repository),
					"am", "ORIG_HEAD", &curr_head, پوچ,
					0,
					UPDATE_REFS_DIE_ON_ERR);
	} وگرنه {
		write_state_text(state, "abort-safety", "");
		اگر (!state->rebasing)
			refs_delete_ref(get_main_ref_store(the_repository),
					پوچ, "ORIG_HEAD", پوچ, 0);
	}

	/*
	 * NOTE: Since the "next" and "last" files determine if an am_state
	 * session is in progress, they should be written last.
	 */

	write_state_شمار(state, "next", state->cur);
	write_state_شمار(state, "last", state->last);

	strbuf_release(&sb);
}

/**
 * Increments the patch pointer, and cleans am_state for the application of the
 * next patch.
 */
ایستا تهی am_next(ساختار am_state *state)
{
	ساختار object_id head;

	FREE_AND_NULL(state->author_name);
	FREE_AND_NULL(state->author_email);
	FREE_AND_NULL(state->author_date);
	FREE_AND_NULL(state->پیام);
	state->پیام_len = 0;

	unlink(am_path(state, "author-script"));
	unlink(am_path(state, "final-commit"));

	oidclr(&state->orig_commit, the_repository->hash_algo);
	unlink(am_path(state, "original-commit"));
	refs_delete_ref(get_main_ref_store(the_repository), پوچ,
			"REBASE_HEAD", پوچ, REF_NO_DEREF);

	اگر (!repo_get_oid(the_repository, "HEAD", &head))
		write_state_text(state, "abort-safety", oid_to_hex(&head));
	وگرنه
		write_state_text(state, "abort-safety", "");

	state->cur++;
	write_state_شمار(state, "next", state->cur);
}

/**
 * Returns the filename of the current patch email.
 */
ایستا ثابت نویسه *msgnum(ثابت ساختار am_state *state)
{
	ایستا ساختار strbuf sb = STRBUF_INIT;

	strbuf_reset(&sb);
	strbuf_addf(&sb, "%0*d", state->prec, state->cur);

	بازگشت sb.buf;
}

/**
 * Dies with a user-friendly message on how to proceed after resolving the
 * problem. This message can be overridden with state->resolvemsg.
 */
ایستا تهی NORETURN die_user_resolve(ثابت ساختار am_state *state)
{
	اگر (state->resolvemsg) {
		advise_if_enabled(ADVICE_MERGE_CONFLICT, "%s", state->resolvemsg);
	} وگرنه {
		ثابت نویسه *cmdline = state->interactive ? "git am -i" : "git am";
		ساختار strbuf sb = STRBUF_INIT;

		strbuf_addf(&sb, _("When you have resolved this problem, run \"%s --continue\".\n"), cmdline);
		strbuf_addf(&sb, _("If you prefer to skip this patch, run \"%s --skip\" instead.\n"), cmdline);

		اگر (advice_enabled(ADVICE_AM_WORK_DIR) &&
		    is_empty_or_missing_file(am_path(state, "patch")) &&
		    !repo_index_has_changes(the_repository, پوچ, پوچ))
			strbuf_addf(&sb, _("To record the empty patch as an empty commit, run \"%s --allow-empty\".\n"), cmdline);

		strbuf_addf(&sb, _("To restore the original branch and stop patching, run \"%s --abort\"."), cmdline);

		advise_if_enabled(ADVICE_MERGE_CONFLICT, "%s", sb.buf);
		strbuf_release(&sb);
	}

	die(پوچ);
}

/**
 * Appends signoff to the "msg" field of the am_state.
 */
ایستا تهی am_append_signoff(ساختار am_state *state)
{
	ساختار strbuf sb = STRBUF_INIT;

	strbuf_attach(&sb, state->پیام, state->پیام_len, state->پیام_len + 1);
	append_signoff(&sb, 0, 0);
	state->پیام = strbuf_detach(&sb, &state->پیام_len);
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
ایستا صحیح parse_mail(ساختار am_state *state, ثابت نویسه *mail)
{
	FILE *fp;
	ساختار strbuf sb = STRBUF_INIT;
	ساختار strbuf پیام = STRBUF_INIT;
	ساختار strbuf author_name = STRBUF_INIT;
	ساختار strbuf author_date = STRBUF_INIT;
	ساختار strbuf author_email = STRBUF_INIT;
	صحیح ret = 0;
	ساختار mailinfo mi;

	setup_mailinfo(the_repository, &mi);

	اگر (state->utf8)
		mi.metainfo_charset = get_commit_output_encoding();
	وگرنه
		mi.metainfo_charset = پوچ;

	انتخاب (state->keep) {
	حالت KEEP_FALSE:
		بشکن;
	حالت KEEP_TRUE:
		mi.keep_subject = 1;
		بشکن;
	حالت KEEP_NON_PATCH:
		mi.keep_non_patch_brackets_in_subject = 1;
		بشکن;
	پیشفرض:
		BUG("invalid value for state->keep");
	}

	اگر (state->message_id)
		mi.add_message_id = 1;

	انتخاب (state->scissors) {
	حالت SCISSORS_UNSET:
		بشکن;
	حالت SCISSORS_FALSE:
		mi.use_scissors = 0;
		بشکن;
	حالت SCISSORS_TRUE:
		mi.use_scissors = 1;
		بشکن;
	پیشفرض:
		BUG("invalid value for state->scissors");
	}

	انتخاب (state->quoted_cr) {
	حالت quoted_cr_unset:
		بشکن;
	حالت quoted_cr_nowarn:
	حالت quoted_cr_warn:
	حالت quoted_cr_strip:
		mi.quoted_cr = state->quoted_cr;
		بشکن;
	پیشفرض:
		BUG("invalid value for state->quoted_cr");
	}

	mi.درون_داد = xfopen(mail, "r");
	mi.output = xfopen(am_path(state, "info"), "w");
	اگر (mailinfo(&mi, am_path(state, "msg"), am_path(state, "patch")))
		die("could not parse patch");

	fclose(mi.درون_داد);
	fclose(mi.output);

	اگر (mi.format_flowed)
		warning(_("Patch sent with format=flowed; "
			  "space at the end of lines might be lost."));

	/* Extract message and author information */
	fp = xfopen(am_path(state, "info"), "r");
	وقتی (!strbuf_getline_lf(&sb, fp)) {
		ثابت نویسه *x;

		اگر (skip_prefix(sb.buf, "Subject: ", &x)) {
			اگر (پیام.len)
				strbuf_addch(&پیام, '\n');
			strbuf_addstr(&پیام, x);
		} وگرنه اگر (skip_prefix(sb.buf, "Author: ", &x))
			strbuf_addstr(&author_name, x);
		وگرنه اگر (skip_prefix(sb.buf, "Email: ", &x))
			strbuf_addstr(&author_email, x);
		وگرنه اگر (skip_prefix(sb.buf, "Date: ", &x))
			strbuf_addstr(&author_date, x);
	}
	fclose(fp);

	/* Skip pine's internal folder data */
	اگر (!strcmp(author_name.buf, "Mail System Internal Data")) {
		ret = 1;
		برو finish;
	}

	strbuf_addstr(&پیام, "\n\n");
	strbuf_addbuf(&پیام, &mi.log_message);
	strbuf_stripspace(&پیام, پوچ);

	أكد(!state->author_name);
	state->author_name = strbuf_detach(&author_name, پوچ);

	أكد(!state->author_email);
	state->author_email = strbuf_detach(&author_email, پوچ);

	أكد(!state->author_date);
	state->author_date = strbuf_detach(&author_date, پوچ);

	أكد(!state->پیام);
	state->پیام = strbuf_detach(&پیام, &state->پیام_len);

finish:
	strbuf_release(&پیام);
	strbuf_release(&author_date);
	strbuf_release(&author_email);
	strbuf_release(&author_name);
	strbuf_release(&sb);
	clear_mailinfo(&mi);
	بازگشت ret;
}

/**
 * Sets commit_id to the commit hash where the mail was generated from.
 * Returns 0 on success, -1 on failure.
 */
ایستا صحیح get_mail_commit_oid(ساختار object_id *commit_id, ثابت نویسه *mail)
{
	ساختار strbuf sb = STRBUF_INIT;
	FILE *fp = xfopen(mail, "r");
	ثابت نویسه *x;
	صحیح ret = 0;

	اگر (strbuf_getline_lf(&sb, fp) ||
	    !skip_prefix(sb.buf, "From ", &x) ||
	    get_oid_hex(x, commit_id) < 0)
		ret = -1;

	strbuf_release(&sb);
	fclose(fp);
	بازگشت ret;
}

/**
 * Sets state->msg, state->author_name, state->author_email, state->author_date
 * to the commit's respective info.
 */
ایستا تهی get_commit_info(ساختار am_state *state, ساختار commit *commit)
{
	ثابت نویسه *buffer, *ident_سطر, *پیام;
	size_t ident_len;
	ساختار ident_split id;

	buffer = repo_logmsg_reencode(the_repository, commit, پوچ,
				      get_commit_output_encoding());

	ident_سطر = find_commit_header(buffer, "author", &ident_len);
	اگر (!ident_سطر)
		die(_("missing author line in commit %s"),
		      oid_to_hex(&commit->object.oid));
	اگر (split_ident_سطر(&id, ident_سطر, ident_len) < 0)
		die(_("invalid ident line: %.*s"), (صحیح)ident_len, ident_سطر);

	أكد(!state->author_name);
	اگر (id.name_begin)
		state->author_name =
			xmemdupz(id.name_begin, id.name_end - id.name_begin);
	وگرنه
		state->author_name = xstrdup("");

	أكد(!state->author_email);
	اگر (id.mail_begin)
		state->author_email =
			xmemdupz(id.mail_begin, id.mail_end - id.mail_begin);
	وگرنه
		state->author_email = xstrdup("");

	أكد(!state->author_date);
	state->author_date = xstrdup(show_ident_date(&id, DATE_MODE(NORMAL)));

	أكد(!state->پیام);
	پیام = strstr(buffer, "\n\n");
	اگر (!پیام)
		die(_("unable to parse commit %s"), oid_to_hex(&commit->object.oid));
	state->پیام = xstrdup(پیام + 2);
	state->پیام_len = strlen(state->پیام);
	repo_unuse_commit_buffer(the_repository, commit, buffer);
}

/**
 * Writes `commit` as a patch to the state directory's "patch" file.
 */
ایستا تهی write_commit_patch(ثابت ساختار am_state *state, ساختار commit *commit)
{
	ساختار rev_info rev_info;
	FILE *fp;

	fp = xfopen(am_path(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, پوچ);
	rev_info.diff = 1;
	rev_info.abbrev = 0;
	rev_info.disable_stdin = 1;
	rev_info.show_ریشه_diff = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.flags.دودویی = 1;
	rev_info.diffopt.flags.full_index = 1;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	add_pending_object(&rev_info, &commit->object, "");
	diff_setup_done(&rev_info.diffopt);
	log_درخت_commit(&rev_info, commit);
	release_revisions(&rev_info);
}

/**
 * Writes the diff of the index against HEAD as a patch to the state
 * directory's "patch" file.
 */
ایستا تهی write_index_patch(ثابت ساختار am_state *state)
{
	ساختار درخت *درخت;
	ساختار object_id head;
	ساختار rev_info rev_info;
	FILE *fp;

	اگر (!repo_get_oid(the_repository, "HEAD", &head)) {
		ساختار commit *commit = lookup_commit_or_die(&head, "HEAD");
		درخت = repo_get_commit_درخت(the_repository, commit);
	} وگرنه
		درخت = lookup_درخت(the_repository,
				   the_repository->hash_algo->empty_درخت);

	fp = xfopen(am_path(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, پوچ);
	rev_info.diff = 1;
	rev_info.disable_stdin = 1;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	add_pending_object(&rev_info, &درخت->object, "");
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
ایستا صحیح parse_mail_rebase(ساختار am_state *state, ثابت نویسه *mail)
{
	ساختار commit *commit;
	ساختار object_id commit_oid;

	اگر (get_mail_commit_oid(&commit_oid, mail) < 0)
		die(_("could not parse %s"), mail);

	commit = lookup_commit_or_die(&commit_oid, mail);

	get_commit_info(state, commit);

	write_commit_patch(state, commit);

	oidcpy(&state->orig_commit, &commit_oid);
	write_state_text(state, "original-commit", oid_to_hex(&commit_oid));
	refs_update_ref(get_main_ref_store(the_repository), "am",
			"REBASE_HEAD", &commit_oid,
			پوچ, REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);

	بازگشت 0;
}

/**
 * Applies current patch with git-apply. Returns 0 on success, -1 otherwise. If
 * `index_file` is not NULL, the patch will be applied to that index.
 */
ایستا صحیح run_apply(ثابت ساختار am_state *state, ثابت نویسه *index_file)
{
	ساختار strvec apply_paths = STRVEC_INIT;
	ساختار strvec apply_opts = STRVEC_INIT;
	ساختار apply_state apply_state;
	صحیح res, opts_چپ;
	صحیح force_apply = 0;
	صحیح options = 0;
	ثابت نویسه **apply_argv;

	اگر (init_apply_state(&apply_state, the_repository, پوچ))
		BUG("init_apply_state() failed");

	strvec_افزودن(&apply_opts, "apply");
	strvec_pushv(&apply_opts, state->git_apply_opts.v);

	/*
	 * Build a copy that apply_parse_options() can rearrange.
	 * apply_opts.v keeps referencing the allocated strings for
	 * strvec_clear() to release.
	 */
	DUP_ARRAY(apply_argv, apply_opts.v, apply_opts.nr);

	opts_چپ = apply_parse_options(apply_opts.nr, apply_argv,
					&apply_state, &force_apply, &options,
					پوچ);

	اگر (opts_چپ != 0)
		die("unknown option passed through to git apply");

	اگر (index_file) {
		apply_state.index_file = index_file;
		apply_state.cached = 1;
	} وگرنه
		apply_state.check_index = 1;

	/*
	 * If we are allowed to fall back on 3-way merge, don't give false
	 * errors during the initial attempt.
	 */
	اگر (state->threeway && !index_file)
		apply_state.apply_verbosity = verbosity_silent;

	اگر (check_apply_state(&apply_state, force_apply))
		BUG("check_apply_state() failed");

	strvec_افزودن(&apply_paths, am_path(state, "patch"));

	res = apply_all_patches(&apply_state, apply_paths.nr, apply_paths.v, options);

	strvec_clear(&apply_paths);
	strvec_clear(&apply_opts);
	clear_apply_state(&apply_state);
	آزاد(apply_argv);

	اگر (res)
		بازگشت res;

	اگر (index_file) {
		/* Reload index as apply_all_patches() will have modified it. */
		discard_index(the_repository->index);
		read_index_from(the_repository->index, index_file,
				repo_get_git_dir(the_repository));
	}

	بازگشت 0;
}

/**
 * Builds an index that contains just the blobs needed for a 3way merge.
 */
ایستا صحیح build_fake_ancestor(ثابت ساختار am_state *state, ثابت نویسه *index_file)
{
	ساختار child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	strvec_افزودن(&cp.args, "apply");
	strvec_pushv(&cp.args, state->git_apply_opts.v);
	strvec_pushf(&cp.args, "--build-fake-ancestor=%s", index_file);
	strvec_افزودن(&cp.args, am_path(state, "patch"));

	اگر (run_command(&cp))
		بازگشت -1;

	بازگشت 0;
}

/**
 * Attempt a threeway merge, using index_path as the temporary index.
 */
ایستا صحیح fall_back_threeway(ثابت ساختار am_state *state, ثابت نویسه *index_path)
{
	ساختار object_id their_درخت, our_درخت;
	ساختار object_id bases[1] = { 0 };
	ساختار merge_options o;
	ساختار commit *result;
	نویسه *their_درخت_name;

	اگر (repo_get_oid(the_repository, "HEAD", &our_درخت) < 0)
		oidcpy(&our_درخت, the_hash_algo->empty_درخت);

	اگر (build_fake_ancestor(state, index_path))
		بازگشت error("could not build fake ancestor");

	discard_index(the_repository->index);
	read_index_from(the_repository->index, index_path, repo_get_git_dir(the_repository));

	اگر (write_index_as_درخت(&bases[0], the_repository->index, index_path, 0, پوچ))
		بازگشت error(_("Repository lacks necessary blobs to fall back on 3-way merge."));

	say(state, stdout, _("Using index info to reconstruct a base tree..."));

	اگر (!state->quiet) {
		/*
		 * List paths that needed 3-way fallback, so that the user can
		 * review them with extra care to spot mismerges.
		 */
		ساختار rev_info rev_info;

		repo_init_revisions(the_repository, &rev_info, پوچ);
		rev_info.diffopt.output_format = DIFF_FORMAT_NAME_STATUS;
		rev_info.diffopt.filter |= diff_filter_bit('A');
		rev_info.diffopt.filter |= diff_filter_bit('M');
		add_pending_oid(&rev_info, "HEAD", &our_درخت, 0);
		diff_setup_done(&rev_info.diffopt);
		run_diff_index(&rev_info, DIFF_INDEX_CACHED);
		release_revisions(&rev_info);
	}

	اگر (run_apply(state, index_path))
		بازگشت error(_("Did you hand edit your patch?\n"
				"It does not apply to blobs recorded in its index."));

	اگر (write_index_as_درخت(&their_درخت, the_repository->index, index_path, 0, پوچ))
		بازگشت error("could not write tree");

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
	their_درخت_name = xstrfmt("%.*s", linelen(state->پیام), state->پیام);
	o.branch2 = their_درخت_name;
	o.ancestor = "constructed fake ancestor";
	o.detect_directory_renames = MERGE_DIRECTORY_RENAMES_NONE;

	اگر (state->quiet)
		o.verbosity = 0;

	اگر (merge_ort_generic(&o, &our_درخت, &their_درخت, 1, bases, &result)) {
		repo_rerere(the_repository, state->allow_rerere_autoupdate);
		آزاد(their_درخت_name);
		بازگشت error(_("Failed to merge in the changes."));
	}

	آزاد(their_درخت_name);
	بازگشت 0;
}

/**
 * Commits the current index with state->msg as the commit message and
 * state->author_name, state->author_email and state->author_date as the author
 * information.
 */
ایستا تهی do_commit(ثابت ساختار am_state *state)
{
	ساختار object_id درخت, parent, commit;
	ثابت ساختار object_id *old_oid;
	ساختار commit_list *parents = پوچ;
	ثابت نویسه *reflog_پیام, *author, *committer = پوچ;
	ساختار strbuf sb = STRBUF_INIT;

	اگر (!state->no_verify && run_hooks(the_repository, "pre-applypatch"))
		exit(1);

	اگر (write_index_as_درخت(&درخت, the_repository->index,
				repo_get_index_file(the_repository),
				0, پوچ))
		die(_("git write-tree failed to write a tree"));

	اگر (!repo_get_oid_commit(the_repository, "HEAD", &parent)) {
		old_oid = &parent;
		commit_list_درج(lookup_commit(the_repository, &parent),
				   &parents);
	} وگرنه {
		old_oid = پوچ;
		say(state, stderr, _("applying to an empty history"));
	}

	author = fmt_ident(state->author_name, state->author_email,
		WANT_AUTHOR_IDENT,
			state->ignore_date ? پوچ : state->author_date,
			IDENT_STRICT);

	اگر (state->committer_date_is_author_date)
		committer = fmt_ident(getenv("GIT_COMMITTER_NAME"),
				      getenv("GIT_COMMITTER_EMAIL"),
				      WANT_COMMITTER_IDENT,
				      state->ignore_date ? پوچ
							 : state->author_date,
				      IDENT_STRICT);

	اگر (commit_درخت_extended(state->پیام, state->پیام_len, &درخت, parents,
				 &commit, author, committer, state->sign_commit,
				 پوچ))
		die(_("failed to write commit object"));

	reflog_پیام = getenv("GIT_REFLOG_ACTION");
	اگر (!reflog_پیام)
		reflog_پیام = "am";

	strbuf_addf(&sb, "%s: %.*s", reflog_پیام, linelen(state->پیام),
			state->پیام);

	refs_update_ref(get_main_ref_store(the_repository), sb.buf, "HEAD",
			&commit, old_oid, 0,
			UPDATE_REFS_DIE_ON_ERR);

	اگر (state->rebasing) {
		FILE *fp = xfopen(am_path(state, "rewritten"), "a");

		أكد(!is_null_oid(&state->orig_commit));
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
ایستا تهی validate_resume_state(ثابت ساختار am_state *state)
{
	اگر (!state->پیام)
		die(_("cannot resume: %s does not exist."),
			am_path(state, "final-commit"));

	اگر (!state->author_name || !state->author_email || !state->author_date)
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
ایستا صحیح do_interactive(ساختار am_state *state)
{
	أكد(state->پیام);

	برای (;;) {
		نویسه reply[64];

		puts(_("Commit Body is:"));
		puts("--------------------------");
		printf("%s", state->پیام);
		puts("--------------------------");

		/*
		 * TRANSLATORS: Make sure to include [y], [n], [e], [v] and [a]
		 * in your translation. The program will only accept English
		 * input at this point.
		 */
		printf(_("Apply? [y]es/[n]o/[e]dit/[v]iew patch/[a]ccept all: "));
		اگر (!fgets(reply, اندازه(reply), stdin))
			die("unable to read from stdin; aborting");

		اگر (*reply == 'y' || *reply == 'Y') {
			بازگشت 0;
		} وگرنه اگر (*reply == 'a' || *reply == 'A') {
			state->interactive = 0;
			بازگشت 0;
		} وگرنه اگر (*reply == 'n' || *reply == 'N') {
			بازگشت 1;
		} وگرنه اگر (*reply == 'e' || *reply == 'E') {
			ساختار strbuf پیام = STRBUF_INIT;

			اگر (!launch_editor(am_path(state, "final-commit"), &پیام, پوچ)) {
				آزاد(state->پیام);
				state->پیام = strbuf_detach(&پیام, &state->پیام_len);
			}
			strbuf_release(&پیام);
		} وگرنه اگر (*reply == 'v' || *reply == 'V') {
			ثابت نویسه *pager = git_pager(the_repository, 1);
			ساختار child_process cp = CHILD_PROCESS_INIT;

			اگر (!pager)
				pager = "cat";
			prepare_pager_args(&cp, pager);
			strvec_افزودن(&cp.args, am_path(state, "patch"));
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
ایستا تهی am_run(ساختار am_state *state, صحیح resume)
{
	ساختار strbuf sb = STRBUF_INIT;

	unlink(am_path(state, "dirtyindex"));

	اگر (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 پوچ, پوچ, پوچ) < 0)
		die(_("unable to write index file"));

	اگر (repo_index_has_changes(the_repository, پوچ, &sb)) {
		write_state_bool(state, "dirtyindex", 1);
		die(_("Dirty index: cannot apply patches (dirty: %s)"), sb.buf);
	}

	strbuf_release(&sb);

	وقتی (state->cur <= state->last) {
		ثابت نویسه *mail = am_path(state, msgnum(state));
		صحیح apply_status;
		صحیح to_keep;

		reset_ident_date();

		اگر (!file_exists(mail))
			برو next;

		اگر (resume) {
			validate_resume_state(state);
		} وگرنه {
			صحیح skip;

			اگر (state->rebasing)
				skip = parse_mail_rebase(state, mail);
			وگرنه
				skip = parse_mail(state, mail);

			اگر (skip)
				برو next; /* mail should be skipped */

			اگر (state->signoff)
				am_append_signoff(state);

			write_author_script(state);
			write_commit_پیام(state);
		}

		اگر (state->interactive && do_interactive(state))
			برو next;

		to_keep = 0;
		اگر (is_empty_or_missing_file(am_path(state, "patch"))) {
			انتخاب (state->empty_type) {
			حالت DROP_EMPTY_COMMIT:
				say(state, stdout, _("Skipping: %.*s"), linelen(state->پیام), state->پیام);
				برو next;
				بشکن;
			حالت KEEP_EMPTY_COMMIT:
				to_keep = 1;
				say(state, stdout, _("Creating an empty commit: %.*s"),
					linelen(state->پیام), state->پیام);
				بشکن;
			حالت STOP_ON_EMPTY_COMMIT:
				printf_ln(_("Patch is empty."));
				die_user_resolve(state);
				بشکن;
			}
		}

		اگر (run_applypatch_پیام_hook(state))
			exit(1);
		اگر (to_keep)
			برو commit;

		say(state, stdout, _("Applying: %.*s"), linelen(state->پیام), state->پیام);

		apply_status = run_apply(state, پوچ);

		اگر (apply_status && state->threeway) {
			ساختار strbuf sb = STRBUF_INIT;

			strbuf_addstr(&sb, am_path(state, "patch-merge-index"));
			apply_status = fall_back_threeway(state, sb.buf);
			strbuf_release(&sb);

			/*
			 * Applying the patch to an earlier tree and merging
			 * the result may have produced the same tree as ours.
			 */
			اگر (!apply_status &&
			    !repo_index_has_changes(the_repository, پوچ, پوچ)) {
				say(state, stdout, _("No changes -- Patch already applied."));
				برو next;
			}
		}

		اگر (apply_status) {
			printf_ln(_("Patch failed at %s %.*s"), msgnum(state),
				linelen(state->پیام), state->پیام);

			اگر (advice_enabled(ADVICE_AM_WORK_DIR))
				advise(_("Use 'git am --show-current-patch=diff' to see the failed patch"));

			die_user_resolve(state);
		}

commit:
		do_commit(state);

next:
		am_next(state);

		اگر (resume)
			am_load(state);
		resume = 0;
	}

	اگر (!is_empty_or_missing_file(am_path(state, "rewritten"))) {
		أكد(state->rebasing);
		copy_notes_for_rebase(state);
		run_post_rewrite_hook(state);
	}

	/*
	 * In rebasing mode, it's up to the caller to take care of
	 * housekeeping.
	 */
	اگر (!state->rebasing) {
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
ایستا تهی am_resolve(ساختار am_state *state, صحیح allow_empty)
{
	validate_resume_state(state);

	say(state, stdout, _("Applying: %.*s"), linelen(state->پیام), state->پیام);

	اگر (!repo_index_has_changes(the_repository, پوچ, پوچ)) {
		اگر (allow_empty && is_empty_or_missing_file(am_path(state, "patch"))) {
			printf_ln(_("No changes - recorded it as an empty commit."));
		} وگرنه {
			printf_ln(_("No changes - did you forget to use 'git add'?\n"
				    "If there is nothing left to stage, chances are that something else\n"
				    "already introduced the same changes; you might want to skip this patch."));
			die_user_resolve(state);
		}
	}

	اگر (unmerged_index(the_repository->index)) {
		printf_ln(_("You still have unmerged paths in your index.\n"
			"You should 'git add' each file with resolved conflicts to mark them as such.\n"
			"You might run `git rm` on a file to accept \"deleted by them\" for it."));
		die_user_resolve(state);
	}

	اگر (state->interactive) {
		write_index_patch(state);
		اگر (do_interactive(state))
			برو next;
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
ایستا صحیح fast_forward_to(ساختار درخت *head, ساختار درخت *remote, صحیح reset)
{
	ساختار lock_file lock_file = LOCK_INIT;
	ساختار unpack_trees_options opts;
	ساختار درخت_desc t[2];

	اگر (repo_parse_درخت(the_repository, head) || repo_parse_درخت(the_repository, remote))
		بازگشت -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	refresh_index(the_repository->index, REFRESH_QUIET, پوچ, پوچ, پوچ);

	memset(&opts, 0, اندازه(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.update = 1;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = twoway_merge;
	init_درخت_desc(&t[0], &head->object.oid, head->buffer, head->size);
	init_درخت_desc(&t[1], &remote->object.oid, remote->buffer, remote->size);

	اگر (unpack_trees(2, t, &opts)) {
		rollback_lock_file(&lock_file);
		بازگشت -1;
	}

	اگر (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	بازگشت 0;
}

/**
 * Merges a tree into the index. The index's stat info will take precedence
 * over the merged tree's. Returns 0 on success, -1 on failure.
 */
ایستا صحیح merge_درخت(ساختار درخت *درخت)
{
	ساختار lock_file lock_file = LOCK_INIT;
	ساختار unpack_trees_options opts;
	ساختار درخت_desc t[1];

	اگر (repo_parse_درخت(the_repository, درخت))
		بازگشت -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, اندازه(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.merge = 1;
	opts.fn = oneway_merge;
	init_درخت_desc(&t[0], &درخت->object.oid, درخت->buffer, درخت->size);

	اگر (unpack_trees(1, t, &opts)) {
		rollback_lock_file(&lock_file);
		بازگشت -1;
	}

	اگر (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	بازگشت 0;
}

/**
 * Clean the index without touching entries that are not modified between
 * `head` and `remote`.
 */
ایستا صحیح clean_index(ثابت ساختار object_id *head, ثابت ساختار object_id *remote)
{
	ساختار درخت *head_درخت, *remote_درخت, *index_درخت;
	ساختار object_id index;

	head_درخت = repo_parse_درخت_indirect(the_repository, head);
	اگر (!head_درخت)
		بازگشت error(_("Could not parse object '%s'."), oid_to_hex(head));

	remote_درخت = repo_parse_درخت_indirect(the_repository, remote);
	اگر (!remote_درخت)
		بازگشت error(_("Could not parse object '%s'."), oid_to_hex(remote));

	repo_read_index_unmerged(the_repository);

	اگر (fast_forward_to(head_درخت, head_درخت, 1))
		بازگشت -1;

	اگر (write_index_as_درخت(&index, the_repository->index,
				repo_get_index_file(the_repository),
				0, پوچ))
		بازگشت -1;

	index_درخت = repo_parse_درخت_indirect(the_repository, &index);
	اگر (!index_درخت)
		بازگشت error(_("Could not parse object '%s'."), oid_to_hex(&index));

	اگر (fast_forward_to(index_درخت, remote_درخت, 0))
		بازگشت -1;

	اگر (merge_درخت(remote_درخت))
		بازگشت -1;

	remove_branch_state(the_repository, 0);

	بازگشت 0;
}

/**
 * Resets rerere's merge resolution metadata.
 */
ایستا تهی am_rerere_clear(تهی)
{
	ساختار string_list merge_rr = STRING_LIST_INIT_DUP;
	rerere_clear(the_repository, &merge_rr);
	string_list_clear(&merge_rr, 1);
}

/**
 * Resume the current am session by skipping the current patch.
 */
ایستا تهی am_skip(ساختار am_state *state)
{
	ساختار object_id head;

	am_rerere_clear();

	اگر (repo_get_oid(the_repository, "HEAD", &head))
		oidcpy(&head, the_hash_algo->empty_درخت);

	اگر (clean_index(&head, &head))
		die(_("failed to clean index"));

	اگر (state->rebasing) {
		FILE *fp = xfopen(am_path(state, "rewritten"), "a");

		أكد(!is_null_oid(&state->orig_commit));
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
ایستا صحیح safe_to_abort(ثابت ساختار am_state *state)
{
	ساختار strbuf sb = STRBUF_INIT;
	ساختار object_id abort_safety, head;

	اگر (file_exists(am_path(state, "dirtyindex")))
		بازگشت 0;

	اگر (read_state_file(&sb, state, "abort-safety", 1) > 0) {
		اگر (get_oid_hex(sb.buf, &abort_safety))
			die(_("could not parse %s"), am_path(state, "abort-safety"));
	} وگرنه
		oidclr(&abort_safety, the_repository->hash_algo);
	strbuf_release(&sb);

	اگر (repo_get_oid(the_repository, "HEAD", &head))
		oidclr(&head, the_repository->hash_algo);

	اگر (oideq(&head, &abort_safety))
		بازگشت 1;

	warning(_("You seem to have moved HEAD since the last 'am' failure.\n"
		"Not rewinding to ORIG_HEAD"));

	بازگشت 0;
}

/**
 * Aborts the current am session if it is safe to do so.
 */
ایستا تهی am_abort(ساختار am_state *state)
{
	ساختار object_id curr_head, orig_head;
	صحیح has_curr_head, has_orig_head;
	نویسه *curr_branch;

	اگر (!safe_to_abort(state)) {
		am_destroy(state);
		بازگشت;
	}

	am_rerere_clear();

	curr_branch = refs_resolve_refdup(get_main_ref_store(the_repository),
					  "HEAD", 0, &curr_head, پوچ);
	has_curr_head = curr_branch && !is_null_oid(&curr_head);
	اگر (!has_curr_head)
		oidcpy(&curr_head, the_hash_algo->empty_درخت);

	has_orig_head = !repo_get_oid(the_repository, "ORIG_HEAD", &orig_head);
	اگر (!has_orig_head)
		oidcpy(&orig_head, the_hash_algo->empty_درخت);

	اگر (clean_index(&curr_head, &orig_head))
		die(_("failed to clean index"));

	اگر (has_orig_head)
		refs_update_ref(get_main_ref_store(the_repository),
				"am --abort", "HEAD", &orig_head,
				has_curr_head ? &curr_head : پوچ, 0,
				UPDATE_REFS_DIE_ON_ERR);
	وگرنه اگر (curr_branch)
		refs_delete_ref(get_main_ref_store(the_repository), پوچ,
				curr_branch, پوچ, REF_NO_DEREF);

	آزاد(curr_branch);
	am_destroy(state);
}

ایستا صحیح show_patch(ساختار am_state *state, شمارش resume_type resume_mode)
{
	ساختار strbuf sb = STRBUF_INIT;
	ثابت نویسه *patch_path;
	صحیح len;

	اگر (!is_null_oid(&state->orig_commit)) {
		ساختار child_process cmd = CHILD_PROCESS_INIT;

		strvec_pushl(&cmd.args, "show", oid_to_hex(&state->orig_commit),
			     "--", پوچ);
		cmd.git_cmd = 1;
		بازگشت run_command(&cmd);
	}

	انتخاب (resume_mode) {
	حالت RESUME_SHOW_PATCH_RAW:
		patch_path = am_path(state, msgnum(state));
		بشکن;
	حالت RESUME_SHOW_PATCH_DIFF:
		patch_path = am_path(state, "patch");
		بشکن;
	پیشفرض:
		BUG("invalid mode for --show-current-patch");
	}

	len = strbuf_read_file(&sb, patch_path, 0);
	اگر (len < 0)
		die_errno(_("failed to read '%s'"), patch_path);

	setup_pager(the_repository);
	write_in_full(1, sb.buf, sb.len);
	strbuf_release(&sb);
	بازگشت 0;
}

/**
 * parse_options() callback that validates and sets opt->value to the
 * PATCH_FORMAT_* enum value corresponding to `arg`.
 */
ایستا صحیح parse_opt_patchformat(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	صحیح *opt_مقدار = opt->مقدار;

	اگر (unset)
		*opt_مقدار = PATCH_FORMAT_UNKNOWN;
	وگرنه اگر (!strcmp(arg, "mbox"))
		*opt_مقدار = PATCH_FORMAT_MBOX;
	وگرنه اگر (!strcmp(arg, "stgit"))
		*opt_مقدار = PATCH_FORMAT_STGIT;
	وگرنه اگر (!strcmp(arg, "stgit-series"))
		*opt_مقدار = PATCH_FORMAT_STGIT_SERIES;
	وگرنه اگر (!strcmp(arg, "hg"))
		*opt_مقدار = PATCH_FORMAT_HG;
	وگرنه اگر (!strcmp(arg, "mboxrd"))
		*opt_مقدار = PATCH_FORMAT_MBOXRD;
	/*
	 * Please update $__git_patchformat in git-completion.bash
	 * when you add new options
	 */
	وگرنه
		بازگشت error(_("invalid value for '%s': '%s'"),
			     "--patch-format", arg);
	بازگشت 0;
}

ایستا صحیح parse_opt_show_current_patch(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	صحیح *opt_مقدار = opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	اگر (!arg)
		*opt_مقدار = opt->defval;
	وگرنه اگر (!strcmp(arg, "raw"))
		*opt_مقدار = RESUME_SHOW_PATCH_RAW;
	وگرنه اگر (!strcmp(arg, "diff"))
		*opt_مقدار = RESUME_SHOW_PATCH_DIFF;
	/*
	 * Please update $__git_showcurrentpatch in git-completion.bash
	 * when you add new options
	 */
	وگرنه
		بازگشت error(_("invalid value for '%s': '%s'"),
			     "--show-current-patch", arg);
	بازگشت 0;
}

صحیح cmd_am(صحیح argc,
	   ثابت نویسه **argv,
	   ثابت نویسه *prefix,
	   ساختار repository *repo UNUSED)
{
	ساختار am_state state;
	صحیح دودویی = -1;
	صحیح keep_cr = -1;
	صحیح patch_format = PATCH_FORMAT_UNKNOWN;
	شمارش resume_type resume_mode = RESUME_FALSE;
	صحیح in_progress;
	صحیح ret = 0;

	ثابت نویسه * ثابت usage[] = {
		N_("git am [<options>] [(<mbox> | <Maildir>)...]"),
		N_("git am [<options>] (--continue | --skip | --abort)"),
		پوچ
	};

	ساختار option options[] = {
		OPT_BOOL('i', "interactive", &state.interactive,
			N_("run interactively")),
		OPT_BOOL('n', "no-verify", &state.no_verify,
			N_("bypass pre-applypatch and applypatch-msg hooks")),
		OPT_HIDDEN_BOOL('b', "binary", &دودویی,
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
		OPT_PASSTHRU_ARGV(0, "ignore-space-change", &state.git_apply_opts, پوچ,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU_ARGV(0, "ignore-whitespace", &state.git_apply_opts, پوچ,
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
		OPT_PASSTHRU_ARGV('C', پوچ, &state.git_apply_opts, N_("n"),
			N_("pass it through git-apply"),
			0),
		OPT_PASSTHRU_ARGV('p', پوچ, &state.git_apply_opts, N_("num"),
			N_("pass it through git-apply"),
			0),
		OPT_CALLBACK(0, "patch-format", &patch_format, N_("format"),
			N_("format the patch(es) are in"),
			parse_opt_patchformat),
		OPT_PASSTHRU_ARGV(0, "reject", &state.git_apply_opts, پوچ,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_STRING(0, "resolvemsg", &state.resolvemsg, پوچ,
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
			.مقدار = &resume_mode,
			.precision = اندازه(resume_mode),
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
			.مقدار = &state.sign_commit,
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

	repo_config(the_repository, git_default_config, پوچ);

	am_state_init(&state);

	in_progress = am_in_progress(&state);
	اگر (in_progress)
		am_load(&state);

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	اگر (دودویی >= 0)
		fprintf_ln(stderr, _("The -b/--binary option has been a no-op for long time, and\n"
				"it will be removed. Please do not use it anymore."));

	/* Ensure a valid committer ident can be constructed */
	git_committer_info(IDENT_STRICT);

	اگر (repo_read_index_preload(the_repository, پوچ, 0) < 0)
		die(_("failed to read the index"));

	اگر (in_progress) {
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
		اگر (argc || (resume_mode == RESUME_FALSE && !isatty(0)))
			die(_("previous rebase directory %s still exists but mbox given."),
				state.dir);

		اگر (resume_mode == RESUME_FALSE)
			resume_mode = RESUME_APPLY;

		اگر (state.signoff == SIGNOFF_EXPLICIT)
			am_append_signoff(&state);
	} وگرنه {
		ساختار strvec paths = STRVEC_INIT;
		صحیح i;

		/*
		 * Handle stray state directory in the independent-run case. In
		 * the --rebasing case, it is up to the caller to take care of
		 * stray directories.
		 */
		اگر (file_exists(state.dir) && !state.rebasing) {
			اگر (resume_mode == RESUME_ABORT || resume_mode == RESUME_QUIT) {
				am_destroy(&state);
				am_state_release(&state);
				بازگشت 0;
			}

			die(_("Stray %s directory found.\n"
				"Use \"git am --abort\" to remove it."),
				state.dir);
		}

		اگر (resume_mode)
			die(_("Resolve operation not in progress, we are not resuming."));

		برای (i = 0; i < argc; i++) {
			اگر (is_absolute_path(argv[i]) || !prefix)
				strvec_افزودن(&paths, argv[i]);
			وگرنه
				strvec_افزودن(&paths, mkpath("%s/%s", prefix, argv[i]));
		}

		اگر (state.interactive && !paths.nr)
			die(_("interactive mode requires patches on the command line"));

		am_setup(&state, patch_format, paths.v, keep_cr);

		strvec_clear(&paths);
	}

	انتخاب (resume_mode) {
	حالت RESUME_FALSE:
		am_run(&state, 0);
		بشکن;
	حالت RESUME_APPLY:
		am_run(&state, 1);
		بشکن;
	حالت RESUME_RESOLVED:
	حالت RESUME_ALLOW_EMPTY:
		am_resolve(&state, resume_mode == RESUME_ALLOW_EMPTY ? 1 : 0);
		بشکن;
	حالت RESUME_SKIP:
		am_skip(&state);
		بشکن;
	حالت RESUME_ABORT:
		am_abort(&state);
		بشکن;
	حالت RESUME_QUIT:
		am_rerere_clear();
		am_destroy(&state);
		بشکن;
	حالت RESUME_SHOW_PATCH_RAW:
	حالت RESUME_SHOW_PATCH_DIFF:
		ret = show_patch(&state, resume_mode);
		بشکن;
	پیشفرض:
		BUG("invalid resume value");
	}

	am_state_release(&state);

	بازگشت ret;
}
