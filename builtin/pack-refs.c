#include "builtin.h"
#include "gettext.h"
#include "pack-refs.h"

정수 cmd_pack_refs(정수 argc,
		  const char **argv,
		  const char *prefix,
		  struct repository *repo)
{
	static char const * const pack_refs_usage[] = {
		N_("git pack-refs " PACK_REFS_OPTS),
		NULL
	};

	반환 pack_refs_core(argc, argv, prefix, repo, pack_refs_usage);
}
