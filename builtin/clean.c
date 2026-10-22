/*
 * "git clean" builtin command
 *
 * Copyright (C) 2007 Shawn Bohrer
 *
 * Based on git-clean.sh by Pavel Roskin
 */

#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#تعریف_کن DISABLE_SIGN_COMPARE_WARNINGS

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
ساكن بیعلامت صحيح colopts;

ساكن ثابت حرف *ثابت builtin_clean_usage[] = {
	N_("git clean [-d] [-f] [-i] [-n] [-q] [-e <pattern>] [-x | -X] [--] [<pathspec>...]"),
	عدم
};

ساكن ثابت حرف *پیام_remove = N_("Removing %s\n");
ساكن ثابت حرف *پیام_would_remove = N_("Would remove %s\n");
ساكن ثابت حرف *پیام_skip_git_dir = N_("Skipping repository %s\n");
ساكن ثابت حرف *پیام_would_skip_git_dir = N_("Would skip repository %s\n");
ساكن ثابت حرف *پیام_warn_remove_failed = N_("failed to remove %s");
ساكن ثابت حرف *پیام_warn_lstat_failed = N_("could not lstat %s\n");
ساكن ثابت حرف *پیام_skip_cwd = N_("Refusing to remove current working directory\n");
ساكن ثابت حرف *پیام_would_skip_cwd = N_("Would refuse to remove current working directory\n");

تعداد color_clean {
	CLEAN_COLOR_RESET = 0,
	CLEAN_COLOR_PLAIN = 1,
	CLEAN_COLOR_PROMPT = 2,
	CLEAN_COLOR_سرآیند = 3,
	CLEAN_COLOR_HELP = 4,
	CLEAN_COLOR_ERROR = 5
};

ساكن ثابت حرف *color_interactive_slots[] = {
	[CLEAN_COLOR_ERROR]  = "error",
	[CLEAN_COLOR_سرآیند] = "header",
	[CLEAN_COLOR_HELP]   = "help",
	[CLEAN_COLOR_PLAIN]  = "plain",
	[CLEAN_COLOR_PROMPT] = "prompt",
	[CLEAN_COLOR_RESET]  = "reset",
};

ساكن تعداد git_colorbool clean_use_color = GIT_COLOR_UNKNOWN;
ساكن حرف clean_colors[][COLOR_MAXLEN] = {
	[CLEAN_COLOR_ERROR] = GIT_COLOR_BOLD_RED,
	[CLEAN_COLOR_سرآیند] = GIT_COLOR_BOLD,
	[CLEAN_COLOR_HELP] = GIT_COLOR_BOLD_RED,
	[CLEAN_COLOR_PLAIN] = GIT_COLOR_نرمال,
	[CLEAN_COLOR_PROMPT] = GIT_COLOR_BOLD_BLUE,
	[CLEAN_COLOR_RESET] = GIT_COLOR_RESET,
};

#تعریف_کن MENU_OPTS_SINGLETON		01
#تعریف_کن MENU_OPTS_فوری		02
#تعریف_کن MENU_OPTS_LIST_ONLY		04

بنية menu_opts {
	ثابت حرف *سرآیند;
	ثابت حرف *prompt;
	صحيح flags;
};

#تعریف_کن MENU_RETURN_NO_LOOP		10

بنية menu_قلم {
	حرف hotkey;
	ثابت حرف *title;
	صحيح selected;
	صحيح (*fn)(فراغ);
};

تعداد menu_stuff_type {
	MENU_STUFF_TYPE_STRING_LIST = 1,
	MENU_STUFF_TYPE_MENU_قلم
};

بنية menu_stuff {
	تعداد menu_stuff_type type;
	صحيح nr;
	فراغ *stuff;
};

define_list_config_array(color_interactive_slots);

