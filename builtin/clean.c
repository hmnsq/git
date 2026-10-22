/*
 * "git clean" builtin command
 *
 * Copyright (C) 2007 Shawn Bohrer
 *
 * Based on git-clean.sh by Pavel Roskin
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "abspath.h"
#समावेश "config.h"
#समावेश "dir.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "parse-options.h"
#समावेश "path.h"
#समावेश "read-cache-ll.h"
#समावेश "setup.h"
#समावेश "string-list.h"
#समावेश "quote.h"
#समावेश "column.h"
#समावेश "color.h"
#समावेश "pathspec.h"
#समावेश "help.h"
#समावेश "prompt.h"

अटल पूर्णांक require_force = -1; /* unset */
अटल पूर्णांक interactive;
अटल काष्ठा string_list del_list = STRING_LIST_INIT_DUP;
अटल अचिन्हित पूर्णांक colopts;

अटल स्थिर अक्षर *स्थिर builtin_clean_usage[] = {
	N_("git clean [-d] [-f] [-i] [-n] [-q] [-e <pattern>] [-x | -X] [--] [<pathspec>...]"),
	शून्य
};

अटल स्थिर अक्षर *संदेश_remove = N_("Removing %s\n");
अटल स्थिर अक्षर *संदेश_would_remove = N_("Would remove %s\n");
अटल स्थिर अक्षर *संदेश_skip_git_dir = N_("Skipping repository %s\n");
अटल स्थिर अक्षर *संदेश_would_skip_git_dir = N_("Would skip repository %s\n");
अटल स्थिर अक्षर *संदेश_warn_remove_failed = N_("failed to remove %s");
अटल स्थिर अक्षर *संदेश_warn_lstat_failed = N_("could not lstat %s\n");
अटल स्थिर अक्षर *संदेश_skip_cwd = N_("Refusing to remove current working directory\n");
अटल स्थिर अक्षर *संदेश_would_skip_cwd = N_("Would refuse to remove current working directory\n");

क्रमागत color_clean {
	CLEAN_COLOR_RESET = 0,
	CLEAN_COLOR_PLAIN = 1,
	CLEAN_COLOR_PROMPT = 2,
	CLEAN_COLOR_HEADER = 3,
	CLEAN_COLOR_HELP = 4,
	CLEAN_COLOR_ERROR = 5
};

अटल स्थिर अक्षर *color_interactive_slots[] = {
	[CLEAN_COLOR_ERROR]  = "error",
	[CLEAN_COLOR_HEADER] = "header",
	[CLEAN_COLOR_HELP]   = "help",
	[CLEAN_COLOR_PLAIN]  = "plain",
	[CLEAN_COLOR_PROMPT] = "prompt",
	[CLEAN_COLOR_RESET]  = "reset",
};

अटल क्रमागत git_colorbool clean_use_color = GIT_COLOR_UNKNOWN;
अटल अक्षर clean_colors[][COLOR_MAXLEN] = {
	[CLEAN_COLOR_ERROR] = GIT_COLOR_BOLD_RED,
	[CLEAN_COLOR_HEADER] = GIT_COLOR_BOLD,
	[CLEAN_COLOR_HELP] = GIT_COLOR_BOLD_RED,
	[CLEAN_COLOR_PLAIN] = GIT_COLOR_NORMAL,
	[CLEAN_COLOR_PROMPT] = GIT_COLOR_BOLD_BLUE,
	[CLEAN_COLOR_RESET] = GIT_COLOR_RESET,
};

#घोषणा MENU_OPTS_SINGLETON		01
#घोषणा MENU_OPTS_IMMEDIATE		02
#घोषणा MENU_OPTS_LIST_ONLY		04

काष्ठा menu_opts {
	स्थिर अक्षर *header;
	स्थिर अक्षर *prompt;
	पूर्णांक flags;
};

#घोषणा MENU_RETURN_NO_LOOP		10

काष्ठा menu_item {
	अक्षर hotkey;
	स्थिर अक्षर *title;
	पूर्णांक selected;
	पूर्णांक (*fn)(व्योम);
};

क्रमागत menu_stuff_type {
	MENU_STUFF_TYPE_STRING_LIST = 1,
	MENU_STUFF_TYPE_MENU_ITEM
};

