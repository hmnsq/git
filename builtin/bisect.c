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

স্থির ধ্রুব বর্ণ * ধ্রুব git_bisect_usage[] = {
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
	রিক্ত
};

গণনা reset_when_found_mode {
	RESET_WHEN_FOUND_NONE,
	RESET_WHEN_FOUND_TO_ORIGINAL,
	RESET_WHEN_FOUND_TO_FOUND,
};

সংরচনা add_bisect_ref_data {
	সংরচনা rev_info *revs;
	unsigned পূর্ণাঙ্ক object_flags;
};

সংরচনা bisect_terms {
	বর্ণ *term_good;
	বর্ণ *term_bad;
};

স্থির শূন্য free_terms(সংরচনা bisect_terms *terms)
{
	FREE_AND_NULL(terms->term_good);
	FREE_AND_NULL(terms->term_bad);
}

স্থির শূন্য set_terms(সংরচনা bisect_terms *terms, ধ্রুব বর্ণ *bad,
		      ধ্রুব বর্ণ *good)
{
	মুক্ত((শূন্য *)terms->term_good);
	terms->term_good = xstrdup(good);
	মুক্ত((শূন্য *)terms->term_bad);
	terms->term_bad = xstrdup(bad);
}

স্থির ধ্রুব বর্ণ vocab_bad[] = "bad|new";
স্থির ধ্রুব বর্ণ vocab_good[] = "good|old";

স্থির পূর্ণাঙ্ক bisect_autostart(সংরচনা bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
LAST_ARG_MUST_BE_NULL
স্থির পূর্ণাঙ্ক one_of(ধ্রুব বর্ণ *term, ...)
{
	পূর্ণাঙ্ক res = 0;
	va_list matches;
	ধ্রুব বর্ণ *match;

	va_start(matches, term);
	যাবত্ (!res && (match = va_arg(matches, ধ্রুব বর্ণ *)))
		res = !strcmp(term, match);
	va_end(matches);

	প্রত্যাগম res;
}

/*
 * return code BISECT_INTERNAL_SUCCESS_MERGE_BASE
 * and BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND are codes
 * that indicate special success.
 */

স্থির পূর্ণাঙ্ক is_bisect_success(গণনা bisect_error res)
{
	প্রত্যাগম !res ||
		res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND ||
		res == BISECT_INTERNAL_SUCCESS_MERGE_BASE;
}

স্থির পূর্ণাঙ্ক write_in_file(ধ্রুব বর্ণ *path, ধ্রুব বর্ণ *mode, ধ্রুব বর্ণ *format, va_list args)
{
	FILE *fp = রিক্ত;
	পূর্ণাঙ্ক res = 0;

	যদি (strcmp(mode, "w") && strcmp(mode, "a"))
		BUG("write-in-file does not support '%s' mode", mode);
	fp = fopen(path, mode);
	যদি (!fp)
		প্রত্যাগম error_errno(_("cannot open file '%s' in mode '%s'"), path, mode);
	res = vfprintf(fp, format, args);

	যদি (res < 0) {
		পূর্ণাঙ্ক saved_errno = errno;
		fclose(fp);
		errno = saved_errno;
		প্রত্যাগম error_errno(_("could not write to file '%s'"), path);
	}

	প্রত্যাগম fclose(fp);
}

__attribute__((format (printf, 2, 3)))
স্থির পূর্ণাঙ্ক write_to_file(ধ্রুব বর্ণ *path, ধ্রুব বর্ণ *format, ...)
{
	পূর্ণাঙ্ক res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "w", format, args);
	va_end(args);

	প্রত্যাগম res;
}

__attribute__((format (printf, 2, 3)))
স্থির পূর্ণাঙ্ক append_to_file(ধ্রুব বর্ণ *path, ধ্রুব বর্ণ *format, ...)
{
	পূর্ণাঙ্ক res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "a", format, args);
	va_end(args);

	প্রত্যাগম res;
}

