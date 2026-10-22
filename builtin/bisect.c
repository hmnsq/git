#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "copy.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "bisect.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "strvec.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "oid-array.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "prompt.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "revision.h"

સ્થિર GIT_PATH_FUNC(git_path_bisect_terms, "BISECT_TERMS")
સ્થિર GIT_PATH_FUNC(git_path_bisect_ancestors_ok, "BISECT_ANCESTORS_OK")
સ્થિર GIT_PATH_FUNC(git_path_bisect_start, "BISECT_START")
સ્થિર GIT_PATH_FUNC(git_path_bisect_log, "BISECT_LOG")
સ્થિર GIT_PATH_FUNC(git_path_bisect_names, "BISECT_NAMES")
સ્થિર GIT_PATH_FUNC(git_path_bisect_first_parent, "BISECT_FIRST_PARENT")
સ્થિર GIT_PATH_FUNC(git_path_bisect_reset_when_found, "BISECT_RESET_WHEN_FOUND")
સ્થિર GIT_PATH_FUNC(git_path_bisect_run, "BISECT_RUN")

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

સ્થિર અચલ અક્ષર * અચલ git_bisect_usage[] = {
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
	ખાલી
};

ગણના reset_when_found_mode {
	RESET_WHEN_FOUND_NONE,
	RESET_WHEN_FOUND_TO_ORIGINAL,
	RESET_WHEN_FOUND_TO_FOUND,
};

સંરચના add_bisect_ref_data {
	સંરચના rev_info *revs;
	unsigned પૂર્ણાંક object_flags;
};

સંરચના bisect_terms {
	અક્ષર *term_good;
	અક્ષર *term_bad;
};

સ્થિર શૂન્ય free_terms(સંરચના bisect_terms *terms)
{
	FREE_AND_NULL(terms->term_good);
	FREE_AND_NULL(terms->term_bad);
}

સ્થિર શૂન્ય set_terms(સંરચના bisect_terms *terms, અચલ અક્ષર *bad,
		      અચલ અક્ષર *good)
{
	મુક્ત((શૂન્ય *)terms->term_good);
	terms->term_good = xstrdup(good);
	મુક્ત((શૂન્ય *)terms->term_bad);
	terms->term_bad = xstrdup(bad);
}

સ્થિર અચલ અક્ષર vocab_bad[] = "bad|new";
સ્થિર અચલ અક્ષર vocab_good[] = "good|old";

સ્થિર પૂર્ણાંક bisect_autostart(સંરચના bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
LAST_ARG_MUST_BE_NULL
સ્થિર પૂર્ણાંક one_of(અચલ અક્ષર *term, ...)
{
	પૂર્ણાંક res = 0;
	va_list matches;
	અચલ અક્ષર *match;

	va_start(matches, term);
	જ્યારે (!res && (match = va_arg(matches, અચલ અક્ષર *)))
		res = !strcmp(term, match);
	va_end(matches);

	પાછા res;
}

/*
 * return code BISECT_INTERNAL_SUCCESS_MERGE_BASE
 * and BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND are codes
 * that indicate special success.
 */

સ્થિર પૂર્ણાંક is_bisect_success(ગણના bisect_error res)
{
	પાછા !res ||
		res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND ||
		res == BISECT_INTERNAL_SUCCESS_MERGE_BASE;
}

સ્થિર પૂર્ણાંક write_in_file(અચલ અક્ષર *path, અચલ અક્ષર *mode, અચલ અક્ષર *format, va_list args)
{
	FILE *fp = ખાલી;
	પૂર્ણાંક res = 0;

	જો (strcmp(mode, "w") && strcmp(mode, "a"))
		BUG("write-in-file does not support '%s' mode", mode);
	fp = fopen(path, mode);
	જો (!fp)
		પાછા error_errno(_("cannot open file '%s' in mode '%s'"), path, mode);
	res = vfprintf(fp, format, args);

	જો (res < 0) {
		પૂર્ણાંક saved_errno = errno;
		fclose(fp);
		errno = saved_errno;
		પાછા error_errno(_("could not write to file '%s'"), path);
	}

	પાછા fclose(fp);
}

__attribute__((format (printf, 2, 3)))
સ્થિર પૂર્ણાંક write_to_file(અચલ અક્ષર *path, અચલ અક્ષર *format, ...)
{
	પૂર્ણાંક res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "w", format, args);
	va_end(args);

	પાછા res;
}

__attribute__((format (printf, 2, 3)))
સ્થિર પૂર્ણાંક append_to_file(અચલ અક્ષર *path, અચલ અક્ષર *format, ...)
{
	પૂર્ણાંક res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "a", format, args);
	va_end(args);

	પાછા res;
}

સ્થિર પૂર્ણાંક print_file_to_stdout(અચલ અક્ષર *path)
{
	પૂર્ણાંક fd = open(path, O_RDONLY);
	પૂર્ણાંક ret = 0;

	જો (fd < 0)
		પાછા error_errno(_("cannot open file '%s' for reading"), path);
	જો (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), path);
	close(fd);
	પાછા ret;
}

