/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "tree.h"
#समावेश "cache-tree.h"
#समावेश "parse-options.h"

अटल स्थिर अक्षर * स्थिर write_वृक्ष_usage[] = {
	N_("git write-tree [--missing-ok] [--prefix=<prefix>/]"),
	शून्य
};

पूर्णांक cmd_write_वृक्ष(पूर्णांक argc,
		   स्थिर अक्षर **argv,
		   स्थिर अक्षर *cmd_prefix,
		   काष्ठा repository *repo UNUSED)
{
	पूर्णांक flags = 0, ret;
	स्थिर अक्षर *वृक्ष_prefix = शून्य;
	काष्ठा object_id oid;
	स्थिर अक्षर *me = "git-write-tree";
	काष्ठा option write_वृक्ष_options[] = {
		OPT_BIT(0, "missing-ok", &flags, N_("allow missing objects"),
			WRITE_वृक्ष_MISSING_OK),
		OPT_STRING(0, "prefix", &वृक्ष_prefix, N_("<prefix>/"),
			   N_("write tree object for a subdirectory <prefix>")),
		{
			.type = OPTION_BIT,
			.long_name = "ignore-cache-tree",
			.मूल्य = &flags,
			.precision = माप(flags),
			.help = N_("only useful for debugging"),
			.flags = PARSE_OPT_HIDDEN | PARSE_OPT_NOARG,
			.defval = WRITE_वृक्ष_IGNORE_CACHE_वृक्ष,
		},
		OPT_END()
	};

	repo_config(the_repository, git_default_config, शून्य);
	argc = parse_options(argc, argv, cmd_prefix, write_वृक्ष_options,
			     write_वृक्ष_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_सूचकांक = 0;

	ret = write_सूचकांक_as_वृक्ष(&oid, the_repository->सूचकांक,
				  repo_get_सूचकांक_file(the_repository),
				  flags, वृक्ष_prefix);
	चयन (ret) {
	हाल 0:
		म_लिखो("%s\n", oid_to_hex(&oid));
		अवरोध;
	हाल WRITE_वृक्ष_UNREADABLE_सूचकांक:
		die("%s: error reading the index", me);
		अवरोध;
	हाल WRITE_वृक्ष_UNMERGED_सूचकांक:
		die("%s: error building trees", me);
		अवरोध;
	हाल WRITE_वृक्ष_PREFIX_ERROR:
		die("%s: prefix %s not found", me, वृक्ष_prefix);
		अवरोध;
	}
	वापस ret;
}
