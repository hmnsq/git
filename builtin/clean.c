/*
 * "git clean" builtin command
 *
 * Copyright (C) 2007 Shawn Bohrer
 *
 * Based on git-clean.sh by Pavel Roskin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "abspath.h"
#شامل "config.h"
#شامل "dir.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "read-cache-ll.h"
#شامل "setup.h"
#شامل "string-list.h"
#شامل "quote.h"
#شامل "column.h"
#شامل "color.h"
#شامل "pathspec.h"
#شامل "help.h"
#شامل "prompt.h"

ایستا صحیح require_force = -1; /* unset */
ایستا صحیح interactive;
ایستا ساختار string_list del_list = STRING_LIST_INIT_DUP;
ایستا unsigned صحیح colopts;

ایستا ثابت نویسه *ثابت builtin_clean_usage[] = {
	N_("git clean [-d] [-f] [-i] [-n] [-q] [-e <pattern>] [-x | -X] [--] [<pathspec>...]"),
	پوچ
};

ایستا ثابت نویسه *پیام_remove = N_("Removing %s\n");
ایستا ثابت نویسه *پیام_would_remove = N_("Would remove %s\n");
ایستا ثابت نویسه *پیام_skip_git_dir = N_("Skipping repository %s\n");
ایستا ثابت نویسه *پیام_would_skip_git_dir = N_("Would skip repository %s\n");
ایستا ثابت نویسه *پیام_warn_remove_failed = N_("failed to remove %s");
ایستا ثابت نویسه *پیام_warn_lstat_failed = N_("could not lstat %s\n");
ایستا ثابت نویسه *پیام_skip_cwd = N_("Refusing to remove current working directory\n");
ایستا ثابت نویسه *پیام_would_skip_cwd = N_("Would refuse to remove current working directory\n");

شمارش color_clean {
	CLEAN_COLOR_RESET = 0,
	CLEAN_COLOR_PLAIN = 1,
	CLEAN_COLOR_PROMPT = 2,
	CLEAN_COLOR_HEADER = 3,
	CLEAN_COLOR_HELP = 4,
	CLEAN_COLOR_ERROR = 5
};

ایستا ثابت نویسه *color_interactive_slots[] = {
	[CLEAN_COLOR_ERROR]  = "error",
	[CLEAN_COLOR_HEADER] = "header",
	[CLEAN_COLOR_HELP]   = "help",
	[CLEAN_COLOR_PLAIN]  = "plain",
	[CLEAN_COLOR_PROMPT] = "prompt",
	[CLEAN_COLOR_RESET]  = "reset",
};

