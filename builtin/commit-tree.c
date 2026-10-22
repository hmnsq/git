/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"

#ଅନ୍ତର୍ଭାଵ "commit.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ commit_ବୃକ୍ଷ_usage[] = {
	N_("git commit-tree <tree> [(-p <parent>)...]"),
	N_("git commit-tree [(-p <parent>)...] [-S[<keyid>]] [(-m <message>)...]\n"
	   "                [(-F <file>)...] <tree>"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *sign_commit;

ସ୍ଥିର ଶୂନ୍ୟ new_parent(ସଂରଚନା commit *parent, ସଂରଚନା commit_list **parents_p)
{
	ସଂରଚନା object_id *oid = &parent->object.oid;
	ସଂରଚନା commit_list *parents;
	ପାଇଁ (parents = *parents_p; parents; parents = parents->next) {
		ଯଦି (parents->item == parent) {
			error(_("duplicate parent %s ignored"), oid_to_hex(oid));
			ଫେରନ୍ତୁ;
		}
		parents_p = &parents->next;
	}
	commit_list_ଭର୍ତ୍ତି(parent, parents_p);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_parent_arg_callback(ଅଚଳ ସଂରଚନା option *opt,
		ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା object_id oid;
	ସଂରଚନା commit_list **parents = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	ଯଦି (repo_get_oid_commit(the_repository, arg, &oid))
		die(_("not a valid object name %s"), arg);

	odb_assert_oid_type(the_repository->objects, &oid, OBJ_COMMIT);
	new_parent(lookup_commit(the_repository, &oid), parents);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_message_arg_callback(ଅଚଳ ସଂରଚନା option *opt,
		ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା strbuf *buf = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	ଯଦି (buf->len)
		strbuf_addch(buf, '\n');
	strbuf_addstr(buf, arg);
	strbuf_complete_ଧାଡ଼ି(buf);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_file_arg_callback(ଅଚଳ ସଂରଚନା option *opt,
		ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
	ସଂରଚନା strbuf *buf = opt->ମୂଲ୍ୟ;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	ଯଦି (buf->len)
		strbuf_addch(buf, '\n');
	ଯଦି (!strcmp(arg, "-"))
		fd = 0;
	ଅନ୍ୟଥା {
		fd = xopen(arg, O_RDONLY);
	}
	ଯଦି (strbuf_read(buf, fd, 0) < 0)
		die_errno(_("git commit-tree: failed to read '%s'"), arg);
	ଯଦି (fd && close(fd))
		die_errno(_("git commit-tree: failed to close '%s'"), arg);

	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_commit_ବୃକ୍ଷ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		    ଅଚଳ ଅକ୍ଷର **argv,
		    ଅଚଳ ଅକ୍ଷର *prefix,
		    ସଂରଚନା repository *repo UNUSED)
{
	ସ୍ଥିର ସଂରଚନା strbuf buffer = STRBUF_INIT;
	ସଂରଚନା commit_list *parents = ଖାଲି;
	ସଂରଚନା object_id ବୃକ୍ଷ_oid;
	ସଂରଚନା object_id commit_oid;

	ସଂରଚନା option options[] = {
		OPT_CALLBACK_F('p', ଖାଲି, &parents, N_("parent"),
			N_("id of a parent commit object"), PARSE_OPT_NONEG,
			parse_parent_arg_callback),
		OPT_CALLBACK_F('m', ଖାଲି, &buffer, N_("message"),
			N_("commit message"), PARSE_OPT_NONEG,
			parse_message_arg_callback),
		OPT_CALLBACK_F('F', ଖାଲି, &buffer, N_("file"),
			N_("read commit log message from file"), PARSE_OPT_NONEG,
			parse_file_arg_callback),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.ମୂଲ୍ୟ = &sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_END()
	};
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	repo_config(the_repository, git_default_config, ଖାଲି);

	show_usage_with_options_if_asked(argc, argv,
					 commit_ବୃକ୍ଷ_usage, options);

	argc = parse_options(argc, argv, prefix, options, commit_ବୃକ୍ଷ_usage, 0);

	ଯଦି (argc != 1)
		die(_("must give exactly one tree"));

	ଯଦି (repo_get_oid_ବୃକ୍ଷ(the_repository, argv[0], &ବୃକ୍ଷ_oid))
		die(_("not a valid object name %s"), argv[0]);

	ଯଦି (!buffer.len) {
		ଯଦି (strbuf_read(&buffer, 0, 0) < 0)
			die_errno(_("git commit-tree: failed to read"));
	}

	ଯଦି (commit_ବୃକ୍ଷ(buffer.buf, buffer.len, &ବୃକ୍ଷ_oid, parents, &commit_oid,
			ଖାଲି, sign_commit)) {
		ret = 1;
		ଯାଅ out;
	}

	printf("%s\n", oid_to_hex(&commit_oid));
	ret = 0;

out:
	commit_list_free(parents);
	strbuf_release(&buffer);
	ଫେରନ୍ତୁ ret;
}
