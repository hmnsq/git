#include "builtin.h"
#include "gettext.h"
#include "pack-refs.h"

цяло cmd_pack_refs(цяло argc,
		  const char **argv,
		  const char *prefix,
		  struct repository *repo)
{
	static char const * const pack_refs_usage[] = {
		N_("git pack-refs " PACK_REFS_OPTS),
		NULL
	};

	върни pack_refs_core(argc, argv, prefix, repo, pack_refs_usage);
}
