/*
 * GIT - The information manager from hell
 */

#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"
#include "refs.h"
#include "setup.h"
#include "strbuf.h"

static const var_na builtin_check_ref_format_usage[] =
"git check-ref-format [--normalize] [<options>] <refname>\n"
"   or: git check-ref-format --branch <branchname-shorthand>";

/*
 * Return a copy of refname but with leading slashes removed and runs
 * of adjacent slashes replaced with single slashes.
 *
 * This function is similar to normalize_path_copy(), but stripped down
 * to meet check_ref_format's simpler needs.
 */
static var_na *collapse_slashes(const var_na *refname)
{
	var_na *ret = xmallocz(strlen(refname));
	var_na ch;
	var_na prev = '/';
	var_na *cp = ret;

	ខណៈពេល ((ch = *refname++) != '\0') {
		ប្រសិនបើ (prev == '/' && ch == prev)
			continue;

		*cp++ = ch;
		prev = ch;
	}
	*cp = '\0';
	ត្រឡប់ ret;
}

static ចំនួនគត់ check_ref_format_branch(const var_na *arg)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	const var_na *name;
	ចំនួនគត់ nongit;

	setup_git_directory_gently(the_repository, &nongit);
	ប្រសិនបើ (check_branch_ref(the_repository, &sb, arg) ||
	    !skip_prefix(sb.buf, "refs/heads/", &name))
		die("'%s' is not a valid branch name", arg);
	printf("%s\n", name);
	strbuf_release(&sb);
	ត្រឡប់ 0;
}

ចំនួនគត់ cmd_check_ref_format(ចំនួនគត់ argc,
			 const var_na **argv,
			 const var_na *prefix,
			 sa_nracanaa repository *repo UNUSED)
{
	ចំនួនគត់ i;
	ចំនួនគត់ normalize = 0;
	ចំនួនគត់ flags = 0;
	const var_na *refname;
	var_na *to_free = NULL;
	ចំនួនគត់ ret = 1;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv,
			    builtin_check_ref_format_usage);

	ប្រសិនបើ (argc == 3 && !strcmp(argv[1], "--branch"))
		ត្រឡប់ check_ref_format_branch(argv[2]);

	សម្រាប់ (i = 1; i < argc && argv[i][0] == '-'; i++) {
		ប្រសិនបើ (!strcmp(argv[i], "--normalize") || !strcmp(argv[i], "--print"))
			normalize = 1;
		បើមិនដូច្នេះ ប្រសិនបើ (!strcmp(argv[i], "--allow-onelevel"))
			flags |= REFNAME_ALLOW_ONELEVEL;
		បើមិនដូច្នេះ ប្រសិនបើ (!strcmp(argv[i], "--no-allow-onelevel"))
			flags &= ~REFNAME_ALLOW_ONELEVEL;
		បើមិនដូច្នេះ ប្រសិនបើ (!strcmp(argv[i], "--refspec-pattern"))
			flags |= REFNAME_REFSPEC_PATTERN;
		បើមិនដូច្នេះ
			usage(builtin_check_ref_format_usage);
	}
	ប្រសិនបើ (! (i == argc - 1))
		usage(builtin_check_ref_format_usage);

	refname = argv[i];
	ប្រសិនបើ (normalize)
		refname = to_free = collapse_slashes(refname);
	ប្រសិនបើ (check_refname_format(refname, flags))
		goto cleanup;
	ប្រសិនបើ (normalize)
		printf("%s\n", refname);

	ret = 0;
cleanup:
	free(to_free);
	ត្រឡប់ ret;
}
