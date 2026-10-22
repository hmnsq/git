#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "advice.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hash.h"
#అన్తర్భావ "merge-ort-wrappers.h"
#అన్తర్భావ "object-name.h"

స్థిర స్థిరరాశి అక్షరం builtin_విలీనం_recursive_usage[] =
	"git %s <base>... -- <head> <remote> ...";

స్థిర అక్షరం *better_branch_name(స్థిరరాశి అక్షరం *branch)
{
	స్థిర అక్షరం githead_env[8 + GIT_MAX_HEXSZ + 1];
	అక్షరం *name;

	ఒకవేళ (అక్షరశ్రేణి_పొడవు(branch) != the_hash_algo->hexsz)
		తిరిగి xstrdup(branch);
	xsnprintf(githead_env, పరిమాణం(githead_env), "GITHEAD_%s", branch);
	name = getenv(githead_env);
	తిరిగి xstrdup(name ? name : branch);
}

పూర్ణాంకం cmd_విలీనం_recursive(పూర్ణాంకం argc,
			స్థిరరాశి అక్షరం **argv,
			స్థిరరాశి అక్షరం *ఉపసర్గ వాడని,
			నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం object_id bases[21];
	గుర్తులేని bases_లెక్క = 0;
	పూర్ణాంకం i, failed;
	నిర్మాణం object_id h1, h2;
	నిర్మాణం విలీనం_options o;
	అక్షరం *better1, *better2;
	నిర్మాణం commit *ఫలితం;

	init_basic_విలీనం_options(&o, the_repository);
	ఒకవేళ (argv[0] && చివర్లు_with(argv[0], "-subtree"))
		o.subtree_జరుపు = "";

	ఒకవేళ (argc == 2 && (!strcmp(argv[1], "-h") ||
			  !strcmp(argv[1], "--help-all"))) {
		నిర్మాణం strbuf సందేశం = STRBUF_INIT;
		strbuf_addf(&సందేశం, builtin_విలీనం_recursive_usage, argv[0]);
		show_usage_if_asked(argc, argv, సందేశం.buf);
	}

	ఒకవేళ (argc < 4)
		usagef(builtin_విలీనం_recursive_usage, argv[0]);

	కొరకు (i = 1; i < argc; ++i) {
		స్థిరరాశి అక్షరం *arg = argv[i];

		ఒకవేళ (starts_with(arg, "--")) {
			ఒకవేళ (!arg[2])
				ఆపు;
			ఒకవేళ (parse_విలీనం_opt(&o, arg + 2))
				die(_("unknown option %s"), arg);
			కొనసాగు;
		}
		ఒకవేళ (bases_లెక్క < ARRAY_కొలత(bases)-1) {
			ఒకవేళ (repo_get_oid(the_repository, argv[i], &bases[bases_లెక్క++]))
				die(_("could not parse object '%s'"), argv[i]);
		}
		లేకపోతే
			warning(Q_("cannot handle more than %d base. "
				   "Ignoring %s.",
				   "cannot handle more than %d bases. "
				   "Ignoring %s.",
				    ARRAY_కొలత(bases)-1),
				(పూర్ణాంకం)ARRAY_కొలత(bases)-1, argv[i]);
	}
	ఒకవేళ (argc - i != 3) /* "--" "<head>" "<remote>" */
		die(_("not handling anything other than two heads merge."));

	ఒకవేళ (repo_read_సూచీ_unmerged(the_repository))
		die_resolve_conflict("merge");

	o.branch1 = argv[++i];
	o.branch2 = argv[++i];

	ఒకవేళ (repo_get_oid(the_repository, o.branch1, &h1))
		die(_("could not resolve ref '%s'"), o.branch1);
	ఒకవేళ (repo_get_oid(the_repository, o.branch2, &h2))
		die(_("could not resolve ref '%s'"), o.branch2);

	o.branch1 = better1 = better_branch_name(o.branch1);
	o.branch2 = better2 = better_branch_name(o.branch2);

	ఒకవేళ (o.verbosity >= 3)
		రాయి(_("Merging %s with %s\n"), o.branch1, o.branch2);

	failed = విలీనం_ort_generic(&o, &h1, &h2, bases_లెక్క, bases, &ఫలితం);

	విడుదల(better1);
	విడుదల(better2);

	ఒకవేళ (failed < 0)
		తిరిగి 128; /* die() error code */
	తిరిగి failed;
}
