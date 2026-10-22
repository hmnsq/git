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

ساكن ثابت حرف * ثابت commit_tree_usage[] = {
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
	עבור (parents = *parents_p; parents; parents = parents->next) {
		אם (parents->item == parent) {
			error(_("duplicate parent %s ignored"), oid_to_hex(oid));
			החזר;
		}
		parents_p = &parents->next;
	}
	commit_list_insert(parent, parents_p);
}

ساكن صحيح parse_parent_arg_callback(ثابت بنية option *opt,
		ثابت حرف *arg, صحيح unset)
{
	بنية object_id oid;
	بنية commit_list **parents = opt->value;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	אם (repo_get_oid_commit(the_repository, arg, &oid))
		die(_("not a valid object name %s"), arg);

	odb_assert_oid_type(the_repository->objects, &oid, OBJ_COMMIT);
	new_parent(lookup_commit(the_repository, &oid), parents);
	החזר 0;
}

ساكن صحيح parse_message_arg_callback(ثابت بنية option *opt,
		ثابت حرف *arg, صحيح unset)
{
	بنية strbuf *buf = opt->value;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	אם (buf->len)
		strbuf_addch(buf, '\n');
	strbuf_addstr(buf, arg);
	strbuf_complete_line(buf);

	החזר 0;
}

ساكن صحيح parse_file_arg_callback(ثابت بنية option *opt,
		ثابت حرف *arg, صحيح unset)
{
	صحيح fd;
	بنية strbuf *buf = opt->value;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	אם (buf->len)
		strbuf_addch(buf, '\n');
	אם (!strcmp(arg, "-"))
		fd = 0;
	אחרת {
		fd = xopen(arg, O_RDONLY);
	}
	אם (strbuf_read(buf, fd, 0) < 0)
		die_errno(_("git commit-tree: failed to read '%s'"), arg);
	אם (fd && close(fd))
		die_errno(_("git commit-tree: failed to close '%s'"), arg);

	החזר 0;
}

صحيح cmd_commit_tree(صحيح argc,
		    ثابت حرف **argv,
		    ثابت حرف *prefix,
		    بنية repository *repo UNUSED)
{
	ساكن بنية strbuf buffer = STRBUF_INIT;
	بنية commit_list *parents = عدم;
	بنية object_id tree_oid;
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
			.value = &sign_commit,
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
					 commit_tree_usage, options);

	argc = parse_options(argc, argv, prefix, options, commit_tree_usage, 0);

	אם (argc != 1)
		die(_("must give exactly one tree"));

	אם (repo_get_oid_tree(the_repository, argv[0], &tree_oid))
		die(_("not a valid object name %s"), argv[0]);

	אם (!buffer.len) {
		אם (strbuf_read(&buffer, 0, 0) < 0)
			die_errno(_("git commit-tree: failed to read"));
	}

	אם (commit_tree(buffer.buf, buffer.len, &tree_oid, parents, &commit_oid,
			عدم, sign_commit)) {
		ret = 1;
		اذهب out;
	}

	printf("%s\n", oid_to_hex(&commit_oid));
	ret = 0;

out:
	commit_list_free(parents);
	strbuf_release(&buffer);
	החזר ret;
}
