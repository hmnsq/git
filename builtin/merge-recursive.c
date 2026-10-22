#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"
#समावेश "advice.h"
#समावेश "gettext.h"
#समावेश "hash.h"
#समावेश "merge-ort-wrappers.h"
#समावेश "object-name.h"

अटल स्थिर अक्षर builtin_merge_recursive_usage[] =
	"git %s <base>... -- <head> <remote> ...";

अटल अक्षर *better_branch_name(स्थिर अक्षर *branch)
{
	अटल अक्षर githead_env[8 + GIT_MAX_HEXSZ + 1];
	अक्षर *name;

	अगर (म_माप(branch) != the_hash_algo->hexsz)
		वापस xstrdup(branch);
	xsnprintf(githead_env, माप(githead_env), "GITHEAD_%s", branch);
	name = दो_पर्या(githead_env);
	वापस xstrdup(name ? name : branch);
}

पूर्णांक cmd_merge_recursive(पूर्णांक argc,
			स्थिर अक्षर **argv,
			स्थिर अक्षर *prefix UNUSED,
			काष्ठा repository *repo UNUSED)
{
	काष्ठा object_id bases[21];
	अचिन्हित bases_गिनती = 0;
	पूर्णांक i, failed;
	काष्ठा object_id h1, h2;
	काष्ठा merge_options o;
	अक्षर *better1, *better2;
	काष्ठा commit *परिणाम;

	init_basic_merge_options(&o, the_repository);
	अगर (argv[0] && ends_with(argv[0], "-subtree"))
		o.subtree_shift = "";

	अगर (argc == 2 && (!म_भेद(argv[1], "-h") ||
			  !म_भेद(argv[1], "--help-all"))) {
		काष्ठा strbuf संदेश = STRBUF_INIT;
		strbuf_addf(&संदेश, builtin_merge_recursive_usage, argv[0]);
		show_usage_if_asked(argc, argv, संदेश.buf);
	}

	अगर (argc < 4)
		usagef(builtin_merge_recursive_usage, argv[0]);

	क्रम (i = 1; i < argc; ++i) {
		स्थिर अक्षर *arg = argv[i];

		अगर (starts_with(arg, "--")) {
			अगर (!arg[2])
				अवरोध;
			अगर (parse_merge_opt(&o, arg + 2))
				die(_("unknown option %s"), arg);
			जारी;
		}
		अगर (bases_गिनती < ARRAY_आकार(bases)-1) {
			अगर (repo_get_oid(the_repository, argv[i], &bases[bases_गिनती++]))
				die(_("could not parse object '%s'"), argv[i]);
		}
		अन्यथा
			warning(Q_("cannot handle more than %d base. "
				   "Ignoring %s.",
				   "cannot handle more than %d bases. "
				   "Ignoring %s.",
				    ARRAY_आकार(bases)-1),
				(पूर्णांक)ARRAY_आकार(bases)-1, argv[i]);
	}
	अगर (argc - i != 3) /* "--" "<head>" "<remote>" */
		die(_("not handling anything other than two heads merge."));

	अगर (repo_read_सूचकांक_unmerged(the_repository))
		die_resolve_conflict("merge");

	o.branch1 = argv[++i];
	o.branch2 = argv[++i];

	अगर (repo_get_oid(the_repository, o.branch1, &h1))
		die(_("could not resolve ref '%s'"), o.branch1);
	अगर (repo_get_oid(the_repository, o.branch2, &h2))
		die(_("could not resolve ref '%s'"), o.branch2);

	o.branch1 = better1 = better_branch_name(o.branch1);
	o.branch2 = better2 = better_branch_name(o.branch2);

	अगर (o.verbosity >= 3)
		म_लिखो(_("Merging %s with %s\n"), o.branch1, o.branch2);

	failed = merge_ort_generic(&o, &h1, &h2, bases_गिनती, bases, &परिणाम);

	मुक्त(better1);
	मुक्त(better2);

	अगर (failed < 0)
		वापस 128; /* die() error code */
	वापस failed;
}
