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

static 정수 verify_one_pack(const char *path, unsigned 정수 flags, const char *hash_algo)
{
	struct child_process index_pack = CHILD_PROCESS_INIT;
	struct strvec *argv = &index_pack.args;
	struct strbuf arg = STRBUF_INIT;
	정수 verbose = flags & VERIFY_PACK_VERBOSE;
	정수 stat_only = flags & VERIFY_PACK_STAT_ONLY;
	정수 err;

	strvec_push(argv, "index-pack");

	만약 (stat_only)
		strvec_push(argv, "--verify-stat-only");
	아니면 만약 (verbose)
		strvec_push(argv, "--verify-stat");
	아니면
		strvec_push(argv, "--verify");

	만약 (hash_algo)
		strvec_pushf(argv, "--object-format=%s", hash_algo);

	/*
	 * In addition to "foo.pack" we accept "foo.idx" and "foo";
	 * normalize these forms to "foo.pack" for "index-pack --verify".
	 */
	strbuf_addstr(&arg, path);
	만약 (strbuf_strip_suffix(&arg, ".idx") ||
	    !ends_with(arg.buf, ".pack"))
		strbuf_addstr(&arg, ".pack");
	strvec_push(argv, arg.buf);

	index_pack.git_cmd = 1;

	err = run_command(&index_pack);

	만약 (verbose || stat_only) {
		만약 (err)
			printf("%s: bad\n", arg.buf);
		아니면 {
			만약 (!stat_only)
				printf("%s: ok\n", arg.buf);
		}
	}
	strbuf_release(&arg);

	반환 err;
}

static const char * const verify_pack_usage[] = {
	N_("git verify-pack [-v | --verbose] [-s | --stat-only] [--] <pack>.idx..."),
	NULL
};

정수 cmd_verify_pack(정수 argc,
		    const char **argv,
		    const char *prefix,
		    struct repository *repo UNUSED)
{
	정수 err = 0;
	unsigned 정수 flags = 0;
	const char *object_format = NULL;
	정수 i;
	const struct option verify_pack_options[] = {
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
	만약 (argc < 1)
		usage_with_options(verify_pack_usage, verify_pack_options);
	위해 (i = 0; i < argc; i++) {
		만약 (verify_one_pack(argv[i], flags, object_format))
			err = 1;
	}

	반환 err;
}
