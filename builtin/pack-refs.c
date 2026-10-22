#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "pack-refs.h"

ಪೂರ್ಣಾಙ್ಕ cmd_pack_refs(ಪೂರ್ಣಾಙ್ಕ argc,
		  ಧ್ರುವ ವರ್ಣ **argv,
		  ಧ್ರುವ ವರ್ಣ *prefix,
		  ಸಂರಚನಾ repository *repo)
{
	ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ pack_refs_usage[] = {
		N_("git pack-refs " PACK_REFS_OPTS),
		ರಿಕ್ತ
	};

	ಪ್ರತ್ಯಾಗಮ pack_refs_core(argc, argv, prefix, repo, pack_refs_usage);
}
