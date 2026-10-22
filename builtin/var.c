/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Eric Biederman, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE

#अन्तर्भाव "builtin.h"

#अन्तर्भाव "attr.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "editor.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "ident.h"
#अन्तर्भाव "pager.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "string-list.h"

स्थिर ध्रुव वर्ण * ध्रुव var_usage[] = {
	N_("git var [-z] -l"),
	N_("git var [-z] <variable>..."),
	रिक्त
};

गणना ident_part {
	IDENT_NAME,
	IDENT_MAIL,
	IDENT_DATE,
};

स्थिर वर्ण *ident_part(ध्रुव वर्ण *ident, गणना ident_part part)
{
	संरचना ident_split split;

	यदि (!ident)
		प्रत्यागम रिक्त;
	यदि (split_ident_ओळ(&split, ident, strlen(ident)))
		प्रत्यागम रिक्त;

	विकल्प (part) {
	पक्ष IDENT_NAME:
		यदि (!split.name_begin || !split.name_end)
			BUG("split_ident_line() gave NULL names???");
		प्रत्यागम xmemdupz(split.name_begin,
				split.name_end - split.name_begin);
	पक्ष IDENT_MAIL:
		यदि (!split.mail_begin || !split.mail_end)
			BUG("split_ident_line() gave NULL mail???");
		प्रत्यागम xmemdupz(split.mail_begin,
				split.mail_end - split.mail_begin);
	पक्ष IDENT_DATE:
		यदि (!split.date_begin || !split.tz_end)
			BUG("split_ident_line() gave NULL date/tz???");
		प्रत्यागम xmemdupz(split.date_begin,
				split.tz_end - split.date_begin);
	स्वभाव:
		BUG("unknown ident_part %d", part);
	}
}

स्थिर वर्ण *committer(पूर्णाङ्क ident_flag)
{
	प्रत्यागम xstrdup_or_null(git_committer_info(ident_flag));
}

स्थिर वर्ण *committer_name(पूर्णाङ्क ident_flag)
{
	प्रत्यागम ident_part(git_committer_info(ident_flag), IDENT_NAME);
}

स्थिर वर्ण *committer_email(पूर्णाङ्क ident_flag)
{
	प्रत्यागम ident_part(git_committer_info(ident_flag), IDENT_MAIL);
}

स्थिर वर्ण *committer_date(पूर्णाङ्क ident_flag)
{
	प्रत्यागम ident_part(git_committer_info(ident_flag), IDENT_DATE);
}

स्थिर वर्ण *author(पूर्णाङ्क ident_flag)
{
	प्रत्यागम xstrdup_or_null(git_author_info(ident_flag));
}

स्थिर वर्ण *author_name(पूर्णाङ्क ident_flag)
{
	प्रत्यागम ident_part(git_author_info(ident_flag), IDENT_NAME);
}

स्थिर वर्ण *author_email(पूर्णाङ्क ident_flag)
{
	प्रत्यागम ident_part(git_author_info(ident_flag), IDENT_MAIL);
}

स्थिर वर्ण *author_date(पूर्णाङ्क ident_flag)
{
	प्रत्यागम ident_part(git_author_info(ident_flag), IDENT_DATE);
}

स्थिर वर्ण *editor(पूर्णाङ्क ident_flag UNUSED)
{
	प्रत्यागम xstrdup_or_null(git_editor());
}

स्थिर वर्ण *sequence_editor(पूर्णाङ्क ident_flag UNUSED)
{
	प्रत्यागम xstrdup_or_null(git_sequence_editor());
}

स्थिर वर्ण *pager(पूर्णाङ्क ident_flag UNUSED)
{
	ध्रुव वर्ण *pgm = git_pager(the_repository, 1);

	यदि (!pgm)
		pgm = "cat";
	प्रत्यागम xstrdup(pgm);
}

