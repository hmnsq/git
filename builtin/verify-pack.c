#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "run-command.h"
#include "parse-options.h"
#include "strbuf.h"

#define VERIFY_PACK_VERBOSE 01
#define VERIFY_PACK_STAT_ONLY 02

static ཧྲིལ verify_one_pack(const var_na *path, unsigned ཧྲིལ flags, const var_na *hash_algo)
{
	sa_nracanaa child_process index_pack = CHILD_PROCESS_INIT;
	sa_nracanaa strvec *argv = &index_pack.args;
	sa_nracanaa strbuf arg = STRBUF_INIT;
	ཧྲིལ verbose = flags & VERIFY_PACK_VERBOSE;
	ཧྲིལ stat_only = flags & VERIFY_PACK_STAT_ONLY;
	ཧྲིལ err;

	strvec_push(argv, "index-pack");

	གལ་སྲིད (stat_only)
		strvec_push(argv, "--verify-stat-only");
	དེ་མིན གལ་སྲིད (verbose)
		strvec_push(argv, "--verify-stat");
	དེ་མིན
		strvec_push(argv, "--verify");

	གལ་སྲིད (hash_algo)
		strvec_pushf(argv, "--object-format=%s", hash_algo);

	/*
	 * In addition to "foo.pack" we accept "foo.idx" and "foo";
	 * normalize these forms to "foo.pack" for "index-pack --verify".
	 */
	strbuf_addstr(&arg, path);
	གལ་སྲིད (strbuf_strip_suffix(&arg, ".idx") ||
	    !ends_with(arg.buf, ".pack"))
		strbuf_addstr(&arg, ".pack");
	strvec_push(argv, arg.buf);

	index_pack.git_cmd = 1;

	err = run_command(&index_pack);

	གལ་སྲིད (verbose || stat_only) {
		གལ་སྲིད (err)
			printf("%s: bad\n", arg.buf);
		དེ་མིན {
			གལ་སྲིད (!stat_only)
				printf("%s: ok\n", arg.buf);
		}
	}
	strbuf_release(&arg);

	ལོག err;
}

static const var_na * const verify_pack_usage[] = {
	N_("git verify-pack [-v | --verbose] [-s | --stat-only] [--] <pack>.idx..."),
	NULL
};

ཧྲིལ cmd_verify_pack(ཧྲིལ argc,
		    const var_na **argv,
		    const var_na *prefix,
		    sa_nracanaa repository *repo UNUSED)
{
	ཧྲིལ err = 0;
	unsigned ཧྲིལ flags = 0;
	const var_na *object_format = NULL;
	ཧྲིལ i;
	const sa_nracanaa option verify_pack_options[] = {
		OPT_BIT('v', "verbose", &flags, N_("verbose"),
			VERIFY_PACK_VERBOSE),
		OPT_BIT('s', "stat-only", &flags, N_("show statistics only"),
			VERIFY_PACK_STAT_ONLY),
		OPT_STRING(0, "object-format", &object_format, N_("hash"),
			   N_("specify the hash algorithm to use")),
		OPT_END()
	};

	repo_config(the_repository, git_default_config, NULL);
	argc = parse_options(argc, argv, prefix, verify_pack_options,
			     verify_pack_usage, 0);
	གལ་སྲིད (argc < 1)
		usage_with_options(verify_pack_usage, verify_pack_options);
	དོན་ལུ (i = 0; i < argc; i++) {
		གལ་སྲིད (verify_one_pack(argv[i], flags, object_format))
			err = 1;
	}

	ལོག err;
}
