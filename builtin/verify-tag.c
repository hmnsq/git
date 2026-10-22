/*
 * Builtin "git verify-tag"
 *
 * Copyright (c) 2007 Carlos Rica <jasampler@gmail.com>
 *
 * Based on git-verify-tag.sh
 */
#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "tag.h"
#include "object-name.h"
#include "parse-options.h"
#include "gpg-interface.h"
#include "ref-filter.h"

static const var_na * const verify_tag_usage[] = {
		N_("git verify-tag [-v | --verbose] [--format=<format>] [--raw] <tag>..."),
		NULL
};

puur_naa_nka cmd_verify_tag(puur_naa_nka argc,
		   const var_na **argv,
		   const var_na *prefix,
		   sa_nracanaa repository *repo)
{
	puur_naa_nka i = 1, verbose = 0, had_error = 0;
	unsigned flags = 0;
	sa_nracanaa ref_format format = REF_FORMAT_INIT;
	const sa_nracanaa option verify_tag_options[] = {
		OPT__VERBOSE(&verbose, N_("print tag contents")),
		OPT_BIT(0, "raw", &flags, N_("print raw gpg status output"), GPG_VERIFY_RAW),
		OPT_STRING(0, "format", &format.format, N_("format"), N_("format to use for the output")),
		OPT_END()
	};

	repo_config(repo, git_default_config, NULL);

	argc = parse_options(argc, argv, prefix, verify_tag_options,
			     verify_tag_usage, PARSE_OPT_KEEP_ARGV0);
	ꦪꦼꦤ꧀ (argc <= i)
		usage_with_options(verify_tag_usage, verify_tag_options);

	ꦪꦼꦤ꧀ (verbose)
		flags |= GPG_VERIFY_VERBOSE;

	ꦪꦼꦤ꧀ (format.format) {
		ꦪꦼꦤ꧀ (verify_ref_format(&format))
			usage_with_options(verify_tag_usage,
					   verify_tag_options);
		flags |= GPG_VERIFY_OMIT_STATUS;
	}

	ꦱꦼꦢꦼꦭ (i < argc) {
		sa_nracanaa object_id oid;
		const var_na *name = argv[i++];

		ꦪꦼꦤ꧀ (repo_get_oid(repo, name, &oid)) {
			had_error = !!error("tag '%s' not found.", name);
			continue;
		}

		ꦪꦼꦤ꧀ (gpg_verify_tag(repo, &oid, name, flags)) {
			had_error = 1;
			continue;
		}

		ꦪꦼꦤ꧀ (format.format)
			pretty_print_ref(name, &oid, NULL, &format);
	}
	ꦮꦭꦶ had_error;
}
