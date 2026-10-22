/*
 * Builtin "git commit-commit"
 *
 * Copyright (c) 2014 Michael J Gruber <git@drmicha.warpmail.net>
 *
 * Based on git-verify-tag
 */
#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "object-name.h"
#include "commit.h"
#include "parse-options.h"
#include "gpg-interface.h"

static const char * const verify_commit_usage[] = {
		N_("git verify-commit [-v | --verbose] [--raw] <commit>..."),
		NULL
};

static 정수 run_gpg_verify(struct commit *commit, unsigned flags)
{
	struct signature_check signature_check;
	정수 ret;

	memset(&signature_check, 0, sizeof(signature_check));

	ret = check_commit_signature(commit, &signature_check);
	print_signature_buffer(&signature_check, flags);

	signature_check_clear(&signature_check);
	반환 ret;
}

static 정수 verify_commit(struct repository *repo, const char *name, unsigned flags)
{
	struct object_id oid;
	struct object *obj;

	만약 (repo_get_oid(repo, name, &oid))
		반환 error("commit '%s' not found.", name);

	obj = parse_object(repo, &oid);
	만약 (!obj)
		반환 error("%s: unable to read file.", name);
	만약 (obj->type != OBJ_COMMIT)
		반환 error("%s: cannot verify a non-commit object of type %s.",
				name, type_name(obj->type));

	반환 run_gpg_verify((struct commit *)obj, flags);
}

정수 cmd_verify_commit(정수 argc,
		      const char **argv,
		      const char *prefix,
		      struct repository *repo)
{
	정수 i = 1, verbose = 0, had_error = 0;
	unsigned flags = 0;
	const struct option verify_commit_options[] = {
		OPT__VERBOSE(&verbose, N_("print commit contents")),
		OPT_BIT(0, "raw", &flags, N_("print raw gpg status output"), GPG_VERIFY_RAW),
		OPT_END()
	};

	repo_config(repo, git_default_config, NULL);

	argc = parse_options(argc, argv, prefix, verify_commit_options,
			     verify_commit_usage, PARSE_OPT_KEEP_ARGV0);
	만약 (argc <= i)
		usage_with_options(verify_commit_usage, verify_commit_options);

	만약 (verbose)
		flags |= GPG_VERIFY_VERBOSE;

	/* sometimes the program was terminated because this signal
	 * was received in the process of writing the gpg input: */
	signal(SIGPIPE, SIG_IGN);
	동안 (i < argc)
		만약 (verify_commit(repo, argv[i++], flags))
			had_error = 1;
	반환 had_error;
}
