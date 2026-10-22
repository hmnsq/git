/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#परिभाषय USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "odb.h"

#अन्तर्भाव "commit.h"
#अन्तर्भाव "parse-options.h"

स्थिर ध्रुव वर्ण * ध्रुव commit_वृक्षः_usage[] = {
	N_("git commit-tree <tree> [(-p <parent>)...]"),
	N_("git commit-tree [(-p <parent>)...] [-S[<keyid>]] [(-m <message>)...]\n"
	   "                [(-F <file>)...] <tree>"),
	रिक्त
};

स्थिर ध्रुव वर्ण *sign_commit;

स्थिर शून्य new_parent(संरचना commit *parent, संरचना commit_list **parents_p)
{
	संरचना object_id *oid = &parent->object.oid;
	संरचना commit_list *parents;
	प्रति (parents = *parents_p; parents; parents = parents->अग्र) {
		यदि (parents->item == parent) {
			error(_("duplicate parent %s ignored"), oid_गन्तव्यम्_hex(oid));
			प्रत्यागम;
		}
		parents_p = &parents->अग्र;
	}
	commit_list_निवेशय(parent, parents_p);
}

स्थिर पूर्णाङ्क parse_parent_arg_callback(ध्रुव संरचना option *opt,
		ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना object_id oid;
	संरचना commit_list **parents = opt->मानम्;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	यदि (repo_get_oid_commit(the_repository, arg, &oid))
		die(_("not a valid object name %s"), arg);

	odb_assert_oid_type(the_repository->objects, &oid, OBJ_COMMIT);
	new_parent(lookup_commit(the_repository, &oid), parents);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क parse_सन्देशः_arg_callback(ध्रुव संरचना option *opt,
		ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना strbuf *buf = opt->मानम्;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	यदि (buf->len)
		strbuf_addch(buf, '\n');
	strbuf_addstr(buf, arg);
	strbuf_complete_पङ्क्तिः(buf);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क parse_file_arg_callback(ध्रुव संरचना option *opt,
		ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	पूर्णाङ्क fd;
	संरचना strbuf *buf = opt->मानम्;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	यदि (buf->len)
		strbuf_addch(buf, '\n');
	यदि (!वाक्यं_तुलय(arg, "-"))
		fd = 0;
	अन्यथा {
		fd = xopen(arg, O_RDONLY);
	}
	यदि (strbuf_read(buf, fd, 0) < 0)
		die_errno(_("git commit-tree: failed to read '%s'"), arg);
	यदि (fd && close(fd))
		die_errno(_("git commit-tree: failed to close '%s'"), arg);

	प्रत्यागम 0;
}

पूर्णाङ्क cmd_commit_वृक्षः(पूर्णाङ्क argc,
		    ध्रुव वर्ण **argv,
		    ध्रुव वर्ण *उपसर्गः,
		    संरचना repository *repo अप्रयुक्तम्)
{
	स्थिर संरचना strbuf बफर = STRBUF_INIT;
	संरचना commit_list *parents = रिक्त;
	संरचना object_id वृक्षः_oid;
	संरचना object_id commit_oid;

	संरचना option options[] = {
		OPT_CALLBACK_F('p', रिक्त, &parents, N_("parent"),
			N_("id of a parent commit object"), PARSE_OPT_NONEG,
			parse_parent_arg_callback),
		OPT_CALLBACK_F('m', रिक्त, &बफर, N_("message"),
			N_("commit message"), PARSE_OPT_NONEG,
			parse_सन्देशः_arg_callback),
		OPT_CALLBACK_F('F', रिक्त, &बफर, N_("file"),
			N_("read commit log message from file"), PARSE_OPT_NONEG,
			parse_file_arg_callback),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.मानम् = &sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_अन्तः()
	};
	पूर्णाङ्क ret;

	repo_config(the_repository, git_default_config, रिक्त);

	show_usage_with_options_if_asked(argc, argv,
					 commit_वृक्षः_usage, options);

	argc = parse_options(argc, argv, उपसर्गः, options, commit_वृक्षः_usage, 0);

	यदि (argc != 1)
		die(_("must give exactly one tree"));

	यदि (repo_get_oid_वृक्षः(the_repository, argv[0], &वृक्षः_oid))
		die(_("not a valid object name %s"), argv[0]);

	यदि (!बफर.len) {
		यदि (strbuf_read(&बफर, 0, 0) < 0)
			die_errno(_("git commit-tree: failed to read"));
	}

	यदि (commit_वृक्षः(बफर.buf, बफर.len, &वृक्षः_oid, parents, &commit_oid,
			रिक्त, sign_commit)) {
		ret = 1;
		गच्छ बहिर्गमनम्;
	}

	प्रारूप_लिख("%s\n", oid_गन्तव्यम्_hex(&commit_oid));
	ret = 0;

बहिर्गमनम्:
	commit_list_free(parents);
	strbuf_release(&बफर);
	प्रत्यागम ret;
}
