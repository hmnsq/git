/*
 * Builtin "git am"
 *
 * Based on git-am.sh by Junio C Hamano.
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

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
ساکن صحیح linelen(ثابت حرف *پیغام)
{
	واپس strchrnul(پیغام, '\n') - پیغام;
}

/**
 * Returns true if `str` consists of only whitespace, false otherwise.
 */
ساکن صحیح str_isspace(ثابت حرف *str)
{
	برائے (; *str; str++)
		اگر (!خلا_ہے(*str))
			واپس 0;

	واپس 1;
}

شمار patch_format {
	PATCH_FORMAT_UNKNOWN = 0,
	PATCH_FORMAT_MBOX,
	PATCH_FORMAT_STGIT,
	PATCH_FORMAT_STGIT_SERIES,
	PATCH_FORMAT_HG,
	PATCH_FORMAT_MBOXRD
};

شمار keep_type {
	KEEP_FALSE = 0,
	KEEP_TRUE,      /* pass -k flag to git-mailinfo */
	KEEP_NON_PATCH  /* pass -b flag to git-mailinfo */
};

شمار scissors_type {
	SCISSORS_UNSET = -1,
	SCISSORS_FALSE = 0,  /* pass --no-scissors to git-mailinfo */
	SCISSORS_TRUE        /* pass --scissors to git-mailinfo */
};

شمار signoff_type {
	SIGNOFF_FALSE = 0,
	SIGNOFF_TRUE = 1,
	SIGNOFF_EXPLICIT /* --signoff was set on the command-line */
};

شمار resume_type {
	RESUME_FALSE = 0,
	RESUME_APPLY,
	RESUME_RESOLVED,
	RESUME_SKIP,
	RESUME_ABORT,
	RESUME_QUIT,
	RESUME_SHOW_PATCH_RAW,
	RESUME_SHOW_PATCH_DIFF,
	RESUME_ALLOW_خالی_خانے,
};

شمار خالی_خانے_action {
	STOP_ON_خالی_خانے_COMMIT = 0,  /* output errors and stop in the middle of an am session */
	DROP_خالی_خانے_COMMIT,         /* skip with a notice message, unless "--quiet" has been passed */
	KEEP_خالی_خانے_COMMIT,         /* keep recording as empty commits */
};

ساخت am_state {
	/* state directory path */
	حرف *dir;

	/* current and last patch numbers, 1-indexed */
	صحیح cur;
	صحیح آخری;

	/* commit metadata and message */
	حرف *author_name;
	حرف *author_email;
	حرف *author_date;
	حرف *پیغام;
	سائز_t پیغام_len;

	/* when --rebasing, records the original commit the patch came from */
	ساخت object_id orig_commit;

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
	صحیح پیغام_id;
	صحیح scissors; /* enum scissors_type */
	صحیح quoted_cr; /* enum quoted_cr_action */
	صحیح خالی_خانے_type; /* enum empty_action */
	ساخت strvec git_apply_opts;
	ثابت حرف *resolvemsg;
	صحیح committer_date_is_author_date;
	صحیح ignore_date;
	صحیح allow_rerere_autoupdate;
	ثابت حرف *sign_commit;
	صحیح rebasing;
};

/**
 * Initializes am_state with the default values.
 */
ساکن خالی am_state_init(ساخت am_state *state)
{
	صحیح gpgsign;

	memset(state, 0, حجم(*state));

	state->dir = repo_git_راستہ(the_repository, "rebase-apply");

	state->prec = 4;

	repo_config_get_bool(the_repository, "am.threeway", &state->threeway);

	state->utf8 = 1;

	repo_config_get_bool(the_repository, "am.messageid", &state->پیغام_id);

	state->scissors = SCISSORS_UNSET;
	state->quoted_cr = quoted_cr_unset;

	strvec_init(&state->git_apply_opts);

	اگر (!repo_config_get_bool(the_repository, "commit.gpgsign", &gpgsign))
		state->sign_commit = gpgsign ? "" : تہی;
}

/**
 * Releases memory allocated by an am_state.
 */
ساکن خالی am_state_release(ساخت am_state *state)
{
	آزاد(state->dir);
	آزاد(state->author_name);
	آزاد(state->author_email);
	آزاد(state->author_date);
	آزاد(state->پیغام);
	strvec_clear(&state->git_apply_opts);
}

ساکن صحیح am_option_parse_quoted_cr(ثابت ساخت option *opt,
				     ثابت حرف *arg, صحیح unset)
{
	BUG_ON_OPT_NEG(unset);

	اگر (mailinfo_parse_quoted_cr_action(arg, opt->قدر) != 0)
		واپس error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	واپس 0;
}

ساکن صحیح am_option_parse_خالی_خانے(ثابت ساخت option *opt,
				     ثابت حرف *arg, صحیح unset)
{
	صحیح *opt_قدر = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	اگر (!strcmp(arg, "stop"))
		*opt_قدر = STOP_ON_خالی_خانے_COMMIT;
	ورنہ اگر (!strcmp(arg, "drop"))
		*opt_قدر = DROP_خالی_خانے_COMMIT;
	ورنہ اگر (!strcmp(arg, "keep"))
		*opt_قدر = KEEP_خالی_خانے_COMMIT;
	ورنہ
		واپس error(_("invalid value for '%s': '%s'"), "--empty", arg);

	واپس 0;
}

/**
 * Returns path relative to the am_state directory.
 */
ساکن اندرونی ثابت حرف *am_راستہ(ثابت ساخت am_state *state, ثابت حرف *راستہ)
{
	واپس mkpath("%s/%s", state->dir, راستہ);
}

/**
 * For convenience to call write_file()
 */
ساکن خالی write_state_متن(ثابت ساخت am_state *state,
			     ثابت حرف *name, ثابت حرف *string)
{
	write_file(am_راستہ(state, name), "%s", string);
}

ساکن خالی write_state_گنتی(ثابت ساخت am_state *state,
			      ثابت حرف *name, صحیح قدر)
{
	write_file(am_راستہ(state, name), "%d", قدر);
}

ساکن خالی write_state_bool(ثابت ساخت am_state *state,
			     ثابت حرف *name, صحیح قدر)
{
	write_state_متن(state, name, قدر ? "t" : "f");
}

/**
 * If state->quiet is false, calls fprintf(fp, fmt, ...), and appends a newline
 * at the end.
 */
__attribute__((format (لکھو, 3, 4)))
ساکن خالی say(ثابت ساخت am_state *state, FILE *fp, ثابت حرف *fmt, ...)
{
	va_list ap;

	va_آغاز(ap, fmt);
	اگر (!state->quiet) {
		vfprintf(fp, fmt, ap);
		putc('\n', fp);
	}
	va_انتہا(ap);
}

/**
 * Returns 1 if there is an am session in progress, 0 otherwise.
 */
ساکن صحیح am_اندر_پیش_رفت(ثابت ساخت am_state *state)
{
	ساخت stat st;

	اگر (lstat(state->dir, &st) < 0 || !S_ISDIR(st.st_منوال))
		واپس 0;
	اگر (lstat(am_راستہ(state, "last"), &st) || !S_ISREG(st.st_منوال))
		واپس 0;
	اگر (lstat(am_راستہ(state, "next"), &st) || !S_ISREG(st.st_منوال))
		واپس 0;
	واپس 1;
}

/**
 * Reads the contents of `file` in the `state` directory into `sb`. Returns the
 * number of bytes read on success, -1 if the file does not exist. If `trim` is
 * set, trailing whitespace will be removed.
 */
ساکن صحیح read_state_file(ساخت strbuf *sb, ثابت ساخت am_state *state,
			ثابت حرف *file, صحیح trim)
{
	strbuf_reset(sb);

	اگر (strbuf_read_file(sb, am_راستہ(state, file), 0) >= 0) {
		اگر (trim)
			strbuf_trim(sb);

		واپس sb->len;
	}

	اگر (errno == ENOENT)
		واپس -1;

	die_errno(_("could not read '%s'"), am_راستہ(state, file));
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
ساکن صحیح read_am_author_script(ساخت am_state *state)
{
	ثابت حرف *filename = am_راستہ(state, "author-script");

	أكد(!state->author_name);
	أكد(!state->author_email);
	أكد(!state->author_date);

	واپس read_author_script(filename, &state->author_name,
				  &state->author_email, &state->author_date, 1);
}

/**
 * Saves state->author_name, state->author_email and state->author_date in the
 * state directory's "author-script" file.
 */
ساکن خالی write_author_script(ثابت ساخت am_state *state)
{
	ساخت strbuf sb = STRBUF_INIT;

	strbuf_addstr(&sb, "GIT_AUTHOR_NAME=");
	sq_quote_buf(&sb, state->author_name);
	strbuf_addch(&sb, '\n');

	strbuf_addstr(&sb, "GIT_AUTHOR_EMAIL=");
	sq_quote_buf(&sb, state->author_email);
	strbuf_addch(&sb, '\n');

	strbuf_addstr(&sb, "GIT_AUTHOR_DATE=");
	sq_quote_buf(&sb, state->author_date);
	strbuf_addch(&sb, '\n');

	write_state_متن(state, "author-script", sb.buf);

	strbuf_release(&sb);
}

/**
 * Reads the commit message from the state directory's "final-commit" file,
 * setting state->msg to its contents and state->msg_len to the length of its
 * contents in bytes.
 *
 * Returns 0 on success, -1 if the file does not exist.
 */
ساکن صحیح read_commit_پیغام(ساخت am_state *state)
{
	ساخت strbuf sb = STRBUF_INIT;

	أكد(!state->پیغام);

	اگر (read_state_file(&sb, state, "final-commit", 0) < 0) {
		strbuf_release(&sb);
		واپس -1;
	}

	state->پیغام = strbuf_detach(&sb, &state->پیغام_len);
	واپس 0;
}

/**
 * Saves state->msg in the state directory's "final-commit" file.
 */
ساکن خالی write_commit_پیغام(ثابت ساخت am_state *state)
{
	ثابت حرف *filename = am_راستہ(state, "final-commit");
	write_file_buf(filename, state->پیغام, state->پیغام_len);
}

/**
 * Loads state from disk.
 */
ساکن خالی am_بھرو(ساخت am_state *state)
{
	ساخت strbuf sb = STRBUF_INIT;

	اگر (read_state_file(&sb, state, "next", 1) < 0)
		BUG("state file 'next' does not exist");
	state->cur = strtol(sb.buf, تہی, 10);

	اگر (read_state_file(&sb, state, "last", 1) < 0)
		BUG("state file 'last' does not exist");
	state->آخری = strtol(sb.buf, تہی, 10);

	اگر (read_am_author_script(state) < 0)
		die(_("could not parse author script"));

	read_commit_پیغام(state);

	اگر (read_state_file(&sb, state, "original-commit", 1) < 0)
		oidclr(&state->orig_commit, the_repository->hash_algo);
	ورنہ اگر (get_oid_hex(sb.buf, &state->orig_commit) < 0)
		die(_("could not parse %s"), am_راستہ(state, "original-commit"));

	read_state_file(&sb, state, "threeway", 1);
	state->threeway = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "quiet", 1);
	state->quiet = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "sign", 1);
	state->signoff = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "utf8", 1);
	state->utf8 = !strcmp(sb.buf, "t");

	اگر (file_exists(am_راستہ(state, "rerere-autoupdate"))) {
		read_state_file(&sb, state, "rerere-autoupdate", 1);
		state->allow_rerere_autoupdate = strcmp(sb.buf, "t") ?
			RERERE_NOAUTOUPDATE : RERERE_AUTOUPDATE;
	} ورنہ {
		state->allow_rerere_autoupdate = 0;
	}

	read_state_file(&sb, state, "keep", 1);
	اگر (!strcmp(sb.buf, "t"))
		state->keep = KEEP_TRUE;
	ورنہ اگر (!strcmp(sb.buf, "b"))
		state->keep = KEEP_NON_PATCH;
	ورنہ
		state->keep = KEEP_FALSE;

	read_state_file(&sb, state, "messageid", 1);
	state->پیغام_id = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "scissors", 1);
	اگر (!strcmp(sb.buf, "t"))
		state->scissors = SCISSORS_TRUE;
	ورنہ اگر (!strcmp(sb.buf, "f"))
		state->scissors = SCISSORS_FALSE;
	ورنہ
		state->scissors = SCISSORS_UNSET;

	read_state_file(&sb, state, "quoted-cr", 1);
	اگر (!*sb.buf)
		state->quoted_cr = quoted_cr_unset;
	ورنہ اگر (mailinfo_parse_quoted_cr_action(sb.buf, &state->quoted_cr) != 0)
		die(_("could not parse %s"), am_راستہ(state, "quoted-cr"));

	read_state_file(&sb, state, "apply-opt", 1);
	strvec_clear(&state->git_apply_opts);
	اگر (sq_dequote_مقصد_strvec(sb.buf, &state->git_apply_opts) < 0)
		die(_("could not parse %s"), am_راستہ(state, "apply-opt"));

	state->rebasing = !!file_exists(am_راستہ(state, "rebasing"));

	strbuf_release(&sb);
}