काष्ठा menu_stuff {
	क्रमागत menu_stuff_type type;
	पूर्णांक nr;
	व्योम *stuff;
};

define_list_config_array(color_interactive_slots);

अटल पूर्णांक git_clean_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
			    स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	स्थिर अक्षर *slot_name;

	अगर (starts_with(var, "column."))
		वापस git_column_config(var, मूल्य, "clean", &colopts);

	/* honors the color.interactive* config variables which also
	   applied in git-add--interactive and git-stash */
	अगर (!म_भेद(var, "color.interactive")) {
		clean_use_color = git_config_colorbool(var, मूल्य);
		वापस 0;
	}
	अगर (skip_prefix(var, "color.interactive.", &slot_name)) {
		पूर्णांक slot = LOOKUP_CONFIG(color_interactive_slots, slot_name);
		अगर (slot < 0)
			वापस 0;
		अगर (!मूल्य)
			वापस config_error_nonbool(var);
		वापस color_parse(मूल्य, clean_colors[slot]);
	}

	अगर (!म_भेद(var, "clean.requireforce")) {
		require_force = git_config_bool(var, मूल्य);
		वापस 0;
	}

	अगर (git_color_config(var, मूल्य, cb) < 0)
		वापस -1;

	वापस git_default_config(var, मूल्य, ctx, cb);
}

अटल स्थिर अक्षर *clean_get_color(क्रमागत color_clean ix)
{
	अगर (want_color(clean_use_color))
		वापस clean_colors[ix];
	वापस "";
}

अटल व्योम clean_print_color(क्रमागत color_clean ix)
{
	म_लिखो("%s", clean_get_color(ix));
}

अटल पूर्णांक exclude_cb(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा string_list *exclude_list = opt->मूल्य;
	BUG_ON_OPT_NEG(unset);
	string_list_append(exclude_list, arg);
	वापस 0;
}

