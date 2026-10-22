#अन्तर्भाव "builtin.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "prune-packed.h"

स्थिर स्थिरांक अक्षर * स्थिरांक prune_packed_usage[] = {
	"git prune-packed [-n | --dry-run] [-q | --quiet]",
	रिक्त
};

पूर्णांक cmd_prune_packed(पूर्णांक argc,
		     स्थिरांक अक्षर **argv,
		     स्थिरांक अक्षर *prefix,
		     संरचना repository *repo UNUSED)
{
	पूर्णांक opts = isatty(2) ? PRUNE_PACKED_VERBOSE : 0;
	स्थिरांक संरचना option prune_packed_options[] = {
		OPT_BIT('n', "dry-run", &opts, N_("dry run"),
			PRUNE_PACKED_DRY_RUN),
		OPT_NEGBIT('q', "quiet", &opts, N_("be quiet"),
			   PRUNE_PACKED_VERBOSE),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, prune_packed_options,
			     prune_packed_usage, 0);

	जर (argc > 0)
		usage_संदेश_opt(_("too many arguments"),
			      prune_packed_usage,
			      prune_packed_options);

	prune_packed_objects(opts);
	परत 0;
}
