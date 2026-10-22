#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

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

ಸ್ಥಿರ GIT_ಮಾರ್ಗ_FUNC(git_ಮಾರ್ಗ_bisect_terms, "BISECT_TERMS")
ಸ್ಥಿರ GIT_ಮಾರ್ಗ_FUNC(git_ಮಾರ್ಗ_bisect_ancestors_ok, "BISECT_ANCESTORS_OK")
ಸ್ಥಿರ GIT_ಮಾರ್ಗ_FUNC(git_ಮಾರ್ಗ_bisect_ಆರಂಭ, "BISECT_START")
ಸ್ಥಿರ GIT_ಮಾರ್ಗ_FUNC(git_ಮಾರ್ಗ_bisect_log, "BISECT_LOG")
ಸ್ಥಿರ GIT_ಮಾರ್ಗ_FUNC(git_ಮಾರ್ಗ_bisect_ಹೆಸರುಗಳು, "BISECT_NAMES")
ಸ್ಥಿರ GIT_ಮಾರ್ಗ_FUNC(git_ಮಾರ್ಗ_bisect_ಮೊದಲ_ಪೋಷಕ, "BISECT_FIRST_PARENT")
ಸ್ಥಿರ GIT_ಮಾರ್ಗ_FUNC(git_ಮಾರ್ಗ_bisect_reset_when_ಸಿಕ್ಕಿತು, "BISECT_RESET_WHEN_FOUND")
ಸ್ಥಿರ GIT_ಮಾರ್ಗ_FUNC(git_ಮಾರ್ಗ_bisect_ಸತತ, "BISECT_RUN")

#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_GIT_BISECT_ಆರಂಭ_USAGE \
	N_("git bisect start [--term-(bad|new)=<term-new> --term-(good|old)=<term-old>]\n" \
	   "                 [--no-checkout] [--first-parent] [--reset-when-found[=<where>]] [<bad> [<good>...]] [--] [<pathspec>...]")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_GIT_BISECT_BAD_USAGE \
	N_("git bisect (bad|new|<term-new>) [<rev>]")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_GIT_BISECT_GOOD_USAGE \
	N_("git bisect (good|old|<term-old>) [<rev>...]")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_GIT_BISECT_TERMS_USAGE \
	"git bisect terms [--term-(good|old) | --term-(bad|new)]"
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_GIT_BISECT_SKIP_USAGE \
	N_("git bisect skip [(<rev>|<range>)...]")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_GIT_BISECT_ಮುಂದಿನ_USAGE \
	"git bisect next"
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_GIT_BISECT_RESET_USAGE \
	N_("git bisect reset [<commit>]")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_GIT_BISECT_VISUALIZE_USAGE \
	"git bisect (visualize|view)"
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_GIT_BISECT_REPLAY_USAGE \
	N_("git bisect replay <logfile>")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_GIT_BISECT_LOG_USAGE \
	"git bisect log"
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_GIT_BISECT_ಸತತ_USAGE \
	N_("git bisect run [--reset-when-found[=<where>]] <cmd> [<arg>...]")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_GIT_BISECT_HELP_USAGE \
	"git bisect help"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_bisect_usage[] = {
	BUILTIN_GIT_BISECT_ಆರಂಭ_USAGE,
	BUILTIN_GIT_BISECT_BAD_USAGE,
	BUILTIN_GIT_BISECT_GOOD_USAGE,
	BUILTIN_GIT_BISECT_TERMS_USAGE,
	BUILTIN_GIT_BISECT_SKIP_USAGE,
	BUILTIN_GIT_BISECT_ಮುಂದಿನ_USAGE,
	BUILTIN_GIT_BISECT_RESET_USAGE,
	BUILTIN_GIT_BISECT_VISUALIZE_USAGE,
	BUILTIN_GIT_BISECT_REPLAY_USAGE,
	BUILTIN_GIT_BISECT_LOG_USAGE,
	BUILTIN_GIT_BISECT_ಸತತ_USAGE,
	BUILTIN_GIT_BISECT_HELP_USAGE,
	ಖಾಲಿ
};

ಎಣಿಕೆ reset_when_ಸಿಕ್ಕಿತು_ಬಹುಲಕ {
	RESET_WHEN_ಸಿಕ್ಕಿತು_NONE,
	RESET_WHEN_ಸಿಕ್ಕಿತು_ಗಮ್ಯ_ORIGINAL,
	RESET_WHEN_ಸಿಕ್ಕಿತು_ಗಮ್ಯ_ಸಿಕ್ಕಿತು,
};

ರಚನೆ ಕೂಡಿಸು_bisect_ref_ದತ್ತಾಂಶ {
	ರಚನೆ rev_info *revs;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ object_flags;
};

ರಚನೆ bisect_terms {
	ಅಕ್ಷರ *ಪದ_good;
	ಅಕ್ಷರ *ಪದ_bad;
};

