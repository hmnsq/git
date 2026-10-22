/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "diff-merges.h"
#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "preload-index.h"
#அந்தர்பாவ "revision.h"

நிலையான மாறிலி எழுத்து diff_files_usage[] =
"git diff-files [-q] [-0 | -1 | -2 | -3 | -c | --cc] [<common-diff-options>] [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

முழு cmd_diff_files(முழு argc,
		   மாறிலி எழுத்து **argv,
		   மாறிலி எழுத்து *prefix,
		   கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு rev_info rev;
	முழு result;
	unsigned options = 0;

	show_usage_if_asked(argc, argv, diff_files_usage);

	repo_config(the_repository, git_diff_basic_config, வெற்று); /* no "diff" UI options */

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

	argc = setup_revisions(argc, argv, &rev, வெற்று);
	போது (1 < argc && argv[1][0] == '-') {
		எனில் (!strcmp(argv[1], "--base"))
			rev.max_மொத்தம் = 1;
		இல்லையெனில் எனில் (!strcmp(argv[1], "--ours"))
			rev.max_மொத்தம் = 2;
		இல்லையெனில் எனில் (!strcmp(argv[1], "--theirs"))
			rev.max_மொத்தம் = 3;
		இல்லையெனில் எனில் (!strcmp(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		இல்லையெனில்
			usage(diff_files_usage);
		argv++; argc--;
	}
	எனில் (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;
	rev.diffopt.rotate_to_strict = 1;

	/*
	 * Make sure there are NO revision (i.e. pending object) parameter,
	 * rev.max_count is reasonable (0 <= n <= 3), and
	 * there is no other revision filtering parameters.
	 */
	எனில் (rev.pending.nr ||
	    rev.min_age != -1 || rev.max_age != -1 ||
	    3 < rev.max_மொத்தம்)
		usage(diff_files_usage);

	/*
	 * "diff-files --base -p" should not combine merges because it
	 * was not asked to.  "diff-files -c -p" should not densify
	 * (the user should ask with "diff-files --cc" explicitly).
	 */
	எனில் (rev.max_மொத்தம் == -1 &&
	    (rev.diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_set_dense_combined_if_unset(&rev);

	எனில் (repo_read_index_preload(the_repository, &rev.diffopt.pathspec, 0) < 0)
		die_errno("repo_read_index_preload");
	run_diff_files(&rev, options);
	result = diff_result_code(&rev);
	release_revisions(&rev);
	திரும்பு result;
}
