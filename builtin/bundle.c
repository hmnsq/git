#define USE_THE_REPOSITORY_VARIABLE
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "abspath.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "strvec.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "pkt-line.h"
#ଅନ୍ତର୍ଭାଵ "bundle.h"

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

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ * ଅଚଳ builtin_bundle_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	BUILTIN_BUNDLE_VERIFY_USAGE,
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	ଖାଲି,
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_bundle_create_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_bundle_verify_usage[] = {
	BUILTIN_BUNDLE_VERIFY_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_bundle_list_heads_usage[] = {
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_bundle_unbundle_usage[] = {
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	ଖାଲି
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_options_cmd_bundle(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		ଅଚଳ ଅକ୍ଷର **argv,
		ଅଚଳ ଅକ୍ଷର* prefix,
		ଅଚଳ ଅକ୍ଷର * ଅଚଳ usagestr[],
		ଅଚଳ ସଂରଚନା option options[],
		ଅକ୍ଷର **bundle_file) {
	argc = parse_options(argc, argv, ଖାଲି, options, usagestr,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	ଯଦି (!argc)
		usage_ସନ୍ଦେଶ_opt(_("need a <file> argument"), usagestr, options);
	*bundle_file = prefix_filename_except_for_dash(prefix, argv[0]);
	ଫେରନ୍ତୁ argc;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_bundle_create(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			     ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା progress = isatty(STDERR_FILENO);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା version = -1;
	ସଂରଚନା option options[] = {
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
	ଅକ୍ଷର *bundle_file;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_create_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ଯଦି (!startup_info->have_repository)
		die(_("Need a repository to create a bundle."));
	ret = !!create_bundle(the_repository, bundle_file, argc, argv, version, progress);
	ମୁକ୍ତ(bundle_file);
	ଫେରନ୍ତୁ ret;
}

/*
 * Similar to read_bundle_header(), but handle "-" as stdin.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା open_bundle(ଅଚଳ ଅକ୍ଷର *path, ସଂରଚନା bundle_header *header,
		       ଅଚଳ ଅକ୍ଷର **name)
{
	ଯଦି (!strcmp(path, "-")) {
		ଯଦି (name)
			*name = "<stdin>";
		ଫେରନ୍ତୁ read_bundle_header_fd(0, header, "<stdin>");
	}

	ଯଦି (name)
		*name = path;
	ଫେରନ୍ତୁ read_bundle_header(path, header);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_bundle_verify(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			     ସଂରଚନା repository *repo UNUSED) {
	ସଂରଚନା bundle_header header = BUNDLE_HEADER_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା bundle_fd = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ସଂରଚନା option options[] = {
		OPT_BOOL('q', "quiet", &quiet,
			    N_("do not show bundle details")),
		OPT_END()
	};
	ଅକ୍ଷର *bundle_file;
	ଅଚଳ ଅକ୍ଷର *name;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_verify_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ଯଦି (!startup_info->have_repository) {
		ret = error(_("need a repository to verify a bundle"));
		ଯାଅ cleanup;
	}

	ଯଦି ((bundle_fd = open_bundle(bundle_file, &header, &name)) < 0) {
		ret = 1;
		ଯାଅ cleanup;
	}
	close(bundle_fd);
	ଯଦି (verify_bundle(the_repository, &header,
			  quiet ? VERIFY_BUNDLE_QUIET : VERIFY_BUNDLE_VERBOSE)) {
		ret = 1;
		ଯାଅ cleanup;
	}

	fprintf(stderr, _("%s is okay\n"), name);
	ret = 0;
cleanup:
	ମୁକ୍ତ(bundle_file);
	bundle_header_release(&header);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_bundle_list_heads(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
				 ସଂରଚନା repository *repo UNUSED) {
	ସଂରଚନା bundle_header header = BUNDLE_HEADER_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା bundle_fd = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ସଂରଚନା option options[] = {
		OPT_END()
	};
	ଅକ୍ଷର *bundle_file;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_list_heads_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ଯଦି ((bundle_fd = open_bundle(bundle_file, &header, ଖାଲି)) < 0) {
		ret = 1;
		ଯାଅ cleanup;
	}
	close(bundle_fd);
	ret = !!list_bundle_refs(&header, argc, argv);
cleanup:
	ମୁକ୍ତ(bundle_file);
	bundle_header_release(&header);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_bundle_unbundle(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			       ସଂରଚନା repository *repo UNUSED) {
	ସଂରଚନା bundle_header header = BUNDLE_HEADER_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା bundle_fd = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା progress = isatty(2);

	ସଂରଚନା option options[] = {
		OPT_BOOL(0, "progress", &progress,
			 N_("show progress meter")),
		OPT_END()
	};
	ଅକ୍ଷର *bundle_file;
	ସଂରଚନା strvec extra_index_pack_args = STRVEC_INIT;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_unbundle_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ଯଦି (!startup_info->have_repository)
		die(_("Need a repository to unbundle."));

	ଯଦି ((bundle_fd = open_bundle(bundle_file, &header, ଖାଲି)) < 0) {
		ret = 1;
		ଯାଅ cleanup;
	}
	ଯଦି (progress)
		strvec_pushl(&extra_index_pack_args, "-v", "--progress-title",
			     _("Unbundling objects"), ଖାଲି);
	ret = !!unbundle(the_repository, &header, bundle_fd,
			 &extra_index_pack_args, ଖାଲି) ||
		list_bundle_refs(&header, argc, argv);
	bundle_header_release(&header);

cleanup:
	strvec_clear(&extra_index_pack_args);
	ମୁକ୍ତ(bundle_file);
	ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_bundle(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	       ଅଚଳ ଅକ୍ଷର **argv,
	       ଅଚଳ ଅକ୍ଷର *prefix,
	       ସଂରଚନା repository *repo)
{
	parse_opt_subcommand_fn *fn = ଖାଲି;
	ସଂରଚନା option options[] = {
		OPT_SUBCOMMAND("create", &fn, cmd_bundle_create),
		OPT_SUBCOMMAND("verify", &fn, cmd_bundle_verify),
		OPT_SUBCOMMAND("list-heads", &fn, cmd_bundle_list_heads),
		OPT_SUBCOMMAND("unbundle", &fn, cmd_bundle_unbundle),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, builtin_bundle_usage,
			     0);

	packet_trace_identity("bundle");

	ଫେରନ୍ତୁ !!fn(argc, argv, prefix, repo);
}
