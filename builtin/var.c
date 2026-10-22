/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Eric Biederman, 2005
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE

#समावेश "builtin.h"

#समावेश "attr.h"
#समावेश "config.h"
#समावेश "editor.h"
#समावेश "environment.h"
#समावेश "ident.h"
#समावेश "pager.h"
#समावेश "parse-options.h"
#समावेश "path.h"
#समावेश "refs.h"
#समावेश "run-command.h"
#समावेश "strbuf.h"
#समावेश "string-list.h"

अटल स्थिर अक्षर * स्थिर var_usage[] = {
	N_("git var [-z] -l"),
	N_("git var [-z] <variable>..."),
	शून्य
};

क्रमागत ident_part {
	IDENT_NAME,
	IDENT_MAIL,
	IDENT_DATE,
};

अटल अक्षर *ident_part(स्थिर अक्षर *ident, क्रमागत ident_part part)
{
	काष्ठा ident_split split;

	अगर (!ident)
		वापस शून्य;
	अगर (split_ident_पंक्ति(&split, ident, म_माप(ident)))
		वापस शून्य;

	चयन (part) {
	हाल IDENT_NAME:
		अगर (!split.name_begin || !split.name_end)
			BUG("split_ident_line() gave NULL names???");
		वापस xmemdupz(split.name_begin,
				split.name_end - split.name_begin);
	हाल IDENT_MAIL:
		अगर (!split.mail_begin || !split.mail_end)
			BUG("split_ident_line() gave NULL mail???");
		वापस xmemdupz(split.mail_begin,
				split.mail_end - split.mail_begin);
	हाल IDENT_DATE:
		अगर (!split.date_begin || !split.tz_end)
			BUG("split_ident_line() gave NULL date/tz???");
		वापस xmemdupz(split.date_begin,
				split.tz_end - split.date_begin);
	शेष:
		BUG("unknown ident_part %d", part);
	}
}

अटल अक्षर *committer(पूर्णांक ident_flag)
{
	वापस xstrdup_or_null(git_committer_info(ident_flag));
}

अटल अक्षर *committer_name(पूर्णांक ident_flag)
{
	वापस ident_part(git_committer_info(ident_flag), IDENT_NAME);
}

अटल अक्षर *committer_email(पूर्णांक ident_flag)
{
	वापस ident_part(git_committer_info(ident_flag), IDENT_MAIL);
}

अटल अक्षर *committer_date(पूर्णांक ident_flag)
{
	वापस ident_part(git_committer_info(ident_flag), IDENT_DATE);
}

अटल अक्षर *author(पूर्णांक ident_flag)
{
	वापस xstrdup_or_null(git_author_info(ident_flag));
}

अटल अक्षर *author_name(पूर्णांक ident_flag)
{
	वापस ident_part(git_author_info(ident_flag), IDENT_NAME);
}

अटल अक्षर *author_email(पूर्णांक ident_flag)
{
	वापस ident_part(git_author_info(ident_flag), IDENT_MAIL);
}

अटल अक्षर *author_date(पूर्णांक ident_flag)
{
	वापस ident_part(git_author_info(ident_flag), IDENT_DATE);
}

अटल अक्षर *editor(पूर्णांक ident_flag UNUSED)
{
	वापस xstrdup_or_null(git_editor());
}

अटल अक्षर *sequence_editor(पूर्णांक ident_flag UNUSED)
{
	वापस xstrdup_or_null(git_sequence_editor());
}

अटल अक्षर *pager(पूर्णांक ident_flag UNUSED)
{
	स्थिर अक्षर *pgm = git_pager(the_repository, 1);

	अगर (!pgm)
		pgm = "cat";
	वापस xstrdup(pgm);
}

अटल अक्षर *default_branch(पूर्णांक ident_flag UNUSED)
{
	वापस repo_default_branch_name(the_repository, 1);
}

अटल अक्षर *shell_path(पूर्णांक ident_flag UNUSED)
{
	वापस git_shell_path();
}

अटल अक्षर *git_attr_val_system(पूर्णांक ident_flag UNUSED)
{
	अगर (git_attr_system_is_enabled()) {
		अक्षर *file = xstrdup(git_attr_system_file());
		normalize_path_copy(file, file);
		वापस file;
	}
	वापस शून्य;
}

अटल अक्षर *git_attr_val_global(पूर्णांक ident_flag UNUSED)
{
	अक्षर *file = xstrdup_or_null(git_attr_global_file());
	अगर (file) {
		normalize_path_copy(file, file);
		वापस file;
	}
	वापस शून्य;
}

अटल अक्षर *git_config_val_system(पूर्णांक ident_flag UNUSED)
{
	अगर (git_config_system()) {
		अक्षर *file = git_system_config();
		normalize_path_copy(file, file);
		वापस file;
	}
	वापस शून्य;
}

अटल व्योम git_config_val_global(काष्ठा string_list *list)
{
	अक्षर *user, *xdg;

	git_global_config_paths(&user, &xdg);
	अगर (xdg && *xdg) {
		normalize_path_copy(xdg, xdg);
		string_list_append(list, xdg);
	}
	अगर (user && *user) {
		normalize_path_copy(user, user);
		string_list_append(list, user);
	}
	मुक्त(xdg);
	मुक्त(user);
}

