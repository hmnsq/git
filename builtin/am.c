/*
 * Builtin "git am"
 *
 * Based on git-am.sh by Junio C Hamano.
 */

#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "abspath.h"
#અન્તર્ભાવ "advice.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "editor.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "dir.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "hook.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "tempfile.h"
#અન્તર્ભાવ "lockfile.h"
#અન્તર્ભાવ "cache-tree.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "unpack-trees.h"
#અન્તર્ભાવ "branch.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "preload-index.h"
#અન્તર્ભાવ "sequencer.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "merge-ort-wrappers.h"
#અન્તર્ભાવ "log-tree.h"
#અન્તર્ભાવ "notes-utils.h"
#અન્તર્ભાવ "rerere.h"
#અન્તર્ભાવ "mailinfo.h"
#અન્તર્ભાવ "apply.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "pager.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "pretty.h"

/**
 * Returns the length of the first line of msg.
 */
સ્થિર પૂર્ણાંક linelen(અચલ અક્ષર *સંદેશ)
{
	પાછા strchrnul(સંદેશ, '\n') - સંદેશ;
}

/**
 * Returns true if `str` consists of only whitespace, false otherwise.
 */
સ્થિર પૂર્ણાંક str_isspace(અચલ અક્ષર *str)
{
	માટે (; *str; str++)
		જો (!isspace(*str))
			પાછા 0;

	પાછા 1;
}

ગણના patch_format {
	PATCH_FORMAT_UNKNOWN = 0,
	PATCH_FORMAT_MBOX,
	PATCH_FORMAT_STGIT,
	PATCH_FORMAT_STGIT_SERIES,
	PATCH_FORMAT_HG,
	PATCH_FORMAT_MBOXRD
};

ગણના keep_type {
	KEEP_FALSE = 0,
	KEEP_TRUE,      /* pass -k flag to git-mailinfo */
	KEEP_NON_PATCH  /* pass -b flag to git-mailinfo */
};

ગણના scissors_type {
	SCISSORS_UNSET = -1,
	SCISSORS_FALSE = 0,  /* pass --no-scissors to git-mailinfo */
	SCISSORS_TRUE        /* pass --scissors to git-mailinfo */
};

ગણના signoff_type {
	SIGNOFF_FALSE = 0,
	SIGNOFF_TRUE = 1,
	SIGNOFF_EXPLICIT /* --signoff was set on the command-line */
};

ગણના resume_type {
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

ગણના empty_action {
	STOP_ON_EMPTY_COMMIT = 0,  /* output errors and stop in the middle of an am session */
	DROP_EMPTY_COMMIT,         /* skip with a notice message, unless "--quiet" has been passed */
	KEEP_EMPTY_COMMIT,         /* keep recording as empty commits */
};

સંરચના am_state {
	/* state directory path */
	અક્ષર *dir;

	/* current and last patch numbers, 1-indexed */
	પૂર્ણાંક cur;
	પૂર્ણાંક last;

	/* commit metadata and message */
	અક્ષર *author_name;
	અક્ષર *author_email;
	અક્ષર *author_date;
	અક્ષર *સંદેશ;
	size_t સંદેશ_len;

	/* when --rebasing, records the original commit the patch came from */
	સંરચના object_id orig_commit;

	/* number of digits in patch filename */
	પૂર્ણાંક prec;

	/* various operating modes and command line options */
	પૂર્ણાંક interactive;
	પૂર્ણાંક no_verify;
	પૂર્ણાંક threeway;
	પૂર્ણાંક quiet;
	પૂર્ણાંક signoff; /* enum signoff_type */
	પૂર્ણાંક utf8;
	પૂર્ણાંક keep; /* enum keep_type */
	પૂર્ણાંક message_id;
	પૂર્ણાંક scissors; /* enum scissors_type */
	પૂર્ણાંક quoted_cr; /* enum quoted_cr_action */
	પૂર્ણાંક empty_type; /* enum empty_action */
	સંરચના strvec git_apply_opts;
	અચલ અક્ષર *resolvemsg;
	પૂર્ણાંક committer_date_is_author_date;
	પૂર્ણાંક ignore_date;
	પૂર્ણાંક allow_rerere_autoupdate;
	અચલ અક્ષર *sign_commit;
	પૂર્ણાંક rebasing;
};

/**
 * Initializes am_state with the default values.
 */
સ્થિર શૂન્ય am_state_init(સંરચના am_state *state)
{
	પૂર્ણાંક gpgsign;

	memset(state, 0, કદ(*state));

	state->dir = repo_git_path(the_repository, "rebase-apply");

	state->prec = 4;

	repo_config_get_bool(the_repository, "am.threeway", &state->threeway);

	state->utf8 = 1;

	repo_config_get_bool(the_repository, "am.messageid", &state->message_id);

	state->scissors = SCISSORS_UNSET;
	state->quoted_cr = quoted_cr_unset;

	strvec_init(&state->git_apply_opts);

	જો (!repo_config_get_bool(the_repository, "commit.gpgsign", &gpgsign))
		state->sign_commit = gpgsign ? "" : ખાલી;
}

/**
 * Releases memory allocated by an am_state.
 */
સ્થિર શૂન્ય am_state_release(સંરચના am_state *state)
{
	મુક્ત(state->dir);
	મુક્ત(state->author_name);
	મુક્ત(state->author_email);
	મુક્ત(state->author_date);
	મુક્ત(state->સંદેશ);
	strvec_clear(&state->git_apply_opts);
}

સ્થિર પૂર્ણાંક am_option_parse_quoted_cr(અચલ સંરચના option *opt,
				     અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	BUG_ON_OPT_NEG(unset);

	જો (mailinfo_parse_quoted_cr_action(arg, opt->કિંમત) != 0)
		પાછા error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	પાછા 0;
}

સ્થિર પૂર્ણાંક am_option_parse_empty(અચલ સંરચના option *opt,
				     અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	પૂર્ણાંક *opt_કિંમત = opt->કિંમત;

	BUG_ON_OPT_NEG(unset);

	જો (!strcmp(arg, "stop"))
		*opt_કિંમત = STOP_ON_EMPTY_COMMIT;
	નહીંતર જો (!strcmp(arg, "drop"))
		*opt_કિંમત = DROP_EMPTY_COMMIT;
	નહીંતર જો (!strcmp(arg, "keep"))
		*opt_કિંમત = KEEP_EMPTY_COMMIT;
	નહીંતર
		પાછા error(_("invalid value for '%s': '%s'"), "--empty", arg);

	પાછા 0;
}

/**
 * Returns path relative to the am_state directory.
 */
સ્થિર અંતઃરેખ અચલ અક્ષર *am_path(અચલ સંરચના am_state *state, અચલ અક્ષર *path)
{
	પાછા mkpath("%s/%s", state->dir, path);
}

/**
 * For convenience to call write_file()
 */
સ્થિર શૂન્ય write_state_text(અચલ સંરચના am_state *state,
			     અચલ અક્ષર *name, અચલ અક્ષર *string)
{
	write_file(am_path(state, name), "%s", string);
}

સ્થિર શૂન્ય write_state_ગણતરી(અચલ સંરચના am_state *state,
			      અચલ અક્ષર *name, પૂર્ણાંક કિંમત)
{
	write_file(am_path(state, name), "%d", કિંમત);
}

સ્થિર શૂન્ય write_state_bool(અચલ સંરચના am_state *state,
			     અચલ અક્ષર *name, પૂર્ણાંક કિંમત)
{
	write_state_text(state, name, કિંમત ? "t" : "f");
}

/**
 * If state->quiet is false, calls fprintf(fp, fmt, ...), and appends a newline
 * at the end.
 */
__attribute__((format (printf, 3, 4)))
સ્થિર શૂન્ય say(અચલ સંરચના am_state *state, FILE *fp, અચલ અક્ષર *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	જો (!state->quiet) {
		vfprintf(fp, fmt, ap);
		putc('\n', fp);
	}
	va_end(ap);
}

/**
 * Returns 1 if there is an am session in progress, 0 otherwise.
 */
સ્થિર પૂર્ણાંક am_in_progress(અચલ સંરચના am_state *state)
{
	સંરચના stat st;

	જો (lstat(state->dir, &st) < 0 || !S_ISDIR(st.st_mode))
		પાછા 0;
	જો (lstat(am_path(state, "last"), &st) || !S_ISREG(st.st_mode))
		પાછા 0;
	જો (lstat(am_path(state, "next"), &st) || !S_ISREG(st.st_mode))
		પાછા 0;
	પાછા 1;
}

/**
 * Reads the contents of `file` in the `state` directory into `sb`. Returns the
 * number of bytes read on success, -1 if the file does not exist. If `trim` is
 * set, trailing whitespace will be removed.
 */
સ્થિર પૂર્ણાંક read_state_file(સંરચના strbuf *sb, અચલ સંરચના am_state *state,
			અચલ અક્ષર *file, પૂર્ણાંક trim)
{
	strbuf_reset(sb);

	જો (strbuf_read_file(sb, am_path(state, file), 0) >= 0) {
		જો (trim)
			strbuf_trim(sb);

		પાછા sb->len;
	}

	જો (errno == ENOENT)
		પાછા -1;

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
સ્થિર પૂર્ણાંક read_am_author_script(સંરચના am_state *state)
{
	અચલ અક્ષર *filename = am_path(state, "author-script");

	પ્રતિજ્ઞા(!state->author_name);
	પ્રતિજ્ઞા(!state->author_email);
	પ્રતિજ્ઞા(!state->author_date);

	પાછા read_author_script(filename, &state->author_name,
				  &state->author_email, &state->author_date, 1);
}

/**
 * Saves state->author_name, state->author_email and state->author_date in the
 * state directory's "author-script" file.
 */
સ્થિર શૂન્ય write_author_script(અચલ સંરચના am_state *state)
{
	સંરચના strbuf sb = STRBUF_INIT;

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
સ્થિર પૂર્ણાંક read_commit_સંદેશ(સંરચના am_state *state)
{
	સંરચના strbuf sb = STRBUF_INIT;

	પ્રતિજ્ઞા(!state->સંદેશ);

	જો (read_state_file(&sb, state, "final-commit", 0) < 0) {
		strbuf_release(&sb);
		પાછા -1;
	}

	state->સંદેશ = strbuf_detach(&sb, &state->સંદેશ_len);
	પાછા 0;
}

/**
 * Saves state->msg in the state directory's "final-commit" file.
 */
સ્થિર શૂન્ય write_commit_સંદેશ(અચલ સંરચના am_state *state)
{
	અચલ અક્ષર *filename = am_path(state, "final-commit");
	write_file_buf(filename, state->સંદેશ, state->સંદેશ_len);
}

/**
 * Loads state from disk.
 */
સ્થિર શૂન્ય am_load(સંરચના am_state *state)
{
	સંરચના strbuf sb = STRBUF_INIT;

	જો (read_state_file(&sb, state, "next", 1) < 0)
		BUG("state file 'next' does not exist");
	state->cur = strtol(sb.buf, ખાલી, 10);

	જો (read_state_file(&sb, state, "last", 1) < 0)
		BUG("state file 'last' does not exist");
	state->last = strtol(sb.buf, ખાલી, 10);

	જો (read_am_author_script(state) < 0)
		die(_("could not parse author script"));

	read_commit_સંદેશ(state);

	જો (read_state_file(&sb, state, "original-commit", 1) < 0)
		oidclr(&state->orig_commit, the_repository->hash_algo);
	નહીંતર જો (get_oid_hex(sb.buf, &state->orig_commit) < 0)
		die(_("could not parse %s"), am_path(state, "original-commit"));

	read_state_file(&sb, state, "threeway", 1);
	state->threeway = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "quiet", 1);
	state->quiet = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "sign", 1);
	state->signoff = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "utf8", 1);
	state->utf8 = !strcmp(sb.buf, "t");

	જો (file_exists(am_path(state, "rerere-autoupdate"))) {
		read_state_file(&sb, state, "rerere-autoupdate", 1);
		state->allow_rerere_autoupdate = strcmp(sb.buf, "t") ?
			RERERE_NOAUTOUPDATE : RERERE_AUTOUPDATE;
	} નહીંતર {
		state->allow_rerere_autoupdate = 0;
	}

	read_state_file(&sb, state, "keep", 1);
	જો (!strcmp(sb.buf, "t"))
		state->keep = KEEP_TRUE;
	નહીંતર જો (!strcmp(sb.buf, "b"))
		state->keep = KEEP_NON_PATCH;
	નહીંતર
		state->keep = KEEP_FALSE;

	read_state_file(&sb, state, "messageid", 1);
	state->message_id = !strcmp(sb.buf, "t");

	read_state_file(&sb, state, "scissors", 1);
	જો (!strcmp(sb.buf, "t"))
		state->scissors = SCISSORS_TRUE;
	નહીંતર જો (!strcmp(sb.buf, "f"))
		state->scissors = SCISSORS_FALSE;
	નહીંતર
		state->scissors = SCISSORS_UNSET;

	read_state_file(&sb, state, "quoted-cr", 1);
	જો (!*sb.buf)
		state->quoted_cr = quoted_cr_unset;
	નહીંતર જો (mailinfo_parse_quoted_cr_action(sb.buf, &state->quoted_cr) != 0)
		die(_("could not parse %s"), am_path(state, "quoted-cr"));

	read_state_file(&sb, state, "apply-opt", 1);
	strvec_clear(&state->git_apply_opts);
	જો (sq_dequote_to_strvec(sb.buf, &state->git_apply_opts) < 0)
		die(_("could not parse %s"), am_path(state, "apply-opt"));

	state->rebasing = !!file_exists(am_path(state, "rebasing"));

	strbuf_release(&sb);
}

