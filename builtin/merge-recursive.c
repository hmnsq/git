#define USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "advice.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hash.h"
#ಅನ್ತರ್ಭಾವ "merge-ort-wrappers.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ builtin_merge_recursive_usage[] =
	"git %s <base>... -- <head> <remote> ...";

ಸ್ಥಿರ ವರ್ಣ *better_branch_name(ಧ್ರುವ ವರ್ಣ *branch)
{
	ಸ್ಥಿರ ವರ್ಣ githead_env[8 + GIT_MAX_HEXSZ + 1];
	ವರ್ಣ *name;

	ಯದಿ (strlen(branch) != the_hash_algo->hexsz)
		ಪ್ರತ್ಯಾಗಮ xstrdup(branch);
	xsnprintf(githead_env, ಪರಿಮಾಣ(githead_env), "GITHEAD_%s", branch);
	name = getenv(githead_env);
	ಪ್ರತ್ಯಾಗಮ xstrdup(name ? name : branch);
}

ಪೂರ್ಣಾಙ್ಕ cmd_merge_recursive(ಪೂರ್ಣಾಙ್ಕ argc,
			ಧ್ರುವ ವರ್ಣ **argv,
			ಧ್ರುವ ವರ್ಣ *prefix UNUSED,
			ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ object_id bases[21];
	unsigned bases_ಎಣಿಕೆ = 0;
	ಪೂರ್ಣಾಙ್ಕ i, failed;
	ಸಂರಚನಾ object_id h1, h2;
	ಸಂರಚನಾ merge_options o;
	ವರ್ಣ *better1, *better2;
	ಸಂರಚನಾ commit *result;

	init_basic_merge_options(&o, the_repository);
	ಯದಿ (argv[0] && ends_with(argv[0], "-subtree"))
		o.subtree_shift = "";

	ಯದಿ (argc == 2 && (!strcmp(argv[1], "-h") ||
			  !strcmp(argv[1], "--help-all"))) {
		ಸಂರಚನಾ strbuf ಸಂದೇಶ = STRBUF_INIT;
		strbuf_addf(&ಸಂದೇಶ, builtin_merge_recursive_usage, argv[0]);
		show_usage_if_asked(argc, argv, ಸಂದೇಶ.buf);
	}

	ಯದಿ (argc < 4)
		usagef(builtin_merge_recursive_usage, argv[0]);

	ಪ್ರತಿ (i = 1; i < argc; ++i) {
		ಧ್ರುವ ವರ್ಣ *arg = argv[i];

		ಯದಿ (starts_with(arg, "--")) {
			ಯದಿ (!arg[2])
				ಭಙ್ಗ;
			ಯದಿ (parse_merge_opt(&o, arg + 2))
				die(_("unknown option %s"), arg);
			ಅನುವರ್ತ;
		}
		ಯದಿ (bases_ಎಣಿಕೆ < ARRAY_SIZE(bases)-1) {
			ಯದಿ (repo_get_oid(the_repository, argv[i], &bases[bases_ಎಣಿಕೆ++]))
				die(_("could not parse object '%s'"), argv[i]);
		}
		ಅನ್ಯಥಾ
			warning(Q_("cannot handle more than %d base. "
				   "Ignoring %s.",
				   "cannot handle more than %d bases. "
				   "Ignoring %s.",
				    ARRAY_SIZE(bases)-1),
				(ಪೂರ್ಣಾಙ್ಕ)ARRAY_SIZE(bases)-1, argv[i]);
	}
	ಯದಿ (argc - i != 3) /* "--" "<head>" "<remote>" */
		die(_("not handling anything other than two heads merge."));

	ಯದಿ (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("merge");

	o.branch1 = argv[++i];
	o.branch2 = argv[++i];

	ಯದಿ (repo_get_oid(the_repository, o.branch1, &h1))
		die(_("could not resolve ref '%s'"), o.branch1);
	ಯದಿ (repo_get_oid(the_repository, o.branch2, &h2))
		die(_("could not resolve ref '%s'"), o.branch2);

	o.branch1 = better1 = better_branch_name(o.branch1);
	o.branch2 = better2 = better_branch_name(o.branch2);

	ಯದಿ (o.verbosity >= 3)
		printf(_("Merging %s with %s\n"), o.branch1, o.branch2);

	failed = merge_ort_generic(&o, &h1, &h2, bases_ಎಣಿಕೆ, bases, &result);

	ಮುಕ್ತ(better1);
	ಮುಕ್ತ(better2);

	ಯದಿ (failed < 0)
		ಪ್ರತ್ಯಾಗಮ 128; /* die() error code */
	ಪ್ರತ್ಯಾಗಮ failed;
}
