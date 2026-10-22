/*
 * "git clean" builtin command
 *
 * Copyright (C) 2007 Shawn Bohrer
 *
 * Based on git-clean.sh by Pavel Roskin
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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

ساکن صحیح require_force = -1; /* unset */
ساکن صحیح interactive;
ساکن ساخت string_list del_list = STRING_LIST_INIT_DUP;
ساکن غیرعلامتی صحیح colopts;

ساکن ثابت حرف *ثابت builtin_clean_usage[] = {
	N_("git clean [-d] [-f] [-i] [-n] [-q] [-e <pattern>] [-x | -X] [--] [<pathspec>...]"),
	تہی
};

ساکن ثابت حرف *پیغام_remove = N_("Removing %s\n");
ساکن ثابت حرف *پیغام_would_remove = N_("Would remove %s\n");
ساکن ثابت حرف *پیغام_skip_git_dir = N_("Skipping repository %s\n");
ساکن ثابت حرف *پیغام_would_skip_git_dir = N_("Would skip repository %s\n");
ساکن ثابت حرف *پیغام_warn_remove_failed = N_("failed to remove %s");
ساکن ثابت حرف *پیغام_warn_lstat_failed = N_("could not lstat %s\n");
ساکن ثابت حرف *پیغام_skip_cwd = N_("Refusing to remove current working directory\n");
ساکن ثابت حرف *پیغام_would_skip_cwd = N_("Would refuse to remove current working directory\n");

شمار color_clean {
	CLEAN_COLOR_RESET = 0,
	CLEAN_COLOR_PLAIN = 1,
	CLEAN_COLOR_PROMPT = 2,
	CLEAN_COLOR_سرخی = 3,
	CLEAN_COLOR_HELP = 4,
	CLEAN_COLOR_ERROR = 5
};

ساکن ثابت حرف *color_interactive_slots[] = {
	[CLEAN_COLOR_ERROR]  = "error",
	[CLEAN_COLOR_سرخی] = "header",
	[CLEAN_COLOR_HELP]   = "help",
	[CLEAN_COLOR_PLAIN]  = "plain",
	[CLEAN_COLOR_PROMPT] = "prompt",
	[CLEAN_COLOR_RESET]  = "reset",
};

ساکن شمار git_colorbool clean_use_color = GIT_COLOR_UNKNOWN;
ساکن حرف clean_colors[][COLOR_MAXLEN] = {
	[CLEAN_COLOR_ERROR] = GIT_COLOR_BOLD_RED,
	[CLEAN_COLOR_سرخی] = GIT_COLOR_BOLD,
	[CLEAN_COLOR_HELP] = GIT_COLOR_BOLD_RED,
	[CLEAN_COLOR_PLAIN] = GIT_COLOR_معمول,
	[CLEAN_COLOR_PROMPT] = GIT_COLOR_BOLD_BLUE,
	[CLEAN_COLOR_RESET] = GIT_COLOR_RESET,
};

#تعریف_کرو MENU_OPTS_SINGLETON		01
#تعریف_کرو MENU_OPTS_فوری		02
#تعریف_کرو MENU_OPTS_LIST_ONLY		04

ساخت menu_opts {
	ثابت حرف *سرخی;
	ثابت حرف *prompt;
	صحیح flags;
};

#تعریف_کرو MENU_RETURN_NO_LOOP		10

ساخت menu_شے {
	حرف hotkey;
	ثابت حرف *title;
	صحیح selected;
	صحیح (*fn)(خالی);
};

شمار menu_stuff_type {
	MENU_STUFF_TYPE_STRING_LIST = 1,
	MENU_STUFF_TYPE_MENU_شے
};

ساخت menu_stuff {
	شمار menu_stuff_type type;
	صحیح nr;
	خالی *stuff;
};

define_list_config_array(color_interactive_slots);

