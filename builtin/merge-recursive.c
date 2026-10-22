#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "advice.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hash.h"
#અન્તર્ભાવ "merge-ort-wrappers.h"
#અન્તર્ભાવ "object-name.h"

સ્થિર અચલ અક્ષર builtin_merge_recursive_usage[] =
	"git %s <base>... -- <head> <remote> ...";

સ્થિર અક્ષર *better_branch_name(અચલ અક્ષર *branch)
{
	સ્થિર અક્ષર githead_env[8 + GIT_MAX_HEXSZ + 1];
	અક્ષર *name;

	જો (strlen(branch) != the_hash_algo->hexsz)
		પાછા xstrdup(branch);
	xsnprintf(githead_env, કદ(githead_env), "GITHEAD_%s", branch);
	name = getenv(githead_env);
	પાછા xstrdup(name ? name : branch);
}

પૂર્ણાંક cmd_merge_recursive(પૂર્ણાંક argc,
			અચલ અક્ષર **argv,
			અચલ અક્ષર *prefix UNUSED,
			સંરચના repository *repo UNUSED)
{
	સંરચના object_id bases[21];
	unsigned bases_ગણતરી = 0;
	પૂર્ણાંક i, failed;
	સંરચના object_id h1, h2;
	સંરચના merge_options o;
	અક્ષર *better1, *better2;
	સંરચના commit *result;

	init_basic_merge_options(&o, the_repository);
	જો (argv[0] && ends_with(argv[0], "-subtree"))
		o.subtree_shift = "";

	જો (argc == 2 && (!strcmp(argv[1], "-h") ||
			  !strcmp(argv[1], "--help-all"))) {
		સંરચના strbuf સંદેશ = STRBUF_INIT;
		strbuf_addf(&સંદેશ, builtin_merge_recursive_usage, argv[0]);
		show_usage_if_asked(argc, argv, સંદેશ.buf);
	}

	જો (argc < 4)
		usagef(builtin_merge_recursive_usage, argv[0]);

	માટે (i = 1; i < argc; ++i) {
		અચલ અક્ષર *arg = argv[i];

		જો (starts_with(arg, "--")) {
			જો (!arg[2])
				તોડો;
			જો (parse_merge_opt(&o, arg + 2))
				die(_("unknown option %s"), arg);
			ચાલુ;
		}
		જો (bases_ગણતરી < ARRAY_SIZE(bases)-1) {
			જો (repo_get_oid(the_repository, argv[i], &bases[bases_ગણતરી++]))
				die(_("could not parse object '%s'"), argv[i]);
		}
		નહીંતર
			warning(Q_("cannot handle more than %d base. "
				   "Ignoring %s.",
				   "cannot handle more than %d bases. "
				   "Ignoring %s.",
				    ARRAY_SIZE(bases)-1),
				(પૂર્ણાંક)ARRAY_SIZE(bases)-1, argv[i]);
	}
	જો (argc - i != 3) /* "--" "<head>" "<remote>" */
		die(_("not handling anything other than two heads merge."));

	જો (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("merge");

	o.branch1 = argv[++i];
	o.branch2 = argv[++i];

	જો (repo_get_oid(the_repository, o.branch1, &h1))
		die(_("could not resolve ref '%s'"), o.branch1);
	જો (repo_get_oid(the_repository, o.branch2, &h2))
		die(_("could not resolve ref '%s'"), o.branch2);

	o.branch1 = better1 = better_branch_name(o.branch1);
	o.branch2 = better2 = better_branch_name(o.branch2);

	જો (o.verbosity >= 3)
		printf(_("Merging %s with %s\n"), o.branch1, o.branch2);

	failed = merge_ort_generic(&o, &h1, &h2, bases_ગણતરી, bases, &result);

	મુક્ત(better1);
	મુક્ત(better2);

	જો (failed < 0)
		પાછા 128; /* die() error code */
	પાછા failed;
}
