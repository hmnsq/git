/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Eric Biederman, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE

#അന്തര്ഭാവ "builtin.h"

#അന്തര്ഭാവ "attr.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "editor.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "ident.h"
#അന്തര്ഭാവ "pager.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "strbuf.h"
#അന്തര്ഭാവ "string-list.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം var_usage[] = {
	N_("git var [-z] -l"),
	N_("git var [-z] <variable>..."),
	ശൂന്യ
};

എണ്ണൽ ident_part {
	IDENT_NAME,
	IDENT_MAIL,
	IDENT_DATE,
};

സ്ഥിതം അക്ഷരം *ident_part(സ്ഥിരം അക്ഷരം *ident, എണ്ണൽ ident_part part)
{
	ഘടന ident_split split;

	എങ്കിൽ (!ident)
		തിരികെ ശൂന്യ;
	എങ്കിൽ (split_ident_വരി(&split, ident, strlen(ident)))
		തിരികെ ശൂന്യ;

	തിരഞ്ഞെടുപ്പ് (part) {
	അവസ്ഥ IDENT_NAME:
		എങ്കിൽ (!split.name_begin || !split.name_end)
			BUG("split_ident_line() gave NULL names???");
		തിരികെ xmemdupz(split.name_begin,
				split.name_end - split.name_begin);
	അവസ്ഥ IDENT_MAIL:
		എങ്കിൽ (!split.mail_begin || !split.mail_end)
			BUG("split_ident_line() gave NULL mail???");
		തിരികെ xmemdupz(split.mail_begin,
				split.mail_end - split.mail_begin);
	അവസ്ഥ IDENT_DATE:
		എങ്കിൽ (!split.date_begin || !split.tz_end)
			BUG("split_ident_line() gave NULL date/tz???");
		തിരികെ xmemdupz(split.date_begin,
				split.tz_end - split.date_begin);
	സ്ഥിരസ്ഥിതി:
		BUG("unknown ident_part %d", part);
	}
}

സ്ഥിതം അക്ഷരം *committer(പൂർണ്ണസംഖ്യ ident_flag)
{
	തിരികെ xstrdup_or_null(git_committer_info(ident_flag));
}

സ്ഥിതം അക്ഷരം *committer_name(പൂർണ്ണസംഖ്യ ident_flag)
{
	തിരികെ ident_part(git_committer_info(ident_flag), IDENT_NAME);
}

സ്ഥിതം അക്ഷരം *committer_email(പൂർണ്ണസംഖ്യ ident_flag)
{
	തിരികെ ident_part(git_committer_info(ident_flag), IDENT_MAIL);
}

സ്ഥിതം അക്ഷരം *committer_date(പൂർണ്ണസംഖ്യ ident_flag)
{
	തിരികെ ident_part(git_committer_info(ident_flag), IDENT_DATE);
}

സ്ഥിതം അക്ഷരം *author(പൂർണ്ണസംഖ്യ ident_flag)
{
	തിരികെ xstrdup_or_null(git_author_info(ident_flag));
}

സ്ഥിതം അക്ഷരം *author_name(പൂർണ്ണസംഖ്യ ident_flag)
{
	തിരികെ ident_part(git_author_info(ident_flag), IDENT_NAME);
}

സ്ഥിതം അക്ഷരം *author_email(പൂർണ്ണസംഖ്യ ident_flag)
{
	തിരികെ ident_part(git_author_info(ident_flag), IDENT_MAIL);
}

സ്ഥിതം അക്ഷരം *author_date(പൂർണ്ണസംഖ്യ ident_flag)
{
	തിരികെ ident_part(git_author_info(ident_flag), IDENT_DATE);
}

സ്ഥിതം അക്ഷരം *editor(പൂർണ്ണസംഖ്യ ident_flag UNUSED)
{
	തിരികെ xstrdup_or_null(git_editor());
}

സ്ഥിതം അക്ഷരം *sequence_editor(പൂർണ്ണസംഖ്യ ident_flag UNUSED)
{
	തിരികെ xstrdup_or_null(git_sequence_editor());
}

സ്ഥിതം അക്ഷരം *pager(പൂർണ്ണസംഖ്യ ident_flag UNUSED)
{
	സ്ഥിരം അക്ഷരം *pgm = git_pager(the_repository, 1);

	എങ്കിൽ (!pgm)
		pgm = "cat";
	തിരികെ xstrdup(pgm);
}

