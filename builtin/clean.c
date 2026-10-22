/*
 * "git clean" builtin command
 *
 * Copyright (C) 2007 Shawn Bohrer
 *
 * Based on git-clean.sh by Pavel Roskin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "abspath.h"
#include "config.h"
#include "dir.h"
#include "environment.h"
#include "gettext.h"
#include "parse-options.h"
#include "path.h"
#include "read-cache-ll.h"
#include "setup.h"
#include "string-list.h"
#include "quote.h"
#include "column.h"
#include "color.h"
#include "pathspec.h"
#include "help.h"
#include "prompt.h"

static 整数 require_force = -1; /* unset */
static 整数 interactive;
static struct string_list del_list = STRING_LIST_INIT_DUP;
static unsigned 整数 colopts;

static const char *const builtin_clean_usage[] = {
	N_("git clean [-d] [-f] [-i] [-n] [-q] [-e <pattern>] [-x | -X] [--] [<pathspec>...]"),
	NULL
};

static const char *msg_remove = N_("Removing %s\n");
static const char *msg_would_remove = N_("Would remove %s\n");
static const char *msg_skip_git_dir = N_("Skipping repository %s\n");
static const char *msg_would_skip_git_dir = N_("Would skip repository %s\n");
static const char *msg_warn_remove_failed = N_("failed to remove %s");
static const char *msg_warn_lstat_failed = N_("could not lstat %s\n");
static const char *msg_skip_cwd = N_("Refusing to remove current working directory\n");
static const char *msg_would_skip_cwd = N_("Would refuse to remove current working directory\n");

enum color_clean {
	CLEAN_COLOR_RESET = 0,
	CLEAN_COLOR_PLAIN = 1,
	CLEAN_COLOR_PROMPT = 2,
	CLEAN_COLOR_HEADER = 3,
	CLEAN_COLOR_HELP = 4,
	CLEAN_COLOR_ERROR = 5
};

static const char *color_interactive_slots[] = {
	[CLEAN_COLOR_ERROR]  = "error",
	[CLEAN_COLOR_HEADER] = "header",
	[CLEAN_COLOR_HELP]   = "help",
	[CLEAN_COLOR_PLAIN]  = "plain",
	[CLEAN_COLOR_PROMPT] = "prompt",
	[CLEAN_COLOR_RESET]  = "reset",
};

