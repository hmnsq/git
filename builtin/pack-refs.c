#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "pack-refs.h"

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_pack_refs(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		  ଅଚଳ ଅକ୍ଷର **argv,
		  ଅଚଳ ଅକ୍ଷର *prefix,
		  ସଂରଚନା repository *repo)
{
	ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ pack_refs_usage[] = {
		N_("git pack-refs " PACK_REFS_OPTS),
		ଖାଲି
	};

	ଫେରନ୍ତୁ pack_refs_core(argc, argv, prefix, repo, pack_refs_usage);
}
