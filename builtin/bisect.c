#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#وصف_ڪريو DISABLE_SIGN_COMPARE_WARNINGS

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

ساكن GIT_رستو_FUNC(git_رستو_bisect_terms, "BISECT_TERMS")
ساكن GIT_رستو_FUNC(git_رستو_bisect_ancestors_ok, "BISECT_ANCESTORS_OK")
ساكن GIT_رستو_FUNC(git_رستو_bisect_شروعات, "BISECT_START")
ساكن GIT_رستو_FUNC(git_رستو_bisect_log, "BISECT_LOG")
ساكن GIT_رستو_FUNC(git_رستو_bisect_نالا, "BISECT_NAMES")
ساكن GIT_رستو_FUNC(git_رستو_bisect_پهريون_والد, "BISECT_FIRST_PARENT")
ساكن GIT_رستو_FUNC(git_رستو_bisect_reset_when_مليو, "BISECT_RESET_WHEN_FOUND")
ساكن GIT_رستو_FUNC(git_رستو_bisect_لاڳيتو, "BISECT_RUN")

#وصف_ڪريو BUILTIN_GIT_BISECT_شروعات_USAGE \
	N_("git bisect start [--term-(bad|new)=<term-new> --term-(good|old)=<term-old>]\n" \
	   "                 [--no-checkout] [--first-parent] [--reset-when-found[=<where>]] [<bad> [<good>...]] [--] [<pathspec>...]")
#وصف_ڪريو BUILTIN_GIT_BISECT_BAD_USAGE \
	N_("git bisect (bad|new|<term-new>) [<rev>]")
#وصف_ڪريو BUILTIN_GIT_BISECT_GOOD_USAGE \
	N_("git bisect (good|old|<term-old>) [<rev>...]")
#وصف_ڪريو BUILTIN_GIT_BISECT_TERMS_USAGE \
	"git bisect terms [--term-(good|old) | --term-(bad|new)]"
#وصف_ڪريو BUILTIN_GIT_BISECT_SKIP_USAGE \
	N_("git bisect skip [(<rev>|<range>)...]")
#وصف_ڪريو BUILTIN_GIT_BISECT_ايندڙ_USAGE \
	"git bisect next"
#وصف_ڪريو BUILTIN_GIT_BISECT_RESET_USAGE \
	N_("git bisect reset [<commit>]")
#وصف_ڪريو BUILTIN_GIT_BISECT_VISUALIZE_USAGE \
	"git bisect (visualize|view)"
#وصف_ڪريو BUILTIN_GIT_BISECT_REPLAY_USAGE \
	N_("git bisect replay <logfile>")
#وصف_ڪريو BUILTIN_GIT_BISECT_LOG_USAGE \
	"git bisect log"
#وصف_ڪريو BUILTIN_GIT_BISECT_لاڳيتو_USAGE \
	N_("git bisect run [--reset-when-found[=<where>]] <cmd> [<arg>...]")
#وصف_ڪريو BUILTIN_GIT_BISECT_HELP_USAGE \
	"git bisect help"

ساكن ثابت حرف * ثابت git_bisect_usage[] = {
	BUILTIN_GIT_BISECT_شروعات_USAGE,
	BUILTIN_GIT_BISECT_BAD_USAGE,
	BUILTIN_GIT_BISECT_GOOD_USAGE,
	BUILTIN_GIT_BISECT_TERMS_USAGE,
	BUILTIN_GIT_BISECT_SKIP_USAGE,
	BUILTIN_GIT_BISECT_ايندڙ_USAGE,
	BUILTIN_GIT_BISECT_RESET_USAGE,
	BUILTIN_GIT_BISECT_VISUALIZE_USAGE,
	BUILTIN_GIT_BISECT_REPLAY_USAGE,
	BUILTIN_GIT_BISECT_LOG_USAGE,
	BUILTIN_GIT_BISECT_لاڳيتو_USAGE,
	BUILTIN_GIT_BISECT_HELP_USAGE,
	عدم
};

تعداد reset_when_مليو_گهڻائي {
	RESET_WHEN_مليو_NONE,
	RESET_WHEN_مليو_منزل_ORIGINAL,
	RESET_WHEN_مليو_منزل_مليو,
};

بنية جوڙيو_bisect_ref_معلومات {
	بنية rev_info *revs;
	بنانشان صحيح object_flags;
};

بنية bisect_terms {
	حرف *رڪن_good;
	حرف *رڪن_bad;
};

ساكن فراغ free_terms(بنية bisect_terms *terms)
{
	FREE_AND_NULL(terms->رڪن_good);
	FREE_AND_NULL(terms->رڪن_bad);
}

ساكن فراغ سيٽ_terms(بنية bisect_terms *terms, ثابت حرف *bad,
		      ثابت حرف *good)
{
	حرر((فراغ *)terms->رڪن_good);
	terms->رڪن_good = xstrdup(good);
	حرر((فراغ *)terms->رڪن_bad);
	terms->رڪن_bad = xstrdup(bad);
}

ساكن ثابت حرف vocab_bad[] = "bad|new";
ساكن ثابت حرف vocab_good[] = "good|old";

