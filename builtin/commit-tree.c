/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "odb.h"

#অন্তর্ভাব "commit.h"
#অন্তর্ভাব "parse-options.h"

স্থির ধ্রুবক অক্ষর * ধ্রুবক commit_বৃক্ষ_usage[] = {
	N_("git commit-tree <tree> [(-p <parent>)...]"),
	N_("git commit-tree [(-p <parent>)...] [-S[<keyid>]] [(-m <message>)...]\n"
	   "                [(-F <file>)...] <tree>"),
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর *sign_commit;

স্থির শূন্য new_parent(কাঠামো commit *parent, কাঠামো commit_list **parents_p)
{
	কাঠামো object_id *oid = &parent->object.oid;
	কাঠামো commit_list *parents;
	জন্য (parents = *parents_p; parents; parents = parents->পরবর্তী) {
		যদি (parents->item == parent) {
			error(_("duplicate parent %s ignored"), oid_to_hex(oid));
			ফেরত;
		}
		parents_p = &parents->পরবর্তী;
	}
	commit_list_ঢোকাও(parent, parents_p);
}

স্থির পূর্ণসংখ্যা parse_parent_arg_callback(ধ্রুবক কাঠামো option *opt,
		ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	কাঠামো object_id oid;
	কাঠামো commit_list **parents = opt->মান;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	যদি (repo_get_oid_commit(the_repository, arg, &oid))
		die(_("not a valid object name %s"), arg);

	odb_assert_oid_type(the_repository->objects, &oid, OBJ_COMMIT);
	new_parent(lookup_commit(the_repository, &oid), parents);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা parse_message_arg_callback(ধ্রুবক কাঠামো option *opt,
		ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	কাঠামো strbuf *buf = opt->মান;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	যদি (buf->len)
		strbuf_addch(buf, '\n');
	strbuf_addstr(buf, arg);
	strbuf_complete_সারি(buf);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা parse_file_arg_callback(ধ্রুবক কাঠামো option *opt,
		ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	পূর্ণসংখ্যা fd;
	কাঠামো strbuf *buf = opt->মান;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	যদি (buf->len)
		strbuf_addch(buf, '\n');
	যদি (!strcmp(arg, "-"))
		fd = 0;
	অন্যথা {
		fd = xopen(arg, O_RDONLY);
	}
	যদি (strbuf_read(buf, fd, 0) < 0)
		die_errno(_("git commit-tree: failed to read '%s'"), arg);
	যদি (fd && close(fd))
		die_errno(_("git commit-tree: failed to close '%s'"), arg);

	ফেরত 0;
}

পূর্ণসংখ্যা cmd_commit_বৃক্ষ(পূর্ণসংখ্যা argc,
		    ধ্রুবক অক্ষর **argv,
		    ধ্রুবক অক্ষর *prefix,
		    কাঠামো repository *repo UNUSED)
{
	স্থির কাঠামো strbuf বাফার = STRBUF_INIT;
	কাঠামো commit_list *parents = ফাঁকা;
	কাঠামো object_id বৃক্ষ_oid;
	কাঠামো object_id commit_oid;

	কাঠামো option options[] = {
		OPT_CALLBACK_F('p', ফাঁকা, &parents, N_("parent"),
			N_("id of a parent commit object"), PARSE_OPT_NONEG,
			parse_parent_arg_callback),
		OPT_CALLBACK_F('m', ফাঁকা, &বাফার, N_("message"),
			N_("commit message"), PARSE_OPT_NONEG,
			parse_message_arg_callback),
		OPT_CALLBACK_F('F', ফাঁকা, &বাফার, N_("file"),
			N_("read commit log message from file"), PARSE_OPT_NONEG,
			parse_file_arg_callback),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.মান = &sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_END()
	};
	পূর্ণসংখ্যা ret;

	repo_config(the_repository, git_default_config, ফাঁকা);

	show_usage_with_options_if_asked(argc, argv,
					 commit_বৃক্ষ_usage, options);

	argc = parse_options(argc, argv, prefix, options, commit_বৃক্ষ_usage, 0);

	যদি (argc != 1)
		die(_("must give exactly one tree"));

	যদি (repo_get_oid_বৃক্ষ(the_repository, argv[0], &বৃক্ষ_oid))
		die(_("not a valid object name %s"), argv[0]);

	যদি (!বাফার.len) {
		যদি (strbuf_read(&বাফার, 0, 0) < 0)
			die_errno(_("git commit-tree: failed to read"));
	}

	যদি (commit_বৃক্ষ(বাফার.buf, বাফার.len, &বৃক্ষ_oid, parents, &commit_oid,
			ফাঁকা, sign_commit)) {
		ret = 1;
		যাও out;
	}

	printf("%s\n", oid_to_hex(&commit_oid));
	ret = 0;

out:
	commit_list_free(parents);
	strbuf_release(&বাফার);
	ফেরত ret;
}
