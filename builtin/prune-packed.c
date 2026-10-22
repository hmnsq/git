#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "prune-packed.h"

நிலையான மாறிலி எழுத்து * மாறிலி prune_packed_usage[] = {
	"git prune-packed [-n | --dry-run] [-q | --quiet]",
	வெற்று
};

முழு cmd_prune_packed(முழு argc,
		     மாறிலி எழுத்து **argv,
		     மாறிலி எழுத்து *prefix,
		     கட்டமைப்பு repository *repo UNUSED)
{
	முழு opts = isatty(2) ? PRUNE_PACKED_VERBOSE : 0;
	மாறிலி கட்டமைப்பு option prune_packed_options[] = {
		OPT_BIT('n', "dry-run", &opts, N_("dry run"),
			PRUNE_PACKED_DRY_RUN),
		OPT_NEGBIT('q', "quiet", &opts, N_("be quiet"),
			   PRUNE_PACKED_VERBOSE),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, prune_packed_options,
			     prune_packed_usage, 0);

	எனில் (argc > 0)
		usage_செய்தி_opt(_("too many arguments"),
			      prune_packed_usage,
			      prune_packed_options);

	prune_packed_objects(opts);
	திரும்பு 0;
}
