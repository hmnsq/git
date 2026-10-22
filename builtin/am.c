/*
 * Builtin "git am"
 *
 * Based on git-am.sh by Junio C Hamano.
 */

#परिभाषय USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "abspath.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "editor.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "hook.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "tempfile.h"
#अन्तर्भाव "lockfile.h"
#अन्तर्भाव "cache-tree.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "commit.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "unpack-trees.h"
#अन्तर्भाव "branch.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "preload-index.h"
#अन्तर्भाव "sequencer.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "merge-ort-wrappers.h"
#अन्तर्भाव "log-tree.h"
#अन्तर्भाव "notes-utils.h"
#अन्तर्भाव "rerere.h"
#अन्तर्भाव "mailinfo.h"
#अन्तर्भाव "apply.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "pager.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "pretty.h"

/**
 * Returns the length of the first line of msg.
 */
स्थिर पूर्णाङ्क linelen(ध्रुव वर्ण *सन्देशः)
{
	प्रत्यागम strchrnul(सन्देशः, '\n') - सन्देशः;
}

/**
 * Returns true if `str` consists of only whitespace, false otherwise.
 */
स्थिर पूर्णाङ्क str_isspace(ध्रुव वर्ण *str)
{
	प्रति (; *str; str++)
		यदि (!रिक्तम्_वा(*str))
			प्रत्यागम 0;

	प्रत्यागम 1;
}

गणना patch_format {
	PATCH_FORMAT_UNKNOWN = 0,
	PATCH_FORMAT_MBOX,
	PATCH_FORMAT_STGIT,
	PATCH_FORMAT_STGIT_SERIES,
	PATCH_FORMAT_HG,
	PATCH_FORMAT_MBOXRD
};

गणना keep_type {
	KEEP_FALSE = 0,
	KEEP_TRUE,      /* pass -k flag to git-mailinfo */
	KEEP_NON_PATCH  /* pass -b flag to git-mailinfo */
};

गणना scissors_type {
	SCISSORS_UNSET = -1,
	SCISSORS_FALSE = 0,  /* pass --no-scissors to git-mailinfo */
	SCISSORS_TRUE        /* pass --scissors to git-mailinfo */
};

गणना signoff_type {
	SIGNOFF_FALSE = 0,
	SIGNOFF_TRUE = 1,
	SIGNOFF_EXPLICIT /* --signoff was set on the command-line */
};

गणना resume_type {
	RESUME_FALSE = 0,
	RESUME_APPLY,
	RESUME_RESOLVED,
	RESUME_SKIP,
	RESUME_ABORT,
	RESUME_QUIT,
	RESUME_SHOW_PATCH_RAW,
	RESUME_SHOW_PATCH_DIFF,
	RESUME_ALLOW_रिक्तानि,
};

गणना रिक्तानि_action {
	STOP_ON_रिक्तानि_COMMIT = 0,  /* output errors and stop in the middle of an am session */
	DROP_रिक्तानि_COMMIT,         /* skip with a notice message, unless "--quiet" has been passed */
	KEEP_रिक्तानि_COMMIT,         /* keep recording as empty commits */
};

संरचना am_state {
	/* state directory path */
	वर्ण *dir;

	/* current and last patch numbers, 1-indexed */
	पूर्णाङ्क cur;
	पूर्णाङ्क चरमः;

	/* commit metadata and message */
	वर्ण *author_name;
	वर्ण *author_email;
	वर्ण *author_date;
	वर्ण *सन्देशः;
	आकार_t सन्देशः_len;

	/* when --rebasing, records the original commit the patch came from */
	संरचना object_id orig_commit;

	/* number of digits in patch filename */
	पूर्णाङ्क prec;

	/* various operating modes and command line options */
	पूर्णाङ्क interactive;
	पूर्णाङ्क no_verify;
	पूर्णाङ्क threeway;
	पूर्णाङ्क quiet;
	पूर्णाङ्क signoff; /* enum signoff_type */
	पूर्णाङ्क utf8;
	पूर्णाङ्क keep; /* enum keep_type */
	पूर्णाङ्क सन्देशः_id;
	पूर्णाङ्क scissors; /* enum scissors_type */
	पूर्णाङ्क quoted_cr; /* enum quoted_cr_action */
	पूर्णाङ्क रिक्तानि_type; /* enum empty_action */
	संरचना strvec git_apply_opts;
	ध्रुव वर्ण *resolvemsg;
	पूर्णाङ्क committer_date_is_author_date;
	पूर्णाङ्क ignore_date;
	पूर्णाङ्क allow_rerere_autoupdate;
	ध्रुव वर्ण *sign_commit;
	पूर्णाङ्क rebasing;
};

/**
 * Initializes am_state with the default values.
 */
स्थिर शून्य am_state_init(संरचना am_state *state)
{
	पूर्णाङ्क gpgsign;

	स्मृतिं_पूरय(state, 0, परिमाण(*state));

	state->dir = repo_git_मार्गः(the_repository, "rebase-apply");

	state->prec = 4;

	repo_config_get_bool(the_repository, "am.threeway", &state->threeway);

	state->utf8 = 1;

	repo_config_get_bool(the_repository, "am.messageid", &state->सन्देशः_id);

	state->scissors = SCISSORS_UNSET;
	state->quoted_cr = quoted_cr_unset;

	strvec_init(&state->git_apply_opts);

	यदि (!repo_config_get_bool(the_repository, "commit.gpgsign", &gpgsign))
		state->sign_commit = gpgsign ? "" : रिक्त;
}

/**
 * Releases memory allocated by an am_state.
 */
स्थिर शून्य am_state_release(संरचना am_state *state)
{
	मुक्त(state->dir);
	मुक्त(state->author_name);
	मुक्त(state->author_email);
	मुक्त(state->author_date);
	मुक्त(state->सन्देशः);
	strvec_clear(&state->git_apply_opts);
}

स्थिर पूर्णाङ्क am_option_parse_quoted_cr(ध्रुव संरचना option *opt,
				     ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	BUG_ON_OPT_NEG(unset);

	यदि (mailinfo_parse_quoted_cr_action(arg, opt->मानम्) != 0)
		प्रत्यागम error(_("bad action '%s' for '%s'"), arg, "--quoted-cr");
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क am_option_parse_रिक्तानि(ध्रुव संरचना option *opt,
				     ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *opt_मानम् = opt->मानम्;

	BUG_ON_OPT_NEG(unset);

	यदि (!वाक्यं_तुलय(arg, "stop"))
		*opt_मानम् = STOP_ON_रिक्तानि_COMMIT;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "drop"))
		*opt_मानम् = DROP_रिक्तानि_COMMIT;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "keep"))
		*opt_मानम् = KEEP_रिक्तानि_COMMIT;
	अन्यथा
		प्रत्यागम error(_("invalid value for '%s': '%s'"), "--empty", arg);

	प्रत्यागम 0;
}

/**
 * Returns path relative to the am_state directory.
 */
स्थिर अन्तःस्थ ध्रुव वर्ण *am_मार्गः(ध्रुव संरचना am_state *state, ध्रुव वर्ण *मार्गः)
{
	प्रत्यागम mkpath("%s/%s", state->dir, मार्गः);
}

/**
 * For convenience to call write_file()
 */
स्थिर शून्य write_state_text(ध्रुव संरचना am_state *state,
			     ध्रुव वर्ण *name, ध्रुव वर्ण *string)
{
	write_file(am_मार्गः(state, name), "%s", string);
}

स्थिर शून्य write_state_गणनम्(ध्रुव संरचना am_state *state,
			      ध्रुव वर्ण *name, पूर्णाङ्क मानम्)
{
	write_file(am_मार्गः(state, name), "%d", मानम्);
}

स्थिर शून्य write_state_bool(ध्रुव संरचना am_state *state,
			     ध्रुव वर्ण *name, पूर्णाङ्क मानम्)
{
	write_state_text(state, name, मानम् ? "t" : "f");
}

/**
 * If state->quiet is false, calls fprintf(fp, fmt, ...), and appends a newline
 * at the end.
 */
__attribute__((format (प्रारूप_लिख, 3, 4)))
स्थिर शून्य say(ध्रुव संरचना am_state *state, FILE *fp, ध्रुव वर्ण *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	यदि (!state->quiet) {
		vfprintf(fp, fmt, ap);
		putc('\n', fp);
	}
	va_अन्तः(ap);
}

/**
 * Returns 1 if there is an am session in progress, 0 otherwise.
 */
स्थिर पूर्णाङ्क am_अन्तर्_प्रगतिः(ध्रुव संरचना am_state *state)
{
	संरचना stat st;

	यदि (lstat(state->dir, &st) < 0 || !S_ISDIR(st.st_बहुलकम्))
		प्रत्यागम 0;
	यदि (lstat(am_मार्गः(state, "last"), &st) || !S_ISREG(st.st_बहुलकम्))
		प्रत्यागम 0;
	यदि (lstat(am_मार्गः(state, "next"), &st) || !S_ISREG(st.st_बहुलकम्))
		प्रत्यागम 0;
	प्रत्यागम 1;
}

/**
 * Reads the contents of `file` in the `state` directory into `sb`. Returns the
 * number of bytes read on success, -1 if the file does not exist. If `trim` is
 * set, trailing whitespace will be removed.
 */