/**
 * Removes the am_state directory, forcefully terminating the current am
 * session.
 */
ساکن خالی am_destroy(ثابت ساخت am_state *state)
{
	ساخت strbuf sb = STRBUF_INIT;

	strbuf_addstr(&sb, state->dir);
	remove_dir_recursively(&sb, 0);
	strbuf_release(&sb);
}

/**
 * Runs applypatch-msg hook. Returns its exit code.
 */
ساکن صحیح تسلسل_applypatch_پیغام_hook(ساخت am_state *state)
{
	صحیح ret = 0;

	أكد(state->پیغام);

	اگر (!state->no_verify) {
		ساخت تسلسل_hooks_opt opt = تسلسل_HOOKS_OPT_INIT_FORCE_SERIAL;
		strvec_ڈالو(&opt.دلائل, am_راستہ(state, "final-commit"));
		ret = تسلسل_hooks_opt(the_repository, "applypatch-msg", &opt);
	}

	اگر (!ret) {
		FREE_AND_NULL(state->پیغام);
		اگر (read_commit_پیغام(state) < 0)
			die(_("'%s' was deleted by the applypatch-msg hook"),
				am_راستہ(state, "final-commit"));
	}

	واپس ret;
}

/**
 * Runs post-rewrite hook. Returns it exit code.
 */
ساکن صحیح تسلسل_post_rewrite_hook(ثابت ساخت am_state *state)
{
	ساخت تسلسل_hooks_opt opt = تسلسل_HOOKS_OPT_INIT;

	strvec_ڈالو(&opt.دلائل, "rebase");
	opt.راستہ_مقصد_stdin = am_راستہ(state, "rewritten");

	واپس تسلسل_hooks_opt(the_repository, "post-rewrite", &opt);
}

/**
 * Reads the state directory's "rewritten" file, and copies notes from the old
 * commits listed in the file to their rewritten commits.
 *
 * Returns 0 on success, -1 on failure.
 */
ساکن صحیح copy_notes_for_rebase(ثابت ساخت am_state *state)
{
	ساخت notes_rewrite_cfg *c;
	ساخت strbuf sb = STRBUF_INIT;
	ثابت حرف *invalid_سطر = _("Malformed input line: '%s'.");
	ثابت حرف *پیغام = "Notes added by 'git rebase'";
	FILE *fp;
	صحیح ret = 0;

	أكد(state->rebasing);

	c = init_copy_notes_for_rewrite("rebase");
	اگر (!c)
		واپس 0;

	fp = xfopen(am_راستہ(state, "rewritten"), "r");

	جب (!strbuf_getline_lf(&sb, fp)) {
		ساخت object_id from_obj, مقصد_obj;
		ثابت حرف *p;

		اگر (sb.len != the_hash_algo->hexsz * 2 + 1) {
			ret = error(invalid_سطر, sb.buf);
			جاؤ اختتام;
		}

		اگر (parse_oid_hex(sb.buf, &from_obj, &p)) {
			ret = error(invalid_سطر, sb.buf);
			جاؤ اختتام;
		}

		اگر (*p != ' ') {
			ret = error(invalid_سطر, sb.buf);
			جاؤ اختتام;
		}

		اگر (get_oid_hex(p + 1, &مقصد_obj)) {
			ret = error(invalid_سطر, sb.buf);
			جاؤ اختتام;
		}

		اگر (copy_note_for_rewrite(c, &from_obj, &مقصد_obj))
			ret = error(_("Failed to copy notes from '%s' to '%s'"),
					oid_مقصد_hex(&from_obj), oid_مقصد_hex(&مقصد_obj));
	}

اختتام:
	اختتام_copy_notes_for_rewrite(the_repository, c, پیغام);
	fclose(fp);
	strbuf_release(&sb);
	واپس ret;
}

/**
 * Determines if the file looks like a piece of RFC2822 mail by grabbing all
 * non-indented lines and checking if they look like they begin with valid
 * header field names.
 *
 * Returns 1 if the file looks like a piece of mail, 0 otherwise.
 */
ساکن صحیح is_mail(FILE *fp)
{
	ثابت حرف *سرخی_regex = "^[!-9;-~]+:";
	ساخت strbuf sb = STRBUF_INIT;
	regex_t regex;
	صحیح ret = 1;

	اگر (fseek(fp, 0L, SEEK_سیٹ))
		die_errno(_("fseek failed"));

	اگر (regcomp(&regex, سرخی_regex, REG_NOSUB | REG_EXTENDED))
		die("invalid pattern: %s", سرخی_regex);

	جب (!strbuf_getline(&sb, fp)) {
		اگر (!sb.len)
			توڑو; /* End of header */

		/* Ignore indented folded lines */
		اگر (*sb.buf == '\t' || *sb.buf == ' ')
			جاری;

		/* It's a header if it matches header_regex */
		اگر (regexec(&regex, sb.buf, 0, تہی, 0)) {
			ret = 0;
			جاؤ مکمل;
		}
	}

مکمل:
	regfree(&regex);
	strbuf_release(&sb);
	واپس ret;
}

/**
 * Attempts to detect the patch_format of the patches contained in `paths`,
 * returning the PATCH_FORMAT_* enum value. Returns PATCH_FORMAT_UNKNOWN if
 * detection fails.
 */
ساکن صحیح detect_patch_format(ثابت حرف **paths)
{
	شمار patch_format ret = PATCH_FORMAT_UNKNOWN;
	ساخت strbuf l1 = STRBUF_INIT;
	ساخت strbuf l2 = STRBUF_INIT;
	ساخت strbuf l3 = STRBUF_INIT;
	FILE *fp;

	/*
	 * We default to mbox format if input is from stdin and for directories
	 */
	اگر (!*paths || !strcmp(*paths, "-") || is_directory(*paths))
		واپس PATCH_FORMAT_MBOX;

	/*
	 * Otherwise, check the first few lines of the first patch, starting
	 * from the first non-blank line, to try to detect its format.
	 */

	fp = xfopen(*paths, "r");

	جب (!strbuf_getline(&l1, fp)) {
		اگر (l1.len)
			توڑو;
	}

	اگر (starts_with(l1.buf, "From ") || starts_with(l1.buf, "From: ")) {
		ret = PATCH_FORMAT_MBOX;
		جاؤ مکمل;
	}

	اگر (starts_with(l1.buf, "# This series applies on GIT commit")) {
		ret = PATCH_FORMAT_STGIT_SERIES;
		جاؤ مکمل;
	}

	اگر (!strcmp(l1.buf, "# HG changeset patch")) {
		ret = PATCH_FORMAT_HG;
		جاؤ مکمل;
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
		جاؤ مکمل;
	}

	اگر (l1.len && is_mail(fp)) {
		ret = PATCH_FORMAT_MBOX;
		جاؤ مکمل;
	}

مکمل:
	fclose(fp);
	strbuf_release(&l1);
	strbuf_release(&l2);
	strbuf_release(&l3);
	واپس ret;
}

/**
 * Splits out individual email patches from `paths`, where each path is either
 * a mbox file or a Maildir. Returns 0 on success, -1 on failure.
 */
ساکن صحیح بانٹو_mail_mbox(ساخت am_state *state, ثابت حرف **paths,
				صحیح keep_cr, صحیح mboxrd)
{
	ساخت بچہ_process cp = بچہ_PROCESS_INIT;
	ساخت strbuf آخری = STRBUF_INIT;
	صحیح ret;

	cp.git_cmd = 1;
	strvec_ڈالو(&cp.دلائل, "mailsplit");
	strvec_pushf(&cp.دلائل, "-d%d", state->prec);
	strvec_pushf(&cp.دلائل, "-o%s", state->dir);
	strvec_ڈالو(&cp.دلائل, "-b");
	اگر (keep_cr)
		strvec_ڈالو(&cp.دلائل, "--keep-cr");
	اگر (mboxrd)
		strvec_ڈالو(&cp.دلائل, "--mboxrd");
	strvec_ڈالو(&cp.دلائل, "--");
	strvec_pushv(&cp.دلائل, paths);

	ret = capture_command(&cp, &آخری, 8);
	اگر (ret)
		جاؤ باہر_نکلو;

	state->cur = 1;
	state->آخری = strtol(آخری.buf, تہی, 10);

باہر_نکلو:
	strbuf_release(&آخری);
	واپس ret ? -1 : 0;
}

