#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
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

#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_BUNDLE_CREATE_USAGE \
	N_("git bundle create [-q | --quiet | --progress]\n" \
	   "                  [--version=<version>] <file> <git-rev-list-args>")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_BUNDLE_VERIFY_USAGE \
	N_("git bundle verify [-q | --quiet] <file>")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_BUNDLE_LIST_HEADS_USAGE \
	N_("git bundle list-heads <file> [<refname>...]")
#ವ್ಯಾಖ್ಯಾನಿಸು BUILTIN_BUNDLE_UNBUNDLE_USAGE \
	N_("git bundle unbundle [--progress] <file> [<refname>...]")

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ builtin_bundle_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	BUILTIN_BUNDLE_VERIFY_USAGE,
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	ಖಾಲಿ,
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_bundle_create_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_bundle_verify_usage[] = {
	BUILTIN_BUNDLE_VERIFY_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_bundle_list_heads_usage[] = {
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_bundle_unbundle_usage[] = {
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_options_cmd_bundle(ಪೂರ್ಣಾಂಕ argc,
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಪೂರ್ವಪ್ರತ್ಯಯ,
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ usagestr[],
		ಸ್ಥಿರಾಂಕ ರಚನೆ option options[],
		ಅಕ್ಷರ **bundle_file) {
	argc = parse_options(argc, argv, ಖಾಲಿ, options, usagestr,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	ಆದರೆ (!argc)
		usage_ಸಂದೇಶ_opt(_("need a <file> argument"), usagestr, options);
	*bundle_file = ಪೂರ್ವಪ್ರತ್ಯಯ_filename_except_for_dash(ಪೂರ್ವಪ್ರತ್ಯಯ, argv[0]);
	ಹಿಂತಿರುಗಿ argc;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_bundle_create(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			     ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ ಪ್ರಗತಿ = isatty(STDERR_FILENO);
	ಪೂರ್ಣಾಂಕ ಆವೃತ್ತಿ = -1;
	ರಚನೆ option options[] = {
		OPT_NEGBIT('q', "quiet", &ಪ್ರಗತಿ,
			   N_("do not show progress meter"), 1),
		OPT_ಬಿಟ್(0, "progress", &ಪ್ರಗತಿ,
			N_("show progress meter"), 1),
		OPT_ಬಿಟ್_F(0, "all-progress", &ಪ್ರಗತಿ,
			  N_("historical; same as --progress"), 1,
			  PARSE_OPT_HIDDEN),
		OPT_NOOP_NOARG(0, "all-progress-implied"),
		OPT_INTEGER(0, "version", &ಆವೃತ್ತಿ,
			    N_("specify bundle format version")),
		OPT_ಕೊನೆ()
	};
	ಅಕ್ಷರ *bundle_file;
	ಪೂರ್ಣಾಂಕ ret;

	argc = parse_options_cmd_bundle(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			builtin_bundle_create_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ಆದರೆ (!startup_info->have_repository)
		die(_("Need a repository to create a bundle."));
	ret = !!create_bundle(the_repository, bundle_file, argc, argv, ಆವೃತ್ತಿ, ಪ್ರಗತಿ);
	ಬಿಡುಗಡೆ(bundle_file);
	ಹಿಂತಿರುಗಿ ret;
}

/*
 * Similar to read_bundle_header(), but handle "-" as stdin.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ open_bundle(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ರಚನೆ bundle_ಶೀರ್ಷಿಕೆ *ಶೀರ್ಷಿಕೆ,
		       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **name)
{
	ಆದರೆ (!strcmp(ಮಾರ್ಗ, "-")) {
		ಆದರೆ (name)
			*name = "<stdin>";
		ಹಿಂತಿರುಗಿ read_bundle_ಶೀರ್ಷಿಕೆ_fd(0, ಶೀರ್ಷಿಕೆ, "<stdin>");
	}

	ಆದರೆ (name)
		*name = ಮಾರ್ಗ;
	ಹಿಂತಿರುಗಿ read_bundle_ಶೀರ್ಷಿಕೆ(ಮಾರ್ಗ, ಶೀರ್ಷಿಕೆ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_bundle_verify(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			     ರಚನೆ repository *repo ಬಳಸದ) {
	ರಚನೆ bundle_ಶೀರ್ಷಿಕೆ ಶೀರ್ಷಿಕೆ = BUNDLE_ಶೀರ್ಷಿಕೆ_INIT;
	ಪೂರ್ಣಾಂಕ bundle_fd = -1;
	ಪೂರ್ಣಾಂಕ quiet = 0;
	ಪೂರ್ಣಾಂಕ ret;
	ರಚನೆ option options[] = {
		OPT_BOOL('q', "quiet", &quiet,
			    N_("do not show bundle details")),
		OPT_ಕೊನೆ()
	};
	ಅಕ್ಷರ *bundle_file;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;

	argc = parse_options_cmd_bundle(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			builtin_bundle_verify_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ಆದರೆ (!startup_info->have_repository) {
		ret = error(_("need a repository to verify a bundle"));
		ಹೋಗು cleanup;
	}

	ಆದರೆ ((bundle_fd = open_bundle(bundle_file, &ಶೀರ್ಷಿಕೆ, &name)) < 0) {
		ret = 1;
		ಹೋಗು cleanup;
	}
	close(bundle_fd);
	ಆದರೆ (verify_bundle(the_repository, &ಶೀರ್ಷಿಕೆ,
			  quiet ? VERIFY_BUNDLE_QUIET : VERIFY_BUNDLE_VERBOSE)) {
		ret = 1;
		ಹೋಗು cleanup;
	}

	fprintf(stderr, _("%s is okay\n"), name);
	ret = 0;
cleanup:
	ಬಿಡುಗಡೆ(bundle_file);
	bundle_ಶೀರ್ಷಿಕೆ_release(&ಶೀರ್ಷಿಕೆ);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_bundle_list_heads(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
				 ರಚನೆ repository *repo ಬಳಸದ) {
	ರಚನೆ bundle_ಶೀರ್ಷಿಕೆ ಶೀರ್ಷಿಕೆ = BUNDLE_ಶೀರ್ಷಿಕೆ_INIT;
	ಪೂರ್ಣಾಂಕ bundle_fd = -1;
	ಪೂರ್ಣಾಂಕ ret;
	ರಚನೆ option options[] = {
		OPT_ಕೊನೆ()
	};
	ಅಕ್ಷರ *bundle_file;

	argc = parse_options_cmd_bundle(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			builtin_bundle_list_heads_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ಆದರೆ ((bundle_fd = open_bundle(bundle_file, &ಶೀರ್ಷಿಕೆ, ಖಾಲಿ)) < 0) {
		ret = 1;
		ಹೋಗು cleanup;
	}
	close(bundle_fd);
	ret = !!list_bundle_refs(&ಶೀರ್ಷಿಕೆ, argc, argv);
cleanup:
	ಬಿಡುಗಡೆ(bundle_file);
	bundle_ಶೀರ್ಷಿಕೆ_release(&ಶೀರ್ಷಿಕೆ);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cmd_bundle_unbundle(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			       ರಚನೆ repository *repo ಬಳಸದ) {
	ರಚನೆ bundle_ಶೀರ್ಷಿಕೆ ಶೀರ್ಷಿಕೆ = BUNDLE_ಶೀರ್ಷಿಕೆ_INIT;
	ಪೂರ್ಣಾಂಕ bundle_fd = -1;
	ಪೂರ್ಣಾಂಕ ret;
	ಪೂರ್ಣಾಂಕ ಪ್ರಗತಿ = isatty(2);

	ರಚನೆ option options[] = {
		OPT_BOOL(0, "progress", &ಪ್ರಗತಿ,
			 N_("show progress meter")),
		OPT_ಕೊನೆ()
	};
	ಅಕ್ಷರ *bundle_file;
	ರಚನೆ strvec extra_ಸೂಚ್ಯಂಕ_pack_ವಾದಗಳು = STRVEC_INIT;

	argc = parse_options_cmd_bundle(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ,
			builtin_bundle_unbundle_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ಆದರೆ (!startup_info->have_repository)
		die(_("Need a repository to unbundle."));

	ಆದರೆ ((bundle_fd = open_bundle(bundle_file, &ಶೀರ್ಷಿಕೆ, ಖಾಲಿ)) < 0) {
		ret = 1;
		ಹೋಗು cleanup;
	}
	ಆದರೆ (ಪ್ರಗತಿ)
		strvec_pushl(&extra_ಸೂಚ್ಯಂಕ_pack_ವಾದಗಳು, "-v", "--progress-title",
			     _("Unbundling objects"), ಖಾಲಿ);
	ret = !!unbundle(the_repository, &ಶೀರ್ಷಿಕೆ, bundle_fd,
			 &extra_ಸೂಚ್ಯಂಕ_pack_ವಾದಗಳು, ಖಾಲಿ) ||
		list_bundle_refs(&ಶೀರ್ಷಿಕೆ, argc, argv);
	bundle_ಶೀರ್ಷಿಕೆ_release(&ಶೀರ್ಷಿಕೆ);

cleanup:
	strvec_clear(&extra_ಸೂಚ್ಯಂಕ_pack_ವಾದಗಳು);
	ಬಿಡುಗಡೆ(bundle_file);
	ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ cmd_bundle(ಪೂರ್ಣಾಂಕ argc,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	       ರಚನೆ repository *repo)
{
	parse_opt_subcommand_fn *fn = ಖಾಲಿ;
	ರಚನೆ option options[] = {
		OPT_SUBCOMMAND("create", &fn, cmd_bundle_create),
		OPT_SUBCOMMAND("verify", &fn, cmd_bundle_verify),
		OPT_SUBCOMMAND("list-heads", &fn, cmd_bundle_list_heads),
		OPT_SUBCOMMAND("unbundle", &fn, cmd_bundle_unbundle),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, builtin_bundle_usage,
			     0);

	packet_trace_ತಟಸ್ಥ("bundle");

	ಹಿಂತಿರುಗಿ !!fn(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, repo);
}
