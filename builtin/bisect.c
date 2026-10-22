#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "copy.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "bisect.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "strvec.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "oid-array.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "prompt.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "revision.h"

స్థిర GIT_మార్గం_FUNC(git_మార్గం_bisect_terms, "BISECT_TERMS")
స్థిర GIT_మార్గం_FUNC(git_మార్గం_bisect_ancestors_ok, "BISECT_ANCESTORS_OK")
స్థిర GIT_మార్గం_FUNC(git_మార్గం_bisect_ప్రారంభం, "BISECT_START")
స్థిర GIT_మార్గం_FUNC(git_మార్గం_bisect_log, "BISECT_LOG")
స్థిర GIT_మార్గం_FUNC(git_మార్గం_bisect_పేర్లు, "BISECT_NAMES")
స్థిర GIT_మార్గం_FUNC(git_మార్గం_bisect_మొదటి_మాతృ, "BISECT_FIRST_PARENT")
స్థిర GIT_మార్గం_FUNC(git_మార్గం_bisect_reset_when_దొరికింది, "BISECT_RESET_WHEN_FOUND")
స్థిర GIT_మార్గం_FUNC(git_మార్గం_bisect_నిరంతరం, "BISECT_RUN")

#నిర్వచించు BUILTIN_GIT_BISECT_ప్రారంభం_USAGE \
	N_("git bisect start [--term-(bad|new)=<term-new> --term-(good|old)=<term-old>]\n" \
	   "                 [--no-checkout] [--first-parent] [--reset-when-found[=<where>]] [<bad> [<good>...]] [--] [<pathspec>...]")
#నిర్వచించు BUILTIN_GIT_BISECT_BAD_USAGE \
	N_("git bisect (bad|new|<term-new>) [<rev>]")
#నిర్వచించు BUILTIN_GIT_BISECT_GOOD_USAGE \
	N_("git bisect (good|old|<term-old>) [<rev>...]")
#నిర్వచించు BUILTIN_GIT_BISECT_TERMS_USAGE \
	"git bisect terms [--term-(good|old) | --term-(bad|new)]"
#నిర్వచించు BUILTIN_GIT_BISECT_SKIP_USAGE \
	N_("git bisect skip [(<rev>|<range>)...]")
#నిర్వచించు BUILTIN_GIT_BISECT_తదుపరి_USAGE \
	"git bisect next"
#నిర్వచించు BUILTIN_GIT_BISECT_RESET_USAGE \
	N_("git bisect reset [<commit>]")
#నిర్వచించు BUILTIN_GIT_BISECT_VISUALIZE_USAGE \
	"git bisect (visualize|view)"
#నిర్వచించు BUILTIN_GIT_BISECT_REPLAY_USAGE \
	N_("git bisect replay <logfile>")
#నిర్వచించు BUILTIN_GIT_BISECT_LOG_USAGE \
	"git bisect log"
#నిర్వచించు BUILTIN_GIT_BISECT_నిరంతరం_USAGE \
	N_("git bisect run [--reset-when-found[=<where>]] <cmd> [<arg>...]")
#నిర్వచించు BUILTIN_GIT_BISECT_HELP_USAGE \
	"git bisect help"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_bisect_usage[] = {
	BUILTIN_GIT_BISECT_ప్రారంభం_USAGE,
	BUILTIN_GIT_BISECT_BAD_USAGE,
	BUILTIN_GIT_BISECT_GOOD_USAGE,
	BUILTIN_GIT_BISECT_TERMS_USAGE,
	BUILTIN_GIT_BISECT_SKIP_USAGE,
	BUILTIN_GIT_BISECT_తదుపరి_USAGE,
	BUILTIN_GIT_BISECT_RESET_USAGE,
	BUILTIN_GIT_BISECT_VISUALIZE_USAGE,
	BUILTIN_GIT_BISECT_REPLAY_USAGE,
	BUILTIN_GIT_BISECT_LOG_USAGE,
	BUILTIN_GIT_BISECT_నిరంతరం_USAGE,
	BUILTIN_GIT_BISECT_HELP_USAGE,
	ఖాళీ
};

గణన reset_when_దొరికింది_బహుళకం {
	RESET_WHEN_దొరికింది_NONE,
	RESET_WHEN_దొరికింది_గమ్యం_ORIGINAL,
	RESET_WHEN_దొరికింది_గమ్యం_దొరికింది,
};

నిర్మాణం కూడు_bisect_ref_దత్తాంశం {
	నిర్మాణం rev_info *revs;
	గుర్తులేని పూర్ణాంకం object_flags;
};

నిర్మాణం bisect_terms {
	అక్షరం *పదం_good;
	అక్షరం *పదం_bad;
};

స్థిర శూన్య free_terms(నిర్మాణం bisect_terms *terms)
{
	FREE_AND_NULL(terms->పదం_good);
	FREE_AND_NULL(terms->పదం_bad);
}

స్థిర శూన్య సమితి_terms(నిర్మాణం bisect_terms *terms, స్థిరరాశి అక్షరం *bad,
		      స్థిరరాశి అక్షరం *good)
{
	విడుదల((శూన్య *)terms->పదం_good);
	terms->పదం_good = xstrdup(good);
	విడుదల((శూన్య *)terms->పదం_bad);
	terms->పదం_bad = xstrdup(bad);
}

స్థిర స్థిరరాశి అక్షరం vocab_bad[] = "bad|new";
స్థిర స్థిరరాశి అక్షరం vocab_good[] = "good|old";

