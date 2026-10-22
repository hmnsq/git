/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Eric Biederman, 2005
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE

#అన్తర్భావ "builtin.h"

#అన్తర్భావ "attr.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "editor.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "ident.h"
#అన్తర్భావ "pager.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "strbuf.h"
#అన్తర్భావ "string-list.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి var_usage[] = {
	N_("git var [-z] -l"),
	N_("git var [-z] <variable>..."),
	ఖాళీ
};

గణన ident_part {
	IDENT_NAME,
	IDENT_MAIL,
	IDENT_DATE,
};

స్థిర అక్షరం *ident_part(స్థిరరాశి అక్షరం *ident, గణన ident_part part)
{
	నిర్మాణం ident_విభజించు విభజించు;

	ఒకవేళ (!ident)
		తిరిగి ఖాళీ;
	ఒకవేళ (విభజించు_ident_పంక్తి(&విభజించు, ident, అక్షరశ్రేణి_పొడవు(ident)))
		తిరిగి ఖాళీ;

	ఎంపిక (part) {
	స్థితి IDENT_NAME:
		ఒకవేళ (!విభజించు.name_begin || !విభజించు.name_చివర)
			BUG("split_ident_line() gave NULL names???");
		తిరిగి xmemdupz(విభజించు.name_begin,
				విభజించు.name_చివర - విభజించు.name_begin);
	స్థితి IDENT_MAIL:
		ఒకవేళ (!విభజించు.mail_begin || !విభజించు.mail_చివర)
			BUG("split_ident_line() gave NULL mail???");
		తిరిగి xmemdupz(విభజించు.mail_begin,
				విభజించు.mail_చివర - విభజించు.mail_begin);
	స్థితి IDENT_DATE:
		ఒకవేళ (!విభజించు.date_begin || !విభజించు.tz_చివర)
			BUG("split_ident_line() gave NULL date/tz???");
		తిరిగి xmemdupz(విభజించు.date_begin,
				విభజించు.tz_చివర - విభజించు.date_begin);
	అప్రమేయ:
		BUG("unknown ident_part %d", part);
	}
}

స్థిర అక్షరం *committer(పూర్ణాంకం ident_flag)
{
	తిరిగి xstrdup_or_null(git_committer_info(ident_flag));
}

స్థిర అక్షరం *committer_name(పూర్ణాంకం ident_flag)
{
	తిరిగి ident_part(git_committer_info(ident_flag), IDENT_NAME);
}

స్థిర అక్షరం *committer_email(పూర్ణాంకం ident_flag)
{
	తిరిగి ident_part(git_committer_info(ident_flag), IDENT_MAIL);
}

స్థిర అక్షరం *committer_date(పూర్ణాంకం ident_flag)
{
	తిరిగి ident_part(git_committer_info(ident_flag), IDENT_DATE);
}

స్థిర అక్షరం *author(పూర్ణాంకం ident_flag)
{
	తిరిగి xstrdup_or_null(git_author_info(ident_flag));
}

స్థిర అక్షరం *author_name(పూర్ణాంకం ident_flag)
{
	తిరిగి ident_part(git_author_info(ident_flag), IDENT_NAME);
}

స్థిర అక్షరం *author_email(పూర్ణాంకం ident_flag)
{
	తిరిగి ident_part(git_author_info(ident_flag), IDENT_MAIL);
}

స్థిర అక్షరం *author_date(పూర్ణాంకం ident_flag)
{
	తిరిగి ident_part(git_author_info(ident_flag), IDENT_DATE);
}

స్థిర అక్షరం *editor(పూర్ణాంకం ident_flag వాడని)
{
	తిరిగి xstrdup_or_null(git_editor());
}

స్థిర అక్షరం *sequence_editor(పూర్ణాంకం ident_flag వాడని)
{
	తిరిగి xstrdup_or_null(git_sequence_editor());
}

స్థిర అక్షరం *pager(పూర్ణాంకం ident_flag వాడని)
{
	స్థిరరాశి అక్షరం *pgm = git_pager(the_repository, 1);

	ఒకవేళ (!pgm)
		pgm = "cat";
	తిరిగి xstrdup(pgm);
}

