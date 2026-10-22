#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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

ساكن GIT_رستہ_FUNC(git_رستہ_bisect_terms, "BISECT_TERMS")
ساكن GIT_رستہ_FUNC(git_رستہ_bisect_ancestors_ok, "BISECT_ANCESTORS_OK")
ساكن GIT_رستہ_FUNC(git_رستہ_bisect_شروع, "BISECT_START")
ساكن GIT_رستہ_FUNC(git_رستہ_bisect_log, "BISECT_LOG")
ساكن GIT_رستہ_FUNC(git_رستہ_bisect_ناں, "BISECT_NAMES")
ساكن GIT_رستہ_FUNC(git_رستہ_bisect_پہلا_ماپا, "BISECT_FIRST_PARENT")
ساكن GIT_رستہ_FUNC(git_رستہ_bisect_reset_when_لبھیا, "BISECT_RESET_WHEN_FOUND")
ساكن GIT_رستہ_FUNC(git_رستہ_bisect_لگاتار, "BISECT_RUN")

#تعریف_کرو BUILTIN_GIT_BISECT_شروع_USAGE \
	N_("git bisect start [--term-(bad|new)=<term-new> --term-(good|old)=<term-old>]\n" \
	   "                 [--no-checkout] [--first-parent] [--reset-when-found[=<where>]] [<bad> [<good>...]] [--] [<pathspec>...]")
#تعریف_کرو BUILTIN_GIT_BISECT_BAD_USAGE \
	N_("git bisect (bad|new|<term-new>) [<rev>]")
#تعریف_کرو BUILTIN_GIT_BISECT_GOOD_USAGE \
	N_("git bisect (good|old|<term-old>) [<rev>...]")
#تعریف_کرو BUILTIN_GIT_BISECT_TERMS_USAGE \
	"git bisect terms [--term-(good|old) | --term-(bad|new)]"
#تعریف_کرو BUILTIN_GIT_BISECT_SKIP_USAGE \
	N_("git bisect skip [(<rev>|<range>)...]")
#تعریف_کرو BUILTIN_GIT_BISECT_اگلا_USAGE \
	"git bisect next"
#تعریف_کرو BUILTIN_GIT_BISECT_RESET_USAGE \
	N_("git bisect reset [<commit>]")
#تعریف_کرو BUILTIN_GIT_BISECT_VISUALIZE_USAGE \
	"git bisect (visualize|view)"
#تعریف_کرو BUILTIN_GIT_BISECT_REPLAY_USAGE \
	N_("git bisect replay <logfile>")
#تعریف_کرو BUILTIN_GIT_BISECT_LOG_USAGE \
	"git bisect log"
#تعریف_کرو BUILTIN_GIT_BISECT_لگاتار_USAGE \
	N_("git bisect run [--reset-when-found[=<where>]] <cmd> [<arg>...]")
#تعریف_کرو BUILTIN_GIT_BISECT_HELP_USAGE \
	"git bisect help"

ساكن ثابت حرف * ثابت git_bisect_usage[] = {
	BUILTIN_GIT_BISECT_شروع_USAGE,
	BUILTIN_GIT_BISECT_BAD_USAGE,
	BUILTIN_GIT_BISECT_GOOD_USAGE,
	BUILTIN_GIT_BISECT_TERMS_USAGE,
	BUILTIN_GIT_BISECT_SKIP_USAGE,
	BUILTIN_GIT_BISECT_اگلا_USAGE,
	BUILTIN_GIT_BISECT_RESET_USAGE,
	BUILTIN_GIT_BISECT_VISUALIZE_USAGE,
	BUILTIN_GIT_BISECT_REPLAY_USAGE,
	BUILTIN_GIT_BISECT_LOG_USAGE,
	BUILTIN_GIT_BISECT_لگاتار_USAGE,
	BUILTIN_GIT_BISECT_HELP_USAGE,
	عدم
};

تعداد reset_when_لبھیا_منوال {
	RESET_WHEN_لبھیا_NONE,
	RESET_WHEN_لبھیا_منزل_ORIGINAL,
	RESET_WHEN_لبھیا_منزل_لبھیا,
};

بنية جوڑو_bisect_ref_انکڑے {
	بنية rev_info *revs;
	بنانشانی صحيح object_flags;
};

بنية bisect_terms {
	حرف *رکن_good;
	حرف *رکن_bad;
};

ساكن فراغ free_terms(بنية bisect_terms *terms)
{
	FREE_AND_NULL(terms->رکن_good);
	FREE_AND_NULL(terms->رکن_bad);
}

ساكن فراغ سیٹ_terms(بنية bisect_terms *terms, ثابت حرف *bad,
		      ثابت حرف *good)
{
	حرر((فراغ *)terms->رکن_good);
	terms->رکن_good = xstrdup(good);
	حرر((فراغ *)terms->رکن_bad);
	terms->رکن_bad = xstrdup(bad);
}

ساكن ثابت حرف vocab_bad[] = "bad|new";
ساكن ثابت حرف vocab_good[] = "good|old";

