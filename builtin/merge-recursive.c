#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "advice.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hash.h"
#অন্তর্ভাব "merge-ort-wrappers.h"
#অন্তর্ভাব "object-name.h"

স্থির ধ্রুবক অক্ষর builtin_merge_recursive_usage[] =
	"git %s <base>... -- <head> <remote> ...";

স্থির অক্ষর *better_branch_name(ধ্রুবক অক্ষর *branch)
{
	স্থির অক্ষর githead_env[8 + GIT_MAX_HEXSZ + 1];
	অক্ষর *name;

	যদি (strlen(branch) != the_hash_algo->hexsz)
		ফেরত xstrdup(branch);
	xsnprintf(githead_env, আকার(githead_env), "GITHEAD_%s", branch);
	name = getenv(githead_env);
	ফেরত xstrdup(name ? name : branch);
}

পূর্ণসংখ্যা cmd_merge_recursive(পূর্ণসংখ্যা argc,
			ধ্রুবক অক্ষর **argv,
			ধ্রুবক অক্ষর *prefix UNUSED,
			কাঠামো repository *repo UNUSED)
{
	কাঠামো object_id bases[21];
	unsigned bases_গুনতি = 0;
	পূর্ণসংখ্যা i, failed;
	কাঠামো object_id h1, h2;
	কাঠামো merge_options o;
	অক্ষর *better1, *better2;
	কাঠামো commit *ফলাফল;

	init_basic_merge_options(&o, the_repository);
	যদি (argv[0] && ends_with(argv[0], "-subtree"))
		o.subtree_shift = "";

	যদি (argc == 2 && (!strcmp(argv[1], "-h") ||
			  !strcmp(argv[1], "--help-all"))) {
		কাঠামো strbuf বার্তা = STRBUF_INIT;
		strbuf_addf(&বার্তা, builtin_merge_recursive_usage, argv[0]);
		show_usage_if_asked(argc, argv, বার্তা.buf);
	}

	যদি (argc < 4)
		usagef(builtin_merge_recursive_usage, argv[0]);

	জন্য (i = 1; i < argc; ++i) {
		ধ্রুবক অক্ষর *arg = argv[i];

		যদি (starts_with(arg, "--")) {
			যদি (!arg[2])
				বিরতি;
			যদি (parse_merge_opt(&o, arg + 2))
				die(_("unknown option %s"), arg);
			চালু;
		}
		যদি (bases_গুনতি < ARRAY_আকার(bases)-1) {
			যদি (repo_get_oid(the_repository, argv[i], &bases[bases_গুনতি++]))
				die(_("could not parse object '%s'"), argv[i]);
		}
		অন্যথা
			warning(Q_("cannot handle more than %d base. "
				   "Ignoring %s.",
				   "cannot handle more than %d bases. "
				   "Ignoring %s.",
				    ARRAY_আকার(bases)-1),
				(পূর্ণসংখ্যা)ARRAY_আকার(bases)-1, argv[i]);
	}
	যদি (argc - i != 3) /* "--" "<head>" "<remote>" */
		die(_("not handling anything other than two heads merge."));

	যদি (repo_read_সূচক_unmerged(the_repository))
		die_resolve_conflict("merge");

	o.branch1 = argv[++i];
	o.branch2 = argv[++i];

	যদি (repo_get_oid(the_repository, o.branch1, &h1))
		die(_("could not resolve ref '%s'"), o.branch1);
	যদি (repo_get_oid(the_repository, o.branch2, &h2))
		die(_("could not resolve ref '%s'"), o.branch2);

	o.branch1 = better1 = better_branch_name(o.branch1);
	o.branch2 = better2 = better_branch_name(o.branch2);

	যদি (o.verbosity >= 3)
		printf(_("Merging %s with %s\n"), o.branch1, o.branch2);

	failed = merge_ort_generic(&o, &h1, &h2, bases_গুনতি, bases, &ফলাফল);

	মুক্ত(better1);
	মুক্ত(better2);

	যদি (failed < 0)
		ফেরত 128; /* die() error code */
	ফেরত failed;
}