/**
 * Callback signature for split_mail_conv(). The foreign patch should be
 * read from `in`, and the converted patch (in RFC2822 mail format) should be
 * written to `out`. Return 0 on success, or -1 on failure.
 */
قسمتعریف صحیح (*mail_conv_fn)(FILE *برآمد, FILE *اندر, صحیح keep_cr);

/**
 * Calls `fn` for each file in `paths` to convert the foreign patch to the
 * RFC2822 mail format suitable for parsing with git-mailinfo.
 *
 * Returns 0 on success, -1 on failure.
 */
ساکن صحیح بانٹو_mail_conv(mail_conv_fn fn, ساخت am_state *state,
			ثابت حرف **paths, صحیح keep_cr)
{
	ساکن ثابت حرف *stdin_only[] = {"-", تہی};
	صحیح i;

	اگر (!*paths)
		paths = stdin_only;

	برائے (i = 0; *paths; paths++, i++) {
		FILE *اندر, *برآمد;
		ثابت حرف *mail;
		صحیح ret;

		اگر (!strcmp(*paths, "-"))
			اندر = stdin;
		ورنہ
			اندر = fopen(*paths, "r");

		اگر (!اندر)
			واپس error_errno(_("could not open '%s' for reading"),
					   *paths);

		mail = mkpath("%s/%0*d", state->dir, state->prec, i + 1);

		برآمد = fopen(mail, "w");
		اگر (!برآمد) {
			اگر (اندر != stdin)
				fclose(اندر);
			واپس error_errno(_("could not open '%s' for writing"),
					   mail);
		}

		ret = fn(برآمد, اندر, keep_cr);

		fclose(برآمد);
		اگر (اندر != stdin)
			fclose(اندر);

		اگر (ret)
			واپس error(_("could not parse patch '%s'"), *paths);
	}

	state->cur = 1;
	state->آخری = i;
	واپس 0;
}

/**
 * A split_mail_conv() callback that converts an StGit patch to an RFC2822
 * message suitable for parsing with git-mailinfo.
 */
