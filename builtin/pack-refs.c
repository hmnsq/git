#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pack-refs.h"

ਪੂਰਨ cmd_pack_refs(ਪੂਰਨ argc,
		  ਅਚਲ ਅੱਖਰ **argv,
		  ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		  ਬਣਤਰ repository *repo)
{
	ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ pack_refs_usage[] = {
		N_("git pack-refs " PACK_REFS_OPTS),
		ਨਿਰਾ
	};

	ਵਾਪਸ pack_refs_core(argc, argv, ਅਗੇਤਰ, repo, pack_refs_usage);
}