സ്ഥിതം അക്ഷരം *default_branch(പൂർണ്ണസംഖ്യ ident_flag UNUSED)
{
	തിരികെ repo_default_branch_name(the_repository, 1);
}

സ്ഥിതം അക്ഷരം *shell_path(പൂർണ്ണസംഖ്യ ident_flag UNUSED)
{
	തിരികെ git_shell_path();
}

സ്ഥിതം അക്ഷരം *git_attr_val_system(പൂർണ്ണസംഖ്യ ident_flag UNUSED)
{
	എങ്കിൽ (git_attr_system_is_enabled()) {
		അക്ഷരം *file = xstrdup(git_attr_system_file());
		normalize_path_copy(file, file);
		തിരികെ file;
	}
	തിരികെ ശൂന്യ;
}

സ്ഥിതം അക്ഷരം *git_attr_val_global(പൂർണ്ണസംഖ്യ ident_flag UNUSED)
{
	അക്ഷരം *file = xstrdup_or_null(git_attr_global_file());
	എങ്കിൽ (file) {
		normalize_path_copy(file, file);
		തിരികെ file;
	}
	തിരികെ ശൂന്യ;
}

സ്ഥിതം അക്ഷരം *git_config_val_system(പൂർണ്ണസംഖ്യ ident_flag UNUSED)
{
	എങ്കിൽ (git_config_system()) {
		അക്ഷരം *file = git_system_config();
		normalize_path_copy(file, file);
		തിരികെ file;
	}
	തിരികെ ശൂന്യ;
}

സ്ഥിതം ശൂന്യം git_config_val_global(ഘടന string_list *list)
{
	അക്ഷരം *user, *xdg;

	git_global_config_paths(&user, &xdg);
	എങ്കിൽ (xdg && *xdg) {
		normalize_path_copy(xdg, xdg);
		string_list_append(list, xdg);
	}
	എങ്കിൽ (user && *user) {
		normalize_path_copy(user, user);
		string_list_append(list, user);
	}
	സ്വതന്ത്രം(xdg);
	സ്വതന്ത്രം(user);
}

ഘടന git_var {
	സ്ഥിരം അക്ഷരം *name;
	അക്ഷരം *(*പഠ)(പൂർണ്ണസംഖ്യ);
	ശൂന്യം (*multiread)(ഘടന string_list *);
};
സ്ഥിതം ഘടന git_var git_vars[] = {
	{
		.name = "GIT_COMMITTER_IDENT",
		.പഠ = committer,
	},
	{
		.name = "GIT_COMMITTER_NAME",
		.പഠ = committer_name,
	},
	{
		.name = "GIT_COMMITTER_EMAIL",
		.പഠ = committer_email,
	},
	{
		.name = "GIT_COMMITTER_DATE",
		.പഠ = committer_date,
	},
	{
		.name = "GIT_AUTHOR_IDENT",
		.പഠ = author,
	},
	{
		.name = "GIT_AUTHOR_NAME",
		.പഠ = author_name,
	},
	{
		.name = "GIT_AUTHOR_EMAIL",
		.പഠ = author_email,
	},
	{
		.name = "GIT_AUTHOR_DATE",
		.പഠ = author_date,
	},
	{
		.name = "GIT_EDITOR",
		.പഠ = editor,
	},
	{
		.name = "GIT_SEQUENCE_EDITOR",
		.പഠ = sequence_editor,
	},
	{
		.name = "GIT_PAGER",
		.പഠ = pager,
	},
	{
		.name = "GIT_DEFAULT_BRANCH",
		.പഠ = default_branch,
	},
	{
		.name = "GIT_SHELL_PATH",
		.പഠ = shell_path,
	},
	{
		.name = "GIT_ATTR_SYSTEM",
		.പഠ = git_attr_val_system,
	},
	{
		.name = "GIT_ATTR_GLOBAL",
		.പഠ = git_attr_val_global,
	},
	{
		.name = "GIT_CONFIG_SYSTEM",
		.പഠ = git_config_val_system,
	},
	{
		.name = "GIT_CONFIG_GLOBAL",
		.multiread = git_config_val_global,
	},
	{
		.name = "",
		.പഠ = ശൂന്യ,
	},
};

