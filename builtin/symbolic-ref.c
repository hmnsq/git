#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "refs.h"
#include "parse-options.h"
#include "strbuf.h"

static const char * const git_symbolic_ref_usage[] = {
	N_("git symbolic-ref [-m <reason>] <name> <ref>"),
	N_("git symbolic-ref [-q] [--short] [--no-recurse] <name>"),
	N_("git symbolic-ref --delete [-q] <name>"),
	NULL
};

static số_nguyên check_symref(const char *HEAD, số_nguyên quiet, số_nguyên shorten, số_nguyên recurse, số_nguyên print)
{
	số_nguyên resolve_flags, flag;
	const char *refname;

	resolve_flags = (recurse ? 0 : RESOLVE_REF_NO_RECURSE);
	refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					  HEAD, resolve_flags, NULL, &flag);

	nếu (!refname)
		die("No such ref: %s", HEAD);
	không_thì nếu (!(flag & REF_ISSYMREF)) {
		nếu (!quiet)
			die("ref %s is not a symbolic ref", HEAD);
		không_thì
			trả_về 1;
	}
	nếu (print) {
		char *to_free = NULL;
		nếu (shorten)
			refname = to_free = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									 refname,
									 0);
		puts(refname);
		free(to_free);
	}
	trả_về 0;
}

số_nguyên cmd_symbolic_ref(số_nguyên argc,
		     const char **argv,
		     const char *prefix,
		     struct repository *repo UNUSED)
{
	số_nguyên quiet = 0, delete = 0, shorten = 0, recurse = 1, ret = 0;
	const char *msg = NULL;
	struct option options[] = {
		OPT__QUIET(&quiet,
			N_("suppress error message for non-symbolic (detached) refs")),
		OPT_BOOL('d', "delete", &delete, N_("delete symbolic ref")),
		OPT_BOOL(0, "short", &shorten, N_("shorten ref output")),
		OPT_BOOL(0, "recurse", &recurse, N_("recursively dereference (default)")),
		OPT_STRING('m', NULL, &msg, N_("reason"), N_("reason of the update")),
		OPT_END(),
	};

	repo_config(the_repository, git_default_config, NULL);
	argc = parse_options(argc, argv, prefix, options,
			     git_symbolic_ref_usage, 0);
	nếu (msg && !*msg)
		die("Refusing to perform update with empty message");

	nếu (delete) {
		nếu (argc != 1)
			usage_with_options(git_symbolic_ref_usage, options);
		ret = check_symref(argv[0], 1, 0, 0, 0);
		nếu (ret)
			die("Cannot delete %s, not a symbolic ref", argv[0]);
		nếu (!strcmp(argv[0], "HEAD"))
			die("deleting '%s' is not allowed", argv[0]);
		trả_về refs_delete_ref(get_main_ref_store(the_repository),
				       NULL, argv[0], NULL, REF_NO_DEREF);
	}

	switch (argc) {
	case 1:
		ret = check_symref(argv[0], quiet, shorten, recurse, 1);
		break;
	case 2:
		nếu (!strcmp(argv[0], "HEAD") &&
		    !starts_with(argv[1], "refs/"))
			die("Refusing to point HEAD outside of refs/");
		nếu (check_refname_format(argv[1], REFNAME_ALLOW_ONELEVEL) < 0)
			die("Refusing to set '%s' to invalid ref '%s'", argv[0], argv[1]);
		ret = !!refs_update_symref(get_main_ref_store(the_repository),
					   argv[0], argv[1], msg);
		break;
	default:
		usage_with_options(git_symbolic_ref_usage, options);
	}
	trả_về ret;
}
