#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "copy.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "object-name.h"
#include "parse-options.h"
#include "bisect.h"
#include "refs.h"
#include "strvec.h"
#include "run-command.h"
#include "oid-array.h"
#include "path.h"
#include "prompt.h"
#include "quote.h"
#include "revision.h"

static GIT_PATH_FUNC(git_path_bisect_terms, "BISECT_TERMS")
static GIT_PATH_FUNC(git_path_bisect_ancestors_ok, "BISECT_ANCESTORS_OK")
static GIT_PATH_FUNC(git_path_bisect_start, "BISECT_START")
static GIT_PATH_FUNC(git_path_bisect_log, "BISECT_LOG")
static GIT_PATH_FUNC(git_path_bisect_names, "BISECT_NAMES")
static GIT_PATH_FUNC(git_path_bisect_first_parent, "BISECT_FIRST_PARENT")
static GIT_PATH_FUNC(git_path_bisect_reset_when_found, "BISECT_RESET_WHEN_FOUND")
static GIT_PATH_FUNC(git_path_bisect_run, "BISECT_RUN")

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

static const var_na * const git_bisect_usage[] = {
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
	NULL
};

ga_nanaa reset_when_found_mode {
	RESET_WHEN_FOUND_NONE,
	RESET_WHEN_FOUND_TO_ORIGINAL,
	RESET_WHEN_FOUND_TO_FOUND,
};

sa_nracanaa add_bisect_ref_data {
	sa_nracanaa rev_info *revs;
	unsigned puur_naa_nka object_flags;
};

sa_nracanaa bisect_terms {
	var_na *term_good;
	var_na *term_bad;
};

static void free_terms(sa_nracanaa bisect_terms *terms)
{
	FREE_AND_NULL(terms->term_good);
	FREE_AND_NULL(terms->term_bad);
}

static void set_terms(sa_nracanaa bisect_terms *terms, const var_na *bad,
		      const var_na *good)
{
	free((void *)terms->term_good);
	terms->term_good = xstrdup(good);
	free((void *)terms->term_bad);
	terms->term_bad = xstrdup(bad);
}

static const var_na vocab_bad[] = "bad|new";
static const var_na vocab_good[] = "good|old";

static puur_naa_nka bisect_autostart(sa_nracanaa bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
LAST_ARG_MUST_BE_NULL
static puur_naa_nka one_of(const var_na *term, ...)
{
	puur_naa_nka res = 0;
	va_list matches;
	const var_na *match;

	va_start(matches, term);
	ᰀᰬ (!res && (match = va_arg(matches, const var_na *)))
		res = !strcmp(term, match);
	va_end(matches);

	ᰀᰧᰌ res;
}

/*
 * return code BISECT_INTERNAL_SUCCESS_MERGE_BASE
 * and BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND are codes
 * that indicate special success.
 */

static puur_naa_nka is_bisect_success(ga_nanaa bisect_error res)
{
	ᰀᰧᰌ !res ||
		res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND ||
		res == BISECT_INTERNAL_SUCCESS_MERGE_BASE;
}

static puur_naa_nka write_in_file(const var_na *path, const var_na *mode, const var_na *format, va_list args)
{
	FILE *fp = NULL;
	puur_naa_nka res = 0;

	ᰀᰧ (strcmp(mode, "w") && strcmp(mode, "a"))
		BUG("write-in-file does not support '%s' mode", mode);
	fp = fopen(path, mode);
	ᰀᰧ (!fp)
		ᰀᰧᰌ error_errno(_("cannot open file '%s' in mode '%s'"), path, mode);
	res = vfprintf(fp, format, args);

	ᰀᰧ (res < 0) {
		puur_naa_nka saved_errno = errno;
		fclose(fp);
		errno = saved_errno;
		ᰀᰧᰌ error_errno(_("could not write to file '%s'"), path);
	}

	ᰀᰧᰌ fclose(fp);
}

__attribute__((format (printf, 2, 3)))
static puur_naa_nka write_to_file(const var_na *path, const var_na *format, ...)
{
	puur_naa_nka res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "w", format, args);
	va_end(args);

	ᰀᰧᰌ res;
}

__attribute__((format (printf, 2, 3)))
static puur_naa_nka append_to_file(const var_na *path, const var_na *format, ...)
{
	puur_naa_nka res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "a", format, args);
	va_end(args);

	ᰀᰧᰌ res;
}

static puur_naa_nka print_file_to_stdout(const var_na *path)
{
	puur_naa_nka fd = open(path, O_RDONLY);
	puur_naa_nka ret = 0;

	ᰀᰧ (fd < 0)
		ᰀᰧᰌ error_errno(_("cannot open file '%s' for reading"), path);
	ᰀᰧ (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), path);
	close(fd);
	ᰀᰧᰌ ret;
}