ساکن صحیح git_clean_config(ثابت حرف *var, ثابت حرف *قدر,
			    ثابت ساخت config_context *ctx, خالی *cb)
{
	ثابت حرف *خانہ_name;

	اگر (starts_with(var, "column."))
		واپس git_عمود_config(var, قدر, "clean", &colopts);

	/* honors the color.interactive* config variables which also
	   applied in git-add--interactive and git-stash */
	اگر (!strcmp(var, "color.interactive")) {
		clean_use_color = git_config_colorbool(var, قدر);
		واپس 0;
	}
	اگر (skip_سابقہ(var, "color.interactive.", &خانہ_name)) {
		صحیح خانہ = LOOKUP_CONFIG(color_interactive_slots, خانہ_name);
		اگر (خانہ < 0)
			واپس 0;
		اگر (!قدر)
			واپس config_error_nonbool(var);
		واپس color_parse(قدر, clean_colors[خانہ]);
	}

	اگر (!strcmp(var, "clean.requireforce")) {
		require_force = git_config_bool(var, قدر);
		واپس 0;
	}

	اگر (git_color_config(var, قدر, cb) < 0)
		واپس -1;

	واپس git_default_config(var, قدر, ctx, cb);
}

ساکن ثابت حرف *clean_get_color(شمار color_clean ix)
{
	اگر (want_color(clean_use_color))
		واپس clean_colors[ix];
	واپس "";
}

ساکن خالی clean_print_color(شمار color_clean ix)
{
	لکھو("%s", clean_get_color(ix));
}

ساکن صحیح exclude_cb(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت string_list *exclude_list = opt->قدر;
	BUG_ON_OPT_NEG(unset);
	string_list_append(exclude_list, arg);
	واپس 0;
}