स्थिर वर्ण *default_branch(पूर्णाङ्क ident_flag UNUSED)
{
	प्रत्यागम repo_default_branch_name(the_repository, 1);
}

स्थिर वर्ण *shell_path(पूर्णाङ्क ident_flag UNUSED)
{
	प्रत्यागम git_shell_path();
}

स्थिर वर्ण *git_attr_val_system(पूर्णाङ्क ident_flag UNUSED)
{
	यदि (git_attr_system_is_enabled()) {
		वर्ण *file = xstrdup(git_attr_system_file());
		normalize_path_copy(file, file);
		प्रत्यागम file;
	}
	प्रत्यागम रिक्त;
}

स्थिर वर्ण *git_attr_val_global(पूर्णाङ्क ident_flag UNUSED)
{
	वर्ण *file = xstrdup_or_null(git_attr_global_file());
	यदि (file) {
		normalize_path_copy(file, file);
		प्रत्यागम file;
	}
	प्रत्यागम रिक्त;
}

स्थिर वर्ण *git_config_val_system(पूर्णाङ्क ident_flag UNUSED)
{
	यदि (git_config_system()) {
		वर्ण *file = git_system_config();
		normalize_path_copy(file, file);
		प्रत्यागम file;
	}
	प्रत्यागम रिक्त;
}

स्थिर शून्य git_config_val_global(संरचना string_list *list)
{
	वर्ण *user, *xdg;

	git_global_config_paths(&user, &xdg);
	यदि (xdg && *xdg) {
		normalize_path_copy(xdg, xdg);
		string_list_append(list, xdg);
	}
	यदि (user && *user) {
		normalize_path_copy(user, user);
		string_list_append(list, user);
	}
	मुक्त(xdg);
	मुक्त(user);
}

संरचना git_var {
	ध्रुव वर्ण *name;
	वर्ण *(*पठ)(पूर्णाङ्क);
	शून्य (*multiread)(संरचना string_list *);
};
स्थिर संरचना git_var git_vars[] = {
	{
		.name = "GIT_COMMITTER_IDENT",
		.पठ = committer,
	},
	{
		.name = "GIT_COMMITTER_NAME",
		.पठ = committer_name,
	},
	{
		.name = "GIT_COMMITTER_EMAIL",
		.पठ = committer_email,
	},
	{
		.name = "GIT_COMMITTER_DATE",
		.पठ = committer_date,
	},
	{
		.name = "GIT_AUTHOR_IDENT",
		.पठ = author,
	},
	{
		.name = "GIT_AUTHOR_NAME",
		.पठ = author_name,
	},
	{
		.name = "GIT_AUTHOR_EMAIL",
		.पठ = author_email,
	},
	{
		.name = "GIT_AUTHOR_DATE",
		.पठ = author_date,
	},
	{
		.name = "GIT_EDITOR",
		.पठ = editor,
	},
	{
		.name = "GIT_SEQUENCE_EDITOR",
		.पठ = sequence_editor,
	},
	{
		.name = "GIT_PAGER",
		.पठ = pager,
	},
	{
		.name = "GIT_DEFAULT_BRANCH",
		.पठ = default_branch,
	},
	{
		.name = "GIT_SHELL_PATH",
		.पठ = shell_path,
	},
	{
		.name = "GIT_ATTR_SYSTEM",
		.पठ = git_attr_val_system,
	},
	{
		.name = "GIT_ATTR_GLOBAL",
		.पठ = git_attr_val_global,
	},
	{
		.name = "GIT_CONFIG_SYSTEM",
		.पठ = git_config_val_system,
	},
	{
		.name = "GIT_CONFIG_GLOBAL",
		.multiread = git_config_val_global,
	},
	{
		.name = "",
		.पठ = रिक्त,
	},
};

