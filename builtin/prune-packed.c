#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "prune-packed.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ prune_packed_usage[] = {
	"git prune-packed [-n | --dry-run] [-q | --quiet]",
	ଖାଲି
};

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_prune_packed(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		     ଅଚଳ ଅକ୍ଷର **argv,
		     ଅଚଳ ଅକ୍ଷର *prefix,
		     ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା opts = isatty(2) ? PRUNE_PACKED_VERBOSE : 0;
	ଅଚଳ ସଂରଚନା option prune_packed_options[] = {
		OPT_BIT('n', "dry-run", &opts, N_("dry run"),
			PRUNE_PACKED_DRY_RUN),
		OPT_NEGBIT('q', "quiet", &opts, N_("be quiet"),
			   PRUNE_PACKED_VERBOSE),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, prune_packed_options,
			     prune_packed_usage, 0);

	ଯଦି (argc > 0)
		usage_ସନ୍ଦେଶ_opt(_("too many arguments"),
			      prune_packed_usage,
			      prune_packed_options);

	prune_packed_objects(opts);
	ଫେରନ୍ତୁ 0;
}
