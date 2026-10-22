/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cache-tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ write_ਰੁੱਖ_usage[] = {
	N_("git write-tree [--missing-ok] [--prefix=<prefix>/]"),
	ਨਿਰਾ
};

ਪੂਰਨ cmd_write_ਰੁੱਖ(ਪੂਰਨ argc,
		   ਅਚਲ ਅੱਖਰ **argv,
		   ਅਚਲ ਅੱਖਰ *cmd_ਅਗੇਤਰ,
		   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ flags = 0, ret;
	ਅਚਲ ਅੱਖਰ *ਰੁੱਖ_ਅਗੇਤਰ = ਨਿਰਾ;
	ਬਣਤਰ object_id oid;
	ਅਚਲ ਅੱਖਰ *me = "git-write-tree";
	ਬਣਤਰ option write_ਰੁੱਖ_options[] = {
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "missing-ok", &flags, N_("allow missing objects"),
			WRITE_ਰੁੱਖ_MISSING_OK),
		OPT_STRING(0, "prefix", &ਰੁੱਖ_ਅਗੇਤਰ, N_("<prefix>/"),
			   N_("write tree object for a subdirectory <prefix>")),
		{
			.type = OPTION_ਦੋਆਧਾਰੀ_ਅੰਕ,
			.long_name = "ignore-cache-tree",
			.ਮੁੱਲ = &flags,
			.precision = ਆਕਾਰ(flags),
			.help = N_("only useful for debugging"),
			.flags = PARSE_OPT_HIDDEN | PARSE_OPT_NOARG,
			.defval = WRITE_ਰੁੱਖ_IGNORE_CACHE_ਰੁੱਖ,
		},
		OPT_ਸਿਰਾ()
	};

	repo_config(the_repository, git_default_config, ਨਿਰਾ);
	argc = parse_options(argc, argv, cmd_ਅਗੇਤਰ, write_ਰੁੱਖ_options,
			     write_ਰੁੱਖ_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;

	ret = write_ਸੂਚਕਾਂਕ_as_ਰੁੱਖ(&oid, the_repository->ਸੂਚਕਾਂਕ,
				  repo_get_ਸੂਚਕਾਂਕ_file(the_repository),
				  flags, ਰੁੱਖ_ਅਗੇਤਰ);
	ਚੋਣ (ret) {
	ਹਾਲਤ 0:
		ਲਿਖੋ("%s\n", oid_ਮੰਜ਼ਿਲ_hex(&oid));
		ਰੋਕੋ;
	ਹਾਲਤ WRITE_ਰੁੱਖ_UNREADABLE_ਸੂਚਕਾਂਕ:
		die("%s: error reading the index", me);
		ਰੋਕੋ;
	ਹਾਲਤ WRITE_ਰੁੱਖ_UNMERGED_ਸੂਚਕਾਂਕ:
		die("%s: error building trees", me);
		ਰੋਕੋ;
	ਹਾਲਤ WRITE_ਰੁੱਖ_ਅਗੇਤਰ_ERROR:
		die("%s: prefix %s not found", me, ਰੁੱਖ_ਅਗੇਤਰ);
		ਰੋਕੋ;
	}
	ਵਾਪਸ ret;
}
