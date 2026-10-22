/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Eric Biederman, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "builtin.h"

#અન્તર્ભાવ "attr.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "editor.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "ident.h"
#અન્તર્ભાવ "pager.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "strbuf.h"
#અન્તર્ભાવ "string-list.h"

સ્થિર અચલ અક્ષર * અચલ var_usage[] = {
	N_("git var [-z] -l"),
	N_("git var [-z] <variable>..."),
	ખાલી
};

ગણના ident_part {
	IDENT_NAME,
	IDENT_MAIL,
	IDENT_DATE,
};

સ્થિર અક્ષર *ident_part(અચલ અક્ષર *ident, ગણના ident_part part)
{
	સંરચના ident_split split;

	જો (!ident)
		પાછા ખાલી;
	જો (split_ident_લીટી(&split, ident, strlen(ident)))
		પાછા ખાલી;

	પસંદગી (part) {
	સ્થિતિ IDENT_NAME:
		જો (!split.name_begin || !split.name_end)
			BUG("split_ident_line() gave NULL names???");
		પાછા xmemdupz(split.name_begin,
				split.name_end - split.name_begin);
	સ્થિતિ IDENT_MAIL:
		જો (!split.mail_begin || !split.mail_end)
			BUG("split_ident_line() gave NULL mail???");
		પાછા xmemdupz(split.mail_begin,
				split.mail_end - split.mail_begin);
	સ્થિતિ IDENT_DATE:
		જો (!split.date_begin || !split.tz_end)
			BUG("split_ident_line() gave NULL date/tz???");
		પાછા xmemdupz(split.date_begin,
				split.tz_end - split.date_begin);
	મૂળભૂત:
		BUG("unknown ident_part %d", part);
	}
}

સ્થિર અક્ષર *committer(પૂર્ણાંક ident_flag)
{
	પાછા xstrdup_or_null(git_committer_info(ident_flag));
}

સ્થિર અક્ષર *committer_name(પૂર્ણાંક ident_flag)
{
	પાછા ident_part(git_committer_info(ident_flag), IDENT_NAME);
}

સ્થિર અક્ષર *committer_email(પૂર્ણાંક ident_flag)
{
	પાછા ident_part(git_committer_info(ident_flag), IDENT_MAIL);
}

સ્થિર અક્ષર *committer_date(પૂર્ણાંક ident_flag)
{
	પાછા ident_part(git_committer_info(ident_flag), IDENT_DATE);
}

સ્થિર અક્ષર *author(પૂર્ણાંક ident_flag)
{
	પાછા xstrdup_or_null(git_author_info(ident_flag));
}

સ્થિર અક્ષર *author_name(પૂર્ણાંક ident_flag)
{
	પાછા ident_part(git_author_info(ident_flag), IDENT_NAME);
}

સ્થિર અક્ષર *author_email(પૂર્ણાંક ident_flag)
{
	પાછા ident_part(git_author_info(ident_flag), IDENT_MAIL);
}

સ્થિર અક્ષર *author_date(પૂર્ણાંક ident_flag)
{
	પાછા ident_part(git_author_info(ident_flag), IDENT_DATE);
}

સ્થિર અક્ષર *editor(પૂર્ણાંક ident_flag UNUSED)
{
	પાછા xstrdup_or_null(git_editor());
}

સ્થિર અક્ષર *sequence_editor(પૂર્ણાંક ident_flag UNUSED)
{
	પાછા xstrdup_or_null(git_sequence_editor());
}

સ્થિર અક્ષર *pager(પૂર્ણાંક ident_flag UNUSED)
{
	અચલ અક્ષર *pgm = git_pager(the_repository, 1);

	જો (!pgm)
		pgm = "cat";
	પાછા xstrdup(pgm);
}

સ્થિર અક્ષર *default_branch(પૂર્ણાંક ident_flag UNUSED)
{
	પાછા repo_default_branch_name(the_repository, 1);
}

સ્થિર અક્ષર *shell_path(પૂર્ણાંક ident_flag UNUSED)
{
	પાછા git_shell_path();
}

સ્થિર અક્ષર *git_attr_val_system(પૂર્ણાંક ident_flag UNUSED)
{
	જો (git_attr_system_is_enabled()) {
		અક્ષર *file = xstrdup(git_attr_system_file());
		normalize_path_copy(file, file);
		પાછા file;
	}
	પાછા ખાલી;
}

સ્થિર અક્ષર *git_attr_val_global(પૂર્ણાંક ident_flag UNUSED)
{
	અક્ષર *file = xstrdup_or_null(git_attr_global_file());
	જો (file) {
		normalize_path_copy(file, file);
		પાછા file;
	}
	પાછા ખાલી;
}

સ્થિર અક્ષર *git_config_val_system(પૂર્ણાંક ident_flag UNUSED)
{
	જો (git_config_system()) {
		અક્ષર *file = git_system_config();
		normalize_path_copy(file, file);
		પાછા file;
	}
	પાછા ખાલી;
}

સ્થિર શૂન્ય git_config_val_global(સંરચના string_list *list)
{
	અક્ષર *user, *xdg;

	git_global_config_paths(&user, &xdg);
	જો (xdg && *xdg) {
		normalize_path_copy(xdg, xdg);
		string_list_append(list, xdg);
	}
	જો (user && *user) {
		normalize_path_copy(user, user);
		string_list_append(list, user);
	}
	મુક્ત(xdg);
	મુક્ત(user);
}

