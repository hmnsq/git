#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "copy.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "object-name.h"
#شامل "parse-options.h"
#شامل "bisect.h"
#شامل "refs.h"
#شامل "strvec.h"
#شامل "run-command.h"
#شامل "oid-array.h"
#شامل "path.h"
#شامل "prompt.h"
#شامل "quote.h"
#شامل "revision.h"

ایستا GIT_PATH_FUNC(git_path_bisect_terms, "BISECT_TERMS")
ایستا GIT_PATH_FUNC(git_path_bisect_ancestors_ok, "BISECT_ANCESTORS_OK")
ایستا GIT_PATH_FUNC(git_path_bisect_start, "BISECT_START")
ایستا GIT_PATH_FUNC(git_path_bisect_log, "BISECT_LOG")
ایستا GIT_PATH_FUNC(git_path_bisect_names, "BISECT_NAMES")
ایستا GIT_PATH_FUNC(git_path_bisect_first_parent, "BISECT_FIRST_PARENT")
ایستا GIT_PATH_FUNC(git_path_bisect_reset_when_found, "BISECT_RESET_WHEN_FOUND")
ایستا GIT_PATH_FUNC(git_path_bisect_run, "BISECT_RUN")

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

ایستا ثابت نویسه * ثابت git_bisect_usage[] = {
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
	پوچ
};

شمارش reset_when_found_mode {
	RESET_WHEN_FOUND_NONE,
	RESET_WHEN_FOUND_TO_ORIGINAL,
	RESET_WHEN_FOUND_TO_FOUND,
};

ساختار add_bisect_ref_data {
	ساختار rev_info *revs;
	unsigned صحیح object_flags;
};

ساختار bisect_terms {
	نویسه *term_good;
	نویسه *term_bad;
};

ایستا تهی free_terms(ساختار bisect_terms *terms)
{
	FREE_AND_NULL(terms->term_good);
	FREE_AND_NULL(terms->term_bad);
}

ایستا تهی set_terms(ساختار bisect_terms *terms, ثابت نویسه *bad,
		      ثابت نویسه *good)
{
	آزاد((تهی *)terms->term_good);
	terms->term_good = xstrdup(good);
	آزاد((تهی *)terms->term_bad);
	terms->term_bad = xstrdup(bad);
}

ایستا ثابت نویسه vocab_bad[] = "bad|new";
ایستا ثابت نویسه vocab_good[] = "good|old";

ایستا صحیح bisect_autostart(ساختار bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
LAST_ARG_MUST_BE_NULL
ایستا صحیح one_of(ثابت نویسه *term, ...)
{
	صحیح res = 0;
	va_list matches;
	ثابت نویسه *match;

	va_start(matches, term);
	وقتی (!res && (match = va_arg(matches, ثابت نویسه *)))
		res = !strcmp(term, match);
	va_end(matches);

	بازگشت res;
}

/*
 * return code BISECT_INTERNAL_SUCCESS_MERGE_BASE
 * and BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND are codes
 * that indicate special success.
 */

ایستا صحیح is_bisect_success(شمارش bisect_error res)
{
	بازگشت !res ||
		res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND ||
		res == BISECT_INTERNAL_SUCCESS_MERGE_BASE;
}

ایستا صحیح write_in_file(ثابت نویسه *path, ثابت نویسه *mode, ثابت نویسه *format, va_list args)
{
	FILE *fp = پوچ;
	صحیح res = 0;

	اگر (strcmp(mode, "w") && strcmp(mode, "a"))
		BUG("write-in-file does not support '%s' mode", mode);
	fp = fopen(path, mode);
	اگر (!fp)
		بازگشت error_errno(_("cannot open file '%s' in mode '%s'"), path, mode);
	res = vfprintf(fp, format, args);

	اگر (res < 0) {
		صحیح saved_errno = errno;
		fclose(fp);
		errno = saved_errno;
		بازگشت error_errno(_("could not write to file '%s'"), path);
	}

	بازگشت fclose(fp);
}

__attribute__((format (printf, 2, 3)))
ایستا صحیح write_to_file(ثابت نویسه *path, ثابت نویسه *format, ...)
{
	صحیح res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "w", format, args);
	va_end(args);

	بازگشت res;
}

__attribute__((format (printf, 2, 3)))
ایستا صحیح append_to_file(ثابت نویسه *path, ثابت نویسه *format, ...)
{
	صحیح res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "a", format, args);
	va_end(args);

	بازگشت res;
}

ایستا صحیح print_file_to_stdout(ثابت نویسه *path)
{
	صحیح fd = open(path, O_RDONLY);
	صحیح ret = 0;

	اگر (fd < 0)
		بازگشت error_errno(_("cannot open file '%s' for reading"), path);
	اگر (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), path);
	close(fd);
	بازگشت ret;
}

