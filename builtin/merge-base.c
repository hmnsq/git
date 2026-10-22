#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "config.h"
#شامل "commit.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "object-name.h"
#شامل "parse-options.h"
#شامل "commit-reach.h"

ساکن صحیح show_ملاؤ_بنیاد(ساخت commit **rev, سائز_t rev_nr, صحیح show_all)
{
	شمار ملاؤ_بنیاد_flags flags = show_all ? ملاؤ_بنیاد_ڈھونڈو_ALL : 0;
	ساخت commit_list *نتیجہ = تہی, *r;

	اگر (repo_get_ملاؤ_bases_many_dirty(the_repository, rev[0],
					    rev_nr - 1, rev + 1,
					    flags, &نتیجہ) < 0) {
		commit_list_free(نتیجہ);
		واپس -1;
	}

	اگر (!نتیجہ)
		واپس 1;

	برائے (r = نتیجہ; r; r = r->اگلا) {
		لکھو("%s\n", oid_مقصد_hex(&r->شے->object.oid));
		اگر (!show_all)
			توڑو;
	}

	commit_list_free(نتیجہ);
	واپس 0;
}

ساکن ثابت حرف * ثابت ملاؤ_بنیاد_usage[] = {
	N_("git merge-base [-a | --all] <commit> <commit>..."),
	N_("git merge-base [-a | --all] --octopus <commit>..."),
	N_("git merge-base --is-ancestor <commit> <commit>"),
	N_("git merge-base --independent <commit>..."),
	N_("git merge-base --fork-point <ref> [<commit>]"),
	تہی
};

ساکن ساخت commit *get_commit_reference(ثابت حرف *arg)
{
	ساخت object_id revkey;
	ساخت commit *r;

	اگر (repo_get_oid(the_repository, arg, &revkey))
		die("Not a valid object name %s", arg);
	r = lookup_commit_reference(the_repository, &revkey);
	اگر (!r)
		die("Not a valid commit name %s", arg);

	واپس r;
}

ساکن صحیح handle_independent(صحیح گنتی, ثابت حرف **دلائل)
{
	ساخت commit_list *revs = تہی, *rev;
	صحیح i;

	برائے (i = گنتی - 1; i >= 0; i--)
		commit_list_داخل_کرو(get_commit_reference(دلائل[i]), &revs);

	reduce_heads_replace(&revs);

	اگر (!revs)
		واپس 1;

	برائے (rev = revs; rev; rev = rev->اگلا)
		لکھو("%s\n", oid_مقصد_hex(&rev->شے->object.oid));

	commit_list_free(revs);
	واپس 0;
}

ساکن صحیح handle_octopus(صحیح گنتی, ثابت حرف **دلائل, صحیح show_all)
{
	ساخت commit_list *revs = تہی;
	ساخت commit_list *نتیجہ = تہی, *rev;
	صحیح i;

	برائے (i = گنتی - 1; i >= 0; i--)
		commit_list_داخل_کرو(get_commit_reference(دلائل[i]), &revs);

	اگر (get_octopus_ملاؤ_bases(revs, &نتیجہ) < 0) {
		commit_list_free(revs);
		commit_list_free(نتیجہ);
		واپس 128;
	}
	commit_list_free(revs);
	reduce_heads_replace(&نتیجہ);

	اگر (!نتیجہ)
		واپس 1;

	برائے (rev = نتیجہ; rev; rev = rev->اگلا) {
		لکھو("%s\n", oid_مقصد_hex(&rev->شے->object.oid));
		اگر (!show_all)
			توڑو;
	}

	commit_list_free(نتیجہ);
	واپس 0;
}

ساکن صحیح handle_is_ancestor(صحیح argc, ثابت حرف **argv)
{
	ساخت commit *one, *two;
	صحیح ret;

	اگر (argc != 2)
		die("--is-ancestor takes exactly two commits");
	one = get_commit_reference(argv[0]);
	two = get_commit_reference(argv[1]);
	ret = repo_اندر_ملاؤ_bases(the_repository, one, two);
	اگر (ret < 0)
		باہر_نکلو(128);
	اگر (ret)
		واپس 0;
	ورنہ
		واپس 1;
}

ساکن صحیح handle_fork_نقطہ(صحیح argc, ثابت حرف **argv)
{
	ساخت object_id oid;
	ساخت commit *derived, *fork_نقطہ;
	ثابت حرف *commitname;

	commitname = (argc == 2) ? argv[1] : "HEAD";
	اگر (repo_get_oid(the_repository, commitname, &oid))
		die("Not a valid object name: '%s'", commitname);

	derived = lookup_commit_reference(the_repository, &oid);

	fork_نقطہ = get_fork_نقطہ(argv[0], derived);

	اگر (!fork_نقطہ)
		واپس 1;

	لکھو("%s\n", oid_مقصد_hex(&fork_نقطہ->object.oid));
	واپس 0;
}

صحیح cmd_ملاؤ_بنیاد(صحیح argc,
		   ثابت حرف **argv,
		   ثابت حرف *سابقہ,
		   ساخت repository *repo غیر_مستعمل)
{
	ساخت commit **rev;
	سائز_t rev_nr = 0;
	صحیح show_all = 0;
	صحیح cmdmode = 0;
	صحیح ret;

	ساخت option options[] = {
		OPT_BOOL('a', "all", &show_all, N_("output all common ancestors")),
		OPT_CMDMODE(0, "octopus", &cmdmode,
			    N_("find ancestors for a single n-way merge"), 'o'),
		OPT_CMDMODE(0, "independent", &cmdmode,
			    N_("list revs not reachable from others"), 'r'),
		OPT_CMDMODE(0, "is-ancestor", &cmdmode,
			    N_("is the first one ancestor of the other?"), 'a'),
		OPT_CMDMODE(0, "fork-point", &cmdmode,
			    N_("find where <commit> forked from reflog of <ref>"), 'f'),
		OPT_انتہا()
	};

	repo_config(the_repository, git_default_config, تہی);
	argc = parse_options(argc, argv, سابقہ, options, ملاؤ_بنیاد_usage, 0);

	اگر (cmdmode == 'a') {
		اگر (argc < 2)
			usage_with_options(ملاؤ_بنیاد_usage, options);
		اگر (show_all)
			die(_("options '%s' and '%s' cannot be used together"),
			    "--is-ancestor", "--all");
		واپس handle_is_ancestor(argc, argv);
	}

	اگر (cmdmode == 'r' && show_all)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--independent", "--all");

	اگر (cmdmode == 'o')
		واپس handle_octopus(argc, argv, show_all);

	اگر (cmdmode == 'r')
		واپس handle_independent(argc, argv);

	اگر (cmdmode == 'f') {
		اگر (argc < 1 || 2 < argc)
			usage_with_options(ملاؤ_بنیاد_usage, options);
		واپس handle_fork_نقطہ(argc, argv);
	}

	اگر (argc < 2)
		usage_with_options(ملاؤ_بنیاد_usage, options);

	ALLOC_ARRAY(rev, argc);
	جب (argc-- > 0)
		rev[rev_nr++] = get_commit_reference(*argv++);
	ret = show_ملاؤ_بنیاد(rev, rev_nr, show_all);
	آزاد(rev);
	واپس ret;
}
