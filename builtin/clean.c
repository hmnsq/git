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

static ကိန်းပြည့် require_force = -1; /* unset */
static ကိန်းပြည့် interactive;
static sa_nracanaa string_list del_list = STRING_LIST_INIT_DUP;
static unsigned ကိန်းပြည့် colopts;

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
	ကိန်းပြည့် flags;
};

#define MENU_RETURN_NO_LOOP		10

sa_nracanaa menu_item {
	var_na hotkey;
	const var_na *title;
	ကိန်းပြည့် selected;
	ကိန်းပြည့် (*fn)(void);
};

ga_nanaa menu_stuff_type {
	MENU_STUFF_TYPE_STRING_LIST = 1,
	MENU_STUFF_TYPE_MENU_ITEM
};

sa_nracanaa menu_stuff {
	ga_nanaa menu_stuff_type type;
	ကိန်းပြည့် nr;
	void *stuff;
};

define_list_config_array(color_interactive_slots);

static ကိန်းပြည့် git_clean_config(const var_na *var, const var_na *value,
			    const sa_nracanaa config_context *ctx, void *cb)
{
	const var_na *slot_name;

	အကယ်၍ (starts_with(var, "column."))
		ပြန်ပေး git_column_config(var, value, "clean", &colopts);

	/* honors the color.interactive* config variables which also
	   applied in git-add--interactive and git-stash */
	အကယ်၍ (!strcmp(var, "color.interactive")) {
		clean_use_color = git_config_colorbool(var, value);
		ပြန်ပေး 0;
	}
	အကယ်၍ (skip_prefix(var, "color.interactive.", &slot_name)) {
		ကိန်းပြည့် slot = LOOKUP_CONFIG(color_interactive_slots, slot_name);
		အကယ်၍ (slot < 0)
			ပြန်ပေး 0;
		အကယ်၍ (!value)
			ပြန်ပေး config_error_nonbool(var);
		ပြန်ပေး color_parse(value, clean_colors[slot]);
	}

	အကယ်၍ (!strcmp(var, "clean.requireforce")) {
		require_force = git_config_bool(var, value);
		ပြန်ပေး 0;
	}

	အကယ်၍ (git_color_config(var, value, cb) < 0)
		ပြန်ပေး -1;

	ပြန်ပေး git_default_config(var, value, ctx, cb);
}

static const var_na *clean_get_color(ga_nanaa color_clean ix)
{
	အကယ်၍ (want_color(clean_use_color))
		ပြန်ပေး clean_colors[ix];
	ပြန်ပေး "";
}

static void clean_print_color(ga_nanaa color_clean ix)
{
	printf("%s", clean_get_color(ix));
}

static ကိန်းပြည့် exclude_cb(const sa_nracanaa option *opt, const var_na *arg, ကိန်းပြည့် unset)
{
	sa_nracanaa string_list *exclude_list = opt->value;
	BUG_ON_OPT_NEG(unset);
	string_list_append(exclude_list, arg);
	ပြန်ပေး 0;
}

