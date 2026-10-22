#define USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "abspath.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "strvec.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "pkt-line.h"
#अन्तर्भाव "bundle.h"

/*
 * Basic handler for bundle files to connect repositories via sneakernet.
 * Invocation must include action.
 * This function can create a bundle or provide information on an existing
 * bundle supporting "fetch", "pull", and "ls-remote".
 */

#define BUILTIN_BUNDLE_CREATE_USAGE \
	N_("git bundle create [-q | --quiet | --progress]\n" \
	   "                  [--version=<version>] <file> <git-rev-list-args>")
#define BUILTIN_BUNDLE_VERIFY_USAGE \
	N_("git bundle verify [-q | --quiet] <file>")
#define BUILTIN_BUNDLE_LIST_HEADS_USAGE \
	N_("git bundle list-heads <file> [<refname>...]")
#define BUILTIN_BUNDLE_UNBUNDLE_USAGE \
	N_("git bundle unbundle [--progress] <file> [<refname>...]")

स्थिर अक्षर स्थिरांक * स्थिरांक builtin_bundle_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	BUILTIN_BUNDLE_VERIFY_USAGE,
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	रिक्त,
};

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_bundle_create_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	रिक्त
};

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_bundle_verify_usage[] = {
	BUILTIN_BUNDLE_VERIFY_USAGE,
	रिक्त
};

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_bundle_list_heads_usage[] = {
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	रिक्त
};

स्थिर स्थिरांक अक्षर * स्थिरांक builtin_bundle_unbundle_usage[] = {
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	रिक्त
};