static enum git_colorbool clean_use_color = GIT_COLOR_UNKNOWN;
static char clean_colors[][COLOR_MAXLEN] = {
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

struct menu_opts {
	const char *header;
	const char *prompt;
	整数 flags;
};

#define MENU_RETURN_NO_LOOP		10

struct menu_item {
	char hotkey;
	const char *title;
	整数 selected;
	整数 (*fn)(void);
};

enum menu_stuff_type {
	MENU_STUFF_TYPE_STRING_LIST = 1,
	MENU_STUFF_TYPE_MENU_ITEM
};

struct menu_stuff {
	enum menu_stuff_type type;
	整数 nr;
	void *stuff;
};

define_list_config_array(color_interactive_slots);

static 整数 git_clean_config(const char *var, const char *value,
			    const struct config_context *ctx, void *cb)
{
	const char *slot_name;

	もし (starts_with(var, "column."))
		戻る git_column_config(var, value, "clean", &colopts);

	/* honors the color.interactive* config variables which also
	   applied in git-add--interactive and git-stash */
	もし (!strcmp(var, "color.interactive")) {
		clean_use_color = git_config_colorbool(var, value);
		戻る 0;
	}
	もし (skip_prefix(var, "color.interactive.", &slot_name)) {
		整数 slot = LOOKUP_CONFIG(color_interactive_slots, slot_name);
		もし (slot < 0)
			戻る 0;
		もし (!value)
			戻る config_error_nonbool(var);
		戻る color_parse(value, clean_colors[slot]);
	}

	もし (!strcmp(var, "clean.requireforce")) {
		require_force = git_config_bool(var, value);
		戻る 0;
	}

	もし (git_color_config(var, value, cb) < 0)
		戻る -1;

	戻る git_default_config(var, value, ctx, cb);
}

static const char *clean_get_color(enum color_clean ix)
{
	もし (want_color(clean_use_color))
		戻る clean_colors[ix];
	戻る "";
}

static void clean_print_color(enum color_clean ix)
{
	printf("%s", clean_get_color(ix));
}

static 整数 exclude_cb(const struct option *opt, const char *arg, 整数 unset)
{
	struct string_list *exclude_list = opt->value;
	BUG_ON_OPT_NEG(unset);
	string_list_append(exclude_list, arg);
	戻る 0;
}

static 整数 remove_dirs(struct strbuf *path, const char *prefix, 整数 force_flag,
		整数 dry_run, 整数 quiet, 整数 *dir_gone)
{
	DIR *dir;
	struct strbuf quoted = STRBUF_INIT;
	struct strbuf realpath = STRBUF_INIT;
	struct strbuf real_ocwd = STRBUF_INIT;
	struct dirent *e;
	整数 res = 0, ret = 0, gone = 1, original_len = path->len, len;
	struct string_list dels = STRING_LIST_INIT_DUP;

	*dir_gone = 1;

	もし ((force_flag & REMOVE_DIR_KEEP_NESTED_GIT) &&
	    is_nonbare_repository_dir(path)) {
		もし (!quiet) {
			quote_path(path->buf, prefix, &quoted, 0);
			printf(dry_run ?  _(msg_would_skip_git_dir) : _(msg_skip_git_dir),
					quoted.buf);
		}

		*dir_gone = 0;
		goto out;
	}

	dir = opendir(path->buf);
	もし (!dir) {
		/* an empty dir could be removed even if it is unreadble */
		res = dry_run ? 0 : rmdir(path->buf);
		もし (res) {
			整数 saved_errno = errno;
			quote_path(path->buf, prefix, &quoted, 0);
			errno = saved_errno;
			warning_errno(_(msg_warn_remove_failed), quoted.buf);
			*dir_gone = 0;
		}
		ret = res;
		goto out;
	}

	strbuf_complete(path, '/');

	len = path->len;
	ながら ((e = readdir_skip_dot_and_dotdot(dir)) != NULL) {
		struct stat st;

		strbuf_setlen(path, len);
		strbuf_addstr(path, e->d_name);
		もし (lstat(path->buf, &st))
			warning_errno(_(msg_warn_lstat_failed), path->buf);
		否则 もし (S_ISDIR(st.st_mode)) {
			もし (remove_dirs(path, prefix, force_flag, dry_run, quiet, &gone))
				ret = 1;
			もし (gone) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} 否则
				*dir_gone = 0;
			continue;
		} 否则 {
			res = dry_run ? 0 : unlink(path->buf);
			もし (!res) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} 否则 {
				整数 saved_errno = errno;
				quote_path(path->buf, prefix, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(msg_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
			continue;
		}

		/* path too long, stat fails, or non-directory still exists */
		*dir_gone = 0;
		ret = 1;
		break;
	}
	closedir(dir);

	strbuf_setlen(path, original_len);

	もし (*dir_gone) {
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
		 もし (startup_info->original_cwd)
			 strbuf_realpath(&real_ocwd,
					 startup_info->original_cwd, 1);

		もし (!strbuf_cmp(&realpath, &real_ocwd)) {
			printf("%s", dry_run ? _(msg_would_skip_cwd) : _(msg_skip_cwd));
			*dir_gone = 0;
		} 否则 {
			res = dry_run ? 0 : rmdir(path->buf);
			もし (!res)
				*dir_gone = 1;
			否则 {
				整数 saved_errno = errno;
				quote_path(path->buf, prefix, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(msg_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
		}
	}

	もし (!*dir_gone && !quiet) {
		整数 i;
		ために (i = 0; i < dels.nr; i++)
			printf(dry_run ?  _(msg_would_remove) : _(msg_remove), dels.items[i].string);
	}
out:
	strbuf_release(&realpath);
	strbuf_release(&real_ocwd);
	strbuf_release(&quoted);
	string_list_clear(&dels, 0);
	戻る ret;
}

static void pretty_print_dels(void)
{
	struct string_list list = STRING_LIST_INIT_DUP;
	struct string_list_item *item;
	struct strbuf buf = STRBUF_INIT;
	const char *qname;
	struct column_options copts;

	for_each_string_list_item(item, &del_list) {
		qname = quote_path(item->string, NULL, &buf, 0);
		string_list_append(&list, qname);
	}

	/*
	 * always enable column display, we only consult column.*
	 * about layout strategy and stuff
	 */
	colopts = (colopts & ~COL_ENABLE_MASK) | COL_ENABLED;
	memset(&copts, 0, sizeof(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(&list, colopts, &copts);
	strbuf_release(&buf);
	string_list_clear(&list, 0);
}

static void pretty_print_menus(struct string_list *menu_list)
{
	unsigned 整数 local_colopts = 0;
	struct column_options copts;

	local_colopts = COL_ENABLED | COL_ROW;
	memset(&copts, 0, sizeof(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(menu_list, local_colopts, &copts);
}

static void prompt_help_cmd(整数 singleton)
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
static void print_highlight_menu_stuff(struct menu_stuff *stuff, 整数 **chosen)
{
	struct string_list menu_list = STRING_LIST_INIT_DUP;
	struct strbuf menu = STRBUF_INIT;
	struct menu_item *menu_item;
	struct string_list_item *string_list_item;
	整数 i;

	switch (stuff->type) {
	default:
		die("Bad type of menu_stuff when print menu");
	case MENU_STUFF_TYPE_MENU_ITEM:
		menu_item = (struct menu_item *)stuff->stuff;
		ために (i = 0; i < stuff->nr; i++, menu_item++) {
			const char *p;
			整数 highlighted = 0;

			p = menu_item->title;
			もし ((*chosen)[i] < 0)
				(*chosen)[i] = menu_item->selected ? 1 : 0;
			strbuf_addf(&menu, "%s%2d: ", (*chosen)[i] ? "*" : " ", i+1);
			ために (; *p; p++) {
				もし (!highlighted && *p == menu_item->hotkey) {
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_PROMPT));
					strbuf_addch(&menu, *p);
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_RESET));
					highlighted = 1;
				} 否则 {
					strbuf_addch(&menu, *p);
				}
			}
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
		}
		break;
	case MENU_STUFF_TYPE_STRING_LIST:
		i = 0;
		for_each_string_list_item(string_list_item, (struct string_list *)stuff->stuff) {
			もし ((*chosen)[i] < 0)
				(*chosen)[i] = 0;
			strbuf_addf(&menu, "%s%2d: %s",
				    (*chosen)[i] ? "*" : " ", i+1, string_list_item->string);
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
			i++;
		}
		break;
	}

	pretty_print_menus(&menu_list);

	strbuf_release(&menu);
	string_list_clear(&menu_list, 0);
}

static 整数 find_unique(const char *choice, struct menu_stuff *menu_stuff)
{
	struct menu_item *menu_item;
	struct string_list_item *string_list_item;
	整数 i, len, found = 0;

	len = strlen(choice);
	switch (menu_stuff->type) {
	default:
		die("Bad type of menu_stuff when parse choice");
	case MENU_STUFF_TYPE_MENU_ITEM:

		menu_item = (struct menu_item *)menu_stuff->stuff;
		ために (i = 0; i < menu_stuff->nr; i++, menu_item++) {
			もし (len == 1 && *choice == menu_item->hotkey) {
				found = i + 1;
				break;
			}
			もし (!strncasecmp(choice, menu_item->title, len)) {
				もし (found) {
					もし (len == 1) {
						/* continue for hotkey matching */
						found = -1;
					} 否则 {
						found = 0;
						break;
					}
				} 否则 {
					found = i + 1;
				}
			}
		}
		break;
	case MENU_STUFF_TYPE_STRING_LIST:
		string_list_item = ((struct string_list *)menu_stuff->stuff)->items;
		ために (i = 0; i < menu_stuff->nr; i++, string_list_item++) {
			もし (!strncasecmp(choice, string_list_item->string, len)) {
				もし (found) {
					found = 0;
					break;
				}
				found = i + 1;
			}
		}
		break;
	}
	戻る found;
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
static 整数 parse_choice(struct menu_stuff *menu_stuff,
			整数 is_single,
			char *input,
			整数 **chosen)
{
	struct string_list choice = STRING_LIST_INIT_NODUP;
	struct string_list_item *item;
	整数 nr = 0;
	整数 i;

	string_list_split_in_place_f(&choice, input,
				     is_single ? "\n" : ", ", -1,
				     STRING_LIST_SPLIT_TRIM);

	for_each_string_list_item(item, &choice) {
		const char *string;
		整数 choose;
		整数 bottom = 0, top = 0;
		整数 is_range, is_number;

		string = item->string;
		もし (!*string)
			continue;

		/* Input that begins with '-'; unchoose */
		もし (string[0] == '-') {
			choose = 0;
			string++;
		} 否则 {
			choose = 1;
		}

		is_range = 0;
		is_number = 1;
		ために (const char *p = string; *p; p++) {
			もし ('-' == *p) {
				もし (!is_range) {
					is_range = 1;
					is_number = 0;
				} 否则 {
					is_number = 0;
					is_range = 0;
					break;
				}
			} 否则 もし (!isdigit(*p)) {
				is_number = 0;
				is_range = 0;
				break;
			}
		}

		もし (is_number) {
			bottom = atoi(string);
			top = bottom;
		} 否则 もし (is_range) {
			bottom = atoi(string);
			/* a range can be specified like 5-7 or 5- */
			もし (!*(strchr(string, '-') + 1))
				top = menu_stuff->nr;
			否则
				top = atoi(strchr(string, '-') + 1);
		} 否则 もし (!strcmp(string, "*")) {
			bottom = 1;
			top = menu_stuff->nr;
		} 否则 {
			bottom = find_unique(string, menu_stuff);
			top = bottom;
		}

		もし (top <= 0 || bottom <= 0 || top > menu_stuff->nr || bottom > top ||
		    (is_single && bottom != top)) {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf(_("Huh (%s)?\n"), string);
			clean_print_color(CLEAN_COLOR_RESET);
			continue;
		}

		ために (i = bottom; i <= top; i++)
			(*chosen)[i-1] = choose;
	}

	string_list_clear(&choice, 0);

	ために (i = 0; i < menu_stuff->nr; i++)
		nr += (*chosen)[i];
	戻る nr;
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
static 整数 *list_and_choose(struct menu_opts *opts, struct menu_stuff *stuff)
{
	struct strbuf choice = STRBUF_INIT;
	整数 *chosen, *result;
	整数 nr = 0;
	整数 eof = 0;
	整数 i;

	ALLOC_ARRAY(chosen, stuff->nr);
	/* set chosen as uninitialized */
	ために (i = 0; i < stuff->nr; i++)
		chosen[i] = -1;

	ために (;;) {
		もし (opts->header) {
			printf_ln("%s%s%s",
				  clean_get_color(CLEAN_COLOR_HEADER),
				  _(opts->header),
				  clean_get_color(CLEAN_COLOR_RESET));
		}

		/* chosen will be initialized by print_highlight_menu_stuff */
		print_highlight_menu_stuff(stuff, &chosen);

		もし (opts->flags & MENU_OPTS_LIST_ONLY)
			break;

		もし (opts->prompt) {
			printf("%s%s%s%s",
			       clean_get_color(CLEAN_COLOR_PROMPT),
			       _(opts->prompt),
			       opts->flags & MENU_OPTS_SINGLETON ? "> " : ">> ",
			       clean_get_color(CLEAN_COLOR_RESET));
		}

		もし (git_read_line_interactively(&choice) == EOF) {
			eof = 1;
			break;
		}

		/* help for prompt */
		もし (!strcmp(choice.buf, "?")) {
			prompt_help_cmd(opts->flags & MENU_OPTS_SINGLETON);
			continue;
		}

		/* for a multiple-choice menu, press ENTER (empty) will return back */
		もし (!(opts->flags & MENU_OPTS_SINGLETON) && !choice.len)
			break;

		nr = parse_choice(stuff,
				  opts->flags & MENU_OPTS_SINGLETON,
				  choice.buf,
				  &chosen);

		もし (opts->flags & MENU_OPTS_SINGLETON) {
			もし (nr)
				break;
		} 否则 もし (opts->flags & MENU_OPTS_IMMEDIATE) {
			break;
		}
	}

	もし (eof) {
		result = xmalloc(sizeof(整数));
		*result = EOF;
	} 否则 {
		整数 j = 0;

		/*
		 * recalculate nr, if return back from menu directly with
		 * default selections.
		 */
		もし (!nr) {
			ために (i = 0; i < stuff->nr; i++)
				nr += chosen[i];
		}

		CALLOC_ARRAY(result, st_add(nr, 1));
		ために (i = 0; i < stuff->nr && j < nr; i++) {
			もし (chosen[i])
				result[j++] = i;
		}
		result[j] = EOF;
	}

	free(chosen);
	strbuf_release(&choice);
	戻る result;
}

static 整数 clean_cmd(void)
{
	戻る MENU_RETURN_NO_LOOP;
}

static 整数 filter_by_patterns_cmd(void)
{
	struct dir_struct dir = DIR_INIT;
	struct strbuf confirm = STRBUF_INIT;
	struct pattern_list *pl;
	整数 changed = -1, i;

	ために (;;) {
		struct string_list ignore_list = STRING_LIST_INIT_NODUP;
		struct string_list_item *item;

		もし (!del_list.nr)
			break;

		もし (changed)
			pretty_print_dels();

		clean_print_color(CLEAN_COLOR_PROMPT);
		printf(_("Input ignore patterns>> "));
		clean_print_color(CLEAN_COLOR_RESET);
		もし (git_read_line_interactively(&confirm) == EOF)
			putchar('\n');

		/* quit filter_by_pattern mode if press ENTER or Ctrl-D */
		もし (!confirm.len)
			break;

		pl = add_pattern_list(&dir, EXC_CMDL, "manual exclude");

		string_list_split_in_place_f(&ignore_list, confirm.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);

		ために (i = 0; i < ignore_list.nr; i++) {
			item = &ignore_list.items[i];
			もし (!*item->string)
				continue;
			add_pattern(item->string, "", 0, pl, -(i+1));
		}

		changed = 0;
		for_each_string_list_item(item, &del_list) {
			整数 dtype = DT_UNKNOWN;

			もし (is_excluded(&dir, the_repository->index, item->string, &dtype)) {
				*item->string = '\0';
				changed++;
			}
		}

		もし (changed) {
			string_list_remove_empty_items(&del_list, 0);
		} 否则 {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf_ln(_("WARNING: Cannot find items matched by: %s"), confirm.buf);
			clean_print_color(CLEAN_COLOR_RESET);
		}

		string_list_clear(&ignore_list, 0);
		dir_clear(&dir);
	}

	strbuf_release(&confirm);
	戻る 0;
}

static 整数 select_by_numbers_cmd(void)
{
	struct menu_opts menu_opts;
	struct menu_stuff menu_stuff;
	struct string_list_item *items;
	整数 *chosen;
	整数 i, j;

	menu_opts.header = NULL;
	menu_opts.prompt = N_("Select items to delete");
	menu_opts.flags = 0;

	menu_stuff.type = MENU_STUFF_TYPE_STRING_LIST;
	menu_stuff.stuff = &del_list;
	menu_stuff.nr = del_list.nr;

	chosen = list_and_choose(&menu_opts, &menu_stuff);
	items = del_list.items;
	ために (i = 0, j = 0; i < del_list.nr; i++) {
		もし (i < chosen[j]) {
			*(items[i].string) = '\0';
		} 否则 もし (i == chosen[j]) {
			/* delete selected item */
			j++;
			continue;
		} 否则 {
			/* end of chosen (chosen[j] == EOF), won't delete */
			*(items[i].string) = '\0';
		}
	}

	string_list_remove_empty_items(&del_list, 0);

	free(chosen);
	戻る 0;
}

static 整数 ask_each_cmd(void)
{
	struct strbuf confirm = STRBUF_INIT;
	struct strbuf buf = STRBUF_INIT;
	struct string_list_item *item;
	const char *qname;
	整数 changed = 0, eof = 0;

	for_each_string_list_item(item, &del_list) {
		/* Ctrl-D should stop removing files */
		もし (!eof) {
			qname = quote_path(item->string, NULL, &buf, 0);
			/* TRANSLATORS: Make sure to keep [y/N] as is */
			printf(_("Remove %s [y/N]? "), qname);
			もし (git_read_line_interactively(&confirm) == EOF) {
				putchar('\n');
				eof = 1;
			}
		}
		もし (!confirm.len || strncasecmp(confirm.buf, "yes", confirm.len)) {
			*item->string = '\0';
			changed++;
		}
	}

	もし (changed)
		string_list_remove_empty_items(&del_list, 0);

	strbuf_release(&buf);
	strbuf_release(&confirm);
	戻る MENU_RETURN_NO_LOOP;
}

static 整数 quit_cmd(void)
{
	string_list_clear(&del_list, 0);
	printf(_("Bye.\n"));
	戻る MENU_RETURN_NO_LOOP;
}

static 整数 help_cmd(void)
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
	戻る 0;
}

static void interactive_main_loop(void)
{
	ながら (del_list.nr) {
		struct menu_opts menu_opts;
		struct menu_stuff menu_stuff;
		struct menu_item menus[] = {
			{'c', "clean",			0, clean_cmd},
			{'f', "filter by pattern",	0, filter_by_patterns_cmd},
			{'s', "select by numbers",	0, select_by_numbers_cmd},
			{'a', "ask each",		0, ask_each_cmd},
			{'q', "quit",			0, quit_cmd},
			{'h', "help",			0, help_cmd},
		};
		整数 *chosen;

		menu_opts.header = N_("*** Commands ***");
		menu_opts.prompt = N_("What now");
		menu_opts.flags = MENU_OPTS_SINGLETON;

		menu_stuff.type = MENU_STUFF_TYPE_MENU_ITEM;
		menu_stuff.stuff = menus;
		menu_stuff.nr = sizeof(menus) / sizeof(struct menu_item);

		clean_print_color(CLEAN_COLOR_HEADER);
		printf_ln(Q_("Would remove the following item:",
			     "Would remove the following items:",
			     del_list.nr));
		clean_print_color(CLEAN_COLOR_RESET);

		pretty_print_dels();

		chosen = list_and_choose(&menu_opts, &menu_stuff);

		もし (*chosen != EOF) {
			整数 ret;
			ret = menus[*chosen].fn();
			もし (ret != MENU_RETURN_NO_LOOP) {
				FREE_AND_NULL(chosen);
				もし (!del_list.nr) {
					clean_print_color(CLEAN_COLOR_ERROR);
					printf_ln(_("No more files to clean, exiting."));
					clean_print_color(CLEAN_COLOR_RESET);
					break;
				}
				continue;
			}
		} 否则 {
			quit_cmd();
		}

		FREE_AND_NULL(chosen);
		break;
	}
}

static void correct_untracked_entries(struct dir_struct *dir)
{
	整数 src, dst, ign;

	ために (src = dst = ign = 0; src < dir->nr; src++) {
		/* skip paths in ignored[] that cannot be inside entries[src] */
		ながら (ign < dir->ignored_nr &&
		       0 <= cmp_dir_entry(&dir->entries[src], &dir->ignored[ign]))
			ign++;

		もし (ign < dir->ignored_nr &&
		    check_dir_entry_contains(dir->entries[src], dir->ignored[ign])) {
			/* entries[src] contains an ignored path, so we drop it */
			free(dir->entries[src]);
		} 否则 {
			struct dir_entry *ent = dir->entries[src++];

			/* entries[src] does not contain an ignored path, so we keep it */
			dir->entries[dst++] = ent;

			/* then discard paths in entries[] contained inside entries[src] */
			ながら (src < dir->nr &&
			       check_dir_entry_contains(ent, dir->entries[src]))
				free(dir->entries[src++]);

			/* compensate for the outer loop's loop control */
			src--;
		}
	}
	dir->nr = dst;
}

整数 cmd_clean(整数 argc,
	      const char **argv,
	      const char *prefix,
	      struct repository *repo UNUSED)
{
	整数 i, res;
	整数 dry_run = 0, remove_directories = 0, quiet = 0, ignored = 0;
	整数 ignored_only = 0, force = 0, errors = 0, gone = 1;
	整数 rm_flags = REMOVE_DIR_KEEP_NESTED_GIT;
	struct strbuf abs_path = STRBUF_INIT;
	struct dir_struct dir = DIR_INIT;
	struct pathspec pathspec;
	struct strbuf buf = STRBUF_INIT;
	struct string_list exclude_list = STRING_LIST_INIT_NODUP;
	struct pattern_list *pl;
	struct string_list_item *item;
	const char *qname;
	struct option options[] = {
		OPT__QUIET(&quiet, N_("do not print names of files removed")),
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('i', "interactive", &interactive, N_("interactive cleaning")),
		OPT_BOOL('d', NULL, &remove_directories,
				N_("remove whole directories")),
		OPT_CALLBACK_F('e', "exclude", &exclude_list, N_("pattern"),
		  N_("add <pattern> to ignore rules"), PARSE_OPT_NONEG, exclude_cb),
		OPT_BOOL('x', NULL, &ignored, N_("remove ignored files, too")),
		OPT_BOOL('X', NULL, &ignored_only,
				N_("remove only ignored files")),
		OPT_END()
	};

	repo_config(the_repository, git_clean_config, NULL);

	argc = parse_options(argc, argv, prefix, options, builtin_clean_usage,
			     0);

	もし (require_force != 0 && !force && !interactive && !dry_run)
		die(_("clean.requireForce is true and -f not given: refusing to clean"));

	もし (force > 1)
		rm_flags = 0;
	否则
		dir.flags |= DIR_SKIP_NESTED_GIT;

	dir.flags |= DIR_SHOW_OTHER_DIRECTORIES;

	もし (ignored && ignored_only)
		die(_("options '%s' and '%s' cannot be used together"), "-x", "-X");
	もし (!ignored)
		setup_standard_excludes(&dir);
	もし (ignored_only)
		dir.flags |= DIR_SHOW_IGNORED;

	もし (argc) {
		/*
		 * Remaining args implies pathspecs specified, and we should
		 * recurse within those.
		 */
		remove_directories = 1;
	}

	もし (remove_directories && !ignored_only) {
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
		もし (!ignored)
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

	もし (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	ために (i = 0; i < exclude_list.nr; i++)
		add_pattern(exclude_list.items[i].string, "", 0, pl, -(i+1));

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	fill_directory(&dir, the_repository->index, &pathspec);
	correct_untracked_entries(&dir);

	ために (i = 0; i < dir.nr; i++) {
		struct dir_entry *ent = dir.entries[i];
		struct stat st;
		const char *rel;

		もし (!index_name_is_other(the_repository->index, ent->name, ent->len))
			continue;

		もし (lstat(ent->name, &st))
			die_errno("Cannot lstat '%s'", ent->name);

		もし (S_ISDIR(st.st_mode) && !remove_directories)
			continue;

		rel = relative_path(ent->name, prefix, &buf);
		string_list_append(&del_list, rel);
	}

	dir_clear(&dir);

	もし (interactive && del_list.nr > 0)
		interactive_main_loop();

	for_each_string_list_item(item, &del_list) {
		struct stat st;

		strbuf_reset(&abs_path);
		もし (prefix)
			strbuf_addstr(&abs_path, prefix);

		strbuf_addstr(&abs_path, item->string);

		/*
		 * we might have removed this as part of earlier
		 * recursive directory removal, so lstat() here could
		 * fail with ENOENT.
		 */
		もし (lstat(abs_path.buf, &st))
			continue;

		もし (S_ISDIR(st.st_mode)) {
			もし (remove_dirs(&abs_path, prefix, rm_flags, dry_run, quiet, &gone))
				errors++;
			もし (gone && !quiet) {
				qname = quote_path(item->string, NULL, &buf, 0);
				printf(dry_run ? _(msg_would_remove) : _(msg_remove), qname);
			}
		} 否则 {
			res = dry_run ? 0 : unlink(abs_path.buf);
			もし (res) {
				整数 saved_errno = errno;
				qname = quote_path(item->string, NULL, &buf, 0);
				errno = saved_errno;
				warning_errno(_(msg_warn_remove_failed), qname);
				errors++;
			} 否则 もし (!quiet) {
				qname = quote_path(item->string, NULL, &buf, 0);
				printf(dry_run ? _(msg_would_remove) : _(msg_remove), qname);
			}
		}
	}

	strbuf_release(&abs_path);
	strbuf_release(&buf);
	string_list_clear(&del_list, 0);
	string_list_clear(&exclude_list, 0);
	clear_pathspec(&pathspec);
	戻る (errors != 0);
}
