#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "copy.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "bisect.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "strvec.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "oid-array.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "prompt.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "revision.h"

நிலையான GIT_PATH_FUNC(git_path_bisect_terms, "BISECT_TERMS")
நிலையான GIT_PATH_FUNC(git_path_bisect_ancestors_ok, "BISECT_ANCESTORS_OK")
நிலையான GIT_PATH_FUNC(git_path_bisect_start, "BISECT_START")
நிலையான GIT_PATH_FUNC(git_path_bisect_log, "BISECT_LOG")
நிலையான GIT_PATH_FUNC(git_path_bisect_names, "BISECT_NAMES")
நிலையான GIT_PATH_FUNC(git_path_bisect_first_parent, "BISECT_FIRST_PARENT")
நிலையான GIT_PATH_FUNC(git_path_bisect_reset_when_found, "BISECT_RESET_WHEN_FOUND")
நிலையான GIT_PATH_FUNC(git_path_bisect_run, "BISECT_RUN")

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

நிலையான மாறிலி எழுத்து * மாறிலி git_bisect_usage[] = {
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
	வெற்று
};

எண்ணிக்கை reset_when_found_mode {
	RESET_WHEN_FOUND_NONE,
	RESET_WHEN_FOUND_TO_ORIGINAL,
	RESET_WHEN_FOUND_TO_FOUND,
};

கட்டமைப்பு add_bisect_ref_data {
	கட்டமைப்பு rev_info *revs;
	unsigned முழு object_flags;
};

கட்டமைப்பு bisect_terms {
	எழுத்து *term_good;
	எழுத்து *term_bad;
};

நிலையான வெறுமை free_terms(கட்டமைப்பு bisect_terms *terms)
{
	FREE_AND_NULL(terms->term_good);
	FREE_AND_NULL(terms->term_bad);
}

நிலையான வெறுமை set_terms(கட்டமைப்பு bisect_terms *terms, மாறிலி எழுத்து *bad,
		      மாறிலி எழுத்து *good)
{
	விடுவி((வெறுமை *)terms->term_good);
	terms->term_good = xstrdup(good);
	விடுவி((வெறுமை *)terms->term_bad);
	terms->term_bad = xstrdup(bad);
}

நிலையான மாறிலி எழுத்து vocab_bad[] = "bad|new";
நிலையான மாறிலி எழுத்து vocab_good[] = "good|old";

நிலையான முழு bisect_autostart(கட்டமைப்பு bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
LAST_ARG_MUST_BE_NULL
நிலையான முழு one_of(மாறிலி எழுத்து *term, ...)
{
	முழு res = 0;
	va_list matches;
	மாறிலி எழுத்து *match;

	va_start(matches, term);
	போது (!res && (match = va_arg(matches, மாறிலி எழுத்து *)))
		res = !strcmp(term, match);
	va_end(matches);

	திரும்பு res;
}

/*
 * return code BISECT_INTERNAL_SUCCESS_MERGE_BASE
 * and BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND are codes
 * that indicate special success.
 */

நிலையான முழு is_bisect_success(எண்ணிக்கை bisect_error res)
{
	திரும்பு !res ||
		res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND ||
		res == BISECT_INTERNAL_SUCCESS_MERGE_BASE;
}

நிலையான முழு write_in_file(மாறிலி எழுத்து *path, மாறிலி எழுத்து *mode, மாறிலி எழுத்து *format, va_list args)
{
	FILE *fp = வெற்று;
	முழு res = 0;

	எனில் (strcmp(mode, "w") && strcmp(mode, "a"))
		BUG("write-in-file does not support '%s' mode", mode);
	fp = fopen(path, mode);
	எனில் (!fp)
		திரும்பு error_errno(_("cannot open file '%s' in mode '%s'"), path, mode);
	res = vfprintf(fp, format, args);

	எனில் (res < 0) {
		முழு saved_errno = errno;
		fclose(fp);
		errno = saved_errno;
		திரும்பு error_errno(_("could not write to file '%s'"), path);
	}

	திரும்பு fclose(fp);
}

__attribute__((format (printf, 2, 3)))
நிலையான முழு write_to_file(மாறிலி எழுத்து *path, மாறிலி எழுத்து *format, ...)
{
	முழு res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "w", format, args);
	va_end(args);

	திரும்பு res;
}

__attribute__((format (printf, 2, 3)))
நிலையான முழு append_to_file(மாறிலி எழுத்து *path, மாறிலி எழுத்து *format, ...)
{
	முழு res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "a", format, args);
	va_end(args);

	திரும்பு res;
}

நிலையான முழு print_file_to_stdout(மாறிலி எழுத்து *path)
{
	முழு fd = open(path, O_RDONLY);
	முழு ret = 0;

	எனில் (fd < 0)
		திரும்பு error_errno(_("cannot open file '%s' for reading"), path);
	எனில் (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), path);
	close(fd);
	திரும்பு ret;
}

