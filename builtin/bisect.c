#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "copy.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "object-name.h"
#समावेश "parse-options.h"
#समावेश "bisect.h"
#समावेश "refs.h"
#समावेश "strvec.h"
#समावेश "run-command.h"
#समावेश "oid-array.h"
#समावेश "path.h"
#समावेश "prompt.h"
#समावेश "quote.h"
#समावेश "revision.h"

अटल GIT_PATH_FUNC(git_path_bisect_terms, "BISECT_TERMS")
अटल GIT_PATH_FUNC(git_path_bisect_ancestors_ok, "BISECT_ANCESTORS_OK")
अटल GIT_PATH_FUNC(git_path_bisect_start, "BISECT_START")
अटल GIT_PATH_FUNC(git_path_bisect_log, "BISECT_LOG")
अटल GIT_PATH_FUNC(git_path_bisect_names, "BISECT_NAMES")
अटल GIT_PATH_FUNC(git_path_bisect_first_parent, "BISECT_FIRST_PARENT")
अटल GIT_PATH_FUNC(git_path_bisect_reset_when_found, "BISECT_RESET_WHEN_FOUND")
अटल GIT_PATH_FUNC(git_path_bisect_run, "BISECT_RUN")

#घोषणा BUILTIN_GIT_BISECT_START_USAGE \
	N_("git bisect start [--term-(bad|new)=<term-new> --term-(good|old)=<term-old>]\n" \
	   "                 [--no-checkout] [--first-parent] [--reset-when-found[=<where>]] [<bad> [<good>...]] [--] [<pathspec>...]")
#घोषणा BUILTIN_GIT_BISECT_BAD_USAGE \
	N_("git bisect (bad|new|<term-new>) [<rev>]")
#घोषणा BUILTIN_GIT_BISECT_GOOD_USAGE \
	N_("git bisect (good|old|<term-old>) [<rev>...]")
#घोषणा BUILTIN_GIT_BISECT_TERMS_USAGE \
	"git bisect terms [--term-(good|old) | --term-(bad|new)]"
#घोषणा BUILTIN_GIT_BISECT_SKIP_USAGE \
	N_("git bisect skip [(<rev>|<range>)...]")
#घोषणा BUILTIN_GIT_BISECT_अग्र_USAGE \
	"git bisect next"
#घोषणा BUILTIN_GIT_BISECT_RESET_USAGE \
	N_("git bisect reset [<commit>]")
#घोषणा BUILTIN_GIT_BISECT_VISUALIZE_USAGE \
	"git bisect (visualize|view)"
#घोषणा BUILTIN_GIT_BISECT_REPLAY_USAGE \
	N_("git bisect replay <logfile>")
#घोषणा BUILTIN_GIT_BISECT_LOG_USAGE \
	"git bisect log"
#घोषणा BUILTIN_GIT_BISECT_RUN_USAGE \
	N_("git bisect run [--reset-when-found[=<where>]] <cmd> [<arg>...]")
#घोषणा BUILTIN_GIT_BISECT_HELP_USAGE \
	"git bisect help"

अटल स्थिर अक्षर * स्थिर git_bisect_usage[] = {
	BUILTIN_GIT_BISECT_START_USAGE,
	BUILTIN_GIT_BISECT_BAD_USAGE,
	BUILTIN_GIT_BISECT_GOOD_USAGE,
	BUILTIN_GIT_BISECT_TERMS_USAGE,
	BUILTIN_GIT_BISECT_SKIP_USAGE,
	BUILTIN_GIT_BISECT_अग्र_USAGE,
	BUILTIN_GIT_BISECT_RESET_USAGE,
	BUILTIN_GIT_BISECT_VISUALIZE_USAGE,
	BUILTIN_GIT_BISECT_REPLAY_USAGE,
	BUILTIN_GIT_BISECT_LOG_USAGE,
	BUILTIN_GIT_BISECT_RUN_USAGE,
	BUILTIN_GIT_BISECT_HELP_USAGE,
	शून्य
};

क्रमागत reset_when_found_mode {
	RESET_WHEN_FOUND_NONE,
	RESET_WHEN_FOUND_TO_ORIGINAL,
	RESET_WHEN_FOUND_TO_FOUND,
};

काष्ठा add_bisect_ref_तथ्य {
	काष्ठा rev_info *revs;
	अचिन्हित पूर्णांक object_flags;
};

काष्ठा bisect_terms {
	अक्षर *term_good;
	अक्षर *term_bad;
};

अटल व्योम free_terms(काष्ठा bisect_terms *terms)
{
	FREE_AND_NULL(terms->term_good);
	FREE_AND_NULL(terms->term_bad);
}

अटल व्योम set_terms(काष्ठा bisect_terms *terms, स्थिर अक्षर *bad,
		      स्थिर अक्षर *good)
{
	मुक्त((व्योम *)terms->term_good);
	terms->term_good = xstrdup(good);
	मुक्त((व्योम *)terms->term_bad);
	terms->term_bad = xstrdup(bad);
}

अटल स्थिर अक्षर vocab_bad[] = "bad|new";
अटल स्थिर अक्षर vocab_good[] = "good|old";

अटल पूर्णांक bisect_autostart(काष्ठा bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
LAST_ARG_MUST_BE_NULL
अटल पूर्णांक one_of(स्थिर अक्षर *term, ...)
{
	पूर्णांक res = 0;
	बहु_सूची matches;
	स्थिर अक्षर *match;

	बहु_शुरू(matches, term);
	जबतक (!res && (match = बहु_तर्क(matches, स्थिर अक्षर *)))
		res = !म_भेद(term, match);
	बहु_पूर्ण(matches);

	वापस res;
}

/*
 * return code BISECT_INTERNAL_SUCCESS_MERGE_BASE
 * and BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND are codes
 * that indicate special success.
 */

अटल पूर्णांक is_bisect_success(क्रमागत bisect_error res)
{
	वापस !res ||
		res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND ||
		res == BISECT_INTERNAL_SUCCESS_MERGE_BASE;
}

अटल पूर्णांक write_in_file(स्थिर अक्षर *path, स्थिर अक्षर *mode, स्थिर अक्षर *format, बहु_सूची args)
{
	खाता *fp = शून्य;
	पूर्णांक res = 0;

	अगर (म_भेद(mode, "w") && म_भेद(mode, "a"))
		BUG("write-in-file does not support '%s' mode", mode);
	fp = ख_खोलो(path, mode);
	अगर (!fp)
		वापस error_errno(_("cannot open file '%s' in mode '%s'"), path, mode);
	res = भख_लिखो(fp, format, args);

	अगर (res < 0) {
		पूर्णांक saved_errno = त्रुटि_सं;
		ख_बंद(fp);
		त्रुटि_सं = saved_errno;
		वापस error_errno(_("could not write to file '%s'"), path);
	}

	वापस ख_बंद(fp);
}

__attribute__((format (म_लिखो, 2, 3)))
अटल पूर्णांक write_to_file(स्थिर अक्षर *path, स्थिर अक्षर *format, ...)
{
	पूर्णांक res;
	बहु_सूची args;

	बहु_शुरू(args, format);
	res = write_in_file(path, "w", format, args);
	बहु_पूर्ण(args);

	वापस res;
}

__attribute__((format (म_लिखो, 2, 3)))
अटल पूर्णांक append_to_file(स्थिर अक्षर *path, स्थिर अक्षर *format, ...)
{
	पूर्णांक res;
	बहु_सूची args;

	बहु_शुरू(args, format);
	res = write_in_file(path, "a", format, args);
	बहु_पूर्ण(args);

	वापस res;
}

अटल पूर्णांक print_file_to_stdout(स्थिर अक्षर *path)
{
	पूर्णांक fd = खोलो(path, O_RDONLY);
	पूर्णांक ret = 0;

	अगर (fd < 0)
		वापस error_errno(_("cannot open file '%s' for reading"), path);
	अगर (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), path);
	बंद(fd);
	वापस ret;
}

