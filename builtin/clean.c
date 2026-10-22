/*
 * "git clean" builtin command
 *
 * Copyright (C) 2007 Shawn Bohrer
 *
 * Based on git-clean.sh by Pavel Roskin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "abspath.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "read-cache-ll.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "column.h"
#ଅନ୍ତର୍ଭାଵ "color.h"
#ଅନ୍ତର୍ଭାଵ "pathspec.h"
#ଅନ୍ତର୍ଭାଵ "help.h"
#ଅନ୍ତର୍ଭାଵ "prompt.h"

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା require_force = -1; /* unset */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା interactive;
ସ୍ଥିର ସଂରଚନା string_list del_list = STRING_LIST_INIT_DUP;
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା colopts;

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ଅଚଳ builtin_clean_usage[] = {
	N_("git clean [-d] [-f] [-i] [-n] [-q] [-e <pattern>] [-x | -X] [--] [<pathspec>...]"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ_remove = N_("Removing %s\n");
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ_would_remove = N_("Would remove %s\n");
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ_skip_git_dir = N_("Skipping repository %s\n");
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ_would_skip_git_dir = N_("Would skip repository %s\n");
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ_warn_remove_failed = N_("failed to remove %s");
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ_warn_lstat_failed = N_("could not lstat %s\n");
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ_skip_cwd = N_("Refusing to remove current working directory\n");
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ_would_skip_cwd = N_("Would refuse to remove current working directory\n");

ଗଣନା color_clean {
	CLEAN_COLOR_RESET = 0,
	CLEAN_COLOR_PLAIN = 1,
	CLEAN_COLOR_PROMPT = 2,
	CLEAN_COLOR_HEADER = 3,
	CLEAN_COLOR_HELP = 4,
	CLEAN_COLOR_ERROR = 5
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *color_interactive_slots[] = {
	[CLEAN_COLOR_ERROR]  = "error",
	[CLEAN_COLOR_HEADER] = "header",
	[CLEAN_COLOR_HELP]   = "help",
	[CLEAN_COLOR_PLAIN]  = "plain",
	[CLEAN_COLOR_PROMPT] = "prompt",
	[CLEAN_COLOR_RESET]  = "reset",
};

ସ୍ଥିର ଗଣନା git_colorbool clean_use_color = GIT_COLOR_UNKNOWN;
ସ୍ଥିର ଅକ୍ଷର clean_colors[][COLOR_MAXLEN] = {
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

ସଂରଚନା menu_opts {
	ଅଚଳ ଅକ୍ଷର *header;
	ଅଚଳ ଅକ୍ଷର *prompt;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags;
};

#define MENU_RETURN_NO_LOOP		10

ସଂରଚନା menu_item {
	ଅକ୍ଷର hotkey;
	ଅଚଳ ଅକ୍ଷର *title;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା selected;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*fn)(ଶୂନ୍ୟ);
};

ଗଣନା menu_stuff_type {
	MENU_STUFF_TYPE_STRING_LIST = 1,
	MENU_STUFF_TYPE_MENU_ITEM
};

ସଂରଚନା menu_stuff {
	ଗଣନା menu_stuff_type type;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr;
	ଶୂନ୍ୟ *stuff;
};

define_list_config_array(color_interactive_slots);

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_clean_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
			    ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ଅଚଳ ଅକ୍ଷର *slot_name;

	ଯଦି (starts_with(var, "column."))
		ଫେରନ୍ତୁ git_column_config(var, ମୂଲ୍ୟ, "clean", &colopts);

	/* honors the color.interactive* config variables which also
	   applied in git-add--interactive and git-stash */
	ଯଦି (!strcmp(var, "color.interactive")) {
		clean_use_color = git_config_colorbool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (skip_prefix(var, "color.interactive.", &slot_name)) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା slot = LOOKUP_CONFIG(color_interactive_slots, slot_name);
		ଯଦି (slot < 0)
			ଫେରନ୍ତୁ 0;
		ଯଦି (!ମୂଲ୍ୟ)
			ଫେରନ୍ତୁ config_error_nonbool(var);
		ଫେରନ୍ତୁ color_parse(ମୂଲ୍ୟ, clean_colors[slot]);
	}

	ଯଦି (!strcmp(var, "clean.requireforce")) {
		require_force = git_config_bool(var, ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (git_color_config(var, ମୂଲ୍ୟ, cb) < 0)
		ଫେରନ୍ତୁ -1;

	ଫେରନ୍ତୁ git_default_config(var, ମୂଲ୍ୟ, ctx, cb);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *clean_get_color(ଗଣନା color_clean ix)
{
	ଯଦି (want_color(clean_use_color))
		ଫେରନ୍ତୁ clean_colors[ix];
	ଫେରନ୍ତୁ "";
}

ସ୍ଥିର ଶୂନ୍ୟ clean_print_color(ଗଣନା color_clean ix)
{
	printf("%s", clean_get_color(ix));
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା exclude_cb(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା string_list *exclude_list = opt->ମୂଲ୍ୟ;
	BUG_ON_OPT_NEG(unset);
	string_list_append(exclude_list, arg);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା remove_dirs(ସଂରଚନା strbuf *path, ଅଚଳ ଅକ୍ଷର *prefix, ପୂର୍ଣ୍ଣସଂଖ୍ୟା force_flag,
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା dry_run, ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *dir_gone)
{
	DIR *dir;
	ସଂରଚନା strbuf quoted = STRBUF_INIT;
	ସଂରଚନା strbuf realpath = STRBUF_INIT;
	ସଂରଚନା strbuf real_ocwd = STRBUF_INIT;
	ସଂରଚନା dirent *e;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = 0, ret = 0, gone = 1, original_len = path->len, len;
	ସଂରଚନା string_list dels = STRING_LIST_INIT_DUP;

	*dir_gone = 1;

	ଯଦି ((force_flag & REMOVE_DIR_KEEP_NESTED_GIT) &&
	    is_nonbare_repository_dir(path)) {
		ଯଦି (!quiet) {
			quote_path(path->buf, prefix, &quoted, 0);
			printf(dry_run ?  _(ସନ୍ଦେଶ_would_skip_git_dir) : _(ସନ୍ଦେଶ_skip_git_dir),
					quoted.buf);
		}

		*dir_gone = 0;
		ଯାଅ out;
	}

	dir = opendir(path->buf);
	ଯଦି (!dir) {
		/* an empty dir could be removed even if it is unreadble */
		res = dry_run ? 0 : rmdir(path->buf);
		ଯଦି (res) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
			quote_path(path->buf, prefix, &quoted, 0);
			errno = saved_errno;
			warning_errno(_(ସନ୍ଦେଶ_warn_remove_failed), quoted.buf);
			*dir_gone = 0;
		}
		ret = res;
		ଯାଅ out;
	}

	strbuf_complete(path, '/');

	len = path->len;
	ଯେତେବେଳେ ((e = readdir_skip_dot_and_dotdot(dir)) != ଖାଲି) {
		ସଂରଚନା stat st;

		strbuf_setlen(path, len);
		strbuf_addstr(path, e->d_name);
		ଯଦି (lstat(path->buf, &st))
			warning_errno(_(ସନ୍ଦେଶ_warn_lstat_failed), path->buf);
		ଅନ୍ୟଥା ଯଦି (S_ISDIR(st.st_mode)) {
			ଯଦି (remove_dirs(path, prefix, force_flag, dry_run, quiet, &gone))
				ret = 1;
			ଯଦି (gone) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} ଅନ୍ୟଥା
				*dir_gone = 0;
			ଜାରି;
		} ଅନ୍ୟଥା {
			res = dry_run ? 0 : unlink(path->buf);
			ଯଦି (!res) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} ଅନ୍ୟଥା {
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
				quote_path(path->buf, prefix, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(ସନ୍ଦେଶ_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
			ଜାରି;
		}

		/* path too long, stat fails, or non-directory still exists */
		*dir_gone = 0;
		ret = 1;
		ଭାଙ୍ଗ;
	}
	closedir(dir);

	strbuf_setlen(path, original_len);

	ଯଦି (*dir_gone) {
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
		 ଯଦି (startup_info->original_cwd)
			 strbuf_realpath(&real_ocwd,
					 startup_info->original_cwd, 1);

		ଯଦି (!strbuf_cmp(&realpath, &real_ocwd)) {
			printf("%s", dry_run ? _(ସନ୍ଦେଶ_would_skip_cwd) : _(ସନ୍ଦେଶ_skip_cwd));
			*dir_gone = 0;
		} ଅନ୍ୟଥା {
			res = dry_run ? 0 : rmdir(path->buf);
			ଯଦି (!res)
				*dir_gone = 1;
			ଅନ୍ୟଥା {
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
				quote_path(path->buf, prefix, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(ସନ୍ଦେଶ_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
		}
	}

	ଯଦି (!*dir_gone && !quiet) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
		ପାଇଁ (i = 0; i < dels.nr; i++)
			printf(dry_run ?  _(ସନ୍ଦେଶ_would_remove) : _(ସନ୍ଦେଶ_remove), dels.ବସ୍ତୁଗୁଡ଼ିକ[i].string);
	}
out:
	strbuf_release(&realpath);
	strbuf_release(&real_ocwd);
	strbuf_release(&quoted);
	string_list_clear(&dels, 0);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ pretty_print_dels(ଶୂନ୍ୟ)
{
	ସଂରଚନା string_list list = STRING_LIST_INIT_DUP;
	ସଂରଚନା string_list_item *item;
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *qname;
	ସଂରଚନା column_options copts;

	for_each_string_list_item(item, &del_list) {
		qname = quote_path(item->string, ଖାଲି, &buf, 0);
		string_list_append(&list, qname);
	}

	/*
	 * always enable column display, we only consult column.*
	 * about layout strategy and stuff
	 */
	colopts = (colopts & ~COL_ENABLE_MASK) | COL_ENABLED;
	memset(&copts, 0, ଆକାର(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(&list, colopts, &copts);
	strbuf_release(&buf);
	string_list_clear(&list, 0);
}

ସ୍ଥିର ଶୂନ୍ୟ pretty_print_menus(ସଂରଚନା string_list *menu_list)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା local_colopts = 0;
	ସଂରଚନା column_options copts;

	local_colopts = COL_ENABLED | COL_ROW;
	memset(&copts, 0, ଆକାର(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(menu_list, local_colopts, &copts);
}

ସ୍ଥିର ଶୂନ୍ୟ prompt_help_cmd(ପୂର୍ଣ୍ଣସଂଖ୍ୟା singleton)
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
ସ୍ଥିର ଶୂନ୍ୟ print_highlight_menu_stuff(ସଂରଚନା menu_stuff *stuff, ପୂର୍ଣ୍ଣସଂଖ୍ୟା **chosen)
{
	ସଂରଚନା string_list menu_list = STRING_LIST_INIT_DUP;
	ସଂରଚନା strbuf menu = STRBUF_INIT;
	ସଂରଚନା menu_item *menu_item;
	ସଂରଚନା string_list_item *string_list_item;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ବାଛ (stuff->type) {
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		die("Bad type of menu_stuff when print menu");
	ସ୍ଥିତି MENU_STUFF_TYPE_MENU_ITEM:
		menu_item = (ସଂରଚନା menu_item *)stuff->stuff;
		ପାଇଁ (i = 0; i < stuff->nr; i++, menu_item++) {
			ଅଚଳ ଅକ୍ଷର *p;
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା highlighted = 0;

			p = menu_item->title;
			ଯଦି ((*chosen)[i] < 0)
				(*chosen)[i] = menu_item->selected ? 1 : 0;
			strbuf_addf(&menu, "%s%2d: ", (*chosen)[i] ? "*" : " ", i+1);
			ପାଇଁ (; *p; p++) {
				ଯଦି (!highlighted && *p == menu_item->hotkey) {
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_PROMPT));
					strbuf_addch(&menu, *p);
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_RESET));
					highlighted = 1;
				} ଅନ୍ୟଥା {
					strbuf_addch(&menu, *p);
				}
			}
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
		}
		ଭାଙ୍ଗ;
	ସ୍ଥିତି MENU_STUFF_TYPE_STRING_LIST:
		i = 0;
		for_each_string_list_item(string_list_item, (ସଂରଚନା string_list *)stuff->stuff) {
			ଯଦି ((*chosen)[i] < 0)
				(*chosen)[i] = 0;
			strbuf_addf(&menu, "%s%2d: %s",
				    (*chosen)[i] ? "*" : " ", i+1, string_list_item->string);
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
			i++;
		}
		ଭାଙ୍ଗ;
	}

	pretty_print_menus(&menu_list);

	strbuf_release(&menu);
	string_list_clear(&menu_list, 0);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା find_unique(ଅଚଳ ଅକ୍ଷର *choice, ସଂରଚନା menu_stuff *menu_stuff)
{
	ସଂରଚନା menu_item *menu_item;
	ସଂରଚନା string_list_item *string_list_item;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, len, found = 0;

	len = strlen(choice);
	ବାଛ (menu_stuff->type) {
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		die("Bad type of menu_stuff when parse choice");
	ସ୍ଥିତି MENU_STUFF_TYPE_MENU_ITEM:

		menu_item = (ସଂରଚନା menu_item *)menu_stuff->stuff;
		ପାଇଁ (i = 0; i < menu_stuff->nr; i++, menu_item++) {
			ଯଦି (len == 1 && *choice == menu_item->hotkey) {
				found = i + 1;
				ଭାଙ୍ଗ;
			}
			ଯଦି (!strncasecmp(choice, menu_item->title, len)) {
				ଯଦି (found) {
					ଯଦି (len == 1) {
						/* continue for hotkey matching */
						found = -1;
					} ଅନ୍ୟଥା {
						found = 0;
						ଭାଙ୍ଗ;
					}
				} ଅନ୍ୟଥା {
					found = i + 1;
				}
			}
		}
		ଭାଙ୍ଗ;
	ସ୍ଥିତି MENU_STUFF_TYPE_STRING_LIST:
		string_list_item = ((ସଂରଚନା string_list *)menu_stuff->stuff)->ବସ୍ତୁଗୁଡ଼ିକ;
		ପାଇଁ (i = 0; i < menu_stuff->nr; i++, string_list_item++) {
			ଯଦି (!strncasecmp(choice, string_list_item->string, len)) {
				ଯଦି (found) {
					found = 0;
					ଭାଙ୍ଗ;
				}
				found = i + 1;
			}
		}
		ଭାଙ୍ଗ;
	}
	ଫେରନ୍ତୁ found;
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_choice(ସଂରଚନା menu_stuff *menu_stuff,
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_single,
			ଅକ୍ଷର *ନିବେଶ,
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା **chosen)
{
	ସଂରଚନା string_list choice = STRING_LIST_INIT_NODUP;
	ସଂରଚନା string_list_item *item;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	string_list_split_in_place_f(&choice, ନିବେଶ,
				     is_single ? "\n" : ", ", -1,
				     STRING_LIST_SPLIT_TRIM);

	for_each_string_list_item(item, &choice) {
		ଅଚଳ ଅକ୍ଷର *string;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା choose;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା bottom = 0, ଶୀର୍ଷ = 0;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_range, is_ସଂଖ୍ୟା;

		string = item->string;
		ଯଦି (!*string)
			ଜାରି;

		/* Input that begins with '-'; unchoose */
		ଯଦି (string[0] == '-') {
			choose = 0;
			string++;
		} ଅନ୍ୟଥା {
			choose = 1;
		}

		is_range = 0;
		is_ସଂଖ୍ୟା = 1;
		ପାଇଁ (ଅଚଳ ଅକ୍ଷର *p = string; *p; p++) {
			ଯଦି ('-' == *p) {
				ଯଦି (!is_range) {
					is_range = 1;
					is_ସଂଖ୍ୟା = 0;
				} ଅନ୍ୟଥା {
					is_ସଂଖ୍ୟା = 0;
					is_range = 0;
					ଭାଙ୍ଗ;
				}
			} ଅନ୍ୟଥା ଯଦି (!isdigit(*p)) {
				is_ସଂଖ୍ୟା = 0;
				is_range = 0;
				ଭାଙ୍ଗ;
			}
		}

		ଯଦି (is_ସଂଖ୍ୟା) {
			bottom = atoi(string);
			ଶୀର୍ଷ = bottom;
		} ଅନ୍ୟଥା ଯଦି (is_range) {
			bottom = atoi(string);
			/* a range can be specified like 5-7 or 5- */
			ଯଦି (!*(strchr(string, '-') + 1))
				ଶୀର୍ଷ = menu_stuff->nr;
			ଅନ୍ୟଥା
				ଶୀର୍ଷ = atoi(strchr(string, '-') + 1);
		} ଅନ୍ୟଥା ଯଦି (!strcmp(string, "*")) {
			bottom = 1;
			ଶୀର୍ଷ = menu_stuff->nr;
		} ଅନ୍ୟଥା {
			bottom = find_unique(string, menu_stuff);
			ଶୀର୍ଷ = bottom;
		}

		ଯଦି (ଶୀର୍ଷ <= 0 || bottom <= 0 || ଶୀର୍ଷ > menu_stuff->nr || bottom > ଶୀର୍ଷ ||
		    (is_single && bottom != ଶୀର୍ଷ)) {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf(_("Huh (%s)?\n"), string);
			clean_print_color(CLEAN_COLOR_RESET);
			ଜାରି;
		}

		ପାଇଁ (i = bottom; i <= ଶୀର୍ଷ; i++)
			(*chosen)[i-1] = choose;
	}

	string_list_clear(&choice, 0);

	ପାଇଁ (i = 0; i < menu_stuff->nr; i++)
		nr += (*chosen)[i];
	ଫେରନ୍ତୁ nr;
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା *list_and_choose(ସଂରଚନା menu_opts *opts, ସଂରଚନା menu_stuff *stuff)
{
	ସଂରଚନା strbuf choice = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *chosen, *result;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା eof = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ALLOC_ARRAY(chosen, stuff->nr);
	/* set chosen as uninitialized */
	ପାଇଁ (i = 0; i < stuff->nr; i++)
		chosen[i] = -1;

	ପାଇଁ (;;) {
		ଯଦି (opts->header) {
			printf_ln("%s%s%s",
				  clean_get_color(CLEAN_COLOR_HEADER),
				  _(opts->header),
				  clean_get_color(CLEAN_COLOR_RESET));
		}

		/* chosen will be initialized by print_highlight_menu_stuff */
		print_highlight_menu_stuff(stuff, &chosen);

		ଯଦି (opts->flags & MENU_OPTS_LIST_ONLY)
			ଭାଙ୍ଗ;

		ଯଦି (opts->prompt) {
			printf("%s%s%s%s",
			       clean_get_color(CLEAN_COLOR_PROMPT),
			       _(opts->prompt),
			       opts->flags & MENU_OPTS_SINGLETON ? "> " : ">> ",
			       clean_get_color(CLEAN_COLOR_RESET));
		}

		ଯଦି (git_read_ଧାଡ଼ି_interactively(&choice) == EOF) {
			eof = 1;
			ଭାଙ୍ଗ;
		}

		/* help for prompt */
		ଯଦି (!strcmp(choice.buf, "?")) {
			prompt_help_cmd(opts->flags & MENU_OPTS_SINGLETON);
			ଜାରି;
		}

		/* for a multiple-choice menu, press ENTER (empty) will return back */
		ଯଦି (!(opts->flags & MENU_OPTS_SINGLETON) && !choice.len)
			ଭାଙ୍ଗ;

		nr = parse_choice(stuff,
				  opts->flags & MENU_OPTS_SINGLETON,
				  choice.buf,
				  &chosen);

		ଯଦି (opts->flags & MENU_OPTS_SINGLETON) {
			ଯଦି (nr)
				ଭାଙ୍ଗ;
		} ଅନ୍ୟଥା ଯଦି (opts->flags & MENU_OPTS_IMMEDIATE) {
			ଭାଙ୍ଗ;
		}
	}

	ଯଦି (eof) {
		result = xmalloc(ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
		*result = EOF;
	} ଅନ୍ୟଥା {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0;

		/*
		 * recalculate nr, if return back from menu directly with
		 * default selections.
		 */
		ଯଦି (!nr) {
			ପାଇଁ (i = 0; i < stuff->nr; i++)
				nr += chosen[i];
		}

		CALLOC_ARRAY(result, st_add(nr, 1));
		ପାଇଁ (i = 0; i < stuff->nr && j < nr; i++) {
			ଯଦି (chosen[i])
				result[j++] = i;
		}
		result[j] = EOF;
	}

	ମୁକ୍ତ(chosen);
	strbuf_release(&choice);
	ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା clean_cmd(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ MENU_RETURN_NO_LOOP;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା filter_by_patterns_cmd(ଶୂନ୍ୟ)
{
	ସଂରଚନା dir_struct dir = DIR_INIT;
	ସଂରଚନା strbuf confirm = STRBUF_INIT;
	ସଂରଚନା pattern_list *pl;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା changed = -1, i;

	ପାଇଁ (;;) {
		ସଂରଚନା string_list ignore_list = STRING_LIST_INIT_NODUP;
		ସଂରଚନା string_list_item *item;

		ଯଦି (!del_list.nr)
			ଭାଙ୍ଗ;

		ଯଦି (changed)
			pretty_print_dels();

		clean_print_color(CLEAN_COLOR_PROMPT);
		printf(_("Input ignore patterns>> "));
		clean_print_color(CLEAN_COLOR_RESET);
		ଯଦି (git_read_ଧାଡ଼ି_interactively(&confirm) == EOF)
			putchar('\n');

		/* quit filter_by_pattern mode if press ENTER or Ctrl-D */
		ଯଦି (!confirm.len)
			ଭାଙ୍ଗ;

		pl = add_pattern_list(&dir, EXC_CMDL, "manual exclude");

		string_list_split_in_place_f(&ignore_list, confirm.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);

		ପାଇଁ (i = 0; i < ignore_list.nr; i++) {
			item = &ignore_list.ବସ୍ତୁଗୁଡ଼ିକ[i];
			ଯଦି (!*item->string)
				ଜାରି;
			add_pattern(item->string, "", 0, pl, -(i+1));
		}

		changed = 0;
		for_each_string_list_item(item, &del_list) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype = DT_UNKNOWN;

			ଯଦି (is_excluded(&dir, the_repository->index, item->string, &dtype)) {
				*item->string = '\0';
				changed++;
			}
		}

		ଯଦି (changed) {
			string_list_remove_empty_ବସ୍ତୁଗୁଡ଼ିକ(&del_list, 0);
		} ଅନ୍ୟଥା {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf_ln(_("WARNING: Cannot find items matched by: %s"), confirm.buf);
			clean_print_color(CLEAN_COLOR_RESET);
		}

		string_list_clear(&ignore_list, 0);
		dir_clear(&dir);
	}

	strbuf_release(&confirm);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା select_by_numbers_cmd(ଶୂନ୍ୟ)
{
	ସଂରଚନା menu_opts menu_opts;
	ସଂରଚନା menu_stuff menu_stuff;
	ସଂରଚନା string_list_item *ବସ୍ତୁଗୁଡ଼ିକ;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *chosen;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;

	menu_opts.header = ଖାଲି;
	menu_opts.prompt = N_("Select items to delete");
	menu_opts.flags = 0;

	menu_stuff.type = MENU_STUFF_TYPE_STRING_LIST;
	menu_stuff.stuff = &del_list;
	menu_stuff.nr = del_list.nr;

	chosen = list_and_choose(&menu_opts, &menu_stuff);
	ବସ୍ତୁଗୁଡ଼ିକ = del_list.ବସ୍ତୁଗୁଡ଼ିକ;
	ପାଇଁ (i = 0, j = 0; i < del_list.nr; i++) {
		ଯଦି (i < chosen[j]) {
			*(ବସ୍ତୁଗୁଡ଼ିକ[i].string) = '\0';
		} ଅନ୍ୟଥା ଯଦି (i == chosen[j]) {
			/* delete selected item */
			j++;
			ଜାରି;
		} ଅନ୍ୟଥା {
			/* end of chosen (chosen[j] == EOF), won't delete */
			*(ବସ୍ତୁଗୁଡ଼ିକ[i].string) = '\0';
		}
	}

	string_list_remove_empty_ବସ୍ତୁଗୁଡ଼ିକ(&del_list, 0);

	ମୁକ୍ତ(chosen);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ask_each_cmd(ଶୂନ୍ୟ)
{
	ସଂରଚନା strbuf confirm = STRBUF_INIT;
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ସଂରଚନା string_list_item *item;
	ଅଚଳ ଅକ୍ଷର *qname;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା changed = 0, eof = 0;

	for_each_string_list_item(item, &del_list) {
		/* Ctrl-D should stop removing files */
		ଯଦି (!eof) {
			qname = quote_path(item->string, ଖାଲି, &buf, 0);
			/* TRANSLATORS: Make sure to keep [y/N] as is */
			printf(_("Remove %s [y/N]? "), qname);
			ଯଦି (git_read_ଧାଡ଼ି_interactively(&confirm) == EOF) {
				putchar('\n');
				eof = 1;
			}
		}
		ଯଦି (!confirm.len || strncasecmp(confirm.buf, "yes", confirm.len)) {
			*item->string = '\0';
			changed++;
		}
	}

	ଯଦି (changed)
		string_list_remove_empty_ବସ୍ତୁଗୁଡ଼ିକ(&del_list, 0);

	strbuf_release(&buf);
	strbuf_release(&confirm);
	ଫେରନ୍ତୁ MENU_RETURN_NO_LOOP;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା quit_cmd(ଶୂନ୍ୟ)
{
	string_list_clear(&del_list, 0);
	printf(_("Bye.\n"));
	ଫେରନ୍ତୁ MENU_RETURN_NO_LOOP;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା help_cmd(ଶୂନ୍ୟ)
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
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ interactive_main_loop(ଶୂନ୍ୟ)
{
	ଯେତେବେଳେ (del_list.nr) {
		ସଂରଚନା menu_opts menu_opts;
		ସଂରଚନା menu_stuff menu_stuff;
		ସଂରଚନା menu_item menus[] = {
			{'c', "clean",			0, clean_cmd},
			{'f', "filter by pattern",	0, filter_by_patterns_cmd},
			{'s', "select by numbers",	0, select_by_numbers_cmd},
			{'a', "ask each",		0, ask_each_cmd},
			{'q', "quit",			0, quit_cmd},
			{'h', "help",			0, help_cmd},
		};
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା *chosen;

		menu_opts.header = N_("*** Commands ***");
		menu_opts.prompt = N_("What now");
		menu_opts.flags = MENU_OPTS_SINGLETON;

		menu_stuff.type = MENU_STUFF_TYPE_MENU_ITEM;
		menu_stuff.stuff = menus;
		menu_stuff.nr = ଆକାର(menus) / ଆକାର(ସଂରଚନା menu_item);

		clean_print_color(CLEAN_COLOR_HEADER);
		printf_ln(Q_("Would remove the following item:",
			     "Would remove the following items:",
			     del_list.nr));
		clean_print_color(CLEAN_COLOR_RESET);

		pretty_print_dels();

		chosen = list_and_choose(&menu_opts, &menu_stuff);

		ଯଦି (*chosen != EOF) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
			ret = menus[*chosen].fn();
			ଯଦି (ret != MENU_RETURN_NO_LOOP) {
				FREE_AND_NULL(chosen);
				ଯଦି (!del_list.nr) {
					clean_print_color(CLEAN_COLOR_ERROR);
					printf_ln(_("No more files to clean, exiting."));
					clean_print_color(CLEAN_COLOR_RESET);
					ଭାଙ୍ଗ;
				}
				ଜାରି;
			}
		} ଅନ୍ୟଥା {
			quit_cmd();
		}

		FREE_AND_NULL(chosen);
		ଭାଙ୍ଗ;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ correct_untracked_entries(ସଂରଚନା dir_struct *dir)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା src, dst, ign;

	ପାଇଁ (src = dst = ign = 0; src < dir->nr; src++) {
		/* skip paths in ignored[] that cannot be inside entries[src] */
		ଯେତେବେଳେ (ign < dir->ignored_nr &&
		       0 <= cmp_dir_entry(&dir->entries[src], &dir->ignored[ign]))
			ign++;

		ଯଦି (ign < dir->ignored_nr &&
		    check_dir_entry_contains(dir->entries[src], dir->ignored[ign])) {
			/* entries[src] contains an ignored path, so we drop it */
			ମୁକ୍ତ(dir->entries[src]);
		} ଅନ୍ୟଥା {
			ସଂରଚନା dir_entry *ent = dir->entries[src++];

			/* entries[src] does not contain an ignored path, so we keep it */
			dir->entries[dst++] = ent;

			/* then discard paths in entries[] contained inside entries[src] */
			ଯେତେବେଳେ (src < dir->nr &&
			       check_dir_entry_contains(ent, dir->entries[src]))
				ମୁକ୍ତ(dir->entries[src++]);

			/* compensate for the outer loop's loop control */
			src--;
		}
	}
	dir->nr = dst;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_clean(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	      ଅଚଳ ଅକ୍ଷର **argv,
	      ଅଚଳ ଅକ୍ଷର *prefix,
	      ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, res;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା dry_run = 0, remove_directories = 0, quiet = 0, ignored = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ignored_only = 0, force = 0, errors = 0, gone = 1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା rm_flags = REMOVE_DIR_KEEP_NESTED_GIT;
	ସଂରଚନା strbuf abs_path = STRBUF_INIT;
	ସଂରଚନା dir_struct dir = DIR_INIT;
	ସଂରଚନା pathspec pathspec;
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ସଂରଚନା string_list exclude_list = STRING_LIST_INIT_NODUP;
	ସଂରଚନା pattern_list *pl;
	ସଂରଚନା string_list_item *item;
	ଅଚଳ ଅକ୍ଷର *qname;
	ସଂରଚନା option options[] = {
		OPT__QUIET(&quiet, N_("do not print names of files removed")),
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('i', "interactive", &interactive, N_("interactive cleaning")),
		OPT_BOOL('d', ଖାଲି, &remove_directories,
				N_("remove whole directories")),
		OPT_CALLBACK_F('e', "exclude", &exclude_list, N_("pattern"),
		  N_("add <pattern> to ignore rules"), PARSE_OPT_NONEG, exclude_cb),
		OPT_BOOL('x', ଖାଲି, &ignored, N_("remove ignored files, too")),
		OPT_BOOL('X', ଖାଲି, &ignored_only,
				N_("remove only ignored files")),
		OPT_END()
	};

	repo_config(the_repository, git_clean_config, ଖାଲି);

	argc = parse_options(argc, argv, prefix, options, builtin_clean_usage,
			     0);

	ଯଦି (require_force != 0 && !force && !interactive && !dry_run)
		die(_("clean.requireForce is true and -f not given: refusing to clean"));

	ଯଦି (force > 1)
		rm_flags = 0;
	ଅନ୍ୟଥା
		dir.flags |= DIR_SKIP_NESTED_GIT;

	dir.flags |= DIR_SHOW_OTHER_DIRECTORIES;

	ଯଦି (ignored && ignored_only)
		die(_("options '%s' and '%s' cannot be used together"), "-x", "-X");
	ଯଦି (!ignored)
		setup_standard_excludes(&dir);
	ଯଦି (ignored_only)
		dir.flags |= DIR_SHOW_IGNORED;

	ଯଦି (argc) {
		/*
		 * Remaining args implies pathspecs specified, and we should
		 * recurse within those.
		 */
		remove_directories = 1;
	}

	ଯଦି (remove_directories && !ignored_only) {
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
		ଯଦି (!ignored)
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

	ଯଦି (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	ପାଇଁ (i = 0; i < exclude_list.nr; i++)
		add_pattern(exclude_list.ବସ୍ତୁଗୁଡ଼ିକ[i].string, "", 0, pl, -(i+1));

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	fill_directory(&dir, the_repository->index, &pathspec);
	correct_untracked_entries(&dir);

	ପାଇଁ (i = 0; i < dir.nr; i++) {
		ସଂରଚନା dir_entry *ent = dir.entries[i];
		ସଂରଚନା stat st;
		ଅଚଳ ଅକ୍ଷର *rel;

		ଯଦି (!index_name_is_other(the_repository->index, ent->name, ent->len))
			ଜାରି;

		ଯଦି (lstat(ent->name, &st))
			die_errno("Cannot lstat '%s'", ent->name);

		ଯଦି (S_ISDIR(st.st_mode) && !remove_directories)
			ଜାରି;

		rel = relative_path(ent->name, prefix, &buf);
		string_list_append(&del_list, rel);
	}

	dir_clear(&dir);

	ଯଦି (interactive && del_list.nr > 0)
		interactive_main_loop();

	for_each_string_list_item(item, &del_list) {
		ସଂରଚନା stat st;

		strbuf_reset(&abs_path);
		ଯଦି (prefix)
			strbuf_addstr(&abs_path, prefix);

		strbuf_addstr(&abs_path, item->string);

		/*
		 * we might have removed this as part of earlier
		 * recursive directory removal, so lstat() here could
		 * fail with ENOENT.
		 */
		ଯଦି (lstat(abs_path.buf, &st))
			ଜାରି;

		ଯଦି (S_ISDIR(st.st_mode)) {
			ଯଦି (remove_dirs(&abs_path, prefix, rm_flags, dry_run, quiet, &gone))
				errors++;
			ଯଦି (gone && !quiet) {
				qname = quote_path(item->string, ଖାଲି, &buf, 0);
				printf(dry_run ? _(ସନ୍ଦେଶ_would_remove) : _(ସନ୍ଦେଶ_remove), qname);
			}
		} ଅନ୍ୟଥା {
			res = dry_run ? 0 : unlink(abs_path.buf);
			ଯଦି (res) {
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
				qname = quote_path(item->string, ଖାଲି, &buf, 0);
				errno = saved_errno;
				warning_errno(_(ସନ୍ଦେଶ_warn_remove_failed), qname);
				errors++;
			} ଅନ୍ୟଥା ଯଦି (!quiet) {
				qname = quote_path(item->string, ଖାଲି, &buf, 0);
				printf(dry_run ? _(ସନ୍ଦେଶ_would_remove) : _(ସନ୍ଦେଶ_remove), qname);
			}
		}
	}

	strbuf_release(&abs_path);
	strbuf_release(&buf);
	string_list_clear(&del_list, 0);
	string_list_clear(&exclude_list, 0);
	clear_pathspec(&pathspec);
	ଫେରନ୍ତୁ (errors != 0);
}
