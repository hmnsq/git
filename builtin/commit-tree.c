/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "object-name.h"
#شامل "odb.h"

#شامل "commit.h"
#شامل "parse-options.h"

ایستا ثابت نویسه * ثابت commit_درخت_usage[] = {
	N_("git commit-tree <tree> [(-p <parent>)...]"),
	N_("git commit-tree [(-p <parent>)...] [-S[<keyid>]] [(-m <message>)...]\n"
	   "                [(-F <file>)...] <tree>"),
	پوچ
};

ایستا ثابت نویسه *sign_commit;

ایستا تهی new_parent(ساختار commit *parent, ساختار commit_list **parents_p)
{
	ساختار object_id *oid = &parent->object.oid;
	ساختار commit_list *parents;
	برای (parents = *parents_p; parents; parents = parents->next) {
		اگر (parents->item == parent) {
			error(_("duplicate parent %s ignored"), oid_to_hex(oid));
			بازگشت;
		}
		parents_p = &parents->next;
	}
	commit_list_درج(parent, parents_p);
}

ایستا صحیح parse_parent_arg_callback(ثابت ساختار option *opt,
		ثابت نویسه *arg, صحیح unset)
{
	ساختار object_id oid;
	ساختار commit_list **parents = opt->مقدار;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	اگر (repo_get_oid_commit(the_repository, arg, &oid))
		die(_("not a valid object name %s"), arg);

	odb_assert_oid_type(the_repository->objects, &oid, OBJ_COMMIT);
	new_parent(lookup_commit(the_repository, &oid), parents);
	بازگشت 0;
}

ایستا صحیح parse_message_arg_callback(ثابت ساختار option *opt,
		ثابت نویسه *arg, صحیح unset)
{
	ساختار strbuf *buf = opt->مقدار;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	اگر (buf->len)
		strbuf_addch(buf, '\n');
	strbuf_addstr(buf, arg);
	strbuf_complete_سطر(buf);

	بازگشت 0;
}

ایستا صحیح parse_file_arg_callback(ثابت ساختار option *opt,
		ثابت نویسه *arg, صحیح unset)
{
	صحیح fd;
	ساختار strbuf *buf = opt->مقدار;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	اگر (buf->len)
		strbuf_addch(buf, '\n');
	اگر (!strcmp(arg, "-"))
		fd = 0;
	وگرنه {
		fd = xopen(arg, O_RDONLY);
	}
	اگر (strbuf_read(buf, fd, 0) < 0)
		die_errno(_("git commit-tree: failed to read '%s'"), arg);
	اگر (fd && close(fd))
		die_errno(_("git commit-tree: failed to close '%s'"), arg);

	بازگشت 0;
}

صحیح cmd_commit_درخت(صحیح argc,
		    ثابت نویسه **argv,
		    ثابت نویسه *prefix,
		    ساختار repository *repo UNUSED)
{
	ایستا ساختار strbuf buffer = STRBUF_INIT;
	ساختار commit_list *parents = پوچ;
	ساختار object_id درخت_oid;
	ساختار object_id commit_oid;

	ساختار option options[] = {
		OPT_CALLBACK_F('p', پوچ, &parents, N_("parent"),
			N_("id of a parent commit object"), PARSE_OPT_NONEG,
			parse_parent_arg_callback),
		OPT_CALLBACK_F('m', پوچ, &buffer, N_("message"),
			N_("commit message"), PARSE_OPT_NONEG,
			parse_message_arg_callback),
		OPT_CALLBACK_F('F', پوچ, &buffer, N_("file"),
			N_("read commit log message from file"), PARSE_OPT_NONEG,
			parse_file_arg_callback),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.مقدار = &sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_END()
	};
	صحیح ret;

	repo_config(the_repository, git_default_config, پوچ);

	show_usage_with_options_if_asked(argc, argv,
					 commit_درخت_usage, options);

	argc = parse_options(argc, argv, prefix, options, commit_درخت_usage, 0);

	اگر (argc != 1)
		die(_("must give exactly one tree"));

	اگر (repo_get_oid_درخت(the_repository, argv[0], &درخت_oid))
		die(_("not a valid object name %s"), argv[0]);

	اگر (!buffer.len) {
		اگر (strbuf_read(&buffer, 0, 0) < 0)
			die_errno(_("git commit-tree: failed to read"));
	}

	اگر (commit_درخت(buffer.buf, buffer.len, &درخت_oid, parents, &commit_oid,
			پوچ, sign_commit)) {
		ret = 1;
		برو out;
	}

	printf("%s\n", oid_to_hex(&commit_oid));
	ret = 0;

out:
	commit_list_free(parents);
	strbuf_release(&buffer);
	بازگشت ret;
}