സ്ഥിതം ശൂന്യം list_vars(പൂർണ്ണസംഖ്യ nul_term)
{
	ഘടന git_var *ptr;
	അക്ഷരം delim = nul_term ? '\n' : '=';
	അക്ഷരം term = nul_term ? '\0' : '\n';

	വേണ്ടി (ptr = git_vars; ptr->പഠ || ptr->multiread; ptr++) {
		എങ്കിൽ (ptr->പഠ) {
			അക്ഷരം *val = ptr->പഠ(0);

			എങ്കിൽ (val) {
				printf("%s%c%s%c", ptr->name, delim, val, term);
				സ്വതന്ത്രം(val);
			}
		} അല്ലെങ്കിൽ {
			ഘടന string_list list = STRING_LIST_INIT_DUP;

			ptr->multiread(&list);
			വേണ്ടി (size_t i = 0; i < list.nr; i++)
				printf("%s%c%s%c", ptr->name, delim,
				       list.ഇനങ്ങൾ[i].string, term);
			string_list_clear(&list, 0);
		}
	}
}

സ്ഥിതം സ്ഥിരം ഘടന git_var *get_git_var(സ്ഥിരം അക്ഷരം *var)
{
	ഘടന git_var *ptr;
	വേണ്ടി (ptr = git_vars; ptr->പഠ || ptr->multiread; ptr++) {
		എങ്കിൽ (strcmp(var, ptr->name) == 0) {
			തിരികെ ptr;
		}
	}
	തിരികെ ശൂന്യ;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
		       സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	പൂർണ്ണസംഖ്യ *nul_term = cb;
	അക്ഷരം delim = *nul_term ? '\n' : '=';
	അക്ഷരം term = *nul_term ? '\0' : '\n';

	എങ്കിൽ (മൂല്യം)
		printf("%s%c%s%c", var, delim, മൂല്യം, term);
	അല്ലെങ്കിൽ
		printf("%s%c", var, term);
	തിരികെ git_default_config(var, മൂല്യം, ctx, cb);
}

പൂർണ്ണസംഖ്യ cmd_var(പൂർണ്ണസംഖ്യ argc,
	    സ്ഥിരം അക്ഷരം **argv,
	    സ്ഥിരം അക്ഷരം *prefix,
	    ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ list = 0;
	പൂർണ്ണസംഖ്യ nul_term = 0;
	അക്ഷരം delim;
	അക്ഷരം term;
	ഘടന option options[] = {
		OPT_BOOL('l', ശൂന്യ, &list,
			 N_("list all variables")),
		OPT_BOOL('z', ശൂന്യ, &nul_term,
			 N_("terminate entries with NUL")),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options,
			     var_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	എങ്കിൽ (list) {
		എങ്കിൽ (argc)
			usage_with_options(var_usage, options);
		repo_config(the_repository, show_config, &nul_term);
		list_vars(nul_term);
		തിരികെ 0;
	}

	എങ്കിൽ (!argc)
		usage_with_options(var_usage, options);

	repo_config(the_repository, git_default_config, ശൂന്യ);

	delim = nul_term ? '\n' : '=';
	term = nul_term ? '\0' : '\n';

	വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < argc; i++) {
		സ്ഥിരം ഘടന git_var *git_var = get_git_var(argv[i]);

		എങ്കിൽ (!git_var)
			usage_with_options(var_usage, options);

		എങ്കിൽ (git_var->പഠ) {
			അക്ഷരം *val = git_var->പഠ(IDENT_STRICT);

			എങ്കിൽ (!val) {
				എങ്കിൽ (argc == 1)
					തിരികെ 1;
				തുടരുക;
			}
			എങ്കിൽ (argc == 1)
				printf("%s%c", val, term);
			അല്ലെങ്കിൽ
				printf("%s%c%s%c", git_var->name, delim,
				       val, term);
			സ്വതന്ത്രം(val);
		} അല്ലെങ്കിൽ {
			ഘടന string_list list = STRING_LIST_INIT_DUP;

			git_var->multiread(&list);
			എങ്കിൽ (argc == 1 && !list.nr) {
				string_list_clear(&list, 0);
				തിരികെ 1;
			}
			വേണ്ടി (size_t j = 0; j < list.nr; j++) {
				എങ്കിൽ (argc == 1)
					printf("%s%c", list.ഇനങ്ങൾ[j].string,
					       term);
				അല്ലെങ്കിൽ
					printf("%s%c%s%c", git_var->name, delim,
					       list.ഇനങ്ങൾ[j].string, term);
			}
			string_list_clear(&list, 0);
		}
	}

	തിരികെ 0;
}
