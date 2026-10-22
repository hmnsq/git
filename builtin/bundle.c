#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strvec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pkt-line.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "bundle.h"

/*
 * Basic handler for bundle files to connect repositories via sneakernet.
 * Invocation must include action.
 * This function can create a bundle or provide information on an existing
 * bundle supporting "fetch", "pull", and "ls-remote".
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_BUNDLE_CREATE_USAGE \
	N_("git bundle create [-q | --quiet | --progress]\n" \
	   "                  [--version=<version>] <file> <git-rev-list-args>")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_BUNDLE_VERIFY_USAGE \
	N_("git bundle verify [-q | --quiet] <file>")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_BUNDLE_LIST_HEADS_USAGE \
	N_("git bundle list-heads <file> [<refname>...]")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUILTIN_BUNDLE_UNBUNDLE_USAGE \
	N_("git bundle unbundle [--progress] <file> [<refname>...]")

ਸਥਿਰ ਅੱਖਰ ਅਚਲ * ਅਚਲ builtin_bundle_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	BUILTIN_BUNDLE_VERIFY_USAGE,
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	ਨਿਰਾ,
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_bundle_create_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_bundle_verify_usage[] = {
	BUILTIN_BUNDLE_VERIFY_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_bundle_list_heads_usage[] = {
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_bundle_unbundle_usage[] = {
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ parse_options_cmd_bundle(ਪੂਰਨ argc,
		ਅਚਲ ਅੱਖਰ **argv,
		ਅਚਲ ਅੱਖਰ* ਅਗੇਤਰ,
		ਅਚਲ ਅੱਖਰ * ਅਚਲ usagestr[],
		ਅਚਲ ਬਣਤਰ option options[],
		ਅੱਖਰ **bundle_file) {
	argc = parse_options(argc, argv, ਨਿਰਾ, options, usagestr,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	ਜੇ (!argc)
		usage_ਸੁਨੇਹਾ_opt(_("need a <file> argument"), usagestr, options);
	*bundle_file = ਅਗੇਤਰ_filename_except_for_dash(ਅਗੇਤਰ, argv[0]);
	ਵਾਪਸ argc;
}

ਸਥਿਰ ਪੂਰਨ cmd_bundle_create(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			     ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ ਤਰੱਕੀ = isatty(STDERR_FILENO);
	ਪੂਰਨ ਸੰਸਕਰਣ = -1;
	ਬਣਤਰ option options[] = {
		OPT_NEGBIT('q', "quiet", &ਤਰੱਕੀ,
			   N_("do not show progress meter"), 1),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "progress", &ਤਰੱਕੀ,
			N_("show progress meter"), 1),
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ_F(0, "all-progress", &ਤਰੱਕੀ,
			  N_("historical; same as --progress"), 1,
			  PARSE_OPT_HIDDEN),
		OPT_NOOP_NOARG(0, "all-progress-implied"),
		OPT_INTEGER(0, "version", &ਸੰਸਕਰਣ,
			    N_("specify bundle format version")),
		OPT_ਸਿਰਾ()
	};
	ਅੱਖਰ *bundle_file;
	ਪੂਰਨ ret;

	argc = parse_options_cmd_bundle(argc, argv, ਅਗੇਤਰ,
			builtin_bundle_create_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ਜੇ (!startup_info->have_repository)
		die(_("Need a repository to create a bundle."));
	ret = !!create_bundle(the_repository, bundle_file, argc, argv, ਸੰਸਕਰਣ, ਤਰੱਕੀ);
	ਮੁਕਤ(bundle_file);
	ਵਾਪਸ ret;
}

/*
 * Similar to read_bundle_header(), but handle "-" as stdin.
 */
ਸਥਿਰ ਪੂਰਨ open_bundle(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਬਣਤਰ bundle_ਸਿਰਲੇਖ *ਸਿਰਲੇਖ,
		       ਅਚਲ ਅੱਖਰ **name)
{
	ਜੇ (!strcmp(ਰਸਤਾ, "-")) {
		ਜੇ (name)
			*name = "<stdin>";
		ਵਾਪਸ read_bundle_ਸਿਰਲੇਖ_fd(0, ਸਿਰਲੇਖ, "<stdin>");
	}

	ਜੇ (name)
		*name = ਰਸਤਾ;
	ਵਾਪਸ read_bundle_ਸਿਰਲੇਖ(ਰਸਤਾ, ਸਿਰਲੇਖ);
}

