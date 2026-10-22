#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "copy.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "bisect.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "strvec.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "oid-array.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "prompt.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "revision.h"

ಸ್ಥಿರ GIT_PATH_FUNC(git_path_bisect_terms, "BISECT_TERMS")
ಸ್ಥಿರ GIT_PATH_FUNC(git_path_bisect_ancestors_ok, "BISECT_ANCESTORS_OK")
ಸ್ಥಿರ GIT_PATH_FUNC(git_path_bisect_start, "BISECT_START")
ಸ್ಥಿರ GIT_PATH_FUNC(git_path_bisect_log, "BISECT_LOG")
ಸ್ಥಿರ GIT_PATH_FUNC(git_path_bisect_names, "BISECT_NAMES")
ಸ್ಥಿರ GIT_PATH_FUNC(git_path_bisect_first_parent, "BISECT_FIRST_PARENT")
ಸ್ಥಿರ GIT_PATH_FUNC(git_path_bisect_reset_when_found, "BISECT_RESET_WHEN_FOUND")
ಸ್ಥಿರ GIT_PATH_FUNC(git_path_bisect_run, "BISECT_RUN")

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

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_bisect_usage[] = {
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
	ರಿಕ್ತ
};

ಗಣನಾ reset_when_found_mode {
	RESET_WHEN_FOUND_NONE,
	RESET_WHEN_FOUND_TO_ORIGINAL,
	RESET_WHEN_FOUND_TO_FOUND,
};

ಸಂರಚನಾ add_bisect_ref_data {
	ಸಂರಚನಾ rev_info *revs;
	unsigned ಪೂರ್ಣಾಙ್ಕ object_flags;
};

ಸಂರಚನಾ bisect_terms {
	ವರ್ಣ *term_good;
	ವರ್ಣ *term_bad;
};

ಸ್ಥಿರ ಶೂನ್ಯ free_terms(ಸಂರಚನಾ bisect_terms *terms)
{
	FREE_AND_NULL(terms->term_good);
	FREE_AND_NULL(terms->term_bad);
}

