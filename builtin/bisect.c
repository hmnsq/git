#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "copy.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "bisect.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "strvec.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "oid-array.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "prompt.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"

ସ୍ଥିର GIT_PATH_FUNC(git_path_bisect_terms, "BISECT_TERMS")
ସ୍ଥିର GIT_PATH_FUNC(git_path_bisect_ancestors_ok, "BISECT_ANCESTORS_OK")
ସ୍ଥିର GIT_PATH_FUNC(git_path_bisect_start, "BISECT_START")
ସ୍ଥିର GIT_PATH_FUNC(git_path_bisect_log, "BISECT_LOG")
ସ୍ଥିର GIT_PATH_FUNC(git_path_bisect_names, "BISECT_NAMES")
ସ୍ଥିର GIT_PATH_FUNC(git_path_bisect_first_parent, "BISECT_FIRST_PARENT")
ସ୍ଥିର GIT_PATH_FUNC(git_path_bisect_reset_when_found, "BISECT_RESET_WHEN_FOUND")
ସ୍ଥିର GIT_PATH_FUNC(git_path_bisect_run, "BISECT_RUN")

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

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_bisect_usage[] = {
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
	ଖାଲି
};

ଗଣନା reset_when_found_mode {
	RESET_WHEN_FOUND_NONE,
	RESET_WHEN_FOUND_TO_ORIGINAL,
	RESET_WHEN_FOUND_TO_FOUND,
};

ସଂରଚନା add_bisect_ref_data {
	ସଂରଚନା rev_info *revs;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା object_flags;
};

ସଂରଚନା bisect_terms {
	ଅକ୍ଷର *term_good;
	ଅକ୍ଷର *term_bad;
};

ସ୍ଥିର ଶୂନ୍ୟ free_terms(ସଂରଚନା bisect_terms *terms)
{
	FREE_AND_NULL(terms->term_good);
	FREE_AND_NULL(terms->term_bad);
}

ସ୍ଥିର ଶୂନ୍ୟ set_terms(ସଂରଚନା bisect_terms *terms, ଅଚଳ ଅକ୍ଷର *bad,
		      ଅଚଳ ଅକ୍ଷର *good)
{
	ମୁକ୍ତ((ଶୂନ୍ୟ *)terms->term_good);
	terms->term_good = xstrdup(good);
	ମୁକ୍ତ((ଶୂନ୍ୟ *)terms->term_bad);
	terms->term_bad = xstrdup(bad);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର vocab_bad[] = "bad|new";
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର vocab_good[] = "good|old";

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା bisect_autostart(ସଂରଚନା bisect_terms *terms);

/*
 * Check whether the string `term` belongs to the set of strings
 * included in the variable arguments.
 */
LAST_ARG_MUST_BE_NULL
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା one_of(ଅଚଳ ଅକ୍ଷର *term, ...)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = 0;
	va_list matches;
	ଅଚଳ ଅକ୍ଷର *match;

	va_start(matches, term);
	ଯେତେବେଳେ (!res && (match = va_arg(matches, ଅଚଳ ଅକ୍ଷର *)))
		res = !strcmp(term, match);
	va_end(matches);

	ଫେରନ୍ତୁ res;
}

/*
 * return code BISECT_INTERNAL_SUCCESS_MERGE_BASE
 * and BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND are codes
 * that indicate special success.
 */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_bisect_success(ଗଣନା bisect_error res)
{
	ଫେରନ୍ତୁ !res ||
		res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND ||
		res == BISECT_INTERNAL_SUCCESS_MERGE_BASE;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା write_in_file(ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *mode, ଅଚଳ ଅକ୍ଷର *format, va_list args)
{
	FILE *fp = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = 0;

	ଯଦି (strcmp(mode, "w") && strcmp(mode, "a"))
		BUG("write-in-file does not support '%s' mode", mode);
	fp = fopen(path, mode);
	ଯଦି (!fp)
		ଫେରନ୍ତୁ error_errno(_("cannot open file '%s' in mode '%s'"), path, mode);
	res = vfprintf(fp, format, args);

	ଯଦି (res < 0) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
		fclose(fp);
		errno = saved_errno;
		ଫେରନ୍ତୁ error_errno(_("could not write to file '%s'"), path);
	}

	ଫେରନ୍ତୁ fclose(fp);
}

__attribute__((format (printf, 2, 3)))
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା write_to_file(ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *format, ...)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "w", format, args);
	va_end(args);

	ଫେରନ୍ତୁ res;
}

