#define USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hash.h"
#अन्तर्भाव "merge-ort-wrappers.h"
#अन्तर्भाव "object-name.h"

स्थिर ध्रुव वर्ण builtin_merge_recursive_usage[] =
	"git %s <base>... -- <head> <remote> ...";

स्थिर वर्ण *better_branch_name(ध्रुव वर्ण *branch)
{
	स्थिर वर्ण githead_env[8 + GIT_MAX_HEXSZ + 1];
	वर्ण *name;

	यदि (strlen(branch) != the_hash_algo->hexsz)
		प्रत्यागम xstrdup(branch);
	xsnprintf(githead_env, परिमाण(githead_env), "GITHEAD_%s", branch);
	name = getenv(githead_env);
	प्रत्यागम xstrdup(name ? name : branch);
}

पूर्णाङ्क cmd_merge_recursive(पूर्णाङ्क argc,
			ध्रुव वर्ण **argv,
			ध्रुव वर्ण *prefix UNUSED,
			संरचना repository *repo UNUSED)
{
	संरचना object_id bases[21];
	unsigned bases_गिनती = 0;
	पूर्णाङ्क i, failed;
	संरचना object_id h1, h2;
	संरचना merge_options o;
	वर्ण *better1, *better2;
	संरचना commit *result;

	init_basic_merge_options(&o, the_repository);
	यदि (argv[0] && ends_with(argv[0], "-subtree"))
		o.subtree_shift = "";

	यदि (argc == 2 && (!strcmp(argv[1], "-h") ||
			  !strcmp(argv[1], "--help-all"))) {
		संरचना strbuf संदेश = STRBUF_INIT;
		strbuf_addf(&संदेश, builtin_merge_recursive_usage, argv[0]);
		show_usage_if_asked(argc, argv, संदेश.buf);
	}

	यदि (argc < 4)
		usagef(builtin_merge_recursive_usage, argv[0]);

	प्रति (i = 1; i < argc; ++i) {
		ध्रुव वर्ण *arg = argv[i];

		यदि (starts_with(arg, "--")) {
			यदि (!arg[2])
				भङ्ग;
			यदि (parse_merge_opt(&o, arg + 2))
				die(_("unknown option %s"), arg);
			अनुवर्त;
		}
		यदि (bases_गिनती < ARRAY_SIZE(bases)-1) {
			यदि (repo_get_oid(the_repository, argv[i], &bases[bases_गिनती++]))
				die(_("could not parse object '%s'"), argv[i]);
		}
		अन्यथा
			warning(Q_("cannot handle more than %d base. "
				   "Ignoring %s.",
				   "cannot handle more than %d bases. "
				   "Ignoring %s.",
				    ARRAY_SIZE(bases)-1),
				(पूर्णाङ्क)ARRAY_SIZE(bases)-1, argv[i]);
	}
	यदि (argc - i != 3) /* "--" "<head>" "<remote>" */
		die(_("not handling anything other than two heads merge."));

	यदि (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("merge");

	o.branch1 = argv[++i];
	o.branch2 = argv[++i];

	यदि (repo_get_oid(the_repository, o.branch1, &h1))
		die(_("could not resolve ref '%s'"), o.branch1);
	यदि (repo_get_oid(the_repository, o.branch2, &h2))
		die(_("could not resolve ref '%s'"), o.branch2);

	o.branch1 = better1 = better_branch_name(o.branch1);
	o.branch2 = better2 = better_branch_name(o.branch2);

	यदि (o.verbosity >= 3)
		printf(_("Merging %s with %s\n"), o.branch1, o.branch2);

	failed = merge_ort_generic(&o, &h1, &h2, bases_गिनती, bases, &result);

	मुक्त(better1);
	मुक्त(better2);

	यदि (failed < 0)
		प्रत्यागम 128; /* die() error code */
	प्रत्यागम failed;
}
