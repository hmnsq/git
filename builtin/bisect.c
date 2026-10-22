#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "copy.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "bisect.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strvec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oid-array.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "prompt.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"

ਸਥਿਰ GIT_ਰਸਤਾ_FUNC(git_ਰਸਤਾ_bisect_terms, "BISECT_TERMS")
ਸਥਿਰ GIT_ਰਸਤਾ_FUNC(git_ਰਸਤਾ_bisect_ancestors_ok, "BISECT_ANCESTORS_OK")
ਸਥਿਰ GIT_ਰਸਤਾ_FUNC(git_ਰਸਤਾ_bisect_ਸ਼ੁਰੂ, "BISECT_START")
ਸਥਿਰ GIT_ਰਸਤਾ_FUNC(git_ਰਸਤਾ_bisect_log, "BISECT_LOG")
ਸਥਿਰ GIT_ਰਸਤਾ_FUNC(git_ਰਸਤਾ_bisect_ਨਾਂ, "BISECT_NAMES")
ਸਥਿਰ GIT_ਰਸਤਾ_FUNC(git_ਰਸਤਾ_bisect_ਪਹਿਲਾ_ਮਾਪਾ, "BISECT_FIRST_PARENT")
ਸਥਿਰ GIT_ਰਸਤਾ_FUNC(git_ਰਸਤਾ_bisect_reset_when_ਮਿਲਿਆ, "BISECT_RESET_WHEN_FOUND")
ਸਥਿਰ GIT_ਰਸਤਾ_FUNC(git_ਰਸਤਾ_bisect_ਲੜੀਵਾਰ, "BISECT_RUN")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_GIT_BISECT_ਸ਼ੁਰੂ_USAGE \
	N_("git bisect start [--term-(bad|new)=<term-new> --term-(good|old)=<term-old>]\n" \
	   "                 [--no-checkout] [--first-parent] [--reset-when-found[=<where>]] [<bad> [<good>...]] [--] [<pathspec>...]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_GIT_BISECT_BAD_USAGE \
	N_("git bisect (bad|new|<term-new>) [<rev>]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_GIT_BISECT_GOOD_USAGE \
	N_("git bisect (good|old|<term-old>) [<rev>...]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_GIT_BISECT_TERMS_USAGE \
	"git bisect terms [--term-(good|old) | --term-(bad|new)]"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_GIT_BISECT_SKIP_USAGE \
	N_("git bisect skip [(<rev>|<range>)...]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_GIT_BISECT_ਅਗਲਾ_USAGE \
	"git bisect next"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_GIT_BISECT_RESET_USAGE \
	N_("git bisect reset [<commit>]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_GIT_BISECT_VISUALIZE_USAGE \
	"git bisect (visualize|view)"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_GIT_BISECT_REPLAY_USAGE \
	N_("git bisect replay <logfile>")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_GIT_BISECT_LOG_USAGE \
	"git bisect log"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_GIT_BISECT_ਲੜੀਵਾਰ_USAGE \
	N_("git bisect run [--reset-when-found[=<where>]] <cmd> [<arg>...]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_GIT_BISECT_HELP_USAGE \
	"git bisect help"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_bisect_usage[] = {
	BUILTIN_GIT_BISECT_ਸ਼ੁਰੂ_USAGE,
	BUILTIN_GIT_BISECT_BAD_USAGE,
	BUILTIN_GIT_BISECT_GOOD_USAGE,
	BUILTIN_GIT_BISECT_TERMS_USAGE,
	BUILTIN_GIT_BISECT_SKIP_USAGE,
	BUILTIN_GIT_BISECT_ਅਗਲਾ_USAGE,
	BUILTIN_GIT_BISECT_RESET_USAGE,
	BUILTIN_GIT_BISECT_VISUALIZE_USAGE,
	BUILTIN_GIT_BISECT_REPLAY_USAGE,
	BUILTIN_GIT_BISECT_LOG_USAGE,
	BUILTIN_GIT_BISECT_ਲੜੀਵਾਰ_USAGE,
	BUILTIN_GIT_BISECT_HELP_USAGE,
	ਨਿਰਾ
};

ਗਿਣਤੀ reset_when_ਮਿਲਿਆ_ਬਹੁਲਕ {
	RESET_WHEN_ਮਿਲਿਆ_NONE,
	RESET_WHEN_ਮਿਲਿਆ_ਮੰਜ਼ਿਲ_ORIGINAL,
	RESET_WHEN_ਮਿਲਿਆ_ਮੰਜ਼ਿਲ_ਮਿਲਿਆ,
};

ਬਣਤਰ ਜੋੜੋ_bisect_ref_ਅੰਕੜੇ {
	ਬਣਤਰ rev_info *revs;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ object_flags;
};

ਬਣਤਰ bisect_terms {
	ਅੱਖਰ *ਪਦ_good;
	ਅੱਖਰ *ਪਦ_bad;
};

ਸਥਿਰ ਖਾਲੀ free_terms(ਬਣਤਰ bisect_terms *terms)
{
	FREE_AND_NULL(terms->ਪਦ_good);
	FREE_AND_NULL(terms->ਪਦ_bad);
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_terms(ਬਣਤਰ bisect_terms *terms, ਅਚਲ ਅੱਖਰ *bad,
		      ਅਚਲ ਅੱਖਰ *good)
{
	ਮੁਕਤ((ਖਾਲੀ *)terms->ਪਦ_good);
	terms->ਪਦ_good = xstrdup(good);
	ਮੁਕਤ((ਖਾਲੀ *)terms->ਪਦ_bad);
	terms->ਪਦ_bad = xstrdup(bad);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ vocab_bad[] = "bad|new";
ਸਥਿਰ ਅਚਲ ਅੱਖਰ vocab_good[] = "good|old";

ਸਥਿਰ ਪੂਰਨ bisect_autostart(ਬਣਤਰ bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
ਆਖਰੀ_ARG_MUST_BE_NULL
ਸਥਿਰ ਪੂਰਨ one_ਦਾ(ਅਚਲ ਅੱਖਰ *ਪਦ, ...)
{
	ਪੂਰਨ res = 0;
	va_list matches;
	ਅਚਲ ਅੱਖਰ *ਮੇਲ;

	va_ਸ਼ੁਰੂ(matches, ਪਦ);
	ਜਦੋਂ (!res && (ਮੇਲ = va_arg(matches, ਅਚਲ ਅੱਖਰ *)))
		res = !strcmp(ਪਦ, ਮੇਲ);
	va_ਸਿਰਾ(matches);

	ਵਾਪਸ res;
}

/*
 * return code BISECT_INTERNAL_SUCCESS_MERGE_BASE
 * and BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND are codes
 * that indicate special success.
 */

ਸਥਿਰ ਪੂਰਨ is_bisect_success(ਗਿਣਤੀ bisect_error res)
{
	ਵਾਪਸ !res ||
		res == BISECT_ਅੰਦਰੂਨੀ_SUCCESS_1_ST_BAD_ਮਿਲਿਆ ||
		res == BISECT_ਅੰਦਰੂਨੀ_SUCCESS_ਮਿਲਾਓ_ਅਧਾਰ;
}

ਸਥਿਰ ਪੂਰਨ write_ਵਿੱਚ_file(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *ਬਹੁਲਕ, ਅਚਲ ਅੱਖਰ *format, va_list ਦਲੀਲਾਂ)
{
	FILE *fp = ਨਿਰਾ;
	ਪੂਰਨ res = 0;

	ਜੇ (strcmp(ਬਹੁਲਕ, "w") && strcmp(ਬਹੁਲਕ, "a"))
		BUG("write-in-file does not support '%s' mode", ਬਹੁਲਕ);
	fp = fopen(ਰਸਤਾ, ਬਹੁਲਕ);
	ਜੇ (!fp)
		ਵਾਪਸ error_errno(_("cannot open file '%s' in mode '%s'"), ਰਸਤਾ, ਬਹੁਲਕ);
	res = vfprintf(fp, format, ਦਲੀਲਾਂ);

	ਜੇ (res < 0) {
		ਪੂਰਨ saved_errno = errno;
		fclose(fp);
		errno = saved_errno;
		ਵਾਪਸ error_errno(_("could not write to file '%s'"), ਰਸਤਾ);
	}

	ਵਾਪਸ fclose(fp);
}

__attribute__((format (ਲਿਖੋ, 2, 3)))
ਸਥਿਰ ਪੂਰਨ write_ਮੰਜ਼ਿਲ_file(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *format, ...)
{
	ਪੂਰਨ res;
	va_list ਦਲੀਲਾਂ;

	va_ਸ਼ੁਰੂ(ਦਲੀਲਾਂ, format);
	res = write_ਵਿੱਚ_file(ਰਸਤਾ, "w", format, ਦਲੀਲਾਂ);
	va_ਸਿਰਾ(ਦਲੀਲਾਂ);

	ਵਾਪਸ res;
}

__attribute__((format (ਲਿਖੋ, 2, 3)))
ਸਥਿਰ ਪੂਰਨ append_ਮੰਜ਼ਿਲ_file(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *format, ...)
{
	ਪੂਰਨ res;
	va_list ਦਲੀਲਾਂ;

	va_ਸ਼ੁਰੂ(ਦਲੀਲਾਂ, format);
	res = write_ਵਿੱਚ_file(ਰਸਤਾ, "a", format, ਦਲੀਲਾਂ);
	va_ਸਿਰਾ(ਦਲੀਲਾਂ);

	ਵਾਪਸ res;
}

ਸਥਿਰ ਪੂਰਨ print_file_ਮੰਜ਼ਿਲ_stdout(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਪੂਰਨ fd = open(ਰਸਤਾ, O_RDONLY);
	ਪੂਰਨ ret = 0;

	ਜੇ (fd < 0)
		ਵਾਪਸ error_errno(_("cannot open file '%s' for reading"), ਰਸਤਾ);
	ਜੇ (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), ਰਸਤਾ);
	close(fd);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ check_ਪਦ_format(ਅਚਲ ਅੱਖਰ *ਪਦ, ਅਚਲ ਅੱਖਰ *orig_ਪਦ)
{
	ਪੂਰਨ res;
	ਅੱਖਰ *new_ਪਦ = xstrfmt("refs/bisect/%s", ਪਦ);

	res = check_refname_format(new_ਪਦ, 0);
	ਮੁਕਤ(new_ਪਦ);

	ਜੇ (res)
		ਵਾਪਸ error(_("'%s' is not a valid term"), ਪਦ);

	ਜੇ (one_ਦਾ(ਪਦ, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", ਨਿਰਾ))
		ਵਾਪਸ error(_("can't use the builtin command '%s' as a term"), ਪਦ);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	ਜੇ ((strcmp(orig_ਪਦ, "bad") && one_ਦਾ(ਪਦ, "bad", "new", ਨਿਰਾ)) ||
		 (strcmp(orig_ਪਦ, "good") && one_ਦਾ(ਪਦ, "good", "old", ਨਿਰਾ)))
		ਵਾਪਸ error(_("can't change the meaning of the term '%s'"), ਪਦ);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ write_terms(ਅਚਲ ਅੱਖਰ *bad, ਅਚਲ ਅੱਖਰ *good)
{
	ਪੂਰਨ res;

	ਜੇ (!strcmp(bad, good))
		ਵਾਪਸ error(_("please use two different terms"));

	ਜੇ (check_ਪਦ_format(bad, "bad") || check_ਪਦ_format(good, "good"))
		ਵਾਪਸ -1;

	res = write_ਮੰਜ਼ਿਲ_file(git_ਰਸਤਾ_bisect_terms(), "%s\n%s\n", bad, good);

	ਵਾਪਸ res;
}

ਸਥਿਰ ਪੂਰਨ bisect_reset(ਅਚਲ ਅੱਖਰ *commit, bool quiet)
{
	ਬਣਤਰ strbuf branch = STRBUF_INIT;

	ਜੇ (!commit) {
		ਜੇ (!strbuf_read_file(&branch, git_ਰਸਤਾ_bisect_ਸ਼ੁਰੂ(), 0))
			ਲਿਖੋ(_("We are not bisecting.\n"));
		ਨਹੀਂਤਾਂ
			strbuf_rtrim(&branch);
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ object_id oid;

		ਜੇ (repo_get_oid_commit(the_repository, commit, &oid))
			ਵਾਪਸ error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	ਜੇ (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.ਦਲੀਲਾਂ, "checkout", "--ignore-other-worktrees", ਨਿਰਾ);
		ਜੇ (quiet)
			strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--quiet");
		strvec_pushl(&cmd.ਦਲੀਲਾਂ, branch.buf, "--", ਨਿਰਾ);
		ਜੇ (ਲੜੀਵਾਰ_command(&cmd)) {
			error(_("could not check out original"
				" HEAD '%s'. Try 'git bisect"
				" reset <commit>'."), branch.buf);
			strbuf_release(&branch);
			ਵਾਪਸ -1;
		}
	}

	strbuf_release(&branch);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ parse_reset_when_ਮਿਲਿਆ(ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
				  ਗਿਣਤੀ reset_when_ਮਿਲਿਆ_ਬਹੁਲਕ *ਬਹੁਲਕ)
{
	ਜੇ (!strcmp(ਮੁੱਲ, "original"))
		*ਬਹੁਲਕ = RESET_WHEN_ਮਿਲਿਆ_ਮੰਜ਼ਿਲ_ORIGINAL;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(ਮੁੱਲ, "found"))
		*ਬਹੁਲਕ = RESET_WHEN_ਮਿਲਿਆ_ਮੰਜ਼ਿਲ_ਮਿਲਿਆ;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ error(_("invalid value for '--reset-when-found': '%s'"),
			     ਮੁੱਲ);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *reset_when_ਮਿਲਿਆ_ਬਹੁਲਕ_name(ਗਿਣਤੀ reset_when_ਮਿਲਿਆ_ਬਹੁਲਕ ਬਹੁਲਕ)
{
	ਚੋਣ (ਬਹੁਲਕ) {
	ਹਾਲਤ RESET_WHEN_ਮਿਲਿਆ_ਮੰਜ਼ਿਲ_ORIGINAL:
		ਵਾਪਸ "original";
	ਹਾਲਤ RESET_WHEN_ਮਿਲਿਆ_ਮੰਜ਼ਿਲ_ਮਿਲਿਆ:
		ਵਾਪਸ "found";
	ਹਾਲਤ RESET_WHEN_ਮਿਲਿਆ_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", ਬਹੁਲਕ);
}

ਸਥਿਰ ਪੂਰਨ read_reset_when_ਮਿਲਿਆ(ਗਿਣਤੀ reset_when_ਮਿਲਿਆ_ਬਹੁਲਕ *ਬਹੁਲਕ)
{
	ਬਣਤਰ strbuf ਮੁੱਲ = STRBUF_INIT;
	ਪੂਰਨ res = 0;

	*ਬਹੁਲਕ = RESET_WHEN_ਮਿਲਿਆ_NONE;
	ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_or_missing_file(git_ਰਸਤਾ_bisect_reset_when_ਮਿਲਿਆ()))
		ਵਾਪਸ 0;

	ਜੇ (strbuf_read_file(&ਮੁੱਲ, git_ਰਸਤਾ_bisect_reset_when_ਮਿਲਿਆ(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_ਰਸਤਾ_bisect_reset_when_ਮਿਲਿਆ());
		ਜਾਓ ਬਾਹਰਲਾ;
	}
	strbuf_trim(&ਮੁੱਲ);
	ਜੇ (parse_reset_when_ਮਿਲਿਆ(ਮੁੱਲ.buf, ਬਹੁਲਕ))
		res = -1;

ਬਾਹਰਲਾ:
	strbuf_release(&ਮੁੱਲ);
	ਵਾਪਸ res;
}

ਸਥਿਰ ਪੂਰਨ bisect_reset_when_ਮਿਲਿਆ(ਗਿਣਤੀ reset_when_ਮਿਲਿਆ_ਬਹੁਲਕ ਬਹੁਲਕ)
{
	ਬਣਤਰ bisect_terms terms = { 0 };
	ਅੱਖਰ *commit = ਨਿਰਾ;
	ਪੂਰਨ res;

	ਜੇ (ਬਹੁਲਕ == RESET_WHEN_ਮਿਲਿਆ_ਮੰਜ਼ਿਲ_ਮਿਲਿਆ) {
		read_bisect_terms(&terms.ਪਦ_bad, &terms.ਪਦ_good);
		commit = xstrfmt("refs/bisect/%s", terms.ਪਦ_bad);
	} ਨਹੀਂਤਾਂ ਜੇ (ਬਹੁਲਕ == RESET_WHEN_ਮਿਲਿਆ_NONE) {
		BUG("automatic reset requested without a reset mode");
	}

	res = bisect_reset(commit, true);
	ਜੇ (!res)
		res = bisect_clean_state();

	ਮੁਕਤ(commit);
	free_terms(&terms);
	ਵਾਪਸ res;
}

ਸਥਿਰ ਖਾਲੀ log_commit(FILE *fp,
		       ਅਚਲ ਅੱਖਰ *fmt, ਅਚਲ ਅੱਖਰ *state,
		       ਬਣਤਰ commit *commit)
{
	ਬਣਤਰ pretty_print_context pp = {0};
	ਬਣਤਰ strbuf commit_ਸੁਨੇਹਾ = STRBUF_INIT;
	ਅੱਖਰ *label = xstrfmt(fmt, state);

	repo_format_commit_ਸੁਨੇਹਾ(the_repository, commit, "%s", &commit_ਸੁਨੇਹਾ,
				   &pp);

	fprintf(fp, "# %s: [%s] %s\n", label, oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid),
		commit_ਸੁਨੇਹਾ.buf);

	strbuf_release(&commit_ਸੁਨੇਹਾ);
	ਮੁਕਤ(label);
}

ਸਥਿਰ ਪੂਰਨ bisect_write(ਅਚਲ ਅੱਖਰ *state, ਅਚਲ ਅੱਖਰ *rev,
			ਅਚਲ ਬਣਤਰ bisect_terms *terms, ਪੂਰਨ nolog)
{
	ਬਣਤਰ strbuf ਟੈਗ = STRBUF_INIT;
	ਬਣਤਰ object_id oid;
	ਬਣਤਰ commit *commit;
	FILE *fp = ਨਿਰਾ;
	ਪੂਰਨ res = 0;

	ਜੇ (!strcmp(state, terms->ਪਦ_bad)) {
		strbuf_addf(&ਟੈਗ, "refs/bisect/%s", state);
	} ਨਹੀਂਤਾਂ ਜੇ (one_ਦਾ(state, terms->ਪਦ_good, "skip", ਨਿਰਾ)) {
		strbuf_addf(&ਟੈਗ, "refs/bisect/%s-%s", state, rev);
	} ਨਹੀਂਤਾਂ {
		res = error(_("Bad bisect_write argument: %s"), state);
		ਜਾਓ ਅੰਤ;
	}

	ਜੇ (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		ਜਾਓ ਅੰਤ;
	}

	ਜੇ (refs_update_ref(get_main_ref_store(the_repository), ਨਿਰਾ, ਟੈਗ.buf, &oid, ਨਿਰਾ, 0,
			    UPDATE_REFS_ਸੁਨੇਹਾ_ON_ERR)) {
		res = -1;
		ਜਾਓ ਅੰਤ;
	}

	fp = fopen(git_ਰਸਤਾ_bisect_log(), "a");
	ਜੇ (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_ਰਸਤਾ_bisect_log());
		ਜਾਓ ਅੰਤ;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	ਜੇ (!nolog)
		fprintf(fp, "git bisect %s %s\n", state, rev);

ਅੰਤ:
	ਜੇ (fp)
		fclose(fp);
	strbuf_release(&ਟੈਗ);
	ਵਾਪਸ res;
}

ਸਥਿਰ ਪੂਰਨ check_and_ਸੈੱਟ_terms(ਬਣਤਰ bisect_terms *terms, ਅਚਲ ਅੱਖਰ *cmd)
{
	ਪੂਰਨ has_ਪਦ_file = !is_ਖਾਲੀ_ਥਾਵਾਂ_or_missing_file(git_ਰਸਤਾ_bisect_terms());

	ਜੇ (one_ਦਾ(cmd, "skip", "start", "terms", ਨਿਰਾ))
		ਵਾਪਸ 0;

	ਜੇ (has_ਪਦ_file && strcmp(cmd, terms->ਪਦ_bad) &&
	    strcmp(cmd, terms->ਪਦ_good))
		ਵਾਪਸ error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->ਪਦ_bad,
				terms->ਪਦ_good);

	ਜੇ (!has_ਪਦ_file) {
		ਜੇ (one_ਦਾ(cmd, "bad", "good", ਨਿਰਾ)) {
			ਸੈੱਟ_terms(terms, "bad", "good");
			ਵਾਪਸ write_terms(terms->ਪਦ_bad, terms->ਪਦ_good);
		}
		ਜੇ (one_ਦਾ(cmd, "new", "old", ਨਿਰਾ)) {
			ਸੈੱਟ_terms(terms, "new", "old");
			ਵਾਪਸ write_terms(terms->ਪਦ_bad, terms->ਪਦ_good);
		}
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ inc_nr(ਅਚਲ ਬਣਤਰ reference *ref ਅਣਵਰਤਿਆ, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *nr = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *)cb_ਅੰਕੜੇ;
	(*nr)++;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ need_bad_and_good_revision_warning[] =
	N_("You need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

ਸਥਿਰ ਅਚਲ ਅੱਖਰ need_bisect_ਸ਼ੁਰੂ_warning[] =
	N_("You need to start by \"git bisect start\".\n"
	   "You then need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

ਸਥਿਰ ਪੂਰਨ decide_ਅਗਲਾ(ਅਚਲ ਬਣਤਰ bisect_terms *terms,
		       ਅਚਲ ਅੱਖਰ *current_ਪਦ, ਪੂਰਨ missing_good,
		       ਪੂਰਨ missing_bad)
{
	ਜੇ (!missing_good && !missing_bad)
		ਵਾਪਸ 0;
	ਜੇ (!current_ਪਦ)
		ਵਾਪਸ -1;

	ਜੇ (missing_good && !missing_bad &&
	    !strcmp(current_ਪਦ, terms->ਪਦ_good)) {
		ਅੱਖਰ *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->ਪਦ_bad);
		ਜੇ (!isatty(0))
			ਵਾਪਸ 0;
		/*
		 * TRANSLATORS: Make sure to include [Y] and [n] in your
		 * translation. The program will only accept English input
		 * at this point.
		 */
		yesno = git_prompt(_("Are you sure [Y/n]? "), PROMPT_ECHO);
		ਜੇ (starts_with(yesno, "N") || starts_with(yesno, "n"))
			ਵਾਪਸ -1;
		ਵਾਪਸ 0;
	}

	ਜੇ (!is_ਖਾਲੀ_ਥਾਵਾਂ_or_missing_file(git_ਰਸਤਾ_bisect_ਸ਼ੁਰੂ()))
		ਵਾਪਸ error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ error(_(need_bisect_ਸ਼ੁਰੂ_warning),
			     vocab_good, vocab_bad, vocab_good, vocab_bad);
}

ਸਥਿਰ ਖਾਲੀ bisect_ਸਥਿਤੀ(ਬਣਤਰ bisect_state *state,
			  ਅਚਲ ਬਣਤਰ bisect_terms *terms)
{
	ਅੱਖਰ *bad_ref = xstrfmt("refs/bisect/%s", terms->ਪਦ_bad);
	ਅੱਖਰ *good_glob = xstrfmt("%s-*", terms->ਪਦ_good);
	ਬਣਤਰ refs_for_each_ref_options opts = {
		.ਨਮੂਨਾ = good_glob,
		.ਅਗੇਤਰ = "refs/bisect/",
		.trim_ਅਗੇਤਰ = ਲੜੀ_ਲੰਬਾਈ("refs/bisect/"),
	};

	ਜੇ (refs_ref_exists(get_main_ref_store(the_repository), bad_ref))
		state->nr_bad = 1;

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      inc_nr, &state->nr_good, &opts);

	ਮੁਕਤ(good_glob);
	ਮੁਕਤ(bad_ref);
}

__attribute__((format (ਲਿਖੋ, 1, 2)))
ਸਥਿਰ ਖਾਲੀ bisect_log_printf(ਅਚਲ ਅੱਖਰ *fmt, ...)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	va_list ap;

	va_ਸ਼ੁਰੂ(ap, fmt);
	strbuf_vaddf(&buf, fmt, ap);
	va_ਸਿਰਾ(ap);

	ਲਿਖੋ("%s", buf.buf);
	append_ਮੰਜ਼ਿਲ_file(git_ਰਸਤਾ_bisect_log(), "# %s", buf.buf);

	strbuf_release(&buf);
}

ਸਥਿਰ ਖਾਲੀ bisect_print_ਸਥਿਤੀ(ਅਚਲ ਬਣਤਰ bisect_terms *terms)
{
	ਬਣਤਰ bisect_state state = { 0 };

	bisect_ਸਥਿਤੀ(&state, terms);

	/* If we had both, we'd already be started, and shouldn't get here. */
	ਜੇ (state.nr_good && state.nr_bad)
		ਵਾਪਸ;

	ਜੇ (!state.nr_good && !state.nr_bad)
		bisect_log_printf(_("status: waiting for both '%s' and '%s' commits\n"),
				  terms->ਪਦ_good, terms->ਪਦ_bad);
	ਨਹੀਂਤਾਂ ਜੇ (state.nr_good)
		bisect_log_printf(Q_("status: waiting for '%s' commit, %d '%s' commit known\n",
				     "status: waiting for '%s' commit, %d '%s' commits known\n",
				     state.nr_good),
				  terms->ਪਦ_bad, state.nr_good, terms->ਪਦ_good);
	ਨਹੀਂਤਾਂ
		bisect_log_printf(_("status: waiting for '%s' commit(s), '%s' commit known\n"),
				  terms->ਪਦ_good, terms->ਪਦ_bad);
}

ਸਥਿਰ ਪੂਰਨ bisect_ਅਗਲਾ_check(ਅਚਲ ਬਣਤਰ bisect_terms *terms,
			     ਅਚਲ ਅੱਖਰ *current_ਪਦ)
{
	ਬਣਤਰ bisect_state state = { 0 };
	bisect_ਸਥਿਤੀ(&state, terms);
	ਵਾਪਸ decide_ਅਗਲਾ(terms, current_ਪਦ, !state.nr_good, !state.nr_bad);
}

ਸਥਿਰ ਪੂਰਨ get_terms(ਬਣਤਰ bisect_terms *terms, ਪੂਰਨ file_missing_is_ok)
{
	ਬਣਤਰ strbuf str = STRBUF_INIT;
	FILE *fp = ਨਿਰਾ;
	ਪੂਰਨ res = 0;

	fp = fopen(git_ਰਸਤਾ_bisect_terms(), "r");
	ਜੇ (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		ਜਾਓ ਅੰਤ;
	}

	free_terms(terms);
	ਜੇ (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		ਜਾਓ ਅੰਤ;
	}
	terms->ਪਦ_bad = strbuf_detach(&str, ਨਿਰਾ);
	ਜੇ (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		FREE_AND_NULL(terms->ਪਦ_bad);
		ਜਾਓ ਅੰਤ;
	}
	terms->ਪਦ_good = strbuf_detach(&str, ਨਿਰਾ);

ਅੰਤ:
	ਜੇ (fp)
		fclose(fp);
	strbuf_release(&str);
	ਵਾਪਸ res;
}

ਸਥਿਰ ਪੂਰਨ bisect_terms(ਬਣਤਰ bisect_terms *terms, ਅਚਲ ਅੱਖਰ *option)
{
	ਜੇ (get_terms(terms, 0))
		ਵਾਪਸ error(_("no terms defined"));

	ਜੇ (!option) {
		ਲਿਖੋ(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->ਪਦ_good, terms->ਪਦ_bad);
		ਵਾਪਸ 0;
	}
	ਜੇ (one_ਦਾ(option, "--term-good", "--term-old", ਨਿਰਾ))
		ਲਿਖੋ("%s\n", terms->ਪਦ_good);
	ਨਹੀਂਤਾਂ ਜੇ (one_ਦਾ(option, "--term-bad", "--term-new", ਨਿਰਾ))
		ਲਿਖੋ("%s\n", terms->ਪਦ_bad);
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ error(_("invalid argument %s for 'git bisect terms'.\n"
			       "Supported options are: "
			       "--term-good|--term-old and "
			       "--term-bad|--term-new."), option);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ bisect_append_log_quoted(ਅਚਲ ਅੱਖਰ **argv)
{
	ਪੂਰਨ res = 0;
	FILE *fp = fopen(git_ਰਸਤਾ_bisect_log(), "a");
	ਬਣਤਰ strbuf orig_ਦਲੀਲਾਂ = STRBUF_INIT;

	ਜੇ (!fp)
		ਵਾਪਸ -1;

	ਜੇ (fprintf(fp, "git bisect start") < 1) {
		res = -1;
		ਜਾਓ ਅੰਤ;
	}

	sq_quote_argv(&orig_ਦਲੀਲਾਂ, argv);
	ਜੇ (fprintf(fp, "%s\n", orig_ਦਲੀਲਾਂ.buf) < 1)
		res = -1;

ਅੰਤ:
	fclose(fp);
	strbuf_release(&orig_ਦਲੀਲਾਂ);
	ਵਾਪਸ res;
}

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_bisect_ref(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cb)
{
	ਬਣਤਰ ਜੋੜੋ_bisect_ref_ਅੰਕੜੇ *ਅੰਕੜੇ = cb;

	ਜੋੜੋ_pending_oid(ਅੰਕੜੇ->revs, ref->name, ref->oid, ਅੰਕੜੇ->object_flags);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ prepare_revs(ਬਣਤਰ bisect_terms *terms, ਬਣਤਰ rev_info *revs)
{
	ਬਣਤਰ refs_for_each_ref_options opts = {
		.ਅਗੇਤਰ = "refs/bisect/",
		.trim_ਅਗੇਤਰ = ਲੜੀ_ਲੰਬਾਈ("refs/bisect/"),
	};
	ਪੂਰਨ res = 0;
	ਬਣਤਰ ਜੋੜੋ_bisect_ref_ਅੰਕੜੇ cb = { revs };
	ਅੱਖਰ *good = xstrfmt("%s-*", terms->ਪਦ_good);

	/*
	 * We cannot use terms->term_bad directly in
	 * for_each_glob_ref_in() and we have to append a '*' to it,
	 * otherwise for_each_glob_ref_in() will append '/' and '*'.
	 */
	ਅੱਖਰ *bad = xstrfmt("%s*", terms->ਪਦ_bad);

	/*
	 * It is important to reset the flags used by revision walks
	 * as the previous call to bisect_next_all() in turn
	 * sets up a revision walk.
	 */
	reset_revision_walk();
	repo_init_revisions(the_repository, revs, ਨਿਰਾ);
	ਤਿਆਰੀ_revisions(0, ਨਿਰਾ, revs, ਨਿਰਾ);

	opts.ਨਮੂਨਾ = bad;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      ਜੋੜੋ_bisect_ref, &cb, &opts);

	cb.object_flags = UNINTERESTING;
	opts.ਨਮੂਨਾ = good;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      ਜੋੜੋ_bisect_ref, &cb, &opts);

	ਜੇ (prepare_revision_walk(revs))
		res = error(_("revision walk setup failed"));

	ਮੁਕਤ(good);
	ਮੁਕਤ(bad);
	ਵਾਪਸ res;
}

ਸਥਿਰ ਪੂਰਨ bisect_skipped_commits(ਬਣਤਰ bisect_terms *terms)
{
	ਪੂਰਨ res;
	FILE *fp = ਨਿਰਾ;
	ਬਣਤਰ rev_info revs;
	ਬਣਤਰ commit *commit;
	ਬਣਤਰ pretty_print_context pp = {0};
	ਬਣਤਰ strbuf commit_name = STRBUF_INIT;

	res = prepare_revs(terms, &revs);
	ਜੇ (res)
		ਵਾਪਸ res;

	fp = fopen(git_ਰਸਤਾ_bisect_log(), "a");
	ਜੇ (!fp)
		ਵਾਪਸ error_errno(_("could not open '%s' for appending"),
				  git_ਰਸਤਾ_bisect_log());

	ਜੇ (fprintf(fp, "# only skipped commits left to test\n") < 0)
		ਵਾਪਸ error_errno(_("failed to write to '%s'"), git_ਰਸਤਾ_bisect_log());

	ਜਦੋਂ ((commit = get_revision(&revs)) != ਨਿਰਾ) {
		strbuf_reset(&commit_name);
		repo_format_commit_ਸੁਨੇਹਾ(the_repository, commit, "%s",
					   &commit_name, &pp);
		fprintf(fp, "# possible first '%s' commit: [%s] %s\n",
			terms->ਪਦ_bad, oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid),
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
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ bisect_successful(ਬਣਤਰ bisect_terms *terms)
{
	ਬਣਤਰ object_id oid;
	ਬਣਤਰ commit *commit;
	ਬਣਤਰ pretty_print_context pp = {0};
	ਬਣਤਰ strbuf commit_name = STRBUF_INIT;
	ਅੱਖਰ *bad_ref = xstrfmt("refs/bisect/%s",terms->ਪਦ_bad);
	ਪੂਰਨ res;

	refs_read_ref(get_main_ref_store(the_repository), bad_ref, &oid);
	commit = lookup_commit_reference_by_name(bad_ref);
	ਜੇ (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		ਮੁਕਤ(bad_ref);
		ਵਾਪਸ BISECT_FAILED;
	}
	repo_format_commit_ਸੁਨੇਹਾ(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_ਮੰਜ਼ਿਲ_file(git_ਰਸਤਾ_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->ਪਦ_bad, oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	ਮੁਕਤ(bad_ref);
	ਵਾਪਸ res;
}

ਸਥਿਰ ਗਿਣਤੀ bisect_error bisect_ਅਗਲਾ(ਬਣਤਰ bisect_terms *terms,
				     ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ)
{
	ਗਿਣਤੀ bisect_error res;

	ਜੇ (bisect_autostart(terms))
		ਵਾਪਸ BISECT_FAILED;

	ਜੇ (bisect_ਅਗਲਾ_check(terms, terms->ਪਦ_good))
		ਵਾਪਸ BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_ਅਗਲਾ_all(the_repository, ਅਗੇਤਰ);

	ਜੇ (res == BISECT_ਅੰਦਰੂਨੀ_SUCCESS_1_ST_BAD_ਮਿਲਿਆ) {
		res = bisect_successful(terms);
		ਵਾਪਸ res ? res : BISECT_ਅੰਦਰੂਨੀ_SUCCESS_1_ST_BAD_ਮਿਲਿਆ;
	} ਨਹੀਂਤਾਂ ਜੇ (res == BISECT_ONLY_SKIPPED_ਖੱਬਾ) {
		res = bisect_skipped_commits(terms);
		ਵਾਪਸ res ? res : BISECT_ONLY_SKIPPED_ਖੱਬਾ;
	}
	ਵਾਪਸ res;
}

ਸਥਿਰ ਗਿਣਤੀ bisect_error bisect_auto_ਅਗਲਾ(ਬਣਤਰ bisect_terms *terms,
					  ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ)
{
	ਜੇ (bisect_ਅਗਲਾ_check(terms, ਨਿਰਾ)) {
		bisect_print_ਸਥਿਤੀ(terms);
		ਵਾਪਸ BISECT_OK;
	}

	ਵਾਪਸ bisect_ਅਗਲਾ(terms, ਅਗੇਤਰ);
}

ਸਥਿਰ ਗਿਣਤੀ bisect_error bisect_ਸ਼ੁਰੂ(ਬਣਤਰ bisect_terms *terms, ਪੂਰਨ argc,
				      ਅਚਲ ਅੱਖਰ **argv)
{
	ਪੂਰਨ no_checkout = 0;
	ਪੂਰਨ ਪਹਿਲਾ_ਮਾਪਾ_only = 0;
	ਪੂਰਨ i, has_double_dash = 0, must_write_terms = 0, bad_ਵੇਖਿਆ = 0;
	ਪੂਰਨ flags, pathspec_pos;
	ਗਿਣਤੀ bisect_error res = BISECT_OK;
	ਬਣਤਰ string_list revs = STRING_LIST_INIT_DUP;
	ਬਣਤਰ string_list ਹਾਲਤਾਂ = STRING_LIST_INIT_DUP;
	ਬਣਤਰ strbuf ਸ਼ੁਰੂ_ਸਿਰ = STRBUF_INIT;
	ਬਣਤਰ strbuf bisect_ਨਾਂ = STRBUF_INIT;
	ਬਣਤਰ object_id ਸਿਰ_oid;
	ਬਣਤਰ object_id oid;
	ਗਿਣਤੀ reset_when_ਮਿਲਿਆ_ਬਹੁਲਕ reset_when_ਮਿਲਿਆ = RESET_WHEN_ਮਿਲਿਆ_NONE;
	ਅਚਲ ਅੱਖਰ *ਸਿਰ;

	ਜੇ (is_bare_repository(the_repository))
		no_checkout = 1;

	/*
	 * Check for one bad and then some good revisions
	 */
	ਲਈ (i = 0; i < argc; i++) {
		ਜੇ (!strcmp(argv[i], "--")) {
			has_double_dash = 1;
			ਰੋਕੋ;
		}
	}

	ਲਈ (i = 0; i < argc; i++) {
		ਅਚਲ ਅੱਖਰ *arg = argv[i];
		ਜੇ (!strcmp(argv[i], "--")) {
			ਰੋਕੋ;
		} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "--no-checkout")) {
			no_checkout = 1;
		} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "--first-parent")) {
			ਪਹਿਲਾ_ਮਾਪਾ_only = 1;
		} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "--reset-when-found")) {
			reset_when_ਮਿਲਿਆ = RESET_WHEN_ਮਿਲਿਆ_ਮੰਜ਼ਿਲ_ORIGINAL;
		} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(arg, "--reset-when-found=", &arg)) {
			ਜੇ (parse_reset_when_ਮਿਲਿਆ(arg, &reset_when_ਮਿਲਿਆ)) {
				res = BISECT_FAILED;
				ਜਾਓ ਅੰਤ;
			}
		} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "--term-good") ||
			 !strcmp(arg, "--term-old")) {
			i++;
			ਜੇ (argc <= i)
				ਵਾਪਸ error(_("'' is not a valid term"));
			must_write_terms = 1;
			ਮੁਕਤ((ਖਾਲੀ *) terms->ਪਦ_good);
			terms->ਪਦ_good = xstrdup(argv[i]);
		} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(arg, "--term-good=", &arg) ||
			   skip_ਅਗੇਤਰ(arg, "--term-old=", &arg)) {
			must_write_terms = 1;
			ਮੁਕਤ((ਖਾਲੀ *) terms->ਪਦ_good);
			terms->ਪਦ_good = xstrdup(arg);
		} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(arg, "--term-bad") ||
			 !strcmp(arg, "--term-new")) {
			i++;
			ਜੇ (argc <= i)
				ਵਾਪਸ error(_("'' is not a valid term"));
			must_write_terms = 1;
			ਮੁਕਤ((ਖਾਲੀ *) terms->ਪਦ_bad);
			terms->ਪਦ_bad = xstrdup(argv[i]);
		} ਨਹੀਂਤਾਂ ਜੇ (skip_ਅਗੇਤਰ(arg, "--term-bad=", &arg) ||
			   skip_ਅਗੇਤਰ(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			ਮੁਕਤ((ਖਾਲੀ *) terms->ਪਦ_bad);
			terms->ਪਦ_bad = xstrdup(arg);
		} ਨਹੀਂਤਾਂ ਜੇ (starts_with(arg, "--")) {
			ਵਾਪਸ error(_("unrecognized option: '%s'"), arg);
		} ਨਹੀਂਤਾਂ ਜੇ (!get_oidf(&oid, "%s^{commit}", arg)) {
			string_list_append(&revs, oid_ਮੰਜ਼ਿਲ_hex(&oid));
		} ਨਹੀਂਤਾਂ ਜੇ (has_double_dash) {
			die(_("'%s' does not appear to be a valid "
			      "revision"), arg);
		} ਨਹੀਂਤਾਂ {
			ਰੋਕੋ;
		}
	}
	ਜੇ (reset_when_ਮਿਲਿਆ != RESET_WHEN_ਮਿਲਿਆ_NONE && no_checkout) {
		res = error(_("options '%s' and '%s' cannot be used together"),
			    "--reset-when-found", "--no-checkout");
		ਜਾਓ ਅੰਤ;
	}
	pathspec_pos = i;

	/*
	 * The user ran "git bisect start <sha1> <sha1>", hence did not
	 * explicitly specify the terms, but we are already starting to
	 * set references named with the default terms, and won't be able
	 * to change afterwards.
	 */
	ਜੇ (revs.nr)
		must_write_terms = 1;
	ਲਈ (i = 0; i < revs.nr; i++) {
		ਜੇ (bad_ਵੇਖਿਆ) {
			string_list_append(&ਹਾਲਤਾਂ, terms->ਪਦ_good);
		} ਨਹੀਂਤਾਂ {
			bad_ਵੇਖਿਆ = 1;
			string_list_append(&ਹਾਲਤਾਂ, terms->ਪਦ_bad);
		}
	}

	/*
	 * Verify HEAD
	 */
	ਸਿਰ = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
				       "HEAD", 0, &ਸਿਰ_oid, &flags);
	ਜੇ (!ਸਿਰ) {
		ਜੇ (repo_get_oid(the_repository, "HEAD", &ਸਿਰ_oid))
			ਵਾਪਸ error(_("bad HEAD - I need a HEAD"));
		ਸਿਰ = "HEAD";
	}

	/*
	 * Check if we are bisecting
	 */
	ਜੇ (!is_ਖਾਲੀ_ਥਾਵਾਂ_or_missing_file(git_ਰਸਤਾ_bisect_ਸ਼ੁਰੂ())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&ਸ਼ੁਰੂ_ਸਿਰ, git_ਰਸਤਾ_bisect_ਸ਼ੁਰੂ(), 0);
		strbuf_trim(&ਸ਼ੁਰੂ_ਸਿਰ);
		ਜੇ (!no_checkout) {
			ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.ਦਲੀਲਾਂ, "checkout", ਸ਼ੁਰੂ_ਸਿਰ.buf,
				     "--", ਨਿਰਾ);
			ਜੇ (ਲੜੀਵਾਰ_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       ਸ਼ੁਰੂ_ਸਿਰ.buf);
				ਜਾਓ ਅੰਤ;
			}
		}
	} ਨਹੀਂਤਾਂ {
		/* Get the rev from where we start. */
		ਜੇ (!repo_get_oid(the_repository, ਸਿਰ, &ਸਿਰ_oid) &&
		    !starts_with(ਸਿਰ, "refs/heads/")) {
			strbuf_reset(&ਸ਼ੁਰੂ_ਸਿਰ);
			strbuf_ਜੋੜੋ_oid_hex(&ਸ਼ੁਰੂ_ਸਿਰ, &ਸਿਰ_oid);
		} ਨਹੀਂਤਾਂ ਜੇ (!repo_get_oid(the_repository, ਸਿਰ, &ਸਿਰ_oid) &&
			   skip_ਅਗੇਤਰ(ਸਿਰ, "refs/heads/", &ਸਿਰ)) {
			strbuf_addstr(&ਸ਼ੁਰੂ_ਸਿਰ, ਸਿਰ);
		} ਨਹੀਂਤਾਂ {
			ਵਾਪਸ error(_("bad HEAD - strange symbolic ref"));
		}
	}

	/*
	 * Get rid of any old bisect state.
	 */
	ਜੇ (bisect_clean_state())
		ਵਾਪਸ BISECT_FAILED;

	/*
	 * Write new start state
	 */
	write_file(git_ਰਸਤਾ_bisect_ਸ਼ੁਰੂ(), "%s\n", ਸ਼ੁਰੂ_ਸਿਰ.buf);

	ਜੇ (ਪਹਿਲਾ_ਮਾਪਾ_only)
		write_file(git_ਰਸਤਾ_bisect_ਪਹਿਲਾ_ਮਾਪਾ(), "\n");

	ਜੇ (reset_when_ਮਿਲਿਆ != RESET_WHEN_ਮਿਲਿਆ_NONE)
		write_file(git_ਰਸਤਾ_bisect_reset_when_ਮਿਲਿਆ(), "%s\n",
			   reset_when_ਮਿਲਿਆ_ਬਹੁਲਕ_name(reset_when_ਮਿਲਿਆ));

	ਜੇ (no_checkout) {
		ਜੇ (repo_get_oid(the_repository, ਸ਼ੁਰੂ_ਸਿਰ.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), ਸ਼ੁਰੂ_ਸਿਰ.buf);
			ਜਾਓ ਅੰਤ;
		}
		ਜੇ (refs_update_ref(get_main_ref_store(the_repository), ਨਿਰਾ, "BISECT_HEAD", &oid, ਨਿਰਾ, 0,
				    UPDATE_REFS_ਸੁਨੇਹਾ_ON_ERR)) {
			res = BISECT_FAILED;
			ਜਾਓ ਅੰਤ;
		}
	}

	ਜੇ (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_ਨਾਂ, argv + pathspec_pos);
	write_file(git_ਰਸਤਾ_bisect_ਨਾਂ(), "%s\n", bisect_ਨਾਂ.buf);

	ਲਈ (i = 0; i < ਹਾਲਤਾਂ.nr; i++)
		ਜੇ (bisect_write(ਹਾਲਤਾਂ.ਚੀਜ਼ਾਂ[i].string,
				 revs.ਚੀਜ਼ਾਂ[i].string, terms, 1)) {
			res = BISECT_FAILED;
			ਜਾਓ ਅੰਤ;
		}

	ਜੇ (must_write_terms && write_terms(terms->ਪਦ_bad,
					    terms->ਪਦ_good)) {
		res = BISECT_FAILED;
		ਜਾਓ ਅੰਤ;
	}

	res = bisect_append_log_quoted(argv);
	ਜੇ (res)
		res = BISECT_FAILED;