ایستا صحیح check_term_format(ثابت نویسه *term, ثابت نویسه *orig_term)
{
	صحیح res;
	نویسه *new_term = xstrfmt("refs/bisect/%s", term);

	res = check_refname_format(new_term, 0);
	آزاد(new_term);

	اگر (res)
		بازگشت error(_("'%s' is not a valid term"), term);

	اگر (one_of(term, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", پوچ))
		بازگشت error(_("can't use the builtin command '%s' as a term"), term);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	اگر ((strcmp(orig_term, "bad") && one_of(term, "bad", "new", پوچ)) ||
		 (strcmp(orig_term, "good") && one_of(term, "good", "old", پوچ)))
		بازگشت error(_("can't change the meaning of the term '%s'"), term);

	بازگشت 0;
}

ایستا صحیح write_terms(ثابت نویسه *bad, ثابت نویسه *good)
{
	صحیح res;

	اگر (!strcmp(bad, good))
		بازگشت error(_("please use two different terms"));

	اگر (check_term_format(bad, "bad") || check_term_format(good, "good"))
		بازگشت -1;

	res = write_to_file(git_path_bisect_terms(), "%s\n%s\n", bad, good);

	بازگشت res;
}

ایستا صحیح bisect_reset(ثابت نویسه *commit, bool quiet)
{
	ساختار strbuf branch = STRBUF_INIT;

	اگر (!commit) {
		اگر (!strbuf_read_file(&branch, git_path_bisect_start(), 0))
			printf(_("We are not bisecting.\n"));
		وگرنه
			strbuf_rtrim(&branch);
	} وگرنه {
		ساختار object_id oid;

		اگر (repo_get_oid_commit(the_repository, commit, &oid))
			بازگشت error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	اگر (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		ساختار child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "checkout", "--ignore-other-worktrees", پوچ);
		اگر (quiet)
			strvec_افزودن(&cmd.args, "--quiet");
		strvec_pushl(&cmd.args, branch.buf, "--", پوچ);
		اگر (run_command(&cmd)) {
			error(_("could not check out original"
				" HEAD '%s'. Try 'git bisect"
				" reset <commit>'."), branch.buf);
			strbuf_release(&branch);
			بازگشت -1;
		}
	}

	strbuf_release(&branch);
	بازگشت 0;
}

ایستا صحیح parse_reset_when_found(ثابت نویسه *مقدار,
				  شمارش reset_when_found_mode *mode)
{
	اگر (!strcmp(مقدار, "original"))
		*mode = RESET_WHEN_FOUND_TO_ORIGINAL;
	وگرنه اگر (!strcmp(مقدار, "found"))
		*mode = RESET_WHEN_FOUND_TO_FOUND;
	وگرنه
		بازگشت error(_("invalid value for '--reset-when-found': '%s'"),
			     مقدار);

	بازگشت 0;
}

ایستا ثابت نویسه *reset_when_found_mode_name(شمارش reset_when_found_mode mode)
{
	انتخاب (mode) {
	حالت RESET_WHEN_FOUND_TO_ORIGINAL:
		بازگشت "original";
	حالت RESET_WHEN_FOUND_TO_FOUND:
		بازگشت "found";
	حالت RESET_WHEN_FOUND_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", mode);
}

ایستا صحیح read_reset_when_found(شمارش reset_when_found_mode *mode)
{
	ساختار strbuf مقدار = STRBUF_INIT;
	صحیح res = 0;

	*mode = RESET_WHEN_FOUND_NONE;
	اگر (is_empty_or_missing_file(git_path_bisect_reset_when_found()))
		بازگشت 0;

	اگر (strbuf_read_file(&مقدار, git_path_bisect_reset_when_found(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_path_bisect_reset_when_found());
		برو out;
	}
	strbuf_trim(&مقدار);
	اگر (parse_reset_when_found(مقدار.buf, mode))
		res = -1;

out:
	strbuf_release(&مقدار);
	بازگشت res;
}

ایستا صحیح bisect_reset_when_found(شمارش reset_when_found_mode mode)
{
	ساختار bisect_terms terms = { 0 };
	نویسه *commit = پوچ;
	صحیح res;

	اگر (mode == RESET_WHEN_FOUND_TO_FOUND) {
		read_bisect_terms(&terms.term_bad, &terms.term_good);
		commit = xstrfmt("refs/bisect/%s", terms.term_bad);
	} وگرنه اگر (mode == RESET_WHEN_FOUND_NONE) {
		BUG("automatic reset requested without a reset mode");
	}

	res = bisect_reset(commit, true);
	اگر (!res)
		res = bisect_clean_state();

	آزاد(commit);
	free_terms(&terms);
	بازگشت res;
}

ایستا تهی log_commit(FILE *fp,
		       ثابت نویسه *fmt, ثابت نویسه *state,
		       ساختار commit *commit)
{
	ساختار pretty_print_context pp = {0};
	ساختار strbuf commit_پیام = STRBUF_INIT;
	نویسه *label = xstrfmt(fmt, state);

	repo_format_commit_message(the_repository, commit, "%s", &commit_پیام,
				   &pp);

	fprintf(fp, "# %s: [%s] %s\n", label, oid_to_hex(&commit->object.oid),
		commit_پیام.buf);

	strbuf_release(&commit_پیام);
	آزاد(label);
}

ایستا صحیح bisect_write(ثابت نویسه *state, ثابت نویسه *rev,
			ثابت ساختار bisect_terms *terms, صحیح nolog)
{
	ساختار strbuf tag = STRBUF_INIT;
	ساختار object_id oid;
	ساختار commit *commit;
	FILE *fp = پوچ;
	صحیح res = 0;

	اگر (!strcmp(state, terms->term_bad)) {
		strbuf_addf(&tag, "refs/bisect/%s", state);
	} وگرنه اگر (one_of(state, terms->term_good, "skip", پوچ)) {
		strbuf_addf(&tag, "refs/bisect/%s-%s", state, rev);
	} وگرنه {
		res = error(_("Bad bisect_write argument: %s"), state);
		برو finish;
	}

	اگر (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		برو finish;
	}

	اگر (refs_update_ref(get_main_ref_store(the_repository), پوچ, tag.buf, &oid, پوچ, 0,
			    UPDATE_REFS_پیام_ON_ERR)) {
		res = -1;
		برو finish;
	}

	fp = fopen(git_path_bisect_log(), "a");
	اگر (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_path_bisect_log());
		برو finish;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	اگر (!nolog)
		fprintf(fp, "git bisect %s %s\n", state, rev);

finish:
	اگر (fp)
		fclose(fp);
	strbuf_release(&tag);
	بازگشت res;
}

ایستا صحیح check_and_set_terms(ساختار bisect_terms *terms, ثابت نویسه *cmd)
{
	صحیح has_term_file = !is_empty_or_missing_file(git_path_bisect_terms());

	اگر (one_of(cmd, "skip", "start", "terms", پوچ))
		بازگشت 0;

	اگر (has_term_file && strcmp(cmd, terms->term_bad) &&
	    strcmp(cmd, terms->term_good))
		بازگشت error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->term_bad,
				terms->term_good);

	اگر (!has_term_file) {
		اگر (one_of(cmd, "bad", "good", پوچ)) {
			set_terms(terms, "bad", "good");
			بازگشت write_terms(terms->term_bad, terms->term_good);
		}
		اگر (one_of(cmd, "new", "old", پوچ)) {
			set_terms(terms, "new", "old");
			بازگشت write_terms(terms->term_bad, terms->term_good);
		}
	}

	بازگشت 0;
}

ایستا صحیح inc_nr(ثابت ساختار reference *ref UNUSED, تهی *cb_data)
{
	unsigned صحیح *nr = (unsigned صحیح *)cb_data;
	(*nr)++;
	بازگشت 0;
}

ایستا ثابت نویسه need_bad_and_good_revision_warning[] =
	N_("You need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

ایستا ثابت نویسه need_bisect_start_warning[] =
	N_("You need to start by \"git bisect start\".\n"
	   "You then need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

ایستا صحیح decide_next(ثابت ساختار bisect_terms *terms,
		       ثابت نویسه *current_term, صحیح missing_good,
		       صحیح missing_bad)
{
	اگر (!missing_good && !missing_bad)
		بازگشت 0;
	اگر (!current_term)
		بازگشت -1;

	اگر (missing_good && !missing_bad &&
	    !strcmp(current_term, terms->term_good)) {
		نویسه *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->term_bad);
		اگر (!isatty(0))
			بازگشت 0;
		/*
		 * TRANSLATORS: Make sure to include [Y] and [n] in your
		 * translation. The program will only accept English input
		 * at this point.
		 */
		yesno = git_prompt(_("Are you sure [Y/n]? "), PROMPT_ECHO);
		اگر (starts_with(yesno, "N") || starts_with(yesno, "n"))
			بازگشت -1;
		بازگشت 0;
	}

	اگر (!is_empty_or_missing_file(git_path_bisect_start()))
		بازگشت error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	وگرنه
		بازگشت error(_(need_bisect_start_warning),
			     vocab_good, vocab_bad, vocab_good, vocab_bad);
}

ایستا تهی bisect_status(ساختار bisect_state *state,
			  ثابت ساختار bisect_terms *terms)
{
	نویسه *bad_ref = xstrfmt("refs/bisect/%s", terms->term_bad);
	نویسه *good_glob = xstrfmt("%s-*", terms->term_good);
	ساختار refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	اگر (refs_ref_exists(get_main_ref_store(the_repository), bad_ref))
		state->nr_bad = 1;

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      inc_nr, &state->nr_good, &opts);

	آزاد(good_glob);
	آزاد(bad_ref);
}

__attribute__((format (printf, 1, 2)))
ایستا تهی bisect_log_printf(ثابت نویسه *fmt, ...)
{
	ساختار strbuf buf = STRBUF_INIT;
	va_list ap;

	va_start(ap, fmt);
	strbuf_vaddf(&buf, fmt, ap);
	va_end(ap);

	printf("%s", buf.buf);
	append_to_file(git_path_bisect_log(), "# %s", buf.buf);

	strbuf_release(&buf);
}

ایستا تهی bisect_print_status(ثابت ساختار bisect_terms *terms)
{
	ساختار bisect_state state = { 0 };

	bisect_status(&state, terms);

	/* If we had both, we'd already be started, and shouldn't get here. */
	اگر (state.nr_good && state.nr_bad)
		بازگشت;

	اگر (!state.nr_good && !state.nr_bad)
		bisect_log_printf(_("status: waiting for both '%s' and '%s' commits\n"),
				  terms->term_good, terms->term_bad);
	وگرنه اگر (state.nr_good)
		bisect_log_printf(Q_("status: waiting for '%s' commit, %d '%s' commit known\n",
				     "status: waiting for '%s' commit, %d '%s' commits known\n",
				     state.nr_good),
				  terms->term_bad, state.nr_good, terms->term_good);
	وگرنه
		bisect_log_printf(_("status: waiting for '%s' commit(s), '%s' commit known\n"),
				  terms->term_good, terms->term_bad);
}

ایستا صحیح bisect_next_check(ثابت ساختار bisect_terms *terms,
			     ثابت نویسه *current_term)
{
	ساختار bisect_state state = { 0 };
	bisect_status(&state, terms);
	بازگشت decide_next(terms, current_term, !state.nr_good, !state.nr_bad);
}

ایستا صحیح get_terms(ساختار bisect_terms *terms, صحیح file_missing_is_ok)
{
	ساختار strbuf str = STRBUF_INIT;
	FILE *fp = پوچ;
	صحیح res = 0;

	fp = fopen(git_path_bisect_terms(), "r");
	اگر (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		برو finish;
	}

	free_terms(terms);
	اگر (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		برو finish;
	}
	terms->term_bad = strbuf_detach(&str, پوچ);
	اگر (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		FREE_AND_NULL(terms->term_bad);
		برو finish;
	}
	terms->term_good = strbuf_detach(&str, پوچ);

finish:
	اگر (fp)
		fclose(fp);
	strbuf_release(&str);
	بازگشت res;
}

ایستا صحیح bisect_terms(ساختار bisect_terms *terms, ثابت نویسه *option)
{
	اگر (get_terms(terms, 0))
		بازگشت error(_("no terms defined"));

	اگر (!option) {
		printf(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->term_good, terms->term_bad);
		بازگشت 0;
	}
	اگر (one_of(option, "--term-good", "--term-old", پوچ))
		printf("%s\n", terms->term_good);
	وگرنه اگر (one_of(option, "--term-bad", "--term-new", پوچ))
		printf("%s\n", terms->term_bad);
	وگرنه
		بازگشت error(_("invalid argument %s for 'git bisect terms'.\n"
			       "Supported options are: "
			       "--term-good|--term-old and "
			       "--term-bad|--term-new."), option);

	بازگشت 0;
}

ایستا صحیح bisect_append_log_quoted(ثابت نویسه **argv)
{
	صحیح res = 0;
	FILE *fp = fopen(git_path_bisect_log(), "a");
	ساختار strbuf orig_args = STRBUF_INIT;

	اگر (!fp)
		بازگشت -1;

	اگر (fprintf(fp, "git bisect start") < 1) {
		res = -1;
		برو finish;
	}

	sq_quote_argv(&orig_args, argv);
	اگر (fprintf(fp, "%s\n", orig_args.buf) < 1)
		res = -1;

finish:
	fclose(fp);
	strbuf_release(&orig_args);
	بازگشت res;
}

ایستا صحیح add_bisect_ref(ثابت ساختار reference *ref, تهی *cb)
{
	ساختار add_bisect_ref_data *data = cb;

	add_pending_oid(data->revs, ref->name, ref->oid, data->object_flags);

	بازگشت 0;
}

ایستا صحیح prepare_revs(ساختار bisect_terms *terms, ساختار rev_info *revs)
{
	ساختار refs_for_each_ref_options opts = {
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};
	صحیح res = 0;
	ساختار add_bisect_ref_data cb = { revs };
	نویسه *good = xstrfmt("%s-*", terms->term_good);

	/*
	 * We cannot use terms->term_bad directly in
	 * for_each_glob_ref_in() and we have to append a '*' to it,
	 * otherwise for_each_glob_ref_in() will append '/' and '*'.
	 */
	نویسه *bad = xstrfmt("%s*", terms->term_bad);

	/*
	 * It is important to reset the flags used by revision walks
	 * as the previous call to bisect_next_all() in turn
	 * sets up a revision walk.
	 */
	reset_revision_walk();
	repo_init_revisions(the_repository, revs, پوچ);
	setup_revisions(0, پوچ, revs, پوچ);

	opts.pattern = bad;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      add_bisect_ref, &cb, &opts);

	cb.object_flags = UNINTERESTING;
	opts.pattern = good;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      add_bisect_ref, &cb, &opts);

	اگر (prepare_revision_walk(revs))
		res = error(_("revision walk setup failed"));

	آزاد(good);
	آزاد(bad);
	بازگشت res;
}

ایستا صحیح bisect_skipped_commits(ساختار bisect_terms *terms)
{
	صحیح res;
	FILE *fp = پوچ;
	ساختار rev_info revs;
	ساختار commit *commit;
	ساختار pretty_print_context pp = {0};
	ساختار strbuf commit_name = STRBUF_INIT;

	res = prepare_revs(terms, &revs);
	اگر (res)
		بازگشت res;

	fp = fopen(git_path_bisect_log(), "a");
	اگر (!fp)
		بازگشت error_errno(_("could not open '%s' for appending"),
				  git_path_bisect_log());

	اگر (fprintf(fp, "# only skipped commits left to test\n") < 0)
		بازگشت error_errno(_("failed to write to '%s'"), git_path_bisect_log());

	وقتی ((commit = get_revision(&revs)) != پوچ) {
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
	بازگشت 0;
}

ایستا صحیح bisect_successful(ساختار bisect_terms *terms)
{
	ساختار object_id oid;
	ساختار commit *commit;
	ساختار pretty_print_context pp = {0};
	ساختار strbuf commit_name = STRBUF_INIT;
	نویسه *bad_ref = xstrfmt("refs/bisect/%s",terms->term_bad);
	صحیح res;

	refs_read_ref(get_main_ref_store(the_repository), bad_ref, &oid);
	commit = lookup_commit_reference_by_name(bad_ref);
	اگر (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		آزاد(bad_ref);
		بازگشت BISECT_FAILED;
	}
	repo_format_commit_message(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_to_file(git_path_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->term_bad, oid_to_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	آزاد(bad_ref);
	بازگشت res;
}

ایستا شمارش bisect_error bisect_next(ساختار bisect_terms *terms,
				     ثابت نویسه *prefix)
{
	شمارش bisect_error res;

	اگر (bisect_autostart(terms))
		بازگشت BISECT_FAILED;

	اگر (bisect_next_check(terms, terms->term_good))
		بازگشت BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_next_all(the_repository, prefix);

	اگر (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		res = bisect_successful(terms);
		بازگشت res ? res : BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND;
	} وگرنه اگر (res == BISECT_ONLY_SKIPPED_چپ) {
		res = bisect_skipped_commits(terms);
		بازگشت res ? res : BISECT_ONLY_SKIPPED_چپ;
	}
	بازگشت res;
}

ایستا شمارش bisect_error bisect_auto_next(ساختار bisect_terms *terms,
					  ثابت نویسه *prefix)
{
	اگر (bisect_next_check(terms, پوچ)) {
		bisect_print_status(terms);
		بازگشت BISECT_OK;
	}

	بازگشت bisect_next(terms, prefix);
}

ایستا شمارش bisect_error bisect_start(ساختار bisect_terms *terms, صحیح argc,
				      ثابت نویسه **argv)
{
	صحیح no_checkout = 0;
	صحیح first_parent_only = 0;
	صحیح i, has_double_dash = 0, must_write_terms = 0, bad_seen = 0;
	صحیح flags, pathspec_pos;
	شمارش bisect_error res = BISECT_OK;
	ساختار string_list revs = STRING_LIST_INIT_DUP;
	ساختار string_list states = STRING_LIST_INIT_DUP;
	ساختار strbuf start_head = STRBUF_INIT;
	ساختار strbuf bisect_names = STRBUF_INIT;
	ساختار object_id head_oid;
	ساختار object_id oid;
	شمارش reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;
	ثابت نویسه *head;

	اگر (is_bare_repository(the_repository))
		no_checkout = 1;

	/*
	 * Check for one bad and then some good revisions
	 */
	برای (i = 0; i < argc; i++) {
		اگر (!strcmp(argv[i], "--")) {
			has_double_dash = 1;
			بشکن;
		}
	}

	برای (i = 0; i < argc; i++) {
		ثابت نویسه *arg = argv[i];
		اگر (!strcmp(argv[i], "--")) {
			بشکن;
		} وگرنه اگر (!strcmp(arg, "--no-checkout")) {
			no_checkout = 1;
		} وگرنه اگر (!strcmp(arg, "--first-parent")) {
			first_parent_only = 1;
		} وگرنه اگر (!strcmp(arg, "--reset-when-found")) {
			reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
		} وگرنه اگر (skip_prefix(arg, "--reset-when-found=", &arg)) {
			اگر (parse_reset_when_found(arg, &reset_when_found)) {
				res = BISECT_FAILED;
				برو finish;
			}
		} وگرنه اگر (!strcmp(arg, "--term-good") ||
			 !strcmp(arg, "--term-old")) {
			i++;
			اگر (argc <= i)
				بازگشت error(_("'' is not a valid term"));
			must_write_terms = 1;
			آزاد((تهی *) terms->term_good);
			terms->term_good = xstrdup(argv[i]);
		} وگرنه اگر (skip_prefix(arg, "--term-good=", &arg) ||
			   skip_prefix(arg, "--term-old=", &arg)) {
			must_write_terms = 1;
			آزاد((تهی *) terms->term_good);
			terms->term_good = xstrdup(arg);
		} وگرنه اگر (!strcmp(arg, "--term-bad") ||
			 !strcmp(arg, "--term-new")) {
			i++;
			اگر (argc <= i)
				بازگشت error(_("'' is not a valid term"));
			must_write_terms = 1;
			آزاد((تهی *) terms->term_bad);
			terms->term_bad = xstrdup(argv[i]);
		} وگرنه اگر (skip_prefix(arg, "--term-bad=", &arg) ||
			   skip_prefix(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			آزاد((تهی *) terms->term_bad);
			terms->term_bad = xstrdup(arg);
		} وگرنه اگر (starts_with(arg, "--")) {
			بازگشت error(_("unrecognized option: '%s'"), arg);
		} وگرنه اگر (!get_oidf(&oid, "%s^{commit}", arg)) {
			string_list_append(&revs, oid_to_hex(&oid));
		} وگرنه اگر (has_double_dash) {
			die(_("'%s' does not appear to be a valid "
			      "revision"), arg);
		} وگرنه {
			بشکن;
		}
	}
	اگر (reset_when_found != RESET_WHEN_FOUND_NONE && no_checkout) {
		res = error(_("options '%s' and '%s' cannot be used together"),
			    "--reset-when-found", "--no-checkout");
		برو finish;
	}
	pathspec_pos = i;

	/*
	 * The user ran "git bisect start <sha1> <sha1>", hence did not
	 * explicitly specify the terms, but we are already starting to
	 * set references named with the default terms, and won't be able
	 * to change afterwards.
	 */
	اگر (revs.nr)
		must_write_terms = 1;
	برای (i = 0; i < revs.nr; i++) {
		اگر (bad_seen) {
			string_list_append(&states, terms->term_good);
		} وگرنه {
			bad_seen = 1;
			string_list_append(&states, terms->term_bad);
		}
	}

	/*
	 * Verify HEAD
	 */
	head = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
				       "HEAD", 0, &head_oid, &flags);
	اگر (!head) {
		اگر (repo_get_oid(the_repository, "HEAD", &head_oid))
			بازگشت error(_("bad HEAD - I need a HEAD"));
		head = "HEAD";
	}

	/*
	 * Check if we are bisecting
	 */
	اگر (!is_empty_or_missing_file(git_path_bisect_start())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&start_head, git_path_bisect_start(), 0);
		strbuf_trim(&start_head);
		اگر (!no_checkout) {
			ساختار child_process cmd = CHILD_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.args, "checkout", start_head.buf,
				     "--", پوچ);
			اگر (run_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       start_head.buf);
				برو finish;
			}
		}
	} وگرنه {
		/* Get the rev from where we start. */
		اگر (!repo_get_oid(the_repository, head, &head_oid) &&
		    !starts_with(head, "refs/heads/")) {
			strbuf_reset(&start_head);
			strbuf_add_oid_hex(&start_head, &head_oid);
		} وگرنه اگر (!repo_get_oid(the_repository, head, &head_oid) &&
			   skip_prefix(head, "refs/heads/", &head)) {
			strbuf_addstr(&start_head, head);
		} وگرنه {
			بازگشت error(_("bad HEAD - strange symbolic ref"));
		}
	}

	/*
	 * Get rid of any old bisect state.
	 */
	اگر (bisect_clean_state())
		بازگشت BISECT_FAILED;

	/*
	 * Write new start state
	 */
	write_file(git_path_bisect_start(), "%s\n", start_head.buf);

	اگر (first_parent_only)
		write_file(git_path_bisect_first_parent(), "\n");

	اگر (reset_when_found != RESET_WHEN_FOUND_NONE)
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));

	اگر (no_checkout) {
		اگر (repo_get_oid(the_repository, start_head.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), start_head.buf);
			برو finish;
		}
		اگر (refs_update_ref(get_main_ref_store(the_repository), پوچ, "BISECT_HEAD", &oid, پوچ, 0,
				    UPDATE_REFS_پیام_ON_ERR)) {
			res = BISECT_FAILED;
			برو finish;
		}
	}

	اگر (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_names, argv + pathspec_pos);
	write_file(git_path_bisect_names(), "%s\n", bisect_names.buf);

	برای (i = 0; i < states.nr; i++)
		اگر (bisect_write(states.اقلام[i].string,
				 revs.اقلام[i].string, terms, 1)) {
			res = BISECT_FAILED;
			برو finish;
		}

	اگر (must_write_terms && write_terms(terms->term_bad,
					    terms->term_good)) {
		res = BISECT_FAILED;
		برو finish;
	}

	res = bisect_append_log_quoted(argv);
	اگر (res)
		res = BISECT_FAILED;

