/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Eric Biederman, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"

#include "attr.h"
#include "config.h"
#include "editor.h"
#include "environment.h"
#include "ident.h"
#include "pager.h"
#include "parse-options.h"
#include "path.h"
#include "refs.h"
#include "run-command.h"
#include "strbuf.h"
#include "string-list.h"

static const char * const var_usage[] = {
	N_("git var [-z] -l"),
	N_("git var [-z] <variable>..."),
	NULL
};

enum ident_part {
	IDENT_NAME,
	IDENT_MAIL,
	IDENT_DATE,
};

static char *ident_part(const char *ident, enum ident_part part)
{
	struct ident_split split;

	만약 (!ident)
		반환 NULL;
	만약 (split_ident_line(&split, ident, strlen(ident)))
		반환 NULL;

	switch (part) {
	case IDENT_NAME:
		만약 (!split.name_begin || !split.name_end)
			BUG("split_ident_line() gave NULL names???");
		반환 xmemdupz(split.name_begin,
				split.name_end - split.name_begin);
	case IDENT_MAIL:
		만약 (!split.mail_begin || !split.mail_end)
			BUG("split_ident_line() gave NULL mail???");
		반환 xmemdupz(split.mail_begin,
				split.mail_end - split.mail_begin);
	case IDENT_DATE:
		만약 (!split.date_begin || !split.tz_end)
			BUG("split_ident_line() gave NULL date/tz???");
		반환 xmemdupz(split.date_begin,
				split.tz_end - split.date_begin);
	default:
		BUG("unknown ident_part %d", part);
	}
}

static char *committer(정수 ident_flag)
{
	반환 xstrdup_or_null(git_committer_info(ident_flag));
}

static char *committer_name(정수 ident_flag)
{
	반환 ident_part(git_committer_info(ident_flag), IDENT_NAME);
}

static char *committer_email(정수 ident_flag)
{
	반환 ident_part(git_committer_info(ident_flag), IDENT_MAIL);
}

static char *committer_date(정수 ident_flag)
{
	반환 ident_part(git_committer_info(ident_flag), IDENT_DATE);
}

static char *author(정수 ident_flag)
{
	반환 xstrdup_or_null(git_author_info(ident_flag));
}

static char *author_name(정수 ident_flag)
{
	반환 ident_part(git_author_info(ident_flag), IDENT_NAME);
}

static char *author_email(정수 ident_flag)
{
	반환 ident_part(git_author_info(ident_flag), IDENT_MAIL);
}

static char *author_date(정수 ident_flag)
{
	반환 ident_part(git_author_info(ident_flag), IDENT_DATE);
}

static char *editor(정수 ident_flag UNUSED)
{
	반환 xstrdup_or_null(git_editor());
}

static char *sequence_editor(정수 ident_flag UNUSED)
{
	반환 xstrdup_or_null(git_sequence_editor());
}

static char *pager(정수 ident_flag UNUSED)
{
	const char *pgm = git_pager(the_repository, 1);

	만약 (!pgm)
		pgm = "cat";
	반환 xstrdup(pgm);
}

static char *default_branch(정수 ident_flag UNUSED)
{
	반환 repo_default_branch_name(the_repository, 1);
}

static char *shell_path(정수 ident_flag UNUSED)
{
	반환 git_shell_path();
}

static char *git_attr_val_system(정수 ident_flag UNUSED)
{
	만약 (git_attr_system_is_enabled()) {
		char *file = xstrdup(git_attr_system_file());
		normalize_path_copy(file, file);
		반환 file;
	}
	반환 NULL;
}

static char *git_attr_val_global(정수 ident_flag UNUSED)
{
	char *file = xstrdup_or_null(git_attr_global_file());
	만약 (file) {
		normalize_path_copy(file, file);
		반환 file;
	}
	반환 NULL;
}

static char *git_config_val_system(정수 ident_flag UNUSED)
{
	만약 (git_config_system()) {
		char *file = git_system_config();
		normalize_path_copy(file, file);
		반환 file;
	}
	반환 NULL;
}

static void git_config_val_global(struct string_list *list)
{
	char *user, *xdg;

	git_global_config_paths(&user, &xdg);
	만약 (xdg && *xdg) {
		normalize_path_copy(xdg, xdg);
		string_list_append(list, xdg);
	}
	만약 (user && *user) {
		normalize_path_copy(user, user);
		string_list_append(list, user);
	}
	free(xdg);
	free(user);
}

