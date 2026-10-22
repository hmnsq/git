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
	unsigned ຈຳນວນເຕັມ object_flags;
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

static ຈຳນວນເຕັມ bisect_autostart(sa_nracanaa bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
LAST_ARG_MUST_BE_NULL
static ຈຳນວນເຕັມ one_of(const var_na *term, ...)
{
	ຈຳນວນເຕັມ res = 0;
	va_list matches;
	const var_na *match;

	va_start(matches, term);
	ໃນຂະນະທີ່ (!res && (match = va_arg(matches, const var_na *)))
		res = !strcmp(term, match);
	va_end(matches);

	ສົ່ງຄືນ res;
}

/*
 * return code BISECT_INTERNAL_SUCCESS_MERGE_BASE
 * and BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND are codes
 * that indicate special success.
 */

static ຈຳນວນເຕັມ is_bisect_success(ga_nanaa bisect_error res)
{
	ສົ່ງຄືນ !res ||
		res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND ||
		res == BISECT_INTERNAL_SUCCESS_MERGE_BASE;
}

static ຈຳນວນເຕັມ write_in_file(const var_na *path, const var_na *mode, const var_na *format, va_list args)
{
	FILE *fp = NULL;
	ຈຳນວນເຕັມ res = 0;

	ຖ້າ (strcmp(mode, "w") && strcmp(mode, "a"))
		BUG("write-in-file does not support '%s' mode", mode);
	fp = fopen(path, mode);
	ຖ້າ (!fp)
		ສົ່ງຄືນ error_errno(_("cannot open file '%s' in mode '%s'"), path, mode);
	res = vfprintf(fp, format, args);

	ຖ້າ (res < 0) {
		ຈຳນວນເຕັມ saved_errno = errno;
		fclose(fp);
		errno = saved_errno;
		ສົ່ງຄືນ error_errno(_("could not write to file '%s'"), path);
	}

	ສົ່ງຄືນ fclose(fp);
}

__attribute__((format (printf, 2, 3)))
static ຈຳນວນເຕັມ write_to_file(const var_na *path, const var_na *format, ...)
{
	ຈຳນວນເຕັມ res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "w", format, args);
	va_end(args);

	ສົ່ງຄືນ res;
}

__attribute__((format (printf, 2, 3)))
static ຈຳນວນເຕັມ append_to_file(const var_na *path, const var_na *format, ...)
{
	ຈຳນວນເຕັມ res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "a", format, args);
	va_end(args);

	ສົ່ງຄືນ res;
}