ಸ್ಥಿರ ಶೂನ್ಯ free_terms(ರಚನೆ bisect_terms *terms)
{
	FREE_AND_NULL(terms->ಪದ_good);
	FREE_AND_NULL(terms->ಪದ_bad);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_terms(ರಚನೆ bisect_terms *terms, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *bad,
		      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *good)
{
	ಬಿಡುಗಡೆ((ಶೂನ್ಯ *)terms->ಪದ_good);
	terms->ಪದ_good = xstrdup(good);
	ಬಿಡುಗಡೆ((ಶೂನ್ಯ *)terms->ಪದ_bad);
	terms->ಪದ_bad = xstrdup(bad);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ vocab_bad[] = "bad|new";
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ vocab_good[] = "good|old";

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bisect_autostart(ರಚನೆ bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
ಕೊನೆಯ_ARG_MUST_BE_NULL
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ one_ದ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪದ, ...)
{
	ಪೂರ್ಣಾಂಕ res = 0;
	va_list matches;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಹೊಂದಿಕೆ;

	va_ಆರಂಭ(matches, ಪದ);
	ಆಗ (!res && (ಹೊಂದಿಕೆ = va_arg(matches, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *)))
		res = !strcmp(ಪದ, ಹೊಂದಿಕೆ);
	va_ಕೊನೆ(matches);

	ಹಿಂತಿರುಗಿ res;
}

/*
 * return code BISECT_INTERNAL_SUCCESS_MERGE_BASE
 * and BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND are codes
 * that indicate special success.
 */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ is_bisect_success(ಎಣಿಕೆ bisect_error res)
{
	ಹಿಂತಿರುಗಿ !res ||
		res == BISECT_ಆಂತರಿಕ_SUCCESS_1_ST_BAD_ಸಿಕ್ಕಿತು ||
		res == BISECT_ಆಂತರಿಕ_SUCCESS_ವಿಲೀನ_ತಳ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ write_ಒಳ_file(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಹುಲಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format, va_list ವಾದಗಳು)
{
	FILE *fp = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ res = 0;

	ಆದರೆ (strcmp(ಬಹುಲಕ, "w") && strcmp(ಬಹುಲಕ, "a"))
		BUG("write-in-file does not support '%s' mode", ಬಹುಲಕ);
	fp = fopen(ಮಾರ್ಗ, ಬಹುಲಕ);
	ಆದರೆ (!fp)
		ಹಿಂತಿರುಗಿ error_errno(_("cannot open file '%s' in mode '%s'"), ಮಾರ್ಗ, ಬಹುಲಕ);
	res = vfprintf(fp, format, ವಾದಗಳು);

	ಆದರೆ (res < 0) {
		ಪೂರ್ಣಾಂಕ saved_errno = errno;
		fclose(fp);
		errno = saved_errno;
		ಹಿಂತಿರುಗಿ error_errno(_("could not write to file '%s'"), ಮಾರ್ಗ);
	}

	ಹಿಂತಿರುಗಿ fclose(fp);
}

__attribute__((format (ಬರೆ, 2, 3)))
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ write_ಗಮ್ಯ_file(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format, ...)
{
	ಪೂರ್ಣಾಂಕ res;
	va_list ವಾದಗಳು;

	va_ಆರಂಭ(ವಾದಗಳು, format);
	res = write_ಒಳ_file(ಮಾರ್ಗ, "w", format, ವಾದಗಳು);
	va_ಕೊನೆ(ವಾದಗಳು);

	ಹಿಂತಿರುಗಿ res;
}

__attribute__((format (ಬರೆ, 2, 3)))
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ append_ಗಮ್ಯ_file(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format, ...)
{
	ಪೂರ್ಣಾಂಕ res;
	va_list ವಾದಗಳು;

	va_ಆರಂಭ(ವಾದಗಳು, format);
	res = write_ಒಳ_file(ಮಾರ್ಗ, "a", format, ವಾದಗಳು);
	va_ಕೊನೆ(ವಾದಗಳು);

	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ print_file_ಗಮ್ಯ_stdout(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಪೂರ್ಣಾಂಕ fd = open(ಮಾರ್ಗ, O_RDONLY);
	ಪೂರ್ಣಾಂಕ ret = 0;

	ಆದರೆ (fd < 0)
		ಹಿಂತಿರುಗಿ error_errno(_("cannot open file '%s' for reading"), ಮಾರ್ಗ);
	ಆದರೆ (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), ಮಾರ್ಗ);
	close(fd);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_ಪದ_format(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪದ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *orig_ಪದ)
{
	ಪೂರ್ಣಾಂಕ res;
	ಅಕ್ಷರ *new_ಪದ = xstrfmt("refs/bisect/%s", ಪದ);

	res = check_refname_format(new_ಪದ, 0);
	ಬಿಡುಗಡೆ(new_ಪದ);

	ಆದರೆ (res)
		ಹಿಂತಿರುಗಿ error(_("'%s' is not a valid term"), ಪದ);

	ಆದರೆ (one_ದ(ಪದ, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", ಖಾಲಿ))
		ಹಿಂತಿರುಗಿ error(_("can't use the builtin command '%s' as a term"), ಪದ);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	ಆದರೆ ((strcmp(orig_ಪದ, "bad") && one_ದ(ಪದ, "bad", "new", ಖಾಲಿ)) ||
		 (strcmp(orig_ಪದ, "good") && one_ದ(ಪದ, "good", "old", ಖಾಲಿ)))
		ಹಿಂತಿರುಗಿ error(_("can't change the meaning of the term '%s'"), ಪದ);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ write_terms(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *bad, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *good)
{
	ಪೂರ್ಣಾಂಕ res;

	ಆದರೆ (!strcmp(bad, good))
		ಹಿಂತಿರುಗಿ error(_("please use two different terms"));

	ಆದರೆ (check_ಪದ_format(bad, "bad") || check_ಪದ_format(good, "good"))
		ಹಿಂತಿರುಗಿ -1;

	res = write_ಗಮ್ಯ_file(git_ಮಾರ್ಗ_bisect_terms(), "%s\n%s\n", bad, good);

	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bisect_reset(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *commit, bool quiet)
{
	ರಚನೆ strbuf branch = STRBUF_INIT;

	ಆದರೆ (!commit) {
		ಆದರೆ (!strbuf_read_file(&branch, git_ಮಾರ್ಗ_bisect_ಆರಂಭ(), 0))
			ಬರೆ(_("We are not bisecting.\n"));
		ಇಲ್ಲದಿದ್ದರೆ
			strbuf_rtrim(&branch);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ object_id oid;

		ಆದರೆ (repo_get_oid_commit(the_repository, commit, &oid))
			ಹಿಂತಿರುಗಿ error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	ಆದರೆ (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.ವಾದಗಳು, "checkout", "--ignore-other-worktrees", ಖಾಲಿ);
		ಆದರೆ (quiet)
			strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--quiet");
		strvec_pushl(&cmd.ವಾದಗಳು, branch.buf, "--", ಖಾಲಿ);
		ಆದರೆ (ಸತತ_command(&cmd)) {
			error(_("could not check out original"
				" HEAD '%s'. Try 'git bisect"
				" reset <commit>'."), branch.buf);
			strbuf_release(&branch);
			ಹಿಂತಿರುಗಿ -1;
		}
	}

	strbuf_release(&branch);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_reset_when_ಸಿಕ್ಕಿತು(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
				  ಎಣಿಕೆ reset_when_ಸಿಕ್ಕಿತು_ಬಹುಲಕ *ಬಹುಲಕ)
{
	ಆದರೆ (!strcmp(ಮೌಲ್ಯ, "original"))
		*ಬಹುಲಕ = RESET_WHEN_ಸಿಕ್ಕಿತು_ಗಮ್ಯ_ORIGINAL;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(ಮೌಲ್ಯ, "found"))
		*ಬಹುಲಕ = RESET_WHEN_ಸಿಕ್ಕಿತು_ಗಮ್ಯ_ಸಿಕ್ಕಿತು;
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ error(_("invalid value for '--reset-when-found': '%s'"),
			     ಮೌಲ್ಯ);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *reset_when_ಸಿಕ್ಕಿತು_ಬಹುಲಕ_name(ಎಣಿಕೆ reset_when_ಸಿಕ್ಕಿತು_ಬಹುಲಕ ಬಹುಲಕ)
{
	ಆಯ್ಕೆ (ಬಹುಲಕ) {
	ಸ್ಥಿತಿ RESET_WHEN_ಸಿಕ್ಕಿತು_ಗಮ್ಯ_ORIGINAL:
		ಹಿಂತಿರುಗಿ "original";
	ಸ್ಥಿತಿ RESET_WHEN_ಸಿಕ್ಕಿತು_ಗಮ್ಯ_ಸಿಕ್ಕಿತು:
		ಹಿಂತಿರುಗಿ "found";
	ಸ್ಥಿತಿ RESET_WHEN_ಸಿಕ್ಕಿತು_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", ಬಹುಲಕ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ read_reset_when_ಸಿಕ್ಕಿತು(ಎಣಿಕೆ reset_when_ಸಿಕ್ಕಿತು_ಬಹುಲಕ *ಬಹುಲಕ)
{
	ರಚನೆ strbuf ಮೌಲ್ಯ = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ res = 0;

	*ಬಹುಲಕ = RESET_WHEN_ಸಿಕ್ಕಿತು_NONE;
	ಆದರೆ (is_ಖಾಲಿಗಳು_or_missing_file(git_ಮಾರ್ಗ_bisect_reset_when_ಸಿಕ್ಕಿತು()))
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (strbuf_read_file(&ಮೌಲ್ಯ, git_ಮಾರ್ಗ_bisect_reset_when_ಸಿಕ್ಕಿತು(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_ಮಾರ್ಗ_bisect_reset_when_ಸಿಕ್ಕಿತು());
		ಹೋಗು ಹೊರಹೋಗು;
	}
	strbuf_trim(&ಮೌಲ್ಯ);
	ಆದರೆ (parse_reset_when_ಸಿಕ್ಕಿತು(ಮೌಲ್ಯ.buf, ಬಹುಲಕ))
		res = -1;

ಹೊರಹೋಗು:
	strbuf_release(&ಮೌಲ್ಯ);
	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bisect_reset_when_ಸಿಕ್ಕಿತು(ಎಣಿಕೆ reset_when_ಸಿಕ್ಕಿತು_ಬಹುಲಕ ಬಹುಲಕ)
{
	ರಚನೆ bisect_terms terms = { 0 };
	ಅಕ್ಷರ *commit = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ res;

	ಆದರೆ (ಬಹುಲಕ == RESET_WHEN_ಸಿಕ್ಕಿತು_ಗಮ್ಯ_ಸಿಕ್ಕಿತು) {
		read_bisect_terms(&terms.ಪದ_bad, &terms.ಪದ_good);
		commit = xstrfmt("refs/bisect/%s", terms.ಪದ_bad);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಬಹುಲಕ == RESET_WHEN_ಸಿಕ್ಕಿತು_NONE) {
		BUG("automatic reset requested without a reset mode");
	}

	res = bisect_reset(commit, true);
	ಆದರೆ (!res)
		res = bisect_clean_state();

	ಬಿಡುಗಡೆ(commit);
	free_terms(&terms);
	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಶೂನ್ಯ log_commit(FILE *fp,
		       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state,
		       ರಚನೆ commit *commit)
{
	ರಚನೆ pretty_print_context pp = {0};
	ರಚನೆ strbuf commit_ಸಂದೇಶ = STRBUF_INIT;
	ಅಕ್ಷರ *label = xstrfmt(fmt, state);

	repo_format_commit_ಸಂದೇಶ(the_repository, commit, "%s", &commit_ಸಂದೇಶ,
				   &pp);

	fprintf(fp, "# %s: [%s] %s\n", label, oid_ಗಮ್ಯ_hex(&commit->object.oid),
		commit_ಸಂದೇಶ.buf);

	strbuf_release(&commit_ಸಂದೇಶ);
	ಬಿಡುಗಡೆ(label);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bisect_write(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rev,
			ಸ್ಥಿರಾಂಕ ರಚನೆ bisect_terms *terms, ಪೂರ್ಣಾಂಕ nolog)
{
	ರಚನೆ strbuf ಟ್ಯಾಗ್ = STRBUF_INIT;
	ರಚನೆ object_id oid;
	ರಚನೆ commit *commit;
	FILE *fp = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ res = 0;

	ಆದರೆ (!strcmp(state, terms->ಪದ_bad)) {
		strbuf_addf(&ಟ್ಯಾಗ್, "refs/bisect/%s", state);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (one_ದ(state, terms->ಪದ_good, "skip", ಖಾಲಿ)) {
		strbuf_addf(&ಟ್ಯಾಗ್, "refs/bisect/%s-%s", state, rev);
	} ಇಲ್ಲದಿದ್ದರೆ {
		res = error(_("Bad bisect_write argument: %s"), state);
		ಹೋಗು ಮುಕ್ತಾಯ;
	}

	ಆದರೆ (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		ಹೋಗು ಮುಕ್ತಾಯ;
	}

	ಆದರೆ (refs_update_ref(get_main_ref_store(the_repository), ಖಾಲಿ, ಟ್ಯಾಗ್.buf, &oid, ಖಾಲಿ, 0,
			    UPDATE_REFS_ಸಂದೇಶ_ON_ERR)) {
		res = -1;
		ಹೋಗು ಮುಕ್ತಾಯ;
	}

	fp = fopen(git_ಮಾರ್ಗ_bisect_log(), "a");
	ಆದರೆ (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_ಮಾರ್ಗ_bisect_log());
		ಹೋಗು ಮುಕ್ತಾಯ;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	ಆದರೆ (!nolog)
		fprintf(fp, "git bisect %s %s\n", state, rev);

ಮುಕ್ತಾಯ:
	ಆದರೆ (fp)
		fclose(fp);
	strbuf_release(&ಟ್ಯಾಗ್);
	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_and_ಗಣ_terms(ರಚನೆ bisect_terms *terms, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cmd)
{
	ಪೂರ್ಣಾಂಕ has_ಪದ_file = !is_ಖಾಲಿಗಳು_or_missing_file(git_ಮಾರ್ಗ_bisect_terms());

	ಆದರೆ (one_ದ(cmd, "skip", "start", "terms", ಖಾಲಿ))
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (has_ಪದ_file && strcmp(cmd, terms->ಪದ_bad) &&
	    strcmp(cmd, terms->ಪದ_good))
		ಹಿಂತಿರುಗಿ error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->ಪದ_bad,
				terms->ಪದ_good);

	ಆದರೆ (!has_ಪದ_file) {
		ಆದರೆ (one_ದ(cmd, "bad", "good", ಖಾಲಿ)) {
			ಗಣ_terms(terms, "bad", "good");
			ಹಿಂತಿರುಗಿ write_terms(terms->ಪದ_bad, terms->ಪದ_good);
		}
		ಆದರೆ (one_ದ(cmd, "new", "old", ಖಾಲಿ)) {
			ಗಣ_terms(terms, "new", "old");
			ಹಿಂತಿರುಗಿ write_terms(terms->ಪದ_bad, terms->ಪದ_good);
		}
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ inc_nr(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref ಬಳಸದ, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *nr = (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *)cb_ದತ್ತಾಂಶ;
	(*nr)++;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ need_bad_and_good_revision_warning[] =
	N_("You need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ need_bisect_ಆರಂಭ_warning[] =
	N_("You need to start by \"git bisect start\".\n"
	   "You then need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ decide_ಮುಂದಿನ(ಸ್ಥಿರಾಂಕ ರಚನೆ bisect_terms *terms,
		       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *current_ಪದ, ಪೂರ್ಣಾಂಕ missing_good,
		       ಪೂರ್ಣಾಂಕ missing_bad)
{
	ಆದರೆ (!missing_good && !missing_bad)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (!current_ಪದ)
		ಹಿಂತಿರುಗಿ -1;

	ಆದರೆ (missing_good && !missing_bad &&
	    !strcmp(current_ಪದ, terms->ಪದ_good)) {
		ಅಕ್ಷರ *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->ಪದ_bad);
		ಆದರೆ (!isatty(0))
			ಹಿಂತಿರುಗಿ 0;
		/*
		 * TRANSLATORS: Make sure to include [Y] and [n] in your
		 * translation. The program will only accept English input
		 * at this point.
		 */
		yesno = git_prompt(_("Are you sure [Y/n]? "), PROMPT_ECHO);
		ಆದರೆ (starts_with(yesno, "N") || starts_with(yesno, "n"))
			ಹಿಂತಿರುಗಿ -1;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!is_ಖಾಲಿಗಳು_or_missing_file(git_ಮಾರ್ಗ_bisect_ಆರಂಭ()))
		ಹಿಂತಿರುಗಿ error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ error(_(need_bisect_ಆರಂಭ_warning),
			     vocab_good, vocab_bad, vocab_good, vocab_bad);
}

ಸ್ಥಿರ ಶೂನ್ಯ bisect_ಸ್ಥಿತಿ(ರಚನೆ bisect_state *state,
			  ಸ್ಥಿರಾಂಕ ರಚನೆ bisect_terms *terms)
{
	ಅಕ್ಷರ *bad_ref = xstrfmt("refs/bisect/%s", terms->ಪದ_bad);
	ಅಕ್ಷರ *good_glob = xstrfmt("%s-*", terms->ಪದ_good);
	ರಚನೆ refs_for_each_ref_options opts = {
		.ನಮೂನೆ = good_glob,
		.ಪೂರ್ವಪ್ರತ್ಯಯ = "refs/bisect/",
		.trim_ಪೂರ್ವಪ್ರತ್ಯಯ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ("refs/bisect/"),
	};

	ಆದರೆ (refs_ref_exists(get_main_ref_store(the_repository), bad_ref))
		state->nr_bad = 1;

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      inc_nr, &state->nr_good, &opts);

	ಬಿಡುಗಡೆ(good_glob);
	ಬಿಡುಗಡೆ(bad_ref);
}

__attribute__((format (ಬರೆ, 1, 2)))
ಸ್ಥಿರ ಶೂನ್ಯ bisect_log_printf(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	va_list ap;

	va_ಆರಂಭ(ap, fmt);
	strbuf_vaddf(&buf, fmt, ap);
	va_ಕೊನೆ(ap);

	ಬರೆ("%s", buf.buf);
	append_ಗಮ್ಯ_file(git_ಮಾರ್ಗ_bisect_log(), "# %s", buf.buf);

	strbuf_release(&buf);
}

ಸ್ಥಿರ ಶೂನ್ಯ bisect_print_ಸ್ಥಿತಿ(ಸ್ಥಿರಾಂಕ ರಚನೆ bisect_terms *terms)
{
	ರಚನೆ bisect_state state = { 0 };

	bisect_ಸ್ಥಿತಿ(&state, terms);

	/* If we had both, we'd already be started, and shouldn't get here. */
	ಆದರೆ (state.nr_good && state.nr_bad)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!state.nr_good && !state.nr_bad)
		bisect_log_printf(_("status: waiting for both '%s' and '%s' commits\n"),
				  terms->ಪದ_good, terms->ಪದ_bad);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (state.nr_good)
		bisect_log_printf(Q_("status: waiting for '%s' commit, %d '%s' commit known\n",
				     "status: waiting for '%s' commit, %d '%s' commits known\n",
				     state.nr_good),
				  terms->ಪದ_bad, state.nr_good, terms->ಪದ_good);
	ಇಲ್ಲದಿದ್ದರೆ
		bisect_log_printf(_("status: waiting for '%s' commit(s), '%s' commit known\n"),
				  terms->ಪದ_good, terms->ಪದ_bad);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bisect_ಮುಂದಿನ_check(ಸ್ಥಿರಾಂಕ ರಚನೆ bisect_terms *terms,
			     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *current_ಪದ)
{
	ರಚನೆ bisect_state state = { 0 };
	bisect_ಸ್ಥಿತಿ(&state, terms);
	ಹಿಂತಿರುಗಿ decide_ಮುಂದಿನ(terms, current_ಪದ, !state.nr_good, !state.nr_bad);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_terms(ರಚನೆ bisect_terms *terms, ಪೂರ್ಣಾಂಕ file_missing_is_ok)
{
	ರಚನೆ strbuf str = STRBUF_INIT;
	FILE *fp = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ res = 0;

	fp = fopen(git_ಮಾರ್ಗ_bisect_terms(), "r");
	ಆದರೆ (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		ಹೋಗು ಮುಕ್ತಾಯ;
	}

	free_terms(terms);
	ಆದರೆ (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		ಹೋಗು ಮುಕ್ತಾಯ;
	}
	terms->ಪದ_bad = strbuf_detach(&str, ಖಾಲಿ);
	ಆದರೆ (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		FREE_AND_NULL(terms->ಪದ_bad);
		ಹೋಗು ಮುಕ್ತಾಯ;
	}
	terms->ಪದ_good = strbuf_detach(&str, ಖಾಲಿ);

ಮುಕ್ತಾಯ:
	ಆದರೆ (fp)
		fclose(fp);
	strbuf_release(&str);
	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bisect_terms(ರಚನೆ bisect_terms *terms, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *option)
{
	ಆದರೆ (get_terms(terms, 0))
		ಹಿಂತಿರುಗಿ error(_("no terms defined"));

	ಆದರೆ (!option) {
		ಬರೆ(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->ಪದ_good, terms->ಪದ_bad);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (one_ದ(option, "--term-good", "--term-old", ಖಾಲಿ))
		ಬರೆ("%s\n", terms->ಪದ_good);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (one_ದ(option, "--term-bad", "--term-new", ಖಾಲಿ))
		ಬರೆ("%s\n", terms->ಪದ_bad);
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ error(_("invalid argument %s for 'git bisect terms'.\n"
			       "Supported options are: "
			       "--term-good|--term-old and "
			       "--term-bad|--term-new."), option);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bisect_append_log_quoted(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv)
{
	ಪೂರ್ಣಾಂಕ res = 0;
	FILE *fp = fopen(git_ಮಾರ್ಗ_bisect_log(), "a");
	ರಚನೆ strbuf orig_ವಾದಗಳು = STRBUF_INIT;

	ಆದರೆ (!fp)
		ಹಿಂತಿರುಗಿ -1;

	ಆದರೆ (fprintf(fp, "git bisect start") < 1) {
		res = -1;
		ಹೋಗು ಮುಕ್ತಾಯ;
	}

	sq_quote_argv(&orig_ವಾದಗಳು, argv);
	ಆದರೆ (fprintf(fp, "%s\n", orig_ವಾದಗಳು.buf) < 1)
		res = -1;

ಮುಕ್ತಾಯ:
	fclose(fp);
	strbuf_release(&orig_ವಾದಗಳು);
	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_bisect_ref(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cb)
{
	ರಚನೆ ಕೂಡಿಸು_bisect_ref_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = cb;

	ಕೂಡಿಸು_pending_oid(ದತ್ತಾಂಶ->revs, ref->name, ref->oid, ದತ್ತಾಂಶ->object_flags);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ prepare_revs(ರಚನೆ bisect_terms *terms, ರಚನೆ rev_info *revs)
{
	ರಚನೆ refs_for_each_ref_options opts = {
		.ಪೂರ್ವಪ್ರತ್ಯಯ = "refs/bisect/",
		.trim_ಪೂರ್ವಪ್ರತ್ಯಯ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ("refs/bisect/"),
	};
	ಪೂರ್ಣಾಂಕ res = 0;
	ರಚನೆ ಕೂಡಿಸು_bisect_ref_ದತ್ತಾಂಶ cb = { revs };
	ಅಕ್ಷರ *good = xstrfmt("%s-*", terms->ಪದ_good);

	/*
	 * We cannot use terms->term_bad directly in
	 * for_each_glob_ref_in() and we have to append a '*' to it,
	 * otherwise for_each_glob_ref_in() will append '/' and '*'.
	 */
	ಅಕ್ಷರ *bad = xstrfmt("%s*", terms->ಪದ_bad);

	/*
	 * It is important to reset the flags used by revision walks
	 * as the previous call to bisect_next_all() in turn
	 * sets up a revision walk.
	 */
	reset_revision_walk();
	repo_init_revisions(the_repository, revs, ಖಾಲಿ);
	ಸಿದ್ಧತೆ_revisions(0, ಖಾಲಿ, revs, ಖಾಲಿ);

	opts.ನಮೂನೆ = bad;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      ಕೂಡಿಸು_bisect_ref, &cb, &opts);

	cb.object_flags = UNINTERESTING;
	opts.ನಮೂನೆ = good;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      ಕೂಡಿಸು_bisect_ref, &cb, &opts);

	ಆದರೆ (prepare_revision_walk(revs))
		res = error(_("revision walk setup failed"));

	ಬಿಡುಗಡೆ(good);
	ಬಿಡುಗಡೆ(bad);
	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bisect_skipped_commits(ರಚನೆ bisect_terms *terms)
{
	ಪೂರ್ಣಾಂಕ res;
	FILE *fp = ಖಾಲಿ;
	ರಚನೆ rev_info revs;
	ರಚನೆ commit *commit;
	ರಚನೆ pretty_print_context pp = {0};
	ರಚನೆ strbuf commit_name = STRBUF_INIT;

	res = prepare_revs(terms, &revs);
	ಆದರೆ (res)
		ಹಿಂತಿರುಗಿ res;

	fp = fopen(git_ಮಾರ್ಗ_bisect_log(), "a");
	ಆದರೆ (!fp)
		ಹಿಂತಿರುಗಿ error_errno(_("could not open '%s' for appending"),
				  git_ಮಾರ್ಗ_bisect_log());

	ಆದರೆ (fprintf(fp, "# only skipped commits left to test\n") < 0)
		ಹಿಂತಿರುಗಿ error_errno(_("failed to write to '%s'"), git_ಮಾರ್ಗ_bisect_log());

	ಆಗ ((commit = get_revision(&revs)) != ಖಾಲಿ) {
		strbuf_reset(&commit_name);
		repo_format_commit_ಸಂದೇಶ(the_repository, commit, "%s",
					   &commit_name, &pp);
		fprintf(fp, "# possible first '%s' commit: [%s] %s\n",
			terms->ಪದ_bad, oid_ಗಮ್ಯ_hex(&commit->object.oid),
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
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bisect_successful(ರಚನೆ bisect_terms *terms)
{
	ರಚನೆ object_id oid;
	ರಚನೆ commit *commit;
	ರಚನೆ pretty_print_context pp = {0};
	ರಚನೆ strbuf commit_name = STRBUF_INIT;
	ಅಕ್ಷರ *bad_ref = xstrfmt("refs/bisect/%s",terms->ಪದ_bad);
	ಪೂರ್ಣಾಂಕ res;

	refs_read_ref(get_main_ref_store(the_repository), bad_ref, &oid);
	commit = lookup_commit_reference_by_name(bad_ref);
	ಆದರೆ (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		ಬಿಡುಗಡೆ(bad_ref);
		ಹಿಂತಿರುಗಿ BISECT_FAILED;
	}
	repo_format_commit_ಸಂದೇಶ(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_ಗಮ್ಯ_file(git_ಮಾರ್ಗ_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->ಪದ_bad, oid_ಗಮ್ಯ_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	ಬಿಡುಗಡೆ(bad_ref);
	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಎಣಿಕೆ bisect_error bisect_ಮುಂದಿನ(ರಚನೆ bisect_terms *terms,
				     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
	ಎಣಿಕೆ bisect_error res;

	ಆದರೆ (bisect_autostart(terms))
		ಹಿಂತಿರುಗಿ BISECT_FAILED;

	ಆದರೆ (bisect_ಮುಂದಿನ_check(terms, terms->ಪದ_good))
		ಹಿಂತಿರುಗಿ BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_ಮುಂದಿನ_all(the_repository, ಪೂರ್ವಪ್ರತ್ಯಯ);

	ಆದರೆ (res == BISECT_ಆಂತರಿಕ_SUCCESS_1_ST_BAD_ಸಿಕ್ಕಿತು) {
		res = bisect_successful(terms);
		ಹಿಂತಿರುಗಿ res ? res : BISECT_ಆಂತರಿಕ_SUCCESS_1_ST_BAD_ಸಿಕ್ಕಿತು;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (res == BISECT_ONLY_SKIPPED_ಎಡ) {
		res = bisect_skipped_commits(terms);
		ಹಿಂತಿರುಗಿ res ? res : BISECT_ONLY_SKIPPED_ಎಡ;
	}
	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಎಣಿಕೆ bisect_error bisect_auto_ಮುಂದಿನ(ರಚನೆ bisect_terms *terms,
					  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
	ಆದರೆ (bisect_ಮುಂದಿನ_check(terms, ಖಾಲಿ)) {
		bisect_print_ಸ್ಥಿತಿ(terms);
		ಹಿಂತಿರುಗಿ BISECT_OK;
	}

	ಹಿಂತಿರುಗಿ bisect_ಮುಂದಿನ(terms, ಪೂರ್ವಪ್ರತ್ಯಯ);
}

ಸ್ಥಿರ ಎಣಿಕೆ bisect_error bisect_ಆರಂಭ(ರಚನೆ bisect_terms *terms, ಪೂರ್ಣಾಂಕ argc,
				      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv)
{
	ಪೂರ್ಣಾಂಕ no_checkout = 0;
	ಪೂರ್ಣಾಂಕ ಮೊದಲ_ಪೋಷಕ_only = 0;
	ಪೂರ್ಣಾಂಕ i, has_double_dash = 0, must_write_terms = 0, bad_ನೋಡಿದ = 0;
	ಪೂರ್ಣಾಂಕ flags, pathspec_pos;
	ಎಣಿಕೆ bisect_error res = BISECT_OK;
	ರಚನೆ string_list revs = STRING_LIST_INIT_DUP;
	ರಚನೆ string_list ಸ್ಥಿತಿಗಳು = STRING_LIST_INIT_DUP;
	ರಚನೆ strbuf ಆರಂಭ_ತಲೆ = STRBUF_INIT;
	ರಚನೆ strbuf bisect_ಹೆಸರುಗಳು = STRBUF_INIT;
	ರಚನೆ object_id ತಲೆ_oid;
	ರಚನೆ object_id oid;
	ಎಣಿಕೆ reset_when_ಸಿಕ್ಕಿತು_ಬಹುಲಕ reset_when_ಸಿಕ್ಕಿತು = RESET_WHEN_ಸಿಕ್ಕಿತು_NONE;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಲೆ;

	ಆದರೆ (is_bare_repository(the_repository))
		no_checkout = 1;

	/*
	 * Check for one bad and then some good revisions
	 */
	ಗಾಗಿ (i = 0; i < argc; i++) {
		ಆದರೆ (!strcmp(argv[i], "--")) {
			has_double_dash = 1;
			ನಿಲ್ಲಿಸು;
		}
	}

	ಗಾಗಿ (i = 0; i < argc; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg = argv[i];
		ಆದರೆ (!strcmp(argv[i], "--")) {
			ನಿಲ್ಲಿಸು;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "--no-checkout")) {
			no_checkout = 1;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "--first-parent")) {
			ಮೊದಲ_ಪೋಷಕ_only = 1;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "--reset-when-found")) {
			reset_when_ಸಿಕ್ಕಿತು = RESET_WHEN_ಸಿಕ್ಕಿತು_ಗಮ್ಯ_ORIGINAL;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "--reset-when-found=", &arg)) {
			ಆದರೆ (parse_reset_when_ಸಿಕ್ಕಿತು(arg, &reset_when_ಸಿಕ್ಕಿತು)) {
				res = BISECT_FAILED;
				ಹೋಗು ಮುಕ್ತಾಯ;
			}
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "--term-good") ||
			 !strcmp(arg, "--term-old")) {
			i++;
			ಆದರೆ (argc <= i)
				ಹಿಂತಿರುಗಿ error(_("'' is not a valid term"));
			must_write_terms = 1;
			ಬಿಡುಗಡೆ((ಶೂನ್ಯ *) terms->ಪದ_good);
			terms->ಪದ_good = xstrdup(argv[i]);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "--term-good=", &arg) ||
			   skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "--term-old=", &arg)) {
			must_write_terms = 1;
			ಬಿಡುಗಡೆ((ಶೂನ್ಯ *) terms->ಪದ_good);
			terms->ಪದ_good = xstrdup(arg);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(arg, "--term-bad") ||
			 !strcmp(arg, "--term-new")) {
			i++;
			ಆದರೆ (argc <= i)
				ಹಿಂತಿರುಗಿ error(_("'' is not a valid term"));
			must_write_terms = 1;
			ಬಿಡುಗಡೆ((ಶೂನ್ಯ *) terms->ಪದ_bad);
			terms->ಪದ_bad = xstrdup(argv[i]);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "--term-bad=", &arg) ||
			   skip_ಪೂರ್ವಪ್ರತ್ಯಯ(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			ಬಿಡುಗಡೆ((ಶೂನ್ಯ *) terms->ಪದ_bad);
			terms->ಪದ_bad = xstrdup(arg);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (starts_with(arg, "--")) {
			ಹಿಂತಿರುಗಿ error(_("unrecognized option: '%s'"), arg);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!get_oidf(&oid, "%s^{commit}", arg)) {
			string_list_append(&revs, oid_ಗಮ್ಯ_hex(&oid));
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (has_double_dash) {
			die(_("'%s' does not appear to be a valid "
			      "revision"), arg);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ನಿಲ್ಲಿಸು;
		}
	}
	ಆದರೆ (reset_when_ಸಿಕ್ಕಿತು != RESET_WHEN_ಸಿಕ್ಕಿತು_NONE && no_checkout) {
		res = error(_("options '%s' and '%s' cannot be used together"),
			    "--reset-when-found", "--no-checkout");
		ಹೋಗು ಮುಕ್ತಾಯ;
	}
	pathspec_pos = i;

	/*
	 * The user ran "git bisect start <sha1> <sha1>", hence did not
	 * explicitly specify the terms, but we are already starting to
	 * set references named with the default terms, and won't be able
	 * to change afterwards.
	 */
	ಆದರೆ (revs.nr)
		must_write_terms = 1;
	ಗಾಗಿ (i = 0; i < revs.nr; i++) {
		ಆದರೆ (bad_ನೋಡಿದ) {
			string_list_append(&ಸ್ಥಿತಿಗಳು, terms->ಪದ_good);
		} ಇಲ್ಲದಿದ್ದರೆ {
			bad_ನೋಡಿದ = 1;
			string_list_append(&ಸ್ಥಿತಿಗಳು, terms->ಪದ_bad);
		}
	}

	/*
	 * Verify HEAD
	 */
	ತಲೆ = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
				       "HEAD", 0, &ತಲೆ_oid, &flags);
	ಆದರೆ (!ತಲೆ) {
		ಆದರೆ (repo_get_oid(the_repository, "HEAD", &ತಲೆ_oid))
			ಹಿಂತಿರುಗಿ error(_("bad HEAD - I need a HEAD"));
		ತಲೆ = "HEAD";
	}

	/*
	 * Check if we are bisecting
	 */
	ಆದರೆ (!is_ಖಾಲಿಗಳು_or_missing_file(git_ಮಾರ್ಗ_bisect_ಆರಂಭ())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&ಆರಂಭ_ತಲೆ, git_ಮಾರ್ಗ_bisect_ಆರಂಭ(), 0);
		strbuf_trim(&ಆರಂಭ_ತಲೆ);
		ಆದರೆ (!no_checkout) {
			ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.ವಾದಗಳು, "checkout", ಆರಂಭ_ತಲೆ.buf,
				     "--", ಖಾಲಿ);
			ಆದರೆ (ಸತತ_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       ಆರಂಭ_ತಲೆ.buf);
				ಹೋಗು ಮುಕ್ತಾಯ;
			}
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* Get the rev from where we start. */
		ಆದರೆ (!repo_get_oid(the_repository, ತಲೆ, &ತಲೆ_oid) &&
		    !starts_with(ತಲೆ, "refs/heads/")) {
			strbuf_reset(&ಆರಂಭ_ತಲೆ);
			strbuf_ಕೂಡಿಸು_oid_hex(&ಆರಂಭ_ತಲೆ, &ತಲೆ_oid);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!repo_get_oid(the_repository, ತಲೆ, &ತಲೆ_oid) &&
			   skip_ಪೂರ್ವಪ್ರತ್ಯಯ(ತಲೆ, "refs/heads/", &ತಲೆ)) {
			strbuf_addstr(&ಆರಂಭ_ತಲೆ, ತಲೆ);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಹಿಂತಿರುಗಿ error(_("bad HEAD - strange symbolic ref"));
		}
	}

	/*
	 * Get rid of any old bisect state.
	 */
	ಆದರೆ (bisect_clean_state())
		ಹಿಂತಿರುಗಿ BISECT_FAILED;

	/*
	 * Write new start state
	 */
	write_file(git_ಮಾರ್ಗ_bisect_ಆರಂಭ(), "%s\n", ಆರಂಭ_ತಲೆ.buf);

	ಆದರೆ (ಮೊದಲ_ಪೋಷಕ_only)
		write_file(git_ಮಾರ್ಗ_bisect_ಮೊದಲ_ಪೋಷಕ(), "\n");

	ಆದರೆ (reset_when_ಸಿಕ್ಕಿತು != RESET_WHEN_ಸಿಕ್ಕಿತು_NONE)
		write_file(git_ಮಾರ್ಗ_bisect_reset_when_ಸಿಕ್ಕಿತು(), "%s\n",
			   reset_when_ಸಿಕ್ಕಿತು_ಬಹುಲಕ_name(reset_when_ಸಿಕ್ಕಿತು));

	ಆದರೆ (no_checkout) {
		ಆದರೆ (repo_get_oid(the_repository, ಆರಂಭ_ತಲೆ.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), ಆರಂಭ_ತಲೆ.buf);
			ಹೋಗು ಮುಕ್ತಾಯ;
		}
		ಆದರೆ (refs_update_ref(get_main_ref_store(the_repository), ಖಾಲಿ, "BISECT_HEAD", &oid, ಖಾಲಿ, 0,
				    UPDATE_REFS_ಸಂದೇಶ_ON_ERR)) {
			res = BISECT_FAILED;
			ಹೋಗು ಮುಕ್ತಾಯ;
		}
	}

	ಆದರೆ (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_ಹೆಸರುಗಳು, argv + pathspec_pos);
	write_file(git_ಮಾರ್ಗ_bisect_ಹೆಸರುಗಳು(), "%s\n", bisect_ಹೆಸರುಗಳು.buf);

	ಗಾಗಿ (i = 0; i < ಸ್ಥಿತಿಗಳು.nr; i++)
		ಆದರೆ (bisect_write(ಸ್ಥಿತಿಗಳು.ವಸ್ತುಗಳು[i].string,
				 revs.ವಸ್ತುಗಳು[i].string, terms, 1)) {
			res = BISECT_FAILED;
			ಹೋಗು ಮುಕ್ತಾಯ;
		}

	ಆದರೆ (must_write_terms && write_terms(terms->ಪದ_bad,
					    terms->ಪದ_good)) {
		res = BISECT_FAILED;
		ಹೋಗು ಮುಕ್ತಾಯ;
	}

	res = bisect_append_log_quoted(argv);
	ಆದರೆ (res)
		res = BISECT_FAILED;

ಮುಕ್ತಾಯ:
	string_list_clear(&revs, 0);
	string_list_clear(&ಸ್ಥಿತಿಗಳು, 0);
	strbuf_release(&ಆರಂಭ_ತಲೆ);
	strbuf_release(&bisect_ಹೆಸರುಗಳು);
	ಆದರೆ (res)
		ಹಿಂತಿರುಗಿ res;

	res = bisect_auto_ಮುಂದಿನ(terms, ಖಾಲಿ);
	ಆದರೆ (!is_bisect_success(res))
		bisect_clean_state();
	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ file_is_not_ಖಾಲಿಗಳು(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಹಿಂತಿರುಗಿ !is_ಖಾಲಿಗಳು_or_missing_file(ಮಾರ್ಗ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bisect_autostart(ರಚನೆ bisect_terms *terms)
{
	ಪೂರ್ಣಾಂಕ res;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *yesno;

	ಆದರೆ (file_is_not_ಖಾಲಿಗಳು(git_ಮಾರ್ಗ_bisect_ಆರಂಭ()))
		ಹಿಂತಿರುಗಿ 0;

	fprintf_ln(stderr, _("You need to start by \"git bisect "
			  "start\"\n"));

	ಆದರೆ (!isatty(STDIN_FILENO))
		ಹಿಂತಿರುಗಿ -1;

	/*
	 * TRANSLATORS: Make sure to include [Y] and [n] in your
	 * translation. The program will only accept English input
	 * at this point.
	 */
	yesno = git_prompt(_("Do you want me to do it for you "
			     "[Y/n]? "), PROMPT_ECHO);
	res = tolower(*yesno) == 'n' ?
		-1 : bisect_ಆರಂಭ(terms, 0, ಖಾಲಿಗಳು_strvec);

	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಎಣಿಕೆ bisect_error bisect_state(ರಚನೆ bisect_terms *terms, ಪೂರ್ಣಾಂಕ argc,
				      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state;
	ಪೂರ್ಣಾಂಕ i, verify_expected = 1;
	ರಚನೆ object_id oid, expected;
	ರಚನೆ oid_array revs = OID_ARRAY_INIT;

	ಆದರೆ (!argc)
		ಹಿಂತಿರುಗಿ error(_("Please call `--bisect-state` with at least one argument"));

	ಆದರೆ (bisect_autostart(terms))
		ಹಿಂತಿರುಗಿ BISECT_FAILED;

	state = argv[0];
	ಆದರೆ (check_and_ಗಣ_terms(terms, state) ||
	    !one_ದ(state, terms->ಪದ_good, terms->ಪದ_bad, "skip", ಖಾಲಿ))
		ಹಿಂತಿರುಗಿ BISECT_FAILED;

	argv++;
	argc--;
	ಆದರೆ (argc > 1 && !strcmp(state, terms->ಪದ_bad))
		ಹಿಂತಿರುಗಿ error(_("'git bisect %s' can take only one argument."), terms->ಪದ_bad);

	ಆದರೆ (argc == 0) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಲೆ = "BISECT_HEAD";
		ಎಣಿಕೆ get_oid_ಫಲಿತಾಂಶ res_ತಲೆ = repo_get_oid(the_repository,
							    ತಲೆ, &oid);

		ಆದರೆ (res_ತಲೆ == MISSING_OBJECT) {
			ತಲೆ = "HEAD";
			res_ತಲೆ = repo_get_oid(the_repository, ತಲೆ, &oid);
		}

		ಆದರೆ (res_ತಲೆ)
			error(_("Bad rev input: %s"), ತಲೆ);
		oid_array_append(&revs, &oid);
	}

	/*
	 * All input revs must be checked before executing bisect_write()
	 * to discard junk revs.
	 */

	ಗಾಗಿ (; argc; argc--, argv++) {
		ರಚನೆ commit *commit;

		ಆದರೆ (repo_get_oid(the_repository, *argv, &oid)){
			error(_("Bad rev input: %s"), *argv);
			oid_array_clear(&revs);
			ಹಿಂತಿರುಗಿ BISECT_FAILED;
		}

		commit = lookup_commit_reference(the_repository, &oid);
		ಆದರೆ (!commit)
			die(_("Bad rev input (not a commit): %s"), *argv);

		oid_array_append(&revs, &commit->object.oid);
	}

	ಆದರೆ (refs_read_ref(get_main_ref_store(the_repository), "BISECT_EXPECTED_REV", &expected))
		verify_expected = 0; /* Ignore invalid file contents */

	ಗಾಗಿ (i = 0; i < revs.nr; i++) {
		ಆದರೆ (bisect_write(state, oid_ಗಮ್ಯ_hex(&revs.oid[i]), terms, 0)) {
			oid_array_clear(&revs);
			ಹಿಂತಿರುಗಿ BISECT_FAILED;
		}
		ಆದರೆ (verify_expected && !oideq(&revs.oid[i], &expected)) {
			unlink_or_warn(git_ಮಾರ್ಗ_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					ಖಾಲಿ, "BISECT_EXPECTED_REV", ಖಾಲಿ,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	ಹಿಂತಿರುಗಿ bisect_auto_ಮುಂದಿನ(terms, ಖಾಲಿ);
}

ಸ್ಥಿರ ಎಣಿಕೆ bisect_error bisect_log(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿತಿ_2;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* filename = git_ಮಾರ್ಗ_bisect_log();

	ಆದರೆ (is_ಖಾಲಿಗಳು_or_missing_file(filename))
		ಹಿಂತಿರುಗಿ error(_("We are not bisecting."));

	fd = open(filename, O_RDONLY);
	ಆದರೆ (fd < 0)
		ಹಿಂತಿರುಗಿ BISECT_FAILED;

	ಸ್ಥಿತಿ_2 = copy_fd(fd, STDOUT_FILENO);
	close(fd);
	ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2 ? BISECT_FAILED : BISECT_OK;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ process_replay_ಸಾಲು(ರಚನೆ bisect_terms *terms, ರಚನೆ strbuf *ಸಾಲು)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p = ಸಾಲು->buf + strspn(ಸಾಲು->buf, " \t");
	ಅಕ್ಷರ *ಶಬ್ದ_ಕೊನೆ, *rev;

	ಆದರೆ ((!skip_ಪೂರ್ವಪ್ರತ್ಯಯ(p, "git bisect", &p) &&
	!skip_ಪೂರ್ವಪ್ರತ್ಯಯ(p, "git-bisect", &p)) || !ಖಾಲಿಯೇ(*p))
		ಹಿಂತಿರುಗಿ 0;
	p += strspn(p, " \t");

	ಶಬ್ದ_ಕೊನೆ = (ಅಕ್ಷರ *)p + strcspn(p, " \t");
	rev = ಶಬ್ದ_ಕೊನೆ + strspn(ಶಬ್ದ_ಕೊನೆ, " \t");
	*ಶಬ್ದ_ಕೊನೆ = '\0'; /* NUL-terminate the word */

	ಆದರೆ (get_terms(terms, 1))
		ಹಿಂತಿರುಗಿ error(_("no terms defined"));
	ಆದರೆ (check_and_ಗಣ_terms(terms, p))
		ಹಿಂತಿರುಗಿ -1;

	ಆದರೆ (!strcmp(p, "start")) {
		ರಚನೆ strvec argv = STRVEC_INIT;
		ಪೂರ್ಣಾಂಕ res;
		sq_dequote_ಗಮ್ಯ_strvec(rev, &argv);
		res = bisect_ಆರಂಭ(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		ಹಿಂತಿರುಗಿ res;
	}

	ಆದರೆ (one_ದ(p, terms->ಪದ_good,
	   terms->ಪದ_bad, "skip", ಖಾಲಿ))
		ಹಿಂತಿರುಗಿ bisect_write(p, rev, terms, 0);

	ಆದರೆ (!strcmp(p, "terms")) {
		ರಚನೆ strvec argv = STRVEC_INIT;
		ಪೂರ್ಣಾಂಕ res;
		sq_dequote_ಗಮ್ಯ_strvec(rev, &argv);
		res = bisect_terms(terms, argv.nr == 1 ? argv.v[0] : ಖಾಲಿ);
		strvec_clear(&argv);
		ಹಿಂತಿರುಗಿ res;
	}
	error(_("'%s'?? what are you talking about?"), p);

	ಹಿಂತಿರುಗಿ -1;
}

ಸ್ಥಿರ ಎಣಿಕೆ bisect_error bisect_replay(ರಚನೆ bisect_terms *terms, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename)
{
	FILE *fp = ಖಾಲಿ;
	ಎಣಿಕೆ bisect_error res = BISECT_OK;
	ರಚನೆ strbuf ಸಾಲು = STRBUF_INIT;

	ಆದರೆ (is_ಖಾಲಿಗಳು_or_missing_file(filename))
		ಹಿಂತಿರುಗಿ error(_("cannot read file '%s' for replaying"), filename);

	ಆದರೆ (bisect_clean_state())
		ಹಿಂತಿರುಗಿ BISECT_FAILED;

	fp = fopen(filename, "r");
	ಆದರೆ (!fp)
		ಹಿಂತಿರುಗಿ BISECT_FAILED;

	ಆಗ ((strbuf_getline(&ಸಾಲು, fp) != EOF) && !res)
		res = process_replay_ಸಾಲು(terms, &ಸಾಲು);

	strbuf_release(&ಸಾಲು);
	fclose(fp);

	ಆದರೆ (res)
		ಹಿಂತಿರುಗಿ BISECT_FAILED;

	ಹಿಂತಿರುಗಿ bisect_auto_ಮುಂದಿನ(terms, ಖಾಲಿ);
}

ಸ್ಥಿರ ಎಣಿಕೆ bisect_error bisect_skip(ರಚನೆ bisect_terms *terms, ಪೂರ್ಣಾಂಕ argc,
				     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv)
{
	ಪೂರ್ಣಾಂಕ i;
	ಎಣಿಕೆ bisect_error res;
	ರಚನೆ strvec argv_state = STRVEC_INIT;

	strvec_ತಳ್ಳು(&argv_state, "skip");

	ಗಾಗಿ (i = 0; i < argc; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dotdot = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(argv[i], "..");

		ಆದರೆ (dotdot) {
			ರಚನೆ rev_info revs;
			ರಚನೆ commit *commit;

			repo_init_revisions(the_repository, &revs, ಖಾಲಿ);
			ಸಿದ್ಧತೆ_revisions(2, argv + i - 1, &revs, ಖಾಲಿ);

			ಆದರೆ (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			ಆಗ ((commit = get_revision(&revs)) != ಖಾಲಿ)
				strvec_ತಳ್ಳು(&argv_state,
						oid_ಗಮ್ಯ_hex(&commit->object.oid));

			reset_revision_walk();
			release_revisions(&revs);
		} ಇಲ್ಲದಿದ್ದರೆ {
			strvec_ತಳ್ಳು(&argv_state, argv[i]);
		}
	}
	res = bisect_state(terms, argv_state.nr, argv_state.v);

	strvec_clear(&argv_state);
	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bisect_visualize(ರಚನೆ bisect_terms *terms, ಪೂರ್ಣಾಂಕ argc,
			    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv)
{
	ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;
	ರಚನೆ strbuf sb = STRBUF_INIT;

	ಆದರೆ (bisect_ಮುಂದಿನ_check(terms, ಖಾಲಿ) != 0)
		ಹಿಂತಿರುಗಿ BISECT_FAILED;

	cmd.no_stdin = 1;
	ಆದರೆ (!argc) {
		ಆದರೆ ((getenv("DISPLAY") || getenv("SESSIONNAME") || getenv("MSYSTEM") ||
		     getenv("SECURITYSESSIONID")) && exists_ಒಳ_ಮಾರ್ಗ("gitk")) {
			strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "gitk");
		} ಇಲ್ಲದಿದ್ದರೆ {
			strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "log");
			cmd.git_cmd = 1;
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (argv[0][0] == '-') {
			strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "log");
			cmd.git_cmd = 1;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp(argv[0], "tig") && !starts_with(argv[0], "git"))
			cmd.git_cmd = 1;

		strvec_pushv(&cmd.ವಾದಗಳು, argv);
	}

	strvec_pushl(&cmd.ವಾದಗಳು, "--bisect", "--", ಖಾಲಿ);

	strbuf_read_file(&sb, git_ಮಾರ್ಗ_bisect_ಹೆಸರುಗಳು(), 0);
	sq_dequote_ಗಮ್ಯ_strvec(sb.buf, &cmd.ವಾದಗಳು);
	strbuf_release(&sb);

	ಹಿಂತಿರುಗಿ ಸತತ_command(&cmd);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_ಮೊದಲ_good(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	oidcpy(cb_ದತ್ತಾಂಶ, ref->oid);
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_bisect_ಸತತ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *command)
{
	ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;

	ಬರೆ(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, command);
	ಹಿಂತಿರುಗಿ ಸತತ_command(&cmd);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ verify_good(ಸ್ಥಿರಾಂಕ ರಚನೆ bisect_terms *terms, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *command)
{
	ಪೂರ್ಣಾಂಕ rc;
	ಎಣಿಕೆ bisect_error res;
	ರಚನೆ object_id good_rev;
	ರಚನೆ object_id current_rev;
	ಅಕ್ಷರ *good_glob = xstrfmt("%s-*", terms->ಪದ_good);
	ಪೂರ್ಣಾಂಕ no_checkout = refs_ref_exists(get_main_ref_store(the_repository),
					  "BISECT_HEAD");
	ರಚನೆ refs_for_each_ref_options opts = {
		.ನಮೂನೆ = good_glob,
		.ಪೂರ್ವಪ್ರತ್ಯಯ = "refs/bisect/",
		.trim_ಪೂರ್ವಪ್ರತ್ಯಯ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_ಮೊದಲ_good, &good_rev, &opts);
	ಬಿಡುಗಡೆ(good_glob);

	ಆದರೆ (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		ಹಿಂತಿರುಗಿ -1;

	res = bisect_checkout(&good_rev, no_checkout);
	ಆದರೆ (res != BISECT_OK)
		ಹಿಂತಿರುಗಿ -1;

	rc = do_bisect_ಸತತ(command);

	res = bisect_checkout(&current_rev, no_checkout);
	ಆದರೆ (res != BISECT_OK)
		ಹಿಂತಿರುಗಿ -1;

	ಹಿಂತಿರುಗಿ rc;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bisect_ಸತತ(ರಚನೆ bisect_terms *terms, ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv)
{
	ಪೂರ್ಣಾಂಕ res = BISECT_OK;
	ರಚನೆ strbuf command = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *reset_when_ಸಿಕ್ಕಿತು_arg;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *new_state;
	ಪೂರ್ಣಾಂಕ temporary_stdout_fd, saved_stdout;
	ಪೂರ್ಣಾಂಕ is_ಮೊದಲ_ಸತತ = 1;
	ಎಣಿಕೆ reset_when_ಸಿಕ್ಕಿತು_ಬಹುಲಕ reset_when_ಸಿಕ್ಕಿತು = RESET_WHEN_ಸಿಕ್ಕಿತು_NONE;

	ಆದರೆ (bisect_ಮುಂದಿನ_check(terms, ಖಾಲಿ))
		ಹಿಂತಿರುಗಿ BISECT_FAILED;

	ಆದರೆ (argc && !strcmp(argv[0], "--reset-when-found")) {
		reset_when_ಸಿಕ್ಕಿತು = RESET_WHEN_ಸಿಕ್ಕಿತು_ಗಮ್ಯ_ORIGINAL;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (argc && skip_ಪೂರ್ವಪ್ರತ್ಯಯ(argv[0], "--reset-when-found=",
				    &reset_when_ಸಿಕ್ಕಿತು_arg)) {
		ಆದರೆ (parse_reset_when_ಸಿಕ್ಕಿತು(reset_when_ಸಿಕ್ಕಿತು_arg,
					   &reset_when_ಸಿಕ್ಕಿತು))
			ಹಿಂತಿರುಗಿ BISECT_FAILED;
	}

	ಆದರೆ (reset_when_ಸಿಕ್ಕಿತು != RESET_WHEN_ಸಿಕ್ಕಿತು_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		ಹಿಂತಿರುಗಿ error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	ಆದರೆ (reset_when_ಸಿಕ್ಕಿತು != RESET_WHEN_ಸಿಕ್ಕಿತು_NONE) {
		write_file(git_ಮಾರ್ಗ_bisect_reset_when_ಸಿಕ್ಕಿತು(), "%s\n",
			   reset_when_ಸಿಕ್ಕಿತು_ಬಹುಲಕ_name(reset_when_ಸಿಕ್ಕಿತು));
		argc--;
		argv++;
	}

	ಆದರೆ (!argc) {
		error(_("bisect run failed: no command provided."));
		ಹಿಂತಿರುಗಿ BISECT_FAILED;
	}

	sq_quote_argv(&command, argv);
	strbuf_ltrim(&command);
	ಆಗ (1) {
		res = do_bisect_ಸತತ(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		ಆದರೆ (is_ಮೊದಲ_ಸತತ && (res == 126 || res == 127)) {
			ಪೂರ್ಣಾಂಕ rc = verify_good(terms, command.buf);
			is_ಮೊದಲ_ಸತತ = 0;
			ಆದರೆ (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->ಪದ_good);
				res = BISECT_FAILED;
				ನಿಲ್ಲಿಸು;
			}
			ಆದರೆ (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->ಪದ_good);
				res = BISECT_FAILED;
				ನಿಲ್ಲಿಸು;
			}
		}

		ಆದರೆ (res < 0 || 128 <= res) {
			error(_("bisect run failed: exit code %d from"
				" %s is < 0 or >= 128"), res, command.buf);
			ನಿಲ್ಲಿಸು;
		}

		ಆದರೆ (res == 125)
			new_state = "skip";
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!res)
			new_state = terms->ಪದ_good;
		ಇಲ್ಲದಿದ್ದರೆ
			new_state = terms->ಪದ_bad;

		temporary_stdout_fd = open(git_ಮಾರ್ಗ_bisect_ಸತತ(), O_CREAT | O_WRONLY | O_TRUNC, 0666);

		ಆದರೆ (temporary_stdout_fd < 0) {
			res = error_errno(_("cannot open file '%s' for writing"), git_ಮಾರ್ಗ_bisect_ಸತತ());
			ನಿಲ್ಲಿಸು;
		}

		fflush(stdout);
		saved_stdout = dup(1);
		ಆದರೆ (saved_stdout < 0 ||
		    dup2(temporary_stdout_fd, 1) < 0) {
			res = error_errno(_("could not duplicate stdout"));
			ಆದರೆ (saved_stdout >= 0)
				close(saved_stdout);
			close(temporary_stdout_fd);
			ನಿಲ್ಲಿಸು;
		}

		res = bisect_state(terms, 1, &new_state);

		fflush(stdout);
		dup2(saved_stdout, 1);
		close(saved_stdout);
		close(temporary_stdout_fd);

		print_file_ಗಮ್ಯ_stdout(git_ಮಾರ್ಗ_bisect_ಸತತ());

		ಆದರೆ (res == BISECT_ONLY_SKIPPED_ಎಡ)
			error(_("bisect run cannot continue any more"));
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (res == BISECT_ಆಂತರಿಕ_SUCCESS_ವಿಲೀನ_ತಳ) {
			puts(_("bisect run success"));
			res = BISECT_OK;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (res == BISECT_ಆಂತರಿಕ_SUCCESS_1_ST_BAD_ಸಿಕ್ಕಿತು) {
			ಬರೆ(_("bisect found first '%s' commit\n"), terms->ಪದ_bad);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (res) {
			error(_("bisect run failed: 'git bisect %s'"
				" exited with error code %d"), new_state, res);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಮುಂದುವರಿ;
		}
		ನಿಲ್ಲಿಸು;
	}

	strbuf_release(&command);
	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_bisect__reset(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ ಬಳಸದ,
			     ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ res;

	ಆದರೆ (argc > 1)
		ಹಿಂತಿರುಗಿ error(_("'%s' requires either no argument or a commit"),
			     "git bisect reset");
	res = bisect_reset(argc ? argv[0] : ಖಾಲಿ, false);
	ಆದರೆ (res)
		ಹಿಂತಿರುಗಿ res;
	ಹಿಂತಿರುಗಿ bisect_clean_state();
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_bisect__terms(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ ಬಳಸದ,
			     ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ res;
	ರಚನೆ bisect_terms terms = { 0 };

	ಆದರೆ (argc > 1)
		ಹಿಂತಿರುಗಿ error(_("'%s' requires 0 or 1 argument"),
			     "git bisect terms");
	res = bisect_terms(&terms, argc == 1 ? argv[0] : ಖಾಲಿ);
	free_terms(&terms);
	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_bisect_ಆರಂಭ(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ ಬಳಸದ,
			     ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ res;
	ರಚನೆ bisect_terms terms = { 0 };

	ಗಣ_terms(&terms, "bad", "good");
	res = bisect_ಆರಂಭ(&terms, argc, argv);
	free_terms(&terms);
	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_bisect_ಮುಂದಿನ(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv ಬಳಸದ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			    ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ res;
	ರಚನೆ bisect_terms terms = { 0 };

	ಆದರೆ (argc)
		ಹಿಂತಿರುಗಿ error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	ಆದರೆ (get_terms(&terms, 1))
		ಹಿಂತಿರುಗಿ error(_("no terms defined"));
	res = bisect_ಮುಂದಿನ(&terms, ಪೂರ್ವಪ್ರತ್ಯಯ);
	free_terms(&terms);
	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_bisect__log(ಪೂರ್ಣಾಂಕ argc ಬಳಸದ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv ಬಳಸದ,
			   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ ಬಳಸದ,
			   ರಚನೆ repository *repo ಬಳಸದ)
{
	ಹಿಂತಿರುಗಿ bisect_log();
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_bisect__replay(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ ಬಳಸದ,
			      ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ res;
	ರಚನೆ bisect_terms terms = { 0 };

	ಆದರೆ (argc != 1)
		ಹಿಂತಿರುಗಿ error(_("no logfile given"));
	ಗಣ_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_bisect__skip(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ ಬಳಸದ,
			    ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ res;
	ರಚನೆ bisect_terms terms = { 0 };

	ಗಣ_terms(&terms, "bad", "good");
	ಆದರೆ (get_terms(&terms, 1))
		ಹಿಂತಿರುಗಿ error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_bisect__visualize(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ ಬಳಸದ,
				 ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ res;
	ರಚನೆ bisect_terms terms = { 0 };

	ಆದರೆ (get_terms(&terms, 1))
		ಹಿಂತಿರುಗಿ error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_bisect_ಸತತ(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ ಬಳಸದ,
			   ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ res;
	ರಚನೆ bisect_terms terms = { 0 };

	ಆದರೆ (!argc)
		ಹಿಂತಿರುಗಿ error(_("'%s' failed: no command provided."), "git bisect run");
	ಆದರೆ (get_terms(&terms, 1))
		ಹಿಂತಿರುಗಿ error(_("no terms defined"));
	res = bisect_ಸತತ(&terms, argc, argv);
	free_terms(&terms);
	ಹಿಂತಿರುಗಿ res;
}

ಪೂರ್ಣಾಂಕ cmd_bisect(ಪೂರ್ಣಾಂಕ argc,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	       ರಚನೆ repository *repo)
{
	ಪೂರ್ಣಾಂಕ res = 0;
	parse_opt_subcommand_fn *fn = ಖಾಲಿ;
	ರಚನೆ option options[] = {
		OPT_SUBCOMMAND("reset", &fn, cmd_bisect__reset),
		OPT_SUBCOMMAND("terms", &fn, cmd_bisect__terms),
		OPT_SUBCOMMAND("start", &fn, cmd_bisect_ಆರಂಭ),
		OPT_SUBCOMMAND("next", &fn, cmd_bisect_ಮುಂದಿನ),
		OPT_SUBCOMMAND("log", &fn, cmd_bisect__log),
		OPT_SUBCOMMAND("replay", &fn, cmd_bisect__replay),
		OPT_SUBCOMMAND("skip", &fn, cmd_bisect__skip),
		OPT_SUBCOMMAND("visualize", &fn, cmd_bisect__visualize),
		OPT_SUBCOMMAND("view", &fn, cmd_bisect__visualize),
		OPT_SUBCOMMAND("run", &fn, cmd_bisect_ಸತತ),
		OPT_ಕೊನೆ()
	};
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, git_bisect_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);

	ಆದರೆ (!fn) {
		ರಚನೆ bisect_terms terms = { 0 };

		ಆದರೆ (!argc)
			usage_ಸಂದೇಶ_opt(_("need a command"), git_bisect_usage, options);

		ಆದರೆ (!strcmp(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		ಗಣ_terms(&terms, "bad", "good");
		ಆದರೆ (get_terms(&terms, 1))
			ಹಿಂತಿರುಗಿ error(_("no terms defined"));
		ಆದರೆ (check_and_ಗಣ_terms(&terms, argv[0]) ||
		    !one_ದ(argv[0], terms.ಪದ_good, terms.ಪದ_bad, ಖಾಲಿ))
			usage_ಸಂದೇಶ_optf(_("unknown command: '%s'"), git_bisect_usage,
				       options, argv[0]);
		ಇಲ್ಲದಿದ್ದರೆ
			res = bisect_state(&terms, argc, argv);
		free_terms(&terms);
	} ಇಲ್ಲದಿದ್ದರೆ {
		argc--;
		argv++;
		res = fn(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, repo);
	}

	ಆದರೆ (res == BISECT_ಆಂತರಿಕ_SUCCESS_1_ST_BAD_ಸಿಕ್ಕಿತು) {
		ಎಣಿಕೆ reset_when_ಸಿಕ್ಕಿತು_ಬಹುಲಕ ಬಹುಲಕ;

		ಆದರೆ (read_reset_when_ಸಿಕ್ಕಿತು(&ಬಹುಲಕ))
			res = BISECT_FAILED;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಬಹುಲಕ != RESET_WHEN_ಸಿಕ್ಕಿತು_NONE &&
			 bisect_reset_when_ಸಿಕ್ಕಿತು(ಬಹುಲಕ))
			res = BISECT_FAILED;
	}

	ಹಿಂತಿರುಗಿ is_bisect_success(res) ? 0 : -res;
}
