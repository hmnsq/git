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

static පූර්ණ_සංඛ්‍යා require_force = -1; /* unset */
static පූර්ණ_සංඛ්‍යා interactive;
static sa_nracanaa string_list del_list = STRING_LIST_INIT_DUP;
static unsigned පූර්ණ_සංඛ්‍යා colopts;

static const var_na *const builtin_clean_usage[] = {
	N_("git clean [-d] [-f] [-i] [-n] [-q] [-e <pattern>] [-x | -X] [--] [<pathspec>...]"),
	NULL
};

static const var_na *msg_remove = N_("Removing %s\n");
static const var_na *msg_would_remove = N_("Would remove %s\n");
static const var_na *msg_skip_git_dir = N_("Skipping repository %s\n");
static const var_na *msg_would_skip_git_dir = N_("Would skip repository %s\n");
static const var_na *msg_warn_remove_failed = N_("failed to remove %s");
static const var_na *msg_warn_lstat_failed = N_("could not lstat %s\n");
static const var_na *msg_skip_cwd = N_("Refusing to remove current working directory\n");
static const var_na *msg_would_skip_cwd = N_("Would refuse to remove current working directory\n");

ga_nanaa color_clean {
	CLEAN_COLOR_RESET = 0,
	CLEAN_COLOR_PLAIN = 1,
	CLEAN_COLOR_PROMPT = 2,
	CLEAN_COLOR_HEADER = 3,
	CLEAN_COLOR_HELP = 4,
	CLEAN_COLOR_ERROR = 5
};

static const var_na *color_interactive_slots[] = {
	[CLEAN_COLOR_ERROR]  = "error",
	[CLEAN_COLOR_HEADER] = "header",
	[CLEAN_COLOR_HELP]   = "help",
	[CLEAN_COLOR_PLAIN]  = "plain",
	[CLEAN_COLOR_PROMPT] = "prompt",
	[CLEAN_COLOR_RESET]  = "reset",
};

