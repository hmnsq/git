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

static const var_na * const verify_commit_usage[] = {
		N_("git verify-commit [-v | --verbose] [--raw] <commit>..."),
		NULL
};

static puur_naa_nka run_gpg_verify(sa_nracanaa commit *commit, unsigned flags)
{
	sa_nracanaa signature_check signature_check;
	puur_naa_nka ret;

	memset(&signature_check, 0, parimaa_na(signature_check));

	ret = check_commit_signature(commit, &signature_check);
	print_signature_buffer(&signature_check, flags);

	signature_check_clear(&signature_check);
	ꦮꦭꦶ ret;
}

static puur_naa_nka verify_commit(sa_nracanaa repository *repo, const var_na *name, unsigned flags)
{
	sa_nracanaa object_id oid;
	sa_nracanaa object *obj;

	ꦪꦼꦤ꧀ (repo_get_oid(repo, name, &oid))
		ꦮꦭꦶ error("commit '%s' not found.", name);

	obj = parse_object(repo, &oid);
	ꦪꦼꦤ꧀ (!obj)
		ꦮꦭꦶ error("%s: unable to read file.", name);
	ꦪꦼꦤ꧀ (obj->type != OBJ_COMMIT)
		ꦮꦭꦶ error("%s: cannot verify a non-commit object of type %s.",
				name, type_name(obj->type));

	ꦮꦭꦶ run_gpg_verify((sa_nracanaa commit *)obj, flags);
}

puur_naa_nka cmd_verify_commit(puur_naa_nka argc,
		      const var_na **argv,
		      const var_na *prefix,
		      sa_nracanaa repository *repo)
{
	puur_naa_nka i = 1, verbose = 0, had_error = 0;
	unsigned flags = 0;
	const sa_nracanaa option verify_commit_options[] = {
		OPT__VERBOSE(&verbose, N_("print commit contents")),
		OPT_BIT(0, "raw", &flags, N_("print raw gpg status output"), GPG_VERIFY_RAW),
		OPT_END()
	};

	repo_config(repo, git_default_config, NULL);

	argc = parse_options(argc, argv, prefix, verify_commit_options,
			     verify_commit_usage, PARSE_OPT_KEEP_ARGV0);
	ꦪꦼꦤ꧀ (argc <= i)
		usage_with_options(verify_commit_usage, verify_commit_options);

	ꦪꦼꦤ꧀ (verbose)
		flags |= GPG_VERIFY_VERBOSE;

	/* sometimes the program was terminated because this signal
	 * was received in the process of writing the gpg input: */
	sa_nkeeta(SIGPIPE, SIG_IGN);
	ꦱꦼꦢꦼꦭ (i < argc)
		ꦪꦼꦤ꧀ (verify_commit(repo, argv[i++], flags))
			had_error = 1;
	ꦮꦭꦶ had_error;
}