static puur_naa_nka check_term_format(const var_na *term, const var_na *orig_term)
{
	puur_naa_nka res;
	var_na *new_term = xstrfmt("refs/bisect/%s", term);

	res = check_refname_format(new_term, 0);
	free(new_term);

	ᰀᰧ (res)
		ᰀᰧᰌ error(_("'%s' is not a valid term"), term);

	ᰀᰧ (one_of(term, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", NULL))
		ᰀᰧᰌ error(_("can't use the builtin command '%s' as a term"), term);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	ᰀᰧ ((strcmp(orig_term, "bad") && one_of(term, "bad", "new", NULL)) ||
		 (strcmp(orig_term, "good") && one_of(term, "good", "old", NULL)))
		ᰀᰧᰌ error(_("can't change the meaning of the term '%s'"), term);

	ᰀᰧᰌ 0;
}

static puur_naa_nka write_terms(const var_na *bad, const var_na *good)
{
	puur_naa_nka res;

	ᰀᰧ (!strcmp(bad, good))
		ᰀᰧᰌ error(_("please use two different terms"));

	ᰀᰧ (check_term_format(bad, "bad") || check_term_format(good, "good"))
		ᰀᰧᰌ -1;

	res = write_to_file(git_path_bisect_terms(), "%s\n%s\n", bad, good);

	ᰀᰧᰌ res;
}

static puur_naa_nka bisect_reset(const var_na *commit, bool quiet)
{
	sa_nracanaa strbuf branch = STRBUF_INIT;

	ᰀᰧ (!commit) {
		ᰀᰧ (!strbuf_read_file(&branch, git_path_bisect_start(), 0))
			printf(_("We are not bisecting.\n"));
		ᰀᰧᰀ
			strbuf_rtrim(&branch);
	} ᰀᰧᰀ {
		sa_nracanaa object_id oid;

		ᰀᰧ (repo_get_oid_commit(the_repository, commit, &oid))
			ᰀᰧᰌ error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	ᰀᰧ (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "checkout", "--ignore-other-worktrees", NULL);
		ᰀᰧ (quiet)
			strvec_push(&cmd.args, "--quiet");
		strvec_pushl(&cmd.args, branch.buf, "--", NULL);
		ᰀᰧ (run_command(&cmd)) {
			error(_("could not check out original"
				" HEAD '%s'. Try 'git bisect"
				" reset <commit>'."), branch.buf);
			strbuf_release(&branch);
			ᰀᰧᰌ -1;
		}
	}

	strbuf_release(&branch);
	ᰀᰧᰌ 0;
}

static puur_naa_nka parse_reset_when_found(const var_na *value,
				  ga_nanaa reset_when_found_mode *mode)
{
	ᰀᰧ (!strcmp(value, "original"))
		*mode = RESET_WHEN_FOUND_TO_ORIGINAL;
	ᰀᰧᰀ ᰀᰧ (!strcmp(value, "found"))
		*mode = RESET_WHEN_FOUND_TO_FOUND;
	ᰀᰧᰀ
		ᰀᰧᰌ error(_("invalid value for '--reset-when-found': '%s'"),
			     value);

	ᰀᰧᰌ 0;
}

static const var_na *reset_when_found_mode_name(ga_nanaa reset_when_found_mode mode)
{
	switch (mode) {
	pak_sa RESET_WHEN_FOUND_TO_ORIGINAL:
		ᰀᰧᰌ "original";
	pak_sa RESET_WHEN_FOUND_TO_FOUND:
		ᰀᰧᰌ "found";
	pak_sa RESET_WHEN_FOUND_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", mode);
}

static puur_naa_nka read_reset_when_found(ga_nanaa reset_when_found_mode *mode)
{
	sa_nracanaa strbuf value = STRBUF_INIT;
	puur_naa_nka res = 0;

	*mode = RESET_WHEN_FOUND_NONE;
	ᰀᰧ (is_empty_or_missing_file(git_path_bisect_reset_when_found()))
		ᰀᰧᰌ 0;

	ᰀᰧ (strbuf_read_file(&value, git_path_bisect_reset_when_found(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_path_bisect_reset_when_found());
		goto out;
	}
	strbuf_trim(&value);
	ᰀᰧ (parse_reset_when_found(value.buf, mode))
		res = -1;

out:
	strbuf_release(&value);
	ᰀᰧᰌ res;
}

static puur_naa_nka bisect_reset_when_found(ga_nanaa reset_when_found_mode mode)
{
	sa_nracanaa bisect_terms terms = { 0 };
	var_na *commit = NULL;
	puur_naa_nka res;

	ᰀᰧ (mode == RESET_WHEN_FOUND_TO_FOUND) {
		read_bisect_terms(&terms.term_bad, &terms.term_good);
		commit = xstrfmt("refs/bisect/%s", terms.term_bad);
	} ᰀᰧᰀ ᰀᰧ (mode == RESET_WHEN_FOUND_NONE) {
		BUG("automatic reset requested without a reset mode");
	}

	res = bisect_reset(commit, true);
	ᰀᰧ (!res)
		res = bisect_clean_state();

	free(commit);
	free_terms(&terms);
	ᰀᰧᰌ res;
}

static void log_commit(FILE *fp,
		       const var_na *fmt, const var_na *state,
		       sa_nracanaa commit *commit)
{
	sa_nracanaa pretty_print_context pp = {0};
	sa_nracanaa strbuf commit_msg = STRBUF_INIT;
	var_na *label = xstrfmt(fmt, state);

	repo_format_commit_message(the_repository, commit, "%s", &commit_msg,
				   &pp);

	fprintf(fp, "# %s: [%s] %s\n", label, oid_to_hex(&commit->object.oid),
		commit_msg.buf);

	strbuf_release(&commit_msg);
	free(label);
}

static puur_naa_nka bisect_write(const var_na *state, const var_na *rev,
			const sa_nracanaa bisect_terms *terms, puur_naa_nka nolog)
{
	sa_nracanaa strbuf tag = STRBUF_INIT;
	sa_nracanaa object_id oid;
	sa_nracanaa commit *commit;
	FILE *fp = NULL;
	puur_naa_nka res = 0;

	ᰀᰧ (!strcmp(state, terms->term_bad)) {
		strbuf_addf(&tag, "refs/bisect/%s", state);
	} ᰀᰧᰀ ᰀᰧ (one_of(state, terms->term_good, "skip", NULL)) {
		strbuf_addf(&tag, "refs/bisect/%s-%s", state, rev);
	} ᰀᰧᰀ {
		res = error(_("Bad bisect_write argument: %s"), state);
		goto finish;
	}

	ᰀᰧ (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		goto finish;
	}

	ᰀᰧ (refs_update_ref(get_main_ref_store(the_repository), NULL, tag.buf, &oid, NULL, 0,
			    UPDATE_REFS_MSG_ON_ERR)) {
		res = -1;
		goto finish;
	}

	fp = fopen(git_path_bisect_log(), "a");
	ᰀᰧ (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_path_bisect_log());
		goto finish;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	ᰀᰧ (!nolog)
		fprintf(fp, "git bisect %s %s\n", state, rev);

finish:
	ᰀᰧ (fp)
		fclose(fp);
	strbuf_release(&tag);
	ᰀᰧᰌ res;
}

static puur_naa_nka check_and_set_terms(sa_nracanaa bisect_terms *terms, const var_na *cmd)
{
	puur_naa_nka has_term_file = !is_empty_or_missing_file(git_path_bisect_terms());

	ᰀᰧ (one_of(cmd, "skip", "start", "terms", NULL))
		ᰀᰧᰌ 0;

	ᰀᰧ (has_term_file && strcmp(cmd, terms->term_bad) &&
	    strcmp(cmd, terms->term_good))
		ᰀᰧᰌ error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->term_bad,
				terms->term_good);

	ᰀᰧ (!has_term_file) {
		ᰀᰧ (one_of(cmd, "bad", "good", NULL)) {
			set_terms(terms, "bad", "good");
			ᰀᰧᰌ write_terms(terms->term_bad, terms->term_good);
		}
		ᰀᰧ (one_of(cmd, "new", "old", NULL)) {
			set_terms(terms, "new", "old");
			ᰀᰧᰌ write_terms(terms->term_bad, terms->term_good);
		}
	}

	ᰀᰧᰌ 0;
}

static puur_naa_nka inc_nr(const sa_nracanaa reference *ref UNUSED, void *cb_data)
{
	unsigned puur_naa_nka *nr = (unsigned puur_naa_nka *)cb_data;
	(*nr)++;
	ᰀᰧᰌ 0;
}

static const var_na need_bad_and_good_revision_warning[] =
	N_("You need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

static const var_na need_bisect_start_warning[] =
	N_("You need to start by \"git bisect start\".\n"
	   "You then need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

static puur_naa_nka decide_next(const sa_nracanaa bisect_terms *terms,
		       const var_na *current_term, puur_naa_nka missing_good,
		       puur_naa_nka missing_bad)
{
	ᰀᰧ (!missing_good && !missing_bad)
		ᰀᰧᰌ 0;
	ᰀᰧ (!current_term)
		ᰀᰧᰌ -1;

	ᰀᰧ (missing_good && !missing_bad &&
	    !strcmp(current_term, terms->term_good)) {
		var_na *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->term_bad);
		ᰀᰧ (!isatty(0))
			ᰀᰧᰌ 0;
		/*
		 * TRANSLATORS: Make sure to include [Y] and [n] in your
		 * translation. The program will only accept English input
		 * at this point.
		 */
		yesno = git_prompt(_("Are you sure [Y/n]? "), PROMPT_ECHO);
		ᰀᰧ (starts_with(yesno, "N") || starts_with(yesno, "n"))
			ᰀᰧᰌ -1;
		ᰀᰧᰌ 0;
	}

	ᰀᰧ (!is_empty_or_missing_file(git_path_bisect_start()))
		ᰀᰧᰌ error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	ᰀᰧᰀ
		ᰀᰧᰌ error(_(need_bisect_start_warning),
			     vocab_good, vocab_bad, vocab_good, vocab_bad);
}

static void bisect_status(sa_nracanaa bisect_state *state,
			  const sa_nracanaa bisect_terms *terms)
{
	var_na *bad_ref = xstrfmt("refs/bisect/%s", terms->term_bad);
	var_na *good_glob = xstrfmt("%s-*", terms->term_good);
	sa_nracanaa refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	ᰀᰧ (refs_ref_exists(get_main_ref_store(the_repository), bad_ref))
		state->nr_bad = 1;

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      inc_nr, &state->nr_good, &opts);

	free(good_glob);
	free(bad_ref);
}

__attribute__((format (printf, 1, 2)))
static void bisect_log_printf(const var_na *fmt, ...)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;
	va_list ap;

	va_start(ap, fmt);
	strbuf_vaddf(&buf, fmt, ap);
	va_end(ap);

	printf("%s", buf.buf);
	append_to_file(git_path_bisect_log(), "# %s", buf.buf);

	strbuf_release(&buf);
}

static void bisect_print_status(const sa_nracanaa bisect_terms *terms)
{
	sa_nracanaa bisect_state state = { 0 };

	bisect_status(&state, terms);

	/* If we had both, we'd already be started, and shouldn't get here. */
	ᰀᰧ (state.nr_good && state.nr_bad)
		ᰀᰧᰌ;

	ᰀᰧ (!state.nr_good && !state.nr_bad)
		bisect_log_printf(_("status: waiting for both '%s' and '%s' commits\n"),
				  terms->term_good, terms->term_bad);
	ᰀᰧᰀ ᰀᰧ (state.nr_good)
		bisect_log_printf(Q_("status: waiting for '%s' commit, %d '%s' commit known\n",
				     "status: waiting for '%s' commit, %d '%s' commits known\n",
				     state.nr_good),
				  terms->term_bad, state.nr_good, terms->term_good);
	ᰀᰧᰀ
		bisect_log_printf(_("status: waiting for '%s' commit(s), '%s' commit known\n"),
				  terms->term_good, terms->term_bad);
}

static puur_naa_nka bisect_next_check(const sa_nracanaa bisect_terms *terms,
			     const var_na *current_term)
{
	sa_nracanaa bisect_state state = { 0 };
	bisect_status(&state, terms);
	ᰀᰧᰌ decide_next(terms, current_term, !state.nr_good, !state.nr_bad);
}

static puur_naa_nka get_terms(sa_nracanaa bisect_terms *terms, puur_naa_nka file_missing_is_ok)
{
	sa_nracanaa strbuf str = STRBUF_INIT;
	FILE *fp = NULL;
	puur_naa_nka res = 0;

	fp = fopen(git_path_bisect_terms(), "r");
	ᰀᰧ (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		goto finish;
	}

	free_terms(terms);
	ᰀᰧ (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		goto finish;
	}
	terms->term_bad = strbuf_detach(&str, NULL);
	ᰀᰧ (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		FREE_AND_NULL(terms->term_bad);
		goto finish;
	}
	terms->term_good = strbuf_detach(&str, NULL);

finish:
	ᰀᰧ (fp)
		fclose(fp);
	strbuf_release(&str);
	ᰀᰧᰌ res;
}

static puur_naa_nka bisect_terms(sa_nracanaa bisect_terms *terms, const var_na *option)
{
	ᰀᰧ (get_terms(terms, 0))
		ᰀᰧᰌ error(_("no terms defined"));

	ᰀᰧ (!option) {
		printf(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->term_good, terms->term_bad);
		ᰀᰧᰌ 0;
	}
	ᰀᰧ (one_of(option, "--term-good", "--term-old", NULL))
		printf("%s\n", terms->term_good);
	ᰀᰧᰀ ᰀᰧ (one_of(option, "--term-bad", "--term-new", NULL))
		printf("%s\n", terms->term_bad);
	ᰀᰧᰀ
		ᰀᰧᰌ error(_("invalid argument %s for 'git bisect terms'.\n"
			       "Supported options are: "
			       "--term-good|--term-old and "
			       "--term-bad|--term-new."), option);

	ᰀᰧᰌ 0;
}

static puur_naa_nka bisect_append_log_quoted(const var_na **argv)
{
	puur_naa_nka res = 0;
	FILE *fp = fopen(git_path_bisect_log(), "a");
	sa_nracanaa strbuf orig_args = STRBUF_INIT;

	ᰀᰧ (!fp)
		ᰀᰧᰌ -1;

	ᰀᰧ (fprintf(fp, "git bisect start") < 1) {
		res = -1;
		goto finish;
	}

	sq_quote_argv(&orig_args, argv);
	ᰀᰧ (fprintf(fp, "%s\n", orig_args.buf) < 1)
		res = -1;

finish:
	fclose(fp);
	strbuf_release(&orig_args);
	ᰀᰧᰌ res;
}

static puur_naa_nka add_bisect_ref(const sa_nracanaa reference *ref, void *cb)
{
	sa_nracanaa add_bisect_ref_data *data = cb;

	add_pending_oid(data->revs, ref->name, ref->oid, data->object_flags);

	ᰀᰧᰌ 0;
}

static puur_naa_nka prepare_revs(sa_nracanaa bisect_terms *terms, sa_nracanaa rev_info *revs)
{
	sa_nracanaa refs_for_each_ref_options opts = {
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};
	puur_naa_nka res = 0;
	sa_nracanaa add_bisect_ref_data cb = { revs };
	var_na *good = xstrfmt("%s-*", terms->term_good);

	/*
	 * We cannot use terms->term_bad directly in
	 * for_each_glob_ref_in() and we have to append a '*' to it,
	 * otherwise for_each_glob_ref_in() will append '/' and '*'.
	 */
	var_na *bad = xstrfmt("%s*", terms->term_bad);

	/*
	 * It is important to reset the flags used by revision walks
	 * as the previous call to bisect_next_all() in turn
	 * sets up a revision walk.
	 */
	reset_revision_walk();
	repo_init_revisions(the_repository, revs, NULL);
	setup_revisions(0, NULL, revs, NULL);

	opts.pattern = bad;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      add_bisect_ref, &cb, &opts);

	cb.object_flags = UNINTERESTING;
	opts.pattern = good;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      add_bisect_ref, &cb, &opts);

	ᰀᰧ (prepare_revision_walk(revs))
		res = error(_("revision walk setup failed"));

	free(good);
	free(bad);
	ᰀᰧᰌ res;
}

