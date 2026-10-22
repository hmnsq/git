/*
 * "git clean" builtin command
 *
 * Copyright (C) 2007 Shawn Bohrer
 *
 * Based on git-clean.sh by Pavel Roskin
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"
#అన్తర్భావ "abspath.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "read-cache-ll.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "column.h"
#అన్తర్భావ "color.h"
#అన్తర్భావ "pathspec.h"
#అన్తర్భావ "help.h"
#అన్తర్భావ "prompt.h"

స్థిర పూర్ణాంకం require_force = -1; /* unset */
స్థిర పూర్ణాంకం interactive;
స్థిర నిర్మాణం string_list del_list = STRING_LIST_INIT_DUP;
స్థిర గుర్తులేని పూర్ణాంకం colopts;

స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి builtin_clean_usage[] = {
	N_("git clean [-d] [-f] [-i] [-n] [-q] [-e <pattern>] [-x | -X] [--] [<pathspec>...]"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం *సందేశం_remove = N_("Removing %s\n");
స్థిర స్థిరరాశి అక్షరం *సందేశం_would_remove = N_("Would remove %s\n");
స్థిర స్థిరరాశి అక్షరం *సందేశం_skip_git_dir = N_("Skipping repository %s\n");
స్థిర స్థిరరాశి అక్షరం *సందేశం_would_skip_git_dir = N_("Would skip repository %s\n");
స్థిర స్థిరరాశి అక్షరం *సందేశం_warn_remove_failed = N_("failed to remove %s");
స్థిర స్థిరరాశి అక్షరం *సందేశం_warn_lstat_failed = N_("could not lstat %s\n");
స్థిర స్థిరరాశి అక్షరం *సందేశం_skip_cwd = N_("Refusing to remove current working directory\n");
స్థిర స్థిరరాశి అక్షరం *సందేశం_would_skip_cwd = N_("Would refuse to remove current working directory\n");

గణన color_clean {
	CLEAN_COLOR_RESET = 0,
	CLEAN_COLOR_PLAIN = 1,
	CLEAN_COLOR_PROMPT = 2,
	CLEAN_COLOR_శీర్షిక = 3,
	CLEAN_COLOR_HELP = 4,
	CLEAN_COLOR_ERROR = 5
};

స్థిర స్థిరరాశి అక్షరం *color_interactive_slots[] = {
	[CLEAN_COLOR_ERROR]  = "error",
	[CLEAN_COLOR_శీర్షిక] = "header",
	[CLEAN_COLOR_HELP]   = "help",
	[CLEAN_COLOR_PLAIN]  = "plain",
	[CLEAN_COLOR_PROMPT] = "prompt",
	[CLEAN_COLOR_RESET]  = "reset",
};

స్థిర గణన git_colorbool clean_use_color = GIT_COLOR_UNKNOWN;
స్థిర అక్షరం clean_colors[][COLOR_MAXLEN] = {
	[CLEAN_COLOR_ERROR] = GIT_COLOR_BOLD_RED,
	[CLEAN_COLOR_శీర్షిక] = GIT_COLOR_BOLD,
	[CLEAN_COLOR_HELP] = GIT_COLOR_BOLD_RED,
	[CLEAN_COLOR_PLAIN] = GIT_COLOR_సాధారణ,
	[CLEAN_COLOR_PROMPT] = GIT_COLOR_BOLD_BLUE,
	[CLEAN_COLOR_RESET] = GIT_COLOR_RESET,
};

#నిర్వచించు MENU_OPTS_SINGLETON		01
#నిర్వచించు MENU_OPTS_తక్షణ		02
#నిర్వచించు MENU_OPTS_LIST_ONLY		04

నిర్మాణం menu_opts {
	స్థిరరాశి అక్షరం *శీర్షిక;
	స్థిరరాశి అక్షరం *prompt;
	పూర్ణాంకం flags;
};

#నిర్వచించు MENU_RETURN_NO_LOOP		10

నిర్మాణం menu_అంశం {
	అక్షరం hotkey;
	స్థిరరాశి అక్షరం *title;
	పూర్ణాంకం selected;
	పూర్ణాంకం (*fn)(శూన్య);
};

గణన menu_stuff_type {
	MENU_STUFF_TYPE_STRING_LIST = 1,
	MENU_STUFF_TYPE_MENU_అంశం
};

నిర్మాణం menu_stuff {
	గణన menu_stuff_type type;
	పూర్ణాంకం nr;
	శూన్య *stuff;
};

define_list_config_array(color_interactive_slots);

స్థిర పూర్ణాంకం git_clean_config(స్థిరరాశి అక్షరం *var, స్థిరరాశి అక్షరం *విలువ,
			    స్థిరరాశి నిర్మాణం config_context *ctx, శూన్య *cb)
{
	స్థిరరాశి అక్షరం *స్థానం_name;

	ఒకవేళ (starts_with(var, "column."))
		తిరిగి git_స్తంభం_config(var, విలువ, "clean", &colopts);

	/* honors the color.interactive* config variables which also
	   applied in git-add--interactive and git-stash */
	ఒకవేళ (!strcmp(var, "color.interactive")) {
		clean_use_color = git_config_colorbool(var, విలువ);
		తిరిగి 0;
	}
	ఒకవేళ (skip_ఉపసర్గ(var, "color.interactive.", &స్థానం_name)) {
		పూర్ణాంకం స్థానం = LOOKUP_CONFIG(color_interactive_slots, స్థానం_name);
		ఒకవేళ (స్థానం < 0)
			తిరిగి 0;
		ఒకవేళ (!విలువ)
			తిరిగి config_error_nonbool(var);
		తిరిగి color_parse(విలువ, clean_colors[స్థానం]);
	}

	ఒకవేళ (!strcmp(var, "clean.requireforce")) {
		require_force = git_config_bool(var, విలువ);
		తిరిగి 0;
	}

	ఒకవేళ (git_color_config(var, విలువ, cb) < 0)
		తిరిగి -1;

	తిరిగి git_default_config(var, విలువ, ctx, cb);
}

స్థిర స్థిరరాశి అక్షరం *clean_get_color(గణన color_clean ix)
{
	ఒకవేళ (want_color(clean_use_color))
		తిరిగి clean_colors[ix];
	తిరిగి "";
}

స్థిర శూన్య clean_print_color(గణన color_clean ix)
{
	రాయి("%s", clean_get_color(ix));
}

స్థిర పూర్ణాంకం exclude_cb(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం string_list *exclude_list = opt->విలువ;
	BUG_ON_OPT_NEG(unset);
	string_list_append(exclude_list, arg);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం remove_dirs(నిర్మాణం strbuf *మార్గం, స్థిరరాశి అక్షరం *ఉపసర్గ, పూర్ణాంకం force_flag,
		పూర్ణాంకం dry_నిరంతరం, పూర్ణాంకం quiet, పూర్ణాంకం *dir_gone)
{
	DIR *dir;
	నిర్మాణం strbuf quoted = STRBUF_INIT;
	నిర్మాణం strbuf realpath = STRBUF_INIT;
	నిర్మాణం strbuf real_ocwd = STRBUF_INIT;
	నిర్మాణం dirent *e;
	పూర్ణాంకం res = 0, ret = 0, gone = 1, original_len = మార్గం->len, len;
	నిర్మాణం string_list dels = STRING_LIST_INIT_DUP;

	*dir_gone = 1;

	ఒకవేళ ((force_flag & REMOVE_DIR_KEEP_NESTED_GIT) &&
	    is_nonbare_repository_dir(మార్గం)) {
		ఒకవేళ (!quiet) {
			quote_మార్గం(మార్గం->buf, ఉపసర్గ, &quoted, 0);
			రాయి(dry_నిరంతరం ?  _(సందేశం_would_skip_git_dir) : _(సందేశం_skip_git_dir),
					quoted.buf);
		}

		*dir_gone = 0;
		వెళ్ళు బహిర్గతం;
	}

	dir = opendir(మార్గం->buf);
	ఒకవేళ (!dir) {
		/* an empty dir could be removed even if it is unreadble */
		res = dry_నిరంతరం ? 0 : rmdir(మార్గం->buf);
		ఒకవేళ (res) {
			పూర్ణాంకం saved_errno = errno;
			quote_మార్గం(మార్గం->buf, ఉపసర్గ, &quoted, 0);
			errno = saved_errno;
			warning_errno(_(సందేశం_warn_remove_failed), quoted.buf);
			*dir_gone = 0;
		}
		ret = res;
		వెళ్ళు బహిర్గతం;
	}

	strbuf_complete(మార్గం, '/');

	len = మార్గం->len;
	అయితే ((e = readdir_skip_dot_and_dotdot(dir)) != ఖాళీ) {
		నిర్మాణం stat st;

		strbuf_setlen(మార్గం, len);
		strbuf_addstr(మార్గం, e->d_name);
		ఒకవేళ (lstat(మార్గం->buf, &st))
			warning_errno(_(సందేశం_warn_lstat_failed), మార్గం->buf);
		లేకపోతే ఒకవేళ (S_ISDIR(st.st_బహుళకం)) {
			ఒకవేళ (remove_dirs(మార్గం, ఉపసర్గ, force_flag, dry_నిరంతరం, quiet, &gone))
				ret = 1;
			ఒకవేళ (gone) {
				quote_మార్గం(మార్గం->buf, ఉపసర్గ, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} లేకపోతే
				*dir_gone = 0;
			కొనసాగు;
		} లేకపోతే {
			res = dry_నిరంతరం ? 0 : unlink(మార్గం->buf);
			ఒకవేళ (!res) {
				quote_మార్గం(మార్గం->buf, ఉపసర్గ, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} లేకపోతే {
				పూర్ణాంకం saved_errno = errno;
				quote_మార్గం(మార్గం->buf, ఉపసర్గ, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(సందేశం_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
			కొనసాగు;
		}

		/* path too long, stat fails, or non-directory still exists */
		*dir_gone = 0;
		ret = 1;
		ఆపు;
	}
	closedir(dir);

	strbuf_setlen(మార్గం, original_len);

	ఒకవేళ (*dir_gone) {
		/*
		 * Normalize path components in path->buf, e.g. change '\' to
		 * '/' on Windows.
		 */
		strbuf_realpath(&realpath, మార్గం->buf, 1);

		/*
		 * path and realpath are absolute; for comparison, we would
		 * like to transform startup_info->original_cwd to an absolute
		 * path too.
		 */
		 ఒకవేళ (startup_info->original_cwd)
			 strbuf_realpath(&real_ocwd,
					 startup_info->original_cwd, 1);

		ఒకవేళ (!strbuf_cmp(&realpath, &real_ocwd)) {
			రాయి("%s", dry_నిరంతరం ? _(సందేశం_would_skip_cwd) : _(సందేశం_skip_cwd));
			*dir_gone = 0;
		} లేకపోతే {
			res = dry_నిరంతరం ? 0 : rmdir(మార్గం->buf);
			ఒకవేళ (!res)
				*dir_gone = 1;
			లేకపోతే {
				పూర్ణాంకం saved_errno = errno;
				quote_మార్గం(మార్గం->buf, ఉపసర్గ, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(సందేశం_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
		}
	}

	ఒకవేళ (!*dir_gone && !quiet) {
		పూర్ణాంకం i;
		కొరకు (i = 0; i < dels.nr; i++)
			రాయి(dry_నిరంతరం ?  _(సందేశం_would_remove) : _(సందేశం_remove), dels.వస్తువులు[i].string);
	}
బహిర్గతం:
	strbuf_release(&realpath);
	strbuf_release(&real_ocwd);
	strbuf_release(&quoted);
	string_list_clear(&dels, 0);
	తిరిగి ret;
}

స్థిర శూన్య pretty_print_dels(శూన్య)
{
	నిర్మాణం string_list list = STRING_LIST_INIT_DUP;
	నిర్మాణం string_list_అంశం *అంశం;
	నిర్మాణం strbuf buf = STRBUF_INIT;
	స్థిరరాశి అక్షరం *qname;
	నిర్మాణం స్తంభం_options copts;

	for_each_string_list_అంశం(అంశం, &del_list) {
		qname = quote_మార్గం(అంశం->string, ఖాళీ, &buf, 0);
		string_list_append(&list, qname);
	}

	/*
	 * always enable column display, we only consult column.*
	 * about layout strategy and stuff
	 */
	colopts = (colopts & ~నిలువు_ENABLE_ముసుగు) | నిలువు_ENABLED;
	memset(&copts, 0, పరిమాణం(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(&list, colopts, &copts);
	strbuf_release(&buf);
	string_list_clear(&list, 0);
}

స్థిర శూన్య pretty_print_menus(నిర్మాణం string_list *menu_list)
{
	గుర్తులేని పూర్ణాంకం local_colopts = 0;
	నిర్మాణం స్తంభం_options copts;

	local_colopts = నిలువు_ENABLED | నిలువు_అడ్డువరుస;
	memset(&copts, 0, పరిమాణం(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(menu_list, local_colopts, &copts);
}

స్థిర శూన్య prompt_help_cmd(పూర్ణాంకం singleton)
{
	clean_print_color(CLEAN_COLOR_HELP);
	రాయి(singleton ?
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
స్థిర శూన్య print_highlight_menu_stuff(నిర్మాణం menu_stuff *stuff, పూర్ణాంకం **chosen)
{
	నిర్మాణం string_list menu_list = STRING_LIST_INIT_DUP;
	నిర్మాణం strbuf menu = STRBUF_INIT;
	నిర్మాణం menu_అంశం *menu_అంశం;
	నిర్మాణం string_list_అంశం *string_list_అంశం;
	పూర్ణాంకం i;

	ఎంపిక (stuff->type) {
	అప్రమేయ:
		die("Bad type of menu_stuff when print menu");
	స్థితి MENU_STUFF_TYPE_MENU_అంశం:
		menu_అంశం = (నిర్మాణం menu_అంశం *)stuff->stuff;
		కొరకు (i = 0; i < stuff->nr; i++, menu_అంశం++) {
			స్థిరరాశి అక్షరం *p;
			పూర్ణాంకం highlighted = 0;

			p = menu_అంశం->title;
			ఒకవేళ ((*chosen)[i] < 0)
				(*chosen)[i] = menu_అంశం->selected ? 1 : 0;
			strbuf_addf(&menu, "%s%2d: ", (*chosen)[i] ? "*" : " ", i+1);
			కొరకు (; *p; p++) {
				ఒకవేళ (!highlighted && *p == menu_అంశం->hotkey) {
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_PROMPT));
					strbuf_addch(&menu, *p);
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_RESET));
					highlighted = 1;
				} లేకపోతే {
					strbuf_addch(&menu, *p);
				}
			}
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
		}
		ఆపు;
	స్థితి MENU_STUFF_TYPE_STRING_LIST:
		i = 0;
		for_each_string_list_అంశం(string_list_అంశం, (నిర్మాణం string_list *)stuff->stuff) {
			ఒకవేళ ((*chosen)[i] < 0)
				(*chosen)[i] = 0;
			strbuf_addf(&menu, "%s%2d: %s",
				    (*chosen)[i] ? "*" : " ", i+1, string_list_అంశం->string);
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
			i++;
		}
		ఆపు;
	}

	pretty_print_menus(&menu_list);

	strbuf_release(&menu);
	string_list_clear(&menu_list, 0);
}

స్థిర పూర్ణాంకం కనుగొను_unique(స్థిరరాశి అక్షరం *choice, నిర్మాణం menu_stuff *menu_stuff)
{
	నిర్మాణం menu_అంశం *menu_అంశం;
	నిర్మాణం string_list_అంశం *string_list_అంశం;
	పూర్ణాంకం i, len, దొరికింది_2 = 0;

	len = అక్షరశ్రేణి_పొడవు(choice);
	ఎంపిక (menu_stuff->type) {
	అప్రమేయ:
		die("Bad type of menu_stuff when parse choice");
	స్థితి MENU_STUFF_TYPE_MENU_అంశం:

		menu_అంశం = (నిర్మాణం menu_అంశం *)menu_stuff->stuff;
		కొరకు (i = 0; i < menu_stuff->nr; i++, menu_అంశం++) {
			ఒకవేళ (len == 1 && *choice == menu_అంశం->hotkey) {
				దొరికింది_2 = i + 1;
				ఆపు;
			}
			ఒకవేళ (!strncasecmp(choice, menu_అంశం->title, len)) {
				ఒకవేళ (దొరికింది_2) {
					ఒకవేళ (len == 1) {
						/* continue for hotkey matching */
						దొరికింది_2 = -1;
					} లేకపోతే {
						దొరికింది_2 = 0;
						ఆపు;
					}
				} లేకపోతే {
					దొరికింది_2 = i + 1;
				}
			}
		}
		ఆపు;
	స్థితి MENU_STUFF_TYPE_STRING_LIST:
		string_list_అంశం = ((నిర్మాణం string_list *)menu_stuff->stuff)->వస్తువులు;
		కొరకు (i = 0; i < menu_stuff->nr; i++, string_list_అంశం++) {
			ఒకవేళ (!strncasecmp(choice, string_list_అంశం->string, len)) {
				ఒకవేళ (దొరికింది_2) {
					దొరికింది_2 = 0;
					ఆపు;
				}
				దొరికింది_2 = i + 1;
			}
		}
		ఆపు;
	}
	తిరిగి దొరికింది_2;
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
స్థిర పూర్ణాంకం parse_choice(నిర్మాణం menu_stuff *menu_stuff,
			పూర్ణాంకం is_single,
			అక్షరం *ఆదానం,
			పూర్ణాంకం **chosen)
{
	నిర్మాణం string_list choice = STRING_LIST_INIT_NODUP;
	నిర్మాణం string_list_అంశం *అంశం;
	పూర్ణాంకం nr = 0;
	పూర్ణాంకం i;

	string_list_విభజించు_లోపల_ఉంచు_f(&choice, ఆదానం,
				     is_single ? "\n" : ", ", -1,
				     STRING_LIST_విభజించు_TRIM);

	for_each_string_list_అంశం(అంశం, &choice) {
		స్థిరరాశి అక్షరం *string;
		పూర్ణాంకం ఎంచుకో;
		పూర్ణాంకం bottom = 0, శిఖరం = 0;
		పూర్ణాంకం is_range, is_సంఖ్య;

		string = అంశం->string;
		ఒకవేళ (!*string)
			కొనసాగు;

		/* Input that begins with '-'; unchoose */
		ఒకవేళ (string[0] == '-') {
			ఎంచుకో = 0;
			string++;
		} లేకపోతే {
			ఎంచుకో = 1;
		}

		is_range = 0;
		is_సంఖ్య = 1;
		కొరకు (స్థిరరాశి అక్షరం *p = string; *p; p++) {
			ఒకవేళ ('-' == *p) {
				ఒకవేళ (!is_range) {
					is_range = 1;
					is_సంఖ్య = 0;
				} లేకపోతే {
					is_సంఖ్య = 0;
					is_range = 0;
					ఆపు;
				}
			} లేకపోతే ఒకవేళ (!అంకెనా(*p)) {
				is_సంఖ్య = 0;
				is_range = 0;
				ఆపు;
			}
		}

		ఒకవేళ (is_సంఖ్య) {
			bottom = atoi(string);
			శిఖరం = bottom;
		} లేకపోతే ఒకవేళ (is_range) {
			bottom = atoi(string);
			/* a range can be specified like 5-7 or 5- */
			ఒకవేళ (!*(strchr(string, '-') + 1))
				శిఖరం = menu_stuff->nr;
			లేకపోతే
				శిఖరం = atoi(strchr(string, '-') + 1);
		} లేకపోతే ఒకవేళ (!strcmp(string, "*")) {
			bottom = 1;
			శిఖరం = menu_stuff->nr;
		} లేకపోతే {
			bottom = కనుగొను_unique(string, menu_stuff);
			శిఖరం = bottom;
		}

		ఒకవేళ (శిఖరం <= 0 || bottom <= 0 || శిఖరం > menu_stuff->nr || bottom > శిఖరం ||
		    (is_single && bottom != శిఖరం)) {
			clean_print_color(CLEAN_COLOR_ERROR);
			రాయి(_("Huh (%s)?\n"), string);
			clean_print_color(CLEAN_COLOR_RESET);
			కొనసాగు;
		}

		కొరకు (i = bottom; i <= శిఖరం; i++)
			(*chosen)[i-1] = ఎంచుకో;
	}

	string_list_clear(&choice, 0);

	కొరకు (i = 0; i < menu_stuff->nr; i++)
		nr += (*chosen)[i];
	తిరిగి nr;
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
స్థిర పూర్ణాంకం *list_and_ఎంచుకో(నిర్మాణం menu_opts *opts, నిర్మాణం menu_stuff *stuff)
{
	నిర్మాణం strbuf choice = STRBUF_INIT;
	పూర్ణాంకం *chosen, *ఫలితం;
	పూర్ణాంకం nr = 0;
	పూర్ణాంకం eof = 0;
	పూర్ణాంకం i;

	ALLOC_ARRAY(chosen, stuff->nr);
	/* set chosen as uninitialized */
	కొరకు (i = 0; i < stuff->nr; i++)
		chosen[i] = -1;

	కొరకు (;;) {
		ఒకవేళ (opts->శీర్షిక) {
			printf_ln("%s%s%s",
				  clean_get_color(CLEAN_COLOR_శీర్షిక),
				  _(opts->శీర్షిక),
				  clean_get_color(CLEAN_COLOR_RESET));
		}

		/* chosen will be initialized by print_highlight_menu_stuff */
		print_highlight_menu_stuff(stuff, &chosen);

		ఒకవేళ (opts->flags & MENU_OPTS_LIST_ONLY)
			ఆపు;

		ఒకవేళ (opts->prompt) {
			రాయి("%s%s%s%s",
			       clean_get_color(CLEAN_COLOR_PROMPT),
			       _(opts->prompt),
			       opts->flags & MENU_OPTS_SINGLETON ? "> " : ">> ",
			       clean_get_color(CLEAN_COLOR_RESET));
		}

		ఒకవేళ (git_read_పంక్తి_interactively(&choice) == EOF) {
			eof = 1;
			ఆపు;
		}

		/* help for prompt */
		ఒకవేళ (!strcmp(choice.buf, "?")) {
			prompt_help_cmd(opts->flags & MENU_OPTS_SINGLETON);
			కొనసాగు;
		}

		/* for a multiple-choice menu, press ENTER (empty) will return back */
		ఒకవేళ (!(opts->flags & MENU_OPTS_SINGLETON) && !choice.len)
			ఆపు;

		nr = parse_choice(stuff,
				  opts->flags & MENU_OPTS_SINGLETON,
				  choice.buf,
				  &chosen);

		ఒకవేళ (opts->flags & MENU_OPTS_SINGLETON) {
			ఒకవేళ (nr)
				ఆపు;
		} లేకపోతే ఒకవేళ (opts->flags & MENU_OPTS_తక్షణ) {
			ఆపు;
		}
	}

	ఒకవేళ (eof) {
		ఫలితం = xmalloc(పరిమాణం(పూర్ణాంకం));
		*ఫలితం = EOF;
	} లేకపోతే {
		పూర్ణాంకం j = 0;

		/*
		 * recalculate nr, if return back from menu directly with
		 * default selections.
		 */
		ఒకవేళ (!nr) {
			కొరకు (i = 0; i < stuff->nr; i++)
				nr += chosen[i];
		}

		CALLOC_ARRAY(ఫలితం, st_కూడు(nr, 1));
		కొరకు (i = 0; i < stuff->nr && j < nr; i++) {
			ఒకవేళ (chosen[i])
				ఫలితం[j++] = i;
		}
		ఫలితం[j] = EOF;
	}

	విడుదల(chosen);
	strbuf_release(&choice);
	తిరిగి ఫలితం;
}

స్థిర పూర్ణాంకం clean_cmd(శూన్య)
{
	తిరిగి MENU_RETURN_NO_LOOP;
}

స్థిర పూర్ణాంకం filter_by_patterns_cmd(శూన్య)
{
	నిర్మాణం dir_struct dir = DIR_INIT;
	నిర్మాణం strbuf confirm = STRBUF_INIT;
	నిర్మాణం నమూనా_list *pl;
	పూర్ణాంకం మారింది = -1, i;

	కొరకు (;;) {
		నిర్మాణం string_list ignore_list = STRING_LIST_INIT_NODUP;
		నిర్మాణం string_list_అంశం *అంశం;

		ఒకవేళ (!del_list.nr)
			ఆపు;

		ఒకవేళ (మారింది)
			pretty_print_dels();

		clean_print_color(CLEAN_COLOR_PROMPT);
		రాయి(_("Input ignore patterns>> "));
		clean_print_color(CLEAN_COLOR_RESET);
		ఒకవేళ (git_read_పంక్తి_interactively(&confirm) == EOF)
			అక్షరం_రాయి('\n');

		/* quit filter_by_pattern mode if press ENTER or Ctrl-D */
		ఒకవేళ (!confirm.len)
			ఆపు;

		pl = కూడు_నమూనా_list(&dir, EXC_CMDL, "manual exclude");

		string_list_విభజించు_లోపల_ఉంచు_f(&ignore_list, confirm.buf, " ", -1,
					     STRING_LIST_విభజించు_TRIM);

		కొరకు (i = 0; i < ignore_list.nr; i++) {
			అంశం = &ignore_list.వస్తువులు[i];
			ఒకవేళ (!*అంశం->string)
				కొనసాగు;
			కూడు_నమూనా(అంశం->string, "", 0, pl, -(i+1));
		}

		మారింది = 0;
		for_each_string_list_అంశం(అంశం, &del_list) {
			పూర్ణాంకం dtype = DT_UNKNOWN;

			ఒకవేళ (is_excluded(&dir, the_repository->సూచీ, అంశం->string, &dtype)) {
				*అంశం->string = '\0';
				మారింది++;
			}
		}

		ఒకవేళ (మారింది) {
			string_list_remove_ఖాళీలు_వస్తువులు(&del_list, 0);
		} లేకపోతే {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf_ln(_("WARNING: Cannot find items matched by: %s"), confirm.buf);
			clean_print_color(CLEAN_COLOR_RESET);
		}

		string_list_clear(&ignore_list, 0);
		dir_clear(&dir);
	}

	strbuf_release(&confirm);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం select_by_numbers_cmd(శూన్య)
{
	నిర్మాణం menu_opts menu_opts;
	నిర్మాణం menu_stuff menu_stuff;
	నిర్మాణం string_list_అంశం *వస్తువులు;
	పూర్ణాంకం *chosen;
	పూర్ణాంకం i, j;

	menu_opts.శీర్షిక = ఖాళీ;
	menu_opts.prompt = N_("Select items to delete");
	menu_opts.flags = 0;

	menu_stuff.type = MENU_STUFF_TYPE_STRING_LIST;
	menu_stuff.stuff = &del_list;
	menu_stuff.nr = del_list.nr;

	chosen = list_and_ఎంచుకో(&menu_opts, &menu_stuff);
	వస్తువులు = del_list.వస్తువులు;
	కొరకు (i = 0, j = 0; i < del_list.nr; i++) {
		ఒకవేళ (i < chosen[j]) {
			*(వస్తువులు[i].string) = '\0';
		} లేకపోతే ఒకవేళ (i == chosen[j]) {
			/* delete selected item */
			j++;
			కొనసాగు;
		} లేకపోతే {
			/* end of chosen (chosen[j] == EOF), won't delete */
			*(వస్తువులు[i].string) = '\0';
		}
	}

	string_list_remove_ఖాళీలు_వస్తువులు(&del_list, 0);

	విడుదల(chosen);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం ask_each_cmd(శూన్య)
{
	నిర్మాణం strbuf confirm = STRBUF_INIT;
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం string_list_అంశం *అంశం;
	స్థిరరాశి అక్షరం *qname;
	పూర్ణాంకం మారింది = 0, eof = 0;

	for_each_string_list_అంశం(అంశం, &del_list) {
		/* Ctrl-D should stop removing files */
		ఒకవేళ (!eof) {
			qname = quote_మార్గం(అంశం->string, ఖాళీ, &buf, 0);
			/* TRANSLATORS: Make sure to keep [y/N] as is */
			రాయి(_("Remove %s [y/N]? "), qname);
			ఒకవేళ (git_read_పంక్తి_interactively(&confirm) == EOF) {
				అక్షరం_రాయి('\n');
				eof = 1;
			}
		}
		ఒకవేళ (!confirm.len || strncasecmp(confirm.buf, "yes", confirm.len)) {
			*అంశం->string = '\0';
			మారింది++;
		}
	}

	ఒకవేళ (మారింది)
		string_list_remove_ఖాళీలు_వస్తువులు(&del_list, 0);

	strbuf_release(&buf);
	strbuf_release(&confirm);
	తిరిగి MENU_RETURN_NO_LOOP;
}

స్థిర పూర్ణాంకం quit_cmd(శూన్య)
{
	string_list_clear(&del_list, 0);
	రాయి(_("Bye.\n"));
	తిరిగి MENU_RETURN_NO_LOOP;
}

స్థిర పూర్ణాంకం help_cmd(శూన్య)
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
	తిరిగి 0;
}

స్థిర శూన్య interactive_main_loop(శూన్య)
{
	అయితే (del_list.nr) {
		నిర్మాణం menu_opts menu_opts;
		నిర్మాణం menu_stuff menu_stuff;
		నిర్మాణం menu_అంశం menus[] = {
			{'c', "clean",			0, clean_cmd},
			{'f', "filter by pattern",	0, filter_by_patterns_cmd},
			{'s', "select by numbers",	0, select_by_numbers_cmd},
			{'a', "ask each",		0, ask_each_cmd},
			{'q', "quit",			0, quit_cmd},
			{'h', "help",			0, help_cmd},
		};
		పూర్ణాంకం *chosen;

		menu_opts.శీర్షిక = N_("*** Commands ***");
		menu_opts.prompt = N_("What now");
		menu_opts.flags = MENU_OPTS_SINGLETON;

		menu_stuff.type = MENU_STUFF_TYPE_MENU_అంశం;
		menu_stuff.stuff = menus;
		menu_stuff.nr = పరిమాణం(menus) / పరిమాణం(నిర్మాణం menu_అంశం);

		clean_print_color(CLEAN_COLOR_శీర్షిక);
		printf_ln(Q_("Would remove the following item:",
			     "Would remove the following items:",
			     del_list.nr));
		clean_print_color(CLEAN_COLOR_RESET);

		pretty_print_dels();

		chosen = list_and_ఎంచుకో(&menu_opts, &menu_stuff);

		ఒకవేళ (*chosen != EOF) {
			పూర్ణాంకం ret;
			ret = menus[*chosen].fn();
			ఒకవేళ (ret != MENU_RETURN_NO_LOOP) {
				FREE_AND_NULL(chosen);
				ఒకవేళ (!del_list.nr) {
					clean_print_color(CLEAN_COLOR_ERROR);
					printf_ln(_("No more files to clean, exiting."));
					clean_print_color(CLEAN_COLOR_RESET);
					ఆపు;
				}
				కొనసాగు;
			}
		} లేకపోతే {
			quit_cmd();
		}

		FREE_AND_NULL(chosen);
		ఆపు;
	}
}

స్థిర శూన్య correct_untracked_entries(నిర్మాణం dir_struct *dir)
{
	పూర్ణాంకం src, dst, ign;

	కొరకు (src = dst = ign = 0; src < dir->nr; src++) {
		/* skip paths in ignored[] that cannot be inside entries[src] */
		అయితే (ign < dir->ignored_nr &&
		       0 <= cmp_dir_నమోదు(&dir->entries[src], &dir->ignored[ign]))
			ign++;

		ఒకవేళ (ign < dir->ignored_nr &&
		    check_dir_నమోదు_కలిగియుంది(dir->entries[src], dir->ignored[ign])) {
			/* entries[src] contains an ignored path, so we drop it */
			విడుదల(dir->entries[src]);
		} లేకపోతే {
			నిర్మాణం dir_నమోదు *ent = dir->entries[src++];

			/* entries[src] does not contain an ignored path, so we keep it */
			dir->entries[dst++] = ent;

			/* then discard paths in entries[] contained inside entries[src] */
			అయితే (src < dir->nr &&
			       check_dir_నమోదు_కలిగియుంది(ent, dir->entries[src]))
				విడుదల(dir->entries[src++]);

			/* compensate for the outer loop's loop control */
			src--;
		}
	}
	dir->nr = dst;
}

పూర్ణాంకం cmd_clean(పూర్ణాంకం argc,
	      స్థిరరాశి అక్షరం **argv,
	      స్థిరరాశి అక్షరం *ఉపసర్గ,
	      నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం i, res;
	పూర్ణాంకం dry_నిరంతరం = 0, remove_directories = 0, quiet = 0, ignored = 0;
	పూర్ణాంకం ignored_only = 0, force = 0, errors = 0, gone = 1;
	పూర్ణాంకం rm_flags = REMOVE_DIR_KEEP_NESTED_GIT;
	నిర్మాణం strbuf abs_మార్గం = STRBUF_INIT;
	నిర్మాణం dir_struct dir = DIR_INIT;
	నిర్మాణం pathspec pathspec;
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం string_list exclude_list = STRING_LIST_INIT_NODUP;
	నిర్మాణం నమూనా_list *pl;
	నిర్మాణం string_list_అంశం *అంశం;
	స్థిరరాశి అక్షరం *qname;
	నిర్మాణం option options[] = {
		OPT__QUIET(&quiet, N_("do not print names of files removed")),
		OPT_DRY_నిరంతరం(&dry_నిరంతరం, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('i', "interactive", &interactive, N_("interactive cleaning")),
		OPT_BOOL('d', ఖాళీ, &remove_directories,
				N_("remove whole directories")),
		OPT_CALLBACK_F('e', "exclude", &exclude_list, N_("pattern"),
		  N_("add <pattern> to ignore rules"), PARSE_OPT_NONEG, exclude_cb),
		OPT_BOOL('x', ఖాళీ, &ignored, N_("remove ignored files, too")),
		OPT_BOOL('X', ఖాళీ, &ignored_only,
				N_("remove only ignored files")),
		OPT_చివర()
	};

	repo_config(the_repository, git_clean_config, ఖాళీ);

	argc = parse_options(argc, argv, ఉపసర్గ, options, builtin_clean_usage,
			     0);

	ఒకవేళ (require_force != 0 && !force && !interactive && !dry_నిరంతరం)
		die(_("clean.requireForce is true and -f not given: refusing to clean"));

	ఒకవేళ (force > 1)
		rm_flags = 0;
	లేకపోతే
		dir.flags |= DIR_SKIP_NESTED_GIT;

	dir.flags |= DIR_SHOW_OTHER_DIRECTORIES;

	ఒకవేళ (ignored && ignored_only)
		die(_("options '%s' and '%s' cannot be used together"), "-x", "-X");
	ఒకవేళ (!ignored)
		అమరిక_standard_excludes(&dir);
	ఒకవేళ (ignored_only)
		dir.flags |= DIR_SHOW_IGNORED;

	ఒకవేళ (argc) {
		/*
		 * Remaining args implies pathspecs specified, and we should
		 * recurse within those.
		 */
		remove_directories = 1;
	}

	ఒకవేళ (remove_directories && !ignored_only) {
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
		ఒకవేళ (!ignored)
			dir.flags |= DIR_SHOW_IGNORED_TOO_బహుళకం_MATCHING;

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
	the_repository->settings.command_requires_నిండిన_సూచీ = 0;

	ఒకవేళ (repo_read_సూచీ(the_repository) < 0)
		die(_("index file corrupt"));

	pl = కూడు_నమూనా_list(&dir, EXC_CMDL, "--exclude option");
	కొరకు (i = 0; i < exclude_list.nr; i++)
		కూడు_నమూనా(exclude_list.వస్తువులు[i].string, "", 0, pl, -(i+1));

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       ఉపసర్గ, argv);

	fill_directory(&dir, the_repository->సూచీ, &pathspec);
	correct_untracked_entries(&dir);

	కొరకు (i = 0; i < dir.nr; i++) {
		నిర్మాణం dir_నమోదు *ent = dir.entries[i];
		నిర్మాణం stat st;
		స్థిరరాశి అక్షరం *rel;

		ఒకవేళ (!సూచీ_name_is_other(the_repository->సూచీ, ent->name, ent->len))
			కొనసాగు;

		ఒకవేళ (lstat(ent->name, &st))
			die_errno("Cannot lstat '%s'", ent->name);

		ఒకవేళ (S_ISDIR(st.st_బహుళకం) && !remove_directories)
			కొనసాగు;

		rel = relative_మార్గం(ent->name, ఉపసర్గ, &buf);
		string_list_append(&del_list, rel);
	}

	dir_clear(&dir);

	ఒకవేళ (interactive && del_list.nr > 0)
		interactive_main_loop();

	for_each_string_list_అంశం(అంశం, &del_list) {
		నిర్మాణం stat st;

		strbuf_reset(&abs_మార్గం);
		ఒకవేళ (ఉపసర్గ)
			strbuf_addstr(&abs_మార్గం, ఉపసర్గ);

		strbuf_addstr(&abs_మార్గం, అంశం->string);

		/*
		 * we might have removed this as part of earlier
		 * recursive directory removal, so lstat() here could
		 * fail with ENOENT.
		 */
		ఒకవేళ (lstat(abs_మార్గం.buf, &st))
			కొనసాగు;

		ఒకవేళ (S_ISDIR(st.st_బహుళకం)) {
			ఒకవేళ (remove_dirs(&abs_మార్గం, ఉపసర్గ, rm_flags, dry_నిరంతరం, quiet, &gone))
				errors++;
			ఒకవేళ (gone && !quiet) {
				qname = quote_మార్గం(అంశం->string, ఖాళీ, &buf, 0);
				రాయి(dry_నిరంతరం ? _(సందేశం_would_remove) : _(సందేశం_remove), qname);
			}
		} లేకపోతే {
			res = dry_నిరంతరం ? 0 : unlink(abs_మార్గం.buf);
			ఒకవేళ (res) {
				పూర్ణాంకం saved_errno = errno;
				qname = quote_మార్గం(అంశం->string, ఖాళీ, &buf, 0);
				errno = saved_errno;
				warning_errno(_(సందేశం_warn_remove_failed), qname);
				errors++;
			} లేకపోతే ఒకవేళ (!quiet) {
				qname = quote_మార్గం(అంశం->string, ఖాళీ, &buf, 0);
				రాయి(dry_నిరంతరం ? _(సందేశం_would_remove) : _(సందేశం_remove), qname);
			}
		}
	}

	strbuf_release(&abs_మార్గం);
	strbuf_release(&buf);
	string_list_clear(&del_list, 0);
	string_list_clear(&exclude_list, 0);
	clear_pathspec(&pathspec);
	తిరిగి (errors != 0);
}
