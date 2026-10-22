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
	unsigned ကိန်းပြည့် object_flags;
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

static ကိန်းပြည့် bisect_autostart(sa_nracanaa bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
LAST_ARG_MUST_BE_NULL
static ကိန်းပြည့် one_of(const var_na *term, ...)
{
	ကိန်းပြည့် res = 0;
	va_list matches;
	const var_na *match;

	va_start(matches, term);
	နေစဉ် (!res && (match = va_arg(matches, const var_na *)))
		res = !strcmp(term, match);
	va_end(matches);

	ပြန်ပေး res;
}

/*
 * return code BISECT_INTERNAL_SUCCESS_MERGE_BASE
 * and BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND are codes
 * that indicate special success.
 */

static ကိန်းပြည့် is_bisect_success(ga_nanaa bisect_error res)
{
	ပြန်ပေး !res ||
		res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND ||
		res == BISECT_INTERNAL_SUCCESS_MERGE_BASE;
}

static ကိန်းပြည့် write_in_file(const var_na *path, const var_na *mode, const var_na *format, va_list args)
{
	FILE *fp = NULL;
	ကိန်းပြည့် res = 0;

	အကယ်၍ (strcmp(mode, "w") && strcmp(mode, "a"))
		BUG("write-in-file does not support '%s' mode", mode);
	fp = fopen(path, mode);
	အကယ်၍ (!fp)
		ပြန်ပေး error_errno(_("cannot open file '%s' in mode '%s'"), path, mode);
	res = vfprintf(fp, format, args);

	အကယ်၍ (res < 0) {
		ကိန်းပြည့် saved_errno = errno;
		fclose(fp);
		errno = saved_errno;
		ပြန်ပေး error_errno(_("could not write to file '%s'"), path);
	}

	ပြန်ပေး fclose(fp);
}

__attribute__((format (printf, 2, 3)))
static ကိန်းပြည့် write_to_file(const var_na *path, const var_na *format, ...)
{
	ကိန်းပြည့် res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "w", format, args);
	va_end(args);

	ပြန်ပေး res;
}

__attribute__((format (printf, 2, 3)))
static ကိန်းပြည့် append_to_file(const var_na *path, const var_na *format, ...)
{
	ကိန်းပြည့် res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "a", format, args);
	va_end(args);

	ပြန်ပေး res;
}

static ကိန်းပြည့် print_file_to_stdout(const var_na *path)
{
	ကိန်းပြည့် fd = open(path, O_RDONLY);
	ကိန်းပြည့် ret = 0;

	အကယ်၍ (fd < 0)
		ပြန်ပေး error_errno(_("cannot open file '%s' for reading"), path);
	အကယ်၍ (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), path);
	close(fd);
	ပြန်ပေး ret;
}

