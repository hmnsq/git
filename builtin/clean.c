/*
 * "git clean" builtin command
 *
 * Copyright (C) 2007 Shawn Bohrer
 *
 * Based on git-clean.sh by Pavel Roskin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "abspath.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "dir.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "read-cache-ll.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "column.h"
#અન્તર્ભાવ "color.h"
#અન્તર્ભાવ "pathspec.h"
#અન્તર્ભાવ "help.h"
#અન્તર્ભાવ "prompt.h"

સ્થિર પૂર્ણાંક require_force = -1; /* unset */
સ્થિર પૂર્ણાંક interactive;
સ્થિર સંરચના string_list del_list = STRING_LIST_INIT_DUP;
સ્થિર unsigned પૂર્ણાંક colopts;

સ્થિર અચલ અક્ષર *અચલ builtin_clean_usage[] = {
	N_("git clean [-d] [-f] [-i] [-n] [-q] [-e <pattern>] [-x | -X] [--] [<pathspec>...]"),
	ખાલી
};

સ્થિર અચલ અક્ષર *સંદેશ_remove = N_("Removing %s\n");
સ્થિર અચલ અક્ષર *સંદેશ_would_remove = N_("Would remove %s\n");
સ્થિર અચલ અક્ષર *સંદેશ_skip_git_dir = N_("Skipping repository %s\n");
સ્થિર અચલ અક્ષર *સંદેશ_would_skip_git_dir = N_("Would skip repository %s\n");
સ્થિર અચલ અક્ષર *સંદેશ_warn_remove_failed = N_("failed to remove %s");
સ્થિર અચલ અક્ષર *સંદેશ_warn_lstat_failed = N_("could not lstat %s\n");
સ્થિર અચલ અક્ષર *સંદેશ_skip_cwd = N_("Refusing to remove current working directory\n");
સ્થિર અચલ અક્ષર *સંદેશ_would_skip_cwd = N_("Would refuse to remove current working directory\n");

ગણના color_clean {
	CLEAN_COLOR_RESET = 0,
	CLEAN_COLOR_PLAIN = 1,
	CLEAN_COLOR_PROMPT = 2,
	CLEAN_COLOR_HEADER = 3,
	CLEAN_COLOR_HELP = 4,
	CLEAN_COLOR_ERROR = 5
};

સ્થિર અચલ અક્ષર *color_interactive_slots[] = {
	[CLEAN_COLOR_ERROR]  = "error",
	[CLEAN_COLOR_HEADER] = "header",
	[CLEAN_COLOR_HELP]   = "help",
	[CLEAN_COLOR_PLAIN]  = "plain",
	[CLEAN_COLOR_PROMPT] = "prompt",
	[CLEAN_COLOR_RESET]  = "reset",
};

સ્થિર ગણના git_colorbool clean_use_color = GIT_COLOR_UNKNOWN;
સ્થિર અક્ષર clean_colors[][COLOR_MAXLEN] = {
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

સંરચના menu_opts {
	અચલ અક્ષર *header;
	અચલ અક્ષર *prompt;
	પૂર્ણાંક flags;
};

#define MENU_RETURN_NO_LOOP		10

સંરચના menu_item {
	અક્ષર hotkey;
	અચલ અક્ષર *title;
	પૂર્ણાંક selected;
	પૂર્ણાંક (*fn)(શૂન્ય);
};

ગણના menu_stuff_type {
	MENU_STUFF_TYPE_STRING_LIST = 1,
	MENU_STUFF_TYPE_MENU_ITEM
};

સંરચના menu_stuff {
	ગણના menu_stuff_type type;
	પૂર્ણાંક nr;
	શૂન્ય *stuff;
};

define_list_config_array(color_interactive_slots);

સ્થિર પૂર્ણાંક git_clean_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
			    અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	અચલ અક્ષર *slot_name;

	જો (starts_with(var, "column."))
		પાછા git_column_config(var, કિંમત, "clean", &colopts);

	/* honors the color.interactive* config variables which also
	   applied in git-add--interactive and git-stash */
	જો (!strcmp(var, "color.interactive")) {
		clean_use_color = git_config_colorbool(var, કિંમત);
		પાછા 0;
	}
	જો (skip_prefix(var, "color.interactive.", &slot_name)) {
		પૂર્ણાંક slot = LOOKUP_CONFIG(color_interactive_slots, slot_name);
		જો (slot < 0)
			પાછા 0;
		જો (!કિંમત)
			પાછા config_error_nonbool(var);
		પાછા color_parse(કિંમત, clean_colors[slot]);
	}

	જો (!strcmp(var, "clean.requireforce")) {
		require_force = git_config_bool(var, કિંમત);
		પાછા 0;
	}

	જો (git_color_config(var, કિંમત, cb) < 0)
		પાછા -1;

	પાછા git_default_config(var, કિંમત, ctx, cb);
}

