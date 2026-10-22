#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "abspath.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "strvec.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "pkt-line.h"
#અન્તર્ભાવ "bundle.h"

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

સ્થિર અક્ષર અચલ * અચલ builtin_bundle_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	BUILTIN_BUNDLE_VERIFY_USAGE,
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	ખાલી,
};

સ્થિર અચલ અક્ષર * અચલ builtin_bundle_create_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ builtin_bundle_verify_usage[] = {
	BUILTIN_BUNDLE_VERIFY_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ builtin_bundle_list_heads_usage[] = {
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ builtin_bundle_unbundle_usage[] = {
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	ખાલી
};

સ્થિર પૂર્ણાંક parse_options_cmd_bundle(પૂર્ણાંક argc,
		અચલ અક્ષર **argv,
		અચલ અક્ષર* prefix,
		અચલ અક્ષર * અચલ usagestr[],
		અચલ સંરચના option options[],
		અક્ષર **bundle_file) {
	argc = parse_options(argc, argv, ખાલી, options, usagestr,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	જો (!argc)
		usage_સંદેશ_opt(_("need a <file> argument"), usagestr, options);
	*bundle_file = prefix_filename_except_for_dash(prefix, argv[0]);
	પાછા argc;
}

સ્થિર પૂર્ણાંક cmd_bundle_create(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			     સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક progress = isatty(STDERR_FILENO);
	પૂર્ણાંક version = -1;
	સંરચના option options[] = {
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
	અક્ષર *bundle_file;
	પૂર્ણાંક ret;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_create_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	જો (!startup_info->have_repository)
		die(_("Need a repository to create a bundle."));
	ret = !!create_bundle(the_repository, bundle_file, argc, argv, version, progress);
	મુક્ત(bundle_file);
	પાછા ret;
}

/*
 * Similar to read_bundle_header(), but handle "-" as stdin.
 */
સ્થિર પૂર્ણાંક open_bundle(અચલ અક્ષર *path, સંરચના bundle_header *header,
		       અચલ અક્ષર **name)
{
	જો (!strcmp(path, "-")) {
		જો (name)
			*name = "<stdin>";
		પાછા read_bundle_header_fd(0, header, "<stdin>");
	}

	જો (name)
		*name = path;
	પાછા read_bundle_header(path, header);
}

સ્થિર પૂર્ણાંક cmd_bundle_verify(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			     સંરચના repository *repo UNUSED) {
	સંરચના bundle_header header = BUNDLE_HEADER_INIT;
	પૂર્ણાંક bundle_fd = -1;
	પૂર્ણાંક quiet = 0;
	પૂર્ણાંક ret;
	સંરચના option options[] = {
		OPT_BOOL('q', "quiet", &quiet,
			    N_("do not show bundle details")),
		OPT_END()
	};
	અક્ષર *bundle_file;
	અચલ અક્ષર *name;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_verify_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	જો (!startup_info->have_repository) {
		ret = error(_("need a repository to verify a bundle"));
		જાઓ cleanup;
	}

	જો ((bundle_fd = open_bundle(bundle_file, &header, &name)) < 0) {
		ret = 1;
		જાઓ cleanup;
	}
	close(bundle_fd);
	જો (verify_bundle(the_repository, &header,
			  quiet ? VERIFY_BUNDLE_QUIET : VERIFY_BUNDLE_VERBOSE)) {
		ret = 1;
		જાઓ cleanup;
	}

	fprintf(stderr, _("%s is okay\n"), name);
	ret = 0;
cleanup:
	મુક્ત(bundle_file);
	bundle_header_release(&header);
	પાછા ret;
}

સ્થિર પૂર્ણાંક cmd_bundle_list_heads(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
				 સંરચના repository *repo UNUSED) {
	સંરચના bundle_header header = BUNDLE_HEADER_INIT;
	પૂર્ણાંક bundle_fd = -1;
	પૂર્ણાંક ret;
	સંરચના option options[] = {
		OPT_END()
	};
	અક્ષર *bundle_file;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_list_heads_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	જો ((bundle_fd = open_bundle(bundle_file, &header, ખાલી)) < 0) {
		ret = 1;
		જાઓ cleanup;
	}
	close(bundle_fd);
	ret = !!list_bundle_refs(&header, argc, argv);
cleanup:
	મુક્ત(bundle_file);
	bundle_header_release(&header);
	પાછા ret;
}

સ્થિર પૂર્ણાંક cmd_bundle_unbundle(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			       સંરચના repository *repo UNUSED) {
	સંરચના bundle_header header = BUNDLE_HEADER_INIT;
	પૂર્ણાંક bundle_fd = -1;
	પૂર્ણાંક ret;
	પૂર્ણાંક progress = isatty(2);

	સંરચના option options[] = {
		OPT_BOOL(0, "progress", &progress,
			 N_("show progress meter")),
		OPT_END()
	};
	અક્ષર *bundle_file;
	સંરચના strvec extra_index_pack_args = STRVEC_INIT;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_unbundle_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	જો (!startup_info->have_repository)
		die(_("Need a repository to unbundle."));

	જો ((bundle_fd = open_bundle(bundle_file, &header, ખાલી)) < 0) {
		ret = 1;
		જાઓ cleanup;
	}
	જો (progress)
		strvec_pushl(&extra_index_pack_args, "-v", "--progress-title",
			     _("Unbundling objects"), ખાલી);
	ret = !!unbundle(the_repository, &header, bundle_fd,
			 &extra_index_pack_args, ખાલી) ||
		list_bundle_refs(&header, argc, argv);
	bundle_header_release(&header);

cleanup:
	strvec_clear(&extra_index_pack_args);
	મુક્ત(bundle_file);
	પાછા ret;
}

પૂર્ણાંક cmd_bundle(પૂર્ણાંક argc,
	       અચલ અક્ષર **argv,
	       અચલ અક્ષર *prefix,
	       સંરચના repository *repo)
{
	parse_opt_subcommand_fn *fn = ખાલી;
	સંરચના option options[] = {
		OPT_SUBCOMMAND("create", &fn, cmd_bundle_create),
		OPT_SUBCOMMAND("verify", &fn, cmd_bundle_verify),
		OPT_SUBCOMMAND("list-heads", &fn, cmd_bundle_list_heads),
		OPT_SUBCOMMAND("unbundle", &fn, cmd_bundle_unbundle),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, builtin_bundle_usage,
			     0);

	packet_trace_identity("bundle");

	પાછા !!fn(argc, argv, prefix, repo);
}