ساکن صحیح remove_dirs(ساخت strbuf *راستہ, ثابت حرف *سابقہ, صحیح force_flag,
		صحیح dry_تسلسل, صحیح quiet, صحیح *dir_gone)
{
	DIR *dir;
	ساخت strbuf quoted = STRBUF_INIT;
	ساخت strbuf realpath = STRBUF_INIT;
	ساخت strbuf real_ocwd = STRBUF_INIT;
	ساخت dirent *e;
	صحیح res = 0, ret = 0, gone = 1, original_len = راستہ->len, len;
	ساخت string_list dels = STRING_LIST_INIT_DUP;

	*dir_gone = 1;

	اگر ((force_flag & REMOVE_DIR_KEEP_NESTED_GIT) &&
	    is_nonbare_repository_dir(راستہ)) {
		اگر (!quiet) {
			quote_راستہ(راستہ->buf, سابقہ, &quoted, 0);
			لکھو(dry_تسلسل ?  _(پیغام_would_skip_git_dir) : _(پیغام_skip_git_dir),
					quoted.buf);
		}

		*dir_gone = 0;
		جاؤ برآمد;
	}

	dir = opendir(راستہ->buf);
	اگر (!dir) {
		/* an empty dir could be removed even if it is unreadble */
		res = dry_تسلسل ? 0 : rmdir(راستہ->buf);
		اگر (res) {
			صحیح saved_errno = errno;
			quote_راستہ(راستہ->buf, سابقہ, &quoted, 0);
			errno = saved_errno;
			warning_errno(_(پیغام_warn_remove_failed), quoted.buf);
			*dir_gone = 0;
		}
		ret = res;
		جاؤ برآمد;
	}

	strbuf_complete(راستہ, '/');

	len = راستہ->len;
	جب ((e = readdir_skip_dot_and_dotdot(dir)) != تہی) {
		ساخت stat st;

		strbuf_setlen(راستہ, len);
		strbuf_addstr(راستہ, e->d_name);
		اگر (lstat(راستہ->buf, &st))
			warning_errno(_(پیغام_warn_lstat_failed), راستہ->buf);
		ورنہ اگر (S_ISDIR(st.st_منوال)) {
			اگر (remove_dirs(راستہ, سابقہ, force_flag, dry_تسلسل, quiet, &gone))
				ret = 1;
			اگر (gone) {
				quote_راستہ(راستہ->buf, سابقہ, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} ورنہ
				*dir_gone = 0;
			جاری;
		} ورنہ {
			res = dry_تسلسل ? 0 : unlink(راستہ->buf);
			اگر (!res) {
				quote_راستہ(راستہ->buf, سابقہ, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} ورنہ {
				صحیح saved_errno = errno;
				quote_راستہ(راستہ->buf, سابقہ, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(پیغام_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
			جاری;
		}

		/* path too long, stat fails, or non-directory still exists */
		*dir_gone = 0;
		ret = 1;
		توڑو;
	}
	closedir(dir);

	strbuf_setlen(راستہ, original_len);

	اگر (*dir_gone) {
		/*
		 * Normalize path components in path->buf, e.g. change '\' to
		 * '/' on Windows.
		 */
		strbuf_realpath(&realpath, راستہ->buf, 1);

		/*
		 * path and realpath are absolute; for comparison, we would
		 * like to transform startup_info->original_cwd to an absolute
		 * path too.
		 */
		 اگر (startup_info->original_cwd)
			 strbuf_realpath(&real_ocwd,
					 startup_info->original_cwd, 1);

		اگر (!strbuf_cmp(&realpath, &real_ocwd)) {
			لکھو("%s", dry_تسلسل ? _(پیغام_would_skip_cwd) : _(پیغام_skip_cwd));
			*dir_gone = 0;
		} ورنہ {
			res = dry_تسلسل ? 0 : rmdir(راستہ->buf);
			اگر (!res)
				*dir_gone = 1;
			ورنہ {
				صحیح saved_errno = errno;
				quote_راستہ(راستہ->buf, سابقہ, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(پیغام_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
		}
	}

	اگر (!*dir_gone && !quiet) {
		صحیح i;
		برائے (i = 0; i < dels.nr; i++)
			لکھو(dry_تسلسل ?  _(پیغام_would_remove) : _(پیغام_remove), dels.اشیاء[i].string);
	}
برآمد:
	strbuf_release(&realpath);
	strbuf_release(&real_ocwd);
	strbuf_release(&quoted);
	string_list_clear(&dels, 0);
	واپس ret;
}

ساکن خالی pretty_print_dels(خالی)
{
	ساخت string_list list = STRING_LIST_INIT_DUP;
	ساخت string_list_شے *شے;
	ساخت strbuf buf = STRBUF_INIT;
	ثابت حرف *qname;
	ساخت عمود_options copts;

	for_each_string_list_شے(شے, &del_list) {
		qname = quote_راستہ(شے->string, تہی, &buf, 0);
		string_list_append(&list, qname);
	}

	/*
	 * always enable column display, we only consult column.*
	 * about layout strategy and stuff
	 */
	colopts = (colopts & ~ستون_ENABLE_نقاب) | ستون_ENABLED;
	memset(&copts, 0, حجم(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(&list, colopts, &copts);
	strbuf_release(&buf);
	string_list_clear(&list, 0);
}

ساکن خالی pretty_print_menus(ساخت string_list *menu_list)
{
	غیرعلامتی صحیح local_colopts = 0;
	ساخت عمود_options copts;

	local_colopts = ستون_ENABLED | ستون_صف;
	memset(&copts, 0, حجم(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(menu_list, local_colopts, &copts);
}

ساکن خالی prompt_help_cmd(صحیح singleton)
{
	clean_print_color(CLEAN_COLOR_HELP);
	لکھو(singleton ?
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
ساکن خالی print_highlight_menu_stuff(ساخت menu_stuff *stuff, صحیح **chosen)
{
	ساخت string_list menu_list = STRING_LIST_INIT_DUP;
	ساخت strbuf menu = STRBUF_INIT;
	ساخت menu_شے *menu_شے;
	ساخت string_list_شے *string_list_شے;
	صحیح i;

	انتخاب (stuff->type) {
	طےشدہ:
		die("Bad type of menu_stuff when print menu");
	حالت MENU_STUFF_TYPE_MENU_شے:
		menu_شے = (ساخت menu_شے *)stuff->stuff;
		برائے (i = 0; i < stuff->nr; i++, menu_شے++) {
			ثابت حرف *p;
			صحیح highlighted = 0;

			p = menu_شے->title;
			اگر ((*chosen)[i] < 0)
				(*chosen)[i] = menu_شے->selected ? 1 : 0;
			strbuf_addf(&menu, "%s%2d: ", (*chosen)[i] ? "*" : " ", i+1);
			برائے (; *p; p++) {
				اگر (!highlighted && *p == menu_شے->hotkey) {
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_PROMPT));
					strbuf_addch(&menu, *p);
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_RESET));
					highlighted = 1;
				} ورنہ {
					strbuf_addch(&menu, *p);
				}
			}
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
		}
		توڑو;
	حالت MENU_STUFF_TYPE_STRING_LIST:
		i = 0;
		for_each_string_list_شے(string_list_شے, (ساخت string_list *)stuff->stuff) {
			اگر ((*chosen)[i] < 0)
				(*chosen)[i] = 0;
			strbuf_addf(&menu, "%s%2d: %s",
				    (*chosen)[i] ? "*" : " ", i+1, string_list_شے->string);
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
			i++;
		}
		توڑو;
	}

	pretty_print_menus(&menu_list);

	strbuf_release(&menu);
	string_list_clear(&menu_list, 0);
}

ساکن صحیح ڈھونڈو_unique(ثابت حرف *choice, ساخت menu_stuff *menu_stuff)
{
	ساخت menu_شے *menu_شے;
	ساخت string_list_شے *string_list_شے;
	صحیح i, len, ملا_2 = 0;

	len = لڑی_لمبائی(choice);
	انتخاب (menu_stuff->type) {
	طےشدہ:
		die("Bad type of menu_stuff when parse choice");
	حالت MENU_STUFF_TYPE_MENU_شے:

		menu_شے = (ساخت menu_شے *)menu_stuff->stuff;
		برائے (i = 0; i < menu_stuff->nr; i++, menu_شے++) {
			اگر (len == 1 && *choice == menu_شے->hotkey) {
				ملا_2 = i + 1;
				توڑو;
			}
			اگر (!strncasecmp(choice, menu_شے->title, len)) {
				اگر (ملا_2) {
					اگر (len == 1) {
						/* continue for hotkey matching */
						ملا_2 = -1;
					} ورنہ {
						ملا_2 = 0;
						توڑو;
					}
				} ورنہ {
					ملا_2 = i + 1;
				}
			}
		}
		توڑو;
	حالت MENU_STUFF_TYPE_STRING_LIST:
		string_list_شے = ((ساخت string_list *)menu_stuff->stuff)->اشیاء;
		برائے (i = 0; i < menu_stuff->nr; i++, string_list_شے++) {
			اگر (!strncasecmp(choice, string_list_شے->string, len)) {
				اگر (ملا_2) {
					ملا_2 = 0;
					توڑو;
				}
				ملا_2 = i + 1;
			}
		}
		توڑو;
	}
	واپس ملا_2;
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
ساکن صحیح parse_choice(ساخت menu_stuff *menu_stuff,
			صحیح is_single,
			حرف *داخلہ,
			صحیح **chosen)
{
	ساخت string_list choice = STRING_LIST_INIT_NODUP;
	ساخت string_list_شے *شے;
	صحیح nr = 0;
	صحیح i;

	string_list_بانٹو_اندر_رکھو_f(&choice, داخلہ,
				     is_single ? "\n" : ", ", -1,
				     STRING_LIST_بانٹو_TRIM);

	for_each_string_list_شے(شے, &choice) {
		ثابت حرف *string;
		صحیح انتخاب_2;
		صحیح bottom = 0, چوٹی = 0;
		صحیح is_range, is_نمبر;

		string = شے->string;
		اگر (!*string)
			جاری;

		/* Input that begins with '-'; unchoose */
		اگر (string[0] == '-') {
			انتخاب_2 = 0;
			string++;
		} ورنہ {
			انتخاب_2 = 1;
		}

		is_range = 0;
		is_نمبر = 1;
		برائے (ثابت حرف *p = string; *p; p++) {
			اگر ('-' == *p) {
				اگر (!is_range) {
					is_range = 1;
					is_نمبر = 0;
				} ورنہ {
					is_نمبر = 0;
					is_range = 0;
					توڑو;
				}
			} ورنہ اگر (!ہندسہ_ہے(*p)) {
				is_نمبر = 0;
				is_range = 0;
				توڑو;
			}
		}

		اگر (is_نمبر) {
			bottom = atoi(string);
			چوٹی = bottom;
		} ورنہ اگر (is_range) {
			bottom = atoi(string);
			/* a range can be specified like 5-7 or 5- */
			اگر (!*(strchr(string, '-') + 1))
				چوٹی = menu_stuff->nr;
			ورنہ
				چوٹی = atoi(strchr(string, '-') + 1);
		} ورنہ اگر (!strcmp(string, "*")) {
			bottom = 1;
			چوٹی = menu_stuff->nr;
		} ورنہ {
			bottom = ڈھونڈو_unique(string, menu_stuff);
			چوٹی = bottom;
		}

		اگر (چوٹی <= 0 || bottom <= 0 || چوٹی > menu_stuff->nr || bottom > چوٹی ||
		    (is_single && bottom != چوٹی)) {
			clean_print_color(CLEAN_COLOR_ERROR);
			لکھو(_("Huh (%s)?\n"), string);
			clean_print_color(CLEAN_COLOR_RESET);
			جاری;
		}

		برائے (i = bottom; i <= چوٹی; i++)
			(*chosen)[i-1] = انتخاب_2;
	}

	string_list_clear(&choice, 0);

	برائے (i = 0; i < menu_stuff->nr; i++)
		nr += (*chosen)[i];
	واپس nr;
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
ساکن صحیح *list_and_انتخاب(ساخت menu_opts *opts, ساخت menu_stuff *stuff)
{
	ساخت strbuf choice = STRBUF_INIT;
	صحیح *chosen, *نتیجہ;
	صحیح nr = 0;
	صحیح eof = 0;
	صحیح i;

	ALLOC_ARRAY(chosen, stuff->nr);
	/* set chosen as uninitialized */
	برائے (i = 0; i < stuff->nr; i++)
		chosen[i] = -1;

	برائے (;;) {
		اگر (opts->سرخی) {
			printf_ln("%s%s%s",
				  clean_get_color(CLEAN_COLOR_سرخی),
				  _(opts->سرخی),
				  clean_get_color(CLEAN_COLOR_RESET));
		}

		/* chosen will be initialized by print_highlight_menu_stuff */
		print_highlight_menu_stuff(stuff, &chosen);

		اگر (opts->flags & MENU_OPTS_LIST_ONLY)
			توڑو;

		اگر (opts->prompt) {
			لکھو("%s%s%s%s",
			       clean_get_color(CLEAN_COLOR_PROMPT),
			       _(opts->prompt),
			       opts->flags & MENU_OPTS_SINGLETON ? "> " : ">> ",
			       clean_get_color(CLEAN_COLOR_RESET));
		}

		اگر (git_read_سطر_interactively(&choice) == EOF) {
			eof = 1;
			توڑو;
		}

		/* help for prompt */
		اگر (!strcmp(choice.buf, "?")) {
			prompt_help_cmd(opts->flags & MENU_OPTS_SINGLETON);
			جاری;
		}

		/* for a multiple-choice menu, press ENTER (empty) will return back */
		اگر (!(opts->flags & MENU_OPTS_SINGLETON) && !choice.len)
			توڑو;

		nr = parse_choice(stuff,
				  opts->flags & MENU_OPTS_SINGLETON,
				  choice.buf,
				  &chosen);

		اگر (opts->flags & MENU_OPTS_SINGLETON) {
			اگر (nr)
				توڑو;
		} ورنہ اگر (opts->flags & MENU_OPTS_فوری) {
			توڑو;
		}
	}

	اگر (eof) {
		نتیجہ = xmalloc(حجم(صحیح));
		*نتیجہ = EOF;
	} ورنہ {
		صحیح j = 0;

		/*
		 * recalculate nr, if return back from menu directly with
		 * default selections.
		 */
		اگر (!nr) {
			برائے (i = 0; i < stuff->nr; i++)
				nr += chosen[i];
		}

		CALLOC_ARRAY(نتیجہ, st_جوڑو(nr, 1));
		برائے (i = 0; i < stuff->nr && j < nr; i++) {
			اگر (chosen[i])
				نتیجہ[j++] = i;
		}
		نتیجہ[j] = EOF;
	}

	آزاد(chosen);
	strbuf_release(&choice);
	واپس نتیجہ;
}

ساکن صحیح clean_cmd(خالی)
{
	واپس MENU_RETURN_NO_LOOP;
}

ساکن صحیح filter_by_patterns_cmd(خالی)
{
	ساخت dir_struct dir = DIR_INIT;
	ساخت strbuf confirm = STRBUF_INIT;
	ساخت نمونہ_list *pl;
	صحیح بدلا = -1, i;

	برائے (;;) {
		ساخت string_list ignore_list = STRING_LIST_INIT_NODUP;
		ساخت string_list_شے *شے;

		اگر (!del_list.nr)
			توڑو;

		اگر (بدلا)
			pretty_print_dels();

		clean_print_color(CLEAN_COLOR_PROMPT);
		لکھو(_("Input ignore patterns>> "));
		clean_print_color(CLEAN_COLOR_RESET);
		اگر (git_read_سطر_interactively(&confirm) == EOF)
			حرف_لکھو('\n');

		/* quit filter_by_pattern mode if press ENTER or Ctrl-D */
		اگر (!confirm.len)
			توڑو;

		pl = جوڑو_نمونہ_list(&dir, EXC_CMDL, "manual exclude");

		string_list_بانٹو_اندر_رکھو_f(&ignore_list, confirm.buf, " ", -1,
					     STRING_LIST_بانٹو_TRIM);

		برائے (i = 0; i < ignore_list.nr; i++) {
			شے = &ignore_list.اشیاء[i];
			اگر (!*شے->string)
				جاری;
			جوڑو_نمونہ(شے->string, "", 0, pl, -(i+1));
		}

		بدلا = 0;
		for_each_string_list_شے(شے, &del_list) {
			صحیح dtype = DT_UNKNOWN;

			اگر (is_excluded(&dir, the_repository->اشاریہ, شے->string, &dtype)) {
				*شے->string = '\0';
				بدلا++;
			}
		}

		اگر (بدلا) {
			string_list_remove_خالی_خانے_اشیاء(&del_list, 0);
		} ورنہ {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf_ln(_("WARNING: Cannot find items matched by: %s"), confirm.buf);
			clean_print_color(CLEAN_COLOR_RESET);
		}

		string_list_clear(&ignore_list, 0);
		dir_clear(&dir);
	}

	strbuf_release(&confirm);
	واپس 0;
}

ساکن صحیح select_by_numbers_cmd(خالی)
{
	ساخت menu_opts menu_opts;
	ساخت menu_stuff menu_stuff;
	ساخت string_list_شے *اشیاء;
	صحیح *chosen;
	صحیح i, j;

	menu_opts.سرخی = تہی;
	menu_opts.prompt = N_("Select items to delete");
	menu_opts.flags = 0;

	menu_stuff.type = MENU_STUFF_TYPE_STRING_LIST;
	menu_stuff.stuff = &del_list;
	menu_stuff.nr = del_list.nr;

	chosen = list_and_انتخاب(&menu_opts, &menu_stuff);
	اشیاء = del_list.اشیاء;
	برائے (i = 0, j = 0; i < del_list.nr; i++) {
		اگر (i < chosen[j]) {
			*(اشیاء[i].string) = '\0';
		} ورنہ اگر (i == chosen[j]) {
			/* delete selected item */
			j++;
			جاری;
		} ورنہ {
			/* end of chosen (chosen[j] == EOF), won't delete */
			*(اشیاء[i].string) = '\0';
		}
	}

	string_list_remove_خالی_خانے_اشیاء(&del_list, 0);

	آزاد(chosen);
	واپس 0;
}

ساکن صحیح ask_each_cmd(خالی)
{
	ساخت strbuf confirm = STRBUF_INIT;
	ساخت strbuf buf = STRBUF_INIT;
	ساخت string_list_شے *شے;
	ثابت حرف *qname;
	صحیح بدلا = 0, eof = 0;

	for_each_string_list_شے(شے, &del_list) {
		/* Ctrl-D should stop removing files */
		اگر (!eof) {
			qname = quote_راستہ(شے->string, تہی, &buf, 0);
			/* TRANSLATORS: Make sure to keep [y/N] as is */
			لکھو(_("Remove %s [y/N]? "), qname);
			اگر (git_read_سطر_interactively(&confirm) == EOF) {
				حرف_لکھو('\n');
				eof = 1;
			}
		}
		اگر (!confirm.len || strncasecmp(confirm.buf, "yes", confirm.len)) {
			*شے->string = '\0';
			بدلا++;
		}
	}

	اگر (بدلا)
		string_list_remove_خالی_خانے_اشیاء(&del_list, 0);

	strbuf_release(&buf);
	strbuf_release(&confirm);
	واپس MENU_RETURN_NO_LOOP;
}

ساکن صحیح quit_cmd(خالی)
{
	string_list_clear(&del_list, 0);
	لکھو(_("Bye.\n"));
	واپس MENU_RETURN_NO_LOOP;
}

ساکن صحیح help_cmd(خالی)
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
	واپس 0;
}

ساکن خالی interactive_main_loop(خالی)
{
	جب (del_list.nr) {
		ساخت menu_opts menu_opts;
		ساخت menu_stuff menu_stuff;
		ساخت menu_شے menus[] = {
			{'c', "clean",			0, clean_cmd},
			{'f', "filter by pattern",	0, filter_by_patterns_cmd},
			{'s', "select by numbers",	0, select_by_numbers_cmd},
			{'a', "ask each",		0, ask_each_cmd},
			{'q', "quit",			0, quit_cmd},
			{'h', "help",			0, help_cmd},
		};
		صحیح *chosen;

		menu_opts.سرخی = N_("*** Commands ***");
		menu_opts.prompt = N_("What now");
		menu_opts.flags = MENU_OPTS_SINGLETON;

		menu_stuff.type = MENU_STUFF_TYPE_MENU_شے;
		menu_stuff.stuff = menus;
		menu_stuff.nr = حجم(menus) / حجم(ساخت menu_شے);

		clean_print_color(CLEAN_COLOR_سرخی);
		printf_ln(Q_("Would remove the following item:",
			     "Would remove the following items:",
			     del_list.nr));
		clean_print_color(CLEAN_COLOR_RESET);

		pretty_print_dels();

		chosen = list_and_انتخاب(&menu_opts, &menu_stuff);

		اگر (*chosen != EOF) {
			صحیح ret;
			ret = menus[*chosen].fn();
			اگر (ret != MENU_RETURN_NO_LOOP) {
				FREE_AND_NULL(chosen);
				اگر (!del_list.nr) {
					clean_print_color(CLEAN_COLOR_ERROR);
					printf_ln(_("No more files to clean, exiting."));
					clean_print_color(CLEAN_COLOR_RESET);
					توڑو;
				}
				جاری;
			}
		} ورنہ {
			quit_cmd();
		}

		FREE_AND_NULL(chosen);
		توڑو;
	}
}

ساکن خالی correct_untracked_entries(ساخت dir_struct *dir)
{
	صحیح src, dst, ign;

	برائے (src = dst = ign = 0; src < dir->nr; src++) {
		/* skip paths in ignored[] that cannot be inside entries[src] */
		جب (ign < dir->ignored_nr &&
		       0 <= cmp_dir_اندراج(&dir->entries[src], &dir->ignored[ign]))
			ign++;

		اگر (ign < dir->ignored_nr &&
		    check_dir_اندراج_شامل_ہے(dir->entries[src], dir->ignored[ign])) {
			/* entries[src] contains an ignored path, so we drop it */
			آزاد(dir->entries[src]);
		} ورنہ {
			ساخت dir_اندراج *ent = dir->entries[src++];

			/* entries[src] does not contain an ignored path, so we keep it */
			dir->entries[dst++] = ent;

			/* then discard paths in entries[] contained inside entries[src] */
			جب (src < dir->nr &&
			       check_dir_اندراج_شامل_ہے(ent, dir->entries[src]))
				آزاد(dir->entries[src++]);

			/* compensate for the outer loop's loop control */
			src--;
		}
	}
	dir->nr = dst;
}

صحیح cmd_clean(صحیح argc,
	      ثابت حرف **argv,
	      ثابت حرف *سابقہ,
	      ساخت repository *repo غیر_مستعمل)
{
	صحیح i, res;
	صحیح dry_تسلسل = 0, remove_directories = 0, quiet = 0, ignored = 0;
	صحیح ignored_only = 0, force = 0, errors = 0, gone = 1;
	صحیح rm_flags = REMOVE_DIR_KEEP_NESTED_GIT;
	ساخت strbuf abs_راستہ = STRBUF_INIT;
	ساخت dir_struct dir = DIR_INIT;
	ساخت pathspec pathspec;
	ساخت strbuf buf = STRBUF_INIT;
	ساخت string_list exclude_list = STRING_LIST_INIT_NODUP;
	ساخت نمونہ_list *pl;
	ساخت string_list_شے *شے;
	ثابت حرف *qname;
	ساخت option options[] = {
		OPT__QUIET(&quiet, N_("do not print names of files removed")),
		OPT_DRY_تسلسل(&dry_تسلسل, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('i', "interactive", &interactive, N_("interactive cleaning")),
		OPT_BOOL('d', تہی, &remove_directories,
				N_("remove whole directories")),
		OPT_CALLBACK_F('e', "exclude", &exclude_list, N_("pattern"),
		  N_("add <pattern> to ignore rules"), PARSE_OPT_NONEG, exclude_cb),
		OPT_BOOL('x', تہی, &ignored, N_("remove ignored files, too")),
		OPT_BOOL('X', تہی, &ignored_only,
				N_("remove only ignored files")),
		OPT_انتہا()
	};

	repo_config(the_repository, git_clean_config, تہی);

	argc = parse_options(argc, argv, سابقہ, options, builtin_clean_usage,
			     0);

	اگر (require_force != 0 && !force && !interactive && !dry_تسلسل)
		die(_("clean.requireForce is true and -f not given: refusing to clean"));

	اگر (force > 1)
		rm_flags = 0;
	ورنہ
		dir.flags |= DIR_SKIP_NESTED_GIT;

	dir.flags |= DIR_SHOW_OTHER_DIRECTORIES;

	اگر (ignored && ignored_only)
		die(_("options '%s' and '%s' cannot be used together"), "-x", "-X");
	اگر (!ignored)
		تیاری_standard_excludes(&dir);
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
			dir.flags |= DIR_SHOW_IGNORED_TOO_منوال_MATCHING;

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
	the_repository->settings.command_requires_بھرا_اشاریہ = 0;

	اگر (repo_read_اشاریہ(the_repository) < 0)
		die(_("index file corrupt"));

	pl = جوڑو_نمونہ_list(&dir, EXC_CMDL, "--exclude option");
	برائے (i = 0; i < exclude_list.nr; i++)
		جوڑو_نمونہ(exclude_list.اشیاء[i].string, "", 0, pl, -(i+1));

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       سابقہ, argv);

	fill_directory(&dir, the_repository->اشاریہ, &pathspec);
	correct_untracked_entries(&dir);

	برائے (i = 0; i < dir.nr; i++) {
		ساخت dir_اندراج *ent = dir.entries[i];
		ساخت stat st;
		ثابت حرف *rel;

		اگر (!اشاریہ_name_is_other(the_repository->اشاریہ, ent->name, ent->len))
			جاری;

		اگر (lstat(ent->name, &st))
			die_errno("Cannot lstat '%s'", ent->name);

		اگر (S_ISDIR(st.st_منوال) && !remove_directories)
			جاری;

		rel = relative_راستہ(ent->name, سابقہ, &buf);
		string_list_append(&del_list, rel);
	}

	dir_clear(&dir);

	اگر (interactive && del_list.nr > 0)
		interactive_main_loop();

	for_each_string_list_شے(شے, &del_list) {
		ساخت stat st;

		strbuf_reset(&abs_راستہ);
		اگر (سابقہ)
			strbuf_addstr(&abs_راستہ, سابقہ);

		strbuf_addstr(&abs_راستہ, شے->string);

		/*
		 * we might have removed this as part of earlier
		 * recursive directory removal, so lstat() here could
		 * fail with ENOENT.
		 */
		اگر (lstat(abs_راستہ.buf, &st))
			جاری;

		اگر (S_ISDIR(st.st_منوال)) {
			اگر (remove_dirs(&abs_راستہ, سابقہ, rm_flags, dry_تسلسل, quiet, &gone))
				errors++;
			اگر (gone && !quiet) {
				qname = quote_راستہ(شے->string, تہی, &buf, 0);
				لکھو(dry_تسلسل ? _(پیغام_would_remove) : _(پیغام_remove), qname);
			}
		} ورنہ {
			res = dry_تسلسل ? 0 : unlink(abs_راستہ.buf);
			اگر (res) {
				صحیح saved_errno = errno;
				qname = quote_راستہ(شے->string, تہی, &buf, 0);
				errno = saved_errno;
				warning_errno(_(پیغام_warn_remove_failed), qname);
				errors++;
			} ورنہ اگر (!quiet) {
				qname = quote_راستہ(شے->string, تہی, &buf, 0);
				لکھو(dry_تسلسل ? _(پیغام_would_remove) : _(پیغام_remove), qname);
			}
		}
	}

	strbuf_release(&abs_راستہ);
	strbuf_release(&buf);
	string_list_clear(&del_list, 0);
	string_list_clear(&exclude_list, 0);
	clear_pathspec(&pathspec);
	واپس (errors != 0);
}
