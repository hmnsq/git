#ضمن "builtin.h"
#ضمن "gettext.h"
#ضمن "pack-refs.h"

صحيح cmd_pack_refs(صحيح argc,
		  ثابت حرف **argv,
		  ثابت حرف *پیشوند,
		  بنية repository *repo)
{
	ساكن حرف ثابت * ثابت pack_refs_usage[] = {
		N_("git pack-refs " PACK_REFS_OPTS),
		عدم
	};

	أرجع pack_refs_core(argc, argv, پیشوند, repo, pack_refs_usage);
}
