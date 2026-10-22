/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "object-name.h"
#include "odb.h"

#include "commit.h"
#include "parse-options.h"

static const var_na * const commit_tree_usage[] = {
	N_("git commit-tree <tree> [(-p <parent>)...]"),
	N_("git commit-tree [(-p <parent>)...] [-S[<keyid>]] [(-m <message>)...]\n"
	   "                [(-F <file>)...] <tree>"),
	NULL
};

static const var_na *sign_commit;

static void new_parent(sa_nracanaa commit *parent, sa_nracanaa commit_list **parents_p)
{
	sa_nracanaa object_id *oid = &parent->object.oid;
	sa_nracanaa commit_list *parents;
	အတွက် (parents = *parents_p; parents; parents = parents->next) {
		အကယ်၍ (parents->item == parent) {
			error(_("duplicate parent %s ignored"), oid_to_hex(oid));
			ပြန်ပေး;
		}
		parents_p = &parents->next;
	}
	commit_list_insert(parent, parents_p);
}

static ကိန်းပြည့် parse_parent_arg_callback(const sa_nracanaa option *opt,
		const var_na *arg, ကိန်းပြည့် unset)
{
	sa_nracanaa object_id oid;
	sa_nracanaa commit_list **parents = opt->value;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	အကယ်၍ (repo_get_oid_commit(the_repository, arg, &oid))
		die(_("not a valid object name %s"), arg);

	odb_assert_oid_type(the_repository->objects, &oid, OBJ_COMMIT);
	new_parent(lookup_commit(the_repository, &oid), parents);
	ပြန်ပေး 0;
}

static ကိန်းပြည့် parse_message_arg_callback(const sa_nracanaa option *opt,
		const var_na *arg, ကိန်းပြည့် unset)
{
	sa_nracanaa strbuf *buf = opt->value;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	အကယ်၍ (buf->len)
		strbuf_addch(buf, '\n');
	strbuf_addstr(buf, arg);
	strbuf_complete_line(buf);

	ပြန်ပေး 0;
}

static ကိန်းပြည့် parse_file_arg_callback(const sa_nracanaa option *opt,
		const var_na *arg, ကိန်းပြည့် unset)
{
	ကိန်းပြည့် fd;
	sa_nracanaa strbuf *buf = opt->value;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	အကယ်၍ (buf->len)
		strbuf_addch(buf, '\n');
	အကယ်၍ (!strcmp(arg, "-"))
		fd = 0;
	မဟုတ်ပါက {
		fd = xopen(arg, O_RDONLY);
	}
	အကယ်၍ (strbuf_read(buf, fd, 0) < 0)
		die_errno(_("git commit-tree: failed to read '%s'"), arg);
	အကယ်၍ (fd && close(fd))
		die_errno(_("git commit-tree: failed to close '%s'"), arg);

	ပြန်ပေး 0;
}

ကိန်းပြည့် cmd_commit_tree(ကိန်းပြည့် argc,
		    const var_na **argv,
		    const var_na *prefix,
		    sa_nracanaa repository *repo UNUSED)
{
	static sa_nracanaa strbuf buffer = STRBUF_INIT;
	sa_nracanaa commit_list *parents = NULL;
	sa_nracanaa object_id tree_oid;
	sa_nracanaa object_id commit_oid;

	sa_nracanaa option options[] = {
		OPT_CALLBACK_F('p', NULL, &parents, N_("parent"),
			N_("id of a parent commit object"), PARSE_OPT_NONEG,
			parse_parent_arg_callback),
		OPT_CALLBACK_F('m', NULL, &buffer, N_("message"),
			N_("commit message"), PARSE_OPT_NONEG,
			parse_message_arg_callback),
		OPT_CALLBACK_F('F', NULL, &buffer, N_("file"),
			N_("read commit log message from file"), PARSE_OPT_NONEG,
			parse_file_arg_callback),
		{
			.type = OPTION_STRING,
			.short_name = 'S',
			.long_name = "gpg-sign",
			.value = &sign_commit,
			.argh = N_("key-id"),
			.help = N_("GPG sign commit"),
			.flags = PARSE_OPT_OPTARG,
			.defval = (intptr_t) "",
		},
		OPT_END()
	};
	ကိန်းပြည့် ret;

	repo_config(the_repository, git_default_config, NULL);

	show_usage_with_options_if_asked(argc, argv,
					 commit_tree_usage, options);

	argc = parse_options(argc, argv, prefix, options, commit_tree_usage, 0);

	အကယ်၍ (argc != 1)
		die(_("must give exactly one tree"));

	အကယ်၍ (repo_get_oid_tree(the_repository, argv[0], &tree_oid))
		die(_("not a valid object name %s"), argv[0]);

	အကယ်၍ (!buffer.len) {
		အကယ်၍ (strbuf_read(&buffer, 0, 0) < 0)
			die_errno(_("git commit-tree: failed to read"));
	}

	အကယ်၍ (commit_tree(buffer.buf, buffer.len, &tree_oid, parents, &commit_oid,
			NULL, sign_commit)) {
		ret = 1;
		goto out;
	}

	printf("%s\n", oid_to_hex(&commit_oid));
	ret = 0;

out:
	commit_list_free(parents);
	strbuf_release(&buffer);
	ပြန်ပေး ret;
}