ਅੰਤ:
	string_list_clear(&revs, 0);
	string_list_clear(&ਹਾਲਤਾਂ, 0);
	strbuf_release(&ਸ਼ੁਰੂ_ਸਿਰ);
	strbuf_release(&bisect_ਨਾਂ);
	ਜੇ (res)
		ਵਾਪਸ res;

	res = bisect_auto_ਅਗਲਾ(terms, ਨਿਰਾ);
	ਜੇ (!is_bisect_success(res))
		bisect_clean_state();
	ਵਾਪਸ res;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ file_is_not_ਖਾਲੀ_ਥਾਵਾਂ(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਵਾਪਸ !is_ਖਾਲੀ_ਥਾਵਾਂ_or_missing_file(ਰਸਤਾ);
}

ਸਥਿਰ ਪੂਰਨ bisect_autostart(ਬਣਤਰ bisect_terms *terms)
{
	ਪੂਰਨ res;
	ਅਚਲ ਅੱਖਰ *yesno;

	ਜੇ (file_is_not_ਖਾਲੀ_ਥਾਵਾਂ(git_ਰਸਤਾ_bisect_ਸ਼ੁਰੂ()))
		ਵਾਪਸ 0;

	fprintf_ln(stderr, _("You need to start by \"git bisect "
			  "start\"\n"));

	ਜੇ (!isatty(STDIN_FILENO))
		ਵਾਪਸ -1;

	/*
	 * TRANSLATORS: Make sure to include [Y] and [n] in your
	 * translation. The program will only accept English input
	 * at this point.
	 */
	yesno = git_prompt(_("Do you want me to do it for you "
			     "[Y/n]? "), PROMPT_ECHO);
	res = tolower(*yesno) == 'n' ?
		-1 : bisect_ਸ਼ੁਰੂ(terms, 0, ਖਾਲੀ_ਥਾਵਾਂ_strvec);

	ਵਾਪਸ res;
}

