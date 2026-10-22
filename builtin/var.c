/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Eric Biederman, 2005
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE

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

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ var_usage[] = {
	N_("git var [-z] -l"),
	N_("git var [-z] <variable>..."),
	ಖಾಲಿ
};

ಎಣಿಕೆ ident_part {
	IDENT_NAME,
	IDENT_MAIL,
	IDENT_DATE,
};

ಸ್ಥಿರ ಅಕ್ಷರ *ident_part(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ident, ಎಣಿಕೆ ident_part part)
{
	ರಚನೆ ident_ವಿಭಜಿಸು ವಿಭಜಿಸು;

	ಆದರೆ (!ident)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	ಆದರೆ (ವಿಭಜಿಸು_ident_ಸಾಲು(&ವಿಭಜಿಸು, ident, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ident)))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಆಯ್ಕೆ (part) {
	ಸ್ಥಿತಿ IDENT_NAME:
		ಆದರೆ (!ವಿಭಜಿಸು.name_begin || !ವಿಭಜಿಸು.name_ಕೊನೆ)
			BUG("split_ident_line() gave NULL names???");
		ಹಿಂತಿರುಗಿ xmemdupz(ವಿಭಜಿಸು.name_begin,
				ವಿಭಜಿಸು.name_ಕೊನೆ - ವಿಭಜಿಸು.name_begin);
	ಸ್ಥಿತಿ IDENT_MAIL:
		ಆದರೆ (!ವಿಭಜಿಸು.mail_begin || !ವಿಭಜಿಸು.mail_ಕೊನೆ)
			BUG("split_ident_line() gave NULL mail???");
		ಹಿಂತಿರುಗಿ xmemdupz(ವಿಭಜಿಸು.mail_begin,
				ವಿಭಜಿಸು.mail_ಕೊನೆ - ವಿಭಜಿಸು.mail_begin);
	ಸ್ಥಿತಿ IDENT_DATE:
		ಆದರೆ (!ವಿಭಜಿಸು.date_begin || !ವಿಭಜಿಸು.tz_ಕೊನೆ)
			BUG("split_ident_line() gave NULL date/tz???");
		ಹಿಂತಿರುಗಿ xmemdupz(ವಿಭಜಿಸು.date_begin,
				ವಿಭಜಿಸು.tz_ಕೊನೆ - ವಿಭಜಿಸು.date_begin);
	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("unknown ident_part %d", part);
	}
}

ಸ್ಥಿರ ಅಕ್ಷರ *committer(ಪೂರ್ಣಾಂಕ ident_flag)
{
	ಹಿಂತಿರುಗಿ xstrdup_or_null(git_committer_info(ident_flag));
}

ಸ್ಥಿರ ಅಕ್ಷರ *committer_name(ಪೂರ್ಣಾಂಕ ident_flag)
{
	ಹಿಂತಿರುಗಿ ident_part(git_committer_info(ident_flag), IDENT_NAME);
}

ಸ್ಥಿರ ಅಕ್ಷರ *committer_email(ಪೂರ್ಣಾಂಕ ident_flag)
{
	ಹಿಂತಿರುಗಿ ident_part(git_committer_info(ident_flag), IDENT_MAIL);
}

ಸ್ಥಿರ ಅಕ್ಷರ *committer_date(ಪೂರ್ಣಾಂಕ ident_flag)
{
	ಹಿಂತಿರುಗಿ ident_part(git_committer_info(ident_flag), IDENT_DATE);
}

ಸ್ಥಿರ ಅಕ್ಷರ *author(ಪೂರ್ಣಾಂಕ ident_flag)
{
	ಹಿಂತಿರುಗಿ xstrdup_or_null(git_author_info(ident_flag));
}

ಸ್ಥಿರ ಅಕ್ಷರ *author_name(ಪೂರ್ಣಾಂಕ ident_flag)
{
	ಹಿಂತಿರುಗಿ ident_part(git_author_info(ident_flag), IDENT_NAME);
}

ಸ್ಥಿರ ಅಕ್ಷರ *author_email(ಪೂರ್ಣಾಂಕ ident_flag)
{
	ಹಿಂತಿರುಗಿ ident_part(git_author_info(ident_flag), IDENT_MAIL);
}

ಸ್ಥಿರ ಅಕ್ಷರ *author_date(ಪೂರ್ಣಾಂಕ ident_flag)
{
	ಹಿಂತಿರುಗಿ ident_part(git_author_info(ident_flag), IDENT_DATE);
}

