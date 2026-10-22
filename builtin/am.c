/*
 * Builtin "git am"
 *
 * Based on git-am.sh by Junio C Hamano.
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "abspath.h"
#అన్తర్భావ "advice.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "editor.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "hook.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "tempfile.h"
#అన్తర్భావ "lockfile.h"
#అన్తర్భావ "cache-tree.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "unpack-trees.h"
#అన్తర్భావ "branch.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "preload-index.h"
#అన్తర్భావ "sequencer.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "merge-ort-wrappers.h"
#అన్తర్భావ "log-tree.h"
#అన్తర్భావ "notes-utils.h"
#అన్తర్భావ "rerere.h"
#అన్తర్భావ "mailinfo.h"
#అన్తర్భావ "apply.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "pager.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "pretty.h"

/**
 * Returns the length of the first line of msg.
 */
స్థిర పూర్ణాంకం linelen(స్థిరరాశి అక్షరం *సందేశం)
{
	తిరిగి strchrnul(సందేశం, '\n') - సందేశం;
}

/**
 * Returns true if `str` consists of only whitespace, false otherwise.
 */
స్థిర పూర్ణాంకం str_isspace(స్థిరరాశి అక్షరం *str)
{
	కొరకు (; *str; str++)
		ఒకవేళ (!ఖాళీనా(*str))
			తిరిగి 0;

	తిరిగి 1;
}

గణన patch_format {
	PATCH_FORMAT_UNKNOWN = 0,
	PATCH_FORMAT_MBOX,
	PATCH_FORMAT_STGIT,
	PATCH_FORMAT_STGIT_SERIES,
	PATCH_FORMAT_HG,
	PATCH_FORMAT_MBOXRD
};

గణన keep_type {
	KEEP_FALSE = 0,
	KEEP_TRUE,      /* pass -k flag to git-mailinfo */
	KEEP_NON_PATCH  /* pass -b flag to git-mailinfo */
};

గణన scissors_type {
	SCISSORS_UNSET = -1,
	SCISSORS_FALSE = 0,  /* pass --no-scissors to git-mailinfo */
	SCISSORS_TRUE        /* pass --scissors to git-mailinfo */
};

గణన signoff_type {
	SIGNOFF_FALSE = 0,
	SIGNOFF_TRUE = 1,
	SIGNOFF_EXPLICIT /* --signoff was set on the command-line */
};

గణన resume_type {
	RESUME_FALSE = 0,
	RESUME_APPLY,
	RESUME_RESOLVED,
	RESUME_SKIP,
	RESUME_ABORT,
	RESUME_QUIT,
	RESUME_SHOW_PATCH_RAW,
	RESUME_SHOW_PATCH_DIFF,
	RESUME_ALLOW_ఖాళీలు,
};

గణన ఖాళీలు_action {
	STOP_ON_ఖాళీలు_COMMIT = 0,  /* output errors and stop in the middle of an am session */
	DROP_ఖాళీలు_COMMIT,         /* skip with a notice message, unless "--quiet" has been passed */
	KEEP_ఖాళీలు_COMMIT,         /* keep recording as empty commits */
};

నిర్మాణం am_state {
	/* state directory path */
	అక్షరం *dir;

	/* current and last patch numbers, 1-indexed */
	పూర్ణాంకం cur;
	పూర్ణాంకం చివరి;

	/* commit metadata and message */
	అక్షరం *author_name;
	అక్షరం *author_email;
	అక్షరం *author_date;
	అక్షరం *సందేశం;
	కొలత_t సందేశం_len;

	/* when --rebasing, records the original commit the patch came from */
	నిర్మాణం object_id orig_commit;

	/* number of digits in patch filename */
	పూర్ణాంకం prec;

	/* various operating modes and command line options */
	పూర్ణాంకం interactive;
	పూర్ణాంకం no_verify;
	పూర్ణాంకం threeway;
	పూర్ణాంకం quiet;
	పూర్ణాంకం signoff; /* enum signoff_type */
	పూర్ణాంకం utf8;
	పూర్ణాంకం keep; /* enum keep_type */
	పూర్ణాంకం సందేశం_id;
	పూర్ణాంకం scissors; /* enum scissors_type */
	పూర్ణాంకం quoted_cr; /* enum quoted_cr_action */
	పూర్ణాంకం ఖాళీలు_type; /* enum empty_action */
	నిర్మాణం strvec git_apply_opts;
	స్థిరరాశి అక్షరం *resolvemsg;
	పూర్ణాంకం committer_date_is_author_date;
	పూర్ణాంకం ignore_date;
	పూర్ణాంకం allow_rerere_autoupdate;
	స్థిరరాశి అక్షరం *sign_commit;
	పూర్ణాంకం rebasing;
};

/**
 * Initializes am_state with the default values.
 */
స్థిర శూన్య am_state_init(నిర్మాణం am_state *state)
{
	పూర్ణాంకం gpgsign;

	memset(state, 0, పరిమాణం(*state));

	state->dir = repo_git_మార్గం(the_repository, "rebase-apply");

	state->prec = 4;

	repo_config_get_bool(the_repository, "am.threeway", &state->threeway);

	state->utf8 = 1;

	repo_config_get_bool(the_repository, "am.messageid", &state->సందేశం_id);

	state->scissors = SCISSORS_UNSET;
	state->quoted_cr = quoted_cr_unset;

	strvec_init(&state->git_apply_opts);

	ఒకవేళ (!repo_config_get_bool(the_repository, "commit.gpgsign", &gpgsign))
		state->sign_commit = gpgsign ? "" : ఖాళీ;
}

/**
 * Releases memory allocated by an am_state.
 */
స్థిర శూన్య am_state_release(నిర్మాణం am_state *state)
{
	విడుదల(state->dir);
	విడుదల(state->author_name);
	విడుదల(state->author_email);
	విడుదల(state->author_date);
	విడుదల(state->సందేశం);
	strvec_clear(&state->git_apply_opts);
}

స్థిర పూర్ణాంకం am_option_parse_quoted_cr(స్థిరరాశి నిర్మాణం option *opt,
				     స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	BUG_ON_OPT_NEG(unset);

	ఒకవేళ (mailinfo_parse_quoted_cr_action(arg, opt->విలువ) != 0)
		తిరిగి error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	తిరిగి 0;
}

స్థిర పూర్ణాంకం am_option_parse_ఖాళీలు(స్థిరరాశి నిర్మాణం option *opt,
				     స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	పూర్ణాంకం *opt_విలువ = opt->విలువ;

	BUG_ON_OPT_NEG(unset);

	ఒకవేళ (!strcmp(arg, "stop"))
		*opt_విలువ = STOP_ON_ఖాళీలు_COMMIT;
	లేకపోతే ఒకవేళ (!strcmp(arg, "drop"))
		*opt_విలువ = DROP_ఖాళీలు_COMMIT;
	లేకపోతే ఒకవేళ (!strcmp(arg, "keep"))
		*opt_విలువ = KEEP_ఖాళీలు_COMMIT;
	లేకపోతే
		తిరిగి error(_("invalid value for '%s': '%s'"), "--empty", arg);

	తిరిగి 0;
}

/**
 * Returns path relative to the am_state directory.
 */
స్థిర అంతఃవరుస స్థిరరాశి అక్షరం *am_మార్గం(స్థిరరాశి నిర్మాణం am_state *state, స్థిరరాశి అక్షరం *మార్గం)
{
	తిరిగి mkpath("%s/%s", state->dir, మార్గం);
}

/**
 * For convenience to call write_file()
 */
స్థిర శూన్య write_state_పాఠ్యం(స్థిరరాశి నిర్మాణం am_state *state,
			     స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *string)
{
	write_file(am_మార్గం(state, name), "%s", string);
}

స్థిర శూన్య write_state_లెక్క(స్థిరరాశి నిర్మాణం am_state *state,
			      స్థిరరాశి అక్షరం *name, పూర్ణాంకం విలువ)
{
	write_file(am_మార్గం(state, name), "%d", విలువ);
}

స్థిర శూన్య write_state_bool(స్థిరరాశి నిర్మాణం am_state *state,
			     స్థిరరాశి అక్షరం *name, పూర్ణాంకం విలువ)
{
	write_state_పాఠ్యం(state, name, విలువ ? "t" : "f");
}

/**
 * If state->quiet is false, calls fprintf(fp, fmt, ...), and appends a newline
 * at the end.
 */
__attribute__((format (రాయి, 3, 4)))
స్థిర శూన్య say(స్థిరరాశి నిర్మాణం am_state *state, FILE *fp, స్థిరరాశి అక్షరం *fmt, ...)
{
	va_list ap;

	va_ప్రారంభం(ap, fmt);
	ఒకవేళ (!state->quiet) {
		vfprintf(fp, fmt, ap);
		putc('\n', fp);
	}
	va_చివర(ap);
}

/**
 * Returns 1 if there is an am session in progress, 0 otherwise.
 */
స్థిర పూర్ణాంకం am_లోపల_పురోగతి(స్థిరరాశి నిర్మాణం am_state *state)
{
	నిర్మాణం stat st;

	ఒకవేళ (lstat(state->dir, &st) < 0 || !S_ISDIR(st.st_బహుళకం))
		తిరిగి 0;
	ఒకవేళ (lstat(am_మార్గం(state, "last"), &st) || !S_ISREG(st.st_బహుళకం))
		తిరిగి 0;
	ఒకవేళ (lstat(am_మార్గం(state, "next"), &st) || !S_ISREG(st.st_బహుళకం))
		తిరిగి 0;
	తిరిగి 1;
}

/**
 * Reads the contents of `file` in the `state` directory into `sb`. Returns the
 * number of bytes read on success, -1 if the file does not exist. If `trim` is
 * set, trailing whitespace will be removed.
 */
స్థిర పూర్ణాంకం read_state_file(నిర్మాణం strbuf *sb, స్థిరరాశి నిర్మాణం am_state *state,
			స్థిరరాశి అక్షరం *file, పూర్ణాంకం trim)
{
	strbuf_reset(sb);

	ఒకవేళ (strbuf_read_file(sb, am_మార్గం(state, file), 0) >= 0) {
		ఒకవేళ (trim)
			strbuf_trim(sb);

		తిరిగి sb->len;
	}

	ఒకవేళ (errno == ENOENT)
		తిరిగి -1;

	die_errno(_("could not read '%s'"), am_మార్గం(state, file));
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
స్థిర పూర్ణాంకం read_am_author_script(నిర్మాణం am_state *state)
{
	స్థిరరాశి అక్షరం *filename = am_మార్గం(state, "author-script");

	ప్రతిజ్ఞా(!state->author_name);
	ప్రతిజ్ఞా(!state->author_email);
	ప్రతిజ్ఞా(!state->author_date);

	తిరిగి read_author_script(filename, &state->author_name,
				  &state->author_email, &state->author_date, 1);
}

/**
 * Saves state->author_name, state->author_email and state->author_date in the
 * state directory's "author-script" file.
 */
స్థిర శూన్య write_author_script(స్థిరరాశి నిర్మాణం am_state *state)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;

	strbuf_addstr(&sb, "GIT_AUTHOR_NAME=");
	sq_quote_buf(&sb, state->author_name);
	strbuf_addch(&sb, '\n');

	strbuf_addstr(&sb, "GIT_AUTHOR_EMAIL=");
	sq_quote_buf(&sb, state->author_email);
	strbuf_addch(&sb, '\n');

	strbuf_addstr(&sb, "GIT_AUTHOR_DATE=");
	sq_quote_buf(&sb, state->author_date);
	strbuf_addch(&sb, '\n');

	write_state_పాఠ్యం(state, "author-script", sb.buf);

	strbuf_release(&sb);
}

/**
 * Reads the commit message from the state directory's "final-commit" file,
 * setting state->msg to its contents and state->msg_len to the length of its
 * contents in bytes.
 *
 * Returns 0 on success, -1 if the file does not exist.
 */