स्थिर पूर्णाङ्क read_state_file(संरचना strbuf *sb, ध्रुव संरचना am_state *state,
			ध्रुव वर्ण *file, पूर्णाङ्क trim)
{
	strbuf_reset(sb);

	यदि (strbuf_read_file(sb, am_मार्गः(state, file), 0) >= 0) {
		यदि (trim)
			strbuf_trim(sb);

		प्रत्यागम sb->len;
	}

	यदि (errno == ENOENT)
		प्रत्यागम -1;

	die_errno(_("could not read '%s'"), am_मार्गः(state, file));
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
स्थिर पूर्णाङ्क read_am_author_script(संरचना am_state *state)
{
	ध्रुव वर्ण *filename = am_मार्गः(state, "author-script");

	प्रतिज्ञा(!state->author_name);
	प्रतिज्ञा(!state->author_email);
	प्रतिज्ञा(!state->author_date);

	प्रत्यागम read_author_script(filename, &state->author_name,
				  &state->author_email, &state->author_date, 1);
}

/**
 * Saves state->author_name, state->author_email and state->author_date in the
 * state directory's "author-script" file.
 */
स्थिर शून्य write_author_script(ध्रुव संरचना am_state *state)
{
	संरचना strbuf sb = STRBUF_INIT;

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
स्थिर पूर्णाङ्क read_commit_सन्देशः(संरचना am_state *state)
{
	संरचना strbuf sb = STRBUF_INIT;

	प्रतिज्ञा(!state->सन्देशः);

	यदि (read_state_file(&sb, state, "final-commit", 0) < 0) {
		strbuf_release(&sb);
		प्रत्यागम -1;
	}

	state->सन्देशः = strbuf_detach(&sb, &state->सन्देशः_len);
	प्रत्यागम 0;
}

/**
 * Saves state->msg in the state directory's "final-commit" file.
 */
स्थिर शून्य write_commit_सन्देशः(ध्रुव संरचना am_state *state)
{
	ध्रुव वर्ण *filename = am_मार्गः(state, "final-commit");
	write_file_buf(filename, state->सन्देशः, state->सन्देशः_len);
}

/**
 * Loads state from disk.
 */
स्थिर शून्य am_आरोपय(संरचना am_state *state)
{
	संरचना strbuf sb = STRBUF_INIT;

	यदि (read_state_file(&sb, state, "next", 1) < 0)
		BUG("state file 'next' does not exist");
	state->cur = strtol(sb.buf, रिक्त, 10);

	यदि (read_state_file(&sb, state, "last", 1) < 0)
		BUG("state file 'last' does not exist");
	state->चरमः = strtol(sb.buf, रिक्त, 10);

	यदि (read_am_author_script(state) < 0)
		die(_("could not parse author script"));

	read_commit_सन्देशः(state);

	यदि (read_state_file(&sb, state, "original-commit", 1) < 0)
		oidclr(&state->orig_commit, the_repository->hash_algo);
	अन्यथा यदि (get_oid_hex(sb.buf, &state->orig_commit) < 0)
		die(_("could not parse %s"), am_मार्गः(state, "original-commit"));

	read_state_file(&sb, state, "threeway", 1);
	state->threeway = !वाक्यं_तुलय(sb.buf, "t");

	read_state_file(&sb, state, "quiet", 1);
	state->quiet = !वाक्यं_तुलय(sb.buf, "t");

	read_state_file(&sb, state, "sign", 1);
	state->signoff = !वाक्यं_तुलय(sb.buf, "t");

	read_state_file(&sb, state, "utf8", 1);
	state->utf8 = !वाक्यं_तुलय(sb.buf, "t");

	यदि (file_exists(am_मार्गः(state, "rerere-autoupdate"))) {
		read_state_file(&sb, state, "rerere-autoupdate", 1);
		state->allow_rerere_autoupdate = वाक्यं_तुलय(sb.buf, "t") ?
			RERERE_NOAUTOUPDATE : RERERE_AUTOUPDATE;
	} अन्यथा {
		state->allow_rerere_autoupdate = 0;
	}

	read_state_file(&sb, state, "keep", 1);
	यदि (!वाक्यं_तुलय(sb.buf, "t"))
		state->keep = KEEP_TRUE;
	अन्यथा यदि (!वाक्यं_तुलय(sb.buf, "b"))
		state->keep = KEEP_NON_PATCH;
	अन्यथा
		state->keep = KEEP_FALSE;

	read_state_file(&sb, state, "messageid", 1);
	state->सन्देशः_id = !वाक्यं_तुलय(sb.buf, "t");

	read_state_file(&sb, state, "scissors", 1);
	यदि (!वाक्यं_तुलय(sb.buf, "t"))
		state->scissors = SCISSORS_TRUE;
	अन्यथा यदि (!वाक्यं_तुलय(sb.buf, "f"))
		state->scissors = SCISSORS_FALSE;
	अन्यथा
		state->scissors = SCISSORS_UNSET;

	read_state_file(&sb, state, "quoted-cr", 1);
	यदि (!*sb.buf)
		state->quoted_cr = quoted_cr_unset;
	अन्यथा यदि (mailinfo_parse_quoted_cr_action(sb.buf, &state->quoted_cr) != 0)
		die(_("could not parse %s"), am_मार्गः(state, "quoted-cr"));

	read_state_file(&sb, state, "apply-opt", 1);
	strvec_clear(&state->git_apply_opts);
	यदि (sq_dequote_गन्तव्यम्_strvec(sb.buf, &state->git_apply_opts) < 0)
		die(_("could not parse %s"), am_मार्गः(state, "apply-opt"));

	state->rebasing = !!file_exists(am_मार्गः(state, "rebasing"));

	strbuf_release(&sb);
}

/**
 * Removes the am_state directory, forcefully terminating the current am
 * session.
 */
स्थिर शून्य am_destroy(ध्रुव संरचना am_state *state)
{
	संरचना strbuf sb = STRBUF_INIT;

	strbuf_addstr(&sb, state->dir);
	remove_dir_recursively(&sb, 0);
	strbuf_release(&sb);
}

/**
 * Runs applypatch-msg hook. Returns its exit code.
 */
स्थिर पूर्णाङ्क सततम्_applypatch_सन्देशः_hook(संरचना am_state *state)
{
	पूर्णाङ्क ret = 0;

	प्रतिज्ञा(state->सन्देशः);

	यदि (!state->no_verify) {
		संरचना सततम्_hooks_opt opt = सततम्_HOOKS_OPT_INIT_FORCE_SERIAL;
		strvec_प्रक्षिप(&opt.तर्काः, am_मार्गः(state, "final-commit"));
		ret = सततम्_hooks_opt(the_repository, "applypatch-msg", &opt);
	}

	यदि (!ret) {
		FREE_AND_NULL(state->सन्देशः);
		यदि (read_commit_सन्देशः(state) < 0)
			die(_("'%s' was deleted by the applypatch-msg hook"),
				am_मार्गः(state, "final-commit"));
	}

	प्रत्यागम ret;
}

/**
 * Runs post-rewrite hook. Returns it exit code.
 */
स्थिर पूर्णाङ्क सततम्_post_rewrite_hook(ध्रुव संरचना am_state *state)
{
	संरचना सततम्_hooks_opt opt = सततम्_HOOKS_OPT_INIT;

	strvec_प्रक्षिप(&opt.तर्काः, "rebase");
	opt.मार्गः_गन्तव्यम्_stdin = am_मार्गः(state, "rewritten");

	प्रत्यागम सततम्_hooks_opt(the_repository, "post-rewrite", &opt);
}

/**
 * Reads the state directory's "rewritten" file, and copies notes from the old
 * commits listed in the file to their rewritten commits.
 *
 * Returns 0 on success, -1 on failure.
 */
स्थिर पूर्णाङ्क copy_notes_for_rebase(ध्रुव संरचना am_state *state)
{
	संरचना notes_rewrite_cfg *c;
	संरचना strbuf sb = STRBUF_INIT;
	ध्रुव वर्ण *invalid_पङ्क्तिः = _("Malformed input line: '%s'.");
	ध्रुव वर्ण *सन्देशः = "Notes added by 'git rebase'";
	FILE *fp;
	पूर्णाङ्क ret = 0;

	प्रतिज्ञा(state->rebasing);

	c = init_copy_notes_for_rewrite("rebase");
	यदि (!c)
		प्रत्यागम 0;

	fp = xfopen(am_मार्गः(state, "rewritten"), "r");

	यावत् (!strbuf_getline_lf(&sb, fp)) {
		संरचना object_id from_obj, गन्तव्यम्_obj;
		ध्रुव वर्ण *p;

		यदि (sb.len != the_hash_algo->hexsz * 2 + 1) {
			ret = error(invalid_पङ्क्तिः, sb.buf);
			गच्छ finish;
		}

		यदि (parse_oid_hex(sb.buf, &from_obj, &p)) {
			ret = error(invalid_पङ्क्तिः, sb.buf);
			गच्छ finish;
		}

		यदि (*p != ' ') {
			ret = error(invalid_पङ्क्तिः, sb.buf);
			गच्छ finish;
		}

		यदि (get_oid_hex(p + 1, &गन्तव्यम्_obj)) {
			ret = error(invalid_पङ्क्तिः, sb.buf);
			गच्छ finish;
		}

		यदि (copy_note_for_rewrite(c, &from_obj, &गन्तव्यम्_obj))
			ret = error(_("Failed to copy notes from '%s' to '%s'"),
					oid_गन्तव्यम्_hex(&from_obj), oid_गन्तव्यम्_hex(&गन्तव्यम्_obj));
	}

finish:
	finish_copy_notes_for_rewrite(the_repository, c, सन्देशः);
	fclose(fp);
	strbuf_release(&sb);
	प्रत्यागम ret;
}

/**
 * Determines if the file looks like a piece of RFC2822 mail by grabbing all
 * non-indented lines and checking if they look like they begin with valid
 * header field names.
 *
 * Returns 1 if the file looks like a piece of mail, 0 otherwise.
 */
स्थिर पूर्णाङ्क is_mail(FILE *fp)
{
	ध्रुव वर्ण *शीर्षकम्_regex = "^[!-9;-~]+:";
	संरचना strbuf sb = STRBUF_INIT;
	regex_t regex;
	पूर्णाङ्क ret = 1;

	यदि (fseek(fp, 0L, SEEK_समुच्चयः))
		die_errno(_("fseek failed"));

	यदि (regcomp(&regex, शीर्षकम्_regex, REG_NOSUB | REG_EXTENDED))
		die("invalid pattern: %s", शीर्षकम्_regex);

	यावत् (!strbuf_getline(&sb, fp)) {
		यदि (!sb.len)
			भङ्ग; /* End of header */

		/* Ignore indented folded lines */
		यदि (*sb.buf == '\t' || *sb.buf == ' ')
			अनुवर्त;

		/* It's a header if it matches header_regex */
		यदि (regexec(&regex, sb.buf, 0, रिक्त, 0)) {
			ret = 0;
			गच्छ समाप्तम्;
		}
	}

समाप्तम्:
	regfree(&regex);
	strbuf_release(&sb);
	प्रत्यागम ret;
}

/**
 * Attempts to detect the patch_format of the patches contained in `paths`,
 * returning the PATCH_FORMAT_* enum value. Returns PATCH_FORMAT_UNKNOWN if
 * detection fails.
 */
स्थिर पूर्णाङ्क detect_patch_format(ध्रुव वर्ण **paths)
{
	गणना patch_format ret = PATCH_FORMAT_UNKNOWN;
	संरचना strbuf l1 = STRBUF_INIT;
	संरचना strbuf l2 = STRBUF_INIT;
	संरचना strbuf l3 = STRBUF_INIT;
	FILE *fp;

	/*
	 * We default to mbox format if input is from stdin and for directories
	 */
	यदि (!*paths || !वाक्यं_तुलय(*paths, "-") || is_directory(*paths))
		प्रत्यागम PATCH_FORMAT_MBOX;

	/*
	 * Otherwise, check the first few lines of the first patch, starting
	 * from the first non-blank line, to try to detect its format.
	 */

	fp = xfopen(*paths, "r");

	यावत् (!strbuf_getline(&l1, fp)) {
		यदि (l1.len)
			भङ्ग;
	}

	यदि (starts_with(l1.buf, "From ") || starts_with(l1.buf, "From: ")) {
		ret = PATCH_FORMAT_MBOX;
		गच्छ समाप्तम्;
	}

	यदि (starts_with(l1.buf, "# This series applies on GIT commit")) {
		ret = PATCH_FORMAT_STGIT_SERIES;
		गच्छ समाप्तम्;
	}

	यदि (!वाक्यं_तुलय(l1.buf, "# HG changeset patch")) {
		ret = PATCH_FORMAT_HG;
		गच्छ समाप्तम्;
	}

	strbuf_getline(&l2, fp);
	strbuf_getline(&l3, fp);

	/*
	 * If the second line is empty and the third is a From, Author or Date
	 * entry, this is likely an StGit patch.
	 */
	यदि (l1.len && !l2.len &&
		(starts_with(l3.buf, "From:") ||
		 starts_with(l3.buf, "Author:") ||
		 starts_with(l3.buf, "Date:"))) {
		ret = PATCH_FORMAT_STGIT;
		गच्छ समाप्तम्;
	}

	यदि (l1.len && is_mail(fp)) {
		ret = PATCH_FORMAT_MBOX;
		गच्छ समाप्तम्;
	}

समाप्तम्:
	fclose(fp);
	strbuf_release(&l1);
	strbuf_release(&l2);
	strbuf_release(&l3);
	प्रत्यागम ret;
}

/**
 * Splits out individual email patches from `paths`, where each path is either
 * a mbox file or a Maildir. Returns 0 on success, -1 on failure.
 */
स्थिर पूर्णाङ्क विभज_mail_mbox(संरचना am_state *state, ध्रुव वर्ण **paths,
				पूर्णाङ्क keep_cr, पूर्णाङ्क mboxrd)
{
	संरचना child_process cp = CHILD_PROCESS_INIT;
	संरचना strbuf चरमः = STRBUF_INIT;
	पूर्णाङ्क ret;

	cp.git_cmd = 1;
	strvec_प्रक्षिप(&cp.तर्काः, "mailsplit");
	strvec_pushf(&cp.तर्काः, "-d%d", state->prec);
	strvec_pushf(&cp.तर्काः, "-o%s", state->dir);
	strvec_प्रक्षिप(&cp.तर्काः, "-b");
	यदि (keep_cr)
		strvec_प्रक्षिप(&cp.तर्काः, "--keep-cr");
	यदि (mboxrd)
		strvec_प्रक्षिप(&cp.तर्काः, "--mboxrd");
	strvec_प्रक्षिप(&cp.तर्काः, "--");
	strvec_pushv(&cp.तर्काः, paths);

	ret = capture_command(&cp, &चरमः, 8);
	यदि (ret)
		गच्छ निर्गच्छ;

	state->cur = 1;
	state->चरमः = strtol(चरमः.buf, रिक्त, 10);

निर्गच्छ:
	strbuf_release(&चरमः);
	प्रत्यागम ret ? -1 : 0;
}

/**
 * Callback signature for split_mail_conv(). The foreign patch should be
 * read from `in`, and the converted patch (in RFC2822 mail format) should be
 * written to `out`. Return 0 on success, or -1 on failure.
 */
प्रकारनिर्देश पूर्णाङ्क (*mail_conv_fn)(FILE *बहिर्गमनम्, FILE *अन्तर्, पूर्णाङ्क keep_cr);

/**
 * Calls `fn` for each file in `paths` to convert the foreign patch to the
 * RFC2822 mail format suitable for parsing with git-mailinfo.
 *
 * Returns 0 on success, -1 on failure.
 */
स्थिर पूर्णाङ्क विभज_mail_conv(mail_conv_fn fn, संरचना am_state *state,
			ध्रुव वर्ण **paths, पूर्णाङ्क keep_cr)
{
	स्थिर ध्रुव वर्ण *stdin_only[] = {"-", रिक्त};
	पूर्णाङ्क i;

	यदि (!*paths)
		paths = stdin_only;

	प्रति (i = 0; *paths; paths++, i++) {
		FILE *अन्तर्, *बहिर्गमनम्;
		ध्रुव वर्ण *mail;
		पूर्णाङ्क ret;

		यदि (!वाक्यं_तुलय(*paths, "-"))
			अन्तर् = stdin;
		अन्यथा
			अन्तर् = fopen(*paths, "r");

		यदि (!अन्तर्)
			प्रत्यागम error_errno(_("could not open '%s' for reading"),
					   *paths);

		mail = mkpath("%s/%0*d", state->dir, state->prec, i + 1);

		बहिर्गमनम् = fopen(mail, "w");
		यदि (!बहिर्गमनम्) {
			यदि (अन्तर् != stdin)
				fclose(अन्तर्);
			प्रत्यागम error_errno(_("could not open '%s' for writing"),
					   mail);
		}

		ret = fn(बहिर्गमनम्, अन्तर्, keep_cr);

		fclose(बहिर्गमनम्);
		यदि (अन्तर् != stdin)
			fclose(अन्तर्);

		यदि (ret)
			प्रत्यागम error(_("could not parse patch '%s'"), *paths);
	}

	state->cur = 1;
	state->चरमः = i;
	प्रत्यागम 0;
}

/**
 * A split_mail_conv() callback that converts an StGit patch to an RFC2822
 * message suitable for parsing with git-mailinfo.
 */
स्थिर पूर्णाङ्क stgit_patch_गन्तव्यम्_mail(FILE *बहिर्गमनम्, FILE *अन्तर्, पूर्णाङ्क keep_cr अप्रयुक्तम्)
{
	संरचना strbuf sb = STRBUF_INIT;
	पूर्णाङ्क subject_printed = 0;

	यावत् (!strbuf_getline_lf(&sb, अन्तर्)) {
		ध्रुव वर्ण *str;

		यदि (str_isspace(sb.buf))
			अनुवर्त;
		अन्यथा यदि (skip_उपसर्गः(sb.buf, "Author:", &str))
			fprintf(बहिर्गमनम्, "From:%s\n", str);
		अन्यथा यदि (starts_with(sb.buf, "From") || starts_with(sb.buf, "Date"))
			fprintf(बहिर्गमनम्, "%s\n", sb.buf);
		अन्यथा यदि (!subject_printed) {
			fprintf(बहिर्गमनम्, "Subject: %s\n", sb.buf);
			subject_printed = 1;
		} अन्यथा {
			fprintf(बहिर्गमनम्, "\n%s\n", sb.buf);
			भङ्ग;
		}
	}

	strbuf_reset(&sb);
	यावत् (strbuf_fread(&sb, 8192, अन्तर्) > 0) {
		fwrite(sb.buf, 1, sb.len, बहिर्गमनम्);
		strbuf_reset(&sb);
	}

	strbuf_release(&sb);
	प्रत्यागम 0;
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
स्थिर पूर्णाङ्क विभज_mail_stgit_series(संरचना am_state *state, ध्रुव वर्ण **paths,
					पूर्णाङ्क keep_cr)
{
	ध्रुव वर्ण *series_dir;
	वर्ण *series_dir_buf;
	FILE *fp;
	संरचना strvec patches = STRVEC_INIT;
	संरचना strbuf sb = STRBUF_INIT;
	पूर्णाङ्क ret;

	यदि (!paths[0] || paths[1])
		प्रत्यागम error(_("Only one StGIT patch series can be applied at once"));

	series_dir_buf = xstrdup(*paths);
	series_dir = dirname(series_dir_buf);

	fp = fopen(*paths, "r");
	यदि (!fp) {
		मुक्त(series_dir_buf);
		प्रत्यागम error_errno(_("could not open '%s' for reading"), *paths);
	}

	यावत् (!strbuf_getline_lf(&sb, fp)) {
		यदि (*sb.buf == '#')
			अनुवर्त; /* skip comment lines */

		strvec_प्रक्षिप(&patches, mkpath("%s/%s", series_dir, sb.buf));
	}

	fclose(fp);
	strbuf_release(&sb);
	मुक्त(series_dir_buf);

	ret = विभज_mail_conv(stgit_patch_गन्तव्यम्_mail, state, patches.v, keep_cr);

	strvec_clear(&patches);
	प्रत्यागम ret;
}

/**
 * A split_patches_conv() callback that converts a mercurial patch to a RFC2822
 * message suitable for parsing with git-mailinfo.
 */
स्थिर पूर्णाङ्क hg_patch_गन्तव्यम्_mail(FILE *बहिर्गमनम्, FILE *अन्तर्, पूर्णाङ्क keep_cr अप्रयुक्तम्)
{
	संरचना strbuf sb = STRBUF_INIT;
	पूर्णाङ्क rc = 0;

	यावत् (!strbuf_getline_lf(&sb, अन्तर्)) {
		ध्रुव वर्ण *str;

		यदि (skip_उपसर्गः(sb.buf, "# User ", &str))
			fprintf(बहिर्गमनम्, "From: %s\n", str);
		अन्यथा यदि (skip_उपसर्गः(sb.buf, "# Date ", &str)) {
			timestamp_t timestamp;
			दीर्घ tz, tz2;
			वर्ण *अन्तः;

			errno = 0;
			timestamp = parse_timestamp(str, &अन्तः, 10);
			यदि (errno) {
				rc = error(_("invalid timestamp"));
				गच्छ निर्गच्छ;
			}

			यदि (!skip_उपसर्गः(अन्तः, " ", &str)) {
				rc = error(_("invalid Date line"));
				गच्छ निर्गच्छ;
			}

			errno = 0;
			tz = strtol(str, &अन्तः, 10);
			यदि (errno) {
				rc = error(_("invalid timezone offset"));
				गच्छ निर्गच्छ;
			}

			यदि (*अन्तः) {
				rc = error(_("invalid Date line"));
				गच्छ निर्गच्छ;
			}

			/*
			 * mercurial's timezone is in seconds west of UTC,
			 * however git's timezone is in hours + minutes east of
			 * UTC. Convert it.
			 */
			tz2 = labs(tz) / 3600 * 100 + labs(tz) % 3600 / 60;
			यदि (tz > 0)
				tz2 = -tz2;

			fprintf(बहिर्गमनम्, "Date: %s\n", show_date(timestamp, tz2, DATE_बहुलकम्(RFC2822)));
		} अन्यथा यदि (starts_with(sb.buf, "# ")) {
			अनुवर्त;
		} अन्यथा {
			fprintf(बहिर्गमनम्, "\n%s\n", sb.buf);
			भङ्ग;
		}
	}

	strbuf_reset(&sb);
	यावत् (strbuf_fread(&sb, 8192, अन्तर्) > 0) {
		fwrite(sb.buf, 1, sb.len, बहिर्गमनम्);
		strbuf_reset(&sb);
	}
निर्गच्छ:
	strbuf_release(&sb);
	प्रत्यागम rc;
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
स्थिर पूर्णाङ्क विभज_mail(संरचना am_state *state, गणना patch_format patch_format,
			ध्रुव वर्ण **paths, पूर्णाङ्क keep_cr)
{
	यदि (keep_cr < 0) {
		keep_cr = 0;
		repo_config_get_bool(the_repository, "am.keepcr", &keep_cr);
	}

	विकल्प (patch_format) {
	पक्ष PATCH_FORMAT_MBOX:
		प्रत्यागम विभज_mail_mbox(state, paths, keep_cr, 0);
	पक्ष PATCH_FORMAT_STGIT:
		प्रत्यागम विभज_mail_conv(stgit_patch_गन्तव्यम्_mail, state, paths, keep_cr);
	पक्ष PATCH_FORMAT_STGIT_SERIES:
		प्रत्यागम विभज_mail_stgit_series(state, paths, keep_cr);
	पक्ष PATCH_FORMAT_HG:
		प्रत्यागम विभज_mail_conv(hg_patch_गन्तव्यम्_mail, state, paths, keep_cr);
	पक्ष PATCH_FORMAT_MBOXRD:
		प्रत्यागम विभज_mail_mbox(state, paths, keep_cr, 1);
	स्वभाव:
		BUG("invalid patch_format");
	}
	प्रत्यागम -1;
}

/**
 * Setup a new am session for applying patches
 */
स्थिर शून्य am_सज्जता(संरचना am_state *state, गणना patch_format patch_format,
			ध्रुव वर्ण **paths, पूर्णाङ्क keep_cr)
{
	संरचना object_id curr_शीर्ष;
	ध्रुव वर्ण *str;
	संरचना strbuf sb = STRBUF_INIT;

	यदि (!patch_format)
		patch_format = detect_patch_format(paths);

	यदि (!patch_format) {
		fprintf_ln(stderr, _("Patch format detection failed."));
		die(रिक्त);
	}

	यदि (mkdir(state->dir, 0777) < 0 && errno != EEXIST)
		die_errno(_("failed to create directory '%s'"), state->dir);
	refs_delete_ref(get_main_ref_store(the_repository), रिक्त,
			"REBASE_HEAD", रिक्त, REF_NO_DEREF);

	यदि (विभज_mail(state, patch_format, paths, keep_cr) < 0) {
		am_destroy(state);
		die(_("Failed to split patches."));
	}

	यदि (state->rebasing)
		state->threeway = 1;

	write_state_bool(state, "threeway", state->threeway);
	write_state_bool(state, "quiet", state->quiet);
	write_state_bool(state, "sign", state->signoff);
	write_state_bool(state, "utf8", state->utf8);

	यदि (state->allow_rerere_autoupdate)
		write_state_bool(state, "rerere-autoupdate",
			 state->allow_rerere_autoupdate == RERERE_AUTOUPDATE);

	विकल्प (state->keep) {
	पक्ष KEEP_FALSE:
		str = "f";
		भङ्ग;
	पक्ष KEEP_TRUE:
		str = "t";
		भङ्ग;
	पक्ष KEEP_NON_PATCH:
		str = "b";
		भङ्ग;
	स्वभाव:
		BUG("invalid value for state->keep");
	}

	write_state_text(state, "keep", str);
	write_state_bool(state, "messageid", state->सन्देशः_id);

	विकल्प (state->scissors) {
	पक्ष SCISSORS_UNSET:
		str = "";
		भङ्ग;
	पक्ष SCISSORS_FALSE:
		str = "f";
		भङ्ग;
	पक्ष SCISSORS_TRUE:
		str = "t";
		भङ्ग;
	स्वभाव:
		BUG("invalid value for state->scissors");
	}
	write_state_text(state, "scissors", str);

	विकल्प (state->quoted_cr) {
	पक्ष quoted_cr_unset:
		str = "";
		भङ्ग;
	पक्ष quoted_cr_nowarn:
		str = "nowarn";
		भङ्ग;
	पक्ष quoted_cr_warn:
		str = "warn";
		भङ्ग;
	पक्ष quoted_cr_strip:
		str = "strip";
		भङ्ग;
	स्वभाव:
		BUG("invalid value for state->quoted_cr");
	}
	write_state_text(state, "quoted-cr", str);

	sq_quote_argv(&sb, state->git_apply_opts.v);
	write_state_text(state, "apply-opt", sb.buf);

	यदि (state->rebasing)
		write_state_text(state, "rebasing", "");
	अन्यथा
		write_state_text(state, "applying", "");

	यदि (!repo_get_oid(the_repository, "HEAD", &curr_शीर्ष)) {
		write_state_text(state, "abort-safety", oid_गन्तव्यम्_hex(&curr_शीर्ष));
		यदि (!state->rebasing)
			refs_update_ref(get_main_ref_store(the_repository),
					"am", "ORIG_HEAD", &curr_शीर्ष, रिक्त,
					0,
					UPDATE_REFS_DIE_ON_ERR);
	} अन्यथा {
		write_state_text(state, "abort-safety", "");
		यदि (!state->rebasing)
			refs_delete_ref(get_main_ref_store(the_repository),
					रिक्त, "ORIG_HEAD", रिक्त, 0);
	}

	/*
	 * NOTE: Since the "next" and "last" files determine if an am_state
	 * session is in progress, they should be written last.
	 */

	write_state_गणनम्(state, "next", state->cur);
	write_state_गणनम्(state, "last", state->चरमः);

	strbuf_release(&sb);
}

/**
 * Increments the patch pointer, and cleans am_state for the application of the
 * next patch.
 */
स्थिर शून्य am_अग्र(संरचना am_state *state)
{
	संरचना object_id शीर्ष;

	FREE_AND_NULL(state->author_name);
	FREE_AND_NULL(state->author_email);
	FREE_AND_NULL(state->author_date);
	FREE_AND_NULL(state->सन्देशः);
	state->सन्देशः_len = 0;

	unlink(am_मार्गः(state, "author-script"));
	unlink(am_मार्गः(state, "final-commit"));

	oidclr(&state->orig_commit, the_repository->hash_algo);
	unlink(am_मार्गः(state, "original-commit"));
	refs_delete_ref(get_main_ref_store(the_repository), रिक्त,
			"REBASE_HEAD", रिक्त, REF_NO_DEREF);

	यदि (!repo_get_oid(the_repository, "HEAD", &शीर्ष))
		write_state_text(state, "abort-safety", oid_गन्तव्यम्_hex(&शीर्ष));
	अन्यथा
		write_state_text(state, "abort-safety", "");

	state->cur++;
	write_state_गणनम्(state, "next", state->cur);
}

/**
 * Returns the filename of the current patch email.
 */
स्थिर ध्रुव वर्ण *msgnum(ध्रुव संरचना am_state *state)
{
	स्थिर संरचना strbuf sb = STRBUF_INIT;

	strbuf_reset(&sb);
	strbuf_addf(&sb, "%0*d", state->prec, state->cur);

	प्रत्यागम sb.buf;
}

/**
 * Dies with a user-friendly message on how to proceed after resolving the
 * problem. This message can be overridden with state->resolvemsg.
 */
स्थिर शून्य NORETURN die_user_resolve(ध्रुव संरचना am_state *state)
{
	यदि (state->resolvemsg) {
		advise_if_enabled(ADVICE_MERGE_CONFLICT, "%s", state->resolvemsg);
	} अन्यथा {
		ध्रुव वर्ण *cmdline = state->interactive ? "git am -i" : "git am";
		संरचना strbuf sb = STRBUF_INIT;

		strbuf_addf(&sb, _("When you have resolved this problem, run \"%s --continue\".\n"), cmdline);
		strbuf_addf(&sb, _("If you prefer to skip this patch, run \"%s --skip\" instead.\n"), cmdline);

		यदि (advice_enabled(ADVICE_AM_WORK_DIR) &&
		    is_रिक्तानि_or_missing_file(am_मार्गः(state, "patch")) &&
		    !repo_सूचकांक_has_changes(the_repository, रिक्त, रिक्त))
			strbuf_addf(&sb, _("To record the empty patch as an empty commit, run \"%s --allow-empty\".\n"), cmdline);

		strbuf_addf(&sb, _("To restore the original branch and stop patching, run \"%s --abort\"."), cmdline);

		advise_if_enabled(ADVICE_MERGE_CONFLICT, "%s", sb.buf);
		strbuf_release(&sb);
	}

	die(रिक्त);
}

/**
 * Appends signoff to the "msg" field of the am_state.
 */
स्थिर शून्य am_append_signoff(संरचना am_state *state)
{
	संरचना strbuf sb = STRBUF_INIT;

	strbuf_attach(&sb, state->सन्देशः, state->सन्देशः_len, state->सन्देशः_len + 1);
	append_signoff(&sb, 0, 0);
	state->सन्देशः = strbuf_detach(&sb, &state->सन्देशः_len);
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
स्थिर पूर्णाङ्क parse_mail(संरचना am_state *state, ध्रुव वर्ण *mail)
{
	FILE *fp;
	संरचना strbuf sb = STRBUF_INIT;
	संरचना strbuf सन्देशः = STRBUF_INIT;
	संरचना strbuf author_name = STRBUF_INIT;
	संरचना strbuf author_date = STRBUF_INIT;
	संरचना strbuf author_email = STRBUF_INIT;
	पूर्णाङ्क ret = 0;
	संरचना mailinfo mi;

	सज्जता_mailinfo(the_repository, &mi);

	यदि (state->utf8)
		mi.metainfo_charset = get_commit_output_encoding();
	अन्यथा
		mi.metainfo_charset = रिक्त;

	विकल्प (state->keep) {
	पक्ष KEEP_FALSE:
		भङ्ग;
	पक्ष KEEP_TRUE:
		mi.keep_subject = 1;
		भङ्ग;
	पक्ष KEEP_NON_PATCH:
		mi.keep_non_patch_brackets_अन्तर्_subject = 1;
		भङ्ग;
	स्वभाव:
		BUG("invalid value for state->keep");
	}

	यदि (state->सन्देशः_id)
		mi.सङ्कलय_सन्देशः_id = 1;

	विकल्प (state->scissors) {
	पक्ष SCISSORS_UNSET:
		भङ्ग;
	पक्ष SCISSORS_FALSE:
		mi.use_scissors = 0;
		भङ्ग;
	पक्ष SCISSORS_TRUE:
		mi.use_scissors = 1;
		भङ्ग;
	स्वभाव:
		BUG("invalid value for state->scissors");
	}

	विकल्प (state->quoted_cr) {
	पक्ष quoted_cr_unset:
		भङ्ग;
	पक्ष quoted_cr_nowarn:
	पक्ष quoted_cr_warn:
	पक्ष quoted_cr_strip:
		mi.quoted_cr = state->quoted_cr;
		भङ्ग;
	स्वभाव:
		BUG("invalid value for state->quoted_cr");
	}

	mi.निवेशः = xfopen(mail, "r");
	mi.output = xfopen(am_मार्गः(state, "info"), "w");
	यदि (mailinfo(&mi, am_मार्गः(state, "msg"), am_मार्गः(state, "patch")))
		die("could not parse patch");

	fclose(mi.निवेशः);
	fclose(mi.output);

	यदि (mi.format_flowed)
		warning(_("Patch sent with format=flowed; "
			  "space at the end of lines might be lost."));

	/* Extract message and author information */
	fp = xfopen(am_मार्गः(state, "info"), "r");
	यावत् (!strbuf_getline_lf(&sb, fp)) {
		ध्रुव वर्ण *x;

		यदि (skip_उपसर्गः(sb.buf, "Subject: ", &x)) {
			यदि (सन्देशः.len)
				strbuf_addch(&सन्देशः, '\n');
			strbuf_addstr(&सन्देशः, x);
		} अन्यथा यदि (skip_उपसर्गः(sb.buf, "Author: ", &x))
			strbuf_addstr(&author_name, x);
		अन्यथा यदि (skip_उपसर्गः(sb.buf, "Email: ", &x))
			strbuf_addstr(&author_email, x);
		अन्यथा यदि (skip_उपसर्गः(sb.buf, "Date: ", &x))
			strbuf_addstr(&author_date, x);
	}
	fclose(fp);

	/* Skip pine's internal folder data */
	यदि (!वाक्यं_तुलय(author_name.buf, "Mail System Internal Data")) {
		ret = 1;
		गच्छ finish;
	}

	strbuf_addstr(&सन्देशः, "\n\n");
	strbuf_addbuf(&सन्देशः, &mi.log_सन्देशः);
	strbuf_stripspace(&सन्देशः, रिक्त);

	प्रतिज्ञा(!state->author_name);
	state->author_name = strbuf_detach(&author_name, रिक्त);

	प्रतिज्ञा(!state->author_email);
	state->author_email = strbuf_detach(&author_email, रिक्त);

	प्रतिज्ञा(!state->author_date);
	state->author_date = strbuf_detach(&author_date, रिक्त);

	प्रतिज्ञा(!state->सन्देशः);
	state->सन्देशः = strbuf_detach(&सन्देशः, &state->सन्देशः_len);

finish:
	strbuf_release(&सन्देशः);
	strbuf_release(&author_date);
	strbuf_release(&author_email);
	strbuf_release(&author_name);
	strbuf_release(&sb);
	clear_mailinfo(&mi);
	प्रत्यागम ret;
}

/**
 * Sets commit_id to the commit hash where the mail was generated from.
 * Returns 0 on success, -1 on failure.
 */
स्थिर पूर्णाङ्क get_mail_commit_oid(संरचना object_id *commit_id, ध्रुव वर्ण *mail)
{
	संरचना strbuf sb = STRBUF_INIT;
	FILE *fp = xfopen(mail, "r");
	ध्रुव वर्ण *x;
	पूर्णाङ्क ret = 0;

	यदि (strbuf_getline_lf(&sb, fp) ||
	    !skip_उपसर्गः(sb.buf, "From ", &x) ||
	    get_oid_hex(x, commit_id) < 0)
		ret = -1;

	strbuf_release(&sb);
	fclose(fp);
	प्रत्यागम ret;
}

/**
 * Sets state->msg, state->author_name, state->author_email, state->author_date
 * to the commit's respective info.
 */
स्थिर शून्य get_commit_info(संरचना am_state *state, संरचना commit *commit)
{
	ध्रुव वर्ण *बफर, *ident_पङ्क्तिः, *सन्देशः;
	आकार_t ident_len;
	संरचना ident_विभज id;

	बफर = repo_logmsg_reencode(the_repository, commit, रिक्त,
				      get_commit_output_encoding());

	ident_पङ्क्तिः = find_commit_शीर्षकम्(बफर, "author", &ident_len);
	यदि (!ident_पङ्क्तिः)
		die(_("missing author line in commit %s"),
		      oid_गन्तव्यम्_hex(&commit->object.oid));
	यदि (विभज_ident_पङ्क्तिः(&id, ident_पङ्क्तिः, ident_len) < 0)
		die(_("invalid ident line: %.*s"), (पूर्णाङ्क)ident_len, ident_पङ्क्तिः);

	प्रतिज्ञा(!state->author_name);
	यदि (id.name_begin)
		state->author_name =
			xmemdupz(id.name_begin, id.name_अन्तः - id.name_begin);
	अन्यथा
		state->author_name = xstrdup("");

	प्रतिज्ञा(!state->author_email);
	यदि (id.mail_begin)
		state->author_email =
			xmemdupz(id.mail_begin, id.mail_अन्तः - id.mail_begin);
	अन्यथा
		state->author_email = xstrdup("");

	प्रतिज्ञा(!state->author_date);
	state->author_date = xstrdup(show_ident_date(&id, DATE_बहुलकम्(प्रसामान्यम्)));

	प्रतिज्ञा(!state->सन्देशः);
	सन्देशः = वाक्ये_अन्विष(बफर, "\n\n");
	यदि (!सन्देशः)
		die(_("unable to parse commit %s"), oid_गन्तव्यम्_hex(&commit->object.oid));
	state->सन्देशः = xstrdup(सन्देशः + 2);
	state->सन्देशः_len = वाक्यदैर्घ्यम्(state->सन्देशः);
	repo_unuse_commit_बफर(the_repository, commit, बफर);
}

/**
 * Writes `commit` as a patch to the state directory's "patch" file.
 */
स्थिर शून्य write_commit_patch(ध्रुव संरचना am_state *state, संरचना commit *commit)
{
	संरचना rev_info rev_info;
	FILE *fp;

	fp = xfopen(am_मार्गः(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, रिक्त);
	rev_info.diff = 1;
	rev_info.abbrev = 0;
	rev_info.disable_stdin = 1;
	rev_info.show_मूलम्_diff = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.flags.द्विमान = 1;
	rev_info.diffopt.flags.पूर्णम्_सूचकांक = 1;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	सङ्कलय_pending_object(&rev_info, &commit->object, "");
	diff_सज्जता_समाप्तम्(&rev_info.diffopt);
	log_वृक्षः_commit(&rev_info, commit);
	release_revisions(&rev_info);
}

/**
 * Writes the diff of the index against HEAD as a patch to the state
 * directory's "patch" file.
 */
स्थिर शून्य write_सूचकांक_patch(ध्रुव संरचना am_state *state)
{
	संरचना वृक्षः *वृक्षः;
	संरचना object_id शीर्ष;
	संरचना rev_info rev_info;
	FILE *fp;

	यदि (!repo_get_oid(the_repository, "HEAD", &शीर्ष)) {
		संरचना commit *commit = lookup_commit_or_die(&शीर्ष, "HEAD");
		वृक्षः = repo_get_commit_वृक्षः(the_repository, commit);
	} अन्यथा
		वृक्षः = lookup_वृक्षः(the_repository,
				   the_repository->hash_algo->रिक्तानि_वृक्षः);

	fp = xfopen(am_मार्गः(state, "patch"), "w");
	repo_init_revisions(the_repository, &rev_info, रिक्त);
	rev_info.diff = 1;
	rev_info.disable_stdin = 1;
	rev_info.no_commit_id = 1;
	rev_info.diffopt.output_format = DIFF_FORMAT_PATCH;
	rev_info.diffopt.use_color = GIT_COLOR_NEVER;
	rev_info.diffopt.file = fp;
	rev_info.diffopt.close_file = 1;
	सङ्कलय_pending_object(&rev_info, &वृक्षः->object, "");
	diff_सज्जता_समाप्तम्(&rev_info.diffopt);
	सततम्_diff_सूचकांक(&rev_info, DIFF_सूचकांक_CACHED);
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
स्थिर पूर्णाङ्क parse_mail_rebase(संरचना am_state *state, ध्रुव वर्ण *mail)
{
	संरचना commit *commit;
	संरचना object_id commit_oid;

	यदि (get_mail_commit_oid(&commit_oid, mail) < 0)
		die(_("could not parse %s"), mail);

	commit = lookup_commit_or_die(&commit_oid, mail);

	get_commit_info(state, commit);

	write_commit_patch(state, commit);

	oidcpy(&state->orig_commit, &commit_oid);
	write_state_text(state, "original-commit", oid_गन्तव्यम्_hex(&commit_oid));
	refs_update_ref(get_main_ref_store(the_repository), "am",
			"REBASE_HEAD", &commit_oid,
			रिक्त, REF_NO_DEREF, UPDATE_REFS_DIE_ON_ERR);

	प्रत्यागम 0;
}

/**
 * Applies current patch with git-apply. Returns 0 on success, -1 otherwise. If
 * `index_file` is not NULL, the patch will be applied to that index.
 */
स्थिर पूर्णाङ्क सततम्_apply(ध्रुव संरचना am_state *state, ध्रुव वर्ण *सूचकांक_file)
{
	संरचना strvec apply_paths = STRVEC_INIT;
	संरचना strvec apply_opts = STRVEC_INIT;
	संरचना apply_state apply_state;
	पूर्णाङ्क res, opts_वामः;
	पूर्णाङ्क force_apply = 0;
	पूर्णाङ्क options = 0;
	ध्रुव वर्ण **apply_argv;

	यदि (init_apply_state(&apply_state, the_repository, रिक्त))
		BUG("init_apply_state() failed");

	strvec_प्रक्षिप(&apply_opts, "apply");
	strvec_pushv(&apply_opts, state->git_apply_opts.v);

	/*
	 * Build a copy that apply_parse_options() can rearrange.
	 * apply_opts.v keeps referencing the allocated strings for
	 * strvec_clear() to release.
	 */
	DUP_ARRAY(apply_argv, apply_opts.v, apply_opts.nr);

	opts_वामः = apply_parse_options(apply_opts.nr, apply_argv,
					&apply_state, &force_apply, &options,
					रिक्त);

	यदि (opts_वामः != 0)
		die("unknown option passed through to git apply");

	यदि (सूचकांक_file) {
		apply_state.सूचकांक_file = सूचकांक_file;
		apply_state.cached = 1;
	} अन्यथा
		apply_state.check_सूचकांक = 1;

	/*
	 * If we are allowed to fall back on 3-way merge, don't give false
	 * errors during the initial attempt.
	 */
	यदि (state->threeway && !सूचकांक_file)
		apply_state.apply_verbosity = verbosity_silent;

	यदि (check_apply_state(&apply_state, force_apply))
		BUG("check_apply_state() failed");

	strvec_प्रक्षिप(&apply_paths, am_मार्गः(state, "patch"));

	res = apply_all_patches(&apply_state, apply_paths.nr, apply_paths.v, options);

	strvec_clear(&apply_paths);
	strvec_clear(&apply_opts);
	clear_apply_state(&apply_state);
	मुक्त(apply_argv);

	यदि (res)
		प्रत्यागम res;

	यदि (सूचकांक_file) {
		/* Reload index as apply_all_patches() will have modified it. */
		discard_सूचकांक(the_repository->सूचकांक);
		read_सूचकांक_from(the_repository->सूचकांक, सूचकांक_file,
				repo_get_git_dir(the_repository));
	}

	प्रत्यागम 0;
}

/**
 * Builds an index that contains just the blobs needed for a 3way merge.
 */
स्थिर पूर्णाङ्क build_fake_ancestor(ध्रुव संरचना am_state *state, ध्रुव वर्ण *सूचकांक_file)
{
	संरचना child_process cp = CHILD_PROCESS_INIT;

	cp.git_cmd = 1;
	strvec_प्रक्षिप(&cp.तर्काः, "apply");
	strvec_pushv(&cp.तर्काः, state->git_apply_opts.v);
	strvec_pushf(&cp.तर्काः, "--build-fake-ancestor=%s", सूचकांक_file);
	strvec_प्रक्षिप(&cp.तर्काः, am_मार्गः(state, "patch"));

	यदि (सततम्_command(&cp))
		प्रत्यागम -1;

	प्रत्यागम 0;
}

/**
 * Attempt a threeway merge, using index_path as the temporary index.
 */
स्थिर पूर्णाङ्क fall_पृष्ठभागः_threeway(ध्रुव संरचना am_state *state, ध्रुव वर्ण *सूचकांक_मार्गः)
{
	संरचना object_id their_वृक्षः, our_वृक्षः;
	संरचना object_id bases[1] = { 0 };
	संरचना merge_options o;
	संरचना commit *परिणाम;
	वर्ण *their_वृक्षः_name;

	यदि (repo_get_oid(the_repository, "HEAD", &our_वृक्षः) < 0)
		oidcpy(&our_वृक्षः, the_hash_algo->रिक्तानि_वृक्षः);

	यदि (build_fake_ancestor(state, सूचकांक_मार्गः))
		प्रत्यागम error("could not build fake ancestor");

	discard_सूचकांक(the_repository->सूचकांक);
	read_सूचकांक_from(the_repository->सूचकांक, सूचकांक_मार्गः, repo_get_git_dir(the_repository));

	यदि (write_सूचकांक_as_वृक्षः(&bases[0], the_repository->सूचकांक, सूचकांक_मार्गः, 0, रिक्त))
		प्रत्यागम error(_("Repository lacks necessary blobs to fall back on 3-way merge."));

	say(state, stdout, _("Using index info to reconstruct a base tree..."));

	यदि (!state->quiet) {
		/*
		 * List paths that needed 3-way fallback, so that the user can
		 * review them with extra care to spot mismerges.
		 */
		संरचना rev_info rev_info;

		repo_init_revisions(the_repository, &rev_info, रिक्त);
		rev_info.diffopt.output_format = DIFF_FORMAT_NAME_स्थितिः;
		rev_info.diffopt.filter |= diff_filter_bit('A');
		rev_info.diffopt.filter |= diff_filter_bit('M');
		सङ्कलय_pending_oid(&rev_info, "HEAD", &our_वृक्षः, 0);
		diff_सज्जता_समाप्तम्(&rev_info.diffopt);
		सततम्_diff_सूचकांक(&rev_info, DIFF_सूचकांक_CACHED);
		release_revisions(&rev_info);
	}

	यदि (सततम्_apply(state, सूचकांक_मार्गः))
		प्रत्यागम error(_("Did you hand edit your patch?\n"
				"It does not apply to blobs recorded in its index."));

	यदि (write_सूचकांक_as_वृक्षः(&their_वृक्षः, the_repository->सूचकांक, सूचकांक_मार्गः, 0, रिक्त))
		प्रत्यागम error("could not write tree");

	say(state, stdout, _("Falling back to patching base and 3-way merge..."));

	discard_सूचकांक(the_repository->सूचकांक);
	repo_read_सूचकांक(the_repository);

	/*
	 * This is not so wrong. Depending on which base we picked, orig_tree
	 * may be wildly different from ours, but their_tree has the same set of
	 * wildly different changes in parts the patch did not touch, so
	 * recursive ends up canceling them, saying that we reverted all those
	 * changes.
	 */

	init_ui_merge_options(&o, the_repository);

	o.branch1 = "HEAD";
	their_वृक्षः_name = xstrfmt("%.*s", linelen(state->सन्देशः), state->सन्देशः);
	o.branch2 = their_वृक्षः_name;
	o.ancestor = "constructed fake ancestor";
	o.detect_directory_renames = MERGE_DIRECTORY_RENAMES_NONE;

	यदि (state->quiet)
		o.verbosity = 0;

	यदि (merge_ort_generic(&o, &our_वृक्षः, &their_वृक्षः, 1, bases, &परिणाम)) {
		repo_rerere(the_repository, state->allow_rerere_autoupdate);
		मुक्त(their_वृक्षः_name);
		प्रत्यागम error(_("Failed to merge in the changes."));
	}

	मुक्त(their_वृक्षः_name);
	प्रत्यागम 0;
}

/**
 * Commits the current index with state->msg as the commit message and
 * state->author_name, state->author_email and state->author_date as the author
 * information.
 */
स्थिर शून्य do_commit(ध्रुव संरचना am_state *state)
{
	संरचना object_id वृक्षः, parent, commit;
	ध्रुव संरचना object_id *old_oid;
	संरचना commit_list *parents = रिक्त;
	ध्रुव वर्ण *reflog_सन्देशः, *author, *committer = रिक्त;
	संरचना strbuf sb = STRBUF_INIT;

	यदि (!state->no_verify && सततम्_hooks(the_repository, "pre-applypatch"))
		निर्गच्छ(1);

	यदि (write_सूचकांक_as_वृक्षः(&वृक्षः, the_repository->सूचकांक,
				repo_get_सूचकांक_file(the_repository),
				0, रिक्त))
		die(_("git write-tree failed to write a tree"));

	यदि (!repo_get_oid_commit(the_repository, "HEAD", &parent)) {
		old_oid = &parent;
		commit_list_निवेशय(lookup_commit(the_repository, &parent),
				   &parents);
	} अन्यथा {
		old_oid = रिक्त;
		say(state, stderr, _("applying to an empty history"));
	}

	author = fmt_ident(state->author_name, state->author_email,
		WANT_AUTHOR_IDENT,
			state->ignore_date ? रिक्त : state->author_date,
			IDENT_STRICT);

	यदि (state->committer_date_is_author_date)
		committer = fmt_ident(getenv("GIT_COMMITTER_NAME"),
				      getenv("GIT_COMMITTER_EMAIL"),
				      WANT_COMMITTER_IDENT,
				      state->ignore_date ? रिक्त
							 : state->author_date,
				      IDENT_STRICT);

	यदि (commit_वृक्षः_extended(state->सन्देशः, state->सन्देशः_len, &वृक्षः, parents,
				 &commit, author, committer, state->sign_commit,
				 रिक्त))
		die(_("failed to write commit object"));

	reflog_सन्देशः = getenv("GIT_REFLOG_ACTION");
	यदि (!reflog_सन्देशः)
		reflog_सन्देशः = "am";

	strbuf_addf(&sb, "%s: %.*s", reflog_सन्देशः, linelen(state->सन्देशः),
			state->सन्देशः);

	refs_update_ref(get_main_ref_store(the_repository), sb.buf, "HEAD",
			&commit, old_oid, 0,
			UPDATE_REFS_DIE_ON_ERR);

	यदि (state->rebasing) {
		FILE *fp = xfopen(am_मार्गः(state, "rewritten"), "a");

		प्रतिज्ञा(!is_null_oid(&state->orig_commit));
		fprintf(fp, "%s ", oid_गन्तव्यम्_hex(&state->orig_commit));
		fprintf(fp, "%s\n", oid_गन्तव्यम्_hex(&commit));
		fclose(fp);
	}

	सततम्_hooks(the_repository, "post-applypatch");

	commit_list_free(parents);
	strbuf_release(&sb);
}

/**
 * Validates the am_state for resuming -- the "msg" and authorship fields must
 * be filled up.
 */
स्थिर शून्य validate_resume_state(ध्रुव संरचना am_state *state)
{
	यदि (!state->सन्देशः)
		die(_("cannot resume: %s does not exist."),
			am_मार्गः(state, "final-commit"));

	यदि (!state->author_name || !state->author_email || !state->author_date)
		die(_("cannot resume: %s does not exist."),
			am_मार्गः(state, "author-script"));
}

/**
 * Interactively prompt the user on whether the current patch should be
 * applied.
 *
 * Returns 0 if the user chooses to apply the patch, 1 if the user chooses to
 * skip it.
 */
स्थिर पूर्णाङ्क do_interactive(संरचना am_state *state)
{
	प्रतिज्ञा(state->सन्देशः);

	प्रति (;;) {
		वर्ण प्रत्युत्तरम्[64];

		पङ्क्तिं_लिख(_("Commit Body is:"));
		पङ्क्तिं_लिख("--------------------------");
		प्रारूप_लिख("%s", state->सन्देशः);
		पङ्क्तिं_लिख("--------------------------");

		/*
		 * TRANSLATORS: Make sure to include [y], [n], [e], [v] and [a]
		 * in your translation. The program will only accept English
		 * input at this point.
		 */
		प्रारूप_लिख(_("Apply? [y]es/[n]o/[e]dit/[v]iew patch/[a]ccept all: "));
		यदि (!fgets(प्रत्युत्तरम्, परिमाण(प्रत्युत्तरम्), stdin))
			die("unable to read from stdin; aborting");

		यदि (*प्रत्युत्तरम् == 'y' || *प्रत्युत्तरम् == 'Y') {
			प्रत्यागम 0;
		} अन्यथा यदि (*प्रत्युत्तरम् == 'a' || *प्रत्युत्तरम् == 'A') {
			state->interactive = 0;
			प्रत्यागम 0;
		} अन्यथा यदि (*प्रत्युत्तरम् == 'n' || *प्रत्युत्तरम् == 'N') {
			प्रत्यागम 1;
		} अन्यथा यदि (*प्रत्युत्तरम् == 'e' || *प्रत्युत्तरम् == 'E') {
			संरचना strbuf सन्देशः = STRBUF_INIT;

			यदि (!launch_editor(am_मार्गः(state, "final-commit"), &सन्देशः, रिक्त)) {
				मुक्त(state->सन्देशः);
				state->सन्देशः = strbuf_detach(&सन्देशः, &state->सन्देशः_len);
			}
			strbuf_release(&सन्देशः);
		} अन्यथा यदि (*प्रत्युत्तरम् == 'v' || *प्रत्युत्तरम् == 'V') {
			ध्रुव वर्ण *pager = git_pager(the_repository, 1);
			संरचना child_process cp = CHILD_PROCESS_INIT;

			यदि (!pager)
				pager = "cat";
			prepare_pager_तर्काः(&cp, pager);
			strvec_प्रक्षिप(&cp.तर्काः, am_मार्गः(state, "patch"));
			सततम्_command(&cp);
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
स्थिर शून्य am_सततम्(संरचना am_state *state, पूर्णाङ्क resume)
{
	संरचना strbuf sb = STRBUF_INIT;

	unlink(am_मार्गः(state, "dirtyindex"));

	यदि (repo_refresh_and_write_सूचकांक(the_repository, REFRESH_QUIET, 0, 0,
					 रिक्त, रिक्त, रिक्त) < 0)
		die(_("unable to write index file"));

	यदि (repo_सूचकांक_has_changes(the_repository, रिक्त, &sb)) {
		write_state_bool(state, "dirtyindex", 1);
		die(_("Dirty index: cannot apply patches (dirty: %s)"), sb.buf);
	}

	strbuf_release(&sb);

	यावत् (state->cur <= state->चरमः) {
		ध्रुव वर्ण *mail = am_मार्गः(state, msgnum(state));
		पूर्णाङ्क apply_स्थितिः;
		पूर्णाङ्क गन्तव्यम्_keep;

		reset_ident_date();

		यदि (!file_exists(mail))
			गच्छ अग्र;

		यदि (resume) {
			validate_resume_state(state);
		} अन्यथा {
			पूर्णाङ्क skip;

			यदि (state->rebasing)
				skip = parse_mail_rebase(state, mail);
			अन्यथा
				skip = parse_mail(state, mail);

			यदि (skip)
				गच्छ अग्र; /* mail should be skipped */

			यदि (state->signoff)
				am_append_signoff(state);

			write_author_script(state);
			write_commit_सन्देशः(state);
		}

		यदि (state->interactive && do_interactive(state))
			गच्छ अग्र;

		गन्तव्यम्_keep = 0;
		यदि (is_रिक्तानि_or_missing_file(am_मार्गः(state, "patch"))) {
			विकल्प (state->रिक्तानि_type) {
			पक्ष DROP_रिक्तानि_COMMIT:
				say(state, stdout, _("Skipping: %.*s"), linelen(state->सन्देशः), state->सन्देशः);
				गच्छ अग्र;
				भङ्ग;
			पक्ष KEEP_रिक्तानि_COMMIT:
				गन्तव्यम्_keep = 1;
				say(state, stdout, _("Creating an empty commit: %.*s"),
					linelen(state->सन्देशः), state->सन्देशः);
				भङ्ग;
			पक्ष STOP_ON_रिक्तानि_COMMIT:
				printf_ln(_("Patch is empty."));
				die_user_resolve(state);
				भङ्ग;
			}
		}

		यदि (सततम्_applypatch_सन्देशः_hook(state))
			निर्गच्छ(1);
		यदि (गन्तव्यम्_keep)
			गच्छ commit;

		say(state, stdout, _("Applying: %.*s"), linelen(state->सन्देशः), state->सन्देशः);

		apply_स्थितिः = सततम्_apply(state, रिक्त);

		यदि (apply_स्थितिः && state->threeway) {
			संरचना strbuf sb = STRBUF_INIT;

			strbuf_addstr(&sb, am_मार्गः(state, "patch-merge-index"));
			apply_स्थितिः = fall_पृष्ठभागः_threeway(state, sb.buf);
			strbuf_release(&sb);

			/*
			 * Applying the patch to an earlier tree and merging
			 * the result may have produced the same tree as ours.
			 */
			यदि (!apply_स्थितिः &&
			    !repo_सूचकांक_has_changes(the_repository, रिक्त, रिक्त)) {
				say(state, stdout, _("No changes -- Patch already applied."));
				गच्छ अग्र;
			}
		}

		यदि (apply_स्थितिः) {
			printf_ln(_("Patch failed at %s %.*s"), msgnum(state),
				linelen(state->सन्देशः), state->सन्देशः);

			यदि (advice_enabled(ADVICE_AM_WORK_DIR))
				advise(_("Use 'git am --show-current-patch=diff' to see the failed patch"));

			die_user_resolve(state);
		}

commit:
		do_commit(state);

अग्र:
		am_अग्र(state);

		यदि (resume)
			am_आरोपय(state);
		resume = 0;
	}

	यदि (!is_रिक्तानि_or_missing_file(am_मार्गः(state, "rewritten"))) {
		प्रतिज्ञा(state->rebasing);
		copy_notes_for_rebase(state);
		सततम्_post_rewrite_hook(state);
	}

	/*
	 * In rebasing mode, it's up to the caller to take care of
	 * housekeeping.
	 */
	यदि (!state->rebasing) {
		am_destroy(state);
		सततम्_auto_maintenance(the_repository, state->quiet);
	}
}

/**
 * Resume the current am session after patch application failure. The user did
 * all the hard work, and we do not have to do any patch application. Just
 * trust and commit what the user has in the index and working tree. If `allow_empty`
 * is true, commit as an empty commit when index has not changed and lacking a patch.
 */
स्थिर शून्य am_resolve(संरचना am_state *state, पूर्णाङ्क allow_रिक्तानि)
{
	validate_resume_state(state);

	say(state, stdout, _("Applying: %.*s"), linelen(state->सन्देशः), state->सन्देशः);

	यदि (!repo_सूचकांक_has_changes(the_repository, रिक्त, रिक्त)) {
		यदि (allow_रिक्तानि && is_रिक्तानि_or_missing_file(am_मार्गः(state, "patch"))) {
			printf_ln(_("No changes - recorded it as an empty commit."));
		} अन्यथा {
			printf_ln(_("No changes - did you forget to use 'git add'?\n"
				    "If there is nothing left to stage, chances are that something else\n"
				    "already introduced the same changes; you might want to skip this patch."));
			die_user_resolve(state);
		}
	}

	यदि (unmerged_सूचकांक(the_repository->सूचकांक)) {
		printf_ln(_("You still have unmerged paths in your index.\n"
			"You should 'git add' each file with resolved conflicts to mark them as such.\n"
			"You might run `git rm` on a file to accept \"deleted by them\" for it."));
		die_user_resolve(state);
	}

	यदि (state->interactive) {
		write_सूचकांक_patch(state);
		यदि (do_interactive(state))
			गच्छ अग्र;
	}

	repo_rerere(the_repository, 0);

	do_commit(state);

अग्र:
	am_अग्र(state);
	am_आरोपय(state);
	am_सततम्(state, 0);
}

/**
 * Performs a checkout fast-forward from `head` to `remote`. If `reset` is
 * true, any unmerged entries will be discarded. Returns 0 on success, -1 on
 * failure.
 */
स्थिर पूर्णाङ्क fast_forward_गन्तव्यम्(संरचना वृक्षः *शीर्ष, संरचना वृक्षः *remote, पूर्णाङ्क reset)
{
	संरचना तालकम्_file तालकम्_file = तालकम्_INIT;
	संरचना unpack_trees_options opts;
	संरचना वृक्षः_desc t[2];

	यदि (repo_parse_वृक्षः(the_repository, शीर्ष) || repo_parse_वृक्षः(the_repository, remote))
		प्रत्यागम -1;

	repo_hold_locked_सूचकांक(the_repository, &तालकम्_file, तालकम्_DIE_ON_ERROR);

	refresh_सूचकांक(the_repository->सूचकांक, REFRESH_QUIET, रिक्त, रिक्त, रिक्त);

	स्मृतिं_पूरय(&opts, 0, परिमाण(opts));
	opts.शीर्ष_idx = 1;
	opts.src_सूचकांक = the_repository->सूचकांक;
	opts.dst_सूचकांक = the_repository->सूचकांक;
	opts.update = 1;
	opts.merge = 1;
	opts.reset = reset ? UNPACK_RESET_PROTECT_UNTRACKED : 0;
	opts.preserve_ignored = 0; /* FIXME: !overwrite_ignore */
	opts.fn = twoway_merge;
	init_वृक्षः_desc(&t[0], &शीर्ष->object.oid, शीर्ष->बफर, शीर्ष->आकार);
	init_वृक्षः_desc(&t[1], &remote->object.oid, remote->बफर, remote->आकार);

	यदि (unpack_trees(2, t, &opts)) {
		rollback_तालकम्_file(&तालकम्_file);
		प्रत्यागम -1;
	}

	यदि (write_locked_सूचकांक(the_repository->सूचकांक, &तालकम्_file, COMMIT_तालकम्))
		die(_("unable to write new index file"));

	प्रत्यागम 0;
}

/**
 * Merges a tree into the index. The index's stat info will take precedence
 * over the merged tree's. Returns 0 on success, -1 on failure.
 */
स्थिर पूर्णाङ्क merge_वृक्षः(संरचना वृक्षः *वृक्षः)
{
	संरचना तालकम्_file तालकम्_file = तालकम्_INIT;
	संरचना unpack_trees_options opts;
	संरचना वृक्षः_desc t[1];

	यदि (repo_parse_वृक्षः(the_repository, वृक्षः))
		प्रत्यागम -1;

	repo_hold_locked_सूचकांक(the_repository, &तालकम्_file, तालकम्_DIE_ON_ERROR);

	स्मृतिं_पूरय(&opts, 0, परिमाण(opts));
	opts.शीर्ष_idx = 1;
	opts.src_सूचकांक = the_repository->सूचकांक;
	opts.dst_सूचकांक = the_repository->सूचकांक;
	opts.merge = 1;
	opts.fn = oneway_merge;
	init_वृक्षः_desc(&t[0], &वृक्षः->object.oid, वृक्षः->बफर, वृक्षः->आकार);

	यदि (unpack_trees(1, t, &opts)) {
		rollback_तालकम्_file(&तालकम्_file);
		प्रत्यागम -1;
	}

	यदि (write_locked_सूचकांक(the_repository->सूचकांक, &तालकम्_file, COMMIT_तालकम्))
		die(_("unable to write new index file"));

	प्रत्यागम 0;
}

/**
 * Clean the index without touching entries that are not modified between
 * `head` and `remote`.
 */
स्थिर पूर्णाङ्क clean_सूचकांक(ध्रुव संरचना object_id *शीर्ष, ध्रुव संरचना object_id *remote)
{
	संरचना वृक्षः *शीर्ष_वृक्षः, *remote_वृक्षः, *सूचकांक_वृक्षः;
	संरचना object_id सूचकांक;

	शीर्ष_वृक्षः = repo_parse_वृक्षः_परोक्षम्(the_repository, शीर्ष);
	यदि (!शीर्ष_वृक्षः)
		प्रत्यागम error(_("Could not parse object '%s'."), oid_गन्तव्यम्_hex(शीर्ष));

	remote_वृक्षः = repo_parse_वृक्षः_परोक्षम्(the_repository, remote);
	यदि (!remote_वृक्षः)
		प्रत्यागम error(_("Could not parse object '%s'."), oid_गन्तव्यम्_hex(remote));

	repo_read_सूचकांक_unmerged(the_repository);

	यदि (fast_forward_गन्तव्यम्(शीर्ष_वृक्षः, शीर्ष_वृक्षः, 1))
		प्रत्यागम -1;

	यदि (write_सूचकांक_as_वृक्षः(&सूचकांक, the_repository->सूचकांक,
				repo_get_सूचकांक_file(the_repository),
				0, रिक्त))
		प्रत्यागम -1;

	सूचकांक_वृक्षः = repo_parse_वृक्षः_परोक्षम्(the_repository, &सूचकांक);
	यदि (!सूचकांक_वृक्षः)
		प्रत्यागम error(_("Could not parse object '%s'."), oid_गन्तव्यम्_hex(&सूचकांक));

	यदि (fast_forward_गन्तव्यम्(सूचकांक_वृक्षः, remote_वृक्षः, 0))
		प्रत्यागम -1;

	यदि (merge_वृक्षः(remote_वृक्षः))
		प्रत्यागम -1;

	remove_branch_state(the_repository, 0);

	प्रत्यागम 0;
}

/**
 * Resets rerere's merge resolution metadata.
 */
स्थिर शून्य am_rerere_clear(शून्य)
{
	संरचना string_list merge_rr = STRING_LIST_INIT_DUP;
	rerere_clear(the_repository, &merge_rr);
	string_list_clear(&merge_rr, 1);
}

/**
 * Resume the current am session by skipping the current patch.
 */
स्थिर शून्य am_skip(संरचना am_state *state)
{
	संरचना object_id शीर्ष;

	am_rerere_clear();

	यदि (repo_get_oid(the_repository, "HEAD", &शीर्ष))
		oidcpy(&शीर्ष, the_hash_algo->रिक्तानि_वृक्षः);

	यदि (clean_सूचकांक(&शीर्ष, &शीर्ष))
		die(_("failed to clean index"));

	यदि (state->rebasing) {
		FILE *fp = xfopen(am_मार्गः(state, "rewritten"), "a");

		प्रतिज्ञा(!is_null_oid(&state->orig_commit));
		fprintf(fp, "%s ", oid_गन्तव्यम्_hex(&state->orig_commit));
		fprintf(fp, "%s\n", oid_गन्तव्यम्_hex(&शीर्ष));
		fclose(fp);
	}

	am_अग्र(state);
	am_आरोपय(state);
	am_सततम्(state, 0);
}

/**
 * Returns true if it is safe to reset HEAD to the ORIG_HEAD, false otherwise.
 *
 * It is not safe to reset HEAD when:
 * 1. git-am previously failed because the index was dirty.
 * 2. HEAD has moved since git-am previously failed.
 */
स्थिर पूर्णाङ्क safe_गन्तव्यम्_abort(ध्रुव संरचना am_state *state)
{
	संरचना strbuf sb = STRBUF_INIT;
	संरचना object_id abort_safety, शीर्ष;

	यदि (file_exists(am_मार्गः(state, "dirtyindex")))
		प्रत्यागम 0;

	यदि (read_state_file(&sb, state, "abort-safety", 1) > 0) {
		यदि (get_oid_hex(sb.buf, &abort_safety))
			die(_("could not parse %s"), am_मार्गः(state, "abort-safety"));
	} अन्यथा
		oidclr(&abort_safety, the_repository->hash_algo);
	strbuf_release(&sb);

	यदि (repo_get_oid(the_repository, "HEAD", &शीर्ष))
		oidclr(&शीर्ष, the_repository->hash_algo);

	यदि (oideq(&शीर्ष, &abort_safety))
		प्रत्यागम 1;

	warning(_("You seem to have moved HEAD since the last 'am' failure.\n"
		"Not rewinding to ORIG_HEAD"));

	प्रत्यागम 0;
}

/**
 * Aborts the current am session if it is safe to do so.
 */
स्थिर शून्य am_abort(संरचना am_state *state)
{
	संरचना object_id curr_शीर्ष, orig_शीर्ष;
	पूर्णाङ्क has_curr_शीर्ष, has_orig_शीर्ष;
	वर्ण *curr_branch;

	यदि (!safe_गन्तव्यम्_abort(state)) {
		am_destroy(state);
		प्रत्यागम;
	}

	am_rerere_clear();

	curr_branch = refs_resolve_refdup(get_main_ref_store(the_repository),
					  "HEAD", 0, &curr_शीर्ष, रिक्त);
	has_curr_शीर्ष = curr_branch && !is_null_oid(&curr_शीर्ष);
	यदि (!has_curr_शीर्ष)
		oidcpy(&curr_शीर्ष, the_hash_algo->रिक्तानि_वृक्षः);

	has_orig_शीर्ष = !repo_get_oid(the_repository, "ORIG_HEAD", &orig_शीर्ष);
	यदि (!has_orig_शीर्ष)
		oidcpy(&orig_शीर्ष, the_hash_algo->रिक्तानि_वृक्षः);

	यदि (clean_सूचकांक(&curr_शीर्ष, &orig_शीर्ष))
		die(_("failed to clean index"));

	यदि (has_orig_शीर्ष)
		refs_update_ref(get_main_ref_store(the_repository),
				"am --abort", "HEAD", &orig_शीर्ष,
				has_curr_शीर्ष ? &curr_शीर्ष : रिक्त, 0,
				UPDATE_REFS_DIE_ON_ERR);
	अन्यथा यदि (curr_branch)
		refs_delete_ref(get_main_ref_store(the_repository), रिक्त,
				curr_branch, रिक्त, REF_NO_DEREF);

	मुक्त(curr_branch);
	am_destroy(state);
}

स्थिर पूर्णाङ्क show_patch(संरचना am_state *state, गणना resume_type resume_बहुलकम्)
{
	संरचना strbuf sb = STRBUF_INIT;
	ध्रुव वर्ण *patch_मार्गः;
	पूर्णाङ्क len;

	यदि (!is_null_oid(&state->orig_commit)) {
		संरचना child_process cmd = CHILD_PROCESS_INIT;

		strvec_pushl(&cmd.तर्काः, "show", oid_गन्तव्यम्_hex(&state->orig_commit),
			     "--", रिक्त);
		cmd.git_cmd = 1;
		प्रत्यागम सततम्_command(&cmd);
	}

	विकल्प (resume_बहुलकम्) {
	पक्ष RESUME_SHOW_PATCH_RAW:
		patch_मार्गः = am_मार्गः(state, msgnum(state));
		भङ्ग;
	पक्ष RESUME_SHOW_PATCH_DIFF:
		patch_मार्गः = am_मार्गः(state, "patch");
		भङ्ग;
	स्वभाव:
		BUG("invalid mode for --show-current-patch");
	}

	len = strbuf_read_file(&sb, patch_मार्गः, 0);
	यदि (len < 0)
		die_errno(_("failed to read '%s'"), patch_मार्गः);

	सज्जता_pager(the_repository);
	write_अन्तर्_पूर्णम्(1, sb.buf, sb.len);
	strbuf_release(&sb);
	प्रत्यागम 0;
}

/**
 * parse_options() callback that validates and sets opt->value to the
 * PATCH_FORMAT_* enum value corresponding to `arg`.
 */
स्थिर पूर्णाङ्क parse_opt_patchformat(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *opt_मानम् = opt->मानम्;

	यदि (unset)
		*opt_मानम् = PATCH_FORMAT_UNKNOWN;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "mbox"))
		*opt_मानम् = PATCH_FORMAT_MBOX;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "stgit"))
		*opt_मानम् = PATCH_FORMAT_STGIT;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "stgit-series"))
		*opt_मानम् = PATCH_FORMAT_STGIT_SERIES;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "hg"))
		*opt_मानम् = PATCH_FORMAT_HG;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "mboxrd"))
		*opt_मानम् = PATCH_FORMAT_MBOXRD;
	/*
	 * Please update $__git_patchformat in git-completion.bash
	 * when you add new options
	 */
	अन्यथा
		प्रत्यागम error(_("invalid value for '%s': '%s'"),
			     "--patch-format", arg);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क parse_opt_show_current_patch(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क *opt_मानम् = opt->मानम्;

	BUG_ON_OPT_NEG(unset);

	यदि (!arg)
		*opt_मानम् = opt->defval;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "raw"))
		*opt_मानम् = RESUME_SHOW_PATCH_RAW;
	अन्यथा यदि (!वाक्यं_तुलय(arg, "diff"))
		*opt_मानम् = RESUME_SHOW_PATCH_DIFF;
	/*
	 * Please update $__git_showcurrentpatch in git-completion.bash
	 * when you add new options
	 */
	अन्यथा
		प्रत्यागम error(_("invalid value for '%s': '%s'"),
			     "--show-current-patch", arg);
	प्रत्यागम 0;
}