ಸ್ಥಿರ ಶೂನ್ಯ set_terms(ಸಂರಚನಾ bisect_terms *terms, ಧ್ರುವ ವರ್ಣ *bad,
		      ಧ್ರುವ ವರ್ಣ *good)
{
	ಮುಕ್ತ((ಶೂನ್ಯ *)terms->term_good);
	terms->term_good = xstrdup(good);
	ಮುಕ್ತ((ಶೂನ್ಯ *)terms->term_bad);
	terms->term_bad = xstrdup(bad);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ vocab_bad[] = "bad|new";
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ vocab_good[] = "good|old";

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ bisect_autostart(ಸಂರಚನಾ bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
LAST_ARG_MUST_BE_NULL
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ one_of(ಧ್ರುವ ವರ್ಣ *term, ...)
{
	ಪೂರ್ಣಾಙ್ಕ res = 0;
	va_list matches;
	ಧ್ರುವ ವರ್ಣ *match;

	va_start(matches, term);
	ಯಾವತ್ (!res && (match = va_arg(matches, ಧ್ರುವ ವರ್ಣ *)))
		res = !strcmp(term, match);
	va_end(matches);

	ಪ್ರತ್ಯಾಗಮ res;
}

/*
 * return code BISECT_INTERNAL_SUCCESS_MERGE_BASE
 * and BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND are codes
 * that indicate special success.
 */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ is_bisect_success(ಗಣನಾ bisect_error res)
{
	ಪ್ರತ್ಯಾಗಮ !res ||
		res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND ||
		res == BISECT_INTERNAL_SUCCESS_MERGE_BASE;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ write_in_file(ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *mode, ಧ್ರುವ ವರ್ಣ *format, va_list args)
{
	FILE *fp = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ res = 0;

	ಯದಿ (strcmp(mode, "w") && strcmp(mode, "a"))
		BUG("write-in-file does not support '%s' mode", mode);
	fp = fopen(path, mode);
	ಯದಿ (!fp)
		ಪ್ರತ್ಯಾಗಮ error_errno(_("cannot open file '%s' in mode '%s'"), path, mode);
	res = vfprintf(fp, format, args);

	ಯದಿ (res < 0) {
		ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
		fclose(fp);
		errno = saved_errno;
		ಪ್ರತ್ಯಾಗಮ error_errno(_("could not write to file '%s'"), path);
	}

	ಪ್ರತ್ಯಾಗಮ fclose(fp);
}

__attribute__((format (printf, 2, 3)))
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ write_to_file(ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *format, ...)
{
	ಪೂರ್ಣಾಙ್ಕ res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "w", format, args);
	va_end(args);

	ಪ್ರತ್ಯಾಗಮ res;
}

__attribute__((format (printf, 2, 3)))
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ append_to_file(ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *format, ...)
{
	ಪೂರ್ಣಾಙ್ಕ res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "a", format, args);
	va_end(args);

	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ print_file_to_stdout(ಧ್ರುವ ವರ್ಣ *path)
{
	ಪೂರ್ಣಾಙ್ಕ fd = open(path, O_RDONLY);
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	ಯದಿ (fd < 0)
		ಪ್ರತ್ಯಾಗಮ error_errno(_("cannot open file '%s' for reading"), path);
	ಯದಿ (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), path);
	close(fd);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_term_format(ಧ್ರುವ ವರ್ಣ *term, ಧ್ರುವ ವರ್ಣ *orig_term)
{
	ಪೂರ್ಣಾಙ್ಕ res;
	ವರ್ಣ *new_term = xstrfmt("refs/bisect/%s", term);

	res = check_refname_format(new_term, 0);
	ಮುಕ್ತ(new_term);

	ಯದಿ (res)
		ಪ್ರತ್ಯಾಗಮ error(_("'%s' is not a valid term"), term);

	ಯದಿ (one_of(term, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", ರಿಕ್ತ))
		ಪ್ರತ್ಯಾಗಮ error(_("can't use the builtin command '%s' as a term"), term);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	ಯದಿ ((strcmp(orig_term, "bad") && one_of(term, "bad", "new", ರಿಕ್ತ)) ||
		 (strcmp(orig_term, "good") && one_of(term, "good", "old", ರಿಕ್ತ)))
		ಪ್ರತ್ಯಾಗಮ error(_("can't change the meaning of the term '%s'"), term);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ write_terms(ಧ್ರುವ ವರ್ಣ *bad, ಧ್ರುವ ವರ್ಣ *good)
{
	ಪೂರ್ಣಾಙ್ಕ res;

	ಯದಿ (!strcmp(bad, good))
		ಪ್ರತ್ಯಾಗಮ error(_("please use two different terms"));

	ಯದಿ (check_term_format(bad, "bad") || check_term_format(good, "good"))
		ಪ್ರತ್ಯಾಗಮ -1;

	res = write_to_file(git_path_bisect_terms(), "%s\n%s\n", bad, good);

	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ bisect_reset(ಧ್ರುವ ವರ್ಣ *commit, bool quiet)
{
	ಸಂರಚನಾ strbuf branch = STRBUF_INIT;

	ಯದಿ (!commit) {
		ಯದಿ (!strbuf_read_file(&branch, git_path_bisect_start(), 0))
			printf(_("We are not bisecting.\n"));
		ಅನ್ಯಥಾ
			strbuf_rtrim(&branch);
	} ಅನ್ಯಥಾ {
		ಸಂರಚನಾ object_id oid;

		ಯದಿ (repo_get_oid_commit(the_repository, commit, &oid))
			ಪ್ರತ್ಯಾಗಮ error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	ಯದಿ (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "checkout", "--ignore-other-worktrees", ರಿಕ್ತ);
		ಯದಿ (quiet)
			strvec_ತಳ್ಳು(&cmd.args, "--quiet");
		strvec_pushl(&cmd.args, branch.buf, "--", ರಿಕ್ತ);
		ಯದಿ (run_command(&cmd)) {
			error(_("could not check out original"
				" HEAD '%s'. Try 'git bisect"
				" reset <commit>'."), branch.buf);
			strbuf_release(&branch);
			ಪ್ರತ್ಯಾಗಮ -1;
		}
	}

	strbuf_release(&branch);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_reset_when_found(ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
				  ಗಣನಾ reset_when_found_mode *mode)
{
	ಯದಿ (!strcmp(ಮೌಲ್ಯ, "original"))
		*mode = RESET_WHEN_FOUND_TO_ORIGINAL;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(ಮೌಲ್ಯ, "found"))
		*mode = RESET_WHEN_FOUND_TO_FOUND;
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ error(_("invalid value for '--reset-when-found': '%s'"),
			     ಮೌಲ್ಯ);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *reset_when_found_mode_name(ಗಣನಾ reset_when_found_mode mode)
{
	ವಿಕಲ್ಪ (mode) {
	ಪಕ್ಷ RESET_WHEN_FOUND_TO_ORIGINAL:
		ಪ್ರತ್ಯಾಗಮ "original";
	ಪಕ್ಷ RESET_WHEN_FOUND_TO_FOUND:
		ಪ್ರತ್ಯಾಗಮ "found";
	ಪಕ್ಷ RESET_WHEN_FOUND_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", mode);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ read_reset_when_found(ಗಣನಾ reset_when_found_mode *mode)
{
	ಸಂರಚನಾ strbuf ಮೌಲ್ಯ = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ res = 0;

	*mode = RESET_WHEN_FOUND_NONE;
	ಯದಿ (is_empty_or_missing_file(git_path_bisect_reset_when_found()))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (strbuf_read_file(&ಮೌಲ್ಯ, git_path_bisect_reset_when_found(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_path_bisect_reset_when_found());
		ಗಚ್ಛ out;
	}
	strbuf_trim(&ಮೌಲ್ಯ);
	ಯದಿ (parse_reset_when_found(ಮೌಲ್ಯ.buf, mode))
		res = -1;

out:
	strbuf_release(&ಮೌಲ್ಯ);
	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ bisect_reset_when_found(ಗಣನಾ reset_when_found_mode mode)
{
	ಸಂರಚನಾ bisect_terms terms = { 0 };
	ವರ್ಣ *commit = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ res;

	ಯದಿ (mode == RESET_WHEN_FOUND_TO_FOUND) {
		read_bisect_terms(&terms.term_bad, &terms.term_good);
		commit = xstrfmt("refs/bisect/%s", terms.term_bad);
	} ಅನ್ಯಥಾ ಯದಿ (mode == RESET_WHEN_FOUND_NONE) {
		BUG("automatic reset requested without a reset mode");
	}

	res = bisect_reset(commit, true);
	ಯದಿ (!res)
		res = bisect_clean_state();

	ಮುಕ್ತ(commit);
	free_terms(&terms);
	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಶೂನ್ಯ log_commit(FILE *fp,
		       ಧ್ರುವ ವರ್ಣ *fmt, ಧ್ರುವ ವರ್ಣ *state,
		       ಸಂರಚನಾ commit *commit)
{
	ಸಂರಚನಾ pretty_print_context pp = {0};
	ಸಂರಚನಾ strbuf commit_ಸಂದೇಶ = STRBUF_INIT;
	ವರ್ಣ *label = xstrfmt(fmt, state);

	repo_format_commit_message(the_repository, commit, "%s", &commit_ಸಂದೇಶ,
				   &pp);

	fprintf(fp, "# %s: [%s] %s\n", label, oid_to_hex(&commit->object.oid),
		commit_ಸಂದೇಶ.buf);

	strbuf_release(&commit_ಸಂದೇಶ);
	ಮುಕ್ತ(label);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ bisect_write(ಧ್ರುವ ವರ್ಣ *state, ಧ್ರುವ ವರ್ಣ *rev,
			ಧ್ರುವ ಸಂರಚನಾ bisect_terms *terms, ಪೂರ್ಣಾಙ್ಕ nolog)
{
	ಸಂರಚನಾ strbuf tag = STRBUF_INIT;
	ಸಂರಚನಾ object_id oid;
	ಸಂರಚನಾ commit *commit;
	FILE *fp = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ res = 0;

	ಯದಿ (!strcmp(state, terms->term_bad)) {
		strbuf_addf(&tag, "refs/bisect/%s", state);
	} ಅನ್ಯಥಾ ಯದಿ (one_of(state, terms->term_good, "skip", ರಿಕ್ತ)) {
		strbuf_addf(&tag, "refs/bisect/%s-%s", state, rev);
	} ಅನ್ಯಥಾ {
		res = error(_("Bad bisect_write argument: %s"), state);
		ಗಚ್ಛ finish;
	}

	ಯದಿ (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		ಗಚ್ಛ finish;
	}

	ಯದಿ (refs_update_ref(get_main_ref_store(the_repository), ರಿಕ್ತ, tag.buf, &oid, ರಿಕ್ತ, 0,
			    UPDATE_REFS_ಸಂದೇಶ_ON_ERR)) {
		res = -1;
		ಗಚ್ಛ finish;
	}

	fp = fopen(git_path_bisect_log(), "a");
	ಯದಿ (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_path_bisect_log());
		ಗಚ್ಛ finish;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	ಯದಿ (!nolog)
		fprintf(fp, "git bisect %s %s\n", state, rev);

finish:
	ಯದಿ (fp)
		fclose(fp);
	strbuf_release(&tag);
	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_and_set_terms(ಸಂರಚನಾ bisect_terms *terms, ಧ್ರುವ ವರ್ಣ *cmd)
{
	ಪೂರ್ಣಾಙ್ಕ has_term_file = !is_empty_or_missing_file(git_path_bisect_terms());

	ಯದಿ (one_of(cmd, "skip", "start", "terms", ರಿಕ್ತ))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (has_term_file && strcmp(cmd, terms->term_bad) &&
	    strcmp(cmd, terms->term_good))
		ಪ್ರತ್ಯಾಗಮ error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->term_bad,
				terms->term_good);

	ಯದಿ (!has_term_file) {
		ಯದಿ (one_of(cmd, "bad", "good", ರಿಕ್ತ)) {
			set_terms(terms, "bad", "good");
			ಪ್ರತ್ಯಾಗಮ write_terms(terms->term_bad, terms->term_good);
		}
		ಯದಿ (one_of(cmd, "new", "old", ರಿಕ್ತ)) {
			set_terms(terms, "new", "old");
			ಪ್ರತ್ಯಾಗಮ write_terms(terms->term_bad, terms->term_good);
		}
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ inc_nr(ಧ್ರುವ ಸಂರಚನಾ reference *ref UNUSED, ಶೂನ್ಯ *cb_data)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ *nr = (unsigned ಪೂರ್ಣಾಙ್ಕ *)cb_data;
	(*nr)++;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ need_bad_and_good_revision_warning[] =
	N_("You need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ need_bisect_start_warning[] =
	N_("You need to start by \"git bisect start\".\n"
	   "You then need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ decide_next(ಧ್ರುವ ಸಂರಚನಾ bisect_terms *terms,
		       ಧ್ರುವ ವರ್ಣ *current_term, ಪೂರ್ಣಾಙ್ಕ missing_good,
		       ಪೂರ್ಣಾಙ್ಕ missing_bad)
{
	ಯದಿ (!missing_good && !missing_bad)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (!current_term)
		ಪ್ರತ್ಯಾಗಮ -1;

	ಯದಿ (missing_good && !missing_bad &&
	    !strcmp(current_term, terms->term_good)) {
		ವರ್ಣ *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->term_bad);
		ಯದಿ (!isatty(0))
			ಪ್ರತ್ಯಾಗಮ 0;
		/*
		 * TRANSLATORS: Make sure to include [Y] and [n] in your
		 * translation. The program will only accept English input
		 * at this point.
		 */
		yesno = git_prompt(_("Are you sure [Y/n]? "), PROMPT_ECHO);
		ಯದಿ (starts_with(yesno, "N") || starts_with(yesno, "n"))
			ಪ್ರತ್ಯಾಗಮ -1;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!is_empty_or_missing_file(git_path_bisect_start()))
		ಪ್ರತ್ಯಾಗಮ error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ error(_(need_bisect_start_warning),
			     vocab_good, vocab_bad, vocab_good, vocab_bad);
}

ಸ್ಥಿರ ಶೂನ್ಯ bisect_status(ಸಂರಚನಾ bisect_state *state,
			  ಧ್ರುವ ಸಂರಚನಾ bisect_terms *terms)
{
	ವರ್ಣ *bad_ref = xstrfmt("refs/bisect/%s", terms->term_bad);
	ವರ್ಣ *good_glob = xstrfmt("%s-*", terms->term_good);
	ಸಂರಚನಾ refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	ಯದಿ (refs_ref_exists(get_main_ref_store(the_repository), bad_ref))
		state->nr_bad = 1;

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      inc_nr, &state->nr_good, &opts);

	ಮುಕ್ತ(good_glob);
	ಮುಕ್ತ(bad_ref);
}

__attribute__((format (printf, 1, 2)))
ಸ್ಥಿರ ಶೂನ್ಯ bisect_log_printf(ಧ್ರುವ ವರ್ಣ *fmt, ...)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	va_list ap;

	va_start(ap, fmt);
	strbuf_vaddf(&buf, fmt, ap);
	va_end(ap);

	printf("%s", buf.buf);
	append_to_file(git_path_bisect_log(), "# %s", buf.buf);

	strbuf_release(&buf);
}

ಸ್ಥಿರ ಶೂನ್ಯ bisect_print_status(ಧ್ರುವ ಸಂರಚನಾ bisect_terms *terms)
{
	ಸಂರಚನಾ bisect_state state = { 0 };

	bisect_status(&state, terms);

	/* If we had both, we'd already be started, and shouldn't get here. */
	ಯದಿ (state.nr_good && state.nr_bad)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (!state.nr_good && !state.nr_bad)
		bisect_log_printf(_("status: waiting for both '%s' and '%s' commits\n"),
				  terms->term_good, terms->term_bad);
	ಅನ್ಯಥಾ ಯದಿ (state.nr_good)
		bisect_log_printf(Q_("status: waiting for '%s' commit, %d '%s' commit known\n",
				     "status: waiting for '%s' commit, %d '%s' commits known\n",
				     state.nr_good),
				  terms->term_bad, state.nr_good, terms->term_good);
	ಅನ್ಯಥಾ
		bisect_log_printf(_("status: waiting for '%s' commit(s), '%s' commit known\n"),
				  terms->term_good, terms->term_bad);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ bisect_next_check(ಧ್ರುವ ಸಂರಚನಾ bisect_terms *terms,
			     ಧ್ರುವ ವರ್ಣ *current_term)
{
	ಸಂರಚನಾ bisect_state state = { 0 };
	bisect_status(&state, terms);
	ಪ್ರತ್ಯಾಗಮ decide_next(terms, current_term, !state.nr_good, !state.nr_bad);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_terms(ಸಂರಚನಾ bisect_terms *terms, ಪೂರ್ಣಾಙ್ಕ file_missing_is_ok)
{
	ಸಂರಚನಾ strbuf str = STRBUF_INIT;
	FILE *fp = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ res = 0;

	fp = fopen(git_path_bisect_terms(), "r");
	ಯದಿ (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		ಗಚ್ಛ finish;
	}

	free_terms(terms);
	ಯದಿ (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		ಗಚ್ಛ finish;
	}
	terms->term_bad = strbuf_detach(&str, ರಿಕ್ತ);
	ಯದಿ (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		FREE_AND_NULL(terms->term_bad);
		ಗಚ್ಛ finish;
	}
	terms->term_good = strbuf_detach(&str, ರಿಕ್ತ);

finish:
	ಯದಿ (fp)
		fclose(fp);
	strbuf_release(&str);
	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ bisect_terms(ಸಂರಚನಾ bisect_terms *terms, ಧ್ರುವ ವರ್ಣ *option)
{
	ಯದಿ (get_terms(terms, 0))
		ಪ್ರತ್ಯಾಗಮ error(_("no terms defined"));

	ಯದಿ (!option) {
		printf(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->term_good, terms->term_bad);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (one_of(option, "--term-good", "--term-old", ರಿಕ್ತ))
		printf("%s\n", terms->term_good);
	ಅನ್ಯಥಾ ಯದಿ (one_of(option, "--term-bad", "--term-new", ರಿಕ್ತ))
		printf("%s\n", terms->term_bad);
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ error(_("invalid argument %s for 'git bisect terms'.\n"
			       "Supported options are: "
			       "--term-good|--term-old and "
			       "--term-bad|--term-new."), option);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ bisect_append_log_quoted(ಧ್ರುವ ವರ್ಣ **argv)
{
	ಪೂರ್ಣಾಙ್ಕ res = 0;
	FILE *fp = fopen(git_path_bisect_log(), "a");
	ಸಂರಚನಾ strbuf orig_args = STRBUF_INIT;

	ಯದಿ (!fp)
		ಪ್ರತ್ಯಾಗಮ -1;

	ಯದಿ (fprintf(fp, "git bisect start") < 1) {
		res = -1;
		ಗಚ್ಛ finish;
	}

	sq_quote_argv(&orig_args, argv);
	ಯದಿ (fprintf(fp, "%s\n", orig_args.buf) < 1)
		res = -1;

finish:
	fclose(fp);
	strbuf_release(&orig_args);
	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add_bisect_ref(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *cb)
{
	ಸಂರಚನಾ add_bisect_ref_data *data = cb;

	add_pending_oid(data->revs, ref->name, ref->oid, data->object_flags);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ prepare_revs(ಸಂರಚನಾ bisect_terms *terms, ಸಂರಚನಾ rev_info *revs)
{
	ಸಂರಚನಾ refs_for_each_ref_options opts = {
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};
	ಪೂರ್ಣಾಙ್ಕ res = 0;
	ಸಂರಚನಾ add_bisect_ref_data cb = { revs };
	ವರ್ಣ *good = xstrfmt("%s-*", terms->term_good);

	/*
	 * We cannot use terms->term_bad directly in
	 * for_each_glob_ref_in() and we have to append a '*' to it,
	 * otherwise for_each_glob_ref_in() will append '/' and '*'.
	 */
	ವರ್ಣ *bad = xstrfmt("%s*", terms->term_bad);

	/*
	 * It is important to reset the flags used by revision walks
	 * as the previous call to bisect_next_all() in turn
	 * sets up a revision walk.
	 */
	reset_revision_walk();
	repo_init_revisions(the_repository, revs, ರಿಕ್ತ);
	setup_revisions(0, ರಿಕ್ತ, revs, ರಿಕ್ತ);

	opts.pattern = bad;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      add_bisect_ref, &cb, &opts);

	cb.object_flags = UNINTERESTING;
	opts.pattern = good;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      add_bisect_ref, &cb, &opts);

	ಯದಿ (prepare_revision_walk(revs))
		res = error(_("revision walk setup failed"));

	ಮುಕ್ತ(good);
	ಮುಕ್ತ(bad);
	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ bisect_skipped_commits(ಸಂರಚನಾ bisect_terms *terms)
{
	ಪೂರ್ಣಾಙ್ಕ res;
	FILE *fp = ರಿಕ್ತ;
	ಸಂರಚನಾ rev_info revs;
	ಸಂರಚನಾ commit *commit;
	ಸಂರಚನಾ pretty_print_context pp = {0};
	ಸಂರಚನಾ strbuf commit_name = STRBUF_INIT;

	res = prepare_revs(terms, &revs);
	ಯದಿ (res)
		ಪ್ರತ್ಯಾಗಮ res;

	fp = fopen(git_path_bisect_log(), "a");
	ಯದಿ (!fp)
		ಪ್ರತ್ಯಾಗಮ error_errno(_("could not open '%s' for appending"),
				  git_path_bisect_log());

	ಯದಿ (fprintf(fp, "# only skipped commits left to test\n") < 0)
		ಪ್ರತ್ಯಾಗಮ error_errno(_("failed to write to '%s'"), git_path_bisect_log());

	ಯಾವತ್ ((commit = get_revision(&revs)) != ರಿಕ್ತ) {
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
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ bisect_successful(ಸಂರಚನಾ bisect_terms *terms)
{
	ಸಂರಚನಾ object_id oid;
	ಸಂರಚನಾ commit *commit;
	ಸಂರಚನಾ pretty_print_context pp = {0};
	ಸಂರಚನಾ strbuf commit_name = STRBUF_INIT;
	ವರ್ಣ *bad_ref = xstrfmt("refs/bisect/%s",terms->term_bad);
	ಪೂರ್ಣಾಙ್ಕ res;

	refs_read_ref(get_main_ref_store(the_repository), bad_ref, &oid);
	commit = lookup_commit_reference_by_name(bad_ref);
	ಯದಿ (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		ಮುಕ್ತ(bad_ref);
		ಪ್ರತ್ಯಾಗಮ BISECT_FAILED;
	}
	repo_format_commit_message(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_to_file(git_path_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->term_bad, oid_to_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	ಮುಕ್ತ(bad_ref);
	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಗಣನಾ bisect_error bisect_next(ಸಂರಚನಾ bisect_terms *terms,
				     ಧ್ರುವ ವರ್ಣ *prefix)
{
	ಗಣನಾ bisect_error res;

	ಯದಿ (bisect_autostart(terms))
		ಪ್ರತ್ಯಾಗಮ BISECT_FAILED;

	ಯದಿ (bisect_next_check(terms, terms->term_good))
		ಪ್ರತ್ಯಾಗಮ BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_next_all(the_repository, prefix);

	ಯದಿ (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		res = bisect_successful(terms);
		ಪ್ರತ್ಯಾಗಮ res ? res : BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND;
	} ಅನ್ಯಥಾ ಯದಿ (res == BISECT_ONLY_SKIPPED_ಎಡ) {
		res = bisect_skipped_commits(terms);
		ಪ್ರತ್ಯಾಗಮ res ? res : BISECT_ONLY_SKIPPED_ಎಡ;
	}
	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಗಣನಾ bisect_error bisect_auto_next(ಸಂರಚನಾ bisect_terms *terms,
					  ಧ್ರುವ ವರ್ಣ *prefix)
{
	ಯದಿ (bisect_next_check(terms, ರಿಕ್ತ)) {
		bisect_print_status(terms);
		ಪ್ರತ್ಯಾಗಮ BISECT_OK;
	}

	ಪ್ರತ್ಯಾಗಮ bisect_next(terms, prefix);
}

ಸ್ಥಿರ ಗಣನಾ bisect_error bisect_start(ಸಂರಚನಾ bisect_terms *terms, ಪೂರ್ಣಾಙ್ಕ argc,
				      ಧ್ರುವ ವರ್ಣ **argv)
{
	ಪೂರ್ಣಾಙ್ಕ no_checkout = 0;
	ಪೂರ್ಣಾಙ್ಕ first_parent_only = 0;
	ಪೂರ್ಣಾಙ್ಕ i, has_double_dash = 0, must_write_terms = 0, bad_seen = 0;
	ಪೂರ್ಣಾಙ್ಕ flags, pathspec_pos;
	ಗಣನಾ bisect_error res = BISECT_OK;
	ಸಂರಚನಾ string_list revs = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ string_list states = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ strbuf start_head = STRBUF_INIT;
	ಸಂರಚನಾ strbuf bisect_names = STRBUF_INIT;
	ಸಂರಚನಾ object_id head_oid;
	ಸಂರಚನಾ object_id oid;
	ಗಣನಾ reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;
	ಧ್ರುವ ವರ್ಣ *head;

	ಯದಿ (is_bare_repository(the_repository))
		no_checkout = 1;

	/*
	 * Check for one bad and then some good revisions
	 */
	ಪ್ರತಿ (i = 0; i < argc; i++) {
		ಯದಿ (!strcmp(argv[i], "--")) {
			has_double_dash = 1;
			ಭಙ್ಗ;
		}
	}

	ಪ್ರತಿ (i = 0; i < argc; i++) {
		ಧ್ರುವ ವರ್ಣ *arg = argv[i];
		ಯದಿ (!strcmp(argv[i], "--")) {
			ಭಙ್ಗ;
		} ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "--no-checkout")) {
			no_checkout = 1;
		} ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "--first-parent")) {
			first_parent_only = 1;
		} ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "--reset-when-found")) {
			reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
		} ಅನ್ಯಥಾ ಯದಿ (skip_prefix(arg, "--reset-when-found=", &arg)) {
			ಯದಿ (parse_reset_when_found(arg, &reset_when_found)) {
				res = BISECT_FAILED;
				ಗಚ್ಛ finish;
			}
		} ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "--term-good") ||
			 !strcmp(arg, "--term-old")) {
			i++;
			ಯದಿ (argc <= i)
				ಪ್ರತ್ಯಾಗಮ error(_("'' is not a valid term"));
			must_write_terms = 1;
			ಮುಕ್ತ((ಶೂನ್ಯ *) terms->term_good);
			terms->term_good = xstrdup(argv[i]);
		} ಅನ್ಯಥಾ ಯದಿ (skip_prefix(arg, "--term-good=", &arg) ||
			   skip_prefix(arg, "--term-old=", &arg)) {
			must_write_terms = 1;
			ಮುಕ್ತ((ಶೂನ್ಯ *) terms->term_good);
			terms->term_good = xstrdup(arg);
		} ಅನ್ಯಥಾ ಯದಿ (!strcmp(arg, "--term-bad") ||
			 !strcmp(arg, "--term-new")) {
			i++;
			ಯದಿ (argc <= i)
				ಪ್ರತ್ಯಾಗಮ error(_("'' is not a valid term"));
			must_write_terms = 1;
			ಮುಕ್ತ((ಶೂನ್ಯ *) terms->term_bad);
			terms->term_bad = xstrdup(argv[i]);
		} ಅನ್ಯಥಾ ಯದಿ (skip_prefix(arg, "--term-bad=", &arg) ||
			   skip_prefix(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			ಮುಕ್ತ((ಶೂನ್ಯ *) terms->term_bad);
			terms->term_bad = xstrdup(arg);
		} ಅನ್ಯಥಾ ಯದಿ (starts_with(arg, "--")) {
			ಪ್ರತ್ಯಾಗಮ error(_("unrecognized option: '%s'"), arg);
		} ಅನ್ಯಥಾ ಯದಿ (!get_oidf(&oid, "%s^{commit}", arg)) {
			string_list_append(&revs, oid_to_hex(&oid));
		} ಅನ್ಯಥಾ ಯದಿ (has_double_dash) {
			die(_("'%s' does not appear to be a valid "
			      "revision"), arg);
		} ಅನ್ಯಥಾ {
			ಭಙ್ಗ;
		}
	}
	ಯದಿ (reset_when_found != RESET_WHEN_FOUND_NONE && no_checkout) {
		res = error(_("options '%s' and '%s' cannot be used together"),
			    "--reset-when-found", "--no-checkout");
		ಗಚ್ಛ finish;
	}
	pathspec_pos = i;

	/*
	 * The user ran "git bisect start <sha1> <sha1>", hence did not
	 * explicitly specify the terms, but we are already starting to
	 * set references named with the default terms, and won't be able
	 * to change afterwards.
	 */
	ಯದಿ (revs.nr)
		must_write_terms = 1;
	ಪ್ರತಿ (i = 0; i < revs.nr; i++) {
		ಯದಿ (bad_seen) {
			string_list_append(&states, terms->term_good);
		} ಅನ್ಯಥಾ {
			bad_seen = 1;
			string_list_append(&states, terms->term_bad);
		}
	}

	/*
	 * Verify HEAD
	 */
	head = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
				       "HEAD", 0, &head_oid, &flags);
	ಯದಿ (!head) {
		ಯದಿ (repo_get_oid(the_repository, "HEAD", &head_oid))
			ಪ್ರತ್ಯಾಗಮ error(_("bad HEAD - I need a HEAD"));
		head = "HEAD";
	}

	/*
	 * Check if we are bisecting
	 */
	ಯದಿ (!is_empty_or_missing_file(git_path_bisect_start())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&start_head, git_path_bisect_start(), 0);
		strbuf_trim(&start_head);
		ಯದಿ (!no_checkout) {
			ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.args, "checkout", start_head.buf,
				     "--", ರಿಕ್ತ);
			ಯದಿ (run_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       start_head.buf);
				ಗಚ್ಛ finish;
			}
		}
	} ಅನ್ಯಥಾ {
		/* Get the rev from where we start. */
		ಯದಿ (!repo_get_oid(the_repository, head, &head_oid) &&
		    !starts_with(head, "refs/heads/")) {
			strbuf_reset(&start_head);
			strbuf_add_oid_hex(&start_head, &head_oid);
		} ಅನ್ಯಥಾ ಯದಿ (!repo_get_oid(the_repository, head, &head_oid) &&
			   skip_prefix(head, "refs/heads/", &head)) {
			strbuf_addstr(&start_head, head);
		} ಅನ್ಯಥಾ {
			ಪ್ರತ್ಯಾಗಮ error(_("bad HEAD - strange symbolic ref"));
		}
	}

	/*
	 * Get rid of any old bisect state.
	 */
	ಯದಿ (bisect_clean_state())
		ಪ್ರತ್ಯಾಗಮ BISECT_FAILED;

	/*
	 * Write new start state
	 */
	write_file(git_path_bisect_start(), "%s\n", start_head.buf);

	ಯದಿ (first_parent_only)
		write_file(git_path_bisect_first_parent(), "\n");

	ಯದಿ (reset_when_found != RESET_WHEN_FOUND_NONE)
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));

	ಯದಿ (no_checkout) {
		ಯದಿ (repo_get_oid(the_repository, start_head.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), start_head.buf);
			ಗಚ್ಛ finish;
		}
		ಯದಿ (refs_update_ref(get_main_ref_store(the_repository), ರಿಕ್ತ, "BISECT_HEAD", &oid, ರಿಕ್ತ, 0,
				    UPDATE_REFS_ಸಂದೇಶ_ON_ERR)) {
			res = BISECT_FAILED;
			ಗಚ್ಛ finish;
		}
	}

	ಯದಿ (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_names, argv + pathspec_pos);
	write_file(git_path_bisect_names(), "%s\n", bisect_names.buf);

	ಪ್ರತಿ (i = 0; i < states.nr; i++)
		ಯದಿ (bisect_write(states.ವಸ್ತುಗಳು[i].string,
				 revs.ವಸ್ತುಗಳು[i].string, terms, 1)) {
			res = BISECT_FAILED;
			ಗಚ್ಛ finish;
		}

	ಯದಿ (must_write_terms && write_terms(terms->term_bad,
					    terms->term_good)) {
		res = BISECT_FAILED;
		ಗಚ್ಛ finish;
	}

	res = bisect_append_log_quoted(argv);
	ಯದಿ (res)
		res = BISECT_FAILED;

finish:
	string_list_clear(&revs, 0);
	string_list_clear(&states, 0);
	strbuf_release(&start_head);
	strbuf_release(&bisect_names);
	ಯದಿ (res)
		ಪ್ರತ್ಯಾಗಮ res;

	res = bisect_auto_next(terms, ರಿಕ್ತ);
	ಯದಿ (!is_bisect_success(res))
		bisect_clean_state();
	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ file_is_not_empty(ಧ್ರುವ ವರ್ಣ *path)
{
	ಪ್ರತ್ಯಾಗಮ !is_empty_or_missing_file(path);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ bisect_autostart(ಸಂರಚನಾ bisect_terms *terms)
{
	ಪೂರ್ಣಾಙ್ಕ res;
	ಧ್ರುವ ವರ್ಣ *yesno;

	ಯದಿ (file_is_not_empty(git_path_bisect_start()))
		ಪ್ರತ್ಯಾಗಮ 0;

	fprintf_ln(stderr, _("You need to start by \"git bisect "
			  "start\"\n"));

	ಯದಿ (!isatty(STDIN_FILENO))
		ಪ್ರತ್ಯಾಗಮ -1;

	/*
	 * TRANSLATORS: Make sure to include [Y] and [n] in your
	 * translation. The program will only accept English input
	 * at this point.
	 */
	yesno = git_prompt(_("Do you want me to do it for you "
			     "[Y/n]? "), PROMPT_ECHO);
	res = tolower(*yesno) == 'n' ?
		-1 : bisect_start(terms, 0, empty_strvec);

	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಗಣನಾ bisect_error bisect_state(ಸಂರಚನಾ bisect_terms *terms, ಪೂರ್ಣಾಙ್ಕ argc,
				      ಧ್ರುವ ವರ್ಣ **argv)
{
	ಧ್ರುವ ವರ್ಣ *state;
	ಪೂರ್ಣಾಙ್ಕ i, verify_expected = 1;
	ಸಂರಚನಾ object_id oid, expected;
	ಸಂರಚನಾ oid_array revs = OID_ARRAY_INIT;

	ಯದಿ (!argc)
		ಪ್ರತ್ಯಾಗಮ error(_("Please call `--bisect-state` with at least one argument"));

	ಯದಿ (bisect_autostart(terms))
		ಪ್ರತ್ಯಾಗಮ BISECT_FAILED;

	state = argv[0];
	ಯದಿ (check_and_set_terms(terms, state) ||
	    !one_of(state, terms->term_good, terms->term_bad, "skip", ರಿಕ್ತ))
		ಪ್ರತ್ಯಾಗಮ BISECT_FAILED;

	argv++;
	argc--;
	ಯದಿ (argc > 1 && !strcmp(state, terms->term_bad))
		ಪ್ರತ್ಯಾಗಮ error(_("'git bisect %s' can take only one argument."), terms->term_bad);

	ಯದಿ (argc == 0) {
		ಧ್ರುವ ವರ್ಣ *head = "BISECT_HEAD";
		ಗಣನಾ get_oid_result res_head = repo_get_oid(the_repository,
							    head, &oid);

		ಯದಿ (res_head == MISSING_OBJECT) {
			head = "HEAD";
			res_head = repo_get_oid(the_repository, head, &oid);
		}

		ಯದಿ (res_head)
			error(_("Bad rev input: %s"), head);
		oid_array_append(&revs, &oid);
	}

	/*
	 * All input revs must be checked before executing bisect_write()
	 * to discard junk revs.
	 */

	ಪ್ರತಿ (; argc; argc--, argv++) {
		ಸಂರಚನಾ commit *commit;

		ಯದಿ (repo_get_oid(the_repository, *argv, &oid)){
			error(_("Bad rev input: %s"), *argv);
			oid_array_clear(&revs);
			ಪ್ರತ್ಯಾಗಮ BISECT_FAILED;
		}

		commit = lookup_commit_reference(the_repository, &oid);
		ಯದಿ (!commit)
			die(_("Bad rev input (not a commit): %s"), *argv);

		oid_array_append(&revs, &commit->object.oid);
	}

	ಯದಿ (refs_read_ref(get_main_ref_store(the_repository), "BISECT_EXPECTED_REV", &expected))
		verify_expected = 0; /* Ignore invalid file contents */

	ಪ್ರತಿ (i = 0; i < revs.nr; i++) {
		ಯದಿ (bisect_write(state, oid_to_hex(&revs.oid[i]), terms, 0)) {
			oid_array_clear(&revs);
			ಪ್ರತ್ಯಾಗಮ BISECT_FAILED;
		}
		ಯದಿ (verify_expected && !oideq(&revs.oid[i], &expected)) {
			unlink_or_warn(git_path_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					ರಿಕ್ತ, "BISECT_EXPECTED_REV", ರಿಕ್ತ,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	ಪ್ರತ್ಯಾಗಮ bisect_auto_next(terms, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಗಣನಾ bisect_error bisect_log(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಙ್ಕ fd, status;
	ಧ್ರುವ ವರ್ಣ* filename = git_path_bisect_log();

	ಯದಿ (is_empty_or_missing_file(filename))
		ಪ್ರತ್ಯಾಗಮ error(_("We are not bisecting."));

	fd = open(filename, O_RDONLY);
	ಯದಿ (fd < 0)
		ಪ್ರತ್ಯಾಗಮ BISECT_FAILED;

	status = copy_fd(fd, STDOUT_FILENO);
	close(fd);
	ಪ್ರತ್ಯಾಗಮ status ? BISECT_FAILED : BISECT_OK;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ process_replay_ಸಾಲು(ಸಂರಚನಾ bisect_terms *terms, ಸಂರಚನಾ strbuf *ಸಾಲು)
{
	ಧ್ರುವ ವರ್ಣ *p = ಸಾಲು->buf + strspn(ಸಾಲು->buf, " \t");
	ವರ್ಣ *word_end, *rev;

	ಯದಿ ((!skip_prefix(p, "git bisect", &p) &&
	!skip_prefix(p, "git-bisect", &p)) || !isspace(*p))
		ಪ್ರತ್ಯಾಗಮ 0;
	p += strspn(p, " \t");

	word_end = (ವರ್ಣ *)p + strcspn(p, " \t");
	rev = word_end + strspn(word_end, " \t");
	*word_end = '\0'; /* NUL-terminate the word */

	ಯದಿ (get_terms(terms, 1))
		ಪ್ರತ್ಯಾಗಮ error(_("no terms defined"));
	ಯದಿ (check_and_set_terms(terms, p))
		ಪ್ರತ್ಯಾಗಮ -1;

	ಯದಿ (!strcmp(p, "start")) {
		ಸಂರಚನಾ strvec argv = STRVEC_INIT;
		ಪೂರ್ಣಾಙ್ಕ res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_start(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		ಪ್ರತ್ಯಾಗಮ res;
	}

	ಯದಿ (one_of(p, terms->term_good,
	   terms->term_bad, "skip", ರಿಕ್ತ))
		ಪ್ರತ್ಯಾಗಮ bisect_write(p, rev, terms, 0);

	ಯದಿ (!strcmp(p, "terms")) {
		ಸಂರಚನಾ strvec argv = STRVEC_INIT;
		ಪೂರ್ಣಾಙ್ಕ res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_terms(terms, argv.nr == 1 ? argv.v[0] : ರಿಕ್ತ);
		strvec_clear(&argv);
		ಪ್ರತ್ಯಾಗಮ res;
	}
	error(_("'%s'?? what are you talking about?"), p);

	ಪ್ರತ್ಯಾಗಮ -1;
}

ಸ್ಥಿರ ಗಣನಾ bisect_error bisect_replay(ಸಂರಚನಾ bisect_terms *terms, ಧ್ರುವ ವರ್ಣ *filename)
{
	FILE *fp = ರಿಕ್ತ;
	ಗಣನಾ bisect_error res = BISECT_OK;
	ಸಂರಚನಾ strbuf ಸಾಲು = STRBUF_INIT;

	ಯದಿ (is_empty_or_missing_file(filename))
		ಪ್ರತ್ಯಾಗಮ error(_("cannot read file '%s' for replaying"), filename);

	ಯದಿ (bisect_clean_state())
		ಪ್ರತ್ಯಾಗಮ BISECT_FAILED;

	fp = fopen(filename, "r");
	ಯದಿ (!fp)
		ಪ್ರತ್ಯಾಗಮ BISECT_FAILED;

	ಯಾವತ್ ((strbuf_getline(&ಸಾಲು, fp) != EOF) && !res)
		res = process_replay_ಸಾಲು(terms, &ಸಾಲು);

	strbuf_release(&ಸಾಲು);
	fclose(fp);

	ಯದಿ (res)
		ಪ್ರತ್ಯಾಗಮ BISECT_FAILED;

	ಪ್ರತ್ಯಾಗಮ bisect_auto_next(terms, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಗಣನಾ bisect_error bisect_skip(ಸಂರಚನಾ bisect_terms *terms, ಪೂರ್ಣಾಙ್ಕ argc,
				     ಧ್ರುವ ವರ್ಣ **argv)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಗಣನಾ bisect_error res;
	ಸಂರಚನಾ strvec argv_state = STRVEC_INIT;

	strvec_ತಳ್ಳು(&argv_state, "skip");

	ಪ್ರತಿ (i = 0; i < argc; i++) {
		ಧ್ರುವ ವರ್ಣ *dotdot = strstr(argv[i], "..");

		ಯದಿ (dotdot) {
			ಸಂರಚನಾ rev_info revs;
			ಸಂರಚನಾ commit *commit;

			repo_init_revisions(the_repository, &revs, ರಿಕ್ತ);
			setup_revisions(2, argv + i - 1, &revs, ರಿಕ್ತ);

			ಯದಿ (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			ಯಾವತ್ ((commit = get_revision(&revs)) != ರಿಕ್ತ)
				strvec_ತಳ್ಳು(&argv_state,
						oid_to_hex(&commit->object.oid));

			reset_revision_walk();
			release_revisions(&revs);
		} ಅನ್ಯಥಾ {
			strvec_ತಳ್ಳು(&argv_state, argv[i]);
		}
	}
	res = bisect_state(terms, argv_state.nr, argv_state.v);

	strvec_clear(&argv_state);
	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ bisect_visualize(ಸಂರಚನಾ bisect_terms *terms, ಪೂರ್ಣಾಙ್ಕ argc,
			    ಧ್ರುವ ವರ್ಣ **argv)
{
	ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ strbuf sb = STRBUF_INIT;

	ಯದಿ (bisect_next_check(terms, ರಿಕ್ತ) != 0)
		ಪ್ರತ್ಯಾಗಮ BISECT_FAILED;

	cmd.no_stdin = 1;
	ಯದಿ (!argc) {
		ಯದಿ ((getenv("DISPLAY") || getenv("SESSIONNAME") || getenv("MSYSTEM") ||
		     getenv("SECURITYSESSIONID")) && exists_in_PATH("gitk")) {
			strvec_ತಳ್ಳು(&cmd.args, "gitk");
		} ಅನ್ಯಥಾ {
			strvec_ತಳ್ಳು(&cmd.args, "log");
			cmd.git_cmd = 1;
		}
	} ಅನ್ಯಥಾ {
		ಯದಿ (argv[0][0] == '-') {
			strvec_ತಳ್ಳು(&cmd.args, "log");
			cmd.git_cmd = 1;
		} ಅನ್ಯಥಾ ಯದಿ (strcmp(argv[0], "tig") && !starts_with(argv[0], "git"))
			cmd.git_cmd = 1;

		strvec_pushv(&cmd.args, argv);
	}

	strvec_pushl(&cmd.args, "--bisect", "--", ರಿಕ್ತ);

	strbuf_read_file(&sb, git_path_bisect_names(), 0);
	sq_dequote_to_strvec(sb.buf, &cmd.args);
	strbuf_release(&sb);

	ಪ್ರತ್ಯಾಗಮ run_command(&cmd);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_first_good(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *cb_data)
{
	oidcpy(cb_data, ref->oid);
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ do_bisect_run(ಧ್ರುವ ವರ್ಣ *command)
{
	ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;

	printf(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_ತಳ್ಳು(&cmd.args, command);
	ಪ್ರತ್ಯಾಗಮ run_command(&cmd);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ verify_good(ಧ್ರುವ ಸಂರಚನಾ bisect_terms *terms, ಧ್ರುವ ವರ್ಣ *command)
{
	ಪೂರ್ಣಾಙ್ಕ rc;
	ಗಣನಾ bisect_error res;
	ಸಂರಚನಾ object_id good_rev;
	ಸಂರಚನಾ object_id current_rev;
	ವರ್ಣ *good_glob = xstrfmt("%s-*", terms->term_good);
	ಪೂರ್ಣಾಙ್ಕ no_checkout = refs_ref_exists(get_main_ref_store(the_repository),
					  "BISECT_HEAD");
	ಸಂರಚನಾ refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_first_good, &good_rev, &opts);
	ಮುಕ್ತ(good_glob);

	ಯದಿ (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		ಪ್ರತ್ಯಾಗಮ -1;

	res = bisect_checkout(&good_rev, no_checkout);
	ಯದಿ (res != BISECT_OK)
		ಪ್ರತ್ಯಾಗಮ -1;

	rc = do_bisect_run(command);

	res = bisect_checkout(&current_rev, no_checkout);
	ಯದಿ (res != BISECT_OK)
		ಪ್ರತ್ಯಾಗಮ -1;

	ಪ್ರತ್ಯಾಗಮ rc;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ bisect_run(ಸಂರಚನಾ bisect_terms *terms, ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv)
{
	ಪೂರ್ಣಾಙ್ಕ res = BISECT_OK;
	ಸಂರಚನಾ strbuf command = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *reset_when_found_arg;
	ಧ್ರುವ ವರ್ಣ *new_state;
	ಪೂರ್ಣಾಙ್ಕ temporary_stdout_fd, saved_stdout;
	ಪೂರ್ಣಾಙ್ಕ is_first_run = 1;
	ಗಣನಾ reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;

	ಯದಿ (bisect_next_check(terms, ರಿಕ್ತ))
		ಪ್ರತ್ಯಾಗಮ BISECT_FAILED;

	ಯದಿ (argc && !strcmp(argv[0], "--reset-when-found")) {
		reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
	} ಅನ್ಯಥಾ ಯದಿ (argc && skip_prefix(argv[0], "--reset-when-found=",
				    &reset_when_found_arg)) {
		ಯದಿ (parse_reset_when_found(reset_when_found_arg,
					   &reset_when_found))
			ಪ್ರತ್ಯಾಗಮ BISECT_FAILED;
	}

	ಯದಿ (reset_when_found != RESET_WHEN_FOUND_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		ಪ್ರತ್ಯಾಗಮ error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	ಯದಿ (reset_when_found != RESET_WHEN_FOUND_NONE) {
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));
		argc--;
		argv++;
	}

	ಯದಿ (!argc) {
		error(_("bisect run failed: no command provided."));
		ಪ್ರತ್ಯಾಗಮ BISECT_FAILED;
	}

	sq_quote_argv(&command, argv);
	strbuf_ltrim(&command);
	ಯಾವತ್ (1) {
		res = do_bisect_run(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		ಯದಿ (is_first_run && (res == 126 || res == 127)) {
			ಪೂರ್ಣಾಙ್ಕ rc = verify_good(terms, command.buf);
			is_first_run = 0;
			ಯದಿ (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->term_good);
				res = BISECT_FAILED;
				ಭಙ್ಗ;
			}
			ಯದಿ (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->term_good);
				res = BISECT_FAILED;
				ಭಙ್ಗ;
			}
		}

		ಯದಿ (res < 0 || 128 <= res) {
			error(_("bisect run failed: exit code %d from"
				" %s is < 0 or >= 128"), res, command.buf);
			ಭಙ್ಗ;
		}

		ಯದಿ (res == 125)
			new_state = "skip";
		ಅನ್ಯಥಾ ಯದಿ (!res)
			new_state = terms->term_good;
		ಅನ್ಯಥಾ
			new_state = terms->term_bad;

		temporary_stdout_fd = open(git_path_bisect_run(), O_CREAT | O_WRONLY | O_TRUNC, 0666);

		ಯದಿ (temporary_stdout_fd < 0) {
			res = error_errno(_("cannot open file '%s' for writing"), git_path_bisect_run());
			ಭಙ್ಗ;
		}

		fflush(stdout);
		saved_stdout = dup(1);
		ಯದಿ (saved_stdout < 0 ||
		    dup2(temporary_stdout_fd, 1) < 0) {
			res = error_errno(_("could not duplicate stdout"));
			ಯದಿ (saved_stdout >= 0)
				close(saved_stdout);
			close(temporary_stdout_fd);
			ಭಙ್ಗ;
		}

		res = bisect_state(terms, 1, &new_state);

		fflush(stdout);
		dup2(saved_stdout, 1);
		close(saved_stdout);
		close(temporary_stdout_fd);

		print_file_to_stdout(git_path_bisect_run());

		ಯದಿ (res == BISECT_ONLY_SKIPPED_ಎಡ)
			error(_("bisect run cannot continue any more"));
		ಅನ್ಯಥಾ ಯದಿ (res == BISECT_INTERNAL_SUCCESS_MERGE_BASE) {
			puts(_("bisect run success"));
			res = BISECT_OK;
		} ಅನ್ಯಥಾ ಯದಿ (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
			printf(_("bisect found first '%s' commit\n"), terms->term_bad);
		} ಅನ್ಯಥಾ ಯದಿ (res) {
			error(_("bisect run failed: 'git bisect %s'"
				" exited with error code %d"), new_state, res);
		} ಅನ್ಯಥಾ {
			ಅನುವರ್ತ;
		}
		ಭಙ್ಗ;
	}

	strbuf_release(&command);
	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_bisect__reset(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix UNUSED,
			     ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ res;

	ಯದಿ (argc > 1)
		ಪ್ರತ್ಯಾಗಮ error(_("'%s' requires either no argument or a commit"),
			     "git bisect reset");
	res = bisect_reset(argc ? argv[0] : ರಿಕ್ತ, false);
	ಯದಿ (res)
		ಪ್ರತ್ಯಾಗಮ res;
	ಪ್ರತ್ಯಾಗಮ bisect_clean_state();
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_bisect__terms(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix UNUSED,
			     ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ res;
	ಸಂರಚನಾ bisect_terms terms = { 0 };

	ಯದಿ (argc > 1)
		ಪ್ರತ್ಯಾಗಮ error(_("'%s' requires 0 or 1 argument"),
			     "git bisect terms");
	res = bisect_terms(&terms, argc == 1 ? argv[0] : ರಿಕ್ತ);
	free_terms(&terms);
	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_bisect__start(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix UNUSED,
			     ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ res;
	ಸಂರಚನಾ bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	res = bisect_start(&terms, argc, argv);
	free_terms(&terms);
	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_bisect__next(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv UNUSED, ಧ್ರುವ ವರ್ಣ *prefix,
			    ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ res;
	ಸಂರಚನಾ bisect_terms terms = { 0 };

	ಯದಿ (argc)
		ಪ್ರತ್ಯಾಗಮ error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	ಯದಿ (get_terms(&terms, 1))
		ಪ್ರತ್ಯಾಗಮ error(_("no terms defined"));
	res = bisect_next(&terms, prefix);
	free_terms(&terms);
	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_bisect__log(ಪೂರ್ಣಾಙ್ಕ argc UNUSED, ಧ್ರುವ ವರ್ಣ **argv UNUSED,
			   ಧ್ರುವ ವರ್ಣ *prefix UNUSED,
			   ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪ್ರತ್ಯಾಗಮ bisect_log();
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_bisect__replay(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix UNUSED,
			      ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ res;
	ಸಂರಚನಾ bisect_terms terms = { 0 };

	ಯದಿ (argc != 1)
		ಪ್ರತ್ಯಾಗಮ error(_("no logfile given"));
	set_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_bisect__skip(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix UNUSED,
			    ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ res;
	ಸಂರಚನಾ bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	ಯದಿ (get_terms(&terms, 1))
		ಪ್ರತ್ಯಾಗಮ error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_bisect__visualize(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix UNUSED,
				 ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ res;
	ಸಂರಚನಾ bisect_terms terms = { 0 };

	ಯದಿ (get_terms(&terms, 1))
		ಪ್ರತ್ಯಾಗಮ error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_bisect__run(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix UNUSED,
			   ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ res;
	ಸಂರಚನಾ bisect_terms terms = { 0 };

	ಯದಿ (!argc)
		ಪ್ರತ್ಯಾಗಮ error(_("'%s' failed: no command provided."), "git bisect run");
	ಯದಿ (get_terms(&terms, 1))
		ಪ್ರತ್ಯಾಗಮ error(_("no terms defined"));
	res = bisect_run(&terms, argc, argv);
	free_terms(&terms);
	ಪ್ರತ್ಯಾಗಮ res;
}

ಪೂರ್ಣಾಙ್ಕ cmd_bisect(ಪೂರ್ಣಾಙ್ಕ argc,
	       ಧ್ರುವ ವರ್ಣ **argv,
	       ಧ್ರುವ ವರ್ಣ *prefix,
	       ಸಂರಚನಾ repository *repo)
{
	ಪೂರ್ಣಾಙ್ಕ res = 0;
	parse_opt_subcommand_fn *fn = ರಿಕ್ತ;
	ಸಂರಚನಾ option options[] = {
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

	ಯದಿ (!fn) {
		ಸಂರಚನಾ bisect_terms terms = { 0 };

		ಯದಿ (!argc)
			usage_ಸಂದೇಶ_opt(_("need a command"), git_bisect_usage, options);

		ಯದಿ (!strcmp(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		set_terms(&terms, "bad", "good");
		ಯದಿ (get_terms(&terms, 1))
			ಪ್ರತ್ಯಾಗಮ error(_("no terms defined"));
		ಯದಿ (check_and_set_terms(&terms, argv[0]) ||
		    !one_of(argv[0], terms.term_good, terms.term_bad, ರಿಕ್ತ))
			usage_ಸಂದೇಶ_optf(_("unknown command: '%s'"), git_bisect_usage,
				       options, argv[0]);
		ಅನ್ಯಥಾ
			res = bisect_state(&terms, argc, argv);
		free_terms(&terms);
	} ಅನ್ಯಥಾ {
		argc--;
		argv++;
		res = fn(argc, argv, prefix, repo);
	}

	ಯದಿ (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		ಗಣನಾ reset_when_found_mode mode;

		ಯದಿ (read_reset_when_found(&mode))
			res = BISECT_FAILED;
		ಅನ್ಯಥಾ ಯದಿ (mode != RESET_WHEN_FOUND_NONE &&
			 bisect_reset_when_found(mode))
			res = BISECT_FAILED;
	}

	ಪ್ರತ್ಯಾಗಮ is_bisect_success(res) ? 0 : -res;
}