நிலையான முழு check_term_format(மாறிலி எழுத்து *term, மாறிலி எழுத்து *orig_term)
{
	முழு res;
	எழுத்து *new_term = xstrfmt("refs/bisect/%s", term);

	res = check_refname_format(new_term, 0);
	விடுவி(new_term);

	எனில் (res)
		திரும்பு error(_("'%s' is not a valid term"), term);

	எனில் (one_of(term, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", வெற்று))
		திரும்பு error(_("can't use the builtin command '%s' as a term"), term);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	எனில் ((strcmp(orig_term, "bad") && one_of(term, "bad", "new", வெற்று)) ||
		 (strcmp(orig_term, "good") && one_of(term, "good", "old", வெற்று)))
		திரும்பு error(_("can't change the meaning of the term '%s'"), term);

	திரும்பு 0;
}

நிலையான முழு write_terms(மாறிலி எழுத்து *bad, மாறிலி எழுத்து *good)
{
	முழு res;

	எனில் (!strcmp(bad, good))
		திரும்பு error(_("please use two different terms"));

	எனில் (check_term_format(bad, "bad") || check_term_format(good, "good"))
		திரும்பு -1;

	res = write_to_file(git_path_bisect_terms(), "%s\n%s\n", bad, good);

	திரும்பு res;
}

நிலையான முழு bisect_reset(மாறிலி எழுத்து *commit, bool quiet)
{
	கட்டமைப்பு strbuf branch = STRBUF_INIT;

	எனில் (!commit) {
		எனில் (!strbuf_read_file(&branch, git_path_bisect_start(), 0))
			printf(_("We are not bisecting.\n"));
		இல்லையெனில்
			strbuf_rtrim(&branch);
	} இல்லையெனில் {
		கட்டமைப்பு object_id oid;

		எனில் (repo_get_oid_commit(the_repository, commit, &oid))
			திரும்பு error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	எனில் (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "checkout", "--ignore-other-worktrees", வெற்று);
		எனில் (quiet)
			strvec_தள்ளு(&cmd.args, "--quiet");
		strvec_pushl(&cmd.args, branch.buf, "--", வெற்று);
		எனில் (run_command(&cmd)) {
			error(_("could not check out original"
				" HEAD '%s'. Try 'git bisect"
				" reset <commit>'."), branch.buf);
			strbuf_release(&branch);
			திரும்பு -1;
		}
	}

	strbuf_release(&branch);
	திரும்பு 0;
}

நிலையான முழு parse_reset_when_found(மாறிலி எழுத்து *மதிப்பு,
				  எண்ணிக்கை reset_when_found_mode *mode)
{
	எனில் (!strcmp(மதிப்பு, "original"))
		*mode = RESET_WHEN_FOUND_TO_ORIGINAL;
	இல்லையெனில் எனில் (!strcmp(மதிப்பு, "found"))
		*mode = RESET_WHEN_FOUND_TO_FOUND;
	இல்லையெனில்
		திரும்பு error(_("invalid value for '--reset-when-found': '%s'"),
			     மதிப்பு);

	திரும்பு 0;
}

நிலையான மாறிலி எழுத்து *reset_when_found_mode_name(எண்ணிக்கை reset_when_found_mode mode)
{
	தேர்வு (mode) {
	நிலை RESET_WHEN_FOUND_TO_ORIGINAL:
		திரும்பு "original";
	நிலை RESET_WHEN_FOUND_TO_FOUND:
		திரும்பு "found";
	நிலை RESET_WHEN_FOUND_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", mode);
}

நிலையான முழு read_reset_when_found(எண்ணிக்கை reset_when_found_mode *mode)
{
	கட்டமைப்பு strbuf மதிப்பு = STRBUF_INIT;
	முழு res = 0;

	*mode = RESET_WHEN_FOUND_NONE;
	எனில் (is_empty_or_missing_file(git_path_bisect_reset_when_found()))
		திரும்பு 0;

	எனில் (strbuf_read_file(&மதிப்பு, git_path_bisect_reset_when_found(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_path_bisect_reset_when_found());
		செல் out;
	}
	strbuf_trim(&மதிப்பு);
	எனில் (parse_reset_when_found(மதிப்பு.buf, mode))
		res = -1;

out:
	strbuf_release(&மதிப்பு);
	திரும்பு res;
}

நிலையான முழு bisect_reset_when_found(எண்ணிக்கை reset_when_found_mode mode)
{
	கட்டமைப்பு bisect_terms terms = { 0 };
	எழுத்து *commit = வெற்று;
	முழு res;

	எனில் (mode == RESET_WHEN_FOUND_TO_FOUND) {
		read_bisect_terms(&terms.term_bad, &terms.term_good);
		commit = xstrfmt("refs/bisect/%s", terms.term_bad);
	} இல்லையெனில் எனில் (mode == RESET_WHEN_FOUND_NONE) {
		BUG("automatic reset requested without a reset mode");
	}

	res = bisect_reset(commit, true);
	எனில் (!res)
		res = bisect_clean_state();

	விடுவி(commit);
	free_terms(&terms);
	திரும்பு res;
}

நிலையான வெறுமை log_commit(FILE *fp,
		       மாறிலி எழுத்து *fmt, மாறிலி எழுத்து *state,
		       கட்டமைப்பு commit *commit)
{
	கட்டமைப்பு pretty_print_context pp = {0};
	கட்டமைப்பு strbuf commit_செய்தி = STRBUF_INIT;
	எழுத்து *label = xstrfmt(fmt, state);

	repo_format_commit_message(the_repository, commit, "%s", &commit_செய்தி,
				   &pp);

	fprintf(fp, "# %s: [%s] %s\n", label, oid_to_hex(&commit->object.oid),
		commit_செய்தி.buf);

	strbuf_release(&commit_செய்தி);
	விடுவி(label);
}

நிலையான முழு bisect_write(மாறிலி எழுத்து *state, மாறிலி எழுத்து *rev,
			மாறிலி கட்டமைப்பு bisect_terms *terms, முழு nolog)
{
	கட்டமைப்பு strbuf tag = STRBUF_INIT;
	கட்டமைப்பு object_id oid;
	கட்டமைப்பு commit *commit;
	FILE *fp = வெற்று;
	முழு res = 0;

	எனில் (!strcmp(state, terms->term_bad)) {
		strbuf_addf(&tag, "refs/bisect/%s", state);
	} இல்லையெனில் எனில் (one_of(state, terms->term_good, "skip", வெற்று)) {
		strbuf_addf(&tag, "refs/bisect/%s-%s", state, rev);
	} இல்லையெனில் {
		res = error(_("Bad bisect_write argument: %s"), state);
		செல் finish;
	}

	எனில் (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		செல் finish;
	}

	எனில் (refs_update_ref(get_main_ref_store(the_repository), வெற்று, tag.buf, &oid, வெற்று, 0,
			    UPDATE_REFS_செய்தி_ON_ERR)) {
		res = -1;
		செல் finish;
	}

	fp = fopen(git_path_bisect_log(), "a");
	எனில் (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_path_bisect_log());
		செல் finish;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	எனில் (!nolog)
		fprintf(fp, "git bisect %s %s\n", state, rev);

finish:
	எனில் (fp)
		fclose(fp);
	strbuf_release(&tag);
	திரும்பு res;
}

நிலையான முழு check_and_set_terms(கட்டமைப்பு bisect_terms *terms, மாறிலி எழுத்து *cmd)
{
	முழு has_term_file = !is_empty_or_missing_file(git_path_bisect_terms());

	எனில் (one_of(cmd, "skip", "start", "terms", வெற்று))
		திரும்பு 0;

	எனில் (has_term_file && strcmp(cmd, terms->term_bad) &&
	    strcmp(cmd, terms->term_good))
		திரும்பு error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->term_bad,
				terms->term_good);

	எனில் (!has_term_file) {
		எனில் (one_of(cmd, "bad", "good", வெற்று)) {
			set_terms(terms, "bad", "good");
			திரும்பு write_terms(terms->term_bad, terms->term_good);
		}
		எனில் (one_of(cmd, "new", "old", வெற்று)) {
			set_terms(terms, "new", "old");
			திரும்பு write_terms(terms->term_bad, terms->term_good);
		}
	}

	திரும்பு 0;
}

நிலையான முழு inc_nr(மாறிலி கட்டமைப்பு reference *ref UNUSED, வெறுமை *cb_data)
{
	unsigned முழு *nr = (unsigned முழு *)cb_data;
	(*nr)++;
	திரும்பு 0;
}

நிலையான மாறிலி எழுத்து need_bad_and_good_revision_warning[] =
	N_("You need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

நிலையான மாறிலி எழுத்து need_bisect_start_warning[] =
	N_("You need to start by \"git bisect start\".\n"
	   "You then need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

நிலையான முழு decide_next(மாறிலி கட்டமைப்பு bisect_terms *terms,
		       மாறிலி எழுத்து *current_term, முழு missing_good,
		       முழு missing_bad)
{
	எனில் (!missing_good && !missing_bad)
		திரும்பு 0;
	எனில் (!current_term)
		திரும்பு -1;

	எனில் (missing_good && !missing_bad &&
	    !strcmp(current_term, terms->term_good)) {
		எழுத்து *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->term_bad);
		எனில் (!isatty(0))
			திரும்பு 0;
		/*
		 * TRANSLATORS: Make sure to include [Y] and [n] in your
		 * translation. The program will only accept English input
		 * at this point.
		 */
		yesno = git_prompt(_("Are you sure [Y/n]? "), PROMPT_ECHO);
		எனில் (starts_with(yesno, "N") || starts_with(yesno, "n"))
			திரும்பு -1;
		திரும்பு 0;
	}

	எனில் (!is_empty_or_missing_file(git_path_bisect_start()))
		திரும்பு error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	இல்லையெனில்
		திரும்பு error(_(need_bisect_start_warning),
			     vocab_good, vocab_bad, vocab_good, vocab_bad);
}

நிலையான வெறுமை bisect_status(கட்டமைப்பு bisect_state *state,
			  மாறிலி கட்டமைப்பு bisect_terms *terms)
{
	எழுத்து *bad_ref = xstrfmt("refs/bisect/%s", terms->term_bad);
	எழுத்து *good_glob = xstrfmt("%s-*", terms->term_good);
	கட்டமைப்பு refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	எனில் (refs_ref_exists(get_main_ref_store(the_repository), bad_ref))
		state->nr_bad = 1;

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      inc_nr, &state->nr_good, &opts);

	விடுவி(good_glob);
	விடுவி(bad_ref);
}

__attribute__((format (printf, 1, 2)))
நிலையான வெறுமை bisect_log_printf(மாறிலி எழுத்து *fmt, ...)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	va_list ap;

	va_start(ap, fmt);
	strbuf_vaddf(&buf, fmt, ap);
	va_end(ap);

	printf("%s", buf.buf);
	append_to_file(git_path_bisect_log(), "# %s", buf.buf);

	strbuf_release(&buf);
}

நிலையான வெறுமை bisect_print_status(மாறிலி கட்டமைப்பு bisect_terms *terms)
{
	கட்டமைப்பு bisect_state state = { 0 };

	bisect_status(&state, terms);

	/* If we had both, we'd already be started, and shouldn't get here. */
	எனில் (state.nr_good && state.nr_bad)
		திரும்பு;

	எனில் (!state.nr_good && !state.nr_bad)
		bisect_log_printf(_("status: waiting for both '%s' and '%s' commits\n"),
				  terms->term_good, terms->term_bad);
	இல்லையெனில் எனில் (state.nr_good)
		bisect_log_printf(Q_("status: waiting for '%s' commit, %d '%s' commit known\n",
				     "status: waiting for '%s' commit, %d '%s' commits known\n",
				     state.nr_good),
				  terms->term_bad, state.nr_good, terms->term_good);
	இல்லையெனில்
		bisect_log_printf(_("status: waiting for '%s' commit(s), '%s' commit known\n"),
				  terms->term_good, terms->term_bad);
}

நிலையான முழு bisect_next_check(மாறிலி கட்டமைப்பு bisect_terms *terms,
			     மாறிலி எழுத்து *current_term)
{
	கட்டமைப்பு bisect_state state = { 0 };
	bisect_status(&state, terms);
	திரும்பு decide_next(terms, current_term, !state.nr_good, !state.nr_bad);
}

நிலையான முழு get_terms(கட்டமைப்பு bisect_terms *terms, முழு file_missing_is_ok)
{
	கட்டமைப்பு strbuf str = STRBUF_INIT;
	FILE *fp = வெற்று;
	முழு res = 0;

	fp = fopen(git_path_bisect_terms(), "r");
	எனில் (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		செல் finish;
	}

	free_terms(terms);
	எனில் (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		செல் finish;
	}
	terms->term_bad = strbuf_detach(&str, வெற்று);
	எனில் (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		FREE_AND_NULL(terms->term_bad);
		செல் finish;
	}
	terms->term_good = strbuf_detach(&str, வெற்று);

finish:
	எனில் (fp)
		fclose(fp);
	strbuf_release(&str);
	திரும்பு res;
}

நிலையான முழு bisect_terms(கட்டமைப்பு bisect_terms *terms, மாறிலி எழுத்து *option)
{
	எனில் (get_terms(terms, 0))
		திரும்பு error(_("no terms defined"));

	எனில் (!option) {
		printf(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->term_good, terms->term_bad);
		திரும்பு 0;
	}
	எனில் (one_of(option, "--term-good", "--term-old", வெற்று))
		printf("%s\n", terms->term_good);
	இல்லையெனில் எனில் (one_of(option, "--term-bad", "--term-new", வெற்று))
		printf("%s\n", terms->term_bad);
	இல்லையெனில்
		திரும்பு error(_("invalid argument %s for 'git bisect terms'.\n"
			       "Supported options are: "
			       "--term-good|--term-old and "
			       "--term-bad|--term-new."), option);

	திரும்பு 0;
}

நிலையான முழு bisect_append_log_quoted(மாறிலி எழுத்து **argv)
{
	முழு res = 0;
	FILE *fp = fopen(git_path_bisect_log(), "a");
	கட்டமைப்பு strbuf orig_args = STRBUF_INIT;

	எனில் (!fp)
		திரும்பு -1;

	எனில் (fprintf(fp, "git bisect start") < 1) {
		res = -1;
		செல் finish;
	}

	sq_quote_argv(&orig_args, argv);
	எனில் (fprintf(fp, "%s\n", orig_args.buf) < 1)
		res = -1;

finish:
	fclose(fp);
	strbuf_release(&orig_args);
	திரும்பு res;
}

நிலையான முழு add_bisect_ref(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cb)
{
	கட்டமைப்பு add_bisect_ref_data *data = cb;

	add_pending_oid(data->revs, ref->name, ref->oid, data->object_flags);

	திரும்பு 0;
}

நிலையான முழு prepare_revs(கட்டமைப்பு bisect_terms *terms, கட்டமைப்பு rev_info *revs)
{
	கட்டமைப்பு refs_for_each_ref_options opts = {
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};
	முழு res = 0;
	கட்டமைப்பு add_bisect_ref_data cb = { revs };
	எழுத்து *good = xstrfmt("%s-*", terms->term_good);

	/*
	 * We cannot use terms->term_bad directly in
	 * for_each_glob_ref_in() and we have to append a '*' to it,
	 * otherwise for_each_glob_ref_in() will append '/' and '*'.
	 */
	எழுத்து *bad = xstrfmt("%s*", terms->term_bad);

	/*
	 * It is important to reset the flags used by revision walks
	 * as the previous call to bisect_next_all() in turn
	 * sets up a revision walk.
	 */
	reset_revision_walk();
	repo_init_revisions(the_repository, revs, வெற்று);
	setup_revisions(0, வெற்று, revs, வெற்று);

	opts.pattern = bad;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      add_bisect_ref, &cb, &opts);

	cb.object_flags = UNINTERESTING;
	opts.pattern = good;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      add_bisect_ref, &cb, &opts);

	எனில் (prepare_revision_walk(revs))
		res = error(_("revision walk setup failed"));

	விடுவி(good);
	விடுவி(bad);
	திரும்பு res;
}

