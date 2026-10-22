#شامل "builtin.h"
#شامل "gettext.h"
#شامل "parse-options.h"
#شامل "prune-packed.h"

ایستا ثابت نویسه * ثابت prune_packed_usage[] = {
	"git prune-packed [-n | --dry-run] [-q | --quiet]",
	پوچ
};

صحیح cmd_prune_packed(صحیح argc,
		     ثابت نویسه **argv,
		     ثابت نویسه *prefix,
		     ساختار repository *repo UNUSED)
{
	صحیح opts = isatty(2) ? PRUNE_PACKED_VERBOSE : 0;
	ثابت ساختار option prune_packed_options[] = {
		OPT_BIT('n', "dry-run", &opts, N_("dry run"),
			PRUNE_PACKED_DRY_RUN),
		OPT_NEGBIT('q', "quiet", &opts, N_("be quiet"),
			   PRUNE_PACKED_VERBOSE),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, prune_packed_options,
			     prune_packed_usage, 0);

	اگر (argc > 0)
		usage_پیام_opt(_("too many arguments"),
			      prune_packed_usage,
			      prune_packed_options);

	prune_packed_objects(opts);
	بازگشت 0;
}
