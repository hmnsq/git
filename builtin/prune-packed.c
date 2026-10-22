#ضمن "builtin.h"
#ضمن "gettext.h"
#ضمن "parse-options.h"
#ضمن "prune-packed.h"

ساكن ثابت حرف * ثابت prune_packed_usage[] = {
	"git prune-packed [-n | --dry-run] [-q | --quiet]",
	عدم
};

صحيح cmd_prune_packed(صحيح argc,
		     ثابت حرف **argv,
		     ثابت حرف *پیشوند,
		     بنية repository *repo استفادهنشده)
{
	صحيح opts = isatty(2) ? PRUNE_PACKED_VERBOSE : 0;
	ثابت بنية option prune_packed_options[] = {
		OPT_بیت('n', "dry-run", &opts, N_("dry run"),
			PRUNE_PACKED_DRY_پیاپی),
		OPT_NEGBIT('q', "quiet", &opts, N_("be quiet"),
			   PRUNE_PACKED_VERBOSE),
		OPT_انتها()
	};

	argc = parse_options(argc, argv, پیشوند, prune_packed_options,
			     prune_packed_usage, 0);

	إذا (argc > 0)
		usage_پیام_opt(_("too many arguments"),
			      prune_packed_usage,
			      prune_packed_options);

	prune_packed_objects(opts);
	أرجع 0;
}
