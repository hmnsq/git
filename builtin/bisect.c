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
	unsigned ཧྲིལ object_flags;
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

static ཧྲིལ bisect_autostart(sa_nracanaa bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
LAST_ARG_MUST_BE_NULL
static ཧྲིལ one_of(const var_na *term, ...)
{
	ཧྲིལ res = 0;
	va_list matches;
	const var_na *match;

	va_start(matches, term);
	སྐབས (!res && (match = va_arg(matches, const var_na *)))
		res = !strcmp(term, match);
	va_end(matches);

	ལོག res;
}

/*
 * return code BISECT_INTERNAL_SUCCESS_MERGE_BASE
 * and BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND are codes
 * that indicate special success.
 */

static ཧྲིལ is_bisect_success(ga_nanaa bisect_error res)
{
	ལོག !res ||
		res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND ||
		res == BISECT_INTERNAL_SUCCESS_MERGE_BASE;
}

static ཧྲིལ write_in_file(const var_na *path, const var_na *mode, const var_na *format, va_list args)
{
	FILE *fp = NULL;
	ཧྲིལ res = 0;

	གལ་སྲིད (strcmp(mode, "w") && strcmp(mode, "a"))
		BUG("write-in-file does not support '%s' mode", mode);
	fp = fopen(path, mode);
	གལ་སྲིད (!fp)
		ལོག error_errno(_("cannot open file '%s' in mode '%s'"), path, mode);
	res = vfprintf(fp, format, args);

	གལ་སྲིད (res < 0) {
		ཧྲིལ saved_errno = errno;
		fclose(fp);
		errno = saved_errno;
		ལོག error_errno(_("could not write to file '%s'"), path);
	}

	ལོག fclose(fp);
}

__attribute__((format (printf, 2, 3)))
static ཧྲིལ write_to_file(const var_na *path, const var_na *format, ...)
{
	ཧྲིལ res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "w", format, args);
	va_end(args);

	ལོག res;
}

__attribute__((format (printf, 2, 3)))
static ཧྲིལ append_to_file(const var_na *path, const var_na *format, ...)
{
	ཧྲིལ res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "a", format, args);
	va_end(args);

	ལོག res;
}

static ཧྲིལ print_file_to_stdout(const var_na *path)
{
	ཧྲིལ fd = open(path, O_RDONLY);
	ཧྲིལ ret = 0;

	གལ་སྲིད (fd < 0)
		ལོག error_errno(_("cannot open file '%s' for reading"), path);
	གལ་སྲིད (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), path);
	close(fd);
	ལོག ret;
}

