#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "copy.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "object-name.h"
#شامل "parse-options.h"
#شامل "bisect.h"
#شامل "refs.h"
#شامل "strvec.h"
#شامل "run-command.h"
#شامل "oid-array.h"
#شامل "path.h"
#شامل "prompt.h"
#شامل "quote.h"
#شامل "revision.h"

ساکن GIT_راستہ_FUNC(git_راستہ_bisect_terms, "BISECT_TERMS")
ساکن GIT_راستہ_FUNC(git_راستہ_bisect_ancestors_ok, "BISECT_ANCESTORS_OK")
ساکن GIT_راستہ_FUNC(git_راستہ_bisect_آغاز, "BISECT_START")
ساکن GIT_راستہ_FUNC(git_راستہ_bisect_log, "BISECT_LOG")
ساکن GIT_راستہ_FUNC(git_راستہ_bisect_نام, "BISECT_NAMES")
ساکن GIT_راستہ_FUNC(git_راستہ_bisect_پہلا_والد, "BISECT_FIRST_PARENT")
ساکن GIT_راستہ_FUNC(git_راستہ_bisect_reset_when_ملا, "BISECT_RESET_WHEN_FOUND")
ساکن GIT_راستہ_FUNC(git_راستہ_bisect_تسلسل, "BISECT_RUN")

#تعریف_کرو BUILTIN_GIT_BISECT_آغاز_USAGE \
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
#تعریف_کرو BUILTIN_GIT_BISECT_تسلسل_USAGE \
	N_("git bisect run [--reset-when-found[=<where>]] <cmd> [<arg>...]")
#تعریف_کرو BUILTIN_GIT_BISECT_HELP_USAGE \
	"git bisect help"

ساکن ثابت حرف * ثابت git_bisect_usage[] = {
	BUILTIN_GIT_BISECT_آغاز_USAGE,
	BUILTIN_GIT_BISECT_BAD_USAGE,
	BUILTIN_GIT_BISECT_GOOD_USAGE,
	BUILTIN_GIT_BISECT_TERMS_USAGE,
	BUILTIN_GIT_BISECT_SKIP_USAGE,
	BUILTIN_GIT_BISECT_اگلا_USAGE,
	BUILTIN_GIT_BISECT_RESET_USAGE,
	BUILTIN_GIT_BISECT_VISUALIZE_USAGE,
	BUILTIN_GIT_BISECT_REPLAY_USAGE,
	BUILTIN_GIT_BISECT_LOG_USAGE,
	BUILTIN_GIT_BISECT_تسلسل_USAGE,
	BUILTIN_GIT_BISECT_HELP_USAGE,
	تہی
};

شمار reset_when_ملا_منوال {
	RESET_WHEN_ملا_NONE,
	RESET_WHEN_ملا_مقصد_ORIGINAL,
	RESET_WHEN_ملا_مقصد_ملا,
};

ساخت جوڑو_bisect_ref_ڈیٹا {
	ساخت rev_info *revs;
	غیرعلامتی صحیح object_flags;
};

ساخت bisect_terms {
	حرف *رکن_good;
	حرف *رکن_bad;
};

ساکن خالی free_terms(ساخت bisect_terms *terms)
{
	FREE_AND_NULL(terms->رکن_good);
	FREE_AND_NULL(terms->رکن_bad);
}

ساکن خالی سیٹ_terms(ساخت bisect_terms *terms, ثابت حرف *bad,
		      ثابت حرف *good)
{
	آزاد((خالی *)terms->رکن_good);
	terms->رکن_good = xstrdup(good);
	آزاد((خالی *)terms->رکن_bad);
	terms->رکن_bad = xstrdup(bad);
}

ساکن ثابت حرف vocab_bad[] = "bad|new";
ساکن ثابت حرف vocab_good[] = "good|old";

ساکن صحیح bisect_autostart(ساخت bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
آخری_ARG_MUST_BE_NULL
ساکن صحیح one_کا(ثابت حرف *رکن, ...)
{
	صحیح res = 0;
	va_list matches;
	ثابت حرف *مطابقت;

	va_آغاز(matches, رکن);
	جب (!res && (مطابقت = va_arg(matches, ثابت حرف *)))
		res = !strcmp(رکن, مطابقت);
	va_انتہا(matches);

	واپس res;
}

/*
 * return code BISECT_INTERNAL_SUCCESS_MERGE_BASE
 * and BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND are codes
 * that indicate special success.
 */

ساکن صحیح is_bisect_success(شمار bisect_error res)
{
	واپس !res ||
		res == BISECT_اندرونی_SUCCESS_1_ST_BAD_ملا ||
		res == BISECT_اندرونی_SUCCESS_ملاؤ_بنیاد;
}

ساکن صحیح write_اندر_file(ثابت حرف *راستہ, ثابت حرف *منوال, ثابت حرف *format, va_list دلائل)
{
	FILE *fp = تہی;
	صحیح res = 0;

	اگر (strcmp(منوال, "w") && strcmp(منوال, "a"))
		BUG("write-in-file does not support '%s' mode", منوال);
	fp = fopen(راستہ, منوال);
	اگر (!fp)
		واپس error_errno(_("cannot open file '%s' in mode '%s'"), راستہ, منوال);
	res = vfprintf(fp, format, دلائل);

	اگر (res < 0) {
		صحیح saved_errno = errno;
		fclose(fp);
		errno = saved_errno;
		واپس error_errno(_("could not write to file '%s'"), راستہ);
	}

	واپس fclose(fp);
}

__attribute__((format (لکھو, 2, 3)))
ساکن صحیح write_مقصد_file(ثابت حرف *راستہ, ثابت حرف *format, ...)
{
	صحیح res;
	va_list دلائل;

	va_آغاز(دلائل, format);
	res = write_اندر_file(راستہ, "w", format, دلائل);
	va_انتہا(دلائل);

	واپس res;
}

__attribute__((format (لکھو, 2, 3)))
ساکن صحیح append_مقصد_file(ثابت حرف *راستہ, ثابت حرف *format, ...)
{
	صحیح res;
	va_list دلائل;

	va_آغاز(دلائل, format);
	res = write_اندر_file(راستہ, "a", format, دلائل);
	va_انتہا(دلائل);

	واپس res;
}

ساکن صحیح print_file_مقصد_stdout(ثابت حرف *راستہ)
{
	صحیح fd = open(راستہ, O_RDONLY);
	صحیح ret = 0;

	اگر (fd < 0)
		واپس error_errno(_("cannot open file '%s' for reading"), راستہ);
	اگر (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), راستہ);
	close(fd);
	واپس ret;
}