સંરચના git_var {
	અચલ અક્ષર *name;
	અક્ષર *(*પઠ)(પૂર્ણાંક);
	શૂન્ય (*multiread)(સંરચના string_list *);
};
સ્થિર સંરચના git_var git_vars[] = {
	{
		.name = "GIT_COMMITTER_IDENT",
		.પઠ = committer,
	},
	{
		.name = "GIT_COMMITTER_NAME",
		.પઠ = committer_name,
	},
	{
		.name = "GIT_COMMITTER_EMAIL",
		.પઠ = committer_email,
	},
	{
		.name = "GIT_COMMITTER_DATE",
		.પઠ = committer_date,
	},
	{
		.name = "GIT_AUTHOR_IDENT",
		.પઠ = author,
	},
	{
		.name = "GIT_AUTHOR_NAME",
		.પઠ = author_name,
	},
	{
		.name = "GIT_AUTHOR_EMAIL",
		.પઠ = author_email,
	},
	{
		.name = "GIT_AUTHOR_DATE",
		.પઠ = author_date,
	},
	{
		.name = "GIT_EDITOR",
		.પઠ = editor,
	},
	{
		.name = "GIT_SEQUENCE_EDITOR",
		.પઠ = sequence_editor,
	},
	{
		.name = "GIT_PAGER",
		.પઠ = pager,
	},
	{
		.name = "GIT_DEFAULT_BRANCH",
		.પઠ = default_branch,
	},
	{
		.name = "GIT_SHELL_PATH",
		.પઠ = shell_path,
	},
	{
		.name = "GIT_ATTR_SYSTEM",
		.પઠ = git_attr_val_system,
	},
	{
		.name = "GIT_ATTR_GLOBAL",
		.પઠ = git_attr_val_global,
	},
	{
		.name = "GIT_CONFIG_SYSTEM",
		.પઠ = git_config_val_system,
	},
	{
		.name = "GIT_CONFIG_GLOBAL",
		.multiread = git_config_val_global,
	},
	{
		.name = "",
		.પઠ = ખાલી,
	},
};

સ્થિર શૂન્ય list_vars(પૂર્ણાંક nul_term)
{
	સંરચના git_var *ptr;
	અક્ષર delim = nul_term ? '\n' : '=';
	અક્ષર term = nul_term ? '\0' : '\n';

	માટે (ptr = git_vars; ptr->પઠ || ptr->multiread; ptr++) {
		જો (ptr->પઠ) {
			અક્ષર *val = ptr->પઠ(0);

			જો (val) {
				printf("%s%c%s%c", ptr->name, delim, val, term);
				મુક્ત(val);
			}
		} નહીંતર {
			સંરચના string_list list = STRING_LIST_INIT_DUP;

			ptr->multiread(&list);
			માટે (size_t i = 0; i < list.nr; i++)
				printf("%s%c%s%c", ptr->name, delim,
				       list.વસ્તુઓ[i].string, term);
			string_list_clear(&list, 0);
		}
	}
}

સ્થિર અચલ સંરચના git_var *get_git_var(અચલ અક્ષર *var)
{
	સંરચના git_var *ptr;
	માટે (ptr = git_vars; ptr->પઠ || ptr->multiread; ptr++) {
		જો (strcmp(var, ptr->name) == 0) {
			પાછા ptr;
		}
	}
	પાછા ખાલી;
}

સ્થિર પૂર્ણાંક show_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
		       અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	પૂર્ણાંક *nul_term = cb;
	અક્ષર delim = *nul_term ? '\n' : '=';
	અક્ષર term = *nul_term ? '\0' : '\n';

	જો (કિંમત)
		printf("%s%c%s%c", var, delim, કિંમત, term);
	નહીંતર
		printf("%s%c", var, term);
	પાછા git_default_config(var, કિંમત, ctx, cb);
}

પૂર્ણાંક cmd_var(પૂર્ણાંક argc,
	    અચલ અક્ષર **argv,
	    અચલ અક્ષર *prefix,
	    સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક list = 0;
	પૂર્ણાંક nul_term = 0;
	અક્ષર delim;
	અક્ષર term;
	સંરચના option options[] = {
		OPT_BOOL('l', ખાલી, &list,
			 N_("list all variables")),
		OPT_BOOL('z', ખાલી, &nul_term,
			 N_("terminate entries with NUL")),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options,
			     var_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	જો (list) {
		જો (argc)
			usage_with_options(var_usage, options);
		repo_config(the_repository, show_config, &nul_term);
		list_vars(nul_term);
		પાછા 0;
	}

	જો (!argc)
		usage_with_options(var_usage, options);

	repo_config(the_repository, git_default_config, ખાલી);

	delim = nul_term ? '\n' : '=';
	term = nul_term ? '\0' : '\n';

	માટે (પૂર્ણાંક i = 0; i < argc; i++) {
		અચલ સંરચના git_var *git_var = get_git_var(argv[i]);

		જો (!git_var)
			usage_with_options(var_usage, options);

		જો (git_var->પઠ) {
			અક્ષર *val = git_var->પઠ(IDENT_STRICT);

			જો (!val) {
				જો (argc == 1)
					પાછા 1;
				ચાલુ;
			}
			જો (argc == 1)
				printf("%s%c", val, term);
			નહીંતર
				printf("%s%c%s%c", git_var->name, delim,
				       val, term);
			મુક્ત(val);
		} નહીંતર {
			સંરચના string_list list = STRING_LIST_INIT_DUP;

			git_var->multiread(&list);
			જો (argc == 1 && !list.nr) {
				string_list_clear(&list, 0);
				પાછા 1;
			}
			માટે (size_t j = 0; j < list.nr; j++) {
				જો (argc == 1)
					printf("%s%c", list.વસ્તુઓ[j].string,
					       term);
				નહીંતર
					printf("%s%c%s%c", git_var->name, delim,
					       list.વસ્તુઓ[j].string, term);
			}
			string_list_clear(&list, 0);
		}
	}

	પાછા 0;
}
