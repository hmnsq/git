/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "odb.h"

#અન્તર્ભાવ "commit.h"
#અન્તર્ભાવ "parse-options.h"

સ્થિર અચલ અક્ષર * અચલ commit_વૃક્ષ_usage[] = {
	N_("git commit-tree <tree> [(-p <parent>)...]"),
	N_("git commit-tree [(-p <parent>)...] [-S[<keyid>]] [(-m <message>)...]\n"
	   "                [(-F <file>)...] <tree>"),
	ખાલી
};

સ્થિર અચલ અક્ષર *sign_commit;

સ્થિર શૂન્ય new_parent(સંરચના commit *parent, સંરચના commit_list **parents_p)
{
	સંરચના object_id *oid = &parent->object.oid;
	સંરચના commit_list *parents;
	માટે (parents = *parents_p; parents; parents = parents->next) {
		જો (parents->item == parent) {
			error(_("duplicate parent %s ignored"), oid_to_hex(oid));
			પાછા;
		}
		parents_p = &parents->next;
	}
	commit_list_ઉમેરો(parent, parents_p);
}

સ્થિર પૂર્ણાંક parse_parent_arg_callback(અચલ સંરચના option *opt,
		અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના object_id oid;
	સંરચના commit_list **parents = opt->કિંમત;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	જો (repo_get_oid_commit(the_repository, arg, &oid))
		die(_("not a valid object name %s"), arg);

	odb_assert_oid_type(the_repository->objects, &oid, OBJ_COMMIT);
	new_parent(lookup_commit(the_repository, &oid), parents);
	પાછા 0;
}

સ્થિર પૂર્ણાંક parse_message_arg_callback(અચલ સંરચના option *opt,
		અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના strbuf *buf = opt->કિંમત;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	જો (buf->len)
		strbuf_addch(buf, '\n');
	strbuf_addstr(buf, arg);
	strbuf_complete_લીટી(buf);

	પાછા 0;
}

સ્થિર પૂર્ણાંક parse_file_arg_callback(અચલ સંરચના option *opt,
		અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	પૂર્ણાંક fd;
	સંરચના strbuf *buf = opt->કિંમત;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	જો (buf->len)
		strbuf_addch(buf, '\n');
	જો (!strcmp(arg, "-"))
		fd = 0;
	નહીંતર {
		fd = xopen(arg, O_RDONLY);
	}
	જો (strbuf_read(buf, fd, 0) < 0)
		die_errno(_("git commit-tree: failed to read '%s'"), arg);
	જો (fd && close(fd))
		die_errno(_("git commit-tree: failed to close '%s'"), arg);

	પાછા 0;
}

પૂર્ણાંક cmd_commit_વૃક્ષ(પૂર્ણાંક argc,
		    અચલ અક્ષર **argv,
		    અચલ અક્ષર *prefix,
		    સંરચના repository *repo UNUSED)
{
	સ્થિર સંરચના strbuf buffer = STRBUF_INIT;
	સંરચના commit_list *parents = ખાલી;
	સંરચના object_id વૃક્ષ_oid;
	સંરચના object_id commit_oid;

	સંરચના option options[] = {
		OPT_CALLBACK_F('p', ખાલી, &parents, N_("parent"),
			N_("id of a parent commit object"), PARSE_OPT_NONEG,
			parse_parent_arg_callback),
		OPT_CALLBACK_F('m', ખાલી, &buffer, N_("message"),
			N_("commit message"), PARSE_OPT_NONEG,
			parse_message_arg_callback),
		OPT_CALLBACK_F('F', ખાલી, &buffer, N_("file"),
			N_("read commit log message from file"), PARSE_OPT_NONEG,
			parse_file_arg_callback),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.કિંમત = &sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_END()
	};
	પૂર્ણાંક ret;

	repo_config(the_repository, git_default_config, ખાલી);

	show_usage_with_options_if_asked(argc, argv,
					 commit_વૃક્ષ_usage, options);

	argc = parse_options(argc, argv, prefix, options, commit_વૃક્ષ_usage, 0);

	જો (argc != 1)
		die(_("must give exactly one tree"));

	જો (repo_get_oid_વૃક્ષ(the_repository, argv[0], &વૃક્ષ_oid))
		die(_("not a valid object name %s"), argv[0]);

	જો (!buffer.len) {
		જો (strbuf_read(&buffer, 0, 0) < 0)
			die_errno(_("git commit-tree: failed to read"));
	}

	જો (commit_વૃક્ષ(buffer.buf, buffer.len, &વૃક્ષ_oid, parents, &commit_oid,
			ખાલી, sign_commit)) {
		ret = 1;
		જાઓ out;
	}

	printf("%s\n", oid_to_hex(&commit_oid));
	ret = 0;

out:
	commit_list_free(parents);
	strbuf_release(&buffer);
	પાછા ret;
}
