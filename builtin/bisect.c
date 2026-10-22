#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "copy.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "bisect.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "strvec.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "oid-array.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "prompt.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "revision.h"

স্থির GIT_PATH_FUNC(git_path_bisect_terms, "BISECT_TERMS")
স্থির GIT_PATH_FUNC(git_path_bisect_ancestors_ok, "BISECT_ANCESTORS_OK")
স্থির GIT_PATH_FUNC(git_path_bisect_start, "BISECT_START")
স্থির GIT_PATH_FUNC(git_path_bisect_log, "BISECT_LOG")
স্থির GIT_PATH_FUNC(git_path_bisect_names, "BISECT_NAMES")
স্থির GIT_PATH_FUNC(git_path_bisect_first_parent, "BISECT_FIRST_PARENT")
স্থির GIT_PATH_FUNC(git_path_bisect_reset_when_found, "BISECT_RESET_WHEN_FOUND")
স্থির GIT_PATH_FUNC(git_path_bisect_run, "BISECT_RUN")

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
#define BUILTIN_GIT_BISECT_পরবর্তী_USAGE \
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

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_bisect_usage[] = {
	BUILTIN_GIT_BISECT_START_USAGE,
	BUILTIN_GIT_BISECT_BAD_USAGE,
	BUILTIN_GIT_BISECT_GOOD_USAGE,
	BUILTIN_GIT_BISECT_TERMS_USAGE,
	BUILTIN_GIT_BISECT_SKIP_USAGE,
	BUILTIN_GIT_BISECT_পরবর্তী_USAGE,
	BUILTIN_GIT_BISECT_RESET_USAGE,
	BUILTIN_GIT_BISECT_VISUALIZE_USAGE,
	BUILTIN_GIT_BISECT_REPLAY_USAGE,
	BUILTIN_GIT_BISECT_LOG_USAGE,
	BUILTIN_GIT_BISECT_RUN_USAGE,
	BUILTIN_GIT_BISECT_HELP_USAGE,
	ফাঁকা
};

গণনা reset_when_found_mode {
	RESET_WHEN_FOUND_NONE,
	RESET_WHEN_FOUND_TO_ORIGINAL,
	RESET_WHEN_FOUND_TO_FOUND,
};

কাঠামো add_bisect_ref_তথ্য {
	কাঠামো rev_info *revs;
	unsigned পূর্ণসংখ্যা object_flags;
};

কাঠামো bisect_terms {
	অক্ষর *term_good;
	অক্ষর *term_bad;
};

স্থির শূন্য free_terms(কাঠামো bisect_terms *terms)
{
	FREE_AND_NULL(terms->term_good);
	FREE_AND_NULL(terms->term_bad);
}

স্থির শূন্য set_terms(কাঠামো bisect_terms *terms, ধ্রুবক অক্ষর *bad,
		      ধ্রুবক অক্ষর *good)
{
	মুক্ত((শূন্য *)terms->term_good);
	terms->term_good = xstrdup(good);
	মুক্ত((শূন্য *)terms->term_bad);
	terms->term_bad = xstrdup(bad);
}

স্থির ধ্রুবক অক্ষর vocab_bad[] = "bad|new";
স্থির ধ্রুবক অক্ষর vocab_good[] = "good|old";

স্থির পূর্ণসংখ্যা bisect_autostart(কাঠামো bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
LAST_ARG_MUST_BE_NULL
স্থির পূর্ণসংখ্যা one_of(ধ্রুবক অক্ষর *term, ...)
{
	পূর্ণসংখ্যা res = 0;
	va_list matches;
	ধ্রুবক অক্ষর *match;

	va_start(matches, term);
	যখন (!res && (match = va_arg(matches, ধ্রুবক অক্ষর *)))
		res = !strcmp(term, match);
	va_end(matches);

	ফেরত res;
}

/*
 * return code BISECT_INTERNAL_SUCCESS_MERGE_BASE
 * and BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND are codes
 * that indicate special success.
 */

স্থির পূর্ণসংখ্যা is_bisect_success(গণনা bisect_error res)
{
	ফেরত !res ||
		res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND ||
		res == BISECT_INTERNAL_SUCCESS_MERGE_BASE;
}

স্থির পূর্ণসংখ্যা write_in_file(ধ্রুবক অক্ষর *path, ধ্রুবক অক্ষর *mode, ধ্রুবক অক্ষর *format, va_list args)
{
	FILE *fp = ফাঁকা;
	পূর্ণসংখ্যা res = 0;

	যদি (strcmp(mode, "w") && strcmp(mode, "a"))
		BUG("write-in-file does not support '%s' mode", mode);
	fp = fopen(path, mode);
	যদি (!fp)
		ফেরত error_errno(_("cannot open file '%s' in mode '%s'"), path, mode);
	res = vfprintf(fp, format, args);

	যদি (res < 0) {
		পূর্ণসংখ্যা saved_errno = errno;
		fclose(fp);
		errno = saved_errno;
		ফেরত error_errno(_("could not write to file '%s'"), path);
	}

	ফেরত fclose(fp);
}

__attribute__((format (printf, 2, 3)))
স্থির পূর্ণসংখ্যা write_to_file(ধ্রুবক অক্ষর *path, ধ্রুবক অক্ষর *format, ...)
{
	পূর্ণসংখ্যা res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "w", format, args);
	va_end(args);

	ফেরত res;
}

__attribute__((format (printf, 2, 3)))
স্থির পূর্ণসংখ্যা append_to_file(ধ্রুবক অক্ষর *path, ধ্রুবক অক্ষর *format, ...)
{
	পূর্ণসংখ্যা res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "a", format, args);
	va_end(args);

	ফেরত res;
}

