/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "tree.h"
#అన్తర్భావ "cache-tree.h"
#అన్తర్భావ "parse-options.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి write_వృక్షం_usage[] = {
	N_("git write-tree [--missing-ok] [--prefix=<prefix>/]"),
	ఖాళీ
};

పూర్ణాంకం cmd_write_వృక్షం(పూర్ణాంకం argc,
		   స్థిరరాశి అక్షరం **argv,
		   స్థిరరాశి అక్షరం *cmd_ఉపసర్గ,
		   నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం flags = 0, ret;
	స్థిరరాశి అక్షరం *వృక్షం_ఉపసర్గ = ఖాళీ;
	నిర్మాణం object_id oid;
	స్థిరరాశి అక్షరం *me = "git-write-tree";
	నిర్మాణం option write_వృక్షం_options[] = {
		OPT_బిట్టు(0, "missing-ok", &flags, N_("allow missing objects"),
			WRITE_వృక్షం_MISSING_OK),
		OPT_STRING(0, "prefix", &వృక్షం_ఉపసర్గ, N_("<prefix>/"),
			   N_("write tree object for a subdirectory <prefix>")),
		{
			.type = OPTION_బిట్టు,
			.long_name = "ignore-cache-tree",
			.విలువ = &flags,
			.precision = పరిమాణం(flags),
			.help = N_("only useful for debugging"),
			.flags = PARSE_OPT_HIDDEN | PARSE_OPT_NOARG,
			.defval = WRITE_వృక్షం_IGNORE_CACHE_వృక్షం,
		},
		OPT_చివర()
	};

	repo_config(the_repository, git_default_config, ఖాళీ);
	argc = parse_options(argc, argv, cmd_ఉపసర్గ, write_వృక్షం_options,
			     write_వృక్షం_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_నిండిన_సూచీ = 0;

	ret = write_సూచీ_as_వృక్షం(&oid, the_repository->సూచీ,
				  repo_get_సూచీ_file(the_repository),
				  flags, వృక్షం_ఉపసర్గ);
	ఎంపిక (ret) {
	స్థితి 0:
		రాయి("%s\n", oid_గమ్యం_hex(&oid));
		ఆపు;
	స్థితి WRITE_వృక్షం_UNREADABLE_సూచీ:
		die("%s: error reading the index", me);
		ఆపు;
	స్థితి WRITE_వృక్షం_UNMERGED_సూచీ:
		die("%s: error building trees", me);
		ఆపు;
	స్థితి WRITE_వృక్షం_ఉపసర్గ_ERROR:
		die("%s: prefix %s not found", me, వృక్షం_ఉపసర్గ);
		ఆపు;
	}
	తిరిగి ret;
}
