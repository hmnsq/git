#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "advice.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hash.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "merge-ort-wrappers.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ builtin_ਮਿਲਾਓ_recursive_usage[] =
	"git %s <base>... -- <head> <remote> ...";

ਸਥਿਰ ਅੱਖਰ *better_branch_name(ਅਚਲ ਅੱਖਰ *branch)
{
	ਸਥਿਰ ਅੱਖਰ githead_env[8 + GIT_MAX_HEXSZ + 1];
	ਅੱਖਰ *name;

	ਜੇ (ਲੜੀ_ਲੰਬਾਈ(branch) != the_hash_algo->hexsz)
		ਵਾਪਸ xstrdup(branch);
	xsnprintf(githead_env, ਆਕਾਰ(githead_env), "GITHEAD_%s", branch);
	name = getenv(githead_env);
	ਵਾਪਸ xstrdup(name ? name : branch);
}

ਪੂਰਨ cmd_ਮਿਲਾਓ_recursive(ਪੂਰਨ argc,
			ਅਚਲ ਅੱਖਰ **argv,
			ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ ਅਣਵਰਤਿਆ,
			ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ object_id bases[21];
	ਬਿਨਾਨਿਸ਼ਾਨੀ bases_ਗਿਣਤੀ = 0;
	ਪੂਰਨ i, failed;
	ਬਣਤਰ object_id h1, h2;
	ਬਣਤਰ ਮਿਲਾਓ_options o;
	ਅੱਖਰ *better1, *better2;
	ਬਣਤਰ commit *ਨਤੀਜਾ;

	init_basic_ਮਿਲਾਓ_options(&o, the_repository);
	ਜੇ (argv[0] && ਸਿਰੇ_with(argv[0], "-subtree"))
		o.subtree_ਖਿਸਕਾਓ = "";

	ਜੇ (argc == 2 && (!strcmp(argv[1], "-h") ||
			  !strcmp(argv[1], "--help-all"))) {
		ਬਣਤਰ strbuf ਸੁਨੇਹਾ = STRBUF_INIT;
		strbuf_addf(&ਸੁਨੇਹਾ, builtin_ਮਿਲਾਓ_recursive_usage, argv[0]);
		show_usage_if_asked(argc, argv, ਸੁਨੇਹਾ.buf);
	}

	ਜੇ (argc < 4)
		usagef(builtin_ਮਿਲਾਓ_recursive_usage, argv[0]);

	ਲਈ (i = 1; i < argc; ++i) {
		ਅਚਲ ਅੱਖਰ *arg = argv[i];

		ਜੇ (starts_with(arg, "--")) {
			ਜੇ (!arg[2])
				ਰੋਕੋ;
			ਜੇ (parse_ਮਿਲਾਓ_opt(&o, arg + 2))
				die(_("unknown option %s"), arg);
			ਜਾਰੀ;
		}
		ਜੇ (bases_ਗਿਣਤੀ < ARRAY_ਮਾਪ(bases)-1) {
			ਜੇ (repo_get_oid(the_repository, argv[i], &bases[bases_ਗਿਣਤੀ++]))
				die(_("could not parse object '%s'"), argv[i]);
		}
		ਨਹੀਂਤਾਂ
			warning(Q_("cannot handle more than %d base. "
				   "Ignoring %s.",
				   "cannot handle more than %d bases. "
				   "Ignoring %s.",
				    ARRAY_ਮਾਪ(bases)-1),
				(ਪੂਰਨ)ARRAY_ਮਾਪ(bases)-1, argv[i]);
	}
	ਜੇ (argc - i != 3) /* "--" "<head>" "<remote>" */
		die(_("not handling anything other than two heads merge."));

	ਜੇ (repo_read_ਸੂਚਕਾਂਕ_unmerged(the_repository))
		die_resolve_conflict("merge");

	o.branch1 = argv[++i];
	o.branch2 = argv[++i];

	ਜੇ (repo_get_oid(the_repository, o.branch1, &h1))
		die(_("could not resolve ref '%s'"), o.branch1);
	ਜੇ (repo_get_oid(the_repository, o.branch2, &h2))
		die(_("could not resolve ref '%s'"), o.branch2);

	o.branch1 = better1 = better_branch_name(o.branch1);
	o.branch2 = better2 = better_branch_name(o.branch2);

	ਜੇ (o.verbosity >= 3)
		ਲਿਖੋ(_("Merging %s with %s\n"), o.branch1, o.branch2);

	failed = ਮਿਲਾਓ_ort_generic(&o, &h1, &h2, bases_ਗਿਣਤੀ, bases, &ਨਤੀਜਾ);

	ਮੁਕਤ(better1);
	ਮੁਕਤ(better2);

	ਜੇ (failed < 0)
		ਵਾਪਸ 128; /* die() error code */
	ਵਾਪਸ failed;
}
