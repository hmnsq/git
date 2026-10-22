/*
 * "git clean" builtin command
 *
 * Copyright (C) 2007 Shawn Bohrer
 *
 * Based on git-clean.sh by Pavel Roskin
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "abspath.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "read-cache-ll.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "column.h"
#अन्तर्भाव "color.h"
#अन्तर्भाव "pathspec.h"
#अन्तर्भाव "help.h"
#अन्तर्भाव "prompt.h"

स्थिर पूर्णाङ्क require_force = -1; /* unset */
स्थिर पूर्णाङ्क interactive;
स्थिर संरचना string_list del_list = STRING_LIST_INIT_DUP;
स्थिर unsigned पूर्णाङ्क colopts;

स्थिर ध्रुव वर्ण *ध्रुव builtin_clean_usage[] = {
	N_("git clean [-d] [-f] [-i] [-n] [-q] [-e <pattern>] [-x | -X] [--] [<pathspec>...]"),
	रिक्त
};

स्थिर ध्रुव वर्ण *सन्देशः_remove = N_("Removing %s\n");
स्थिर ध्रुव वर्ण *सन्देशः_would_remove = N_("Would remove %s\n");
स्थिर ध्रुव वर्ण *सन्देशः_skip_git_dir = N_("Skipping repository %s\n");
स्थिर ध्रुव वर्ण *सन्देशः_would_skip_git_dir = N_("Would skip repository %s\n");
स्थिर ध्रुव वर्ण *सन्देशः_warn_remove_failed = N_("failed to remove %s");
स्थिर ध्रुव वर्ण *सन्देशः_warn_lstat_failed = N_("could not lstat %s\n");
स्थिर ध्रुव वर्ण *सन्देशः_skip_cwd = N_("Refusing to remove current working directory\n");
स्थिर ध्रुव वर्ण *सन्देशः_would_skip_cwd = N_("Would refuse to remove current working directory\n");

गणना color_clean {
	CLEAN_COLOR_RESET = 0,
	CLEAN_COLOR_PLAIN = 1,
	CLEAN_COLOR_PROMPT = 2,
	CLEAN_COLOR_HEADER = 3,
	CLEAN_COLOR_HELP = 4,
	CLEAN_COLOR_ERROR = 5
};

स्थिर ध्रुव वर्ण *color_interactive_slots[] = {
	[CLEAN_COLOR_ERROR]  = "error",
	[CLEAN_COLOR_HEADER] = "header",
	[CLEAN_COLOR_HELP]   = "help",
	[CLEAN_COLOR_PLAIN]  = "plain",
	[CLEAN_COLOR_PROMPT] = "prompt",
	[CLEAN_COLOR_RESET]  = "reset",
};

