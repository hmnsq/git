#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "advice.h"
#ضمن "gettext.h"
#ضمن "hash.h"
#ضمن "merge-ort-wrappers.h"
#ضمن "object-name.h"

ساكن ثابت حرف builtin_ادغام_recursive_usage[] =
	"git %s <base>... -- <head> <remote> ...";

ساكن حرف *better_branch_name(ثابت حرف *branch)
{
	ساكن حرف githead_env[8 + GIT_MAX_HEXSZ + 1];
	حرف *name;

	إذا (طول_رشته(branch) != the_hash_algo->hexsz)
		أرجع xstrdup(branch);
	xsnprintf(githead_env, حجم(githead_env), "GITHEAD_%s", branch);
	name = getenv(githead_env);
	أرجع xstrdup(name ? name : branch);
}

صحيح cmd_ادغام_recursive(صحيح argc,
			ثابت حرف **argv,
			ثابت حرف *پیشوند استفادهنشده,
			بنية repository *repo استفادهنشده)
{
	بنية object_id bases[21];
	بیعلامت bases_شمار = 0;
	صحيح i, failed;
	بنية object_id h1, h2;
	بنية ادغام_options o;
	حرف *better1, *better2;
	بنية commit *نتیجه;

	init_basic_ادغام_options(&o, the_repository);
	إذا (argv[0] && دوسر_with(argv[0], "-subtree"))
		o.subtree_لغزش = "";

	إذا (argc == 2 && (!strcmp(argv[1], "-h") ||
			  !strcmp(argv[1], "--help-all"))) {
		بنية strbuf پیام = STRBUF_INIT;
		strbuf_addf(&پیام, builtin_ادغام_recursive_usage, argv[0]);
		show_usage_if_asked(argc, argv, پیام.buf);
	}

	إذا (argc < 4)
		usagef(builtin_ادغام_recursive_usage, argv[0]);

	لكل (i = 1; i < argc; ++i) {
		ثابت حرف *arg = argv[i];

		إذا (starts_with(arg, "--")) {
			إذا (!arg[2])
				اكسر;
			إذا (parse_ادغام_opt(&o, arg + 2))
				die(_("unknown option %s"), arg);
			استمر;
		}
		إذا (bases_شمار < ARRAY_اندازه(bases)-1) {
			إذا (repo_get_oid(the_repository, argv[i], &bases[bases_شمار++]))
				die(_("could not parse object '%s'"), argv[i]);
		}
		وإلا
			warning(Q_("cannot handle more than %d base. "
				   "Ignoring %s.",
				   "cannot handle more than %d bases. "
				   "Ignoring %s.",
				    ARRAY_اندازه(bases)-1),
				(صحيح)ARRAY_اندازه(bases)-1, argv[i]);
	}
	إذا (argc - i != 3) /* "--" "<head>" "<remote>" */
		die(_("not handling anything other than two heads merge."));

	إذا (repo_read_شاخص_unmerged(the_repository))
		die_resolve_conflict("merge");

	o.branch1 = argv[++i];
	o.branch2 = argv[++i];

	إذا (repo_get_oid(the_repository, o.branch1, &h1))
		die(_("could not resolve ref '%s'"), o.branch1);
	إذا (repo_get_oid(the_repository, o.branch2, &h2))
		die(_("could not resolve ref '%s'"), o.branch2);

	o.branch1 = better1 = better_branch_name(o.branch1);
	o.branch2 = better2 = better_branch_name(o.branch2);

	إذا (o.verbosity >= 3)
		چاپ_کن(_("Merging %s with %s\n"), o.branch1, o.branch2);

	failed = ادغام_ort_generic(&o, &h1, &h2, bases_شمار, bases, &نتیجه);

	حرر(better1);
	حرر(better2);

	إذا (failed < 0)
		أرجع 128; /* die() error code */
	أرجع failed;
}
