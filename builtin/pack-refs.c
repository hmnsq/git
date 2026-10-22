#include "builtin.h"
#include "gettext.h"
#include "pack-refs.h"

puur_naa_nka cmd_pack_refs(puur_naa_nka argc,
		  const var_na **argv,
		  const var_na *prefix,
		  sa_nracanaa repository *repo)
{
	static var_na const * const pack_refs_usage[] = {
		N_("git pack-refs " PACK_REFS_OPTS),
		NULL
	};

	ᬯᬮᬶ pack_refs_core(argc, argv, prefix, repo, pack_refs_usage);
}
