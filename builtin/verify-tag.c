/*
 * Builtin "git verify-tag"
 *
 * Copyright (c) 2007 Carlos Rica <jasampler@gmail.com>
 *
 * Based on git-verify-tag.sh
 */
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "tag.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "gpg-interface.h"
#ଅନ୍ତର୍ଭାଵ "ref-filter.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ verify_tag_usage[] = {
		N_("git verify-tag [-v | --verbose] [--format=<format>] [--raw] <tag>..."),
		ଖାଲି
};

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_verify_tag(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		   ଅଚଳ ଅକ୍ଷର **argv,
		   ଅଚଳ ଅକ୍ଷର *prefix,
		   ସଂରଚନା repository *repo)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1, verbose = 0, had_error = 0;
	unsigned flags = 0;
	ସଂରଚନା ref_format format = REF_FORMAT_INIT;
	ଅଚଳ ସଂରଚନା option verify_tag_options[] = {
		OPT__VERBOSE(&verbose, N_("print tag contents")),
		OPT_BIT(0, "raw", &flags, N_("print raw gpg status output"), GPG_VERIFY_RAW),
		OPT_STRING(0, "format", &format.format, N_("format"), N_("format to use for the output")),
		OPT_END()
	};

	repo_config(repo, git_default_config, ଖାଲି);

	argc = parse_options(argc, argv, prefix, verify_tag_options,
			     verify_tag_usage, PARSE_OPT_KEEP_ARGV0);
	ଯଦି (argc <= i)
		usage_with_options(verify_tag_usage, verify_tag_options);

	ଯଦି (verbose)
		flags |= GPG_VERIFY_VERBOSE;

	ଯଦି (format.format) {
		ଯଦି (verify_ref_format(&format))
			usage_with_options(verify_tag_usage,
					   verify_tag_options);
		flags |= GPG_VERIFY_OMIT_STATUS;
	}

	ଯେତେବେଳେ (i < argc) {
		ସଂରଚନା object_id oid;
		ଅଚଳ ଅକ୍ଷର *name = argv[i++];

		ଯଦି (repo_get_oid(repo, name, &oid)) {
			had_error = !!error("tag '%s' not found.", name);
			ଜାରି;
		}

		ଯଦି (gpg_verify_tag(repo, &oid, name, flags)) {
			had_error = 1;
			ଜାରି;
		}

		ଯଦି (format.format)
			pretty_print_ref(name, &oid, ଖାଲି, &format);
	}
	ଫେରନ୍ତୁ had_error;
}
