#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "prune-packed.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ prune_packed_usage[] = {
	"git prune-packed [-n | --dry-run] [-q | --quiet]",
	ರಿಕ್ತ
};

ಪೂರ್ಣಾಙ್ಕ cmd_prune_packed(ಪೂರ್ಣಾಙ್ಕ argc,
		     ಧ್ರುವ ವರ್ಣ **argv,
		     ಧ್ರುವ ವರ್ಣ *prefix,
		     ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ opts = isatty(2) ? PRUNE_PACKED_VERBOSE : 0;
	ಧ್ರುವ ಸಂರಚನಾ option prune_packed_options[] = {
		OPT_BIT('n', "dry-run", &opts, N_("dry run"),
			PRUNE_PACKED_DRY_RUN),
		OPT_NEGBIT('q', "quiet", &opts, N_("be quiet"),
			   PRUNE_PACKED_VERBOSE),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, prune_packed_options,
			     prune_packed_usage, 0);

	ಯದಿ (argc > 0)
		usage_ಸಂದೇಶ_opt(_("too many arguments"),
			      prune_packed_usage,
			      prune_packed_options);

	prune_packed_objects(opts);
	ಪ್ರತ್ಯಾಗಮ 0;
}
