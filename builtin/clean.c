/*
 * "git clean" builtin command
 *
 * Copyright (C) 2007 Shawn Bohrer
 *
 * Based on git-clean.sh by Pavel Roskin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "read-cache-ll.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "column.h"
#ಅನ್ತರ್ಭಾವ "color.h"
#ಅನ್ತರ್ಭಾವ "pathspec.h"
#ಅನ್ತರ್ಭಾವ "help.h"
#ಅನ್ತರ್ಭಾವ "prompt.h"

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ require_force = -1; /* unset */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ interactive;
ಸ್ಥಿರ ಸಂರಚನಾ string_list del_list = STRING_LIST_INIT_DUP;
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ colopts;

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ builtin_clean_usage[] = {
	N_("git clean [-d] [-f] [-i] [-n] [-q] [-e <pattern>] [-x | -X] [--] [<pathspec>...]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ_remove = N_("Removing %s\n");
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ_would_remove = N_("Would remove %s\n");
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ_skip_git_dir = N_("Skipping repository %s\n");
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ_would_skip_git_dir = N_("Would skip repository %s\n");
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ_warn_remove_failed = N_("failed to remove %s");
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ_warn_lstat_failed = N_("could not lstat %s\n");
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ_skip_cwd = N_("Refusing to remove current working directory\n");
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ_would_skip_cwd = N_("Would refuse to remove current working directory\n");

ಗಣನಾ color_clean {
	CLEAN_COLOR_RESET = 0,
	CLEAN_COLOR_PLAIN = 1,
	CLEAN_COLOR_PROMPT = 2,
	CLEAN_COLOR_HEADER = 3,
	CLEAN_COLOR_HELP = 4,
	CLEAN_COLOR_ERROR = 5
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *color_interactive_slots[] = {
	[CLEAN_COLOR_ERROR]  = "error",
	[CLEAN_COLOR_HEADER] = "header",
	[CLEAN_COLOR_HELP]   = "help",
	[CLEAN_COLOR_PLAIN]  = "plain",
	[CLEAN_COLOR_PROMPT] = "prompt",
	[CLEAN_COLOR_RESET]  = "reset",
};

ಸ್ಥಿರ ಗಣನಾ git_colorbool clean_use_color = GIT_COLOR_UNKNOWN;
ಸ್ಥಿರ ವರ್ಣ clean_colors[][COLOR_MAXLEN] = {
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

ಸಂರಚನಾ menu_opts {
	ಧ್ರುವ ವರ್ಣ *header;
	ಧ್ರುವ ವರ್ಣ *prompt;
	ಪೂರ್ಣಾಙ್ಕ flags;
};

#define MENU_RETURN_NO_LOOP		10

ಸಂರಚನಾ menu_item {
	ವರ್ಣ hotkey;
	ಧ್ರುವ ವರ್ಣ *title;
	ಪೂರ್ಣಾಙ್ಕ selected;
	ಪೂರ್ಣಾಙ್ಕ (*fn)(ಶೂನ್ಯ);
};

ಗಣನಾ menu_stuff_type {
	MENU_STUFF_TYPE_STRING_LIST = 1,
	MENU_STUFF_TYPE_MENU_ITEM
};

ಸಂರಚನಾ menu_stuff {
	ಗಣನಾ menu_stuff_type type;
	ಪೂರ್ಣಾಙ್ಕ nr;
	ಶೂನ್ಯ *stuff;
};

define_list_config_array(color_interactive_slots);

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_clean_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
			    ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಧ್ರುವ ವರ್ಣ *slot_name;

	ಯದಿ (starts_with(var, "column."))
		ಪ್ರತ್ಯಾಗಮ git_column_config(var, ಮೌಲ್ಯ, "clean", &colopts);

	/* honors the color.interactive* config variables which also
	   applied in git-add--interactive and git-stash */
	ಯದಿ (!strcmp(var, "color.interactive")) {
		clean_use_color = git_config_colorbool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (skip_prefix(var, "color.interactive.", &slot_name)) {
		ಪೂರ್ಣಾಙ್ಕ slot = LOOKUP_CONFIG(color_interactive_slots, slot_name);
		ಯದಿ (slot < 0)
			ಪ್ರತ್ಯಾಗಮ 0;
		ಯದಿ (!ಮೌಲ್ಯ)
			ಪ್ರತ್ಯಾಗಮ config_error_nonbool(var);
		ಪ್ರತ್ಯಾಗಮ color_parse(ಮೌಲ್ಯ, clean_colors[slot]);
	}

	ಯದಿ (!strcmp(var, "clean.requireforce")) {
		require_force = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (git_color_config(var, ಮೌಲ್ಯ, cb) < 0)
		ಪ್ರತ್ಯಾಗಮ -1;

	ಪ್ರತ್ಯಾಗಮ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *clean_get_color(ಗಣನಾ color_clean ix)
{
	ಯದಿ (want_color(clean_use_color))
		ಪ್ರತ್ಯಾಗಮ clean_colors[ix];
	ಪ್ರತ್ಯಾಗಮ "";
}

ಸ್ಥಿರ ಶೂನ್ಯ clean_print_color(ಗಣನಾ color_clean ix)
{
	printf("%s", clean_get_color(ix));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ exclude_cb(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ string_list *exclude_list = opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_NEG(unset);
	string_list_append(exclude_list, arg);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ remove_dirs(ಸಂರಚನಾ strbuf *path, ಧ್ರುವ ವರ್ಣ *prefix, ಪೂರ್ಣಾಙ್ಕ force_flag,
		ಪೂರ್ಣಾಙ್ಕ dry_run, ಪೂರ್ಣಾಙ್ಕ quiet, ಪೂರ್ಣಾಙ್ಕ *dir_gone)
{
	DIR *dir;
	ಸಂರಚನಾ strbuf quoted = STRBUF_INIT;
	ಸಂರಚನಾ strbuf realpath = STRBUF_INIT;
	ಸಂರಚನಾ strbuf real_ocwd = STRBUF_INIT;
	ಸಂರಚನಾ dirent *e;
	ಪೂರ್ಣಾಙ್ಕ res = 0, ret = 0, gone = 1, original_len = path->len, len;
	ಸಂರಚನಾ string_list dels = STRING_LIST_INIT_DUP;

	*dir_gone = 1;

	ಯದಿ ((force_flag & REMOVE_DIR_KEEP_NESTED_GIT) &&
	    is_nonbare_repository_dir(path)) {
		ಯದಿ (!quiet) {
			quote_path(path->buf, prefix, &quoted, 0);
			printf(dry_run ?  _(ಸಂದೇಶ_would_skip_git_dir) : _(ಸಂದೇಶ_skip_git_dir),
					quoted.buf);
		}

		*dir_gone = 0;
		ಗಚ್ಛ out;
	}

	dir = opendir(path->buf);
	ಯದಿ (!dir) {
		/* an empty dir could be removed even if it is unreadble */
		res = dry_run ? 0 : rmdir(path->buf);
		ಯದಿ (res) {
			ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
			quote_path(path->buf, prefix, &quoted, 0);
			errno = saved_errno;
			warning_errno(_(ಸಂದೇಶ_warn_remove_failed), quoted.buf);
			*dir_gone = 0;
		}
		ret = res;
		ಗಚ್ಛ out;
	}

	strbuf_complete(path, '/');

	len = path->len;
	ಯಾವತ್ ((e = readdir_skip_dot_and_dotdot(dir)) != ರಿಕ್ತ) {
		ಸಂರಚನಾ stat st;

		strbuf_setlen(path, len);
		strbuf_addstr(path, e->d_name);
		ಯದಿ (lstat(path->buf, &st))
			warning_errno(_(ಸಂದೇಶ_warn_lstat_failed), path->buf);
		ಅನ್ಯಥಾ ಯದಿ (S_ISDIR(st.st_mode)) {
			ಯದಿ (remove_dirs(path, prefix, force_flag, dry_run, quiet, &gone))
				ret = 1;
			ಯದಿ (gone) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} ಅನ್ಯಥಾ
				*dir_gone = 0;
			ಅನುವರ್ತ;
		} ಅನ್ಯಥಾ {
			res = dry_run ? 0 : unlink(path->buf);
			ಯದಿ (!res) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} ಅನ್ಯಥಾ {
				ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
				quote_path(path->buf, prefix, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(ಸಂದೇಶ_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
			ಅನುವರ್ತ;
		}

		/* path too long, stat fails, or non-directory still exists */
		*dir_gone = 0;
		ret = 1;
		ಭಙ್ಗ;
	}
	closedir(dir);

	strbuf_setlen(path, original_len);

	ಯದಿ (*dir_gone) {
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
		 ಯದಿ (startup_info->original_cwd)
			 strbuf_realpath(&real_ocwd,
					 startup_info->original_cwd, 1);

		ಯದಿ (!strbuf_cmp(&realpath, &real_ocwd)) {
			printf("%s", dry_run ? _(ಸಂದೇಶ_would_skip_cwd) : _(ಸಂದೇಶ_skip_cwd));
			*dir_gone = 0;
		} ಅನ್ಯಥಾ {
			res = dry_run ? 0 : rmdir(path->buf);
			ಯದಿ (!res)
				*dir_gone = 1;
			ಅನ್ಯಥಾ {
				ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
				quote_path(path->buf, prefix, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(ಸಂದೇಶ_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
		}
	}

	ಯದಿ (!*dir_gone && !quiet) {
		ಪೂರ್ಣಾಙ್ಕ i;
		ಪ್ರತಿ (i = 0; i < dels.nr; i++)
			printf(dry_run ?  _(ಸಂದೇಶ_would_remove) : _(ಸಂದೇಶ_remove), dels.ವಸ್ತುಗಳು[i].string);
	}
out:
	strbuf_release(&realpath);
	strbuf_release(&real_ocwd);
	strbuf_release(&quoted);
	string_list_clear(&dels, 0);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ pretty_print_dels(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ string_list list = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ string_list_item *item;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *qname;
	ಸಂರಚನಾ column_options copts;

	for_each_string_list_item(item, &del_list) {
		qname = quote_path(item->string, ರಿಕ್ತ, &buf, 0);
		string_list_append(&list, qname);
	}

	/*
	 * always enable column display, we only consult column.*
	 * about layout strategy and stuff
	 */
	colopts = (colopts & ~COL_ENABLE_MASK) | COL_ENABLED;
	memset(&copts, 0, ಪರಿಮಾಣ(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(&list, colopts, &copts);
	strbuf_release(&buf);
	string_list_clear(&list, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ pretty_print_menus(ಸಂರಚನಾ string_list *menu_list)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ local_colopts = 0;
	ಸಂರಚನಾ column_options copts;

	local_colopts = COL_ENABLED | COL_ROW;
	memset(&copts, 0, ಪರಿಮಾಣ(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(menu_list, local_colopts, &copts);
}

ಸ್ಥಿರ ಶೂನ್ಯ prompt_help_cmd(ಪೂರ್ಣಾಙ್ಕ singleton)
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
ಸ್ಥಿರ ಶೂನ್ಯ print_highlight_menu_stuff(ಸಂರಚನಾ menu_stuff *stuff, ಪೂರ್ಣಾಙ್ಕ **chosen)
{
	ಸಂರಚನಾ string_list menu_list = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ strbuf menu = STRBUF_INIT;
	ಸಂರಚನಾ menu_item *menu_item;
	ಸಂರಚನಾ string_list_item *string_list_item;
	ಪೂರ್ಣಾಙ್ಕ i;

	ವಿಕಲ್ಪ (stuff->type) {
	ಸ್ವಭಾವ:
		die("Bad type of menu_stuff when print menu");
	ಪಕ್ಷ MENU_STUFF_TYPE_MENU_ITEM:
		menu_item = (ಸಂರಚನಾ menu_item *)stuff->stuff;
		ಪ್ರತಿ (i = 0; i < stuff->nr; i++, menu_item++) {
			ಧ್ರುವ ವರ್ಣ *p;
			ಪೂರ್ಣಾಙ್ಕ highlighted = 0;

			p = menu_item->title;
			ಯದಿ ((*chosen)[i] < 0)
				(*chosen)[i] = menu_item->selected ? 1 : 0;
			strbuf_addf(&menu, "%s%2d: ", (*chosen)[i] ? "*" : " ", i+1);
			ಪ್ರತಿ (; *p; p++) {
				ಯದಿ (!highlighted && *p == menu_item->hotkey) {
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_PROMPT));
					strbuf_addch(&menu, *p);
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_RESET));
					highlighted = 1;
				} ಅನ್ಯಥಾ {
					strbuf_addch(&menu, *p);
				}
			}
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
		}
		ಭಙ್ಗ;
	ಪಕ್ಷ MENU_STUFF_TYPE_STRING_LIST:
		i = 0;
		for_each_string_list_item(string_list_item, (ಸಂರಚನಾ string_list *)stuff->stuff) {
			ಯದಿ ((*chosen)[i] < 0)
				(*chosen)[i] = 0;
			strbuf_addf(&menu, "%s%2d: %s",
				    (*chosen)[i] ? "*" : " ", i+1, string_list_item->string);
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
			i++;
		}
		ಭಙ್ಗ;
	}

	pretty_print_menus(&menu_list);

	strbuf_release(&menu);
	string_list_clear(&menu_list, 0);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ find_unique(ಧ್ರುವ ವರ್ಣ *choice, ಸಂರಚನಾ menu_stuff *menu_stuff)
{
	ಸಂರಚನಾ menu_item *menu_item;
	ಸಂರಚನಾ string_list_item *string_list_item;
	ಪೂರ್ಣಾಙ್ಕ i, len, found = 0;

	len = strlen(choice);
	ವಿಕಲ್ಪ (menu_stuff->type) {
	ಸ್ವಭಾವ:
		die("Bad type of menu_stuff when parse choice");
	ಪಕ್ಷ MENU_STUFF_TYPE_MENU_ITEM:

		menu_item = (ಸಂರಚನಾ menu_item *)menu_stuff->stuff;
		ಪ್ರತಿ (i = 0; i < menu_stuff->nr; i++, menu_item++) {
			ಯದಿ (len == 1 && *choice == menu_item->hotkey) {
				found = i + 1;
				ಭಙ್ಗ;
			}
			ಯದಿ (!strncasecmp(choice, menu_item->title, len)) {
				ಯದಿ (found) {
					ಯದಿ (len == 1) {
						/* continue for hotkey matching */
						found = -1;
					} ಅನ್ಯಥಾ {
						found = 0;
						ಭಙ್ಗ;
					}
				} ಅನ್ಯಥಾ {
					found = i + 1;
				}
			}
		}
		ಭಙ್ಗ;
	ಪಕ್ಷ MENU_STUFF_TYPE_STRING_LIST:
		string_list_item = ((ಸಂರಚನಾ string_list *)menu_stuff->stuff)->ವಸ್ತುಗಳು;
		ಪ್ರತಿ (i = 0; i < menu_stuff->nr; i++, string_list_item++) {
			ಯದಿ (!strncasecmp(choice, string_list_item->string, len)) {
				ಯದಿ (found) {
					found = 0;
					ಭಙ್ಗ;
				}
				found = i + 1;
			}
		}
		ಭಙ್ಗ;
	}
	ಪ್ರತ್ಯಾಗಮ found;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_choice(ಸಂರಚನಾ menu_stuff *menu_stuff,
			ಪೂರ್ಣಾಙ್ಕ is_single,
			ವರ್ಣ *ಒಳಹರಿವು,
			ಪೂರ್ಣಾಙ್ಕ **chosen)
{
	ಸಂರಚನಾ string_list choice = STRING_LIST_INIT_NODUP;
	ಸಂರಚನಾ string_list_item *item;
	ಪೂರ್ಣಾಙ್ಕ nr = 0;
	ಪೂರ್ಣಾಙ್ಕ i;

	string_list_split_in_place_f(&choice, ಒಳಹರಿವು,
				     is_single ? "\n" : ", ", -1,
				     STRING_LIST_SPLIT_TRIM);

	for_each_string_list_item(item, &choice) {
		ಧ್ರುವ ವರ್ಣ *string;
		ಪೂರ್ಣಾಙ್ಕ choose;
		ಪೂರ್ಣಾಙ್ಕ bottom = 0, ತುದಿ = 0;
		ಪೂರ್ಣಾಙ್ಕ is_range, is_ಸಂಖ್ಯೆ;

		string = item->string;
		ಯದಿ (!*string)
			ಅನುವರ್ತ;

		/* Input that begins with '-'; unchoose */
		ಯದಿ (string[0] == '-') {
			choose = 0;
			string++;
		} ಅನ್ಯಥಾ {
			choose = 1;
		}

		is_range = 0;
		is_ಸಂಖ್ಯೆ = 1;
		ಪ್ರತಿ (ಧ್ರುವ ವರ್ಣ *p = string; *p; p++) {
			ಯದಿ ('-' == *p) {
				ಯದಿ (!is_range) {
					is_range = 1;
					is_ಸಂಖ್ಯೆ = 0;
				} ಅನ್ಯಥಾ {
					is_ಸಂಖ್ಯೆ = 0;
					is_range = 0;
					ಭಙ್ಗ;
				}
			} ಅನ್ಯಥಾ ಯದಿ (!isdigit(*p)) {
				is_ಸಂಖ್ಯೆ = 0;
				is_range = 0;
				ಭಙ್ಗ;
			}
		}

		ಯದಿ (is_ಸಂಖ್ಯೆ) {
			bottom = atoi(string);
			ತುದಿ = bottom;
		} ಅನ್ಯಥಾ ಯದಿ (is_range) {
			bottom = atoi(string);
			/* a range can be specified like 5-7 or 5- */
			ಯದಿ (!*(strchr(string, '-') + 1))
				ತುದಿ = menu_stuff->nr;
			ಅನ್ಯಥಾ
				ತುದಿ = atoi(strchr(string, '-') + 1);
		} ಅನ್ಯಥಾ ಯದಿ (!strcmp(string, "*")) {
			bottom = 1;
			ತುದಿ = menu_stuff->nr;
		} ಅನ್ಯಥಾ {
			bottom = find_unique(string, menu_stuff);
			ತುದಿ = bottom;
		}

		ಯದಿ (ತುದಿ <= 0 || bottom <= 0 || ತುದಿ > menu_stuff->nr || bottom > ತುದಿ ||
		    (is_single && bottom != ತುದಿ)) {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf(_("Huh (%s)?\n"), string);
			clean_print_color(CLEAN_COLOR_RESET);
			ಅನುವರ್ತ;
		}

		ಪ್ರತಿ (i = bottom; i <= ತುದಿ; i++)
			(*chosen)[i-1] = choose;
	}

	string_list_clear(&choice, 0);

	ಪ್ರತಿ (i = 0; i < menu_stuff->nr; i++)
		nr += (*chosen)[i];
	ಪ್ರತ್ಯಾಗಮ nr;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ *list_and_choose(ಸಂರಚನಾ menu_opts *opts, ಸಂರಚನಾ menu_stuff *stuff)
{
	ಸಂರಚನಾ strbuf choice = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ *chosen, *result;
	ಪೂರ್ಣಾಙ್ಕ nr = 0;
	ಪೂರ್ಣಾಙ್ಕ eof = 0;
	ಪೂರ್ಣಾಙ್ಕ i;

	ALLOC_ARRAY(chosen, stuff->nr);
	/* set chosen as uninitialized */
	ಪ್ರತಿ (i = 0; i < stuff->nr; i++)
		chosen[i] = -1;

	ಪ್ರತಿ (;;) {
		ಯದಿ (opts->header) {
			printf_ln("%s%s%s",
				  clean_get_color(CLEAN_COLOR_HEADER),
				  _(opts->header),
				  clean_get_color(CLEAN_COLOR_RESET));
		}

		/* chosen will be initialized by print_highlight_menu_stuff */
		print_highlight_menu_stuff(stuff, &chosen);

		ಯದಿ (opts->flags & MENU_OPTS_LIST_ONLY)
			ಭಙ್ಗ;

		ಯದಿ (opts->prompt) {
			printf("%s%s%s%s",
			       clean_get_color(CLEAN_COLOR_PROMPT),
			       _(opts->prompt),
			       opts->flags & MENU_OPTS_SINGLETON ? "> " : ">> ",
			       clean_get_color(CLEAN_COLOR_RESET));
		}

		ಯದಿ (git_read_ಸಾಲು_interactively(&choice) == EOF) {
			eof = 1;
			ಭಙ್ಗ;
		}

		/* help for prompt */
		ಯದಿ (!strcmp(choice.buf, "?")) {
			prompt_help_cmd(opts->flags & MENU_OPTS_SINGLETON);
			ಅನುವರ್ತ;
		}

		/* for a multiple-choice menu, press ENTER (empty) will return back */
		ಯದಿ (!(opts->flags & MENU_OPTS_SINGLETON) && !choice.len)
			ಭಙ್ಗ;

		nr = parse_choice(stuff,
				  opts->flags & MENU_OPTS_SINGLETON,
				  choice.buf,
				  &chosen);

		ಯದಿ (opts->flags & MENU_OPTS_SINGLETON) {
			ಯದಿ (nr)
				ಭಙ್ಗ;
		} ಅನ್ಯಥಾ ಯದಿ (opts->flags & MENU_OPTS_IMMEDIATE) {
			ಭಙ್ಗ;
		}
	}

	ಯದಿ (eof) {
		result = xmalloc(ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ));
		*result = EOF;
	} ಅನ್ಯಥಾ {
		ಪೂರ್ಣಾಙ್ಕ j = 0;

		/*
		 * recalculate nr, if return back from menu directly with
		 * default selections.
		 */
		ಯದಿ (!nr) {
			ಪ್ರತಿ (i = 0; i < stuff->nr; i++)
				nr += chosen[i];
		}

		CALLOC_ARRAY(result, st_add(nr, 1));
		ಪ್ರತಿ (i = 0; i < stuff->nr && j < nr; i++) {
			ಯದಿ (chosen[i])
				result[j++] = i;
		}
		result[j] = EOF;
	}

	ಮುಕ್ತ(chosen);
	strbuf_release(&choice);
	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ clean_cmd(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ MENU_RETURN_NO_LOOP;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ filter_by_patterns_cmd(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ dir_struct dir = DIR_INIT;
	ಸಂರಚನಾ strbuf confirm = STRBUF_INIT;
	ಸಂರಚನಾ pattern_list *pl;
	ಪೂರ್ಣಾಙ್ಕ changed = -1, i;

	ಪ್ರತಿ (;;) {
		ಸಂರಚನಾ string_list ignore_list = STRING_LIST_INIT_NODUP;
		ಸಂರಚನಾ string_list_item *item;

		ಯದಿ (!del_list.nr)
			ಭಙ್ಗ;

		ಯದಿ (changed)
			pretty_print_dels();

		clean_print_color(CLEAN_COLOR_PROMPT);
		printf(_("Input ignore patterns>> "));
		clean_print_color(CLEAN_COLOR_RESET);
		ಯದಿ (git_read_ಸಾಲು_interactively(&confirm) == EOF)
			putchar('\n');

		/* quit filter_by_pattern mode if press ENTER or Ctrl-D */
		ಯದಿ (!confirm.len)
			ಭಙ್ಗ;

		pl = add_pattern_list(&dir, EXC_CMDL, "manual exclude");

		string_list_split_in_place_f(&ignore_list, confirm.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);

		ಪ್ರತಿ (i = 0; i < ignore_list.nr; i++) {
			item = &ignore_list.ವಸ್ತುಗಳು[i];
			ಯದಿ (!*item->string)
				ಅನುವರ್ತ;
			add_pattern(item->string, "", 0, pl, -(i+1));
		}

		changed = 0;
		for_each_string_list_item(item, &del_list) {
			ಪೂರ್ಣಾಙ್ಕ dtype = DT_UNKNOWN;

			ಯದಿ (is_excluded(&dir, the_repository->index, item->string, &dtype)) {
				*item->string = '\0';
				changed++;
			}
		}

		ಯದಿ (changed) {
			string_list_remove_empty_ವಸ್ತುಗಳು(&del_list, 0);
		} ಅನ್ಯಥಾ {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf_ln(_("WARNING: Cannot find items matched by: %s"), confirm.buf);
			clean_print_color(CLEAN_COLOR_RESET);
		}

		string_list_clear(&ignore_list, 0);
		dir_clear(&dir);
	}

	strbuf_release(&confirm);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ select_by_numbers_cmd(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ menu_opts menu_opts;
	ಸಂರಚನಾ menu_stuff menu_stuff;
	ಸಂರಚನಾ string_list_item *ವಸ್ತುಗಳು;
	ಪೂರ್ಣಾಙ್ಕ *chosen;
	ಪೂರ್ಣಾಙ್ಕ i, j;

	menu_opts.header = ರಿಕ್ತ;
	menu_opts.prompt = N_("Select items to delete");
	menu_opts.flags = 0;

	menu_stuff.type = MENU_STUFF_TYPE_STRING_LIST;
	menu_stuff.stuff = &del_list;
	menu_stuff.nr = del_list.nr;

	chosen = list_and_choose(&menu_opts, &menu_stuff);
	ವಸ್ತುಗಳು = del_list.ವಸ್ತುಗಳು;
	ಪ್ರತಿ (i = 0, j = 0; i < del_list.nr; i++) {
		ಯದಿ (i < chosen[j]) {
			*(ವಸ್ತುಗಳು[i].string) = '\0';
		} ಅನ್ಯಥಾ ಯದಿ (i == chosen[j]) {
			/* delete selected item */
			j++;
			ಅನುವರ್ತ;
		} ಅನ್ಯಥಾ {
			/* end of chosen (chosen[j] == EOF), won't delete */
			*(ವಸ್ತುಗಳು[i].string) = '\0';
		}
	}

	string_list_remove_empty_ವಸ್ತುಗಳು(&del_list, 0);

	ಮುಕ್ತ(chosen);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ask_each_cmd(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ strbuf confirm = STRBUF_INIT;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಸಂರಚನಾ string_list_item *item;
	ಧ್ರುವ ವರ್ಣ *qname;
	ಪೂರ್ಣಾಙ್ಕ changed = 0, eof = 0;

	for_each_string_list_item(item, &del_list) {
		/* Ctrl-D should stop removing files */
		ಯದಿ (!eof) {
			qname = quote_path(item->string, ರಿಕ್ತ, &buf, 0);
			/* TRANSLATORS: Make sure to keep [y/N] as is */
			printf(_("Remove %s [y/N]? "), qname);
			ಯದಿ (git_read_ಸಾಲು_interactively(&confirm) == EOF) {
				putchar('\n');
				eof = 1;
			}
		}
		ಯದಿ (!confirm.len || strncasecmp(confirm.buf, "yes", confirm.len)) {
			*item->string = '\0';
			changed++;
		}
	}

	ಯದಿ (changed)
		string_list_remove_empty_ವಸ್ತುಗಳು(&del_list, 0);

	strbuf_release(&buf);
	strbuf_release(&confirm);
	ಪ್ರತ್ಯಾಗಮ MENU_RETURN_NO_LOOP;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ quit_cmd(ಶೂನ್ಯ)
{
	string_list_clear(&del_list, 0);
	printf(_("Bye.\n"));
	ಪ್ರತ್ಯಾಗಮ MENU_RETURN_NO_LOOP;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ help_cmd(ಶೂನ್ಯ)
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
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ interactive_main_loop(ಶೂನ್ಯ)
{
	ಯಾವತ್ (del_list.nr) {
		ಸಂರಚನಾ menu_opts menu_opts;
		ಸಂರಚನಾ menu_stuff menu_stuff;
		ಸಂರಚನಾ menu_item menus[] = {
			{'c', "clean",			0, clean_cmd},
			{'f', "filter by pattern",	0, filter_by_patterns_cmd},
			{'s', "select by numbers",	0, select_by_numbers_cmd},
			{'a', "ask each",		0, ask_each_cmd},
			{'q', "quit",			0, quit_cmd},
			{'h', "help",			0, help_cmd},
		};
		ಪೂರ್ಣಾಙ್ಕ *chosen;

		menu_opts.header = N_("*** Commands ***");
		menu_opts.prompt = N_("What now");
		menu_opts.flags = MENU_OPTS_SINGLETON;

		menu_stuff.type = MENU_STUFF_TYPE_MENU_ITEM;
		menu_stuff.stuff = menus;
		menu_stuff.nr = ಪರಿಮಾಣ(menus) / ಪರಿಮಾಣ(ಸಂರಚನಾ menu_item);

		clean_print_color(CLEAN_COLOR_HEADER);
		printf_ln(Q_("Would remove the following item:",
			     "Would remove the following items:",
			     del_list.nr));
		clean_print_color(CLEAN_COLOR_RESET);

		pretty_print_dels();

		chosen = list_and_choose(&menu_opts, &menu_stuff);

		ಯದಿ (*chosen != EOF) {
			ಪೂರ್ಣಾಙ್ಕ ret;
			ret = menus[*chosen].fn();
			ಯದಿ (ret != MENU_RETURN_NO_LOOP) {
				FREE_AND_NULL(chosen);
				ಯದಿ (!del_list.nr) {
					clean_print_color(CLEAN_COLOR_ERROR);
					printf_ln(_("No more files to clean, exiting."));
					clean_print_color(CLEAN_COLOR_RESET);
					ಭಙ್ಗ;
				}
				ಅನುವರ್ತ;
			}
		} ಅನ್ಯಥಾ {
			quit_cmd();
		}

		FREE_AND_NULL(chosen);
		ಭಙ್ಗ;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ correct_untracked_entries(ಸಂರಚನಾ dir_struct *dir)
{
	ಪೂರ್ಣಾಙ್ಕ src, dst, ign;

	ಪ್ರತಿ (src = dst = ign = 0; src < dir->nr; src++) {
		/* skip paths in ignored[] that cannot be inside entries[src] */
		ಯಾವತ್ (ign < dir->ignored_nr &&
		       0 <= cmp_dir_entry(&dir->entries[src], &dir->ignored[ign]))
			ign++;

		ಯದಿ (ign < dir->ignored_nr &&
		    check_dir_entry_contains(dir->entries[src], dir->ignored[ign])) {
			/* entries[src] contains an ignored path, so we drop it */
			ಮುಕ್ತ(dir->entries[src]);
		} ಅನ್ಯಥಾ {
			ಸಂರಚನಾ dir_entry *ent = dir->entries[src++];

			/* entries[src] does not contain an ignored path, so we keep it */
			dir->entries[dst++] = ent;

			/* then discard paths in entries[] contained inside entries[src] */
			ಯಾವತ್ (src < dir->nr &&
			       check_dir_entry_contains(ent, dir->entries[src]))
				ಮುಕ್ತ(dir->entries[src++]);

			/* compensate for the outer loop's loop control */
			src--;
		}
	}
	dir->nr = dst;
}

ಪೂರ್ಣಾಙ್ಕ cmd_clean(ಪೂರ್ಣಾಙ್ಕ argc,
	      ಧ್ರುವ ವರ್ಣ **argv,
	      ಧ್ರುವ ವರ್ಣ *prefix,
	      ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ i, res;
	ಪೂರ್ಣಾಙ್ಕ dry_run = 0, remove_directories = 0, quiet = 0, ignored = 0;
	ಪೂರ್ಣಾಙ್ಕ ignored_only = 0, force = 0, errors = 0, gone = 1;
	ಪೂರ್ಣಾಙ್ಕ rm_flags = REMOVE_DIR_KEEP_NESTED_GIT;
	ಸಂರಚನಾ strbuf abs_path = STRBUF_INIT;
	ಸಂರಚನಾ dir_struct dir = DIR_INIT;
	ಸಂರಚನಾ pathspec pathspec;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಸಂರಚನಾ string_list exclude_list = STRING_LIST_INIT_NODUP;
	ಸಂರಚನಾ pattern_list *pl;
	ಸಂರಚನಾ string_list_item *item;
	ಧ್ರುವ ವರ್ಣ *qname;
	ಸಂರಚನಾ option options[] = {
		OPT__QUIET(&quiet, N_("do not print names of files removed")),
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('i', "interactive", &interactive, N_("interactive cleaning")),
		OPT_BOOL('d', ರಿಕ್ತ, &remove_directories,
				N_("remove whole directories")),
		OPT_CALLBACK_F('e', "exclude", &exclude_list, N_("pattern"),
		  N_("add <pattern> to ignore rules"), PARSE_OPT_NONEG, exclude_cb),
		OPT_BOOL('x', ರಿಕ್ತ, &ignored, N_("remove ignored files, too")),
		OPT_BOOL('X', ರಿಕ್ತ, &ignored_only,
				N_("remove only ignored files")),
		OPT_END()
	};

	repo_config(the_repository, git_clean_config, ರಿಕ್ತ);

	argc = parse_options(argc, argv, prefix, options, builtin_clean_usage,
			     0);

	ಯದಿ (require_force != 0 && !force && !interactive && !dry_run)
		die(_("clean.requireForce is true and -f not given: refusing to clean"));

	ಯದಿ (force > 1)
		rm_flags = 0;
	ಅನ್ಯಥಾ
		dir.flags |= DIR_SKIP_NESTED_GIT;

	dir.flags |= DIR_SHOW_OTHER_DIRECTORIES;

	ಯದಿ (ignored && ignored_only)
		die(_("options '%s' and '%s' cannot be used together"), "-x", "-X");
	ಯದಿ (!ignored)
		setup_standard_excludes(&dir);
	ಯದಿ (ignored_only)
		dir.flags |= DIR_SHOW_IGNORED;

	ಯದಿ (argc) {
		/*
		 * Remaining args implies pathspecs specified, and we should
		 * recurse within those.
		 */
		remove_directories = 1;
	}

	ಯದಿ (remove_directories && !ignored_only) {
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
		ಯದಿ (!ignored)
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

	ಯದಿ (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	ಪ್ರತಿ (i = 0; i < exclude_list.nr; i++)
		add_pattern(exclude_list.ವಸ್ತುಗಳು[i].string, "", 0, pl, -(i+1));

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	fill_directory(&dir, the_repository->index, &pathspec);
	correct_untracked_entries(&dir);

	ಪ್ರತಿ (i = 0; i < dir.nr; i++) {
		ಸಂರಚನಾ dir_entry *ent = dir.entries[i];
		ಸಂರಚನಾ stat st;
		ಧ್ರುವ ವರ್ಣ *rel;

		ಯದಿ (!index_name_is_other(the_repository->index, ent->name, ent->len))
			ಅನುವರ್ತ;

		ಯದಿ (lstat(ent->name, &st))
			die_errno("Cannot lstat '%s'", ent->name);

		ಯದಿ (S_ISDIR(st.st_mode) && !remove_directories)
			ಅನುವರ್ತ;

		rel = relative_path(ent->name, prefix, &buf);
		string_list_append(&del_list, rel);
	}

	dir_clear(&dir);

	ಯದಿ (interactive && del_list.nr > 0)
		interactive_main_loop();

	for_each_string_list_item(item, &del_list) {
		ಸಂರಚನಾ stat st;

		strbuf_reset(&abs_path);
		ಯದಿ (prefix)
			strbuf_addstr(&abs_path, prefix);

		strbuf_addstr(&abs_path, item->string);

		/*
		 * we might have removed this as part of earlier
		 * recursive directory removal, so lstat() here could
		 * fail with ENOENT.
		 */
		ಯದಿ (lstat(abs_path.buf, &st))
			ಅನುವರ್ತ;

		ಯದಿ (S_ISDIR(st.st_mode)) {
			ಯದಿ (remove_dirs(&abs_path, prefix, rm_flags, dry_run, quiet, &gone))
				errors++;
			ಯದಿ (gone && !quiet) {
				qname = quote_path(item->string, ರಿಕ್ತ, &buf, 0);
				printf(dry_run ? _(ಸಂದೇಶ_would_remove) : _(ಸಂದೇಶ_remove), qname);
			}
		} ಅನ್ಯಥಾ {
			res = dry_run ? 0 : unlink(abs_path.buf);
			ಯದಿ (res) {
				ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
				qname = quote_path(item->string, ರಿಕ್ತ, &buf, 0);
				errno = saved_errno;
				warning_errno(_(ಸಂದೇಶ_warn_remove_failed), qname);
				errors++;
			} ಅನ್ಯಥಾ ಯದಿ (!quiet) {
				qname = quote_path(item->string, ರಿಕ್ತ, &buf, 0);
				printf(dry_run ? _(ಸಂದೇಶ_would_remove) : _(ಸಂದೇಶ_remove), qname);
			}
		}
	}

	strbuf_release(&abs_path);
	strbuf_release(&buf);
	string_list_clear(&del_list, 0);
	string_list_clear(&exclude_list, 0);
	clear_pathspec(&pathspec);
	ಪ್ರತ್ಯಾಗಮ (errors != 0);
}