స్థిర పూర్ణాంకం read_commit_సందేశం(నిర్మాణం am_state *state)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;

	ప్రతిజ్ఞా(!state->సందేశం);

	ఒకవేళ (read_state_file(&sb, state, "final-commit", 0) < 0) {
		strbuf_release(&sb);
		తిరిగి -1;
	}

	state->సందేశం = strbuf_detach(&sb, &state->సందేశం_len);
	తిరిగి 0;
}

/**
 * Saves state->msg in the state directory's "final-commit" file.
 */
స్థిర శూన్య write_commit_సందేశం(స్థిరరాశి నిర్మాణం am_state *state)
{
	స్థిరరాశి అక్షరం *filename = am_మార్గం(state, "final-commit");
	write_file_buf(filename, state->సందేశం, state->సందేశం_len);
}

/**
 * Loads state from disk.
 */
స్థిర శూన్య am_ఎక్కించు(నిర్మాణం am_state *state)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;

	ఒకవేళ (read_state_file(&sb, state, "next", 1) < 0)
		BUG("state file 'next' does not exist");
	state->cur = strtol(sb.buf, ఖాళీ, 10);

	ఒకవేళ (read_state_file(&sb, state, "last", 1) < 0)
		BUG("state file 'last' does not exist");
	state->చివరి = strtol(sb.buf, ఖాళీ, 10);

	ఒకవేళ (read_am_author_script(state) < 0)
		die(_("could not parse author script"));

	read_commit_సందేశం(state);

	ఒకవేళ (read_state_file(&sb, state, "original-commit", 1) < 0)
		oidclr(&state->orig_commit, the_repository->hash_algo);
	లేకపోతే ఒకవేళ (get_oid_hex(sb.buf, &state->orig_commit) < 0)
		die(_("could not parse %s"), am_మార్గం(state, "original-commit"));

	read_state_file(&sb, state, "threeway", 1);
	state->threeway = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "quiet", 1);
	state->quiet = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "sign", 1);
	state->signoff = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "utf8", 1);
	state->utf8 = !strcmp(sb.buf, "t");

	ఒకవేళ (file_exists(am_మార్గం(state, "rerere-autoupdate"))) {
		read_state_file(&sb, state, "rerere-autoupdate", 1);
		state->allow_rerere_autoupdate = strcmp(sb.buf, "t") ?
			RERERE_NOAUTOUPDATE : RERERE_AUTOUPDATE;
	} లేకపోతే {
		state->allow_rerere_autoupdate = 0;
	}

	read_state_file(&sb, state, "keep", 1);
	ఒకవేళ (!strcmp(sb.buf, "t"))
		state->keep = KEEP_TRUE;
	లేకపోతే ఒకవేళ (!strcmp(sb.buf, "b"))
		state->keep = KEEP_NON_PATCH;
	లేకపోతే
		state->keep = KEEP_FALSE;

	read_state_file(&sb, state, "messageid", 1);
	state->సందేశం_id = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "scissors", 1);
	ఒకవేళ (!strcmp(sb.buf, "t"))
		state->scissors = SCISSORS_TRUE;
	లేకపోతే ఒకవేళ (!strcmp(sb.buf, "f"))
		state->scissors = SCISSORS_FALSE;
	లేకపోతే
		state->scissors = SCISSORS_UNSET;

	read_state_file(&sb, state, "quoted-cr", 1);
	ఒకవేళ (!*sb.buf)
		state->quoted_cr = quoted_cr_unset;
	లేకపోతే ఒకవేళ (mailinfo_parse_quoted_cr_action(sb.buf, &state->quoted_cr) != 0)
		die(_("could not parse %s"), am_మార్గం(state, "quoted-cr"));

	read_state_file(&sb, state, "apply-opt", 1);
	strvec_clear(&state->git_apply_opts);
	ఒకవేళ (sq_dequote_గమ్యం_strvec(sb.buf, &state->git_apply_opts) < 0)
		die(_("could not parse %s"), am_మార్గం(state, "apply-opt"));

	state->rebasing = !!file_exists(am_మార్గం(state, "rebasing"));

	strbuf_release(&sb);
}

/**
 * Removes the am_state directory, forcefully terminating the current am
 * session.
 */
స్థిర శూన్య am_destroy(స్థిరరాశి నిర్మాణం am_state *state)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;

	strbuf_addstr(&sb, state->dir);
	remove_dir_recursively(&sb, 0);
	strbuf_release(&sb);
}

/**
 * Runs applypatch-msg hook. Returns its exit code.
 */
స్థిర పూర్ణాంకం నిరంతరం_applypatch_సందేశం_hook(నిర్మాణం am_state *state)
{
	పూర్ణాంకం ret = 0;

	ప్రతిజ్ఞా(state->సందేశం);

	ఒకవేళ (!state->no_verify) {
		నిర్మాణం నిరంతరం_hooks_opt opt = నిరంతరం_HOOKS_OPT_INIT_FORCE_SERIAL;
		strvec_నెట్టు(&opt.వాదనలు, am_మార్గం(state, "final-commit"));
		ret = నిరంతరం_hooks_opt(the_repository, "applypatch-msg", &opt);
	}

	ఒకవేళ (!ret) {
		FREE_AND_NULL(state->సందేశం);
		ఒకవేళ (read_commit_సందేశం(state) < 0)
			die(_("'%s' was deleted by the applypatch-msg hook"),
				am_మార్గం(state, "final-commit"));
	}

	తిరిగి ret;
}

/**
 * Runs post-rewrite hook. Returns it exit code.
 */
స్థిర పూర్ణాంకం నిరంతరం_post_rewrite_hook(స్థిరరాశి నిర్మాణం am_state *state)
{
	నిర్మాణం నిరంతరం_hooks_opt opt = నిరంతరం_HOOKS_OPT_INIT;

	strvec_నెట్టు(&opt.వాదనలు, "rebase");
	opt.మార్గం_గమ్యం_stdin = am_మార్గం(state, "rewritten");

	తిరిగి నిరంతరం_hooks_opt(the_repository, "post-rewrite", &opt);
}

/**
 * Reads the state directory's "rewritten" file, and copies notes from the old
 * commits listed in the file to their rewritten commits.
 *
 * Returns 0 on success, -1 on failure.
 */
స్థిర పూర్ణాంకం copy_notes_for_rebase(స్థిరరాశి నిర్మాణం am_state *state)
{
	నిర్మాణం notes_rewrite_cfg *c;
	నిర్మాణం strbuf sb = STRBUF_INIT;
	స్థిరరాశి అక్షరం *invalid_పంక్తి = _("Malformed input line: '%s'.");
	స్థిరరాశి అక్షరం *సందేశం = "Notes added by 'git rebase'";
	FILE *fp;
	పూర్ణాంకం ret = 0;

	ప్రతిజ్ఞా(state->rebasing);

	c = init_copy_notes_for_rewrite("rebase");
	ఒకవేళ (!c)
		తిరిగి 0;

	fp = xfopen(am_మార్గం(state, "rewritten"), "r");

	అయితే (!strbuf_getline_lf(&sb, fp)) {
		నిర్మాణం object_id from_obj, గమ్యం_obj;
		స్థిరరాశి అక్షరం *p;

		ఒకవేళ (sb.len != the_hash_algo->hexsz * 2 + 1) {
			ret = error(invalid_పంక్తి, sb.buf);
			వెళ్ళు ముగింపు;
		}

		ఒకవేళ (parse_oid_hex(sb.buf, &from_obj, &p)) {
			ret = error(invalid_పంక్తి, sb.buf);
			వెళ్ళు ముగింపు;
		}

		ఒకవేళ (*p != ' ') {
			ret = error(invalid_పంక్తి, sb.buf);
			వెళ్ళు ముగింపు;
		}

		ఒకవేళ (get_oid_hex(p + 1, &గమ్యం_obj)) {
			ret = error(invalid_పంక్తి, sb.buf);
			వెళ్ళు ముగింపు;
		}

		ఒకవేళ (copy_note_for_rewrite(c, &from_obj, &గమ్యం_obj))
			ret = error(_("Failed to copy notes from '%s' to '%s'"),
					oid_గమ్యం_hex(&from_obj), oid_గమ్యం_hex(&గమ్యం_obj));
	}

ముగింపు:
	ముగింపు_copy_notes_for_rewrite(the_repository, c, సందేశం);
	fclose(fp);
	strbuf_release(&sb);
	తిరిగి ret;
}

/**
 * Determines if the file looks like a piece of RFC2822 mail by grabbing all
 * non-indented lines and checking if they look like they begin with valid
 * header field names.
 *
 * Returns 1 if the file looks like a piece of mail, 0 otherwise.
 */
స్థిర పూర్ణాంకం is_mail(FILE *fp)
{
	స్థిరరాశి అక్షరం *శీర్షిక_regex = "^[!-9;-~]+:";
	నిర్మాణం strbuf sb = STRBUF_INIT;
	regex_t regex;
	పూర్ణాంకం ret = 1;

	ఒకవేళ (fseek(fp, 0L, SEEK_సమితి))
		die_errno(_("fseek failed"));

	ఒకవేళ (regcomp(&regex, శీర్షిక_regex, REG_NOSUB | REG_EXTENDED))
		die("invalid pattern: %s", శీర్షిక_regex);

	అయితే (!strbuf_getline(&sb, fp)) {
		ఒకవేళ (!sb.len)
			ఆపు; /* End of header */

		/* Ignore indented folded lines */
		ఒకవేళ (*sb.buf == '\t' || *sb.buf == ' ')
			కొనసాగు;

		/* It's a header if it matches header_regex */
		ఒకవేళ (regexec(&regex, sb.buf, 0, ఖాళీ, 0)) {
			ret = 0;
			వెళ్ళు పూర్తి;
		}
	}

పూర్తి:
	regfree(&regex);
	strbuf_release(&sb);
	తిరిగి ret;
}

/**
 * Attempts to detect the patch_format of the patches contained in `paths`,
 * returning the PATCH_FORMAT_* enum value. Returns PATCH_FORMAT_UNKNOWN if
 * detection fails.
 */
