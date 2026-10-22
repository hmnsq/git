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

	နေစဉ် ((ch = *refname++) != '\0') {
		အကယ်၍ (prev == '/' && ch == prev)
			continue;

		*cp++ = ch;
		prev = ch;
	}
	*cp = '\0';
	ပြန်ပေး ret;
}

static ကိန်းပြည့် check_ref_format_branch(const var_na *arg)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	const var_na *name;
	ကိန်းပြည့် nongit;

	setup_git_directory_gently(the_repository, &nongit);
	အကယ်၍ (check_branch_ref(the_repository, &sb, arg) ||
	    !skip_prefix(sb.buf, "refs/heads/", &name))
		die("'%s' is not a valid branch name", arg);
	printf("%s\n", name);
	strbuf_release(&sb);
	ပြန်ပေး 0;
}

ကိန်းပြည့် cmd_check_ref_format(ကိန်းပြည့် argc,
			 const var_na **argv,
			 const var_na *prefix,
			 sa_nracanaa repository *repo UNUSED)
{
	ကိန်းပြည့် i;
	ကိန်းပြည့် normalize = 0;
	ကိန်းပြည့် flags = 0;
	const var_na *refname;
	var_na *to_free = NULL;
	ကိန်းပြည့် ret = 1;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv,
			    builtin_check_ref_format_usage);

	အကယ်၍ (argc == 3 && !strcmp(argv[1], "--branch"))
		ပြန်ပေး check_ref_format_branch(argv[2]);

	အတွက် (i = 1; i < argc && argv[i][0] == '-'; i++) {
		အကယ်၍ (!strcmp(argv[i], "--normalize") || !strcmp(argv[i], "--print"))
			normalize = 1;
		မဟုတ်ပါက အကယ်၍ (!strcmp(argv[i], "--allow-onelevel"))
			flags |= REFNAME_ALLOW_ONELEVEL;
		မဟုတ်ပါက အကယ်၍ (!strcmp(argv[i], "--no-allow-onelevel"))
			flags &= ~REFNAME_ALLOW_ONELEVEL;
		မဟုတ်ပါက အကယ်၍ (!strcmp(argv[i], "--refspec-pattern"))
			flags |= REFNAME_REFSPEC_PATTERN;
		မဟုတ်ပါက
			usage(builtin_check_ref_format_usage);
	}
	အကယ်၍ (! (i == argc - 1))
		usage(builtin_check_ref_format_usage);

	refname = argv[i];
	အကယ်၍ (normalize)
		refname = to_free = collapse_slashes(refname);
	အကယ်၍ (check_refname_format(refname, flags))
		goto cleanup;
	အကယ်၍ (normalize)
		printf("%s\n", refname);

	ret = 0;
cleanup:
	free(to_free);
	ပြန်ပေး ret;
}