अटल पूर्णांक remove_dirs(काष्ठा strbuf *path, स्थिर अक्षर *prefix, पूर्णांक force_flag,
		पूर्णांक dry_run, पूर्णांक quiet, पूर्णांक *dir_gone)
{
	सूची *dir;
	काष्ठा strbuf quoted = STRBUF_INIT;
	काष्ठा strbuf realpath = STRBUF_INIT;
	काष्ठा strbuf real_ocwd = STRBUF_INIT;
	काष्ठा dirent *e;
	पूर्णांक res = 0, ret = 0, gone = 1, original_len = path->len, len;
	काष्ठा string_list dels = STRING_LIST_INIT_DUP;

	*dir_gone = 1;

	अगर ((force_flag & REMOVE_DIR_KEEP_NESTED_GIT) &&
	    is_nonbare_repository_dir(path)) {
		अगर (!quiet) {
			quote_path(path->buf, prefix, &quoted, 0);
			म_लिखो(dry_run ?  _(संदेश_would_skip_git_dir) : _(संदेश_skip_git_dir),
					quoted.buf);
		}

		*dir_gone = 0;
		जाओ out;
	}

	dir = सूची_खोलो(path->buf);
	अगर (!dir) {
		/* an empty dir could be removed even if it is unreadble */
		res = dry_run ? 0 : सूची_हटाओ(path->buf);
		अगर (res) {
			पूर्णांक saved_errno = त्रुटि_सं;
			quote_path(path->buf, prefix, &quoted, 0);
			त्रुटि_सं = saved_errno;
			warning_errno(_(संदेश_warn_remove_failed), quoted.buf);
			*dir_gone = 0;
		}
		ret = res;
		जाओ out;
	}

	strbuf_complete(path, '/');

	len = path->len;
	जबतक ((e = readdir_skip_dot_and_dotdot(dir)) != शून्य) {
		काष्ठा stat st;

		strbuf_setlen(path, len);
		strbuf_addstr(path, e->d_name);
		अगर (lstat(path->buf, &st))
			warning_errno(_(संदेश_warn_lstat_failed), path->buf);
		अन्यथा अगर (S_ISDIR(st.st_mode)) {
			अगर (remove_dirs(path, prefix, force_flag, dry_run, quiet, &gone))
				ret = 1;
			अगर (gone) {
				quote_path(path->buf, prefix, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} अन्यथा
				*dir_gone = 0;
			जारी;
		} अन्यथा {
			res = dry_run ? 0 : unlink(path->buf);
			अगर (!res) {
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
			जारी;
		}

		/* path too long, stat fails, or non-directory still exists */
		*dir_gone = 0;
		ret = 1;
		अवरोध;
	}
	बंद_सूची(dir);

	strbuf_setlen(path, original_len);

	अगर (*dir_gone) {
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
		 अगर (startup_info->original_cwd)
			 strbuf_realpath(&real_ocwd,
					 startup_info->original_cwd, 1);

		अगर (!strbuf_cmp(&realpath, &real_ocwd)) {
			म_लिखो("%s", dry_run ? _(संदेश_would_skip_cwd) : _(संदेश_skip_cwd));
			*dir_gone = 0;
		} अन्यथा {
			res = dry_run ? 0 : सूची_हटाओ(path->buf);
			अगर (!res)
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

	अगर (!*dir_gone && !quiet) {
		पूर्णांक i;
		क्रम (i = 0; i < dels.nr; i++)
			म_लिखो(dry_run ?  _(संदेश_would_remove) : _(संदेश_remove), dels.वस्तुएँ[i].string);
	}
out:
	strbuf_release(&realpath);
	strbuf_release(&real_ocwd);
	strbuf_release(&quoted);
	string_list_clear(&dels, 0);
	वापस ret;
}

अटल व्योम pretty_print_dels(व्योम)
{
	काष्ठा string_list list = STRING_LIST_INIT_DUP;
	काष्ठा string_list_item *item;
	काष्ठा strbuf buf = STRBUF_INIT;
	स्थिर अक्षर *qname;
	काष्ठा column_options copts;

	for_each_string_list_item(item, &del_list) {
		qname = quote_path(item->string, शून्य, &buf, 0);
		string_list_append(&list, qname);
	}

	/*
	 * always enable column display, we only consult column.*
	 * about layout strategy and stuff
	 */
	colopts = (colopts & ~COL_ENABLE_MASK) | COL_ENABLED;
	स_रखो(&copts, 0, माप(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(&list, colopts, &copts);
	strbuf_release(&buf);
	string_list_clear(&list, 0);
}

अटल व्योम pretty_print_menus(काष्ठा string_list *menu_list)
{
	अचिन्हित पूर्णांक local_colopts = 0;
	काष्ठा column_options copts;

	local_colopts = COL_ENABLED | COL_ROW;
	स_रखो(&copts, 0, माप(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(menu_list, local_colopts, &copts);
}

अटल व्योम prompt_help_cmd(पूर्णांक singleton)
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
अटल व्योम print_highlight_menu_stuff(काष्ठा menu_stuff *stuff, पूर्णांक **chosen)
{
	काष्ठा string_list menu_list = STRING_LIST_INIT_DUP;
	काष्ठा strbuf menu = STRBUF_INIT;
	काष्ठा menu_item *menu_item;
	काष्ठा string_list_item *string_list_item;
	पूर्णांक i;

	चयन (stuff->type) {
	शेष:
		die("Bad type of menu_stuff when print menu");
	हाल MENU_STUFF_TYPE_MENU_ITEM:
		menu_item = (काष्ठा menu_item *)stuff->stuff;
		क्रम (i = 0; i < stuff->nr; i++, menu_item++) {
			स्थिर अक्षर *p;
			पूर्णांक highlighted = 0;

			p = menu_item->title;
			अगर ((*chosen)[i] < 0)
				(*chosen)[i] = menu_item->selected ? 1 : 0;
			strbuf_addf(&menu, "%s%2d: ", (*chosen)[i] ? "*" : " ", i+1);
			क्रम (; *p; p++) {
				अगर (!highlighted && *p == menu_item->hotkey) {
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
		अवरोध;
	हाल MENU_STUFF_TYPE_STRING_LIST:
		i = 0;
		for_each_string_list_item(string_list_item, (काष्ठा string_list *)stuff->stuff) {
			अगर ((*chosen)[i] < 0)
				(*chosen)[i] = 0;
			strbuf_addf(&menu, "%s%2d: %s",
				    (*chosen)[i] ? "*" : " ", i+1, string_list_item->string);
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
			i++;
		}
		अवरोध;
	}

	pretty_print_menus(&menu_list);

	strbuf_release(&menu);
	string_list_clear(&menu_list, 0);
}

अटल पूर्णांक find_unique(स्थिर अक्षर *choice, काष्ठा menu_stuff *menu_stuff)
{
	काष्ठा menu_item *menu_item;
	काष्ठा string_list_item *string_list_item;
	पूर्णांक i, len, found = 0;

	len = म_माप(choice);
	चयन (menu_stuff->type) {
	शेष:
		die("Bad type of menu_stuff when parse choice");
	हाल MENU_STUFF_TYPE_MENU_ITEM:

		menu_item = (काष्ठा menu_item *)menu_stuff->stuff;
		क्रम (i = 0; i < menu_stuff->nr; i++, menu_item++) {
			अगर (len == 1 && *choice == menu_item->hotkey) {
				found = i + 1;
				अवरोध;
			}
			अगर (!strncasecmp(choice, menu_item->title, len)) {
				अगर (found) {
					अगर (len == 1) {
						/* continue for hotkey matching */
						found = -1;
					} अन्यथा {
						found = 0;
						अवरोध;
					}
				} अन्यथा {
					found = i + 1;
				}
			}
		}
		अवरोध;
	हाल MENU_STUFF_TYPE_STRING_LIST:
		string_list_item = ((काष्ठा string_list *)menu_stuff->stuff)->वस्तुएँ;
		क्रम (i = 0; i < menu_stuff->nr; i++, string_list_item++) {
			अगर (!strncasecmp(choice, string_list_item->string, len)) {
				अगर (found) {
					found = 0;
					अवरोध;
				}
				found = i + 1;
			}
		}
		अवरोध;
	}
	वापस found;
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
अटल पूर्णांक parse_choice(काष्ठा menu_stuff *menu_stuff,
			पूर्णांक is_single,
			अक्षर *निवेश,
			पूर्णांक **chosen)
{
	काष्ठा string_list choice = STRING_LIST_INIT_NODUP;
	काष्ठा string_list_item *item;
	पूर्णांक nr = 0;
	पूर्णांक i;

	string_list_split_in_place_f(&choice, निवेश,
				     is_single ? "\n" : ", ", -1,
				     STRING_LIST_SPLIT_TRIM);

	for_each_string_list_item(item, &choice) {
		स्थिर अक्षर *string;
		पूर्णांक choose;
		पूर्णांक bottom = 0, शीर्ष_2 = 0;
		पूर्णांक is_range, is_संख्या;

		string = item->string;
		अगर (!*string)
			जारी;

		/* Input that begins with '-'; unchoose */
		अगर (string[0] == '-') {
			choose = 0;
			string++;
		} अन्यथा {
			choose = 1;
		}

		is_range = 0;
		is_संख्या = 1;
		क्रम (स्थिर अक्षर *p = string; *p; p++) {
			अगर ('-' == *p) {
				अगर (!is_range) {
					is_range = 1;
					is_संख्या = 0;
				} अन्यथा {
					is_संख्या = 0;
					is_range = 0;
					अवरोध;
				}
			} अन्यथा अगर (!है_अंक(*p)) {
				is_संख्या = 0;
				is_range = 0;
				अवरोध;
			}
		}

		अगर (is_संख्या) {
			bottom = म_से_प(string);
			शीर्ष_2 = bottom;
		} अन्यथा अगर (is_range) {
			bottom = म_से_प(string);
			/* a range can be specified like 5-7 or 5- */
			अगर (!*(म_अक्षर(string, '-') + 1))
				शीर्ष_2 = menu_stuff->nr;
			अन्यथा
				शीर्ष_2 = म_से_प(म_अक्षर(string, '-') + 1);
		} अन्यथा अगर (!म_भेद(string, "*")) {
			bottom = 1;
			शीर्ष_2 = menu_stuff->nr;
		} अन्यथा {
			bottom = find_unique(string, menu_stuff);
			शीर्ष_2 = bottom;
		}

		अगर (शीर्ष_2 <= 0 || bottom <= 0 || शीर्ष_2 > menu_stuff->nr || bottom > शीर्ष_2 ||
		    (is_single && bottom != शीर्ष_2)) {
			clean_print_color(CLEAN_COLOR_ERROR);
			म_लिखो(_("Huh (%s)?\n"), string);
			clean_print_color(CLEAN_COLOR_RESET);
			जारी;
		}

		क्रम (i = bottom; i <= शीर्ष_2; i++)
			(*chosen)[i-1] = choose;
	}

	string_list_clear(&choice, 0);

	क्रम (i = 0; i < menu_stuff->nr; i++)
		nr += (*chosen)[i];
	वापस nr;
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
अटल पूर्णांक *list_and_choose(काष्ठा menu_opts *opts, काष्ठा menu_stuff *stuff)
{
	काष्ठा strbuf choice = STRBUF_INIT;
	पूर्णांक *chosen, *परिणाम;
	पूर्णांक nr = 0;
	पूर्णांक eof = 0;
	पूर्णांक i;

	ALLOC_ARRAY(chosen, stuff->nr);
	/* set chosen as uninitialized */
	क्रम (i = 0; i < stuff->nr; i++)
		chosen[i] = -1;

	क्रम (;;) {
		अगर (opts->header) {
			printf_ln("%s%s%s",
				  clean_get_color(CLEAN_COLOR_HEADER),
				  _(opts->header),
				  clean_get_color(CLEAN_COLOR_RESET));
		}

		/* chosen will be initialized by print_highlight_menu_stuff */
		print_highlight_menu_stuff(stuff, &chosen);

		अगर (opts->flags & MENU_OPTS_LIST_ONLY)
			अवरोध;

		अगर (opts->prompt) {
			म_लिखो("%s%s%s%s",
			       clean_get_color(CLEAN_COLOR_PROMPT),
			       _(opts->prompt),
			       opts->flags & MENU_OPTS_SINGLETON ? "> " : ">> ",
			       clean_get_color(CLEAN_COLOR_RESET));
		}

		अगर (git_read_पंक्ति_interactively(&choice) == खातापूर्ण) {
			eof = 1;
			अवरोध;
		}

		/* help for prompt */
		अगर (!म_भेद(choice.buf, "?")) {
			prompt_help_cmd(opts->flags & MENU_OPTS_SINGLETON);
			जारी;
		}

		/* for a multiple-choice menu, press ENTER (empty) will return back */
		अगर (!(opts->flags & MENU_OPTS_SINGLETON) && !choice.len)
			अवरोध;

		nr = parse_choice(stuff,
				  opts->flags & MENU_OPTS_SINGLETON,
				  choice.buf,
				  &chosen);

		अगर (opts->flags & MENU_OPTS_SINGLETON) {
			अगर (nr)
				अवरोध;
		} अन्यथा अगर (opts->flags & MENU_OPTS_IMMEDIATE) {
			अवरोध;
		}
	}

	अगर (eof) {
		परिणाम = xmalloc(माप(पूर्णांक));
		*परिणाम = खातापूर्ण;
	} अन्यथा {
		पूर्णांक j = 0;

		/*
		 * recalculate nr, if return back from menu directly with
		 * default selections.
		 */
		अगर (!nr) {
			क्रम (i = 0; i < stuff->nr; i++)
				nr += chosen[i];
		}

		CALLOC_ARRAY(परिणाम, st_add(nr, 1));
		क्रम (i = 0; i < stuff->nr && j < nr; i++) {
			अगर (chosen[i])
				परिणाम[j++] = i;
		}
		परिणाम[j] = खातापूर्ण;
	}

	मुक्त(chosen);
	strbuf_release(&choice);
	वापस परिणाम;
}

अटल पूर्णांक clean_cmd(व्योम)
{
	वापस MENU_RETURN_NO_LOOP;
}

अटल पूर्णांक filter_by_patterns_cmd(व्योम)
{
	काष्ठा dir_struct dir = DIR_INIT;
	काष्ठा strbuf confirm = STRBUF_INIT;
	काष्ठा pattern_list *pl;
	पूर्णांक changed = -1, i;

	क्रम (;;) {
		काष्ठा string_list ignore_list = STRING_LIST_INIT_NODUP;
		काष्ठा string_list_item *item;

		अगर (!del_list.nr)
			अवरोध;

		अगर (changed)
			pretty_print_dels();

		clean_print_color(CLEAN_COLOR_PROMPT);
		म_लिखो(_("Input ignore patterns>> "));
		clean_print_color(CLEAN_COLOR_RESET);
		अगर (git_read_पंक्ति_interactively(&confirm) == खातापूर्ण)
			अक्षर_दो('\n');

		/* quit filter_by_pattern mode if press ENTER or Ctrl-D */
		अगर (!confirm.len)
			अवरोध;

		pl = add_pattern_list(&dir, EXC_CMDL, "manual exclude");

		string_list_split_in_place_f(&ignore_list, confirm.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);

		क्रम (i = 0; i < ignore_list.nr; i++) {
			item = &ignore_list.वस्तुएँ[i];
			अगर (!*item->string)
				जारी;
			add_pattern(item->string, "", 0, pl, -(i+1));
		}

		changed = 0;
		for_each_string_list_item(item, &del_list) {
			पूर्णांक dtype = DT_UNKNOWN;

			अगर (is_excluded(&dir, the_repository->सूचकांक, item->string, &dtype)) {
				*item->string = '\0';
				changed++;
			}
		}

		अगर (changed) {
			string_list_remove_empty_वस्तुएँ(&del_list, 0);
		} अन्यथा {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf_ln(_("WARNING: Cannot find items matched by: %s"), confirm.buf);
			clean_print_color(CLEAN_COLOR_RESET);
		}

		string_list_clear(&ignore_list, 0);
		dir_clear(&dir);
	}

	strbuf_release(&confirm);
	वापस 0;
}

अटल पूर्णांक select_by_numbers_cmd(व्योम)
{
	काष्ठा menu_opts menu_opts;
	काष्ठा menu_stuff menu_stuff;
	काष्ठा string_list_item *वस्तुएँ;
	पूर्णांक *chosen;
	पूर्णांक i, j;

	menu_opts.header = शून्य;
	menu_opts.prompt = N_("Select items to delete");
	menu_opts.flags = 0;

	menu_stuff.type = MENU_STUFF_TYPE_STRING_LIST;
	menu_stuff.stuff = &del_list;
	menu_stuff.nr = del_list.nr;

	chosen = list_and_choose(&menu_opts, &menu_stuff);
	वस्तुएँ = del_list.वस्तुएँ;
	क्रम (i = 0, j = 0; i < del_list.nr; i++) {
		अगर (i < chosen[j]) {
			*(वस्तुएँ[i].string) = '\0';
		} अन्यथा अगर (i == chosen[j]) {
			/* delete selected item */
			j++;
			जारी;
		} अन्यथा {
			/* end of chosen (chosen[j] == EOF), won't delete */
			*(वस्तुएँ[i].string) = '\0';
		}
	}

	string_list_remove_empty_वस्तुएँ(&del_list, 0);

	मुक्त(chosen);
	वापस 0;
}

अटल पूर्णांक ask_each_cmd(व्योम)
{
	काष्ठा strbuf confirm = STRBUF_INIT;
	काष्ठा strbuf buf = STRBUF_INIT;
	काष्ठा string_list_item *item;
	स्थिर अक्षर *qname;
	पूर्णांक changed = 0, eof = 0;

	for_each_string_list_item(item, &del_list) {
		/* Ctrl-D should stop removing files */
		अगर (!eof) {
			qname = quote_path(item->string, शून्य, &buf, 0);
			/* TRANSLATORS: Make sure to keep [y/N] as is */
			म_लिखो(_("Remove %s [y/N]? "), qname);
			अगर (git_read_पंक्ति_interactively(&confirm) == खातापूर्ण) {
				अक्षर_दो('\n');
				eof = 1;
			}
		}
		अगर (!confirm.len || strncasecmp(confirm.buf, "yes", confirm.len)) {
			*item->string = '\0';
			changed++;
		}
	}

	अगर (changed)
		string_list_remove_empty_वस्तुएँ(&del_list, 0);

	strbuf_release(&buf);
	strbuf_release(&confirm);
	वापस MENU_RETURN_NO_LOOP;
}

अटल पूर्णांक quit_cmd(व्योम)
{
	string_list_clear(&del_list, 0);
	म_लिखो(_("Bye.\n"));
	वापस MENU_RETURN_NO_LOOP;
}

अटल पूर्णांक help_cmd(व्योम)
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
	वापस 0;
}

अटल व्योम interactive_main_loop(व्योम)
{
	जबतक (del_list.nr) {
		काष्ठा menu_opts menu_opts;
		काष्ठा menu_stuff menu_stuff;
		काष्ठा menu_item menus[] = {
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
		menu_stuff.nr = माप(menus) / माप(काष्ठा menu_item);

		clean_print_color(CLEAN_COLOR_HEADER);
		printf_ln(Q_("Would remove the following item:",
			     "Would remove the following items:",
			     del_list.nr));
		clean_print_color(CLEAN_COLOR_RESET);

		pretty_print_dels();

		chosen = list_and_choose(&menu_opts, &menu_stuff);

		अगर (*chosen != खातापूर्ण) {
			पूर्णांक ret;
			ret = menus[*chosen].fn();
			अगर (ret != MENU_RETURN_NO_LOOP) {
				FREE_AND_NULL(chosen);
				अगर (!del_list.nr) {
					clean_print_color(CLEAN_COLOR_ERROR);
					printf_ln(_("No more files to clean, exiting."));
					clean_print_color(CLEAN_COLOR_RESET);
					अवरोध;
				}
				जारी;
			}
		} अन्यथा {
			quit_cmd();
		}

		FREE_AND_NULL(chosen);
		अवरोध;
	}
}

अटल व्योम correct_untracked_entries(काष्ठा dir_struct *dir)
{
	पूर्णांक src, dst, ign;

	क्रम (src = dst = ign = 0; src < dir->nr; src++) {
		/* skip paths in ignored[] that cannot be inside entries[src] */
		जबतक (ign < dir->ignored_nr &&
		       0 <= cmp_dir_entry(&dir->entries[src], &dir->ignored[ign]))
			ign++;

		अगर (ign < dir->ignored_nr &&
		    check_dir_entry_contains(dir->entries[src], dir->ignored[ign])) {
			/* entries[src] contains an ignored path, so we drop it */
			मुक्त(dir->entries[src]);
		} अन्यथा {
			काष्ठा dir_entry *ent = dir->entries[src++];

			/* entries[src] does not contain an ignored path, so we keep it */
			dir->entries[dst++] = ent;

			/* then discard paths in entries[] contained inside entries[src] */
			जबतक (src < dir->nr &&
			       check_dir_entry_contains(ent, dir->entries[src]))
				मुक्त(dir->entries[src++]);

			/* compensate for the outer loop's loop control */
			src--;
		}
	}
	dir->nr = dst;
}

पूर्णांक cmd_clean(पूर्णांक argc,
	      स्थिर अक्षर **argv,
	      स्थिर अक्षर *prefix,
	      काष्ठा repository *repo UNUSED)
{
	पूर्णांक i, res;
	पूर्णांक dry_run = 0, remove_directories = 0, quiet = 0, ignored = 0;
	पूर्णांक ignored_only = 0, force = 0, errors = 0, gone = 1;
	पूर्णांक rm_flags = REMOVE_DIR_KEEP_NESTED_GIT;
	काष्ठा strbuf abs_path = STRBUF_INIT;
	काष्ठा dir_struct dir = DIR_INIT;
	काष्ठा pathspec pathspec;
	काष्ठा strbuf buf = STRBUF_INIT;
	काष्ठा string_list exclude_list = STRING_LIST_INIT_NODUP;
	काष्ठा pattern_list *pl;
	काष्ठा string_list_item *item;
	स्थिर अक्षर *qname;
	काष्ठा option options[] = {
		OPT__QUIET(&quiet, N_("do not print names of files removed")),
		OPT__DRY_RUN(&dry_run, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('i', "interactive", &interactive, N_("interactive cleaning")),
		OPT_BOOL('d', शून्य, &remove_directories,
				N_("remove whole directories")),
		OPT_CALLBACK_F('e', "exclude", &exclude_list, N_("pattern"),
		  N_("add <pattern> to ignore rules"), PARSE_OPT_NONEG, exclude_cb),
		OPT_BOOL('x', शून्य, &ignored, N_("remove ignored files, too")),
		OPT_BOOL('X', शून्य, &ignored_only,
				N_("remove only ignored files")),
		OPT_END()
	};

	repo_config(the_repository, git_clean_config, शून्य);

	argc = parse_options(argc, argv, prefix, options, builtin_clean_usage,
			     0);

	अगर (require_force != 0 && !force && !interactive && !dry_run)
		die(_("clean.requireForce is true and -f not given: refusing to clean"));

	अगर (force > 1)
		rm_flags = 0;
	अन्यथा
		dir.flags |= DIR_SKIP_NESTED_GIT;

	dir.flags |= DIR_SHOW_OTHER_DIRECTORIES;

	अगर (ignored && ignored_only)
		die(_("options '%s' and '%s' cannot be used together"), "-x", "-X");
	अगर (!ignored)
		setup_standard_excludes(&dir);
	अगर (ignored_only)
		dir.flags |= DIR_SHOW_IGNORED;

	अगर (argc) {
		/*
		 * Remaining args implies pathspecs specified, and we should
		 * recurse within those.
		 */
		remove_directories = 1;
	}

	अगर (remove_directories && !ignored_only) {
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
		अगर (!ignored)
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
	the_repository->settings.command_requires_full_सूचकांक = 0;

	अगर (repo_read_सूचकांक(the_repository) < 0)
		die(_("index file corrupt"));

	pl = add_pattern_list(&dir, EXC_CMDL, "--exclude option");
	क्रम (i = 0; i < exclude_list.nr; i++)
		add_pattern(exclude_list.वस्तुएँ[i].string, "", 0, pl, -(i+1));

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       prefix, argv);

	fill_directory(&dir, the_repository->सूचकांक, &pathspec);
	correct_untracked_entries(&dir);

	क्रम (i = 0; i < dir.nr; i++) {
		काष्ठा dir_entry *ent = dir.entries[i];
		काष्ठा stat st;
		स्थिर अक्षर *rel;

		अगर (!सूचकांक_name_is_other(the_repository->सूचकांक, ent->name, ent->len))
			जारी;

		अगर (lstat(ent->name, &st))
			die_errno("Cannot lstat '%s'", ent->name);

		अगर (S_ISDIR(st.st_mode) && !remove_directories)
			जारी;

		rel = relative_path(ent->name, prefix, &buf);
		string_list_append(&del_list, rel);
	}

	dir_clear(&dir);

	अगर (interactive && del_list.nr > 0)
		interactive_main_loop();

	for_each_string_list_item(item, &del_list) {
		काष्ठा stat st;

		strbuf_reset(&abs_path);
		अगर (prefix)
			strbuf_addstr(&abs_path, prefix);

		strbuf_addstr(&abs_path, item->string);

		/*
		 * we might have removed this as part of earlier
		 * recursive directory removal, so lstat() here could
		 * fail with ENOENT.
		 */
		अगर (lstat(abs_path.buf, &st))
			जारी;

		अगर (S_ISDIR(st.st_mode)) {
			अगर (remove_dirs(&abs_path, prefix, rm_flags, dry_run, quiet, &gone))
				errors++;
			अगर (gone && !quiet) {
				qname = quote_path(item->string, शून्य, &buf, 0);
				म_लिखो(dry_run ? _(संदेश_would_remove) : _(संदेश_remove), qname);
			}
		} अन्यथा {
			res = dry_run ? 0 : unlink(abs_path.buf);
			अगर (res) {
				पूर्णांक saved_errno = त्रुटि_सं;
				qname = quote_path(item->string, शून्य, &buf, 0);
				त्रुटि_सं = saved_errno;
				warning_errno(_(संदेश_warn_remove_failed), qname);
				errors++;
			} अन्यथा अगर (!quiet) {
				qname = quote_path(item->string, शून्य, &buf, 0);
				म_लिखो(dry_run ? _(संदेश_would_remove) : _(संदेश_remove), qname);
			}
		}
	}

	strbuf_release(&abs_path);
	strbuf_release(&buf);
	string_list_clear(&del_list, 0);
	string_list_clear(&exclude_list, 0);
	clear_pathspec(&pathspec);
	वापस (errors != 0);
}
