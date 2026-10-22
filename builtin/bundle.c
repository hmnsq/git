#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "abspath.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "strvec.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "pkt-line.h"
#అన్తర్భావ "bundle.h"

/*
 * Basic handler for bundle files to connect repositories via sneakernet.
 * Invocation must include action.
 * This function can create a bundle or provide information on an existing
 * bundle supporting "fetch", "pull", and "ls-remote".
 */

#నిర్వచించు BUILTIN_BUNDLE_CREATE_USAGE \
	N_("git bundle create [-q | --quiet | --progress]\n" \
	   "                  [--version=<version>] <file> <git-rev-list-args>")
#నిర్వచించు BUILTIN_BUNDLE_VERIFY_USAGE \
	N_("git bundle verify [-q | --quiet] <file>")
#నిర్వచించు BUILTIN_BUNDLE_LIST_HEADS_USAGE \
	N_("git bundle list-heads <file> [<refname>...]")
#నిర్వచించు BUILTIN_BUNDLE_UNBUNDLE_USAGE \
	N_("git bundle unbundle [--progress] <file> [<refname>...]")

స్థిర అక్షరం స్థిరరాశి * స్థిరరాశి builtin_bundle_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	BUILTIN_BUNDLE_VERIFY_USAGE,
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	ఖాళీ,
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_bundle_create_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_bundle_verify_usage[] = {
	BUILTIN_BUNDLE_VERIFY_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_bundle_list_heads_usage[] = {
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి builtin_bundle_unbundle_usage[] = {
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	ఖాళీ
};

స్థిర పూర్ణాంకం parse_options_cmd_bundle(పూర్ణాంకం argc,
		స్థిరరాశి అక్షరం **argv,
		స్థిరరాశి అక్షరం* ఉపసర్గ,
		స్థిరరాశి అక్షరం * స్థిరరాశి usagestr[],
		స్థిరరాశి నిర్మాణం option options[],
		అక్షరం **bundle_file) {
	argc = parse_options(argc, argv, ఖాళీ, options, usagestr,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	ఒకవేళ (!argc)
		usage_సందేశం_opt(_("need a <file> argument"), usagestr, options);
	*bundle_file = ఉపసర్గ_filename_except_for_dash(ఉపసర్గ, argv[0]);
	తిరిగి argc;
}

స్థిర పూర్ణాంకం cmd_bundle_create(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			     నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం పురోగతి = isatty(STDERR_FILENO);
	పూర్ణాంకం సంస్కరణ = -1;
	నిర్మాణం option options[] = {
		OPT_NEGBIT('q', "quiet", &పురోగతి,
			   N_("do not show progress meter"), 1),
		OPT_బిట్టు(0, "progress", &పురోగతి,
			N_("show progress meter"), 1),
		OPT_బిట్టు_F(0, "all-progress", &పురోగతి,
			  N_("historical; same as --progress"), 1,
			  PARSE_OPT_HIDDEN),
		OPT_NOOP_NOARG(0, "all-progress-implied"),
		OPT_INTEGER(0, "version", &సంస్కరణ,
			    N_("specify bundle format version")),
		OPT_చివర()
	};
	అక్షరం *bundle_file;
	పూర్ణాంకం ret;

	argc = parse_options_cmd_bundle(argc, argv, ఉపసర్గ,
			builtin_bundle_create_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ఒకవేళ (!startup_info->have_repository)
		die(_("Need a repository to create a bundle."));
	ret = !!create_bundle(the_repository, bundle_file, argc, argv, సంస్కరణ, పురోగతి);
	విడుదల(bundle_file);
	తిరిగి ret;
}

/*
 * Similar to read_bundle_header(), but handle "-" as stdin.
 */
స్థిర పూర్ణాంకం open_bundle(స్థిరరాశి అక్షరం *మార్గం, నిర్మాణం bundle_శీర్షిక *శీర్షిక,
		       స్థిరరాశి అక్షరం **name)
{
	ఒకవేళ (!strcmp(మార్గం, "-")) {
		ఒకవేళ (name)
			*name = "<stdin>";
		తిరిగి read_bundle_శీర్షిక_fd(0, శీర్షిక, "<stdin>");
	}

	ఒకవేళ (name)
		*name = మార్గం;
	తిరిగి read_bundle_శీర్షిక(మార్గం, శీర్షిక);
}

స్థిర పూర్ణాంకం cmd_bundle_verify(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			     నిర్మాణం repository *repo వాడని) {
	నిర్మాణం bundle_శీర్షిక శీర్షిక = BUNDLE_శీర్షిక_INIT;
	పూర్ణాంకం bundle_fd = -1;
	పూర్ణాంకం quiet = 0;
	పూర్ణాంకం ret;
	నిర్మాణం option options[] = {
		OPT_BOOL('q', "quiet", &quiet,
			    N_("do not show bundle details")),
		OPT_చివర()
	};
	అక్షరం *bundle_file;
	స్థిరరాశి అక్షరం *name;

	argc = parse_options_cmd_bundle(argc, argv, ఉపసర్గ,
			builtin_bundle_verify_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ఒకవేళ (!startup_info->have_repository) {
		ret = error(_("need a repository to verify a bundle"));
		వెళ్ళు cleanup;
	}

	ఒకవేళ ((bundle_fd = open_bundle(bundle_file, &శీర్షిక, &name)) < 0) {
		ret = 1;
		వెళ్ళు cleanup;
	}
	close(bundle_fd);
	ఒకవేళ (verify_bundle(the_repository, &శీర్షిక,
			  quiet ? VERIFY_BUNDLE_QUIET : VERIFY_BUNDLE_VERBOSE)) {
		ret = 1;
		వెళ్ళు cleanup;
	}

	fprintf(stderr, _("%s is okay\n"), name);
	ret = 0;
cleanup:
	విడుదల(bundle_file);
	bundle_శీర్షిక_release(&శీర్షిక);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం cmd_bundle_list_heads(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
				 నిర్మాణం repository *repo వాడని) {
	నిర్మాణం bundle_శీర్షిక శీర్షిక = BUNDLE_శీర్షిక_INIT;
	పూర్ణాంకం bundle_fd = -1;
	పూర్ణాంకం ret;
	నిర్మాణం option options[] = {
		OPT_చివర()
	};
	అక్షరం *bundle_file;

	argc = parse_options_cmd_bundle(argc, argv, ఉపసర్గ,
			builtin_bundle_list_heads_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ఒకవేళ ((bundle_fd = open_bundle(bundle_file, &శీర్షిక, ఖాళీ)) < 0) {
		ret = 1;
		వెళ్ళు cleanup;
	}
	close(bundle_fd);
	ret = !!list_bundle_refs(&శీర్షిక, argc, argv);
cleanup:
	విడుదల(bundle_file);
	bundle_శీర్షిక_release(&శీర్షిక);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం cmd_bundle_unbundle(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
			       నిర్మాణం repository *repo వాడని) {
	నిర్మాణం bundle_శీర్షిక శీర్షిక = BUNDLE_శీర్షిక_INIT;
	పూర్ణాంకం bundle_fd = -1;
	పూర్ణాంకం ret;
	పూర్ణాంకం పురోగతి = isatty(2);

	నిర్మాణం option options[] = {
		OPT_BOOL(0, "progress", &పురోగతి,
			 N_("show progress meter")),
		OPT_చివర()
	};
	అక్షరం *bundle_file;
	నిర్మాణం strvec extra_సూచీ_pack_వాదనలు = STRVEC_INIT;

	argc = parse_options_cmd_bundle(argc, argv, ఉపసర్గ,
			builtin_bundle_unbundle_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ఒకవేళ (!startup_info->have_repository)
		die(_("Need a repository to unbundle."));

	ఒకవేళ ((bundle_fd = open_bundle(bundle_file, &శీర్షిక, ఖాళీ)) < 0) {
		ret = 1;
		వెళ్ళు cleanup;
	}
	ఒకవేళ (పురోగతి)
		strvec_pushl(&extra_సూచీ_pack_వాదనలు, "-v", "--progress-title",
			     _("Unbundling objects"), ఖాళీ);
	ret = !!unbundle(the_repository, &శీర్షిక, bundle_fd,
			 &extra_సూచీ_pack_వాదనలు, ఖాళీ) ||
		list_bundle_refs(&శీర్షిక, argc, argv);
	bundle_శీర్షిక_release(&శీర్షిక);

cleanup:
	strvec_clear(&extra_సూచీ_pack_వాదనలు);
	విడుదల(bundle_file);
	తిరిగి ret;
}

పూర్ణాంకం cmd_bundle(పూర్ణాంకం argc,
	       స్థిరరాశి అక్షరం **argv,
	       స్థిరరాశి అక్షరం *ఉపసర్గ,
	       నిర్మాణం repository *repo)
{
	parse_opt_subcommand_fn *fn = ఖాళీ;
	నిర్మాణం option options[] = {
		OPT_SUBCOMMAND("create", &fn, cmd_bundle_create),
		OPT_SUBCOMMAND("verify", &fn, cmd_bundle_verify),
		OPT_SUBCOMMAND("list-heads", &fn, cmd_bundle_list_heads),
		OPT_SUBCOMMAND("unbundle", &fn, cmd_bundle_unbundle),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options, builtin_bundle_usage,
			     0);

	packet_trace_తటస్థం("bundle");

	తిరిగి !!fn(argc, argv, ఉపసర్గ, repo);
}
