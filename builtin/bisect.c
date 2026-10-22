#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "copy.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "object-name.h"
#ضمن "parse-options.h"
#ضمن "bisect.h"
#ضمن "refs.h"
#ضمن "strvec.h"
#ضمن "run-command.h"
#ضمن "oid-array.h"
#ضمن "path.h"
#ضمن "prompt.h"
#ضمن "quote.h"
#ضمن "revision.h"

ساكن GIT_PATH_FUNC(git_path_bisect_terms, "BISECT_TERMS")
ساكن GIT_PATH_FUNC(git_path_bisect_ancestors_ok, "BISECT_ANCESTORS_OK")
ساكن GIT_PATH_FUNC(git_path_bisect_start, "BISECT_START")
ساكن GIT_PATH_FUNC(git_path_bisect_log, "BISECT_LOG")
ساكن GIT_PATH_FUNC(git_path_bisect_names, "BISECT_NAMES")
ساكن GIT_PATH_FUNC(git_path_bisect_first_parent, "BISECT_FIRST_PARENT")
ساكن GIT_PATH_FUNC(git_path_bisect_reset_when_found, "BISECT_RESET_WHEN_FOUND")
ساكن GIT_PATH_FUNC(git_path_bisect_run, "BISECT_RUN")

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

ساكن ثابت حرف * ثابت git_bisect_usage[] = {
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
	عدم
};

تعداد reset_when_found_mode {
	RESET_WHEN_FOUND_NONE,
	RESET_WHEN_FOUND_TO_ORIGINAL,
	RESET_WHEN_FOUND_TO_FOUND,
};

بنية add_bisect_ref_data {
	بنية rev_info *revs;
	unsigned صحيح object_flags;
};

بنية bisect_terms {
	حرف *term_good;
	حرف *term_bad;
};

ساكن فراغ free_terms(بنية bisect_terms *terms)
{
	FREE_AND_NULL(terms->term_good);
	FREE_AND_NULL(terms->term_bad);
}

ساكن فراغ set_terms(بنية bisect_terms *terms, ثابت حرف *bad,
		      ثابت حرف *good)
{
	حرر((فراغ *)terms->term_good);
	terms->term_good = xstrdup(good);
	حرر((فراغ *)terms->term_bad);
	terms->term_bad = xstrdup(bad);
}

ساكن ثابت حرف vocab_bad[] = "bad|new";
ساكن ثابت حرف vocab_good[] = "good|old";

ساكن صحيح bisect_autostart(بنية bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
LAST_ARG_MUST_BE_NULL
ساكن صحيح one_of(ثابت حرف *term, ...)
{
	صحيح res = 0;
	va_list matches;
	ثابت حرف *match;

	va_start(matches, term);
	طالما (!res && (match = va_arg(matches, ثابت حرف *)))
		res = !strcmp(term, match);
	va_end(matches);

	أرجع res;
}

/*
 * return code BISECT_INTERNAL_SUCCESS_MERGE_BASE
 * and BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND are codes
 * that indicate special success.
 */

ساكن صحيح is_bisect_success(تعداد bisect_error res)
{
	أرجع !res ||
		res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND ||
		res == BISECT_INTERNAL_SUCCESS_MERGE_BASE;
}

ساكن صحيح write_in_file(ثابت حرف *path, ثابت حرف *mode, ثابت حرف *format, va_list args)
{
	FILE *fp = عدم;
	صحيح res = 0;

	إذا (strcmp(mode, "w") && strcmp(mode, "a"))
		BUG("write-in-file does not support '%s' mode", mode);
	fp = fopen(path, mode);
	إذا (!fp)
		أرجع error_errno(_("cannot open file '%s' in mode '%s'"), path, mode);
	res = vfprintf(fp, format, args);

	إذا (res < 0) {
		صحيح saved_errno = errno;
		fclose(fp);
		errno = saved_errno;
		أرجع error_errno(_("could not write to file '%s'"), path);
	}

	أرجع fclose(fp);
}

__attribute__((format (printf, 2, 3)))
ساكن صحيح write_to_file(ثابت حرف *path, ثابت حرف *format, ...)
{
	صحيح res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "w", format, args);
	va_end(args);

	أرجع res;
}

__attribute__((format (printf, 2, 3)))
ساكن صحيح append_to_file(ثابت حرف *path, ثابت حرف *format, ...)
{
	صحيح res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "a", format, args);
	va_end(args);

	أرجع res;
}

ساكن صحيح print_file_to_stdout(ثابت حرف *path)
{
	صحيح fd = open(path, O_RDONLY);
	صحيح ret = 0;

	إذا (fd < 0)
		أرجع error_errno(_("cannot open file '%s' for reading"), path);
	إذا (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), path);
	close(fd);
	أرجع ret;
}

