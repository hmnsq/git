#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "abspath.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "strvec.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "pkt-line.h"
#അന്തര്ഭാവ "bundle.h"

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

സ്ഥിതം അക്ഷരം സ്ഥിരം * സ്ഥിരം builtin_bundle_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	BUILTIN_BUNDLE_VERIFY_USAGE,
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	ശൂന്യ,
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_bundle_create_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_bundle_verify_usage[] = {
	BUILTIN_BUNDLE_VERIFY_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_bundle_list_heads_usage[] = {
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം builtin_bundle_unbundle_usage[] = {
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	ശൂന്യ
};

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_options_cmd_bundle(പൂർണ്ണസംഖ്യ argc,
		സ്ഥിരം അക്ഷരം **argv,
		സ്ഥിരം അക്ഷരം* prefix,
		സ്ഥിരം അക്ഷരം * സ്ഥിരം usagestr[],
		സ്ഥിരം ഘടന option options[],
		അക്ഷരം **bundle_file) {
	argc = parse_options(argc, argv, ശൂന്യ, options, usagestr,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	എങ്കിൽ (!argc)
		usage_സന്ദേശം_opt(_("need a <file> argument"), usagestr, options);
	*bundle_file = prefix_filename_except_for_dash(prefix, argv[0]);
	തിരികെ argc;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_bundle_create(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			     ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ progress = isatty(STDERR_FILENO);
	പൂർണ്ണസംഖ്യ version = -1;
	ഘടന option options[] = {
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
	അക്ഷരം *bundle_file;
	പൂർണ്ണസംഖ്യ ret;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_create_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	എങ്കിൽ (!startup_info->have_repository)
		die(_("Need a repository to create a bundle."));
	ret = !!create_bundle(the_repository, bundle_file, argc, argv, version, progress);
	സ്വതന്ത്രം(bundle_file);
	തിരികെ ret;
}

/*
 * Similar to read_bundle_header(), but handle "-" as stdin.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ open_bundle(സ്ഥിരം അക്ഷരം *path, ഘടന bundle_header *header,
		       സ്ഥിരം അക്ഷരം **name)
{
	എങ്കിൽ (!strcmp(path, "-")) {
		എങ്കിൽ (name)
			*name = "<stdin>";
		തിരികെ read_bundle_header_fd(0, header, "<stdin>");
	}

	എങ്കിൽ (name)
		*name = path;
	തിരികെ read_bundle_header(path, header);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_bundle_verify(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			     ഘടന repository *repo UNUSED) {
	ഘടന bundle_header header = BUNDLE_HEADER_INIT;
	പൂർണ്ണസംഖ്യ bundle_fd = -1;
	പൂർണ്ണസംഖ്യ quiet = 0;
	പൂർണ്ണസംഖ്യ ret;
	ഘടന option options[] = {
		OPT_BOOL('q', "quiet", &quiet,
			    N_("do not show bundle details")),
		OPT_END()
	};
	അക്ഷരം *bundle_file;
	സ്ഥിരം അക്ഷരം *name;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_verify_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	എങ്കിൽ (!startup_info->have_repository) {
		ret = error(_("need a repository to verify a bundle"));
		പോകുക cleanup;
	}

	എങ്കിൽ ((bundle_fd = open_bundle(bundle_file, &header, &name)) < 0) {
		ret = 1;
		പോകുക cleanup;
	}
	close(bundle_fd);
	എങ്കിൽ (verify_bundle(the_repository, &header,
			  quiet ? VERIFY_BUNDLE_QUIET : VERIFY_BUNDLE_VERBOSE)) {
		ret = 1;
		പോകുക cleanup;
	}

	fprintf(stderr, _("%s is okay\n"), name);
	ret = 0;
cleanup:
	സ്വതന്ത്രം(bundle_file);
	bundle_header_release(&header);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_bundle_list_heads(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
				 ഘടന repository *repo UNUSED) {
	ഘടന bundle_header header = BUNDLE_HEADER_INIT;
	പൂർണ്ണസംഖ്യ bundle_fd = -1;
	പൂർണ്ണസംഖ്യ ret;
	ഘടന option options[] = {
		OPT_END()
	};
	അക്ഷരം *bundle_file;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_list_heads_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	എങ്കിൽ ((bundle_fd = open_bundle(bundle_file, &header, ശൂന്യ)) < 0) {
		ret = 1;
		പോകുക cleanup;
	}
	close(bundle_fd);
	ret = !!list_bundle_refs(&header, argc, argv);
cleanup:
	സ്വതന്ത്രം(bundle_file);
	bundle_header_release(&header);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cmd_bundle_unbundle(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
			       ഘടന repository *repo UNUSED) {
	ഘടന bundle_header header = BUNDLE_HEADER_INIT;
	പൂർണ്ണസംഖ്യ bundle_fd = -1;
	പൂർണ്ണസംഖ്യ ret;
	പൂർണ്ണസംഖ്യ progress = isatty(2);

	ഘടന option options[] = {
		OPT_BOOL(0, "progress", &progress,
			 N_("show progress meter")),
		OPT_END()
	};
	അക്ഷരം *bundle_file;
	ഘടന strvec extra_index_pack_args = STRVEC_INIT;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_unbundle_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	എങ്കിൽ (!startup_info->have_repository)
		die(_("Need a repository to unbundle."));

	എങ്കിൽ ((bundle_fd = open_bundle(bundle_file, &header, ശൂന്യ)) < 0) {
		ret = 1;
		പോകുക cleanup;
	}
	എങ്കിൽ (progress)
		strvec_pushl(&extra_index_pack_args, "-v", "--progress-title",
			     _("Unbundling objects"), ശൂന്യ);
	ret = !!unbundle(the_repository, &header, bundle_fd,
			 &extra_index_pack_args, ശൂന്യ) ||
		list_bundle_refs(&header, argc, argv);
	bundle_header_release(&header);

cleanup:
	strvec_clear(&extra_index_pack_args);
	സ്വതന്ത്രം(bundle_file);
	തിരികെ ret;
}

പൂർണ്ണസംഖ്യ cmd_bundle(പൂർണ്ണസംഖ്യ argc,
	       സ്ഥിരം അക്ഷരം **argv,
	       സ്ഥിരം അക്ഷരം *prefix,
	       ഘടന repository *repo)
{
	parse_opt_subcommand_fn *fn = ശൂന്യ;
	ഘടന option options[] = {
		OPT_SUBCOMMAND("create", &fn, cmd_bundle_create),
		OPT_SUBCOMMAND("verify", &fn, cmd_bundle_verify),
		OPT_SUBCOMMAND("list-heads", &fn, cmd_bundle_list_heads),
		OPT_SUBCOMMAND("unbundle", &fn, cmd_bundle_unbundle),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, builtin_bundle_usage,
			     0);

	packet_trace_identity("bundle");

	തിരികെ !!fn(argc, argv, prefix, repo);
}
