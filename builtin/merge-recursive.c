#define USE_THE_REPOSITORY_VARIABLE
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "advice.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hash.h"
#ଅନ୍ତର୍ଭାଵ "merge-ort-wrappers.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର builtin_merge_recursive_usage[] =
	"git %s <base>... -- <head> <remote> ...";

ସ୍ଥିର ଅକ୍ଷର *better_branch_name(ଅଚଳ ଅକ୍ଷର *branch)
{
	ସ୍ଥିର ଅକ୍ଷର githead_env[8 + GIT_MAX_HEXSZ + 1];
	ଅକ୍ଷର *name;

	ଯଦି (strlen(branch) != the_hash_algo->hexsz)
		ଫେରନ୍ତୁ xstrdup(branch);
	xsnprintf(githead_env, ଆକାର(githead_env), "GITHEAD_%s", branch);
	name = getenv(githead_env);
	ଫେରନ୍ତୁ xstrdup(name ? name : branch);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_merge_recursive(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
			ଅଚଳ ଅକ୍ଷର **argv,
			ଅଚଳ ଅକ୍ଷର *prefix UNUSED,
			ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା object_id bases[21];
	unsigned bases_ଗଣନା = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, failed;
	ସଂରଚନା object_id h1, h2;
	ସଂରଚନା merge_options o;
	ଅକ୍ଷର *better1, *better2;
	ସଂରଚନା commit *result;

	init_basic_merge_options(&o, the_repository);
	ଯଦି (argv[0] && ends_with(argv[0], "-subtree"))
		o.subtree_shift = "";

	ଯଦି (argc == 2 && (!strcmp(argv[1], "-h") ||
			  !strcmp(argv[1], "--help-all"))) {
		ସଂରଚନା strbuf ସନ୍ଦେଶ = STRBUF_INIT;
		strbuf_addf(&ସନ୍ଦେଶ, builtin_merge_recursive_usage, argv[0]);
		show_usage_if_asked(argc, argv, ସନ୍ଦେଶ.buf);
	}

	ଯଦି (argc < 4)
		usagef(builtin_merge_recursive_usage, argv[0]);

	ପାଇଁ (i = 1; i < argc; ++i) {
		ଅଚଳ ଅକ୍ଷର *arg = argv[i];

		ଯଦି (starts_with(arg, "--")) {
			ଯଦି (!arg[2])
				ଭାଙ୍ଗ;
			ଯଦି (parse_merge_opt(&o, arg + 2))
				die(_("unknown option %s"), arg);
			ଜାରି;
		}
		ଯଦି (bases_ଗଣନା < ARRAY_SIZE(bases)-1) {
			ଯଦି (repo_get_oid(the_repository, argv[i], &bases[bases_ଗଣନା++]))
				die(_("could not parse object '%s'"), argv[i]);
		}
		ଅନ୍ୟଥା
			warning(Q_("cannot handle more than %d base. "
				   "Ignoring %s.",
				   "cannot handle more than %d bases. "
				   "Ignoring %s.",
				    ARRAY_SIZE(bases)-1),
				(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ARRAY_SIZE(bases)-1, argv[i]);
	}
	ଯଦି (argc - i != 3) /* "--" "<head>" "<remote>" */
		die(_("not handling anything other than two heads merge."));

	ଯଦି (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("merge");

	o.branch1 = argv[++i];
	o.branch2 = argv[++i];

	ଯଦି (repo_get_oid(the_repository, o.branch1, &h1))
		die(_("could not resolve ref '%s'"), o.branch1);
	ଯଦି (repo_get_oid(the_repository, o.branch2, &h2))
		die(_("could not resolve ref '%s'"), o.branch2);

	o.branch1 = better1 = better_branch_name(o.branch1);
	o.branch2 = better2 = better_branch_name(o.branch2);

	ଯଦି (o.verbosity >= 3)
		printf(_("Merging %s with %s\n"), o.branch1, o.branch2);

	failed = merge_ort_generic(&o, &h1, &h2, bases_ଗଣନା, bases, &result);

	ମୁକ୍ତ(better1);
	ମୁକ୍ତ(better2);

	ଯଦି (failed < 0)
		ଫେରନ୍ତୁ 128; /* die() error code */
	ଫେରନ୍ତୁ failed;
}
