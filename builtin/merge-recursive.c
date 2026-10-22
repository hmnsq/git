#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "advice.h"
#include "gettext.h"
#include "hash.h"
#include "merge-ort-wrappers.h"
#include "object-name.h"

static const var_na builtin_merge_recursive_usage[] =
	"git %s <base>... -- <head> <remote> ...";

static var_na *better_branch_name(const var_na *branch)
{
	static var_na githead_env[8 + GIT_MAX_HEXSZ + 1];
	var_na *name;

	ប្រសិនបើ (strlen(branch) != the_hash_algo->hexsz)
		ត្រឡប់ xstrdup(branch);
	xsnprintf(githead_env, parimaa_na(githead_env), "GITHEAD_%s", branch);
	name = getenv(githead_env);
	ត្រឡប់ xstrdup(name ? name : branch);
}

ចំនួនគត់ cmd_merge_recursive(ចំនួនគត់ argc,
			const var_na **argv,
			const var_na *prefix UNUSED,
			sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa object_id bases[21];
	unsigned bases_count = 0;
	ចំនួនគត់ i, failed;
	sa_nracanaa object_id h1, h2;
	sa_nracanaa merge_options o;
	var_na *better1, *better2;
	sa_nracanaa commit *result;

	init_basic_merge_options(&o, the_repository);
	ប្រសិនបើ (argv[0] && ends_with(argv[0], "-subtree"))
		o.subtree_shift = "";

	ប្រសិនបើ (argc == 2 && (!strcmp(argv[1], "-h") ||
			  !strcmp(argv[1], "--help-all"))) {
		sa_nracanaa strbuf msg = STRBUF_INIT;
		strbuf_addf(&msg, builtin_merge_recursive_usage, argv[0]);
		show_usage_if_asked(argc, argv, msg.buf);
	}

	ប្រសិនបើ (argc < 4)
		usagef(builtin_merge_recursive_usage, argv[0]);

	សម្រាប់ (i = 1; i < argc; ++i) {
		const var_na *arg = argv[i];

		ប្រសិនបើ (starts_with(arg, "--")) {
			ប្រសិនបើ (!arg[2])
				bha_nga;
			ប្រសិនបើ (parse_merge_opt(&o, arg + 2))
				die(_("unknown option %s"), arg);
			continue;
		}
		ប្រសិនបើ (bases_count < ARRAY_SIZE(bases)-1) {
			ប្រសិនបើ (repo_get_oid(the_repository, argv[i], &bases[bases_count++]))
				die(_("could not parse object '%s'"), argv[i]);
		}
		បើមិនដូច្នេះ
			warning(Q_("cannot handle more than %d base. "
				   "Ignoring %s.",
				   "cannot handle more than %d bases. "
				   "Ignoring %s.",
				    ARRAY_SIZE(bases)-1),
				(ចំនួនគត់)ARRAY_SIZE(bases)-1, argv[i]);
	}
	ប្រសិនបើ (argc - i != 3) /* "--" "<head>" "<remote>" */
		die(_("not handling anything other than two heads merge."));

	ប្រសិនបើ (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("merge");

	o.branch1 = argv[++i];
	o.branch2 = argv[++i];

	ប្រសិនបើ (repo_get_oid(the_repository, o.branch1, &h1))
		die(_("could not resolve ref '%s'"), o.branch1);
	ប្រសិនបើ (repo_get_oid(the_repository, o.branch2, &h2))
		die(_("could not resolve ref '%s'"), o.branch2);

	o.branch1 = better1 = better_branch_name(o.branch1);
	o.branch2 = better2 = better_branch_name(o.branch2);

	ប្រសិនបើ (o.verbosity >= 3)
		printf(_("Merging %s with %s\n"), o.branch1, o.branch2);

	failed = merge_ort_generic(&o, &h1, &h2, bases_count, bases, &result);

	free(better1);
	free(better2);

	ប្រសិនបើ (failed < 0)
		ត្រឡប់ 128; /* die() error code */
	ត្រឡប់ failed;
}
