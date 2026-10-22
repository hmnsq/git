#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "commit.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "object-name.h"
#ضمن "parse-options.h"
#ضمن "commit-reach.h"

ساكن صحيح show_ملايو_بنياد(بنية commit **rev, سائيز_t rev_nr, صحيح show_all)
{
	تعداد ملايو_بنياد_flags flags = show_all ? ملايو_بنياد_ڳوليو_ALL : 0;
	بنية commit_list *نتيجو = عدم, *r;

	إذا (repo_get_ملايو_bases_many_dirty(the_repository, rev[0],
					    rev_nr - 1, rev + 1,
					    flags, &نتيجو) < 0) {
		commit_list_free(نتيجو);
		أرجع -1;
	}

	إذا (!نتيجو)
		أرجع 1;

	لكل (r = نتيجو; r; r = r->ايندڙ) {
		لکو("%s\n", oid_منزل_hex(&r->شي->object.oid));
		إذا (!show_all)
			اكسر;
	}

	commit_list_free(نتيجو);
	أرجع 0;
}

ساكن ثابت حرف * ثابت ملايو_بنياد_usage[] = {
	N_("git merge-base [-a | --all] <commit> <commit>..."),
	N_("git merge-base [-a | --all] --octopus <commit>..."),
	N_("git merge-base --is-ancestor <commit> <commit>"),
	N_("git merge-base --independent <commit>..."),
	N_("git merge-base --fork-point <ref> [<commit>]"),
	عدم
};

ساكن بنية commit *get_commit_reference(ثابت حرف *arg)
{
	بنية object_id revkey;
	بنية commit *r;

	إذا (repo_get_oid(the_repository, arg, &revkey))
		die("Not a valid object name %s", arg);
	r = lookup_commit_reference(the_repository, &revkey);
	إذا (!r)
		die("Not a valid commit name %s", arg);

	أرجع r;
}

ساكن صحيح handle_independent(صحيح ڳڻپ, ثابت حرف **دليلون)
{
	بنية commit_list *revs = عدم, *rev;
	صحيح i;

	لكل (i = ڳڻپ - 1; i >= 0; i--)
		commit_list_وجهو(get_commit_reference(دليلون[i]), &revs);

	reduce_heads_replace(&revs);

	إذا (!revs)
		أرجع 1;

	لكل (rev = revs; rev; rev = rev->ايندڙ)
		لکو("%s\n", oid_منزل_hex(&rev->شي->object.oid));

	commit_list_free(revs);
	أرجع 0;
}

ساكن صحيح handle_octopus(صحيح ڳڻپ, ثابت حرف **دليلون, صحيح show_all)
{
	بنية commit_list *revs = عدم;
	بنية commit_list *نتيجو = عدم, *rev;
	صحيح i;

	لكل (i = ڳڻپ - 1; i >= 0; i--)
		commit_list_وجهو(get_commit_reference(دليلون[i]), &revs);

	إذا (get_octopus_ملايو_bases(revs, &نتيجو) < 0) {
		commit_list_free(revs);
		commit_list_free(نتيجو);
		أرجع 128;
	}
	commit_list_free(revs);
	reduce_heads_replace(&نتيجو);

	إذا (!نتيجو)
		أرجع 1;

	لكل (rev = نتيجو; rev; rev = rev->ايندڙ) {
		لکو("%s\n", oid_منزل_hex(&rev->شي->object.oid));
		إذا (!show_all)
			اكسر;
	}

	commit_list_free(نتيجو);
	أرجع 0;
}

ساكن صحيح handle_is_ancestor(صحيح argc, ثابت حرف **argv)
{
	بنية commit *one, *two;
	صحيح ret;

	إذا (argc != 2)
		die("--is-ancestor takes exactly two commits");
	one = get_commit_reference(argv[0]);
	two = get_commit_reference(argv[1]);
	ret = repo_اندريون_ملايو_bases(the_repository, one, two);
	إذا (ret < 0)
		ٻاهر_نڪرو(128);
	إذا (ret)
		أرجع 0;
	وإلا
		أرجع 1;
}

ساكن صحيح handle_fork_نقطو(صحيح argc, ثابت حرف **argv)
{
	بنية object_id oid;
	بنية commit *derived, *fork_نقطو;
	ثابت حرف *commitname;

	commitname = (argc == 2) ? argv[1] : "HEAD";
	إذا (repo_get_oid(the_repository, commitname, &oid))
		die("Not a valid object name: '%s'", commitname);

	derived = lookup_commit_reference(the_repository, &oid);

	fork_نقطو = get_fork_نقطو(argv[0], derived);

	إذا (!fork_نقطو)
		أرجع 1;

	لکو("%s\n", oid_منزل_hex(&fork_نقطو->object.oid));
	أرجع 0;
}

صحيح cmd_ملايو_بنياد(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *اڳياڙي,
		   بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية commit **rev;
	سائيز_t rev_nr = 0;
	صحيح show_all = 0;
	صحيح cmdmode = 0;
	صحيح ret;

	بنية option options[] = {
		OPT_BOOL('a', "all", &show_all, N_("output all common ancestors")),
		OPT_CMDMODE(0, "octopus", &cmdmode,
			    N_("find ancestors for a single n-way merge"), 'o'),
		OPT_CMDMODE(0, "independent", &cmdmode,
			    N_("list revs not reachable from others"), 'r'),
		OPT_CMDMODE(0, "is-ancestor", &cmdmode,
			    N_("is the first one ancestor of the other?"), 'a'),
		OPT_CMDMODE(0, "fork-point", &cmdmode,
			    N_("find where <commit> forked from reflog of <ref>"), 'f'),
		OPT_پڇاڙي()
	};

	repo_config(the_repository, git_default_config, عدم);
	argc = parse_options(argc, argv, اڳياڙي, options, ملايو_بنياد_usage, 0);

	إذا (cmdmode == 'a') {
		إذا (argc < 2)
			usage_with_options(ملايو_بنياد_usage, options);
		إذا (show_all)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--is-ancestor", "--all");
		أرجع handle_is_ancestor(argc, argv);
	}

	إذا (cmdmode == 'r' && show_all)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--independent", "--all");

	إذا (cmdmode == 'o')
		أرجع handle_octopus(argc, argv, show_all);

	إذا (cmdmode == 'r')
		أرجع handle_independent(argc, argv);

	إذا (cmdmode == 'f') {
		إذا (argc < 1 || 2 < argc)
			usage_with_options(ملايو_بنياد_usage, options);
		أرجع handle_fork_نقطو(argc, argv);
	}

	إذا (argc < 2)
		usage_with_options(ملايو_بنياد_usage, options);

	ALLOC_ARRAY(rev, argc);
	طالما (argc-- > 0)
		rev[rev_nr++] = get_commit_reference(*argv++);
	ret = show_ملايو_بنياد(rev, rev_nr, show_all);
	حرر(rev);
	أرجع ret;
}
