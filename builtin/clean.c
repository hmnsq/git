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

स्थिर पूर्णांक require_force = -1; /* unset */
स्थिर पूर्णांक interactive;
स्थिर संरचना string_list del_list = STRING_LIST_INIT_DUP;
स्थिर अचिन्हित पूर्णांक colopts;

स्थिर स्थिरांक अक्षर *स्थिरांक builtin_clean_usage[] = {
	N_("git clean [-d] [-f] [-i] [-n] [-q] [-e <pattern>] [-x | -X] [--] [<pathspec>...]"),
	रिक्त
};

स्थिर स्थिरांक अक्षर *संदेश_remove = N_("Removing %s\n");
स्थिर स्थिरांक अक्षर *संदेश_would_remove = N_("Would remove %s\n");
स्थिर स्थिरांक अक्षर *संदेश_skip_git_dir = N_("Skipping repository %s\n");
स्थिर स्थिरांक अक्षर *संदेश_would_skip_git_dir = N_("Would skip repository %s\n");
स्थिर स्थिरांक अक्षर *संदेश_warn_remove_failed = N_("failed to remove %s");
स्थिर स्थिरांक अक्षर *संदेश_warn_lstat_failed = N_("could not lstat %s\n");
स्थिर स्थिरांक अक्षर *संदेश_skip_cwd = N_("Refusing to remove current working directory\n");
स्थिर स्थिरांक अक्षर *संदेश_would_skip_cwd = N_("Would refuse to remove current working directory\n");

गणना color_clean {
	CLEAN_COLOR_RESET = 0,
	CLEAN_COLOR_PLAIN = 1,
	CLEAN_COLOR_PROMPT = 2,
	CLEAN_COLOR_HEADER = 3,
	CLEAN_COLOR_HELP = 4,
	CLEAN_COLOR_ERROR = 5
};

स्थिर स्थिरांक अक्षर *color_interactive_slots[] = {
	[CLEAN_COLOR_ERROR]  = "error",
	[CLEAN_COLOR_HEADER] = "header",
	[CLEAN_COLOR_HELP]   = "help",
	[CLEAN_COLOR_PLAIN]  = "plain",
	[CLEAN_COLOR_PROMPT] = "prompt",
	[CLEAN_COLOR_RESET]  = "reset",
};

स्थिर गणना git_colorbool clean_use_color = GIT_COLOR_UNKNOWN;
स्थिर अक्षर clean_colors[][COLOR_MAXLEN] = {
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
	स्थिरांक अक्षर *header;
	स्थिरांक अक्षर *prompt;
	पूर्णांक flags;
};

#define MENU_RETURN_NO_LOOP		10

संरचना menu_item {
	अक्षर hotkey;
	स्थिरांक अक्षर *title;
	पूर्णांक selected;
	पूर्णांक (*fn)(शून्य);
};

गणना menu_stuff_type {
	MENU_STUFF_TYPE_STRING_LIST = 1,
	MENU_STUFF_TYPE_MENU_ITEM
};

संरचना menu_stuff {
	गणना menu_stuff_type type;
	पूर्णांक nr;
	शून्य *stuff;
};

define_list_config_array(color_interactive_slots);

स्थिर पूर्णांक git_clean_config(स्थिरांक अक्षर *var, स्थिरांक अक्षर *मूल्य,
			    स्थिरांक संरचना config_context *ctx, शून्य *cb)
{
	स्थिरांक अक्षर *slot_name;

	जर (starts_with(var, "column."))
		परत git_column_config(var, मूल्य, "clean", &colopts);

	/* honors the color.interactive* config variables which also
	   applied in git-add--interactive and git-stash */
	जर (!म_भेद(var, "color.interactive")) {
		clean_use_color = git_config_colorbool(var, मूल्य);
		परत 0;
	}
	जर (skip_prefix(var, "color.interactive.", &slot_name)) {
		पूर्णांक slot = LOOKUP_CONFIG(color_interactive_slots, slot_name);
		जर (slot < 0)
			परत 0;
		जर (!मूल्य)
			परत config_error_nonbool(var);
		परत color_parse(मूल्य, clean_colors[slot]);
	}

	जर (!म_भेद(var, "clean.requireforce")) {
		require_force = git_config_bool(var, मूल्य);
		परत 0;
	}

	जर (git_color_config(var, मूल्य, cb) < 0)
		परत -1;

	परत git_default_config(var, मूल्य, ctx, cb);
}