ساكن صحيح check_term_format(ثابت حرف *term, ثابت حرف *orig_term)
{
	صحيح res;
	حرف *new_term = xstrfmt("refs/bisect/%s", term);

	res = check_refname_format(new_term, 0);
	حرر(new_term);

	إذا (res)
		أرجع error(_("'%s' is not a valid term"), term);

	إذا (one_of(term, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", عدم))
		أرجع error(_("can't use the builtin command '%s' as a term"), term);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	إذا ((strcmp(orig_term, "bad") && one_of(term, "bad", "new", عدم)) ||
		 (strcmp(orig_term, "good") && one_of(term, "good", "old", عدم)))
		أرجع error(_("can't change the meaning of the term '%s'"), term);

	أرجع 0;
}

ساكن صحيح write_terms(ثابت حرف *bad, ثابت حرف *good)
{
	صحيح res;

	إذا (!strcmp(bad, good))
		أرجع error(_("please use two different terms"));

	إذا (check_term_format(bad, "bad") || check_term_format(good, "good"))
		أرجع -1;

	res = write_to_file(git_path_bisect_terms(), "%s\n%s\n", bad, good);

	أرجع res;
}

ساكن صحيح bisect_reset(ثابت حرف *commit, bool quiet)
{
	بنية strbuf branch = STRBUF_INIT;

	إذا (!commit) {
		إذا (!strbuf_read_file(&branch, git_path_bisect_start(), 0))
			printf(_("We are not bisecting.\n"));
		وإلا
			strbuf_rtrim(&branch);
	} وإلا {
		بنية object_id oid;

		إذا (repo_get_oid_commit(the_repository, commit, &oid))
			أرجع error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	إذا (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		بنية child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "checkout", "--ignore-other-worktrees", عدم);
		إذا (quiet)
			strvec_ڈالو(&cmd.args, "--quiet");
		strvec_pushl(&cmd.args, branch.buf, "--", عدم);
		إذا (run_command(&cmd)) {
			error(_("could not check out original"
				" HEAD '%s'. Try 'git bisect"
				" reset <commit>'."), branch.buf);
			strbuf_release(&branch);
			أرجع -1;
		}
	}

	strbuf_release(&branch);
	أرجع 0;
}

ساكن صحيح parse_reset_when_found(ثابت حرف *قدر,
				  تعداد reset_when_found_mode *mode)
{
	إذا (!strcmp(قدر, "original"))
		*mode = RESET_WHEN_FOUND_TO_ORIGINAL;
	وإلا إذا (!strcmp(قدر, "found"))
		*mode = RESET_WHEN_FOUND_TO_FOUND;
	وإلا
		أرجع error(_("invalid value for '--reset-when-found': '%s'"),
			     قدر);

	أرجع 0;
}

ساكن ثابت حرف *reset_when_found_mode_name(تعداد reset_when_found_mode mode)
{
	بدل (mode) {
	حالة RESET_WHEN_FOUND_TO_ORIGINAL:
		أرجع "original";
	حالة RESET_WHEN_FOUND_TO_FOUND:
		أرجع "found";
	حالة RESET_WHEN_FOUND_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", mode);
}

ساكن صحيح read_reset_when_found(تعداد reset_when_found_mode *mode)
{
	بنية strbuf قدر = STRBUF_INIT;
	صحيح res = 0;

	*mode = RESET_WHEN_FOUND_NONE;
	إذا (is_empty_or_missing_file(git_path_bisect_reset_when_found()))
		أرجع 0;

	إذا (strbuf_read_file(&قدر, git_path_bisect_reset_when_found(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_path_bisect_reset_when_found());
		اذهب out;
	}
	strbuf_trim(&قدر);
	إذا (parse_reset_when_found(قدر.buf, mode))
		res = -1;

out:
	strbuf_release(&قدر);
	أرجع res;
}

ساكن صحيح bisect_reset_when_found(تعداد reset_when_found_mode mode)
{
	بنية bisect_terms terms = { 0 };
	حرف *commit = عدم;
	صحيح res;

	إذا (mode == RESET_WHEN_FOUND_TO_FOUND) {
		read_bisect_terms(&terms.term_bad, &terms.term_good);
		commit = xstrfmt("refs/bisect/%s", terms.term_bad);
	} وإلا إذا (mode == RESET_WHEN_FOUND_NONE) {
		BUG("automatic reset requested without a reset mode");
	}

	res = bisect_reset(commit, true);
	إذا (!res)
		res = bisect_clean_state();

	حرر(commit);
	free_terms(&terms);
	أرجع res;
}

ساكن فراغ log_commit(FILE *fp,
		       ثابت حرف *fmt, ثابت حرف *state,
		       بنية commit *commit)
{
	بنية pretty_print_context pp = {0};
	بنية strbuf commit_پیغام = STRBUF_INIT;
	حرف *label = xstrfmt(fmt, state);

	repo_format_commit_message(the_repository, commit, "%s", &commit_پیغام,
				   &pp);

	fprintf(fp, "# %s: [%s] %s\n", label, oid_to_hex(&commit->object.oid),
		commit_پیغام.buf);

	strbuf_release(&commit_پیغام);
	حرر(label);
}

ساكن صحيح bisect_write(ثابت حرف *state, ثابت حرف *rev,
			ثابت بنية bisect_terms *terms, صحيح nolog)
{
	بنية strbuf tag = STRBUF_INIT;
	بنية object_id oid;
	بنية commit *commit;
	FILE *fp = عدم;
	صحيح res = 0;

	إذا (!strcmp(state, terms->term_bad)) {
		strbuf_addf(&tag, "refs/bisect/%s", state);
	} وإلا إذا (one_of(state, terms->term_good, "skip", عدم)) {
		strbuf_addf(&tag, "refs/bisect/%s-%s", state, rev);
	} وإلا {
		res = error(_("Bad bisect_write argument: %s"), state);
		اذهب finish;
	}

	إذا (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		اذهب finish;
	}

	إذا (refs_update_ref(get_main_ref_store(the_repository), عدم, tag.buf, &oid, عدم, 0,
			    UPDATE_REFS_پیغام_ON_ERR)) {
		res = -1;
		اذهب finish;
	}

	fp = fopen(git_path_bisect_log(), "a");
	إذا (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_path_bisect_log());
		اذهب finish;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	إذا (!nolog)
		fprintf(fp, "git bisect %s %s\n", state, rev);

finish:
	إذا (fp)
		fclose(fp);
	strbuf_release(&tag);
	أرجع res;
}

ساكن صحيح check_and_set_terms(بنية bisect_terms *terms, ثابت حرف *cmd)
{
	صحيح has_term_file = !is_empty_or_missing_file(git_path_bisect_terms());

	إذا (one_of(cmd, "skip", "start", "terms", عدم))
		أرجع 0;

	إذا (has_term_file && strcmp(cmd, terms->term_bad) &&
	    strcmp(cmd, terms->term_good))
		أرجع error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->term_bad,
				terms->term_good);

	إذا (!has_term_file) {
		إذا (one_of(cmd, "bad", "good", عدم)) {
			set_terms(terms, "bad", "good");
			أرجع write_terms(terms->term_bad, terms->term_good);
		}
		إذا (one_of(cmd, "new", "old", عدم)) {
			set_terms(terms, "new", "old");
			أرجع write_terms(terms->term_bad, terms->term_good);
		}
	}

	أرجع 0;
}

ساكن صحيح inc_nr(ثابت بنية reference *ref UNUSED, فراغ *cb_data)
{
	unsigned صحيح *nr = (unsigned صحيح *)cb_data;
	(*nr)++;
	أرجع 0;
}

ساكن ثابت حرف need_bad_and_good_revision_warning[] =
	N_("You need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

ساكن ثابت حرف need_bisect_start_warning[] =
	N_("You need to start by \"git bisect start\".\n"
	   "You then need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

ساكن صحيح decide_next(ثابت بنية bisect_terms *terms,
		       ثابت حرف *current_term, صحيح missing_good,
		       صحيح missing_bad)
{
	إذا (!missing_good && !missing_bad)
		أرجع 0;
	إذا (!current_term)
		أرجع -1;

	إذا (missing_good && !missing_bad &&
	    !strcmp(current_term, terms->term_good)) {
		حرف *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->term_bad);
		إذا (!isatty(0))
			أرجع 0;
		/*
		 * TRANSLATORS: Make sure to include [Y] and [n] in your
		 * translation. The program will only accept English input
		 * at this point.
		 */
		yesno = git_prompt(_("Are you sure [Y/n]? "), PROMPT_ECHO);
		إذا (starts_with(yesno, "N") || starts_with(yesno, "n"))
			أرجع -1;
		أرجع 0;
	}

	إذا (!is_empty_or_missing_file(git_path_bisect_start()))
		أرجع error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	وإلا
		أرجع error(_(need_bisect_start_warning),
			     vocab_good, vocab_bad, vocab_good, vocab_bad);
}

ساكن فراغ bisect_status(بنية bisect_state *state,
			  ثابت بنية bisect_terms *terms)
{
	حرف *bad_ref = xstrfmt("refs/bisect/%s", terms->term_bad);
	حرف *good_glob = xstrfmt("%s-*", terms->term_good);
	بنية refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	إذا (refs_ref_exists(get_main_ref_store(the_repository), bad_ref))
		state->nr_bad = 1;

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      inc_nr, &state->nr_good, &opts);

	حرر(good_glob);
	حرر(bad_ref);
}

__attribute__((format (printf, 1, 2)))
ساكن فراغ bisect_log_printf(ثابت حرف *fmt, ...)
{
	بنية strbuf buf = STRBUF_INIT;
	va_list ap;

	va_start(ap, fmt);
	strbuf_vaddf(&buf, fmt, ap);
	va_end(ap);

	printf("%s", buf.buf);
	append_to_file(git_path_bisect_log(), "# %s", buf.buf);

	strbuf_release(&buf);
}

ساكن فراغ bisect_print_status(ثابت بنية bisect_terms *terms)
{
	بنية bisect_state state = { 0 };

	bisect_status(&state, terms);

	/* If we had both, we'd already be started, and shouldn't get here. */
	إذا (state.nr_good && state.nr_bad)
		أرجع;

	إذا (!state.nr_good && !state.nr_bad)
		bisect_log_printf(_("status: waiting for both '%s' and '%s' commits\n"),
				  terms->term_good, terms->term_bad);
	وإلا إذا (state.nr_good)
		bisect_log_printf(Q_("status: waiting for '%s' commit, %d '%s' commit known\n",
				     "status: waiting for '%s' commit, %d '%s' commits known\n",
				     state.nr_good),
				  terms->term_bad, state.nr_good, terms->term_good);
	وإلا
		bisect_log_printf(_("status: waiting for '%s' commit(s), '%s' commit known\n"),
				  terms->term_good, terms->term_bad);
}

ساكن صحيح bisect_next_check(ثابت بنية bisect_terms *terms,
			     ثابت حرف *current_term)
{
	بنية bisect_state state = { 0 };
	bisect_status(&state, terms);
	أرجع decide_next(terms, current_term, !state.nr_good, !state.nr_bad);
}

ساكن صحيح get_terms(بنية bisect_terms *terms, صحيح file_missing_is_ok)
{
	بنية strbuf str = STRBUF_INIT;
	FILE *fp = عدم;
	صحيح res = 0;

	fp = fopen(git_path_bisect_terms(), "r");
	إذا (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		اذهب finish;
	}

	free_terms(terms);
	إذا (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		اذهب finish;
	}
	terms->term_bad = strbuf_detach(&str, عدم);
	إذا (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		FREE_AND_NULL(terms->term_bad);
		اذهب finish;
	}
	terms->term_good = strbuf_detach(&str, عدم);

finish:
	إذا (fp)
		fclose(fp);
	strbuf_release(&str);
	أرجع res;
}

ساكن صحيح bisect_terms(بنية bisect_terms *terms, ثابت حرف *option)
{
	إذا (get_terms(terms, 0))
		أرجع error(_("no terms defined"));

	إذا (!option) {
		printf(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->term_good, terms->term_bad);
		أرجع 0;
	}
	إذا (one_of(option, "--term-good", "--term-old", عدم))
		printf("%s\n", terms->term_good);
	وإلا إذا (one_of(option, "--term-bad", "--term-new", عدم))
		printf("%s\n", terms->term_bad);
	وإلا
		أرجع error(_("invalid argument %s for 'git bisect terms'.\n"
			       "Supported options are: "
			       "--term-good|--term-old and "
			       "--term-bad|--term-new."), option);

	أرجع 0;
}

ساكن صحيح bisect_append_log_quoted(ثابت حرف **argv)
{
	صحيح res = 0;
	FILE *fp = fopen(git_path_bisect_log(), "a");
	بنية strbuf orig_args = STRBUF_INIT;

	إذا (!fp)
		أرجع -1;

	إذا (fprintf(fp, "git bisect start") < 1) {
		res = -1;
		اذهب finish;
	}

	sq_quote_argv(&orig_args, argv);
	إذا (fprintf(fp, "%s\n", orig_args.buf) < 1)
		res = -1;

finish:
	fclose(fp);
	strbuf_release(&orig_args);
	أرجع res;
}

ساكن صحيح add_bisect_ref(ثابت بنية reference *ref, فراغ *cb)
{
	بنية add_bisect_ref_data *data = cb;

	add_pending_oid(data->revs, ref->name, ref->oid, data->object_flags);

	أرجع 0;
}

ساكن صحيح prepare_revs(بنية bisect_terms *terms, بنية rev_info *revs)
{
	بنية refs_for_each_ref_options opts = {
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};
	صحيح res = 0;
	بنية add_bisect_ref_data cb = { revs };
	حرف *good = xstrfmt("%s-*", terms->term_good);

	/*
	 * We cannot use terms->term_bad directly in
	 * for_each_glob_ref_in() and we have to append a '*' to it,
	 * otherwise for_each_glob_ref_in() will append '/' and '*'.
	 */
	حرف *bad = xstrfmt("%s*", terms->term_bad);

	/*
	 * It is important to reset the flags used by revision walks
	 * as the previous call to bisect_next_all() in turn
	 * sets up a revision walk.
	 */
	reset_revision_walk();
	repo_init_revisions(the_repository, revs, عدم);
	setup_revisions(0, عدم, revs, عدم);

	opts.pattern = bad;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      add_bisect_ref, &cb, &opts);

	cb.object_flags = UNINTERESTING;
	opts.pattern = good;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      add_bisect_ref, &cb, &opts);

	إذا (prepare_revision_walk(revs))
		res = error(_("revision walk setup failed"));

	حرر(good);
	حرر(bad);
	أرجع res;
}

ساكن صحيح bisect_skipped_commits(بنية bisect_terms *terms)
{
	صحيح res;
	FILE *fp = عدم;
	بنية rev_info revs;
	بنية commit *commit;
	بنية pretty_print_context pp = {0};
	بنية strbuf commit_name = STRBUF_INIT;

	res = prepare_revs(terms, &revs);
	إذا (res)
		أرجع res;

	fp = fopen(git_path_bisect_log(), "a");
	إذا (!fp)
		أرجع error_errno(_("could not open '%s' for appending"),
				  git_path_bisect_log());

	إذا (fprintf(fp, "# only skipped commits left to test\n") < 0)
		أرجع error_errno(_("failed to write to '%s'"), git_path_bisect_log());

	طالما ((commit = get_revision(&revs)) != عدم) {
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
	أرجع 0;
}

ساكن صحيح bisect_successful(بنية bisect_terms *terms)
{
	بنية object_id oid;
	بنية commit *commit;
	بنية pretty_print_context pp = {0};
	بنية strbuf commit_name = STRBUF_INIT;
	حرف *bad_ref = xstrfmt("refs/bisect/%s",terms->term_bad);
	صحيح res;

	refs_read_ref(get_main_ref_store(the_repository), bad_ref, &oid);
	commit = lookup_commit_reference_by_name(bad_ref);
	إذا (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		حرر(bad_ref);
		أرجع BISECT_FAILED;
	}
	repo_format_commit_message(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_to_file(git_path_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->term_bad, oid_to_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	حرر(bad_ref);
	أرجع res;
}

ساكن تعداد bisect_error bisect_next(بنية bisect_terms *terms,
				     ثابت حرف *prefix)
{
	تعداد bisect_error res;

	إذا (bisect_autostart(terms))
		أرجع BISECT_FAILED;

	إذا (bisect_next_check(terms, terms->term_good))
		أرجع BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_next_all(the_repository, prefix);

	إذا (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		res = bisect_successful(terms);
		أرجع res ? res : BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND;
	} وإلا إذا (res == BISECT_ONLY_SKIPPED_بایاں) {
		res = bisect_skipped_commits(terms);
		أرجع res ? res : BISECT_ONLY_SKIPPED_بایاں;
	}
	أرجع res;
}

ساكن تعداد bisect_error bisect_auto_next(بنية bisect_terms *terms,
					  ثابت حرف *prefix)
{
	إذا (bisect_next_check(terms, عدم)) {
		bisect_print_status(terms);
		أرجع BISECT_OK;
	}

	أرجع bisect_next(terms, prefix);
}

ساكن تعداد bisect_error bisect_start(بنية bisect_terms *terms, صحيح argc,
				      ثابت حرف **argv)
{
	صحيح no_checkout = 0;
	صحيح first_parent_only = 0;
	صحيح i, has_double_dash = 0, must_write_terms = 0, bad_seen = 0;
	صحيح flags, pathspec_pos;
	تعداد bisect_error res = BISECT_OK;
	بنية string_list revs = STRING_LIST_INIT_DUP;
	بنية string_list states = STRING_LIST_INIT_DUP;
	بنية strbuf start_head = STRBUF_INIT;
	بنية strbuf bisect_names = STRBUF_INIT;
	بنية object_id head_oid;
	بنية object_id oid;
	تعداد reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;
	ثابت حرف *head;

	إذا (is_bare_repository(the_repository))
		no_checkout = 1;

	/*
	 * Check for one bad and then some good revisions
	 */
	لكل (i = 0; i < argc; i++) {
		إذا (!strcmp(argv[i], "--")) {
			has_double_dash = 1;
			اكسر;
		}
	}

	لكل (i = 0; i < argc; i++) {
		ثابت حرف *arg = argv[i];
		إذا (!strcmp(argv[i], "--")) {
			اكسر;
		} وإلا إذا (!strcmp(arg, "--no-checkout")) {
			no_checkout = 1;
		} وإلا إذا (!strcmp(arg, "--first-parent")) {
			first_parent_only = 1;
		} وإلا إذا (!strcmp(arg, "--reset-when-found")) {
			reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
		} وإلا إذا (skip_prefix(arg, "--reset-when-found=", &arg)) {
			إذا (parse_reset_when_found(arg, &reset_when_found)) {
				res = BISECT_FAILED;
				اذهب finish;
			}
		} وإلا إذا (!strcmp(arg, "--term-good") ||
			 !strcmp(arg, "--term-old")) {
			i++;
			إذا (argc <= i)
				أرجع error(_("'' is not a valid term"));
			must_write_terms = 1;
			حرر((فراغ *) terms->term_good);
			terms->term_good = xstrdup(argv[i]);
		} وإلا إذا (skip_prefix(arg, "--term-good=", &arg) ||
			   skip_prefix(arg, "--term-old=", &arg)) {
			must_write_terms = 1;
			حرر((فراغ *) terms->term_good);
			terms->term_good = xstrdup(arg);
		} وإلا إذا (!strcmp(arg, "--term-bad") ||
			 !strcmp(arg, "--term-new")) {
			i++;
			إذا (argc <= i)
				أرجع error(_("'' is not a valid term"));
			must_write_terms = 1;
			حرر((فراغ *) terms->term_bad);
			terms->term_bad = xstrdup(argv[i]);
		} وإلا إذا (skip_prefix(arg, "--term-bad=", &arg) ||
			   skip_prefix(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			حرر((فراغ *) terms->term_bad);
			terms->term_bad = xstrdup(arg);
		} وإلا إذا (starts_with(arg, "--")) {
			أرجع error(_("unrecognized option: '%s'"), arg);
		} وإلا إذا (!get_oidf(&oid, "%s^{commit}", arg)) {
			string_list_append(&revs, oid_to_hex(&oid));
		} وإلا إذا (has_double_dash) {
			die(_("'%s' does not appear to be a valid "
			      "revision"), arg);
		} وإلا {
			اكسر;
		}
	}
	إذا (reset_when_found != RESET_WHEN_FOUND_NONE && no_checkout) {
		res = error(_("options '%s' and '%s' cannot be used together"),
			    "--reset-when-found", "--no-checkout");
		اذهب finish;
	}
	pathspec_pos = i;

	/*
	 * The user ran "git bisect start <sha1> <sha1>", hence did not
	 * explicitly specify the terms, but we are already starting to
	 * set references named with the default terms, and won't be able
	 * to change afterwards.
	 */
	إذا (revs.nr)
		must_write_terms = 1;
	لكل (i = 0; i < revs.nr; i++) {
		إذا (bad_seen) {
			string_list_append(&states, terms->term_good);
		} وإلا {
			bad_seen = 1;
			string_list_append(&states, terms->term_bad);
		}
	}

	/*
	 * Verify HEAD
	 */
	head = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
				       "HEAD", 0, &head_oid, &flags);
	إذا (!head) {
		إذا (repo_get_oid(the_repository, "HEAD", &head_oid))
			أرجع error(_("bad HEAD - I need a HEAD"));
		head = "HEAD";
	}

	/*
	 * Check if we are bisecting
	 */
	إذا (!is_empty_or_missing_file(git_path_bisect_start())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&start_head, git_path_bisect_start(), 0);
		strbuf_trim(&start_head);
		إذا (!no_checkout) {
			بنية child_process cmd = CHILD_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.args, "checkout", start_head.buf,
				     "--", عدم);
			إذا (run_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       start_head.buf);
				اذهب finish;
			}
		}
	} وإلا {
		/* Get the rev from where we start. */
		إذا (!repo_get_oid(the_repository, head, &head_oid) &&
		    !starts_with(head, "refs/heads/")) {
			strbuf_reset(&start_head);
			strbuf_add_oid_hex(&start_head, &head_oid);
		} وإلا إذا (!repo_get_oid(the_repository, head, &head_oid) &&
			   skip_prefix(head, "refs/heads/", &head)) {
			strbuf_addstr(&start_head, head);
		} وإلا {
			أرجع error(_("bad HEAD - strange symbolic ref"));
		}
	}

	/*
	 * Get rid of any old bisect state.
	 */
	إذا (bisect_clean_state())
		أرجع BISECT_FAILED;

	/*
	 * Write new start state
	 */
	write_file(git_path_bisect_start(), "%s\n", start_head.buf);

	إذا (first_parent_only)
		write_file(git_path_bisect_first_parent(), "\n");

	إذا (reset_when_found != RESET_WHEN_FOUND_NONE)
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));

	إذا (no_checkout) {
		إذا (repo_get_oid(the_repository, start_head.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), start_head.buf);
			اذهب finish;
		}
		إذا (refs_update_ref(get_main_ref_store(the_repository), عدم, "BISECT_HEAD", &oid, عدم, 0,
				    UPDATE_REFS_پیغام_ON_ERR)) {
			res = BISECT_FAILED;
			اذهب finish;
		}
	}

	إذا (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_names, argv + pathspec_pos);
	write_file(git_path_bisect_names(), "%s\n", bisect_names.buf);

	لكل (i = 0; i < states.nr; i++)
		إذا (bisect_write(states.اشیاء[i].string,
				 revs.اشیاء[i].string, terms, 1)) {
			res = BISECT_FAILED;
			اذهب finish;
		}

	إذا (must_write_terms && write_terms(terms->term_bad,
					    terms->term_good)) {
		res = BISECT_FAILED;
		اذهب finish;
	}

	res = bisect_append_log_quoted(argv);
	إذا (res)
		res = BISECT_FAILED;

