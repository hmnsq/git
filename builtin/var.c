/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Eric Biederman, 2005
 */

#define USE_THE_REPOSITORY_VARIABLE

#அந்தர்பாவ "builtin.h"

#அந்தர்பாவ "attr.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "editor.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "ident.h"
#அந்தர்பாவ "pager.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "strbuf.h"
#அந்தர்பாவ "string-list.h"

நிலையான மாறிலி எழுத்து * மாறிலி var_usage[] = {
	N_("git var [-z] -l"),
	N_("git var [-z] <variable>..."),
	வெற்று
};

எண்ணிக்கை ident_part {
	IDENT_NAME,
	IDENT_MAIL,
	IDENT_DATE,
};

நிலையான எழுத்து *ident_part(மாறிலி எழுத்து *ident, எண்ணிக்கை ident_part part)
{
	கட்டமைப்பு ident_split split;

	எனில் (!ident)
		திரும்பு வெற்று;
	எனில் (split_ident_வரி(&split, ident, strlen(ident)))
		திரும்பு வெற்று;

	தேர்வு (part) {
	நிலை IDENT_NAME:
		எனில் (!split.name_begin || !split.name_end)
			BUG("split_ident_line() gave NULL names???");
		திரும்பு xmemdupz(split.name_begin,
				split.name_end - split.name_begin);
	நிலை IDENT_MAIL:
		எனில் (!split.mail_begin || !split.mail_end)
			BUG("split_ident_line() gave NULL mail???");
		திரும்பு xmemdupz(split.mail_begin,
				split.mail_end - split.mail_begin);
	நிலை IDENT_DATE:
		எனில் (!split.date_begin || !split.tz_end)
			BUG("split_ident_line() gave NULL date/tz???");
		திரும்பு xmemdupz(split.date_begin,
				split.tz_end - split.date_begin);
	இயல்பு:
		BUG("unknown ident_part %d", part);
	}
}

நிலையான எழுத்து *committer(முழு ident_flag)
{
	திரும்பு xstrdup_or_null(git_committer_info(ident_flag));
}

நிலையான எழுத்து *committer_name(முழு ident_flag)
{
	திரும்பு ident_part(git_committer_info(ident_flag), IDENT_NAME);
}

நிலையான எழுத்து *committer_email(முழு ident_flag)
{
	திரும்பு ident_part(git_committer_info(ident_flag), IDENT_MAIL);
}

நிலையான எழுத்து *committer_date(முழு ident_flag)
{
	திரும்பு ident_part(git_committer_info(ident_flag), IDENT_DATE);
}

நிலையான எழுத்து *author(முழு ident_flag)
{
	திரும்பு xstrdup_or_null(git_author_info(ident_flag));
}

நிலையான எழுத்து *author_name(முழு ident_flag)
{
	திரும்பு ident_part(git_author_info(ident_flag), IDENT_NAME);
}

நிலையான எழுத்து *author_email(முழு ident_flag)
{
	திரும்பு ident_part(git_author_info(ident_flag), IDENT_MAIL);
}

நிலையான எழுத்து *author_date(முழு ident_flag)
{
	திரும்பு ident_part(git_author_info(ident_flag), IDENT_DATE);
}

நிலையான எழுத்து *editor(முழு ident_flag UNUSED)
{
	திரும்பு xstrdup_or_null(git_editor());
}

நிலையான எழுத்து *sequence_editor(முழு ident_flag UNUSED)
{
	திரும்பு xstrdup_or_null(git_sequence_editor());
}

நிலையான எழுத்து *pager(முழு ident_flag UNUSED)
{
	மாறிலி எழுத்து *pgm = git_pager(the_repository, 1);

	எனில் (!pgm)
		pgm = "cat";
	திரும்பு xstrdup(pgm);
}

நிலையான எழுத்து *default_branch(முழு ident_flag UNUSED)
{
	திரும்பு repo_default_branch_name(the_repository, 1);
}

