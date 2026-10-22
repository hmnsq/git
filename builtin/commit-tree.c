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

static const char * const commit_tree_usage[] = {
	N_("git commit-tree <tree> [(-p <parent>)...]"),
	N_("git commit-tree [(-p <parent>)...] [-S[<keyid>]] [(-m <message>)...]\n"
	   "                [(-F <file>)...] <tree>"),
	NULL
};

static const char *sign_commit;

static void new_parent(struct commit *parent, struct commit_list **parents_p)
{
	struct object_id *oid = &parent->object.oid;
	struct commit_list *parents;
	为 (parents = *parents_p; parents; parents = parents->next) {
		如果 (parents->item == parent) {
			error(_("duplicate parent %s ignored"), oid_to_hex(oid));
			返回;
		}
		parents_p = &parents->next;
	}
	commit_list_insert(parent, parents_p);
}

static 整数 parse_parent_arg_callback(const struct option *opt,
		const char *arg, 整数 unset)
{
	struct object_id oid;
	struct commit_list **parents = opt->value;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	如果 (repo_get_oid_commit(the_repository, arg, &oid))
		die(_("not a valid object name %s"), arg);

	odb_assert_oid_type(the_repository->objects, &oid, OBJ_COMMIT);
	new_parent(lookup_commit(the_repository, &oid), parents);
	返回 0;
}

static 整数 parse_message_arg_callback(const struct option *opt,
		const char *arg, 整数 unset)
{
	struct strbuf *buf = opt->value;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	如果 (buf->len)
		strbuf_addch(buf, '\n');
	strbuf_addstr(buf, arg);
	strbuf_complete_line(buf);

	返回 0;
}

static 整数 parse_file_arg_callback(const struct option *opt,
		const char *arg, 整数 unset)
{
	整数 fd;
	struct strbuf *buf = opt->value;

	BUG_ON_OPT_NEG_NOARG(unset, arg);

	如果 (buf->len)
		strbuf_addch(buf, '\n');
	如果 (!strcmp(arg, "-"))
		fd = 0;
	否则 {
		fd = xopen(arg, O_RDONLY);
	}
	如果 (strbuf_read(buf, fd, 0) < 0)
		die_errno(_("git commit-tree: failed to read '%s'"), arg);
	如果 (fd && close(fd))
		die_errno(_("git commit-tree: failed to close '%s'"), arg);

	返回 0;
}

整数 cmd_commit_tree(整数 argc,
		    const char **argv,
		    const char *prefix,
		    struct repository *repo UNUSED)
{
	static struct strbuf buffer = STRBUF_INIT;
	struct commit_list *parents = NULL;
	struct object_id tree_oid;
	struct object_id commit_oid;

	struct option options[] = {
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
	整数 ret;

	repo_config(the_repository, git_default_config, NULL);

	show_usage_with_options_if_asked(argc, argv,
					 commit_tree_usage, options);

	argc = parse_options(argc, argv, prefix, options, commit_tree_usage, 0);

	如果 (argc != 1)
		die(_("must give exactly one tree"));

	如果 (repo_get_oid_tree(the_repository, argv[0], &tree_oid))
		die(_("not a valid object name %s"), argv[0]);

	如果 (!buffer.len) {
		如果 (strbuf_read(&buffer, 0, 0) < 0)
			die_errno(_("git commit-tree: failed to read"));
	}

	如果 (commit_tree(buffer.buf, buffer.len, &tree_oid, parents, &commit_oid,
			NULL, sign_commit)) {
		ret = 1;
		goto out;
	}

	printf("%s\n", oid_to_hex(&commit_oid));
	ret = 0;

out:
	commit_list_free(parents);
	strbuf_release(&buffer);
	返回 ret;
}
