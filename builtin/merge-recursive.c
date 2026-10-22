#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "advice.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hash.h"
#അന്തര്ഭാവ "merge-ort-wrappers.h"
#അന്തര്ഭാവ "object-name.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം builtin_merge_recursive_usage[] =
	"git %s <base>... -- <head> <remote> ...";

സ്ഥിതം അക്ഷരം *better_branch_name(സ്ഥിരം അക്ഷരം *branch)
{
	സ്ഥിതം അക്ഷരം githead_env[8 + GIT_MAX_HEXSZ + 1];
	അക്ഷരം *name;

	എങ്കിൽ (strlen(branch) != the_hash_algo->hexsz)
		തിരികെ xstrdup(branch);
	xsnprintf(githead_env, വലുപ്പം(githead_env), "GITHEAD_%s", branch);
	name = getenv(githead_env);
	തിരികെ xstrdup(name ? name : branch);
}

പൂർണ്ണസംഖ്യ cmd_merge_recursive(പൂർണ്ണസംഖ്യ argc,
			സ്ഥിരം അക്ഷരം **argv,
			സ്ഥിരം അക്ഷരം *prefix UNUSED,
			ഘടന repository *repo UNUSED)
{
	ഘടന object_id bases[21];
	unsigned bases_എണ്ണം = 0;
	പൂർണ്ണസംഖ്യ i, failed;
	ഘടന object_id h1, h2;
	ഘടന merge_options o;
	അക്ഷരം *better1, *better2;
	ഘടന commit *result;

	init_basic_merge_options(&o, the_repository);
	എങ്കിൽ (argv[0] && ends_with(argv[0], "-subtree"))
		o.subtree_shift = "";

	എങ്കിൽ (argc == 2 && (!strcmp(argv[1], "-h") ||
			  !strcmp(argv[1], "--help-all"))) {
		ഘടന strbuf സന്ദേശം = STRBUF_INIT;
		strbuf_addf(&സന്ദേശം, builtin_merge_recursive_usage, argv[0]);
		show_usage_if_asked(argc, argv, സന്ദേശം.buf);
	}

	എങ്കിൽ (argc < 4)
		usagef(builtin_merge_recursive_usage, argv[0]);

	വേണ്ടി (i = 1; i < argc; ++i) {
		സ്ഥിരം അക്ഷരം *arg = argv[i];

		എങ്കിൽ (starts_with(arg, "--")) {
			എങ്കിൽ (!arg[2])
				നിർത്തുക;
			എങ്കിൽ (parse_merge_opt(&o, arg + 2))
				die(_("unknown option %s"), arg);
			തുടരുക;
		}
		എങ്കിൽ (bases_എണ്ണം < ARRAY_SIZE(bases)-1) {
			എങ്കിൽ (repo_get_oid(the_repository, argv[i], &bases[bases_എണ്ണം++]))
				die(_("could not parse object '%s'"), argv[i]);
		}
		അല്ലെങ്കിൽ
			warning(Q_("cannot handle more than %d base. "
				   "Ignoring %s.",
				   "cannot handle more than %d bases. "
				   "Ignoring %s.",
				    ARRAY_SIZE(bases)-1),
				(പൂർണ്ണസംഖ്യ)ARRAY_SIZE(bases)-1, argv[i]);
	}
	എങ്കിൽ (argc - i != 3) /* "--" "<head>" "<remote>" */
		die(_("not handling anything other than two heads merge."));

	എങ്കിൽ (repo_read_index_unmerged(the_repository))
		die_resolve_conflict("merge");

	o.branch1 = argv[++i];
	o.branch2 = argv[++i];

	എങ്കിൽ (repo_get_oid(the_repository, o.branch1, &h1))
		die(_("could not resolve ref '%s'"), o.branch1);
	എങ്കിൽ (repo_get_oid(the_repository, o.branch2, &h2))
		die(_("could not resolve ref '%s'"), o.branch2);

	o.branch1 = better1 = better_branch_name(o.branch1);
	o.branch2 = better2 = better_branch_name(o.branch2);

	എങ്കിൽ (o.verbosity >= 3)
		printf(_("Merging %s with %s\n"), o.branch1, o.branch2);

	failed = merge_ort_generic(&o, &h1, &h2, bases_എണ്ണം, bases, &result);

	സ്വതന്ത്രം(better1);
	സ്വതന്ത്രം(better2);

	എങ്കിൽ (failed < 0)
		തിരികെ 128; /* die() error code */
	തിരികെ failed;
}
