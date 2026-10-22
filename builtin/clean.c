/*
 * "git clean" builtin command
 *
 * Copyright (C) 2007 Shawn Bohrer
 *
 * Based on git-clean.sh by Pavel Roskin
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

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
ساكن بنانشانی صحيح colopts;

ساكن ثابت حرف *ثابت builtin_clean_usage[] = {
	N_("git clean [-d] [-f] [-i] [-n] [-q] [-e <pattern>] [-x | -X] [--] [<pathspec>...]"),
	عدم
};

ساكن ثابت حرف *پیغام_remove = N_("Removing %s\n");
ساكن ثابت حرف *پیغام_would_remove = N_("Would remove %s\n");
ساكن ثابت حرف *پیغام_skip_git_dir = N_("Skipping repository %s\n");
ساكن ثابت حرف *پیغام_would_skip_git_dir = N_("Would skip repository %s\n");
ساكن ثابت حرف *پیغام_warn_remove_failed = N_("failed to remove %s");
ساكن ثابت حرف *پیغام_warn_lstat_failed = N_("could not lstat %s\n");
ساكن ثابت حرف *پیغام_skip_cwd = N_("Refusing to remove current working directory\n");
ساكن ثابت حرف *پیغام_would_skip_cwd = N_("Would refuse to remove current working directory\n");

تعداد color_clean {
	CLEAN_COLOR_RESET = 0,
	CLEAN_COLOR_PLAIN = 1,
	CLEAN_COLOR_PROMPT = 2,
	CLEAN_COLOR_سرخی = 3,
	CLEAN_COLOR_HELP = 4,
	CLEAN_COLOR_ERROR = 5
};

ساكن ثابت حرف *color_interactive_slots[] = {
	[CLEAN_COLOR_ERROR]  = "error",
	[CLEAN_COLOR_سرخی] = "header",
	[CLEAN_COLOR_HELP]   = "help",
	[CLEAN_COLOR_PLAIN]  = "plain",
	[CLEAN_COLOR_PROMPT] = "prompt",
	[CLEAN_COLOR_RESET]  = "reset",
};

ساكن تعداد git_colorbool clean_use_color = GIT_COLOR_UNKNOWN;
ساكن حرف clean_colors[][COLOR_MAXLEN] = {
	[CLEAN_COLOR_ERROR] = GIT_COLOR_BOLD_RED,
	[CLEAN_COLOR_سرخی] = GIT_COLOR_BOLD,
	[CLEAN_COLOR_HELP] = GIT_COLOR_BOLD_RED,
	[CLEAN_COLOR_PLAIN] = GIT_COLOR_عام,
	[CLEAN_COLOR_PROMPT] = GIT_COLOR_BOLD_BLUE,
	[CLEAN_COLOR_RESET] = GIT_COLOR_RESET,
};

#تعریف_کرو MENU_OPTS_SINGLETON		01
#تعریف_کرو MENU_OPTS_فوری		02
#تعریف_کرو MENU_OPTS_LIST_ONLY		04

بنية menu_opts {
	ثابت حرف *سرخی;
	ثابت حرف *prompt;
	صحيح flags;
};

#تعریف_کرو MENU_RETURN_NO_LOOP		10

بنية menu_شے {
	حرف hotkey;
	ثابت حرف *title;
	صحيح selected;
	صحيح (*fn)(فراغ);
};

تعداد menu_stuff_type {
	MENU_STUFF_TYPE_STRING_LIST = 1,
	MENU_STUFF_TYPE_MENU_شے
};

بنية menu_stuff {
	تعداد menu_stuff_type type;
	صحيح nr;
	فراغ *stuff;
};

define_list_config_array(color_interactive_slots);

ساكن صحيح git_clean_config(ثابت حرف *var, ثابت حرف *قدر,
			    ثابت بنية config_context *ctx, فراغ *cb)
{
	ثابت حرف *تھاں_name;

	إذا (starts_with(var, "column."))
		أرجع git_کالم_config(var, قدر, "clean", &colopts);

	/* honors the color.interactive* config variables which also
	   applied in git-add--interactive and git-stash */
	إذا (!strcmp(var, "color.interactive")) {
		clean_use_color = git_config_colorbool(var, قدر);
		أرجع 0;
	}
	إذا (skip_اگیتر(var, "color.interactive.", &تھاں_name)) {
		صحيح تھاں = LOOKUP_CONFIG(color_interactive_slots, تھاں_name);
		إذا (تھاں < 0)
			أرجع 0;
		إذا (!قدر)
			أرجع config_error_nonbool(var);
		أرجع color_parse(قدر, clean_colors[تھاں]);
	}

	إذا (!strcmp(var, "clean.requireforce")) {
		require_force = git_config_bool(var, قدر);
		أرجع 0;
	}

	إذا (git_color_config(var, قدر, cb) < 0)
		أرجع -1;

	أرجع git_default_config(var, قدر, ctx, cb);
}

