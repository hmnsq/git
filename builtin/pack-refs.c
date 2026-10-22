#अन्तर्भाव "builtin.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "pack-refs.h"

पूर्णाङ्क cmd_pack_refs(पूर्णाङ्क argc,
		  ध्रुव वर्ण **argv,
		  ध्रुव वर्ण *prefix,
		  संरचना repository *repo)
{
	स्थिर वर्ण ध्रुव * ध्रुव pack_refs_usage[] = {
		N_("git pack-refs " PACK_REFS_OPTS),
		रिक्त
	};

	प्रत्यागम pack_refs_core(argc, argv, prefix, repo, pack_refs_usage);
}
