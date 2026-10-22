/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Eric Biederman, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE

#ଅନ୍ତର୍ଭାଵ "builtin.h"

#ଅନ୍ତର୍ଭାଵ "attr.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "editor.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "ident.h"
#ଅନ୍ତର୍ଭାଵ "pager.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ var_usage[] = {
	N_("git var [-z] -l"),
	N_("git var [-z] <variable>..."),
	ଖାଲି
};

ଗଣନା ident_part {
	IDENT_NAME,
	IDENT_MAIL,
	IDENT_DATE,
};

ସ୍ଥିର ଅକ୍ଷର *ident_part(ଅଚଳ ଅକ୍ଷର *ident, ଗଣନା ident_part part)
{
	ସଂରଚନା ident_split split;

	ଯଦି (!ident)
		ଫେରନ୍ତୁ ଖାଲି;
	ଯଦି (split_ident_ଧାଡ଼ି(&split, ident, strlen(ident)))
		ଫେରନ୍ତୁ ଖାଲି;

	ବାଛ (part) {
	ସ୍ଥିତି IDENT_NAME:
		ଯଦି (!split.name_begin || !split.name_end)
			BUG("split_ident_line() gave NULL names???");
		ଫେରନ୍ତୁ xmemdupz(split.name_begin,
				split.name_end - split.name_begin);
	ସ୍ଥିତି IDENT_MAIL:
		ଯଦି (!split.mail_begin || !split.mail_end)
			BUG("split_ident_line() gave NULL mail???");
		ଫେରନ୍ତୁ xmemdupz(split.mail_begin,
				split.mail_end - split.mail_begin);
	ସ୍ଥିତି IDENT_DATE:
		ଯଦି (!split.date_begin || !split.tz_end)
			BUG("split_ident_line() gave NULL date/tz???");
		ଫେରନ୍ତୁ xmemdupz(split.date_begin,
				split.tz_end - split.date_begin);
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("unknown ident_part %d", part);
	}
}

ସ୍ଥିର ଅକ୍ଷର *committer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ident_flag)
{
	ଫେରନ୍ତୁ xstrdup_or_null(git_committer_info(ident_flag));
}

ସ୍ଥିର ଅକ୍ଷର *committer_name(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ident_flag)
{
	ଫେରନ୍ତୁ ident_part(git_committer_info(ident_flag), IDENT_NAME);
}

ସ୍ଥିର ଅକ୍ଷର *committer_email(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ident_flag)
{
	ଫେରନ୍ତୁ ident_part(git_committer_info(ident_flag), IDENT_MAIL);
}

ସ୍ଥିର ଅକ୍ଷର *committer_date(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ident_flag)
{
	ଫେରନ୍ତୁ ident_part(git_committer_info(ident_flag), IDENT_DATE);
}

ସ୍ଥିର ଅକ୍ଷର *author(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ident_flag)
{
	ଫେରନ୍ତୁ xstrdup_or_null(git_author_info(ident_flag));
}

ସ୍ଥିର ଅକ୍ଷର *author_name(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ident_flag)
{
	ଫେରନ୍ତୁ ident_part(git_author_info(ident_flag), IDENT_NAME);
}

ସ୍ଥିର ଅକ୍ଷର *author_email(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ident_flag)
{
	ଫେରନ୍ତୁ ident_part(git_author_info(ident_flag), IDENT_MAIL);
}

ସ୍ଥିର ଅକ୍ଷର *author_date(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ident_flag)
{
	ଫେରନ୍ତୁ ident_part(git_author_info(ident_flag), IDENT_DATE);
}

ସ୍ଥିର ଅକ୍ଷର *editor(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ident_flag UNUSED)
{
	ଫେରନ୍ତୁ xstrdup_or_null(git_editor());
}

ସ୍ଥିର ଅକ୍ଷର *sequence_editor(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ident_flag UNUSED)
{
	ଫେରନ୍ତୁ xstrdup_or_null(git_sequence_editor());
}

ସ୍ଥିର ଅକ୍ଷର *pager(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ident_flag UNUSED)
{
	ଅଚଳ ଅକ୍ଷର *pgm = git_pager(the_repository, 1);

	ଯଦି (!pgm)
		pgm = "cat";
	ଫେରନ୍ତୁ xstrdup(pgm);
}

