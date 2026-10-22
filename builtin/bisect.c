#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "copy.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "bisect.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "strvec.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "oid-array.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "prompt.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "revision.h"

स्थिर GIT_PATH_FUNC(git_path_bisect_terms, "BISECT_TERMS")
स्थिर GIT_PATH_FUNC(git_path_bisect_ancestors_ok, "BISECT_ANCESTORS_OK")
स्थिर GIT_PATH_FUNC(git_path_bisect_start, "BISECT_START")
स्थिर GIT_PATH_FUNC(git_path_bisect_log, "BISECT_LOG")
स्थिर GIT_PATH_FUNC(git_path_bisect_names, "BISECT_NAMES")
स्थिर GIT_PATH_FUNC(git_path_bisect_first_parent, "BISECT_FIRST_PARENT")
स्थिर GIT_PATH_FUNC(git_path_bisect_reset_when_found, "BISECT_RESET_WHEN_FOUND")
स्थिर GIT_PATH_FUNC(git_path_bisect_run, "BISECT_RUN")

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

स्थिर ध्रुव वर्ण * ध्रुव git_bisect_usage[] = {
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
	रिक्त
};

गणना reset_when_found_mode {
	RESET_WHEN_FOUND_NONE,
	RESET_WHEN_FOUND_TO_ORIGINAL,
	RESET_WHEN_FOUND_TO_FOUND,
};

संरचना add_bisect_ref_data {
	संरचना rev_info *revs;
	unsigned पूर्णाङ्क object_flags;
};

संरचना bisect_terms {
	वर्ण *term_good;
	वर्ण *term_bad;
};

स्थिर शून्य free_terms(संरचना bisect_terms *terms)
{
	FREE_AND_NULL(terms->term_good);
	FREE_AND_NULL(terms->term_bad);
}

स्थिर शून्य set_terms(संरचना bisect_terms *terms, ध्रुव वर्ण *bad,
		      ध्रुव वर्ण *good)
{
	मुक्त((शून्य *)terms->term_good);
	terms->term_good = xstrdup(good);
	मुक्त((शून्य *)terms->term_bad);
	terms->term_bad = xstrdup(bad);
}

स्थिर ध्रुव वर्ण vocab_bad[] = "bad|new";
स्थिर ध्रुव वर्ण vocab_good[] = "good|old";

स्थिर पूर्णाङ्क bisect_autostart(संरचना bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
LAST_ARG_MUST_BE_NULL
स्थिर पूर्णाङ्क one_of(ध्रुव वर्ण *term, ...)
{
	पूर्णाङ्क res = 0;
	va_list matches;
	ध्रुव वर्ण *match;

	va_start(matches, term);
	जाव (!res && (match = va_arg(matches, ध्रुव वर्ण *)))
		res = !strcmp(term, match);
	va_end(matches);

	प्रत्यागम res;
}

/*
 * return code BISECT_INTERNAL_SUCCESS_MERGE_BASE
 * and BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND are codes
 * that indicate special success.
 */

स्थिर पूर्णाङ्क is_bisect_success(गणना bisect_error res)
{
	प्रत्यागम !res ||
		res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND ||
		res == BISECT_INTERNAL_SUCCESS_MERGE_BASE;
}

स्थिर पूर्णाङ्क write_in_file(ध्रुव वर्ण *path, ध्रुव वर्ण *mode, ध्रुव वर्ण *format, va_list args)
{
	FILE *fp = रिक्त;
	पूर्णाङ्क res = 0;

	जइ (strcmp(mode, "w") && strcmp(mode, "a"))
		BUG("write-in-file does not support '%s' mode", mode);
	fp = fopen(path, mode);
	जइ (!fp)
		प्रत्यागम error_errno(_("cannot open file '%s' in mode '%s'"), path, mode);
	res = vfprintf(fp, format, args);

	जइ (res < 0) {
		पूर्णाङ्क saved_errno = errno;
		fclose(fp);
		errno = saved_errno;
		प्रत्यागम error_errno(_("could not write to file '%s'"), path);
	}

	प्रत्यागम fclose(fp);
}

__attribute__((format (printf, 2, 3)))
स्थिर पूर्णाङ्क write_to_file(ध्रुव वर्ण *path, ध्रुव वर्ण *format, ...)
{
	पूर्णाङ्क res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "w", format, args);
	va_end(args);

	प्रत्यागम res;
}

__attribute__((format (printf, 2, 3)))
स्थिर पूर्णाङ्क append_to_file(ध्रुव वर्ण *path, ध्रुव वर्ण *format, ...)
{
	पूर्णाङ्क res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "a", format, args);
	va_end(args);

	प्रत्यागम res;
}

