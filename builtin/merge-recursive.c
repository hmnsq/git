#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "advice.h"
#شامل "gettext.h"
#شامل "hash.h"
#شامل "merge-ort-wrappers.h"
#شامل "object-name.h"

ساکن ثابت حرف builtin_ملاؤ_recursive_usage[] =
	"git %s <base>... -- <head> <remote> ...";

ساکن حرف *better_branch_name(ثابت حرف *branch)
{
	ساکن حرف githead_env[8 + GIT_MAX_HEXSZ + 1];
	حرف *name;

	اگر (لڑی_لمبائی(branch) != the_hash_algo->hexsz)
		واپس xstrdup(branch);
	xsnprintf(githead_env, حجم(githead_env), "GITHEAD_%s", branch);
	name = getenv(githead_env);
	واپس xstrdup(name ? name : branch);
}

صحیح cmd_ملاؤ_recursive(صحیح argc,
			ثابت حرف **argv,
			ثابت حرف *سابقہ غیر_مستعمل,
			ساخت repository *repo غیر_مستعمل)
{
	ساخت object_id bases[21];
	غیرعلامتی bases_گنتی = 0;
	صحیح i, failed;
	ساخت object_id h1, h2;
	ساخت ملاؤ_options o;
	حرف *better1, *better2;
	ساخت commit *نتیجہ;

	init_basic_ملاؤ_options(&o, the_repository);
	اگر (argv[0] && سرے_with(argv[0], "-subtree"))
		o.subtree_سرکاؤ = "";

	اگر (argc == 2 && (!strcmp(argv[1], "-h") ||
			  !strcmp(argv[1], "--help-all"))) {
		ساخت strbuf پیغام = STRBUF_INIT;
		strbuf_addf(&پیغام, builtin_ملاؤ_recursive_usage, argv[0]);
		show_usage_if_asked(argc, argv, پیغام.buf);
	}

	اگر (argc < 4)
		usagef(builtin_ملاؤ_recursive_usage, argv[0]);

	برائے (i = 1; i < argc; ++i) {
		ثابت حرف *arg = argv[i];

		اگر (starts_with(arg, "--")) {
			اگر (!arg[2])
				توڑو;
			اگر (parse_ملاؤ_opt(&o, arg + 2))
				die(_("unknown option %s"), arg);
			جاری;
		}
		اگر (bases_گنتی < ARRAY_سائز(bases)-1) {
			اگر (repo_get_oid(the_repository, argv[i], &bases[bases_گنتی++]))
				die(_("could not parse object '%s'"), argv[i]);
		}
		ورنہ
			warning(Q_("cannot handle more than %d base. "
				   "Ignoring %s.",
				   "cannot handle more than %d bases. "
				   "Ignoring %s.",
				    ARRAY_سائز(bases)-1),
				(صحیح)ARRAY_سائز(bases)-1, argv[i]);
	}
	اگر (argc - i != 3) /* "--" "<head>" "<remote>" */
		die(_("not handling anything other than two heads merge."));

	اگر (repo_read_اشاریہ_unmerged(the_repository))
		die_resolve_conflict("merge");

	o.branch1 = argv[++i];
	o.branch2 = argv[++i];

	اگر (repo_get_oid(the_repository, o.branch1, &h1))
		die(_("could not resolve ref '%s'"), o.branch1);
	اگر (repo_get_oid(the_repository, o.branch2, &h2))
		die(_("could not resolve ref '%s'"), o.branch2);

	o.branch1 = better1 = better_branch_name(o.branch1);
	o.branch2 = better2 = better_branch_name(o.branch2);

	اگر (o.verbosity >= 3)
		لکھو(_("Merging %s with %s\n"), o.branch1, o.branch2);

	failed = ملاؤ_ort_generic(&o, &h1, &h2, bases_گنتی, bases, &نتیجہ);

	آزاد(better1);
	آزاد(better2);

	اگر (failed < 0)
		واپس 128; /* die() error code */
	واپس failed;
}
