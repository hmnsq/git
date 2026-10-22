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

स्थिर अचर वर्ण * अचर write_रूख_usage[] = {
	N_("git write-tree [--missing-ok] [--prefix=<prefix>/]"),
	रिक्त
};

पूर्णाङ्क cmd_write_रूख(पूर्णाङ्क argc,
		   अचर वर्ण **argv,
		   अचर वर्ण *cmd_prefix,
		   संरचना repository *repo UNUSED)
{
	पूर्णाङ्क flags = 0, ret;
	अचर वर्ण *रूख_prefix = रिक्त;
	संरचना object_id oid;
	अचर वर्ण *me = "git-write-tree";
	संरचना option write_रूख_options[] = {
		OPT_BIT(0, "missing-ok", &flags, N_("allow missing objects"),
			WRITE_रूख_MISSING_OK),
		OPT_STRING(0, "prefix", &रूख_prefix, N_("<prefix>/"),
			   N_("write tree object for a subdirectory <prefix>")),
		{
			.type = OPTION_BIT,
			.long_name = "ignore-cache-tree",
			.मान = &flags,
			.precision = परिमाण(flags),
			.help = N_("only useful for debugging"),
			.flags = PARSE_OPT_HIDDEN | PARSE_OPT_NOARG,
			.defval = WRITE_रूख_IGNORE_CACHE_रूख,
		},
		OPT_END()
	};

	repo_config(the_repository, git_default_config, रिक्त);
	argc = parse_options(argc, argv, cmd_prefix, write_रूख_options,
			     write_रूख_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	ret = write_index_as_रूख(&oid, the_repository->index,
				  repo_get_index_file(the_repository),
				  flags, रूख_prefix);
	विकल्प (ret) {
	पक्ष 0:
		म_लिखो("%s\n", oid_to_hex(&oid));
		रोक;
	पक्ष WRITE_रूख_UNREADABLE_INDEX:
		die("%s: error reading the index", me);
		रोक;
	पक्ष WRITE_रूख_UNMERGED_INDEX:
		die("%s: error building trees", me);
		रोक;
	पक्ष WRITE_रूख_PREFIX_ERROR:
		die("%s: prefix %s not found", me, रूख_prefix);
		रोक;
	}
	फर्काऊ ret;
}