સ્થિર અચલ અક્ષર *clean_get_color(ગણના color_clean ix)
{
	જો (want_color(clean_use_color))
		પાછા clean_colors[ix];
	પાછા "";
}

સ્થિર શૂન્ય clean_print_color(ગણના color_clean ix)
{
	printf("%s", clean_get_color(ix));
}

સ્થિર પૂર્ણાંક exclude_cb(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના string_list *exclude_list = opt->કિંમત;
	BUG_ON_OPT_NEG(unset);
	string_list_append(exclude_list, arg);
	પાછા 0;
}

સ્થિર પૂર્ણાંક remove_dirs(સંરચના strbuf *path, અચલ અક્ષર *prefix, પૂર્ણાંક force_flag,
		પૂર્ણાંક dry_run, પૂર્ણાંક quiet, પૂર્ણાંક *dir_gone)
{
	DIR *dir;
	સંરચના strbuf quoted = STRBUF_INIT;
	સંરચના strbuf realpath = STRBUF_INIT;
	સંરચના strbuf real_ocwd = STRBUF_INIT;
	સંરચના dirent *e;
	પૂર્ણાંક res = 0, ret = 0, gone = 1, original_len = path->len, len;
	સંરચના string_list dels = STRING_LIST_INIT_DUP;

	*dir_gone = 1;

	જો ((force_flag & REMOVE_DIR_KEEP_NESTED_GIT) &&
	    is_nonbare_repository_dir(path)) {
		જો (!quiet) {
			quote_path(path->buf, prefix, &quoted, 0);
			printf(dry_run ?  _(સંદેશ_would_skip_git_dir) : _(સંદેશ_skip_git_dir),
					quoted.buf);
		}

		*dir_gone = 0;
		જાઓ out;
	}

	dir = opendir(path->buf);
	જો (!dir) {
		/* an empty dir could be removed even if it is unreadble */
		res = dry_run ? 0 : rmdir(path->buf);
		જો (res) {
			પૂર્ણાંક saved_errno = errno;
			quote_path(path->buf, prefix, &quoted, 0);
			errno = saved_errno;
			warning_errno(_(સંદેશ_warn_remove_failed), quoted.buf);
			*dir_gone = 0;
		}
		ret = res;
		જાઓ out;
	}

	strbuf_complete(path, '/');

	len = path->len;
	જ્યારે ((e = readdir_skip_dot_and_dotdot(dir)) != ખાલી) {
		સંરચના stat st;

		strbuf_setlen(path, len);
		strbuf_addstr(path, e->d_name);
		જો (lstat(path->buf, &st))
			warning_errno(_(સંદેશ_warn_lstat_failed), path->buf);
		નહીંતર જો (S_ISDIR(st.st_mode)) {
			જો (remove_dirs(path, prefix, force_flag, dry_run, quiet, &gone))
				ret = 1;
			જો (gone) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} નહીંતર
				*dir_gone = 0;
			ચાલુ;
		} નહીંતર {
			res = dry_run ? 0 : unlink(path->buf);
			જો (!res) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} નહીંતર {
				પૂર્ણાંક saved_errno = errno;
				quote_path(path->buf, prefix, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(સંદેશ_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
			ચાલુ;
		}

		/* path too long, stat fails, or non-directory still exists */
		*dir_gone = 0;
		ret = 1;
		તોડો;
	}
	closedir(dir);

	strbuf_setlen(path, original_len);

	જો (*dir_gone) {
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
		 જો (startup_info->original_cwd)
			 strbuf_realpath(&real_ocwd,
					 startup_info->original_cwd, 1);

		જો (!strbuf_cmp(&realpath, &real_ocwd)) {
			printf("%s", dry_run ? _(સંદેશ_would_skip_cwd) : _(સંદેશ_skip_cwd));
			*dir_gone = 0;
		} નહીંતર {
			res = dry_run ? 0 : rmdir(path->buf);
			જો (!res)
				*dir_gone = 1;
			નહીંતર {
				પૂર્ણાંક saved_errno = errno;
				quote_path(path->buf, prefix, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(સંદેશ_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
		}
	}

	જો (!*dir_gone && !quiet) {
		પૂર્ણાંક i;
		માટે (i = 0; i < dels.nr; i++)
			printf(dry_run ?  _(સંદેશ_would_remove) : _(સંદેશ_remove), dels.વસ્તુઓ[i].string);
	}
out:
	strbuf_release(&realpath);
	strbuf_release(&real_ocwd);
	strbuf_release(&quoted);
	string_list_clear(&dels, 0);
	પાછા ret;
}

સ્થિર શૂન્ય pretty_print_dels(શૂન્ય)
{
	સંરચના string_list list = STRING_LIST_INIT_DUP;
	સંરચના string_list_item *item;
	સંરચના strbuf buf = STRBUF_INIT;
	અચલ અક્ષર *qname;
	સંરચના column_options copts;

	for_each_string_list_item(item, &del_list) {
		qname = quote_path(item->string, ખાલી, &buf, 0);
		string_list_append(&list, qname);
	}

	/*
	 * always enable column display, we only consult column.*
	 * about layout strategy and stuff
	 */
	colopts = (colopts & ~COL_ENABLE_MASK) | COL_ENABLED;
	memset(&copts, 0, કદ(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(&list, colopts, &copts);
	strbuf_release(&buf);
	string_list_clear(&list, 0);
}

સ્થિર શૂન્ય pretty_print_menus(સંરચના string_list *menu_list)
{
	unsigned પૂર્ણાંક local_colopts = 0;
	સંરચના column_options copts;

	local_colopts = COL_ENABLED | COL_ROW;
	memset(&copts, 0, કદ(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(menu_list, local_colopts, &copts);
}

સ્થિર શૂન્ય prompt_help_cmd(પૂર્ણાંક singleton)
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
સ્થિર શૂન્ય print_highlight_menu_stuff(સંરચના menu_stuff *stuff, પૂર્ણાંક **chosen)
{
	સંરચના string_list menu_list = STRING_LIST_INIT_DUP;
	સંરચના strbuf menu = STRBUF_INIT;
	સંરચના menu_item *menu_item;
	સંરચના string_list_item *string_list_item;
	પૂર્ણાંક i;

	પસંદગી (stuff->type) {
	મૂળભૂત:
		die("Bad type of menu_stuff when print menu");
	સ્થિતિ MENU_STUFF_TYPE_MENU_ITEM:
		menu_item = (સંરચના menu_item *)stuff->stuff;
		માટે (i = 0; i < stuff->nr; i++, menu_item++) {
			અચલ અક્ષર *p;
			પૂર્ણાંક highlighted = 0;

			p = menu_item->title;
			જો ((*chosen)[i] < 0)
				(*chosen)[i] = menu_item->selected ? 1 : 0;
			strbuf_addf(&menu, "%s%2d: ", (*chosen)[i] ? "*" : " ", i+1);
			માટે (; *p; p++) {
				જો (!highlighted && *p == menu_item->hotkey) {
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_PROMPT));
					strbuf_addch(&menu, *p);
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_RESET));
					highlighted = 1;
				} નહીંતર {
					strbuf_addch(&menu, *p);
				}
			}
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
		}
		તોડો;
	સ્થિતિ MENU_STUFF_TYPE_STRING_LIST:
		i = 0;
		for_each_string_list_item(string_list_item, (સંરચના string_list *)stuff->stuff) {
			જો ((*chosen)[i] < 0)
				(*chosen)[i] = 0;
			strbuf_addf(&menu, "%s%2d: %s",
				    (*chosen)[i] ? "*" : " ", i+1, string_list_item->string);
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
			i++;
		}
		તોડો;
	}

	pretty_print_menus(&menu_list);

	strbuf_release(&menu);
	string_list_clear(&menu_list, 0);
}

સ્થિર પૂર્ણાંક find_unique(અચલ અક્ષર *choice, સંરચના menu_stuff *menu_stuff)
{
	સંરચના menu_item *menu_item;
	સંરચના string_list_item *string_list_item;
	પૂર્ણાંક i, len, found = 0;

	len = strlen(choice);
	પસંદગી (menu_stuff->type) {
	મૂળભૂત:
		die("Bad type of menu_stuff when parse choice");
	સ્થિતિ MENU_STUFF_TYPE_MENU_ITEM:

		menu_item = (સંરચના menu_item *)menu_stuff->stuff;
		માટે (i = 0; i < menu_stuff->nr; i++, menu_item++) {
			જો (len == 1 && *choice == menu_item->hotkey) {
				found = i + 1;
				તોડો;
			}
			જો (!strncasecmp(choice, menu_item->title, len)) {
				જો (found) {
					જો (len == 1) {
						/* continue for hotkey matching */
						found = -1;
					} નહીંતર {
						found = 0;
						તોડો;
					}
				} નહીંતર {
					found = i + 1;
				}
			}
		}
		તોડો;
	સ્થિતિ MENU_STUFF_TYPE_STRING_LIST:
		string_list_item = ((સંરચના string_list *)menu_stuff->stuff)->વસ્તુઓ;
		માટે (i = 0; i < menu_stuff->nr; i++, string_list_item++) {
			જો (!strncasecmp(choice, string_list_item->string, len)) {
				જો (found) {
					found = 0;
					તોડો;
				}
				found = i + 1;
			}
		}
		તોડો;
	}
	પાછા found;
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
સ્થિર પૂર્ણાંક parse_choice(સંરચના menu_stuff *menu_stuff,
			પૂર્ણાંક is_single,
			અક્ષર *આગત,
			પૂર્ણાંક **chosen)
{
	સંરચના string_list choice = STRING_LIST_INIT_NODUP;
	સંરચના string_list_item *item;
	પૂર્ણાંક nr = 0;
	પૂર્ણાંક i;

	string_list_split_in_place_f(&choice, આગત,
				     is_single ? "\n" : ", ", -1,
				     STRING_LIST_SPLIT_TRIM);

	for_each_string_list_item(item, &choice) {
		અચલ અક્ષર *string;
		પૂર્ણાંક choose;
		પૂર્ણાંક bottom = 0, ટોચ = 0;
		પૂર્ણાંક is_range, is_સંખ્યા;

		string = item->string;
		જો (!*string)
			ચાલુ;

		/* Input that begins with '-'; unchoose */
		જો (string[0] == '-') {
			choose = 0;
			string++;
		} નહીંતર {
			choose = 1;
		}

		is_range = 0;
		is_સંખ્યા = 1;
		માટે (અચલ અક્ષર *p = string; *p; p++) {
			જો ('-' == *p) {
				જો (!is_range) {
					is_range = 1;
					is_સંખ્યા = 0;
				} નહીંતર {
					is_સંખ્યા = 0;
					is_range = 0;
					તોડો;
				}
			} નહીંતર જો (!isdigit(*p)) {
				is_સંખ્યા = 0;
				is_range = 0;
				તોડો;
			}
		}

		જો (is_સંખ્યા) {
			bottom = atoi(string);
			ટોચ = bottom;
		} નહીંતર જો (is_range) {
			bottom = atoi(string);
			/* a range can be specified like 5-7 or 5- */
			જો (!*(strchr(string, '-') + 1))
				ટોચ = menu_stuff->nr;
			નહીંતર
				ટોચ = atoi(strchr(string, '-') + 1);
		} નહીંતર જો (!strcmp(string, "*")) {
			bottom = 1;
			ટોચ = menu_stuff->nr;
		} નહીંતર {
			bottom = find_unique(string, menu_stuff);
			ટોચ = bottom;
		}

		જો (ટોચ <= 0 || bottom <= 0 || ટોચ > menu_stuff->nr || bottom > ટોચ ||
		    (is_single && bottom != ટોચ)) {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf(_("Huh (%s)?\n"), string);
			clean_print_color(CLEAN_COLOR_RESET);
			ચાલુ;
		}

		માટે (i = bottom; i <= ટોચ; i++)
			(*chosen)[i-1] = choose;
	}

	string_list_clear(&choice, 0);

	માટે (i = 0; i < menu_stuff->nr; i++)
		nr += (*chosen)[i];
	પાછા nr;
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
સ્થિર પૂર્ણાંક *list_and_choose(સંરચના menu_opts *opts, સંરચના menu_stuff *stuff)
{
	સંરચના strbuf choice = STRBUF_INIT;
	પૂર્ણાંક *chosen, *result;
	પૂર્ણાંક nr = 0;
	પૂર્ણાંક eof = 0;
	પૂર્ણાંક i;

	ALLOC_ARRAY(chosen, stuff->nr);
	/* set chosen as uninitialized */
	માટે (i = 0; i < stuff->nr; i++)
		chosen[i] = -1;

	માટે (;;) {
		જો (opts->header) {
			printf_ln("%s%s%s",
				  clean_get_color(CLEAN_COLOR_HEADER),
				  _(opts->header),
				  clean_get_color(CLEAN_COLOR_RESET));
		}

		/* chosen will be initialized by print_highlight_menu_stuff */
		print_highlight_menu_stuff(stuff, &chosen);

		જો (opts->flags & MENU_OPTS_LIST_ONLY)
			તોડો;

		જો (opts->prompt) {
			printf("%s%s%s%s",
			       clean_get_color(CLEAN_COLOR_PROMPT),
			       _(opts->prompt),
			       opts->flags & MENU_OPTS_SINGLETON ? "> " : ">> ",
			       clean_get_color(CLEAN_COLOR_RESET));
		}

		જો (git_read_લીટી_interactively(&choice) == EOF) {
			eof = 1;
			તોડો;
		}

		/* help for prompt */
		જો (!strcmp(choice.buf, "?")) {
			prompt_help_cmd(opts->flags & MENU_OPTS_SINGLETON);
			ચાલુ;
		}

		/* for a multiple-choice menu, press ENTER (empty) will return back */
		જો (!(opts->flags & MENU_OPTS_SINGLETON) && !choice.len)
			તોડો;

		nr = parse_choice(stuff,
				  opts->flags & MENU_OPTS_SINGLETON,
				  choice.buf,
				  &chosen);

		જો (opts->flags & MENU_OPTS_SINGLETON) {
			જો (nr)
				તોડો;
		} નહીંતર જો (opts->flags & MENU_OPTS_IMMEDIATE) {
			તોડો;
		}
	}

	જો (eof) {
		result = xmalloc(કદ(પૂર્ણાંક));
		*result = EOF;
	} નહીંતર {
		પૂર્ણાંક j = 0;

		/*
		 * recalculate nr, if return back from menu directly with
		 * default selections.
		 */
		જો (!nr) {
			માટે (i = 0; i < stuff->nr; i++)
				nr += chosen[i];
		}

		CALLOC_ARRAY(result, st_add(nr, 1));
		માટે (i = 0; i < stuff->nr && j < nr; i++) {
			જો (chosen[i])
				result[j++] = i;
		}
		result[j] = EOF;
	}

	મુક્ત(chosen);
	strbuf_release(&choice);
	પાછા result;
}

સ્થિર પૂર્ણાંક clean_cmd(શૂન્ય)
{
	પાછા MENU_RETURN_NO_LOOP;
}

સ્થિર પૂર્ણાંક filter_by_patterns_cmd(શૂન્ય)
{
	સંરચના dir_struct dir = DIR_INIT;
	સંરચના strbuf confirm = STRBUF_INIT;
	સંરચના pattern_list *pl;
	પૂર્ણાંક changed = -1, i;

	માટે (;;) {
		સંરચના string_list ignore_list = STRING_LIST_INIT_NODUP;
		સંરચના string_list_item *item;

		જો (!del_list.nr)
			તોડો;

		જો (changed)
			pretty_print_dels();

		clean_print_color(CLEAN_COLOR_PROMPT);
		printf(_("Input ignore patterns>> "));
		clean_print_color(CLEAN_COLOR_RESET);
		જો (git_read_લીટી_interactively(&confirm) == EOF)
			putchar('\n');

		/* quit filter_by_pattern mode if press ENTER or Ctrl-D */
		જો (!confirm.len)
			તોડો;

		pl = add_pattern_list(&dir, EXC_CMDL, "manual exclude");

		string_list_split_in_place_f(&ignore_list, confirm.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);

		માટે (i = 0; i < ignore_list.nr; i++) {
			item = &ignore_list.વસ્તુઓ[i];
			જો (!*item->string)
				ચાલુ;
			add_pattern(item->string, "", 0, pl, -(i+1));
		}

		changed = 0;
		for_each_string_list_item(item, &del_list) {
			પૂર્ણાંક dtype = DT_UNKNOWN;

			જો (is_excluded(&dir, the_repository->index, item->string, &dtype)) {
				*item->string = '\0';
				changed++;
			}
		}

		જો (changed) {
			string_list_remove_empty_વસ્તુઓ(&del_list, 0);
		} નહીંતર {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf_ln(_("WARNING: Cannot find items matched by: %s"), confirm.buf);
			clean_print_color(CLEAN_COLOR_RESET);
		}

		string_list_clear(&ignore_list, 0);
		dir_clear(&dir);
	}

	strbuf_release(&confirm);
	પાછા 0;
}

સ્થિર પૂર્ણાંક select_by_numbers_cmd(શૂન્ય)
{
	સંરચના menu_opts menu_opts;
	સંરચના menu_stuff menu_stuff;
	સંરચના string_list_item *વસ્તુઓ;
	પૂર્ણાંક *chosen;
	પૂર્ણાંક i, j;

	menu_opts.header = ખાલી;
	menu_opts.prompt = N_("Select items to delete");
	menu_opts.flags = 0;

	menu_stuff.type = MENU_STUFF_TYPE_STRING_LIST;
	menu_stuff.stuff = &del_list;
	menu_stuff.nr = del_list.nr;

	chosen = list_and_choose(&menu_opts, &menu_stuff);
	વસ્તુઓ = del_list.વસ્તુઓ;
	માટે (i = 0, j = 0; i < del_list.nr; i++) {
		જો (i < chosen[j]) {
			*(વસ્તુઓ[i].string) = '\0';
		} નહીંતર જો (i == chosen[j]) {
			/* delete selected item */
			j++;
			ચાલુ;
		} નહીંતર {
			/* end of chosen (chosen[j] == EOF), won't delete */
			*(વસ્તુઓ[i].string) = '\0';
		}
	}

	string_list_remove_empty_વસ્તુઓ(&del_list, 0);

	મુક્ત(chosen);
	પાછા 0;
}

સ્થિર પૂર્ણાંક ask_each_cmd(શૂન્ય)
{
	સંરચના strbuf confirm = STRBUF_INIT;
	સંરચના strbuf buf = STRBUF_INIT;
	સંરચના string_list_item *item;
	અચલ અક્ષર *qname;
	પૂર્ણાંક changed = 0, eof = 0;

	for_each_string_list_item(item, &del_list) {
		/* Ctrl-D should stop removing files */
		જો (!eof) {
			qname = quote_path(item->string, ખાલી, &buf, 0);
			/* TRANSLATORS: Make sure to keep [y/N] as is */
			printf(_("Remove %s [y/N]? "), qname);
			જો (git_read_લીટી_interactively(&confirm) == EOF) {
				putchar('\n');
				eof = 1;
			}
		}
		જો (!confirm.len || strncasecmp(confirm.buf, "yes", confirm.len)) {
			*item->string = '\0';
			changed++;
		}
	}

	જો (changed)
		string_list_remove_empty_વસ્તુઓ(&del_list, 0);

	strbuf_release(&buf);
	strbuf_release(&confirm);
	પાછા MENU_RETURN_NO_LOOP;
}

સ્થિર પૂર્ણાંક quit_cmd(શૂન્ય)
{
	string_list_clear(&del_list, 0);
	printf(_("Bye.\n"));
	પાછા MENU_RETURN_NO_LOOP;
}

સ્થિર પૂર્ણાંક help_cmd(શૂન્ય)
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
	પાછા 0;
}

સ્થિર શૂન્ય interactive_main_loop(શૂન્ય)
{
	જ્યારે (del_list.nr) {
		સંરચના menu_opts menu_opts;
		સંરચના menu_stuff menu_stuff;
		સંરચના menu_item menus[] = {
			{'c', "clean",			0, clean_cmd},
			{'f', "filter by pattern",	0, filter_by_patterns_cmd},
			{'s', "select by numbers",	0, select_by_numbers_cmd},
			{'a', "ask each",		0, ask_each_cmd},
			{'q', "quit",			0, quit_cmd},
			{'h', "help",			0, help_cmd},
		};
		પૂર્ણાંક *chosen;

		menu_opts.header = N_("*** Commands ***");
		menu_opts.prompt = N_("What now");
		menu_opts.flags = MENU_OPTS_SINGLETON;

		menu_stuff.type = MENU_STUFF_TYPE_MENU_ITEM;
		menu_stuff.stuff = menus;
		menu_stuff.nr = કદ(menus) / કદ(સંરચના menu_item);

		clean_print_color(CLEAN_COLOR_HEADER);
		printf_ln(Q_("Would remove the following item:",
			     "Would remove the following items:",
			     del_list.nr));
		clean_print_color(CLEAN_COLOR_RESET);

		pretty_print_dels();

		chosen = list_and_choose(&menu_opts, &menu_stuff);

		જો (*chosen != EOF) {
			પૂર્ણાંક ret;
			ret = menus[*chosen].fn();
			જો (ret != MENU_RETURN_NO_LOOP) {
				FREE_AND_NULL(chosen);
				જો (!del_list.nr) {
					clean_print_color(CLEAN_COLOR_ERROR);
					printf_ln(_("No more files to clean, exiting."));
					clean_print_color(CLEAN_COLOR_RESET);
					તોડો;
				}
				ચાલુ;
			}
		} નહીંતર {
			quit_cmd();
		}

		FREE_AND_NULL(chosen);
		તોડો;
	}
}

સ્થિર શૂન્ય correct_untracked_entries(સંરચના dir_struct *dir)
{
	પૂર્ણાંક src, dst, ign;

	માટે (src = dst = ign = 0; src < dir->nr; src++) {
		/* skip paths in ignored[] that cannot be inside entries[src] */
		જ્યારે (ign < dir->ignored_nr &&
		       0 <= cmp_dir_entry(&dir->entries[src], &dir->ignored[ign]))
			ign++;

		જો (ign < dir->ignored_nr &&
		    check_dir_entry_contains(dir->entries[src], dir->ignored[ign])) {
			/* entries[src] contains an ignored path, so we drop it */
			મુક્ત(dir->entries[src]);
		} નહીંતર {
			સંરચના dir_entry *ent = dir->entries[src++];

			/* entries[src] does not contain an ignored path, so we keep it */
			dir->entries[dst++] = ent;

			/* then discard paths in entries[] contained inside entries[src] */
			જ્યારે (src < dir->nr &&
			       check_dir_entry_contains(ent, dir->entries[src]))
				મુક્ત(dir->entries[src++]);

			/* compensate for the outer loop's loop control */
			src--;
		}
	}
	dir->nr = dst;
}

પૂર્ણાંક cmd_clean(પૂર્ણાંક argc,
	      અચલ અક્ષર **argv,
	      અચલ અક્ષર *prefix,
	      સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક i, res;
	પૂર્ણાંક dry_run = 0, remove_directories = 0, quiet = 0, ignored = 0;
	પૂર્ણાંક ignored_only = 0, force = 0, errors = 0, gone = 1;
	પૂર્ણાંક rm_flags = REMOVE_DIR_KEEP_NESTED_GIT;
	સંરચના strbuf abs_path = STRBUF_INIT;
	સંરચના dir_struct dir = DIR_INIT;
	સંરચના pathspec pathspec;
	સંરચના strbuf buf = STRBUF_INIT;
	સંરચના string_list exclude_list = STRING_LIST_INIT_NODUP;
	સંરચના pattern_list *pl;
	સંરચના string_list_item *item;
	અચલ અક્ષર *qname;
	સંરચના option options[] = {
		OPT__QUIET(&quiet, N_("do not print names of files removed")),
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('i', "interactive", &interactive, N_("interactive cleaning")),
		OPT_BOOL('d', ખાલી, &remove_directories,
				N_("remove whole directories")),
		OPT_CALLBACK_F('e', "exclude", &exclude_list, N_("pattern"),
		  N_("add <pattern> to ignore rules"), PARSE_OPT_NONEG, exclude_cb),
		OPT_BOOL('x', ખાલી, &ignored, N_("remove ignored files, too")),
		OPT_BOOL('X', ખાલી, &ignored_only,
				N_("remove only ignored files")),
		OPT_END()
	};

	repo_config(the_repository, git_clean_config, ખાલી);

	argc = parse_options(argc, argv, prefix, options, builtin_clean_usage,
			     0);

	જો (require_force != 0 && !force && !interactive && !dry_run)
		die(_("clean.requireForce is true and -f not given: refusing to clean"));

	જો (force > 1)
		rm_flags = 0;
	નહીંતર
		dir.flags |= DIR_SKIP_NESTED_GIT;

	dir.flags |= DIR_SHOW_OTHER_DIRECTORIES;

	જો (ignored && ignored_only)
		die(_("options '%s' and '%s' cannot be used together"), "-x", "-X");
	જો (!ignored)
		setup_standard_excludes(&dir);
	જો (ignored_only)
		dir.flags |= DIR_SHOW_IGNORED;

	જો (argc) {
		/*
		 * Remaining args implies pathspecs specified, and we should
		 * recurse within those.
		 */
		remove_directories = 1;
	}

	જો (remove_directories && !ignored_only) {
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
		જો (!ignored)
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

	જો (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	માટે (i = 0; i < exclude_list.nr; i++)
		add_pattern(exclude_list.વસ્તુઓ[i].string, "", 0, pl, -(i+1));

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	fill_directory(&dir, the_repository->index, &pathspec);
	correct_untracked_entries(&dir);

	માટે (i = 0; i < dir.nr; i++) {
		સંરચના dir_entry *ent = dir.entries[i];
		સંરચના stat st;
		અચલ અક્ષર *rel;

		જો (!index_name_is_other(the_repository->index, ent->name, ent->len))
			ચાલુ;

		જો (lstat(ent->name, &st))
			die_errno("Cannot lstat '%s'", ent->name);

		જો (S_ISDIR(st.st_mode) && !remove_directories)
			ચાલુ;

		rel = relative_path(ent->name, prefix, &buf);
		string_list_append(&del_list, rel);
	}

	dir_clear(&dir);

	જો (interactive && del_list.nr > 0)
		interactive_main_loop();

	for_each_string_list_item(item, &del_list) {
		સંરચના stat st;

		strbuf_reset(&abs_path);
		જો (prefix)
			strbuf_addstr(&abs_path, prefix);

		strbuf_addstr(&abs_path, item->string);

		/*
		 * we might have removed this as part of earlier
		 * recursive directory removal, so lstat() here could
		 * fail with ENOENT.
		 */
		જો (lstat(abs_path.buf, &st))
			ચાલુ;

		જો (S_ISDIR(st.st_mode)) {
			જો (remove_dirs(&abs_path, prefix, rm_flags, dry_run, quiet, &gone))
				errors++;
			જો (gone && !quiet) {
				qname = quote_path(item->string, ખાલી, &buf, 0);
				printf(dry_run ? _(સંદેશ_would_remove) : _(સંદેશ_remove), qname);
			}
		} નહીંતર {
			res = dry_run ? 0 : unlink(abs_path.buf);
			જો (res) {
				પૂર્ણાંક saved_errno = errno;
				qname = quote_path(item->string, ખાલી, &buf, 0);
				errno = saved_errno;
				warning_errno(_(સંદેશ_warn_remove_failed), qname);
				errors++;
			} નહીંતર જો (!quiet) {
				qname = quote_path(item->string, ખાલી, &buf, 0);
				printf(dry_run ? _(સંદેશ_would_remove) : _(સંદેશ_remove), qname);
			}
		}
	}

	strbuf_release(&abs_path);
	strbuf_release(&buf);
	string_list_clear(&del_list, 0);
	string_list_clear(&exclude_list, 0);
	clear_pathspec(&pathspec);
	પાછા (errors != 0);
}