ಸ್ಥಿರ ಅಕ್ಷರ *editor(ಪೂರ್ಣಾಂಕ ident_flag ಬಳಸದ)
{
	ಹಿಂತಿರುಗಿ xstrdup_or_null(git_editor());
}

ಸ್ಥಿರ ಅಕ್ಷರ *sequence_editor(ಪೂರ್ಣಾಂಕ ident_flag ಬಳಸದ)
{
	ಹಿಂತಿರುಗಿ xstrdup_or_null(git_sequence_editor());
}

ಸ್ಥಿರ ಅಕ್ಷರ *pager(ಪೂರ್ಣಾಂಕ ident_flag ಬಳಸದ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pgm = git_pager(the_repository, 1);

	ಆದರೆ (!pgm)
		pgm = "cat";
	ಹಿಂತಿರುಗಿ xstrdup(pgm);
}

ಸ್ಥಿರ ಅಕ್ಷರ *default_branch(ಪೂರ್ಣಾಂಕ ident_flag ಬಳಸದ)
{
	ಹಿಂತಿರುಗಿ repo_default_branch_name(the_repository, 1);
}

ಸ್ಥಿರ ಅಕ್ಷರ *shell_ಮಾರ್ಗ(ಪೂರ್ಣಾಂಕ ident_flag ಬಳಸದ)
{
	ಹಿಂತಿರುಗಿ git_shell_ಮಾರ್ಗ();
}

