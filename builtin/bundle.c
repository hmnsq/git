#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "strvec.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "pkt-line.h"
#அந்தர்பாவ "bundle.h"

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

நிலையான எழுத்து மாறிலி * மாறிலி builtin_bundle_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	BUILTIN_BUNDLE_VERIFY_USAGE,
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	வெற்று,
};

நிலையான மாறிலி எழுத்து * மாறிலி builtin_bundle_create_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி builtin_bundle_verify_usage[] = {
	BUILTIN_BUNDLE_VERIFY_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி builtin_bundle_list_heads_usage[] = {
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி builtin_bundle_unbundle_usage[] = {
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	வெற்று
};

நிலையான முழு parse_options_cmd_bundle(முழு argc,
		மாறிலி எழுத்து **argv,
		மாறிலி எழுத்து* prefix,
		மாறிலி எழுத்து * மாறிலி usagestr[],
		மாறிலி கட்டமைப்பு option options[],
		எழுத்து **bundle_file) {
	argc = parse_options(argc, argv, வெற்று, options, usagestr,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	எனில் (!argc)
		usage_செய்தி_opt(_("need a <file> argument"), usagestr, options);
	*bundle_file = prefix_filename_except_for_dash(prefix, argv[0]);
	திரும்பு argc;
}

நிலையான முழு cmd_bundle_create(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			     கட்டமைப்பு repository *repo UNUSED)
{
	முழு progress = isatty(STDERR_FILENO);
	முழு version = -1;
	கட்டமைப்பு option options[] = {
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
	எழுத்து *bundle_file;
	முழு ret;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_create_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	எனில் (!startup_info->have_repository)
		die(_("Need a repository to create a bundle."));
	ret = !!create_bundle(the_repository, bundle_file, argc, argv, version, progress);
	விடுவி(bundle_file);
	திரும்பு ret;
}

/*
 * Similar to read_bundle_header(), but handle "-" as stdin.
 */
நிலையான முழு open_bundle(மாறிலி எழுத்து *path, கட்டமைப்பு bundle_header *header,
		       மாறிலி எழுத்து **name)
{
	எனில் (!strcmp(path, "-")) {
		எனில் (name)
			*name = "<stdin>";
		திரும்பு read_bundle_header_fd(0, header, "<stdin>");
	}

	எனில் (name)
		*name = path;
	திரும்பு read_bundle_header(path, header);
}

நிலையான முழு cmd_bundle_verify(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			     கட்டமைப்பு repository *repo UNUSED) {
	கட்டமைப்பு bundle_header header = BUNDLE_HEADER_INIT;
	முழு bundle_fd = -1;
	முழு quiet = 0;
	முழு ret;
	கட்டமைப்பு option options[] = {
		OPT_BOOL('q', "quiet", &quiet,
			    N_("do not show bundle details")),
		OPT_END()
	};
	எழுத்து *bundle_file;
	மாறிலி எழுத்து *name;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_verify_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	எனில் (!startup_info->have_repository) {
		ret = error(_("need a repository to verify a bundle"));
		செல் cleanup;
	}

	எனில் ((bundle_fd = open_bundle(bundle_file, &header, &name)) < 0) {
		ret = 1;
		செல் cleanup;
	}
	close(bundle_fd);
	எனில் (verify_bundle(the_repository, &header,
			  quiet ? VERIFY_BUNDLE_QUIET : VERIFY_BUNDLE_VERBOSE)) {
		ret = 1;
		செல் cleanup;
	}

	fprintf(stderr, _("%s is okay\n"), name);
	ret = 0;
cleanup:
	விடுவி(bundle_file);
	bundle_header_release(&header);
	திரும்பு ret;
}

நிலையான முழு cmd_bundle_list_heads(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
				 கட்டமைப்பு repository *repo UNUSED) {
	கட்டமைப்பு bundle_header header = BUNDLE_HEADER_INIT;
	முழு bundle_fd = -1;
	முழு ret;
	கட்டமைப்பு option options[] = {
		OPT_END()
	};
	எழுத்து *bundle_file;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_list_heads_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	எனில் ((bundle_fd = open_bundle(bundle_file, &header, வெற்று)) < 0) {
		ret = 1;
		செல் cleanup;
	}
	close(bundle_fd);
	ret = !!list_bundle_refs(&header, argc, argv);
cleanup:
	விடுவி(bundle_file);
	bundle_header_release(&header);
	திரும்பு ret;
}

நிலையான முழு cmd_bundle_unbundle(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
			       கட்டமைப்பு repository *repo UNUSED) {
	கட்டமைப்பு bundle_header header = BUNDLE_HEADER_INIT;
	முழு bundle_fd = -1;
	முழு ret;
	முழு progress = isatty(2);

	கட்டமைப்பு option options[] = {
		OPT_BOOL(0, "progress", &progress,
			 N_("show progress meter")),
		OPT_END()
	};
	எழுத்து *bundle_file;
	கட்டமைப்பு strvec extra_index_pack_args = STRVEC_INIT;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_unbundle_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	எனில் (!startup_info->have_repository)
		die(_("Need a repository to unbundle."));

	எனில் ((bundle_fd = open_bundle(bundle_file, &header, வெற்று)) < 0) {
		ret = 1;
		செல் cleanup;
	}
	எனில் (progress)
		strvec_pushl(&extra_index_pack_args, "-v", "--progress-title",
			     _("Unbundling objects"), வெற்று);
	ret = !!unbundle(the_repository, &header, bundle_fd,
			 &extra_index_pack_args, வெற்று) ||
		list_bundle_refs(&header, argc, argv);
	bundle_header_release(&header);

cleanup:
	strvec_clear(&extra_index_pack_args);
	விடுவி(bundle_file);
	திரும்பு ret;
}

முழு cmd_bundle(முழு argc,
	       மாறிலி எழுத்து **argv,
	       மாறிலி எழுத்து *prefix,
	       கட்டமைப்பு repository *repo)
{
	parse_opt_subcommand_fn *fn = வெற்று;
	கட்டமைப்பு option options[] = {
		OPT_SUBCOMMAND("create", &fn, cmd_bundle_create),
		OPT_SUBCOMMAND("verify", &fn, cmd_bundle_verify),
		OPT_SUBCOMMAND("list-heads", &fn, cmd_bundle_list_heads),
		OPT_SUBCOMMAND("unbundle", &fn, cmd_bundle_unbundle),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, builtin_bundle_usage,
			     0);

	packet_trace_identity("bundle");

	திரும்பு !!fn(argc, argv, prefix, repo);
}