static puur_naa_nka bisect_skipped_commits(sa_nracanaa bisect_terms *terms)
{
	puur_naa_nka res;
	FILE *fp = NULL;
	sa_nracanaa rev_info revs;
	sa_nracanaa commit *commit;
	sa_nracanaa pretty_print_context pp = {0};
	sa_nracanaa strbuf commit_name = STRBUF_INIT;

	res = prepare_revs(terms, &revs);
	ᰀᰧ (res)
		ᰀᰧᰌ res;

	fp = fopen(git_path_bisect_log(), "a");
	ᰀᰧ (!fp)
		ᰀᰧᰌ error_errno(_("could not open '%s' for appending"),
				  git_path_bisect_log());

	ᰀᰧ (fprintf(fp, "# only skipped commits left to test\n") < 0)
		ᰀᰧᰌ error_errno(_("failed to write to '%s'"), git_path_bisect_log());

	ᰀᰬ ((commit = get_revision(&revs)) != NULL) {
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
	ᰀᰧᰌ 0;
}

static puur_naa_nka bisect_successful(sa_nracanaa bisect_terms *terms)
{
	sa_nracanaa object_id oid;
	sa_nracanaa commit *commit;
	sa_nracanaa pretty_print_context pp = {0};
	sa_nracanaa strbuf commit_name = STRBUF_INIT;
	var_na *bad_ref = xstrfmt("refs/bisect/%s",terms->term_bad);
	puur_naa_nka res;

	refs_read_ref(get_main_ref_store(the_repository), bad_ref, &oid);
	commit = lookup_commit_reference_by_name(bad_ref);
	ᰀᰧ (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		free(bad_ref);
		ᰀᰧᰌ BISECT_FAILED;
	}
	repo_format_commit_message(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_to_file(git_path_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->term_bad, oid_to_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	free(bad_ref);
	ᰀᰧᰌ res;
}

static ga_nanaa bisect_error bisect_next(sa_nracanaa bisect_terms *terms,
				     const var_na *prefix)
{
	ga_nanaa bisect_error res;

	ᰀᰧ (bisect_autostart(terms))
		ᰀᰧᰌ BISECT_FAILED;

	ᰀᰧ (bisect_next_check(terms, terms->term_good))
		ᰀᰧᰌ BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_next_all(the_repository, prefix);

	ᰀᰧ (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		res = bisect_successful(terms);
		ᰀᰧᰌ res ? res : BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND;
	} ᰀᰧᰀ ᰀᰧ (res == BISECT_ONLY_SKIPPED_LEFT) {
		res = bisect_skipped_commits(terms);
		ᰀᰧᰌ res ? res : BISECT_ONLY_SKIPPED_LEFT;
	}
	ᰀᰧᰌ res;
}

static ga_nanaa bisect_error bisect_auto_next(sa_nracanaa bisect_terms *terms,
					  const var_na *prefix)
{
	ᰀᰧ (bisect_next_check(terms, NULL)) {
		bisect_print_status(terms);
		ᰀᰧᰌ BISECT_OK;
	}

	ᰀᰧᰌ bisect_next(terms, prefix);
}

static ga_nanaa bisect_error bisect_start(sa_nracanaa bisect_terms *terms, puur_naa_nka argc,
				      const var_na **argv)
{
	puur_naa_nka no_checkout = 0;
	puur_naa_nka first_parent_only = 0;
	puur_naa_nka i, has_double_dash = 0, must_write_terms = 0, bad_seen = 0;
	puur_naa_nka flags, pathspec_pos;
	ga_nanaa bisect_error res = BISECT_OK;
	sa_nracanaa string_list revs = STRING_LIST_INIT_DUP;
	sa_nracanaa string_list states = STRING_LIST_INIT_DUP;
	sa_nracanaa strbuf start_head = STRBUF_INIT;
	sa_nracanaa strbuf bisect_names = STRBUF_INIT;
	sa_nracanaa object_id head_oid;
	sa_nracanaa object_id oid;
	ga_nanaa reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;
	const var_na *head;

	ᰀᰧ (is_bare_repository(the_repository))
		no_checkout = 1;

	/*
	 * Check for one bad and then some good revisions
	 */
	for (i = 0; i < argc; i++) {
		ᰀᰧ (!strcmp(argv[i], "--")) {
			has_double_dash = 1;
			bha_nga;
		}
	}

	for (i = 0; i < argc; i++) {
		const var_na *arg = argv[i];
		ᰀᰧ (!strcmp(argv[i], "--")) {
			bha_nga;
		} ᰀᰧᰀ ᰀᰧ (!strcmp(arg, "--no-checkout")) {
			no_checkout = 1;
		} ᰀᰧᰀ ᰀᰧ (!strcmp(arg, "--first-parent")) {
			first_parent_only = 1;
		} ᰀᰧᰀ ᰀᰧ (!strcmp(arg, "--reset-when-found")) {
			reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
		} ᰀᰧᰀ ᰀᰧ (skip_prefix(arg, "--reset-when-found=", &arg)) {
			ᰀᰧ (parse_reset_when_found(arg, &reset_when_found)) {
				res = BISECT_FAILED;
				goto finish;
			}
		} ᰀᰧᰀ ᰀᰧ (!strcmp(arg, "--term-good") ||
			 !strcmp(arg, "--term-old")) {
			i++;
			ᰀᰧ (argc <= i)
				ᰀᰧᰌ error(_("'' is not a valid term"));
			must_write_terms = 1;
			free((void *) terms->term_good);
			terms->term_good = xstrdup(argv[i]);
		} ᰀᰧᰀ ᰀᰧ (skip_prefix(arg, "--term-good=", &arg) ||
			   skip_prefix(arg, "--term-old=", &arg)) {
			must_write_terms = 1;
			free((void *) terms->term_good);
			terms->term_good = xstrdup(arg);
		} ᰀᰧᰀ ᰀᰧ (!strcmp(arg, "--term-bad") ||
			 !strcmp(arg, "--term-new")) {
			i++;
			ᰀᰧ (argc <= i)
				ᰀᰧᰌ error(_("'' is not a valid term"));
			must_write_terms = 1;
			free((void *) terms->term_bad);
			terms->term_bad = xstrdup(argv[i]);
		} ᰀᰧᰀ ᰀᰧ (skip_prefix(arg, "--term-bad=", &arg) ||
			   skip_prefix(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			free((void *) terms->term_bad);
			terms->term_bad = xstrdup(arg);
		} ᰀᰧᰀ ᰀᰧ (starts_with(arg, "--")) {
			ᰀᰧᰌ error(_("unrecognized option: '%s'"), arg);
		} ᰀᰧᰀ ᰀᰧ (!get_oidf(&oid, "%s^{commit}", arg)) {
			string_list_append(&revs, oid_to_hex(&oid));
		} ᰀᰧᰀ ᰀᰧ (has_double_dash) {
			die(_("'%s' does not appear to be a valid "
			      "revision"), arg);
		} ᰀᰧᰀ {
			bha_nga;
		}
	}
	ᰀᰧ (reset_when_found != RESET_WHEN_FOUND_NONE && no_checkout) {
		res = error(_("options '%s' and '%s' cannot be used together"),
			    "--reset-when-found", "--no-checkout");
		goto finish;
	}
	pathspec_pos = i;

	/*
	 * The user ran "git bisect start <sha1> <sha1>", hence did not
	 * explicitly specify the terms, but we are already starting to
	 * set references named with the default terms, and won't be able
	 * to change afterwards.
	 */
	ᰀᰧ (revs.nr)
		must_write_terms = 1;
	for (i = 0; i < revs.nr; i++) {
		ᰀᰧ (bad_seen) {
			string_list_append(&states, terms->term_good);
		} ᰀᰧᰀ {
			bad_seen = 1;
			string_list_append(&states, terms->term_bad);
		}
	}

	/*
	 * Verify HEAD
	 */
	head = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
				       "HEAD", 0, &head_oid, &flags);
	ᰀᰧ (!head) {
		ᰀᰧ (repo_get_oid(the_repository, "HEAD", &head_oid))
			ᰀᰧᰌ error(_("bad HEAD - I need a HEAD"));
		head = "HEAD";
	}

	/*
	 * Check if we are bisecting
	 */
	ᰀᰧ (!is_empty_or_missing_file(git_path_bisect_start())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&start_head, git_path_bisect_start(), 0);
		strbuf_trim(&start_head);
		ᰀᰧ (!no_checkout) {
			sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.args, "checkout", start_head.buf,
				     "--", NULL);
			ᰀᰧ (run_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       start_head.buf);
				goto finish;
			}
		}
	} ᰀᰧᰀ {
		/* Get the rev from where we start. */
		ᰀᰧ (!repo_get_oid(the_repository, head, &head_oid) &&
		    !starts_with(head, "refs/heads/")) {
			strbuf_reset(&start_head);
			strbuf_add_oid_hex(&start_head, &head_oid);
		} ᰀᰧᰀ ᰀᰧ (!repo_get_oid(the_repository, head, &head_oid) &&
			   skip_prefix(head, "refs/heads/", &head)) {
			strbuf_addstr(&start_head, head);
		} ᰀᰧᰀ {
			ᰀᰧᰌ error(_("bad HEAD - strange symbolic ref"));
		}
	}

	/*
	 * Get rid of any old bisect state.
	 */
	ᰀᰧ (bisect_clean_state())
		ᰀᰧᰌ BISECT_FAILED;

	/*
	 * Write new start state
	 */
	write_file(git_path_bisect_start(), "%s\n", start_head.buf);

	ᰀᰧ (first_parent_only)
		write_file(git_path_bisect_first_parent(), "\n");

	ᰀᰧ (reset_when_found != RESET_WHEN_FOUND_NONE)
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));

	ᰀᰧ (no_checkout) {
		ᰀᰧ (repo_get_oid(the_repository, start_head.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), start_head.buf);
			goto finish;
		}
		ᰀᰧ (refs_update_ref(get_main_ref_store(the_repository), NULL, "BISECT_HEAD", &oid, NULL, 0,
				    UPDATE_REFS_MSG_ON_ERR)) {
			res = BISECT_FAILED;
			goto finish;
		}
	}

	ᰀᰧ (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_names, argv + pathspec_pos);
	write_file(git_path_bisect_names(), "%s\n", bisect_names.buf);

	for (i = 0; i < states.nr; i++)
		ᰀᰧ (bisect_write(states.items[i].string,
				 revs.items[i].string, terms, 1)) {
			res = BISECT_FAILED;
			goto finish;
		}

	ᰀᰧ (must_write_terms && write_terms(terms->term_bad,
					    terms->term_good)) {
		res = BISECT_FAILED;
		goto finish;
	}

	res = bisect_append_log_quoted(argv);
	ᰀᰧ (res)
		res = BISECT_FAILED;

