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

स्थिर अचर वर्ण * अचर git_bisect_usage[] = {
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
	अचिन्हित पूर्णाङ्क object_flags;
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

स्थिर शून्य set_terms(संरचना bisect_terms *terms, अचर वर्ण *bad,
		      अचर वर्ण *good)
{
	मुक्त((शून्य *)terms->term_good);
	terms->term_good = xstrdup(good);
	मुक्त((शून्य *)terms->term_bad);
	terms->term_bad = xstrdup(bad);
}

स्थिर अचर वर्ण vocab_bad[] = "bad|new";
स्थिर अचर वर्ण vocab_good[] = "good|old";

स्थिर पूर्णाङ्क bisect_autostart(संरचना bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
LAST_ARG_MUST_BE_NULL
स्थिर पूर्णाङ्क one_of(अचर वर्ण *term, ...)
{
	पूर्णाङ्क res = 0;
	बहु_सूची matches;
	अचर वर्ण *match;

	बहु_शुरू(matches, term);
	जबसम्म (!res && (match = बहु_तर्क(matches, अचर वर्ण *)))
		res = !म_भेद(term, match);
	बहु_पूर्ण(matches);

	फर्काऊ res;
}

/*
 * return code BISECT_INTERNAL_SUCCESS_MERGE_BASE
 * and BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND are codes
 * that indicate special success.
 */

स्थिर पूर्णाङ्क is_bisect_success(गणना bisect_error res)
{
	फर्काऊ !res ||
		res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND ||
		res == BISECT_INTERNAL_SUCCESS_MERGE_BASE;
}

स्थिर पूर्णाङ्क write_in_file(अचर वर्ण *path, अचर वर्ण *mode, अचर वर्ण *format, बहु_सूची args)
{
	खाता *fp = रिक्त;
	पूर्णाङ्क res = 0;

	यदि (म_भेद(mode, "w") && म_भेद(mode, "a"))
		BUG("write-in-file does not support '%s' mode", mode);
	fp = ख_खोलो(path, mode);
	यदि (!fp)
		फर्काऊ error_errno(_("cannot open file '%s' in mode '%s'"), path, mode);
	res = भख_लिखो(fp, format, args);

	यदि (res < 0) {
		पूर्णाङ्क saved_errno = त्रुटि_सं;
		ख_बंद(fp);
		त्रुटि_सं = saved_errno;
		फर्काऊ error_errno(_("could not write to file '%s'"), path);
	}

	फर्काऊ ख_बंद(fp);
}

__attribute__((format (म_लिखो, 2, 3)))
स्थिर पूर्णाङ्क write_to_file(अचर वर्ण *path, अचर वर्ण *format, ...)
{
	पूर्णाङ्क res;
	बहु_सूची args;

	बहु_शुरू(args, format);
	res = write_in_file(path, "w", format, args);
	बहु_पूर्ण(args);

	फर्काऊ res;
}

__attribute__((format (म_लिखो, 2, 3)))
स्थिर पूर्णाङ्क append_to_file(अचर वर्ण *path, अचर वर्ण *format, ...)
{
	पूर्णाङ्क res;
	बहु_सूची args;

	बहु_शुरू(args, format);
	res = write_in_file(path, "a", format, args);
	बहु_पूर्ण(args);

	फर्काऊ res;
}

स्थिर पूर्णाङ्क print_file_to_stdout(अचर वर्ण *path)
{
	पूर्णाङ्क fd = खोलो(path, O_RDONLY);
	पूर्णाङ्क ret = 0;

	यदि (fd < 0)
		फर्काऊ error_errno(_("cannot open file '%s' for reading"), path);
	यदि (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), path);
	बंद(fd);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क check_term_format(अचर वर्ण *term, अचर वर्ण *orig_term)
{
	पूर्णाङ्क res;
	वर्ण *new_term = xstrfmt("refs/bisect/%s", term);

	res = check_refname_format(new_term, 0);
	मुक्त(new_term);

	यदि (res)
		फर्काऊ error(_("'%s' is not a valid term"), term);

	यदि (one_of(term, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", रिक्त))
		फर्काऊ error(_("can't use the builtin command '%s' as a term"), term);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	यदि ((म_भेद(orig_term, "bad") && one_of(term, "bad", "new", रिक्त)) ||
		 (म_भेद(orig_term, "good") && one_of(term, "good", "old", रिक्त)))
		फर्काऊ error(_("can't change the meaning of the term '%s'"), term);

	फर्काऊ 0;
}

स्थिर पूर्णाङ्क write_terms(अचर वर्ण *bad, अचर वर्ण *good)
{
	पूर्णाङ्क res;

	यदि (!म_भेद(bad, good))
		फर्काऊ error(_("please use two different terms"));

	यदि (check_term_format(bad, "bad") || check_term_format(good, "good"))
		फर्काऊ -1;

	res = write_to_file(git_path_bisect_terms(), "%s\n%s\n", bad, good);

	फर्काऊ res;
}

स्थिर पूर्णाङ्क bisect_reset(अचर वर्ण *commit, bool quiet)
{
	संरचना strbuf branch = STRBUF_INIT;

	यदि (!commit) {
		यदि (!strbuf_read_file(&branch, git_path_bisect_start(), 0))
			म_लिखो(_("We are not bisecting.\n"));
		अन्यथा
			strbuf_rtrim(&branch);
	} अन्यथा {
		संरचना object_id oid;

		यदि (repo_get_oid_commit(the_repository, commit, &oid))
			फर्काऊ error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	यदि (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		संरचना child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "checkout", "--ignore-other-worktrees", रिक्त);
		यदि (quiet)
			strvec_थप(&cmd.args, "--quiet");
		strvec_pushl(&cmd.args, branch.buf, "--", रिक्त);
		यदि (run_command(&cmd)) {
			error(_("could not check out original"
				" HEAD '%s'. Try 'git bisect"
				" reset <commit>'."), branch.buf);
			strbuf_release(&branch);
			फर्काऊ -1;
		}
	}

	strbuf_release(&branch);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क parse_reset_when_found(अचर वर्ण *मान,
				  गणना reset_when_found_mode *mode)
{
	यदि (!म_भेद(मान, "original"))
		*mode = RESET_WHEN_FOUND_TO_ORIGINAL;
	अन्यथा यदि (!म_भेद(मान, "found"))
		*mode = RESET_WHEN_FOUND_TO_FOUND;
	अन्यथा
		फर्काऊ error(_("invalid value for '--reset-when-found': '%s'"),
			     मान);

	फर्काऊ 0;
}

स्थिर अचर वर्ण *reset_when_found_mode_name(गणना reset_when_found_mode mode)
{
	विकल्प (mode) {
	पक्ष RESET_WHEN_FOUND_TO_ORIGINAL:
		फर्काऊ "original";
	पक्ष RESET_WHEN_FOUND_TO_FOUND:
		फर्काऊ "found";
	पक्ष RESET_WHEN_FOUND_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", mode);
}

स्थिर पूर्णाङ्क read_reset_when_found(गणना reset_when_found_mode *mode)
{
	संरचना strbuf मान = STRBUF_INIT;
	पूर्णाङ्क res = 0;

	*mode = RESET_WHEN_FOUND_NONE;
	यदि (is_empty_or_missing_file(git_path_bisect_reset_when_found()))
		फर्काऊ 0;

	यदि (strbuf_read_file(&मान, git_path_bisect_reset_when_found(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_path_bisect_reset_when_found());
		गच्छ out;
	}
	strbuf_trim(&मान);
	यदि (parse_reset_when_found(मान.buf, mode))
		res = -1;

out:
	strbuf_release(&मान);
	फर्काऊ res;
}

स्थिर पूर्णाङ्क bisect_reset_when_found(गणना reset_when_found_mode mode)
{
	संरचना bisect_terms terms = { 0 };
	वर्ण *commit = रिक्त;
	पूर्णाङ्क res;

	यदि (mode == RESET_WHEN_FOUND_TO_FOUND) {
		read_bisect_terms(&terms.term_bad, &terms.term_good);
		commit = xstrfmt("refs/bisect/%s", terms.term_bad);
	} अन्यथा यदि (mode == RESET_WHEN_FOUND_NONE) {
		BUG("automatic reset requested without a reset mode");
	}

	res = bisect_reset(commit, true);
	यदि (!res)
		res = bisect_clean_state();

	मुक्त(commit);
	free_terms(&terms);
	फर्काऊ res;
}

स्थिर शून्य log_commit(खाता *fp,
		       अचर वर्ण *fmt, अचर वर्ण *state,
		       संरचना commit *commit)
{
	संरचना pretty_print_context pp = {0};
	संरचना strbuf commit_सन्देश = STRBUF_INIT;
	वर्ण *label = xstrfmt(fmt, state);

	repo_format_commit_message(the_repository, commit, "%s", &commit_सन्देश,
				   &pp);

	ख_लिखो(fp, "# %s: [%s] %s\n", label, oid_to_hex(&commit->object.oid),
		commit_सन्देश.buf);

	strbuf_release(&commit_सन्देश);
	मुक्त(label);
}

स्थिर पूर्णाङ्क bisect_write(अचर वर्ण *state, अचर वर्ण *rev,
			अचर संरचना bisect_terms *terms, पूर्णाङ्क nolog)
{
	संरचना strbuf tag = STRBUF_INIT;
	संरचना object_id oid;
	संरचना commit *commit;
	खाता *fp = रिक्त;
	पूर्णाङ्क res = 0;

	यदि (!म_भेद(state, terms->term_bad)) {
		strbuf_addf(&tag, "refs/bisect/%s", state);
	} अन्यथा यदि (one_of(state, terms->term_good, "skip", रिक्त)) {
		strbuf_addf(&tag, "refs/bisect/%s-%s", state, rev);
	} अन्यथा {
		res = error(_("Bad bisect_write argument: %s"), state);
		गच्छ finish;
	}

	यदि (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		गच्छ finish;
	}

	यदि (refs_update_ref(get_main_ref_store(the_repository), रिक्त, tag.buf, &oid, रिक्त, 0,
			    UPDATE_REFS_सन्देश_ON_ERR)) {
		res = -1;
		गच्छ finish;
	}

	fp = ख_खोलो(git_path_bisect_log(), "a");
	यदि (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_path_bisect_log());
		गच्छ finish;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	यदि (!nolog)
		ख_लिखो(fp, "git bisect %s %s\n", state, rev);

finish:
	यदि (fp)
		ख_बंद(fp);
	strbuf_release(&tag);
	फर्काऊ res;
}

स्थिर पूर्णाङ्क check_and_set_terms(संरचना bisect_terms *terms, अचर वर्ण *cmd)
{
	पूर्णाङ्क has_term_file = !is_empty_or_missing_file(git_path_bisect_terms());

	यदि (one_of(cmd, "skip", "start", "terms", रिक्त))
		फर्काऊ 0;

	यदि (has_term_file && म_भेद(cmd, terms->term_bad) &&
	    म_भेद(cmd, terms->term_good))
		फर्काऊ error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->term_bad,
				terms->term_good);

	यदि (!has_term_file) {
		यदि (one_of(cmd, "bad", "good", रिक्त)) {
			set_terms(terms, "bad", "good");
			फर्काऊ write_terms(terms->term_bad, terms->term_good);
		}
		यदि (one_of(cmd, "new", "old", रिक्त)) {
			set_terms(terms, "new", "old");
			फर्काऊ write_terms(terms->term_bad, terms->term_good);
		}
	}

	फर्काऊ 0;
}

स्थिर पूर्णाङ्क inc_nr(अचर संरचना reference *ref UNUSED, शून्य *cb_data)
{
	अचिन्हित पूर्णाङ्क *nr = (अचिन्हित पूर्णाङ्क *)cb_data;
	(*nr)++;
	फर्काऊ 0;
}

स्थिर अचर वर्ण need_bad_and_good_revision_warning[] =
	N_("You need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

स्थिर अचर वर्ण need_bisect_start_warning[] =
	N_("You need to start by \"git bisect start\".\n"
	   "You then need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

स्थिर पूर्णाङ्क decide_next(अचर संरचना bisect_terms *terms,
		       अचर वर्ण *current_term, पूर्णाङ्क missing_good,
		       पूर्णाङ्क missing_bad)
{
	यदि (!missing_good && !missing_bad)
		फर्काऊ 0;
	यदि (!current_term)
		फर्काऊ -1;

	यदि (missing_good && !missing_bad &&
	    !म_भेद(current_term, terms->term_good)) {
		वर्ण *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->term_bad);
		यदि (!isatty(0))
			फर्काऊ 0;
		/*
		 * TRANSLATORS: Make sure to include [Y] and [n] in your
		 * translation. The program will only accept English input
		 * at this point.
		 */
		yesno = git_prompt(_("Are you sure [Y/n]? "), PROMPT_ECHO);
		यदि (starts_with(yesno, "N") || starts_with(yesno, "n"))
			फर्काऊ -1;
		फर्काऊ 0;
	}

	यदि (!is_empty_or_missing_file(git_path_bisect_start()))
		फर्काऊ error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	अन्यथा
		फर्काऊ error(_(need_bisect_start_warning),
			     vocab_good, vocab_bad, vocab_good, vocab_bad);
}

स्थिर शून्य bisect_status(संरचना bisect_state *state,
			  अचर संरचना bisect_terms *terms)
{
	वर्ण *bad_ref = xstrfmt("refs/bisect/%s", terms->term_bad);
	वर्ण *good_glob = xstrfmt("%s-*", terms->term_good);
	संरचना refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = म_माप("refs/bisect/"),
	};

	यदि (refs_ref_exists(get_main_ref_store(the_repository), bad_ref))
		state->nr_bad = 1;

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      inc_nr, &state->nr_good, &opts);

	मुक्त(good_glob);
	मुक्त(bad_ref);
}

__attribute__((format (म_लिखो, 1, 2)))
स्थिर शून्य bisect_log_printf(अचर वर्ण *fmt, ...)
{
	संरचना strbuf buf = STRBUF_INIT;
	बहु_सूची ap;

	बहु_शुरू(ap, fmt);
	strbuf_vaddf(&buf, fmt, ap);
	बहु_पूर्ण(ap);

	म_लिखो("%s", buf.buf);
	append_to_file(git_path_bisect_log(), "# %s", buf.buf);

	strbuf_release(&buf);
}

स्थिर शून्य bisect_print_status(अचर संरचना bisect_terms *terms)
{
	संरचना bisect_state state = { 0 };

	bisect_status(&state, terms);

	/* If we had both, we'd already be started, and shouldn't get here. */
	यदि (state.nr_good && state.nr_bad)
		फर्काऊ;

	यदि (!state.nr_good && !state.nr_bad)
		bisect_log_printf(_("status: waiting for both '%s' and '%s' commits\n"),
				  terms->term_good, terms->term_bad);
	अन्यथा यदि (state.nr_good)
		bisect_log_printf(Q_("status: waiting for '%s' commit, %d '%s' commit known\n",
				     "status: waiting for '%s' commit, %d '%s' commits known\n",
				     state.nr_good),
				  terms->term_bad, state.nr_good, terms->term_good);
	अन्यथा
		bisect_log_printf(_("status: waiting for '%s' commit(s), '%s' commit known\n"),
				  terms->term_good, terms->term_bad);
}

स्थिर पूर्णाङ्क bisect_next_check(अचर संरचना bisect_terms *terms,
			     अचर वर्ण *current_term)
{
	संरचना bisect_state state = { 0 };
	bisect_status(&state, terms);
	फर्काऊ decide_next(terms, current_term, !state.nr_good, !state.nr_bad);
}

स्थिर पूर्णाङ्क get_terms(संरचना bisect_terms *terms, पूर्णाङ्क file_missing_is_ok)
{
	संरचना strbuf str = STRBUF_INIT;
	खाता *fp = रिक्त;
	पूर्णाङ्क res = 0;

	fp = ख_खोलो(git_path_bisect_terms(), "r");
	यदि (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		गच्छ finish;
	}

	free_terms(terms);
	यदि (strbuf_getline_lf(&str, fp) == खातापूर्ण) {
		res = -1;
		गच्छ finish;
	}
	terms->term_bad = strbuf_detach(&str, रिक्त);
	यदि (strbuf_getline_lf(&str, fp) == खातापूर्ण) {
		res = -1;
		FREE_AND_NULL(terms->term_bad);
		गच्छ finish;
	}
	terms->term_good = strbuf_detach(&str, रिक्त);

finish:
	यदि (fp)
		ख_बंद(fp);
	strbuf_release(&str);
	फर्काऊ res;
}

स्थिर पूर्णाङ्क bisect_terms(संरचना bisect_terms *terms, अचर वर्ण *option)
{
	यदि (get_terms(terms, 0))
		फर्काऊ error(_("no terms defined"));

	यदि (!option) {
		म_लिखो(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->term_good, terms->term_bad);
		फर्काऊ 0;
	}
	यदि (one_of(option, "--term-good", "--term-old", रिक्त))
		म_लिखो("%s\n", terms->term_good);
	अन्यथा यदि (one_of(option, "--term-bad", "--term-new", रिक्त))
		म_लिखो("%s\n", terms->term_bad);
	अन्यथा
		फर्काऊ error(_("invalid argument %s for 'git bisect terms'.\n"
			       "Supported options are: "
			       "--term-good|--term-old and "
			       "--term-bad|--term-new."), option);

	फर्काऊ 0;
}

स्थिर पूर्णाङ्क bisect_append_log_quoted(अचर वर्ण **argv)
{
	पूर्णाङ्क res = 0;
	खाता *fp = ख_खोलो(git_path_bisect_log(), "a");
	संरचना strbuf orig_args = STRBUF_INIT;

	यदि (!fp)
		फर्काऊ -1;

	यदि (ख_लिखो(fp, "git bisect start") < 1) {
		res = -1;
		गच्छ finish;
	}

	sq_quote_argv(&orig_args, argv);
	यदि (ख_लिखो(fp, "%s\n", orig_args.buf) < 1)
		res = -1;

finish:
	ख_बंद(fp);
	strbuf_release(&orig_args);
	फर्काऊ res;
}

स्थिर पूर्णाङ्क add_bisect_ref(अचर संरचना reference *ref, शून्य *cb)
{
	संरचना add_bisect_ref_data *data = cb;

	add_pending_oid(data->revs, ref->name, ref->oid, data->object_flags);

	फर्काऊ 0;
}

स्थिर पूर्णाङ्क prepare_revs(संरचना bisect_terms *terms, संरचना rev_info *revs)
{
	संरचना refs_for_each_ref_options opts = {
		.prefix = "refs/bisect/",
		.trim_prefix = म_माप("refs/bisect/"),
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

	यदि (prepare_revision_walk(revs))
		res = error(_("revision walk setup failed"));

	मुक्त(good);
	मुक्त(bad);
	फर्काऊ res;
}

स्थिर पूर्णाङ्क bisect_skipped_commits(संरचना bisect_terms *terms)
{
	पूर्णाङ्क res;
	खाता *fp = रिक्त;
	संरचना rev_info revs;
	संरचना commit *commit;
	संरचना pretty_print_context pp = {0};
	संरचना strbuf commit_name = STRBUF_INIT;

	res = prepare_revs(terms, &revs);
	यदि (res)
		फर्काऊ res;

	fp = ख_खोलो(git_path_bisect_log(), "a");
	यदि (!fp)
		फर्काऊ error_errno(_("could not open '%s' for appending"),
				  git_path_bisect_log());

	यदि (ख_लिखो(fp, "# only skipped commits left to test\n") < 0)
		फर्काऊ error_errno(_("failed to write to '%s'"), git_path_bisect_log());

	जबसम्म ((commit = get_revision(&revs)) != रिक्त) {
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
	फर्काऊ 0;
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
	यदि (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		मुक्त(bad_ref);
		फर्काऊ BISECT_FAILED;
	}
	repo_format_commit_message(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_to_file(git_path_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->term_bad, oid_to_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	मुक्त(bad_ref);
	फर्काऊ res;
}

स्थिर गणना bisect_error bisect_next(संरचना bisect_terms *terms,
				     अचर वर्ण *prefix)
{
	गणना bisect_error res;

	यदि (bisect_autostart(terms))
		फर्काऊ BISECT_FAILED;

	यदि (bisect_next_check(terms, terms->term_good))
		फर्काऊ BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_next_all(the_repository, prefix);

	यदि (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		res = bisect_successful(terms);
		फर्काऊ res ? res : BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND;
	} अन्यथा यदि (res == BISECT_ONLY_SKIPPED_बायाँ) {
		res = bisect_skipped_commits(terms);
		फर्काऊ res ? res : BISECT_ONLY_SKIPPED_बायाँ;
	}
	फर्काऊ res;
}

स्थिर गणना bisect_error bisect_auto_next(संरचना bisect_terms *terms,
					  अचर वर्ण *prefix)
{
	यदि (bisect_next_check(terms, रिक्त)) {
		bisect_print_status(terms);
		फर्काऊ BISECT_OK;
	}

	फर्काऊ bisect_next(terms, prefix);
}

स्थिर गणना bisect_error bisect_start(संरचना bisect_terms *terms, पूर्णाङ्क argc,
				      अचर वर्ण **argv)
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
	अचर वर्ण *head;

	यदि (is_bare_repository(the_repository))
		no_checkout = 1;

	/*
	 * Check for one bad and then some good revisions
	 */
	लागि (i = 0; i < argc; i++) {
		यदि (!म_भेद(argv[i], "--")) {
			has_double_dash = 1;
			रोक;
		}
	}

	लागि (i = 0; i < argc; i++) {
		अचर वर्ण *arg = argv[i];
		यदि (!म_भेद(argv[i], "--")) {
			रोक;
		} अन्यथा यदि (!म_भेद(arg, "--no-checkout")) {
			no_checkout = 1;
		} अन्यथा यदि (!म_भेद(arg, "--first-parent")) {
			first_parent_only = 1;
		} अन्यथा यदि (!म_भेद(arg, "--reset-when-found")) {
			reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
		} अन्यथा यदि (skip_prefix(arg, "--reset-when-found=", &arg)) {
			यदि (parse_reset_when_found(arg, &reset_when_found)) {
				res = BISECT_FAILED;
				गच्छ finish;
			}
		} अन्यथा यदि (!म_भेद(arg, "--term-good") ||
			 !म_भेद(arg, "--term-old")) {
			i++;
			यदि (argc <= i)
				फर्काऊ error(_("'' is not a valid term"));
			must_write_terms = 1;
			मुक्त((शून्य *) terms->term_good);
			terms->term_good = xstrdup(argv[i]);
		} अन्यथा यदि (skip_prefix(arg, "--term-good=", &arg) ||
			   skip_prefix(arg, "--term-old=", &arg)) {
			must_write_terms = 1;
			मुक्त((शून्य *) terms->term_good);
			terms->term_good = xstrdup(arg);
		} अन्यथा यदि (!म_भेद(arg, "--term-bad") ||
			 !म_भेद(arg, "--term-new")) {
			i++;
			यदि (argc <= i)
				फर्काऊ error(_("'' is not a valid term"));
			must_write_terms = 1;
			मुक्त((शून्य *) terms->term_bad);
			terms->term_bad = xstrdup(argv[i]);
		} अन्यथा यदि (skip_prefix(arg, "--term-bad=", &arg) ||
			   skip_prefix(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			मुक्त((शून्य *) terms->term_bad);
			terms->term_bad = xstrdup(arg);
		} अन्यथा यदि (starts_with(arg, "--")) {
			फर्काऊ error(_("unrecognized option: '%s'"), arg);
		} अन्यथा यदि (!get_oidf(&oid, "%s^{commit}", arg)) {
			string_list_append(&revs, oid_to_hex(&oid));
		} अन्यथा यदि (has_double_dash) {
			die(_("'%s' does not appear to be a valid "
			      "revision"), arg);
		} अन्यथा {
			रोक;
		}
	}
	यदि (reset_when_found != RESET_WHEN_FOUND_NONE && no_checkout) {
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
	यदि (revs.nr)
		must_write_terms = 1;
	लागि (i = 0; i < revs.nr; i++) {
		यदि (bad_seen) {
			string_list_append(&states, terms->term_good);
		} अन्यथा {
			bad_seen = 1;
			string_list_append(&states, terms->term_bad);
		}
	}

	/*
	 * Verify HEAD
	 */
	head = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
				       "HEAD", 0, &head_oid, &flags);
	यदि (!head) {
		यदि (repo_get_oid(the_repository, "HEAD", &head_oid))
			फर्काऊ error(_("bad HEAD - I need a HEAD"));
		head = "HEAD";
	}

	/*
	 * Check if we are bisecting
	 */
	यदि (!is_empty_or_missing_file(git_path_bisect_start())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&start_head, git_path_bisect_start(), 0);
		strbuf_trim(&start_head);
		यदि (!no_checkout) {
			संरचना child_process cmd = CHILD_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.args, "checkout", start_head.buf,
				     "--", रिक्त);
			यदि (run_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       start_head.buf);
				गच्छ finish;
			}
		}
	} अन्यथा {
		/* Get the rev from where we start. */
		यदि (!repo_get_oid(the_repository, head, &head_oid) &&
		    !starts_with(head, "refs/heads/")) {
			strbuf_reset(&start_head);
			strbuf_add_oid_hex(&start_head, &head_oid);
		} अन्यथा यदि (!repo_get_oid(the_repository, head, &head_oid) &&
			   skip_prefix(head, "refs/heads/", &head)) {
			strbuf_addstr(&start_head, head);
		} अन्यथा {
			फर्काऊ error(_("bad HEAD - strange symbolic ref"));
		}
	}

	/*
	 * Get rid of any old bisect state.
	 */
	यदि (bisect_clean_state())
		फर्काऊ BISECT_FAILED;

	/*
	 * Write new start state
	 */
	write_file(git_path_bisect_start(), "%s\n", start_head.buf);

	यदि (first_parent_only)
		write_file(git_path_bisect_first_parent(), "\n");

	यदि (reset_when_found != RESET_WHEN_FOUND_NONE)
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));

	यदि (no_checkout) {
		यदि (repo_get_oid(the_repository, start_head.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), start_head.buf);
			गच्छ finish;
		}
		यदि (refs_update_ref(get_main_ref_store(the_repository), रिक्त, "BISECT_HEAD", &oid, रिक्त, 0,
				    UPDATE_REFS_सन्देश_ON_ERR)) {
			res = BISECT_FAILED;
			गच्छ finish;
		}
	}

	यदि (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_names, argv + pathspec_pos);
	write_file(git_path_bisect_names(), "%s\n", bisect_names.buf);

	लागि (i = 0; i < states.nr; i++)
		यदि (bisect_write(states.वस्तुहरू[i].string,
				 revs.वस्तुहरू[i].string, terms, 1)) {
			res = BISECT_FAILED;
			गच्छ finish;
		}

	यदि (must_write_terms && write_terms(terms->term_bad,
					    terms->term_good)) {
		res = BISECT_FAILED;
		गच्छ finish;
	}

	res = bisect_append_log_quoted(argv);
	यदि (res)
		res = BISECT_FAILED;

finish:
	string_list_clear(&revs, 0);
	string_list_clear(&states, 0);
	strbuf_release(&start_head);
	strbuf_release(&bisect_names);
	यदि (res)
		फर्काऊ res;

	res = bisect_auto_next(terms, रिक्त);
	यदि (!is_bisect_success(res))
		bisect_clean_state();
	फर्काऊ res;
}

स्थिर अन्तःस्थ पूर्णाङ्क file_is_not_empty(अचर वर्ण *path)
{
	फर्काऊ !is_empty_or_missing_file(path);
}

स्थिर पूर्णाङ्क bisect_autostart(संरचना bisect_terms *terms)
{
	पूर्णाङ्क res;
	अचर वर्ण *yesno;

	यदि (file_is_not_empty(git_path_bisect_start()))
		फर्काऊ 0;

	fprintf_ln(मानक_त्रुटि, _("You need to start by \"git bisect "
			  "start\"\n"));

	यदि (!isatty(STDIN_FILENO))
		फर्काऊ -1;

	/*
	 * TRANSLATORS: Make sure to include [Y] and [n] in your
	 * translation. The program will only accept English input
	 * at this point.
	 */
	yesno = git_prompt(_("Do you want me to do it for you "
			     "[Y/n]? "), PROMPT_ECHO);
	res = छोटे(*yesno) == 'n' ?
		-1 : bisect_start(terms, 0, empty_strvec);

	फर्काऊ res;
}

स्थिर गणना bisect_error bisect_state(संरचना bisect_terms *terms, पूर्णाङ्क argc,
				      अचर वर्ण **argv)
{
	अचर वर्ण *state;
	पूर्णाङ्क i, verify_expected = 1;
	संरचना object_id oid, expected;
	संरचना oid_array revs = OID_ARRAY_INIT;

	यदि (!argc)
		फर्काऊ error(_("Please call `--bisect-state` with at least one argument"));

	यदि (bisect_autostart(terms))
		फर्काऊ BISECT_FAILED;

	state = argv[0];
	यदि (check_and_set_terms(terms, state) ||
	    !one_of(state, terms->term_good, terms->term_bad, "skip", रिक्त))
		फर्काऊ BISECT_FAILED;

	argv++;
	argc--;
	यदि (argc > 1 && !म_भेद(state, terms->term_bad))
		फर्काऊ error(_("'git bisect %s' can take only one argument."), terms->term_bad);

	यदि (argc == 0) {
		अचर वर्ण *head = "BISECT_HEAD";
		गणना get_oid_result res_head = repo_get_oid(the_repository,
							    head, &oid);

		यदि (res_head == MISSING_OBJECT) {
			head = "HEAD";
			res_head = repo_get_oid(the_repository, head, &oid);
		}

		यदि (res_head)
			error(_("Bad rev input: %s"), head);
		oid_array_append(&revs, &oid);
	}

	/*
	 * All input revs must be checked before executing bisect_write()
	 * to discard junk revs.
	 */

	लागि (; argc; argc--, argv++) {
		संरचना commit *commit;

		यदि (repo_get_oid(the_repository, *argv, &oid)){
			error(_("Bad rev input: %s"), *argv);
			oid_array_clear(&revs);
			फर्काऊ BISECT_FAILED;
		}

		commit = lookup_commit_reference(the_repository, &oid);
		यदि (!commit)
			die(_("Bad rev input (not a commit): %s"), *argv);

		oid_array_append(&revs, &commit->object.oid);
	}

	यदि (refs_read_ref(get_main_ref_store(the_repository), "BISECT_EXPECTED_REV", &expected))
		verify_expected = 0; /* Ignore invalid file contents */

	लागि (i = 0; i < revs.nr; i++) {
		यदि (bisect_write(state, oid_to_hex(&revs.oid[i]), terms, 0)) {
			oid_array_clear(&revs);
			फर्काऊ BISECT_FAILED;
		}
		यदि (verify_expected && !oideq(&revs.oid[i], &expected)) {
			unlink_or_warn(git_path_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					रिक्त, "BISECT_EXPECTED_REV", रिक्त,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	फर्काऊ bisect_auto_next(terms, रिक्त);
}

स्थिर गणना bisect_error bisect_log(शून्य)
{
	पूर्णाङ्क fd, status;
	अचर वर्ण* filename = git_path_bisect_log();

	यदि (is_empty_or_missing_file(filename))
		फर्काऊ error(_("We are not bisecting."));

	fd = खोलो(filename, O_RDONLY);
	यदि (fd < 0)
		फर्काऊ BISECT_FAILED;

	status = copy_fd(fd, STDOUT_FILENO);
	बंद(fd);
	फर्काऊ status ? BISECT_FAILED : BISECT_OK;
}

स्थिर पूर्णाङ्क process_replay_पङ्क्ति(संरचना bisect_terms *terms, संरचना strbuf *पङ्क्ति)
{
	अचर वर्ण *p = पङ्क्ति->buf + म_अखोज(पङ्क्ति->buf, " \t");
	वर्ण *word_end, *rev;

	यदि ((!skip_prefix(p, "git bisect", &p) &&
	!skip_prefix(p, "git-bisect", &p)) || !है_खाली(*p))
		फर्काऊ 0;
	p += म_अखोज(p, " \t");

	word_end = (वर्ण *)p + म_खोज(p, " \t");
	rev = word_end + म_अखोज(word_end, " \t");
	*word_end = '\0'; /* NUL-terminate the word */

	यदि (get_terms(terms, 1))
		फर्काऊ error(_("no terms defined"));
	यदि (check_and_set_terms(terms, p))
		फर्काऊ -1;

	यदि (!म_भेद(p, "start")) {
		संरचना strvec argv = STRVEC_INIT;
		पूर्णाङ्क res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_start(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		फर्काऊ res;
	}

	यदि (one_of(p, terms->term_good,
	   terms->term_bad, "skip", रिक्त))
		फर्काऊ bisect_write(p, rev, terms, 0);

	यदि (!म_भेद(p, "terms")) {
		संरचना strvec argv = STRVEC_INIT;
		पूर्णाङ्क res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_terms(terms, argv.nr == 1 ? argv.v[0] : रिक्त);
		strvec_clear(&argv);
		फर्काऊ res;
	}
	error(_("'%s'?? what are you talking about?"), p);

	फर्काऊ -1;
}

स्थिर गणना bisect_error bisect_replay(संरचना bisect_terms *terms, अचर वर्ण *filename)
{
	खाता *fp = रिक्त;
	गणना bisect_error res = BISECT_OK;
	संरचना strbuf पङ्क्ति = STRBUF_INIT;

	यदि (is_empty_or_missing_file(filename))
		फर्काऊ error(_("cannot read file '%s' for replaying"), filename);

	यदि (bisect_clean_state())
		फर्काऊ BISECT_FAILED;

	fp = ख_खोलो(filename, "r");
	यदि (!fp)
		फर्काऊ BISECT_FAILED;

	जबसम्म ((strbuf_getline(&पङ्क्ति, fp) != खातापूर्ण) && !res)
		res = process_replay_पङ्क्ति(terms, &पङ्क्ति);

	strbuf_release(&पङ्क्ति);
	ख_बंद(fp);

	यदि (res)
		फर्काऊ BISECT_FAILED;

	फर्काऊ bisect_auto_next(terms, रिक्त);
}

स्थिर गणना bisect_error bisect_skip(संरचना bisect_terms *terms, पूर्णाङ्क argc,
				     अचर वर्ण **argv)
{
	पूर्णाङ्क i;
	गणना bisect_error res;
	संरचना strvec argv_state = STRVEC_INIT;

	strvec_थप(&argv_state, "skip");

	लागि (i = 0; i < argc; i++) {
		अचर वर्ण *dotdot = म_माला(argv[i], "..");

		यदि (dotdot) {
			संरचना rev_info revs;
			संरचना commit *commit;

			repo_init_revisions(the_repository, &revs, रिक्त);
			setup_revisions(2, argv + i - 1, &revs, रिक्त);

			यदि (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			जबसम्म ((commit = get_revision(&revs)) != रिक्त)
				strvec_थप(&argv_state,
						oid_to_hex(&commit->object.oid));

			reset_revision_walk();
			release_revisions(&revs);
		} अन्यथा {
			strvec_थप(&argv_state, argv[i]);
		}
	}
	res = bisect_state(terms, argv_state.nr, argv_state.v);

	strvec_clear(&argv_state);
	फर्काऊ res;
}

स्थिर पूर्णाङ्क bisect_visualize(संरचना bisect_terms *terms, पूर्णाङ्क argc,
			    अचर वर्ण **argv)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;
	संरचना strbuf sb = STRBUF_INIT;

	यदि (bisect_next_check(terms, रिक्त) != 0)
		फर्काऊ BISECT_FAILED;

	cmd.no_stdin = 1;
	यदि (!argc) {
		यदि ((दो_पर्या("DISPLAY") || दो_पर्या("SESSIONNAME") || दो_पर्या("MSYSTEM") ||
		     दो_पर्या("SECURITYSESSIONID")) && exists_in_PATH("gitk")) {
			strvec_थप(&cmd.args, "gitk");
		} अन्यथा {
			strvec_थप(&cmd.args, "log");
			cmd.git_cmd = 1;
		}
	} अन्यथा {
		यदि (argv[0][0] == '-') {
			strvec_थप(&cmd.args, "log");
			cmd.git_cmd = 1;
		} अन्यथा यदि (म_भेद(argv[0], "tig") && !starts_with(argv[0], "git"))
			cmd.git_cmd = 1;

		strvec_pushv(&cmd.args, argv);
	}

	strvec_pushl(&cmd.args, "--bisect", "--", रिक्त);

	strbuf_read_file(&sb, git_path_bisect_names(), 0);
	sq_dequote_to_strvec(sb.buf, &cmd.args);
	strbuf_release(&sb);

	फर्काऊ run_command(&cmd);
}

स्थिर पूर्णाङ्क get_first_good(अचर संरचना reference *ref, शून्य *cb_data)
{
	oidcpy(cb_data, ref->oid);
	फर्काऊ 1;
}

स्थिर पूर्णाङ्क do_bisect_run(अचर वर्ण *command)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	म_लिखो(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_थप(&cmd.args, command);
	फर्काऊ run_command(&cmd);
}

स्थिर पूर्णाङ्क verify_good(अचर संरचना bisect_terms *terms, अचर वर्ण *command)
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
		.trim_prefix = म_माप("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_first_good, &good_rev, &opts);
	मुक्त(good_glob);

	यदि (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		फर्काऊ -1;

	res = bisect_checkout(&good_rev, no_checkout);
	यदि (res != BISECT_OK)
		फर्काऊ -1;

	rc = do_bisect_run(command);

	res = bisect_checkout(&current_rev, no_checkout);
	यदि (res != BISECT_OK)
		फर्काऊ -1;

	फर्काऊ rc;
}

स्थिर पूर्णाङ्क bisect_run(संरचना bisect_terms *terms, पूर्णाङ्क argc, अचर वर्ण **argv)
{
	पूर्णाङ्क res = BISECT_OK;
	संरचना strbuf command = STRBUF_INIT;
	अचर वर्ण *reset_when_found_arg;
	अचर वर्ण *new_state;
	पूर्णाङ्क temporary_stdout_fd, saved_stdout;
	पूर्णाङ्क is_first_run = 1;
	गणना reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;

	यदि (bisect_next_check(terms, रिक्त))
		फर्काऊ BISECT_FAILED;

	यदि (argc && !म_भेद(argv[0], "--reset-when-found")) {
		reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
	} अन्यथा यदि (argc && skip_prefix(argv[0], "--reset-when-found=",
				    &reset_when_found_arg)) {
		यदि (parse_reset_when_found(reset_when_found_arg,
					   &reset_when_found))
			फर्काऊ BISECT_FAILED;
	}

	यदि (reset_when_found != RESET_WHEN_FOUND_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		फर्काऊ error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	यदि (reset_when_found != RESET_WHEN_FOUND_NONE) {
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));
		argc--;
		argv++;
	}

	यदि (!argc) {
		error(_("bisect run failed: no command provided."));
		फर्काऊ BISECT_FAILED;
	}

	sq_quote_argv(&command, argv);
	strbuf_ltrim(&command);
	जबसम्म (1) {
		res = do_bisect_run(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		यदि (is_first_run && (res == 126 || res == 127)) {
			पूर्णाङ्क rc = verify_good(terms, command.buf);
			is_first_run = 0;
			यदि (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->term_good);
				res = BISECT_FAILED;
				रोक;
			}
			यदि (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->term_good);
				res = BISECT_FAILED;
				रोक;
			}
		}

		यदि (res < 0 || 128 <= res) {
			error(_("bisect run failed: exit code %d from"
				" %s is < 0 or >= 128"), res, command.buf);
			रोक;
		}

		यदि (res == 125)
			new_state = "skip";
		अन्यथा यदि (!res)
			new_state = terms->term_good;
		अन्यथा
			new_state = terms->term_bad;

		temporary_stdout_fd = खोलो(git_path_bisect_run(), O_CREAT | O_WRONLY | O_TRUNC, 0666);

		यदि (temporary_stdout_fd < 0) {
			res = error_errno(_("cannot open file '%s' for writing"), git_path_bisect_run());
			रोक;
		}

		ख_साफ(मानक_निकास);
		saved_stdout = dup(1);
		यदि (saved_stdout < 0 ||
		    dup2(temporary_stdout_fd, 1) < 0) {
			res = error_errno(_("could not duplicate stdout"));
			यदि (saved_stdout >= 0)
				बंद(saved_stdout);
			बंद(temporary_stdout_fd);
			रोक;
		}

		res = bisect_state(terms, 1, &new_state);

		ख_साफ(मानक_निकास);
		dup2(saved_stdout, 1);
		बंद(saved_stdout);
		बंद(temporary_stdout_fd);

		print_file_to_stdout(git_path_bisect_run());

		यदि (res == BISECT_ONLY_SKIPPED_बायाँ)
			error(_("bisect run cannot continue any more"));
		अन्यथा यदि (res == BISECT_INTERNAL_SUCCESS_MERGE_BASE) {
			माला_दो(_("bisect run success"));
			res = BISECT_OK;
		} अन्यथा यदि (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
			म_लिखो(_("bisect found first '%s' commit\n"), terms->term_bad);
		} अन्यथा यदि (res) {
			error(_("bisect run failed: 'git bisect %s'"
				" exited with error code %d"), new_state, res);
		} अन्यथा {
			जारी;
		}
		रोक;
	}

	strbuf_release(&command);
	फर्काऊ res;
}

स्थिर पूर्णाङ्क cmd_bisect__reset(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix UNUSED,
			     संरचना repository *repo UNUSED)
{
	पूर्णाङ्क res;

	यदि (argc > 1)
		फर्काऊ error(_("'%s' requires either no argument or a commit"),
			     "git bisect reset");
	res = bisect_reset(argc ? argv[0] : रिक्त, false);
	यदि (res)
		फर्काऊ res;
	फर्काऊ bisect_clean_state();
}

स्थिर पूर्णाङ्क cmd_bisect__terms(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix UNUSED,
			     संरचना repository *repo UNUSED)
{
	पूर्णाङ्क res;
	संरचना bisect_terms terms = { 0 };

	यदि (argc > 1)
		फर्काऊ error(_("'%s' requires 0 or 1 argument"),
			     "git bisect terms");
	res = bisect_terms(&terms, argc == 1 ? argv[0] : रिक्त);
	free_terms(&terms);
	फर्काऊ res;
}

स्थिर पूर्णाङ्क cmd_bisect__start(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix UNUSED,
			     संरचना repository *repo UNUSED)
{
	पूर्णाङ्क res;
	संरचना bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	res = bisect_start(&terms, argc, argv);
	free_terms(&terms);
	फर्काऊ res;
}

स्थिर पूर्णाङ्क cmd_bisect__next(पूर्णाङ्क argc, अचर वर्ण **argv UNUSED, अचर वर्ण *prefix,
			    संरचना repository *repo UNUSED)
{
	पूर्णाङ्क res;
	संरचना bisect_terms terms = { 0 };

	यदि (argc)
		फर्काऊ error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	यदि (get_terms(&terms, 1))
		फर्काऊ error(_("no terms defined"));
	res = bisect_next(&terms, prefix);
	free_terms(&terms);
	फर्काऊ res;
}

स्थिर पूर्णाङ्क cmd_bisect__log(पूर्णाङ्क argc UNUSED, अचर वर्ण **argv UNUSED,
			   अचर वर्ण *prefix UNUSED,
			   संरचना repository *repo UNUSED)
{
	फर्काऊ bisect_log();
}

स्थिर पूर्णाङ्क cmd_bisect__replay(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix UNUSED,
			      संरचना repository *repo UNUSED)
{
	पूर्णाङ्क res;
	संरचना bisect_terms terms = { 0 };

	यदि (argc != 1)
		फर्काऊ error(_("no logfile given"));
	set_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	फर्काऊ res;
}

स्थिर पूर्णाङ्क cmd_bisect__skip(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix UNUSED,
			    संरचना repository *repo UNUSED)
{
	पूर्णाङ्क res;
	संरचना bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	यदि (get_terms(&terms, 1))
		फर्काऊ error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	फर्काऊ res;
}

स्थिर पूर्णाङ्क cmd_bisect__visualize(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix UNUSED,
				 संरचना repository *repo UNUSED)
{
	पूर्णाङ्क res;
	संरचना bisect_terms terms = { 0 };

	यदि (get_terms(&terms, 1))
		फर्काऊ error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	फर्काऊ res;
}

स्थिर पूर्णाङ्क cmd_bisect__run(पूर्णाङ्क argc, अचर वर्ण **argv, अचर वर्ण *prefix UNUSED,
			   संरचना repository *repo UNUSED)
{
	पूर्णाङ्क res;
	संरचना bisect_terms terms = { 0 };

	यदि (!argc)
		फर्काऊ error(_("'%s' failed: no command provided."), "git bisect run");
	यदि (get_terms(&terms, 1))
		फर्काऊ error(_("no terms defined"));
	res = bisect_run(&terms, argc, argv);
	free_terms(&terms);
	फर्काऊ res;
}

पूर्णाङ्क cmd_bisect(पूर्णाङ्क argc,
	       अचर वर्ण **argv,
	       अचर वर्ण *prefix,
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

	यदि (!fn) {
		संरचना bisect_terms terms = { 0 };

		यदि (!argc)
			usage_सन्देश_opt(_("need a command"), git_bisect_usage, options);

		यदि (!म_भेद(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		set_terms(&terms, "bad", "good");
		यदि (get_terms(&terms, 1))
			फर्काऊ error(_("no terms defined"));
		यदि (check_and_set_terms(&terms, argv[0]) ||
		    !one_of(argv[0], terms.term_good, terms.term_bad, रिक्त))
			usage_सन्देश_optf(_("unknown command: '%s'"), git_bisect_usage,
				       options, argv[0]);
		अन्यथा
			res = bisect_state(&terms, argc, argv);
		free_terms(&terms);
	} अन्यथा {
		argc--;
		argv++;
		res = fn(argc, argv, prefix, repo);
	}

	यदि (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		गणना reset_when_found_mode mode;

		यदि (read_reset_when_found(&mode))
			res = BISECT_FAILED;
		अन्यथा यदि (mode != RESET_WHEN_FOUND_NONE &&
			 bisect_reset_when_found(mode))
			res = BISECT_FAILED;
	}

	फर्काऊ is_bisect_success(res) ? 0 : -res;
}
