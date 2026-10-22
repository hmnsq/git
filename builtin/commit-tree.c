/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "odb.h"

#അന്തര്ഭാവ "commit.h"
#അന്തര്ഭാവ "parse-options.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം commit_വൃക്ഷം_usage[] = {
	N_("git commit-tree <tree> [(-p <parent>)...]"),
	N_("git commit-tree [(-p <parent>)...] [-S[<keyid>]] [(-m <message>)...]\n"
	   "                [(-F <file>)...] <tree>"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *sign_commit;

സ്ഥിതം ശൂന്യം new_parent(ഘടന commit *parent, ഘടന commit_list **parents_p)
{
	ഘടന object_id *oid = &parent->object.oid;
	ഘടന commit_list *parents;
	വേണ്ടി (parents = *parents_p; parents; parents = parents->next) {
		എങ്കിൽ (parents->item == parent) {
			error(_("duplicate parent %s ignored"), oid_to_hex(oid));
			തിരികെ;
		}
		parents_p = &parents->next;
	}
	commit_list_തിരുകുക(parent, parents_p);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_parent_arg_callback(സ്ഥിരം ഘടന option *opt,
		സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന object_id oid;
	ഘടന commit_list **parents = opt->മൂല്യം;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	എങ്കിൽ (repo_get_oid_commit(the_repository, arg, &oid))
		die(_("not a valid object name %s"), arg);

	odb_assert_oid_type(the_repository->objects, &oid, OBJ_COMMIT);
	new_parent(lookup_commit(the_repository, &oid), parents);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_message_arg_callback(സ്ഥിരം ഘടന option *opt,
		സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന strbuf *buf = opt->മൂല്യം;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	എങ്കിൽ (buf->len)
		strbuf_addch(buf, '\n');
	strbuf_addstr(buf, arg);
	strbuf_complete_വരി(buf);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_file_arg_callback(സ്ഥിരം ഘടന option *opt,
		സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	പൂർണ്ണസംഖ്യ fd;
	ഘടന strbuf *buf = opt->മൂല്യം;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	എങ്കിൽ (buf->len)
		strbuf_addch(buf, '\n');
	എങ്കിൽ (!strcmp(arg, "-"))
		fd = 0;
	അല്ലെങ്കിൽ {
		fd = xopen(arg, O_RDONLY);
	}
	എങ്കിൽ (strbuf_read(buf, fd, 0) < 0)
		die_errno(_("git commit-tree: failed to read '%s'"), arg);
	എങ്കിൽ (fd && close(fd))
		die_errno(_("git commit-tree: failed to close '%s'"), arg);

	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ cmd_commit_വൃക്ഷം(പൂർണ്ണസംഖ്യ argc,
		    സ്ഥിരം അക്ഷരം **argv,
		    സ്ഥിരം അക്ഷരം *prefix,
		    ഘടന repository *repo UNUSED)
{
	സ്ഥിതം ഘടന strbuf buffer = STRBUF_INIT;
	ഘടന commit_list *parents = ശൂന്യ;
	ഘടന object_id വൃക്ഷം_oid;
	ഘടന object_id commit_oid;

	ഘടന option options[] = {
		OPT_CALLBACK_F('p', ശൂന്യ, &parents, N_("parent"),
			N_("id of a parent commit object"), PARSE_OPT_NONEG,
			parse_parent_arg_callback),
		OPT_CALLBACK_F('m', ശൂന്യ, &buffer, N_("message"),
			N_("commit message"), PARSE_OPT_NONEG,
			parse_message_arg_callback),
		OPT_CALLBACK_F('F', ശൂന്യ, &buffer, N_("file"),
			N_("read commit log message from file"), PARSE_OPT_NONEG,
			parse_file_arg_callback),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.മൂല്യം = &sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_END()
	};
	പൂർണ്ണസംഖ്യ ret;

	repo_config(the_repository, git_default_config, ശൂന്യ);

	show_usage_with_options_if_asked(argc, argv,
					 commit_വൃക്ഷം_usage, options);

	argc = parse_options(argc, argv, prefix, options, commit_വൃക്ഷം_usage, 0);

	എങ്കിൽ (argc != 1)
		die(_("must give exactly one tree"));

	എങ്കിൽ (repo_get_oid_വൃക്ഷം(the_repository, argv[0], &വൃക്ഷം_oid))
		die(_("not a valid object name %s"), argv[0]);

	എങ്കിൽ (!buffer.len) {
		എങ്കിൽ (strbuf_read(&buffer, 0, 0) < 0)
			die_errno(_("git commit-tree: failed to read"));
	}

	എങ്കിൽ (commit_വൃക്ഷം(buffer.buf, buffer.len, &വൃക്ഷം_oid, parents, &commit_oid,
			ശൂന്യ, sign_commit)) {
		ret = 1;
		പോകുക out;
	}

	printf("%s\n", oid_to_hex(&commit_oid));
	ret = 0;

out:
	commit_list_free(parents);
	strbuf_release(&buffer);
	തിരികെ ret;
}
