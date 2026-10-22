/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "object-name.h"
#समावेश "odb.h"

#समावेश "commit.h"
#समावेश "parse-options.h"

अटल स्थिर अक्षर * स्थिर commit_वृक्ष_usage[] = {
	N_("git commit-tree <tree> [(-p <parent>)...]"),
	N_("git commit-tree [(-p <parent>)...] [-S[<keyid>]] [(-m <message>)...]\n"
	   "                [(-F <file>)...] <tree>"),
	शून्य
};

अटल स्थिर अक्षर *sign_commit;

अटल व्योम new_parent(काष्ठा commit *parent, काष्ठा commit_list **parents_p)
{
	काष्ठा object_id *oid = &parent->object.oid;
	काष्ठा commit_list *parents;
	क्रम (parents = *parents_p; parents; parents = parents->अग्र) {
		अगर (parents->item == parent) {
			error(_("duplicate parent %s ignored"), oid_to_hex(oid));
			वापस;
		}
		parents_p = &parents->अग्र;
	}
	commit_list_जोड़ो(parent, parents_p);
}

अटल पूर्णांक parse_parent_arg_callback(स्थिर काष्ठा option *opt,
		स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा object_id oid;
	काष्ठा commit_list **parents = opt->मूल्य;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	अगर (repo_get_oid_commit(the_repository, arg, &oid))
		die(_("not a valid object name %s"), arg);

	odb_assert_oid_type(the_repository->objects, &oid, OBJ_COMMIT);
	new_parent(lookup_commit(the_repository, &oid), parents);
	वापस 0;
}

अटल पूर्णांक parse_message_arg_callback(स्थिर काष्ठा option *opt,
		स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा strbuf *buf = opt->मूल्य;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	अगर (buf->len)
		strbuf_addch(buf, '\n');
	strbuf_addstr(buf, arg);
	strbuf_complete_पंक्ति(buf);

	वापस 0;
}

अटल पूर्णांक parse_file_arg_callback(स्थिर काष्ठा option *opt,
		स्थिर अक्षर *arg, पूर्णांक unset)
{
	पूर्णांक fd;
	काष्ठा strbuf *buf = opt->मूल्य;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	अगर (buf->len)
		strbuf_addch(buf, '\n');
	अगर (!म_भेद(arg, "-"))
		fd = 0;
	अन्यथा {
		fd = xopen(arg, O_RDONLY);
	}
	अगर (strbuf_read(buf, fd, 0) < 0)
		die_errno(_("git commit-tree: failed to read '%s'"), arg);
	अगर (fd && बंद(fd))
		die_errno(_("git commit-tree: failed to close '%s'"), arg);

	वापस 0;
}

पूर्णांक cmd_commit_वृक्ष(पूर्णांक argc,
		    स्थिर अक्षर **argv,
		    स्थिर अक्षर *prefix,
		    काष्ठा repository *repo UNUSED)
{
	अटल काष्ठा strbuf बफर = STRBUF_INIT;
	काष्ठा commit_list *parents = शून्य;
	काष्ठा object_id वृक्ष_oid;
	काष्ठा object_id commit_oid;

	काष्ठा option options[] = {
		OPT_CALLBACK_F('p', शून्य, &parents, N_("parent"),
			N_("id of a parent commit object"), PARSE_OPT_NONEG,
			parse_parent_arg_callback),
		OPT_CALLBACK_F('m', शून्य, &बफर, N_("message"),
			N_("commit message"), PARSE_OPT_NONEG,
			parse_message_arg_callback),
		OPT_CALLBACK_F('F', शून्य, &बफर, N_("file"),
			N_("read commit log message from file"), PARSE_OPT_NONEG,
			parse_file_arg_callback),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.मूल्य = &sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_END()
	};
	पूर्णांक ret;

	repo_config(the_repository, git_default_config, शून्य);

	show_usage_with_options_if_asked(argc, argv,
					 commit_वृक्ष_usage, options);

	argc = parse_options(argc, argv, prefix, options, commit_वृक्ष_usage, 0);

	अगर (argc != 1)
		die(_("must give exactly one tree"));

	अगर (repo_get_oid_वृक्ष(the_repository, argv[0], &वृक्ष_oid))
		die(_("not a valid object name %s"), argv[0]);

	अगर (!बफर.len) {
		अगर (strbuf_read(&बफर, 0, 0) < 0)
			die_errno(_("git commit-tree: failed to read"));
	}

	अगर (commit_वृक्ष(बफर.buf, बफर.len, &वृक्ष_oid, parents, &commit_oid,
			शून्य, sign_commit)) {
		ret = 1;
		जाओ out;
	}

	म_लिखो("%s\n", oid_to_hex(&commit_oid));
	ret = 0;

out:
	commit_list_free(parents);
	strbuf_release(&बफर);
	वापस ret;
}
