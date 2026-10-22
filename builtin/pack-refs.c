#include "builtin.h"
#include "gettext.h"
#include "pack-refs.h"

số_nguyên cmd_pack_refs(số_nguyên argc,
		  const char **argv,
		  const char *prefix,
		  struct repository *repo)
{
	static char const * const pack_refs_usage[] = {
		N_("git pack-refs " PACK_REFS_OPTS),
		NULL
	};

	trả_về pack_refs_core(argc, argv, prefix, repo, pack_refs_usage);
}