अटल पूर्णांक check_term_format(स्थिर अक्षर *term, स्थिर अक्षर *orig_term)
{
	पूर्णांक res;
	अक्षर *new_term = xstrfmt("refs/bisect/%s", term);

	res = check_refname_format(new_term, 0);
	मुक्त(new_term);

	अगर (res)
		वापस error(_("'%s' is not a valid term"), term);

	अगर (one_of(term, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", शून्य))
		वापस error(_("can't use the builtin command '%s' as a term"), term);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	अगर ((म_भेद(orig_term, "bad") && one_of(term, "bad", "new", शून्य)) ||
		 (म_भेद(orig_term, "good") && one_of(term, "good", "old", शून्य)))
		वापस error(_("can't change the meaning of the term '%s'"), term);

	वापस 0;
}

अटल पूर्णांक write_terms(स्थिर अक्षर *bad, स्थिर अक्षर *good)
{
	पूर्णांक res;

	अगर (!म_भेद(bad, good))
		वापस error(_("please use two different terms"));

	अगर (check_term_format(bad, "bad") || check_term_format(good, "good"))
		वापस -1;

	res = write_to_file(git_path_bisect_terms(), "%s\n%s\n", bad, good);

	वापस res;
}

अटल पूर्णांक bisect_reset(स्थिर अक्षर *commit, bool quiet)
{
	काष्ठा strbuf branch = STRBUF_INIT;

	अगर (!commit) {
		अगर (!strbuf_read_file(&branch, git_path_bisect_start(), 0))
			म_लिखो(_("We are not bisecting.\n"));
		अन्यथा
			strbuf_rtrim(&branch);
	} अन्यथा {
		काष्ठा object_id oid;

		अगर (repo_get_oid_commit(the_repository, commit, &oid))
			वापस error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	अगर (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		काष्ठा child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "checkout", "--ignore-other-worktrees", शून्य);
		अगर (quiet)
			strvec_धकेलो(&cmd.args, "--quiet");
		strvec_pushl(&cmd.args, branch.buf, "--", शून्य);
		अगर (run_command(&cmd)) {
			error(_("could not check out original"
				" HEAD '%s'. Try 'git bisect"
				" reset <commit>'."), branch.buf);
			strbuf_release(&branch);
			वापस -1;
		}
	}

	strbuf_release(&branch);
	वापस 0;
}

अटल पूर्णांक parse_reset_when_found(स्थिर अक्षर *मूल्य,
				  क्रमागत reset_when_found_mode *mode)
{
	अगर (!म_भेद(मूल्य, "original"))
		*mode = RESET_WHEN_FOUND_TO_ORIGINAL;
	अन्यथा अगर (!म_भेद(मूल्य, "found"))
		*mode = RESET_WHEN_FOUND_TO_FOUND;
	अन्यथा
		वापस error(_("invalid value for '--reset-when-found': '%s'"),
			     मूल्य);

	वापस 0;
}

अटल स्थिर अक्षर *reset_when_found_mode_name(क्रमागत reset_when_found_mode mode)
{
	चयन (mode) {
	हाल RESET_WHEN_FOUND_TO_ORIGINAL:
		वापस "original";
	हाल RESET_WHEN_FOUND_TO_FOUND:
		वापस "found";
	हाल RESET_WHEN_FOUND_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", mode);
}

अटल पूर्णांक read_reset_when_found(क्रमागत reset_when_found_mode *mode)
{
	काष्ठा strbuf मूल्य = STRBUF_INIT;
	पूर्णांक res = 0;

	*mode = RESET_WHEN_FOUND_NONE;
	अगर (is_empty_or_missing_file(git_path_bisect_reset_when_found()))
		वापस 0;

	अगर (strbuf_read_file(&मूल्य, git_path_bisect_reset_when_found(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_path_bisect_reset_when_found());
		जाओ out;
	}
	strbuf_trim(&मूल्य);
	अगर (parse_reset_when_found(मूल्य.buf, mode))
		res = -1;

out:
	strbuf_release(&मूल्य);
	वापस res;
}

अटल पूर्णांक bisect_reset_when_found(क्रमागत reset_when_found_mode mode)
{
	काष्ठा bisect_terms terms = { 0 };
	अक्षर *commit = शून्य;
	पूर्णांक res;

	अगर (mode == RESET_WHEN_FOUND_TO_FOUND) {
		read_bisect_terms(&terms.term_bad, &terms.term_good);
		commit = xstrfmt("refs/bisect/%s", terms.term_bad);
	} अन्यथा अगर (mode == RESET_WHEN_FOUND_NONE) {
		BUG("automatic reset requested without a reset mode");
	}

	res = bisect_reset(commit, true);
	अगर (!res)
		res = bisect_clean_state();

	मुक्त(commit);
	free_terms(&terms);
	वापस res;
}

अटल व्योम log_commit(खाता *fp,
		       स्थिर अक्षर *fmt, स्थिर अक्षर *state,
		       काष्ठा commit *commit)
{
	काष्ठा pretty_print_context pp = {0};
	काष्ठा strbuf commit_संदेश = STRBUF_INIT;
	अक्षर *label = xstrfmt(fmt, state);

	repo_format_commit_message(the_repository, commit, "%s", &commit_संदेश,
				   &pp);

	ख_लिखो(fp, "# %s: [%s] %s\n", label, oid_to_hex(&commit->object.oid),
		commit_संदेश.buf);

	strbuf_release(&commit_संदेश);
	मुक्त(label);
}

अटल पूर्णांक bisect_write(स्थिर अक्षर *state, स्थिर अक्षर *rev,
			स्थिर काष्ठा bisect_terms *terms, पूर्णांक nolog)
{
	काष्ठा strbuf tag = STRBUF_INIT;
	काष्ठा object_id oid;
	काष्ठा commit *commit;
	खाता *fp = शून्य;
	पूर्णांक res = 0;

	अगर (!म_भेद(state, terms->term_bad)) {
		strbuf_addf(&tag, "refs/bisect/%s", state);
	} अन्यथा अगर (one_of(state, terms->term_good, "skip", शून्य)) {
		strbuf_addf(&tag, "refs/bisect/%s-%s", state, rev);
	} अन्यथा {
		res = error(_("Bad bisect_write argument: %s"), state);
		जाओ finish;
	}

	अगर (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		जाओ finish;
	}

	अगर (refs_update_ref(get_main_ref_store(the_repository), शून्य, tag.buf, &oid, शून्य, 0,
			    UPDATE_REFS_संदेश_ON_ERR)) {
		res = -1;
		जाओ finish;
	}

	fp = ख_खोलो(git_path_bisect_log(), "a");
	अगर (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_path_bisect_log());
		जाओ finish;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	अगर (!nolog)
		ख_लिखो(fp, "git bisect %s %s\n", state, rev);

finish:
	अगर (fp)
		ख_बंद(fp);
	strbuf_release(&tag);
	वापस res;
}

अटल पूर्णांक check_and_set_terms(काष्ठा bisect_terms *terms, स्थिर अक्षर *cmd)
{
	पूर्णांक has_term_file = !is_empty_or_missing_file(git_path_bisect_terms());

	अगर (one_of(cmd, "skip", "start", "terms", शून्य))
		वापस 0;

	अगर (has_term_file && म_भेद(cmd, terms->term_bad) &&
	    म_भेद(cmd, terms->term_good))
		वापस error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->term_bad,
				terms->term_good);

	अगर (!has_term_file) {
		अगर (one_of(cmd, "bad", "good", शून्य)) {
			set_terms(terms, "bad", "good");
			वापस write_terms(terms->term_bad, terms->term_good);
		}
		अगर (one_of(cmd, "new", "old", शून्य)) {
			set_terms(terms, "new", "old");
			वापस write_terms(terms->term_bad, terms->term_good);
		}
	}

	वापस 0;
}

अटल पूर्णांक inc_nr(स्थिर काष्ठा reference *ref UNUSED, व्योम *cb_तथ्य)
{
	अचिन्हित पूर्णांक *nr = (अचिन्हित पूर्णांक *)cb_तथ्य;
	(*nr)++;
	वापस 0;
}

अटल स्थिर अक्षर need_bad_and_good_revision_warning[] =
	N_("You need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

अटल स्थिर अक्षर need_bisect_start_warning[] =
	N_("You need to start by \"git bisect start\".\n"
	   "You then need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

अटल पूर्णांक decide_अग्र(स्थिर काष्ठा bisect_terms *terms,
		       स्थिर अक्षर *current_term, पूर्णांक missing_good,
		       पूर्णांक missing_bad)
{
	अगर (!missing_good && !missing_bad)
		वापस 0;
	अगर (!current_term)
		वापस -1;

	अगर (missing_good && !missing_bad &&
	    !म_भेद(current_term, terms->term_good)) {
		अक्षर *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->term_bad);
		अगर (!isatty(0))
			वापस 0;
		/*
		 * TRANSLATORS: Make sure to include [Y] and [n] in your
		 * translation. The program will only accept English input
		 * at this point.
		 */
		yesno = git_prompt(_("Are you sure [Y/n]? "), PROMPT_ECHO);
		अगर (starts_with(yesno, "N") || starts_with(yesno, "n"))
			वापस -1;
		वापस 0;
	}

	अगर (!is_empty_or_missing_file(git_path_bisect_start()))
		वापस error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	अन्यथा
		वापस error(_(need_bisect_start_warning),
			     vocab_good, vocab_bad, vocab_good, vocab_bad);
}

अटल व्योम bisect_status(काष्ठा bisect_state *state,
			  स्थिर काष्ठा bisect_terms *terms)
{
	अक्षर *bad_ref = xstrfmt("refs/bisect/%s", terms->term_bad);
	अक्षर *good_glob = xstrfmt("%s-*", terms->term_good);
	काष्ठा refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = म_माप("refs/bisect/"),
	};

	अगर (refs_ref_exists(get_main_ref_store(the_repository), bad_ref))
		state->nr_bad = 1;

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      inc_nr, &state->nr_good, &opts);

	मुक्त(good_glob);
	मुक्त(bad_ref);
}

__attribute__((format (म_लिखो, 1, 2)))
अटल व्योम bisect_log_printf(स्थिर अक्षर *fmt, ...)
{
	काष्ठा strbuf buf = STRBUF_INIT;
	बहु_सूची ap;

	बहु_शुरू(ap, fmt);
	strbuf_vaddf(&buf, fmt, ap);
	बहु_पूर्ण(ap);

	म_लिखो("%s", buf.buf);
	append_to_file(git_path_bisect_log(), "# %s", buf.buf);

	strbuf_release(&buf);
}

अटल व्योम bisect_print_status(स्थिर काष्ठा bisect_terms *terms)
{
	काष्ठा bisect_state state = { 0 };

	bisect_status(&state, terms);

	/* If we had both, we'd already be started, and shouldn't get here. */
	अगर (state.nr_good && state.nr_bad)
		वापस;

	अगर (!state.nr_good && !state.nr_bad)
		bisect_log_printf(_("status: waiting for both '%s' and '%s' commits\n"),
				  terms->term_good, terms->term_bad);
	अन्यथा अगर (state.nr_good)
		bisect_log_printf(Q_("status: waiting for '%s' commit, %d '%s' commit known\n",
				     "status: waiting for '%s' commit, %d '%s' commits known\n",
				     state.nr_good),
				  terms->term_bad, state.nr_good, terms->term_good);
	अन्यथा
		bisect_log_printf(_("status: waiting for '%s' commit(s), '%s' commit known\n"),
				  terms->term_good, terms->term_bad);
}

अटल पूर्णांक bisect_अग्र_check(स्थिर काष्ठा bisect_terms *terms,
			     स्थिर अक्षर *current_term)
{
	काष्ठा bisect_state state = { 0 };
	bisect_status(&state, terms);
	वापस decide_अग्र(terms, current_term, !state.nr_good, !state.nr_bad);
}

अटल पूर्णांक get_terms(काष्ठा bisect_terms *terms, पूर्णांक file_missing_is_ok)
{
	काष्ठा strbuf str = STRBUF_INIT;
	खाता *fp = शून्य;
	पूर्णांक res = 0;

	fp = ख_खोलो(git_path_bisect_terms(), "r");
	अगर (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		जाओ finish;
	}

	free_terms(terms);
	अगर (strbuf_getline_lf(&str, fp) == खातापूर्ण) {
		res = -1;
		जाओ finish;
	}
	terms->term_bad = strbuf_detach(&str, शून्य);
	अगर (strbuf_getline_lf(&str, fp) == खातापूर्ण) {
		res = -1;
		FREE_AND_NULL(terms->term_bad);
		जाओ finish;
	}
	terms->term_good = strbuf_detach(&str, शून्य);

finish:
	अगर (fp)
		ख_बंद(fp);
	strbuf_release(&str);
	वापस res;
}

अटल पूर्णांक bisect_terms(काष्ठा bisect_terms *terms, स्थिर अक्षर *option)
{
	अगर (get_terms(terms, 0))
		वापस error(_("no terms defined"));

	अगर (!option) {
		म_लिखो(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->term_good, terms->term_bad);
		वापस 0;
	}
	अगर (one_of(option, "--term-good", "--term-old", शून्य))
		म_लिखो("%s\n", terms->term_good);
	अन्यथा अगर (one_of(option, "--term-bad", "--term-new", शून्य))
		म_लिखो("%s\n", terms->term_bad);
	अन्यथा
		वापस error(_("invalid argument %s for 'git bisect terms'.\n"
			       "Supported options are: "
			       "--term-good|--term-old and "
			       "--term-bad|--term-new."), option);

	वापस 0;
}

अटल पूर्णांक bisect_append_log_quoted(स्थिर अक्षर **argv)
{
	पूर्णांक res = 0;
	खाता *fp = ख_खोलो(git_path_bisect_log(), "a");
	काष्ठा strbuf orig_args = STRBUF_INIT;

	अगर (!fp)
		वापस -1;

	अगर (ख_लिखो(fp, "git bisect start") < 1) {
		res = -1;
		जाओ finish;
	}

	sq_quote_argv(&orig_args, argv);
	अगर (ख_लिखो(fp, "%s\n", orig_args.buf) < 1)
		res = -1;

finish:
	ख_बंद(fp);
	strbuf_release(&orig_args);
	वापस res;
}

अटल पूर्णांक add_bisect_ref(स्थिर काष्ठा reference *ref, व्योम *cb)
{
	काष्ठा add_bisect_ref_तथ्य *तथ्य = cb;

	add_pending_oid(तथ्य->revs, ref->name, ref->oid, तथ्य->object_flags);

	वापस 0;
}

अटल पूर्णांक prepare_revs(काष्ठा bisect_terms *terms, काष्ठा rev_info *revs)
{
	काष्ठा refs_for_each_ref_options opts = {
		.prefix = "refs/bisect/",
		.trim_prefix = म_माप("refs/bisect/"),
	};
	पूर्णांक res = 0;
	काष्ठा add_bisect_ref_तथ्य cb = { revs };
	अक्षर *good = xstrfmt("%s-*", terms->term_good);

	/*
	 * We cannot use terms->term_bad directly in
	 * for_each_glob_ref_in() and we have to append a '*' to it,
	 * otherwise for_each_glob_ref_in() will append '/' and '*'.
	 */
	अक्षर *bad = xstrfmt("%s*", terms->term_bad);

	/*
	 * It is important to reset the flags used by revision walks
	 * as the previous call to bisect_next_all() in turn
	 * sets up a revision walk.
	 */
	reset_revision_walk();
	repo_init_revisions(the_repository, revs, शून्य);
	setup_revisions(0, शून्य, revs, शून्य);

	opts.pattern = bad;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      add_bisect_ref, &cb, &opts);

	cb.object_flags = UNINTERESTING;
	opts.pattern = good;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      add_bisect_ref, &cb, &opts);

	अगर (prepare_revision_walk(revs))
		res = error(_("revision walk setup failed"));

	मुक्त(good);
	मुक्त(bad);
	वापस res;
}