நிலையான முழு bisect_skipped_commits(கட்டமைப்பு bisect_terms *terms)
{
	முழு res;
	FILE *fp = வெற்று;
	கட்டமைப்பு rev_info revs;
	கட்டமைப்பு commit *commit;
	கட்டமைப்பு pretty_print_context pp = {0};
	கட்டமைப்பு strbuf commit_name = STRBUF_INIT;

	res = prepare_revs(terms, &revs);
	எனில் (res)
		திரும்பு res;

	fp = fopen(git_path_bisect_log(), "a");
	எனில் (!fp)
		திரும்பு error_errno(_("could not open '%s' for appending"),
				  git_path_bisect_log());

	எனில் (fprintf(fp, "# only skipped commits left to test\n") < 0)
		திரும்பு error_errno(_("failed to write to '%s'"), git_path_bisect_log());

	போது ((commit = get_revision(&revs)) != வெற்று) {
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
	திரும்பு 0;
}

நிலையான முழு bisect_successful(கட்டமைப்பு bisect_terms *terms)
{
	கட்டமைப்பு object_id oid;
	கட்டமைப்பு commit *commit;
	கட்டமைப்பு pretty_print_context pp = {0};
	கட்டமைப்பு strbuf commit_name = STRBUF_INIT;
	எழுத்து *bad_ref = xstrfmt("refs/bisect/%s",terms->term_bad);
	முழு res;

	refs_read_ref(get_main_ref_store(the_repository), bad_ref, &oid);
	commit = lookup_commit_reference_by_name(bad_ref);
	எனில் (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		விடுவி(bad_ref);
		திரும்பு BISECT_FAILED;
	}
	repo_format_commit_message(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_to_file(git_path_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->term_bad, oid_to_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	விடுவி(bad_ref);
	திரும்பு res;
}

நிலையான எண்ணிக்கை bisect_error bisect_next(கட்டமைப்பு bisect_terms *terms,
				     மாறிலி எழுத்து *prefix)
{
	எண்ணிக்கை bisect_error res;

	எனில் (bisect_autostart(terms))
		திரும்பு BISECT_FAILED;

	எனில் (bisect_next_check(terms, terms->term_good))
		திரும்பு BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_next_all(the_repository, prefix);

	எனில் (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		res = bisect_successful(terms);
		திரும்பு res ? res : BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND;
	} இல்லையெனில் எனில் (res == BISECT_ONLY_SKIPPED_இடது) {
		res = bisect_skipped_commits(terms);
		திரும்பு res ? res : BISECT_ONLY_SKIPPED_இடது;
	}
	திரும்பு res;
}

நிலையான எண்ணிக்கை bisect_error bisect_auto_next(கட்டமைப்பு bisect_terms *terms,
					  மாறிலி எழுத்து *prefix)
{
	எனில் (bisect_next_check(terms, வெற்று)) {
		bisect_print_status(terms);
		திரும்பு BISECT_OK;
	}

	திரும்பு bisect_next(terms, prefix);
}

நிலையான எண்ணிக்கை bisect_error bisect_start(கட்டமைப்பு bisect_terms *terms, முழு argc,
				      மாறிலி எழுத்து **argv)
{
	முழு no_checkout = 0;
	முழு first_parent_only = 0;
	முழு i, has_double_dash = 0, must_write_terms = 0, bad_seen = 0;
	முழு flags, pathspec_pos;
	எண்ணிக்கை bisect_error res = BISECT_OK;
	கட்டமைப்பு string_list revs = STRING_LIST_INIT_DUP;
	கட்டமைப்பு string_list states = STRING_LIST_INIT_DUP;
	கட்டமைப்பு strbuf start_head = STRBUF_INIT;
	கட்டமைப்பு strbuf bisect_names = STRBUF_INIT;
	கட்டமைப்பு object_id head_oid;
	கட்டமைப்பு object_id oid;
	எண்ணிக்கை reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;
	மாறிலி எழுத்து *head;

	எனில் (is_bare_repository(the_repository))
		no_checkout = 1;

	/*
	 * Check for one bad and then some good revisions
	 */
	க்கு (i = 0; i < argc; i++) {
		எனில் (!strcmp(argv[i], "--")) {
			has_double_dash = 1;
			நிறுத்து;
		}
	}

	க்கு (i = 0; i < argc; i++) {
		மாறிலி எழுத்து *arg = argv[i];
		எனில் (!strcmp(argv[i], "--")) {
			நிறுத்து;
		} இல்லையெனில் எனில் (!strcmp(arg, "--no-checkout")) {
			no_checkout = 1;
		} இல்லையெனில் எனில் (!strcmp(arg, "--first-parent")) {
			first_parent_only = 1;
		} இல்லையெனில் எனில் (!strcmp(arg, "--reset-when-found")) {
			reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
		} இல்லையெனில் எனில் (skip_prefix(arg, "--reset-when-found=", &arg)) {
			எனில் (parse_reset_when_found(arg, &reset_when_found)) {
				res = BISECT_FAILED;
				செல் finish;
			}
		} இல்லையெனில் எனில் (!strcmp(arg, "--term-good") ||
			 !strcmp(arg, "--term-old")) {
			i++;
			எனில் (argc <= i)
				திரும்பு error(_("'' is not a valid term"));
			must_write_terms = 1;
			விடுவி((வெறுமை *) terms->term_good);
			terms->term_good = xstrdup(argv[i]);
		} இல்லையெனில் எனில் (skip_prefix(arg, "--term-good=", &arg) ||
			   skip_prefix(arg, "--term-old=", &arg)) {
			must_write_terms = 1;
			விடுவி((வெறுமை *) terms->term_good);
			terms->term_good = xstrdup(arg);
		} இல்லையெனில் எனில் (!strcmp(arg, "--term-bad") ||
			 !strcmp(arg, "--term-new")) {
			i++;
			எனில் (argc <= i)
				திரும்பு error(_("'' is not a valid term"));
			must_write_terms = 1;
			விடுவி((வெறுமை *) terms->term_bad);
			terms->term_bad = xstrdup(argv[i]);
		} இல்லையெனில் எனில் (skip_prefix(arg, "--term-bad=", &arg) ||
			   skip_prefix(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			விடுவி((வெறுமை *) terms->term_bad);
			terms->term_bad = xstrdup(arg);
		} இல்லையெனில் எனில் (starts_with(arg, "--")) {
			திரும்பு error(_("unrecognized option: '%s'"), arg);
		} இல்லையெனில் எனில் (!get_oidf(&oid, "%s^{commit}", arg)) {
			string_list_append(&revs, oid_to_hex(&oid));
		} இல்லையெனில் எனில் (has_double_dash) {
			die(_("'%s' does not appear to be a valid "
			      "revision"), arg);
		} இல்லையெனில் {
			நிறுத்து;
		}
	}
	எனில் (reset_when_found != RESET_WHEN_FOUND_NONE && no_checkout) {
		res = error(_("options '%s' and '%s' cannot be used together"),
			    "--reset-when-found", "--no-checkout");
		செல் finish;
	}
	pathspec_pos = i;

	/*
	 * The user ran "git bisect start <sha1> <sha1>", hence did not
	 * explicitly specify the terms, but we are already starting to
	 * set references named with the default terms, and won't be able
	 * to change afterwards.
	 */
	எனில் (revs.nr)
		must_write_terms = 1;
	க்கு (i = 0; i < revs.nr; i++) {
		எனில் (bad_seen) {
			string_list_append(&states, terms->term_good);
		} இல்லையெனில் {
			bad_seen = 1;
			string_list_append(&states, terms->term_bad);
		}
	}

	/*
	 * Verify HEAD
	 */
	head = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
				       "HEAD", 0, &head_oid, &flags);
	எனில் (!head) {
		எனில் (repo_get_oid(the_repository, "HEAD", &head_oid))
			திரும்பு error(_("bad HEAD - I need a HEAD"));
		head = "HEAD";
	}

	/*
	 * Check if we are bisecting
	 */
	எனில் (!is_empty_or_missing_file(git_path_bisect_start())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&start_head, git_path_bisect_start(), 0);
		strbuf_trim(&start_head);
		எனில் (!no_checkout) {
			கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.args, "checkout", start_head.buf,
				     "--", வெற்று);
			எனில் (run_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       start_head.buf);
				செல் finish;
			}
		}
	} இல்லையெனில் {
		/* Get the rev from where we start. */
		எனில் (!repo_get_oid(the_repository, head, &head_oid) &&
		    !starts_with(head, "refs/heads/")) {
			strbuf_reset(&start_head);
			strbuf_add_oid_hex(&start_head, &head_oid);
		} இல்லையெனில் எனில் (!repo_get_oid(the_repository, head, &head_oid) &&
			   skip_prefix(head, "refs/heads/", &head)) {
			strbuf_addstr(&start_head, head);
		} இல்லையெனில் {
			திரும்பு error(_("bad HEAD - strange symbolic ref"));
		}
	}

	/*
	 * Get rid of any old bisect state.
	 */
	எனில் (bisect_clean_state())
		திரும்பு BISECT_FAILED;

	/*
	 * Write new start state
	 */
	write_file(git_path_bisect_start(), "%s\n", start_head.buf);

	எனில் (first_parent_only)
		write_file(git_path_bisect_first_parent(), "\n");

	எனில் (reset_when_found != RESET_WHEN_FOUND_NONE)
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));

	எனில் (no_checkout) {
		எனில் (repo_get_oid(the_repository, start_head.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), start_head.buf);
			செல் finish;
		}
		எனில் (refs_update_ref(get_main_ref_store(the_repository), வெற்று, "BISECT_HEAD", &oid, வெற்று, 0,
				    UPDATE_REFS_செய்தி_ON_ERR)) {
			res = BISECT_FAILED;
			செல் finish;
		}
	}

	எனில் (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_names, argv + pathspec_pos);
	write_file(git_path_bisect_names(), "%s\n", bisect_names.buf);

	க்கு (i = 0; i < states.nr; i++)
		எனில் (bisect_write(states.பொருட்கள்[i].string,
				 revs.பொருட்கள்[i].string, terms, 1)) {
			res = BISECT_FAILED;
			செல் finish;
		}

	எனில் (must_write_terms && write_terms(terms->term_bad,
					    terms->term_good)) {
		res = BISECT_FAILED;
		செல் finish;
	}

	res = bisect_append_log_quoted(argv);
	எனில் (res)
		res = BISECT_FAILED;

