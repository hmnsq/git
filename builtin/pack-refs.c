#समावेश "builtin.h"
#समावेश "gettext.h"
#समावेश "pack-refs.h"

पूर्णांक cmd_pack_refs(पूर्णांक argc,
		  स्थिर अक्षर **argv,
		  स्थिर अक्षर *prefix,
		  काष्ठा repository *repo)
{
	अटल अक्षर स्थिर * स्थिर pack_refs_usage[] = {
		N_("git pack-refs " PACK_REFS_OPTS),
		शून्य
	};

	वापस pack_refs_core(argc, argv, prefix, repo, pack_refs_usage);
}
