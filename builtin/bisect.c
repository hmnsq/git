#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "copy.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "bisect.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "strvec.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "oid-array.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "prompt.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "revision.h"

സ്ഥിതം GIT_PATH_FUNC(git_path_bisect_terms, "BISECT_TERMS")
സ്ഥിതം GIT_PATH_FUNC(git_path_bisect_ancestors_ok, "BISECT_ANCESTORS_OK")
സ്ഥിതം GIT_PATH_FUNC(git_path_bisect_start, "BISECT_START")
സ്ഥിതം GIT_PATH_FUNC(git_path_bisect_log, "BISECT_LOG")
സ്ഥിതം GIT_PATH_FUNC(git_path_bisect_names, "BISECT_NAMES")
സ്ഥിതം GIT_PATH_FUNC(git_path_bisect_first_parent, "BISECT_FIRST_PARENT")
സ്ഥിതം GIT_PATH_FUNC(git_path_bisect_reset_when_found, "BISECT_RESET_WHEN_FOUND")
സ്ഥിതം GIT_PATH_FUNC(git_path_bisect_run, "BISECT_RUN")

#define BUILTIN_GIT_BISECT_START_USAGE \
	N_("git bisect start [--term-(bad|new)=<term-new> --term-(good|old)=<term-old>]\n" \
	   "                 [--no-checkout] [--first-parent] [--reset-when-found[=<where>]] [<bad> [<good>...]] [--] [<pathspec>...]")
#define BUILTIN_GIT_BISECT_BAD_USAGE \
	N_("git bisect (bad|new|<term-new>) [<rev>]")
#define BUILTIN_GIT_BISECT_GOOD_USAGE \
	N_("git bisect (good|old|<term-old>) [<rev>...]")
#define BUILTIN_GIT_BISECT_TERMS_USAGE \
	"git bisect terms [--term-(good|old) | --term-(bad|new)]"
#define BUILTIN_GIT_BISECT_SKIP_USAGE \
	N_("git bisect skip [(<rev>|<range>)...]")
#define BUILTIN_GIT_BISECT_NEXT_USAGE \
	"git bisect next"
#define BUILTIN_GIT_BISECT_RESET_USAGE \
	N_("git bisect reset [<commit>]")
#define BUILTIN_GIT_BISECT_VISUALIZE_USAGE \
	"git bisect (visualize|view)"
#define BUILTIN_GIT_BISECT_REPLAY_USAGE \
	N_("git bisect replay <logfile>")
#define BUILTIN_GIT_BISECT_LOG_USAGE \
	"git bisect log"
#define BUILTIN_GIT_BISECT_RUN_USAGE \
	N_("git bisect run [--reset-when-found[=<where>]] <cmd> [<arg>...]")
#define BUILTIN_GIT_BISECT_HELP_USAGE \
	"git bisect help"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_bisect_usage[] = {
	BUILTIN_GIT_BISECT_START_USAGE,
	BUILTIN_GIT_BISECT_BAD_USAGE,
	BUILTIN_GIT_BISECT_GOOD_USAGE,
	BUILTIN_GIT_BISECT_TERMS_USAGE,
	BUILTIN_GIT_BISECT_SKIP_USAGE,
	BUILTIN_GIT_BISECT_NEXT_USAGE,
	BUILTIN_GIT_BISECT_RESET_USAGE,
	BUILTIN_GIT_BISECT_VISUALIZE_USAGE,
	BUILTIN_GIT_BISECT_REPLAY_USAGE,
	BUILTIN_GIT_BISECT_LOG_USAGE,
	BUILTIN_GIT_BISECT_RUN_USAGE,
	BUILTIN_GIT_BISECT_HELP_USAGE,
	ശൂന്യ
};

എണ്ണൽ reset_when_found_mode {
	RESET_WHEN_FOUND_NONE,
	RESET_WHEN_FOUND_TO_ORIGINAL,
	RESET_WHEN_FOUND_TO_FOUND,
};

ഘടന add_bisect_ref_data {
	ഘടന rev_info *revs;
	unsigned പൂർണ്ണസംഖ്യ object_flags;
};

ഘടന bisect_terms {
	അക്ഷരം *term_good;
	അക്ഷരം *term_bad;
};

സ്ഥിതം ശൂന്യം free_terms(ഘടന bisect_terms *terms)
{
	FREE_AND_NULL(terms->term_good);
	FREE_AND_NULL(terms->term_bad);
}

