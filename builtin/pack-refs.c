#include "builtin.h"
#include "gettext.h"
#include "pack-refs.h"

整数 cmd_pack_refs(整数 argc,
		  const char **argv,
		  const char *prefix,
		  struct repository *repo)
{
	static char const * const pack_refs_usage[] = {
		N_("git pack-refs " PACK_REFS_OPTS),
		NULL
	};

	戻る pack_refs_core(argc, argv, prefix, repo, pack_refs_usage);
}
