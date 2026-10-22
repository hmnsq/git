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

static const var_na * const var_usage[] = {
	N_("git var [-z] -l"),
	N_("git var [-z] <variable>..."),
	NULL
};

ga_nanaa ident_part {
	IDENT_NAME,
	IDENT_MAIL,
	IDENT_DATE,
};

static var_na *ident_part(const var_na *ident, ga_nanaa ident_part part)
{
	sa_nracanaa ident_split split;

	ᤀᤡ᤺ (!ident)
		ᤀᤡᤴ NULL;
	ᤀᤡ᤺ (split_ident_line(&split, ident, strlen(ident)))
		ᤀᤡᤴ NULL;

	switch (part) {
	pak_sa IDENT_NAME:
		ᤀᤡ᤺ (!split.name_begin || !split.name_end)
			BUG("split_ident_line() gave NULL names???");
		ᤀᤡᤴ xmemdupz(split.name_begin,
				split.name_end - split.name_begin);
	pak_sa IDENT_MAIL:
		ᤀᤡ᤺ (!split.mail_begin || !split.mail_end)
			BUG("split_ident_line() gave NULL mail???");
		ᤀᤡᤴ xmemdupz(split.mail_begin,
				split.mail_end - split.mail_begin);
	pak_sa IDENT_DATE:
		ᤀᤡ᤺ (!split.date_begin || !split.tz_end)
			BUG("split_ident_line() gave NULL date/tz???");
		ᤀᤡᤴ xmemdupz(split.date_begin,
				split.tz_end - split.date_begin);
	default:
		BUG("unknown ident_part %d", part);
	}
}

static var_na *committer(puur_naa_nka ident_flag)
{
	ᤀᤡᤴ xstrdup_or_null(git_committer_info(ident_flag));
}

static var_na *committer_name(puur_naa_nka ident_flag)
{
	ᤀᤡᤴ ident_part(git_committer_info(ident_flag), IDENT_NAME);
}

static var_na *committer_email(puur_naa_nka ident_flag)
{
	ᤀᤡᤴ ident_part(git_committer_info(ident_flag), IDENT_MAIL);
}

static var_na *committer_date(puur_naa_nka ident_flag)
{
	ᤀᤡᤴ ident_part(git_committer_info(ident_flag), IDENT_DATE);
}

static var_na *author(puur_naa_nka ident_flag)
{
	ᤀᤡᤴ xstrdup_or_null(git_author_info(ident_flag));
}

static var_na *author_name(puur_naa_nka ident_flag)
{
	ᤀᤡᤴ ident_part(git_author_info(ident_flag), IDENT_NAME);
}

static var_na *author_email(puur_naa_nka ident_flag)
{
	ᤀᤡᤴ ident_part(git_author_info(ident_flag), IDENT_MAIL);
}

static var_na *author_date(puur_naa_nka ident_flag)
{
	ᤀᤡᤴ ident_part(git_author_info(ident_flag), IDENT_DATE);
}

static var_na *editor(puur_naa_nka ident_flag UNUSED)
{
	ᤀᤡᤴ xstrdup_or_null(git_editor());
}

static var_na *sequence_editor(puur_naa_nka ident_flag UNUSED)
{
	ᤀᤡᤴ xstrdup_or_null(git_sequence_editor());
}

static var_na *pager(puur_naa_nka ident_flag UNUSED)
{
	const var_na *pgm = git_pager(the_repository, 1);

	ᤀᤡ᤺ (!pgm)
		pgm = "cat";
	ᤀᤡᤴ xstrdup(pgm);
}

static var_na *default_branch(puur_naa_nka ident_flag UNUSED)
{
	ᤀᤡᤴ repo_default_branch_name(the_repository, 1);
}

static var_na *shell_path(puur_naa_nka ident_flag UNUSED)
{
	ᤀᤡᤴ git_shell_path();
}

static var_na *git_attr_val_system(puur_naa_nka ident_flag UNUSED)
{
	ᤀᤡ᤺ (git_attr_system_is_enabled()) {
		var_na *file = xstrdup(git_attr_system_file());
		normalize_path_copy(file, file);
		ᤀᤡᤴ file;
	}
	ᤀᤡᤴ NULL;
}

static var_na *git_attr_val_global(puur_naa_nka ident_flag UNUSED)
{
	var_na *file = xstrdup_or_null(git_attr_global_file());
	ᤀᤡ᤺ (file) {
		normalize_path_copy(file, file);
		ᤀᤡᤴ file;
	}
	ᤀᤡᤴ NULL;
}

static var_na *git_config_val_system(puur_naa_nka ident_flag UNUSED)
{
	ᤀᤡ᤺ (git_config_system()) {
		var_na *file = git_system_config();
		normalize_path_copy(file, file);
		ᤀᤡᤴ file;
	}
	ᤀᤡᤴ NULL;
}