स्थिर पूर्णाङ्क print_file_to_stdout(ध्रुव वर्ण *path)
{
	पूर्णाङ्क fd = open(path, O_RDONLY);
	पूर्णाङ्क ret = 0;

	जइ (fd < 0)
		प्रत्यागम error_errno(_("cannot open file '%s' for reading"), path);
	जइ (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), path);
	close(fd);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क check_term_format(ध्रुव वर्ण *term, ध्रुव वर्ण *orig_term)
{
	पूर्णाङ्क res;
	वर्ण *new_term = xstrfmt("refs/bisect/%s", term);

	res = check_refname_format(new_term, 0);
	मुक्त(new_term);

	जइ (res)
		प्रत्यागम error(_("'%s' is not a valid term"), term);

	जइ (one_of(term, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", रिक्त))
		प्रत्यागम error(_("can't use the builtin command '%s' as a term"), term);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	जइ ((strcmp(orig_term, "bad") && one_of(term, "bad", "new", रिक्त)) ||
		 (strcmp(orig_term, "good") && one_of(term, "good", "old", रिक्त)))
		प्रत्यागम error(_("can't change the meaning of the term '%s'"), term);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क write_terms(ध्रुव वर्ण *bad, ध्रुव वर्ण *good)
{
	पूर्णाङ्क res;

	जइ (!strcmp(bad, good))
		प्रत्यागम error(_("please use two different terms"));

	जइ (check_term_format(bad, "bad") || check_term_format(good, "good"))
		प्रत्यागम -1;

	res = write_to_file(git_path_bisect_terms(), "%s\n%s\n", bad, good);

	प्रत्यागम res;
}

स्थिर पूर्णाङ्क bisect_reset(ध्रुव वर्ण *commit, bool quiet)
{
	संरचना strbuf branch = STRBUF_INIT;

	जइ (!commit) {
		जइ (!strbuf_read_file(&branch, git_path_bisect_start(), 0))
			printf(_("We are not bisecting.\n"));
		अण्णहा
			strbuf_rtrim(&branch);
	} अण्णहा {
		संरचना object_id oid;

		जइ (repo_get_oid_commit(the_repository, commit, &oid))
			प्रत्यागम error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	जइ (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		संरचना child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "checkout", "--ignore-other-worktrees", रिक्त);
		जइ (quiet)
			strvec_प्रक्षिप(&cmd.args, "--quiet");
		strvec_pushl(&cmd.args, branch.buf, "--", रिक्त);
		जइ (run_command(&cmd)) {
			error(_("could not check out original"
				" HEAD '%s'. Try 'git bisect"
				" reset <commit>'."), branch.buf);
			strbuf_release(&branch);
			प्रत्यागम -1;
		}
	}

	strbuf_release(&branch);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क parse_reset_when_found(ध्रुव वर्ण *मानम्,
				  गणना reset_when_found_mode *mode)
{
	जइ (!strcmp(मानम्, "original"))
		*mode = RESET_WHEN_FOUND_TO_ORIGINAL;
	अण्णहा जइ (!strcmp(मानम्, "found"))
		*mode = RESET_WHEN_FOUND_TO_FOUND;
	अण्णहा
		प्रत्यागम error(_("invalid value for '--reset-when-found': '%s'"),
			     मानम्);

	प्रत्यागम 0;
}

स्थिर ध्रुव वर्ण *reset_when_found_mode_name(गणना reset_when_found_mode mode)
{
	विकल्प (mode) {
	पक्ष RESET_WHEN_FOUND_TO_ORIGINAL:
		प्रत्यागम "original";
	पक्ष RESET_WHEN_FOUND_TO_FOUND:
		प्रत्यागम "found";
	पक्ष RESET_WHEN_FOUND_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", mode);
}

स्थिर पूर्णाङ्क read_reset_when_found(गणना reset_when_found_mode *mode)
{
	संरचना strbuf मानम् = STRBUF_INIT;
	पूर्णाङ्क res = 0;

	*mode = RESET_WHEN_FOUND_NONE;
	जइ (is_empty_or_missing_file(git_path_bisect_reset_when_found()))
		प्रत्यागम 0;

	जइ (strbuf_read_file(&मानम्, git_path_bisect_reset_when_found(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_path_bisect_reset_when_found());
		गच्छ out;
	}
	strbuf_trim(&मानम्);
	जइ (parse_reset_when_found(मानम्.buf, mode))
		res = -1;

out:
	strbuf_release(&मानम्);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क bisect_reset_when_found(गणना reset_when_found_mode mode)
{
	संरचना bisect_terms terms = { 0 };
	वर्ण *commit = रिक्त;
	पूर्णाङ्क res;

	जइ (mode == RESET_WHEN_FOUND_TO_FOUND) {
		read_bisect_terms(&terms.term_bad, &terms.term_good);
		commit = xstrfmt("refs/bisect/%s", terms.term_bad);
	} अण्णहा जइ (mode == RESET_WHEN_FOUND_NONE) {
		BUG("automatic reset requested without a reset mode");
	}

	res = bisect_reset(commit, true);
	जइ (!res)
		res = bisect_clean_state();

	मुक्त(commit);
	free_terms(&terms);
	प्रत्यागम res;
}

स्थिर शून्य log_commit(FILE *fp,
		       ध्रुव वर्ण *fmt, ध्रुव वर्ण *state,
		       संरचना commit *commit)
{
	संरचना pretty_print_context pp = {0};
	संरचना strbuf commit_सन्देशः = STRBUF_INIT;
	वर्ण *label = xstrfmt(fmt, state);

	repo_format_commit_message(the_repository, commit, "%s", &commit_सन्देशः,
				   &pp);

	fprintf(fp, "# %s: [%s] %s\n", label, oid_to_hex(&commit->object.oid),
		commit_सन्देशः.buf);

	strbuf_release(&commit_सन्देशः);
	मुक्त(label);
}

स्थिर पूर्णाङ्क bisect_write(ध्रुव वर्ण *state, ध्रुव वर्ण *rev,
			ध्रुव संरचना bisect_terms *terms, पूर्णाङ्क nolog)
{
	संरचना strbuf tag = STRBUF_INIT;
	संरचना object_id oid;
	संरचना commit *commit;
	FILE *fp = रिक्त;
	पूर्णाङ्क res = 0;

	जइ (!strcmp(state, terms->term_bad)) {
		strbuf_addf(&tag, "refs/bisect/%s", state);
	} अण्णहा जइ (one_of(state, terms->term_good, "skip", रिक्त)) {
		strbuf_addf(&tag, "refs/bisect/%s-%s", state, rev);
	} अण्णहा {
		res = error(_("Bad bisect_write argument: %s"), state);
		गच्छ finish;
	}

	जइ (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		गच्छ finish;
	}

	जइ (refs_update_ref(get_main_ref_store(the_repository), रिक्त, tag.buf, &oid, रिक्त, 0,
			    UPDATE_REFS_सन्देशः_ON_ERR)) {
		res = -1;
		गच्छ finish;
	}

	fp = fopen(git_path_bisect_log(), "a");
	जइ (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_path_bisect_log());
		गच्छ finish;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	जइ (!nolog)
		fprintf(fp, "git bisect %s %s\n", state, rev);

finish:
	जइ (fp)
		fclose(fp);
	strbuf_release(&tag);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क check_and_set_terms(संरचना bisect_terms *terms, ध्रुव वर्ण *cmd)
{
	पूर्णाङ्क has_term_file = !is_empty_or_missing_file(git_path_bisect_terms());

	जइ (one_of(cmd, "skip", "start", "terms", रिक्त))
		प्रत्यागम 0;

	जइ (has_term_file && strcmp(cmd, terms->term_bad) &&
	    strcmp(cmd, terms->term_good))
		प्रत्यागम error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->term_bad,
				terms->term_good);

	जइ (!has_term_file) {
		जइ (one_of(cmd, "bad", "good", रिक्त)) {
			set_terms(terms, "bad", "good");
			प्रत्यागम write_terms(terms->term_bad, terms->term_good);
		}
		जइ (one_of(cmd, "new", "old", रिक्त)) {
			set_terms(terms, "new", "old");
			प्रत्यागम write_terms(terms->term_bad, terms->term_good);
		}
	}

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क inc_nr(ध्रुव संरचना reference *ref UNUSED, शून्य *cb_data)
{
	unsigned पूर्णाङ्क *nr = (unsigned पूर्णाङ्क *)cb_data;
	(*nr)++;
	प्रत्यागम 0;
}

स्थिर ध्रुव वर्ण need_bad_and_good_revision_warning[] =
	N_("You need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

स्थिर ध्रुव वर्ण need_bisect_start_warning[] =
	N_("You need to start by \"git bisect start\".\n"
	   "You then need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

स्थिर पूर्णाङ्क decide_next(ध्रुव संरचना bisect_terms *terms,
		       ध्रुव वर्ण *current_term, पूर्णाङ्क missing_good,
		       पूर्णाङ्क missing_bad)
{
	जइ (!missing_good && !missing_bad)
		प्रत्यागम 0;
	जइ (!current_term)
		प्रत्यागम -1;

	जइ (missing_good && !missing_bad &&
	    !strcmp(current_term, terms->term_good)) {
		वर्ण *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->term_bad);
		जइ (!isatty(0))
			प्रत्यागम 0;
		/*
		 * TRANSLATORS: Make sure to include [Y] and [n] in your
		 * translation. The program will only accept English input
		 * at this point.
		 */
		yesno = git_prompt(_("Are you sure [Y/n]? "), PROMPT_ECHO);
		जइ (starts_with(yesno, "N") || starts_with(yesno, "n"))
			प्रत्यागम -1;
		प्रत्यागम 0;
	}

	जइ (!is_empty_or_missing_file(git_path_bisect_start()))
		प्रत्यागम error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	अण्णहा
		प्रत्यागम error(_(need_bisect_start_warning),
			     vocab_good, vocab_bad, vocab_good, vocab_bad);
}

स्थिर शून्य bisect_status(संरचना bisect_state *state,
			  ध्रुव संरचना bisect_terms *terms)
{
	वर्ण *bad_ref = xstrfmt("refs/bisect/%s", terms->term_bad);
	वर्ण *good_glob = xstrfmt("%s-*", terms->term_good);
	संरचना refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	जइ (refs_ref_exists(get_main_ref_store(the_repository), bad_ref))
		state->nr_bad = 1;

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      inc_nr, &state->nr_good, &opts);

	मुक्त(good_glob);
	मुक्त(bad_ref);
}

__attribute__((format (printf, 1, 2)))
स्थिर शून्य bisect_log_printf(ध्रुव वर्ण *fmt, ...)
{
	संरचना strbuf buf = STRBUF_INIT;
	va_list ap;

	va_start(ap, fmt);
	strbuf_vaddf(&buf, fmt, ap);
	va_end(ap);

	printf("%s", buf.buf);
	append_to_file(git_path_bisect_log(), "# %s", buf.buf);

	strbuf_release(&buf);
}

स्थिर शून्य bisect_print_status(ध्रुव संरचना bisect_terms *terms)
{
	संरचना bisect_state state = { 0 };

	bisect_status(&state, terms);

	/* If we had both, we'd already be started, and shouldn't get here. */
	जइ (state.nr_good && state.nr_bad)
		प्रत्यागम;

	जइ (!state.nr_good && !state.nr_bad)
		bisect_log_printf(_("status: waiting for both '%s' and '%s' commits\n"),
				  terms->term_good, terms->term_bad);
	अण्णहा जइ (state.nr_good)
		bisect_log_printf(Q_("status: waiting for '%s' commit, %d '%s' commit known\n",
				     "status: waiting for '%s' commit, %d '%s' commits known\n",
				     state.nr_good),
				  terms->term_bad, state.nr_good, terms->term_good);
	अण्णहा
		bisect_log_printf(_("status: waiting for '%s' commit(s), '%s' commit known\n"),
				  terms->term_good, terms->term_bad);
}

स्थिर पूर्णाङ्क bisect_next_check(ध्रुव संरचना bisect_terms *terms,
			     ध्रुव वर्ण *current_term)
{
	संरचना bisect_state state = { 0 };
	bisect_status(&state, terms);
	प्रत्यागम decide_next(terms, current_term, !state.nr_good, !state.nr_bad);
}

स्थिर पूर्णाङ्क get_terms(संरचना bisect_terms *terms, पूर्णाङ्क file_missing_is_ok)
{
	संरचना strbuf str = STRBUF_INIT;
	FILE *fp = रिक्त;
	पूर्णाङ्क res = 0;

	fp = fopen(git_path_bisect_terms(), "r");
	जइ (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		गच्छ finish;
	}

	free_terms(terms);
	जइ (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		गच्छ finish;
	}
	terms->term_bad = strbuf_detach(&str, रिक्त);
	जइ (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		FREE_AND_NULL(terms->term_bad);
		गच्छ finish;
	}
	terms->term_good = strbuf_detach(&str, रिक्त);

finish:
	जइ (fp)
		fclose(fp);
	strbuf_release(&str);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क bisect_terms(संरचना bisect_terms *terms, ध्रुव वर्ण *option)
{
	जइ (get_terms(terms, 0))
		प्रत्यागम error(_("no terms defined"));

	जइ (!option) {
		printf(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->term_good, terms->term_bad);
		प्रत्यागम 0;
	}
	जइ (one_of(option, "--term-good", "--term-old", रिक्त))
		printf("%s\n", terms->term_good);
	अण्णहा जइ (one_of(option, "--term-bad", "--term-new", रिक्त))
		printf("%s\n", terms->term_bad);
	अण्णहा
		प्रत्यागम error(_("invalid argument %s for 'git bisect terms'.\n"
			       "Supported options are: "
			       "--term-good|--term-old and "
			       "--term-bad|--term-new."), option);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क bisect_append_log_quoted(ध्रुव वर्ण **argv)
{
	पूर्णाङ्क res = 0;
	FILE *fp = fopen(git_path_bisect_log(), "a");
	संरचना strbuf orig_args = STRBUF_INIT;

	जइ (!fp)
		प्रत्यागम -1;

	जइ (fprintf(fp, "git bisect start") < 1) {
		res = -1;
		गच्छ finish;
	}

	sq_quote_argv(&orig_args, argv);
	जइ (fprintf(fp, "%s\n", orig_args.buf) < 1)
		res = -1;

finish:
	fclose(fp);
	strbuf_release(&orig_args);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क add_bisect_ref(ध्रुव संरचना reference *ref, शून्य *cb)
{
	संरचना add_bisect_ref_data *data = cb;

	add_pending_oid(data->revs, ref->name, ref->oid, data->object_flags);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क prepare_revs(संरचना bisect_terms *terms, संरचना rev_info *revs)
{
	संरचना refs_for_each_ref_options opts = {
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};
	पूर्णाङ्क res = 0;
	संरचना add_bisect_ref_data cb = { revs };
	वर्ण *good = xstrfmt("%s-*", terms->term_good);

	/*
	 * We cannot use terms->term_bad directly in
	 * for_each_glob_ref_in() and we have to append a '*' to it,
	 * otherwise for_each_glob_ref_in() will append '/' and '*'.
	 */
	वर्ण *bad = xstrfmt("%s*", terms->term_bad);

	/*
	 * It is important to reset the flags used by revision walks
	 * as the previous call to bisect_next_all() in turn
	 * sets up a revision walk.
	 */
	reset_revision_walk();
	repo_init_revisions(the_repository, revs, रिक्त);
	setup_revisions(0, रिक्त, revs, रिक्त);

	opts.pattern = bad;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      add_bisect_ref, &cb, &opts);

	cb.object_flags = UNINTERESTING;
	opts.pattern = good;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      add_bisect_ref, &cb, &opts);

	जइ (prepare_revision_walk(revs))
		res = error(_("revision walk setup failed"));

	मुक्त(good);
	मुक्त(bad);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क bisect_skipped_commits(संरचना bisect_terms *terms)
{
	पूर्णाङ्क res;
	FILE *fp = रिक्त;
	संरचना rev_info revs;
	संरचना commit *commit;
	संरचना pretty_print_context pp = {0};
	संरचना strbuf commit_name = STRBUF_INIT;

	res = prepare_revs(terms, &revs);
	जइ (res)
		प्रत्यागम res;

	fp = fopen(git_path_bisect_log(), "a");
	जइ (!fp)
		प्रत्यागम error_errno(_("could not open '%s' for appending"),
				  git_path_bisect_log());

	जइ (fprintf(fp, "# only skipped commits left to test\n") < 0)
		प्रत्यागम error_errno(_("failed to write to '%s'"), git_path_bisect_log());

	जाव ((commit = get_revision(&revs)) != रिक्त) {
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
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क bisect_successful(संरचना bisect_terms *terms)
{
	संरचना object_id oid;
	संरचना commit *commit;
	संरचना pretty_print_context pp = {0};
	संरचना strbuf commit_name = STRBUF_INIT;
	वर्ण *bad_ref = xstrfmt("refs/bisect/%s",terms->term_bad);
	पूर्णाङ्क res;

	refs_read_ref(get_main_ref_store(the_repository), bad_ref, &oid);
	commit = lookup_commit_reference_by_name(bad_ref);
	जइ (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		मुक्त(bad_ref);
		प्रत्यागम BISECT_FAILED;
	}
	repo_format_commit_message(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_to_file(git_path_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->term_bad, oid_to_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	मुक्त(bad_ref);
	प्रत्यागम res;
}

स्थिर गणना bisect_error bisect_next(संरचना bisect_terms *terms,
				     ध्रुव वर्ण *prefix)
{
	गणना bisect_error res;

	जइ (bisect_autostart(terms))
		प्रत्यागम BISECT_FAILED;

	जइ (bisect_next_check(terms, terms->term_good))
		प्रत्यागम BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_next_all(the_repository, prefix);

	जइ (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		res = bisect_successful(terms);
		प्रत्यागम res ? res : BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND;
	} अण्णहा जइ (res == BISECT_ONLY_SKIPPED_वामः) {
		res = bisect_skipped_commits(terms);
		प्रत्यागम res ? res : BISECT_ONLY_SKIPPED_वामः;
	}
	प्रत्यागम res;
}

स्थिर गणना bisect_error bisect_auto_next(संरचना bisect_terms *terms,
					  ध्रुव वर्ण *prefix)
{
	जइ (bisect_next_check(terms, रिक्त)) {
		bisect_print_status(terms);
		प्रत्यागम BISECT_OK;
	}

	प्रत्यागम bisect_next(terms, prefix);
}

स्थिर गणना bisect_error bisect_start(संरचना bisect_terms *terms, पूर्णाङ्क argc,
				      ध्रुव वर्ण **argv)
{
	पूर्णाङ्क no_checkout = 0;
	पूर्णाङ्क first_parent_only = 0;
	पूर्णाङ्क i, has_double_dash = 0, must_write_terms = 0, bad_seen = 0;
	पूर्णाङ्क flags, pathspec_pos;
	गणना bisect_error res = BISECT_OK;
	संरचना string_list revs = STRING_LIST_INIT_DUP;
	संरचना string_list states = STRING_LIST_INIT_DUP;
	संरचना strbuf start_head = STRBUF_INIT;
	संरचना strbuf bisect_names = STRBUF_INIT;
	संरचना object_id head_oid;
	संरचना object_id oid;
	गणना reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;
	ध्रुव वर्ण *head;

	जइ (is_bare_repository(the_repository))
		no_checkout = 1;

	/*
	 * Check for one bad and then some good revisions
	 */
	प्रति (i = 0; i < argc; i++) {
		जइ (!strcmp(argv[i], "--")) {
			has_double_dash = 1;
			भङ्ग;
		}
	}

	प्रति (i = 0; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];
		जइ (!strcmp(argv[i], "--")) {
			भङ्ग;
		} अण्णहा जइ (!strcmp(arg, "--no-checkout")) {
			no_checkout = 1;
		} अण्णहा जइ (!strcmp(arg, "--first-parent")) {
			first_parent_only = 1;
		} अण्णहा जइ (!strcmp(arg, "--reset-when-found")) {
			reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
		} अण्णहा जइ (skip_prefix(arg, "--reset-when-found=", &arg)) {
			जइ (parse_reset_when_found(arg, &reset_when_found)) {
				res = BISECT_FAILED;
				गच्छ finish;
			}
		} अण्णहा जइ (!strcmp(arg, "--term-good") ||
			 !strcmp(arg, "--term-old")) {
			i++;
			जइ (argc <= i)
				प्रत्यागम error(_("'' is not a valid term"));
			must_write_terms = 1;
			मुक्त((शून्य *) terms->term_good);
			terms->term_good = xstrdup(argv[i]);
		} अण्णहा जइ (skip_prefix(arg, "--term-good=", &arg) ||
			   skip_prefix(arg, "--term-old=", &arg)) {
			must_write_terms = 1;
			मुक्त((शून्य *) terms->term_good);
			terms->term_good = xstrdup(arg);
		} अण्णहा जइ (!strcmp(arg, "--term-bad") ||
			 !strcmp(arg, "--term-new")) {
			i++;
			जइ (argc <= i)
				प्रत्यागम error(_("'' is not a valid term"));
			must_write_terms = 1;
			मुक्त((शून्य *) terms->term_bad);
			terms->term_bad = xstrdup(argv[i]);
		} अण्णहा जइ (skip_prefix(arg, "--term-bad=", &arg) ||
			   skip_prefix(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			मुक्त((शून्य *) terms->term_bad);
			terms->term_bad = xstrdup(arg);
		} अण्णहा जइ (starts_with(arg, "--")) {
			प्रत्यागम error(_("unrecognized option: '%s'"), arg);
		} अण्णहा जइ (!get_oidf(&oid, "%s^{commit}", arg)) {
			string_list_append(&revs, oid_to_hex(&oid));
		} अण्णहा जइ (has_double_dash) {
			die(_("'%s' does not appear to be a valid "
			      "revision"), arg);
		} अण्णहा {
			भङ्ग;
		}
	}
	जइ (reset_when_found != RESET_WHEN_FOUND_NONE && no_checkout) {
		res = error(_("options '%s' and '%s' cannot be used together"),
			    "--reset-when-found", "--no-checkout");
		गच्छ finish;
	}
	pathspec_pos = i;

	/*
	 * The user ran "git bisect start <sha1> <sha1>", hence did not
	 * explicitly specify the terms, but we are already starting to
	 * set references named with the default terms, and won't be able
	 * to change afterwards.
	 */
	जइ (revs.nr)
		must_write_terms = 1;
	प्रति (i = 0; i < revs.nr; i++) {
		जइ (bad_seen) {
			string_list_append(&states, terms->term_good);
		} अण्णहा {
			bad_seen = 1;
			string_list_append(&states, terms->term_bad);
		}
	}

	/*
	 * Verify HEAD
	 */
	head = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
				       "HEAD", 0, &head_oid, &flags);
	जइ (!head) {
		जइ (repo_get_oid(the_repository, "HEAD", &head_oid))
			प्रत्यागम error(_("bad HEAD - I need a HEAD"));
		head = "HEAD";
	}

	/*
	 * Check if we are bisecting
	 */
	जइ (!is_empty_or_missing_file(git_path_bisect_start())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&start_head, git_path_bisect_start(), 0);
		strbuf_trim(&start_head);
		जइ (!no_checkout) {
			संरचना child_process cmd = CHILD_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.args, "checkout", start_head.buf,
				     "--", रिक्त);
			जइ (run_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       start_head.buf);
				गच्छ finish;
			}
		}
	} अण्णहा {
		/* Get the rev from where we start. */
		जइ (!repo_get_oid(the_repository, head, &head_oid) &&
		    !starts_with(head, "refs/heads/")) {
			strbuf_reset(&start_head);
			strbuf_add_oid_hex(&start_head, &head_oid);
		} अण्णहा जइ (!repo_get_oid(the_repository, head, &head_oid) &&
			   skip_prefix(head, "refs/heads/", &head)) {
			strbuf_addstr(&start_head, head);
		} अण्णहा {
			प्रत्यागम error(_("bad HEAD - strange symbolic ref"));
		}
	}

	/*
	 * Get rid of any old bisect state.
	 */
	जइ (bisect_clean_state())
		प्रत्यागम BISECT_FAILED;

	/*
	 * Write new start state
	 */
	write_file(git_path_bisect_start(), "%s\n", start_head.buf);

	जइ (first_parent_only)
		write_file(git_path_bisect_first_parent(), "\n");

	जइ (reset_when_found != RESET_WHEN_FOUND_NONE)
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));

	जइ (no_checkout) {
		जइ (repo_get_oid(the_repository, start_head.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), start_head.buf);
			गच्छ finish;
		}
		जइ (refs_update_ref(get_main_ref_store(the_repository), रिक्त, "BISECT_HEAD", &oid, रिक्त, 0,
				    UPDATE_REFS_सन्देशः_ON_ERR)) {
			res = BISECT_FAILED;
			गच्छ finish;
		}
	}

	जइ (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_names, argv + pathspec_pos);
	write_file(git_path_bisect_names(), "%s\n", bisect_names.buf);

	प्रति (i = 0; i < states.nr; i++)
		जइ (bisect_write(states.वस्तूनि[i].string,
				 revs.वस्तूनि[i].string, terms, 1)) {
			res = BISECT_FAILED;
			गच्छ finish;
		}

	जइ (must_write_terms && write_terms(terms->term_bad,
					    terms->term_good)) {
		res = BISECT_FAILED;
		गच्छ finish;
	}

	res = bisect_append_log_quoted(argv);
	जइ (res)
		res = BISECT_FAILED;

finish:
	string_list_clear(&revs, 0);
	string_list_clear(&states, 0);
	strbuf_release(&start_head);
	strbuf_release(&bisect_names);
	जइ (res)
		प्रत्यागम res;

	res = bisect_auto_next(terms, रिक्त);
	जइ (!is_bisect_success(res))
		bisect_clean_state();
	प्रत्यागम res;
}

स्थिर अन्तःस्थ पूर्णाङ्क file_is_not_empty(ध्रुव वर्ण *path)
{
	प्रत्यागम !is_empty_or_missing_file(path);
}

स्थिर पूर्णाङ्क bisect_autostart(संरचना bisect_terms *terms)
{
	पूर्णाङ्क res;
	ध्रुव वर्ण *yesno;

	जइ (file_is_not_empty(git_path_bisect_start()))
		प्रत्यागम 0;

	fprintf_ln(stderr, _("You need to start by \"git bisect "
			  "start\"\n"));

	जइ (!isatty(STDIN_FILENO))
		प्रत्यागम -1;

	/*
	 * TRANSLATORS: Make sure to include [Y] and [n] in your
	 * translation. The program will only accept English input
	 * at this point.
	 */
	yesno = git_prompt(_("Do you want me to do it for you "
			     "[Y/n]? "), PROMPT_ECHO);
	res = tolower(*yesno) == 'n' ?
		-1 : bisect_start(terms, 0, empty_strvec);

	प्रत्यागम res;
}

स्थिर गणना bisect_error bisect_state(संरचना bisect_terms *terms, पूर्णाङ्क argc,
				      ध्रुव वर्ण **argv)
{
	ध्रुव वर्ण *state;
	पूर्णाङ्क i, verify_expected = 1;
	संरचना object_id oid, expected;
	संरचना oid_array revs = OID_ARRAY_INIT;

	जइ (!argc)
		प्रत्यागम error(_("Please call `--bisect-state` with at least one argument"));

	जइ (bisect_autostart(terms))
		प्रत्यागम BISECT_FAILED;

	state = argv[0];
	जइ (check_and_set_terms(terms, state) ||
	    !one_of(state, terms->term_good, terms->term_bad, "skip", रिक्त))
		प्रत्यागम BISECT_FAILED;

	argv++;
	argc--;
	जइ (argc > 1 && !strcmp(state, terms->term_bad))
		प्रत्यागम error(_("'git bisect %s' can take only one argument."), terms->term_bad);

	जइ (argc == 0) {
		ध्रुव वर्ण *head = "BISECT_HEAD";
		गणना get_oid_result res_head = repo_get_oid(the_repository,
							    head, &oid);

		जइ (res_head == MISSING_OBJECT) {
			head = "HEAD";
			res_head = repo_get_oid(the_repository, head, &oid);
		}

		जइ (res_head)
			error(_("Bad rev input: %s"), head);
		oid_array_append(&revs, &oid);
	}

	/*
	 * All input revs must be checked before executing bisect_write()
	 * to discard junk revs.
	 */

	प्रति (; argc; argc--, argv++) {
		संरचना commit *commit;

		जइ (repo_get_oid(the_repository, *argv, &oid)){
			error(_("Bad rev input: %s"), *argv);
			oid_array_clear(&revs);
			प्रत्यागम BISECT_FAILED;
		}

		commit = lookup_commit_reference(the_repository, &oid);
		जइ (!commit)
			die(_("Bad rev input (not a commit): %s"), *argv);

		oid_array_append(&revs, &commit->object.oid);
	}

	जइ (refs_read_ref(get_main_ref_store(the_repository), "BISECT_EXPECTED_REV", &expected))
		verify_expected = 0; /* Ignore invalid file contents */

	प्रति (i = 0; i < revs.nr; i++) {
		जइ (bisect_write(state, oid_to_hex(&revs.oid[i]), terms, 0)) {
			oid_array_clear(&revs);
			प्रत्यागम BISECT_FAILED;
		}
		जइ (verify_expected && !oideq(&revs.oid[i], &expected)) {
			unlink_or_warn(git_path_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					रिक्त, "BISECT_EXPECTED_REV", रिक्त,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	प्रत्यागम bisect_auto_next(terms, रिक्त);
}

स्थिर गणना bisect_error bisect_log(शून्य)
{
	पूर्णाङ्क fd, status;
	ध्रुव वर्ण* filename = git_path_bisect_log();

	जइ (is_empty_or_missing_file(filename))
		प्रत्यागम error(_("We are not bisecting."));

	fd = open(filename, O_RDONLY);
	जइ (fd < 0)
		प्रत्यागम BISECT_FAILED;

	status = copy_fd(fd, STDOUT_FILENO);
	close(fd);
	प्रत्यागम status ? BISECT_FAILED : BISECT_OK;
}

स्थिर पूर्णाङ्क process_replay_पङ्क्तिः(संरचना bisect_terms *terms, संरचना strbuf *पङ्क्तिः)
{
	ध्रुव वर्ण *p = पङ्क्तिः->buf + strspn(पङ्क्तिः->buf, " \t");
	वर्ण *word_end, *rev;

	जइ ((!skip_prefix(p, "git bisect", &p) &&
	!skip_prefix(p, "git-bisect", &p)) || !isspace(*p))
		प्रत्यागम 0;
	p += strspn(p, " \t");

	word_end = (वर्ण *)p + strcspn(p, " \t");
	rev = word_end + strspn(word_end, " \t");
	*word_end = '\0'; /* NUL-terminate the word */

	जइ (get_terms(terms, 1))
		प्रत्यागम error(_("no terms defined"));
	जइ (check_and_set_terms(terms, p))
		प्रत्यागम -1;

	जइ (!strcmp(p, "start")) {
		संरचना strvec argv = STRVEC_INIT;
		पूर्णाङ्क res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_start(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		प्रत्यागम res;
	}

	जइ (one_of(p, terms->term_good,
	   terms->term_bad, "skip", रिक्त))
		प्रत्यागम bisect_write(p, rev, terms, 0);

	जइ (!strcmp(p, "terms")) {
		संरचना strvec argv = STRVEC_INIT;
		पूर्णाङ्क res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_terms(terms, argv.nr == 1 ? argv.v[0] : रिक्त);
		strvec_clear(&argv);
		प्रत्यागम res;
	}
	error(_("'%s'?? what are you talking about?"), p);

	प्रत्यागम -1;
}

स्थिर गणना bisect_error bisect_replay(संरचना bisect_terms *terms, ध्रुव वर्ण *filename)
{
	FILE *fp = रिक्त;
	गणना bisect_error res = BISECT_OK;
	संरचना strbuf पङ्क्तिः = STRBUF_INIT;

	जइ (is_empty_or_missing_file(filename))
		प्रत्यागम error(_("cannot read file '%s' for replaying"), filename);

	जइ (bisect_clean_state())
		प्रत्यागम BISECT_FAILED;

	fp = fopen(filename, "r");
	जइ (!fp)
		प्रत्यागम BISECT_FAILED;

	जाव ((strbuf_getline(&पङ्क्तिः, fp) != EOF) && !res)
		res = process_replay_पङ्क्तिः(terms, &पङ्क्तिः);

	strbuf_release(&पङ्क्तिः);
	fclose(fp);

	जइ (res)
		प्रत्यागम BISECT_FAILED;

	प्रत्यागम bisect_auto_next(terms, रिक्त);
}

स्थिर गणना bisect_error bisect_skip(संरचना bisect_terms *terms, पूर्णाङ्क argc,
				     ध्रुव वर्ण **argv)
{
	पूर्णाङ्क i;
	गणना bisect_error res;
	संरचना strvec argv_state = STRVEC_INIT;

	strvec_प्रक्षिप(&argv_state, "skip");

	प्रति (i = 0; i < argc; i++) {
		ध्रुव वर्ण *dotdot = strstr(argv[i], "..");

		जइ (dotdot) {
			संरचना rev_info revs;
			संरचना commit *commit;

			repo_init_revisions(the_repository, &revs, रिक्त);
			setup_revisions(2, argv + i - 1, &revs, रिक्त);

			जइ (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			जाव ((commit = get_revision(&revs)) != रिक्त)
				strvec_प्रक्षिप(&argv_state,
						oid_to_hex(&commit->object.oid));

			reset_revision_walk();
			release_revisions(&revs);
		} अण्णहा {
			strvec_प्रक्षिप(&argv_state, argv[i]);
		}
	}
	res = bisect_state(terms, argv_state.nr, argv_state.v);

	strvec_clear(&argv_state);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क bisect_visualize(संरचना bisect_terms *terms, पूर्णाङ्क argc,
			    ध्रुव वर्ण **argv)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;
	संरचना strbuf sb = STRBUF_INIT;

	जइ (bisect_next_check(terms, रिक्त) != 0)
		प्रत्यागम BISECT_FAILED;

	cmd.no_stdin = 1;
	जइ (!argc) {
		जइ ((getenv("DISPLAY") || getenv("SESSIONNAME") || getenv("MSYSTEM") ||
		     getenv("SECURITYSESSIONID")) && exists_in_PATH("gitk")) {
			strvec_प्रक्षिप(&cmd.args, "gitk");
		} अण्णहा {
			strvec_प्रक्षिप(&cmd.args, "log");
			cmd.git_cmd = 1;
		}
	} अण्णहा {
		जइ (argv[0][0] == '-') {
			strvec_प्रक्षिप(&cmd.args, "log");
			cmd.git_cmd = 1;
		} अण्णहा जइ (strcmp(argv[0], "tig") && !starts_with(argv[0], "git"))
			cmd.git_cmd = 1;

		strvec_pushv(&cmd.args, argv);
	}

	strvec_pushl(&cmd.args, "--bisect", "--", रिक्त);

	strbuf_read_file(&sb, git_path_bisect_names(), 0);
	sq_dequote_to_strvec(sb.buf, &cmd.args);
	strbuf_release(&sb);

	प्रत्यागम run_command(&cmd);
}

स्थिर पूर्णाङ्क get_first_good(ध्रुव संरचना reference *ref, शून्य *cb_data)
{
	oidcpy(cb_data, ref->oid);
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क do_bisect_run(ध्रुव वर्ण *command)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	printf(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_प्रक्षिप(&cmd.args, command);
	प्रत्यागम run_command(&cmd);
}

स्थिर पूर्णाङ्क verify_good(ध्रुव संरचना bisect_terms *terms, ध्रुव वर्ण *command)
{
	पूर्णाङ्क rc;
	गणना bisect_error res;
	संरचना object_id good_rev;
	संरचना object_id current_rev;
	वर्ण *good_glob = xstrfmt("%s-*", terms->term_good);
	पूर्णाङ्क no_checkout = refs_ref_exists(get_main_ref_store(the_repository),
					  "BISECT_HEAD");
	संरचना refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_first_good, &good_rev, &opts);
	मुक्त(good_glob);

	जइ (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		प्रत्यागम -1;

	res = bisect_checkout(&good_rev, no_checkout);
	जइ (res != BISECT_OK)
		प्रत्यागम -1;

	rc = do_bisect_run(command);

	res = bisect_checkout(&current_rev, no_checkout);
	जइ (res != BISECT_OK)
		प्रत्यागम -1;

	प्रत्यागम rc;
}

स्थिर पूर्णाङ्क bisect_run(संरचना bisect_terms *terms, पूर्णाङ्क argc, ध्रुव वर्ण **argv)
{
	पूर्णाङ्क res = BISECT_OK;
	संरचना strbuf command = STRBUF_INIT;
	ध्रुव वर्ण *reset_when_found_arg;
	ध्रुव वर्ण *new_state;
	पूर्णाङ्क temporary_stdout_fd, saved_stdout;
	पूर्णाङ्क is_first_run = 1;
	गणना reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;

	जइ (bisect_next_check(terms, रिक्त))
		प्रत्यागम BISECT_FAILED;

	जइ (argc && !strcmp(argv[0], "--reset-when-found")) {
		reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
	} अण्णहा जइ (argc && skip_prefix(argv[0], "--reset-when-found=",
				    &reset_when_found_arg)) {
		जइ (parse_reset_when_found(reset_when_found_arg,
					   &reset_when_found))
			प्रत्यागम BISECT_FAILED;
	}

	जइ (reset_when_found != RESET_WHEN_FOUND_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		प्रत्यागम error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	जइ (reset_when_found != RESET_WHEN_FOUND_NONE) {
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));
		argc--;
		argv++;
	}

	जइ (!argc) {
		error(_("bisect run failed: no command provided."));
		प्रत्यागम BISECT_FAILED;
	}

	sq_quote_argv(&command, argv);
	strbuf_ltrim(&command);
	जाव (1) {
		res = do_bisect_run(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		जइ (is_first_run && (res == 126 || res == 127)) {
			पूर्णाङ्क rc = verify_good(terms, command.buf);
			is_first_run = 0;
			जइ (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->term_good);
				res = BISECT_FAILED;
				भङ्ग;
			}
			जइ (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->term_good);
				res = BISECT_FAILED;
				भङ्ग;
			}
		}

		जइ (res < 0 || 128 <= res) {
			error(_("bisect run failed: exit code %d from"
				" %s is < 0 or >= 128"), res, command.buf);
			भङ्ग;
		}

		जइ (res == 125)
			new_state = "skip";
		अण्णहा जइ (!res)
			new_state = terms->term_good;
		अण्णहा
			new_state = terms->term_bad;

		temporary_stdout_fd = open(git_path_bisect_run(), O_CREAT | O_WRONLY | O_TRUNC, 0666);

		जइ (temporary_stdout_fd < 0) {
			res = error_errno(_("cannot open file '%s' for writing"), git_path_bisect_run());
			भङ्ग;
		}

		fflush(stdout);
		saved_stdout = dup(1);
		जइ (saved_stdout < 0 ||
		    dup2(temporary_stdout_fd, 1) < 0) {
			res = error_errno(_("could not duplicate stdout"));
			जइ (saved_stdout >= 0)
				close(saved_stdout);
			close(temporary_stdout_fd);
			भङ्ग;
		}

		res = bisect_state(terms, 1, &new_state);

		fflush(stdout);
		dup2(saved_stdout, 1);
		close(saved_stdout);
		close(temporary_stdout_fd);

		print_file_to_stdout(git_path_bisect_run());

		जइ (res == BISECT_ONLY_SKIPPED_वामः)
			error(_("bisect run cannot continue any more"));
		अण्णहा जइ (res == BISECT_INTERNAL_SUCCESS_MERGE_BASE) {
			puts(_("bisect run success"));
			res = BISECT_OK;
		} अण्णहा जइ (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
			printf(_("bisect found first '%s' commit\n"), terms->term_bad);
		} अण्णहा जइ (res) {
			error(_("bisect run failed: 'git bisect %s'"
				" exited with error code %d"), new_state, res);
		} अण्णहा {
			अनुवर्त;
		}
		भङ्ग;
	}

	strbuf_release(&command);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क cmd_bisect__reset(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix UNUSED,
			     संरचना repository *repo UNUSED)
{
	पूर्णाङ्क res;

	जइ (argc > 1)
		प्रत्यागम error(_("'%s' requires either no argument or a commit"),
			     "git bisect reset");
	res = bisect_reset(argc ? argv[0] : रिक्त, false);
	जइ (res)
		प्रत्यागम res;
	प्रत्यागम bisect_clean_state();
}

स्थिर पूर्णाङ्क cmd_bisect__terms(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix UNUSED,
			     संरचना repository *repo UNUSED)
{
	पूर्णाङ्क res;
	संरचना bisect_terms terms = { 0 };

	जइ (argc > 1)
		प्रत्यागम error(_("'%s' requires 0 or 1 argument"),
			     "git bisect terms");
	res = bisect_terms(&terms, argc == 1 ? argv[0] : रिक्त);
	free_terms(&terms);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क cmd_bisect__start(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix UNUSED,
			     संरचना repository *repo UNUSED)
{
	पूर्णाङ्क res;
	संरचना bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	res = bisect_start(&terms, argc, argv);
	free_terms(&terms);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क cmd_bisect__next(पूर्णाङ्क argc, ध्रुव वर्ण **argv UNUSED, ध्रुव वर्ण *prefix,
			    संरचना repository *repo UNUSED)
{
	पूर्णाङ्क res;
	संरचना bisect_terms terms = { 0 };

	जइ (argc)
		प्रत्यागम error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	जइ (get_terms(&terms, 1))
		प्रत्यागम error(_("no terms defined"));
	res = bisect_next(&terms, prefix);
	free_terms(&terms);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क cmd_bisect__log(पूर्णाङ्क argc UNUSED, ध्रुव वर्ण **argv UNUSED,
			   ध्रुव वर्ण *prefix UNUSED,
			   संरचना repository *repo UNUSED)
{
	प्रत्यागम bisect_log();
}

स्थिर पूर्णाङ्क cmd_bisect__replay(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix UNUSED,
			      संरचना repository *repo UNUSED)
{
	पूर्णाङ्क res;
	संरचना bisect_terms terms = { 0 };

	जइ (argc != 1)
		प्रत्यागम error(_("no logfile given"));
	set_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क cmd_bisect__skip(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix UNUSED,
			    संरचना repository *repo UNUSED)
{
	पूर्णाङ्क res;
	संरचना bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	जइ (get_terms(&terms, 1))
		प्रत्यागम error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क cmd_bisect__visualize(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix UNUSED,
				 संरचना repository *repo UNUSED)
{
	पूर्णाङ्क res;
	संरचना bisect_terms terms = { 0 };

	जइ (get_terms(&terms, 1))
		प्रत्यागम error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क cmd_bisect__run(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix UNUSED,
			   संरचना repository *repo UNUSED)
{
	पूर्णाङ्क res;
	संरचना bisect_terms terms = { 0 };

	जइ (!argc)
		प्रत्यागम error(_("'%s' failed: no command provided."), "git bisect run");
	जइ (get_terms(&terms, 1))
		प्रत्यागम error(_("no terms defined"));
	res = bisect_run(&terms, argc, argv);
	free_terms(&terms);
	प्रत्यागम res;
}

पूर्णाङ्क cmd_bisect(पूर्णाङ्क argc,
	       ध्रुव वर्ण **argv,
	       ध्रुव वर्ण *prefix,
	       संरचना repository *repo)
{
	पूर्णाङ्क res = 0;
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option options[] = {
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

	जइ (!fn) {
		संरचना bisect_terms terms = { 0 };

		जइ (!argc)
			usage_सन्देशः_opt(_("need a command"), git_bisect_usage, options);

		जइ (!strcmp(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		set_terms(&terms, "bad", "good");
		जइ (get_terms(&terms, 1))
			प्रत्यागम error(_("no terms defined"));
		जइ (check_and_set_terms(&terms, argv[0]) ||
		    !one_of(argv[0], terms.term_good, terms.term_bad, रिक्त))
			usage_सन्देशः_optf(_("unknown command: '%s'"), git_bisect_usage,
				       options, argv[0]);
		अण्णहा
			res = bisect_state(&terms, argc, argv);
		free_terms(&terms);
	} अण्णहा {
		argc--;
		argv++;
		res = fn(argc, argv, prefix, repo);
	}

	जइ (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		गणना reset_when_found_mode mode;

		जइ (read_reset_when_found(&mode))
			res = BISECT_FAILED;
		अण्णहा जइ (mode != RESET_WHEN_FOUND_NONE &&
			 bisect_reset_when_found(mode))
			res = BISECT_FAILED;
	}

	प्रत्यागम is_bisect_success(res) ? 0 : -res;
}
