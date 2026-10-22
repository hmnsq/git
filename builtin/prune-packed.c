#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "prune-packed.h"

સ્થિર અચલ અક્ષર * અચલ prune_packed_usage[] = {
	"git prune-packed [-n | --dry-run] [-q | --quiet]",
	ખાલી
};

પૂર્ણાંક cmd_prune_packed(પૂર્ણાંક argc,
		     અચલ અક્ષર **argv,
		     અચલ અક્ષર *prefix,
		     સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક opts = isatty(2) ? PRUNE_PACKED_VERBOSE : 0;
	અચલ સંરચના option prune_packed_options[] = {
		OPT_BIT('n', "dry-run", &opts, N_("dry run"),
			PRUNE_PACKED_DRY_RUN),
		OPT_NEGBIT('q', "quiet", &opts, N_("be quiet"),
			   PRUNE_PACKED_VERBOSE),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, prune_packed_options,
			     prune_packed_usage, 0);

	જો (argc > 0)
		usage_સંદેશ_opt(_("too many arguments"),
			      prune_packed_usage,
			      prune_packed_options);

	prune_packed_objects(opts);
	પાછા 0;
}
