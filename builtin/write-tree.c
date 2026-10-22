/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "tree.h"
#ಅನ್ತರ್ಭಾವ "cache-tree.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ write_ವೃಕ್ಷ_usage[] = {
	N_("git write-tree [--missing-ok] [--prefix=<prefix>/]"),
	ರಿಕ್ತ
};

ಪೂರ್ಣಾಙ್ಕ cmd_write_ವೃಕ್ಷ(ಪೂರ್ಣಾಙ್ಕ argc,
		   ಧ್ರುವ ವರ್ಣ **argv,
		   ಧ್ರುವ ವರ್ಣ *cmd_prefix,
		   ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ flags = 0, ret;
	ಧ್ರುವ ವರ್ಣ *ವೃಕ್ಷ_prefix = ರಿಕ್ತ;
	ಸಂರಚನಾ object_id oid;
	ಧ್ರುವ ವರ್ಣ *me = "git-write-tree";
	ಸಂರಚನಾ option write_ವೃಕ್ಷ_options[] = {
		OPT_BIT(0, "missing-ok", &flags, N_("allow missing objects"),
			WRITE_ವೃಕ್ಷ_MISSING_OK),
		OPT_STRING(0, "prefix", &ವೃಕ್ಷ_prefix, N_("<prefix>/"),
			   N_("write tree object for a subdirectory <prefix>")),
		{
			.type = OPTION_BIT,
			.long_name = "ignore-cache-tree",
			.ಮೌಲ್ಯ = &flags,
			.precision = ಪರಿಮಾಣ(flags),
			.help = N_("only useful for debugging"),
			.flags = PARSE_OPT_HIDDEN | PARSE_OPT_NOARG,
			.defval = WRITE_ವೃಕ್ಷ_IGNORE_CACHE_ವೃಕ್ಷ,
		},
		OPT_END()
	};

	repo_config(the_repository, git_default_config, ರಿಕ್ತ);
	argc = parse_options(argc, argv, cmd_prefix, write_ವೃಕ್ಷ_options,
			     write_ವೃಕ್ಷ_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	ret = write_index_as_ವೃಕ್ಷ(&oid, the_repository->index,
				  repo_get_index_file(the_repository),
				  flags, ವೃಕ್ಷ_prefix);
	ವಿಕಲ್ಪ (ret) {
	ಪಕ್ಷ 0:
		printf("%s\n", oid_to_hex(&oid));
		ಭಙ್ಗ;
	ಪಕ್ಷ WRITE_ವೃಕ್ಷ_UNREADABLE_INDEX:
		die("%s: error reading the index", me);
		ಭಙ್ಗ;
	ಪಕ್ಷ WRITE_ವೃಕ್ಷ_UNMERGED_INDEX:
		die("%s: error building trees", me);
		ಭಙ್ಗ;
	ಪಕ್ಷ WRITE_ವೃಕ್ಷ_PREFIX_ERROR:
		die("%s: prefix %s not found", me, ವೃಕ್ಷ_prefix);
		ಭಙ್ಗ;
	}
	ಪ್ರತ್ಯಾಗಮ ret;
}
