#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "advice.h"
#شامل "gettext.h"
#شامل "hash.h"
#شامل "merge-ort-wrappers.h"
#شامل "object-name.h"

ایستا ثابت نویسه builtin_merge_recursive_usage[] =
	"git %s <base>... -- <head> <remote> ...";

ایستا نویسه *better_branch_name(ثابت نویسه *branch)
{
	ایستا نویسه githead_env[8 + GIT_MAX_HEXSZ + 1];
	نویسه *name;

	اگر (strlen(branch) != the_hash_algo->hexsz)
		بازگشت xstrdup(branch);
	xsnprintf(githead_env, اندازه(githead_env), "GITHEAD_%s", branch);
	name = getenv(githead_env);
	بازگشت xstrdup(name ? name : branch);
}

صحیح cmd_merge_recursive(صحیح argc,
			ثابت نویسه **argv,
			ثابت نویسه *prefix UNUSED,
			ساختار repository *repo UNUSED)
{
	ساختار object_id bases[21];
	unsigned bases_شمار = 0;
	صحیح i, failed;
	ساختار object_id h1, h2;
	ساختار merge_options o;
	نویسه *better1, *better2;
	ساختار commit *result;

	init_basic_merge_options(&o, the_repository);
	اگر (argv[0] && ends_with(argv[0], "-subtree"))
		o.subtree_shift = "";

	اگر (argc == 2 && (!strcmp(argv[1], "-h") ||
			  !strcmp(argv[1], "--help-all"))) {
		ساختار strbuf پیام = STRBUF_INIT;
		strbuf_addf(&پیام, builtin_merge_recursive_usage, argv[0]);
		show_usage_if_asked(argc, argv, پیام.buf);
	}

	اگر (argc < 4)
		usagef(builtin_merge_recursive_usage, argv[0]);

	برای (i = 1; i < argc; ++i) {
		ثابت نویسه *arg = argv[i];

		اگر (starts_with(arg, "--")) {
			اگر (!arg[2])
				بشکن;
			اگر (parse_merge_opt(&o, arg + 2))
				die(_("unknown option %s"), arg);
			ادامه;
		}
		اگر (bases_شمار < ARRAY_SIZE(bases)-1) {
			اگر (repo_get_oid(the_repository, argv[i], &bases[bases_شمار++]))
				die(_("could not parse object '%s'"), argv[i]);
		}
		وگرنه
			warning(Q_("cannot handle more than %d base. "
				   "Ignoring %s.",
				   "cannot handle more than %d bases. "
				   "Ignoring %s.",
				    ARRAY_SIZE(bases)-1),
				(صحیح)ARRAY_SIZE(bases)-1, argv[i]);
	}
	اگر (argc - i != 3) /* "--" "<head>" "<remote>" */
		die(_("not handling anything other than two heads merge."));

	اگر (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("merge");

	o.branch1 = argv[++i];
	o.branch2 = argv[++i];

	اگر (repo_get_oid(the_repository, o.branch1, &h1))
		die(_("could not resolve ref '%s'"), o.branch1);
	اگر (repo_get_oid(the_repository, o.branch2, &h2))
		die(_("could not resolve ref '%s'"), o.branch2);

	o.branch1 = better1 = better_branch_name(o.branch1);
	o.branch2 = better2 = better_branch_name(o.branch2);

	اگر (o.verbosity >= 3)
		printf(_("Merging %s with %s\n"), o.branch1, o.branch2);

	failed = merge_ort_generic(&o, &h1, &h2, bases_شمار, bases, &result);

	آزاد(better1);
	آزاد(better2);

	اگر (failed < 0)
		بازگشت 128; /* die() error code */
	بازگشت failed;
}
