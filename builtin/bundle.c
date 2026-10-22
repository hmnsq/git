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

static puur_naa_nka parse_options_cmd_bundle(puur_naa_nka argc,
		const var_na **argv,
		const var_na* prefix,
		const var_na * const usagestr[],
		const sa_nracanaa option options[],
		var_na **bundle_file) {
	argc = parse_options(argc, argv, NULL, options, usagestr,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	ᮚᮦᮔ᮪ (!argc)
		usage_msg_opt(_("need a <file> argument"), usagestr, options);
	*bundle_file = prefix_filename_except_for_dash(prefix, argv[0]);
	ᮝᮜᮤ argc;
}

static puur_naa_nka cmd_bundle_create(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			     sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka progress = isatty(STDERR_FILENO);
	puur_naa_nka version = -1;
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
	puur_naa_nka ret;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_create_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ᮚᮦᮔ᮪ (!startup_info->have_repository)
		die(_("Need a repository to create a bundle."));
	ret = !!create_bundle(the_repository, bundle_file, argc, argv, version, progress);
	free(bundle_file);
	ᮝᮜᮤ ret;
}

/*
 * Similar to read_bundle_header(), but handle "-" as stdin.
 */
static puur_naa_nka open_bundle(const var_na *path, sa_nracanaa bundle_header *header,
		       const var_na **name)
{
	ᮚᮦᮔ᮪ (!strcmp(path, "-")) {
		ᮚᮦᮔ᮪ (name)
			*name = "<stdin>";
		ᮝᮜᮤ read_bundle_header_fd(0, header, "<stdin>");
	}

	ᮚᮦᮔ᮪ (name)
		*name = path;
	ᮝᮜᮤ read_bundle_header(path, header);
}

static puur_naa_nka cmd_bundle_verify(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			     sa_nracanaa repository *repo UNUSED) {
	sa_nracanaa bundle_header header = BUNDLE_HEADER_INIT;
	puur_naa_nka bundle_fd = -1;
	puur_naa_nka quiet = 0;
	puur_naa_nka ret;
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

	ᮚᮦᮔ᮪ (!startup_info->have_repository) {
		ret = error(_("need a repository to verify a bundle"));
		goto cleanup;
	}

	ᮚᮦᮔ᮪ ((bundle_fd = open_bundle(bundle_file, &header, &name)) < 0) {
		ret = 1;
		goto cleanup;
	}
	close(bundle_fd);
	ᮚᮦᮔ᮪ (verify_bundle(the_repository, &header,
			  quiet ? VERIFY_BUNDLE_QUIET : VERIFY_BUNDLE_VERBOSE)) {
		ret = 1;
		goto cleanup;
	}

	fprintf(stderr, _("%s is okay\n"), name);
	ret = 0;
cleanup:
	free(bundle_file);
	bundle_header_release(&header);
	ᮝᮜᮤ ret;
}

static puur_naa_nka cmd_bundle_list_heads(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
				 sa_nracanaa repository *repo UNUSED) {
	sa_nracanaa bundle_header header = BUNDLE_HEADER_INIT;
	puur_naa_nka bundle_fd = -1;
	puur_naa_nka ret;
	sa_nracanaa option options[] = {
		OPT_END()
	};
	var_na *bundle_file;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_list_heads_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ᮚᮦᮔ᮪ ((bundle_fd = open_bundle(bundle_file, &header, NULL)) < 0) {
		ret = 1;
		goto cleanup;
	}
	close(bundle_fd);
	ret = !!list_bundle_refs(&header, argc, argv);
cleanup:
	free(bundle_file);
	bundle_header_release(&header);
	ᮝᮜᮤ ret;
}

static puur_naa_nka cmd_bundle_unbundle(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
			       sa_nracanaa repository *repo UNUSED) {
	sa_nracanaa bundle_header header = BUNDLE_HEADER_INIT;
	puur_naa_nka bundle_fd = -1;
	puur_naa_nka ret;
	puur_naa_nka progress = isatty(2);

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

	ᮚᮦᮔ᮪ (!startup_info->have_repository)
		die(_("Need a repository to unbundle."));

	ᮚᮦᮔ᮪ ((bundle_fd = open_bundle(bundle_file, &header, NULL)) < 0) {
		ret = 1;
		goto cleanup;
	}
	ᮚᮦᮔ᮪ (progress)
		strvec_pushl(&extra_index_pack_args, "-v", "--progress-title",
			     _("Unbundling objects"), NULL);
	ret = !!unbundle(the_repository, &header, bundle_fd,
			 &extra_index_pack_args, NULL) ||
		list_bundle_refs(&header, argc, argv);
	bundle_header_release(&header);

cleanup:
	strvec_clear(&extra_index_pack_args);
	free(bundle_file);
	ᮝᮜᮤ ret;
}

puur_naa_nka cmd_bundle(puur_naa_nka argc,
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

	ᮝᮜᮤ !!fn(argc, argv, prefix, repo);
}
