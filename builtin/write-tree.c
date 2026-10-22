/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "tree.h"
#அந்தர்பாவ "cache-tree.h"
#அந்தர்பாவ "parse-options.h"

நிலையான மாறிலி எழுத்து * மாறிலி write_மரம்_usage[] = {
	N_("git write-tree [--missing-ok] [--prefix=<prefix>/]"),
	வெற்று
};

முழு cmd_write_மரம்(முழு argc,
		   மாறிலி எழுத்து **argv,
		   மாறிலி எழுத்து *cmd_prefix,
		   கட்டமைப்பு repository *repo UNUSED)
{
	முழு flags = 0, ret;
	மாறிலி எழுத்து *மரம்_prefix = வெற்று;
	கட்டமைப்பு object_id oid;
	மாறிலி எழுத்து *me = "git-write-tree";
	கட்டமைப்பு option write_மரம்_options[] = {
		OPT_BIT(0, "missing-ok", &flags, N_("allow missing objects"),
			WRITE_மரம்_MISSING_OK),
		OPT_STRING(0, "prefix", &மரம்_prefix, N_("<prefix>/"),
			   N_("write tree object for a subdirectory <prefix>")),
		{
			.type = OPTION_BIT,
			.long_name = "ignore-cache-tree",
			.மதிப்பு = &flags,
			.precision = அளவு(flags),
			.help = N_("only useful for debugging"),
			.flags = PARSE_OPT_HIDDEN | PARSE_OPT_NOARG,
			.defval = WRITE_மரம்_IGNORE_CACHE_மரம்,
		},
		OPT_END()
	};

	repo_config(the_repository, git_default_config, வெற்று);
	argc = parse_options(argc, argv, cmd_prefix, write_மரம்_options,
			     write_மரம்_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	ret = write_index_as_மரம்(&oid, the_repository->index,
				  repo_get_index_file(the_repository),
				  flags, மரம்_prefix);
	தேர்வு (ret) {
	நிலை 0:
		printf("%s\n", oid_to_hex(&oid));
		நிறுத்து;
	நிலை WRITE_மரம்_UNREADABLE_INDEX:
		die("%s: error reading the index", me);
		நிறுத்து;
	நிலை WRITE_மரம்_UNMERGED_INDEX:
		die("%s: error building trees", me);
		நிறுத்து;
	நிலை WRITE_மரம்_PREFIX_ERROR:
		die("%s: prefix %s not found", me, மரம்_prefix);
		நிறுத்து;
	}
	திரும்பு ret;
}
