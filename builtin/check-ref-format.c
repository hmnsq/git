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

	ขณะ ((ch = *refname++) != '\0') {
		ถ้า (prev == '/' && ch == prev)
			continue;

		*cp++ = ch;
		prev = ch;
	}
	*cp = '\0';
	คืน ret;
}

static จำนวนเต็ม check_ref_format_branch(const var_na *arg)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	const var_na *name;
	จำนวนเต็ม nongit;

	setup_git_directory_gently(the_repository, &nongit);
	ถ้า (check_branch_ref(the_repository, &sb, arg) ||
	    !skip_prefix(sb.buf, "refs/heads/", &name))
		die("'%s' is not a valid branch name", arg);
	printf("%s\n", name);
	strbuf_release(&sb);
	คืน 0;
}

จำนวนเต็ม cmd_check_ref_format(จำนวนเต็ม argc,
			 const var_na **argv,
			 const var_na *prefix,
			 sa_nracanaa repository *repo UNUSED)
{
	จำนวนเต็ม i;
	จำนวนเต็ม normalize = 0;
	จำนวนเต็ม flags = 0;
	const var_na *refname;
	var_na *to_free = NULL;
	จำนวนเต็ม ret = 1;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv,
			    builtin_check_ref_format_usage);

	ถ้า (argc == 3 && !strcmp(argv[1], "--branch"))
		คืน check_ref_format_branch(argv[2]);

	สำหรับ (i = 1; i < argc && argv[i][0] == '-'; i++) {
		ถ้า (!strcmp(argv[i], "--normalize") || !strcmp(argv[i], "--print"))
			normalize = 1;
		มิฉะนั้น ถ้า (!strcmp(argv[i], "--allow-onelevel"))
			flags |= REFNAME_ALLOW_ONELEVEL;
		มิฉะนั้น ถ้า (!strcmp(argv[i], "--no-allow-onelevel"))
			flags &= ~REFNAME_ALLOW_ONELEVEL;
		มิฉะนั้น ถ้า (!strcmp(argv[i], "--refspec-pattern"))
			flags |= REFNAME_REFSPEC_PATTERN;
		มิฉะนั้น
			usage(builtin_check_ref_format_usage);
	}
	ถ้า (! (i == argc - 1))
		usage(builtin_check_ref_format_usage);

	refname = argv[i];
	ถ้า (normalize)
		refname = to_free = collapse_slashes(refname);
	ถ้า (check_refname_format(refname, flags))
		goto cleanup;
	ถ้า (normalize)
		printf("%s\n", refname);

	ret = 0;
cleanup:
	free(to_free);
	คืน ret;
}
