#समावेश "builtin.h"
#समावेश "gettext.h"
#समावेश "parse-options.h"
#समावेश "prune-packed.h"

अटल स्थिर अक्षर * स्थिर prune_packed_usage[] = {
	"git prune-packed [-n | --dry-run] [-q | --quiet]",
	शून्य
};

पूर्णांक cmd_prune_packed(पूर्णांक argc,
		     स्थिर अक्षर **argv,
		     स्थिर अक्षर *prefix,
		     काष्ठा repository *repo UNUSED)
{
	पूर्णांक opts = isatty(2) ? PRUNE_PACKED_VERBOSE : 0;
	स्थिर काष्ठा option prune_packed_options[] = {
		OPT_BIT('n', "dry-run", &opts, N_("dry run"),
			PRUNE_PACKED_DRY_RUN),
		OPT_NEGBIT('q', "quiet", &opts, N_("be quiet"),
			   PRUNE_PACKED_VERBOSE),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, prune_packed_options,
			     prune_packed_usage, 0);

	अगर (argc > 0)
		usage_संदेश_opt(_("too many arguments"),
			      prune_packed_usage,
			      prune_packed_options);

	prune_packed_objects(opts);
	वापस 0;
}
