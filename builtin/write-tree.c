/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "tree.h"
#अन्तर्भाव "cache-tree.h"
#अन्तर्भाव "parse-options.h"

स्थिर ध्रुव वर्ण * ध्रुव write_वृक्ष_usage[] = {
	N_("git write-tree [--missing-ok] [--prefix=<prefix>/]"),
	रिक्त
};

पूर्णाङ्क cmd_write_वृक्ष(पूर्णाङ्क argc,
		   ध्रुव वर्ण **argv,
		   ध्रुव वर्ण *cmd_prefix,
		   संरचना repository *repo UNUSED)
{
	पूर्णाङ्क flags = 0, ret;
	ध्रुव वर्ण *वृक्ष_prefix = रिक्त;
	संरचना object_id oid;
	ध्रुव वर्ण *me = "git-write-tree";
	संरचना option write_वृक्ष_options[] = {
		OPT_BIT(0, "missing-ok", &flags, N_("allow missing objects"),
			WRITE_वृक्ष_MISSING_OK),
		OPT_STRING(0, "prefix", &वृक्ष_prefix, N_("<prefix>/"),
			   N_("write tree object for a subdirectory <prefix>")),
		{
			.type = OPTION_BIT,
			.long_name = "ignore-cache-tree",
			.मूल्य = &flags,
			.precision = परिमाण(flags),
			.help = N_("only useful for debugging"),
			.flags = PARSE_OPT_HIDDEN | PARSE_OPT_NOARG,
			.defval = WRITE_वृक्ष_IGNORE_CACHE_वृक्ष,
		},
		OPT_END()
	};

	repo_config(the_repository, git_default_config, रिक्त);
	argc = parse_options(argc, argv, cmd_prefix, write_वृक्ष_options,
			     write_वृक्ष_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	ret = write_index_as_वृक्ष(&oid, the_repository->index,
				  repo_get_index_file(the_repository),
				  flags, वृक्ष_prefix);
	विकल्प (ret) {
	पक्ष 0:
		म_लिखो("%s\n", oid_to_hex(&oid));
		भङ्ग;
	पक्ष WRITE_वृक्ष_UNREADABLE_INDEX:
		die("%s: error reading the index", me);
		भङ्ग;
	पक्ष WRITE_वृक्ष_UNMERGED_INDEX:
		die("%s: error building trees", me);
		भङ्ग;
	पक्ष WRITE_वृक्ष_PREFIX_ERROR:
		die("%s: prefix %s not found", me, वृक्ष_prefix);
		भङ्ग;
	}
	प्रत्यागम ret;
}