अटल पूर्णांक bisect_skipped_commits(काष्ठा bisect_terms *terms)
{
	पूर्णांक res;
	खाता *fp = शून्य;
	काष्ठा rev_info revs;
	काष्ठा commit *commit;
	काष्ठा pretty_print_context pp = {0};
	काष्ठा strbuf commit_name = STRBUF_INIT;

	res = prepare_revs(terms, &revs);
	अगर (res)
		वापस res;

	fp = ख_खोलो(git_path_bisect_log(), "a");
	अगर (!fp)
		वापस error_errno(_("could not open '%s' for appending"),
				  git_path_bisect_log());

	अगर (ख_लिखो(fp, "# only skipped commits left to test\n") < 0)
		वापस error_errno(_("failed to write to '%s'"), git_path_bisect_log());

	जबतक ((commit = get_revision(&revs)) != शून्य) {
		strbuf_reset(&commit_name);
		repo_format_commit_message(the_repository, commit, "%s",
					   &commit_name, &pp);
		ख_लिखो(fp, "# possible first '%s' commit: [%s] %s\n",
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
	ख_बंद(fp);
	वापस 0;
}

अटल पूर्णांक bisect_successful(काष्ठा bisect_terms *terms)
{
	काष्ठा object_id oid;
	काष्ठा commit *commit;
	काष्ठा pretty_print_context pp = {0};
	काष्ठा strbuf commit_name = STRBUF_INIT;
	अक्षर *bad_ref = xstrfmt("refs/bisect/%s",terms->term_bad);
	पूर्णांक res;

	refs_read_ref(get_main_ref_store(the_repository), bad_ref, &oid);
	commit = lookup_commit_reference_by_name(bad_ref);
	अगर (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		मुक्त(bad_ref);
		वापस BISECT_FAILED;
	}
	repo_format_commit_message(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_to_file(git_path_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->term_bad, oid_to_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	मुक्त(bad_ref);
	वापस res;
}

अटल क्रमागत bisect_error bisect_अग्र(काष्ठा bisect_terms *terms,
				     स्थिर अक्षर *prefix)
{
	क्रमागत bisect_error res;

	अगर (bisect_autostart(terms))
		वापस BISECT_FAILED;

	अगर (bisect_अग्र_check(terms, terms->term_good))
		वापस BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_अग्र_all(the_repository, prefix);

	अगर (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		res = bisect_successful(terms);
		वापस res ? res : BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND;
	} अन्यथा अगर (res == BISECT_ONLY_SKIPPED_बायाँ) {
		res = bisect_skipped_commits(terms);
		वापस res ? res : BISECT_ONLY_SKIPPED_बायाँ;
	}
	वापस res;
}

अटल क्रमागत bisect_error bisect_auto_अग्र(काष्ठा bisect_terms *terms,
					  स्थिर अक्षर *prefix)
{
	अगर (bisect_अग्र_check(terms, शून्य)) {
		bisect_print_status(terms);
		वापस BISECT_OK;
	}

	वापस bisect_अग्र(terms, prefix);
}

अटल क्रमागत bisect_error bisect_start(काष्ठा bisect_terms *terms, पूर्णांक argc,
				      स्थिर अक्षर **argv)
{
	पूर्णांक no_checkout = 0;
	पूर्णांक first_parent_only = 0;
	पूर्णांक i, has_double_dash = 0, must_write_terms = 0, bad_seen = 0;
	पूर्णांक flags, pathspec_pos;
	क्रमागत bisect_error res = BISECT_OK;
	काष्ठा string_list revs = STRING_LIST_INIT_DUP;
	काष्ठा string_list states = STRING_LIST_INIT_DUP;
	काष्ठा strbuf start_शीर्ष = STRBUF_INIT;
	काष्ठा strbuf bisect_names = STRBUF_INIT;
	काष्ठा object_id शीर्ष_oid;
	काष्ठा object_id oid;
	क्रमागत reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;
	स्थिर अक्षर *शीर्ष;

	अगर (is_bare_repository(the_repository))
		no_checkout = 1;

	/*
	 * Check for one bad and then some good revisions
	 */
	क्रम (i = 0; i < argc; i++) {
		अगर (!म_भेद(argv[i], "--")) {
			has_double_dash = 1;
			अवरोध;
		}
	}

	क्रम (i = 0; i < argc; i++) {
		स्थिर अक्षर *arg = argv[i];
		अगर (!म_भेद(argv[i], "--")) {
			अवरोध;
		} अन्यथा अगर (!म_भेद(arg, "--no-checkout")) {
			no_checkout = 1;
		} अन्यथा अगर (!म_भेद(arg, "--first-parent")) {
			first_parent_only = 1;
		} अन्यथा अगर (!म_भेद(arg, "--reset-when-found")) {
			reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
		} अन्यथा अगर (skip_prefix(arg, "--reset-when-found=", &arg)) {
			अगर (parse_reset_when_found(arg, &reset_when_found)) {
				res = BISECT_FAILED;
				जाओ finish;
			}
		} अन्यथा अगर (!म_भेद(arg, "--term-good") ||
			 !म_भेद(arg, "--term-old")) {
			i++;
			अगर (argc <= i)
				वापस error(_("'' is not a valid term"));
			must_write_terms = 1;
			मुक्त((व्योम *) terms->term_good);
			terms->term_good = xstrdup(argv[i]);
		} अन्यथा अगर (skip_prefix(arg, "--term-good=", &arg) ||
			   skip_prefix(arg, "--term-old=", &arg)) {
			must_write_terms = 1;
			मुक्त((व्योम *) terms->term_good);
			terms->term_good = xstrdup(arg);
		} अन्यथा अगर (!म_भेद(arg, "--term-bad") ||
			 !म_भेद(arg, "--term-new")) {
			i++;
			अगर (argc <= i)
				वापस error(_("'' is not a valid term"));
			must_write_terms = 1;
			मुक्त((व्योम *) terms->term_bad);
			terms->term_bad = xstrdup(argv[i]);
		} अन्यथा अगर (skip_prefix(arg, "--term-bad=", &arg) ||
			   skip_prefix(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			मुक्त((व्योम *) terms->term_bad);
			terms->term_bad = xstrdup(arg);
		} अन्यथा अगर (starts_with(arg, "--")) {
			वापस error(_("unrecognized option: '%s'"), arg);
		} अन्यथा अगर (!get_oidf(&oid, "%s^{commit}", arg)) {
			string_list_append(&revs, oid_to_hex(&oid));
		} अन्यथा अगर (has_double_dash) {
			die(_("'%s' does not appear to be a valid "
			      "revision"), arg);
		} अन्यथा {
			अवरोध;
		}
	}
	अगर (reset_when_found != RESET_WHEN_FOUND_NONE && no_checkout) {
		res = error(_("options '%s' and '%s' cannot be used together"),
			    "--reset-when-found", "--no-checkout");
		जाओ finish;
	}
	pathspec_pos = i;

	/*
	 * The user ran "git bisect start <sha1> <sha1>", hence did not
	 * explicitly specify the terms, but we are already starting to
	 * set references named with the default terms, and won't be able
	 * to change afterwards.
	 */
	अगर (revs.nr)
		must_write_terms = 1;
	क्रम (i = 0; i < revs.nr; i++) {
		अगर (bad_seen) {
			string_list_append(&states, terms->term_good);
		} अन्यथा {
			bad_seen = 1;
			string_list_append(&states, terms->term_bad);
		}
	}

	/*
	 * Verify HEAD
	 */
	शीर्ष = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
				       "HEAD", 0, &शीर्ष_oid, &flags);
	अगर (!शीर्ष) {
		अगर (repo_get_oid(the_repository, "HEAD", &शीर्ष_oid))
			वापस error(_("bad HEAD - I need a HEAD"));
		शीर्ष = "HEAD";
	}

	/*
	 * Check if we are bisecting
	 */
	अगर (!is_empty_or_missing_file(git_path_bisect_start())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&start_शीर्ष, git_path_bisect_start(), 0);
		strbuf_trim(&start_शीर्ष);
		अगर (!no_checkout) {
			काष्ठा child_process cmd = CHILD_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.args, "checkout", start_शीर्ष.buf,
				     "--", शून्य);
			अगर (run_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       start_शीर्ष.buf);
				जाओ finish;
			}
		}
	} अन्यथा {
		/* Get the rev from where we start. */
		अगर (!repo_get_oid(the_repository, शीर्ष, &शीर्ष_oid) &&
		    !starts_with(शीर्ष, "refs/heads/")) {
			strbuf_reset(&start_शीर्ष);
			strbuf_add_oid_hex(&start_शीर्ष, &शीर्ष_oid);
		} अन्यथा अगर (!repo_get_oid(the_repository, शीर्ष, &शीर्ष_oid) &&
			   skip_prefix(शीर्ष, "refs/heads/", &शीर्ष)) {
			strbuf_addstr(&start_शीर्ष, शीर्ष);
		} अन्यथा {
			वापस error(_("bad HEAD - strange symbolic ref"));
		}
	}

	/*
	 * Get rid of any old bisect state.
	 */
	अगर (bisect_clean_state())
		वापस BISECT_FAILED;

	/*
	 * Write new start state
	 */
	write_file(git_path_bisect_start(), "%s\n", start_शीर्ष.buf);

	अगर (first_parent_only)
		write_file(git_path_bisect_first_parent(), "\n");

	अगर (reset_when_found != RESET_WHEN_FOUND_NONE)
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));

	अगर (no_checkout) {
		अगर (repo_get_oid(the_repository, start_शीर्ष.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), start_शीर्ष.buf);
			जाओ finish;
		}
		अगर (refs_update_ref(get_main_ref_store(the_repository), शून्य, "BISECT_HEAD", &oid, शून्य, 0,
				    UPDATE_REFS_संदेश_ON_ERR)) {
			res = BISECT_FAILED;
			जाओ finish;
		}
	}

	अगर (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_names, argv + pathspec_pos);
	write_file(git_path_bisect_names(), "%s\n", bisect_names.buf);

	क्रम (i = 0; i < states.nr; i++)
		अगर (bisect_write(states.वस्तुएँ[i].string,
				 revs.वस्तुएँ[i].string, terms, 1)) {
			res = BISECT_FAILED;
			जाओ finish;
		}

	अगर (must_write_terms && write_terms(terms->term_bad,
					    terms->term_good)) {
		res = BISECT_FAILED;
		जाओ finish;
	}

	res = bisect_append_log_quoted(argv);
	अगर (res)
		res = BISECT_FAILED;

