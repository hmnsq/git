#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "diff.h"
#ضمن "diff-merges.h"
#ضمن "commit.h"
#ضمن "preload-index.h"
#ضمن "revision.h"
#ضمن "setup.h"

ساكن ثابت حرف diff_cache_usage[] =
"git diff-index [-m] [--cached] [--merge-base] "
"[<common-diff-options>] <tree-ish> [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

صحيح cmd_diff_index(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *prefix,
		   بنية repository *repo UNUSED)
{
	بنية rev_info rev;
	unsigned صحيح option = 0;
	صحيح i;
	صحيح result;

	show_usage_if_asked(argc, argv, diff_cache_usage);

	repo_config(the_repository, git_diff_basic_config, عدم); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	repo_init_revisions(the_repository, &rev, prefix);
	rev.abbrev = 0;
	prefix = precompose_argv_prefix(argc, argv, prefix);

	/*
	 * We need (some of) diff for merges options (e.g., --cc), and we need
	 * to avoid conflict with our own meaning of "-m".
	 */
	diff_merges_suppress_m_parsing();

	argc = setup_revisions(argc, argv, &rev, عدم);
	لكل (i = 1; i < argc; i++) {
		ثابت حرف *arg = argv[i];

		𐤀𐤌 (!strcmp(arg, "--cached"))
			option |= DIFF_INDEX_CACHED;
		وإلا 𐤀𐤌 (!strcmp(arg, "--merge-base"))
			option |= DIFF_INDEX_MERGE_BASE;
		وإلا 𐤀𐤌 (!strcmp(arg, "-m"))
			rev.match_missing = 1;
		وإلا
			usage(diff_cache_usage);
	}
	𐤀𐤌 (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	rev.diffopt.rotate_to_strict = 1;

	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	𐤀𐤌 (rev.pending.nr != 1 ||
	    rev.max_count != -1 || rev.min_age != -1 || rev.max_age != -1)
		usage(diff_cache_usage);
	𐤀𐤌 (!(option & DIFF_INDEX_CACHED)) {
		setup_work_tree(the_repository);
		𐤀𐤌 (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			أرجع -1;
		}
	} وإلا 𐤀𐤌 (repo_read_index(the_repository) < 0) {
		perror("repo_read_index");
		أرجع -1;
	}
	run_diff_index(&rev, option);
	result = diff_result_code(&rev);
	release_revisions(&rev);
	أرجع result;
}