finish:
	string_list_clear(&revs, 0);
	string_list_clear(&states, 0);
	strbuf_release(&start_head);
	strbuf_release(&bisect_names);
	إذا (res)
		أرجع res;

	res = bisect_auto_next(terms, عدم);
	إذا (!is_bisect_success(res))
		bisect_clean_state();
	أرجع res;
}

ساكن مضمن صحيح file_is_not_empty(ثابت حرف *path)
{
	أرجع !is_empty_or_missing_file(path);
}

ساكن صحيح bisect_autostart(بنية bisect_terms *terms)
{
	صحيح res;
	ثابت حرف *yesno;

	إذا (file_is_not_empty(git_path_bisect_start()))
		أرجع 0;

	fprintf_ln(stderr, _("You need to start by \"git bisect "
			  "start\"\n"));

	إذا (!isatty(STDIN_FILENO))
		أرجع -1;

	/*
	 * TRANSLATORS: Make sure to include [Y] and [n] in your
	 * translation. The program will only accept English input
	 * at this point.
	 */
	yesno = git_prompt(_("Do you want me to do it for you "
			     "[Y/n]? "), PROMPT_ECHO);
	res = tolower(*yesno) == 'n' ?
		-1 : bisect_start(terms, 0, empty_strvec);

	أرجع res;
}