स्थिर पूर्णांक parse_options_cmd_bundle(पूर्णांक argc,
		स्थिरांक अक्षर **argv,
		स्थिरांक अक्षर* prefix,
		स्थिरांक अक्षर * स्थिरांक usagestr[],
		स्थिरांक संरचना option options[],
		अक्षर **bundle_file) {
	argc = parse_options(argc, argv, रिक्त, options, usagestr,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	जर (!argc)
		usage_संदेश_opt(_("need a <file> argument"), usagestr, options);
	*bundle_file = prefix_filename_except_for_dash(prefix, argv[0]);
	परत argc;
}

स्थिर पूर्णांक cmd_bundle_create(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
			     संरचना repository *repo UNUSED)
{
	पूर्णांक progress = isatty(STDERR_FILENO);
	पूर्णांक version = -1;
	संरचना option options[] = {
		OPT_NEGBIT('q', "quiet", &progress,
			   N_("do not show progress meter"), 1),
		OPT_BIT(0, "progress", &progress,
			N_("show progress meter"), 1),
		OPT_BIT_F(0, "all-progress", &progress,
			  N_("historical; same as --progress"), 1,
			  PARSE_OPT_HIDDEN),
		OPT_NOOP_NOARG(0, "all-progress-implied"),
		OPT_INTEGER(0, "version", &version,
			    N_("specify bundle format version")),
		OPT_END()
	};
	अक्षर *bundle_file;
	पूर्णांक ret;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_create_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	जर (!startup_info->have_repository)
		die(_("Need a repository to create a bundle."));
	ret = !!create_bundle(the_repository, bundle_file, argc, argv, version, progress);
	मुक्त(bundle_file);
	परत ret;
}

/*
 * Similar to read_bundle_header(), but handle "-" as stdin.
 */
स्थिर पूर्णांक open_bundle(स्थिरांक अक्षर *path, संरचना bundle_header *header,
		       स्थिरांक अक्षर **name)
{
	जर (!म_भेद(path, "-")) {
		जर (name)
			*name = "<stdin>";
		परत read_bundle_header_fd(0, header, "<stdin>");
	}

	जर (name)
		*name = path;
	परत read_bundle_header(path, header);
}

स्थिर पूर्णांक cmd_bundle_verify(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
			     संरचना repository *repo UNUSED) {
	संरचना bundle_header header = BUNDLE_HEADER_INIT;
	पूर्णांक bundle_fd = -1;
	पूर्णांक quiet = 0;
	पूर्णांक ret;
	संरचना option options[] = {
		OPT_BOOL('q', "quiet", &quiet,
			    N_("do not show bundle details")),
		OPT_END()
	};
	अक्षर *bundle_file;
	स्थिरांक अक्षर *name;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_verify_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	जर (!startup_info->have_repository) {
		ret = error(_("need a repository to verify a bundle"));
		जा cleanup;
	}

	जर ((bundle_fd = open_bundle(bundle_file, &header, &name)) < 0) {
		ret = 1;
		जा cleanup;
	}
	बंद(bundle_fd);
	जर (verify_bundle(the_repository, &header,
			  quiet ? VERIFY_BUNDLE_QUIET : VERIFY_BUNDLE_VERBOSE)) {
		ret = 1;
		जा cleanup;
	}

	ख_लिखो(मानक_त्रुटि, _("%s is okay\n"), name);
	ret = 0;
cleanup:
	मुक्त(bundle_file);
	bundle_header_release(&header);
	परत ret;
}

स्थिर पूर्णांक cmd_bundle_list_heads(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
				 संरचना repository *repo UNUSED) {
	संरचना bundle_header header = BUNDLE_HEADER_INIT;
	पूर्णांक bundle_fd = -1;
	पूर्णांक ret;
	संरचना option options[] = {
		OPT_END()
	};
	अक्षर *bundle_file;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_list_heads_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	जर ((bundle_fd = open_bundle(bundle_file, &header, रिक्त)) < 0) {
		ret = 1;
		जा cleanup;
	}
	बंद(bundle_fd);
	ret = !!list_bundle_refs(&header, argc, argv);
cleanup:
	मुक्त(bundle_file);
	bundle_header_release(&header);
	परत ret;
}

स्थिर पूर्णांक cmd_bundle_unbundle(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
			       संरचना repository *repo UNUSED) {
	संरचना bundle_header header = BUNDLE_HEADER_INIT;
	पूर्णांक bundle_fd = -1;
	पूर्णांक ret;
	पूर्णांक progress = isatty(2);

	संरचना option options[] = {
		OPT_BOOL(0, "progress", &progress,
			 N_("show progress meter")),
		OPT_END()
	};
	अक्षर *bundle_file;
	संरचना strvec extra_index_pack_args = STRVEC_INIT;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_unbundle_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	जर (!startup_info->have_repository)
		die(_("Need a repository to unbundle."));

	जर ((bundle_fd = open_bundle(bundle_file, &header, रिक्त)) < 0) {
		ret = 1;
		जा cleanup;
	}
	जर (progress)
		strvec_pushl(&extra_index_pack_args, "-v", "--progress-title",
			     _("Unbundling objects"), रिक्त);
	ret = !!unbundle(the_repository, &header, bundle_fd,
			 &extra_index_pack_args, रिक्त) ||
		list_bundle_refs(&header, argc, argv);
	bundle_header_release(&header);

cleanup:
	strvec_clear(&extra_index_pack_args);
	मुक्त(bundle_file);
	परत ret;
}

पूर्णांक cmd_bundle(पूर्णांक argc,
	       स्थिरांक अक्षर **argv,
	       स्थिरांक अक्षर *prefix,
	       संरचना repository *repo)
{
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option options[] = {
		OPT_SUBCOMMAND("create", &fn, cmd_bundle_create),
		OPT_SUBCOMMAND("verify", &fn, cmd_bundle_verify),
		OPT_SUBCOMMAND("list-heads", &fn, cmd_bundle_list_heads),
		OPT_SUBCOMMAND("unbundle", &fn, cmd_bundle_unbundle),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, builtin_bundle_usage,
			     0);

	packet_trace_identity("bundle");

	परत !!fn(argc, argv, prefix, repo);
}
