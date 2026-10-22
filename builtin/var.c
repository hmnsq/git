/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Eric Biederman, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE

#ضمن "builtin.h"

#ضمن "attr.h"
#ضمن "config.h"
#ضمن "editor.h"
#ضمن "environment.h"
#ضمن "ident.h"
#ضمن "pager.h"
#ضمن "parse-options.h"
#ضمن "path.h"
#ضمن "refs.h"
#ضمن "run-command.h"
#ضمن "strbuf.h"
#ضمن "string-list.h"

ساكن ثابت حرف * ثابت var_usage[] = {
	N_("git var [-z] -l"),
	N_("git var [-z] <variable>..."),
	عدم
};

تعداد ident_part {
	IDENT_NAME,
	IDENT_MAIL,
	IDENT_DATE,
};

ساكن حرف *ident_part(ثابت حرف *ident, تعداد ident_part part)
{
	بنية ident_split split;

	إذا (!ident)
		أرجع عدم;
	إذا (split_ident_سطر(&split, ident, strlen(ident)))
		أرجع عدم;

	بدل (part) {
	حالة IDENT_NAME:
		إذا (!split.name_begin || !split.name_end)
			BUG("split_ident_line() gave NULL names???");
		أرجع xmemdupz(split.name_begin,
				split.name_end - split.name_begin);
	حالة IDENT_MAIL:
		إذا (!split.mail_begin || !split.mail_end)
			BUG("split_ident_line() gave NULL mail???");
		أرجع xmemdupz(split.mail_begin,
				split.mail_end - split.mail_begin);
	حالة IDENT_DATE:
		إذا (!split.date_begin || !split.tz_end)
			BUG("split_ident_line() gave NULL date/tz???");
		أرجع xmemdupz(split.date_begin,
				split.tz_end - split.date_begin);
	افتراضي:
		BUG("unknown ident_part %d", part);
	}
}

ساكن حرف *committer(صحيح ident_flag)
{
	أرجع xstrdup_or_null(git_committer_info(ident_flag));
}

ساكن حرف *committer_name(صحيح ident_flag)
{
	أرجع ident_part(git_committer_info(ident_flag), IDENT_NAME);
}

ساكن حرف *committer_email(صحيح ident_flag)
{
	أرجع ident_part(git_committer_info(ident_flag), IDENT_MAIL);
}

ساكن حرف *committer_date(صحيح ident_flag)
{
	أرجع ident_part(git_committer_info(ident_flag), IDENT_DATE);
}

ساكن حرف *author(صحيح ident_flag)
{
	أرجع xstrdup_or_null(git_author_info(ident_flag));
}

ساكن حرف *author_name(صحيح ident_flag)
{
	أرجع ident_part(git_author_info(ident_flag), IDENT_NAME);
}

ساكن حرف *author_email(صحيح ident_flag)
{
	أرجع ident_part(git_author_info(ident_flag), IDENT_MAIL);
}

ساكن حرف *author_date(صحيح ident_flag)
{
	أرجع ident_part(git_author_info(ident_flag), IDENT_DATE);
}

ساكن حرف *editor(صحيح ident_flag UNUSED)
{
	أرجع xstrdup_or_null(git_editor());
}

ساكن حرف *sequence_editor(صحيح ident_flag UNUSED)
{
	أرجع xstrdup_or_null(git_sequence_editor());
}

ساكن حرف *pager(صحيح ident_flag UNUSED)
{
	ثابت حرف *pgm = git_pager(the_repository, 1);

	إذا (!pgm)
		pgm = "cat";
	أرجع xstrdup(pgm);
}

ساكن حرف *default_branch(صحيح ident_flag UNUSED)
{
	أرجع repo_default_branch_name(the_repository, 1);
}

ساكن حرف *shell_path(صحيح ident_flag UNUSED)
{
	أرجع git_shell_path();
}

ساكن حرف *git_attr_val_system(صحيح ident_flag UNUSED)
{
	إذا (git_attr_system_is_enabled()) {
		حرف *file = xstrdup(git_attr_system_file());
		normalize_path_copy(file, file);
		أرجع file;
	}
	أرجع عدم;
}

ساكن حرف *git_attr_val_global(صحيح ident_flag UNUSED)
{
	حرف *file = xstrdup_or_null(git_attr_global_file());
	إذا (file) {
		normalize_path_copy(file, file);
		أرجع file;
	}
	أرجع عدم;
}

ساكن حرف *git_config_val_system(صحيح ident_flag UNUSED)
{
	إذا (git_config_system()) {
		حرف *file = git_system_config();
		normalize_path_copy(file, file);
		أرجع file;
	}
	أرجع عدم;
}

ساكن فراغ git_config_val_global(بنية string_list *list)
{
	حرف *user, *xdg;

	git_global_config_paths(&user, &xdg);
	إذا (xdg && *xdg) {
		normalize_path_copy(xdg, xdg);
		string_list_append(list, xdg);
	}
	إذا (user && *user) {
		normalize_path_copy(user, user);
		string_list_append(list, user);
	}
	حرر(xdg);
	حرر(user);
}

