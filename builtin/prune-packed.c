#شامل "builtin.h"
#شامل "gettext.h"
#شامل "parse-options.h"
#شامل "prune-packed.h"

ساکن ثابت حرف * ثابت prune_packed_usage[] = {
	"git prune-packed [-n | --dry-run] [-q | --quiet]",
	تہی
};

صحیح cmd_prune_packed(صحیح argc,
		     ثابت حرف **argv,
		     ثابت حرف *سابقہ,
		     ساخت repository *repo غیر_مستعمل)
{
	صحیح opts = isatty(2) ? PRUNE_PACKED_VERBOSE : 0;
	ثابت ساخت option prune_packed_options[] = {
		OPT_ثنائی_ہندسہ('n', "dry-run", &opts, N_("dry run"),
			PRUNE_PACKED_DRY_تسلسل),
		OPT_NEGBIT('q', "quiet", &opts, N_("be quiet"),
			   PRUNE_PACKED_VERBOSE),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, prune_packed_options,
			     prune_packed_usage, 0);

	اگر (argc > 0)
		usage_پیغام_opt(_("too many arguments"),
			      prune_packed_usage,
			      prune_packed_options);

	prune_packed_objects(opts);
	واپس 0;
}
