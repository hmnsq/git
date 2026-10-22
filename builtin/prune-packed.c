#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "prune-packed.h"

স্থির ধ্রুবক অক্ষর * ধ্রুবক prune_packed_usage[] = {
	"git prune-packed [-n | --dry-run] [-q | --quiet]",
	ফাঁকা
};

পূর্ণসংখ্যা cmd_prune_packed(পূর্ণসংখ্যা argc,
		     ধ্রুবক অক্ষর **argv,
		     ধ্রুবক অক্ষর *prefix,
		     কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা opts = isatty(2) ? PRUNE_PACKED_VERBOSE : 0;
	ধ্রুবক কাঠামো option prune_packed_options[] = {
		OPT_BIT('n', "dry-run", &opts, N_("dry run"),
			PRUNE_PACKED_DRY_RUN),
		OPT_NEGBIT('q', "quiet", &opts, N_("be quiet"),
			   PRUNE_PACKED_VERBOSE),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, prune_packed_options,
			     prune_packed_usage, 0);

	যদি (argc > 0)
		usage_বার্তা_opt(_("too many arguments"),
			      prune_packed_usage,
			      prune_packed_options);

	prune_packed_objects(opts);
	ফেরত 0;
}