ಸ್ಥಿರ ಅಕ್ಷರ *git_attr_val_system(ಪೂರ್ಣಾಂಕ ident_flag ಬಳಸದ)
{
	ಆದರೆ (git_attr_system_is_enabled()) {
		ಅಕ್ಷರ *file = xstrdup(git_attr_system_file());
		normalize_ಮಾರ್ಗ_copy(file, file);
		ಹಿಂತಿರುಗಿ file;
	}
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಅಕ್ಷರ *git_attr_val_global(ಪೂರ್ಣಾಂಕ ident_flag ಬಳಸದ)
{
	ಅಕ್ಷರ *file = xstrdup_or_null(git_attr_global_file());
	ಆದರೆ (file) {
		normalize_ಮಾರ್ಗ_copy(file, file);
		ಹಿಂತಿರುಗಿ file;
	}
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಅಕ್ಷರ *git_config_val_system(ಪೂರ್ಣಾಂಕ ident_flag ಬಳಸದ)
{
	ಆದರೆ (git_config_system()) {
		ಅಕ್ಷರ *file = git_system_config();
		normalize_ಮಾರ್ಗ_copy(file, file);
		ಹಿಂತಿರುಗಿ file;
	}
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಶೂನ್ಯ git_config_val_global(ರಚನೆ string_list *list)
{
	ಅಕ್ಷರ *user, *xdg;

	git_global_config_paths(&user, &xdg);
	ಆದರೆ (xdg && *xdg) {
		normalize_ಮಾರ್ಗ_copy(xdg, xdg);
		string_list_append(list, xdg);
	}
	ಆದರೆ (user && *user) {
		normalize_ಮಾರ್ಗ_copy(user, user);
		string_list_append(list, user);
	}
	ಬಿಡುಗಡೆ(xdg);
	ಬಿಡುಗಡೆ(user);
}

ರಚನೆ git_var {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
	ಅಕ್ಷರ *(*ಪಠ)(ಪೂರ್ಣಾಂಕ);
	ಶೂನ್ಯ (*multiread)(ರಚನೆ string_list *);
};
ಸ್ಥಿರ ರಚನೆ git_var git_vars[] = {
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
		.ಪಠ = shell_ಮಾರ್ಗ,
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
		.ಪಠ = ಖಾಲಿ,
	},
};

ಸ್ಥಿರ ಶೂನ್ಯ list_vars(ಪೂರ್ಣಾಂಕ nul_ಪದ)
{
	ರಚನೆ git_var *ptr;
	ಅಕ್ಷರ delim = nul_ಪದ ? '\n' : '=';
	ಅಕ್ಷರ ಪದ = nul_ಪದ ? '\0' : '\n';

	ಗಾಗಿ (ptr = git_vars; ptr->ಪಠ || ptr->multiread; ptr++) {
		ಆದರೆ (ptr->ಪಠ) {
			ಅಕ್ಷರ *val = ptr->ಪಠ(0);

			ಆದರೆ (val) {
				ಬರೆ("%s%c%s%c", ptr->name, delim, val, ಪದ);
				ಬಿಡುಗಡೆ(val);
			}
		} ಇಲ್ಲದಿದ್ದರೆ {
			ರಚನೆ string_list list = STRING_LIST_INIT_DUP;

			ptr->multiread(&list);
			ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < list.nr; i++)
				ಬರೆ("%s%c%s%c", ptr->name, delim,
				       list.ವಸ್ತುಗಳು[i].string, ಪದ);
			string_list_clear(&list, 0);
		}
	}
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ git_var *get_git_var(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var)
{
	ರಚನೆ git_var *ptr;
	ಗಾಗಿ (ptr = git_vars; ptr->ಪಠ || ptr->multiread; ptr++) {
		ಆದರೆ (strcmp(var, ptr->name) == 0) {
			ಹಿಂತಿರುಗಿ ptr;
		}
	}
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
		       ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಪೂರ್ಣಾಂಕ *nul_ಪದ = cb;
	ಅಕ್ಷರ delim = *nul_ಪದ ? '\n' : '=';
	ಅಕ್ಷರ ಪದ = *nul_ಪದ ? '\0' : '\n';

	ಆದರೆ (ಮೌಲ್ಯ)
		ಬರೆ("%s%c%s%c", var, delim, ಮೌಲ್ಯ, ಪದ);
	ಇಲ್ಲದಿದ್ದರೆ
		ಬರೆ("%s%c", var, ಪದ);
	ಹಿಂತಿರುಗಿ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಪೂರ್ಣಾಂಕ cmd_var(ಪೂರ್ಣಾಂಕ argc,
	    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	    ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ list = 0;
	ಪೂರ್ಣಾಂಕ nul_ಪದ = 0;
	ಅಕ್ಷರ delim;
	ಅಕ್ಷರ ಪದ;
	ರಚನೆ option options[] = {
		OPT_BOOL('l', ಖಾಲಿ, &list,
			 N_("list all variables")),
		OPT_BOOL('z', ಖಾಲಿ, &nul_ಪದ,
			 N_("terminate entries with NUL")),
		OPT_ಕೊನೆ(),
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     var_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	ಆದರೆ (list) {
		ಆದರೆ (argc)
			usage_with_options(var_usage, options);
		repo_config(the_repository, show_config, &nul_ಪದ);
		list_vars(nul_ಪದ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!argc)
		usage_with_options(var_usage, options);

	repo_config(the_repository, git_default_config, ಖಾಲಿ);

	delim = nul_ಪದ ? '\n' : '=';
	ಪದ = nul_ಪದ ? '\0' : '\n';

	ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < argc; i++) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ git_var *git_var = get_git_var(argv[i]);

		ಆದರೆ (!git_var)
			usage_with_options(var_usage, options);

		ಆದರೆ (git_var->ಪಠ) {
			ಅಕ್ಷರ *val = git_var->ಪಠ(IDENT_STRICT);

			ಆದರೆ (!val) {
				ಆದರೆ (argc == 1)
					ಹಿಂತಿರುಗಿ 1;
				ಮುಂದುವರಿ;
			}
			ಆದರೆ (argc == 1)
				ಬರೆ("%s%c", val, ಪದ);
			ಇಲ್ಲದಿದ್ದರೆ
				ಬರೆ("%s%c%s%c", git_var->name, delim,
				       val, ಪದ);
			ಬಿಡುಗಡೆ(val);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ರಚನೆ string_list list = STRING_LIST_INIT_DUP;

			git_var->multiread(&list);
			ಆದರೆ (argc == 1 && !list.nr) {
				string_list_clear(&list, 0);
				ಹಿಂತಿರುಗಿ 1;
			}
			ಗಾಗಿ (ಗಾತ್ರ_t j = 0; j < list.nr; j++) {
				ಆದರೆ (argc == 1)
					ಬರೆ("%s%c", list.ವಸ್ತುಗಳು[j].string,
					       ಪದ);
				ಇಲ್ಲದಿದ್ದರೆ
					ಬರೆ("%s%c%s%c", git_var->name, delim,
					       list.ವಸ್ತುಗಳು[j].string, ಪದ);
			}
			string_list_clear(&list, 0);
		}
	}

	ಹಿಂತಿರುಗಿ 0;
}
