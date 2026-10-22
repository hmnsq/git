/*
 * "git clean" builtin command
 *
 * Copyright (C) 2007 Shawn Bohrer
 *
 * Based on git-clean.sh by Pavel Roskin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "abspath.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "dir.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "read-cache-ll.h"
#অন্তর্ভাব "setup.h"
#অন্তর্ভাব "string-list.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "column.h"
#অন্তর্ভাব "color.h"
#অন্তর্ভাব "pathspec.h"
#অন্তর্ভাব "help.h"
#অন্তর্ভাব "prompt.h"

স্থির পূর্ণসংখ্যা require_force = -1; /* unset */
স্থির পূর্ণসংখ্যা interactive;
স্থির কাঠামো string_list del_list = STRING_LIST_INIT_DUP;
স্থির unsigned পূর্ণসংখ্যা colopts;

স্থির ধ্রুবক অক্ষর *ধ্রুবক builtin_clean_usage[] = {
	N_("git clean [-d] [-f] [-i] [-n] [-q] [-e <pattern>] [-x | -X] [--] [<pathspec>...]"),
	ফাঁকা
};

স্থির ধ্রুবক অক্ষর *বার্তা_remove = N_("Removing %s\n");
স্থির ধ্রুবক অক্ষর *বার্তা_would_remove = N_("Would remove %s\n");
স্থির ধ্রুবক অক্ষর *বার্তা_skip_git_dir = N_("Skipping repository %s\n");
স্থির ধ্রুবক অক্ষর *বার্তা_would_skip_git_dir = N_("Would skip repository %s\n");
স্থির ধ্রুবক অক্ষর *বার্তা_warn_remove_failed = N_("failed to remove %s");
স্থির ধ্রুবক অক্ষর *বার্তা_warn_lstat_failed = N_("could not lstat %s\n");
স্থির ধ্রুবক অক্ষর *বার্তা_skip_cwd = N_("Refusing to remove current working directory\n");
স্থির ধ্রুবক অক্ষর *বার্তা_would_skip_cwd = N_("Would refuse to remove current working directory\n");

গণনা color_clean {
	CLEAN_COLOR_RESET = 0,
	CLEAN_COLOR_PLAIN = 1,
	CLEAN_COLOR_PROMPT = 2,
	CLEAN_COLOR_HEADER = 3,
	CLEAN_COLOR_HELP = 4,
	CLEAN_COLOR_ERROR = 5
};

স্থির ধ্রুবক অক্ষর *color_interactive_slots[] = {
	[CLEAN_COLOR_ERROR]  = "error",
	[CLEAN_COLOR_HEADER] = "header",
	[CLEAN_COLOR_HELP]   = "help",
	[CLEAN_COLOR_PLAIN]  = "plain",
	[CLEAN_COLOR_PROMPT] = "prompt",
	[CLEAN_COLOR_RESET]  = "reset",
};