स्थिर स्थिरांक अक्षर *clean_get_color(गणना color_clean ix)
{
	जर (want_color(clean_use_color))
		परत clean_colors[ix];
	परत "";
}

स्थिर शून्य clean_print_color(गणना color_clean ix)
{
	म_लिखो("%s", clean_get_color(ix));
}

स्थिर पूर्णांक exclude_cb(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	संरचना string_list *exclude_list = opt->मूल्य;
	BUG_ON_OPT_NEG(unset);
	string_list_append(exclude_list, arg);
	परत 0;
}

स्थिर पूर्णांक remove_dirs(संरचना strbuf *path, स्थिरांक अक्षर *prefix, पूर्णांक force_flag,
		पूर्णांक dry_run, पूर्णांक quiet, पूर्णांक *dir_gone)
{
	सूची *dir;
	संरचना strbuf quoted = STRBUF_INIT;
	संरचना strbuf realpath = STRBUF_INIT;
	संरचना strbuf real_ocwd = STRBUF_INIT;
	संरचना dirent *e;
	पूर्णांक res = 0, ret = 0, gone = 1, original_len = path->len, len;
	संरचना string_list dels = STRING_LIST_INIT_DUP;

	*dir_gone = 1;

	जर ((force_flag & REMOVE_DIR_KEEP_NESTED_GIT) &&
	    is_nonbare_repository_dir(path)) {
		जर (!quiet) {
			quote_path(path->buf, prefix, &quoted, 0);
			म_लिखो(dry_run ?  _(संदेश_would_skip_git_dir) : _(संदेश_skip_git_dir),
					quoted.buf);
		}

		*dir_gone = 0;
		जा out;
	}

	dir = सूची_खोलो(path->buf);
	जर (!dir) {
		/* an empty dir could be removed even if it is unreadble */
		res = dry_run ? 0 : सूची_हटाओ(path->buf);
		जर (res) {
			पूर्णांक saved_errno = त्रुटि_सं;
			quote_path(path->buf, prefix, &quoted, 0);
			त्रुटि_सं = saved_errno;
			warning_errno(_(संदेश_warn_remove_failed), quoted.buf);
			*dir_gone = 0;
		}
		ret = res;
		जा out;
	}

	strbuf_complete(path, '/');

	len = path->len;
	पर्यंत ((e = readdir_skip_dot_and_dotdot(dir)) != रिक्त) {
		संरचना stat st;

		strbuf_setlen(path, len);
		strbuf_addstr(path, e->d_name);
		जर (lstat(path->buf, &st))
			warning_errno(_(संदेश_warn_lstat_failed), path->buf);
		अन्यथा जर (S_ISDIR(st.st_mode)) {
			जर (remove_dirs(path, prefix, force_flag, dry_run, quiet, &gone))
				ret = 1;
			जर (gone) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} अन्यथा
				*dir_gone = 0;
			सुरूठेवा;
		} अन्यथा {
			res = dry_run ? 0 : unlink(path->buf);
			जर (!res) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} अन्यथा {
				पूर्णांक saved_errno = त्रुटि_सं;
				quote_path(path->buf, prefix, &quoted, 0);
				त्रुटि_सं = saved_errno;
				warning_errno(_(संदेश_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
			सुरूठेवा;
		}

		/* path too long, stat fails, or non-directory still exists */
		*dir_gone = 0;
		ret = 1;
		थांबा;
	}
	बंद_सूची(dir);

	strbuf_setlen(path, original_len);

	जर (*dir_gone) {
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
		 जर (startup_info->original_cwd)
			 strbuf_realpath(&real_ocwd,
					 startup_info->original_cwd, 1);

		जर (!strbuf_cmp(&realpath, &real_ocwd)) {
			म_लिखो("%s", dry_run ? _(संदेश_would_skip_cwd) : _(संदेश_skip_cwd));
			*dir_gone = 0;
		} अन्यथा {
			res = dry_run ? 0 : सूची_हटाओ(path->buf);
			जर (!res)
				*dir_gone = 1;
			अन्यथा {
				पूर्णांक saved_errno = त्रुटि_सं;
				quote_path(path->buf, prefix, &quoted, 0);
				त्रुटि_सं = saved_errno;
				warning_errno(_(संदेश_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
		}
	}

	जर (!*dir_gone && !quiet) {
		पूर्णांक i;
		साठी (i = 0; i < dels.nr; i++)
			म_लिखो(dry_run ?  _(संदेश_would_remove) : _(संदेश_remove), dels.वस्तुसंच[i].string);
	}
out:
	strbuf_release(&realpath);
	strbuf_release(&real_ocwd);
	strbuf_release(&quoted);
	string_list_clear(&dels, 0);
	परत ret;
}

स्थिर शून्य pretty_print_dels(शून्य)
{
	संरचना string_list list = STRING_LIST_INIT_DUP;
	संरचना string_list_item *item;
	संरचना strbuf buf = STRBUF_INIT;
	स्थिरांक अक्षर *qname;
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
	स_रखो(&copts, 0, आकार(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(&list, colopts, &copts);
	strbuf_release(&buf);
	string_list_clear(&list, 0);
}

स्थिर शून्य pretty_print_menus(संरचना string_list *menu_list)
{
	अचिन्हित पूर्णांक local_colopts = 0;
	संरचना column_options copts;

	local_colopts = COL_ENABLED | COL_ROW;
	स_रखो(&copts, 0, आकार(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(menu_list, local_colopts, &copts);
}

स्थिर शून्य prompt_help_cmd(पूर्णांक singleton)
{
	clean_print_color(CLEAN_COLOR_HELP);
	म_लिखो(singleton ?
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
स्थिर शून्य print_highlight_menu_stuff(संरचना menu_stuff *stuff, पूर्णांक **chosen)
{
	संरचना string_list menu_list = STRING_LIST_INIT_DUP;
	संरचना strbuf menu = STRBUF_INIT;
	संरचना menu_item *menu_item;
	संरचना string_list_item *string_list_item;
	पूर्णांक i;

	निवड (stuff->type) {
	पूर्वनिर्धारित:
		die("Bad type of menu_stuff when print menu");
	स्थिती MENU_STUFF_TYPE_MENU_ITEM:
		menu_item = (संरचना menu_item *)stuff->stuff;
		साठी (i = 0; i < stuff->nr; i++, menu_item++) {
			स्थिरांक अक्षर *p;
			पूर्णांक highlighted = 0;

			p = menu_item->title;
			जर ((*chosen)[i] < 0)
				(*chosen)[i] = menu_item->selected ? 1 : 0;
			strbuf_addf(&menu, "%s%2d: ", (*chosen)[i] ? "*" : " ", i+1);
			साठी (; *p; p++) {
				जर (!highlighted && *p == menu_item->hotkey) {
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_PROMPT));
					strbuf_addch(&menu, *p);
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_RESET));
					highlighted = 1;
				} अन्यथा {
					strbuf_addch(&menu, *p);
				}
			}
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
		}
		थांबा;
	स्थिती MENU_STUFF_TYPE_STRING_LIST:
		i = 0;
		for_each_string_list_item(string_list_item, (संरचना string_list *)stuff->stuff) {
			जर ((*chosen)[i] < 0)
				(*chosen)[i] = 0;
			strbuf_addf(&menu, "%s%2d: %s",
				    (*chosen)[i] ? "*" : " ", i+1, string_list_item->string);
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
			i++;
		}
		थांबा;
	}

	pretty_print_menus(&menu_list);

	strbuf_release(&menu);
	string_list_clear(&menu_list, 0);
}

स्थिर पूर्णांक find_unique(स्थिरांक अक्षर *choice, संरचना menu_stuff *menu_stuff)
{
	संरचना menu_item *menu_item;
	संरचना string_list_item *string_list_item;
	पूर्णांक i, len, found = 0;

	len = म_माप(choice);
	निवड (menu_stuff->type) {
	पूर्वनिर्धारित:
		die("Bad type of menu_stuff when parse choice");
	स्थिती MENU_STUFF_TYPE_MENU_ITEM:

		menu_item = (संरचना menu_item *)menu_stuff->stuff;
		साठी (i = 0; i < menu_stuff->nr; i++, menu_item++) {
			जर (len == 1 && *choice == menu_item->hotkey) {
				found = i + 1;
				थांबा;
			}
			जर (!strncasecmp(choice, menu_item->title, len)) {
				जर (found) {
					जर (len == 1) {
						/* continue for hotkey matching */
						found = -1;
					} अन्यथा {
						found = 0;
						थांबा;
					}
				} अन्यथा {
					found = i + 1;
				}
			}
		}
		थांबा;
	स्थिती MENU_STUFF_TYPE_STRING_LIST:
		string_list_item = ((संरचना string_list *)menu_stuff->stuff)->वस्तुसंच;
		साठी (i = 0; i < menu_stuff->nr; i++, string_list_item++) {
			जर (!strncasecmp(choice, string_list_item->string, len)) {
				जर (found) {
					found = 0;
					थांबा;
				}
				found = i + 1;
			}
		}
		थांबा;
	}
	परत found;
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
स्थिर पूर्णांक parse_choice(संरचना menu_stuff *menu_stuff,
			पूर्णांक is_single,
			अक्षर *आदान,
			पूर्णांक **chosen)
{
	संरचना string_list choice = STRING_LIST_INIT_NODUP;
	संरचना string_list_item *item;
	पूर्णांक nr = 0;
	पूर्णांक i;

	string_list_split_in_place_f(&choice, आदान,
				     is_single ? "\n" : ", ", -1,
				     STRING_LIST_SPLIT_TRIM);

	for_each_string_list_item(item, &choice) {
		स्थिरांक अक्षर *string;
		पूर्णांक choose;
		पूर्णांक bottom = 0, शीर्ष = 0;
		पूर्णांक is_range, is_संख्या;

		string = item->string;
		जर (!*string)
			सुरूठेवा;

		/* Input that begins with '-'; unchoose */
		जर (string[0] == '-') {
			choose = 0;
			string++;
		} अन्यथा {
			choose = 1;
		}

		is_range = 0;
		is_संख्या = 1;
		साठी (स्थिरांक अक्षर *p = string; *p; p++) {
			जर ('-' == *p) {
				जर (!is_range) {
					is_range = 1;
					is_संख्या = 0;
				} अन्यथा {
					is_संख्या = 0;
					is_range = 0;
					थांबा;
				}
			} अन्यथा जर (!है_अंक(*p)) {
				is_संख्या = 0;
				is_range = 0;
				थांबा;
			}
		}

		जर (is_संख्या) {
			bottom = म_से_प(string);
			शीर्ष = bottom;
		} अन्यथा जर (is_range) {
			bottom = म_से_प(string);
			/* a range can be specified like 5-7 or 5- */
			जर (!*(म_अक्षर(string, '-') + 1))
				शीर्ष = menu_stuff->nr;
			अन्यथा
				शीर्ष = म_से_प(म_अक्षर(string, '-') + 1);
		} अन्यथा जर (!म_भेद(string, "*")) {
			bottom = 1;
			शीर्ष = menu_stuff->nr;
		} अन्यथा {
			bottom = find_unique(string, menu_stuff);
			शीर्ष = bottom;
		}

		जर (शीर्ष <= 0 || bottom <= 0 || शीर्ष > menu_stuff->nr || bottom > शीर्ष ||
		    (is_single && bottom != शीर्ष)) {
			clean_print_color(CLEAN_COLOR_ERROR);
			म_लिखो(_("Huh (%s)?\n"), string);
			clean_print_color(CLEAN_COLOR_RESET);
			सुरूठेवा;
		}

		साठी (i = bottom; i <= शीर्ष; i++)
			(*chosen)[i-1] = choose;
	}

	string_list_clear(&choice, 0);

	साठी (i = 0; i < menu_stuff->nr; i++)
		nr += (*chosen)[i];
	परत nr;
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
स्थिर पूर्णांक *list_and_choose(संरचना menu_opts *opts, संरचना menu_stuff *stuff)
{
	संरचना strbuf choice = STRBUF_INIT;
	पूर्णांक *chosen, *result;
	पूर्णांक nr = 0;
	पूर्णांक eof = 0;
	पूर्णांक i;

	ALLOC_ARRAY(chosen, stuff->nr);
	/* set chosen as uninitialized */
	साठी (i = 0; i < stuff->nr; i++)
		chosen[i] = -1;

	साठी (;;) {
		जर (opts->header) {
			printf_ln("%s%s%s",
				  clean_get_color(CLEAN_COLOR_HEADER),
				  _(opts->header),
				  clean_get_color(CLEAN_COLOR_RESET));
		}

		/* chosen will be initialized by print_highlight_menu_stuff */
		print_highlight_menu_stuff(stuff, &chosen);

		जर (opts->flags & MENU_OPTS_LIST_ONLY)
			थांबा;

		जर (opts->prompt) {
			म_लिखो("%s%s%s%s",
			       clean_get_color(CLEAN_COLOR_PROMPT),
			       _(opts->prompt),
			       opts->flags & MENU_OPTS_SINGLETON ? "> " : ">> ",
			       clean_get_color(CLEAN_COLOR_RESET));
		}

		जर (git_read_ओळ_interactively(&choice) == खातापूर्ण) {
			eof = 1;
			थांबा;
		}

		/* help for prompt */
		जर (!म_भेद(choice.buf, "?")) {
			prompt_help_cmd(opts->flags & MENU_OPTS_SINGLETON);
			सुरूठेवा;
		}

		/* for a multiple-choice menu, press ENTER (empty) will return back */
		जर (!(opts->flags & MENU_OPTS_SINGLETON) && !choice.len)
			थांबा;

		nr = parse_choice(stuff,
				  opts->flags & MENU_OPTS_SINGLETON,
				  choice.buf,
				  &chosen);

		जर (opts->flags & MENU_OPTS_SINGLETON) {
			जर (nr)
				थांबा;
		} अन्यथा जर (opts->flags & MENU_OPTS_IMMEDIATE) {
			थांबा;
		}
	}

	जर (eof) {
		result = xmalloc(आकार(पूर्णांक));
		*result = खातापूर्ण;
	} अन्यथा {
		पूर्णांक j = 0;

		/*
		 * recalculate nr, if return back from menu directly with
		 * default selections.
		 */
		जर (!nr) {
			साठी (i = 0; i < stuff->nr; i++)
				nr += chosen[i];
		}

		CALLOC_ARRAY(result, st_add(nr, 1));
		साठी (i = 0; i < stuff->nr && j < nr; i++) {
			जर (chosen[i])
				result[j++] = i;
		}
		result[j] = खातापूर्ण;
	}

	मुक्त(chosen);
	strbuf_release(&choice);
	परत result;
}

स्थिर पूर्णांक clean_cmd(शून्य)
{
	परत MENU_RETURN_NO_LOOP;
}

स्थिर पूर्णांक filter_by_patterns_cmd(शून्य)
{
	संरचना dir_struct dir = DIR_INIT;
	संरचना strbuf confirm = STRBUF_INIT;
	संरचना pattern_list *pl;
	पूर्णांक changed = -1, i;

	साठी (;;) {
		संरचना string_list ignore_list = STRING_LIST_INIT_NODUP;
		संरचना string_list_item *item;

		जर (!del_list.nr)
			थांबा;

		जर (changed)
			pretty_print_dels();

		clean_print_color(CLEAN_COLOR_PROMPT);
		म_लिखो(_("Input ignore patterns>> "));
		clean_print_color(CLEAN_COLOR_RESET);
		जर (git_read_ओळ_interactively(&confirm) == खातापूर्ण)
			अक्षर_दो('\n');

		/* quit filter_by_pattern mode if press ENTER or Ctrl-D */
		जर (!confirm.len)
			थांबा;

		pl = add_pattern_list(&dir, EXC_CMDL, "manual exclude");

		string_list_split_in_place_f(&ignore_list, confirm.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);

		साठी (i = 0; i < ignore_list.nr; i++) {
			item = &ignore_list.वस्तुसंच[i];
			जर (!*item->string)
				सुरूठेवा;
			add_pattern(item->string, "", 0, pl, -(i+1));
		}

		changed = 0;
		for_each_string_list_item(item, &del_list) {
			पूर्णांक dtype = DT_UNKNOWN;

			जर (is_excluded(&dir, the_repository->index, item->string, &dtype)) {
				*item->string = '\0';
				changed++;
			}
		}

		जर (changed) {
			string_list_remove_empty_वस्तुसंच(&del_list, 0);
		} अन्यथा {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf_ln(_("WARNING: Cannot find items matched by: %s"), confirm.buf);
			clean_print_color(CLEAN_COLOR_RESET);
		}

		string_list_clear(&ignore_list, 0);
		dir_clear(&dir);
	}

	strbuf_release(&confirm);
	परत 0;
}

स्थिर पूर्णांक select_by_numbers_cmd(शून्य)
{
	संरचना menu_opts menu_opts;
	संरचना menu_stuff menu_stuff;
	संरचना string_list_item *वस्तुसंच;
	पूर्णांक *chosen;
	पूर्णांक i, j;

	menu_opts.header = रिक्त;
	menu_opts.prompt = N_("Select items to delete");
	menu_opts.flags = 0;

	menu_stuff.type = MENU_STUFF_TYPE_STRING_LIST;
	menu_stuff.stuff = &del_list;
	menu_stuff.nr = del_list.nr;

	chosen = list_and_choose(&menu_opts, &menu_stuff);
	वस्तुसंच = del_list.वस्तुसंच;
	साठी (i = 0, j = 0; i < del_list.nr; i++) {
		जर (i < chosen[j]) {
			*(वस्तुसंच[i].string) = '\0';
		} अन्यथा जर (i == chosen[j]) {
			/* delete selected item */
			j++;
			सुरूठेवा;
		} अन्यथा {
			/* end of chosen (chosen[j] == EOF), won't delete */
			*(वस्तुसंच[i].string) = '\0';
		}
	}

	string_list_remove_empty_वस्तुसंच(&del_list, 0);

	मुक्त(chosen);
	परत 0;
}

स्थिर पूर्णांक ask_each_cmd(शून्य)
{
	संरचना strbuf confirm = STRBUF_INIT;
	संरचना strbuf buf = STRBUF_INIT;
	संरचना string_list_item *item;
	स्थिरांक अक्षर *qname;
	पूर्णांक changed = 0, eof = 0;

	for_each_string_list_item(item, &del_list) {
		/* Ctrl-D should stop removing files */
		जर (!eof) {
			qname = quote_path(item->string, रिक्त, &buf, 0);
			/* TRANSLATORS: Make sure to keep [y/N] as is */
			म_लिखो(_("Remove %s [y/N]? "), qname);
			जर (git_read_ओळ_interactively(&confirm) == खातापूर्ण) {
				अक्षर_दो('\n');
				eof = 1;
			}
		}
		जर (!confirm.len || strncasecmp(confirm.buf, "yes", confirm.len)) {
			*item->string = '\0';
			changed++;
		}
	}

	जर (changed)
		string_list_remove_empty_वस्तुसंच(&del_list, 0);

	strbuf_release(&buf);
	strbuf_release(&confirm);
	परत MENU_RETURN_NO_LOOP;
}

स्थिर पूर्णांक quit_cmd(शून्य)
{
	string_list_clear(&del_list, 0);
	म_लिखो(_("Bye.\n"));
	परत MENU_RETURN_NO_LOOP;
}

स्थिर पूर्णांक help_cmd(शून्य)
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
	परत 0;
}

स्थिर शून्य interactive_main_loop(शून्य)
{
	पर्यंत (del_list.nr) {
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
		पूर्णांक *chosen;

		menu_opts.header = N_("*** Commands ***");
		menu_opts.prompt = N_("What now");
		menu_opts.flags = MENU_OPTS_SINGLETON;

		menu_stuff.type = MENU_STUFF_TYPE_MENU_ITEM;
		menu_stuff.stuff = menus;
		menu_stuff.nr = आकार(menus) / आकार(संरचना menu_item);

		clean_print_color(CLEAN_COLOR_HEADER);
		printf_ln(Q_("Would remove the following item:",
			     "Would remove the following items:",
			     del_list.nr));
		clean_print_color(CLEAN_COLOR_RESET);

		pretty_print_dels();

		chosen = list_and_choose(&menu_opts, &menu_stuff);

		जर (*chosen != खातापूर्ण) {
			पूर्णांक ret;
			ret = menus[*chosen].fn();
			जर (ret != MENU_RETURN_NO_LOOP) {
				FREE_AND_NULL(chosen);
				जर (!del_list.nr) {
					clean_print_color(CLEAN_COLOR_ERROR);
					printf_ln(_("No more files to clean, exiting."));
					clean_print_color(CLEAN_COLOR_RESET);
					थांबा;
				}
				सुरूठेवा;
			}
		} अन्यथा {
			quit_cmd();
		}

		FREE_AND_NULL(chosen);
		थांबा;
	}
}

स्थिर शून्य correct_untracked_entries(संरचना dir_struct *dir)
{
	पूर्णांक src, dst, ign;

	साठी (src = dst = ign = 0; src < dir->nr; src++) {
		/* skip paths in ignored[] that cannot be inside entries[src] */
		पर्यंत (ign < dir->ignored_nr &&
		       0 <= cmp_dir_entry(&dir->entries[src], &dir->ignored[ign]))
			ign++;

		जर (ign < dir->ignored_nr &&
		    check_dir_entry_contains(dir->entries[src], dir->ignored[ign])) {
			/* entries[src] contains an ignored path, so we drop it */
			मुक्त(dir->entries[src]);
		} अन्यथा {
			संरचना dir_entry *ent = dir->entries[src++];

			/* entries[src] does not contain an ignored path, so we keep it */
			dir->entries[dst++] = ent;

			/* then discard paths in entries[] contained inside entries[src] */
			पर्यंत (src < dir->nr &&
			       check_dir_entry_contains(ent, dir->entries[src]))
				मुक्त(dir->entries[src++]);

			/* compensate for the outer loop's loop control */
			src--;
		}
	}
	dir->nr = dst;
}

पूर्णांक cmd_clean(पूर्णांक argc,
	      स्थिरांक अक्षर **argv,
	      स्थिरांक अक्षर *prefix,
	      संरचना repository *repo UNUSED)
{
	पूर्णांक i, res;
	पूर्णांक dry_run = 0, remove_directories = 0, quiet = 0, ignored = 0;
	पूर्णांक ignored_only = 0, force = 0, errors = 0, gone = 1;
	पूर्णांक rm_flags = REMOVE_DIR_KEEP_NESTED_GIT;
	संरचना strbuf abs_path = STRBUF_INIT;
	संरचना dir_struct dir = DIR_INIT;
	संरचना pathspec pathspec;
	संरचना strbuf buf = STRBUF_INIT;
	संरचना string_list exclude_list = STRING_LIST_INIT_NODUP;
	संरचना pattern_list *pl;
	संरचना string_list_item *item;
	स्थिरांक अक्षर *qname;
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

	जर (require_force != 0 && !force && !interactive && !dry_run)
		die(_("clean.requireForce is true and -f not given: refusing to clean"));

	जर (force > 1)
		rm_flags = 0;
	अन्यथा
		dir.flags |= DIR_SKIP_NESTED_GIT;

	dir.flags |= DIR_SHOW_OTHER_DIRECTORIES;

	जर (ignored && ignored_only)
		die(_("options '%s' and '%s' cannot be used together"), "-x", "-X");
	जर (!ignored)
		setup_standard_excludes(&dir);
	जर (ignored_only)
		dir.flags |= DIR_SHOW_IGNORED;

	जर (argc) {
		/*
		 * Remaining args implies pathspecs specified, and we should
		 * recurse within those.
		 */
		remove_directories = 1;
	}

	जर (remove_directories && !ignored_only) {
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
		जर (!ignored)
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

	जर (repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	साठी (i = 0; i < exclude_list.nr; i++)
		add_pattern(exclude_list.वस्तुसंच[i].string, "", 0, pl, -(i+1));

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	fill_directory(&dir, the_repository->index, &pathspec);
	correct_untracked_entries(&dir);

	साठी (i = 0; i < dir.nr; i++) {
		संरचना dir_entry *ent = dir.entries[i];
		संरचना stat st;
		स्थिरांक अक्षर *rel;

		जर (!index_name_is_other(the_repository->index, ent->name, ent->len))
			सुरूठेवा;

		जर (lstat(ent->name, &st))
			die_errno("Cannot lstat '%s'", ent->name);

		जर (S_ISDIR(st.st_mode) && !remove_directories)
			सुरूठेवा;

		rel = relative_path(ent->name, prefix, &buf);
		string_list_append(&del_list, rel);
	}

	dir_clear(&dir);

	जर (interactive && del_list.nr > 0)
		interactive_main_loop();

	for_each_string_list_item(item, &del_list) {
		संरचना stat st;

		strbuf_reset(&abs_path);
		जर (prefix)
			strbuf_addstr(&abs_path, prefix);

		strbuf_addstr(&abs_path, item->string);

		/*
		 * we might have removed this as part of earlier
		 * recursive directory removal, so lstat() here could
		 * fail with ENOENT.
		 */
		जर (lstat(abs_path.buf, &st))
			सुरूठेवा;

		जर (S_ISDIR(st.st_mode)) {
			जर (remove_dirs(&abs_path, prefix, rm_flags, dry_run, quiet, &gone))
				errors++;
			जर (gone && !quiet) {
				qname = quote_path(item->string, रिक्त, &buf, 0);
				म_लिखो(dry_run ? _(संदेश_would_remove) : _(संदेश_remove), qname);
			}
		} अन्यथा {
			res = dry_run ? 0 : unlink(abs_path.buf);
			जर (res) {
				पूर्णांक saved_errno = त्रुटि_सं;
				qname = quote_path(item->string, रिक्त, &buf, 0);
				त्रुटि_सं = saved_errno;
				warning_errno(_(संदेश_warn_remove_failed), qname);
				errors++;
			} अन्यथा जर (!quiet) {
				qname = quote_path(item->string, रिक्त, &buf, 0);
				म_लिखो(dry_run ? _(संदेश_would_remove) : _(संदेश_remove), qname);
			}
		}
	}

	strbuf_release(&abs_path);
	strbuf_release(&buf);
	string_list_clear(&del_list, 0);
	string_list_clear(&exclude_list, 0);
	clear_pathspec(&pathspec);
	परत (errors != 0);
}