static ကိန်းပြည့် check_term_format(const var_na *term, const var_na *orig_term)
{
	ကိန်းပြည့် res;
	var_na *new_term = xstrfmt("refs/bisect/%s", term);

	res = check_refname_format(new_term, 0);
	free(new_term);

	အကယ်၍ (res)
		ပြန်ပေး error(_("'%s' is not a valid term"), term);

	အကယ်၍ (one_of(term, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", NULL))
		ပြန်ပေး error(_("can't use the builtin command '%s' as a term"), term);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	အကယ်၍ ((strcmp(orig_term, "bad") && one_of(term, "bad", "new", NULL)) ||
		 (strcmp(orig_term, "good") && one_of(term, "good", "old", NULL)))
		ပြန်ပေး error(_("can't change the meaning of the term '%s'"), term);

	ပြန်ပေး 0;
}

static ကိန်းပြည့် write_terms(const var_na *bad, const var_na *good)
{
	ကိန်းပြည့် res;

	အကယ်၍ (!strcmp(bad, good))
		ပြန်ပေး error(_("please use two different terms"));

	အကယ်၍ (check_term_format(bad, "bad") || check_term_format(good, "good"))
		ပြန်ပေး -1;

	res = write_to_file(git_path_bisect_terms(), "%s\n%s\n", bad, good);

	ပြန်ပေး res;
}

static ကိန်းပြည့် bisect_reset(const var_na *commit, bool quiet)
{
	sa_nracanaa strbuf branch = STRBUF_INIT;

	အကယ်၍ (!commit) {
		အကယ်၍ (!strbuf_read_file(&branch, git_path_bisect_start(), 0))
			printf(_("We are not bisecting.\n"));
		မဟုတ်ပါက
			strbuf_rtrim(&branch);
	} မဟုတ်ပါက {
		sa_nracanaa object_id oid;

		အကယ်၍ (repo_get_oid_commit(the_repository, commit, &oid))
			ပြန်ပေး error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	အကယ်၍ (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "checkout", "--ignore-other-worktrees", NULL);
		အကယ်၍ (quiet)
			strvec_push(&cmd.args, "--quiet");
		strvec_pushl(&cmd.args, branch.buf, "--", NULL);
		အကယ်၍ (run_command(&cmd)) {
			error(_("could not check out original"
				" HEAD '%s'. Try 'git bisect"
				" reset <commit>'."), branch.buf);
			strbuf_release(&branch);
			ပြန်ပေး -1;
		}
	}

	strbuf_release(&branch);
	ပြန်ပေး 0;
}

static ကိန်းပြည့် parse_reset_when_found(const var_na *value,
				  ga_nanaa reset_when_found_mode *mode)
{
	အကယ်၍ (!strcmp(value, "original"))
		*mode = RESET_WHEN_FOUND_TO_ORIGINAL;
	မဟုတ်ပါက အကယ်၍ (!strcmp(value, "found"))
		*mode = RESET_WHEN_FOUND_TO_FOUND;
	မဟုတ်ပါက
		ပြန်ပေး error(_("invalid value for '--reset-when-found': '%s'"),
			     value);

	ပြန်ပေး 0;
}

static const var_na *reset_when_found_mode_name(ga_nanaa reset_when_found_mode mode)
{
	switch (mode) {
	pak_sa RESET_WHEN_FOUND_TO_ORIGINAL:
		ပြန်ပေး "original";
	pak_sa RESET_WHEN_FOUND_TO_FOUND:
		ပြန်ပေး "found";
	pak_sa RESET_WHEN_FOUND_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", mode);
}

static ကိန်းပြည့် read_reset_when_found(ga_nanaa reset_when_found_mode *mode)
{
	sa_nracanaa strbuf value = STRBUF_INIT;
	ကိန်းပြည့် res = 0;

	*mode = RESET_WHEN_FOUND_NONE;
	အကယ်၍ (is_empty_or_missing_file(git_path_bisect_reset_when_found()))
		ပြန်ပေး 0;

	အကယ်၍ (strbuf_read_file(&value, git_path_bisect_reset_when_found(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_path_bisect_reset_when_found());
		goto out;
	}
	strbuf_trim(&value);
	အကယ်၍ (parse_reset_when_found(value.buf, mode))
		res = -1;

out:
	strbuf_release(&value);
	ပြန်ပေး res;
}

static ကိန်းပြည့် bisect_reset_when_found(ga_nanaa reset_when_found_mode mode)
{
	sa_nracanaa bisect_terms terms = { 0 };
	var_na *commit = NULL;
	ကိန်းပြည့် res;

	အကယ်၍ (mode == RESET_WHEN_FOUND_TO_FOUND) {
		read_bisect_terms(&terms.term_bad, &terms.term_good);
		commit = xstrfmt("refs/bisect/%s", terms.term_bad);
	} မဟုတ်ပါက အကယ်၍ (mode == RESET_WHEN_FOUND_NONE) {
		BUG("automatic reset requested without a reset mode");
	}

	res = bisect_reset(commit, true);
	အကယ်၍ (!res)
		res = bisect_clean_state();

	free(commit);
	free_terms(&terms);
	ပြန်ပေး res;
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

static ကိန်းပြည့် bisect_write(const var_na *state, const var_na *rev,
			const sa_nracanaa bisect_terms *terms, ကိန်းပြည့် nolog)
{
	sa_nracanaa strbuf tag = STRBUF_INIT;
	sa_nracanaa object_id oid;
	sa_nracanaa commit *commit;
	FILE *fp = NULL;
	ကိန်းပြည့် res = 0;

	အကယ်၍ (!strcmp(state, terms->term_bad)) {
		strbuf_addf(&tag, "refs/bisect/%s", state);
	} မဟုတ်ပါက အကယ်၍ (one_of(state, terms->term_good, "skip", NULL)) {
		strbuf_addf(&tag, "refs/bisect/%s-%s", state, rev);
	} မဟုတ်ပါက {
		res = error(_("Bad bisect_write argument: %s"), state);
		goto finish;
	}

	အကယ်၍ (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		goto finish;
	}

	အကယ်၍ (refs_update_ref(get_main_ref_store(the_repository), NULL, tag.buf, &oid, NULL, 0,
			    UPDATE_REFS_MSG_ON_ERR)) {
		res = -1;
		goto finish;
	}

	fp = fopen(git_path_bisect_log(), "a");
	အကယ်၍ (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_path_bisect_log());
		goto finish;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	အကယ်၍ (!nolog)
		fprintf(fp, "git bisect %s %s\n", state, rev);

finish:
	အကယ်၍ (fp)
		fclose(fp);
	strbuf_release(&tag);
	ပြန်ပေး res;
}

static ကိန်းပြည့် check_and_set_terms(sa_nracanaa bisect_terms *terms, const var_na *cmd)
{
	ကိန်းပြည့် has_term_file = !is_empty_or_missing_file(git_path_bisect_terms());

	အကယ်၍ (one_of(cmd, "skip", "start", "terms", NULL))
		ပြန်ပေး 0;

	အကယ်၍ (has_term_file && strcmp(cmd, terms->term_bad) &&
	    strcmp(cmd, terms->term_good))
		ပြန်ပေး error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->term_bad,
				terms->term_good);

	အကယ်၍ (!has_term_file) {
		အကယ်၍ (one_of(cmd, "bad", "good", NULL)) {
			set_terms(terms, "bad", "good");
			ပြန်ပေး write_terms(terms->term_bad, terms->term_good);
		}
		အကယ်၍ (one_of(cmd, "new", "old", NULL)) {
			set_terms(terms, "new", "old");
			ပြန်ပေး write_terms(terms->term_bad, terms->term_good);
		}
	}

	ပြန်ပေး 0;
}

static ကိန်းပြည့် inc_nr(const sa_nracanaa reference *ref UNUSED, void *cb_data)
{
	unsigned ကိန်းပြည့် *nr = (unsigned ကိန်းပြည့် *)cb_data;
	(*nr)++;
	ပြန်ပေး 0;
}

static const var_na need_bad_and_good_revision_warning[] =
	N_("You need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

static const var_na need_bisect_start_warning[] =
	N_("You need to start by \"git bisect start\".\n"
	   "You then need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

static ကိန်းပြည့် decide_next(const sa_nracanaa bisect_terms *terms,
		       const var_na *current_term, ကိန်းပြည့် missing_good,
		       ကိန်းပြည့် missing_bad)
{
	အကယ်၍ (!missing_good && !missing_bad)
		ပြန်ပေး 0;
	အကယ်၍ (!current_term)
		ပြန်ပေး -1;

	အကယ်၍ (missing_good && !missing_bad &&
	    !strcmp(current_term, terms->term_good)) {
		var_na *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->term_bad);
		အကယ်၍ (!isatty(0))
			ပြန်ပေး 0;
		/*
		 * TRANSLATORS: Make sure to include [Y] and [n] in your
		 * translation. The program will only accept English input
		 * at this point.
		 */
		yesno = git_prompt(_("Are you sure [Y/n]? "), PROMPT_ECHO);
		အကယ်၍ (starts_with(yesno, "N") || starts_with(yesno, "n"))
			ပြန်ပေး -1;
		ပြန်ပေး 0;
	}

	အကယ်၍ (!is_empty_or_missing_file(git_path_bisect_start()))
		ပြန်ပေး error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	မဟုတ်ပါက
		ပြန်ပေး error(_(need_bisect_start_warning),
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

	အကယ်၍ (refs_ref_exists(get_main_ref_store(the_repository), bad_ref))
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
	အကယ်၍ (state.nr_good && state.nr_bad)
		ပြန်ပေး;

	အကယ်၍ (!state.nr_good && !state.nr_bad)
		bisect_log_printf(_("status: waiting for both '%s' and '%s' commits\n"),
				  terms->term_good, terms->term_bad);
	မဟုတ်ပါက အကယ်၍ (state.nr_good)
		bisect_log_printf(Q_("status: waiting for '%s' commit, %d '%s' commit known\n",
				     "status: waiting for '%s' commit, %d '%s' commits known\n",
				     state.nr_good),
				  terms->term_bad, state.nr_good, terms->term_good);
	မဟုတ်ပါက
		bisect_log_printf(_("status: waiting for '%s' commit(s), '%s' commit known\n"),
				  terms->term_good, terms->term_bad);
}

static ကိန်းပြည့် bisect_next_check(const sa_nracanaa bisect_terms *terms,
			     const var_na *current_term)
{
	sa_nracanaa bisect_state state = { 0 };
	bisect_status(&state, terms);
	ပြန်ပေး decide_next(terms, current_term, !state.nr_good, !state.nr_bad);
}

static ကိန်းပြည့် get_terms(sa_nracanaa bisect_terms *terms, ကိန်းပြည့် file_missing_is_ok)
{
	sa_nracanaa strbuf str = STRBUF_INIT;
	FILE *fp = NULL;
	ကိန်းပြည့် res = 0;

	fp = fopen(git_path_bisect_terms(), "r");
	အကယ်၍ (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		goto finish;
	}

	free_terms(terms);
	အကယ်၍ (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		goto finish;
	}
	terms->term_bad = strbuf_detach(&str, NULL);
	အကယ်၍ (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		FREE_AND_NULL(terms->term_bad);
		goto finish;
	}
	terms->term_good = strbuf_detach(&str, NULL);

finish:
	အကယ်၍ (fp)
		fclose(fp);
	strbuf_release(&str);
	ပြန်ပေး res;
}

static ကိန်းပြည့် bisect_terms(sa_nracanaa bisect_terms *terms, const var_na *option)
{
	အကယ်၍ (get_terms(terms, 0))
		ပြန်ပေး error(_("no terms defined"));

	အကယ်၍ (!option) {
		printf(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->term_good, terms->term_bad);
		ပြန်ပေး 0;
	}
	အကယ်၍ (one_of(option, "--term-good", "--term-old", NULL))
		printf("%s\n", terms->term_good);
	မဟုတ်ပါက အကယ်၍ (one_of(option, "--term-bad", "--term-new", NULL))
		printf("%s\n", terms->term_bad);
	မဟုတ်ပါက
		ပြန်ပေး error(_("invalid argument %s for 'git bisect terms'.\n"
			       "Supported options are: "
			       "--term-good|--term-old and "
			       "--term-bad|--term-new."), option);

	ပြန်ပေး 0;
}

static ကိန်းပြည့် bisect_append_log_quoted(const var_na **argv)
{
	ကိန်းပြည့် res = 0;
	FILE *fp = fopen(git_path_bisect_log(), "a");
	sa_nracanaa strbuf orig_args = STRBUF_INIT;

	အကယ်၍ (!fp)
		ပြန်ပေး -1;

	အကယ်၍ (fprintf(fp, "git bisect start") < 1) {
		res = -1;
		goto finish;
	}

	sq_quote_argv(&orig_args, argv);
	အကယ်၍ (fprintf(fp, "%s\n", orig_args.buf) < 1)
		res = -1;

finish:
	fclose(fp);
	strbuf_release(&orig_args);
	ပြန်ပေး res;
}

static ကိန်းပြည့် add_bisect_ref(const sa_nracanaa reference *ref, void *cb)
{
	sa_nracanaa add_bisect_ref_data *data = cb;

	add_pending_oid(data->revs, ref->name, ref->oid, data->object_flags);

	ပြန်ပေး 0;
}

static ကိန်းပြည့် prepare_revs(sa_nracanaa bisect_terms *terms, sa_nracanaa rev_info *revs)
{
	sa_nracanaa refs_for_each_ref_options opts = {
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};
	ကိန်းပြည့် res = 0;
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

	အကယ်၍ (prepare_revision_walk(revs))
		res = error(_("revision walk setup failed"));

	free(good);
	free(bad);
	ပြန်ပေး res;
}

static ကိန်းပြည့် bisect_skipped_commits(sa_nracanaa bisect_terms *terms)
{
	ကိန်းပြည့် res;
	FILE *fp = NULL;
	sa_nracanaa rev_info revs;
	sa_nracanaa commit *commit;
	sa_nracanaa pretty_print_context pp = {0};
	sa_nracanaa strbuf commit_name = STRBUF_INIT;

	res = prepare_revs(terms, &revs);
	အကယ်၍ (res)
		ပြန်ပေး res;

	fp = fopen(git_path_bisect_log(), "a");
	အကယ်၍ (!fp)
		ပြန်ပေး error_errno(_("could not open '%s' for appending"),
				  git_path_bisect_log());

	အကယ်၍ (fprintf(fp, "# only skipped commits left to test\n") < 0)
		ပြန်ပေး error_errno(_("failed to write to '%s'"), git_path_bisect_log());

	နေစဉ် ((commit = get_revision(&revs)) != NULL) {
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
	ပြန်ပေး 0;
}

static ကိန်းပြည့် bisect_successful(sa_nracanaa bisect_terms *terms)
{
	sa_nracanaa object_id oid;
	sa_nracanaa commit *commit;
	sa_nracanaa pretty_print_context pp = {0};
	sa_nracanaa strbuf commit_name = STRBUF_INIT;
	var_na *bad_ref = xstrfmt("refs/bisect/%s",terms->term_bad);
	ကိန်းပြည့် res;

	refs_read_ref(get_main_ref_store(the_repository), bad_ref, &oid);
	commit = lookup_commit_reference_by_name(bad_ref);
	အကယ်၍ (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		free(bad_ref);
		ပြန်ပေး BISECT_FAILED;
	}
	repo_format_commit_message(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_to_file(git_path_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->term_bad, oid_to_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	free(bad_ref);
	ပြန်ပေး res;
}

static ga_nanaa bisect_error bisect_next(sa_nracanaa bisect_terms *terms,
				     const var_na *prefix)
{
	ga_nanaa bisect_error res;

	အကယ်၍ (bisect_autostart(terms))
		ပြန်ပေး BISECT_FAILED;

	အကယ်၍ (bisect_next_check(terms, terms->term_good))
		ပြန်ပေး BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_next_all(the_repository, prefix);

	အကယ်၍ (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		res = bisect_successful(terms);
		ပြန်ပေး res ? res : BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND;
	} မဟုတ်ပါက အကယ်၍ (res == BISECT_ONLY_SKIPPED_LEFT) {
		res = bisect_skipped_commits(terms);
		ပြန်ပေး res ? res : BISECT_ONLY_SKIPPED_LEFT;
	}
	ပြန်ပေး res;
}

static ga_nanaa bisect_error bisect_auto_next(sa_nracanaa bisect_terms *terms,
					  const var_na *prefix)
{
	အကယ်၍ (bisect_next_check(terms, NULL)) {
		bisect_print_status(terms);
		ပြန်ပေး BISECT_OK;
	}

	ပြန်ပေး bisect_next(terms, prefix);
}

static ga_nanaa bisect_error bisect_start(sa_nracanaa bisect_terms *terms, ကိန်းပြည့် argc,
				      const var_na **argv)
{
	ကိန်းပြည့် no_checkout = 0;
	ကိန်းပြည့် first_parent_only = 0;
	ကိန်းပြည့် i, has_double_dash = 0, must_write_terms = 0, bad_seen = 0;
	ကိန်းပြည့် flags, pathspec_pos;
	ga_nanaa bisect_error res = BISECT_OK;
	sa_nracanaa string_list revs = STRING_LIST_INIT_DUP;
	sa_nracanaa string_list states = STRING_LIST_INIT_DUP;
	sa_nracanaa strbuf start_head = STRBUF_INIT;
	sa_nracanaa strbuf bisect_names = STRBUF_INIT;
	sa_nracanaa object_id head_oid;
	sa_nracanaa object_id oid;
	ga_nanaa reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;
	const var_na *head;

	အကယ်၍ (is_bare_repository(the_repository))
		no_checkout = 1;

	/*
	 * Check for one bad and then some good revisions
	 */
	အတွက် (i = 0; i < argc; i++) {
		အကယ်၍ (!strcmp(argv[i], "--")) {
			has_double_dash = 1;
			bha_nga;
		}
	}

	အတွက် (i = 0; i < argc; i++) {
		const var_na *arg = argv[i];
		အကယ်၍ (!strcmp(argv[i], "--")) {
			bha_nga;
		} မဟုတ်ပါက အကယ်၍ (!strcmp(arg, "--no-checkout")) {
			no_checkout = 1;
		} မဟုတ်ပါက အကယ်၍ (!strcmp(arg, "--first-parent")) {
			first_parent_only = 1;
		} မဟုတ်ပါက အကယ်၍ (!strcmp(arg, "--reset-when-found")) {
			reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
		} မဟုတ်ပါက အကယ်၍ (skip_prefix(arg, "--reset-when-found=", &arg)) {
			အကယ်၍ (parse_reset_when_found(arg, &reset_when_found)) {
				res = BISECT_FAILED;
				goto finish;
			}
		} မဟုတ်ပါက အကယ်၍ (!strcmp(arg, "--term-good") ||
			 !strcmp(arg, "--term-old")) {
			i++;
			အကယ်၍ (argc <= i)
				ပြန်ပေး error(_("'' is not a valid term"));
			must_write_terms = 1;
			free((void *) terms->term_good);
			terms->term_good = xstrdup(argv[i]);
		} မဟုတ်ပါက အကယ်၍ (skip_prefix(arg, "--term-good=", &arg) ||
			   skip_prefix(arg, "--term-old=", &arg)) {
			must_write_terms = 1;
			free((void *) terms->term_good);
			terms->term_good = xstrdup(arg);
		} မဟုတ်ပါက အကယ်၍ (!strcmp(arg, "--term-bad") ||
			 !strcmp(arg, "--term-new")) {
			i++;
			အကယ်၍ (argc <= i)
				ပြန်ပေး error(_("'' is not a valid term"));
			must_write_terms = 1;
			free((void *) terms->term_bad);
			terms->term_bad = xstrdup(argv[i]);
		} မဟုတ်ပါက အကယ်၍ (skip_prefix(arg, "--term-bad=", &arg) ||
			   skip_prefix(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			free((void *) terms->term_bad);
			terms->term_bad = xstrdup(arg);
		} မဟုတ်ပါက အကယ်၍ (starts_with(arg, "--")) {
			ပြန်ပေး error(_("unrecognized option: '%s'"), arg);
		} မဟုတ်ပါက အကယ်၍ (!get_oidf(&oid, "%s^{commit}", arg)) {
			string_list_append(&revs, oid_to_hex(&oid));
		} မဟုတ်ပါက အကယ်၍ (has_double_dash) {
			die(_("'%s' does not appear to be a valid "
			      "revision"), arg);
		} မဟုတ်ပါက {
			bha_nga;
		}
	}
	အကယ်၍ (reset_when_found != RESET_WHEN_FOUND_NONE && no_checkout) {
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
	အကယ်၍ (revs.nr)
		must_write_terms = 1;
	အတွက် (i = 0; i < revs.nr; i++) {
		အကယ်၍ (bad_seen) {
			string_list_append(&states, terms->term_good);
		} မဟုတ်ပါက {
			bad_seen = 1;
			string_list_append(&states, terms->term_bad);
		}
	}

	/*
	 * Verify HEAD
	 */
	head = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
				       "HEAD", 0, &head_oid, &flags);
	အကယ်၍ (!head) {
		အကယ်၍ (repo_get_oid(the_repository, "HEAD", &head_oid))
			ပြန်ပေး error(_("bad HEAD - I need a HEAD"));
		head = "HEAD";
	}

	/*
	 * Check if we are bisecting
	 */
	အကယ်၍ (!is_empty_or_missing_file(git_path_bisect_start())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&start_head, git_path_bisect_start(), 0);
		strbuf_trim(&start_head);
		အကယ်၍ (!no_checkout) {
			sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.args, "checkout", start_head.buf,
				     "--", NULL);
			အကယ်၍ (run_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       start_head.buf);
				goto finish;
			}
		}
	} မဟုတ်ပါက {
		/* Get the rev from where we start. */
		အကယ်၍ (!repo_get_oid(the_repository, head, &head_oid) &&
		    !starts_with(head, "refs/heads/")) {
			strbuf_reset(&start_head);
			strbuf_add_oid_hex(&start_head, &head_oid);
		} မဟုတ်ပါက အကယ်၍ (!repo_get_oid(the_repository, head, &head_oid) &&
			   skip_prefix(head, "refs/heads/", &head)) {
			strbuf_addstr(&start_head, head);
		} မဟုတ်ပါက {
			ပြန်ပေး error(_("bad HEAD - strange symbolic ref"));
		}
	}

	/*
	 * Get rid of any old bisect state.
	 */
	အကယ်၍ (bisect_clean_state())
		ပြန်ပေး BISECT_FAILED;

	/*
	 * Write new start state
	 */
	write_file(git_path_bisect_start(), "%s\n", start_head.buf);

	အကယ်၍ (first_parent_only)
		write_file(git_path_bisect_first_parent(), "\n");

	အကယ်၍ (reset_when_found != RESET_WHEN_FOUND_NONE)
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));

	အကယ်၍ (no_checkout) {
		အကယ်၍ (repo_get_oid(the_repository, start_head.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), start_head.buf);
			goto finish;
		}
		အကယ်၍ (refs_update_ref(get_main_ref_store(the_repository), NULL, "BISECT_HEAD", &oid, NULL, 0,
				    UPDATE_REFS_MSG_ON_ERR)) {
			res = BISECT_FAILED;
			goto finish;
		}
	}

	အကယ်၍ (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_names, argv + pathspec_pos);
	write_file(git_path_bisect_names(), "%s\n", bisect_names.buf);

	အတွက် (i = 0; i < states.nr; i++)
		အကယ်၍ (bisect_write(states.items[i].string,
				 revs.items[i].string, terms, 1)) {
			res = BISECT_FAILED;
			goto finish;
		}

	အကယ်၍ (must_write_terms && write_terms(terms->term_bad,
					    terms->term_good)) {
		res = BISECT_FAILED;
		goto finish;
	}

	res = bisect_append_log_quoted(argv);
	အကယ်၍ (res)
		res = BISECT_FAILED;

finish:
	string_list_clear(&revs, 0);
	string_list_clear(&states, 0);
	strbuf_release(&start_head);
	strbuf_release(&bisect_names);
	အကယ်၍ (res)
		ပြန်ပေး res;

	res = bisect_auto_next(terms, NULL);
	အကယ်၍ (!is_bisect_success(res))
		bisect_clean_state();
	ပြန်ပေး res;
}

static anta_hstha ကိန်းပြည့် file_is_not_empty(const var_na *path)
{
	ပြန်ပေး !is_empty_or_missing_file(path);
}

static ကိန်းပြည့် bisect_autostart(sa_nracanaa bisect_terms *terms)
{
	ကိန်းပြည့် res;
	const var_na *yesno;

	အကယ်၍ (file_is_not_empty(git_path_bisect_start()))
		ပြန်ပေး 0;

	fprintf_ln(stderr, _("You need to start by \"git bisect "
			  "start\"\n"));

	အကယ်၍ (!isatty(STDIN_FILENO))
		ပြန်ပေး -1;

	/*
	 * TRANSLATORS: Make sure to include [Y] and [n] in your
	 * translation. The program will only accept English input
	 * at this point.
	 */
	yesno = git_prompt(_("Do you want me to do it for you "
			     "[Y/n]? "), PROMPT_ECHO);
	res = tolower(*yesno) == 'n' ?
		-1 : bisect_start(terms, 0, empty_strvec);

	ပြန်ပေး res;
}

static ga_nanaa bisect_error bisect_state(sa_nracanaa bisect_terms *terms, ကိန်းပြည့် argc,
				      const var_na **argv)
{
	const var_na *state;
	ကိန်းပြည့် i, verify_expected = 1;
	sa_nracanaa object_id oid, expected;
	sa_nracanaa oid_array revs = OID_ARRAY_INIT;

	အကယ်၍ (!argc)
		ပြန်ပေး error(_("Please call `--bisect-state` with at least one argument"));

	အကယ်၍ (bisect_autostart(terms))
		ပြန်ပေး BISECT_FAILED;

	state = argv[0];
	အကယ်၍ (check_and_set_terms(terms, state) ||
	    !one_of(state, terms->term_good, terms->term_bad, "skip", NULL))
		ပြန်ပေး BISECT_FAILED;

	argv++;
	argc--;
	အကယ်၍ (argc > 1 && !strcmp(state, terms->term_bad))
		ပြန်ပေး error(_("'git bisect %s' can take only one argument."), terms->term_bad);

	အကယ်၍ (argc == 0) {
		const var_na *head = "BISECT_HEAD";
		ga_nanaa get_oid_result res_head = repo_get_oid(the_repository,
							    head, &oid);

		အကယ်၍ (res_head == MISSING_OBJECT) {
			head = "HEAD";
			res_head = repo_get_oid(the_repository, head, &oid);
		}

		အကယ်၍ (res_head)
			error(_("Bad rev input: %s"), head);
		oid_array_append(&revs, &oid);
	}

	/*
	 * All input revs must be checked before executing bisect_write()
	 * to discard junk revs.
	 */

	အတွက် (; argc; argc--, argv++) {
		sa_nracanaa commit *commit;

		အကယ်၍ (repo_get_oid(the_repository, *argv, &oid)){
			error(_("Bad rev input: %s"), *argv);
			oid_array_clear(&revs);
			ပြန်ပေး BISECT_FAILED;
		}

		commit = lookup_commit_reference(the_repository, &oid);
		အကယ်၍ (!commit)
			die(_("Bad rev input (not a commit): %s"), *argv);

		oid_array_append(&revs, &commit->object.oid);
	}

	အကယ်၍ (refs_read_ref(get_main_ref_store(the_repository), "BISECT_EXPECTED_REV", &expected))
		verify_expected = 0; /* Ignore invalid file contents */

	အတွက် (i = 0; i < revs.nr; i++) {
		အကယ်၍ (bisect_write(state, oid_to_hex(&revs.oid[i]), terms, 0)) {
			oid_array_clear(&revs);
			ပြန်ပေး BISECT_FAILED;
		}
		အကယ်၍ (verify_expected && !oideq(&revs.oid[i], &expected)) {
			unlink_or_warn(git_path_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					NULL, "BISECT_EXPECTED_REV", NULL,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	ပြန်ပေး bisect_auto_next(terms, NULL);
}

static ga_nanaa bisect_error bisect_log(void)
{
	ကိန်းပြည့် fd, status;
	const var_na* filename = git_path_bisect_log();

	အကယ်၍ (is_empty_or_missing_file(filename))
		ပြန်ပေး error(_("We are not bisecting."));

	fd = open(filename, O_RDONLY);
	အကယ်၍ (fd < 0)
		ပြန်ပေး BISECT_FAILED;

	status = copy_fd(fd, STDOUT_FILENO);
	close(fd);
	ပြန်ပေး status ? BISECT_FAILED : BISECT_OK;
}

static ကိန်းပြည့် process_replay_line(sa_nracanaa bisect_terms *terms, sa_nracanaa strbuf *line)
{
	const var_na *p = line->buf + strspn(line->buf, " \t");
	var_na *word_end, *rev;

	အကယ်၍ ((!skip_prefix(p, "git bisect", &p) &&
	!skip_prefix(p, "git-bisect", &p)) || !isspace(*p))
		ပြန်ပေး 0;
	p += strspn(p, " \t");

	word_end = (var_na *)p + strcspn(p, " \t");
	rev = word_end + strspn(word_end, " \t");
	*word_end = '\0'; /* NUL-terminate the word */

	အကယ်၍ (get_terms(terms, 1))
		ပြန်ပေး error(_("no terms defined"));
	အကယ်၍ (check_and_set_terms(terms, p))
		ပြန်ပေး -1;

	အကယ်၍ (!strcmp(p, "start")) {
		sa_nracanaa strvec argv = STRVEC_INIT;
		ကိန်းပြည့် res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_start(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		ပြန်ပေး res;
	}

	အကယ်၍ (one_of(p, terms->term_good,
	   terms->term_bad, "skip", NULL))
		ပြန်ပေး bisect_write(p, rev, terms, 0);

	အကယ်၍ (!strcmp(p, "terms")) {
		sa_nracanaa strvec argv = STRVEC_INIT;
		ကိန်းပြည့် res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_terms(terms, argv.nr == 1 ? argv.v[0] : NULL);
		strvec_clear(&argv);
		ပြန်ပေး res;
	}
	error(_("'%s'?? what are you talking about?"), p);

	ပြန်ပေး -1;
}

static ga_nanaa bisect_error bisect_replay(sa_nracanaa bisect_terms *terms, const var_na *filename)
{
	FILE *fp = NULL;
	ga_nanaa bisect_error res = BISECT_OK;
	sa_nracanaa strbuf line = STRBUF_INIT;

	အကယ်၍ (is_empty_or_missing_file(filename))
		ပြန်ပေး error(_("cannot read file '%s' for replaying"), filename);

	အကယ်၍ (bisect_clean_state())
		ပြန်ပေး BISECT_FAILED;

	fp = fopen(filename, "r");
	အကယ်၍ (!fp)
		ပြန်ပေး BISECT_FAILED;

	နေစဉ် ((strbuf_getline(&line, fp) != EOF) && !res)
		res = process_replay_line(terms, &line);

	strbuf_release(&line);
	fclose(fp);

	အကယ်၍ (res)
		ပြန်ပေး BISECT_FAILED;

	ပြန်ပေး bisect_auto_next(terms, NULL);
}

static ga_nanaa bisect_error bisect_skip(sa_nracanaa bisect_terms *terms, ကိန်းပြည့် argc,
				     const var_na **argv)
{
	ကိန်းပြည့် i;
	ga_nanaa bisect_error res;
	sa_nracanaa strvec argv_state = STRVEC_INIT;

	strvec_push(&argv_state, "skip");

	အတွက် (i = 0; i < argc; i++) {
		const var_na *dotdot = strstr(argv[i], "..");

		အကယ်၍ (dotdot) {
			sa_nracanaa rev_info revs;
			sa_nracanaa commit *commit;

			repo_init_revisions(the_repository, &revs, NULL);
			setup_revisions(2, argv + i - 1, &revs, NULL);

			အကယ်၍ (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			နေစဉ် ((commit = get_revision(&revs)) != NULL)
				strvec_push(&argv_state,
						oid_to_hex(&commit->object.oid));

			reset_revision_walk();
			release_revisions(&revs);
		} မဟုတ်ပါက {
			strvec_push(&argv_state, argv[i]);
		}
	}
	res = bisect_state(terms, argv_state.nr, argv_state.v);

	strvec_clear(&argv_state);
	ပြန်ပေး res;
}

static ကိန်းပြည့် bisect_visualize(sa_nracanaa bisect_terms *terms, ကိန်းပြည့် argc,
			    const var_na **argv)
{
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;
	sa_nracanaa strbuf sb = STRBUF_INIT;

	အကယ်၍ (bisect_next_check(terms, NULL) != 0)
		ပြန်ပေး BISECT_FAILED;

	cmd.no_stdin = 1;
	အကယ်၍ (!argc) {
		အကယ်၍ ((getenv("DISPLAY") || getenv("SESSIONNAME") || getenv("MSYSTEM") ||
		     getenv("SECURITYSESSIONID")) && exists_in_PATH("gitk")) {
			strvec_push(&cmd.args, "gitk");
		} မဟုတ်ပါက {
			strvec_push(&cmd.args, "log");
			cmd.git_cmd = 1;
		}
	} မဟုတ်ပါက {
		အကယ်၍ (argv[0][0] == '-') {
			strvec_push(&cmd.args, "log");
			cmd.git_cmd = 1;
		} မဟုတ်ပါက အကယ်၍ (strcmp(argv[0], "tig") && !starts_with(argv[0], "git"))
			cmd.git_cmd = 1;

		strvec_pushv(&cmd.args, argv);
	}

	strvec_pushl(&cmd.args, "--bisect", "--", NULL);

	strbuf_read_file(&sb, git_path_bisect_names(), 0);
	sq_dequote_to_strvec(sb.buf, &cmd.args);
	strbuf_release(&sb);

	ပြန်ပေး run_command(&cmd);
}

static ကိန်းပြည့် get_first_good(const sa_nracanaa reference *ref, void *cb_data)
{
	oidcpy(cb_data, ref->oid);
	ပြန်ပေး 1;
}

static ကိန်းပြည့် do_bisect_run(const var_na *command)
{
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

	printf(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_push(&cmd.args, command);
	ပြန်ပေး run_command(&cmd);
}

static ကိန်းပြည့် verify_good(const sa_nracanaa bisect_terms *terms, const var_na *command)
{
	ကိန်းပြည့် rc;
	ga_nanaa bisect_error res;
	sa_nracanaa object_id good_rev;
	sa_nracanaa object_id current_rev;
	var_na *good_glob = xstrfmt("%s-*", terms->term_good);
	ကိန်းပြည့် no_checkout = refs_ref_exists(get_main_ref_store(the_repository),
					  "BISECT_HEAD");
	sa_nracanaa refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_first_good, &good_rev, &opts);
	free(good_glob);

	အကယ်၍ (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		ပြန်ပေး -1;

	res = bisect_checkout(&good_rev, no_checkout);
	အကယ်၍ (res != BISECT_OK)
		ပြန်ပေး -1;

	rc = do_bisect_run(command);

	res = bisect_checkout(&current_rev, no_checkout);
	အကယ်၍ (res != BISECT_OK)
		ပြန်ပေး -1;

	ပြန်ပေး rc;
}

static ကိန်းပြည့် bisect_run(sa_nracanaa bisect_terms *terms, ကိန်းပြည့် argc, const var_na **argv)
{
	ကိန်းပြည့် res = BISECT_OK;
	sa_nracanaa strbuf command = STRBUF_INIT;
	const var_na *reset_when_found_arg;
	const var_na *new_state;
	ကိန်းပြည့် temporary_stdout_fd, saved_stdout;
	ကိန်းပြည့် is_first_run = 1;
	ga_nanaa reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;

	အကယ်၍ (bisect_next_check(terms, NULL))
		ပြန်ပေး BISECT_FAILED;

	အကယ်၍ (argc && !strcmp(argv[0], "--reset-when-found")) {
		reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
	} မဟုတ်ပါက အကယ်၍ (argc && skip_prefix(argv[0], "--reset-when-found=",
				    &reset_when_found_arg)) {
		အကယ်၍ (parse_reset_when_found(reset_when_found_arg,
					   &reset_when_found))
			ပြန်ပေး BISECT_FAILED;
	}

	အကယ်၍ (reset_when_found != RESET_WHEN_FOUND_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		ပြန်ပေး error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	အကယ်၍ (reset_when_found != RESET_WHEN_FOUND_NONE) {
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));
		argc--;
		argv++;
	}

	အကယ်၍ (!argc) {
		error(_("bisect run failed: no command provided."));
		ပြန်ပေး BISECT_FAILED;
	}

	sq_quote_argv(&command, argv);
	strbuf_ltrim(&command);
	နေစဉ် (1) {
		res = do_bisect_run(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		အကယ်၍ (is_first_run && (res == 126 || res == 127)) {
			ကိန်းပြည့် rc = verify_good(terms, command.buf);
			is_first_run = 0;
			အကယ်၍ (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->term_good);
				res = BISECT_FAILED;
				bha_nga;
			}
			အကယ်၍ (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->term_good);
				res = BISECT_FAILED;
				bha_nga;
			}
		}

		အကယ်၍ (res < 0 || 128 <= res) {
			error(_("bisect run failed: exit code %d from"
				" %s is < 0 or >= 128"), res, command.buf);
			bha_nga;
		}

		အကယ်၍ (res == 125)
			new_state = "skip";
		မဟုတ်ပါက အကယ်၍ (!res)
			new_state = terms->term_good;
		မဟုတ်ပါက
			new_state = terms->term_bad;

		temporary_stdout_fd = open(git_path_bisect_run(), O_CREAT | O_WRONLY | O_TRUNC, 0666);

		အကယ်၍ (temporary_stdout_fd < 0) {
			res = error_errno(_("cannot open file '%s' for writing"), git_path_bisect_run());
			bha_nga;
		}

		fflush(stdout);
		saved_stdout = dup(1);
		အကယ်၍ (saved_stdout < 0 ||
		    dup2(temporary_stdout_fd, 1) < 0) {
			res = error_errno(_("could not duplicate stdout"));
			အကယ်၍ (saved_stdout >= 0)
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

		အကယ်၍ (res == BISECT_ONLY_SKIPPED_LEFT)
			error(_("bisect run cannot continue any more"));
		မဟုတ်ပါက အကယ်၍ (res == BISECT_INTERNAL_SUCCESS_MERGE_BASE) {
			puts(_("bisect run success"));
			res = BISECT_OK;
		} မဟုတ်ပါက အကယ်၍ (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
			printf(_("bisect found first '%s' commit\n"), terms->term_bad);
		} မဟုတ်ပါက အကယ်၍ (res) {
			error(_("bisect run failed: 'git bisect %s'"
				" exited with error code %d"), new_state, res);
		} မဟုတ်ပါက {
			continue;
		}
		bha_nga;
	}

	strbuf_release(&command);
	ပြန်ပေး res;
}

static ကိန်းပြည့် cmd_bisect__reset(ကိန်းပြည့် argc, const var_na **argv, const var_na *prefix UNUSED,
			     sa_nracanaa repository *repo UNUSED)
{
	ကိန်းပြည့် res;

	အကယ်၍ (argc > 1)
		ပြန်ပေး error(_("'%s' requires either no argument or a commit"),
			     "git bisect reset");
	res = bisect_reset(argc ? argv[0] : NULL, false);
	အကယ်၍ (res)
		ပြန်ပေး res;
	ပြန်ပေး bisect_clean_state();
}

static ကိန်းပြည့် cmd_bisect__terms(ကိန်းပြည့် argc, const var_na **argv, const var_na *prefix UNUSED,
			     sa_nracanaa repository *repo UNUSED)
{
	ကိန်းပြည့် res;
	sa_nracanaa bisect_terms terms = { 0 };

	အကယ်၍ (argc > 1)
		ပြန်ပေး error(_("'%s' requires 0 or 1 argument"),
			     "git bisect terms");
	res = bisect_terms(&terms, argc == 1 ? argv[0] : NULL);
	free_terms(&terms);
	ပြန်ပေး res;
}

static ကိန်းပြည့် cmd_bisect__start(ကိန်းပြည့် argc, const var_na **argv, const var_na *prefix UNUSED,
			     sa_nracanaa repository *repo UNUSED)
{
	ကိန်းပြည့် res;
	sa_nracanaa bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	res = bisect_start(&terms, argc, argv);
	free_terms(&terms);
	ပြန်ပေး res;
}

static ကိန်းပြည့် cmd_bisect__next(ကိန်းပြည့် argc, const var_na **argv UNUSED, const var_na *prefix,
			    sa_nracanaa repository *repo UNUSED)
{
	ကိန်းပြည့် res;
	sa_nracanaa bisect_terms terms = { 0 };

	အကယ်၍ (argc)
		ပြန်ပေး error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	အကယ်၍ (get_terms(&terms, 1))
		ပြန်ပေး error(_("no terms defined"));
	res = bisect_next(&terms, prefix);
	free_terms(&terms);
	ပြန်ပေး res;
}

static ကိန်းပြည့် cmd_bisect__log(ကိန်းပြည့် argc UNUSED, const var_na **argv UNUSED,
			   const var_na *prefix UNUSED,
			   sa_nracanaa repository *repo UNUSED)
{
	ပြန်ပေး bisect_log();
}

static ကိန်းပြည့် cmd_bisect__replay(ကိန်းပြည့် argc, const var_na **argv, const var_na *prefix UNUSED,
			      sa_nracanaa repository *repo UNUSED)
{
	ကိန်းပြည့် res;
	sa_nracanaa bisect_terms terms = { 0 };

	အကယ်၍ (argc != 1)
		ပြန်ပေး error(_("no logfile given"));
	set_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	ပြန်ပေး res;
}

static ကိန်းပြည့် cmd_bisect__skip(ကိန်းပြည့် argc, const var_na **argv, const var_na *prefix UNUSED,
			    sa_nracanaa repository *repo UNUSED)
{
	ကိန်းပြည့် res;
	sa_nracanaa bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	အကယ်၍ (get_terms(&terms, 1))
		ပြန်ပေး error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	ပြန်ပေး res;
}

static ကိန်းပြည့် cmd_bisect__visualize(ကိန်းပြည့် argc, const var_na **argv, const var_na *prefix UNUSED,
				 sa_nracanaa repository *repo UNUSED)
{
	ကိန်းပြည့် res;
	sa_nracanaa bisect_terms terms = { 0 };

	အကယ်၍ (get_terms(&terms, 1))
		ပြန်ပေး error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	ပြန်ပေး res;
}

static ကိန်းပြည့် cmd_bisect__run(ကိန်းပြည့် argc, const var_na **argv, const var_na *prefix UNUSED,
			   sa_nracanaa repository *repo UNUSED)
{
	ကိန်းပြည့် res;
	sa_nracanaa bisect_terms terms = { 0 };

	အကယ်၍ (!argc)
		ပြန်ပေး error(_("'%s' failed: no command provided."), "git bisect run");
	အကယ်၍ (get_terms(&terms, 1))
		ပြန်ပေး error(_("no terms defined"));
	res = bisect_run(&terms, argc, argv);
	free_terms(&terms);
	ပြန်ပေး res;
}

ကိန်းပြည့် cmd_bisect(ကိန်းပြည့် argc,
	       const var_na **argv,
	       const var_na *prefix,
	       sa_nracanaa repository *repo)
{
	ကိန်းပြည့် res = 0;
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

	အကယ်၍ (!fn) {
		sa_nracanaa bisect_terms terms = { 0 };

		အကယ်၍ (!argc)
			usage_msg_opt(_("need a command"), git_bisect_usage, options);

		အကယ်၍ (!strcmp(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		set_terms(&terms, "bad", "good");
		အကယ်၍ (get_terms(&terms, 1))
			ပြန်ပေး error(_("no terms defined"));
		အကယ်၍ (check_and_set_terms(&terms, argv[0]) ||
		    !one_of(argv[0], terms.term_good, terms.term_bad, NULL))
			usage_msg_optf(_("unknown command: '%s'"), git_bisect_usage,
				       options, argv[0]);
		မဟုတ်ပါက
			res = bisect_state(&terms, argc, argv);
		free_terms(&terms);
	} မဟုတ်ပါက {
		argc--;
		argv++;
		res = fn(argc, argv, prefix, repo);
	}

	အကယ်၍ (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		ga_nanaa reset_when_found_mode mode;

		အကယ်၍ (read_reset_when_found(&mode))
			res = BISECT_FAILED;
		မဟုတ်ပါက အကယ်၍ (mode != RESET_WHEN_FOUND_NONE &&
			 bisect_reset_when_found(mode))
			res = BISECT_FAILED;
	}

	ပြန်ပေး is_bisect_success(res) ? 0 : -res;
}