स्थिर गणना git_colorbool clean_use_color = GIT_COLOR_UNKNOWN;
स्थिर वर्ण clean_colors[][COLOR_MAXLEN] = {
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

संरचना menu_opts {
	ध्रुव वर्ण *header;
	ध्रुव वर्ण *prompt;
	पूर्णाङ्क flags;
};

#define MENU_RETURN_NO_LOOP		10

संरचना menu_item {
	वर्ण hotkey;
	ध्रुव वर्ण *title;
	पूर्णाङ्क selected;
	पूर्णाङ्क (*fn)(शून्य);
};

गणना menu_stuff_type {
	MENU_STUFF_TYPE_STRING_LIST = 1,
	MENU_STUFF_TYPE_MENU_ITEM
};

संरचना menu_stuff {
	गणना menu_stuff_type type;
	पूर्णाङ्क nr;
	शून्य *stuff;
};

define_list_config_array(color_interactive_slots);

स्थिर पूर्णाङ्क git_clean_config(ध्रुव वर्ण *var, ध्रुव वर्ण *मानम्,
			    ध्रुव संरचना config_context *ctx, शून्य *cb)
{
	ध्रुव वर्ण *slot_name;

	सचे (starts_with(var, "column."))
		पटिनिवत्त git_column_config(var, मानम्, "clean", &colopts);

	/* honors the color.interactive* config variables which also
	   applied in git-add--interactive and git-stash */
	सचे (!strcmp(var, "color.interactive")) {
		clean_use_color = git_config_colorbool(var, मानम्);
		पटिनिवत्त 0;
	}
	सचे (skip_prefix(var, "color.interactive.", &slot_name)) {
		पूर्णाङ्क slot = LOOKUP_CONFIG(color_interactive_slots, slot_name);
		सचे (slot < 0)
			पटिनिवत्त 0;
		सचे (!मानम्)
			पटिनिवत्त config_error_nonbool(var);
		पटिनिवत्त color_parse(मानम्, clean_colors[slot]);
	}

	सचे (!strcmp(var, "clean.requireforce")) {
		require_force = git_config_bool(var, मानम्);
		पटिनिवत्त 0;
	}

	सचे (git_color_config(var, मानम्, cb) < 0)
		पटिनिवत्त -1;

	पटिनिवत्त git_default_config(var, मानम्, ctx, cb);
}

स्थिर ध्रुव वर्ण *clean_get_color(गणना color_clean ix)
{
	सचे (want_color(clean_use_color))
		पटिनिवत्त clean_colors[ix];
	पटिनिवत्त "";
}

स्थिर शून्य clean_print_color(गणना color_clean ix)
{
	printf("%s", clean_get_color(ix));
}

स्थिर पूर्णाङ्क exclude_cb(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना string_list *exclude_list = opt->मानम्;
	BUG_ON_OPT_NEG(unset);
	string_list_append(exclude_list, arg);
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क remove_dirs(संरचना strbuf *path, ध्रुव वर्ण *prefix, पूर्णाङ्क force_flag,
		पूर्णाङ्क dry_run, पूर्णाङ्क quiet, पूर्णाङ्क *dir_gone)
{
	DIR *dir;
	संरचना strbuf quoted = STRBUF_INIT;
	संरचना strbuf realpath = STRBUF_INIT;
	संरचना strbuf real_ocwd = STRBUF_INIT;
	संरचना dirent *e;
	पूर्णाङ्क res = 0, ret = 0, gone = 1, original_len = path->len, len;
	संरचना string_list dels = STRING_LIST_INIT_DUP;

	*dir_gone = 1;

	सचे ((force_flag & REMOVE_DIR_KEEP_NESTED_GIT) &&
	    is_nonbare_repository_dir(path)) {
		सचे (!quiet) {
			quote_path(path->buf, prefix, &quoted, 0);
			printf(dry_run ?  _(सन्देशः_would_skip_git_dir) : _(सन्देशः_skip_git_dir),
					quoted.buf);
		}

		*dir_gone = 0;
		गच्छ out;
	}

	dir = opendir(path->buf);
	सचे (!dir) {
		/* an empty dir could be removed even if it is unreadble */
		res = dry_run ? 0 : rmdir(path->buf);
		सचे (res) {
			पूर्णाङ्क saved_errno = errno;
			quote_path(path->buf, prefix, &quoted, 0);
			errno = saved_errno;
			warning_errno(_(सन्देशः_warn_remove_failed), quoted.buf);
			*dir_gone = 0;
		}
		ret = res;
		गच्छ out;
	}

	strbuf_complete(path, '/');

	len = path->len;
	याव ((e = readdir_skip_dot_and_dotdot(dir)) != रिक्त) {
		संरचना stat st;

		strbuf_setlen(path, len);
		strbuf_addstr(path, e->d_name);
		सचे (lstat(path->buf, &st))
			warning_errno(_(सन्देशः_warn_lstat_failed), path->buf);
		अञ्ञथा सचे (S_ISDIR(st.st_mode)) {
			सचे (remove_dirs(path, prefix, force_flag, dry_run, quiet, &gone))
				ret = 1;
			सचे (gone) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} अञ्ञथा
				*dir_gone = 0;
			अनुवर्त;
		} अञ्ञथा {
			res = dry_run ? 0 : unlink(path->buf);
			सचे (!res) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} अञ्ञथा {
				पूर्णाङ्क saved_errno = errno;
				quote_path(path->buf, prefix, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(सन्देशः_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
			अनुवर्त;
		}

		/* path too long, stat fails, or non-directory still exists */
		*dir_gone = 0;
		ret = 1;
		भङ्ग;
	}
	closedir(dir);

	strbuf_setlen(path, original_len);

	सचे (*dir_gone) {
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
		 सचे (startup_info->original_cwd)
			 strbuf_realpath(&real_ocwd,
					 startup_info->original_cwd, 1);

		सचे (!strbuf_cmp(&realpath, &real_ocwd)) {
			printf("%s", dry_run ? _(सन्देशः_would_skip_cwd) : _(सन्देशः_skip_cwd));
			*dir_gone = 0;
		} अञ्ञथा {
			res = dry_run ? 0 : rmdir(path->buf);
			सचे (!res)
				*dir_gone = 1;
			अञ्ञथा {
				पूर्णाङ्क saved_errno = errno;
				quote_path(path->buf, prefix, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(सन्देशः_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
		}
	}

	सचे (!*dir_gone && !quiet) {
		पूर्णाङ्क i;
		अत्थाय (i = 0; i < dels.nr; i++)
			printf(dry_run ?  _(सन्देशः_would_remove) : _(सन्देशः_remove), dels.वस्तूनि[i].string);
	}
out:
	strbuf_release(&realpath);
	strbuf_release(&real_ocwd);
	strbuf_release(&quoted);
	string_list_clear(&dels, 0);
	पटिनिवत्त ret;
}

स्थिर शून्य pretty_print_dels(शून्य)
{
	संरचना string_list list = STRING_LIST_INIT_DUP;
	संरचना string_list_item *item;
	संरचना strbuf buf = STRBUF_INIT;
	ध्रुव वर्ण *qname;
	संरचना column_options copts;

	for_each_string_list_item(item, &del_list) {
		qname = quote_path(item->string, रिक्त, &buf, 0);
		string_list_append(&list, qname);
	}

	/*
	 * always enable column display, we only consult column.*
	 * about layout strategy and stuff
	 */
	colopts = (colopts & ~COL_ENABLE_MASK) | COL_ENABLED;
	memset(&copts, 0, परिमाण(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(&list, colopts, &copts);
	strbuf_release(&buf);
	string_list_clear(&list, 0);
}

स्थिर शून्य pretty_print_menus(संरचना string_list *menu_list)
{
	unsigned पूर्णाङ्क local_colopts = 0;
	संरचना column_options copts;

	local_colopts = COL_ENABLED | COL_ROW;
	memset(&copts, 0, परिमाण(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(menu_list, local_colopts, &copts);
}

स्थिर शून्य prompt_help_cmd(पूर्णाङ्क singleton)
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
स्थिर शून्य print_highlight_menu_stuff(संरचना menu_stuff *stuff, पूर्णाङ्क **chosen)
{
	संरचना string_list menu_list = STRING_LIST_INIT_DUP;
	संरचना strbuf menu = STRBUF_INIT;
	संरचना menu_item *menu_item;
	संरचना string_list_item *string_list_item;
	पूर्णाङ्क i;

	विकल्प (stuff->type) {
	स्वभाव:
		die("Bad type of menu_stuff when print menu");
	पक्ष MENU_STUFF_TYPE_MENU_ITEM:
		menu_item = (संरचना menu_item *)stuff->stuff;
		अत्थाय (i = 0; i < stuff->nr; i++, menu_item++) {
			ध्रुव वर्ण *p;
			पूर्णाङ्क highlighted = 0;

			p = menu_item->title;
			सचे ((*chosen)[i] < 0)
				(*chosen)[i] = menu_item->selected ? 1 : 0;
			strbuf_addf(&menu, "%s%2d: ", (*chosen)[i] ? "*" : " ", i+1);
			अत्थाय (; *p; p++) {
				सचे (!highlighted && *p == menu_item->hotkey) {
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_PROMPT));
					strbuf_addch(&menu, *p);
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_RESET));
					highlighted = 1;
				} अञ्ञथा {
					strbuf_addch(&menu, *p);
				}
			}
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
		}
		भङ्ग;
	पक्ष MENU_STUFF_TYPE_STRING_LIST:
		i = 0;
		for_each_string_list_item(string_list_item, (संरचना string_list *)stuff->stuff) {
			सचे ((*chosen)[i] < 0)
				(*chosen)[i] = 0;
			strbuf_addf(&menu, "%s%2d: %s",
				    (*chosen)[i] ? "*" : " ", i+1, string_list_item->string);
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
			i++;
		}
		भङ्ग;
	}

	pretty_print_menus(&menu_list);

	strbuf_release(&menu);
	string_list_clear(&menu_list, 0);
}

स्थिर पूर्णाङ्क find_unique(ध्रुव वर्ण *choice, संरचना menu_stuff *menu_stuff)
{
	संरचना menu_item *menu_item;
	संरचना string_list_item *string_list_item;
	पूर्णाङ्क i, len, found = 0;

	len = strlen(choice);
	विकल्प (menu_stuff->type) {
	स्वभाव:
		die("Bad type of menu_stuff when parse choice");
	पक्ष MENU_STUFF_TYPE_MENU_ITEM:

		menu_item = (संरचना menu_item *)menu_stuff->stuff;
		अत्थाय (i = 0; i < menu_stuff->nr; i++, menu_item++) {
			सचे (len == 1 && *choice == menu_item->hotkey) {
				found = i + 1;
				भङ्ग;
			}
			सचे (!strncasecmp(choice, menu_item->title, len)) {
				सचे (found) {
					सचे (len == 1) {
						/* continue for hotkey matching */
						found = -1;
					} अञ्ञथा {
						found = 0;
						भङ्ग;
					}
				} अञ्ञथा {
					found = i + 1;
				}
			}
		}
		भङ्ग;
	पक्ष MENU_STUFF_TYPE_STRING_LIST:
		string_list_item = ((संरचना string_list *)menu_stuff->stuff)->वस्तूनि;
		अत्थाय (i = 0; i < menu_stuff->nr; i++, string_list_item++) {
			सचे (!strncasecmp(choice, string_list_item->string, len)) {
				सचे (found) {
					found = 0;
					भङ्ग;
				}
				found = i + 1;
			}
		}
		भङ्ग;
	}
	पटिनिवत्त found;
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
स्थिर पूर्णाङ्क parse_choice(संरचना menu_stuff *menu_stuff,
			पूर्णाङ्क is_single,
			वर्ण *निवेशः,
			पूर्णाङ्क **chosen)
{
	संरचना string_list choice = STRING_LIST_INIT_NODUP;
	संरचना string_list_item *item;
	पूर्णाङ्क nr = 0;
	पूर्णाङ्क i;

	string_list_split_in_place_f(&choice, निवेशः,
				     is_single ? "\n" : ", ", -1,
				     STRING_LIST_SPLIT_TRIM);

	for_each_string_list_item(item, &choice) {
		ध्रुव वर्ण *string;
		पूर्णाङ्क choose;
		पूर्णाङ्क bottom = 0, शिखरम् = 0;
		पूर्णाङ्क is_range, is_सङ्ख्या;

		string = item->string;
		सचे (!*string)
			अनुवर्त;

		/* Input that begins with '-'; unchoose */
		सचे (string[0] == '-') {
			choose = 0;
			string++;
		} अञ्ञथा {
			choose = 1;
		}

		is_range = 0;
		is_सङ्ख्या = 1;
		अत्थाय (ध्रुव वर्ण *p = string; *p; p++) {
			सचे ('-' == *p) {
				सचे (!is_range) {
					is_range = 1;
					is_सङ्ख्या = 0;
				} अञ्ञथा {
					is_सङ्ख्या = 0;
					is_range = 0;
					भङ्ग;
				}
			} अञ्ञथा सचे (!isdigit(*p)) {
				is_सङ्ख्या = 0;
				is_range = 0;
				भङ्ग;
			}
		}

		सचे (is_सङ्ख्या) {
			bottom = atoi(string);
			शिखरम् = bottom;
		} अञ्ञथा सचे (is_range) {
			bottom = atoi(string);
			/* a range can be specified like 5-7 or 5- */
			सचे (!*(strchr(string, '-') + 1))
				शिखरम् = menu_stuff->nr;
			अञ्ञथा
				शिखरम् = atoi(strchr(string, '-') + 1);
		} अञ्ञथा सचे (!strcmp(string, "*")) {
			bottom = 1;
			शिखरम् = menu_stuff->nr;
		} अञ्ञथा {
			bottom = find_unique(string, menu_stuff);
			शिखरम् = bottom;
		}

		सचे (शिखरम् <= 0 || bottom <= 0 || शिखरम् > menu_stuff->nr || bottom > शिखरम् ||
		    (is_single && bottom != शिखरम्)) {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf(_("Huh (%s)?\n"), string);
			clean_print_color(CLEAN_COLOR_RESET);
			अनुवर्त;
		}

		अत्थाय (i = bottom; i <= शिखरम्; i++)
			(*chosen)[i-1] = choose;
	}

	string_list_clear(&choice, 0);

	अत्थाय (i = 0; i < menu_stuff->nr; i++)
		nr += (*chosen)[i];
	पटिनिवत्त nr;
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
स्थिर पूर्णाङ्क *list_and_choose(संरचना menu_opts *opts, संरचना menu_stuff *stuff)
{
	संरचना strbuf choice = STRBUF_INIT;
	पूर्णाङ्क *chosen, *result;
	पूर्णाङ्क nr = 0;
	पूर्णाङ्क eof = 0;
	पूर्णाङ्क i;

	ALLOC_ARRAY(chosen, stuff->nr);
	/* set chosen as uninitialized */
	अत्थाय (i = 0; i < stuff->nr; i++)
		chosen[i] = -1;

	अत्थाय (;;) {
		सचे (opts->header) {
			printf_ln("%s%s%s",
				  clean_get_color(CLEAN_COLOR_HEADER),
				  _(opts->header),
				  clean_get_color(CLEAN_COLOR_RESET));
		}

		/* chosen will be initialized by print_highlight_menu_stuff */
		print_highlight_menu_stuff(stuff, &chosen);

		सचे (opts->flags & MENU_OPTS_LIST_ONLY)
			भङ्ग;

		सचे (opts->prompt) {
			printf("%s%s%s%s",
			       clean_get_color(CLEAN_COLOR_PROMPT),
			       _(opts->prompt),
			       opts->flags & MENU_OPTS_SINGLETON ? "> " : ">> ",
			       clean_get_color(CLEAN_COLOR_RESET));
		}

		सचे (git_read_पङ्क्तिः_interactively(&choice) == EOF) {
			eof = 1;
			भङ्ग;
		}

		/* help for prompt */
		सचे (!strcmp(choice.buf, "?")) {
			prompt_help_cmd(opts->flags & MENU_OPTS_SINGLETON);
			अनुवर्त;
		}

		/* for a multiple-choice menu, press ENTER (empty) will return back */
		सचे (!(opts->flags & MENU_OPTS_SINGLETON) && !choice.len)
			भङ्ग;

		nr = parse_choice(stuff,
				  opts->flags & MENU_OPTS_SINGLETON,
				  choice.buf,
				  &chosen);

		सचे (opts->flags & MENU_OPTS_SINGLETON) {
			सचे (nr)
				भङ्ग;
		} अञ्ञथा सचे (opts->flags & MENU_OPTS_IMMEDIATE) {
			भङ्ग;
		}
	}

	सचे (eof) {
		result = xmalloc(परिमाण(पूर्णाङ्क));
		*result = EOF;
	} अञ्ञथा {
		पूर्णाङ्क j = 0;

		/*
		 * recalculate nr, if return back from menu directly with
		 * default selections.
		 */
		सचे (!nr) {
			अत्थाय (i = 0; i < stuff->nr; i++)
				nr += chosen[i];
		}

		CALLOC_ARRAY(result, st_add(nr, 1));
		अत्थाय (i = 0; i < stuff->nr && j < nr; i++) {
			सचे (chosen[i])
				result[j++] = i;
		}
		result[j] = EOF;
	}

	मुक्त(chosen);
	strbuf_release(&choice);
	पटिनिवत्त result;
}

स्थिर पूर्णाङ्क clean_cmd(शून्य)
{
	पटिनिवत्त MENU_RETURN_NO_LOOP;
}

स्थिर पूर्णाङ्क filter_by_patterns_cmd(शून्य)
{
	संरचना dir_struct dir = DIR_INIT;
	संरचना strbuf confirm = STRBUF_INIT;
	संरचना pattern_list *pl;
	पूर्णाङ्क changed = -1, i;

	अत्थाय (;;) {
		संरचना string_list ignore_list = STRING_LIST_INIT_NODUP;
		संरचना string_list_item *item;

		सचे (!del_list.nr)
			भङ्ग;

		सचे (changed)
			pretty_print_dels();

		clean_print_color(CLEAN_COLOR_PROMPT);
		printf(_("Input ignore patterns>> "));
		clean_print_color(CLEAN_COLOR_RESET);
		सचे (git_read_पङ्क्तिः_interactively(&confirm) == EOF)
			putchar('\n');

		/* quit filter_by_pattern mode if press ENTER or Ctrl-D */
		सचे (!confirm.len)
			भङ्ग;

		pl = add_pattern_list(&dir, EXC_CMDL, "manual exclude");

		string_list_split_in_place_f(&ignore_list, confirm.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);

		अत्थाय (i = 0; i < ignore_list.nr; i++) {
			item = &ignore_list.वस्तूनि[i];
			सचे (!*item->string)
				अनुवर्त;
			add_pattern(item->string, "", 0, pl, -(i+1));
		}

		changed = 0;
		for_each_string_list_item(item, &del_list) {
			पूर्णाङ्क dtype = DT_UNKNOWN;

			सचे (is_excluded(&dir, the_repository->index, item->string, &dtype)) {
				*item->string = '\0';
				changed++;
			}
		}

		सचे (changed) {
			string_list_remove_empty_वस्तूनि(&del_list, 0);
		} अञ्ञथा {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf_ln(_("WARNING: Cannot find items matched by: %s"), confirm.buf);
			clean_print_color(CLEAN_COLOR_RESET);
		}

		string_list_clear(&ignore_list, 0);
		dir_clear(&dir);
	}

	strbuf_release(&confirm);
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क select_by_numbers_cmd(शून्य)
{
	संरचना menu_opts menu_opts;
	संरचना menu_stuff menu_stuff;
	संरचना string_list_item *वस्तूनि;
	पूर्णाङ्क *chosen;
	पूर्णाङ्क i, j;

	menu_opts.header = रिक्त;
	menu_opts.prompt = N_("Select items to delete");
	menu_opts.flags = 0;

	menu_stuff.type = MENU_STUFF_TYPE_STRING_LIST;
	menu_stuff.stuff = &del_list;
	menu_stuff.nr = del_list.nr;

	chosen = list_and_choose(&menu_opts, &menu_stuff);
	वस्तूनि = del_list.वस्तूनि;
	अत्थाय (i = 0, j = 0; i < del_list.nr; i++) {
		सचे (i < chosen[j]) {
			*(वस्तूनि[i].string) = '\0';
		} अञ्ञथा सचे (i == chosen[j]) {
			/* delete selected item */
			j++;
			अनुवर्त;
		} अञ्ञथा {
			/* end of chosen (chosen[j] == EOF), won't delete */
			*(वस्तूनि[i].string) = '\0';
		}
	}

	string_list_remove_empty_वस्तूनि(&del_list, 0);

	मुक्त(chosen);
	पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क ask_each_cmd(शून्य)
{
	संरचना strbuf confirm = STRBUF_INIT;
	संरचना strbuf buf = STRBUF_INIT;
	संरचना string_list_item *item;
	ध्रुव वर्ण *qname;
	पूर्णाङ्क changed = 0, eof = 0;

	for_each_string_list_item(item, &del_list) {
		/* Ctrl-D should stop removing files */
		सचे (!eof) {
			qname = quote_path(item->string, रिक्त, &buf, 0);
			/* TRANSLATORS: Make sure to keep [y/N] as is */
			printf(_("Remove %s [y/N]? "), qname);
			सचे (git_read_पङ्क्तिः_interactively(&confirm) == EOF) {
				putchar('\n');
				eof = 1;
			}
		}
		सचे (!confirm.len || strncasecmp(confirm.buf, "yes", confirm.len)) {
			*item->string = '\0';
			changed++;
		}
	}

	सचे (changed)
		string_list_remove_empty_वस्तूनि(&del_list, 0);

	strbuf_release(&buf);
	strbuf_release(&confirm);
	पटिनिवत्त MENU_RETURN_NO_LOOP;
}

स्थिर पूर्णाङ्क quit_cmd(शून्य)
{
	string_list_clear(&del_list, 0);
	printf(_("Bye.\n"));
	पटिनिवत्त MENU_RETURN_NO_LOOP;
}

स्थिर पूर्णाङ्क help_cmd(शून्य)
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
	पटिनिवत्त 0;
}

स्थिर शून्य interactive_main_loop(शून्य)
{
	याव (del_list.nr) {
		संरचना menu_opts menu_opts;
		संरचना menu_stuff menu_stuff;
		संरचना menu_item menus[] = {
			{'c', "clean",			0, clean_cmd},
			{'f', "filter by pattern",	0, filter_by_patterns_cmd},
			{'s', "select by numbers",	0, select_by_numbers_cmd},
			{'a', "ask each",		0, ask_each_cmd},
			{'q', "quit",			0, quit_cmd},
			{'h', "help",			0, help_cmd},
		};
		पूर्णाङ्क *chosen;

		menu_opts.header = N_("*** Commands ***");
		menu_opts.prompt = N_("What now");
		menu_opts.flags = MENU_OPTS_SINGLETON;

		menu_stuff.type = MENU_STUFF_TYPE_MENU_ITEM;
		menu_stuff.stuff = menus;
		menu_stuff.nr = परिमाण(menus) / परिमाण(संरचना menu_item);

		clean_print_color(CLEAN_COLOR_HEADER);
		printf_ln(Q_("Would remove the following item:",
			     "Would remove the following items:",
			     del_list.nr));
		clean_print_color(CLEAN_COLOR_RESET);

		pretty_print_dels();

		chosen = list_and_choose(&menu_opts, &menu_stuff);

		सचे (*chosen != EOF) {
			पूर्णाङ्क ret;
			ret = menus[*chosen].fn();
			सचे (ret != MENU_RETURN_NO_LOOP) {
				FREE_AND_NULL(chosen);
				सचे (!del_list.nr) {
					clean_print_color(CLEAN_COLOR_ERROR);
					printf_ln(_("No more files to clean, exiting."));
					clean_print_color(CLEAN_COLOR_RESET);
					भङ्ग;
				}
				अनुवर्त;
			}
		} अञ्ञथा {
			quit_cmd();
		}

		FREE_AND_NULL(chosen);
		भङ्ग;
	}
}

स्थिर शून्य correct_untracked_entries(संरचना dir_struct *dir)
{
	पूर्णाङ्क src, dst, ign;

	अत्थाय (src = dst = ign = 0; src < dir->nr; src++) {
		/* skip paths in ignored[] that cannot be inside entries[src] */
		याव (ign < dir->ignored_nr &&
		       0 <= cmp_dir_entry(&dir->entries[src], &dir->ignored[ign]))
			ign++;

		सचे (ign < dir->ignored_nr &&
		    check_dir_entry_contains(dir->entries[src], dir->ignored[ign])) {
			/* entries[src] contains an ignored path, so we drop it */
			मुक्त(dir->entries[src]);
		} अञ्ञथा {
			संरचना dir_entry *ent = dir->entries[src++];

			/* entries[src] does not contain an ignored path, so we keep it */
			dir->entries[dst++] = ent;

			/* then discard paths in entries[] contained inside entries[src] */
			याव (src < dir->nr &&
			       check_dir_entry_contains(ent, dir->entries[src]))
				मुक्त(dir->entries[src++]);

			/* compensate for the outer loop's loop control */
			src--;
		}
	}
	dir->nr = dst;
}

पूर्णाङ्क cmd_clean(पूर्णाङ्क argc,
	      ध्रुव वर्ण **argv,
	      ध्रुव वर्ण *prefix,
	      संरचना repository *repo UNUSED)
{
	पूर्णाङ्क i, res;
	पूर्णाङ्क dry_run = 0, remove_directories = 0, quiet = 0, ignored = 0;
	पूर्णाङ्क ignored_only = 0, force = 0, errors = 0, gone = 1;
	पूर्णाङ्क rm_flags = REMOVE_DIR_KEEP_NESTED_GIT;
	संरचना strbuf abs_path = STRBUF_INIT;
	संरचना dir_struct dir = DIR_INIT;
	संरचना pathspec pathspec;
	संरचना strbuf buf = STRBUF_INIT;
	संरचना string_list exclude_list = STRING_LIST_INIT_NODUP;
	संरचना pattern_list *pl;
	संरचना string_list_item *item;
	ध्रुव वर्ण *qname;
	संरचना option options[] = {
		OPT__QUIET(&quiet, N_("do not print names of files removed")),
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('i', "interactive", &interactive, N_("interactive cleaning")),
		OPT_BOOL('d', रिक्त, &remove_directories,
				N_("remove whole directories")),
		OPT_CALLBACK_F('e', "exclude", &exclude_list, N_("pattern"),
		  N_("add <pattern> to ignore rules"), PARSE_OPT_NONEG, exclude_cb),
		OPT_BOOL('x', रिक्त, &ignored, N_("remove ignored files, too")),
		OPT_BOOL('X', रिक्त, &ignored_only,
				N_("remove only ignored files")),
		OPT_END()
	};

	repo_config(the_repository, git_clean_config, रिक्त);

	argc = parse_options(argc, argv, prefix, options, builtin_clean_usage,
			     0);

	सचे (require_force != 0 && !force && !interactive && !dry_run)
		die(_("clean.requireForce is true and -f not given: refusing to clean"));

	सचे (force > 1)
		rm_flags = 0;
	अञ्ञथा
		dir.flags |= DIR_SKIP_NESTED_GIT;

	dir.flags |= DIR_SHOW_OTHER_DIRECTORIES;

	सचे (ignored && ignored_only)
		die(_("options '%s' and '%s' cannot be used together"), "-x", "-X");
	सचे (!ignored)
		setup_standard_excludes(&dir);
	सचे (ignored_only)
		dir.flags |= DIR_SHOW_IGNORED;

	सचे (argc) {
		/*
		 * Remaining args implies pathspecs specified, and we should
		 * recurse within those.
		 */
		remove_directories = 1;
	}

	सचे (remove_directories && !ignored_only) {
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
		सचे (!ignored)
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

	सचे (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	अत्थाय (i = 0; i < exclude_list.nr; i++)
		add_pattern(exclude_list.वस्तूनि[i].string, "", 0, pl, -(i+1));

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	fill_directory(&dir, the_repository->index, &pathspec);
	correct_untracked_entries(&dir);

	अत्थाय (i = 0; i < dir.nr; i++) {
		संरचना dir_entry *ent = dir.entries[i];
		संरचना stat st;
		ध्रुव वर्ण *rel;

		सचे (!index_name_is_other(the_repository->index, ent->name, ent->len))
			अनुवर्त;

		सचे (lstat(ent->name, &st))
			die_errno("Cannot lstat '%s'", ent->name);

		सचे (S_ISDIR(st.st_mode) && !remove_directories)
			अनुवर्त;

		rel = relative_path(ent->name, prefix, &buf);
		string_list_append(&del_list, rel);
	}

	dir_clear(&dir);

	सचे (interactive && del_list.nr > 0)
		interactive_main_loop();

	for_each_string_list_item(item, &del_list) {
		संरचना stat st;

		strbuf_reset(&abs_path);
		सचे (prefix)
			strbuf_addstr(&abs_path, prefix);

		strbuf_addstr(&abs_path, item->string);

		/*
		 * we might have removed this as part of earlier
		 * recursive directory removal, so lstat() here could
		 * fail with ENOENT.
		 */
		सचे (lstat(abs_path.buf, &st))
			अनुवर्त;

		सचे (S_ISDIR(st.st_mode)) {
			सचे (remove_dirs(&abs_path, prefix, rm_flags, dry_run, quiet, &gone))
				errors++;
			सचे (gone && !quiet) {
				qname = quote_path(item->string, रिक्त, &buf, 0);
				printf(dry_run ? _(सन्देशः_would_remove) : _(सन्देशः_remove), qname);
			}
		} अञ्ञथा {
			res = dry_run ? 0 : unlink(abs_path.buf);
			सचे (res) {
				पूर्णाङ्क saved_errno = errno;
				qname = quote_path(item->string, रिक्त, &buf, 0);
				errno = saved_errno;
				warning_errno(_(सन्देशः_warn_remove_failed), qname);
				errors++;
			} अञ्ञथा सचे (!quiet) {
				qname = quote_path(item->string, रिक्त, &buf, 0);
				printf(dry_run ? _(सन्देशः_would_remove) : _(सन्देशः_remove), qname);
			}
		}
	}

	strbuf_release(&abs_path);
	strbuf_release(&buf);
	string_list_clear(&del_list, 0);
	string_list_clear(&exclude_list, 0);
	clear_pathspec(&pathspec);
	पटिनिवत्त (errors != 0);
}
