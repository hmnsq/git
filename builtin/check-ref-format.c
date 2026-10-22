/*
 * GIT - The information manager from hell
 */

#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "strbuf.h"

સ્થિર અચલ અક્ષર builtin_check_ref_format_usage[] =
"git check-ref-format [--normalize] [<options>] <refname>\n"
"   or: git check-ref-format --branch <branchname-shorthand>";

/*
 * Return a copy of refname but with leading slashes removed and runs
 * of adjacent slashes replaced with single slashes.
 *
 * This function is similar to normalize_path_copy(), but stripped down
 * to meet check_ref_format's simpler needs.
 */
સ્થિર અક્ષર *collapse_slashes(અચલ અક્ષર *refname)
{
	અક્ષર *ret = xmallocz(strlen(refname));
	અક્ષર ch;
	અક્ષર prev = '/';
	અક્ષર *cp = ret;

	જ્યારે ((ch = *refname++) != '\0') {
		જો (prev == '/' && ch == prev)
			ચાલુ;

		*cp++ = ch;
		prev = ch;
	}
	*cp = '\0';
	પાછા ret;
}

સ્થિર પૂર્ણાંક check_ref_format_branch(અચલ અક્ષર *arg)
{
	સંરચના strbuf sb = STRBUF_INIT;
	અચલ અક્ષર *name;
	પૂર્ણાંક nongit;

	setup_git_directory_gently(the_repository, &nongit);
	જો (check_branch_ref(the_repository, &sb, arg) ||
	    !skip_prefix(sb.buf, "refs/heads/", &name))
		die("'%s' is not a valid branch name", arg);
	printf("%s\n", name);
	strbuf_release(&sb);
	પાછા 0;
}

પૂર્ણાંક cmd_check_ref_format(પૂર્ણાંક argc,
			 અચલ અક્ષર **argv,
			 અચલ અક્ષર *prefix,
			 સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક i;
	પૂર્ણાંક normalize = 0;
	પૂર્ણાંક flags = 0;
	અચલ અક્ષર *refname;
	અક્ષર *to_free = ખાલી;
	પૂર્ણાંક ret = 1;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv,
			    builtin_check_ref_format_usage);

	જો (argc == 3 && !strcmp(argv[1], "--branch"))
		પાછા check_ref_format_branch(argv[2]);

	માટે (i = 1; i < argc && argv[i][0] == '-'; i++) {
		જો (!strcmp(argv[i], "--normalize") || !strcmp(argv[i], "--print"))
			normalize = 1;
		નહીંતર જો (!strcmp(argv[i], "--allow-onelevel"))
			flags |= REFNAME_ALLOW_ONELEVEL;
		નહીંતર જો (!strcmp(argv[i], "--no-allow-onelevel"))
			flags &= ~REFNAME_ALLOW_ONELEVEL;
		નહીંતર જો (!strcmp(argv[i], "--refspec-pattern"))
			flags |= REFNAME_REFSPEC_PATTERN;
		નહીંતર
			usage(builtin_check_ref_format_usage);
	}
	જો (! (i == argc - 1))
		usage(builtin_check_ref_format_usage);

	refname = argv[i];
	જો (normalize)
		refname = to_free = collapse_slashes(refname);
	જો (check_refname_format(refname, flags))
		જાઓ cleanup;
	જો (normalize)
		printf("%s\n", refname);

	ret = 0;
cleanup:
	મુક્ત(to_free);
	પાછા ret;
}
