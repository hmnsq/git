/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "tree.h"
#ಅನ್ತರ್ಭಾವ "cache-tree.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ write_ವೃಕ್ಷ_usage[] = {
	N_("git write-tree [--missing-ok] [--prefix=<prefix>/]"),
	ಖಾಲಿ
};

ಪೂರ್ಣಾಂಕ cmd_write_ವೃಕ್ಷ(ಪೂರ್ಣಾಂಕ argc,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cmd_ಪೂರ್ವಪ್ರತ್ಯಯ,
		   ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ flags = 0, ret;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ವೃಕ್ಷ_ಪೂರ್ವಪ್ರತ್ಯಯ = ಖಾಲಿ;
	ರಚನೆ object_id oid;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *me = "git-write-tree";
	ರಚನೆ option write_ವೃಕ್ಷ_options[] = {
		OPT_ಬಿಟ್(0, "missing-ok", &flags, N_("allow missing objects"),
			WRITE_ವೃಕ್ಷ_MISSING_OK),
		OPT_STRING(0, "prefix", &ವೃಕ್ಷ_ಪೂರ್ವಪ್ರತ್ಯಯ, N_("<prefix>/"),
			   N_("write tree object for a subdirectory <prefix>")),
		{
			.type = OPTION_ಬಿಟ್,
			.long_name = "ignore-cache-tree",
			.ಮೌಲ್ಯ = &flags,
			.precision = ಗಾತ್ರ(flags),
			.help = N_("only useful for debugging"),
			.flags = PARSE_OPT_HIDDEN | PARSE_OPT_NOARG,
			.defval = WRITE_ವೃಕ್ಷ_IGNORE_CACHE_ವೃಕ್ಷ,
		},
		OPT_ಕೊನೆ()
	};

	repo_config(the_repository, git_default_config, ಖಾಲಿ);
	argc = parse_options(argc, argv, cmd_ಪೂರ್ವಪ್ರತ್ಯಯ, write_ವೃಕ್ಷ_options,
			     write_ವೃಕ್ಷ_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;

	ret = write_ಸೂಚ್ಯಂಕ_as_ವೃಕ್ಷ(&oid, the_repository->ಸೂಚ್ಯಂಕ,
				  repo_get_ಸೂಚ್ಯಂಕ_file(the_repository),
				  flags, ವೃಕ್ಷ_ಪೂರ್ವಪ್ರತ್ಯಯ);
	ಆಯ್ಕೆ (ret) {
	ಸ್ಥಿತಿ 0:
		ಬರೆ("%s\n", oid_ಗಮ್ಯ_hex(&oid));
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ WRITE_ವೃಕ್ಷ_UNREADABLE_ಸೂಚ್ಯಂಕ:
		die("%s: error reading the index", me);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ WRITE_ವೃಕ್ಷ_UNMERGED_ಸೂಚ್ಯಂಕ:
		die("%s: error building trees", me);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ WRITE_ವೃಕ್ಷ_ಪೂರ್ವಪ್ರತ್ಯಯ_ERROR:
		die("%s: prefix %s not found", me, ವೃಕ್ಷ_ಪೂರ್ವಪ್ರತ್ಯಯ);
		ನಿಲ್ಲಿಸು;
	}
	ಹಿಂತಿರುಗಿ ret;
}