finish:
	string_list_clear(&revs, 0);
	string_list_clear(&states, 0);
	strbuf_release(&start_शीर्ष);
	strbuf_release(&bisect_names);
	अगर (res)
		वापस res;

	res = bisect_auto_अग्र(terms, शून्य);
	अगर (!is_bisect_success(res))
		bisect_clean_state();
	वापस res;
}

अटल अंतरभूत पूर्णांक file_is_not_empty(स्थिर अक्षर *path)
{
	वापस !is_empty_or_missing_file(path);
}

अटल पूर्णांक bisect_autostart(काष्ठा bisect_terms *terms)
{
	पूर्णांक res;
	स्थिर अक्षर *yesno;

	अगर (file_is_not_empty(git_path_bisect_start()))
		वापस 0;

	fprintf_ln(मानक_त्रुटि, _("You need to start by \"git bisect "
			  "start\"\n"));

	अगर (!isatty(STDIN_FILENO))
		वापस -1;

	/*
	 * TRANSLATORS: Make sure to include [Y] and [n] in your
	 * translation. The program will only accept English input
	 * at this point.
	 */
	yesno = git_prompt(_("Do you want me to do it for you "
			     "[Y/n]? "), PROMPT_ECHO);
	res = छोटे(*yesno) == 'n' ?
		-1 : bisect_start(terms, 0, empty_strvec);

	वापस res;
}