ساكن صحيح git_clean_config(ثابت حرف *var, ثابت حرف *مقدار,
			    ثابت بنية config_context *ctx, فراغ *cb)
{
	ثابت حرف *جایگاه_name;

	إذا (starts_with(var, "column."))
		أرجع git_ستونی_config(var, مقدار, "clean", &colopts);

	/* honors the color.interactive* config variables which also
	   applied in git-add--interactive and git-stash */
	إذا (!strcmp(var, "color.interactive")) {
		clean_use_color = git_config_colorbool(var, مقدار);
		أرجع 0;
	}
	إذا (skip_پیشوند(var, "color.interactive.", &جایگاه_name)) {
		صحيح جایگاه = LOOKUP_CONFIG(color_interactive_slots, جایگاه_name);
		إذا (جایگاه < 0)
			أرجع 0;
		إذا (!مقدار)
			أرجع config_error_nonbool(var);
		أرجع color_parse(مقدار, clean_colors[جایگاه]);
	}

	إذا (!strcmp(var, "clean.requireforce")) {
		require_force = git_config_bool(var, مقدار);
		أرجع 0;
	}

	إذا (git_color_config(var, مقدار, cb) < 0)
		أرجع -1;

	أرجع git_default_config(var, مقدار, ctx, cb);
}

ساكن ثابت حرف *clean_get_color(تعداد color_clean ix)
{
	إذا (want_color(clean_use_color))
		أرجع clean_colors[ix];
	أرجع "";
}

ساكن فراغ clean_print_color(تعداد color_clean ix)
{
	چاپ_کن("%s", clean_get_color(ix));
}

ساكن صحيح exclude_cb(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية string_list *exclude_list = opt->مقدار;
	BUG_ON_OPT_NEG(unset);
	string_list_append(exclude_list, arg);
	أرجع 0;
}

