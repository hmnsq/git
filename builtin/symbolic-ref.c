#define USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "strbuf.h"

स्थिर स्थिरांक अक्षर * स्थिरांक git_symbolic_ref_usage[] = {
	N_("git symbolic-ref [-m <reason>] <name> <ref>"),
	N_("git symbolic-ref [-q] [--short] [--no-recurse] <name>"),
	N_("git symbolic-ref --delete [-q] <name>"),
	रिक्त
};

स्थिर पूर्णांक check_symref(स्थिरांक अक्षर *HEAD, पूर्णांक quiet, पूर्णांक shorten, पूर्णांक recurse, पूर्णांक print)
{
	पूर्णांक resolve_flags, flag;
	स्थिरांक अक्षर *refname;

	resolve_flags = (recurse ? 0 : RESOLVE_REF_NO_RECURSE);
	refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					  HEAD, resolve_flags, रिक्त, &flag);

	जर (!refname)
		die("No such ref: %s", HEAD);
	अन्यथा जर (!(flag & REF_ISSYMREF)) {
		जर (!quiet)
			die("ref %s is not a symbolic ref", HEAD);
		अन्यथा
			परत 1;
	}
	जर (print) {
		अक्षर *to_free = रिक्त;
		जर (shorten)
			refname = to_free = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									 refname,
									 0);
		माला_दो(refname);
		मुक्त(to_free);
	}
	परत 0;
}

पूर्णांक cmd_symbolic_ref(पूर्णांक argc,
		     स्थिरांक अक्षर **argv,
		     स्थिरांक अक्षर *prefix,
		     संरचना repository *repo UNUSED)
{
	पूर्णांक quiet = 0, delete = 0, shorten = 0, recurse = 1, ret = 0;
	स्थिरांक अक्षर *संदेश = रिक्त;
	संरचना option options[] = {
		OPT__QUIET(&quiet,
			N_("suppress error message for non-symbolic (detached) refs")),
		OPT_BOOL('d', "delete", &delete, N_("delete symbolic ref")),
		OPT_BOOL(0, "short", &shorten, N_("shorten ref output")),
		OPT_BOOL(0, "recurse", &recurse, N_("recursively dereference (default)")),
		OPT_STRING('m', रिक्त, &संदेश, N_("reason"), N_("reason of the update")),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, रिक्त);
	argc = parse_options(argc, argv, prefix, options,
			     git_symbolic_ref_usage, 0);
	जर (संदेश && !*संदेश)
		die("Refusing to perform update with empty message");

	जर (delete) {
		जर (argc != 1)
			usage_with_options(git_symbolic_ref_usage, options);
		ret = check_symref(argv[0], 1, 0, 0, 0);
		जर (ret)
			die("Cannot delete %s, not a symbolic ref", argv[0]);
		जर (!म_भेद(argv[0], "HEAD"))
			die("deleting '%s' is not allowed", argv[0]);
		परत refs_delete_ref(get_main_ref_store(the_repository),
				       रिक्त, argv[0], रिक्त, REF_NO_DEREF);
	}

	निवड (argc) {
	स्थिती 1:
		ret = check_symref(argv[0], quiet, shorten, recurse, 1);
		थांबा;
	स्थिती 2:
		जर (!म_भेद(argv[0], "HEAD") &&
		    !starts_with(argv[1], "refs/"))
			die("Refusing to point HEAD outside of refs/");
		जर (check_refname_format(argv[1], REFNAME_ALLOW_ONELEVEL) < 0)
			die("Refusing to set '%s' to invalid ref '%s'", argv[0], argv[1]);
		ret = !!refs_update_symref(get_main_ref_store(the_repository),
					   argv[0], argv[1], संदेश);
		थांबा;
	पूर्वनिर्धारित:
		usage_with_options(git_symbolic_ref_usage, options);
	}
	परत ret;
}
