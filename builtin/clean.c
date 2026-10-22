/*
 * "git clean" builtin command
 *
 * Copyright (C) 2007 Shawn Bohrer
 *
 * Based on git-clean.sh by Pavel Roskin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "abspath.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "read-cache-ll.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "column.h"
#അന്തര്ഭാവ "color.h"
#അന്തര്ഭാവ "pathspec.h"
#അന്തര്ഭാവ "help.h"
#അന്തര്ഭാവ "prompt.h"

സ്ഥിതം പൂർണ്ണസംഖ്യ require_force = -1; /* unset */
സ്ഥിതം പൂർണ്ണസംഖ്യ interactive;
സ്ഥിതം ഘടന string_list del_list = STRING_LIST_INIT_DUP;
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ colopts;

സ്ഥിതം സ്ഥിരം അക്ഷരം *സ്ഥിരം builtin_clean_usage[] = {
	N_("git clean [-d] [-f] [-i] [-n] [-q] [-e <pattern>] [-x | -X] [--] [<pathspec>...]"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *സന്ദേശം_remove = N_("Removing %s\n");
സ്ഥിതം സ്ഥിരം അക്ഷരം *സന്ദേശം_would_remove = N_("Would remove %s\n");
സ്ഥിതം സ്ഥിരം അക്ഷരം *സന്ദേശം_skip_git_dir = N_("Skipping repository %s\n");
സ്ഥിതം സ്ഥിരം അക്ഷരം *സന്ദേശം_would_skip_git_dir = N_("Would skip repository %s\n");
സ്ഥിതം സ്ഥിരം അക്ഷരം *സന്ദേശം_warn_remove_failed = N_("failed to remove %s");
സ്ഥിതം സ്ഥിരം അക്ഷരം *സന്ദേശം_warn_lstat_failed = N_("could not lstat %s\n");
സ്ഥിതം സ്ഥിരം അക്ഷരം *സന്ദേശം_skip_cwd = N_("Refusing to remove current working directory\n");
സ്ഥിതം സ്ഥിരം അക്ഷരം *സന്ദേശം_would_skip_cwd = N_("Would refuse to remove current working directory\n");

എണ്ണൽ color_clean {
	CLEAN_COLOR_RESET = 0,
	CLEAN_COLOR_PLAIN = 1,
	CLEAN_COLOR_PROMPT = 2,
	CLEAN_COLOR_HEADER = 3,
	CLEAN_COLOR_HELP = 4,
	CLEAN_COLOR_ERROR = 5
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *color_interactive_slots[] = {
	[CLEAN_COLOR_ERROR]  = "error",
	[CLEAN_COLOR_HEADER] = "header",
	[CLEAN_COLOR_HELP]   = "help",
	[CLEAN_COLOR_PLAIN]  = "plain",
	[CLEAN_COLOR_PROMPT] = "prompt",
	[CLEAN_COLOR_RESET]  = "reset",
};

സ്ഥിതം എണ്ണൽ git_colorbool clean_use_color = GIT_COLOR_UNKNOWN;
സ്ഥിതം അക്ഷരം clean_colors[][COLOR_MAXLEN] = {
	[CLEAN_COLOR_ERROR] = GIT_COLOR_BOLD_RED,
	[CLEAN_COLOR_HEADER] = GIT_COLOR_BOLD,
	[CLEAN_COLOR_HELP] = GIT_COLOR_BOLD_RED,
	[CLEAN_COLOR_PLAIN] = GIT_COLOR_NORMAL,
	[CLEAN_COLOR_PROMPT] = GIT_COLOR_BOLD_BLUE,
	[CLEAN_COLOR_RESET] = GIT_COLOR_RESET,
};

#define MENU_OPTS_SINGLETON		01
#define MENU_OPTS_IMMEDIATE		02
#define MENU_OPTS_LIST_ONLY		04

ഘടന menu_opts {
	സ്ഥിരം അക്ഷരം *header;
	സ്ഥിരം അക്ഷരം *prompt;
	പൂർണ്ണസംഖ്യ flags;
};

#define MENU_RETURN_NO_LOOP		10

ഘടന menu_item {
	അക്ഷരം hotkey;
	സ്ഥിരം അക്ഷരം *title;
	പൂർണ്ണസംഖ്യ selected;
	പൂർണ്ണസംഖ്യ (*fn)(ശൂന്യം);
};

എണ്ണൽ menu_stuff_type {
	MENU_STUFF_TYPE_STRING_LIST = 1,
	MENU_STUFF_TYPE_MENU_ITEM
};

ഘടന menu_stuff {
	എണ്ണൽ menu_stuff_type type;
	പൂർണ്ണസംഖ്യ nr;
	ശൂന്യം *stuff;
};

define_list_config_array(color_interactive_slots);

സ്ഥിതം പൂർണ്ണസംഖ്യ git_clean_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
			    സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	സ്ഥിരം അക്ഷരം *slot_name;

	എങ്കിൽ (starts_with(var, "column."))
		തിരികെ git_column_config(var, മൂല്യം, "clean", &colopts);

	/* honors the color.interactive* config variables which also
	   applied in git-add--interactive and git-stash */
	എങ്കിൽ (!strcmp(var, "color.interactive")) {
		clean_use_color = git_config_colorbool(var, മൂല്യം);
		തിരികെ 0;
	}
	എങ്കിൽ (skip_prefix(var, "color.interactive.", &slot_name)) {
		പൂർണ്ണസംഖ്യ slot = LOOKUP_CONFIG(color_interactive_slots, slot_name);
		എങ്കിൽ (slot < 0)
			തിരികെ 0;
		എങ്കിൽ (!മൂല്യം)
			തിരികെ config_error_nonbool(var);
		തിരികെ color_parse(മൂല്യം, clean_colors[slot]);
	}

	എങ്കിൽ (!strcmp(var, "clean.requireforce")) {
		require_force = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (git_color_config(var, മൂല്യം, cb) < 0)
		തിരികെ -1;

	തിരികെ git_default_config(var, മൂല്യം, ctx, cb);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *clean_get_color(എണ്ണൽ color_clean ix)
{
	എങ്കിൽ (want_color(clean_use_color))
		തിരികെ clean_colors[ix];
	തിരികെ "";
}

സ്ഥിതം ശൂന്യം clean_print_color(എണ്ണൽ color_clean ix)
{
	printf("%s", clean_get_color(ix));
}

സ്ഥിതം പൂർണ്ണസംഖ്യ exclude_cb(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന string_list *exclude_list = opt->മൂല്യം;
	BUG_ON_OPT_NEG(unset);
	string_list_append(exclude_list, arg);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ remove_dirs(ഘടന strbuf *path, സ്ഥിരം അക്ഷരം *prefix, പൂർണ്ണസംഖ്യ force_flag,
		പൂർണ്ണസംഖ്യ dry_run, പൂർണ്ണസംഖ്യ quiet, പൂർണ്ണസംഖ്യ *dir_gone)
{
	DIR *dir;
	ഘടന strbuf quoted = STRBUF_INIT;
	ഘടന strbuf realpath = STRBUF_INIT;
	ഘടന strbuf real_ocwd = STRBUF_INIT;
	ഘടന dirent *e;
	പൂർണ്ണസംഖ്യ res = 0, ret = 0, gone = 1, original_len = path->len, len;
	ഘടന string_list dels = STRING_LIST_INIT_DUP;

	*dir_gone = 1;

	എങ്കിൽ ((force_flag & REMOVE_DIR_KEEP_NESTED_GIT) &&
	    is_nonbare_repository_dir(path)) {
		എങ്കിൽ (!quiet) {
			quote_path(path->buf, prefix, &quoted, 0);
			printf(dry_run ?  _(സന്ദേശം_would_skip_git_dir) : _(സന്ദേശം_skip_git_dir),
					quoted.buf);
		}

		*dir_gone = 0;
		പോകുക out;
	}

	dir = opendir(path->buf);
	എങ്കിൽ (!dir) {
		/* an empty dir could be removed even if it is unreadble */
		res = dry_run ? 0 : rmdir(path->buf);
		എങ്കിൽ (res) {
			പൂർണ്ണസംഖ്യ saved_errno = errno;
			quote_path(path->buf, prefix, &quoted, 0);
			errno = saved_errno;
			warning_errno(_(സന്ദേശം_warn_remove_failed), quoted.buf);
			*dir_gone = 0;
		}
		ret = res;
		പോകുക out;
	}

	strbuf_complete(path, '/');

	len = path->len;
	ആകുമ്പോൾ ((e = readdir_skip_dot_and_dotdot(dir)) != ശൂന്യ) {
		ഘടന stat st;

		strbuf_setlen(path, len);
		strbuf_addstr(path, e->d_name);
		എങ്കിൽ (lstat(path->buf, &st))
			warning_errno(_(സന്ദേശം_warn_lstat_failed), path->buf);
		അല്ലെങ്കിൽ എങ്കിൽ (S_ISDIR(st.st_mode)) {
			എങ്കിൽ (remove_dirs(path, prefix, force_flag, dry_run, quiet, &gone))
				ret = 1;
			എങ്കിൽ (gone) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} അല്ലെങ്കിൽ
				*dir_gone = 0;
			തുടരുക;
		} അല്ലെങ്കിൽ {
			res = dry_run ? 0 : unlink(path->buf);
			എങ്കിൽ (!res) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} അല്ലെങ്കിൽ {
				പൂർണ്ണസംഖ്യ saved_errno = errno;
				quote_path(path->buf, prefix, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(സന്ദേശം_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
			തുടരുക;
		}

		/* path too long, stat fails, or non-directory still exists */
		*dir_gone = 0;
		ret = 1;
		നിർത്തുക;
	}
	closedir(dir);

	strbuf_setlen(path, original_len);

	എങ്കിൽ (*dir_gone) {
		/*
		 * Normalize path components in path->buf, e.g. change '\' to
		 * '/' on Windows.
		 */
		strbuf_realpath(&realpath, path->buf, 1);

		/*
		 * path and realpath are absolute; for comparison, we would
		 * like to transform startup_info->original_cwd to an absolute
		 * path too.
		 */
		 എങ്കിൽ (startup_info->original_cwd)
			 strbuf_realpath(&real_ocwd,
					 startup_info->original_cwd, 1);

		എങ്കിൽ (!strbuf_cmp(&realpath, &real_ocwd)) {
			printf("%s", dry_run ? _(സന്ദേശം_would_skip_cwd) : _(സന്ദേശം_skip_cwd));
			*dir_gone = 0;
		} അല്ലെങ്കിൽ {
			res = dry_run ? 0 : rmdir(path->buf);
			എങ്കിൽ (!res)
				*dir_gone = 1;
			അല്ലെങ്കിൽ {
				പൂർണ്ണസംഖ്യ saved_errno = errno;
				quote_path(path->buf, prefix, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(സന്ദേശം_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
		}
	}

	എങ്കിൽ (!*dir_gone && !quiet) {
		പൂർണ്ണസംഖ്യ i;
		വേണ്ടി (i = 0; i < dels.nr; i++)
			printf(dry_run ?  _(സന്ദേശം_would_remove) : _(സന്ദേശം_remove), dels.ഇനങ്ങൾ[i].string);
	}
out:
	strbuf_release(&realpath);
	strbuf_release(&real_ocwd);
	strbuf_release(&quoted);
	string_list_clear(&dels, 0);
	തിരികെ ret;
}

സ്ഥിതം ശൂന്യം pretty_print_dels(ശൂന്യം)
{
	ഘടന string_list list = STRING_LIST_INIT_DUP;
	ഘടന string_list_item *item;
	ഘടന strbuf buf = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *qname;
	ഘടന column_options copts;

	for_each_string_list_item(item, &del_list) {
		qname = quote_path(item->string, ശൂന്യ, &buf, 0);
		string_list_append(&list, qname);
	}

	/*
	 * always enable column display, we only consult column.*
	 * about layout strategy and stuff
	 */
	colopts = (colopts & ~COL_ENABLE_MASK) | COL_ENABLED;
	memset(&copts, 0, വലുപ്പം(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(&list, colopts, &copts);
	strbuf_release(&buf);
	string_list_clear(&list, 0);
}

സ്ഥിതം ശൂന്യം pretty_print_menus(ഘടന string_list *menu_list)
{
	unsigned പൂർണ്ണസംഖ്യ local_colopts = 0;
	ഘടന column_options copts;

	local_colopts = COL_ENABLED | COL_ROW;
	memset(&copts, 0, വലുപ്പം(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(menu_list, local_colopts, &copts);
}

സ്ഥിതം ശൂന്യം prompt_help_cmd(പൂർണ്ണസംഖ്യ singleton)
{
	clean_print_color(CLEAN_COLOR_HELP);
	printf(singleton ?
		  _("Prompt help:\n"
		    "1          - select a numbered item\n"
		    "foo        - select item based on unique prefix\n"
		    "           - (empty) select nothing\n") :
		  _("Prompt help:\n"
		    "1          - select a single item\n"
		    "3-5        - select a range of items\n"
		    "2-3,6-9    - select multiple ranges\n"
		    "foo        - select item based on unique prefix\n"
		    "-...       - unselect specified items\n"
		    "*          - choose all items\n"
		    "           - (empty) finish selecting\n"));
	clean_print_color(CLEAN_COLOR_RESET);
}

/*
 * display menu stuff with number prefix and hotkey highlight
 */
സ്ഥിതം ശൂന്യം print_highlight_menu_stuff(ഘടന menu_stuff *stuff, പൂർണ്ണസംഖ്യ **chosen)
{
	ഘടന string_list menu_list = STRING_LIST_INIT_DUP;
	ഘടന strbuf menu = STRBUF_INIT;
	ഘടന menu_item *menu_item;
	ഘടന string_list_item *string_list_item;
	പൂർണ്ണസംഖ്യ i;

	തിരഞ്ഞെടുപ്പ് (stuff->type) {
	സ്ഥിരസ്ഥിതി:
		die("Bad type of menu_stuff when print menu");
	അവസ്ഥ MENU_STUFF_TYPE_MENU_ITEM:
		menu_item = (ഘടന menu_item *)stuff->stuff;
		വേണ്ടി (i = 0; i < stuff->nr; i++, menu_item++) {
			സ്ഥിരം അക്ഷരം *p;
			പൂർണ്ണസംഖ്യ highlighted = 0;

			p = menu_item->title;
			എങ്കിൽ ((*chosen)[i] < 0)
				(*chosen)[i] = menu_item->selected ? 1 : 0;
			strbuf_addf(&menu, "%s%2d: ", (*chosen)[i] ? "*" : " ", i+1);
			വേണ്ടി (; *p; p++) {
				എങ്കിൽ (!highlighted && *p == menu_item->hotkey) {
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_PROMPT));
					strbuf_addch(&menu, *p);
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_RESET));
					highlighted = 1;
				} അല്ലെങ്കിൽ {
					strbuf_addch(&menu, *p);
				}
			}
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
		}
		നിർത്തുക;
	അവസ്ഥ MENU_STUFF_TYPE_STRING_LIST:
		i = 0;
		for_each_string_list_item(string_list_item, (ഘടന string_list *)stuff->stuff) {
			എങ്കിൽ ((*chosen)[i] < 0)
				(*chosen)[i] = 0;
			strbuf_addf(&menu, "%s%2d: %s",
				    (*chosen)[i] ? "*" : " ", i+1, string_list_item->string);
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
			i++;
		}
		നിർത്തുക;
	}

	pretty_print_menus(&menu_list);

	strbuf_release(&menu);
	string_list_clear(&menu_list, 0);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ find_unique(സ്ഥിരം അക്ഷരം *choice, ഘടന menu_stuff *menu_stuff)
{
	ഘടന menu_item *menu_item;
	ഘടന string_list_item *string_list_item;
	പൂർണ്ണസംഖ്യ i, len, found = 0;

	len = strlen(choice);
	തിരഞ്ഞെടുപ്പ് (menu_stuff->type) {
	സ്ഥിരസ്ഥിതി:
		die("Bad type of menu_stuff when parse choice");
	അവസ്ഥ MENU_STUFF_TYPE_MENU_ITEM:

		menu_item = (ഘടന menu_item *)menu_stuff->stuff;
		വേണ്ടി (i = 0; i < menu_stuff->nr; i++, menu_item++) {
			എങ്കിൽ (len == 1 && *choice == menu_item->hotkey) {
				found = i + 1;
				നിർത്തുക;
			}
			എങ്കിൽ (!strncasecmp(choice, menu_item->title, len)) {
				എങ്കിൽ (found) {
					എങ്കിൽ (len == 1) {
						/* continue for hotkey matching */
						found = -1;
					} അല്ലെങ്കിൽ {
						found = 0;
						നിർത്തുക;
					}
				} അല്ലെങ്കിൽ {
					found = i + 1;
				}
			}
		}
		നിർത്തുക;
	അവസ്ഥ MENU_STUFF_TYPE_STRING_LIST:
		string_list_item = ((ഘടന string_list *)menu_stuff->stuff)->ഇനങ്ങൾ;
		വേണ്ടി (i = 0; i < menu_stuff->nr; i++, string_list_item++) {
			എങ്കിൽ (!strncasecmp(choice, string_list_item->string, len)) {
				എങ്കിൽ (found) {
					found = 0;
					നിർത്തുക;
				}
				found = i + 1;
			}
		}
		നിർത്തുക;
	}
	തിരികെ found;
}

/*
 * Parse user input, and return choice(s) for menu (menu_stuff).
 *
 * Input
 *     (for single choice)
 *         1          - select a numbered item
 *         foo        - select item based on menu title
 *                    - (empty) select nothing
 *
 *     (for multiple choice)
 *         1          - select a single item
 *         3-5        - select a range of items
 *         2-3,6-9    - select multiple ranges
 *         foo        - select item based on menu title
 *         -...       - unselect specified items
 *         *          - choose all items
 *                    - (empty) finish selecting
 *
 * The parse result will be saved in array **chosen, and
 * return number of total selections.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ parse_choice(ഘടന menu_stuff *menu_stuff,
			പൂർണ്ണസംഖ്യ is_single,
			അക്ഷരം *നിവേശം,
			പൂർണ്ണസംഖ്യ **chosen)
{
	ഘടന string_list choice = STRING_LIST_INIT_NODUP;
	ഘടന string_list_item *item;
	പൂർണ്ണസംഖ്യ nr = 0;
	പൂർണ്ണസംഖ്യ i;

	string_list_split_in_place_f(&choice, നിവേശം,
				     is_single ? "\n" : ", ", -1,
				     STRING_LIST_SPLIT_TRIM);

	for_each_string_list_item(item, &choice) {
		സ്ഥിരം അക്ഷരം *string;
		പൂർണ്ണസംഖ്യ choose;
		പൂർണ്ണസംഖ്യ bottom = 0, മുകൾ = 0;
		പൂർണ്ണസംഖ്യ is_range, is_സംഖ്യ;

		string = item->string;
		എങ്കിൽ (!*string)
			തുടരുക;

		/* Input that begins with '-'; unchoose */
		എങ്കിൽ (string[0] == '-') {
			choose = 0;
			string++;
		} അല്ലെങ്കിൽ {
			choose = 1;
		}

		is_range = 0;
		is_സംഖ്യ = 1;
		വേണ്ടി (സ്ഥിരം അക്ഷരം *p = string; *p; p++) {
			എങ്കിൽ ('-' == *p) {
				എങ്കിൽ (!is_range) {
					is_range = 1;
					is_സംഖ്യ = 0;
				} അല്ലെങ്കിൽ {
					is_സംഖ്യ = 0;
					is_range = 0;
					നിർത്തുക;
				}
			} അല്ലെങ്കിൽ എങ്കിൽ (!isdigit(*p)) {
				is_സംഖ്യ = 0;
				is_range = 0;
				നിർത്തുക;
			}
		}

		എങ്കിൽ (is_സംഖ്യ) {
			bottom = atoi(string);
			മുകൾ = bottom;
		} അല്ലെങ്കിൽ എങ്കിൽ (is_range) {
			bottom = atoi(string);
			/* a range can be specified like 5-7 or 5- */
			എങ്കിൽ (!*(strchr(string, '-') + 1))
				മുകൾ = menu_stuff->nr;
			അല്ലെങ്കിൽ
				മുകൾ = atoi(strchr(string, '-') + 1);
		} അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(string, "*")) {
			bottom = 1;
			മുകൾ = menu_stuff->nr;
		} അല്ലെങ്കിൽ {
			bottom = find_unique(string, menu_stuff);
			മുകൾ = bottom;
		}

		എങ്കിൽ (മുകൾ <= 0 || bottom <= 0 || മുകൾ > menu_stuff->nr || bottom > മുകൾ ||
		    (is_single && bottom != മുകൾ)) {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf(_("Huh (%s)?\n"), string);
			clean_print_color(CLEAN_COLOR_RESET);
			തുടരുക;
		}

		വേണ്ടി (i = bottom; i <= മുകൾ; i++)
			(*chosen)[i-1] = choose;
	}

	string_list_clear(&choice, 0);

	വേണ്ടി (i = 0; i < menu_stuff->nr; i++)
		nr += (*chosen)[i];
	തിരികെ nr;
}

/*
 * Implement a git-add-interactive compatible UI, which is borrowed
 * from add-interactive.c.
 *
 * Return value:
 *
 *   - Return an array of integers
 *   - , and it is up to you to free the allocated memory.
 *   - The array ends with EOF.
 *   - If user pressed CTRL-D (i.e. EOF), no selection returned.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ *list_and_choose(ഘടന menu_opts *opts, ഘടന menu_stuff *stuff)
{
	ഘടന strbuf choice = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ *chosen, *result;
	പൂർണ്ണസംഖ്യ nr = 0;
	പൂർണ്ണസംഖ്യ eof = 0;
	പൂർണ്ണസംഖ്യ i;

	ALLOC_ARRAY(chosen, stuff->nr);
	/* set chosen as uninitialized */
	വേണ്ടി (i = 0; i < stuff->nr; i++)
		chosen[i] = -1;

	വേണ്ടി (;;) {
		എങ്കിൽ (opts->header) {
			printf_ln("%s%s%s",
				  clean_get_color(CLEAN_COLOR_HEADER),
				  _(opts->header),
				  clean_get_color(CLEAN_COLOR_RESET));
		}

		/* chosen will be initialized by print_highlight_menu_stuff */
		print_highlight_menu_stuff(stuff, &chosen);

		എങ്കിൽ (opts->flags & MENU_OPTS_LIST_ONLY)
			നിർത്തുക;

		എങ്കിൽ (opts->prompt) {
			printf("%s%s%s%s",
			       clean_get_color(CLEAN_COLOR_PROMPT),
			       _(opts->prompt),
			       opts->flags & MENU_OPTS_SINGLETON ? "> " : ">> ",
			       clean_get_color(CLEAN_COLOR_RESET));
		}

		എങ്കിൽ (git_read_വരി_interactively(&choice) == EOF) {
			eof = 1;
			നിർത്തുക;
		}

		/* help for prompt */
		എങ്കിൽ (!strcmp(choice.buf, "?")) {
			prompt_help_cmd(opts->flags & MENU_OPTS_SINGLETON);
			തുടരുക;
		}

		/* for a multiple-choice menu, press ENTER (empty) will return back */
		എങ്കിൽ (!(opts->flags & MENU_OPTS_SINGLETON) && !choice.len)
			നിർത്തുക;

		nr = parse_choice(stuff,
				  opts->flags & MENU_OPTS_SINGLETON,
				  choice.buf,
				  &chosen);

		എങ്കിൽ (opts->flags & MENU_OPTS_SINGLETON) {
			എങ്കിൽ (nr)
				നിർത്തുക;
		} അല്ലെങ്കിൽ എങ്കിൽ (opts->flags & MENU_OPTS_IMMEDIATE) {
			നിർത്തുക;
		}
	}

	എങ്കിൽ (eof) {
		result = xmalloc(വലുപ്പം(പൂർണ്ണസംഖ്യ));
		*result = EOF;
	} അല്ലെങ്കിൽ {
		പൂർണ്ണസംഖ്യ j = 0;

		/*
		 * recalculate nr, if return back from menu directly with
		 * default selections.
		 */
		എങ്കിൽ (!nr) {
			വേണ്ടി (i = 0; i < stuff->nr; i++)
				nr += chosen[i];
		}

		CALLOC_ARRAY(result, st_add(nr, 1));
		വേണ്ടി (i = 0; i < stuff->nr && j < nr; i++) {
			എങ്കിൽ (chosen[i])
				result[j++] = i;
		}
		result[j] = EOF;
	}

	സ്വതന്ത്രം(chosen);
	strbuf_release(&choice);
	തിരികെ result;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ clean_cmd(ശൂന്യം)
{
	തിരികെ MENU_RETURN_NO_LOOP;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ filter_by_patterns_cmd(ശൂന്യം)
{
	ഘടന dir_struct dir = DIR_INIT;
	ഘടന strbuf confirm = STRBUF_INIT;
	ഘടന pattern_list *pl;
	പൂർണ്ണസംഖ്യ changed = -1, i;

	വേണ്ടി (;;) {
		ഘടന string_list ignore_list = STRING_LIST_INIT_NODUP;
		ഘടന string_list_item *item;

		എങ്കിൽ (!del_list.nr)
			നിർത്തുക;

		എങ്കിൽ (changed)
			pretty_print_dels();

		clean_print_color(CLEAN_COLOR_PROMPT);
		printf(_("Input ignore patterns>> "));
		clean_print_color(CLEAN_COLOR_RESET);
		എങ്കിൽ (git_read_വരി_interactively(&confirm) == EOF)
			putchar('\n');

		/* quit filter_by_pattern mode if press ENTER or Ctrl-D */
		എങ്കിൽ (!confirm.len)
			നിർത്തുക;

		pl = add_pattern_list(&dir, EXC_CMDL, "manual exclude");

		string_list_split_in_place_f(&ignore_list, confirm.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);

		വേണ്ടി (i = 0; i < ignore_list.nr; i++) {
			item = &ignore_list.ഇനങ്ങൾ[i];
			എങ്കിൽ (!*item->string)
				തുടരുക;
			add_pattern(item->string, "", 0, pl, -(i+1));
		}

		changed = 0;
		for_each_string_list_item(item, &del_list) {
			പൂർണ്ണസംഖ്യ dtype = DT_UNKNOWN;

			എങ്കിൽ (is_excluded(&dir, the_repository->index, item->string, &dtype)) {
				*item->string = '\0';
				changed++;
			}
		}

		എങ്കിൽ (changed) {
			string_list_remove_empty_ഇനങ്ങൾ(&del_list, 0);
		} അല്ലെങ്കിൽ {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf_ln(_("WARNING: Cannot find items matched by: %s"), confirm.buf);
			clean_print_color(CLEAN_COLOR_RESET);
		}

		string_list_clear(&ignore_list, 0);
		dir_clear(&dir);
	}

	strbuf_release(&confirm);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ select_by_numbers_cmd(ശൂന്യം)
{
	ഘടന menu_opts menu_opts;
	ഘടന menu_stuff menu_stuff;
	ഘടന string_list_item *ഇനങ്ങൾ;
	പൂർണ്ണസംഖ്യ *chosen;
	പൂർണ്ണസംഖ്യ i, j;

	menu_opts.header = ശൂന്യ;
	menu_opts.prompt = N_("Select items to delete");
	menu_opts.flags = 0;

	menu_stuff.type = MENU_STUFF_TYPE_STRING_LIST;
	menu_stuff.stuff = &del_list;
	menu_stuff.nr = del_list.nr;

	chosen = list_and_choose(&menu_opts, &menu_stuff);
	ഇനങ്ങൾ = del_list.ഇനങ്ങൾ;
	വേണ്ടി (i = 0, j = 0; i < del_list.nr; i++) {
		എങ്കിൽ (i < chosen[j]) {
			*(ഇനങ്ങൾ[i].string) = '\0';
		} അല്ലെങ്കിൽ എങ്കിൽ (i == chosen[j]) {
			/* delete selected item */
			j++;
			തുടരുക;
		} അല്ലെങ്കിൽ {
			/* end of chosen (chosen[j] == EOF), won't delete */
			*(ഇനങ്ങൾ[i].string) = '\0';
		}
	}

	string_list_remove_empty_ഇനങ്ങൾ(&del_list, 0);

	സ്വതന്ത്രം(chosen);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ ask_each_cmd(ശൂന്യം)
{
	ഘടന strbuf confirm = STRBUF_INIT;
	ഘടന strbuf buf = STRBUF_INIT;
	ഘടന string_list_item *item;
	സ്ഥിരം അക്ഷരം *qname;
	പൂർണ്ണസംഖ്യ changed = 0, eof = 0;

	for_each_string_list_item(item, &del_list) {
		/* Ctrl-D should stop removing files */
		എങ്കിൽ (!eof) {
			qname = quote_path(item->string, ശൂന്യ, &buf, 0);
			/* TRANSLATORS: Make sure to keep [y/N] as is */
			printf(_("Remove %s [y/N]? "), qname);
			എങ്കിൽ (git_read_വരി_interactively(&confirm) == EOF) {
				putchar('\n');
				eof = 1;
			}
		}
		എങ്കിൽ (!confirm.len || strncasecmp(confirm.buf, "yes", confirm.len)) {
			*item->string = '\0';
			changed++;
		}
	}

	എങ്കിൽ (changed)
		string_list_remove_empty_ഇനങ്ങൾ(&del_list, 0);

	strbuf_release(&buf);
	strbuf_release(&confirm);
	തിരികെ MENU_RETURN_NO_LOOP;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ quit_cmd(ശൂന്യം)
{
	string_list_clear(&del_list, 0);
	printf(_("Bye.\n"));
	തിരികെ MENU_RETURN_NO_LOOP;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ help_cmd(ശൂന്യം)
{
	clean_print_color(CLEAN_COLOR_HELP);
	printf_ln(_(
		    "clean               - start cleaning\n"
		    "filter by pattern   - exclude items from deletion\n"
		    "select by numbers   - select items to be deleted by numbers\n"
		    "ask each            - confirm each deletion (like \"rm -i\")\n"
		    "quit                - stop cleaning\n"
		    "help                - this screen\n"
		    "?                   - help for prompt selection"
		   ));
	clean_print_color(CLEAN_COLOR_RESET);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം interactive_main_loop(ശൂന്യം)
{
	ആകുമ്പോൾ (del_list.nr) {
		ഘടന menu_opts menu_opts;
		ഘടന menu_stuff menu_stuff;
		ഘടന menu_item menus[] = {
			{'c', "clean",			0, clean_cmd},
			{'f', "filter by pattern",	0, filter_by_patterns_cmd},
			{'s', "select by numbers",	0, select_by_numbers_cmd},
			{'a', "ask each",		0, ask_each_cmd},
			{'q', "quit",			0, quit_cmd},
			{'h', "help",			0, help_cmd},
		};
		പൂർണ്ണസംഖ്യ *chosen;

		menu_opts.header = N_("*** Commands ***");
		menu_opts.prompt = N_("What now");
		menu_opts.flags = MENU_OPTS_SINGLETON;

		menu_stuff.type = MENU_STUFF_TYPE_MENU_ITEM;
		menu_stuff.stuff = menus;
		menu_stuff.nr = വലുപ്പം(menus) / വലുപ്പം(ഘടന menu_item);

		clean_print_color(CLEAN_COLOR_HEADER);
		printf_ln(Q_("Would remove the following item:",
			     "Would remove the following items:",
			     del_list.nr));
		clean_print_color(CLEAN_COLOR_RESET);

		pretty_print_dels();

		chosen = list_and_choose(&menu_opts, &menu_stuff);

		എങ്കിൽ (*chosen != EOF) {
			പൂർണ്ണസംഖ്യ ret;
			ret = menus[*chosen].fn();
			എങ്കിൽ (ret != MENU_RETURN_NO_LOOP) {
				FREE_AND_NULL(chosen);
				എങ്കിൽ (!del_list.nr) {
					clean_print_color(CLEAN_COLOR_ERROR);
					printf_ln(_("No more files to clean, exiting."));
					clean_print_color(CLEAN_COLOR_RESET);
					നിർത്തുക;
				}
				തുടരുക;
			}
		} അല്ലെങ്കിൽ {
			quit_cmd();
		}

		FREE_AND_NULL(chosen);
		നിർത്തുക;
	}
}

സ്ഥിതം ശൂന്യം correct_untracked_entries(ഘടന dir_struct *dir)
{
	പൂർണ്ണസംഖ്യ src, dst, ign;

	വേണ്ടി (src = dst = ign = 0; src < dir->nr; src++) {
		/* skip paths in ignored[] that cannot be inside entries[src] */
		ആകുമ്പോൾ (ign < dir->ignored_nr &&
		       0 <= cmp_dir_entry(&dir->entries[src], &dir->ignored[ign]))
			ign++;

		എങ്കിൽ (ign < dir->ignored_nr &&
		    check_dir_entry_contains(dir->entries[src], dir->ignored[ign])) {
			/* entries[src] contains an ignored path, so we drop it */
			സ്വതന്ത്രം(dir->entries[src]);
		} അല്ലെങ്കിൽ {
			ഘടന dir_entry *ent = dir->entries[src++];

			/* entries[src] does not contain an ignored path, so we keep it */
			dir->entries[dst++] = ent;

			/* then discard paths in entries[] contained inside entries[src] */
			ആകുമ്പോൾ (src < dir->nr &&
			       check_dir_entry_contains(ent, dir->entries[src]))
				സ്വതന്ത്രം(dir->entries[src++]);

			/* compensate for the outer loop's loop control */
			src--;
		}
	}
	dir->nr = dst;
}

പൂർണ്ണസംഖ്യ cmd_clean(പൂർണ്ണസംഖ്യ argc,
	      സ്ഥിരം അക്ഷരം **argv,
	      സ്ഥിരം അക്ഷരം *prefix,
	      ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ i, res;
	പൂർണ്ണസംഖ്യ dry_run = 0, remove_directories = 0, quiet = 0, ignored = 0;
	പൂർണ്ണസംഖ്യ ignored_only = 0, force = 0, errors = 0, gone = 1;
	പൂർണ്ണസംഖ്യ rm_flags = REMOVE_DIR_KEEP_NESTED_GIT;
	ഘടന strbuf abs_path = STRBUF_INIT;
	ഘടന dir_struct dir = DIR_INIT;
	ഘടന pathspec pathspec;
	ഘടന strbuf buf = STRBUF_INIT;
	ഘടന string_list exclude_list = STRING_LIST_INIT_NODUP;
	ഘടന pattern_list *pl;
	ഘടന string_list_item *item;
	സ്ഥിരം അക്ഷരം *qname;
	ഘടന option options[] = {
		OPT__QUIET(&quiet, N_("do not print names of files removed")),
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('i', "interactive", &interactive, N_("interactive cleaning")),
		OPT_BOOL('d', ശൂന്യ, &remove_directories,
				N_("remove whole directories")),
		OPT_CALLBACK_F('e', "exclude", &exclude_list, N_("pattern"),
		  N_("add <pattern> to ignore rules"), PARSE_OPT_NONEG, exclude_cb),
		OPT_BOOL('x', ശൂന്യ, &ignored, N_("remove ignored files, too")),
		OPT_BOOL('X', ശൂന്യ, &ignored_only,
				N_("remove only ignored files")),
		OPT_END()
	};

	repo_config(the_repository, git_clean_config, ശൂന്യ);

	argc = parse_options(argc, argv, prefix, options, builtin_clean_usage,
			     0);

	എങ്കിൽ (require_force != 0 && !force && !interactive && !dry_run)
		die(_("clean.requireForce is true and -f not given: refusing to clean"));

	എങ്കിൽ (force > 1)
		rm_flags = 0;
	അല്ലെങ്കിൽ
		dir.flags |= DIR_SKIP_NESTED_GIT;

	dir.flags |= DIR_SHOW_OTHER_DIRECTORIES;

	എങ്കിൽ (ignored && ignored_only)
		die(_("options '%s' and '%s' cannot be used together"), "-x", "-X");
	എങ്കിൽ (!ignored)
		setup_standard_excludes(&dir);
	എങ്കിൽ (ignored_only)
		dir.flags |= DIR_SHOW_IGNORED;

	എങ്കിൽ (argc) {
		/*
		 * Remaining args implies pathspecs specified, and we should
		 * recurse within those.
		 */
		remove_directories = 1;
	}

	എങ്കിൽ (remove_directories && !ignored_only) {
		/*
		 * We need to know about ignored files too:
		 *
		 * If (ignored), then we will delete ignored files as well.
		 *
		 * If (!ignored), then even though we not are doing
		 * anything with ignored files, we need to know about them
		 * so that we can avoid deleting a directory of untracked
		 * files that also contains an ignored file within it.
		 *
		 * For the (!ignored) case, since we only need to avoid
		 * deleting ignored files, we can set
		 * DIR_SHOW_IGNORED_TOO_MODE_MATCHING in order to avoid
		 * recursing into a directory which is itself ignored.
		 */
		dir.flags |= DIR_SHOW_IGNORED_TOO;
		എങ്കിൽ (!ignored)
			dir.flags |= DIR_SHOW_IGNORED_TOO_MODE_MATCHING;

		/*
		 * Let the fill_directory() machinery know that we aren't
		 * just recursing to collect the ignored files; we want all
		 * the untracked ones so that we can delete them.  (Note:
		 * we could also set DIR_KEEP_UNTRACKED_CONTENTS when
		 * ignored_only is true, since DIR_KEEP_UNTRACKED_CONTENTS
		 * only has effect in combination with DIR_SHOW_IGNORED_TOO.  It makes
		 * the code clearer to exclude it, though.
		 */
		dir.flags |= DIR_KEEP_UNTRACKED_CONTENTS;
	}

	prepare_repo_settings(the_repository);
	the_repository->settings.command_requires_full_index = 0;

	എങ്കിൽ (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	വേണ്ടി (i = 0; i < exclude_list.nr; i++)
		add_pattern(exclude_list.ഇനങ്ങൾ[i].string, "", 0, pl, -(i+1));

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	fill_directory(&dir, the_repository->index, &pathspec);
	correct_untracked_entries(&dir);

	വേണ്ടി (i = 0; i < dir.nr; i++) {
		ഘടന dir_entry *ent = dir.entries[i];
		ഘടന stat st;
		സ്ഥിരം അക്ഷരം *rel;

		എങ്കിൽ (!index_name_is_other(the_repository->index, ent->name, ent->len))
			തുടരുക;

		എങ്കിൽ (lstat(ent->name, &st))
			die_errno("Cannot lstat '%s'", ent->name);

		എങ്കിൽ (S_ISDIR(st.st_mode) && !remove_directories)
			തുടരുക;

		rel = relative_path(ent->name, prefix, &buf);
		string_list_append(&del_list, rel);
	}

	dir_clear(&dir);

	എങ്കിൽ (interactive && del_list.nr > 0)
		interactive_main_loop();

	for_each_string_list_item(item, &del_list) {
		ഘടന stat st;

		strbuf_reset(&abs_path);
		എങ്കിൽ (prefix)
			strbuf_addstr(&abs_path, prefix);

		strbuf_addstr(&abs_path, item->string);

		/*
		 * we might have removed this as part of earlier
		 * recursive directory removal, so lstat() here could
		 * fail with ENOENT.
		 */
		എങ്കിൽ (lstat(abs_path.buf, &st))
			തുടരുക;

		എങ്കിൽ (S_ISDIR(st.st_mode)) {
			എങ്കിൽ (remove_dirs(&abs_path, prefix, rm_flags, dry_run, quiet, &gone))
				errors++;
			എങ്കിൽ (gone && !quiet) {
				qname = quote_path(item->string, ശൂന്യ, &buf, 0);
				printf(dry_run ? _(സന്ദേശം_would_remove) : _(സന്ദേശം_remove), qname);
			}
		} അല്ലെങ്കിൽ {
			res = dry_run ? 0 : unlink(abs_path.buf);
			എങ്കിൽ (res) {
				പൂർണ്ണസംഖ്യ saved_errno = errno;
				qname = quote_path(item->string, ശൂന്യ, &buf, 0);
				errno = saved_errno;
				warning_errno(_(സന്ദേശം_warn_remove_failed), qname);
				errors++;
			} അല്ലെങ്കിൽ എങ്കിൽ (!quiet) {
				qname = quote_path(item->string, ശൂന്യ, &buf, 0);
				printf(dry_run ? _(സന്ദേശം_would_remove) : _(സന്ദേശം_remove), qname);
			}
		}
	}

	strbuf_release(&abs_path);
	strbuf_release(&buf);
	string_list_clear(&del_list, 0);
	string_list_clear(&exclude_list, 0);
	clear_pathspec(&pathspec);
	തിരികെ (errors != 0);
}
