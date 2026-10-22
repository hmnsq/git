#अन्तर्भाव "builtin.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "pack-refs.h"

पूर्णाङ्क cmd_pack_refs(पूर्णाङ्क argc,
		  अचर वर्ण **argv,
		  अचर वर्ण *prefix,
		  संरचना repository *repo)
{
	स्थिर वर्ण अचर * अचर pack_refs_usage[] = {
		N_("git pack-refs " PACK_REFS_OPTS),
		रिक्त
	};

	फर्काऊ pack_refs_core(argc, argv, prefix, repo, pack_refs_usage);
}
