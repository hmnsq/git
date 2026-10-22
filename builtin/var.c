/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Eric Biederman, 2005
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE

#شامل "builtin.h"

#شامل "attr.h"
#شامل "config.h"
#شامل "editor.h"
#شامل "environment.h"
#شامل "ident.h"
#شامل "pager.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "refs.h"
#شامل "run-command.h"
#شامل "strbuf.h"
#شامل "string-list.h"

ساکن ثابت حرف * ثابت var_usage[] = {
	N_("git var [-z] -l"),
	N_("git var [-z] <variable>..."),
	تہی
};

شمار ident_part {
	IDENT_NAME,
	IDENT_MAIL,
	IDENT_DATE,
};

ساکن حرف *ident_part(ثابت حرف *ident, شمار ident_part part)
{
	ساخت ident_بانٹو بانٹو;

	اگر (!ident)
		واپس تہی;
	اگر (بانٹو_ident_سطر(&بانٹو, ident, لڑی_لمبائی(ident)))
		واپس تہی;

	انتخاب (part) {
	حالت IDENT_NAME:
		اگر (!بانٹو.name_begin || !بانٹو.name_انتہا)
			BUG("split_ident_line() gave NULL names???");
		واپس xmemdupz(بانٹو.name_begin,
				بانٹو.name_انتہا - بانٹو.name_begin);
	حالت IDENT_MAIL:
		اگر (!بانٹو.mail_begin || !بانٹو.mail_انتہا)
			BUG("split_ident_line() gave NULL mail???");
		واپس xmemdupz(بانٹو.mail_begin,
				بانٹو.mail_انتہا - بانٹو.mail_begin);
	حالت IDENT_DATE:
		اگر (!بانٹو.date_begin || !بانٹو.tz_انتہا)
			BUG("split_ident_line() gave NULL date/tz???");
		واپس xmemdupz(بانٹو.date_begin,
				بانٹو.tz_انتہا - بانٹو.date_begin);
	طےشدہ:
		BUG("unknown ident_part %d", part);
	}
}

ساکن حرف *committer(صحیح ident_flag)
{
	واپس xstrdup_or_null(git_committer_info(ident_flag));
}

ساکن حرف *committer_name(صحیح ident_flag)
{
	واپس ident_part(git_committer_info(ident_flag), IDENT_NAME);
}

ساکن حرف *committer_email(صحیح ident_flag)
{
	واپس ident_part(git_committer_info(ident_flag), IDENT_MAIL);
}

ساکن حرف *committer_date(صحیح ident_flag)
{
	واپس ident_part(git_committer_info(ident_flag), IDENT_DATE);
}

ساکن حرف *author(صحیح ident_flag)
{
	واپس xstrdup_or_null(git_author_info(ident_flag));
}

ساکن حرف *author_name(صحیح ident_flag)
{
	واپس ident_part(git_author_info(ident_flag), IDENT_NAME);
}

ساکن حرف *author_email(صحیح ident_flag)
{
	واپس ident_part(git_author_info(ident_flag), IDENT_MAIL);
}

ساکن حرف *author_date(صحیح ident_flag)
{
	واپس ident_part(git_author_info(ident_flag), IDENT_DATE);
}

ساکن حرف *editor(صحیح ident_flag غیر_مستعمل)
{
	واپس xstrdup_or_null(git_editor());
}

ساکن حرف *sequence_editor(صحیح ident_flag غیر_مستعمل)
{
	واپس xstrdup_or_null(git_sequence_editor());
}

ساکن حرف *pager(صحیح ident_flag غیر_مستعمل)
{
	ثابت حرف *pgm = git_pager(the_repository, 1);

	اگر (!pgm)
		pgm = "cat";
	واپس xstrdup(pgm);
}

ساکن حرف *default_branch(صحیح ident_flag غیر_مستعمل)
{
	واپس repo_default_branch_name(the_repository, 1);
}

ساکن حرف *shell_راستہ(صحیح ident_flag غیر_مستعمل)
{
	واپس git_shell_راستہ();
}

ساکن حرف *git_attr_val_system(صحیح ident_flag غیر_مستعمل)
{
	اگر (git_attr_system_is_enabled()) {
		حرف *file = xstrdup(git_attr_system_file());
		normalize_راستہ_copy(file, file);
		واپس file;
	}
	واپس تہی;
}

ساکن حرف *git_attr_val_global(صحیح ident_flag غیر_مستعمل)
{
	حرف *file = xstrdup_or_null(git_attr_global_file());
	اگر (file) {
		normalize_راستہ_copy(file, file);
		واپس file;
	}
	واپس تہی;
}

ساکن حرف *git_config_val_system(صحیح ident_flag غیر_مستعمل)
{
	اگر (git_config_system()) {
		حرف *file = git_system_config();
		normalize_راستہ_copy(file, file);
		واپس file;
	}
	واپس تہی;
}

