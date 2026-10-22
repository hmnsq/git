#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "strbuf.h"

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_symbolic_ref_usage[] = {
	N_("git symbolic-ref [-m <reason>] <name> <ref>"),
	N_("git symbolic-ref [-q] [--short] [--no-recurse] <name>"),
	N_("git symbolic-ref --delete [-q] <name>"),
	ফাঁকা
};

স্থির পূর্ণসংখ্যা check_symref(ধ্রুবক অক্ষর *শীর্ষ_3, পূর্ণসংখ্যা quiet, পূর্ণসংখ্যা shorten, পূর্ণসংখ্যা recurse, পূর্ণসংখ্যা print)
{
	পূর্ণসংখ্যা resolve_flags, flag;
	ধ্রুবক অক্ষর *refname;

	resolve_flags = (recurse ? 0 : RESOLVE_REF_NO_RECURSE);
	refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					  শীর্ষ_3, resolve_flags, ফাঁকা, &flag);

	যদি (!refname)
		die("No such ref: %s", শীর্ষ_3);
	অন্যথা যদি (!(flag & REF_ISSYMREF)) {
		যদি (!quiet)
			die("ref %s is not a symbolic ref", শীর্ষ_3);
		অন্যথা
			ফেরত 1;
	}
	যদি (print) {
		অক্ষর *to_free = ফাঁকা;
		যদি (shorten)
			refname = to_free = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									 refname,
									 0);
		puts(refname);
		মুক্ত(to_free);
	}
	ফেরত 0;
}

পূর্ণসংখ্যা cmd_symbolic_ref(পূর্ণসংখ্যা argc,
		     ধ্রুবক অক্ষর **argv,
		     ধ্রুবক অক্ষর *prefix,
		     কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা quiet = 0, delete = 0, shorten = 0, recurse = 1, ret = 0;
	ধ্রুবক অক্ষর *বার্তা = ফাঁকা;
	কাঠামো option options[] = {
		OPT__QUIET(&quiet,
			N_("suppress error message for non-symbolic (detached) refs")),
		OPT_BOOL('d', "delete", &delete, N_("delete symbolic ref")),
		OPT_BOOL(0, "short", &shorten, N_("shorten ref output")),
		OPT_BOOL(0, "recurse", &recurse, N_("recursively dereference (default)")),
		OPT_STRING('m', ফাঁকা, &বার্তা, N_("reason"), N_("reason of the update")),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, ফাঁকা);
	argc = parse_options(argc, argv, prefix, options,
			     git_symbolic_ref_usage, 0);
	যদি (বার্তা && !*বার্তা)
		die("Refusing to perform update with empty message");

	যদি (delete) {
		যদি (argc != 1)
			usage_with_options(git_symbolic_ref_usage, options);
		ret = check_symref(argv[0], 1, 0, 0, 0);
		যদি (ret)
			die("Cannot delete %s, not a symbolic ref", argv[0]);
		যদি (!strcmp(argv[0], "HEAD"))
			die("deleting '%s' is not allowed", argv[0]);
		ফেরত refs_delete_ref(get_main_ref_store(the_repository),
				       ফাঁকা, argv[0], ফাঁকা, REF_NO_DEREF);
	}

	নির্বাচন (argc) {
	ক্ষেত্র 1:
		ret = check_symref(argv[0], quiet, shorten, recurse, 1);
		বিরতি;
	ক্ষেত্র 2:
		যদি (!strcmp(argv[0], "HEAD") &&
		    !starts_with(argv[1], "refs/"))
			die("Refusing to point HEAD outside of refs/");
		যদি (check_refname_format(argv[1], REFNAME_ALLOW_ONELEVEL) < 0)
			die("Refusing to set '%s' to invalid ref '%s'", argv[0], argv[1]);
		ret = !!refs_update_symref(get_main_ref_store(the_repository),
					   argv[0], argv[1], বার্তা);
		বিরতি;
	পূর্বনির্ধারিত:
		usage_with_options(git_symbolic_ref_usage, options);
	}
	ফেরত ret;
}
