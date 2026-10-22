#define USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "strbuf.h"

स्थिर ध्रुव वर्ण * ध्रुव git_symbolic_ref_usage[] = {
	N_("git symbolic-ref [-m <reason>] <name> <ref>"),
	N_("git symbolic-ref [-q] [--short] [--no-recurse] <name>"),
	N_("git symbolic-ref --delete [-q] <name>"),
	रिक्त
};

स्थिर पूर्णाङ्क check_symref(ध्रुव वर्ण *HEAD, पूर्णाङ्क quiet, पूर्णाङ्क shorten, पूर्णाङ्क recurse, पूर्णाङ्क print)
{
	पूर्णाङ्क resolve_flags, flag;
	ध्रुव वर्ण *refname;

	resolve_flags = (recurse ? 0 : RESOLVE_REF_NO_RECURSE);
	refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					  HEAD, resolve_flags, रिक्त, &flag);

	जइ (!refname)
		die("No such ref: %s", HEAD);
	अण्णहा जइ (!(flag & REF_ISSYMREF)) {
		जइ (!quiet)
			die("ref %s is not a symbolic ref", HEAD);
		अण्णहा
			प्रत्यागम 1;
	}
	जइ (print) {
		वर्ण *to_free = रिक्त;
		जइ (shorten)
			refname = to_free = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									 refname,
									 0);
		puts(refname);
		मुक्त(to_free);
	}
	प्रत्यागम 0;
}

पूर्णाङ्क cmd_symbolic_ref(पूर्णाङ्क argc,
		     ध्रुव वर्ण **argv,
		     ध्रुव वर्ण *prefix,
		     संरचना repository *repo UNUSED)
{
	पूर्णाङ्क quiet = 0, delete = 0, shorten = 0, recurse = 1, ret = 0;
	ध्रुव वर्ण *सन्देशः = रिक्त;
	संरचना option options[] = {
		OPT__QUIET(&quiet,
			N_("suppress error message for non-symbolic (detached) refs")),
		OPT_BOOL('d', "delete", &delete, N_("delete symbolic ref")),
		OPT_BOOL(0, "short", &shorten, N_("shorten ref output")),
		OPT_BOOL(0, "recurse", &recurse, N_("recursively dereference (default)")),
		OPT_STRING('m', रिक्त, &सन्देशः, N_("reason"), N_("reason of the update")),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, रिक्त);
	argc = parse_options(argc, argv, prefix, options,
			     git_symbolic_ref_usage, 0);
	जइ (सन्देशः && !*सन्देशः)
		die("Refusing to perform update with empty message");

	जइ (delete) {
		जइ (argc != 1)
			usage_with_options(git_symbolic_ref_usage, options);
		ret = check_symref(argv[0], 1, 0, 0, 0);
		जइ (ret)
			die("Cannot delete %s, not a symbolic ref", argv[0]);
		जइ (!strcmp(argv[0], "HEAD"))
			die("deleting '%s' is not allowed", argv[0]);
		प्रत्यागम refs_delete_ref(get_main_ref_store(the_repository),
				       रिक्त, argv[0], रिक्त, REF_NO_DEREF);
	}

	विकल्प (argc) {
	पक्ष 1:
		ret = check_symref(argv[0], quiet, shorten, recurse, 1);
		भङ्ग;
	पक्ष 2:
		जइ (!strcmp(argv[0], "HEAD") &&
		    !starts_with(argv[1], "refs/"))
			die("Refusing to point HEAD outside of refs/");
		जइ (check_refname_format(argv[1], REFNAME_ALLOW_ONELEVEL) < 0)
			die("Refusing to set '%s' to invalid ref '%s'", argv[0], argv[1]);
		ret = !!refs_update_symref(get_main_ref_store(the_repository),
					   argv[0], argv[1], सन्देशः);
		भङ्ग;
	स्वभाव:
		usage_with_options(git_symbolic_ref_usage, options);
	}
	प्रत्यागम ret;
}