ساكن صحيح remove_dirs(بنية strbuf *مسیر, ثابت حرف *پیشوند, صحيح force_flag,
		صحيح dry_پیاپی, صحيح quiet, صحيح *dir_gone)
{
	DIR *dir;
	بنية strbuf quoted = STRBUF_INIT;
	بنية strbuf realpath = STRBUF_INIT;
	بنية strbuf real_ocwd = STRBUF_INIT;
	بنية dirent *e;
	صحيح res = 0, ret = 0, gone = 1, original_len = مسیر->len, len;
	بنية string_list dels = STRING_LIST_INIT_DUP;

	*dir_gone = 1;

	إذا ((force_flag & REMOVE_DIR_KEEP_NESTED_GIT) &&
	    is_nonbare_repository_dir(مسیر)) {
		إذا (!quiet) {
			quote_مسیر(مسیر->buf, پیشوند, &quoted, 0);
			چاپ_کن(dry_پیاپی ?  _(پیام_would_skip_git_dir) : _(پیام_skip_git_dir),
					quoted.buf);
		}

		*dir_gone = 0;
		اذهب بیرون;
	}

	dir = opendir(مسیر->buf);
	إذا (!dir) {
		/* an empty dir could be removed even if it is unreadble */
		res = dry_پیاپی ? 0 : rmdir(مسیر->buf);
		إذا (res) {
			صحيح saved_errno = errno;
			quote_مسیر(مسیر->buf, پیشوند, &quoted, 0);
			errno = saved_errno;
			warning_errno(_(پیام_warn_remove_failed), quoted.buf);
			*dir_gone = 0;
		}
		ret = res;
		اذهب بیرون;
	}

	strbuf_complete(مسیر, '/');

	len = مسیر->len;
	طالما ((e = readdir_skip_dot_and_dotdot(dir)) != عدم) {
		بنية stat st;

		strbuf_setlen(مسیر, len);
		strbuf_addstr(مسیر, e->d_name);
		إذا (lstat(مسیر->buf, &st))
			warning_errno(_(پیام_warn_lstat_failed), مسیر->buf);
		وإلا إذا (S_ISDIR(st.st_نما)) {
			إذا (remove_dirs(مسیر, پیشوند, force_flag, dry_پیاپی, quiet, &gone))
				ret = 1;
			إذا (gone) {
				quote_مسیر(مسیر->buf, پیشوند, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} وإلا
				*dir_gone = 0;
			استمر;
		} وإلا {
			res = dry_پیاپی ? 0 : unlink(مسیر->buf);
			إذا (!res) {
				quote_مسیر(مسیر->buf, پیشوند, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} وإلا {
				صحيح saved_errno = errno;
				quote_مسیر(مسیر->buf, پیشوند, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(پیام_warn_remove_failed), quoted.buf);
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

	strbuf_setlen(مسیر, original_len);

	إذا (*dir_gone) {
		/*
		 * Normalize path components in path->buf, e.g. change '\' to
		 * '/' on Windows.
		 */
		strbuf_realpath(&realpath, مسیر->buf, 1);

		/*
		 * path and realpath are absolute; for comparison, we would
		 * like to transform startup_info->original_cwd to an absolute
		 * path too.
		 */
		 إذا (startup_info->original_cwd)
			 strbuf_realpath(&real_ocwd,
					 startup_info->original_cwd, 1);

		إذا (!strbuf_cmp(&realpath, &real_ocwd)) {
			چاپ_کن("%s", dry_پیاپی ? _(پیام_would_skip_cwd) : _(پیام_skip_cwd));
			*dir_gone = 0;
		} وإلا {
			res = dry_پیاپی ? 0 : rmdir(مسیر->buf);
			إذا (!res)
				*dir_gone = 1;
			وإلا {
				صحيح saved_errno = errno;
				quote_مسیر(مسیر->buf, پیشوند, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(پیام_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
		}
	}

	إذا (!*dir_gone && !quiet) {
		صحيح i;
		لكل (i = 0; i < dels.nr; i++)
			چاپ_کن(dry_پیاپی ?  _(پیام_would_remove) : _(پیام_remove), dels.اقلام[i].string);
	}
بیرون:
	strbuf_release(&realpath);
	strbuf_release(&real_ocwd);
	strbuf_release(&quoted);
	string_list_clear(&dels, 0);
	أرجع ret;
}

ساكن فراغ pretty_print_dels(فراغ)
{
	بنية string_list list = STRING_LIST_INIT_DUP;
	بنية string_list_قلم *قلم;
	بنية strbuf buf = STRBUF_INIT;
	ثابت حرف *qname;
	بنية ستونی_options copts;

	for_each_string_list_قلم(قلم, &del_list) {
		qname = quote_مسیر(قلم->string, عدم, &buf, 0);
		string_list_append(&list, qname);
	}

	/*
	 * always enable column display, we only consult column.*
	 * about layout strategy and stuff
	 */
	colopts = (colopts & ~ستون_ENABLE_ماسک) | ستون_ENABLED;
	memset(&copts, 0, حجم(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(&list, colopts, &copts);
	strbuf_release(&buf);
	string_list_clear(&list, 0);
}

ساكن فراغ pretty_print_menus(بنية string_list *menu_list)
{
	بیعلامت صحيح local_colopts = 0;
	بنية ستونی_options copts;

	local_colopts = ستون_ENABLED | ستون_ردیف;
	memset(&copts, 0, حجم(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(menu_list, local_colopts, &copts);
}

ساكن فراغ prompt_help_cmd(صحيح singleton)
{
	clean_print_color(CLEAN_COLOR_HELP);
	چاپ_کن(singleton ?
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
	بنية menu_قلم *menu_قلم;
	بنية string_list_قلم *string_list_قلم;
	صحيح i;

	بدل (stuff->type) {
	افتراضي:
		die("Bad type of menu_stuff when print menu");
	حالة MENU_STUFF_TYPE_MENU_قلم:
		menu_قلم = (بنية menu_قلم *)stuff->stuff;
		لكل (i = 0; i < stuff->nr; i++, menu_قلم++) {
			ثابت حرف *p;
			صحيح highlighted = 0;

			p = menu_قلم->title;
			إذا ((*chosen)[i] < 0)
				(*chosen)[i] = menu_قلم->selected ? 1 : 0;
			strbuf_addf(&menu, "%s%2d: ", (*chosen)[i] ? "*" : " ", i+1);
			لكل (; *p; p++) {
				إذا (!highlighted && *p == menu_قلم->hotkey) {
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_PROMPT));
					strbuf_addch(&menu, *p);
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_RESET));
					highlighted = 1;
				} وإلا {
					strbuf_addch(&menu, *p);
				}
			}
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
		}
		اكسر;
	حالة MENU_STUFF_TYPE_STRING_LIST:
		i = 0;
		for_each_string_list_قلم(string_list_قلم, (بنية string_list *)stuff->stuff) {
			إذا ((*chosen)[i] < 0)
				(*chosen)[i] = 0;
			strbuf_addf(&menu, "%s%2d: %s",
				    (*chosen)[i] ? "*" : " ", i+1, string_list_قلم->string);
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

ساكن صحيح پیدا_کن_unique(ثابت حرف *choice, بنية menu_stuff *menu_stuff)
{
	بنية menu_قلم *menu_قلم;
	بنية string_list_قلم *string_list_قلم;
	صحيح i, len, یافت_2 = 0;

	len = طول_رشته(choice);
	بدل (menu_stuff->type) {
	افتراضي:
		die("Bad type of menu_stuff when parse choice");
	حالة MENU_STUFF_TYPE_MENU_قلم:

		menu_قلم = (بنية menu_قلم *)menu_stuff->stuff;
		لكل (i = 0; i < menu_stuff->nr; i++, menu_قلم++) {
			إذا (len == 1 && *choice == menu_قلم->hotkey) {
				یافت_2 = i + 1;
				اكسر;
			}
			إذا (!strncasecmp(choice, menu_قلم->title, len)) {
				إذا (یافت_2) {
					إذا (len == 1) {
						/* continue for hotkey matching */
						یافت_2 = -1;
					} وإلا {
						یافت_2 = 0;
						اكسر;
					}
				} وإلا {
					یافت_2 = i + 1;
				}
			}
		}
		اكسر;
	حالة MENU_STUFF_TYPE_STRING_LIST:
		string_list_قلم = ((بنية string_list *)menu_stuff->stuff)->اقلام;
		لكل (i = 0; i < menu_stuff->nr; i++, string_list_قلم++) {
			إذا (!strncasecmp(choice, string_list_قلم->string, len)) {
				إذا (یافت_2) {
					یافت_2 = 0;
					اكسر;
				}
				یافت_2 = i + 1;
			}
		}
		اكسر;
	}
	أرجع یافت_2;
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
			حرف *درون_داد,
			صحيح **chosen)
{
	بنية string_list choice = STRING_LIST_INIT_NODUP;
	بنية string_list_قلم *قلم;
	صحيح nr = 0;
	صحيح i;

	string_list_تقسیم_درون_قراربده_f(&choice, درون_داد,
				     is_single ? "\n" : ", ", -1,
				     STRING_LIST_تقسیم_TRIM);

	for_each_string_list_قلم(قلم, &choice) {
		ثابت حرف *string;
		صحيح انتخاب;
		صحيح bottom = 0, بالاترین = 0;
		صحيح is_range, is_عدد;

		string = قلم->string;
		إذا (!*string)
			استمر;

		/* Input that begins with '-'; unchoose */
		إذا (string[0] == '-') {
			انتخاب = 0;
			string++;
		} وإلا {
			انتخاب = 1;
		}

		is_range = 0;
		is_عدد = 1;
		لكل (ثابت حرف *p = string; *p; p++) {
			إذا ('-' == *p) {
				إذا (!is_range) {
					is_range = 1;
					is_عدد = 0;
				} وإلا {
					is_عدد = 0;
					is_range = 0;
					اكسر;
				}
			} وإلا إذا (!آیا_رقم(*p)) {
				is_عدد = 0;
				is_range = 0;
				اكسر;
			}
		}

		إذا (is_عدد) {
			bottom = atoi(string);
			بالاترین = bottom;
		} وإلا إذا (is_range) {
			bottom = atoi(string);
			/* a range can be specified like 5-7 or 5- */
			إذا (!*(strchr(string, '-') + 1))
				بالاترین = menu_stuff->nr;
			وإلا
				بالاترین = atoi(strchr(string, '-') + 1);
		} وإلا إذا (!strcmp(string, "*")) {
			bottom = 1;
			بالاترین = menu_stuff->nr;
		} وإلا {
			bottom = پیدا_کن_unique(string, menu_stuff);
			بالاترین = bottom;
		}

		إذا (بالاترین <= 0 || bottom <= 0 || بالاترین > menu_stuff->nr || bottom > بالاترین ||
		    (is_single && bottom != بالاترین)) {
			clean_print_color(CLEAN_COLOR_ERROR);
			چاپ_کن(_("Huh (%s)?\n"), string);
			clean_print_color(CLEAN_COLOR_RESET);
			استمر;
		}

		لكل (i = bottom; i <= بالاترین; i++)
			(*chosen)[i-1] = انتخاب;
	}

	string_list_clear(&choice, 0);

	لكل (i = 0; i < menu_stuff->nr; i++)
		nr += (*chosen)[i];
	أرجع nr;
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
ساكن صحيح *list_and_انتخاب(بنية menu_opts *opts, بنية menu_stuff *stuff)
{
	بنية strbuf choice = STRBUF_INIT;
	صحيح *chosen, *نتیجه;
	صحيح nr = 0;
	صحيح eof = 0;
	صحيح i;

	ALLOC_ARRAY(chosen, stuff->nr);
	/* set chosen as uninitialized */
	لكل (i = 0; i < stuff->nr; i++)
		chosen[i] = -1;

	لكل (;;) {
		إذا (opts->سرآیند) {
			printf_ln("%s%s%s",
				  clean_get_color(CLEAN_COLOR_سرآیند),
				  _(opts->سرآیند),
				  clean_get_color(CLEAN_COLOR_RESET));
		}

		/* chosen will be initialized by print_highlight_menu_stuff */
		print_highlight_menu_stuff(stuff, &chosen);

		إذا (opts->flags & MENU_OPTS_LIST_ONLY)
			اكسر;

		إذا (opts->prompt) {
			چاپ_کن("%s%s%s%s",
			       clean_get_color(CLEAN_COLOR_PROMPT),
			       _(opts->prompt),
			       opts->flags & MENU_OPTS_SINGLETON ? "> " : ">> ",
			       clean_get_color(CLEAN_COLOR_RESET));
		}

		إذا (git_read_سطر_interactively(&choice) == EOF) {
			eof = 1;
			اكسر;
		}

		/* help for prompt */
		إذا (!strcmp(choice.buf, "?")) {
			prompt_help_cmd(opts->flags & MENU_OPTS_SINGLETON);
			استمر;
		}

		/* for a multiple-choice menu, press ENTER (empty) will return back */
		إذا (!(opts->flags & MENU_OPTS_SINGLETON) && !choice.len)
			اكسر;

		nr = parse_choice(stuff,
				  opts->flags & MENU_OPTS_SINGLETON,
				  choice.buf,
				  &chosen);

		إذا (opts->flags & MENU_OPTS_SINGLETON) {
			إذا (nr)
				اكسر;
		} وإلا إذا (opts->flags & MENU_OPTS_فوری) {
			اكسر;
		}
	}

	إذا (eof) {
		نتیجه = xmalloc(حجم(صحيح));
		*نتیجه = EOF;
	} وإلا {
		صحيح j = 0;

		/*
		 * recalculate nr, if return back from menu directly with
		 * default selections.
		 */
		إذا (!nr) {
			لكل (i = 0; i < stuff->nr; i++)
				nr += chosen[i];
		}

		CALLOC_ARRAY(نتیجه, st_جمع_کن(nr, 1));
		لكل (i = 0; i < stuff->nr && j < nr; i++) {
			إذا (chosen[i])
				نتیجه[j++] = i;
		}
		نتیجه[j] = EOF;
	}

	حرر(chosen);
	strbuf_release(&choice);
	أرجع نتیجه;
}

ساكن صحيح clean_cmd(فراغ)
{
	أرجع MENU_RETURN_NO_LOOP;
}

ساكن صحيح filter_by_patterns_cmd(فراغ)
{
	بنية dir_struct dir = DIR_INIT;
	بنية strbuf confirm = STRBUF_INIT;
	بنية الگو_list *pl;
	صحيح تغییریافته = -1, i;

	لكل (;;) {
		بنية string_list ignore_list = STRING_LIST_INIT_NODUP;
		بنية string_list_قلم *قلم;

		إذا (!del_list.nr)
			اكسر;

		إذا (تغییریافته)
			pretty_print_dels();

		clean_print_color(CLEAN_COLOR_PROMPT);
		چاپ_کن(_("Input ignore patterns>> "));
		clean_print_color(CLEAN_COLOR_RESET);
		إذا (git_read_سطر_interactively(&confirm) == EOF)
			حرف_چاپ_کن('\n');

		/* quit filter_by_pattern mode if press ENTER or Ctrl-D */
		إذا (!confirm.len)
			اكسر;

		pl = جمع_کن_الگو_list(&dir, EXC_CMDL, "manual exclude");

		string_list_تقسیم_درون_قراربده_f(&ignore_list, confirm.buf, " ", -1,
					     STRING_LIST_تقسیم_TRIM);

		لكل (i = 0; i < ignore_list.nr; i++) {
			قلم = &ignore_list.اقلام[i];
			إذا (!*قلم->string)
				استمر;
			جمع_کن_الگو(قلم->string, "", 0, pl, -(i+1));
		}

		تغییریافته = 0;
		for_each_string_list_قلم(قلم, &del_list) {
			صحيح dtype = DT_UNKNOWN;

			إذا (is_excluded(&dir, the_repository->شاخص, قلم->string, &dtype)) {
				*قلم->string = '\0';
				تغییریافته++;
			}
		}

		إذا (تغییریافته) {
			string_list_remove_خالی_اقلام(&del_list, 0);
		} وإلا {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf_ln(_("WARNING: Cannot find items matched by: %s"), confirm.buf);
			clean_print_color(CLEAN_COLOR_RESET);
		}

		string_list_clear(&ignore_list, 0);
		dir_clear(&dir);
	}

	strbuf_release(&confirm);
	أرجع 0;
}

ساكن صحيح select_by_numbers_cmd(فراغ)
{
	بنية menu_opts menu_opts;
	بنية menu_stuff menu_stuff;
	بنية string_list_قلم *اقلام;
	صحيح *chosen;
	صحيح i, j;

	menu_opts.سرآیند = عدم;
	menu_opts.prompt = N_("Select items to delete");
	menu_opts.flags = 0;

	menu_stuff.type = MENU_STUFF_TYPE_STRING_LIST;
	menu_stuff.stuff = &del_list;
	menu_stuff.nr = del_list.nr;

	chosen = list_and_انتخاب(&menu_opts, &menu_stuff);
	اقلام = del_list.اقلام;
	لكل (i = 0, j = 0; i < del_list.nr; i++) {
		إذا (i < chosen[j]) {
			*(اقلام[i].string) = '\0';
		} وإلا إذا (i == chosen[j]) {
			/* delete selected item */
			j++;
			استمر;
		} وإلا {
			/* end of chosen (chosen[j] == EOF), won't delete */
			*(اقلام[i].string) = '\0';
		}
	}

	string_list_remove_خالی_اقلام(&del_list, 0);

	حرر(chosen);
	أرجع 0;
}

ساكن صحيح ask_each_cmd(فراغ)
{
	بنية strbuf confirm = STRBUF_INIT;
	بنية strbuf buf = STRBUF_INIT;
	بنية string_list_قلم *قلم;
	ثابت حرف *qname;
	صحيح تغییریافته = 0, eof = 0;

	for_each_string_list_قلم(قلم, &del_list) {
		/* Ctrl-D should stop removing files */
		إذا (!eof) {
			qname = quote_مسیر(قلم->string, عدم, &buf, 0);
			/* TRANSLATORS: Make sure to keep [y/N] as is */
			چاپ_کن(_("Remove %s [y/N]? "), qname);
			إذا (git_read_سطر_interactively(&confirm) == EOF) {
				حرف_چاپ_کن('\n');
				eof = 1;
			}
		}
		إذا (!confirm.len || strncasecmp(confirm.buf, "yes", confirm.len)) {
			*قلم->string = '\0';
			تغییریافته++;
		}
	}

	إذا (تغییریافته)
		string_list_remove_خالی_اقلام(&del_list, 0);

	strbuf_release(&buf);
	strbuf_release(&confirm);
	أرجع MENU_RETURN_NO_LOOP;
}

ساكن صحيح quit_cmd(فراغ)
{
	string_list_clear(&del_list, 0);
	چاپ_کن(_("Bye.\n"));
	أرجع MENU_RETURN_NO_LOOP;
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
	أرجع 0;
}

ساكن فراغ interactive_main_loop(فراغ)
{
	طالما (del_list.nr) {
		بنية menu_opts menu_opts;
		بنية menu_stuff menu_stuff;
		بنية menu_قلم menus[] = {
			{'c', "clean",			0, clean_cmd},
			{'f', "filter by pattern",	0, filter_by_patterns_cmd},
			{'s', "select by numbers",	0, select_by_numbers_cmd},
			{'a', "ask each",		0, ask_each_cmd},
			{'q', "quit",			0, quit_cmd},
			{'h', "help",			0, help_cmd},
		};
		صحيح *chosen;

		menu_opts.سرآیند = N_("*** Commands ***");
		menu_opts.prompt = N_("What now");
		menu_opts.flags = MENU_OPTS_SINGLETON;

		menu_stuff.type = MENU_STUFF_TYPE_MENU_قلم;
		menu_stuff.stuff = menus;
		menu_stuff.nr = حجم(menus) / حجم(بنية menu_قلم);

		clean_print_color(CLEAN_COLOR_سرآیند);
		printf_ln(Q_("Would remove the following item:",
			     "Would remove the following items:",
			     del_list.nr));
		clean_print_color(CLEAN_COLOR_RESET);

		pretty_print_dels();

		chosen = list_and_انتخاب(&menu_opts, &menu_stuff);

		إذا (*chosen != EOF) {
			صحيح ret;
			ret = menus[*chosen].fn();
			إذا (ret != MENU_RETURN_NO_LOOP) {
				FREE_AND_NULL(chosen);
				إذا (!del_list.nr) {
					clean_print_color(CLEAN_COLOR_ERROR);
					printf_ln(_("No more files to clean, exiting."));
					clean_print_color(CLEAN_COLOR_RESET);
					اكسر;
				}
				استمر;
			}
		} وإلا {
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
		طالما (ign < dir->ignored_nr &&
		       0 <= cmp_dir_مدخل(&dir->entries[src], &dir->ignored[ign]))
			ign++;

		إذا (ign < dir->ignored_nr &&
		    check_dir_مدخل_شامل_است(dir->entries[src], dir->ignored[ign])) {
			/* entries[src] contains an ignored path, so we drop it */
			حرر(dir->entries[src]);
		} وإلا {
			بنية dir_مدخل *ent = dir->entries[src++];

			/* entries[src] does not contain an ignored path, so we keep it */
			dir->entries[dst++] = ent;

			/* then discard paths in entries[] contained inside entries[src] */
			طالما (src < dir->nr &&
			       check_dir_مدخل_شامل_است(ent, dir->entries[src]))
				حرر(dir->entries[src++]);

			/* compensate for the outer loop's loop control */
			src--;
		}
	}
	dir->nr = dst;
}

صحيح cmd_clean(صحيح argc,
	      ثابت حرف **argv,
	      ثابت حرف *پیشوند,
	      بنية repository *repo استفادهنشده)
{
	صحيح i, res;
	صحيح dry_پیاپی = 0, remove_directories = 0, quiet = 0, ignored = 0;
	صحيح ignored_only = 0, force = 0, errors = 0, gone = 1;
	صحيح rm_flags = REMOVE_DIR_KEEP_NESTED_GIT;
	بنية strbuf abs_مسیر = STRBUF_INIT;
	بنية dir_struct dir = DIR_INIT;
	بنية pathspec pathspec;
	بنية strbuf buf = STRBUF_INIT;
	بنية string_list exclude_list = STRING_LIST_INIT_NODUP;
	بنية الگو_list *pl;
	بنية string_list_قلم *قلم;
	ثابت حرف *qname;
	بنية option options[] = {
		OPT__QUIET(&quiet, N_("do not print names of files removed")),
		OPT_DRY_پیاپی(&dry_پیاپی, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('i', "interactive", &interactive, N_("interactive cleaning")),
		OPT_BOOL('d', عدم, &remove_directories,
				N_("remove whole directories")),
		OPT_CALLBACK_F('e', "exclude", &exclude_list, N_("pattern"),
		  N_("add <pattern> to ignore rules"), PARSE_OPT_NONEG, exclude_cb),
		OPT_BOOL('x', عدم, &ignored, N_("remove ignored files, too")),
		OPT_BOOL('X', عدم, &ignored_only,
				N_("remove only ignored files")),
		OPT_انتها()
	};

	repo_config(the_repository, git_clean_config, عدم);

	argc = parse_options(argc, argv, پیشوند, options, builtin_clean_usage,
			     0);

	إذا (require_force != 0 && !force && !interactive && !dry_پیاپی)
		die(_("clean.requireForce is true and -f not given: refusing to clean"));

	إذا (force > 1)
		rm_flags = 0;
	وإلا
		dir.flags |= DIR_SKIP_NESTED_GIT;

	dir.flags |= DIR_SHOW_OTHER_DIRECTORIES;

	إذا (ignored && ignored_only)
		die(_("options '%s' and '%s' cannot be used together"), "-x", "-X");
	إذا (!ignored)
		راهاندازی_standard_excludes(&dir);
	إذا (ignored_only)
		dir.flags |= DIR_SHOW_IGNORED;

	إذا (argc) {
		/*
		 * Remaining args implies pathspecs specified, and we should
		 * recurse within those.
		 */
		remove_directories = 1;
	}

	إذا (remove_directories && !ignored_only) {
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
		إذا (!ignored)
			dir.flags |= DIR_SHOW_IGNORED_TOO_نما_MATCHING;

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
	the_repository->settings.command_requires_پر_شاخص = 0;

	إذا (repo_read_شاخص(the_repository) < 0)
		die(_("index file corrupt"));

	pl = جمع_کن_الگو_list(&dir, EXC_CMDL, "--exclude option");
	لكل (i = 0; i < exclude_list.nr; i++)
		جمع_کن_الگو(exclude_list.اقلام[i].string, "", 0, pl, -(i+1));

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       پیشوند, argv);

	fill_directory(&dir, the_repository->شاخص, &pathspec);
	correct_untracked_entries(&dir);

	لكل (i = 0; i < dir.nr; i++) {
		بنية dir_مدخل *ent = dir.entries[i];
		بنية stat st;
		ثابت حرف *rel;

		إذا (!شاخص_name_is_other(the_repository->شاخص, ent->name, ent->len))
			استمر;

		إذا (lstat(ent->name, &st))
			die_errno("Cannot lstat '%s'", ent->name);

		إذا (S_ISDIR(st.st_نما) && !remove_directories)
			استمر;

		rel = relative_مسیر(ent->name, پیشوند, &buf);
		string_list_append(&del_list, rel);
	}

	dir_clear(&dir);

	إذا (interactive && del_list.nr > 0)
		interactive_main_loop();

	for_each_string_list_قلم(قلم, &del_list) {
		بنية stat st;

		strbuf_reset(&abs_مسیر);
		إذا (پیشوند)
			strbuf_addstr(&abs_مسیر, پیشوند);

		strbuf_addstr(&abs_مسیر, قلم->string);

		/*
		 * we might have removed this as part of earlier
		 * recursive directory removal, so lstat() here could
		 * fail with ENOENT.
		 */
		إذا (lstat(abs_مسیر.buf, &st))
			استمر;

		إذا (S_ISDIR(st.st_نما)) {
			إذا (remove_dirs(&abs_مسیر, پیشوند, rm_flags, dry_پیاپی, quiet, &gone))
				errors++;
			إذا (gone && !quiet) {
				qname = quote_مسیر(قلم->string, عدم, &buf, 0);
				چاپ_کن(dry_پیاپی ? _(پیام_would_remove) : _(پیام_remove), qname);
			}
		} وإلا {
			res = dry_پیاپی ? 0 : unlink(abs_مسیر.buf);
			إذا (res) {
				صحيح saved_errno = errno;
				qname = quote_مسیر(قلم->string, عدم, &buf, 0);
				errno = saved_errno;
				warning_errno(_(پیام_warn_remove_failed), qname);
				errors++;
			} وإلا إذا (!quiet) {
				qname = quote_مسیر(قلم->string, عدم, &buf, 0);
				چاپ_کن(dry_پیاپی ? _(پیام_would_remove) : _(پیام_remove), qname);
			}
		}
	}

	strbuf_release(&abs_مسیر);
	strbuf_release(&buf);
	string_list_clear(&del_list, 0);
	string_list_clear(&exclude_list, 0);
	clear_pathspec(&pathspec);
	أرجع (errors != 0);
}