স্থির গণনা git_colorbool clean_use_color = GIT_COLOR_UNKNOWN;
স্থির অক্ষর clean_colors[][COLOR_MAXLEN] = {
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

কাঠামো menu_opts {
	ধ্রুবক অক্ষর *header;
	ধ্রুবক অক্ষর *prompt;
	পূর্ণসংখ্যা flags;
};

#define MENU_RETURN_NO_LOOP		10

কাঠামো menu_item {
	অক্ষর hotkey;
	ধ্রুবক অক্ষর *title;
	পূর্ণসংখ্যা selected;
	পূর্ণসংখ্যা (*fn)(শূন্য);
};

গণনা menu_stuff_type {
	MENU_STUFF_TYPE_STRING_LIST = 1,
	MENU_STUFF_TYPE_MENU_ITEM
};

কাঠামো menu_stuff {
	গণনা menu_stuff_type type;
	পূর্ণসংখ্যা nr;
	শূন্য *stuff;
};

define_list_config_array(color_interactive_slots);

স্থির পূর্ণসংখ্যা git_clean_config(ধ্রুবক অক্ষর *var, ধ্রুবক অক্ষর *মান,
			    ধ্রুবক কাঠামো config_context *ctx, শূন্য *cb)
{
	ধ্রুবক অক্ষর *slot_name;

	যদি (starts_with(var, "column."))
		ফেরত git_column_config(var, মান, "clean", &colopts);

	/* honors the color.interactive* config variables which also
	   applied in git-add--interactive and git-stash */
	যদি (!strcmp(var, "color.interactive")) {
		clean_use_color = git_config_colorbool(var, মান);
		ফেরত 0;
	}
	যদি (skip_prefix(var, "color.interactive.", &slot_name)) {
		পূর্ণসংখ্যা slot = LOOKUP_CONFIG(color_interactive_slots, slot_name);
		যদি (slot < 0)
			ফেরত 0;
		যদি (!মান)
			ফেরত config_error_nonbool(var);
		ফেরত color_parse(মান, clean_colors[slot]);
	}

	যদি (!strcmp(var, "clean.requireforce")) {
		require_force = git_config_bool(var, মান);
		ফেরত 0;
	}

	যদি (git_color_config(var, মান, cb) < 0)
		ফেরত -1;

	ফেরত git_default_config(var, মান, ctx, cb);
}

স্থির ধ্রুবক অক্ষর *clean_get_color(গণনা color_clean ix)
{
	যদি (want_color(clean_use_color))
		ফেরত clean_colors[ix];
	ফেরত "";
}

স্থির শূন্য clean_print_color(গণনা color_clean ix)
{
	printf("%s", clean_get_color(ix));
}

স্থির পূর্ণসংখ্যা exclude_cb(ধ্রুবক কাঠামো option *opt, ধ্রুবক অক্ষর *arg, পূর্ণসংখ্যা unset)
{
	কাঠামো string_list *exclude_list = opt->মান;
	BUG_ON_OPT_NEG(unset);
	string_list_append(exclude_list, arg);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা remove_dirs(কাঠামো strbuf *path, ধ্রুবক অক্ষর *prefix, পূর্ণসংখ্যা force_flag,
		পূর্ণসংখ্যা dry_run, পূর্ণসংখ্যা quiet, পূর্ণসংখ্যা *dir_gone)
{
	DIR *dir;
	কাঠামো strbuf quoted = STRBUF_INIT;
	কাঠামো strbuf realpath = STRBUF_INIT;
	কাঠামো strbuf real_ocwd = STRBUF_INIT;
	কাঠামো dirent *e;
	পূর্ণসংখ্যা res = 0, ret = 0, gone = 1, original_len = path->len, len;
	কাঠামো string_list dels = STRING_LIST_INIT_DUP;

	*dir_gone = 1;

	যদি ((force_flag & REMOVE_DIR_KEEP_NESTED_GIT) &&
	    is_nonbare_repository_dir(path)) {
		যদি (!quiet) {
			quote_path(path->buf, prefix, &quoted, 0);
			printf(dry_run ?  _(বার্তা_would_skip_git_dir) : _(বার্তা_skip_git_dir),
					quoted.buf);
		}

		*dir_gone = 0;
		যাও out;
	}

	dir = opendir(path->buf);
	যদি (!dir) {
		/* an empty dir could be removed even if it is unreadble */
		res = dry_run ? 0 : rmdir(path->buf);
		যদি (res) {
			পূর্ণসংখ্যা saved_errno = errno;
			quote_path(path->buf, prefix, &quoted, 0);
			errno = saved_errno;
			warning_errno(_(বার্তা_warn_remove_failed), quoted.buf);
			*dir_gone = 0;
		}
		ret = res;
		যাও out;
	}

	strbuf_complete(path, '/');

	len = path->len;
	যখন ((e = readdir_skip_dot_and_dotdot(dir)) != ফাঁকা) {
		কাঠামো stat st;

		strbuf_setlen(path, len);
		strbuf_addstr(path, e->d_name);
		যদি (lstat(path->buf, &st))
			warning_errno(_(বার্তা_warn_lstat_failed), path->buf);
		অন্যথা যদি (S_ISDIR(st.st_mode)) {
			যদি (remove_dirs(path, prefix, force_flag, dry_run, quiet, &gone))
				ret = 1;
			যদি (gone) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} অন্যথা
				*dir_gone = 0;
			চালু;
		} অন্যথা {
			res = dry_run ? 0 : unlink(path->buf);
			যদি (!res) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} অন্যথা {
				পূর্ণসংখ্যা saved_errno = errno;
				quote_path(path->buf, prefix, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(বার্তা_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
			চালু;
		}

		/* path too long, stat fails, or non-directory still exists */
		*dir_gone = 0;
		ret = 1;
		বিরতি;
	}
	closedir(dir);

	strbuf_setlen(path, original_len);

	যদি (*dir_gone) {
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
		 যদি (startup_info->original_cwd)
			 strbuf_realpath(&real_ocwd,
					 startup_info->original_cwd, 1);

		যদি (!strbuf_cmp(&realpath, &real_ocwd)) {
			printf("%s", dry_run ? _(বার্তা_would_skip_cwd) : _(বার্তা_skip_cwd));
			*dir_gone = 0;
		} অন্যথা {
			res = dry_run ? 0 : rmdir(path->buf);
			যদি (!res)
				*dir_gone = 1;
			অন্যথা {
				পূর্ণসংখ্যা saved_errno = errno;
				quote_path(path->buf, prefix, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(বার্তা_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
		}
	}

	যদি (!*dir_gone && !quiet) {
		পূর্ণসংখ্যা i;
		জন্য (i = 0; i < dels.nr; i++)
			printf(dry_run ?  _(বার্তা_would_remove) : _(বার্তা_remove), dels.বস্তুসমূহ[i].string);
	}
out:
	strbuf_release(&realpath);
	strbuf_release(&real_ocwd);
	strbuf_release(&quoted);
	string_list_clear(&dels, 0);
	ফেরত ret;
}

স্থির শূন্য pretty_print_dels(শূন্য)
{
	কাঠামো string_list list = STRING_LIST_INIT_DUP;
	কাঠামো string_list_item *item;
	কাঠামো strbuf buf = STRBUF_INIT;
	ধ্রুবক অক্ষর *qname;
	কাঠামো column_options copts;

	for_each_string_list_item(item, &del_list) {
		qname = quote_path(item->string, ফাঁকা, &buf, 0);
		string_list_append(&list, qname);
	}

	/*
	 * always enable column display, we only consult column.*
	 * about layout strategy and stuff
	 */
	colopts = (colopts & ~COL_ENABLE_MASK) | COL_ENABLED;
	memset(&copts, 0, আকার(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(&list, colopts, &copts);
	strbuf_release(&buf);
	string_list_clear(&list, 0);
}

স্থির শূন্য pretty_print_menus(কাঠামো string_list *menu_list)
{
	unsigned পূর্ণসংখ্যা local_colopts = 0;
	কাঠামো column_options copts;

	local_colopts = COL_ENABLED | COL_ROW;
	memset(&copts, 0, আকার(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(menu_list, local_colopts, &copts);
}

স্থির শূন্য prompt_help_cmd(পূর্ণসংখ্যা singleton)
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
স্থির শূন্য print_highlight_menu_stuff(কাঠামো menu_stuff *stuff, পূর্ণসংখ্যা **chosen)
{
	কাঠামো string_list menu_list = STRING_LIST_INIT_DUP;
	কাঠামো strbuf menu = STRBUF_INIT;
	কাঠামো menu_item *menu_item;
	কাঠামো string_list_item *string_list_item;
	পূর্ণসংখ্যা i;

	নির্বাচন (stuff->type) {
	পূর্বনির্ধারিত:
		die("Bad type of menu_stuff when print menu");
	ক্ষেত্র MENU_STUFF_TYPE_MENU_ITEM:
		menu_item = (কাঠামো menu_item *)stuff->stuff;
		জন্য (i = 0; i < stuff->nr; i++, menu_item++) {
			ধ্রুবক অক্ষর *p;
			পূর্ণসংখ্যা highlighted = 0;

			p = menu_item->title;
			যদি ((*chosen)[i] < 0)
				(*chosen)[i] = menu_item->selected ? 1 : 0;
			strbuf_addf(&menu, "%s%2d: ", (*chosen)[i] ? "*" : " ", i+1);
			জন্য (; *p; p++) {
				যদি (!highlighted && *p == menu_item->hotkey) {
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_PROMPT));
					strbuf_addch(&menu, *p);
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_RESET));
					highlighted = 1;
				} অন্যথা {
					strbuf_addch(&menu, *p);
				}
			}
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
		}
		বিরতি;
	ক্ষেত্র MENU_STUFF_TYPE_STRING_LIST:
		i = 0;
		for_each_string_list_item(string_list_item, (কাঠামো string_list *)stuff->stuff) {
			যদি ((*chosen)[i] < 0)
				(*chosen)[i] = 0;
			strbuf_addf(&menu, "%s%2d: %s",
				    (*chosen)[i] ? "*" : " ", i+1, string_list_item->string);
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
			i++;
		}
		বিরতি;
	}

	pretty_print_menus(&menu_list);

	strbuf_release(&menu);
	string_list_clear(&menu_list, 0);
}

স্থির পূর্ণসংখ্যা find_unique(ধ্রুবক অক্ষর *choice, কাঠামো menu_stuff *menu_stuff)
{
	কাঠামো menu_item *menu_item;
	কাঠামো string_list_item *string_list_item;
	পূর্ণসংখ্যা i, len, found = 0;

	len = strlen(choice);
	নির্বাচন (menu_stuff->type) {
	পূর্বনির্ধারিত:
		die("Bad type of menu_stuff when parse choice");
	ক্ষেত্র MENU_STUFF_TYPE_MENU_ITEM:

		menu_item = (কাঠামো menu_item *)menu_stuff->stuff;
		জন্য (i = 0; i < menu_stuff->nr; i++, menu_item++) {
			যদি (len == 1 && *choice == menu_item->hotkey) {
				found = i + 1;
				বিরতি;
			}
			যদি (!strncasecmp(choice, menu_item->title, len)) {
				যদি (found) {
					যদি (len == 1) {
						/* continue for hotkey matching */
						found = -1;
					} অন্যথা {
						found = 0;
						বিরতি;
					}
				} অন্যথা {
					found = i + 1;
				}
			}
		}
		বিরতি;
	ক্ষেত্র MENU_STUFF_TYPE_STRING_LIST:
		string_list_item = ((কাঠামো string_list *)menu_stuff->stuff)->বস্তুসমূহ;
		জন্য (i = 0; i < menu_stuff->nr; i++, string_list_item++) {
			যদি (!strncasecmp(choice, string_list_item->string, len)) {
				যদি (found) {
					found = 0;
					বিরতি;
				}
				found = i + 1;
			}
		}
		বিরতি;
	}
	ফেরত found;
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
স্থির পূর্ণসংখ্যা parse_choice(কাঠামো menu_stuff *menu_stuff,
			পূর্ণসংখ্যা is_single,
			অক্ষর *প্রবেশ,
			পূর্ণসংখ্যা **chosen)
{
	কাঠামো string_list choice = STRING_LIST_INIT_NODUP;
	কাঠামো string_list_item *item;
	পূর্ণসংখ্যা nr = 0;
	পূর্ণসংখ্যা i;

	string_list_split_in_place_f(&choice, প্রবেশ,
				     is_single ? "\n" : ", ", -1,
				     STRING_LIST_SPLIT_TRIM);

	for_each_string_list_item(item, &choice) {
		ধ্রুবক অক্ষর *string;
		পূর্ণসংখ্যা choose;
		পূর্ণসংখ্যা bottom = 0, শীর্ষ_2 = 0;
		পূর্ণসংখ্যা is_range, is_সংখ্যা;

		string = item->string;
		যদি (!*string)
			চালু;

		/* Input that begins with '-'; unchoose */
		যদি (string[0] == '-') {
			choose = 0;
			string++;
		} অন্যথা {
			choose = 1;
		}

		is_range = 0;
		is_সংখ্যা = 1;
		জন্য (ধ্রুবক অক্ষর *p = string; *p; p++) {
			যদি ('-' == *p) {
				যদি (!is_range) {
					is_range = 1;
					is_সংখ্যা = 0;
				} অন্যথা {
					is_সংখ্যা = 0;
					is_range = 0;
					বিরতি;
				}
			} অন্যথা যদি (!isdigit(*p)) {
				is_সংখ্যা = 0;
				is_range = 0;
				বিরতি;
			}
		}

		যদি (is_সংখ্যা) {
			bottom = atoi(string);
			শীর্ষ_2 = bottom;
		} অন্যথা যদি (is_range) {
			bottom = atoi(string);
			/* a range can be specified like 5-7 or 5- */
			যদি (!*(strchr(string, '-') + 1))
				শীর্ষ_2 = menu_stuff->nr;
			অন্যথা
				শীর্ষ_2 = atoi(strchr(string, '-') + 1);
		} অন্যথা যদি (!strcmp(string, "*")) {
			bottom = 1;
			শীর্ষ_2 = menu_stuff->nr;
		} অন্যথা {
			bottom = find_unique(string, menu_stuff);
			শীর্ষ_2 = bottom;
		}

		যদি (শীর্ষ_2 <= 0 || bottom <= 0 || শীর্ষ_2 > menu_stuff->nr || bottom > শীর্ষ_2 ||
		    (is_single && bottom != শীর্ষ_2)) {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf(_("Huh (%s)?\n"), string);
			clean_print_color(CLEAN_COLOR_RESET);
			চালু;
		}

		জন্য (i = bottom; i <= শীর্ষ_2; i++)
			(*chosen)[i-1] = choose;
	}

	string_list_clear(&choice, 0);

	জন্য (i = 0; i < menu_stuff->nr; i++)
		nr += (*chosen)[i];
	ফেরত nr;
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
স্থির পূর্ণসংখ্যা *list_and_choose(কাঠামো menu_opts *opts, কাঠামো menu_stuff *stuff)
{
	কাঠামো strbuf choice = STRBUF_INIT;
	পূর্ণসংখ্যা *chosen, *ফলাফল;
	পূর্ণসংখ্যা nr = 0;
	পূর্ণসংখ্যা eof = 0;
	পূর্ণসংখ্যা i;

	ALLOC_ARRAY(chosen, stuff->nr);
	/* set chosen as uninitialized */
	জন্য (i = 0; i < stuff->nr; i++)
		chosen[i] = -1;

	জন্য (;;) {
		যদি (opts->header) {
			printf_ln("%s%s%s",
				  clean_get_color(CLEAN_COLOR_HEADER),
				  _(opts->header),
				  clean_get_color(CLEAN_COLOR_RESET));
		}

		/* chosen will be initialized by print_highlight_menu_stuff */
		print_highlight_menu_stuff(stuff, &chosen);

		যদি (opts->flags & MENU_OPTS_LIST_ONLY)
			বিরতি;

		যদি (opts->prompt) {
			printf("%s%s%s%s",
			       clean_get_color(CLEAN_COLOR_PROMPT),
			       _(opts->prompt),
			       opts->flags & MENU_OPTS_SINGLETON ? "> " : ">> ",
			       clean_get_color(CLEAN_COLOR_RESET));
		}

		যদি (git_read_সারি_interactively(&choice) == EOF) {
			eof = 1;
			বিরতি;
		}

		/* help for prompt */
		যদি (!strcmp(choice.buf, "?")) {
			prompt_help_cmd(opts->flags & MENU_OPTS_SINGLETON);
			চালু;
		}

		/* for a multiple-choice menu, press ENTER (empty) will return back */
		যদি (!(opts->flags & MENU_OPTS_SINGLETON) && !choice.len)
			বিরতি;

		nr = parse_choice(stuff,
				  opts->flags & MENU_OPTS_SINGLETON,
				  choice.buf,
				  &chosen);

		যদি (opts->flags & MENU_OPTS_SINGLETON) {
			যদি (nr)
				বিরতি;
		} অন্যথা যদি (opts->flags & MENU_OPTS_IMMEDIATE) {
			বিরতি;
		}
	}

	যদি (eof) {
		ফলাফল = xmalloc(আকার(পূর্ণসংখ্যা));
		*ফলাফল = EOF;
	} অন্যথা {
		পূর্ণসংখ্যা j = 0;

		/*
		 * recalculate nr, if return back from menu directly with
		 * default selections.
		 */
		যদি (!nr) {
			জন্য (i = 0; i < stuff->nr; i++)
				nr += chosen[i];
		}

		CALLOC_ARRAY(ফলাফল, st_add(nr, 1));
		জন্য (i = 0; i < stuff->nr && j < nr; i++) {
			যদি (chosen[i])
				ফলাফল[j++] = i;
		}
		ফলাফল[j] = EOF;
	}

	মুক্ত(chosen);
	strbuf_release(&choice);
	ফেরত ফলাফল;
}

স্থির পূর্ণসংখ্যা clean_cmd(শূন্য)
{
	ফেরত MENU_RETURN_NO_LOOP;
}

স্থির পূর্ণসংখ্যা filter_by_patterns_cmd(শূন্য)
{
	কাঠামো dir_struct dir = DIR_INIT;
	কাঠামো strbuf confirm = STRBUF_INIT;
	কাঠামো pattern_list *pl;
	পূর্ণসংখ্যা changed = -1, i;

	জন্য (;;) {
		কাঠামো string_list ignore_list = STRING_LIST_INIT_NODUP;
		কাঠামো string_list_item *item;

		যদি (!del_list.nr)
			বিরতি;

		যদি (changed)
			pretty_print_dels();

		clean_print_color(CLEAN_COLOR_PROMPT);
		printf(_("Input ignore patterns>> "));
		clean_print_color(CLEAN_COLOR_RESET);
		যদি (git_read_সারি_interactively(&confirm) == EOF)
			putchar('\n');

		/* quit filter_by_pattern mode if press ENTER or Ctrl-D */
		যদি (!confirm.len)
			বিরতি;

		pl = add_pattern_list(&dir, EXC_CMDL, "manual exclude");

		string_list_split_in_place_f(&ignore_list, confirm.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);

		জন্য (i = 0; i < ignore_list.nr; i++) {
			item = &ignore_list.বস্তুসমূহ[i];
			যদি (!*item->string)
				চালু;
			add_pattern(item->string, "", 0, pl, -(i+1));
		}

		changed = 0;
		for_each_string_list_item(item, &del_list) {
			পূর্ণসংখ্যা dtype = DT_UNKNOWN;

			যদি (is_excluded(&dir, the_repository->সূচক, item->string, &dtype)) {
				*item->string = '\0';
				changed++;
			}
		}

		যদি (changed) {
			string_list_remove_empty_বস্তুসমূহ(&del_list, 0);
		} অন্যথা {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf_ln(_("WARNING: Cannot find items matched by: %s"), confirm.buf);
			clean_print_color(CLEAN_COLOR_RESET);
		}

		string_list_clear(&ignore_list, 0);
		dir_clear(&dir);
	}

	strbuf_release(&confirm);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা select_by_numbers_cmd(শূন্য)
{
	কাঠামো menu_opts menu_opts;
	কাঠামো menu_stuff menu_stuff;
	কাঠামো string_list_item *বস্তুসমূহ;
	পূর্ণসংখ্যা *chosen;
	পূর্ণসংখ্যা i, j;

	menu_opts.header = ফাঁকা;
	menu_opts.prompt = N_("Select items to delete");
	menu_opts.flags = 0;

	menu_stuff.type = MENU_STUFF_TYPE_STRING_LIST;
	menu_stuff.stuff = &del_list;
	menu_stuff.nr = del_list.nr;

	chosen = list_and_choose(&menu_opts, &menu_stuff);
	বস্তুসমূহ = del_list.বস্তুসমূহ;
	জন্য (i = 0, j = 0; i < del_list.nr; i++) {
		যদি (i < chosen[j]) {
			*(বস্তুসমূহ[i].string) = '\0';
		} অন্যথা যদি (i == chosen[j]) {
			/* delete selected item */
			j++;
			চালু;
		} অন্যথা {
			/* end of chosen (chosen[j] == EOF), won't delete */
			*(বস্তুসমূহ[i].string) = '\0';
		}
	}

	string_list_remove_empty_বস্তুসমূহ(&del_list, 0);

	মুক্ত(chosen);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা ask_each_cmd(শূন্য)
{
	কাঠামো strbuf confirm = STRBUF_INIT;
	কাঠামো strbuf buf = STRBUF_INIT;
	কাঠামো string_list_item *item;
	ধ্রুবক অক্ষর *qname;
	পূর্ণসংখ্যা changed = 0, eof = 0;

	for_each_string_list_item(item, &del_list) {
		/* Ctrl-D should stop removing files */
		যদি (!eof) {
			qname = quote_path(item->string, ফাঁকা, &buf, 0);
			/* TRANSLATORS: Make sure to keep [y/N] as is */
			printf(_("Remove %s [y/N]? "), qname);
			যদি (git_read_সারি_interactively(&confirm) == EOF) {
				putchar('\n');
				eof = 1;
			}
		}
		যদি (!confirm.len || strncasecmp(confirm.buf, "yes", confirm.len)) {
			*item->string = '\0';
			changed++;
		}
	}

	যদি (changed)
		string_list_remove_empty_বস্তুসমূহ(&del_list, 0);

	strbuf_release(&buf);
	strbuf_release(&confirm);
	ফেরত MENU_RETURN_NO_LOOP;
}

স্থির পূর্ণসংখ্যা quit_cmd(শূন্য)
{
	string_list_clear(&del_list, 0);
	printf(_("Bye.\n"));
	ফেরত MENU_RETURN_NO_LOOP;
}

স্থির পূর্ণসংখ্যা help_cmd(শূন্য)
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
	ফেরত 0;
}

স্থির শূন্য interactive_main_loop(শূন্য)
{
	যখন (del_list.nr) {
		কাঠামো menu_opts menu_opts;
		কাঠামো menu_stuff menu_stuff;
		কাঠামো menu_item menus[] = {
			{'c', "clean",			0, clean_cmd},
			{'f', "filter by pattern",	0, filter_by_patterns_cmd},
			{'s', "select by numbers",	0, select_by_numbers_cmd},
			{'a', "ask each",		0, ask_each_cmd},
			{'q', "quit",			0, quit_cmd},
			{'h', "help",			0, help_cmd},
		};
		পূর্ণসংখ্যা *chosen;

		menu_opts.header = N_("*** Commands ***");
		menu_opts.prompt = N_("What now");
		menu_opts.flags = MENU_OPTS_SINGLETON;

		menu_stuff.type = MENU_STUFF_TYPE_MENU_ITEM;
		menu_stuff.stuff = menus;
		menu_stuff.nr = আকার(menus) / আকার(কাঠামো menu_item);

		clean_print_color(CLEAN_COLOR_HEADER);
		printf_ln(Q_("Would remove the following item:",
			     "Would remove the following items:",
			     del_list.nr));
		clean_print_color(CLEAN_COLOR_RESET);

		pretty_print_dels();

		chosen = list_and_choose(&menu_opts, &menu_stuff);

		যদি (*chosen != EOF) {
			পূর্ণসংখ্যা ret;
			ret = menus[*chosen].fn();
			যদি (ret != MENU_RETURN_NO_LOOP) {
				FREE_AND_NULL(chosen);
				যদি (!del_list.nr) {
					clean_print_color(CLEAN_COLOR_ERROR);
					printf_ln(_("No more files to clean, exiting."));
					clean_print_color(CLEAN_COLOR_RESET);
					বিরতি;
				}
				চালু;
			}
		} অন্যথা {
			quit_cmd();
		}

		FREE_AND_NULL(chosen);
		বিরতি;
	}
}

স্থির শূন্য correct_untracked_entries(কাঠামো dir_struct *dir)
{
	পূর্ণসংখ্যা src, dst, ign;

	জন্য (src = dst = ign = 0; src < dir->nr; src++) {
		/* skip paths in ignored[] that cannot be inside entries[src] */
		যখন (ign < dir->ignored_nr &&
		       0 <= cmp_dir_entry(&dir->entries[src], &dir->ignored[ign]))
			ign++;

		যদি (ign < dir->ignored_nr &&
		    check_dir_entry_contains(dir->entries[src], dir->ignored[ign])) {
			/* entries[src] contains an ignored path, so we drop it */
			মুক্ত(dir->entries[src]);
		} অন্যথা {
			কাঠামো dir_entry *ent = dir->entries[src++];

			/* entries[src] does not contain an ignored path, so we keep it */
			dir->entries[dst++] = ent;

			/* then discard paths in entries[] contained inside entries[src] */
			যখন (src < dir->nr &&
			       check_dir_entry_contains(ent, dir->entries[src]))
				মুক্ত(dir->entries[src++]);

			/* compensate for the outer loop's loop control */
			src--;
		}
	}
	dir->nr = dst;
}

পূর্ণসংখ্যা cmd_clean(পূর্ণসংখ্যা argc,
	      ধ্রুবক অক্ষর **argv,
	      ধ্রুবক অক্ষর *prefix,
	      কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা i, res;
	পূর্ণসংখ্যা dry_run = 0, remove_directories = 0, quiet = 0, ignored = 0;
	পূর্ণসংখ্যা ignored_only = 0, force = 0, errors = 0, gone = 1;
	পূর্ণসংখ্যা rm_flags = REMOVE_DIR_KEEP_NESTED_GIT;
	কাঠামো strbuf abs_path = STRBUF_INIT;
	কাঠামো dir_struct dir = DIR_INIT;
	কাঠামো pathspec pathspec;
	কাঠামো strbuf buf = STRBUF_INIT;
	কাঠামো string_list exclude_list = STRING_LIST_INIT_NODUP;
	কাঠামো pattern_list *pl;
	কাঠামো string_list_item *item;
	ধ্রুবক অক্ষর *qname;
	কাঠামো option options[] = {
		OPT__QUIET(&quiet, N_("do not print names of files removed")),
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('i', "interactive", &interactive, N_("interactive cleaning")),
		OPT_BOOL('d', ফাঁকা, &remove_directories,
				N_("remove whole directories")),
		OPT_CALLBACK_F('e', "exclude", &exclude_list, N_("pattern"),
		  N_("add <pattern> to ignore rules"), PARSE_OPT_NONEG, exclude_cb),
		OPT_BOOL('x', ফাঁকা, &ignored, N_("remove ignored files, too")),
		OPT_BOOL('X', ফাঁকা, &ignored_only,
				N_("remove only ignored files")),
		OPT_END()
	};

	repo_config(the_repository, git_clean_config, ফাঁকা);

	argc = parse_options(argc, argv, prefix, options, builtin_clean_usage,
			     0);

	যদি (require_force != 0 && !force && !interactive && !dry_run)
		die(_("clean.requireForce is true and -f not given: refusing to clean"));

	যদি (force > 1)
		rm_flags = 0;
	অন্যথা
		dir.flags |= DIR_SKIP_NESTED_GIT;

	dir.flags |= DIR_SHOW_OTHER_DIRECTORIES;

	যদি (ignored && ignored_only)
		die(_("options '%s' and '%s' cannot be used together"), "-x", "-X");
	যদি (!ignored)
		setup_standard_excludes(&dir);
	যদি (ignored_only)
		dir.flags |= DIR_SHOW_IGNORED;

	যদি (argc) {
		/*
		 * Remaining args implies pathspecs specified, and we should
		 * recurse within those.
		 */
		remove_directories = 1;
	}

	যদি (remove_directories && !ignored_only) {
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
		যদি (!ignored)
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
	the_repository->settings.command_requires_full_সূচক = 0;

	যদি (repo_read_সূচক(the_repository) < 0)
		die(_("index file corrupt"));

	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	জন্য (i = 0; i < exclude_list.nr; i++)
		add_pattern(exclude_list.বস্তুসমূহ[i].string, "", 0, pl, -(i+1));

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	fill_directory(&dir, the_repository->সূচক, &pathspec);
	correct_untracked_entries(&dir);

	জন্য (i = 0; i < dir.nr; i++) {
		কাঠামো dir_entry *ent = dir.entries[i];
		কাঠামো stat st;
		ধ্রুবক অক্ষর *rel;

		যদি (!সূচক_name_is_other(the_repository->সূচক, ent->name, ent->len))
			চালু;

		যদি (lstat(ent->name, &st))
			die_errno("Cannot lstat '%s'", ent->name);

		যদি (S_ISDIR(st.st_mode) && !remove_directories)
			চালু;

		rel = relative_path(ent->name, prefix, &buf);
		string_list_append(&del_list, rel);
	}

	dir_clear(&dir);

	যদি (interactive && del_list.nr > 0)
		interactive_main_loop();

	for_each_string_list_item(item, &del_list) {
		কাঠামো stat st;

		strbuf_reset(&abs_path);
		যদি (prefix)
			strbuf_addstr(&abs_path, prefix);

		strbuf_addstr(&abs_path, item->string);

		/*
		 * we might have removed this as part of earlier
		 * recursive directory removal, so lstat() here could
		 * fail with ENOENT.
		 */
		যদি (lstat(abs_path.buf, &st))
			চালু;

		যদি (S_ISDIR(st.st_mode)) {
			যদি (remove_dirs(&abs_path, prefix, rm_flags, dry_run, quiet, &gone))
				errors++;
			যদি (gone && !quiet) {
				qname = quote_path(item->string, ফাঁকা, &buf, 0);
				printf(dry_run ? _(বার্তা_would_remove) : _(বার্তা_remove), qname);
			}
		} অন্যথা {
			res = dry_run ? 0 : unlink(abs_path.buf);
			যদি (res) {
				পূর্ণসংখ্যা saved_errno = errno;
				qname = quote_path(item->string, ফাঁকা, &buf, 0);
				errno = saved_errno;
				warning_errno(_(বার্তা_warn_remove_failed), qname);
				errors++;
			} অন্যথা যদি (!quiet) {
				qname = quote_path(item->string, ফাঁকা, &buf, 0);
				printf(dry_run ? _(বার্তা_would_remove) : _(বার্তা_remove), qname);
			}
		}
	}

	strbuf_release(&abs_path);
	strbuf_release(&buf);
	string_list_clear(&del_list, 0);
	string_list_clear(&exclude_list, 0);
	clear_pathspec(&pathspec);
	ফেরত (errors != 0);
}