நிலையான எழுத்து *shell_path(முழு ident_flag UNUSED)
{
	திரும்பு git_shell_path();
}

நிலையான எழுத்து *git_attr_val_system(முழு ident_flag UNUSED)
{
	எனில் (git_attr_system_is_enabled()) {
		எழுத்து *file = xstrdup(git_attr_system_file());
		normalize_path_copy(file, file);
		திரும்பு file;
	}
	திரும்பு வெற்று;
}

நிலையான எழுத்து *git_attr_val_global(முழு ident_flag UNUSED)
{
	எழுத்து *file = xstrdup_or_null(git_attr_global_file());
	எனில் (file) {
		normalize_path_copy(file, file);
		திரும்பு file;
	}
	திரும்பு வெற்று;
}

நிலையான எழுத்து *git_config_val_system(முழு ident_flag UNUSED)
{
	எனில் (git_config_system()) {
		எழுத்து *file = git_system_config();
		normalize_path_copy(file, file);
		திரும்பு file;
	}
	திரும்பு வெற்று;
}

நிலையான வெறுமை git_config_val_global(கட்டமைப்பு string_list *list)
{
	எழுத்து *user, *xdg;

	git_global_config_paths(&user, &xdg);
	எனில் (xdg && *xdg) {
		normalize_path_copy(xdg, xdg);
		string_list_append(list, xdg);
	}
	எனில் (user && *user) {
		normalize_path_copy(user, user);
		string_list_append(list, user);
	}
	விடுவி(xdg);
	விடுவி(user);
}

கட்டமைப்பு git_var {
	மாறிலி எழுத்து *name;
	எழுத்து *(*பட)(முழு);
	வெறுமை (*multiread)(கட்டமைப்பு string_list *);
};
நிலையான கட்டமைப்பு git_var git_vars[] = {
	{
		.name = "GIT_COMMITTER_IDENT",
		.பட = committer,
	},
	{
		.name = "GIT_COMMITTER_NAME",
		.பட = committer_name,
	},
	{
		.name = "GIT_COMMITTER_EMAIL",
		.பட = committer_email,
	},
	{
		.name = "GIT_COMMITTER_DATE",
		.பட = committer_date,
	},
	{
		.name = "GIT_AUTHOR_IDENT",
		.பட = author,
	},
	{
		.name = "GIT_AUTHOR_NAME",
		.பட = author_name,
	},
	{
		.name = "GIT_AUTHOR_EMAIL",
		.பட = author_email,
	},
	{
		.name = "GIT_AUTHOR_DATE",
		.பட = author_date,
	},
	{
		.name = "GIT_EDITOR",
		.பட = editor,
	},
	{
		.name = "GIT_SEQUENCE_EDITOR",
		.பட = sequence_editor,
	},
	{
		.name = "GIT_PAGER",
		.பட = pager,
	},
	{
		.name = "GIT_DEFAULT_BRANCH",
		.பட = default_branch,
	},
	{
		.name = "GIT_SHELL_PATH",
		.பட = shell_path,
	},
	{
		.name = "GIT_ATTR_SYSTEM",
		.பட = git_attr_val_system,
	},
	{
		.name = "GIT_ATTR_GLOBAL",
		.பட = git_attr_val_global,
	},
	{
		.name = "GIT_CONFIG_SYSTEM",
		.பட = git_config_val_system,
	},
	{
		.name = "GIT_CONFIG_GLOBAL",
		.multiread = git_config_val_global,
	},
	{
		.name = "",
		.பட = வெற்று,
	},
};