__attribute__((format (printf, 2, 3)))
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା append_to_file(ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *format, ...)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;
	va_list args;

	va_start(args, format);
	res = write_in_file(path, "a", format, args);
	va_end(args);

	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା print_file_to_stdout(ଅଚଳ ଅକ୍ଷର *path)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = open(path, O_RDONLY);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	ଯଦି (fd < 0)
		ଫେରନ୍ତୁ error_errno(_("cannot open file '%s' for reading"), path);
	ଯଦି (copy_fd(fd, 1) < 0)
		ret = error_errno(_("failed to read '%s'"), path);
	close(fd);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_term_format(ଅଚଳ ଅକ୍ଷର *term, ଅଚଳ ଅକ୍ଷର *orig_term)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;
	ଅକ୍ଷର *new_term = xstrfmt("refs/bisect/%s", term);

	res = check_refname_format(new_term, 0);
	ମୁକ୍ତ(new_term);

	ଯଦି (res)
		ଫେରନ୍ତୁ error(_("'%s' is not a valid term"), term);

	ଯଦି (one_of(term, "help", "start", "skip", "next", "reset",
			"visualize", "view", "replay", "log", "run", "terms", ଖାଲି))
		ଫେରନ୍ତୁ error(_("can't use the builtin command '%s' as a term"), term);

	/*
	 * In theory, nothing prevents swapping completely good and bad,
	 * but this situation could be confusing and hasn't been tested
	 * enough. Forbid it for now.
	 */

	ଯଦି ((strcmp(orig_term, "bad") && one_of(term, "bad", "new", ଖାଲି)) ||
		 (strcmp(orig_term, "good") && one_of(term, "good", "old", ଖାଲି)))
		ଫେରନ୍ତୁ error(_("can't change the meaning of the term '%s'"), term);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା write_terms(ଅଚଳ ଅକ୍ଷର *bad, ଅଚଳ ଅକ୍ଷର *good)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;

	ଯଦି (!strcmp(bad, good))
		ଫେରନ୍ତୁ error(_("please use two different terms"));

	ଯଦି (check_term_format(bad, "bad") || check_term_format(good, "good"))
		ଫେରନ୍ତୁ -1;

	res = write_to_file(git_path_bisect_terms(), "%s\n%s\n", bad, good);

	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା bisect_reset(ଅଚଳ ଅକ୍ଷର *commit, bool quiet)
{
	ସଂରଚନା strbuf branch = STRBUF_INIT;

	ଯଦି (!commit) {
		ଯଦି (!strbuf_read_file(&branch, git_path_bisect_start(), 0))
			printf(_("We are not bisecting.\n"));
		ଅନ୍ୟଥା
			strbuf_rtrim(&branch);
	} ଅନ୍ୟଥା {
		ସଂରଚନା object_id oid;

		ଯଦି (repo_get_oid_commit(the_repository, commit, &oid))
			ଫେରନ୍ତୁ error(_("'%s' is not a valid commit"), commit);
		strbuf_addstr(&branch, commit);
	}

	ଯଦି (branch.len && !refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD")) {
		ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;

		cmd.git_cmd = 1;
		strvec_pushl(&cmd.args, "checkout", "--ignore-other-worktrees", ଖାଲି);
		ଯଦି (quiet)
			strvec_ଠେଲ(&cmd.args, "--quiet");
		strvec_pushl(&cmd.args, branch.buf, "--", ଖାଲି);
		ଯଦି (run_command(&cmd)) {
			error(_("could not check out original"
				" HEAD '%s'. Try 'git bisect"
				" reset <commit>'."), branch.buf);
			strbuf_release(&branch);
			ଫେରନ୍ତୁ -1;
		}
	}

	strbuf_release(&branch);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_reset_when_found(ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
				  ଗଣନା reset_when_found_mode *mode)
{
	ଯଦି (!strcmp(ମୂଲ୍ୟ, "original"))
		*mode = RESET_WHEN_FOUND_TO_ORIGINAL;
	ଅନ୍ୟଥା ଯଦି (!strcmp(ମୂଲ୍ୟ, "found"))
		*mode = RESET_WHEN_FOUND_TO_FOUND;
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ error(_("invalid value for '--reset-when-found': '%s'"),
			     ମୂଲ୍ୟ);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *reset_when_found_mode_name(ଗଣନା reset_when_found_mode mode)
{
	ବାଛ (mode) {
	ସ୍ଥିତି RESET_WHEN_FOUND_TO_ORIGINAL:
		ଫେରନ୍ତୁ "original";
	ସ୍ଥିତି RESET_WHEN_FOUND_TO_FOUND:
		ଫେରନ୍ତୁ "found";
	ସ୍ଥିତି RESET_WHEN_FOUND_NONE:
		BUG("no name for unset reset-when-found mode");
	}
	BUG("unknown reset-when-found mode %d", mode);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା read_reset_when_found(ଗଣନା reset_when_found_mode *mode)
{
	ସଂରଚନା strbuf ମୂଲ୍ୟ = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = 0;

	*mode = RESET_WHEN_FOUND_NONE;
	ଯଦି (is_empty_or_missing_file(git_path_bisect_reset_when_found()))
		ଫେରନ୍ତୁ 0;

	ଯଦି (strbuf_read_file(&ମୂଲ୍ୟ, git_path_bisect_reset_when_found(), 0) < 0) {
		res = error_errno(_("could not read '%s'"),
				  git_path_bisect_reset_when_found());
		ଯାଅ out;
	}
	strbuf_trim(&ମୂଲ୍ୟ);
	ଯଦି (parse_reset_when_found(ମୂଲ୍ୟ.buf, mode))
		res = -1;

out:
	strbuf_release(&ମୂଲ୍ୟ);
	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା bisect_reset_when_found(ଗଣନା reset_when_found_mode mode)
{
	ସଂରଚନା bisect_terms terms = { 0 };
	ଅକ୍ଷର *commit = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;

	ଯଦି (mode == RESET_WHEN_FOUND_TO_FOUND) {
		read_bisect_terms(&terms.term_bad, &terms.term_good);
		commit = xstrfmt("refs/bisect/%s", terms.term_bad);
	} ଅନ୍ୟଥା ଯଦି (mode == RESET_WHEN_FOUND_NONE) {
		BUG("automatic reset requested without a reset mode");
	}

	res = bisect_reset(commit, true);
	ଯଦି (!res)
		res = bisect_clean_state();

	ମୁକ୍ତ(commit);
	free_terms(&terms);
	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ଶୂନ୍ୟ log_commit(FILE *fp,
		       ଅଚଳ ଅକ୍ଷର *fmt, ଅଚଳ ଅକ୍ଷର *state,
		       ସଂରଚନା commit *commit)
{
	ସଂରଚନା pretty_print_context pp = {0};
	ସଂରଚନା strbuf commit_ସନ୍ଦେଶ = STRBUF_INIT;
	ଅକ୍ଷର *label = xstrfmt(fmt, state);

	repo_format_commit_message(the_repository, commit, "%s", &commit_ସନ୍ଦେଶ,
				   &pp);

	fprintf(fp, "# %s: [%s] %s\n", label, oid_to_hex(&commit->object.oid),
		commit_ସନ୍ଦେଶ.buf);

	strbuf_release(&commit_ସନ୍ଦେଶ);
	ମୁକ୍ତ(label);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା bisect_write(ଅଚଳ ଅକ୍ଷର *state, ଅଚଳ ଅକ୍ଷର *rev,
			ଅଚଳ ସଂରଚନା bisect_terms *terms, ପୂର୍ଣ୍ଣସଂଖ୍ୟା nolog)
{
	ସଂରଚନା strbuf tag = STRBUF_INIT;
	ସଂରଚନା object_id oid;
	ସଂରଚନା commit *commit;
	FILE *fp = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = 0;

	ଯଦି (!strcmp(state, terms->term_bad)) {
		strbuf_addf(&tag, "refs/bisect/%s", state);
	} ଅନ୍ୟଥା ଯଦି (one_of(state, terms->term_good, "skip", ଖାଲି)) {
		strbuf_addf(&tag, "refs/bisect/%s-%s", state, rev);
	} ଅନ୍ୟଥା {
		res = error(_("Bad bisect_write argument: %s"), state);
		ଯାଅ finish;
	}

	ଯଦି (repo_get_oid(the_repository, rev, &oid)) {
		res = error(_("couldn't get the oid of the rev '%s'"), rev);
		ଯାଅ finish;
	}

	ଯଦି (refs_update_ref(get_main_ref_store(the_repository), ଖାଲି, tag.buf, &oid, ଖାଲି, 0,
			    UPDATE_REFS_ସନ୍ଦେଶ_ON_ERR)) {
		res = -1;
		ଯାଅ finish;
	}

	fp = fopen(git_path_bisect_log(), "a");
	ଯଦି (!fp) {
		res = error_errno(_("couldn't open the file '%s'"), git_path_bisect_log());
		ଯାଅ finish;
	}

	commit = lookup_commit_reference(the_repository, &oid);
	log_commit(fp, "%s", state, commit);

	ଯଦି (!nolog)
		fprintf(fp, "git bisect %s %s\n", state, rev);

finish:
	ଯଦି (fp)
		fclose(fp);
	strbuf_release(&tag);
	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_and_set_terms(ସଂରଚନା bisect_terms *terms, ଅଚଳ ଅକ୍ଷର *cmd)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_term_file = !is_empty_or_missing_file(git_path_bisect_terms());

	ଯଦି (one_of(cmd, "skip", "start", "terms", ଖାଲି))
		ଫେରନ୍ତୁ 0;

	ଯଦି (has_term_file && strcmp(cmd, terms->term_bad) &&
	    strcmp(cmd, terms->term_good))
		ଫେରନ୍ତୁ error(_("Invalid command: you're currently in a "
				"%s/%s bisect"), terms->term_bad,
				terms->term_good);

	ଯଦି (!has_term_file) {
		ଯଦି (one_of(cmd, "bad", "good", ଖାଲି)) {
			set_terms(terms, "bad", "good");
			ଫେରନ୍ତୁ write_terms(terms->term_bad, terms->term_good);
		}
		ଯଦି (one_of(cmd, "new", "old", ଖାଲି)) {
			set_terms(terms, "new", "old");
			ଫେରନ୍ତୁ write_terms(terms->term_bad, terms->term_good);
		}
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା inc_nr(ଅଚଳ ସଂରଚନା reference *ref UNUSED, ଶୂନ୍ୟ *cb_data)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *nr = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *)cb_data;
	(*nr)++;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର need_bad_and_good_revision_warning[] =
	N_("You need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର need_bisect_start_warning[] =
	N_("You need to start by \"git bisect start\".\n"
	   "You then need to give me at least one %s and %s revision.\n"
	   "You can use \"git bisect %s\" and \"git bisect %s\" for that.");

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା decide_next(ଅଚଳ ସଂରଚନା bisect_terms *terms,
		       ଅଚଳ ଅକ୍ଷର *current_term, ପୂର୍ଣ୍ଣସଂଖ୍ୟା missing_good,
		       ପୂର୍ଣ୍ଣସଂଖ୍ୟା missing_bad)
{
	ଯଦି (!missing_good && !missing_bad)
		ଫେରନ୍ତୁ 0;
	ଯଦି (!current_term)
		ଫେରନ୍ତୁ -1;

	ଯଦି (missing_good && !missing_bad &&
	    !strcmp(current_term, terms->term_good)) {
		ଅକ୍ଷର *yesno;
		/*
		 * have bad (or new) but not good (or old). We could bisect
		 * although this is less optimum.
		 */
		warning(_("bisecting only with a %s commit"), terms->term_bad);
		ଯଦି (!isatty(0))
			ଫେରନ୍ତୁ 0;
		/*
		 * TRANSLATORS: Make sure to include [Y] and [n] in your
		 * translation. The program will only accept English input
		 * at this point.
		 */
		yesno = git_prompt(_("Are you sure [Y/n]? "), PROMPT_ECHO);
		ଯଦି (starts_with(yesno, "N") || starts_with(yesno, "n"))
			ଫେରନ୍ତୁ -1;
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!is_empty_or_missing_file(git_path_bisect_start()))
		ଫେରନ୍ତୁ error(_(need_bad_and_good_revision_warning),
			     vocab_bad, vocab_good, vocab_bad, vocab_good);
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ error(_(need_bisect_start_warning),
			     vocab_good, vocab_bad, vocab_good, vocab_bad);
}

ସ୍ଥିର ଶୂନ୍ୟ bisect_status(ସଂରଚନା bisect_state *state,
			  ଅଚଳ ସଂରଚନା bisect_terms *terms)
{
	ଅକ୍ଷର *bad_ref = xstrfmt("refs/bisect/%s", terms->term_bad);
	ଅକ୍ଷର *good_glob = xstrfmt("%s-*", terms->term_good);
	ସଂରଚନା refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	ଯଦି (refs_ref_exists(get_main_ref_store(the_repository), bad_ref))
		state->nr_bad = 1;

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      inc_nr, &state->nr_good, &opts);

	ମୁକ୍ତ(good_glob);
	ମୁକ୍ତ(bad_ref);
}

__attribute__((format (printf, 1, 2)))
ସ୍ଥିର ଶୂନ୍ୟ bisect_log_printf(ଅଚଳ ଅକ୍ଷର *fmt, ...)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	va_list ap;

	va_start(ap, fmt);
	strbuf_vaddf(&buf, fmt, ap);
	va_end(ap);

	printf("%s", buf.buf);
	append_to_file(git_path_bisect_log(), "# %s", buf.buf);

	strbuf_release(&buf);
}

ସ୍ଥିର ଶୂନ୍ୟ bisect_print_status(ଅଚଳ ସଂରଚନା bisect_terms *terms)
{
	ସଂରଚନା bisect_state state = { 0 };

	bisect_status(&state, terms);

	/* If we had both, we'd already be started, and shouldn't get here. */
	ଯଦି (state.nr_good && state.nr_bad)
		ଫେରନ୍ତୁ;

	ଯଦି (!state.nr_good && !state.nr_bad)
		bisect_log_printf(_("status: waiting for both '%s' and '%s' commits\n"),
				  terms->term_good, terms->term_bad);
	ଅନ୍ୟଥା ଯଦି (state.nr_good)
		bisect_log_printf(Q_("status: waiting for '%s' commit, %d '%s' commit known\n",
				     "status: waiting for '%s' commit, %d '%s' commits known\n",
				     state.nr_good),
				  terms->term_bad, state.nr_good, terms->term_good);
	ଅନ୍ୟଥା
		bisect_log_printf(_("status: waiting for '%s' commit(s), '%s' commit known\n"),
				  terms->term_good, terms->term_bad);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା bisect_next_check(ଅଚଳ ସଂରଚନା bisect_terms *terms,
			     ଅଚଳ ଅକ୍ଷର *current_term)
{
	ସଂରଚନା bisect_state state = { 0 };
	bisect_status(&state, terms);
	ଫେରନ୍ତୁ decide_next(terms, current_term, !state.nr_good, !state.nr_bad);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_terms(ସଂରଚନା bisect_terms *terms, ପୂର୍ଣ୍ଣସଂଖ୍ୟା file_missing_is_ok)
{
	ସଂରଚନା strbuf str = STRBUF_INIT;
	FILE *fp = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = 0;

	fp = fopen(git_path_bisect_terms(), "r");
	ଯଦି (!fp) {
		res = file_missing_is_ok ? 0 : -1;
		ଯାଅ finish;
	}

	free_terms(terms);
	ଯଦି (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		ଯାଅ finish;
	}
	terms->term_bad = strbuf_detach(&str, ଖାଲି);
	ଯଦି (strbuf_getline_lf(&str, fp) == EOF) {
		res = -1;
		FREE_AND_NULL(terms->term_bad);
		ଯାଅ finish;
	}
	terms->term_good = strbuf_detach(&str, ଖାଲି);

finish:
	ଯଦି (fp)
		fclose(fp);
	strbuf_release(&str);
	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା bisect_terms(ସଂରଚନା bisect_terms *terms, ଅଚଳ ଅକ୍ଷର *option)
{
	ଯଦି (get_terms(terms, 0))
		ଫେରନ୍ତୁ error(_("no terms defined"));

	ଯଦି (!option) {
		printf(_("Your current terms are '%s' for the old state\n"
			 "and '%s' for the new state.\n"),
		       terms->term_good, terms->term_bad);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (one_of(option, "--term-good", "--term-old", ଖାଲି))
		printf("%s\n", terms->term_good);
	ଅନ୍ୟଥା ଯଦି (one_of(option, "--term-bad", "--term-new", ଖାଲି))
		printf("%s\n", terms->term_bad);
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ error(_("invalid argument %s for 'git bisect terms'.\n"
			       "Supported options are: "
			       "--term-good|--term-old and "
			       "--term-bad|--term-new."), option);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା bisect_append_log_quoted(ଅଚଳ ଅକ୍ଷର **argv)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = 0;
	FILE *fp = fopen(git_path_bisect_log(), "a");
	ସଂରଚନା strbuf orig_args = STRBUF_INIT;

	ଯଦି (!fp)
		ଫେରନ୍ତୁ -1;

	ଯଦି (fprintf(fp, "git bisect start") < 1) {
		res = -1;
		ଯାଅ finish;
	}

	sq_quote_argv(&orig_args, argv);
	ଯଦି (fprintf(fp, "%s\n", orig_args.buf) < 1)
		res = -1;

finish:
	fclose(fp);
	strbuf_release(&orig_args);
	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_bisect_ref(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *cb)
{
	ସଂରଚନା add_bisect_ref_data *data = cb;

	add_pending_oid(data->revs, ref->name, ref->oid, data->object_flags);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା prepare_revs(ସଂରଚନା bisect_terms *terms, ସଂରଚନା rev_info *revs)
{
	ସଂରଚନା refs_for_each_ref_options opts = {
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = 0;
	ସଂରଚନା add_bisect_ref_data cb = { revs };
	ଅକ୍ଷର *good = xstrfmt("%s-*", terms->term_good);

	/*
	 * We cannot use terms->term_bad directly in
	 * for_each_glob_ref_in() and we have to append a '*' to it,
	 * otherwise for_each_glob_ref_in() will append '/' and '*'.
	 */
	ଅକ୍ଷର *bad = xstrfmt("%s*", terms->term_bad);

	/*
	 * It is important to reset the flags used by revision walks
	 * as the previous call to bisect_next_all() in turn
	 * sets up a revision walk.
	 */
	reset_revision_walk();
	repo_init_revisions(the_repository, revs, ଖାଲି);
	setup_revisions(0, ଖାଲି, revs, ଖାଲି);

	opts.pattern = bad;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      add_bisect_ref, &cb, &opts);

	cb.object_flags = UNINTERESTING;
	opts.pattern = good;
	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      add_bisect_ref, &cb, &opts);

	ଯଦି (prepare_revision_walk(revs))
		res = error(_("revision walk setup failed"));

	ମୁକ୍ତ(good);
	ମୁକ୍ତ(bad);
	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା bisect_skipped_commits(ସଂରଚନା bisect_terms *terms)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;
	FILE *fp = ଖାଲି;
	ସଂରଚନା rev_info revs;
	ସଂରଚନା commit *commit;
	ସଂରଚନା pretty_print_context pp = {0};
	ସଂରଚନା strbuf commit_name = STRBUF_INIT;

	res = prepare_revs(terms, &revs);
	ଯଦି (res)
		ଫେରନ୍ତୁ res;

	fp = fopen(git_path_bisect_log(), "a");
	ଯଦି (!fp)
		ଫେରନ୍ତୁ error_errno(_("could not open '%s' for appending"),
				  git_path_bisect_log());

	ଯଦି (fprintf(fp, "# only skipped commits left to test\n") < 0)
		ଫେରନ୍ତୁ error_errno(_("failed to write to '%s'"), git_path_bisect_log());

	ଯେତେବେଳେ ((commit = get_revision(&revs)) != ଖାଲି) {
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
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା bisect_successful(ସଂରଚନା bisect_terms *terms)
{
	ସଂରଚନା object_id oid;
	ସଂରଚନା commit *commit;
	ସଂରଚନା pretty_print_context pp = {0};
	ସଂରଚନା strbuf commit_name = STRBUF_INIT;
	ଅକ୍ଷର *bad_ref = xstrfmt("refs/bisect/%s",terms->term_bad);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;

	refs_read_ref(get_main_ref_store(the_repository), bad_ref, &oid);
	commit = lookup_commit_reference_by_name(bad_ref);
	ଯଦି (!commit) {
		error(_("could not find commit for '%s'"), bad_ref);
		ମୁକ୍ତ(bad_ref);
		ଫେରନ୍ତୁ BISECT_FAILED;
	}
	repo_format_commit_message(the_repository, commit, "%s", &commit_name,
				   &pp);

	res = append_to_file(git_path_bisect_log(), "# first '%s' commit: [%s] %s\n",
			    terms->term_bad, oid_to_hex(&commit->object.oid),
			    commit_name.buf);

	strbuf_release(&commit_name);
	ମୁକ୍ତ(bad_ref);
	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ଗଣନା bisect_error bisect_next(ସଂରଚନା bisect_terms *terms,
				     ଅଚଳ ଅକ୍ଷର *prefix)
{
	ଗଣନା bisect_error res;

	ଯଦି (bisect_autostart(terms))
		ଫେରନ୍ତୁ BISECT_FAILED;

	ଯଦି (bisect_next_check(terms, terms->term_good))
		ଫେରନ୍ତୁ BISECT_FAILED;

	/* Perform all bisection computation */
	res = bisect_next_all(the_repository, prefix);

	ଯଦି (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		res = bisect_successful(terms);
		ଫେରନ୍ତୁ res ? res : BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND;
	} ଅନ୍ୟଥା ଯଦି (res == BISECT_ONLY_SKIPPED_ବାମ) {
		res = bisect_skipped_commits(terms);
		ଫେରନ୍ତୁ res ? res : BISECT_ONLY_SKIPPED_ବାମ;
	}
	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ଗଣନା bisect_error bisect_auto_next(ସଂରଚନା bisect_terms *terms,
					  ଅଚଳ ଅକ୍ଷର *prefix)
{
	ଯଦି (bisect_next_check(terms, ଖାଲି)) {
		bisect_print_status(terms);
		ଫେରନ୍ତୁ BISECT_OK;
	}

	ଫେରନ୍ତୁ bisect_next(terms, prefix);
}

ସ୍ଥିର ଗଣନା bisect_error bisect_start(ସଂରଚନା bisect_terms *terms, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
				      ଅଚଳ ଅକ୍ଷର **argv)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା no_checkout = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_parent_only = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, has_double_dash = 0, must_write_terms = 0, bad_seen = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, pathspec_pos;
	ଗଣନା bisect_error res = BISECT_OK;
	ସଂରଚନା string_list revs = STRING_LIST_INIT_DUP;
	ସଂରଚନା string_list states = STRING_LIST_INIT_DUP;
	ସଂରଚନା strbuf start_head = STRBUF_INIT;
	ସଂରଚନା strbuf bisect_names = STRBUF_INIT;
	ସଂରଚନା object_id head_oid;
	ସଂରଚନା object_id oid;
	ଗଣନା reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;
	ଅଚଳ ଅକ୍ଷର *head;

	ଯଦି (is_bare_repository(the_repository))
		no_checkout = 1;

	/*
	 * Check for one bad and then some good revisions
	 */
	ପାଇଁ (i = 0; i < argc; i++) {
		ଯଦି (!strcmp(argv[i], "--")) {
			has_double_dash = 1;
			ଭାଙ୍ଗ;
		}
	}

	ପାଇଁ (i = 0; i < argc; i++) {
		ଅଚଳ ଅକ୍ଷର *arg = argv[i];
		ଯଦି (!strcmp(argv[i], "--")) {
			ଭାଙ୍ଗ;
		} ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "--no-checkout")) {
			no_checkout = 1;
		} ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "--first-parent")) {
			first_parent_only = 1;
		} ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "--reset-when-found")) {
			reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
		} ଅନ୍ୟଥା ଯଦି (skip_prefix(arg, "--reset-when-found=", &arg)) {
			ଯଦି (parse_reset_when_found(arg, &reset_when_found)) {
				res = BISECT_FAILED;
				ଯାଅ finish;
			}
		} ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "--term-good") ||
			 !strcmp(arg, "--term-old")) {
			i++;
			ଯଦି (argc <= i)
				ଫେରନ୍ତୁ error(_("'' is not a valid term"));
			must_write_terms = 1;
			ମୁକ୍ତ((ଶୂନ୍ୟ *) terms->term_good);
			terms->term_good = xstrdup(argv[i]);
		} ଅନ୍ୟଥା ଯଦି (skip_prefix(arg, "--term-good=", &arg) ||
			   skip_prefix(arg, "--term-old=", &arg)) {
			must_write_terms = 1;
			ମୁକ୍ତ((ଶୂନ୍ୟ *) terms->term_good);
			terms->term_good = xstrdup(arg);
		} ଅନ୍ୟଥା ଯଦି (!strcmp(arg, "--term-bad") ||
			 !strcmp(arg, "--term-new")) {
			i++;
			ଯଦି (argc <= i)
				ଫେରନ୍ତୁ error(_("'' is not a valid term"));
			must_write_terms = 1;
			ମୁକ୍ତ((ଶୂନ୍ୟ *) terms->term_bad);
			terms->term_bad = xstrdup(argv[i]);
		} ଅନ୍ୟଥା ଯଦି (skip_prefix(arg, "--term-bad=", &arg) ||
			   skip_prefix(arg, "--term-new=", &arg)) {
			must_write_terms = 1;
			ମୁକ୍ତ((ଶୂନ୍ୟ *) terms->term_bad);
			terms->term_bad = xstrdup(arg);
		} ଅନ୍ୟଥା ଯଦି (starts_with(arg, "--")) {
			ଫେରନ୍ତୁ error(_("unrecognized option: '%s'"), arg);
		} ଅନ୍ୟଥା ଯଦି (!get_oidf(&oid, "%s^{commit}", arg)) {
			string_list_append(&revs, oid_to_hex(&oid));
		} ଅନ୍ୟଥା ଯଦି (has_double_dash) {
			die(_("'%s' does not appear to be a valid "
			      "revision"), arg);
		} ଅନ୍ୟଥା {
			ଭାଙ୍ଗ;
		}
	}
	ଯଦି (reset_when_found != RESET_WHEN_FOUND_NONE && no_checkout) {
		res = error(_("options '%s' and '%s' cannot be used together"),
			    "--reset-when-found", "--no-checkout");
		ଯାଅ finish;
	}
	pathspec_pos = i;

	/*
	 * The user ran "git bisect start <sha1> <sha1>", hence did not
	 * explicitly specify the terms, but we are already starting to
	 * set references named with the default terms, and won't be able
	 * to change afterwards.
	 */
	ଯଦି (revs.nr)
		must_write_terms = 1;
	ପାଇଁ (i = 0; i < revs.nr; i++) {
		ଯଦି (bad_seen) {
			string_list_append(&states, terms->term_good);
		} ଅନ୍ୟଥା {
			bad_seen = 1;
			string_list_append(&states, terms->term_bad);
		}
	}

	/*
	 * Verify HEAD
	 */
	head = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
				       "HEAD", 0, &head_oid, &flags);
	ଯଦି (!head) {
		ଯଦି (repo_get_oid(the_repository, "HEAD", &head_oid))
			ଫେରନ୍ତୁ error(_("bad HEAD - I need a HEAD"));
		head = "HEAD";
	}

	/*
	 * Check if we are bisecting
	 */
	ଯଦି (!is_empty_or_missing_file(git_path_bisect_start())) {
		/* Reset to the rev from where we started */
		strbuf_read_file(&start_head, git_path_bisect_start(), 0);
		strbuf_trim(&start_head);
		ଯଦି (!no_checkout) {
			ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;

			cmd.git_cmd = 1;
			strvec_pushl(&cmd.args, "checkout", start_head.buf,
				     "--", ଖାଲି);
			ଯଦି (run_command(&cmd)) {
				res = error(_("checking out '%s' failed."
						 " Try 'git bisect start "
						 "<valid-branch>'."),
					       start_head.buf);
				ଯାଅ finish;
			}
		}
	} ଅନ୍ୟଥା {
		/* Get the rev from where we start. */
		ଯଦି (!repo_get_oid(the_repository, head, &head_oid) &&
		    !starts_with(head, "refs/heads/")) {
			strbuf_reset(&start_head);
			strbuf_add_oid_hex(&start_head, &head_oid);
		} ଅନ୍ୟଥା ଯଦି (!repo_get_oid(the_repository, head, &head_oid) &&
			   skip_prefix(head, "refs/heads/", &head)) {
			strbuf_addstr(&start_head, head);
		} ଅନ୍ୟଥା {
			ଫେରନ୍ତୁ error(_("bad HEAD - strange symbolic ref"));
		}
	}

	/*
	 * Get rid of any old bisect state.
	 */
	ଯଦି (bisect_clean_state())
		ଫେରନ୍ତୁ BISECT_FAILED;

	/*
	 * Write new start state
	 */
	write_file(git_path_bisect_start(), "%s\n", start_head.buf);

	ଯଦି (first_parent_only)
		write_file(git_path_bisect_first_parent(), "\n");

	ଯଦି (reset_when_found != RESET_WHEN_FOUND_NONE)
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));

	ଯଦି (no_checkout) {
		ଯଦି (repo_get_oid(the_repository, start_head.buf, &oid) < 0) {
			res = error(_("invalid ref: '%s'"), start_head.buf);
			ଯାଅ finish;
		}
		ଯଦି (refs_update_ref(get_main_ref_store(the_repository), ଖାଲି, "BISECT_HEAD", &oid, ଖାଲି, 0,
				    UPDATE_REFS_ସନ୍ଦେଶ_ON_ERR)) {
			res = BISECT_FAILED;
			ଯାଅ finish;
		}
	}

	ଯଦି (pathspec_pos < argc - 1)
		sq_quote_argv(&bisect_names, argv + pathspec_pos);
	write_file(git_path_bisect_names(), "%s\n", bisect_names.buf);

	ପାଇଁ (i = 0; i < states.nr; i++)
		ଯଦି (bisect_write(states.ବସ୍ତୁଗୁଡ଼ିକ[i].string,
				 revs.ବସ୍ତୁଗୁଡ଼ିକ[i].string, terms, 1)) {
			res = BISECT_FAILED;
			ଯାଅ finish;
		}

	ଯଦି (must_write_terms && write_terms(terms->term_bad,
					    terms->term_good)) {
		res = BISECT_FAILED;
		ଯାଅ finish;
	}

	res = bisect_append_log_quoted(argv);
	ଯଦି (res)
		res = BISECT_FAILED;

finish:
	string_list_clear(&revs, 0);
	string_list_clear(&states, 0);
	strbuf_release(&start_head);
	strbuf_release(&bisect_names);
	ଯଦି (res)
		ଫେରନ୍ତୁ res;

	res = bisect_auto_next(terms, ଖାଲି);
	ଯଦି (!is_bisect_success(res))
		bisect_clean_state();
	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା file_is_not_empty(ଅଚଳ ଅକ୍ଷର *path)
{
	ଫେରନ୍ତୁ !is_empty_or_missing_file(path);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା bisect_autostart(ସଂରଚନା bisect_terms *terms)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;
	ଅଚଳ ଅକ୍ଷର *yesno;

	ଯଦି (file_is_not_empty(git_path_bisect_start()))
		ଫେରନ୍ତୁ 0;

	fprintf_ln(stderr, _("You need to start by \"git bisect "
			  "start\"\n"));

	ଯଦି (!isatty(STDIN_FILENO))
		ଫେରନ୍ତୁ -1;

	/*
	 * TRANSLATORS: Make sure to include [Y] and [n] in your
	 * translation. The program will only accept English input
	 * at this point.
	 */
	yesno = git_prompt(_("Do you want me to do it for you "
			     "[Y/n]? "), PROMPT_ECHO);
	res = tolower(*yesno) == 'n' ?
		-1 : bisect_start(terms, 0, empty_strvec);

	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ଗଣନା bisect_error bisect_state(ସଂରଚନା bisect_terms *terms, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
				      ଅଚଳ ଅକ୍ଷର **argv)
{
	ଅଚଳ ଅକ୍ଷର *state;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, verify_expected = 1;
	ସଂରଚନା object_id oid, expected;
	ସଂରଚନା oid_array revs = OID_ARRAY_INIT;

	ଯଦି (!argc)
		ଫେରନ୍ତୁ error(_("Please call `--bisect-state` with at least one argument"));

	ଯଦି (bisect_autostart(terms))
		ଫେରନ୍ତୁ BISECT_FAILED;

	state = argv[0];
	ଯଦି (check_and_set_terms(terms, state) ||
	    !one_of(state, terms->term_good, terms->term_bad, "skip", ଖାଲି))
		ଫେରନ୍ତୁ BISECT_FAILED;

	argv++;
	argc--;
	ଯଦି (argc > 1 && !strcmp(state, terms->term_bad))
		ଫେରନ୍ତୁ error(_("'git bisect %s' can take only one argument."), terms->term_bad);

	ଯଦି (argc == 0) {
		ଅଚଳ ଅକ୍ଷର *head = "BISECT_HEAD";
		ଗଣନା get_oid_result res_head = repo_get_oid(the_repository,
							    head, &oid);

		ଯଦି (res_head == MISSING_OBJECT) {
			head = "HEAD";
			res_head = repo_get_oid(the_repository, head, &oid);
		}

		ଯଦି (res_head)
			error(_("Bad rev input: %s"), head);
		oid_array_append(&revs, &oid);
	}

	/*
	 * All input revs must be checked before executing bisect_write()
	 * to discard junk revs.
	 */

	ପାଇଁ (; argc; argc--, argv++) {
		ସଂରଚନା commit *commit;

		ଯଦି (repo_get_oid(the_repository, *argv, &oid)){
			error(_("Bad rev input: %s"), *argv);
			oid_array_clear(&revs);
			ଫେରନ୍ତୁ BISECT_FAILED;
		}

		commit = lookup_commit_reference(the_repository, &oid);
		ଯଦି (!commit)
			die(_("Bad rev input (not a commit): %s"), *argv);

		oid_array_append(&revs, &commit->object.oid);
	}

	ଯଦି (refs_read_ref(get_main_ref_store(the_repository), "BISECT_EXPECTED_REV", &expected))
		verify_expected = 0; /* Ignore invalid file contents */

	ପାଇଁ (i = 0; i < revs.nr; i++) {
		ଯଦି (bisect_write(state, oid_to_hex(&revs.oid[i]), terms, 0)) {
			oid_array_clear(&revs);
			ଫେରନ୍ତୁ BISECT_FAILED;
		}
		ଯଦି (verify_expected && !oideq(&revs.oid[i], &expected)) {
			unlink_or_warn(git_path_bisect_ancestors_ok());
			refs_delete_ref(get_main_ref_store(the_repository),
					ଖାଲି, "BISECT_EXPECTED_REV", ଖାଲି,
					REF_NO_DEREF);
			verify_expected = 0;
		}
	}

	oid_array_clear(&revs);
	ଫେରନ୍ତୁ bisect_auto_next(terms, ଖାଲି);
}

ସ୍ଥିର ଗଣନା bisect_error bisect_log(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, status;
	ଅଚଳ ଅକ୍ଷର* filename = git_path_bisect_log();

	ଯଦି (is_empty_or_missing_file(filename))
		ଫେରନ୍ତୁ error(_("We are not bisecting."));

	fd = open(filename, O_RDONLY);
	ଯଦି (fd < 0)
		ଫେରନ୍ତୁ BISECT_FAILED;

	status = copy_fd(fd, STDOUT_FILENO);
	close(fd);
	ଫେରନ୍ତୁ status ? BISECT_FAILED : BISECT_OK;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା process_replay_ଧାଡ଼ି(ସଂରଚନା bisect_terms *terms, ସଂରଚନା strbuf *ଧାଡ଼ି)
{
	ଅଚଳ ଅକ୍ଷର *p = ଧାଡ଼ି->buf + strspn(ଧାଡ଼ି->buf, " \t");
	ଅକ୍ଷର *word_end, *rev;

	ଯଦି ((!skip_prefix(p, "git bisect", &p) &&
	!skip_prefix(p, "git-bisect", &p)) || !isspace(*p))
		ଫେରନ୍ତୁ 0;
	p += strspn(p, " \t");

	word_end = (ଅକ୍ଷର *)p + strcspn(p, " \t");
	rev = word_end + strspn(word_end, " \t");
	*word_end = '\0'; /* NUL-terminate the word */

	ଯଦି (get_terms(terms, 1))
		ଫେରନ୍ତୁ error(_("no terms defined"));
	ଯଦି (check_and_set_terms(terms, p))
		ଫେରନ୍ତୁ -1;

	ଯଦି (!strcmp(p, "start")) {
		ସଂରଚନା strvec argv = STRVEC_INIT;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_start(terms, argv.nr, argv.v);
		strvec_clear(&argv);
		ଫେରନ୍ତୁ res;
	}

	ଯଦି (one_of(p, terms->term_good,
	   terms->term_bad, "skip", ଖାଲି))
		ଫେରନ୍ତୁ bisect_write(p, rev, terms, 0);

	ଯଦି (!strcmp(p, "terms")) {
		ସଂରଚନା strvec argv = STRVEC_INIT;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;
		sq_dequote_to_strvec(rev, &argv);
		res = bisect_terms(terms, argv.nr == 1 ? argv.v[0] : ଖାଲି);
		strvec_clear(&argv);
		ଫେରନ୍ତୁ res;
	}
	error(_("'%s'?? what are you talking about?"), p);

	ଫେରନ୍ତୁ -1;
}

ସ୍ଥିର ଗଣନା bisect_error bisect_replay(ସଂରଚନା bisect_terms *terms, ଅଚଳ ଅକ୍ଷର *filename)
{
	FILE *fp = ଖାଲି;
	ଗଣନା bisect_error res = BISECT_OK;
	ସଂରଚନା strbuf ଧାଡ଼ି = STRBUF_INIT;

	ଯଦି (is_empty_or_missing_file(filename))
		ଫେରନ୍ତୁ error(_("cannot read file '%s' for replaying"), filename);

	ଯଦି (bisect_clean_state())
		ଫେରନ୍ତୁ BISECT_FAILED;

	fp = fopen(filename, "r");
	ଯଦି (!fp)
		ଫେରନ୍ତୁ BISECT_FAILED;

	ଯେତେବେଳେ ((strbuf_getline(&ଧାଡ଼ି, fp) != EOF) && !res)
		res = process_replay_ଧାଡ଼ି(terms, &ଧାଡ଼ି);

	strbuf_release(&ଧାଡ଼ି);
	fclose(fp);

	ଯଦି (res)
		ଫେରନ୍ତୁ BISECT_FAILED;

	ଫେରନ୍ତୁ bisect_auto_next(terms, ଖାଲି);
}

ସ୍ଥିର ଗଣନା bisect_error bisect_skip(ସଂରଚନା bisect_terms *terms, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
				     ଅଚଳ ଅକ୍ଷର **argv)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ଗଣନା bisect_error res;
	ସଂରଚନା strvec argv_state = STRVEC_INIT;

	strvec_ଠେଲ(&argv_state, "skip");

	ପାଇଁ (i = 0; i < argc; i++) {
		ଅଚଳ ଅକ୍ଷର *dotdot = strstr(argv[i], "..");

		ଯଦି (dotdot) {
			ସଂରଚନା rev_info revs;
			ସଂରଚନା commit *commit;

			repo_init_revisions(the_repository, &revs, ଖାଲି);
			setup_revisions(2, argv + i - 1, &revs, ଖାଲି);

			ଯଦି (prepare_revision_walk(&revs))
				die(_("revision walk setup failed"));
			ଯେତେବେଳେ ((commit = get_revision(&revs)) != ଖାଲି)
				strvec_ଠେଲ(&argv_state,
						oid_to_hex(&commit->object.oid));

			reset_revision_walk();
			release_revisions(&revs);
		} ଅନ୍ୟଥା {
			strvec_ଠେଲ(&argv_state, argv[i]);
		}
	}
	res = bisect_state(terms, argv_state.nr, argv_state.v);

	strvec_clear(&argv_state);
	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା bisect_visualize(ସଂରଚନା bisect_terms *terms, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
			    ଅଚଳ ଅକ୍ଷର **argv)
{
	ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;
	ସଂରଚନା strbuf sb = STRBUF_INIT;

	ଯଦି (bisect_next_check(terms, ଖାଲି) != 0)
		ଫେରନ୍ତୁ BISECT_FAILED;

	cmd.no_stdin = 1;
	ଯଦି (!argc) {
		ଯଦି ((getenv("DISPLAY") || getenv("SESSIONNAME") || getenv("MSYSTEM") ||
		     getenv("SECURITYSESSIONID")) && exists_in_PATH("gitk")) {
			strvec_ଠେଲ(&cmd.args, "gitk");
		} ଅନ୍ୟଥା {
			strvec_ଠେଲ(&cmd.args, "log");
			cmd.git_cmd = 1;
		}
	} ଅନ୍ୟଥା {
		ଯଦି (argv[0][0] == '-') {
			strvec_ଠେଲ(&cmd.args, "log");
			cmd.git_cmd = 1;
		} ଅନ୍ୟଥା ଯଦି (strcmp(argv[0], "tig") && !starts_with(argv[0], "git"))
			cmd.git_cmd = 1;

		strvec_pushv(&cmd.args, argv);
	}

	strvec_pushl(&cmd.args, "--bisect", "--", ଖାଲି);

	strbuf_read_file(&sb, git_path_bisect_names(), 0);
	sq_dequote_to_strvec(sb.buf, &cmd.args);
	strbuf_release(&sb);

	ଫେରନ୍ତୁ run_command(&cmd);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_first_good(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *cb_data)
{
	oidcpy(cb_data, ref->oid);
	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_bisect_run(ଅଚଳ ଅକ୍ଷର *command)
{
	ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;

	printf(_("running %s\n"), command);
	cmd.use_shell = 1;
	strvec_ଠେଲ(&cmd.args, command);
	ଫେରନ୍ତୁ run_command(&cmd);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା verify_good(ଅଚଳ ସଂରଚନା bisect_terms *terms, ଅଚଳ ଅକ୍ଷର *command)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା rc;
	ଗଣନା bisect_error res;
	ସଂରଚନା object_id good_rev;
	ସଂରଚନା object_id current_rev;
	ଅକ୍ଷର *good_glob = xstrfmt("%s-*", terms->term_good);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା no_checkout = refs_ref_exists(get_main_ref_store(the_repository),
					  "BISECT_HEAD");
	ସଂରଚନା refs_for_each_ref_options opts = {
		.pattern = good_glob,
		.prefix = "refs/bisect/",
		.trim_prefix = strlen("refs/bisect/"),
	};

	refs_for_each_ref_ext(get_main_ref_store(the_repository),
			      get_first_good, &good_rev, &opts);
	ମୁକ୍ତ(good_glob);

	ଯଦି (refs_read_ref(get_main_ref_store(the_repository), no_checkout ? "BISECT_HEAD" : "HEAD", &current_rev))
		ଫେରନ୍ତୁ -1;

	res = bisect_checkout(&good_rev, no_checkout);
	ଯଦି (res != BISECT_OK)
		ଫେରନ୍ତୁ -1;

	rc = do_bisect_run(command);

	res = bisect_checkout(&current_rev, no_checkout);
	ଯଦି (res != BISECT_OK)
		ଫେରନ୍ତୁ -1;

	ଫେରନ୍ତୁ rc;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା bisect_run(ସଂରଚନା bisect_terms *terms, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = BISECT_OK;
	ସଂରଚନା strbuf command = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *reset_when_found_arg;
	ଅଚଳ ଅକ୍ଷର *new_state;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା temporary_stdout_fd, saved_stdout;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_first_run = 1;
	ଗଣନା reset_when_found_mode reset_when_found = RESET_WHEN_FOUND_NONE;

	ଯଦି (bisect_next_check(terms, ଖାଲି))
		ଫେରନ୍ତୁ BISECT_FAILED;

	ଯଦି (argc && !strcmp(argv[0], "--reset-when-found")) {
		reset_when_found = RESET_WHEN_FOUND_TO_ORIGINAL;
	} ଅନ୍ୟଥା ଯଦି (argc && skip_prefix(argv[0], "--reset-when-found=",
				    &reset_when_found_arg)) {
		ଯଦି (parse_reset_when_found(reset_when_found_arg,
					   &reset_when_found))
			ଫେରନ୍ତୁ BISECT_FAILED;
	}

	ଯଦି (reset_when_found != RESET_WHEN_FOUND_NONE &&
	    refs_ref_exists(get_main_ref_store(the_repository), "BISECT_HEAD"))
		ଫେରନ୍ତୁ error(_("options '%s' and '%s' cannot be used together"),
			     "--reset-when-found", "--no-checkout");

	ଯଦି (reset_when_found != RESET_WHEN_FOUND_NONE) {
		write_file(git_path_bisect_reset_when_found(), "%s\n",
			   reset_when_found_mode_name(reset_when_found));
		argc--;
		argv++;
	}

	ଯଦି (!argc) {
		error(_("bisect run failed: no command provided."));
		ଫେରନ୍ତୁ BISECT_FAILED;
	}

	sq_quote_argv(&command, argv);
	strbuf_ltrim(&command);
	ଯେତେବେଳେ (1) {
		res = do_bisect_run(command.buf);

		/*
		 * Exit code 126 and 127 can either come from the shell
		 * if it was unable to execute or even find the script,
		 * or from the script itself.  Check with a known-good
		 * revision to avoid trashing the bisect run due to a
		 * missing or non-executable script.
		 */
		ଯଦି (is_first_run && (res == 126 || res == 127)) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା rc = verify_good(terms, command.buf);
			is_first_run = 0;
			ଯଦି (rc < 0 || 128 <= rc) {
				error(_("unable to verify %s on '%s' revision"),
				      command.buf, terms->term_good);
				res = BISECT_FAILED;
				ଭାଙ୍ଗ;
			}
			ଯଦି (rc == res) {
				error(_("bogus exit code %d for '%s' revision"),
				      rc, terms->term_good);
				res = BISECT_FAILED;
				ଭାଙ୍ଗ;
			}
		}

		ଯଦି (res < 0 || 128 <= res) {
			error(_("bisect run failed: exit code %d from"
				" %s is < 0 or >= 128"), res, command.buf);
			ଭାଙ୍ଗ;
		}

		ଯଦି (res == 125)
			new_state = "skip";
		ଅନ୍ୟଥା ଯଦି (!res)
			new_state = terms->term_good;
		ଅନ୍ୟଥା
			new_state = terms->term_bad;

		temporary_stdout_fd = open(git_path_bisect_run(), O_CREAT | O_WRONLY | O_TRUNC, 0666);

		ଯଦି (temporary_stdout_fd < 0) {
			res = error_errno(_("cannot open file '%s' for writing"), git_path_bisect_run());
			ଭାଙ୍ଗ;
		}

		fflush(stdout);
		saved_stdout = dup(1);
		ଯଦି (saved_stdout < 0 ||
		    dup2(temporary_stdout_fd, 1) < 0) {
			res = error_errno(_("could not duplicate stdout"));
			ଯଦି (saved_stdout >= 0)
				close(saved_stdout);
			close(temporary_stdout_fd);
			ଭାଙ୍ଗ;
		}

		res = bisect_state(terms, 1, &new_state);

		fflush(stdout);
		dup2(saved_stdout, 1);
		close(saved_stdout);
		close(temporary_stdout_fd);

		print_file_to_stdout(git_path_bisect_run());

		ଯଦି (res == BISECT_ONLY_SKIPPED_ବାମ)
			error(_("bisect run cannot continue any more"));
		ଅନ୍ୟଥା ଯଦି (res == BISECT_INTERNAL_SUCCESS_MERGE_BASE) {
			puts(_("bisect run success"));
			res = BISECT_OK;
		} ଅନ୍ୟଥା ଯଦି (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
			printf(_("bisect found first '%s' commit\n"), terms->term_bad);
		} ଅନ୍ୟଥା ଯଦି (res) {
			error(_("bisect run failed: 'git bisect %s'"
				" exited with error code %d"), new_state, res);
		} ଅନ୍ୟଥା {
			ଜାରି;
		}
		ଭାଙ୍ଗ;
	}

	strbuf_release(&command);
	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_bisect__reset(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix UNUSED,
			     ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;

	ଯଦି (argc > 1)
		ଫେରନ୍ତୁ error(_("'%s' requires either no argument or a commit"),
			     "git bisect reset");
	res = bisect_reset(argc ? argv[0] : ଖାଲି, false);
	ଯଦି (res)
		ଫେରନ୍ତୁ res;
	ଫେରନ୍ତୁ bisect_clean_state();
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_bisect__terms(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix UNUSED,
			     ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;
	ସଂରଚନା bisect_terms terms = { 0 };

	ଯଦି (argc > 1)
		ଫେରନ୍ତୁ error(_("'%s' requires 0 or 1 argument"),
			     "git bisect terms");
	res = bisect_terms(&terms, argc == 1 ? argv[0] : ଖାଲି);
	free_terms(&terms);
	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_bisect__start(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix UNUSED,
			     ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;
	ସଂରଚନା bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	res = bisect_start(&terms, argc, argv);
	free_terms(&terms);
	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_bisect__next(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv UNUSED, ଅଚଳ ଅକ୍ଷର *prefix,
			    ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;
	ସଂରଚନା bisect_terms terms = { 0 };

	ଯଦି (argc)
		ଫେରନ୍ତୁ error(_("'%s' requires 0 arguments"),
			     "git bisect next");
	ଯଦି (get_terms(&terms, 1))
		ଫେରନ୍ତୁ error(_("no terms defined"));
	res = bisect_next(&terms, prefix);
	free_terms(&terms);
	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_bisect__log(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc UNUSED, ଅଚଳ ଅକ୍ଷର **argv UNUSED,
			   ଅଚଳ ଅକ୍ଷର *prefix UNUSED,
			   ସଂରଚନା repository *repo UNUSED)
{
	ଫେରନ୍ତୁ bisect_log();
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_bisect__replay(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix UNUSED,
			      ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;
	ସଂରଚନା bisect_terms terms = { 0 };

	ଯଦି (argc != 1)
		ଫେରନ୍ତୁ error(_("no logfile given"));
	set_terms(&terms, "bad", "good");
	res = bisect_replay(&terms, argv[0]);
	free_terms(&terms);
	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_bisect__skip(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix UNUSED,
			    ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;
	ସଂରଚନା bisect_terms terms = { 0 };

	set_terms(&terms, "bad", "good");
	ଯଦି (get_terms(&terms, 1))
		ଫେରନ୍ତୁ error(_("no terms defined"));
	res = bisect_skip(&terms, argc, argv);
	free_terms(&terms);
	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_bisect__visualize(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix UNUSED,
				 ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;
	ସଂରଚନା bisect_terms terms = { 0 };

	ଯଦି (get_terms(&terms, 1))
		ଫେରନ୍ତୁ error(_("no terms defined"));
	res = bisect_visualize(&terms, argc, argv);
	free_terms(&terms);
	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_bisect__run(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix UNUSED,
			   ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res;
	ସଂରଚନା bisect_terms terms = { 0 };

	ଯଦି (!argc)
		ଫେରନ୍ତୁ error(_("'%s' failed: no command provided."), "git bisect run");
	ଯଦି (get_terms(&terms, 1))
		ଫେରନ୍ତୁ error(_("no terms defined"));
	res = bisect_run(&terms, argc, argv);
	free_terms(&terms);
	ଫେରନ୍ତୁ res;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_bisect(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	       ଅଚଳ ଅକ୍ଷର **argv,
	       ଅଚଳ ଅକ୍ଷର *prefix,
	       ସଂରଚନା repository *repo)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = 0;
	parse_opt_subcommand_fn *fn = ଖାଲି;
	ସଂରଚନା option options[] = {
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

	ଯଦି (!fn) {
		ସଂରଚନା bisect_terms terms = { 0 };

		ଯଦି (!argc)
			usage_ସନ୍ଦେଶ_opt(_("need a command"), git_bisect_usage, options);

		ଯଦି (!strcmp(argv[0], "help"))
			usage_with_options(git_bisect_usage, options);

		set_terms(&terms, "bad", "good");
		ଯଦି (get_terms(&terms, 1))
			ଫେରନ୍ତୁ error(_("no terms defined"));
		ଯଦି (check_and_set_terms(&terms, argv[0]) ||
		    !one_of(argv[0], terms.term_good, terms.term_bad, ଖାଲି))
			usage_ସନ୍ଦେଶ_optf(_("unknown command: '%s'"), git_bisect_usage,
				       options, argv[0]);
		ଅନ୍ୟଥା
			res = bisect_state(&terms, argc, argv);
		free_terms(&terms);
	} ଅନ୍ୟଥା {
		argc--;
		argv++;
		res = fn(argc, argv, prefix, repo);
	}

	ଯଦି (res == BISECT_INTERNAL_SUCCESS_1ST_BAD_FOUND) {
		ଗଣନା reset_when_found_mode mode;

		ଯଦି (read_reset_when_found(&mode))
			res = BISECT_FAILED;
		ଅନ୍ୟଥା ଯଦି (mode != RESET_WHEN_FOUND_NONE &&
			 bisect_reset_when_found(mode))
			res = BISECT_FAILED;
	}

	ଫେରନ୍ତୁ is_bisect_success(res) ? 0 : -res;
}