స్థిర పూర్ణాంకం bisect_autostart(నిర్మాణం bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
చివరి_ARG_MUST_BE_NULL
స్థిర పూర్ణాంకం one_యొక్క(స్థిరరాశి అక్షరం *పదం, ...)
{
	పూర్ణాంకం res = 0;
	va_list matches;
	స్థిరరాశి అక్షరం *సరిపోలిక;

	va_ప్రారంభం(matches, పదం);
	అయితే (!res && (సరిపోలిక = va_arg(matches, స్థిరరాశి అక్షరం *)))
		res = !strcmp(పదం, సరిపోలిక);
	va_చివర(matches);

	తిరిగి res;
}

/*
 * return code BISECT_INTERNAL_SUCCESS_MERGE_BASE
 * and BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND are codes
 * that indicate special success.
 */

స్థిర పూర్ణాంకం is_bisect_success(గణన bisect_error res)
{
	తిరిగి !res ||
		res == BISECT_అంతర్గత_SUCCESS_1_ST_BAD_దొరికింది ||
		res == BISECT_అంతర్గత_SUCCESS_విలీనం_పునాది;
}

స్థిర పూర్ణాంకం write_లోపల_file(స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *బహుళకం, స్థిరరాశి అక్షరం *format, va_list వాదనలు)
{
	FILE *fp = ఖాళీ;
	పూర్ణాంకం res = 0;

	ఒకవేళ (strcmp(బహుళకం, "w") && strcmp(బహుళకం, "a"))
		BUG("write-in-file does not support '%s' mode", బహుళకం);
	fp = fopen(మార్గం, బహుళకం);
	ఒకవేళ (!fp)
		తిరిగి error_errno(_("cannot open file '%s' in mode '%s'"), మార్గం, బహుళకం);
	res = vfprintf(fp, format, వాదనలు);

	ఒకవేళ (res < 0) {
		పూర్ణాంకం saved_errno = errno;
		fclose(fp);
		errno = saved_errno;
		తిరిగి error_errno(_("could not write to file '%s'"), మార్గం);
	}

	తిరిగి fclose(fp);
}

__attribute__((format (రాయి, 2, 3)))
స్థిర పూర్ణాంకం write_గమ్యం_file(స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *format, ...)
{
	పూర్ణాంకం res;
	va_list వాదనలు;

	va_ప్రారంభం(వాదనలు, format);
	res = write_లోపల_file(మార్గం, "w", format, వాదనలు);
	va_చివర(వాదనలు);

	తిరిగి res;
}

__attribute__((format (రాయి, 2, 3)))
స్థిర పూర్ణాంకం append_గమ్యం_file(స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *format, ...)
{
	పూర్ణాంకం res;
	va_list వాదనలు;

	va_ప్రారంభం(వాదనలు, format);
	res = write_లోపల_file(మార్గం, "a", format, వాదనలు);
	va_చివర(వాదనలు);

	తిరిగి res;
}

స్థిర పూర్ణాంకం print_file_గమ్యం_stdout(స్థిరరాశి అక్షరం *మార్గం)
{
	పూర్ణాంకం fd = open(మార్గం, O_RDONLY);
	పూర్ణాంకం ret = 0;

	ఒకవేళ (fd < 0)
		తిరిగి error_errno(_("cannot open file '%s' for reading"), మార్గం);
	ఒకవేళ (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), మార్గం);
	close(fd);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం check_పదం_format(స్థిరరాశి అక్షరం *పదం, స్థిరరాశి అక్షరం *orig_పదం)
{
	పూర్ణాంకం res;
	అక్షరం *new_పదం = xstrfmt("refs/bisect/%s", పదం);

	res = check_refname_format(new_పదం, 0);
	విడుదల(new_పదం);

	ఒకవేళ (res)
		తిరిగి error(_("'%s' is not a valid term"), పదం);

	ఒకవేళ (one_యొక్క(పదం, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", ఖాళీ))
		తిరిగి error(_("can't use the builtin command '%s' as a term"), పదం);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	ఒకవేళ ((strcmp(orig_పదం, "bad") && one_యొక్క(పదం, "bad", "new", ఖాళీ)) ||
		 (strcmp(orig_పదం, "good") && one_యొక్క(పదం, "good", "old", ఖాళీ)))
		తిరిగి error(_("can't change the meaning of the term '%s'"), పదం);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం write_terms(స్థిరరాశి అక్షరం *bad, స్థిరరాశి అక్షరం *good)
{
	పూర్ణాంకం res;

	ఒకవేళ (!strcmp(bad, good))
		తిరిగి error(_("please use two different terms"));

	ఒకవేళ (check_పదం_format(bad, "bad") || check_పదం_format(good, "good"))
		తిరిగి -1;

	res = write_గమ్యం_file(git_మార్గం_bisect_terms(), "%s\n%s\n", bad, good);

	తిరిగి res;
}

స్థిర పూర్ణాంకం bisect_reset(స్థిరరాశి అక్షరం *commit, bool quiet)
{
	నిర్మాణం strbuf branch = STRBUF_INIT;

	ఒకవేళ (!commit) {
		ఒకవేళ (!strbuf_read_file(&branch, git_మార్గం_bisect_ప్రారంభం(), 0))
			రాయి(_("We are not bisecting.\n"));
		లేకపోతే
			strbuf_rtrim(&branch);
	} లేకపోతే {
		నిర్మాణం object_id oid;

		ఒకవేళ (repo_get_oid_commit(the_repository, commit, &oid))
			తిరిగి error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	ఒకవేళ (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.వాదనలు, "checkout", "--ignore-other-worktrees", ఖాళీ);
		ఒకవేళ (quiet)
			strvec_నెట్టు(&cmd.వాదనలు, "--quiet");
		strvec_pushl(&cmd.వాదనలు, branch.buf, "--", ఖాళీ);
		ఒకవేళ (నిరంతరం_command(&cmd)) {
			error(_("could not check out original"
				" HEAD '%s'. Try 'git bisect"
				" reset <commit>'."), branch.buf);
			strbuf_release(&branch);
			తిరిగి -1;
		}
	}

	strbuf_release(&branch);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం parse_reset_when_దొరికింది(స్థిరరాశి అక్షరం *విలువ,
				  గణన reset_when_దొరికింది_బహుళకం *బహుళకం)
{
	ఒకవేళ (!strcmp(విలువ, "original"))
		*బహుళకం = RESET_WHEN_దొరికింది_గమ్యం_ORIGINAL;
	లేకపోతే ఒకవేళ (!strcmp(విలువ, "found"))
		*బహుళకం = RESET_WHEN_దొరికింది_గమ్యం_దొరికింది;
	లేకపోతే
		తిరిగి error(_("invalid value for '--reset-when-found': '%s'"),
			     విలువ);

	తిరిగి 0;
}

స్థిర స్థిరరాశి అక్షరం *reset_when_దొరికింది_బహుళకం_name(గణన reset_when_దొరికింది_బహుళకం బహుళకం)
{
	ఎంపిక (బహుళకం) {
	స్థితి RESET_WHEN_దొరికింది_గమ్యం_ORIGINAL:
		తిరిగి "original";
	స్థితి RESET_WHEN_దొరికింది_గమ్యం_దొరికింది:
		తిరిగి "found";
	స్థితి RESET_WHEN_దొరికింది_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", బహుళకం);
}

స్థిర పూర్ణాంకం read_reset_when_దొరికింది(గణన reset_when_దొరికింది_బహుళకం *బహుళకం)
{
	నిర్మాణం strbuf విలువ = STRBUF_INIT;
	పూర్ణాంకం res = 0;

	*బహుళకం = RESET_WHEN_దొరికింది_NONE;
	ఒకవేళ (is_ఖాళీలు_or_missing_file(git_మార్గం_bisect_reset_when_దొరికింది()))
		తిరిగి 0;

	ఒకవేళ (strbuf_read_file(&విలువ, git_మార్గం_bisect_reset_when_దొరికింది(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_మార్గం_bisect_reset_when_దొరికింది());
		వెళ్ళు బహిర్గతం;
	}
	strbuf_trim(&విలువ);
	ఒకవేళ (parse_reset_when_దొరికింది(విలువ.buf, బహుళకం))
		res = -1;

బహిర్గతం:
	strbuf_release(&విలువ);
	తిరిగి res;
}

స్థిర పూర్ణాంకం bisect_reset_when_దొరికింది(గణన reset_when_దొరికింది_బహుళకం బహుళకం)
{
	నిర్మాణం bisect_terms terms = { 0 };
	అక్షరం *commit = ఖాళీ;
	పూర్ణాంకం res;

	ఒకవేళ (బహుళకం == RESET_WHEN_దొరికింది_గమ్యం_దొరికింది) {
		read_bisect_terms(&terms.పదం_bad, &terms.పదం_good);
		commit = xstrfmt("refs/bisect/%s", terms.పదం_bad);
	} లేకపోతే ఒకవేళ (బహుళకం == RESET_WHEN_దొరికింది_NONE) {
		BUG("automatic reset requested without a reset mode");
	}

	res = bisect_reset(commit, true);
	ఒకవేళ (!res)
		res = bisect_clean_state();

	విడుదల(commit);
	free_terms(&terms);
	తిరిగి res;
}

స్థిర శూన్య log_commit(FILE *fp,
		       స్థిరరాశి అక్షరం *fmt, స్థిరరాశి అక్షరం *state,
		       నిర్మాణం commit *commit)
{
	నిర్మాణం pretty_print_context pp = {0};
	నిర్మాణం strbuf commit_సందేశం = STRBUF_INIT;
	అక్షరం *label = xstrfmt(fmt, state);

	repo_format_commit_సందేశం(the_repository, commit, "%s", &commit_సందేశం,
				   &pp);

	fprintf(fp, "# %s: [%s] %s\n", label, oid_గమ్యం_hex(&commit->object.oid),
		commit_సందేశం.buf);

	strbuf_release(&commit_సందేశం);
	విడుదల(label);
}

స్థిర పూర్ణాంకం bisect_write(స్థిరరాశి అక్షరం *state, స్థిరరాశి అక్షరం *rev,
			స్థిరరాశి నిర్మాణం bisect_terms *terms, పూర్ణాంకం nolog)
{
	నిర్మాణం strbuf ట్యాగ్ = STRBUF_INIT;
	నిర్మాణం object_id oid;
	నిర్మాణం commit *commit;
	FILE *fp = ఖాళీ;
	పూర్ణాంకం res = 0;

	ఒకవేళ (!strcmp(state, terms->పదం_bad)) {
		strbuf_addf(&ట్యాగ్, "refs/bisect/%s", state);
	} లేకపోతే ఒకవేళ (one_యొక్క(state, terms->పదం_good, "skip", ఖాళీ)) {
		strbuf_addf(&ట్యాగ్, "refs/bisect/%s-%s", state, rev);
	} లేకపోతే {
		res = error(_("Bad bisect_write argument: %s"), state);
		వెళ్ళు ముగింపు;
	}

	ఒకవేళ (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		వెళ్ళు ముగింపు;
	}

	ఒకవేళ (refs_update_ref(get_main_ref_store(the_repository), ఖాళీ, ట్యాగ్.buf, &oid, ఖాళీ, 0,
			    UPDATE_REFS_సందేశం_ON_ERR)) {
		res = -1;
		వెళ్ళు ముగింపు;
	}

	fp = fopen(git_మార్గం_bisect_log(), "a");
	ఒకవేళ (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_మార్గం_bisect_log());
		వెళ్ళు ముగింపు;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	ఒకవేళ (!nolog)
		fprintf(fp, "git bisect %s %s\n", state, rev);

ముగింపు:
	ఒకవేళ (fp)
		fclose(fp);
	strbuf_release(&ట్యాగ్);
	తిరిగి res;
}

స్థిర పూర్ణాంకం check_and_సమితి_terms(నిర్మాణం bisect_terms *terms, స్థిరరాశి అక్షరం *cmd)
{
	పూర్ణాంకం has_పదం_file = !is_ఖాళీలు_or_missing_file(git_మార్గం_bisect_terms());

	ఒకవేళ (one_యొక్క(cmd, "skip", "start", "terms", ఖాళీ))
		తిరిగి 0;

	ఒకవేళ (has_పదం_file && strcmp(cmd, terms->పదం_bad) &&
	    strcmp(cmd, terms->పదం_good))
		తిరిగి error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->పదం_bad,
				terms->పదం_good);

	ఒకవేళ (!has_పదం_file) {
		ఒకవేళ (one_యొక్క(cmd, "bad", "good", ఖాళీ)) {
			సమితి_terms(terms, "bad", "good");
			తిరిగి write_terms(terms->పదం_bad, terms->పదం_good);
		}
		ఒకవేళ (one_యొక్క(cmd, "new", "old", ఖాళీ)) {
			సమితి_terms(terms, "new", "old");
			తిరిగి write_terms(terms->పదం_bad, terms->పదం_good);
		}
	}

	తిరిగి 0;
}

స్థిర పూర్ణాంకం inc_nr(స్థిరరాశి నిర్మాణం reference *ref వాడని, శూన్య *cb_దత్తాంశం)
{
	గుర్తులేని పూర్ణాంకం *nr = (గుర్తులేని పూర్ణాంకం *)cb_దత్తాంశం;
	(*nr)++;
	తిరిగి 0;
}

స్థిర స్థిరరాశి అక్షరం need_bad_and_good_revision_warning[] =
	N_("You need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

స్థిర స్థిరరాశి అక్షరం need_bisect_ప్రారంభం_warning[] =
	N_("You need to start by \"git bisect start\".\n"
	   "You then need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

స్థిర పూర్ణాంకం decide_తదుపరి(స్థిరరాశి నిర్మాణం bisect_terms *terms,
		       స్థిరరాశి అక్షరం *current_పదం, పూర్ణాంకం missing_good,
		       పూర్ణాంకం missing_bad)
{
	ఒకవేళ (!missing_good && !missing_bad)
		తిరిగి 0;
	ఒకవేళ (!current_పదం)
		తిరిగి -1;

	ఒకవేళ (missing_good && !missing_bad &&
	    !strcmp(current_పదం, terms->పదం_good)) {
		అక్షరం *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->పదం_bad);
		ఒకవేళ (!isatty(0))
			తిరిగి 0;
		/*
		 * TRANSLATORS: Make sure to include [Y] and [n] in your
		 * translation. The program will only accept English input
		 * at this point.
		 */
		yesno = git_prompt(_("Are you sure [Y/n]? "), PROMPT_ECHO);
		ఒకవేళ (starts_with(yesno, "N") || starts_with(yesno, "n"))
			తిరిగి -1;
		తిరిగి 0;
	}

	ఒకవేళ (!is_ఖాళీలు_or_missing_file(git_మార్గం_bisect_ప్రారంభం()))
		తిరిగి error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	లేకపోతే
		తిరిగి error(_(need_bisect_ప్రారంభం_warning),
			     vocab_good, vocab_bad, vocab_good, vocab_bad);
}

స్థిర శూన్య bisect_స్థితి(నిర్మాణం bisect_state *state,
			  స్థిరరాశి నిర్మాణం bisect_terms *terms)
{
	అక్షరం *bad_ref = xstrfmt("refs/bisect/%s", terms->పదం_bad);
	అక్షరం *good_glob = xstrfmt("%s-*", terms->పదం_good);
	నిర్మాణం refs_for_each_ref_options opts = {
		.నమూనా = good_glob,
		.ఉపసర్గ = "refs/bisect/",
		.trim_ఉపసర్గ = అక్షరశ్రేణి_పొడవు("refs/bisect/"),
	};

	ఒకవేళ (refs_ref_exists(get_main_ref_store(the_repository), bad_ref))
		state->nr_bad = 1;

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      inc_nr, &state->nr_good, &opts);

	విడుదల(good_glob);
	విడుదల(bad_ref);
}

__attribute__((format (రాయి, 1, 2)))
స్థిర శూన్య bisect_log_printf(స్థిరరాశి అక్షరం *fmt, ...)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	va_list ap;

	va_ప్రారంభం(ap, fmt);
	strbuf_vaddf(&buf, fmt, ap);
	va_చివర(ap);

	రాయి("%s", buf.buf);
	append_గమ్యం_file(git_మార్గం_bisect_log(), "# %s", buf.buf);

	strbuf_release(&buf);
}

స్థిర శూన్య bisect_print_స్థితి(స్థిరరాశి నిర్మాణం bisect_terms *terms)
{
	నిర్మాణం bisect_state state = { 0 };

	bisect_స్థితి(&state, terms);

	/* If we had both, we'd already be started, and shouldn't get here. */
	ఒకవేళ (state.nr_good && state.nr_bad)
		తిరిగి;

	ఒకవేళ (!state.nr_good && !state.nr_bad)
		bisect_log_printf(_("status: waiting for both '%s' and '%s' commits\n"),
				  terms->పదం_good, terms->పదం_bad);
	లేకపోతే ఒకవేళ (state.nr_good)
		bisect_log_printf(Q_("status: waiting for '%s' commit, %d '%s' commit known\n",
				     "status: waiting for '%s' commit, %d '%s' commits known\n",
				     state.nr_good),
				  terms->పదం_bad, state.nr_good, terms->పదం_good);
	లేకపోతే
		bisect_log_printf(_("status: waiting for '%s' commit(s), '%s' commit known\n"),
				  terms->పదం_good, terms->పదం_bad);
}

స్థిర పూర్ణాంకం bisect_తదుపరి_check(స్థిరరాశి నిర్మాణం bisect_terms *terms,
			     స్థిరరాశి అక్షరం *current_పదం)
{
	నిర్మాణం bisect_state state = { 0 };
	bisect_స్థితి(&state, terms);
	తిరిగి decide_తదుపరి(terms, current_పదం, !state.nr_good, !state.nr_bad);
}

స్థిర పూర్ణాంకం get_terms(నిర్మాణం bisect_terms *terms, పూర్ణాంకం file_missing_is_ok)
{
	నిర్మాణం strbuf str = STRBUF_INIT;
	FILE *fp = ఖాళీ;
	పూర్ణాంకం res = 0;

	fp = fopen(git_మార్గం_bisect_terms(), "r");
	ఒకవేళ (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		వెళ్ళు ముగింపు;
	}

	free_terms(terms);
	ఒకవేళ (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		వెళ్ళు ముగింపు;
	}
	terms->పదం_bad = strbuf_detach(&str, ఖాళీ);
	ఒకవేళ (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		FREE_AND_NULL(terms->పదం_bad);
		వెళ్ళు ముగింపు;
	}
	terms->పదం_good = strbuf_detach(&str, ఖాళీ);

ముగింపు:
	ఒకవేళ (fp)
		fclose(fp);
	strbuf_release(&str);
	తిరిగి res;
}

స్థిర పూర్ణాంకం bisect_terms(నిర్మాణం bisect_terms *terms, స్థిరరాశి అక్షరం *option)
{
	ఒకవేళ (get_terms(terms, 0))
		తిరిగి error(_("no terms defined"));

	ఒకవేళ (!option) {
		రాయి(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->పదం_good, terms->పదం_bad);
		తిరిగి 0;
	}
	ఒకవేళ (one_యొక్క(option, "--term-good", "--term-old", ఖాళీ))
		రాయి("%s\n", terms->పదం_good);
	లేకపోతే ఒకవేళ (one_యొక్క(option, "--term-bad", "--term-new", ఖాళీ))
		రాయి("%s\n", terms->పదం_bad);
	లేకపోతే
		తిరిగి error(_("invalid argument %s for 'git bisect terms'.\n"
			       "Supported options are: "
			       "--term-good|--term-old and "
			       "--term-bad|--term-new."), option);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం bisect_append_log_quoted(స్థిరరాశి అక్షరం **argv)
{
	పూర్ణాంకం res = 0;
	FILE *fp = fopen(git_మార్గం_bisect_log(), "a");
	నిర్మాణం strbuf orig_వాదనలు = STRBUF_INIT;

	ఒకవేళ (!fp)
		తిరిగి -1;

	ఒకవేళ (fprintf(fp, "git bisect start") < 1) {
		res = -1;
		వెళ్ళు ముగింపు;
	}

	sq_quote_argv(&orig_వాదనలు, argv);
	ఒకవేళ (fprintf(fp, "%s\n", orig_వాదనలు.buf) < 1)
		res = -1;

ముగింపు:
	fclose(fp);
	strbuf_release(&orig_వాదనలు);
	తిరిగి res;
}

స్థిర పూర్ణాంకం కూడు_bisect_ref(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cb)
{
	నిర్మాణం కూడు_bisect_ref_దత్తాంశం *దత్తాంశం = cb;

	కూడు_pending_oid(దత్తాంశం->revs, ref->name, ref->oid, దత్తాంశం->object_flags);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం prepare_revs(నిర్మాణం bisect_terms *terms, నిర్మాణం rev_info *revs)
{
	నిర్మాణం refs_for_each_ref_options opts = {
		.ఉపసర్గ = "refs/bisect/",
		.trim_ఉపసర్గ = అక్షరశ్రేణి_పొడవు("refs/bisect/"),
	};
	పూర్ణాంకం res = 0;
	నిర్మాణం కూడు_bisect_ref_దత్తాంశం cb = { revs };
	అక్షరం *good = xstrfmt("%s-*", terms->పదం_good);

	/*
	 * We cannot use terms->term_bad directly in
	 * for_each_glob_ref_in() and we have to append a '*' to it,
	 * otherwise for_each_glob_ref_in() will append '/' and '*'.
	 */
	అక్షరం *bad = xstrfmt("%s*", terms->పదం_bad);

	/*
	 * It is important to reset the flags used by revision walks
	 * as the previous call to bisect_next_all() in turn
	 * sets up a revision walk.
	 */
	reset_revision_walk();
	repo_init_revisions(the_repository, revs, ఖాళీ);
	అమరిక_revisions(0, ఖాళీ, revs, ఖాళీ);

	opts.నమూనా = bad;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      కూడు_bisect_ref, &cb, &opts);

	cb.object_flags = UNINTERESTING;
	opts.నమూనా = good;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      కూడు_bisect_ref, &cb, &opts);

	ఒకవేళ (prepare_revision_walk(revs))
		res = error(_("revision walk setup failed"));

	విడుదల(good);
	విడుదల(bad);
	తిరిగి res;
}

స్థిర పూర్ణాంకం bisect_skipped_commits(నిర్మాణం bisect_terms *terms)
{
	పూర్ణాంకం res;
	FILE *fp = ఖాళీ;
	నిర్మాణం rev_info revs;
	నిర్మాణం commit *commit;
	నిర్మాణం pretty_print_context pp = {0};
	నిర్మాణం strbuf commit_name = STRBUF_INIT;

	res = prepare_revs(terms, &revs);
	ఒకవేళ (res)
		తిరిగి res;

	fp = fopen(git_మార్గం_bisect_log(), "a");
	ఒకవేళ (!fp)
		తిరిగి error_errno(_("could not open '%s' for appending"),
				  git_మార్గం_bisect_log());

	ఒకవేళ (fprintf(fp, "# only skipped commits left to test\n") < 0)
		తిరిగి error_errno(_("failed to write to '%s'"), git_మార్గం_bisect_log());

	అయితే ((commit = get_revision(&revs)) != ఖాళీ) {
		strbuf_reset(&commit_name);
		repo_format_commit_సందేశం(the_repository, commit, "%s",
					   &commit_name, &pp);
		fprintf(fp, "# possible first '%s' commit: [%s] %s\n",
			terms->పదం_bad, oid_గమ్యం_hex(&commit->object.oid),
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
	తిరిగి 0;
}

స్థిర పూర్ణాంకం bisect_successful(నిర్మాణం bisect_terms *terms)
{
	నిర్మాణం object_id oid;
	నిర్మాణం commit *commit;
	నిర్మాణం pretty_print_context pp = {0};
	నిర్మాణం strbuf commit_name = STRBUF_INIT;
	అక్షరం *bad_ref = xstrfmt("refs/bisect/%s",terms->పదం_bad);
	పూర్ణాంకం res;

	refs_read_ref(get_main_ref_store(the_repository), bad_ref, &oid);
	commit = lookup_commit_reference_by_name(bad_ref);
	ఒకవేళ (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		విడుదల(bad_ref);
		తిరిగి BISECT_FAILED;
	}
	repo_format_commit_సందేశం(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_గమ్యం_file(git_మార్గం_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->పదం_bad, oid_గమ్యం_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	విడుదల(bad_ref);
	తిరిగి res;
}

స్థిర గణన bisect_error bisect_తదుపరి(నిర్మాణం bisect_terms *terms,
				     స్థిరరాశి అక్షరం *ఉపసర్గ)
{
	గణన bisect_error res;

	ఒకవేళ (bisect_autostart(terms))
		తిరిగి BISECT_FAILED;

	ఒకవేళ (bisect_తదుపరి_check(terms, terms->పదం_good))
		తిరిగి BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_తదుపరి_all(the_repository, ఉపసర్గ);

	ఒకవేళ (res == BISECT_అంతర్గత_SUCCESS_1_ST_BAD_దొరికింది) {
		res = bisect_successful(terms);
		తిరిగి res ? res : BISECT_అంతర్గత_SUCCESS_1_ST_BAD_దొరికింది;
	} లేకపోతే ఒకవేళ (res == BISECT_ONLY_SKIPPED_ఎడమ) {
		res = bisect_skipped_commits(terms);
		తిరిగి res ? res : BISECT_ONLY_SKIPPED_ఎడమ;
	}
	తిరిగి res;
}

స్థిర గణన bisect_error bisect_auto_తదుపరి(నిర్మాణం bisect_terms *terms,
					  స్థిరరాశి అక్షరం *ఉపసర్గ)
{
	ఒకవేళ (bisect_తదుపరి_check(terms, ఖాళీ)) {
		bisect_print_స్థితి(terms);
		తిరిగి BISECT_OK;
	}

	తిరిగి bisect_తదుపరి(terms, ఉపసర్గ);
}

స్థిర గణన bisect_error bisect_ప్రారంభం(నిర్మాణం bisect_terms *terms, పూర్ణాంకం argc,
				      స్థిరరాశి అక్షరం **argv)
{
	పూర్ణాంకం no_checkout = 0;
	పూర్ణాంకం మొదటి_మాతృ_only = 0;
	పూర్ణాంకం i, has_double_dash = 0, must_write_terms = 0, bad_చూసిన = 0;
	పూర్ణాంకం flags, pathspec_pos;
	గణన bisect_error res = BISECT_OK;
	నిర్మాణం string_list revs = STRING_LIST_INIT_DUP;
	నిర్మాణం string_list స్థితులు = STRING_LIST_INIT_DUP;
	నిర్మాణం strbuf ప్రారంభం_తల = STRBUF_INIT;
	నిర్మాణం strbuf bisect_పేర్లు = STRBUF_INIT;
	నిర్మాణం object_id తల_oid;
	నిర్మాణం object_id oid;
	గణన reset_when_దొరికింది_బహుళకం reset_when_దొరికింది = RESET_WHEN_దొరికింది_NONE;
	స్థిరరాశి అక్షరం *తల;

	ఒకవేళ (is_bare_repository(the_repository))
		no_checkout = 1;

	/*
	 * Check for one bad and then some good revisions
	 */
	కొరకు (i = 0; i < argc; i++) {
		ఒకవేళ (!strcmp(argv[i], "--")) {
			has_double_dash = 1;
			ఆపు;
		}
	}

	కొరకు (i = 0; i < argc; i++) {
		స్థిరరాశి అక్షరం *arg = argv[i];
		ఒకవేళ (!strcmp(argv[i], "--")) {
			ఆపు;
		} లేకపోతే ఒకవేళ (!strcmp(arg, "--no-checkout")) {
			no_checkout = 1;
		} లేకపోతే ఒకవేళ (!strcmp(arg, "--first-parent")) {
			మొదటి_మాతృ_only = 1;
		} లేకపోతే ఒకవేళ (!strcmp(arg, "--reset-when-found")) {
			reset_when_దొరికింది = RESET_WHEN_దొరికింది_గమ్యం_ORIGINAL;
		} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(arg, "--reset-when-found=", &arg)) {
			ఒకవేళ (parse_reset_when_దొరికింది(arg, &reset_when_దొరికింది)) {
				res = BISECT_FAILED;
				వెళ్ళు ముగింపు;
			}
		} లేకపోతే ఒకవేళ (!strcmp(arg, "--term-good") ||
			 !strcmp(arg, "--term-old")) {
			i++;
			ఒకవేళ (argc <= i)
				తిరిగి error(_("'' is not a valid term"));
			must_write_terms = 1;
			విడుదల((శూన్య *) terms->పదం_good);
			terms->పదం_good = xstrdup(argv[i]);
		} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(arg, "--term-good=", &arg) ||
			   skip_ఉపసర్గ(arg, "--term-old=", &arg)) {
			must_write_terms = 1;
			విడుదల((శూన్య *) terms->పదం_good);
			terms->పదం_good = xstrdup(arg);
		} లేకపోతే ఒకవేళ (!strcmp(arg, "--term-bad") ||
			 !strcmp(arg, "--term-new")) {
			i++;
			ఒకవేళ (argc <= i)
				తిరిగి error(_("'' is not a valid term"));
			must_write_terms = 1;
			విడుదల((శూన్య *) terms->పదం_bad);
			terms->పదం_bad = xstrdup(argv[i]);
		} లేకపోతే ఒకవేళ (skip_ఉపసర్గ(arg, "--term-bad=", &arg) ||
			   skip_ఉపసర్గ(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			విడుదల((శూన్య *) terms->పదం_bad);
			terms->పదం_bad = xstrdup(arg);
		} లేకపోతే ఒకవేళ (starts_with(arg, "--")) {
			తిరిగి error(_("unrecognized option: '%s'"), arg);
		} లేకపోతే ఒకవేళ (!get_oidf(&oid, "%s^{commit}", arg)) {
			string_list_append(&revs, oid_గమ్యం_hex(&oid));
		} లేకపోతే ఒకవేళ (has_double_dash) {
			die(_("'%s' does not appear to be a valid "
			      "revision"), arg);
		} లేకపోతే {
			ఆపు;
		}
	}
	ఒకవేళ (reset_when_దొరికింది != RESET_WHEN_దొరికింది_NONE && no_checkout) {
		res = error(_("options '%s' and '%s' cannot be used together"),
			    "--reset-when-found", "--no-checkout");
		వెళ్ళు ముగింపు;
	}
	pathspec_pos = i;

	/*
	 * The user ran "git bisect start <sha1> <sha1>", hence did not
	 * explicitly specify the terms, but we are already starting to
	 * set references named with the default terms, and won't be able
	 * to change afterwards.
	 */
	ఒకవేళ (revs.nr)
		must_write_terms = 1;
	కొరకు (i = 0; i < revs.nr; i++) {
		ఒకవేళ (bad_చూసిన) {
			string_list_append(&స్థితులు, terms->పదం_good);
		} లేకపోతే {
			bad_చూసిన = 1;
			string_list_append(&స్థితులు, terms->పదం_bad);
		}
	}

	/*
	 * Verify HEAD
	 */
	తల = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
				       "HEAD", 0, &తల_oid, &flags);
	ఒకవేళ (!తల) {
		ఒకవేళ (repo_get_oid(the_repository, "HEAD", &తల_oid))
			తిరిగి error(_("bad HEAD - I need a HEAD"));
		తల = "HEAD";
	}

	/*
	 * Check if we are bisecting
	 */
	ఒకవేళ (!is_ఖాళీలు_or_missing_file(git_మార్గం_bisect_ప్రారంభం())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&ప్రారంభం_తల, git_మార్గం_bisect_ప్రారంభం(), 0);
		strbuf_trim(&ప్రారంభం_తల);
		ఒకవేళ (!no_checkout) {
			నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.వాదనలు, "checkout", ప్రారంభం_తల.buf,
				     "--", ఖాళీ);
			ఒకవేళ (నిరంతరం_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       ప్రారంభం_తల.buf);
				వెళ్ళు ముగింపు;
			}
		}
	} లేకపోతే {
		/* Get the rev from where we start. */
		ఒకవేళ (!repo_get_oid(the_repository, తల, &తల_oid) &&
		    !starts_with(తల, "refs/heads/")) {
			strbuf_reset(&ప్రారంభం_తల);
			strbuf_కూడు_oid_hex(&ప్రారంభం_తల, &తల_oid);
		} లేకపోతే ఒకవేళ (!repo_get_oid(the_repository, తల, &తల_oid) &&
			   skip_ఉపసర్గ(తల, "refs/heads/", &తల)) {
			strbuf_addstr(&ప్రారంభం_తల, తల);
		} లేకపోతే {
			తిరిగి error(_("bad HEAD - strange symbolic ref"));
		}
	}

	/*
	 * Get rid of any old bisect state.
	 */
	ఒకవేళ (bisect_clean_state())
		తిరిగి BISECT_FAILED;

	/*
	 * Write new start state
	 */
	write_file(git_మార్గం_bisect_ప్రారంభం(), "%s\n", ప్రారంభం_తల.buf);

	ఒకవేళ (మొదటి_మాతృ_only)
		write_file(git_మార్గం_bisect_మొదటి_మాతృ(), "\n");

	ఒకవేళ (reset_when_దొరికింది != RESET_WHEN_దొరికింది_NONE)
		write_file(git_మార్గం_bisect_reset_when_దొరికింది(), "%s\n",
			   reset_when_దొరికింది_బహుళకం_name(reset_when_దొరికింది));

	ఒకవేళ (no_checkout) {
		ఒకవేళ (repo_get_oid(the_repository, ప్రారంభం_తల.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), ప్రారంభం_తల.buf);
			వెళ్ళు ముగింపు;
		}
		ఒకవేళ (refs_update_ref(get_main_ref_store(the_repository), ఖాళీ, "BISECT_HEAD", &oid, ఖాళీ, 0,
				    UPDATE_REFS_సందేశం_ON_ERR)) {
			res = BISECT_FAILED;
			వెళ్ళు ముగింపు;
		}
	}

	ఒకవేళ (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_పేర్లు, argv + pathspec_pos);
	write_file(git_మార్గం_bisect_పేర్లు(), "%s\n", bisect_పేర్లు.buf);

	కొరకు (i = 0; i < స్థితులు.nr; i++)
		ఒకవేళ (bisect_write(స్థితులు.వస్తువులు[i].string,
				 revs.వస్తువులు[i].string, terms, 1)) {
			res = BISECT_FAILED;
			వెళ్ళు ముగింపు;
		}

	ఒకవేళ (must_write_terms && write_terms(terms->పదం_bad,
					    terms->పదం_good)) {
		res = BISECT_FAILED;
		వెళ్ళు ముగింపు;
	}

	res = bisect_append_log_quoted(argv);
	ఒకవేళ (res)
		res = BISECT_FAILED;

ముగింపు:
	string_list_clear(&revs, 0);
	string_list_clear(&స్థితులు, 0);
	strbuf_release(&ప్రారంభం_తల);
	strbuf_release(&bisect_పేర్లు);
	ఒకవేళ (res)
		తిరిగి res;

	res = bisect_auto_తదుపరి(terms, ఖాళీ);
	ఒకవేళ (!is_bisect_success(res))
		bisect_clean_state();
	తిరిగి res;
}

స్థిర అంతఃవరుస పూర్ణాంకం file_is_not_ఖాళీలు(స్థిరరాశి అక్షరం *మార్గం)
{
	తిరిగి !is_ఖాళీలు_or_missing_file(మార్గం);
}

స్థిర పూర్ణాంకం bisect_autostart(నిర్మాణం bisect_terms *terms)
{
	పూర్ణాంకం res;
	స్థిరరాశి అక్షరం *yesno;

	ఒకవేళ (file_is_not_ఖాళీలు(git_మార్గం_bisect_ప్రారంభం()))
		తిరిగి 0;

	fprintf_ln(stderr, _("You need to start by \"git bisect "
			  "start\"\n"));

	ఒకవేళ (!isatty(STDIN_FILENO))
		తిరిగి -1;

	/*
	 * TRANSLATORS: Make sure to include [Y] and [n] in your
	 * translation. The program will only accept English input
	 * at this point.
	 */
	yesno = git_prompt(_("Do you want me to do it for you "
			     "[Y/n]? "), PROMPT_ECHO);
	res = tolower(*yesno) == 'n' ?
		-1 : bisect_ప్రారంభం(terms, 0, ఖాళీలు_strvec);

	తిరిగి res;
}

స్థిర గణన bisect_error bisect_state(నిర్మాణం bisect_terms *terms, పూర్ణాంకం argc,
				      స్థిరరాశి అక్షరం **argv)
{
	స్థిరరాశి అక్షరం *state;
	పూర్ణాంకం i, verify_expected = 1;
	నిర్మాణం object_id oid, expected;
	నిర్మాణం oid_array revs = OID_ARRAY_INIT;

	ఒకవేళ (!argc)
		తిరిగి error(_("Please call `--bisect-state` with at least one argument"));

	ఒకవేళ (bisect_autostart(terms))
		తిరిగి BISECT_FAILED;

	state = argv[0];
	ఒకవేళ (check_and_సమితి_terms(terms, state) ||
	    !one_యొక్క(state, terms->పదం_good, terms->పదం_bad, "skip", ఖాళీ))
		తిరిగి BISECT_FAILED;

	argv++;
	argc--;
	ఒకవేళ (argc > 1 && !strcmp(state, terms->పదం_bad))
		తిరిగి error(_("'git bisect %s' can take only one argument."), terms->పదం_bad);

	ఒకవేళ (argc == 0) {
		స్థిరరాశి అక్షరం *తల = "BISECT_HEAD";
		గణన get_oid_ఫలితం res_తల = repo_get_oid(the_repository,
							    తల, &oid);

		ఒకవేళ (res_తల == MISSING_OBJECT) {
			తల = "HEAD";
			res_తల = repo_get_oid(the_repository, తల, &oid);
		}

		ఒకవేళ (res_తల)
			error(_("Bad rev input: %s"), తల);
		oid_array_append(&revs, &oid);
	}

	/*
	 * All input revs must be checked before executing bisect_write()
	 * to discard junk revs.
	 */

	కొరకు (; argc; argc--, argv++) {
		నిర్మాణం commit *commit;

		ఒకవేళ (repo_get_oid(the_repository, *argv, &oid)){
			error(_("Bad rev input: %s"), *argv);
			oid_array_clear(&revs);
			తిరిగి BISECT_FAILED;
		}

		commit = lookup_commit_reference(the_repository, &oid);
		ఒకవేళ (!commit)
			die(_("Bad rev input (not a commit): %s"), *argv);

		oid_array_append(&revs, &commit->object.oid);
	}

	ఒకవేళ (refs_read_ref(get_main_ref_store(the_repository), "BISECT_EXPECTED_REV", &expected))
		verify_expected = 0; /* Ignore invalid file contents */

	కొరకు (i = 0; i < revs.nr; i++) {
		ఒకవేళ (bisect_write(state, oid_గమ్యం_hex(&revs.oid[i]), terms, 0)) {
			oid_array_clear(&revs);
			తిరిగి BISECT_FAILED;
		}
		ఒకవేళ (verify_expected && !oideq(&revs.oid[i], &expected)) {
			unlink_or_warn(git_మార్గం_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					ఖాళీ, "BISECT_EXPECTED_REV", ఖాళీ,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	తిరిగి bisect_auto_తదుపరి(terms, ఖాళీ);
}

స్థిర గణన bisect_error bisect_log(శూన్య)
{
	పూర్ణాంకం fd, స్థితి_2;
	స్థిరరాశి అక్షరం* filename = git_మార్గం_bisect_log();

	ఒకవేళ (is_ఖాళీలు_or_missing_file(filename))
		తిరిగి error(_("We are not bisecting."));

	fd = open(filename, O_RDONLY);
	ఒకవేళ (fd < 0)
		తిరిగి BISECT_FAILED;

	స్థితి_2 = copy_fd(fd, STDOUT_FILENO);
	close(fd);
	తిరిగి స్థితి_2 ? BISECT_FAILED : BISECT_OK;
}

స్థిర పూర్ణాంకం process_replay_పంక్తి(నిర్మాణం bisect_terms *terms, నిర్మాణం strbuf *పంక్తి)
{
	స్థిరరాశి అక్షరం *p = పంక్తి->buf + strspn(పంక్తి->buf, " \t");
	అక్షరం *మాట_చివర, *rev;

	ఒకవేళ ((!skip_ఉపసర్గ(p, "git bisect", &p) &&
	!skip_ఉపసర్గ(p, "git-bisect", &p)) || !ఖాళీనా(*p))
		తిరిగి 0;
	p += strspn(p, " \t");

	మాట_చివర = (అక్షరం *)p + strcspn(p, " \t");
	rev = మాట_చివర + strspn(మాట_చివర, " \t");
	*మాట_చివర = '\0'; /* NUL-terminate the word */

	ఒకవేళ (get_terms(terms, 1))
		తిరిగి error(_("no terms defined"));
	ఒకవేళ (check_and_సమితి_terms(terms, p))
		తిరిగి -1;

	ఒకవేళ (!strcmp(p, "start")) {
		నిర్మాణం strvec argv = STRVEC_INIT;
		పూర్ణాంకం res;
		sq_dequote_గమ్యం_strvec(rev, &argv);
		res = bisect_ప్రారంభం(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		తిరిగి res;
	}

	ఒకవేళ (one_యొక్క(p, terms->పదం_good,
	   terms->పదం_bad, "skip", ఖాళీ))
		తిరిగి bisect_write(p, rev, terms, 0);

	ఒకవేళ (!strcmp(p, "terms")) {
		నిర్మాణం strvec argv = STRVEC_INIT;
		పూర్ణాంకం res;
		sq_dequote_గమ్యం_strvec(rev, &argv);
		res = bisect_terms(terms, argv.nr == 1 ? argv.v[0] : ఖాళీ);
		strvec_clear(&argv);
		తిరిగి res;
	}
	error(_("'%s'?? what are you talking about?"), p);

	తిరిగి -1;
}

స్థిర గణన bisect_error bisect_replay(నిర్మాణం bisect_terms *terms, స్థిరరాశి అక్షరం *filename)
{
	FILE *fp = ఖాళీ;
	గణన bisect_error res = BISECT_OK;
	నిర్మాణం strbuf పంక్తి = STRBUF_INIT;

	ఒకవేళ (is_ఖాళీలు_or_missing_file(filename))
		తిరిగి error(_("cannot read file '%s' for replaying"), filename);

	ఒకవేళ (bisect_clean_state())
		తిరిగి BISECT_FAILED;

	fp = fopen(filename, "r");
	ఒకవేళ (!fp)
		తిరిగి BISECT_FAILED;

	అయితే ((strbuf_getline(&పంక్తి, fp) != EOF) && !res)
		res = process_replay_పంక్తి(terms, &పంక్తి);

	strbuf_release(&పంక్తి);
	fclose(fp);

	ఒకవేళ (res)
		తిరిగి BISECT_FAILED;

	తిరిగి bisect_auto_తదుపరి(terms, ఖాళీ);
}

స్థిర గణన bisect_error bisect_skip(నిర్మాణం bisect_terms *terms, పూర్ణాంకం argc,
				     స్థిరరాశి అక్షరం **argv)
{
	పూర్ణాంకం i;
	గణన bisect_error res;
	నిర్మాణం strvec argv_state = STRVEC_INIT;

	strvec_నెట్టు(&argv_state, "skip");

	కొరకు (i = 0; i < argc; i++) {
		స్థిరరాశి అక్షరం *dotdot = అక్షరశ్రేణి_శోధన(argv[i], "..");

		ఒకవేళ (dotdot) {
			నిర్మాణం rev_info revs;
			నిర్మాణం commit *commit;

			repo_init_revisions(the_repository, &revs, ఖాళీ);
			అమరిక_revisions(2, argv + i - 1, &revs, ఖాళీ);

			ఒకవేళ (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			అయితే ((commit = get_revision(&revs)) != ఖాళీ)
				strvec_నెట్టు(&argv_state,
						oid_గమ్యం_hex(&commit->object.oid));

			reset_revision_walk();
			release_revisions(&revs);
		} లేకపోతే {
			strvec_నెట్టు(&argv_state, argv[i]);
		}
	}
	res = bisect_state(terms, argv_state.nr, argv_state.v);

	strvec_clear(&argv_state);
	తిరిగి res;
}

స్థిర పూర్ణాంకం bisect_visualize(నిర్మాణం bisect_terms *terms, పూర్ణాంకం argc,
			    స్థిరరాశి అక్షరం **argv)
{
	నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;
	నిర్మాణం strbuf sb = STRBUF_INIT;

	ఒకవేళ (bisect_తదుపరి_check(terms, ఖాళీ) != 0)
		తిరిగి BISECT_FAILED;

	cmd.no_stdin = 1;
	ఒకవేళ (!argc) {
		ఒకవేళ ((getenv("DISPLAY") || getenv("SESSIONNAME") || getenv("MSYSTEM") ||
		     getenv("SECURITYSESSIONID")) && exists_లోపల_మార్గం("gitk")) {
			strvec_నెట్టు(&cmd.వాదనలు, "gitk");
		} లేకపోతే {
			strvec_నెట్టు(&cmd.వాదనలు, "log");
			cmd.git_cmd = 1;
		}
	} లేకపోతే {
		ఒకవేళ (argv[0][0] == '-') {
			strvec_నెట్టు(&cmd.వాదనలు, "log");
			cmd.git_cmd = 1;
		} లేకపోతే ఒకవేళ (strcmp(argv[0], "tig") && !starts_with(argv[0], "git"))
			cmd.git_cmd = 1;

		strvec_pushv(&cmd.వాదనలు, argv);
	}

	strvec_pushl(&cmd.వాదనలు, "--bisect", "--", ఖాళీ);

	strbuf_read_file(&sb, git_మార్గం_bisect_పేర్లు(), 0);
	sq_dequote_గమ్యం_strvec(sb.buf, &cmd.వాదనలు);
	strbuf_release(&sb);

	తిరిగి నిరంతరం_command(&cmd);
}

స్థిర పూర్ణాంకం get_మొదటి_good(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cb_దత్తాంశం)
{
	oidcpy(cb_దత్తాంశం, ref->oid);
	తిరిగి 1;
}

స్థిర పూర్ణాంకం do_bisect_నిరంతరం(స్థిరరాశి అక్షరం *command)
{
	నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;

	రాయి(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_నెట్టు(&cmd.వాదనలు, command);
	తిరిగి నిరంతరం_command(&cmd);
}

స్థిర పూర్ణాంకం verify_good(స్థిరరాశి నిర్మాణం bisect_terms *terms, స్థిరరాశి అక్షరం *command)
{
	పూర్ణాంకం rc;
	గణన bisect_error res;
	నిర్మాణం object_id good_rev;
	నిర్మాణం object_id current_rev;
	అక్షరం *good_glob = xstrfmt("%s-*", terms->పదం_good);
	పూర్ణాంకం no_checkout = refs_ref_exists(get_main_ref_store(the_repository),
					  "BISECT_HEAD");
	నిర్మాణం refs_for_each_ref_options opts = {
		.నమూనా = good_glob,
		.ఉపసర్గ = "refs/bisect/",
		.trim_ఉపసర్గ = అక్షరశ్రేణి_పొడవు("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_మొదటి_good, &good_rev, &opts);
	విడుదల(good_glob);

	ఒకవేళ (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		తిరిగి -1;

	res = bisect_checkout(&good_rev, no_checkout);
	ఒకవేళ (res != BISECT_OK)
		తిరిగి -1;

	rc = do_bisect_నిరంతరం(command);

	res = bisect_checkout(&current_rev, no_checkout);
	ఒకవేళ (res != BISECT_OK)
		తిరిగి -1;

	తిరిగి rc;
}

స్థిర పూర్ణాంకం bisect_నిరంతరం(నిర్మాణం bisect_terms *terms, పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv)
{
	పూర్ణాంకం res = BISECT_OK;
	నిర్మాణం strbuf command = STRBUF_INIT;
	స్థిరరాశి అక్షరం *reset_when_దొరికింది_arg;
	స్థిరరాశి అక్షరం *new_state;
	పూర్ణాంకం temporary_stdout_fd, saved_stdout;
	పూర్ణాంకం is_మొదటి_నిరంతరం = 1;
	గణన reset_when_దొరికింది_బహుళకం reset_when_దొరికింది = RESET_WHEN_దొరికింది_NONE;

	ఒకవేళ (bisect_తదుపరి_check(terms, ఖాళీ))
		తిరిగి BISECT_FAILED;

	ఒకవేళ (argc && !strcmp(argv[0], "--reset-when-found")) {
		reset_when_దొరికింది = RESET_WHEN_దొరికింది_గమ్యం_ORIGINAL;
	} లేకపోతే ఒకవేళ (argc && skip_ఉపసర్గ(argv[0], "--reset-when-found=",
				    &reset_when_దొరికింది_arg)) {
		ఒకవేళ (parse_reset_when_దొరికింది(reset_when_దొరికింది_arg,
					   &reset_when_దొరికింది))
			తిరిగి BISECT_FAILED;
	}

	ఒకవేళ (reset_when_దొరికింది != RESET_WHEN_దొరికింది_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		తిరిగి error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	ఒకవేళ (reset_when_దొరికింది != RESET_WHEN_దొరికింది_NONE) {
		write_file(git_మార్గం_bisect_reset_when_దొరికింది(), "%s\n",
			   reset_when_దొరికింది_బహుళకం_name(reset_when_దొరికింది));
		argc--;
		argv++;
	}

	ఒకవేళ (!argc) {
		error(_("bisect run failed: no command provided."));
		తిరిగి BISECT_FAILED;
	}

	sq_quote_argv(&command, argv);
	strbuf_ltrim(&command);
	అయితే (1) {
		res = do_bisect_నిరంతరం(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		ఒకవేళ (is_మొదటి_నిరంతరం && (res == 126 || res == 127)) {
			పూర్ణాంకం rc = verify_good(terms, command.buf);
			is_మొదటి_నిరంతరం = 0;
			ఒకవేళ (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->పదం_good);
				res = BISECT_FAILED;
				ఆపు;
			}
			ఒకవేళ (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->పదం_good);
				res = BISECT_FAILED;
				ఆపు;
			}
		}

		ఒకవేళ (res < 0 || 128 <= res) {
			error(_("bisect run failed: exit code %d from"
				" %s is < 0 or >= 128"), res, command.buf);
			ఆపు;
		}

		ఒకవేళ (res == 125)
			new_state = "skip";
		లేకపోతే ఒకవేళ (!res)
			new_state = terms->పదం_good;
		లేకపోతే
			new_state = terms->పదం_bad;

		temporary_stdout_fd = open(git_మార్గం_bisect_నిరంతరం(), O_CREAT | O_WRONLY | O_TRUNC, 0666);

		ఒకవేళ (temporary_stdout_fd < 0) {
			res = error_errno(_("cannot open file '%s' for writing"), git_మార్గం_bisect_నిరంతరం());
			ఆపు;
		}

		fflush(stdout);
		saved_stdout = dup(1);
		ఒకవేళ (saved_stdout < 0 ||
		    dup2(temporary_stdout_fd, 1) < 0) {
			res = error_errno(_("could not duplicate stdout"));
			ఒకవేళ (saved_stdout >= 0)
				close(saved_stdout);
			close(temporary_stdout_fd);
			ఆపు;
		}

		res = bisect_state(terms, 1, &new_state);

		fflush(stdout);
		dup2(saved_stdout, 1);
		close(saved_stdout);
		close(temporary_stdout_fd);

		print_file_గమ్యం_stdout(git_మార్గం_bisect_నిరంతరం());

		ఒకవేళ (res == BISECT_ONLY_SKIPPED_ఎడమ)
			error(_("bisect run cannot continue any more"));
		లేకపోతే ఒకవేళ (res == BISECT_అంతర్గత_SUCCESS_విలీనం_పునాది) {
			puts(_("bisect run success"));
			res = BISECT_OK;
		} లేకపోతే ఒకవేళ (res == BISECT_అంతర్గత_SUCCESS_1_ST_BAD_దొరికింది) {
			రాయి(_("bisect found first '%s' commit\n"), terms->పదం_bad);
		} లేకపోతే ఒకవేళ (res) {
			error(_("bisect run failed: 'git bisect %s'"
				" exited with error code %d"), new_state, res);
		} లేకపోతే {
			కొనసాగు;
		}
		ఆపు;
	}

	strbuf_release(&command);
	తిరిగి res;
}

స్థిర పూర్ణాంకం cmd_bisect__reset(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ వాడని,
			     నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం res;

	ఒకవేళ (argc > 1)
		తిరిగి error(_("'%s' requires either no argument or a commit"),
			     "git bisect reset");
	res = bisect_reset(argc ? argv[0] : ఖాళీ, false);
	ఒకవేళ (res)
		తిరిగి res;
	తిరిగి bisect_clean_state();
}

స్థిర పూర్ణాంకం cmd_bisect__terms(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ వాడని,
			     నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం res;
	నిర్మాణం bisect_terms terms = { 0 };

	ఒకవేళ (argc > 1)
		తిరిగి error(_("'%s' requires 0 or 1 argument"),
			     "git bisect terms");
	res = bisect_terms(&terms, argc == 1 ? argv[0] : ఖాళీ);
	free_terms(&terms);
	తిరిగి res;
}

స్థిర పూర్ణాంకం cmd_bisect_ప్రారంభం(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ వాడని,
			     నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం res;
	నిర్మాణం bisect_terms terms = { 0 };

	సమితి_terms(&terms, "bad", "good");
	res = bisect_ప్రారంభం(&terms, argc, argv);
	free_terms(&terms);
	తిరిగి res;
}

స్థిర పూర్ణాంకం cmd_bisect_తదుపరి(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv వాడని, స్థిరరాశి అక్షరం *ఉపసర్గ,
			    నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం res;
	నిర్మాణం bisect_terms terms = { 0 };

	ఒకవేళ (argc)
		తిరిగి error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	ఒకవేళ (get_terms(&terms, 1))
		తిరిగి error(_("no terms defined"));
	res = bisect_తదుపరి(&terms, ఉపసర్గ);
	free_terms(&terms);
	తిరిగి res;
}

స్థిర పూర్ణాంకం cmd_bisect__log(పూర్ణాంకం argc వాడని, స్థిరరాశి అక్షరం **argv వాడని,
			   స్థిరరాశి అక్షరం *ఉపసర్గ వాడని,
			   నిర్మాణం repository *repo వాడని)
{
	తిరిగి bisect_log();
}

స్థిర పూర్ణాంకం cmd_bisect__replay(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ వాడని,
			      నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం res;
	నిర్మాణం bisect_terms terms = { 0 };

	ఒకవేళ (argc != 1)
		తిరిగి error(_("no logfile given"));
	సమితి_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	తిరిగి res;
}

స్థిర పూర్ణాంకం cmd_bisect__skip(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ వాడని,
			    నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం res;
	నిర్మాణం bisect_terms terms = { 0 };

	సమితి_terms(&terms, "bad", "good");
	ఒకవేళ (get_terms(&terms, 1))
		తిరిగి error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	తిరిగి res;
}

స్థిర పూర్ణాంకం cmd_bisect__visualize(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ వాడని,
				 నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం res;
	నిర్మాణం bisect_terms terms = { 0 };

	ఒకవేళ (get_terms(&terms, 1))
		తిరిగి error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	తిరిగి res;
}

స్థిర పూర్ణాంకం cmd_bisect_నిరంతరం(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ వాడని,
			   నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం res;
	నిర్మాణం bisect_terms terms = { 0 };

	ఒకవేళ (!argc)
		తిరిగి error(_("'%s' failed: no command provided."), "git bisect run");
	ఒకవేళ (get_terms(&terms, 1))
		తిరిగి error(_("no terms defined"));
	res = bisect_నిరంతరం(&terms, argc, argv);
	free_terms(&terms);
	తిరిగి res;
}

పూర్ణాంకం cmd_bisect(పూర్ణాంకం argc,
	       స్థిరరాశి అక్షరం **argv,
	       స్థిరరాశి అక్షరం *ఉపసర్గ,
	       నిర్మాణం repository *repo)
{
	పూర్ణాంకం res = 0;
	parse_opt_subcommand_fn *fn = ఖాళీ;
	నిర్మాణం option options[] = {
		OPT_SUBCOMMAND("reset", &fn, cmd_bisect__reset),
		OPT_SUBCOMMAND("terms", &fn, cmd_bisect__terms),
		OPT_SUBCOMMAND("start", &fn, cmd_bisect_ప్రారంభం),
		OPT_SUBCOMMAND("next", &fn, cmd_bisect_తదుపరి),
		OPT_SUBCOMMAND("log", &fn, cmd_bisect__log),
		OPT_SUBCOMMAND("replay", &fn, cmd_bisect__replay),
		OPT_SUBCOMMAND("skip", &fn, cmd_bisect__skip),
		OPT_SUBCOMMAND("visualize", &fn, cmd_bisect__visualize),
		OPT_SUBCOMMAND("view", &fn, cmd_bisect__visualize),
		OPT_SUBCOMMAND("run", &fn, cmd_bisect_నిరంతరం),
		OPT_చివర()
	};
	argc = parse_options(argc, argv, ఉపసర్గ, options, git_bisect_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);

	ఒకవేళ (!fn) {
		నిర్మాణం bisect_terms terms = { 0 };

		ఒకవేళ (!argc)
			usage_సందేశం_opt(_("need a command"), git_bisect_usage, options);

		ఒకవేళ (!strcmp(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		సమితి_terms(&terms, "bad", "good");
		ఒకవేళ (get_terms(&terms, 1))
			తిరిగి error(_("no terms defined"));
		ఒకవేళ (check_and_సమితి_terms(&terms, argv[0]) ||
		    !one_యొక్క(argv[0], terms.పదం_good, terms.పదం_bad, ఖాళీ))
			usage_సందేశం_optf(_("unknown command: '%s'"), git_bisect_usage,
				       options, argv[0]);
		లేకపోతే
			res = bisect_state(&terms, argc, argv);
		free_terms(&terms);
	} లేకపోతే {
		argc--;
		argv++;
		res = fn(argc, argv, ఉపసర్గ, repo);
	}

	ఒకవేళ (res == BISECT_అంతర్గత_SUCCESS_1_ST_BAD_దొరికింది) {
		గణన reset_when_దొరికింది_బహుళకం బహుళకం;

		ఒకవేళ (read_reset_when_దొరికింది(&బహుళకం))
			res = BISECT_FAILED;
		లేకపోతే ఒకవేళ (బహుళకం != RESET_WHEN_దొరికింది_NONE &&
			 bisect_reset_when_దొరికింది(బహుళకం))
			res = BISECT_FAILED;
	}

	తిరిగి is_bisect_success(res) ? 0 : -res;
}