पूर्णाङ्क cmd_am(पूर्णाङ्क argc,
	   ध्रुव वर्ण **argv,
	   ध्रुव वर्ण *उपसर्गः,
	   संरचना repository *repo अप्रयुक्तम्)
{
	संरचना am_state state;
	पूर्णाङ्क द्विमान = -1;
	पूर्णाङ्क keep_cr = -1;
	पूर्णाङ्क patch_format = PATCH_FORMAT_UNKNOWN;
	गणना resume_type resume_बहुलकम् = RESUME_FALSE;
	पूर्णाङ्क अन्तर्_प्रगतिः;
	पूर्णाङ्क ret = 0;

	ध्रुव वर्ण * ध्रुव usage[] = {
		N_("git am [<options>] [(<mbox> | <Maildir>)...]"),
		N_("git am [<options>] (--continue | --skip | --abort)"),
		रिक्त
	};

	संरचना option options[] = {
		OPT_BOOL('i', "interactive", &state.interactive,
			N_("run interactively")),
		OPT_BOOL('n', "no-verify", &state.no_verify,
			N_("bypass pre-applypatch and applypatch-msg hooks")),
		OPT_HIDDEN_BOOL('b', "binary", &द्विमान,
			N_("historical option -- no-op")),
		OPT_BOOL('3', "3way", &state.threeway,
			N_("allow fall back on 3way merging if needed")),
		OPT__QUIET(&state.quiet, N_("be quiet")),
		OPT_समुच्चयः_INT('s', "signoff", &state.signoff,
			N_("add a Signed-off-by trailer to the commit message"),
			SIGNOFF_EXPLICIT),
		OPT_BOOL('u', "utf8", &state.utf8,
			N_("recode into utf8 (default)")),
		OPT_समुच्चयः_INT('k', "keep", &state.keep,
			N_("pass -k flag to git-mailinfo"), KEEP_TRUE),
		OPT_समुच्चयः_INT(0, "keep-non-patch", &state.keep,
			N_("pass -b flag to git-mailinfo"), KEEP_NON_PATCH),
		OPT_BOOL('m', "message-id", &state.सन्देशः_id,
			N_("pass -m flag to git-mailinfo")),
		OPT_समुच्चयः_INT(0, "keep-cr", &keep_cr,
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
		OPT_PASSTHRU_ARGV(0, "ignore-space-change", &state.git_apply_opts, रिक्त,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_PASSTHRU_ARGV(0, "ignore-whitespace", &state.git_apply_opts, रिक्त,
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
		OPT_PASSTHRU_ARGV('C', रिक्त, &state.git_apply_opts, N_("n"),
			N_("pass it through git-apply"),
			0),
		OPT_PASSTHRU_ARGV('p', रिक्त, &state.git_apply_opts, N_("num"),
			N_("pass it through git-apply"),
			0),
		OPT_CALLBACK(0, "patch-format", &patch_format, N_("format"),
			N_("format the patch(es) are in"),
			parse_opt_patchformat),
		OPT_PASSTHRU_ARGV(0, "reject", &state.git_apply_opts, रिक्त,
			N_("pass it through git-apply"),
			PARSE_OPT_NOARG),
		OPT_STRING(0, "resolvemsg", &state.resolvemsg, रिक्त,
			N_("override error message when patch failure occurs")),
		OPT_CMDMODE(0, "continue", &resume_बहुलकम्,
			N_("continue applying patches after resolving a conflict"),
			RESUME_RESOLVED),
		OPT_CMDMODE('r', "resolved", &resume_बहुलकम्,
			N_("synonyms for --continue"),
			RESUME_RESOLVED),
		OPT_CMDMODE(0, "skip", &resume_बहुलकम्,
			N_("skip the current patch"),
			RESUME_SKIP),
		OPT_CMDMODE(0, "abort", &resume_बहुलकम्,
			N_("restore the original branch and abort the patching operation"),
			RESUME_ABORT),
		OPT_CMDMODE(0, "quit", &resume_बहुलकम्,
			N_("abort the patching operation but keep HEAD where it is"),
			RESUME_QUIT),
		{
			.type = OPTION_CALLBACK,
			.long_name = "show-current-patch",
			.मानम् = &resume_बहुलकम्,
			.precision = परिमाण(resume_बहुलकम्),
			.argh = "(diff|raw)",
			.help = N_("show the patch being applied"),
			.flags = PARSE_OPT_CMDMODE | PARSE_OPT_OPTARG | PARSE_OPT_NONEG | PARSE_OPT_LITERAL_ARGHELP,
			.callback = parse_opt_show_current_patch,
			.defval = RESUME_SHOW_PATCH_RAW,
		},
		OPT_CMDMODE(0, "retry", &resume_बहुलकम्,
			N_("try to apply current patch again"),
			RESUME_APPLY),
		OPT_CMDMODE(0, "allow-empty", &resume_बहुलकम्,
			N_("record the empty patch as an empty commit"),
			RESUME_ALLOW_रिक्तानि),
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
			.मानम् = &state.sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG-sign commits"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_CALLBACK_F(0, "empty", &state.रिक्तानि_type, "(stop|drop|keep)",
		  N_("how to handle empty patches"),
		  PARSE_OPT_NONEG, am_option_parse_रिक्तानि),
		OPT_HIDDEN_BOOL(0, "rebasing", &state.rebasing,
			N_("(internal use for git-rebase)")),
		OPT_अन्तः()
	};

	show_usage_with_options_if_asked(argc, argv, usage, options);

	repo_config(the_repository, git_default_config, रिक्त);

	am_state_init(&state);

	अन्तर्_प्रगतिः = am_अन्तर्_प्रगतिः(&state);
	यदि (अन्तर्_प्रगतिः)
		am_आरोपय(&state);

	argc = parse_options(argc, argv, उपसर्गः, options, usage, 0);

	यदि (द्विमान >= 0)
		fprintf_ln(stderr, _("The -b/--binary option has been a no-op for long time, and\n"
				"it will be removed. Please do not use it anymore."));

	/* Ensure a valid committer ident can be constructed */
	git_committer_info(IDENT_STRICT);

	यदि (repo_read_सूचकांक_preload(the_repository, रिक्त, 0) < 0)
		die(_("failed to read the index"));

	यदि (अन्तर्_प्रगतिः) {
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
		यदि (argc || (resume_बहुलकम् == RESUME_FALSE && !isatty(0)))
			die(_("previous rebase directory %s still exists but mbox given."),
				state.dir);

		यदि (resume_बहुलकम् == RESUME_FALSE)
			resume_बहुलकम् = RESUME_APPLY;

		यदि (state.signoff == SIGNOFF_EXPLICIT)
			am_append_signoff(&state);
	} अन्यथा {
		संरचना strvec paths = STRVEC_INIT;
		पूर्णाङ्क i;

		/*
		 * Handle stray state directory in the independent-run case. In
		 * the --rebasing case, it is up to the caller to take care of
		 * stray directories.
		 */
		यदि (file_exists(state.dir) && !state.rebasing) {
			यदि (resume_बहुलकम् == RESUME_ABORT || resume_बहुलकम् == RESUME_QUIT) {
				am_destroy(&state);
				am_state_release(&state);
				प्रत्यागम 0;
			}

			die(_("Stray %s directory found.\n"
				"Use \"git am --abort\" to remove it."),
				state.dir);
		}

		यदि (resume_बहुलकम्)
			die(_("Resolve operation not in progress, we are not resuming."));

		प्रति (i = 0; i < argc; i++) {
			यदि (is_absolute_मार्गः(argv[i]) || !उपसर्गः)
				strvec_प्रक्षिप(&paths, argv[i]);
			अन्यथा
				strvec_प्रक्षिप(&paths, mkpath("%s/%s", उपसर्गः, argv[i]));
		}

		यदि (state.interactive && !paths.nr)
			die(_("interactive mode requires patches on the command line"));

		am_सज्जता(&state, patch_format, paths.v, keep_cr);

		strvec_clear(&paths);
	}

	विकल्प (resume_बहुलकम्) {
	पक्ष RESUME_FALSE:
		am_सततम्(&state, 0);
		भङ्ग;
	पक्ष RESUME_APPLY:
		am_सततम्(&state, 1);
		भङ्ग;
	पक्ष RESUME_RESOLVED:
	पक्ष RESUME_ALLOW_रिक्तानि:
		am_resolve(&state, resume_बहुलकम् == RESUME_ALLOW_रिक्तानि ? 1 : 0);
		भङ्ग;
	पक्ष RESUME_SKIP:
		am_skip(&state);
		भङ्ग;
	पक्ष RESUME_ABORT:
		am_abort(&state);
		भङ्ग;
	पक्ष RESUME_QUIT:
		am_rerere_clear();
		am_destroy(&state);
		भङ्ग;
	पक्ष RESUME_SHOW_PATCH_RAW:
	पक्ष RESUME_SHOW_PATCH_DIFF:
		ret = show_patch(&state, resume_बहुलकम्);
		भङ्ग;
	स्वभाव:
		BUG("invalid resume value");
	}

	am_state_release(&state);

	प्रत्यागम ret;
}
