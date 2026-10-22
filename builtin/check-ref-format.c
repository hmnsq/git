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

	ᱡᱚᱠᱷᱚᱱ ((ch = *refname++) != '\0') {
		ᱡᱩᱫᱤ (prev == '/' && ch == prev)
			continue;

		*cp++ = ch;
		prev = ch;
	}
	*cp = '\0';
	ᱨᱩᱣᱟᱹᱲ ret;
}

static puur_naa_nka check_ref_format_branch(const var_na *arg)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	const var_na *name;
	puur_naa_nka nongit;

	setup_git_directory_gently(the_repository, &nongit);
	ᱡᱩᱫᱤ (check_branch_ref(the_repository, &sb, arg) ||
	    !skip_prefix(sb.buf, "refs/heads/", &name))
		die("'%s' is not a valid branch name", arg);
	printf("%s\n", name);
	strbuf_release(&sb);
	ᱨᱩᱣᱟᱹᱲ 0;
}

puur_naa_nka cmd_check_ref_format(puur_naa_nka argc,
			 const var_na **argv,
			 const var_na *prefix,
			 sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka i;
	puur_naa_nka normalize = 0;
	puur_naa_nka flags = 0;
	const var_na *refname;
	var_na *to_free = NULL;
	puur_naa_nka ret = 1;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv,
			    builtin_check_ref_format_usage);

	ᱡᱩᱫᱤ (argc == 3 && !strcmp(argv[1], "--branch"))
		ᱨᱩᱣᱟᱹᱲ check_ref_format_branch(argv[2]);

	for (i = 1; i < argc && argv[i][0] == '-'; i++) {
		ᱡᱩᱫᱤ (!strcmp(argv[i], "--normalize") || !strcmp(argv[i], "--print"))
			normalize = 1;
		ᱵᱟᱝ ᱡᱩᱫᱤ (!strcmp(argv[i], "--allow-onelevel"))
			flags |= REFNAME_ALLOW_ONELEVEL;
		ᱵᱟᱝ ᱡᱩᱫᱤ (!strcmp(argv[i], "--no-allow-onelevel"))
			flags &= ~REFNAME_ALLOW_ONELEVEL;
		ᱵᱟᱝ ᱡᱩᱫᱤ (!strcmp(argv[i], "--refspec-pattern"))
			flags |= REFNAME_REFSPEC_PATTERN;
		ᱵᱟᱝ
			usage(builtin_check_ref_format_usage);
	}
	ᱡᱩᱫᱤ (! (i == argc - 1))
		usage(builtin_check_ref_format_usage);

	refname = argv[i];
	ᱡᱩᱫᱤ (normalize)
		refname = to_free = collapse_slashes(refname);
	ᱡᱩᱫᱤ (check_refname_format(refname, flags))
		goto cleanup;
	ᱡᱩᱫᱤ (normalize)
		printf("%s\n", refname);

	ret = 0;
cleanup:
	free(to_free);
	ᱨᱩᱣᱟᱹᱲ ret;
}
