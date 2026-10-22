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

	trong_khi ((ch = *refname++) != '\0') {
		nếu (prev == '/' && ch == prev)
			continue;

		*cp++ = ch;
		prev = ch;
	}
	*cp = '\0';
	trả_về ret;
}

static số_nguyên check_ref_format_branch(const char *arg)
{
	struct strbuf sb = STRBUF_INIT;
	const char *name;
	số_nguyên nongit;

	setup_git_directory_gently(the_repository, &nongit);
	nếu (check_branch_ref(the_repository, &sb, arg) ||
	    !skip_prefix(sb.buf, "refs/heads/", &name))
		die("'%s' is not a valid branch name", arg);
	printf("%s\n", name);
	strbuf_release(&sb);
	trả_về 0;
}

số_nguyên cmd_check_ref_format(số_nguyên argc,
			 const char **argv,
			 const char *prefix,
			 struct repository *repo UNUSED)
{
	số_nguyên i;
	số_nguyên normalize = 0;
	số_nguyên flags = 0;
	const char *refname;
	char *to_free = NULL;
	số_nguyên ret = 1;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv,
			    builtin_check_ref_format_usage);

	nếu (argc == 3 && !strcmp(argv[1], "--branch"))
		trả_về check_ref_format_branch(argv[2]);

	cho (i = 1; i < argc && argv[i][0] == '-'; i++) {
		nếu (!strcmp(argv[i], "--normalize") || !strcmp(argv[i], "--print"))
			normalize = 1;
		không_thì nếu (!strcmp(argv[i], "--allow-onelevel"))
			flags |= REFNAME_ALLOW_ONELEVEL;
		không_thì nếu (!strcmp(argv[i], "--no-allow-onelevel"))
			flags &= ~REFNAME_ALLOW_ONELEVEL;
		không_thì nếu (!strcmp(argv[i], "--refspec-pattern"))
			flags |= REFNAME_REFSPEC_PATTERN;
		không_thì
			usage(builtin_check_ref_format_usage);
	}
	nếu (! (i == argc - 1))
		usage(builtin_check_ref_format_usage);

	refname = argv[i];
	nếu (normalize)
		refname = to_free = collapse_slashes(refname);
	nếu (check_refname_format(refname, flags))
		goto cleanup;
	nếu (normalize)
		printf("%s\n", refname);

	ret = 0;
cleanup:
	free(to_free);
	trả_về ret;
}