స్థిర అక్షరం *default_branch(పూర్ణాంకం ident_flag వాడని)
{
	తిరిగి repo_default_branch_name(the_repository, 1);
}

స్థిర అక్షరం *shell_మార్గం(పూర్ణాంకం ident_flag వాడని)
{
	తిరిగి git_shell_మార్గం();
}

స్థిర అక్షరం *git_attr_val_system(పూర్ణాంకం ident_flag వాడని)
{
	ఒకవేళ (git_attr_system_is_enabled()) {
		అక్షరం *file = xstrdup(git_attr_system_file());
		normalize_మార్గం_copy(file, file);
		తిరిగి file;
	}
	తిరిగి ఖాళీ;
}

స్థిర అక్షరం *git_attr_val_global(పూర్ణాంకం ident_flag వాడని)
{
	అక్షరం *file = xstrdup_or_null(git_attr_global_file());
	ఒకవేళ (file) {
		normalize_మార్గం_copy(file, file);
		తిరిగి file;
	}
	తిరిగి ఖాళీ;
}

స్థిర అక్షరం *git_config_val_system(పూర్ణాంకం ident_flag వాడని)
{
	ఒకవేళ (git_config_system()) {
		అక్షరం *file = git_system_config();
		normalize_మార్గం_copy(file, file);
		తిరిగి file;
	}
	తిరిగి ఖాళీ;
}

స్థిర శూన్య git_config_val_global(నిర్మాణం string_list *list)
{
	అక్షరం *user, *xdg;

	git_global_config_paths(&user, &xdg);
	ఒకవేళ (xdg && *xdg) {
		normalize_మార్గం_copy(xdg, xdg);
		string_list_append(list, xdg);
	}
	ఒకవేళ (user && *user) {
		normalize_మార్గం_copy(user, user);
		string_list_append(list, user);
	}
	విడుదల(xdg);
	విడుదల(user);
}

నిర్మాణం git_var {
	స్థిరరాశి అక్షరం *name;
	అక్షరం *(*పఠ)(పూర్ణాంకం);
	శూన్య (*multiread)(నిర్మాణం string_list *);
};
స్థిర నిర్మాణం git_var git_vars[] = {
	{
		.name = "GIT_COMMITTER_IDENT",
		.పఠ = committer,
	},
	{
		.name = "GIT_COMMITTER_NAME",
		.పఠ = committer_name,
	},
	{
		.name = "GIT_COMMITTER_EMAIL",
		.పఠ = committer_email,
	},
	{
		.name = "GIT_COMMITTER_DATE",
		.పఠ = committer_date,
	},
	{
		.name = "GIT_AUTHOR_IDENT",
		.పఠ = author,
	},
	{
		.name = "GIT_AUTHOR_NAME",
		.పఠ = author_name,
	},
	{
		.name = "GIT_AUTHOR_EMAIL",
		.పఠ = author_email,
	},
	{
		.name = "GIT_AUTHOR_DATE",
		.పఠ = author_date,
	},
	{
		.name = "GIT_EDITOR",
		.పఠ = editor,
	},
	{
		.name = "GIT_SEQUENCE_EDITOR",
		.పఠ = sequence_editor,
	},
	{
		.name = "GIT_PAGER",
		.పఠ = pager,
	},
	{
		.name = "GIT_DEFAULT_BRANCH",
		.పఠ = default_branch,
	},
	{
		.name = "GIT_SHELL_PATH",
		.పఠ = shell_మార్గం,
	},
	{
		.name = "GIT_ATTR_SYSTEM",
		.పఠ = git_attr_val_system,
	},
	{
		.name = "GIT_ATTR_GLOBAL",
		.పఠ = git_attr_val_global,
	},
	{
		.name = "GIT_CONFIG_SYSTEM",
		.పఠ = git_config_val_system,
	},
	{
		.name = "GIT_CONFIG_GLOBAL",
		.multiread = git_config_val_global,
	},
	{
		.name = "",
		.పఠ = ఖాళీ,
	},
};

