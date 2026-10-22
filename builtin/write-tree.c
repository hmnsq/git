/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "tree.h"
#અન્તર્ભાવ "cache-tree.h"
#અન્તર્ભાવ "parse-options.h"

સ્થિર અચલ અક્ષર * અચલ write_વૃક્ષ_usage[] = {
	N_("git write-tree [--missing-ok] [--prefix=<prefix>/]"),
	ખાલી
};

પૂર્ણાંક cmd_write_વૃક્ષ(પૂર્ણાંક argc,
		   અચલ અક્ષર **argv,
		   અચલ અક્ષર *cmd_prefix,
		   સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક flags = 0, ret;
	અચલ અક્ષર *વૃક્ષ_prefix = ખાલી;
	સંરચના object_id oid;
	અચલ અક્ષર *me = "git-write-tree";
	સંરચના option write_વૃક્ષ_options[] = {
		OPT_BIT(0, "missing-ok", &flags, N_("allow missing objects"),
			WRITE_વૃક્ષ_MISSING_OK),
		OPT_STRING(0, "prefix", &વૃક્ષ_prefix, N_("<prefix>/"),
			   N_("write tree object for a subdirectory <prefix>")),
		{
			.type = OPTION_BIT,
			.long_name = "ignore-cache-tree",
			.કિંમત = &flags,
			.precision = કદ(flags),
			.help = N_("only useful for debugging"),
			.flags = PARSE_OPT_HIDDEN | PARSE_OPT_NOARG,
			.defval = WRITE_વૃક્ષ_IGNORE_CACHE_વૃક્ષ,
		},
		OPT_END()
	};

	repo_config(the_repository, git_default_config, ખાલી);
	argc = parse_options(argc, argv, cmd_prefix, write_વૃક્ષ_options,
			     write_વૃક્ષ_usage, 0);

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	ret = write_index_as_વૃક્ષ(&oid, the_repository->index,
				  repo_get_index_file(the_repository),
				  flags, વૃક્ષ_prefix);
	પસંદગી (ret) {
	સ્થિતિ 0:
		printf("%s\n", oid_to_hex(&oid));
		તોડો;
	સ્થિતિ WRITE_વૃક્ષ_UNREADABLE_INDEX:
		die("%s: error reading the index", me);
		તોડો;
	સ્થિતિ WRITE_વૃક્ષ_UNMERGED_INDEX:
		die("%s: error building trees", me);
		તોડો;
	સ્થિતિ WRITE_વૃક્ષ_PREFIX_ERROR:
		die("%s: prefix %s not found", me, વૃક્ષ_prefix);
		તોડો;
	}
	પાછા ret;
}
