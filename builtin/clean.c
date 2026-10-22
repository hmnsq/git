/*
 * "git clean" builtin command
 *
 * Copyright (C) 2007 Shawn Bohrer
 *
 * Based on git-clean.sh by Pavel Roskin
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "read-cache-ll.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "column.h"
#ಅನ್ತರ್ಭಾವ "color.h"
#ಅನ್ತರ್ಭಾವ "pathspec.h"
#ಅನ್ತರ್ಭಾವ "help.h"
#ಅನ್ತರ್ಭಾವ "prompt.h"

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ require_force = -1; /* unset */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ interactive;
ಸ್ಥಿರ ರಚನೆ string_list del_list = STRING_LIST_INIT_DUP;
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ colopts;

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ builtin_clean_usage[] = {
	N_("git clean [-d] [-f] [-i] [-n] [-q] [-e <pattern>] [-x | -X] [--] [<pathspec>...]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_remove = N_("Removing %s\n");
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_would_remove = N_("Would remove %s\n");
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_skip_git_dir = N_("Skipping repository %s\n");
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_would_skip_git_dir = N_("Would skip repository %s\n");
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_warn_remove_failed = N_("failed to remove %s");
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_warn_lstat_failed = N_("could not lstat %s\n");
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_skip_cwd = N_("Refusing to remove current working directory\n");
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_would_skip_cwd = N_("Would refuse to remove current working directory\n");

ಎಣಿಕೆ color_clean {
	CLEAN_COLOR_RESET = 0,
	CLEAN_COLOR_PLAIN = 1,
	CLEAN_COLOR_PROMPT = 2,
	CLEAN_COLOR_ಶೀರ್ಷಿಕೆ = 3,
	CLEAN_COLOR_HELP = 4,
	CLEAN_COLOR_ERROR = 5
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *color_interactive_slots[] = {
	[CLEAN_COLOR_ERROR]  = "error",
	[CLEAN_COLOR_ಶೀರ್ಷಿಕೆ] = "header",
	[CLEAN_COLOR_HELP]   = "help",
	[CLEAN_COLOR_PLAIN]  = "plain",
	[CLEAN_COLOR_PROMPT] = "prompt",
	[CLEAN_COLOR_RESET]  = "reset",
};

ಸ್ಥಿರ ಎಣಿಕೆ git_colorbool clean_use_color = GIT_COLOR_UNKNOWN;
ಸ್ಥಿರ ಅಕ್ಷರ clean_colors[][COLOR_MAXLEN] = {
	[CLEAN_COLOR_ERROR] = GIT_COLOR_BOLD_RED,
	[CLEAN_COLOR_ಶೀರ್ಷಿಕೆ] = GIT_COLOR_BOLD,
	[CLEAN_COLOR_HELP] = GIT_COLOR_BOLD_RED,
	[CLEAN_COLOR_PLAIN] = GIT_COLOR_ಸಾಮಾನ್ಯ,
	[CLEAN_COLOR_PROMPT] = GIT_COLOR_BOLD_BLUE,
	[CLEAN_COLOR_RESET] = GIT_COLOR_RESET,
};

#ವ್ಯಾಖ್ಯಾನಿಸು MENU_OPTS_SINGLETON		01
#ವ್ಯಾಖ್ಯಾನಿಸು MENU_OPTS_ತಕ್ಷಣ		02
#ವ್ಯಾಖ್ಯಾನಿಸು MENU_OPTS_LIST_ONLY		04

ರಚನೆ menu_opts {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಶೀರ್ಷಿಕೆ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *prompt;
	ಪೂರ್ಣಾಂಕ flags;
};

#ವ್ಯಾಖ್ಯಾನಿಸು MENU_RETURN_NO_LOOP		10

ರಚನೆ menu_ಅಂಶ {
	ಅಕ್ಷರ hotkey;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *title;
	ಪೂರ್ಣಾಂಕ selected;
	ಪೂರ್ಣಾಂಕ (*fn)(ಶೂನ್ಯ);
};

ಎಣಿಕೆ menu_stuff_type {
	MENU_STUFF_TYPE_STRING_LIST = 1,
	MENU_STUFF_TYPE_MENU_ಅಂಶ
};

ರಚನೆ menu_stuff {
	ಎಣಿಕೆ menu_stuff_type type;
	ಪೂರ್ಣಾಂಕ nr;
	ಶೂನ್ಯ *stuff;
};

define_list_config_array(color_interactive_slots);

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_clean_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
			    ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಾನ_name;

	ಆದರೆ (starts_with(var, "column."))
		ಹಿಂತಿರುಗಿ git_ಸ್ತಂಭ_config(var, ಮೌಲ್ಯ, "clean", &colopts);

	/* honors the color.interactive* config variables which also
	   applied in git-add--interactive and git-stash */
	ಆದರೆ (!strcmp(var, "color.interactive")) {
		clean_use_color = git_config_colorbool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (skip_ಪೂರ್ವಪ್ರತ್ಯಯ(var, "color.interactive.", &ಸ್ಥಾನ_name)) {
		ಪೂರ್ಣಾಂಕ ಸ್ಥಾನ = LOOKUP_CONFIG(color_interactive_slots, ಸ್ಥಾನ_name);
		ಆದರೆ (ಸ್ಥಾನ < 0)
			ಹಿಂತಿರುಗಿ 0;
		ಆದರೆ (!ಮೌಲ್ಯ)
			ಹಿಂತಿರುಗಿ config_error_nonbool(var);
		ಹಿಂತಿರುಗಿ color_parse(ಮೌಲ್ಯ, clean_colors[ಸ್ಥಾನ]);
	}

	ಆದರೆ (!strcmp(var, "clean.requireforce")) {
		require_force = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (git_color_config(var, ಮೌಲ್ಯ, cb) < 0)
		ಹಿಂತಿರುಗಿ -1;

	ಹಿಂತಿರುಗಿ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *clean_get_color(ಎಣಿಕೆ color_clean ix)
{
	ಆದರೆ (want_color(clean_use_color))
		ಹಿಂತಿರುಗಿ clean_colors[ix];
	ಹಿಂತಿರುಗಿ "";
}

ಸ್ಥಿರ ಶೂನ್ಯ clean_print_color(ಎಣಿಕೆ color_clean ix)
{
	ಬರೆ("%s", clean_get_color(ix));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ exclude_cb(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ string_list *exclude_list = opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_NEG(unset);
	string_list_append(exclude_list, arg);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ remove_dirs(ರಚನೆ strbuf *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ, ಪೂರ್ಣಾಂಕ force_flag,
		ಪೂರ್ಣಾಂಕ dry_ಸತತ, ಪೂರ್ಣಾಂಕ quiet, ಪೂರ್ಣಾಂಕ *dir_gone)
{
	DIR *dir;
	ರಚನೆ strbuf quoted = STRBUF_INIT;
	ರಚನೆ strbuf realpath = STRBUF_INIT;
	ರಚನೆ strbuf real_ocwd = STRBUF_INIT;
	ರಚನೆ dirent *e;
	ಪೂರ್ಣಾಂಕ res = 0, ret = 0, gone = 1, original_len = ಮಾರ್ಗ->len, len;
	ರಚನೆ string_list dels = STRING_LIST_INIT_DUP;

	*dir_gone = 1;

	ಆದರೆ ((force_flag & REMOVE_DIR_KEEP_NESTED_GIT) &&
	    is_nonbare_repository_dir(ಮಾರ್ಗ)) {
		ಆದರೆ (!quiet) {
			quote_ಮಾರ್ಗ(ಮಾರ್ಗ->buf, ಪೂರ್ವಪ್ರತ್ಯಯ, &quoted, 0);
			ಬರೆ(dry_ಸತತ ?  _(ಸಂದೇಶ_would_skip_git_dir) : _(ಸಂದೇಶ_skip_git_dir),
					quoted.buf);
		}

		*dir_gone = 0;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	dir = opendir(ಮಾರ್ಗ->buf);
	ಆದರೆ (!dir) {
		/* an empty dir could be removed even if it is unreadble */
		res = dry_ಸತತ ? 0 : rmdir(ಮಾರ್ಗ->buf);
		ಆದರೆ (res) {
			ಪೂರ್ಣಾಂಕ saved_errno = errno;
			quote_ಮಾರ್ಗ(ಮಾರ್ಗ->buf, ಪೂರ್ವಪ್ರತ್ಯಯ, &quoted, 0);
			errno = saved_errno;
			warning_errno(_(ಸಂದೇಶ_warn_remove_failed), quoted.buf);
			*dir_gone = 0;
		}
		ret = res;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	strbuf_complete(ಮಾರ್ಗ, '/');

	len = ಮಾರ್ಗ->len;
	ಆಗ ((e = readdir_skip_dot_and_dotdot(dir)) != ಖಾಲಿ) {
		ರಚನೆ stat st;

		strbuf_setlen(ಮಾರ್ಗ, len);
		strbuf_addstr(ಮಾರ್ಗ, e->d_name);
		ಆದರೆ (lstat(ಮಾರ್ಗ->buf, &st))
			warning_errno(_(ಸಂದೇಶ_warn_lstat_failed), ಮಾರ್ಗ->buf);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISDIR(st.st_ಬಹುಲಕ)) {
			ಆದರೆ (remove_dirs(ಮಾರ್ಗ, ಪೂರ್ವಪ್ರತ್ಯಯ, force_flag, dry_ಸತತ, quiet, &gone))
				ret = 1;
			ಆದರೆ (gone) {
				quote_ಮಾರ್ಗ(ಮಾರ್ಗ->buf, ಪೂರ್ವಪ್ರತ್ಯಯ, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} ಇಲ್ಲದಿದ್ದರೆ
				*dir_gone = 0;
			ಮುಂದುವರಿ;
		} ಇಲ್ಲದಿದ್ದರೆ {
			res = dry_ಸತತ ? 0 : unlink(ಮಾರ್ಗ->buf);
			ಆದರೆ (!res) {
				quote_ಮಾರ್ಗ(ಮಾರ್ಗ->buf, ಪೂರ್ವಪ್ರತ್ಯಯ, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} ಇಲ್ಲದಿದ್ದರೆ {
				ಪೂರ್ಣಾಂಕ saved_errno = errno;
				quote_ಮಾರ್ಗ(ಮಾರ್ಗ->buf, ಪೂರ್ವಪ್ರತ್ಯಯ, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(ಸಂದೇಶ_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
			ಮುಂದುವರಿ;
		}

		/* path too long, stat fails, or non-directory still exists */
		*dir_gone = 0;
		ret = 1;
		ನಿಲ್ಲಿಸು;
	}
	closedir(dir);

	strbuf_setlen(ಮಾರ್ಗ, original_len);

	ಆದರೆ (*dir_gone) {
		/*
		 * Normalize path components in path->buf, e.g. change '\' to
		 * '/' on Windows.
		 */
		strbuf_realpath(&realpath, ಮಾರ್ಗ->buf, 1);

		/*
		 * path and realpath are absolute; for comparison, we would
		 * like to transform startup_info->original_cwd to an absolute
		 * path too.
		 */
		 ಆದರೆ (startup_info->original_cwd)
			 strbuf_realpath(&real_ocwd,
					 startup_info->original_cwd, 1);

		ಆದರೆ (!strbuf_cmp(&realpath, &real_ocwd)) {
			ಬರೆ("%s", dry_ಸತತ ? _(ಸಂದೇಶ_would_skip_cwd) : _(ಸಂದೇಶ_skip_cwd));
			*dir_gone = 0;
		} ಇಲ್ಲದಿದ್ದರೆ {
			res = dry_ಸತತ ? 0 : rmdir(ಮಾರ್ಗ->buf);
			ಆದರೆ (!res)
				*dir_gone = 1;
			ಇಲ್ಲದಿದ್ದರೆ {
				ಪೂರ್ಣಾಂಕ saved_errno = errno;
				quote_ಮಾರ್ಗ(ಮಾರ್ಗ->buf, ಪೂರ್ವಪ್ರತ್ಯಯ, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(ಸಂದೇಶ_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
		}
	}

	ಆದರೆ (!*dir_gone && !quiet) {
		ಪೂರ್ಣಾಂಕ i;
		ಗಾಗಿ (i = 0; i < dels.nr; i++)
			ಬರೆ(dry_ಸತತ ?  _(ಸಂದೇಶ_would_remove) : _(ಸಂದೇಶ_remove), dels.ವಸ್ತುಗಳು[i].string);
	}
ಹೊರಹೋಗು:
	strbuf_release(&realpath);
	strbuf_release(&real_ocwd);
	strbuf_release(&quoted);
	string_list_clear(&dels, 0);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ pretty_print_dels(ಶೂನ್ಯ)
{
	ರಚನೆ string_list list = STRING_LIST_INIT_DUP;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *qname;
	ರಚನೆ ಸ್ತಂಭ_options copts;

	for_each_string_list_ಅಂಶ(ಅಂಶ, &del_list) {
		qname = quote_ಮಾರ್ಗ(ಅಂಶ->string, ಖಾಲಿ, &buf, 0);
		string_list_append(&list, qname);
	}

	/*
	 * always enable column display, we only consult column.*
	 * about layout strategy and stuff
	 */
	colopts = (colopts & ~ಲಂಬಸಾಲು_ENABLE_ಮುಖವಾಡ) | ಲಂಬಸಾಲು_ENABLED;
	memset(&copts, 0, ಗಾತ್ರ(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(&list, colopts, &copts);
	strbuf_release(&buf);
	string_list_clear(&list, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ pretty_print_menus(ರಚನೆ string_list *menu_list)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ local_colopts = 0;
	ರಚನೆ ಸ್ತಂಭ_options copts;

	local_colopts = ಲಂಬಸಾಲು_ENABLED | ಲಂಬಸಾಲು_ಅಡ್ಡಸಾಲು;
	memset(&copts, 0, ಗಾತ್ರ(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(menu_list, local_colopts, &copts);
}

ಸ್ಥಿರ ಶೂನ್ಯ prompt_help_cmd(ಪೂರ್ಣಾಂಕ singleton)
{
	clean_print_color(CLEAN_COLOR_HELP);
	ಬರೆ(singleton ?
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
ಸ್ಥಿರ ಶೂನ್ಯ print_highlight_menu_stuff(ರಚನೆ menu_stuff *stuff, ಪೂರ್ಣಾಂಕ **chosen)
{
	ರಚನೆ string_list menu_list = STRING_LIST_INIT_DUP;
	ರಚನೆ strbuf menu = STRBUF_INIT;
	ರಚನೆ menu_ಅಂಶ *menu_ಅಂಶ;
	ರಚನೆ string_list_ಅಂಶ *string_list_ಅಂಶ;
	ಪೂರ್ಣಾಂಕ i;

	ಆಯ್ಕೆ (stuff->type) {
	ಪೂರ್ವನಿಯೋಜಿತ:
		die("Bad type of menu_stuff when print menu");
	ಸ್ಥಿತಿ MENU_STUFF_TYPE_MENU_ಅಂಶ:
		menu_ಅಂಶ = (ರಚನೆ menu_ಅಂಶ *)stuff->stuff;
		ಗಾಗಿ (i = 0; i < stuff->nr; i++, menu_ಅಂಶ++) {
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p;
			ಪೂರ್ಣಾಂಕ highlighted = 0;

			p = menu_ಅಂಶ->title;
			ಆದರೆ ((*chosen)[i] < 0)
				(*chosen)[i] = menu_ಅಂಶ->selected ? 1 : 0;
			strbuf_addf(&menu, "%s%2d: ", (*chosen)[i] ? "*" : " ", i+1);
			ಗಾಗಿ (; *p; p++) {
				ಆದರೆ (!highlighted && *p == menu_ಅಂಶ->hotkey) {
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_PROMPT));
					strbuf_addch(&menu, *p);
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_RESET));
					highlighted = 1;
				} ಇಲ್ಲದಿದ್ದರೆ {
					strbuf_addch(&menu, *p);
				}
			}
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
		}
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ MENU_STUFF_TYPE_STRING_LIST:
		i = 0;
		for_each_string_list_ಅಂಶ(string_list_ಅಂಶ, (ರಚನೆ string_list *)stuff->stuff) {
			ಆದರೆ ((*chosen)[i] < 0)
				(*chosen)[i] = 0;
			strbuf_addf(&menu, "%s%2d: %s",
				    (*chosen)[i] ? "*" : " ", i+1, string_list_ಅಂಶ->string);
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
			i++;
		}
		ನಿಲ್ಲಿಸು;
	}

	pretty_print_menus(&menu_list);

	strbuf_release(&menu);
	string_list_clear(&menu_list, 0);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕಂಡುಹಿಡಿ_unique(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *choice, ರಚನೆ menu_stuff *menu_stuff)
{
	ರಚನೆ menu_ಅಂಶ *menu_ಅಂಶ;
	ರಚನೆ string_list_ಅಂಶ *string_list_ಅಂಶ;
	ಪೂರ್ಣಾಂಕ i, len, ಸಿಕ್ಕಿತು_2 = 0;

	len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(choice);
	ಆಯ್ಕೆ (menu_stuff->type) {
	ಪೂರ್ವನಿಯೋಜಿತ:
		die("Bad type of menu_stuff when parse choice");
	ಸ್ಥಿತಿ MENU_STUFF_TYPE_MENU_ಅಂಶ:

		menu_ಅಂಶ = (ರಚನೆ menu_ಅಂಶ *)menu_stuff->stuff;
		ಗಾಗಿ (i = 0; i < menu_stuff->nr; i++, menu_ಅಂಶ++) {
			ಆದರೆ (len == 1 && *choice == menu_ಅಂಶ->hotkey) {
				ಸಿಕ್ಕಿತು_2 = i + 1;
				ನಿಲ್ಲಿಸು;
			}
			ಆದರೆ (!strncasecmp(choice, menu_ಅಂಶ->title, len)) {
				ಆದರೆ (ಸಿಕ್ಕಿತು_2) {
					ಆದರೆ (len == 1) {
						/* continue for hotkey matching */
						ಸಿಕ್ಕಿತು_2 = -1;
					} ಇಲ್ಲದಿದ್ದರೆ {
						ಸಿಕ್ಕಿತು_2 = 0;
						ನಿಲ್ಲಿಸು;
					}
				} ಇಲ್ಲದಿದ್ದರೆ {
					ಸಿಕ್ಕಿತು_2 = i + 1;
				}
			}
		}
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ MENU_STUFF_TYPE_STRING_LIST:
		string_list_ಅಂಶ = ((ರಚನೆ string_list *)menu_stuff->stuff)->ವಸ್ತುಗಳು;
		ಗಾಗಿ (i = 0; i < menu_stuff->nr; i++, string_list_ಅಂಶ++) {
			ಆದರೆ (!strncasecmp(choice, string_list_ಅಂಶ->string, len)) {
				ಆದರೆ (ಸಿಕ್ಕಿತು_2) {
					ಸಿಕ್ಕಿತು_2 = 0;
					ನಿಲ್ಲಿಸು;
				}
				ಸಿಕ್ಕಿತು_2 = i + 1;
			}
		}
		ನಿಲ್ಲಿಸು;
	}
	ಹಿಂತಿರುಗಿ ಸಿಕ್ಕಿತು_2;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_choice(ರಚನೆ menu_stuff *menu_stuff,
			ಪೂರ್ಣಾಂಕ is_single,
			ಅಕ್ಷರ *ಒಳಹರಿವು,
			ಪೂರ್ಣಾಂಕ **chosen)
{
	ರಚನೆ string_list choice = STRING_LIST_INIT_NODUP;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ಪೂರ್ಣಾಂಕ nr = 0;
	ಪೂರ್ಣಾಂಕ i;

	string_list_ವಿಭಜಿಸು_ಒಳ_ಇರಿಸು_f(&choice, ಒಳಹರಿವು,
				     is_single ? "\n" : ", ", -1,
				     STRING_LIST_ವಿಭಜಿಸು_TRIM);

	for_each_string_list_ಅಂಶ(ಅಂಶ, &choice) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string;
		ಪೂರ್ಣಾಂಕ ಆರಿಸು;
		ಪೂರ್ಣಾಂಕ bottom = 0, ತುದಿ = 0;
		ಪೂರ್ಣಾಂಕ is_range, is_ಸಂಖ್ಯೆ;

		string = ಅಂಶ->string;
		ಆದರೆ (!*string)
			ಮುಂದುವರಿ;

		/* Input that begins with '-'; unchoose */
		ಆದರೆ (string[0] == '-') {
			ಆರಿಸು = 0;
			string++;
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಆರಿಸು = 1;
		}

		is_range = 0;
		is_ಸಂಖ್ಯೆ = 1;
		ಗಾಗಿ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p = string; *p; p++) {
			ಆದರೆ ('-' == *p) {
				ಆದರೆ (!is_range) {
					is_range = 1;
					is_ಸಂಖ್ಯೆ = 0;
				} ಇಲ್ಲದಿದ್ದರೆ {
					is_ಸಂಖ್ಯೆ = 0;
					is_range = 0;
					ನಿಲ್ಲಿಸು;
				}
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ಅಂಕಿಯೇ(*p)) {
				is_ಸಂಖ್ಯೆ = 0;
				is_range = 0;
				ನಿಲ್ಲಿಸು;
			}
		}

		ಆದರೆ (is_ಸಂಖ್ಯೆ) {
			bottom = atoi(string);
			ತುದಿ = bottom;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_range) {
			bottom = atoi(string);
			/* a range can be specified like 5-7 or 5- */
			ಆದರೆ (!*(strchr(string, '-') + 1))
				ತುದಿ = menu_stuff->nr;
			ಇಲ್ಲದಿದ್ದರೆ
				ತುದಿ = atoi(strchr(string, '-') + 1);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(string, "*")) {
			bottom = 1;
			ತುದಿ = menu_stuff->nr;
		} ಇಲ್ಲದಿದ್ದರೆ {
			bottom = ಕಂಡುಹಿಡಿ_unique(string, menu_stuff);
			ತುದಿ = bottom;
		}

		ಆದರೆ (ತುದಿ <= 0 || bottom <= 0 || ತುದಿ > menu_stuff->nr || bottom > ತುದಿ ||
		    (is_single && bottom != ತುದಿ)) {
			clean_print_color(CLEAN_COLOR_ERROR);
			ಬರೆ(_("Huh (%s)?\n"), string);
			clean_print_color(CLEAN_COLOR_RESET);
			ಮುಂದುವರಿ;
		}

		ಗಾಗಿ (i = bottom; i <= ತುದಿ; i++)
			(*chosen)[i-1] = ಆರಿಸು;
	}

	string_list_clear(&choice, 0);

	ಗಾಗಿ (i = 0; i < menu_stuff->nr; i++)
		nr += (*chosen)[i];
	ಹಿಂತಿರುಗಿ nr;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ *list_and_ಆರಿಸು(ರಚನೆ menu_opts *opts, ರಚನೆ menu_stuff *stuff)
{
	ರಚನೆ strbuf choice = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ *chosen, *ಫಲಿತಾಂಶ;
	ಪೂರ್ಣಾಂಕ nr = 0;
	ಪೂರ್ಣಾಂಕ eof = 0;
	ಪೂರ್ಣಾಂಕ i;

	ALLOC_ARRAY(chosen, stuff->nr);
	/* set chosen as uninitialized */
	ಗಾಗಿ (i = 0; i < stuff->nr; i++)
		chosen[i] = -1;

	ಗಾಗಿ (;;) {
		ಆದರೆ (opts->ಶೀರ್ಷಿಕೆ) {
			printf_ln("%s%s%s",
				  clean_get_color(CLEAN_COLOR_ಶೀರ್ಷಿಕೆ),
				  _(opts->ಶೀರ್ಷಿಕೆ),
				  clean_get_color(CLEAN_COLOR_RESET));
		}

		/* chosen will be initialized by print_highlight_menu_stuff */
		print_highlight_menu_stuff(stuff, &chosen);

		ಆದರೆ (opts->flags & MENU_OPTS_LIST_ONLY)
			ನಿಲ್ಲಿಸು;

		ಆದರೆ (opts->prompt) {
			ಬರೆ("%s%s%s%s",
			       clean_get_color(CLEAN_COLOR_PROMPT),
			       _(opts->prompt),
			       opts->flags & MENU_OPTS_SINGLETON ? "> " : ">> ",
			       clean_get_color(CLEAN_COLOR_RESET));
		}

		ಆದರೆ (git_read_ಸಾಲು_interactively(&choice) == EOF) {
			eof = 1;
			ನಿಲ್ಲಿಸು;
		}

		/* help for prompt */
		ಆದರೆ (!strcmp(choice.buf, "?")) {
			prompt_help_cmd(opts->flags & MENU_OPTS_SINGLETON);
			ಮುಂದುವರಿ;
		}

		/* for a multiple-choice menu, press ENTER (empty) will return back */
		ಆದರೆ (!(opts->flags & MENU_OPTS_SINGLETON) && !choice.len)
			ನಿಲ್ಲಿಸು;

		nr = parse_choice(stuff,
				  opts->flags & MENU_OPTS_SINGLETON,
				  choice.buf,
				  &chosen);

		ಆದರೆ (opts->flags & MENU_OPTS_SINGLETON) {
			ಆದರೆ (nr)
				ನಿಲ್ಲಿಸು;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (opts->flags & MENU_OPTS_ತಕ್ಷಣ) {
			ನಿಲ್ಲಿಸು;
		}
	}

	ಆದರೆ (eof) {
		ಫಲಿತಾಂಶ = xmalloc(ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ));
		*ಫಲಿತಾಂಶ = EOF;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಪೂರ್ಣಾಂಕ j = 0;

		/*
		 * recalculate nr, if return back from menu directly with
		 * default selections.
		 */
		ಆದರೆ (!nr) {
			ಗಾಗಿ (i = 0; i < stuff->nr; i++)
				nr += chosen[i];
		}

		CALLOC_ARRAY(ಫಲಿತಾಂಶ, st_ಕೂಡಿಸು(nr, 1));
		ಗಾಗಿ (i = 0; i < stuff->nr && j < nr; i++) {
			ಆದರೆ (chosen[i])
				ಫಲಿತಾಂಶ[j++] = i;
		}
		ಫಲಿತಾಂಶ[j] = EOF;
	}

	ಬಿಡುಗಡೆ(chosen);
	strbuf_release(&choice);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ clean_cmd(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ MENU_RETURN_NO_LOOP;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ filter_by_patterns_cmd(ಶೂನ್ಯ)
{
	ರಚನೆ dir_struct dir = DIR_INIT;
	ರಚನೆ strbuf confirm = STRBUF_INIT;
	ರಚನೆ ನಮೂನೆ_list *pl;
	ಪೂರ್ಣಾಂಕ ಬದಲಾಯಿತು = -1, i;

	ಗಾಗಿ (;;) {
		ರಚನೆ string_list ignore_list = STRING_LIST_INIT_NODUP;
		ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;

		ಆದರೆ (!del_list.nr)
			ನಿಲ್ಲಿಸು;

		ಆದರೆ (ಬದಲಾಯಿತು)
			pretty_print_dels();

		clean_print_color(CLEAN_COLOR_PROMPT);
		ಬರೆ(_("Input ignore patterns>> "));
		clean_print_color(CLEAN_COLOR_RESET);
		ಆದರೆ (git_read_ಸಾಲು_interactively(&confirm) == EOF)
			ಅಕ್ಷರ_ಬರೆ('\n');

		/* quit filter_by_pattern mode if press ENTER or Ctrl-D */
		ಆದರೆ (!confirm.len)
			ನಿಲ್ಲಿಸು;

		pl = ಕೂಡಿಸು_ನಮೂನೆ_list(&dir, EXC_CMDL, "manual exclude");

		string_list_ವಿಭಜಿಸು_ಒಳ_ಇರಿಸು_f(&ignore_list, confirm.buf, " ", -1,
					     STRING_LIST_ವಿಭಜಿಸು_TRIM);

		ಗಾಗಿ (i = 0; i < ignore_list.nr; i++) {
			ಅಂಶ = &ignore_list.ವಸ್ತುಗಳು[i];
			ಆದರೆ (!*ಅಂಶ->string)
				ಮುಂದುವರಿ;
			ಕೂಡಿಸು_ನಮೂನೆ(ಅಂಶ->string, "", 0, pl, -(i+1));
		}

		ಬದಲಾಯಿತು = 0;
		for_each_string_list_ಅಂಶ(ಅಂಶ, &del_list) {
			ಪೂರ್ಣಾಂಕ dtype = DT_UNKNOWN;

			ಆದರೆ (is_excluded(&dir, the_repository->ಸೂಚ್ಯಂಕ, ಅಂಶ->string, &dtype)) {
				*ಅಂಶ->string = '\0';
				ಬದಲಾಯಿತು++;
			}
		}

		ಆದರೆ (ಬದಲಾಯಿತು) {
			string_list_remove_ಖಾಲಿಗಳು_ವಸ್ತುಗಳು(&del_list, 0);
		} ಇಲ್ಲದಿದ್ದರೆ {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf_ln(_("WARNING: Cannot find items matched by: %s"), confirm.buf);
			clean_print_color(CLEAN_COLOR_RESET);
		}

		string_list_clear(&ignore_list, 0);
		dir_clear(&dir);
	}

	strbuf_release(&confirm);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ select_by_numbers_cmd(ಶೂನ್ಯ)
{
	ರಚನೆ menu_opts menu_opts;
	ರಚನೆ menu_stuff menu_stuff;
	ರಚನೆ string_list_ಅಂಶ *ವಸ್ತುಗಳು;
	ಪೂರ್ಣಾಂಕ *chosen;
	ಪೂರ್ಣಾಂಕ i, j;

	menu_opts.ಶೀರ್ಷಿಕೆ = ಖಾಲಿ;
	menu_opts.prompt = N_("Select items to delete");
	menu_opts.flags = 0;

	menu_stuff.type = MENU_STUFF_TYPE_STRING_LIST;
	menu_stuff.stuff = &del_list;
	menu_stuff.nr = del_list.nr;

	chosen = list_and_ಆರಿಸು(&menu_opts, &menu_stuff);
	ವಸ್ತುಗಳು = del_list.ವಸ್ತುಗಳು;
	ಗಾಗಿ (i = 0, j = 0; i < del_list.nr; i++) {
		ಆದರೆ (i < chosen[j]) {
			*(ವಸ್ತುಗಳು[i].string) = '\0';
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (i == chosen[j]) {
			/* delete selected item */
			j++;
			ಮುಂದುವರಿ;
		} ಇಲ್ಲದಿದ್ದರೆ {
			/* end of chosen (chosen[j] == EOF), won't delete */
			*(ವಸ್ತುಗಳು[i].string) = '\0';
		}
	}

	string_list_remove_ಖಾಲಿಗಳು_ವಸ್ತುಗಳು(&del_list, 0);

	ಬಿಡುಗಡೆ(chosen);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ask_each_cmd(ಶೂನ್ಯ)
{
	ರಚನೆ strbuf confirm = STRBUF_INIT;
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *qname;
	ಪೂರ್ಣಾಂಕ ಬದಲಾಯಿತು = 0, eof = 0;

	for_each_string_list_ಅಂಶ(ಅಂಶ, &del_list) {
		/* Ctrl-D should stop removing files */
		ಆದರೆ (!eof) {
			qname = quote_ಮಾರ್ಗ(ಅಂಶ->string, ಖಾಲಿ, &buf, 0);
			/* TRANSLATORS: Make sure to keep [y/N] as is */
			ಬರೆ(_("Remove %s [y/N]? "), qname);
			ಆದರೆ (git_read_ಸಾಲು_interactively(&confirm) == EOF) {
				ಅಕ್ಷರ_ಬರೆ('\n');
				eof = 1;
			}
		}
		ಆದರೆ (!confirm.len || strncasecmp(confirm.buf, "yes", confirm.len)) {
			*ಅಂಶ->string = '\0';
			ಬದಲಾಯಿತು++;
		}
	}

	ಆದರೆ (ಬದಲಾಯಿತು)
		string_list_remove_ಖಾಲಿಗಳು_ವಸ್ತುಗಳು(&del_list, 0);

	strbuf_release(&buf);
	strbuf_release(&confirm);
	ಹಿಂತಿರುಗಿ MENU_RETURN_NO_LOOP;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ quit_cmd(ಶೂನ್ಯ)
{
	string_list_clear(&del_list, 0);
	ಬರೆ(_("Bye.\n"));
	ಹಿಂತಿರುಗಿ MENU_RETURN_NO_LOOP;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ help_cmd(ಶೂನ್ಯ)
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
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ interactive_main_loop(ಶೂನ್ಯ)
{
	ಆಗ (del_list.nr) {
		ರಚನೆ menu_opts menu_opts;
		ರಚನೆ menu_stuff menu_stuff;
		ರಚನೆ menu_ಅಂಶ menus[] = {
			{'c', "clean",			0, clean_cmd},
			{'f', "filter by pattern",	0, filter_by_patterns_cmd},
			{'s', "select by numbers",	0, select_by_numbers_cmd},
			{'a', "ask each",		0, ask_each_cmd},
			{'q', "quit",			0, quit_cmd},
			{'h', "help",			0, help_cmd},
		};
		ಪೂರ್ಣಾಂಕ *chosen;

		menu_opts.ಶೀರ್ಷಿಕೆ = N_("*** Commands ***");
		menu_opts.prompt = N_("What now");
		menu_opts.flags = MENU_OPTS_SINGLETON;

		menu_stuff.type = MENU_STUFF_TYPE_MENU_ಅಂಶ;
		menu_stuff.stuff = menus;
		menu_stuff.nr = ಗಾತ್ರ(menus) / ಗಾತ್ರ(ರಚನೆ menu_ಅಂಶ);

		clean_print_color(CLEAN_COLOR_ಶೀರ್ಷಿಕೆ);
		printf_ln(Q_("Would remove the following item:",
			     "Would remove the following items:",
			     del_list.nr));
		clean_print_color(CLEAN_COLOR_RESET);

		pretty_print_dels();

		chosen = list_and_ಆರಿಸು(&menu_opts, &menu_stuff);

		ಆದರೆ (*chosen != EOF) {
			ಪೂರ್ಣಾಂಕ ret;
			ret = menus[*chosen].fn();
			ಆದರೆ (ret != MENU_RETURN_NO_LOOP) {
				FREE_AND_NULL(chosen);
				ಆದರೆ (!del_list.nr) {
					clean_print_color(CLEAN_COLOR_ERROR);
					printf_ln(_("No more files to clean, exiting."));
					clean_print_color(CLEAN_COLOR_RESET);
					ನಿಲ್ಲಿಸು;
				}
				ಮುಂದುವರಿ;
			}
		} ಇಲ್ಲದಿದ್ದರೆ {
			quit_cmd();
		}

		FREE_AND_NULL(chosen);
		ನಿಲ್ಲಿಸು;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ correct_untracked_entries(ರಚನೆ dir_struct *dir)
{
	ಪೂರ್ಣಾಂಕ src, dst, ign;

	ಗಾಗಿ (src = dst = ign = 0; src < dir->nr; src++) {
		/* skip paths in ignored[] that cannot be inside entries[src] */
		ಆಗ (ign < dir->ignored_nr &&
		       0 <= cmp_dir_ನಮೂದು(&dir->entries[src], &dir->ignored[ign]))
			ign++;

		ಆದರೆ (ign < dir->ignored_nr &&
		    check_dir_ನಮೂದು_ಒಳಗೊಂಡಿದೆ(dir->entries[src], dir->ignored[ign])) {
			/* entries[src] contains an ignored path, so we drop it */
			ಬಿಡುಗಡೆ(dir->entries[src]);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ರಚನೆ dir_ನಮೂದು *ent = dir->entries[src++];

			/* entries[src] does not contain an ignored path, so we keep it */
			dir->entries[dst++] = ent;

			/* then discard paths in entries[] contained inside entries[src] */
			ಆಗ (src < dir->nr &&
			       check_dir_ನಮೂದು_ಒಳಗೊಂಡಿದೆ(ent, dir->entries[src]))
				ಬಿಡುಗಡೆ(dir->entries[src++]);

			/* compensate for the outer loop's loop control */
			src--;
		}
	}
	dir->nr = dst;
}

ಪೂರ್ಣಾಂಕ cmd_clean(ಪೂರ್ಣಾಂಕ argc,
	      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	      ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ i, res;
	ಪೂರ್ಣಾಂಕ dry_ಸತತ = 0, remove_directories = 0, quiet = 0, ignored = 0;
	ಪೂರ್ಣಾಂಕ ignored_only = 0, force = 0, errors = 0, gone = 1;
	ಪೂರ್ಣಾಂಕ rm_flags = REMOVE_DIR_KEEP_NESTED_GIT;
	ರಚನೆ strbuf abs_ಮಾರ್ಗ = STRBUF_INIT;
	ರಚನೆ dir_struct dir = DIR_INIT;
	ರಚನೆ pathspec pathspec;
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ರಚನೆ string_list exclude_list = STRING_LIST_INIT_NODUP;
	ರಚನೆ ನಮೂನೆ_list *pl;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *qname;
	ರಚನೆ option options[] = {
		OPT__QUIET(&quiet, N_("do not print names of files removed")),
		OPT_DRY_ಸತತ(&dry_ಸತತ, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('i', "interactive", &interactive, N_("interactive cleaning")),
		OPT_BOOL('d', ಖಾಲಿ, &remove_directories,
				N_("remove whole directories")),
		OPT_CALLBACK_F('e', "exclude", &exclude_list, N_("pattern"),
		  N_("add <pattern> to ignore rules"), PARSE_OPT_NONEG, exclude_cb),
		OPT_BOOL('x', ಖಾಲಿ, &ignored, N_("remove ignored files, too")),
		OPT_BOOL('X', ಖಾಲಿ, &ignored_only,
				N_("remove only ignored files")),
		OPT_ಕೊನೆ()
	};

	repo_config(the_repository, git_clean_config, ಖಾಲಿ);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, builtin_clean_usage,
			     0);

	ಆದರೆ (require_force != 0 && !force && !interactive && !dry_ಸತತ)
		die(_("clean.requireForce is true and -f not given: refusing to clean"));

	ಆದರೆ (force > 1)
		rm_flags = 0;
	ಇಲ್ಲದಿದ್ದರೆ
		dir.flags |= DIR_SKIP_NESTED_GIT;

	dir.flags |= DIR_SHOW_OTHER_DIRECTORIES;

	ಆದರೆ (ignored && ignored_only)
		die(_("options '%s' and '%s' cannot be used together"), "-x", "-X");
	ಆದರೆ (!ignored)
		ಸಿದ್ಧತೆ_standard_excludes(&dir);
	ಆದರೆ (ignored_only)
		dir.flags |= DIR_SHOW_IGNORED;

	ಆದರೆ (argc) {
		/*
		 * Remaining args implies pathspecs specified, and we should
		 * recurse within those.
		 */
		remove_directories = 1;
	}

	ಆದರೆ (remove_directories && !ignored_only) {
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
		ಆದರೆ (!ignored)
			dir.flags |= DIR_SHOW_IGNORED_TOO_ಬಹುಲಕ_MATCHING;

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
	the_repository->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;

	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(the_repository) < 0)
		die(_("index file corrupt"));

	pl = ಕೂಡಿಸು_ನಮೂನೆ_list(&dir, EXC_CMDL, "--exclude option");
	ಗಾಗಿ (i = 0; i < exclude_list.nr; i++)
		ಕೂಡಿಸು_ನಮೂನೆ(exclude_list.ವಸ್ತುಗಳು[i].string, "", 0, pl, -(i+1));

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       ಪೂರ್ವಪ್ರತ್ಯಯ, argv);

	fill_directory(&dir, the_repository->ಸೂಚ್ಯಂಕ, &pathspec);
	correct_untracked_entries(&dir);

	ಗಾಗಿ (i = 0; i < dir.nr; i++) {
		ರಚನೆ dir_ನಮೂದು *ent = dir.entries[i];
		ರಚನೆ stat st;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rel;

		ಆದರೆ (!ಸೂಚ್ಯಂಕ_name_is_other(the_repository->ಸೂಚ್ಯಂಕ, ent->name, ent->len))
			ಮುಂದುವರಿ;

		ಆದರೆ (lstat(ent->name, &st))
			die_errno("Cannot lstat '%s'", ent->name);

		ಆದರೆ (S_ISDIR(st.st_ಬಹುಲಕ) && !remove_directories)
			ಮುಂದುವರಿ;

		rel = relative_ಮಾರ್ಗ(ent->name, ಪೂರ್ವಪ್ರತ್ಯಯ, &buf);
		string_list_append(&del_list, rel);
	}

	dir_clear(&dir);

	ಆದರೆ (interactive && del_list.nr > 0)
		interactive_main_loop();

	for_each_string_list_ಅಂಶ(ಅಂಶ, &del_list) {
		ರಚನೆ stat st;

		strbuf_reset(&abs_ಮಾರ್ಗ);
		ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ)
			strbuf_addstr(&abs_ಮಾರ್ಗ, ಪೂರ್ವಪ್ರತ್ಯಯ);

		strbuf_addstr(&abs_ಮಾರ್ಗ, ಅಂಶ->string);

		/*
		 * we might have removed this as part of earlier
		 * recursive directory removal, so lstat() here could
		 * fail with ENOENT.
		 */
		ಆದರೆ (lstat(abs_ಮಾರ್ಗ.buf, &st))
			ಮುಂದುವರಿ;

		ಆದರೆ (S_ISDIR(st.st_ಬಹುಲಕ)) {
			ಆದರೆ (remove_dirs(&abs_ಮಾರ್ಗ, ಪೂರ್ವಪ್ರತ್ಯಯ, rm_flags, dry_ಸತತ, quiet, &gone))
				errors++;
			ಆದರೆ (gone && !quiet) {
				qname = quote_ಮಾರ್ಗ(ಅಂಶ->string, ಖಾಲಿ, &buf, 0);
				ಬರೆ(dry_ಸತತ ? _(ಸಂದೇಶ_would_remove) : _(ಸಂದೇಶ_remove), qname);
			}
		} ಇಲ್ಲದಿದ್ದರೆ {
			res = dry_ಸತತ ? 0 : unlink(abs_ಮಾರ್ಗ.buf);
			ಆದರೆ (res) {
				ಪೂರ್ಣಾಂಕ saved_errno = errno;
				qname = quote_ಮಾರ್ಗ(ಅಂಶ->string, ಖಾಲಿ, &buf, 0);
				errno = saved_errno;
				warning_errno(_(ಸಂದೇಶ_warn_remove_failed), qname);
				errors++;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!quiet) {
				qname = quote_ಮಾರ್ಗ(ಅಂಶ->string, ಖಾಲಿ, &buf, 0);
				ಬರೆ(dry_ಸತತ ? _(ಸಂದೇಶ_would_remove) : _(ಸಂದೇಶ_remove), qname);
			}
		}
	}

	strbuf_release(&abs_ಮಾರ್ಗ);
	strbuf_release(&buf);
	string_list_clear(&del_list, 0);
	string_list_clear(&exclude_list, 0);
	clear_pathspec(&pathspec);
	ಹಿಂತಿರುಗಿ (errors != 0);
}