స్థిర శూన్య list_vars(పూర్ణాంకం nul_పదం)
{
	నిర్మాణం git_var *ptr;
	అక్షరం delim = nul_పదం ? '\n' : '=';
	అక్షరం పదం = nul_పదం ? '\0' : '\n';

	కొరకు (ptr = git_vars; ptr->పఠ || ptr->multiread; ptr++) {
		ఒకవేళ (ptr->పఠ) {
			అక్షరం *val = ptr->పఠ(0);

			ఒకవేళ (val) {
				రాయి("%s%c%s%c", ptr->name, delim, val, పదం);
				విడుదల(val);
			}
		} లేకపోతే {
			నిర్మాణం string_list list = STRING_LIST_INIT_DUP;

			ptr->multiread(&list);
			కొరకు (కొలత_t i = 0; i < list.nr; i++)
				రాయి("%s%c%s%c", ptr->name, delim,
				       list.వస్తువులు[i].string, పదం);
			string_list_clear(&list, 0);
		}
	}
}

స్థిర స్థిరరాశి నిర్మాణం git_var *get_git_var(స్థిరరాశి అక్షరం *var)
{
	నిర్మాణం git_var *ptr;
	కొరకు (ptr = git_vars; ptr->పఠ || ptr->multiread; ptr++) {
		ఒకవేళ (strcmp(var, ptr->name) == 0) {
			తిరిగి ptr;
		}
	}
	తిరిగి ఖాళీ;
}

స్థిర పూర్ణాంకం show_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
		       స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	పూర్ణాంకం *nul_పదం = cb;
	అక్షరం delim = *nul_పదం ? '\n' : '=';
	అక్షరం పదం = *nul_పదం ? '\0' : '\n';

	ఒకవేళ (విలువ)
		రాయి("%s%c%s%c", var, delim, విలువ, పదం);
	లేకపోతే
		రాయి("%s%c", var, పదం);
	తిరిగి git_default_config(var, విలువ, ctx, cb);
}

పూర్ణాంకం cmd_var(పూర్ణాంకం argc,
	    స్థిరరాశి అక్షరం **argv,
	    స్థిరరాశి అక్షరం *ఉపసర్గ,
	    నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం list = 0;
	పూర్ణాంకం nul_పదం = 0;
	అక్షరం delim;
	అక్షరం పదం;
	నిర్మాణం option options[] = {
		OPT_BOOL('l', ఖాళీ, &list,
			 N_("list all variables")),
		OPT_BOOL('z', ఖాళీ, &nul_పదం,
			 N_("terminate entries with NUL")),
		OPT_చివర(),
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     var_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	ఒకవేళ (list) {
		ఒకవేళ (argc)
			usage_with_options(var_usage, options);
		repo_config(the_repository, show_config, &nul_పదం);
		list_vars(nul_పదం);
		తిరిగి 0;
	}

	ఒకవేళ (!argc)
		usage_with_options(var_usage, options);

	repo_config(the_repository, git_default_config, ఖాళీ);

	delim = nul_పదం ? '\n' : '=';
	పదం = nul_పదం ? '\0' : '\n';

	కొరకు (పూర్ణాంకం i = 0; i < argc; i++) {
		స్థిరరాశి నిర్మాణం git_var *git_var = get_git_var(argv[i]);

		ఒకవేళ (!git_var)
			usage_with_options(var_usage, options);

		ఒకవేళ (git_var->పఠ) {
			అక్షరం *val = git_var->పఠ(IDENT_STRICT);

			ఒకవేళ (!val) {
				ఒకవేళ (argc == 1)
					తిరిగి 1;
				కొనసాగు;
			}
			ఒకవేళ (argc == 1)
				రాయి("%s%c", val, పదం);
			లేకపోతే
				రాయి("%s%c%s%c", git_var->name, delim,
				       val, పదం);
			విడుదల(val);
		} లేకపోతే {
			నిర్మాణం string_list list = STRING_LIST_INIT_DUP;

			git_var->multiread(&list);
			ఒకవేళ (argc == 1 && !list.nr) {
				string_list_clear(&list, 0);
				తిరిగి 1;
			}
			కొరకు (కొలత_t j = 0; j < list.nr; j++) {
				ఒకవేళ (argc == 1)
					రాయి("%s%c", list.వస్తువులు[j].string,
					       పదం);
				లేకపోతే
					రాయి("%s%c%s%c", git_var->name, delim,
					       list.వస్తువులు[j].string, పదం);
			}
			string_list_clear(&list, 0);
		}
	}

	తిరిగి 0;
}
