/*
 * GIT - The information manager from hell
 */

#define USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"
#شامل "refs.h"
#شامل "setup.h"
#شامل "strbuf.h"

ایستا ثابت نویسه builtin_check_ref_format_usage[] =
"git check-ref-format [--normalize] [<options>] <refname>\n"
"   or: git check-ref-format --branch <branchname-shorthand>";

/*
 * Return a copy of refname but with leading slashes removed and runs
 * of adjacent slashes replaced with single slashes.
 *
 * This function is similar to normalize_path_copy(), but stripped down
 * to meet check_ref_format's simpler needs.
 */
ایستا نویسه *collapse_slashes(ثابت نویسه *refname)
{
	نویسه *ret = xmallocz(strlen(refname));
	نویسه ch;
	نویسه prev = '/';
	نویسه *cp = ret;

	وقتی ((ch = *refname++) != '\0') {
		اگر (prev == '/' && ch == prev)
			ادامه;

		*cp++ = ch;
		prev = ch;
	}
	*cp = '\0';
	بازگشت ret;
}

ایستا صحیح check_ref_format_branch(ثابت نویسه *arg)
{
	ساختار strbuf sb = STRBUF_INIT;
	ثابت نویسه *name;
	صحیح nongit;

	setup_git_directory_gently(the_repository, &nongit);
	اگر (check_branch_ref(the_repository, &sb, arg) ||
	    !skip_prefix(sb.buf, "refs/heads/", &name))
		die("'%s' is not a valid branch name", arg);
	printf("%s\n", name);
	strbuf_release(&sb);
	بازگشت 0;
}

صحیح cmd_check_ref_format(صحیح argc,
			 ثابت نویسه **argv,
			 ثابت نویسه *prefix,
			 ساختار repository *repo UNUSED)
{
	صحیح i;
	صحیح normalize = 0;
	صحیح flags = 0;
	ثابت نویسه *refname;
	نویسه *to_free = پوچ;
	صحیح ret = 1;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv,
			    builtin_check_ref_format_usage);

	اگر (argc == 3 && !strcmp(argv[1], "--branch"))
		بازگشت check_ref_format_branch(argv[2]);

	برای (i = 1; i < argc && argv[i][0] == '-'; i++) {
		اگر (!strcmp(argv[i], "--normalize") || !strcmp(argv[i], "--print"))
			normalize = 1;
		وگرنه اگر (!strcmp(argv[i], "--allow-onelevel"))
			flags |= REFNAME_ALLOW_ONELEVEL;
		وگرنه اگر (!strcmp(argv[i], "--no-allow-onelevel"))
			flags &= ~REFNAME_ALLOW_ONELEVEL;
		وگرنه اگر (!strcmp(argv[i], "--refspec-pattern"))
			flags |= REFNAME_REFSPEC_PATTERN;
		وگرنه
			usage(builtin_check_ref_format_usage);
	}
	اگر (! (i == argc - 1))
		usage(builtin_check_ref_format_usage);

	refname = argv[i];
	اگر (normalize)
		refname = to_free = collapse_slashes(refname);
	اگر (check_refname_format(refname, flags))
		برو cleanup;
	اگر (normalize)
		printf("%s\n", refname);

	ret = 0;
cleanup:
	آزاد(to_free);
	بازگشت ret;
}
