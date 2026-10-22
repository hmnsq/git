/*
 * Builtin "git am"
 *
 * Based on git-am.sh by Junio C Hamano.
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"
#ضمن "abspath.h"
#ضمن "advice.h"
#ضمن "config.h"
#ضمن "editor.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "parse-options.h"
#ضمن "dir.h"
#ضمن "run-command.h"
#ضمن "hook.h"
#ضمن "quote.h"
#ضمن "tempfile.h"
#ضمن "lockfile.h"
#ضمن "cache-tree.h"
#ضمن "refs.h"
#ضمن "commit.h"
#ضمن "diff.h"
#ضمن "unpack-trees.h"
#ضمن "branch.h"
#ضمن "object-name.h"
#ضمن "preload-index.h"
#ضمن "sequencer.h"
#ضمن "revision.h"
#ضمن "merge-ort-wrappers.h"
#ضمن "log-tree.h"
#ضمن "notes-utils.h"
#ضمن "rerere.h"
#ضمن "mailinfo.h"
#ضمن "apply.h"
#ضمن "string-list.h"
#ضمن "pager.h"
#ضمن "path.h"
#ضمن "pretty.h"

/**
 * Returns the length of the first line of msg.
 */
ساكن صحيح linelen(ثابت حرف *پیغام)
{
	أرجع strchrnul(پیغام, '\n') - پیغام;
}

/**
 * Returns true if `str` consists of only whitespace, false otherwise.
 */
ساكن صحيح str_isspace(ثابت حرف *str)
{
	لكل (; *str; str++)
		إذا (!خلا_اے(*str))
			أرجع 0;

	أرجع 1;
}

تعداد patch_format {
	PATCH_FORMAT_UNKNOWN = 0,
	PATCH_FORMAT_MBOX,
	PATCH_FORMAT_STGIT,
	PATCH_FORMAT_STGIT_SERIES,
	PATCH_FORMAT_HG,
	PATCH_FORMAT_MBOXRD
};

تعداد keep_type {
	KEEP_FALSE = 0,
	KEEP_TRUE,      /* pass -k flag to git-mailinfo */
	KEEP_NON_PATCH  /* pass -b flag to git-mailinfo */
};

تعداد scissors_type {
	SCISSORS_UNSET = -1,
	SCISSORS_FALSE = 0,  /* pass --no-scissors to git-mailinfo */
	SCISSORS_TRUE        /* pass --scissors to git-mailinfo */
};

تعداد signoff_type {
	SIGNOFF_FALSE = 0,
	SIGNOFF_TRUE = 1,
	SIGNOFF_EXPLICIT /* --signoff was set on the command-line */
};

تعداد resume_type {
	RESUME_FALSE = 0,
	RESUME_APPLY,
	RESUME_RESOLVED,
	RESUME_SKIP,
	RESUME_ABORT,
	RESUME_QUIT,
	RESUME_SHOW_PATCH_RAW,
	RESUME_SHOW_PATCH_DIFF,
	RESUME_ALLOW_خالی_تھاواں,
};

تعداد خالی_تھاواں_action {
	STOP_ON_خالی_تھاواں_COMMIT = 0,  /* output errors and stop in the middle of an am session */
	DROP_خالی_تھاواں_COMMIT,         /* skip with a notice message, unless "--quiet" has been passed */
	KEEP_خالی_تھاواں_COMMIT,         /* keep recording as empty commits */
};

بنية am_state {
	/* state directory path */
	حرف *dir;

	/* current and last patch numbers, 1-indexed */
	صحيح cur;
	صحيح آخری;

	/* commit metadata and message */
	حرف *author_name;
	حرف *author_email;
	حرف *author_date;
	حرف *پیغام;
	ناپ_t پیغام_len;

	/* when --rebasing, records the original commit the patch came from */
	بنية object_id orig_commit;

	/* number of digits in patch filename */
	صحيح prec;

	/* various operating modes and command line options */
	صحيح interactive;
	صحيح no_verify;
	صحيح threeway;
	صحيح quiet;
	صحيح signoff; /* enum signoff_type */
	صحيح utf8;
	صحيح keep; /* enum keep_type */
	صحيح سنیہا_id;
	صحيح scissors; /* enum scissors_type */
	صحيح quoted_cr; /* enum quoted_cr_action */
	صحيح خالی_تھاواں_type; /* enum empty_action */
	بنية strvec git_apply_opts;
	ثابت حرف *resolvemsg;
	صحيح committer_date_is_author_date;
	صحيح ignore_date;
	صحيح allow_rerere_autoupdate;
	ثابت حرف *sign_commit;
	صحيح rebasing;
};

/**
 * Initializes am_state with the default values.
 */
ساكن فراغ am_state_init(بنية am_state *state)
{
	صحيح gpgsign;

	memset(state, 0, حجم(*state));

	state->dir = repo_git_رستہ(the_repository, "rebase-apply");

	state->prec = 4;

	repo_config_get_bool(the_repository, "am.threeway", &state->threeway);

	state->utf8 = 1;

	repo_config_get_bool(the_repository, "am.messageid", &state->سنیہا_id);

	state->scissors = SCISSORS_UNSET;
	state->quoted_cr = quoted_cr_unset;

	strvec_init(&state->git_apply_opts);

	إذا (!repo_config_get_bool(the_repository, "commit.gpgsign", &gpgsign))
		state->sign_commit = gpgsign ? "" : عدم;
}

/**
 * Releases memory allocated by an am_state.
 */
ساكن فراغ am_state_release(بنية am_state *state)
{
	حرر(state->dir);
	حرر(state->author_name);
	حرر(state->author_email);
	حرر(state->author_date);
	حرر(state->پیغام);
	strvec_clear(&state->git_apply_opts);
}

ساكن صحيح am_option_parse_quoted_cr(ثابت بنية option *opt,
				     ثابت حرف *arg, صحيح unset)
{
	BUG_ON_OPT_NEG(unset);

	إذا (mailinfo_parse_quoted_cr_action(arg, opt->قدر) != 0)
		أرجع error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	أرجع 0;
}

ساكن صحيح am_option_parse_خالی_تھاواں(ثابت بنية option *opt,
				     ثابت حرف *arg, صحيح unset)
{
	صحيح *opt_قدر = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	إذا (!strcmp(arg, "stop"))
		*opt_قدر = STOP_ON_خالی_تھاواں_COMMIT;
	وإلا إذا (!strcmp(arg, "drop"))
		*opt_قدر = DROP_خالی_تھاواں_COMMIT;
	وإلا إذا (!strcmp(arg, "keep"))
		*opt_قدر = KEEP_خالی_تھاواں_COMMIT;
	وإلا
		أرجع error(_("invalid value for '%s': '%s'"), "--empty", arg);

	أرجع 0;
}

/**
 * Returns path relative to the am_state directory.
 */
ساكن مضمن ثابت حرف *am_رستہ(ثابت بنية am_state *state, ثابت حرف *رستہ)
{
	أرجع mkpath("%s/%s", state->dir, رستہ);
}

/**
 * For convenience to call write_file()
 */
ساكن فراغ write_state_متن(ثابت بنية am_state *state,
			     ثابت حرف *name, ثابت حرف *string)
{
	write_file(am_رستہ(state, name), "%s", string);
}

ساكن فراغ write_state_گنتی(ثابت بنية am_state *state,
			      ثابت حرف *name, صحيح قدر)
{
	write_file(am_رستہ(state, name), "%d", قدر);
}

ساكن فراغ write_state_bool(ثابت بنية am_state *state,
			     ثابت حرف *name, صحيح قدر)
{
	write_state_متن(state, name, قدر ? "t" : "f");
}

/**
 * If state->quiet is false, calls fprintf(fp, fmt, ...), and appends a newline
 * at the end.
 */
__attribute__((format (لکھو, 3, 4)))
ساكن فراغ say(ثابت بنية am_state *state, FILE *fp, ثابت حرف *fmt, ...)
{
	va_list ap;

	va_شروع(ap, fmt);
	إذا (!state->quiet) {
		vfprintf(fp, fmt, ap);
		putc('\n', fp);
	}
	va_سرا(ap);
}

/**
 * Returns 1 if there is an am session in progress, 0 otherwise.
 */
ساكن صحيح am_وچ_ترقی(ثابت بنية am_state *state)
{
	بنية stat st;

	إذا (lstat(state->dir, &st) < 0 || !S_ISDIR(st.st_منوال))
		أرجع 0;
	إذا (lstat(am_رستہ(state, "last"), &st) || !S_ISREG(st.st_منوال))
		أرجع 0;
	إذا (lstat(am_رستہ(state, "next"), &st) || !S_ISREG(st.st_منوال))
		أرجع 0;
	أرجع 1;
}

/**
 * Reads the contents of `file` in the `state` directory into `sb`. Returns the
 * number of bytes read on success, -1 if the file does not exist. If `trim` is
 * set, trailing whitespace will be removed.
 */