স্থির পূর্ণসংখ্যা print_file_to_stdout(ধ্রুবক অক্ষর *path)
{
	পূর্ণসংখ্যা fd = open(path, O_RDONLY);
	পূর্ণসংখ্যা ret = 0;

	যদি (fd < 0)
		ফেরত error_errno(_("cannot open file '%s' for reading"), path);
	যদি (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), path);
	close(fd);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা check_term_format(ধ্রুবক অক্ষর *term, ধ্রুবক অক্ষর *orig_term)
{
	পূর্ণসংখ্যা res;
	অক্ষর *new_term = xstrfmt("refs/bisect/%s", term);

	res = check_refname_format(new_term, 0);
	মুক্ত(new_term);

	যদি (res)
		ফেরত error(_("'%s' is not a valid term"), term);

	যদি (one_of(term, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", ফাঁকা))
		ফেরত error(_("can't use the builtin command '%s' as a term"), term);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	যদি ((strcmp(orig_term, "bad") && one_of(term, "bad", "new", ফাঁকা)) ||
		 (strcmp(orig_term, "good") && one_of(term, "good", "old", ফাঁকা)))
		ফেরত error(_("can't change the meaning of the term '%s'"), term);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা write_terms(ধ্রুবক অক্ষর *bad, ধ্রুবক অক্ষর *good)
{
	পূর্ণসংখ্যা res;

	যদি (!strcmp(bad, good))
		ফেরত error(_("please use two different terms"));

	যদি (check_term_format(bad, "bad") || check_term_format(good, "good"))
		ফেরত -1;

	res = write_to_file(git_path_bisect_terms(), "%s\n%s\n", bad, good);

	ফেরত res;
}

স্থির পূর্ণসংখ্যা bisect_reset(ধ্রুবক অক্ষর *commit, bool quiet)
{
	কাঠামো strbuf branch = STRBUF_INIT;

	যদি (!commit) {
		যদি (!strbuf_read_file(&branch, git_path_bisect_start(), 0))
			printf(_("We are not bisecting.\n"));
		অন্যথা
			strbuf_rtrim(&branch);
	} অন্যথা {
		কাঠামো object_id oid;

		যদি (repo_get_oid_commit(the_repository, commit, &oid))
			ফেরত error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	যদি (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		কাঠামো child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "checkout", "--ignore-other-worktrees", ফাঁকা);
		যদি (quiet)
			strvec_ঠেলো(&cmd.args, "--quiet");
		strvec_pushl(&cmd.args, branch.buf, "--", ফাঁকা);
		যদি (run_command(&cmd)) {
			error(_("could not check out original"
				" HEAD '%s'. Try 'git bisect"
				" reset <commit>'."), branch.buf);
			strbuf_release(&branch);
			ফেরত -1;
		}
	}

	strbuf_release(&branch);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা parse_reset_when_found(ধ্রুবক অক্ষর *মান,
				  গণনা reset_when_found_mode *mode)
{
	যদি (!strcmp(মান, "original"))
		*mode = RESET_WHEN_FOUND_TO_ORIGINAL;
	অন্যথা যদি (!strcmp(মান, "found"))
		*mode = RESET_WHEN_FOUND_TO_FOUND;
	অন্যথা
		ফেরত error(_("invalid value for '--reset-when-found': '%s'"),
			     মান);

	ফেরত 0;
}

স্থির ধ্রুবক অক্ষর *reset_when_found_mode_name(গণনা reset_when_found_mode mode)
{
	নির্বাচন (mode) {
	ক্ষেত্র RESET_WHEN_FOUND_TO_ORIGINAL:
		ফেরত "original";
	ক্ষেত্র RESET_WHEN_FOUND_TO_FOUND:
		ফেরত "found";
	ক্ষেত্র RESET_WHEN_FOUND_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", mode);
}

স্থির পূর্ণসংখ্যা read_reset_when_found(গণনা reset_when_found_mode *mode)
{
	কাঠামো strbuf মান = STRBUF_INIT;
	পূর্ণসংখ্যা res = 0;

	*mode = RESET_WHEN_FOUND_NONE;
	যদি (is_empty_or_missing_file(git_path_bisect_reset_when_found()))
		ফেরত 0;

	যদি (strbuf_read_file(&মান, git_path_bisect_reset_when_found(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_path_bisect_reset_when_found());
		যাও out;
	}
	strbuf_trim(&মান);
	যদি (parse_reset_when_found(মান.buf, mode))
		res = -1;

out:
	strbuf_release(&মান);
	ফেরত res;
}

স্থির পূর্ণসংখ্যা bisect_reset_when_found(গণনা reset_when_found_mode mode)
{
	কাঠামো bisect_terms terms = { 0 };
	অক্ষর *commit = ফাঁকা;
	পূর্ণসংখ্যা res;

	যদি (mode == RESET_WHEN_FOUND_TO_FOUND) {
		read_bisect_terms(&terms.term_bad, &terms.term_good);
		commit = xstrfmt("refs/bisect/%s", terms.term_bad);
	} অন্যথা যদি (mode == RESET_WHEN_FOUND_NONE) {
		BUG("automatic reset requested without a reset mode");
	}

	res = bisect_reset(commit, true);
	যদি (!res)
		res = bisect_clean_state();

	মুক্ত(commit);
	free_terms(&terms);
	ফেরত res;
}

স্থির শূন্য log_commit(FILE *fp,
		       ধ্রুবক অক্ষর *fmt, ধ্রুবক অক্ষর *state,
		       কাঠামো commit *commit)
{
	কাঠামো pretty_print_context pp = {0};
	কাঠামো strbuf commit_বার্তা = STRBUF_INIT;
	অক্ষর *label = xstrfmt(fmt, state);

	repo_format_commit_message(the_repository, commit, "%s", &commit_বার্তা,
				   &pp);

	fprintf(fp, "# %s: [%s] %s\n", label, oid_to_hex(&commit->object.oid),
		commit_বার্তা.buf);

	strbuf_release(&commit_বার্তা);
	মুক্ত(label);
}

স্থির পূর্ণসংখ্যা bisect_write(ধ্রুবক অক্ষর *state, ধ্রুবক অক্ষর *rev,
			ধ্রুবক কাঠামো bisect_terms *terms, পূর্ণসংখ্যা nolog)
{
	কাঠামো strbuf tag = STRBUF_INIT;
	কাঠামো object_id oid;
	কাঠামো commit *commit;
	FILE *fp = ফাঁকা;
	পূর্ণসংখ্যা res = 0;

	যদি (!strcmp(state, terms->term_bad)) {
		strbuf_addf(&tag, "refs/bisect/%s", state);
	} অন্যথা যদি (one_of(state, terms->term_good, "skip", ফাঁকা)) {
		strbuf_addf(&tag, "refs/bisect/%s-%s", state, rev);
	} অন্যথা {
		res = error(_("Bad bisect_write argument: %s"), state);
		যাও finish;
	}

	যদি (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		যাও finish;
	}

	যদি (refs_update_ref(get_main_ref_store(the_repository), ফাঁকা, tag.buf, &oid, ফাঁকা, 0,
			    UPDATE_REFS_বার্তা_ON_ERR)) {
		res = -1;
		যাও finish;
	}

	fp = fopen(git_path_bisect_log(), "a");
	যদি (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_path_bisect_log());
		যাও finish;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	যদি (!nolog)
		fprintf(fp, "git bisect %s %s\n", state, rev);

finish:
	যদি (fp)
		fclose(fp);
	strbuf_release(&tag);
	ফেরত res;
}

স্থির পূর্ণসংখ্যা check_and_set_terms(কাঠামো bisect_terms *terms, ধ্রুবক অক্ষর *cmd)
{
	পূর্ণসংখ্যা has_term_file = !is_empty_or_missing_file(git_path_bisect_terms());

	যদি (one_of(cmd, "skip", "start", "terms", ফাঁকা))
		ফেরত 0;

	যদি (has_term_file && strcmp(cmd, terms->term_bad) &&
	    strcmp(cmd, terms->term_good))
		ফেরত error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->term_bad,
				terms->term_good);

	যদি (!has_term_file) {
		যদি (one_of(cmd, "bad", "good", ফাঁকা)) {
			set_terms(terms, "bad", "good");
			ফেরত write_terms(terms->term_bad, terms->term_good);
		}
		যদি (one_of(cmd, "new", "old", ফাঁকা)) {
			set_terms(terms, "new", "old");
			ফেরত write_terms(terms->term_bad, terms->term_good);
		}
	}

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা inc_nr(ধ্রুবক কাঠামো reference *ref UNUSED, শূন্য *cb_তথ্য)
{
	unsigned পূর্ণসংখ্যা *nr = (unsigned পূর্ণসংখ্যা *)cb_তথ্য;
	(*nr)++;
	ফেরত 0;
}

স্থির ধ্রুবক অক্ষর need_bad_and_good_revision_warning[] =
	N_("You need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

স্থির ধ্রুবক অক্ষর need_bisect_start_warning[] =
	N_("You need to start by \"git bisect start\".\n"
	   "You then need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

স্থির পূর্ণসংখ্যা decide_পরবর্তী(ধ্রুবক কাঠামো bisect_terms *terms,
		       ধ্রুবক অক্ষর *current_term, পূর্ণসংখ্যা missing_good,
		       পূর্ণসংখ্যা missing_bad)
{
	যদি (!missing_good && !missing_bad)
		ফেরত 0;
	যদি (!current_term)
		ফেরত -1;

	যদি (missing_good && !missing_bad &&
	    !strcmp(current_term, terms->term_good)) {
		অক্ষর *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->term_bad);
		যদি (!isatty(0))
			ফেরত 0;
		/*
		 * TRANSLATORS: Make sure to include [Y] and [n] in your
		 * translation. The program will only accept English input
		 * at this point.
		 */
		yesno = git_prompt(_("Are you sure [Y/n]? "), PROMPT_ECHO);
		যদি (starts_with(yesno, "N") || starts_with(yesno, "n"))
			ফেরত -1;
		ফেরত 0;
	}

	যদি (!is_empty_or_missing_file(git_path_bisect_start()))
		ফেরত error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	অন্যথা
		ফেরত error(_(need_bisect_start_warning),
			     vocab_good, vocab_bad, vocab_good, vocab_bad);
}

স্থির শূন্য bisect_status(কাঠামো bisect_state *state,
			  ধ্রুবক কাঠামো bisect_terms *terms)
{
	অক্ষর *bad_ref = xstrfmt("refs/bisect/%s", terms->term_bad);
	অক্ষর *good_glob = xstrfmt("%s-*", terms->term_good);
	কাঠামো refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	যদি (refs_ref_exists(get_main_ref_store(the_repository), bad_ref))
		state->nr_bad = 1;

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      inc_nr, &state->nr_good, &opts);

	মুক্ত(good_glob);
	মুক্ত(bad_ref);
}

__attribute__((format (printf, 1, 2)))
স্থির শূন্য bisect_log_printf(ধ্রুবক অক্ষর *fmt, ...)
{
	কাঠামো strbuf buf = STRBUF_INIT;
	va_list ap;

	va_start(ap, fmt);
	strbuf_vaddf(&buf, fmt, ap);
	va_end(ap);

	printf("%s", buf.buf);
	append_to_file(git_path_bisect_log(), "# %s", buf.buf);

	strbuf_release(&buf);
}

স্থির শূন্য bisect_print_status(ধ্রুবক কাঠামো bisect_terms *terms)
{
	কাঠামো bisect_state state = { 0 };

	bisect_status(&state, terms);

	/* If we had both, we'd already be started, and shouldn't get here. */
	যদি (state.nr_good && state.nr_bad)
		ফেরত;

	যদি (!state.nr_good && !state.nr_bad)
		bisect_log_printf(_("status: waiting for both '%s' and '%s' commits\n"),
				  terms->term_good, terms->term_bad);
	অন্যথা যদি (state.nr_good)
		bisect_log_printf(Q_("status: waiting for '%s' commit, %d '%s' commit known\n",
				     "status: waiting for '%s' commit, %d '%s' commits known\n",
				     state.nr_good),
				  terms->term_bad, state.nr_good, terms->term_good);
	অন্যথা
		bisect_log_printf(_("status: waiting for '%s' commit(s), '%s' commit known\n"),
				  terms->term_good, terms->term_bad);
}

স্থির পূর্ণসংখ্যা bisect_পরবর্তী_check(ধ্রুবক কাঠামো bisect_terms *terms,
			     ধ্রুবক অক্ষর *current_term)
{
	কাঠামো bisect_state state = { 0 };
	bisect_status(&state, terms);
	ফেরত decide_পরবর্তী(terms, current_term, !state.nr_good, !state.nr_bad);
}

স্থির পূর্ণসংখ্যা get_terms(কাঠামো bisect_terms *terms, পূর্ণসংখ্যা file_missing_is_ok)
{
	কাঠামো strbuf str = STRBUF_INIT;
	FILE *fp = ফাঁকা;
	পূর্ণসংখ্যা res = 0;

	fp = fopen(git_path_bisect_terms(), "r");
	যদি (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		যাও finish;
	}

	free_terms(terms);
	যদি (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		যাও finish;
	}
	terms->term_bad = strbuf_detach(&str, ফাঁকা);
	যদি (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		FREE_AND_NULL(terms->term_bad);
		যাও finish;
	}
	terms->term_good = strbuf_detach(&str, ফাঁকা);

finish:
	যদি (fp)
		fclose(fp);
	strbuf_release(&str);
	ফেরত res;
}

স্থির পূর্ণসংখ্যা bisect_terms(কাঠামো bisect_terms *terms, ধ্রুবক অক্ষর *option)
{
	যদি (get_terms(terms, 0))
		ফেরত error(_("no terms defined"));

	যদি (!option) {
		printf(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->term_good, terms->term_bad);
		ফেরত 0;
	}
	যদি (one_of(option, "--term-good", "--term-old", ফাঁকা))
		printf("%s\n", terms->term_good);
	অন্যথা যদি (one_of(option, "--term-bad", "--term-new", ফাঁকা))
		printf("%s\n", terms->term_bad);
	অন্যথা
		ফেরত error(_("invalid argument %s for 'git bisect terms'.\n"
			       "Supported options are: "
			       "--term-good|--term-old and "
			       "--term-bad|--term-new."), option);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা bisect_append_log_quoted(ধ্রুবক অক্ষর **argv)
{
	পূর্ণসংখ্যা res = 0;
	FILE *fp = fopen(git_path_bisect_log(), "a");
	কাঠামো strbuf orig_args = STRBUF_INIT;

	যদি (!fp)
		ফেরত -1;

	যদি (fprintf(fp, "git bisect start") < 1) {
		res = -1;
		যাও finish;
	}

	sq_quote_argv(&orig_args, argv);
	যদি (fprintf(fp, "%s\n", orig_args.buf) < 1)
		res = -1;

finish:
	fclose(fp);
	strbuf_release(&orig_args);
	ফেরত res;
}

স্থির পূর্ণসংখ্যা add_bisect_ref(ধ্রুবক কাঠামো reference *ref, শূন্য *cb)
{
	কাঠামো add_bisect_ref_তথ্য *তথ্য = cb;

	add_pending_oid(তথ্য->revs, ref->name, ref->oid, তথ্য->object_flags);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা prepare_revs(কাঠামো bisect_terms *terms, কাঠামো rev_info *revs)
{
	কাঠামো refs_for_each_ref_options opts = {
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};
	পূর্ণসংখ্যা res = 0;
	কাঠামো add_bisect_ref_তথ্য cb = { revs };
	অক্ষর *good = xstrfmt("%s-*", terms->term_good);

	/*
	 * We cannot use terms->term_bad directly in
	 * for_each_glob_ref_in() and we have to append a '*' to it,
	 * otherwise for_each_glob_ref_in() will append '/' and '*'.
	 */
	অক্ষর *bad = xstrfmt("%s*", terms->term_bad);

	/*
	 * It is important to reset the flags used by revision walks
	 * as the previous call to bisect_next_all() in turn
	 * sets up a revision walk.
	 */
	reset_revision_walk();
	repo_init_revisions(the_repository, revs, ফাঁকা);
	setup_revisions(0, ফাঁকা, revs, ফাঁকা);

	opts.pattern = bad;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      add_bisect_ref, &cb, &opts);

	cb.object_flags = UNINTERESTING;
	opts.pattern = good;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      add_bisect_ref, &cb, &opts);

	যদি (prepare_revision_walk(revs))
		res = error(_("revision walk setup failed"));

	মুক্ত(good);
	মুক্ত(bad);
	ফেরত res;
}

স্থির পূর্ণসংখ্যা bisect_skipped_commits(কাঠামো bisect_terms *terms)
{
	পূর্ণসংখ্যা res;
	FILE *fp = ফাঁকা;
	কাঠামো rev_info revs;
	কাঠামো commit *commit;
	কাঠামো pretty_print_context pp = {0};
	কাঠামো strbuf commit_name = STRBUF_INIT;

	res = prepare_revs(terms, &revs);
	যদি (res)
		ফেরত res;

	fp = fopen(git_path_bisect_log(), "a");
	যদি (!fp)
		ফেরত error_errno(_("could not open '%s' for appending"),
				  git_path_bisect_log());

	যদি (fprintf(fp, "# only skipped commits left to test\n") < 0)
		ফেরত error_errno(_("failed to write to '%s'"), git_path_bisect_log());

	যখন ((commit = get_revision(&revs)) != ফাঁকা) {
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
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা bisect_successful(কাঠামো bisect_terms *terms)
{
	কাঠামো object_id oid;
	কাঠামো commit *commit;
	কাঠামো pretty_print_context pp = {0};
	কাঠামো strbuf commit_name = STRBUF_INIT;
	অক্ষর *bad_ref = xstrfmt("refs/bisect/%s",terms->term_bad);
	পূর্ণসংখ্যা res;

	refs_read_ref(get_main_ref_store(the_repository), bad_ref, &oid);
	commit = lookup_commit_reference_by_name(bad_ref);
	যদি (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		মুক্ত(bad_ref);
		ফেরত BISECT_FAILED;
	}
	repo_format_commit_message(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_to_file(git_path_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->term_bad, oid_to_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	মুক্ত(bad_ref);
	ফেরত res;
}

স্থির গণনা bisect_error bisect_পরবর্তী(কাঠামো bisect_terms *terms,
				     ধ্রুবক অক্ষর *prefix)
{
	গণনা bisect_error res;

	যদি (bisect_autostart(terms))
		ফেরত BISECT_FAILED;

	যদি (bisect_পরবর্তী_check(terms, terms->term_good))
		ফেরত BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_পরবর্তী_all(the_repository, prefix);

	যদি (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		res = bisect_successful(terms);
		ফেরত res ? res : BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND;
	} অন্যথা যদি (res == BISECT_ONLY_SKIPPED_বাম) {
		res = bisect_skipped_commits(terms);
		ফেরত res ? res : BISECT_ONLY_SKIPPED_বাম;
	}
	ফেরত res;
}

স্থির গণনা bisect_error bisect_auto_পরবর্তী(কাঠামো bisect_terms *terms,
					  ধ্রুবক অক্ষর *prefix)
{
	যদি (bisect_পরবর্তী_check(terms, ফাঁকা)) {
		bisect_print_status(terms);
		ফেরত BISECT_OK;
	}

	ফেরত bisect_পরবর্তী(terms, prefix);
}

স্থির গণনা bisect_error bisect_start(কাঠামো bisect_terms *terms, পূর্ণসংখ্যা argc,
				      ধ্রুবক অক্ষর **argv)
{
	পূর্ণসংখ্যা no_checkout = 0;
	পূর্ণসংখ্যা first_parent_only = 0;
	পূর্ণসংখ্যা i, has_double_dash = 0, must_write_terms = 0, bad_seen = 0;
	পূর্ণসংখ্যা flags, pathspec_pos;
	গণনা bisect_error res = BISECT_OK;
	কাঠামো string_list revs = STRING_LIST_INIT_DUP;
	কাঠামো string_list states = STRING_LIST_INIT_DUP;
	কাঠামো strbuf start_শীর্ষ = STRBUF_INIT;
	কাঠামো strbuf bisect_names = STRBUF_INIT;
	কাঠামো object_id শীর্ষ_oid;
	কাঠামো object_id oid;
	গণনা reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;
	ধ্রুবক অক্ষর *শীর্ষ;

	যদি (is_bare_repository(the_repository))
		no_checkout = 1;

	/*
	 * Check for one bad and then some good revisions
	 */
	জন্য (i = 0; i < argc; i++) {
		যদি (!strcmp(argv[i], "--")) {
			has_double_dash = 1;
			বিরতি;
		}
	}

	জন্য (i = 0; i < argc; i++) {
		ধ্রুবক অক্ষর *arg = argv[i];
		যদি (!strcmp(argv[i], "--")) {
			বিরতি;
		} অন্যথা যদি (!strcmp(arg, "--no-checkout")) {
			no_checkout = 1;
		} অন্যথা যদি (!strcmp(arg, "--first-parent")) {
			first_parent_only = 1;
		} অন্যথা যদি (!strcmp(arg, "--reset-when-found")) {
			reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
		} অন্যথা যদি (skip_prefix(arg, "--reset-when-found=", &arg)) {
			যদি (parse_reset_when_found(arg, &reset_when_found)) {
				res = BISECT_FAILED;
				যাও finish;
			}
		} অন্যথা যদি (!strcmp(arg, "--term-good") ||
			 !strcmp(arg, "--term-old")) {
			i++;
			যদি (argc <= i)
				ফেরত error(_("'' is not a valid term"));
			must_write_terms = 1;
			মুক্ত((শূন্য *) terms->term_good);
			terms->term_good = xstrdup(argv[i]);
		} অন্যথা যদি (skip_prefix(arg, "--term-good=", &arg) ||
			   skip_prefix(arg, "--term-old=", &arg)) {
			must_write_terms = 1;
			মুক্ত((শূন্য *) terms->term_good);
			terms->term_good = xstrdup(arg);
		} অন্যথা যদি (!strcmp(arg, "--term-bad") ||
			 !strcmp(arg, "--term-new")) {
			i++;
			যদি (argc <= i)
				ফেরত error(_("'' is not a valid term"));
			must_write_terms = 1;
			মুক্ত((শূন্য *) terms->term_bad);
			terms->term_bad = xstrdup(argv[i]);
		} অন্যথা যদি (skip_prefix(arg, "--term-bad=", &arg) ||
			   skip_prefix(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			মুক্ত((শূন্য *) terms->term_bad);
			terms->term_bad = xstrdup(arg);
		} অন্যথা যদি (starts_with(arg, "--")) {
			ফেরত error(_("unrecognized option: '%s'"), arg);
		} অন্যথা যদি (!get_oidf(&oid, "%s^{commit}", arg)) {
			string_list_append(&revs, oid_to_hex(&oid));
		} অন্যথা যদি (has_double_dash) {
			die(_("'%s' does not appear to be a valid "
			      "revision"), arg);
		} অন্যথা {
			বিরতি;
		}
	}
	যদি (reset_when_found != RESET_WHEN_FOUND_NONE && no_checkout) {
		res = error(_("options '%s' and '%s' cannot be used together"),
			    "--reset-when-found", "--no-checkout");
		যাও finish;
	}
	pathspec_pos = i;

	/*
	 * The user ran "git bisect start <sha1> <sha1>", hence did not
	 * explicitly specify the terms, but we are already starting to
	 * set references named with the default terms, and won't be able
	 * to change afterwards.
	 */
	যদি (revs.nr)
		must_write_terms = 1;
	জন্য (i = 0; i < revs.nr; i++) {
		যদি (bad_seen) {
			string_list_append(&states, terms->term_good);
		} অন্যথা {
			bad_seen = 1;
			string_list_append(&states, terms->term_bad);
		}
	}

	/*
	 * Verify HEAD
	 */
	শীর্ষ = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
				       "HEAD", 0, &শীর্ষ_oid, &flags);
	যদি (!শীর্ষ) {
		যদি (repo_get_oid(the_repository, "HEAD", &শীর্ষ_oid))
			ফেরত error(_("bad HEAD - I need a HEAD"));
		শীর্ষ = "HEAD";
	}

	/*
	 * Check if we are bisecting
	 */
	যদি (!is_empty_or_missing_file(git_path_bisect_start())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&start_শীর্ষ, git_path_bisect_start(), 0);
		strbuf_trim(&start_শীর্ষ);
		যদি (!no_checkout) {
			কাঠামো child_process cmd = CHILD_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.args, "checkout", start_শীর্ষ.buf,
				     "--", ফাঁকা);
			যদি (run_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       start_শীর্ষ.buf);
				যাও finish;
			}
		}
	} অন্যথা {
		/* Get the rev from where we start. */
		যদি (!repo_get_oid(the_repository, শীর্ষ, &শীর্ষ_oid) &&
		    !starts_with(শীর্ষ, "refs/heads/")) {
			strbuf_reset(&start_শীর্ষ);
			strbuf_add_oid_hex(&start_শীর্ষ, &শীর্ষ_oid);
		} অন্যথা যদি (!repo_get_oid(the_repository, শীর্ষ, &শীর্ষ_oid) &&
			   skip_prefix(শীর্ষ, "refs/heads/", &শীর্ষ)) {
			strbuf_addstr(&start_শীর্ষ, শীর্ষ);
		} অন্যথা {
			ফেরত error(_("bad HEAD - strange symbolic ref"));
		}
	}

	/*
	 * Get rid of any old bisect state.
	 */
	যদি (bisect_clean_state())
		ফেরত BISECT_FAILED;

	/*
	 * Write new start state
	 */
	write_file(git_path_bisect_start(), "%s\n", start_শীর্ষ.buf);

	যদি (first_parent_only)
		write_file(git_path_bisect_first_parent(), "\n");

	যদি (reset_when_found != RESET_WHEN_FOUND_NONE)
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));

	যদি (no_checkout) {
		যদি (repo_get_oid(the_repository, start_শীর্ষ.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), start_শীর্ষ.buf);
			যাও finish;
		}
		যদি (refs_update_ref(get_main_ref_store(the_repository), ফাঁকা, "BISECT_HEAD", &oid, ফাঁকা, 0,
				    UPDATE_REFS_বার্তা_ON_ERR)) {
			res = BISECT_FAILED;
			যাও finish;
		}
	}

	যদি (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_names, argv + pathspec_pos);
	write_file(git_path_bisect_names(), "%s\n", bisect_names.buf);

	জন্য (i = 0; i < states.nr; i++)
		যদি (bisect_write(states.বস্তুসমূহ[i].string,
				 revs.বস্তুসমূহ[i].string, terms, 1)) {
			res = BISECT_FAILED;
			যাও finish;
		}

	যদি (must_write_terms && write_terms(terms->term_bad,
					    terms->term_good)) {
		res = BISECT_FAILED;
		যাও finish;
	}

	res = bisect_append_log_quoted(argv);
	যদি (res)
		res = BISECT_FAILED;

finish:
	string_list_clear(&revs, 0);
	string_list_clear(&states, 0);
	strbuf_release(&start_শীর্ষ);
	strbuf_release(&bisect_names);
	যদি (res)
		ফেরত res;

	res = bisect_auto_পরবর্তী(terms, ফাঁকা);
	যদি (!is_bisect_success(res))
		bisect_clean_state();
	ফেরত res;
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা file_is_not_empty(ধ্রুবক অক্ষর *path)
{
	ফেরত !is_empty_or_missing_file(path);
}

স্থির পূর্ণসংখ্যা bisect_autostart(কাঠামো bisect_terms *terms)
{
	পূর্ণসংখ্যা res;
	ধ্রুবক অক্ষর *yesno;

	যদি (file_is_not_empty(git_path_bisect_start()))
		ফেরত 0;

	fprintf_ln(stderr, _("You need to start by \"git bisect "
			  "start\"\n"));

	যদি (!isatty(STDIN_FILENO))
		ফেরত -1;

	/*
	 * TRANSLATORS: Make sure to include [Y] and [n] in your
	 * translation. The program will only accept English input
	 * at this point.
	 */
	yesno = git_prompt(_("Do you want me to do it for you "
			     "[Y/n]? "), PROMPT_ECHO);
	res = tolower(*yesno) == 'n' ?
		-1 : bisect_start(terms, 0, empty_strvec);

	ফেরত res;
}

স্থির গণনা bisect_error bisect_state(কাঠামো bisect_terms *terms, পূর্ণসংখ্যা argc,
				      ধ্রুবক অক্ষর **argv)
{
	ধ্রুবক অক্ষর *state;
	পূর্ণসংখ্যা i, verify_expected = 1;
	কাঠামো object_id oid, expected;
	কাঠামো oid_array revs = OID_ARRAY_INIT;

	যদি (!argc)
		ফেরত error(_("Please call `--bisect-state` with at least one argument"));

	যদি (bisect_autostart(terms))
		ফেরত BISECT_FAILED;

	state = argv[0];
	যদি (check_and_set_terms(terms, state) ||
	    !one_of(state, terms->term_good, terms->term_bad, "skip", ফাঁকা))
		ফেরত BISECT_FAILED;

	argv++;
	argc--;
	যদি (argc > 1 && !strcmp(state, terms->term_bad))
		ফেরত error(_("'git bisect %s' can take only one argument."), terms->term_bad);

	যদি (argc == 0) {
		ধ্রুবক অক্ষর *শীর্ষ = "BISECT_HEAD";
		গণনা get_oid_ফলাফল res_শীর্ষ = repo_get_oid(the_repository,
							    শীর্ষ, &oid);

		যদি (res_শীর্ষ == MISSING_OBJECT) {
			শীর্ষ = "HEAD";
			res_শীর্ষ = repo_get_oid(the_repository, শীর্ষ, &oid);
		}

		যদি (res_শীর্ষ)
			error(_("Bad rev input: %s"), শীর্ষ);
		oid_array_append(&revs, &oid);
	}

	/*
	 * All input revs must be checked before executing bisect_write()
	 * to discard junk revs.
	 */

	জন্য (; argc; argc--, argv++) {
		কাঠামো commit *commit;

		যদি (repo_get_oid(the_repository, *argv, &oid)){
			error(_("Bad rev input: %s"), *argv);
			oid_array_clear(&revs);
			ফেরত BISECT_FAILED;
		}

		commit = lookup_commit_reference(the_repository, &oid);
		যদি (!commit)
			die(_("Bad rev input (not a commit): %s"), *argv);

		oid_array_append(&revs, &commit->object.oid);
	}

	যদি (refs_read_ref(get_main_ref_store(the_repository), "BISECT_EXPECTED_REV", &expected))
		verify_expected = 0; /* Ignore invalid file contents */

	জন্য (i = 0; i < revs.nr; i++) {
		যদি (bisect_write(state, oid_to_hex(&revs.oid[i]), terms, 0)) {
			oid_array_clear(&revs);
			ফেরত BISECT_FAILED;
		}
		যদি (verify_expected && !oideq(&revs.oid[i], &expected)) {
			unlink_or_warn(git_path_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					ফাঁকা, "BISECT_EXPECTED_REV", ফাঁকা,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	ফেরত bisect_auto_পরবর্তী(terms, ফাঁকা);
}

স্থির গণনা bisect_error bisect_log(শূন্য)
{
	পূর্ণসংখ্যা fd, status;
	ধ্রুবক অক্ষর* filename = git_path_bisect_log();

	যদি (is_empty_or_missing_file(filename))
		ফেরত error(_("We are not bisecting."));

	fd = open(filename, O_RDONLY);
	যদি (fd < 0)
		ফেরত BISECT_FAILED;

	status = copy_fd(fd, STDOUT_FILENO);
	close(fd);
	ফেরত status ? BISECT_FAILED : BISECT_OK;
}

স্থির পূর্ণসংখ্যা process_replay_সারি(কাঠামো bisect_terms *terms, কাঠামো strbuf *সারি)
{
	ধ্রুবক অক্ষর *p = সারি->buf + strspn(সারি->buf, " \t");
	অক্ষর *word_end, *rev;

	যদি ((!skip_prefix(p, "git bisect", &p) &&
	!skip_prefix(p, "git-bisect", &p)) || !isspace(*p))
		ফেরত 0;
	p += strspn(p, " \t");

	word_end = (অক্ষর *)p + strcspn(p, " \t");
	rev = word_end + strspn(word_end, " \t");
	*word_end = '\0'; /* NUL-terminate the word */

	যদি (get_terms(terms, 1))
		ফেরত error(_("no terms defined"));
	যদি (check_and_set_terms(terms, p))
		ফেরত -1;

	যদি (!strcmp(p, "start")) {
		কাঠামো strvec argv = STRVEC_INIT;
		পূর্ণসংখ্যা res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_start(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		ফেরত res;
	}

	যদি (one_of(p, terms->term_good,
	   terms->term_bad, "skip", ফাঁকা))
		ফেরত bisect_write(p, rev, terms, 0);

	যদি (!strcmp(p, "terms")) {
		কাঠামো strvec argv = STRVEC_INIT;
		পূর্ণসংখ্যা res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_terms(terms, argv.nr == 1 ? argv.v[0] : ফাঁকা);
		strvec_clear(&argv);
		ফেরত res;
	}
	error(_("'%s'?? what are you talking about?"), p);

	ফেরত -1;
}

স্থির গণনা bisect_error bisect_replay(কাঠামো bisect_terms *terms, ধ্রুবক অক্ষর *filename)
{
	FILE *fp = ফাঁকা;
	গণনা bisect_error res = BISECT_OK;
	কাঠামো strbuf সারি = STRBUF_INIT;

	যদি (is_empty_or_missing_file(filename))
		ফেরত error(_("cannot read file '%s' for replaying"), filename);

	যদি (bisect_clean_state())
		ফেরত BISECT_FAILED;

	fp = fopen(filename, "r");
	যদি (!fp)
		ফেরত BISECT_FAILED;

	যখন ((strbuf_getline(&সারি, fp) != EOF) && !res)
		res = process_replay_সারি(terms, &সারি);

	strbuf_release(&সারি);
	fclose(fp);

	যদি (res)
		ফেরত BISECT_FAILED;

	ফেরত bisect_auto_পরবর্তী(terms, ফাঁকা);
}

স্থির গণনা bisect_error bisect_skip(কাঠামো bisect_terms *terms, পূর্ণসংখ্যা argc,
				     ধ্রুবক অক্ষর **argv)
{
	পূর্ণসংখ্যা i;
	গণনা bisect_error res;
	কাঠামো strvec argv_state = STRVEC_INIT;

	strvec_ঠেলো(&argv_state, "skip");

	জন্য (i = 0; i < argc; i++) {
		ধ্রুবক অক্ষর *dotdot = strstr(argv[i], "..");

		যদি (dotdot) {
			কাঠামো rev_info revs;
			কাঠামো commit *commit;

			repo_init_revisions(the_repository, &revs, ফাঁকা);
			setup_revisions(2, argv + i - 1, &revs, ফাঁকা);

			যদি (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			যখন ((commit = get_revision(&revs)) != ফাঁকা)
				strvec_ঠেলো(&argv_state,
						oid_to_hex(&commit->object.oid));

			reset_revision_walk();
			release_revisions(&revs);
		} অন্যথা {
			strvec_ঠেলো(&argv_state, argv[i]);
		}
	}
	res = bisect_state(terms, argv_state.nr, argv_state.v);

	strvec_clear(&argv_state);
	ফেরত res;
}

স্থির পূর্ণসংখ্যা bisect_visualize(কাঠামো bisect_terms *terms, পূর্ণসংখ্যা argc,
			    ধ্রুবক অক্ষর **argv)
{
	কাঠামো child_process cmd = CHILD_PROCESS_INIT;
	কাঠামো strbuf sb = STRBUF_INIT;

	যদি (bisect_পরবর্তী_check(terms, ফাঁকা) != 0)
		ফেরত BISECT_FAILED;

	cmd.no_stdin = 1;
	যদি (!argc) {
		যদি ((getenv("DISPLAY") || getenv("SESSIONNAME") || getenv("MSYSTEM") ||
		     getenv("SECURITYSESSIONID")) && exists_in_PATH("gitk")) {
			strvec_ঠেলো(&cmd.args, "gitk");
		} অন্যথা {
			strvec_ঠেলো(&cmd.args, "log");
			cmd.git_cmd = 1;
		}
	} অন্যথা {
		যদি (argv[0][0] == '-') {
			strvec_ঠেলো(&cmd.args, "log");
			cmd.git_cmd = 1;
		} অন্যথা যদি (strcmp(argv[0], "tig") && !starts_with(argv[0], "git"))
			cmd.git_cmd = 1;

		strvec_pushv(&cmd.args, argv);
	}

	strvec_pushl(&cmd.args, "--bisect", "--", ফাঁকা);

	strbuf_read_file(&sb, git_path_bisect_names(), 0);
	sq_dequote_to_strvec(sb.buf, &cmd.args);
	strbuf_release(&sb);

	ফেরত run_command(&cmd);
}

স্থির পূর্ণসংখ্যা get_first_good(ধ্রুবক কাঠামো reference *ref, শূন্য *cb_তথ্য)
{
	oidcpy(cb_তথ্য, ref->oid);
	ফেরত 1;
}

স্থির পূর্ণসংখ্যা do_bisect_run(ধ্রুবক অক্ষর *command)
{
	কাঠামো child_process cmd = CHILD_PROCESS_INIT;

	printf(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_ঠেলো(&cmd.args, command);
	ফেরত run_command(&cmd);
}

স্থির পূর্ণসংখ্যা verify_good(ধ্রুবক কাঠামো bisect_terms *terms, ধ্রুবক অক্ষর *command)
{
	পূর্ণসংখ্যা rc;
	গণনা bisect_error res;
	কাঠামো object_id good_rev;
	কাঠামো object_id current_rev;
	অক্ষর *good_glob = xstrfmt("%s-*", terms->term_good);
	পূর্ণসংখ্যা no_checkout = refs_ref_exists(get_main_ref_store(the_repository),
					  "BISECT_HEAD");
	কাঠামো refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_first_good, &good_rev, &opts);
	মুক্ত(good_glob);

	যদি (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		ফেরত -1;

	res = bisect_checkout(&good_rev, no_checkout);
	যদি (res != BISECT_OK)
		ফেরত -1;

	rc = do_bisect_run(command);

	res = bisect_checkout(&current_rev, no_checkout);
	যদি (res != BISECT_OK)
		ফেরত -1;

	ফেরত rc;
}

স্থির পূর্ণসংখ্যা bisect_run(কাঠামো bisect_terms *terms, পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv)
{
	পূর্ণসংখ্যা res = BISECT_OK;
	কাঠামো strbuf command = STRBUF_INIT;
	ধ্রুবক অক্ষর *reset_when_found_arg;
	ধ্রুবক অক্ষর *new_state;
	পূর্ণসংখ্যা temporary_stdout_fd, saved_stdout;
	পূর্ণসংখ্যা is_first_run = 1;
	গণনা reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;

	যদি (bisect_পরবর্তী_check(terms, ফাঁকা))
		ফেরত BISECT_FAILED;

	যদি (argc && !strcmp(argv[0], "--reset-when-found")) {
		reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
	} অন্যথা যদি (argc && skip_prefix(argv[0], "--reset-when-found=",
				    &reset_when_found_arg)) {
		যদি (parse_reset_when_found(reset_when_found_arg,
					   &reset_when_found))
			ফেরত BISECT_FAILED;
	}

	যদি (reset_when_found != RESET_WHEN_FOUND_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		ফেরত error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	যদি (reset_when_found != RESET_WHEN_FOUND_NONE) {
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));
		argc--;
		argv++;
	}

	যদি (!argc) {
		error(_("bisect run failed: no command provided."));
		ফেরত BISECT_FAILED;
	}

	sq_quote_argv(&command, argv);
	strbuf_ltrim(&command);
	যখন (1) {
		res = do_bisect_run(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		যদি (is_first_run && (res == 126 || res == 127)) {
			পূর্ণসংখ্যা rc = verify_good(terms, command.buf);
			is_first_run = 0;
			যদি (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->term_good);
				res = BISECT_FAILED;
				বিরতি;
			}
			যদি (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->term_good);
				res = BISECT_FAILED;
				বিরতি;
			}
		}

		যদি (res < 0 || 128 <= res) {
			error(_("bisect run failed: exit code %d from"
				" %s is < 0 or >= 128"), res, command.buf);
			বিরতি;
		}

		যদি (res == 125)
			new_state = "skip";
		অন্যথা যদি (!res)
			new_state = terms->term_good;
		অন্যথা
			new_state = terms->term_bad;

		temporary_stdout_fd = open(git_path_bisect_run(), O_CREAT | O_WRONLY | O_TRUNC, 0666);

		যদি (temporary_stdout_fd < 0) {
			res = error_errno(_("cannot open file '%s' for writing"), git_path_bisect_run());
			বিরতি;
		}

		fflush(stdout);
		saved_stdout = dup(1);
		যদি (saved_stdout < 0 ||
		    dup2(temporary_stdout_fd, 1) < 0) {
			res = error_errno(_("could not duplicate stdout"));
			যদি (saved_stdout >= 0)
				close(saved_stdout);
			close(temporary_stdout_fd);
			বিরতি;
		}

		res = bisect_state(terms, 1, &new_state);

		fflush(stdout);
		dup2(saved_stdout, 1);
		close(saved_stdout);
		close(temporary_stdout_fd);

		print_file_to_stdout(git_path_bisect_run());

		যদি (res == BISECT_ONLY_SKIPPED_বাম)
			error(_("bisect run cannot continue any more"));
		অন্যথা যদি (res == BISECT_INTERNAL_SUCCESS_MERGE_BASE) {
			puts(_("bisect run success"));
			res = BISECT_OK;
		} অন্যথা যদি (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
			printf(_("bisect found first '%s' commit\n"), terms->term_bad);
		} অন্যথা যদি (res) {
			error(_("bisect run failed: 'git bisect %s'"
				" exited with error code %d"), new_state, res);
		} অন্যথা {
			চালু;
		}
		বিরতি;
	}

	strbuf_release(&command);
	ফেরত res;
}

স্থির পূর্ণসংখ্যা cmd_bisect__reset(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix UNUSED,
			     কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা res;

	যদি (argc > 1)
		ফেরত error(_("'%s' requires either no argument or a commit"),
			     "git bisect reset");
	res = bisect_reset(argc ? argv[0] : ফাঁকা, false);
	যদি (res)
		ফেরত res;
	ফেরত bisect_clean_state();
}

স্থির পূর্ণসংখ্যা cmd_bisect__terms(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix UNUSED,
			     কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা res;
	কাঠামো bisect_terms terms = { 0 };

	যদি (argc > 1)
		ফেরত error(_("'%s' requires 0 or 1 argument"),
			     "git bisect terms");
	res = bisect_terms(&terms, argc == 1 ? argv[0] : ফাঁকা);
	free_terms(&terms);
	ফেরত res;
}

স্থির পূর্ণসংখ্যা cmd_bisect__start(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix UNUSED,
			     কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা res;
	কাঠামো bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	res = bisect_start(&terms, argc, argv);
	free_terms(&terms);
	ফেরত res;
}

স্থির পূর্ণসংখ্যা cmd_bisect_পরবর্তী(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv UNUSED, ধ্রুবক অক্ষর *prefix,
			    কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা res;
	কাঠামো bisect_terms terms = { 0 };

	যদি (argc)
		ফেরত error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	যদি (get_terms(&terms, 1))
		ফেরত error(_("no terms defined"));
	res = bisect_পরবর্তী(&terms, prefix);
	free_terms(&terms);
	ফেরত res;
}

স্থির পূর্ণসংখ্যা cmd_bisect__log(পূর্ণসংখ্যা argc UNUSED, ধ্রুবক অক্ষর **argv UNUSED,
			   ধ্রুবক অক্ষর *prefix UNUSED,
			   কাঠামো repository *repo UNUSED)
{
	ফেরত bisect_log();
}

স্থির পূর্ণসংখ্যা cmd_bisect__replay(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix UNUSED,
			      কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা res;
	কাঠামো bisect_terms terms = { 0 };

	যদি (argc != 1)
		ফেরত error(_("no logfile given"));
	set_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	ফেরত res;
}

স্থির পূর্ণসংখ্যা cmd_bisect__skip(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix UNUSED,
			    কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা res;
	কাঠামো bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	যদি (get_terms(&terms, 1))
		ফেরত error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	ফেরত res;
}

স্থির পূর্ণসংখ্যা cmd_bisect__visualize(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix UNUSED,
				 কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা res;
	কাঠামো bisect_terms terms = { 0 };

	যদি (get_terms(&terms, 1))
		ফেরত error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	ফেরত res;
}

স্থির পূর্ণসংখ্যা cmd_bisect__run(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix UNUSED,
			   কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা res;
	কাঠামো bisect_terms terms = { 0 };

	যদি (!argc)
		ফেরত error(_("'%s' failed: no command provided."), "git bisect run");
	যদি (get_terms(&terms, 1))
		ফেরত error(_("no terms defined"));
	res = bisect_run(&terms, argc, argv);
	free_terms(&terms);
	ফেরত res;
}

পূর্ণসংখ্যা cmd_bisect(পূর্ণসংখ্যা argc,
	       ধ্রুবক অক্ষর **argv,
	       ধ্রুবক অক্ষর *prefix,
	       কাঠামো repository *repo)
{
	পূর্ণসংখ্যা res = 0;
	parse_opt_subcommand_fn *fn = ফাঁকা;
	কাঠামো option options[] = {
		OPT_SUBCOMMAND("reset", &fn, cmd_bisect__reset),
		OPT_SUBCOMMAND("terms", &fn, cmd_bisect__terms),
		OPT_SUBCOMMAND("start", &fn, cmd_bisect__start),
		OPT_SUBCOMMAND("next", &fn, cmd_bisect_পরবর্তী),
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

	যদি (!fn) {
		কাঠামো bisect_terms terms = { 0 };

		যদি (!argc)
			usage_বার্তা_opt(_("need a command"), git_bisect_usage, options);

		যদি (!strcmp(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		set_terms(&terms, "bad", "good");
		যদি (get_terms(&terms, 1))
			ফেরত error(_("no terms defined"));
		যদি (check_and_set_terms(&terms, argv[0]) ||
		    !one_of(argv[0], terms.term_good, terms.term_bad, ফাঁকা))
			usage_বার্তা_optf(_("unknown command: '%s'"), git_bisect_usage,
				       options, argv[0]);
		অন্যথা
			res = bisect_state(&terms, argc, argv);
		free_terms(&terms);
	} অন্যথা {
		argc--;
		argv++;
		res = fn(argc, argv, prefix, repo);
	}

	যদি (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		গণনা reset_when_found_mode mode;

		যদি (read_reset_when_found(&mode))
			res = BISECT_FAILED;
		অন্যথা যদি (mode != RESET_WHEN_FOUND_NONE &&
			 bisect_reset_when_found(mode))
			res = BISECT_FAILED;
	}

	ফেরত is_bisect_success(res) ? 0 : -res;
}
