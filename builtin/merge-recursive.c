#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "advice.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "hash.h"
#অন্তর্ভাৱ "merge-ort-wrappers.h"
#অন্তর্ভাৱ "object-name.h"

স্থির ধ্রুৱ ৱর্ণ builtin_merge_recursive_usage[] =
	"git %s <base>... -- <head> <remote> ...";

স্থির ৱর্ণ *better_branch_name(ধ্রুৱ ৱর্ণ *branch)
{
	স্থির ৱর্ণ githead_env[8 + GIT_MAX_HEXSZ + 1];
	ৱর্ণ *name;

	যদি (strlen(branch) != the_hash_algo->hexsz)
		ঘূৰাই xstrdup(branch);
	xsnprintf(githead_env, পরিমাণ(githead_env), "GITHEAD_%s", branch);
	name = getenv(githead_env);
	ঘূৰাই xstrdup(name ? name : branch);
}

পূর্ণাঙ্ক cmd_merge_recursive(পূর্ণাঙ্ক argc,
			ধ্রুৱ ৱর্ণ **argv,
			ধ্রুৱ ৱর্ণ *prefix UNUSED,
			সংরচনা repository *repo UNUSED)
{
	সংরচনা object_id bases[21];
	unsigned bases_গুনতি = 0;
	পূর্ণাঙ্ক i, failed;
	সংরচনা object_id h1, h2;
	সংরচনা merge_options o;
	ৱর্ণ *better1, *better2;
	সংরচনা commit *result;

	init_basic_merge_options(&o, the_repository);
	যদি (argv[0] && ends_with(argv[0], "-subtree"))
		o.subtree_shift = "";

	যদি (argc == 2 && (!strcmp(argv[1], "-h") ||
			  !strcmp(argv[1], "--help-all"))) {
		সংরচনা strbuf বাৰ্তা = STRBUF_INIT;
		strbuf_addf(&বাৰ্তা, builtin_merge_recursive_usage, argv[0]);
		show_usage_if_asked(argc, argv, বাৰ্তা.buf);
	}

	যদি (argc < 4)
		usagef(builtin_merge_recursive_usage, argv[0]);

	বাবে (i = 1; i < argc; ++i) {
		ধ্রুৱ ৱর্ণ *arg = argv[i];

		যদি (starts_with(arg, "--")) {
			যদি (!arg[2])
				ভঙ্গ;
			যদি (parse_merge_opt(&o, arg + 2))
				die(_("unknown option %s"), arg);
			অনুৱর্ত;
		}
		যদি (bases_গুনতি < ARRAY_SIZE(bases)-1) {
			যদি (repo_get_oid(the_repository, argv[i], &bases[bases_গুনতি++]))
				die(_("could not parse object '%s'"), argv[i]);
		}
		অন্যথা
			warning(Q_("cannot handle more than %d base. "
				   "Ignoring %s.",
				   "cannot handle more than %d bases. "
				   "Ignoring %s.",
				    ARRAY_SIZE(bases)-1),
				(পূর্ণাঙ্ক)ARRAY_SIZE(bases)-1, argv[i]);
	}
	যদি (argc - i != 3) /* "--" "<head>" "<remote>" */
		die(_("not handling anything other than two heads merge."));

	যদি (repo_read_index_unmerged(the_repository))
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

	failed = merge_ort_generic(&o, &h1, &h2, bases_গুনতি, bases, &result);

	মুক্ত(better1);
	মুক্ত(better2);

	যদি (failed < 0)
		ঘূৰাই 128; /* die() error code */
	ঘূৰাই failed;
}