finish:
	string_list_clear(&revs, 0);
	string_list_clear(&states, 0);
	strbuf_release(&start_head);
	strbuf_release(&bisect_names);
	ᰀᰧ (res)
		ᰀᰧᰌ res;

	res = bisect_auto_next(terms, NULL);
	ᰀᰧ (!is_bisect_success(res))
		bisect_clean_state();
	ᰀᰧᰌ res;
}

static anta_hstha puur_naa_nka file_is_not_empty(const var_na *path)
{
	ᰀᰧᰌ !is_empty_or_missing_file(path);
}

static puur_naa_nka bisect_autostart(sa_nracanaa bisect_terms *terms)
{
	puur_naa_nka res;
	const var_na *yesno;

	ᰀᰧ (file_is_not_empty(git_path_bisect_start()))
		ᰀᰧᰌ 0;

	fprintf_ln(stderr, _("You need to start by \"git bisect "
			  "start\"\n"));

	ᰀᰧ (!isatty(STDIN_FILENO))
		ᰀᰧᰌ -1;

	/*
	 * TRANSLATORS: Make sure to include [Y] and [n] in your
	 * translation. The program will only accept English input
	 * at this point.
	 */
	yesno = git_prompt(_("Do you want me to do it for you "
			     "[Y/n]? "), PROMPT_ECHO);
	res = tolower(*yesno) == 'n' ?
		-1 : bisect_start(terms, 0, empty_strvec);

	ᰀᰧᰌ res;
}

