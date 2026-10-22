#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "refs.h"
#समावेश "parse-options.h"
#समावेश "strbuf.h"

अटल स्थिर अक्षर * स्थिर git_symbolic_ref_usage[] = {
	N_("git symbolic-ref [-m <reason>] <name> <ref>"),
	N_("git symbolic-ref [-q] [--short] [--no-recurse] <name>"),
	N_("git symbolic-ref --delete [-q] <name>"),
	शून्य
};

अटल पूर्णांक check_symref(स्थिर अक्षर *शीर्ष_3, पूर्णांक quiet, पूर्णांक shorten, पूर्णांक recurse, पूर्णांक print)
{
	पूर्णांक resolve_flags, flag;
	स्थिर अक्षर *refname;

	resolve_flags = (recurse ? 0 : RESOLVE_REF_NO_RECURSE);
	refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					  शीर्ष_3, resolve_flags, शून्य, &flag);

	अगर (!refname)
		die("No such ref: %s", शीर्ष_3);
	अन्यथा अगर (!(flag & REF_ISSYMREF)) {
		अगर (!quiet)
			die("ref %s is not a symbolic ref", शीर्ष_3);
		अन्यथा
			वापस 1;
	}
	अगर (print) {
		अक्षर *to_free = शून्य;
		अगर (shorten)
			refname = to_free = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									 refname,
									 0);
		माला_दो(refname);
		मुक्त(to_free);
	}
	वापस 0;
}

पूर्णांक cmd_symbolic_ref(पूर्णांक argc,
		     स्थिर अक्षर **argv,
		     स्थिर अक्षर *prefix,
		     काष्ठा repository *repo UNUSED)
{
	पूर्णांक quiet = 0, delete = 0, shorten = 0, recurse = 1, ret = 0;
	स्थिर अक्षर *संदेश = शून्य;
	काष्ठा option options[] = {
		OPT__QUIET(&quiet,
			N_("suppress error message for non-symbolic (detached) refs")),
		OPT_BOOL('d', "delete", &delete, N_("delete symbolic ref")),
		OPT_BOOL(0, "short", &shorten, N_("shorten ref output")),
		OPT_BOOL(0, "recurse", &recurse, N_("recursively dereference (default)")),
		OPT_STRING('m', शून्य, &संदेश, N_("reason"), N_("reason of the update")),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, शून्य);
	argc = parse_options(argc, argv, prefix, options,
			     git_symbolic_ref_usage, 0);
	अगर (संदेश && !*संदेश)
		die("Refusing to perform update with empty message");

	अगर (delete) {
		अगर (argc != 1)
			usage_with_options(git_symbolic_ref_usage, options);
		ret = check_symref(argv[0], 1, 0, 0, 0);
		अगर (ret)
			die("Cannot delete %s, not a symbolic ref", argv[0]);
		अगर (!म_भेद(argv[0], "HEAD"))
			die("deleting '%s' is not allowed", argv[0]);
		वापस refs_delete_ref(get_main_ref_store(the_repository),
				       शून्य, argv[0], शून्य, REF_NO_DEREF);
	}

	चयन (argc) {
	हाल 1:
		ret = check_symref(argv[0], quiet, shorten, recurse, 1);
		अवरोध;
	हाल 2:
		अगर (!म_भेद(argv[0], "HEAD") &&
		    !starts_with(argv[1], "refs/"))
			die("Refusing to point HEAD outside of refs/");
		अगर (check_refname_format(argv[1], REFNAME_ALLOW_ONELEVEL) < 0)
			die("Refusing to set '%s' to invalid ref '%s'", argv[0], argv[1]);
		ret = !!refs_update_symref(get_main_ref_store(the_repository),
					   argv[0], argv[1], संदेश);
		अवरोध;
	शेष:
		usage_with_options(git_symbolic_ref_usage, options);
	}
	वापस ret;
}