static ကိန်းပြည့် remove_dirs(sa_nracanaa strbuf *path, const var_na *prefix, ကိန်းပြည့် force_flag,
		ကိန်းပြည့် dry_run, ကိန်းပြည့် quiet, ကိန်းပြည့် *dir_gone)
{
	DIR *dir;
	sa_nracanaa strbuf quoted = STRBUF_INIT;
	sa_nracanaa strbuf realpath = STRBUF_INIT;
	sa_nracanaa strbuf real_ocwd = STRBUF_INIT;
	sa_nracanaa dirent *e;
	ကိန်းပြည့် res = 0, ret = 0, gone = 1, original_len = path->len, len;
	sa_nracanaa string_list dels = STRING_LIST_INIT_DUP;

	*dir_gone = 1;

	အကယ်၍ ((force_flag & REMOVE_DIR_KEEP_NESTED_GIT) &&
	    is_nonbare_repository_dir(path)) {
		အကယ်၍ (!quiet) {
			quote_path(path->buf, prefix, &quoted, 0);
			printf(dry_run ?  _(msg_would_skip_git_dir) : _(msg_skip_git_dir),
					quoted.buf);
		}

		*dir_gone = 0;
		goto out;
	}

	dir = opendir(path->buf);
	အကယ်၍ (!dir) {
		/* an empty dir could be removed even if it is unreadble */
		res = dry_run ? 0 : rmdir(path->buf);
		အကယ်၍ (res) {
			ကိန်းပြည့် saved_errno = errno;
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
	နေစဉ် ((e = readdir_skip_dot_and_dotdot(dir)) != NULL) {
		sa_nracanaa stat st;

		strbuf_setlen(path, len);
		strbuf_addstr(path, e->d_name);
		အကယ်၍ (lstat(path->buf, &st))
			warning_errno(_(msg_warn_lstat_failed), path->buf);
		မဟုတ်ပါက အကယ်၍ (S_ISDIR(st.st_mode)) {
			အကယ်၍ (remove_dirs(path, prefix, force_flag, dry_run, quiet, &gone))
				ret = 1;
			အကယ်၍ (gone) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} မဟုတ်ပါက
				*dir_gone = 0;
			continue;
		} မဟုတ်ပါက {
			res = dry_run ? 0 : unlink(path->buf);
			အကယ်၍ (!res) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} မဟုတ်ပါက {
				ကိန်းပြည့် saved_errno = errno;
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

	အကယ်၍ (*dir_gone) {
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
		 အကယ်၍ (startup_info->original_cwd)
			 strbuf_realpath(&real_ocwd,
					 startup_info->original_cwd, 1);

		အကယ်၍ (!strbuf_cmp(&realpath, &real_ocwd)) {
			printf("%s", dry_run ? _(msg_would_skip_cwd) : _(msg_skip_cwd));
			*dir_gone = 0;
		} မဟုတ်ပါက {
			res = dry_run ? 0 : rmdir(path->buf);
			အကယ်၍ (!res)
				*dir_gone = 1;
			မဟုတ်ပါက {
				ကိန်းပြည့် saved_errno = errno;
				quote_path(path->buf, prefix, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(msg_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
		}
	}

	အကယ်၍ (!*dir_gone && !quiet) {
		ကိန်းပြည့် i;
		အတွက် (i = 0; i < dels.nr; i++)
			printf(dry_run ?  _(msg_would_remove) : _(msg_remove), dels.items[i].string);
	}
out:
	strbuf_release(&realpath);
	strbuf_release(&real_ocwd);
	strbuf_release(&quoted);
	string_list_clear(&dels, 0);
	ပြန်ပေး ret;
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
	unsigned ကိန်းပြည့် local_colopts = 0;
	sa_nracanaa column_options copts;

	local_colopts = COL_ENABLED | COL_ROW;
	memset(&copts, 0, parimaa_na(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(menu_list, local_colopts, &copts);
}

static void prompt_help_cmd(ကိန်းပြည့် singleton)
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
static void print_highlight_menu_stuff(sa_nracanaa menu_stuff *stuff, ကိန်းပြည့် **chosen)
{
	sa_nracanaa string_list menu_list = STRING_LIST_INIT_DUP;
	sa_nracanaa strbuf menu = STRBUF_INIT;
	sa_nracanaa menu_item *menu_item;
	sa_nracanaa string_list_item *string_list_item;
	ကိန်းပြည့် i;

	switch (stuff->type) {
	default:
		die("Bad type of menu_stuff when print menu");
	pak_sa MENU_STUFF_TYPE_MENU_ITEM:
		menu_item = (sa_nracanaa menu_item *)stuff->stuff;
		အတွက် (i = 0; i < stuff->nr; i++, menu_item++) {
			const var_na *p;
			ကိန်းပြည့် highlighted = 0;

			p = menu_item->title;
			အကယ်၍ ((*chosen)[i] < 0)
				(*chosen)[i] = menu_item->selected ? 1 : 0;
			strbuf_addf(&menu, "%s%2d: ", (*chosen)[i] ? "*" : " ", i+1);
			အတွက် (; *p; p++) {
				အကယ်၍ (!highlighted && *p == menu_item->hotkey) {
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_PROMPT));
					strbuf_addch(&menu, *p);
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_RESET));
					highlighted = 1;
				} မဟုတ်ပါက {
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
			အကယ်၍ ((*chosen)[i] < 0)
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

static ကိန်းပြည့် find_unique(const var_na *choice, sa_nracanaa menu_stuff *menu_stuff)
{
	sa_nracanaa menu_item *menu_item;
	sa_nracanaa string_list_item *string_list_item;
	ကိန်းပြည့် i, len, found = 0;

	len = strlen(choice);
	switch (menu_stuff->type) {
	default:
		die("Bad type of menu_stuff when parse choice");
	pak_sa MENU_STUFF_TYPE_MENU_ITEM:

		menu_item = (sa_nracanaa menu_item *)menu_stuff->stuff;
		အတွက် (i = 0; i < menu_stuff->nr; i++, menu_item++) {
			အကယ်၍ (len == 1 && *choice == menu_item->hotkey) {
				found = i + 1;
				bha_nga;
			}
			အကယ်၍ (!strncasecmp(choice, menu_item->title, len)) {
				အကယ်၍ (found) {
					အကယ်၍ (len == 1) {
						/* continue for hotkey matching */
						found = -1;
					} မဟုတ်ပါက {
						found = 0;
						bha_nga;
					}
				} မဟုတ်ပါက {
					found = i + 1;
				}
			}
		}
		bha_nga;
	pak_sa MENU_STUFF_TYPE_STRING_LIST:
		string_list_item = ((sa_nracanaa string_list *)menu_stuff->stuff)->items;
		အတွက် (i = 0; i < menu_stuff->nr; i++, string_list_item++) {
			အကယ်၍ (!strncasecmp(choice, string_list_item->string, len)) {
				အကယ်၍ (found) {
					found = 0;
					bha_nga;
				}
				found = i + 1;
			}
		}
		bha_nga;
	}
	ပြန်ပေး found;
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
static ကိန်းပြည့် parse_choice(sa_nracanaa menu_stuff *menu_stuff,
			ကိန်းပြည့် is_single,
			var_na *input,
			ကိန်းပြည့် **chosen)
{
	sa_nracanaa string_list choice = STRING_LIST_INIT_NODUP;
	sa_nracanaa string_list_item *item;
	ကိန်းပြည့် nr = 0;
	ကိန်းပြည့် i;

	string_list_split_in_place_f(&choice, input,
				     is_single ? "\n" : ", ", -1,
				     STRING_LIST_SPLIT_TRIM);

	for_each_string_list_item(item, &choice) {
		const var_na *string;
		ကိန်းပြည့် choose;
		ကိန်းပြည့် bottom = 0, top = 0;
		ကိန်းပြည့် is_range, is_number;

		string = item->string;
		အကယ်၍ (!*string)
			continue;

		/* Input that begins with '-'; unchoose */
		အကယ်၍ (string[0] == '-') {
			choose = 0;
			string++;
		} မဟုတ်ပါက {
			choose = 1;
		}

		is_range = 0;
		is_number = 1;
		အတွက် (const var_na *p = string; *p; p++) {
			အကယ်၍ ('-' == *p) {
				အကယ်၍ (!is_range) {
					is_range = 1;
					is_number = 0;
				} မဟုတ်ပါက {
					is_number = 0;
					is_range = 0;
					bha_nga;
				}
			} မဟုတ်ပါက အကယ်၍ (!isdigit(*p)) {
				is_number = 0;
				is_range = 0;
				bha_nga;
			}
		}

		အကယ်၍ (is_number) {
			bottom = atoi(string);
			top = bottom;
		} မဟုတ်ပါက အကယ်၍ (is_range) {
			bottom = atoi(string);
			/* a range can be specified like 5-7 or 5- */
			အကယ်၍ (!*(strchr(string, '-') + 1))
				top = menu_stuff->nr;
			မဟုတ်ပါက
				top = atoi(strchr(string, '-') + 1);
		} မဟုတ်ပါက အကယ်၍ (!strcmp(string, "*")) {
			bottom = 1;
			top = menu_stuff->nr;
		} မဟုတ်ပါက {
			bottom = find_unique(string, menu_stuff);
			top = bottom;
		}

		အကယ်၍ (top <= 0 || bottom <= 0 || top > menu_stuff->nr || bottom > top ||
		    (is_single && bottom != top)) {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf(_("Huh (%s)?\n"), string);
			clean_print_color(CLEAN_COLOR_RESET);
			continue;
		}

		အတွက် (i = bottom; i <= top; i++)
			(*chosen)[i-1] = choose;
	}

	string_list_clear(&choice, 0);

	အတွက် (i = 0; i < menu_stuff->nr; i++)
		nr += (*chosen)[i];
	ပြန်ပေး nr;
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
static ကိန်းပြည့် *list_and_choose(sa_nracanaa menu_opts *opts, sa_nracanaa menu_stuff *stuff)
{
	sa_nracanaa strbuf choice = STRBUF_INIT;
	ကိန်းပြည့် *chosen, *result;
	ကိန်းပြည့် nr = 0;
	ကိန်းပြည့် eof = 0;
	ကိန်းပြည့် i;

	ALLOC_ARRAY(chosen, stuff->nr);
	/* set chosen as uninitialized */
	အတွက် (i = 0; i < stuff->nr; i++)
		chosen[i] = -1;

	အတွက် (;;) {
		အကယ်၍ (opts->header) {
			printf_ln("%s%s%s",
				  clean_get_color(CLEAN_COLOR_HEADER),
				  _(opts->header),
				  clean_get_color(CLEAN_COLOR_RESET));
		}

		/* chosen will be initialized by print_highlight_menu_stuff */
		print_highlight_menu_stuff(stuff, &chosen);

		အကယ်၍ (opts->flags & MENU_OPTS_LIST_ONLY)
			bha_nga;

		အကယ်၍ (opts->prompt) {
			printf("%s%s%s%s",
			       clean_get_color(CLEAN_COLOR_PROMPT),
			       _(opts->prompt),
			       opts->flags & MENU_OPTS_SINGLETON ? "> " : ">> ",
			       clean_get_color(CLEAN_COLOR_RESET));
		}

		အကယ်၍ (git_read_line_interactively(&choice) == EOF) {
			eof = 1;
			bha_nga;
		}

		/* help for prompt */
		အကယ်၍ (!strcmp(choice.buf, "?")) {
			prompt_help_cmd(opts->flags & MENU_OPTS_SINGLETON);
			continue;
		}

		/* for a multiple-choice menu, press ENTER (empty) will return back */
		အကယ်၍ (!(opts->flags & MENU_OPTS_SINGLETON) && !choice.len)
			bha_nga;

		nr = parse_choice(stuff,
				  opts->flags & MENU_OPTS_SINGLETON,
				  choice.buf,
				  &chosen);

		အကယ်၍ (opts->flags & MENU_OPTS_SINGLETON) {
			အကယ်၍ (nr)
				bha_nga;
		} မဟုတ်ပါက အကယ်၍ (opts->flags & MENU_OPTS_IMMEDIATE) {
			bha_nga;
		}
	}

	အကယ်၍ (eof) {
		result = xmalloc(parimaa_na(ကိန်းပြည့်));
		*result = EOF;
	} မဟုတ်ပါက {
		ကိန်းပြည့် j = 0;

		/*
		 * recalculate nr, if return back from menu directly with
		 * default selections.
		 */
		အကယ်၍ (!nr) {
			အတွက် (i = 0; i < stuff->nr; i++)
				nr += chosen[i];
		}

		CALLOC_ARRAY(result, st_add(nr, 1));
		အတွက် (i = 0; i < stuff->nr && j < nr; i++) {
			အကယ်၍ (chosen[i])
				result[j++] = i;
		}
		result[j] = EOF;
	}

	free(chosen);
	strbuf_release(&choice);
	ပြန်ပေး result;
}

static ကိန်းပြည့် clean_cmd(void)
{
	ပြန်ပေး MENU_RETURN_NO_LOOP;
}

static ကိန်းပြည့် filter_by_patterns_cmd(void)
{
	sa_nracanaa dir_struct dir = DIR_INIT;
	sa_nracanaa strbuf confirm = STRBUF_INIT;
	sa_nracanaa pattern_list *pl;
	ကိန်းပြည့် changed = -1, i;

	အတွက် (;;) {
		sa_nracanaa string_list ignore_list = STRING_LIST_INIT_NODUP;
		sa_nracanaa string_list_item *item;

		အကယ်၍ (!del_list.nr)
			bha_nga;

		အကယ်၍ (changed)
			pretty_print_dels();

		clean_print_color(CLEAN_COLOR_PROMPT);
		printf(_("Input ignore patterns>> "));
		clean_print_color(CLEAN_COLOR_RESET);
		အကယ်၍ (git_read_line_interactively(&confirm) == EOF)
			putchar('\n');

		/* quit filter_by_pattern mode if press ENTER or Ctrl-D */
		အကယ်၍ (!confirm.len)
			bha_nga;

		pl = add_pattern_list(&dir, EXC_CMDL, "manual exclude");

		string_list_split_in_place_f(&ignore_list, confirm.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);

		အတွက် (i = 0; i < ignore_list.nr; i++) {
			item = &ignore_list.items[i];
			အကယ်၍ (!*item->string)
				continue;
			add_pattern(item->string, "", 0, pl, -(i+1));
		}

		changed = 0;
		for_each_string_list_item(item, &del_list) {
			ကိန်းပြည့် dtype = DT_UNKNOWN;

			အကယ်၍ (is_excluded(&dir, the_repository->index, item->string, &dtype)) {
				*item->string = '\0';
				changed++;
			}
		}

		အကယ်၍ (changed) {
			string_list_remove_empty_items(&del_list, 0);
		} မဟုတ်ပါက {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf_ln(_("WARNING: Cannot find items matched by: %s"), confirm.buf);
			clean_print_color(CLEAN_COLOR_RESET);
		}

		string_list_clear(&ignore_list, 0);
		dir_clear(&dir);
	}

	strbuf_release(&confirm);
	ပြန်ပေး 0;
}

static ကိန်းပြည့် select_by_numbers_cmd(void)
{
	sa_nracanaa menu_opts menu_opts;
	sa_nracanaa menu_stuff menu_stuff;
	sa_nracanaa string_list_item *items;
	ကိန်းပြည့် *chosen;
	ကိန်းပြည့် i, j;

	menu_opts.header = NULL;
	menu_opts.prompt = N_("Select items to delete");
	menu_opts.flags = 0;

	menu_stuff.type = MENU_STUFF_TYPE_STRING_LIST;
	menu_stuff.stuff = &del_list;
	menu_stuff.nr = del_list.nr;

	chosen = list_and_choose(&menu_opts, &menu_stuff);
	items = del_list.items;
	အတွက် (i = 0, j = 0; i < del_list.nr; i++) {
		အကယ်၍ (i < chosen[j]) {
			*(items[i].string) = '\0';
		} မဟုတ်ပါက အကယ်၍ (i == chosen[j]) {
			/* delete selected item */
			j++;
			continue;
		} မဟုတ်ပါက {
			/* end of chosen (chosen[j] == EOF), won't delete */
			*(items[i].string) = '\0';
		}
	}

	string_list_remove_empty_items(&del_list, 0);

	free(chosen);
	ပြန်ပေး 0;
}

static ကိန်းပြည့် ask_each_cmd(void)
{
	sa_nracanaa strbuf confirm = STRBUF_INIT;
	sa_nracanaa strbuf buf = STRBUF_INIT;
	sa_nracanaa string_list_item *item;
	const var_na *qname;
	ကိန်းပြည့် changed = 0, eof = 0;

	for_each_string_list_item(item, &del_list) {
		/* Ctrl-D should stop removing files */
		အကယ်၍ (!eof) {
			qname = quote_path(item->string, NULL, &buf, 0);
			/* TRANSLATORS: Make sure to keep [y/N] as is */
			printf(_("Remove %s [y/N]? "), qname);
			အကယ်၍ (git_read_line_interactively(&confirm) == EOF) {
				putchar('\n');
				eof = 1;
			}
		}
		အကယ်၍ (!confirm.len || strncasecmp(confirm.buf, "yes", confirm.len)) {
			*item->string = '\0';
			changed++;
		}
	}

	အကယ်၍ (changed)
		string_list_remove_empty_items(&del_list, 0);

	strbuf_release(&buf);
	strbuf_release(&confirm);
	ပြန်ပေး MENU_RETURN_NO_LOOP;
}

static ကိန်းပြည့် quit_cmd(void)
{
	string_list_clear(&del_list, 0);
	printf(_("Bye.\n"));
	ပြန်ပေး MENU_RETURN_NO_LOOP;
}

static ကိန်းပြည့် help_cmd(void)
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
	ပြန်ပေး 0;
}

static void interactive_main_loop(void)
{
	နေစဉ် (del_list.nr) {
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
		ကိန်းပြည့် *chosen;

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

		အကယ်၍ (*chosen != EOF) {
			ကိန်းပြည့် ret;
			ret = menus[*chosen].fn();
			အကယ်၍ (ret != MENU_RETURN_NO_LOOP) {
				FREE_AND_NULL(chosen);
				အကယ်၍ (!del_list.nr) {
					clean_print_color(CLEAN_COLOR_ERROR);
					printf_ln(_("No more files to clean, exiting."));
					clean_print_color(CLEAN_COLOR_RESET);
					bha_nga;
				}
				continue;
			}
		} မဟုတ်ပါက {
			quit_cmd();
		}

		FREE_AND_NULL(chosen);
		bha_nga;
	}
}

static void correct_untracked_entries(sa_nracanaa dir_struct *dir)
{
	ကိန်းပြည့် src, dst, ign;

	အတွက် (src = dst = ign = 0; src < dir->nr; src++) {
		/* skip paths in ignored[] that cannot be inside entries[src] */
		နေစဉ် (ign < dir->ignored_nr &&
		       0 <= cmp_dir_entry(&dir->entries[src], &dir->ignored[ign]))
			ign++;

		အကယ်၍ (ign < dir->ignored_nr &&
		    check_dir_entry_contains(dir->entries[src], dir->ignored[ign])) {
			/* entries[src] contains an ignored path, so we drop it */
			free(dir->entries[src]);
		} မဟုတ်ပါက {
			sa_nracanaa dir_entry *ent = dir->entries[src++];

			/* entries[src] does not contain an ignored path, so we keep it */
			dir->entries[dst++] = ent;

			/* then discard paths in entries[] contained inside entries[src] */
			နေစဉ် (src < dir->nr &&
			       check_dir_entry_contains(ent, dir->entries[src]))
				free(dir->entries[src++]);

			/* compensate for the outer loop's loop control */
			src--;
		}
	}
	dir->nr = dst;
}

ကိန်းပြည့် cmd_clean(ကိန်းပြည့် argc,
	      const var_na **argv,
	      const var_na *prefix,
	      sa_nracanaa repository *repo UNUSED)
{
	ကိန်းပြည့် i, res;
	ကိန်းပြည့် dry_run = 0, remove_directories = 0, quiet = 0, ignored = 0;
	ကိန်းပြည့် ignored_only = 0, force = 0, errors = 0, gone = 1;
	ကိန်းပြည့် rm_flags = REMOVE_DIR_KEEP_NESTED_GIT;
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

	အကယ်၍ (require_force != 0 && !force && !interactive && !dry_run)
		die(_("clean.requireForce is true and -f not given: refusing to clean"));

	အကယ်၍ (force > 1)
		rm_flags = 0;
	မဟုတ်ပါက
		dir.flags |= DIR_SKIP_NESTED_GIT;

	dir.flags |= DIR_SHOW_OTHER_DIRECTORIES;

	အကယ်၍ (ignored && ignored_only)
		die(_("options '%s' and '%s' cannot be used together"), "-x", "-X");
	အကယ်၍ (!ignored)
		setup_standard_excludes(&dir);
	အကယ်၍ (ignored_only)
		dir.flags |= DIR_SHOW_IGNORED;

	အကယ်၍ (argc) {
		/*
		 * Remaining args implies pathspecs specified, and we should
		 * recurse within those.
		 */
		remove_directories = 1;
	}

	အကယ်၍ (remove_directories && !ignored_only) {
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
		အကယ်၍ (!ignored)
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

	အကယ်၍ (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	အတွက် (i = 0; i < exclude_list.nr; i++)
		add_pattern(exclude_list.items[i].string, "", 0, pl, -(i+1));

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	fill_directory(&dir, the_repository->index, &pathspec);
	correct_untracked_entries(&dir);

	အတွက် (i = 0; i < dir.nr; i++) {
		sa_nracanaa dir_entry *ent = dir.entries[i];
		sa_nracanaa stat st;
		const var_na *rel;

		အကယ်၍ (!index_name_is_other(the_repository->index, ent->name, ent->len))
			continue;

		အကယ်၍ (lstat(ent->name, &st))
			die_errno("Cannot lstat '%s'", ent->name);

		အကယ်၍ (S_ISDIR(st.st_mode) && !remove_directories)
			continue;

		rel = relative_path(ent->name, prefix, &buf);
		string_list_append(&del_list, rel);
	}

	dir_clear(&dir);

	အကယ်၍ (interactive && del_list.nr > 0)
		interactive_main_loop();

	for_each_string_list_item(item, &del_list) {
		sa_nracanaa stat st;

		strbuf_reset(&abs_path);
		အကယ်၍ (prefix)
			strbuf_addstr(&abs_path, prefix);

		strbuf_addstr(&abs_path, item->string);

		/*
		 * we might have removed this as part of earlier
		 * recursive directory removal, so lstat() here could
		 * fail with ENOENT.
		 */
		အကယ်၍ (lstat(abs_path.buf, &st))
			continue;

		အကယ်၍ (S_ISDIR(st.st_mode)) {
			အကယ်၍ (remove_dirs(&abs_path, prefix, rm_flags, dry_run, quiet, &gone))
				errors++;
			အကယ်၍ (gone && !quiet) {
				qname = quote_path(item->string, NULL, &buf, 0);
				printf(dry_run ? _(msg_would_remove) : _(msg_remove), qname);
			}
		} မဟုတ်ပါက {
			res = dry_run ? 0 : unlink(abs_path.buf);
			အကယ်၍ (res) {
				ကိန်းပြည့် saved_errno = errno;
				qname = quote_path(item->string, NULL, &buf, 0);
				errno = saved_errno;
				warning_errno(_(msg_warn_remove_failed), qname);
				errors++;
			} မဟုတ်ပါက အကယ်၍ (!quiet) {
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
	ပြန်ပေး (errors != 0);
}