સ્થિર પૂર્ણાંક check_term_format(અચલ અક્ષર *term, અચલ અક્ષર *orig_term)
{
	પૂર્ણાંક res;
	અક્ષર *new_term = xstrfmt("refs/bisect/%s", term);

	res = check_refname_format(new_term, 0);
	મુક્ત(new_term);

	જો (res)
		પાછા error(_("'%s' is not a valid term"), term);

	જો (one_of(term, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", ખાલી))
		પાછા error(_("can't use the builtin command '%s' as a term"), term);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	જો ((strcmp(orig_term, "bad") && one_of(term, "bad", "new", ખાલી)) ||
		 (strcmp(orig_term, "good") && one_of(term, "good", "old", ખાલી)))
		પાછા error(_("can't change the meaning of the term '%s'"), term);

	પાછા 0;
}

સ્થિર પૂર્ણાંક write_terms(અચલ અક્ષર *bad, અચલ અક્ષર *good)
{
	પૂર્ણાંક res;

	જો (!strcmp(bad, good))
		પાછા error(_("please use two different terms"));

	જો (check_term_format(bad, "bad") || check_term_format(good, "good"))
		પાછા -1;

	res = write_to_file(git_path_bisect_terms(), "%s\n%s\n", bad, good);

	પાછા res;
}

સ્થિર પૂર્ણાંક bisect_reset(અચલ અક્ષર *commit, bool quiet)
{
	સંરચના strbuf branch = STRBUF_INIT;

	જો (!commit) {
		જો (!strbuf_read_file(&branch, git_path_bisect_start(), 0))
			printf(_("We are not bisecting.\n"));
		નહીંતર
			strbuf_rtrim(&branch);
	} નહીંતર {
		સંરચના object_id oid;

		જો (repo_get_oid_commit(the_repository, commit, &oid))
			પાછા error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	જો (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		સંરચના child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "checkout", "--ignore-other-worktrees", ખાલી);
		જો (quiet)
			strvec_ધકેલો(&cmd.args, "--quiet");
		strvec_pushl(&cmd.args, branch.buf, "--", ખાલી);
		જો (run_command(&cmd)) {
			error(_("could not check out original"
				" HEAD '%s'. Try 'git bisect"
				" reset <commit>'."), branch.buf);
			strbuf_release(&branch);
			પાછા -1;
		}
	}

	strbuf_release(&branch);
	પાછા 0;
}

સ્થિર પૂર્ણાંક parse_reset_when_found(અચલ અક્ષર *કિંમત,
				  ગણના reset_when_found_mode *mode)
{
	જો (!strcmp(કિંમત, "original"))
		*mode = RESET_WHEN_FOUND_TO_ORIGINAL;
	નહીંતર જો (!strcmp(કિંમત, "found"))
		*mode = RESET_WHEN_FOUND_TO_FOUND;
	નહીંતર
		પાછા error(_("invalid value for '--reset-when-found': '%s'"),
			     કિંમત);

	પાછા 0;
}

સ્થિર અચલ અક્ષર *reset_when_found_mode_name(ગણના reset_when_found_mode mode)
{
	પસંદગી (mode) {
	સ્થિતિ RESET_WHEN_FOUND_TO_ORIGINAL:
		પાછા "original";
	સ્થિતિ RESET_WHEN_FOUND_TO_FOUND:
		પાછા "found";
	સ્થિતિ RESET_WHEN_FOUND_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", mode);
}

સ્થિર પૂર્ણાંક read_reset_when_found(ગણના reset_when_found_mode *mode)
{
	સંરચના strbuf કિંમત = STRBUF_INIT;
	પૂર્ણાંક res = 0;

	*mode = RESET_WHEN_FOUND_NONE;
	જો (is_empty_or_missing_file(git_path_bisect_reset_when_found()))
		પાછા 0;

	જો (strbuf_read_file(&કિંમત, git_path_bisect_reset_when_found(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_path_bisect_reset_when_found());
		જાઓ out;
	}
	strbuf_trim(&કિંમત);
	જો (parse_reset_when_found(કિંમત.buf, mode))
		res = -1;

out:
	strbuf_release(&કિંમત);
	પાછા res;
}

સ્થિર પૂર્ણાંક bisect_reset_when_found(ગણના reset_when_found_mode mode)
{
	સંરચના bisect_terms terms = { 0 };
	અક્ષર *commit = ખાલી;
	પૂર્ણાંક res;

	જો (mode == RESET_WHEN_FOUND_TO_FOUND) {
		read_bisect_terms(&terms.term_bad, &terms.term_good);
		commit = xstrfmt("refs/bisect/%s", terms.term_bad);
	} નહીંતર જો (mode == RESET_WHEN_FOUND_NONE) {
		BUG("automatic reset requested without a reset mode");
	}

	res = bisect_reset(commit, true);
	જો (!res)
		res = bisect_clean_state();

	મુક્ત(commit);
	free_terms(&terms);
	પાછા res;
}

સ્થિર શૂન્ય log_commit(FILE *fp,
		       અચલ અક્ષર *fmt, અચલ અક્ષર *state,
		       સંરચના commit *commit)
{
	સંરચના pretty_print_context pp = {0};
	સંરચના strbuf commit_સંદેશ = STRBUF_INIT;
	અક્ષર *label = xstrfmt(fmt, state);

	repo_format_commit_message(the_repository, commit, "%s", &commit_સંદેશ,
				   &pp);

	fprintf(fp, "# %s: [%s] %s\n", label, oid_to_hex(&commit->object.oid),
		commit_સંદેશ.buf);

	strbuf_release(&commit_સંદેશ);
	મુક્ત(label);
}

સ્થિર પૂર્ણાંક bisect_write(અચલ અક્ષર *state, અચલ અક્ષર *rev,
			અચલ સંરચના bisect_terms *terms, પૂર્ણાંક nolog)
{
	સંરચના strbuf tag = STRBUF_INIT;
	સંરચના object_id oid;
	સંરચના commit *commit;
	FILE *fp = ખાલી;
	પૂર્ણાંક res = 0;

	જો (!strcmp(state, terms->term_bad)) {
		strbuf_addf(&tag, "refs/bisect/%s", state);
	} નહીંતર જો (one_of(state, terms->term_good, "skip", ખાલી)) {
		strbuf_addf(&tag, "refs/bisect/%s-%s", state, rev);
	} નહીંતર {
		res = error(_("Bad bisect_write argument: %s"), state);
		જાઓ finish;
	}

	જો (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		જાઓ finish;
	}

	જો (refs_update_ref(get_main_ref_store(the_repository), ખાલી, tag.buf, &oid, ખાલી, 0,
			    UPDATE_REFS_સંદેશ_ON_ERR)) {
		res = -1;
		જાઓ finish;
	}

	fp = fopen(git_path_bisect_log(), "a");
	જો (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_path_bisect_log());
		જાઓ finish;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	જો (!nolog)
		fprintf(fp, "git bisect %s %s\n", state, rev);

finish:
	જો (fp)
		fclose(fp);
	strbuf_release(&tag);
	પાછા res;
}

સ્થિર પૂર્ણાંક check_and_set_terms(સંરચના bisect_terms *terms, અચલ અક્ષર *cmd)
{
	પૂર્ણાંક has_term_file = !is_empty_or_missing_file(git_path_bisect_terms());

	જો (one_of(cmd, "skip", "start", "terms", ખાલી))
		પાછા 0;

	જો (has_term_file && strcmp(cmd, terms->term_bad) &&
	    strcmp(cmd, terms->term_good))
		પાછા error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->term_bad,
				terms->term_good);

	જો (!has_term_file) {
		જો (one_of(cmd, "bad", "good", ખાલી)) {
			set_terms(terms, "bad", "good");
			પાછા write_terms(terms->term_bad, terms->term_good);
		}
		જો (one_of(cmd, "new", "old", ખાલી)) {
			set_terms(terms, "new", "old");
			પાછા write_terms(terms->term_bad, terms->term_good);
		}
	}

	પાછા 0;
}

સ્થિર પૂર્ણાંક inc_nr(અચલ સંરચના reference *ref UNUSED, શૂન્ય *cb_data)
{
	unsigned પૂર્ણાંક *nr = (unsigned પૂર્ણાંક *)cb_data;
	(*nr)++;
	પાછા 0;
}

સ્થિર અચલ અક્ષર need_bad_and_good_revision_warning[] =
	N_("You need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

સ્થિર અચલ અક્ષર need_bisect_start_warning[] =
	N_("You need to start by \"git bisect start\".\n"
	   "You then need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

સ્થિર પૂર્ણાંક decide_next(અચલ સંરચના bisect_terms *terms,
		       અચલ અક્ષર *current_term, પૂર્ણાંક missing_good,
		       પૂર્ણાંક missing_bad)
{
	જો (!missing_good && !missing_bad)
		પાછા 0;
	જો (!current_term)
		પાછા -1;

	જો (missing_good && !missing_bad &&
	    !strcmp(current_term, terms->term_good)) {
		અક્ષર *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->term_bad);
		જો (!isatty(0))
			પાછા 0;
		/*
		 * TRANSLATORS: Make sure to include [Y] and [n] in your
		 * translation. The program will only accept English input
		 * at this point.
		 */
		yesno = git_prompt(_("Are you sure [Y/n]? "), PROMPT_ECHO);
		જો (starts_with(yesno, "N") || starts_with(yesno, "n"))
			પાછા -1;
		પાછા 0;
	}

	જો (!is_empty_or_missing_file(git_path_bisect_start()))
		પાછા error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	નહીંતર
		પાછા error(_(need_bisect_start_warning),
			     vocab_good, vocab_bad, vocab_good, vocab_bad);
}

સ્થિર શૂન્ય bisect_status(સંરચના bisect_state *state,
			  અચલ સંરચના bisect_terms *terms)
{
	અક્ષર *bad_ref = xstrfmt("refs/bisect/%s", terms->term_bad);
	અક્ષર *good_glob = xstrfmt("%s-*", terms->term_good);
	સંરચના refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	જો (refs_ref_exists(get_main_ref_store(the_repository), bad_ref))
		state->nr_bad = 1;

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      inc_nr, &state->nr_good, &opts);

	મુક્ત(good_glob);
	મુક્ત(bad_ref);
}

__attribute__((format (printf, 1, 2)))
સ્થિર શૂન્ય bisect_log_printf(અચલ અક્ષર *fmt, ...)
{
	સંરચના strbuf buf = STRBUF_INIT;
	va_list ap;

	va_start(ap, fmt);
	strbuf_vaddf(&buf, fmt, ap);
	va_end(ap);

	printf("%s", buf.buf);
	append_to_file(git_path_bisect_log(), "# %s", buf.buf);

	strbuf_release(&buf);
}

સ્થિર શૂન્ય bisect_print_status(અચલ સંરચના bisect_terms *terms)
{
	સંરચના bisect_state state = { 0 };

	bisect_status(&state, terms);

	/* If we had both, we'd already be started, and shouldn't get here. */
	જો (state.nr_good && state.nr_bad)
		પાછા;

	જો (!state.nr_good && !state.nr_bad)
		bisect_log_printf(_("status: waiting for both '%s' and '%s' commits\n"),
				  terms->term_good, terms->term_bad);
	નહીંતર જો (state.nr_good)
		bisect_log_printf(Q_("status: waiting for '%s' commit, %d '%s' commit known\n",
				     "status: waiting for '%s' commit, %d '%s' commits known\n",
				     state.nr_good),
				  terms->term_bad, state.nr_good, terms->term_good);
	નહીંતર
		bisect_log_printf(_("status: waiting for '%s' commit(s), '%s' commit known\n"),
				  terms->term_good, terms->term_bad);
}

સ્થિર પૂર્ણાંક bisect_next_check(અચલ સંરચના bisect_terms *terms,
			     અચલ અક્ષર *current_term)
{
	સંરચના bisect_state state = { 0 };
	bisect_status(&state, terms);
	પાછા decide_next(terms, current_term, !state.nr_good, !state.nr_bad);
}

સ્થિર પૂર્ણાંક get_terms(સંરચના bisect_terms *terms, પૂર્ણાંક file_missing_is_ok)
{
	સંરચના strbuf str = STRBUF_INIT;
	FILE *fp = ખાલી;
	પૂર્ણાંક res = 0;

	fp = fopen(git_path_bisect_terms(), "r");
	જો (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		જાઓ finish;
	}

	free_terms(terms);
	જો (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		જાઓ finish;
	}
	terms->term_bad = strbuf_detach(&str, ખાલી);
	જો (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		FREE_AND_NULL(terms->term_bad);
		જાઓ finish;
	}
	terms->term_good = strbuf_detach(&str, ખાલી);

finish:
	જો (fp)
		fclose(fp);
	strbuf_release(&str);
	પાછા res;
}

સ્થિર પૂર્ણાંક bisect_terms(સંરચના bisect_terms *terms, અચલ અક્ષર *option)
{
	જો (get_terms(terms, 0))
		પાછા error(_("no terms defined"));

	જો (!option) {
		printf(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->term_good, terms->term_bad);
		પાછા 0;
	}
	જો (one_of(option, "--term-good", "--term-old", ખાલી))
		printf("%s\n", terms->term_good);
	નહીંતર જો (one_of(option, "--term-bad", "--term-new", ખાલી))
		printf("%s\n", terms->term_bad);
	નહીંતર
		પાછા error(_("invalid argument %s for 'git bisect terms'.\n"
			       "Supported options are: "
			       "--term-good|--term-old and "
			       "--term-bad|--term-new."), option);

	પાછા 0;
}

સ્થિર પૂર્ણાંક bisect_append_log_quoted(અચલ અક્ષર **argv)
{
	પૂર્ણાંક res = 0;
	FILE *fp = fopen(git_path_bisect_log(), "a");
	સંરચના strbuf orig_args = STRBUF_INIT;

	જો (!fp)
		પાછા -1;

	જો (fprintf(fp, "git bisect start") < 1) {
		res = -1;
		જાઓ finish;
	}

	sq_quote_argv(&orig_args, argv);
	જો (fprintf(fp, "%s\n", orig_args.buf) < 1)
		res = -1;

finish:
	fclose(fp);
	strbuf_release(&orig_args);
	પાછા res;
}

સ્થિર પૂર્ણાંક add_bisect_ref(અચલ સંરચના reference *ref, શૂન્ય *cb)
{
	સંરચના add_bisect_ref_data *data = cb;

	add_pending_oid(data->revs, ref->name, ref->oid, data->object_flags);

	પાછા 0;
}

સ્થિર પૂર્ણાંક prepare_revs(સંરચના bisect_terms *terms, સંરચના rev_info *revs)
{
	સંરચના refs_for_each_ref_options opts = {
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};
	પૂર્ણાંક res = 0;
	સંરચના add_bisect_ref_data cb = { revs };
	અક્ષર *good = xstrfmt("%s-*", terms->term_good);

	/*
	 * We cannot use terms->term_bad directly in
	 * for_each_glob_ref_in() and we have to append a '*' to it,
	 * otherwise for_each_glob_ref_in() will append '/' and '*'.
	 */
	અક્ષર *bad = xstrfmt("%s*", terms->term_bad);

	/*
	 * It is important to reset the flags used by revision walks
	 * as the previous call to bisect_next_all() in turn
	 * sets up a revision walk.
	 */
	reset_revision_walk();
	repo_init_revisions(the_repository, revs, ખાલી);
	setup_revisions(0, ખાલી, revs, ખાલી);

	opts.pattern = bad;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      add_bisect_ref, &cb, &opts);

	cb.object_flags = UNINTERESTING;
	opts.pattern = good;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      add_bisect_ref, &cb, &opts);

	જો (prepare_revision_walk(revs))
		res = error(_("revision walk setup failed"));

	મુક્ત(good);
	મુક્ત(bad);
	પાછા res;
}

સ્થિર પૂર્ણાંક bisect_skipped_commits(સંરચના bisect_terms *terms)
{
	પૂર્ણાંક res;
	FILE *fp = ખાલી;
	સંરચના rev_info revs;
	સંરચના commit *commit;
	સંરચના pretty_print_context pp = {0};
	સંરચના strbuf commit_name = STRBUF_INIT;

	res = prepare_revs(terms, &revs);
	જો (res)
		પાછા res;

	fp = fopen(git_path_bisect_log(), "a");
	જો (!fp)
		પાછા error_errno(_("could not open '%s' for appending"),
				  git_path_bisect_log());

	જો (fprintf(fp, "# only skipped commits left to test\n") < 0)
		પાછા error_errno(_("failed to write to '%s'"), git_path_bisect_log());

	જ્યારે ((commit = get_revision(&revs)) != ખાલી) {
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
	પાછા 0;
}

સ્થિર પૂર્ણાંક bisect_successful(સંરચના bisect_terms *terms)
{
	સંરચના object_id oid;
	સંરચના commit *commit;
	સંરચના pretty_print_context pp = {0};
	સંરચના strbuf commit_name = STRBUF_INIT;
	અક્ષર *bad_ref = xstrfmt("refs/bisect/%s",terms->term_bad);
	પૂર્ણાંક res;

	refs_read_ref(get_main_ref_store(the_repository), bad_ref, &oid);
	commit = lookup_commit_reference_by_name(bad_ref);
	જો (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		મુક્ત(bad_ref);
		પાછા BISECT_FAILED;
	}
	repo_format_commit_message(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_to_file(git_path_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->term_bad, oid_to_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	મુક્ત(bad_ref);
	પાછા res;
}

સ્થિર ગણના bisect_error bisect_next(સંરચના bisect_terms *terms,
				     અચલ અક્ષર *prefix)
{
	ગણના bisect_error res;

	જો (bisect_autostart(terms))
		પાછા BISECT_FAILED;

	જો (bisect_next_check(terms, terms->term_good))
		પાછા BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_next_all(the_repository, prefix);

	જો (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		res = bisect_successful(terms);
		પાછા res ? res : BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND;
	} નહીંતર જો (res == BISECT_ONLY_SKIPPED_ડાબું) {
		res = bisect_skipped_commits(terms);
		પાછા res ? res : BISECT_ONLY_SKIPPED_ડાબું;
	}
	પાછા res;
}

સ્થિર ગણના bisect_error bisect_auto_next(સંરચના bisect_terms *terms,
					  અચલ અક્ષર *prefix)
{
	જો (bisect_next_check(terms, ખાલી)) {
		bisect_print_status(terms);
		પાછા BISECT_OK;
	}

	પાછા bisect_next(terms, prefix);
}

સ્થિર ગણના bisect_error bisect_start(સંરચના bisect_terms *terms, પૂર્ણાંક argc,
				      અચલ અક્ષર **argv)
{
	પૂર્ણાંક no_checkout = 0;
	પૂર્ણાંક first_parent_only = 0;
	પૂર્ણાંક i, has_double_dash = 0, must_write_terms = 0, bad_seen = 0;
	પૂર્ણાંક flags, pathspec_pos;
	ગણના bisect_error res = BISECT_OK;
	સંરચના string_list revs = STRING_LIST_INIT_DUP;
	સંરચના string_list states = STRING_LIST_INIT_DUP;
	સંરચના strbuf start_head = STRBUF_INIT;
	સંરચના strbuf bisect_names = STRBUF_INIT;
	સંરચના object_id head_oid;
	સંરચના object_id oid;
	ગણના reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;
	અચલ અક્ષર *head;

	જો (is_bare_repository(the_repository))
		no_checkout = 1;

	/*
	 * Check for one bad and then some good revisions
	 */
	માટે (i = 0; i < argc; i++) {
		જો (!strcmp(argv[i], "--")) {
			has_double_dash = 1;
			તોડો;
		}
	}

	માટે (i = 0; i < argc; i++) {
		અચલ અક્ષર *arg = argv[i];
		જો (!strcmp(argv[i], "--")) {
			તોડો;
		} નહીંતર જો (!strcmp(arg, "--no-checkout")) {
			no_checkout = 1;
		} નહીંતર જો (!strcmp(arg, "--first-parent")) {
			first_parent_only = 1;
		} નહીંતર જો (!strcmp(arg, "--reset-when-found")) {
			reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
		} નહીંતર જો (skip_prefix(arg, "--reset-when-found=", &arg)) {
			જો (parse_reset_when_found(arg, &reset_when_found)) {
				res = BISECT_FAILED;
				જાઓ finish;
			}
		} નહીંતર જો (!strcmp(arg, "--term-good") ||
			 !strcmp(arg, "--term-old")) {
			i++;
			જો (argc <= i)
				પાછા error(_("'' is not a valid term"));
			must_write_terms = 1;
			મુક્ત((શૂન્ય *) terms->term_good);
			terms->term_good = xstrdup(argv[i]);
		} નહીંતર જો (skip_prefix(arg, "--term-good=", &arg) ||
			   skip_prefix(arg, "--term-old=", &arg)) {
			must_write_terms = 1;
			મુક્ત((શૂન્ય *) terms->term_good);
			terms->term_good = xstrdup(arg);
		} નહીંતર જો (!strcmp(arg, "--term-bad") ||
			 !strcmp(arg, "--term-new")) {
			i++;
			જો (argc <= i)
				પાછા error(_("'' is not a valid term"));
			must_write_terms = 1;
			મુક્ત((શૂન્ય *) terms->term_bad);
			terms->term_bad = xstrdup(argv[i]);
		} નહીંતર જો (skip_prefix(arg, "--term-bad=", &arg) ||
			   skip_prefix(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			મુક્ત((શૂન્ય *) terms->term_bad);
			terms->term_bad = xstrdup(arg);
		} નહીંતર જો (starts_with(arg, "--")) {
			પાછા error(_("unrecognized option: '%s'"), arg);
		} નહીંતર જો (!get_oidf(&oid, "%s^{commit}", arg)) {
			string_list_append(&revs, oid_to_hex(&oid));
		} નહીંતર જો (has_double_dash) {
			die(_("'%s' does not appear to be a valid "
			      "revision"), arg);
		} નહીંતર {
			તોડો;
		}
	}
	જો (reset_when_found != RESET_WHEN_FOUND_NONE && no_checkout) {
		res = error(_("options '%s' and '%s' cannot be used together"),
			    "--reset-when-found", "--no-checkout");
		જાઓ finish;
	}
	pathspec_pos = i;

	/*
	 * The user ran "git bisect start <sha1> <sha1>", hence did not
	 * explicitly specify the terms, but we are already starting to
	 * set references named with the default terms, and won't be able
	 * to change afterwards.
	 */
	જો (revs.nr)
		must_write_terms = 1;
	માટે (i = 0; i < revs.nr; i++) {
		જો (bad_seen) {
			string_list_append(&states, terms->term_good);
		} નહીંતર {
			bad_seen = 1;
			string_list_append(&states, terms->term_bad);
		}
	}

	/*
	 * Verify HEAD
	 */
	head = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
				       "HEAD", 0, &head_oid, &flags);
	જો (!head) {
		જો (repo_get_oid(the_repository, "HEAD", &head_oid))
			પાછા error(_("bad HEAD - I need a HEAD"));
		head = "HEAD";
	}

	/*
	 * Check if we are bisecting
	 */
	જો (!is_empty_or_missing_file(git_path_bisect_start())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&start_head, git_path_bisect_start(), 0);
		strbuf_trim(&start_head);
		જો (!no_checkout) {
			સંરચના child_process cmd = CHILD_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.args, "checkout", start_head.buf,
				     "--", ખાલી);
			જો (run_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       start_head.buf);
				જાઓ finish;
			}
		}
	} નહીંતર {
		/* Get the rev from where we start. */
		જો (!repo_get_oid(the_repository, head, &head_oid) &&
		    !starts_with(head, "refs/heads/")) {
			strbuf_reset(&start_head);
			strbuf_add_oid_hex(&start_head, &head_oid);
		} નહીંતર જો (!repo_get_oid(the_repository, head, &head_oid) &&
			   skip_prefix(head, "refs/heads/", &head)) {
			strbuf_addstr(&start_head, head);
		} નહીંતર {
			પાછા error(_("bad HEAD - strange symbolic ref"));
		}
	}

	/*
	 * Get rid of any old bisect state.
	 */
	જો (bisect_clean_state())
		પાછા BISECT_FAILED;

	/*
	 * Write new start state
	 */
	write_file(git_path_bisect_start(), "%s\n", start_head.buf);

	જો (first_parent_only)
		write_file(git_path_bisect_first_parent(), "\n");

	જો (reset_when_found != RESET_WHEN_FOUND_NONE)
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));

	જો (no_checkout) {
		જો (repo_get_oid(the_repository, start_head.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), start_head.buf);
			જાઓ finish;
		}
		જો (refs_update_ref(get_main_ref_store(the_repository), ખાલી, "BISECT_HEAD", &oid, ખાલી, 0,
				    UPDATE_REFS_સંદેશ_ON_ERR)) {
			res = BISECT_FAILED;
			જાઓ finish;
		}
	}

	જો (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_names, argv + pathspec_pos);
	write_file(git_path_bisect_names(), "%s\n", bisect_names.buf);

	માટે (i = 0; i < states.nr; i++)
		જો (bisect_write(states.વસ્તુઓ[i].string,
				 revs.વસ્તુઓ[i].string, terms, 1)) {
			res = BISECT_FAILED;
			જાઓ finish;
		}

	જો (must_write_terms && write_terms(terms->term_bad,
					    terms->term_good)) {
		res = BISECT_FAILED;
		જાઓ finish;
	}

	res = bisect_append_log_quoted(argv);
	જો (res)
		res = BISECT_FAILED;

