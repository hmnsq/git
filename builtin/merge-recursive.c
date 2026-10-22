#define USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hash.h"
#अन्तर्भाव "merge-ort-wrappers.h"
#अन्तर्भाव "object-name.h"

स्थिर स्थिरांक अक्षर builtin_merge_recursive_usage[] =
	"git %s <base>... -- <head> <remote> ...";

स्थिर अक्षर *better_branch_name(स्थिरांक अक्षर *branch)
{
	स्थिर अक्षर githead_env[8 + GIT_MAX_HEXSZ + 1];
	अक्षर *name;

	जर (म_माप(branch) != the_hash_algo->hexsz)
		परत xstrdup(branch);
	xsnprintf(githead_env, आकार(githead_env), "GITHEAD_%s", branch);
	name = दो_पर्या(githead_env);
	परत xstrdup(name ? name : branch);
}

पूर्णांक cmd_merge_recursive(पूर्णांक argc,
			स्थिरांक अक्षर **argv,
			स्थिरांक अक्षर *prefix UNUSED,
			संरचना repository *repo UNUSED)
{
	संरचना object_id bases[21];
	अचिन्हित bases_मोजणी = 0;
	पूर्णांक i, failed;
	संरचना object_id h1, h2;
	संरचना merge_options o;
	अक्षर *better1, *better2;
	संरचना commit *result;

	init_basic_merge_options(&o, the_repository);
	जर (argv[0] && ends_with(argv[0], "-subtree"))
		o.subtree_shift = "";

	जर (argc == 2 && (!म_भेद(argv[1], "-h") ||
			  !म_भेद(argv[1], "--help-all"))) {
		संरचना strbuf संदेश = STRBUF_INIT;
		strbuf_addf(&संदेश, builtin_merge_recursive_usage, argv[0]);
		show_usage_if_asked(argc, argv, संदेश.buf);
	}

	जर (argc < 4)
		usagef(builtin_merge_recursive_usage, argv[0]);

	साठी (i = 1; i < argc; ++i) {
		स्थिरांक अक्षर *arg = argv[i];

		जर (starts_with(arg, "--")) {
			जर (!arg[2])
				थांबा;
			जर (parse_merge_opt(&o, arg + 2))
				die(_("unknown option %s"), arg);
			सुरूठेवा;
		}
		जर (bases_मोजणी < ARRAY_SIZE(bases)-1) {
			जर (repo_get_oid(the_repository, argv[i], &bases[bases_मोजणी++]))
				die(_("could not parse object '%s'"), argv[i]);
		}
		अन्यथा
			warning(Q_("cannot handle more than %d base. "
				   "Ignoring %s.",
				   "cannot handle more than %d bases. "
				   "Ignoring %s.",
				    ARRAY_SIZE(bases)-1),
				(पूर्णांक)ARRAY_SIZE(bases)-1, argv[i]);
	}
	जर (argc - i != 3) /* "--" "<head>" "<remote>" */
		die(_("not handling anything other than two heads merge."));

	जर (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("merge");

	o.branch1 = argv[++i];
	o.branch2 = argv[++i];

	जर (repo_get_oid(the_repository, o.branch1, &h1))
		die(_("could not resolve ref '%s'"), o.branch1);
	जर (repo_get_oid(the_repository, o.branch2, &h2))
		die(_("could not resolve ref '%s'"), o.branch2);

	o.branch1 = better1 = better_branch_name(o.branch1);
	o.branch2 = better2 = better_branch_name(o.branch2);

	जर (o.verbosity >= 3)
		म_लिखो(_("Merging %s with %s\n"), o.branch1, o.branch2);

	failed = merge_ort_generic(&o, &h1, &h2, bases_मोजणी, bases, &result);

	मुक्त(better1);
	मुक्त(better2);

	जर (failed < 0)
		परत 128; /* die() error code */
	परत failed;
}
