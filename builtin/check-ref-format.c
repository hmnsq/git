/*
 * GIT - The information manager from hell
 */

#define USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "strbuf.h"

स्थिर स्थिरांक अक्षर builtin_check_ref_format_usage[] =
"git check-ref-format [--normalize] [<options>] <refname>\n"
"   or: git check-ref-format --branch <branchname-shorthand>";

/*
 * Return a copy of refname but with leading slashes removed and runs
 * of adjacent slashes replaced with single slashes.
 *
 * This function is similar to normalize_path_copy(), but stripped down
 * to meet check_ref_format's simpler needs.
 */
स्थिर अक्षर *collapse_slashes(स्थिरांक अक्षर *refname)
{
	अक्षर *ret = xmallocz(म_माप(refname));
	अक्षर ch;
	अक्षर prev = '/';
	अक्षर *cp = ret;

	पर्यंत ((ch = *refname++) != '\0') {
		जर (prev == '/' && ch == prev)
			सुरूठेवा;

		*cp++ = ch;
		prev = ch;
	}
	*cp = '\0';
	परत ret;
}

स्थिर पूर्णांक check_ref_format_branch(स्थिरांक अक्षर *arg)
{
	संरचना strbuf sb = STRBUF_INIT;
	स्थिरांक अक्षर *name;
	पूर्णांक nongit;

	setup_git_directory_gently(the_repository, &nongit);
	जर (check_branch_ref(the_repository, &sb, arg) ||
	    !skip_prefix(sb.buf, "refs/heads/", &name))
		die("'%s' is not a valid branch name", arg);
	म_लिखो("%s\n", name);
	strbuf_release(&sb);
	परत 0;
}

पूर्णांक cmd_check_ref_format(पूर्णांक argc,
			 स्थिरांक अक्षर **argv,
			 स्थिरांक अक्षर *prefix,
			 संरचना repository *repo UNUSED)
{
	पूर्णांक i;
	पूर्णांक normalize = 0;
	पूर्णांक flags = 0;
	स्थिरांक अक्षर *refname;
	अक्षर *to_free = रिक्त;
	पूर्णांक ret = 1;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv,
			    builtin_check_ref_format_usage);

	जर (argc == 3 && !म_भेद(argv[1], "--branch"))
		परत check_ref_format_branch(argv[2]);

	साठी (i = 1; i < argc && argv[i][0] == '-'; i++) {
		जर (!म_भेद(argv[i], "--normalize") || !म_भेद(argv[i], "--print"))
			normalize = 1;
		अन्यथा जर (!म_भेद(argv[i], "--allow-onelevel"))
			flags |= REFNAME_ALLOW_ONELEVEL;
		अन्यथा जर (!म_भेद(argv[i], "--no-allow-onelevel"))
			flags &= ~REFNAME_ALLOW_ONELEVEL;
		अन्यथा जर (!म_भेद(argv[i], "--refspec-pattern"))
			flags |= REFNAME_REFSPEC_PATTERN;
		अन्यथा
			usage(builtin_check_ref_format_usage);
	}
	जर (! (i == argc - 1))
		usage(builtin_check_ref_format_usage);

	refname = argv[i];
	जर (normalize)
		refname = to_free = collapse_slashes(refname);
	जर (check_refname_format(refname, flags))
		जा cleanup;
	जर (normalize)
		म_लिखो("%s\n", refname);

	ret = 0;
cleanup:
	मुक्त(to_free);
	परत ret;
}
