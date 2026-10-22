/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "commit.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ commit_ਰੁੱਖ_usage[] = {
	N_("git commit-tree <tree> [(-p <parent>)...]"),
	N_("git commit-tree [(-p <parent>)...] [-S[<keyid>]] [(-m <message>)...]\n"
	   "                [(-F <file>)...] <tree>"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *sign_commit;

ਸਥਿਰ ਖਾਲੀ new_ਮਾਪਾ(ਬਣਤਰ commit *ਮਾਪਾ, ਬਣਤਰ commit_list **parents_p)
{
	ਬਣਤਰ object_id *oid = &ਮਾਪਾ->object.oid;
	ਬਣਤਰ commit_list *parents;
	ਲਈ (parents = *parents_p; parents; parents = parents->ਅਗਲਾ) {
		ਜੇ (parents->ਵਸਤੂ == ਮਾਪਾ) {
			error(_("duplicate parent %s ignored"), oid_ਮੰਜ਼ਿਲ_hex(oid));
			ਵਾਪਸ;
		}
		parents_p = &parents->ਅਗਲਾ;
	}
	commit_list_ਪਾਓ(ਮਾਪਾ, parents_p);
}

ਸਥਿਰ ਪੂਰਨ parse_ਮਾਪਾ_arg_callback(ਅਚਲ ਬਣਤਰ option *opt,
		ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ object_id oid;
	ਬਣਤਰ commit_list **parents = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	ਜੇ (repo_get_oid_commit(the_repository, arg, &oid))
		die(_("not a valid object name %s"), arg);

	odb_assert_oid_type(the_repository->objects, &oid, OBJ_COMMIT);
	new_ਮਾਪਾ(lookup_commit(the_repository, &oid), parents);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ parse_ਸੁਨੇਹਾ_arg_callback(ਅਚਲ ਬਣਤਰ option *opt,
		ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ strbuf *buf = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	ਜੇ (buf->len)
		strbuf_addch(buf, '\n');
	strbuf_addstr(buf, arg);
	strbuf_complete_ਸਤਰ(buf);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ parse_file_arg_callback(ਅਚਲ ਬਣਤਰ option *opt,
		ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਪੂਰਨ fd;
	ਬਣਤਰ strbuf *buf = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	ਜੇ (buf->len)
		strbuf_addch(buf, '\n');
	ਜੇ (!strcmp(arg, "-"))
		fd = 0;
	ਨਹੀਂਤਾਂ {
		fd = xopen(arg, O_RDONLY);
	}
	ਜੇ (strbuf_read(buf, fd, 0) < 0)
		die_errno(_("git commit-tree: failed to read '%s'"), arg);
	ਜੇ (fd && close(fd))
		die_errno(_("git commit-tree: failed to close '%s'"), arg);

	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_commit_ਰੁੱਖ(ਪੂਰਨ argc,
		    ਅਚਲ ਅੱਖਰ **argv,
		    ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		    ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਸਥਿਰ ਬਣਤਰ strbuf ਬਫਰ = STRBUF_INIT;
	ਬਣਤਰ commit_list *parents = ਨਿਰਾ;
	ਬਣਤਰ object_id ਰੁੱਖ_oid;
	ਬਣਤਰ object_id commit_oid;

	ਬਣਤਰ option options[] = {
		OPT_CALLBACK_F('p', ਨਿਰਾ, &parents, N_("parent"),
			N_("id of a parent commit object"), PARSE_OPT_NONEG,
			parse_ਮਾਪਾ_arg_callback),
		OPT_CALLBACK_F('m', ਨਿਰਾ, &ਬਫਰ, N_("message"),
			N_("commit message"), PARSE_OPT_NONEG,
			parse_ਸੁਨੇਹਾ_arg_callback),
		OPT_CALLBACK_F('F', ਨਿਰਾ, &ਬਫਰ, N_("file"),
			N_("read commit log message from file"), PARSE_OPT_NONEG,
			parse_file_arg_callback),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.ਮੁੱਲ = &sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_ਸਿਰਾ()
	};
	ਪੂਰਨ ret;

	repo_config(the_repository, git_default_config, ਨਿਰਾ);

	show_usage_with_options_if_asked(argc, argv,
					 commit_ਰੁੱਖ_usage, options);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, commit_ਰੁੱਖ_usage, 0);

	ਜੇ (argc != 1)
		die(_("must give exactly one tree"));

	ਜੇ (repo_get_oid_ਰੁੱਖ(the_repository, argv[0], &ਰੁੱਖ_oid))
		die(_("not a valid object name %s"), argv[0]);

	ਜੇ (!ਬਫਰ.len) {
		ਜੇ (strbuf_read(&ਬਫਰ, 0, 0) < 0)
			die_errno(_("git commit-tree: failed to read"));
	}

	ਜੇ (commit_ਰੁੱਖ(ਬਫਰ.buf, ਬਫਰ.len, &ਰੁੱਖ_oid, parents, &commit_oid,
			ਨਿਰਾ, sign_commit)) {
		ret = 1;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਲਿਖੋ("%s\n", oid_ਮੰਜ਼ਿਲ_hex(&commit_oid));
	ret = 0;

ਬਾਹਰਲਾ:
	commit_list_free(parents);
	strbuf_release(&ਬਫਰ);
	ਵਾਪਸ ret;
}
