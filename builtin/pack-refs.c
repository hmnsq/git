#include "builtin.h"
#include "gettext.h"
#include "pack-refs.h"

цео cmd_pack_refs(цео argc,
		  const char **argv,
		  const char *prefix,
		  struct repository *repo)
{
	static char const * const pack_refs_usage[] = {
		N_("git pack-refs " PACK_REFS_OPTS),
		NULL
	};

	врати pack_refs_core(argc, argv, prefix, repo, pack_refs_usage);
}
