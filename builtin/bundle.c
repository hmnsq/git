#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "abspath.h"
#include "gettext.h"
#include "setup.h"
#include "strvec.h"
#include "parse-options.h"
#include "pkt-line.h"
#include "bundle.h"

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

static var_na const * const builtin_bundle_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	BUILTIN_BUNDLE_VERIFY_USAGE,
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	NULL,
};

static const var_na * const builtin_bundle_create_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	NULL
};

static const var_na * const builtin_bundle_verify_usage[] = {
	BUILTIN_BUNDLE_VERIFY_USAGE,
	NULL
};

static const var_na * const builtin_bundle_list_heads_usage[] = {
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	NULL
};

static const var_na * const builtin_bundle_unbundle_usage[] = {
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	NULL
};

static ចំនួនគត់ parse_options_cmd_bundle(ចំនួនគត់ argc,
		const var_na **argv,
		const var_na* prefix,
		const var_na * const usagestr[],
		const sa_nracanaa option options[],
		var_na **bundle_file) {
	argc = parse_options(argc, argv, NULL, options, usagestr,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	ប្រសិនបើ (!argc)
		usage_msg_opt(_("need a <file> argument"), usagestr, options);
	*bundle_file = prefix_filename_except_for_dash(prefix, argv[0]);
	ត្រឡប់ argc;
}

static ចំនួនគត់ cmd_bundle_create(ចំនួនគត់ argc, const var_na **argv, const var_na *prefix,
			     sa_nracanaa repository *repo UNUSED)
{
	ចំនួនគត់ progress = isatty(STDERR_FILENO);
	ចំនួនគត់ version = -1;
	sa_nracanaa option options[] = {
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
	var_na *bundle_file;
	ចំនួនគត់ ret;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_create_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ប្រសិនបើ (!startup_info->have_repository)
		die(_("Need a repository to create a bundle."));
	ret = !!create_bundle(the_repository, bundle_file, argc, argv, version, progress);
	free(bundle_file);
	ត្រឡប់ ret;
}

/*
 * Similar to read_bundle_header(), but handle "-" as stdin.
 */
static ចំនួនគត់ open_bundle(const var_na *path, sa_nracanaa bundle_header *header,
		       const var_na **name)
{
	ប្រសិនបើ (!strcmp(path, "-")) {
		ប្រសិនបើ (name)
			*name = "<stdin>";
		ត្រឡប់ read_bundle_header_fd(0, header, "<stdin>");
	}

	ប្រសិនបើ (name)
		*name = path;
	ត្រឡប់ read_bundle_header(path, header);
}

static ចំនួនគត់ cmd_bundle_verify(ចំនួនគត់ argc, const var_na **argv, const var_na *prefix,
			     sa_nracanaa repository *repo UNUSED) {
	sa_nracanaa bundle_header header = BUNDLE_HEADER_INIT;
	ចំនួនគត់ bundle_fd = -1;
	ចំនួនគត់ quiet = 0;
	ចំនួនគត់ ret;
	sa_nracanaa option options[] = {
		OPT_BOOL('q', "quiet", &quiet,
			    N_("do not show bundle details")),
		OPT_END()
	};
	var_na *bundle_file;
	const var_na *name;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_verify_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ប្រសិនបើ (!startup_info->have_repository) {
		ret = error(_("need a repository to verify a bundle"));
		goto cleanup;
	}

	ប្រសិនបើ ((bundle_fd = open_bundle(bundle_file, &header, &name)) < 0) {
		ret = 1;
		goto cleanup;
	}
	close(bundle_fd);
	ប្រសិនបើ (verify_bundle(the_repository, &header,
			  quiet ? VERIFY_BUNDLE_QUIET : VERIFY_BUNDLE_VERBOSE)) {
		ret = 1;
		goto cleanup;
	}

	fprintf(stderr, _("%s is okay\n"), name);
	ret = 0;
cleanup:
	free(bundle_file);
	bundle_header_release(&header);
	ត្រឡប់ ret;
}

static ចំនួនគត់ cmd_bundle_list_heads(ចំនួនគត់ argc, const var_na **argv, const var_na *prefix,
				 sa_nracanaa repository *repo UNUSED) {
	sa_nracanaa bundle_header header = BUNDLE_HEADER_INIT;
	ចំនួនគត់ bundle_fd = -1;
	ចំនួនគត់ ret;
	sa_nracanaa option options[] = {
		OPT_END()
	};
	var_na *bundle_file;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_list_heads_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ប្រសិនបើ ((bundle_fd = open_bundle(bundle_file, &header, NULL)) < 0) {
		ret = 1;
		goto cleanup;
	}
	close(bundle_fd);
	ret = !!list_bundle_refs(&header, argc, argv);
cleanup:
	free(bundle_file);
	bundle_header_release(&header);
	ត្រឡប់ ret;
}

static ចំនួនគត់ cmd_bundle_unbundle(ចំនួនគត់ argc, const var_na **argv, const var_na *prefix,
			       sa_nracanaa repository *repo UNUSED) {
	sa_nracanaa bundle_header header = BUNDLE_HEADER_INIT;
	ចំនួនគត់ bundle_fd = -1;
	ចំនួនគត់ ret;
	ចំនួនគត់ progress = isatty(2);

	sa_nracanaa option options[] = {
		OPT_BOOL(0, "progress", &progress,
			 N_("show progress meter")),
		OPT_END()
	};
	var_na *bundle_file;
	sa_nracanaa strvec extra_index_pack_args = STRVEC_INIT;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_unbundle_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ប្រសិនបើ (!startup_info->have_repository)
		die(_("Need a repository to unbundle."));

	ប្រសិនបើ ((bundle_fd = open_bundle(bundle_file, &header, NULL)) < 0) {
		ret = 1;
		goto cleanup;
	}
	ប្រសិនបើ (progress)
		strvec_pushl(&extra_index_pack_args, "-v", "--progress-title",
			     _("Unbundling objects"), NULL);
	ret = !!unbundle(the_repository, &header, bundle_fd,
			 &extra_index_pack_args, NULL) ||
		list_bundle_refs(&header, argc, argv);
	bundle_header_release(&header);

cleanup:
	strvec_clear(&extra_index_pack_args);
	free(bundle_file);
	ត្រឡប់ ret;
}

ចំនួនគត់ cmd_bundle(ចំនួនគត់ argc,
	       const var_na **argv,
	       const var_na *prefix,
	       sa_nracanaa repository *repo)
{
	parse_opt_subcommand_fn *fn = NULL;
	sa_nracanaa option options[] = {
		OPT_SUBCOMMAND("create", &fn, cmd_bundle_create),
		OPT_SUBCOMMAND("verify", &fn, cmd_bundle_verify),
		OPT_SUBCOMMAND("list-heads", &fn, cmd_bundle_list_heads),
		OPT_SUBCOMMAND("unbundle", &fn, cmd_bundle_unbundle),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, builtin_bundle_usage,
			     0);

	packet_trace_identity("bundle");

	ត្រឡប់ !!fn(argc, argv, prefix, repo);
}
