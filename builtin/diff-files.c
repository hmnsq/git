/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "diff.h"
#ଅନ୍ତର୍ଭାଵ "diff-merges.h"
#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "preload-index.h"
#ଅନ୍ତର୍ଭାଵ "revision.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର diff_files_usage[] =
"git diff-files [-q] [-0 | -1 | -2 | -3 | -c | --cc] [<common-diff-options>] [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_diff_files(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		   ଅଚଳ ଅକ୍ଷର **argv,
		   ଅଚଳ ଅକ୍ଷର *prefix,
		   ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା rev_info rev;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା result;
	unsigned options = 0;

	show_usage_if_asked(argc, argv, diff_files_usage);

	repo_config(the_repository, git_diff_basic_config, ଖାଲି); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	repo_init_revisions(the_repository, &rev, prefix);
	rev.abbrev = 0;

	/*
	 * Consider "intent-to-add" files as new by default, unless
	 * explicitly specified in the command line or anywhere else.
	 */
	rev.diffopt.ita_invisible_in_index = 1;

	prefix = precompose_argv_prefix(argc, argv, prefix);

	argc = setup_revisions(argc, argv, &rev, ଖାଲି);
	ଯେତେବେଳେ (1 < argc && argv[1][0] == '-') {
		ଯଦି (!strcmp(argv[1], "--base"))
			rev.max_ଗଣନା = 1;
		ଅନ୍ୟଥା ଯଦି (!strcmp(argv[1], "--ours"))
			rev.max_ଗଣନା = 2;
		ଅନ୍ୟଥା ଯଦି (!strcmp(argv[1], "--theirs"))
			rev.max_ଗଣନା = 3;
		ଅନ୍ୟଥା ଯଦି (!strcmp(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		ଅନ୍ୟଥା
			usage(diff_files_usage);
		argv++; argc--;
	}
	ଯଦି (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;
	rev.diffopt.rotate_to_strict = 1;

	/*
	 * Make sure there are NO revision (i.e. pending object) parameter,
	 * rev.max_count is reasonable (0 <= n <= 3), and
	 * there is no other revision filtering parameters.
	 */
	ଯଦି (rev.pending.nr ||
	    rev.min_age != -1 || rev.max_age != -1 ||
	    3 < rev.max_ଗଣନା)
		usage(diff_files_usage);

	/*
	 * "diff-files --base -p" should not combine merges because it
	 * was not asked to.  "diff-files -c -p" should not densify
	 * (the user should ask with "diff-files --cc" explicitly).
	 */
	ଯଦି (rev.max_ଗଣନା == -1 &&
	    (rev.diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_set_dense_combined_if_unset(&rev);

	ଯଦି (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0)
		die_errno("repo_read_index_preload");
	run_diff_files(&rev, options);
	result = diff_result_code(&rev);
	release_revisions(&rev);
	ଫେରନ୍ତୁ result;
}