ସ୍ଥିର ଅକ୍ଷର *default_branch(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ident_flag UNUSED)
{
	ଫେରନ୍ତୁ repo_default_branch_name(the_repository, 1);
}

ସ୍ଥିର ଅକ୍ଷର *shell_path(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ident_flag UNUSED)
{
	ଫେରନ୍ତୁ git_shell_path();
}

ସ୍ଥିର ଅକ୍ଷର *git_attr_val_system(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ident_flag UNUSED)
{
	ଯଦି (git_attr_system_is_enabled()) {
		ଅକ୍ଷର *file = xstrdup(git_attr_system_file());
		normalize_path_copy(file, file);
		ଫେରନ୍ତୁ file;
	}
	ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ଅକ୍ଷର *git_attr_val_global(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ident_flag UNUSED)
{
	ଅକ୍ଷର *file = xstrdup_or_null(git_attr_global_file());
	ଯଦି (file) {
		normalize_path_copy(file, file);
		ଫେରନ୍ତୁ file;
	}
	ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ଅକ୍ଷର *git_config_val_system(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ident_flag UNUSED)
{
	ଯଦି (git_config_system()) {
		ଅକ୍ଷର *file = git_system_config();
		normalize_path_copy(file, file);
		ଫେରନ୍ତୁ file;
	}
	ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ଶୂନ୍ୟ git_config_val_global(ସଂରଚନା string_list *list)
{
	ଅକ୍ଷର *user, *xdg;

	git_global_config_paths(&user, &xdg);
	ଯଦି (xdg && *xdg) {
		normalize_path_copy(xdg, xdg);
		string_list_append(list, xdg);
	}
	ଯଦି (user && *user) {
		normalize_path_copy(user, user);
		string_list_append(list, user);
	}
	ମୁକ୍ତ(xdg);
	ମୁକ୍ତ(user);
}

ସଂରଚନା git_var {
	ଅଚଳ ଅକ୍ଷର *name;
	ଅକ୍ଷର *(*ପଠ)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
	ଶୂନ୍ୟ (*multiread)(ସଂରଚନା string_list *);
};
ସ୍ଥିର ସଂରଚନା git_var git_vars[] = {
	{
		.name = "GIT_COMMITTER_IDENT",
		.ପଠ = committer,
	},
	{
		.name = "GIT_COMMITTER_NAME",
		.ପଠ = committer_name,
	},
	{
		.name = "GIT_COMMITTER_EMAIL",
		.ପଠ = committer_email,
	},
	{
		.name = "GIT_COMMITTER_DATE",
		.ପଠ = committer_date,
	},
	{
		.name = "GIT_AUTHOR_IDENT",
		.ପଠ = author,
	},
	{
		.name = "GIT_AUTHOR_NAME",
		.ପଠ = author_name,
	},
	{
		.name = "GIT_AUTHOR_EMAIL",
		.ପଠ = author_email,
	},
	{
		.name = "GIT_AUTHOR_DATE",
		.ପଠ = author_date,
	},
	{
		.name = "GIT_EDITOR",
		.ପଠ = editor,
	},
	{
		.name = "GIT_SEQUENCE_EDITOR",
		.ପଠ = sequence_editor,
	},
	{
		.name = "GIT_PAGER",
		.ପଠ = pager,
	},
	{
		.name = "GIT_DEFAULT_BRANCH",
		.ପଠ = default_branch,
	},
	{
		.name = "GIT_SHELL_PATH",
		.ପଠ = shell_path,
	},
	{
		.name = "GIT_ATTR_SYSTEM",
		.ପଠ = git_attr_val_system,
	},
	{
		.name = "GIT_ATTR_GLOBAL",
		.ପଠ = git_attr_val_global,
	},
	{
		.name = "GIT_CONFIG_SYSTEM",
		.ପଠ = git_config_val_system,
	},
	{
		.name = "GIT_CONFIG_GLOBAL",
		.multiread = git_config_val_global,
	},
	{
		.name = "",
		.ପଠ = ଖାଲି,
	},
};

ସ୍ଥିର ଶୂନ୍ୟ list_vars(ପୂର୍ଣ୍ଣସଂଖ୍ୟା nul_term)
{
	ସଂରଚନା git_var *ptr;
	ଅକ୍ଷର delim = nul_term ? '\n' : '=';
	ଅକ୍ଷର term = nul_term ? '\0' : '\n';

	ପାଇଁ (ptr = git_vars; ptr->ପଠ || ptr->multiread; ptr++) {
		ଯଦି (ptr->ପଠ) {
			ଅକ୍ଷର *val = ptr->ପଠ(0);

			ଯଦି (val) {
				printf("%s%c%s%c", ptr->name, delim, val, term);
				ମୁକ୍ତ(val);
			}
		} ଅନ୍ୟଥା {
			ସଂରଚନା string_list list = STRING_LIST_INIT_DUP;

			ptr->multiread(&list);
			ପାଇଁ (size_t i = 0; i < list.nr; i++)
				printf("%s%c%s%c", ptr->name, delim,
				       list.ବସ୍ତୁଗୁଡ଼ିକ[i].string, term);
			string_list_clear(&list, 0);
		}
	}
}

ସ୍ଥିର ଅଚଳ ସଂରଚନା git_var *get_git_var(ଅଚଳ ଅକ୍ଷର *var)
{
	ସଂରଚନା git_var *ptr;
	ପାଇଁ (ptr = git_vars; ptr->ପଠ || ptr->multiread; ptr++) {
		ଯଦି (strcmp(var, ptr->name) == 0) {
			ଫେରନ୍ତୁ ptr;
		}
	}
	ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
		       ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *nul_term = cb;
	ଅକ୍ଷର delim = *nul_term ? '\n' : '=';
	ଅକ୍ଷର term = *nul_term ? '\0' : '\n';

	ଯଦି (ମୂଲ୍ୟ)
		printf("%s%c%s%c", var, delim, ମୂଲ୍ୟ, term);
	ଅନ୍ୟଥା
		printf("%s%c", var, term);
	ଫେରନ୍ତୁ git_default_config(var, ମୂଲ୍ୟ, ctx, cb);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_var(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	    ଅଚଳ ଅକ୍ଷର **argv,
	    ଅଚଳ ଅକ୍ଷର *prefix,
	    ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା list = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nul_term = 0;
	ଅକ୍ଷର delim;
	ଅକ୍ଷର term;
	ସଂରଚନା option options[] = {
		OPT_BOOL('l', ଖାଲି, &list,
			 N_("list all variables")),
		OPT_BOOL('z', ଖାଲି, &nul_term,
			 N_("terminate entries with NUL")),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options,
			     var_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	ଯଦି (list) {
		ଯଦି (argc)
			usage_with_options(var_usage, options);
		repo_config(the_repository, show_config, &nul_term);
		list_vars(nul_term);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!argc)
		usage_with_options(var_usage, options);

	repo_config(the_repository, git_default_config, ଖାଲି);

	delim = nul_term ? '\n' : '=';
	term = nul_term ? '\0' : '\n';

	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < argc; i++) {
		ଅଚଳ ସଂରଚନା git_var *git_var = get_git_var(argv[i]);

		ଯଦି (!git_var)
			usage_with_options(var_usage, options);

		ଯଦି (git_var->ପଠ) {
			ଅକ୍ଷର *val = git_var->ପଠ(IDENT_STRICT);

			ଯଦି (!val) {
				ଯଦି (argc == 1)
					ଫେରନ୍ତୁ 1;
				ଜାରି;
			}
			ଯଦି (argc == 1)
				printf("%s%c", val, term);
			ଅନ୍ୟଥା
				printf("%s%c%s%c", git_var->name, delim,
				       val, term);
			ମୁକ୍ତ(val);
		} ଅନ୍ୟଥା {
			ସଂରଚନା string_list list = STRING_LIST_INIT_DUP;

			git_var->multiread(&list);
			ଯଦି (argc == 1 && !list.nr) {
				string_list_clear(&list, 0);
				ଫେରନ୍ତୁ 1;
			}
			ପାଇଁ (size_t j = 0; j < list.nr; j++) {
				ଯଦି (argc == 1)
					printf("%s%c", list.ବସ୍ତୁଗୁଡ଼ିକ[j].string,
					       term);
				ଅନ୍ୟଥା
					printf("%s%c%s%c", git_var->name, delim,
					       list.ବସ୍ତୁଗୁଡ଼ିକ[j].string, term);
			}
			string_list_clear(&list, 0);
		}
	}

	ଫେରନ୍ତୁ 0;
}
