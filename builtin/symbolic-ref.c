#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "refs.h"
#ضمن "parse-options.h"
#ضمن "strbuf.h"

ساكن ثابت حرف * ثابت git_symbolic_ref_usage[] = {
	N_("git symbolic-ref [-m <reason>] <name> <ref>"),
	N_("git symbolic-ref [-q] [--short] [--no-recurse] <name>"),
	N_("git symbolic-ref --delete [-q] <name>"),
	عدم
};

ساكن صحيح check_symref(ثابت حرف *سر_2, صحيح quiet, صحيح shorten, صحيح recurse, صحيح print)
{
	صحيح resolve_flags, flag;
	ثابت حرف *refname;

	resolve_flags = (recurse ? 0 : RESOLVE_REF_NO_RECURSE);
	refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					  سر_2, resolve_flags, عدم, &flag);

	إذا (!refname)
		die("No such ref: %s", سر_2);
	وإلا إذا (!(flag & REF_ISSYMREF)) {
		إذا (!quiet)
			die("ref %s is not a symbolic ref", سر_2);
		وإلا
			أرجع 1;
	}
	إذا (print) {
		حرف *منزل_free = عدم;
		إذا (shorten)
			refname = منزل_free = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									 refname,
									 0);
		puts(refname);
		حرر(منزل_free);
	}
	أرجع 0;
}

صحيح cmd_symbolic_ref(صحيح argc,
		     ثابت حرف **argv,
		     ثابت حرف *اڳياڙي,
		     بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح quiet = 0, delete = 0, shorten = 0, recurse = 1, ret = 0;
	ثابت حرف *نياپو = عدم;
	بنية option options[] = {
		OPT__QUIET(&quiet,
			N_("suppress error message for non-symbolic (detached) refs")),
		OPT_BOOL('d', "delete", &delete, N_("delete symbolic ref")),
		OPT_BOOL(0, "short", &shorten, N_("shorten ref output")),
		OPT_BOOL(0, "recurse", &recurse, N_("recursively dereference (default)")),
		OPT_STRING('m', عدم, &نياپو, N_("reason"), N_("reason of the update")),
		OPT_پڇاڙي(),
	};

	repo_config(the_repository, git_default_config, عدم);
	argc = parse_options(argc, argv, اڳياڙي, options,
			     git_symbolic_ref_usage, 0);
	إذا (نياپو && !*نياپو)
		die("Refusing to perform update with empty message");

	إذا (delete) {
		إذا (argc != 1)
			usage_with_options(git_symbolic_ref_usage, options);
		ret = check_symref(argv[0], 1, 0, 0, 0);
		إذا (ret)
			die("Cannot delete %s, not a symbolic ref", argv[0]);
		إذا (!strcmp(argv[0], "HEAD"))
			die("deleting '%s' is not allowed", argv[0]);
		أرجع refs_delete_ref(get_main_ref_store(the_repository),
				       عدم, argv[0], عدم, REF_NO_DEREF);
	}

	بدل (argc) {
	حالة 1:
		ret = check_symref(argv[0], quiet, shorten, recurse, 1);
		اكسر;
	حالة 2:
		إذا (!strcmp(argv[0], "HEAD") &&
		    !starts_with(argv[1], "refs/"))
			die("Refusing to point HEAD outside of refs/");
		إذا (check_refname_format(argv[1], REFNAME_ALLOW_ONELEVEL) < 0)
			die("Refusing to set '%s' to invalid ref '%s'", argv[0], argv[1]);
		ret = !!refs_update_symref(get_main_ref_store(the_repository),
					   argv[0], argv[1], نياپو);
		اكسر;
	افتراضي:
		usage_with_options(git_symbolic_ref_usage, options);
	}
	أرجع ret;
}
