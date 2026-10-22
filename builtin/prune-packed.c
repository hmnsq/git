#अन्तर्भाव "builtin.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "prune-packed.h"

स्थिर ध्रुव वर्ण * ध्रुव prune_packed_usage[] = {
	"git prune-packed [-n | --dry-run] [-q | --quiet]",
	रिक्त
};

पूर्णाङ्क cmd_prune_packed(पूर्णाङ्क argc,
		     ध्रुव वर्ण **argv,
		     ध्रुव वर्ण *prefix,
		     संरचना repository *repo UNUSED)
{
	पूर्णाङ्क opts = isatty(2) ? PRUNE_PACKED_VERBOSE : 0;
	ध्रुव संरचना option prune_packed_options[] = {
		OPT_BIT('n', "dry-run", &opts, N_("dry run"),
			PRUNE_PACKED_DRY_RUN),
		OPT_NEGBIT('q', "quiet", &opts, N_("be quiet"),
			   PRUNE_PACKED_VERBOSE),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, prune_packed_options,
			     prune_packed_usage, 0);

	जइ (argc > 0)
		usage_सन्देशः_opt(_("too many arguments"),
			      prune_packed_usage,
			      prune_packed_options);

	prune_packed_objects(opts);
	प्रत्यागम 0;
}
