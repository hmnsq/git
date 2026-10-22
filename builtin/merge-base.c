#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "commit.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "object-name.h"
#ضمن "parse-options.h"
#ضمن "commit-reach.h"

ساكن صحيح show_ادغام_پایه(بنية commit **rev, اندازه_t rev_nr, صحيح show_all)
{
	تعداد ادغام_پایه_flags flags = show_all ? ادغام_پایه_پیدا_کن_ALL : 0;
	بنية commit_list *نتیجه = عدم, *r;

	إذا (repo_get_ادغام_bases_many_dirty(the_repository, rev[0],
					    rev_nr - 1, rev + 1,
					    flags, &نتیجه) < 0) {
		commit_list_free(نتیجه);
		أرجع -1;
	}

	إذا (!نتیجه)
		أرجع 1;

	لكل (r = نتیجه; r; r = r->بعدی) {
		چاپ_کن("%s\n", oid_مقصد_hex(&r->قلم->object.oid));
		إذا (!show_all)
			اكسر;
	}

	commit_list_free(نتیجه);
	أرجع 0;
}

ساكن ثابت حرف * ثابت ادغام_پایه_usage[] = {
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

ساكن صحيح handle_independent(صحيح شمار, ثابت حرف **آرگومانها)
{
	بنية commit_list *revs = عدم, *rev;
	صحيح i;

	لكل (i = شمار - 1; i >= 0; i--)
		commit_list_درج(get_commit_reference(آرگومانها[i]), &revs);

	reduce_heads_replace(&revs);

	إذا (!revs)
		أرجع 1;

	لكل (rev = revs; rev; rev = rev->بعدی)
		چاپ_کن("%s\n", oid_مقصد_hex(&rev->قلم->object.oid));

	commit_list_free(revs);
	أرجع 0;
}

ساكن صحيح handle_octopus(صحيح شمار, ثابت حرف **آرگومانها, صحيح show_all)
{
	بنية commit_list *revs = عدم;
	بنية commit_list *نتیجه = عدم, *rev;
	صحيح i;

	لكل (i = شمار - 1; i >= 0; i--)
		commit_list_درج(get_commit_reference(آرگومانها[i]), &revs);

	إذا (get_octopus_ادغام_bases(revs, &نتیجه) < 0) {
		commit_list_free(revs);
		commit_list_free(نتیجه);
		أرجع 128;
	}
	commit_list_free(revs);
	reduce_heads_replace(&نتیجه);

	إذا (!نتیجه)
		أرجع 1;

	لكل (rev = نتیجه; rev; rev = rev->بعدی) {
		چاپ_کن("%s\n", oid_مقصد_hex(&rev->قلم->object.oid));
		إذا (!show_all)
			اكسر;
	}

	commit_list_free(نتیجه);
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
	ret = repo_درون_ادغام_bases(the_repository, one, two);
	إذا (ret < 0)
		خروج(128);
	إذا (ret)
		أرجع 0;
	وإلا
		أرجع 1;
}

ساكن صحيح handle_fork_نقطه(صحيح argc, ثابت حرف **argv)
{
	بنية object_id oid;
	بنية commit *derived, *fork_نقطه;
	ثابت حرف *commitname;

	commitname = (argc == 2) ? argv[1] : "HEAD";
	إذا (repo_get_oid(the_repository, commitname, &oid))
		die("Not a valid object name: '%s'", commitname);

	derived = lookup_commit_reference(the_repository, &oid);

	fork_نقطه = get_fork_نقطه(argv[0], derived);

	إذا (!fork_نقطه)
		أرجع 1;

	چاپ_کن("%s\n", oid_مقصد_hex(&fork_نقطه->object.oid));
	أرجع 0;
}

صحيح cmd_ادغام_پایه(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *پیشوند,
		   بنية repository *repo استفادهنشده)
{
	بنية commit **rev;
	اندازه_t rev_nr = 0;
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
		OPT_انتها()
	};

	repo_config(the_repository, git_default_config, عدم);
	argc = parse_options(argc, argv, پیشوند, options, ادغام_پایه_usage, 0);

	إذا (cmdmode == 'a') {
		إذا (argc < 2)
			usage_with_options(ادغام_پایه_usage, options);
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
			usage_with_options(ادغام_پایه_usage, options);
		أرجع handle_fork_نقطه(argc, argv);
	}

	إذا (argc < 2)
		usage_with_options(ادغام_پایه_usage, options);

	ALLOC_ARRAY(rev, argc);
	طالما (argc-- > 0)
		rev[rev_nr++] = get_commit_reference(*argv++);
	ret = show_ادغام_پایه(rev, rev_nr, show_all);
	حرر(rev);
	أرجع ret;
}
