/*
 * "git clean" builtin command
 *
 * Copyright (C) 2007 Shawn Bohrer
 *
 * Based on git-clean.sh by Pavel Roskin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "abspath.h"
#ضمن "config.h"
#ضمن "dir.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "parse-options.h"
#ضمن "path.h"
#ضمن "read-cache-ll.h"
#ضمن "setup.h"
#ضمن "string-list.h"
#ضمن "quote.h"
#ضمن "column.h"
#ضمن "color.h"
#ضمن "pathspec.h"
#ضمن "help.h"
#ضمن "prompt.h"

ساكن صحيح require_force = -1; /* unset */
ساكن صحيح interactive;
ساكن بنية string_list del_list = STRING_LIST_INIT_DUP;
ساكن unsigned صحيح colopts;

ساكن ثابت حرف *ثابت builtin_clean_usage[] = {
	N_("git clean [-d] [-f] [-i] [-n] [-q] [-e <pattern>] [-x | -X] [--] [<pathspec>...]"),
	عدم
};

ساكن ثابت حرف *msg_remove = N_("Removing %s\n");
ساكن ثابت حرف *msg_would_remove = N_("Would remove %s\n");
ساكن ثابت حرف *msg_skip_git_dir = N_("Skipping repository %s\n");
ساكن ثابت حرف *msg_would_skip_git_dir = N_("Would skip repository %s\n");
ساكن ثابت حرف *msg_warn_remove_failed = N_("failed to remove %s");
ساكن ثابت حرف *msg_warn_lstat_failed = N_("could not lstat %s\n");
ساكن ثابت حرف *msg_skip_cwd = N_("Refusing to remove current working directory\n");
ساكن ثابت حرف *msg_would_skip_cwd = N_("Would refuse to remove current working directory\n");

تعداد color_clean {
	CLEAN_COLOR_RESET = 0,
	CLEAN_COLOR_PLAIN = 1,
	CLEAN_COLOR_PROMPT = 2,
	CLEAN_COLOR_HEADER = 3,
	CLEAN_COLOR_HELP = 4,
	CLEAN_COLOR_ERROR = 5
};

ساكن ثابت حرف *color_interactive_slots[] = {
	[CLEAN_COLOR_ERROR]  = "error",
	[CLEAN_COLOR_HEADER] = "header",
	[CLEAN_COLOR_HELP]   = "help",
	[CLEAN_COLOR_PLAIN]  = "plain",
	[CLEAN_COLOR_PROMPT] = "prompt",
	[CLEAN_COLOR_RESET]  = "reset",
};