ساكن صحيح read_state_file(بنية strbuf *sb, ثابت بنية am_state *state,
			ثابت حرف *file, صحيح trim)
{
	strbuf_reset(sb);

	إذا (strbuf_read_file(sb, am_رستہ(state, file), 0) >= 0) {
		إذا (trim)
			strbuf_trim(sb);

		أرجع sb->len;
	}

	إذا (errno == ENOENT)
		أرجع -1;

	die_errno(_("could not read '%s'"), am_رستہ(state, file));
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
ساكن صحيح read_am_author_script(بنية am_state *state)
{
	ثابت حرف *filename = am_رستہ(state, "author-script");

	أكد(!state->author_name);
	أكد(!state->author_email);
	أكد(!state->author_date);

	أرجع read_author_script(filename, &state->author_name,
				  &state->author_email, &state->author_date, 1);
}

/**
 * Saves state->author_name, state->author_email and state->author_date in the
 * state directory's "author-script" file.
 */
ساكن فراغ write_author_script(ثابت بنية am_state *state)
{
	بنية strbuf sb = STRBUF_INIT;

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
ساكن صحيح read_commit_پیغام(بنية am_state *state)
{
	بنية strbuf sb = STRBUF_INIT;

	أكد(!state->پیغام);

	إذا (read_state_file(&sb, state, "final-commit", 0) < 0) {
		strbuf_release(&sb);
		أرجع -1;
	}

	state->پیغام = strbuf_detach(&sb, &state->پیغام_len);
	أرجع 0;
}

/**
 * Saves state->msg in the state directory's "final-commit" file.
 */
ساكن فراغ write_commit_پیغام(ثابت بنية am_state *state)
{
	ثابت حرف *filename = am_رستہ(state, "final-commit");
	write_file_buf(filename, state->پیغام, state->پیغام_len);
}

/**
 * Loads state from disk.
 */
ساكن فراغ am_لدو(بنية am_state *state)
{
	بنية strbuf sb = STRBUF_INIT;

	إذا (read_state_file(&sb, state, "next", 1) < 0)
		BUG("state file 'next' does not exist");
	state->cur = strtol(sb.buf, عدم, 10);

	إذا (read_state_file(&sb, state, "last", 1) < 0)
		BUG("state file 'last' does not exist");
	state->آخری = strtol(sb.buf, عدم, 10);

	إذا (read_am_author_script(state) < 0)
		die(_("could not parse author script"));

	read_commit_پیغام(state);

	إذا (read_state_file(&sb, state, "original-commit", 1) < 0)
		oidclr(&state->orig_commit, the_repository->hash_algo);
	وإلا إذا (get_oid_hex(sb.buf, &state->orig_commit) < 0)
		die(_("could not parse %s"), am_رستہ(state, "original-commit"));

	read_state_file(&sb, state, "threeway", 1);
	state->threeway = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "quiet", 1);
	state->quiet = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "sign", 1);
	state->signoff = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "utf8", 1);
	state->utf8 = !strcmp(sb.buf, "t");

	إذا (file_exists(am_رستہ(state, "rerere-autoupdate"))) {
		read_state_file(&sb, state, "rerere-autoupdate", 1);
		state->allow_rerere_autoupdate = strcmp(sb.buf, "t") ?
			RERERE_NOAUTOUPDATE : RERERE_AUTOUPDATE;
	} وإلا {
		state->allow_rerere_autoupdate = 0;
	}

	read_state_file(&sb, state, "keep", 1);
	إذا (!strcmp(sb.buf, "t"))
		state->keep = KEEP_TRUE;
	وإلا إذا (!strcmp(sb.buf, "b"))
		state->keep = KEEP_NON_PATCH;
	وإلا
		state->keep = KEEP_FALSE;

	read_state_file(&sb, state, "messageid", 1);
	state->سنیہا_id = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "scissors", 1);
	إذا (!strcmp(sb.buf, "t"))
		state->scissors = SCISSORS_TRUE;
	وإلا إذا (!strcmp(sb.buf, "f"))
		state->scissors = SCISSORS_FALSE;
	وإلا
		state->scissors = SCISSORS_UNSET;

	read_state_file(&sb, state, "quoted-cr", 1);
	إذا (!*sb.buf)
		state->quoted_cr = quoted_cr_unset;
	وإلا إذا (mailinfo_parse_quoted_cr_action(sb.buf, &state->quoted_cr) != 0)
		die(_("could not parse %s"), am_رستہ(state, "quoted-cr"));

	read_state_file(&sb, state, "apply-opt", 1);
	strvec_clear(&state->git_apply_opts);
	إذا (sq_dequote_منزل_strvec(sb.buf, &state->git_apply_opts) < 0)
		die(_("could not parse %s"), am_رستہ(state, "apply-opt"));

	state->rebasing = !!file_exists(am_رستہ(state, "rebasing"));

	strbuf_release(&sb);
}

/**
 * Removes the am_state directory, forcefully terminating the current am
 * session.
 */
ساكن فراغ am_destroy(ثابت بنية am_state *state)
{
	بنية strbuf sb = STRBUF_INIT;

	strbuf_addstr(&sb, state->dir);
	remove_dir_recursively(&sb, 0);
	strbuf_release(&sb);
}

/**
 * Runs applypatch-msg hook. Returns its exit code.
 */
ساكن صحيح لگاتار_applypatch_پیغام_hook(بنية am_state *state)
{
	صحيح ret = 0;

	أكد(state->پیغام);

	إذا (!state->no_verify) {
		بنية لگاتار_hooks_opt opt = لگاتار_HOOKS_OPT_INIT_FORCE_SERIAL;
		strvec_ڈالو(&opt.دلیلاں, am_رستہ(state, "final-commit"));
		ret = لگاتار_hooks_opt(the_repository, "applypatch-msg", &opt);
	}

	إذا (!ret) {
		FREE_AND_NULL(state->پیغام);
		إذا (read_commit_پیغام(state) < 0)
			die(_("'%s' was deleted by the applypatch-msg hook"),
				am_رستہ(state, "final-commit"));
	}

	أرجع ret;
}

/**
 * Runs post-rewrite hook. Returns it exit code.
 */
ساكن صحيح لگاتار_post_rewrite_hook(ثابت بنية am_state *state)
{
	بنية لگاتار_hooks_opt opt = لگاتار_HOOKS_OPT_INIT;

	strvec_ڈالو(&opt.دلیلاں, "rebase");
	opt.رستہ_منزل_stdin = am_رستہ(state, "rewritten");

	أرجع لگاتار_hooks_opt(the_repository, "post-rewrite", &opt);
}

/**
 * Reads the state directory's "rewritten" file, and copies notes from the old
 * commits listed in the file to their rewritten commits.
 *
 * Returns 0 on success, -1 on failure.
 */
ساكن صحيح copy_notes_for_rebase(ثابت بنية am_state *state)
{
	بنية notes_rewrite_cfg *c;
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *invalid_سطر = _("Malformed input line: '%s'.");
	ثابت حرف *پیغام = "Notes added by 'git rebase'";
	FILE *fp;
	صحيح ret = 0;

	أكد(state->rebasing);

	c = init_copy_notes_for_rewrite("rebase");
	إذا (!c)
		أرجع 0;

	fp = xfopen(am_رستہ(state, "rewritten"), "r");

	طالما (!strbuf_getline_lf(&sb, fp)) {
		بنية object_id from_obj, منزل_obj;
		ثابت حرف *p;

		إذا (sb.len != the_hash_algo->hexsz * 2 + 1) {
			ret = error(invalid_سطر, sb.buf);
			اذهب انت;
		}

		إذا (parse_oid_hex(sb.buf, &from_obj, &p)) {
			ret = error(invalid_سطر, sb.buf);
			اذهب انت;
		}

		إذا (*p != ' ') {
			ret = error(invalid_سطر, sb.buf);
			اذهب انت;
		}

		إذا (get_oid_hex(p + 1, &منزل_obj)) {
			ret = error(invalid_سطر, sb.buf);
			اذهب انت;
		}

		إذا (copy_note_for_rewrite(c, &from_obj, &منزل_obj))
			ret = error(_("Failed to copy notes from '%s' to '%s'"),
					oid_منزل_hex(&from_obj), oid_منزل_hex(&منزل_obj));
	}

انت:
	انت_copy_notes_for_rewrite(the_repository, c, پیغام);
	fclose(fp);
	strbuf_release(&sb);
	أرجع ret;
}

/**
 * Determines if the file looks like a piece of RFC2822 mail by grabbing all
 * non-indented lines and checking if they look like they begin with valid
 * header field names.
 *
 * Returns 1 if the file looks like a piece of mail, 0 otherwise.
 */
ساكن صحيح is_mail(FILE *fp)
{
	ثابت حرف *سرخی_regex = "^[!-9;-~]+:";
	بنية strbuf sb = STRBUF_INIT;
	regex_t regex;
	صحيح ret = 1;

	إذا (fseek(fp, 0L, SEEK_سیٹ))
		die_errno(_("fseek failed"));

	إذا (regcomp(&regex, سرخی_regex, REG_NOSUB | REG_EXTENDED))
		die("invalid pattern: %s", سرخی_regex);

	طالما (!strbuf_getline(&sb, fp)) {
		إذا (!sb.len)
			اكسر; /* End of header */

		/* Ignore indented folded lines */
		إذا (*sb.buf == '\t' || *sb.buf == ' ')
			استمر;

		/* It's a header if it matches header_regex */
		إذا (regexec(&regex, sb.buf, 0, عدم, 0)) {
			ret = 0;
			اذهب پورا;
		}
	}

پورا:
	regfree(&regex);
	strbuf_release(&sb);
	أرجع ret;
}

/**
 * Attempts to detect the patch_format of the patches contained in `paths`,
 * returning the PATCH_FORMAT_* enum value. Returns PATCH_FORMAT_UNKNOWN if
 * detection fails.
 */
ساكن صحيح detect_patch_format(ثابت حرف **paths)
{
	تعداد patch_format ret = PATCH_FORMAT_UNKNOWN;
	بنية strbuf l1 = STRBUF_INIT;
	بنية strbuf l2 = STRBUF_INIT;
	بنية strbuf l3 = STRBUF_INIT;
	FILE *fp;

	/*
	 * We default to mbox format if input is from stdin and for directories
	 */
	إذا (!*paths || !strcmp(*paths, "-") || is_directory(*paths))
		أرجع PATCH_FORMAT_MBOX;

	/*
	 * Otherwise, check the first few lines of the first patch, starting
	 * from the first non-blank line, to try to detect its format.
	 */

	fp = xfopen(*paths, "r");

	طالما (!strbuf_getline(&l1, fp)) {
		إذا (l1.len)
			اكسر;
	}

	إذا (starts_with(l1.buf, "From ") || starts_with(l1.buf, "From: ")) {
		ret = PATCH_FORMAT_MBOX;
		اذهب پورا;
	}

	إذا (starts_with(l1.buf, "# This series applies on GIT commit")) {
		ret = PATCH_FORMAT_STGIT_SERIES;
		اذهب پورا;
	}

	إذا (!strcmp(l1.buf, "# HG changeset patch")) {
		ret = PATCH_FORMAT_HG;
		اذهب پورا;
	}

	strbuf_getline(&l2, fp);
	strbuf_getline(&l3, fp);

	/*
	 * If the second line is empty and the third is a From, Author or Date
	 * entry, this is likely an StGit patch.
	 */
	إذا (l1.len && !l2.len &&
		(starts_with(l3.buf, "From:") ||
		 starts_with(l3.buf, "Author:") ||
		 starts_with(l3.buf, "Date:"))) {
		ret = PATCH_FORMAT_STGIT;
		اذهب پورا;
	}

	إذا (l1.len && is_mail(fp)) {
		ret = PATCH_FORMAT_MBOX;
		اذهب پورا;
	}

پورا:
	fclose(fp);
	strbuf_release(&l1);
	strbuf_release(&l2);
	strbuf_release(&l3);
	أرجع ret;
}