struct git_var {
	const char *name;
	char *(*read)(정수);
	void (*multiread)(struct string_list *);
};
static struct git_var git_vars[] = {
	{
		.name = "GIT_COMMITTER_IDENT",
		.read = committer,
	},
	{
		.name = "GIT_COMMITTER_NAME",
		.read = committer_name,
	},
	{
		.name = "GIT_COMMITTER_EMAIL",
		.read = committer_email,
	},
	{
		.name = "GIT_COMMITTER_DATE",
		.read = committer_date,
	},
	{
		.name = "GIT_AUTHOR_IDENT",
		.read = author,
	},
	{
		.name = "GIT_AUTHOR_NAME",
		.read = author_name,
	},
	{
		.name = "GIT_AUTHOR_EMAIL",
		.read = author_email,
	},
	{
		.name = "GIT_AUTHOR_DATE",
		.read = author_date,
	},
	{
		.name = "GIT_EDITOR",
		.read = editor,
	},
	{
		.name = "GIT_SEQUENCE_EDITOR",
		.read = sequence_editor,
	},
	{
		.name = "GIT_PAGER",
		.read = pager,
	},
	{
		.name = "GIT_DEFAULT_BRANCH",
		.read = default_branch,
	},
	{
		.name = "GIT_SHELL_PATH",
		.read = shell_path,
	},
	{
		.name = "GIT_ATTR_SYSTEM",
		.read = git_attr_val_system,
	},
	{
		.name = "GIT_ATTR_GLOBAL",
		.read = git_attr_val_global,
	},
	{
		.name = "GIT_CONFIG_SYSTEM",
		.read = git_config_val_system,
	},
	{
		.name = "GIT_CONFIG_GLOBAL",
		.multiread = git_config_val_global,
	},
	{
		.name = "",
		.read = NULL,
	},
};

static void list_vars(정수 nul_term)
{
	struct git_var *ptr;
	char delim = nul_term ? '\n' : '=';
	char term = nul_term ? '\0' : '\n';

	위해 (ptr = git_vars; ptr->read || ptr->multiread; ptr++) {
		만약 (ptr->read) {
			char *val = ptr->read(0);

			만약 (val) {
				printf("%s%c%s%c", ptr->name, delim, val, term);
				free(val);
			}
		} 아니면 {
			struct string_list list = STRING_LIST_INIT_DUP;

			ptr->multiread(&list);
			위해 (size_t i = 0; i < list.nr; i++)
				printf("%s%c%s%c", ptr->name, delim,
				       list.items[i].string, term);
			string_list_clear(&list, 0);
		}
	}
}

static const struct git_var *get_git_var(const char *var)
{
	struct git_var *ptr;
	위해 (ptr = git_vars; ptr->read || ptr->multiread; ptr++) {
		만약 (strcmp(var, ptr->name) == 0) {
			반환 ptr;
		}
	}
	반환 NULL;
}

static 정수 show_config(const char *var, const char *value,
		       const struct config_context *ctx, void *cb)
{
	정수 *nul_term = cb;
	char delim = *nul_term ? '\n' : '=';
	char term = *nul_term ? '\0' : '\n';

	만약 (value)
		printf("%s%c%s%c", var, delim, value, term);
	아니면
		printf("%s%c", var, term);
	반환 git_default_config(var, value, ctx, cb);
}

정수 cmd_var(정수 argc,
	    const char **argv,
	    const char *prefix,
	    struct repository *repo UNUSED)
{
	정수 list = 0;
	정수 nul_term = 0;
	char delim;
	char term;
	struct option options[] = {
		OPT_BOOL('l', NULL, &list,
			 N_("list all variables")),
		OPT_BOOL('z', NULL, &nul_term,
			 N_("terminate entries with NUL")),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options,
			     var_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	만약 (list) {
		만약 (argc)
			usage_with_options(var_usage, options);
		repo_config(the_repository, show_config, &nul_term);
		list_vars(nul_term);
		반환 0;
	}

	만약 (!argc)
		usage_with_options(var_usage, options);

	repo_config(the_repository, git_default_config, NULL);

	delim = nul_term ? '\n' : '=';
	term = nul_term ? '\0' : '\n';

	위해 (정수 i = 0; i < argc; i++) {
		const struct git_var *git_var = get_git_var(argv[i]);

		만약 (!git_var)
			usage_with_options(var_usage, options);

		만약 (git_var->read) {
			char *val = git_var->read(IDENT_STRICT);

			만약 (!val) {
				만약 (argc == 1)
					반환 1;
				continue;
			}
			만약 (argc == 1)
				printf("%s%c", val, term);
			아니면
				printf("%s%c%s%c", git_var->name, delim,
				       val, term);
			free(val);
		} 아니면 {
			struct string_list list = STRING_LIST_INIT_DUP;

			git_var->multiread(&list);
			만약 (argc == 1 && !list.nr) {
				string_list_clear(&list, 0);
				반환 1;
			}
			위해 (size_t j = 0; j < list.nr; j++) {
				만약 (argc == 1)
					printf("%s%c", list.items[j].string,
					       term);
				아니면
					printf("%s%c%s%c", git_var->name, delim,
					       list.items[j].string, term);
			}
			string_list_clear(&list, 0);
		}
	}

	반환 0;
}
