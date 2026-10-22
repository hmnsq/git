/*
 * "git clean" builtin command
 *
 * Copyright (C) 2007 Shawn Bohrer
 *
 * Based on git-clean.sh by Pavel Roskin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "read-cache-ll.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "column.h"
#அந்தர்பாவ "color.h"
#அந்தர்பாவ "pathspec.h"
#அந்தர்பாவ "help.h"
#அந்தர்பாவ "prompt.h"

நிலையான முழு require_force = -1; /* unset */
நிலையான முழு interactive;
நிலையான கட்டமைப்பு string_list del_list = STRING_LIST_INIT_DUP;
நிலையான unsigned முழு colopts;

நிலையான மாறிலி எழுத்து *மாறிலி builtin_clean_usage[] = {
	N_("git clean [-d] [-f] [-i] [-n] [-q] [-e <pattern>] [-x | -X] [--] [<pathspec>...]"),
	வெற்று
};

நிலையான மாறிலி எழுத்து *செய்தி_remove = N_("Removing %s\n");
நிலையான மாறிலி எழுத்து *செய்தி_would_remove = N_("Would remove %s\n");
நிலையான மாறிலி எழுத்து *செய்தி_skip_git_dir = N_("Skipping repository %s\n");
நிலையான மாறிலி எழுத்து *செய்தி_would_skip_git_dir = N_("Would skip repository %s\n");
நிலையான மாறிலி எழுத்து *செய்தி_warn_remove_failed = N_("failed to remove %s");
நிலையான மாறிலி எழுத்து *செய்தி_warn_lstat_failed = N_("could not lstat %s\n");
நிலையான மாறிலி எழுத்து *செய்தி_skip_cwd = N_("Refusing to remove current working directory\n");
நிலையான மாறிலி எழுத்து *செய்தி_would_skip_cwd = N_("Would refuse to remove current working directory\n");

எண்ணிக்கை color_clean {
	CLEAN_COLOR_RESET = 0,
	CLEAN_COLOR_PLAIN = 1,
	CLEAN_COLOR_PROMPT = 2,
	CLEAN_COLOR_HEADER = 3,
	CLEAN_COLOR_HELP = 4,
	CLEAN_COLOR_ERROR = 5
};

நிலையான மாறிலி எழுத்து *color_interactive_slots[] = {
	[CLEAN_COLOR_ERROR]  = "error",
	[CLEAN_COLOR_HEADER] = "header",
	[CLEAN_COLOR_HELP]   = "help",
	[CLEAN_COLOR_PLAIN]  = "plain",
	[CLEAN_COLOR_PROMPT] = "prompt",
	[CLEAN_COLOR_RESET]  = "reset",
};

