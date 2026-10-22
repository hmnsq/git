/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "diff-merges.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "preload-index.h"
#అన్తర్భావ "revision.h"

స్థిర స్థిరరాశి అక్షరం diff_files_usage[] =
"git diff-files [-q] [-0 | -1 | -2 | -3 | -c | --cc] [<common-diff-options>] [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

పూర్ణాంకం cmd_diff_files(పూర్ణాంకం argc,
		   స్థిరరాశి అక్షరం **argv,
		   స్థిరరాశి అక్షరం *ఉపసర్గ,
		   నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం rev_info rev;
	పూర్ణాంకం ఫలితం;
	గుర్తులేని options = 0;

	show_usage_if_asked(argc, argv, diff_files_usage);

	repo_config(the_repository, git_diff_basic_config, ఖాళీ); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_నిండిన_సూచీ = 0;

	repo_init_revisions(the_repository, &rev, ఉపసర్గ);
	rev.abbrev = 0;

	/*
	 * Consider "intent-to-add" files as new by default, unless
	 * explicitly specified in the command line or anywhere else.
	 */
	rev.diffopt.ita_invisible_లోపల_సూచీ = 1;

	ఉపసర్గ = precompose_argv_ఉపసర్గ(argc, argv, ఉపసర్గ);

	argc = అమరిక_revisions(argc, argv, &rev, ఖాళీ);
	అయితే (1 < argc && argv[1][0] == '-') {
		ఒకవేళ (!strcmp(argv[1], "--base"))
			rev.max_లెక్క = 1;
		లేకపోతే ఒకవేళ (!strcmp(argv[1], "--ours"))
			rev.max_లెక్క = 2;
		లేకపోతే ఒకవేళ (!strcmp(argv[1], "--theirs"))
			rev.max_లెక్క = 3;
		లేకపోతే ఒకవేళ (!strcmp(argv[1], "-q"))
			options |= DIFF_SILENT_ON_REMOVED;
		లేకపోతే
			usage(diff_files_usage);
		argv++; argc--;
	}
	ఒకవేళ (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;
	rev.diffopt.rotate_గమ్యం_strict = 1;

	/*
	 * Make sure there are NO revision (i.e. pending object) parameter,
	 * rev.max_count is reasonable (0 <= n <= 3), and
	 * there is no other revision filtering parameters.
	 */
	ఒకవేళ (rev.pending.nr ||
	    rev.min_age != -1 || rev.max_age != -1 ||
	    3 < rev.max_లెక్క)
		usage(diff_files_usage);

	/*
	 * "diff-files --base -p" should not combine merges because it
	 * was not asked to.  "diff-files -c -p" should not densify
	 * (the user should ask with "diff-files --cc" explicitly).
	 */
	ఒకవేళ (rev.max_లెక్క == -1 &&
	    (rev.diffopt.output_format & DIFF_FORMAT_PATCH))
		diff_merges_సమితి_dense_combined_if_unset(&rev);

	ఒకవేళ (repo_read_సూచీ_preload(the_repository, &rev.diffopt.pathspec, 0) < 0)
		die_errno("repo_read_index_preload");
	నిరంతరం_diff_files(&rev, options);
	ఫలితం = diff_ఫలితం_సంకేతం(&rev);
	release_revisions(&rev);
	తిరిగి ఫలితం;
}
