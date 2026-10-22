/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Eric Biederman, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE

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

ایستا ثابت نویسه * ثابت var_usage[] = {
	N_("git var [-z] -l"),
	N_("git var [-z] <variable>..."),
	پوچ
};

شمارش ident_part {
	IDENT_NAME,
	IDENT_MAIL,
	IDENT_DATE,
};

ایستا نویسه *ident_part(ثابت نویسه *ident, شمارش ident_part part)
{
	ساختار ident_split split;

	اگر (!ident)
		بازگشت پوچ;
	اگر (split_ident_سطر(&split, ident, strlen(ident)))
		بازگشت پوچ;

	انتخاب (part) {
	حالت IDENT_NAME:
		اگر (!split.name_begin || !split.name_end)
			BUG("split_ident_line() gave NULL names???");
		بازگشت xmemdupz(split.name_begin,
				split.name_end - split.name_begin);
	حالت IDENT_MAIL:
		اگر (!split.mail_begin || !split.mail_end)
			BUG("split_ident_line() gave NULL mail???");
		بازگشت xmemdupz(split.mail_begin,
				split.mail_end - split.mail_begin);
	حالت IDENT_DATE:
		اگر (!split.date_begin || !split.tz_end)
			BUG("split_ident_line() gave NULL date/tz???");
		بازگشت xmemdupz(split.date_begin,
				split.tz_end - split.date_begin);
	پیشفرض:
		BUG("unknown ident_part %d", part);
	}
}

ایستا نویسه *committer(صحیح ident_flag)
{
	بازگشت xstrdup_or_null(git_committer_info(ident_flag));
}

ایستا نویسه *committer_name(صحیح ident_flag)
{
	بازگشت ident_part(git_committer_info(ident_flag), IDENT_NAME);
}

ایستا نویسه *committer_email(صحیح ident_flag)
{
	بازگشت ident_part(git_committer_info(ident_flag), IDENT_MAIL);
}

ایستا نویسه *committer_date(صحیح ident_flag)
{
	بازگشت ident_part(git_committer_info(ident_flag), IDENT_DATE);
}

ایستا نویسه *author(صحیح ident_flag)
{
	بازگشت xstrdup_or_null(git_author_info(ident_flag));
}

ایستا نویسه *author_name(صحیح ident_flag)
{
	بازگشت ident_part(git_author_info(ident_flag), IDENT_NAME);
}

ایستا نویسه *author_email(صحیح ident_flag)
{
	بازگشت ident_part(git_author_info(ident_flag), IDENT_MAIL);
}

ایستا نویسه *author_date(صحیح ident_flag)
{
	بازگشت ident_part(git_author_info(ident_flag), IDENT_DATE);
}

ایستا نویسه *editor(صحیح ident_flag UNUSED)
{
	بازگشت xstrdup_or_null(git_editor());
}

ایستا نویسه *sequence_editor(صحیح ident_flag UNUSED)
{
	بازگشت xstrdup_or_null(git_sequence_editor());
}

ایستا نویسه *pager(صحیح ident_flag UNUSED)
{
	ثابت نویسه *pgm = git_pager(the_repository, 1);

	اگر (!pgm)
		pgm = "cat";
	بازگشت xstrdup(pgm);
}

ایستا نویسه *default_branch(صحیح ident_flag UNUSED)
{
	بازگشت repo_default_branch_name(the_repository, 1);
}

ایستا نویسه *shell_path(صحیح ident_flag UNUSED)
{
	بازگشت git_shell_path();
}

ایستا نویسه *git_attr_val_system(صحیح ident_flag UNUSED)
{
	اگر (git_attr_system_is_enabled()) {
		نویسه *file = xstrdup(git_attr_system_file());
		normalize_path_copy(file, file);
		بازگشت file;
	}
	بازگشت پوچ;
}

ایستا نویسه *git_attr_val_global(صحیح ident_flag UNUSED)
{
	نویسه *file = xstrdup_or_null(git_attr_global_file());
	اگر (file) {
		normalize_path_copy(file, file);
		بازگشت file;
	}
	بازگشت پوچ;
}

ایستا نویسه *git_config_val_system(صحیح ident_flag UNUSED)
{
	اگر (git_config_system()) {
		نویسه *file = git_system_config();
		normalize_path_copy(file, file);
		بازگشت file;
	}
	بازگشت پوچ;
}

