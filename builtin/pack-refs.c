#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "pack-refs.h"

முழு cmd_pack_refs(முழு argc,
		  மாறிலி எழுத்து **argv,
		  மாறிலி எழுத்து *prefix,
		  கட்டமைப்பு repository *repo)
{
	நிலையான எழுத்து மாறிலி * மாறிலி pack_refs_usage[] = {
		N_("git pack-refs " PACK_REFS_OPTS),
		வெற்று
	};

	திரும்பு pack_refs_core(argc, argv, prefix, repo, pack_refs_usage);
}
