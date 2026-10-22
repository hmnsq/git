/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "tree.h"
#ଅନ୍ତର୍ଭାଵ "cache-tree.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ write_ବୃକ୍ଷ_usage[] = {
	N_("git write-tree [--missing-ok] [--prefix=<prefix>/]"),
	ଖାଲି
};

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_write_ବୃକ୍ଷ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		   ଅଚଳ ଅକ୍ଷର **argv,
		   ଅଚଳ ଅକ୍ଷର *cmd_prefix,
		   ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = 0, ret;
	ଅଚଳ ଅକ୍ଷର *ବୃକ୍ଷ_prefix = ଖାଲି;
	ସଂରଚନା object_id oid;
	ଅଚଳ ଅକ୍ଷର *me = "git-write-tree";
	ସଂରଚନା option write_ବୃକ୍ଷ_options[] = {
		OPT_BIT(0, "missing-ok", &flags, N_("allow missing objects"),
			WRITE_ବୃକ୍ଷ_MISSING_OK),
		OPT_STRING(0, "prefix", &ବୃକ୍ଷ_prefix, N_("<prefix>/"),
			   N_("write tree object for a subdirectory <prefix>")),
		{
			.type = OPTION_BIT,
			.long_name = "ignore-cache-tree",
			.ମୂଲ୍ୟ = &flags,
			.precision = ଆକାର(flags),
			.help = N_("only useful for debugging"),
			.flags = PARSE_OPT_HIDDEN | PARSE_OPT_NOARG,
			.defval = WRITE_ବୃକ୍ଷ_IGNORE_CACHE_ବୃକ୍ଷ,
		},
		OPT_END()
	};

	repo_config(the_repository, git_default_config, ଖାଲି);
	argc = parse_options(argc, argv, cmd_prefix, write_ବୃକ୍ଷ_options,
			     write_ବୃକ୍ଷ_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	ret = write_index_as_ବୃକ୍ଷ(&oid, the_repository->index,
				  repo_get_index_file(the_repository),
				  flags, ବୃକ୍ଷ_prefix);
	ବାଛ (ret) {
	ସ୍ଥିତି 0:
		printf("%s\n", oid_to_hex(&oid));
		ଭାଙ୍ଗ;
	ସ୍ଥିତି WRITE_ବୃକ୍ଷ_UNREADABLE_INDEX:
		die("%s: error reading the index", me);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି WRITE_ବୃକ୍ଷ_UNMERGED_INDEX:
		die("%s: error building trees", me);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି WRITE_ବୃକ୍ଷ_PREFIX_ERROR:
		die("%s: prefix %s not found", me, ବୃକ୍ଷ_prefix);
		ଭାଙ୍ଗ;
	}
	ଫେରନ୍ତୁ ret;
}