/**
 * Splits out individual email patches from `paths`, where each path is either
 * a mbox file or a Maildir. Returns 0 on success, -1 on failure.
 */
ساكن صحيح ونڈو_mail_mbox(بنية am_state *state, ثابت حرف **paths,
				صحيح keep_cr, صحيح mboxrd)
{
	بنية بچہ_process cp = بچہ_PROCESS_INIT;
	بنية strbuf آخری = STRBUF_INIT;
	صحيح ret;

	cp.git_cmd = 1;
	strvec_ڈالو(&cp.دلیلاں, "mailsplit");
	strvec_pushf(&cp.دلیلاں, "-d%d", state->prec);
	strvec_pushf(&cp.دلیلاں, "-o%s", state->dir);
	strvec_ڈالو(&cp.دلیلاں, "-b");
	إذا (keep_cr)
		strvec_ڈالو(&cp.دلیلاں, "--keep-cr");
	إذا (mboxrd)
		strvec_ڈالو(&cp.دلیلاں, "--mboxrd");
	strvec_ڈالو(&cp.دلیلاں, "--");
	strvec_pushv(&cp.دلیلاں, paths);

	ret = capture_command(&cp, &آخری, 8);
	إذا (ret)
		اذهب باہر_نکلو;

	state->cur = 1;
	state->آخری = strtol(آخری.buf, عدم, 10);

باہر_نکلو:
	strbuf_release(&آخری);
	أرجع ret ? -1 : 0;
}

/**
 * Callback signature for split_mail_conv(). The foreign patch should be
 * read from `in`, and the converted patch (in RFC2822 mail format) should be
 * written to `out`. Return 0 on success, or -1 on failure.
 */
تعريفنوع صحيح (*mail_conv_fn)(FILE *باہرلا, FILE *وچ, صحيح keep_cr);

/**
 * Calls `fn` for each file in `paths` to convert the foreign patch to the
 * RFC2822 mail format suitable for parsing with git-mailinfo.
 *
 * Returns 0 on success, -1 on failure.
 */
ساكن صحيح ونڈو_mail_conv(mail_conv_fn fn, بنية am_state *state,
			ثابت حرف **paths, صحيح keep_cr)
{
	ساكن ثابت حرف *stdin_only[] = {"-", عدم};
	صحيح i;

	إذا (!*paths)
		paths = stdin_only;

	لكل (i = 0; *paths; paths++, i++) {
		FILE *وچ, *باہرلا;
		ثابت حرف *mail;
		صحيح ret;

		إذا (!strcmp(*paths, "-"))
			وچ = stdin;
		وإلا
			وچ = fopen(*paths, "r");

		إذا (!وچ)
			أرجع error_errno(_("could not open '%s' for reading"),
					   *paths);

		mail = mkpath("%s/%0*d", state->dir, state->prec, i + 1);

		باہرلا = fopen(mail, "w");
		إذا (!باہرلا) {
			إذا (وچ != stdin)
				fclose(وچ);
			أرجع error_errno(_("could not open '%s' for writing"),
					   mail);
		}

		ret = fn(باہرلا, وچ, keep_cr);

		fclose(باہرلا);
		إذا (وچ != stdin)
			fclose(وچ);

		إذا (ret)
			أرجع error(_("could not parse patch '%s'"), *paths);
	}

	state->cur = 1;
	state->آخری = i;
	أرجع 0;
}

/**
 * A split_mail_conv() callback that converts an StGit patch to an RFC2822
 * message suitable for parsing with git-mailinfo.
 */