/**
 * Removes the am_state directory, forcefully terminating the current am
 * session.
 */
સ્થિર શૂન્ય am_destroy(અચલ સંરચના am_state *state)
{
	સંરચના strbuf sb = STRBUF_INIT;

	strbuf_addstr(&sb, state->dir);
	remove_dir_recursively(&sb, 0);
	strbuf_release(&sb);
}

/**
 * Runs applypatch-msg hook. Returns its exit code.
 */
સ્થિર પૂર્ણાંક run_applypatch_સંદેશ_hook(સંરચના am_state *state)
{
	પૂર્ણાંક ret = 0;

	પ્રતિજ્ઞા(state->સંદેશ);

	જો (!state->no_verify) {
		સંરચના run_hooks_opt opt = RUN_HOOKS_OPT_INIT_FORCE_SERIAL;
		strvec_ધકેલો(&opt.args, am_path(state, "final-commit"));
		ret = run_hooks_opt(the_repository, "applypatch-msg", &opt);
	}

	જો (!ret) {
		FREE_AND_NULL(state->સંદેશ);
		જો (read_commit_સંદેશ(state) < 0)
			die(_("'%s' was deleted by the applypatch-msg hook"),
				am_path(state, "final-commit"));
	}

	પાછા ret;
}

/**
 * Runs post-rewrite hook. Returns it exit code.
 */
સ્થિર પૂર્ણાંક run_post_rewrite_hook(અચલ સંરચના am_state *state)
{
	સંરચના run_hooks_opt opt = RUN_HOOKS_OPT_INIT;

	strvec_ધકેલો(&opt.args, "rebase");
	opt.path_to_stdin = am_path(state, "rewritten");

	પાછા run_hooks_opt(the_repository, "post-rewrite", &opt);
}

/**
 * Reads the state directory's "rewritten" file, and copies notes from the old
 * commits listed in the file to their rewritten commits.
 *
 * Returns 0 on success, -1 on failure.
 */
સ્થિર પૂર્ણાંક copy_notes_for_rebase(અચલ સંરચના am_state *state)
{
	સંરચના notes_rewrite_cfg *c;
	સંરચના strbuf sb = STRBUF_INIT;
	અચલ અક્ષર *invalid_લીટી = _("Malformed input line: '%s'.");
	અચલ અક્ષર *સંદેશ = "Notes added by 'git rebase'";
	FILE *fp;
	પૂર્ણાંક ret = 0;

	પ્રતિજ્ઞા(state->rebasing);

	c = init_copy_notes_for_rewrite("rebase");
	જો (!c)
		પાછા 0;

	fp = xfopen(am_path(state, "rewritten"), "r");

	જ્યારે (!strbuf_getline_lf(&sb, fp)) {
		સંરચના object_id from_obj, to_obj;
		અચલ અક્ષર *p;

		જો (sb.len != the_hash_algo->hexsz * 2 + 1) {
			ret = error(invalid_લીટી, sb.buf);
			જાઓ finish;
		}

		જો (parse_oid_hex(sb.buf, &from_obj, &p)) {
			ret = error(invalid_લીટી, sb.buf);
			જાઓ finish;
		}

		જો (*p != ' ') {
			ret = error(invalid_લીટી, sb.buf);
			જાઓ finish;
		}

		જો (get_oid_hex(p + 1, &to_obj)) {
			ret = error(invalid_લીટી, sb.buf);
			જાઓ finish;
		}

		જો (copy_note_for_rewrite(c, &from_obj, &to_obj))
			ret = error(_("Failed to copy notes from '%s' to '%s'"),
					oid_to_hex(&from_obj), oid_to_hex(&to_obj));
	}

finish:
	finish_copy_notes_for_rewrite(the_repository, c, સંદેશ);
	fclose(fp);
	strbuf_release(&sb);
	પાછા ret;
}

/**
 * Determines if the file looks like a piece of RFC2822 mail by grabbing all
 * non-indented lines and checking if they look like they begin with valid
 * header field names.
 *
 * Returns 1 if the file looks like a piece of mail, 0 otherwise.
 */
સ્થિર પૂર્ણાંક is_mail(FILE *fp)
{
	અચલ અક્ષર *header_regex = "^[!-9;-~]+:";
	સંરચના strbuf sb = STRBUF_INIT;
	regex_t regex;
	પૂર્ણાંક ret = 1;

	જો (fseek(fp, 0L, SEEK_SET))
		die_errno(_("fseek failed"));

	જો (regcomp(&regex, header_regex, REG_NOSUB | REG_EXTENDED))
		die("invalid pattern: %s", header_regex);

	જ્યારે (!strbuf_getline(&sb, fp)) {
		જો (!sb.len)
			તોડો; /* End of header */

		/* Ignore indented folded lines */
		જો (*sb.buf == '\t' || *sb.buf == ' ')
			ચાલુ;

		/* It's a header if it matches header_regex */
		જો (regexec(&regex, sb.buf, 0, ખાલી, 0)) {
			ret = 0;
			જાઓ done;
		}
	}

done:
	regfree(&regex);
	strbuf_release(&sb);
	પાછા ret;
}

/**
 * Attempts to detect the patch_format of the patches contained in `paths`,
 * returning the PATCH_FORMAT_* enum value. Returns PATCH_FORMAT_UNKNOWN if
 * detection fails.
 */