ساكن تعداد git_colorbool clean_use_color = GIT_COLOR_UNKNOWN;
ساكن حرف clean_colors[][COLOR_MAXLEN] = {
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

بنية menu_opts {
	ثابت حرف *header;
	ثابت حرف *prompt;
	صحيح flags;
};

#define MENU_RETURN_NO_LOOP		10

بنية menu_item {
	حرف hotkey;
	ثابت حرف *title;
	صحيح selected;
	صحيح (*fn)(فراغ);
};

تعداد menu_stuff_type {
	MENU_STUFF_TYPE_STRING_LIST = 1,
	MENU_STUFF_TYPE_MENU_ITEM
};

بنية menu_stuff {
	تعداد menu_stuff_type type;
	صحيح nr;
	فراغ *stuff;
};

define_list_config_array(color_interactive_slots);

ساكن صحيح git_clean_config(ثابت حرف *var, ثابت حرف *value,
			    ثابت بنية config_context *ctx, فراغ *cb)
{
	ثابت حرف *slot_name;

	ܐܢ (starts_with(var, "column."))
		ܦܢܐ git_column_config(var, value, "clean", &colopts);

	/* honors the color.interactive* config variables which also
	   applied in git-add--interactive and git-stash */
	ܐܢ (!strcmp(var, "color.interactive")) {
		clean_use_color = git_config_colorbool(var, value);
		ܦܢܐ 0;
	}
	ܐܢ (skip_prefix(var, "color.interactive.", &slot_name)) {
		صحيح slot = LOOKUP_CONFIG(color_interactive_slots, slot_name);
		ܐܢ (slot < 0)
			ܦܢܐ 0;
		ܐܢ (!value)
			ܦܢܐ config_error_nonbool(var);
		ܦܢܐ color_parse(value, clean_colors[slot]);
	}

	ܐܢ (!strcmp(var, "clean.requireforce")) {
		require_force = git_config_bool(var, value);
		ܦܢܐ 0;
	}

	ܐܢ (git_color_config(var, value, cb) < 0)
		ܦܢܐ -1;

	ܦܢܐ git_default_config(var, value, ctx, cb);
}

ساكن ثابت حرف *clean_get_color(تعداد color_clean ix)
{
	ܐܢ (want_color(clean_use_color))
		ܦܢܐ clean_colors[ix];
	ܦܢܐ "";
}

ساكن فراغ clean_print_color(تعداد color_clean ix)
{
	printf("%s", clean_get_color(ix));
}

ساكن صحيح exclude_cb(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية string_list *exclude_list = opt->value;
	BUG_ON_OPT_NEG(unset);
	string_list_append(exclude_list, arg);
	ܦܢܐ 0;
}

ساكن صحيح remove_dirs(بنية strbuf *path, ثابت حرف *prefix, صحيح force_flag,
		صحيح dry_run, صحيح quiet, صحيح *dir_gone)
{
	DIR *dir;
	بنية strbuf quoted = STRBUF_INIT;
	بنية strbuf realpath = STRBUF_INIT;
	بنية strbuf real_ocwd = STRBUF_INIT;
	بنية dirent *e;
	صحيح res = 0, ret = 0, gone = 1, original_len = path->len, len;
	بنية string_list dels = STRING_LIST_INIT_DUP;

	*dir_gone = 1;

	ܐܢ ((force_flag & REMOVE_DIR_KEEP_NESTED_GIT) &&
	    is_nonbare_repository_dir(path)) {
		ܐܢ (!quiet) {
			quote_path(path->buf, prefix, &quoted, 0);
			printf(dry_run ?  _(msg_would_skip_git_dir) : _(msg_skip_git_dir),
					quoted.buf);
		}

		*dir_gone = 0;
		اذهب out;
	}

	dir = opendir(path->buf);
	ܐܢ (!dir) {
		/* an empty dir could be removed even if it is unreadble */
		res = dry_run ? 0 : rmdir(path->buf);
		ܐܢ (res) {
			صحيح saved_errno = errno;
			quote_path(path->buf, prefix, &quoted, 0);
			errno = saved_errno;
			warning_errno(_(msg_warn_remove_failed), quoted.buf);
			*dir_gone = 0;
		}
		ret = res;
		اذهب out;
	}

	strbuf_complete(path, '/');

	len = path->len;
	ܟܡܐ ((e = readdir_skip_dot_and_dotdot(dir)) != عدم) {
		بنية stat st;

		strbuf_setlen(path, len);
		strbuf_addstr(path, e->d_name);
		ܐܢ (lstat(path->buf, &st))
			warning_errno(_(msg_warn_lstat_failed), path->buf);
		ܐܚܪܢܐ ܐܢ (S_ISDIR(st.st_mode)) {
			ܐܢ (remove_dirs(path, prefix, force_flag, dry_run, quiet, &gone))
				ret = 1;
			ܐܢ (gone) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} ܐܚܪܢܐ
				*dir_gone = 0;
			استمر;
		} ܐܚܪܢܐ {
			res = dry_run ? 0 : unlink(path->buf);
			ܐܢ (!res) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} ܐܚܪܢܐ {
				صحيح saved_errno = errno;
				quote_path(path->buf, prefix, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(msg_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
			استمر;
		}

		/* path too long, stat fails, or non-directory still exists */
		*dir_gone = 0;
		ret = 1;
		اكسر;
	}
	closedir(dir);

	strbuf_setlen(path, original_len);

	ܐܢ (*dir_gone) {
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
		 ܐܢ (startup_info->original_cwd)
			 strbuf_realpath(&real_ocwd,
					 startup_info->original_cwd, 1);

		ܐܢ (!strbuf_cmp(&realpath, &real_ocwd)) {
			printf("%s", dry_run ? _(msg_would_skip_cwd) : _(msg_skip_cwd));
			*dir_gone = 0;
		} ܐܚܪܢܐ {
			res = dry_run ? 0 : rmdir(path->buf);
			ܐܢ (!res)
				*dir_gone = 1;
			ܐܚܪܢܐ {
				صحيح saved_errno = errno;
				quote_path(path->buf, prefix, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(msg_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
		}
	}

	ܐܢ (!*dir_gone && !quiet) {
		صحيح i;
		لكل (i = 0; i < dels.nr; i++)
			printf(dry_run ?  _(msg_would_remove) : _(msg_remove), dels.items[i].string);
	}
out:
	strbuf_release(&realpath);
	strbuf_release(&real_ocwd);
	strbuf_release(&quoted);
	string_list_clear(&dels, 0);
	ܦܢܐ ret;
}

ساكن فراغ pretty_print_dels(فراغ)
{
	بنية string_list list = STRING_LIST_INIT_DUP;
	بنية string_list_item *item;
	بنية strbuf buf = STRBUF_INIT;
	ثابت حرف *qname;
	بنية column_options copts;

	for_each_string_list_item(item, &del_list) {
		qname = quote_path(item->string, عدم, &buf, 0);
		string_list_append(&list, qname);
	}

	/*
	 * always enable column display, we only consult column.*
	 * about layout strategy and stuff
	 */
	colopts = (colopts & ~COL_ENABLE_MASK) | COL_ENABLED;
	memset(&copts, 0, حجم(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(&list, colopts, &copts);
	strbuf_release(&buf);
	string_list_clear(&list, 0);
}

ساكن فراغ pretty_print_menus(بنية string_list *menu_list)
{
	unsigned صحيح local_colopts = 0;
	بنية column_options copts;

	local_colopts = COL_ENABLED | COL_ROW;
	memset(&copts, 0, حجم(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(menu_list, local_colopts, &copts);
}

ساكن فراغ prompt_help_cmd(صحيح singleton)
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
ساكن فراغ print_highlight_menu_stuff(بنية menu_stuff *stuff, صحيح **chosen)
{
	بنية string_list menu_list = STRING_LIST_INIT_DUP;
	بنية strbuf menu = STRBUF_INIT;
	بنية menu_item *menu_item;
	بنية string_list_item *string_list_item;
	صحيح i;

	بدل (stuff->type) {
	افتراضي:
		die("Bad type of menu_stuff when print menu");
	حالة MENU_STUFF_TYPE_MENU_ITEM:
		menu_item = (بنية menu_item *)stuff->stuff;
		لكل (i = 0; i < stuff->nr; i++, menu_item++) {
			ثابت حرف *p;
			صحيح highlighted = 0;

			p = menu_item->title;
			ܐܢ ((*chosen)[i] < 0)
				(*chosen)[i] = menu_item->selected ? 1 : 0;
			strbuf_addf(&menu, "%s%2d: ", (*chosen)[i] ? "*" : " ", i+1);
			لكل (; *p; p++) {
				ܐܢ (!highlighted && *p == menu_item->hotkey) {
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_PROMPT));
					strbuf_addch(&menu, *p);
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_RESET));
					highlighted = 1;
				} ܐܚܪܢܐ {
					strbuf_addch(&menu, *p);
				}
			}
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
		}
		اكسر;
	حالة MENU_STUFF_TYPE_STRING_LIST:
		i = 0;
		for_each_string_list_item(string_list_item, (بنية string_list *)stuff->stuff) {
			ܐܢ ((*chosen)[i] < 0)
				(*chosen)[i] = 0;
			strbuf_addf(&menu, "%s%2d: %s",
				    (*chosen)[i] ? "*" : " ", i+1, string_list_item->string);
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
			i++;
		}
		اكسر;
	}

	pretty_print_menus(&menu_list);

	strbuf_release(&menu);
	string_list_clear(&menu_list, 0);
}

ساكن صحيح find_unique(ثابت حرف *choice, بنية menu_stuff *menu_stuff)
{
	بنية menu_item *menu_item;
	بنية string_list_item *string_list_item;
	صحيح i, len, found = 0;

	len = strlen(choice);
	بدل (menu_stuff->type) {
	افتراضي:
		die("Bad type of menu_stuff when parse choice");
	حالة MENU_STUFF_TYPE_MENU_ITEM:

		menu_item = (بنية menu_item *)menu_stuff->stuff;
		لكل (i = 0; i < menu_stuff->nr; i++, menu_item++) {
			ܐܢ (len == 1 && *choice == menu_item->hotkey) {
				found = i + 1;
				اكسر;
			}
			ܐܢ (!strncasecmp(choice, menu_item->title, len)) {
				ܐܢ (found) {
					ܐܢ (len == 1) {
						/* continue for hotkey matching */
						found = -1;
					} ܐܚܪܢܐ {
						found = 0;
						اكسر;
					}
				} ܐܚܪܢܐ {
					found = i + 1;
				}
			}
		}
		اكسر;
	حالة MENU_STUFF_TYPE_STRING_LIST:
		string_list_item = ((بنية string_list *)menu_stuff->stuff)->items;
		لكل (i = 0; i < menu_stuff->nr; i++, string_list_item++) {
			ܐܢ (!strncasecmp(choice, string_list_item->string, len)) {
				ܐܢ (found) {
					found = 0;
					اكسر;
				}
				found = i + 1;
			}
		}
		اكسر;
	}
	ܦܢܐ found;
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
ساكن صحيح parse_choice(بنية menu_stuff *menu_stuff,
			صحيح is_single,
			حرف *input,
			صحيح **chosen)
{
	بنية string_list choice = STRING_LIST_INIT_NODUP;
	بنية string_list_item *item;
	صحيح nr = 0;
	صحيح i;

	string_list_split_in_place_f(&choice, input,
				     is_single ? "\n" : ", ", -1,
				     STRING_LIST_SPLIT_TRIM);

	for_each_string_list_item(item, &choice) {
		ثابت حرف *string;
		صحيح choose;
		صحيح bottom = 0, top = 0;
		صحيح is_range, is_number;

		string = item->string;
		ܐܢ (!*string)
			استمر;

		/* Input that begins with '-'; unchoose */
		ܐܢ (string[0] == '-') {
			choose = 0;
			string++;
		} ܐܚܪܢܐ {
			choose = 1;
		}

		is_range = 0;
		is_number = 1;
		لكل (ثابت حرف *p = string; *p; p++) {
			ܐܢ ('-' == *p) {
				ܐܢ (!is_range) {
					is_range = 1;
					is_number = 0;
				} ܐܚܪܢܐ {
					is_number = 0;
					is_range = 0;
					اكسر;
				}
			} ܐܚܪܢܐ ܐܢ (!isdigit(*p)) {
				is_number = 0;
				is_range = 0;
				اكسر;
			}
		}

		ܐܢ (is_number) {
			bottom = atoi(string);
			top = bottom;
		} ܐܚܪܢܐ ܐܢ (is_range) {
			bottom = atoi(string);
			/* a range can be specified like 5-7 or 5- */
			ܐܢ (!*(strchr(string, '-') + 1))
				top = menu_stuff->nr;
			ܐܚܪܢܐ
				top = atoi(strchr(string, '-') + 1);
		} ܐܚܪܢܐ ܐܢ (!strcmp(string, "*")) {
			bottom = 1;
			top = menu_stuff->nr;
		} ܐܚܪܢܐ {
			bottom = find_unique(string, menu_stuff);
			top = bottom;
		}

		ܐܢ (top <= 0 || bottom <= 0 || top > menu_stuff->nr || bottom > top ||
		    (is_single && bottom != top)) {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf(_("Huh (%s)?\n"), string);
			clean_print_color(CLEAN_COLOR_RESET);
			استمر;
		}

		لكل (i = bottom; i <= top; i++)
			(*chosen)[i-1] = choose;
	}

	string_list_clear(&choice, 0);

	لكل (i = 0; i < menu_stuff->nr; i++)
		nr += (*chosen)[i];
	ܦܢܐ nr;
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
ساكن صحيح *list_and_choose(بنية menu_opts *opts, بنية menu_stuff *stuff)
{
	بنية strbuf choice = STRBUF_INIT;
	صحيح *chosen, *result;
	صحيح nr = 0;
	صحيح eof = 0;
	صحيح i;

	ALLOC_ARRAY(chosen, stuff->nr);
	/* set chosen as uninitialized */
	لكل (i = 0; i < stuff->nr; i++)
		chosen[i] = -1;

	لكل (;;) {
		ܐܢ (opts->header) {
			printf_ln("%s%s%s",
				  clean_get_color(CLEAN_COLOR_HEADER),
				  _(opts->header),
				  clean_get_color(CLEAN_COLOR_RESET));
		}

		/* chosen will be initialized by print_highlight_menu_stuff */
		print_highlight_menu_stuff(stuff, &chosen);

		ܐܢ (opts->flags & MENU_OPTS_LIST_ONLY)
			اكسر;

		ܐܢ (opts->prompt) {
			printf("%s%s%s%s",
			       clean_get_color(CLEAN_COLOR_PROMPT),
			       _(opts->prompt),
			       opts->flags & MENU_OPTS_SINGLETON ? "> " : ">> ",
			       clean_get_color(CLEAN_COLOR_RESET));
		}

		ܐܢ (git_read_line_interactively(&choice) == EOF) {
			eof = 1;
			اكسر;
		}

		/* help for prompt */
		ܐܢ (!strcmp(choice.buf, "?")) {
			prompt_help_cmd(opts->flags & MENU_OPTS_SINGLETON);
			استمر;
		}

		/* for a multiple-choice menu, press ENTER (empty) will return back */
		ܐܢ (!(opts->flags & MENU_OPTS_SINGLETON) && !choice.len)
			اكسر;

		nr = parse_choice(stuff,
				  opts->flags & MENU_OPTS_SINGLETON,
				  choice.buf,
				  &chosen);

		ܐܢ (opts->flags & MENU_OPTS_SINGLETON) {
			ܐܢ (nr)
				اكسر;
		} ܐܚܪܢܐ ܐܢ (opts->flags & MENU_OPTS_IMMEDIATE) {
			اكسر;
		}
	}

	ܐܢ (eof) {
		result = xmalloc(حجم(صحيح));
		*result = EOF;
	} ܐܚܪܢܐ {
		صحيح j = 0;

		/*
		 * recalculate nr, if return back from menu directly with
		 * default selections.
		 */
		ܐܢ (!nr) {
			لكل (i = 0; i < stuff->nr; i++)
				nr += chosen[i];
		}

		CALLOC_ARRAY(result, st_add(nr, 1));
		لكل (i = 0; i < stuff->nr && j < nr; i++) {
			ܐܢ (chosen[i])
				result[j++] = i;
		}
		result[j] = EOF;
	}

	حرر(chosen);
	strbuf_release(&choice);
	ܦܢܐ result;
}

ساكن صحيح clean_cmd(فراغ)
{
	ܦܢܐ MENU_RETURN_NO_LOOP;
}

ساكن صحيح filter_by_patterns_cmd(فراغ)
{
	بنية dir_struct dir = DIR_INIT;
	بنية strbuf confirm = STRBUF_INIT;
	بنية pattern_list *pl;
	صحيح changed = -1, i;

	لكل (;;) {
		بنية string_list ignore_list = STRING_LIST_INIT_NODUP;
		بنية string_list_item *item;

		ܐܢ (!del_list.nr)
			اكسر;

		ܐܢ (changed)
			pretty_print_dels();

		clean_print_color(CLEAN_COLOR_PROMPT);
		printf(_("Input ignore patterns>> "));
		clean_print_color(CLEAN_COLOR_RESET);
		ܐܢ (git_read_line_interactively(&confirm) == EOF)
			putchar('\n');

		/* quit filter_by_pattern mode if press ENTER or Ctrl-D */
		ܐܢ (!confirm.len)
			اكسر;

		pl = add_pattern_list(&dir, EXC_CMDL, "manual exclude");

		string_list_split_in_place_f(&ignore_list, confirm.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);

		لكل (i = 0; i < ignore_list.nr; i++) {
			item = &ignore_list.items[i];
			ܐܢ (!*item->string)
				استمر;
			add_pattern(item->string, "", 0, pl, -(i+1));
		}

		changed = 0;
		for_each_string_list_item(item, &del_list) {
			صحيح dtype = DT_UNKNOWN;

			ܐܢ (is_excluded(&dir, the_repository->index, item->string, &dtype)) {
				*item->string = '\0';
				changed++;
			}
		}

		ܐܢ (changed) {
			string_list_remove_empty_items(&del_list, 0);
		} ܐܚܪܢܐ {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf_ln(_("WARNING: Cannot find items matched by: %s"), confirm.buf);
			clean_print_color(CLEAN_COLOR_RESET);
		}

		string_list_clear(&ignore_list, 0);
		dir_clear(&dir);
	}

	strbuf_release(&confirm);
	ܦܢܐ 0;
}

ساكن صحيح select_by_numbers_cmd(فراغ)
{
	بنية menu_opts menu_opts;
	بنية menu_stuff menu_stuff;
	بنية string_list_item *items;
	صحيح *chosen;
	صحيح i, j;

	menu_opts.header = عدم;
	menu_opts.prompt = N_("Select items to delete");
	menu_opts.flags = 0;

	menu_stuff.type = MENU_STUFF_TYPE_STRING_LIST;
	menu_stuff.stuff = &del_list;
	menu_stuff.nr = del_list.nr;

	chosen = list_and_choose(&menu_opts, &menu_stuff);
	items = del_list.items;
	لكل (i = 0, j = 0; i < del_list.nr; i++) {
		ܐܢ (i < chosen[j]) {
			*(items[i].string) = '\0';
		} ܐܚܪܢܐ ܐܢ (i == chosen[j]) {
			/* delete selected item */
			j++;
			استمر;
		} ܐܚܪܢܐ {
			/* end of chosen (chosen[j] == EOF), won't delete */
			*(items[i].string) = '\0';
		}
	}

	string_list_remove_empty_items(&del_list, 0);

	حرر(chosen);
	ܦܢܐ 0;
}

ساكن صحيح ask_each_cmd(فراغ)
{
	بنية strbuf confirm = STRBUF_INIT;
	بنية strbuf buf = STRBUF_INIT;
	بنية string_list_item *item;
	ثابت حرف *qname;
	صحيح changed = 0, eof = 0;

	for_each_string_list_item(item, &del_list) {
		/* Ctrl-D should stop removing files */
		ܐܢ (!eof) {
			qname = quote_path(item->string, عدم, &buf, 0);
			/* TRANSLATORS: Make sure to keep [y/N] as is */
			printf(_("Remove %s [y/N]? "), qname);
			ܐܢ (git_read_line_interactively(&confirm) == EOF) {
				putchar('\n');
				eof = 1;
			}
		}
		ܐܢ (!confirm.len || strncasecmp(confirm.buf, "yes", confirm.len)) {
			*item->string = '\0';
			changed++;
		}
	}

	ܐܢ (changed)
		string_list_remove_empty_items(&del_list, 0);

	strbuf_release(&buf);
	strbuf_release(&confirm);
	ܦܢܐ MENU_RETURN_NO_LOOP;
}

ساكن صحيح quit_cmd(فراغ)
{
	string_list_clear(&del_list, 0);
	printf(_("Bye.\n"));
	ܦܢܐ MENU_RETURN_NO_LOOP;
}

ساكن صحيح help_cmd(فراغ)
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
	ܦܢܐ 0;
}

ساكن فراغ interactive_main_loop(فراغ)
{
	ܟܡܐ (del_list.nr) {
		بنية menu_opts menu_opts;
		بنية menu_stuff menu_stuff;
		بنية menu_item menus[] = {
			{'c', "clean",			0, clean_cmd},
			{'f', "filter by pattern",	0, filter_by_patterns_cmd},
			{'s', "select by numbers",	0, select_by_numbers_cmd},
			{'a', "ask each",		0, ask_each_cmd},
			{'q', "quit",			0, quit_cmd},
			{'h', "help",			0, help_cmd},
		};
		صحيح *chosen;

		menu_opts.header = N_("*** Commands ***");
		menu_opts.prompt = N_("What now");
		menu_opts.flags = MENU_OPTS_SINGLETON;

		menu_stuff.type = MENU_STUFF_TYPE_MENU_ITEM;
		menu_stuff.stuff = menus;
		menu_stuff.nr = حجم(menus) / حجم(بنية menu_item);

		clean_print_color(CLEAN_COLOR_HEADER);
		printf_ln(Q_("Would remove the following item:",
			     "Would remove the following items:",
			     del_list.nr));
		clean_print_color(CLEAN_COLOR_RESET);

		pretty_print_dels();

		chosen = list_and_choose(&menu_opts, &menu_stuff);

		ܐܢ (*chosen != EOF) {
			صحيح ret;
			ret = menus[*chosen].fn();
			ܐܢ (ret != MENU_RETURN_NO_LOOP) {
				FREE_AND_NULL(chosen);
				ܐܢ (!del_list.nr) {
					clean_print_color(CLEAN_COLOR_ERROR);
					printf_ln(_("No more files to clean, exiting."));
					clean_print_color(CLEAN_COLOR_RESET);
					اكسر;
				}
				استمر;
			}
		} ܐܚܪܢܐ {
			quit_cmd();
		}

		FREE_AND_NULL(chosen);
		اكسر;
	}
}

ساكن فراغ correct_untracked_entries(بنية dir_struct *dir)
{
	صحيح src, dst, ign;

	لكل (src = dst = ign = 0; src < dir->nr; src++) {
		/* skip paths in ignored[] that cannot be inside entries[src] */
		ܟܡܐ (ign < dir->ignored_nr &&
		       0 <= cmp_dir_entry(&dir->entries[src], &dir->ignored[ign]))
			ign++;

		ܐܢ (ign < dir->ignored_nr &&
		    check_dir_entry_contains(dir->entries[src], dir->ignored[ign])) {
			/* entries[src] contains an ignored path, so we drop it */
			حرر(dir->entries[src]);
		} ܐܚܪܢܐ {
			بنية dir_entry *ent = dir->entries[src++];

			/* entries[src] does not contain an ignored path, so we keep it */
			dir->entries[dst++] = ent;

			/* then discard paths in entries[] contained inside entries[src] */
			ܟܡܐ (src < dir->nr &&
			       check_dir_entry_contains(ent, dir->entries[src]))
				حرر(dir->entries[src++]);

			/* compensate for the outer loop's loop control */
			src--;
		}
	}
	dir->nr = dst;
}

صحيح cmd_clean(صحيح argc,
	      ثابت حرف **argv,
	      ثابت حرف *prefix,
	      بنية repository *repo UNUSED)
{
	صحيح i, res;
	صحيح dry_run = 0, remove_directories = 0, quiet = 0, ignored = 0;
	صحيح ignored_only = 0, force = 0, errors = 0, gone = 1;
	صحيح rm_flags = REMOVE_DIR_KEEP_NESTED_GIT;
	بنية strbuf abs_path = STRBUF_INIT;
	بنية dir_struct dir = DIR_INIT;
	بنية pathspec pathspec;
	بنية strbuf buf = STRBUF_INIT;
	بنية string_list exclude_list = STRING_LIST_INIT_NODUP;
	بنية pattern_list *pl;
	بنية string_list_item *item;
	ثابت حرف *qname;
	بنية option options[] = {
		OPT__QUIET(&quiet, N_("do not print names of files removed")),
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('i', "interactive", &interactive, N_("interactive cleaning")),
		OPT_BOOL('d', عدم, &remove_directories,
				N_("remove whole directories")),
		OPT_CALLBACK_F('e', "exclude", &exclude_list, N_("pattern"),
		  N_("add <pattern> to ignore rules"), PARSE_OPT_NONEG, exclude_cb),
		OPT_BOOL('x', عدم, &ignored, N_("remove ignored files, too")),
		OPT_BOOL('X', عدم, &ignored_only,
				N_("remove only ignored files")),
		OPT_END()
	};

	repo_config(the_repository, git_clean_config, عدم);

	argc = parse_options(argc, argv, prefix, options, builtin_clean_usage,
			     0);

	ܐܢ (require_force != 0 && !force && !interactive && !dry_run)
		die(_("clean.requireForce is true and -f not given: refusing to clean"));

	ܐܢ (force > 1)
		rm_flags = 0;
	ܐܚܪܢܐ
		dir.flags |= DIR_SKIP_NESTED_GIT;

	dir.flags |= DIR_SHOW_OTHER_DIRECTORIES;

	ܐܢ (ignored && ignored_only)
		die(_("options '%s' and '%s' cannot be used together"), "-x", "-X");
	ܐܢ (!ignored)
		setup_standard_excludes(&dir);
	ܐܢ (ignored_only)
		dir.flags |= DIR_SHOW_IGNORED;

	ܐܢ (argc) {
		/*
		 * Remaining args implies pathspecs specified, and we should
		 * recurse within those.
		 */
		remove_directories = 1;
	}

	ܐܢ (remove_directories && !ignored_only) {
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
		ܐܢ (!ignored)
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

	ܐܢ (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	لكل (i = 0; i < exclude_list.nr; i++)
		add_pattern(exclude_list.items[i].string, "", 0, pl, -(i+1));

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	fill_directory(&dir, the_repository->index, &pathspec);
	correct_untracked_entries(&dir);

	لكل (i = 0; i < dir.nr; i++) {
		بنية dir_entry *ent = dir.entries[i];
		بنية stat st;
		ثابت حرف *rel;

		ܐܢ (!index_name_is_other(the_repository->index, ent->name, ent->len))
			استمر;

		ܐܢ (lstat(ent->name, &st))
			die_errno("Cannot lstat '%s'", ent->name);

		ܐܢ (S_ISDIR(st.st_mode) && !remove_directories)
			استمر;

		rel = relative_path(ent->name, prefix, &buf);
		string_list_append(&del_list, rel);
	}

	dir_clear(&dir);

	ܐܢ (interactive && del_list.nr > 0)
		interactive_main_loop();

	for_each_string_list_item(item, &del_list) {
		بنية stat st;

		strbuf_reset(&abs_path);
		ܐܢ (prefix)
			strbuf_addstr(&abs_path, prefix);

		strbuf_addstr(&abs_path, item->string);

		/*
		 * we might have removed this as part of earlier
		 * recursive directory removal, so lstat() here could
		 * fail with ENOENT.
		 */
		ܐܢ (lstat(abs_path.buf, &st))
			استمر;

		ܐܢ (S_ISDIR(st.st_mode)) {
			ܐܢ (remove_dirs(&abs_path, prefix, rm_flags, dry_run, quiet, &gone))
				errors++;
			ܐܢ (gone && !quiet) {
				qname = quote_path(item->string, عدم, &buf, 0);
				printf(dry_run ? _(msg_would_remove) : _(msg_remove), qname);
			}
		} ܐܚܪܢܐ {
			res = dry_run ? 0 : unlink(abs_path.buf);
			ܐܢ (res) {
				صحيح saved_errno = errno;
				qname = quote_path(item->string, عدم, &buf, 0);
				errno = saved_errno;
				warning_errno(_(msg_warn_remove_failed), qname);
				errors++;
			} ܐܚܪܢܐ ܐܢ (!quiet) {
				qname = quote_path(item->string, عدم, &buf, 0);
				printf(dry_run ? _(msg_would_remove) : _(msg_remove), qname);
			}
		}
	}

	strbuf_release(&abs_path);
	strbuf_release(&buf);
	string_list_clear(&del_list, 0);
	string_list_clear(&exclude_list, 0);
	clear_pathspec(&pathspec);
	ܦܢܐ (errors != 0);
}