ساکن خالی git_config_val_global(ساخت string_list *list)
{
	حرف *user, *xdg;

	git_global_config_paths(&user, &xdg);
	اگر (xdg && *xdg) {
		normalize_راستہ_copy(xdg, xdg);
		string_list_append(list, xdg);
	}
	اگر (user && *user) {
		normalize_راستہ_copy(user, user);
		string_list_append(list, user);
	}
	آزاد(xdg);
	آزاد(user);
}

ساخت git_var {
	ثابت حرف *name;
	حرف *(*اقرأ)(صحیح);
	خالی (*multiread)(ساخت string_list *);
};
ساکن ساخت git_var git_vars[] = {
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
		.اقرأ = shell_راستہ,
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
		.اقرأ = تہی,
	},
};

ساکن خالی list_vars(صحیح nul_رکن)
{
	ساخت git_var *ptr;
	حرف delim = nul_رکن ? '\n' : '=';
	حرف رکن = nul_رکن ? '\0' : '\n';

	برائے (ptr = git_vars; ptr->اقرأ || ptr->multiread; ptr++) {
		اگر (ptr->اقرأ) {
			حرف *val = ptr->اقرأ(0);

			اگر (val) {
				لکھو("%s%c%s%c", ptr->name, delim, val, رکن);
				آزاد(val);
			}
		} ورنہ {
			ساخت string_list list = STRING_LIST_INIT_DUP;

			ptr->multiread(&list);
			برائے (سائز_t i = 0; i < list.nr; i++)
				لکھو("%s%c%s%c", ptr->name, delim,
				       list.اشیاء[i].string, رکن);
			string_list_clear(&list, 0);
		}
	}
}

ساکن ثابت ساخت git_var *get_git_var(ثابت حرف *var)
{
	ساخت git_var *ptr;
	برائے (ptr = git_vars; ptr->اقرأ || ptr->multiread; ptr++) {
		اگر (strcmp(var, ptr->name) == 0) {
			واپس ptr;
		}
	}
	واپس تہی;
}

ساکن صحیح show_config(ثابت حرف *var, ثابت حرف *قدر,
		       ثابت ساخت config_context *ctx, خالی *cb)
{
	صحیح *nul_رکن = cb;
	حرف delim = *nul_رکن ? '\n' : '=';
	حرف رکن = *nul_رکن ? '\0' : '\n';

	اگر (قدر)
		لکھو("%s%c%s%c", var, delim, قدر, رکن);
	ورنہ
		لکھو("%s%c", var, رکن);
	واپس git_default_config(var, قدر, ctx, cb);
}

صحیح cmd_var(صحیح argc,
	    ثابت حرف **argv,
	    ثابت حرف *سابقہ,
	    ساخت repository *repo غیر_مستعمل)
{
	صحیح list = 0;
	صحیح nul_رکن = 0;
	حرف delim;
	حرف رکن;
	ساخت option options[] = {
		OPT_BOOL('l', تہی, &list,
			 N_("list all variables")),
		OPT_BOOL('z', تہی, &nul_رکن,
			 N_("terminate entries with NUL")),
		OPT_انتہا(),
	};

	argc = parse_options(argc, argv, سابقہ, options,
			     var_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	اگر (list) {
		اگر (argc)
			usage_with_options(var_usage, options);
		repo_config(the_repository, show_config, &nul_رکن);
		list_vars(nul_رکن);
		واپس 0;
	}

	اگر (!argc)
		usage_with_options(var_usage, options);

	repo_config(the_repository, git_default_config, تہی);

	delim = nul_رکن ? '\n' : '=';
	رکن = nul_رکن ? '\0' : '\n';

	برائے (صحیح i = 0; i < argc; i++) {
		ثابت ساخت git_var *git_var = get_git_var(argv[i]);

		اگر (!git_var)
			usage_with_options(var_usage, options);

		اگر (git_var->اقرأ) {
			حرف *val = git_var->اقرأ(IDENT_STRICT);

			اگر (!val) {
				اگر (argc == 1)
					واپس 1;
				جاری;
			}
			اگر (argc == 1)
				لکھو("%s%c", val, رکن);
			ورنہ
				لکھو("%s%c%s%c", git_var->name, delim,
				       val, رکن);
			آزاد(val);
		} ورنہ {
			ساخت string_list list = STRING_LIST_INIT_DUP;

			git_var->multiread(&list);
			اگر (argc == 1 && !list.nr) {
				string_list_clear(&list, 0);
				واپس 1;
			}
			برائے (سائز_t j = 0; j < list.nr; j++) {
				اگر (argc == 1)
					لکھو("%s%c", list.اشیاء[j].string,
					       رکن);
				ورنہ
					لکھو("%s%c%s%c", git_var->name, delim,
					       list.اشیاء[j].string, رکن);
			}
			string_list_clear(&list, 0);
		}
	}

	واپس 0;
}
