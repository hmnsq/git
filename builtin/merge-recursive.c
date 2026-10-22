#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "advice.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hash.h"
#அந்தர்பாவ "merge-ort-wrappers.h"
#அந்தர்பாவ "object-name.h"

நிலையான மாறிலி எழுத்து builtin_merge_recursive_usage[] =
	"git %s <base>... -- <head> <remote> ...";

நிலையான எழுத்து *better_branch_name(மாறிலி எழுத்து *branch)
{
	நிலையான எழுத்து githead_env[8 + GIT_MAX_HEXSZ + 1];
	எழுத்து *name;

	எனில் (strlen(branch) != the_hash_algo->hexsz)
		திரும்பு xstrdup(branch);
	xsnprintf(githead_env, அளவு(githead_env), "GITHEAD_%s", branch);
	name = getenv(githead_env);
	திரும்பு xstrdup(name ? name : branch);
}

முழு cmd_merge_recursive(முழு argc,
			மாறிலி எழுத்து **argv,
			மாறிலி எழுத்து *prefix UNUSED,
			கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு object_id bases[21];
	unsigned bases_மொத்தம் = 0;
	முழு i, failed;
	கட்டமைப்பு object_id h1, h2;
	கட்டமைப்பு merge_options o;
	எழுத்து *better1, *better2;
	கட்டமைப்பு commit *result;

	init_basic_merge_options(&o, the_repository);
	எனில் (argv[0] && ends_with(argv[0], "-subtree"))
		o.subtree_shift = "";

	எனில் (argc == 2 && (!strcmp(argv[1], "-h") ||
			  !strcmp(argv[1], "--help-all"))) {
		கட்டமைப்பு strbuf செய்தி = STRBUF_INIT;
		strbuf_addf(&செய்தி, builtin_merge_recursive_usage, argv[0]);
		show_usage_if_asked(argc, argv, செய்தி.buf);
	}

	எனில் (argc < 4)
		usagef(builtin_merge_recursive_usage, argv[0]);

	க்கு (i = 1; i < argc; ++i) {
		மாறிலி எழுத்து *arg = argv[i];

		எனில் (starts_with(arg, "--")) {
			எனில் (!arg[2])
				நிறுத்து;
			எனில் (parse_merge_opt(&o, arg + 2))
				die(_("unknown option %s"), arg);
			தொடர்;
		}
		எனில் (bases_மொத்தம் < ARRAY_SIZE(bases)-1) {
			எனில் (repo_get_oid(the_repository, argv[i], &bases[bases_மொத்தம்++]))
				die(_("could not parse object '%s'"), argv[i]);
		}
		இல்லையெனில்
			warning(Q_("cannot handle more than %d base. "
				   "Ignoring %s.",
				   "cannot handle more than %d bases. "
				   "Ignoring %s.",
				    ARRAY_SIZE(bases)-1),
				(முழு)ARRAY_SIZE(bases)-1, argv[i]);
	}
	எனில் (argc - i != 3) /* "--" "<head>" "<remote>" */
		die(_("not handling anything other than two heads merge."));

	எனில் (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("merge");

	o.branch1 = argv[++i];
	o.branch2 = argv[++i];

	எனில் (repo_get_oid(the_repository, o.branch1, &h1))
		die(_("could not resolve ref '%s'"), o.branch1);
	எனில் (repo_get_oid(the_repository, o.branch2, &h2))
		die(_("could not resolve ref '%s'"), o.branch2);

	o.branch1 = better1 = better_branch_name(o.branch1);
	o.branch2 = better2 = better_branch_name(o.branch2);

	எனில் (o.verbosity >= 3)
		printf(_("Merging %s with %s\n"), o.branch1, o.branch2);

	failed = merge_ort_generic(&o, &h1, &h2, bases_மொத்தம், bases, &result);

	விடுவி(better1);
	விடுவி(better2);

	எனில் (failed < 0)
		திரும்பு 128; /* die() error code */
	திரும்பு failed;
}