static ຈຳນວນເຕັມ print_file_to_stdout(const var_na *path)
{
	ຈຳນວນເຕັມ fd = open(path, O_RDONLY);
	ຈຳນວນເຕັມ ret = 0;

	ຖ້າ (fd < 0)
		ສົ່ງຄືນ error_errno(_("cannot open file '%s' for reading"), path);
	ຖ້າ (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), path);
	close(fd);
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ check_term_format(const var_na *term, const var_na *orig_term)
{
	ຈຳນວນເຕັມ res;
	var_na *new_term = xstrfmt("refs/bisect/%s", term);

	res = check_refname_format(new_term, 0);
	free(new_term);

	ຖ້າ (res)
		ສົ່ງຄືນ error(_("'%s' is not a valid term"), term);

	ຖ້າ (one_of(term, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", NULL))
		ສົ່ງຄືນ error(_("can't use the builtin command '%s' as a term"), term);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	ຖ້າ ((strcmp(orig_term, "bad") && one_of(term, "bad", "new", NULL)) ||
		 (strcmp(orig_term, "good") && one_of(term, "good", "old", NULL)))
		ສົ່ງຄືນ error(_("can't change the meaning of the term '%s'"), term);

	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ write_terms(const var_na *bad, const var_na *good)
{
	ຈຳນວນເຕັມ res;

	ຖ້າ (!strcmp(bad, good))
		ສົ່ງຄືນ error(_("please use two different terms"));

	ຖ້າ (check_term_format(bad, "bad") || check_term_format(good, "good"))
		ສົ່ງຄືນ -1;

	res = write_to_file(git_path_bisect_terms(), "%s\n%s\n", bad, good);

	ສົ່ງຄືນ res;
}

static ຈຳນວນເຕັມ bisect_reset(const var_na *commit, bool quiet)
{
	sa_nracanaa strbuf branch = STRBUF_INIT;

	ຖ້າ (!commit) {
		ຖ້າ (!strbuf_read_file(&branch, git_path_bisect_start(), 0))
			printf(_("We are not bisecting.\n"));
		ບໍ່ດັ່ງນັ້ນ
			strbuf_rtrim(&branch);
	} ບໍ່ດັ່ງນັ້ນ {
		sa_nracanaa object_id oid;

		ຖ້າ (repo_get_oid_commit(the_repository, commit, &oid))
			ສົ່ງຄືນ error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	ຖ້າ (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "checkout", "--ignore-other-worktrees", NULL);
		ຖ້າ (quiet)
			strvec_push(&cmd.args, "--quiet");
		strvec_pushl(&cmd.args, branch.buf, "--", NULL);
		ຖ້າ (run_command(&cmd)) {
			error(_("could not check out original"
				" HEAD '%s'. Try 'git bisect"
				" reset <commit>'."), branch.buf);
			strbuf_release(&branch);
			ສົ່ງຄືນ -1;
		}
	}

	strbuf_release(&branch);
	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ parse_reset_when_found(const var_na *value,
				  ga_nanaa reset_when_found_mode *mode)
{
	ຖ້າ (!strcmp(value, "original"))
		*mode = RESET_WHEN_FOUND_TO_ORIGINAL;
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(value, "found"))
		*mode = RESET_WHEN_FOUND_TO_FOUND;
	ບໍ່ດັ່ງນັ້ນ
		ສົ່ງຄືນ error(_("invalid value for '--reset-when-found': '%s'"),
			     value);

	ສົ່ງຄືນ 0;
}

static const var_na *reset_when_found_mode_name(ga_nanaa reset_when_found_mode mode)
{
	switch (mode) {
	pak_sa RESET_WHEN_FOUND_TO_ORIGINAL:
		ສົ່ງຄືນ "original";
	pak_sa RESET_WHEN_FOUND_TO_FOUND:
		ສົ່ງຄືນ "found";
	pak_sa RESET_WHEN_FOUND_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", mode);
}

static ຈຳນວນເຕັມ read_reset_when_found(ga_nanaa reset_when_found_mode *mode)
{
	sa_nracanaa strbuf value = STRBUF_INIT;
	ຈຳນວນເຕັມ res = 0;

	*mode = RESET_WHEN_FOUND_NONE;
	ຖ້າ (is_empty_or_missing_file(git_path_bisect_reset_when_found()))
		ສົ່ງຄືນ 0;

	ຖ້າ (strbuf_read_file(&value, git_path_bisect_reset_when_found(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_path_bisect_reset_when_found());
		goto out;
	}
	strbuf_trim(&value);
	ຖ້າ (parse_reset_when_found(value.buf, mode))
		res = -1;

out:
	strbuf_release(&value);
	ສົ່ງຄືນ res;
}

static ຈຳນວນເຕັມ bisect_reset_when_found(ga_nanaa reset_when_found_mode mode)
{
	sa_nracanaa bisect_terms terms = { 0 };
	var_na *commit = NULL;
	ຈຳນວນເຕັມ res;

	ຖ້າ (mode == RESET_WHEN_FOUND_TO_FOUND) {
		read_bisect_terms(&terms.term_bad, &terms.term_good);
		commit = xstrfmt("refs/bisect/%s", terms.term_bad);
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (mode == RESET_WHEN_FOUND_NONE) {
		BUG("automatic reset requested without a reset mode");
	}

	res = bisect_reset(commit, true);
	ຖ້າ (!res)
		res = bisect_clean_state();

	free(commit);
	free_terms(&terms);
	ສົ່ງຄືນ res;
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

static ຈຳນວນເຕັມ bisect_write(const var_na *state, const var_na *rev,
			const sa_nracanaa bisect_terms *terms, ຈຳນວນເຕັມ nolog)
{
	sa_nracanaa strbuf tag = STRBUF_INIT;
	sa_nracanaa object_id oid;
	sa_nracanaa commit *commit;
	FILE *fp = NULL;
	ຈຳນວນເຕັມ res = 0;

	ຖ້າ (!strcmp(state, terms->term_bad)) {
		strbuf_addf(&tag, "refs/bisect/%s", state);
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (one_of(state, terms->term_good, "skip", NULL)) {
		strbuf_addf(&tag, "refs/bisect/%s-%s", state, rev);
	} ບໍ່ດັ່ງນັ້ນ {
		res = error(_("Bad bisect_write argument: %s"), state);
		goto finish;
	}

	ຖ້າ (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		goto finish;
	}

	ຖ້າ (refs_update_ref(get_main_ref_store(the_repository), NULL, tag.buf, &oid, NULL, 0,
			    UPDATE_REFS_MSG_ON_ERR)) {
		res = -1;
		goto finish;
	}

	fp = fopen(git_path_bisect_log(), "a");
	ຖ້າ (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_path_bisect_log());
		goto finish;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	ຖ້າ (!nolog)
		fprintf(fp, "git bisect %s %s\n", state, rev);

finish:
	ຖ້າ (fp)
		fclose(fp);
	strbuf_release(&tag);
	ສົ່ງຄືນ res;
}

static ຈຳນວນເຕັມ check_and_set_terms(sa_nracanaa bisect_terms *terms, const var_na *cmd)
{
	ຈຳນວນເຕັມ has_term_file = !is_empty_or_missing_file(git_path_bisect_terms());

	ຖ້າ (one_of(cmd, "skip", "start", "terms", NULL))
		ສົ່ງຄືນ 0;

	ຖ້າ (has_term_file && strcmp(cmd, terms->term_bad) &&
	    strcmp(cmd, terms->term_good))
		ສົ່ງຄືນ error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->term_bad,
				terms->term_good);

	ຖ້າ (!has_term_file) {
		ຖ້າ (one_of(cmd, "bad", "good", NULL)) {
			set_terms(terms, "bad", "good");
			ສົ່ງຄືນ write_terms(terms->term_bad, terms->term_good);
		}
		ຖ້າ (one_of(cmd, "new", "old", NULL)) {
			set_terms(terms, "new", "old");
			ສົ່ງຄືນ write_terms(terms->term_bad, terms->term_good);
		}
	}

	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ inc_nr(const sa_nracanaa reference *ref UNUSED, void *cb_data)
{
	unsigned ຈຳນວນເຕັມ *nr = (unsigned ຈຳນວນເຕັມ *)cb_data;
	(*nr)++;
	ສົ່ງຄືນ 0;
}

static const var_na need_bad_and_good_revision_warning[] =
	N_("You need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

static const var_na need_bisect_start_warning[] =
	N_("You need to start by \"git bisect start\".\n"
	   "You then need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

static ຈຳນວນເຕັມ decide_next(const sa_nracanaa bisect_terms *terms,
		       const var_na *current_term, ຈຳນວນເຕັມ missing_good,
		       ຈຳນວນເຕັມ missing_bad)
{
	ຖ້າ (!missing_good && !missing_bad)
		ສົ່ງຄືນ 0;
	ຖ້າ (!current_term)
		ສົ່ງຄືນ -1;

	ຖ້າ (missing_good && !missing_bad &&
	    !strcmp(current_term, terms->term_good)) {
		var_na *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->term_bad);
		ຖ້າ (!isatty(0))
			ສົ່ງຄືນ 0;
		/*
		 * TRANSLATORS: Make sure to include [Y] and [n] in your
		 * translation. The program will only accept English input
		 * at this point.
		 */
		yesno = git_prompt(_("Are you sure [Y/n]? "), PROMPT_ECHO);
		ຖ້າ (starts_with(yesno, "N") || starts_with(yesno, "n"))
			ສົ່ງຄືນ -1;
		ສົ່ງຄືນ 0;
	}

	ຖ້າ (!is_empty_or_missing_file(git_path_bisect_start()))
		ສົ່ງຄືນ error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	ບໍ່ດັ່ງນັ້ນ
		ສົ່ງຄືນ error(_(need_bisect_start_warning),
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

	ຖ້າ (refs_ref_exists(get_main_ref_store(the_repository), bad_ref))
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
	ຖ້າ (state.nr_good && state.nr_bad)
		ສົ່ງຄືນ;

	ຖ້າ (!state.nr_good && !state.nr_bad)
		bisect_log_printf(_("status: waiting for both '%s' and '%s' commits\n"),
				  terms->term_good, terms->term_bad);
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (state.nr_good)
		bisect_log_printf(Q_("status: waiting for '%s' commit, %d '%s' commit known\n",
				     "status: waiting for '%s' commit, %d '%s' commits known\n",
				     state.nr_good),
				  terms->term_bad, state.nr_good, terms->term_good);
	ບໍ່ດັ່ງນັ້ນ
		bisect_log_printf(_("status: waiting for '%s' commit(s), '%s' commit known\n"),
				  terms->term_good, terms->term_bad);
}

static ຈຳນວນເຕັມ bisect_next_check(const sa_nracanaa bisect_terms *terms,
			     const var_na *current_term)
{
	sa_nracanaa bisect_state state = { 0 };
	bisect_status(&state, terms);
	ສົ່ງຄືນ decide_next(terms, current_term, !state.nr_good, !state.nr_bad);
}

static ຈຳນວນເຕັມ get_terms(sa_nracanaa bisect_terms *terms, ຈຳນວນເຕັມ file_missing_is_ok)
{
	sa_nracanaa strbuf str = STRBUF_INIT;
	FILE *fp = NULL;
	ຈຳນວນເຕັມ res = 0;

	fp = fopen(git_path_bisect_terms(), "r");
	ຖ້າ (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		goto finish;
	}

	free_terms(terms);
	ຖ້າ (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		goto finish;
	}
	terms->term_bad = strbuf_detach(&str, NULL);
	ຖ້າ (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		FREE_AND_NULL(terms->term_bad);
		goto finish;
	}
	terms->term_good = strbuf_detach(&str, NULL);

finish:
	ຖ້າ (fp)
		fclose(fp);
	strbuf_release(&str);
	ສົ່ງຄືນ res;
}

static ຈຳນວນເຕັມ bisect_terms(sa_nracanaa bisect_terms *terms, const var_na *option)
{
	ຖ້າ (get_terms(terms, 0))
		ສົ່ງຄືນ error(_("no terms defined"));

	ຖ້າ (!option) {
		printf(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->term_good, terms->term_bad);
		ສົ່ງຄືນ 0;
	}
	ຖ້າ (one_of(option, "--term-good", "--term-old", NULL))
		printf("%s\n", terms->term_good);
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (one_of(option, "--term-bad", "--term-new", NULL))
		printf("%s\n", terms->term_bad);
	ບໍ່ດັ່ງນັ້ນ
		ສົ່ງຄືນ error(_("invalid argument %s for 'git bisect terms'.\n"
			       "Supported options are: "
			       "--term-good|--term-old and "
			       "--term-bad|--term-new."), option);

	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ bisect_append_log_quoted(const var_na **argv)
{
	ຈຳນວນເຕັມ res = 0;
	FILE *fp = fopen(git_path_bisect_log(), "a");
	sa_nracanaa strbuf orig_args = STRBUF_INIT;

	ຖ້າ (!fp)
		ສົ່ງຄືນ -1;

	ຖ້າ (fprintf(fp, "git bisect start") < 1) {
		res = -1;
		goto finish;
	}

	sq_quote_argv(&orig_args, argv);
	ຖ້າ (fprintf(fp, "%s\n", orig_args.buf) < 1)
		res = -1;

finish:
	fclose(fp);
	strbuf_release(&orig_args);
	ສົ່ງຄືນ res;
}

static ຈຳນວນເຕັມ add_bisect_ref(const sa_nracanaa reference *ref, void *cb)
{
	sa_nracanaa add_bisect_ref_data *data = cb;

	add_pending_oid(data->revs, ref->name, ref->oid, data->object_flags);

	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ prepare_revs(sa_nracanaa bisect_terms *terms, sa_nracanaa rev_info *revs)
{
	sa_nracanaa refs_for_each_ref_options opts = {
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};
	ຈຳນວນເຕັມ res = 0;
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

	ຖ້າ (prepare_revision_walk(revs))
		res = error(_("revision walk setup failed"));

	free(good);
	free(bad);
	ສົ່ງຄືນ res;
}

static ຈຳນວນເຕັມ bisect_skipped_commits(sa_nracanaa bisect_terms *terms)
{
	ຈຳນວນເຕັມ res;
	FILE *fp = NULL;
	sa_nracanaa rev_info revs;
	sa_nracanaa commit *commit;
	sa_nracanaa pretty_print_context pp = {0};
	sa_nracanaa strbuf commit_name = STRBUF_INIT;

	res = prepare_revs(terms, &revs);
	ຖ້າ (res)
		ສົ່ງຄືນ res;

	fp = fopen(git_path_bisect_log(), "a");
	ຖ້າ (!fp)
		ສົ່ງຄືນ error_errno(_("could not open '%s' for appending"),
				  git_path_bisect_log());

	ຖ້າ (fprintf(fp, "# only skipped commits left to test\n") < 0)
		ສົ່ງຄືນ error_errno(_("failed to write to '%s'"), git_path_bisect_log());

	ໃນຂະນະທີ່ ((commit = get_revision(&revs)) != NULL) {
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
	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ bisect_successful(sa_nracanaa bisect_terms *terms)
{
	sa_nracanaa object_id oid;
	sa_nracanaa commit *commit;
	sa_nracanaa pretty_print_context pp = {0};
	sa_nracanaa strbuf commit_name = STRBUF_INIT;
	var_na *bad_ref = xstrfmt("refs/bisect/%s",terms->term_bad);
	ຈຳນວນເຕັມ res;

	refs_read_ref(get_main_ref_store(the_repository), bad_ref, &oid);
	commit = lookup_commit_reference_by_name(bad_ref);
	ຖ້າ (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		free(bad_ref);
		ສົ່ງຄືນ BISECT_FAILED;
	}
	repo_format_commit_message(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_to_file(git_path_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->term_bad, oid_to_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	free(bad_ref);
	ສົ່ງຄືນ res;
}

static ga_nanaa bisect_error bisect_next(sa_nracanaa bisect_terms *terms,
				     const var_na *prefix)
{
	ga_nanaa bisect_error res;

	ຖ້າ (bisect_autostart(terms))
		ສົ່ງຄືນ BISECT_FAILED;

	ຖ້າ (bisect_next_check(terms, terms->term_good))
		ສົ່ງຄືນ BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_next_all(the_repository, prefix);

	ຖ້າ (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		res = bisect_successful(terms);
		ສົ່ງຄືນ res ? res : BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND;
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (res == BISECT_ONLY_SKIPPED_LEFT) {
		res = bisect_skipped_commits(terms);
		ສົ່ງຄືນ res ? res : BISECT_ONLY_SKIPPED_LEFT;
	}
	ສົ່ງຄືນ res;
}

static ga_nanaa bisect_error bisect_auto_next(sa_nracanaa bisect_terms *terms,
					  const var_na *prefix)
{
	ຖ້າ (bisect_next_check(terms, NULL)) {
		bisect_print_status(terms);
		ສົ່ງຄືນ BISECT_OK;
	}

	ສົ່ງຄືນ bisect_next(terms, prefix);
}

static ga_nanaa bisect_error bisect_start(sa_nracanaa bisect_terms *terms, ຈຳນວນເຕັມ argc,
				      const var_na **argv)
{
	ຈຳນວນເຕັມ no_checkout = 0;
	ຈຳນວນເຕັມ first_parent_only = 0;
	ຈຳນວນເຕັມ i, has_double_dash = 0, must_write_terms = 0, bad_seen = 0;
	ຈຳນວນເຕັມ flags, pathspec_pos;
	ga_nanaa bisect_error res = BISECT_OK;
	sa_nracanaa string_list revs = STRING_LIST_INIT_DUP;
	sa_nracanaa string_list states = STRING_LIST_INIT_DUP;
	sa_nracanaa strbuf start_head = STRBUF_INIT;
	sa_nracanaa strbuf bisect_names = STRBUF_INIT;
	sa_nracanaa object_id head_oid;
	sa_nracanaa object_id oid;
	ga_nanaa reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;
	const var_na *head;

	ຖ້າ (is_bare_repository(the_repository))
		no_checkout = 1;

	/*
	 * Check for one bad and then some good revisions
	 */
	ສຳລັບ (i = 0; i < argc; i++) {
		ຖ້າ (!strcmp(argv[i], "--")) {
			has_double_dash = 1;
			bha_nga;
		}
	}

	ສຳລັບ (i = 0; i < argc; i++) {
		const var_na *arg = argv[i];
		ຖ້າ (!strcmp(argv[i], "--")) {
			bha_nga;
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(arg, "--no-checkout")) {
			no_checkout = 1;
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(arg, "--first-parent")) {
			first_parent_only = 1;
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(arg, "--reset-when-found")) {
			reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (skip_prefix(arg, "--reset-when-found=", &arg)) {
			ຖ້າ (parse_reset_when_found(arg, &reset_when_found)) {
				res = BISECT_FAILED;
				goto finish;
			}
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(arg, "--term-good") ||
			 !strcmp(arg, "--term-old")) {
			i++;
			ຖ້າ (argc <= i)
				ສົ່ງຄືນ error(_("'' is not a valid term"));
			must_write_terms = 1;
			free((void *) terms->term_good);
			terms->term_good = xstrdup(argv[i]);
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (skip_prefix(arg, "--term-good=", &arg) ||
			   skip_prefix(arg, "--term-old=", &arg)) {
			must_write_terms = 1;
			free((void *) terms->term_good);
			terms->term_good = xstrdup(arg);
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (!strcmp(arg, "--term-bad") ||
			 !strcmp(arg, "--term-new")) {
			i++;
			ຖ້າ (argc <= i)
				ສົ່ງຄືນ error(_("'' is not a valid term"));
			must_write_terms = 1;
			free((void *) terms->term_bad);
			terms->term_bad = xstrdup(argv[i]);
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (skip_prefix(arg, "--term-bad=", &arg) ||
			   skip_prefix(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			free((void *) terms->term_bad);
			terms->term_bad = xstrdup(arg);
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (starts_with(arg, "--")) {
			ສົ່ງຄືນ error(_("unrecognized option: '%s'"), arg);
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (!get_oidf(&oid, "%s^{commit}", arg)) {
			string_list_append(&revs, oid_to_hex(&oid));
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (has_double_dash) {
			die(_("'%s' does not appear to be a valid "
			      "revision"), arg);
		} ບໍ່ດັ່ງນັ້ນ {
			bha_nga;
		}
	}
	ຖ້າ (reset_when_found != RESET_WHEN_FOUND_NONE && no_checkout) {
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
	ຖ້າ (revs.nr)
		must_write_terms = 1;
	ສຳລັບ (i = 0; i < revs.nr; i++) {
		ຖ້າ (bad_seen) {
			string_list_append(&states, terms->term_good);
		} ບໍ່ດັ່ງນັ້ນ {
			bad_seen = 1;
			string_list_append(&states, terms->term_bad);
		}
	}

	/*
	 * Verify HEAD
	 */
	head = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
				       "HEAD", 0, &head_oid, &flags);
	ຖ້າ (!head) {
		ຖ້າ (repo_get_oid(the_repository, "HEAD", &head_oid))
			ສົ່ງຄືນ error(_("bad HEAD - I need a HEAD"));
		head = "HEAD";
	}

	/*
	 * Check if we are bisecting
	 */
	ຖ້າ (!is_empty_or_missing_file(git_path_bisect_start())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&start_head, git_path_bisect_start(), 0);
		strbuf_trim(&start_head);
		ຖ້າ (!no_checkout) {
			sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.args, "checkout", start_head.buf,
				     "--", NULL);
			ຖ້າ (run_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       start_head.buf);
				goto finish;
			}
		}
	} ບໍ່ດັ່ງນັ້ນ {
		/* Get the rev from where we start. */
		ຖ້າ (!repo_get_oid(the_repository, head, &head_oid) &&
		    !starts_with(head, "refs/heads/")) {
			strbuf_reset(&start_head);
			strbuf_add_oid_hex(&start_head, &head_oid);
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (!repo_get_oid(the_repository, head, &head_oid) &&
			   skip_prefix(head, "refs/heads/", &head)) {
			strbuf_addstr(&start_head, head);
		} ບໍ່ດັ່ງນັ້ນ {
			ສົ່ງຄືນ error(_("bad HEAD - strange symbolic ref"));
		}
	}

	/*
	 * Get rid of any old bisect state.
	 */
	ຖ້າ (bisect_clean_state())
		ສົ່ງຄືນ BISECT_FAILED;

	/*
	 * Write new start state
	 */
	write_file(git_path_bisect_start(), "%s\n", start_head.buf);

	ຖ້າ (first_parent_only)
		write_file(git_path_bisect_first_parent(), "\n");

	ຖ້າ (reset_when_found != RESET_WHEN_FOUND_NONE)
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));

	ຖ້າ (no_checkout) {
		ຖ້າ (repo_get_oid(the_repository, start_head.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), start_head.buf);
			goto finish;
		}
		ຖ້າ (refs_update_ref(get_main_ref_store(the_repository), NULL, "BISECT_HEAD", &oid, NULL, 0,
				    UPDATE_REFS_MSG_ON_ERR)) {
			res = BISECT_FAILED;
			goto finish;
		}
	}

	ຖ້າ (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_names, argv + pathspec_pos);
	write_file(git_path_bisect_names(), "%s\n", bisect_names.buf);

	ສຳລັບ (i = 0; i < states.nr; i++)
		ຖ້າ (bisect_write(states.items[i].string,
				 revs.items[i].string, terms, 1)) {
			res = BISECT_FAILED;
			goto finish;
		}

	ຖ້າ (must_write_terms && write_terms(terms->term_bad,
					    terms->term_good)) {
		res = BISECT_FAILED;
		goto finish;
	}

	res = bisect_append_log_quoted(argv);
	ຖ້າ (res)
		res = BISECT_FAILED;

finish:
	string_list_clear(&revs, 0);
	string_list_clear(&states, 0);
	strbuf_release(&start_head);
	strbuf_release(&bisect_names);
	ຖ້າ (res)
		ສົ່ງຄືນ res;

	res = bisect_auto_next(terms, NULL);
	ຖ້າ (!is_bisect_success(res))
		bisect_clean_state();
	ສົ່ງຄືນ res;
}

static anta_hstha ຈຳນວນເຕັມ file_is_not_empty(const var_na *path)
{
	ສົ່ງຄືນ !is_empty_or_missing_file(path);
}

static ຈຳນວນເຕັມ bisect_autostart(sa_nracanaa bisect_terms *terms)
{
	ຈຳນວນເຕັມ res;
	const var_na *yesno;

	ຖ້າ (file_is_not_empty(git_path_bisect_start()))
		ສົ່ງຄືນ 0;

	fprintf_ln(stderr, _("You need to start by \"git bisect "
			  "start\"\n"));

	ຖ້າ (!isatty(STDIN_FILENO))
		ສົ່ງຄືນ -1;

	/*
	 * TRANSLATORS: Make sure to include [Y] and [n] in your
	 * translation. The program will only accept English input
	 * at this point.
	 */
	yesno = git_prompt(_("Do you want me to do it for you "
			     "[Y/n]? "), PROMPT_ECHO);
	res = tolower(*yesno) == 'n' ?
		-1 : bisect_start(terms, 0, empty_strvec);

	ສົ່ງຄືນ res;
}

static ga_nanaa bisect_error bisect_state(sa_nracanaa bisect_terms *terms, ຈຳນວນເຕັມ argc,
				      const var_na **argv)
{
	const var_na *state;
	ຈຳນວນເຕັມ i, verify_expected = 1;
	sa_nracanaa object_id oid, expected;
	sa_nracanaa oid_array revs = OID_ARRAY_INIT;

	ຖ້າ (!argc)
		ສົ່ງຄືນ error(_("Please call `--bisect-state` with at least one argument"));

	ຖ້າ (bisect_autostart(terms))
		ສົ່ງຄືນ BISECT_FAILED;

	state = argv[0];
	ຖ້າ (check_and_set_terms(terms, state) ||
	    !one_of(state, terms->term_good, terms->term_bad, "skip", NULL))
		ສົ່ງຄືນ BISECT_FAILED;

	argv++;
	argc--;
	ຖ້າ (argc > 1 && !strcmp(state, terms->term_bad))
		ສົ່ງຄືນ error(_("'git bisect %s' can take only one argument."), terms->term_bad);

	ຖ້າ (argc == 0) {
		const var_na *head = "BISECT_HEAD";
		ga_nanaa get_oid_result res_head = repo_get_oid(the_repository,
							    head, &oid);

		ຖ້າ (res_head == MISSING_OBJECT) {
			head = "HEAD";
			res_head = repo_get_oid(the_repository, head, &oid);
		}

		ຖ້າ (res_head)
			error(_("Bad rev input: %s"), head);
		oid_array_append(&revs, &oid);
	}

	/*
	 * All input revs must be checked before executing bisect_write()
	 * to discard junk revs.
	 */

	ສຳລັບ (; argc; argc--, argv++) {
		sa_nracanaa commit *commit;

		ຖ້າ (repo_get_oid(the_repository, *argv, &oid)){
			error(_("Bad rev input: %s"), *argv);
			oid_array_clear(&revs);
			ສົ່ງຄືນ BISECT_FAILED;
		}

		commit = lookup_commit_reference(the_repository, &oid);
		ຖ້າ (!commit)
			die(_("Bad rev input (not a commit): %s"), *argv);

		oid_array_append(&revs, &commit->object.oid);
	}

	ຖ້າ (refs_read_ref(get_main_ref_store(the_repository), "BISECT_EXPECTED_REV", &expected))
		verify_expected = 0; /* Ignore invalid file contents */

	ສຳລັບ (i = 0; i < revs.nr; i++) {
		ຖ້າ (bisect_write(state, oid_to_hex(&revs.oid[i]), terms, 0)) {
			oid_array_clear(&revs);
			ສົ່ງຄືນ BISECT_FAILED;
		}
		ຖ້າ (verify_expected && !oideq(&revs.oid[i], &expected)) {
			unlink_or_warn(git_path_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					NULL, "BISECT_EXPECTED_REV", NULL,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	ສົ່ງຄືນ bisect_auto_next(terms, NULL);
}

static ga_nanaa bisect_error bisect_log(void)
{
	ຈຳນວນເຕັມ fd, status;
	const var_na* filename = git_path_bisect_log();

	ຖ້າ (is_empty_or_missing_file(filename))
		ສົ່ງຄືນ error(_("We are not bisecting."));

	fd = open(filename, O_RDONLY);
	ຖ້າ (fd < 0)
		ສົ່ງຄືນ BISECT_FAILED;

	status = copy_fd(fd, STDOUT_FILENO);
	close(fd);
	ສົ່ງຄືນ status ? BISECT_FAILED : BISECT_OK;
}

static ຈຳນວນເຕັມ process_replay_line(sa_nracanaa bisect_terms *terms, sa_nracanaa strbuf *line)
{
	const var_na *p = line->buf + strspn(line->buf, " \t");
	var_na *word_end, *rev;

	ຖ້າ ((!skip_prefix(p, "git bisect", &p) &&
	!skip_prefix(p, "git-bisect", &p)) || !isspace(*p))
		ສົ່ງຄືນ 0;
	p += strspn(p, " \t");

	word_end = (var_na *)p + strcspn(p, " \t");
	rev = word_end + strspn(word_end, " \t");
	*word_end = '\0'; /* NUL-terminate the word */

	ຖ້າ (get_terms(terms, 1))
		ສົ່ງຄືນ error(_("no terms defined"));
	ຖ້າ (check_and_set_terms(terms, p))
		ສົ່ງຄືນ -1;

	ຖ້າ (!strcmp(p, "start")) {
		sa_nracanaa strvec argv = STRVEC_INIT;
		ຈຳນວນເຕັມ res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_start(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		ສົ່ງຄືນ res;
	}

	ຖ້າ (one_of(p, terms->term_good,
	   terms->term_bad, "skip", NULL))
		ສົ່ງຄືນ bisect_write(p, rev, terms, 0);

	ຖ້າ (!strcmp(p, "terms")) {
		sa_nracanaa strvec argv = STRVEC_INIT;
		ຈຳນວນເຕັມ res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_terms(terms, argv.nr == 1 ? argv.v[0] : NULL);
		strvec_clear(&argv);
		ສົ່ງຄືນ res;
	}
	error(_("'%s'?? what are you talking about?"), p);

	ສົ່ງຄືນ -1;
}

static ga_nanaa bisect_error bisect_replay(sa_nracanaa bisect_terms *terms, const var_na *filename)
{
	FILE *fp = NULL;
	ga_nanaa bisect_error res = BISECT_OK;
	sa_nracanaa strbuf line = STRBUF_INIT;

	ຖ້າ (is_empty_or_missing_file(filename))
		ສົ່ງຄືນ error(_("cannot read file '%s' for replaying"), filename);

	ຖ້າ (bisect_clean_state())
		ສົ່ງຄືນ BISECT_FAILED;

	fp = fopen(filename, "r");
	ຖ້າ (!fp)
		ສົ່ງຄືນ BISECT_FAILED;

	ໃນຂະນະທີ່ ((strbuf_getline(&line, fp) != EOF) && !res)
		res = process_replay_line(terms, &line);

	strbuf_release(&line);
	fclose(fp);

	ຖ້າ (res)
		ສົ່ງຄືນ BISECT_FAILED;

	ສົ່ງຄືນ bisect_auto_next(terms, NULL);
}

static ga_nanaa bisect_error bisect_skip(sa_nracanaa bisect_terms *terms, ຈຳນວນເຕັມ argc,
				     const var_na **argv)
{
	ຈຳນວນເຕັມ i;
	ga_nanaa bisect_error res;
	sa_nracanaa strvec argv_state = STRVEC_INIT;

	strvec_push(&argv_state, "skip");

	ສຳລັບ (i = 0; i < argc; i++) {
		const var_na *dotdot = strstr(argv[i], "..");

		ຖ້າ (dotdot) {
			sa_nracanaa rev_info revs;
			sa_nracanaa commit *commit;

			repo_init_revisions(the_repository, &revs, NULL);
			setup_revisions(2, argv + i - 1, &revs, NULL);

			ຖ້າ (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			ໃນຂະນະທີ່ ((commit = get_revision(&revs)) != NULL)
				strvec_push(&argv_state,
						oid_to_hex(&commit->object.oid));

			reset_revision_walk();
			release_revisions(&revs);
		} ບໍ່ດັ່ງນັ້ນ {
			strvec_push(&argv_state, argv[i]);
		}
	}
	res = bisect_state(terms, argv_state.nr, argv_state.v);

	strvec_clear(&argv_state);
	ສົ່ງຄືນ res;
}

static ຈຳນວນເຕັມ bisect_visualize(sa_nracanaa bisect_terms *terms, ຈຳນວນເຕັມ argc,
			    const var_na **argv)
{
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;
	sa_nracanaa strbuf sb = STRBUF_INIT;

	ຖ້າ (bisect_next_check(terms, NULL) != 0)
		ສົ່ງຄືນ BISECT_FAILED;

	cmd.no_stdin = 1;
	ຖ້າ (!argc) {
		ຖ້າ ((getenv("DISPLAY") || getenv("SESSIONNAME") || getenv("MSYSTEM") ||
		     getenv("SECURITYSESSIONID")) && exists_in_PATH("gitk")) {
			strvec_push(&cmd.args, "gitk");
		} ບໍ່ດັ່ງນັ້ນ {
			strvec_push(&cmd.args, "log");
			cmd.git_cmd = 1;
		}
	} ບໍ່ດັ່ງນັ້ນ {
		ຖ້າ (argv[0][0] == '-') {
			strvec_push(&cmd.args, "log");
			cmd.git_cmd = 1;
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (strcmp(argv[0], "tig") && !starts_with(argv[0], "git"))
			cmd.git_cmd = 1;

		strvec_pushv(&cmd.args, argv);
	}

	strvec_pushl(&cmd.args, "--bisect", "--", NULL);

	strbuf_read_file(&sb, git_path_bisect_names(), 0);
	sq_dequote_to_strvec(sb.buf, &cmd.args);
	strbuf_release(&sb);

	ສົ່ງຄືນ run_command(&cmd);
}

static ຈຳນວນເຕັມ get_first_good(const sa_nracanaa reference *ref, void *cb_data)
{
	oidcpy(cb_data, ref->oid);
	ສົ່ງຄືນ 1;
}

static ຈຳນວນເຕັມ do_bisect_run(const var_na *command)
{
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

	printf(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_push(&cmd.args, command);
	ສົ່ງຄືນ run_command(&cmd);
}

static ຈຳນວນເຕັມ verify_good(const sa_nracanaa bisect_terms *terms, const var_na *command)
{
	ຈຳນວນເຕັມ rc;
	ga_nanaa bisect_error res;
	sa_nracanaa object_id good_rev;
	sa_nracanaa object_id current_rev;
	var_na *good_glob = xstrfmt("%s-*", terms->term_good);
	ຈຳນວນເຕັມ no_checkout = refs_ref_exists(get_main_ref_store(the_repository),
					  "BISECT_HEAD");
	sa_nracanaa refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_first_good, &good_rev, &opts);
	free(good_glob);

	ຖ້າ (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		ສົ່ງຄືນ -1;

	res = bisect_checkout(&good_rev, no_checkout);
	ຖ້າ (res != BISECT_OK)
		ສົ່ງຄືນ -1;

	rc = do_bisect_run(command);

	res = bisect_checkout(&current_rev, no_checkout);
	ຖ້າ (res != BISECT_OK)
		ສົ່ງຄືນ -1;

	ສົ່ງຄືນ rc;
}

static ຈຳນວນເຕັມ bisect_run(sa_nracanaa bisect_terms *terms, ຈຳນວນເຕັມ argc, const var_na **argv)
{
	ຈຳນວນເຕັມ res = BISECT_OK;
	sa_nracanaa strbuf command = STRBUF_INIT;
	const var_na *reset_when_found_arg;
	const var_na *new_state;
	ຈຳນວນເຕັມ temporary_stdout_fd, saved_stdout;
	ຈຳນວນເຕັມ is_first_run = 1;
	ga_nanaa reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;

	ຖ້າ (bisect_next_check(terms, NULL))
		ສົ່ງຄືນ BISECT_FAILED;

	ຖ້າ (argc && !strcmp(argv[0], "--reset-when-found")) {
		reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (argc && skip_prefix(argv[0], "--reset-when-found=",
				    &reset_when_found_arg)) {
		ຖ້າ (parse_reset_when_found(reset_when_found_arg,
					   &reset_when_found))
			ສົ່ງຄືນ BISECT_FAILED;
	}

	ຖ້າ (reset_when_found != RESET_WHEN_FOUND_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		ສົ່ງຄືນ error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	ຖ້າ (reset_when_found != RESET_WHEN_FOUND_NONE) {
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));
		argc--;
		argv++;
	}

	ຖ້າ (!argc) {
		error(_("bisect run failed: no command provided."));
		ສົ່ງຄືນ BISECT_FAILED;
	}

	sq_quote_argv(&command, argv);
	strbuf_ltrim(&command);
	ໃນຂະນະທີ່ (1) {
		res = do_bisect_run(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		ຖ້າ (is_first_run && (res == 126 || res == 127)) {
			ຈຳນວນເຕັມ rc = verify_good(terms, command.buf);
			is_first_run = 0;
			ຖ້າ (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->term_good);
				res = BISECT_FAILED;
				bha_nga;
			}
			ຖ້າ (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->term_good);
				res = BISECT_FAILED;
				bha_nga;
			}
		}

		ຖ້າ (res < 0 || 128 <= res) {
			error(_("bisect run failed: exit code %d from"
				" %s is < 0 or >= 128"), res, command.buf);
			bha_nga;
		}

		ຖ້າ (res == 125)
			new_state = "skip";
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (!res)
			new_state = terms->term_good;
		ບໍ່ດັ່ງນັ້ນ
			new_state = terms->term_bad;

		temporary_stdout_fd = open(git_path_bisect_run(), O_CREAT | O_WRONLY | O_TRUNC, 0666);

		ຖ້າ (temporary_stdout_fd < 0) {
			res = error_errno(_("cannot open file '%s' for writing"), git_path_bisect_run());
			bha_nga;
		}

		fflush(stdout);
		saved_stdout = dup(1);
		ຖ້າ (saved_stdout < 0 ||
		    dup2(temporary_stdout_fd, 1) < 0) {
			res = error_errno(_("could not duplicate stdout"));
			ຖ້າ (saved_stdout >= 0)
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

		ຖ້າ (res == BISECT_ONLY_SKIPPED_LEFT)
			error(_("bisect run cannot continue any more"));
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (res == BISECT_INTERNAL_SUCCESS_MERGE_BASE) {
			puts(_("bisect run success"));
			res = BISECT_OK;
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
			printf(_("bisect found first '%s' commit\n"), terms->term_bad);
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (res) {
			error(_("bisect run failed: 'git bisect %s'"
				" exited with error code %d"), new_state, res);
		} ບໍ່ດັ່ງນັ້ນ {
			continue;
		}
		bha_nga;
	}

	strbuf_release(&command);
	ສົ່ງຄືນ res;
}

static ຈຳນວນເຕັມ cmd_bisect__reset(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix UNUSED,
			     sa_nracanaa repository *repo UNUSED)
{
	ຈຳນວນເຕັມ res;

	ຖ້າ (argc > 1)
		ສົ່ງຄືນ error(_("'%s' requires either no argument or a commit"),
			     "git bisect reset");
	res = bisect_reset(argc ? argv[0] : NULL, false);
	ຖ້າ (res)
		ສົ່ງຄືນ res;
	ສົ່ງຄືນ bisect_clean_state();
}

static ຈຳນວນເຕັມ cmd_bisect__terms(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix UNUSED,
			     sa_nracanaa repository *repo UNUSED)
{
	ຈຳນວນເຕັມ res;
	sa_nracanaa bisect_terms terms = { 0 };

	ຖ້າ (argc > 1)
		ສົ່ງຄືນ error(_("'%s' requires 0 or 1 argument"),
			     "git bisect terms");
	res = bisect_terms(&terms, argc == 1 ? argv[0] : NULL);
	free_terms(&terms);
	ສົ່ງຄືນ res;
}

static ຈຳນວນເຕັມ cmd_bisect__start(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix UNUSED,
			     sa_nracanaa repository *repo UNUSED)
{
	ຈຳນວນເຕັມ res;
	sa_nracanaa bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	res = bisect_start(&terms, argc, argv);
	free_terms(&terms);
	ສົ່ງຄືນ res;
}

static ຈຳນວນເຕັມ cmd_bisect__next(ຈຳນວນເຕັມ argc, const var_na **argv UNUSED, const var_na *prefix,
			    sa_nracanaa repository *repo UNUSED)
{
	ຈຳນວນເຕັມ res;
	sa_nracanaa bisect_terms terms = { 0 };

	ຖ້າ (argc)
		ສົ່ງຄືນ error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	ຖ້າ (get_terms(&terms, 1))
		ສົ່ງຄືນ error(_("no terms defined"));
	res = bisect_next(&terms, prefix);
	free_terms(&terms);
	ສົ່ງຄືນ res;
}

static ຈຳນວນເຕັມ cmd_bisect__log(ຈຳນວນເຕັມ argc UNUSED, const var_na **argv UNUSED,
			   const var_na *prefix UNUSED,
			   sa_nracanaa repository *repo UNUSED)
{
	ສົ່ງຄືນ bisect_log();
}

static ຈຳນວນເຕັມ cmd_bisect__replay(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix UNUSED,
			      sa_nracanaa repository *repo UNUSED)
{
	ຈຳນວນເຕັມ res;
	sa_nracanaa bisect_terms terms = { 0 };

	ຖ້າ (argc != 1)
		ສົ່ງຄືນ error(_("no logfile given"));
	set_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	ສົ່ງຄືນ res;
}

static ຈຳນວນເຕັມ cmd_bisect__skip(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix UNUSED,
			    sa_nracanaa repository *repo UNUSED)
{
	ຈຳນວນເຕັມ res;
	sa_nracanaa bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	ຖ້າ (get_terms(&terms, 1))
		ສົ່ງຄືນ error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	ສົ່ງຄືນ res;
}

static ຈຳນວນເຕັມ cmd_bisect__visualize(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix UNUSED,
				 sa_nracanaa repository *repo UNUSED)
{
	ຈຳນວນເຕັມ res;
	sa_nracanaa bisect_terms terms = { 0 };

	ຖ້າ (get_terms(&terms, 1))
		ສົ່ງຄືນ error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	ສົ່ງຄືນ res;
}

static ຈຳນວນເຕັມ cmd_bisect__run(ຈຳນວນເຕັມ argc, const var_na **argv, const var_na *prefix UNUSED,
			   sa_nracanaa repository *repo UNUSED)
{
	ຈຳນວນເຕັມ res;
	sa_nracanaa bisect_terms terms = { 0 };

	ຖ້າ (!argc)
		ສົ່ງຄືນ error(_("'%s' failed: no command provided."), "git bisect run");
	ຖ້າ (get_terms(&terms, 1))
		ສົ່ງຄືນ error(_("no terms defined"));
	res = bisect_run(&terms, argc, argv);
	free_terms(&terms);
	ສົ່ງຄືນ res;
}

ຈຳນວນເຕັມ cmd_bisect(ຈຳນວນເຕັມ argc,
	       const var_na **argv,
	       const var_na *prefix,
	       sa_nracanaa repository *repo)
{
	ຈຳນວນເຕັມ res = 0;
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

	ຖ້າ (!fn) {
		sa_nracanaa bisect_terms terms = { 0 };

		ຖ້າ (!argc)
			usage_msg_opt(_("need a command"), git_bisect_usage, options);

		ຖ້າ (!strcmp(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		set_terms(&terms, "bad", "good");
		ຖ້າ (get_terms(&terms, 1))
			ສົ່ງຄືນ error(_("no terms defined"));
		ຖ້າ (check_and_set_terms(&terms, argv[0]) ||
		    !one_of(argv[0], terms.term_good, terms.term_bad, NULL))
			usage_msg_optf(_("unknown command: '%s'"), git_bisect_usage,
				       options, argv[0]);
		ບໍ່ດັ່ງນັ້ນ
			res = bisect_state(&terms, argc, argv);
		free_terms(&terms);
	} ບໍ່ດັ່ງນັ້ນ {
		argc--;
		argv++;
		res = fn(argc, argv, prefix, repo);
	}

	ຖ້າ (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		ga_nanaa reset_when_found_mode mode;

		ຖ້າ (read_reset_when_found(&mode))
			res = BISECT_FAILED;
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (mode != RESET_WHEN_FOUND_NONE &&
			 bisect_reset_when_found(mode))
			res = BISECT_FAILED;
	}

	ສົ່ງຄືນ is_bisect_success(res) ? 0 : -res;
}
