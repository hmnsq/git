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

static const char * const git_bisect_usage[] = {
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

enum reset_when_found_mode {
	RESET_WHEN_FOUND_NONE,
	RESET_WHEN_FOUND_TO_ORIGINAL,
	RESET_WHEN_FOUND_TO_FOUND,
};

struct add_bisect_ref_data {
	struct rev_info *revs;
	unsigned цяло object_flags;
};

struct bisect_terms {
	char *term_good;
	char *term_bad;
};

static void free_terms(struct bisect_terms *terms)
{
	FREE_AND_NULL(terms->term_good);
	FREE_AND_NULL(terms->term_bad);
}

static void set_terms(struct bisect_terms *terms, const char *bad,
		      const char *good)
{
	free((void *)terms->term_good);
	terms->term_good = xstrdup(good);
	free((void *)terms->term_bad);
	terms->term_bad = xstrdup(bad);
}

static const char vocab_bad[] = "bad|new";
static const char vocab_good[] = "good|old";

static цяло bisect_autostart(struct bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
LAST_ARG_MUST_BE_NULL
static цяло one_of(const char *term, ...)
{
	цяло res = 0;
	va_list matches;
	const char *match;

	va_start(matches, term);
	докато (!res && (match = va_arg(matches, const char *)))
		res = !strcmp(term, match);
	va_end(matches);

	върни res;
}

/*
 * return code BISECT_INTERNAL_SUCCESS_MERGE_BASE
 * and BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND are codes
 * that indicate special success.
 */

static цяло is_bisect_success(enum bisect_error res)
{
	върни !res ||
		res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND ||
		res == BISECT_INTERNAL_SUCCESS_MERGE_BASE;
}

static цяло write_in_file(const char *path, const char *mode, const char *format, va_list args)
{
	FILE *fp = NULL;
	цяло res = 0;

	ако (strcmp(mode, "w") && strcmp(mode, "a"))
		BUG("write-in-file does not support '%s' mode", mode);
	fp = fopen(path, mode);
	ако (!fp)
		върни error_errno(_("cannot open file '%s' in mode '%s'"), path, mode);
	res = vfprintf(fp, format, args);

	ако (res < 0) {
		цяло saved_errno = errno;
		fclose(fp);
		errno = saved_errno;
		върни error_errno(_("could not write to file '%s'"), path);
	}

	върни fclose(fp);
}

__attribute__((format (printf, 2, 3)))
static цяло write_to_file(const char *path, const char *format, ...)
{
	цяло res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "w", format, args);
	va_end(args);

	върни res;
}

__attribute__((format (printf, 2, 3)))
static цяло append_to_file(const char *path, const char *format, ...)
{
	цяло res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "a", format, args);
	va_end(args);

	върни res;
}

static цяло print_file_to_stdout(const char *path)
{
	цяло fd = open(path, O_RDONLY);
	цяло ret = 0;

	ако (fd < 0)
		върни error_errno(_("cannot open file '%s' for reading"), path);
	ако (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), path);
	close(fd);
	върни ret;
}

