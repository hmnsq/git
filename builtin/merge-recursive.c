#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "advice.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hash.h"
#ಅನ್ತರ್ಭಾವ "merge-ort-wrappers.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ builtin_ವಿಲೀನ_recursive_usage[] =
	"git %s <base>... -- <head> <remote> ...";

ಸ್ಥಿರ ಅಕ್ಷರ *better_branch_name(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *branch)
{
	ಸ್ಥಿರ ಅಕ್ಷರ githead_env[8 + GIT_MAX_HEXSZ + 1];
	ಅಕ್ಷರ *name;

	ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(branch) != the_hash_algo->hexsz)
		ಹಿಂತಿರುಗಿ xstrdup(branch);
	xsnprintf(githead_env, ಗಾತ್ರ(githead_env), "GITHEAD_%s", branch);
	name = getenv(githead_env);
	ಹಿಂತಿರುಗಿ xstrdup(name ? name : branch);
}

ಪೂರ್ಣಾಂಕ cmd_ವಿಲೀನ_recursive(ಪೂರ್ಣಾಂಕ argc,
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ ಬಳಸದ,
			ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ object_id bases[21];
	ಚಿಹ್ನೆರಹಿತ bases_ಎಣಿಕೆ = 0;
	ಪೂರ್ಣಾಂಕ i, failed;
	ರಚನೆ object_id h1, h2;
	ರಚನೆ ವಿಲೀನ_options o;
	ಅಕ್ಷರ *better1, *better2;
	ರಚನೆ commit *ಫಲಿತಾಂಶ;

	init_basic_ವಿಲೀನ_options(&o, the_repository);
	ಆದರೆ (argv[0] && ತುದಿಗಳು_with(argv[0], "-subtree"))
		o.subtree_ಸರಿಸು = "";

	ಆದರೆ (argc == 2 && (!strcmp(argv[1], "-h") ||
			  !strcmp(argv[1], "--help-all"))) {
		ರಚನೆ strbuf ಸಂದೇಶ = STRBUF_INIT;
		strbuf_addf(&ಸಂದೇಶ, builtin_ವಿಲೀನ_recursive_usage, argv[0]);
		show_usage_if_asked(argc, argv, ಸಂದೇಶ.buf);
	}

	ಆದರೆ (argc < 4)
		usagef(builtin_ವಿಲೀನ_recursive_usage, argv[0]);

	ಗಾಗಿ (i = 1; i < argc; ++i) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg = argv[i];

		ಆದರೆ (starts_with(arg, "--")) {
			ಆದರೆ (!arg[2])
				ನಿಲ್ಲಿಸು;
			ಆದರೆ (parse_ವಿಲೀನ_opt(&o, arg + 2))
				die(_("unknown option %s"), arg);
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (bases_ಎಣಿಕೆ < ARRAY_ಗಾತ್ರ(bases)-1) {
			ಆದರೆ (repo_get_oid(the_repository, argv[i], &bases[bases_ಎಣಿಕೆ++]))
				die(_("could not parse object '%s'"), argv[i]);
		}
		ಇಲ್ಲದಿದ್ದರೆ
			warning(Q_("cannot handle more than %d base. "
				   "Ignoring %s.",
				   "cannot handle more than %d bases. "
				   "Ignoring %s.",
				    ARRAY_ಗಾತ್ರ(bases)-1),
				(ಪೂರ್ಣಾಂಕ)ARRAY_ಗಾತ್ರ(bases)-1, argv[i]);
	}
	ಆದರೆ (argc - i != 3) /* "--" "<head>" "<remote>" */
		die(_("not handling anything other than two heads merge."));

	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ_unmerged(the_repository))
		die_resolve_conflict("merge");

	o.branch1 = argv[++i];
	o.branch2 = argv[++i];

	ಆದರೆ (repo_get_oid(the_repository, o.branch1, &h1))
		die(_("could not resolve ref '%s'"), o.branch1);
	ಆದರೆ (repo_get_oid(the_repository, o.branch2, &h2))
		die(_("could not resolve ref '%s'"), o.branch2);

	o.branch1 = better1 = better_branch_name(o.branch1);
	o.branch2 = better2 = better_branch_name(o.branch2);

	ಆದರೆ (o.verbosity >= 3)
		ಬರೆ(_("Merging %s with %s\n"), o.branch1, o.branch2);

	failed = ವಿಲೀನ_ort_generic(&o, &h1, &h2, bases_ಎಣಿಕೆ, bases, &ಫಲಿತಾಂಶ);

	ಬಿಡುಗಡೆ(better1);
	ಬಿಡುಗಡೆ(better2);

	ಆದರೆ (failed < 0)
		ಹಿಂತಿರುಗಿ 128; /* die() error code */
	ಹಿಂತಿರುಗಿ failed;
}
