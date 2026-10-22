/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "object-name.h"
#شامل "odb.h"

#شامل "commit.h"
#شامل "parse-options.h"

ساکن ثابت حرف * ثابت commit_درخت_usage[] = {
	N_("git commit-tree <tree> [(-p <parent>)...]"),
	N_("git commit-tree [(-p <parent>)...] [-S[<keyid>]] [(-m <message>)...]\n"
	   "                [(-F <file>)...] <tree>"),
	تہی
};

ساکن ثابت حرف *sign_commit;

ساکن خالی new_والد(ساخت commit *والد, ساخت commit_list **parents_p)
{
	ساخت object_id *oid = &والد->object.oid;
	ساخت commit_list *parents;
	برائے (parents = *parents_p; parents; parents = parents->اگلا) {
		اگر (parents->شے == والد) {
			error(_("duplicate parent %s ignored"), oid_مقصد_hex(oid));
			واپس;
		}
		parents_p = &parents->اگلا;
	}
	commit_list_داخل_کرو(والد, parents_p);
}

ساکن صحیح parse_والد_arg_callback(ثابت ساخت option *opt,
		ثابت حرف *arg, صحیح unset)
{
	ساخت object_id oid;
	ساخت commit_list **parents = opt->قدر;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	اگر (repo_get_oid_commit(the_repository, arg, &oid))
		die(_("not a valid object name %s"), arg);

	odb_assert_oid_type(the_repository->objects, &oid, OBJ_COMMIT);
	new_والد(lookup_commit(the_repository, &oid), parents);
	واپس 0;
}

ساکن صحیح parse_پیغام_arg_callback(ثابت ساخت option *opt,
		ثابت حرف *arg, صحیح unset)
{
	ساخت strbuf *buf = opt->قدر;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	اگر (buf->len)
		strbuf_addch(buf, '\n');
	strbuf_addstr(buf, arg);
	strbuf_complete_سطر(buf);

	واپس 0;
}

ساکن صحیح parse_file_arg_callback(ثابت ساخت option *opt,
		ثابت حرف *arg, صحیح unset)
{
	صحیح fd;
	ساخت strbuf *buf = opt->قدر;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	اگر (buf->len)
		strbuf_addch(buf, '\n');
	اگر (!strcmp(arg, "-"))
		fd = 0;
	ورنہ {
		fd = xopen(arg, O_RDONLY);
	}
	اگر (strbuf_read(buf, fd, 0) < 0)
		die_errno(_("git commit-tree: failed to read '%s'"), arg);
	اگر (fd && close(fd))
		die_errno(_("git commit-tree: failed to close '%s'"), arg);

	واپس 0;
}

صحیح cmd_commit_درخت(صحیح argc,
		    ثابت حرف **argv,
		    ثابت حرف *سابقہ,
		    ساخت repository *repo غیر_مستعمل)
{
	ساکن ساخت strbuf بفر = STRBUF_INIT;
	ساخت commit_list *parents = تہی;
	ساخت object_id درخت_oid;
	ساخت object_id commit_oid;

	ساخت option options[] = {
		OPT_CALLBACK_F('p', تہی, &parents, N_("parent"),
			N_("id of a parent commit object"), PARSE_OPT_NONEG,
			parse_والد_arg_callback),
		OPT_CALLBACK_F('m', تہی, &بفر, N_("message"),
			N_("commit message"), PARSE_OPT_NONEG,
			parse_پیغام_arg_callback),
		OPT_CALLBACK_F('F', تہی, &بفر, N_("file"),
			N_("read commit log message from file"), PARSE_OPT_NONEG,
			parse_file_arg_callback),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.قدر = &sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_انتہا()
	};
	صحیح ret;

	repo_config(the_repository, git_default_config, تہی);

	show_usage_with_options_if_asked(argc, argv,
					 commit_درخت_usage, options);

	argc = parse_options(argc, argv, سابقہ, options, commit_درخت_usage, 0);

	اگر (argc != 1)
		die(_("must give exactly one tree"));

	اگر (repo_get_oid_درخت(the_repository, argv[0], &درخت_oid))
		die(_("not a valid object name %s"), argv[0]);

	اگر (!بفر.len) {
		اگر (strbuf_read(&بفر, 0, 0) < 0)
			die_errno(_("git commit-tree: failed to read"));
	}

	اگر (commit_درخت(بفر.buf, بفر.len, &درخت_oid, parents, &commit_oid,
			تہی, sign_commit)) {
		ret = 1;
		جاؤ برآمد;
	}

	لکھو("%s\n", oid_مقصد_hex(&commit_oid));
	ret = 0;

برآمد:
	commit_list_free(parents);
	strbuf_release(&بفر);
	واپس ret;
}
