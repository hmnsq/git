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

	동안 ((ch = *refname++) != '\0') {
		만약 (prev == '/' && ch == prev)
			continue;

		*cp++ = ch;
		prev = ch;
	}
	*cp = '\0';
	반환 ret;
}

static 정수 check_ref_format_branch(const char *arg)
{
	struct strbuf sb = STRBUF_INIT;
	const char *name;
	정수 nongit;

	setup_git_directory_gently(the_repository, &nongit);
	만약 (check_branch_ref(the_repository, &sb, arg) ||
	    !skip_prefix(sb.buf, "refs/heads/", &name))
		die("'%s' is not a valid branch name", arg);
	printf("%s\n", name);
	strbuf_release(&sb);
	반환 0;
}

정수 cmd_check_ref_format(정수 argc,
			 const char **argv,
			 const char *prefix,
			 struct repository *repo UNUSED)
{
	정수 i;
	정수 normalize = 0;
	정수 flags = 0;
	const char *refname;
	char *to_free = NULL;
	정수 ret = 1;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv,
			    builtin_check_ref_format_usage);

	만약 (argc == 3 && !strcmp(argv[1], "--branch"))
		반환 check_ref_format_branch(argv[2]);

	위해 (i = 1; i < argc && argv[i][0] == '-'; i++) {
		만약 (!strcmp(argv[i], "--normalize") || !strcmp(argv[i], "--print"))
			normalize = 1;
		아니면 만약 (!strcmp(argv[i], "--allow-onelevel"))
			flags |= REFNAME_ALLOW_ONELEVEL;
		아니면 만약 (!strcmp(argv[i], "--no-allow-onelevel"))
			flags &= ~REFNAME_ALLOW_ONELEVEL;
		아니면 만약 (!strcmp(argv[i], "--refspec-pattern"))
			flags |= REFNAME_REFSPEC_PATTERN;
		아니면
			usage(builtin_check_ref_format_usage);
	}
	만약 (! (i == argc - 1))
		usage(builtin_check_ref_format_usage);

	refname = argv[i];
	만약 (normalize)
		refname = to_free = collapse_slashes(refname);
	만약 (check_refname_format(refname, flags))
		goto cleanup;
	만약 (normalize)
		printf("%s\n", refname);

	ret = 0;
cleanup:
	free(to_free);
	반환 ret;
}
