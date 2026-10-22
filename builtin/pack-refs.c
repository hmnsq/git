#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "pack-refs.h"

ಪೂರ್ಣಾಂಕ cmd_pack_refs(ಪೂರ್ಣಾಂಕ argc,
		  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		  ರಚನೆ repository *repo)
{
	ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ pack_refs_usage[] = {
		N_("git pack-refs " PACK_REFS_OPTS),
		ಖಾಲಿ
	};

	ಹಿಂತಿರುಗಿ pack_refs_core(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, repo, pack_refs_usage);
}
