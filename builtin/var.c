/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Eric Biederman, 2005
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "attr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "editor.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ident.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pager.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ var_usage[] = {
	N_("git var [-z] -l"),
	N_("git var [-z] <variable>..."),
	ਨਿਰਾ
};

ਗਿਣਤੀ ident_part {
	IDENT_NAME,
	IDENT_MAIL,
	IDENT_DATE,
};

ਸਥਿਰ ਅੱਖਰ *ident_part(ਅਚਲ ਅੱਖਰ *ident, ਗਿਣਤੀ ident_part part)
{
	ਬਣਤਰ ident_ਵੰਡੋ ਵੰਡੋ;

	ਜੇ (!ident)
		ਵਾਪਸ ਨਿਰਾ;
	ਜੇ (ਵੰਡੋ_ident_ਸਤਰ(&ਵੰਡੋ, ident, ਲੜੀ_ਲੰਬਾਈ(ident)))
		ਵਾਪਸ ਨਿਰਾ;

	ਚੋਣ (part) {
	ਹਾਲਤ IDENT_NAME:
		ਜੇ (!ਵੰਡੋ.name_begin || !ਵੰਡੋ.name_ਸਿਰਾ)
			BUG("split_ident_line() gave NULL names???");
		ਵਾਪਸ xmemdupz(ਵੰਡੋ.name_begin,
				ਵੰਡੋ.name_ਸਿਰਾ - ਵੰਡੋ.name_begin);
	ਹਾਲਤ IDENT_MAIL:
		ਜੇ (!ਵੰਡੋ.mail_begin || !ਵੰਡੋ.mail_ਸਿਰਾ)
			BUG("split_ident_line() gave NULL mail???");
		ਵਾਪਸ xmemdupz(ਵੰਡੋ.mail_begin,
				ਵੰਡੋ.mail_ਸਿਰਾ - ਵੰਡੋ.mail_begin);
	ਹਾਲਤ IDENT_DATE:
		ਜੇ (!ਵੰਡੋ.date_begin || !ਵੰਡੋ.tz_ਸਿਰਾ)
			BUG("split_ident_line() gave NULL date/tz???");
		ਵਾਪਸ xmemdupz(ਵੰਡੋ.date_begin,
				ਵੰਡੋ.tz_ਸਿਰਾ - ਵੰਡੋ.date_begin);
	ਮੂਲ:
		BUG("unknown ident_part %d", part);
	}
}

ਸਥਿਰ ਅੱਖਰ *committer(ਪੂਰਨ ident_flag)
{
	ਵਾਪਸ xstrdup_or_null(git_committer_info(ident_flag));
}

ਸਥਿਰ ਅੱਖਰ *committer_name(ਪੂਰਨ ident_flag)
{
	ਵਾਪਸ ident_part(git_committer_info(ident_flag), IDENT_NAME);
}

ਸਥਿਰ ਅੱਖਰ *committer_email(ਪੂਰਨ ident_flag)
{
	ਵਾਪਸ ident_part(git_committer_info(ident_flag), IDENT_MAIL);
}

ਸਥਿਰ ਅੱਖਰ *committer_date(ਪੂਰਨ ident_flag)
{
	ਵਾਪਸ ident_part(git_committer_info(ident_flag), IDENT_DATE);
}

ਸਥਿਰ ਅੱਖਰ *author(ਪੂਰਨ ident_flag)
{
	ਵਾਪਸ xstrdup_or_null(git_author_info(ident_flag));
}

ਸਥਿਰ ਅੱਖਰ *author_name(ਪੂਰਨ ident_flag)
{
	ਵਾਪਸ ident_part(git_author_info(ident_flag), IDENT_NAME);
}

ਸਥਿਰ ਅੱਖਰ *author_email(ਪੂਰਨ ident_flag)
{
	ਵਾਪਸ ident_part(git_author_info(ident_flag), IDENT_MAIL);
}

ਸਥਿਰ ਅੱਖਰ *author_date(ਪੂਰਨ ident_flag)
{
	ਵਾਪਸ ident_part(git_author_info(ident_flag), IDENT_DATE);
}

ਸਥਿਰ ਅੱਖਰ *editor(ਪੂਰਨ ident_flag ਅਣਵਰਤਿਆ)
{
	ਵਾਪਸ xstrdup_or_null(git_editor());
}

ਸਥਿਰ ਅੱਖਰ *sequence_editor(ਪੂਰਨ ident_flag ਅਣਵਰਤਿਆ)
{
	ਵਾਪਸ xstrdup_or_null(git_sequence_editor());
}

ਸਥਿਰ ਅੱਖਰ *pager(ਪੂਰਨ ident_flag ਅਣਵਰਤਿਆ)
{
	ਅਚਲ ਅੱਖਰ *pgm = git_pager(the_repository, 1);

	ਜੇ (!pgm)
		pgm = "cat";
	ਵਾਪਸ xstrdup(pgm);
}

