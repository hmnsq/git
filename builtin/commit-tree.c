/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "object-name.h"
#ضمن "odb.h"

#ضمن "commit.h"
#ضمن "parse-options.h"

ساكن ثابت حرف * ثابت commit_درەخت_usage[] = {
	N_("git commit-tree <tree> [(-p <parent>)...]"),
	N_("git commit-tree [(-p <parent>)...] [-S[<keyid>]] [(-m <message>)...]\n"
	   "                [(-F <file>)...] <tree>"),
	عدم
};

ساكن ثابت حرف *sign_commit;

ساكن فراغ new_parent(بنية commit *parent, بنية commit_list **parents_p)
{
	بنية object_id *oid = &parent->object.oid;
	بنية commit_list *parents;
	لكل (parents = *parents_p; parents; parents = parents->next) {
		إذا (parents->item == parent) {
			error(_("duplicate parent %s ignored"), oid_to_hex(oid));
			أرجع;
		}
		parents_p = &parents->next;
	}
	commit_list_زیادکردن(parent, parents_p);
}

ساكن صحيح parse_parent_arg_callback(ثابت بنية option *opt,
		ثابت حرف *arg, صحيح unset)
{
	بنية object_id oid;
	بنية commit_list **parents = opt->بەها;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	إذا (repo_get_oid_commit(the_repository, arg, &oid))
		die(_("not a valid object name %s"), arg);

	odb_assert_oid_type(the_repository->objects, &oid, OBJ_COMMIT);
	new_parent(lookup_commit(the_repository, &oid), parents);
	أرجع 0;
}

ساكن صحيح parse_message_arg_callback(ثابت بنية option *opt,
		ثابت حرف *arg, صحيح unset)
{
	بنية strbuf *buf = opt->بەها;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	إذا (buf->len)
		strbuf_addch(buf, '\n');
	strbuf_addstr(buf, arg);
	strbuf_complete_هێڵ(buf);

	أرجع 0;
}

ساكن صحيح parse_file_arg_callback(ثابت بنية option *opt,
		ثابت حرف *arg, صحيح unset)
{
	صحيح fd;
	بنية strbuf *buf = opt->بەها;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	إذا (buf->len)
		strbuf_addch(buf, '\n');
	إذا (!strcmp(arg, "-"))
		fd = 0;
	وإلا {
		fd = xopen(arg, O_RDONLY);
	}
	إذا (strbuf_read(buf, fd, 0) < 0)
		die_errno(_("git commit-tree: failed to read '%s'"), arg);
	إذا (fd && close(fd))
		die_errno(_("git commit-tree: failed to close '%s'"), arg);

	أرجع 0;
}

صحيح cmd_commit_درەخت(صحيح argc,
		    ثابت حرف **argv,
		    ثابت حرف *prefix,
		    بنية repository *repo UNUSED)
{
	ساكن بنية strbuf buffer = STRBUF_INIT;
	بنية commit_list *parents = عدم;
	بنية object_id درەخت_oid;
	بنية object_id commit_oid;

	بنية option options[] = {
		OPT_CALLBACK_F('p', عدم, &parents, N_("parent"),
			N_("id of a parent commit object"), PARSE_OPT_NONEG,
			parse_parent_arg_callback),
		OPT_CALLBACK_F('m', عدم, &buffer, N_("message"),
			N_("commit message"), PARSE_OPT_NONEG,
			parse_message_arg_callback),
		OPT_CALLBACK_F('F', عدم, &buffer, N_("file"),
			N_("read commit log message from file"), PARSE_OPT_NONEG,
			parse_file_arg_callback),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.بەها = &sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_END()
	};
	صحيح ret;

	repo_config(the_repository, git_default_config, عدم);

	show_usage_with_options_if_asked(argc, argv,
					 commit_درەخت_usage, options);

	argc = parse_options(argc, argv, prefix, options, commit_درەخت_usage, 0);

	إذا (argc != 1)
		die(_("must give exactly one tree"));

	إذا (repo_get_oid_درەخت(the_repository, argv[0], &درەخت_oid))
		die(_("not a valid object name %s"), argv[0]);

	إذا (!buffer.len) {
		إذا (strbuf_read(&buffer, 0, 0) < 0)
			die_errno(_("git commit-tree: failed to read"));
	}

	إذا (commit_درەخت(buffer.buf, buffer.len, &درەخت_oid, parents, &commit_oid,
			عدم, sign_commit)) {
		ret = 1;
		اذهب out;
	}

	printf("%s\n", oid_to_hex(&commit_oid));
	ret = 0;

out:
	commit_list_free(parents);
	strbuf_release(&buffer);
	أرجع ret;
}
