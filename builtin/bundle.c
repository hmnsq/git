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

static char const * const builtin_bundle_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	BUILTIN_BUNDLE_VERIFY_USAGE,
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	NULL,
};

static const char * const builtin_bundle_create_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	NULL
};

static const char * const builtin_bundle_verify_usage[] = {
	BUILTIN_BUNDLE_VERIFY_USAGE,
	NULL
};

static const char * const builtin_bundle_list_heads_usage[] = {
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	NULL
};

static const char * const builtin_bundle_unbundle_usage[] = {
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	NULL
};

static целое parse_options_cmd_bundle(целое argc,
		const char **argv,
		const char* prefix,
		const char * const usagestr[],
		const struct option options[],
		char **bundle_file) {
	argc = parse_options(argc, argv, NULL, options, usagestr,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	если (!argc)
		usage_msg_opt(_("need a <file> argument"), usagestr, options);
	*bundle_file = prefix_filename_except_for_dash(prefix, argv[0]);
	вернуть argc;
}

static целое cmd_bundle_create(целое argc, const char **argv, const char *prefix,
			     struct repository *repo UNUSED)
{
	целое progress = isatty(STDERR_FILENO);
	целое version = -1;
	struct option options[] = {
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
	char *bundle_file;
	целое ret;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_create_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	если (!startup_info->have_repository)
		die(_("Need a repository to create a bundle."));
	ret = !!create_bundle(the_repository, bundle_file, argc, argv, version, progress);
	free(bundle_file);
	вернуть ret;
}

/*
 * Similar to read_bundle_header(), but handle "-" as stdin.
 */
static целое open_bundle(const char *path, struct bundle_header *header,
		       const char **name)
{
	если (!strcmp(path, "-")) {
		если (name)
			*name = "<stdin>";
		вернуть read_bundle_header_fd(0, header, "<stdin>");
	}

	если (name)
		*name = path;
	вернуть read_bundle_header(path, header);
}

static целое cmd_bundle_verify(целое argc, const char **argv, const char *prefix,
			     struct repository *repo UNUSED) {
	struct bundle_header header = BUNDLE_HEADER_INIT;
	целое bundle_fd = -1;
	целое quiet = 0;
	целое ret;
	struct option options[] = {
		OPT_BOOL('q', "quiet", &quiet,
			    N_("do not show bundle details")),
		OPT_END()
	};
	char *bundle_file;
	const char *name;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_verify_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	если (!startup_info->have_repository) {
		ret = error(_("need a repository to verify a bundle"));
		goto cleanup;
	}

	если ((bundle_fd = open_bundle(bundle_file, &header, &name)) < 0) {
		ret = 1;
		goto cleanup;
	}
	close(bundle_fd);
	если (verify_bundle(the_repository, &header,
			  quiet ? VERIFY_BUNDLE_QUIET : VERIFY_BUNDLE_VERBOSE)) {
		ret = 1;
		goto cleanup;
	}

	fprintf(stderr, _("%s is okay\n"), name);
	ret = 0;
cleanup:
	free(bundle_file);
	bundle_header_release(&header);
	вернуть ret;
}

static целое cmd_bundle_list_heads(целое argc, const char **argv, const char *prefix,
				 struct repository *repo UNUSED) {
	struct bundle_header header = BUNDLE_HEADER_INIT;
	целое bundle_fd = -1;
	целое ret;
	struct option options[] = {
		OPT_END()
	};
	char *bundle_file;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_list_heads_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	если ((bundle_fd = open_bundle(bundle_file, &header, NULL)) < 0) {
		ret = 1;
		goto cleanup;
	}
	close(bundle_fd);
	ret = !!list_bundle_refs(&header, argc, argv);
cleanup:
	free(bundle_file);
	bundle_header_release(&header);
	вернуть ret;
}

static целое cmd_bundle_unbundle(целое argc, const char **argv, const char *prefix,
			       struct repository *repo UNUSED) {
	struct bundle_header header = BUNDLE_HEADER_INIT;
	целое bundle_fd = -1;
	целое ret;
	целое progress = isatty(2);

	struct option options[] = {
		OPT_BOOL(0, "progress", &progress,
			 N_("show progress meter")),
		OPT_END()
	};
	char *bundle_file;
	struct strvec extra_index_pack_args = STRVEC_INIT;

	argc = parse_options_cmd_bundle(argc, argv, prefix,
			builtin_bundle_unbundle_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	если (!startup_info->have_repository)
		die(_("Need a repository to unbundle."));

	если ((bundle_fd = open_bundle(bundle_file, &header, NULL)) < 0) {
		ret = 1;
		goto cleanup;
	}
	если (progress)
		strvec_pushl(&extra_index_pack_args, "-v", "--progress-title",
			     _("Unbundling objects"), NULL);
	ret = !!unbundle(the_repository, &header, bundle_fd,
			 &extra_index_pack_args, NULL) ||
		list_bundle_refs(&header, argc, argv);
	bundle_header_release(&header);

cleanup:
	strvec_clear(&extra_index_pack_args);
	free(bundle_file);
	вернуть ret;
}

целое cmd_bundle(целое argc,
	       const char **argv,
	       const char *prefix,
	       struct repository *repo)
{
	parse_opt_subcommand_fn *fn = NULL;
	struct option options[] = {
		OPT_SUBCOMMAND("create", &fn, cmd_bundle_create),
		OPT_SUBCOMMAND("verify", &fn, cmd_bundle_verify),
		OPT_SUBCOMMAND("list-heads", &fn, cmd_bundle_list_heads),
		OPT_SUBCOMMAND("unbundle", &fn, cmd_bundle_unbundle),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, builtin_bundle_usage,
			     0);

	packet_trace_identity("bundle");

	вернуть !!fn(argc, argv, prefix, repo);
}