finish:
	string_list_clear(&revs, 0);
	string_list_clear(&states, 0);
	strbuf_release(&start_head);
	strbuf_release(&bisect_names);
	اگر (res)
		بازگشت res;

	res = bisect_auto_next(terms, پوچ);
	اگر (!is_bisect_success(res))
		bisect_clean_state();
	بازگشت res;
}

ایستا درونخطی صحیح file_is_not_empty(ثابت نویسه *path)
{
	بازگشت !is_empty_or_missing_file(path);
}

ایستا صحیح bisect_autostart(ساختار bisect_terms *terms)
{
	صحیح res;
	ثابت نویسه *yesno;

	اگر (file_is_not_empty(git_path_bisect_start()))
		بازگشت 0;

	fprintf_ln(stderr, _("You need to start by \"git bisect "
			  "start\"\n"));

	اگر (!isatty(STDIN_FILENO))
		بازگشت -1;

	/*
	 * TRANSLATORS: Make sure to include [Y] and [n] in your
	 * translation. The program will only accept English input
	 * at this point.
	 */
	yesno = git_prompt(_("Do you want me to do it for you "
			     "[Y/n]? "), PROMPT_ECHO);
	res = tolower(*yesno) == 'n' ?
		-1 : bisect_start(terms, 0, empty_strvec);

	بازگشت res;
}