స్థిర పూర్ణాంకం detect_patch_format(స్థిరరాశి అక్షరం **paths)
{
	గణన patch_format ret = PATCH_FORMAT_UNKNOWN;
	నిర్మాణం strbuf l1 = STRBUF_INIT;
	నిర్మాణం strbuf l2 = STRBUF_INIT;
	నిర్మాణం strbuf l3 = STRBUF_INIT;
	FILE *fp;

	/*
	 * We default to mbox format if input is from stdin and for directories
	 */
	ఒకవేళ (!*paths || !strcmp(*paths, "-") || is_directory(*paths))
		తిరిగి PATCH_FORMAT_MBOX;

	/*
	 * Otherwise, check the first few lines of the first patch, starting
	 * from the first non-blank line, to try to detect its format.
	 */

	fp = xfopen(*paths, "r");

	అయితే (!strbuf_getline(&l1, fp)) {
		ఒకవేళ (l1.len)
			ఆపు;
	}

	ఒకవేళ (starts_with(l1.buf, "From ") || starts_with(l1.buf, "From: ")) {
		ret = PATCH_FORMAT_MBOX;
		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (starts_with(l1.buf, "# This series applies on GIT commit")) {
		ret = PATCH_FORMAT_STGIT_SERIES;
		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (!strcmp(l1.buf, "# HG changeset patch")) {
		ret = PATCH_FORMAT_HG;
		వెళ్ళు పూర్తి;
	}

	strbuf_getline(&l2, fp);
	strbuf_getline(&l3, fp);

	/*
	 * If the second line is empty and the third is a From, Author or Date
	 * entry, this is likely an StGit patch.
	 */
	ఒకవేళ (l1.len && !l2.len &&
		(starts_with(l3.buf, "From:") ||
		 starts_with(l3.buf, "Author:") ||
		 starts_with(l3.buf, "Date:"))) {
		ret = PATCH_FORMAT_STGIT;
		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (l1.len && is_mail(fp)) {
		ret = PATCH_FORMAT_MBOX;
		వెళ్ళు పూర్తి;
	}

పూర్తి:
	fclose(fp);
	strbuf_release(&l1);
	strbuf_release(&l2);
	strbuf_release(&l3);
	తిరిగి ret;
}

/**
 * Splits out individual email patches from `paths`, where each path is either
 * a mbox file or a Maildir. Returns 0 on success, -1 on failure.
 */
స్థిర పూర్ణాంకం విభజించు_mail_mbox(నిర్మాణం am_state *state, స్థిరరాశి అక్షరం **paths,
				పూర్ణాంకం keep_cr, పూర్ణాంకం mboxrd)
{
	నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;
	నిర్మాణం strbuf చివరి = STRBUF_INIT;
	పూర్ణాంకం ret;

	cp.git_cmd = 1;
	strvec_నెట్టు(&cp.వాదనలు, "mailsplit");
	strvec_pushf(&cp.వాదనలు, "-d%d", state->prec);
	strvec_pushf(&cp.వాదనలు, "-o%s", state->dir);
	strvec_నెట్టు(&cp.వాదనలు, "-b");
	ఒకవేళ (keep_cr)
		strvec_నెట్టు(&cp.వాదనలు, "--keep-cr");
	ఒకవేళ (mboxrd)
		strvec_నెట్టు(&cp.వాదనలు, "--mboxrd");
	strvec_నెట్టు(&cp.వాదనలు, "--");
	strvec_pushv(&cp.వాదనలు, paths);

	ret = capture_command(&cp, &చివరి, 8);
	ఒకవేళ (ret)
		వెళ్ళు నిష్క్రమించు;

	state->cur = 1;
	state->చివరి = strtol(చివరి.buf, ఖాళీ, 10);

నిష్క్రమించు:
	strbuf_release(&చివరి);
	తిరిగి ret ? -1 : 0;
}

/**
 * Callback signature for split_mail_conv(). The foreign patch should be
 * read from `in`, and the converted patch (in RFC2822 mail format) should be
 * written to `out`. Return 0 on success, or -1 on failure.
 */
రకనిర్వచనం పూర్ణాంకం (*mail_conv_fn)(FILE *బహిర్గతం, FILE *లోపల, పూర్ణాంకం keep_cr);

/**
 * Calls `fn` for each file in `paths` to convert the foreign patch to the
 * RFC2822 mail format suitable for parsing with git-mailinfo.
 *
 * Returns 0 on success, -1 on failure.
 */
స్థిర పూర్ణాంకం విభజించు_mail_conv(mail_conv_fn fn, నిర్మాణం am_state *state,
			స్థిరరాశి అక్షరం **paths, పూర్ణాంకం keep_cr)
{
	స్థిర స్థిరరాశి అక్షరం *stdin_only[] = {"-", ఖాళీ};
	పూర్ణాంకం i;

	ఒకవేళ (!*paths)
		paths = stdin_only;

	కొరకు (i = 0; *paths; paths++, i++) {
		FILE *లోపల, *బహిర్గతం;
		స్థిరరాశి అక్షరం *mail;
		పూర్ణాంకం ret;

		ఒకవేళ (!strcmp(*paths, "-"))
			లోపల = stdin;
		లేకపోతే
			లోపల = fopen(*paths, "r");

		ఒకవేళ (!లోపల)
			తిరిగి error_errno(_("could not open '%s' for reading"),
					   *paths);

		mail = mkpath("%s/%0*d", state->dir, state->prec, i + 1);

		బహిర్గతం = fopen(mail, "w");
		ఒకవేళ (!బహిర్గతం) {
			ఒకవేళ (లోపల != stdin)
				fclose(లోపల);
			తిరిగి error_errno(_("could not open '%s' for writing"),
					   mail);
		}

		ret = fn(బహిర్గతం, లోపల, keep_cr);

		fclose(బహిర్గతం);
		ఒకవేళ (లోపల != stdin)
			fclose(లోపల);

		ఒకవేళ (ret)
			తిరిగి error(_("could not parse patch '%s'"), *paths);
	}

	state->cur = 1;
	state->చివరి = i;
	తిరిగి 0;
}

/**
 * A split_mail_conv() callback that converts an StGit patch to an RFC2822
 * message suitable for parsing with git-mailinfo.
 */
స్థిర పూర్ణాంకం stgit_patch_గమ్యం_mail(FILE *బహిర్గతం, FILE *లోపల, పూర్ణాంకం keep_cr వాడని)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;
	పూర్ణాంకం subject_printed = 0;

	అయితే (!strbuf_getline_lf(&sb, లోపల)) {
		స్థిరరాశి అక్షరం *str;

		ఒకవేళ (str_isspace(sb.buf))
			కొనసాగు;
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(sb.buf, "Author:", &str))
			fprintf(బహిర్గతం, "From:%s\n", str);
		లేకపోతే ఒకవేళ (starts_with(sb.buf, "From") || starts_with(sb.buf, "Date"))
			fprintf(బహిర్గతం, "%s\n", sb.buf);
		లేకపోతే ఒకవేళ (!subject_printed) {
			fprintf(బహిర్గతం, "Subject: %s\n", sb.buf);
			subject_printed = 1;
		} లేకపోతే {
			fprintf(బహిర్గతం, "\n%s\n", sb.buf);
			ఆపు;
		}
	}

	strbuf_reset(&sb);
	అయితే (strbuf_fread(&sb, 8192, లోపల) > 0) {
		fwrite(sb.buf, 1, sb.len, బహిర్గతం);
		strbuf_reset(&sb);
	}

	strbuf_release(&sb);
	తిరిగి 0;
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
స్థిర పూర్ణాంకం విభజించు_mail_stgit_series(నిర్మాణం am_state *state, స్థిరరాశి అక్షరం **paths,
					పూర్ణాంకం keep_cr)
{
	స్థిరరాశి అక్షరం *series_dir;
	అక్షరం *series_dir_buf;
	FILE *fp;
	నిర్మాణం strvec patches = STRVEC_INIT;
	నిర్మాణం strbuf sb = STRBUF_INIT;
	పూర్ణాంకం ret;

	ఒకవేళ (!paths[0] || paths[1])
		తిరిగి error(_("Only one StGIT patch series can be applied at once"));

	series_dir_buf = xstrdup(*paths);
	series_dir = dirname(series_dir_buf);

	fp = fopen(*paths, "r");
	ఒకవేళ (!fp) {
		విడుదల(series_dir_buf);
		తిరిగి error_errno(_("could not open '%s' for reading"), *paths);
	}

	అయితే (!strbuf_getline_lf(&sb, fp)) {
		ఒకవేళ (*sb.buf == '#')
			కొనసాగు; /* skip comment lines */

		strvec_నెట్టు(&patches, mkpath("%s/%s", series_dir, sb.buf));
	}

	fclose(fp);
	strbuf_release(&sb);
	విడుదల(series_dir_buf);

	ret = విభజించు_mail_conv(stgit_patch_గమ్యం_mail, state, patches.v, keep_cr);

	strvec_clear(&patches);
	తిరిగి ret;
}

/**
 * A split_patches_conv() callback that converts a mercurial patch to a RFC2822
 * message suitable for parsing with git-mailinfo.
 */
స్థిర పూర్ణాంకం hg_patch_గమ్యం_mail(FILE *బహిర్గతం, FILE *లోపల, పూర్ణాంకం keep_cr వాడని)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;
	పూర్ణాంకం rc = 0;

	అయితే (!strbuf_getline_lf(&sb, లోపల)) {
		స్థిరరాశి అక్షరం *str;

		ఒకవేళ (skip_ఉపసర్గ(sb.buf, "# User ", &str))
			fprintf(బహిర్గతం, "From: %s\n", str);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(sb.buf, "# Date ", &str)) {
			timestamp_t timestamp;
			దీర్ఘ tz, tz2;
			అక్షరం *చివర;

			errno = 0;
			timestamp = parse_timestamp(str, &చివర, 10);
			ఒకవేళ (errno) {
				rc = error(_("invalid timestamp"));
				వెళ్ళు నిష్క్రమించు;
			}

			ఒకవేళ (!skip_ఉపసర్గ(చివర, " ", &str)) {
				rc = error(_("invalid Date line"));
				వెళ్ళు నిష్క్రమించు;
			}

			errno = 0;
			tz = strtol(str, &చివర, 10);
			ఒకవేళ (errno) {
				rc = error(_("invalid timezone offset"));
				వెళ్ళు నిష్క్రమించు;
			}

			ఒకవేళ (*చివర) {
				rc = error(_("invalid Date line"));
				వెళ్ళు నిష్క్రమించు;
			}

			/*
			 * mercurial's timezone is in seconds west of UTC,
			 * however git's timezone is in hours + minutes east of
			 * UTC. Convert it.
			 */
			tz2 = labs(tz) / 3600 * 100 + labs(tz) % 3600 / 60;
			ఒకవేళ (tz > 0)
				tz2 = -tz2;

			fprintf(బహిర్గతం, "Date: %s\n", show_date(timestamp, tz2, DATE_బహుళకం(RFC2822)));
		} లేకపోతే ఒకవేళ (starts_with(sb.buf, "# ")) {
			కొనసాగు;
		} లేకపోతే {
			fprintf(బహిర్గతం, "\n%s\n", sb.buf);
			ఆపు;
		}
	}

	strbuf_reset(&sb);
	అయితే (strbuf_fread(&sb, 8192, లోపల) > 0) {
		fwrite(sb.buf, 1, sb.len, బహిర్గతం);
		strbuf_reset(&sb);
	}
నిష్క్రమించు:
	strbuf_release(&sb);
	తిరిగి rc;
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
స్థిర పూర్ణాంకం విభజించు_mail(నిర్మాణం am_state *state, గణన patch_format patch_format,
			స్థిరరాశి అక్షరం **paths, పూర్ణాంకం keep_cr)
{
	ఒకవేళ (keep_cr < 0) {
		keep_cr = 0;
		repo_config_get_bool(the_repository, "am.keepcr", &keep_cr);
	}

	ఎంపిక (patch_format) {
	స్థితి PATCH_FORMAT_MBOX:
		తిరిగి విభజించు_mail_mbox(state, paths, keep_cr, 0);
	స్థితి PATCH_FORMAT_STGIT:
		తిరిగి విభజించు_mail_conv(stgit_patch_గమ్యం_mail, state, paths, keep_cr);
	స్థితి PATCH_FORMAT_STGIT_SERIES:
		తిరిగి విభజించు_mail_stgit_series(state, paths, keep_cr);
	స్థితి PATCH_FORMAT_HG:
		తిరిగి విభజించు_mail_conv(hg_patch_గమ్యం_mail, state, paths, keep_cr);
	స్థితి PATCH_FORMAT_MBOXRD:
		తిరిగి విభజించు_mail_mbox(state, paths, keep_cr, 1);
	అప్రమేయ:
		BUG("invalid patch_format");
	}
	తిరిగి -1;
}

/**
 * Setup a new am session for applying patches
 */
స్థిర శూన్య am_అమరిక(నిర్మాణం am_state *state, గణన patch_format patch_format,
			స్థిరరాశి అక్షరం **paths, పూర్ణాంకం keep_cr)
{
	నిర్మాణం object_id curr_తల;
	స్థిరరాశి అక్షరం *str;
	నిర్మాణం strbuf sb = STRBUF_INIT;

	ఒకవేళ (!patch_format)
		patch_format = detect_patch_format(paths);

	ఒకవేళ (!patch_format) {
		fprintf_ln(stderr, _("Patch format detection failed."));
		die(ఖాళీ);
	}

	ఒకవేళ (mkdir(state->dir, 0777) < 0 && errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), state->dir);
	refs_delete_ref(get_main_ref_store(the_repository), ఖాళీ,
			"REBASE_HEAD", ఖాళీ, REF_NO_DEREF);

	ఒకవేళ (విభజించు_mail(state, patch_format, paths, keep_cr) < 0) {
		am_destroy(state);
		die(_("Failed to split patches."));
	}

	ఒకవేళ (state->rebasing)
		state->threeway = 1;

	write_state_bool(state, "threeway", state->threeway);
	write_state_bool(state, "quiet", state->quiet);
	write_state_bool(state, "sign", state->signoff);
	write_state_bool(state, "utf8", state->utf8);

	ఒకవేళ (state->allow_rerere_autoupdate)
		write_state_bool(state, "rerere-autoupdate",
			 state->allow_rerere_autoupdate == RERERE_AUTOUPDATE);

	ఎంపిక (state->keep) {
	స్థితి KEEP_FALSE:
		str = "f";
		ఆపు;
	స్థితి KEEP_TRUE:
		str = "t";
		ఆపు;
	స్థితి KEEP_NON_PATCH:
		str = "b";
		ఆపు;
	అప్రమేయ:
		BUG("invalid value for state->keep");
	}

	write_state_పాఠ్యం(state, "keep", str);
	write_state_bool(state, "messageid", state->సందేశం_id);

	ఎంపిక (state->scissors) {
	స్థితి SCISSORS_UNSET:
		str = "";
		ఆపు;
	స్థితి SCISSORS_FALSE:
		str = "f";
		ఆపు;
	స్థితి SCISSORS_TRUE:
		str = "t";
		ఆపు;
	అప్రమేయ:
		BUG("invalid value for state->scissors");
	}
	write_state_పాఠ్యం(state, "scissors", str);

	ఎంపిక (state->quoted_cr) {
	స్థితి quoted_cr_unset:
		str = "";
		ఆపు;
	స్థితి quoted_cr_nowarn:
		str = "nowarn";
		ఆపు;
	స్థితి quoted_cr_warn:
		str = "warn";
		ఆపు;
	స్థితి quoted_cr_strip:
		str = "strip";
		ఆపు;
	అప్రమేయ:
		BUG("invalid value for state->quoted_cr");
	}
	write_state_పాఠ్యం(state, "quoted-cr", str);

	sq_quote_argv(&sb, state->git_apply_opts.v);
	write_state_పాఠ్యం(state, "apply-opt", sb.buf);

	ఒకవేళ (state->rebasing)
		write_state_పాఠ్యం(state, "rebasing", "");
	లేకపోతే
		write_state_పాఠ్యం(state, "applying", "");

	ఒకవేళ (!repo_get_oid(the_repository, "HEAD", &curr_తల)) {
		write_state_పాఠ్యం(state, "abort-safety", oid_గమ్యం_hex(&curr_తల));
		ఒకవేళ (!state->rebasing)
			refs_update_ref(get_main_ref_store(the_repository),
					"am", "ORIG_HEAD", &curr_తల, ఖాళీ,
					0,
					UPDATE_REFS_DIE_ON_ERR);
	} లేకపోతే {
		write_state_పాఠ్యం(state, "abort-safety", "");
		ఒకవేళ (!state->rebasing)
			refs_delete_ref(get_main_ref_store(the_repository),
					ఖాళీ, "ORIG_HEAD", ఖాళీ, 0);
	}

	/*
	 * NOTE: Since the "next" and "last" files determine if an am_state
	 * session is in progress, they should be written last.
	 */

	write_state_లెక్క(state, "next", state->cur);
	write_state_లెక్క(state, "last", state->చివరి);

	strbuf_release(&sb);
}