স্থির পূর্ণাঙ্ক print_file_to_stdout(ধ্রুব বর্ণ *path)
{
	পূর্ণাঙ্ক fd = open(path, O_RDONLY);
	পূর্ণাঙ্ক ret = 0;

	যদি (fd < 0)
		প্রত্যাগম error_errno(_("cannot open file '%s' for reading"), path);
	যদি (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), path);
	close(fd);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক check_term_format(ধ্রুব বর্ণ *term, ধ্রুব বর্ণ *orig_term)
{
	পূর্ণাঙ্ক res;
	বর্ণ *new_term = xstrfmt("refs/bisect/%s", term);

	res = check_refname_format(new_term, 0);
	মুক্ত(new_term);

	যদি (res)
		প্রত্যাগম error(_("'%s' is not a valid term"), term);

	যদি (one_of(term, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", রিক্ত))
		প্রত্যাগম error(_("can't use the builtin command '%s' as a term"), term);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	যদি ((strcmp(orig_term, "bad") && one_of(term, "bad", "new", রিক্ত)) ||
		 (strcmp(orig_term, "good") && one_of(term, "good", "old", রিক্ত)))
		প্রত্যাগম error(_("can't change the meaning of the term '%s'"), term);

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক write_terms(ধ্রুব বর্ণ *bad, ধ্রুব বর্ণ *good)
{
	পূর্ণাঙ্ক res;

	যদি (!strcmp(bad, good))
		প্রত্যাগম error(_("please use two different terms"));

	যদি (check_term_format(bad, "bad") || check_term_format(good, "good"))
		প্রত্যাগম -1;

	res = write_to_file(git_path_bisect_terms(), "%s\n%s\n", bad, good);

	প্রত্যাগম res;
}

স্থির পূর্ণাঙ্ক bisect_reset(ধ্রুব বর্ণ *commit, bool quiet)
{
	সংরচনা strbuf branch = STRBUF_INIT;

	যদি (!commit) {
		যদি (!strbuf_read_file(&branch, git_path_bisect_start(), 0))
			printf(_("We are not bisecting.\n"));
		অন্যথা
			strbuf_rtrim(&branch);
	} অন্যথা {
		সংরচনা object_id oid;

		যদি (repo_get_oid_commit(the_repository, commit, &oid))
			প্রত্যাগম error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	যদি (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		সংরচনা child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "checkout", "--ignore-other-worktrees", রিক্ত);
		যদি (quiet)
			strvec_ঠেলো(&cmd.args, "--quiet");
		strvec_pushl(&cmd.args, branch.buf, "--", রিক্ত);
		যদি (run_command(&cmd)) {
			error(_("could not check out original"
				" HEAD '%s'. Try 'git bisect"
				" reset <commit>'."), branch.buf);
			strbuf_release(&branch);
			প্রত্যাগম -1;
		}
	}

	strbuf_release(&branch);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক parse_reset_when_found(ধ্রুব বর্ণ *মান,
				  গণনা reset_when_found_mode *mode)
{
	যদি (!strcmp(মান, "original"))
		*mode = RESET_WHEN_FOUND_TO_ORIGINAL;
	অন্যথা যদি (!strcmp(মান, "found"))
		*mode = RESET_WHEN_FOUND_TO_FOUND;
	অন্যথা
		প্রত্যাগম error(_("invalid value for '--reset-when-found': '%s'"),
			     মান);

	প্রত্যাগম 0;
}

স্থির ধ্রুব বর্ণ *reset_when_found_mode_name(গণনা reset_when_found_mode mode)
{
	বিকল্প (mode) {
	পক্ষ RESET_WHEN_FOUND_TO_ORIGINAL:
		প্রত্যাগম "original";
	পক্ষ RESET_WHEN_FOUND_TO_FOUND:
		প্রত্যাগম "found";
	পক্ষ RESET_WHEN_FOUND_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", mode);
}

স্থির পূর্ণাঙ্ক read_reset_when_found(গণনা reset_when_found_mode *mode)
{
	সংরচনা strbuf মান = STRBUF_INIT;
	পূর্ণাঙ্ক res = 0;

	*mode = RESET_WHEN_FOUND_NONE;
	যদি (is_empty_or_missing_file(git_path_bisect_reset_when_found()))
		প্রত্যাগম 0;

	যদি (strbuf_read_file(&মান, git_path_bisect_reset_when_found(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_path_bisect_reset_when_found());
		গচ্ছ out;
	}
	strbuf_trim(&মান);
	যদি (parse_reset_when_found(মান.buf, mode))
		res = -1;

out:
	strbuf_release(&মান);
	প্রত্যাগম res;
}

স্থির পূর্ণাঙ্ক bisect_reset_when_found(গণনা reset_when_found_mode mode)
{
	সংরচনা bisect_terms terms = { 0 };
	বর্ণ *commit = রিক্ত;
	পূর্ণাঙ্ক res;

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
	প্রত্যাগম res;
}

স্থির শূন্য log_commit(FILE *fp,
		       ধ্রুব বর্ণ *fmt, ধ্রুব বর্ণ *state,
		       সংরচনা commit *commit)
{
	সংরচনা pretty_print_context pp = {0};
	সংরচনা strbuf commit_বার্তা = STRBUF_INIT;
	বর্ণ *label = xstrfmt(fmt, state);

	repo_format_commit_message(the_repository, commit, "%s", &commit_বার্তা,
				   &pp);

	fprintf(fp, "# %s: [%s] %s\n", label, oid_to_hex(&commit->object.oid),
		commit_বার্তা.buf);

	strbuf_release(&commit_বার্তা);
	মুক্ত(label);
}

স্থির পূর্ণাঙ্ক bisect_write(ধ্রুব বর্ণ *state, ধ্রুব বর্ণ *rev,
			ধ্রুব সংরচনা bisect_terms *terms, পূর্ণাঙ্ক nolog)
{
	সংরচনা strbuf tag = STRBUF_INIT;
	সংরচনা object_id oid;
	সংরচনা commit *commit;
	FILE *fp = রিক্ত;
	পূর্ণাঙ্ক res = 0;

	যদি (!strcmp(state, terms->term_bad)) {
		strbuf_addf(&tag, "refs/bisect/%s", state);
	} অন্যথা যদি (one_of(state, terms->term_good, "skip", রিক্ত)) {
		strbuf_addf(&tag, "refs/bisect/%s-%s", state, rev);
	} অন্যথা {
		res = error(_("Bad bisect_write argument: %s"), state);
		গচ্ছ finish;
	}

	যদি (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		গচ্ছ finish;
	}

	যদি (refs_update_ref(get_main_ref_store(the_repository), রিক্ত, tag.buf, &oid, রিক্ত, 0,
			    UPDATE_REFS_বার্তা_ON_ERR)) {
		res = -1;
		গচ্ছ finish;
	}

	fp = fopen(git_path_bisect_log(), "a");
	যদি (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_path_bisect_log());
		গচ্ছ finish;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	যদি (!nolog)
		fprintf(fp, "git bisect %s %s\n", state, rev);

finish:
	যদি (fp)
		fclose(fp);
	strbuf_release(&tag);
	প্রত্যাগম res;
}

স্থির পূর্ণাঙ্ক check_and_set_terms(সংরচনা bisect_terms *terms, ধ্রুব বর্ণ *cmd)
{
	পূর্ণাঙ্ক has_term_file = !is_empty_or_missing_file(git_path_bisect_terms());

	যদি (one_of(cmd, "skip", "start", "terms", রিক্ত))
		প্রত্যাগম 0;

	যদি (has_term_file && strcmp(cmd, terms->term_bad) &&
	    strcmp(cmd, terms->term_good))
		প্রত্যাগম error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->term_bad,
				terms->term_good);

	যদি (!has_term_file) {
		যদি (one_of(cmd, "bad", "good", রিক্ত)) {
			set_terms(terms, "bad", "good");
			প্রত্যাগম write_terms(terms->term_bad, terms->term_good);
		}
		যদি (one_of(cmd, "new", "old", রিক্ত)) {
			set_terms(terms, "new", "old");
			প্রত্যাগম write_terms(terms->term_bad, terms->term_good);
		}
	}

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক inc_nr(ধ্রুব সংরচনা reference *ref UNUSED, শূন্য *cb_data)
{
	unsigned পূর্ণাঙ্ক *nr = (unsigned পূর্ণাঙ্ক *)cb_data;
	(*nr)++;
	প্রত্যাগম 0;
}

স্থির ধ্রুব বর্ণ need_bad_and_good_revision_warning[] =
	N_("You need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

স্থির ধ্রুব বর্ণ need_bisect_start_warning[] =
	N_("You need to start by \"git bisect start\".\n"
	   "You then need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

স্থির পূর্ণাঙ্ক decide_next(ধ্রুব সংরচনা bisect_terms *terms,
		       ধ্রুব বর্ণ *current_term, পূর্ণাঙ্ক missing_good,
		       পূর্ণাঙ্ক missing_bad)
{
	যদি (!missing_good && !missing_bad)
		প্রত্যাগম 0;
	যদি (!current_term)
		প্রত্যাগম -1;

	যদি (missing_good && !missing_bad &&
	    !strcmp(current_term, terms->term_good)) {
		বর্ণ *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->term_bad);
		যদি (!isatty(0))
			প্রত্যাগম 0;
		/*
		 * TRANSLATORS: Make sure to include [Y] and [n] in your
		 * translation. The program will only accept English input
		 * at this point.
		 */
		yesno = git_prompt(_("Are you sure [Y/n]? "), PROMPT_ECHO);
		যদি (starts_with(yesno, "N") || starts_with(yesno, "n"))
			প্রত্যাগম -1;
		প্রত্যাগম 0;
	}

	যদি (!is_empty_or_missing_file(git_path_bisect_start()))
		প্রত্যাগম error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	অন্যথা
		প্রত্যাগম error(_(need_bisect_start_warning),
			     vocab_good, vocab_bad, vocab_good, vocab_bad);
}

স্থির শূন্য bisect_status(সংরচনা bisect_state *state,
			  ধ্রুব সংরচনা bisect_terms *terms)
{
	বর্ণ *bad_ref = xstrfmt("refs/bisect/%s", terms->term_bad);
	বর্ণ *good_glob = xstrfmt("%s-*", terms->term_good);
	সংরচনা refs_for_each_ref_options opts = {
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
স্থির শূন্য bisect_log_printf(ধ্রুব বর্ণ *fmt, ...)
{
	সংরচনা strbuf buf = STRBUF_INIT;
	va_list ap;

	va_start(ap, fmt);
	strbuf_vaddf(&buf, fmt, ap);
	va_end(ap);

	printf("%s", buf.buf);
	append_to_file(git_path_bisect_log(), "# %s", buf.buf);

	strbuf_release(&buf);
}

স্থির শূন্য bisect_print_status(ধ্রুব সংরচনা bisect_terms *terms)
{
	সংরচনা bisect_state state = { 0 };

	bisect_status(&state, terms);

	/* If we had both, we'd already be started, and shouldn't get here. */
	যদি (state.nr_good && state.nr_bad)
		প্রত্যাগম;

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

স্থির পূর্ণাঙ্ক bisect_next_check(ধ্রুব সংরচনা bisect_terms *terms,
			     ধ্রুব বর্ণ *current_term)
{
	সংরচনা bisect_state state = { 0 };
	bisect_status(&state, terms);
	প্রত্যাগম decide_next(terms, current_term, !state.nr_good, !state.nr_bad);
}

স্থির পূর্ণাঙ্ক get_terms(সংরচনা bisect_terms *terms, পূর্ণাঙ্ক file_missing_is_ok)
{
	সংরচনা strbuf str = STRBUF_INIT;
	FILE *fp = রিক্ত;
	পূর্ণাঙ্ক res = 0;

	fp = fopen(git_path_bisect_terms(), "r");
	যদি (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		গচ্ছ finish;
	}

	free_terms(terms);
	যদি (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		গচ্ছ finish;
	}
	terms->term_bad = strbuf_detach(&str, রিক্ত);
	যদি (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		FREE_AND_NULL(terms->term_bad);
		গচ্ছ finish;
	}
	terms->term_good = strbuf_detach(&str, রিক্ত);

finish:
	যদি (fp)
		fclose(fp);
	strbuf_release(&str);
	প্রত্যাগম res;
}

স্থির পূর্ণাঙ্ক bisect_terms(সংরচনা bisect_terms *terms, ধ্রুব বর্ণ *option)
{
	যদি (get_terms(terms, 0))
		প্রত্যাগম error(_("no terms defined"));

	যদি (!option) {
		printf(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->term_good, terms->term_bad);
		প্রত্যাগম 0;
	}
	যদি (one_of(option, "--term-good", "--term-old", রিক্ত))
		printf("%s\n", terms->term_good);
	অন্যথা যদি (one_of(option, "--term-bad", "--term-new", রিক্ত))
		printf("%s\n", terms->term_bad);
	অন্যথা
		প্রত্যাগম error(_("invalid argument %s for 'git bisect terms'.\n"
			       "Supported options are: "
			       "--term-good|--term-old and "
			       "--term-bad|--term-new."), option);

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক bisect_append_log_quoted(ধ্রুব বর্ণ **argv)
{
	পূর্ণাঙ্ক res = 0;
	FILE *fp = fopen(git_path_bisect_log(), "a");
	সংরচনা strbuf orig_args = STRBUF_INIT;

	যদি (!fp)
		প্রত্যাগম -1;

	যদি (fprintf(fp, "git bisect start") < 1) {
		res = -1;
		গচ্ছ finish;
	}

	sq_quote_argv(&orig_args, argv);
	যদি (fprintf(fp, "%s\n", orig_args.buf) < 1)
		res = -1;

finish:
	fclose(fp);
	strbuf_release(&orig_args);
	প্রত্যাগম res;
}

স্থির পূর্ণাঙ্ক add_bisect_ref(ধ্রুব সংরচনা reference *ref, শূন্য *cb)
{
	সংরচনা add_bisect_ref_data *data = cb;

	add_pending_oid(data->revs, ref->name, ref->oid, data->object_flags);

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক prepare_revs(সংরচনা bisect_terms *terms, সংরচনা rev_info *revs)
{
	সংরচনা refs_for_each_ref_options opts = {
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};
	পূর্ণাঙ্ক res = 0;
	সংরচনা add_bisect_ref_data cb = { revs };
	বর্ণ *good = xstrfmt("%s-*", terms->term_good);

	/*
	 * We cannot use terms->term_bad directly in
	 * for_each_glob_ref_in() and we have to append a '*' to it,
	 * otherwise for_each_glob_ref_in() will append '/' and '*'.
	 */
	বর্ণ *bad = xstrfmt("%s*", terms->term_bad);

	/*
	 * It is important to reset the flags used by revision walks
	 * as the previous call to bisect_next_all() in turn
	 * sets up a revision walk.
	 */
	reset_revision_walk();
	repo_init_revisions(the_repository, revs, রিক্ত);
	setup_revisions(0, রিক্ত, revs, রিক্ত);

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
	প্রত্যাগম res;
}

স্থির পূর্ণাঙ্ক bisect_skipped_commits(সংরচনা bisect_terms *terms)
{
	পূর্ণাঙ্ক res;
	FILE *fp = রিক্ত;
	সংরচনা rev_info revs;
	সংরচনা commit *commit;
	সংরচনা pretty_print_context pp = {0};
	সংরচনা strbuf commit_name = STRBUF_INIT;

	res = prepare_revs(terms, &revs);
	যদি (res)
		প্রত্যাগম res;

	fp = fopen(git_path_bisect_log(), "a");
	যদি (!fp)
		প্রত্যাগম error_errno(_("could not open '%s' for appending"),
				  git_path_bisect_log());

	যদি (fprintf(fp, "# only skipped commits left to test\n") < 0)
		প্রত্যাগম error_errno(_("failed to write to '%s'"), git_path_bisect_log());

	যাবত্ ((commit = get_revision(&revs)) != রিক্ত) {
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
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক bisect_successful(সংরচনা bisect_terms *terms)
{
	সংরচনা object_id oid;
	সংরচনা commit *commit;
	সংরচনা pretty_print_context pp = {0};
	সংরচনা strbuf commit_name = STRBUF_INIT;
	বর্ণ *bad_ref = xstrfmt("refs/bisect/%s",terms->term_bad);
	পূর্ণাঙ্ক res;

	refs_read_ref(get_main_ref_store(the_repository), bad_ref, &oid);
	commit = lookup_commit_reference_by_name(bad_ref);
	যদি (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		মুক্ত(bad_ref);
		প্রত্যাগম BISECT_FAILED;
	}
	repo_format_commit_message(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_to_file(git_path_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->term_bad, oid_to_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	মুক্ত(bad_ref);
	প্রত্যাগম res;
}

স্থির গণনা bisect_error bisect_next(সংরচনা bisect_terms *terms,
				     ধ্রুব বর্ণ *prefix)
{
	গণনা bisect_error res;

	যদি (bisect_autostart(terms))
		প্রত্যাগম BISECT_FAILED;

	যদি (bisect_next_check(terms, terms->term_good))
		প্রত্যাগম BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_next_all(the_repository, prefix);

	যদি (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		res = bisect_successful(terms);
		প্রত্যাগম res ? res : BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND;
	} অন্যথা যদি (res == BISECT_ONLY_SKIPPED_বাম) {
		res = bisect_skipped_commits(terms);
		প্রত্যাগম res ? res : BISECT_ONLY_SKIPPED_বাম;
	}
	প্রত্যাগম res;
}

স্থির গণনা bisect_error bisect_auto_next(সংরচনা bisect_terms *terms,
					  ধ্রুব বর্ণ *prefix)
{
	যদি (bisect_next_check(terms, রিক্ত)) {
		bisect_print_status(terms);
		প্রত্যাগম BISECT_OK;
	}

	প্রত্যাগম bisect_next(terms, prefix);
}

স্থির গণনা bisect_error bisect_start(সংরচনা bisect_terms *terms, পূর্ণাঙ্ক argc,
				      ধ্রুব বর্ণ **argv)
{
	পূর্ণাঙ্ক no_checkout = 0;
	পূর্ণাঙ্ক first_parent_only = 0;
	পূর্ণাঙ্ক i, has_double_dash = 0, must_write_terms = 0, bad_seen = 0;
	পূর্ণাঙ্ক flags, pathspec_pos;
	গণনা bisect_error res = BISECT_OK;
	সংরচনা string_list revs = STRING_LIST_INIT_DUP;
	সংরচনা string_list states = STRING_LIST_INIT_DUP;
	সংরচনা strbuf start_head = STRBUF_INIT;
	সংরচনা strbuf bisect_names = STRBUF_INIT;
	সংরচনা object_id head_oid;
	সংরচনা object_id oid;
	গণনা reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;
	ধ্রুব বর্ণ *head;

	যদি (is_bare_repository(the_repository))
		no_checkout = 1;

	/*
	 * Check for one bad and then some good revisions
	 */
	প্রতি (i = 0; i < argc; i++) {
		যদি (!strcmp(argv[i], "--")) {
			has_double_dash = 1;
			ভঙ্গ;
		}
	}

	প্রতি (i = 0; i < argc; i++) {
		ধ্রুব বর্ণ *arg = argv[i];
		যদি (!strcmp(argv[i], "--")) {
			ভঙ্গ;
		} অন্যথা যদি (!strcmp(arg, "--no-checkout")) {
			no_checkout = 1;
		} অন্যথা যদি (!strcmp(arg, "--first-parent")) {
			first_parent_only = 1;
		} অন্যথা যদি (!strcmp(arg, "--reset-when-found")) {
			reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
		} অন্যথা যদি (skip_prefix(arg, "--reset-when-found=", &arg)) {
			যদি (parse_reset_when_found(arg, &reset_when_found)) {
				res = BISECT_FAILED;
				গচ্ছ finish;
			}
		} অন্যথা যদি (!strcmp(arg, "--term-good") ||
			 !strcmp(arg, "--term-old")) {
			i++;
			যদি (argc <= i)
				প্রত্যাগম error(_("'' is not a valid term"));
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
				প্রত্যাগম error(_("'' is not a valid term"));
			must_write_terms = 1;
			মুক্ত((শূন্য *) terms->term_bad);
			terms->term_bad = xstrdup(argv[i]);
		} অন্যথা যদি (skip_prefix(arg, "--term-bad=", &arg) ||
			   skip_prefix(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			মুক্ত((শূন্য *) terms->term_bad);
			terms->term_bad = xstrdup(arg);
		} অন্যথা যদি (starts_with(arg, "--")) {
			প্রত্যাগম error(_("unrecognized option: '%s'"), arg);
		} অন্যথা যদি (!get_oidf(&oid, "%s^{commit}", arg)) {
			string_list_append(&revs, oid_to_hex(&oid));
		} অন্যথা যদি (has_double_dash) {
			die(_("'%s' does not appear to be a valid "
			      "revision"), arg);
		} অন্যথা {
			ভঙ্গ;
		}
	}
	যদি (reset_when_found != RESET_WHEN_FOUND_NONE && no_checkout) {
		res = error(_("options '%s' and '%s' cannot be used together"),
			    "--reset-when-found", "--no-checkout");
		গচ্ছ finish;
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
	প্রতি (i = 0; i < revs.nr; i++) {
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
	head = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
				       "HEAD", 0, &head_oid, &flags);
	যদি (!head) {
		যদি (repo_get_oid(the_repository, "HEAD", &head_oid))
			প্রত্যাগম error(_("bad HEAD - I need a HEAD"));
		head = "HEAD";
	}

	/*
	 * Check if we are bisecting
	 */
	যদি (!is_empty_or_missing_file(git_path_bisect_start())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&start_head, git_path_bisect_start(), 0);
		strbuf_trim(&start_head);
		যদি (!no_checkout) {
			সংরচনা child_process cmd = CHILD_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.args, "checkout", start_head.buf,
				     "--", রিক্ত);
			যদি (run_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       start_head.buf);
				গচ্ছ finish;
			}
		}
	} অন্যথা {
		/* Get the rev from where we start. */
		যদি (!repo_get_oid(the_repository, head, &head_oid) &&
		    !starts_with(head, "refs/heads/")) {
			strbuf_reset(&start_head);
			strbuf_add_oid_hex(&start_head, &head_oid);
		} অন্যথা যদি (!repo_get_oid(the_repository, head, &head_oid) &&
			   skip_prefix(head, "refs/heads/", &head)) {
			strbuf_addstr(&start_head, head);
		} অন্যথা {
			প্রত্যাগম error(_("bad HEAD - strange symbolic ref"));
		}
	}

	/*
	 * Get rid of any old bisect state.
	 */
	যদি (bisect_clean_state())
		প্রত্যাগম BISECT_FAILED;

	/*
	 * Write new start state
	 */
	write_file(git_path_bisect_start(), "%s\n", start_head.buf);

	যদি (first_parent_only)
		write_file(git_path_bisect_first_parent(), "\n");

	যদি (reset_when_found != RESET_WHEN_FOUND_NONE)
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));

	যদি (no_checkout) {
		যদি (repo_get_oid(the_repository, start_head.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), start_head.buf);
			গচ্ছ finish;
		}
		যদি (refs_update_ref(get_main_ref_store(the_repository), রিক্ত, "BISECT_HEAD", &oid, রিক্ত, 0,
				    UPDATE_REFS_বার্তা_ON_ERR)) {
			res = BISECT_FAILED;
			গচ্ছ finish;
		}
	}

	যদি (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_names, argv + pathspec_pos);
	write_file(git_path_bisect_names(), "%s\n", bisect_names.buf);

	প্রতি (i = 0; i < states.nr; i++)
		যদি (bisect_write(states.বস্তুসমূহ[i].string,
				 revs.বস্তুসমূহ[i].string, terms, 1)) {
			res = BISECT_FAILED;
			গচ্ছ finish;
		}

	যদি (must_write_terms && write_terms(terms->term_bad,
					    terms->term_good)) {
		res = BISECT_FAILED;
		গচ্ছ finish;
	}

	res = bisect_append_log_quoted(argv);
	যদি (res)
		res = BISECT_FAILED;

finish:
	string_list_clear(&revs, 0);
	string_list_clear(&states, 0);
	strbuf_release(&start_head);
	strbuf_release(&bisect_names);
	যদি (res)
		প্রত্যাগম res;

	res = bisect_auto_next(terms, রিক্ত);
	যদি (!is_bisect_success(res))
		bisect_clean_state();
	প্রত্যাগম res;
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক file_is_not_empty(ধ্রুব বর্ণ *path)
{
	প্রত্যাগম !is_empty_or_missing_file(path);
}

স্থির পূর্ণাঙ্ক bisect_autostart(সংরচনা bisect_terms *terms)
{
	পূর্ণাঙ্ক res;
	ধ্রুব বর্ণ *yesno;

	যদি (file_is_not_empty(git_path_bisect_start()))
		প্রত্যাগম 0;

	fprintf_ln(stderr, _("You need to start by \"git bisect "
			  "start\"\n"));

	যদি (!isatty(STDIN_FILENO))
		প্রত্যাগম -1;

	/*
	 * TRANSLATORS: Make sure to include [Y] and [n] in your
	 * translation. The program will only accept English input
	 * at this point.
	 */
	yesno = git_prompt(_("Do you want me to do it for you "
			     "[Y/n]? "), PROMPT_ECHO);
	res = tolower(*yesno) == 'n' ?
		-1 : bisect_start(terms, 0, empty_strvec);

	প্রত্যাগম res;
}

স্থির গণনা bisect_error bisect_state(সংরচনা bisect_terms *terms, পূর্ণাঙ্ক argc,
				      ধ্রুব বর্ণ **argv)
{
	ধ্রুব বর্ণ *state;
	পূর্ণাঙ্ক i, verify_expected = 1;
	সংরচনা object_id oid, expected;
	সংরচনা oid_array revs = OID_ARRAY_INIT;

	যদি (!argc)
		প্রত্যাগম error(_("Please call `--bisect-state` with at least one argument"));

	যদি (bisect_autostart(terms))
		প্রত্যাগম BISECT_FAILED;

	state = argv[0];
	যদি (check_and_set_terms(terms, state) ||
	    !one_of(state, terms->term_good, terms->term_bad, "skip", রিক্ত))
		প্রত্যাগম BISECT_FAILED;

	argv++;
	argc--;
	যদি (argc > 1 && !strcmp(state, terms->term_bad))
		প্রত্যাগম error(_("'git bisect %s' can take only one argument."), terms->term_bad);

	যদি (argc == 0) {
		ধ্রুব বর্ণ *head = "BISECT_HEAD";
		গণনা get_oid_result res_head = repo_get_oid(the_repository,
							    head, &oid);

		যদি (res_head == MISSING_OBJECT) {
			head = "HEAD";
			res_head = repo_get_oid(the_repository, head, &oid);
		}

		যদি (res_head)
			error(_("Bad rev input: %s"), head);
		oid_array_append(&revs, &oid);
	}

	/*
	 * All input revs must be checked before executing bisect_write()
	 * to discard junk revs.
	 */

	প্রতি (; argc; argc--, argv++) {
		সংরচনা commit *commit;

		যদি (repo_get_oid(the_repository, *argv, &oid)){
			error(_("Bad rev input: %s"), *argv);
			oid_array_clear(&revs);
			প্রত্যাগম BISECT_FAILED;
		}

		commit = lookup_commit_reference(the_repository, &oid);
		যদি (!commit)
			die(_("Bad rev input (not a commit): %s"), *argv);

		oid_array_append(&revs, &commit->object.oid);
	}

	যদি (refs_read_ref(get_main_ref_store(the_repository), "BISECT_EXPECTED_REV", &expected))
		verify_expected = 0; /* Ignore invalid file contents */

	প্রতি (i = 0; i < revs.nr; i++) {
		যদি (bisect_write(state, oid_to_hex(&revs.oid[i]), terms, 0)) {
			oid_array_clear(&revs);
			প্রত্যাগম BISECT_FAILED;
		}
		যদি (verify_expected && !oideq(&revs.oid[i], &expected)) {
			unlink_or_warn(git_path_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					রিক্ত, "BISECT_EXPECTED_REV", রিক্ত,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	প্রত্যাগম bisect_auto_next(terms, রিক্ত);
}

স্থির গণনা bisect_error bisect_log(শূন্য)
{
	পূর্ণাঙ্ক fd, status;
	ধ্রুব বর্ণ* filename = git_path_bisect_log();

	যদি (is_empty_or_missing_file(filename))
		প্রত্যাগম error(_("We are not bisecting."));

	fd = open(filename, O_RDONLY);
	যদি (fd < 0)
		প্রত্যাগম BISECT_FAILED;

	status = copy_fd(fd, STDOUT_FILENO);
	close(fd);
	প্রত্যাগম status ? BISECT_FAILED : BISECT_OK;
}

স্থির পূর্ণাঙ্ক process_replay_সারি(সংরচনা bisect_terms *terms, সংরচনা strbuf *সারি)
{
	ধ্রুব বর্ণ *p = সারি->buf + strspn(সারি->buf, " \t");
	বর্ণ *word_end, *rev;

	যদি ((!skip_prefix(p, "git bisect", &p) &&
	!skip_prefix(p, "git-bisect", &p)) || !isspace(*p))
		প্রত্যাগম 0;
	p += strspn(p, " \t");

	word_end = (বর্ণ *)p + strcspn(p, " \t");
	rev = word_end + strspn(word_end, " \t");
	*word_end = '\0'; /* NUL-terminate the word */

	যদি (get_terms(terms, 1))
		প্রত্যাগম error(_("no terms defined"));
	যদি (check_and_set_terms(terms, p))
		প্রত্যাগম -1;

	যদি (!strcmp(p, "start")) {
		সংরচনা strvec argv = STRVEC_INIT;
		পূর্ণাঙ্ক res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_start(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		প্রত্যাগম res;
	}

	যদি (one_of(p, terms->term_good,
	   terms->term_bad, "skip", রিক্ত))
		প্রত্যাগম bisect_write(p, rev, terms, 0);

	যদি (!strcmp(p, "terms")) {
		সংরচনা strvec argv = STRVEC_INIT;
		পূর্ণাঙ্ক res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_terms(terms, argv.nr == 1 ? argv.v[0] : রিক্ত);
		strvec_clear(&argv);
		প্রত্যাগম res;
	}
	error(_("'%s'?? what are you talking about?"), p);

	প্রত্যাগম -1;
}

স্থির গণনা bisect_error bisect_replay(সংরচনা bisect_terms *terms, ধ্রুব বর্ণ *filename)
{
	FILE *fp = রিক্ত;
	গণনা bisect_error res = BISECT_OK;
	সংরচনা strbuf সারি = STRBUF_INIT;

	যদি (is_empty_or_missing_file(filename))
		প্রত্যাগম error(_("cannot read file '%s' for replaying"), filename);

	যদি (bisect_clean_state())
		প্রত্যাগম BISECT_FAILED;

	fp = fopen(filename, "r");
	যদি (!fp)
		প্রত্যাগম BISECT_FAILED;

	যাবত্ ((strbuf_getline(&সারি, fp) != EOF) && !res)
		res = process_replay_সারি(terms, &সারি);

	strbuf_release(&সারি);
	fclose(fp);

	যদি (res)
		প্রত্যাগম BISECT_FAILED;

	প্রত্যাগম bisect_auto_next(terms, রিক্ত);
}

স্থির গণনা bisect_error bisect_skip(সংরচনা bisect_terms *terms, পূর্ণাঙ্ক argc,
				     ধ্রুব বর্ণ **argv)
{
	পূর্ণাঙ্ক i;
	গণনা bisect_error res;
	সংরচনা strvec argv_state = STRVEC_INIT;

	strvec_ঠেলো(&argv_state, "skip");

	প্রতি (i = 0; i < argc; i++) {
		ধ্রুব বর্ণ *dotdot = strstr(argv[i], "..");

		যদি (dotdot) {
			সংরচনা rev_info revs;
			সংরচনা commit *commit;

			repo_init_revisions(the_repository, &revs, রিক্ত);
			setup_revisions(2, argv + i - 1, &revs, রিক্ত);

			যদি (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			যাবত্ ((commit = get_revision(&revs)) != রিক্ত)
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
	প্রত্যাগম res;
}

স্থির পূর্ণাঙ্ক bisect_visualize(সংরচনা bisect_terms *terms, পূর্ণাঙ্ক argc,
			    ধ্রুব বর্ণ **argv)
{
	সংরচনা child_process cmd = CHILD_PROCESS_INIT;
	সংরচনা strbuf sb = STRBUF_INIT;

	যদি (bisect_next_check(terms, রিক্ত) != 0)
		প্রত্যাগম BISECT_FAILED;

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

	strvec_pushl(&cmd.args, "--bisect", "--", রিক্ত);

	strbuf_read_file(&sb, git_path_bisect_names(), 0);
	sq_dequote_to_strvec(sb.buf, &cmd.args);
	strbuf_release(&sb);

	প্রত্যাগম run_command(&cmd);
}

স্থির পূর্ণাঙ্ক get_first_good(ধ্রুব সংরচনা reference *ref, শূন্য *cb_data)
{
	oidcpy(cb_data, ref->oid);
	প্রত্যাগম 1;
}

স্থির পূর্ণাঙ্ক do_bisect_run(ধ্রুব বর্ণ *command)
{
	সংরচনা child_process cmd = CHILD_PROCESS_INIT;

	printf(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_ঠেলো(&cmd.args, command);
	প্রত্যাগম run_command(&cmd);
}

স্থির পূর্ণাঙ্ক verify_good(ধ্রুব সংরচনা bisect_terms *terms, ধ্রুব বর্ণ *command)
{
	পূর্ণাঙ্ক rc;
	গণনা bisect_error res;
	সংরচনা object_id good_rev;
	সংরচনা object_id current_rev;
	বর্ণ *good_glob = xstrfmt("%s-*", terms->term_good);
	পূর্ণাঙ্ক no_checkout = refs_ref_exists(get_main_ref_store(the_repository),
					  "BISECT_HEAD");
	সংরচনা refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_first_good, &good_rev, &opts);
	মুক্ত(good_glob);

	যদি (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		প্রত্যাগম -1;

	res = bisect_checkout(&good_rev, no_checkout);
	যদি (res != BISECT_OK)
		প্রত্যাগম -1;

	rc = do_bisect_run(command);

	res = bisect_checkout(&current_rev, no_checkout);
	যদি (res != BISECT_OK)
		প্রত্যাগম -1;

	প্রত্যাগম rc;
}

স্থির পূর্ণাঙ্ক bisect_run(সংরচনা bisect_terms *terms, পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv)
{
	পূর্ণাঙ্ক res = BISECT_OK;
	সংরচনা strbuf command = STRBUF_INIT;
	ধ্রুব বর্ণ *reset_when_found_arg;
	ধ্রুব বর্ণ *new_state;
	পূর্ণাঙ্ক temporary_stdout_fd, saved_stdout;
	পূর্ণাঙ্ক is_first_run = 1;
	গণনা reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;

	যদি (bisect_next_check(terms, রিক্ত))
		প্রত্যাগম BISECT_FAILED;

	যদি (argc && !strcmp(argv[0], "--reset-when-found")) {
		reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
	} অন্যথা যদি (argc && skip_prefix(argv[0], "--reset-when-found=",
				    &reset_when_found_arg)) {
		যদি (parse_reset_when_found(reset_when_found_arg,
					   &reset_when_found))
			প্রত্যাগম BISECT_FAILED;
	}

	যদি (reset_when_found != RESET_WHEN_FOUND_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		প্রত্যাগম error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	যদি (reset_when_found != RESET_WHEN_FOUND_NONE) {
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));
		argc--;
		argv++;
	}

	যদি (!argc) {
		error(_("bisect run failed: no command provided."));
		প্রত্যাগম BISECT_FAILED;
	}

	sq_quote_argv(&command, argv);
	strbuf_ltrim(&command);
	যাবত্ (1) {
		res = do_bisect_run(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		যদি (is_first_run && (res == 126 || res == 127)) {
			পূর্ণাঙ্ক rc = verify_good(terms, command.buf);
			is_first_run = 0;
			যদি (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->term_good);
				res = BISECT_FAILED;
				ভঙ্গ;
			}
			যদি (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->term_good);
				res = BISECT_FAILED;
				ভঙ্গ;
			}
		}

		যদি (res < 0 || 128 <= res) {
			error(_("bisect run failed: exit code %d from"
				" %s is < 0 or >= 128"), res, command.buf);
			ভঙ্গ;
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
			ভঙ্গ;
		}

		fflush(stdout);
		saved_stdout = dup(1);
		যদি (saved_stdout < 0 ||
		    dup2(temporary_stdout_fd, 1) < 0) {
			res = error_errno(_("could not duplicate stdout"));
			যদি (saved_stdout >= 0)
				close(saved_stdout);
			close(temporary_stdout_fd);
			ভঙ্গ;
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
			অনুবর্ত;
		}
		ভঙ্গ;
	}

	strbuf_release(&command);
	প্রত্যাগম res;
}

স্থির পূর্ণাঙ্ক cmd_bisect__reset(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix UNUSED,
			     সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক res;

	যদি (argc > 1)
		প্রত্যাগম error(_("'%s' requires either no argument or a commit"),
			     "git bisect reset");
	res = bisect_reset(argc ? argv[0] : রিক্ত, false);
	যদি (res)
		প্রত্যাগম res;
	প্রত্যাগম bisect_clean_state();
}

স্থির পূর্ণাঙ্ক cmd_bisect__terms(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix UNUSED,
			     সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক res;
	সংরচনা bisect_terms terms = { 0 };

	যদি (argc > 1)
		প্রত্যাগম error(_("'%s' requires 0 or 1 argument"),
			     "git bisect terms");
	res = bisect_terms(&terms, argc == 1 ? argv[0] : রিক্ত);
	free_terms(&terms);
	প্রত্যাগম res;
}

স্থির পূর্ণাঙ্ক cmd_bisect__start(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix UNUSED,
			     সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক res;
	সংরচনা bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	res = bisect_start(&terms, argc, argv);
	free_terms(&terms);
	প্রত্যাগম res;
}

স্থির পূর্ণাঙ্ক cmd_bisect__next(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv UNUSED, ধ্রুব বর্ণ *prefix,
			    সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক res;
	সংরচনা bisect_terms terms = { 0 };

	যদি (argc)
		প্রত্যাগম error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	যদি (get_terms(&terms, 1))
		প্রত্যাগম error(_("no terms defined"));
	res = bisect_next(&terms, prefix);
	free_terms(&terms);
	প্রত্যাগম res;
}

স্থির পূর্ণাঙ্ক cmd_bisect__log(পূর্ণাঙ্ক argc UNUSED, ধ্রুব বর্ণ **argv UNUSED,
			   ধ্রুব বর্ণ *prefix UNUSED,
			   সংরচনা repository *repo UNUSED)
{
	প্রত্যাগম bisect_log();
}

স্থির পূর্ণাঙ্ক cmd_bisect__replay(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix UNUSED,
			      সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক res;
	সংরচনা bisect_terms terms = { 0 };

	যদি (argc != 1)
		প্রত্যাগম error(_("no logfile given"));
	set_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	প্রত্যাগম res;
}

স্থির পূর্ণাঙ্ক cmd_bisect__skip(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix UNUSED,
			    সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক res;
	সংরচনা bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	যদি (get_terms(&terms, 1))
		প্রত্যাগম error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	প্রত্যাগম res;
}

স্থির পূর্ণাঙ্ক cmd_bisect__visualize(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix UNUSED,
				 সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক res;
	সংরচনা bisect_terms terms = { 0 };

	যদি (get_terms(&terms, 1))
		প্রত্যাগম error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	প্রত্যাগম res;
}

স্থির পূর্ণাঙ্ক cmd_bisect__run(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix UNUSED,
			   সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক res;
	সংরচনা bisect_terms terms = { 0 };

	যদি (!argc)
		প্রত্যাগম error(_("'%s' failed: no command provided."), "git bisect run");
	যদি (get_terms(&terms, 1))
		প্রত্যাগম error(_("no terms defined"));
	res = bisect_run(&terms, argc, argv);
	free_terms(&terms);
	প্রত্যাগম res;
}

পূর্ণাঙ্ক cmd_bisect(পূর্ণাঙ্ক argc,
	       ধ্রুব বর্ণ **argv,
	       ধ্রুব বর্ণ *prefix,
	       সংরচনা repository *repo)
{
	পূর্ণাঙ্ক res = 0;
	parse_opt_subcommand_fn *fn = রিক্ত;
	সংরচনা option options[] = {
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

	যদি (!fn) {
		সংরচনা bisect_terms terms = { 0 };

		যদি (!argc)
			usage_বার্তা_opt(_("need a command"), git_bisect_usage, options);

		যদি (!strcmp(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		set_terms(&terms, "bad", "good");
		যদি (get_terms(&terms, 1))
			প্রত্যাগম error(_("no terms defined"));
		যদি (check_and_set_terms(&terms, argv[0]) ||
		    !one_of(argv[0], terms.term_good, terms.term_bad, রিক্ত))
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

	প্রত্যাগম is_bisect_success(res) ? 0 : -res;
}