ایستا تهی git_config_val_global(ساختار string_list *list)
{
	نویسه *user, *xdg;

	git_global_config_paths(&user, &xdg);
	اگر (xdg && *xdg) {
		normalize_path_copy(xdg, xdg);
		string_list_append(list, xdg);
	}
	اگر (user && *user) {
		normalize_path_copy(user, user);
		string_list_append(list, user);
	}
	آزاد(xdg);
	آزاد(user);
}

ساختار git_var {
	ثابت نویسه *name;
	نویسه *(*اقرأ)(صحیح);
	تهی (*multiread)(ساختار string_list *);
};
ایستا ساختار git_var git_vars[] = {
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
		.اقرأ = پوچ,
	},
};

ایستا تهی list_vars(صحیح nul_term)
{
	ساختار git_var *ptr;
	نویسه delim = nul_term ? '\n' : '=';
	نویسه term = nul_term ? '\0' : '\n';

	برای (ptr = git_vars; ptr->اقرأ || ptr->multiread; ptr++) {
		اگر (ptr->اقرأ) {
			نویسه *val = ptr->اقرأ(0);

			اگر (val) {
				printf("%s%c%s%c", ptr->name, delim, val, term);
				آزاد(val);
			}
		} وگرنه {
			ساختار string_list list = STRING_LIST_INIT_DUP;

			ptr->multiread(&list);
			برای (size_t i = 0; i < list.nr; i++)
				printf("%s%c%s%c", ptr->name, delim,
				       list.اقلام[i].string, term);
			string_list_clear(&list, 0);
		}
	}
}

ایستا ثابت ساختار git_var *get_git_var(ثابت نویسه *var)
{
	ساختار git_var *ptr;
	برای (ptr = git_vars; ptr->اقرأ || ptr->multiread; ptr++) {
		اگر (strcmp(var, ptr->name) == 0) {
			بازگشت ptr;
		}
	}
	بازگشت پوچ;
}

ایستا صحیح show_config(ثابت نویسه *var, ثابت نویسه *مقدار,
		       ثابت ساختار config_context *ctx, تهی *cb)
{
	صحیح *nul_term = cb;
	نویسه delim = *nul_term ? '\n' : '=';
	نویسه term = *nul_term ? '\0' : '\n';

	اگر (مقدار)
		printf("%s%c%s%c", var, delim, مقدار, term);
	وگرنه
		printf("%s%c", var, term);
	بازگشت git_default_config(var, مقدار, ctx, cb);
}

صحیح cmd_var(صحیح argc,
	    ثابت نویسه **argv,
	    ثابت نویسه *prefix,
	    ساختار repository *repo UNUSED)
{
	صحیح list = 0;
	صحیح nul_term = 0;
	نویسه delim;
	نویسه term;
	ساختار option options[] = {
		OPT_BOOL('l', پوچ, &list,
			 N_("list all variables")),
		OPT_BOOL('z', پوچ, &nul_term,
			 N_("terminate entries with NUL")),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options,
			     var_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	اگر (list) {
		اگر (argc)
			usage_with_options(var_usage, options);
		repo_config(the_repository, show_config, &nul_term);
		list_vars(nul_term);
		بازگشت 0;
	}

	اگر (!argc)
		usage_with_options(var_usage, options);

	repo_config(the_repository, git_default_config, پوچ);

	delim = nul_term ? '\n' : '=';
	term = nul_term ? '\0' : '\n';

	برای (صحیح i = 0; i < argc; i++) {
		ثابت ساختار git_var *git_var = get_git_var(argv[i]);

		اگر (!git_var)
			usage_with_options(var_usage, options);

		اگر (git_var->اقرأ) {
			نویسه *val = git_var->اقرأ(IDENT_STRICT);

			اگر (!val) {
				اگر (argc == 1)
					بازگشت 1;
				ادامه;
			}
			اگر (argc == 1)
				printf("%s%c", val, term);
			وگرنه
				printf("%s%c%s%c", git_var->name, delim,
				       val, term);
			آزاد(val);
		} وگرنه {
			ساختار string_list list = STRING_LIST_INIT_DUP;

			git_var->multiread(&list);
			اگر (argc == 1 && !list.nr) {
				string_list_clear(&list, 0);
				بازگشت 1;
			}
			برای (size_t j = 0; j < list.nr; j++) {
				اگر (argc == 1)
					printf("%s%c", list.اقلام[j].string,
					       term);
				وگرنه
					printf("%s%c%s%c", git_var->name, delim,
					       list.اقلام[j].string, term);
			}
			string_list_clear(&list, 0);
		}
	}

	بازگشت 0;
}