સ્થિર પૂર્ણાંક detect_patch_format(અચલ અક્ષર **paths)
{
	ગણના patch_format ret = PATCH_FORMAT_UNKNOWN;
	સંરચના strbuf l1 = STRBUF_INIT;
	સંરચના strbuf l2 = STRBUF_INIT;
	સંરચના strbuf l3 = STRBUF_INIT;
	FILE *fp;

	/*
	 * We default to mbox format if input is from stdin and for directories
	 */
	જો (!*paths || !strcmp(*paths, "-") || is_directory(*paths))
		પાછા PATCH_FORMAT_MBOX;

	/*
	 * Otherwise, check the first few lines of the first patch, starting
	 * from the first non-blank line, to try to detect its format.
	 */

	fp = xfopen(*paths, "r");

	જ્યારે (!strbuf_getline(&l1, fp)) {
		જો (l1.len)
			તોડો;
	}

	જો (starts_with(l1.buf, "From ") || starts_with(l1.buf, "From: ")) {
		ret = PATCH_FORMAT_MBOX;
		જાઓ done;
	}

	જો (starts_with(l1.buf, "# This series applies on GIT commit")) {
		ret = PATCH_FORMAT_STGIT_SERIES;
		જાઓ done;
	}

	જો (!strcmp(l1.buf, "# HG changeset patch")) {
		ret = PATCH_FORMAT_HG;
		જાઓ done;
	}

	strbuf_getline(&l2, fp);
	strbuf_getline(&l3, fp);

	/*
	 * If the second line is empty and the third is a From, Author or Date
	 * entry, this is likely an StGit patch.
	 */
	જો (l1.len && !l2.len &&
		(starts_with(l3.buf, "From:") ||
		 starts_with(l3.buf, "Author:") ||
		 starts_with(l3.buf, "Date:"))) {
		ret = PATCH_FORMAT_STGIT;
		જાઓ done;
	}

	જો (l1.len && is_mail(fp)) {
		ret = PATCH_FORMAT_MBOX;
		જાઓ done;
	}

done:
	fclose(fp);
	strbuf_release(&l1);
	strbuf_release(&l2);
	strbuf_release(&l3);
	પાછા ret;
}

/**
 * Splits out individual email patches from `paths`, where each path is either
 * a mbox file or a Maildir. Returns 0 on success, -1 on failure.
 */
સ્થિર પૂર્ણાંક split_mail_mbox(સંરચના am_state *state, અચલ અક્ષર **paths,
				પૂર્ણાંક keep_cr, પૂર્ણાંક mboxrd)
{
	સંરચના child_process cp = CHILD_PROCESS_INIT;
	સંરચના strbuf last = STRBUF_INIT;
	પૂર્ણાંક ret;

	cp.git_cmd = 1;
	strvec_ધકેલો(&cp.args, "mailsplit");
	strvec_pushf(&cp.args, "-d%d", state->prec);
	strvec_pushf(&cp.args, "-o%s", state->dir);
	strvec_ધકેલો(&cp.args, "-b");
	જો (keep_cr)
		strvec_ધકેલો(&cp.args, "--keep-cr");
	જો (mboxrd)
		strvec_ધકેલો(&cp.args, "--mboxrd");
	strvec_ધકેલો(&cp.args, "--");
	strvec_pushv(&cp.args, paths);

	ret = capture_command(&cp, &last, 8);
	જો (ret)
		જાઓ exit;

	state->cur = 1;
	state->last = strtol(last.buf, ખાલી, 10);

exit:
	strbuf_release(&last);
	પાછા ret ? -1 : 0;
}

/**
 * Callback signature for split_mail_conv(). The foreign patch should be
 * read from `in`, and the converted patch (in RFC2822 mail format) should be
 * written to `out`. Return 0 on success, or -1 on failure.
 */
પ્રકારવ્યાખ્યા પૂર્ણાંક (*mail_conv_fn)(FILE *out, FILE *in, પૂર્ણાંક keep_cr);

/**
 * Calls `fn` for each file in `paths` to convert the foreign patch to the
 * RFC2822 mail format suitable for parsing with git-mailinfo.
 *
 * Returns 0 on success, -1 on failure.
 */
સ્થિર પૂર્ણાંક split_mail_conv(mail_conv_fn fn, સંરચના am_state *state,
			અચલ અક્ષર **paths, પૂર્ણાંક keep_cr)
{
	સ્થિર અચલ અક્ષર *stdin_only[] = {"-", ખાલી};
	પૂર્ણાંક i;

	જો (!*paths)
		paths = stdin_only;

	માટે (i = 0; *paths; paths++, i++) {
		FILE *in, *out;
		અચલ અક્ષર *mail;
		પૂર્ણાંક ret;

		જો (!strcmp(*paths, "-"))
			in = stdin;
		નહીંતર
			in = fopen(*paths, "r");

		જો (!in)
			પાછા error_errno(_("could not open '%s' for reading"),
					   *paths);

		mail = mkpath("%s/%0*d", state->dir, state->prec, i + 1);

		out = fopen(mail, "w");
		જો (!out) {
			જો (in != stdin)
				fclose(in);
			પાછા error_errno(_("could not open '%s' for writing"),
					   mail);
		}

		ret = fn(out, in, keep_cr);

		fclose(out);
		જો (in != stdin)
			fclose(in);

		જો (ret)
			પાછા error(_("could not parse patch '%s'"), *paths);
	}

	state->cur = 1;
	state->last = i;
	પાછા 0;
}

/**
 * A split_mail_conv() callback that converts an StGit patch to an RFC2822
 * message suitable for parsing with git-mailinfo.
 */
