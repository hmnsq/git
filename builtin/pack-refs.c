#अन्तर्भाव "builtin.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "pack-refs.h"

पूर्णांक cmd_pack_refs(पूर्णांक argc,
		  स्थिरांक अक्षर **argv,
		  स्थिरांक अक्षर *prefix,
		  संरचना repository *repo)
{
	स्थिर अक्षर स्थिरांक * स्थिरांक pack_refs_usage[] = {
		N_("git pack-refs " PACK_REFS_OPTS),
		रिक्त
	};

	परत pack_refs_core(argc, argv, prefix, repo, pack_refs_usage);
}
