/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Eric Biederman, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE

#ಅನ್ತರ್ಭಾವ "builtin.h"

#ಅನ್ತರ್ಭಾವ "attr.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "editor.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "ident.h"
#ಅನ್ತರ್ಭಾವ "pager.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ var_usage[] = {
	N_("git var [-z] -l"),
	N_("git var [-z] <variable>..."),
	ರಿಕ್ತ
};

ಗಣನಾ ident_part {
	IDENT_NAME,
	IDENT_MAIL,
	IDENT_DATE,
};

ಸ್ಥಿರ ವರ್ಣ *ident_part(ಧ್ರುವ ವರ್ಣ *ident, ಗಣನಾ ident_part part)
{
	ಸಂರಚನಾ ident_split split;

	ಯದಿ (!ident)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	ಯದಿ (split_ident_ಸಾಲು(&split, ident, strlen(ident)))
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	ವಿಕಲ್ಪ (part) {
	ಪಕ್ಷ IDENT_NAME:
		ಯದಿ (!split.name_begin || !split.name_end)
			BUG("split_ident_line() gave NULL names???");
		ಪ್ರತ್ಯಾಗಮ xmemdupz(split.name_begin,
				split.name_end - split.name_begin);
	ಪಕ್ಷ IDENT_MAIL:
		ಯದಿ (!split.mail_begin || !split.mail_end)
			BUG("split_ident_line() gave NULL mail???");
		ಪ್ರತ್ಯಾಗಮ xmemdupz(split.mail_begin,
				split.mail_end - split.mail_begin);
	ಪಕ್ಷ IDENT_DATE:
		ಯದಿ (!split.date_begin || !split.tz_end)
			BUG("split_ident_line() gave NULL date/tz???");
		ಪ್ರತ್ಯಾಗಮ xmemdupz(split.date_begin,
				split.tz_end - split.date_begin);
	ಸ್ವಭಾವ:
		BUG("unknown ident_part %d", part);
	}
}

ಸ್ಥಿರ ವರ್ಣ *committer(ಪೂರ್ಣಾಙ್ಕ ident_flag)
{
	ಪ್ರತ್ಯಾಗಮ xstrdup_or_null(git_committer_info(ident_flag));
}

ಸ್ಥಿರ ವರ್ಣ *committer_name(ಪೂರ್ಣಾಙ್ಕ ident_flag)
{
	ಪ್ರತ್ಯಾಗಮ ident_part(git_committer_info(ident_flag), IDENT_NAME);
}

ಸ್ಥಿರ ವರ್ಣ *committer_email(ಪೂರ್ಣಾಙ್ಕ ident_flag)
{
	ಪ್ರತ್ಯಾಗಮ ident_part(git_committer_info(ident_flag), IDENT_MAIL);
}

ಸ್ಥಿರ ವರ್ಣ *committer_date(ಪೂರ್ಣಾಙ್ಕ ident_flag)
{
	ಪ್ರತ್ಯಾಗಮ ident_part(git_committer_info(ident_flag), IDENT_DATE);
}

ಸ್ಥಿರ ವರ್ಣ *author(ಪೂರ್ಣಾಙ್ಕ ident_flag)
{
	ಪ್ರತ್ಯಾಗಮ xstrdup_or_null(git_author_info(ident_flag));
}

ಸ್ಥಿರ ವರ್ಣ *author_name(ಪೂರ್ಣಾಙ್ಕ ident_flag)
{
	ಪ್ರತ್ಯಾಗಮ ident_part(git_author_info(ident_flag), IDENT_NAME);
}

ಸ್ಥಿರ ವರ್ಣ *author_email(ಪೂರ್ಣಾಙ್ಕ ident_flag)
{
	ಪ್ರತ್ಯಾಗಮ ident_part(git_author_info(ident_flag), IDENT_MAIL);
}

ಸ್ಥಿರ ವರ್ಣ *author_date(ಪೂರ್ಣಾಙ್ಕ ident_flag)
{
	ಪ್ರತ್ಯಾಗಮ ident_part(git_author_info(ident_flag), IDENT_DATE);
}

ಸ್ಥಿರ ವರ್ಣ *editor(ಪೂರ್ಣಾಙ್ಕ ident_flag UNUSED)
{
	ಪ್ರತ್ಯಾಗಮ xstrdup_or_null(git_editor());
}

ಸ್ಥಿರ ವರ್ಣ *sequence_editor(ಪೂರ್ಣಾಙ್ಕ ident_flag UNUSED)
{
	ಪ್ರತ್ಯಾಗಮ xstrdup_or_null(git_sequence_editor());
}

