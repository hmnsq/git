#شامل "builtin.h"
#شامل "gettext.h"
#شامل "pack-refs.h"

صحیح cmd_pack_refs(صحیح argc,
		  ثابت حرف **argv,
		  ثابت حرف *سابقہ,
		  ساخت repository *repo)
{
	ساکن حرف ثابت * ثابت pack_refs_usage[] = {
		N_("git pack-refs " PACK_REFS_OPTS),
		تہی
	};

	واپس pack_refs_core(argc, argv, سابقہ, repo, pack_refs_usage);
}