ساكن صحيح bisect_autostart(بنية bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
آخري_ARG_MUST_BE_NULL
ساكن صحيح one_جو(ثابت حرف *رڪن, ...)
{
	صحيح res = 0;
	va_list matches;
	ثابت حرف *ميلاپ;

	va_شروعات(matches, رڪن);
	طالما (!res && (ميلاپ = va_arg(matches, ثابت حرف *)))
		res = !strcmp(رڪن, ميلاپ);
	va_پڇاڙي(matches);

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
		res == BISECT_اندروني_SUCCESS_1_ST_BAD_مليو ||
		res == BISECT_اندروني_SUCCESS_ملايو_بنياد;
}

ساكن صحيح write_اندريون_file(ثابت حرف *رستو, ثابت حرف *گهڻائي, ثابت حرف *format, va_list دليلون)
{
	FILE *fp = عدم;
	صحيح res = 0;

	إذا (strcmp(گهڻائي, "w") && strcmp(گهڻائي, "a"))
		BUG("write-in-file does not support '%s' mode", گهڻائي);
	fp = fopen(رستو, گهڻائي);
	إذا (!fp)
		أرجع error_errno(_("cannot open file '%s' in mode '%s'"), رستو, گهڻائي);
	res = vfprintf(fp, format, دليلون);

	إذا (res < 0) {
		صحيح saved_errno = errno;
		fclose(fp);
		errno = saved_errno;
		أرجع error_errno(_("could not write to file '%s'"), رستو);
	}

	أرجع fclose(fp);
}

__attribute__((format (لکو, 2, 3)))
ساكن صحيح write_منزل_file(ثابت حرف *رستو, ثابت حرف *format, ...)
{
	صحيح res;
	va_list دليلون;

	va_شروعات(دليلون, format);
	res = write_اندريون_file(رستو, "w", format, دليلون);
	va_پڇاڙي(دليلون);

	أرجع res;
}

__attribute__((format (لکو, 2, 3)))
ساكن صحيح append_منزل_file(ثابت حرف *رستو, ثابت حرف *format, ...)
{
	صحيح res;
	va_list دليلون;

	va_شروعات(دليلون, format);
	res = write_اندريون_file(رستو, "a", format, دليلون);
	va_پڇاڙي(دليلون);

	أرجع res;
}

ساكن صحيح print_file_منزل_stdout(ثابت حرف *رستو)
{
	صحيح fd = open(رستو, O_RDONLY);
	صحيح ret = 0;

	إذا (fd < 0)
		أرجع error_errno(_("cannot open file '%s' for reading"), رستو);
	إذا (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), رستو);
	close(fd);
	أرجع ret;
}

