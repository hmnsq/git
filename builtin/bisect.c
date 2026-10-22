#परिभाषय USE_THE_REPOSITORY_VARIABLE
#परिभाषय DISABLE_SIGN_COMPARE_WARNINGS

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

स्थिर GIT_मार्गः_FUNC(git_मार्गः_bisect_terms, "BISECT_TERMS")
स्थिर GIT_मार्गः_FUNC(git_मार्गः_bisect_ancestors_ok, "BISECT_ANCESTORS_OK")
स्थिर GIT_मार्गः_FUNC(git_मार्गः_bisect_start, "BISECT_START")
स्थिर GIT_मार्गः_FUNC(git_मार्गः_bisect_log, "BISECT_LOG")
स्थिर GIT_मार्गः_FUNC(git_मार्गः_bisect_नामानि, "BISECT_NAMES")
स्थिर GIT_मार्गः_FUNC(git_मार्गः_bisect_first_parent, "BISECT_FIRST_PARENT")
स्थिर GIT_मार्गः_FUNC(git_मार्गः_bisect_reset_when_प्राप्तम्, "BISECT_RESET_WHEN_FOUND")
स्थिर GIT_मार्गः_FUNC(git_मार्गः_bisect_सततम्, "BISECT_RUN")

#परिभाषय BUILTIN_GIT_BISECT_START_USAGE \
	N_("git bisect start [--term-(bad|new)=<term-new> --term-(good|old)=<term-old>]\n" \
	   "                 [--no-checkout] [--first-parent] [--reset-when-found[=<where>]] [<bad> [<good>...]] [--] [<pathspec>...]")
#परिभाषय BUILTIN_GIT_BISECT_BAD_USAGE \
	N_("git bisect (bad|new|<term-new>) [<rev>]")
#परिभाषय BUILTIN_GIT_BISECT_GOOD_USAGE \
	N_("git bisect (good|old|<term-old>) [<rev>...]")
#परिभाषय BUILTIN_GIT_BISECT_TERMS_USAGE \
	"git bisect terms [--term-(good|old) | --term-(bad|new)]"
#परिभाषय BUILTIN_GIT_BISECT_SKIP_USAGE \
	N_("git bisect skip [(<rev>|<range>)...]")
#परिभाषय BUILTIN_GIT_BISECT_अग्र_USAGE \
	"git bisect next"
#परिभाषय BUILTIN_GIT_BISECT_RESET_USAGE \
	N_("git bisect reset [<commit>]")
#परिभाषय BUILTIN_GIT_BISECT_VISUALIZE_USAGE \
	"git bisect (visualize|view)"
#परिभाषय BUILTIN_GIT_BISECT_REPLAY_USAGE \
	N_("git bisect replay <logfile>")
#परिभाषय BUILTIN_GIT_BISECT_LOG_USAGE \
	"git bisect log"
#परिभाषय BUILTIN_GIT_BISECT_सततम्_USAGE \
	N_("git bisect run [--reset-when-found[=<where>]] <cmd> [<arg>...]")
#परिभाषय BUILTIN_GIT_BISECT_HELP_USAGE \
	"git bisect help"

स्थिर ध्रुव वर्ण * ध्रुव git_bisect_usage[] = {
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
	BUILTIN_GIT_BISECT_सततम्_USAGE,
	BUILTIN_GIT_BISECT_HELP_USAGE,
	रिक्त
};

गणना reset_when_प्राप्तम्_बहुलकम् {
	RESET_WHEN_प्राप्तम्_NONE,
	RESET_WHEN_प्राप्तम्_गन्तव्यम्_ORIGINAL,
	RESET_WHEN_प्राप्तम्_गन्तव्यम्_प्राप्तम्,
};

संरचना सङ्कलय_bisect_ref_दत्त {
	संरचना rev_info *revs;
	अचिह्नित पूर्णाङ्क object_flags;
};

संरचना bisect_terms {
	वर्ण *पदांशः_good;
	वर्ण *पदांशः_bad;
};

स्थिर शून्य free_terms(संरचना bisect_terms *terms)
{
	FREE_AND_NULL(terms->पदांशः_good);
	FREE_AND_NULL(terms->पदांशः_bad);
}

स्थिर शून्य समुच्चयः_terms(संरचना bisect_terms *terms, ध्रुव वर्ण *bad,
		      ध्रुव वर्ण *good)
{
	मुक्त((शून्य *)terms->पदांशः_good);
	terms->पदांशः_good = xstrdup(good);
	मुक्त((शून्य *)terms->पदांशः_bad);
	terms->पदांशः_bad = xstrdup(bad);
}

स्थिर ध्रुव वर्ण vocab_bad[] = "bad|new";
स्थिर ध्रुव वर्ण vocab_good[] = "good|old";