काष्ठा git_var {
	स्थिर अक्षर *name;
	अक्षर *(*पढ़ो)(पूर्णांक);
	व्योम (*multiread)(काष्ठा string_list *);
};
अटल काष्ठा git_var git_vars[] = {
	{
		.name = "GIT_COMMITTER_IDENT",
		.पढ़ो = committer,
	},
	{
		.name = "GIT_COMMITTER_NAME",
		.पढ़ो = committer_name,
	},
	{
		.name = "GIT_COMMITTER_EMAIL",
		.पढ़ो = committer_email,
	},
	{
		.name = "GIT_COMMITTER_DATE",
		.पढ़ो = committer_date,
	},
	{
		.name = "GIT_AUTHOR_IDENT",
		.पढ़ो = author,
	},
	{
		.name = "GIT_AUTHOR_NAME",
		.पढ़ो = author_name,
	},
	{
		.name = "GIT_AUTHOR_EMAIL",
		.पढ़ो = author_email,
	},
	{
		.name = "GIT_AUTHOR_DATE",
		.पढ़ो = author_date,
	},
	{
		.name = "GIT_EDITOR",
		.पढ़ो = editor,
	},
	{
		.name = "GIT_SEQUENCE_EDITOR",
		.पढ़ो = sequence_editor,
	},
	{
		.name = "GIT_PAGER",
		.पढ़ो = pager,
	},
	{
		.name = "GIT_DEFAULT_BRANCH",
		.पढ़ो = default_branch,
	},
	{
		.name = "GIT_SHELL_PATH",
		.पढ़ो = shell_path,
	},
	{
		.name = "GIT_ATTR_SYSTEM",
		.पढ़ो = git_attr_val_system,
	},
	{
		.name = "GIT_ATTR_GLOBAL",
		.पढ़ो = git_attr_val_global,
	},
	{
		.name = "GIT_CONFIG_SYSTEM",
		.पढ़ो = git_config_val_system,
	},
	{
		.name = "GIT_CONFIG_GLOBAL",
		.multiread = git_config_val_global,
	},
	{
		.name = "",
		.पढ़ो = शून्य,
	},
};

अटल व्योम list_vars(पूर्णांक nul_term)
{
	काष्ठा git_var *ptr;
	अक्षर delim = nul_term ? '\n' : '=';
	अक्षर term = nul_term ? '\0' : '\n';

	क्रम (ptr = git_vars; ptr->पढ़ो || ptr->multiread; ptr++) {
		अगर (ptr->पढ़ो) {
			अक्षर *val = ptr->पढ़ो(0);

			अगर (val) {
				म_लिखो("%s%c%s%c", ptr->name, delim, val, term);
				मुक्त(val);
			}
		} अन्यथा {
			काष्ठा string_list list = STRING_LIST_INIT_DUP;

			ptr->multiread(&list);
			क्रम (माप_प्रकार i = 0; i < list.nr; i++)
				म_लिखो("%s%c%s%c", ptr->name, delim,
				       list.वस्तुएँ[i].string, term);
			string_list_clear(&list, 0);
		}
	}
}

अटल स्थिर काष्ठा git_var *get_git_var(स्थिर अक्षर *var)
{
	काष्ठा git_var *ptr;
	क्रम (ptr = git_vars; ptr->पढ़ो || ptr->multiread; ptr++) {
		अगर (म_भेद(var, ptr->name) == 0) {
			वापस ptr;
		}
	}
	वापस शून्य;
}

अटल पूर्णांक show_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
		       स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	पूर्णांक *nul_term = cb;
	अक्षर delim = *nul_term ? '\n' : '=';
	अक्षर term = *nul_term ? '\0' : '\n';

	अगर (मूल्य)
		म_लिखो("%s%c%s%c", var, delim, मूल्य, term);
	अन्यथा
		म_लिखो("%s%c", var, term);
	वापस git_default_config(var, मूल्य, ctx, cb);
}

पूर्णांक cmd_var(पूर्णांक argc,
	    स्थिर अक्षर **argv,
	    स्थिर अक्षर *prefix,
	    काष्ठा repository *repo UNUSED)
{
	पूर्णांक list = 0;
	पूर्णांक nul_term = 0;
	अक्षर delim;
	अक्षर term;
	काष्ठा option options[] = {
		OPT_BOOL('l', शून्य, &list,
			 N_("list all variables")),
		OPT_BOOL('z', शून्य, &nul_term,
			 N_("terminate entries with NUL")),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options,
			     var_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	अगर (list) {
		अगर (argc)
			usage_with_options(var_usage, options);
		repo_config(the_repository, show_config, &nul_term);
		list_vars(nul_term);
		वापस 0;
	}

	अगर (!argc)
		usage_with_options(var_usage, options);

	repo_config(the_repository, git_default_config, शून्य);

	delim = nul_term ? '\n' : '=';
	term = nul_term ? '\0' : '\n';

	क्रम (पूर्णांक i = 0; i < argc; i++) {
		स्थिर काष्ठा git_var *git_var = get_git_var(argv[i]);

		अगर (!git_var)
			usage_with_options(var_usage, options);

		अगर (git_var->पढ़ो) {
			अक्षर *val = git_var->पढ़ो(IDENT_STRICT);

			अगर (!val) {
				अगर (argc == 1)
					वापस 1;
				जारी;
			}
			अगर (argc == 1)
				म_लिखो("%s%c", val, term);
			अन्यथा
				म_लिखो("%s%c%s%c", git_var->name, delim,
				       val, term);
			मुक्त(val);
		} अन्यथा {
			काष्ठा string_list list = STRING_LIST_INIT_DUP;

			git_var->multiread(&list);
			अगर (argc == 1 && !list.nr) {
				string_list_clear(&list, 0);
				वापस 1;
			}
			क्रम (माप_प्रकार j = 0; j < list.nr; j++) {
				अगर (argc == 1)
					म_लिखो("%s%c", list.वस्तुएँ[j].string,
					       term);
				अन्यथा
					म_लिखो("%s%c%s%c", git_var->name, delim,
					       list.वस्तुएँ[j].string, term);
			}
			string_list_clear(&list, 0);
		}
	}

	वापस 0;
}
