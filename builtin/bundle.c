#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
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

#تعریف_کرو BUILTIN_BUNDLE_CREATE_USAGE \
	N_("git bundle create [-q | --quiet | --progress]\n" \
	   "                  [--version=<version>] <file> <git-rev-list-args>")
#تعریف_کرو BUILTIN_BUNDLE_VERIFY_USAGE \
	N_("git bundle verify [-q | --quiet] <file>")
#تعریف_کرو BUILTIN_BUNDLE_LIST_HEADS_USAGE \
	N_("git bundle list-heads <file> [<refname>...]")
#تعریف_کرو BUILTIN_BUNDLE_UNBUNDLE_USAGE \
	N_("git bundle unbundle [--progress] <file> [<refname>...]")

ساکن حرف ثابت * ثابت builtin_bundle_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	BUILTIN_BUNDLE_VERIFY_USAGE,
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	تہی,
};

ساکن ثابت حرف * ثابت builtin_bundle_create_usage[] = {
	BUILTIN_BUNDLE_CREATE_USAGE,
	تہی
};

ساکن ثابت حرف * ثابت builtin_bundle_verify_usage[] = {
	BUILTIN_BUNDLE_VERIFY_USAGE,
	تہی
};

ساکن ثابت حرف * ثابت builtin_bundle_list_heads_usage[] = {
	BUILTIN_BUNDLE_LIST_HEADS_USAGE,
	تہی
};

ساکن ثابت حرف * ثابت builtin_bundle_unbundle_usage[] = {
	BUILTIN_BUNDLE_UNBUNDLE_USAGE,
	تہی
};

ساکن صحیح parse_options_cmd_bundle(صحیح argc,
		ثابت حرف **argv,
		ثابت حرف* سابقہ,
		ثابت حرف * ثابت usagestr[],
		ثابت ساخت option options[],
		حرف **bundle_file) {
	argc = parse_options(argc, argv, تہی, options, usagestr,
			     PARSE_OPT_STOP_AT_NON_OPTION);
	اگر (!argc)
		usage_پیغام_opt(_("need a <file> argument"), usagestr, options);
	*bundle_file = سابقہ_filename_except_for_dash(سابقہ, argv[0]);
	واپس argc;
}