स्थिर शून्य list_vars(पूर्णाङ्क nul_term)
{
	संरचना git_var *ptr;
	वर्ण delim = nul_term ? '\n' : '=';
	वर्ण term = nul_term ? '\0' : '\n';

	प्रति (ptr = git_vars; ptr->पठ || ptr->multiread; ptr++) {
		यदि (ptr->पठ) {
			वर्ण *val = ptr->पठ(0);

			यदि (val) {
				printf("%s%c%s%c", ptr->name, delim, val, term);
				मुक्त(val);
			}
		} अन्यथा {
			संरचना string_list list = STRING_LIST_INIT_DUP;

			ptr->multiread(&list);
			प्रति (size_t i = 0; i < list.nr; i++)
				printf("%s%c%s%c", ptr->name, delim,
				       list.वस्तुसंच[i].string, term);
			string_list_clear(&list, 0);
		}
	}
}

स्थिर ध्रुव संरचना git_var *get_git_var(ध्रुव वर्ण *var)
{
	संरचना git_var *ptr;
	प्रति (ptr = git_vars; ptr->पठ || ptr->multiread; ptr++) {
		यदि (strcmp(var, ptr->name) == 0) {
			प्रत्यागम ptr;
		}
	}
	प्रत्यागम रिक्त;
}

स्थिर पूर्णाङ्क show_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मूल्य,
		       ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	पूर्णाङ्क *nul_term = cb;
	वर्ण delim = *nul_term ? '\n' : '=';
	वर्ण term = *nul_term ? '\0' : '\n';

	यदि (मूल्य)
		printf("%s%c%s%c", var, delim, मूल्य, term);
	अन्यथा
		printf("%s%c", var, term);
	प्रत्यागम git_default_config(var, मूल्य, ctx, cb);
}

पूर्णाङ्क cmd_var(पूर्णाङ्क argc,
	    ध्रुव वर्ण **argv,
	    ध्रुव वर्ण *prefix,
	    संरचना repository *repo UNUSED)
{
	पूर्णाङ्क list = 0;
	पूर्णाङ्क nul_term = 0;
	वर्ण delim;
	वर्ण term;
	संरचना option options[] = {
		OPT_BOOL('l', रिक्त, &list,
			 N_("list all variables")),
		OPT_BOOL('z', रिक्त, &nul_term,
			 N_("terminate entries with NUL")),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options,
			     var_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	यदि (list) {
		यदि (argc)
			usage_with_options(var_usage, options);
		repo_config(the_repository, show_config, &nul_term);
		list_vars(nul_term);
		प्रत्यागम 0;
	}

	यदि (!argc)
		usage_with_options(var_usage, options);

	repo_config(the_repository, git_default_config, रिक्त);

	delim = nul_term ? '\n' : '=';
	term = nul_term ? '\0' : '\n';

	प्रति (पूर्णाङ्क i = 0; i < argc; i++) {
		ध्रुव संरचना git_var *git_var = get_git_var(argv[i]);

		यदि (!git_var)
			usage_with_options(var_usage, options);

		यदि (git_var->पठ) {
			वर्ण *val = git_var->पठ(IDENT_STRICT);

			यदि (!val) {
				यदि (argc == 1)
					प्रत्यागम 1;
				अनुवर्त;
			}
			यदि (argc == 1)
				printf("%s%c", val, term);
			अन्यथा
				printf("%s%c%s%c", git_var->name, delim,
				       val, term);
			मुक्त(val);
		} अन्यथा {
			संरचना string_list list = STRING_LIST_INIT_DUP;

			git_var->multiread(&list);
			यदि (argc == 1 && !list.nr) {
				string_list_clear(&list, 0);
				प्रत्यागम 1;
			}
			प्रति (size_t j = 0; j < list.nr; j++) {
				यदि (argc == 1)
					printf("%s%c", list.वस्तुसंच[j].string,
					       term);
				अन्यथा
					printf("%s%c%s%c", git_var->name, delim,
					       list.वस्तुसंच[j].string, term);
			}
			string_list_clear(&list, 0);
		}
	}

	प्रत्यागम 0;
}
