#అన్తర్భావ "builtin.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "prune-packed.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి prune_packed_usage[] = {
	"git prune-packed [-n | --dry-run] [-q | --quiet]",
	ఖాళీ
};

పూర్ణాంకం cmd_prune_packed(పూర్ణాంకం argc,
		     స్థిరరాశి అక్షరం **argv,
		     స్థిరరాశి అక్షరం *ఉపసర్గ,
		     నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం opts = isatty(2) ? PRUNE_PACKED_VERBOSE : 0;
	స్థిరరాశి నిర్మాణం option prune_packed_options[] = {
		OPT_బిట్టు('n', "dry-run", &opts, N_("dry run"),
			PRUNE_PACKED_DRY_నిరంతరం),
		OPT_NEGBIT('q', "quiet", &opts, N_("be quiet"),
			   PRUNE_PACKED_VERBOSE),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, prune_packed_options,
			     prune_packed_usage, 0);

	ఒకవేళ (argc > 0)
		usage_సందేశం_opt(_("too many arguments"),
			      prune_packed_usage,
			      prune_packed_options);

	prune_packed_objects(opts);
	తిరిగి 0;
}
