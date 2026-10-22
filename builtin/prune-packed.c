#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "parse-options.h"
#অন্তর্ভাৱ "prune-packed.h"

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ prune_packed_usage[] = {
	"git prune-packed [-n | --dry-run] [-q | --quiet]",
	রিক্ত
};

পূর্ণাঙ্ক cmd_prune_packed(পূর্ণাঙ্ক argc,
		     ধ্রুৱ ৱর্ণ **argv,
		     ধ্রুৱ ৱর্ণ *prefix,
		     সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক opts = isatty(2) ? PRUNE_PACKED_VERBOSE : 0;
	ধ্রুৱ সংরচনা option prune_packed_options[] = {
		OPT_BIT('n', "dry-run", &opts, N_("dry run"),
			PRUNE_PACKED_DRY_RUN),
		OPT_NEGBIT('q', "quiet", &opts, N_("be quiet"),
			   PRUNE_PACKED_VERBOSE),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, prune_packed_options,
			     prune_packed_usage, 0);

	যদি (argc > 0)
		usage_বাৰ্তা_opt(_("too many arguments"),
			      prune_packed_usage,
			      prune_packed_options);

	prune_packed_objects(opts);
	ঘূৰাই 0;
}