ಸ್ಥಿರ ವರ್ಣ *pager(ಪೂರ್ಣಾಙ್ಕ ident_flag UNUSED)
{
	ಧ್ರುವ ವರ್ಣ *pgm = git_pager(the_repository, 1);

	ಯದಿ (!pgm)
		pgm = "cat";
	ಪ್ರತ್ಯಾಗಮ xstrdup(pgm);
}

ಸ್ಥಿರ ವರ್ಣ *default_branch(ಪೂರ್ಣಾಙ್ಕ ident_flag UNUSED)
{
	ಪ್ರತ್ಯಾಗಮ repo_default_branch_name(the_repository, 1);
}

ಸ್ಥಿರ ವರ್ಣ *shell_path(ಪೂರ್ಣಾಙ್ಕ ident_flag UNUSED)
{
	ಪ್ರತ್ಯಾಗಮ git_shell_path();
}

ಸ್ಥಿರ ವರ್ಣ *git_attr_val_system(ಪೂರ್ಣಾಙ್ಕ ident_flag UNUSED)
{
	ಯದಿ (git_attr_system_is_enabled()) {
		ವರ್ಣ *file = xstrdup(git_attr_system_file());
		normalize_path_copy(file, file);
		ಪ್ರತ್ಯಾಗಮ file;
	}
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ವರ್ಣ *git_attr_val_global(ಪೂರ್ಣಾಙ್ಕ ident_flag UNUSED)
{
	ವರ್ಣ *file = xstrdup_or_null(git_attr_global_file());
	ಯದಿ (file) {
		normalize_path_copy(file, file);
		ಪ್ರತ್ಯಾಗಮ file;
	}
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ವರ್ಣ *git_config_val_system(ಪೂರ್ಣಾಙ್ಕ ident_flag UNUSED)
{
	ಯದಿ (git_config_system()) {
		ವರ್ಣ *file = git_system_config();
		normalize_path_copy(file, file);
		ಪ್ರತ್ಯಾಗಮ file;
	}
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಶೂನ್ಯ git_config_val_global(ಸಂರಚನಾ string_list *list)
{
	ವರ್ಣ *user, *xdg;

	git_global_config_paths(&user, &xdg);
	ಯದಿ (xdg && *xdg) {
		normalize_path_copy(xdg, xdg);
		string_list_append(list, xdg);
	}
	ಯದಿ (user && *user) {
		normalize_path_copy(user, user);
		string_list_append(list, user);
	}
	ಮುಕ್ತ(xdg);
	ಮುಕ್ತ(user);
}

ಸಂರಚನಾ git_var {
	ಧ್ರುವ ವರ್ಣ *name;
	ವರ್ಣ *(*ಪಠ)(ಪೂರ್ಣಾಙ್ಕ);
	ಶೂನ್ಯ (*multiread)(ಸಂರಚನಾ string_list *);
};
ಸ್ಥಿರ ಸಂರಚನಾ git_var git_vars[] = {
	{
		.name = "GIT_COMMITTER_IDENT",
		.ಪಠ = committer,
	},
	{
		.name = "GIT_COMMITTER_NAME",
		.ಪಠ = committer_name,
	},
	{
		.name = "GIT_COMMITTER_EMAIL",
		.ಪಠ = committer_email,
	},
	{
		.name = "GIT_COMMITTER_DATE",
		.ಪಠ = committer_date,
	},
	{
		.name = "GIT_AUTHOR_IDENT",
		.ಪಠ = author,
	},
	{
		.name = "GIT_AUTHOR_NAME",
		.ಪಠ = author_name,
	},
	{
		.name = "GIT_AUTHOR_EMAIL",
		.ಪಠ = author_email,
	},
	{
		.name = "GIT_AUTHOR_DATE",
		.ಪಠ = author_date,
	},
	{
		.name = "GIT_EDITOR",
		.ಪಠ = editor,
	},
	{
		.name = "GIT_SEQUENCE_EDITOR",
		.ಪಠ = sequence_editor,
	},
	{
		.name = "GIT_PAGER",
		.ಪಠ = pager,
	},
	{
		.name = "GIT_DEFAULT_BRANCH",
		.ಪಠ = default_branch,
	},
	{
		.name = "GIT_SHELL_PATH",
		.ಪಠ = shell_path,
	},
	{
		.name = "GIT_ATTR_SYSTEM",
		.ಪಠ = git_attr_val_system,
	},
	{
		.name = "GIT_ATTR_GLOBAL",
		.ಪಠ = git_attr_val_global,
	},
	{
		.name = "GIT_CONFIG_SYSTEM",
		.ಪಠ = git_config_val_system,
	},
	{
		.name = "GIT_CONFIG_GLOBAL",
		.multiread = git_config_val_global,
	},
	{
		.name = "",
		.ಪಠ = ರಿಕ್ತ,
	},
};

ಸ್ಥಿರ ಶೂನ್ಯ list_vars(ಪೂರ್ಣಾಙ್ಕ nul_term)
{
	ಸಂರಚನಾ git_var *ptr;
	ವರ್ಣ delim = nul_term ? '\n' : '=';
	ವರ್ಣ term = nul_term ? '\0' : '\n';

	ಪ್ರತಿ (ptr = git_vars; ptr->ಪಠ || ptr->multiread; ptr++) {
		ಯದಿ (ptr->ಪಠ) {
			ವರ್ಣ *val = ptr->ಪಠ(0);

			ಯದಿ (val) {
				printf("%s%c%s%c", ptr->name, delim, val, term);
				ಮುಕ್ತ(val);
			}
		} ಅನ್ಯಥಾ {
			ಸಂರಚನಾ string_list list = STRING_LIST_INIT_DUP;

			ptr->multiread(&list);
			ಪ್ರತಿ (size_t i = 0; i < list.nr; i++)
				printf("%s%c%s%c", ptr->name, delim,
				       list.ವಸ್ತುಗಳು[i].string, term);
			string_list_clear(&list, 0);
		}
	}
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ git_var *get_git_var(ಧ್ರುವ ವರ್ಣ *var)
{
	ಸಂರಚನಾ git_var *ptr;
	ಪ್ರತಿ (ptr = git_vars; ptr->ಪಠ || ptr->multiread; ptr++) {
		ಯದಿ (strcmp(var, ptr->name) == 0) {
			ಪ್ರತ್ಯಾಗಮ ptr;
		}
	}
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
		       ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಪೂರ್ಣಾಙ್ಕ *nul_term = cb;
	ವರ್ಣ delim = *nul_term ? '\n' : '=';
	ವರ್ಣ term = *nul_term ? '\0' : '\n';

	ಯದಿ (ಮೌಲ್ಯ)
		printf("%s%c%s%c", var, delim, ಮೌಲ್ಯ, term);
	ಅನ್ಯಥಾ
		printf("%s%c", var, term);
	ಪ್ರತ್ಯಾಗಮ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಪೂರ್ಣಾಙ್ಕ cmd_var(ಪೂರ್ಣಾಙ್ಕ argc,
	    ಧ್ರುವ ವರ್ಣ **argv,
	    ಧ್ರುವ ವರ್ಣ *prefix,
	    ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ list = 0;
	ಪೂರ್ಣಾಙ್ಕ nul_term = 0;
	ವರ್ಣ delim;
	ವರ್ಣ term;
	ಸಂರಚನಾ option options[] = {
		OPT_BOOL('l', ರಿಕ್ತ, &list,
			 N_("list all variables")),
		OPT_BOOL('z', ರಿಕ್ತ, &nul_term,
			 N_("terminate entries with NUL")),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options,
			     var_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	ಯದಿ (list) {
		ಯದಿ (argc)
			usage_with_options(var_usage, options);
		repo_config(the_repository, show_config, &nul_term);
		list_vars(nul_term);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!argc)
		usage_with_options(var_usage, options);

	repo_config(the_repository, git_default_config, ರಿಕ್ತ);

	delim = nul_term ? '\n' : '=';
	term = nul_term ? '\0' : '\n';

	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < argc; i++) {
		ಧ್ರುವ ಸಂರಚನಾ git_var *git_var = get_git_var(argv[i]);

		ಯದಿ (!git_var)
			usage_with_options(var_usage, options);

		ಯದಿ (git_var->ಪಠ) {
			ವರ್ಣ *val = git_var->ಪಠ(IDENT_STRICT);

			ಯದಿ (!val) {
				ಯದಿ (argc == 1)
					ಪ್ರತ್ಯಾಗಮ 1;
				ಅನುವರ್ತ;
			}
			ಯದಿ (argc == 1)
				printf("%s%c", val, term);
			ಅನ್ಯಥಾ
				printf("%s%c%s%c", git_var->name, delim,
				       val, term);
			ಮುಕ್ತ(val);
		} ಅನ್ಯಥಾ {
			ಸಂರಚನಾ string_list list = STRING_LIST_INIT_DUP;

			git_var->multiread(&list);
			ಯದಿ (argc == 1 && !list.nr) {
				string_list_clear(&list, 0);
				ಪ್ರತ್ಯಾಗಮ 1;
			}
			ಪ್ರತಿ (size_t j = 0; j < list.nr; j++) {
				ಯದಿ (argc == 1)
					printf("%s%c", list.ವಸ್ತುಗಳು[j].string,
					       term);
				ಅನ್ಯಥಾ
					printf("%s%c%s%c", git_var->name, delim,
					       list.ವಸ್ತುಗಳು[j].string, term);
			}
			string_list_clear(&list, 0);
		}
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}
