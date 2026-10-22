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

स्थिर स्थिरांक अक्षर * स्थिरांक var_usage[] = {
	N_("git var [-z] -l"),
	N_("git var [-z] <variable>..."),
	रिक्त
};

गणना ident_part {
	IDENT_NAME,
	IDENT_MAIL,
	IDENT_DATE,
};

स्थिर अक्षर *ident_part(स्थिरांक अक्षर *ident, गणना ident_part part)
{
	संरचना ident_split split;

	जर (!ident)
		परत रिक्त;
	जर (split_ident_ओळ(&split, ident, म_माप(ident)))
		परत रिक्त;

	निवड (part) {
	स्थिती IDENT_NAME:
		जर (!split.name_begin || !split.name_end)
			BUG("split_ident_line() gave NULL names???");
		परत xmemdupz(split.name_begin,
				split.name_end - split.name_begin);
	स्थिती IDENT_MAIL:
		जर (!split.mail_begin || !split.mail_end)
			BUG("split_ident_line() gave NULL mail???");
		परत xmemdupz(split.mail_begin,
				split.mail_end - split.mail_begin);
	स्थिती IDENT_DATE:
		जर (!split.date_begin || !split.tz_end)
			BUG("split_ident_line() gave NULL date/tz???");
		परत xmemdupz(split.date_begin,
				split.tz_end - split.date_begin);
	पूर्वनिर्धारित:
		BUG("unknown ident_part %d", part);
	}
}

स्थिर अक्षर *committer(पूर्णांक ident_flag)
{
	परत xstrdup_or_null(git_committer_info(ident_flag));
}

स्थिर अक्षर *committer_name(पूर्णांक ident_flag)
{
	परत ident_part(git_committer_info(ident_flag), IDENT_NAME);
}

स्थिर अक्षर *committer_email(पूर्णांक ident_flag)
{
	परत ident_part(git_committer_info(ident_flag), IDENT_MAIL);
}

स्थिर अक्षर *committer_date(पूर्णांक ident_flag)
{
	परत ident_part(git_committer_info(ident_flag), IDENT_DATE);
}

स्थिर अक्षर *author(पूर्णांक ident_flag)
{
	परत xstrdup_or_null(git_author_info(ident_flag));
}

स्थिर अक्षर *author_name(पूर्णांक ident_flag)
{
	परत ident_part(git_author_info(ident_flag), IDENT_NAME);
}

स्थिर अक्षर *author_email(पूर्णांक ident_flag)
{
	परत ident_part(git_author_info(ident_flag), IDENT_MAIL);
}

स्थिर अक्षर *author_date(पूर्णांक ident_flag)
{
	परत ident_part(git_author_info(ident_flag), IDENT_DATE);
}

स्थिर अक्षर *editor(पूर्णांक ident_flag UNUSED)
{
	परत xstrdup_or_null(git_editor());
}

स्थिर अक्षर *sequence_editor(पूर्णांक ident_flag UNUSED)
{
	परत xstrdup_or_null(git_sequence_editor());
}

स्थिर अक्षर *pager(पूर्णांक ident_flag UNUSED)
{
	स्थिरांक अक्षर *pgm = git_pager(the_repository, 1);

	जर (!pgm)
		pgm = "cat";
	परत xstrdup(pgm);
}

स्थिर अक्षर *default_branch(पूर्णांक ident_flag UNUSED)
{
	परत repo_default_branch_name(the_repository, 1);
}

स्थिर अक्षर *shell_path(पूर्णांक ident_flag UNUSED)
{
	परत git_shell_path();
}

स्थिर अक्षर *git_attr_val_system(पूर्णांक ident_flag UNUSED)
{
	जर (git_attr_system_is_enabled()) {
		अक्षर *file = xstrdup(git_attr_system_file());
		normalize_path_copy(file, file);
		परत file;
	}
	परत रिक्त;
}

स्थिर अक्षर *git_attr_val_global(पूर्णांक ident_flag UNUSED)
{
	अक्षर *file = xstrdup_or_null(git_attr_global_file());
	जर (file) {
		normalize_path_copy(file, file);
		परत file;
	}
	परत रिक्त;
}

स्थिर अक्षर *git_config_val_system(पूर्णांक ident_flag UNUSED)
{
	जर (git_config_system()) {
		अक्षर *file = git_system_config();
		normalize_path_copy(file, file);
		परत file;
	}
	परत रिक्त;
}