ساكن صحيح check_رڪن_format(ثابت حرف *رڪن, ثابت حرف *orig_رڪن)
{
	صحيح res;
	حرف *new_رڪن = xstrfmt("refs/bisect/%s", رڪن);

	res = check_refname_format(new_رڪن, 0);
	حرر(new_رڪن);

	إذا (res)
		أرجع error(_("'%s' is not a valid term"), رڪن);

	إذا (one_جو(رڪن, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", عدم))
		أرجع error(_("can't use the builtin command '%s' as a term"), رڪن);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	إذا ((strcmp(orig_رڪن, "bad") && one_جو(رڪن, "bad", "new", عدم)) ||
		 (strcmp(orig_رڪن, "good") && one_جو(رڪن, "good", "old", عدم)))
		أرجع error(_("can't change the meaning of the term '%s'"), رڪن);

	أرجع 0;
}

ساكن صحيح write_terms(ثابت حرف *bad, ثابت حرف *good)
{
	صحيح res;

	إذا (!strcmp(bad, good))
		أرجع error(_("please use two different terms"));

	إذا (check_رڪن_format(bad, "bad") || check_رڪن_format(good, "good"))
		أرجع -1;

	res = write_منزل_file(git_رستو_bisect_terms(), "%s\n%s\n", bad, good);

	أرجع res;
}

ساكن صحيح bisect_reset(ثابت حرف *commit, bool quiet)
{
	بنية strbuf branch = STRBUF_INIT;

	إذا (!commit) {
		إذا (!strbuf_read_file(&branch, git_رستو_bisect_شروعات(), 0))
			لکو(_("We are not bisecting.\n"));
		وإلا
			strbuf_rtrim(&branch);
	} وإلا {
		بنية object_id oid;

		إذا (repo_get_oid_commit(the_repository, commit, &oid))
			أرجع error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	إذا (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		بنية ٻار_process cmd = ٻار_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.دليلون, "checkout", "--ignore-other-worktrees", عدم);
		إذا (quiet)
			strvec_ڌڪو(&cmd.دليلون, "--quiet");
		strvec_pushl(&cmd.دليلون, branch.buf, "--", عدم);
		إذا (لاڳيتو_command(&cmd)) {
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

ساكن صحيح parse_reset_when_مليو(ثابت حرف *قدر,
				  تعداد reset_when_مليو_گهڻائي *گهڻائي)
{
	إذا (!strcmp(قدر, "original"))
		*گهڻائي = RESET_WHEN_مليو_منزل_ORIGINAL;
	وإلا إذا (!strcmp(قدر, "found"))
		*گهڻائي = RESET_WHEN_مليو_منزل_مليو;
	وإلا
		أرجع error(_("invalid value for '--reset-when-found': '%s'"),
			     قدر);

	أرجع 0;
}

ساكن ثابت حرف *reset_when_مليو_گهڻائي_name(تعداد reset_when_مليو_گهڻائي گهڻائي)
{
	بدل (گهڻائي) {
	حالة RESET_WHEN_مليو_منزل_ORIGINAL:
		أرجع "original";
	حالة RESET_WHEN_مليو_منزل_مليو:
		أرجع "found";
	حالة RESET_WHEN_مليو_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", گهڻائي);
}

ساكن صحيح read_reset_when_مليو(تعداد reset_when_مليو_گهڻائي *گهڻائي)
{
	بنية strbuf قدر = STRBUF_INIT;
	صحيح res = 0;

	*گهڻائي = RESET_WHEN_مليو_NONE;
	إذا (is_خالي_جايون_or_missing_file(git_رستو_bisect_reset_when_مليو()))
		أرجع 0;

	إذا (strbuf_read_file(&قدر, git_رستو_bisect_reset_when_مليو(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_رستو_bisect_reset_when_مليو());
		اذهب ٻاهريون;
	}
	strbuf_trim(&قدر);
	إذا (parse_reset_when_مليو(قدر.buf, گهڻائي))
		res = -1;

ٻاهريون:
	strbuf_release(&قدر);
	أرجع res;
}

ساكن صحيح bisect_reset_when_مليو(تعداد reset_when_مليو_گهڻائي گهڻائي)
{
	بنية bisect_terms terms = { 0 };
	حرف *commit = عدم;
	صحيح res;

	إذا (گهڻائي == RESET_WHEN_مليو_منزل_مليو) {
		read_bisect_terms(&terms.رڪن_bad, &terms.رڪن_good);
		commit = xstrfmt("refs/bisect/%s", terms.رڪن_bad);
	} وإلا إذا (گهڻائي == RESET_WHEN_مليو_NONE) {
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
	بنية strbuf commit_نياپو = STRBUF_INIT;
	حرف *label = xstrfmt(fmt, state);

	repo_format_commit_پيغام(the_repository, commit, "%s", &commit_نياپو,
				   &pp);

	fprintf(fp, "# %s: [%s] %s\n", label, oid_منزل_hex(&commit->object.oid),
		commit_نياپو.buf);

	strbuf_release(&commit_نياپو);
	حرر(label);
}

ساكن صحيح bisect_write(ثابت حرف *state, ثابت حرف *rev,
			ثابت بنية bisect_terms *terms, صحيح nolog)
{
	بنية strbuf ٽيگ = STRBUF_INIT;
	بنية object_id oid;
	بنية commit *commit;
	FILE *fp = عدم;
	صحيح res = 0;

	إذا (!strcmp(state, terms->رڪن_bad)) {
		strbuf_addf(&ٽيگ, "refs/bisect/%s", state);
	} وإلا إذا (one_جو(state, terms->رڪن_good, "skip", عدم)) {
		strbuf_addf(&ٽيگ, "refs/bisect/%s-%s", state, rev);
	} وإلا {
		res = error(_("Bad bisect_write argument: %s"), state);
		اذهب پڄاڻي;
	}

	إذا (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		اذهب پڄاڻي;
	}

	إذا (refs_update_ref(get_main_ref_store(the_repository), عدم, ٽيگ.buf, &oid, عدم, 0,
			    UPDATE_REFS_نياپو_ON_ERR)) {
		res = -1;
		اذهب پڄاڻي;
	}

	fp = fopen(git_رستو_bisect_log(), "a");
	إذا (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_رستو_bisect_log());
		اذهب پڄاڻي;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	إذا (!nolog)
		fprintf(fp, "git bisect %s %s\n", state, rev);

پڄاڻي:
	إذا (fp)
		fclose(fp);
	strbuf_release(&ٽيگ);
	أرجع res;
}

ساكن صحيح check_and_سيٽ_terms(بنية bisect_terms *terms, ثابت حرف *cmd)
{
	صحيح has_رڪن_file = !is_خالي_جايون_or_missing_file(git_رستو_bisect_terms());

	إذا (one_جو(cmd, "skip", "start", "terms", عدم))
		أرجع 0;

	إذا (has_رڪن_file && strcmp(cmd, terms->رڪن_bad) &&
	    strcmp(cmd, terms->رڪن_good))
		أرجع error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->رڪن_bad,
				terms->رڪن_good);

	إذا (!has_رڪن_file) {
		إذا (one_جو(cmd, "bad", "good", عدم)) {
			سيٽ_terms(terms, "bad", "good");
			أرجع write_terms(terms->رڪن_bad, terms->رڪن_good);
		}
		إذا (one_جو(cmd, "new", "old", عدم)) {
			سيٽ_terms(terms, "new", "old");
			أرجع write_terms(terms->رڪن_bad, terms->رڪن_good);
		}
	}

	أرجع 0;
}

ساكن صحيح inc_nr(ثابت بنية reference *ref اڻ_استعمال_ٿيل, فراغ *cb_معلومات)
{
	بنانشان صحيح *nr = (بنانشان صحيح *)cb_معلومات;
	(*nr)++;
	أرجع 0;
}

ساكن ثابت حرف need_bad_and_good_revision_warning[] =
	N_("You need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

ساكن ثابت حرف need_bisect_شروعات_warning[] =
	N_("You need to start by \"git bisect start\".\n"
	   "You then need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

ساكن صحيح decide_ايندڙ(ثابت بنية bisect_terms *terms,
		       ثابت حرف *current_رڪن, صحيح missing_good,
		       صحيح missing_bad)
{
	إذا (!missing_good && !missing_bad)
		أرجع 0;
	إذا (!current_رڪن)
		أرجع -1;

	إذا (missing_good && !missing_bad &&
	    !strcmp(current_رڪن, terms->رڪن_good)) {
		حرف *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->رڪن_bad);
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

	إذا (!is_خالي_جايون_or_missing_file(git_رستو_bisect_شروعات()))
		أرجع error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	وإلا
		أرجع error(_(need_bisect_شروعات_warning),
			     vocab_good, vocab_bad, vocab_good, vocab_bad);
}

ساكن فراغ bisect_حالت(بنية bisect_state *state,
			  ثابت بنية bisect_terms *terms)
{
	حرف *bad_ref = xstrfmt("refs/bisect/%s", terms->رڪن_bad);
	حرف *good_glob = xstrfmt("%s-*", terms->رڪن_good);
	بنية refs_for_each_ref_options opts = {
		.نمونو = good_glob,
		.اڳياڙي = "refs/bisect/",
		.trim_اڳياڙي = لڙي_ڊيگهه("refs/bisect/"),
	};

	إذا (refs_ref_exists(get_main_ref_store(the_repository), bad_ref))
		state->nr_bad = 1;

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      inc_nr, &state->nr_good, &opts);

	حرر(good_glob);
	حرر(bad_ref);
}

__attribute__((format (لکو, 1, 2)))
ساكن فراغ bisect_log_printf(ثابت حرف *fmt, ...)
{
	بنية strbuf buf = STRBUF_INIT;
	va_list ap;

	va_شروعات(ap, fmt);
	strbuf_vaddf(&buf, fmt, ap);
	va_پڇاڙي(ap);

	لکو("%s", buf.buf);
	append_منزل_file(git_رستو_bisect_log(), "# %s", buf.buf);

	strbuf_release(&buf);
}

ساكن فراغ bisect_print_حالت(ثابت بنية bisect_terms *terms)
{
	بنية bisect_state state = { 0 };

	bisect_حالت(&state, terms);

	/* If we had both, we'd already be started, and shouldn't get here. */
	إذا (state.nr_good && state.nr_bad)
		أرجع;

	إذا (!state.nr_good && !state.nr_bad)
		bisect_log_printf(_("status: waiting for both '%s' and '%s' commits\n"),
				  terms->رڪن_good, terms->رڪن_bad);
	وإلا إذا (state.nr_good)
		bisect_log_printf(Q_("status: waiting for '%s' commit, %d '%s' commit known\n",
				     "status: waiting for '%s' commit, %d '%s' commits known\n",
				     state.nr_good),
				  terms->رڪن_bad, state.nr_good, terms->رڪن_good);
	وإلا
		bisect_log_printf(_("status: waiting for '%s' commit(s), '%s' commit known\n"),
				  terms->رڪن_good, terms->رڪن_bad);
}

ساكن صحيح bisect_ايندڙ_check(ثابت بنية bisect_terms *terms,
			     ثابت حرف *current_رڪن)
{
	بنية bisect_state state = { 0 };
	bisect_حالت(&state, terms);
	أرجع decide_ايندڙ(terms, current_رڪن, !state.nr_good, !state.nr_bad);
}

ساكن صحيح get_terms(بنية bisect_terms *terms, صحيح file_missing_is_ok)
{
	بنية strbuf str = STRBUF_INIT;
	FILE *fp = عدم;
	صحيح res = 0;

	fp = fopen(git_رستو_bisect_terms(), "r");
	إذا (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		اذهب پڄاڻي;
	}

	free_terms(terms);
	إذا (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		اذهب پڄاڻي;
	}
	terms->رڪن_bad = strbuf_detach(&str, عدم);
	إذا (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		FREE_AND_NULL(terms->رڪن_bad);
		اذهب پڄاڻي;
	}
	terms->رڪن_good = strbuf_detach(&str, عدم);

پڄاڻي:
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
		لکو(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->رڪن_good, terms->رڪن_bad);
		أرجع 0;
	}
	إذا (one_جو(option, "--term-good", "--term-old", عدم))
		لکو("%s\n", terms->رڪن_good);
	وإلا إذا (one_جو(option, "--term-bad", "--term-new", عدم))
		لکو("%s\n", terms->رڪن_bad);
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
	FILE *fp = fopen(git_رستو_bisect_log(), "a");
	بنية strbuf orig_دليلون = STRBUF_INIT;

	إذا (!fp)
		أرجع -1;

	إذا (fprintf(fp, "git bisect start") < 1) {
		res = -1;
		اذهب پڄاڻي;
	}

	sq_quote_argv(&orig_دليلون, argv);
	إذا (fprintf(fp, "%s\n", orig_دليلون.buf) < 1)
		res = -1;

پڄاڻي:
	fclose(fp);
	strbuf_release(&orig_دليلون);
	أرجع res;
}

ساكن صحيح جوڙيو_bisect_ref(ثابت بنية reference *ref, فراغ *cb)
{
	بنية جوڙيو_bisect_ref_معلومات *معلومات = cb;

	جوڙيو_pending_oid(معلومات->revs, ref->name, ref->oid, معلومات->object_flags);

	أرجع 0;
}

ساكن صحيح prepare_revs(بنية bisect_terms *terms, بنية rev_info *revs)
{
	بنية refs_for_each_ref_options opts = {
		.اڳياڙي = "refs/bisect/",
		.trim_اڳياڙي = لڙي_ڊيگهه("refs/bisect/"),
	};
	صحيح res = 0;
	بنية جوڙيو_bisect_ref_معلومات cb = { revs };
	حرف *good = xstrfmt("%s-*", terms->رڪن_good);

	/*
	 * We cannot use terms->term_bad directly in
	 * for_each_glob_ref_in() and we have to append a '*' to it,
	 * otherwise for_each_glob_ref_in() will append '/' and '*'.
	 */
	حرف *bad = xstrfmt("%s*", terms->رڪن_bad);

	/*
	 * It is important to reset the flags used by revision walks
	 * as the previous call to bisect_next_all() in turn
	 * sets up a revision walk.
	 */
	reset_revision_walk();
	repo_init_revisions(the_repository, revs, عدم);
	تياري_revisions(0, عدم, revs, عدم);

	opts.نمونو = bad;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      جوڙيو_bisect_ref, &cb, &opts);

	cb.object_flags = UNINTERESTING;
	opts.نمونو = good;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      جوڙيو_bisect_ref, &cb, &opts);

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

	fp = fopen(git_رستو_bisect_log(), "a");
	إذا (!fp)
		أرجع error_errno(_("could not open '%s' for appending"),
				  git_رستو_bisect_log());

	إذا (fprintf(fp, "# only skipped commits left to test\n") < 0)
		أرجع error_errno(_("failed to write to '%s'"), git_رستو_bisect_log());

	طالما ((commit = get_revision(&revs)) != عدم) {
		strbuf_reset(&commit_name);
		repo_format_commit_پيغام(the_repository, commit, "%s",
					   &commit_name, &pp);
		fprintf(fp, "# possible first '%s' commit: [%s] %s\n",
			terms->رڪن_bad, oid_منزل_hex(&commit->object.oid),
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
	حرف *bad_ref = xstrfmt("refs/bisect/%s",terms->رڪن_bad);
	صحيح res;

	refs_read_ref(get_main_ref_store(the_repository), bad_ref, &oid);
	commit = lookup_commit_reference_by_name(bad_ref);
	إذا (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		حرر(bad_ref);
		أرجع BISECT_FAILED;
	}
	repo_format_commit_پيغام(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_منزل_file(git_رستو_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->رڪن_bad, oid_منزل_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	حرر(bad_ref);
	أرجع res;
}

ساكن تعداد bisect_error bisect_ايندڙ(بنية bisect_terms *terms,
				     ثابت حرف *اڳياڙي)
{
	تعداد bisect_error res;

	إذا (bisect_autostart(terms))
		أرجع BISECT_FAILED;

	إذا (bisect_ايندڙ_check(terms, terms->رڪن_good))
		أرجع BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_ايندڙ_all(the_repository, اڳياڙي);

	إذا (res == BISECT_اندروني_SUCCESS_1_ST_BAD_مليو) {
		res = bisect_successful(terms);
		أرجع res ? res : BISECT_اندروني_SUCCESS_1_ST_BAD_مليو;
	} وإلا إذا (res == BISECT_ONLY_SKIPPED_کاٻو) {
		res = bisect_skipped_commits(terms);
		أرجع res ? res : BISECT_ONLY_SKIPPED_کاٻو;
	}
	أرجع res;
}

ساكن تعداد bisect_error bisect_auto_ايندڙ(بنية bisect_terms *terms,
					  ثابت حرف *اڳياڙي)
{
	إذا (bisect_ايندڙ_check(terms, عدم)) {
		bisect_print_حالت(terms);
		أرجع BISECT_OK;
	}

	أرجع bisect_ايندڙ(terms, اڳياڙي);
}

ساكن تعداد bisect_error bisect_شروعات(بنية bisect_terms *terms, صحيح argc,
				      ثابت حرف **argv)
{
	صحيح no_checkout = 0;
	صحيح پهريون_والد_only = 0;
	صحيح i, has_double_dash = 0, must_write_terms = 0, bad_ڏٺل = 0;
	صحيح flags, pathspec_pos;
	تعداد bisect_error res = BISECT_OK;
	بنية string_list revs = STRING_LIST_INIT_DUP;
	بنية string_list حالتون = STRING_LIST_INIT_DUP;
	بنية strbuf شروعات_سر = STRBUF_INIT;
	بنية strbuf bisect_نالا = STRBUF_INIT;
	بنية object_id سر_oid;
	بنية object_id oid;
	تعداد reset_when_مليو_گهڻائي reset_when_مليو = RESET_WHEN_مليو_NONE;
	ثابت حرف *سر;

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
			پهريون_والد_only = 1;
		} وإلا إذا (!strcmp(arg, "--reset-when-found")) {
			reset_when_مليو = RESET_WHEN_مليو_منزل_ORIGINAL;
		} وإلا إذا (skip_اڳياڙي(arg, "--reset-when-found=", &arg)) {
			إذا (parse_reset_when_مليو(arg, &reset_when_مليو)) {
				res = BISECT_FAILED;
				اذهب پڄاڻي;
			}
		} وإلا إذا (!strcmp(arg, "--term-good") ||
			 !strcmp(arg, "--term-old")) {
			i++;
			إذا (argc <= i)
				أرجع error(_("'' is not a valid term"));
			must_write_terms = 1;
			حرر((فراغ *) terms->رڪن_good);
			terms->رڪن_good = xstrdup(argv[i]);
		} وإلا إذا (skip_اڳياڙي(arg, "--term-good=", &arg) ||
			   skip_اڳياڙي(arg, "--term-old=", &arg)) {
			must_write_terms = 1;
			حرر((فراغ *) terms->رڪن_good);
			terms->رڪن_good = xstrdup(arg);
		} وإلا إذا (!strcmp(arg, "--term-bad") ||
			 !strcmp(arg, "--term-new")) {
			i++;
			إذا (argc <= i)
				أرجع error(_("'' is not a valid term"));
			must_write_terms = 1;
			حرر((فراغ *) terms->رڪن_bad);
			terms->رڪن_bad = xstrdup(argv[i]);
		} وإلا إذا (skip_اڳياڙي(arg, "--term-bad=", &arg) ||
			   skip_اڳياڙي(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			حرر((فراغ *) terms->رڪن_bad);
			terms->رڪن_bad = xstrdup(arg);
		} وإلا إذا (starts_with(arg, "--")) {
			أرجع error(_("unrecognized option: '%s'"), arg);
		} وإلا إذا (!get_oidf(&oid, "%s^{commit}", arg)) {
			string_list_append(&revs, oid_منزل_hex(&oid));
		} وإلا إذا (has_double_dash) {
			die(_("'%s' does not appear to be a valid "
			      "revision"), arg);
		} وإلا {
			اكسر;
		}
	}
	إذا (reset_when_مليو != RESET_WHEN_مليو_NONE && no_checkout) {
		res = error(_("options '%s' and '%s' cannot be used together"),
			    "--reset-when-found", "--no-checkout");
		اذهب پڄاڻي;
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
		إذا (bad_ڏٺل) {
			string_list_append(&حالتون, terms->رڪن_good);
		} وإلا {
			bad_ڏٺل = 1;
			string_list_append(&حالتون, terms->رڪن_bad);
		}
	}

	/*
	 * Verify HEAD
	 */
	سر = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
				       "HEAD", 0, &سر_oid, &flags);
	إذا (!سر) {
		إذا (repo_get_oid(the_repository, "HEAD", &سر_oid))
			أرجع error(_("bad HEAD - I need a HEAD"));
		سر = "HEAD";
	}

	/*
	 * Check if we are bisecting
	 */
	إذا (!is_خالي_جايون_or_missing_file(git_رستو_bisect_شروعات())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&شروعات_سر, git_رستو_bisect_شروعات(), 0);
		strbuf_trim(&شروعات_سر);
		إذا (!no_checkout) {
			بنية ٻار_process cmd = ٻار_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.دليلون, "checkout", شروعات_سر.buf,
				     "--", عدم);
			إذا (لاڳيتو_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       شروعات_سر.buf);
				اذهب پڄاڻي;
			}
		}
	} وإلا {
		/* Get the rev from where we start. */
		إذا (!repo_get_oid(the_repository, سر, &سر_oid) &&
		    !starts_with(سر, "refs/heads/")) {
			strbuf_reset(&شروعات_سر);
			strbuf_جوڙيو_oid_hex(&شروعات_سر, &سر_oid);
		} وإلا إذا (!repo_get_oid(the_repository, سر, &سر_oid) &&
			   skip_اڳياڙي(سر, "refs/heads/", &سر)) {
			strbuf_addstr(&شروعات_سر, سر);
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
	write_file(git_رستو_bisect_شروعات(), "%s\n", شروعات_سر.buf);

	إذا (پهريون_والد_only)
		write_file(git_رستو_bisect_پهريون_والد(), "\n");

	إذا (reset_when_مليو != RESET_WHEN_مليو_NONE)
		write_file(git_رستو_bisect_reset_when_مليو(), "%s\n",
			   reset_when_مليو_گهڻائي_name(reset_when_مليو));

	إذا (no_checkout) {
		إذا (repo_get_oid(the_repository, شروعات_سر.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), شروعات_سر.buf);
			اذهب پڄاڻي;
		}
		إذا (refs_update_ref(get_main_ref_store(the_repository), عدم, "BISECT_HEAD", &oid, عدم, 0,
				    UPDATE_REFS_نياپو_ON_ERR)) {
			res = BISECT_FAILED;
			اذهب پڄاڻي;
		}
	}

	إذا (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_نالا, argv + pathspec_pos);
	write_file(git_رستو_bisect_نالا(), "%s\n", bisect_نالا.buf);

	لكل (i = 0; i < حالتون.nr; i++)
		إذا (bisect_write(حالتون.شيون[i].string,
				 revs.شيون[i].string, terms, 1)) {
			res = BISECT_FAILED;
			اذهب پڄاڻي;
		}

	إذا (must_write_terms && write_terms(terms->رڪن_bad,
					    terms->رڪن_good)) {
		res = BISECT_FAILED;
		اذهب پڄاڻي;
	}

	res = bisect_append_log_quoted(argv);
	إذا (res)
		res = BISECT_FAILED;

پڄاڻي:
	string_list_clear(&revs, 0);
	string_list_clear(&حالتون, 0);
	strbuf_release(&شروعات_سر);
	strbuf_release(&bisect_نالا);
	إذا (res)
		أرجع res;

	res = bisect_auto_ايندڙ(terms, عدم);
	إذا (!is_bisect_success(res))
		bisect_clean_state();
	أرجع res;
}

ساكن مضمن صحيح file_is_not_خالي_جايون(ثابت حرف *رستو)
{
	أرجع !is_خالي_جايون_or_missing_file(رستو);
}

ساكن صحيح bisect_autostart(بنية bisect_terms *terms)
{
	صحيح res;
	ثابت حرف *yesno;

	إذا (file_is_not_خالي_جايون(git_رستو_bisect_شروعات()))
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
		-1 : bisect_شروعات(terms, 0, خالي_جايون_strvec);

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
	إذا (check_and_سيٽ_terms(terms, state) ||
	    !one_جو(state, terms->رڪن_good, terms->رڪن_bad, "skip", عدم))
		أرجع BISECT_FAILED;

	argv++;
	argc--;
	إذا (argc > 1 && !strcmp(state, terms->رڪن_bad))
		أرجع error(_("'git bisect %s' can take only one argument."), terms->رڪن_bad);

	إذا (argc == 0) {
		ثابت حرف *سر = "BISECT_HEAD";
		تعداد get_oid_نتيجو res_سر = repo_get_oid(the_repository,
							    سر, &oid);

		إذا (res_سر == MISSING_OBJECT) {
			سر = "HEAD";
			res_سر = repo_get_oid(the_repository, سر, &oid);
		}

		إذا (res_سر)
			error(_("Bad rev input: %s"), سر);
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
		إذا (bisect_write(state, oid_منزل_hex(&revs.oid[i]), terms, 0)) {
			oid_array_clear(&revs);
			أرجع BISECT_FAILED;
		}
		إذا (verify_expected && !oideq(&revs.oid[i], &expected)) {
			unlink_or_warn(git_رستو_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					عدم, "BISECT_EXPECTED_REV", عدم,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	أرجع bisect_auto_ايندڙ(terms, عدم);
}

ساكن تعداد bisect_error bisect_log(فراغ)
{
	صحيح fd, حالت;
	ثابت حرف* filename = git_رستو_bisect_log();

	إذا (is_خالي_جايون_or_missing_file(filename))
		أرجع error(_("We are not bisecting."));

	fd = open(filename, O_RDONLY);
	إذا (fd < 0)
		أرجع BISECT_FAILED;

	حالت = copy_fd(fd, STDOUT_FILENO);
	close(fd);
	أرجع حالت ? BISECT_FAILED : BISECT_OK;
}

ساكن صحيح process_replay_سٽ(بنية bisect_terms *terms, بنية strbuf *سٽ)
{
	ثابت حرف *p = سٽ->buf + strspn(سٽ->buf, " \t");
	حرف *لفظ_پڇاڙي, *rev;

	إذا ((!skip_اڳياڙي(p, "git bisect", &p) &&
	!skip_اڳياڙي(p, "git-bisect", &p)) || !خال_آهي(*p))
		أرجع 0;
	p += strspn(p, " \t");

	لفظ_پڇاڙي = (حرف *)p + strcspn(p, " \t");
	rev = لفظ_پڇاڙي + strspn(لفظ_پڇاڙي, " \t");
	*لفظ_پڇاڙي = '\0'; /* NUL-terminate the word */

	إذا (get_terms(terms, 1))
		أرجع error(_("no terms defined"));
	إذا (check_and_سيٽ_terms(terms, p))
		أرجع -1;

	إذا (!strcmp(p, "start")) {
		بنية strvec argv = STRVEC_INIT;
		صحيح res;
		sq_dequote_منزل_strvec(rev, &argv);
		res = bisect_شروعات(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		أرجع res;
	}

	إذا (one_جو(p, terms->رڪن_good,
	   terms->رڪن_bad, "skip", عدم))
		أرجع bisect_write(p, rev, terms, 0);

	إذا (!strcmp(p, "terms")) {
		بنية strvec argv = STRVEC_INIT;
		صحيح res;
		sq_dequote_منزل_strvec(rev, &argv);
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
	بنية strbuf سٽ = STRBUF_INIT;

	إذا (is_خالي_جايون_or_missing_file(filename))
		أرجع error(_("cannot read file '%s' for replaying"), filename);

	إذا (bisect_clean_state())
		أرجع BISECT_FAILED;

	fp = fopen(filename, "r");
	إذا (!fp)
		أرجع BISECT_FAILED;

	طالما ((strbuf_getline(&سٽ, fp) != EOF) && !res)
		res = process_replay_سٽ(terms, &سٽ);

	strbuf_release(&سٽ);
	fclose(fp);

	إذا (res)
		أرجع BISECT_FAILED;

	أرجع bisect_auto_ايندڙ(terms, عدم);
}

ساكن تعداد bisect_error bisect_skip(بنية bisect_terms *terms, صحيح argc,
				     ثابت حرف **argv)
{
	صحيح i;
	تعداد bisect_error res;
	بنية strvec argv_state = STRVEC_INIT;

	strvec_ڌڪو(&argv_state, "skip");

	لكل (i = 0; i < argc; i++) {
		ثابت حرف *dotdot = لڙي_ڳولا(argv[i], "..");

		إذا (dotdot) {
			بنية rev_info revs;
			بنية commit *commit;

			repo_init_revisions(the_repository, &revs, عدم);
			تياري_revisions(2, argv + i - 1, &revs, عدم);

			إذا (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			طالما ((commit = get_revision(&revs)) != عدم)
				strvec_ڌڪو(&argv_state,
						oid_منزل_hex(&commit->object.oid));

			reset_revision_walk();
			release_revisions(&revs);
		} وإلا {
			strvec_ڌڪو(&argv_state, argv[i]);
		}
	}
	res = bisect_state(terms, argv_state.nr, argv_state.v);

	strvec_clear(&argv_state);
	أرجع res;
}

ساكن صحيح bisect_visualize(بنية bisect_terms *terms, صحيح argc,
			    ثابت حرف **argv)
{
	بنية ٻار_process cmd = ٻار_PROCESS_INIT;
	بنية strbuf sb = STRBUF_INIT;

	إذا (bisect_ايندڙ_check(terms, عدم) != 0)
		أرجع BISECT_FAILED;

	cmd.no_stdin = 1;
	إذا (!argc) {
		إذا ((getenv("DISPLAY") || getenv("SESSIONNAME") || getenv("MSYSTEM") ||
		     getenv("SECURITYSESSIONID")) && exists_اندريون_رستو("gitk")) {
			strvec_ڌڪو(&cmd.دليلون, "gitk");
		} وإلا {
			strvec_ڌڪو(&cmd.دليلون, "log");
			cmd.git_cmd = 1;
		}
	} وإلا {
		إذا (argv[0][0] == '-') {
			strvec_ڌڪو(&cmd.دليلون, "log");
			cmd.git_cmd = 1;
		} وإلا إذا (strcmp(argv[0], "tig") && !starts_with(argv[0], "git"))
			cmd.git_cmd = 1;

		strvec_pushv(&cmd.دليلون, argv);
	}

	strvec_pushl(&cmd.دليلون, "--bisect", "--", عدم);

	strbuf_read_file(&sb, git_رستو_bisect_نالا(), 0);
	sq_dequote_منزل_strvec(sb.buf, &cmd.دليلون);
	strbuf_release(&sb);

	أرجع لاڳيتو_command(&cmd);
}

ساكن صحيح get_پهريون_good(ثابت بنية reference *ref, فراغ *cb_معلومات)
{
	oidcpy(cb_معلومات, ref->oid);
	أرجع 1;
}

ساكن صحيح do_bisect_لاڳيتو(ثابت حرف *command)
{
	بنية ٻار_process cmd = ٻار_PROCESS_INIT;

	لکو(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_ڌڪو(&cmd.دليلون, command);
	أرجع لاڳيتو_command(&cmd);
}

ساكن صحيح verify_good(ثابت بنية bisect_terms *terms, ثابت حرف *command)
{
	صحيح rc;
	تعداد bisect_error res;
	بنية object_id good_rev;
	بنية object_id current_rev;
	حرف *good_glob = xstrfmt("%s-*", terms->رڪن_good);
	صحيح no_checkout = refs_ref_exists(get_main_ref_store(the_repository),
					  "BISECT_HEAD");
	بنية refs_for_each_ref_options opts = {
		.نمونو = good_glob,
		.اڳياڙي = "refs/bisect/",
		.trim_اڳياڙي = لڙي_ڊيگهه("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_پهريون_good, &good_rev, &opts);
	حرر(good_glob);

	إذا (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		أرجع -1;

	res = bisect_checkout(&good_rev, no_checkout);
	إذا (res != BISECT_OK)
		أرجع -1;

	rc = do_bisect_لاڳيتو(command);

	res = bisect_checkout(&current_rev, no_checkout);
	إذا (res != BISECT_OK)
		أرجع -1;

	أرجع rc;
}

ساكن صحيح bisect_لاڳيتو(بنية bisect_terms *terms, صحيح argc, ثابت حرف **argv)
{
	صحيح res = BISECT_OK;
	بنية strbuf command = STRBUF_INIT;
	ثابت حرف *reset_when_مليو_arg;
	ثابت حرف *new_state;
	صحيح temporary_stdout_fd, saved_stdout;
	صحيح is_پهريون_لاڳيتو = 1;
	تعداد reset_when_مليو_گهڻائي reset_when_مليو = RESET_WHEN_مليو_NONE;

	إذا (bisect_ايندڙ_check(terms, عدم))
		أرجع BISECT_FAILED;

	إذا (argc && !strcmp(argv[0], "--reset-when-found")) {
		reset_when_مليو = RESET_WHEN_مليو_منزل_ORIGINAL;
	} وإلا إذا (argc && skip_اڳياڙي(argv[0], "--reset-when-found=",
				    &reset_when_مليو_arg)) {
		إذا (parse_reset_when_مليو(reset_when_مليو_arg,
					   &reset_when_مليو))
			أرجع BISECT_FAILED;
	}

	إذا (reset_when_مليو != RESET_WHEN_مليو_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		أرجع error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	إذا (reset_when_مليو != RESET_WHEN_مليو_NONE) {
		write_file(git_رستو_bisect_reset_when_مليو(), "%s\n",
			   reset_when_مليو_گهڻائي_name(reset_when_مليو));
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
		res = do_bisect_لاڳيتو(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		إذا (is_پهريون_لاڳيتو && (res == 126 || res == 127)) {
			صحيح rc = verify_good(terms, command.buf);
			is_پهريون_لاڳيتو = 0;
			إذا (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->رڪن_good);
				res = BISECT_FAILED;
				اكسر;
			}
			إذا (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->رڪن_good);
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
			new_state = terms->رڪن_good;
		وإلا
			new_state = terms->رڪن_bad;

		temporary_stdout_fd = open(git_رستو_bisect_لاڳيتو(), O_CREAT | O_WRONLY | O_TRUNC, 0666);

		إذا (temporary_stdout_fd < 0) {
			res = error_errno(_("cannot open file '%s' for writing"), git_رستو_bisect_لاڳيتو());
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

		print_file_منزل_stdout(git_رستو_bisect_لاڳيتو());

		إذا (res == BISECT_ONLY_SKIPPED_کاٻو)
			error(_("bisect run cannot continue any more"));
		وإلا إذا (res == BISECT_اندروني_SUCCESS_ملايو_بنياد) {
			puts(_("bisect run success"));
			res = BISECT_OK;
		} وإلا إذا (res == BISECT_اندروني_SUCCESS_1_ST_BAD_مليو) {
			لکو(_("bisect found first '%s' commit\n"), terms->رڪن_bad);
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

ساكن صحيح cmd_bisect__reset(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي اڻ_استعمال_ٿيل,
			     بنية repository *repo اڻ_استعمال_ٿيل)
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

ساكن صحيح cmd_bisect__terms(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي اڻ_استعمال_ٿيل,
			     بنية repository *repo اڻ_استعمال_ٿيل)
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

ساكن صحيح cmd_bisect_شروعات(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي اڻ_استعمال_ٿيل,
			     بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح res;
	بنية bisect_terms terms = { 0 };

	سيٽ_terms(&terms, "bad", "good");
	res = bisect_شروعات(&terms, argc, argv);
	free_terms(&terms);
	أرجع res;
}

ساكن صحيح cmd_bisect_ايندڙ(صحيح argc, ثابت حرف **argv اڻ_استعمال_ٿيل, ثابت حرف *اڳياڙي,
			    بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح res;
	بنية bisect_terms terms = { 0 };

	إذا (argc)
		أرجع error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	إذا (get_terms(&terms, 1))
		أرجع error(_("no terms defined"));
	res = bisect_ايندڙ(&terms, اڳياڙي);
	free_terms(&terms);
	أرجع res;
}

ساكن صحيح cmd_bisect__log(صحيح argc اڻ_استعمال_ٿيل, ثابت حرف **argv اڻ_استعمال_ٿيل,
			   ثابت حرف *اڳياڙي اڻ_استعمال_ٿيل,
			   بنية repository *repo اڻ_استعمال_ٿيل)
{
	أرجع bisect_log();
}

ساكن صحيح cmd_bisect__replay(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي اڻ_استعمال_ٿيل,
			      بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح res;
	بنية bisect_terms terms = { 0 };

	إذا (argc != 1)
		أرجع error(_("no logfile given"));
	سيٽ_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	أرجع res;
}

ساكن صحيح cmd_bisect__skip(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي اڻ_استعمال_ٿيل,
			    بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح res;
	بنية bisect_terms terms = { 0 };

	سيٽ_terms(&terms, "bad", "good");
	إذا (get_terms(&terms, 1))
		أرجع error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	أرجع res;
}

ساكن صحيح cmd_bisect__visualize(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي اڻ_استعمال_ٿيل,
				 بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح res;
	بنية bisect_terms terms = { 0 };

	إذا (get_terms(&terms, 1))
		أرجع error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	أرجع res;
}

ساكن صحيح cmd_bisect_لاڳيتو(صحيح argc, ثابت حرف **argv, ثابت حرف *اڳياڙي اڻ_استعمال_ٿيل,
			   بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح res;
	بنية bisect_terms terms = { 0 };

	إذا (!argc)
		أرجع error(_("'%s' failed: no command provided."), "git bisect run");
	إذا (get_terms(&terms, 1))
		أرجع error(_("no terms defined"));
	res = bisect_لاڳيتو(&terms, argc, argv);
	free_terms(&terms);
	أرجع res;
}

صحيح cmd_bisect(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *اڳياڙي,
	       بنية repository *repo)
{
	صحيح res = 0;
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT_SUBCOMMAND("reset", &fn, cmd_bisect__reset),
		OPT_SUBCOMMAND("terms", &fn, cmd_bisect__terms),
		OPT_SUBCOMMAND("start", &fn, cmd_bisect_شروعات),
		OPT_SUBCOMMAND("next", &fn, cmd_bisect_ايندڙ),
		OPT_SUBCOMMAND("log", &fn, cmd_bisect__log),
		OPT_SUBCOMMAND("replay", &fn, cmd_bisect__replay),
		OPT_SUBCOMMAND("skip", &fn, cmd_bisect__skip),
		OPT_SUBCOMMAND("visualize", &fn, cmd_bisect__visualize),
		OPT_SUBCOMMAND("view", &fn, cmd_bisect__visualize),
		OPT_SUBCOMMAND("run", &fn, cmd_bisect_لاڳيتو),
		OPT_پڇاڙي()
	};
	argc = parse_options(argc, argv, اڳياڙي, options, git_bisect_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);

	إذا (!fn) {
		بنية bisect_terms terms = { 0 };

		إذا (!argc)
			usage_نياپو_opt(_("need a command"), git_bisect_usage, options);

		إذا (!strcmp(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		سيٽ_terms(&terms, "bad", "good");
		إذا (get_terms(&terms, 1))
			أرجع error(_("no terms defined"));
		إذا (check_and_سيٽ_terms(&terms, argv[0]) ||
		    !one_جو(argv[0], terms.رڪن_good, terms.رڪن_bad, عدم))
			usage_نياپو_optf(_("unknown command: '%s'"), git_bisect_usage,
				       options, argv[0]);
		وإلا
			res = bisect_state(&terms, argc, argv);
		free_terms(&terms);
	} وإلا {
		argc--;
		argv++;
		res = fn(argc, argv, اڳياڙي, repo);
	}

	إذا (res == BISECT_اندروني_SUCCESS_1_ST_BAD_مليو) {
		تعداد reset_when_مليو_گهڻائي گهڻائي;

		إذا (read_reset_when_مليو(&گهڻائي))
			res = BISECT_FAILED;
		وإلا إذا (گهڻائي != RESET_WHEN_مليو_NONE &&
			 bisect_reset_when_مليو(گهڻائي))
			res = BISECT_FAILED;
	}

	أرجع is_bisect_success(res) ? 0 : -res;
}