finish:
	string_list_clear(&revs, 0);
	string_list_clear(&states, 0);
	strbuf_release(&start_head);
	strbuf_release(&bisect_names);
	જો (res)
		પાછા res;

	res = bisect_auto_next(terms, ખાલી);
	જો (!is_bisect_success(res))
		bisect_clean_state();
	પાછા res;
}

સ્થિર અંતઃરેખ પૂર્ણાંક file_is_not_empty(અચલ અક્ષર *path)
{
	પાછા !is_empty_or_missing_file(path);
}

સ્થિર પૂર્ણાંક bisect_autostart(સંરચના bisect_terms *terms)
{
	પૂર્ણાંક res;
	અચલ અક્ષર *yesno;

	જો (file_is_not_empty(git_path_bisect_start()))
		પાછા 0;

	fprintf_ln(stderr, _("You need to start by \"git bisect "
			  "start\"\n"));

	જો (!isatty(STDIN_FILENO))
		પાછા -1;

	/*
	 * TRANSLATORS: Make sure to include [Y] and [n] in your
	 * translation. The program will only accept English input
	 * at this point.
	 */
	yesno = git_prompt(_("Do you want me to do it for you "
			     "[Y/n]? "), PROMPT_ECHO);
	res = tolower(*yesno) == 'n' ?
		-1 : bisect_start(terms, 0, empty_strvec);

	પાછા res;
}