अटल क्रमागत bisect_error bisect_state(काष्ठा bisect_terms *terms, पूर्णांक argc,
				      स्थिर अक्षर **argv)
{
	स्थिर अक्षर *state;
	पूर्णांक i, verify_expected = 1;
	काष्ठा object_id oid, expected;
	काष्ठा oid_array revs = OID_ARRAY_INIT;

	अगर (!argc)
		वापस error(_("Please call `--bisect-state` with at least one argument"));

	अगर (bisect_autostart(terms))
		वापस BISECT_FAILED;

	state = argv[0];
	अगर (check_and_set_terms(terms, state) ||
	    !one_of(state, terms->term_good, terms->term_bad, "skip", शून्य))
		वापस BISECT_FAILED;

	argv++;
	argc--;
	अगर (argc > 1 && !म_भेद(state, terms->term_bad))
		वापस error(_("'git bisect %s' can take only one argument."), terms->term_bad);

	अगर (argc == 0) {
		स्थिर अक्षर *शीर्ष = "BISECT_HEAD";
		क्रमागत get_oid_परिणाम res_शीर्ष = repo_get_oid(the_repository,
							    शीर्ष, &oid);

		अगर (res_शीर्ष == MISSING_OBJECT) {
			शीर्ष = "HEAD";
			res_शीर्ष = repo_get_oid(the_repository, शीर्ष, &oid);
		}

		अगर (res_शीर्ष)
			error(_("Bad rev input: %s"), शीर्ष);
		oid_array_append(&revs, &oid);
	}

	/*
	 * All input revs must be checked before executing bisect_write()
	 * to discard junk revs.
	 */

	क्रम (; argc; argc--, argv++) {
		काष्ठा commit *commit;

		अगर (repo_get_oid(the_repository, *argv, &oid)){
			error(_("Bad rev input: %s"), *argv);
			oid_array_clear(&revs);
			वापस BISECT_FAILED;
		}

		commit = lookup_commit_reference(the_repository, &oid);
		अगर (!commit)
			die(_("Bad rev input (not a commit): %s"), *argv);

		oid_array_append(&revs, &commit->object.oid);
	}

	अगर (refs_read_ref(get_main_ref_store(the_repository), "BISECT_EXPECTED_REV", &expected))
		verify_expected = 0; /* Ignore invalid file contents */

	क्रम (i = 0; i < revs.nr; i++) {
		अगर (bisect_write(state, oid_to_hex(&revs.oid[i]), terms, 0)) {
			oid_array_clear(&revs);
			वापस BISECT_FAILED;
		}
		अगर (verify_expected && !oideq(&revs.oid[i], &expected)) {
			unlink_or_warn(git_path_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					शून्य, "BISECT_EXPECTED_REV", शून्य,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	वापस bisect_auto_अग्र(terms, शून्य);
}

अटल क्रमागत bisect_error bisect_log(व्योम)
{
	पूर्णांक fd, status;
	स्थिर अक्षर* filename = git_path_bisect_log();

	अगर (is_empty_or_missing_file(filename))
		वापस error(_("We are not bisecting."));

	fd = खोलो(filename, O_RDONLY);
	अगर (fd < 0)
		वापस BISECT_FAILED;

	status = copy_fd(fd, STDOUT_FILENO);
	बंद(fd);
	वापस status ? BISECT_FAILED : BISECT_OK;
}

अटल पूर्णांक process_replay_पंक्ति(काष्ठा bisect_terms *terms, काष्ठा strbuf *पंक्ति)
{
	स्थिर अक्षर *p = पंक्ति->buf + म_अखोज(पंक्ति->buf, " \t");
	अक्षर *word_end, *rev;

	अगर ((!skip_prefix(p, "git bisect", &p) &&
	!skip_prefix(p, "git-bisect", &p)) || !है_खाली(*p))
		वापस 0;
	p += म_अखोज(p, " \t");

	word_end = (अक्षर *)p + म_खोज(p, " \t");
	rev = word_end + म_अखोज(word_end, " \t");
	*word_end = '\0'; /* NUL-terminate the word */

	अगर (get_terms(terms, 1))
		वापस error(_("no terms defined"));
	अगर (check_and_set_terms(terms, p))
		वापस -1;

	अगर (!म_भेद(p, "start")) {
		काष्ठा strvec argv = STRVEC_INIT;
		पूर्णांक res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_start(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		वापस res;
	}

	अगर (one_of(p, terms->term_good,
	   terms->term_bad, "skip", शून्य))
		वापस bisect_write(p, rev, terms, 0);

	अगर (!म_भेद(p, "terms")) {
		काष्ठा strvec argv = STRVEC_INIT;
		पूर्णांक res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_terms(terms, argv.nr == 1 ? argv.v[0] : शून्य);
		strvec_clear(&argv);
		वापस res;
	}
	error(_("'%s'?? what are you talking about?"), p);

	वापस -1;
}

अटल क्रमागत bisect_error bisect_replay(काष्ठा bisect_terms *terms, स्थिर अक्षर *filename)
{
	खाता *fp = शून्य;
	क्रमागत bisect_error res = BISECT_OK;
	काष्ठा strbuf पंक्ति = STRBUF_INIT;

	अगर (is_empty_or_missing_file(filename))
		वापस error(_("cannot read file '%s' for replaying"), filename);

	अगर (bisect_clean_state())
		वापस BISECT_FAILED;

	fp = ख_खोलो(filename, "r");
	अगर (!fp)
		वापस BISECT_FAILED;

	जबतक ((strbuf_getline(&पंक्ति, fp) != खातापूर्ण) && !res)
		res = process_replay_पंक्ति(terms, &पंक्ति);

	strbuf_release(&पंक्ति);
	ख_बंद(fp);

	अगर (res)
		वापस BISECT_FAILED;

	वापस bisect_auto_अग्र(terms, शून्य);
}

अटल क्रमागत bisect_error bisect_skip(काष्ठा bisect_terms *terms, पूर्णांक argc,
				     स्थिर अक्षर **argv)
{
	पूर्णांक i;
	क्रमागत bisect_error res;
	काष्ठा strvec argv_state = STRVEC_INIT;

	strvec_धकेलो(&argv_state, "skip");

	क्रम (i = 0; i < argc; i++) {
		स्थिर अक्षर *dotdot = म_माला(argv[i], "..");

		अगर (dotdot) {
			काष्ठा rev_info revs;
			काष्ठा commit *commit;

			repo_init_revisions(the_repository, &revs, शून्य);
			setup_revisions(2, argv + i - 1, &revs, शून्य);

			अगर (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			जबतक ((commit = get_revision(&revs)) != शून्य)
				strvec_धकेलो(&argv_state,
						oid_to_hex(&commit->object.oid));

			reset_revision_walk();
			release_revisions(&revs);
		} अन्यथा {
			strvec_धकेलो(&argv_state, argv[i]);
		}
	}
	res = bisect_state(terms, argv_state.nr, argv_state.v);

	strvec_clear(&argv_state);
	वापस res;
}

अटल पूर्णांक bisect_visualize(काष्ठा bisect_terms *terms, पूर्णांक argc,
			    स्थिर अक्षर **argv)
{
	काष्ठा child_process cmd = CHILD_PROCESS_INIT;
	काष्ठा strbuf sb = STRBUF_INIT;

	अगर (bisect_अग्र_check(terms, शून्य) != 0)
		वापस BISECT_FAILED;

	cmd.no_stdin = 1;
	अगर (!argc) {
		अगर ((दो_पर्या("DISPLAY") || दो_पर्या("SESSIONNAME") || दो_पर्या("MSYSTEM") ||
		     दो_पर्या("SECURITYSESSIONID")) && exists_in_PATH("gitk")) {
			strvec_धकेलो(&cmd.args, "gitk");
		} अन्यथा {
			strvec_धकेलो(&cmd.args, "log");
			cmd.git_cmd = 1;
		}
	} अन्यथा {
		अगर (argv[0][0] == '-') {
			strvec_धकेलो(&cmd.args, "log");
			cmd.git_cmd = 1;
		} अन्यथा अगर (म_भेद(argv[0], "tig") && !starts_with(argv[0], "git"))
			cmd.git_cmd = 1;

		strvec_pushv(&cmd.args, argv);
	}

	strvec_pushl(&cmd.args, "--bisect", "--", शून्य);

	strbuf_read_file(&sb, git_path_bisect_names(), 0);
	sq_dequote_to_strvec(sb.buf, &cmd.args);
	strbuf_release(&sb);

	वापस run_command(&cmd);
}

अटल पूर्णांक get_first_good(स्थिर काष्ठा reference *ref, व्योम *cb_तथ्य)
{
	oidcpy(cb_तथ्य, ref->oid);
	वापस 1;
}

अटल पूर्णांक do_bisect_run(स्थिर अक्षर *command)
{
	काष्ठा child_process cmd = CHILD_PROCESS_INIT;

	म_लिखो(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_धकेलो(&cmd.args, command);
	वापस run_command(&cmd);
}

अटल पूर्णांक verify_good(स्थिर काष्ठा bisect_terms *terms, स्थिर अक्षर *command)
{
	पूर्णांक rc;
	क्रमागत bisect_error res;
	काष्ठा object_id good_rev;
	काष्ठा object_id current_rev;
	अक्षर *good_glob = xstrfmt("%s-*", terms->term_good);
	पूर्णांक no_checkout = refs_ref_exists(get_main_ref_store(the_repository),
					  "BISECT_HEAD");
	काष्ठा refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = म_माप("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_first_good, &good_rev, &opts);
	मुक्त(good_glob);

	अगर (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		वापस -1;

	res = bisect_checkout(&good_rev, no_checkout);
	अगर (res != BISECT_OK)
		वापस -1;

	rc = do_bisect_run(command);

	res = bisect_checkout(&current_rev, no_checkout);
	अगर (res != BISECT_OK)
		वापस -1;

	वापस rc;
}

अटल पूर्णांक bisect_run(काष्ठा bisect_terms *terms, पूर्णांक argc, स्थिर अक्षर **argv)
{
	पूर्णांक res = BISECT_OK;
	काष्ठा strbuf command = STRBUF_INIT;
	स्थिर अक्षर *reset_when_found_arg;
	स्थिर अक्षर *new_state;
	पूर्णांक temporary_stdout_fd, saved_stdout;
	पूर्णांक is_first_run = 1;
	क्रमागत reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;

	अगर (bisect_अग्र_check(terms, शून्य))
		वापस BISECT_FAILED;

	अगर (argc && !म_भेद(argv[0], "--reset-when-found")) {
		reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
	} अन्यथा अगर (argc && skip_prefix(argv[0], "--reset-when-found=",
				    &reset_when_found_arg)) {
		अगर (parse_reset_when_found(reset_when_found_arg,
					   &reset_when_found))
			वापस BISECT_FAILED;
	}

	अगर (reset_when_found != RESET_WHEN_FOUND_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		वापस error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	अगर (reset_when_found != RESET_WHEN_FOUND_NONE) {
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));
		argc--;
		argv++;
	}

	अगर (!argc) {
		error(_("bisect run failed: no command provided."));
		वापस BISECT_FAILED;
	}

	sq_quote_argv(&command, argv);
	strbuf_ltrim(&command);
	जबतक (1) {
		res = do_bisect_run(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		अगर (is_first_run && (res == 126 || res == 127)) {
			पूर्णांक rc = verify_good(terms, command.buf);
			is_first_run = 0;
			अगर (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->term_good);
				res = BISECT_FAILED;
				अवरोध;
			}
			अगर (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->term_good);
				res = BISECT_FAILED;
				अवरोध;
			}
		}

		अगर (res < 0 || 128 <= res) {
			error(_("bisect run failed: exit code %d from"
				" %s is < 0 or >= 128"), res, command.buf);
			अवरोध;
		}

		अगर (res == 125)
			new_state = "skip";
		अन्यथा अगर (!res)
			new_state = terms->term_good;
		अन्यथा
			new_state = terms->term_bad;

		temporary_stdout_fd = खोलो(git_path_bisect_run(), O_CREAT | O_WRONLY | O_TRUNC, 0666);

		अगर (temporary_stdout_fd < 0) {
			res = error_errno(_("cannot open file '%s' for writing"), git_path_bisect_run());
			अवरोध;
		}

		ख_साफ(मानक_निकास);
		saved_stdout = dup(1);
		अगर (saved_stdout < 0 ||
		    dup2(temporary_stdout_fd, 1) < 0) {
			res = error_errno(_("could not duplicate stdout"));
			अगर (saved_stdout >= 0)
				बंद(saved_stdout);
			बंद(temporary_stdout_fd);
			अवरोध;
		}

		res = bisect_state(terms, 1, &new_state);

		ख_साफ(मानक_निकास);
		dup2(saved_stdout, 1);
		बंद(saved_stdout);
		बंद(temporary_stdout_fd);

		print_file_to_stdout(git_path_bisect_run());

		अगर (res == BISECT_ONLY_SKIPPED_बायाँ)
			error(_("bisect run cannot continue any more"));
		अन्यथा अगर (res == BISECT_INTERNAL_SUCCESS_MERGE_BASE) {
			माला_दो(_("bisect run success"));
			res = BISECT_OK;
		} अन्यथा अगर (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
			म_लिखो(_("bisect found first '%s' commit\n"), terms->term_bad);
		} अन्यथा अगर (res) {
			error(_("bisect run failed: 'git bisect %s'"
				" exited with error code %d"), new_state, res);
		} अन्यथा {
			जारी;
		}
		अवरोध;
	}

	strbuf_release(&command);
	वापस res;
}

अटल पूर्णांक cmd_bisect__reset(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix UNUSED,
			     काष्ठा repository *repo UNUSED)
{
	पूर्णांक res;

	अगर (argc > 1)
		वापस error(_("'%s' requires either no argument or a commit"),
			     "git bisect reset");
	res = bisect_reset(argc ? argv[0] : शून्य, false);
	अगर (res)
		वापस res;
	वापस bisect_clean_state();
}

अटल पूर्णांक cmd_bisect__terms(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix UNUSED,
			     काष्ठा repository *repo UNUSED)
{
	पूर्णांक res;
	काष्ठा bisect_terms terms = { 0 };

	अगर (argc > 1)
		वापस error(_("'%s' requires 0 or 1 argument"),
			     "git bisect terms");
	res = bisect_terms(&terms, argc == 1 ? argv[0] : शून्य);
	free_terms(&terms);
	वापस res;
}

अटल पूर्णांक cmd_bisect__start(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix UNUSED,
			     काष्ठा repository *repo UNUSED)
{
	पूर्णांक res;
	काष्ठा bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	res = bisect_start(&terms, argc, argv);
	free_terms(&terms);
	वापस res;
}

अटल पूर्णांक cmd_bisect_अग्र(पूर्णांक argc, स्थिर अक्षर **argv UNUSED, स्थिर अक्षर *prefix,
			    काष्ठा repository *repo UNUSED)
{
	पूर्णांक res;
	काष्ठा bisect_terms terms = { 0 };

	अगर (argc)
		वापस error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	अगर (get_terms(&terms, 1))
		वापस error(_("no terms defined"));
	res = bisect_अग्र(&terms, prefix);
	free_terms(&terms);
	वापस res;
}

अटल पूर्णांक cmd_bisect__log(पूर्णांक argc UNUSED, स्थिर अक्षर **argv UNUSED,
			   स्थिर अक्षर *prefix UNUSED,
			   काष्ठा repository *repo UNUSED)
{
	वापस bisect_log();
}

अटल पूर्णांक cmd_bisect__replay(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix UNUSED,
			      काष्ठा repository *repo UNUSED)
{
	पूर्णांक res;
	काष्ठा bisect_terms terms = { 0 };

	अगर (argc != 1)
		वापस error(_("no logfile given"));
	set_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	वापस res;
}

अटल पूर्णांक cmd_bisect__skip(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix UNUSED,
			    काष्ठा repository *repo UNUSED)
{
	पूर्णांक res;
	काष्ठा bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	अगर (get_terms(&terms, 1))
		वापस error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	वापस res;
}

अटल पूर्णांक cmd_bisect__visualize(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix UNUSED,
				 काष्ठा repository *repo UNUSED)
{
	पूर्णांक res;
	काष्ठा bisect_terms terms = { 0 };

	अगर (get_terms(&terms, 1))
		वापस error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	वापस res;
}

अटल पूर्णांक cmd_bisect__run(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix UNUSED,
			   काष्ठा repository *repo UNUSED)
{
	पूर्णांक res;
	काष्ठा bisect_terms terms = { 0 };

	अगर (!argc)
		वापस error(_("'%s' failed: no command provided."), "git bisect run");
	अगर (get_terms(&terms, 1))
		वापस error(_("no terms defined"));
	res = bisect_run(&terms, argc, argv);
	free_terms(&terms);
	वापस res;
}

पूर्णांक cmd_bisect(पूर्णांक argc,
	       स्थिर अक्षर **argv,
	       स्थिर अक्षर *prefix,
	       काष्ठा repository *repo)
{
	पूर्णांक res = 0;
	parse_opt_subcommand_fn *fn = शून्य;
	काष्ठा option options[] = {
		OPT_SUBCOMMAND("reset", &fn, cmd_bisect__reset),
		OPT_SUBCOMMAND("terms", &fn, cmd_bisect__terms),
		OPT_SUBCOMMAND("start", &fn, cmd_bisect__start),
		OPT_SUBCOMMAND("next", &fn, cmd_bisect_अग्र),
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

	अगर (!fn) {
		काष्ठा bisect_terms terms = { 0 };

		अगर (!argc)
			usage_संदेश_opt(_("need a command"), git_bisect_usage, options);

		अगर (!म_भेद(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		set_terms(&terms, "bad", "good");
		अगर (get_terms(&terms, 1))
			वापस error(_("no terms defined"));
		अगर (check_and_set_terms(&terms, argv[0]) ||
		    !one_of(argv[0], terms.term_good, terms.term_bad, शून्य))
			usage_संदेश_optf(_("unknown command: '%s'"), git_bisect_usage,
				       options, argv[0]);
		अन्यथा
			res = bisect_state(&terms, argc, argv);
		free_terms(&terms);
	} अन्यथा {
		argc--;
		argv++;
		res = fn(argc, argv, prefix, repo);
	}

	अगर (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		क्रमागत reset_when_found_mode mode;

		अगर (read_reset_when_found(&mode))
			res = BISECT_FAILED;
		अन्यथा अगर (mode != RESET_WHEN_FOUND_NONE &&
			 bisect_reset_when_found(mode))
			res = BISECT_FAILED;
	}

	वापस is_bisect_success(res) ? 0 : -res;
}