static ga_nanaa git_colorbool clean_use_color = GIT_COLOR_UNKNOWN;
static var_na clean_colors[][COLOR_MAXLEN] = {
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

sa_nracanaa menu_opts {
	const var_na *header;
	const var_na *prompt;
	පූර්ණ_සංඛ්‍යා flags;
};

#define MENU_RETURN_NO_LOOP		10

sa_nracanaa menu_item {
	var_na hotkey;
	const var_na *title;
	පූර්ණ_සංඛ්‍යා selected;
	පූර්ණ_සංඛ්‍යා (*fn)(void);
};

ga_nanaa menu_stuff_type {
	MENU_STUFF_TYPE_STRING_LIST = 1,
	MENU_STUFF_TYPE_MENU_ITEM
};

sa_nracanaa menu_stuff {
	ga_nanaa menu_stuff_type type;
	පූර්ණ_සංඛ්‍යා nr;
	void *stuff;
};

define_list_config_array(color_interactive_slots);

static පූර්ණ_සංඛ්‍යා git_clean_config(const var_na *var, const var_na *value,
			    const sa_nracanaa config_context *ctx, void *cb)
{
	const var_na *slot_name;

	නම් (starts_with(var, "column."))
		ආපසු git_column_config(var, value, "clean", &colopts);

	/* honors the color.interactive* config variables which also
	   applied in git-add--interactive and git-stash */
	නම් (!strcmp(var, "color.interactive")) {
		clean_use_color = git_config_colorbool(var, value);
		ආපසු 0;
	}
	නම් (skip_prefix(var, "color.interactive.", &slot_name)) {
		පූර්ණ_සංඛ්‍යා slot = LOOKUP_CONFIG(color_interactive_slots, slot_name);
		නම් (slot < 0)
			ආපසු 0;
		නම් (!value)
			ආපසු config_error_nonbool(var);
		ආපසු color_parse(value, clean_colors[slot]);
	}

	නම් (!strcmp(var, "clean.requireforce")) {
		require_force = git_config_bool(var, value);
		ආපසු 0;
	}

	නම් (git_color_config(var, value, cb) < 0)
		ආපසු -1;

	ආපසු git_default_config(var, value, ctx, cb);
}

static const var_na *clean_get_color(ga_nanaa color_clean ix)
{
	නම් (want_color(clean_use_color))
		ආපසු clean_colors[ix];
	ආපසු "";
}

static void clean_print_color(ga_nanaa color_clean ix)
{
	printf("%s", clean_get_color(ix));
}

static පූර්ණ_සංඛ්‍යා exclude_cb(const sa_nracanaa option *opt, const var_na *arg, පූර්ණ_සංඛ්‍යා unset)
{
	sa_nracanaa string_list *exclude_list = opt->value;
	BUG_ON_OPT_NEG(unset);
	string_list_append(exclude_list, arg);
	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා remove_dirs(sa_nracanaa strbuf *path, const var_na *prefix, පූර්ණ_සංඛ්‍යා force_flag,
		පූර්ණ_සංඛ්‍යා dry_run, පූර්ණ_සංඛ්‍යා quiet, පූර්ණ_සංඛ්‍යා *dir_gone)
{
	DIR *dir;
	sa_nracanaa strbuf quoted = STRBUF_INIT;
	sa_nracanaa strbuf realpath = STRBUF_INIT;
	sa_nracanaa strbuf real_ocwd = STRBUF_INIT;
	sa_nracanaa dirent *e;
	පූර්ණ_සංඛ්‍යා res = 0, ret = 0, gone = 1, original_len = path->len, len;
	sa_nracanaa string_list dels = STRING_LIST_INIT_DUP;

	*dir_gone = 1;

	නම් ((force_flag & REMOVE_DIR_KEEP_NESTED_GIT) &&
	    is_nonbare_repository_dir(path)) {
		නම් (!quiet) {
			quote_path(path->buf, prefix, &quoted, 0);
			printf(dry_run ?  _(msg_would_skip_git_dir) : _(msg_skip_git_dir),
					quoted.buf);
		}

		*dir_gone = 0;
		goto out;
	}

	dir = opendir(path->buf);
	නම් (!dir) {
		/* an empty dir could be removed even if it is unreadble */
		res = dry_run ? 0 : rmdir(path->buf);
		නම් (res) {
			පූර්ණ_සංඛ්‍යා saved_errno = errno;
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
	අතරතුර ((e = readdir_skip_dot_and_dotdot(dir)) != NULL) {
		sa_nracanaa stat st;

		strbuf_setlen(path, len);
		strbuf_addstr(path, e->d_name);
		නම් (lstat(path->buf, &st))
			warning_errno(_(msg_warn_lstat_failed), path->buf);
		නැතහොත් නම් (S_ISDIR(st.st_mode)) {
			නම් (remove_dirs(path, prefix, force_flag, dry_run, quiet, &gone))
				ret = 1;
			නම් (gone) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} නැතහොත්
				*dir_gone = 0;
			continue;
		} නැතහොත් {
			res = dry_run ? 0 : unlink(path->buf);
			නම් (!res) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} නැතහොත් {
				පූර්ණ_සංඛ්‍යා saved_errno = errno;
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
		bha_nga;
	}
	closedir(dir);

	strbuf_setlen(path, original_len);

	නම් (*dir_gone) {
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
		 නම් (startup_info->original_cwd)
			 strbuf_realpath(&real_ocwd,
					 startup_info->original_cwd, 1);

		නම් (!strbuf_cmp(&realpath, &real_ocwd)) {
			printf("%s", dry_run ? _(msg_would_skip_cwd) : _(msg_skip_cwd));
			*dir_gone = 0;
		} නැතහොත් {
			res = dry_run ? 0 : rmdir(path->buf);
			නම් (!res)
				*dir_gone = 1;
			නැතහොත් {
				පූර්ණ_සංඛ්‍යා saved_errno = errno;
				quote_path(path->buf, prefix, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(msg_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
		}
	}

	නම් (!*dir_gone && !quiet) {
		පූර්ණ_සංඛ්‍යා i;
		සඳහා (i = 0; i < dels.nr; i++)
			printf(dry_run ?  _(msg_would_remove) : _(msg_remove), dels.items[i].string);
	}
out:
	strbuf_release(&realpath);
	strbuf_release(&real_ocwd);
	strbuf_release(&quoted);
	string_list_clear(&dels, 0);
	ආපසු ret;
}

static void pretty_print_dels(void)
{
	sa_nracanaa string_list list = STRING_LIST_INIT_DUP;
	sa_nracanaa string_list_item *item;
	sa_nracanaa strbuf buf = STRBUF_INIT;
	const var_na *qname;
	sa_nracanaa column_options copts;

	for_each_string_list_item(item, &del_list) {
		qname = quote_path(item->string, NULL, &buf, 0);
		string_list_append(&list, qname);
	}

	/*
	 * always enable column display, we only consult column.*
	 * about layout strategy and stuff
	 */
	colopts = (colopts & ~COL_ENABLE_MASK) | COL_ENABLED;
	memset(&copts, 0, parimaa_na(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(&list, colopts, &copts);
	strbuf_release(&buf);
	string_list_clear(&list, 0);
}

static void pretty_print_menus(sa_nracanaa string_list *menu_list)
{
	unsigned පූර්ණ_සංඛ්‍යා local_colopts = 0;
	sa_nracanaa column_options copts;

	local_colopts = COL_ENABLED | COL_ROW;
	memset(&copts, 0, parimaa_na(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(menu_list, local_colopts, &copts);
}

static void prompt_help_cmd(පූර්ණ_සංඛ්‍යා singleton)
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
static void print_highlight_menu_stuff(sa_nracanaa menu_stuff *stuff, පූර්ණ_සංඛ්‍යා **chosen)
{
	sa_nracanaa string_list menu_list = STRING_LIST_INIT_DUP;
	sa_nracanaa strbuf menu = STRBUF_INIT;
	sa_nracanaa menu_item *menu_item;
	sa_nracanaa string_list_item *string_list_item;
	පූර්ණ_සංඛ්‍යා i;

	switch (stuff->type) {
	default:
		die("Bad type of menu_stuff when print menu");
	pak_sa MENU_STUFF_TYPE_MENU_ITEM:
		menu_item = (sa_nracanaa menu_item *)stuff->stuff;
		සඳහා (i = 0; i < stuff->nr; i++, menu_item++) {
			const var_na *p;
			පූර්ණ_සංඛ්‍යා highlighted = 0;

			p = menu_item->title;
			නම් ((*chosen)[i] < 0)
				(*chosen)[i] = menu_item->selected ? 1 : 0;
			strbuf_addf(&menu, "%s%2d: ", (*chosen)[i] ? "*" : " ", i+1);
			සඳහා (; *p; p++) {
				නම් (!highlighted && *p == menu_item->hotkey) {
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_PROMPT));
					strbuf_addch(&menu, *p);
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_RESET));
					highlighted = 1;
				} නැතහොත් {
					strbuf_addch(&menu, *p);
				}
			}
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
		}
		bha_nga;
	pak_sa MENU_STUFF_TYPE_STRING_LIST:
		i = 0;
		for_each_string_list_item(string_list_item, (sa_nracanaa string_list *)stuff->stuff) {
			නම් ((*chosen)[i] < 0)
				(*chosen)[i] = 0;
			strbuf_addf(&menu, "%s%2d: %s",
				    (*chosen)[i] ? "*" : " ", i+1, string_list_item->string);
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
			i++;
		}
		bha_nga;
	}

	pretty_print_menus(&menu_list);

	strbuf_release(&menu);
	string_list_clear(&menu_list, 0);
}

static පූර්ණ_සංඛ්‍යා find_unique(const var_na *choice, sa_nracanaa menu_stuff *menu_stuff)
{
	sa_nracanaa menu_item *menu_item;
	sa_nracanaa string_list_item *string_list_item;
	පූර්ණ_සංඛ්‍යා i, len, found = 0;

	len = strlen(choice);
	switch (menu_stuff->type) {
	default:
		die("Bad type of menu_stuff when parse choice");
	pak_sa MENU_STUFF_TYPE_MENU_ITEM:

		menu_item = (sa_nracanaa menu_item *)menu_stuff->stuff;
		සඳහා (i = 0; i < menu_stuff->nr; i++, menu_item++) {
			නම් (len == 1 && *choice == menu_item->hotkey) {
				found = i + 1;
				bha_nga;
			}
			නම් (!strncasecmp(choice, menu_item->title, len)) {
				නම් (found) {
					නම් (len == 1) {
						/* continue for hotkey matching */
						found = -1;
					} නැතහොත් {
						found = 0;
						bha_nga;
					}
				} නැතහොත් {
					found = i + 1;
				}
			}
		}
		bha_nga;
	pak_sa MENU_STUFF_TYPE_STRING_LIST:
		string_list_item = ((sa_nracanaa string_list *)menu_stuff->stuff)->items;
		සඳහා (i = 0; i < menu_stuff->nr; i++, string_list_item++) {
			නම් (!strncasecmp(choice, string_list_item->string, len)) {
				නම් (found) {
					found = 0;
					bha_nga;
				}
				found = i + 1;
			}
		}
		bha_nga;
	}
	ආපසු found;
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
static පූර්ණ_සංඛ්‍යා parse_choice(sa_nracanaa menu_stuff *menu_stuff,
			පූර්ණ_සංඛ්‍යා is_single,
			var_na *input,
			පූර්ණ_සංඛ්‍යා **chosen)
{
	sa_nracanaa string_list choice = STRING_LIST_INIT_NODUP;
	sa_nracanaa string_list_item *item;
	පූර්ණ_සංඛ්‍යා nr = 0;
	පූර්ණ_සංඛ්‍යා i;

	string_list_split_in_place_f(&choice, input,
				     is_single ? "\n" : ", ", -1,
				     STRING_LIST_SPLIT_TRIM);

	for_each_string_list_item(item, &choice) {
		const var_na *string;
		පූර්ණ_සංඛ්‍යා choose;
		පූර්ණ_සංඛ්‍යා bottom = 0, top = 0;
		පූර්ණ_සංඛ්‍යා is_range, is_number;

		string = item->string;
		නම් (!*string)
			continue;

		/* Input that begins with '-'; unchoose */
		නම් (string[0] == '-') {
			choose = 0;
			string++;
		} නැතහොත් {
			choose = 1;
		}

		is_range = 0;
		is_number = 1;
		සඳහා (const var_na *p = string; *p; p++) {
			නම් ('-' == *p) {
				නම් (!is_range) {
					is_range = 1;
					is_number = 0;
				} නැතහොත් {
					is_number = 0;
					is_range = 0;
					bha_nga;
				}
			} නැතහොත් නම් (!isdigit(*p)) {
				is_number = 0;
				is_range = 0;
				bha_nga;
			}
		}

		නම් (is_number) {
			bottom = atoi(string);
			top = bottom;
		} නැතහොත් නම් (is_range) {
			bottom = atoi(string);
			/* a range can be specified like 5-7 or 5- */
			නම් (!*(strchr(string, '-') + 1))
				top = menu_stuff->nr;
			නැතහොත්
				top = atoi(strchr(string, '-') + 1);
		} නැතහොත් නම් (!strcmp(string, "*")) {
			bottom = 1;
			top = menu_stuff->nr;
		} නැතහොත් {
			bottom = find_unique(string, menu_stuff);
			top = bottom;
		}

		නම් (top <= 0 || bottom <= 0 || top > menu_stuff->nr || bottom > top ||
		    (is_single && bottom != top)) {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf(_("Huh (%s)?\n"), string);
			clean_print_color(CLEAN_COLOR_RESET);
			continue;
		}

		සඳහා (i = bottom; i <= top; i++)
			(*chosen)[i-1] = choose;
	}

	string_list_clear(&choice, 0);

	සඳහා (i = 0; i < menu_stuff->nr; i++)
		nr += (*chosen)[i];
	ආපසු nr;
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
static පූර්ණ_සංඛ්‍යා *list_and_choose(sa_nracanaa menu_opts *opts, sa_nracanaa menu_stuff *stuff)
{
	sa_nracanaa strbuf choice = STRBUF_INIT;
	පූර්ණ_සංඛ්‍යා *chosen, *result;
	පූර්ණ_සංඛ්‍යා nr = 0;
	පූර්ණ_සංඛ්‍යා eof = 0;
	පූර්ණ_සංඛ්‍යා i;

	ALLOC_ARRAY(chosen, stuff->nr);
	/* set chosen as uninitialized */
	සඳහා (i = 0; i < stuff->nr; i++)
		chosen[i] = -1;

	සඳහා (;;) {
		නම් (opts->header) {
			printf_ln("%s%s%s",
				  clean_get_color(CLEAN_COLOR_HEADER),
				  _(opts->header),
				  clean_get_color(CLEAN_COLOR_RESET));
		}

		/* chosen will be initialized by print_highlight_menu_stuff */
		print_highlight_menu_stuff(stuff, &chosen);

		නම් (opts->flags & MENU_OPTS_LIST_ONLY)
			bha_nga;

		නම් (opts->prompt) {
			printf("%s%s%s%s",
			       clean_get_color(CLEAN_COLOR_PROMPT),
			       _(opts->prompt),
			       opts->flags & MENU_OPTS_SINGLETON ? "> " : ">> ",
			       clean_get_color(CLEAN_COLOR_RESET));
		}

		නම් (git_read_line_interactively(&choice) == EOF) {
			eof = 1;
			bha_nga;
		}

		/* help for prompt */
		නම් (!strcmp(choice.buf, "?")) {
			prompt_help_cmd(opts->flags & MENU_OPTS_SINGLETON);
			continue;
		}

		/* for a multiple-choice menu, press ENTER (empty) will return back */
		නම් (!(opts->flags & MENU_OPTS_SINGLETON) && !choice.len)
			bha_nga;

		nr = parse_choice(stuff,
				  opts->flags & MENU_OPTS_SINGLETON,
				  choice.buf,
				  &chosen);

		නම් (opts->flags & MENU_OPTS_SINGLETON) {
			නම් (nr)
				bha_nga;
		} නැතහොත් නම් (opts->flags & MENU_OPTS_IMMEDIATE) {
			bha_nga;
		}
	}

	නම් (eof) {
		result = xmalloc(parimaa_na(පූර්ණ_සංඛ්‍යා));
		*result = EOF;
	} නැතහොත් {
		පූර්ණ_සංඛ්‍යා j = 0;

		/*
		 * recalculate nr, if return back from menu directly with
		 * default selections.
		 */
		නම් (!nr) {
			සඳහා (i = 0; i < stuff->nr; i++)
				nr += chosen[i];
		}

		CALLOC_ARRAY(result, st_add(nr, 1));
		සඳහා (i = 0; i < stuff->nr && j < nr; i++) {
			නම් (chosen[i])
				result[j++] = i;
		}
		result[j] = EOF;
	}

	free(chosen);
	strbuf_release(&choice);
	ආපසු result;
}

static පූර්ණ_සංඛ්‍යා clean_cmd(void)
{
	ආපසු MENU_RETURN_NO_LOOP;
}

static පූර්ණ_සංඛ්‍යා filter_by_patterns_cmd(void)
{
	sa_nracanaa dir_struct dir = DIR_INIT;
	sa_nracanaa strbuf confirm = STRBUF_INIT;
	sa_nracanaa pattern_list *pl;
	පූර්ණ_සංඛ්‍යා changed = -1, i;

	සඳහා (;;) {
		sa_nracanaa string_list ignore_list = STRING_LIST_INIT_NODUP;
		sa_nracanaa string_list_item *item;

		නම් (!del_list.nr)
			bha_nga;

		නම් (changed)
			pretty_print_dels();

		clean_print_color(CLEAN_COLOR_PROMPT);
		printf(_("Input ignore patterns>> "));
		clean_print_color(CLEAN_COLOR_RESET);
		නම් (git_read_line_interactively(&confirm) == EOF)
			putchar('\n');

		/* quit filter_by_pattern mode if press ENTER or Ctrl-D */
		නම් (!confirm.len)
			bha_nga;

		pl = add_pattern_list(&dir, EXC_CMDL, "manual exclude");

		string_list_split_in_place_f(&ignore_list, confirm.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);

		සඳහා (i = 0; i < ignore_list.nr; i++) {
			item = &ignore_list.items[i];
			නම් (!*item->string)
				continue;
			add_pattern(item->string, "", 0, pl, -(i+1));
		}

		changed = 0;
		for_each_string_list_item(item, &del_list) {
			පූර්ණ_සංඛ්‍යා dtype = DT_UNKNOWN;

			නම් (is_excluded(&dir, the_repository->index, item->string, &dtype)) {
				*item->string = '\0';
				changed++;
			}
		}

		නම් (changed) {
			string_list_remove_empty_items(&del_list, 0);
		} නැතහොත් {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf_ln(_("WARNING: Cannot find items matched by: %s"), confirm.buf);
			clean_print_color(CLEAN_COLOR_RESET);
		}

		string_list_clear(&ignore_list, 0);
		dir_clear(&dir);
	}

	strbuf_release(&confirm);
	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා select_by_numbers_cmd(void)
{
	sa_nracanaa menu_opts menu_opts;
	sa_nracanaa menu_stuff menu_stuff;
	sa_nracanaa string_list_item *items;
	පූර්ණ_සංඛ්‍යා *chosen;
	පූර්ණ_සංඛ්‍යා i, j;

	menu_opts.header = NULL;
	menu_opts.prompt = N_("Select items to delete");
	menu_opts.flags = 0;

	menu_stuff.type = MENU_STUFF_TYPE_STRING_LIST;
	menu_stuff.stuff = &del_list;
	menu_stuff.nr = del_list.nr;

	chosen = list_and_choose(&menu_opts, &menu_stuff);
	items = del_list.items;
	සඳහා (i = 0, j = 0; i < del_list.nr; i++) {
		නම් (i < chosen[j]) {
			*(items[i].string) = '\0';
		} නැතහොත් නම් (i == chosen[j]) {
			/* delete selected item */
			j++;
			continue;
		} නැතහොත් {
			/* end of chosen (chosen[j] == EOF), won't delete */
			*(items[i].string) = '\0';
		}
	}

	string_list_remove_empty_items(&del_list, 0);

	free(chosen);
	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා ask_each_cmd(void)
{
	sa_nracanaa strbuf confirm = STRBUF_INIT;
	sa_nracanaa strbuf buf = STRBUF_INIT;
	sa_nracanaa string_list_item *item;
	const var_na *qname;
	පූර්ණ_සංඛ්‍යා changed = 0, eof = 0;

	for_each_string_list_item(item, &del_list) {
		/* Ctrl-D should stop removing files */
		නම් (!eof) {
			qname = quote_path(item->string, NULL, &buf, 0);
			/* TRANSLATORS: Make sure to keep [y/N] as is */
			printf(_("Remove %s [y/N]? "), qname);
			නම් (git_read_line_interactively(&confirm) == EOF) {
				putchar('\n');
				eof = 1;
			}
		}
		නම් (!confirm.len || strncasecmp(confirm.buf, "yes", confirm.len)) {
			*item->string = '\0';
			changed++;
		}
	}

	නම් (changed)
		string_list_remove_empty_items(&del_list, 0);

	strbuf_release(&buf);
	strbuf_release(&confirm);
	ආපසු MENU_RETURN_NO_LOOP;
}

static පූර්ණ_සංඛ්‍යා quit_cmd(void)
{
	string_list_clear(&del_list, 0);
	printf(_("Bye.\n"));
	ආපසු MENU_RETURN_NO_LOOP;
}

static පූර්ණ_සංඛ්‍යා help_cmd(void)
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
	ආපසු 0;
}

static void interactive_main_loop(void)
{
	අතරතුර (del_list.nr) {
		sa_nracanaa menu_opts menu_opts;
		sa_nracanaa menu_stuff menu_stuff;
		sa_nracanaa menu_item menus[] = {
			{'c', "clean",			0, clean_cmd},
			{'f', "filter by pattern",	0, filter_by_patterns_cmd},
			{'s', "select by numbers",	0, select_by_numbers_cmd},
			{'a', "ask each",		0, ask_each_cmd},
			{'q', "quit",			0, quit_cmd},
			{'h', "help",			0, help_cmd},
		};
		පූර්ණ_සංඛ්‍යා *chosen;

		menu_opts.header = N_("*** Commands ***");
		menu_opts.prompt = N_("What now");
		menu_opts.flags = MENU_OPTS_SINGLETON;

		menu_stuff.type = MENU_STUFF_TYPE_MENU_ITEM;
		menu_stuff.stuff = menus;
		menu_stuff.nr = parimaa_na(menus) / parimaa_na(sa_nracanaa menu_item);

		clean_print_color(CLEAN_COLOR_HEADER);
		printf_ln(Q_("Would remove the following item:",
			     "Would remove the following items:",
			     del_list.nr));
		clean_print_color(CLEAN_COLOR_RESET);

		pretty_print_dels();

		chosen = list_and_choose(&menu_opts, &menu_stuff);

		නම් (*chosen != EOF) {
			පූර්ණ_සංඛ්‍යා ret;
			ret = menus[*chosen].fn();
			නම් (ret != MENU_RETURN_NO_LOOP) {
				FREE_AND_NULL(chosen);
				නම් (!del_list.nr) {
					clean_print_color(CLEAN_COLOR_ERROR);
					printf_ln(_("No more files to clean, exiting."));
					clean_print_color(CLEAN_COLOR_RESET);
					bha_nga;
				}
				continue;
			}
		} නැතහොත් {
			quit_cmd();
		}

		FREE_AND_NULL(chosen);
		bha_nga;
	}
}

static void correct_untracked_entries(sa_nracanaa dir_struct *dir)
{
	පූර්ණ_සංඛ්‍යා src, dst, ign;

	සඳහා (src = dst = ign = 0; src < dir->nr; src++) {
		/* skip paths in ignored[] that cannot be inside entries[src] */
		අතරතුර (ign < dir->ignored_nr &&
		       0 <= cmp_dir_entry(&dir->entries[src], &dir->ignored[ign]))
			ign++;

		නම් (ign < dir->ignored_nr &&
		    check_dir_entry_contains(dir->entries[src], dir->ignored[ign])) {
			/* entries[src] contains an ignored path, so we drop it */
			free(dir->entries[src]);
		} නැතහොත් {
			sa_nracanaa dir_entry *ent = dir->entries[src++];

			/* entries[src] does not contain an ignored path, so we keep it */
			dir->entries[dst++] = ent;

			/* then discard paths in entries[] contained inside entries[src] */
			අතරතුර (src < dir->nr &&
			       check_dir_entry_contains(ent, dir->entries[src]))
				free(dir->entries[src++]);

			/* compensate for the outer loop's loop control */
			src--;
		}
	}
	dir->nr = dst;
}

පූර්ණ_සංඛ්‍යා cmd_clean(පූර්ණ_සංඛ්‍යා argc,
	      const var_na **argv,
	      const var_na *prefix,
	      sa_nracanaa repository *repo UNUSED)
{
	පූර්ණ_සංඛ්‍යා i, res;
	පූර්ණ_සංඛ්‍යා dry_run = 0, remove_directories = 0, quiet = 0, ignored = 0;
	පූර්ණ_සංඛ්‍යා ignored_only = 0, force = 0, errors = 0, gone = 1;
	පූර්ණ_සංඛ්‍යා rm_flags = REMOVE_DIR_KEEP_NESTED_GIT;
	sa_nracanaa strbuf abs_path = STRBUF_INIT;
	sa_nracanaa dir_struct dir = DIR_INIT;
	sa_nracanaa pathspec pathspec;
	sa_nracanaa strbuf buf = STRBUF_INIT;
	sa_nracanaa string_list exclude_list = STRING_LIST_INIT_NODUP;
	sa_nracanaa pattern_list *pl;
	sa_nracanaa string_list_item *item;
	const var_na *qname;
	sa_nracanaa option options[] = {
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

	නම් (require_force != 0 && !force && !interactive && !dry_run)
		die(_("clean.requireForce is true and -f not given: refusing to clean"));

	නම් (force > 1)
		rm_flags = 0;
	නැතහොත්
		dir.flags |= DIR_SKIP_NESTED_GIT;

	dir.flags |= DIR_SHOW_OTHER_DIRECTORIES;

	නම් (ignored && ignored_only)
		die(_("options '%s' and '%s' cannot be used together"), "-x", "-X");
	නම් (!ignored)
		setup_standard_excludes(&dir);
	නම් (ignored_only)
		dir.flags |= DIR_SHOW_IGNORED;

	නම් (argc) {
		/*
		 * Remaining args implies pathspecs specified, and we should
		 * recurse within those.
		 */
		remove_directories = 1;
	}

	නම් (remove_directories && !ignored_only) {
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
		නම් (!ignored)
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

	නම් (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	සඳහා (i = 0; i < exclude_list.nr; i++)
		add_pattern(exclude_list.items[i].string, "", 0, pl, -(i+1));

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	fill_directory(&dir, the_repository->index, &pathspec);
	correct_untracked_entries(&dir);

	සඳහා (i = 0; i < dir.nr; i++) {
		sa_nracanaa dir_entry *ent = dir.entries[i];
		sa_nracanaa stat st;
		const var_na *rel;

		නම් (!index_name_is_other(the_repository->index, ent->name, ent->len))
			continue;

		නම් (lstat(ent->name, &st))
			die_errno("Cannot lstat '%s'", ent->name);

		නම් (S_ISDIR(st.st_mode) && !remove_directories)
			continue;

		rel = relative_path(ent->name, prefix, &buf);
		string_list_append(&del_list, rel);
	}

	dir_clear(&dir);

	නම් (interactive && del_list.nr > 0)
		interactive_main_loop();

	for_each_string_list_item(item, &del_list) {
		sa_nracanaa stat st;

		strbuf_reset(&abs_path);
		නම් (prefix)
			strbuf_addstr(&abs_path, prefix);

		strbuf_addstr(&abs_path, item->string);

		/*
		 * we might have removed this as part of earlier
		 * recursive directory removal, so lstat() here could
		 * fail with ENOENT.
		 */
		නම් (lstat(abs_path.buf, &st))
			continue;

		නම් (S_ISDIR(st.st_mode)) {
			නම් (remove_dirs(&abs_path, prefix, rm_flags, dry_run, quiet, &gone))
				errors++;
			නම් (gone && !quiet) {
				qname = quote_path(item->string, NULL, &buf, 0);
				printf(dry_run ? _(msg_would_remove) : _(msg_remove), qname);
			}
		} නැතහොත් {
			res = dry_run ? 0 : unlink(abs_path.buf);
			නම් (res) {
				පූර්ණ_සංඛ්‍යා saved_errno = errno;
				qname = quote_path(item->string, NULL, &buf, 0);
				errno = saved_errno;
				warning_errno(_(msg_warn_remove_failed), qname);
				errors++;
			} නැතහොත් නම් (!quiet) {
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
	ආපසු (errors != 0);
}
