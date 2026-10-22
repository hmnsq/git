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
	unsigned 整数 object_flags;
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

static 整数 bisect_autostart(struct bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
LAST_ARG_MUST_BE_NULL
static 整数 one_of(const char *term, ...)
{
	整数 res = 0;
	va_list matches;
	const char *match;

	va_start(matches, term);
	当 (!res && (match = va_arg(matches, const char *)))
		res = !strcmp(term, match);
	va_end(matches);

	返回 res;
}

/*
 * return code BISECT_INTERNAL_SUCCESS_MERGE_BASE
 * and BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND are codes
 * that indicate special success.
 */

static 整数 is_bisect_success(enum bisect_error res)
{
	返回 !res ||
		res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND ||
		res == BISECT_INTERNAL_SUCCESS_MERGE_BASE;
}

static 整数 write_in_file(const char *path, const char *mode, const char *format, va_list args)
{
	FILE *fp = NULL;
	整数 res = 0;

	如果 (strcmp(mode, "w") && strcmp(mode, "a"))
		BUG("write-in-file does not support '%s' mode", mode);
	fp = fopen(path, mode);
	如果 (!fp)
		返回 error_errno(_("cannot open file '%s' in mode '%s'"), path, mode);
	res = vfprintf(fp, format, args);

	如果 (res < 0) {
		整数 saved_errno = errno;
		fclose(fp);
		errno = saved_errno;
		返回 error_errno(_("could not write to file '%s'"), path);
	}

	返回 fclose(fp);
}

__attribute__((format (printf, 2, 3)))
static 整数 write_to_file(const char *path, const char *format, ...)
{
	整数 res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "w", format, args);
	va_end(args);

	返回 res;
}

__attribute__((format (printf, 2, 3)))
static 整数 append_to_file(const char *path, const char *format, ...)
{
	整数 res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "a", format, args);
	va_end(args);

	返回 res;
}

static 整数 print_file_to_stdout(const char *path)
{
	整数 fd = open(path, O_RDONLY);
	整数 ret = 0;

	如果 (fd < 0)
		返回 error_errno(_("cannot open file '%s' for reading"), path);
	如果 (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), path);
	close(fd);
	返回 ret;
}

