/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#परिभाषय USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "tree.h"
#अन्तर्भाव "cache-tree.h"
#अन्तर्भाव "parse-options.h"

स्थिर ध्रुव वर्ण * ध्रुव write_वृक्षः_usage[] = {
	N_("git write-tree [--missing-ok] [--prefix=<prefix>/]"),
	रिक्त
};

पूर्णाङ्क cmd_write_वृक्षः(पूर्णाङ्क argc,
		   ध्रुव वर्ण **argv,
		   ध्रुव वर्ण *cmd_उपसर्गः,
		   संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क flags = 0, ret;
	ध्रुव वर्ण *वृक्षः_उपसर्गः = रिक्त;
	संरचना object_id oid;
	ध्रुव वर्ण *me = "git-write-tree";
	संरचना option write_वृक्षः_options[] = {
		OPT_BIT(0, "missing-ok", &flags, N_("allow missing objects"),
			WRITE_वृक्षः_MISSING_OK),
		OPT_STRING(0, "prefix", &वृक्षः_उपसर्गः, N_("<prefix>/"),
			   N_("write tree object for a subdirectory <prefix>")),
		{
			.type = OPTION_BIT,
			.long_name = "ignore-cache-tree",
			.मानम् = &flags,
			.precision = परिमाण(flags),
			.help = N_("only useful for debugging"),
			.flags = PARSE_OPT_HIDDEN | PARSE_OPT_NOARG,
			.defval = WRITE_वृक्षः_IGNORE_CACHE_वृक्षः,
		},
		OPT_अन्तः()
	};

	repo_config(the_repository, git_default_config, रिक्त);
	argc = parse_options(argc, argv, cmd_उपसर्गः, write_वृक्षः_options,
			     write_वृक्षः_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_पूर्णम्_सूचकांक = 0;

	ret = write_सूचकांक_as_वृक्षः(&oid, the_repository->सूचकांक,
				  repo_get_सूचकांक_file(the_repository),
				  flags, वृक्षः_उपसर्गः);
	विकल्प (ret) {
	पक्ष 0:
		प्रारूप_लिख("%s\n", oid_गन्तव्यम्_hex(&oid));
		भङ्ग;
	पक्ष WRITE_वृक्षः_UNREADABLE_सूचकांक:
		die("%s: error reading the index", me);
		भङ्ग;
	पक्ष WRITE_वृक्षः_UNMERGED_सूचकांक:
		die("%s: error building trees", me);
		भङ्ग;
	पक्ष WRITE_वृक्षः_उपसर्गः_ERROR:
		die("%s: prefix %s not found", me, वृक्षः_उपसर्गः);
		भङ्ग;
	}
	प्रत्यागम ret;
}