ساکن صحیح check_رکن_format(ثابت حرف *رکن, ثابت حرف *orig_رکن)
{
	صحیح res;
	حرف *new_رکن = xstrfmt("refs/bisect/%s", رکن);

	res = check_refname_format(new_رکن, 0);
	آزاد(new_رکن);

	اگر (res)
		واپس error(_("'%s' is not a valid term"), رکن);

	اگر (one_کا(رکن, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", تہی))
		واپس error(_("can't use the builtin command '%s' as a term"), رکن);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	اگر ((strcmp(orig_رکن, "bad") && one_کا(رکن, "bad", "new", تہی)) ||
		 (strcmp(orig_رکن, "good") && one_کا(رکن, "good", "old", تہی)))
		واپس error(_("can't change the meaning of the term '%s'"), رکن);

	واپس 0;
}

ساکن صحیح write_terms(ثابت حرف *bad, ثابت حرف *good)
{
	صحیح res;

	اگر (!strcmp(bad, good))
		واپس error(_("please use two different terms"));

	اگر (check_رکن_format(bad, "bad") || check_رکن_format(good, "good"))
		واپس -1;

	res = write_مقصد_file(git_راستہ_bisect_terms(), "%s\n%s\n", bad, good);

	واپس res;
}

ساکن صحیح bisect_reset(ثابت حرف *commit, bool quiet)
{
	ساخت strbuf branch = STRBUF_INIT;

	اگر (!commit) {
		اگر (!strbuf_read_file(&branch, git_راستہ_bisect_آغاز(), 0))
			لکھو(_("We are not bisecting.\n"));
		ورنہ
			strbuf_rtrim(&branch);
	} ورنہ {
		ساخت object_id oid;

		اگر (repo_get_oid_commit(the_repository, commit, &oid))
			واپس error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	اگر (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		ساخت بچہ_process cmd = بچہ_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.دلائل, "checkout", "--ignore-other-worktrees", تہی);
		اگر (quiet)
			strvec_ڈالو(&cmd.دلائل, "--quiet");
		strvec_pushl(&cmd.دلائل, branch.buf, "--", تہی);
		اگر (تسلسل_command(&cmd)) {
			error(_("could not check out original"
				" HEAD '%s'. Try 'git bisect"
				" reset <commit>'."), branch.buf);
			strbuf_release(&branch);
			واپس -1;
		}
	}

	strbuf_release(&branch);
	واپس 0;
}

ساکن صحیح parse_reset_when_ملا(ثابت حرف *قدر,
				  شمار reset_when_ملا_منوال *منوال)
{
	اگر (!strcmp(قدر, "original"))
		*منوال = RESET_WHEN_ملا_مقصد_ORIGINAL;
	ورنہ اگر (!strcmp(قدر, "found"))
		*منوال = RESET_WHEN_ملا_مقصد_ملا;
	ورنہ
		واپس error(_("invalid value for '--reset-when-found': '%s'"),
			     قدر);

	واپس 0;
}

ساکن ثابت حرف *reset_when_ملا_منوال_name(شمار reset_when_ملا_منوال منوال)
{
	انتخاب (منوال) {
	حالت RESET_WHEN_ملا_مقصد_ORIGINAL:
		واپس "original";
	حالت RESET_WHEN_ملا_مقصد_ملا:
		واپس "found";
	حالت RESET_WHEN_ملا_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", منوال);
}

ساکن صحیح read_reset_when_ملا(شمار reset_when_ملا_منوال *منوال)
{
	ساخت strbuf قدر = STRBUF_INIT;
	صحیح res = 0;

	*منوال = RESET_WHEN_ملا_NONE;
	اگر (is_خالی_خانے_or_missing_file(git_راستہ_bisect_reset_when_ملا()))
		واپس 0;

	اگر (strbuf_read_file(&قدر, git_راستہ_bisect_reset_when_ملا(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_راستہ_bisect_reset_when_ملا());
		جاؤ برآمد;
	}
	strbuf_trim(&قدر);
	اگر (parse_reset_when_ملا(قدر.buf, منوال))
		res = -1;

برآمد:
	strbuf_release(&قدر);
	واپس res;
}

ساکن صحیح bisect_reset_when_ملا(شمار reset_when_ملا_منوال منوال)
{
	ساخت bisect_terms terms = { 0 };
	حرف *commit = تہی;
	صحیح res;

	اگر (منوال == RESET_WHEN_ملا_مقصد_ملا) {
		read_bisect_terms(&terms.رکن_bad, &terms.رکن_good);
		commit = xstrfmt("refs/bisect/%s", terms.رکن_bad);
	} ورنہ اگر (منوال == RESET_WHEN_ملا_NONE) {
		BUG("automatic reset requested without a reset mode");
	}

	res = bisect_reset(commit, true);
	اگر (!res)
		res = bisect_clean_state();

	آزاد(commit);
	free_terms(&terms);
	واپس res;
}

ساکن خالی log_commit(FILE *fp,
		       ثابت حرف *fmt, ثابت حرف *state,
		       ساخت commit *commit)
{
	ساخت pretty_print_context pp = {0};
	ساخت strbuf commit_پیغام = STRBUF_INIT;
	حرف *label = xstrfmt(fmt, state);

	repo_format_commit_پیغام(the_repository, commit, "%s", &commit_پیغام,
				   &pp);

	fprintf(fp, "# %s: [%s] %s\n", label, oid_مقصد_hex(&commit->object.oid),
		commit_پیغام.buf);

	strbuf_release(&commit_پیغام);
	آزاد(label);
}

ساکن صحیح bisect_write(ثابت حرف *state, ثابت حرف *rev,
			ثابت ساخت bisect_terms *terms, صحیح nolog)
{
	ساخت strbuf لیبل = STRBUF_INIT;
	ساخت object_id oid;
	ساخت commit *commit;
	FILE *fp = تہی;
	صحیح res = 0;

	اگر (!strcmp(state, terms->رکن_bad)) {
		strbuf_addf(&لیبل, "refs/bisect/%s", state);
	} ورنہ اگر (one_کا(state, terms->رکن_good, "skip", تہی)) {
		strbuf_addf(&لیبل, "refs/bisect/%s-%s", state, rev);
	} ورنہ {
		res = error(_("Bad bisect_write argument: %s"), state);
		جاؤ اختتام;
	}

	اگر (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		جاؤ اختتام;
	}

	اگر (refs_update_ref(get_main_ref_store(the_repository), تہی, لیبل.buf, &oid, تہی, 0,
			    UPDATE_REFS_پیغام_ON_ERR)) {
		res = -1;
		جاؤ اختتام;
	}

	fp = fopen(git_راستہ_bisect_log(), "a");
	اگر (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_راستہ_bisect_log());
		جاؤ اختتام;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	اگر (!nolog)
		fprintf(fp, "git bisect %s %s\n", state, rev);

اختتام:
	اگر (fp)
		fclose(fp);
	strbuf_release(&لیبل);
	واپس res;
}

ساکن صحیح check_and_سیٹ_terms(ساخت bisect_terms *terms, ثابت حرف *cmd)
{
	صحیح has_رکن_file = !is_خالی_خانے_or_missing_file(git_راستہ_bisect_terms());

	اگر (one_کا(cmd, "skip", "start", "terms", تہی))
		واپس 0;

	اگر (has_رکن_file && strcmp(cmd, terms->رکن_bad) &&
	    strcmp(cmd, terms->رکن_good))
		واپس error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->رکن_bad,
				terms->رکن_good);

	اگر (!has_رکن_file) {
		اگر (one_کا(cmd, "bad", "good", تہی)) {
			سیٹ_terms(terms, "bad", "good");
			واپس write_terms(terms->رکن_bad, terms->رکن_good);
		}
		اگر (one_کا(cmd, "new", "old", تہی)) {
			سیٹ_terms(terms, "new", "old");
			واپس write_terms(terms->رکن_bad, terms->رکن_good);
		}
	}

	واپس 0;
}

ساکن صحیح inc_nr(ثابت ساخت reference *ref غیر_مستعمل, خالی *cb_ڈیٹا)
{
	غیرعلامتی صحیح *nr = (غیرعلامتی صحیح *)cb_ڈیٹا;
	(*nr)++;
	واپس 0;
}

ساکن ثابت حرف need_bad_and_good_revision_warning[] =
	N_("You need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

ساکن ثابت حرف need_bisect_آغاز_warning[] =
	N_("You need to start by \"git bisect start\".\n"
	   "You then need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

ساکن صحیح decide_اگلا(ثابت ساخت bisect_terms *terms,
		       ثابت حرف *current_رکن, صحیح missing_good,
		       صحیح missing_bad)
{
	اگر (!missing_good && !missing_bad)
		واپس 0;
	اگر (!current_رکن)
		واپس -1;

	اگر (missing_good && !missing_bad &&
	    !strcmp(current_رکن, terms->رکن_good)) {
		حرف *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->رکن_bad);
		اگر (!isatty(0))
			واپس 0;
		/*
		 * TRANSLATORS: Make sure to include [Y] and [n] in your
		 * translation. The program will only accept English input
		 * at this point.
		 */
		yesno = git_prompt(_("Are you sure [Y/n]? "), PROMPT_ECHO);
		اگر (starts_with(yesno, "N") || starts_with(yesno, "n"))
			واپس -1;
		واپس 0;
	}

	اگر (!is_خالی_خانے_or_missing_file(git_راستہ_bisect_آغاز()))
		واپس error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	ورنہ
		واپس error(_(need_bisect_آغاز_warning),
			     vocab_good, vocab_bad, vocab_good, vocab_bad);
}

ساکن خالی bisect_حالت(ساخت bisect_state *state,
			  ثابت ساخت bisect_terms *terms)
{
	حرف *bad_ref = xstrfmt("refs/bisect/%s", terms->رکن_bad);
	حرف *good_glob = xstrfmt("%s-*", terms->رکن_good);
	ساخت refs_for_each_ref_options opts = {
		.نمونہ = good_glob,
		.سابقہ = "refs/bisect/",
		.trim_سابقہ = لڑی_لمبائی("refs/bisect/"),
	};

	اگر (refs_ref_exists(get_main_ref_store(the_repository), bad_ref))
		state->nr_bad = 1;

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      inc_nr, &state->nr_good, &opts);

	آزاد(good_glob);
	آزاد(bad_ref);
}

__attribute__((format (لکھو, 1, 2)))
ساکن خالی bisect_log_printf(ثابت حرف *fmt, ...)
{
	ساخت strbuf buf = STRBUF_INIT;
	va_list ap;

	va_آغاز(ap, fmt);
	strbuf_vaddf(&buf, fmt, ap);
	va_انتہا(ap);

	لکھو("%s", buf.buf);
	append_مقصد_file(git_راستہ_bisect_log(), "# %s", buf.buf);

	strbuf_release(&buf);
}

ساکن خالی bisect_print_حالت(ثابت ساخت bisect_terms *terms)
{
	ساخت bisect_state state = { 0 };

	bisect_حالت(&state, terms);

	/* If we had both, we'd already be started, and shouldn't get here. */
	اگر (state.nr_good && state.nr_bad)
		واپس;

	اگر (!state.nr_good && !state.nr_bad)
		bisect_log_printf(_("status: waiting for both '%s' and '%s' commits\n"),
				  terms->رکن_good, terms->رکن_bad);
	ورنہ اگر (state.nr_good)
		bisect_log_printf(Q_("status: waiting for '%s' commit, %d '%s' commit known\n",
				     "status: waiting for '%s' commit, %d '%s' commits known\n",
				     state.nr_good),
				  terms->رکن_bad, state.nr_good, terms->رکن_good);
	ورنہ
		bisect_log_printf(_("status: waiting for '%s' commit(s), '%s' commit known\n"),
				  terms->رکن_good, terms->رکن_bad);
}

ساکن صحیح bisect_اگلا_check(ثابت ساخت bisect_terms *terms,
			     ثابت حرف *current_رکن)
{
	ساخت bisect_state state = { 0 };
	bisect_حالت(&state, terms);
	واپس decide_اگلا(terms, current_رکن, !state.nr_good, !state.nr_bad);
}

ساکن صحیح get_terms(ساخت bisect_terms *terms, صحیح file_missing_is_ok)
{
	ساخت strbuf str = STRBUF_INIT;
	FILE *fp = تہی;
	صحیح res = 0;

	fp = fopen(git_راستہ_bisect_terms(), "r");
	اگر (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		جاؤ اختتام;
	}

	free_terms(terms);
	اگر (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		جاؤ اختتام;
	}
	terms->رکن_bad = strbuf_detach(&str, تہی);
	اگر (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		FREE_AND_NULL(terms->رکن_bad);
		جاؤ اختتام;
	}
	terms->رکن_good = strbuf_detach(&str, تہی);

اختتام:
	اگر (fp)
		fclose(fp);
	strbuf_release(&str);
	واپس res;
}

ساکن صحیح bisect_terms(ساخت bisect_terms *terms, ثابت حرف *option)
{
	اگر (get_terms(terms, 0))
		واپس error(_("no terms defined"));

	اگر (!option) {
		لکھو(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->رکن_good, terms->رکن_bad);
		واپس 0;
	}
	اگر (one_کا(option, "--term-good", "--term-old", تہی))
		لکھو("%s\n", terms->رکن_good);
	ورنہ اگر (one_کا(option, "--term-bad", "--term-new", تہی))
		لکھو("%s\n", terms->رکن_bad);
	ورنہ
		واپس error(_("invalid argument %s for 'git bisect terms'.\n"
			       "Supported options are: "
			       "--term-good|--term-old and "
			       "--term-bad|--term-new."), option);

	واپس 0;
}

ساکن صحیح bisect_append_log_quoted(ثابت حرف **argv)
{
	صحیح res = 0;
	FILE *fp = fopen(git_راستہ_bisect_log(), "a");
	ساخت strbuf orig_دلائل = STRBUF_INIT;

	اگر (!fp)
		واپس -1;

	اگر (fprintf(fp, "git bisect start") < 1) {
		res = -1;
		جاؤ اختتام;
	}

	sq_quote_argv(&orig_دلائل, argv);
	اگر (fprintf(fp, "%s\n", orig_دلائل.buf) < 1)
		res = -1;

اختتام:
	fclose(fp);
	strbuf_release(&orig_دلائل);
	واپس res;
}

ساکن صحیح جوڑو_bisect_ref(ثابت ساخت reference *ref, خالی *cb)
{
	ساخت جوڑو_bisect_ref_ڈیٹا *ڈیٹا = cb;

	جوڑو_pending_oid(ڈیٹا->revs, ref->name, ref->oid, ڈیٹا->object_flags);

	واپس 0;
}

ساکن صحیح prepare_revs(ساخت bisect_terms *terms, ساخت rev_info *revs)
{
	ساخت refs_for_each_ref_options opts = {
		.سابقہ = "refs/bisect/",
		.trim_سابقہ = لڑی_لمبائی("refs/bisect/"),
	};
	صحیح res = 0;
	ساخت جوڑو_bisect_ref_ڈیٹا cb = { revs };
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
	repo_init_revisions(the_repository, revs, تہی);
	تیاری_revisions(0, تہی, revs, تہی);

	opts.نمونہ = bad;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      جوڑو_bisect_ref, &cb, &opts);

	cb.object_flags = UNINTERESTING;
	opts.نمونہ = good;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      جوڑو_bisect_ref, &cb, &opts);

	اگر (prepare_revision_walk(revs))
		res = error(_("revision walk setup failed"));

	آزاد(good);
	آزاد(bad);
	واپس res;
}

ساکن صحیح bisect_skipped_commits(ساخت bisect_terms *terms)
{
	صحیح res;
	FILE *fp = تہی;
	ساخت rev_info revs;
	ساخت commit *commit;
	ساخت pretty_print_context pp = {0};
	ساخت strbuf commit_name = STRBUF_INIT;

	res = prepare_revs(terms, &revs);
	اگر (res)
		واپس res;

	fp = fopen(git_راستہ_bisect_log(), "a");
	اگر (!fp)
		واپس error_errno(_("could not open '%s' for appending"),
				  git_راستہ_bisect_log());

	اگر (fprintf(fp, "# only skipped commits left to test\n") < 0)
		واپس error_errno(_("failed to write to '%s'"), git_راستہ_bisect_log());

	جب ((commit = get_revision(&revs)) != تہی) {
		strbuf_reset(&commit_name);
		repo_format_commit_پیغام(the_repository, commit, "%s",
					   &commit_name, &pp);
		fprintf(fp, "# possible first '%s' commit: [%s] %s\n",
			terms->رکن_bad, oid_مقصد_hex(&commit->object.oid),
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
	واپس 0;
}

ساکن صحیح bisect_successful(ساخت bisect_terms *terms)
{
	ساخت object_id oid;
	ساخت commit *commit;
	ساخت pretty_print_context pp = {0};
	ساخت strbuf commit_name = STRBUF_INIT;
	حرف *bad_ref = xstrfmt("refs/bisect/%s",terms->رکن_bad);
	صحیح res;

	refs_read_ref(get_main_ref_store(the_repository), bad_ref, &oid);
	commit = lookup_commit_reference_by_name(bad_ref);
	اگر (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		آزاد(bad_ref);
		واپس BISECT_FAILED;
	}
	repo_format_commit_پیغام(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_مقصد_file(git_راستہ_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->رکن_bad, oid_مقصد_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	آزاد(bad_ref);
	واپس res;
}

ساکن شمار bisect_error bisect_اگلا(ساخت bisect_terms *terms,
				     ثابت حرف *سابقہ)
{
	شمار bisect_error res;

	اگر (bisect_autostart(terms))
		واپس BISECT_FAILED;

	اگر (bisect_اگلا_check(terms, terms->رکن_good))
		واپس BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_اگلا_all(the_repository, سابقہ);

	اگر (res == BISECT_اندرونی_SUCCESS_1_ST_BAD_ملا) {
		res = bisect_successful(terms);
		واپس res ? res : BISECT_اندرونی_SUCCESS_1_ST_BAD_ملا;
	} ورنہ اگر (res == BISECT_ONLY_SKIPPED_بایاں) {
		res = bisect_skipped_commits(terms);
		واپس res ? res : BISECT_ONLY_SKIPPED_بایاں;
	}
	واپس res;
}

ساکن شمار bisect_error bisect_auto_اگلا(ساخت bisect_terms *terms,
					  ثابت حرف *سابقہ)
{
	اگر (bisect_اگلا_check(terms, تہی)) {
		bisect_print_حالت(terms);
		واپس BISECT_OK;
	}

	واپس bisect_اگلا(terms, سابقہ);
}

ساکن شمار bisect_error bisect_آغاز(ساخت bisect_terms *terms, صحیح argc,
				      ثابت حرف **argv)
{
	صحیح no_checkout = 0;
	صحیح پہلا_والد_only = 0;
	صحیح i, has_double_dash = 0, must_write_terms = 0, bad_نظر_آیا = 0;
	صحیح flags, pathspec_pos;
	شمار bisect_error res = BISECT_OK;
	ساخت string_list revs = STRING_LIST_INIT_DUP;
	ساخت string_list حالتیں = STRING_LIST_INIT_DUP;
	ساخت strbuf آغاز_سر = STRBUF_INIT;
	ساخت strbuf bisect_نام = STRBUF_INIT;
	ساخت object_id سر_oid;
	ساخت object_id oid;
	شمار reset_when_ملا_منوال reset_when_ملا = RESET_WHEN_ملا_NONE;
	ثابت حرف *سر;

	اگر (is_bare_repository(the_repository))
		no_checkout = 1;

	/*
	 * Check for one bad and then some good revisions
	 */
	برائے (i = 0; i < argc; i++) {
		اگر (!strcmp(argv[i], "--")) {
			has_double_dash = 1;
			توڑو;
		}
	}

	برائے (i = 0; i < argc; i++) {
		ثابت حرف *arg = argv[i];
		اگر (!strcmp(argv[i], "--")) {
			توڑو;
		} ورنہ اگر (!strcmp(arg, "--no-checkout")) {
			no_checkout = 1;
		} ورنہ اگر (!strcmp(arg, "--first-parent")) {
			پہلا_والد_only = 1;
		} ورنہ اگر (!strcmp(arg, "--reset-when-found")) {
			reset_when_ملا = RESET_WHEN_ملا_مقصد_ORIGINAL;
		} ورنہ اگر (skip_سابقہ(arg, "--reset-when-found=", &arg)) {
			اگر (parse_reset_when_ملا(arg, &reset_when_ملا)) {
				res = BISECT_FAILED;
				جاؤ اختتام;
			}
		} ورنہ اگر (!strcmp(arg, "--term-good") ||
			 !strcmp(arg, "--term-old")) {
			i++;
			اگر (argc <= i)
				واپس error(_("'' is not a valid term"));
			must_write_terms = 1;
			آزاد((خالی *) terms->رکن_good);
			terms->رکن_good = xstrdup(argv[i]);
		} ورنہ اگر (skip_سابقہ(arg, "--term-good=", &arg) ||
			   skip_سابقہ(arg, "--term-old=", &arg)) {
			must_write_terms = 1;
			آزاد((خالی *) terms->رکن_good);
			terms->رکن_good = xstrdup(arg);
		} ورنہ اگر (!strcmp(arg, "--term-bad") ||
			 !strcmp(arg, "--term-new")) {
			i++;
			اگر (argc <= i)
				واپس error(_("'' is not a valid term"));
			must_write_terms = 1;
			آزاد((خالی *) terms->رکن_bad);
			terms->رکن_bad = xstrdup(argv[i]);
		} ورنہ اگر (skip_سابقہ(arg, "--term-bad=", &arg) ||
			   skip_سابقہ(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			آزاد((خالی *) terms->رکن_bad);
			terms->رکن_bad = xstrdup(arg);
		} ورنہ اگر (starts_with(arg, "--")) {
			واپس error(_("unrecognized option: '%s'"), arg);
		} ورنہ اگر (!get_oidf(&oid, "%s^{commit}", arg)) {
			string_list_append(&revs, oid_مقصد_hex(&oid));
		} ورنہ اگر (has_double_dash) {
			die(_("'%s' does not appear to be a valid "
			      "revision"), arg);
		} ورنہ {
			توڑو;
		}
	}
	اگر (reset_when_ملا != RESET_WHEN_ملا_NONE && no_checkout) {
		res = error(_("options '%s' and '%s' cannot be used together"),
			    "--reset-when-found", "--no-checkout");
		جاؤ اختتام;
	}
	pathspec_pos = i;

	/*
	 * The user ran "git bisect start <sha1> <sha1>", hence did not
	 * explicitly specify the terms, but we are already starting to
	 * set references named with the default terms, and won't be able
	 * to change afterwards.
	 */
	اگر (revs.nr)
		must_write_terms = 1;
	برائے (i = 0; i < revs.nr; i++) {
		اگر (bad_نظر_آیا) {
			string_list_append(&حالتیں, terms->رکن_good);
		} ورنہ {
			bad_نظر_آیا = 1;
			string_list_append(&حالتیں, terms->رکن_bad);
		}
	}

	/*
	 * Verify HEAD
	 */
	سر = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
				       "HEAD", 0, &سر_oid, &flags);
	اگر (!سر) {
		اگر (repo_get_oid(the_repository, "HEAD", &سر_oid))
			واپس error(_("bad HEAD - I need a HEAD"));
		سر = "HEAD";
	}

	/*
	 * Check if we are bisecting
	 */
	اگر (!is_خالی_خانے_or_missing_file(git_راستہ_bisect_آغاز())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&آغاز_سر, git_راستہ_bisect_آغاز(), 0);
		strbuf_trim(&آغاز_سر);
		اگر (!no_checkout) {
			ساخت بچہ_process cmd = بچہ_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.دلائل, "checkout", آغاز_سر.buf,
				     "--", تہی);
			اگر (تسلسل_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       آغاز_سر.buf);
				جاؤ اختتام;
			}
		}
	} ورنہ {
		/* Get the rev from where we start. */
		اگر (!repo_get_oid(the_repository, سر, &سر_oid) &&
		    !starts_with(سر, "refs/heads/")) {
			strbuf_reset(&آغاز_سر);
			strbuf_جوڑو_oid_hex(&آغاز_سر, &سر_oid);
		} ورنہ اگر (!repo_get_oid(the_repository, سر, &سر_oid) &&
			   skip_سابقہ(سر, "refs/heads/", &سر)) {
			strbuf_addstr(&آغاز_سر, سر);
		} ورنہ {
			واپس error(_("bad HEAD - strange symbolic ref"));
		}
	}

	/*
	 * Get rid of any old bisect state.
	 */
	اگر (bisect_clean_state())
		واپس BISECT_FAILED;

	/*
	 * Write new start state
	 */
	write_file(git_راستہ_bisect_آغاز(), "%s\n", آغاز_سر.buf);

	اگر (پہلا_والد_only)
		write_file(git_راستہ_bisect_پہلا_والد(), "\n");

	اگر (reset_when_ملا != RESET_WHEN_ملا_NONE)
		write_file(git_راستہ_bisect_reset_when_ملا(), "%s\n",
			   reset_when_ملا_منوال_name(reset_when_ملا));

	اگر (no_checkout) {
		اگر (repo_get_oid(the_repository, آغاز_سر.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), آغاز_سر.buf);
			جاؤ اختتام;
		}
		اگر (refs_update_ref(get_main_ref_store(the_repository), تہی, "BISECT_HEAD", &oid, تہی, 0,
				    UPDATE_REFS_پیغام_ON_ERR)) {
			res = BISECT_FAILED;
			جاؤ اختتام;
		}
	}

	اگر (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_نام, argv + pathspec_pos);
	write_file(git_راستہ_bisect_نام(), "%s\n", bisect_نام.buf);

	برائے (i = 0; i < حالتیں.nr; i++)
		اگر (bisect_write(حالتیں.اشیاء[i].string,
				 revs.اشیاء[i].string, terms, 1)) {
			res = BISECT_FAILED;
			جاؤ اختتام;
		}

	اگر (must_write_terms && write_terms(terms->رکن_bad,
					    terms->رکن_good)) {
		res = BISECT_FAILED;
		جاؤ اختتام;
	}

	res = bisect_append_log_quoted(argv);
	اگر (res)
		res = BISECT_FAILED;

اختتام:
	string_list_clear(&revs, 0);
	string_list_clear(&حالتیں, 0);
	strbuf_release(&آغاز_سر);
	strbuf_release(&bisect_نام);
	اگر (res)
		واپس res;

	res = bisect_auto_اگلا(terms, تہی);
	اگر (!is_bisect_success(res))
		bisect_clean_state();
	واپس res;
}

ساکن اندرونی صحیح file_is_not_خالی_خانے(ثابت حرف *راستہ)
{
	واپس !is_خالی_خانے_or_missing_file(راستہ);
}

ساکن صحیح bisect_autostart(ساخت bisect_terms *terms)
{
	صحیح res;
	ثابت حرف *yesno;

	اگر (file_is_not_خالی_خانے(git_راستہ_bisect_آغاز()))
		واپس 0;

	fprintf_ln(stderr, _("You need to start by \"git bisect "
			  "start\"\n"));

	اگر (!isatty(STDIN_FILENO))
		واپس -1;

	/*
	 * TRANSLATORS: Make sure to include [Y] and [n] in your
	 * translation. The program will only accept English input
	 * at this point.
	 */
	yesno = git_prompt(_("Do you want me to do it for you "
			     "[Y/n]? "), PROMPT_ECHO);
	res = tolower(*yesno) == 'n' ?
		-1 : bisect_آغاز(terms, 0, خالی_خانے_strvec);

	واپس res;
}

ساکن شمار bisect_error bisect_state(ساخت bisect_terms *terms, صحیح argc,
				      ثابت حرف **argv)
{
	ثابت حرف *state;
	صحیح i, verify_expected = 1;
	ساخت object_id oid, expected;
	ساخت oid_array revs = OID_ARRAY_INIT;

	اگر (!argc)
		واپس error(_("Please call `--bisect-state` with at least one argument"));

	اگر (bisect_autostart(terms))
		واپس BISECT_FAILED;

	state = argv[0];
	اگر (check_and_سیٹ_terms(terms, state) ||
	    !one_کا(state, terms->رکن_good, terms->رکن_bad, "skip", تہی))
		واپس BISECT_FAILED;

	argv++;
	argc--;
	اگر (argc > 1 && !strcmp(state, terms->رکن_bad))
		واپس error(_("'git bisect %s' can take only one argument."), terms->رکن_bad);

	اگر (argc == 0) {
		ثابت حرف *سر = "BISECT_HEAD";
		شمار get_oid_نتیجہ res_سر = repo_get_oid(the_repository,
							    سر, &oid);

		اگر (res_سر == MISSING_OBJECT) {
			سر = "HEAD";
			res_سر = repo_get_oid(the_repository, سر, &oid);
		}

		اگر (res_سر)
			error(_("Bad rev input: %s"), سر);
		oid_array_append(&revs, &oid);
	}

	/*
	 * All input revs must be checked before executing bisect_write()
	 * to discard junk revs.
	 */

	برائے (; argc; argc--, argv++) {
		ساخت commit *commit;

		اگر (repo_get_oid(the_repository, *argv, &oid)){
			error(_("Bad rev input: %s"), *argv);
			oid_array_clear(&revs);
			واپس BISECT_FAILED;
		}

		commit = lookup_commit_reference(the_repository, &oid);
		اگر (!commit)
			die(_("Bad rev input (not a commit): %s"), *argv);

		oid_array_append(&revs, &commit->object.oid);
	}

	اگر (refs_read_ref(get_main_ref_store(the_repository), "BISECT_EXPECTED_REV", &expected))
		verify_expected = 0; /* Ignore invalid file contents */

	برائے (i = 0; i < revs.nr; i++) {
		اگر (bisect_write(state, oid_مقصد_hex(&revs.oid[i]), terms, 0)) {
			oid_array_clear(&revs);
			واپس BISECT_FAILED;
		}
		اگر (verify_expected && !oideq(&revs.oid[i], &expected)) {
			unlink_or_warn(git_راستہ_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					تہی, "BISECT_EXPECTED_REV", تہی,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	واپس bisect_auto_اگلا(terms, تہی);
}

ساکن شمار bisect_error bisect_log(خالی)
{
	صحیح fd, حالت_2;
	ثابت حرف* filename = git_راستہ_bisect_log();

	اگر (is_خالی_خانے_or_missing_file(filename))
		واپس error(_("We are not bisecting."));

	fd = open(filename, O_RDONLY);
	اگر (fd < 0)
		واپس BISECT_FAILED;

	حالت_2 = copy_fd(fd, STDOUT_FILENO);
	close(fd);
	واپس حالت_2 ? BISECT_FAILED : BISECT_OK;
}

ساکن صحیح process_replay_سطر(ساخت bisect_terms *terms, ساخت strbuf *سطر)
{
	ثابت حرف *p = سطر->buf + strspn(سطر->buf, " \t");
	حرف *لفظ_انتہا, *rev;

	اگر ((!skip_سابقہ(p, "git bisect", &p) &&
	!skip_سابقہ(p, "git-bisect", &p)) || !خلا_ہے(*p))
		واپس 0;
	p += strspn(p, " \t");

	لفظ_انتہا = (حرف *)p + strcspn(p, " \t");
	rev = لفظ_انتہا + strspn(لفظ_انتہا, " \t");
	*لفظ_انتہا = '\0'; /* NUL-terminate the word */

	اگر (get_terms(terms, 1))
		واپس error(_("no terms defined"));
	اگر (check_and_سیٹ_terms(terms, p))
		واپس -1;

	اگر (!strcmp(p, "start")) {
		ساخت strvec argv = STRVEC_INIT;
		صحیح res;
		sq_dequote_مقصد_strvec(rev, &argv);
		res = bisect_آغاز(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		واپس res;
	}

	اگر (one_کا(p, terms->رکن_good,
	   terms->رکن_bad, "skip", تہی))
		واپس bisect_write(p, rev, terms, 0);

	اگر (!strcmp(p, "terms")) {
		ساخت strvec argv = STRVEC_INIT;
		صحیح res;
		sq_dequote_مقصد_strvec(rev, &argv);
		res = bisect_terms(terms, argv.nr == 1 ? argv.v[0] : تہی);
		strvec_clear(&argv);
		واپس res;
	}
	error(_("'%s'?? what are you talking about?"), p);

	واپس -1;
}

ساکن شمار bisect_error bisect_replay(ساخت bisect_terms *terms, ثابت حرف *filename)
{
	FILE *fp = تہی;
	شمار bisect_error res = BISECT_OK;
	ساخت strbuf سطر = STRBUF_INIT;

	اگر (is_خالی_خانے_or_missing_file(filename))
		واپس error(_("cannot read file '%s' for replaying"), filename);

	اگر (bisect_clean_state())
		واپس BISECT_FAILED;

	fp = fopen(filename, "r");
	اگر (!fp)
		واپس BISECT_FAILED;

	جب ((strbuf_getline(&سطر, fp) != EOF) && !res)
		res = process_replay_سطر(terms, &سطر);

	strbuf_release(&سطر);
	fclose(fp);

	اگر (res)
		واپس BISECT_FAILED;

	واپس bisect_auto_اگلا(terms, تہی);
}

ساکن شمار bisect_error bisect_skip(ساخت bisect_terms *terms, صحیح argc,
				     ثابت حرف **argv)
{
	صحیح i;
	شمار bisect_error res;
	ساخت strvec argv_state = STRVEC_INIT;

	strvec_ڈالو(&argv_state, "skip");

	برائے (i = 0; i < argc; i++) {
		ثابت حرف *dotdot = لڑی_تلاش(argv[i], "..");

		اگر (dotdot) {
			ساخت rev_info revs;
			ساخت commit *commit;

			repo_init_revisions(the_repository, &revs, تہی);
			تیاری_revisions(2, argv + i - 1, &revs, تہی);

			اگر (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			جب ((commit = get_revision(&revs)) != تہی)
				strvec_ڈالو(&argv_state,
						oid_مقصد_hex(&commit->object.oid));

			reset_revision_walk();
			release_revisions(&revs);
		} ورنہ {
			strvec_ڈالو(&argv_state, argv[i]);
		}
	}
	res = bisect_state(terms, argv_state.nr, argv_state.v);

	strvec_clear(&argv_state);
	واپس res;
}

ساکن صحیح bisect_visualize(ساخت bisect_terms *terms, صحیح argc,
			    ثابت حرف **argv)
{
	ساخت بچہ_process cmd = بچہ_PROCESS_INIT;
	ساخت strbuf sb = STRBUF_INIT;

	اگر (bisect_اگلا_check(terms, تہی) != 0)
		واپس BISECT_FAILED;

	cmd.no_stdin = 1;
	اگر (!argc) {
		اگر ((getenv("DISPLAY") || getenv("SESSIONNAME") || getenv("MSYSTEM") ||
		     getenv("SECURITYSESSIONID")) && exists_اندر_راستہ("gitk")) {
			strvec_ڈالو(&cmd.دلائل, "gitk");
		} ورنہ {
			strvec_ڈالو(&cmd.دلائل, "log");
			cmd.git_cmd = 1;
		}
	} ورنہ {
		اگر (argv[0][0] == '-') {
			strvec_ڈالو(&cmd.دلائل, "log");
			cmd.git_cmd = 1;
		} ورنہ اگر (strcmp(argv[0], "tig") && !starts_with(argv[0], "git"))
			cmd.git_cmd = 1;

		strvec_pushv(&cmd.دلائل, argv);
	}

	strvec_pushl(&cmd.دلائل, "--bisect", "--", تہی);

	strbuf_read_file(&sb, git_راستہ_bisect_نام(), 0);
	sq_dequote_مقصد_strvec(sb.buf, &cmd.دلائل);
	strbuf_release(&sb);

	واپس تسلسل_command(&cmd);
}

ساکن صحیح get_پہلا_good(ثابت ساخت reference *ref, خالی *cb_ڈیٹا)
{
	oidcpy(cb_ڈیٹا, ref->oid);
	واپس 1;
}

ساکن صحیح do_bisect_تسلسل(ثابت حرف *command)
{
	ساخت بچہ_process cmd = بچہ_PROCESS_INIT;

	لکھو(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_ڈالو(&cmd.دلائل, command);
	واپس تسلسل_command(&cmd);
}

ساکن صحیح verify_good(ثابت ساخت bisect_terms *terms, ثابت حرف *command)
{
	صحیح rc;
	شمار bisect_error res;
	ساخت object_id good_rev;
	ساخت object_id current_rev;
	حرف *good_glob = xstrfmt("%s-*", terms->رکن_good);
	صحیح no_checkout = refs_ref_exists(get_main_ref_store(the_repository),
					  "BISECT_HEAD");
	ساخت refs_for_each_ref_options opts = {
		.نمونہ = good_glob,
		.سابقہ = "refs/bisect/",
		.trim_سابقہ = لڑی_لمبائی("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_پہلا_good, &good_rev, &opts);
	آزاد(good_glob);

	اگر (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		واپس -1;

	res = bisect_checkout(&good_rev, no_checkout);
	اگر (res != BISECT_OK)
		واپس -1;

	rc = do_bisect_تسلسل(command);

	res = bisect_checkout(&current_rev, no_checkout);
	اگر (res != BISECT_OK)
		واپس -1;

	واپس rc;
}

ساکن صحیح bisect_تسلسل(ساخت bisect_terms *terms, صحیح argc, ثابت حرف **argv)
{
	صحیح res = BISECT_OK;
	ساخت strbuf command = STRBUF_INIT;
	ثابت حرف *reset_when_ملا_arg;
	ثابت حرف *new_state;
	صحیح temporary_stdout_fd, saved_stdout;
	صحیح is_پہلا_تسلسل = 1;
	شمار reset_when_ملا_منوال reset_when_ملا = RESET_WHEN_ملا_NONE;

	اگر (bisect_اگلا_check(terms, تہی))
		واپس BISECT_FAILED;

	اگر (argc && !strcmp(argv[0], "--reset-when-found")) {
		reset_when_ملا = RESET_WHEN_ملا_مقصد_ORIGINAL;
	} ورنہ اگر (argc && skip_سابقہ(argv[0], "--reset-when-found=",
				    &reset_when_ملا_arg)) {
		اگر (parse_reset_when_ملا(reset_when_ملا_arg,
					   &reset_when_ملا))
			واپس BISECT_FAILED;
	}

	اگر (reset_when_ملا != RESET_WHEN_ملا_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		واپس error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	اگر (reset_when_ملا != RESET_WHEN_ملا_NONE) {
		write_file(git_راستہ_bisect_reset_when_ملا(), "%s\n",
			   reset_when_ملا_منوال_name(reset_when_ملا));
		argc--;
		argv++;
	}

	اگر (!argc) {
		error(_("bisect run failed: no command provided."));
		واپس BISECT_FAILED;
	}

	sq_quote_argv(&command, argv);
	strbuf_ltrim(&command);
	جب (1) {
		res = do_bisect_تسلسل(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		اگر (is_پہلا_تسلسل && (res == 126 || res == 127)) {
			صحیح rc = verify_good(terms, command.buf);
			is_پہلا_تسلسل = 0;
			اگر (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->رکن_good);
				res = BISECT_FAILED;
				توڑو;
			}
			اگر (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->رکن_good);
				res = BISECT_FAILED;
				توڑو;
			}
		}

		اگر (res < 0 || 128 <= res) {
			error(_("bisect run failed: exit code %d from"
				" %s is < 0 or >= 128"), res, command.buf);
			توڑو;
		}

		اگر (res == 125)
			new_state = "skip";
		ورنہ اگر (!res)
			new_state = terms->رکن_good;
		ورنہ
			new_state = terms->رکن_bad;

		temporary_stdout_fd = open(git_راستہ_bisect_تسلسل(), O_CREAT | O_WRONLY | O_TRUNC, 0666);

		اگر (temporary_stdout_fd < 0) {
			res = error_errno(_("cannot open file '%s' for writing"), git_راستہ_bisect_تسلسل());
			توڑو;
		}

		fflush(stdout);
		saved_stdout = dup(1);
		اگر (saved_stdout < 0 ||
		    dup2(temporary_stdout_fd, 1) < 0) {
			res = error_errno(_("could not duplicate stdout"));
			اگر (saved_stdout >= 0)
				close(saved_stdout);
			close(temporary_stdout_fd);
			توڑو;
		}

		res = bisect_state(terms, 1, &new_state);

		fflush(stdout);
		dup2(saved_stdout, 1);
		close(saved_stdout);
		close(temporary_stdout_fd);

		print_file_مقصد_stdout(git_راستہ_bisect_تسلسل());

		اگر (res == BISECT_ONLY_SKIPPED_بایاں)
			error(_("bisect run cannot continue any more"));
		ورنہ اگر (res == BISECT_اندرونی_SUCCESS_ملاؤ_بنیاد) {
			puts(_("bisect run success"));
			res = BISECT_OK;
		} ورنہ اگر (res == BISECT_اندرونی_SUCCESS_1_ST_BAD_ملا) {
			لکھو(_("bisect found first '%s' commit\n"), terms->رکن_bad);
		} ورنہ اگر (res) {
			error(_("bisect run failed: 'git bisect %s'"
				" exited with error code %d"), new_state, res);
		} ورنہ {
			جاری;
		}
		توڑو;
	}

	strbuf_release(&command);
	واپس res;
}

ساکن صحیح cmd_bisect__reset(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ غیر_مستعمل,
			     ساخت repository *repo غیر_مستعمل)
{
	صحیح res;

	اگر (argc > 1)
		واپس error(_("'%s' requires either no argument or a commit"),
			     "git bisect reset");
	res = bisect_reset(argc ? argv[0] : تہی, false);
	اگر (res)
		واپس res;
	واپس bisect_clean_state();
}

ساکن صحیح cmd_bisect__terms(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ غیر_مستعمل,
			     ساخت repository *repo غیر_مستعمل)
{
	صحیح res;
	ساخت bisect_terms terms = { 0 };

	اگر (argc > 1)
		واپس error(_("'%s' requires 0 or 1 argument"),
			     "git bisect terms");
	res = bisect_terms(&terms, argc == 1 ? argv[0] : تہی);
	free_terms(&terms);
	واپس res;
}

ساکن صحیح cmd_bisect_آغاز(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ غیر_مستعمل,
			     ساخت repository *repo غیر_مستعمل)
{
	صحیح res;
	ساخت bisect_terms terms = { 0 };

	سیٹ_terms(&terms, "bad", "good");
	res = bisect_آغاز(&terms, argc, argv);
	free_terms(&terms);
	واپس res;
}

ساکن صحیح cmd_bisect_اگلا(صحیح argc, ثابت حرف **argv غیر_مستعمل, ثابت حرف *سابقہ,
			    ساخت repository *repo غیر_مستعمل)
{
	صحیح res;
	ساخت bisect_terms terms = { 0 };

	اگر (argc)
		واپس error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	اگر (get_terms(&terms, 1))
		واپس error(_("no terms defined"));
	res = bisect_اگلا(&terms, سابقہ);
	free_terms(&terms);
	واپس res;
}

ساکن صحیح cmd_bisect__log(صحیح argc غیر_مستعمل, ثابت حرف **argv غیر_مستعمل,
			   ثابت حرف *سابقہ غیر_مستعمل,
			   ساخت repository *repo غیر_مستعمل)
{
	واپس bisect_log();
}

ساکن صحیح cmd_bisect__replay(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ غیر_مستعمل,
			      ساخت repository *repo غیر_مستعمل)
{
	صحیح res;
	ساخت bisect_terms terms = { 0 };

	اگر (argc != 1)
		واپس error(_("no logfile given"));
	سیٹ_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	واپس res;
}

ساکن صحیح cmd_bisect__skip(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ غیر_مستعمل,
			    ساخت repository *repo غیر_مستعمل)
{
	صحیح res;
	ساخت bisect_terms terms = { 0 };

	سیٹ_terms(&terms, "bad", "good");
	اگر (get_terms(&terms, 1))
		واپس error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	واپس res;
}

ساکن صحیح cmd_bisect__visualize(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ غیر_مستعمل,
				 ساخت repository *repo غیر_مستعمل)
{
	صحیح res;
	ساخت bisect_terms terms = { 0 };

	اگر (get_terms(&terms, 1))
		واپس error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	واپس res;
}

ساکن صحیح cmd_bisect_تسلسل(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ غیر_مستعمل,
			   ساخت repository *repo غیر_مستعمل)
{
	صحیح res;
	ساخت bisect_terms terms = { 0 };

	اگر (!argc)
		واپس error(_("'%s' failed: no command provided."), "git bisect run");
	اگر (get_terms(&terms, 1))
		واپس error(_("no terms defined"));
	res = bisect_تسلسل(&terms, argc, argv);
	free_terms(&terms);
	واپس res;
}

صحیح cmd_bisect(صحیح argc,
	       ثابت حرف **argv,
	       ثابت حرف *سابقہ,
	       ساخت repository *repo)
{
	صحیح res = 0;
	parse_opt_subcommand_fn *fn = تہی;
	ساخت option options[] = {
		OPT_SUBCOMMAND("reset", &fn, cmd_bisect__reset),
		OPT_SUBCOMMAND("terms", &fn, cmd_bisect__terms),
		OPT_SUBCOMMAND("start", &fn, cmd_bisect_آغاز),
		OPT_SUBCOMMAND("next", &fn, cmd_bisect_اگلا),
		OPT_SUBCOMMAND("log", &fn, cmd_bisect__log),
		OPT_SUBCOMMAND("replay", &fn, cmd_bisect__replay),
		OPT_SUBCOMMAND("skip", &fn, cmd_bisect__skip),
		OPT_SUBCOMMAND("visualize", &fn, cmd_bisect__visualize),
		OPT_SUBCOMMAND("view", &fn, cmd_bisect__visualize),
		OPT_SUBCOMMAND("run", &fn, cmd_bisect_تسلسل),
		OPT_انتہا()
	};
	argc = parse_options(argc, argv, سابقہ, options, git_bisect_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);

	اگر (!fn) {
		ساخت bisect_terms terms = { 0 };

		اگر (!argc)
			usage_پیغام_opt(_("need a command"), git_bisect_usage, options);

		اگر (!strcmp(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		سیٹ_terms(&terms, "bad", "good");
		اگر (get_terms(&terms, 1))
			واپس error(_("no terms defined"));
		اگر (check_and_سیٹ_terms(&terms, argv[0]) ||
		    !one_کا(argv[0], terms.رکن_good, terms.رکن_bad, تہی))
			usage_پیغام_optf(_("unknown command: '%s'"), git_bisect_usage,
				       options, argv[0]);
		ورنہ
			res = bisect_state(&terms, argc, argv);
		free_terms(&terms);
	} ورنہ {
		argc--;
		argv++;
		res = fn(argc, argv, سابقہ, repo);
	}

	اگر (res == BISECT_اندرونی_SUCCESS_1_ST_BAD_ملا) {
		شمار reset_when_ملا_منوال منوال;

		اگر (read_reset_when_ملا(&منوال))
			res = BISECT_FAILED;
		ورنہ اگر (منوال != RESET_WHEN_ملا_NONE &&
			 bisect_reset_when_ملا(منوال))
			res = BISECT_FAILED;
	}

	واپس is_bisect_success(res) ? 0 : -res;
}
