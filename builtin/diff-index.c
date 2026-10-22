#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "diff-merges.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "preload-index.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "setup.h"

స్థిర స్థిరరాశి అక్షరం diff_cache_usage[] =
"git diff-index [-m] [--cached] [--merge-base] "
"[<common-diff-options>] <tree-ish> [<path>...]"
"\n"
COMMON_DIFF_OPTIONS_HELP;

పూర్ణాంకం cmd_diff_సూచీ(పూర్ణాంకం argc,
		   స్థిరరాశి అక్షరం **argv,
		   స్థిరరాశి అక్షరం *ఉపసర్గ,
		   నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం rev_info rev;
	గుర్తులేని పూర్ణాంకం option = 0;
	పూర్ణాంకం i;
	పూర్ణాంకం ఫలితం;

	show_usage_if_asked(argc, argv, diff_cache_usage);

	repo_config(the_repository, git_diff_basic_config, ఖాళీ); /* no "diff" UI options */

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_నిండిన_సూచీ = 0;

	repo_init_revisions(the_repository, &rev, ఉపసర్గ);
	rev.abbrev = 0;
	ఉపసర్గ = precompose_argv_ఉపసర్గ(argc, argv, ఉపసర్గ);

	/*
	 * We need (some of) diff for merges options (e.g., --cc), and we need
	 * to avoid conflict with our own meaning of "-m".
	 */
	diff_merges_suppress_m_parsing();

	argc = అమరిక_revisions(argc, argv, &rev, ఖాళీ);
	కొరకు (i = 1; i < argc; i++) {
		స్థిరరాశి అక్షరం *arg = argv[i];

		ఒకవేళ (!strcmp(arg, "--cached"))
			option |= DIFF_సూచీ_CACHED;
		లేకపోతే ఒకవేళ (!strcmp(arg, "--merge-base"))
			option |= DIFF_సూచీ_విలీనం_పునాది;
		లేకపోతే ఒకవేళ (!strcmp(arg, "-m"))
			rev.సరిపోలిక_missing = 1;
		లేకపోతే
			usage(diff_cache_usage);
	}
	ఒకవేళ (!rev.diffopt.output_format)
		rev.diffopt.output_format = DIFF_FORMAT_RAW;

	rev.diffopt.rotate_గమ్యం_strict = 1;

	/*
	 * Make sure there is one revision (i.e. pending object),
	 * and there is no revision filtering parameters.
	 */
	ఒకవేళ (rev.pending.nr != 1 ||
	    rev.max_లెక్క != -1 || rev.min_age != -1 || rev.max_age != -1)
		usage(diff_cache_usage);
	ఒకవేళ (!(option & DIFF_సూచీ_CACHED)) {
		అమరిక_work_వృక్షం(the_repository);
		ఒకవేళ (repo_read_సూచీ_preload(the_repository, &rev.diffopt.pathspec, 0) < 0) {
			perror("repo_read_index_preload");
			తిరిగి -1;
		}
	} లేకపోతే ఒకవేళ (repo_read_సూచీ(the_repository) < 0) {
		perror("repo_read_index");
		తిరిగి -1;
	}
	నిరంతరం_diff_సూచీ(&rev, option);
	ఫలితం = diff_ఫలితం_సంకేతం(&rev);
	release_revisions(&rev);
	తిరిగి ఫలితం;
}