finish:
	string_list_clear(&revs, 0);
	string_list_clear(&states, 0);
	strbuf_release(&start_head);
	strbuf_release(&bisect_names);
	எனில் (res)
		திரும்பு res;

	res = bisect_auto_next(terms, வெற்று);
	எனில் (!is_bisect_success(res))
		bisect_clean_state();
	திரும்பு res;
}

நிலையான உள்வரி முழு file_is_not_empty(மாறிலி எழுத்து *path)
{
	திரும்பு !is_empty_or_missing_file(path);
}

நிலையான முழு bisect_autostart(கட்டமைப்பு bisect_terms *terms)
{
	முழு res;
	மாறிலி எழுத்து *yesno;

	எனில் (file_is_not_empty(git_path_bisect_start()))
		திரும்பு 0;

	fprintf_ln(stderr, _("You need to start by \"git bisect "
			  "start\"\n"));

	எனில் (!isatty(STDIN_FILENO))
		திரும்பு -1;

	/*
	 * TRANSLATORS: Make sure to include [Y] and [n] in your
	 * translation. The program will only accept English input
	 * at this point.
	 */
	yesno = git_prompt(_("Do you want me to do it for you "
			     "[Y/n]? "), PROMPT_ECHO);
	res = tolower(*yesno) == 'n' ?
		-1 : bisect_start(terms, 0, empty_strvec);

	திரும்பு res;
}

