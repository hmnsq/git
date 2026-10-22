#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "advice.h"
#include "gettext.h"
#include "hash.h"
#include "merge-ort-wrappers.h"
#include "object-name.h"

static const char builtin_merge_recursive_usage[] =
	"git %s <base>... -- <head> <remote> ...";

static char *better_branch_name(const char *branch)
{
	static char githead_env[8 + GIT_MAX_HEXSZ + 1];
	char *name;

	ако (strlen(branch) != the_hash_algo->hexsz)
		врати xstrdup(branch);
	xsnprintf(githead_env, sizeof(githead_env), "GITHEAD_%s", branch);
	name = getenv(githead_env);
	врати xstrdup(name ? name : branch);
}

цео cmd_merge_recursive(цео argc,
			const char **argv,
			const char *prefix UNUSED,
			struct repository *repo UNUSED)
{
	struct object_id bases[21];
	unsigned bases_count = 0;
	цео i, failed;
	struct object_id h1, h2;
	struct merge_options o;
	char *better1, *better2;
	struct commit *result;

	init_basic_merge_options(&o, the_repository);
	ако (argv[0] && ends_with(argv[0], "-subtree"))
		o.subtree_shift = "";

	ако (argc == 2 && (!strcmp(argv[1], "-h") ||
			  !strcmp(argv[1], "--help-all"))) {
		struct strbuf msg = STRBUF_INIT;
		strbuf_addf(&msg, builtin_merge_recursive_usage, argv[0]);
		show_usage_if_asked(argc, argv, msg.buf);
	}

	ако (argc < 4)
		usagef(builtin_merge_recursive_usage, argv[0]);

	за (i = 1; i < argc; ++i) {
		const char *arg = argv[i];

		ако (starts_with(arg, "--")) {
			ако (!arg[2])
				break;
			ако (parse_merge_opt(&o, arg + 2))
				die(_("unknown option %s"), arg);
			continue;
		}
		ако (bases_count < ARRAY_SIZE(bases)-1) {
			ако (repo_get_oid(the_repository, argv[i], &bases[bases_count++]))
				die(_("could not parse object '%s'"), argv[i]);
		}
		инако
			warning(Q_("cannot handle more than %d base. "
				   "Ignoring %s.",
				   "cannot handle more than %d bases. "
				   "Ignoring %s.",
				    ARRAY_SIZE(bases)-1),
				(цео)ARRAY_SIZE(bases)-1, argv[i]);
	}
	ако (argc - i != 3) /* "--" "<head>" "<remote>" */
		die(_("not handling anything other than two heads merge."));

	ако (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("merge");

	o.branch1 = argv[++i];
	o.branch2 = argv[++i];

	ако (repo_get_oid(the_repository, o.branch1, &h1))
		die(_("could not resolve ref '%s'"), o.branch1);
	ако (repo_get_oid(the_repository, o.branch2, &h2))
		die(_("could not resolve ref '%s'"), o.branch2);

	o.branch1 = better1 = better_branch_name(o.branch1);
	o.branch2 = better2 = better_branch_name(o.branch2);

	ако (o.verbosity >= 3)
		printf(_("Merging %s with %s\n"), o.branch1, o.branch2);

	failed = merge_ort_generic(&o, &h1, &h2, bases_count, bases, &result);

	free(better1);
	free(better2);

	ако (failed < 0)
		врати 128; /* die() error code */
	врати failed;
}
