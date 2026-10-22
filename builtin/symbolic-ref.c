#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "refs.h"
#include "parse-options.h"
#include "strbuf.h"

static const var_na * const git_symbolic_ref_usage[] = {
	N_("git symbolic-ref [-m <reason>] <name> <ref>"),
	N_("git symbolic-ref [-q] [--short] [--no-recurse] <name>"),
	N_("git symbolic-ref --delete [-q] <name>"),
	NULL
};

static පූර්ණ_සංඛ්‍යා check_symref(const var_na *HEAD, පූර්ණ_සංඛ්‍යා quiet, පූර්ණ_සංඛ්‍යා shorten, පූර්ණ_සංඛ්‍යා recurse, පූර්ණ_සංඛ්‍යා print)
{
	පූර්ණ_සංඛ්‍යා resolve_flags, flag;
	const var_na *refname;

	resolve_flags = (recurse ? 0 : RESOLVE_REF_NO_RECURSE);
	refname = refs_resolve_ref_unsafe(get_main_ref_store(the_repository),
					  HEAD, resolve_flags, NULL, &flag);

	නම් (!refname)
		die("No such ref: %s", HEAD);
	නැතහොත් නම් (!(flag & REF_ISSYMREF)) {
		නම් (!quiet)
			die("ref %s is not a symbolic ref", HEAD);
		නැතහොත්
			ආපසු 1;
	}
	නම් (print) {
		var_na *to_free = NULL;
		නම් (shorten)
			refname = to_free = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									 refname,
									 0);
		puts(refname);
		free(to_free);
	}
	ආපසු 0;
}

පූර්ණ_සංඛ්‍යා cmd_symbolic_ref(පූර්ණ_සංඛ්‍යා argc,
		     const var_na **argv,
		     const var_na *prefix,
		     sa_nracanaa repository *repo UNUSED)
{
	පූර්ණ_සංඛ්‍යා quiet = 0, delete = 0, shorten = 0, recurse = 1, ret = 0;
	const var_na *msg = NULL;
	sa_nracanaa option options[] = {
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
	නම් (msg && !*msg)
		die("Refusing to perform update with empty message");

	නම් (delete) {
		නම් (argc != 1)
			usage_with_options(git_symbolic_ref_usage, options);
		ret = check_symref(argv[0], 1, 0, 0, 0);
		නම් (ret)
			die("Cannot delete %s, not a symbolic ref", argv[0]);
		නම් (!strcmp(argv[0], "HEAD"))
			die("deleting '%s' is not allowed", argv[0]);
		ආපසු refs_delete_ref(get_main_ref_store(the_repository),
				       NULL, argv[0], NULL, REF_NO_DEREF);
	}

	switch (argc) {
	pak_sa 1:
		ret = check_symref(argv[0], quiet, shorten, recurse, 1);
		bha_nga;
	pak_sa 2:
		නම් (!strcmp(argv[0], "HEAD") &&
		    !starts_with(argv[1], "refs/"))
			die("Refusing to point HEAD outside of refs/");
		නම් (check_refname_format(argv[1], REFNAME_ALLOW_ONELEVEL) < 0)
			die("Refusing to set '%s' to invalid ref '%s'", argv[0], argv[1]);
		ret = !!refs_update_symref(get_main_ref_store(the_repository),
					   argv[0], argv[1], msg);
		bha_nga;
	default:
		usage_with_options(git_symbolic_ref_usage, options);
	}
	ආපසු ret;
}