static цяло check_term_format(const char *term, const char *orig_term)
{
	цяло res;
	char *new_term = xstrfmt("refs/bisect/%s", term);

	res = check_refname_format(new_term, 0);
	free(new_term);

	ако (res)
		върни error(_("'%s' is not a valid term"), term);

	ако (one_of(term, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", NULL))
		върни error(_("can't use the builtin command '%s' as a term"), term);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	ако ((strcmp(orig_term, "bad") && one_of(term, "bad", "new", NULL)) ||
		 (strcmp(orig_term, "good") && one_of(term, "good", "old", NULL)))
		върни error(_("can't change the meaning of the term '%s'"), term);

	върни 0;
}

static цяло write_terms(const char *bad, const char *good)
{
	цяло res;

	ако (!strcmp(bad, good))
		върни error(_("please use two different terms"));

	ако (check_term_format(bad, "bad") || check_term_format(good, "good"))
		върни -1;

	res = write_to_file(git_path_bisect_terms(), "%s\n%s\n", bad, good);

	върни res;
}

static цяло bisect_reset(const char *commit, bool quiet)
{
	struct strbuf branch = STRBUF_INIT;

	ако (!commit) {
		ако (!strbuf_read_file(&branch, git_path_bisect_start(), 0))
			printf(_("We are not bisecting.\n"));
		иначе
			strbuf_rtrim(&branch);
	} иначе {
		struct object_id oid;

		ако (repo_get_oid_commit(the_repository, commit, &oid))
			върни error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	ако (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		struct child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "checkout", "--ignore-other-worktrees", NULL);
		ако (quiet)
			strvec_push(&cmd.args, "--quiet");
		strvec_pushl(&cmd.args, branch.buf, "--", NULL);
		ако (run_command(&cmd)) {
			error(_("could not check out original"
				" HEAD '%s'. Try 'git bisect"
				" reset <commit>'."), branch.buf);
			strbuf_release(&branch);
			върни -1;
		}
	}

	strbuf_release(&branch);
	върни 0;
}

static цяло parse_reset_when_found(const char *value,
				  enum reset_when_found_mode *mode)
{
	ако (!strcmp(value, "original"))
		*mode = RESET_WHEN_FOUND_TO_ORIGINAL;
	иначе ако (!strcmp(value, "found"))
		*mode = RESET_WHEN_FOUND_TO_FOUND;
	иначе
		върни error(_("invalid value for '--reset-when-found': '%s'"),
			     value);

	върни 0;
}

static const char *reset_when_found_mode_name(enum reset_when_found_mode mode)
{
	switch (mode) {
	case RESET_WHEN_FOUND_TO_ORIGINAL:
		върни "original";
	case RESET_WHEN_FOUND_TO_FOUND:
		върни "found";
	case RESET_WHEN_FOUND_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", mode);
}

static цяло read_reset_when_found(enum reset_when_found_mode *mode)
{
	struct strbuf value = STRBUF_INIT;
	цяло res = 0;

	*mode = RESET_WHEN_FOUND_NONE;
	ако (is_empty_or_missing_file(git_path_bisect_reset_when_found()))
		върни 0;

	ако (strbuf_read_file(&value, git_path_bisect_reset_when_found(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_path_bisect_reset_when_found());
		goto out;
	}
	strbuf_trim(&value);
	ако (parse_reset_when_found(value.buf, mode))
		res = -1;

out:
	strbuf_release(&value);
	върни res;
}

static цяло bisect_reset_when_found(enum reset_when_found_mode mode)
{
	struct bisect_terms terms = { 0 };
	char *commit = NULL;
	цяло res;

	ако (mode == RESET_WHEN_FOUND_TO_FOUND) {
		read_bisect_terms(&terms.term_bad, &terms.term_good);
		commit = xstrfmt("refs/bisect/%s", terms.term_bad);
	} иначе ако (mode == RESET_WHEN_FOUND_NONE) {
		BUG("automatic reset requested without a reset mode");
	}

	res = bisect_reset(commit, true);
	ако (!res)
		res = bisect_clean_state();

	free(commit);
	free_terms(&terms);
	върни res;
}

static void log_commit(FILE *fp,
		       const char *fmt, const char *state,
		       struct commit *commit)
{
	struct pretty_print_context pp = {0};
	struct strbuf commit_msg = STRBUF_INIT;
	char *label = xstrfmt(fmt, state);

	repo_format_commit_message(the_repository, commit, "%s", &commit_msg,
				   &pp);

	fprintf(fp, "# %s: [%s] %s\n", label, oid_to_hex(&commit->object.oid),
		commit_msg.buf);

	strbuf_release(&commit_msg);
	free(label);
}

static цяло bisect_write(const char *state, const char *rev,
			const struct bisect_terms *terms, цяло nolog)
{
	struct strbuf tag = STRBUF_INIT;
	struct object_id oid;
	struct commit *commit;
	FILE *fp = NULL;
	цяло res = 0;

	ако (!strcmp(state, terms->term_bad)) {
		strbuf_addf(&tag, "refs/bisect/%s", state);
	} иначе ако (one_of(state, terms->term_good, "skip", NULL)) {
		strbuf_addf(&tag, "refs/bisect/%s-%s", state, rev);
	} иначе {
		res = error(_("Bad bisect_write argument: %s"), state);
		goto finish;
	}

	ако (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		goto finish;
	}

	ако (refs_update_ref(get_main_ref_store(the_repository), NULL, tag.buf, &oid, NULL, 0,
			    UPDATE_REFS_MSG_ON_ERR)) {
		res = -1;
		goto finish;
	}

	fp = fopen(git_path_bisect_log(), "a");
	ако (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_path_bisect_log());
		goto finish;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	ако (!nolog)
		fprintf(fp, "git bisect %s %s\n", state, rev);

finish:
	ако (fp)
		fclose(fp);
	strbuf_release(&tag);
	върни res;
}

static цяло check_and_set_terms(struct bisect_terms *terms, const char *cmd)
{
	цяло has_term_file = !is_empty_or_missing_file(git_path_bisect_terms());

	ако (one_of(cmd, "skip", "start", "terms", NULL))
		върни 0;

	ако (has_term_file && strcmp(cmd, terms->term_bad) &&
	    strcmp(cmd, terms->term_good))
		върни error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->term_bad,
				terms->term_good);

	ако (!has_term_file) {
		ако (one_of(cmd, "bad", "good", NULL)) {
			set_terms(terms, "bad", "good");
			върни write_terms(terms->term_bad, terms->term_good);
		}
		ако (one_of(cmd, "new", "old", NULL)) {
			set_terms(terms, "new", "old");
			върни write_terms(terms->term_bad, terms->term_good);
		}
	}

	върни 0;
}

static цяло inc_nr(const struct reference *ref UNUSED, void *cb_data)
{
	unsigned цяло *nr = (unsigned цяло *)cb_data;
	(*nr)++;
	върни 0;
}

static const char need_bad_and_good_revision_warning[] =
	N_("You need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

static const char need_bisect_start_warning[] =
	N_("You need to start by \"git bisect start\".\n"
	   "You then need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

static цяло decide_next(const struct bisect_terms *terms,
		       const char *current_term, цяло missing_good,
		       цяло missing_bad)
{
	ако (!missing_good && !missing_bad)
		върни 0;
	ако (!current_term)
		върни -1;

	ако (missing_good && !missing_bad &&
	    !strcmp(current_term, terms->term_good)) {
		char *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->term_bad);
		ако (!isatty(0))
			върни 0;
		/*
		 * TRANSLATORS: Make sure to include [Y] and [n] in your
		 * translation. The program will only accept English input
		 * at this point.
		 */
		yesno = git_prompt(_("Are you sure [Y/n]? "), PROMPT_ECHO);
		ако (starts_with(yesno, "N") || starts_with(yesno, "n"))
			върни -1;
		върни 0;
	}

	ако (!is_empty_or_missing_file(git_path_bisect_start()))
		върни error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	иначе
		върни error(_(need_bisect_start_warning),
			     vocab_good, vocab_bad, vocab_good, vocab_bad);
}

static void bisect_status(struct bisect_state *state,
			  const struct bisect_terms *terms)
{
	char *bad_ref = xstrfmt("refs/bisect/%s", terms->term_bad);
	char *good_glob = xstrfmt("%s-*", terms->term_good);
	struct refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	ако (refs_ref_exists(get_main_ref_store(the_repository), bad_ref))
		state->nr_bad = 1;

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      inc_nr, &state->nr_good, &opts);

	free(good_glob);
	free(bad_ref);
}

__attribute__((format (printf, 1, 2)))
static void bisect_log_printf(const char *fmt, ...)
{
	struct strbuf buf = STRBUF_INIT;
	va_list ap;

	va_start(ap, fmt);
	strbuf_vaddf(&buf, fmt, ap);
	va_end(ap);

	printf("%s", buf.buf);
	append_to_file(git_path_bisect_log(), "# %s", buf.buf);

	strbuf_release(&buf);
}

static void bisect_print_status(const struct bisect_terms *terms)
{
	struct bisect_state state = { 0 };

	bisect_status(&state, terms);

	/* If we had both, we'd already be started, and shouldn't get here. */
	ако (state.nr_good && state.nr_bad)
		върни;

	ако (!state.nr_good && !state.nr_bad)
		bisect_log_printf(_("status: waiting for both '%s' and '%s' commits\n"),
				  terms->term_good, terms->term_bad);
	иначе ако (state.nr_good)
		bisect_log_printf(Q_("status: waiting for '%s' commit, %d '%s' commit known\n",
				     "status: waiting for '%s' commit, %d '%s' commits known\n",
				     state.nr_good),
				  terms->term_bad, state.nr_good, terms->term_good);
	иначе
		bisect_log_printf(_("status: waiting for '%s' commit(s), '%s' commit known\n"),
				  terms->term_good, terms->term_bad);
}

static цяло bisect_next_check(const struct bisect_terms *terms,
			     const char *current_term)
{
	struct bisect_state state = { 0 };
	bisect_status(&state, terms);
	върни decide_next(terms, current_term, !state.nr_good, !state.nr_bad);
}

static цяло get_terms(struct bisect_terms *terms, цяло file_missing_is_ok)
{
	struct strbuf str = STRBUF_INIT;
	FILE *fp = NULL;
	цяло res = 0;

	fp = fopen(git_path_bisect_terms(), "r");
	ако (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		goto finish;
	}

	free_terms(terms);
	ако (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		goto finish;
	}
	terms->term_bad = strbuf_detach(&str, NULL);
	ако (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		FREE_AND_NULL(terms->term_bad);
		goto finish;
	}
	terms->term_good = strbuf_detach(&str, NULL);

finish:
	ако (fp)
		fclose(fp);
	strbuf_release(&str);
	върни res;
}

static цяло bisect_terms(struct bisect_terms *terms, const char *option)
{
	ако (get_terms(terms, 0))
		върни error(_("no terms defined"));

	ако (!option) {
		printf(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->term_good, terms->term_bad);
		върни 0;
	}
	ако (one_of(option, "--term-good", "--term-old", NULL))
		printf("%s\n", terms->term_good);
	иначе ако (one_of(option, "--term-bad", "--term-new", NULL))
		printf("%s\n", terms->term_bad);
	иначе
		върни error(_("invalid argument %s for 'git bisect terms'.\n"
			       "Supported options are: "
			       "--term-good|--term-old and "
			       "--term-bad|--term-new."), option);

	върни 0;
}

static цяло bisect_append_log_quoted(const char **argv)
{
	цяло res = 0;
	FILE *fp = fopen(git_path_bisect_log(), "a");
	struct strbuf orig_args = STRBUF_INIT;

	ако (!fp)
		върни -1;

	ако (fprintf(fp, "git bisect start") < 1) {
		res = -1;
		goto finish;
	}

	sq_quote_argv(&orig_args, argv);
	ако (fprintf(fp, "%s\n", orig_args.buf) < 1)
		res = -1;

finish:
	fclose(fp);
	strbuf_release(&orig_args);
	върни res;
}

static цяло add_bisect_ref(const struct reference *ref, void *cb)
{
	struct add_bisect_ref_data *data = cb;

	add_pending_oid(data->revs, ref->name, ref->oid, data->object_flags);

	върни 0;
}

static цяло prepare_revs(struct bisect_terms *terms, struct rev_info *revs)
{
	struct refs_for_each_ref_options opts = {
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};
	цяло res = 0;
	struct add_bisect_ref_data cb = { revs };
	char *good = xstrfmt("%s-*", terms->term_good);

	/*
	 * We cannot use terms->term_bad directly in
	 * for_each_glob_ref_in() and we have to append a '*' to it,
	 * otherwise for_each_glob_ref_in() will append '/' and '*'.
	 */
	char *bad = xstrfmt("%s*", terms->term_bad);

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

	ако (prepare_revision_walk(revs))
		res = error(_("revision walk setup failed"));

	free(good);
	free(bad);
	върни res;
}

static цяло bisect_skipped_commits(struct bisect_terms *terms)
{
	цяло res;
	FILE *fp = NULL;
	struct rev_info revs;
	struct commit *commit;
	struct pretty_print_context pp = {0};
	struct strbuf commit_name = STRBUF_INIT;

	res = prepare_revs(terms, &revs);
	ако (res)
		върни res;

	fp = fopen(git_path_bisect_log(), "a");
	ако (!fp)
		върни error_errno(_("could not open '%s' for appending"),
				  git_path_bisect_log());

	ако (fprintf(fp, "# only skipped commits left to test\n") < 0)
		върни error_errno(_("failed to write to '%s'"), git_path_bisect_log());

	докато ((commit = get_revision(&revs)) != NULL) {
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
	върни 0;
}

static цяло bisect_successful(struct bisect_terms *terms)
{
	struct object_id oid;
	struct commit *commit;
	struct pretty_print_context pp = {0};
	struct strbuf commit_name = STRBUF_INIT;
	char *bad_ref = xstrfmt("refs/bisect/%s",terms->term_bad);
	цяло res;

	refs_read_ref(get_main_ref_store(the_repository), bad_ref, &oid);
	commit = lookup_commit_reference_by_name(bad_ref);
	ако (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		free(bad_ref);
		върни BISECT_FAILED;
	}
	repo_format_commit_message(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_to_file(git_path_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->term_bad, oid_to_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	free(bad_ref);
	върни res;
}

static enum bisect_error bisect_next(struct bisect_terms *terms,
				     const char *prefix)
{
	enum bisect_error res;

	ако (bisect_autostart(terms))
		върни BISECT_FAILED;

	ако (bisect_next_check(terms, terms->term_good))
		върни BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_next_all(the_repository, prefix);

	ако (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		res = bisect_successful(terms);
		върни res ? res : BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND;
	} иначе ако (res == BISECT_ONLY_SKIPPED_LEFT) {
		res = bisect_skipped_commits(terms);
		върни res ? res : BISECT_ONLY_SKIPPED_LEFT;
	}
	върни res;
}

static enum bisect_error bisect_auto_next(struct bisect_terms *terms,
					  const char *prefix)
{
	ако (bisect_next_check(terms, NULL)) {
		bisect_print_status(terms);
		върни BISECT_OK;
	}

	върни bisect_next(terms, prefix);
}

static enum bisect_error bisect_start(struct bisect_terms *terms, цяло argc,
				      const char **argv)
{
	цяло no_checkout = 0;
	цяло first_parent_only = 0;
	цяло i, has_double_dash = 0, must_write_terms = 0, bad_seen = 0;
	цяло flags, pathspec_pos;
	enum bisect_error res = BISECT_OK;
	struct string_list revs = STRING_LIST_INIT_DUP;
	struct string_list states = STRING_LIST_INIT_DUP;
	struct strbuf start_head = STRBUF_INIT;
	struct strbuf bisect_names = STRBUF_INIT;
	struct object_id head_oid;
	struct object_id oid;
	enum reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;
	const char *head;

	ако (is_bare_repository(the_repository))
		no_checkout = 1;

	/*
	 * Check for one bad and then some good revisions
	 */
	за (i = 0; i < argc; i++) {
		ако (!strcmp(argv[i], "--")) {
			has_double_dash = 1;
			break;
		}
	}

	за (i = 0; i < argc; i++) {
		const char *arg = argv[i];
		ако (!strcmp(argv[i], "--")) {
			break;
		} иначе ако (!strcmp(arg, "--no-checkout")) {
			no_checkout = 1;
		} иначе ако (!strcmp(arg, "--first-parent")) {
			first_parent_only = 1;
		} иначе ако (!strcmp(arg, "--reset-when-found")) {
			reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
		} иначе ако (skip_prefix(arg, "--reset-when-found=", &arg)) {
			ако (parse_reset_when_found(arg, &reset_when_found)) {
				res = BISECT_FAILED;
				goto finish;
			}
		} иначе ако (!strcmp(arg, "--term-good") ||
			 !strcmp(arg, "--term-old")) {
			i++;
			ако (argc <= i)
				върни error(_("'' is not a valid term"));
			must_write_terms = 1;
			free((void *) terms->term_good);
			terms->term_good = xstrdup(argv[i]);
		} иначе ако (skip_prefix(arg, "--term-good=", &arg) ||
			   skip_prefix(arg, "--term-old=", &arg)) {
			must_write_terms = 1;
			free((void *) terms->term_good);
			terms->term_good = xstrdup(arg);
		} иначе ако (!strcmp(arg, "--term-bad") ||
			 !strcmp(arg, "--term-new")) {
			i++;
			ако (argc <= i)
				върни error(_("'' is not a valid term"));
			must_write_terms = 1;
			free((void *) terms->term_bad);
			terms->term_bad = xstrdup(argv[i]);
		} иначе ако (skip_prefix(arg, "--term-bad=", &arg) ||
			   skip_prefix(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			free((void *) terms->term_bad);
			terms->term_bad = xstrdup(arg);
		} иначе ако (starts_with(arg, "--")) {
			върни error(_("unrecognized option: '%s'"), arg);
		} иначе ако (!get_oidf(&oid, "%s^{commit}", arg)) {
			string_list_append(&revs, oid_to_hex(&oid));
		} иначе ако (has_double_dash) {
			die(_("'%s' does not appear to be a valid "
			      "revision"), arg);
		} иначе {
			break;
		}
	}
	ако (reset_when_found != RESET_WHEN_FOUND_NONE && no_checkout) {
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
	ако (revs.nr)
		must_write_terms = 1;
	за (i = 0; i < revs.nr; i++) {
		ако (bad_seen) {
			string_list_append(&states, terms->term_good);
		} иначе {
			bad_seen = 1;
			string_list_append(&states, terms->term_bad);
		}
	}

	/*
	 * Verify HEAD
	 */
	head = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
				       "HEAD", 0, &head_oid, &flags);
	ако (!head) {
		ако (repo_get_oid(the_repository, "HEAD", &head_oid))
			върни error(_("bad HEAD - I need a HEAD"));
		head = "HEAD";
	}

	/*
	 * Check if we are bisecting
	 */
	ако (!is_empty_or_missing_file(git_path_bisect_start())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&start_head, git_path_bisect_start(), 0);
		strbuf_trim(&start_head);
		ако (!no_checkout) {
			struct child_process cmd = CHILD_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.args, "checkout", start_head.buf,
				     "--", NULL);
			ако (run_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       start_head.buf);
				goto finish;
			}
		}
	} иначе {
		/* Get the rev from where we start. */
		ако (!repo_get_oid(the_repository, head, &head_oid) &&
		    !starts_with(head, "refs/heads/")) {
			strbuf_reset(&start_head);
			strbuf_add_oid_hex(&start_head, &head_oid);
		} иначе ако (!repo_get_oid(the_repository, head, &head_oid) &&
			   skip_prefix(head, "refs/heads/", &head)) {
			strbuf_addstr(&start_head, head);
		} иначе {
			върни error(_("bad HEAD - strange symbolic ref"));
		}
	}

	/*
	 * Get rid of any old bisect state.
	 */
	ако (bisect_clean_state())
		върни BISECT_FAILED;

	/*
	 * Write new start state
	 */
	write_file(git_path_bisect_start(), "%s\n", start_head.buf);

	ако (first_parent_only)
		write_file(git_path_bisect_first_parent(), "\n");

	ако (reset_when_found != RESET_WHEN_FOUND_NONE)
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));

	ако (no_checkout) {
		ако (repo_get_oid(the_repository, start_head.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), start_head.buf);
			goto finish;
		}
		ако (refs_update_ref(get_main_ref_store(the_repository), NULL, "BISECT_HEAD", &oid, NULL, 0,
				    UPDATE_REFS_MSG_ON_ERR)) {
			res = BISECT_FAILED;
			goto finish;
		}
	}

	ако (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_names, argv + pathspec_pos);
	write_file(git_path_bisect_names(), "%s\n", bisect_names.buf);

	за (i = 0; i < states.nr; i++)
		ако (bisect_write(states.items[i].string,
				 revs.items[i].string, terms, 1)) {
			res = BISECT_FAILED;
			goto finish;
		}

	ако (must_write_terms && write_terms(terms->term_bad,
					    terms->term_good)) {
		res = BISECT_FAILED;
		goto finish;
	}

	res = bisect_append_log_quoted(argv);
	ако (res)
		res = BISECT_FAILED;

finish:
	string_list_clear(&revs, 0);
	string_list_clear(&states, 0);
	strbuf_release(&start_head);
	strbuf_release(&bisect_names);
	ако (res)
		върни res;

	res = bisect_auto_next(terms, NULL);
	ако (!is_bisect_success(res))
		bisect_clean_state();
	върни res;
}

static inline цяло file_is_not_empty(const char *path)
{
	върни !is_empty_or_missing_file(path);
}

static цяло bisect_autostart(struct bisect_terms *terms)
{
	цяло res;
	const char *yesno;

	ако (file_is_not_empty(git_path_bisect_start()))
		върни 0;

	fprintf_ln(stderr, _("You need to start by \"git bisect "
			  "start\"\n"));

	ако (!isatty(STDIN_FILENO))
		върни -1;

	/*
	 * TRANSLATORS: Make sure to include [Y] and [n] in your
	 * translation. The program will only accept English input
	 * at this point.
	 */
	yesno = git_prompt(_("Do you want me to do it for you "
			     "[Y/n]? "), PROMPT_ECHO);
	res = tolower(*yesno) == 'n' ?
		-1 : bisect_start(terms, 0, empty_strvec);

	върни res;
}

static enum bisect_error bisect_state(struct bisect_terms *terms, цяло argc,
				      const char **argv)
{
	const char *state;
	цяло i, verify_expected = 1;
	struct object_id oid, expected;
	struct oid_array revs = OID_ARRAY_INIT;

	ако (!argc)
		върни error(_("Please call `--bisect-state` with at least one argument"));

	ако (bisect_autostart(terms))
		върни BISECT_FAILED;

	state = argv[0];
	ако (check_and_set_terms(terms, state) ||
	    !one_of(state, terms->term_good, terms->term_bad, "skip", NULL))
		върни BISECT_FAILED;

	argv++;
	argc--;
	ако (argc > 1 && !strcmp(state, terms->term_bad))
		върни error(_("'git bisect %s' can take only one argument."), terms->term_bad);

	ако (argc == 0) {
		const char *head = "BISECT_HEAD";
		enum get_oid_result res_head = repo_get_oid(the_repository,
							    head, &oid);

		ако (res_head == MISSING_OBJECT) {
			head = "HEAD";
			res_head = repo_get_oid(the_repository, head, &oid);
		}

		ако (res_head)
			error(_("Bad rev input: %s"), head);
		oid_array_append(&revs, &oid);
	}

	/*
	 * All input revs must be checked before executing bisect_write()
	 * to discard junk revs.
	 */

	за (; argc; argc--, argv++) {
		struct commit *commit;

		ако (repo_get_oid(the_repository, *argv, &oid)){
			error(_("Bad rev input: %s"), *argv);
			oid_array_clear(&revs);
			върни BISECT_FAILED;
		}

		commit = lookup_commit_reference(the_repository, &oid);
		ако (!commit)
			die(_("Bad rev input (not a commit): %s"), *argv);

		oid_array_append(&revs, &commit->object.oid);
	}

	ако (refs_read_ref(get_main_ref_store(the_repository), "BISECT_EXPECTED_REV", &expected))
		verify_expected = 0; /* Ignore invalid file contents */

	за (i = 0; i < revs.nr; i++) {
		ако (bisect_write(state, oid_to_hex(&revs.oid[i]), terms, 0)) {
			oid_array_clear(&revs);
			върни BISECT_FAILED;
		}
		ако (verify_expected && !oideq(&revs.oid[i], &expected)) {
			unlink_or_warn(git_path_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					NULL, "BISECT_EXPECTED_REV", NULL,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	върни bisect_auto_next(terms, NULL);
}

static enum bisect_error bisect_log(void)
{
	цяло fd, status;
	const char* filename = git_path_bisect_log();

	ако (is_empty_or_missing_file(filename))
		върни error(_("We are not bisecting."));

	fd = open(filename, O_RDONLY);
	ако (fd < 0)
		върни BISECT_FAILED;

	status = copy_fd(fd, STDOUT_FILENO);
	close(fd);
	върни status ? BISECT_FAILED : BISECT_OK;
}

static цяло process_replay_line(struct bisect_terms *terms, struct strbuf *line)
{
	const char *p = line->buf + strspn(line->buf, " \t");
	char *word_end, *rev;

	ако ((!skip_prefix(p, "git bisect", &p) &&
	!skip_prefix(p, "git-bisect", &p)) || !isspace(*p))
		върни 0;
	p += strspn(p, " \t");

	word_end = (char *)p + strcspn(p, " \t");
	rev = word_end + strspn(word_end, " \t");
	*word_end = '\0'; /* NUL-terminate the word */

	ако (get_terms(terms, 1))
		върни error(_("no terms defined"));
	ако (check_and_set_terms(terms, p))
		върни -1;

	ако (!strcmp(p, "start")) {
		struct strvec argv = STRVEC_INIT;
		цяло res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_start(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		върни res;
	}

	ако (one_of(p, terms->term_good,
	   terms->term_bad, "skip", NULL))
		върни bisect_write(p, rev, terms, 0);

	ако (!strcmp(p, "terms")) {
		struct strvec argv = STRVEC_INIT;
		цяло res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_terms(terms, argv.nr == 1 ? argv.v[0] : NULL);
		strvec_clear(&argv);
		върни res;
	}
	error(_("'%s'?? what are you talking about?"), p);

	върни -1;
}

static enum bisect_error bisect_replay(struct bisect_terms *terms, const char *filename)
{
	FILE *fp = NULL;
	enum bisect_error res = BISECT_OK;
	struct strbuf line = STRBUF_INIT;

	ако (is_empty_or_missing_file(filename))
		върни error(_("cannot read file '%s' for replaying"), filename);

	ако (bisect_clean_state())
		върни BISECT_FAILED;

	fp = fopen(filename, "r");
	ако (!fp)
		върни BISECT_FAILED;

	докато ((strbuf_getline(&line, fp) != EOF) && !res)
		res = process_replay_line(terms, &line);

	strbuf_release(&line);
	fclose(fp);

	ако (res)
		върни BISECT_FAILED;

	върни bisect_auto_next(terms, NULL);
}

static enum bisect_error bisect_skip(struct bisect_terms *terms, цяло argc,
				     const char **argv)
{
	цяло i;
	enum bisect_error res;
	struct strvec argv_state = STRVEC_INIT;

	strvec_push(&argv_state, "skip");

	за (i = 0; i < argc; i++) {
		const char *dotdot = strstr(argv[i], "..");

		ако (dotdot) {
			struct rev_info revs;
			struct commit *commit;

			repo_init_revisions(the_repository, &revs, NULL);
			setup_revisions(2, argv + i - 1, &revs, NULL);

			ако (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			докато ((commit = get_revision(&revs)) != NULL)
				strvec_push(&argv_state,
						oid_to_hex(&commit->object.oid));

			reset_revision_walk();
			release_revisions(&revs);
		} иначе {
			strvec_push(&argv_state, argv[i]);
		}
	}
	res = bisect_state(terms, argv_state.nr, argv_state.v);

	strvec_clear(&argv_state);
	върни res;
}

static цяло bisect_visualize(struct bisect_terms *terms, цяло argc,
			    const char **argv)
{
	struct child_process cmd = CHILD_PROCESS_INIT;
	struct strbuf sb = STRBUF_INIT;

	ако (bisect_next_check(terms, NULL) != 0)
		върни BISECT_FAILED;

	cmd.no_stdin = 1;
	ако (!argc) {
		ако ((getenv("DISPLAY") || getenv("SESSIONNAME") || getenv("MSYSTEM") ||
		     getenv("SECURITYSESSIONID")) && exists_in_PATH("gitk")) {
			strvec_push(&cmd.args, "gitk");
		} иначе {
			strvec_push(&cmd.args, "log");
			cmd.git_cmd = 1;
		}
	} иначе {
		ако (argv[0][0] == '-') {
			strvec_push(&cmd.args, "log");
			cmd.git_cmd = 1;
		} иначе ако (strcmp(argv[0], "tig") && !starts_with(argv[0], "git"))
			cmd.git_cmd = 1;

		strvec_pushv(&cmd.args, argv);
	}

	strvec_pushl(&cmd.args, "--bisect", "--", NULL);

	strbuf_read_file(&sb, git_path_bisect_names(), 0);
	sq_dequote_to_strvec(sb.buf, &cmd.args);
	strbuf_release(&sb);

	върни run_command(&cmd);
}

static цяло get_first_good(const struct reference *ref, void *cb_data)
{
	oidcpy(cb_data, ref->oid);
	върни 1;
}

static цяло do_bisect_run(const char *command)
{
	struct child_process cmd = CHILD_PROCESS_INIT;

	printf(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_push(&cmd.args, command);
	върни run_command(&cmd);
}

static цяло verify_good(const struct bisect_terms *terms, const char *command)
{
	цяло rc;
	enum bisect_error res;
	struct object_id good_rev;
	struct object_id current_rev;
	char *good_glob = xstrfmt("%s-*", terms->term_good);
	цяло no_checkout = refs_ref_exists(get_main_ref_store(the_repository),
					  "BISECT_HEAD");
	struct refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_first_good, &good_rev, &opts);
	free(good_glob);

	ако (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		върни -1;

	res = bisect_checkout(&good_rev, no_checkout);
	ако (res != BISECT_OK)
		върни -1;

	rc = do_bisect_run(command);

	res = bisect_checkout(&current_rev, no_checkout);
	ако (res != BISECT_OK)
		върни -1;

	върни rc;
}

static цяло bisect_run(struct bisect_terms *terms, цяло argc, const char **argv)
{
	цяло res = BISECT_OK;
	struct strbuf command = STRBUF_INIT;
	const char *reset_when_found_arg;
	const char *new_state;
	цяло temporary_stdout_fd, saved_stdout;
	цяло is_first_run = 1;
	enum reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;

	ако (bisect_next_check(terms, NULL))
		върни BISECT_FAILED;

	ако (argc && !strcmp(argv[0], "--reset-when-found")) {
		reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
	} иначе ако (argc && skip_prefix(argv[0], "--reset-when-found=",
				    &reset_when_found_arg)) {
		ако (parse_reset_when_found(reset_when_found_arg,
					   &reset_when_found))
			върни BISECT_FAILED;
	}

	ако (reset_when_found != RESET_WHEN_FOUND_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		върни error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	ако (reset_when_found != RESET_WHEN_FOUND_NONE) {
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));
		argc--;
		argv++;
	}

	ако (!argc) {
		error(_("bisect run failed: no command provided."));
		върни BISECT_FAILED;
	}

	sq_quote_argv(&command, argv);
	strbuf_ltrim(&command);
	докато (1) {
		res = do_bisect_run(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		ако (is_first_run && (res == 126 || res == 127)) {
			цяло rc = verify_good(terms, command.buf);
			is_first_run = 0;
			ако (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->term_good);
				res = BISECT_FAILED;
				break;
			}
			ако (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->term_good);
				res = BISECT_FAILED;
				break;
			}
		}

		ако (res < 0 || 128 <= res) {
			error(_("bisect run failed: exit code %d from"
				" %s is < 0 or >= 128"), res, command.buf);
			break;
		}

		ако (res == 125)
			new_state = "skip";
		иначе ако (!res)
			new_state = terms->term_good;
		иначе
			new_state = terms->term_bad;

		temporary_stdout_fd = open(git_path_bisect_run(), O_CREAT | O_WRONLY | O_TRUNC, 0666);

		ако (temporary_stdout_fd < 0) {
			res = error_errno(_("cannot open file '%s' for writing"), git_path_bisect_run());
			break;
		}

		fflush(stdout);
		saved_stdout = dup(1);
		ако (saved_stdout < 0 ||
		    dup2(temporary_stdout_fd, 1) < 0) {
			res = error_errno(_("could not duplicate stdout"));
			ако (saved_stdout >= 0)
				close(saved_stdout);
			close(temporary_stdout_fd);
			break;
		}

		res = bisect_state(terms, 1, &new_state);

		fflush(stdout);
		dup2(saved_stdout, 1);
		close(saved_stdout);
		close(temporary_stdout_fd);

		print_file_to_stdout(git_path_bisect_run());

		ако (res == BISECT_ONLY_SKIPPED_LEFT)
			error(_("bisect run cannot continue any more"));
		иначе ако (res == BISECT_INTERNAL_SUCCESS_MERGE_BASE) {
			puts(_("bisect run success"));
			res = BISECT_OK;
		} иначе ако (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
			printf(_("bisect found first '%s' commit\n"), terms->term_bad);
		} иначе ако (res) {
			error(_("bisect run failed: 'git bisect %s'"
				" exited with error code %d"), new_state, res);
		} иначе {
			continue;
		}
		break;
	}

	strbuf_release(&command);
	върни res;
}

static цяло cmd_bisect__reset(цяло argc, const char **argv, const char *prefix UNUSED,
			     struct repository *repo UNUSED)
{
	цяло res;

	ако (argc > 1)
		върни error(_("'%s' requires either no argument or a commit"),
			     "git bisect reset");
	res = bisect_reset(argc ? argv[0] : NULL, false);
	ако (res)
		върни res;
	върни bisect_clean_state();
}

static цяло cmd_bisect__terms(цяло argc, const char **argv, const char *prefix UNUSED,
			     struct repository *repo UNUSED)
{
	цяло res;
	struct bisect_terms terms = { 0 };

	ако (argc > 1)
		върни error(_("'%s' requires 0 or 1 argument"),
			     "git bisect terms");
	res = bisect_terms(&terms, argc == 1 ? argv[0] : NULL);
	free_terms(&terms);
	върни res;
}

static цяло cmd_bisect__start(цяло argc, const char **argv, const char *prefix UNUSED,
			     struct repository *repo UNUSED)
{
	цяло res;
	struct bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	res = bisect_start(&terms, argc, argv);
	free_terms(&terms);
	върни res;
}

static цяло cmd_bisect__next(цяло argc, const char **argv UNUSED, const char *prefix,
			    struct repository *repo UNUSED)
{
	цяло res;
	struct bisect_terms terms = { 0 };

	ако (argc)
		върни error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	ако (get_terms(&terms, 1))
		върни error(_("no terms defined"));
	res = bisect_next(&terms, prefix);
	free_terms(&terms);
	върни res;
}

static цяло cmd_bisect__log(цяло argc UNUSED, const char **argv UNUSED,
			   const char *prefix UNUSED,
			   struct repository *repo UNUSED)
{
	върни bisect_log();
}

static цяло cmd_bisect__replay(цяло argc, const char **argv, const char *prefix UNUSED,
			      struct repository *repo UNUSED)
{
	цяло res;
	struct bisect_terms terms = { 0 };

	ако (argc != 1)
		върни error(_("no logfile given"));
	set_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	върни res;
}

static цяло cmd_bisect__skip(цяло argc, const char **argv, const char *prefix UNUSED,
			    struct repository *repo UNUSED)
{
	цяло res;
	struct bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	ако (get_terms(&terms, 1))
		върни error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	върни res;
}

static цяло cmd_bisect__visualize(цяло argc, const char **argv, const char *prefix UNUSED,
				 struct repository *repo UNUSED)
{
	цяло res;
	struct bisect_terms terms = { 0 };

	ако (get_terms(&terms, 1))
		върни error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	върни res;
}

static цяло cmd_bisect__run(цяло argc, const char **argv, const char *prefix UNUSED,
			   struct repository *repo UNUSED)
{
	цяло res;
	struct bisect_terms terms = { 0 };

	ако (!argc)
		върни error(_("'%s' failed: no command provided."), "git bisect run");
	ако (get_terms(&terms, 1))
		върни error(_("no terms defined"));
	res = bisect_run(&terms, argc, argv);
	free_terms(&terms);
	върни res;
}

цяло cmd_bisect(цяло argc,
	       const char **argv,
	       const char *prefix,
	       struct repository *repo)
{
	цяло res = 0;
	parse_opt_subcommand_fn *fn = NULL;
	struct option options[] = {
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

	ако (!fn) {
		struct bisect_terms terms = { 0 };

		ако (!argc)
			usage_msg_opt(_("need a command"), git_bisect_usage, options);

		ако (!strcmp(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		set_terms(&terms, "bad", "good");
		ако (get_terms(&terms, 1))
			върни error(_("no terms defined"));
		ако (check_and_set_terms(&terms, argv[0]) ||
		    !one_of(argv[0], terms.term_good, terms.term_bad, NULL))
			usage_msg_optf(_("unknown command: '%s'"), git_bisect_usage,
				       options, argv[0]);
		иначе
			res = bisect_state(&terms, argc, argv);
		free_terms(&terms);
	} иначе {
		argc--;
		argv++;
		res = fn(argc, argv, prefix, repo);
	}

	ако (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		enum reset_when_found_mode mode;

		ако (read_reset_when_found(&mode))
			res = BISECT_FAILED;
		иначе ако (mode != RESET_WHEN_FOUND_NONE &&
			 bisect_reset_when_found(mode))
			res = BISECT_FAILED;
	}

	върни is_bisect_success(res) ? 0 : -res;
}