ساكن صحيح bisect_autostart(بنية bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
آخری_ARG_MUST_BE_NULL
ساكن صحيح one_دا(ثابت حرف *رکن, ...)
{
	صحيح res = 0;
	va_list matches;
	ثابت حرف *میل;

	va_شروع(matches, رکن);
	طالما (!res && (میل = va_arg(matches, ثابت حرف *)))
		res = !strcmp(رکن, میل);
	va_سرا(matches);

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
		res == BISECT_اندرونی_SUCCESS_1_ST_BAD_لبھیا ||
		res == BISECT_اندرونی_SUCCESS_ملاؤ_ادھار;
}

ساكن صحيح write_وچ_file(ثابت حرف *رستہ, ثابت حرف *منوال, ثابت حرف *format, va_list دلیلاں)
{
	FILE *fp = عدم;
	صحيح res = 0;

	إذا (strcmp(منوال, "w") && strcmp(منوال, "a"))
		BUG("write-in-file does not support '%s' mode", منوال);
	fp = fopen(رستہ, منوال);
	إذا (!fp)
		أرجع error_errno(_("cannot open file '%s' in mode '%s'"), رستہ, منوال);
	res = vfprintf(fp, format, دلیلاں);

	إذا (res < 0) {
		صحيح saved_errno = errno;
		fclose(fp);
		errno = saved_errno;
		أرجع error_errno(_("could not write to file '%s'"), رستہ);
	}

	أرجع fclose(fp);
}

__attribute__((format (لکھو, 2, 3)))
ساكن صحيح write_منزل_file(ثابت حرف *رستہ, ثابت حرف *format, ...)
{
	صحيح res;
	va_list دلیلاں;

	va_شروع(دلیلاں, format);
	res = write_وچ_file(رستہ, "w", format, دلیلاں);
	va_سرا(دلیلاں);

	أرجع res;
}

__attribute__((format (لکھو, 2, 3)))
ساكن صحيح append_منزل_file(ثابت حرف *رستہ, ثابت حرف *format, ...)
{
	صحيح res;
	va_list دلیلاں;

	va_شروع(دلیلاں, format);
	res = write_وچ_file(رستہ, "a", format, دلیلاں);
	va_سرا(دلیلاں);

	أرجع res;
}

ساكن صحيح print_file_منزل_stdout(ثابت حرف *رستہ)
{
	صحيح fd = open(رستہ, O_RDONLY);
	صحيح ret = 0;

	إذا (fd < 0)
		أرجع error_errno(_("cannot open file '%s' for reading"), رستہ);
	إذا (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), رستہ);
	close(fd);
	أرجع ret;
}