நிலையான எண்ணிக்கை git_colorbool clean_use_color = GIT_COLOR_UNKNOWN;
நிலையான எழுத்து clean_colors[][COLOR_MAXLEN] = {
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

கட்டமைப்பு menu_opts {
	மாறிலி எழுத்து *header;
	மாறிலி எழுத்து *prompt;
	முழு flags;
};

#define MENU_RETURN_NO_LOOP		10

கட்டமைப்பு menu_item {
	எழுத்து hotkey;
	மாறிலி எழுத்து *title;
	முழு selected;
	முழு (*fn)(வெறுமை);
};

எண்ணிக்கை menu_stuff_type {
	MENU_STUFF_TYPE_STRING_LIST = 1,
	MENU_STUFF_TYPE_MENU_ITEM
};

கட்டமைப்பு menu_stuff {
	எண்ணிக்கை menu_stuff_type type;
	முழு nr;
	வெறுமை *stuff;
};

define_list_config_array(color_interactive_slots);

நிலையான முழு git_clean_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
			    மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	மாறிலி எழுத்து *slot_name;

	எனில் (starts_with(var, "column."))
		திரும்பு git_column_config(var, மதிப்பு, "clean", &colopts);

	/* honors the color.interactive* config variables which also
	   applied in git-add--interactive and git-stash */
	எனில் (!strcmp(var, "color.interactive")) {
		clean_use_color = git_config_colorbool(var, மதிப்பு);
		திரும்பு 0;
	}
	எனில் (skip_prefix(var, "color.interactive.", &slot_name)) {
		முழு slot = LOOKUP_CONFIG(color_interactive_slots, slot_name);
		எனில் (slot < 0)
			திரும்பு 0;
		எனில் (!மதிப்பு)
			திரும்பு config_error_nonbool(var);
		திரும்பு color_parse(மதிப்பு, clean_colors[slot]);
	}

	எனில் (!strcmp(var, "clean.requireforce")) {
		require_force = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (git_color_config(var, மதிப்பு, cb) < 0)
		திரும்பு -1;

	திரும்பு git_default_config(var, மதிப்பு, ctx, cb);
}

நிலையான மாறிலி எழுத்து *clean_get_color(எண்ணிக்கை color_clean ix)
{
	எனில் (want_color(clean_use_color))
		திரும்பு clean_colors[ix];
	திரும்பு "";
}

நிலையான வெறுமை clean_print_color(எண்ணிக்கை color_clean ix)
{
	printf("%s", clean_get_color(ix));
}

நிலையான முழு exclude_cb(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு string_list *exclude_list = opt->மதிப்பு;
	BUG_ON_OPT_NEG(unset);
	string_list_append(exclude_list, arg);
	திரும்பு 0;
}

நிலையான முழு remove_dirs(கட்டமைப்பு strbuf *path, மாறிலி எழுத்து *prefix, முழு force_flag,
		முழு dry_run, முழு quiet, முழு *dir_gone)
{
	DIR *dir;
	கட்டமைப்பு strbuf quoted = STRBUF_INIT;
	கட்டமைப்பு strbuf realpath = STRBUF_INIT;
	கட்டமைப்பு strbuf real_ocwd = STRBUF_INIT;
	கட்டமைப்பு dirent *e;
	முழு res = 0, ret = 0, gone = 1, original_len = path->len, len;
	கட்டமைப்பு string_list dels = STRING_LIST_INIT_DUP;

	*dir_gone = 1;

	எனில் ((force_flag & REMOVE_DIR_KEEP_NESTED_GIT) &&
	    is_nonbare_repository_dir(path)) {
		எனில் (!quiet) {
			quote_path(path->buf, prefix, &quoted, 0);
			printf(dry_run ?  _(செய்தி_would_skip_git_dir) : _(செய்தி_skip_git_dir),
					quoted.buf);
		}

		*dir_gone = 0;
		செல் out;
	}

	dir = opendir(path->buf);
	எனில் (!dir) {
		/* an empty dir could be removed even if it is unreadble */
		res = dry_run ? 0 : rmdir(path->buf);
		எனில் (res) {
			முழு saved_errno = errno;
			quote_path(path->buf, prefix, &quoted, 0);
			errno = saved_errno;
			warning_errno(_(செய்தி_warn_remove_failed), quoted.buf);
			*dir_gone = 0;
		}
		ret = res;
		செல் out;
	}

	strbuf_complete(path, '/');

	len = path->len;
	போது ((e = readdir_skip_dot_and_dotdot(dir)) != வெற்று) {
		கட்டமைப்பு stat st;

		strbuf_setlen(path, len);
		strbuf_addstr(path, e->d_name);
		எனில் (lstat(path->buf, &st))
			warning_errno(_(செய்தி_warn_lstat_failed), path->buf);
		இல்லையெனில் எனில் (S_ISDIR(st.st_mode)) {
			எனில் (remove_dirs(path, prefix, force_flag, dry_run, quiet, &gone))
				ret = 1;
			எனில் (gone) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} இல்லையெனில்
				*dir_gone = 0;
			தொடர்;
		} இல்லையெனில் {
			res = dry_run ? 0 : unlink(path->buf);
			எனில் (!res) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} இல்லையெனில் {
				முழு saved_errno = errno;
				quote_path(path->buf, prefix, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(செய்தி_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
			தொடர்;
		}

		/* path too long, stat fails, or non-directory still exists */
		*dir_gone = 0;
		ret = 1;
		நிறுத்து;
	}
	closedir(dir);

	strbuf_setlen(path, original_len);

	எனில் (*dir_gone) {
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
		 எனில் (startup_info->original_cwd)
			 strbuf_realpath(&real_ocwd,
					 startup_info->original_cwd, 1);

		எனில் (!strbuf_cmp(&realpath, &real_ocwd)) {
			printf("%s", dry_run ? _(செய்தி_would_skip_cwd) : _(செய்தி_skip_cwd));
			*dir_gone = 0;
		} இல்லையெனில் {
			res = dry_run ? 0 : rmdir(path->buf);
			எனில் (!res)
				*dir_gone = 1;
			இல்லையெனில் {
				முழு saved_errno = errno;
				quote_path(path->buf, prefix, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(செய்தி_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
		}
	}

	எனில் (!*dir_gone && !quiet) {
		முழு i;
		க்கு (i = 0; i < dels.nr; i++)
			printf(dry_run ?  _(செய்தி_would_remove) : _(செய்தி_remove), dels.பொருட்கள்[i].string);
	}
out:
	strbuf_release(&realpath);
	strbuf_release(&real_ocwd);
	strbuf_release(&quoted);
	string_list_clear(&dels, 0);
	திரும்பு ret;
}

நிலையான வெறுமை pretty_print_dels(வெறுமை)
{
	கட்டமைப்பு string_list list = STRING_LIST_INIT_DUP;
	கட்டமைப்பு string_list_item *item;
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	மாறிலி எழுத்து *qname;
	கட்டமைப்பு column_options copts;

	for_each_string_list_item(item, &del_list) {
		qname = quote_path(item->string, வெற்று, &buf, 0);
		string_list_append(&list, qname);
	}

	/*
	 * always enable column display, we only consult column.*
	 * about layout strategy and stuff
	 */
	colopts = (colopts & ~COL_ENABLE_MASK) | COL_ENABLED;
	memset(&copts, 0, அளவு(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(&list, colopts, &copts);
	strbuf_release(&buf);
	string_list_clear(&list, 0);
}

நிலையான வெறுமை pretty_print_menus(கட்டமைப்பு string_list *menu_list)
{
	unsigned முழு local_colopts = 0;
	கட்டமைப்பு column_options copts;

	local_colopts = COL_ENABLED | COL_ROW;
	memset(&copts, 0, அளவு(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(menu_list, local_colopts, &copts);
}

நிலையான வெறுமை prompt_help_cmd(முழு singleton)
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
நிலையான வெறுமை print_highlight_menu_stuff(கட்டமைப்பு menu_stuff *stuff, முழு **chosen)
{
	கட்டமைப்பு string_list menu_list = STRING_LIST_INIT_DUP;
	கட்டமைப்பு strbuf menu = STRBUF_INIT;
	கட்டமைப்பு menu_item *menu_item;
	கட்டமைப்பு string_list_item *string_list_item;
	முழு i;

	தேர்வு (stuff->type) {
	இயல்பு:
		die("Bad type of menu_stuff when print menu");
	நிலை MENU_STUFF_TYPE_MENU_ITEM:
		menu_item = (கட்டமைப்பு menu_item *)stuff->stuff;
		க்கு (i = 0; i < stuff->nr; i++, menu_item++) {
			மாறிலி எழுத்து *p;
			முழு highlighted = 0;

			p = menu_item->title;
			எனில் ((*chosen)[i] < 0)
				(*chosen)[i] = menu_item->selected ? 1 : 0;
			strbuf_addf(&menu, "%s%2d: ", (*chosen)[i] ? "*" : " ", i+1);
			க்கு (; *p; p++) {
				எனில் (!highlighted && *p == menu_item->hotkey) {
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_PROMPT));
					strbuf_addch(&menu, *p);
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_RESET));
					highlighted = 1;
				} இல்லையெனில் {
					strbuf_addch(&menu, *p);
				}
			}
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
		}
		நிறுத்து;
	நிலை MENU_STUFF_TYPE_STRING_LIST:
		i = 0;
		for_each_string_list_item(string_list_item, (கட்டமைப்பு string_list *)stuff->stuff) {
			எனில் ((*chosen)[i] < 0)
				(*chosen)[i] = 0;
			strbuf_addf(&menu, "%s%2d: %s",
				    (*chosen)[i] ? "*" : " ", i+1, string_list_item->string);
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
			i++;
		}
		நிறுத்து;
	}

	pretty_print_menus(&menu_list);

	strbuf_release(&menu);
	string_list_clear(&menu_list, 0);
}

நிலையான முழு find_unique(மாறிலி எழுத்து *choice, கட்டமைப்பு menu_stuff *menu_stuff)
{
	கட்டமைப்பு menu_item *menu_item;
	கட்டமைப்பு string_list_item *string_list_item;
	முழு i, len, found = 0;

	len = strlen(choice);
	தேர்வு (menu_stuff->type) {
	இயல்பு:
		die("Bad type of menu_stuff when parse choice");
	நிலை MENU_STUFF_TYPE_MENU_ITEM:

		menu_item = (கட்டமைப்பு menu_item *)menu_stuff->stuff;
		க்கு (i = 0; i < menu_stuff->nr; i++, menu_item++) {
			எனில் (len == 1 && *choice == menu_item->hotkey) {
				found = i + 1;
				நிறுத்து;
			}
			எனில் (!strncasecmp(choice, menu_item->title, len)) {
				எனில் (found) {
					எனில் (len == 1) {
						/* continue for hotkey matching */
						found = -1;
					} இல்லையெனில் {
						found = 0;
						நிறுத்து;
					}
				} இல்லையெனில் {
					found = i + 1;
				}
			}
		}
		நிறுத்து;
	நிலை MENU_STUFF_TYPE_STRING_LIST:
		string_list_item = ((கட்டமைப்பு string_list *)menu_stuff->stuff)->பொருட்கள்;
		க்கு (i = 0; i < menu_stuff->nr; i++, string_list_item++) {
			எனில் (!strncasecmp(choice, string_list_item->string, len)) {
				எனில் (found) {
					found = 0;
					நிறுத்து;
				}
				found = i + 1;
			}
		}
		நிறுத்து;
	}
	திரும்பு found;
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
நிலையான முழு parse_choice(கட்டமைப்பு menu_stuff *menu_stuff,
			முழு is_single,
			எழுத்து *உள்ளீடு,
			முழு **chosen)
{
	கட்டமைப்பு string_list choice = STRING_LIST_INIT_NODUP;
	கட்டமைப்பு string_list_item *item;
	முழு nr = 0;
	முழு i;

	string_list_split_in_place_f(&choice, உள்ளீடு,
				     is_single ? "\n" : ", ", -1,
				     STRING_LIST_SPLIT_TRIM);

	for_each_string_list_item(item, &choice) {
		மாறிலி எழுத்து *string;
		முழு choose;
		முழு bottom = 0, உச்சி = 0;
		முழு is_range, is_எண்;

		string = item->string;
		எனில் (!*string)
			தொடர்;

		/* Input that begins with '-'; unchoose */
		எனில் (string[0] == '-') {
			choose = 0;
			string++;
		} இல்லையெனில் {
			choose = 1;
		}

		is_range = 0;
		is_எண் = 1;
		க்கு (மாறிலி எழுத்து *p = string; *p; p++) {
			எனில் ('-' == *p) {
				எனில் (!is_range) {
					is_range = 1;
					is_எண் = 0;
				} இல்லையெனில் {
					is_எண் = 0;
					is_range = 0;
					நிறுத்து;
				}
			} இல்லையெனில் எனில் (!isdigit(*p)) {
				is_எண் = 0;
				is_range = 0;
				நிறுத்து;
			}
		}

		எனில் (is_எண்) {
			bottom = atoi(string);
			உச்சி = bottom;
		} இல்லையெனில் எனில் (is_range) {
			bottom = atoi(string);
			/* a range can be specified like 5-7 or 5- */
			எனில் (!*(strchr(string, '-') + 1))
				உச்சி = menu_stuff->nr;
			இல்லையெனில்
				உச்சி = atoi(strchr(string, '-') + 1);
		} இல்லையெனில் எனில் (!strcmp(string, "*")) {
			bottom = 1;
			உச்சி = menu_stuff->nr;
		} இல்லையெனில் {
			bottom = find_unique(string, menu_stuff);
			உச்சி = bottom;
		}

		எனில் (உச்சி <= 0 || bottom <= 0 || உச்சி > menu_stuff->nr || bottom > உச்சி ||
		    (is_single && bottom != உச்சி)) {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf(_("Huh (%s)?\n"), string);
			clean_print_color(CLEAN_COLOR_RESET);
			தொடர்;
		}

		க்கு (i = bottom; i <= உச்சி; i++)
			(*chosen)[i-1] = choose;
	}

	string_list_clear(&choice, 0);

	க்கு (i = 0; i < menu_stuff->nr; i++)
		nr += (*chosen)[i];
	திரும்பு nr;
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
நிலையான முழு *list_and_choose(கட்டமைப்பு menu_opts *opts, கட்டமைப்பு menu_stuff *stuff)
{
	கட்டமைப்பு strbuf choice = STRBUF_INIT;
	முழு *chosen, *result;
	முழு nr = 0;
	முழு eof = 0;
	முழு i;

	ALLOC_ARRAY(chosen, stuff->nr);
	/* set chosen as uninitialized */
	க்கு (i = 0; i < stuff->nr; i++)
		chosen[i] = -1;

	க்கு (;;) {
		எனில் (opts->header) {
			printf_ln("%s%s%s",
				  clean_get_color(CLEAN_COLOR_HEADER),
				  _(opts->header),
				  clean_get_color(CLEAN_COLOR_RESET));
		}

		/* chosen will be initialized by print_highlight_menu_stuff */
		print_highlight_menu_stuff(stuff, &chosen);

		எனில் (opts->flags & MENU_OPTS_LIST_ONLY)
			நிறுத்து;

		எனில் (opts->prompt) {
			printf("%s%s%s%s",
			       clean_get_color(CLEAN_COLOR_PROMPT),
			       _(opts->prompt),
			       opts->flags & MENU_OPTS_SINGLETON ? "> " : ">> ",
			       clean_get_color(CLEAN_COLOR_RESET));
		}

		எனில் (git_read_வரி_interactively(&choice) == EOF) {
			eof = 1;
			நிறுத்து;
		}

		/* help for prompt */
		எனில் (!strcmp(choice.buf, "?")) {
			prompt_help_cmd(opts->flags & MENU_OPTS_SINGLETON);
			தொடர்;
		}

		/* for a multiple-choice menu, press ENTER (empty) will return back */
		எனில் (!(opts->flags & MENU_OPTS_SINGLETON) && !choice.len)
			நிறுத்து;

		nr = parse_choice(stuff,
				  opts->flags & MENU_OPTS_SINGLETON,
				  choice.buf,
				  &chosen);

		எனில் (opts->flags & MENU_OPTS_SINGLETON) {
			எனில் (nr)
				நிறுத்து;
		} இல்லையெனில் எனில் (opts->flags & MENU_OPTS_IMMEDIATE) {
			நிறுத்து;
		}
	}

	எனில் (eof) {
		result = xmalloc(அளவு(முழு));
		*result = EOF;
	} இல்லையெனில் {
		முழு j = 0;

		/*
		 * recalculate nr, if return back from menu directly with
		 * default selections.
		 */
		எனில் (!nr) {
			க்கு (i = 0; i < stuff->nr; i++)
				nr += chosen[i];
		}

		CALLOC_ARRAY(result, st_add(nr, 1));
		க்கு (i = 0; i < stuff->nr && j < nr; i++) {
			எனில் (chosen[i])
				result[j++] = i;
		}
		result[j] = EOF;
	}

	விடுவி(chosen);
	strbuf_release(&choice);
	திரும்பு result;
}

நிலையான முழு clean_cmd(வெறுமை)
{
	திரும்பு MENU_RETURN_NO_LOOP;
}

நிலையான முழு filter_by_patterns_cmd(வெறுமை)
{
	கட்டமைப்பு dir_struct dir = DIR_INIT;
	கட்டமைப்பு strbuf confirm = STRBUF_INIT;
	கட்டமைப்பு pattern_list *pl;
	முழு changed = -1, i;

	க்கு (;;) {
		கட்டமைப்பு string_list ignore_list = STRING_LIST_INIT_NODUP;
		கட்டமைப்பு string_list_item *item;

		எனில் (!del_list.nr)
			நிறுத்து;

		எனில் (changed)
			pretty_print_dels();

		clean_print_color(CLEAN_COLOR_PROMPT);
		printf(_("Input ignore patterns>> "));
		clean_print_color(CLEAN_COLOR_RESET);
		எனில் (git_read_வரி_interactively(&confirm) == EOF)
			putchar('\n');

		/* quit filter_by_pattern mode if press ENTER or Ctrl-D */
		எனில் (!confirm.len)
			நிறுத்து;

		pl = add_pattern_list(&dir, EXC_CMDL, "manual exclude");

		string_list_split_in_place_f(&ignore_list, confirm.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);

		க்கு (i = 0; i < ignore_list.nr; i++) {
			item = &ignore_list.பொருட்கள்[i];
			எனில் (!*item->string)
				தொடர்;
			add_pattern(item->string, "", 0, pl, -(i+1));
		}

		changed = 0;
		for_each_string_list_item(item, &del_list) {
			முழு dtype = DT_UNKNOWN;

			எனில் (is_excluded(&dir, the_repository->index, item->string, &dtype)) {
				*item->string = '\0';
				changed++;
			}
		}

		எனில் (changed) {
			string_list_remove_empty_பொருட்கள்(&del_list, 0);
		} இல்லையெனில் {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf_ln(_("WARNING: Cannot find items matched by: %s"), confirm.buf);
			clean_print_color(CLEAN_COLOR_RESET);
		}

		string_list_clear(&ignore_list, 0);
		dir_clear(&dir);
	}

	strbuf_release(&confirm);
	திரும்பு 0;
}

நிலையான முழு select_by_numbers_cmd(வெறுமை)
{
	கட்டமைப்பு menu_opts menu_opts;
	கட்டமைப்பு menu_stuff menu_stuff;
	கட்டமைப்பு string_list_item *பொருட்கள்;
	முழு *chosen;
	முழு i, j;

	menu_opts.header = வெற்று;
	menu_opts.prompt = N_("Select items to delete");
	menu_opts.flags = 0;

	menu_stuff.type = MENU_STUFF_TYPE_STRING_LIST;
	menu_stuff.stuff = &del_list;
	menu_stuff.nr = del_list.nr;

	chosen = list_and_choose(&menu_opts, &menu_stuff);
	பொருட்கள் = del_list.பொருட்கள்;
	க்கு (i = 0, j = 0; i < del_list.nr; i++) {
		எனில் (i < chosen[j]) {
			*(பொருட்கள்[i].string) = '\0';
		} இல்லையெனில் எனில் (i == chosen[j]) {
			/* delete selected item */
			j++;
			தொடர்;
		} இல்லையெனில் {
			/* end of chosen (chosen[j] == EOF), won't delete */
			*(பொருட்கள்[i].string) = '\0';
		}
	}

	string_list_remove_empty_பொருட்கள்(&del_list, 0);

	விடுவி(chosen);
	திரும்பு 0;
}

நிலையான முழு ask_each_cmd(வெறுமை)
{
	கட்டமைப்பு strbuf confirm = STRBUF_INIT;
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு string_list_item *item;
	மாறிலி எழுத்து *qname;
	முழு changed = 0, eof = 0;

	for_each_string_list_item(item, &del_list) {
		/* Ctrl-D should stop removing files */
		எனில் (!eof) {
			qname = quote_path(item->string, வெற்று, &buf, 0);
			/* TRANSLATORS: Make sure to keep [y/N] as is */
			printf(_("Remove %s [y/N]? "), qname);
			எனில் (git_read_வரி_interactively(&confirm) == EOF) {
				putchar('\n');
				eof = 1;
			}
		}
		எனில் (!confirm.len || strncasecmp(confirm.buf, "yes", confirm.len)) {
			*item->string = '\0';
			changed++;
		}
	}

	எனில் (changed)
		string_list_remove_empty_பொருட்கள்(&del_list, 0);

	strbuf_release(&buf);
	strbuf_release(&confirm);
	திரும்பு MENU_RETURN_NO_LOOP;
}

நிலையான முழு quit_cmd(வெறுமை)
{
	string_list_clear(&del_list, 0);
	printf(_("Bye.\n"));
	திரும்பு MENU_RETURN_NO_LOOP;
}

நிலையான முழு help_cmd(வெறுமை)
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
	திரும்பு 0;
}

நிலையான வெறுமை interactive_main_loop(வெறுமை)
{
	போது (del_list.nr) {
		கட்டமைப்பு menu_opts menu_opts;
		கட்டமைப்பு menu_stuff menu_stuff;
		கட்டமைப்பு menu_item menus[] = {
			{'c', "clean",			0, clean_cmd},
			{'f', "filter by pattern",	0, filter_by_patterns_cmd},
			{'s', "select by numbers",	0, select_by_numbers_cmd},
			{'a', "ask each",		0, ask_each_cmd},
			{'q', "quit",			0, quit_cmd},
			{'h', "help",			0, help_cmd},
		};
		முழு *chosen;

		menu_opts.header = N_("*** Commands ***");
		menu_opts.prompt = N_("What now");
		menu_opts.flags = MENU_OPTS_SINGLETON;

		menu_stuff.type = MENU_STUFF_TYPE_MENU_ITEM;
		menu_stuff.stuff = menus;
		menu_stuff.nr = அளவு(menus) / அளவு(கட்டமைப்பு menu_item);

		clean_print_color(CLEAN_COLOR_HEADER);
		printf_ln(Q_("Would remove the following item:",
			     "Would remove the following items:",
			     del_list.nr));
		clean_print_color(CLEAN_COLOR_RESET);

		pretty_print_dels();

		chosen = list_and_choose(&menu_opts, &menu_stuff);

		எனில் (*chosen != EOF) {
			முழு ret;
			ret = menus[*chosen].fn();
			எனில் (ret != MENU_RETURN_NO_LOOP) {
				FREE_AND_NULL(chosen);
				எனில் (!del_list.nr) {
					clean_print_color(CLEAN_COLOR_ERROR);
					printf_ln(_("No more files to clean, exiting."));
					clean_print_color(CLEAN_COLOR_RESET);
					நிறுத்து;
				}
				தொடர்;
			}
		} இல்லையெனில் {
			quit_cmd();
		}

		FREE_AND_NULL(chosen);
		நிறுத்து;
	}
}

நிலையான வெறுமை correct_untracked_entries(கட்டமைப்பு dir_struct *dir)
{
	முழு src, dst, ign;

	க்கு (src = dst = ign = 0; src < dir->nr; src++) {
		/* skip paths in ignored[] that cannot be inside entries[src] */
		போது (ign < dir->ignored_nr &&
		       0 <= cmp_dir_entry(&dir->entries[src], &dir->ignored[ign]))
			ign++;

		எனில் (ign < dir->ignored_nr &&
		    check_dir_entry_contains(dir->entries[src], dir->ignored[ign])) {
			/* entries[src] contains an ignored path, so we drop it */
			விடுவி(dir->entries[src]);
		} இல்லையெனில் {
			கட்டமைப்பு dir_entry *ent = dir->entries[src++];

			/* entries[src] does not contain an ignored path, so we keep it */
			dir->entries[dst++] = ent;

			/* then discard paths in entries[] contained inside entries[src] */
			போது (src < dir->nr &&
			       check_dir_entry_contains(ent, dir->entries[src]))
				விடுவி(dir->entries[src++]);

			/* compensate for the outer loop's loop control */
			src--;
		}
	}
	dir->nr = dst;
}

முழு cmd_clean(முழு argc,
	      மாறிலி எழுத்து **argv,
	      மாறிலி எழுத்து *prefix,
	      கட்டமைப்பு repository *repo UNUSED)
{
	முழு i, res;
	முழு dry_run = 0, remove_directories = 0, quiet = 0, ignored = 0;
	முழு ignored_only = 0, force = 0, errors = 0, gone = 1;
	முழு rm_flags = REMOVE_DIR_KEEP_NESTED_GIT;
	கட்டமைப்பு strbuf abs_path = STRBUF_INIT;
	கட்டமைப்பு dir_struct dir = DIR_INIT;
	கட்டமைப்பு pathspec pathspec;
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு string_list exclude_list = STRING_LIST_INIT_NODUP;
	கட்டமைப்பு pattern_list *pl;
	கட்டமைப்பு string_list_item *item;
	மாறிலி எழுத்து *qname;
	கட்டமைப்பு option options[] = {
		OPT__QUIET(&quiet, N_("do not print names of files removed")),
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('i', "interactive", &interactive, N_("interactive cleaning")),
		OPT_BOOL('d', வெற்று, &remove_directories,
				N_("remove whole directories")),
		OPT_CALLBACK_F('e', "exclude", &exclude_list, N_("pattern"),
		  N_("add <pattern> to ignore rules"), PARSE_OPT_NONEG, exclude_cb),
		OPT_BOOL('x', வெற்று, &ignored, N_("remove ignored files, too")),
		OPT_BOOL('X', வெற்று, &ignored_only,
				N_("remove only ignored files")),
		OPT_END()
	};

	repo_config(the_repository, git_clean_config, வெற்று);

	argc = parse_options(argc, argv, prefix, options, builtin_clean_usage,
			     0);

	எனில் (require_force != 0 && !force && !interactive && !dry_run)
		die(_("clean.requireForce is true and -f not given: refusing to clean"));

	எனில் (force > 1)
		rm_flags = 0;
	இல்லையெனில்
		dir.flags |= DIR_SKIP_NESTED_GIT;

	dir.flags |= DIR_SHOW_OTHER_DIRECTORIES;

	எனில் (ignored && ignored_only)
		die(_("options '%s' and '%s' cannot be used together"), "-x", "-X");
	எனில் (!ignored)
		setup_standard_excludes(&dir);
	எனில் (ignored_only)
		dir.flags |= DIR_SHOW_IGNORED;

	எனில் (argc) {
		/*
		 * Remaining args implies pathspecs specified, and we should
		 * recurse within those.
		 */
		remove_directories = 1;
	}

	எனில் (remove_directories && !ignored_only) {
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
		எனில் (!ignored)
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

	எனில் (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	க்கு (i = 0; i < exclude_list.nr; i++)
		add_pattern(exclude_list.பொருட்கள்[i].string, "", 0, pl, -(i+1));

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	fill_directory(&dir, the_repository->index, &pathspec);
	correct_untracked_entries(&dir);

	க்கு (i = 0; i < dir.nr; i++) {
		கட்டமைப்பு dir_entry *ent = dir.entries[i];
		கட்டமைப்பு stat st;
		மாறிலி எழுத்து *rel;

		எனில் (!index_name_is_other(the_repository->index, ent->name, ent->len))
			தொடர்;

		எனில் (lstat(ent->name, &st))
			die_errno("Cannot lstat '%s'", ent->name);

		எனில் (S_ISDIR(st.st_mode) && !remove_directories)
			தொடர்;

		rel = relative_path(ent->name, prefix, &buf);
		string_list_append(&del_list, rel);
	}

	dir_clear(&dir);

	எனில் (interactive && del_list.nr > 0)
		interactive_main_loop();

	for_each_string_list_item(item, &del_list) {
		கட்டமைப்பு stat st;

		strbuf_reset(&abs_path);
		எனில் (prefix)
			strbuf_addstr(&abs_path, prefix);

		strbuf_addstr(&abs_path, item->string);

		/*
		 * we might have removed this as part of earlier
		 * recursive directory removal, so lstat() here could
		 * fail with ENOENT.
		 */
		எனில் (lstat(abs_path.buf, &st))
			தொடர்;

		எனில் (S_ISDIR(st.st_mode)) {
			எனில் (remove_dirs(&abs_path, prefix, rm_flags, dry_run, quiet, &gone))
				errors++;
			எனில் (gone && !quiet) {
				qname = quote_path(item->string, வெற்று, &buf, 0);
				printf(dry_run ? _(செய்தி_would_remove) : _(செய்தி_remove), qname);
			}
		} இல்லையெனில் {
			res = dry_run ? 0 : unlink(abs_path.buf);
			எனில் (res) {
				முழு saved_errno = errno;
				qname = quote_path(item->string, வெற்று, &buf, 0);
				errno = saved_errno;
				warning_errno(_(செய்தி_warn_remove_failed), qname);
				errors++;
			} இல்லையெனில் எனில் (!quiet) {
				qname = quote_path(item->string, வெற்று, &buf, 0);
				printf(dry_run ? _(செய்தி_would_remove) : _(செய்தி_remove), qname);
			}
		}
	}

	strbuf_release(&abs_path);
	strbuf_release(&buf);
	string_list_clear(&del_list, 0);
	string_list_clear(&exclude_list, 0);
	clear_pathspec(&pathspec);
	திரும்பு (errors != 0);
}
