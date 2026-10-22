#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "prune-packed.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ prune_packed_usage[] = {
	"git prune-packed [-n | --dry-run] [-q | --quiet]",
	ਨਿਰਾ
};

ਪੂਰਨ cmd_prune_packed(ਪੂਰਨ argc,
		     ਅਚਲ ਅੱਖਰ **argv,
		     ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		     ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ opts = isatty(2) ? PRUNE_PACKED_VERBOSE : 0;
	ਅਚਲ ਬਣਤਰ option prune_packed_options[] = {
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ('n', "dry-run", &opts, N_("dry run"),
			PRUNE_PACKED_DRY_ਲੜੀਵਾਰ),
		OPT_NEGBIT('q', "quiet", &opts, N_("be quiet"),
			   PRUNE_PACKED_VERBOSE),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, prune_packed_options,
			     prune_packed_usage, 0);

	ਜੇ (argc > 0)
		usage_ਸੁਨੇਹਾ_opt(_("too many arguments"),
			      prune_packed_usage,
			      prune_packed_options);

	prune_packed_objects(opts);
	ਵਾਪਸ 0;
}
