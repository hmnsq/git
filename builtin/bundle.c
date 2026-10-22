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

স্থির বর্ণ ধ্রুব * ধ্রুব builtin_bundle_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	BUILTIN_BUNDLE_VERIFY_USAGE,
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	রিক্ত,
};

স্থির ধ্রুব বর্ণ * ধ্রুব builtin_bundle_create_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব builtin_bundle_verify_usage[] = {
	BUILTIN_BUNDLE_VERIFY_USAGE,
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব builtin_bundle_list_heads_usage[] = {
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব builtin_bundle_unbundle_usage[] = {
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	রিক্ত
};

স্থির পূর্ণাঙ্ক parse_options_cmd_bundle(পূর্ণাঙ্ক argc,
		ধ্রুব বর্ণ **argv,
		ধ্রুব বর্ণ* prefix,
		ধ্রুব বর্ণ * ধ্রুব usagestr[],
		ধ্রুব সংরচনা option options[],
		বর্ণ **bundle_file) {
	argc = parse_options(argc, argv, রিক্ত, options, usagestr,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	যদি (!argc)
		usage_বার্তা_opt(_("need a <file> argument"), usagestr, options);
	*bundle_file = prefix_filename_except_for_dash(prefix, argv[0]);
	প্রত্যাগম argc;
}

স্থির পূর্ণাঙ্ক cmd_bundle_create(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
			     সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক progress = isatty(STDERR_FILENO);
	পূর্ণাঙ্ক version = -1;
	সংরচনা option options[] = {
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
	বর্ণ *bundle_file;
	পূর্ণাঙ্ক ret;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_create_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	যদি (!startup_info->have_repository)
		die(_("Need a repository to create a bundle."));
	ret = !!create_bundle(the_repository, bundle_file, argc, argv, version, progress);
	মুক্ত(bundle_file);
	প্রত্যাগম ret;
}

/*
 * Similar to read_bundle_header(), but handle "-" as stdin.
 */
স্থির পূর্ণাঙ্ক open_bundle(ধ্রুব বর্ণ *path, সংরচনা bundle_header *header,
		       ধ্রুব বর্ণ **name)
{
	যদি (!strcmp(path, "-")) {
		যদি (name)
			*name = "<stdin>";
		প্রত্যাগম read_bundle_header_fd(0, header, "<stdin>");
	}

	যদি (name)
		*name = path;
	প্রত্যাগম read_bundle_header(path, header);
}

স্থির পূর্ণাঙ্ক cmd_bundle_verify(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
			     সংরচনা repository *repo UNUSED) {
	সংরচনা bundle_header header = BUNDLE_HEADER_INIT;
	পূর্ণাঙ্ক bundle_fd = -1;
	পূর্ণাঙ্ক quiet = 0;
	পূর্ণাঙ্ক ret;
	সংরচনা option options[] = {
		OPT_BOOL('q', "quiet", &quiet,
			    N_("do not show bundle details")),
		OPT_END()
	};
	বর্ণ *bundle_file;
	ধ্রুব বর্ণ *name;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_verify_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	যদি (!startup_info->have_repository) {
		ret = error(_("need a repository to verify a bundle"));
		গচ্ছ cleanup;
	}

	যদি ((bundle_fd = open_bundle(bundle_file, &header, &name)) < 0) {
		ret = 1;
		গচ্ছ cleanup;
	}
	close(bundle_fd);
	যদি (verify_bundle(the_repository, &header,
			  quiet ? VERIFY_BUNDLE_QUIET : VERIFY_BUNDLE_VERBOSE)) {
		ret = 1;
		গচ্ছ cleanup;
	}

	fprintf(stderr, _("%s is okay\n"), name);
	ret = 0;
cleanup:
	মুক্ত(bundle_file);
	bundle_header_release(&header);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক cmd_bundle_list_heads(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
				 সংরচনা repository *repo UNUSED) {
	সংরচনা bundle_header header = BUNDLE_HEADER_INIT;
	পূর্ণাঙ্ক bundle_fd = -1;
	পূর্ণাঙ্ক ret;
	সংরচনা option options[] = {
		OPT_END()
	};
	বর্ণ *bundle_file;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_list_heads_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	যদি ((bundle_fd = open_bundle(bundle_file, &header, রিক্ত)) < 0) {
		ret = 1;
		গচ্ছ cleanup;
	}
	close(bundle_fd);
	ret = !!list_bundle_refs(&header, argc, argv);
cleanup:
	মুক্ত(bundle_file);
	bundle_header_release(&header);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক cmd_bundle_unbundle(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
			       সংরচনা repository *repo UNUSED) {
	সংরচনা bundle_header header = BUNDLE_HEADER_INIT;
	পূর্ণাঙ্ক bundle_fd = -1;
	পূর্ণাঙ্ক ret;
	পূর্ণাঙ্ক progress = isatty(2);

	সংরচনা option options[] = {
		OPT_BOOL(0, "progress", &progress,
			 N_("show progress meter")),
		OPT_END()
	};
	বর্ণ *bundle_file;
	সংরচনা strvec extra_index_pack_args = STRVEC_INIT;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_unbundle_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	যদি (!startup_info->have_repository)
		die(_("Need a repository to unbundle."));

	যদি ((bundle_fd = open_bundle(bundle_file, &header, রিক্ত)) < 0) {
		ret = 1;
		গচ্ছ cleanup;
	}
	যদি (progress)
		strvec_pushl(&extra_index_pack_args, "-v", "--progress-title",
			     _("Unbundling objects"), রিক্ত);
	ret = !!unbundle(the_repository, &header, bundle_fd,
			 &extra_index_pack_args, রিক্ত) ||
		list_bundle_refs(&header, argc, argv);
	bundle_header_release(&header);

cleanup:
	strvec_clear(&extra_index_pack_args);
	মুক্ত(bundle_file);
	প্রত্যাগম ret;
}

পূর্ণাঙ্ক cmd_bundle(পূর্ণাঙ্ক argc,
	       ধ্রুব বর্ণ **argv,
	       ধ্রুব বর্ণ *prefix,
	       সংরচনা repository *repo)
{
	parse_opt_subcommand_fn *fn = রিক্ত;
	সংরচনা option options[] = {
		OPT_SUBCOMMAND("create", &fn, cmd_bundle_create),
		OPT_SUBCOMMAND("verify", &fn, cmd_bundle_verify),
		OPT_SUBCOMMAND("list-heads", &fn, cmd_bundle_list_heads),
		OPT_SUBCOMMAND("unbundle", &fn, cmd_bundle_unbundle),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, builtin_bundle_usage,
			     0);

	packet_trace_identity("bundle");

	প্রত্যাগম !!fn(argc, argv, prefix, repo);
}
