#परिभाषय USE_THE_REPOSITORY_VARIABLE
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

#परिभाषय BUILTIN_BUNDLE_CREATE_USAGE \
	N_("git bundle create [-q | --quiet | --progress]\n" \
	   "                  [--version=<version>] <file> <git-rev-list-args>")
#परिभाषय BUILTIN_BUNDLE_VERIFY_USAGE \
	N_("git bundle verify [-q | --quiet] <file>")
#परिभाषय BUILTIN_BUNDLE_LIST_HEADS_USAGE \
	N_("git bundle list-heads <file> [<refname>...]")
#परिभाषय BUILTIN_BUNDLE_UNBUNDLE_USAGE \
	N_("git bundle unbundle [--progress] <file> [<refname>...]")

स्थिर वर्ण ध्रुव * ध्रुव builtin_bundle_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	BUILTIN_BUNDLE_VERIFY_USAGE,
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	रिक्त,
};

स्थिर ध्रुव वर्ण * ध्रुव builtin_bundle_create_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव builtin_bundle_verify_usage[] = {
	BUILTIN_BUNDLE_VERIFY_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव builtin_bundle_list_heads_usage[] = {
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव builtin_bundle_unbundle_usage[] = {
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	रिक्त
};

स्थिर पूर्णाङ्क parse_options_cmd_bundle(पूर्णाङ्क argc,
		ध्रुव वर्ण **argv,
		ध्रुव वर्ण* उपसर्गः,
		ध्रुव वर्ण * ध्रुव usagestr[],
		ध्रुव संरचना option options[],
		वर्ण **bundle_file) {
	argc = parse_options(argc, argv, रिक्त, options, usagestr,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	यदि (!argc)
		usage_सन्देशः_opt(_("need a <file> argument"), usagestr, options);
	*bundle_file = उपसर्गः_filename_except_for_dash(उपसर्गः, argv[0]);
	प्रत्यागम argc;
}

स्थिर पूर्णाङ्क cmd_bundle_create(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			     संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क प्रगतिः = isatty(STDERR_FILENO);
	पूर्णाङ्क संस्करणम् = -1;
	संरचना option options[] = {
		OPT_NEGBIT('q', "quiet", &प्रगतिः,
			   N_("do not show progress meter"), 1),
		OPT_BIT(0, "progress", &प्रगतिः,
			N_("show progress meter"), 1),
		OPT_BIT_F(0, "all-progress", &प्रगतिः,
			  N_("historical; same as --progress"), 1,
			  PARSE_OPT_HIDDEN),
		OPT_NOOP_NOARG(0, "all-progress-implied"),
		OPT_INTEGER(0, "version", &संस्करणम्,
			    N_("specify bundle format version")),
		OPT_अन्तः()
	};
	वर्ण *bundle_file;
	पूर्णाङ्क ret;

	argc = parse_options_cmd_bundle(argc, argv, उपसर्गः,
			builtin_bundle_create_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	यदि (!startup_info->have_repository)
		die(_("Need a repository to create a bundle."));
	ret = !!create_bundle(the_repository, bundle_file, argc, argv, संस्करणम्, प्रगतिः);
	मुक्त(bundle_file);
	प्रत्यागम ret;
}

/*
 * Similar to read_bundle_header(), but handle "-" as stdin.
 */
स्थिर पूर्णाङ्क open_bundle(ध्रुव वर्ण *मार्गः, संरचना bundle_शीर्षकम् *शीर्षकम्,
		       ध्रुव वर्ण **name)
{
	यदि (!वाक्यं_तुलय(मार्गः, "-")) {
		यदि (name)
			*name = "<stdin>";
		प्रत्यागम read_bundle_शीर्षकम्_fd(0, शीर्षकम्, "<stdin>");
	}

	यदि (name)
		*name = मार्गः;
	प्रत्यागम read_bundle_शीर्षकम्(मार्गः, शीर्षकम्);
}

स्थिर पूर्णाङ्क cmd_bundle_verify(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			     संरचना repository *repo अप्रयुक्तम्) {
	संरचना bundle_शीर्षकम् शीर्षकम् = BUNDLE_शीर्षकम्_INIT;
	पूर्णाङ्क bundle_fd = -1;
	पूर्णाङ्क quiet = 0;
	पूर्णाङ्क ret;
	संरचना option options[] = {
		OPT_BOOL('q', "quiet", &quiet,
			    N_("do not show bundle details")),
		OPT_अन्तः()
	};
	वर्ण *bundle_file;
	ध्रुव वर्ण *name;

	argc = parse_options_cmd_bundle(argc, argv, उपसर्गः,
			builtin_bundle_verify_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	यदि (!startup_info->have_repository) {
		ret = error(_("need a repository to verify a bundle"));
		गच्छ cleanup;
	}

	यदि ((bundle_fd = open_bundle(bundle_file, &शीर्षकम्, &name)) < 0) {
		ret = 1;
		गच्छ cleanup;
	}
	close(bundle_fd);
	यदि (verify_bundle(the_repository, &शीर्षकम्,
			  quiet ? VERIFY_BUNDLE_QUIET : VERIFY_BUNDLE_VERBOSE)) {
		ret = 1;
		गच्छ cleanup;
	}

	fprintf(stderr, _("%s is okay\n"), name);
	ret = 0;
cleanup:
	मुक्त(bundle_file);
	bundle_शीर्षकम्_release(&शीर्षकम्);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क cmd_bundle_list_heads(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
				 संरचना repository *repo अप्रयुक्तम्) {
	संरचना bundle_शीर्षकम् शीर्षकम् = BUNDLE_शीर्षकम्_INIT;
	पूर्णाङ्क bundle_fd = -1;
	पूर्णाङ्क ret;
	संरचना option options[] = {
		OPT_अन्तः()
	};
	वर्ण *bundle_file;

	argc = parse_options_cmd_bundle(argc, argv, उपसर्गः,
			builtin_bundle_list_heads_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	यदि ((bundle_fd = open_bundle(bundle_file, &शीर्षकम्, रिक्त)) < 0) {
		ret = 1;
		गच्छ cleanup;
	}
	close(bundle_fd);
	ret = !!list_bundle_refs(&शीर्षकम्, argc, argv);
cleanup:
	मुक्त(bundle_file);
	bundle_शीर्षकम्_release(&शीर्षकम्);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क cmd_bundle_unbundle(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			       संरचना repository *repo अप्रयुक्तम्) {
	संरचना bundle_शीर्षकम् शीर्षकम् = BUNDLE_शीर्षकम्_INIT;
	पूर्णाङ्क bundle_fd = -1;
	पूर्णाङ्क ret;
	पूर्णाङ्क प्रगतिः = isatty(2);

	संरचना option options[] = {
		OPT_BOOL(0, "progress", &प्रगतिः,
			 N_("show progress meter")),
		OPT_अन्तः()
	};
	वर्ण *bundle_file;
	संरचना strvec extra_सूचकांक_pack_तर्काः = STRVEC_INIT;

	argc = parse_options_cmd_bundle(argc, argv, उपसर्गः,
			builtin_bundle_unbundle_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	यदि (!startup_info->have_repository)
		die(_("Need a repository to unbundle."));

	यदि ((bundle_fd = open_bundle(bundle_file, &शीर्षकम्, रिक्त)) < 0) {
		ret = 1;
		गच्छ cleanup;
	}
	यदि (प्रगतिः)
		strvec_pushl(&extra_सूचकांक_pack_तर्काः, "-v", "--progress-title",
			     _("Unbundling objects"), रिक्त);
	ret = !!unbundle(the_repository, &शीर्षकम्, bundle_fd,
			 &extra_सूचकांक_pack_तर्काः, रिक्त) ||
		list_bundle_refs(&शीर्षकम्, argc, argv);
	bundle_शीर्षकम्_release(&शीर्षकम्);

cleanup:
	strvec_clear(&extra_सूचकांक_pack_तर्काः);
	मुक्त(bundle_file);
	प्रत्यागम ret;
}

पूर्णाङ्क cmd_bundle(पूर्णाङ्क argc,
	       ध्रुव वर्ण **argv,
	       ध्रुव वर्ण *उपसर्गः,
	       संरचना repository *repo)
{
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option options[] = {
		OPT_SUBCOMMAND("create", &fn, cmd_bundle_create),
		OPT_SUBCOMMAND("verify", &fn, cmd_bundle_verify),
		OPT_SUBCOMMAND("list-heads", &fn, cmd_bundle_list_heads),
		OPT_SUBCOMMAND("unbundle", &fn, cmd_bundle_unbundle),
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, options, builtin_bundle_usage,
			     0);

	packet_trace_तटस्थः("bundle");

	प्रत्यागम !!fn(argc, argv, उपसर्गः, repo);
}