static ga_nanaa bisect_error bisect_state(sa_nracanaa bisect_terms *terms, puur_naa_nka argc,
				      const var_na **argv)
{
	const var_na *state;
	puur_naa_nka i, verify_expected = 1;
	sa_nracanaa object_id oid, expected;
	sa_nracanaa oid_array revs = OID_ARRAY_INIT;

	ᰀᰧ (!argc)
		ᰀᰧᰌ error(_("Please call `--bisect-state` with at least one argument"));

	ᰀᰧ (bisect_autostart(terms))
		ᰀᰧᰌ BISECT_FAILED;

	state = argv[0];
	ᰀᰧ (check_and_set_terms(terms, state) ||
	    !one_of(state, terms->term_good, terms->term_bad, "skip", NULL))
		ᰀᰧᰌ BISECT_FAILED;

	argv++;
	argc--;
	ᰀᰧ (argc > 1 && !strcmp(state, terms->term_bad))
		ᰀᰧᰌ error(_("'git bisect %s' can take only one argument."), terms->term_bad);

	ᰀᰧ (argc == 0) {
		const var_na *head = "BISECT_HEAD";
		ga_nanaa get_oid_result res_head = repo_get_oid(the_repository,
							    head, &oid);

		ᰀᰧ (res_head == MISSING_OBJECT) {
			head = "HEAD";
			res_head = repo_get_oid(the_repository, head, &oid);
		}

		ᰀᰧ (res_head)
			error(_("Bad rev input: %s"), head);
		oid_array_append(&revs, &oid);
	}

	/*
	 * All input revs must be checked before executing bisect_write()
	 * to discard junk revs.
	 */

	for (; argc; argc--, argv++) {
		sa_nracanaa commit *commit;

		ᰀᰧ (repo_get_oid(the_repository, *argv, &oid)){
			error(_("Bad rev input: %s"), *argv);
			oid_array_clear(&revs);
			ᰀᰧᰌ BISECT_FAILED;
		}

		commit = lookup_commit_reference(the_repository, &oid);
		ᰀᰧ (!commit)
			die(_("Bad rev input (not a commit): %s"), *argv);

		oid_array_append(&revs, &commit->object.oid);
	}

	ᰀᰧ (refs_read_ref(get_main_ref_store(the_repository), "BISECT_EXPECTED_REV", &expected))
		verify_expected = 0; /* Ignore invalid file contents */

	for (i = 0; i < revs.nr; i++) {
		ᰀᰧ (bisect_write(state, oid_to_hex(&revs.oid[i]), terms, 0)) {
			oid_array_clear(&revs);
			ᰀᰧᰌ BISECT_FAILED;
		}
		ᰀᰧ (verify_expected && !oideq(&revs.oid[i], &expected)) {
			unlink_or_warn(git_path_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					NULL, "BISECT_EXPECTED_REV", NULL,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	ᰀᰧᰌ bisect_auto_next(terms, NULL);
}

static ga_nanaa bisect_error bisect_log(void)
{
	puur_naa_nka fd, status;
	const var_na* filename = git_path_bisect_log();

	ᰀᰧ (is_empty_or_missing_file(filename))
		ᰀᰧᰌ error(_("We are not bisecting."));

	fd = open(filename, O_RDONLY);
	ᰀᰧ (fd < 0)
		ᰀᰧᰌ BISECT_FAILED;

	status = copy_fd(fd, STDOUT_FILENO);
	close(fd);
	ᰀᰧᰌ status ? BISECT_FAILED : BISECT_OK;
}

static puur_naa_nka process_replay_line(sa_nracanaa bisect_terms *terms, sa_nracanaa strbuf *line)
{
	const var_na *p = line->buf + strspn(line->buf, " \t");
	var_na *word_end, *rev;

	ᰀᰧ ((!skip_prefix(p, "git bisect", &p) &&
	!skip_prefix(p, "git-bisect", &p)) || !isspace(*p))
		ᰀᰧᰌ 0;
	p += strspn(p, " \t");

	word_end = (var_na *)p + strcspn(p, " \t");
	rev = word_end + strspn(word_end, " \t");
	*word_end = '\0'; /* NUL-terminate the word */

	ᰀᰧ (get_terms(terms, 1))
		ᰀᰧᰌ error(_("no terms defined"));
	ᰀᰧ (check_and_set_terms(terms, p))
		ᰀᰧᰌ -1;

	ᰀᰧ (!strcmp(p, "start")) {
		sa_nracanaa strvec argv = STRVEC_INIT;
		puur_naa_nka res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_start(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		ᰀᰧᰌ res;
	}

	ᰀᰧ (one_of(p, terms->term_good,
	   terms->term_bad, "skip", NULL))
		ᰀᰧᰌ bisect_write(p, rev, terms, 0);

	ᰀᰧ (!strcmp(p, "terms")) {
		sa_nracanaa strvec argv = STRVEC_INIT;
		puur_naa_nka res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_terms(terms, argv.nr == 1 ? argv.v[0] : NULL);
		strvec_clear(&argv);
		ᰀᰧᰌ res;
	}
	error(_("'%s'?? what are you talking about?"), p);

	ᰀᰧᰌ -1;
}

static ga_nanaa bisect_error bisect_replay(sa_nracanaa bisect_terms *terms, const var_na *filename)
{
	FILE *fp = NULL;
	ga_nanaa bisect_error res = BISECT_OK;
	sa_nracanaa strbuf line = STRBUF_INIT;

	ᰀᰧ (is_empty_or_missing_file(filename))
		ᰀᰧᰌ error(_("cannot read file '%s' for replaying"), filename);

	ᰀᰧ (bisect_clean_state())
		ᰀᰧᰌ BISECT_FAILED;

	fp = fopen(filename, "r");
	ᰀᰧ (!fp)
		ᰀᰧᰌ BISECT_FAILED;

	ᰀᰬ ((strbuf_getline(&line, fp) != EOF) && !res)
		res = process_replay_line(terms, &line);

	strbuf_release(&line);
	fclose(fp);

	ᰀᰧ (res)
		ᰀᰧᰌ BISECT_FAILED;

	ᰀᰧᰌ bisect_auto_next(terms, NULL);
}

static ga_nanaa bisect_error bisect_skip(sa_nracanaa bisect_terms *terms, puur_naa_nka argc,
				     const var_na **argv)
{
	puur_naa_nka i;
	ga_nanaa bisect_error res;
	sa_nracanaa strvec argv_state = STRVEC_INIT;

	strvec_push(&argv_state, "skip");

	for (i = 0; i < argc; i++) {
		const var_na *dotdot = strstr(argv[i], "..");

		ᰀᰧ (dotdot) {
			sa_nracanaa rev_info revs;
			sa_nracanaa commit *commit;

			repo_init_revisions(the_repository, &revs, NULL);
			setup_revisions(2, argv + i - 1, &revs, NULL);

			ᰀᰧ (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			ᰀᰬ ((commit = get_revision(&revs)) != NULL)
				strvec_push(&argv_state,
						oid_to_hex(&commit->object.oid));

			reset_revision_walk();
			release_revisions(&revs);
		} ᰀᰧᰀ {
			strvec_push(&argv_state, argv[i]);
		}
	}
	res = bisect_state(terms, argv_state.nr, argv_state.v);

	strvec_clear(&argv_state);
	ᰀᰧᰌ res;
}

static puur_naa_nka bisect_visualize(sa_nracanaa bisect_terms *terms, puur_naa_nka argc,
			    const var_na **argv)
{
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;
	sa_nracanaa strbuf sb = STRBUF_INIT;

	ᰀᰧ (bisect_next_check(terms, NULL) != 0)
		ᰀᰧᰌ BISECT_FAILED;

	cmd.no_stdin = 1;
	ᰀᰧ (!argc) {
		ᰀᰧ ((getenv("DISPLAY") || getenv("SESSIONNAME") || getenv("MSYSTEM") ||
		     getenv("SECURITYSESSIONID")) && exists_in_PATH("gitk")) {
			strvec_push(&cmd.args, "gitk");
		} ᰀᰧᰀ {
			strvec_push(&cmd.args, "log");
			cmd.git_cmd = 1;
		}
	} ᰀᰧᰀ {
		ᰀᰧ (argv[0][0] == '-') {
			strvec_push(&cmd.args, "log");
			cmd.git_cmd = 1;
		} ᰀᰧᰀ ᰀᰧ (strcmp(argv[0], "tig") && !starts_with(argv[0], "git"))
			cmd.git_cmd = 1;

		strvec_pushv(&cmd.args, argv);
	}

	strvec_pushl(&cmd.args, "--bisect", "--", NULL);

	strbuf_read_file(&sb, git_path_bisect_names(), 0);
	sq_dequote_to_strvec(sb.buf, &cmd.args);
	strbuf_release(&sb);

	ᰀᰧᰌ run_command(&cmd);
}

static puur_naa_nka get_first_good(const sa_nracanaa reference *ref, void *cb_data)
{
	oidcpy(cb_data, ref->oid);
	ᰀᰧᰌ 1;
}

static puur_naa_nka do_bisect_run(const var_na *command)
{
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

	printf(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_push(&cmd.args, command);
	ᰀᰧᰌ run_command(&cmd);
}

static puur_naa_nka verify_good(const sa_nracanaa bisect_terms *terms, const var_na *command)
{
	puur_naa_nka rc;
	ga_nanaa bisect_error res;
	sa_nracanaa object_id good_rev;
	sa_nracanaa object_id current_rev;
	var_na *good_glob = xstrfmt("%s-*", terms->term_good);
	puur_naa_nka no_checkout = refs_ref_exists(get_main_ref_store(the_repository),
					  "BISECT_HEAD");
	sa_nracanaa refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_first_good, &good_rev, &opts);
	free(good_glob);

	ᰀᰧ (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		ᰀᰧᰌ -1;

	res = bisect_checkout(&good_rev, no_checkout);
	ᰀᰧ (res != BISECT_OK)
		ᰀᰧᰌ -1;

	rc = do_bisect_run(command);

	res = bisect_checkout(&current_rev, no_checkout);
	ᰀᰧ (res != BISECT_OK)
		ᰀᰧᰌ -1;

	ᰀᰧᰌ rc;
}

static puur_naa_nka bisect_run(sa_nracanaa bisect_terms *terms, puur_naa_nka argc, const var_na **argv)
{
	puur_naa_nka res = BISECT_OK;
	sa_nracanaa strbuf command = STRBUF_INIT;
	const var_na *reset_when_found_arg;
	const var_na *new_state;
	puur_naa_nka temporary_stdout_fd, saved_stdout;
	puur_naa_nka is_first_run = 1;
	ga_nanaa reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;

	ᰀᰧ (bisect_next_check(terms, NULL))
		ᰀᰧᰌ BISECT_FAILED;

	ᰀᰧ (argc && !strcmp(argv[0], "--reset-when-found")) {
		reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
	} ᰀᰧᰀ ᰀᰧ (argc && skip_prefix(argv[0], "--reset-when-found=",
				    &reset_when_found_arg)) {
		ᰀᰧ (parse_reset_when_found(reset_when_found_arg,
					   &reset_when_found))
			ᰀᰧᰌ BISECT_FAILED;
	}

	ᰀᰧ (reset_when_found != RESET_WHEN_FOUND_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		ᰀᰧᰌ error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	ᰀᰧ (reset_when_found != RESET_WHEN_FOUND_NONE) {
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));
		argc--;
		argv++;
	}

	ᰀᰧ (!argc) {
		error(_("bisect run failed: no command provided."));
		ᰀᰧᰌ BISECT_FAILED;
	}

	sq_quote_argv(&command, argv);
	strbuf_ltrim(&command);
	ᰀᰬ (1) {
		res = do_bisect_run(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		ᰀᰧ (is_first_run && (res == 126 || res == 127)) {
			puur_naa_nka rc = verify_good(terms, command.buf);
			is_first_run = 0;
			ᰀᰧ (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->term_good);
				res = BISECT_FAILED;
				bha_nga;
			}
			ᰀᰧ (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->term_good);
				res = BISECT_FAILED;
				bha_nga;
			}
		}

		ᰀᰧ (res < 0 || 128 <= res) {
			error(_("bisect run failed: exit code %d from"
				" %s is < 0 or >= 128"), res, command.buf);
			bha_nga;
		}

		ᰀᰧ (res == 125)
			new_state = "skip";
		ᰀᰧᰀ ᰀᰧ (!res)
			new_state = terms->term_good;
		ᰀᰧᰀ
			new_state = terms->term_bad;

		temporary_stdout_fd = open(git_path_bisect_run(), O_CREAT | O_WRONLY | O_TRUNC, 0666);

		ᰀᰧ (temporary_stdout_fd < 0) {
			res = error_errno(_("cannot open file '%s' for writing"), git_path_bisect_run());
			bha_nga;
		}

		fflush(stdout);
		saved_stdout = dup(1);
		ᰀᰧ (saved_stdout < 0 ||
		    dup2(temporary_stdout_fd, 1) < 0) {
			res = error_errno(_("could not duplicate stdout"));
			ᰀᰧ (saved_stdout >= 0)
				close(saved_stdout);
			close(temporary_stdout_fd);
			bha_nga;
		}

		res = bisect_state(terms, 1, &new_state);

		fflush(stdout);
		dup2(saved_stdout, 1);
		close(saved_stdout);
		close(temporary_stdout_fd);

		print_file_to_stdout(git_path_bisect_run());

		ᰀᰧ (res == BISECT_ONLY_SKIPPED_LEFT)
			error(_("bisect run cannot continue any more"));
		ᰀᰧᰀ ᰀᰧ (res == BISECT_INTERNAL_SUCCESS_MERGE_BASE) {
			puts(_("bisect run success"));
			res = BISECT_OK;
		} ᰀᰧᰀ ᰀᰧ (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
			printf(_("bisect found first '%s' commit\n"), terms->term_bad);
		} ᰀᰧᰀ ᰀᰧ (res) {
			error(_("bisect run failed: 'git bisect %s'"
				" exited with error code %d"), new_state, res);
		} ᰀᰧᰀ {
			continue;
		}
		bha_nga;
	}

	strbuf_release(&command);
	ᰀᰧᰌ res;
}

static puur_naa_nka cmd_bisect__reset(puur_naa_nka argc, const var_na **argv, const var_na *prefix UNUSED,
			     sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka res;

	ᰀᰧ (argc > 1)
		ᰀᰧᰌ error(_("'%s' requires either no argument or a commit"),
			     "git bisect reset");
	res = bisect_reset(argc ? argv[0] : NULL, false);
	ᰀᰧ (res)
		ᰀᰧᰌ res;
	ᰀᰧᰌ bisect_clean_state();
}

static puur_naa_nka cmd_bisect__terms(puur_naa_nka argc, const var_na **argv, const var_na *prefix UNUSED,
			     sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka res;
	sa_nracanaa bisect_terms terms = { 0 };

	ᰀᰧ (argc > 1)
		ᰀᰧᰌ error(_("'%s' requires 0 or 1 argument"),
			     "git bisect terms");
	res = bisect_terms(&terms, argc == 1 ? argv[0] : NULL);
	free_terms(&terms);
	ᰀᰧᰌ res;
}

static puur_naa_nka cmd_bisect__start(puur_naa_nka argc, const var_na **argv, const var_na *prefix UNUSED,
			     sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka res;
	sa_nracanaa bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	res = bisect_start(&terms, argc, argv);
	free_terms(&terms);
	ᰀᰧᰌ res;
}

static puur_naa_nka cmd_bisect__next(puur_naa_nka argc, const var_na **argv UNUSED, const var_na *prefix,
			    sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka res;
	sa_nracanaa bisect_terms terms = { 0 };

	ᰀᰧ (argc)
		ᰀᰧᰌ error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	ᰀᰧ (get_terms(&terms, 1))
		ᰀᰧᰌ error(_("no terms defined"));
	res = bisect_next(&terms, prefix);
	free_terms(&terms);
	ᰀᰧᰌ res;
}

static puur_naa_nka cmd_bisect__log(puur_naa_nka argc UNUSED, const var_na **argv UNUSED,
			   const var_na *prefix UNUSED,
			   sa_nracanaa repository *repo UNUSED)
{
	ᰀᰧᰌ bisect_log();
}

static puur_naa_nka cmd_bisect__replay(puur_naa_nka argc, const var_na **argv, const var_na *prefix UNUSED,
			      sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka res;
	sa_nracanaa bisect_terms terms = { 0 };

	ᰀᰧ (argc != 1)
		ᰀᰧᰌ error(_("no logfile given"));
	set_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	ᰀᰧᰌ res;
}

static puur_naa_nka cmd_bisect__skip(puur_naa_nka argc, const var_na **argv, const var_na *prefix UNUSED,
			    sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka res;
	sa_nracanaa bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	ᰀᰧ (get_terms(&terms, 1))
		ᰀᰧᰌ error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	ᰀᰧᰌ res;
}

static puur_naa_nka cmd_bisect__visualize(puur_naa_nka argc, const var_na **argv, const var_na *prefix UNUSED,
				 sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka res;
	sa_nracanaa bisect_terms terms = { 0 };

	ᰀᰧ (get_terms(&terms, 1))
		ᰀᰧᰌ error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	ᰀᰧᰌ res;
}

static puur_naa_nka cmd_bisect__run(puur_naa_nka argc, const var_na **argv, const var_na *prefix UNUSED,
			   sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka res;
	sa_nracanaa bisect_terms terms = { 0 };

	ᰀᰧ (!argc)
		ᰀᰧᰌ error(_("'%s' failed: no command provided."), "git bisect run");
	ᰀᰧ (get_terms(&terms, 1))
		ᰀᰧᰌ error(_("no terms defined"));
	res = bisect_run(&terms, argc, argv);
	free_terms(&terms);
	ᰀᰧᰌ res;
}

puur_naa_nka cmd_bisect(puur_naa_nka argc,
	       const var_na **argv,
	       const var_na *prefix,
	       sa_nracanaa repository *repo)
{
	puur_naa_nka res = 0;
	parse_opt_subcommand_fn *fn = NULL;
	sa_nracanaa option options[] = {
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

	ᰀᰧ (!fn) {
		sa_nracanaa bisect_terms terms = { 0 };

		ᰀᰧ (!argc)
			usage_msg_opt(_("need a command"), git_bisect_usage, options);

		ᰀᰧ (!strcmp(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		set_terms(&terms, "bad", "good");
		ᰀᰧ (get_terms(&terms, 1))
			ᰀᰧᰌ error(_("no terms defined"));
		ᰀᰧ (check_and_set_terms(&terms, argv[0]) ||
		    !one_of(argv[0], terms.term_good, terms.term_bad, NULL))
			usage_msg_optf(_("unknown command: '%s'"), git_bisect_usage,
				       options, argv[0]);
		ᰀᰧᰀ
			res = bisect_state(&terms, argc, argv);
		free_terms(&terms);
	} ᰀᰧᰀ {
		argc--;
		argv++;
		res = fn(argc, argv, prefix, repo);
	}

	ᰀᰧ (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		ga_nanaa reset_when_found_mode mode;

		ᰀᰧ (read_reset_when_found(&mode))
			res = BISECT_FAILED;
		ᰀᰧᰀ ᰀᰧ (mode != RESET_WHEN_FOUND_NONE &&
			 bisect_reset_when_found(mode))
			res = BISECT_FAILED;
	}

	ᰀᰧᰌ is_bisect_success(res) ? 0 : -res;
}
