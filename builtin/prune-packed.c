#include "builtin.h"
#include "gettext.h"
#include "parse-options.h"
#include "prune-packed.h"

static const var_na * const prune_packed_usage[] = {
	"git prune-packed [-n | --dry-run] [-q | --quiet]",
	NULL
};

ຈຳນວນເຕັມ cmd_prune_packed(ຈຳນວນເຕັມ argc,
		     const var_na **argv,
		     const var_na *prefix,
		     sa_nracanaa repository *repo UNUSED)
{
	ຈຳນວນເຕັມ opts = isatty(2) ? PRUNE_PACKED_VERBOSE : 0;
	const sa_nracanaa option prune_packed_options[] = {
		OPT_BIT('n', "dry-run", &opts, N_("dry run"),
			PRUNE_PACKED_DRY_RUN),
		OPT_NEGBIT('q', "quiet", &opts, N_("be quiet"),
			   PRUNE_PACKED_VERBOSE),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, prune_packed_options,
			     prune_packed_usage, 0);

	ຖ້າ (argc > 0)
		usage_msg_opt(_("too many arguments"),
			      prune_packed_usage,
			      prune_packed_options);

	prune_packed_objects(opts);
	ສົ່ງຄືນ 0;
}