நிலையான எண்ணிக்கை bisect_error bisect_state(கட்டமைப்பு bisect_terms *terms, முழு argc,
				      மாறிலி எழுத்து **argv)
{
	மாறிலி எழுத்து *state;
	முழு i, verify_expected = 1;
	கட்டமைப்பு object_id oid, expected;
	கட்டமைப்பு oid_array revs = OID_ARRAY_INIT;

	எனில் (!argc)
		திரும்பு error(_("Please call `--bisect-state` with at least one argument"));

	எனில் (bisect_autostart(terms))
		திரும்பு BISECT_FAILED;

	state = argv[0];
	எனில் (check_and_set_terms(terms, state) ||
	    !one_of(state, terms->term_good, terms->term_bad, "skip", வெற்று))
		திரும்பு BISECT_FAILED;

	argv++;
	argc--;
	எனில் (argc > 1 && !strcmp(state, terms->term_bad))
		திரும்பு error(_("'git bisect %s' can take only one argument."), terms->term_bad);

	எனில் (argc == 0) {
		மாறிலி எழுத்து *head = "BISECT_HEAD";
		எண்ணிக்கை get_oid_result res_head = repo_get_oid(the_repository,
							    head, &oid);

		எனில் (res_head == MISSING_OBJECT) {
			head = "HEAD";
			res_head = repo_get_oid(the_repository, head, &oid);
		}

		எனில் (res_head)
			error(_("Bad rev input: %s"), head);
		oid_array_append(&revs, &oid);
	}

	/*
	 * All input revs must be checked before executing bisect_write()
	 * to discard junk revs.
	 */

	க்கு (; argc; argc--, argv++) {
		கட்டமைப்பு commit *commit;

		எனில் (repo_get_oid(the_repository, *argv, &oid)){
			error(_("Bad rev input: %s"), *argv);
			oid_array_clear(&revs);
			திரும்பு BISECT_FAILED;
		}

		commit = lookup_commit_reference(the_repository, &oid);
		எனில் (!commit)
			die(_("Bad rev input (not a commit): %s"), *argv);

		oid_array_append(&revs, &commit->object.oid);
	}

	எனில் (refs_read_ref(get_main_ref_store(the_repository), "BISECT_EXPECTED_REV", &expected))
		verify_expected = 0; /* Ignore invalid file contents */

	க்கு (i = 0; i < revs.nr; i++) {
		எனில் (bisect_write(state, oid_to_hex(&revs.oid[i]), terms, 0)) {
			oid_array_clear(&revs);
			திரும்பு BISECT_FAILED;
		}
		எனில் (verify_expected && !oideq(&revs.oid[i], &expected)) {
			unlink_or_warn(git_path_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					வெற்று, "BISECT_EXPECTED_REV", வெற்று,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	திரும்பு bisect_auto_next(terms, வெற்று);
}

நிலையான எண்ணிக்கை bisect_error bisect_log(வெறுமை)
{
	முழு fd, status;
	மாறிலி எழுத்து* filename = git_path_bisect_log();

	எனில் (is_empty_or_missing_file(filename))
		திரும்பு error(_("We are not bisecting."));

	fd = open(filename, O_RDONLY);
	எனில் (fd < 0)
		திரும்பு BISECT_FAILED;

	status = copy_fd(fd, STDOUT_FILENO);
	close(fd);
	திரும்பு status ? BISECT_FAILED : BISECT_OK;
}

நிலையான முழு process_replay_வரி(கட்டமைப்பு bisect_terms *terms, கட்டமைப்பு strbuf *வரி)
{
	மாறிலி எழுத்து *p = வரி->buf + strspn(வரி->buf, " \t");
	எழுத்து *word_end, *rev;

	எனில் ((!skip_prefix(p, "git bisect", &p) &&
	!skip_prefix(p, "git-bisect", &p)) || !isspace(*p))
		திரும்பு 0;
	p += strspn(p, " \t");

	word_end = (எழுத்து *)p + strcspn(p, " \t");
	rev = word_end + strspn(word_end, " \t");
	*word_end = '\0'; /* NUL-terminate the word */

	எனில் (get_terms(terms, 1))
		திரும்பு error(_("no terms defined"));
	எனில் (check_and_set_terms(terms, p))
		திரும்பு -1;

	எனில் (!strcmp(p, "start")) {
		கட்டமைப்பு strvec argv = STRVEC_INIT;
		முழு res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_start(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		திரும்பு res;
	}

	எனில் (one_of(p, terms->term_good,
	   terms->term_bad, "skip", வெற்று))
		திரும்பு bisect_write(p, rev, terms, 0);

	எனில் (!strcmp(p, "terms")) {
		கட்டமைப்பு strvec argv = STRVEC_INIT;
		முழு res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_terms(terms, argv.nr == 1 ? argv.v[0] : வெற்று);
		strvec_clear(&argv);
		திரும்பு res;
	}
	error(_("'%s'?? what are you talking about?"), p);

	திரும்பு -1;
}

நிலையான எண்ணிக்கை bisect_error bisect_replay(கட்டமைப்பு bisect_terms *terms, மாறிலி எழுத்து *filename)
{
	FILE *fp = வெற்று;
	எண்ணிக்கை bisect_error res = BISECT_OK;
	கட்டமைப்பு strbuf வரி = STRBUF_INIT;

	எனில் (is_empty_or_missing_file(filename))
		திரும்பு error(_("cannot read file '%s' for replaying"), filename);

	எனில் (bisect_clean_state())
		திரும்பு BISECT_FAILED;

	fp = fopen(filename, "r");
	எனில் (!fp)
		திரும்பு BISECT_FAILED;

	போது ((strbuf_getline(&வரி, fp) != EOF) && !res)
		res = process_replay_வரி(terms, &வரி);

	strbuf_release(&வரி);
	fclose(fp);

	எனில் (res)
		திரும்பு BISECT_FAILED;

	திரும்பு bisect_auto_next(terms, வெற்று);
}

நிலையான எண்ணிக்கை bisect_error bisect_skip(கட்டமைப்பு bisect_terms *terms, முழு argc,
				     மாறிலி எழுத்து **argv)
{
	முழு i;
	எண்ணிக்கை bisect_error res;
	கட்டமைப்பு strvec argv_state = STRVEC_INIT;

	strvec_தள்ளு(&argv_state, "skip");

	க்கு (i = 0; i < argc; i++) {
		மாறிலி எழுத்து *dotdot = strstr(argv[i], "..");

		எனில் (dotdot) {
			கட்டமைப்பு rev_info revs;
			கட்டமைப்பு commit *commit;

			repo_init_revisions(the_repository, &revs, வெற்று);
			setup_revisions(2, argv + i - 1, &revs, வெற்று);

			எனில் (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			போது ((commit = get_revision(&revs)) != வெற்று)
				strvec_தள்ளு(&argv_state,
						oid_to_hex(&commit->object.oid));

			reset_revision_walk();
			release_revisions(&revs);
		} இல்லையெனில் {
			strvec_தள்ளு(&argv_state, argv[i]);
		}
	}
	res = bisect_state(terms, argv_state.nr, argv_state.v);

	strvec_clear(&argv_state);
	திரும்பு res;
}

நிலையான முழு bisect_visualize(கட்டமைப்பு bisect_terms *terms, முழு argc,
			    மாறிலி எழுத்து **argv)
{
	கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;
	கட்டமைப்பு strbuf sb = STRBUF_INIT;

	எனில் (bisect_next_check(terms, வெற்று) != 0)
		திரும்பு BISECT_FAILED;

	cmd.no_stdin = 1;
	எனில் (!argc) {
		எனில் ((getenv("DISPLAY") || getenv("SESSIONNAME") || getenv("MSYSTEM") ||
		     getenv("SECURITYSESSIONID")) && exists_in_PATH("gitk")) {
			strvec_தள்ளு(&cmd.args, "gitk");
		} இல்லையெனில் {
			strvec_தள்ளு(&cmd.args, "log");
			cmd.git_cmd = 1;
		}
	} இல்லையெனில் {
		எனில் (argv[0][0] == '-') {
			strvec_தள்ளு(&cmd.args, "log");
			cmd.git_cmd = 1;
		} இல்லையெனில் எனில் (strcmp(argv[0], "tig") && !starts_with(argv[0], "git"))
			cmd.git_cmd = 1;

		strvec_pushv(&cmd.args, argv);
	}

	strvec_pushl(&cmd.args, "--bisect", "--", வெற்று);

	strbuf_read_file(&sb, git_path_bisect_names(), 0);
	sq_dequote_to_strvec(sb.buf, &cmd.args);
	strbuf_release(&sb);

	திரும்பு run_command(&cmd);
}

நிலையான முழு get_first_good(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cb_data)
{
	oidcpy(cb_data, ref->oid);
	திரும்பு 1;
}

நிலையான முழு do_bisect_run(மாறிலி எழுத்து *command)
{
	கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;

	printf(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_தள்ளு(&cmd.args, command);
	திரும்பு run_command(&cmd);
}

நிலையான முழு verify_good(மாறிலி கட்டமைப்பு bisect_terms *terms, மாறிலி எழுத்து *command)
{
	முழு rc;
	எண்ணிக்கை bisect_error res;
	கட்டமைப்பு object_id good_rev;
	கட்டமைப்பு object_id current_rev;
	எழுத்து *good_glob = xstrfmt("%s-*", terms->term_good);
	முழு no_checkout = refs_ref_exists(get_main_ref_store(the_repository),
					  "BISECT_HEAD");
	கட்டமைப்பு refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_first_good, &good_rev, &opts);
	விடுவி(good_glob);

	எனில் (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		திரும்பு -1;

	res = bisect_checkout(&good_rev, no_checkout);
	எனில் (res != BISECT_OK)
		திரும்பு -1;

	rc = do_bisect_run(command);

	res = bisect_checkout(&current_rev, no_checkout);
	எனில் (res != BISECT_OK)
		திரும்பு -1;

	திரும்பு rc;
}

நிலையான முழு bisect_run(கட்டமைப்பு bisect_terms *terms, முழு argc, மாறிலி எழுத்து **argv)
{
	முழு res = BISECT_OK;
	கட்டமைப்பு strbuf command = STRBUF_INIT;
	மாறிலி எழுத்து *reset_when_found_arg;
	மாறிலி எழுத்து *new_state;
	முழு temporary_stdout_fd, saved_stdout;
	முழு is_first_run = 1;
	எண்ணிக்கை reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;

	எனில் (bisect_next_check(terms, வெற்று))
		திரும்பு BISECT_FAILED;

	எனில் (argc && !strcmp(argv[0], "--reset-when-found")) {
		reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
	} இல்லையெனில் எனில் (argc && skip_prefix(argv[0], "--reset-when-found=",
				    &reset_when_found_arg)) {
		எனில் (parse_reset_when_found(reset_when_found_arg,
					   &reset_when_found))
			திரும்பு BISECT_FAILED;
	}

	எனில் (reset_when_found != RESET_WHEN_FOUND_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		திரும்பு error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	எனில் (reset_when_found != RESET_WHEN_FOUND_NONE) {
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));
		argc--;
		argv++;
	}

	எனில் (!argc) {
		error(_("bisect run failed: no command provided."));
		திரும்பு BISECT_FAILED;
	}

	sq_quote_argv(&command, argv);
	strbuf_ltrim(&command);
	போது (1) {
		res = do_bisect_run(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		எனில் (is_first_run && (res == 126 || res == 127)) {
			முழு rc = verify_good(terms, command.buf);
			is_first_run = 0;
			எனில் (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->term_good);
				res = BISECT_FAILED;
				நிறுத்து;
			}
			எனில் (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->term_good);
				res = BISECT_FAILED;
				நிறுத்து;
			}
		}

		எனில் (res < 0 || 128 <= res) {
			error(_("bisect run failed: exit code %d from"
				" %s is < 0 or >= 128"), res, command.buf);
			நிறுத்து;
		}

		எனில் (res == 125)
			new_state = "skip";
		இல்லையெனில் எனில் (!res)
			new_state = terms->term_good;
		இல்லையெனில்
			new_state = terms->term_bad;

		temporary_stdout_fd = open(git_path_bisect_run(), O_CREAT | O_WRONLY | O_TRUNC, 0666);

		எனில் (temporary_stdout_fd < 0) {
			res = error_errno(_("cannot open file '%s' for writing"), git_path_bisect_run());
			நிறுத்து;
		}

		fflush(stdout);
		saved_stdout = dup(1);
		எனில் (saved_stdout < 0 ||
		    dup2(temporary_stdout_fd, 1) < 0) {
			res = error_errno(_("could not duplicate stdout"));
			எனில் (saved_stdout >= 0)
				close(saved_stdout);
			close(temporary_stdout_fd);
			நிறுத்து;
		}

		res = bisect_state(terms, 1, &new_state);

		fflush(stdout);
		dup2(saved_stdout, 1);
		close(saved_stdout);
		close(temporary_stdout_fd);

		print_file_to_stdout(git_path_bisect_run());

		எனில் (res == BISECT_ONLY_SKIPPED_இடது)
			error(_("bisect run cannot continue any more"));
		இல்லையெனில் எனில் (res == BISECT_INTERNAL_SUCCESS_MERGE_BASE) {
			puts(_("bisect run success"));
			res = BISECT_OK;
		} இல்லையெனில் எனில் (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
			printf(_("bisect found first '%s' commit\n"), terms->term_bad);
		} இல்லையெனில் எனில் (res) {
			error(_("bisect run failed: 'git bisect %s'"
				" exited with error code %d"), new_state, res);
		} இல்லையெனில் {
			தொடர்;
		}
		நிறுத்து;
	}

	strbuf_release(&command);
	திரும்பு res;
}

நிலையான முழு cmd_bisect__reset(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix UNUSED,
			     கட்டமைப்பு repository *repo UNUSED)
{
	முழு res;

	எனில் (argc > 1)
		திரும்பு error(_("'%s' requires either no argument or a commit"),
			     "git bisect reset");
	res = bisect_reset(argc ? argv[0] : வெற்று, false);
	எனில் (res)
		திரும்பு res;
	திரும்பு bisect_clean_state();
}

நிலையான முழு cmd_bisect__terms(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix UNUSED,
			     கட்டமைப்பு repository *repo UNUSED)
{
	முழு res;
	கட்டமைப்பு bisect_terms terms = { 0 };

	எனில் (argc > 1)
		திரும்பு error(_("'%s' requires 0 or 1 argument"),
			     "git bisect terms");
	res = bisect_terms(&terms, argc == 1 ? argv[0] : வெற்று);
	free_terms(&terms);
	திரும்பு res;
}

நிலையான முழு cmd_bisect__start(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix UNUSED,
			     கட்டமைப்பு repository *repo UNUSED)
{
	முழு res;
	கட்டமைப்பு bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	res = bisect_start(&terms, argc, argv);
	free_terms(&terms);
	திரும்பு res;
}

நிலையான முழு cmd_bisect__next(முழு argc, மாறிலி எழுத்து **argv UNUSED, மாறிலி எழுத்து *prefix,
			    கட்டமைப்பு repository *repo UNUSED)
{
	முழு res;
	கட்டமைப்பு bisect_terms terms = { 0 };

	எனில் (argc)
		திரும்பு error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	எனில் (get_terms(&terms, 1))
		திரும்பு error(_("no terms defined"));
	res = bisect_next(&terms, prefix);
	free_terms(&terms);
	திரும்பு res;
}

நிலையான முழு cmd_bisect__log(முழு argc UNUSED, மாறிலி எழுத்து **argv UNUSED,
			   மாறிலி எழுத்து *prefix UNUSED,
			   கட்டமைப்பு repository *repo UNUSED)
{
	திரும்பு bisect_log();
}

நிலையான முழு cmd_bisect__replay(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix UNUSED,
			      கட்டமைப்பு repository *repo UNUSED)
{
	முழு res;
	கட்டமைப்பு bisect_terms terms = { 0 };

	எனில் (argc != 1)
		திரும்பு error(_("no logfile given"));
	set_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	திரும்பு res;
}

நிலையான முழு cmd_bisect__skip(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix UNUSED,
			    கட்டமைப்பு repository *repo UNUSED)
{
	முழு res;
	கட்டமைப்பு bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	எனில் (get_terms(&terms, 1))
		திரும்பு error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	திரும்பு res;
}

நிலையான முழு cmd_bisect__visualize(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix UNUSED,
				 கட்டமைப்பு repository *repo UNUSED)
{
	முழு res;
	கட்டமைப்பு bisect_terms terms = { 0 };

	எனில் (get_terms(&terms, 1))
		திரும்பு error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	திரும்பு res;
}

நிலையான முழு cmd_bisect__run(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix UNUSED,
			   கட்டமைப்பு repository *repo UNUSED)
{
	முழு res;
	கட்டமைப்பு bisect_terms terms = { 0 };

	எனில் (!argc)
		திரும்பு error(_("'%s' failed: no command provided."), "git bisect run");
	எனில் (get_terms(&terms, 1))
		திரும்பு error(_("no terms defined"));
	res = bisect_run(&terms, argc, argv);
	free_terms(&terms);
	திரும்பு res;
}

முழு cmd_bisect(முழு argc,
	       மாறிலி எழுத்து **argv,
	       மாறிலி எழுத்து *prefix,
	       கட்டமைப்பு repository *repo)
{
	முழு res = 0;
	parse_opt_subcommand_fn *fn = வெற்று;
	கட்டமைப்பு option options[] = {
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

	எனில் (!fn) {
		கட்டமைப்பு bisect_terms terms = { 0 };

		எனில் (!argc)
			usage_செய்தி_opt(_("need a command"), git_bisect_usage, options);

		எனில் (!strcmp(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		set_terms(&terms, "bad", "good");
		எனில் (get_terms(&terms, 1))
			திரும்பு error(_("no terms defined"));
		எனில் (check_and_set_terms(&terms, argv[0]) ||
		    !one_of(argv[0], terms.term_good, terms.term_bad, வெற்று))
			usage_செய்தி_optf(_("unknown command: '%s'"), git_bisect_usage,
				       options, argv[0]);
		இல்லையெனில்
			res = bisect_state(&terms, argc, argv);
		free_terms(&terms);
	} இல்லையெனில் {
		argc--;
		argv++;
		res = fn(argc, argv, prefix, repo);
	}

	எனில் (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		எண்ணிக்கை reset_when_found_mode mode;

		எனில் (read_reset_when_found(&mode))
			res = BISECT_FAILED;
		இல்லையெனில் எனில் (mode != RESET_WHEN_FOUND_NONE &&
			 bisect_reset_when_found(mode))
			res = BISECT_FAILED;
	}

	திரும்பு is_bisect_success(res) ? 0 : -res;
}
