#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
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

#تعریف_کړه BUILTIN_BUNDLE_CREATE_USAGE \
	N_("git bundle create [-q | --quiet | --progress]\n" \
	   "                  [--version=<version>] <file> <git-rev-list-args>")
#تعریف_کړه BUILTIN_BUNDLE_VERIFY_USAGE \
	N_("git bundle verify [-q | --quiet] <file>")
#تعریف_کړه BUILTIN_BUNDLE_LIST_HEADS_USAGE \
	N_("git bundle list-heads <file> [<refname>...]")
#تعریف_کړه BUILTIN_BUNDLE_UNBUNDLE_USAGE \
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
		ثابت حرف* مختاړی,
		ثابت حرف * ثابت usagestr[],
		ثابت بنية option options[],
		حرف **bundle_file) {
	argc = parse_options(argc, argv, عدم, options, usagestr,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	إذا (!argc)
		usage_پیغام_opt(_("need a <file> argument"), usagestr, options);
	*bundle_file = مختاړی_filename_except_for_dash(مختاړی, argv[0]);
	أرجع argc;
}

ساكن صحيح cmd_bundle_create(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			     بنية repository *repo ناکارول_شوی)
{
	صحيح پرمختګ = isatty(STDERR_FILENO);
	صحيح نسخه = -1;
	بنية option options[] = {
		OPT_NEGBIT('q', "quiet", &پرمختګ,
			   N_("do not show progress meter"), 1),
		OPT_بټ(0, "progress", &پرمختګ,
			N_("show progress meter"), 1),
		OPT_بټ_F(0, "all-progress", &پرمختګ,
			  N_("historical; same as --progress"), 1,
			  PARSE_OPT_HIDDEN),
		OPT_NOOP_NOARG(0, "all-progress-implied"),
		OPT_INTEGER(0, "version", &نسخه,
			    N_("specify bundle format version")),
		OPT_پای_ټکی()
	};
	حرف *bundle_file;
	صحيح ret;

	argc = parse_options_cmd_bundle(argc, argv, مختاړی,
			builtin_bundle_create_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	إذا (!startup_info->have_repository)
		die(_("Need a repository to create a bundle."));
	ret = !!create_bundle(the_repository, bundle_file, argc, argv, نسخه, پرمختګ);
	حرر(bundle_file);
	أرجع ret;
}

/*
 * Similar to read_bundle_header(), but handle "-" as stdin.
 */
ساكن صحيح open_bundle(ثابت حرف *لار, بنية bundle_سرلیک *سرلیک,
		       ثابت حرف **name)
{
	إذا (!strcmp(لار, "-")) {
		إذا (name)
			*name = "<stdin>";
		أرجع read_bundle_سرلیک_fd(0, سرلیک, "<stdin>");
	}

	إذا (name)
		*name = لار;
	أرجع read_bundle_سرلیک(لار, سرلیک);
}

ساكن صحيح cmd_bundle_verify(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			     بنية repository *repo ناکارول_شوی) {
	بنية bundle_سرلیک سرلیک = BUNDLE_سرلیک_INIT;
	صحيح bundle_fd = -1;
	صحيح quiet = 0;
	صحيح ret;
	بنية option options[] = {
		OPT_BOOL('q', "quiet", &quiet,
			    N_("do not show bundle details")),
		OPT_پای_ټکی()
	};
	حرف *bundle_file;
	ثابت حرف *name;

	argc = parse_options_cmd_bundle(argc, argv, مختاړی,
			builtin_bundle_verify_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	إذا (!startup_info->have_repository) {
		ret = error(_("need a repository to verify a bundle"));
		اذهب cleanup;
	}

	إذا ((bundle_fd = open_bundle(bundle_file, &سرلیک, &name)) < 0) {
		ret = 1;
		اذهب cleanup;
	}
	close(bundle_fd);
	إذا (verify_bundle(the_repository, &سرلیک,
			  quiet ? VERIFY_BUNDLE_QUIET : VERIFY_BUNDLE_VERBOSE)) {
		ret = 1;
		اذهب cleanup;
	}

	fprintf(stderr, _("%s is okay\n"), name);
	ret = 0;
cleanup:
	حرر(bundle_file);
	bundle_سرلیک_release(&سرلیک);
	أرجع ret;
}

ساكن صحيح cmd_bundle_list_heads(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
				 بنية repository *repo ناکارول_شوی) {
	بنية bundle_سرلیک سرلیک = BUNDLE_سرلیک_INIT;
	صحيح bundle_fd = -1;
	صحيح ret;
	بنية option options[] = {
		OPT_پای_ټکی()
	};
	حرف *bundle_file;

	argc = parse_options_cmd_bundle(argc, argv, مختاړی,
			builtin_bundle_list_heads_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	إذا ((bundle_fd = open_bundle(bundle_file, &سرلیک, عدم)) < 0) {
		ret = 1;
		اذهب cleanup;
	}
	close(bundle_fd);
	ret = !!list_bundle_refs(&سرلیک, argc, argv);
cleanup:
	حرر(bundle_file);
	bundle_سرلیک_release(&سرلیک);
	أرجع ret;
}

ساكن صحيح cmd_bundle_unbundle(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
			       بنية repository *repo ناکارول_شوی) {
	بنية bundle_سرلیک سرلیک = BUNDLE_سرلیک_INIT;
	صحيح bundle_fd = -1;
	صحيح ret;
	صحيح پرمختګ = isatty(2);

	بنية option options[] = {
		OPT_BOOL(0, "progress", &پرمختګ,
			 N_("show progress meter")),
		OPT_پای_ټکی()
	};
	حرف *bundle_file;
	بنية strvec extra_شاخص_pack_دلیلونه = STRVEC_INIT;

	argc = parse_options_cmd_bundle(argc, argv, مختاړی,
			builtin_bundle_unbundle_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	إذا (!startup_info->have_repository)
		die(_("Need a repository to unbundle."));

	إذا ((bundle_fd = open_bundle(bundle_file, &سرلیک, عدم)) < 0) {
		ret = 1;
		اذهب cleanup;
	}
	إذا (پرمختګ)
		strvec_pushl(&extra_شاخص_pack_دلیلونه, "-v", "--progress-title",
			     _("Unbundling objects"), عدم);
	ret = !!unbundle(the_repository, &سرلیک, bundle_fd,
			 &extra_شاخص_pack_دلیلونه, عدم) ||
		list_bundle_refs(&سرلیک, argc, argv);
	bundle_سرلیک_release(&سرلیک);

cleanup:
	strvec_clear(&extra_شاخص_pack_دلیلونه);
	حرر(bundle_file);
	أرجع ret;
}

صحيح cmd_bundle(صحيح argc,
	       ثابت حرف **argv,
	       ثابت حرف *مختاړی,
	       بنية repository *repo)
{
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT_SUBCOMMAND("create", &fn, cmd_bundle_create),
		OPT_SUBCOMMAND("verify", &fn, cmd_bundle_verify),
		OPT_SUBCOMMAND("list-heads", &fn, cmd_bundle_list_heads),
		OPT_SUBCOMMAND("unbundle", &fn, cmd_bundle_unbundle),
		OPT_پای_ټکی()
	};

	argc = parse_options(argc, argv, مختاړی, options, builtin_bundle_usage,
			     0);

	packet_trace_بې_اغېزه_عنصر("bundle");

	أرجع !!fn(argc, argv, مختاړی, repo);
}