स्थिर शून्य git_config_val_global(संरचना string_list *list)
{
	अक्षर *user, *xdg;

	git_global_config_paths(&user, &xdg);
	जर (xdg && *xdg) {
		normalize_path_copy(xdg, xdg);
		string_list_append(list, xdg);
	}
	जर (user && *user) {
		normalize_path_copy(user, user);
		string_list_append(list, user);
	}
	मुक्त(xdg);
	मुक्त(user);
}

संरचना git_var {
	स्थिरांक अक्षर *name;
	अक्षर *(*पठ)(पूर्णांक);
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

स्थिर शून्य list_vars(पूर्णांक nul_term)
{
	संरचना git_var *ptr;
	अक्षर delim = nul_term ? '\n' : '=';
	अक्षर term = nul_term ? '\0' : '\n';

	साठी (ptr = git_vars; ptr->पठ || ptr->multiread; ptr++) {
		जर (ptr->पठ) {
			अक्षर *val = ptr->पठ(0);

			जर (val) {
				म_लिखो("%s%c%s%c", ptr->name, delim, val, term);
				मुक्त(val);
			}
		} अन्यथा {
			संरचना string_list list = STRING_LIST_INIT_DUP;

			ptr->multiread(&list);
			साठी (माप_प्रकार i = 0; i < list.nr; i++)
				म_लिखो("%s%c%s%c", ptr->name, delim,
				       list.वस्तुसंच[i].string, term);
			string_list_clear(&list, 0);
		}
	}
}

स्थिर स्थिरांक संरचना git_var *get_git_var(स्थिरांक अक्षर *var)
{
	संरचना git_var *ptr;
	साठी (ptr = git_vars; ptr->पठ || ptr->multiread; ptr++) {
		जर (म_भेद(var, ptr->name) == 0) {
			परत ptr;
		}
	}
	परत रिक्त;
}

स्थिर पूर्णांक show_config(स्थिरांक अक्षर *var, स्थिरांक अक्षर *मूल्य,
		       स्थिरांक संरचना config_context *ctx, शून्य *cb)
{
	पूर्णांक *nul_term = cb;
	अक्षर delim = *nul_term ? '\n' : '=';
	अक्षर term = *nul_term ? '\0' : '\n';

	जर (मूल्य)
		म_लिखो("%s%c%s%c", var, delim, मूल्य, term);
	अन्यथा
		म_लिखो("%s%c", var, term);
	परत git_default_config(var, मूल्य, ctx, cb);
}

पूर्णांक cmd_var(पूर्णांक argc,
	    स्थिरांक अक्षर **argv,
	    स्थिरांक अक्षर *prefix,
	    संरचना repository *repo UNUSED)
{
	पूर्णांक list = 0;
	पूर्णांक nul_term = 0;
	अक्षर delim;
	अक्षर term;
	संरचना option options[] = {
		OPT_BOOL('l', रिक्त, &list,
			 N_("list all variables")),
		OPT_BOOL('z', रिक्त, &nul_term,
			 N_("terminate entries with NUL")),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options,
			     var_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	जर (list) {
		जर (argc)
			usage_with_options(var_usage, options);
		repo_config(the_repository, show_config, &nul_term);
		list_vars(nul_term);
		परत 0;
	}

	जर (!argc)
		usage_with_options(var_usage, options);

	repo_config(the_repository, git_default_config, रिक्त);

	delim = nul_term ? '\n' : '=';
	term = nul_term ? '\0' : '\n';

	साठी (पूर्णांक i = 0; i < argc; i++) {
		स्थिरांक संरचना git_var *git_var = get_git_var(argv[i]);

		जर (!git_var)
			usage_with_options(var_usage, options);

		जर (git_var->पठ) {
			अक्षर *val = git_var->पठ(IDENT_STRICT);

			जर (!val) {
				जर (argc == 1)
					परत 1;
				सुरूठेवा;
			}
			जर (argc == 1)
				म_लिखो("%s%c", val, term);
			अन्यथा
				म_लिखो("%s%c%s%c", git_var->name, delim,
				       val, term);
			मुक्त(val);
		} अन्यथा {
			संरचना string_list list = STRING_LIST_INIT_DUP;

			git_var->multiread(&list);
			जर (argc == 1 && !list.nr) {
				string_list_clear(&list, 0);
				परत 1;
			}
			साठी (माप_प्रकार j = 0; j < list.nr; j++) {
				जर (argc == 1)
					म_लिखो("%s%c", list.वस्तुसंच[j].string,
					       term);
				अन्यथा
					म_लिखो("%s%c%s%c", git_var->name, delim,
					       list.वस्तुसंच[j].string, term);
			}
			string_list_clear(&list, 0);
		}
	}

	परत 0;
}