நிலையான வெறுமை list_vars(முழு nul_term)
{
	கட்டமைப்பு git_var *ptr;
	எழுத்து delim = nul_term ? '\n' : '=';
	எழுத்து term = nul_term ? '\0' : '\n';

	க்கு (ptr = git_vars; ptr->பட || ptr->multiread; ptr++) {
		எனில் (ptr->பட) {
			எழுத்து *val = ptr->பட(0);

			எனில் (val) {
				printf("%s%c%s%c", ptr->name, delim, val, term);
				விடுவி(val);
			}
		} இல்லையெனில் {
			கட்டமைப்பு string_list list = STRING_LIST_INIT_DUP;

			ptr->multiread(&list);
			க்கு (size_t i = 0; i < list.nr; i++)
				printf("%s%c%s%c", ptr->name, delim,
				       list.பொருட்கள்[i].string, term);
			string_list_clear(&list, 0);
		}
	}
}

நிலையான மாறிலி கட்டமைப்பு git_var *get_git_var(மாறிலி எழுத்து *var)
{
	கட்டமைப்பு git_var *ptr;
	க்கு (ptr = git_vars; ptr->பட || ptr->multiread; ptr++) {
		எனில் (strcmp(var, ptr->name) == 0) {
			திரும்பு ptr;
		}
	}
	திரும்பு வெற்று;
}

நிலையான முழு show_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
		       மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	முழு *nul_term = cb;
	எழுத்து delim = *nul_term ? '\n' : '=';
	எழுத்து term = *nul_term ? '\0' : '\n';

	எனில் (மதிப்பு)
		printf("%s%c%s%c", var, delim, மதிப்பு, term);
	இல்லையெனில்
		printf("%s%c", var, term);
	திரும்பு git_default_config(var, மதிப்பு, ctx, cb);
}

முழு cmd_var(முழு argc,
	    மாறிலி எழுத்து **argv,
	    மாறிலி எழுத்து *prefix,
	    கட்டமைப்பு repository *repo UNUSED)
{
	முழு list = 0;
	முழு nul_term = 0;
	எழுத்து delim;
	எழுத்து term;
	கட்டமைப்பு option options[] = {
		OPT_BOOL('l', வெற்று, &list,
			 N_("list all variables")),
		OPT_BOOL('z', வெற்று, &nul_term,
			 N_("terminate entries with NUL")),
		OPT_END(),
	};

	argc = parse_options(argc, argv, prefix, options,
			     var_usage, PARSE_OPT_STOP_AT_NON_OPTION);

	எனில் (list) {
		எனில் (argc)
			usage_with_options(var_usage, options);
		repo_config(the_repository, show_config, &nul_term);
		list_vars(nul_term);
		திரும்பு 0;
	}

	எனில் (!argc)
		usage_with_options(var_usage, options);

	repo_config(the_repository, git_default_config, வெற்று);

	delim = nul_term ? '\n' : '=';
	term = nul_term ? '\0' : '\n';

	க்கு (முழு i = 0; i < argc; i++) {
		மாறிலி கட்டமைப்பு git_var *git_var = get_git_var(argv[i]);

		எனில் (!git_var)
			usage_with_options(var_usage, options);

		எனில் (git_var->பட) {
			எழுத்து *val = git_var->பட(IDENT_STRICT);

			எனில் (!val) {
				எனில் (argc == 1)
					திரும்பு 1;
				தொடர்;
			}
			எனில் (argc == 1)
				printf("%s%c", val, term);
			இல்லையெனில்
				printf("%s%c%s%c", git_var->name, delim,
				       val, term);
			விடுவி(val);
		} இல்லையெனில் {
			கட்டமைப்பு string_list list = STRING_LIST_INIT_DUP;

			git_var->multiread(&list);
			எனில் (argc == 1 && !list.nr) {
				string_list_clear(&list, 0);
				திரும்பு 1;
			}
			க்கு (size_t j = 0; j < list.nr; j++) {
				எனில் (argc == 1)
					printf("%s%c", list.பொருட்கள்[j].string,
					       term);
				இல்லையெனில்
					printf("%s%c%s%c", git_var->name, delim,
					       list.பொருட்கள்[j].string, term);
			}
			string_list_clear(&list, 0);
		}
	}

	திரும்பு 0;
}