بنية git_var {
	ثابت حرف *name;
	حرف *(*اقرأ)(صحيح);
	فراغ (*multiread)(بنية string_list *);
};
ساكن بنية git_var git_vars[] = {
	{
		.name = "GIT_COMMITTER_IDENT",
		.اقرأ = committer,
	},
	{
		.name = "GIT_COMMITTER_NAME",
		.اقرأ = committer_name,
	},
	{
		.name = "GIT_COMMITTER_EMAIL",
		.اقرأ = committer_email,
	},
	{
		.name = "GIT_COMMITTER_DATE",
		.اقرأ = committer_date,
	},
	{
		.name = "GIT_AUTHOR_IDENT",
		.اقرأ = author,
	},
	{
		.name = "GIT_AUTHOR_NAME",
		.اقرأ = author_name,
	},
	{
		.name = "GIT_AUTHOR_EMAIL",
		.اقرأ = author_email,
	},
	{
		.name = "GIT_AUTHOR_DATE",
		.اقرأ = author_date,
	},
	{
		.name = "GIT_EDITOR",
		.اقرأ = editor,
	},
	{
		.name = "GIT_SEQUENCE_EDITOR",
		.اقرأ = sequence_editor,
	},
	{
		.name = "GIT_PAGER",
		.اقرأ = pager,
	},
	{
		.name = "GIT_DEFAULT_BRANCH",
		.اقرأ = default_branch,
	},
	{
		.name = "GIT_SHELL_PATH",
		.اقرأ = shell_path,
	},
	{
		.name = "GIT_ATTR_SYSTEM",
		.اقرأ = git_attr_val_system,
	},
	{
		.name = "GIT_ATTR_GLOBAL",
		.اقرأ = git_attr_val_global,
	},
	{
		.name = "GIT_CONFIG_SYSTEM",
		.اقرأ = git_config_val_system,
	},
	{
		.name = "GIT_CONFIG_GLOBAL",
		.multiread = git_config_val_global,
	},
	{
		.name = "",
		.اقرأ = عدم,
	},
};

ساكن فراغ list_vars(صحيح nul_term)
{
	بنية git_var *ptr;
	حرف delim = nul_term ? '\n' : '=';
	حرف term = nul_term ? '\0' : '\n';

	لكل (ptr = git_vars; ptr->اقرأ || ptr->multiread; ptr++) {
		إذا (ptr->اقرأ) {
			حرف *val = ptr->اقرأ(0);

			إذا (val) {
				printf("%s%c%s%c", ptr->name, delim, val, term);
				حرر(val);
			}
		} وإلا {
			بنية string_list list = STRING_LIST_INIT_DUP;

			ptr->multiread(&list);
			لكل (size_t i = 0; i < list.nr; i++)
				printf("%s%c%s%c", ptr->name, delim,
				       list.اشیاء[i].string, term);
			string_list_clear(&list, 0);
		}
	}
}

ساكن ثابت بنية git_var *get_git_var(ثابت حرف *var)
{
	بنية git_var *ptr;
	لكل (ptr = git_vars; ptr->اقرأ || ptr->multiread; ptr++) {
		إذا (strcmp(var, ptr->name) == 0) {
			أرجع ptr;
		}
	}
	أرجع عدم;
}

ساكن صحيح show_config(ثابت حرف *var, ثابت حرف *قدر,
		       ثابت بنية config_context *ctx, فراغ *cb)
{
	صحيح *nul_term = cb;
	حرف delim = *nul_term ? '\n' : '=';
	حرف term = *nul_term ? '\0' : '\n';

	إذا (قدر)
		printf("%s%c%s%c", var, delim, قدر, term);
	وإلا
		printf("%s%c", var, term);
	أرجع git_default_config(var, قدر, ctx, cb);
}

صحيح cmd_var(صحيح argc,
	    ثابت حرف **argv,
	    ثابت حرف *prefix,
	    بنية repository *repo UNUSED)
{
	صحيح list = 0;
	صحيح nul_term = 0;
	حرف delim;
	حرف term;
	بنية option options[] = {
		OPT_BOOL('l', عدم, &list,
			 N_("list all variables")),
		OPT_BOOL('z', عدم, &nul_term,
			 N_("terminate entries with NUL")),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options,
			     var_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	إذا (list) {
		إذا (argc)
			usage_with_options(var_usage, options);
		repo_config(the_repository, show_config, &nul_term);
		list_vars(nul_term);
		أرجع 0;
	}

	إذا (!argc)
		usage_with_options(var_usage, options);

	repo_config(the_repository, git_default_config, عدم);

	delim = nul_term ? '\n' : '=';
	term = nul_term ? '\0' : '\n';

	لكل (صحيح i = 0; i < argc; i++) {
		ثابت بنية git_var *git_var = get_git_var(argv[i]);

		إذا (!git_var)
			usage_with_options(var_usage, options);

		إذا (git_var->اقرأ) {
			حرف *val = git_var->اقرأ(IDENT_STRICT);

			إذا (!val) {
				إذا (argc == 1)
					أرجع 1;
				استمر;
			}
			إذا (argc == 1)
				printf("%s%c", val, term);
			وإلا
				printf("%s%c%s%c", git_var->name, delim,
				       val, term);
			حرر(val);
		} وإلا {
			بنية string_list list = STRING_LIST_INIT_DUP;

			git_var->multiread(&list);
			إذا (argc == 1 && !list.nr) {
				string_list_clear(&list, 0);
				أرجع 1;
			}
			لكل (size_t j = 0; j < list.nr; j++) {
				إذا (argc == 1)
					printf("%s%c", list.اشیاء[j].string,
					       term);
				وإلا
					printf("%s%c%s%c", git_var->name, delim,
					       list.اشیاء[j].string, term);
			}
			string_list_clear(&list, 0);
		}
	}

	أرجع 0;
}
