#شامل "builtin.h"
#شامل "gettext.h"
#شامل "pack-refs.h"

صحیح cmd_pack_refs(صحیح argc,
		  ثابت نویسه **argv,
		  ثابت نویسه *prefix,
		  ساختار repository *repo)
{
	ایستا نویسه ثابت * ثابت pack_refs_usage[] = {
		N_("git pack-refs " PACK_REFS_OPTS),
		پوچ
	};

	بازگشت pack_refs_core(argc, argv, prefix, repo, pack_refs_usage);
}
