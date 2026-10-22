/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "odb.h"

#ಅನ್ತರ್ಭಾವ "commit.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ commit_ವೃಕ್ಷ_usage[] = {
	N_("git commit-tree <tree> [(-p <parent>)...]"),
	N_("git commit-tree [(-p <parent>)...] [-S[<keyid>]] [(-m <message>)...]\n"
	   "                [(-F <file>)...] <tree>"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *sign_commit;

ಸ್ಥಿರ ಶೂನ್ಯ new_ಪೋಷಕ(ರಚನೆ commit *ಪೋಷಕ, ರಚನೆ commit_list **parents_p)
{
	ರಚನೆ object_id *oid = &ಪೋಷಕ->object.oid;
	ರಚನೆ commit_list *parents;
	ಗಾಗಿ (parents = *parents_p; parents; parents = parents->ಮುಂದಿನ) {
		ಆದರೆ (parents->ಅಂಶ == ಪೋಷಕ) {
			error(_("duplicate parent %s ignored"), oid_ಗಮ್ಯ_hex(oid));
			ಹಿಂತಿರುಗಿ;
		}
		parents_p = &parents->ಮುಂದಿನ;
	}
	commit_list_ಸೇರಿಸು(ಪೋಷಕ, parents_p);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_ಪೋಷಕ_arg_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ object_id oid;
	ರಚನೆ commit_list **parents = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	ಆದರೆ (repo_get_oid_commit(the_repository, arg, &oid))
		die(_("not a valid object name %s"), arg);

	odb_assert_oid_type(the_repository->objects, &oid, OBJ_COMMIT);
	new_ಪೋಷಕ(lookup_commit(the_repository, &oid), parents);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_ಸಂದೇಶ_arg_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ strbuf *buf = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	ಆದರೆ (buf->len)
		strbuf_addch(buf, '\n');
	strbuf_addstr(buf, arg);
	strbuf_complete_ಸಾಲು(buf);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_file_arg_callback(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಪೂರ್ಣಾಂಕ fd;
	ರಚನೆ strbuf *buf = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	ಆದರೆ (buf->len)
		strbuf_addch(buf, '\n');
	ಆದರೆ (!strcmp(arg, "-"))
		fd = 0;
	ಇಲ್ಲದಿದ್ದರೆ {
		fd = xopen(arg, O_RDONLY);
	}
	ಆದರೆ (strbuf_read(buf, fd, 0) < 0)
		die_errno(_("git commit-tree: failed to read '%s'"), arg);
	ಆದರೆ (fd && close(fd))
		die_errno(_("git commit-tree: failed to close '%s'"), arg);

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_commit_ವೃಕ್ಷ(ಪೂರ್ಣಾಂಕ argc,
		    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		    ರಚನೆ repository *repo ಬಳಸದ)
{
	ಸ್ಥಿರ ರಚನೆ strbuf ಬಫರ್ = STRBUF_INIT;
	ರಚನೆ commit_list *parents = ಖಾಲಿ;
	ರಚನೆ object_id ವೃಕ್ಷ_oid;
	ರಚನೆ object_id commit_oid;

	ರಚನೆ option options[] = {
		OPT_CALLBACK_F('p', ಖಾಲಿ, &parents, N_("parent"),
			N_("id of a parent commit object"), PARSE_OPT_NONEG,
			parse_ಪೋಷಕ_arg_callback),
		OPT_CALLBACK_F('m', ಖಾಲಿ, &ಬಫರ್, N_("message"),
			N_("commit message"), PARSE_OPT_NONEG,
			parse_ಸಂದೇಶ_arg_callback),
		OPT_CALLBACK_F('F', ಖಾಲಿ, &ಬಫರ್, N_("file"),
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
		OPT_ಕೊನೆ()
	};
	ಪೂರ್ಣಾಂಕ ret;

	repo_config(the_repository, git_default_config, ಖಾಲಿ);

	show_usage_with_options_if_asked(argc, argv,
					 commit_ವೃಕ್ಷ_usage, options);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, commit_ವೃಕ್ಷ_usage, 0);

	ಆದರೆ (argc != 1)
		die(_("must give exactly one tree"));

	ಆದರೆ (repo_get_oid_ವೃಕ್ಷ(the_repository, argv[0], &ವೃಕ್ಷ_oid))
		die(_("not a valid object name %s"), argv[0]);

	ಆದರೆ (!ಬಫರ್.len) {
		ಆದರೆ (strbuf_read(&ಬಫರ್, 0, 0) < 0)
			die_errno(_("git commit-tree: failed to read"));
	}

	ಆದರೆ (commit_ವೃಕ್ಷ(ಬಫರ್.buf, ಬಫರ್.len, &ವೃಕ್ಷ_oid, parents, &commit_oid,
			ಖಾಲಿ, sign_commit)) {
		ret = 1;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಬರೆ("%s\n", oid_ಗಮ್ಯ_hex(&commit_oid));
	ret = 0;

ಹೊರಹೋಗು:
	commit_list_free(parents);
	strbuf_release(&ಬಫರ್);
	ಹಿಂತಿರುಗಿ ret;
}