સ્થિર ગણના bisect_error bisect_state(સંરચના bisect_terms *terms, પૂર્ણાંક argc,
				      અચલ અક્ષર **argv)
{
	અચલ અક્ષર *state;
	પૂર્ણાંક i, verify_expected = 1;
	સંરચના object_id oid, expected;
	સંરચના oid_array revs = OID_ARRAY_INIT;

	જો (!argc)
		પાછા error(_("Please call `--bisect-state` with at least one argument"));

	જો (bisect_autostart(terms))
		પાછા BISECT_FAILED;

	state = argv[0];
	જો (check_and_set_terms(terms, state) ||
	    !one_of(state, terms->term_good, terms->term_bad, "skip", ખાલી))
		પાછા BISECT_FAILED;

	argv++;
	argc--;
	જો (argc > 1 && !strcmp(state, terms->term_bad))
		પાછા error(_("'git bisect %s' can take only one argument."), terms->term_bad);

	જો (argc == 0) {
		અચલ અક્ષર *head = "BISECT_HEAD";
		ગણના get_oid_result res_head = repo_get_oid(the_repository,
							    head, &oid);

		જો (res_head == MISSING_OBJECT) {
			head = "HEAD";
			res_head = repo_get_oid(the_repository, head, &oid);
		}

		જો (res_head)
			error(_("Bad rev input: %s"), head);
		oid_array_append(&revs, &oid);
	}

	/*
	 * All input revs must be checked before executing bisect_write()
	 * to discard junk revs.
	 */

	માટે (; argc; argc--, argv++) {
		સંરચના commit *commit;

		જો (repo_get_oid(the_repository, *argv, &oid)){
			error(_("Bad rev input: %s"), *argv);
			oid_array_clear(&revs);
			પાછા BISECT_FAILED;
		}

		commit = lookup_commit_reference(the_repository, &oid);
		જો (!commit)
			die(_("Bad rev input (not a commit): %s"), *argv);

		oid_array_append(&revs, &commit->object.oid);
	}

	જો (refs_read_ref(get_main_ref_store(the_repository), "BISECT_EXPECTED_REV", &expected))
		verify_expected = 0; /* Ignore invalid file contents */

	માટે (i = 0; i < revs.nr; i++) {
		જો (bisect_write(state, oid_to_hex(&revs.oid[i]), terms, 0)) {
			oid_array_clear(&revs);
			પાછા BISECT_FAILED;
		}
		જો (verify_expected && !oideq(&revs.oid[i], &expected)) {
			unlink_or_warn(git_path_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					ખાલી, "BISECT_EXPECTED_REV", ખાલી,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	પાછા bisect_auto_next(terms, ખાલી);
}

સ્થિર ગણના bisect_error bisect_log(શૂન્ય)
{
	પૂર્ણાંક fd, status;
	અચલ અક્ષર* filename = git_path_bisect_log();

	જો (is_empty_or_missing_file(filename))
		પાછા error(_("We are not bisecting."));

	fd = open(filename, O_RDONLY);
	જો (fd < 0)
		પાછા BISECT_FAILED;

	status = copy_fd(fd, STDOUT_FILENO);
	close(fd);
	પાછા status ? BISECT_FAILED : BISECT_OK;
}

સ્થિર પૂર્ણાંક process_replay_લીટી(સંરચના bisect_terms *terms, સંરચના strbuf *લીટી)
{
	અચલ અક્ષર *p = લીટી->buf + strspn(લીટી->buf, " \t");
	અક્ષર *word_end, *rev;

	જો ((!skip_prefix(p, "git bisect", &p) &&
	!skip_prefix(p, "git-bisect", &p)) || !isspace(*p))
		પાછા 0;
	p += strspn(p, " \t");

	word_end = (અક્ષર *)p + strcspn(p, " \t");
	rev = word_end + strspn(word_end, " \t");
	*word_end = '\0'; /* NUL-terminate the word */

	જો (get_terms(terms, 1))
		પાછા error(_("no terms defined"));
	જો (check_and_set_terms(terms, p))
		પાછા -1;

	જો (!strcmp(p, "start")) {
		સંરચના strvec argv = STRVEC_INIT;
		પૂર્ણાંક res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_start(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		પાછા res;
	}

	જો (one_of(p, terms->term_good,
	   terms->term_bad, "skip", ખાલી))
		પાછા bisect_write(p, rev, terms, 0);

	જો (!strcmp(p, "terms")) {
		સંરચના strvec argv = STRVEC_INIT;
		પૂર્ણાંક res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_terms(terms, argv.nr == 1 ? argv.v[0] : ખાલી);
		strvec_clear(&argv);
		પાછા res;
	}
	error(_("'%s'?? what are you talking about?"), p);

	પાછા -1;
}

સ્થિર ગણના bisect_error bisect_replay(સંરચના bisect_terms *terms, અચલ અક્ષર *filename)
{
	FILE *fp = ખાલી;
	ગણના bisect_error res = BISECT_OK;
	સંરચના strbuf લીટી = STRBUF_INIT;

	જો (is_empty_or_missing_file(filename))
		પાછા error(_("cannot read file '%s' for replaying"), filename);

	જો (bisect_clean_state())
		પાછા BISECT_FAILED;

	fp = fopen(filename, "r");
	જો (!fp)
		પાછા BISECT_FAILED;

	જ્યારે ((strbuf_getline(&લીટી, fp) != EOF) && !res)
		res = process_replay_લીટી(terms, &લીટી);

	strbuf_release(&લીટી);
	fclose(fp);

	જો (res)
		પાછા BISECT_FAILED;

	પાછા bisect_auto_next(terms, ખાલી);
}

સ્થિર ગણના bisect_error bisect_skip(સંરચના bisect_terms *terms, પૂર્ણાંક argc,
				     અચલ અક્ષર **argv)
{
	પૂર્ણાંક i;
	ગણના bisect_error res;
	સંરચના strvec argv_state = STRVEC_INIT;

	strvec_ધકેલો(&argv_state, "skip");

	માટે (i = 0; i < argc; i++) {
		અચલ અક્ષર *dotdot = strstr(argv[i], "..");

		જો (dotdot) {
			સંરચના rev_info revs;
			સંરચના commit *commit;

			repo_init_revisions(the_repository, &revs, ખાલી);
			setup_revisions(2, argv + i - 1, &revs, ખાલી);

			જો (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			જ્યારે ((commit = get_revision(&revs)) != ખાલી)
				strvec_ધકેલો(&argv_state,
						oid_to_hex(&commit->object.oid));

			reset_revision_walk();
			release_revisions(&revs);
		} નહીંતર {
			strvec_ધકેલો(&argv_state, argv[i]);
		}
	}
	res = bisect_state(terms, argv_state.nr, argv_state.v);

	strvec_clear(&argv_state);
	પાછા res;
}

સ્થિર પૂર્ણાંક bisect_visualize(સંરચના bisect_terms *terms, પૂર્ણાંક argc,
			    અચલ અક્ષર **argv)
{
	સંરચના child_process cmd = CHILD_PROCESS_INIT;
	સંરચના strbuf sb = STRBUF_INIT;

	જો (bisect_next_check(terms, ખાલી) != 0)
		પાછા BISECT_FAILED;

	cmd.no_stdin = 1;
	જો (!argc) {
		જો ((getenv("DISPLAY") || getenv("SESSIONNAME") || getenv("MSYSTEM") ||
		     getenv("SECURITYSESSIONID")) && exists_in_PATH("gitk")) {
			strvec_ધકેલો(&cmd.args, "gitk");
		} નહીંતર {
			strvec_ધકેલો(&cmd.args, "log");
			cmd.git_cmd = 1;
		}
	} નહીંતર {
		જો (argv[0][0] == '-') {
			strvec_ધકેલો(&cmd.args, "log");
			cmd.git_cmd = 1;
		} નહીંતર જો (strcmp(argv[0], "tig") && !starts_with(argv[0], "git"))
			cmd.git_cmd = 1;

		strvec_pushv(&cmd.args, argv);
	}

	strvec_pushl(&cmd.args, "--bisect", "--", ખાલી);

	strbuf_read_file(&sb, git_path_bisect_names(), 0);
	sq_dequote_to_strvec(sb.buf, &cmd.args);
	strbuf_release(&sb);

	પાછા run_command(&cmd);
}

સ્થિર પૂર્ણાંક get_first_good(અચલ સંરચના reference *ref, શૂન્ય *cb_data)
{
	oidcpy(cb_data, ref->oid);
	પાછા 1;
}

સ્થિર પૂર્ણાંક do_bisect_run(અચલ અક્ષર *command)
{
	સંરચના child_process cmd = CHILD_PROCESS_INIT;

	printf(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_ધકેલો(&cmd.args, command);
	પાછા run_command(&cmd);
}

સ્થિર પૂર્ણાંક verify_good(અચલ સંરચના bisect_terms *terms, અચલ અક્ષર *command)
{
	પૂર્ણાંક rc;
	ગણના bisect_error res;
	સંરચના object_id good_rev;
	સંરચના object_id current_rev;
	અક્ષર *good_glob = xstrfmt("%s-*", terms->term_good);
	પૂર્ણાંક no_checkout = refs_ref_exists(get_main_ref_store(the_repository),
					  "BISECT_HEAD");
	સંરચના refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_first_good, &good_rev, &opts);
	મુક્ત(good_glob);

	જો (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		પાછા -1;

	res = bisect_checkout(&good_rev, no_checkout);
	જો (res != BISECT_OK)
		પાછા -1;

	rc = do_bisect_run(command);

	res = bisect_checkout(&current_rev, no_checkout);
	જો (res != BISECT_OK)
		પાછા -1;

	પાછા rc;
}

સ્થિર પૂર્ણાંક bisect_run(સંરચના bisect_terms *terms, પૂર્ણાંક argc, અચલ અક્ષર **argv)
{
	પૂર્ણાંક res = BISECT_OK;
	સંરચના strbuf command = STRBUF_INIT;
	અચલ અક્ષર *reset_when_found_arg;
	અચલ અક્ષર *new_state;
	પૂર્ણાંક temporary_stdout_fd, saved_stdout;
	પૂર્ણાંક is_first_run = 1;
	ગણના reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;

	જો (bisect_next_check(terms, ખાલી))
		પાછા BISECT_FAILED;

	જો (argc && !strcmp(argv[0], "--reset-when-found")) {
		reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
	} નહીંતર જો (argc && skip_prefix(argv[0], "--reset-when-found=",
				    &reset_when_found_arg)) {
		જો (parse_reset_when_found(reset_when_found_arg,
					   &reset_when_found))
			પાછા BISECT_FAILED;
	}

	જો (reset_when_found != RESET_WHEN_FOUND_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		પાછા error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	જો (reset_when_found != RESET_WHEN_FOUND_NONE) {
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));
		argc--;
		argv++;
	}

	જો (!argc) {
		error(_("bisect run failed: no command provided."));
		પાછા BISECT_FAILED;
	}

	sq_quote_argv(&command, argv);
	strbuf_ltrim(&command);
	જ્યારે (1) {
		res = do_bisect_run(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		જો (is_first_run && (res == 126 || res == 127)) {
			પૂર્ણાંક rc = verify_good(terms, command.buf);
			is_first_run = 0;
			જો (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->term_good);
				res = BISECT_FAILED;
				તોડો;
			}
			જો (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->term_good);
				res = BISECT_FAILED;
				તોડો;
			}
		}

		જો (res < 0 || 128 <= res) {
			error(_("bisect run failed: exit code %d from"
				" %s is < 0 or >= 128"), res, command.buf);
			તોડો;
		}

		જો (res == 125)
			new_state = "skip";
		નહીંતર જો (!res)
			new_state = terms->term_good;
		નહીંતર
			new_state = terms->term_bad;

		temporary_stdout_fd = open(git_path_bisect_run(), O_CREAT | O_WRONLY | O_TRUNC, 0666);

		જો (temporary_stdout_fd < 0) {
			res = error_errno(_("cannot open file '%s' for writing"), git_path_bisect_run());
			તોડો;
		}

		fflush(stdout);
		saved_stdout = dup(1);
		જો (saved_stdout < 0 ||
		    dup2(temporary_stdout_fd, 1) < 0) {
			res = error_errno(_("could not duplicate stdout"));
			જો (saved_stdout >= 0)
				close(saved_stdout);
			close(temporary_stdout_fd);
			તોડો;
		}

		res = bisect_state(terms, 1, &new_state);

		fflush(stdout);
		dup2(saved_stdout, 1);
		close(saved_stdout);
		close(temporary_stdout_fd);

		print_file_to_stdout(git_path_bisect_run());

		જો (res == BISECT_ONLY_SKIPPED_ડાબું)
			error(_("bisect run cannot continue any more"));
		નહીંતર જો (res == BISECT_INTERNAL_SUCCESS_MERGE_BASE) {
			puts(_("bisect run success"));
			res = BISECT_OK;
		} નહીંતર જો (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
			printf(_("bisect found first '%s' commit\n"), terms->term_bad);
		} નહીંતર જો (res) {
			error(_("bisect run failed: 'git bisect %s'"
				" exited with error code %d"), new_state, res);
		} નહીંતર {
			ચાલુ;
		}
		તોડો;
	}

	strbuf_release(&command);
	પાછા res;
}

સ્થિર પૂર્ણાંક cmd_bisect__reset(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix UNUSED,
			     સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક res;

	જો (argc > 1)
		પાછા error(_("'%s' requires either no argument or a commit"),
			     "git bisect reset");
	res = bisect_reset(argc ? argv[0] : ખાલી, false);
	જો (res)
		પાછા res;
	પાછા bisect_clean_state();
}

સ્થિર પૂર્ણાંક cmd_bisect__terms(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix UNUSED,
			     સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક res;
	સંરચના bisect_terms terms = { 0 };

	જો (argc > 1)
		પાછા error(_("'%s' requires 0 or 1 argument"),
			     "git bisect terms");
	res = bisect_terms(&terms, argc == 1 ? argv[0] : ખાલી);
	free_terms(&terms);
	પાછા res;
}

સ્થિર પૂર્ણાંક cmd_bisect__start(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix UNUSED,
			     સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક res;
	સંરચના bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	res = bisect_start(&terms, argc, argv);
	free_terms(&terms);
	પાછા res;
}

સ્થિર પૂર્ણાંક cmd_bisect__next(પૂર્ણાંક argc, અચલ અક્ષર **argv UNUSED, અચલ અક્ષર *prefix,
			    સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક res;
	સંરચના bisect_terms terms = { 0 };

	જો (argc)
		પાછા error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	જો (get_terms(&terms, 1))
		પાછા error(_("no terms defined"));
	res = bisect_next(&terms, prefix);
	free_terms(&terms);
	પાછા res;
}

સ્થિર પૂર્ણાંક cmd_bisect__log(પૂર્ણાંક argc UNUSED, અચલ અક્ષર **argv UNUSED,
			   અચલ અક્ષર *prefix UNUSED,
			   સંરચના repository *repo UNUSED)
{
	પાછા bisect_log();
}

સ્થિર પૂર્ણાંક cmd_bisect__replay(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix UNUSED,
			      સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક res;
	સંરચના bisect_terms terms = { 0 };

	જો (argc != 1)
		પાછા error(_("no logfile given"));
	set_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	પાછા res;
}

સ્થિર પૂર્ણાંક cmd_bisect__skip(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix UNUSED,
			    સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક res;
	સંરચના bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	જો (get_terms(&terms, 1))
		પાછા error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	પાછા res;
}

સ્થિર પૂર્ણાંક cmd_bisect__visualize(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix UNUSED,
				 સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક res;
	સંરચના bisect_terms terms = { 0 };

	જો (get_terms(&terms, 1))
		પાછા error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	પાછા res;
}

સ્થિર પૂર્ણાંક cmd_bisect__run(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix UNUSED,
			   સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક res;
	સંરચના bisect_terms terms = { 0 };

	જો (!argc)
		પાછા error(_("'%s' failed: no command provided."), "git bisect run");
	જો (get_terms(&terms, 1))
		પાછા error(_("no terms defined"));
	res = bisect_run(&terms, argc, argv);
	free_terms(&terms);
	પાછા res;
}

પૂર્ણાંક cmd_bisect(પૂર્ણાંક argc,
	       અચલ અક્ષર **argv,
	       અચલ અક્ષર *prefix,
	       સંરચના repository *repo)
{
	પૂર્ણાંક res = 0;
	parse_opt_subcommand_fn *fn = ખાલી;
	સંરચના option options[] = {
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

	જો (!fn) {
		સંરચના bisect_terms terms = { 0 };

		જો (!argc)
			usage_સંદેશ_opt(_("need a command"), git_bisect_usage, options);

		જો (!strcmp(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		set_terms(&terms, "bad", "good");
		જો (get_terms(&terms, 1))
			પાછા error(_("no terms defined"));
		જો (check_and_set_terms(&terms, argv[0]) ||
		    !one_of(argv[0], terms.term_good, terms.term_bad, ખાલી))
			usage_સંદેશ_optf(_("unknown command: '%s'"), git_bisect_usage,
				       options, argv[0]);
		નહીંતર
			res = bisect_state(&terms, argc, argv);
		free_terms(&terms);
	} નહીંતર {
		argc--;
		argv++;
		res = fn(argc, argv, prefix, repo);
	}

	જો (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		ગણના reset_when_found_mode mode;

		જો (read_reset_when_found(&mode))
			res = BISECT_FAILED;
		નહીંતર જો (mode != RESET_WHEN_FOUND_NONE &&
			 bisect_reset_when_found(mode))
			res = BISECT_FAILED;
	}

	પાછા is_bisect_success(res) ? 0 : -res;
}