/**
 * Increments the patch pointer, and cleans am_state for the application of the
 * next patch.
 */
స్థిర శూన్య am_తదుపరి(నిర్మాణం am_state *state)
{
	నిర్మాణం object_id తల;

	FREE_AND_NULL(state->author_name);
	FREE_AND_NULL(state->author_email);
	FREE_AND_NULL(state->author_date);
	FREE_AND_NULL(state->సందేశం);
	state->సందేశం_len = 0;

	unlink(am_మార్గం(state, "author-script"));
	unlink(am_మార్గం(state, "final-commit"));

	oidclr(&state->orig_commit, the_repository->hash_algo);
	unlink(am_మార్గం(state, "original-commit"));
	refs_delete_ref(get_main_ref_store(the_repository), ఖాళీ,
			"REBASE_HEAD", ఖాళీ, REF_NO_DEREF);

	ఒకవేళ (!repo_get_oid(the_repository, "HEAD", &తల))
		write_state_పాఠ్యం(state, "abort-safety", oid_గమ్యం_hex(&తల));
	లేకపోతే
		write_state_పాఠ్యం(state, "abort-safety", "");

	state->cur++;
	write_state_లెక్క(state, "next", state->cur);
}

/**
 * Returns the filename of the current patch email.
 */
స్థిర స్థిరరాశి అక్షరం *msgnum(స్థిరరాశి నిర్మాణం am_state *state)
{
	స్థిర నిర్మాణం strbuf sb = STRBUF_INIT;

	strbuf_reset(&sb);
	strbuf_addf(&sb, "%0*d", state->prec, state->cur);

	తిరిగి sb.buf;
}

/**
 * Dies with a user-friendly message on how to proceed after resolving the
 * problem. This message can be overridden with state->resolvemsg.
 */
స్థిర శూన్య NORETURN die_user_resolve(స్థిరరాశి నిర్మాణం am_state *state)
{
	ఒకవేళ (state->resolvemsg) {
		advise_if_enabled(ADVICE_విలీనం_CONFLICT, "%s", state->resolvemsg);
	} లేకపోతే {
		స్థిరరాశి అక్షరం *cmdline = state->interactive ? "git am -i" : "git am";
		నిర్మాణం strbuf sb = STRBUF_INIT;

		strbuf_addf(&sb, _("When you have resolved this problem, run \"%s --continue\".\n"), cmdline);
		strbuf_addf(&sb, _("If you prefer to skip this patch, run \"%s --skip\" instead.\n"), cmdline);

		ఒకవేళ (advice_enabled(ADVICE_AM_WORK_DIR) &&
		    is_ఖాళీలు_or_missing_file(am_మార్గం(state, "patch")) &&
		    !repo_సూచీ_has_changes(the_repository, ఖాళీ, ఖాళీ))
			strbuf_addf(&sb, _("To record the empty patch as an empty commit, run \"%s --allow-empty\".\n"), cmdline);

		strbuf_addf(&sb, _("To restore the original branch and stop patching, run \"%s --abort\"."), cmdline);

		advise_if_enabled(ADVICE_విలీనం_CONFLICT, "%s", sb.buf);
		strbuf_release(&sb);
	}

	die(ఖాళీ);
}

/**
 * Appends signoff to the "msg" field of the am_state.
 */
స్థిర శూన్య am_append_signoff(నిర్మాణం am_state *state)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;

	strbuf_attach(&sb, state->సందేశం, state->సందేశం_len, state->సందేశం_len + 1);
	append_signoff(&sb, 0, 0);
	state->సందేశం = strbuf_detach(&sb, &state->సందేశం_len);
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
స్థిర పూర్ణాంకం parse_mail(నిర్మాణం am_state *state, స్థిరరాశి అక్షరం *mail)
{
	FILE *fp;
	నిర్మాణం strbuf sb = STRBUF_INIT;
	నిర్మాణం strbuf సందేశం = STRBUF_INIT;
	నిర్మాణం strbuf author_name = STRBUF_INIT;
	నిర్మాణం strbuf author_date = STRBUF_INIT;
	నిర్మాణం strbuf author_email = STRBUF_INIT;
	పూర్ణాంకం ret = 0;
	నిర్మాణం mailinfo mi;

	అమరిక_mailinfo(the_repository, &mi);

	ఒకవేళ (state->utf8)
		mi.metainfo_charset = get_commit_output_encoding();
	లేకపోతే
		mi.metainfo_charset = ఖాళీ;

	ఎంపిక (state->keep) {
	స్థితి KEEP_FALSE:
		ఆపు;
	స్థితి KEEP_TRUE:
		mi.keep_subject = 1;
		ఆపు;
	స్థితి KEEP_NON_PATCH:
		mi.keep_non_patch_brackets_లోపల_subject = 1;
		ఆపు;
	అప్రమేయ:
		BUG("invalid value for state->keep");
	}

	ఒకవేళ (state->సందేశం_id)
		mi.కూడు_సందేశం_id = 1;

	ఎంపిక (state->scissors) {
	స్థితి SCISSORS_UNSET:
		ఆపు;
	స్థితి SCISSORS_FALSE:
		mi.use_scissors = 0;
		ఆపు;
	స్థితి SCISSORS_TRUE:
		mi.use_scissors = 1;
		ఆపు;
	అప్రమేయ:
		BUG("invalid value for state->scissors");
	}

	ఎంపిక (state->quoted_cr) {
	స్థితి quoted_cr_unset:
		ఆపు;
	స్థితి quoted_cr_nowarn:
	స్థితి quoted_cr_warn:
	స్థితి quoted_cr_strip:
		mi.quoted_cr = state->quoted_cr;
		ఆపు;
	అప్రమేయ:
		BUG("invalid value for state->quoted_cr");
	}

	mi.ఆదానం = xfopen(mail, "r");
	mi.output = xfopen(am_మార్గం(state, "info"), "w");
	ఒకవేళ (mailinfo(&mi, am_మార్గం(state, "msg"), am_మార్గం(state, "patch")))
		die("could not parse patch");

	fclose(mi.ఆదానం);
	fclose(mi.output);

	ఒకవేళ (mi.format_flowed)
		warning(_("Patch sent with format=flowed; "
			  "space at the end of lines might be lost."));

	/* Extract message and author information */
	fp = xfopen(am_మార్గం(state, "info"), "r");
	అయితే (!strbuf_getline_lf(&sb, fp)) {
		స్థిరరాశి అక్షరం *x;

		ఒకవేళ (skip_ఉపసర్గ(sb.buf, "Subject: ", &x)) {
			ఒకవేళ (సందేశం.len)
				strbuf_addch(&సందేశం, '\n');
			strbuf_addstr(&సందేశం, x);
		} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(sb.buf, "Author: ", &x))
			strbuf_addstr(&author_name, x);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(sb.buf, "Email: ", &x))
			strbuf_addstr(&author_email, x);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(sb.buf, "Date: ", &x))
			strbuf_addstr(&author_date, x);
	}
	fclose(fp);

	/* Skip pine's internal folder data */
	ఒకవేళ (!strcmp(author_name.buf, "Mail System Internal Data")) {
		ret = 1;
		వెళ్ళు ముగింపు;
	}

	strbuf_addstr(&సందేశం, "\n\n");
	strbuf_addbuf(&సందేశం, &mi.log_సందేశం);
	strbuf_stripspace(&సందేశం, ఖాళీ);

	ప్రతిజ్ఞా(!state->author_name);
	state->author_name = strbuf_detach(&author_name, ఖాళీ);

	ప్రతిజ్ఞా(!state->author_email);
	state->author_email = strbuf_detach(&author_email, ఖాళీ);

	ప్రతిజ్ఞా(!state->author_date);
	state->author_date = strbuf_detach(&author_date, ఖాళీ);

	ప్రతిజ్ఞా(!state->సందేశం);
	state->సందేశం = strbuf_detach(&సందేశం, &state->సందేశం_len);

ముగింపు:
	strbuf_release(&సందేశం);
	strbuf_release(&author_date);
	strbuf_release(&author_email);
	strbuf_release(&author_name);
	strbuf_release(&sb);
	clear_mailinfo(&mi);
	తిరిగి ret;
}

/**
 * Sets commit_id to the commit hash where the mail was generated from.
 * Returns 0 on success, -1 on failure.
 */
స్థిర పూర్ణాంకం get_mail_commit_oid(నిర్మాణం object_id *commit_id, స్థిరరాశి అక్షరం *mail)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;
	FILE *fp = xfopen(mail, "r");
	స్థిరరాశి అక్షరం *x;
	పూర్ణాంకం ret = 0;

	ఒకవేళ (strbuf_getline_lf(&sb, fp) ||
	    !skip_ఉపసర్గ(sb.buf, "From ", &x) ||
	    get_oid_hex(x, commit_id) < 0)
		ret = -1;

	strbuf_release(&sb);
	fclose(fp);
	తిరిగి ret;
}