ساکن صحیح cmd_bundle_create(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			     ساخت repository *repo غیر_مستعمل)
{
	صحیح پیش_رفت = isatty(STDERR_FILENO);
	صحیح نسخہ = -1;
	ساخت option options[] = {
		OPT_NEGBIT('q', "quiet", &پیش_رفت,
			   N_("do not show progress meter"), 1),
		OPT_ثنائی_ہندسہ(0, "progress", &پیش_رفت,
			N_("show progress meter"), 1),
		OPT_ثنائی_ہندسہ_F(0, "all-progress", &پیش_رفت,
			  N_("historical; same as --progress"), 1,
			  PARSE_OPT_HIDDEN),
		OPT_NOOP_NOARG(0, "all-progress-implied"),
		OPT_INTEGER(0, "version", &نسخہ,
			    N_("specify bundle format version")),
		OPT_انتہا()
	};
	حرف *bundle_file;
	صحیح ret;

	argc = parse_options_cmd_bundle(argc, argv, سابقہ,
			builtin_bundle_create_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	اگر (!startup_info->have_repository)
		die(_("Need a repository to create a bundle."));
	ret = !!create_bundle(the_repository, bundle_file, argc, argv, نسخہ, پیش_رفت);
	آزاد(bundle_file);
	واپس ret;
}

/*
 * Similar to read_bundle_header(), but handle "-" as stdin.
 */
ساکن صحیح open_bundle(ثابت حرف *راستہ, ساخت bundle_سرخی *سرخی,
		       ثابت حرف **name)
{
	اگر (!strcmp(راستہ, "-")) {
		اگر (name)
			*name = "<stdin>";
		واپس read_bundle_سرخی_fd(0, سرخی, "<stdin>");
	}

	اگر (name)
		*name = راستہ;
	واپس read_bundle_سرخی(راستہ, سرخی);
}

ساکن صحیح cmd_bundle_verify(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			     ساخت repository *repo غیر_مستعمل) {
	ساخت bundle_سرخی سرخی = BUNDLE_سرخی_INIT;
	صحیح bundle_fd = -1;
	صحیح quiet = 0;
	صحیح ret;
	ساخت option options[] = {
		OPT_BOOL('q', "quiet", &quiet,
			    N_("do not show bundle details")),
		OPT_انتہا()
	};
	حرف *bundle_file;
	ثابت حرف *name;

	argc = parse_options_cmd_bundle(argc, argv, سابقہ,
			builtin_bundle_verify_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	اگر (!startup_info->have_repository) {
		ret = error(_("need a repository to verify a bundle"));
		جاؤ cleanup;
	}

	اگر ((bundle_fd = open_bundle(bundle_file, &سرخی, &name)) < 0) {
		ret = 1;
		جاؤ cleanup;
	}
	close(bundle_fd);
	اگر (verify_bundle(the_repository, &سرخی,
			  quiet ? VERIFY_BUNDLE_QUIET : VERIFY_BUNDLE_VERBOSE)) {
		ret = 1;
		جاؤ cleanup;
	}

	fprintf(stderr, _("%s is okay\n"), name);
	ret = 0;
cleanup:
	آزاد(bundle_file);
	bundle_سرخی_release(&سرخی);
	واپس ret;
}

ساکن صحیح cmd_bundle_list_heads(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
				 ساخت repository *repo غیر_مستعمل) {
	ساخت bundle_سرخی سرخی = BUNDLE_سرخی_INIT;
	صحیح bundle_fd = -1;
	صحیح ret;
	ساخت option options[] = {
		OPT_انتہا()
	};
	حرف *bundle_file;

	argc = parse_options_cmd_bundle(argc, argv, سابقہ,
			builtin_bundle_list_heads_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	اگر ((bundle_fd = open_bundle(bundle_file, &سرخی, تہی)) < 0) {
		ret = 1;
		جاؤ cleanup;
	}
	close(bundle_fd);
	ret = !!list_bundle_refs(&سرخی, argc, argv);
cleanup:
	آزاد(bundle_file);
	bundle_سرخی_release(&سرخی);
	واپس ret;
}

ساکن صحیح cmd_bundle_unbundle(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
			       ساخت repository *repo غیر_مستعمل) {
	ساخت bundle_سرخی سرخی = BUNDLE_سرخی_INIT;
	صحیح bundle_fd = -1;
	صحیح ret;
	صحیح پیش_رفت = isatty(2);

	ساخت option options[] = {
		OPT_BOOL(0, "progress", &پیش_رفت,
			 N_("show progress meter")),
		OPT_انتہا()
	};
	حرف *bundle_file;
	ساخت strvec extra_اشاریہ_pack_دلائل = STRVEC_INIT;

	argc = parse_options_cmd_bundle(argc, argv, سابقہ,
			builtin_bundle_unbundle_usage, options, &bundle_file);
	/* bundle internals use argv[1] as further parameters */

	اگر (!startup_info->have_repository)
		die(_("Need a repository to unbundle."));

	اگر ((bundle_fd = open_bundle(bundle_file, &سرخی, تہی)) < 0) {
		ret = 1;
		جاؤ cleanup;
	}
	اگر (پیش_رفت)
		strvec_pushl(&extra_اشاریہ_pack_دلائل, "-v", "--progress-title",
			     _("Unbundling objects"), تہی);
	ret = !!unbundle(the_repository, &سرخی, bundle_fd,
			 &extra_اشاریہ_pack_دلائل, تہی) ||
		list_bundle_refs(&سرخی, argc, argv);
	bundle_سرخی_release(&سرخی);

cleanup:
	strvec_clear(&extra_اشاریہ_pack_دلائل);
	آزاد(bundle_file);
	واپس ret;
}

صحیح cmd_bundle(صحیح argc,
	       ثابت حرف **argv,
	       ثابت حرف *سابقہ,
	       ساخت repository *repo)
{
	parse_opt_subcommand_fn *fn = تہی;
	ساخت option options[] = {
		OPT_SUBCOMMAND("create", &fn, cmd_bundle_create),
		OPT_SUBCOMMAND("verify", &fn, cmd_bundle_verify),
		OPT_SUBCOMMAND("list-heads", &fn, cmd_bundle_list_heads),
		OPT_SUBCOMMAND("unbundle", &fn, cmd_bundle_unbundle),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options, builtin_bundle_usage,
			     0);

	packet_trace_شناختی_عنصر("bundle");

	واپس !!fn(argc, argv, سابقہ, repo);
}
