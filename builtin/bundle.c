#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "abspath.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "setup.h"
#অন্তর্ভাব "strvec.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "pkt-line.h"
#অন্তর্ভাব "bundle.h"

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

স্থির অক্ষর ধ্রুবক * ধ্রুবক builtin_bundle_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	BUILTIN_BUNDLE_VERIFY_USAGE,
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	ফাঁকা,
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক builtin_bundle_create_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক builtin_bundle_verify_usage[] = {
	BUILTIN_BUNDLE_VERIFY_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক builtin_bundle_list_heads_usage[] = {
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর * ধ্রুবক builtin_bundle_unbundle_usage[] = {
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	ফাঁকা
};

স্থির পূর্ণসংখ্যা parse_options_cmd_bundle(পূর্ণসংখ্যা argc,
		ধ্রুবক অক্ষর **argv,
		ধ্রুবক অক্ষর* prefix,
		ধ্রুবক অক্ষর * ধ্রুবক usagestr[],
		ধ্রুবক কাঠামো option options[],
		অক্ষর **bundle_file) {
	argc = parse_options(argc, argv, ফাঁকা, options, usagestr,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	যদি (!argc)
		usage_বার্তা_opt(_("need a <file> argument"), usagestr, options);
	*bundle_file = prefix_filename_except_for_dash(prefix, argv[0]);
	ফেরত argc;
}

স্থির পূর্ণসংখ্যা cmd_bundle_create(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			     কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা progress = isatty(STDERR_FILENO);
	পূর্ণসংখ্যা version = -1;
	কাঠামো option options[] = {
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
	অক্ষর *bundle_file;
	পূর্ণসংখ্যা ret;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_create_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	যদি (!startup_info->have_repository)
		die(_("Need a repository to create a bundle."));
	ret = !!create_bundle(the_repository, bundle_file, argc, argv, version, progress);
	মুক্ত(bundle_file);
	ফেরত ret;
}

/*
 * Similar to read_bundle_header(), but handle "-" as stdin.
 */
স্থির পূর্ণসংখ্যা open_bundle(ধ্রুবক অক্ষর *path, কাঠামো bundle_header *header,
		       ধ্রুবক অক্ষর **name)
{
	যদি (!strcmp(path, "-")) {
		যদি (name)
			*name = "<stdin>";
		ফেরত read_bundle_header_fd(0, header, "<stdin>");
	}

	যদি (name)
		*name = path;
	ফেরত read_bundle_header(path, header);
}

স্থির পূর্ণসংখ্যা cmd_bundle_verify(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			     কাঠামো repository *repo UNUSED) {
	কাঠামো bundle_header header = BUNDLE_HEADER_INIT;
	পূর্ণসংখ্যা bundle_fd = -1;
	পূর্ণসংখ্যা quiet = 0;
	পূর্ণসংখ্যা ret;
	কাঠামো option options[] = {
		OPT_BOOL('q', "quiet", &quiet,
			    N_("do not show bundle details")),
		OPT_END()
	};
	অক্ষর *bundle_file;
	ধ্রুবক অক্ষর *name;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_verify_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	যদি (!startup_info->have_repository) {
		ret = error(_("need a repository to verify a bundle"));
		যাও cleanup;
	}

	যদি ((bundle_fd = open_bundle(bundle_file, &header, &name)) < 0) {
		ret = 1;
		যাও cleanup;
	}
	close(bundle_fd);
	যদি (verify_bundle(the_repository, &header,
			  quiet ? VERIFY_BUNDLE_QUIET : VERIFY_BUNDLE_VERBOSE)) {
		ret = 1;
		যাও cleanup;
	}

	fprintf(stderr, _("%s is okay\n"), name);
	ret = 0;
cleanup:
	মুক্ত(bundle_file);
	bundle_header_release(&header);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা cmd_bundle_list_heads(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
				 কাঠামো repository *repo UNUSED) {
	কাঠামো bundle_header header = BUNDLE_HEADER_INIT;
	পূর্ণসংখ্যা bundle_fd = -1;
	পূর্ণসংখ্যা ret;
	কাঠামো option options[] = {
		OPT_END()
	};
	অক্ষর *bundle_file;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_list_heads_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	যদি ((bundle_fd = open_bundle(bundle_file, &header, ফাঁকা)) < 0) {
		ret = 1;
		যাও cleanup;
	}
	close(bundle_fd);
	ret = !!list_bundle_refs(&header, argc, argv);
cleanup:
	মুক্ত(bundle_file);
	bundle_header_release(&header);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা cmd_bundle_unbundle(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, ধ্রুবক অক্ষর *prefix,
			       কাঠামো repository *repo UNUSED) {
	কাঠামো bundle_header header = BUNDLE_HEADER_INIT;
	পূর্ণসংখ্যা bundle_fd = -1;
	পূর্ণসংখ্যা ret;
	পূর্ণসংখ্যা progress = isatty(2);

	কাঠামো option options[] = {
		OPT_BOOL(0, "progress", &progress,
			 N_("show progress meter")),
		OPT_END()
	};
	অক্ষর *bundle_file;
	কাঠামো strvec extra_সূচক_pack_args = STRVEC_INIT;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_unbundle_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	যদি (!startup_info->have_repository)
		die(_("Need a repository to unbundle."));

	যদি ((bundle_fd = open_bundle(bundle_file, &header, ফাঁকা)) < 0) {
		ret = 1;
		যাও cleanup;
	}
	যদি (progress)
		strvec_pushl(&extra_সূচক_pack_args, "-v", "--progress-title",
			     _("Unbundling objects"), ফাঁকা);
	ret = !!unbundle(the_repository, &header, bundle_fd,
			 &extra_সূচক_pack_args, ফাঁকা) ||
		list_bundle_refs(&header, argc, argv);
	bundle_header_release(&header);

cleanup:
	strvec_clear(&extra_সূচক_pack_args);
	মুক্ত(bundle_file);
	ফেরত ret;
}

পূর্ণসংখ্যা cmd_bundle(পূর্ণসংখ্যা argc,
	       ধ্রুবক অক্ষর **argv,
	       ধ্রুবক অক্ষর *prefix,
	       কাঠামো repository *repo)
{
	parse_opt_subcommand_fn *fn = ফাঁকা;
	কাঠামো option options[] = {
		OPT_SUBCOMMAND("create", &fn, cmd_bundle_create),
		OPT_SUBCOMMAND("verify", &fn, cmd_bundle_verify),
		OPT_SUBCOMMAND("list-heads", &fn, cmd_bundle_list_heads),
		OPT_SUBCOMMAND("unbundle", &fn, cmd_bundle_unbundle),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, builtin_bundle_usage,
			     0);

	packet_trace_identity("bundle");

	ফেরত !!fn(argc, argv, prefix, repo);
}
