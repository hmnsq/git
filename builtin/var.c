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

	nếu (!ident)
		trả_về NULL;
	nếu (split_ident_line(&split, ident, strlen(ident)))
		trả_về NULL;

	switch (part) {
	case IDENT_NAME:
		nếu (!split.name_begin || !split.name_end)
			BUG("split_ident_line() gave NULL names???");
		trả_về xmemdupz(split.name_begin,
				split.name_end - split.name_begin);
	case IDENT_MAIL:
		nếu (!split.mail_begin || !split.mail_end)
			BUG("split_ident_line() gave NULL mail???");
		trả_về xmemdupz(split.mail_begin,
				split.mail_end - split.mail_begin);
	case IDENT_DATE:
		nếu (!split.date_begin || !split.tz_end)
			BUG("split_ident_line() gave NULL date/tz???");
		trả_về xmemdupz(split.date_begin,
				split.tz_end - split.date_begin);
	default:
		BUG("unknown ident_part %d", part);
	}
}

static char *committer(số_nguyên ident_flag)
{
	trả_về xstrdup_or_null(git_committer_info(ident_flag));
}

static char *committer_name(số_nguyên ident_flag)
{
	trả_về ident_part(git_committer_info(ident_flag), IDENT_NAME);
}

static char *committer_email(số_nguyên ident_flag)
{
	trả_về ident_part(git_committer_info(ident_flag), IDENT_MAIL);
}

static char *committer_date(số_nguyên ident_flag)
{
	trả_về ident_part(git_committer_info(ident_flag), IDENT_DATE);
}

static char *author(số_nguyên ident_flag)
{
	trả_về xstrdup_or_null(git_author_info(ident_flag));
}

static char *author_name(số_nguyên ident_flag)
{
	trả_về ident_part(git_author_info(ident_flag), IDENT_NAME);
}

static char *author_email(số_nguyên ident_flag)
{
	trả_về ident_part(git_author_info(ident_flag), IDENT_MAIL);
}

static char *author_date(số_nguyên ident_flag)
{
	trả_về ident_part(git_author_info(ident_flag), IDENT_DATE);
}

static char *editor(số_nguyên ident_flag UNUSED)
{
	trả_về xstrdup_or_null(git_editor());
}

static char *sequence_editor(số_nguyên ident_flag UNUSED)
{
	trả_về xstrdup_or_null(git_sequence_editor());
}

static char *pager(số_nguyên ident_flag UNUSED)
{
	const char *pgm = git_pager(the_repository, 1);

	nếu (!pgm)
		pgm = "cat";
	trả_về xstrdup(pgm);
}

static char *default_branch(số_nguyên ident_flag UNUSED)
{
	trả_về repo_default_branch_name(the_repository, 1);
}

static char *shell_path(số_nguyên ident_flag UNUSED)
{
	trả_về git_shell_path();
}

static char *git_attr_val_system(số_nguyên ident_flag UNUSED)
{
	nếu (git_attr_system_is_enabled()) {
		char *file = xstrdup(git_attr_system_file());
		normalize_path_copy(file, file);
		trả_về file;
	}
	trả_về NULL;
}

static char *git_attr_val_global(số_nguyên ident_flag UNUSED)
{
	char *file = xstrdup_or_null(git_attr_global_file());
	nếu (file) {
		normalize_path_copy(file, file);
		trả_về file;
	}
	trả_về NULL;
}

static char *git_config_val_system(số_nguyên ident_flag UNUSED)
{
	nếu (git_config_system()) {
		char *file = git_system_config();
		normalize_path_copy(file, file);
		trả_về file;
	}
	trả_về NULL;
}

static void git_config_val_global(struct string_list *list)
{
	char *user, *xdg;

	git_global_config_paths(&user, &xdg);
	nếu (xdg && *xdg) {
		normalize_path_copy(xdg, xdg);
		string_list_append(list, xdg);
	}
	nếu (user && *user) {
		normalize_path_copy(user, user);
		string_list_append(list, user);
	}
	free(xdg);
	free(user);
}

struct git_var {
	const char *name;
	char *(*read)(số_nguyên);
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

static void list_vars(số_nguyên nul_term)
{
	struct git_var *ptr;
	char delim = nul_term ? '\n' : '=';
	char term = nul_term ? '\0' : '\n';

	cho (ptr = git_vars; ptr->read || ptr->multiread; ptr++) {
		nếu (ptr->read) {
			char *val = ptr->read(0);

			nếu (val) {
				printf("%s%c%s%c", ptr->name, delim, val, term);
				free(val);
			}
		} không_thì {
			struct string_list list = STRING_LIST_INIT_DUP;

			ptr->multiread(&list);
			cho (size_t i = 0; i < list.nr; i++)
				printf("%s%c%s%c", ptr->name, delim,
				       list.items[i].string, term);
			string_list_clear(&list, 0);
		}
	}
}

static const struct git_var *get_git_var(const char *var)
{
	struct git_var *ptr;
	cho (ptr = git_vars; ptr->read || ptr->multiread; ptr++) {
		nếu (strcmp(var, ptr->name) == 0) {
			trả_về ptr;
		}
	}
	trả_về NULL;
}

static số_nguyên show_config(const char *var, const char *value,
		       const struct config_context *ctx, void *cb)
{
	số_nguyên *nul_term = cb;
	char delim = *nul_term ? '\n' : '=';
	char term = *nul_term ? '\0' : '\n';

	nếu (value)
		printf("%s%c%s%c", var, delim, value, term);
	không_thì
		printf("%s%c", var, term);
	trả_về git_default_config(var, value, ctx, cb);
}

số_nguyên cmd_var(số_nguyên argc,
	    const char **argv,
	    const char *prefix,
	    struct repository *repo UNUSED)
{
	số_nguyên list = 0;
	số_nguyên nul_term = 0;
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

	nếu (list) {
		nếu (argc)
			usage_with_options(var_usage, options);
		repo_config(the_repository, show_config, &nul_term);
		list_vars(nul_term);
		trả_về 0;
	}

	nếu (!argc)
		usage_with_options(var_usage, options);

	repo_config(the_repository, git_default_config, NULL);

	delim = nul_term ? '\n' : '=';
	term = nul_term ? '\0' : '\n';

	cho (số_nguyên i = 0; i < argc; i++) {
		const struct git_var *git_var = get_git_var(argv[i]);

		nếu (!git_var)
			usage_with_options(var_usage, options);

		nếu (git_var->read) {
			char *val = git_var->read(IDENT_STRICT);

			nếu (!val) {
				nếu (argc == 1)
					trả_về 1;
				continue;
			}
			nếu (argc == 1)
				printf("%s%c", val, term);
			không_thì
				printf("%s%c%s%c", git_var->name, delim,
				       val, term);
			free(val);
		} không_thì {
			struct string_list list = STRING_LIST_INIT_DUP;

			git_var->multiread(&list);
			nếu (argc == 1 && !list.nr) {
				string_list_clear(&list, 0);
				trả_về 1;
			}
			cho (size_t j = 0; j < list.nr; j++) {
				nếu (argc == 1)
					printf("%s%c", list.items[j].string,
					       term);
				không_thì
					printf("%s%c%s%c", git_var->name, delim,
					       list.items[j].string, term);
			}
			string_list_clear(&list, 0);
		}
	}

	trả_về 0;
}