ਸਥਿਰ ਗਿਣਤੀ bisect_error bisect_state(ਬਣਤਰ bisect_terms *terms, ਪੂਰਨ argc,
				      ਅਚਲ ਅੱਖਰ **argv)
{
	ਅਚਲ ਅੱਖਰ *state;
	ਪੂਰਨ i, verify_expected = 1;
	ਬਣਤਰ object_id oid, expected;
	ਬਣਤਰ oid_array revs = OID_ARRAY_INIT;

	ਜੇ (!argc)
		ਵਾਪਸ error(_("Please call `--bisect-state` with at least one argument"));

	ਜੇ (bisect_autostart(terms))
		ਵਾਪਸ BISECT_FAILED;

	state = argv[0];
	ਜੇ (check_and_ਸੈੱਟ_terms(terms, state) ||
	    !one_ਦਾ(state, terms->ਪਦ_good, terms->ਪਦ_bad, "skip", ਨਿਰਾ))
		ਵਾਪਸ BISECT_FAILED;

	argv++;
	argc--;
	ਜੇ (argc > 1 && !strcmp(state, terms->ਪਦ_bad))
		ਵਾਪਸ error(_("'git bisect %s' can take only one argument."), terms->ਪਦ_bad);

	ਜੇ (argc == 0) {
		ਅਚਲ ਅੱਖਰ *ਸਿਰ = "BISECT_HEAD";
		ਗਿਣਤੀ get_oid_ਨਤੀਜਾ res_ਸਿਰ = repo_get_oid(the_repository,
							    ਸਿਰ, &oid);

		ਜੇ (res_ਸਿਰ == MISSING_OBJECT) {
			ਸਿਰ = "HEAD";
			res_ਸਿਰ = repo_get_oid(the_repository, ਸਿਰ, &oid);
		}

		ਜੇ (res_ਸਿਰ)
			error(_("Bad rev input: %s"), ਸਿਰ);
		oid_array_append(&revs, &oid);
	}

	/*
	 * All input revs must be checked before executing bisect_write()
	 * to discard junk revs.
	 */

	ਲਈ (; argc; argc--, argv++) {
		ਬਣਤਰ commit *commit;

		ਜੇ (repo_get_oid(the_repository, *argv, &oid)){
			error(_("Bad rev input: %s"), *argv);
			oid_array_clear(&revs);
			ਵਾਪਸ BISECT_FAILED;
		}

		commit = lookup_commit_reference(the_repository, &oid);
		ਜੇ (!commit)
			die(_("Bad rev input (not a commit): %s"), *argv);

		oid_array_append(&revs, &commit->object.oid);
	}

	ਜੇ (refs_read_ref(get_main_ref_store(the_repository), "BISECT_EXPECTED_REV", &expected))
		verify_expected = 0; /* Ignore invalid file contents */

	ਲਈ (i = 0; i < revs.nr; i++) {
		ਜੇ (bisect_write(state, oid_ਮੰਜ਼ਿਲ_hex(&revs.oid[i]), terms, 0)) {
			oid_array_clear(&revs);
			ਵਾਪਸ BISECT_FAILED;
		}
		ਜੇ (verify_expected && !oideq(&revs.oid[i], &expected)) {
			unlink_or_warn(git_ਰਸਤਾ_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					ਨਿਰਾ, "BISECT_EXPECTED_REV", ਨਿਰਾ,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	ਵਾਪਸ bisect_auto_ਅਗਲਾ(terms, ਨਿਰਾ);
}

ਸਥਿਰ ਗਿਣਤੀ bisect_error bisect_log(ਖਾਲੀ)
{
	ਪੂਰਨ fd, ਸਥਿਤੀ;
	ਅਚਲ ਅੱਖਰ* filename = git_ਰਸਤਾ_bisect_log();

	ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_or_missing_file(filename))
		ਵਾਪਸ error(_("We are not bisecting."));

	fd = open(filename, O_RDONLY);
	ਜੇ (fd < 0)
		ਵਾਪਸ BISECT_FAILED;

	ਸਥਿਤੀ = copy_fd(fd, STDOUT_FILENO);
	close(fd);
	ਵਾਪਸ ਸਥਿਤੀ ? BISECT_FAILED : BISECT_OK;
}

ਸਥਿਰ ਪੂਰਨ process_replay_ਸਤਰ(ਬਣਤਰ bisect_terms *terms, ਬਣਤਰ strbuf *ਸਤਰ)
{
	ਅਚਲ ਅੱਖਰ *p = ਸਤਰ->buf + strspn(ਸਤਰ->buf, " \t");
	ਅੱਖਰ *ਸ਼ਬਦ_ਸਿਰਾ, *rev;

	ਜੇ ((!skip_ਅਗੇਤਰ(p, "git bisect", &p) &&
	!skip_ਅਗੇਤਰ(p, "git-bisect", &p)) || !ਖਾਲੀ_ਥਾਂ_ਹੈ(*p))
		ਵਾਪਸ 0;
	p += strspn(p, " \t");

	ਸ਼ਬਦ_ਸਿਰਾ = (ਅੱਖਰ *)p + strcspn(p, " \t");
	rev = ਸ਼ਬਦ_ਸਿਰਾ + strspn(ਸ਼ਬਦ_ਸਿਰਾ, " \t");
	*ਸ਼ਬਦ_ਸਿਰਾ = '\0'; /* NUL-terminate the word */

	ਜੇ (get_terms(terms, 1))
		ਵਾਪਸ error(_("no terms defined"));
	ਜੇ (check_and_ਸੈੱਟ_terms(terms, p))
		ਵਾਪਸ -1;

	ਜੇ (!strcmp(p, "start")) {
		ਬਣਤਰ strvec argv = STRVEC_INIT;
		ਪੂਰਨ res;
		sq_dequote_ਮੰਜ਼ਿਲ_strvec(rev, &argv);
		res = bisect_ਸ਼ੁਰੂ(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		ਵਾਪਸ res;
	}

	ਜੇ (one_ਦਾ(p, terms->ਪਦ_good,
	   terms->ਪਦ_bad, "skip", ਨਿਰਾ))
		ਵਾਪਸ bisect_write(p, rev, terms, 0);

	ਜੇ (!strcmp(p, "terms")) {
		ਬਣਤਰ strvec argv = STRVEC_INIT;
		ਪੂਰਨ res;
		sq_dequote_ਮੰਜ਼ਿਲ_strvec(rev, &argv);
		res = bisect_terms(terms, argv.nr == 1 ? argv.v[0] : ਨਿਰਾ);
		strvec_clear(&argv);
		ਵਾਪਸ res;
	}
	error(_("'%s'?? what are you talking about?"), p);

	ਵਾਪਸ -1;
}

ਸਥਿਰ ਗਿਣਤੀ bisect_error bisect_replay(ਬਣਤਰ bisect_terms *terms, ਅਚਲ ਅੱਖਰ *filename)
{
	FILE *fp = ਨਿਰਾ;
	ਗਿਣਤੀ bisect_error res = BISECT_OK;
	ਬਣਤਰ strbuf ਸਤਰ = STRBUF_INIT;

	ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_or_missing_file(filename))
		ਵਾਪਸ error(_("cannot read file '%s' for replaying"), filename);

	ਜੇ (bisect_clean_state())
		ਵਾਪਸ BISECT_FAILED;

	fp = fopen(filename, "r");
	ਜੇ (!fp)
		ਵਾਪਸ BISECT_FAILED;

	ਜਦੋਂ ((strbuf_getline(&ਸਤਰ, fp) != EOF) && !res)
		res = process_replay_ਸਤਰ(terms, &ਸਤਰ);

	strbuf_release(&ਸਤਰ);
	fclose(fp);

	ਜੇ (res)
		ਵਾਪਸ BISECT_FAILED;

	ਵਾਪਸ bisect_auto_ਅਗਲਾ(terms, ਨਿਰਾ);
}

ਸਥਿਰ ਗਿਣਤੀ bisect_error bisect_skip(ਬਣਤਰ bisect_terms *terms, ਪੂਰਨ argc,
				     ਅਚਲ ਅੱਖਰ **argv)
{
	ਪੂਰਨ i;
	ਗਿਣਤੀ bisect_error res;
	ਬਣਤਰ strvec argv_state = STRVEC_INIT;

	strvec_ਧੱਕੋ(&argv_state, "skip");

	ਲਈ (i = 0; i < argc; i++) {
		ਅਚਲ ਅੱਖਰ *dotdot = ਲੜੀ_ਖੋਜ(argv[i], "..");

		ਜੇ (dotdot) {
			ਬਣਤਰ rev_info revs;
			ਬਣਤਰ commit *commit;

			repo_init_revisions(the_repository, &revs, ਨਿਰਾ);
			ਤਿਆਰੀ_revisions(2, argv + i - 1, &revs, ਨਿਰਾ);

			ਜੇ (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			ਜਦੋਂ ((commit = get_revision(&revs)) != ਨਿਰਾ)
				strvec_ਧੱਕੋ(&argv_state,
						oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid));

			reset_revision_walk();
			release_revisions(&revs);
		} ਨਹੀਂਤਾਂ {
			strvec_ਧੱਕੋ(&argv_state, argv[i]);
		}
	}
	res = bisect_state(terms, argv_state.nr, argv_state.v);

	strvec_clear(&argv_state);
	ਵਾਪਸ res;
}

ਸਥਿਰ ਪੂਰਨ bisect_visualize(ਬਣਤਰ bisect_terms *terms, ਪੂਰਨ argc,
			    ਅਚਲ ਅੱਖਰ **argv)
{
	ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ strbuf sb = STRBUF_INIT;

	ਜੇ (bisect_ਅਗਲਾ_check(terms, ਨਿਰਾ) != 0)
		ਵਾਪਸ BISECT_FAILED;

	cmd.no_stdin = 1;
	ਜੇ (!argc) {
		ਜੇ ((getenv("DISPLAY") || getenv("SESSIONNAME") || getenv("MSYSTEM") ||
		     getenv("SECURITYSESSIONID")) && exists_ਵਿੱਚ_ਰਸਤਾ("gitk")) {
			strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "gitk");
		} ਨਹੀਂਤਾਂ {
			strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "log");
			cmd.git_cmd = 1;
		}
	} ਨਹੀਂਤਾਂ {
		ਜੇ (argv[0][0] == '-') {
			strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "log");
			cmd.git_cmd = 1;
		} ਨਹੀਂਤਾਂ ਜੇ (strcmp(argv[0], "tig") && !starts_with(argv[0], "git"))
			cmd.git_cmd = 1;

		strvec_pushv(&cmd.ਦਲੀਲਾਂ, argv);
	}

	strvec_pushl(&cmd.ਦਲੀਲਾਂ, "--bisect", "--", ਨਿਰਾ);

	strbuf_read_file(&sb, git_ਰਸਤਾ_bisect_ਨਾਂ(), 0);
	sq_dequote_ਮੰਜ਼ਿਲ_strvec(sb.buf, &cmd.ਦਲੀਲਾਂ);
	strbuf_release(&sb);

	ਵਾਪਸ ਲੜੀਵਾਰ_command(&cmd);
}

