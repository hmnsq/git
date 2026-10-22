/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "object-name.h"
#ضمن "odb.h"

#ضمن "commit.h"
#ضمن "parse-options.h"

ساكن ثابت حرف * ثابت commit_ونه_usage[] = {
	N_("git commit-tree <tree> [(-p <parent>)...]"),
	N_("git commit-tree [(-p <parent>)...] [-S[<keyid>]] [(-m <message>)...]\n"
	   "                [(-F <file>)...] <tree>"),
	عدم
};

ساكن ثابت حرف *sign_commit;

ساكن فراغ new_پلار(بنية commit *پلار, بنية commit_list **parents_p)
{
	بنية object_id *oid = &پلار->object.oid;
	بنية commit_list *parents;
	لكل (parents = *parents_p; parents; parents = parents->بل) {
		إذا (parents->توکی == پلار) {
			error(_("duplicate parent %s ignored"), oid_منزل_hex(oid));
			أرجع;
		}
		parents_p = &parents->بل;
	}
	commit_list_ورننباسه(پلار, parents_p);
}

ساكن صحيح parse_پلار_arg_callback(ثابت بنية option *opt,
		ثابت حرف *arg, صحيح unset)
{
	بنية object_id oid;
	بنية commit_list **parents = opt->ارزښت;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	إذا (repo_get_oid_commit(the_repository, arg, &oid))
		die(_("not a valid object name %s"), arg);

	odb_assert_oid_type(the_repository->objects, &oid, OBJ_COMMIT);
	new_پلار(lookup_commit(the_repository, &oid), parents);
	أرجع 0;
}

ساكن صحيح parse_پیغام_arg_callback(ثابت بنية option *opt,
		ثابت حرف *arg, صحيح unset)
{
	بنية strbuf *buf = opt->ارزښت;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	إذا (buf->len)
		strbuf_addch(buf, '\n');
	strbuf_addstr(buf, arg);
	strbuf_complete_کرښه(buf);

	أرجع 0;
}

ساكن صحيح parse_file_arg_callback(ثابت بنية option *opt,
		ثابت حرف *arg, صحيح unset)
{
	صحيح fd;
	بنية strbuf *buf = opt->ارزښت;

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

صحيح cmd_commit_ونه(صحيح argc,
		    ثابت حرف **argv,
		    ثابت حرف *مختاړی,
		    بنية repository *repo ناکارول_شوی)
{
	ساكن بنية strbuf بفر = STRBUF_INIT;
	بنية commit_list *parents = عدم;
	بنية object_id ونه_oid;
	بنية object_id commit_oid;

	بنية option options[] = {
		OPT_CALLBACK_F('p', عدم, &parents, N_("parent"),
			N_("id of a parent commit object"), PARSE_OPT_NONEG,
			parse_پلار_arg_callback),
		OPT_CALLBACK_F('m', عدم, &بفر, N_("message"),
			N_("commit message"), PARSE_OPT_NONEG,
			parse_پیغام_arg_callback),
		OPT_CALLBACK_F('F', عدم, &بفر, N_("file"),
			N_("read commit log message from file"), PARSE_OPT_NONEG,
			parse_file_arg_callback),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.ارزښت = &sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_پای_ټکی()
	};
	صحيح ret;

	repo_config(the_repository, git_default_config, عدم);

	show_usage_with_options_if_asked(argc, argv,
					 commit_ونه_usage, options);

	argc = parse_options(argc, argv, مختاړی, options, commit_ونه_usage, 0);

	إذا (argc != 1)
		die(_("must give exactly one tree"));

	إذا (repo_get_oid_ونه(the_repository, argv[0], &ونه_oid))
		die(_("not a valid object name %s"), argv[0]);

	إذا (!بفر.len) {
		إذا (strbuf_read(&بفر, 0, 0) < 0)
			die_errno(_("git commit-tree: failed to read"));
	}

	إذا (commit_ونه(بفر.buf, بفر.len, &ونه_oid, parents, &commit_oid,
			عدم, sign_commit)) {
		ret = 1;
		اذهب بهر;
	}

	چاپ_کړه("%s\n", oid_منزل_hex(&commit_oid));
	ret = 0;

بهر:
	commit_list_free(parents);
	strbuf_release(&بفر);
	أرجع ret;
}
