/*
 * GIT - The information manager from hell
 */

#define USE_THE_REPOSITORY_VARIABLE

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "strbuf.h"

நிலையான மாறிலி எழுத்து builtin_check_ref_format_usage[] =
"git check-ref-format [--normalize] [<options>] <refname>\n"
"   or: git check-ref-format --branch <branchname-shorthand>";

/*
 * Return a copy of refname but with leading slashes removed and runs
 * of adjacent slashes replaced with single slashes.
 *
 * This function is similar to normalize_path_copy(), but stripped down
 * to meet check_ref_format's simpler needs.
 */
நிலையான எழுத்து *collapse_slashes(மாறிலி எழுத்து *refname)
{
	எழுத்து *ret = xmallocz(strlen(refname));
	எழுத்து ch;
	எழுத்து prev = '/';
	எழுத்து *cp = ret;

	போது ((ch = *refname++) != '\0') {
		எனில் (prev == '/' && ch == prev)
			தொடர்;

		*cp++ = ch;
		prev = ch;
	}
	*cp = '\0';
	திரும்பு ret;
}

நிலையான முழு check_ref_format_branch(மாறிலி எழுத்து *arg)
{
	கட்டமைப்பு strbuf sb = STRBUF_INIT;
	மாறிலி எழுத்து *name;
	முழு nongit;

	setup_git_directory_gently(the_repository, &nongit);
	எனில் (check_branch_ref(the_repository, &sb, arg) ||
	    !skip_prefix(sb.buf, "refs/heads/", &name))
		die("'%s' is not a valid branch name", arg);
	printf("%s\n", name);
	strbuf_release(&sb);
	திரும்பு 0;
}

முழு cmd_check_ref_format(முழு argc,
			 மாறிலி எழுத்து **argv,
			 மாறிலி எழுத்து *prefix,
			 கட்டமைப்பு repository *repo UNUSED)
{
	முழு i;
	முழு normalize = 0;
	முழு flags = 0;
	மாறிலி எழுத்து *refname;
	எழுத்து *to_free = வெற்று;
	முழு ret = 1;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv,
			    builtin_check_ref_format_usage);

	எனில் (argc == 3 && !strcmp(argv[1], "--branch"))
		திரும்பு check_ref_format_branch(argv[2]);

	க்கு (i = 1; i < argc && argv[i][0] == '-'; i++) {
		எனில் (!strcmp(argv[i], "--normalize") || !strcmp(argv[i], "--print"))
			normalize = 1;
		இல்லையெனில் எனில் (!strcmp(argv[i], "--allow-onelevel"))
			flags |= REFNAME_ALLOW_ONELEVEL;
		இல்லையெனில் எனில் (!strcmp(argv[i], "--no-allow-onelevel"))
			flags &= ~REFNAME_ALLOW_ONELEVEL;
		இல்லையெனில் எனில் (!strcmp(argv[i], "--refspec-pattern"))
			flags |= REFNAME_REFSPEC_PATTERN;
		இல்லையெனில்
			usage(builtin_check_ref_format_usage);
	}
	எனில் (! (i == argc - 1))
		usage(builtin_check_ref_format_usage);

	refname = argv[i];
	எனில் (normalize)
		refname = to_free = collapse_slashes(refname);
	எனில் (check_refname_format(refname, flags))
		செல் cleanup;
	எனில் (normalize)
		printf("%s\n", refname);

	ret = 0;
cleanup:
	விடுவி(to_free);
	திரும்பு ret;
}