static 整数 check_term_format(const char *term, const char *orig_term)
{
	整数 res;
	char *new_term = xstrfmt("refs/bisect/%s", term);

	res = check_refname_format(new_term, 0);
	free(new_term);

	如果 (res)
		返回 error(_("'%s' is not a valid term"), term);

	如果 (one_of(term, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", NULL))
		返回 error(_("can't use the builtin command '%s' as a term"), term);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	如果 ((strcmp(orig_term, "bad") && one_of(term, "bad", "new", NULL)) ||
		 (strcmp(orig_term, "good") && one_of(term, "good", "old", NULL)))
		返回 error(_("can't change the meaning of the term '%s'"), term);

	返回 0;
}

static 整数 write_terms(const char *bad, const char *good)
{
	整数 res;

	如果 (!strcmp(bad, good))
		返回 error(_("please use two different terms"));

	如果 (check_term_format(bad, "bad") || check_term_format(good, "good"))
		返回 -1;

	res = write_to_file(git_path_bisect_terms(), "%s\n%s\n", bad, good);

	返回 res;
}

static 整数 bisect_reset(const char *commit, bool quiet)
{
	struct strbuf branch = STRBUF_INIT;

	如果 (!commit) {
		如果 (!strbuf_read_file(&branch, git_path_bisect_start(), 0))
			printf(_("We are not bisecting.\n"));
		否则
			strbuf_rtrim(&branch);
	} 否则 {
		struct object_id oid;

		如果 (repo_get_oid_commit(the_repository, commit, &oid))
			返回 error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	如果 (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		struct child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "checkout", "--ignore-other-worktrees", NULL);
		如果 (quiet)
			strvec_push(&cmd.args, "--quiet");
		strvec_pushl(&cmd.args, branch.buf, "--", NULL);
		如果 (run_command(&cmd)) {
			error(_("could not check out original"
				" HEAD '%s'. Try 'git bisect"
				" reset <commit>'."), branch.buf);
			strbuf_release(&branch);
			返回 -1;
		}
	}

	strbuf_release(&branch);
	返回 0;
}

static 整数 parse_reset_when_found(const char *value,
				  enum reset_when_found_mode *mode)
{
	如果 (!strcmp(value, "original"))
		*mode = RESET_WHEN_FOUND_TO_ORIGINAL;
	否则 如果 (!strcmp(value, "found"))
		*mode = RESET_WHEN_FOUND_TO_FOUND;
	否则
		返回 error(_("invalid value for '--reset-when-found': '%s'"),
			     value);

	返回 0;
}

static const char *reset_when_found_mode_name(enum reset_when_found_mode mode)
{
	switch (mode) {
	case RESET_WHEN_FOUND_TO_ORIGINAL:
		返回 "original";
	case RESET_WHEN_FOUND_TO_FOUND:
		返回 "found";
	case RESET_WHEN_FOUND_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", mode);
}

static 整数 read_reset_when_found(enum reset_when_found_mode *mode)
{
	struct strbuf value = STRBUF_INIT;
	整数 res = 0;

	*mode = RESET_WHEN_FOUND_NONE;
	如果 (is_empty_or_missing_file(git_path_bisect_reset_when_found()))
		返回 0;

	如果 (strbuf_read_file(&value, git_path_bisect_reset_when_found(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_path_bisect_reset_when_found());
		goto out;
	}
	strbuf_trim(&value);
	如果 (parse_reset_when_found(value.buf, mode))
		res = -1;

out:
	strbuf_release(&value);
	返回 res;
}

static 整数 bisect_reset_when_found(enum reset_when_found_mode mode)
{
	struct bisect_terms terms = { 0 };
	char *commit = NULL;
	整数 res;

	如果 (mode == RESET_WHEN_FOUND_TO_FOUND) {
		read_bisect_terms(&terms.term_bad, &terms.term_good);
		commit = xstrfmt("refs/bisect/%s", terms.term_bad);
	} 否则 如果 (mode == RESET_WHEN_FOUND_NONE) {
		BUG("automatic reset requested without a reset mode");
	}

	res = bisect_reset(commit, true);
	如果 (!res)
		res = bisect_clean_state();

	free(commit);
	free_terms(&terms);
	返回 res;
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

static 整数 bisect_write(const char *state, const char *rev,
			const struct bisect_terms *terms, 整数 nolog)
{
	struct strbuf tag = STRBUF_INIT;
	struct object_id oid;
	struct commit *commit;
	FILE *fp = NULL;
	整数 res = 0;

	如果 (!strcmp(state, terms->term_bad)) {
		strbuf_addf(&tag, "refs/bisect/%s", state);
	} 否则 如果 (one_of(state, terms->term_good, "skip", NULL)) {
		strbuf_addf(&tag, "refs/bisect/%s-%s", state, rev);
	} 否则 {
		res = error(_("Bad bisect_write argument: %s"), state);
		goto finish;
	}

	如果 (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		goto finish;
	}

	如果 (refs_update_ref(get_main_ref_store(the_repository), NULL, tag.buf, &oid, NULL, 0,
			    UPDATE_REFS_MSG_ON_ERR)) {
		res = -1;
		goto finish;
	}

	fp = fopen(git_path_bisect_log(), "a");
	如果 (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_path_bisect_log());
		goto finish;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	如果 (!nolog)
		fprintf(fp, "git bisect %s %s\n", state, rev);

finish:
	如果 (fp)
		fclose(fp);
	strbuf_release(&tag);
	返回 res;
}

static 整数 check_and_set_terms(struct bisect_terms *terms, const char *cmd)
{
	整数 has_term_file = !is_empty_or_missing_file(git_path_bisect_terms());

	如果 (one_of(cmd, "skip", "start", "terms", NULL))
		返回 0;

	如果 (has_term_file && strcmp(cmd, terms->term_bad) &&
	    strcmp(cmd, terms->term_good))
		返回 error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->term_bad,
				terms->term_good);

	如果 (!has_term_file) {
		如果 (one_of(cmd, "bad", "good", NULL)) {
			set_terms(terms, "bad", "good");
			返回 write_terms(terms->term_bad, terms->term_good);
		}
		如果 (one_of(cmd, "new", "old", NULL)) {
			set_terms(terms, "new", "old");
			返回 write_terms(terms->term_bad, terms->term_good);
		}
	}

	返回 0;
}

static 整数 inc_nr(const struct reference *ref UNUSED, void *cb_data)
{
	unsigned 整数 *nr = (unsigned 整数 *)cb_data;
	(*nr)++;
	返回 0;
}

static const char need_bad_and_good_revision_warning[] =
	N_("You need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

static const char need_bisect_start_warning[] =
	N_("You need to start by \"git bisect start\".\n"
	   "You then need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

static 整数 decide_next(const struct bisect_terms *terms,
		       const char *current_term, 整数 missing_good,
		       整数 missing_bad)
{
	如果 (!missing_good && !missing_bad)
		返回 0;
	如果 (!current_term)
		返回 -1;

	如果 (missing_good && !missing_bad &&
	    !strcmp(current_term, terms->term_good)) {
		char *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->term_bad);
		如果 (!isatty(0))
			返回 0;
		/*
		 * TRANSLATORS: Make sure to include [Y] and [n] in your
		 * translation. The program will only accept English input
		 * at this point.
		 */
		yesno = git_prompt(_("Are you sure [Y/n]? "), PROMPT_ECHO);
		如果 (starts_with(yesno, "N") || starts_with(yesno, "n"))
			返回 -1;
		返回 0;
	}

	如果 (!is_empty_or_missing_file(git_path_bisect_start()))
		返回 error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	否则
		返回 error(_(need_bisect_start_warning),
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

	如果 (refs_ref_exists(get_main_ref_store(the_repository), bad_ref))
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
	如果 (state.nr_good && state.nr_bad)
		返回;

	如果 (!state.nr_good && !state.nr_bad)
		bisect_log_printf(_("status: waiting for both '%s' and '%s' commits\n"),
				  terms->term_good, terms->term_bad);
	否则 如果 (state.nr_good)
		bisect_log_printf(Q_("status: waiting for '%s' commit, %d '%s' commit known\n",
				     "status: waiting for '%s' commit, %d '%s' commits known\n",
				     state.nr_good),
				  terms->term_bad, state.nr_good, terms->term_good);
	否则
		bisect_log_printf(_("status: waiting for '%s' commit(s), '%s' commit known\n"),
				  terms->term_good, terms->term_bad);
}

static 整数 bisect_next_check(const struct bisect_terms *terms,
			     const char *current_term)
{
	struct bisect_state state = { 0 };
	bisect_status(&state, terms);
	返回 decide_next(terms, current_term, !state.nr_good, !state.nr_bad);
}

static 整数 get_terms(struct bisect_terms *terms, 整数 file_missing_is_ok)
{
	struct strbuf str = STRBUF_INIT;
	FILE *fp = NULL;
	整数 res = 0;

	fp = fopen(git_path_bisect_terms(), "r");
	如果 (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		goto finish;
	}

	free_terms(terms);
	如果 (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		goto finish;
	}
	terms->term_bad = strbuf_detach(&str, NULL);
	如果 (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		FREE_AND_NULL(terms->term_bad);
		goto finish;
	}
	terms->term_good = strbuf_detach(&str, NULL);

finish:
	如果 (fp)
		fclose(fp);
	strbuf_release(&str);
	返回 res;
}

static 整数 bisect_terms(struct bisect_terms *terms, const char *option)
{
	如果 (get_terms(terms, 0))
		返回 error(_("no terms defined"));

	如果 (!option) {
		printf(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->term_good, terms->term_bad);
		返回 0;
	}
	如果 (one_of(option, "--term-good", "--term-old", NULL))
		printf("%s\n", terms->term_good);
	否则 如果 (one_of(option, "--term-bad", "--term-new", NULL))
		printf("%s\n", terms->term_bad);
	否则
		返回 error(_("invalid argument %s for 'git bisect terms'.\n"
			       "Supported options are: "
			       "--term-good|--term-old and "
			       "--term-bad|--term-new."), option);

	返回 0;
}

static 整数 bisect_append_log_quoted(const char **argv)
{
	整数 res = 0;
	FILE *fp = fopen(git_path_bisect_log(), "a");
	struct strbuf orig_args = STRBUF_INIT;

	如果 (!fp)
		返回 -1;

	如果 (fprintf(fp, "git bisect start") < 1) {
		res = -1;
		goto finish;
	}

	sq_quote_argv(&orig_args, argv);
	如果 (fprintf(fp, "%s\n", orig_args.buf) < 1)
		res = -1;

finish:
	fclose(fp);
	strbuf_release(&orig_args);
	返回 res;
}

static 整数 add_bisect_ref(const struct reference *ref, void *cb)
{
	struct add_bisect_ref_data *data = cb;

	add_pending_oid(data->revs, ref->name, ref->oid, data->object_flags);

	返回 0;
}

static 整数 prepare_revs(struct bisect_terms *terms, struct rev_info *revs)
{
	struct refs_for_each_ref_options opts = {
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};
	整数 res = 0;
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

	如果 (prepare_revision_walk(revs))
		res = error(_("revision walk setup failed"));

	free(good);
	free(bad);
	返回 res;
}

static 整数 bisect_skipped_commits(struct bisect_terms *terms)
{
	整数 res;
	FILE *fp = NULL;
	struct rev_info revs;
	struct commit *commit;
	struct pretty_print_context pp = {0};
	struct strbuf commit_name = STRBUF_INIT;

	res = prepare_revs(terms, &revs);
	如果 (res)
		返回 res;

	fp = fopen(git_path_bisect_log(), "a");
	如果 (!fp)
		返回 error_errno(_("could not open '%s' for appending"),
				  git_path_bisect_log());

	如果 (fprintf(fp, "# only skipped commits left to test\n") < 0)
		返回 error_errno(_("failed to write to '%s'"), git_path_bisect_log());

	当 ((commit = get_revision(&revs)) != NULL) {
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
	返回 0;
}

static 整数 bisect_successful(struct bisect_terms *terms)
{
	struct object_id oid;
	struct commit *commit;
	struct pretty_print_context pp = {0};
	struct strbuf commit_name = STRBUF_INIT;
	char *bad_ref = xstrfmt("refs/bisect/%s",terms->term_bad);
	整数 res;

	refs_read_ref(get_main_ref_store(the_repository), bad_ref, &oid);
	commit = lookup_commit_reference_by_name(bad_ref);
	如果 (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		free(bad_ref);
		返回 BISECT_FAILED;
	}
	repo_format_commit_message(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_to_file(git_path_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->term_bad, oid_to_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	free(bad_ref);
	返回 res;
}

static enum bisect_error bisect_next(struct bisect_terms *terms,
				     const char *prefix)
{
	enum bisect_error res;

	如果 (bisect_autostart(terms))
		返回 BISECT_FAILED;

	如果 (bisect_next_check(terms, terms->term_good))
		返回 BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_next_all(the_repository, prefix);

	如果 (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		res = bisect_successful(terms);
		返回 res ? res : BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND;
	} 否则 如果 (res == BISECT_ONLY_SKIPPED_LEFT) {
		res = bisect_skipped_commits(terms);
		返回 res ? res : BISECT_ONLY_SKIPPED_LEFT;
	}
	返回 res;
}

static enum bisect_error bisect_auto_next(struct bisect_terms *terms,
					  const char *prefix)
{
	如果 (bisect_next_check(terms, NULL)) {
		bisect_print_status(terms);
		返回 BISECT_OK;
	}

	返回 bisect_next(terms, prefix);
}

static enum bisect_error bisect_start(struct bisect_terms *terms, 整数 argc,
				      const char **argv)
{
	整数 no_checkout = 0;
	整数 first_parent_only = 0;
	整数 i, has_double_dash = 0, must_write_terms = 0, bad_seen = 0;
	整数 flags, pathspec_pos;
	enum bisect_error res = BISECT_OK;
	struct string_list revs = STRING_LIST_INIT_DUP;
	struct string_list states = STRING_LIST_INIT_DUP;
	struct strbuf start_head = STRBUF_INIT;
	struct strbuf bisect_names = STRBUF_INIT;
	struct object_id head_oid;
	struct object_id oid;
	enum reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;
	const char *head;

	如果 (is_bare_repository(the_repository))
		no_checkout = 1;

	/*
	 * Check for one bad and then some good revisions
	 */
	为 (i = 0; i < argc; i++) {
		如果 (!strcmp(argv[i], "--")) {
			has_double_dash = 1;
			break;
		}
	}

	为 (i = 0; i < argc; i++) {
		const char *arg = argv[i];
		如果 (!strcmp(argv[i], "--")) {
			break;
		} 否则 如果 (!strcmp(arg, "--no-checkout")) {
			no_checkout = 1;
		} 否则 如果 (!strcmp(arg, "--first-parent")) {
			first_parent_only = 1;
		} 否则 如果 (!strcmp(arg, "--reset-when-found")) {
			reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
		} 否则 如果 (skip_prefix(arg, "--reset-when-found=", &arg)) {
			如果 (parse_reset_when_found(arg, &reset_when_found)) {
				res = BISECT_FAILED;
				goto finish;
			}
		} 否则 如果 (!strcmp(arg, "--term-good") ||
			 !strcmp(arg, "--term-old")) {
			i++;
			如果 (argc <= i)
				返回 error(_("'' is not a valid term"));
			must_write_terms = 1;
			free((void *) terms->term_good);
			terms->term_good = xstrdup(argv[i]);
		} 否则 如果 (skip_prefix(arg, "--term-good=", &arg) ||
			   skip_prefix(arg, "--term-old=", &arg)) {
			must_write_terms = 1;
			free((void *) terms->term_good);
			terms->term_good = xstrdup(arg);
		} 否则 如果 (!strcmp(arg, "--term-bad") ||
			 !strcmp(arg, "--term-new")) {
			i++;
			如果 (argc <= i)
				返回 error(_("'' is not a valid term"));
			must_write_terms = 1;
			free((void *) terms->term_bad);
			terms->term_bad = xstrdup(argv[i]);
		} 否则 如果 (skip_prefix(arg, "--term-bad=", &arg) ||
			   skip_prefix(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			free((void *) terms->term_bad);
			terms->term_bad = xstrdup(arg);
		} 否则 如果 (starts_with(arg, "--")) {
			返回 error(_("unrecognized option: '%s'"), arg);
		} 否则 如果 (!get_oidf(&oid, "%s^{commit}", arg)) {
			string_list_append(&revs, oid_to_hex(&oid));
		} 否则 如果 (has_double_dash) {
			die(_("'%s' does not appear to be a valid "
			      "revision"), arg);
		} 否则 {
			break;
		}
	}
	如果 (reset_when_found != RESET_WHEN_FOUND_NONE && no_checkout) {
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
	如果 (revs.nr)
		must_write_terms = 1;
	为 (i = 0; i < revs.nr; i++) {
		如果 (bad_seen) {
			string_list_append(&states, terms->term_good);
		} 否则 {
			bad_seen = 1;
			string_list_append(&states, terms->term_bad);
		}
	}

	/*
	 * Verify HEAD
	 */
	head = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
				       "HEAD", 0, &head_oid, &flags);
	如果 (!head) {
		如果 (repo_get_oid(the_repository, "HEAD", &head_oid))
			返回 error(_("bad HEAD - I need a HEAD"));
		head = "HEAD";
	}

	/*
	 * Check if we are bisecting
	 */
	如果 (!is_empty_or_missing_file(git_path_bisect_start())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&start_head, git_path_bisect_start(), 0);
		strbuf_trim(&start_head);
		如果 (!no_checkout) {
			struct child_process cmd = CHILD_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.args, "checkout", start_head.buf,
				     "--", NULL);
			如果 (run_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       start_head.buf);
				goto finish;
			}
		}
	} 否则 {
		/* Get the rev from where we start. */
		如果 (!repo_get_oid(the_repository, head, &head_oid) &&
		    !starts_with(head, "refs/heads/")) {
			strbuf_reset(&start_head);
			strbuf_add_oid_hex(&start_head, &head_oid);
		} 否则 如果 (!repo_get_oid(the_repository, head, &head_oid) &&
			   skip_prefix(head, "refs/heads/", &head)) {
			strbuf_addstr(&start_head, head);
		} 否则 {
			返回 error(_("bad HEAD - strange symbolic ref"));
		}
	}

	/*
	 * Get rid of any old bisect state.
	 */
	如果 (bisect_clean_state())
		返回 BISECT_FAILED;

	/*
	 * Write new start state
	 */
	write_file(git_path_bisect_start(), "%s\n", start_head.buf);

	如果 (first_parent_only)
		write_file(git_path_bisect_first_parent(), "\n");

	如果 (reset_when_found != RESET_WHEN_FOUND_NONE)
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));

	如果 (no_checkout) {
		如果 (repo_get_oid(the_repository, start_head.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), start_head.buf);
			goto finish;
		}
		如果 (refs_update_ref(get_main_ref_store(the_repository), NULL, "BISECT_HEAD", &oid, NULL, 0,
				    UPDATE_REFS_MSG_ON_ERR)) {
			res = BISECT_FAILED;
			goto finish;
		}
	}

	如果 (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_names, argv + pathspec_pos);
	write_file(git_path_bisect_names(), "%s\n", bisect_names.buf);

	为 (i = 0; i < states.nr; i++)
		如果 (bisect_write(states.items[i].string,
				 revs.items[i].string, terms, 1)) {
			res = BISECT_FAILED;
			goto finish;
		}

	如果 (must_write_terms && write_terms(terms->term_bad,
					    terms->term_good)) {
		res = BISECT_FAILED;
		goto finish;
	}

	res = bisect_append_log_quoted(argv);
	如果 (res)
		res = BISECT_FAILED;

finish:
	string_list_clear(&revs, 0);
	string_list_clear(&states, 0);
	strbuf_release(&start_head);
	strbuf_release(&bisect_names);
	如果 (res)
		返回 res;

	res = bisect_auto_next(terms, NULL);
	如果 (!is_bisect_success(res))
		bisect_clean_state();
	返回 res;
}

static inline 整数 file_is_not_empty(const char *path)
{
	返回 !is_empty_or_missing_file(path);
}

static 整数 bisect_autostart(struct bisect_terms *terms)
{
	整数 res;
	const char *yesno;

	如果 (file_is_not_empty(git_path_bisect_start()))
		返回 0;

	fprintf_ln(stderr, _("You need to start by \"git bisect "
			  "start\"\n"));

	如果 (!isatty(STDIN_FILENO))
		返回 -1;

	/*
	 * TRANSLATORS: Make sure to include [Y] and [n] in your
	 * translation. The program will only accept English input
	 * at this point.
	 */
	yesno = git_prompt(_("Do you want me to do it for you "
			     "[Y/n]? "), PROMPT_ECHO);
	res = tolower(*yesno) == 'n' ?
		-1 : bisect_start(terms, 0, empty_strvec);

	返回 res;
}

static enum bisect_error bisect_state(struct bisect_terms *terms, 整数 argc,
				      const char **argv)
{
	const char *state;
	整数 i, verify_expected = 1;
	struct object_id oid, expected;
	struct oid_array revs = OID_ARRAY_INIT;

	如果 (!argc)
		返回 error(_("Please call `--bisect-state` with at least one argument"));

	如果 (bisect_autostart(terms))
		返回 BISECT_FAILED;

	state = argv[0];
	如果 (check_and_set_terms(terms, state) ||
	    !one_of(state, terms->term_good, terms->term_bad, "skip", NULL))
		返回 BISECT_FAILED;

	argv++;
	argc--;
	如果 (argc > 1 && !strcmp(state, terms->term_bad))
		返回 error(_("'git bisect %s' can take only one argument."), terms->term_bad);

	如果 (argc == 0) {
		const char *head = "BISECT_HEAD";
		enum get_oid_result res_head = repo_get_oid(the_repository,
							    head, &oid);

		如果 (res_head == MISSING_OBJECT) {
			head = "HEAD";
			res_head = repo_get_oid(the_repository, head, &oid);
		}

		如果 (res_head)
			error(_("Bad rev input: %s"), head);
		oid_array_append(&revs, &oid);
	}

	/*
	 * All input revs must be checked before executing bisect_write()
	 * to discard junk revs.
	 */

	为 (; argc; argc--, argv++) {
		struct commit *commit;

		如果 (repo_get_oid(the_repository, *argv, &oid)){
			error(_("Bad rev input: %s"), *argv);
			oid_array_clear(&revs);
			返回 BISECT_FAILED;
		}

		commit = lookup_commit_reference(the_repository, &oid);
		如果 (!commit)
			die(_("Bad rev input (not a commit): %s"), *argv);

		oid_array_append(&revs, &commit->object.oid);
	}

	如果 (refs_read_ref(get_main_ref_store(the_repository), "BISECT_EXPECTED_REV", &expected))
		verify_expected = 0; /* Ignore invalid file contents */

	为 (i = 0; i < revs.nr; i++) {
		如果 (bisect_write(state, oid_to_hex(&revs.oid[i]), terms, 0)) {
			oid_array_clear(&revs);
			返回 BISECT_FAILED;
		}
		如果 (verify_expected && !oideq(&revs.oid[i], &expected)) {
			unlink_or_warn(git_path_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					NULL, "BISECT_EXPECTED_REV", NULL,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	返回 bisect_auto_next(terms, NULL);
}

static enum bisect_error bisect_log(void)
{
	整数 fd, status;
	const char* filename = git_path_bisect_log();

	如果 (is_empty_or_missing_file(filename))
		返回 error(_("We are not bisecting."));

	fd = open(filename, O_RDONLY);
	如果 (fd < 0)
		返回 BISECT_FAILED;

	status = copy_fd(fd, STDOUT_FILENO);
	close(fd);
	返回 status ? BISECT_FAILED : BISECT_OK;
}

static 整数 process_replay_line(struct bisect_terms *terms, struct strbuf *line)
{
	const char *p = line->buf + strspn(line->buf, " \t");
	char *word_end, *rev;

	如果 ((!skip_prefix(p, "git bisect", &p) &&
	!skip_prefix(p, "git-bisect", &p)) || !isspace(*p))
		返回 0;
	p += strspn(p, " \t");

	word_end = (char *)p + strcspn(p, " \t");
	rev = word_end + strspn(word_end, " \t");
	*word_end = '\0'; /* NUL-terminate the word */

	如果 (get_terms(terms, 1))
		返回 error(_("no terms defined"));
	如果 (check_and_set_terms(terms, p))
		返回 -1;

	如果 (!strcmp(p, "start")) {
		struct strvec argv = STRVEC_INIT;
		整数 res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_start(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		返回 res;
	}

	如果 (one_of(p, terms->term_good,
	   terms->term_bad, "skip", NULL))
		返回 bisect_write(p, rev, terms, 0);

	如果 (!strcmp(p, "terms")) {
		struct strvec argv = STRVEC_INIT;
		整数 res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_terms(terms, argv.nr == 1 ? argv.v[0] : NULL);
		strvec_clear(&argv);
		返回 res;
	}
	error(_("'%s'?? what are you talking about?"), p);

	返回 -1;
}

static enum bisect_error bisect_replay(struct bisect_terms *terms, const char *filename)
{
	FILE *fp = NULL;
	enum bisect_error res = BISECT_OK;
	struct strbuf line = STRBUF_INIT;

	如果 (is_empty_or_missing_file(filename))
		返回 error(_("cannot read file '%s' for replaying"), filename);

	如果 (bisect_clean_state())
		返回 BISECT_FAILED;

	fp = fopen(filename, "r");
	如果 (!fp)
		返回 BISECT_FAILED;

	当 ((strbuf_getline(&line, fp) != EOF) && !res)
		res = process_replay_line(terms, &line);

	strbuf_release(&line);
	fclose(fp);

	如果 (res)
		返回 BISECT_FAILED;

	返回 bisect_auto_next(terms, NULL);
}

static enum bisect_error bisect_skip(struct bisect_terms *terms, 整数 argc,
				     const char **argv)
{
	整数 i;
	enum bisect_error res;
	struct strvec argv_state = STRVEC_INIT;

	strvec_push(&argv_state, "skip");

	为 (i = 0; i < argc; i++) {
		const char *dotdot = strstr(argv[i], "..");

		如果 (dotdot) {
			struct rev_info revs;
			struct commit *commit;

			repo_init_revisions(the_repository, &revs, NULL);
			setup_revisions(2, argv + i - 1, &revs, NULL);

			如果 (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			当 ((commit = get_revision(&revs)) != NULL)
				strvec_push(&argv_state,
						oid_to_hex(&commit->object.oid));

			reset_revision_walk();
			release_revisions(&revs);
		} 否则 {
			strvec_push(&argv_state, argv[i]);
		}
	}
	res = bisect_state(terms, argv_state.nr, argv_state.v);

	strvec_clear(&argv_state);
	返回 res;
}

static 整数 bisect_visualize(struct bisect_terms *terms, 整数 argc,
			    const char **argv)
{
	struct child_process cmd = CHILD_PROCESS_INIT;
	struct strbuf sb = STRBUF_INIT;

	如果 (bisect_next_check(terms, NULL) != 0)
		返回 BISECT_FAILED;

	cmd.no_stdin = 1;
	如果 (!argc) {
		如果 ((getenv("DISPLAY") || getenv("SESSIONNAME") || getenv("MSYSTEM") ||
		     getenv("SECURITYSESSIONID")) && exists_in_PATH("gitk")) {
			strvec_push(&cmd.args, "gitk");
		} 否则 {
			strvec_push(&cmd.args, "log");
			cmd.git_cmd = 1;
		}
	} 否则 {
		如果 (argv[0][0] == '-') {
			strvec_push(&cmd.args, "log");
			cmd.git_cmd = 1;
		} 否则 如果 (strcmp(argv[0], "tig") && !starts_with(argv[0], "git"))
			cmd.git_cmd = 1;

		strvec_pushv(&cmd.args, argv);
	}

	strvec_pushl(&cmd.args, "--bisect", "--", NULL);

	strbuf_read_file(&sb, git_path_bisect_names(), 0);
	sq_dequote_to_strvec(sb.buf, &cmd.args);
	strbuf_release(&sb);

	返回 run_command(&cmd);
}

static 整数 get_first_good(const struct reference *ref, void *cb_data)
{
	oidcpy(cb_data, ref->oid);
	返回 1;
}

static 整数 do_bisect_run(const char *command)
{
	struct child_process cmd = CHILD_PROCESS_INIT;

	printf(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_push(&cmd.args, command);
	返回 run_command(&cmd);
}

static 整数 verify_good(const struct bisect_terms *terms, const char *command)
{
	整数 rc;
	enum bisect_error res;
	struct object_id good_rev;
	struct object_id current_rev;
	char *good_glob = xstrfmt("%s-*", terms->term_good);
	整数 no_checkout = refs_ref_exists(get_main_ref_store(the_repository),
					  "BISECT_HEAD");
	struct refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_first_good, &good_rev, &opts);
	free(good_glob);

	如果 (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		返回 -1;

	res = bisect_checkout(&good_rev, no_checkout);
	如果 (res != BISECT_OK)
		返回 -1;

	rc = do_bisect_run(command);

	res = bisect_checkout(&current_rev, no_checkout);
	如果 (res != BISECT_OK)
		返回 -1;

	返回 rc;
}

static 整数 bisect_run(struct bisect_terms *terms, 整数 argc, const char **argv)
{
	整数 res = BISECT_OK;
	struct strbuf command = STRBUF_INIT;
	const char *reset_when_found_arg;
	const char *new_state;
	整数 temporary_stdout_fd, saved_stdout;
	整数 is_first_run = 1;
	enum reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;

	如果 (bisect_next_check(terms, NULL))
		返回 BISECT_FAILED;

	如果 (argc && !strcmp(argv[0], "--reset-when-found")) {
		reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
	} 否则 如果 (argc && skip_prefix(argv[0], "--reset-when-found=",
				    &reset_when_found_arg)) {
		如果 (parse_reset_when_found(reset_when_found_arg,
					   &reset_when_found))
			返回 BISECT_FAILED;
	}

	如果 (reset_when_found != RESET_WHEN_FOUND_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		返回 error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	如果 (reset_when_found != RESET_WHEN_FOUND_NONE) {
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));
		argc--;
		argv++;
	}

	如果 (!argc) {
		error(_("bisect run failed: no command provided."));
		返回 BISECT_FAILED;
	}

	sq_quote_argv(&command, argv);
	strbuf_ltrim(&command);
	当 (1) {
		res = do_bisect_run(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		如果 (is_first_run && (res == 126 || res == 127)) {
			整数 rc = verify_good(terms, command.buf);
			is_first_run = 0;
			如果 (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->term_good);
				res = BISECT_FAILED;
				break;
			}
			如果 (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->term_good);
				res = BISECT_FAILED;
				break;
			}
		}

		如果 (res < 0 || 128 <= res) {
			error(_("bisect run failed: exit code %d from"
				" %s is < 0 or >= 128"), res, command.buf);
			break;
		}

		如果 (res == 125)
			new_state = "skip";
		否则 如果 (!res)
			new_state = terms->term_good;
		否则
			new_state = terms->term_bad;

		temporary_stdout_fd = open(git_path_bisect_run(), O_CREAT | O_WRONLY | O_TRUNC, 0666);

		如果 (temporary_stdout_fd < 0) {
			res = error_errno(_("cannot open file '%s' for writing"), git_path_bisect_run());
			break;
		}

		fflush(stdout);
		saved_stdout = dup(1);
		如果 (saved_stdout < 0 ||
		    dup2(temporary_stdout_fd, 1) < 0) {
			res = error_errno(_("could not duplicate stdout"));
			如果 (saved_stdout >= 0)
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

		如果 (res == BISECT_ONLY_SKIPPED_LEFT)
			error(_("bisect run cannot continue any more"));
		否则 如果 (res == BISECT_INTERNAL_SUCCESS_MERGE_BASE) {
			puts(_("bisect run success"));
			res = BISECT_OK;
		} 否则 如果 (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
			printf(_("bisect found first '%s' commit\n"), terms->term_bad);
		} 否则 如果 (res) {
			error(_("bisect run failed: 'git bisect %s'"
				" exited with error code %d"), new_state, res);
		} 否则 {
			continue;
		}
		break;
	}

	strbuf_release(&command);
	返回 res;
}

static 整数 cmd_bisect__reset(整数 argc, const char **argv, const char *prefix UNUSED,
			     struct repository *repo UNUSED)
{
	整数 res;

	如果 (argc > 1)
		返回 error(_("'%s' requires either no argument or a commit"),
			     "git bisect reset");
	res = bisect_reset(argc ? argv[0] : NULL, false);
	如果 (res)
		返回 res;
	返回 bisect_clean_state();
}

static 整数 cmd_bisect__terms(整数 argc, const char **argv, const char *prefix UNUSED,
			     struct repository *repo UNUSED)
{
	整数 res;
	struct bisect_terms terms = { 0 };

	如果 (argc > 1)
		返回 error(_("'%s' requires 0 or 1 argument"),
			     "git bisect terms");
	res = bisect_terms(&terms, argc == 1 ? argv[0] : NULL);
	free_terms(&terms);
	返回 res;
}

static 整数 cmd_bisect__start(整数 argc, const char **argv, const char *prefix UNUSED,
			     struct repository *repo UNUSED)
{
	整数 res;
	struct bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	res = bisect_start(&terms, argc, argv);
	free_terms(&terms);
	返回 res;
}

static 整数 cmd_bisect__next(整数 argc, const char **argv UNUSED, const char *prefix,
			    struct repository *repo UNUSED)
{
	整数 res;
	struct bisect_terms terms = { 0 };

	如果 (argc)
		返回 error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	如果 (get_terms(&terms, 1))
		返回 error(_("no terms defined"));
	res = bisect_next(&terms, prefix);
	free_terms(&terms);
	返回 res;
}

static 整数 cmd_bisect__log(整数 argc UNUSED, const char **argv UNUSED,
			   const char *prefix UNUSED,
			   struct repository *repo UNUSED)
{
	返回 bisect_log();
}

static 整数 cmd_bisect__replay(整数 argc, const char **argv, const char *prefix UNUSED,
			      struct repository *repo UNUSED)
{
	整数 res;
	struct bisect_terms terms = { 0 };

	如果 (argc != 1)
		返回 error(_("no logfile given"));
	set_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	返回 res;
}

static 整数 cmd_bisect__skip(整数 argc, const char **argv, const char *prefix UNUSED,
			    struct repository *repo UNUSED)
{
	整数 res;
	struct bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	如果 (get_terms(&terms, 1))
		返回 error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	返回 res;
}

static 整数 cmd_bisect__visualize(整数 argc, const char **argv, const char *prefix UNUSED,
				 struct repository *repo UNUSED)
{
	整数 res;
	struct bisect_terms terms = { 0 };

	如果 (get_terms(&terms, 1))
		返回 error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	返回 res;
}

static 整数 cmd_bisect__run(整数 argc, const char **argv, const char *prefix UNUSED,
			   struct repository *repo UNUSED)
{
	整数 res;
	struct bisect_terms terms = { 0 };

	如果 (!argc)
		返回 error(_("'%s' failed: no command provided."), "git bisect run");
	如果 (get_terms(&terms, 1))
		返回 error(_("no terms defined"));
	res = bisect_run(&terms, argc, argv);
	free_terms(&terms);
	返回 res;
}

整数 cmd_bisect(整数 argc,
	       const char **argv,
	       const char *prefix,
	       struct repository *repo)
{
	整数 res = 0;
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

	如果 (!fn) {
		struct bisect_terms terms = { 0 };

		如果 (!argc)
			usage_msg_opt(_("need a command"), git_bisect_usage, options);

		如果 (!strcmp(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		set_terms(&terms, "bad", "good");
		如果 (get_terms(&terms, 1))
			返回 error(_("no terms defined"));
		如果 (check_and_set_terms(&terms, argv[0]) ||
		    !one_of(argv[0], terms.term_good, terms.term_bad, NULL))
			usage_msg_optf(_("unknown command: '%s'"), git_bisect_usage,
				       options, argv[0]);
		否则
			res = bisect_state(&terms, argc, argv);
		free_terms(&terms);
	} 否则 {
		argc--;
		argv++;
		res = fn(argc, argv, prefix, repo);
	}

	如果 (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		enum reset_when_found_mode mode;

		如果 (read_reset_when_found(&mode))
			res = BISECT_FAILED;
		否则 如果 (mode != RESET_WHEN_FOUND_NONE &&
			 bisect_reset_when_found(mode))
			res = BISECT_FAILED;
	}

	返回 is_bisect_success(res) ? 0 : -res;
}