ساكن صحيح stgit_patch_منزل_mail(FILE *باہرلا, FILE *وچ, صحيح keep_cr بن_ورتیا)
{
	بنية strbuf sb = STRBUF_INIT;
	صحيح subject_printed = 0;

	طالما (!strbuf_getline_lf(&sb, وچ)) {
		ثابت حرف *str;

		إذا (str_isspace(sb.buf))
			استمر;
		وإلا إذا (skip_اگیتر(sb.buf, "Author:", &str))
			fprintf(باہرلا, "From:%s\n", str);
		وإلا إذا (starts_with(sb.buf, "From") || starts_with(sb.buf, "Date"))
			fprintf(باہرلا, "%s\n", sb.buf);
		وإلا إذا (!subject_printed) {
			fprintf(باہرلا, "Subject: %s\n", sb.buf);
			subject_printed = 1;
		} وإلا {
			fprintf(باہرلا, "\n%s\n", sb.buf);
			اكسر;
		}
	}

	strbuf_reset(&sb);
	طالما (strbuf_fread(&sb, 8192, وچ) > 0) {
		fwrite(sb.buf, 1, sb.len, باہرلا);
		strbuf_reset(&sb);
	}

	strbuf_release(&sb);
	أرجع 0;
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
ساكن صحيح ونڈو_mail_stgit_series(بنية am_state *state, ثابت حرف **paths,
					صحيح keep_cr)
{
	ثابت حرف *series_dir;
	حرف *series_dir_buf;
	FILE *fp;
	بنية strvec patches = STRVEC_INIT;
	بنية strbuf sb = STRBUF_INIT;
	صحيح ret;

	إذا (!paths[0] || paths[1])
		أرجع error(_("Only one StGIT patch series can be applied at once"));

	series_dir_buf = xstrdup(*paths);
	series_dir = dirname(series_dir_buf);

	fp = fopen(*paths, "r");
	إذا (!fp) {
		حرر(series_dir_buf);
		أرجع error_errno(_("could not open '%s' for reading"), *paths);
	}

	طالما (!strbuf_getline_lf(&sb, fp)) {
		إذا (*sb.buf == '#')
			استمر; /* skip comment lines */

		strvec_ڈالو(&patches, mkpath("%s/%s", series_dir, sb.buf));
	}

	fclose(fp);
	strbuf_release(&sb);
	حرر(series_dir_buf);

	ret = ونڈو_mail_conv(stgit_patch_منزل_mail, state, patches.v, keep_cr);

	strvec_clear(&patches);
	أرجع ret;
}

/**
 * A split_patches_conv() callback that converts a mercurial patch to a RFC2822
 * message suitable for parsing with git-mailinfo.
 */
ساكن صحيح hg_patch_منزل_mail(FILE *باہرلا, FILE *وچ, صحيح keep_cr بن_ورتیا)
{
	بنية strbuf sb = STRBUF_INIT;
	صحيح rc = 0;

	طالما (!strbuf_getline_lf(&sb, وچ)) {
		ثابت حرف *str;

		إذا (skip_اگیتر(sb.buf, "# User ", &str))
			fprintf(باہرلا, "From: %s\n", str);
		وإلا إذا (skip_اگیتر(sb.buf, "# Date ", &str)) {
			timestamp_t timestamp;
			لما tz, tz2;
			حرف *سرا;

			errno = 0;
			timestamp = parse_timestamp(str, &سرا, 10);
			إذا (errno) {
				rc = error(_("invalid timestamp"));
				اذهب باہر_نکلو;
			}

			إذا (!skip_اگیتر(سرا, " ", &str)) {
				rc = error(_("invalid Date line"));
				اذهب باہر_نکلو;
			}

			errno = 0;
			tz = strtol(str, &سرا, 10);
			إذا (errno) {
				rc = error(_("invalid timezone offset"));
				اذهب باہر_نکلو;
			}

			إذا (*سرا) {
				rc = error(_("invalid Date line"));
				اذهب باہر_نکلو;
			}

			/*
			 * mercurial's timezone is in seconds west of UTC,
			 * however git's timezone is in hours + minutes east of
			 * UTC. Convert it.
			 */
			tz2 = labs(tz) / 3600 * 100 + labs(tz) % 3600 / 60;
			إذا (tz > 0)
				tz2 = -tz2;

			fprintf(باہرلا, "Date: %s\n", show_date(timestamp, tz2, DATE_منوال(RFC2822)));
		} وإلا إذا (starts_with(sb.buf, "# ")) {
			استمر;
		} وإلا {
			fprintf(باہرلا, "\n%s\n", sb.buf);
			اكسر;
		}
	}

	strbuf_reset(&sb);
	طالما (strbuf_fread(&sb, 8192, وچ) > 0) {
		fwrite(sb.buf, 1, sb.len, باہرلا);
		strbuf_reset(&sb);
	}
باہر_نکلو:
	strbuf_release(&sb);
	أرجع rc;
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
ساكن صحيح ونڈو_mail(بنية am_state *state, تعداد patch_format patch_format,
			ثابت حرف **paths, صحيح keep_cr)
{
	إذا (keep_cr < 0) {
		keep_cr = 0;
		repo_config_get_bool(the_repository, "am.keepcr", &keep_cr);
	}

	بدل (patch_format) {
	حالة PATCH_FORMAT_MBOX:
		أرجع ونڈو_mail_mbox(state, paths, keep_cr, 0);
	حالة PATCH_FORMAT_STGIT:
		أرجع ونڈو_mail_conv(stgit_patch_منزل_mail, state, paths, keep_cr);
	حالة PATCH_FORMAT_STGIT_SERIES:
		أرجع ونڈو_mail_stgit_series(state, paths, keep_cr);
	حالة PATCH_FORMAT_HG:
		أرجع ونڈو_mail_conv(hg_patch_منزل_mail, state, paths, keep_cr);
	حالة PATCH_FORMAT_MBOXRD:
		أرجع ونڈو_mail_mbox(state, paths, keep_cr, 1);
	افتراضي:
		BUG("invalid patch_format");
	}
	أرجع -1;
}

/**
 * Setup a new am session for applying patches
 */
ساكن فراغ am_تیاری(بنية am_state *state, تعداد patch_format patch_format,
			ثابت حرف **paths, صحيح keep_cr)
{
	بنية object_id curr_سر;
	ثابت حرف *str;
	بنية strbuf sb = STRBUF_INIT;

	إذا (!patch_format)
		patch_format = detect_patch_format(paths);

	إذا (!patch_format) {
		fprintf_ln(stderr, _("Patch format detection failed."));
		die(عدم);
	}

	إذا (mkdir(state->dir, 0777) < 0 && errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), state->dir);
	refs_delete_ref(get_main_ref_store(the_repository), عدم,
			"REBASE_HEAD", عدم, REF_NO_DEREF);

	إذا (ونڈو_mail(state, patch_format, paths, keep_cr) < 0) {
		am_destroy(state);
		die(_("Failed to split patches."));
	}

	إذا (state->rebasing)
		state->threeway = 1;

	write_state_bool(state, "threeway", state->threeway);
	write_state_bool(state, "quiet", state->quiet);
	write_state_bool(state, "sign", state->signoff);
	write_state_bool(state, "utf8", state->utf8);

	إذا (state->allow_rerere_autoupdate)
		write_state_bool(state, "rerere-autoupdate",
			 state->allow_rerere_autoupdate == RERERE_AUTOUPDATE);

	بدل (state->keep) {
	حالة KEEP_FALSE:
		str = "f";
		اكسر;
	حالة KEEP_TRUE:
		str = "t";
		اكسر;
	حالة KEEP_NON_PATCH:
		str = "b";
		اكسر;
	افتراضي:
		BUG("invalid value for state->keep");
	}

	write_state_متن(state, "keep", str);
	write_state_bool(state, "messageid", state->سنیہا_id);

	بدل (state->scissors) {
	حالة SCISSORS_UNSET:
		str = "";
		اكسر;
	حالة SCISSORS_FALSE:
		str = "f";
		اكسر;
	حالة SCISSORS_TRUE:
		str = "t";
		اكسر;
	افتراضي:
		BUG("invalid value for state->scissors");
	}
	write_state_متن(state, "scissors", str);

	بدل (state->quoted_cr) {
	حالة quoted_cr_unset:
		str = "";
		اكسر;
	حالة quoted_cr_nowarn:
		str = "nowarn";
		اكسر;
	حالة quoted_cr_warn:
		str = "warn";
		اكسر;
	حالة quoted_cr_strip:
		str = "strip";
		اكسر;
	افتراضي:
		BUG("invalid value for state->quoted_cr");
	}
	write_state_متن(state, "quoted-cr", str);

	sq_quote_argv(&sb, state->git_apply_opts.v);
	write_state_متن(state, "apply-opt", sb.buf);

	إذا (state->rebasing)
		write_state_متن(state, "rebasing", "");
	وإلا
		write_state_متن(state, "applying", "");

	إذا (!repo_get_oid(the_repository, "HEAD", &curr_سر)) {
		write_state_متن(state, "abort-safety", oid_منزل_hex(&curr_سر));
		إذا (!state->rebasing)
			refs_update_ref(get_main_ref_store(the_repository),
					"am", "ORIG_HEAD", &curr_سر, عدم,
					0,
					UPDATE_REFS_DIE_ON_ERR);
	} وإلا {
		write_state_متن(state, "abort-safety", "");
		إذا (!state->rebasing)
			refs_delete_ref(get_main_ref_store(the_repository),
					عدم, "ORIG_HEAD", عدم, 0);
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
ساكن فراغ am_اگلا(بنية am_state *state)
{
	بنية object_id سر;

	FREE_AND_NULL(state->author_name);
	FREE_AND_NULL(state->author_email);
	FREE_AND_NULL(state->author_date);
	FREE_AND_NULL(state->پیغام);
	state->پیغام_len = 0;

	unlink(am_رستہ(state, "author-script"));
	unlink(am_رستہ(state, "final-commit"));

	oidclr(&state->orig_commit, the_repository->hash_algo);
	unlink(am_رستہ(state, "original-commit"));
	refs_delete_ref(get_main_ref_store(the_repository), عدم,
			"REBASE_HEAD", عدم, REF_NO_DEREF);

	إذا (!repo_get_oid(the_repository, "HEAD", &سر))
		write_state_متن(state, "abort-safety", oid_منزل_hex(&سر));
	وإلا
		write_state_متن(state, "abort-safety", "");

	state->cur++;
	write_state_گنتی(state, "next", state->cur);
}

/**
 * Returns the filename of the current patch email.
 */
ساكن ثابت حرف *msgnum(ثابت بنية am_state *state)
{
	ساكن بنية strbuf sb = STRBUF_INIT;

	strbuf_reset(&sb);
	strbuf_addf(&sb, "%0*d", state->prec, state->cur);

	أرجع sb.buf;
}

/**
 * Dies with a user-friendly message on how to proceed after resolving the
 * problem. This message can be overridden with state->resolvemsg.
 */
ساكن فراغ NORETURN die_user_resolve(ثابت بنية am_state *state)
{
	إذا (state->resolvemsg) {
		advise_if_enabled(ADVICE_ملاؤ_CONFLICT, "%s", state->resolvemsg);
	} وإلا {
		ثابت حرف *cmdline = state->interactive ? "git am -i" : "git am";
		بنية strbuf sb = STRBUF_INIT;

		strbuf_addf(&sb, _("When you have resolved this problem, run \"%s --continue\".\n"), cmdline);
		strbuf_addf(&sb, _("If you prefer to skip this patch, run \"%s --skip\" instead.\n"), cmdline);

		إذا (advice_enabled(ADVICE_AM_WORK_DIR) &&
		    is_خالی_تھاواں_or_missing_file(am_رستہ(state, "patch")) &&
		    !repo_اشاریہ_has_changes(the_repository, عدم, عدم))
			strbuf_addf(&sb, _("To record the empty patch as an empty commit, run \"%s --allow-empty\".\n"), cmdline);

		strbuf_addf(&sb, _("To restore the original branch and stop patching, run \"%s --abort\"."), cmdline);

		advise_if_enabled(ADVICE_ملاؤ_CONFLICT, "%s", sb.buf);
		strbuf_release(&sb);
	}

	die(عدم);
}

/**
 * Appends signoff to the "msg" field of the am_state.
 */
ساكن فراغ am_append_signoff(بنية am_state *state)
{
	بنية strbuf sb = STRBUF_INIT;

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
ساكن صحيح parse_mail(بنية am_state *state, ثابت حرف *mail)
{
	FILE *fp;
	بنية strbuf sb = STRBUF_INIT;
	بنية strbuf پیغام = STRBUF_INIT;
	بنية strbuf author_name = STRBUF_INIT;
	بنية strbuf author_date = STRBUF_INIT;
	بنية strbuf author_email = STRBUF_INIT;
	صحيح ret = 0;
	بنية mailinfo mi;

	تیاری_mailinfo(the_repository, &mi);

	إذا (state->utf8)
		mi.metainfo_charset = get_commit_output_encoding();
	وإلا
		mi.metainfo_charset = عدم;

	بدل (state->keep) {
	حالة KEEP_FALSE:
		اكسر;
	حالة KEEP_TRUE:
		mi.keep_subject = 1;
		اكسر;
	حالة KEEP_NON_PATCH:
		mi.keep_non_patch_brackets_وچ_subject = 1;
		اكسر;
	افتراضي:
		BUG("invalid value for state->keep");
	}

	إذا (state->سنیہا_id)
		mi.جوڑو_سنیہا_id = 1;

	بدل (state->scissors) {
	حالة SCISSORS_UNSET:
		اكسر;
	حالة SCISSORS_FALSE:
		mi.use_scissors = 0;
		اكسر;
	حالة SCISSORS_TRUE:
		mi.use_scissors = 1;
		اكسر;
	افتراضي:
		BUG("invalid value for state->scissors");
	}

	بدل (state->quoted_cr) {
	حالة quoted_cr_unset:
		اكسر;
	حالة quoted_cr_nowarn:
	حالة quoted_cr_warn:
	حالة quoted_cr_strip:
		mi.quoted_cr = state->quoted_cr;
		اكسر;
	افتراضي:
		BUG("invalid value for state->quoted_cr");
	}

	mi.داخلہ = xfopen(mail, "r");
	mi.output = xfopen(am_رستہ(state, "info"), "w");
	إذا (mailinfo(&mi, am_رستہ(state, "msg"), am_رستہ(state, "patch")))
		die("could not parse patch");

	fclose(mi.داخلہ);
	fclose(mi.output);

	إذا (mi.format_flowed)
		warning(_("Patch sent with format=flowed; "
			  "space at the end of lines might be lost."));

	/* Extract message and author information */
	fp = xfopen(am_رستہ(state, "info"), "r");
	طالما (!strbuf_getline_lf(&sb, fp)) {
		ثابت حرف *x;

		إذا (skip_اگیتر(sb.buf, "Subject: ", &x)) {
			إذا (پیغام.len)
				strbuf_addch(&پیغام, '\n');
			strbuf_addstr(&پیغام, x);
		} وإلا إذا (skip_اگیتر(sb.buf, "Author: ", &x))
			strbuf_addstr(&author_name, x);
		وإلا إذا (skip_اگیتر(sb.buf, "Email: ", &x))
			strbuf_addstr(&author_email, x);
		وإلا إذا (skip_اگیتر(sb.buf, "Date: ", &x))
			strbuf_addstr(&author_date, x);
	}
	fclose(fp);

	/* Skip pine's internal folder data */
	إذا (!strcmp(author_name.buf, "Mail System Internal Data")) {
		ret = 1;
		اذهب انت;
	}

	strbuf_addstr(&پیغام, "\n\n");
	strbuf_addbuf(&پیغام, &mi.log_سنیہا);
	strbuf_stripspace(&پیغام, عدم);

	أكد(!state->author_name);
	state->author_name = strbuf_detach(&author_name, عدم);

	أكد(!state->author_email);
	state->author_email = strbuf_detach(&author_email, عدم);

	أكد(!state->author_date);
	state->author_date = strbuf_detach(&author_date, عدم);

	أكد(!state->پیغام);
	state->پیغام = strbuf_detach(&پیغام, &state->پیغام_len);

انت:
	strbuf_release(&پیغام);
	strbuf_release(&author_date);
	strbuf_release(&author_email);
	strbuf_release(&author_name);
	strbuf_release(&sb);
	clear_mailinfo(&mi);
	أرجع ret;
}

/**
 * Sets commit_id to the commit hash where the mail was generated from.
 * Returns 0 on success, -1 on failure.
 */
ساكن صحيح get_mail_commit_oid(بنية object_id *commit_id, ثابت حرف *mail)
{
	بنية strbuf sb = STRBUF_INIT;
	FILE *fp = xfopen(mail, "r");
	ثابت حرف *x;
	صحيح ret = 0;

	إذا (strbuf_getline_lf(&sb, fp) ||
	    !skip_اگیتر(sb.buf, "From ", &x) ||
	    get_oid_hex(x, commit_id) < 0)
		ret = -1;

	strbuf_release(&sb);
	fclose(fp);
	أرجع ret;
}

/**
 * Sets state->msg, state->author_name, state->author_email, state->author_date
 * to the commit's respective info.
 */
ساكن فراغ get_commit_info(بنية am_state *state, بنية commit *commit)
{
	ثابت حرف *بفر, *ident_سطر, *پیغام;
	ناپ_t ident_len;
	بنية ident_ونڈو id;

	بفر = repo_logmsg_reencode(the_repository, commit, عدم,
				      get_commit_output_encoding());

	ident_سطر = لبھو_commit_سرخی(بفر, "author", &ident_len);
	إذا (!ident_سطر)
		die(_("missing author line in commit %s"),
		      oid_منزل_hex(&commit->object.oid));
	إذا (ونڈو_ident_سطر(&id, ident_سطر, ident_len) < 0)
		die(_("invalid ident line: %.*s"), (صحيح)ident_len, ident_سطر);

	أكد(!state->author_name);
	إذا (id.name_begin)
		state->author_name =
			xmemdupz(id.name_begin, id.name_سرا - id.name_begin);
	وإلا
		state->author_name = xstrdup("");

	أكد(!state->author_email);
	إذا (id.mail_begin)
		state->author_email =
			xmemdupz(id.mail_begin, id.mail_سرا - id.mail_begin);
	وإلا
		state->author_email = xstrdup("");

	أكد(!state->author_date);
	state->author_date = xstrdup(show_ident_date(&id, DATE_منوال(عام_2)));

	أكد(!state->پیغام);
	پیغام = لڑی_کھوج(بفر, "\n\n");
	إذا (!پیغام)
		die(_("unable to parse commit %s"), oid_منزل_hex(&commit->object.oid));
	state->پیغام = xstrdup(پیغام + 2);
	state->پیغام_len = لڑی_لمبائی(state->پیغام);
	repo_unuse_commit_بفر(the_repository, commit, بفر);
}

/**
 * Writes `commit` as a patch to the state directory's "patch" file.
 */
ساكن فراغ write_commit_patch(ثابت بنية am_state *state, بنية commit *commit)
{
	بنية rev_info rev_info;
	FILE *fp;

	fp = xfopen(am_رستہ(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, عدم);
	rev_info.diff = 1;
	rev_info.abbrev = 0;
	rev_info.disable_stdin = 1;
	rev_info.show_جڑ_diff = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.flags.ثنائی = 1;
	rev_info.diffopt.flags.بھریا_اشاریہ = 1;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	جوڑو_pending_object(&rev_info, &commit->object, "");
	diff_تیاری_پورا(&rev_info.diffopt);
	log_درخت_commit(&rev_info, commit);
	release_revisions(&rev_info);
}

/**
 * Writes the diff of the index against HEAD as a patch to the state
 * directory's "patch" file.
 */
ساكن فراغ write_اشاریہ_patch(ثابت بنية am_state *state)
{
	بنية درخت *درخت;
	بنية object_id سر;
	بنية rev_info rev_info;
	FILE *fp;

	إذا (!repo_get_oid(the_repository, "HEAD", &سر)) {
		بنية commit *commit = lookup_commit_or_die(&سر, "HEAD");
		درخت = repo_get_commit_درخت(the_repository, commit);
	} وإلا
		درخت = lookup_درخت(the_repository,
				   the_repository->hash_algo->خالی_تھاواں_درخت);

	fp = xfopen(am_رستہ(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, عدم);
	rev_info.diff = 1;
	rev_info.disable_stdin = 1;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	جوڑو_pending_object(&rev_info, &درخت->object, "");
	diff_تیاری_پورا(&rev_info.diffopt);
	لگاتار_diff_اشاریہ(&rev_info, DIFF_اشاریہ_CACHED);
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
ساكن صحيح parse_mail_rebase(بنية am_state *state, ثابت حرف *mail)
{
	بنية commit *commit;
	بنية object_id commit_oid;

	إذا (get_mail_commit_oid(&commit_oid, mail) < 0)
		die(_("could not parse %s"), mail);

	commit = lookup_commit_or_die(&commit_oid, mail);

	get_commit_info(state, commit);

	write_commit_patch(state, commit);

	oidcpy(&state->orig_commit, &commit_oid);
	write_state_متن(state, "original-commit", oid_منزل_hex(&commit_oid));
	refs_update_ref(get_main_ref_store(the_repository), "am",
			"REBASE_HEAD", &commit_oid,
			عدم, REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);

	أرجع 0;
}

/**
 * Applies current patch with git-apply. Returns 0 on success, -1 otherwise. If
 * `index_file` is not NULL, the patch will be applied to that index.
 */
ساكن صحيح لگاتار_apply(ثابت بنية am_state *state, ثابت حرف *اشاریہ_file)
{
	بنية strvec apply_paths = STRVEC_INIT;
	بنية strvec apply_opts = STRVEC_INIT;
	بنية apply_state apply_state;
	صحيح res, opts_بایاں;
	صحيح force_apply = 0;
	صحيح options = 0;
	ثابت حرف **apply_argv;

	إذا (init_apply_state(&apply_state, the_repository, عدم))
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
					عدم);

	إذا (opts_بایاں != 0)
		die("unknown option passed through to git apply");

	إذا (اشاریہ_file) {
		apply_state.اشاریہ_file = اشاریہ_file;
		apply_state.cached = 1;
	} وإلا
		apply_state.check_اشاریہ = 1;

	/*
	 * If we are allowed to fall back on 3-way merge, don't give false
	 * errors during the initial attempt.
	 */
	إذا (state->threeway && !اشاریہ_file)
		apply_state.apply_verbosity = verbosity_silent;

	إذا (check_apply_state(&apply_state, force_apply))
		BUG("check_apply_state() failed");

	strvec_ڈالو(&apply_paths, am_رستہ(state, "patch"));

	res = apply_all_patches(&apply_state, apply_paths.nr, apply_paths.v, options);

	strvec_clear(&apply_paths);
	strvec_clear(&apply_opts);
	clear_apply_state(&apply_state);
	حرر(apply_argv);

	إذا (res)
		أرجع res;

	إذا (اشاریہ_file) {
		/* Reload index as apply_all_patches() will have modified it. */
		discard_اشاریہ(the_repository->اشاریہ);
		read_اشاریہ_from(the_repository->اشاریہ, اشاریہ_file,
				repo_get_git_dir(the_repository));
	}

	أرجع 0;
}

/**
 * Builds an index that contains just the blobs needed for a 3way merge.
 */
ساكن صحيح build_fake_ancestor(ثابت بنية am_state *state, ثابت حرف *اشاریہ_file)
{
	بنية بچہ_process cp = بچہ_PROCESS_INIT;

	cp.git_cmd = 1;
	strvec_ڈالو(&cp.دلیلاں, "apply");
	strvec_pushv(&cp.دلیلاں, state->git_apply_opts.v);
	strvec_pushf(&cp.دلیلاں, "--build-fake-ancestor=%s", اشاریہ_file);
	strvec_ڈالو(&cp.دلیلاں, am_رستہ(state, "patch"));

	إذا (لگاتار_command(&cp))
		أرجع -1;

	أرجع 0;
}

/**
 * Attempt a threeway merge, using index_path as the temporary index.
 */
ساكن صحيح fall_پٹھ_threeway(ثابت بنية am_state *state, ثابت حرف *اشاریہ_رستہ)
{
	بنية object_id their_درخت, our_درخت;
	بنية object_id bases[1] = { 0 };
	بنية ملاؤ_options o;
	بنية commit *نتیجہ;
	حرف *their_درخت_name;

	إذا (repo_get_oid(the_repository, "HEAD", &our_درخت) < 0)
		oidcpy(&our_درخت, the_hash_algo->خالی_تھاواں_درخت);

	إذا (build_fake_ancestor(state, اشاریہ_رستہ))
		أرجع error("could not build fake ancestor");

	discard_اشاریہ(the_repository->اشاریہ);
	read_اشاریہ_from(the_repository->اشاریہ, اشاریہ_رستہ, repo_get_git_dir(the_repository));

	إذا (write_اشاریہ_as_درخت(&bases[0], the_repository->اشاریہ, اشاریہ_رستہ, 0, عدم))
		أرجع error(_("Repository lacks necessary blobs to fall back on 3-way merge."));

	say(state, stdout, _("Using index info to reconstruct a base tree..."));

	إذا (!state->quiet) {
		/*
		 * List paths that needed 3-way fallback, so that the user can
		 * review them with extra care to spot mismerges.
		 */
		بنية rev_info rev_info;

		repo_init_revisions(the_repository, &rev_info, عدم);
		rev_info.diffopt.output_format = DIFF_FORMAT_NAME_حالت;
		rev_info.diffopt.filter |= diff_filter_ثنائی_ہندسہ('A');
		rev_info.diffopt.filter |= diff_filter_ثنائی_ہندسہ('M');
		جوڑو_pending_oid(&rev_info, "HEAD", &our_درخت, 0);
		diff_تیاری_پورا(&rev_info.diffopt);
		لگاتار_diff_اشاریہ(&rev_info, DIFF_اشاریہ_CACHED);
		release_revisions(&rev_info);
	}

	إذا (لگاتار_apply(state, اشاریہ_رستہ))
		أرجع error(_("Did you hand edit your patch?\n"
				"It does not apply to blobs recorded in its index."));

	إذا (write_اشاریہ_as_درخت(&their_درخت, the_repository->اشاریہ, اشاریہ_رستہ, 0, عدم))
		أرجع error("could not write tree");

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

	إذا (state->quiet)
		o.verbosity = 0;

	إذا (ملاؤ_ort_generic(&o, &our_درخت, &their_درخت, 1, bases, &نتیجہ)) {
		repo_rerere(the_repository, state->allow_rerere_autoupdate);
		حرر(their_درخت_name);
		أرجع error(_("Failed to merge in the changes."));
	}

	حرر(their_درخت_name);
	أرجع 0;
}

/**
 * Commits the current index with state->msg as the commit message and
 * state->author_name, state->author_email and state->author_date as the author
 * information.
 */
ساكن فراغ do_commit(ثابت بنية am_state *state)
{
	بنية object_id درخت, ماپا, commit;
	ثابت بنية object_id *old_oid;
	بنية commit_list *parents = عدم;
	ثابت حرف *reflog_پیغام, *author, *committer = عدم;
	بنية strbuf sb = STRBUF_INIT;

	إذا (!state->no_verify && لگاتار_hooks(the_repository, "pre-applypatch"))
		باہر_نکلو(1);

	إذا (write_اشاریہ_as_درخت(&درخت, the_repository->اشاریہ,
				repo_get_اشاریہ_file(the_repository),
				0, عدم))
		die(_("git write-tree failed to write a tree"));

	إذا (!repo_get_oid_commit(the_repository, "HEAD", &ماپا)) {
		old_oid = &ماپا;
		commit_list_داخل_کرو(lookup_commit(the_repository, &ماپا),
				   &parents);
	} وإلا {
		old_oid = عدم;
		say(state, stderr, _("applying to an empty history"));
	}

	author = fmt_ident(state->author_name, state->author_email,
		WANT_AUTHOR_IDENT,
			state->ignore_date ? عدم : state->author_date,
			IDENT_STRICT);

	إذا (state->committer_date_is_author_date)
		committer = fmt_ident(getenv("GIT_COMMITTER_NAME"),
				      getenv("GIT_COMMITTER_EMAIL"),
				      WANT_COMMITTER_IDENT,
				      state->ignore_date ? عدم
							 : state->author_date,
				      IDENT_STRICT);

	إذا (commit_درخت_extended(state->پیغام, state->پیغام_len, &درخت, parents,
				 &commit, author, committer, state->sign_commit,
				 عدم))
		die(_("failed to write commit object"));

	reflog_پیغام = getenv("GIT_REFLOG_ACTION");
	إذا (!reflog_پیغام)
		reflog_پیغام = "am";

	strbuf_addf(&sb, "%s: %.*s", reflog_پیغام, linelen(state->پیغام),
			state->پیغام);

	refs_update_ref(get_main_ref_store(the_repository), sb.buf, "HEAD",
			&commit, old_oid, 0,
			UPDATE_REFS_DIE_ON_ERR);

	إذا (state->rebasing) {
		FILE *fp = xfopen(am_رستہ(state, "rewritten"), "a");

		أكد(!is_null_oid(&state->orig_commit));
		fprintf(fp, "%s ", oid_منزل_hex(&state->orig_commit));
		fprintf(fp, "%s\n", oid_منزل_hex(&commit));
		fclose(fp);
	}

	لگاتار_hooks(the_repository, "post-applypatch");

	commit_list_free(parents);
	strbuf_release(&sb);
}

/**
 * Validates the am_state for resuming -- the "msg" and authorship fields must
 * be filled up.
 */
ساكن فراغ validate_resume_state(ثابت بنية am_state *state)
{
	إذا (!state->پیغام)
		die(_("cannot resume: %s does not exist."),
			am_رستہ(state, "final-commit"));

	إذا (!state->author_name || !state->author_email || !state->author_date)
		die(_("cannot resume: %s does not exist."),
			am_رستہ(state, "author-script"));
}

/**
 * Interactively prompt the user on whether the current patch should be
 * applied.
 *
 * Returns 0 if the user chooses to apply the patch, 1 if the user chooses to
 * skip it.
 */
ساكن صحيح do_interactive(بنية am_state *state)
{
	أكد(state->پیغام);

	لكل (;;) {
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
		إذا (!fgets(جواب, حجم(جواب), stdin))
			die("unable to read from stdin; aborting");

		إذا (*جواب == 'y' || *جواب == 'Y') {
			أرجع 0;
		} وإلا إذا (*جواب == 'a' || *جواب == 'A') {
			state->interactive = 0;
			أرجع 0;
		} وإلا إذا (*جواب == 'n' || *جواب == 'N') {
			أرجع 1;
		} وإلا إذا (*جواب == 'e' || *جواب == 'E') {
			بنية strbuf پیغام = STRBUF_INIT;

			إذا (!launch_editor(am_رستہ(state, "final-commit"), &پیغام, عدم)) {
				حرر(state->پیغام);
				state->پیغام = strbuf_detach(&پیغام, &state->پیغام_len);
			}
			strbuf_release(&پیغام);
		} وإلا إذا (*جواب == 'v' || *جواب == 'V') {
			ثابت حرف *pager = git_pager(the_repository, 1);
			بنية بچہ_process cp = بچہ_PROCESS_INIT;

			إذا (!pager)
				pager = "cat";
			prepare_pager_دلیلاں(&cp, pager);
			strvec_ڈالو(&cp.دلیلاں, am_رستہ(state, "patch"));
			لگاتار_command(&cp);
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
ساكن فراغ am_لگاتار(بنية am_state *state, صحيح resume)
{
	بنية strbuf sb = STRBUF_INIT;

	unlink(am_رستہ(state, "dirtyindex"));

	إذا (repo_refresh_and_write_اشاریہ(the_repository, REFRESH_QUIET, 0, 0,
					 عدم, عدم, عدم) < 0)
		die(_("unable to write index file"));

	إذا (repo_اشاریہ_has_changes(the_repository, عدم, &sb)) {
		write_state_bool(state, "dirtyindex", 1);
		die(_("Dirty index: cannot apply patches (dirty: %s)"), sb.buf);
	}

	strbuf_release(&sb);

	طالما (state->cur <= state->آخری) {
		ثابت حرف *mail = am_رستہ(state, msgnum(state));
		صحيح apply_حالت;
		صحيح منزل_keep;

		reset_ident_date();

		إذا (!file_exists(mail))
			اذهب اگلا;

		إذا (resume) {
			validate_resume_state(state);
		} وإلا {
			صحيح skip;

			إذا (state->rebasing)
				skip = parse_mail_rebase(state, mail);
			وإلا
				skip = parse_mail(state, mail);

			إذا (skip)
				اذهب اگلا; /* mail should be skipped */

			إذا (state->signoff)
				am_append_signoff(state);

			write_author_script(state);
			write_commit_پیغام(state);
		}

		إذا (state->interactive && do_interactive(state))
			اذهب اگلا;

		منزل_keep = 0;
		إذا (is_خالی_تھاواں_or_missing_file(am_رستہ(state, "patch"))) {
			بدل (state->خالی_تھاواں_type) {
			حالة DROP_خالی_تھاواں_COMMIT:
				say(state, stdout, _("Skipping: %.*s"), linelen(state->پیغام), state->پیغام);
				اذهب اگلا;
				اكسر;
			حالة KEEP_خالی_تھاواں_COMMIT:
				منزل_keep = 1;
				say(state, stdout, _("Creating an empty commit: %.*s"),
					linelen(state->پیغام), state->پیغام);
				اكسر;
			حالة STOP_ON_خالی_تھاواں_COMMIT:
				printf_ln(_("Patch is empty."));
				die_user_resolve(state);
				اكسر;
			}
		}

		إذا (لگاتار_applypatch_پیغام_hook(state))
			باہر_نکلو(1);
		إذا (منزل_keep)
			اذهب commit;

		say(state, stdout, _("Applying: %.*s"), linelen(state->پیغام), state->پیغام);

		apply_حالت = لگاتار_apply(state, عدم);

		إذا (apply_حالت && state->threeway) {
			بنية strbuf sb = STRBUF_INIT;

			strbuf_addstr(&sb, am_رستہ(state, "patch-merge-index"));
			apply_حالت = fall_پٹھ_threeway(state, sb.buf);
			strbuf_release(&sb);

			/*
			 * Applying the patch to an earlier tree and merging
			 * the result may have produced the same tree as ours.
			 */
			إذا (!apply_حالت &&
			    !repo_اشاریہ_has_changes(the_repository, عدم, عدم)) {
				say(state, stdout, _("No changes -- Patch already applied."));
				اذهب اگلا;
			}
		}

		إذا (apply_حالت) {
			printf_ln(_("Patch failed at %s %.*s"), msgnum(state),
				linelen(state->پیغام), state->پیغام);

			إذا (advice_enabled(ADVICE_AM_WORK_DIR))
				advise(_("Use 'git am --show-current-patch=diff' to see the failed patch"));

			die_user_resolve(state);
		}

commit:
		do_commit(state);

اگلا:
		am_اگلا(state);

		إذا (resume)
			am_لدو(state);
		resume = 0;
	}

	إذا (!is_خالی_تھاواں_or_missing_file(am_رستہ(state, "rewritten"))) {
		أكد(state->rebasing);
		copy_notes_for_rebase(state);
		لگاتار_post_rewrite_hook(state);
	}

	/*
	 * In rebasing mode, it's up to the caller to take care of
	 * housekeeping.
	 */
	إذا (!state->rebasing) {
		am_destroy(state);
		لگاتار_auto_maintenance(the_repository, state->quiet);
	}
}

/**
 * Resume the current am session after patch application failure. The user did
 * all the hard work, and we do not have to do any patch application. Just
 * trust and commit what the user has in the index and working tree. If `allow_empty`
 * is true, commit as an empty commit when index has not changed and lacking a patch.
 */
ساكن فراغ am_resolve(بنية am_state *state, صحيح allow_خالی_تھاواں)
{
	validate_resume_state(state);

	say(state, stdout, _("Applying: %.*s"), linelen(state->پیغام), state->پیغام);

	إذا (!repo_اشاریہ_has_changes(the_repository, عدم, عدم)) {
		إذا (allow_خالی_تھاواں && is_خالی_تھاواں_or_missing_file(am_رستہ(state, "patch"))) {
			printf_ln(_("No changes - recorded it as an empty commit."));
		} وإلا {
			printf_ln(_("No changes - did you forget to use 'git add'?\n"
				    "If there is nothing left to stage, chances are that something else\n"
				    "already introduced the same changes; you might want to skip this patch."));
			die_user_resolve(state);
		}
	}

	إذا (unmerged_اشاریہ(the_repository->اشاریہ)) {
		printf_ln(_("You still have unmerged paths in your index.\n"
			"You should 'git add' each file with resolved conflicts to mark them as such.\n"
			"You might run `git rm` on a file to accept \"deleted by them\" for it."));
		die_user_resolve(state);
	}

	إذا (state->interactive) {
		write_اشاریہ_patch(state);
		إذا (do_interactive(state))
			اذهب اگلا;
	}

	repo_rerere(the_repository, 0);

	do_commit(state);

اگلا:
	am_اگلا(state);
	am_لدو(state);
	am_لگاتار(state, 0);
}

/**
 * Performs a checkout fast-forward from `head` to `remote`. If `reset` is
 * true, any unmerged entries will be discarded. Returns 0 on success, -1 on
 * failure.
 */
ساكن صحيح fast_forward_منزل(بنية درخت *سر, بنية درخت *remote, صحيح reset)
{
	بنية جندرا_file جندرا_file = جندرا_INIT;
	بنية unpack_trees_options opts;
	بنية درخت_desc t[2];

	إذا (repo_parse_درخت(the_repository, سر) || repo_parse_درخت(the_repository, remote))
		أرجع -1;

	repo_hold_locked_اشاریہ(the_repository, &جندرا_file, جندرا_DIE_ON_ERROR);

	refresh_اشاریہ(the_repository->اشاریہ, REFRESH_QUIET, عدم, عدم, عدم);

	memset(&opts, 0, حجم(opts));
	opts.سر_idx = 1;
	opts.src_اشاریہ = the_repository->اشاریہ;
	opts.dst_اشاریہ = the_repository->اشاریہ;
	opts.update = 1;
	opts.ملاؤ = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = twoway_ملاؤ;
	init_درخت_desc(&t[0], &سر->object.oid, سر->بفر, سر->ناپ);
	init_درخت_desc(&t[1], &remote->object.oid, remote->بفر, remote->ناپ);

	إذا (unpack_trees(2, t, &opts)) {
		rollback_جندرا_file(&جندرا_file);
		أرجع -1;
	}

	إذا (write_locked_اشاریہ(the_repository->اشاریہ, &جندرا_file, COMMIT_جندرا))
		die(_("unable to write new index file"));

	أرجع 0;
}

/**
 * Merges a tree into the index. The index's stat info will take precedence
 * over the merged tree's. Returns 0 on success, -1 on failure.
 */
ساكن صحيح ملاؤ_درخت(بنية درخت *درخت)
{
	بنية جندرا_file جندرا_file = جندرا_INIT;
	بنية unpack_trees_options opts;
	بنية درخت_desc t[1];

	إذا (repo_parse_درخت(the_repository, درخت))
		أرجع -1;

	repo_hold_locked_اشاریہ(the_repository, &جندرا_file, جندرا_DIE_ON_ERROR);

	memset(&opts, 0, حجم(opts));
	opts.سر_idx = 1;
	opts.src_اشاریہ = the_repository->اشاریہ;
	opts.dst_اشاریہ = the_repository->اشاریہ;
	opts.ملاؤ = 1;
	opts.fn = oneway_ملاؤ;
	init_درخت_desc(&t[0], &درخت->object.oid, درخت->بفر, درخت->ناپ);

	إذا (unpack_trees(1, t, &opts)) {
		rollback_جندرا_file(&جندرا_file);
		أرجع -1;
	}

	إذا (write_locked_اشاریہ(the_repository->اشاریہ, &جندرا_file, COMMIT_جندرا))
		die(_("unable to write new index file"));

	أرجع 0;
}

/**
 * Clean the index without touching entries that are not modified between
 * `head` and `remote`.
 */
ساكن صحيح clean_اشاریہ(ثابت بنية object_id *سر, ثابت بنية object_id *remote)
{
	بنية درخت *سر_درخت, *remote_درخت, *اشاریہ_درخت;
	بنية object_id اشاریہ;

	سر_درخت = repo_parse_درخت_اسدھا(the_repository, سر);
	إذا (!سر_درخت)
		أرجع error(_("Could not parse object '%s'."), oid_منزل_hex(سر));

	remote_درخت = repo_parse_درخت_اسدھا(the_repository, remote);
	إذا (!remote_درخت)
		أرجع error(_("Could not parse object '%s'."), oid_منزل_hex(remote));

	repo_read_اشاریہ_unmerged(the_repository);

	إذا (fast_forward_منزل(سر_درخت, سر_درخت, 1))
		أرجع -1;

	إذا (write_اشاریہ_as_درخت(&اشاریہ, the_repository->اشاریہ,
				repo_get_اشاریہ_file(the_repository),
				0, عدم))
		أرجع -1;

	اشاریہ_درخت = repo_parse_درخت_اسدھا(the_repository, &اشاریہ);
	إذا (!اشاریہ_درخت)
		أرجع error(_("Could not parse object '%s'."), oid_منزل_hex(&اشاریہ));

	إذا (fast_forward_منزل(اشاریہ_درخت, remote_درخت, 0))
		أرجع -1;

	إذا (ملاؤ_درخت(remote_درخت))
		أرجع -1;

	remove_branch_state(the_repository, 0);

	أرجع 0;
}

/**
 * Resets rerere's merge resolution metadata.
 */
ساكن فراغ am_rerere_clear(فراغ)
{
	بنية string_list ملاؤ_rr = STRING_LIST_INIT_DUP;
	rerere_clear(the_repository, &ملاؤ_rr);
	string_list_clear(&ملاؤ_rr, 1);
}

/**
 * Resume the current am session by skipping the current patch.
 */
ساكن فراغ am_skip(بنية am_state *state)
{
	بنية object_id سر;

	am_rerere_clear();

	إذا (repo_get_oid(the_repository, "HEAD", &سر))
		oidcpy(&سر, the_hash_algo->خالی_تھاواں_درخت);

	إذا (clean_اشاریہ(&سر, &سر))
		die(_("failed to clean index"));

	إذا (state->rebasing) {
		FILE *fp = xfopen(am_رستہ(state, "rewritten"), "a");

		أكد(!is_null_oid(&state->orig_commit));
		fprintf(fp, "%s ", oid_منزل_hex(&state->orig_commit));
		fprintf(fp, "%s\n", oid_منزل_hex(&سر));
		fclose(fp);
	}

	am_اگلا(state);
	am_لدو(state);
	am_لگاتار(state, 0);
}

/**
 * Returns true if it is safe to reset HEAD to the ORIG_HEAD, false otherwise.
 *
 * It is not safe to reset HEAD when:
 * 1. git-am previously failed because the index was dirty.
 * 2. HEAD has moved since git-am previously failed.
 */
ساكن صحيح محفوظ_منزل_abort(ثابت بنية am_state *state)
{
	بنية strbuf sb = STRBUF_INIT;
	بنية object_id abort_safety, سر;

	إذا (file_exists(am_رستہ(state, "dirtyindex")))
		أرجع 0;

	إذا (read_state_file(&sb, state, "abort-safety", 1) > 0) {
		إذا (get_oid_hex(sb.buf, &abort_safety))
			die(_("could not parse %s"), am_رستہ(state, "abort-safety"));
	} وإلا
		oidclr(&abort_safety, the_repository->hash_algo);
	strbuf_release(&sb);

	إذا (repo_get_oid(the_repository, "HEAD", &سر))
		oidclr(&سر, the_repository->hash_algo);

	إذا (oideq(&سر, &abort_safety))
		أرجع 1;

	warning(_("You seem to have moved HEAD since the last 'am' failure.\n"
		"Not rewinding to ORIG_HEAD"));

	أرجع 0;
}

/**
 * Aborts the current am session if it is safe to do so.
 */
ساكن فراغ am_abort(بنية am_state *state)
{
	بنية object_id curr_سر, orig_سر;
	صحيح has_curr_سر, has_orig_سر;
	حرف *curr_branch;

	إذا (!محفوظ_منزل_abort(state)) {
		am_destroy(state);
		أرجع;
	}

	am_rerere_clear();

	curr_branch = refs_resolve_refdup(get_main_ref_store(the_repository),
					  "HEAD", 0, &curr_سر, عدم);
	has_curr_سر = curr_branch && !is_null_oid(&curr_سر);
	إذا (!has_curr_سر)
		oidcpy(&curr_سر, the_hash_algo->خالی_تھاواں_درخت);

	has_orig_سر = !repo_get_oid(the_repository, "ORIG_HEAD", &orig_سر);
	إذا (!has_orig_سر)
		oidcpy(&orig_سر, the_hash_algo->خالی_تھاواں_درخت);

	إذا (clean_اشاریہ(&curr_سر, &orig_سر))
		die(_("failed to clean index"));

	إذا (has_orig_سر)
		refs_update_ref(get_main_ref_store(the_repository),
				"am --abort", "HEAD", &orig_سر,
				has_curr_سر ? &curr_سر : عدم, 0,
				UPDATE_REFS_DIE_ON_ERR);
	وإلا إذا (curr_branch)
		refs_delete_ref(get_main_ref_store(the_repository), عدم,
				curr_branch, عدم, REF_NO_DEREF);

	حرر(curr_branch);
	am_destroy(state);
}

ساكن صحيح show_patch(بنية am_state *state, تعداد resume_type resume_منوال)
{
	بنية strbuf sb = STRBUF_INIT;
	ثابت حرف *patch_رستہ;
	صحيح len;

	إذا (!is_null_oid(&state->orig_commit)) {
		بنية بچہ_process cmd = بچہ_PROCESS_INIT;

		strvec_pushl(&cmd.دلیلاں, "show", oid_منزل_hex(&state->orig_commit),
			     "--", عدم);
		cmd.git_cmd = 1;
		أرجع لگاتار_command(&cmd);
	}

	بدل (resume_منوال) {
	حالة RESUME_SHOW_PATCH_RAW:
		patch_رستہ = am_رستہ(state, msgnum(state));
		اكسر;
	حالة RESUME_SHOW_PATCH_DIFF:
		patch_رستہ = am_رستہ(state, "patch");
		اكسر;
	افتراضي:
		BUG("invalid mode for --show-current-patch");
	}

	len = strbuf_read_file(&sb, patch_رستہ, 0);
	إذا (len < 0)
		die_errno(_("failed to read '%s'"), patch_رستہ);

	تیاری_pager(the_repository);
	write_وچ_بھریا(1, sb.buf, sb.len);
	strbuf_release(&sb);
	أرجع 0;
}

/**
 * parse_options() callback that validates and sets opt->value to the
 * PATCH_FORMAT_* enum value corresponding to `arg`.
 */
ساكن صحيح parse_opt_patchformat(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	صحيح *opt_قدر = opt->قدر;

	إذا (unset)
		*opt_قدر = PATCH_FORMAT_UNKNOWN;
	وإلا إذا (!strcmp(arg, "mbox"))
		*opt_قدر = PATCH_FORMAT_MBOX;
	وإلا إذا (!strcmp(arg, "stgit"))
		*opt_قدر = PATCH_FORMAT_STGIT;
	وإلا إذا (!strcmp(arg, "stgit-series"))
		*opt_قدر = PATCH_FORMAT_STGIT_SERIES;
	وإلا إذا (!strcmp(arg, "hg"))
		*opt_قدر = PATCH_FORMAT_HG;
	وإلا إذا (!strcmp(arg, "mboxrd"))
		*opt_قدر = PATCH_FORMAT_MBOXRD;
	/*
	 * Please update $__git_patchformat in git-completion.bash
	 * when you add new options
	 */
	وإلا
		أرجع error(_("invalid value for '%s': '%s'"),
			     "--patch-format", arg);
	أرجع 0;
}

ساكن صحيح parse_opt_show_current_patch(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	صحيح *opt_قدر = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	إذا (!arg)
		*opt_قدر = opt->defval;
	وإلا إذا (!strcmp(arg, "raw"))
		*opt_قدر = RESUME_SHOW_PATCH_RAW;
	وإلا إذا (!strcmp(arg, "diff"))
		*opt_قدر = RESUME_SHOW_PATCH_DIFF;
	/*
	 * Please update $__git_showcurrentpatch in git-completion.bash
	 * when you add new options
	 */
	وإلا
		أرجع error(_("invalid value for '%s': '%s'"),
			     "--show-current-patch", arg);
	أرجع 0;
}

صحيح cmd_am(صحيح argc,
	   ثابت حرف **argv,
	   ثابت حرف *اگیتر,
	   بنية repository *repo بن_ورتیا)
{
	بنية am_state state;
	صحيح ثنائی = -1;
	صحيح keep_cr = -1;
	صحيح patch_format = PATCH_FORMAT_UNKNOWN;
	تعداد resume_type resume_منوال = RESUME_FALSE;
	صحيح وچ_ترقی;
	صحيح ret = 0;

	ثابت حرف * ثابت usage[] = {
		N_("git am [<options>] [(<mbox> | <Maildir>)...]"),
		N_("git am [<options>] (--continue | --skip | --abort)"),
		عدم
	};

	بنية option options[] = {
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
		OPT_BOOL('m', "message-id", &state.سنیہا_id,
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
		OPT_PASSTHRU_ARGV(0, "ignore-space-change", &state.git_apply_opts, عدم,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU_ARGV(0, "ignore-whitespace", &state.git_apply_opts, عدم,
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
		OPT_PASSTHRU_ARGV('C', عدم, &state.git_apply_opts, N_("n"),
			N_("pass it through git-apply"),
			0),
		OPT_PASSTHRU_ARGV('p', عدم, &state.git_apply_opts, N_("num"),
			N_("pass it through git-apply"),
			0),
		OPT_CALLBACK(0, "patch-format", &patch_format, N_("format"),
			N_("format the patch(es) are in"),
			parse_opt_patchformat),
		OPT_PASSTHRU_ARGV(0, "reject", &state.git_apply_opts, عدم,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_STRING(0, "resolvemsg", &state.resolvemsg, عدم,
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
			RESUME_ALLOW_خالی_تھاواں),
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
		OPT_CALLBACK_F(0, "empty", &state.خالی_تھاواں_type, "(stop|drop|keep)",
		  N_("how to handle empty patches"),
		  PARSE_OPT_NONEG, am_option_parse_خالی_تھاواں),
		OPT_HIDDEN_BOOL(0, "rebasing", &state.rebasing,
			N_("(internal use for git-rebase)")),
		OPT_سرا()
	};

	show_usage_with_options_if_asked(argc, argv, usage, options);

	repo_config(the_repository, git_default_config, عدم);

	am_state_init(&state);

	وچ_ترقی = am_وچ_ترقی(&state);
	إذا (وچ_ترقی)
		am_لدو(&state);

	argc = parse_options(argc, argv, اگیتر, options, usage, 0);

	إذا (ثنائی >= 0)
		fprintf_ln(stderr, _("The -b/--binary option has been a no-op for long time, and\n"
				"it will be removed. Please do not use it anymore."));

	/* Ensure a valid committer ident can be constructed */
	git_committer_info(IDENT_STRICT);

	إذا (repo_read_اشاریہ_preload(the_repository, عدم, 0) < 0)
		die(_("failed to read the index"));

	إذا (وچ_ترقی) {
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
		إذا (argc || (resume_منوال == RESUME_FALSE && !isatty(0)))
			die(_("previous rebase directory %s still exists but mbox given."),
				state.dir);

		إذا (resume_منوال == RESUME_FALSE)
			resume_منوال = RESUME_APPLY;

		إذا (state.signoff == SIGNOFF_EXPLICIT)
			am_append_signoff(&state);
	} وإلا {
		بنية strvec paths = STRVEC_INIT;
		صحيح i;

		/*
		 * Handle stray state directory in the independent-run case. In
		 * the --rebasing case, it is up to the caller to take care of
		 * stray directories.
		 */
		إذا (file_exists(state.dir) && !state.rebasing) {
			إذا (resume_منوال == RESUME_ABORT || resume_منوال == RESUME_QUIT) {
				am_destroy(&state);
				am_state_release(&state);
				أرجع 0;
			}

			die(_("Stray %s directory found.\n"
				"Use \"git am --abort\" to remove it."),
				state.dir);
		}

		إذا (resume_منوال)
			die(_("Resolve operation not in progress, we are not resuming."));

		لكل (i = 0; i < argc; i++) {
			إذا (is_absolute_رستہ(argv[i]) || !اگیتر)
				strvec_ڈالو(&paths, argv[i]);
			وإلا
				strvec_ڈالو(&paths, mkpath("%s/%s", اگیتر, argv[i]));
		}

		إذا (state.interactive && !paths.nr)
			die(_("interactive mode requires patches on the command line"));

		am_تیاری(&state, patch_format, paths.v, keep_cr);

		strvec_clear(&paths);
	}

	بدل (resume_منوال) {
	حالة RESUME_FALSE:
		am_لگاتار(&state, 0);
		اكسر;
	حالة RESUME_APPLY:
		am_لگاتار(&state, 1);
		اكسر;
	حالة RESUME_RESOLVED:
	حالة RESUME_ALLOW_خالی_تھاواں:
		am_resolve(&state, resume_منوال == RESUME_ALLOW_خالی_تھاواں ? 1 : 0);
		اكسر;
	حالة RESUME_SKIP:
		am_skip(&state);
		اكسر;
	حالة RESUME_ABORT:
		am_abort(&state);
		اكسر;
	حالة RESUME_QUIT:
		am_rerere_clear();
		am_destroy(&state);
		اكسر;
	حالة RESUME_SHOW_PATCH_RAW:
	حالة RESUME_SHOW_PATCH_DIFF:
		ret = show_patch(&state, resume_منوال);
		اكسر;
	افتراضي:
		BUG("invalid resume value");
	}

	am_state_release(&state);

	أرجع ret;
}
