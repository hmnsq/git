#define USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "abspath.h"
#ضمن "gettext.h"
#ضمن "setup.h"
#ضمن "strvec.h"
#ضمن "parse-options.h"
#ضمن "pkt-line.h"
#ضمن "bundle.h"

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

ساكن حرف ثابت * ثابت builtin_bundle_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	BUILTIN_BUNDLE_VERIFY_USAGE,
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	عدم,
};

ساكن ثابت حرف * ثابت builtin_bundle_create_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت builtin_bundle_verify_usage[] = {
	BUILTIN_BUNDLE_VERIFY_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت builtin_bundle_list_heads_usage[] = {
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	عدم
};

ساكن ثابت حرف * ثابت builtin_bundle_unbundle_usage[] = {
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	عدم
};

ساكن صحيح parse_options_cmd_bundle(صحيح argc,
		ثابت حرف **argv,
		ثابت حرف* prefix,
		ثابت حرف * ثابت usagestr[],
		ثابت بنية option options[],
		حرف **bundle_file) {
	argc = parse_options(argc, argv, عدم, options, usagestr,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	אם (!argc)
		usage_msg_opt(_("need a <file> argument"), usagestr, options);
	*bundle_file = prefix_filename_except_for_dash(prefix, argv[0]);
	החזר argc;
}

ساكن صحيح cmd_bundle_create(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			     بنية repository *repo UNUSED)
{
	صحيح progress = isatty(STDERR_FILENO);
	صحيح version = -1;
	بنية option options[] = {
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
	حرف *bundle_file;
	صحيح ret;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_create_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	אם (!startup_info->have_repository)
		die(_("Need a repository to create a bundle."));
	ret = !!create_bundle(the_repository, bundle_file, argc, argv, version, progress);
	حرر(bundle_file);
	החזר ret;
}

/*
 * Similar to read_bundle_header(), but handle "-" as stdin.
 */
ساكن صحيح open_bundle(ثابت حرف *path, بنية bundle_header *header,
		       ثابت حرف **name)
{
	אם (!strcmp(path, "-")) {
		אם (name)
			*name = "<stdin>";
		החזר read_bundle_header_fd(0, header, "<stdin>");
	}

	אם (name)
		*name = path;
	החזר read_bundle_header(path, header);
}

ساكن صحيح cmd_bundle_verify(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			     بنية repository *repo UNUSED) {
	بنية bundle_header header = BUNDLE_HEADER_INIT;
	صحيح bundle_fd = -1;
	صحيح quiet = 0;
	صحيح ret;
	بنية option options[] = {
		OPT_BOOL('q', "quiet", &quiet,
			    N_("do not show bundle details")),
		OPT_END()
	};
	حرف *bundle_file;
	ثابت حرف *name;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_verify_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	אם (!startup_info->have_repository) {
		ret = error(_("need a repository to verify a bundle"));
		اذهب cleanup;
	}

	אם ((bundle_fd = open_bundle(bundle_file, &header, &name)) < 0) {
		ret = 1;
		اذهب cleanup;
	}
	close(bundle_fd);
	אם (verify_bundle(the_repository, &header,
			  quiet ? VERIFY_BUNDLE_QUIET : VERIFY_BUNDLE_VERBOSE)) {
		ret = 1;
		اذهب cleanup;
	}

	fprintf(stderr, _("%s is okay\n"), name);
	ret = 0;
cleanup:
	حرر(bundle_file);
	bundle_header_release(&header);
	החזר ret;
}

ساكن صحيح cmd_bundle_list_heads(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
				 بنية repository *repo UNUSED) {
	بنية bundle_header header = BUNDLE_HEADER_INIT;
	صحيح bundle_fd = -1;
	صحيح ret;
	بنية option options[] = {
		OPT_END()
	};
	حرف *bundle_file;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_list_heads_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	אם ((bundle_fd = open_bundle(bundle_file, &header, عدم)) < 0) {
		ret = 1;
		اذهب cleanup;
	}
	close(bundle_fd);
	ret = !!list_bundle_refs(&header, argc, argv);
cleanup:
	حرر(bundle_file);
	bundle_header_release(&header);
	החזר ret;
}

ساكن صحيح cmd_bundle_unbundle(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
			       بنية repository *repo UNUSED) {
	بنية bundle_header header = BUNDLE_HEADER_INIT;
	صحيح bundle_fd = -1;
	صحيح ret;
	صحيح progress = isatty(2);

	بنية option options[] = {
		OPT_BOOL(0, "progress", &progress,
			 N_("show progress meter")),
		OPT_END()
	};
	حرف *bundle_file;
	بنية strvec extra_index_pack_args = STRVEC_INIT;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_unbundle_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	אם (!startup_info->have_repository)
		die(_("Need a repository to unbundle."));

	אם ((bundle_fd = open_bundle(bundle_file, &header, عدم)) < 0) {
		ret = 1;
		اذهب cleanup;
	}
	אם (progress)
		strvec_pushl(&extra_index_pack_args, "-v", "--progress-title",
			     _("Unbundling objects"), عدم);
	ret = !!unbundle(the_repository, &header, bundle_fd,
			 &extra_index_pack_args, عدم) ||
		list_bundle_refs(&header, argc, argv);
	bundle_header_release(&header);

cleanup:
	strvec_clear(&extra_index_pack_args);
	حرر(bundle_file);
	החזר ret;
}

صحيح cmd_bundle(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *prefix,
	       بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT_SUBCOMMAND("create", &fn, cmd_bundle_create),
		OPT_SUBCOMMAND("verify", &fn, cmd_bundle_verify),
		OPT_SUBCOMMAND("list-heads", &fn, cmd_bundle_list_heads),
		OPT_SUBCOMMAND("unbundle", &fn, cmd_bundle_unbundle),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, builtin_bundle_usage,
			     0);

	packet_trace_identity("bundle");

	החזר !!fn(argc, argv, prefix, repo);
}
