#define USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "strvec.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "pkt-line.h"
#ಅನ್ತರ್ಭಾವ "bundle.h"

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

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ * ಧ್ರುವ builtin_bundle_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	BUILTIN_BUNDLE_VERIFY_USAGE,
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	ರಿಕ್ತ,
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_bundle_create_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_bundle_verify_usage[] = {
	BUILTIN_BUNDLE_VERIFY_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_bundle_list_heads_usage[] = {
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_bundle_unbundle_usage[] = {
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_options_cmd_bundle(ಪೂರ್ಣಾಙ್ಕ argc,
		ಧ್ರುವ ವರ್ಣ **argv,
		ಧ್ರುವ ವರ್ಣ* prefix,
		ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ usagestr[],
		ಧ್ರುವ ಸಂರಚನಾ option options[],
		ವರ್ಣ **bundle_file) {
	argc = parse_options(argc, argv, ರಿಕ್ತ, options, usagestr,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	ಯದಿ (!argc)
		usage_ಸಂದೇಶ_opt(_("need a <file> argument"), usagestr, options);
	*bundle_file = prefix_filename_except_for_dash(prefix, argv[0]);
	ಪ್ರತ್ಯಾಗಮ argc;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_bundle_create(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			     ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ progress = isatty(STDERR_FILENO);
	ಪೂರ್ಣಾಙ್ಕ version = -1;
	ಸಂರಚನಾ option options[] = {
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
	ವರ್ಣ *bundle_file;
	ಪೂರ್ಣಾಙ್ಕ ret;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_create_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ಯದಿ (!startup_info->have_repository)
		die(_("Need a repository to create a bundle."));
	ret = !!create_bundle(the_repository, bundle_file, argc, argv, version, progress);
	ಮುಕ್ತ(bundle_file);
	ಪ್ರತ್ಯಾಗಮ ret;
}

/*
 * Similar to read_bundle_header(), but handle "-" as stdin.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ open_bundle(ಧ್ರುವ ವರ್ಣ *path, ಸಂರಚನಾ bundle_header *header,
		       ಧ್ರುವ ವರ್ಣ **name)
{
	ಯದಿ (!strcmp(path, "-")) {
		ಯದಿ (name)
			*name = "<stdin>";
		ಪ್ರತ್ಯಾಗಮ read_bundle_header_fd(0, header, "<stdin>");
	}

	ಯದಿ (name)
		*name = path;
	ಪ್ರತ್ಯಾಗಮ read_bundle_header(path, header);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_bundle_verify(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			     ಸಂರಚನಾ repository *repo UNUSED) {
	ಸಂರಚನಾ bundle_header header = BUNDLE_HEADER_INIT;
	ಪೂರ್ಣಾಙ್ಕ bundle_fd = -1;
	ಪೂರ್ಣಾಙ್ಕ quiet = 0;
	ಪೂರ್ಣಾಙ್ಕ ret;
	ಸಂರಚನಾ option options[] = {
		OPT_BOOL('q', "quiet", &quiet,
			    N_("do not show bundle details")),
		OPT_END()
	};
	ವರ್ಣ *bundle_file;
	ಧ್ರುವ ವರ್ಣ *name;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_verify_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ಯದಿ (!startup_info->have_repository) {
		ret = error(_("need a repository to verify a bundle"));
		ಗಚ್ಛ cleanup;
	}

	ಯದಿ ((bundle_fd = open_bundle(bundle_file, &header, &name)) < 0) {
		ret = 1;
		ಗಚ್ಛ cleanup;
	}
	close(bundle_fd);
	ಯದಿ (verify_bundle(the_repository, &header,
			  quiet ? VERIFY_BUNDLE_QUIET : VERIFY_BUNDLE_VERBOSE)) {
		ret = 1;
		ಗಚ್ಛ cleanup;
	}

	fprintf(stderr, _("%s is okay\n"), name);
	ret = 0;
cleanup:
	ಮುಕ್ತ(bundle_file);
	bundle_header_release(&header);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_bundle_list_heads(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
				 ಸಂರಚನಾ repository *repo UNUSED) {
	ಸಂರಚನಾ bundle_header header = BUNDLE_HEADER_INIT;
	ಪೂರ್ಣಾಙ್ಕ bundle_fd = -1;
	ಪೂರ್ಣಾಙ್ಕ ret;
	ಸಂರಚನಾ option options[] = {
		OPT_END()
	};
	ವರ್ಣ *bundle_file;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_list_heads_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ಯದಿ ((bundle_fd = open_bundle(bundle_file, &header, ರಿಕ್ತ)) < 0) {
		ret = 1;
		ಗಚ್ಛ cleanup;
	}
	close(bundle_fd);
	ret = !!list_bundle_refs(&header, argc, argv);
cleanup:
	ಮುಕ್ತ(bundle_file);
	bundle_header_release(&header);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmd_bundle_unbundle(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
			       ಸಂರಚನಾ repository *repo UNUSED) {
	ಸಂರಚನಾ bundle_header header = BUNDLE_HEADER_INIT;
	ಪೂರ್ಣಾಙ್ಕ bundle_fd = -1;
	ಪೂರ್ಣಾಙ್ಕ ret;
	ಪೂರ್ಣಾಙ್ಕ progress = isatty(2);

	ಸಂರಚನಾ option options[] = {
		OPT_BOOL(0, "progress", &progress,
			 N_("show progress meter")),
		OPT_END()
	};
	ವರ್ಣ *bundle_file;
	ಸಂರಚನಾ strvec extra_index_pack_args = STRVEC_INIT;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_unbundle_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ಯದಿ (!startup_info->have_repository)
		die(_("Need a repository to unbundle."));

	ಯದಿ ((bundle_fd = open_bundle(bundle_file, &header, ರಿಕ್ತ)) < 0) {
		ret = 1;
		ಗಚ್ಛ cleanup;
	}
	ಯದಿ (progress)
		strvec_pushl(&extra_index_pack_args, "-v", "--progress-title",
			     _("Unbundling objects"), ರಿಕ್ತ);
	ret = !!unbundle(the_repository, &header, bundle_fd,
			 &extra_index_pack_args, ರಿಕ್ತ) ||
		list_bundle_refs(&header, argc, argv);
	bundle_header_release(&header);

cleanup:
	strvec_clear(&extra_index_pack_args);
	ಮುಕ್ತ(bundle_file);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ cmd_bundle(ಪೂರ್ಣಾಙ್ಕ argc,
	       ಧ್ರುವ ವರ್ಣ **argv,
	       ಧ್ರುವ ವರ್ಣ *prefix,
	       ಸಂರಚನಾ repository *repo)
{
	parse_opt_subcommand_fn *fn = ರಿಕ್ತ;
	ಸಂರಚನಾ option options[] = {
		OPT_SUBCOMMAND("create", &fn, cmd_bundle_create),
		OPT_SUBCOMMAND("verify", &fn, cmd_bundle_verify),
		OPT_SUBCOMMAND("list-heads", &fn, cmd_bundle_list_heads),
		OPT_SUBCOMMAND("unbundle", &fn, cmd_bundle_unbundle),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, builtin_bundle_usage,
			     0);

	packet_trace_identity("bundle");

	ಪ್ರತ್ಯಾಗಮ !!fn(argc, argv, prefix, repo);
}
