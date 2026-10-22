#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "refs.h"
#شامل "parse-options.h"
#شامل "strbuf.h"

ایستا ثابت نویسه * ثابت git_symbolic_ref_usage[] = {
	N_("git symbolic-ref [-m <reason>] <name> <ref>"),
	N_("git symbolic-ref [-q] [--short] [--no-recurse] <name>"),
	N_("git symbolic-ref --delete [-q] <name>"),
	پوچ
};

ایستا صحیح check_symref(ثابت نویسه *HEAD, صحیح quiet, صحیح shorten, صحیح recurse, صحیح print)
{
	صحیح resolve_flags, flag;
	ثابت نویسه *refname;

	resolve_flags = (recurse ? 0 : RESOLVE_REF_NO_RECURSE);
	refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					  HEAD, resolve_flags, پوچ, &flag);

	اگر (!refname)
		die("No such ref: %s", HEAD);
	وگرنه اگر (!(flag & REF_ISSYMREF)) {
		اگر (!quiet)
			die("ref %s is not a symbolic ref", HEAD);
		وگرنه
			بازگشت 1;
	}
	اگر (print) {
		نویسه *to_free = پوچ;
		اگر (shorten)
			refname = to_free = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									 refname,
									 0);
		puts(refname);
		آزاد(to_free);
	}
	بازگشت 0;
}

صحیح cmd_symbolic_ref(صحیح argc,
		     ثابت نویسه **argv,
		     ثابت نویسه *prefix,
		     ساختار repository *repo UNUSED)
{
	صحیح quiet = 0, delete = 0, shorten = 0, recurse = 1, ret = 0;
	ثابت نویسه *پیام = پوچ;
	ساختار option options[] = {
		OPT__QUIET(&quiet,
			N_("suppress error message for non-symbolic (detached) refs")),
		OPT_BOOL('d', "delete", &delete, N_("delete symbolic ref")),
		OPT_BOOL(0, "short", &shorten, N_("shorten ref output")),
		OPT_BOOL(0, "recurse", &recurse, N_("recursively dereference (default)")),
		OPT_STRING('m', پوچ, &پیام, N_("reason"), N_("reason of the update")),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, پوچ);
	argc = parse_options(argc, argv, prefix, options,
			     git_symbolic_ref_usage, 0);
	اگر (پیام && !*پیام)
		die("Refusing to perform update with empty message");

	اگر (delete) {
		اگر (argc != 1)
			usage_with_options(git_symbolic_ref_usage, options);
		ret = check_symref(argv[0], 1, 0, 0, 0);
		اگر (ret)
			die("Cannot delete %s, not a symbolic ref", argv[0]);
		اگر (!strcmp(argv[0], "HEAD"))
			die("deleting '%s' is not allowed", argv[0]);
		بازگشت refs_delete_ref(get_main_ref_store(the_repository),
				       پوچ, argv[0], پوچ, REF_NO_DEREF);
	}

	انتخاب (argc) {
	حالت 1:
		ret = check_symref(argv[0], quiet, shorten, recurse, 1);
		بشکن;
	حالت 2:
		اگر (!strcmp(argv[0], "HEAD") &&
		    !starts_with(argv[1], "refs/"))
			die("Refusing to point HEAD outside of refs/");
		اگر (check_refname_format(argv[1], REFNAME_ALLOW_ONELEVEL) < 0)
			die("Refusing to set '%s' to invalid ref '%s'", argv[0], argv[1]);
		ret = !!refs_update_symref(get_main_ref_store(the_repository),
					   argv[0], argv[1], پیام);
		بشکن;
	پیشفرض:
		usage_with_options(git_symbolic_ref_usage, options);
	}
	بازگشت ret;
}