ਸਥਿਰ ਪੂਰਨ cmd_bundle_verify(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			     ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ) {
	ਬਣਤਰ bundle_ਸਿਰਲੇਖ ਸਿਰਲੇਖ = BUNDLE_ਸਿਰਲੇਖ_INIT;
	ਪੂਰਨ bundle_fd = -1;
	ਪੂਰਨ quiet = 0;
	ਪੂਰਨ ret;
	ਬਣਤਰ option options[] = {
		OPT_BOOL('q', "quiet", &quiet,
			    N_("do not show bundle details")),
		OPT_ਸਿਰਾ()
	};
	ਅੱਖਰ *bundle_file;
	ਅਚਲ ਅੱਖਰ *name;

	argc = parse_options_cmd_bundle(argc, argv, ਅਗੇਤਰ,
			builtin_bundle_verify_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ਜੇ (!startup_info->have_repository) {
		ret = error(_("need a repository to verify a bundle"));
		ਜਾਓ cleanup;
	}

	ਜੇ ((bundle_fd = open_bundle(bundle_file, &ਸਿਰਲੇਖ, &name)) < 0) {
		ret = 1;
		ਜਾਓ cleanup;
	}
	close(bundle_fd);
	ਜੇ (verify_bundle(the_repository, &ਸਿਰਲੇਖ,
			  quiet ? VERIFY_BUNDLE_QUIET : VERIFY_BUNDLE_VERBOSE)) {
		ret = 1;
		ਜਾਓ cleanup;
	}

	fprintf(stderr, _("%s is okay\n"), name);
	ret = 0;
cleanup:
	ਮੁਕਤ(bundle_file);
	bundle_ਸਿਰਲੇਖ_release(&ਸਿਰਲੇਖ);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ cmd_bundle_list_heads(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
				 ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ) {
	ਬਣਤਰ bundle_ਸਿਰਲੇਖ ਸਿਰਲੇਖ = BUNDLE_ਸਿਰਲੇਖ_INIT;
	ਪੂਰਨ bundle_fd = -1;
	ਪੂਰਨ ret;
	ਬਣਤਰ option options[] = {
		OPT_ਸਿਰਾ()
	};
	ਅੱਖਰ *bundle_file;

	argc = parse_options_cmd_bundle(argc, argv, ਅਗੇਤਰ,
			builtin_bundle_list_heads_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ਜੇ ((bundle_fd = open_bundle(bundle_file, &ਸਿਰਲੇਖ, ਨਿਰਾ)) < 0) {
		ret = 1;
		ਜਾਓ cleanup;
	}
	close(bundle_fd);
	ret = !!list_bundle_refs(&ਸਿਰਲੇਖ, argc, argv);
cleanup:
	ਮੁਕਤ(bundle_file);
	bundle_ਸਿਰਲੇਖ_release(&ਸਿਰਲੇਖ);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ cmd_bundle_unbundle(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			       ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ) {
	ਬਣਤਰ bundle_ਸਿਰਲੇਖ ਸਿਰਲੇਖ = BUNDLE_ਸਿਰਲੇਖ_INIT;
	ਪੂਰਨ bundle_fd = -1;
	ਪੂਰਨ ret;
	ਪੂਰਨ ਤਰੱਕੀ = isatty(2);

	ਬਣਤਰ option options[] = {
		OPT_BOOL(0, "progress", &ਤਰੱਕੀ,
			 N_("show progress meter")),
		OPT_ਸਿਰਾ()
	};
	ਅੱਖਰ *bundle_file;
	ਬਣਤਰ strvec extra_ਸੂਚਕਾਂਕ_pack_ਦਲੀਲਾਂ = STRVEC_INIT;

	argc = parse_options_cmd_bundle(argc, argv, ਅਗੇਤਰ,
			builtin_bundle_unbundle_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	ਜੇ (!startup_info->have_repository)
		die(_("Need a repository to unbundle."));

	ਜੇ ((bundle_fd = open_bundle(bundle_file, &ਸਿਰਲੇਖ, ਨਿਰਾ)) < 0) {
		ret = 1;
		ਜਾਓ cleanup;
	}
	ਜੇ (ਤਰੱਕੀ)
		strvec_pushl(&extra_ਸੂਚਕਾਂਕ_pack_ਦਲੀਲਾਂ, "-v", "--progress-title",
			     _("Unbundling objects"), ਨਿਰਾ);
	ret = !!unbundle(the_repository, &ਸਿਰਲੇਖ, bundle_fd,
			 &extra_ਸੂਚਕਾਂਕ_pack_ਦਲੀਲਾਂ, ਨਿਰਾ) ||
		list_bundle_refs(&ਸਿਰਲੇਖ, argc, argv);
	bundle_ਸਿਰਲੇਖ_release(&ਸਿਰਲੇਖ);

cleanup:
	strvec_clear(&extra_ਸੂਚਕਾਂਕ_pack_ਦਲੀਲਾਂ);
	ਮੁਕਤ(bundle_file);
	ਵਾਪਸ ret;
}

ਪੂਰਨ cmd_bundle(ਪੂਰਨ argc,
	       ਅਚਲ ਅੱਖਰ **argv,
	       ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	       ਬਣਤਰ repository *repo)
{
	parse_opt_subcommand_fn *fn = ਨਿਰਾ;
	ਬਣਤਰ option options[] = {
		OPT_SUBCOMMAND("create", &fn, cmd_bundle_create),
		OPT_SUBCOMMAND("verify", &fn, cmd_bundle_verify),
		OPT_SUBCOMMAND("list-heads", &fn, cmd_bundle_list_heads),
		OPT_SUBCOMMAND("unbundle", &fn, cmd_bundle_unbundle),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, builtin_bundle_usage,
			     0);

	packet_trace_ਪਛਾਣ_ਤੱਤ("bundle");

	ਵਾਪਸ !!fn(argc, argv, ਅਗੇਤਰ, repo);
}