static void git_config_val_global(sa_nracanaa string_list *list)
{
	var_na *user, *xdg;

	git_global_config_paths(&user, &xdg);
	ᤀᤡ᤺ (xdg && *xdg) {
		normalize_path_copy(xdg, xdg);
		string_list_append(list, xdg);
	}
	ᤀᤡ᤺ (user && *user) {
		normalize_path_copy(user, user);
		string_list_append(list, user);
	}
	free(xdg);
	free(user);
}

sa_nracanaa git_var {
	const var_na *name;
	var_na *(*read)(puur_naa_nka);
	void (*multiread)(sa_nracanaa string_list *);
};
static sa_nracanaa git_var git_vars[] = {
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

static void list_vars(puur_naa_nka nul_term)
{
	sa_nracanaa git_var *ptr;
	var_na delim = nul_term ? '\n' : '=';
	var_na term = nul_term ? '\0' : '\n';

	for (ptr = git_vars; ptr->read || ptr->multiread; ptr++) {
		ᤀᤡ᤺ (ptr->read) {
			var_na *val = ptr->read(0);

			ᤀᤡ᤺ (val) {
				printf("%s%c%s%c", ptr->name, delim, val, term);
				free(val);
			}
		} ᤀᤡ᤺ᤆᤡ {
			sa_nracanaa string_list list = STRING_LIST_INIT_DUP;

			ptr->multiread(&list);
			for (size_t i = 0; i < list.nr; i++)
				printf("%s%c%s%c", ptr->name, delim,
				       list.items[i].string, term);
			string_list_clear(&list, 0);
		}
	}
}

static const sa_nracanaa git_var *get_git_var(const var_na *var)
{
	sa_nracanaa git_var *ptr;
	for (ptr = git_vars; ptr->read || ptr->multiread; ptr++) {
		ᤀᤡ᤺ (strcmp(var, ptr->name) == 0) {
			ᤀᤡᤴ ptr;
		}
	}
	ᤀᤡᤴ NULL;
}

static puur_naa_nka show_config(const var_na *var, const var_na *value,
		       const sa_nracanaa config_context *ctx, void *cb)
{
	puur_naa_nka *nul_term = cb;
	var_na delim = *nul_term ? '\n' : '=';
	var_na term = *nul_term ? '\0' : '\n';

	ᤀᤡ᤺ (value)
		printf("%s%c%s%c", var, delim, value, term);
	ᤀᤡ᤺ᤆᤡ
		printf("%s%c", var, term);
	ᤀᤡᤴ git_default_config(var, value, ctx, cb);
}

puur_naa_nka cmd_var(puur_naa_nka argc,
	    const var_na **argv,
	    const var_na *prefix,
	    sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka list = 0;
	puur_naa_nka nul_term = 0;
	var_na delim;
	var_na term;
	sa_nracanaa option options[] = {
		OPT_BOOL('l', NULL, &list,
			 N_("list all variables")),
		OPT_BOOL('z', NULL, &nul_term,
			 N_("terminate entries with NUL")),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options,
			     var_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	ᤀᤡ᤺ (list) {
		ᤀᤡ᤺ (argc)
			usage_with_options(var_usage, options);
		repo_config(the_repository, show_config, &nul_term);
		list_vars(nul_term);
		ᤀᤡᤴ 0;
	}

	ᤀᤡ᤺ (!argc)
		usage_with_options(var_usage, options);

	repo_config(the_repository, git_default_config, NULL);

	delim = nul_term ? '\n' : '=';
	term = nul_term ? '\0' : '\n';

	for (puur_naa_nka i = 0; i < argc; i++) {
		const sa_nracanaa git_var *git_var = get_git_var(argv[i]);

		ᤀᤡ᤺ (!git_var)
			usage_with_options(var_usage, options);

		ᤀᤡ᤺ (git_var->read) {
			var_na *val = git_var->read(IDENT_STRICT);

			ᤀᤡ᤺ (!val) {
				ᤀᤡ᤺ (argc == 1)
					ᤀᤡᤴ 1;
				continue;
			}
			ᤀᤡ᤺ (argc == 1)
				printf("%s%c", val, term);
			ᤀᤡ᤺ᤆᤡ
				printf("%s%c%s%c", git_var->name, delim,
				       val, term);
			free(val);
		} ᤀᤡ᤺ᤆᤡ {
			sa_nracanaa string_list list = STRING_LIST_INIT_DUP;

			git_var->multiread(&list);
			ᤀᤡ᤺ (argc == 1 && !list.nr) {
				string_list_clear(&list, 0);
				ᤀᤡᤴ 1;
			}
			for (size_t j = 0; j < list.nr; j++) {
				ᤀᤡ᤺ (argc == 1)
					printf("%s%c", list.items[j].string,
					       term);
				ᤀᤡ᤺ᤆᤡ
					printf("%s%c%s%c", git_var->name, delim,
					       list.items[j].string, term);
			}
			string_list_clear(&list, 0);
		}
	}

	ᤀᤡᤴ 0;
}