ਸਥਿਰ ਅੱਖਰ *default_branch(ਪੂਰਨ ident_flag ਅਣਵਰਤਿਆ)
{
	ਵਾਪਸ repo_default_branch_name(the_repository, 1);
}

ਸਥਿਰ ਅੱਖਰ *shell_ਰਸਤਾ(ਪੂਰਨ ident_flag ਅਣਵਰਤਿਆ)
{
	ਵਾਪਸ git_shell_ਰਸਤਾ();
}

ਸਥਿਰ ਅੱਖਰ *git_attr_val_system(ਪੂਰਨ ident_flag ਅਣਵਰਤਿਆ)
{
	ਜੇ (git_attr_system_is_enabled()) {
		ਅੱਖਰ *file = xstrdup(git_attr_system_file());
		normalize_ਰਸਤਾ_copy(file, file);
		ਵਾਪਸ file;
	}
	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਅੱਖਰ *git_attr_val_global(ਪੂਰਨ ident_flag ਅਣਵਰਤਿਆ)
{
	ਅੱਖਰ *file = xstrdup_or_null(git_attr_global_file());
	ਜੇ (file) {
		normalize_ਰਸਤਾ_copy(file, file);
		ਵਾਪਸ file;
	}
	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਅੱਖਰ *git_config_val_system(ਪੂਰਨ ident_flag ਅਣਵਰਤਿਆ)
{
	ਜੇ (git_config_system()) {
		ਅੱਖਰ *file = git_system_config();
		normalize_ਰਸਤਾ_copy(file, file);
		ਵਾਪਸ file;
	}
	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਖਾਲੀ git_config_val_global(ਬਣਤਰ string_list *list)
{
	ਅੱਖਰ *user, *xdg;

	git_global_config_paths(&user, &xdg);
	ਜੇ (xdg && *xdg) {
		normalize_ਰਸਤਾ_copy(xdg, xdg);
		string_list_append(list, xdg);
	}
	ਜੇ (user && *user) {
		normalize_ਰਸਤਾ_copy(user, user);
		string_list_append(list, user);
	}
	ਮੁਕਤ(xdg);
	ਮੁਕਤ(user);
}

ਬਣਤਰ git_var {
	ਅਚਲ ਅੱਖਰ *name;
	ਅੱਖਰ *(*ਪਠ)(ਪੂਰਨ);
	ਖਾਲੀ (*multiread)(ਬਣਤਰ string_list *);
};
ਸਥਿਰ ਬਣਤਰ git_var git_vars[] = {
	{
		.name = "GIT_COMMITTER_IDENT",
		.ਪਠ = committer,
	},
	{
		.name = "GIT_COMMITTER_NAME",
		.ਪਠ = committer_name,
	},
	{
		.name = "GIT_COMMITTER_EMAIL",
		.ਪਠ = committer_email,
	},
	{
		.name = "GIT_COMMITTER_DATE",
		.ਪਠ = committer_date,
	},
	{
		.name = "GIT_AUTHOR_IDENT",
		.ਪਠ = author,
	},
	{
		.name = "GIT_AUTHOR_NAME",
		.ਪਠ = author_name,
	},
	{
		.name = "GIT_AUTHOR_EMAIL",
		.ਪਠ = author_email,
	},
	{
		.name = "GIT_AUTHOR_DATE",
		.ਪਠ = author_date,
	},
	{
		.name = "GIT_EDITOR",
		.ਪਠ = editor,
	},
	{
		.name = "GIT_SEQUENCE_EDITOR",
		.ਪਠ = sequence_editor,
	},
	{
		.name = "GIT_PAGER",
		.ਪਠ = pager,
	},
	{
		.name = "GIT_DEFAULT_BRANCH",
		.ਪਠ = default_branch,
	},
	{
		.name = "GIT_SHELL_PATH",
		.ਪਠ = shell_ਰਸਤਾ,
	},
	{
		.name = "GIT_ATTR_SYSTEM",
		.ਪਠ = git_attr_val_system,
	},
	{
		.name = "GIT_ATTR_GLOBAL",
		.ਪਠ = git_attr_val_global,
	},
	{
		.name = "GIT_CONFIG_SYSTEM",
		.ਪਠ = git_config_val_system,
	},
	{
		.name = "GIT_CONFIG_GLOBAL",
		.multiread = git_config_val_global,
	},
	{
		.name = "",
		.ਪਠ = ਨਿਰਾ,
	},
};

ਸਥਿਰ ਖਾਲੀ list_vars(ਪੂਰਨ nul_ਪਦ)
{
	ਬਣਤਰ git_var *ptr;
	ਅੱਖਰ delim = nul_ਪਦ ? '\n' : '=';
	ਅੱਖਰ ਪਦ = nul_ਪਦ ? '\0' : '\n';

	ਲਈ (ptr = git_vars; ptr->ਪਠ || ptr->multiread; ptr++) {
		ਜੇ (ptr->ਪਠ) {
			ਅੱਖਰ *val = ptr->ਪਠ(0);

			ਜੇ (val) {
				ਲਿਖੋ("%s%c%s%c", ptr->name, delim, val, ਪਦ);
				ਮੁਕਤ(val);
			}
		} ਨਹੀਂਤਾਂ {
			ਬਣਤਰ string_list list = STRING_LIST_INIT_DUP;

			ptr->multiread(&list);
			ਲਈ (ਮਾਪ_t i = 0; i < list.nr; i++)
				ਲਿਖੋ("%s%c%s%c", ptr->name, delim,
				       list.ਚੀਜ਼ਾਂ[i].string, ਪਦ);
			string_list_clear(&list, 0);
		}
	}
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ git_var *get_git_var(ਅਚਲ ਅੱਖਰ *var)
{
	ਬਣਤਰ git_var *ptr;
	ਲਈ (ptr = git_vars; ptr->ਪਠ || ptr->multiread; ptr++) {
		ਜੇ (strcmp(var, ptr->name) == 0) {
			ਵਾਪਸ ptr;
		}
	}
	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਪੂਰਨ show_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
		       ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਪੂਰਨ *nul_ਪਦ = cb;
	ਅੱਖਰ delim = *nul_ਪਦ ? '\n' : '=';
	ਅੱਖਰ ਪਦ = *nul_ਪਦ ? '\0' : '\n';

	ਜੇ (ਮੁੱਲ)
		ਲਿਖੋ("%s%c%s%c", var, delim, ਮੁੱਲ, ਪਦ);
	ਨਹੀਂਤਾਂ
		ਲਿਖੋ("%s%c", var, ਪਦ);
	ਵਾਪਸ git_default_config(var, ਮੁੱਲ, ctx, cb);
}

ਪੂਰਨ cmd_var(ਪੂਰਨ argc,
	    ਅਚਲ ਅੱਖਰ **argv,
	    ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	    ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ list = 0;
	ਪੂਰਨ nul_ਪਦ = 0;
	ਅੱਖਰ delim;
	ਅੱਖਰ ਪਦ;
	ਬਣਤਰ option options[] = {
		OPT_BOOL('l', ਨਿਰਾ, &list,
			 N_("list all variables")),
		OPT_BOOL('z', ਨਿਰਾ, &nul_ਪਦ,
			 N_("terminate entries with NUL")),
		OPT_ਸਿਰਾ(),
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     var_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	ਜੇ (list) {
		ਜੇ (argc)
			usage_with_options(var_usage, options);
		repo_config(the_repository, show_config, &nul_ਪਦ);
		list_vars(nul_ਪਦ);
		ਵਾਪਸ 0;
	}

	ਜੇ (!argc)
		usage_with_options(var_usage, options);

	repo_config(the_repository, git_default_config, ਨਿਰਾ);

	delim = nul_ਪਦ ? '\n' : '=';
	ਪਦ = nul_ਪਦ ? '\0' : '\n';

	ਲਈ (ਪੂਰਨ i = 0; i < argc; i++) {
		ਅਚਲ ਬਣਤਰ git_var *git_var = get_git_var(argv[i]);

		ਜੇ (!git_var)
			usage_with_options(var_usage, options);

		ਜੇ (git_var->ਪਠ) {
			ਅੱਖਰ *val = git_var->ਪਠ(IDENT_STRICT);

			ਜੇ (!val) {
				ਜੇ (argc == 1)
					ਵਾਪਸ 1;
				ਜਾਰੀ;
			}
			ਜੇ (argc == 1)
				ਲਿਖੋ("%s%c", val, ਪਦ);
			ਨਹੀਂਤਾਂ
				ਲਿਖੋ("%s%c%s%c", git_var->name, delim,
				       val, ਪਦ);
			ਮੁਕਤ(val);
		} ਨਹੀਂਤਾਂ {
			ਬਣਤਰ string_list list = STRING_LIST_INIT_DUP;

			git_var->multiread(&list);
			ਜੇ (argc == 1 && !list.nr) {
				string_list_clear(&list, 0);
				ਵਾਪਸ 1;
			}
			ਲਈ (ਮਾਪ_t j = 0; j < list.nr; j++) {
				ਜੇ (argc == 1)
					ਲਿਖੋ("%s%c", list.ਚੀਜ਼ਾਂ[j].string,
					       ਪਦ);
				ਨਹੀਂਤਾਂ
					ਲਿਖੋ("%s%c%s%c", git_var->name, delim,
					       list.ਚੀਜ਼ਾਂ[j].string, ਪਦ);
			}
			string_list_clear(&list, 0);
		}
	}

	ਵਾਪਸ 0;
}