ساكن صحيح check_رکن_format(ثابت حرف *رکن, ثابت حرف *orig_رکن)
{
	صحيح res;
	حرف *new_رکن = xstrfmt("refs/bisect/%s", رکن);

	res = check_refname_format(new_رکن, 0);
	حرر(new_رکن);

	إذا (res)
		أرجع error(_("'%s' is not a valid term"), رکن);

	إذا (one_دا(رکن, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", عدم))
		أرجع error(_("can't use the builtin command '%s' as a term"), رکن);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	إذا ((strcmp(orig_رکن, "bad") && one_دا(رکن, "bad", "new", عدم)) ||
		 (strcmp(orig_رکن, "good") && one_دا(رکن, "good", "old", عدم)))
		أرجع error(_("can't change the meaning of the term '%s'"), رکن);

	أرجع 0;
}

ساكن صحيح write_terms(ثابت حرف *bad, ثابت حرف *good)
{
	صحيح res;

	إذا (!strcmp(bad, good))
		أرجع error(_("please use two different terms"));

	إذا (check_رکن_format(bad, "bad") || check_رکن_format(good, "good"))
		أرجع -1;

	res = write_منزل_file(git_رستہ_bisect_terms(), "%s\n%s\n", bad, good);

	أرجع res;
}

ساكن صحيح bisect_reset(ثابت حرف *commit, bool quiet)
{
	بنية strbuf branch = STRBUF_INIT;

	إذا (!commit) {
		إذا (!strbuf_read_file(&branch, git_رستہ_bisect_شروع(), 0))
			لکھو(_("We are not bisecting.\n"));
		وإلا
			strbuf_rtrim(&branch);
	} وإلا {
		بنية object_id oid;

		إذا (repo_get_oid_commit(the_repository, commit, &oid))
			أرجع error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	إذا (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		بنية بچہ_process cmd = بچہ_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.دلیلاں, "checkout", "--ignore-other-worktrees", عدم);
		إذا (quiet)
			strvec_ڈالو(&cmd.دلیلاں, "--quiet");
		strvec_pushl(&cmd.دلیلاں, branch.buf, "--", عدم);
		إذا (لگاتار_command(&cmd)) {
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

ساكن صحيح parse_reset_when_لبھیا(ثابت حرف *قدر,
				  تعداد reset_when_لبھیا_منوال *منوال)
{
	إذا (!strcmp(قدر, "original"))
		*منوال = RESET_WHEN_لبھیا_منزل_ORIGINAL;
	وإلا إذا (!strcmp(قدر, "found"))
		*منوال = RESET_WHEN_لبھیا_منزل_لبھیا;
	وإلا
		أرجع error(_("invalid value for '--reset-when-found': '%s'"),
			     قدر);

	أرجع 0;
}

ساكن ثابت حرف *reset_when_لبھیا_منوال_name(تعداد reset_when_لبھیا_منوال منوال)
{
	بدل (منوال) {
	حالة RESET_WHEN_لبھیا_منزل_ORIGINAL:
		أرجع "original";
	حالة RESET_WHEN_لبھیا_منزل_لبھیا:
		أرجع "found";
	حالة RESET_WHEN_لبھیا_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", منوال);
}

ساكن صحيح read_reset_when_لبھیا(تعداد reset_when_لبھیا_منوال *منوال)
{
	بنية strbuf قدر = STRBUF_INIT;
	صحيح res = 0;

	*منوال = RESET_WHEN_لبھیا_NONE;
	إذا (is_خالی_تھاواں_or_missing_file(git_رستہ_bisect_reset_when_لبھیا()))
		أرجع 0;

	إذا (strbuf_read_file(&قدر, git_رستہ_bisect_reset_when_لبھیا(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_رستہ_bisect_reset_when_لبھیا());
		اذهب باہرلا;
	}
	strbuf_trim(&قدر);
	إذا (parse_reset_when_لبھیا(قدر.buf, منوال))
		res = -1;

باہرلا:
	strbuf_release(&قدر);
	أرجع res;
}

ساكن صحيح bisect_reset_when_لبھیا(تعداد reset_when_لبھیا_منوال منوال)
{
	بنية bisect_terms terms = { 0 };
	حرف *commit = عدم;
	صحيح res;

	إذا (منوال == RESET_WHEN_لبھیا_منزل_لبھیا) {
		read_bisect_terms(&terms.رکن_bad, &terms.رکن_good);
		commit = xstrfmt("refs/bisect/%s", terms.رکن_bad);
	} وإلا إذا (منوال == RESET_WHEN_لبھیا_NONE) {
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

	repo_format_commit_سنیہا(the_repository, commit, "%s", &commit_پیغام,
				   &pp);

	fprintf(fp, "# %s: [%s] %s\n", label, oid_منزل_hex(&commit->object.oid),
		commit_پیغام.buf);

	strbuf_release(&commit_پیغام);
	حرر(label);
}

ساكن صحيح bisect_write(ثابت حرف *state, ثابت حرف *rev,
			ثابت بنية bisect_terms *terms, صحيح nolog)
{
	بنية strbuf ٹیگ = STRBUF_INIT;
	بنية object_id oid;
	بنية commit *commit;
	FILE *fp = عدم;
	صحيح res = 0;

	إذا (!strcmp(state, terms->رکن_bad)) {
		strbuf_addf(&ٹیگ, "refs/bisect/%s", state);
	} وإلا إذا (one_دا(state, terms->رکن_good, "skip", عدم)) {
		strbuf_addf(&ٹیگ, "refs/bisect/%s-%s", state, rev);
	} وإلا {
		res = error(_("Bad bisect_write argument: %s"), state);
		اذهب انت;
	}

	إذا (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		اذهب انت;
	}

	إذا (refs_update_ref(get_main_ref_store(the_repository), عدم, ٹیگ.buf, &oid, عدم, 0,
			    UPDATE_REFS_پیغام_ON_ERR)) {
		res = -1;
		اذهب انت;
	}

	fp = fopen(git_رستہ_bisect_log(), "a");
	إذا (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_رستہ_bisect_log());
		اذهب انت;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	إذا (!nolog)
		fprintf(fp, "git bisect %s %s\n", state, rev);

انت:
	إذا (fp)
		fclose(fp);
	strbuf_release(&ٹیگ);
	أرجع res;
}

ساكن صحيح check_and_سیٹ_terms(بنية bisect_terms *terms, ثابت حرف *cmd)
{
	صحيح has_رکن_file = !is_خالی_تھاواں_or_missing_file(git_رستہ_bisect_terms());

	إذا (one_دا(cmd, "skip", "start", "terms", عدم))
		أرجع 0;

	إذا (has_رکن_file && strcmp(cmd, terms->رکن_bad) &&
	    strcmp(cmd, terms->رکن_good))
		أرجع error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->رکن_bad,
				terms->رکن_good);

	إذا (!has_رکن_file) {
		إذا (one_دا(cmd, "bad", "good", عدم)) {
			سیٹ_terms(terms, "bad", "good");
			أرجع write_terms(terms->رکن_bad, terms->رکن_good);
		}
		إذا (one_دا(cmd, "new", "old", عدم)) {
			سیٹ_terms(terms, "new", "old");
			أرجع write_terms(terms->رکن_bad, terms->رکن_good);
		}
	}

	أرجع 0;
}

ساكن صحيح inc_nr(ثابت بنية reference *ref بن_ورتیا, فراغ *cb_انکڑے)
{
	بنانشانی صحيح *nr = (بنانشانی صحيح *)cb_انکڑے;
	(*nr)++;
	أرجع 0;
}

ساكن ثابت حرف need_bad_and_good_revision_warning[] =
	N_("You need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

ساكن ثابت حرف need_bisect_شروع_warning[] =
	N_("You need to start by \"git bisect start\".\n"
	   "You then need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

ساكن صحيح decide_اگلا(ثابت بنية bisect_terms *terms,
		       ثابت حرف *current_رکن, صحيح missing_good,
		       صحيح missing_bad)
{
	إذا (!missing_good && !missing_bad)
		أرجع 0;
	إذا (!current_رکن)
		أرجع -1;

	إذا (missing_good && !missing_bad &&
	    !strcmp(current_رکن, terms->رکن_good)) {
		حرف *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->رکن_bad);
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

	إذا (!is_خالی_تھاواں_or_missing_file(git_رستہ_bisect_شروع()))
		أرجع error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	وإلا
		أرجع error(_(need_bisect_شروع_warning),
			     vocab_good, vocab_bad, vocab_good, vocab_bad);
}

ساكن فراغ bisect_حالت(بنية bisect_state *state,
			  ثابت بنية bisect_terms *terms)
{
	حرف *bad_ref = xstrfmt("refs/bisect/%s", terms->رکن_bad);
	حرف *good_glob = xstrfmt("%s-*", terms->رکن_good);
	بنية refs_for_each_ref_options opts = {
		.نمونہ = good_glob,
		.اگیتر = "refs/bisect/",
		.trim_اگیتر = لڑی_لمبائی("refs/bisect/"),
	};

	إذا (refs_ref_exists(get_main_ref_store(the_repository), bad_ref))
		state->nr_bad = 1;

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      inc_nr, &state->nr_good, &opts);

	حرر(good_glob);
	حرر(bad_ref);
}

__attribute__((format (لکھو, 1, 2)))
ساكن فراغ bisect_log_printf(ثابت حرف *fmt, ...)
{
	بنية strbuf buf = STRBUF_INIT;
	va_list ap;

	va_شروع(ap, fmt);
	strbuf_vaddf(&buf, fmt, ap);
	va_سرا(ap);

	لکھو("%s", buf.buf);
	append_منزل_file(git_رستہ_bisect_log(), "# %s", buf.buf);

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
				  terms->رکن_good, terms->رکن_bad);
	وإلا إذا (state.nr_good)
		bisect_log_printf(Q_("status: waiting for '%s' commit, %d '%s' commit known\n",
				     "status: waiting for '%s' commit, %d '%s' commits known\n",
				     state.nr_good),
				  terms->رکن_bad, state.nr_good, terms->رکن_good);
	وإلا
		bisect_log_printf(_("status: waiting for '%s' commit(s), '%s' commit known\n"),
				  terms->رکن_good, terms->رکن_bad);
}

ساكن صحيح bisect_اگلا_check(ثابت بنية bisect_terms *terms,
			     ثابت حرف *current_رکن)
{
	بنية bisect_state state = { 0 };
	bisect_حالت(&state, terms);
	أرجع decide_اگلا(terms, current_رکن, !state.nr_good, !state.nr_bad);
}

ساكن صحيح get_terms(بنية bisect_terms *terms, صحيح file_missing_is_ok)
{
	بنية strbuf str = STRBUF_INIT;
	FILE *fp = عدم;
	صحيح res = 0;

	fp = fopen(git_رستہ_bisect_terms(), "r");
	إذا (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		اذهب انت;
	}

	free_terms(terms);
	إذا (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		اذهب انت;
	}
	terms->رکن_bad = strbuf_detach(&str, عدم);
	إذا (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		FREE_AND_NULL(terms->رکن_bad);
		اذهب انت;
	}
	terms->رکن_good = strbuf_detach(&str, عدم);

انت:
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
		لکھو(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->رکن_good, terms->رکن_bad);
		أرجع 0;
	}
	إذا (one_دا(option, "--term-good", "--term-old", عدم))
		لکھو("%s\n", terms->رکن_good);
	وإلا إذا (one_دا(option, "--term-bad", "--term-new", عدم))
		لکھو("%s\n", terms->رکن_bad);
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
	FILE *fp = fopen(git_رستہ_bisect_log(), "a");
	بنية strbuf orig_دلیلاں = STRBUF_INIT;

	إذا (!fp)
		أرجع -1;

	إذا (fprintf(fp, "git bisect start") < 1) {
		res = -1;
		اذهب انت;
	}

	sq_quote_argv(&orig_دلیلاں, argv);
	إذا (fprintf(fp, "%s\n", orig_دلیلاں.buf) < 1)
		res = -1;

انت:
	fclose(fp);
	strbuf_release(&orig_دلیلاں);
	أرجع res;
}

ساكن صحيح جوڑو_bisect_ref(ثابت بنية reference *ref, فراغ *cb)
{
	بنية جوڑو_bisect_ref_انکڑے *انکڑے = cb;

	جوڑو_pending_oid(انکڑے->revs, ref->name, ref->oid, انکڑے->object_flags);

	أرجع 0;
}

ساكن صحيح prepare_revs(بنية bisect_terms *terms, بنية rev_info *revs)
{
	بنية refs_for_each_ref_options opts = {
		.اگیتر = "refs/bisect/",
		.trim_اگیتر = لڑی_لمبائی("refs/bisect/"),
	};
	صحيح res = 0;
	بنية جوڑو_bisect_ref_انکڑے cb = { revs };
	حرف *good = xstrfmt("%s-*", terms->رکن_good);

	/*
	 * We cannot use terms->term_bad directly in
	 * for_each_glob_ref_in() and we have to append a '*' to it,
	 * otherwise for_each_glob_ref_in() will append '/' and '*'.
	 */
	حرف *bad = xstrfmt("%s*", terms->رکن_bad);

	/*
	 * It is important to reset the flags used by revision walks
	 * as the previous call to bisect_next_all() in turn
	 * sets up a revision walk.
	 */
	reset_revision_walk();
	repo_init_revisions(the_repository, revs, عدم);
	تیاری_revisions(0, عدم, revs, عدم);

	opts.نمونہ = bad;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      جوڑو_bisect_ref, &cb, &opts);

	cb.object_flags = UNINTERESTING;
	opts.نمونہ = good;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      جوڑو_bisect_ref, &cb, &opts);

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

	fp = fopen(git_رستہ_bisect_log(), "a");
	إذا (!fp)
		أرجع error_errno(_("could not open '%s' for appending"),
				  git_رستہ_bisect_log());

	إذا (fprintf(fp, "# only skipped commits left to test\n") < 0)
		أرجع error_errno(_("failed to write to '%s'"), git_رستہ_bisect_log());

	طالما ((commit = get_revision(&revs)) != عدم) {
		strbuf_reset(&commit_name);
		repo_format_commit_سنیہا(the_repository, commit, "%s",
					   &commit_name, &pp);
		fprintf(fp, "# possible first '%s' commit: [%s] %s\n",
			terms->رکن_bad, oid_منزل_hex(&commit->object.oid),
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
	حرف *bad_ref = xstrfmt("refs/bisect/%s",terms->رکن_bad);
	صحيح res;

	refs_read_ref(get_main_ref_store(the_repository), bad_ref, &oid);
	commit = lookup_commit_reference_by_name(bad_ref);
	إذا (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		حرر(bad_ref);
		أرجع BISECT_FAILED;
	}
	repo_format_commit_سنیہا(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_منزل_file(git_رستہ_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->رکن_bad, oid_منزل_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	حرر(bad_ref);
	أرجع res;
}

ساكن تعداد bisect_error bisect_اگلا(بنية bisect_terms *terms,
				     ثابت حرف *اگیتر)
{
	تعداد bisect_error res;

	إذا (bisect_autostart(terms))
		أرجع BISECT_FAILED;

	إذا (bisect_اگلا_check(terms, terms->رکن_good))
		أرجع BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_اگلا_all(the_repository, اگیتر);

	إذا (res == BISECT_اندرونی_SUCCESS_1_ST_BAD_لبھیا) {
		res = bisect_successful(terms);
		أرجع res ? res : BISECT_اندرونی_SUCCESS_1_ST_BAD_لبھیا;
	} وإلا إذا (res == BISECT_ONLY_SKIPPED_بایاں) {
		res = bisect_skipped_commits(terms);
		أرجع res ? res : BISECT_ONLY_SKIPPED_بایاں;
	}
	أرجع res;
}

ساكن تعداد bisect_error bisect_auto_اگلا(بنية bisect_terms *terms,
					  ثابت حرف *اگیتر)
{
	إذا (bisect_اگلا_check(terms, عدم)) {
		bisect_print_حالت(terms);
		أرجع BISECT_OK;
	}

	أرجع bisect_اگلا(terms, اگیتر);
}

ساكن تعداد bisect_error bisect_شروع(بنية bisect_terms *terms, صحيح argc,
				      ثابت حرف **argv)
{
	صحيح no_checkout = 0;
	صحيح پہلا_ماپا_only = 0;
	صحيح i, has_double_dash = 0, must_write_terms = 0, bad_تکیا = 0;
	صحيح flags, pathspec_pos;
	تعداد bisect_error res = BISECT_OK;
	بنية string_list revs = STRING_LIST_INIT_DUP;
	بنية string_list حالتاں = STRING_LIST_INIT_DUP;
	بنية strbuf شروع_سر = STRBUF_INIT;
	بنية strbuf bisect_ناں = STRBUF_INIT;
	بنية object_id سر_oid;
	بنية object_id oid;
	تعداد reset_when_لبھیا_منوال reset_when_لبھیا = RESET_WHEN_لبھیا_NONE;
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
			پہلا_ماپا_only = 1;
		} وإلا إذا (!strcmp(arg, "--reset-when-found")) {
			reset_when_لبھیا = RESET_WHEN_لبھیا_منزل_ORIGINAL;
		} وإلا إذا (skip_اگیتر(arg, "--reset-when-found=", &arg)) {
			إذا (parse_reset_when_لبھیا(arg, &reset_when_لبھیا)) {
				res = BISECT_FAILED;
				اذهب انت;
			}
		} وإلا إذا (!strcmp(arg, "--term-good") ||
			 !strcmp(arg, "--term-old")) {
			i++;
			إذا (argc <= i)
				أرجع error(_("'' is not a valid term"));
			must_write_terms = 1;
			حرر((فراغ *) terms->رکن_good);
			terms->رکن_good = xstrdup(argv[i]);
		} وإلا إذا (skip_اگیتر(arg, "--term-good=", &arg) ||
			   skip_اگیتر(arg, "--term-old=", &arg)) {
			must_write_terms = 1;
			حرر((فراغ *) terms->رکن_good);
			terms->رکن_good = xstrdup(arg);
		} وإلا إذا (!strcmp(arg, "--term-bad") ||
			 !strcmp(arg, "--term-new")) {
			i++;
			إذا (argc <= i)
				أرجع error(_("'' is not a valid term"));
			must_write_terms = 1;
			حرر((فراغ *) terms->رکن_bad);
			terms->رکن_bad = xstrdup(argv[i]);
		} وإلا إذا (skip_اگیتر(arg, "--term-bad=", &arg) ||
			   skip_اگیتر(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			حرر((فراغ *) terms->رکن_bad);
			terms->رکن_bad = xstrdup(arg);
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
	إذا (reset_when_لبھیا != RESET_WHEN_لبھیا_NONE && no_checkout) {
		res = error(_("options '%s' and '%s' cannot be used together"),
			    "--reset-when-found", "--no-checkout");
		اذهب انت;
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
		إذا (bad_تکیا) {
			string_list_append(&حالتاں, terms->رکن_good);
		} وإلا {
			bad_تکیا = 1;
			string_list_append(&حالتاں, terms->رکن_bad);
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
	إذا (!is_خالی_تھاواں_or_missing_file(git_رستہ_bisect_شروع())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&شروع_سر, git_رستہ_bisect_شروع(), 0);
		strbuf_trim(&شروع_سر);
		إذا (!no_checkout) {
			بنية بچہ_process cmd = بچہ_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.دلیلاں, "checkout", شروع_سر.buf,
				     "--", عدم);
			إذا (لگاتار_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       شروع_سر.buf);
				اذهب انت;
			}
		}
	} وإلا {
		/* Get the rev from where we start. */
		إذا (!repo_get_oid(the_repository, سر, &سر_oid) &&
		    !starts_with(سر, "refs/heads/")) {
			strbuf_reset(&شروع_سر);
			strbuf_جوڑو_oid_hex(&شروع_سر, &سر_oid);
		} وإلا إذا (!repo_get_oid(the_repository, سر, &سر_oid) &&
			   skip_اگیتر(سر, "refs/heads/", &سر)) {
			strbuf_addstr(&شروع_سر, سر);
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
	write_file(git_رستہ_bisect_شروع(), "%s\n", شروع_سر.buf);

	إذا (پہلا_ماپا_only)
		write_file(git_رستہ_bisect_پہلا_ماپا(), "\n");

	إذا (reset_when_لبھیا != RESET_WHEN_لبھیا_NONE)
		write_file(git_رستہ_bisect_reset_when_لبھیا(), "%s\n",
			   reset_when_لبھیا_منوال_name(reset_when_لبھیا));

	إذا (no_checkout) {
		إذا (repo_get_oid(the_repository, شروع_سر.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), شروع_سر.buf);
			اذهب انت;
		}
		إذا (refs_update_ref(get_main_ref_store(the_repository), عدم, "BISECT_HEAD", &oid, عدم, 0,
				    UPDATE_REFS_پیغام_ON_ERR)) {
			res = BISECT_FAILED;
			اذهب انت;
		}
	}

	إذا (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_ناں, argv + pathspec_pos);
	write_file(git_رستہ_bisect_ناں(), "%s\n", bisect_ناں.buf);

	لكل (i = 0; i < حالتاں.nr; i++)
		إذا (bisect_write(حالتاں.اشیاء[i].string,
				 revs.اشیاء[i].string, terms, 1)) {
			res = BISECT_FAILED;
			اذهب انت;
		}

	إذا (must_write_terms && write_terms(terms->رکن_bad,
					    terms->رکن_good)) {
		res = BISECT_FAILED;
		اذهب انت;
	}

	res = bisect_append_log_quoted(argv);
	إذا (res)
		res = BISECT_FAILED;

انت:
	string_list_clear(&revs, 0);
	string_list_clear(&حالتاں, 0);
	strbuf_release(&شروع_سر);
	strbuf_release(&bisect_ناں);
	إذا (res)
		أرجع res;

	res = bisect_auto_اگلا(terms, عدم);
	إذا (!is_bisect_success(res))
		bisect_clean_state();
	أرجع res;
}

ساكن مضمن صحيح file_is_not_خالی_تھاواں(ثابت حرف *رستہ)
{
	أرجع !is_خالی_تھاواں_or_missing_file(رستہ);
}

ساكن صحيح bisect_autostart(بنية bisect_terms *terms)
{
	صحيح res;
	ثابت حرف *yesno;

	إذا (file_is_not_خالی_تھاواں(git_رستہ_bisect_شروع()))
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
		-1 : bisect_شروع(terms, 0, خالی_تھاواں_strvec);

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
	إذا (check_and_سیٹ_terms(terms, state) ||
	    !one_دا(state, terms->رکن_good, terms->رکن_bad, "skip", عدم))
		أرجع BISECT_FAILED;

	argv++;
	argc--;
	إذا (argc > 1 && !strcmp(state, terms->رکن_bad))
		أرجع error(_("'git bisect %s' can take only one argument."), terms->رکن_bad);

	إذا (argc == 0) {
		ثابت حرف *سر = "BISECT_HEAD";
		تعداد get_oid_نتیجہ res_سر = repo_get_oid(the_repository,
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
			unlink_or_warn(git_رستہ_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					عدم, "BISECT_EXPECTED_REV", عدم,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	أرجع bisect_auto_اگلا(terms, عدم);
}

ساكن تعداد bisect_error bisect_log(فراغ)
{
	صحيح fd, حالت;
	ثابت حرف* filename = git_رستہ_bisect_log();

	إذا (is_خالی_تھاواں_or_missing_file(filename))
		أرجع error(_("We are not bisecting."));

	fd = open(filename, O_RDONLY);
	إذا (fd < 0)
		أرجع BISECT_FAILED;

	حالت = copy_fd(fd, STDOUT_FILENO);
	close(fd);
	أرجع حالت ? BISECT_FAILED : BISECT_OK;
}

ساكن صحيح process_replay_سطر(بنية bisect_terms *terms, بنية strbuf *سطر)
{
	ثابت حرف *p = سطر->buf + strspn(سطر->buf, " \t");
	حرف *لفظ_سرا, *rev;

	إذا ((!skip_اگیتر(p, "git bisect", &p) &&
	!skip_اگیتر(p, "git-bisect", &p)) || !خلا_اے(*p))
		أرجع 0;
	p += strspn(p, " \t");

	لفظ_سرا = (حرف *)p + strcspn(p, " \t");
	rev = لفظ_سرا + strspn(لفظ_سرا, " \t");
	*لفظ_سرا = '\0'; /* NUL-terminate the word */

	إذا (get_terms(terms, 1))
		أرجع error(_("no terms defined"));
	إذا (check_and_سیٹ_terms(terms, p))
		أرجع -1;

	إذا (!strcmp(p, "start")) {
		بنية strvec argv = STRVEC_INIT;
		صحيح res;
		sq_dequote_منزل_strvec(rev, &argv);
		res = bisect_شروع(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		أرجع res;
	}

	إذا (one_دا(p, terms->رکن_good,
	   terms->رکن_bad, "skip", عدم))
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
	بنية strbuf سطر = STRBUF_INIT;

	إذا (is_خالی_تھاواں_or_missing_file(filename))
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

	أرجع bisect_auto_اگلا(terms, عدم);
}

ساكن تعداد bisect_error bisect_skip(بنية bisect_terms *terms, صحيح argc,
				     ثابت حرف **argv)
{
	صحيح i;
	تعداد bisect_error res;
	بنية strvec argv_state = STRVEC_INIT;

	strvec_ڈالو(&argv_state, "skip");

	لكل (i = 0; i < argc; i++) {
		ثابت حرف *dotdot = لڑی_کھوج(argv[i], "..");

		إذا (dotdot) {
			بنية rev_info revs;
			بنية commit *commit;

			repo_init_revisions(the_repository, &revs, عدم);
			تیاری_revisions(2, argv + i - 1, &revs, عدم);

			إذا (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			طالما ((commit = get_revision(&revs)) != عدم)
				strvec_ڈالو(&argv_state,
						oid_منزل_hex(&commit->object.oid));

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
	بنية بچہ_process cmd = بچہ_PROCESS_INIT;
	بنية strbuf sb = STRBUF_INIT;

	إذا (bisect_اگلا_check(terms, عدم) != 0)
		أرجع BISECT_FAILED;

	cmd.no_stdin = 1;
	إذا (!argc) {
		إذا ((getenv("DISPLAY") || getenv("SESSIONNAME") || getenv("MSYSTEM") ||
		     getenv("SECURITYSESSIONID")) && exists_وچ_رستہ("gitk")) {
			strvec_ڈالو(&cmd.دلیلاں, "gitk");
		} وإلا {
			strvec_ڈالو(&cmd.دلیلاں, "log");
			cmd.git_cmd = 1;
		}
	} وإلا {
		إذا (argv[0][0] == '-') {
			strvec_ڈالو(&cmd.دلیلاں, "log");
			cmd.git_cmd = 1;
		} وإلا إذا (strcmp(argv[0], "tig") && !starts_with(argv[0], "git"))
			cmd.git_cmd = 1;

		strvec_pushv(&cmd.دلیلاں, argv);
	}

	strvec_pushl(&cmd.دلیلاں, "--bisect", "--", عدم);

	strbuf_read_file(&sb, git_رستہ_bisect_ناں(), 0);
	sq_dequote_منزل_strvec(sb.buf, &cmd.دلیلاں);
	strbuf_release(&sb);

	أرجع لگاتار_command(&cmd);
}

ساكن صحيح get_پہلا_good(ثابت بنية reference *ref, فراغ *cb_انکڑے)
{
	oidcpy(cb_انکڑے, ref->oid);
	أرجع 1;
}

ساكن صحيح do_bisect_لگاتار(ثابت حرف *command)
{
	بنية بچہ_process cmd = بچہ_PROCESS_INIT;

	لکھو(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_ڈالو(&cmd.دلیلاں, command);
	أرجع لگاتار_command(&cmd);
}

ساكن صحيح verify_good(ثابت بنية bisect_terms *terms, ثابت حرف *command)
{
	صحيح rc;
	تعداد bisect_error res;
	بنية object_id good_rev;
	بنية object_id current_rev;
	حرف *good_glob = xstrfmt("%s-*", terms->رکن_good);
	صحيح no_checkout = refs_ref_exists(get_main_ref_store(the_repository),
					  "BISECT_HEAD");
	بنية refs_for_each_ref_options opts = {
		.نمونہ = good_glob,
		.اگیتر = "refs/bisect/",
		.trim_اگیتر = لڑی_لمبائی("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_پہلا_good, &good_rev, &opts);
	حرر(good_glob);

	إذا (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		أرجع -1;

	res = bisect_checkout(&good_rev, no_checkout);
	إذا (res != BISECT_OK)
		أرجع -1;

	rc = do_bisect_لگاتار(command);

	res = bisect_checkout(&current_rev, no_checkout);
	إذا (res != BISECT_OK)
		أرجع -1;

	أرجع rc;
}

ساكن صحيح bisect_لگاتار(بنية bisect_terms *terms, صحيح argc, ثابت حرف **argv)
{
	صحيح res = BISECT_OK;
	بنية strbuf command = STRBUF_INIT;
	ثابت حرف *reset_when_لبھیا_arg;
	ثابت حرف *new_state;
	صحيح temporary_stdout_fd, saved_stdout;
	صحيح is_پہلا_لگاتار = 1;
	تعداد reset_when_لبھیا_منوال reset_when_لبھیا = RESET_WHEN_لبھیا_NONE;

	إذا (bisect_اگلا_check(terms, عدم))
		أرجع BISECT_FAILED;

	إذا (argc && !strcmp(argv[0], "--reset-when-found")) {
		reset_when_لبھیا = RESET_WHEN_لبھیا_منزل_ORIGINAL;
	} وإلا إذا (argc && skip_اگیتر(argv[0], "--reset-when-found=",
				    &reset_when_لبھیا_arg)) {
		إذا (parse_reset_when_لبھیا(reset_when_لبھیا_arg,
					   &reset_when_لبھیا))
			أرجع BISECT_FAILED;
	}

	إذا (reset_when_لبھیا != RESET_WHEN_لبھیا_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		أرجع error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	إذا (reset_when_لبھیا != RESET_WHEN_لبھیا_NONE) {
		write_file(git_رستہ_bisect_reset_when_لبھیا(), "%s\n",
			   reset_when_لبھیا_منوال_name(reset_when_لبھیا));
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
		res = do_bisect_لگاتار(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		إذا (is_پہلا_لگاتار && (res == 126 || res == 127)) {
			صحيح rc = verify_good(terms, command.buf);
			is_پہلا_لگاتار = 0;
			إذا (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->رکن_good);
				res = BISECT_FAILED;
				اكسر;
			}
			إذا (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->رکن_good);
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
			new_state = terms->رکن_good;
		وإلا
			new_state = terms->رکن_bad;

		temporary_stdout_fd = open(git_رستہ_bisect_لگاتار(), O_CREAT | O_WRONLY | O_TRUNC, 0666);

		إذا (temporary_stdout_fd < 0) {
			res = error_errno(_("cannot open file '%s' for writing"), git_رستہ_bisect_لگاتار());
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

		print_file_منزل_stdout(git_رستہ_bisect_لگاتار());

		إذا (res == BISECT_ONLY_SKIPPED_بایاں)
			error(_("bisect run cannot continue any more"));
		وإلا إذا (res == BISECT_اندرونی_SUCCESS_ملاؤ_ادھار) {
			puts(_("bisect run success"));
			res = BISECT_OK;
		} وإلا إذا (res == BISECT_اندرونی_SUCCESS_1_ST_BAD_لبھیا) {
			لکھو(_("bisect found first '%s' commit\n"), terms->رکن_bad);
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

ساكن صحيح cmd_bisect__reset(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر بن_ورتیا,
			     بنية repository *repo بن_ورتیا)
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

ساكن صحيح cmd_bisect__terms(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر بن_ورتیا,
			     بنية repository *repo بن_ورتیا)
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

ساكن صحيح cmd_bisect_شروع(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر بن_ورتیا,
			     بنية repository *repo بن_ورتیا)
{
	صحيح res;
	بنية bisect_terms terms = { 0 };

	سیٹ_terms(&terms, "bad", "good");
	res = bisect_شروع(&terms, argc, argv);
	free_terms(&terms);
	أرجع res;
}

ساكن صحيح cmd_bisect_اگلا(صحيح argc, ثابت حرف **argv بن_ورتیا, ثابت حرف *اگیتر,
			    بنية repository *repo بن_ورتیا)
{
	صحيح res;
	بنية bisect_terms terms = { 0 };

	إذا (argc)
		أرجع error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	إذا (get_terms(&terms, 1))
		أرجع error(_("no terms defined"));
	res = bisect_اگلا(&terms, اگیتر);
	free_terms(&terms);
	أرجع res;
}

ساكن صحيح cmd_bisect__log(صحيح argc بن_ورتیا, ثابت حرف **argv بن_ورتیا,
			   ثابت حرف *اگیتر بن_ورتیا,
			   بنية repository *repo بن_ورتیا)
{
	أرجع bisect_log();
}

ساكن صحيح cmd_bisect__replay(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر بن_ورتیا,
			      بنية repository *repo بن_ورتیا)
{
	صحيح res;
	بنية bisect_terms terms = { 0 };

	إذا (argc != 1)
		أرجع error(_("no logfile given"));
	سیٹ_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	أرجع res;
}

ساكن صحيح cmd_bisect__skip(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر بن_ورتیا,
			    بنية repository *repo بن_ورتیا)
{
	صحيح res;
	بنية bisect_terms terms = { 0 };

	سیٹ_terms(&terms, "bad", "good");
	إذا (get_terms(&terms, 1))
		أرجع error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	أرجع res;
}

ساكن صحيح cmd_bisect__visualize(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر بن_ورتیا,
				 بنية repository *repo بن_ورتیا)
{
	صحيح res;
	بنية bisect_terms terms = { 0 };

	إذا (get_terms(&terms, 1))
		أرجع error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	أرجع res;
}

ساكن صحيح cmd_bisect_لگاتار(صحيح argc, ثابت حرف **argv, ثابت حرف *اگیتر بن_ورتیا,
			   بنية repository *repo بن_ورتیا)
{
	صحيح res;
	بنية bisect_terms terms = { 0 };

	إذا (!argc)
		أرجع error(_("'%s' failed: no command provided."), "git bisect run");
	إذا (get_terms(&terms, 1))
		أرجع error(_("no terms defined"));
	res = bisect_لگاتار(&terms, argc, argv);
	free_terms(&terms);
	أرجع res;
}

صحيح cmd_bisect(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *اگیتر,
	       بنية repository *repo)
{
	صحيح res = 0;
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT_SUBCOMMAND("reset", &fn, cmd_bisect__reset),
		OPT_SUBCOMMAND("terms", &fn, cmd_bisect__terms),
		OPT_SUBCOMMAND("start", &fn, cmd_bisect_شروع),
		OPT_SUBCOMMAND("next", &fn, cmd_bisect_اگلا),
		OPT_SUBCOMMAND("log", &fn, cmd_bisect__log),
		OPT_SUBCOMMAND("replay", &fn, cmd_bisect__replay),
		OPT_SUBCOMMAND("skip", &fn, cmd_bisect__skip),
		OPT_SUBCOMMAND("visualize", &fn, cmd_bisect__visualize),
		OPT_SUBCOMMAND("view", &fn, cmd_bisect__visualize),
		OPT_SUBCOMMAND("run", &fn, cmd_bisect_لگاتار),
		OPT_سرا()
	};
	argc = parse_options(argc, argv, اگیتر, options, git_bisect_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);

	إذا (!fn) {
		بنية bisect_terms terms = { 0 };

		إذا (!argc)
			usage_پیغام_opt(_("need a command"), git_bisect_usage, options);

		إذا (!strcmp(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		سیٹ_terms(&terms, "bad", "good");
		إذا (get_terms(&terms, 1))
			أرجع error(_("no terms defined"));
		إذا (check_and_سیٹ_terms(&terms, argv[0]) ||
		    !one_دا(argv[0], terms.رکن_good, terms.رکن_bad, عدم))
			usage_پیغام_optf(_("unknown command: '%s'"), git_bisect_usage,
				       options, argv[0]);
		وإلا
			res = bisect_state(&terms, argc, argv);
		free_terms(&terms);
	} وإلا {
		argc--;
		argv++;
		res = fn(argc, argv, اگیتر, repo);
	}

	إذا (res == BISECT_اندرونی_SUCCESS_1_ST_BAD_لبھیا) {
		تعداد reset_when_لبھیا_منوال منوال;

		إذا (read_reset_when_لبھیا(&منوال))
			res = BISECT_FAILED;
		وإلا إذا (منوال != RESET_WHEN_لبھیا_NONE &&
			 bisect_reset_when_لبھیا(منوال))
			res = BISECT_FAILED;
	}

	أرجع is_bisect_success(res) ? 0 : -res;
}