ਸਥਿਰ ਪੂਰਨ get_ਪਹਿਲਾ_good(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	oidcpy(cb_ਅੰਕੜੇ, ref->oid);
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ do_bisect_ਲੜੀਵਾਰ(ਅਚਲ ਅੱਖਰ *command)
{
	ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;

	ਲਿਖੋ(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, command);
	ਵਾਪਸ ਲੜੀਵਾਰ_command(&cmd);
}

ਸਥਿਰ ਪੂਰਨ verify_good(ਅਚਲ ਬਣਤਰ bisect_terms *terms, ਅਚਲ ਅੱਖਰ *command)
{
	ਪੂਰਨ rc;
	ਗਿਣਤੀ bisect_error res;
	ਬਣਤਰ object_id good_rev;
	ਬਣਤਰ object_id current_rev;
	ਅੱਖਰ *good_glob = xstrfmt("%s-*", terms->ਪਦ_good);
	ਪੂਰਨ no_checkout = refs_ref_exists(get_main_ref_store(the_repository),
					  "BISECT_HEAD");
	ਬਣਤਰ refs_for_each_ref_options opts = {
		.ਨਮੂਨਾ = good_glob,
		.ਅਗੇਤਰ = "refs/bisect/",
		.trim_ਅਗੇਤਰ = ਲੜੀ_ਲੰਬਾਈ("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_ਪਹਿਲਾ_good, &good_rev, &opts);
	ਮੁਕਤ(good_glob);

	ਜੇ (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		ਵਾਪਸ -1;

	res = bisect_checkout(&good_rev, no_checkout);
	ਜੇ (res != BISECT_OK)
		ਵਾਪਸ -1;

	rc = do_bisect_ਲੜੀਵਾਰ(command);

	res = bisect_checkout(&current_rev, no_checkout);
	ਜੇ (res != BISECT_OK)
		ਵਾਪਸ -1;

	ਵਾਪਸ rc;
}

ਸਥਿਰ ਪੂਰਨ bisect_ਲੜੀਵਾਰ(ਬਣਤਰ bisect_terms *terms, ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv)
{
	ਪੂਰਨ res = BISECT_OK;
	ਬਣਤਰ strbuf command = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *reset_when_ਮਿਲਿਆ_arg;
	ਅਚਲ ਅੱਖਰ *new_state;
	ਪੂਰਨ temporary_stdout_fd, saved_stdout;
	ਪੂਰਨ is_ਪਹਿਲਾ_ਲੜੀਵਾਰ = 1;
	ਗਿਣਤੀ reset_when_ਮਿਲਿਆ_ਬਹੁਲਕ reset_when_ਮਿਲਿਆ = RESET_WHEN_ਮਿਲਿਆ_NONE;

	ਜੇ (bisect_ਅਗਲਾ_check(terms, ਨਿਰਾ))
		ਵਾਪਸ BISECT_FAILED;

	ਜੇ (argc && !strcmp(argv[0], "--reset-when-found")) {
		reset_when_ਮਿਲਿਆ = RESET_WHEN_ਮਿਲਿਆ_ਮੰਜ਼ਿਲ_ORIGINAL;
	} ਨਹੀਂਤਾਂ ਜੇ (argc && skip_ਅਗੇਤਰ(argv[0], "--reset-when-found=",
				    &reset_when_ਮਿਲਿਆ_arg)) {
		ਜੇ (parse_reset_when_ਮਿਲਿਆ(reset_when_ਮਿਲਿਆ_arg,
					   &reset_when_ਮਿਲਿਆ))
			ਵਾਪਸ BISECT_FAILED;
	}

	ਜੇ (reset_when_ਮਿਲਿਆ != RESET_WHEN_ਮਿਲਿਆ_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		ਵਾਪਸ error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	ਜੇ (reset_when_ਮਿਲਿਆ != RESET_WHEN_ਮਿਲਿਆ_NONE) {
		write_file(git_ਰਸਤਾ_bisect_reset_when_ਮਿਲਿਆ(), "%s\n",
			   reset_when_ਮਿਲਿਆ_ਬਹੁਲਕ_name(reset_when_ਮਿਲਿਆ));
		argc--;
		argv++;
	}

	ਜੇ (!argc) {
		error(_("bisect run failed: no command provided."));
		ਵਾਪਸ BISECT_FAILED;
	}

	sq_quote_argv(&command, argv);
	strbuf_ltrim(&command);
	ਜਦੋਂ (1) {
		res = do_bisect_ਲੜੀਵਾਰ(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		ਜੇ (is_ਪਹਿਲਾ_ਲੜੀਵਾਰ && (res == 126 || res == 127)) {
			ਪੂਰਨ rc = verify_good(terms, command.buf);
			is_ਪਹਿਲਾ_ਲੜੀਵਾਰ = 0;
			ਜੇ (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->ਪਦ_good);
				res = BISECT_FAILED;
				ਰੋਕੋ;
			}
			ਜੇ (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->ਪਦ_good);
				res = BISECT_FAILED;
				ਰੋਕੋ;
			}
		}

		ਜੇ (res < 0 || 128 <= res) {
			error(_("bisect run failed: exit code %d from"
				" %s is < 0 or >= 128"), res, command.buf);
			ਰੋਕੋ;
		}

		ਜੇ (res == 125)
			new_state = "skip";
		ਨਹੀਂਤਾਂ ਜੇ (!res)
			new_state = terms->ਪਦ_good;
		ਨਹੀਂਤਾਂ
			new_state = terms->ਪਦ_bad;

		temporary_stdout_fd = open(git_ਰਸਤਾ_bisect_ਲੜੀਵਾਰ(), O_CREAT | O_WRONLY | O_TRUNC, 0666);

		ਜੇ (temporary_stdout_fd < 0) {
			res = error_errno(_("cannot open file '%s' for writing"), git_ਰਸਤਾ_bisect_ਲੜੀਵਾਰ());
			ਰੋਕੋ;
		}

		fflush(stdout);
		saved_stdout = dup(1);
		ਜੇ (saved_stdout < 0 ||
		    dup2(temporary_stdout_fd, 1) < 0) {
			res = error_errno(_("could not duplicate stdout"));
			ਜੇ (saved_stdout >= 0)
				close(saved_stdout);
			close(temporary_stdout_fd);
			ਰੋਕੋ;
		}

		res = bisect_state(terms, 1, &new_state);

		fflush(stdout);
		dup2(saved_stdout, 1);
		close(saved_stdout);
		close(temporary_stdout_fd);

		print_file_ਮੰਜ਼ਿਲ_stdout(git_ਰਸਤਾ_bisect_ਲੜੀਵਾਰ());

		ਜੇ (res == BISECT_ONLY_SKIPPED_ਖੱਬਾ)
			error(_("bisect run cannot continue any more"));
		ਨਹੀਂਤਾਂ ਜੇ (res == BISECT_ਅੰਦਰੂਨੀ_SUCCESS_ਮਿਲਾਓ_ਅਧਾਰ) {
			puts(_("bisect run success"));
			res = BISECT_OK;
		} ਨਹੀਂਤਾਂ ਜੇ (res == BISECT_ਅੰਦਰੂਨੀ_SUCCESS_1_ST_BAD_ਮਿਲਿਆ) {
			ਲਿਖੋ(_("bisect found first '%s' commit\n"), terms->ਪਦ_bad);
		} ਨਹੀਂਤਾਂ ਜੇ (res) {
			error(_("bisect run failed: 'git bisect %s'"
				" exited with error code %d"), new_state, res);
		} ਨਹੀਂਤਾਂ {
			ਜਾਰੀ;
		}
		ਰੋਕੋ;
	}

	strbuf_release(&command);
	ਵਾਪਸ res;
}

ਸਥਿਰ ਪੂਰਨ cmd_bisect__reset(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ ਅਣਵਰਤਿਆ,
			     ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ res;

	ਜੇ (argc > 1)
		ਵਾਪਸ error(_("'%s' requires either no argument or a commit"),
			     "git bisect reset");
	res = bisect_reset(argc ? argv[0] : ਨਿਰਾ, false);
	ਜੇ (res)
		ਵਾਪਸ res;
	ਵਾਪਸ bisect_clean_state();
}

ਸਥਿਰ ਪੂਰਨ cmd_bisect__terms(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ ਅਣਵਰਤਿਆ,
			     ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ res;
	ਬਣਤਰ bisect_terms terms = { 0 };

	ਜੇ (argc > 1)
		ਵਾਪਸ error(_("'%s' requires 0 or 1 argument"),
			     "git bisect terms");
	res = bisect_terms(&terms, argc == 1 ? argv[0] : ਨਿਰਾ);
	free_terms(&terms);
	ਵਾਪਸ res;
}

ਸਥਿਰ ਪੂਰਨ cmd_bisect_ਸ਼ੁਰੂ(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ ਅਣਵਰਤਿਆ,
			     ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ res;
	ਬਣਤਰ bisect_terms terms = { 0 };

	ਸੈੱਟ_terms(&terms, "bad", "good");
	res = bisect_ਸ਼ੁਰੂ(&terms, argc, argv);
	free_terms(&terms);
	ਵਾਪਸ res;
}

ਸਥਿਰ ਪੂਰਨ cmd_bisect_ਅਗਲਾ(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv ਅਣਵਰਤਿਆ, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			    ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ res;
	ਬਣਤਰ bisect_terms terms = { 0 };

	ਜੇ (argc)
		ਵਾਪਸ error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	ਜੇ (get_terms(&terms, 1))
		ਵਾਪਸ error(_("no terms defined"));
	res = bisect_ਅਗਲਾ(&terms, ਅਗੇਤਰ);
	free_terms(&terms);
	ਵਾਪਸ res;
}

ਸਥਿਰ ਪੂਰਨ cmd_bisect__log(ਪੂਰਨ argc ਅਣਵਰਤਿਆ, ਅਚਲ ਅੱਖਰ **argv ਅਣਵਰਤਿਆ,
			   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ ਅਣਵਰਤਿਆ,
			   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਵਾਪਸ bisect_log();
}

ਸਥਿਰ ਪੂਰਨ cmd_bisect__replay(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ ਅਣਵਰਤਿਆ,
			      ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ res;
	ਬਣਤਰ bisect_terms terms = { 0 };

	ਜੇ (argc != 1)
		ਵਾਪਸ error(_("no logfile given"));
	ਸੈੱਟ_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	ਵਾਪਸ res;
}

ਸਥਿਰ ਪੂਰਨ cmd_bisect__skip(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ ਅਣਵਰਤਿਆ,
			    ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ res;
	ਬਣਤਰ bisect_terms terms = { 0 };

	ਸੈੱਟ_terms(&terms, "bad", "good");
	ਜੇ (get_terms(&terms, 1))
		ਵਾਪਸ error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	ਵਾਪਸ res;
}

ਸਥਿਰ ਪੂਰਨ cmd_bisect__visualize(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ ਅਣਵਰਤਿਆ,
				 ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ res;
	ਬਣਤਰ bisect_terms terms = { 0 };

	ਜੇ (get_terms(&terms, 1))
		ਵਾਪਸ error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	ਵਾਪਸ res;
}

ਸਥਿਰ ਪੂਰਨ cmd_bisect_ਲੜੀਵਾਰ(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ ਅਣਵਰਤਿਆ,
			   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ res;
	ਬਣਤਰ bisect_terms terms = { 0 };

	ਜੇ (!argc)
		ਵਾਪਸ error(_("'%s' failed: no command provided."), "git bisect run");
	ਜੇ (get_terms(&terms, 1))
		ਵਾਪਸ error(_("no terms defined"));
	res = bisect_ਲੜੀਵਾਰ(&terms, argc, argv);
	free_terms(&terms);
	ਵਾਪਸ res;
}

ਪੂਰਨ cmd_bisect(ਪੂਰਨ argc,
	       ਅਚਲ ਅੱਖਰ **argv,
	       ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	       ਬਣਤਰ repository *repo)
{
	ਪੂਰਨ res = 0;
	parse_opt_subcommand_fn *fn = ਨਿਰਾ;
	ਬਣਤਰ option options[] = {
		OPT_SUBCOMMAND("reset", &fn, cmd_bisect__reset),
		OPT_SUBCOMMAND("terms", &fn, cmd_bisect__terms),
		OPT_SUBCOMMAND("start", &fn, cmd_bisect_ਸ਼ੁਰੂ),
		OPT_SUBCOMMAND("next", &fn, cmd_bisect_ਅਗਲਾ),
		OPT_SUBCOMMAND("log", &fn, cmd_bisect__log),
		OPT_SUBCOMMAND("replay", &fn, cmd_bisect__replay),
		OPT_SUBCOMMAND("skip", &fn, cmd_bisect__skip),
		OPT_SUBCOMMAND("visualize", &fn, cmd_bisect__visualize),
		OPT_SUBCOMMAND("view", &fn, cmd_bisect__visualize),
		OPT_SUBCOMMAND("run", &fn, cmd_bisect_ਲੜੀਵਾਰ),
		OPT_ਸਿਰਾ()
	};
	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, git_bisect_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);

	ਜੇ (!fn) {
		ਬਣਤਰ bisect_terms terms = { 0 };

		ਜੇ (!argc)
			usage_ਸੁਨੇਹਾ_opt(_("need a command"), git_bisect_usage, options);

		ਜੇ (!strcmp(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		ਸੈੱਟ_terms(&terms, "bad", "good");
		ਜੇ (get_terms(&terms, 1))
			ਵਾਪਸ error(_("no terms defined"));
		ਜੇ (check_and_ਸੈੱਟ_terms(&terms, argv[0]) ||
		    !one_ਦਾ(argv[0], terms.ਪਦ_good, terms.ਪਦ_bad, ਨਿਰਾ))
			usage_ਸੁਨੇਹਾ_optf(_("unknown command: '%s'"), git_bisect_usage,
				       options, argv[0]);
		ਨਹੀਂਤਾਂ
			res = bisect_state(&terms, argc, argv);
		free_terms(&terms);
	} ਨਹੀਂਤਾਂ {
		argc--;
		argv++;
		res = fn(argc, argv, ਅਗੇਤਰ, repo);
	}

	ਜੇ (res == BISECT_ਅੰਦਰੂਨੀ_SUCCESS_1_ST_BAD_ਮਿਲਿਆ) {
		ਗਿਣਤੀ reset_when_ਮਿਲਿਆ_ਬਹੁਲਕ ਬਹੁਲਕ;

		ਜੇ (read_reset_when_ਮਿਲਿਆ(&ਬਹੁਲਕ))
			res = BISECT_FAILED;
		ਨਹੀਂਤਾਂ ਜੇ (ਬਹੁਲਕ != RESET_WHEN_ਮਿਲਿਆ_NONE &&
			 bisect_reset_when_ਮਿਲਿਆ(ਬਹੁਲਕ))
			res = BISECT_FAILED;
	}

	ਵਾਪਸ is_bisect_success(res) ? 0 : -res;
}
