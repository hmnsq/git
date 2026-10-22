/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "odb.h"

#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ commit_ವೃಕ್ಷ_usage[] = {
	N_("git commit-tree <tree> [(-p <parent>)...]"),
	N_("git commit-tree [(-p <parent>)...] [-S[<keyid>]] [(-m <message>)...]\n"
	   "                [(-F <file>)...] <tree>"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *sign_commit;

ಸ್ಥಿರ ಶೂನ್ಯ new_parent(ಸಂರಚನಾ commit *parent, ಸಂರಚನಾ commit_list **parents_p)
{
	ಸಂರಚನಾ object_id *oid = &parent->object.oid;
	ಸಂರಚನಾ commit_list *parents;
	ಪ್ರತಿ (parents = *parents_p; parents; parents = parents->next) {
		ಯದಿ (parents->item == parent) {
			error(_("duplicate parent %s ignored"), oid_to_hex(oid));
			ಪ್ರತ್ಯಾಗಮ;
		}
		parents_p = &parents->next;
	}
	commit_list_ಸೇರಿಸು(parent, parents_p);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_parent_arg_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt,
		ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ object_id oid;
	ಸಂರಚನಾ commit_list **parents = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	ಯದಿ (repo_get_oid_commit(the_repository, arg, &oid))
		die(_("not a valid object name %s"), arg);

	odb_assert_oid_type(the_repository->objects, &oid, OBJ_COMMIT);
	new_parent(lookup_commit(the_repository, &oid), parents);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_message_arg_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt,
		ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ strbuf *buf = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	ಯದಿ (buf->len)
		strbuf_addch(buf, '\n');
	strbuf_addstr(buf, arg);
	strbuf_complete_ಸಾಲು(buf);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_file_arg_callback(ಧ್ರುವ ಸಂರಚನಾ option *opt,
		ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಪೂರ್ಣಾಙ್ಕ fd;
	ಸಂರಚನಾ strbuf *buf = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	ಯದಿ (buf->len)
		strbuf_addch(buf, '\n');
	ಯದಿ (!strcmp(arg, "-"))
		fd = 0;
	ಅನ್ಯಥಾ {
		fd = xopen(arg, O_RDONLY);
	}
	ಯದಿ (strbuf_read(buf, fd, 0) < 0)
		die_errno(_("git commit-tree: failed to read '%s'"), arg);
	ಯದಿ (fd && close(fd))
		die_errno(_("git commit-tree: failed to close '%s'"), arg);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ cmd_commit_ವೃಕ್ಷ(ಪೂರ್ಣಾಙ್ಕ argc,
		    ಧ್ರುವ ವರ್ಣ **argv,
		    ಧ್ರುವ ವರ್ಣ *prefix,
		    ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸ್ಥಿರ ಸಂರಚನಾ strbuf buffer = STRBUF_INIT;
	ಸಂರಚನಾ commit_list *parents = ರಿಕ್ತ;
	ಸಂರಚನಾ object_id ವೃಕ್ಷ_oid;
	ಸಂರಚನಾ object_id commit_oid;

	ಸಂರಚನಾ option options[] = {
		OPT_CALLBACK_F('p', ರಿಕ್ತ, &parents, N_("parent"),
			N_("id of a parent commit object"), PARSE_OPT_NONEG,
			parse_parent_arg_callback),
		OPT_CALLBACK_F('m', ರಿಕ್ತ, &buffer, N_("message"),
			N_("commit message"), PARSE_OPT_NONEG,
			parse_message_arg_callback),
		OPT_CALLBACK_F('F', ರಿಕ್ತ, &buffer, N_("file"),
			N_("read commit log message from file"), PARSE_OPT_NONEG,
			parse_file_arg_callback),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.ಮೌಲ್ಯ = &sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_END()
	};
	ಪೂರ್ಣಾಙ್ಕ ret;

	repo_config(the_repository, git_default_config, ರಿಕ್ತ);

	show_usage_with_options_if_asked(argc, argv,
					 commit_ವೃಕ್ಷ_usage, options);

	argc = parse_options(argc, argv, prefix, options, commit_ವೃಕ್ಷ_usage, 0);

	ಯದಿ (argc != 1)
		die(_("must give exactly one tree"));

	ಯದಿ (repo_get_oid_ವೃಕ್ಷ(the_repository, argv[0], &ವೃಕ್ಷ_oid))
		die(_("not a valid object name %s"), argv[0]);

	ಯದಿ (!buffer.len) {
		ಯದಿ (strbuf_read(&buffer, 0, 0) < 0)
			die_errno(_("git commit-tree: failed to read"));
	}

	ಯದಿ (commit_ವೃಕ್ಷ(buffer.buf, buffer.len, &ವೃಕ್ಷ_oid, parents, &commit_oid,
			ರಿಕ್ತ, sign_commit)) {
		ret = 1;
		ಗಚ್ಛ out;
	}

	printf("%s\n", oid_to_hex(&commit_oid));
	ret = 0;

out:
	commit_list_free(parents);
	strbuf_release(&buffer);
	ಪ್ರತ್ಯಾಗಮ ret;
}