ساکن صحیح stgit_patch_مقصد_mail(FILE *برآمد, FILE *اندر, صحیح keep_cr غیر_مستعمل)
{
	ساخت strbuf sb = STRBUF_INIT;
	صحیح subject_printed = 0;

	جب (!strbuf_getline_lf(&sb, اندر)) {
		ثابت حرف *str;

		اگر (str_isspace(sb.buf))
			جاری;
		ورنہ اگر (skip_سابقہ(sb.buf, "Author:", &str))
			fprintf(برآمد, "From:%s\n", str);
		ورنہ اگر (starts_with(sb.buf, "From") || starts_with(sb.buf, "Date"))
			fprintf(برآمد, "%s\n", sb.buf);
		ورنہ اگر (!subject_printed) {
			fprintf(برآمد, "Subject: %s\n", sb.buf);
			subject_printed = 1;
		} ورنہ {
			fprintf(برآمد, "\n%s\n", sb.buf);
			توڑو;
		}
	}

	strbuf_reset(&sb);
	جب (strbuf_fread(&sb, 8192, اندر) > 0) {
		fwrite(sb.buf, 1, sb.len, برآمد);
		strbuf_reset(&sb);
	}

	strbuf_release(&sb);
	واپس 0;
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
ساکن صحیح بانٹو_mail_stgit_series(ساخت am_state *state, ثابت حرف **paths,
					صحیح keep_cr)
{
	ثابت حرف *series_dir;
	حرف *series_dir_buf;
	FILE *fp;
	ساخت strvec patches = STRVEC_INIT;
	ساخت strbuf sb = STRBUF_INIT;
	صحیح ret;

	اگر (!paths[0] || paths[1])
		واپس error(_("Only one StGIT patch series can be applied at once"));

	series_dir_buf = xstrdup(*paths);
	series_dir = dirname(series_dir_buf);

	fp = fopen(*paths, "r");
	اگر (!fp) {
		آزاد(series_dir_buf);
		واپس error_errno(_("could not open '%s' for reading"), *paths);
	}

	جب (!strbuf_getline_lf(&sb, fp)) {
		اگر (*sb.buf == '#')
			جاری; /* skip comment lines */

		strvec_ڈالو(&patches, mkpath("%s/%s", series_dir, sb.buf));
	}

	fclose(fp);
	strbuf_release(&sb);
	آزاد(series_dir_buf);

	ret = بانٹو_mail_conv(stgit_patch_مقصد_mail, state, patches.v, keep_cr);

	strvec_clear(&patches);
	واپس ret;
}

/**
 * A split_patches_conv() callback that converts a mercurial patch to a RFC2822
 * message suitable for parsing with git-mailinfo.
 */
ساکن صحیح hg_patch_مقصد_mail(FILE *برآمد, FILE *اندر, صحیح keep_cr غیر_مستعمل)
{
	ساخت strbuf sb = STRBUF_INIT;
	صحیح rc = 0;

	جب (!strbuf_getline_lf(&sb, اندر)) {
		ثابت حرف *str;

		اگر (skip_سابقہ(sb.buf, "# User ", &str))
			fprintf(برآمد, "From: %s\n", str);
		ورنہ اگر (skip_سابقہ(sb.buf, "# Date ", &str)) {
			timestamp_t timestamp;
			طویل tz, tz2;
			حرف *انتہا;

			errno = 0;
			timestamp = parse_timestamp(str, &انتہا, 10);
			اگر (errno) {
				rc = error(_("invalid timestamp"));
				جاؤ باہر_نکلو;
			}

			اگر (!skip_سابقہ(انتہا, " ", &str)) {
				rc = error(_("invalid Date line"));
				جاؤ باہر_نکلو;
			}

			errno = 0;
			tz = strtol(str, &انتہا, 10);
			اگر (errno) {
				rc = error(_("invalid timezone offset"));
				جاؤ باہر_نکلو;
			}

			اگر (*انتہا) {
				rc = error(_("invalid Date line"));
				جاؤ باہر_نکلو;
			}

			/*
			 * mercurial's timezone is in seconds west of UTC,
			 * however git's timezone is in hours + minutes east of
			 * UTC. Convert it.
			 */
			tz2 = labs(tz) / 3600 * 100 + labs(tz) % 3600 / 60;
			اگر (tz > 0)
				tz2 = -tz2;

			fprintf(برآمد, "Date: %s\n", show_date(timestamp, tz2, DATE_منوال(RFC2822)));
		} ورنہ اگر (starts_with(sb.buf, "# ")) {
			جاری;
		} ورنہ {
			fprintf(برآمد, "\n%s\n", sb.buf);
			توڑو;
		}
	}

	strbuf_reset(&sb);
	جب (strbuf_fread(&sb, 8192, اندر) > 0) {
		fwrite(sb.buf, 1, sb.len, برآمد);
		strbuf_reset(&sb);
	}
باہر_نکلو:
	strbuf_release(&sb);
	واپس rc;
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
ساکن صحیح بانٹو_mail(ساخت am_state *state, شمار patch_format patch_format,
			ثابت حرف **paths, صحیح keep_cr)
{
	اگر (keep_cr < 0) {
		keep_cr = 0;
		repo_config_get_bool(the_repository, "am.keepcr", &keep_cr);
	}

	انتخاب (patch_format) {
	حالت PATCH_FORMAT_MBOX:
		واپس بانٹو_mail_mbox(state, paths, keep_cr, 0);
	حالت PATCH_FORMAT_STGIT:
		واپس بانٹو_mail_conv(stgit_patch_مقصد_mail, state, paths, keep_cr);
	حالت PATCH_FORMAT_STGIT_SERIES:
		واپس بانٹو_mail_stgit_series(state, paths, keep_cr);
	حالت PATCH_FORMAT_HG:
		واپس بانٹو_mail_conv(hg_patch_مقصد_mail, state, paths, keep_cr);
	حالت PATCH_FORMAT_MBOXRD:
		واپس بانٹو_mail_mbox(state, paths, keep_cr, 1);
	طےشدہ:
		BUG("invalid patch_format");
	}
	واپس -1;
}

/**
 * Setup a new am session for applying patches
 */
ساکن خالی am_تیاری(ساخت am_state *state, شمار patch_format patch_format,
			ثابت حرف **paths, صحیح keep_cr)
{
	ساخت object_id curr_سر;
	ثابت حرف *str;
	ساخت strbuf sb = STRBUF_INIT;

	اگر (!patch_format)
		patch_format = detect_patch_format(paths);

	اگر (!patch_format) {
		fprintf_ln(stderr, _("Patch format detection failed."));
		die(تہی);
	}

	اگر (mkdir(state->dir, 0777) < 0 && errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), state->dir);
	refs_delete_ref(get_main_ref_store(the_repository), تہی,
			"REBASE_HEAD", تہی, REF_NO_DEREF);

	اگر (بانٹو_mail(state, patch_format, paths, keep_cr) < 0) {
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
		توڑو;
	حالت KEEP_TRUE:
		str = "t";
		توڑو;
	حالت KEEP_NON_PATCH:
		str = "b";
		توڑو;
	طےشدہ:
		BUG("invalid value for state->keep");
	}

	write_state_متن(state, "keep", str);
	write_state_bool(state, "messageid", state->پیغام_id);

	انتخاب (state->scissors) {
	حالت SCISSORS_UNSET:
		str = "";
		توڑو;
	حالت SCISSORS_FALSE:
		str = "f";
		توڑو;
	حالت SCISSORS_TRUE:
		str = "t";
		توڑو;
	طےشدہ:
		BUG("invalid value for state->scissors");
	}
	write_state_متن(state, "scissors", str);

	انتخاب (state->quoted_cr) {
	حالت quoted_cr_unset:
		str = "";
		توڑو;
	حالت quoted_cr_nowarn:
		str = "nowarn";
		توڑو;
	حالت quoted_cr_warn:
		str = "warn";
		توڑو;
	حالت quoted_cr_strip:
		str = "strip";
		توڑو;
	طےشدہ:
		BUG("invalid value for state->quoted_cr");
	}
	write_state_متن(state, "quoted-cr", str);

	sq_quote_argv(&sb, state->git_apply_opts.v);
	write_state_متن(state, "apply-opt", sb.buf);

	اگر (state->rebasing)
		write_state_متن(state, "rebasing", "");
	ورنہ
		write_state_متن(state, "applying", "");

	اگر (!repo_get_oid(the_repository, "HEAD", &curr_سر)) {
		write_state_متن(state, "abort-safety", oid_مقصد_hex(&curr_سر));
		اگر (!state->rebasing)
			refs_update_ref(get_main_ref_store(the_repository),
					"am", "ORIG_HEAD", &curr_سر, تہی,
					0,
					UPDATE_REFS_DIE_ON_ERR);
	} ورنہ {
		write_state_متن(state, "abort-safety", "");
		اگر (!state->rebasing)
			refs_delete_ref(get_main_ref_store(the_repository),
					تہی, "ORIG_HEAD", تہی, 0);
	}

	/*
	 * NOTE: Since the "next" and "last" files determine if an am_state
	 * session is in progress, they should be written last.
	 */

	write_state_گنتی(state, "next", state->cur);
	write_state_گنتی(state, "last", state->آخری);

	strbuf_release(&sb);
}

/**
 * Increments the patch pointer, and cleans am_state for the application of the
 * next patch.
 */
ساکن خالی am_اگلا(ساخت am_state *state)
{
	ساخت object_id سر;

	FREE_AND_NULL(state->author_name);
	FREE_AND_NULL(state->author_email);
	FREE_AND_NULL(state->author_date);
	FREE_AND_NULL(state->پیغام);
	state->پیغام_len = 0;

	unlink(am_راستہ(state, "author-script"));
	unlink(am_راستہ(state, "final-commit"));

	oidclr(&state->orig_commit, the_repository->hash_algo);
	unlink(am_راستہ(state, "original-commit"));
	refs_delete_ref(get_main_ref_store(the_repository), تہی,
			"REBASE_HEAD", تہی, REF_NO_DEREF);

	اگر (!repo_get_oid(the_repository, "HEAD", &سر))
		write_state_متن(state, "abort-safety", oid_مقصد_hex(&سر));
	ورنہ
		write_state_متن(state, "abort-safety", "");

	state->cur++;
	write_state_گنتی(state, "next", state->cur);
}

/**
 * Returns the filename of the current patch email.
 */
ساکن ثابت حرف *msgnum(ثابت ساخت am_state *state)
{
	ساکن ساخت strbuf sb = STRBUF_INIT;

	strbuf_reset(&sb);
	strbuf_addf(&sb, "%0*d", state->prec, state->cur);

	واپس sb.buf;
}

/**
 * Dies with a user-friendly message on how to proceed after resolving the
 * problem. This message can be overridden with state->resolvemsg.
 */
ساکن خالی NORETURN die_user_resolve(ثابت ساخت am_state *state)
{
	اگر (state->resolvemsg) {
		advise_if_enabled(ADVICE_ملاؤ_CONFLICT, "%s", state->resolvemsg);
	} ورنہ {
		ثابت حرف *cmdline = state->interactive ? "git am -i" : "git am";
		ساخت strbuf sb = STRBUF_INIT;

		strbuf_addf(&sb, _("When you have resolved this problem, run \"%s --continue\".\n"), cmdline);
		strbuf_addf(&sb, _("If you prefer to skip this patch, run \"%s --skip\" instead.\n"), cmdline);

		اگر (advice_enabled(ADVICE_AM_WORK_DIR) &&
		    is_خالی_خانے_or_missing_file(am_راستہ(state, "patch")) &&
		    !repo_اشاریہ_has_changes(the_repository, تہی, تہی))
			strbuf_addf(&sb, _("To record the empty patch as an empty commit, run \"%s --allow-empty\".\n"), cmdline);

		strbuf_addf(&sb, _("To restore the original branch and stop patching, run \"%s --abort\"."), cmdline);

		advise_if_enabled(ADVICE_ملاؤ_CONFLICT, "%s", sb.buf);
		strbuf_release(&sb);
	}

	die(تہی);
}

/**
 * Appends signoff to the "msg" field of the am_state.
 */
ساکن خالی am_append_signoff(ساخت am_state *state)
{
	ساخت strbuf sb = STRBUF_INIT;

	strbuf_attach(&sb, state->پیغام, state->پیغام_len, state->پیغام_len + 1);
	append_signoff(&sb, 0, 0);
	state->پیغام = strbuf_detach(&sb, &state->پیغام_len);
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
ساکن صحیح parse_mail(ساخت am_state *state, ثابت حرف *mail)
{
	FILE *fp;
	ساخت strbuf sb = STRBUF_INIT;
	ساخت strbuf پیغام = STRBUF_INIT;
	ساخت strbuf author_name = STRBUF_INIT;
	ساخت strbuf author_date = STRBUF_INIT;
	ساخت strbuf author_email = STRBUF_INIT;
	صحیح ret = 0;
	ساخت mailinfo mi;

	تیاری_mailinfo(the_repository, &mi);

	اگر (state->utf8)
		mi.metainfo_charset = get_commit_output_encoding();
	ورنہ
		mi.metainfo_charset = تہی;

	انتخاب (state->keep) {
	حالت KEEP_FALSE:
		توڑو;
	حالت KEEP_TRUE:
		mi.keep_subject = 1;
		توڑو;
	حالت KEEP_NON_PATCH:
		mi.keep_non_patch_brackets_اندر_subject = 1;
		توڑو;
	طےشدہ:
		BUG("invalid value for state->keep");
	}

	اگر (state->پیغام_id)
		mi.جوڑو_پیغام_id = 1;

	انتخاب (state->scissors) {
	حالت SCISSORS_UNSET:
		توڑو;
	حالت SCISSORS_FALSE:
		mi.use_scissors = 0;
		توڑو;
	حالت SCISSORS_TRUE:
		mi.use_scissors = 1;
		توڑو;
	طےشدہ:
		BUG("invalid value for state->scissors");
	}

	انتخاب (state->quoted_cr) {
	حالت quoted_cr_unset:
		توڑو;
	حالت quoted_cr_nowarn:
	حالت quoted_cr_warn:
	حالت quoted_cr_strip:
		mi.quoted_cr = state->quoted_cr;
		توڑو;
	طےشدہ:
		BUG("invalid value for state->quoted_cr");
	}

	mi.داخلہ = xfopen(mail, "r");
	mi.output = xfopen(am_راستہ(state, "info"), "w");
	اگر (mailinfo(&mi, am_راستہ(state, "msg"), am_راستہ(state, "patch")))
		die("could not parse patch");

	fclose(mi.داخلہ);
	fclose(mi.output);

	اگر (mi.format_flowed)
		warning(_("Patch sent with format=flowed; "
			  "space at the end of lines might be lost."));

	/* Extract message and author information */
	fp = xfopen(am_راستہ(state, "info"), "r");
	جب (!strbuf_getline_lf(&sb, fp)) {
		ثابت حرف *x;

		اگر (skip_سابقہ(sb.buf, "Subject: ", &x)) {
			اگر (پیغام.len)
				strbuf_addch(&پیغام, '\n');
			strbuf_addstr(&پیغام, x);
		} ورنہ اگر (skip_سابقہ(sb.buf, "Author: ", &x))
			strbuf_addstr(&author_name, x);
		ورنہ اگر (skip_سابقہ(sb.buf, "Email: ", &x))
			strbuf_addstr(&author_email, x);
		ورنہ اگر (skip_سابقہ(sb.buf, "Date: ", &x))
			strbuf_addstr(&author_date, x);
	}
	fclose(fp);

	/* Skip pine's internal folder data */
	اگر (!strcmp(author_name.buf, "Mail System Internal Data")) {
		ret = 1;
		جاؤ اختتام;
	}

	strbuf_addstr(&پیغام, "\n\n");
	strbuf_addbuf(&پیغام, &mi.log_پیغام);
	strbuf_stripspace(&پیغام, تہی);

	أكد(!state->author_name);
	state->author_name = strbuf_detach(&author_name, تہی);

	أكد(!state->author_email);
	state->author_email = strbuf_detach(&author_email, تہی);

	أكد(!state->author_date);
	state->author_date = strbuf_detach(&author_date, تہی);

	أكد(!state->پیغام);
	state->پیغام = strbuf_detach(&پیغام, &state->پیغام_len);

اختتام:
	strbuf_release(&پیغام);
	strbuf_release(&author_date);
	strbuf_release(&author_email);
	strbuf_release(&author_name);
	strbuf_release(&sb);
	clear_mailinfo(&mi);
	واپس ret;
}

/**
 * Sets commit_id to the commit hash where the mail was generated from.
 * Returns 0 on success, -1 on failure.
 */
ساکن صحیح get_mail_commit_oid(ساخت object_id *commit_id, ثابت حرف *mail)
{
	ساخت strbuf sb = STRBUF_INIT;
	FILE *fp = xfopen(mail, "r");
	ثابت حرف *x;
	صحیح ret = 0;

	اگر (strbuf_getline_lf(&sb, fp) ||
	    !skip_سابقہ(sb.buf, "From ", &x) ||
	    get_oid_hex(x, commit_id) < 0)
		ret = -1;

	strbuf_release(&sb);
	fclose(fp);
	واپس ret;
}

/**
 * Sets state->msg, state->author_name, state->author_email, state->author_date
 * to the commit's respective info.
 */
ساکن خالی get_commit_info(ساخت am_state *state, ساخت commit *commit)
{
	ثابت حرف *بفر, *ident_سطر, *پیغام;
	سائز_t ident_len;
	ساخت ident_بانٹو id;

	بفر = repo_logmsg_reencode(the_repository, commit, تہی,
				      get_commit_output_encoding());

	ident_سطر = ڈھونڈو_commit_سرخی(بفر, "author", &ident_len);
	اگر (!ident_سطر)
		die(_("missing author line in commit %s"),
		      oid_مقصد_hex(&commit->object.oid));
	اگر (بانٹو_ident_سطر(&id, ident_سطر, ident_len) < 0)
		die(_("invalid ident line: %.*s"), (صحیح)ident_len, ident_سطر);

	أكد(!state->author_name);
	اگر (id.name_begin)
		state->author_name =
			xmemdupz(id.name_begin, id.name_انتہا - id.name_begin);
	ورنہ
		state->author_name = xstrdup("");

	أكد(!state->author_email);
	اگر (id.mail_begin)
		state->author_email =
			xmemdupz(id.mail_begin, id.mail_انتہا - id.mail_begin);
	ورنہ
		state->author_email = xstrdup("");

	أكد(!state->author_date);
	state->author_date = xstrdup(show_ident_date(&id, DATE_منوال(معمول)));

	أكد(!state->پیغام);
	پیغام = لڑی_تلاش(بفر, "\n\n");
	اگر (!پیغام)
		die(_("unable to parse commit %s"), oid_مقصد_hex(&commit->object.oid));
	state->پیغام = xstrdup(پیغام + 2);
	state->پیغام_len = لڑی_لمبائی(state->پیغام);
	repo_unuse_commit_بفر(the_repository, commit, بفر);
}

/**
 * Writes `commit` as a patch to the state directory's "patch" file.
 */
ساکن خالی write_commit_patch(ثابت ساخت am_state *state, ساخت commit *commit)
{
	ساخت rev_info rev_info;
	FILE *fp;

	fp = xfopen(am_راستہ(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, تہی);
	rev_info.diff = 1;
	rev_info.abbrev = 0;
	rev_info.disable_stdin = 1;
	rev_info.show_جڑ_diff = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.flags.ثنائی = 1;
	rev_info.diffopt.flags.بھرا_اشاریہ = 1;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	جوڑو_pending_object(&rev_info, &commit->object, "");
	diff_تیاری_مکمل(&rev_info.diffopt);
	log_درخت_commit(&rev_info, commit);
	release_revisions(&rev_info);
}

/**
 * Writes the diff of the index against HEAD as a patch to the state
 * directory's "patch" file.
 */
ساکن خالی write_اشاریہ_patch(ثابت ساخت am_state *state)
{
	ساخت درخت *درخت;
	ساخت object_id سر;
	ساخت rev_info rev_info;
	FILE *fp;

	اگر (!repo_get_oid(the_repository, "HEAD", &سر)) {
		ساخت commit *commit = lookup_commit_or_die(&سر, "HEAD");
		درخت = repo_get_commit_درخت(the_repository, commit);
	} ورنہ
		درخت = lookup_درخت(the_repository,
				   the_repository->hash_algo->خالی_خانے_درخت);

	fp = xfopen(am_راستہ(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, تہی);
	rev_info.diff = 1;
	rev_info.disable_stdin = 1;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	جوڑو_pending_object(&rev_info, &درخت->object, "");
	diff_تیاری_مکمل(&rev_info.diffopt);
	تسلسل_diff_اشاریہ(&rev_info, DIFF_اشاریہ_CACHED);
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
ساکن صحیح parse_mail_rebase(ساخت am_state *state, ثابت حرف *mail)
{
	ساخت commit *commit;
	ساخت object_id commit_oid;

	اگر (get_mail_commit_oid(&commit_oid, mail) < 0)
		die(_("could not parse %s"), mail);

	commit = lookup_commit_or_die(&commit_oid, mail);

	get_commit_info(state, commit);

	write_commit_patch(state, commit);

	oidcpy(&state->orig_commit, &commit_oid);
	write_state_متن(state, "original-commit", oid_مقصد_hex(&commit_oid));
	refs_update_ref(get_main_ref_store(the_repository), "am",
			"REBASE_HEAD", &commit_oid,
			تہی, REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);

	واپس 0;
}

/**
 * Applies current patch with git-apply. Returns 0 on success, -1 otherwise. If
 * `index_file` is not NULL, the patch will be applied to that index.
 */
ساکن صحیح تسلسل_apply(ثابت ساخت am_state *state, ثابت حرف *اشاریہ_file)
{
	ساخت strvec apply_paths = STRVEC_INIT;
	ساخت strvec apply_opts = STRVEC_INIT;
	ساخت apply_state apply_state;
	صحیح res, opts_بایاں;
	صحیح force_apply = 0;
	صحیح options = 0;
	ثابت حرف **apply_argv;

	اگر (init_apply_state(&apply_state, the_repository, تہی))
		BUG("init_apply_state() failed");

	strvec_ڈالو(&apply_opts, "apply");
	strvec_pushv(&apply_opts, state->git_apply_opts.v);

	/*
	 * Build a copy that apply_parse_options() can rearrange.
	 * apply_opts.v keeps referencing the allocated strings for
	 * strvec_clear() to release.
	 */
	DUP_ARRAY(apply_argv, apply_opts.v, apply_opts.nr);

	opts_بایاں = apply_parse_options(apply_opts.nr, apply_argv,
					&apply_state, &force_apply, &options,
					تہی);

	اگر (opts_بایاں != 0)
		die("unknown option passed through to git apply");

	اگر (اشاریہ_file) {
		apply_state.اشاریہ_file = اشاریہ_file;
		apply_state.cached = 1;
	} ورنہ
		apply_state.check_اشاریہ = 1;

	/*
	 * If we are allowed to fall back on 3-way merge, don't give false
	 * errors during the initial attempt.
	 */
	اگر (state->threeway && !اشاریہ_file)
		apply_state.apply_verbosity = verbosity_silent;

	اگر (check_apply_state(&apply_state, force_apply))
		BUG("check_apply_state() failed");

	strvec_ڈالو(&apply_paths, am_راستہ(state, "patch"));

	res = apply_all_patches(&apply_state, apply_paths.nr, apply_paths.v, options);

	strvec_clear(&apply_paths);
	strvec_clear(&apply_opts);
	clear_apply_state(&apply_state);
	آزاد(apply_argv);

	اگر (res)
		واپس res;

	اگر (اشاریہ_file) {
		/* Reload index as apply_all_patches() will have modified it. */
		discard_اشاریہ(the_repository->اشاریہ);
		read_اشاریہ_from(the_repository->اشاریہ, اشاریہ_file,
				repo_get_git_dir(the_repository));
	}

	واپس 0;
}

/**
 * Builds an index that contains just the blobs needed for a 3way merge.
 */
ساکن صحیح build_fake_ancestor(ثابت ساخت am_state *state, ثابت حرف *اشاریہ_file)
{
	ساخت بچہ_process cp = بچہ_PROCESS_INIT;

	cp.git_cmd = 1;
	strvec_ڈالو(&cp.دلائل, "apply");
	strvec_pushv(&cp.دلائل, state->git_apply_opts.v);
	strvec_pushf(&cp.دلائل, "--build-fake-ancestor=%s", اشاریہ_file);
	strvec_ڈالو(&cp.دلائل, am_راستہ(state, "patch"));

	اگر (تسلسل_command(&cp))
		واپس -1;

	واپس 0;
}

/**
 * Attempt a threeway merge, using index_path as the temporary index.
 */
ساکن صحیح fall_عقب_threeway(ثابت ساخت am_state *state, ثابت حرف *اشاریہ_راستہ)
{
	ساخت object_id their_درخت, our_درخت;
	ساخت object_id bases[1] = { 0 };
	ساخت ملاؤ_options o;
	ساخت commit *نتیجہ;
	حرف *their_درخت_name;

	اگر (repo_get_oid(the_repository, "HEAD", &our_درخت) < 0)
		oidcpy(&our_درخت, the_hash_algo->خالی_خانے_درخت);

	اگر (build_fake_ancestor(state, اشاریہ_راستہ))
		واپس error("could not build fake ancestor");

	discard_اشاریہ(the_repository->اشاریہ);
	read_اشاریہ_from(the_repository->اشاریہ, اشاریہ_راستہ, repo_get_git_dir(the_repository));

	اگر (write_اشاریہ_as_درخت(&bases[0], the_repository->اشاریہ, اشاریہ_راستہ, 0, تہی))
		واپس error(_("Repository lacks necessary blobs to fall back on 3-way merge."));

	say(state, stdout, _("Using index info to reconstruct a base tree..."));

	اگر (!state->quiet) {
		/*
		 * List paths that needed 3-way fallback, so that the user can
		 * review them with extra care to spot mismerges.
		 */
		ساخت rev_info rev_info;

		repo_init_revisions(the_repository, &rev_info, تہی);
		rev_info.diffopt.output_format = DIFF_FORMAT_NAME_حالت;
		rev_info.diffopt.filter |= diff_filter_ثنائی_ہندسہ('A');
		rev_info.diffopt.filter |= diff_filter_ثنائی_ہندسہ('M');
		جوڑو_pending_oid(&rev_info, "HEAD", &our_درخت, 0);
		diff_تیاری_مکمل(&rev_info.diffopt);
		تسلسل_diff_اشاریہ(&rev_info, DIFF_اشاریہ_CACHED);
		release_revisions(&rev_info);
	}

	اگر (تسلسل_apply(state, اشاریہ_راستہ))
		واپس error(_("Did you hand edit your patch?\n"
				"It does not apply to blobs recorded in its index."));

	اگر (write_اشاریہ_as_درخت(&their_درخت, the_repository->اشاریہ, اشاریہ_راستہ, 0, تہی))
		واپس error("could not write tree");

	say(state, stdout, _("Falling back to patching base and 3-way merge..."));

	discard_اشاریہ(the_repository->اشاریہ);
	repo_read_اشاریہ(the_repository);

	/*
	 * This is not so wrong. Depending on which base we picked, orig_tree
	 * may be wildly different from ours, but their_tree has the same set of
	 * wildly different changes in parts the patch did not touch, so
	 * recursive ends up canceling them, saying that we reverted all those
	 * changes.
	 */

	init_ui_ملاؤ_options(&o, the_repository);

	o.branch1 = "HEAD";
	their_درخت_name = xstrfmt("%.*s", linelen(state->پیغام), state->پیغام);
	o.branch2 = their_درخت_name;
	o.ancestor = "constructed fake ancestor";
	o.detect_directory_renames = ملاؤ_DIRECTORY_RENAMES_NONE;

	اگر (state->quiet)
		o.verbosity = 0;

	اگر (ملاؤ_ort_generic(&o, &our_درخت, &their_درخت, 1, bases, &نتیجہ)) {
		repo_rerere(the_repository, state->allow_rerere_autoupdate);
		آزاد(their_درخت_name);
		واپس error(_("Failed to merge in the changes."));
	}

	آزاد(their_درخت_name);
	واپس 0;
}

/**
 * Commits the current index with state->msg as the commit message and
 * state->author_name, state->author_email and state->author_date as the author
 * information.
 */
ساکن خالی do_commit(ثابت ساخت am_state *state)
{
	ساخت object_id درخت, والد, commit;
	ثابت ساخت object_id *old_oid;
	ساخت commit_list *parents = تہی;
	ثابت حرف *reflog_پیغام, *author, *committer = تہی;
	ساخت strbuf sb = STRBUF_INIT;

	اگر (!state->no_verify && تسلسل_hooks(the_repository, "pre-applypatch"))
		باہر_نکلو(1);

	اگر (write_اشاریہ_as_درخت(&درخت, the_repository->اشاریہ,
				repo_get_اشاریہ_file(the_repository),
				0, تہی))
		die(_("git write-tree failed to write a tree"));

	اگر (!repo_get_oid_commit(the_repository, "HEAD", &والد)) {
		old_oid = &والد;
		commit_list_داخل_کرو(lookup_commit(the_repository, &والد),
				   &parents);
	} ورنہ {
		old_oid = تہی;
		say(state, stderr, _("applying to an empty history"));
	}

	author = fmt_ident(state->author_name, state->author_email,
		WANT_AUTHOR_IDENT,
			state->ignore_date ? تہی : state->author_date,
			IDENT_STRICT);

	اگر (state->committer_date_is_author_date)
		committer = fmt_ident(getenv("GIT_COMMITTER_NAME"),
				      getenv("GIT_COMMITTER_EMAIL"),
				      WANT_COMMITTER_IDENT,
				      state->ignore_date ? تہی
							 : state->author_date,
				      IDENT_STRICT);

	اگر (commit_درخت_extended(state->پیغام, state->پیغام_len, &درخت, parents,
				 &commit, author, committer, state->sign_commit,
				 تہی))
		die(_("failed to write commit object"));

	reflog_پیغام = getenv("GIT_REFLOG_ACTION");
	اگر (!reflog_پیغام)
		reflog_پیغام = "am";

	strbuf_addf(&sb, "%s: %.*s", reflog_پیغام, linelen(state->پیغام),
			state->پیغام);

	refs_update_ref(get_main_ref_store(the_repository), sb.buf, "HEAD",
			&commit, old_oid, 0,
			UPDATE_REFS_DIE_ON_ERR);

	اگر (state->rebasing) {
		FILE *fp = xfopen(am_راستہ(state, "rewritten"), "a");

		أكد(!is_null_oid(&state->orig_commit));
		fprintf(fp, "%s ", oid_مقصد_hex(&state->orig_commit));
		fprintf(fp, "%s\n", oid_مقصد_hex(&commit));
		fclose(fp);
	}

	تسلسل_hooks(the_repository, "post-applypatch");

	commit_list_free(parents);
	strbuf_release(&sb);
}

/**
 * Validates the am_state for resuming -- the "msg" and authorship fields must
 * be filled up.
 */
ساکن خالی validate_resume_state(ثابت ساخت am_state *state)
{
	اگر (!state->پیغام)
		die(_("cannot resume: %s does not exist."),
			am_راستہ(state, "final-commit"));

	اگر (!state->author_name || !state->author_email || !state->author_date)
		die(_("cannot resume: %s does not exist."),
			am_راستہ(state, "author-script"));
}

/**
 * Interactively prompt the user on whether the current patch should be
 * applied.
 *
 * Returns 0 if the user chooses to apply the patch, 1 if the user chooses to
 * skip it.
 */
ساکن صحیح do_interactive(ساخت am_state *state)
{
	أكد(state->پیغام);

	برائے (;;) {
		حرف جواب[64];

		puts(_("Commit Body is:"));
		puts("--------------------------");
		لکھو("%s", state->پیغام);
		puts("--------------------------");

		/*
		 * TRANSLATORS: Make sure to include [y], [n], [e], [v] and [a]
		 * in your translation. The program will only accept English
		 * input at this point.
		 */
		لکھو(_("Apply? [y]es/[n]o/[e]dit/[v]iew patch/[a]ccept all: "));
		اگر (!fgets(جواب, حجم(جواب), stdin))
			die("unable to read from stdin; aborting");

		اگر (*جواب == 'y' || *جواب == 'Y') {
			واپس 0;
		} ورنہ اگر (*جواب == 'a' || *جواب == 'A') {
			state->interactive = 0;
			واپس 0;
		} ورنہ اگر (*جواب == 'n' || *جواب == 'N') {
			واپس 1;
		} ورنہ اگر (*جواب == 'e' || *جواب == 'E') {
			ساخت strbuf پیغام = STRBUF_INIT;

			اگر (!launch_editor(am_راستہ(state, "final-commit"), &پیغام, تہی)) {
				آزاد(state->پیغام);
				state->پیغام = strbuf_detach(&پیغام, &state->پیغام_len);
			}
			strbuf_release(&پیغام);
		} ورنہ اگر (*جواب == 'v' || *جواب == 'V') {
			ثابت حرف *pager = git_pager(the_repository, 1);
			ساخت بچہ_process cp = بچہ_PROCESS_INIT;

			اگر (!pager)
				pager = "cat";
			prepare_pager_دلائل(&cp, pager);
			strvec_ڈالو(&cp.دلائل, am_راستہ(state, "patch"));
			تسلسل_command(&cp);
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
ساکن خالی am_تسلسل(ساخت am_state *state, صحیح resume)
{
	ساخت strbuf sb = STRBUF_INIT;

	unlink(am_راستہ(state, "dirtyindex"));

	اگر (repo_refresh_and_write_اشاریہ(the_repository, REFRESH_QUIET, 0, 0,
					 تہی, تہی, تہی) < 0)
		die(_("unable to write index file"));

	اگر (repo_اشاریہ_has_changes(the_repository, تہی, &sb)) {
		write_state_bool(state, "dirtyindex", 1);
		die(_("Dirty index: cannot apply patches (dirty: %s)"), sb.buf);
	}

	strbuf_release(&sb);

	جب (state->cur <= state->آخری) {
		ثابت حرف *mail = am_راستہ(state, msgnum(state));
		صحیح apply_حالت;
		صحیح مقصد_keep;

		reset_ident_date();

		اگر (!file_exists(mail))
			جاؤ اگلا;

		اگر (resume) {
			validate_resume_state(state);
		} ورنہ {
			صحیح skip;

			اگر (state->rebasing)
				skip = parse_mail_rebase(state, mail);
			ورنہ
				skip = parse_mail(state, mail);

			اگر (skip)
				جاؤ اگلا; /* mail should be skipped */

			اگر (state->signoff)
				am_append_signoff(state);

			write_author_script(state);
			write_commit_پیغام(state);
		}

		اگر (state->interactive && do_interactive(state))
			جاؤ اگلا;

		مقصد_keep = 0;
		اگر (is_خالی_خانے_or_missing_file(am_راستہ(state, "patch"))) {
			انتخاب (state->خالی_خانے_type) {
			حالت DROP_خالی_خانے_COMMIT:
				say(state, stdout, _("Skipping: %.*s"), linelen(state->پیغام), state->پیغام);
				جاؤ اگلا;
				توڑو;
			حالت KEEP_خالی_خانے_COMMIT:
				مقصد_keep = 1;
				say(state, stdout, _("Creating an empty commit: %.*s"),
					linelen(state->پیغام), state->پیغام);
				توڑو;
			حالت STOP_ON_خالی_خانے_COMMIT:
				printf_ln(_("Patch is empty."));
				die_user_resolve(state);
				توڑو;
			}
		}

		اگر (تسلسل_applypatch_پیغام_hook(state))
			باہر_نکلو(1);
		اگر (مقصد_keep)
			جاؤ commit;

		say(state, stdout, _("Applying: %.*s"), linelen(state->پیغام), state->پیغام);

		apply_حالت = تسلسل_apply(state, تہی);

		اگر (apply_حالت && state->threeway) {
			ساخت strbuf sb = STRBUF_INIT;

			strbuf_addstr(&sb, am_راستہ(state, "patch-merge-index"));
			apply_حالت = fall_عقب_threeway(state, sb.buf);
			strbuf_release(&sb);

			/*
			 * Applying the patch to an earlier tree and merging
			 * the result may have produced the same tree as ours.
			 */
			اگر (!apply_حالت &&
			    !repo_اشاریہ_has_changes(the_repository, تہی, تہی)) {
				say(state, stdout, _("No changes -- Patch already applied."));
				جاؤ اگلا;
			}
		}

		اگر (apply_حالت) {
			printf_ln(_("Patch failed at %s %.*s"), msgnum(state),
				linelen(state->پیغام), state->پیغام);

			اگر (advice_enabled(ADVICE_AM_WORK_DIR))
				advise(_("Use 'git am --show-current-patch=diff' to see the failed patch"));

			die_user_resolve(state);
		}

commit:
		do_commit(state);

اگلا:
		am_اگلا(state);

		اگر (resume)
			am_بھرو(state);
		resume = 0;
	}

	اگر (!is_خالی_خانے_or_missing_file(am_راستہ(state, "rewritten"))) {
		أكد(state->rebasing);
		copy_notes_for_rebase(state);
		تسلسل_post_rewrite_hook(state);
	}

	/*
	 * In rebasing mode, it's up to the caller to take care of
	 * housekeeping.
	 */
	اگر (!state->rebasing) {
		am_destroy(state);
		تسلسل_auto_maintenance(the_repository, state->quiet);
	}
}

/**
 * Resume the current am session after patch application failure. The user did
 * all the hard work, and we do not have to do any patch application. Just
 * trust and commit what the user has in the index and working tree. If `allow_empty`
 * is true, commit as an empty commit when index has not changed and lacking a patch.
 */
ساکن خالی am_resolve(ساخت am_state *state, صحیح allow_خالی_خانے)
{
	validate_resume_state(state);

	say(state, stdout, _("Applying: %.*s"), linelen(state->پیغام), state->پیغام);

	اگر (!repo_اشاریہ_has_changes(the_repository, تہی, تہی)) {
		اگر (allow_خالی_خانے && is_خالی_خانے_or_missing_file(am_راستہ(state, "patch"))) {
			printf_ln(_("No changes - recorded it as an empty commit."));
		} ورنہ {
			printf_ln(_("No changes - did you forget to use 'git add'?\n"
				    "If there is nothing left to stage, chances are that something else\n"
				    "already introduced the same changes; you might want to skip this patch."));
			die_user_resolve(state);
		}
	}

	اگر (unmerged_اشاریہ(the_repository->اشاریہ)) {
		printf_ln(_("You still have unmerged paths in your index.\n"
			"You should 'git add' each file with resolved conflicts to mark them as such.\n"
			"You might run `git rm` on a file to accept \"deleted by them\" for it."));
		die_user_resolve(state);
	}

	اگر (state->interactive) {
		write_اشاریہ_patch(state);
		اگر (do_interactive(state))
			جاؤ اگلا;
	}

	repo_rerere(the_repository, 0);

	do_commit(state);

اگلا:
	am_اگلا(state);
	am_بھرو(state);
	am_تسلسل(state, 0);
}

/**
 * Performs a checkout fast-forward from `head` to `remote`. If `reset` is
 * true, any unmerged entries will be discarded. Returns 0 on success, -1 on
 * failure.
 */
ساکن صحیح fast_forward_مقصد(ساخت درخت *سر, ساخت درخت *remote, صحیح reset)
{
	ساخت تالا_file تالا_file = تالا_INIT;
	ساخت unpack_trees_options opts;
	ساخت درخت_desc t[2];

	اگر (repo_parse_درخت(the_repository, سر) || repo_parse_درخت(the_repository, remote))
		واپس -1;

	repo_hold_locked_اشاریہ(the_repository, &تالا_file, تالا_DIE_ON_ERROR);

	refresh_اشاریہ(the_repository->اشاریہ, REFRESH_QUIET, تہی, تہی, تہی);

	memset(&opts, 0, حجم(opts));
	opts.سر_idx = 1;
	opts.src_اشاریہ = the_repository->اشاریہ;
	opts.dst_اشاریہ = the_repository->اشاریہ;
	opts.update = 1;
	opts.ملاؤ = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = twoway_ملاؤ;
	init_درخت_desc(&t[0], &سر->object.oid, سر->بفر, سر->سائز);
	init_درخت_desc(&t[1], &remote->object.oid, remote->بفر, remote->سائز);

	اگر (unpack_trees(2, t, &opts)) {
		rollback_تالا_file(&تالا_file);
		واپس -1;
	}

	اگر (write_locked_اشاریہ(the_repository->اشاریہ, &تالا_file, COMMIT_تالا))
		die(_("unable to write new index file"));

	واپس 0;
}

/**
 * Merges a tree into the index. The index's stat info will take precedence
 * over the merged tree's. Returns 0 on success, -1 on failure.
 */
ساکن صحیح ملاؤ_درخت(ساخت درخت *درخت)
{
	ساخت تالا_file تالا_file = تالا_INIT;
	ساخت unpack_trees_options opts;
	ساخت درخت_desc t[1];

	اگر (repo_parse_درخت(the_repository, درخت))
		واپس -1;

	repo_hold_locked_اشاریہ(the_repository, &تالا_file, تالا_DIE_ON_ERROR);

	memset(&opts, 0, حجم(opts));
	opts.سر_idx = 1;
	opts.src_اشاریہ = the_repository->اشاریہ;
	opts.dst_اشاریہ = the_repository->اشاریہ;
	opts.ملاؤ = 1;
	opts.fn = oneway_ملاؤ;
	init_درخت_desc(&t[0], &درخت->object.oid, درخت->بفر, درخت->سائز);

	اگر (unpack_trees(1, t, &opts)) {
		rollback_تالا_file(&تالا_file);
		واپس -1;
	}

	اگر (write_locked_اشاریہ(the_repository->اشاریہ, &تالا_file, COMMIT_تالا))
		die(_("unable to write new index file"));

	واپس 0;
}

/**
 * Clean the index without touching entries that are not modified between
 * `head` and `remote`.
 */
ساکن صحیح clean_اشاریہ(ثابت ساخت object_id *سر, ثابت ساخت object_id *remote)
{
	ساخت درخت *سر_درخت, *remote_درخت, *اشاریہ_درخت;
	ساخت object_id اشاریہ;

	سر_درخت = repo_parse_درخت_بالواسطہ(the_repository, سر);
	اگر (!سر_درخت)
		واپس error(_("Could not parse object '%s'."), oid_مقصد_hex(سر));

	remote_درخت = repo_parse_درخت_بالواسطہ(the_repository, remote);
	اگر (!remote_درخت)
		واپس error(_("Could not parse object '%s'."), oid_مقصد_hex(remote));

	repo_read_اشاریہ_unmerged(the_repository);

	اگر (fast_forward_مقصد(سر_درخت, سر_درخت, 1))
		واپس -1;

	اگر (write_اشاریہ_as_درخت(&اشاریہ, the_repository->اشاریہ,
				repo_get_اشاریہ_file(the_repository),
				0, تہی))
		واپس -1;

	اشاریہ_درخت = repo_parse_درخت_بالواسطہ(the_repository, &اشاریہ);
	اگر (!اشاریہ_درخت)
		واپس error(_("Could not parse object '%s'."), oid_مقصد_hex(&اشاریہ));

	اگر (fast_forward_مقصد(اشاریہ_درخت, remote_درخت, 0))
		واپس -1;

	اگر (ملاؤ_درخت(remote_درخت))
		واپس -1;

	remove_branch_state(the_repository, 0);

	واپس 0;
}

/**
 * Resets rerere's merge resolution metadata.
 */
ساکن خالی am_rerere_clear(خالی)
{
	ساخت string_list ملاؤ_rr = STRING_LIST_INIT_DUP;
	rerere_clear(the_repository, &ملاؤ_rr);
	string_list_clear(&ملاؤ_rr, 1);
}

/**
 * Resume the current am session by skipping the current patch.
 */
ساکن خالی am_skip(ساخت am_state *state)
{
	ساخت object_id سر;

	am_rerere_clear();

	اگر (repo_get_oid(the_repository, "HEAD", &سر))
		oidcpy(&سر, the_hash_algo->خالی_خانے_درخت);

	اگر (clean_اشاریہ(&سر, &سر))
		die(_("failed to clean index"));

	اگر (state->rebasing) {
		FILE *fp = xfopen(am_راستہ(state, "rewritten"), "a");

		أكد(!is_null_oid(&state->orig_commit));
		fprintf(fp, "%s ", oid_مقصد_hex(&state->orig_commit));
		fprintf(fp, "%s\n", oid_مقصد_hex(&سر));
		fclose(fp);
	}

	am_اگلا(state);
	am_بھرو(state);
	am_تسلسل(state, 0);
}

/**
 * Returns true if it is safe to reset HEAD to the ORIG_HEAD, false otherwise.
 *
 * It is not safe to reset HEAD when:
 * 1. git-am previously failed because the index was dirty.
 * 2. HEAD has moved since git-am previously failed.
 */
ساکن صحیح محفوظ_مقصد_abort(ثابت ساخت am_state *state)
{
	ساخت strbuf sb = STRBUF_INIT;
	ساخت object_id abort_safety, سر;

	اگر (file_exists(am_راستہ(state, "dirtyindex")))
		واپس 0;

	اگر (read_state_file(&sb, state, "abort-safety", 1) > 0) {
		اگر (get_oid_hex(sb.buf, &abort_safety))
			die(_("could not parse %s"), am_راستہ(state, "abort-safety"));
	} ورنہ
		oidclr(&abort_safety, the_repository->hash_algo);
	strbuf_release(&sb);

	اگر (repo_get_oid(the_repository, "HEAD", &سر))
		oidclr(&سر, the_repository->hash_algo);

	اگر (oideq(&سر, &abort_safety))
		واپس 1;

	warning(_("You seem to have moved HEAD since the last 'am' failure.\n"
		"Not rewinding to ORIG_HEAD"));

	واپس 0;
}

/**
 * Aborts the current am session if it is safe to do so.
 */
ساکن خالی am_abort(ساخت am_state *state)
{
	ساخت object_id curr_سر, orig_سر;
	صحیح has_curr_سر, has_orig_سر;
	حرف *curr_branch;

	اگر (!محفوظ_مقصد_abort(state)) {
		am_destroy(state);
		واپس;
	}

	am_rerere_clear();

	curr_branch = refs_resolve_refdup(get_main_ref_store(the_repository),
					  "HEAD", 0, &curr_سر, تہی);
	has_curr_سر = curr_branch && !is_null_oid(&curr_سر);
	اگر (!has_curr_سر)
		oidcpy(&curr_سر, the_hash_algo->خالی_خانے_درخت);

	has_orig_سر = !repo_get_oid(the_repository, "ORIG_HEAD", &orig_سر);
	اگر (!has_orig_سر)
		oidcpy(&orig_سر, the_hash_algo->خالی_خانے_درخت);

	اگر (clean_اشاریہ(&curr_سر, &orig_سر))
		die(_("failed to clean index"));

	اگر (has_orig_سر)
		refs_update_ref(get_main_ref_store(the_repository),
				"am --abort", "HEAD", &orig_سر,
				has_curr_سر ? &curr_سر : تہی, 0,
				UPDATE_REFS_DIE_ON_ERR);
	ورنہ اگر (curr_branch)
		refs_delete_ref(get_main_ref_store(the_repository), تہی,
				curr_branch, تہی, REF_NO_DEREF);

	آزاد(curr_branch);
	am_destroy(state);
}

ساکن صحیح show_patch(ساخت am_state *state, شمار resume_type resume_منوال)
{
	ساخت strbuf sb = STRBUF_INIT;
	ثابت حرف *patch_راستہ;
	صحیح len;

	اگر (!is_null_oid(&state->orig_commit)) {
		ساخت بچہ_process cmd = بچہ_PROCESS_INIT;

		strvec_pushl(&cmd.دلائل, "show", oid_مقصد_hex(&state->orig_commit),
			     "--", تہی);
		cmd.git_cmd = 1;
		واپس تسلسل_command(&cmd);
	}

	انتخاب (resume_منوال) {
	حالت RESUME_SHOW_PATCH_RAW:
		patch_راستہ = am_راستہ(state, msgnum(state));
		توڑو;
	حالت RESUME_SHOW_PATCH_DIFF:
		patch_راستہ = am_راستہ(state, "patch");
		توڑو;
	طےشدہ:
		BUG("invalid mode for --show-current-patch");
	}

	len = strbuf_read_file(&sb, patch_راستہ, 0);
	اگر (len < 0)
		die_errno(_("failed to read '%s'"), patch_راستہ);

	تیاری_pager(the_repository);
	write_اندر_بھرا(1, sb.buf, sb.len);
	strbuf_release(&sb);
	واپس 0;
}

/**
 * parse_options() callback that validates and sets opt->value to the
 * PATCH_FORMAT_* enum value corresponding to `arg`.
 */
ساکن صحیح parse_opt_patchformat(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	صحیح *opt_قدر = opt->قدر;

	اگر (unset)
		*opt_قدر = PATCH_FORMAT_UNKNOWN;
	ورنہ اگر (!strcmp(arg, "mbox"))
		*opt_قدر = PATCH_FORMAT_MBOX;
	ورنہ اگر (!strcmp(arg, "stgit"))
		*opt_قدر = PATCH_FORMAT_STGIT;
	ورنہ اگر (!strcmp(arg, "stgit-series"))
		*opt_قدر = PATCH_FORMAT_STGIT_SERIES;
	ورنہ اگر (!strcmp(arg, "hg"))
		*opt_قدر = PATCH_FORMAT_HG;
	ورنہ اگر (!strcmp(arg, "mboxrd"))
		*opt_قدر = PATCH_FORMAT_MBOXRD;
	/*
	 * Please update $__git_patchformat in git-completion.bash
	 * when you add new options
	 */
	ورنہ
		واپس error(_("invalid value for '%s': '%s'"),
			     "--patch-format", arg);
	واپس 0;
}

ساکن صحیح parse_opt_show_current_patch(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	صحیح *opt_قدر = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	اگر (!arg)
		*opt_قدر = opt->defval;
	ورنہ اگر (!strcmp(arg, "raw"))
		*opt_قدر = RESUME_SHOW_PATCH_RAW;
	ورنہ اگر (!strcmp(arg, "diff"))
		*opt_قدر = RESUME_SHOW_PATCH_DIFF;
	/*
	 * Please update $__git_showcurrentpatch in git-completion.bash
	 * when you add new options
	 */
	ورنہ
		واپس error(_("invalid value for '%s': '%s'"),
			     "--show-current-patch", arg);
	واپس 0;
}

صحیح cmd_am(صحیح argc,
	   ثابت حرف **argv,
	   ثابت حرف *سابقہ,
	   ساخت repository *repo غیر_مستعمل)
{
	ساخت am_state state;
	صحیح ثنائی = -1;
	صحیح keep_cr = -1;
	صحیح patch_format = PATCH_FORMAT_UNKNOWN;
	شمار resume_type resume_منوال = RESUME_FALSE;
	صحیح اندر_پیش_رفت;
	صحیح ret = 0;

	ثابت حرف * ثابت usage[] = {
		N_("git am [<options>] [(<mbox> | <Maildir>)...]"),
		N_("git am [<options>] (--continue | --skip | --abort)"),
		تہی
	};

	ساخت option options[] = {
		OPT_BOOL('i', "interactive", &state.interactive,
			N_("run interactively")),
		OPT_BOOL('n', "no-verify", &state.no_verify,
			N_("bypass pre-applypatch and applypatch-msg hooks")),
		OPT_HIDDEN_BOOL('b', "binary", &ثنائی,
			N_("historical option -- no-op")),
		OPT_BOOL('3', "3way", &state.threeway,
			N_("allow fall back on 3way merging if needed")),
		OPT__QUIET(&state.quiet, N_("be quiet")),
		OPT_سیٹ_INT('s', "signoff", &state.signoff,
			N_("add a Signed-off-by trailer to the commit message"),
			SIGNOFF_EXPLICIT),
		OPT_BOOL('u', "utf8", &state.utf8,
			N_("recode into utf8 (default)")),
		OPT_سیٹ_INT('k', "keep", &state.keep,
			N_("pass -k flag to git-mailinfo"), KEEP_TRUE),
		OPT_سیٹ_INT(0, "keep-non-patch", &state.keep,
			N_("pass -b flag to git-mailinfo"), KEEP_NON_PATCH),
		OPT_BOOL('m', "message-id", &state.پیغام_id,
			N_("pass -m flag to git-mailinfo")),
		OPT_سیٹ_INT(0, "keep-cr", &keep_cr,
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
		OPT_PASSTHRU_ARGV(0, "ignore-space-change", &state.git_apply_opts, تہی,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU_ARGV(0, "ignore-whitespace", &state.git_apply_opts, تہی,
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
		OPT_PASSTHRU_ARGV('C', تہی, &state.git_apply_opts, N_("n"),
			N_("pass it through git-apply"),
			0),
		OPT_PASSTHRU_ARGV('p', تہی, &state.git_apply_opts, N_("num"),
			N_("pass it through git-apply"),
			0),
		OPT_CALLBACK(0, "patch-format", &patch_format, N_("format"),
			N_("format the patch(es) are in"),
			parse_opt_patchformat),
		OPT_PASSTHRU_ARGV(0, "reject", &state.git_apply_opts, تہی,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_STRING(0, "resolvemsg", &state.resolvemsg, تہی,
			N_("override error message when patch failure occurs")),
		OPT_CMDMODE(0, "continue", &resume_منوال,
			N_("continue applying patches after resolving a conflict"),
			RESUME_RESOLVED),
		OPT_CMDMODE('r', "resolved", &resume_منوال,
			N_("synonyms for --continue"),
			RESUME_RESOLVED),
		OPT_CMDMODE(0, "skip", &resume_منوال,
			N_("skip the current patch"),
			RESUME_SKIP),
		OPT_CMDMODE(0, "abort", &resume_منوال,
			N_("restore the original branch and abort the patching operation"),
			RESUME_ABORT),
		OPT_CMDMODE(0, "quit", &resume_منوال,
			N_("abort the patching operation but keep HEAD where it is"),
			RESUME_QUIT),
		{
			.type = OPTION_CALLBACK,
			.long_name = "show-current-patch",
			.قدر = &resume_منوال,
			.precision = حجم(resume_منوال),
			.argh = "(diff|raw)",
			.help = N_("show the patch being applied"),
			.flags = PARSE_OPT_CMDMODE | PARSE_OPT_OPTARG | PARSE_OPT_NONEG | PARSE_OPT_LITERAL_ARGHELP,
			.callback = parse_opt_show_current_patch,
			.defval = RESUME_SHOW_PATCH_RAW,
		},
		OPT_CMDMODE(0, "retry", &resume_منوال,
			N_("try to apply current patch again"),
			RESUME_APPLY),
		OPT_CMDMODE(0, "allow-empty", &resume_منوال,
			N_("record the empty patch as an empty commit"),
			RESUME_ALLOW_خالی_خانے),
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
			.قدر = &state.sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG-sign commits"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_CALLBACK_F(0, "empty", &state.خالی_خانے_type, "(stop|drop|keep)",
		  N_("how to handle empty patches"),
		  PARSE_OPT_NONEG, am_option_parse_خالی_خانے),
		OPT_HIDDEN_BOOL(0, "rebasing", &state.rebasing,
			N_("(internal use for git-rebase)")),
		OPT_انتہا()
	};

	show_usage_with_options_if_asked(argc, argv, usage, options);

	repo_config(the_repository, git_default_config, تہی);

	am_state_init(&state);

	اندر_پیش_رفت = am_اندر_پیش_رفت(&state);
	اگر (اندر_پیش_رفت)
		am_بھرو(&state);

	argc = parse_options(argc, argv, سابقہ, options, usage, 0);

	اگر (ثنائی >= 0)
		fprintf_ln(stderr, _("The -b/--binary option has been a no-op for long time, and\n"
				"it will be removed. Please do not use it anymore."));

	/* Ensure a valid committer ident can be constructed */
	git_committer_info(IDENT_STRICT);

	اگر (repo_read_اشاریہ_preload(the_repository, تہی, 0) < 0)
		die(_("failed to read the index"));

	اگر (اندر_پیش_رفت) {
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
		اگر (argc || (resume_منوال == RESUME_FALSE && !isatty(0)))
			die(_("previous rebase directory %s still exists but mbox given."),
				state.dir);

		اگر (resume_منوال == RESUME_FALSE)
			resume_منوال = RESUME_APPLY;

		اگر (state.signoff == SIGNOFF_EXPLICIT)
			am_append_signoff(&state);
	} ورنہ {
		ساخت strvec paths = STRVEC_INIT;
		صحیح i;

		/*
		 * Handle stray state directory in the independent-run case. In
		 * the --rebasing case, it is up to the caller to take care of
		 * stray directories.
		 */
		اگر (file_exists(state.dir) && !state.rebasing) {
			اگر (resume_منوال == RESUME_ABORT || resume_منوال == RESUME_QUIT) {
				am_destroy(&state);
				am_state_release(&state);
				واپس 0;
			}

			die(_("Stray %s directory found.\n"
				"Use \"git am --abort\" to remove it."),
				state.dir);
		}

		اگر (resume_منوال)
			die(_("Resolve operation not in progress, we are not resuming."));

		برائے (i = 0; i < argc; i++) {
			اگر (is_absolute_راستہ(argv[i]) || !سابقہ)
				strvec_ڈالو(&paths, argv[i]);
			ورنہ
				strvec_ڈالو(&paths, mkpath("%s/%s", سابقہ, argv[i]));
		}

		اگر (state.interactive && !paths.nr)
			die(_("interactive mode requires patches on the command line"));

		am_تیاری(&state, patch_format, paths.v, keep_cr);

		strvec_clear(&paths);
	}

	انتخاب (resume_منوال) {
	حالت RESUME_FALSE:
		am_تسلسل(&state, 0);
		توڑو;
	حالت RESUME_APPLY:
		am_تسلسل(&state, 1);
		توڑو;
	حالت RESUME_RESOLVED:
	حالت RESUME_ALLOW_خالی_خانے:
		am_resolve(&state, resume_منوال == RESUME_ALLOW_خالی_خانے ? 1 : 0);
		توڑو;
	حالت RESUME_SKIP:
		am_skip(&state);
		توڑو;
	حالت RESUME_ABORT:
		am_abort(&state);
		توڑو;
	حالت RESUME_QUIT:
		am_rerere_clear();
		am_destroy(&state);
		توڑو;
	حالت RESUME_SHOW_PATCH_RAW:
	حالت RESUME_SHOW_PATCH_DIFF:
		ret = show_patch(&state, resume_منوال);
		توڑو;
	طےشدہ:
		BUG("invalid resume value");
	}

	am_state_release(&state);

	واپس ret;
}
