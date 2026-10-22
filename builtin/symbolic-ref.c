#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "strbuf.h"

স্থির ধ্রুব বর্ণ * ধ্রুব git_symbolic_ref_usage[] = {
	N_("git symbolic-ref [-m <reason>] <name> <ref>"),
	N_("git symbolic-ref [-q] [--short] [--no-recurse] <name>"),
	N_("git symbolic-ref --delete [-q] <name>"),
	রিক্ত
};

স্থির পূর্ণাঙ্ক check_symref(ধ্রুব বর্ণ *HEAD, পূর্ণাঙ্ক quiet, পূর্ণাঙ্ক shorten, পূর্ণাঙ্ক recurse, পূর্ণাঙ্ক print)
{
	পূর্ণাঙ্ক resolve_flags, flag;
	ধ্রুব বর্ণ *refname;

	resolve_flags = (recurse ? 0 : RESOLVE_REF_NO_RECURSE);
	refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					  HEAD, resolve_flags, রিক্ত, &flag);

	যদি (!refname)
		die("No such ref: %s", HEAD);
	অন্যথা যদি (!(flag & REF_ISSYMREF)) {
		যদি (!quiet)
			die("ref %s is not a symbolic ref", HEAD);
		অন্যথা
			প্রত্যাগম 1;
	}
	যদি (print) {
		বর্ণ *to_free = রিক্ত;
		যদি (shorten)
			refname = to_free = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									 refname,
									 0);
		puts(refname);
		মুক্ত(to_free);
	}
	প্রত্যাগম 0;
}

পূর্ণাঙ্ক cmd_symbolic_ref(পূর্ণাঙ্ক argc,
		     ধ্রুব বর্ণ **argv,
		     ধ্রুব বর্ণ *prefix,
		     সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক quiet = 0, delete = 0, shorten = 0, recurse = 1, ret = 0;
	ধ্রুব বর্ণ *বার্তা = রিক্ত;
	সংরচনা option options[] = {
		OPT__QUIET(&quiet,
			N_("suppress error message for non-symbolic (detached) refs")),
		OPT_BOOL('d', "delete", &delete, N_("delete symbolic ref")),
		OPT_BOOL(0, "short", &shorten, N_("shorten ref output")),
		OPT_BOOL(0, "recurse", &recurse, N_("recursively dereference (default)")),
		OPT_STRING('m', রিক্ত, &বার্তা, N_("reason"), N_("reason of the update")),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, রিক্ত);
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
		প্রত্যাগম refs_delete_ref(get_main_ref_store(the_repository),
				       রিক্ত, argv[0], রিক্ত, REF_NO_DEREF);
	}

	বিকল্প (argc) {
	পক্ষ 1:
		ret = check_symref(argv[0], quiet, shorten, recurse, 1);
		ভঙ্গ;
	পক্ষ 2:
		যদি (!strcmp(argv[0], "HEAD") &&
		    !starts_with(argv[1], "refs/"))
			die("Refusing to point HEAD outside of refs/");
		যদি (check_refname_format(argv[1], REFNAME_ALLOW_ONELEVEL) < 0)
			die("Refusing to set '%s' to invalid ref '%s'", argv[0], argv[1]);
		ret = !!refs_update_symref(get_main_ref_store(the_repository),
					   argv[0], argv[1], বার্তা);
		ভঙ্গ;
	স্বভাব:
		usage_with_options(git_symbolic_ref_usage, options);
	}
	প্রত্যাগম ret;
}