ایستا شمارش git_colorbool clean_use_color = GIT_COLOR_UNKNOWN;
ایستا نویسه clean_colors[][COLOR_MAXLEN] = {
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

ساختار menu_opts {
	ثابت نویسه *header;
	ثابت نویسه *prompt;
	صحیح flags;
};

#define MENU_RETURN_NO_LOOP		10

ساختار menu_item {
	نویسه hotkey;
	ثابت نویسه *title;
	صحیح selected;
	صحیح (*fn)(تهی);
};

شمارش menu_stuff_type {
	MENU_STUFF_TYPE_STRING_LIST = 1,
	MENU_STUFF_TYPE_MENU_ITEM
};

ساختار menu_stuff {
	شمارش menu_stuff_type type;
	صحیح nr;
	تهی *stuff;
};

define_list_config_array(color_interactive_slots);

ایستا صحیح git_clean_config(ثابت نویسه *var, ثابت نویسه *مقدار,
			    ثابت ساختار config_context *ctx, تهی *cb)
{
	ثابت نویسه *slot_name;

	اگر (starts_with(var, "column."))
		بازگشت git_column_config(var, مقدار, "clean", &colopts);

	/* honors the color.interactive* config variables which also
	   applied in git-add--interactive and git-stash */
	اگر (!strcmp(var, "color.interactive")) {
		clean_use_color = git_config_colorbool(var, مقدار);
		بازگشت 0;
	}
	اگر (skip_prefix(var, "color.interactive.", &slot_name)) {
		صحیح slot = LOOKUP_CONFIG(color_interactive_slots, slot_name);
		اگر (slot < 0)
			بازگشت 0;
		اگر (!مقدار)
			بازگشت config_error_nonbool(var);
		بازگشت color_parse(مقدار, clean_colors[slot]);
	}

	اگر (!strcmp(var, "clean.requireforce")) {
		require_force = git_config_bool(var, مقدار);
		بازگشت 0;
	}

	اگر (git_color_config(var, مقدار, cb) < 0)
		بازگشت -1;

	بازگشت git_default_config(var, مقدار, ctx, cb);
}

ایستا ثابت نویسه *clean_get_color(شمارش color_clean ix)
{
	اگر (want_color(clean_use_color))
		بازگشت clean_colors[ix];
	بازگشت "";
}

ایستا تهی clean_print_color(شمارش color_clean ix)
{
	printf("%s", clean_get_color(ix));
}

ایستا صحیح exclude_cb(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	ساختار string_list *exclude_list = opt->مقدار;
	BUG_ON_OPT_NEG(unset);
	string_list_append(exclude_list, arg);
	بازگشت 0;
}

ایستا صحیح remove_dirs(ساختار strbuf *path, ثابت نویسه *prefix, صحیح force_flag,
		صحیح dry_run, صحیح quiet, صحیح *dir_gone)
{
	DIR *dir;
	ساختار strbuf quoted = STRBUF_INIT;
	ساختار strbuf realpath = STRBUF_INIT;
	ساختار strbuf real_ocwd = STRBUF_INIT;
	ساختار dirent *e;
	صحیح res = 0, ret = 0, gone = 1, original_len = path->len, len;
	ساختار string_list dels = STRING_LIST_INIT_DUP;

	*dir_gone = 1;

	اگر ((force_flag & REMOVE_DIR_KEEP_NESTED_GIT) &&
	    is_nonbare_repository_dir(path)) {
		اگر (!quiet) {
			quote_path(path->buf, prefix, &quoted, 0);
			printf(dry_run ?  _(پیام_would_skip_git_dir) : _(پیام_skip_git_dir),
					quoted.buf);
		}

		*dir_gone = 0;
		برو out;
	}

	dir = opendir(path->buf);
	اگر (!dir) {
		/* an empty dir could be removed even if it is unreadble */
		res = dry_run ? 0 : rmdir(path->buf);
		اگر (res) {
			صحیح saved_errno = errno;
			quote_path(path->buf, prefix, &quoted, 0);
			errno = saved_errno;
			warning_errno(_(پیام_warn_remove_failed), quoted.buf);
			*dir_gone = 0;
		}
		ret = res;
		برو out;
	}

	strbuf_complete(path, '/');

	len = path->len;
	وقتی ((e = readdir_skip_dot_and_dotdot(dir)) != پوچ) {
		ساختار stat st;

		strbuf_setlen(path, len);
		strbuf_addstr(path, e->d_name);
		اگر (lstat(path->buf, &st))
			warning_errno(_(پیام_warn_lstat_failed), path->buf);
		وگرنه اگر (S_ISDIR(st.st_mode)) {
			اگر (remove_dirs(path, prefix, force_flag, dry_run, quiet, &gone))
				ret = 1;
			اگر (gone) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} وگرنه
				*dir_gone = 0;
			ادامه;
		} وگرنه {
			res = dry_run ? 0 : unlink(path->buf);
			اگر (!res) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} وگرنه {
				صحیح saved_errno = errno;
				quote_path(path->buf, prefix, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(پیام_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
			ادامه;
		}

		/* path too long, stat fails, or non-directory still exists */
		*dir_gone = 0;
		ret = 1;
		بشکن;
	}
	closedir(dir);

	strbuf_setlen(path, original_len);

	اگر (*dir_gone) {
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
		 اگر (startup_info->original_cwd)
			 strbuf_realpath(&real_ocwd,
					 startup_info->original_cwd, 1);

		اگر (!strbuf_cmp(&realpath, &real_ocwd)) {
			printf("%s", dry_run ? _(پیام_would_skip_cwd) : _(پیام_skip_cwd));
			*dir_gone = 0;
		} وگرنه {
			res = dry_run ? 0 : rmdir(path->buf);
			اگر (!res)
				*dir_gone = 1;
			وگرنه {
				صحیح saved_errno = errno;
				quote_path(path->buf, prefix, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(پیام_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
		}
	}

	اگر (!*dir_gone && !quiet) {
		صحیح i;
		برای (i = 0; i < dels.nr; i++)
			printf(dry_run ?  _(پیام_would_remove) : _(پیام_remove), dels.اقلام[i].string);
	}
out:
	strbuf_release(&realpath);
	strbuf_release(&real_ocwd);
	strbuf_release(&quoted);
	string_list_clear(&dels, 0);
	بازگشت ret;
}

ایستا تهی pretty_print_dels(تهی)
{
	ساختار string_list list = STRING_LIST_INIT_DUP;
	ساختار string_list_item *item;
	ساختار strbuf buf = STRBUF_INIT;
	ثابت نویسه *qname;
	ساختار column_options copts;

	for_each_string_list_item(item, &del_list) {
		qname = quote_path(item->string, پوچ, &buf, 0);
		string_list_append(&list, qname);
	}

	/*
	 * always enable column display, we only consult column.*
	 * about layout strategy and stuff
	 */
	colopts = (colopts & ~COL_ENABLE_MASK) | COL_ENABLED;
	memset(&copts, 0, اندازه(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(&list, colopts, &copts);
	strbuf_release(&buf);
	string_list_clear(&list, 0);
}

ایستا تهی pretty_print_menus(ساختار string_list *menu_list)
{
	unsigned صحیح local_colopts = 0;
	ساختار column_options copts;

	local_colopts = COL_ENABLED | COL_ROW;
	memset(&copts, 0, اندازه(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(menu_list, local_colopts, &copts);
}

ایستا تهی prompt_help_cmd(صحیح singleton)
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
ایستا تهی print_highlight_menu_stuff(ساختار menu_stuff *stuff, صحیح **chosen)
{
	ساختار string_list menu_list = STRING_LIST_INIT_DUP;
	ساختار strbuf menu = STRBUF_INIT;
	ساختار menu_item *menu_item;
	ساختار string_list_item *string_list_item;
	صحیح i;

	انتخاب (stuff->type) {
	پیشفرض:
		die("Bad type of menu_stuff when print menu");
	حالت MENU_STUFF_TYPE_MENU_ITEM:
		menu_item = (ساختار menu_item *)stuff->stuff;
		برای (i = 0; i < stuff->nr; i++, menu_item++) {
			ثابت نویسه *p;
			صحیح highlighted = 0;

			p = menu_item->title;
			اگر ((*chosen)[i] < 0)
				(*chosen)[i] = menu_item->selected ? 1 : 0;
			strbuf_addf(&menu, "%s%2d: ", (*chosen)[i] ? "*" : " ", i+1);
			برای (; *p; p++) {
				اگر (!highlighted && *p == menu_item->hotkey) {
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_PROMPT));
					strbuf_addch(&menu, *p);
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_RESET));
					highlighted = 1;
				} وگرنه {
					strbuf_addch(&menu, *p);
				}
			}
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
		}
		بشکن;
	حالت MENU_STUFF_TYPE_STRING_LIST:
		i = 0;
		for_each_string_list_item(string_list_item, (ساختار string_list *)stuff->stuff) {
			اگر ((*chosen)[i] < 0)
				(*chosen)[i] = 0;
			strbuf_addf(&menu, "%s%2d: %s",
				    (*chosen)[i] ? "*" : " ", i+1, string_list_item->string);
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
			i++;
		}
		بشکن;
	}

	pretty_print_menus(&menu_list);

	strbuf_release(&menu);
	string_list_clear(&menu_list, 0);
}

ایستا صحیح find_unique(ثابت نویسه *choice, ساختار menu_stuff *menu_stuff)
{
	ساختار menu_item *menu_item;
	ساختار string_list_item *string_list_item;
	صحیح i, len, found = 0;

	len = strlen(choice);
	انتخاب (menu_stuff->type) {
	پیشفرض:
		die("Bad type of menu_stuff when parse choice");
	حالت MENU_STUFF_TYPE_MENU_ITEM:

		menu_item = (ساختار menu_item *)menu_stuff->stuff;
		برای (i = 0; i < menu_stuff->nr; i++, menu_item++) {
			اگر (len == 1 && *choice == menu_item->hotkey) {
				found = i + 1;
				بشکن;
			}
			اگر (!strncasecmp(choice, menu_item->title, len)) {
				اگر (found) {
					اگر (len == 1) {
						/* continue for hotkey matching */
						found = -1;
					} وگرنه {
						found = 0;
						بشکن;
					}
				} وگرنه {
					found = i + 1;
				}
			}
		}
		بشکن;
	حالت MENU_STUFF_TYPE_STRING_LIST:
		string_list_item = ((ساختار string_list *)menu_stuff->stuff)->اقلام;
		برای (i = 0; i < menu_stuff->nr; i++, string_list_item++) {
			اگر (!strncasecmp(choice, string_list_item->string, len)) {
				اگر (found) {
					found = 0;
					بشکن;
				}
				found = i + 1;
			}
		}
		بشکن;
	}
	بازگشت found;
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
ایستا صحیح parse_choice(ساختار menu_stuff *menu_stuff,
			صحیح is_single,
			نویسه *درون_داد,
			صحیح **chosen)
{
	ساختار string_list choice = STRING_LIST_INIT_NODUP;
	ساختار string_list_item *item;
	صحیح nr = 0;
	صحیح i;

	string_list_split_in_place_f(&choice, درون_داد,
				     is_single ? "\n" : ", ", -1,
				     STRING_LIST_SPLIT_TRIM);

	for_each_string_list_item(item, &choice) {
		ثابت نویسه *string;
		صحیح choose;
		صحیح bottom = 0, بالاترین = 0;
		صحیح is_range, is_عدد;

		string = item->string;
		اگر (!*string)
			ادامه;

		/* Input that begins with '-'; unchoose */
		اگر (string[0] == '-') {
			choose = 0;
			string++;
		} وگرنه {
			choose = 1;
		}

		is_range = 0;
		is_عدد = 1;
		برای (ثابت نویسه *p = string; *p; p++) {
			اگر ('-' == *p) {
				اگر (!is_range) {
					is_range = 1;
					is_عدد = 0;
				} وگرنه {
					is_عدد = 0;
					is_range = 0;
					بشکن;
				}
			} وگرنه اگر (!isdigit(*p)) {
				is_عدد = 0;
				is_range = 0;
				بشکن;
			}
		}

		اگر (is_عدد) {
			bottom = atoi(string);
			بالاترین = bottom;
		} وگرنه اگر (is_range) {
			bottom = atoi(string);
			/* a range can be specified like 5-7 or 5- */
			اگر (!*(strchr(string, '-') + 1))
				بالاترین = menu_stuff->nr;
			وگرنه
				بالاترین = atoi(strchr(string, '-') + 1);
		} وگرنه اگر (!strcmp(string, "*")) {
			bottom = 1;
			بالاترین = menu_stuff->nr;
		} وگرنه {
			bottom = find_unique(string, menu_stuff);
			بالاترین = bottom;
		}

		اگر (بالاترین <= 0 || bottom <= 0 || بالاترین > menu_stuff->nr || bottom > بالاترین ||
		    (is_single && bottom != بالاترین)) {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf(_("Huh (%s)?\n"), string);
			clean_print_color(CLEAN_COLOR_RESET);
			ادامه;
		}

		برای (i = bottom; i <= بالاترین; i++)
			(*chosen)[i-1] = choose;
	}

	string_list_clear(&choice, 0);

	برای (i = 0; i < menu_stuff->nr; i++)
		nr += (*chosen)[i];
	بازگشت nr;
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
ایستا صحیح *list_and_choose(ساختار menu_opts *opts, ساختار menu_stuff *stuff)
{
	ساختار strbuf choice = STRBUF_INIT;
	صحیح *chosen, *result;
	صحیح nr = 0;
	صحیح eof = 0;
	صحیح i;

	ALLOC_ARRAY(chosen, stuff->nr);
	/* set chosen as uninitialized */
	برای (i = 0; i < stuff->nr; i++)
		chosen[i] = -1;

	برای (;;) {
		اگر (opts->header) {
			printf_ln("%s%s%s",
				  clean_get_color(CLEAN_COLOR_HEADER),
				  _(opts->header),
				  clean_get_color(CLEAN_COLOR_RESET));
		}

		/* chosen will be initialized by print_highlight_menu_stuff */
		print_highlight_menu_stuff(stuff, &chosen);

		اگر (opts->flags & MENU_OPTS_LIST_ONLY)
			بشکن;

		اگر (opts->prompt) {
			printf("%s%s%s%s",
			       clean_get_color(CLEAN_COLOR_PROMPT),
			       _(opts->prompt),
			       opts->flags & MENU_OPTS_SINGLETON ? "> " : ">> ",
			       clean_get_color(CLEAN_COLOR_RESET));
		}

		اگر (git_read_سطر_interactively(&choice) == EOF) {
			eof = 1;
			بشکن;
		}

		/* help for prompt */
		اگر (!strcmp(choice.buf, "?")) {
			prompt_help_cmd(opts->flags & MENU_OPTS_SINGLETON);
			ادامه;
		}

		/* for a multiple-choice menu, press ENTER (empty) will return back */
		اگر (!(opts->flags & MENU_OPTS_SINGLETON) && !choice.len)
			بشکن;

		nr = parse_choice(stuff,
				  opts->flags & MENU_OPTS_SINGLETON,
				  choice.buf,
				  &chosen);

		اگر (opts->flags & MENU_OPTS_SINGLETON) {
			اگر (nr)
				بشکن;
		} وگرنه اگر (opts->flags & MENU_OPTS_IMMEDIATE) {
			بشکن;
		}
	}

	اگر (eof) {
		result = xmalloc(اندازه(صحیح));
		*result = EOF;
	} وگرنه {
		صحیح j = 0;

		/*
		 * recalculate nr, if return back from menu directly with
		 * default selections.
		 */
		اگر (!nr) {
			برای (i = 0; i < stuff->nr; i++)
				nr += chosen[i];
		}

		CALLOC_ARRAY(result, st_add(nr, 1));
		برای (i = 0; i < stuff->nr && j < nr; i++) {
			اگر (chosen[i])
				result[j++] = i;
		}
		result[j] = EOF;
	}

	آزاد(chosen);
	strbuf_release(&choice);
	بازگشت result;
}

ایستا صحیح clean_cmd(تهی)
{
	بازگشت MENU_RETURN_NO_LOOP;
}

ایستا صحیح filter_by_patterns_cmd(تهی)
{
	ساختار dir_struct dir = DIR_INIT;
	ساختار strbuf confirm = STRBUF_INIT;
	ساختار pattern_list *pl;
	صحیح changed = -1, i;

	برای (;;) {
		ساختار string_list ignore_list = STRING_LIST_INIT_NODUP;
		ساختار string_list_item *item;

		اگر (!del_list.nr)
			بشکن;

		اگر (changed)
			pretty_print_dels();

		clean_print_color(CLEAN_COLOR_PROMPT);
		printf(_("Input ignore patterns>> "));
		clean_print_color(CLEAN_COLOR_RESET);
		اگر (git_read_سطر_interactively(&confirm) == EOF)
			putchar('\n');

		/* quit filter_by_pattern mode if press ENTER or Ctrl-D */
		اگر (!confirm.len)
			بشکن;

		pl = add_pattern_list(&dir, EXC_CMDL, "manual exclude");

		string_list_split_in_place_f(&ignore_list, confirm.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);

		برای (i = 0; i < ignore_list.nr; i++) {
			item = &ignore_list.اقلام[i];
			اگر (!*item->string)
				ادامه;
			add_pattern(item->string, "", 0, pl, -(i+1));
		}

		changed = 0;
		for_each_string_list_item(item, &del_list) {
			صحیح dtype = DT_UNKNOWN;

			اگر (is_excluded(&dir, the_repository->index, item->string, &dtype)) {
				*item->string = '\0';
				changed++;
			}
		}

		اگر (changed) {
			string_list_remove_empty_اقلام(&del_list, 0);
		} وگرنه {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf_ln(_("WARNING: Cannot find items matched by: %s"), confirm.buf);
			clean_print_color(CLEAN_COLOR_RESET);
		}

		string_list_clear(&ignore_list, 0);
		dir_clear(&dir);
	}

	strbuf_release(&confirm);
	بازگشت 0;
}

ایستا صحیح select_by_numbers_cmd(تهی)
{
	ساختار menu_opts menu_opts;
	ساختار menu_stuff menu_stuff;
	ساختار string_list_item *اقلام;
	صحیح *chosen;
	صحیح i, j;

	menu_opts.header = پوچ;
	menu_opts.prompt = N_("Select items to delete");
	menu_opts.flags = 0;

	menu_stuff.type = MENU_STUFF_TYPE_STRING_LIST;
	menu_stuff.stuff = &del_list;
	menu_stuff.nr = del_list.nr;

	chosen = list_and_choose(&menu_opts, &menu_stuff);
	اقلام = del_list.اقلام;
	برای (i = 0, j = 0; i < del_list.nr; i++) {
		اگر (i < chosen[j]) {
			*(اقلام[i].string) = '\0';
		} وگرنه اگر (i == chosen[j]) {
			/* delete selected item */
			j++;
			ادامه;
		} وگرنه {
			/* end of chosen (chosen[j] == EOF), won't delete */
			*(اقلام[i].string) = '\0';
		}
	}

	string_list_remove_empty_اقلام(&del_list, 0);

	آزاد(chosen);
	بازگشت 0;
}

ایستا صحیح ask_each_cmd(تهی)
{
	ساختار strbuf confirm = STRBUF_INIT;
	ساختار strbuf buf = STRBUF_INIT;
	ساختار string_list_item *item;
	ثابت نویسه *qname;
	صحیح changed = 0, eof = 0;

	for_each_string_list_item(item, &del_list) {
		/* Ctrl-D should stop removing files */
		اگر (!eof) {
			qname = quote_path(item->string, پوچ, &buf, 0);
			/* TRANSLATORS: Make sure to keep [y/N] as is */
			printf(_("Remove %s [y/N]? "), qname);
			اگر (git_read_سطر_interactively(&confirm) == EOF) {
				putchar('\n');
				eof = 1;
			}
		}
		اگر (!confirm.len || strncasecmp(confirm.buf, "yes", confirm.len)) {
			*item->string = '\0';
			changed++;
		}
	}

	اگر (changed)
		string_list_remove_empty_اقلام(&del_list, 0);

	strbuf_release(&buf);
	strbuf_release(&confirm);
	بازگشت MENU_RETURN_NO_LOOP;
}

ایستا صحیح quit_cmd(تهی)
{
	string_list_clear(&del_list, 0);
	printf(_("Bye.\n"));
	بازگشت MENU_RETURN_NO_LOOP;
}

ایستا صحیح help_cmd(تهی)
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
	بازگشت 0;
}

ایستا تهی interactive_main_loop(تهی)
{
	وقتی (del_list.nr) {
		ساختار menu_opts menu_opts;
		ساختار menu_stuff menu_stuff;
		ساختار menu_item menus[] = {
			{'c', "clean",			0, clean_cmd},
			{'f', "filter by pattern",	0, filter_by_patterns_cmd},
			{'s', "select by numbers",	0, select_by_numbers_cmd},
			{'a', "ask each",		0, ask_each_cmd},
			{'q', "quit",			0, quit_cmd},
			{'h', "help",			0, help_cmd},
		};
		صحیح *chosen;

		menu_opts.header = N_("*** Commands ***");
		menu_opts.prompt = N_("What now");
		menu_opts.flags = MENU_OPTS_SINGLETON;

		menu_stuff.type = MENU_STUFF_TYPE_MENU_ITEM;
		menu_stuff.stuff = menus;
		menu_stuff.nr = اندازه(menus) / اندازه(ساختار menu_item);

		clean_print_color(CLEAN_COLOR_HEADER);
		printf_ln(Q_("Would remove the following item:",
			     "Would remove the following items:",
			     del_list.nr));
		clean_print_color(CLEAN_COLOR_RESET);

		pretty_print_dels();

		chosen = list_and_choose(&menu_opts, &menu_stuff);

		اگر (*chosen != EOF) {
			صحیح ret;
			ret = menus[*chosen].fn();
			اگر (ret != MENU_RETURN_NO_LOOP) {
				FREE_AND_NULL(chosen);
				اگر (!del_list.nr) {
					clean_print_color(CLEAN_COLOR_ERROR);
					printf_ln(_("No more files to clean, exiting."));
					clean_print_color(CLEAN_COLOR_RESET);
					بشکن;
				}
				ادامه;
			}
		} وگرنه {
			quit_cmd();
		}

		FREE_AND_NULL(chosen);
		بشکن;
	}
}

ایستا تهی correct_untracked_entries(ساختار dir_struct *dir)
{
	صحیح src, dst, ign;

	برای (src = dst = ign = 0; src < dir->nr; src++) {
		/* skip paths in ignored[] that cannot be inside entries[src] */
		وقتی (ign < dir->ignored_nr &&
		       0 <= cmp_dir_entry(&dir->entries[src], &dir->ignored[ign]))
			ign++;

		اگر (ign < dir->ignored_nr &&
		    check_dir_entry_contains(dir->entries[src], dir->ignored[ign])) {
			/* entries[src] contains an ignored path, so we drop it */
			آزاد(dir->entries[src]);
		} وگرنه {
			ساختار dir_entry *ent = dir->entries[src++];

			/* entries[src] does not contain an ignored path, so we keep it */
			dir->entries[dst++] = ent;

			/* then discard paths in entries[] contained inside entries[src] */
			وقتی (src < dir->nr &&
			       check_dir_entry_contains(ent, dir->entries[src]))
				آزاد(dir->entries[src++]);

			/* compensate for the outer loop's loop control */
			src--;
		}
	}
	dir->nr = dst;
}

صحیح cmd_clean(صحیح argc,
	      ثابت نویسه **argv,
	      ثابت نویسه *prefix,
	      ساختار repository *repo UNUSED)
{
	صحیح i, res;
	صحیح dry_run = 0, remove_directories = 0, quiet = 0, ignored = 0;
	صحیح ignored_only = 0, force = 0, errors = 0, gone = 1;
	صحیح rm_flags = REMOVE_DIR_KEEP_NESTED_GIT;
	ساختار strbuf abs_path = STRBUF_INIT;
	ساختار dir_struct dir = DIR_INIT;
	ساختار pathspec pathspec;
	ساختار strbuf buf = STRBUF_INIT;
	ساختار string_list exclude_list = STRING_LIST_INIT_NODUP;
	ساختار pattern_list *pl;
	ساختار string_list_item *item;
	ثابت نویسه *qname;
	ساختار option options[] = {
		OPT__QUIET(&quiet, N_("do not print names of files removed")),
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('i', "interactive", &interactive, N_("interactive cleaning")),
		OPT_BOOL('d', پوچ, &remove_directories,
				N_("remove whole directories")),
		OPT_CALLBACK_F('e', "exclude", &exclude_list, N_("pattern"),
		  N_("add <pattern> to ignore rules"), PARSE_OPT_NONEG, exclude_cb),
		OPT_BOOL('x', پوچ, &ignored, N_("remove ignored files, too")),
		OPT_BOOL('X', پوچ, &ignored_only,
				N_("remove only ignored files")),
		OPT_END()
	};

	repo_config(the_repository, git_clean_config, پوچ);

	argc = parse_options(argc, argv, prefix, options, builtin_clean_usage,
			     0);

	اگر (require_force != 0 && !force && !interactive && !dry_run)
		die(_("clean.requireForce is true and -f not given: refusing to clean"));

	اگر (force > 1)
		rm_flags = 0;
	وگرنه
		dir.flags |= DIR_SKIP_NESTED_GIT;

	dir.flags |= DIR_SHOW_OTHER_DIRECTORIES;

	اگر (ignored && ignored_only)
		die(_("options '%s' and '%s' cannot be used together"), "-x", "-X");
	اگر (!ignored)
		setup_standard_excludes(&dir);
	اگر (ignored_only)
		dir.flags |= DIR_SHOW_IGNORED;

	اگر (argc) {
		/*
		 * Remaining args implies pathspecs specified, and we should
		 * recurse within those.
		 */
		remove_directories = 1;
	}

	اگر (remove_directories && !ignored_only) {
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
		اگر (!ignored)
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

	اگر (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	برای (i = 0; i < exclude_list.nr; i++)
		add_pattern(exclude_list.اقلام[i].string, "", 0, pl, -(i+1));

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	fill_directory(&dir, the_repository->index, &pathspec);
	correct_untracked_entries(&dir);

	برای (i = 0; i < dir.nr; i++) {
		ساختار dir_entry *ent = dir.entries[i];
		ساختار stat st;
		ثابت نویسه *rel;

		اگر (!index_name_is_other(the_repository->index, ent->name, ent->len))
			ادامه;

		اگر (lstat(ent->name, &st))
			die_errno("Cannot lstat '%s'", ent->name);

		اگر (S_ISDIR(st.st_mode) && !remove_directories)
			ادامه;

		rel = relative_path(ent->name, prefix, &buf);
		string_list_append(&del_list, rel);
	}

	dir_clear(&dir);

	اگر (interactive && del_list.nr > 0)
		interactive_main_loop();

	for_each_string_list_item(item, &del_list) {
		ساختار stat st;

		strbuf_reset(&abs_path);
		اگر (prefix)
			strbuf_addstr(&abs_path, prefix);

		strbuf_addstr(&abs_path, item->string);

		/*
		 * we might have removed this as part of earlier
		 * recursive directory removal, so lstat() here could
		 * fail with ENOENT.
		 */
		اگر (lstat(abs_path.buf, &st))
			ادامه;

		اگر (S_ISDIR(st.st_mode)) {
			اگر (remove_dirs(&abs_path, prefix, rm_flags, dry_run, quiet, &gone))
				errors++;
			اگر (gone && !quiet) {
				qname = quote_path(item->string, پوچ, &buf, 0);
				printf(dry_run ? _(پیام_would_remove) : _(پیام_remove), qname);
			}
		} وگرنه {
			res = dry_run ? 0 : unlink(abs_path.buf);
			اگر (res) {
				صحیح saved_errno = errno;
				qname = quote_path(item->string, پوچ, &buf, 0);
				errno = saved_errno;
				warning_errno(_(پیام_warn_remove_failed), qname);
				errors++;
			} وگرنه اگر (!quiet) {
				qname = quote_path(item->string, پوچ, &buf, 0);
				printf(dry_run ? _(پیام_would_remove) : _(پیام_remove), qname);
			}
		}
	}

	strbuf_release(&abs_path);
	strbuf_release(&buf);
	string_list_clear(&del_list, 0);
	string_list_clear(&exclude_list, 0);
	clear_pathspec(&pathspec);
	بازگشت (errors != 0);
}
