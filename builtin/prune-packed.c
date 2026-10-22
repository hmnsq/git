#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "prune-packed.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ prune_packed_usage[] = {
	"git prune-packed [-n | --dry-run] [-q | --quiet]",
	ಖಾಲಿ
};

ಪೂರ್ಣಾಂಕ cmd_prune_packed(ಪೂರ್ಣಾಂಕ argc,
		     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		     ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ opts = isatty(2) ? PRUNE_PACKED_VERBOSE : 0;
	ಸ್ಥಿರಾಂಕ ರಚನೆ option prune_packed_options[] = {
		OPT_ಬಿಟ್('n', "dry-run", &opts, N_("dry run"),
			PRUNE_PACKED_DRY_ಸತತ),
		OPT_NEGBIT('q', "quiet", &opts, N_("be quiet"),
			   PRUNE_PACKED_VERBOSE),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, prune_packed_options,
			     prune_packed_usage, 0);

	ಆದರೆ (argc > 0)
		usage_ಸಂದೇಶ_opt(_("too many arguments"),
			      prune_packed_usage,
			      prune_packed_options);

	prune_packed_objects(opts);
	ಹಿಂತಿರುಗಿ 0;
}