સ્થિર પૂર્ણાંક stgit_patch_to_mail(FILE *out, FILE *in, પૂર્ણાંક keep_cr UNUSED)
{
	સંરચના strbuf sb = STRBUF_INIT;
	પૂર્ણાંક subject_printed = 0;

	જ્યારે (!strbuf_getline_lf(&sb, in)) {
		અચલ અક્ષર *str;

		જો (str_isspace(sb.buf))
			ચાલુ;
		નહીંતર જો (skip_prefix(sb.buf, "Author:", &str))
			fprintf(out, "From:%s\n", str);
		નહીંતર જો (starts_with(sb.buf, "From") || starts_with(sb.buf, "Date"))
			fprintf(out, "%s\n", sb.buf);
		નહીંતર જો (!subject_printed) {
			fprintf(out, "Subject: %s\n", sb.buf);
			subject_printed = 1;
		} નહીંતર {
			fprintf(out, "\n%s\n", sb.buf);
			તોડો;
		}
	}

	strbuf_reset(&sb);
	જ્યારે (strbuf_fread(&sb, 8192, in) > 0) {
		fwrite(sb.buf, 1, sb.len, out);
		strbuf_reset(&sb);
	}

	strbuf_release(&sb);
	પાછા 0;
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
સ્થિર પૂર્ણાંક split_mail_stgit_series(સંરચના am_state *state, અચલ અક્ષર **paths,
					પૂર્ણાંક keep_cr)
{
	અચલ અક્ષર *series_dir;
	અક્ષર *series_dir_buf;
	FILE *fp;
	સંરચના strvec patches = STRVEC_INIT;
	સંરચના strbuf sb = STRBUF_INIT;
	પૂર્ણાંક ret;

	જો (!paths[0] || paths[1])
		પાછા error(_("Only one StGIT patch series can be applied at once"));

	series_dir_buf = xstrdup(*paths);
	series_dir = dirname(series_dir_buf);

	fp = fopen(*paths, "r");
	જો (!fp) {
		મુક્ત(series_dir_buf);
		પાછા error_errno(_("could not open '%s' for reading"), *paths);
	}

	જ્યારે (!strbuf_getline_lf(&sb, fp)) {
		જો (*sb.buf == '#')
			ચાલુ; /* skip comment lines */

		strvec_ધકેલો(&patches, mkpath("%s/%s", series_dir, sb.buf));
	}

	fclose(fp);
	strbuf_release(&sb);
	મુક્ત(series_dir_buf);

	ret = split_mail_conv(stgit_patch_to_mail, state, patches.v, keep_cr);

	strvec_clear(&patches);
	પાછા ret;
}

/**
 * A split_patches_conv() callback that converts a mercurial patch to a RFC2822
 * message suitable for parsing with git-mailinfo.
 */
સ્થિર પૂર્ણાંક hg_patch_to_mail(FILE *out, FILE *in, પૂર્ણાંક keep_cr UNUSED)
{
	સંરચના strbuf sb = STRBUF_INIT;
	પૂર્ણાંક rc = 0;

	જ્યારે (!strbuf_getline_lf(&sb, in)) {
		અચલ અક્ષર *str;

		જો (skip_prefix(sb.buf, "# User ", &str))
			fprintf(out, "From: %s\n", str);
		નહીંતર જો (skip_prefix(sb.buf, "# Date ", &str)) {
			timestamp_t timestamp;
			long tz, tz2;
			અક્ષર *end;

			errno = 0;
			timestamp = parse_timestamp(str, &end, 10);
			જો (errno) {
				rc = error(_("invalid timestamp"));
				જાઓ exit;
			}

			જો (!skip_prefix(end, " ", &str)) {
				rc = error(_("invalid Date line"));
				જાઓ exit;
			}

			errno = 0;
			tz = strtol(str, &end, 10);
			જો (errno) {
				rc = error(_("invalid timezone offset"));
				જાઓ exit;
			}

			જો (*end) {
				rc = error(_("invalid Date line"));
				જાઓ exit;
			}

			/*
			 * mercurial's timezone is in seconds west of UTC,
			 * however git's timezone is in hours + minutes east of
			 * UTC. Convert it.
			 */
			tz2 = labs(tz) / 3600 * 100 + labs(tz) % 3600 / 60;
			જો (tz > 0)
				tz2 = -tz2;

			fprintf(out, "Date: %s\n", show_date(timestamp, tz2, DATE_MODE(RFC2822)));
		} નહીંતર જો (starts_with(sb.buf, "# ")) {
			ચાલુ;
		} નહીંતર {
			fprintf(out, "\n%s\n", sb.buf);
			તોડો;
		}
	}

	strbuf_reset(&sb);
	જ્યારે (strbuf_fread(&sb, 8192, in) > 0) {
		fwrite(sb.buf, 1, sb.len, out);
		strbuf_reset(&sb);
	}
exit:
	strbuf_release(&sb);
	પાછા rc;
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
સ્થિર પૂર્ણાંક split_mail(સંરચના am_state *state, ગણના patch_format patch_format,
			અચલ અક્ષર **paths, પૂર્ણાંક keep_cr)
{
	જો (keep_cr < 0) {
		keep_cr = 0;
		repo_config_get_bool(the_repository, "am.keepcr", &keep_cr);
	}

	પસંદગી (patch_format) {
	સ્થિતિ PATCH_FORMAT_MBOX:
		પાછા split_mail_mbox(state, paths, keep_cr, 0);
	સ્થિતિ PATCH_FORMAT_STGIT:
		પાછા split_mail_conv(stgit_patch_to_mail, state, paths, keep_cr);
	સ્થિતિ PATCH_FORMAT_STGIT_SERIES:
		પાછા split_mail_stgit_series(state, paths, keep_cr);
	સ્થિતિ PATCH_FORMAT_HG:
		પાછા split_mail_conv(hg_patch_to_mail, state, paths, keep_cr);
	સ્થિતિ PATCH_FORMAT_MBOXRD:
		પાછા split_mail_mbox(state, paths, keep_cr, 1);
	મૂળભૂત:
		BUG("invalid patch_format");
	}
	પાછા -1;
}

/**
 * Setup a new am session for applying patches
 */
સ્થિર શૂન્ય am_setup(સંરચના am_state *state, ગણના patch_format patch_format,
			અચલ અક્ષર **paths, પૂર્ણાંક keep_cr)
{
	સંરચના object_id curr_head;
	અચલ અક્ષર *str;
	સંરચના strbuf sb = STRBUF_INIT;

	જો (!patch_format)
		patch_format = detect_patch_format(paths);

	જો (!patch_format) {
		fprintf_ln(stderr, _("Patch format detection failed."));
		die(ખાલી);
	}

	જો (mkdir(state->dir, 0777) < 0 && errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), state->dir);
	refs_delete_ref(get_main_ref_store(the_repository), ખાલી,
			"REBASE_HEAD", ખાલી, REF_NO_DEREF);

	જો (split_mail(state, patch_format, paths, keep_cr) < 0) {
		am_destroy(state);
		die(_("Failed to split patches."));
	}

	જો (state->rebasing)
		state->threeway = 1;

	write_state_bool(state, "threeway", state->threeway);
	write_state_bool(state, "quiet", state->quiet);
	write_state_bool(state, "sign", state->signoff);
	write_state_bool(state, "utf8", state->utf8);

	જો (state->allow_rerere_autoupdate)
		write_state_bool(state, "rerere-autoupdate",
			 state->allow_rerere_autoupdate == RERERE_AUTOUPDATE);

	પસંદગી (state->keep) {
	સ્થિતિ KEEP_FALSE:
		str = "f";
		તોડો;
	સ્થિતિ KEEP_TRUE:
		str = "t";
		તોડો;
	સ્થિતિ KEEP_NON_PATCH:
		str = "b";
		તોડો;
	મૂળભૂત:
		BUG("invalid value for state->keep");
	}

	write_state_text(state, "keep", str);
	write_state_bool(state, "messageid", state->message_id);

	પસંદગી (state->scissors) {
	સ્થિતિ SCISSORS_UNSET:
		str = "";
		તોડો;
	સ્થિતિ SCISSORS_FALSE:
		str = "f";
		તોડો;
	સ્થિતિ SCISSORS_TRUE:
		str = "t";
		તોડો;
	મૂળભૂત:
		BUG("invalid value for state->scissors");
	}
	write_state_text(state, "scissors", str);

	પસંદગી (state->quoted_cr) {
	સ્થિતિ quoted_cr_unset:
		str = "";
		તોડો;
	સ્થિતિ quoted_cr_nowarn:
		str = "nowarn";
		તોડો;
	સ્થિતિ quoted_cr_warn:
		str = "warn";
		તોડો;
	સ્થિતિ quoted_cr_strip:
		str = "strip";
		તોડો;
	મૂળભૂત:
		BUG("invalid value for state->quoted_cr");
	}
	write_state_text(state, "quoted-cr", str);

	sq_quote_argv(&sb, state->git_apply_opts.v);
	write_state_text(state, "apply-opt", sb.buf);

	જો (state->rebasing)
		write_state_text(state, "rebasing", "");
	નહીંતર
		write_state_text(state, "applying", "");

	જો (!repo_get_oid(the_repository, "HEAD", &curr_head)) {
		write_state_text(state, "abort-safety", oid_to_hex(&curr_head));
		જો (!state->rebasing)
			refs_update_ref(get_main_ref_store(the_repository),
					"am", "ORIG_HEAD", &curr_head, ખાલી,
					0,
					UPDATE_REFS_DIE_ON_ERR);
	} નહીંતર {
		write_state_text(state, "abort-safety", "");
		જો (!state->rebasing)
			refs_delete_ref(get_main_ref_store(the_repository),
					ખાલી, "ORIG_HEAD", ખાલી, 0);
	}

	/*
	 * NOTE: Since the "next" and "last" files determine if an am_state
	 * session is in progress, they should be written last.
	 */

	write_state_ગણતરી(state, "next", state->cur);
	write_state_ગણતરી(state, "last", state->last);

	strbuf_release(&sb);
}

/**
 * Increments the patch pointer, and cleans am_state for the application of the
 * next patch.
 */
સ્થિર શૂન્ય am_next(સંરચના am_state *state)
{
	સંરચના object_id head;

	FREE_AND_NULL(state->author_name);
	FREE_AND_NULL(state->author_email);
	FREE_AND_NULL(state->author_date);
	FREE_AND_NULL(state->સંદેશ);
	state->સંદેશ_len = 0;

	unlink(am_path(state, "author-script"));
	unlink(am_path(state, "final-commit"));

	oidclr(&state->orig_commit, the_repository->hash_algo);
	unlink(am_path(state, "original-commit"));
	refs_delete_ref(get_main_ref_store(the_repository), ખાલી,
			"REBASE_HEAD", ખાલી, REF_NO_DEREF);

	જો (!repo_get_oid(the_repository, "HEAD", &head))
		write_state_text(state, "abort-safety", oid_to_hex(&head));
	નહીંતર
		write_state_text(state, "abort-safety", "");

	state->cur++;
	write_state_ગણતરી(state, "next", state->cur);
}

/**
 * Returns the filename of the current patch email.
 */
સ્થિર અચલ અક્ષર *msgnum(અચલ સંરચના am_state *state)
{
	સ્થિર સંરચના strbuf sb = STRBUF_INIT;

	strbuf_reset(&sb);
	strbuf_addf(&sb, "%0*d", state->prec, state->cur);

	પાછા sb.buf;
}

/**
 * Dies with a user-friendly message on how to proceed after resolving the
 * problem. This message can be overridden with state->resolvemsg.
 */
સ્થિર શૂન્ય NORETURN die_user_resolve(અચલ સંરચના am_state *state)
{
	જો (state->resolvemsg) {
		advise_if_enabled(ADVICE_MERGE_CONFLICT, "%s", state->resolvemsg);
	} નહીંતર {
		અચલ અક્ષર *cmdline = state->interactive ? "git am -i" : "git am";
		સંરચના strbuf sb = STRBUF_INIT;

		strbuf_addf(&sb, _("When you have resolved this problem, run \"%s --continue\".\n"), cmdline);
		strbuf_addf(&sb, _("If you prefer to skip this patch, run \"%s --skip\" instead.\n"), cmdline);

		જો (advice_enabled(ADVICE_AM_WORK_DIR) &&
		    is_empty_or_missing_file(am_path(state, "patch")) &&
		    !repo_index_has_changes(the_repository, ખાલી, ખાલી))
			strbuf_addf(&sb, _("To record the empty patch as an empty commit, run \"%s --allow-empty\".\n"), cmdline);

		strbuf_addf(&sb, _("To restore the original branch and stop patching, run \"%s --abort\"."), cmdline);

		advise_if_enabled(ADVICE_MERGE_CONFLICT, "%s", sb.buf);
		strbuf_release(&sb);
	}

	die(ખાલી);
}

/**
 * Appends signoff to the "msg" field of the am_state.
 */
સ્થિર શૂન્ય am_append_signoff(સંરચના am_state *state)
{
	સંરચના strbuf sb = STRBUF_INIT;

	strbuf_attach(&sb, state->સંદેશ, state->સંદેશ_len, state->સંદેશ_len + 1);
	append_signoff(&sb, 0, 0);
	state->સંદેશ = strbuf_detach(&sb, &state->સંદેશ_len);
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
સ્થિર પૂર્ણાંક parse_mail(સંરચના am_state *state, અચલ અક્ષર *mail)
{
	FILE *fp;
	સંરચના strbuf sb = STRBUF_INIT;
	સંરચના strbuf સંદેશ = STRBUF_INIT;
	સંરચના strbuf author_name = STRBUF_INIT;
	સંરચના strbuf author_date = STRBUF_INIT;
	સંરચના strbuf author_email = STRBUF_INIT;
	પૂર્ણાંક ret = 0;
	સંરચના mailinfo mi;

	setup_mailinfo(the_repository, &mi);

	જો (state->utf8)
		mi.metainfo_charset = get_commit_output_encoding();
	નહીંતર
		mi.metainfo_charset = ખાલી;

	પસંદગી (state->keep) {
	સ્થિતિ KEEP_FALSE:
		તોડો;
	સ્થિતિ KEEP_TRUE:
		mi.keep_subject = 1;
		તોડો;
	સ્થિતિ KEEP_NON_PATCH:
		mi.keep_non_patch_brackets_in_subject = 1;
		તોડો;
	મૂળભૂત:
		BUG("invalid value for state->keep");
	}

	જો (state->message_id)
		mi.add_message_id = 1;

	પસંદગી (state->scissors) {
	સ્થિતિ SCISSORS_UNSET:
		તોડો;
	સ્થિતિ SCISSORS_FALSE:
		mi.use_scissors = 0;
		તોડો;
	સ્થિતિ SCISSORS_TRUE:
		mi.use_scissors = 1;
		તોડો;
	મૂળભૂત:
		BUG("invalid value for state->scissors");
	}

	પસંદગી (state->quoted_cr) {
	સ્થિતિ quoted_cr_unset:
		તોડો;
	સ્થિતિ quoted_cr_nowarn:
	સ્થિતિ quoted_cr_warn:
	સ્થિતિ quoted_cr_strip:
		mi.quoted_cr = state->quoted_cr;
		તોડો;
	મૂળભૂત:
		BUG("invalid value for state->quoted_cr");
	}

	mi.આગત = xfopen(mail, "r");
	mi.output = xfopen(am_path(state, "info"), "w");
	જો (mailinfo(&mi, am_path(state, "msg"), am_path(state, "patch")))
		die("could not parse patch");

	fclose(mi.આગત);
	fclose(mi.output);

	જો (mi.format_flowed)
		warning(_("Patch sent with format=flowed; "
			  "space at the end of lines might be lost."));

	/* Extract message and author information */
	fp = xfopen(am_path(state, "info"), "r");
	જ્યારે (!strbuf_getline_lf(&sb, fp)) {
		અચલ અક્ષર *x;

		જો (skip_prefix(sb.buf, "Subject: ", &x)) {
			જો (સંદેશ.len)
				strbuf_addch(&સંદેશ, '\n');
			strbuf_addstr(&સંદેશ, x);
		} નહીંતર જો (skip_prefix(sb.buf, "Author: ", &x))
			strbuf_addstr(&author_name, x);
		નહીંતર જો (skip_prefix(sb.buf, "Email: ", &x))
			strbuf_addstr(&author_email, x);
		નહીંતર જો (skip_prefix(sb.buf, "Date: ", &x))
			strbuf_addstr(&author_date, x);
	}
	fclose(fp);

	/* Skip pine's internal folder data */
	જો (!strcmp(author_name.buf, "Mail System Internal Data")) {
		ret = 1;
		જાઓ finish;
	}

	strbuf_addstr(&સંદેશ, "\n\n");
	strbuf_addbuf(&સંદેશ, &mi.log_message);
	strbuf_stripspace(&સંદેશ, ખાલી);

	પ્રતિજ્ઞા(!state->author_name);
	state->author_name = strbuf_detach(&author_name, ખાલી);

	પ્રતિજ્ઞા(!state->author_email);
	state->author_email = strbuf_detach(&author_email, ખાલી);

	પ્રતિજ્ઞા(!state->author_date);
	state->author_date = strbuf_detach(&author_date, ખાલી);

	પ્રતિજ્ઞા(!state->સંદેશ);
	state->સંદેશ = strbuf_detach(&સંદેશ, &state->સંદેશ_len);

finish:
	strbuf_release(&સંદેશ);
	strbuf_release(&author_date);
	strbuf_release(&author_email);
	strbuf_release(&author_name);
	strbuf_release(&sb);
	clear_mailinfo(&mi);
	પાછા ret;
}

/**
 * Sets commit_id to the commit hash where the mail was generated from.
 * Returns 0 on success, -1 on failure.
 */
સ્થિર પૂર્ણાંક get_mail_commit_oid(સંરચના object_id *commit_id, અચલ અક્ષર *mail)
{
	સંરચના strbuf sb = STRBUF_INIT;
	FILE *fp = xfopen(mail, "r");
	અચલ અક્ષર *x;
	પૂર્ણાંક ret = 0;

	જો (strbuf_getline_lf(&sb, fp) ||
	    !skip_prefix(sb.buf, "From ", &x) ||
	    get_oid_hex(x, commit_id) < 0)
		ret = -1;

	strbuf_release(&sb);
	fclose(fp);
	પાછા ret;
}

/**
 * Sets state->msg, state->author_name, state->author_email, state->author_date
 * to the commit's respective info.
 */
સ્થિર શૂન્ય get_commit_info(સંરચના am_state *state, સંરચના commit *commit)
{
	અચલ અક્ષર *buffer, *ident_લીટી, *સંદેશ;
	size_t ident_len;
	સંરચના ident_split id;

	buffer = repo_logmsg_reencode(the_repository, commit, ખાલી,
				      get_commit_output_encoding());

	ident_લીટી = find_commit_header(buffer, "author", &ident_len);
	જો (!ident_લીટી)
		die(_("missing author line in commit %s"),
		      oid_to_hex(&commit->object.oid));
	જો (split_ident_લીટી(&id, ident_લીટી, ident_len) < 0)
		die(_("invalid ident line: %.*s"), (પૂર્ણાંક)ident_len, ident_લીટી);

	પ્રતિજ્ઞા(!state->author_name);
	જો (id.name_begin)
		state->author_name =
			xmemdupz(id.name_begin, id.name_end - id.name_begin);
	નહીંતર
		state->author_name = xstrdup("");

	પ્રતિજ્ઞા(!state->author_email);
	જો (id.mail_begin)
		state->author_email =
			xmemdupz(id.mail_begin, id.mail_end - id.mail_begin);
	નહીંતર
		state->author_email = xstrdup("");

	પ્રતિજ્ઞા(!state->author_date);
	state->author_date = xstrdup(show_ident_date(&id, DATE_MODE(NORMAL)));

	પ્રતિજ્ઞા(!state->સંદેશ);
	સંદેશ = strstr(buffer, "\n\n");
	જો (!સંદેશ)
		die(_("unable to parse commit %s"), oid_to_hex(&commit->object.oid));
	state->સંદેશ = xstrdup(સંદેશ + 2);
	state->સંદેશ_len = strlen(state->સંદેશ);
	repo_unuse_commit_buffer(the_repository, commit, buffer);
}

/**
 * Writes `commit` as a patch to the state directory's "patch" file.
 */
સ્થિર શૂન્ય write_commit_patch(અચલ સંરચના am_state *state, સંરચના commit *commit)
{
	સંરચના rev_info rev_info;
	FILE *fp;

	fp = xfopen(am_path(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, ખાલી);
	rev_info.diff = 1;
	rev_info.abbrev = 0;
	rev_info.disable_stdin = 1;
	rev_info.show_મૂળ_diff = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.flags.દ્વિઅંકી = 1;
	rev_info.diffopt.flags.full_index = 1;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	add_pending_object(&rev_info, &commit->object, "");
	diff_setup_done(&rev_info.diffopt);
	log_વૃક્ષ_commit(&rev_info, commit);
	release_revisions(&rev_info);
}

/**
 * Writes the diff of the index against HEAD as a patch to the state
 * directory's "patch" file.
 */
સ્થિર શૂન્ય write_index_patch(અચલ સંરચના am_state *state)
{
	સંરચના વૃક્ષ *વૃક્ષ;
	સંરચના object_id head;
	સંરચના rev_info rev_info;
	FILE *fp;

	જો (!repo_get_oid(the_repository, "HEAD", &head)) {
		સંરચના commit *commit = lookup_commit_or_die(&head, "HEAD");
		વૃક્ષ = repo_get_commit_વૃક્ષ(the_repository, commit);
	} નહીંતર
		વૃક્ષ = lookup_વૃક્ષ(the_repository,
				   the_repository->hash_algo->empty_વૃક્ષ);

	fp = xfopen(am_path(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, ખાલી);
	rev_info.diff = 1;
	rev_info.disable_stdin = 1;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	add_pending_object(&rev_info, &વૃક્ષ->object, "");
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
સ્થિર પૂર્ણાંક parse_mail_rebase(સંરચના am_state *state, અચલ અક્ષર *mail)
{
	સંરચના commit *commit;
	સંરચના object_id commit_oid;

	જો (get_mail_commit_oid(&commit_oid, mail) < 0)
		die(_("could not parse %s"), mail);

	commit = lookup_commit_or_die(&commit_oid, mail);

	get_commit_info(state, commit);

	write_commit_patch(state, commit);

	oidcpy(&state->orig_commit, &commit_oid);
	write_state_text(state, "original-commit", oid_to_hex(&commit_oid));
	refs_update_ref(get_main_ref_store(the_repository), "am",
			"REBASE_HEAD", &commit_oid,
			ખાલી, REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);

	પાછા 0;
}

/**
 * Applies current patch with git-apply. Returns 0 on success, -1 otherwise. If
 * `index_file` is not NULL, the patch will be applied to that index.
 */
સ્થિર પૂર્ણાંક run_apply(અચલ સંરચના am_state *state, અચલ અક્ષર *index_file)
{
	સંરચના strvec apply_paths = STRVEC_INIT;
	સંરચના strvec apply_opts = STRVEC_INIT;
	સંરચના apply_state apply_state;
	પૂર્ણાંક res, opts_ડાબું;
	પૂર્ણાંક force_apply = 0;
	પૂર્ણાંક options = 0;
	અચલ અક્ષર **apply_argv;

	જો (init_apply_state(&apply_state, the_repository, ખાલી))
		BUG("init_apply_state() failed");

	strvec_ધકેલો(&apply_opts, "apply");
	strvec_pushv(&apply_opts, state->git_apply_opts.v);

	/*
	 * Build a copy that apply_parse_options() can rearrange.
	 * apply_opts.v keeps referencing the allocated strings for
	 * strvec_clear() to release.
	 */
	DUP_ARRAY(apply_argv, apply_opts.v, apply_opts.nr);

	opts_ડાબું = apply_parse_options(apply_opts.nr, apply_argv,
					&apply_state, &force_apply, &options,
					ખાલી);

	જો (opts_ડાબું != 0)
		die("unknown option passed through to git apply");

	જો (index_file) {
		apply_state.index_file = index_file;
		apply_state.cached = 1;
	} નહીંતર
		apply_state.check_index = 1;

	/*
	 * If we are allowed to fall back on 3-way merge, don't give false
	 * errors during the initial attempt.
	 */
	જો (state->threeway && !index_file)
		apply_state.apply_verbosity = verbosity_silent;

	જો (check_apply_state(&apply_state, force_apply))
		BUG("check_apply_state() failed");

	strvec_ધકેલો(&apply_paths, am_path(state, "patch"));

	res = apply_all_patches(&apply_state, apply_paths.nr, apply_paths.v, options);

	strvec_clear(&apply_paths);
	strvec_clear(&apply_opts);
	clear_apply_state(&apply_state);
	મુક્ત(apply_argv);

	જો (res)
		પાછા res;

	જો (index_file) {
		/* Reload index as apply_all_patches() will have modified it. */
		discard_index(the_repository->index);
		read_index_from(the_repository->index, index_file,
				repo_get_git_dir(the_repository));
	}

	પાછા 0;
}

/**
 * Builds an index that contains just the blobs needed for a 3way merge.
 */
સ્થિર પૂર્ણાંક build_fake_ancestor(અચલ સંરચના am_state *state, અચલ અક્ષર *index_file)
{
	સંરચના child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	strvec_ધકેલો(&cp.args, "apply");
	strvec_pushv(&cp.args, state->git_apply_opts.v);
	strvec_pushf(&cp.args, "--build-fake-ancestor=%s", index_file);
	strvec_ધકેલો(&cp.args, am_path(state, "patch"));

	જો (run_command(&cp))
		પાછા -1;

	પાછા 0;
}

/**
 * Attempt a threeway merge, using index_path as the temporary index.
 */
સ્થિર પૂર્ણાંક fall_back_threeway(અચલ સંરચના am_state *state, અચલ અક્ષર *index_path)
{
	સંરચના object_id their_વૃક્ષ, our_વૃક્ષ;
	સંરચના object_id bases[1] = { 0 };
	સંરચના merge_options o;
	સંરચના commit *result;
	અક્ષર *their_વૃક્ષ_name;

	જો (repo_get_oid(the_repository, "HEAD", &our_વૃક્ષ) < 0)
		oidcpy(&our_વૃક્ષ, the_hash_algo->empty_વૃક્ષ);

	જો (build_fake_ancestor(state, index_path))
		પાછા error("could not build fake ancestor");

	discard_index(the_repository->index);
	read_index_from(the_repository->index, index_path, repo_get_git_dir(the_repository));

	જો (write_index_as_વૃક્ષ(&bases[0], the_repository->index, index_path, 0, ખાલી))
		પાછા error(_("Repository lacks necessary blobs to fall back on 3-way merge."));

	say(state, stdout, _("Using index info to reconstruct a base tree..."));

	જો (!state->quiet) {
		/*
		 * List paths that needed 3-way fallback, so that the user can
		 * review them with extra care to spot mismerges.
		 */
		સંરચના rev_info rev_info;

		repo_init_revisions(the_repository, &rev_info, ખાલી);
		rev_info.diffopt.output_format = DIFF_FORMAT_NAME_STATUS;
		rev_info.diffopt.filter |= diff_filter_bit('A');
		rev_info.diffopt.filter |= diff_filter_bit('M');
		add_pending_oid(&rev_info, "HEAD", &our_વૃક્ષ, 0);
		diff_setup_done(&rev_info.diffopt);
		run_diff_index(&rev_info, DIFF_INDEX_CACHED);
		release_revisions(&rev_info);
	}

	જો (run_apply(state, index_path))
		પાછા error(_("Did you hand edit your patch?\n"
				"It does not apply to blobs recorded in its index."));

	જો (write_index_as_વૃક્ષ(&their_વૃક્ષ, the_repository->index, index_path, 0, ખાલી))
		પાછા error("could not write tree");

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
	their_વૃક્ષ_name = xstrfmt("%.*s", linelen(state->સંદેશ), state->સંદેશ);
	o.branch2 = their_વૃક્ષ_name;
	o.ancestor = "constructed fake ancestor";
	o.detect_directory_renames = MERGE_DIRECTORY_RENAMES_NONE;

	જો (state->quiet)
		o.verbosity = 0;

	જો (merge_ort_generic(&o, &our_વૃક્ષ, &their_વૃક્ષ, 1, bases, &result)) {
		repo_rerere(the_repository, state->allow_rerere_autoupdate);
		મુક્ત(their_વૃક્ષ_name);
		પાછા error(_("Failed to merge in the changes."));
	}

	મુક્ત(their_વૃક્ષ_name);
	પાછા 0;
}

/**
 * Commits the current index with state->msg as the commit message and
 * state->author_name, state->author_email and state->author_date as the author
 * information.
 */
સ્થિર શૂન્ય do_commit(અચલ સંરચના am_state *state)
{
	સંરચના object_id વૃક્ષ, parent, commit;
	અચલ સંરચના object_id *old_oid;
	સંરચના commit_list *parents = ખાલી;
	અચલ અક્ષર *reflog_સંદેશ, *author, *committer = ખાલી;
	સંરચના strbuf sb = STRBUF_INIT;

	જો (!state->no_verify && run_hooks(the_repository, "pre-applypatch"))
		exit(1);

	જો (write_index_as_વૃક્ષ(&વૃક્ષ, the_repository->index,
				repo_get_index_file(the_repository),
				0, ખાલી))
		die(_("git write-tree failed to write a tree"));

	જો (!repo_get_oid_commit(the_repository, "HEAD", &parent)) {
		old_oid = &parent;
		commit_list_ઉમેરો(lookup_commit(the_repository, &parent),
				   &parents);
	} નહીંતર {
		old_oid = ખાલી;
		say(state, stderr, _("applying to an empty history"));
	}

	author = fmt_ident(state->author_name, state->author_email,
		WANT_AUTHOR_IDENT,
			state->ignore_date ? ખાલી : state->author_date,
			IDENT_STRICT);

	જો (state->committer_date_is_author_date)
		committer = fmt_ident(getenv("GIT_COMMITTER_NAME"),
				      getenv("GIT_COMMITTER_EMAIL"),
				      WANT_COMMITTER_IDENT,
				      state->ignore_date ? ખાલી
							 : state->author_date,
				      IDENT_STRICT);

	જો (commit_વૃક્ષ_extended(state->સંદેશ, state->સંદેશ_len, &વૃક્ષ, parents,
				 &commit, author, committer, state->sign_commit,
				 ખાલી))
		die(_("failed to write commit object"));

	reflog_સંદેશ = getenv("GIT_REFLOG_ACTION");
	જો (!reflog_સંદેશ)
		reflog_સંદેશ = "am";

	strbuf_addf(&sb, "%s: %.*s", reflog_સંદેશ, linelen(state->સંદેશ),
			state->સંદેશ);

	refs_update_ref(get_main_ref_store(the_repository), sb.buf, "HEAD",
			&commit, old_oid, 0,
			UPDATE_REFS_DIE_ON_ERR);

	જો (state->rebasing) {
		FILE *fp = xfopen(am_path(state, "rewritten"), "a");

		પ્રતિજ્ઞા(!is_null_oid(&state->orig_commit));
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
સ્થિર શૂન્ય validate_resume_state(અચલ સંરચના am_state *state)
{
	જો (!state->સંદેશ)
		die(_("cannot resume: %s does not exist."),
			am_path(state, "final-commit"));

	જો (!state->author_name || !state->author_email || !state->author_date)
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
સ્થિર પૂર્ણાંક do_interactive(સંરચના am_state *state)
{
	પ્રતિજ્ઞા(state->સંદેશ);

	માટે (;;) {
		અક્ષર reply[64];

		puts(_("Commit Body is:"));
		puts("--------------------------");
		printf("%s", state->સંદેશ);
		puts("--------------------------");

		/*
		 * TRANSLATORS: Make sure to include [y], [n], [e], [v] and [a]
		 * in your translation. The program will only accept English
		 * input at this point.
		 */
		printf(_("Apply? [y]es/[n]o/[e]dit/[v]iew patch/[a]ccept all: "));
		જો (!fgets(reply, કદ(reply), stdin))
			die("unable to read from stdin; aborting");

		જો (*reply == 'y' || *reply == 'Y') {
			પાછા 0;
		} નહીંતર જો (*reply == 'a' || *reply == 'A') {
			state->interactive = 0;
			પાછા 0;
		} નહીંતર જો (*reply == 'n' || *reply == 'N') {
			પાછા 1;
		} નહીંતર જો (*reply == 'e' || *reply == 'E') {
			સંરચના strbuf સંદેશ = STRBUF_INIT;

			જો (!launch_editor(am_path(state, "final-commit"), &સંદેશ, ખાલી)) {
				મુક્ત(state->સંદેશ);
				state->સંદેશ = strbuf_detach(&સંદેશ, &state->સંદેશ_len);
			}
			strbuf_release(&સંદેશ);
		} નહીંતર જો (*reply == 'v' || *reply == 'V') {
			અચલ અક્ષર *pager = git_pager(the_repository, 1);
			સંરચના child_process cp = CHILD_PROCESS_INIT;

			જો (!pager)
				pager = "cat";
			prepare_pager_args(&cp, pager);
			strvec_ધકેલો(&cp.args, am_path(state, "patch"));
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
સ્થિર શૂન્ય am_run(સંરચના am_state *state, પૂર્ણાંક resume)
{
	સંરચના strbuf sb = STRBUF_INIT;

	unlink(am_path(state, "dirtyindex"));

	જો (repo_refresh_and_write_index(the_repository, REFRESH_QUIET, 0, 0,
					 ખાલી, ખાલી, ખાલી) < 0)
		die(_("unable to write index file"));

	જો (repo_index_has_changes(the_repository, ખાલી, &sb)) {
		write_state_bool(state, "dirtyindex", 1);
		die(_("Dirty index: cannot apply patches (dirty: %s)"), sb.buf);
	}

	strbuf_release(&sb);

	જ્યારે (state->cur <= state->last) {
		અચલ અક્ષર *mail = am_path(state, msgnum(state));
		પૂર્ણાંક apply_status;
		પૂર્ણાંક to_keep;

		reset_ident_date();

		જો (!file_exists(mail))
			જાઓ next;

		જો (resume) {
			validate_resume_state(state);
		} નહીંતર {
			પૂર્ણાંક skip;

			જો (state->rebasing)
				skip = parse_mail_rebase(state, mail);
			નહીંતર
				skip = parse_mail(state, mail);

			જો (skip)
				જાઓ next; /* mail should be skipped */

			જો (state->signoff)
				am_append_signoff(state);

			write_author_script(state);
			write_commit_સંદેશ(state);
		}

		જો (state->interactive && do_interactive(state))
			જાઓ next;

		to_keep = 0;
		જો (is_empty_or_missing_file(am_path(state, "patch"))) {
			પસંદગી (state->empty_type) {
			સ્થિતિ DROP_EMPTY_COMMIT:
				say(state, stdout, _("Skipping: %.*s"), linelen(state->સંદેશ), state->સંદેશ);
				જાઓ next;
				તોડો;
			સ્થિતિ KEEP_EMPTY_COMMIT:
				to_keep = 1;
				say(state, stdout, _("Creating an empty commit: %.*s"),
					linelen(state->સંદેશ), state->સંદેશ);
				તોડો;
			સ્થિતિ STOP_ON_EMPTY_COMMIT:
				printf_ln(_("Patch is empty."));
				die_user_resolve(state);
				તોડો;
			}
		}

		જો (run_applypatch_સંદેશ_hook(state))
			exit(1);
		જો (to_keep)
			જાઓ commit;

		say(state, stdout, _("Applying: %.*s"), linelen(state->સંદેશ), state->સંદેશ);

		apply_status = run_apply(state, ખાલી);

		જો (apply_status && state->threeway) {
			સંરચના strbuf sb = STRBUF_INIT;

			strbuf_addstr(&sb, am_path(state, "patch-merge-index"));
			apply_status = fall_back_threeway(state, sb.buf);
			strbuf_release(&sb);

			/*
			 * Applying the patch to an earlier tree and merging
			 * the result may have produced the same tree as ours.
			 */
			જો (!apply_status &&
			    !repo_index_has_changes(the_repository, ખાલી, ખાલી)) {
				say(state, stdout, _("No changes -- Patch already applied."));
				જાઓ next;
			}
		}

		જો (apply_status) {
			printf_ln(_("Patch failed at %s %.*s"), msgnum(state),
				linelen(state->સંદેશ), state->સંદેશ);

			જો (advice_enabled(ADVICE_AM_WORK_DIR))
				advise(_("Use 'git am --show-current-patch=diff' to see the failed patch"));

			die_user_resolve(state);
		}

commit:
		do_commit(state);

next:
		am_next(state);

		જો (resume)
			am_load(state);
		resume = 0;
	}

	જો (!is_empty_or_missing_file(am_path(state, "rewritten"))) {
		પ્રતિજ્ઞા(state->rebasing);
		copy_notes_for_rebase(state);
		run_post_rewrite_hook(state);
	}

	/*
	 * In rebasing mode, it's up to the caller to take care of
	 * housekeeping.
	 */
	જો (!state->rebasing) {
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
સ્થિર શૂન્ય am_resolve(સંરચના am_state *state, પૂર્ણાંક allow_empty)
{
	validate_resume_state(state);

	say(state, stdout, _("Applying: %.*s"), linelen(state->સંદેશ), state->સંદેશ);

	જો (!repo_index_has_changes(the_repository, ખાલી, ખાલી)) {
		જો (allow_empty && is_empty_or_missing_file(am_path(state, "patch"))) {
			printf_ln(_("No changes - recorded it as an empty commit."));
		} નહીંતર {
			printf_ln(_("No changes - did you forget to use 'git add'?\n"
				    "If there is nothing left to stage, chances are that something else\n"
				    "already introduced the same changes; you might want to skip this patch."));
			die_user_resolve(state);
		}
	}

	જો (unmerged_index(the_repository->index)) {
		printf_ln(_("You still have unmerged paths in your index.\n"
			"You should 'git add' each file with resolved conflicts to mark them as such.\n"
			"You might run `git rm` on a file to accept \"deleted by them\" for it."));
		die_user_resolve(state);
	}

	જો (state->interactive) {
		write_index_patch(state);
		જો (do_interactive(state))
			જાઓ next;
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
સ્થિર પૂર્ણાંક fast_forward_to(સંરચના વૃક્ષ *head, સંરચના વૃક્ષ *remote, પૂર્ણાંક reset)
{
	સંરચના lock_file lock_file = LOCK_INIT;
	સંરચના unpack_trees_options opts;
	સંરચના વૃક્ષ_desc t[2];

	જો (repo_parse_વૃક્ષ(the_repository, head) || repo_parse_વૃક્ષ(the_repository, remote))
		પાછા -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	refresh_index(the_repository->index, REFRESH_QUIET, ખાલી, ખાલી, ખાલી);

	memset(&opts, 0, કદ(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.update = 1;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = twoway_merge;
	init_વૃક્ષ_desc(&t[0], &head->object.oid, head->buffer, head->size);
	init_વૃક્ષ_desc(&t[1], &remote->object.oid, remote->buffer, remote->size);

	જો (unpack_trees(2, t, &opts)) {
		rollback_lock_file(&lock_file);
		પાછા -1;
	}

	જો (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	પાછા 0;
}

/**
 * Merges a tree into the index. The index's stat info will take precedence
 * over the merged tree's. Returns 0 on success, -1 on failure.
 */
સ્થિર પૂર્ણાંક merge_વૃક્ષ(સંરચના વૃક્ષ *વૃક્ષ)
{
	સંરચના lock_file lock_file = LOCK_INIT;
	સંરચના unpack_trees_options opts;
	સંરચના વૃક્ષ_desc t[1];

	જો (repo_parse_વૃક્ષ(the_repository, વૃક્ષ))
		પાછા -1;

	repo_hold_locked_index(the_repository, &lock_file, LOCK_DIE_ON_ERROR);

	memset(&opts, 0, કદ(opts));
	opts.head_idx = 1;
	opts.src_index = the_repository->index;
	opts.dst_index = the_repository->index;
	opts.merge = 1;
	opts.fn = oneway_merge;
	init_વૃક્ષ_desc(&t[0], &વૃક્ષ->object.oid, વૃક્ષ->buffer, વૃક્ષ->size);

	જો (unpack_trees(1, t, &opts)) {
		rollback_lock_file(&lock_file);
		પાછા -1;
	}

	જો (write_locked_index(the_repository->index, &lock_file, COMMIT_LOCK))
		die(_("unable to write new index file"));

	પાછા 0;
}

/**
 * Clean the index without touching entries that are not modified between
 * `head` and `remote`.
 */
સ્થિર પૂર્ણાંક clean_index(અચલ સંરચના object_id *head, અચલ સંરચના object_id *remote)
{
	સંરચના વૃક્ષ *head_વૃક્ષ, *remote_વૃક્ષ, *index_વૃક્ષ;
	સંરચના object_id index;

	head_વૃક્ષ = repo_parse_વૃક્ષ_indirect(the_repository, head);
	જો (!head_વૃક્ષ)
		પાછા error(_("Could not parse object '%s'."), oid_to_hex(head));

	remote_વૃક્ષ = repo_parse_વૃક્ષ_indirect(the_repository, remote);
	જો (!remote_વૃક્ષ)
		પાછા error(_("Could not parse object '%s'."), oid_to_hex(remote));

	repo_read_index_unmerged(the_repository);

	જો (fast_forward_to(head_વૃક્ષ, head_વૃક્ષ, 1))
		પાછા -1;

	જો (write_index_as_વૃક્ષ(&index, the_repository->index,
				repo_get_index_file(the_repository),
				0, ખાલી))
		પાછા -1;

	index_વૃક્ષ = repo_parse_વૃક્ષ_indirect(the_repository, &index);
	જો (!index_વૃક્ષ)
		પાછા error(_("Could not parse object '%s'."), oid_to_hex(&index));

	જો (fast_forward_to(index_વૃક્ષ, remote_વૃક્ષ, 0))
		પાછા -1;

	જો (merge_વૃક્ષ(remote_વૃક્ષ))
		પાછા -1;

	remove_branch_state(the_repository, 0);

	પાછા 0;
}

/**
 * Resets rerere's merge resolution metadata.
 */
સ્થિર શૂન્ય am_rerere_clear(શૂન્ય)
{
	સંરચના string_list merge_rr = STRING_LIST_INIT_DUP;
	rerere_clear(the_repository, &merge_rr);
	string_list_clear(&merge_rr, 1);
}

/**
 * Resume the current am session by skipping the current patch.
 */
સ્થિર શૂન્ય am_skip(સંરચના am_state *state)
{
	સંરચના object_id head;

	am_rerere_clear();

	જો (repo_get_oid(the_repository, "HEAD", &head))
		oidcpy(&head, the_hash_algo->empty_વૃક્ષ);

	જો (clean_index(&head, &head))
		die(_("failed to clean index"));

	જો (state->rebasing) {
		FILE *fp = xfopen(am_path(state, "rewritten"), "a");

		પ્રતિજ્ઞા(!is_null_oid(&state->orig_commit));
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
સ્થિર પૂર્ણાંક safe_to_abort(અચલ સંરચના am_state *state)
{
	સંરચના strbuf sb = STRBUF_INIT;
	સંરચના object_id abort_safety, head;

	જો (file_exists(am_path(state, "dirtyindex")))
		પાછા 0;

	જો (read_state_file(&sb, state, "abort-safety", 1) > 0) {
		જો (get_oid_hex(sb.buf, &abort_safety))
			die(_("could not parse %s"), am_path(state, "abort-safety"));
	} નહીંતર
		oidclr(&abort_safety, the_repository->hash_algo);
	strbuf_release(&sb);

	જો (repo_get_oid(the_repository, "HEAD", &head))
		oidclr(&head, the_repository->hash_algo);

	જો (oideq(&head, &abort_safety))
		પાછા 1;

	warning(_("You seem to have moved HEAD since the last 'am' failure.\n"
		"Not rewinding to ORIG_HEAD"));

	પાછા 0;
}

/**
 * Aborts the current am session if it is safe to do so.
 */
સ્થિર શૂન્ય am_abort(સંરચના am_state *state)
{
	સંરચના object_id curr_head, orig_head;
	પૂર્ણાંક has_curr_head, has_orig_head;
	અક્ષર *curr_branch;

	જો (!safe_to_abort(state)) {
		am_destroy(state);
		પાછા;
	}

	am_rerere_clear();

	curr_branch = refs_resolve_refdup(get_main_ref_store(the_repository),
					  "HEAD", 0, &curr_head, ખાલી);
	has_curr_head = curr_branch && !is_null_oid(&curr_head);
	જો (!has_curr_head)
		oidcpy(&curr_head, the_hash_algo->empty_વૃક્ષ);

	has_orig_head = !repo_get_oid(the_repository, "ORIG_HEAD", &orig_head);
	જો (!has_orig_head)
		oidcpy(&orig_head, the_hash_algo->empty_વૃક્ષ);

	જો (clean_index(&curr_head, &orig_head))
		die(_("failed to clean index"));

	જો (has_orig_head)
		refs_update_ref(get_main_ref_store(the_repository),
				"am --abort", "HEAD", &orig_head,
				has_curr_head ? &curr_head : ખાલી, 0,
				UPDATE_REFS_DIE_ON_ERR);
	નહીંતર જો (curr_branch)
		refs_delete_ref(get_main_ref_store(the_repository), ખાલી,
				curr_branch, ખાલી, REF_NO_DEREF);

	મુક્ત(curr_branch);
	am_destroy(state);
}

સ્થિર પૂર્ણાંક show_patch(સંરચના am_state *state, ગણના resume_type resume_mode)
{
	સંરચના strbuf sb = STRBUF_INIT;
	અચલ અક્ષર *patch_path;
	પૂર્ણાંક len;

	જો (!is_null_oid(&state->orig_commit)) {
		સંરચના child_process cmd = CHILD_PROCESS_INIT;

		strvec_pushl(&cmd.args, "show", oid_to_hex(&state->orig_commit),
			     "--", ખાલી);
		cmd.git_cmd = 1;
		પાછા run_command(&cmd);
	}

	પસંદગી (resume_mode) {
	સ્થિતિ RESUME_SHOW_PATCH_RAW:
		patch_path = am_path(state, msgnum(state));
		તોડો;
	સ્થિતિ RESUME_SHOW_PATCH_DIFF:
		patch_path = am_path(state, "patch");
		તોડો;
	મૂળભૂત:
		BUG("invalid mode for --show-current-patch");
	}

	len = strbuf_read_file(&sb, patch_path, 0);
	જો (len < 0)
		die_errno(_("failed to read '%s'"), patch_path);

	setup_pager(the_repository);
	write_in_full(1, sb.buf, sb.len);
	strbuf_release(&sb);
	પાછા 0;
}

/**
 * parse_options() callback that validates and sets opt->value to the
 * PATCH_FORMAT_* enum value corresponding to `arg`.
 */
સ્થિર પૂર્ણાંક parse_opt_patchformat(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	પૂર્ણાંક *opt_કિંમત = opt->કિંમત;

	જો (unset)
		*opt_કિંમત = PATCH_FORMAT_UNKNOWN;
	નહીંતર જો (!strcmp(arg, "mbox"))
		*opt_કિંમત = PATCH_FORMAT_MBOX;
	નહીંતર જો (!strcmp(arg, "stgit"))
		*opt_કિંમત = PATCH_FORMAT_STGIT;
	નહીંતર જો (!strcmp(arg, "stgit-series"))
		*opt_કિંમત = PATCH_FORMAT_STGIT_SERIES;
	નહીંતર જો (!strcmp(arg, "hg"))
		*opt_કિંમત = PATCH_FORMAT_HG;
	નહીંતર જો (!strcmp(arg, "mboxrd"))
		*opt_કિંમત = PATCH_FORMAT_MBOXRD;
	/*
	 * Please update $__git_patchformat in git-completion.bash
	 * when you add new options
	 */
	નહીંતર
		પાછા error(_("invalid value for '%s': '%s'"),
			     "--patch-format", arg);
	પાછા 0;
}

સ્થિર પૂર્ણાંક parse_opt_show_current_patch(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	પૂર્ણાંક *opt_કિંમત = opt->કિંમત;

	BUG_ON_OPT_NEG(unset);

	જો (!arg)
		*opt_કિંમત = opt->defval;
	નહીંતર જો (!strcmp(arg, "raw"))
		*opt_કિંમત = RESUME_SHOW_PATCH_RAW;
	નહીંતર જો (!strcmp(arg, "diff"))
		*opt_કિંમત = RESUME_SHOW_PATCH_DIFF;
	/*
	 * Please update $__git_showcurrentpatch in git-completion.bash
	 * when you add new options
	 */
	નહીંતર
		પાછા error(_("invalid value for '%s': '%s'"),
			     "--show-current-patch", arg);
	પાછા 0;
}

પૂર્ણાંક cmd_am(પૂર્ણાંક argc,
	   અચલ અક્ષર **argv,
	   અચલ અક્ષર *prefix,
	   સંરચના repository *repo UNUSED)
{
	સંરચના am_state state;
	પૂર્ણાંક દ્વિઅંકી = -1;
	પૂર્ણાંક keep_cr = -1;
	પૂર્ણાંક patch_format = PATCH_FORMAT_UNKNOWN;
	ગણના resume_type resume_mode = RESUME_FALSE;
	પૂર્ણાંક in_progress;
	પૂર્ણાંક ret = 0;

	અચલ અક્ષર * અચલ usage[] = {
		N_("git am [<options>] [(<mbox> | <Maildir>)...]"),
		N_("git am [<options>] (--continue | --skip | --abort)"),
		ખાલી
	};

	સંરચના option options[] = {
		OPT_BOOL('i', "interactive", &state.interactive,
			N_("run interactively")),
		OPT_BOOL('n', "no-verify", &state.no_verify,
			N_("bypass pre-applypatch and applypatch-msg hooks")),
		OPT_HIDDEN_BOOL('b', "binary", &દ્વિઅંકી,
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
		OPT_PASSTHRU_ARGV(0, "ignore-space-change", &state.git_apply_opts, ખાલી,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU_ARGV(0, "ignore-whitespace", &state.git_apply_opts, ખાલી,
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
		OPT_PASSTHRU_ARGV('C', ખાલી, &state.git_apply_opts, N_("n"),
			N_("pass it through git-apply"),
			0),
		OPT_PASSTHRU_ARGV('p', ખાલી, &state.git_apply_opts, N_("num"),
			N_("pass it through git-apply"),
			0),
		OPT_CALLBACK(0, "patch-format", &patch_format, N_("format"),
			N_("format the patch(es) are in"),
			parse_opt_patchformat),
		OPT_PASSTHRU_ARGV(0, "reject", &state.git_apply_opts, ખાલી,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_STRING(0, "resolvemsg", &state.resolvemsg, ખાલી,
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
			.કિંમત = &resume_mode,
			.precision = કદ(resume_mode),
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
			.કિંમત = &state.sign_commit,
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

	repo_config(the_repository, git_default_config, ખાલી);

	am_state_init(&state);

	in_progress = am_in_progress(&state);
	જો (in_progress)
		am_load(&state);

	argc = parse_options(argc, argv, prefix, options, usage, 0);

	જો (દ્વિઅંકી >= 0)
		fprintf_ln(stderr, _("The -b/--binary option has been a no-op for long time, and\n"
				"it will be removed. Please do not use it anymore."));

	/* Ensure a valid committer ident can be constructed */
	git_committer_info(IDENT_STRICT);

	જો (repo_read_index_preload(the_repository, ખાલી, 0) < 0)
		die(_("failed to read the index"));

	જો (in_progress) {
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
		જો (argc || (resume_mode == RESUME_FALSE && !isatty(0)))
			die(_("previous rebase directory %s still exists but mbox given."),
				state.dir);

		જો (resume_mode == RESUME_FALSE)
			resume_mode = RESUME_APPLY;

		જો (state.signoff == SIGNOFF_EXPLICIT)
			am_append_signoff(&state);
	} નહીંતર {
		સંરચના strvec paths = STRVEC_INIT;
		પૂર્ણાંક i;

		/*
		 * Handle stray state directory in the independent-run case. In
		 * the --rebasing case, it is up to the caller to take care of
		 * stray directories.
		 */
		જો (file_exists(state.dir) && !state.rebasing) {
			જો (resume_mode == RESUME_ABORT || resume_mode == RESUME_QUIT) {
				am_destroy(&state);
				am_state_release(&state);
				પાછા 0;
			}

			die(_("Stray %s directory found.\n"
				"Use \"git am --abort\" to remove it."),
				state.dir);
		}

		જો (resume_mode)
			die(_("Resolve operation not in progress, we are not resuming."));

		માટે (i = 0; i < argc; i++) {
			જો (is_absolute_path(argv[i]) || !prefix)
				strvec_ધકેલો(&paths, argv[i]);
			નહીંતર
				strvec_ધકેલો(&paths, mkpath("%s/%s", prefix, argv[i]));
		}

		જો (state.interactive && !paths.nr)
			die(_("interactive mode requires patches on the command line"));

		am_setup(&state, patch_format, paths.v, keep_cr);

		strvec_clear(&paths);
	}

	પસંદગી (resume_mode) {
	સ્થિતિ RESUME_FALSE:
		am_run(&state, 0);
		તોડો;
	સ્થિતિ RESUME_APPLY:
		am_run(&state, 1);
		તોડો;
	સ્થિતિ RESUME_RESOLVED:
	સ્થિતિ RESUME_ALLOW_EMPTY:
		am_resolve(&state, resume_mode == RESUME_ALLOW_EMPTY ? 1 : 0);
		તોડો;
	સ્થિતિ RESUME_SKIP:
		am_skip(&state);
		તોડો;
	સ્થિતિ RESUME_ABORT:
		am_abort(&state);
		તોડો;
	સ્થિતિ RESUME_QUIT:
		am_rerere_clear();
		am_destroy(&state);
		તોડો;
	સ્થિતિ RESUME_SHOW_PATCH_RAW:
	સ્થિતિ RESUME_SHOW_PATCH_DIFF:
		ret = show_patch(&state, resume_mode);
		તોડો;
	મૂળભૂત:
		BUG("invalid resume value");
	}

	am_state_release(&state);

	પાછા ret;
}