/**
 * Sets state->msg, state->author_name, state->author_email, state->author_date
 * to the commit's respective info.
 */
స్థిర శూన్య get_commit_info(నిర్మాణం am_state *state, నిర్మాణం commit *commit)
{
	స్థిరరాశి అక్షరం *బఫరు, *ident_పంక్తి, *సందేశం;
	కొలత_t ident_len;
	నిర్మాణం ident_విభజించు id;

	బఫరు = repo_logmsg_reencode(the_repository, commit, ఖాళీ,
				      get_commit_output_encoding());

	ident_పంక్తి = కనుగొను_commit_శీర్షిక(బఫరు, "author", &ident_len);
	ఒకవేళ (!ident_పంక్తి)
		die(_("missing author line in commit %s"),
		      oid_గమ్యం_hex(&commit->object.oid));
	ఒకవేళ (విభజించు_ident_పంక్తి(&id, ident_పంక్తి, ident_len) < 0)
		die(_("invalid ident line: %.*s"), (పూర్ణాంకం)ident_len, ident_పంక్తి);

	ప్రతిజ్ఞా(!state->author_name);
	ఒకవేళ (id.name_begin)
		state->author_name =
			xmemdupz(id.name_begin, id.name_చివర - id.name_begin);
	లేకపోతే
		state->author_name = xstrdup("");

	ప్రతిజ్ఞా(!state->author_email);
	ఒకవేళ (id.mail_begin)
		state->author_email =
			xmemdupz(id.mail_begin, id.mail_చివర - id.mail_begin);
	లేకపోతే
		state->author_email = xstrdup("");

	ప్రతిజ్ఞా(!state->author_date);
	state->author_date = xstrdup(show_ident_date(&id, DATE_బహుళకం(సాధారణ)));

	ప్రతిజ్ఞా(!state->సందేశం);
	సందేశం = అక్షరశ్రేణి_శోధన(బఫరు, "\n\n");
	ఒకవేళ (!సందేశం)
		die(_("unable to parse commit %s"), oid_గమ్యం_hex(&commit->object.oid));
	state->సందేశం = xstrdup(సందేశం + 2);
	state->సందేశం_len = అక్షరశ్రేణి_పొడవు(state->సందేశం);
	repo_unuse_commit_బఫరు(the_repository, commit, బఫరు);
}

/**
 * Writes `commit` as a patch to the state directory's "patch" file.
 */