ساكن تعداد bisect_error bisect_state(بنية bisect_terms *terms, صحيح argc,
				      ثابت حرف **argv)
{
	ثابت حرف *state;
	صحيح i, verify_expected = 1;
	بنية object_id oid, expected;
	بنية oid_array revs = OID_ARRAY_INIT;

	إذا (!argc)
		أرجع error(_("Please call `--bisect-state` with at least one argument"));

	إذا (bisect_autostart(terms))
		أرجع BISECT_FAILED;

	state = argv[0];
	إذا (check_and_set_terms(terms, state) ||
	    !one_of(state, terms->term_good, terms->term_bad, "skip", عدم))
		أرجع BISECT_FAILED;

	argv++;
	argc--;
	إذا (argc > 1 && !strcmp(state, terms->term_bad))
		أرجع error(_("'git bisect %s' can take only one argument."), terms->term_bad);

	إذا (argc == 0) {
		ثابت حرف *head = "BISECT_HEAD";
		تعداد get_oid_result res_head = repo_get_oid(the_repository,
							    head, &oid);

		إذا (res_head == MISSING_OBJECT) {
			head = "HEAD";
			res_head = repo_get_oid(the_repository, head, &oid);
		}

		إذا (res_head)
			error(_("Bad rev input: %s"), head);
		oid_array_append(&revs, &oid);
	}

	/*
	 * All input revs must be checked before executing bisect_write()
	 * to discard junk revs.
	 */

	لكل (; argc; argc--, argv++) {
		بنية commit *commit;

		إذا (repo_get_oid(the_repository, *argv, &oid)){
			error(_("Bad rev input: %s"), *argv);
			oid_array_clear(&revs);
			أرجع BISECT_FAILED;
		}

		commit = lookup_commit_reference(the_repository, &oid);
		إذا (!commit)
			die(_("Bad rev input (not a commit): %s"), *argv);

		oid_array_append(&revs, &commit->object.oid);
	}

	إذا (refs_read_ref(get_main_ref_store(the_repository), "BISECT_EXPECTED_REV", &expected))
		verify_expected = 0; /* Ignore invalid file contents */

	لكل (i = 0; i < revs.nr; i++) {
		إذا (bisect_write(state, oid_to_hex(&revs.oid[i]), terms, 0)) {
			oid_array_clear(&revs);
			أرجع BISECT_FAILED;
		}
		إذا (verify_expected && !oideq(&revs.oid[i], &expected)) {
			unlink_or_warn(git_path_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					عدم, "BISECT_EXPECTED_REV", عدم,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	أرجع bisect_auto_next(terms, عدم);
}

ساكن تعداد bisect_error bisect_log(فراغ)
{
	صحيح fd, status;
	ثابت حرف* filename = git_path_bisect_log();

	إذا (is_empty_or_missing_file(filename))
		أرجع error(_("We are not bisecting."));

	fd = open(filename, O_RDONLY);
	إذا (fd < 0)
		أرجع BISECT_FAILED;

	status = copy_fd(fd, STDOUT_FILENO);
	close(fd);
	أرجع status ? BISECT_FAILED : BISECT_OK;
}

ساكن صحيح process_replay_سطر(بنية bisect_terms *terms, بنية strbuf *سطر)
{
	ثابت حرف *p = سطر->buf + strspn(سطر->buf, " \t");
	حرف *word_end, *rev;

	إذا ((!skip_prefix(p, "git bisect", &p) &&
	!skip_prefix(p, "git-bisect", &p)) || !isspace(*p))
		أرجع 0;
	p += strspn(p, " \t");

	word_end = (حرف *)p + strcspn(p, " \t");
	rev = word_end + strspn(word_end, " \t");
	*word_end = '\0'; /* NUL-terminate the word */

	إذا (get_terms(terms, 1))
		أرجع error(_("no terms defined"));
	إذا (check_and_set_terms(terms, p))
		أرجع -1;

	إذا (!strcmp(p, "start")) {
		بنية strvec argv = STRVEC_INIT;
		صحيح res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_start(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		أرجع res;
	}

	إذا (one_of(p, terms->term_good,
	   terms->term_bad, "skip", عدم))
		أرجع bisect_write(p, rev, terms, 0);

	إذا (!strcmp(p, "terms")) {
		بنية strvec argv = STRVEC_INIT;
		صحيح res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_terms(terms, argv.nr == 1 ? argv.v[0] : عدم);
		strvec_clear(&argv);
		أرجع res;
	}
	error(_("'%s'?? what are you talking about?"), p);

	أرجع -1;
}

ساكن تعداد bisect_error bisect_replay(بنية bisect_terms *terms, ثابت حرف *filename)
{
	FILE *fp = عدم;
	تعداد bisect_error res = BISECT_OK;
	بنية strbuf سطر = STRBUF_INIT;

	إذا (is_empty_or_missing_file(filename))
		أرجع error(_("cannot read file '%s' for replaying"), filename);

	إذا (bisect_clean_state())
		أرجع BISECT_FAILED;

	fp = fopen(filename, "r");
	إذا (!fp)
		أرجع BISECT_FAILED;

	طالما ((strbuf_getline(&سطر, fp) != EOF) && !res)
		res = process_replay_سطر(terms, &سطر);

	strbuf_release(&سطر);
	fclose(fp);

	إذا (res)
		أرجع BISECT_FAILED;

	أرجع bisect_auto_next(terms, عدم);
}

ساكن تعداد bisect_error bisect_skip(بنية bisect_terms *terms, صحيح argc,
				     ثابت حرف **argv)
{
	صحيح i;
	تعداد bisect_error res;
	بنية strvec argv_state = STRVEC_INIT;

	strvec_ڈالو(&argv_state, "skip");

	لكل (i = 0; i < argc; i++) {
		ثابت حرف *dotdot = strstr(argv[i], "..");

		إذا (dotdot) {
			بنية rev_info revs;
			بنية commit *commit;

			repo_init_revisions(the_repository, &revs, عدم);
			setup_revisions(2, argv + i - 1, &revs, عدم);

			إذا (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			طالما ((commit = get_revision(&revs)) != عدم)
				strvec_ڈالو(&argv_state,
						oid_to_hex(&commit->object.oid));

			reset_revision_walk();
			release_revisions(&revs);
		} وإلا {
			strvec_ڈالو(&argv_state, argv[i]);
		}
	}
	res = bisect_state(terms, argv_state.nr, argv_state.v);

	strvec_clear(&argv_state);
	أرجع res;
}

ساكن صحيح bisect_visualize(بنية bisect_terms *terms, صحيح argc,
			    ثابت حرف **argv)
{
	بنية child_process cmd = CHILD_PROCESS_INIT;
	بنية strbuf sb = STRBUF_INIT;

	إذا (bisect_next_check(terms, عدم) != 0)
		أرجع BISECT_FAILED;

	cmd.no_stdin = 1;
	إذا (!argc) {
		إذا ((getenv("DISPLAY") || getenv("SESSIONNAME") || getenv("MSYSTEM") ||
		     getenv("SECURITYSESSIONID")) && exists_in_PATH("gitk")) {
			strvec_ڈالو(&cmd.args, "gitk");
		} وإلا {
			strvec_ڈالو(&cmd.args, "log");
			cmd.git_cmd = 1;
		}
	} وإلا {
		إذا (argv[0][0] == '-') {
			strvec_ڈالو(&cmd.args, "log");
			cmd.git_cmd = 1;
		} وإلا إذا (strcmp(argv[0], "tig") && !starts_with(argv[0], "git"))
			cmd.git_cmd = 1;

		strvec_pushv(&cmd.args, argv);
	}

	strvec_pushl(&cmd.args, "--bisect", "--", عدم);

	strbuf_read_file(&sb, git_path_bisect_names(), 0);
	sq_dequote_to_strvec(sb.buf, &cmd.args);
	strbuf_release(&sb);

	أرجع run_command(&cmd);
}

ساكن صحيح get_first_good(ثابت بنية reference *ref, فراغ *cb_data)
{
	oidcpy(cb_data, ref->oid);
	أرجع 1;
}

ساكن صحيح do_bisect_run(ثابت حرف *command)
{
	بنية child_process cmd = CHILD_PROCESS_INIT;

	printf(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_ڈالو(&cmd.args, command);
	أرجع run_command(&cmd);
}

ساكن صحيح verify_good(ثابت بنية bisect_terms *terms, ثابت حرف *command)
{
	صحيح rc;
	تعداد bisect_error res;
	بنية object_id good_rev;
	بنية object_id current_rev;
	حرف *good_glob = xstrfmt("%s-*", terms->term_good);
	صحيح no_checkout = refs_ref_exists(get_main_ref_store(the_repository),
					  "BISECT_HEAD");
	بنية refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_first_good, &good_rev, &opts);
	حرر(good_glob);

	إذا (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		أرجع -1;

	res = bisect_checkout(&good_rev, no_checkout);
	إذا (res != BISECT_OK)
		أرجع -1;

	rc = do_bisect_run(command);

	res = bisect_checkout(&current_rev, no_checkout);
	إذا (res != BISECT_OK)
		أرجع -1;

	أرجع rc;
}

ساكن صحيح bisect_run(بنية bisect_terms *terms, صحيح argc, ثابت حرف **argv)
{
	صحيح res = BISECT_OK;
	بنية strbuf command = STRBUF_INIT;
	ثابت حرف *reset_when_found_arg;
	ثابت حرف *new_state;
	صحيح temporary_stdout_fd, saved_stdout;
	صحيح is_first_run = 1;
	تعداد reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;

	إذا (bisect_next_check(terms, عدم))
		أرجع BISECT_FAILED;

	إذا (argc && !strcmp(argv[0], "--reset-when-found")) {
		reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
	} وإلا إذا (argc && skip_prefix(argv[0], "--reset-when-found=",
				    &reset_when_found_arg)) {
		إذا (parse_reset_when_found(reset_when_found_arg,
					   &reset_when_found))
			أرجع BISECT_FAILED;
	}

	إذا (reset_when_found != RESET_WHEN_FOUND_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		أرجع error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	إذا (reset_when_found != RESET_WHEN_FOUND_NONE) {
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));
		argc--;
		argv++;
	}

	إذا (!argc) {
		error(_("bisect run failed: no command provided."));
		أرجع BISECT_FAILED;
	}

	sq_quote_argv(&command, argv);
	strbuf_ltrim(&command);
	طالما (1) {
		res = do_bisect_run(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		إذا (is_first_run && (res == 126 || res == 127)) {
			صحيح rc = verify_good(terms, command.buf);
			is_first_run = 0;
			إذا (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->term_good);
				res = BISECT_FAILED;
				اكسر;
			}
			إذا (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->term_good);
				res = BISECT_FAILED;
				اكسر;
			}
		}

		إذا (res < 0 || 128 <= res) {
			error(_("bisect run failed: exit code %d from"
				" %s is < 0 or >= 128"), res, command.buf);
			اكسر;
		}

		إذا (res == 125)
			new_state = "skip";
		وإلا إذا (!res)
			new_state = terms->term_good;
		وإلا
			new_state = terms->term_bad;

		temporary_stdout_fd = open(git_path_bisect_run(), O_CREAT | O_WRONLY | O_TRUNC, 0666);

		إذا (temporary_stdout_fd < 0) {
			res = error_errno(_("cannot open file '%s' for writing"), git_path_bisect_run());
			اكسر;
		}

		fflush(stdout);
		saved_stdout = dup(1);
		إذا (saved_stdout < 0 ||
		    dup2(temporary_stdout_fd, 1) < 0) {
			res = error_errno(_("could not duplicate stdout"));
			إذا (saved_stdout >= 0)
				close(saved_stdout);
			close(temporary_stdout_fd);
			اكسر;
		}

		res = bisect_state(terms, 1, &new_state);

		fflush(stdout);
		dup2(saved_stdout, 1);
		close(saved_stdout);
		close(temporary_stdout_fd);

		print_file_to_stdout(git_path_bisect_run());

		إذا (res == BISECT_ONLY_SKIPPED_بایاں)
			error(_("bisect run cannot continue any more"));
		وإلا إذا (res == BISECT_INTERNAL_SUCCESS_MERGE_BASE) {
			puts(_("bisect run success"));
			res = BISECT_OK;
		} وإلا إذا (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
			printf(_("bisect found first '%s' commit\n"), terms->term_bad);
		} وإلا إذا (res) {
			error(_("bisect run failed: 'git bisect %s'"
				" exited with error code %d"), new_state, res);
		} وإلا {
			استمر;
		}
		اكسر;
	}

	strbuf_release(&command);
	أرجع res;
}

ساكن صحيح cmd_bisect__reset(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix UNUSED,
			     بنية repository *repo UNUSED)
{
	صحيح res;

	إذا (argc > 1)
		أرجع error(_("'%s' requires either no argument or a commit"),
			     "git bisect reset");
	res = bisect_reset(argc ? argv[0] : عدم, false);
	إذا (res)
		أرجع res;
	أرجع bisect_clean_state();
}

ساكن صحيح cmd_bisect__terms(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix UNUSED,
			     بنية repository *repo UNUSED)
{
	صحيح res;
	بنية bisect_terms terms = { 0 };

	إذا (argc > 1)
		أرجع error(_("'%s' requires 0 or 1 argument"),
			     "git bisect terms");
	res = bisect_terms(&terms, argc == 1 ? argv[0] : عدم);
	free_terms(&terms);
	أرجع res;
}

ساكن صحيح cmd_bisect__start(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix UNUSED,
			     بنية repository *repo UNUSED)
{
	صحيح res;
	بنية bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	res = bisect_start(&terms, argc, argv);
	free_terms(&terms);
	أرجع res;
}

ساكن صحيح cmd_bisect__next(صحيح argc, ثابت حرف **argv UNUSED, ثابت حرف *prefix,
			    بنية repository *repo UNUSED)
{
	صحيح res;
	بنية bisect_terms terms = { 0 };

	إذا (argc)
		أرجع error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	إذا (get_terms(&terms, 1))
		أرجع error(_("no terms defined"));
	res = bisect_next(&terms, prefix);
	free_terms(&terms);
	أرجع res;
}

ساكن صحيح cmd_bisect__log(صحيح argc UNUSED, ثابت حرف **argv UNUSED,
			   ثابت حرف *prefix UNUSED,
			   بنية repository *repo UNUSED)
{
	أرجع bisect_log();
}

ساكن صحيح cmd_bisect__replay(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix UNUSED,
			      بنية repository *repo UNUSED)
{
	صحيح res;
	بنية bisect_terms terms = { 0 };

	إذا (argc != 1)
		أرجع error(_("no logfile given"));
	set_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	أرجع res;
}

ساكن صحيح cmd_bisect__skip(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix UNUSED,
			    بنية repository *repo UNUSED)
{
	صحيح res;
	بنية bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	إذا (get_terms(&terms, 1))
		أرجع error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	أرجع res;
}

ساكن صحيح cmd_bisect__visualize(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix UNUSED,
				 بنية repository *repo UNUSED)
{
	صحيح res;
	بنية bisect_terms terms = { 0 };

	إذا (get_terms(&terms, 1))
		أرجع error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	أرجع res;
}

ساكن صحيح cmd_bisect__run(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix UNUSED,
			   بنية repository *repo UNUSED)
{
	صحيح res;
	بنية bisect_terms terms = { 0 };

	إذا (!argc)
		أرجع error(_("'%s' failed: no command provided."), "git bisect run");
	إذا (get_terms(&terms, 1))
		أرجع error(_("no terms defined"));
	res = bisect_run(&terms, argc, argv);
	free_terms(&terms);
	أرجع res;
}

صحيح cmd_bisect(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *prefix,
	       بنية repository *repo)
{
	صحيح res = 0;
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
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

	إذا (!fn) {
		بنية bisect_terms terms = { 0 };

		إذا (!argc)
			usage_پیغام_opt(_("need a command"), git_bisect_usage, options);

		إذا (!strcmp(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		set_terms(&terms, "bad", "good");
		إذا (get_terms(&terms, 1))
			أرجع error(_("no terms defined"));
		إذا (check_and_set_terms(&terms, argv[0]) ||
		    !one_of(argv[0], terms.term_good, terms.term_bad, عدم))
			usage_پیغام_optf(_("unknown command: '%s'"), git_bisect_usage,
				       options, argv[0]);
		وإلا
			res = bisect_state(&terms, argc, argv);
		free_terms(&terms);
	} وإلا {
		argc--;
		argv++;
		res = fn(argc, argv, prefix, repo);
	}

	إذا (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		تعداد reset_when_found_mode mode;

		إذا (read_reset_when_found(&mode))
			res = BISECT_FAILED;
		وإلا إذا (mode != RESET_WHEN_FOUND_NONE &&
			 bisect_reset_when_found(mode))
			res = BISECT_FAILED;
	}

	أرجع is_bisect_success(res) ? 0 : -res;
}