സ്ഥിതം ശൂന്യം set_terms(ഘടന bisect_terms *terms, സ്ഥിരം അക്ഷരം *bad,
		      സ്ഥിരം അക്ഷരം *good)
{
	സ്വതന്ത്രം((ശൂന്യം *)terms->term_good);
	terms->term_good = xstrdup(good);
	സ്വതന്ത്രം((ശൂന്യം *)terms->term_bad);
	terms->term_bad = xstrdup(bad);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം vocab_bad[] = "bad|new";
സ്ഥിതം സ്ഥിരം അക്ഷരം vocab_good[] = "good|old";

സ്ഥിതം പൂർണ്ണസംഖ്യ bisect_autostart(ഘടന bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
LAST_ARG_MUST_BE_NULL
സ്ഥിതം പൂർണ്ണസംഖ്യ one_of(സ്ഥിരം അക്ഷരം *term, ...)
{
	പൂർണ്ണസംഖ്യ res = 0;
	va_list matches;
	സ്ഥിരം അക്ഷരം *match;

	va_start(matches, term);
	ആകുമ്പോൾ (!res && (match = va_arg(matches, സ്ഥിരം അക്ഷരം *)))
		res = !strcmp(term, match);
	va_end(matches);

	തിരികെ res;
}

/*
 * return code BISECT_INTERNAL_SUCCESS_MERGE_BASE
 * and BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND are codes
 * that indicate special success.
 */

സ്ഥിതം പൂർണ്ണസംഖ്യ is_bisect_success(എണ്ണൽ bisect_error res)
{
	തിരികെ !res ||
		res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND ||
		res == BISECT_INTERNAL_SUCCESS_MERGE_BASE;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ write_in_file(സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *mode, സ്ഥിരം അക്ഷരം *format, va_list args)
{
	FILE *fp = ശൂന്യ;
	പൂർണ്ണസംഖ്യ res = 0;

	എങ്കിൽ (strcmp(mode, "w") && strcmp(mode, "a"))
		BUG("write-in-file does not support '%s' mode", mode);
	fp = fopen(path, mode);
	എങ്കിൽ (!fp)
		തിരികെ error_errno(_("cannot open file '%s' in mode '%s'"), path, mode);
	res = vfprintf(fp, format, args);

	എങ്കിൽ (res < 0) {
		പൂർണ്ണസംഖ്യ saved_errno = errno;
		fclose(fp);
		errno = saved_errno;
		തിരികെ error_errno(_("could not write to file '%s'"), path);
	}

	തിരികെ fclose(fp);
}

__attribute__((format (printf, 2, 3)))
സ്ഥിതം പൂർണ്ണസംഖ്യ write_to_file(സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *format, ...)
{
	പൂർണ്ണസംഖ്യ res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "w", format, args);
	va_end(args);

	തിരികെ res;
}

__attribute__((format (printf, 2, 3)))
സ്ഥിതം പൂർണ്ണസംഖ്യ append_to_file(സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *format, ...)
{
	പൂർണ്ണസംഖ്യ res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "a", format, args);
	va_end(args);

	തിരികെ res;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ print_file_to_stdout(സ്ഥിരം അക്ഷരം *path)
{
	പൂർണ്ണസംഖ്യ fd = open(path, O_RDONLY);
	പൂർണ്ണസംഖ്യ ret = 0;

	എങ്കിൽ (fd < 0)
		തിരികെ error_errno(_("cannot open file '%s' for reading"), path);
	എങ്കിൽ (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), path);
	close(fd);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ check_term_format(സ്ഥിരം അക്ഷരം *term, സ്ഥിരം അക്ഷരം *orig_term)
{
	പൂർണ്ണസംഖ്യ res;
	അക്ഷരം *new_term = xstrfmt("refs/bisect/%s", term);

	res = check_refname_format(new_term, 0);
	സ്വതന്ത്രം(new_term);

	എങ്കിൽ (res)
		തിരികെ error(_("'%s' is not a valid term"), term);

	എങ്കിൽ (one_of(term, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", ശൂന്യ))
		തിരികെ error(_("can't use the builtin command '%s' as a term"), term);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	എങ്കിൽ ((strcmp(orig_term, "bad") && one_of(term, "bad", "new", ശൂന്യ)) ||
		 (strcmp(orig_term, "good") && one_of(term, "good", "old", ശൂന്യ)))
		തിരികെ error(_("can't change the meaning of the term '%s'"), term);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ write_terms(സ്ഥിരം അക്ഷരം *bad, സ്ഥിരം അക്ഷരം *good)
{
	പൂർണ്ണസംഖ്യ res;

	എങ്കിൽ (!strcmp(bad, good))
		തിരികെ error(_("please use two different terms"));

	എങ്കിൽ (check_term_format(bad, "bad") || check_term_format(good, "good"))
		തിരികെ -1;

	res = write_to_file(git_path_bisect_terms(), "%s\n%s\n", bad, good);

	തിരികെ res;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ bisect_reset(സ്ഥിരം അക്ഷരം *commit, bool quiet)
{
	ഘടന strbuf branch = STRBUF_INIT;

	എങ്കിൽ (!commit) {
		എങ്കിൽ (!strbuf_read_file(&branch, git_path_bisect_start(), 0))
			printf(_("We are not bisecting.\n"));
		അല്ലെങ്കിൽ
			strbuf_rtrim(&branch);
	} അല്ലെങ്കിൽ {
		ഘടന object_id oid;

		എങ്കിൽ (repo_get_oid_commit(the_repository, commit, &oid))
			തിരികെ error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	എങ്കിൽ (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		ഘടന child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "checkout", "--ignore-other-worktrees", ശൂന്യ);
		എങ്കിൽ (quiet)
			strvec_തള്ളുക(&cmd.args, "--quiet");
		strvec_pushl(&cmd.args, branch.buf, "--", ശൂന്യ);
		എങ്കിൽ (run_command(&cmd)) {
			error(_("could not check out original"
				" HEAD '%s'. Try 'git bisect"
				" reset <commit>'."), branch.buf);
			strbuf_release(&branch);
			തിരികെ -1;
		}
	}

	strbuf_release(&branch);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_reset_when_found(സ്ഥിരം അക്ഷരം *മൂല്യം,
				  എണ്ണൽ reset_when_found_mode *mode)
{
	എങ്കിൽ (!strcmp(മൂല്യം, "original"))
		*mode = RESET_WHEN_FOUND_TO_ORIGINAL;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(മൂല്യം, "found"))
		*mode = RESET_WHEN_FOUND_TO_FOUND;
	അല്ലെങ്കിൽ
		തിരികെ error(_("invalid value for '--reset-when-found': '%s'"),
			     മൂല്യം);

	തിരികെ 0;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *reset_when_found_mode_name(എണ്ണൽ reset_when_found_mode mode)
{
	തിരഞ്ഞെടുപ്പ് (mode) {
	അവസ്ഥ RESET_WHEN_FOUND_TO_ORIGINAL:
		തിരികെ "original";
	അവസ്ഥ RESET_WHEN_FOUND_TO_FOUND:
		തിരികെ "found";
	അവസ്ഥ RESET_WHEN_FOUND_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", mode);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ read_reset_when_found(എണ്ണൽ reset_when_found_mode *mode)
{
	ഘടന strbuf മൂല്യം = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ res = 0;

	*mode = RESET_WHEN_FOUND_NONE;
	എങ്കിൽ (is_empty_or_missing_file(git_path_bisect_reset_when_found()))
		തിരികെ 0;

	എങ്കിൽ (strbuf_read_file(&മൂല്യം, git_path_bisect_reset_when_found(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_path_bisect_reset_when_found());
		പോകുക out;
	}
	strbuf_trim(&മൂല്യം);
	എങ്കിൽ (parse_reset_when_found(മൂല്യം.buf, mode))
		res = -1;

out:
	strbuf_release(&മൂല്യം);
	തിരികെ res;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ bisect_reset_when_found(എണ്ണൽ reset_when_found_mode mode)
{
	ഘടന bisect_terms terms = { 0 };
	അക്ഷരം *commit = ശൂന്യ;
	പൂർണ്ണസംഖ്യ res;

	എങ്കിൽ (mode == RESET_WHEN_FOUND_TO_FOUND) {
		read_bisect_terms(&terms.term_bad, &terms.term_good);
		commit = xstrfmt("refs/bisect/%s", terms.term_bad);
	} അല്ലെങ്കിൽ എങ്കിൽ (mode == RESET_WHEN_FOUND_NONE) {
		BUG("automatic reset requested without a reset mode");
	}

	res = bisect_reset(commit, true);
	എങ്കിൽ (!res)
		res = bisect_clean_state();

	സ്വതന്ത്രം(commit);
	free_terms(&terms);
	തിരികെ res;
}

സ്ഥിതം ശൂന്യം log_commit(FILE *fp,
		       സ്ഥിരം അക്ഷരം *fmt, സ്ഥിരം അക്ഷരം *state,
		       ഘടന commit *commit)
{
	ഘടന pretty_print_context pp = {0};
	ഘടന strbuf commit_സന്ദേശം = STRBUF_INIT;
	അക്ഷരം *label = xstrfmt(fmt, state);

	repo_format_commit_message(the_repository, commit, "%s", &commit_സന്ദേശം,
				   &pp);

	fprintf(fp, "# %s: [%s] %s\n", label, oid_to_hex(&commit->object.oid),
		commit_സന്ദേശം.buf);

	strbuf_release(&commit_സന്ദേശം);
	സ്വതന്ത്രം(label);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ bisect_write(സ്ഥിരം അക്ഷരം *state, സ്ഥിരം അക്ഷരം *rev,
			സ്ഥിരം ഘടന bisect_terms *terms, പൂർണ്ണസംഖ്യ nolog)
{
	ഘടന strbuf tag = STRBUF_INIT;
	ഘടന object_id oid;
	ഘടന commit *commit;
	FILE *fp = ശൂന്യ;
	പൂർണ്ണസംഖ്യ res = 0;

	എങ്കിൽ (!strcmp(state, terms->term_bad)) {
		strbuf_addf(&tag, "refs/bisect/%s", state);
	} അല്ലെങ്കിൽ എങ്കിൽ (one_of(state, terms->term_good, "skip", ശൂന്യ)) {
		strbuf_addf(&tag, "refs/bisect/%s-%s", state, rev);
	} അല്ലെങ്കിൽ {
		res = error(_("Bad bisect_write argument: %s"), state);
		പോകുക finish;
	}

	എങ്കിൽ (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		പോകുക finish;
	}

	എങ്കിൽ (refs_update_ref(get_main_ref_store(the_repository), ശൂന്യ, tag.buf, &oid, ശൂന്യ, 0,
			    UPDATE_REFS_സന്ദേശം_ON_ERR)) {
		res = -1;
		പോകുക finish;
	}

	fp = fopen(git_path_bisect_log(), "a");
	എങ്കിൽ (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_path_bisect_log());
		പോകുക finish;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	എങ്കിൽ (!nolog)
		fprintf(fp, "git bisect %s %s\n", state, rev);

finish:
	എങ്കിൽ (fp)
		fclose(fp);
	strbuf_release(&tag);
	തിരികെ res;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ check_and_set_terms(ഘടന bisect_terms *terms, സ്ഥിരം അക്ഷരം *cmd)
{
	പൂർണ്ണസംഖ്യ has_term_file = !is_empty_or_missing_file(git_path_bisect_terms());

	എങ്കിൽ (one_of(cmd, "skip", "start", "terms", ശൂന്യ))
		തിരികെ 0;

	എങ്കിൽ (has_term_file && strcmp(cmd, terms->term_bad) &&
	    strcmp(cmd, terms->term_good))
		തിരികെ error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->term_bad,
				terms->term_good);

	എങ്കിൽ (!has_term_file) {
		എങ്കിൽ (one_of(cmd, "bad", "good", ശൂന്യ)) {
			set_terms(terms, "bad", "good");
			തിരികെ write_terms(terms->term_bad, terms->term_good);
		}
		എങ്കിൽ (one_of(cmd, "new", "old", ശൂന്യ)) {
			set_terms(terms, "new", "old");
			തിരികെ write_terms(terms->term_bad, terms->term_good);
		}
	}

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ inc_nr(സ്ഥിരം ഘടന reference *ref UNUSED, ശൂന്യം *cb_data)
{
	unsigned പൂർണ്ണസംഖ്യ *nr = (unsigned പൂർണ്ണസംഖ്യ *)cb_data;
	(*nr)++;
	തിരികെ 0;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം need_bad_and_good_revision_warning[] =
	N_("You need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

സ്ഥിതം സ്ഥിരം അക്ഷരം need_bisect_start_warning[] =
	N_("You need to start by \"git bisect start\".\n"
	   "You then need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

സ്ഥിതം പൂർണ്ണസംഖ്യ decide_next(സ്ഥിരം ഘടന bisect_terms *terms,
		       സ്ഥിരം അക്ഷരം *current_term, പൂർണ്ണസംഖ്യ missing_good,
		       പൂർണ്ണസംഖ്യ missing_bad)
{
	എങ്കിൽ (!missing_good && !missing_bad)
		തിരികെ 0;
	എങ്കിൽ (!current_term)
		തിരികെ -1;

	എങ്കിൽ (missing_good && !missing_bad &&
	    !strcmp(current_term, terms->term_good)) {
		അക്ഷരം *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->term_bad);
		എങ്കിൽ (!isatty(0))
			തിരികെ 0;
		/*
		 * TRANSLATORS: Make sure to include [Y] and [n] in your
		 * translation. The program will only accept English input
		 * at this point.
		 */
		yesno = git_prompt(_("Are you sure [Y/n]? "), PROMPT_ECHO);
		എങ്കിൽ (starts_with(yesno, "N") || starts_with(yesno, "n"))
			തിരികെ -1;
		തിരികെ 0;
	}

	എങ്കിൽ (!is_empty_or_missing_file(git_path_bisect_start()))
		തിരികെ error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	അല്ലെങ്കിൽ
		തിരികെ error(_(need_bisect_start_warning),
			     vocab_good, vocab_bad, vocab_good, vocab_bad);
}

സ്ഥിതം ശൂന്യം bisect_status(ഘടന bisect_state *state,
			  സ്ഥിരം ഘടന bisect_terms *terms)
{
	അക്ഷരം *bad_ref = xstrfmt("refs/bisect/%s", terms->term_bad);
	അക്ഷരം *good_glob = xstrfmt("%s-*", terms->term_good);
	ഘടന refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	എങ്കിൽ (refs_ref_exists(get_main_ref_store(the_repository), bad_ref))
		state->nr_bad = 1;

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      inc_nr, &state->nr_good, &opts);

	സ്വതന്ത്രം(good_glob);
	സ്വതന്ത്രം(bad_ref);
}

__attribute__((format (printf, 1, 2)))
സ്ഥിതം ശൂന്യം bisect_log_printf(സ്ഥിരം അക്ഷരം *fmt, ...)
{
	ഘടന strbuf buf = STRBUF_INIT;
	va_list ap;

	va_start(ap, fmt);
	strbuf_vaddf(&buf, fmt, ap);
	va_end(ap);

	printf("%s", buf.buf);
	append_to_file(git_path_bisect_log(), "# %s", buf.buf);

	strbuf_release(&buf);
}

സ്ഥിതം ശൂന്യം bisect_print_status(സ്ഥിരം ഘടന bisect_terms *terms)
{
	ഘടന bisect_state state = { 0 };

	bisect_status(&state, terms);

	/* If we had both, we'd already be started, and shouldn't get here. */
	എങ്കിൽ (state.nr_good && state.nr_bad)
		തിരികെ;

	എങ്കിൽ (!state.nr_good && !state.nr_bad)
		bisect_log_printf(_("status: waiting for both '%s' and '%s' commits\n"),
				  terms->term_good, terms->term_bad);
	അല്ലെങ്കിൽ എങ്കിൽ (state.nr_good)
		bisect_log_printf(Q_("status: waiting for '%s' commit, %d '%s' commit known\n",
				     "status: waiting for '%s' commit, %d '%s' commits known\n",
				     state.nr_good),
				  terms->term_bad, state.nr_good, terms->term_good);
	അല്ലെങ്കിൽ
		bisect_log_printf(_("status: waiting for '%s' commit(s), '%s' commit known\n"),
				  terms->term_good, terms->term_bad);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ bisect_next_check(സ്ഥിരം ഘടന bisect_terms *terms,
			     സ്ഥിരം അക്ഷരം *current_term)
{
	ഘടന bisect_state state = { 0 };
	bisect_status(&state, terms);
	തിരികെ decide_next(terms, current_term, !state.nr_good, !state.nr_bad);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_terms(ഘടന bisect_terms *terms, പൂർണ്ണസംഖ്യ file_missing_is_ok)
{
	ഘടന strbuf str = STRBUF_INIT;
	FILE *fp = ശൂന്യ;
	പൂർണ്ണസംഖ്യ res = 0;

	fp = fopen(git_path_bisect_terms(), "r");
	എങ്കിൽ (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		പോകുക finish;
	}

	free_terms(terms);
	എങ്കിൽ (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		പോകുക finish;
	}
	terms->term_bad = strbuf_detach(&str, ശൂന്യ);
	എങ്കിൽ (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		FREE_AND_NULL(terms->term_bad);
		പോകുക finish;
	}
	terms->term_good = strbuf_detach(&str, ശൂന്യ);

finish:
	എങ്കിൽ (fp)
		fclose(fp);
	strbuf_release(&str);
	തിരികെ res;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ bisect_terms(ഘടന bisect_terms *terms, സ്ഥിരം അക്ഷരം *option)
{
	എങ്കിൽ (get_terms(terms, 0))
		തിരികെ error(_("no terms defined"));

	എങ്കിൽ (!option) {
		printf(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->term_good, terms->term_bad);
		തിരികെ 0;
	}
	എങ്കിൽ (one_of(option, "--term-good", "--term-old", ശൂന്യ))
		printf("%s\n", terms->term_good);
	അല്ലെങ്കിൽ എങ്കിൽ (one_of(option, "--term-bad", "--term-new", ശൂന്യ))
		printf("%s\n", terms->term_bad);
	അല്ലെങ്കിൽ
		തിരികെ error(_("invalid argument %s for 'git bisect terms'.\n"
			       "Supported options are: "
			       "--term-good|--term-old and "
			       "--term-bad|--term-new."), option);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ bisect_append_log_quoted(സ്ഥിരം അക്ഷരം **argv)
{
	പൂർണ്ണസംഖ്യ res = 0;
	FILE *fp = fopen(git_path_bisect_log(), "a");
	ഘടന strbuf orig_args = STRBUF_INIT;

	എങ്കിൽ (!fp)
		തിരികെ -1;

	എങ്കിൽ (fprintf(fp, "git bisect start") < 1) {
		res = -1;
		പോകുക finish;
	}

	sq_quote_argv(&orig_args, argv);
	എങ്കിൽ (fprintf(fp, "%s\n", orig_args.buf) < 1)
		res = -1;

finish:
	fclose(fp);
	strbuf_release(&orig_args);
	തിരികെ res;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ add_bisect_ref(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cb)
{
	ഘടന add_bisect_ref_data *data = cb;

	add_pending_oid(data->revs, ref->name, ref->oid, data->object_flags);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ prepare_revs(ഘടന bisect_terms *terms, ഘടന rev_info *revs)
{
	ഘടന refs_for_each_ref_options opts = {
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};
	പൂർണ്ണസംഖ്യ res = 0;
	ഘടന add_bisect_ref_data cb = { revs };
	അക്ഷരം *good = xstrfmt("%s-*", terms->term_good);

	/*
	 * We cannot use terms->term_bad directly in
	 * for_each_glob_ref_in() and we have to append a '*' to it,
	 * otherwise for_each_glob_ref_in() will append '/' and '*'.
	 */
	അക്ഷരം *bad = xstrfmt("%s*", terms->term_bad);

	/*
	 * It is important to reset the flags used by revision walks
	 * as the previous call to bisect_next_all() in turn
	 * sets up a revision walk.
	 */
	reset_revision_walk();
	repo_init_revisions(the_repository, revs, ശൂന്യ);
	setup_revisions(0, ശൂന്യ, revs, ശൂന്യ);

	opts.pattern = bad;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      add_bisect_ref, &cb, &opts);

	cb.object_flags = UNINTERESTING;
	opts.pattern = good;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      add_bisect_ref, &cb, &opts);

	എങ്കിൽ (prepare_revision_walk(revs))
		res = error(_("revision walk setup failed"));

	സ്വതന്ത്രം(good);
	സ്വതന്ത്രം(bad);
	തിരികെ res;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ bisect_skipped_commits(ഘടന bisect_terms *terms)
{
	പൂർണ്ണസംഖ്യ res;
	FILE *fp = ശൂന്യ;
	ഘടന rev_info revs;
	ഘടന commit *commit;
	ഘടന pretty_print_context pp = {0};
	ഘടന strbuf commit_name = STRBUF_INIT;

	res = prepare_revs(terms, &revs);
	എങ്കിൽ (res)
		തിരികെ res;

	fp = fopen(git_path_bisect_log(), "a");
	എങ്കിൽ (!fp)
		തിരികെ error_errno(_("could not open '%s' for appending"),
				  git_path_bisect_log());

	എങ്കിൽ (fprintf(fp, "# only skipped commits left to test\n") < 0)
		തിരികെ error_errno(_("failed to write to '%s'"), git_path_bisect_log());

	ആകുമ്പോൾ ((commit = get_revision(&revs)) != ശൂന്യ) {
		strbuf_reset(&commit_name);
		repo_format_commit_message(the_repository, commit, "%s",
					   &commit_name, &pp);
		fprintf(fp, "# possible first '%s' commit: [%s] %s\n",
			terms->term_bad, oid_to_hex(&commit->object.oid),
			commit_name.buf);
	}

	/*
	 * Reset the flags used by revision walks in case
	 * there is another revision walk after this one.
	 */
	reset_revision_walk();

	strbuf_release(&commit_name);
	release_revisions(&revs);
	fclose(fp);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ bisect_successful(ഘടന bisect_terms *terms)
{
	ഘടന object_id oid;
	ഘടന commit *commit;
	ഘടന pretty_print_context pp = {0};
	ഘടന strbuf commit_name = STRBUF_INIT;
	അക്ഷരം *bad_ref = xstrfmt("refs/bisect/%s",terms->term_bad);
	പൂർണ്ണസംഖ്യ res;

	refs_read_ref(get_main_ref_store(the_repository), bad_ref, &oid);
	commit = lookup_commit_reference_by_name(bad_ref);
	എങ്കിൽ (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		സ്വതന്ത്രം(bad_ref);
		തിരികെ BISECT_FAILED;
	}
	repo_format_commit_message(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_to_file(git_path_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->term_bad, oid_to_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	സ്വതന്ത്രം(bad_ref);
	തിരികെ res;
}

സ്ഥിതം എണ്ണൽ bisect_error bisect_next(ഘടന bisect_terms *terms,
				     സ്ഥിരം അക്ഷരം *prefix)
{
	എണ്ണൽ bisect_error res;

	എങ്കിൽ (bisect_autostart(terms))
		തിരികെ BISECT_FAILED;

	എങ്കിൽ (bisect_next_check(terms, terms->term_good))
		തിരികെ BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_next_all(the_repository, prefix);

	എങ്കിൽ (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		res = bisect_successful(terms);
		തിരികെ res ? res : BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND;
	} അല്ലെങ്കിൽ എങ്കിൽ (res == BISECT_ONLY_SKIPPED_ഇടത്) {
		res = bisect_skipped_commits(terms);
		തിരികെ res ? res : BISECT_ONLY_SKIPPED_ഇടത്;
	}
	തിരികെ res;
}

സ്ഥിതം എണ്ണൽ bisect_error bisect_auto_next(ഘടന bisect_terms *terms,
					  സ്ഥിരം അക്ഷരം *prefix)
{
	എങ്കിൽ (bisect_next_check(terms, ശൂന്യ)) {
		bisect_print_status(terms);
		തിരികെ BISECT_OK;
	}

	തിരികെ bisect_next(terms, prefix);
}

സ്ഥിതം എണ്ണൽ bisect_error bisect_start(ഘടന bisect_terms *terms, പൂർണ്ണസംഖ്യ argc,
				      സ്ഥിരം അക്ഷരം **argv)
{
	പൂർണ്ണസംഖ്യ no_checkout = 0;
	പൂർണ്ണസംഖ്യ first_parent_only = 0;
	പൂർണ്ണസംഖ്യ i, has_double_dash = 0, must_write_terms = 0, bad_seen = 0;
	പൂർണ്ണസംഖ്യ flags, pathspec_pos;
	എണ്ണൽ bisect_error res = BISECT_OK;
	ഘടന string_list revs = STRING_LIST_INIT_DUP;
	ഘടന string_list states = STRING_LIST_INIT_DUP;
	ഘടന strbuf start_head = STRBUF_INIT;
	ഘടന strbuf bisect_names = STRBUF_INIT;
	ഘടന object_id head_oid;
	ഘടന object_id oid;
	എണ്ണൽ reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;
	സ്ഥിരം അക്ഷരം *head;

	എങ്കിൽ (is_bare_repository(the_repository))
		no_checkout = 1;

	/*
	 * Check for one bad and then some good revisions
	 */
	വേണ്ടി (i = 0; i < argc; i++) {
		എങ്കിൽ (!strcmp(argv[i], "--")) {
			has_double_dash = 1;
			നിർത്തുക;
		}
	}

	വേണ്ടി (i = 0; i < argc; i++) {
		സ്ഥിരം അക്ഷരം *arg = argv[i];
		എങ്കിൽ (!strcmp(argv[i], "--")) {
			നിർത്തുക;
		} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "--no-checkout")) {
			no_checkout = 1;
		} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "--first-parent")) {
			first_parent_only = 1;
		} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "--reset-when-found")) {
			reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
		} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(arg, "--reset-when-found=", &arg)) {
			എങ്കിൽ (parse_reset_when_found(arg, &reset_when_found)) {
				res = BISECT_FAILED;
				പോകുക finish;
			}
		} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "--term-good") ||
			 !strcmp(arg, "--term-old")) {
			i++;
			എങ്കിൽ (argc <= i)
				തിരികെ error(_("'' is not a valid term"));
			must_write_terms = 1;
			സ്വതന്ത്രം((ശൂന്യം *) terms->term_good);
			terms->term_good = xstrdup(argv[i]);
		} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(arg, "--term-good=", &arg) ||
			   skip_prefix(arg, "--term-old=", &arg)) {
			must_write_terms = 1;
			സ്വതന്ത്രം((ശൂന്യം *) terms->term_good);
			terms->term_good = xstrdup(arg);
		} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(arg, "--term-bad") ||
			 !strcmp(arg, "--term-new")) {
			i++;
			എങ്കിൽ (argc <= i)
				തിരികെ error(_("'' is not a valid term"));
			must_write_terms = 1;
			സ്വതന്ത്രം((ശൂന്യം *) terms->term_bad);
			terms->term_bad = xstrdup(argv[i]);
		} അല്ലെങ്കിൽ എങ്കിൽ (skip_prefix(arg, "--term-bad=", &arg) ||
			   skip_prefix(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			സ്വതന്ത്രം((ശൂന്യം *) terms->term_bad);
			terms->term_bad = xstrdup(arg);
		} അല്ലെങ്കിൽ എങ്കിൽ (starts_with(arg, "--")) {
			തിരികെ error(_("unrecognized option: '%s'"), arg);
		} അല്ലെങ്കിൽ എങ്കിൽ (!get_oidf(&oid, "%s^{commit}", arg)) {
			string_list_append(&revs, oid_to_hex(&oid));
		} അല്ലെങ്കിൽ എങ്കിൽ (has_double_dash) {
			die(_("'%s' does not appear to be a valid "
			      "revision"), arg);
		} അല്ലെങ്കിൽ {
			നിർത്തുക;
		}
	}
	എങ്കിൽ (reset_when_found != RESET_WHEN_FOUND_NONE && no_checkout) {
		res = error(_("options '%s' and '%s' cannot be used together"),
			    "--reset-when-found", "--no-checkout");
		പോകുക finish;
	}
	pathspec_pos = i;

	/*
	 * The user ran "git bisect start <sha1> <sha1>", hence did not
	 * explicitly specify the terms, but we are already starting to
	 * set references named with the default terms, and won't be able
	 * to change afterwards.
	 */
	എങ്കിൽ (revs.nr)
		must_write_terms = 1;
	വേണ്ടി (i = 0; i < revs.nr; i++) {
		എങ്കിൽ (bad_seen) {
			string_list_append(&states, terms->term_good);
		} അല്ലെങ്കിൽ {
			bad_seen = 1;
			string_list_append(&states, terms->term_bad);
		}
	}

	/*
	 * Verify HEAD
	 */
	head = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
				       "HEAD", 0, &head_oid, &flags);
	എങ്കിൽ (!head) {
		എങ്കിൽ (repo_get_oid(the_repository, "HEAD", &head_oid))
			തിരികെ error(_("bad HEAD - I need a HEAD"));
		head = "HEAD";
	}

	/*
	 * Check if we are bisecting
	 */
	എങ്കിൽ (!is_empty_or_missing_file(git_path_bisect_start())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&start_head, git_path_bisect_start(), 0);
		strbuf_trim(&start_head);
		എങ്കിൽ (!no_checkout) {
			ഘടന child_process cmd = CHILD_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.args, "checkout", start_head.buf,
				     "--", ശൂന്യ);
			എങ്കിൽ (run_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       start_head.buf);
				പോകുക finish;
			}
		}
	} അല്ലെങ്കിൽ {
		/* Get the rev from where we start. */
		എങ്കിൽ (!repo_get_oid(the_repository, head, &head_oid) &&
		    !starts_with(head, "refs/heads/")) {
			strbuf_reset(&start_head);
			strbuf_add_oid_hex(&start_head, &head_oid);
		} അല്ലെങ്കിൽ എങ്കിൽ (!repo_get_oid(the_repository, head, &head_oid) &&
			   skip_prefix(head, "refs/heads/", &head)) {
			strbuf_addstr(&start_head, head);
		} അല്ലെങ്കിൽ {
			തിരികെ error(_("bad HEAD - strange symbolic ref"));
		}
	}

	/*
	 * Get rid of any old bisect state.
	 */
	എങ്കിൽ (bisect_clean_state())
		തിരികെ BISECT_FAILED;

	/*
	 * Write new start state
	 */
	write_file(git_path_bisect_start(), "%s\n", start_head.buf);

	എങ്കിൽ (first_parent_only)
		write_file(git_path_bisect_first_parent(), "\n");

	എങ്കിൽ (reset_when_found != RESET_WHEN_FOUND_NONE)
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));

	എങ്കിൽ (no_checkout) {
		എങ്കിൽ (repo_get_oid(the_repository, start_head.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), start_head.buf);
			പോകുക finish;
		}
		എങ്കിൽ (refs_update_ref(get_main_ref_store(the_repository), ശൂന്യ, "BISECT_HEAD", &oid, ശൂന്യ, 0,
				    UPDATE_REFS_സന്ദേശം_ON_ERR)) {
			res = BISECT_FAILED;
			പോകുക finish;
		}
	}

	എങ്കിൽ (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_names, argv + pathspec_pos);
	write_file(git_path_bisect_names(), "%s\n", bisect_names.buf);

	വേണ്ടി (i = 0; i < states.nr; i++)
		എങ്കിൽ (bisect_write(states.ഇനങ്ങൾ[i].string,
				 revs.ഇനങ്ങൾ[i].string, terms, 1)) {
			res = BISECT_FAILED;
			പോകുക finish;
		}

	എങ്കിൽ (must_write_terms && write_terms(terms->term_bad,
					    terms->term_good)) {
		res = BISECT_FAILED;
		പോകുക finish;
	}

	res = bisect_append_log_quoted(argv);
	എങ്കിൽ (res)
		res = BISECT_FAILED;

finish:
	string_list_clear(&revs, 0);
	string_list_clear(&states, 0);
	strbuf_release(&start_head);
	strbuf_release(&bisect_names);
	എങ്കിൽ (res)
		തിരികെ res;

	res = bisect_auto_next(terms, ശൂന്യ);
	എങ്കിൽ (!is_bisect_success(res))
		bisect_clean_state();
	തിരികെ res;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ file_is_not_empty(സ്ഥിരം അക്ഷരം *path)
{
	തിരികെ !is_empty_or_missing_file(path);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ bisect_autostart(ഘടന bisect_terms *terms)
{
	പൂർണ്ണസംഖ്യ res;
	സ്ഥിരം അക്ഷരം *yesno;

	എങ്കിൽ (file_is_not_empty(git_path_bisect_start()))
		തിരികെ 0;

	fprintf_ln(stderr, _("You need to start by \"git bisect "
			  "start\"\n"));

	എങ്കിൽ (!isatty(STDIN_FILENO))
		തിരികെ -1;

	/*
	 * TRANSLATORS: Make sure to include [Y] and [n] in your
	 * translation. The program will only accept English input
	 * at this point.
	 */
	yesno = git_prompt(_("Do you want me to do it for you "
			     "[Y/n]? "), PROMPT_ECHO);
	res = tolower(*yesno) == 'n' ?
		-1 : bisect_start(terms, 0, empty_strvec);

	തിരികെ res;
}

സ്ഥിതം എണ്ണൽ bisect_error bisect_state(ഘടന bisect_terms *terms, പൂർണ്ണസംഖ്യ argc,
				      സ്ഥിരം അക്ഷരം **argv)
{
	സ്ഥിരം അക്ഷരം *state;
	പൂർണ്ണസംഖ്യ i, verify_expected = 1;
	ഘടന object_id oid, expected;
	ഘടന oid_array revs = OID_ARRAY_INIT;

	എങ്കിൽ (!argc)
		തിരികെ error(_("Please call `--bisect-state` with at least one argument"));

	എങ്കിൽ (bisect_autostart(terms))
		തിരികെ BISECT_FAILED;

	state = argv[0];
	എങ്കിൽ (check_and_set_terms(terms, state) ||
	    !one_of(state, terms->term_good, terms->term_bad, "skip", ശൂന്യ))
		തിരികെ BISECT_FAILED;

	argv++;
	argc--;
	എങ്കിൽ (argc > 1 && !strcmp(state, terms->term_bad))
		തിരികെ error(_("'git bisect %s' can take only one argument."), terms->term_bad);

	എങ്കിൽ (argc == 0) {
		സ്ഥിരം അക്ഷരം *head = "BISECT_HEAD";
		എണ്ണൽ get_oid_result res_head = repo_get_oid(the_repository,
							    head, &oid);

		എങ്കിൽ (res_head == MISSING_OBJECT) {
			head = "HEAD";
			res_head = repo_get_oid(the_repository, head, &oid);
		}

		എങ്കിൽ (res_head)
			error(_("Bad rev input: %s"), head);
		oid_array_append(&revs, &oid);
	}

	/*
	 * All input revs must be checked before executing bisect_write()
	 * to discard junk revs.
	 */

	വേണ്ടി (; argc; argc--, argv++) {
		ഘടന commit *commit;

		എങ്കിൽ (repo_get_oid(the_repository, *argv, &oid)){
			error(_("Bad rev input: %s"), *argv);
			oid_array_clear(&revs);
			തിരികെ BISECT_FAILED;
		}

		commit = lookup_commit_reference(the_repository, &oid);
		എങ്കിൽ (!commit)
			die(_("Bad rev input (not a commit): %s"), *argv);

		oid_array_append(&revs, &commit->object.oid);
	}

	എങ്കിൽ (refs_read_ref(get_main_ref_store(the_repository), "BISECT_EXPECTED_REV", &expected))
		verify_expected = 0; /* Ignore invalid file contents */

	വേണ്ടി (i = 0; i < revs.nr; i++) {
		എങ്കിൽ (bisect_write(state, oid_to_hex(&revs.oid[i]), terms, 0)) {
			oid_array_clear(&revs);
			തിരികെ BISECT_FAILED;
		}
		എങ്കിൽ (verify_expected && !oideq(&revs.oid[i], &expected)) {
			unlink_or_warn(git_path_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					ശൂന്യ, "BISECT_EXPECTED_REV", ശൂന്യ,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	തിരികെ bisect_auto_next(terms, ശൂന്യ);
}

സ്ഥിതം എണ്ണൽ bisect_error bisect_log(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ fd, status;
	സ്ഥിരം അക്ഷരം* filename = git_path_bisect_log();

	എങ്കിൽ (is_empty_or_missing_file(filename))
		തിരികെ error(_("We are not bisecting."));

	fd = open(filename, O_RDONLY);
	എങ്കിൽ (fd < 0)
		തിരികെ BISECT_FAILED;

	status = copy_fd(fd, STDOUT_FILENO);
	close(fd);
	തിരികെ status ? BISECT_FAILED : BISECT_OK;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ process_replay_വരി(ഘടന bisect_terms *terms, ഘടന strbuf *വരി)
{
	സ്ഥിരം അക്ഷരം *p = വരി->buf + strspn(വരി->buf, " \t");
	അക്ഷരം *word_end, *rev;

	എങ്കിൽ ((!skip_prefix(p, "git bisect", &p) &&
	!skip_prefix(p, "git-bisect", &p)) || !isspace(*p))
		തിരികെ 0;
	p += strspn(p, " \t");

	word_end = (അക്ഷരം *)p + strcspn(p, " \t");
	rev = word_end + strspn(word_end, " \t");
	*word_end = '\0'; /* NUL-terminate the word */

	എങ്കിൽ (get_terms(terms, 1))
		തിരികെ error(_("no terms defined"));
	എങ്കിൽ (check_and_set_terms(terms, p))
		തിരികെ -1;

	എങ്കിൽ (!strcmp(p, "start")) {
		ഘടന strvec argv = STRVEC_INIT;
		പൂർണ്ണസംഖ്യ res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_start(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		തിരികെ res;
	}

	എങ്കിൽ (one_of(p, terms->term_good,
	   terms->term_bad, "skip", ശൂന്യ))
		തിരികെ bisect_write(p, rev, terms, 0);

	എങ്കിൽ (!strcmp(p, "terms")) {
		ഘടന strvec argv = STRVEC_INIT;
		പൂർണ്ണസംഖ്യ res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_terms(terms, argv.nr == 1 ? argv.v[0] : ശൂന്യ);
		strvec_clear(&argv);
		തിരികെ res;
	}
	error(_("'%s'?? what are you talking about?"), p);

	തിരികെ -1;
}

സ്ഥിതം എണ്ണൽ bisect_error bisect_replay(ഘടന bisect_terms *terms, സ്ഥിരം അക്ഷരം *filename)
{
	FILE *fp = ശൂന്യ;
	എണ്ണൽ bisect_error res = BISECT_OK;
	ഘടന strbuf വരി = STRBUF_INIT;

	എങ്കിൽ (is_empty_or_missing_file(filename))
		തിരികെ error(_("cannot read file '%s' for replaying"), filename);

	എങ്കിൽ (bisect_clean_state())
		തിരികെ BISECT_FAILED;

	fp = fopen(filename, "r");
	എങ്കിൽ (!fp)
		തിരികെ BISECT_FAILED;

	ആകുമ്പോൾ ((strbuf_getline(&വരി, fp) != EOF) && !res)
		res = process_replay_വരി(terms, &വരി);

	strbuf_release(&വരി);
	fclose(fp);

	എങ്കിൽ (res)
		തിരികെ BISECT_FAILED;

	തിരികെ bisect_auto_next(terms, ശൂന്യ);
}

സ്ഥിതം എണ്ണൽ bisect_error bisect_skip(ഘടന bisect_terms *terms, പൂർണ്ണസംഖ്യ argc,
				     സ്ഥിരം അക്ഷരം **argv)
{
	പൂർണ്ണസംഖ്യ i;
	എണ്ണൽ bisect_error res;
	ഘടന strvec argv_state = STRVEC_INIT;

	strvec_തള്ളുക(&argv_state, "skip");

	വേണ്ടി (i = 0; i < argc; i++) {
		സ്ഥിരം അക്ഷരം *dotdot = strstr(argv[i], "..");

		എങ്കിൽ (dotdot) {
			ഘടന rev_info revs;
			ഘടന commit *commit;

			repo_init_revisions(the_repository, &revs, ശൂന്യ);
			setup_revisions(2, argv + i - 1, &revs, ശൂന്യ);

			എങ്കിൽ (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			ആകുമ്പോൾ ((commit = get_revision(&revs)) != ശൂന്യ)
				strvec_തള്ളുക(&argv_state,
						oid_to_hex(&commit->object.oid));

			reset_revision_walk();
			release_revisions(&revs);
		} അല്ലെങ്കിൽ {
			strvec_തള്ളുക(&argv_state, argv[i]);
		}
	}
	res = bisect_state(terms, argv_state.nr, argv_state.v);

	strvec_clear(&argv_state);
	തിരികെ res;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ bisect_visualize(ഘടന bisect_terms *terms, പൂർണ്ണസംഖ്യ argc,
			    സ്ഥിരം അക്ഷരം **argv)
{
	ഘടന child_process cmd = CHILD_PROCESS_INIT;
	ഘടന strbuf sb = STRBUF_INIT;

	എങ്കിൽ (bisect_next_check(terms, ശൂന്യ) != 0)
		തിരികെ BISECT_FAILED;

	cmd.no_stdin = 1;
	എങ്കിൽ (!argc) {
		എങ്കിൽ ((getenv("DISPLAY") || getenv("SESSIONNAME") || getenv("MSYSTEM") ||
		     getenv("SECURITYSESSIONID")) && exists_in_PATH("gitk")) {
			strvec_തള്ളുക(&cmd.args, "gitk");
		} അല്ലെങ്കിൽ {
			strvec_തള്ളുക(&cmd.args, "log");
			cmd.git_cmd = 1;
		}
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (argv[0][0] == '-') {
			strvec_തള്ളുക(&cmd.args, "log");
			cmd.git_cmd = 1;
		} അല്ലെങ്കിൽ എങ്കിൽ (strcmp(argv[0], "tig") && !starts_with(argv[0], "git"))
			cmd.git_cmd = 1;

		strvec_pushv(&cmd.args, argv);
	}

	strvec_pushl(&cmd.args, "--bisect", "--", ശൂന്യ);

	strbuf_read_file(&sb, git_path_bisect_names(), 0);
	sq_dequote_to_strvec(sb.buf, &cmd.args);
	strbuf_release(&sb);

	തിരികെ run_command(&cmd);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_first_good(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cb_data)
{
	oidcpy(cb_data, ref->oid);
	തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ do_bisect_run(സ്ഥിരം അക്ഷരം *command)
{
	ഘടന child_process cmd = CHILD_PROCESS_INIT;

	printf(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_തള്ളുക(&cmd.args, command);
	തിരികെ run_command(&cmd);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ verify_good(സ്ഥിരം ഘടന bisect_terms *terms, സ്ഥിരം അക്ഷരം *command)
{
	പൂർണ്ണസംഖ്യ rc;
	എണ്ണൽ bisect_error res;
	ഘടന object_id good_rev;
	ഘടന object_id current_rev;
	അക്ഷരം *good_glob = xstrfmt("%s-*", terms->term_good);
	പൂർണ്ണസംഖ്യ no_checkout = refs_ref_exists(get_main_ref_store(the_repository),
					  "BISECT_HEAD");
	ഘടന refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_first_good, &good_rev, &opts);
	സ്വതന്ത്രം(good_glob);

	എങ്കിൽ (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		തിരികെ -1;

	res = bisect_checkout(&good_rev, no_checkout);
	എങ്കിൽ (res != BISECT_OK)
		തിരികെ -1;

	rc = do_bisect_run(command);

	res = bisect_checkout(&current_rev, no_checkout);
	എങ്കിൽ (res != BISECT_OK)
		തിരികെ -1;

	തിരികെ rc;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ bisect_run(ഘടന bisect_terms *terms, പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv)
{
	പൂർണ്ണസംഖ്യ res = BISECT_OK;
	ഘടന strbuf command = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *reset_when_found_arg;
	സ്ഥിരം അക്ഷരം *new_state;
	പൂർണ്ണസംഖ്യ temporary_stdout_fd, saved_stdout;
	പൂർണ്ണസംഖ്യ is_first_run = 1;
	എണ്ണൽ reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;

	എങ്കിൽ (bisect_next_check(terms, ശൂന്യ))
		തിരികെ BISECT_FAILED;

	എങ്കിൽ (argc && !strcmp(argv[0], "--reset-when-found")) {
		reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
	} അല്ലെങ്കിൽ എങ്കിൽ (argc && skip_prefix(argv[0], "--reset-when-found=",
				    &reset_when_found_arg)) {
		എങ്കിൽ (parse_reset_when_found(reset_when_found_arg,
					   &reset_when_found))
			തിരികെ BISECT_FAILED;
	}

	എങ്കിൽ (reset_when_found != RESET_WHEN_FOUND_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		തിരികെ error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	എങ്കിൽ (reset_when_found != RESET_WHEN_FOUND_NONE) {
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));
		argc--;
		argv++;
	}

	എങ്കിൽ (!argc) {
		error(_("bisect run failed: no command provided."));
		തിരികെ BISECT_FAILED;
	}

	sq_quote_argv(&command, argv);
	strbuf_ltrim(&command);
	ആകുമ്പോൾ (1) {
		res = do_bisect_run(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		എങ്കിൽ (is_first_run && (res == 126 || res == 127)) {
			പൂർണ്ണസംഖ്യ rc = verify_good(terms, command.buf);
			is_first_run = 0;
			എങ്കിൽ (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->term_good);
				res = BISECT_FAILED;
				നിർത്തുക;
			}
			എങ്കിൽ (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->term_good);
				res = BISECT_FAILED;
				നിർത്തുക;
			}
		}

		എങ്കിൽ (res < 0 || 128 <= res) {
			error(_("bisect run failed: exit code %d from"
				" %s is < 0 or >= 128"), res, command.buf);
			നിർത്തുക;
		}

		എങ്കിൽ (res == 125)
			new_state = "skip";
		അല്ലെങ്കിൽ എങ്കിൽ (!res)
			new_state = terms->term_good;
		അല്ലെങ്കിൽ
			new_state = terms->term_bad;

		temporary_stdout_fd = open(git_path_bisect_run(), O_CREAT | O_WRONLY | O_TRUNC, 0666);

		എങ്കിൽ (temporary_stdout_fd < 0) {
			res = error_errno(_("cannot open file '%s' for writing"), git_path_bisect_run());
			നിർത്തുക;
		}

		fflush(stdout);
		saved_stdout = dup(1);
		എങ്കിൽ (saved_stdout < 0 ||
		    dup2(temporary_stdout_fd, 1) < 0) {
			res = error_errno(_("could not duplicate stdout"));
			എങ്കിൽ (saved_stdout >= 0)
				close(saved_stdout);
			close(temporary_stdout_fd);
			നിർത്തുക;
		}

		res = bisect_state(terms, 1, &new_state);

		fflush(stdout);
		dup2(saved_stdout, 1);
		close(saved_stdout);
		close(temporary_stdout_fd);

		print_file_to_stdout(git_path_bisect_run());

		എങ്കിൽ (res == BISECT_ONLY_SKIPPED_ഇടത്)
			error(_("bisect run cannot continue any more"));
		അല്ലെങ്കിൽ എങ്കിൽ (res == BISECT_INTERNAL_SUCCESS_MERGE_BASE) {
			puts(_("bisect run success"));
			res = BISECT_OK;
		} അല്ലെങ്കിൽ എങ്കിൽ (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
			printf(_("bisect found first '%s' commit\n"), terms->term_bad);
		} അല്ലെങ്കിൽ എങ്കിൽ (res) {
			error(_("bisect run failed: 'git bisect %s'"
				" exited with error code %d"), new_state, res);
		} അല്ലെങ്കിൽ {
			തുടരുക;
		}
		നിർത്തുക;
	}

	strbuf_release(&command);
	തിരികെ res;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_bisect__reset(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix UNUSED,
			     ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ res;

	എങ്കിൽ (argc > 1)
		തിരികെ error(_("'%s' requires either no argument or a commit"),
			     "git bisect reset");
	res = bisect_reset(argc ? argv[0] : ശൂന്യ, false);
	എങ്കിൽ (res)
		തിരികെ res;
	തിരികെ bisect_clean_state();
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_bisect__terms(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix UNUSED,
			     ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ res;
	ഘടന bisect_terms terms = { 0 };

	എങ്കിൽ (argc > 1)
		തിരികെ error(_("'%s' requires 0 or 1 argument"),
			     "git bisect terms");
	res = bisect_terms(&terms, argc == 1 ? argv[0] : ശൂന്യ);
	free_terms(&terms);
	തിരികെ res;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_bisect__start(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix UNUSED,
			     ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ res;
	ഘടന bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	res = bisect_start(&terms, argc, argv);
	free_terms(&terms);
	തിരികെ res;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_bisect__next(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv UNUSED, സ്ഥിരം അക്ഷരം *prefix,
			    ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ res;
	ഘടന bisect_terms terms = { 0 };

	എങ്കിൽ (argc)
		തിരികെ error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	എങ്കിൽ (get_terms(&terms, 1))
		തിരികെ error(_("no terms defined"));
	res = bisect_next(&terms, prefix);
	free_terms(&terms);
	തിരികെ res;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_bisect__log(പൂർണ്ണസംഖ്യ argc UNUSED, സ്ഥിരം അക്ഷരം **argv UNUSED,
			   സ്ഥിരം അക്ഷരം *prefix UNUSED,
			   ഘടന repository *repo UNUSED)
{
	തിരികെ bisect_log();
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_bisect__replay(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix UNUSED,
			      ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ res;
	ഘടന bisect_terms terms = { 0 };

	എങ്കിൽ (argc != 1)
		തിരികെ error(_("no logfile given"));
	set_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	തിരികെ res;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_bisect__skip(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix UNUSED,
			    ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ res;
	ഘടന bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	എങ്കിൽ (get_terms(&terms, 1))
		തിരികെ error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	തിരികെ res;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_bisect__visualize(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix UNUSED,
				 ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ res;
	ഘടന bisect_terms terms = { 0 };

	എങ്കിൽ (get_terms(&terms, 1))
		തിരികെ error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	തിരികെ res;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_bisect__run(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix UNUSED,
			   ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ res;
	ഘടന bisect_terms terms = { 0 };

	എങ്കിൽ (!argc)
		തിരികെ error(_("'%s' failed: no command provided."), "git bisect run");
	എങ്കിൽ (get_terms(&terms, 1))
		തിരികെ error(_("no terms defined"));
	res = bisect_run(&terms, argc, argv);
	free_terms(&terms);
	തിരികെ res;
}

പൂർണ്ണസംഖ്യ cmd_bisect(പൂർണ്ണസംഖ്യ argc,
	       സ്ഥിരം അക്ഷരം **argv,
	       സ്ഥിരം അക്ഷരം *prefix,
	       ഘടന repository *repo)
{
	പൂർണ്ണസംഖ്യ res = 0;
	parse_opt_subcommand_fn *fn = ശൂന്യ;
	ഘടന option options[] = {
		OPT_SUBCOMMAND("reset", &fn, cmd_bisect__reset),
		OPT_SUBCOMMAND("terms", &fn, cmd_bisect__terms),
		OPT_SUBCOMMAND("start", &fn, cmd_bisect__start),
		OPT_SUBCOMMAND("next", &fn, cmd_bisect__next),
		OPT_SUBCOMMAND("log", &fn, cmd_bisect__log),
		OPT_SUBCOMMAND("replay", &fn, cmd_bisect__replay),
		OPT_SUBCOMMAND("skip", &fn, cmd_bisect__skip),
		OPT_SUBCOMMAND("visualize", &fn, cmd_bisect__visualize),
		OPT_SUBCOMMAND("view", &fn, cmd_bisect__visualize),
		OPT_SUBCOMMAND("run", &fn, cmd_bisect__run),
		OPT_END()
	};
	argc = parse_options(argc, argv, prefix, options, git_bisect_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);

	എങ്കിൽ (!fn) {
		ഘടന bisect_terms terms = { 0 };

		എങ്കിൽ (!argc)
			usage_സന്ദേശം_opt(_("need a command"), git_bisect_usage, options);

		എങ്കിൽ (!strcmp(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		set_terms(&terms, "bad", "good");
		എങ്കിൽ (get_terms(&terms, 1))
			തിരികെ error(_("no terms defined"));
		എങ്കിൽ (check_and_set_terms(&terms, argv[0]) ||
		    !one_of(argv[0], terms.term_good, terms.term_bad, ശൂന്യ))
			usage_സന്ദേശം_optf(_("unknown command: '%s'"), git_bisect_usage,
				       options, argv[0]);
		അല്ലെങ്കിൽ
			res = bisect_state(&terms, argc, argv);
		free_terms(&terms);
	} അല്ലെങ്കിൽ {
		argc--;
		argv++;
		res = fn(argc, argv, prefix, repo);
	}

	എങ്കിൽ (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		എണ്ണൽ reset_when_found_mode mode;

		എങ്കിൽ (read_reset_when_found(&mode))
			res = BISECT_FAILED;
		അല്ലെങ്കിൽ എങ്കിൽ (mode != RESET_WHEN_FOUND_NONE &&
			 bisect_reset_when_found(mode))
			res = BISECT_FAILED;
	}

	തിരികെ is_bisect_success(res) ? 0 : -res;
}
