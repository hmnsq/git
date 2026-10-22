#define USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "advice.h"
#ضمن "gettext.h"
#ضمن "hash.h"
#ضمن "merge-ort-wrappers.h"
#ضمن "object-name.h"

ساكن ثابت حرف builtin_merge_recursive_usage[] =
	"git %s <base>... -- <head> <remote> ...";

ساكن حرف *better_branch_name(ثابت حرف *branch)
{
	ساكن حرف githead_env[8 + GIT_MAX_HEXSZ + 1];
	حرف *name;

	אם (strlen(branch) != the_hash_algo->hexsz)
		החזר xstrdup(branch);
	xsnprintf(githead_env, حجم(githead_env), "GITHEAD_%s", branch);
	name = getenv(githead_env);
	החזר xstrdup(name ? name : branch);
}

صحيح cmd_merge_recursive(صحيح argc,
			ثابت حرف **argv,
			ثابت حرف *prefix UNUSED,
			بنية repository *repo UNUSED)
{
	بنية object_id bases[21];
	unsigned bases_count = 0;
	صحيح i, failed;
	بنية object_id h1, h2;
	بنية merge_options o;
	حرف *better1, *better2;
	بنية commit *result;

	init_basic_merge_options(&o, the_repository);
	אם (argv[0] && ends_with(argv[0], "-subtree"))
		o.subtree_shift = "";

	אם (argc == 2 && (!strcmp(argv[1], "-h") ||
			  !strcmp(argv[1], "--help-all"))) {
		بنية strbuf msg = STRBUF_INIT;
		strbuf_addf(&msg, builtin_merge_recursive_usage, argv[0]);
		show_usage_if_asked(argc, argv, msg.buf);
	}

	אם (argc < 4)
		usagef(builtin_merge_recursive_usage, argv[0]);

	עבור (i = 1; i < argc; ++i) {
		ثابت حرف *arg = argv[i];

		אם (starts_with(arg, "--")) {
			אם (!arg[2])
				اكسر;
			אם (parse_merge_opt(&o, arg + 2))
				die(_("unknown option %s"), arg);
			استمر;
		}
		אם (bases_count < ARRAY_SIZE(bases)-1) {
			אם (repo_get_oid(the_repository, argv[i], &bases[bases_count++]))
				die(_("could not parse object '%s'"), argv[i]);
		}
		אחרת
			warning(Q_("cannot handle more than %d base. "
				   "Ignoring %s.",
				   "cannot handle more than %d bases. "
				   "Ignoring %s.",
				    ARRAY_SIZE(bases)-1),
				(صحيح)ARRAY_SIZE(bases)-1, argv[i]);
	}
	אם (argc - i != 3) /* "--" "<head>" "<remote>" */
		die(_("not handling anything other than two heads merge."));

	אם (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("merge");

	o.branch1 = argv[++i];
	o.branch2 = argv[++i];

	אם (repo_get_oid(the_repository, o.branch1, &h1))
		die(_("could not resolve ref '%s'"), o.branch1);
	אם (repo_get_oid(the_repository, o.branch2, &h2))
		die(_("could not resolve ref '%s'"), o.branch2);

	o.branch1 = better1 = better_branch_name(o.branch1);
	o.branch2 = better2 = better_branch_name(o.branch2);

	אם (o.verbosity >= 3)
		printf(_("Merging %s with %s\n"), o.branch1, o.branch2);

	failed = merge_ort_generic(&o, &h1, &h2, bases_count, bases, &result);

	حرر(better1);
	حرر(better2);

	אם (failed < 0)
		החזר 128; /* die() error code */
	החזר failed;
}
