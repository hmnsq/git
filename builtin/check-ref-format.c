/*
 * GIT - The information manager from hell
 */

#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"
#include "refs.h"
#include "setup.h"
#include "strbuf.h"

static const char builtin_check_ref_format_usage[] =
"git check-ref-format [--normalize] [<options>] <refname>\n"
"   or: git check-ref-format --branch <branchname-shorthand>";

/*
 * Return a copy of refname but with leading slashes removed and runs
 * of adjacent slashes replaced with single slashes.
 *
 * This function is similar to normalize_path_copy(), but stripped down
 * to meet check_ref_format's simpler needs.
 */
static char *collapse_slashes(const char *refname)
{
	char *ret = xmallocz(strlen(refname));
	char ch;
	char prev = '/';
	char *cp = ret;

	当 ((ch = *refname++) != '\0') {
		如果 (prev == '/' && ch == prev)
			continue;

		*cp++ = ch;
		prev = ch;
	}
	*cp = '\0';
	返回 ret;
}

static 整数 check_ref_format_branch(const char *arg)
{
	struct strbuf sb = STRBUF_INIT;
	const char *name;
	整数 nongit;

	setup_git_directory_gently(the_repository, &nongit);
	如果 (check_branch_ref(the_repository, &sb, arg) ||
	    !skip_prefix(sb.buf, "refs/heads/", &name))
		die("'%s' is not a valid branch name", arg);
	printf("%s\n", name);
	strbuf_release(&sb);
	返回 0;
}

整数 cmd_check_ref_format(整数 argc,
			 const char **argv,
			 const char *prefix,
			 struct repository *repo UNUSED)
{
	整数 i;
	整数 normalize = 0;
	整数 flags = 0;
	const char *refname;
	char *to_free = NULL;
	整数 ret = 1;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv,
			    builtin_check_ref_format_usage);

	如果 (argc == 3 && !strcmp(argv[1], "--branch"))
		返回 check_ref_format_branch(argv[2]);

	为 (i = 1; i < argc && argv[i][0] == '-'; i++) {
		如果 (!strcmp(argv[i], "--normalize") || !strcmp(argv[i], "--print"))
			normalize = 1;
		否则 如果 (!strcmp(argv[i], "--allow-onelevel"))
			flags |= REFNAME_ALLOW_ONELEVEL;
		否则 如果 (!strcmp(argv[i], "--no-allow-onelevel"))
			flags &= ~REFNAME_ALLOW_ONELEVEL;
		否则 如果 (!strcmp(argv[i], "--refspec-pattern"))
			flags |= REFNAME_REFSPEC_PATTERN;
		否则
			usage(builtin_check_ref_format_usage);
	}
	如果 (! (i == argc - 1))
		usage(builtin_check_ref_format_usage);

	refname = argv[i];
	如果 (normalize)
		refname = to_free = collapse_slashes(refname);
	如果 (check_refname_format(refname, flags))
		goto cleanup;
	如果 (normalize)
		printf("%s\n", refname);

	ret = 0;
cleanup:
	free(to_free);
	返回 ret;
}