ایستا شمارش bisect_error bisect_state(ساختار bisect_terms *terms, صحیح argc,
				      ثابت نویسه **argv)
{
	ثابت نویسه *state;
	صحیح i, verify_expected = 1;
	ساختار object_id oid, expected;
	ساختار oid_array revs = OID_ARRAY_INIT;

	اگر (!argc)
		بازگشت error(_("Please call `--bisect-state` with at least one argument"));

	اگر (bisect_autostart(terms))
		بازگشت BISECT_FAILED;

	state = argv[0];
	اگر (check_and_set_terms(terms, state) ||
	    !one_of(state, terms->term_good, terms->term_bad, "skip", پوچ))
		بازگشت BISECT_FAILED;

	argv++;
	argc--;
	اگر (argc > 1 && !strcmp(state, terms->term_bad))
		بازگشت error(_("'git bisect %s' can take only one argument."), terms->term_bad);

	اگر (argc == 0) {
		ثابت نویسه *head = "BISECT_HEAD";
		شمارش get_oid_result res_head = repo_get_oid(the_repository,
							    head, &oid);

		اگر (res_head == MISSING_OBJECT) {
			head = "HEAD";
			res_head = repo_get_oid(the_repository, head, &oid);
		}

		اگر (res_head)
			error(_("Bad rev input: %s"), head);
		oid_array_append(&revs, &oid);
	}

	/*
	 * All input revs must be checked before executing bisect_write()
	 * to discard junk revs.
	 */

	برای (; argc; argc--, argv++) {
		ساختار commit *commit;

		اگر (repo_get_oid(the_repository, *argv, &oid)){
			error(_("Bad rev input: %s"), *argv);
			oid_array_clear(&revs);
			بازگشت BISECT_FAILED;
		}

		commit = lookup_commit_reference(the_repository, &oid);
		اگر (!commit)
			die(_("Bad rev input (not a commit): %s"), *argv);

		oid_array_append(&revs, &commit->object.oid);
	}

	اگر (refs_read_ref(get_main_ref_store(the_repository), "BISECT_EXPECTED_REV", &expected))
		verify_expected = 0; /* Ignore invalid file contents */

	برای (i = 0; i < revs.nr; i++) {
		اگر (bisect_write(state, oid_to_hex(&revs.oid[i]), terms, 0)) {
			oid_array_clear(&revs);
			بازگشت BISECT_FAILED;
		}
		اگر (verify_expected && !oideq(&revs.oid[i], &expected)) {
			unlink_or_warn(git_path_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					پوچ, "BISECT_EXPECTED_REV", پوچ,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	بازگشت bisect_auto_next(terms, پوچ);
}

ایستا شمارش bisect_error bisect_log(تهی)
{
	صحیح fd, status;
	ثابت نویسه* filename = git_path_bisect_log();

	اگر (is_empty_or_missing_file(filename))
		بازگشت error(_("We are not bisecting."));

	fd = open(filename, O_RDONLY);
	اگر (fd < 0)
		بازگشت BISECT_FAILED;

	status = copy_fd(fd, STDOUT_FILENO);
	close(fd);
	بازگشت status ? BISECT_FAILED : BISECT_OK;
}

ایستا صحیح process_replay_سطر(ساختار bisect_terms *terms, ساختار strbuf *سطر)
{
	ثابت نویسه *p = سطر->buf + strspn(سطر->buf, " \t");
	نویسه *word_end, *rev;

	اگر ((!skip_prefix(p, "git bisect", &p) &&
	!skip_prefix(p, "git-bisect", &p)) || !isspace(*p))
		بازگشت 0;
	p += strspn(p, " \t");

	word_end = (نویسه *)p + strcspn(p, " \t");
	rev = word_end + strspn(word_end, " \t");
	*word_end = '\0'; /* NUL-terminate the word */

	اگر (get_terms(terms, 1))
		بازگشت error(_("no terms defined"));
	اگر (check_and_set_terms(terms, p))
		بازگشت -1;

	اگر (!strcmp(p, "start")) {
		ساختار strvec argv = STRVEC_INIT;
		صحیح res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_start(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		بازگشت res;
	}

	اگر (one_of(p, terms->term_good,
	   terms->term_bad, "skip", پوچ))
		بازگشت bisect_write(p, rev, terms, 0);

	اگر (!strcmp(p, "terms")) {
		ساختار strvec argv = STRVEC_INIT;
		صحیح res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_terms(terms, argv.nr == 1 ? argv.v[0] : پوچ);
		strvec_clear(&argv);
		بازگشت res;
	}
	error(_("'%s'?? what are you talking about?"), p);

	بازگشت -1;
}

ایستا شمارش bisect_error bisect_replay(ساختار bisect_terms *terms, ثابت نویسه *filename)
{
	FILE *fp = پوچ;
	شمارش bisect_error res = BISECT_OK;
	ساختار strbuf سطر = STRBUF_INIT;

	اگر (is_empty_or_missing_file(filename))
		بازگشت error(_("cannot read file '%s' for replaying"), filename);

	اگر (bisect_clean_state())
		بازگشت BISECT_FAILED;

	fp = fopen(filename, "r");
	اگر (!fp)
		بازگشت BISECT_FAILED;

	وقتی ((strbuf_getline(&سطر, fp) != EOF) && !res)
		res = process_replay_سطر(terms, &سطر);

	strbuf_release(&سطر);
	fclose(fp);

	اگر (res)
		بازگشت BISECT_FAILED;

	بازگشت bisect_auto_next(terms, پوچ);
}

ایستا شمارش bisect_error bisect_skip(ساختار bisect_terms *terms, صحیح argc,
				     ثابت نویسه **argv)
{
	صحیح i;
	شمارش bisect_error res;
	ساختار strvec argv_state = STRVEC_INIT;

	strvec_افزودن(&argv_state, "skip");

	برای (i = 0; i < argc; i++) {
		ثابت نویسه *dotdot = strstr(argv[i], "..");

		اگر (dotdot) {
			ساختار rev_info revs;
			ساختار commit *commit;

			repo_init_revisions(the_repository, &revs, پوچ);
			setup_revisions(2, argv + i - 1, &revs, پوچ);

			اگر (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			وقتی ((commit = get_revision(&revs)) != پوچ)
				strvec_افزودن(&argv_state,
						oid_to_hex(&commit->object.oid));

			reset_revision_walk();
			release_revisions(&revs);
		} وگرنه {
			strvec_افزودن(&argv_state, argv[i]);
		}
	}
	res = bisect_state(terms, argv_state.nr, argv_state.v);

	strvec_clear(&argv_state);
	بازگشت res;
}

ایستا صحیح bisect_visualize(ساختار bisect_terms *terms, صحیح argc,
			    ثابت نویسه **argv)
{
	ساختار child_process cmd = CHILD_PROCESS_INIT;
	ساختار strbuf sb = STRBUF_INIT;

	اگر (bisect_next_check(terms, پوچ) != 0)
		بازگشت BISECT_FAILED;

	cmd.no_stdin = 1;
	اگر (!argc) {
		اگر ((getenv("DISPLAY") || getenv("SESSIONNAME") || getenv("MSYSTEM") ||
		     getenv("SECURITYSESSIONID")) && exists_in_PATH("gitk")) {
			strvec_افزودن(&cmd.args, "gitk");
		} وگرنه {
			strvec_افزودن(&cmd.args, "log");
			cmd.git_cmd = 1;
		}
	} وگرنه {
		اگر (argv[0][0] == '-') {
			strvec_افزودن(&cmd.args, "log");
			cmd.git_cmd = 1;
		} وگرنه اگر (strcmp(argv[0], "tig") && !starts_with(argv[0], "git"))
			cmd.git_cmd = 1;

		strvec_pushv(&cmd.args, argv);
	}

	strvec_pushl(&cmd.args, "--bisect", "--", پوچ);

	strbuf_read_file(&sb, git_path_bisect_names(), 0);
	sq_dequote_to_strvec(sb.buf, &cmd.args);
	strbuf_release(&sb);

	بازگشت run_command(&cmd);
}

ایستا صحیح get_first_good(ثابت ساختار reference *ref, تهی *cb_data)
{
	oidcpy(cb_data, ref->oid);
	بازگشت 1;
}

ایستا صحیح do_bisect_run(ثابت نویسه *command)
{
	ساختار child_process cmd = CHILD_PROCESS_INIT;

	printf(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_افزودن(&cmd.args, command);
	بازگشت run_command(&cmd);
}

ایستا صحیح verify_good(ثابت ساختار bisect_terms *terms, ثابت نویسه *command)
{
	صحیح rc;
	شمارش bisect_error res;
	ساختار object_id good_rev;
	ساختار object_id current_rev;
	نویسه *good_glob = xstrfmt("%s-*", terms->term_good);
	صحیح no_checkout = refs_ref_exists(get_main_ref_store(the_repository),
					  "BISECT_HEAD");
	ساختار refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_first_good, &good_rev, &opts);
	آزاد(good_glob);

	اگر (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		بازگشت -1;

	res = bisect_checkout(&good_rev, no_checkout);
	اگر (res != BISECT_OK)
		بازگشت -1;

	rc = do_bisect_run(command);

	res = bisect_checkout(&current_rev, no_checkout);
	اگر (res != BISECT_OK)
		بازگشت -1;

	بازگشت rc;
}

ایستا صحیح bisect_run(ساختار bisect_terms *terms, صحیح argc, ثابت نویسه **argv)
{
	صحیح res = BISECT_OK;
	ساختار strbuf command = STRBUF_INIT;
	ثابت نویسه *reset_when_found_arg;
	ثابت نویسه *new_state;
	صحیح temporary_stdout_fd, saved_stdout;
	صحیح is_first_run = 1;
	شمارش reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;

	اگر (bisect_next_check(terms, پوچ))
		بازگشت BISECT_FAILED;

	اگر (argc && !strcmp(argv[0], "--reset-when-found")) {
		reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
	} وگرنه اگر (argc && skip_prefix(argv[0], "--reset-when-found=",
				    &reset_when_found_arg)) {
		اگر (parse_reset_when_found(reset_when_found_arg,
					   &reset_when_found))
			بازگشت BISECT_FAILED;
	}

	اگر (reset_when_found != RESET_WHEN_FOUND_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		بازگشت error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	اگر (reset_when_found != RESET_WHEN_FOUND_NONE) {
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));
		argc--;
		argv++;
	}

	اگر (!argc) {
		error(_("bisect run failed: no command provided."));
		بازگشت BISECT_FAILED;
	}

	sq_quote_argv(&command, argv);
	strbuf_ltrim(&command);
	وقتی (1) {
		res = do_bisect_run(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		اگر (is_first_run && (res == 126 || res == 127)) {
			صحیح rc = verify_good(terms, command.buf);
			is_first_run = 0;
			اگر (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->term_good);
				res = BISECT_FAILED;
				بشکن;
			}
			اگر (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->term_good);
				res = BISECT_FAILED;
				بشکن;
			}
		}

		اگر (res < 0 || 128 <= res) {
			error(_("bisect run failed: exit code %d from"
				" %s is < 0 or >= 128"), res, command.buf);
			بشکن;
		}

		اگر (res == 125)
			new_state = "skip";
		وگرنه اگر (!res)
			new_state = terms->term_good;
		وگرنه
			new_state = terms->term_bad;

		temporary_stdout_fd = open(git_path_bisect_run(), O_CREAT | O_WRONLY | O_TRUNC, 0666);

		اگر (temporary_stdout_fd < 0) {
			res = error_errno(_("cannot open file '%s' for writing"), git_path_bisect_run());
			بشکن;
		}

		fflush(stdout);
		saved_stdout = dup(1);
		اگر (saved_stdout < 0 ||
		    dup2(temporary_stdout_fd, 1) < 0) {
			res = error_errno(_("could not duplicate stdout"));
			اگر (saved_stdout >= 0)
				close(saved_stdout);
			close(temporary_stdout_fd);
			بشکن;
		}

		res = bisect_state(terms, 1, &new_state);

		fflush(stdout);
		dup2(saved_stdout, 1);
		close(saved_stdout);
		close(temporary_stdout_fd);

		print_file_to_stdout(git_path_bisect_run());

		اگر (res == BISECT_ONLY_SKIPPED_چپ)
			error(_("bisect run cannot continue any more"));
		وگرنه اگر (res == BISECT_INTERNAL_SUCCESS_MERGE_BASE) {
			puts(_("bisect run success"));
			res = BISECT_OK;
		} وگرنه اگر (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
			printf(_("bisect found first '%s' commit\n"), terms->term_bad);
		} وگرنه اگر (res) {
			error(_("bisect run failed: 'git bisect %s'"
				" exited with error code %d"), new_state, res);
		} وگرنه {
			ادامه;
		}
		بشکن;
	}

	strbuf_release(&command);
	بازگشت res;
}

ایستا صحیح cmd_bisect__reset(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix UNUSED,
			     ساختار repository *repo UNUSED)
{
	صحیح res;

	اگر (argc > 1)
		بازگشت error(_("'%s' requires either no argument or a commit"),
			     "git bisect reset");
	res = bisect_reset(argc ? argv[0] : پوچ, false);
	اگر (res)
		بازگشت res;
	بازگشت bisect_clean_state();
}

ایستا صحیح cmd_bisect__terms(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix UNUSED,
			     ساختار repository *repo UNUSED)
{
	صحیح res;
	ساختار bisect_terms terms = { 0 };

	اگر (argc > 1)
		بازگشت error(_("'%s' requires 0 or 1 argument"),
			     "git bisect terms");
	res = bisect_terms(&terms, argc == 1 ? argv[0] : پوچ);
	free_terms(&terms);
	بازگشت res;
}

ایستا صحیح cmd_bisect__start(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix UNUSED,
			     ساختار repository *repo UNUSED)
{
	صحیح res;
	ساختار bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	res = bisect_start(&terms, argc, argv);
	free_terms(&terms);
	بازگشت res;
}

ایستا صحیح cmd_bisect__next(صحیح argc, ثابت نویسه **argv UNUSED, ثابت نویسه *prefix,
			    ساختار repository *repo UNUSED)
{
	صحیح res;
	ساختار bisect_terms terms = { 0 };

	اگر (argc)
		بازگشت error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	اگر (get_terms(&terms, 1))
		بازگشت error(_("no terms defined"));
	res = bisect_next(&terms, prefix);
	free_terms(&terms);
	بازگشت res;
}

ایستا صحیح cmd_bisect__log(صحیح argc UNUSED, ثابت نویسه **argv UNUSED,
			   ثابت نویسه *prefix UNUSED,
			   ساختار repository *repo UNUSED)
{
	بازگشت bisect_log();
}

ایستا صحیح cmd_bisect__replay(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix UNUSED,
			      ساختار repository *repo UNUSED)
{
	صحیح res;
	ساختار bisect_terms terms = { 0 };

	اگر (argc != 1)
		بازگشت error(_("no logfile given"));
	set_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	بازگشت res;
}

ایستا صحیح cmd_bisect__skip(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix UNUSED,
			    ساختار repository *repo UNUSED)
{
	صحیح res;
	ساختار bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	اگر (get_terms(&terms, 1))
		بازگشت error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	بازگشت res;
}

ایستا صحیح cmd_bisect__visualize(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix UNUSED,
				 ساختار repository *repo UNUSED)
{
	صحیح res;
	ساختار bisect_terms terms = { 0 };

	اگر (get_terms(&terms, 1))
		بازگشت error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	بازگشت res;
}

ایستا صحیح cmd_bisect__run(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix UNUSED,
			   ساختار repository *repo UNUSED)
{
	صحیح res;
	ساختار bisect_terms terms = { 0 };

	اگر (!argc)
		بازگشت error(_("'%s' failed: no command provided."), "git bisect run");
	اگر (get_terms(&terms, 1))
		بازگشت error(_("no terms defined"));
	res = bisect_run(&terms, argc, argv);
	free_terms(&terms);
	بازگشت res;
}

صحیح cmd_bisect(صحیح argc,
	       ثابت نویسه **argv,
	       ثابت نویسه *prefix,
	       ساختار repository *repo)
{
	صحیح res = 0;
	parse_opt_subcommand_fn *fn = پوچ;
	ساختار option options[] = {
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

	اگر (!fn) {
		ساختار bisect_terms terms = { 0 };

		اگر (!argc)
			usage_پیام_opt(_("need a command"), git_bisect_usage, options);

		اگر (!strcmp(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		set_terms(&terms, "bad", "good");
		اگر (get_terms(&terms, 1))
			بازگشت error(_("no terms defined"));
		اگر (check_and_set_terms(&terms, argv[0]) ||
		    !one_of(argv[0], terms.term_good, terms.term_bad, پوچ))
			usage_پیام_optf(_("unknown command: '%s'"), git_bisect_usage,
				       options, argv[0]);
		وگرنه
			res = bisect_state(&terms, argc, argv);
		free_terms(&terms);
	} وگرنه {
		argc--;
		argv++;
		res = fn(argc, argv, prefix, repo);
	}

	اگر (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		شمارش reset_when_found_mode mode;

		اگر (read_reset_when_found(&mode))
			res = BISECT_FAILED;
		وگرنه اگر (mode != RESET_WHEN_FOUND_NONE &&
			 bisect_reset_when_found(mode))
			res = BISECT_FAILED;
	}

	بازگشت is_bisect_success(res) ? 0 : -res;
}
