/*
 * GIT - The information manager from hell
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "builtin.h"
#समावेश "refs.h"
#समावेश "setup.h"
#समावेश "strbuf.h"

अटल स्थिर अक्षर builtin_check_ref_format_usage[] =
"git check-ref-format [--normalize] [<options>] <refname>\n"
"   or: git check-ref-format --branch <branchname-shorthand>";

/*
 * Return a copy of refname but with leading slashes removed and runs
 * of adjacent slashes replaced with single slashes.
 *
 * This function is similar to normalize_path_copy(), but stripped down
 * to meet check_ref_format's simpler needs.
 */
अटल अक्षर *collapse_slashes(स्थिर अक्षर *refname)
{
	अक्षर *ret = xmallocz(म_माप(refname));
	अक्षर ch;
	अक्षर पूर्व = '/';
	अक्षर *cp = ret;

	जबतक ((ch = *refname++) != '\0') {
		अगर (पूर्व == '/' && ch == पूर्व)
			जारी;

		*cp++ = ch;
		पूर्व = ch;
	}
	*cp = '\0';
	वापस ret;
}

अटल पूर्णांक check_ref_format_branch(स्थिर अक्षर *arg)
{
	काष्ठा strbuf sb = STRBUF_INIT;
	स्थिर अक्षर *name;
	पूर्णांक nongit;

	setup_git_directory_gently(the_repository, &nongit);
	अगर (check_branch_ref(the_repository, &sb, arg) ||
	    !skip_prefix(sb.buf, "refs/heads/", &name))
		die("'%s' is not a valid branch name", arg);
	म_लिखो("%s\n", name);
	strbuf_release(&sb);
	वापस 0;
}

पूर्णांक cmd_check_ref_format(पूर्णांक argc,
			 स्थिर अक्षर **argv,
			 स्थिर अक्षर *prefix,
			 काष्ठा repository *repo UNUSED)
{
	पूर्णांक i;
	पूर्णांक normalize = 0;
	पूर्णांक flags = 0;
	स्थिर अक्षर *refname;
	अक्षर *to_free = शून्य;
	पूर्णांक ret = 1;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv,
			    builtin_check_ref_format_usage);

	अगर (argc == 3 && !म_भेद(argv[1], "--branch"))
		वापस check_ref_format_branch(argv[2]);

	क्रम (i = 1; i < argc && argv[i][0] == '-'; i++) {
		अगर (!म_भेद(argv[i], "--normalize") || !म_भेद(argv[i], "--print"))
			normalize = 1;
		अन्यथा अगर (!म_भेद(argv[i], "--allow-onelevel"))
			flags |= REFNAME_ALLOW_ONELEVEL;
		अन्यथा अगर (!म_भेद(argv[i], "--no-allow-onelevel"))
			flags &= ~REFNAME_ALLOW_ONELEVEL;
		अन्यथा अगर (!म_भेद(argv[i], "--refspec-pattern"))
			flags |= REFNAME_REFSPEC_PATTERN;
		अन्यथा
			usage(builtin_check_ref_format_usage);
	}
	अगर (! (i == argc - 1))
		usage(builtin_check_ref_format_usage);

	refname = argv[i];
	अगर (normalize)
		refname = to_free = collapse_slashes(refname);
	अगर (check_refname_format(refname, flags))
		जाओ cleanup;
	अगर (normalize)
		म_लिखो("%s\n", refname);

	ret = 0;
cleanup:
	मुक्त(to_free);
	वापस ret;
}
