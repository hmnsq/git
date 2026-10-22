#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "abspath.h"
#شامل "gettext.h"
#شامل "setup.h"
#شامل "strvec.h"
#شامل "parse-options.h"
#شامل "pkt-line.h"
#شامل "bundle.h"

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

ایستا نویسه ثابت * ثابت builtin_bundle_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	BUILTIN_BUNDLE_VERIFY_USAGE,
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	پوچ,
};

ایستا ثابت نویسه * ثابت builtin_bundle_create_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	پوچ
};

ایستا ثابت نویسه * ثابت builtin_bundle_verify_usage[] = {
	BUILTIN_BUNDLE_VERIFY_USAGE,
	پوچ
};

ایستا ثابت نویسه * ثابت builtin_bundle_list_heads_usage[] = {
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	پوچ
};

ایستا ثابت نویسه * ثابت builtin_bundle_unbundle_usage[] = {
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	پوچ
};

ایستا صحیح parse_options_cmd_bundle(صحیح argc,
		ثابت نویسه **argv,
		ثابت نویسه* prefix,
		ثابت نویسه * ثابت usagestr[],
		ثابت ساختار option options[],
		نویسه **bundle_file) {
	argc = parse_options(argc, argv, پوچ, options, usagestr,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	اگر (!argc)
		usage_پیام_opt(_("need a <file> argument"), usagestr, options);
	*bundle_file = prefix_filename_except_for_dash(prefix, argv[0]);
	بازگشت argc;
}

ایستا صحیح cmd_bundle_create(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			     ساختار repository *repo UNUSED)
{
	صحیح progress = isatty(STDERR_FILENO);
	صحیح version = -1;
	ساختار option options[] = {
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
	نویسه *bundle_file;
	صحیح ret;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_create_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	اگر (!startup_info->have_repository)
		die(_("Need a repository to create a bundle."));
	ret = !!create_bundle(the_repository, bundle_file, argc, argv, version, progress);
	آزاد(bundle_file);
	بازگشت ret;
}

/*
 * Similar to read_bundle_header(), but handle "-" as stdin.
 */
ایستا صحیح open_bundle(ثابت نویسه *path, ساختار bundle_header *header,
		       ثابت نویسه **name)
{
	اگر (!strcmp(path, "-")) {
		اگر (name)
			*name = "<stdin>";
		بازگشت read_bundle_header_fd(0, header, "<stdin>");
	}

	اگر (name)
		*name = path;
	بازگشت read_bundle_header(path, header);
}

ایستا صحیح cmd_bundle_verify(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			     ساختار repository *repo UNUSED) {
	ساختار bundle_header header = BUNDLE_HEADER_INIT;
	صحیح bundle_fd = -1;
	صحیح quiet = 0;
	صحیح ret;
	ساختار option options[] = {
		OPT_BOOL('q', "quiet", &quiet,
			    N_("do not show bundle details")),
		OPT_END()
	};
	نویسه *bundle_file;
	ثابت نویسه *name;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_verify_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	اگر (!startup_info->have_repository) {
		ret = error(_("need a repository to verify a bundle"));
		برو cleanup;
	}

	اگر ((bundle_fd = open_bundle(bundle_file, &header, &name)) < 0) {
		ret = 1;
		برو cleanup;
	}
	close(bundle_fd);
	اگر (verify_bundle(the_repository, &header,
			  quiet ? VERIFY_BUNDLE_QUIET : VERIFY_BUNDLE_VERBOSE)) {
		ret = 1;
		برو cleanup;
	}

	fprintf(stderr, _("%s is okay\n"), name);
	ret = 0;
cleanup:
	آزاد(bundle_file);
	bundle_header_release(&header);
	بازگشت ret;
}

ایستا صحیح cmd_bundle_list_heads(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
				 ساختار repository *repo UNUSED) {
	ساختار bundle_header header = BUNDLE_HEADER_INIT;
	صحیح bundle_fd = -1;
	صحیح ret;
	ساختار option options[] = {
		OPT_END()
	};
	نویسه *bundle_file;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_list_heads_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	اگر ((bundle_fd = open_bundle(bundle_file, &header, پوچ)) < 0) {
		ret = 1;
		برو cleanup;
	}
	close(bundle_fd);
	ret = !!list_bundle_refs(&header, argc, argv);
cleanup:
	آزاد(bundle_file);
	bundle_header_release(&header);
	بازگشت ret;
}

ایستا صحیح cmd_bundle_unbundle(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			       ساختار repository *repo UNUSED) {
	ساختار bundle_header header = BUNDLE_HEADER_INIT;
	صحیح bundle_fd = -1;
	صحیح ret;
	صحیح progress = isatty(2);

	ساختار option options[] = {
		OPT_BOOL(0, "progress", &progress,
			 N_("show progress meter")),
		OPT_END()
	};
	نویسه *bundle_file;
	ساختار strvec extra_index_pack_args = STRVEC_INIT;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_unbundle_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	اگر (!startup_info->have_repository)
		die(_("Need a repository to unbundle."));

	اگر ((bundle_fd = open_bundle(bundle_file, &header, پوچ)) < 0) {
		ret = 1;
		برو cleanup;
	}
	اگر (progress)
		strvec_pushl(&extra_index_pack_args, "-v", "--progress-title",
			     _("Unbundling objects"), پوچ);
	ret = !!unbundle(the_repository, &header, bundle_fd,
			 &extra_index_pack_args, پوچ) ||
		list_bundle_refs(&header, argc, argv);
	bundle_header_release(&header);

cleanup:
	strvec_clear(&extra_index_pack_args);
	آزاد(bundle_file);
	بازگشت ret;
}

صحیح cmd_bundle(صحیح argc,
	       ثابت نویسه **argv,
	       ثابت نویسه *prefix,
	       ساختار repository *repo)
{
	parse_opt_subcommand_fn *fn = پوچ;
	ساختار option options[] = {
		OPT_SUBCOMMAND("create", &fn, cmd_bundle_create),
		OPT_SUBCOMMAND("verify", &fn, cmd_bundle_verify),
		OPT_SUBCOMMAND("list-heads", &fn, cmd_bundle_list_heads),
		OPT_SUBCOMMAND("unbundle", &fn, cmd_bundle_unbundle),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, builtin_bundle_usage,
			     0);

	packet_trace_identity("bundle");

	بازگشت !!fn(argc, argv, prefix, repo);
}