static ཧྲིལ check_term_format(const var_na *term, const var_na *orig_term)
{
	ཧྲིལ res;
	var_na *new_term = xstrfmt("refs/bisect/%s", term);

	res = check_refname_format(new_term, 0);
	free(new_term);

	གལ་སྲིད (res)
		ལོག error(_("'%s' is not a valid term"), term);

	གལ་སྲིད (one_of(term, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", NULL))
		ལོག error(_("can't use the builtin command '%s' as a term"), term);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	གལ་སྲིད ((strcmp(orig_term, "bad") && one_of(term, "bad", "new", NULL)) ||
		 (strcmp(orig_term, "good") && one_of(term, "good", "old", NULL)))
		ལོག error(_("can't change the meaning of the term '%s'"), term);

	ལོག 0;
}

static ཧྲིལ write_terms(const var_na *bad, const var_na *good)
{
	ཧྲིལ res;

	གལ་སྲིད (!strcmp(bad, good))
		ལོག error(_("please use two different terms"));

	གལ་སྲིད (check_term_format(bad, "bad") || check_term_format(good, "good"))
		ལོག -1;

	res = write_to_file(git_path_bisect_terms(), "%s\n%s\n", bad, good);

	ལོག res;
}

static ཧྲིལ bisect_reset(const var_na *commit, bool quiet)
{
	sa_nracanaa strbuf branch = STRBUF_INIT;

	གལ་སྲིད (!commit) {
		གལ་སྲིད (!strbuf_read_file(&branch, git_path_bisect_start(), 0))
			printf(_("We are not bisecting.\n"));
		དེ་མིན
			strbuf_rtrim(&branch);
	} དེ་མིན {
		sa_nracanaa object_id oid;

		གལ་སྲིད (repo_get_oid_commit(the_repository, commit, &oid))
			ལོག error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	གལ་སྲིད (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "checkout", "--ignore-other-worktrees", NULL);
		གལ་སྲིད (quiet)
			strvec_push(&cmd.args, "--quiet");
		strvec_pushl(&cmd.args, branch.buf, "--", NULL);
		གལ་སྲིད (run_command(&cmd)) {
			error(_("could not check out original"
				" HEAD '%s'. Try 'git bisect"
				" reset <commit>'."), branch.buf);
			strbuf_release(&branch);
			ལོག -1;
		}
	}

	strbuf_release(&branch);
	ལོག 0;
}

static ཧྲིལ parse_reset_when_found(const var_na *value,
				  ga_nanaa reset_when_found_mode *mode)
{
	གལ་སྲིད (!strcmp(value, "original"))
		*mode = RESET_WHEN_FOUND_TO_ORIGINAL;
	དེ་མིན གལ་སྲིད (!strcmp(value, "found"))
		*mode = RESET_WHEN_FOUND_TO_FOUND;
	དེ་མིན
		ལོག error(_("invalid value for '--reset-when-found': '%s'"),
			     value);

	ལོག 0;
}

static const var_na *reset_when_found_mode_name(ga_nanaa reset_when_found_mode mode)
{
	switch (mode) {
	pak_sa RESET_WHEN_FOUND_TO_ORIGINAL:
		ལོག "original";
	pak_sa RESET_WHEN_FOUND_TO_FOUND:
		ལོག "found";
	pak_sa RESET_WHEN_FOUND_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", mode);
}

static ཧྲིལ read_reset_when_found(ga_nanaa reset_when_found_mode *mode)
{
	sa_nracanaa strbuf value = STRBUF_INIT;
	ཧྲིལ res = 0;

	*mode = RESET_WHEN_FOUND_NONE;
	གལ་སྲིད (is_empty_or_missing_file(git_path_bisect_reset_when_found()))
		ལོག 0;

	གལ་སྲིད (strbuf_read_file(&value, git_path_bisect_reset_when_found(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_path_bisect_reset_when_found());
		goto out;
	}
	strbuf_trim(&value);
	གལ་སྲིད (parse_reset_when_found(value.buf, mode))
		res = -1;

out:
	strbuf_release(&value);
	ལོག res;
}

static ཧྲིལ bisect_reset_when_found(ga_nanaa reset_when_found_mode mode)
{
	sa_nracanaa bisect_terms terms = { 0 };
	var_na *commit = NULL;
	ཧྲིལ res;

	གལ་སྲིད (mode == RESET_WHEN_FOUND_TO_FOUND) {
		read_bisect_terms(&terms.term_bad, &terms.term_good);
		commit = xstrfmt("refs/bisect/%s", terms.term_bad);
	} དེ་མིན གལ་སྲིད (mode == RESET_WHEN_FOUND_NONE) {
		BUG("automatic reset requested without a reset mode");
	}

	res = bisect_reset(commit, true);
	གལ་སྲིད (!res)
		res = bisect_clean_state();

	free(commit);
	free_terms(&terms);
	ལོག res;
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

static ཧྲིལ bisect_write(const var_na *state, const var_na *rev,
			const sa_nracanaa bisect_terms *terms, ཧྲིལ nolog)
{
	sa_nracanaa strbuf tag = STRBUF_INIT;
	sa_nracanaa object_id oid;
	sa_nracanaa commit *commit;
	FILE *fp = NULL;
	ཧྲིལ res = 0;

	གལ་སྲིད (!strcmp(state, terms->term_bad)) {
		strbuf_addf(&tag, "refs/bisect/%s", state);
	} དེ་མིན གལ་སྲིད (one_of(state, terms->term_good, "skip", NULL)) {
		strbuf_addf(&tag, "refs/bisect/%s-%s", state, rev);
	} དེ་མིན {
		res = error(_("Bad bisect_write argument: %s"), state);
		goto finish;
	}

	གལ་སྲིད (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		goto finish;
	}

	གལ་སྲིད (refs_update_ref(get_main_ref_store(the_repository), NULL, tag.buf, &oid, NULL, 0,
			    UPDATE_REFS_MSG_ON_ERR)) {
		res = -1;
		goto finish;
	}

	fp = fopen(git_path_bisect_log(), "a");
	གལ་སྲིད (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_path_bisect_log());
		goto finish;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	གལ་སྲིད (!nolog)
		fprintf(fp, "git bisect %s %s\n", state, rev);

finish:
	གལ་སྲིད (fp)
		fclose(fp);
	strbuf_release(&tag);
	ལོག res;
}

static ཧྲིལ check_and_set_terms(sa_nracanaa bisect_terms *terms, const var_na *cmd)
{
	ཧྲིལ has_term_file = !is_empty_or_missing_file(git_path_bisect_terms());

	གལ་སྲིད (one_of(cmd, "skip", "start", "terms", NULL))
		ལོག 0;

	གལ་སྲིད (has_term_file && strcmp(cmd, terms->term_bad) &&
	    strcmp(cmd, terms->term_good))
		ལོག error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->term_bad,
				terms->term_good);

	གལ་སྲིད (!has_term_file) {
		གལ་སྲིད (one_of(cmd, "bad", "good", NULL)) {
			set_terms(terms, "bad", "good");
			ལོག write_terms(terms->term_bad, terms->term_good);
		}
		གལ་སྲིད (one_of(cmd, "new", "old", NULL)) {
			set_terms(terms, "new", "old");
			ལོག write_terms(terms->term_bad, terms->term_good);
		}
	}

	ལོག 0;
}

static ཧྲིལ inc_nr(const sa_nracanaa reference *ref UNUSED, void *cb_data)
{
	unsigned ཧྲིལ *nr = (unsigned ཧྲིལ *)cb_data;
	(*nr)++;
	ལོག 0;
}

static const var_na need_bad_and_good_revision_warning[] =
	N_("You need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

static const var_na need_bisect_start_warning[] =
	N_("You need to start by \"git bisect start\".\n"
	   "You then need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

static ཧྲིལ decide_next(const sa_nracanaa bisect_terms *terms,
		       const var_na *current_term, ཧྲིལ missing_good,
		       ཧྲིལ missing_bad)
{
	གལ་སྲིད (!missing_good && !missing_bad)
		ལོག 0;
	གལ་སྲིད (!current_term)
		ལོག -1;

	གལ་སྲིད (missing_good && !missing_bad &&
	    !strcmp(current_term, terms->term_good)) {
		var_na *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->term_bad);
		གལ་སྲིད (!isatty(0))
			ལོག 0;
		/*
		 * TRANSLATORS: Make sure to include [Y] and [n] in your
		 * translation. The program will only accept English input
		 * at this point.
		 */
		yesno = git_prompt(_("Are you sure [Y/n]? "), PROMPT_ECHO);
		གལ་སྲིད (starts_with(yesno, "N") || starts_with(yesno, "n"))
			ལོག -1;
		ལོག 0;
	}

	གལ་སྲིད (!is_empty_or_missing_file(git_path_bisect_start()))
		ལོག error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	དེ་མིན
		ལོག error(_(need_bisect_start_warning),
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

	གལ་སྲིད (refs_ref_exists(get_main_ref_store(the_repository), bad_ref))
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
	གལ་སྲིད (state.nr_good && state.nr_bad)
		ལོག;

	གལ་སྲིད (!state.nr_good && !state.nr_bad)
		bisect_log_printf(_("status: waiting for both '%s' and '%s' commits\n"),
				  terms->term_good, terms->term_bad);
	དེ་མིན གལ་སྲིད (state.nr_good)
		bisect_log_printf(Q_("status: waiting for '%s' commit, %d '%s' commit known\n",
				     "status: waiting for '%s' commit, %d '%s' commits known\n",
				     state.nr_good),
				  terms->term_bad, state.nr_good, terms->term_good);
	དེ་མིན
		bisect_log_printf(_("status: waiting for '%s' commit(s), '%s' commit known\n"),
				  terms->term_good, terms->term_bad);
}

static ཧྲིལ bisect_next_check(const sa_nracanaa bisect_terms *terms,
			     const var_na *current_term)
{
	sa_nracanaa bisect_state state = { 0 };
	bisect_status(&state, terms);
	ལོག decide_next(terms, current_term, !state.nr_good, !state.nr_bad);
}

static ཧྲིལ get_terms(sa_nracanaa bisect_terms *terms, ཧྲིལ file_missing_is_ok)
{
	sa_nracanaa strbuf str = STRBUF_INIT;
	FILE *fp = NULL;
	ཧྲིལ res = 0;

	fp = fopen(git_path_bisect_terms(), "r");
	གལ་སྲིད (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		goto finish;
	}

	free_terms(terms);
	གལ་སྲིད (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		goto finish;
	}
	terms->term_bad = strbuf_detach(&str, NULL);
	གལ་སྲིད (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		FREE_AND_NULL(terms->term_bad);
		goto finish;
	}
	terms->term_good = strbuf_detach(&str, NULL);

finish:
	གལ་སྲིད (fp)
		fclose(fp);
	strbuf_release(&str);
	ལོག res;
}

static ཧྲིལ bisect_terms(sa_nracanaa bisect_terms *terms, const var_na *option)
{
	གལ་སྲིད (get_terms(terms, 0))
		ལོག error(_("no terms defined"));

	གལ་སྲིད (!option) {
		printf(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->term_good, terms->term_bad);
		ལོག 0;
	}
	གལ་སྲིད (one_of(option, "--term-good", "--term-old", NULL))
		printf("%s\n", terms->term_good);
	དེ་མིན གལ་སྲིད (one_of(option, "--term-bad", "--term-new", NULL))
		printf("%s\n", terms->term_bad);
	དེ་མིན
		ལོག error(_("invalid argument %s for 'git bisect terms'.\n"
			       "Supported options are: "
			       "--term-good|--term-old and "
			       "--term-bad|--term-new."), option);

	ལོག 0;
}

static ཧྲིལ bisect_append_log_quoted(const var_na **argv)
{
	ཧྲིལ res = 0;
	FILE *fp = fopen(git_path_bisect_log(), "a");
	sa_nracanaa strbuf orig_args = STRBUF_INIT;

	གལ་སྲིད (!fp)
		ལོག -1;

	གལ་སྲིད (fprintf(fp, "git bisect start") < 1) {
		res = -1;
		goto finish;
	}

	sq_quote_argv(&orig_args, argv);
	གལ་སྲིད (fprintf(fp, "%s\n", orig_args.buf) < 1)
		res = -1;

finish:
	fclose(fp);
	strbuf_release(&orig_args);
	ལོག res;
}

static ཧྲིལ add_bisect_ref(const sa_nracanaa reference *ref, void *cb)
{
	sa_nracanaa add_bisect_ref_data *data = cb;

	add_pending_oid(data->revs, ref->name, ref->oid, data->object_flags);

	ལོག 0;
}

static ཧྲིལ prepare_revs(sa_nracanaa bisect_terms *terms, sa_nracanaa rev_info *revs)
{
	sa_nracanaa refs_for_each_ref_options opts = {
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};
	ཧྲིལ res = 0;
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

	གལ་སྲིད (prepare_revision_walk(revs))
		res = error(_("revision walk setup failed"));

	free(good);
	free(bad);
	ལོག res;
}

static ཧྲིལ bisect_skipped_commits(sa_nracanaa bisect_terms *terms)
{
	ཧྲིལ res;
	FILE *fp = NULL;
	sa_nracanaa rev_info revs;
	sa_nracanaa commit *commit;
	sa_nracanaa pretty_print_context pp = {0};
	sa_nracanaa strbuf commit_name = STRBUF_INIT;

	res = prepare_revs(terms, &revs);
	གལ་སྲིད (res)
		ལོག res;

	fp = fopen(git_path_bisect_log(), "a");
	གལ་སྲིད (!fp)
		ལོག error_errno(_("could not open '%s' for appending"),
				  git_path_bisect_log());

	གལ་སྲིད (fprintf(fp, "# only skipped commits left to test\n") < 0)
		ལོག error_errno(_("failed to write to '%s'"), git_path_bisect_log());

	སྐབས ((commit = get_revision(&revs)) != NULL) {
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
	ལོག 0;
}

static ཧྲིལ bisect_successful(sa_nracanaa bisect_terms *terms)
{
	sa_nracanaa object_id oid;
	sa_nracanaa commit *commit;
	sa_nracanaa pretty_print_context pp = {0};
	sa_nracanaa strbuf commit_name = STRBUF_INIT;
	var_na *bad_ref = xstrfmt("refs/bisect/%s",terms->term_bad);
	ཧྲིལ res;

	refs_read_ref(get_main_ref_store(the_repository), bad_ref, &oid);
	commit = lookup_commit_reference_by_name(bad_ref);
	གལ་སྲིད (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		free(bad_ref);
		ལོག BISECT_FAILED;
	}
	repo_format_commit_message(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_to_file(git_path_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->term_bad, oid_to_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	free(bad_ref);
	ལོག res;
}

static ga_nanaa bisect_error bisect_next(sa_nracanaa bisect_terms *terms,
				     const var_na *prefix)
{
	ga_nanaa bisect_error res;

	གལ་སྲིད (bisect_autostart(terms))
		ལོག BISECT_FAILED;

	གལ་སྲིད (bisect_next_check(terms, terms->term_good))
		ལོག BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_next_all(the_repository, prefix);

	གལ་སྲིད (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		res = bisect_successful(terms);
		ལོག res ? res : BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND;
	} དེ་མིན གལ་སྲིད (res == BISECT_ONLY_SKIPPED_LEFT) {
		res = bisect_skipped_commits(terms);
		ལོག res ? res : BISECT_ONLY_SKIPPED_LEFT;
	}
	ལོག res;
}

static ga_nanaa bisect_error bisect_auto_next(sa_nracanaa bisect_terms *terms,
					  const var_na *prefix)
{
	གལ་སྲིད (bisect_next_check(terms, NULL)) {
		bisect_print_status(terms);
		ལོག BISECT_OK;
	}

	ལོག bisect_next(terms, prefix);
}

static ga_nanaa bisect_error bisect_start(sa_nracanaa bisect_terms *terms, ཧྲིལ argc,
				      const var_na **argv)
{
	ཧྲིལ no_checkout = 0;
	ཧྲིལ first_parent_only = 0;
	ཧྲིལ i, has_double_dash = 0, must_write_terms = 0, bad_seen = 0;
	ཧྲིལ flags, pathspec_pos;
	ga_nanaa bisect_error res = BISECT_OK;
	sa_nracanaa string_list revs = STRING_LIST_INIT_DUP;
	sa_nracanaa string_list states = STRING_LIST_INIT_DUP;
	sa_nracanaa strbuf start_head = STRBUF_INIT;
	sa_nracanaa strbuf bisect_names = STRBUF_INIT;
	sa_nracanaa object_id head_oid;
	sa_nracanaa object_id oid;
	ga_nanaa reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;
	const var_na *head;

	གལ་སྲིད (is_bare_repository(the_repository))
		no_checkout = 1;

	/*
	 * Check for one bad and then some good revisions
	 */
	དོན་ལུ (i = 0; i < argc; i++) {
		གལ་སྲིད (!strcmp(argv[i], "--")) {
			has_double_dash = 1;
			bha_nga;
		}
	}

	དོན་ལུ (i = 0; i < argc; i++) {
		const var_na *arg = argv[i];
		གལ་སྲིད (!strcmp(argv[i], "--")) {
			bha_nga;
		} དེ་མིན གལ་སྲིད (!strcmp(arg, "--no-checkout")) {
			no_checkout = 1;
		} དེ་མིན གལ་སྲིད (!strcmp(arg, "--first-parent")) {
			first_parent_only = 1;
		} དེ་མིན གལ་སྲིད (!strcmp(arg, "--reset-when-found")) {
			reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
		} དེ་མིན གལ་སྲིད (skip_prefix(arg, "--reset-when-found=", &arg)) {
			གལ་སྲིད (parse_reset_when_found(arg, &reset_when_found)) {
				res = BISECT_FAILED;
				goto finish;
			}
		} དེ་མིན གལ་སྲིད (!strcmp(arg, "--term-good") ||
			 !strcmp(arg, "--term-old")) {
			i++;
			གལ་སྲིད (argc <= i)
				ལོག error(_("'' is not a valid term"));
			must_write_terms = 1;
			free((void *) terms->term_good);
			terms->term_good = xstrdup(argv[i]);
		} དེ་མིན གལ་སྲིད (skip_prefix(arg, "--term-good=", &arg) ||
			   skip_prefix(arg, "--term-old=", &arg)) {
			must_write_terms = 1;
			free((void *) terms->term_good);
			terms->term_good = xstrdup(arg);
		} དེ་མིན གལ་སྲིད (!strcmp(arg, "--term-bad") ||
			 !strcmp(arg, "--term-new")) {
			i++;
			གལ་སྲིད (argc <= i)
				ལོག error(_("'' is not a valid term"));
			must_write_terms = 1;
			free((void *) terms->term_bad);
			terms->term_bad = xstrdup(argv[i]);
		} དེ་མིན གལ་སྲིད (skip_prefix(arg, "--term-bad=", &arg) ||
			   skip_prefix(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			free((void *) terms->term_bad);
			terms->term_bad = xstrdup(arg);
		} དེ་མིན གལ་སྲིད (starts_with(arg, "--")) {
			ལོག error(_("unrecognized option: '%s'"), arg);
		} དེ་མིན གལ་སྲིད (!get_oidf(&oid, "%s^{commit}", arg)) {
			string_list_append(&revs, oid_to_hex(&oid));
		} དེ་མིན གལ་སྲིད (has_double_dash) {
			die(_("'%s' does not appear to be a valid "
			      "revision"), arg);
		} དེ་མིན {
			bha_nga;
		}
	}
	གལ་སྲིད (reset_when_found != RESET_WHEN_FOUND_NONE && no_checkout) {
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
	གལ་སྲིད (revs.nr)
		must_write_terms = 1;
	དོན་ལུ (i = 0; i < revs.nr; i++) {
		གལ་སྲིད (bad_seen) {
			string_list_append(&states, terms->term_good);
		} དེ་མིན {
			bad_seen = 1;
			string_list_append(&states, terms->term_bad);
		}
	}

	/*
	 * Verify HEAD
	 */
	head = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
				       "HEAD", 0, &head_oid, &flags);
	གལ་སྲིད (!head) {
		གལ་སྲིད (repo_get_oid(the_repository, "HEAD", &head_oid))
			ལོག error(_("bad HEAD - I need a HEAD"));
		head = "HEAD";
	}

	/*
	 * Check if we are bisecting
	 */
	གལ་སྲིད (!is_empty_or_missing_file(git_path_bisect_start())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&start_head, git_path_bisect_start(), 0);
		strbuf_trim(&start_head);
		གལ་སྲིད (!no_checkout) {
			sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.args, "checkout", start_head.buf,
				     "--", NULL);
			གལ་སྲིད (run_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       start_head.buf);
				goto finish;
			}
		}
	} དེ་མིན {
		/* Get the rev from where we start. */
		གལ་སྲིད (!repo_get_oid(the_repository, head, &head_oid) &&
		    !starts_with(head, "refs/heads/")) {
			strbuf_reset(&start_head);
			strbuf_add_oid_hex(&start_head, &head_oid);
		} དེ་མིན གལ་སྲིད (!repo_get_oid(the_repository, head, &head_oid) &&
			   skip_prefix(head, "refs/heads/", &head)) {
			strbuf_addstr(&start_head, head);
		} དེ་མིན {
			ལོག error(_("bad HEAD - strange symbolic ref"));
		}
	}

	/*
	 * Get rid of any old bisect state.
	 */
	གལ་སྲིད (bisect_clean_state())
		ལོག BISECT_FAILED;

	/*
	 * Write new start state
	 */
	write_file(git_path_bisect_start(), "%s\n", start_head.buf);

	གལ་སྲིད (first_parent_only)
		write_file(git_path_bisect_first_parent(), "\n");

	གལ་སྲིད (reset_when_found != RESET_WHEN_FOUND_NONE)
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));

	གལ་སྲིད (no_checkout) {
		གལ་སྲིད (repo_get_oid(the_repository, start_head.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), start_head.buf);
			goto finish;
		}
		གལ་སྲིད (refs_update_ref(get_main_ref_store(the_repository), NULL, "BISECT_HEAD", &oid, NULL, 0,
				    UPDATE_REFS_MSG_ON_ERR)) {
			res = BISECT_FAILED;
			goto finish;
		}
	}

	གལ་སྲིད (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_names, argv + pathspec_pos);
	write_file(git_path_bisect_names(), "%s\n", bisect_names.buf);

	དོན་ལུ (i = 0; i < states.nr; i++)
		གལ་སྲིད (bisect_write(states.items[i].string,
				 revs.items[i].string, terms, 1)) {
			res = BISECT_FAILED;
			goto finish;
		}

	གལ་སྲིད (must_write_terms && write_terms(terms->term_bad,
					    terms->term_good)) {
		res = BISECT_FAILED;
		goto finish;
	}

	res = bisect_append_log_quoted(argv);
	གལ་སྲིད (res)
		res = BISECT_FAILED;

finish:
	string_list_clear(&revs, 0);
	string_list_clear(&states, 0);
	strbuf_release(&start_head);
	strbuf_release(&bisect_names);
	གལ་སྲིད (res)
		ལོག res;

	res = bisect_auto_next(terms, NULL);
	གལ་སྲིད (!is_bisect_success(res))
		bisect_clean_state();
	ལོག res;
}

static anta_hstha ཧྲིལ file_is_not_empty(const var_na *path)
{
	ལོག !is_empty_or_missing_file(path);
}

static ཧྲིལ bisect_autostart(sa_nracanaa bisect_terms *terms)
{
	ཧྲིལ res;
	const var_na *yesno;

	གལ་སྲིད (file_is_not_empty(git_path_bisect_start()))
		ལོག 0;

	fprintf_ln(stderr, _("You need to start by \"git bisect "
			  "start\"\n"));

	གལ་སྲིད (!isatty(STDIN_FILENO))
		ལོག -1;

	/*
	 * TRANSLATORS: Make sure to include [Y] and [n] in your
	 * translation. The program will only accept English input
	 * at this point.
	 */
	yesno = git_prompt(_("Do you want me to do it for you "
			     "[Y/n]? "), PROMPT_ECHO);
	res = tolower(*yesno) == 'n' ?
		-1 : bisect_start(terms, 0, empty_strvec);

	ལོག res;
}

static ga_nanaa bisect_error bisect_state(sa_nracanaa bisect_terms *terms, ཧྲིལ argc,
				      const var_na **argv)
{
	const var_na *state;
	ཧྲིལ i, verify_expected = 1;
	sa_nracanaa object_id oid, expected;
	sa_nracanaa oid_array revs = OID_ARRAY_INIT;

	གལ་སྲིད (!argc)
		ལོག error(_("Please call `--bisect-state` with at least one argument"));

	གལ་སྲིད (bisect_autostart(terms))
		ལོག BISECT_FAILED;

	state = argv[0];
	གལ་སྲིད (check_and_set_terms(terms, state) ||
	    !one_of(state, terms->term_good, terms->term_bad, "skip", NULL))
		ལོག BISECT_FAILED;

	argv++;
	argc--;
	གལ་སྲིད (argc > 1 && !strcmp(state, terms->term_bad))
		ལོག error(_("'git bisect %s' can take only one argument."), terms->term_bad);

	གལ་སྲིད (argc == 0) {
		const var_na *head = "BISECT_HEAD";
		ga_nanaa get_oid_result res_head = repo_get_oid(the_repository,
							    head, &oid);

		གལ་སྲིད (res_head == MISSING_OBJECT) {
			head = "HEAD";
			res_head = repo_get_oid(the_repository, head, &oid);
		}

		གལ་སྲིད (res_head)
			error(_("Bad rev input: %s"), head);
		oid_array_append(&revs, &oid);
	}

	/*
	 * All input revs must be checked before executing bisect_write()
	 * to discard junk revs.
	 */

	དོན་ལུ (; argc; argc--, argv++) {
		sa_nracanaa commit *commit;

		གལ་སྲིད (repo_get_oid(the_repository, *argv, &oid)){
			error(_("Bad rev input: %s"), *argv);
			oid_array_clear(&revs);
			ལོག BISECT_FAILED;
		}

		commit = lookup_commit_reference(the_repository, &oid);
		གལ་སྲིད (!commit)
			die(_("Bad rev input (not a commit): %s"), *argv);

		oid_array_append(&revs, &commit->object.oid);
	}

	གལ་སྲིད (refs_read_ref(get_main_ref_store(the_repository), "BISECT_EXPECTED_REV", &expected))
		verify_expected = 0; /* Ignore invalid file contents */

	དོན་ལུ (i = 0; i < revs.nr; i++) {
		གལ་སྲིད (bisect_write(state, oid_to_hex(&revs.oid[i]), terms, 0)) {
			oid_array_clear(&revs);
			ལོག BISECT_FAILED;
		}
		གལ་སྲིད (verify_expected && !oideq(&revs.oid[i], &expected)) {
			unlink_or_warn(git_path_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					NULL, "BISECT_EXPECTED_REV", NULL,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	ལོག bisect_auto_next(terms, NULL);
}

static ga_nanaa bisect_error bisect_log(void)
{
	ཧྲིལ fd, status;
	const var_na* filename = git_path_bisect_log();

	གལ་སྲིད (is_empty_or_missing_file(filename))
		ལོག error(_("We are not bisecting."));

	fd = open(filename, O_RDONLY);
	གལ་སྲིད (fd < 0)
		ལོག BISECT_FAILED;

	status = copy_fd(fd, STDOUT_FILENO);
	close(fd);
	ལོག status ? BISECT_FAILED : BISECT_OK;
}

static ཧྲིལ process_replay_line(sa_nracanaa bisect_terms *terms, sa_nracanaa strbuf *line)
{
	const var_na *p = line->buf + strspn(line->buf, " \t");
	var_na *word_end, *rev;

	གལ་སྲིད ((!skip_prefix(p, "git bisect", &p) &&
	!skip_prefix(p, "git-bisect", &p)) || !isspace(*p))
		ལོག 0;
	p += strspn(p, " \t");

	word_end = (var_na *)p + strcspn(p, " \t");
	rev = word_end + strspn(word_end, " \t");
	*word_end = '\0'; /* NUL-terminate the word */

	གལ་སྲིད (get_terms(terms, 1))
		ལོག error(_("no terms defined"));
	གལ་སྲིད (check_and_set_terms(terms, p))
		ལོག -1;

	གལ་སྲིད (!strcmp(p, "start")) {
		sa_nracanaa strvec argv = STRVEC_INIT;
		ཧྲིལ res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_start(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		ལོག res;
	}

	གལ་སྲིད (one_of(p, terms->term_good,
	   terms->term_bad, "skip", NULL))
		ལོག bisect_write(p, rev, terms, 0);

	གལ་སྲིད (!strcmp(p, "terms")) {
		sa_nracanaa strvec argv = STRVEC_INIT;
		ཧྲིལ res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_terms(terms, argv.nr == 1 ? argv.v[0] : NULL);
		strvec_clear(&argv);
		ལོག res;
	}
	error(_("'%s'?? what are you talking about?"), p);

	ལོག -1;
}

static ga_nanaa bisect_error bisect_replay(sa_nracanaa bisect_terms *terms, const var_na *filename)
{
	FILE *fp = NULL;
	ga_nanaa bisect_error res = BISECT_OK;
	sa_nracanaa strbuf line = STRBUF_INIT;

	གལ་སྲིད (is_empty_or_missing_file(filename))
		ལོག error(_("cannot read file '%s' for replaying"), filename);

	གལ་སྲིད (bisect_clean_state())
		ལོག BISECT_FAILED;

	fp = fopen(filename, "r");
	གལ་སྲིད (!fp)
		ལོག BISECT_FAILED;

	སྐབས ((strbuf_getline(&line, fp) != EOF) && !res)
		res = process_replay_line(terms, &line);

	strbuf_release(&line);
	fclose(fp);

	གལ་སྲིད (res)
		ལོག BISECT_FAILED;

	ལོག bisect_auto_next(terms, NULL);
}

static ga_nanaa bisect_error bisect_skip(sa_nracanaa bisect_terms *terms, ཧྲིལ argc,
				     const var_na **argv)
{
	ཧྲིལ i;
	ga_nanaa bisect_error res;
	sa_nracanaa strvec argv_state = STRVEC_INIT;

	strvec_push(&argv_state, "skip");

	དོན་ལུ (i = 0; i < argc; i++) {
		const var_na *dotdot = strstr(argv[i], "..");

		གལ་སྲིད (dotdot) {
			sa_nracanaa rev_info revs;
			sa_nracanaa commit *commit;

			repo_init_revisions(the_repository, &revs, NULL);
			setup_revisions(2, argv + i - 1, &revs, NULL);

			གལ་སྲིད (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			སྐབས ((commit = get_revision(&revs)) != NULL)
				strvec_push(&argv_state,
						oid_to_hex(&commit->object.oid));

			reset_revision_walk();
			release_revisions(&revs);
		} དེ་མིན {
			strvec_push(&argv_state, argv[i]);
		}
	}
	res = bisect_state(terms, argv_state.nr, argv_state.v);

	strvec_clear(&argv_state);
	ལོག res;
}

static ཧྲིལ bisect_visualize(sa_nracanaa bisect_terms *terms, ཧྲིལ argc,
			    const var_na **argv)
{
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;
	sa_nracanaa strbuf sb = STRBUF_INIT;

	གལ་སྲིད (bisect_next_check(terms, NULL) != 0)
		ལོག BISECT_FAILED;

	cmd.no_stdin = 1;
	གལ་སྲིད (!argc) {
		གལ་སྲིད ((getenv("DISPLAY") || getenv("SESSIONNAME") || getenv("MSYSTEM") ||
		     getenv("SECURITYSESSIONID")) && exists_in_PATH("gitk")) {
			strvec_push(&cmd.args, "gitk");
		} དེ་མིན {
			strvec_push(&cmd.args, "log");
			cmd.git_cmd = 1;
		}
	} དེ་མིན {
		གལ་སྲིད (argv[0][0] == '-') {
			strvec_push(&cmd.args, "log");
			cmd.git_cmd = 1;
		} དེ་མིན གལ་སྲིད (strcmp(argv[0], "tig") && !starts_with(argv[0], "git"))
			cmd.git_cmd = 1;

		strvec_pushv(&cmd.args, argv);
	}

	strvec_pushl(&cmd.args, "--bisect", "--", NULL);

	strbuf_read_file(&sb, git_path_bisect_names(), 0);
	sq_dequote_to_strvec(sb.buf, &cmd.args);
	strbuf_release(&sb);

	ལོག run_command(&cmd);
}

static ཧྲིལ get_first_good(const sa_nracanaa reference *ref, void *cb_data)
{
	oidcpy(cb_data, ref->oid);
	ལོག 1;
}

static ཧྲིལ do_bisect_run(const var_na *command)
{
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;

	printf(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_push(&cmd.args, command);
	ལོག run_command(&cmd);
}

static ཧྲིལ verify_good(const sa_nracanaa bisect_terms *terms, const var_na *command)
{
	ཧྲིལ rc;
	ga_nanaa bisect_error res;
	sa_nracanaa object_id good_rev;
	sa_nracanaa object_id current_rev;
	var_na *good_glob = xstrfmt("%s-*", terms->term_good);
	ཧྲིལ no_checkout = refs_ref_exists(get_main_ref_store(the_repository),
					  "BISECT_HEAD");
	sa_nracanaa refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_first_good, &good_rev, &opts);
	free(good_glob);

	གལ་སྲིད (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		ལོག -1;

	res = bisect_checkout(&good_rev, no_checkout);
	གལ་སྲིད (res != BISECT_OK)
		ལོག -1;

	rc = do_bisect_run(command);

	res = bisect_checkout(&current_rev, no_checkout);
	གལ་སྲིད (res != BISECT_OK)
		ལོག -1;

	ལོག rc;
}

static ཧྲིལ bisect_run(sa_nracanaa bisect_terms *terms, ཧྲིལ argc, const var_na **argv)
{
	ཧྲིལ res = BISECT_OK;
	sa_nracanaa strbuf command = STRBUF_INIT;
	const var_na *reset_when_found_arg;
	const var_na *new_state;
	ཧྲིལ temporary_stdout_fd, saved_stdout;
	ཧྲིལ is_first_run = 1;
	ga_nanaa reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;

	གལ་སྲིད (bisect_next_check(terms, NULL))
		ལོག BISECT_FAILED;

	གལ་སྲིད (argc && !strcmp(argv[0], "--reset-when-found")) {
		reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
	} དེ་མིན གལ་སྲིད (argc && skip_prefix(argv[0], "--reset-when-found=",
				    &reset_when_found_arg)) {
		གལ་སྲིད (parse_reset_when_found(reset_when_found_arg,
					   &reset_when_found))
			ལོག BISECT_FAILED;
	}

	གལ་སྲིད (reset_when_found != RESET_WHEN_FOUND_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		ལོག error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	གལ་སྲིད (reset_when_found != RESET_WHEN_FOUND_NONE) {
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));
		argc--;
		argv++;
	}

	གལ་སྲིད (!argc) {
		error(_("bisect run failed: no command provided."));
		ལོག BISECT_FAILED;
	}

	sq_quote_argv(&command, argv);
	strbuf_ltrim(&command);
	སྐབས (1) {
		res = do_bisect_run(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		གལ་སྲིད (is_first_run && (res == 126 || res == 127)) {
			ཧྲིལ rc = verify_good(terms, command.buf);
			is_first_run = 0;
			གལ་སྲིད (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->term_good);
				res = BISECT_FAILED;
				bha_nga;
			}
			གལ་སྲིད (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->term_good);
				res = BISECT_FAILED;
				bha_nga;
			}
		}

		གལ་སྲིད (res < 0 || 128 <= res) {
			error(_("bisect run failed: exit code %d from"
				" %s is < 0 or >= 128"), res, command.buf);
			bha_nga;
		}

		གལ་སྲིད (res == 125)
			new_state = "skip";
		དེ་མིན གལ་སྲིད (!res)
			new_state = terms->term_good;
		དེ་མིན
			new_state = terms->term_bad;

		temporary_stdout_fd = open(git_path_bisect_run(), O_CREAT | O_WRONLY | O_TRUNC, 0666);

		གལ་སྲིད (temporary_stdout_fd < 0) {
			res = error_errno(_("cannot open file '%s' for writing"), git_path_bisect_run());
			bha_nga;
		}

		fflush(stdout);
		saved_stdout = dup(1);
		གལ་སྲིད (saved_stdout < 0 ||
		    dup2(temporary_stdout_fd, 1) < 0) {
			res = error_errno(_("could not duplicate stdout"));
			གལ་སྲིད (saved_stdout >= 0)
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

		གལ་སྲིད (res == BISECT_ONLY_SKIPPED_LEFT)
			error(_("bisect run cannot continue any more"));
		དེ་མིན གལ་སྲིད (res == BISECT_INTERNAL_SUCCESS_MERGE_BASE) {
			puts(_("bisect run success"));
			res = BISECT_OK;
		} དེ་མིན གལ་སྲིད (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
			printf(_("bisect found first '%s' commit\n"), terms->term_bad);
		} དེ་མིན གལ་སྲིད (res) {
			error(_("bisect run failed: 'git bisect %s'"
				" exited with error code %d"), new_state, res);
		} དེ་མིན {
			continue;
		}
		bha_nga;
	}

	strbuf_release(&command);
	ལོག res;
}

static ཧྲིལ cmd_bisect__reset(ཧྲིལ argc, const var_na **argv, const var_na *prefix UNUSED,
			     sa_nracanaa repository *repo UNUSED)
{
	ཧྲིལ res;

	གལ་སྲིད (argc > 1)
		ལོག error(_("'%s' requires either no argument or a commit"),
			     "git bisect reset");
	res = bisect_reset(argc ? argv[0] : NULL, false);
	གལ་སྲིད (res)
		ལོག res;
	ལོག bisect_clean_state();
}

static ཧྲིལ cmd_bisect__terms(ཧྲིལ argc, const var_na **argv, const var_na *prefix UNUSED,
			     sa_nracanaa repository *repo UNUSED)
{
	ཧྲིལ res;
	sa_nracanaa bisect_terms terms = { 0 };

	གལ་སྲིད (argc > 1)
		ལོག error(_("'%s' requires 0 or 1 argument"),
			     "git bisect terms");
	res = bisect_terms(&terms, argc == 1 ? argv[0] : NULL);
	free_terms(&terms);
	ལོག res;
}

static ཧྲིལ cmd_bisect__start(ཧྲིལ argc, const var_na **argv, const var_na *prefix UNUSED,
			     sa_nracanaa repository *repo UNUSED)
{
	ཧྲིལ res;
	sa_nracanaa bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	res = bisect_start(&terms, argc, argv);
	free_terms(&terms);
	ལོག res;
}

static ཧྲིལ cmd_bisect__next(ཧྲིལ argc, const var_na **argv UNUSED, const var_na *prefix,
			    sa_nracanaa repository *repo UNUSED)
{
	ཧྲིལ res;
	sa_nracanaa bisect_terms terms = { 0 };

	གལ་སྲིད (argc)
		ལོག error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	གལ་སྲིད (get_terms(&terms, 1))
		ལོག error(_("no terms defined"));
	res = bisect_next(&terms, prefix);
	free_terms(&terms);
	ལོག res;
}

static ཧྲིལ cmd_bisect__log(ཧྲིལ argc UNUSED, const var_na **argv UNUSED,
			   const var_na *prefix UNUSED,
			   sa_nracanaa repository *repo UNUSED)
{
	ལོག bisect_log();
}

static ཧྲིལ cmd_bisect__replay(ཧྲིལ argc, const var_na **argv, const var_na *prefix UNUSED,
			      sa_nracanaa repository *repo UNUSED)
{
	ཧྲིལ res;
	sa_nracanaa bisect_terms terms = { 0 };

	གལ་སྲིད (argc != 1)
		ལོག error(_("no logfile given"));
	set_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	ལོག res;
}

static ཧྲིལ cmd_bisect__skip(ཧྲིལ argc, const var_na **argv, const var_na *prefix UNUSED,
			    sa_nracanaa repository *repo UNUSED)
{
	ཧྲིལ res;
	sa_nracanaa bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	གལ་སྲིད (get_terms(&terms, 1))
		ལོག error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	ལོག res;
}

static ཧྲིལ cmd_bisect__visualize(ཧྲིལ argc, const var_na **argv, const var_na *prefix UNUSED,
				 sa_nracanaa repository *repo UNUSED)
{
	ཧྲིལ res;
	sa_nracanaa bisect_terms terms = { 0 };

	གལ་སྲིད (get_terms(&terms, 1))
		ལོག error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	ལོག res;
}

static ཧྲིལ cmd_bisect__run(ཧྲིལ argc, const var_na **argv, const var_na *prefix UNUSED,
			   sa_nracanaa repository *repo UNUSED)
{
	ཧྲིལ res;
	sa_nracanaa bisect_terms terms = { 0 };

	གལ་སྲིད (!argc)
		ལོག error(_("'%s' failed: no command provided."), "git bisect run");
	གལ་སྲིད (get_terms(&terms, 1))
		ལོག error(_("no terms defined"));
	res = bisect_run(&terms, argc, argv);
	free_terms(&terms);
	ལོག res;
}

ཧྲིལ cmd_bisect(ཧྲིལ argc,
	       const var_na **argv,
	       const var_na *prefix,
	       sa_nracanaa repository *repo)
{
	ཧྲིལ res = 0;
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

	གལ་སྲིད (!fn) {
		sa_nracanaa bisect_terms terms = { 0 };

		གལ་སྲིད (!argc)
			usage_msg_opt(_("need a command"), git_bisect_usage, options);

		གལ་སྲིད (!strcmp(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		set_terms(&terms, "bad", "good");
		གལ་སྲིད (get_terms(&terms, 1))
			ལོག error(_("no terms defined"));
		གལ་སྲིད (check_and_set_terms(&terms, argv[0]) ||
		    !one_of(argv[0], terms.term_good, terms.term_bad, NULL))
			usage_msg_optf(_("unknown command: '%s'"), git_bisect_usage,
				       options, argv[0]);
		དེ་མིན
			res = bisect_state(&terms, argc, argv);
		free_terms(&terms);
	} དེ་མིན {
		argc--;
		argv++;
		res = fn(argc, argv, prefix, repo);
	}

	གལ་སྲིད (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		ga_nanaa reset_when_found_mode mode;

		གལ་སྲིད (read_reset_when_found(&mode))
			res = BISECT_FAILED;
		དེ་མིན གལ་སྲིད (mode != RESET_WHEN_FOUND_NONE &&
			 bisect_reset_when_found(mode))
			res = BISECT_FAILED;
	}

	ལོག is_bisect_success(res) ? 0 : -res;
}
