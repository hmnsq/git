#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "prune-packed.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം prune_packed_usage[] = {
	"git prune-packed [-n | --dry-run] [-q | --quiet]",
	ശൂന്യ
};

പൂർണ്ണസംഖ്യ cmd_prune_packed(പൂർണ്ണസംഖ്യ argc,
		     സ്ഥിരം അക്ഷരം **argv,
		     സ്ഥിരം അക്ഷരം *prefix,
		     ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ opts = isatty(2) ? PRUNE_PACKED_VERBOSE : 0;
	സ്ഥിരം ഘടന option prune_packed_options[] = {
		OPT_BIT('n', "dry-run", &opts, N_("dry run"),
			PRUNE_PACKED_DRY_RUN),
		OPT_NEGBIT('q', "quiet", &opts, N_("be quiet"),
			   PRUNE_PACKED_VERBOSE),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, prune_packed_options,
			     prune_packed_usage, 0);

	എങ്കിൽ (argc > 0)
		usage_സന്ദേശം_opt(_("too many arguments"),
			      prune_packed_usage,
			      prune_packed_options);

	prune_packed_objects(opts);
	തിരികെ 0;
}
