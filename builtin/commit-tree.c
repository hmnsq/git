/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "odb.h"

#அந்தர்பாவ "commit.h"
#அந்தர்பாவ "parse-options.h"

நிலையான மாறிலி எழுத்து * மாறிலி commit_மரம்_usage[] = {
	N_("git commit-tree <tree> [(-p <parent>)...]"),
	N_("git commit-tree [(-p <parent>)...] [-S[<keyid>]] [(-m <message>)...]\n"
	   "                [(-F <file>)...] <tree>"),
	வெற்று
};

நிலையான மாறிலி எழுத்து *sign_commit;

நிலையான வெறுமை new_parent(கட்டமைப்பு commit *parent, கட்டமைப்பு commit_list **parents_p)
{
	கட்டமைப்பு object_id *oid = &parent->object.oid;
	கட்டமைப்பு commit_list *parents;
	க்கு (parents = *parents_p; parents; parents = parents->next) {
		எனில் (parents->item == parent) {
			error(_("duplicate parent %s ignored"), oid_to_hex(oid));
			திரும்பு;
		}
		parents_p = &parents->next;
	}
	commit_list_செருகு(parent, parents_p);
}

நிலையான முழு parse_parent_arg_callback(மாறிலி கட்டமைப்பு option *opt,
		மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு object_id oid;
	கட்டமைப்பு commit_list **parents = opt->மதிப்பு;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	எனில் (repo_get_oid_commit(the_repository, arg, &oid))
		die(_("not a valid object name %s"), arg);

	odb_assert_oid_type(the_repository->objects, &oid, OBJ_COMMIT);
	new_parent(lookup_commit(the_repository, &oid), parents);
	திரும்பு 0;
}

நிலையான முழு parse_message_arg_callback(மாறிலி கட்டமைப்பு option *opt,
		மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு strbuf *buf = opt->மதிப்பு;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	எனில் (buf->len)
		strbuf_addch(buf, '\n');
	strbuf_addstr(buf, arg);
	strbuf_complete_வரி(buf);

	திரும்பு 0;
}

நிலையான முழு parse_file_arg_callback(மாறிலி கட்டமைப்பு option *opt,
		மாறிலி எழுத்து *arg, முழு unset)
{
	முழு fd;
	கட்டமைப்பு strbuf *buf = opt->மதிப்பு;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	எனில் (buf->len)
		strbuf_addch(buf, '\n');
	எனில் (!strcmp(arg, "-"))
		fd = 0;
	இல்லையெனில் {
		fd = xopen(arg, O_RDONLY);
	}
	எனில் (strbuf_read(buf, fd, 0) < 0)
		die_errno(_("git commit-tree: failed to read '%s'"), arg);
	எனில் (fd && close(fd))
		die_errno(_("git commit-tree: failed to close '%s'"), arg);

	திரும்பு 0;
}

முழு cmd_commit_மரம்(முழு argc,
		    மாறிலி எழுத்து **argv,
		    மாறிலி எழுத்து *prefix,
		    கட்டமைப்பு repository *repo UNUSED)
{
	நிலையான கட்டமைப்பு strbuf buffer = STRBUF_INIT;
	கட்டமைப்பு commit_list *parents = வெற்று;
	கட்டமைப்பு object_id மரம்_oid;
	கட்டமைப்பு object_id commit_oid;

	கட்டமைப்பு option options[] = {
		OPT_CALLBACK_F('p', வெற்று, &parents, N_("parent"),
			N_("id of a parent commit object"), PARSE_OPT_NONEG,
			parse_parent_arg_callback),
		OPT_CALLBACK_F('m', வெற்று, &buffer, N_("message"),
			N_("commit message"), PARSE_OPT_NONEG,
			parse_message_arg_callback),
		OPT_CALLBACK_F('F', வெற்று, &buffer, N_("file"),
			N_("read commit log message from file"), PARSE_OPT_NONEG,
			parse_file_arg_callback),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.மதிப்பு = &sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_END()
	};
	முழு ret;

	repo_config(the_repository, git_default_config, வெற்று);

	show_usage_with_options_if_asked(argc, argv,
					 commit_மரம்_usage, options);

	argc = parse_options(argc, argv, prefix, options, commit_மரம்_usage, 0);

	எனில் (argc != 1)
		die(_("must give exactly one tree"));

	எனில் (repo_get_oid_மரம்(the_repository, argv[0], &மரம்_oid))
		die(_("not a valid object name %s"), argv[0]);

	எனில் (!buffer.len) {
		எனில் (strbuf_read(&buffer, 0, 0) < 0)
			die_errno(_("git commit-tree: failed to read"));
	}

	எனில் (commit_மரம்(buffer.buf, buffer.len, &மரம்_oid, parents, &commit_oid,
			வெற்று, sign_commit)) {
		ret = 1;
		செல் out;
	}

	printf("%s\n", oid_to_hex(&commit_oid));
	ret = 0;

out:
	commit_list_free(parents);
	strbuf_release(&buffer);
	திரும்பு ret;
}