స్థిర శూన్య write_commit_patch(స్థిరరాశి నిర్మాణం am_state *state, నిర్మాణం commit *commit)
{
	నిర్మాణం rev_info rev_info;
	FILE *fp;

	fp = xfopen(am_మార్గం(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, ఖాళీ);
	rev_info.diff = 1;
	rev_info.abbrev = 0;
	rev_info.disable_stdin = 1;
	rev_info.show_మూలం_diff = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.flags.ద్విమాన = 1;
	rev_info.diffopt.flags.నిండిన_సూచీ = 1;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	కూడు_pending_object(&rev_info, &commit->object, "");
	diff_అమరిక_పూర్తి(&rev_info.diffopt);
	log_వృక్షం_commit(&rev_info, commit);
	release_revisions(&rev_info);
}

/**
 * Writes the diff of the index against HEAD as a patch to the state
 * directory's "patch" file.
 */
స్థిర శూన్య write_సూచీ_patch(స్థిరరాశి నిర్మాణం am_state *state)
{
	నిర్మాణం వృక్షం *వృక్షం;
	నిర్మాణం object_id తల;
	నిర్మాణం rev_info rev_info;
	FILE *fp;

	ఒకవేళ (!repo_get_oid(the_repository, "HEAD", &తల)) {
		నిర్మాణం commit *commit = lookup_commit_or_die(&తల, "HEAD");
		వృక్షం = repo_get_commit_వృక్షం(the_repository, commit);
	} లేకపోతే
		వృక్షం = lookup_వృక్షం(the_repository,
				   the_repository->hash_algo->ఖాళీలు_వృక్షం);

	fp = xfopen(am_మార్గం(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, ఖాళీ);
	rev_info.diff = 1;
	rev_info.disable_stdin = 1;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	కూడు_pending_object(&rev_info, &వృక్షం->object, "");
	diff_అమరిక_పూర్తి(&rev_info.diffopt);
	నిరంతరం_diff_సూచీ(&rev_info, DIFF_సూచీ_CACHED);
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
స్థిర పూర్ణాంకం parse_mail_rebase(నిర్మాణం am_state *state, స్థిరరాశి అక్షరం *mail)
{
	నిర్మాణం commit *commit;
	నిర్మాణం object_id commit_oid;

	ఒకవేళ (get_mail_commit_oid(&commit_oid, mail) < 0)
		die(_("could not parse %s"), mail);

	commit = lookup_commit_or_die(&commit_oid, mail);

	get_commit_info(state, commit);

	write_commit_patch(state, commit);

	oidcpy(&state->orig_commit, &commit_oid);
	write_state_పాఠ్యం(state, "original-commit", oid_గమ్యం_hex(&commit_oid));
	refs_update_ref(get_main_ref_store(the_repository), "am",
			"REBASE_HEAD", &commit_oid,
			ఖాళీ, REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);

	తిరిగి 0;
}

/**
 * Applies current patch with git-apply. Returns 0 on success, -1 otherwise. If
 * `index_file` is not NULL, the patch will be applied to that index.
 */
స్థిర పూర్ణాంకం నిరంతరం_apply(స్థిరరాశి నిర్మాణం am_state *state, స్థిరరాశి అక్షరం *సూచీ_file)
{
	నిర్మాణం strvec apply_paths = STRVEC_INIT;
	నిర్మాణం strvec apply_opts = STRVEC_INIT;
	నిర్మాణం apply_state apply_state;
	పూర్ణాంకం res, opts_ఎడమ;
	పూర్ణాంకం force_apply = 0;
	పూర్ణాంకం options = 0;
	స్థిరరాశి అక్షరం **apply_argv;

	ఒకవేళ (init_apply_state(&apply_state, the_repository, ఖాళీ))
		BUG("init_apply_state() failed");

	strvec_నెట్టు(&apply_opts, "apply");
	strvec_pushv(&apply_opts, state->git_apply_opts.v);

	/*
	 * Build a copy that apply_parse_options() can rearrange.
	 * apply_opts.v keeps referencing the allocated strings for
	 * strvec_clear() to release.
	 */
	DUP_ARRAY(apply_argv, apply_opts.v, apply_opts.nr);

	opts_ఎడమ = apply_parse_options(apply_opts.nr, apply_argv,
					&apply_state, &force_apply, &options,
					ఖాళీ);

	ఒకవేళ (opts_ఎడమ != 0)
		die("unknown option passed through to git apply");

	ఒకవేళ (సూచీ_file) {
		apply_state.సూచీ_file = సూచీ_file;
		apply_state.cached = 1;
	} లేకపోతే
		apply_state.check_సూచీ = 1;

	/*
	 * If we are allowed to fall back on 3-way merge, don't give false
	 * errors during the initial attempt.
	 */
	ఒకవేళ (state->threeway && !సూచీ_file)
		apply_state.apply_verbosity = verbosity_silent;

	ఒకవేళ (check_apply_state(&apply_state, force_apply))
		BUG("check_apply_state() failed");

	strvec_నెట్టు(&apply_paths, am_మార్గం(state, "patch"));

	res = apply_all_patches(&apply_state, apply_paths.nr, apply_paths.v, options);

	strvec_clear(&apply_paths);
	strvec_clear(&apply_opts);
	clear_apply_state(&apply_state);
	విడుదల(apply_argv);

	ఒకవేళ (res)
		తిరిగి res;

	ఒకవేళ (సూచీ_file) {
		/* Reload index as apply_all_patches() will have modified it. */
		discard_సూచీ(the_repository->సూచీ);
		read_సూచీ_from(the_repository->సూచీ, సూచీ_file,
				repo_get_git_dir(the_repository));
	}

	తిరిగి 0;
}

/**
 * Builds an index that contains just the blobs needed for a 3way merge.
 */
స్థిర పూర్ణాంకం build_fake_ancestor(స్థిరరాశి నిర్మాణం am_state *state, స్థిరరాశి అక్షరం *సూచీ_file)
{
	నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;

	cp.git_cmd = 1;
	strvec_నెట్టు(&cp.వాదనలు, "apply");
	strvec_pushv(&cp.వాదనలు, state->git_apply_opts.v);
	strvec_pushf(&cp.వాదనలు, "--build-fake-ancestor=%s", సూచీ_file);
	strvec_నెట్టు(&cp.వాదనలు, am_మార్గం(state, "patch"));

	ఒకవేళ (నిరంతరం_command(&cp))
		తిరిగి -1;

	తిరిగి 0;
}

/**
 * Attempt a threeway merge, using index_path as the temporary index.
 */
స్థిర పూర్ణాంకం fall_వెనుక_threeway(స్థిరరాశి నిర్మాణం am_state *state, స్థిరరాశి అక్షరం *సూచీ_మార్గం)
{
	నిర్మాణం object_id their_వృక్షం, our_వృక్షం;
	నిర్మాణం object_id bases[1] = { 0 };
	నిర్మాణం విలీనం_options o;
	నిర్మాణం commit *ఫలితం;
	అక్షరం *their_వృక్షం_name;

	ఒకవేళ (repo_get_oid(the_repository, "HEAD", &our_వృక్షం) < 0)
		oidcpy(&our_వృక్షం, the_hash_algo->ఖాళీలు_వృక్షం);

	ఒకవేళ (build_fake_ancestor(state, సూచీ_మార్గం))
		తిరిగి error("could not build fake ancestor");

	discard_సూచీ(the_repository->సూచీ);
	read_సూచీ_from(the_repository->సూచీ, సూచీ_మార్గం, repo_get_git_dir(the_repository));

	ఒకవేళ (write_సూచీ_as_వృక్షం(&bases[0], the_repository->సూచీ, సూచీ_మార్గం, 0, ఖాళీ))
		తిరిగి error(_("Repository lacks necessary blobs to fall back on 3-way merge."));

	say(state, stdout, _("Using index info to reconstruct a base tree..."));

	ఒకవేళ (!state->quiet) {
		/*
		 * List paths that needed 3-way fallback, so that the user can
		 * review them with extra care to spot mismerges.
		 */
		నిర్మాణం rev_info rev_info;

		repo_init_revisions(the_repository, &rev_info, ఖాళీ);
		rev_info.diffopt.output_format = DIFF_FORMAT_NAME_స్థితి;
		rev_info.diffopt.filter |= diff_filter_బిట్టు('A');
		rev_info.diffopt.filter |= diff_filter_బిట్టు('M');
		కూడు_pending_oid(&rev_info, "HEAD", &our_వృక్షం, 0);
		diff_అమరిక_పూర్తి(&rev_info.diffopt);
		నిరంతరం_diff_సూచీ(&rev_info, DIFF_సూచీ_CACHED);
		release_revisions(&rev_info);
	}

	ఒకవేళ (నిరంతరం_apply(state, సూచీ_మార్గం))
		తిరిగి error(_("Did you hand edit your patch?\n"
				"It does not apply to blobs recorded in its index."));

	ఒకవేళ (write_సూచీ_as_వృక్షం(&their_వృక్షం, the_repository->సూచీ, సూచీ_మార్గం, 0, ఖాళీ))
		తిరిగి error("could not write tree");

	say(state, stdout, _("Falling back to patching base and 3-way merge..."));

	discard_సూచీ(the_repository->సూచీ);
	repo_read_సూచీ(the_repository);

	/*
	 * This is not so wrong. Depending on which base we picked, orig_tree
	 * may be wildly different from ours, but their_tree has the same set of
	 * wildly different changes in parts the patch did not touch, so
	 * recursive ends up canceling them, saying that we reverted all those
	 * changes.
	 */

	init_ui_విలీనం_options(&o, the_repository);

	o.branch1 = "HEAD";
	their_వృక్షం_name = xstrfmt("%.*s", linelen(state->సందేశం), state->సందేశం);
	o.branch2 = their_వృక్షం_name;
	o.ancestor = "constructed fake ancestor";
	o.detect_directory_renames = విలీనం_DIRECTORY_RENAMES_NONE;

	ఒకవేళ (state->quiet)
		o.verbosity = 0;

	ఒకవేళ (విలీనం_ort_generic(&o, &our_వృక్షం, &their_వృక్షం, 1, bases, &ఫలితం)) {
		repo_rerere(the_repository, state->allow_rerere_autoupdate);
		విడుదల(their_వృక్షం_name);
		తిరిగి error(_("Failed to merge in the changes."));
	}

	విడుదల(their_వృక్షం_name);
	తిరిగి 0;
}

/**
 * Commits the current index with state->msg as the commit message and
 * state->author_name, state->author_email and state->author_date as the author
 * information.
 */
స్థిర శూన్య do_commit(స్థిరరాశి నిర్మాణం am_state *state)
{
	నిర్మాణం object_id వృక్షం, మాతృ, commit;
	స్థిరరాశి నిర్మాణం object_id *old_oid;
	నిర్మాణం commit_list *parents = ఖాళీ;
	స్థిరరాశి అక్షరం *reflog_సందేశం, *author, *committer = ఖాళీ;
	నిర్మాణం strbuf sb = STRBUF_INIT;

	ఒకవేళ (!state->no_verify && నిరంతరం_hooks(the_repository, "pre-applypatch"))
		నిష్క్రమించు(1);

	ఒకవేళ (write_సూచీ_as_వృక్షం(&వృక్షం, the_repository->సూచీ,
				repo_get_సూచీ_file(the_repository),
				0, ఖాళీ))
		die(_("git write-tree failed to write a tree"));

	ఒకవేళ (!repo_get_oid_commit(the_repository, "HEAD", &మాతృ)) {
		old_oid = &మాతృ;
		commit_list_చొప్పించు(lookup_commit(the_repository, &మాతృ),
				   &parents);
	} లేకపోతే {
		old_oid = ఖాళీ;
		say(state, stderr, _("applying to an empty history"));
	}

	author = fmt_ident(state->author_name, state->author_email,
		WANT_AUTHOR_IDENT,
			state->ignore_date ? ఖాళీ : state->author_date,
			IDENT_STRICT);

	ఒకవేళ (state->committer_date_is_author_date)
		committer = fmt_ident(getenv("GIT_COMMITTER_NAME"),
				      getenv("GIT_COMMITTER_EMAIL"),
				      WANT_COMMITTER_IDENT,
				      state->ignore_date ? ఖాళీ
							 : state->author_date,
				      IDENT_STRICT);

	ఒకవేళ (commit_వృక్షం_extended(state->సందేశం, state->సందేశం_len, &వృక్షం, parents,
				 &commit, author, committer, state->sign_commit,
				 ఖాళీ))
		die(_("failed to write commit object"));

	reflog_సందేశం = getenv("GIT_REFLOG_ACTION");
	ఒకవేళ (!reflog_సందేశం)
		reflog_సందేశం = "am";

	strbuf_addf(&sb, "%s: %.*s", reflog_సందేశం, linelen(state->సందేశం),
			state->సందేశం);

	refs_update_ref(get_main_ref_store(the_repository), sb.buf, "HEAD",
			&commit, old_oid, 0,
			UPDATE_REFS_DIE_ON_ERR);

	ఒకవేళ (state->rebasing) {
		FILE *fp = xfopen(am_మార్గం(state, "rewritten"), "a");

		ప్రతిజ్ఞా(!is_null_oid(&state->orig_commit));
		fprintf(fp, "%s ", oid_గమ్యం_hex(&state->orig_commit));
		fprintf(fp, "%s\n", oid_గమ్యం_hex(&commit));
		fclose(fp);
	}

	నిరంతరం_hooks(the_repository, "post-applypatch");

	commit_list_free(parents);
	strbuf_release(&sb);
}

/**
 * Validates the am_state for resuming -- the "msg" and authorship fields must
 * be filled up.
 */
స్థిర శూన్య validate_resume_state(స్థిరరాశి నిర్మాణం am_state *state)
{
	ఒకవేళ (!state->సందేశం)
		die(_("cannot resume: %s does not exist."),
			am_మార్గం(state, "final-commit"));

	ఒకవేళ (!state->author_name || !state->author_email || !state->author_date)
		die(_("cannot resume: %s does not exist."),
			am_మార్గం(state, "author-script"));
}

/**
 * Interactively prompt the user on whether the current patch should be
 * applied.
 *
 * Returns 0 if the user chooses to apply the patch, 1 if the user chooses to
 * skip it.
 */
స్థిర పూర్ణాంకం do_interactive(నిర్మాణం am_state *state)
{
	ప్రతిజ్ఞా(state->సందేశం);

	కొరకు (;;) {
		అక్షరం జవాబు[64];

		puts(_("Commit Body is:"));
		puts("--------------------------");
		రాయి("%s", state->సందేశం);
		puts("--------------------------");

		/*
		 * TRANSLATORS: Make sure to include [y], [n], [e], [v] and [a]
		 * in your translation. The program will only accept English
		 * input at this point.
		 */
		రాయి(_("Apply? [y]es/[n]o/[e]dit/[v]iew patch/[a]ccept all: "));
		ఒకవేళ (!fgets(జవాబు, పరిమాణం(జవాబు), stdin))
			die("unable to read from stdin; aborting");

		ఒకవేళ (*జవాబు == 'y' || *జవాబు == 'Y') {
			తిరిగి 0;
		} లేకపోతే ఒకవేళ (*జవాబు == 'a' || *జవాబు == 'A') {
			state->interactive = 0;
			తిరిగి 0;
		} లేకపోతే ఒకవేళ (*జవాబు == 'n' || *జవాబు == 'N') {
			తిరిగి 1;
		} లేకపోతే ఒకవేళ (*జవాబు == 'e' || *జవాబు == 'E') {
			నిర్మాణం strbuf సందేశం = STRBUF_INIT;

			ఒకవేళ (!launch_editor(am_మార్గం(state, "final-commit"), &సందేశం, ఖాళీ)) {
				విడుదల(state->సందేశం);
				state->సందేశం = strbuf_detach(&సందేశం, &state->సందేశం_len);
			}
			strbuf_release(&సందేశం);
		} లేకపోతే ఒకవేళ (*జవాబు == 'v' || *జవాబు == 'V') {
			స్థిరరాశి అక్షరం *pager = git_pager(the_repository, 1);
			నిర్మాణం బిడ్డ_process cp = బిడ్డ_PROCESS_INIT;

			ఒకవేళ (!pager)
				pager = "cat";
			prepare_pager_వాదనలు(&cp, pager);
			strvec_నెట్టు(&cp.వాదనలు, am_మార్గం(state, "patch"));
			నిరంతరం_command(&cp);
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
స్థిర శూన్య am_నిరంతరం(నిర్మాణం am_state *state, పూర్ణాంకం resume)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;

	unlink(am_మార్గం(state, "dirtyindex"));

	ఒకవేళ (repo_refresh_and_write_సూచీ(the_repository, REFRESH_QUIET, 0, 0,
					 ఖాళీ, ఖాళీ, ఖాళీ) < 0)
		die(_("unable to write index file"));

	ఒకవేళ (repo_సూచీ_has_changes(the_repository, ఖాళీ, &sb)) {
		write_state_bool(state, "dirtyindex", 1);
		die(_("Dirty index: cannot apply patches (dirty: %s)"), sb.buf);
	}

	strbuf_release(&sb);

	అయితే (state->cur <= state->చివరి) {
		స్థిరరాశి అక్షరం *mail = am_మార్గం(state, msgnum(state));
		పూర్ణాంకం apply_స్థితి;
		పూర్ణాంకం గమ్యం_keep;

		reset_ident_date();

		ఒకవేళ (!file_exists(mail))
			వెళ్ళు తదుపరి;

		ఒకవేళ (resume) {
			validate_resume_state(state);
		} లేకపోతే {
			పూర్ణాంకం skip;

			ఒకవేళ (state->rebasing)
				skip = parse_mail_rebase(state, mail);
			లేకపోతే
				skip = parse_mail(state, mail);

			ఒకవేళ (skip)
				వెళ్ళు తదుపరి; /* mail should be skipped */

			ఒకవేళ (state->signoff)
				am_append_signoff(state);

			write_author_script(state);
			write_commit_సందేశం(state);
		}

		ఒకవేళ (state->interactive && do_interactive(state))
			వెళ్ళు తదుపరి;

		గమ్యం_keep = 0;
		ఒకవేళ (is_ఖాళీలు_or_missing_file(am_మార్గం(state, "patch"))) {
			ఎంపిక (state->ఖాళీలు_type) {
			స్థితి DROP_ఖాళీలు_COMMIT:
				say(state, stdout, _("Skipping: %.*s"), linelen(state->సందేశం), state->సందేశం);
				వెళ్ళు తదుపరి;
				ఆపు;
			స్థితి KEEP_ఖాళీలు_COMMIT:
				గమ్యం_keep = 1;
				say(state, stdout, _("Creating an empty commit: %.*s"),
					linelen(state->సందేశం), state->సందేశం);
				ఆపు;
			స్థితి STOP_ON_ఖాళీలు_COMMIT:
				printf_ln(_("Patch is empty."));
				die_user_resolve(state);
				ఆపు;
			}
		}

		ఒకవేళ (నిరంతరం_applypatch_సందేశం_hook(state))
			నిష్క్రమించు(1);
		ఒకవేళ (గమ్యం_keep)
			వెళ్ళు commit;

		say(state, stdout, _("Applying: %.*s"), linelen(state->సందేశం), state->సందేశం);

		apply_స్థితి = నిరంతరం_apply(state, ఖాళీ);

		ఒకవేళ (apply_స్థితి && state->threeway) {
			నిర్మాణం strbuf sb = STRBUF_INIT;

			strbuf_addstr(&sb, am_మార్గం(state, "patch-merge-index"));
			apply_స్థితి = fall_వెనుక_threeway(state, sb.buf);
			strbuf_release(&sb);

			/*
			 * Applying the patch to an earlier tree and merging
			 * the result may have produced the same tree as ours.
			 */
			ఒకవేళ (!apply_స్థితి &&
			    !repo_సూచీ_has_changes(the_repository, ఖాళీ, ఖాళీ)) {
				say(state, stdout, _("No changes -- Patch already applied."));
				వెళ్ళు తదుపరి;
			}
		}

		ఒకవేళ (apply_స్థితి) {
			printf_ln(_("Patch failed at %s %.*s"), msgnum(state),
				linelen(state->సందేశం), state->సందేశం);

			ఒకవేళ (advice_enabled(ADVICE_AM_WORK_DIR))
				advise(_("Use 'git am --show-current-patch=diff' to see the failed patch"));

			die_user_resolve(state);
		}

commit:
		do_commit(state);

తదుపరి:
		am_తదుపరి(state);

		ఒకవేళ (resume)
			am_ఎక్కించు(state);
		resume = 0;
	}

	ఒకవేళ (!is_ఖాళీలు_or_missing_file(am_మార్గం(state, "rewritten"))) {
		ప్రతిజ్ఞా(state->rebasing);
		copy_notes_for_rebase(state);
		నిరంతరం_post_rewrite_hook(state);
	}

	/*
	 * In rebasing mode, it's up to the caller to take care of
	 * housekeeping.
	 */
	ఒకవేళ (!state->rebasing) {
		am_destroy(state);
		నిరంతరం_auto_maintenance(the_repository, state->quiet);
	}
}

/**
 * Resume the current am session after patch application failure. The user did
 * all the hard work, and we do not have to do any patch application. Just
 * trust and commit what the user has in the index and working tree. If `allow_empty`
 * is true, commit as an empty commit when index has not changed and lacking a patch.
 */
స్థిర శూన్య am_resolve(నిర్మాణం am_state *state, పూర్ణాంకం allow_ఖాళీలు)
{
	validate_resume_state(state);

	say(state, stdout, _("Applying: %.*s"), linelen(state->సందేశం), state->సందేశం);

	ఒకవేళ (!repo_సూచీ_has_changes(the_repository, ఖాళీ, ఖాళీ)) {
		ఒకవేళ (allow_ఖాళీలు && is_ఖాళీలు_or_missing_file(am_మార్గం(state, "patch"))) {
			printf_ln(_("No changes - recorded it as an empty commit."));
		} లేకపోతే {
			printf_ln(_("No changes - did you forget to use 'git add'?\n"
				    "If there is nothing left to stage, chances are that something else\n"
				    "already introduced the same changes; you might want to skip this patch."));
			die_user_resolve(state);
		}
	}

	ఒకవేళ (unmerged_సూచీ(the_repository->సూచీ)) {
		printf_ln(_("You still have unmerged paths in your index.\n"
			"You should 'git add' each file with resolved conflicts to mark them as such.\n"
			"You might run `git rm` on a file to accept \"deleted by them\" for it."));
		die_user_resolve(state);
	}

	ఒకవేళ (state->interactive) {
		write_సూచీ_patch(state);
		ఒకవేళ (do_interactive(state))
			వెళ్ళు తదుపరి;
	}

	repo_rerere(the_repository, 0);

	do_commit(state);

తదుపరి:
	am_తదుపరి(state);
	am_ఎక్కించు(state);
	am_నిరంతరం(state, 0);
}

/**
 * Performs a checkout fast-forward from `head` to `remote`. If `reset` is
 * true, any unmerged entries will be discarded. Returns 0 on success, -1 on
 * failure.
 */
స్థిర పూర్ణాంకం fast_forward_గమ్యం(నిర్మాణం వృక్షం *తల, నిర్మాణం వృక్షం *remote, పూర్ణాంకం reset)
{
	నిర్మాణం తాళం_file తాళం_file = తాళం_INIT;
	నిర్మాణం unpack_trees_options opts;
	నిర్మాణం వృక్షం_desc t[2];

	ఒకవేళ (repo_parse_వృక్షం(the_repository, తల) || repo_parse_వృక్షం(the_repository, remote))
		తిరిగి -1;

	repo_hold_locked_సూచీ(the_repository, &తాళం_file, తాళం_DIE_ON_ERROR);

	refresh_సూచీ(the_repository->సూచీ, REFRESH_QUIET, ఖాళీ, ఖాళీ, ఖాళీ);

	memset(&opts, 0, పరిమాణం(opts));
	opts.తల_idx = 1;
	opts.src_సూచీ = the_repository->సూచీ;
	opts.dst_సూచీ = the_repository->సూచీ;
	opts.update = 1;
	opts.విలీనం = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = twoway_విలీనం;
	init_వృక్షం_desc(&t[0], &తల->object.oid, తల->బఫరు, తల->కొలత);
	init_వృక్షం_desc(&t[1], &remote->object.oid, remote->బఫరు, remote->కొలత);

	ఒకవేళ (unpack_trees(2, t, &opts)) {
		rollback_తాళం_file(&తాళం_file);
		తిరిగి -1;
	}

	ఒకవేళ (write_locked_సూచీ(the_repository->సూచీ, &తాళం_file, COMMIT_తాళం))
		die(_("unable to write new index file"));

	తిరిగి 0;
}

/**
 * Merges a tree into the index. The index's stat info will take precedence
 * over the merged tree's. Returns 0 on success, -1 on failure.
 */
స్థిర పూర్ణాంకం విలీనం_వృక్షం(నిర్మాణం వృక్షం *వృక్షం)
{
	నిర్మాణం తాళం_file తాళం_file = తాళం_INIT;
	నిర్మాణం unpack_trees_options opts;
	నిర్మాణం వృక్షం_desc t[1];

	ఒకవేళ (repo_parse_వృక్షం(the_repository, వృక్షం))
		తిరిగి -1;

	repo_hold_locked_సూచీ(the_repository, &తాళం_file, తాళం_DIE_ON_ERROR);

	memset(&opts, 0, పరిమాణం(opts));
	opts.తల_idx = 1;
	opts.src_సూచీ = the_repository->సూచీ;
	opts.dst_సూచీ = the_repository->సూచీ;
	opts.విలీనం = 1;
	opts.fn = oneway_విలీనం;
	init_వృక్షం_desc(&t[0], &వృక్షం->object.oid, వృక్షం->బఫరు, వృక్షం->కొలత);

	ఒకవేళ (unpack_trees(1, t, &opts)) {
		rollback_తాళం_file(&తాళం_file);
		తిరిగి -1;
	}

	ఒకవేళ (write_locked_సూచీ(the_repository->సూచీ, &తాళం_file, COMMIT_తాళం))
		die(_("unable to write new index file"));

	తిరిగి 0;
}

/**
 * Clean the index without touching entries that are not modified between
 * `head` and `remote`.
 */
స్థిర పూర్ణాంకం clean_సూచీ(స్థిరరాశి నిర్మాణం object_id *తల, స్థిరరాశి నిర్మాణం object_id *remote)
{
	నిర్మాణం వృక్షం *తల_వృక్షం, *remote_వృక్షం, *సూచీ_వృక్షం;
	నిర్మాణం object_id సూచీ;

	తల_వృక్షం = repo_parse_వృక్షం_పరోక్ష(the_repository, తల);
	ఒకవేళ (!తల_వృక్షం)
		తిరిగి error(_("Could not parse object '%s'."), oid_గమ్యం_hex(తల));

	remote_వృక్షం = repo_parse_వృక్షం_పరోక్ష(the_repository, remote);
	ఒకవేళ (!remote_వృక్షం)
		తిరిగి error(_("Could not parse object '%s'."), oid_గమ్యం_hex(remote));

	repo_read_సూచీ_unmerged(the_repository);

	ఒకవేళ (fast_forward_గమ్యం(తల_వృక్షం, తల_వృక్షం, 1))
		తిరిగి -1;

	ఒకవేళ (write_సూచీ_as_వృక్షం(&సూచీ, the_repository->సూచీ,
				repo_get_సూచీ_file(the_repository),
				0, ఖాళీ))
		తిరిగి -1;

	సూచీ_వృక్షం = repo_parse_వృక్షం_పరోక్ష(the_repository, &సూచీ);
	ఒకవేళ (!సూచీ_వృక్షం)
		తిరిగి error(_("Could not parse object '%s'."), oid_గమ్యం_hex(&సూచీ));

	ఒకవేళ (fast_forward_గమ్యం(సూచీ_వృక్షం, remote_వృక్షం, 0))
		తిరిగి -1;

	ఒకవేళ (విలీనం_వృక్షం(remote_వృక్షం))
		తిరిగి -1;

	remove_branch_state(the_repository, 0);

	తిరిగి 0;
}

/**
 * Resets rerere's merge resolution metadata.
 */
స్థిర శూన్య am_rerere_clear(శూన్య)
{
	నిర్మాణం string_list విలీనం_rr = STRING_LIST_INIT_DUP;
	rerere_clear(the_repository, &విలీనం_rr);
	string_list_clear(&విలీనం_rr, 1);
}

/**
 * Resume the current am session by skipping the current patch.
 */
స్థిర శూన్య am_skip(నిర్మాణం am_state *state)
{
	నిర్మాణం object_id తల;

	am_rerere_clear();

	ఒకవేళ (repo_get_oid(the_repository, "HEAD", &తల))
		oidcpy(&తల, the_hash_algo->ఖాళీలు_వృక్షం);

	ఒకవేళ (clean_సూచీ(&తల, &తల))
		die(_("failed to clean index"));

	ఒకవేళ (state->rebasing) {
		FILE *fp = xfopen(am_మార్గం(state, "rewritten"), "a");

		ప్రతిజ్ఞా(!is_null_oid(&state->orig_commit));
		fprintf(fp, "%s ", oid_గమ్యం_hex(&state->orig_commit));
		fprintf(fp, "%s\n", oid_గమ్యం_hex(&తల));
		fclose(fp);
	}

	am_తదుపరి(state);
	am_ఎక్కించు(state);
	am_నిరంతరం(state, 0);
}

/**
 * Returns true if it is safe to reset HEAD to the ORIG_HEAD, false otherwise.
 *
 * It is not safe to reset HEAD when:
 * 1. git-am previously failed because the index was dirty.
 * 2. HEAD has moved since git-am previously failed.
 */
స్థిర పూర్ణాంకం సురక్షిత_గమ్యం_abort(స్థిరరాశి నిర్మాణం am_state *state)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;
	నిర్మాణం object_id abort_safety, తల;

	ఒకవేళ (file_exists(am_మార్గం(state, "dirtyindex")))
		తిరిగి 0;

	ఒకవేళ (read_state_file(&sb, state, "abort-safety", 1) > 0) {
		ఒకవేళ (get_oid_hex(sb.buf, &abort_safety))
			die(_("could not parse %s"), am_మార్గం(state, "abort-safety"));
	} లేకపోతే
		oidclr(&abort_safety, the_repository->hash_algo);
	strbuf_release(&sb);

	ఒకవేళ (repo_get_oid(the_repository, "HEAD", &తల))
		oidclr(&తల, the_repository->hash_algo);

	ఒకవేళ (oideq(&తల, &abort_safety))
		తిరిగి 1;

	warning(_("You seem to have moved HEAD since the last 'am' failure.\n"
		"Not rewinding to ORIG_HEAD"));

	తిరిగి 0;
}

/**
 * Aborts the current am session if it is safe to do so.
 */
స్థిర శూన్య am_abort(నిర్మాణం am_state *state)
{
	నిర్మాణం object_id curr_తల, orig_తల;
	పూర్ణాంకం has_curr_తల, has_orig_తల;
	అక్షరం *curr_branch;

	ఒకవేళ (!సురక్షిత_గమ్యం_abort(state)) {
		am_destroy(state);
		తిరిగి;
	}

	am_rerere_clear();

	curr_branch = refs_resolve_refdup(get_main_ref_store(the_repository),
					  "HEAD", 0, &curr_తల, ఖాళీ);
	has_curr_తల = curr_branch && !is_null_oid(&curr_తల);
	ఒకవేళ (!has_curr_తల)
		oidcpy(&curr_తల, the_hash_algo->ఖాళీలు_వృక్షం);

	has_orig_తల = !repo_get_oid(the_repository, "ORIG_HEAD", &orig_తల);
	ఒకవేళ (!has_orig_తల)
		oidcpy(&orig_తల, the_hash_algo->ఖాళీలు_వృక్షం);

	ఒకవేళ (clean_సూచీ(&curr_తల, &orig_తల))
		die(_("failed to clean index"));

	ఒకవేళ (has_orig_తల)
		refs_update_ref(get_main_ref_store(the_repository),
				"am --abort", "HEAD", &orig_తల,
				has_curr_తల ? &curr_తల : ఖాళీ, 0,
				UPDATE_REFS_DIE_ON_ERR);
	లేకపోతే ఒకవేళ (curr_branch)
		refs_delete_ref(get_main_ref_store(the_repository), ఖాళీ,
				curr_branch, ఖాళీ, REF_NO_DEREF);

	విడుదల(curr_branch);
	am_destroy(state);
}

స్థిర పూర్ణాంకం show_patch(నిర్మాణం am_state *state, గణన resume_type resume_బహుళకం)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;
	స్థిరరాశి అక్షరం *patch_మార్గం;
	పూర్ణాంకం len;

	ఒకవేళ (!is_null_oid(&state->orig_commit)) {
		నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;

		strvec_pushl(&cmd.వాదనలు, "show", oid_గమ్యం_hex(&state->orig_commit),
			     "--", ఖాళీ);
		cmd.git_cmd = 1;
		తిరిగి నిరంతరం_command(&cmd);
	}

	ఎంపిక (resume_బహుళకం) {
	స్థితి RESUME_SHOW_PATCH_RAW:
		patch_మార్గం = am_మార్గం(state, msgnum(state));
		ఆపు;
	స్థితి RESUME_SHOW_PATCH_DIFF:
		patch_మార్గం = am_మార్గం(state, "patch");
		ఆపు;
	అప్రమేయ:
		BUG("invalid mode for --show-current-patch");
	}

	len = strbuf_read_file(&sb, patch_మార్గం, 0);
	ఒకవేళ (len < 0)
		die_errno(_("failed to read '%s'"), patch_మార్గం);

	అమరిక_pager(the_repository);
	write_లోపల_నిండిన(1, sb.buf, sb.len);
	strbuf_release(&sb);
	తిరిగి 0;
}

/**
 * parse_options() callback that validates and sets opt->value to the
 * PATCH_FORMAT_* enum value corresponding to `arg`.
 */
స్థిర పూర్ణాంకం parse_opt_patchformat(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	పూర్ణాంకం *opt_విలువ = opt->విలువ;

	ఒకవేళ (unset)
		*opt_విలువ = PATCH_FORMAT_UNKNOWN;
	లేకపోతే ఒకవేళ (!strcmp(arg, "mbox"))
		*opt_విలువ = PATCH_FORMAT_MBOX;
	లేకపోతే ఒకవేళ (!strcmp(arg, "stgit"))
		*opt_విలువ = PATCH_FORMAT_STGIT;
	లేకపోతే ఒకవేళ (!strcmp(arg, "stgit-series"))
		*opt_విలువ = PATCH_FORMAT_STGIT_SERIES;
	లేకపోతే ఒకవేళ (!strcmp(arg, "hg"))
		*opt_విలువ = PATCH_FORMAT_HG;
	లేకపోతే ఒకవేళ (!strcmp(arg, "mboxrd"))
		*opt_విలువ = PATCH_FORMAT_MBOXRD;
	/*
	 * Please update $__git_patchformat in git-completion.bash
	 * when you add new options
	 */
	లేకపోతే
		తిరిగి error(_("invalid value for '%s': '%s'"),
			     "--patch-format", arg);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం parse_opt_show_current_patch(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	పూర్ణాంకం *opt_విలువ = opt->విలువ;

	BUG_ON_OPT_NEG(unset);

	ఒకవేళ (!arg)
		*opt_విలువ = opt->defval;
	లేకపోతే ఒకవేళ (!strcmp(arg, "raw"))
		*opt_విలువ = RESUME_SHOW_PATCH_RAW;
	లేకపోతే ఒకవేళ (!strcmp(arg, "diff"))
		*opt_విలువ = RESUME_SHOW_PATCH_DIFF;
	/*
	 * Please update $__git_showcurrentpatch in git-completion.bash
	 * when you add new options
	 */
	లేకపోతే
		తిరిగి error(_("invalid value for '%s': '%s'"),
			     "--show-current-patch", arg);
	తిరిగి 0;
}

పూర్ణాంకం cmd_am(పూర్ణాంకం argc,
	   స్థిరరాశి అక్షరం **argv,
	   స్థిరరాశి అక్షరం *ఉపసర్గ,
	   నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం am_state state;
	పూర్ణాంకం ద్విమాన = -1;
	పూర్ణాంకం keep_cr = -1;
	పూర్ణాంకం patch_format = PATCH_FORMAT_UNKNOWN;
	గణన resume_type resume_బహుళకం = RESUME_FALSE;
	పూర్ణాంకం లోపల_పురోగతి;
	పూర్ణాంకం ret = 0;

	స్థిరరాశి అక్షరం * స్థిరరాశి usage[] = {
		N_("git am [<options>] [(<mbox> | <Maildir>)...]"),
		N_("git am [<options>] (--continue | --skip | --abort)"),
		ఖాళీ
	};

	నిర్మాణం option options[] = {
		OPT_BOOL('i', "interactive", &state.interactive,
			N_("run interactively")),
		OPT_BOOL('n', "no-verify", &state.no_verify,
			N_("bypass pre-applypatch and applypatch-msg hooks")),
		OPT_HIDDEN_BOOL('b', "binary", &ద్విమాన,
			N_("historical option -- no-op")),
		OPT_BOOL('3', "3way", &state.threeway,
			N_("allow fall back on 3way merging if needed")),
		OPT__QUIET(&state.quiet, N_("be quiet")),
		OPT_సమితి_INT('s', "signoff", &state.signoff,
			N_("add a Signed-off-by trailer to the commit message"),
			SIGNOFF_EXPLICIT),
		OPT_BOOL('u', "utf8", &state.utf8,
			N_("recode into utf8 (default)")),
		OPT_సమితి_INT('k', "keep", &state.keep,
			N_("pass -k flag to git-mailinfo"), KEEP_TRUE),
		OPT_సమితి_INT(0, "keep-non-patch", &state.keep,
			N_("pass -b flag to git-mailinfo"), KEEP_NON_PATCH),
		OPT_BOOL('m', "message-id", &state.సందేశం_id,
			N_("pass -m flag to git-mailinfo")),
		OPT_సమితి_INT(0, "keep-cr", &keep_cr,
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
		OPT_PASSTHRU_ARGV(0, "ignore-space-change", &state.git_apply_opts, ఖాళీ,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU_ARGV(0, "ignore-whitespace", &state.git_apply_opts, ఖాళీ,
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
		OPT_PASSTHRU_ARGV('C', ఖాళీ, &state.git_apply_opts, N_("n"),
			N_("pass it through git-apply"),
			0),
		OPT_PASSTHRU_ARGV('p', ఖాళీ, &state.git_apply_opts, N_("num"),
			N_("pass it through git-apply"),
			0),
		OPT_CALLBACK(0, "patch-format", &patch_format, N_("format"),
			N_("format the patch(es) are in"),
			parse_opt_patchformat),
		OPT_PASSTHRU_ARGV(0, "reject", &state.git_apply_opts, ఖాళీ,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_STRING(0, "resolvemsg", &state.resolvemsg, ఖాళీ,
			N_("override error message when patch failure occurs")),
		OPT_CMDMODE(0, "continue", &resume_బహుళకం,
			N_("continue applying patches after resolving a conflict"),
			RESUME_RESOLVED),
		OPT_CMDMODE('r', "resolved", &resume_బహుళకం,
			N_("synonyms for --continue"),
			RESUME_RESOLVED),
		OPT_CMDMODE(0, "skip", &resume_బహుళకం,
			N_("skip the current patch"),
			RESUME_SKIP),
		OPT_CMDMODE(0, "abort", &resume_బహుళకం,
			N_("restore the original branch and abort the patching operation"),
			RESUME_ABORT),
		OPT_CMDMODE(0, "quit", &resume_బహుళకం,
			N_("abort the patching operation but keep HEAD where it is"),
			RESUME_QUIT),
		{
			.type = OPTION_CALLBACK,
			.long_name = "show-current-patch",
			.విలువ = &resume_బహుళకం,
			.precision = పరిమాణం(resume_బహుళకం),
			.argh = "(diff|raw)",
			.help = N_("show the patch being applied"),
			.flags = PARSE_OPT_CMDMODE | PARSE_OPT_OPTARG | PARSE_OPT_NONEG | PARSE_OPT_LITERAL_ARGHELP,
			.callback = parse_opt_show_current_patch,
			.defval = RESUME_SHOW_PATCH_RAW,
		},
		OPT_CMDMODE(0, "retry", &resume_బహుళకం,
			N_("try to apply current patch again"),
			RESUME_APPLY),
		OPT_CMDMODE(0, "allow-empty", &resume_బహుళకం,
			N_("record the empty patch as an empty commit"),
			RESUME_ALLOW_ఖాళీలు),
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
			.విలువ = &state.sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG-sign commits"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_CALLBACK_F(0, "empty", &state.ఖాళీలు_type, "(stop|drop|keep)",
		  N_("how to handle empty patches"),
		  PARSE_OPT_NONEG, am_option_parse_ఖాళీలు),
		OPT_HIDDEN_BOOL(0, "rebasing", &state.rebasing,
			N_("(internal use for git-rebase)")),
		OPT_చివర()
	};

	show_usage_with_options_if_asked(argc, argv, usage, options);

	repo_config(the_repository, git_default_config, ఖాళీ);

	am_state_init(&state);

	లోపల_పురోగతి = am_లోపల_పురోగతి(&state);
	ఒకవేళ (లోపల_పురోగతి)
		am_ఎక్కించు(&state);

	argc = parse_options(argc, argv, ఉపసర్గ, options, usage, 0);

	ఒకవేళ (ద్విమాన >= 0)
		fprintf_ln(stderr, _("The -b/--binary option has been a no-op for long time, and\n"
				"it will be removed. Please do not use it anymore."));

	/* Ensure a valid committer ident can be constructed */
	git_committer_info(IDENT_STRICT);

	ఒకవేళ (repo_read_సూచీ_preload(the_repository, ఖాళీ, 0) < 0)
		die(_("failed to read the index"));

	ఒకవేళ (లోపల_పురోగతి) {
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
		ఒకవేళ (argc || (resume_బహుళకం == RESUME_FALSE && !isatty(0)))
			die(_("previous rebase directory %s still exists but mbox given."),
				state.dir);

		ఒకవేళ (resume_బహుళకం == RESUME_FALSE)
			resume_బహుళకం = RESUME_APPLY;

		ఒకవేళ (state.signoff == SIGNOFF_EXPLICIT)
			am_append_signoff(&state);
	} లేకపోతే {
		నిర్మాణం strvec paths = STRVEC_INIT;
		పూర్ణాంకం i;

		/*
		 * Handle stray state directory in the independent-run case. In
		 * the --rebasing case, it is up to the caller to take care of
		 * stray directories.
		 */
		ఒకవేళ (file_exists(state.dir) && !state.rebasing) {
			ఒకవేళ (resume_బహుళకం == RESUME_ABORT || resume_బహుళకం == RESUME_QUIT) {
				am_destroy(&state);
				am_state_release(&state);
				తిరిగి 0;
			}

			die(_("Stray %s directory found.\n"
				"Use \"git am --abort\" to remove it."),
				state.dir);
		}

		ఒకవేళ (resume_బహుళకం)
			die(_("Resolve operation not in progress, we are not resuming."));

		కొరకు (i = 0; i < argc; i++) {
			ఒకవేళ (is_absolute_మార్గం(argv[i]) || !ఉపసర్గ)
				strvec_నెట్టు(&paths, argv[i]);
			లేకపోతే
				strvec_నెట్టు(&paths, mkpath("%s/%s", ఉపసర్గ, argv[i]));
		}

		ఒకవేళ (state.interactive && !paths.nr)
			die(_("interactive mode requires patches on the command line"));

		am_అమరిక(&state, patch_format, paths.v, keep_cr);

		strvec_clear(&paths);
	}

	ఎంపిక (resume_బహుళకం) {
	స్థితి RESUME_FALSE:
		am_నిరంతరం(&state, 0);
		ఆపు;
	స్థితి RESUME_APPLY:
		am_నిరంతరం(&state, 1);
		ఆపు;
	స్థితి RESUME_RESOLVED:
	స్థితి RESUME_ALLOW_ఖాళీలు:
		am_resolve(&state, resume_బహుళకం == RESUME_ALLOW_ఖాళీలు ? 1 : 0);
		ఆపు;
	స్థితి RESUME_SKIP:
		am_skip(&state);
		ఆపు;
	స్థితి RESUME_ABORT:
		am_abort(&state);
		ఆపు;
	స్థితి RESUME_QUIT:
		am_rerere_clear();
		am_destroy(&state);
		ఆపు;
	స్థితి RESUME_SHOW_PATCH_RAW:
	స్థితి RESUME_SHOW_PATCH_DIFF:
		ret = show_patch(&state, resume_బహుళకం);
		ఆపు;
	అప్రమేయ:
		BUG("invalid resume value");
	}

	am_state_release(&state);

	తిరిగి ret;
}
