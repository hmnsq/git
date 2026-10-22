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

স্থির ধ্রুব বর্ণ * ধ্রুব commit_বৃক্ষ_usage[] = {
	N_("git commit-tree <tree> [(-p <parent>)...]"),
	N_("git commit-tree [(-p <parent>)...] [-S[<keyid>]] [(-m <message>)...]\n"
	   "                [(-F <file>)...] <tree>"),
	রিক্ত
};

স্থির ধ্রুব বর্ণ *sign_commit;

স্থির শূন্য new_parent(সংরচনা commit *parent, সংরচনা commit_list **parents_p)
{
	সংরচনা object_id *oid = &parent->object.oid;
	সংরচনা commit_list *parents;
	প্রতি (parents = *parents_p; parents; parents = parents->next) {
		যদি (parents->item == parent) {
			error(_("duplicate parent %s ignored"), oid_to_hex(oid));
			প্রত্যাগম;
		}
		parents_p = &parents->next;
	}
	commit_list_ঢোকাও(parent, parents_p);
}

স্থির পূর্ণাঙ্ক parse_parent_arg_callback(ধ্রুব সংরচনা option *opt,
		ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা object_id oid;
	সংরচনা commit_list **parents = opt->মান;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	যদি (repo_get_oid_commit(the_repository, arg, &oid))
		die(_("not a valid object name %s"), arg);

	odb_assert_oid_type(the_repository->objects, &oid, OBJ_COMMIT);
	new_parent(lookup_commit(the_repository, &oid), parents);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক parse_message_arg_callback(ধ্রুব সংরচনা option *opt,
		ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা strbuf *buf = opt->মান;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	যদি (buf->len)
		strbuf_addch(buf, '\n');
	strbuf_addstr(buf, arg);
	strbuf_complete_সারি(buf);

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক parse_file_arg_callback(ধ্রুব সংরচনা option *opt,
		ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	পূর্ণাঙ্ক fd;
	সংরচনা strbuf *buf = opt->মান;

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

	প্রত্যাগম 0;
}

পূর্ণাঙ্ক cmd_commit_বৃক্ষ(পূর্ণাঙ্ক argc,
		    ধ্রুব বর্ণ **argv,
		    ধ্রুব বর্ণ *prefix,
		    সংরচনা repository *repo UNUSED)
{
	স্থির সংরচনা strbuf buffer = STRBUF_INIT;
	সংরচনা commit_list *parents = রিক্ত;
	সংরচনা object_id বৃক্ষ_oid;
	সংরচনা object_id commit_oid;

	সংরচনা option options[] = {
		OPT_CALLBACK_F('p', রিক্ত, &parents, N_("parent"),
			N_("id of a parent commit object"), PARSE_OPT_NONEG,
			parse_parent_arg_callback),
		OPT_CALLBACK_F('m', রিক্ত, &buffer, N_("message"),
			N_("commit message"), PARSE_OPT_NONEG,
			parse_message_arg_callback),
		OPT_CALLBACK_F('F', রিক্ত, &buffer, N_("file"),
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
	পূর্ণাঙ্ক ret;

	repo_config(the_repository, git_default_config, রিক্ত);

	show_usage_with_options_if_asked(argc, argv,
					 commit_বৃক্ষ_usage, options);

	argc = parse_options(argc, argv, prefix, options, commit_বৃক্ষ_usage, 0);

	যদি (argc != 1)
		die(_("must give exactly one tree"));

	যদি (repo_get_oid_বৃক্ষ(the_repository, argv[0], &বৃক্ষ_oid))
		die(_("not a valid object name %s"), argv[0]);

	যদি (!buffer.len) {
		যদি (strbuf_read(&buffer, 0, 0) < 0)
			die_errno(_("git commit-tree: failed to read"));
	}

	যদি (commit_বৃক্ষ(buffer.buf, buffer.len, &বৃক্ষ_oid, parents, &commit_oid,
			রিক্ত, sign_commit)) {
		ret = 1;
		গচ্ছ out;
	}

	printf("%s\n", oid_to_hex(&commit_oid));
	ret = 0;

out:
	commit_list_free(parents);
	strbuf_release(&buffer);
	প্রত্যাগম ret;
}
