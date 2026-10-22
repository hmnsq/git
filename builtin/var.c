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

	если (!ident)
		вернуть NULL;
	если (split_ident_line(&split, ident, strlen(ident)))
		вернуть NULL;

	switch (part) {
	case IDENT_NAME:
		если (!split.name_begin || !split.name_end)
			BUG("split_ident_line() gave NULL names???");
		вернуть xmemdupz(split.name_begin,
				split.name_end - split.name_begin);
	case IDENT_MAIL:
		если (!split.mail_begin || !split.mail_end)
			BUG("split_ident_line() gave NULL mail???");
		вернуть xmemdupz(split.mail_begin,
				split.mail_end - split.mail_begin);
	case IDENT_DATE:
		если (!split.date_begin || !split.tz_end)
			BUG("split_ident_line() gave NULL date/tz???");
		вернуть xmemdupz(split.date_begin,
				split.tz_end - split.date_begin);
	default:
		BUG("unknown ident_part %d", part);
	}
}

static char *committer(целое ident_flag)
{
	вернуть xstrdup_or_null(git_committer_info(ident_flag));
}

static char *committer_name(целое ident_flag)
{
	вернуть ident_part(git_committer_info(ident_flag), IDENT_NAME);
}

static char *committer_email(целое ident_flag)
{
	вернуть ident_part(git_committer_info(ident_flag), IDENT_MAIL);
}

static char *committer_date(целое ident_flag)
{
	вернуть ident_part(git_committer_info(ident_flag), IDENT_DATE);
}

static char *author(целое ident_flag)
{
	вернуть xstrdup_or_null(git_author_info(ident_flag));
}

static char *author_name(целое ident_flag)
{
	вернуть ident_part(git_author_info(ident_flag), IDENT_NAME);
}

static char *author_email(целое ident_flag)
{
	вернуть ident_part(git_author_info(ident_flag), IDENT_MAIL);
}

static char *author_date(целое ident_flag)
{
	вернуть ident_part(git_author_info(ident_flag), IDENT_DATE);
}

static char *editor(целое ident_flag UNUSED)
{
	вернуть xstrdup_or_null(git_editor());
}

static char *sequence_editor(целое ident_flag UNUSED)
{
	вернуть xstrdup_or_null(git_sequence_editor());
}

static char *pager(целое ident_flag UNUSED)
{
	const char *pgm = git_pager(the_repository, 1);

	если (!pgm)
		pgm = "cat";
	вернуть xstrdup(pgm);
}

static char *default_branch(целое ident_flag UNUSED)
{
	вернуть repo_default_branch_name(the_repository, 1);
}

static char *shell_path(целое ident_flag UNUSED)
{
	вернуть git_shell_path();
}

static char *git_attr_val_system(целое ident_flag UNUSED)
{
	если (git_attr_system_is_enabled()) {
		char *file = xstrdup(git_attr_system_file());
		normalize_path_copy(file, file);
		вернуть file;
	}
	вернуть NULL;
}

static char *git_attr_val_global(целое ident_flag UNUSED)
{
	char *file = xstrdup_or_null(git_attr_global_file());
	если (file) {
		normalize_path_copy(file, file);
		вернуть file;
	}
	вернуть NULL;
}

static char *git_config_val_system(целое ident_flag UNUSED)
{
	если (git_config_system()) {
		char *file = git_system_config();
		normalize_path_copy(file, file);
		вернуть file;
	}
	вернуть NULL;
}

static void git_config_val_global(struct string_list *list)
{
	char *user, *xdg;

	git_global_config_paths(&user, &xdg);
	если (xdg && *xdg) {
		normalize_path_copy(xdg, xdg);
		string_list_append(list, xdg);
	}
	если (user && *user) {
		normalize_path_copy(user, user);
		string_list_append(list, user);
	}
	free(xdg);
	free(user);
}

struct git_var {
	const char *name;
	char *(*read)(целое);
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

static void list_vars(целое nul_term)
{
	struct git_var *ptr;
	char delim = nul_term ? '\n' : '=';
	char term = nul_term ? '\0' : '\n';

	для (ptr = git_vars; ptr->read || ptr->multiread; ptr++) {
		если (ptr->read) {
			char *val = ptr->read(0);

			если (val) {
				printf("%s%c%s%c", ptr->name, delim, val, term);
				free(val);
			}
		} иначе {
			struct string_list list = STRING_LIST_INIT_DUP;

			ptr->multiread(&list);
			для (size_t i = 0; i < list.nr; i++)
				printf("%s%c%s%c", ptr->name, delim,
				       list.items[i].string, term);
			string_list_clear(&list, 0);
		}
	}
}

static const struct git_var *get_git_var(const char *var)
{
	struct git_var *ptr;
	для (ptr = git_vars; ptr->read || ptr->multiread; ptr++) {
		если (strcmp(var, ptr->name) == 0) {
			вернуть ptr;
		}
	}
	вернуть NULL;
}

static целое show_config(const char *var, const char *value,
		       const struct config_context *ctx, void *cb)
{
	целое *nul_term = cb;
	char delim = *nul_term ? '\n' : '=';
	char term = *nul_term ? '\0' : '\n';

	если (value)
		printf("%s%c%s%c", var, delim, value, term);
	иначе
		printf("%s%c", var, term);
	вернуть git_default_config(var, value, ctx, cb);
}

целое cmd_var(целое argc,
	    const char **argv,
	    const char *prefix,
	    struct repository *repo UNUSED)
{
	целое list = 0;
	целое nul_term = 0;
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

	если (list) {
		если (argc)
			usage_with_options(var_usage, options);
		repo_config(the_repository, show_config, &nul_term);
		list_vars(nul_term);
		вернуть 0;
	}

	если (!argc)
		usage_with_options(var_usage, options);

	repo_config(the_repository, git_default_config, NULL);

	delim = nul_term ? '\n' : '=';
	term = nul_term ? '\0' : '\n';

	для (целое i = 0; i < argc; i++) {
		const struct git_var *git_var = get_git_var(argv[i]);

		если (!git_var)
			usage_with_options(var_usage, options);

		если (git_var->read) {
			char *val = git_var->read(IDENT_STRICT);

			если (!val) {
				если (argc == 1)
					вернуть 1;
				continue;
			}
			если (argc == 1)
				printf("%s%c", val, term);
			иначе
				printf("%s%c%s%c", git_var->name, delim,
				       val, term);
			free(val);
		} иначе {
			struct string_list list = STRING_LIST_INIT_DUP;

			git_var->multiread(&list);
			если (argc == 1 && !list.nr) {
				string_list_clear(&list, 0);
				вернуть 1;
			}
			для (size_t j = 0; j < list.nr; j++) {
				если (argc == 1)
					printf("%s%c", list.items[j].string,
					       term);
				иначе
					printf("%s%c%s%c", git_var->name, delim,
					       list.items[j].string, term);
			}
			string_list_clear(&list, 0);
		}
	}

	вернуть 0;
}