स्थिर पूर्णाङ्क bisect_autostart(संरचना bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
चरमः_ARG_MUST_BE_NULL
स्थिर पूर्णाङ्क one_सम्बन्धि(ध्रुव वर्ण *पदांशः, ...)
{
	पूर्णाङ्क res = 0;
	va_list matches;
	ध्रुव वर्ण *मेलनम्;

	va_start(matches, पदांशः);
	यावत् (!res && (मेलनम् = va_arg(matches, ध्रुव वर्ण *)))
		res = !वाक्यं_तुलय(पदांशः, मेलनम्);
	va_अन्तः(matches);

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
		res == BISECT_आन्तरिकम्_SUCCESS_1_ST_BAD_प्राप्तम् ||
		res == BISECT_आन्तरिकम्_SUCCESS_MERGE_आधारः;
}

स्थिर पूर्णाङ्क write_अन्तर्_file(ध्रुव वर्ण *मार्गः, ध्रुव वर्ण *बहुलकम्, ध्रुव वर्ण *format, va_list तर्काः)
{
	FILE *fp = रिक्त;
	पूर्णाङ्क res = 0;

	यदि (वाक्यं_तुलय(बहुलकम्, "w") && वाक्यं_तुलय(बहुलकम्, "a"))
		BUG("write-in-file does not support '%s' mode", बहुलकम्);
	fp = fopen(मार्गः, बहुलकम्);
	यदि (!fp)
		प्रत्यागम error_errno(_("cannot open file '%s' in mode '%s'"), मार्गः, बहुलकम्);
	res = vfprintf(fp, format, तर्काः);

	यदि (res < 0) {
		पूर्णाङ्क saved_errno = errno;
		fclose(fp);
		errno = saved_errno;
		प्रत्यागम error_errno(_("could not write to file '%s'"), मार्गः);
	}

	प्रत्यागम fclose(fp);
}

__attribute__((format (प्रारूप_लिख, 2, 3)))
स्थिर पूर्णाङ्क write_गन्तव्यम्_file(ध्रुव वर्ण *मार्गः, ध्रुव वर्ण *format, ...)
{
	पूर्णाङ्क res;
	va_list तर्काः;

	va_start(तर्काः, format);
	res = write_अन्तर्_file(मार्गः, "w", format, तर्काः);
	va_अन्तः(तर्काः);

	प्रत्यागम res;
}

__attribute__((format (प्रारूप_लिख, 2, 3)))
स्थिर पूर्णाङ्क append_गन्तव्यम्_file(ध्रुव वर्ण *मार्गः, ध्रुव वर्ण *format, ...)
{
	पूर्णाङ्क res;
	va_list तर्काः;

	va_start(तर्काः, format);
	res = write_अन्तर्_file(मार्गः, "a", format, तर्काः);
	va_अन्तः(तर्काः);

	प्रत्यागम res;
}

स्थिर पूर्णाङ्क print_file_गन्तव्यम्_stdout(ध्रुव वर्ण *मार्गः)
{
	पूर्णाङ्क fd = उद्घाटय(मार्गः, O_RDONLY);
	पूर्णाङ्क ret = 0;

	यदि (fd < 0)
		प्रत्यागम error_errno(_("cannot open file '%s' for reading"), मार्गः);
	यदि (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), मार्गः);
	close(fd);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क check_पदांशः_format(ध्रुव वर्ण *पदांशः, ध्रुव वर्ण *orig_पदांशः)
{
	पूर्णाङ्क res;
	वर्ण *new_पदांशः = xstrfmt("refs/bisect/%s", पदांशः);

	res = check_refname_format(new_पदांशः, 0);
	मुक्त(new_पदांशः);

	यदि (res)
		प्रत्यागम error(_("'%s' is not a valid term"), पदांशः);

	यदि (one_सम्बन्धि(पदांशः, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", रिक्त))
		प्रत्यागम error(_("can't use the builtin command '%s' as a term"), पदांशः);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	यदि ((वाक्यं_तुलय(orig_पदांशः, "bad") && one_सम्बन्धि(पदांशः, "bad", "new", रिक्त)) ||
		 (वाक्यं_तुलय(orig_पदांशः, "good") && one_सम्बन्धि(पदांशः, "good", "old", रिक्त)))
		प्रत्यागम error(_("can't change the meaning of the term '%s'"), पदांशः);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क write_terms(ध्रुव वर्ण *bad, ध्रुव वर्ण *good)
{
	पूर्णाङ्क res;

	यदि (!वाक्यं_तुलय(bad, good))
		प्रत्यागम error(_("please use two different terms"));

	यदि (check_पदांशः_format(bad, "bad") || check_पदांशः_format(good, "good"))
		प्रत्यागम -1;

	res = write_गन्तव्यम्_file(git_मार्गः_bisect_terms(), "%s\n%s\n", bad, good);

	प्रत्यागम res;
}

स्थिर पूर्णाङ्क bisect_reset(ध्रुव वर्ण *commit, bool quiet)
{
	संरचना strbuf branch = STRBUF_INIT;

	यदि (!commit) {
		यदि (!strbuf_read_file(&branch, git_मार्गः_bisect_start(), 0))
			प्रारूप_लिख(_("We are not bisecting.\n"));
		अन्यथा
			strbuf_rtrim(&branch);
	} अन्यथा {
		संरचना object_id oid;

		यदि (repo_get_oid_commit(the_repository, commit, &oid))
			प्रत्यागम error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	यदि (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		संरचना child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.तर्काः, "checkout", "--ignore-other-worktrees", रिक्त);
		यदि (quiet)
			strvec_प्रक्षिप(&cmd.तर्काः, "--quiet");
		strvec_pushl(&cmd.तर्काः, branch.buf, "--", रिक्त);
		यदि (सततम्_command(&cmd)) {
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

स्थिर पूर्णाङ्क parse_reset_when_प्राप्तम्(ध्रुव वर्ण *मानम्,
				  गणना reset_when_प्राप्तम्_बहुलकम् *बहुलकम्)
{
	यदि (!वाक्यं_तुलय(मानम्, "original"))
		*बहुलकम् = RESET_WHEN_प्राप्तम्_गन्तव्यम्_ORIGINAL;
	अन्यथा यदि (!वाक्यं_तुलय(मानम्, "found"))
		*बहुलकम् = RESET_WHEN_प्राप्तम्_गन्तव्यम्_प्राप्तम्;
	अन्यथा
		प्रत्यागम error(_("invalid value for '--reset-when-found': '%s'"),
			     मानम्);

	प्रत्यागम 0;
}

स्थिर ध्रुव वर्ण *reset_when_प्राप्तम्_बहुलकम्_name(गणना reset_when_प्राप्तम्_बहुलकम् बहुलकम्)
{
	विकल्प (बहुलकम्) {
	पक्ष RESET_WHEN_प्राप्तम्_गन्तव्यम्_ORIGINAL:
		प्रत्यागम "original";
	पक्ष RESET_WHEN_प्राप्तम्_गन्तव्यम्_प्राप्तम्:
		प्रत्यागम "found";
	पक्ष RESET_WHEN_प्राप्तम्_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", बहुलकम्);
}

स्थिर पूर्णाङ्क read_reset_when_प्राप्तम्(गणना reset_when_प्राप्तम्_बहुलकम् *बहुलकम्)
{
	संरचना strbuf मानम् = STRBUF_INIT;
	पूर्णाङ्क res = 0;

	*बहुलकम् = RESET_WHEN_प्राप्तम्_NONE;
	यदि (is_रिक्तानि_or_missing_file(git_मार्गः_bisect_reset_when_प्राप्तम्()))
		प्रत्यागम 0;

	यदि (strbuf_read_file(&मानम्, git_मार्गः_bisect_reset_when_प्राप्तम्(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_मार्गः_bisect_reset_when_प्राप्तम्());
		गच्छ बहिर्गमनम्;
	}
	strbuf_trim(&मानम्);
	यदि (parse_reset_when_प्राप्तम्(मानम्.buf, बहुलकम्))
		res = -1;

बहिर्गमनम्:
	strbuf_release(&मानम्);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क bisect_reset_when_प्राप्तम्(गणना reset_when_प्राप्तम्_बहुलकम् बहुलकम्)
{
	संरचना bisect_terms terms = { 0 };
	वर्ण *commit = रिक्त;
	पूर्णाङ्क res;

	यदि (बहुलकम् == RESET_WHEN_प्राप्तम्_गन्तव्यम्_प्राप्तम्) {
		read_bisect_terms(&terms.पदांशः_bad, &terms.पदांशः_good);
		commit = xstrfmt("refs/bisect/%s", terms.पदांशः_bad);
	} अन्यथा यदि (बहुलकम् == RESET_WHEN_प्राप्तम्_NONE) {
		BUG("automatic reset requested without a reset mode");
	}

	res = bisect_reset(commit, true);
	यदि (!res)
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

	repo_format_commit_सन्देशः(the_repository, commit, "%s", &commit_सन्देशः,
				   &pp);

	fprintf(fp, "# %s: [%s] %s\n", label, oid_गन्तव्यम्_hex(&commit->object.oid),
		commit_सन्देशः.buf);

	strbuf_release(&commit_सन्देशः);
	मुक्त(label);
}

स्थिर पूर्णाङ्क bisect_write(ध्रुव वर्ण *state, ध्रुव वर्ण *rev,
			ध्रुव संरचना bisect_terms *terms, पूर्णाङ्क nolog)
{
	संरचना strbuf चिह्नकम् = STRBUF_INIT;
	संरचना object_id oid;
	संरचना commit *commit;
	FILE *fp = रिक्त;
	पूर्णाङ्क res = 0;

	यदि (!वाक्यं_तुलय(state, terms->पदांशः_bad)) {
		strbuf_addf(&चिह्नकम्, "refs/bisect/%s", state);
	} अन्यथा यदि (one_सम्बन्धि(state, terms->पदांशः_good, "skip", रिक्त)) {
		strbuf_addf(&चिह्नकम्, "refs/bisect/%s-%s", state, rev);
	} अन्यथा {
		res = error(_("Bad bisect_write argument: %s"), state);
		गच्छ finish;
	}

	यदि (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		गच्छ finish;
	}

	यदि (refs_update_ref(get_main_ref_store(the_repository), रिक्त, चिह्नकम्.buf, &oid, रिक्त, 0,
			    UPDATE_REFS_सन्देशः_ON_ERR)) {
		res = -1;
		गच्छ finish;
	}

	fp = fopen(git_मार्गः_bisect_log(), "a");
	यदि (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_मार्गः_bisect_log());
		गच्छ finish;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	यदि (!nolog)
		fprintf(fp, "git bisect %s %s\n", state, rev);

finish:
	यदि (fp)
		fclose(fp);
	strbuf_release(&चिह्नकम्);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क check_and_समुच्चयः_terms(संरचना bisect_terms *terms, ध्रुव वर्ण *cmd)
{
	पूर्णाङ्क has_पदांशः_file = !is_रिक्तानि_or_missing_file(git_मार्गः_bisect_terms());

	यदि (one_सम्बन्धि(cmd, "skip", "start", "terms", रिक्त))
		प्रत्यागम 0;

	यदि (has_पदांशः_file && वाक्यं_तुलय(cmd, terms->पदांशः_bad) &&
	    वाक्यं_तुलय(cmd, terms->पदांशः_good))
		प्रत्यागम error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->पदांशः_bad,
				terms->पदांशः_good);

	यदि (!has_पदांशः_file) {
		यदि (one_सम्बन्धि(cmd, "bad", "good", रिक्त)) {
			समुच्चयः_terms(terms, "bad", "good");
			प्रत्यागम write_terms(terms->पदांशः_bad, terms->पदांशः_good);
		}
		यदि (one_सम्बन्धि(cmd, "new", "old", रिक्त)) {
			समुच्चयः_terms(terms, "new", "old");
			प्रत्यागम write_terms(terms->पदांशः_bad, terms->पदांशः_good);
		}
	}

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क inc_nr(ध्रुव संरचना reference *ref अप्रयुक्तम्, शून्य *cb_दत्त)
{
	अचिह्नित पूर्णाङ्क *nr = (अचिह्नित पूर्णाङ्क *)cb_दत्त;
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

स्थिर पूर्णाङ्क decide_अग्र(ध्रुव संरचना bisect_terms *terms,
		       ध्रुव वर्ण *current_पदांशः, पूर्णाङ्क missing_good,
		       पूर्णाङ्क missing_bad)
{
	यदि (!missing_good && !missing_bad)
		प्रत्यागम 0;
	यदि (!current_पदांशः)
		प्रत्यागम -1;

	यदि (missing_good && !missing_bad &&
	    !वाक्यं_तुलय(current_पदांशः, terms->पदांशः_good)) {
		वर्ण *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->पदांशः_bad);
		यदि (!isatty(0))
			प्रत्यागम 0;
		/*
		 * TRANSLATORS: Make sure to include [Y] and [n] in your
		 * translation. The program will only accept English input
		 * at this point.
		 */
		yesno = git_prompt(_("Are you sure [Y/n]? "), PROMPT_ECHO);
		यदि (starts_with(yesno, "N") || starts_with(yesno, "n"))
			प्रत्यागम -1;
		प्रत्यागम 0;
	}

	यदि (!is_रिक्तानि_or_missing_file(git_मार्गः_bisect_start()))
		प्रत्यागम error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	अन्यथा
		प्रत्यागम error(_(need_bisect_start_warning),
			     vocab_good, vocab_bad, vocab_good, vocab_bad);
}

स्थिर शून्य bisect_स्थितिः(संरचना bisect_state *state,
			  ध्रुव संरचना bisect_terms *terms)
{
	वर्ण *bad_ref = xstrfmt("refs/bisect/%s", terms->पदांशः_bad);
	वर्ण *good_glob = xstrfmt("%s-*", terms->पदांशः_good);
	संरचना refs_for_each_ref_options opts = {
		.आदर्शः = good_glob,
		.उपसर्गः = "refs/bisect/",
		.trim_उपसर्गः = वाक्यदैर्घ्यम्("refs/bisect/"),
	};

	यदि (refs_ref_exists(get_main_ref_store(the_repository), bad_ref))
		state->nr_bad = 1;

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      inc_nr, &state->nr_good, &opts);

	मुक्त(good_glob);
	मुक्त(bad_ref);
}

__attribute__((format (प्रारूप_लिख, 1, 2)))
स्थिर शून्य bisect_log_printf(ध्रुव वर्ण *fmt, ...)
{
	संरचना strbuf buf = STRBUF_INIT;
	va_list ap;

	va_start(ap, fmt);
	strbuf_vaddf(&buf, fmt, ap);
	va_अन्तः(ap);

	प्रारूप_लिख("%s", buf.buf);
	append_गन्तव्यम्_file(git_मार्गः_bisect_log(), "# %s", buf.buf);

	strbuf_release(&buf);
}

स्थिर शून्य bisect_print_स्थितिः(ध्रुव संरचना bisect_terms *terms)
{
	संरचना bisect_state state = { 0 };

	bisect_स्थितिः(&state, terms);

	/* If we had both, we'd already be started, and shouldn't get here. */
	यदि (state.nr_good && state.nr_bad)
		प्रत्यागम;

	यदि (!state.nr_good && !state.nr_bad)
		bisect_log_printf(_("status: waiting for both '%s' and '%s' commits\n"),
				  terms->पदांशः_good, terms->पदांशः_bad);
	अन्यथा यदि (state.nr_good)
		bisect_log_printf(Q_("status: waiting for '%s' commit, %d '%s' commit known\n",
				     "status: waiting for '%s' commit, %d '%s' commits known\n",
				     state.nr_good),
				  terms->पदांशः_bad, state.nr_good, terms->पदांशः_good);
	अन्यथा
		bisect_log_printf(_("status: waiting for '%s' commit(s), '%s' commit known\n"),
				  terms->पदांशः_good, terms->पदांशः_bad);
}

स्थिर पूर्णाङ्क bisect_अग्र_check(ध्रुव संरचना bisect_terms *terms,
			     ध्रुव वर्ण *current_पदांशः)
{
	संरचना bisect_state state = { 0 };
	bisect_स्थितिः(&state, terms);
	प्रत्यागम decide_अग्र(terms, current_पदांशः, !state.nr_good, !state.nr_bad);
}

स्थिर पूर्णाङ्क get_terms(संरचना bisect_terms *terms, पूर्णाङ्क file_missing_is_ok)
{
	संरचना strbuf str = STRBUF_INIT;
	FILE *fp = रिक्त;
	पूर्णाङ्क res = 0;

	fp = fopen(git_मार्गः_bisect_terms(), "r");
	यदि (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		गच्छ finish;
	}

	free_terms(terms);
	यदि (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		गच्छ finish;
	}
	terms->पदांशः_bad = strbuf_detach(&str, रिक्त);
	यदि (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		FREE_AND_NULL(terms->पदांशः_bad);
		गच्छ finish;
	}
	terms->पदांशः_good = strbuf_detach(&str, रिक्त);

finish:
	यदि (fp)
		fclose(fp);
	strbuf_release(&str);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क bisect_terms(संरचना bisect_terms *terms, ध्रुव वर्ण *option)
{
	यदि (get_terms(terms, 0))
		प्रत्यागम error(_("no terms defined"));

	यदि (!option) {
		प्रारूप_लिख(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->पदांशः_good, terms->पदांशः_bad);
		प्रत्यागम 0;
	}
	यदि (one_सम्बन्धि(option, "--term-good", "--term-old", रिक्त))
		प्रारूप_लिख("%s\n", terms->पदांशः_good);
	अन्यथा यदि (one_सम्बन्धि(option, "--term-bad", "--term-new", रिक्त))
		प्रारूप_लिख("%s\n", terms->पदांशः_bad);
	अन्यथा
		प्रत्यागम error(_("invalid argument %s for 'git bisect terms'.\n"
			       "Supported options are: "
			       "--term-good|--term-old and "
			       "--term-bad|--term-new."), option);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क bisect_append_log_quoted(ध्रुव वर्ण **argv)
{
	पूर्णाङ्क res = 0;
	FILE *fp = fopen(git_मार्गः_bisect_log(), "a");
	संरचना strbuf orig_तर्काः = STRBUF_INIT;

	यदि (!fp)
		प्रत्यागम -1;

	यदि (fprintf(fp, "git bisect start") < 1) {
		res = -1;
		गच्छ finish;
	}

	sq_quote_argv(&orig_तर्काः, argv);
	यदि (fprintf(fp, "%s\n", orig_तर्काः.buf) < 1)
		res = -1;

finish:
	fclose(fp);
	strbuf_release(&orig_तर्काः);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क सङ्कलय_bisect_ref(ध्रुव संरचना reference *ref, शून्य *cb)
{
	संरचना सङ्कलय_bisect_ref_दत्त *दत्त = cb;

	सङ्कलय_pending_oid(दत्त->revs, ref->name, ref->oid, दत्त->object_flags);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क prepare_revs(संरचना bisect_terms *terms, संरचना rev_info *revs)
{
	संरचना refs_for_each_ref_options opts = {
		.उपसर्गः = "refs/bisect/",
		.trim_उपसर्गः = वाक्यदैर्घ्यम्("refs/bisect/"),
	};
	पूर्णाङ्क res = 0;
	संरचना सङ्कलय_bisect_ref_दत्त cb = { revs };
	वर्ण *good = xstrfmt("%s-*", terms->पदांशः_good);

	/*
	 * We cannot use terms->term_bad directly in
	 * for_each_glob_ref_in() and we have to append a '*' to it,
	 * otherwise for_each_glob_ref_in() will append '/' and '*'.
	 */
	वर्ण *bad = xstrfmt("%s*", terms->पदांशः_bad);

	/*
	 * It is important to reset the flags used by revision walks
	 * as the previous call to bisect_next_all() in turn
	 * sets up a revision walk.
	 */
	reset_revision_walk();
	repo_init_revisions(the_repository, revs, रिक्त);
	सज्जता_revisions(0, रिक्त, revs, रिक्त);

	opts.आदर्शः = bad;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      सङ्कलय_bisect_ref, &cb, &opts);

	cb.object_flags = UNINTERESTING;
	opts.आदर्शः = good;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      सङ्कलय_bisect_ref, &cb, &opts);

	यदि (prepare_revision_walk(revs))
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
	यदि (res)
		प्रत्यागम res;

	fp = fopen(git_मार्गः_bisect_log(), "a");
	यदि (!fp)
		प्रत्यागम error_errno(_("could not open '%s' for appending"),
				  git_मार्गः_bisect_log());

	यदि (fprintf(fp, "# only skipped commits left to test\n") < 0)
		प्रत्यागम error_errno(_("failed to write to '%s'"), git_मार्गः_bisect_log());

	यावत् ((commit = get_revision(&revs)) != रिक्त) {
		strbuf_reset(&commit_name);
		repo_format_commit_सन्देशः(the_repository, commit, "%s",
					   &commit_name, &pp);
		fprintf(fp, "# possible first '%s' commit: [%s] %s\n",
			terms->पदांशः_bad, oid_गन्तव्यम्_hex(&commit->object.oid),
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
	वर्ण *bad_ref = xstrfmt("refs/bisect/%s",terms->पदांशः_bad);
	पूर्णाङ्क res;

	refs_read_ref(get_main_ref_store(the_repository), bad_ref, &oid);
	commit = lookup_commit_reference_by_name(bad_ref);
	यदि (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		मुक्त(bad_ref);
		प्रत्यागम BISECT_FAILED;
	}
	repo_format_commit_सन्देशः(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_गन्तव्यम्_file(git_मार्गः_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->पदांशः_bad, oid_गन्तव्यम्_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	मुक्त(bad_ref);
	प्रत्यागम res;
}

स्थिर गणना bisect_error bisect_अग्र(संरचना bisect_terms *terms,
				     ध्रुव वर्ण *उपसर्गः)
{
	गणना bisect_error res;

	यदि (bisect_autostart(terms))
		प्रत्यागम BISECT_FAILED;

	यदि (bisect_अग्र_check(terms, terms->पदांशः_good))
		प्रत्यागम BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_अग्र_all(the_repository, उपसर्गः);

	यदि (res == BISECT_आन्तरिकम्_SUCCESS_1_ST_BAD_प्राप्तम्) {
		res = bisect_successful(terms);
		प्रत्यागम res ? res : BISECT_आन्तरिकम्_SUCCESS_1_ST_BAD_प्राप्तम्;
	} अन्यथा यदि (res == BISECT_ONLY_SKIPPED_वामः) {
		res = bisect_skipped_commits(terms);
		प्रत्यागम res ? res : BISECT_ONLY_SKIPPED_वामः;
	}
	प्रत्यागम res;
}

स्थिर गणना bisect_error bisect_auto_अग्र(संरचना bisect_terms *terms,
					  ध्रुव वर्ण *उपसर्गः)
{
	यदि (bisect_अग्र_check(terms, रिक्त)) {
		bisect_print_स्थितिः(terms);
		प्रत्यागम BISECT_OK;
	}

	प्रत्यागम bisect_अग्र(terms, उपसर्गः);
}

स्थिर गणना bisect_error bisect_start(संरचना bisect_terms *terms, पूर्णाङ्क argc,
				      ध्रुव वर्ण **argv)
{
	पूर्णाङ्क no_checkout = 0;
	पूर्णाङ्क first_parent_only = 0;
	पूर्णाङ्क i, has_double_dash = 0, must_write_terms = 0, bad_आलोकितम् = 0;
	पूर्णाङ्क flags, pathspec_pos;
	गणना bisect_error res = BISECT_OK;
	संरचना string_list revs = STRING_LIST_INIT_DUP;
	संरचना string_list अवस्थाः = STRING_LIST_INIT_DUP;
	संरचना strbuf start_शीर्ष = STRBUF_INIT;
	संरचना strbuf bisect_नामानि = STRBUF_INIT;
	संरचना object_id शीर्ष_oid;
	संरचना object_id oid;
	गणना reset_when_प्राप्तम्_बहुलकम् reset_when_प्राप्तम् = RESET_WHEN_प्राप्तम्_NONE;
	ध्रुव वर्ण *शीर्ष;

	यदि (is_bare_repository(the_repository))
		no_checkout = 1;

	/*
	 * Check for one bad and then some good revisions
	 */
	प्रति (i = 0; i < argc; i++) {
		यदि (!वाक्यं_तुलय(argv[i], "--")) {
			has_double_dash = 1;
			भङ्ग;
		}
	}

	प्रति (i = 0; i < argc; i++) {
		ध्रुव वर्ण *arg = argv[i];
		यदि (!वाक्यं_तुलय(argv[i], "--")) {
			भङ्ग;
		} अन्यथा यदि (!वाक्यं_तुलय(arg, "--no-checkout")) {
			no_checkout = 1;
		} अन्यथा यदि (!वाक्यं_तुलय(arg, "--first-parent")) {
			first_parent_only = 1;
		} अन्यथा यदि (!वाक्यं_तुलय(arg, "--reset-when-found")) {
			reset_when_प्राप्तम् = RESET_WHEN_प्राप्तम्_गन्तव्यम्_ORIGINAL;
		} अन्यथा यदि (skip_उपसर्गः(arg, "--reset-when-found=", &arg)) {
			यदि (parse_reset_when_प्राप्तम्(arg, &reset_when_प्राप्तम्)) {
				res = BISECT_FAILED;
				गच्छ finish;
			}
		} अन्यथा यदि (!वाक्यं_तुलय(arg, "--term-good") ||
			 !वाक्यं_तुलय(arg, "--term-old")) {
			i++;
			यदि (argc <= i)
				प्रत्यागम error(_("'' is not a valid term"));
			must_write_terms = 1;
			मुक्त((शून्य *) terms->पदांशः_good);
			terms->पदांशः_good = xstrdup(argv[i]);
		} अन्यथा यदि (skip_उपसर्गः(arg, "--term-good=", &arg) ||
			   skip_उपसर्गः(arg, "--term-old=", &arg)) {
			must_write_terms = 1;
			मुक्त((शून्य *) terms->पदांशः_good);
			terms->पदांशः_good = xstrdup(arg);
		} अन्यथा यदि (!वाक्यं_तुलय(arg, "--term-bad") ||
			 !वाक्यं_तुलय(arg, "--term-new")) {
			i++;
			यदि (argc <= i)
				प्रत्यागम error(_("'' is not a valid term"));
			must_write_terms = 1;
			मुक्त((शून्य *) terms->पदांशः_bad);
			terms->पदांशः_bad = xstrdup(argv[i]);
		} अन्यथा यदि (skip_उपसर्गः(arg, "--term-bad=", &arg) ||
			   skip_उपसर्गः(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			मुक्त((शून्य *) terms->पदांशः_bad);
			terms->पदांशः_bad = xstrdup(arg);
		} अन्यथा यदि (starts_with(arg, "--")) {
			प्रत्यागम error(_("unrecognized option: '%s'"), arg);
		} अन्यथा यदि (!get_oidf(&oid, "%s^{commit}", arg)) {
			string_list_append(&revs, oid_गन्तव्यम्_hex(&oid));
		} अन्यथा यदि (has_double_dash) {
			die(_("'%s' does not appear to be a valid "
			      "revision"), arg);
		} अन्यथा {
			भङ्ग;
		}
	}
	यदि (reset_when_प्राप्तम् != RESET_WHEN_प्राप्तम्_NONE && no_checkout) {
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
	प्रति (i = 0; i < revs.nr; i++) {
		यदि (bad_आलोकितम्) {
			string_list_append(&अवस्थाः, terms->पदांशः_good);
		} अन्यथा {
			bad_आलोकितम् = 1;
			string_list_append(&अवस्थाः, terms->पदांशः_bad);
		}
	}

	/*
	 * Verify HEAD
	 */
	शीर्ष = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
				       "HEAD", 0, &शीर्ष_oid, &flags);
	यदि (!शीर्ष) {
		यदि (repo_get_oid(the_repository, "HEAD", &शीर्ष_oid))
			प्रत्यागम error(_("bad HEAD - I need a HEAD"));
		शीर्ष = "HEAD";
	}

	/*
	 * Check if we are bisecting
	 */
	यदि (!is_रिक्तानि_or_missing_file(git_मार्गः_bisect_start())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&start_शीर्ष, git_मार्गः_bisect_start(), 0);
		strbuf_trim(&start_शीर्ष);
		यदि (!no_checkout) {
			संरचना child_process cmd = CHILD_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.तर्काः, "checkout", start_शीर्ष.buf,
				     "--", रिक्त);
			यदि (सततम्_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       start_शीर्ष.buf);
				गच्छ finish;
			}
		}
	} अन्यथा {
		/* Get the rev from where we start. */
		यदि (!repo_get_oid(the_repository, शीर्ष, &शीर्ष_oid) &&
		    !starts_with(शीर्ष, "refs/heads/")) {
			strbuf_reset(&start_शीर्ष);
			strbuf_सङ्कलय_oid_hex(&start_शीर्ष, &शीर्ष_oid);
		} अन्यथा यदि (!repo_get_oid(the_repository, शीर्ष, &शीर्ष_oid) &&
			   skip_उपसर्गः(शीर्ष, "refs/heads/", &शीर्ष)) {
			strbuf_addstr(&start_शीर्ष, शीर्ष);
		} अन्यथा {
			प्रत्यागम error(_("bad HEAD - strange symbolic ref"));
		}
	}

	/*
	 * Get rid of any old bisect state.
	 */
	यदि (bisect_clean_state())
		प्रत्यागम BISECT_FAILED;

	/*
	 * Write new start state
	 */
	write_file(git_मार्गः_bisect_start(), "%s\n", start_शीर्ष.buf);

	यदि (first_parent_only)
		write_file(git_मार्गः_bisect_first_parent(), "\n");

	यदि (reset_when_प्राप्तम् != RESET_WHEN_प्राप्तम्_NONE)
		write_file(git_मार्गः_bisect_reset_when_प्राप्तम्(), "%s\n",
			   reset_when_प्राप्तम्_बहुलकम्_name(reset_when_प्राप्तम्));

	यदि (no_checkout) {
		यदि (repo_get_oid(the_repository, start_शीर्ष.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), start_शीर्ष.buf);
			गच्छ finish;
		}
		यदि (refs_update_ref(get_main_ref_store(the_repository), रिक्त, "BISECT_HEAD", &oid, रिक्त, 0,
				    UPDATE_REFS_सन्देशः_ON_ERR)) {
			res = BISECT_FAILED;
			गच्छ finish;
		}
	}

	यदि (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_नामानि, argv + pathspec_pos);
	write_file(git_मार्गः_bisect_नामानि(), "%s\n", bisect_नामानि.buf);

	प्रति (i = 0; i < अवस्थाः.nr; i++)
		यदि (bisect_write(अवस्थाः.वस्तूनि[i].string,
				 revs.वस्तूनि[i].string, terms, 1)) {
			res = BISECT_FAILED;
			गच्छ finish;
		}

	यदि (must_write_terms && write_terms(terms->पदांशः_bad,
					    terms->पदांशः_good)) {
		res = BISECT_FAILED;
		गच्छ finish;
	}

	res = bisect_append_log_quoted(argv);
	यदि (res)
		res = BISECT_FAILED;

finish:
	string_list_clear(&revs, 0);
	string_list_clear(&अवस्थाः, 0);
	strbuf_release(&start_शीर्ष);
	strbuf_release(&bisect_नामानि);
	यदि (res)
		प्रत्यागम res;

	res = bisect_auto_अग्र(terms, रिक्त);
	यदि (!is_bisect_success(res))
		bisect_clean_state();
	प्रत्यागम res;
}

स्थिर अन्तःस्थ पूर्णाङ्क file_is_not_रिक्तानि(ध्रुव वर्ण *मार्गः)
{
	प्रत्यागम !is_रिक्तानि_or_missing_file(मार्गः);
}

स्थिर पूर्णाङ्क bisect_autostart(संरचना bisect_terms *terms)
{
	पूर्णाङ्क res;
	ध्रुव वर्ण *yesno;

	यदि (file_is_not_रिक्तानि(git_मार्गः_bisect_start()))
		प्रत्यागम 0;

	fprintf_ln(stderr, _("You need to start by \"git bisect "
			  "start\"\n"));

	यदि (!isatty(STDIN_FILENO))
		प्रत्यागम -1;

	/*
	 * TRANSLATORS: Make sure to include [Y] and [n] in your
	 * translation. The program will only accept English input
	 * at this point.
	 */
	yesno = git_prompt(_("Do you want me to do it for you "
			     "[Y/n]? "), PROMPT_ECHO);
	res = tolower(*yesno) == 'n' ?
		-1 : bisect_start(terms, 0, रिक्तानि_strvec);

	प्रत्यागम res;
}

स्थिर गणना bisect_error bisect_state(संरचना bisect_terms *terms, पूर्णाङ्क argc,
				      ध्रुव वर्ण **argv)
{
	ध्रुव वर्ण *state;
	पूर्णाङ्क i, verify_expected = 1;
	संरचना object_id oid, expected;
	संरचना oid_array revs = OID_ARRAY_INIT;

	यदि (!argc)
		प्रत्यागम error(_("Please call `--bisect-state` with at least one argument"));

	यदि (bisect_autostart(terms))
		प्रत्यागम BISECT_FAILED;

	state = argv[0];
	यदि (check_and_समुच्चयः_terms(terms, state) ||
	    !one_सम्बन्धि(state, terms->पदांशः_good, terms->पदांशः_bad, "skip", रिक्त))
		प्रत्यागम BISECT_FAILED;

	argv++;
	argc--;
	यदि (argc > 1 && !वाक्यं_तुलय(state, terms->पदांशः_bad))
		प्रत्यागम error(_("'git bisect %s' can take only one argument."), terms->पदांशः_bad);

	यदि (argc == 0) {
		ध्रुव वर्ण *शीर्ष = "BISECT_HEAD";
		गणना get_oid_परिणाम res_शीर्ष = repo_get_oid(the_repository,
							    शीर्ष, &oid);

		यदि (res_शीर्ष == MISSING_OBJECT) {
			शीर्ष = "HEAD";
			res_शीर्ष = repo_get_oid(the_repository, शीर्ष, &oid);
		}

		यदि (res_शीर्ष)
			error(_("Bad rev input: %s"), शीर्ष);
		oid_array_append(&revs, &oid);
	}

	/*
	 * All input revs must be checked before executing bisect_write()
	 * to discard junk revs.
	 */

	प्रति (; argc; argc--, argv++) {
		संरचना commit *commit;

		यदि (repo_get_oid(the_repository, *argv, &oid)){
			error(_("Bad rev input: %s"), *argv);
			oid_array_clear(&revs);
			प्रत्यागम BISECT_FAILED;
		}

		commit = lookup_commit_reference(the_repository, &oid);
		यदि (!commit)
			die(_("Bad rev input (not a commit): %s"), *argv);

		oid_array_append(&revs, &commit->object.oid);
	}

	यदि (refs_read_ref(get_main_ref_store(the_repository), "BISECT_EXPECTED_REV", &expected))
		verify_expected = 0; /* Ignore invalid file contents */

	प्रति (i = 0; i < revs.nr; i++) {
		यदि (bisect_write(state, oid_गन्तव्यम्_hex(&revs.oid[i]), terms, 0)) {
			oid_array_clear(&revs);
			प्रत्यागम BISECT_FAILED;
		}
		यदि (verify_expected && !oideq(&revs.oid[i], &expected)) {
			unlink_or_warn(git_मार्गः_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					रिक्त, "BISECT_EXPECTED_REV", रिक्त,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	प्रत्यागम bisect_auto_अग्र(terms, रिक्त);
}

स्थिर गणना bisect_error bisect_log(शून्य)
{
	पूर्णाङ्क fd, स्थितिः;
	ध्रुव वर्ण* filename = git_मार्गः_bisect_log();

	यदि (is_रिक्तानि_or_missing_file(filename))
		प्रत्यागम error(_("We are not bisecting."));

	fd = उद्घाटय(filename, O_RDONLY);
	यदि (fd < 0)
		प्रत्यागम BISECT_FAILED;

	स्थितिः = copy_fd(fd, STDOUT_FILENO);
	close(fd);
	प्रत्यागम स्थितिः ? BISECT_FAILED : BISECT_OK;
}

स्थिर पूर्णाङ्क process_replay_पङ्क्तिः(संरचना bisect_terms *terms, संरचना strbuf *पङ्क्तिः)
{
	ध्रुव वर्ण *p = पङ्क्तिः->buf + strspn(पङ्क्तिः->buf, " \t");
	वर्ण *word_अन्तः, *rev;

	यदि ((!skip_उपसर्गः(p, "git bisect", &p) &&
	!skip_उपसर्गः(p, "git-bisect", &p)) || !रिक्तम्_वा(*p))
		प्रत्यागम 0;
	p += strspn(p, " \t");

	word_अन्तः = (वर्ण *)p + strcspn(p, " \t");
	rev = word_अन्तः + strspn(word_अन्तः, " \t");
	*word_अन्तः = '\0'; /* NUL-terminate the word */

	यदि (get_terms(terms, 1))
		प्रत्यागम error(_("no terms defined"));
	यदि (check_and_समुच्चयः_terms(terms, p))
		प्रत्यागम -1;

	यदि (!वाक्यं_तुलय(p, "start")) {
		संरचना strvec argv = STRVEC_INIT;
		पूर्णाङ्क res;
		sq_dequote_गन्तव्यम्_strvec(rev, &argv);
		res = bisect_start(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		प्रत्यागम res;
	}

	यदि (one_सम्बन्धि(p, terms->पदांशः_good,
	   terms->पदांशः_bad, "skip", रिक्त))
		प्रत्यागम bisect_write(p, rev, terms, 0);

	यदि (!वाक्यं_तुलय(p, "terms")) {
		संरचना strvec argv = STRVEC_INIT;
		पूर्णाङ्क res;
		sq_dequote_गन्तव्यम्_strvec(rev, &argv);
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

	यदि (is_रिक्तानि_or_missing_file(filename))
		प्रत्यागम error(_("cannot read file '%s' for replaying"), filename);

	यदि (bisect_clean_state())
		प्रत्यागम BISECT_FAILED;

	fp = fopen(filename, "r");
	यदि (!fp)
		प्रत्यागम BISECT_FAILED;

	यावत् ((strbuf_getline(&पङ्क्तिः, fp) != EOF) && !res)
		res = process_replay_पङ्क्तिः(terms, &पङ्क्तिः);

	strbuf_release(&पङ्क्तिः);
	fclose(fp);

	यदि (res)
		प्रत्यागम BISECT_FAILED;

	प्रत्यागम bisect_auto_अग्र(terms, रिक्त);
}

स्थिर गणना bisect_error bisect_skip(संरचना bisect_terms *terms, पूर्णाङ्क argc,
				     ध्रुव वर्ण **argv)
{
	पूर्णाङ्क i;
	गणना bisect_error res;
	संरचना strvec argv_state = STRVEC_INIT;

	strvec_प्रक्षिप(&argv_state, "skip");

	प्रति (i = 0; i < argc; i++) {
		ध्रुव वर्ण *dotdot = वाक्ये_अन्विष(argv[i], "..");

		यदि (dotdot) {
			संरचना rev_info revs;
			संरचना commit *commit;

			repo_init_revisions(the_repository, &revs, रिक्त);
			सज्जता_revisions(2, argv + i - 1, &revs, रिक्त);

			यदि (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			यावत् ((commit = get_revision(&revs)) != रिक्त)
				strvec_प्रक्षिप(&argv_state,
						oid_गन्तव्यम्_hex(&commit->object.oid));

			reset_revision_walk();
			release_revisions(&revs);
		} अन्यथा {
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

	यदि (bisect_अग्र_check(terms, रिक्त) != 0)
		प्रत्यागम BISECT_FAILED;

	cmd.no_stdin = 1;
	यदि (!argc) {
		यदि ((getenv("DISPLAY") || getenv("SESSIONNAME") || getenv("MSYSTEM") ||
		     getenv("SECURITYSESSIONID")) && exists_अन्तर्_मार्गः("gitk")) {
			strvec_प्रक्षिप(&cmd.तर्काः, "gitk");
		} अन्यथा {
			strvec_प्रक्षिप(&cmd.तर्काः, "log");
			cmd.git_cmd = 1;
		}
	} अन्यथा {
		यदि (argv[0][0] == '-') {
			strvec_प्रक्षिप(&cmd.तर्काः, "log");
			cmd.git_cmd = 1;
		} अन्यथा यदि (वाक्यं_तुलय(argv[0], "tig") && !starts_with(argv[0], "git"))
			cmd.git_cmd = 1;

		strvec_pushv(&cmd.तर्काः, argv);
	}

	strvec_pushl(&cmd.तर्काः, "--bisect", "--", रिक्त);

	strbuf_read_file(&sb, git_मार्गः_bisect_नामानि(), 0);
	sq_dequote_गन्तव्यम्_strvec(sb.buf, &cmd.तर्काः);
	strbuf_release(&sb);

	प्रत्यागम सततम्_command(&cmd);
}

स्थिर पूर्णाङ्क get_first_good(ध्रुव संरचना reference *ref, शून्य *cb_दत्त)
{
	oidcpy(cb_दत्त, ref->oid);
	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क do_bisect_सततम्(ध्रुव वर्ण *command)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;

	प्रारूप_लिख(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_प्रक्षिप(&cmd.तर्काः, command);
	प्रत्यागम सततम्_command(&cmd);
}

स्थिर पूर्णाङ्क verify_good(ध्रुव संरचना bisect_terms *terms, ध्रुव वर्ण *command)
{
	पूर्णाङ्क rc;
	गणना bisect_error res;
	संरचना object_id good_rev;
	संरचना object_id current_rev;
	वर्ण *good_glob = xstrfmt("%s-*", terms->पदांशः_good);
	पूर्णाङ्क no_checkout = refs_ref_exists(get_main_ref_store(the_repository),
					  "BISECT_HEAD");
	संरचना refs_for_each_ref_options opts = {
		.आदर्शः = good_glob,
		.उपसर्गः = "refs/bisect/",
		.trim_उपसर्गः = वाक्यदैर्घ्यम्("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_first_good, &good_rev, &opts);
	मुक्त(good_glob);

	यदि (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		प्रत्यागम -1;

	res = bisect_checkout(&good_rev, no_checkout);
	यदि (res != BISECT_OK)
		प्रत्यागम -1;

	rc = do_bisect_सततम्(command);

	res = bisect_checkout(&current_rev, no_checkout);
	यदि (res != BISECT_OK)
		प्रत्यागम -1;

	प्रत्यागम rc;
}

स्थिर पूर्णाङ्क bisect_सततम्(संरचना bisect_terms *terms, पूर्णाङ्क argc, ध्रुव वर्ण **argv)
{
	पूर्णाङ्क res = BISECT_OK;
	संरचना strbuf command = STRBUF_INIT;
	ध्रुव वर्ण *reset_when_प्राप्तम्_arg;
	ध्रुव वर्ण *new_state;
	पूर्णाङ्क temporary_stdout_fd, saved_stdout;
	पूर्णाङ्क is_first_सततम् = 1;
	गणना reset_when_प्राप्तम्_बहुलकम् reset_when_प्राप्तम् = RESET_WHEN_प्राप्तम्_NONE;

	यदि (bisect_अग्र_check(terms, रिक्त))
		प्रत्यागम BISECT_FAILED;

	यदि (argc && !वाक्यं_तुलय(argv[0], "--reset-when-found")) {
		reset_when_प्राप्तम् = RESET_WHEN_प्राप्तम्_गन्तव्यम्_ORIGINAL;
	} अन्यथा यदि (argc && skip_उपसर्गः(argv[0], "--reset-when-found=",
				    &reset_when_प्राप्तम्_arg)) {
		यदि (parse_reset_when_प्राप्तम्(reset_when_प्राप्तम्_arg,
					   &reset_when_प्राप्तम्))
			प्रत्यागम BISECT_FAILED;
	}

	यदि (reset_when_प्राप्तम् != RESET_WHEN_प्राप्तम्_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		प्रत्यागम error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	यदि (reset_when_प्राप्तम् != RESET_WHEN_प्राप्तम्_NONE) {
		write_file(git_मार्गः_bisect_reset_when_प्राप्तम्(), "%s\n",
			   reset_when_प्राप्तम्_बहुलकम्_name(reset_when_प्राप्तम्));
		argc--;
		argv++;
	}

	यदि (!argc) {
		error(_("bisect run failed: no command provided."));
		प्रत्यागम BISECT_FAILED;
	}

	sq_quote_argv(&command, argv);
	strbuf_ltrim(&command);
	यावत् (1) {
		res = do_bisect_सततम्(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		यदि (is_first_सततम् && (res == 126 || res == 127)) {
			पूर्णाङ्क rc = verify_good(terms, command.buf);
			is_first_सततम् = 0;
			यदि (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->पदांशः_good);
				res = BISECT_FAILED;
				भङ्ग;
			}
			यदि (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->पदांशः_good);
				res = BISECT_FAILED;
				भङ्ग;
			}
		}

		यदि (res < 0 || 128 <= res) {
			error(_("bisect run failed: exit code %d from"
				" %s is < 0 or >= 128"), res, command.buf);
			भङ्ग;
		}

		यदि (res == 125)
			new_state = "skip";
		अन्यथा यदि (!res)
			new_state = terms->पदांशः_good;
		अन्यथा
			new_state = terms->पदांशः_bad;

		temporary_stdout_fd = उद्घाटय(git_मार्गः_bisect_सततम्(), O_CREAT | O_WRONLY | O_TRUNC, 0666);

		यदि (temporary_stdout_fd < 0) {
			res = error_errno(_("cannot open file '%s' for writing"), git_मार्गः_bisect_सततम्());
			भङ्ग;
		}

		fflush(stdout);
		saved_stdout = dup(1);
		यदि (saved_stdout < 0 ||
		    dup2(temporary_stdout_fd, 1) < 0) {
			res = error_errno(_("could not duplicate stdout"));
			यदि (saved_stdout >= 0)
				close(saved_stdout);
			close(temporary_stdout_fd);
			भङ्ग;
		}

		res = bisect_state(terms, 1, &new_state);

		fflush(stdout);
		dup2(saved_stdout, 1);
		close(saved_stdout);
		close(temporary_stdout_fd);

		print_file_गन्तव्यम्_stdout(git_मार्गः_bisect_सततम्());

		यदि (res == BISECT_ONLY_SKIPPED_वामः)
			error(_("bisect run cannot continue any more"));
		अन्यथा यदि (res == BISECT_आन्तरिकम्_SUCCESS_MERGE_आधारः) {
			पङ्क्तिं_लिख(_("bisect run success"));
			res = BISECT_OK;
		} अन्यथा यदि (res == BISECT_आन्तरिकम्_SUCCESS_1_ST_BAD_प्राप्तम्) {
			प्रारूप_लिख(_("bisect found first '%s' commit\n"), terms->पदांशः_bad);
		} अन्यथा यदि (res) {
			error(_("bisect run failed: 'git bisect %s'"
				" exited with error code %d"), new_state, res);
		} अन्यथा {
			अनुवर्त;
		}
		भङ्ग;
	}

	strbuf_release(&command);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क cmd_bisect__reset(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः अप्रयुक्तम्,
			     संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क res;

	यदि (argc > 1)
		प्रत्यागम error(_("'%s' requires either no argument or a commit"),
			     "git bisect reset");
	res = bisect_reset(argc ? argv[0] : रिक्त, false);
	यदि (res)
		प्रत्यागम res;
	प्रत्यागम bisect_clean_state();
}

स्थिर पूर्णाङ्क cmd_bisect__terms(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः अप्रयुक्तम्,
			     संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क res;
	संरचना bisect_terms terms = { 0 };

	यदि (argc > 1)
		प्रत्यागम error(_("'%s' requires 0 or 1 argument"),
			     "git bisect terms");
	res = bisect_terms(&terms, argc == 1 ? argv[0] : रिक्त);
	free_terms(&terms);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क cmd_bisect__start(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः अप्रयुक्तम्,
			     संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क res;
	संरचना bisect_terms terms = { 0 };

	समुच्चयः_terms(&terms, "bad", "good");
	res = bisect_start(&terms, argc, argv);
	free_terms(&terms);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क cmd_bisect_अग्र(पूर्णाङ्क argc, ध्रुव वर्ण **argv अप्रयुक्तम्, ध्रुव वर्ण *उपसर्गः,
			    संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क res;
	संरचना bisect_terms terms = { 0 };

	यदि (argc)
		प्रत्यागम error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	यदि (get_terms(&terms, 1))
		प्रत्यागम error(_("no terms defined"));
	res = bisect_अग्र(&terms, उपसर्गः);
	free_terms(&terms);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क cmd_bisect__log(पूर्णाङ्क argc अप्रयुक्तम्, ध्रुव वर्ण **argv अप्रयुक्तम्,
			   ध्रुव वर्ण *उपसर्गः अप्रयुक्तम्,
			   संरचना repository *repo अप्रयुक्तम्)
{
	प्रत्यागम bisect_log();
}

स्थिर पूर्णाङ्क cmd_bisect__replay(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः अप्रयुक्तम्,
			      संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क res;
	संरचना bisect_terms terms = { 0 };

	यदि (argc != 1)
		प्रत्यागम error(_("no logfile given"));
	समुच्चयः_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क cmd_bisect__skip(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः अप्रयुक्तम्,
			    संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क res;
	संरचना bisect_terms terms = { 0 };

	समुच्चयः_terms(&terms, "bad", "good");
	यदि (get_terms(&terms, 1))
		प्रत्यागम error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क cmd_bisect__visualize(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः अप्रयुक्तम्,
				 संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क res;
	संरचना bisect_terms terms = { 0 };

	यदि (get_terms(&terms, 1))
		प्रत्यागम error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क cmd_bisect_सततम्(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः अप्रयुक्तम्,
			   संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क res;
	संरचना bisect_terms terms = { 0 };

	यदि (!argc)
		प्रत्यागम error(_("'%s' failed: no command provided."), "git bisect run");
	यदि (get_terms(&terms, 1))
		प्रत्यागम error(_("no terms defined"));
	res = bisect_सततम्(&terms, argc, argv);
	free_terms(&terms);
	प्रत्यागम res;
}

पूर्णाङ्क cmd_bisect(पूर्णाङ्क argc,
	       ध्रुव वर्ण **argv,
	       ध्रुव वर्ण *उपसर्गः,
	       संरचना repository *repo)
{
	पूर्णाङ्क res = 0;
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option options[] = {
		OPT_SUBCOMMAND("reset", &fn, cmd_bisect__reset),
		OPT_SUBCOMMAND("terms", &fn, cmd_bisect__terms),
		OPT_SUBCOMMAND("start", &fn, cmd_bisect__start),
		OPT_SUBCOMMAND("next", &fn, cmd_bisect_अग्र),
		OPT_SUBCOMMAND("log", &fn, cmd_bisect__log),
		OPT_SUBCOMMAND("replay", &fn, cmd_bisect__replay),
		OPT_SUBCOMMAND("skip", &fn, cmd_bisect__skip),
		OPT_SUBCOMMAND("visualize", &fn, cmd_bisect__visualize),
		OPT_SUBCOMMAND("view", &fn, cmd_bisect__visualize),
		OPT_SUBCOMMAND("run", &fn, cmd_bisect_सततम्),
		OPT_अन्तः()
	};
	argc = parse_options(argc, argv, उपसर्गः, options, git_bisect_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);

	यदि (!fn) {
		संरचना bisect_terms terms = { 0 };

		यदि (!argc)
			usage_सन्देशः_opt(_("need a command"), git_bisect_usage, options);

		यदि (!वाक्यं_तुलय(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		समुच्चयः_terms(&terms, "bad", "good");
		यदि (get_terms(&terms, 1))
			प्रत्यागम error(_("no terms defined"));
		यदि (check_and_समुच्चयः_terms(&terms, argv[0]) ||
		    !one_सम्बन्धि(argv[0], terms.पदांशः_good, terms.पदांशः_bad, रिक्त))
			usage_सन्देशः_optf(_("unknown command: '%s'"), git_bisect_usage,
				       options, argv[0]);
		अन्यथा
			res = bisect_state(&terms, argc, argv);
		free_terms(&terms);
	} अन्यथा {
		argc--;
		argv++;
		res = fn(argc, argv, उपसर्गः, repo);
	}

	यदि (res == BISECT_आन्तरिकम्_SUCCESS_1_ST_BAD_प्राप्तम्) {
		गणना reset_when_प्राप्तम्_बहुलकम् बहुलकम्;

		यदि (read_reset_when_प्राप्तम्(&बहुलकम्))
			res = BISECT_FAILED;
		अन्यथा यदि (बहुलकम् != RESET_WHEN_प्राप्तम्_NONE &&
			 bisect_reset_when_प्राप्तम्(बहुलकम्))
			res = BISECT_FAILED;
	}

	प्रत्यागम is_bisect_success(res) ? 0 : -res;
}