ساكن ثابت حرف *clean_get_color(تعداد color_clean ix)
{
	إذا (want_color(clean_use_color))
		أرجع clean_colors[ix];
	أرجع "";
}

ساكن فراغ clean_print_color(تعداد color_clean ix)
{
	لکھو("%s", clean_get_color(ix));
}

ساكن صحيح exclude_cb(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية string_list *exclude_list = opt->قدر;
	BUG_ON_OPT_NEG(unset);
	string_list_append(exclude_list, arg);
	أرجع 0;
}

ساكن صحيح remove_dirs(بنية strbuf *رستہ, ثابت حرف *اگیتر, صحيح force_flag,
		صحيح dry_لگاتار, صحيح quiet, صحيح *dir_gone)
{
	DIR *dir;
	بنية strbuf quoted = STRBUF_INIT;
	بنية strbuf realpath = STRBUF_INIT;
	بنية strbuf real_ocwd = STRBUF_INIT;
	بنية dirent *e;
	صحيح res = 0, ret = 0, gone = 1, original_len = رستہ->len, len;
	بنية string_list dels = STRING_LIST_INIT_DUP;

	*dir_gone = 1;

	إذا ((force_flag & REMOVE_DIR_KEEP_NESTED_GIT) &&
	    is_nonbare_repository_dir(رستہ)) {
		إذا (!quiet) {
			quote_رستہ(رستہ->buf, اگیتر, &quoted, 0);
			لکھو(dry_لگاتار ?  _(پیغام_would_skip_git_dir) : _(پیغام_skip_git_dir),
					quoted.buf);
		}

		*dir_gone = 0;
		اذهب باہرلا;
	}

	dir = opendir(رستہ->buf);
	إذا (!dir) {
		/* an empty dir could be removed even if it is unreadble */
		res = dry_لگاتار ? 0 : rmdir(رستہ->buf);
		إذا (res) {
			صحيح saved_errno = errno;
			quote_رستہ(رستہ->buf, اگیتر, &quoted, 0);
			errno = saved_errno;
			warning_errno(_(پیغام_warn_remove_failed), quoted.buf);
			*dir_gone = 0;
		}
		ret = res;
		اذهب باہرلا;
	}

	strbuf_complete(رستہ, '/');

	len = رستہ->len;
	طالما ((e = readdir_skip_dot_and_dotdot(dir)) != عدم) {
		بنية stat st;

		strbuf_setlen(رستہ, len);
		strbuf_addstr(رستہ, e->d_name);
		إذا (lstat(رستہ->buf, &st))
			warning_errno(_(پیغام_warn_lstat_failed), رستہ->buf);
		وإلا إذا (S_ISDIR(st.st_منوال)) {
			إذا (remove_dirs(رستہ, اگیتر, force_flag, dry_لگاتار, quiet, &gone))
				ret = 1;
			إذا (gone) {
				quote_رستہ(رستہ->buf, اگیتر, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} وإلا
				*dir_gone = 0;
			استمر;
		} وإلا {
			res = dry_لگاتار ? 0 : unlink(رستہ->buf);
			إذا (!res) {
				quote_رستہ(رستہ->buf, اگیتر, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} وإلا {
				صحيح saved_errno = errno;
				quote_رستہ(رستہ->buf, اگیتر, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(پیغام_warn_remove_failed), quoted.buf);
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

	strbuf_setlen(رستہ, original_len);

	إذا (*dir_gone) {
		/*
		 * Normalize path components in path->buf, e.g. change '\' to
		 * '/' on Windows.
		 */
		strbuf_realpath(&realpath, رستہ->buf, 1);

		/*
		 * path and realpath are absolute; for comparison, we would
		 * like to transform startup_info->original_cwd to an absolute
		 * path too.
		 */
		 إذا (startup_info->original_cwd)
			 strbuf_realpath(&real_ocwd,
					 startup_info->original_cwd, 1);

		إذا (!strbuf_cmp(&realpath, &real_ocwd)) {
			لکھو("%s", dry_لگاتار ? _(پیغام_would_skip_cwd) : _(پیغام_skip_cwd));
			*dir_gone = 0;
		} وإلا {
			res = dry_لگاتار ? 0 : rmdir(رستہ->buf);
			إذا (!res)
				*dir_gone = 1;
			وإلا {
				صحيح saved_errno = errno;
				quote_رستہ(رستہ->buf, اگیتر, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(پیغام_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
		}
	}

	إذا (!*dir_gone && !quiet) {
		صحيح i;
		لكل (i = 0; i < dels.nr; i++)
			لکھو(dry_لگاتار ?  _(پیغام_would_remove) : _(پیغام_remove), dels.اشیاء[i].string);
	}
باہرلا:
	strbuf_release(&realpath);
	strbuf_release(&real_ocwd);
	strbuf_release(&quoted);
	string_list_clear(&dels, 0);
	أرجع ret;
}

ساكن فراغ pretty_print_dels(فراغ)
{
	بنية string_list list = STRING_LIST_INIT_DUP;
	بنية string_list_شے *شے;
	بنية strbuf buf = STRBUF_INIT;
	ثابت حرف *qname;
	بنية کالم_options copts;

	for_each_string_list_شے(شے, &del_list) {
		qname = quote_رستہ(شے->string, عدم, &buf, 0);
		string_list_append(&list, qname);
	}

	/*
	 * always enable column display, we only consult column.*
	 * about layout strategy and stuff
	 */
	colopts = (colopts & ~خانہ_ENABLE_نقاب) | خانہ_ENABLED;
	memset(&copts, 0, حجم(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(&list, colopts, &copts);
	strbuf_release(&buf);
	string_list_clear(&list, 0);
}

ساكن فراغ pretty_print_menus(بنية string_list *menu_list)
{
	بنانشانی صحيح local_colopts = 0;
	بنية کالم_options copts;

	local_colopts = خانہ_ENABLED | خانہ_صف;
	memset(&copts, 0, حجم(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(menu_list, local_colopts, &copts);
}

ساكن فراغ prompt_help_cmd(صحيح singleton)
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
ساكن فراغ print_highlight_menu_stuff(بنية menu_stuff *stuff, صحيح **chosen)
{
	بنية string_list menu_list = STRING_LIST_INIT_DUP;
	بنية strbuf menu = STRBUF_INIT;
	بنية menu_شے *menu_شے;
	بنية string_list_شے *string_list_شے;
	صحيح i;

	بدل (stuff->type) {
	افتراضي:
		die("Bad type of menu_stuff when print menu");
	حالة MENU_STUFF_TYPE_MENU_شے:
		menu_شے = (بنية menu_شے *)stuff->stuff;
		لكل (i = 0; i < stuff->nr; i++, menu_شے++) {
			ثابت حرف *p;
			صحيح highlighted = 0;

			p = menu_شے->title;
			إذا ((*chosen)[i] < 0)
				(*chosen)[i] = menu_شے->selected ? 1 : 0;
			strbuf_addf(&menu, "%s%2d: ", (*chosen)[i] ? "*" : " ", i+1);
			لكل (; *p; p++) {
				إذا (!highlighted && *p == menu_شے->hotkey) {
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
		for_each_string_list_شے(string_list_شے, (بنية string_list *)stuff->stuff) {
			إذا ((*chosen)[i] < 0)
				(*chosen)[i] = 0;
			strbuf_addf(&menu, "%s%2d: %s",
				    (*chosen)[i] ? "*" : " ", i+1, string_list_شے->string);
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

ساكن صحيح لبھو_unique(ثابت حرف *choice, بنية menu_stuff *menu_stuff)
{
	بنية menu_شے *menu_شے;
	بنية string_list_شے *string_list_شے;
	صحيح i, len, لبھیا_2 = 0;

	len = لڑی_لمبائی(choice);
	بدل (menu_stuff->type) {
	افتراضي:
		die("Bad type of menu_stuff when parse choice");
	حالة MENU_STUFF_TYPE_MENU_شے:

		menu_شے = (بنية menu_شے *)menu_stuff->stuff;
		لكل (i = 0; i < menu_stuff->nr; i++, menu_شے++) {
			إذا (len == 1 && *choice == menu_شے->hotkey) {
				لبھیا_2 = i + 1;
				اكسر;
			}
			إذا (!strncasecmp(choice, menu_شے->title, len)) {
				إذا (لبھیا_2) {
					إذا (len == 1) {
						/* continue for hotkey matching */
						لبھیا_2 = -1;
					} وإلا {
						لبھیا_2 = 0;
						اكسر;
					}
				} وإلا {
					لبھیا_2 = i + 1;
				}
			}
		}
		اكسر;
	حالة MENU_STUFF_TYPE_STRING_LIST:
		string_list_شے = ((بنية string_list *)menu_stuff->stuff)->اشیاء;
		لكل (i = 0; i < menu_stuff->nr; i++, string_list_شے++) {
			إذا (!strncasecmp(choice, string_list_شے->string, len)) {
				إذا (لبھیا_2) {
					لبھیا_2 = 0;
					اكسر;
				}
				لبھیا_2 = i + 1;
			}
		}
		اكسر;
	}
	أرجع لبھیا_2;
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
			حرف *داخلہ,
			صحيح **chosen)
{
	بنية string_list choice = STRING_LIST_INIT_NODUP;
	بنية string_list_شے *شے;
	صحيح nr = 0;
	صحيح i;

	string_list_ونڈو_وچ_رکھو_f(&choice, داخلہ,
				     is_single ? "\n" : ", ", -1,
				     STRING_LIST_ونڈو_TRIM);

	for_each_string_list_شے(شے, &choice) {
		ثابت حرف *string;
		صحيح چنو;
		صحيح bottom = 0, چوٹی = 0;
		صحيح is_range, is_نمبر;

		string = شے->string;
		إذا (!*string)
			استمر;

		/* Input that begins with '-'; unchoose */
		إذا (string[0] == '-') {
			چنو = 0;
			string++;
		} وإلا {
			چنو = 1;
		}

		is_range = 0;
		is_نمبر = 1;
		لكل (ثابت حرف *p = string; *p; p++) {
			إذا ('-' == *p) {
				إذا (!is_range) {
					is_range = 1;
					is_نمبر = 0;
				} وإلا {
					is_نمبر = 0;
					is_range = 0;
					اكسر;
				}
			} وإلا إذا (!ہندسہ_اے(*p)) {
				is_نمبر = 0;
				is_range = 0;
				اكسر;
			}
		}

		إذا (is_نمبر) {
			bottom = atoi(string);
			چوٹی = bottom;
		} وإلا إذا (is_range) {
			bottom = atoi(string);
			/* a range can be specified like 5-7 or 5- */
			إذا (!*(strchr(string, '-') + 1))
				چوٹی = menu_stuff->nr;
			وإلا
				چوٹی = atoi(strchr(string, '-') + 1);
		} وإلا إذا (!strcmp(string, "*")) {
			bottom = 1;
			چوٹی = menu_stuff->nr;
		} وإلا {
			bottom = لبھو_unique(string, menu_stuff);
			چوٹی = bottom;
		}

		إذا (چوٹی <= 0 || bottom <= 0 || چوٹی > menu_stuff->nr || bottom > چوٹی ||
		    (is_single && bottom != چوٹی)) {
			clean_print_color(CLEAN_COLOR_ERROR);
			لکھو(_("Huh (%s)?\n"), string);
			clean_print_color(CLEAN_COLOR_RESET);
			استمر;
		}

		لكل (i = bottom; i <= چوٹی; i++)
			(*chosen)[i-1] = چنو;
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
ساكن صحيح *list_and_چنو(بنية menu_opts *opts, بنية menu_stuff *stuff)
{
	بنية strbuf choice = STRBUF_INIT;
	صحيح *chosen, *نتیجہ;
	صحيح nr = 0;
	صحيح eof = 0;
	صحيح i;

	ALLOC_ARRAY(chosen, stuff->nr);
	/* set chosen as uninitialized */
	لكل (i = 0; i < stuff->nr; i++)
		chosen[i] = -1;

	لكل (;;) {
		إذا (opts->سرخی) {
			printf_ln("%s%s%s",
				  clean_get_color(CLEAN_COLOR_سرخی),
				  _(opts->سرخی),
				  clean_get_color(CLEAN_COLOR_RESET));
		}

		/* chosen will be initialized by print_highlight_menu_stuff */
		print_highlight_menu_stuff(stuff, &chosen);

		إذا (opts->flags & MENU_OPTS_LIST_ONLY)
			اكسر;

		إذا (opts->prompt) {
			لکھو("%s%s%s%s",
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
		نتیجہ = xmalloc(حجم(صحيح));
		*نتیجہ = EOF;
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

		CALLOC_ARRAY(نتیجہ, st_جوڑو(nr, 1));
		لكل (i = 0; i < stuff->nr && j < nr; i++) {
			إذا (chosen[i])
				نتیجہ[j++] = i;
		}
		نتیجہ[j] = EOF;
	}

	حرر(chosen);
	strbuf_release(&choice);
	أرجع نتیجہ;
}

ساكن صحيح clean_cmd(فراغ)
{
	أرجع MENU_RETURN_NO_LOOP;
}

ساكن صحيح filter_by_patterns_cmd(فراغ)
{
	بنية dir_struct dir = DIR_INIT;
	بنية strbuf confirm = STRBUF_INIT;
	بنية نمونہ_list *pl;
	صحيح بدلیا = -1, i;

	لكل (;;) {
		بنية string_list ignore_list = STRING_LIST_INIT_NODUP;
		بنية string_list_شے *شے;

		إذا (!del_list.nr)
			اكسر;

		إذا (بدلیا)
			pretty_print_dels();

		clean_print_color(CLEAN_COLOR_PROMPT);
		لکھو(_("Input ignore patterns>> "));
		clean_print_color(CLEAN_COLOR_RESET);
		إذا (git_read_سطر_interactively(&confirm) == EOF)
			حرف_لکھو('\n');

		/* quit filter_by_pattern mode if press ENTER or Ctrl-D */
		إذا (!confirm.len)
			اكسر;

		pl = جوڑو_نمونہ_list(&dir, EXC_CMDL, "manual exclude");

		string_list_ونڈو_وچ_رکھو_f(&ignore_list, confirm.buf, " ", -1,
					     STRING_LIST_ونڈو_TRIM);

		لكل (i = 0; i < ignore_list.nr; i++) {
			شے = &ignore_list.اشیاء[i];
			إذا (!*شے->string)
				استمر;
			جوڑو_نمونہ(شے->string, "", 0, pl, -(i+1));
		}

		بدلیا = 0;
		for_each_string_list_شے(شے, &del_list) {
			صحيح dtype = DT_UNKNOWN;

			إذا (is_excluded(&dir, the_repository->اشاریہ, شے->string, &dtype)) {
				*شے->string = '\0';
				بدلیا++;
			}
		}

		إذا (بدلیا) {
			string_list_remove_خالی_تھاواں_اشیاء(&del_list, 0);
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
	بنية string_list_شے *اشیاء;
	صحيح *chosen;
	صحيح i, j;

	menu_opts.سرخی = عدم;
	menu_opts.prompt = N_("Select items to delete");
	menu_opts.flags = 0;

	menu_stuff.type = MENU_STUFF_TYPE_STRING_LIST;
	menu_stuff.stuff = &del_list;
	menu_stuff.nr = del_list.nr;

	chosen = list_and_چنو(&menu_opts, &menu_stuff);
	اشیاء = del_list.اشیاء;
	لكل (i = 0, j = 0; i < del_list.nr; i++) {
		إذا (i < chosen[j]) {
			*(اشیاء[i].string) = '\0';
		} وإلا إذا (i == chosen[j]) {
			/* delete selected item */
			j++;
			استمر;
		} وإلا {
			/* end of chosen (chosen[j] == EOF), won't delete */
			*(اشیاء[i].string) = '\0';
		}
	}

	string_list_remove_خالی_تھاواں_اشیاء(&del_list, 0);

	حرر(chosen);
	أرجع 0;
}

ساكن صحيح ask_each_cmd(فراغ)
{
	بنية strbuf confirm = STRBUF_INIT;
	بنية strbuf buf = STRBUF_INIT;
	بنية string_list_شے *شے;
	ثابت حرف *qname;
	صحيح بدلیا = 0, eof = 0;

	for_each_string_list_شے(شے, &del_list) {
		/* Ctrl-D should stop removing files */
		إذا (!eof) {
			qname = quote_رستہ(شے->string, عدم, &buf, 0);
			/* TRANSLATORS: Make sure to keep [y/N] as is */
			لکھو(_("Remove %s [y/N]? "), qname);
			إذا (git_read_سطر_interactively(&confirm) == EOF) {
				حرف_لکھو('\n');
				eof = 1;
			}
		}
		إذا (!confirm.len || strncasecmp(confirm.buf, "yes", confirm.len)) {
			*شے->string = '\0';
			بدلیا++;
		}
	}

	إذا (بدلیا)
		string_list_remove_خالی_تھاواں_اشیاء(&del_list, 0);

	strbuf_release(&buf);
	strbuf_release(&confirm);
	أرجع MENU_RETURN_NO_LOOP;
}

ساكن صحيح quit_cmd(فراغ)
{
	string_list_clear(&del_list, 0);
	لکھو(_("Bye.\n"));
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
		بنية menu_شے menus[] = {
			{'c', "clean",			0, clean_cmd},
			{'f', "filter by pattern",	0, filter_by_patterns_cmd},
			{'s', "select by numbers",	0, select_by_numbers_cmd},
			{'a', "ask each",		0, ask_each_cmd},
			{'q', "quit",			0, quit_cmd},
			{'h', "help",			0, help_cmd},
		};
		صحيح *chosen;

		menu_opts.سرخی = N_("*** Commands ***");
		menu_opts.prompt = N_("What now");
		menu_opts.flags = MENU_OPTS_SINGLETON;

		menu_stuff.type = MENU_STUFF_TYPE_MENU_شے;
		menu_stuff.stuff = menus;
		menu_stuff.nr = حجم(menus) / حجم(بنية menu_شے);

		clean_print_color(CLEAN_COLOR_سرخی);
		printf_ln(Q_("Would remove the following item:",
			     "Would remove the following items:",
			     del_list.nr));
		clean_print_color(CLEAN_COLOR_RESET);

		pretty_print_dels();

		chosen = list_and_چنو(&menu_opts, &menu_stuff);

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
		       0 <= cmp_dir_اندراج(&dir->entries[src], &dir->ignored[ign]))
			ign++;

		إذا (ign < dir->ignored_nr &&
		    check_dir_اندراج_شامل_اے(dir->entries[src], dir->ignored[ign])) {
			/* entries[src] contains an ignored path, so we drop it */
			حرر(dir->entries[src]);
		} وإلا {
			بنية dir_اندراج *ent = dir->entries[src++];

			/* entries[src] does not contain an ignored path, so we keep it */
			dir->entries[dst++] = ent;

			/* then discard paths in entries[] contained inside entries[src] */
			طالما (src < dir->nr &&
			       check_dir_اندراج_شامل_اے(ent, dir->entries[src]))
				حرر(dir->entries[src++]);

			/* compensate for the outer loop's loop control */
			src--;
		}
	}
	dir->nr = dst;
}

صحيح cmd_clean(صحيح argc,
	      ثابت حرف **argv,
	      ثابت حرف *اگیتر,
	      بنية repository *repo بن_ورتیا)
{
	صحيح i, res;
	صحيح dry_لگاتار = 0, remove_directories = 0, quiet = 0, ignored = 0;
	صحيح ignored_only = 0, force = 0, errors = 0, gone = 1;
	صحيح rm_flags = REMOVE_DIR_KEEP_NESTED_GIT;
	بنية strbuf abs_رستہ = STRBUF_INIT;
	بنية dir_struct dir = DIR_INIT;
	بنية pathspec pathspec;
	بنية strbuf buf = STRBUF_INIT;
	بنية string_list exclude_list = STRING_LIST_INIT_NODUP;
	بنية نمونہ_list *pl;
	بنية string_list_شے *شے;
	ثابت حرف *qname;
	بنية option options[] = {
		OPT__QUIET(&quiet, N_("do not print names of files removed")),
		OPT_DRY_لگاتار(&dry_لگاتار, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('i', "interactive", &interactive, N_("interactive cleaning")),
		OPT_BOOL('d', عدم, &remove_directories,
				N_("remove whole directories")),
		OPT_CALLBACK_F('e', "exclude", &exclude_list, N_("pattern"),
		  N_("add <pattern> to ignore rules"), PARSE_OPT_NONEG, exclude_cb),
		OPT_BOOL('x', عدم, &ignored, N_("remove ignored files, too")),
		OPT_BOOL('X', عدم, &ignored_only,
				N_("remove only ignored files")),
		OPT_سرا()
	};

	repo_config(the_repository, git_clean_config, عدم);

	argc = parse_options(argc, argv, اگیتر, options, builtin_clean_usage,
			     0);

	إذا (require_force != 0 && !force && !interactive && !dry_لگاتار)
		die(_("clean.requireForce is true and -f not given: refusing to clean"));

	إذا (force > 1)
		rm_flags = 0;
	وإلا
		dir.flags |= DIR_SKIP_NESTED_GIT;

	dir.flags |= DIR_SHOW_OTHER_DIRECTORIES;

	إذا (ignored && ignored_only)
		die(_("options '%s' and '%s' cannot be used together"), "-x", "-X");
	إذا (!ignored)
		تیاری_standard_excludes(&dir);
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
	the_repository->settings.command_requires_بھریا_اشاریہ = 0;

	إذا (repo_read_اشاریہ(the_repository) < 0)
		die(_("index file corrupt"));

	pl = جوڑو_نمونہ_list(&dir, EXC_CMDL, "--exclude option");
	لكل (i = 0; i < exclude_list.nr; i++)
		جوڑو_نمونہ(exclude_list.اشیاء[i].string, "", 0, pl, -(i+1));

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       اگیتر, argv);

	fill_directory(&dir, the_repository->اشاریہ, &pathspec);
	correct_untracked_entries(&dir);

	لكل (i = 0; i < dir.nr; i++) {
		بنية dir_اندراج *ent = dir.entries[i];
		بنية stat st;
		ثابت حرف *rel;

		إذا (!اشاریہ_name_is_other(the_repository->اشاریہ, ent->name, ent->len))
			استمر;

		إذا (lstat(ent->name, &st))
			die_errno("Cannot lstat '%s'", ent->name);

		إذا (S_ISDIR(st.st_منوال) && !remove_directories)
			استمر;

		rel = relative_رستہ(ent->name, اگیتر, &buf);
		string_list_append(&del_list, rel);
	}

	dir_clear(&dir);

	إذا (interactive && del_list.nr > 0)
		interactive_main_loop();

	for_each_string_list_شے(شے, &del_list) {
		بنية stat st;

		strbuf_reset(&abs_رستہ);
		إذا (اگیتر)
			strbuf_addstr(&abs_رستہ, اگیتر);

		strbuf_addstr(&abs_رستہ, شے->string);

		/*
		 * we might have removed this as part of earlier
		 * recursive directory removal, so lstat() here could
		 * fail with ENOENT.
		 */
		إذا (lstat(abs_رستہ.buf, &st))
			استمر;

		إذا (S_ISDIR(st.st_منوال)) {
			إذا (remove_dirs(&abs_رستہ, اگیتر, rm_flags, dry_لگاتار, quiet, &gone))
				errors++;
			إذا (gone && !quiet) {
				qname = quote_رستہ(شے->string, عدم, &buf, 0);
				لکھو(dry_لگاتار ? _(پیغام_would_remove) : _(پیغام_remove), qname);
			}
		} وإلا {
			res = dry_لگاتار ? 0 : unlink(abs_رستہ.buf);
			إذا (res) {
				صحيح saved_errno = errno;
				qname = quote_رستہ(شے->string, عدم, &buf, 0);
				errno = saved_errno;
				warning_errno(_(پیغام_warn_remove_failed), qname);
				errors++;
			} وإلا إذا (!quiet) {
				qname = quote_رستہ(شے->string, عدم, &buf, 0);
				لکھو(dry_لگاتار ? _(پیغام_would_remove) : _(پیغام_remove), qname);
			}
		}
	}

	strbuf_release(&abs_رستہ);
	strbuf_release(&buf);
	string_list_clear(&del_list, 0);
	string_list_clear(&exclude_list, 0);
	clear_pathspec(&pathspec);
	أرجع (errors != 0);
}
