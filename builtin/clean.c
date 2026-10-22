/*
 * "git clean" builtin command
 *
 * Copyright (C) 2007 Shawn Bohrer
 *
 * Based on git-clean.sh by Pavel Roskin
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache-ll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "column.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "color.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pathspec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "help.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "prompt.h"

ਸਥਿਰ ਪੂਰਨ require_force = -1; /* unset */
ਸਥਿਰ ਪੂਰਨ interactive;
ਸਥਿਰ ਬਣਤਰ string_list del_list = STRING_LIST_INIT_DUP;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ colopts;

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਅਚਲ builtin_clean_usage[] = {
	N_("git clean [-d] [-f] [-i] [-n] [-q] [-e <pattern>] [-x | -X] [--] [<pathspec>...]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_remove = N_("Removing %s\n");
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_would_remove = N_("Would remove %s\n");
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_skip_git_dir = N_("Skipping repository %s\n");
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_would_skip_git_dir = N_("Would skip repository %s\n");
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_warn_remove_failed = N_("failed to remove %s");
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_warn_lstat_failed = N_("could not lstat %s\n");
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_skip_cwd = N_("Refusing to remove current working directory\n");
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_would_skip_cwd = N_("Would refuse to remove current working directory\n");

ਗਿਣਤੀ color_clean {
	CLEAN_COLOR_RESET = 0,
	CLEAN_COLOR_PLAIN = 1,
	CLEAN_COLOR_PROMPT = 2,
	CLEAN_COLOR_ਸਿਰਲੇਖ = 3,
	CLEAN_COLOR_HELP = 4,
	CLEAN_COLOR_ERROR = 5
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *color_interactive_slots[] = {
	[CLEAN_COLOR_ERROR]  = "error",
	[CLEAN_COLOR_ਸਿਰਲੇਖ] = "header",
	[CLEAN_COLOR_HELP]   = "help",
	[CLEAN_COLOR_PLAIN]  = "plain",
	[CLEAN_COLOR_PROMPT] = "prompt",
	[CLEAN_COLOR_RESET]  = "reset",
};

ਸਥਿਰ ਗਿਣਤੀ git_colorbool clean_use_color = GIT_COLOR_UNKNOWN;
ਸਥਿਰ ਅੱਖਰ clean_colors[][COLOR_MAXLEN] = {
	[CLEAN_COLOR_ERROR] = GIT_COLOR_BOLD_RED,
	[CLEAN_COLOR_ਸਿਰਲੇਖ] = GIT_COLOR_BOLD,
	[CLEAN_COLOR_HELP] = GIT_COLOR_BOLD_RED,
	[CLEAN_COLOR_PLAIN] = GIT_COLOR_ਸਾਧਾਰਨ,
	[CLEAN_COLOR_PROMPT] = GIT_COLOR_BOLD_BLUE,
	[CLEAN_COLOR_RESET] = GIT_COLOR_RESET,
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MENU_OPTS_SINGLETON		01
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MENU_OPTS_ਤੁਰੰਤ		02
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MENU_OPTS_LIST_ONLY		04

ਬਣਤਰ menu_opts {
	ਅਚਲ ਅੱਖਰ *ਸਿਰਲੇਖ;
	ਅਚਲ ਅੱਖਰ *prompt;
	ਪੂਰਨ flags;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MENU_RETURN_NO_LOOP		10

ਬਣਤਰ menu_ਵਸਤੂ {
	ਅੱਖਰ hotkey;
	ਅਚਲ ਅੱਖਰ *title;
	ਪੂਰਨ selected;
	ਪੂਰਨ (*fn)(ਖਾਲੀ);
};

ਗਿਣਤੀ menu_stuff_type {
	MENU_STUFF_TYPE_STRING_LIST = 1,
	MENU_STUFF_TYPE_MENU_ਵਸਤੂ
};

ਬਣਤਰ menu_stuff {
	ਗਿਣਤੀ menu_stuff_type type;
	ਪੂਰਨ nr;
	ਖਾਲੀ *stuff;
};

define_list_config_array(color_interactive_slots);

ਸਥਿਰ ਪੂਰਨ git_clean_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
			    ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਅਚਲ ਅੱਖਰ *ਥਾਂ_name;

	ਜੇ (starts_with(var, "column."))
		ਵਾਪਸ git_ਕਾਲਮ_config(var, ਮੁੱਲ, "clean", &colopts);

	/* honors the color.interactive* config variables which also
	   applied in git-add--interactive and git-stash */
	ਜੇ (!strcmp(var, "color.interactive")) {
		clean_use_color = git_config_colorbool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}
	ਜੇ (skip_ਅਗੇਤਰ(var, "color.interactive.", &ਥਾਂ_name)) {
		ਪੂਰਨ ਥਾਂ = LOOKUP_CONFIG(color_interactive_slots, ਥਾਂ_name);
		ਜੇ (ਥਾਂ < 0)
			ਵਾਪਸ 0;
		ਜੇ (!ਮੁੱਲ)
			ਵਾਪਸ config_error_nonbool(var);
		ਵਾਪਸ color_parse(ਮੁੱਲ, clean_colors[ਥਾਂ]);
	}

	ਜੇ (!strcmp(var, "clean.requireforce")) {
		require_force = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (git_color_config(var, ਮੁੱਲ, cb) < 0)
		ਵਾਪਸ -1;

	ਵਾਪਸ git_default_config(var, ਮੁੱਲ, ctx, cb);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *clean_get_color(ਗਿਣਤੀ color_clean ix)
{
	ਜੇ (want_color(clean_use_color))
		ਵਾਪਸ clean_colors[ix];
	ਵਾਪਸ "";
}

ਸਥਿਰ ਖਾਲੀ clean_print_color(ਗਿਣਤੀ color_clean ix)
{
	ਲਿਖੋ("%s", clean_get_color(ix));
}

ਸਥਿਰ ਪੂਰਨ exclude_cb(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ string_list *exclude_list = opt->ਮੁੱਲ;
	BUG_ON_OPT_NEG(unset);
	string_list_append(exclude_list, arg);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ remove_dirs(ਬਣਤਰ strbuf *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ, ਪੂਰਨ force_flag,
		ਪੂਰਨ dry_ਲੜੀਵਾਰ, ਪੂਰਨ quiet, ਪੂਰਨ *dir_gone)
{
	DIR *dir;
	ਬਣਤਰ strbuf quoted = STRBUF_INIT;
	ਬਣਤਰ strbuf realpath = STRBUF_INIT;
	ਬਣਤਰ strbuf real_ocwd = STRBUF_INIT;
	ਬਣਤਰ dirent *e;
	ਪੂਰਨ res = 0, ret = 0, gone = 1, original_len = ਰਸਤਾ->len, len;
	ਬਣਤਰ string_list dels = STRING_LIST_INIT_DUP;

	*dir_gone = 1;

	ਜੇ ((force_flag & REMOVE_DIR_KEEP_NESTED_GIT) &&
	    is_nonbare_repository_dir(ਰਸਤਾ)) {
		ਜੇ (!quiet) {
			quote_ਰਸਤਾ(ਰਸਤਾ->buf, ਅਗੇਤਰ, &quoted, 0);
			ਲਿਖੋ(dry_ਲੜੀਵਾਰ ?  _(ਸੁਨੇਹਾ_would_skip_git_dir) : _(ਸੁਨੇਹਾ_skip_git_dir),
					quoted.buf);
		}

		*dir_gone = 0;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	dir = opendir(ਰਸਤਾ->buf);
	ਜੇ (!dir) {
		/* an empty dir could be removed even if it is unreadble */
		res = dry_ਲੜੀਵਾਰ ? 0 : rmdir(ਰਸਤਾ->buf);
		ਜੇ (res) {
			ਪੂਰਨ saved_errno = errno;
			quote_ਰਸਤਾ(ਰਸਤਾ->buf, ਅਗੇਤਰ, &quoted, 0);
			errno = saved_errno;
			warning_errno(_(ਸੁਨੇਹਾ_warn_remove_failed), quoted.buf);
			*dir_gone = 0;
		}
		ret = res;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	strbuf_complete(ਰਸਤਾ, '/');

	len = ਰਸਤਾ->len;
	ਜਦੋਂ ((e = readdir_skip_dot_and_dotdot(dir)) != ਨਿਰਾ) {
		ਬਣਤਰ stat st;

		strbuf_setlen(ਰਸਤਾ, len);
		strbuf_addstr(ਰਸਤਾ, e->d_name);
		ਜੇ (lstat(ਰਸਤਾ->buf, &st))
			warning_errno(_(ਸੁਨੇਹਾ_warn_lstat_failed), ਰਸਤਾ->buf);
		ਨਹੀਂਤਾਂ ਜੇ (S_ISDIR(st.st_ਬਹੁਲਕ)) {
			ਜੇ (remove_dirs(ਰਸਤਾ, ਅਗੇਤਰ, force_flag, dry_ਲੜੀਵਾਰ, quiet, &gone))
				ret = 1;
			ਜੇ (gone) {
				quote_ਰਸਤਾ(ਰਸਤਾ->buf, ਅਗੇਤਰ, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} ਨਹੀਂਤਾਂ
				*dir_gone = 0;
			ਜਾਰੀ;
		} ਨਹੀਂਤਾਂ {
			res = dry_ਲੜੀਵਾਰ ? 0 : unlink(ਰਸਤਾ->buf);
			ਜੇ (!res) {
				quote_ਰਸਤਾ(ਰਸਤਾ->buf, ਅਗੇਤਰ, &quoted, 0);
				string_list_append(&dels, quoted.buf);
			} ਨਹੀਂਤਾਂ {
				ਪੂਰਨ saved_errno = errno;
				quote_ਰਸਤਾ(ਰਸਤਾ->buf, ਅਗੇਤਰ, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(ਸੁਨੇਹਾ_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
			ਜਾਰੀ;
		}

		/* path too long, stat fails, or non-directory still exists */
		*dir_gone = 0;
		ret = 1;
		ਰੋਕੋ;
	}
	closedir(dir);

	strbuf_setlen(ਰਸਤਾ, original_len);

	ਜੇ (*dir_gone) {
		/*
		 * Normalize path components in path->buf, e.g. change '\' to
		 * '/' on Windows.
		 */
		strbuf_realpath(&realpath, ਰਸਤਾ->buf, 1);

		/*
		 * path and realpath are absolute; for comparison, we would
		 * like to transform startup_info->original_cwd to an absolute
		 * path too.
		 */
		 ਜੇ (startup_info->original_cwd)
			 strbuf_realpath(&real_ocwd,
					 startup_info->original_cwd, 1);

		ਜੇ (!strbuf_cmp(&realpath, &real_ocwd)) {
			ਲਿਖੋ("%s", dry_ਲੜੀਵਾਰ ? _(ਸੁਨੇਹਾ_would_skip_cwd) : _(ਸੁਨੇਹਾ_skip_cwd));
			*dir_gone = 0;
		} ਨਹੀਂਤਾਂ {
			res = dry_ਲੜੀਵਾਰ ? 0 : rmdir(ਰਸਤਾ->buf);
			ਜੇ (!res)
				*dir_gone = 1;
			ਨਹੀਂਤਾਂ {
				ਪੂਰਨ saved_errno = errno;
				quote_ਰਸਤਾ(ਰਸਤਾ->buf, ਅਗੇਤਰ, &quoted, 0);
				errno = saved_errno;
				warning_errno(_(ਸੁਨੇਹਾ_warn_remove_failed), quoted.buf);
				*dir_gone = 0;
				ret = 1;
			}
		}
	}

	ਜੇ (!*dir_gone && !quiet) {
		ਪੂਰਨ i;
		ਲਈ (i = 0; i < dels.nr; i++)
			ਲਿਖੋ(dry_ਲੜੀਵਾਰ ?  _(ਸੁਨੇਹਾ_would_remove) : _(ਸੁਨੇਹਾ_remove), dels.ਚੀਜ਼ਾਂ[i].string);
	}
ਬਾਹਰਲਾ:
	strbuf_release(&realpath);
	strbuf_release(&real_ocwd);
	strbuf_release(&quoted);
	string_list_clear(&dels, 0);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ pretty_print_dels(ਖਾਲੀ)
{
	ਬਣਤਰ string_list list = STRING_LIST_INIT_DUP;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *qname;
	ਬਣਤਰ ਕਾਲਮ_options copts;

	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &del_list) {
		qname = quote_ਰਸਤਾ(ਵਸਤੂ->string, ਨਿਰਾ, &buf, 0);
		string_list_append(&list, qname);
	}

	/*
	 * always enable column display, we only consult column.*
	 * about layout strategy and stuff
	 */
	colopts = (colopts & ~ਖਾਨਾ_ENABLE_ਨਕਾਬ) | ਖਾਨਾ_ENABLED;
	memset(&copts, 0, ਆਕਾਰ(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(&list, colopts, &copts);
	strbuf_release(&buf);
	string_list_clear(&list, 0);
}

ਸਥਿਰ ਖਾਲੀ pretty_print_menus(ਬਣਤਰ string_list *menu_list)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ local_colopts = 0;
	ਬਣਤਰ ਕਾਲਮ_options copts;

	local_colopts = ਖਾਨਾ_ENABLED | ਖਾਨਾ_ਪੰਕਤੀ;
	memset(&copts, 0, ਆਕਾਰ(copts));
	copts.indent = "  ";
	copts.padding = 2;
	print_columns(menu_list, local_colopts, &copts);
}

ਸਥਿਰ ਖਾਲੀ prompt_help_cmd(ਪੂਰਨ singleton)
{
	clean_print_color(CLEAN_COLOR_HELP);
	ਲਿਖੋ(singleton ?
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
ਸਥਿਰ ਖਾਲੀ print_highlight_menu_stuff(ਬਣਤਰ menu_stuff *stuff, ਪੂਰਨ **chosen)
{
	ਬਣਤਰ string_list menu_list = STRING_LIST_INIT_DUP;
	ਬਣਤਰ strbuf menu = STRBUF_INIT;
	ਬਣਤਰ menu_ਵਸਤੂ *menu_ਵਸਤੂ;
	ਬਣਤਰ string_list_ਵਸਤੂ *string_list_ਵਸਤੂ;
	ਪੂਰਨ i;

	ਚੋਣ (stuff->type) {
	ਮੂਲ:
		die("Bad type of menu_stuff when print menu");
	ਹਾਲਤ MENU_STUFF_TYPE_MENU_ਵਸਤੂ:
		menu_ਵਸਤੂ = (ਬਣਤਰ menu_ਵਸਤੂ *)stuff->stuff;
		ਲਈ (i = 0; i < stuff->nr; i++, menu_ਵਸਤੂ++) {
			ਅਚਲ ਅੱਖਰ *p;
			ਪੂਰਨ highlighted = 0;

			p = menu_ਵਸਤੂ->title;
			ਜੇ ((*chosen)[i] < 0)
				(*chosen)[i] = menu_ਵਸਤੂ->selected ? 1 : 0;
			strbuf_addf(&menu, "%s%2d: ", (*chosen)[i] ? "*" : " ", i+1);
			ਲਈ (; *p; p++) {
				ਜੇ (!highlighted && *p == menu_ਵਸਤੂ->hotkey) {
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_PROMPT));
					strbuf_addch(&menu, *p);
					strbuf_addstr(&menu, clean_get_color(CLEAN_COLOR_RESET));
					highlighted = 1;
				} ਨਹੀਂਤਾਂ {
					strbuf_addch(&menu, *p);
				}
			}
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
		}
		ਰੋਕੋ;
	ਹਾਲਤ MENU_STUFF_TYPE_STRING_LIST:
		i = 0;
		for_each_string_list_ਵਸਤੂ(string_list_ਵਸਤੂ, (ਬਣਤਰ string_list *)stuff->stuff) {
			ਜੇ ((*chosen)[i] < 0)
				(*chosen)[i] = 0;
			strbuf_addf(&menu, "%s%2d: %s",
				    (*chosen)[i] ? "*" : " ", i+1, string_list_ਵਸਤੂ->string);
			string_list_append(&menu_list, menu.buf);
			strbuf_reset(&menu);
			i++;
		}
		ਰੋਕੋ;
	}

	pretty_print_menus(&menu_list);

	strbuf_release(&menu);
	string_list_clear(&menu_list, 0);
}

ਸਥਿਰ ਪੂਰਨ ਲੱਭੋ_unique(ਅਚਲ ਅੱਖਰ *choice, ਬਣਤਰ menu_stuff *menu_stuff)
{
	ਬਣਤਰ menu_ਵਸਤੂ *menu_ਵਸਤੂ;
	ਬਣਤਰ string_list_ਵਸਤੂ *string_list_ਵਸਤੂ;
	ਪੂਰਨ i, len, ਮਿਲਿਆ_2 = 0;

	len = ਲੜੀ_ਲੰਬਾਈ(choice);
	ਚੋਣ (menu_stuff->type) {
	ਮੂਲ:
		die("Bad type of menu_stuff when parse choice");
	ਹਾਲਤ MENU_STUFF_TYPE_MENU_ਵਸਤੂ:

		menu_ਵਸਤੂ = (ਬਣਤਰ menu_ਵਸਤੂ *)menu_stuff->stuff;
		ਲਈ (i = 0; i < menu_stuff->nr; i++, menu_ਵਸਤੂ++) {
			ਜੇ (len == 1 && *choice == menu_ਵਸਤੂ->hotkey) {
				ਮਿਲਿਆ_2 = i + 1;
				ਰੋਕੋ;
			}
			ਜੇ (!strncasecmp(choice, menu_ਵਸਤੂ->title, len)) {
				ਜੇ (ਮਿਲਿਆ_2) {
					ਜੇ (len == 1) {
						/* continue for hotkey matching */
						ਮਿਲਿਆ_2 = -1;
					} ਨਹੀਂਤਾਂ {
						ਮਿਲਿਆ_2 = 0;
						ਰੋਕੋ;
					}
				} ਨਹੀਂਤਾਂ {
					ਮਿਲਿਆ_2 = i + 1;
				}
			}
		}
		ਰੋਕੋ;
	ਹਾਲਤ MENU_STUFF_TYPE_STRING_LIST:
		string_list_ਵਸਤੂ = ((ਬਣਤਰ string_list *)menu_stuff->stuff)->ਚੀਜ਼ਾਂ;
		ਲਈ (i = 0; i < menu_stuff->nr; i++, string_list_ਵਸਤੂ++) {
			ਜੇ (!strncasecmp(choice, string_list_ਵਸਤੂ->string, len)) {
				ਜੇ (ਮਿਲਿਆ_2) {
					ਮਿਲਿਆ_2 = 0;
					ਰੋਕੋ;
				}
				ਮਿਲਿਆ_2 = i + 1;
			}
		}
		ਰੋਕੋ;
	}
	ਵਾਪਸ ਮਿਲਿਆ_2;
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
ਸਥਿਰ ਪੂਰਨ parse_choice(ਬਣਤਰ menu_stuff *menu_stuff,
			ਪੂਰਨ is_single,
			ਅੱਖਰ *ਨਿਵੇਸ਼,
			ਪੂਰਨ **chosen)
{
	ਬਣਤਰ string_list choice = STRING_LIST_INIT_NODUP;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਪੂਰਨ nr = 0;
	ਪੂਰਨ i;

	string_list_ਵੰਡੋ_ਵਿੱਚ_ਰੱਖੋ_f(&choice, ਨਿਵੇਸ਼,
				     is_single ? "\n" : ", ", -1,
				     STRING_LIST_ਵੰਡੋ_TRIM);

	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &choice) {
		ਅਚਲ ਅੱਖਰ *string;
		ਪੂਰਨ ਚੁਣੋ;
		ਪੂਰਨ bottom = 0, ਸਿਖਰ = 0;
		ਪੂਰਨ is_range, is_ਸੰਖਿਆ;

		string = ਵਸਤੂ->string;
		ਜੇ (!*string)
			ਜਾਰੀ;

		/* Input that begins with '-'; unchoose */
		ਜੇ (string[0] == '-') {
			ਚੁਣੋ = 0;
			string++;
		} ਨਹੀਂਤਾਂ {
			ਚੁਣੋ = 1;
		}

		is_range = 0;
		is_ਸੰਖਿਆ = 1;
		ਲਈ (ਅਚਲ ਅੱਖਰ *p = string; *p; p++) {
			ਜੇ ('-' == *p) {
				ਜੇ (!is_range) {
					is_range = 1;
					is_ਸੰਖਿਆ = 0;
				} ਨਹੀਂਤਾਂ {
					is_ਸੰਖਿਆ = 0;
					is_range = 0;
					ਰੋਕੋ;
				}
			} ਨਹੀਂਤਾਂ ਜੇ (!ਅੰਕ_ਹੈ(*p)) {
				is_ਸੰਖਿਆ = 0;
				is_range = 0;
				ਰੋਕੋ;
			}
		}

		ਜੇ (is_ਸੰਖਿਆ) {
			bottom = atoi(string);
			ਸਿਖਰ = bottom;
		} ਨਹੀਂਤਾਂ ਜੇ (is_range) {
			bottom = atoi(string);
			/* a range can be specified like 5-7 or 5- */
			ਜੇ (!*(strchr(string, '-') + 1))
				ਸਿਖਰ = menu_stuff->nr;
			ਨਹੀਂਤਾਂ
				ਸਿਖਰ = atoi(strchr(string, '-') + 1);
		} ਨਹੀਂਤਾਂ ਜੇ (!strcmp(string, "*")) {
			bottom = 1;
			ਸਿਖਰ = menu_stuff->nr;
		} ਨਹੀਂਤਾਂ {
			bottom = ਲੱਭੋ_unique(string, menu_stuff);
			ਸਿਖਰ = bottom;
		}

		ਜੇ (ਸਿਖਰ <= 0 || bottom <= 0 || ਸਿਖਰ > menu_stuff->nr || bottom > ਸਿਖਰ ||
		    (is_single && bottom != ਸਿਖਰ)) {
			clean_print_color(CLEAN_COLOR_ERROR);
			ਲਿਖੋ(_("Huh (%s)?\n"), string);
			clean_print_color(CLEAN_COLOR_RESET);
			ਜਾਰੀ;
		}

		ਲਈ (i = bottom; i <= ਸਿਖਰ; i++)
			(*chosen)[i-1] = ਚੁਣੋ;
	}

	string_list_clear(&choice, 0);

	ਲਈ (i = 0; i < menu_stuff->nr; i++)
		nr += (*chosen)[i];
	ਵਾਪਸ nr;
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
ਸਥਿਰ ਪੂਰਨ *list_and_ਚੁਣੋ(ਬਣਤਰ menu_opts *opts, ਬਣਤਰ menu_stuff *stuff)
{
	ਬਣਤਰ strbuf choice = STRBUF_INIT;
	ਪੂਰਨ *chosen, *ਨਤੀਜਾ;
	ਪੂਰਨ nr = 0;
	ਪੂਰਨ eof = 0;
	ਪੂਰਨ i;

	ALLOC_ARRAY(chosen, stuff->nr);
	/* set chosen as uninitialized */
	ਲਈ (i = 0; i < stuff->nr; i++)
		chosen[i] = -1;

	ਲਈ (;;) {
		ਜੇ (opts->ਸਿਰਲੇਖ) {
			printf_ln("%s%s%s",
				  clean_get_color(CLEAN_COLOR_ਸਿਰਲੇਖ),
				  _(opts->ਸਿਰਲੇਖ),
				  clean_get_color(CLEAN_COLOR_RESET));
		}

		/* chosen will be initialized by print_highlight_menu_stuff */
		print_highlight_menu_stuff(stuff, &chosen);

		ਜੇ (opts->flags & MENU_OPTS_LIST_ONLY)
			ਰੋਕੋ;

		ਜੇ (opts->prompt) {
			ਲਿਖੋ("%s%s%s%s",
			       clean_get_color(CLEAN_COLOR_PROMPT),
			       _(opts->prompt),
			       opts->flags & MENU_OPTS_SINGLETON ? "> " : ">> ",
			       clean_get_color(CLEAN_COLOR_RESET));
		}

		ਜੇ (git_read_ਸਤਰ_interactively(&choice) == EOF) {
			eof = 1;
			ਰੋਕੋ;
		}

		/* help for prompt */
		ਜੇ (!strcmp(choice.buf, "?")) {
			prompt_help_cmd(opts->flags & MENU_OPTS_SINGLETON);
			ਜਾਰੀ;
		}

		/* for a multiple-choice menu, press ENTER (empty) will return back */
		ਜੇ (!(opts->flags & MENU_OPTS_SINGLETON) && !choice.len)
			ਰੋਕੋ;

		nr = parse_choice(stuff,
				  opts->flags & MENU_OPTS_SINGLETON,
				  choice.buf,
				  &chosen);

		ਜੇ (opts->flags & MENU_OPTS_SINGLETON) {
			ਜੇ (nr)
				ਰੋਕੋ;
		} ਨਹੀਂਤਾਂ ਜੇ (opts->flags & MENU_OPTS_ਤੁਰੰਤ) {
			ਰੋਕੋ;
		}
	}

	ਜੇ (eof) {
		ਨਤੀਜਾ = xmalloc(ਆਕਾਰ(ਪੂਰਨ));
		*ਨਤੀਜਾ = EOF;
	} ਨਹੀਂਤਾਂ {
		ਪੂਰਨ j = 0;

		/*
		 * recalculate nr, if return back from menu directly with
		 * default selections.
		 */
		ਜੇ (!nr) {
			ਲਈ (i = 0; i < stuff->nr; i++)
				nr += chosen[i];
		}

		CALLOC_ARRAY(ਨਤੀਜਾ, st_ਜੋੜੋ(nr, 1));
		ਲਈ (i = 0; i < stuff->nr && j < nr; i++) {
			ਜੇ (chosen[i])
				ਨਤੀਜਾ[j++] = i;
		}
		ਨਤੀਜਾ[j] = EOF;
	}

	ਮੁਕਤ(chosen);
	strbuf_release(&choice);
	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਪੂਰਨ clean_cmd(ਖਾਲੀ)
{
	ਵਾਪਸ MENU_RETURN_NO_LOOP;
}

ਸਥਿਰ ਪੂਰਨ filter_by_patterns_cmd(ਖਾਲੀ)
{
	ਬਣਤਰ dir_struct dir = DIR_INIT;
	ਬਣਤਰ strbuf confirm = STRBUF_INIT;
	ਬਣਤਰ ਨਮੂਨਾ_list *pl;
	ਪੂਰਨ ਬਦਲਿਆ = -1, i;

	ਲਈ (;;) {
		ਬਣਤਰ string_list ignore_list = STRING_LIST_INIT_NODUP;
		ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;

		ਜੇ (!del_list.nr)
			ਰੋਕੋ;

		ਜੇ (ਬਦਲਿਆ)
			pretty_print_dels();

		clean_print_color(CLEAN_COLOR_PROMPT);
		ਲਿਖੋ(_("Input ignore patterns>> "));
		clean_print_color(CLEAN_COLOR_RESET);
		ਜੇ (git_read_ਸਤਰ_interactively(&confirm) == EOF)
			ਅੱਖਰ_ਲਿਖੋ('\n');

		/* quit filter_by_pattern mode if press ENTER or Ctrl-D */
		ਜੇ (!confirm.len)
			ਰੋਕੋ;

		pl = ਜੋੜੋ_ਨਮੂਨਾ_list(&dir, EXC_CMDL, "manual exclude");

		string_list_ਵੰਡੋ_ਵਿੱਚ_ਰੱਖੋ_f(&ignore_list, confirm.buf, " ", -1,
					     STRING_LIST_ਵੰਡੋ_TRIM);

		ਲਈ (i = 0; i < ignore_list.nr; i++) {
			ਵਸਤੂ = &ignore_list.ਚੀਜ਼ਾਂ[i];
			ਜੇ (!*ਵਸਤੂ->string)
				ਜਾਰੀ;
			ਜੋੜੋ_ਨਮੂਨਾ(ਵਸਤੂ->string, "", 0, pl, -(i+1));
		}

		ਬਦਲਿਆ = 0;
		for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &del_list) {
			ਪੂਰਨ dtype = DT_UNKNOWN;

			ਜੇ (is_excluded(&dir, the_repository->ਸੂਚਕਾਂਕ, ਵਸਤੂ->string, &dtype)) {
				*ਵਸਤੂ->string = '\0';
				ਬਦਲਿਆ++;
			}
		}

		ਜੇ (ਬਦਲਿਆ) {
			string_list_remove_ਖਾਲੀ_ਥਾਵਾਂ_ਚੀਜ਼ਾਂ(&del_list, 0);
		} ਨਹੀਂਤਾਂ {
			clean_print_color(CLEAN_COLOR_ERROR);
			printf_ln(_("WARNING: Cannot find items matched by: %s"), confirm.buf);
			clean_print_color(CLEAN_COLOR_RESET);
		}

		string_list_clear(&ignore_list, 0);
		dir_clear(&dir);
	}

	strbuf_release(&confirm);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ select_by_numbers_cmd(ਖਾਲੀ)
{
	ਬਣਤਰ menu_opts menu_opts;
	ਬਣਤਰ menu_stuff menu_stuff;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਚੀਜ਼ਾਂ;
	ਪੂਰਨ *chosen;
	ਪੂਰਨ i, j;

	menu_opts.ਸਿਰਲੇਖ = ਨਿਰਾ;
	menu_opts.prompt = N_("Select items to delete");
	menu_opts.flags = 0;

	menu_stuff.type = MENU_STUFF_TYPE_STRING_LIST;
	menu_stuff.stuff = &del_list;
	menu_stuff.nr = del_list.nr;

	chosen = list_and_ਚੁਣੋ(&menu_opts, &menu_stuff);
	ਚੀਜ਼ਾਂ = del_list.ਚੀਜ਼ਾਂ;
	ਲਈ (i = 0, j = 0; i < del_list.nr; i++) {
		ਜੇ (i < chosen[j]) {
			*(ਚੀਜ਼ਾਂ[i].string) = '\0';
		} ਨਹੀਂਤਾਂ ਜੇ (i == chosen[j]) {
			/* delete selected item */
			j++;
			ਜਾਰੀ;
		} ਨਹੀਂਤਾਂ {
			/* end of chosen (chosen[j] == EOF), won't delete */
			*(ਚੀਜ਼ਾਂ[i].string) = '\0';
		}
	}

	string_list_remove_ਖਾਲੀ_ਥਾਵਾਂ_ਚੀਜ਼ਾਂ(&del_list, 0);

	ਮੁਕਤ(chosen);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ask_each_cmd(ਖਾਲੀ)
{
	ਬਣਤਰ strbuf confirm = STRBUF_INIT;
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਅਚਲ ਅੱਖਰ *qname;
	ਪੂਰਨ ਬਦਲਿਆ = 0, eof = 0;

	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &del_list) {
		/* Ctrl-D should stop removing files */
		ਜੇ (!eof) {
			qname = quote_ਰਸਤਾ(ਵਸਤੂ->string, ਨਿਰਾ, &buf, 0);
			/* TRANSLATORS: Make sure to keep [y/N] as is */
			ਲਿਖੋ(_("Remove %s [y/N]? "), qname);
			ਜੇ (git_read_ਸਤਰ_interactively(&confirm) == EOF) {
				ਅੱਖਰ_ਲਿਖੋ('\n');
				eof = 1;
			}
		}
		ਜੇ (!confirm.len || strncasecmp(confirm.buf, "yes", confirm.len)) {
			*ਵਸਤੂ->string = '\0';
			ਬਦਲਿਆ++;
		}
	}

	ਜੇ (ਬਦਲਿਆ)
		string_list_remove_ਖਾਲੀ_ਥਾਵਾਂ_ਚੀਜ਼ਾਂ(&del_list, 0);

	strbuf_release(&buf);
	strbuf_release(&confirm);
	ਵਾਪਸ MENU_RETURN_NO_LOOP;
}

ਸਥਿਰ ਪੂਰਨ quit_cmd(ਖਾਲੀ)
{
	string_list_clear(&del_list, 0);
	ਲਿਖੋ(_("Bye.\n"));
	ਵਾਪਸ MENU_RETURN_NO_LOOP;
}

ਸਥਿਰ ਪੂਰਨ help_cmd(ਖਾਲੀ)
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
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ interactive_main_loop(ਖਾਲੀ)
{
	ਜਦੋਂ (del_list.nr) {
		ਬਣਤਰ menu_opts menu_opts;
		ਬਣਤਰ menu_stuff menu_stuff;
		ਬਣਤਰ menu_ਵਸਤੂ menus[] = {
			{'c', "clean",			0, clean_cmd},
			{'f', "filter by pattern",	0, filter_by_patterns_cmd},
			{'s', "select by numbers",	0, select_by_numbers_cmd},
			{'a', "ask each",		0, ask_each_cmd},
			{'q', "quit",			0, quit_cmd},
			{'h', "help",			0, help_cmd},
		};
		ਪੂਰਨ *chosen;

		menu_opts.ਸਿਰਲੇਖ = N_("*** Commands ***");
		menu_opts.prompt = N_("What now");
		menu_opts.flags = MENU_OPTS_SINGLETON;

		menu_stuff.type = MENU_STUFF_TYPE_MENU_ਵਸਤੂ;
		menu_stuff.stuff = menus;
		menu_stuff.nr = ਆਕਾਰ(menus) / ਆਕਾਰ(ਬਣਤਰ menu_ਵਸਤੂ);

		clean_print_color(CLEAN_COLOR_ਸਿਰਲੇਖ);
		printf_ln(Q_("Would remove the following item:",
			     "Would remove the following items:",
			     del_list.nr));
		clean_print_color(CLEAN_COLOR_RESET);

		pretty_print_dels();

		chosen = list_and_ਚੁਣੋ(&menu_opts, &menu_stuff);

		ਜੇ (*chosen != EOF) {
			ਪੂਰਨ ret;
			ret = menus[*chosen].fn();
			ਜੇ (ret != MENU_RETURN_NO_LOOP) {
				FREE_AND_NULL(chosen);
				ਜੇ (!del_list.nr) {
					clean_print_color(CLEAN_COLOR_ERROR);
					printf_ln(_("No more files to clean, exiting."));
					clean_print_color(CLEAN_COLOR_RESET);
					ਰੋਕੋ;
				}
				ਜਾਰੀ;
			}
		} ਨਹੀਂਤਾਂ {
			quit_cmd();
		}

		FREE_AND_NULL(chosen);
		ਰੋਕੋ;
	}
}

ਸਥਿਰ ਖਾਲੀ correct_untracked_entries(ਬਣਤਰ dir_struct *dir)
{
	ਪੂਰਨ src, dst, ign;

	ਲਈ (src = dst = ign = 0; src < dir->nr; src++) {
		/* skip paths in ignored[] that cannot be inside entries[src] */
		ਜਦੋਂ (ign < dir->ignored_nr &&
		       0 <= cmp_dir_ਇੰਦਰਾਜ(&dir->entries[src], &dir->ignored[ign]))
			ign++;

		ਜੇ (ign < dir->ignored_nr &&
		    check_dir_ਇੰਦਰਾਜ_ਸ਼ਾਮਲ_ਹੈ(dir->entries[src], dir->ignored[ign])) {
			/* entries[src] contains an ignored path, so we drop it */
			ਮੁਕਤ(dir->entries[src]);
		} ਨਹੀਂਤਾਂ {
			ਬਣਤਰ dir_ਇੰਦਰਾਜ *ent = dir->entries[src++];

			/* entries[src] does not contain an ignored path, so we keep it */
			dir->entries[dst++] = ent;

			/* then discard paths in entries[] contained inside entries[src] */
			ਜਦੋਂ (src < dir->nr &&
			       check_dir_ਇੰਦਰਾਜ_ਸ਼ਾਮਲ_ਹੈ(ent, dir->entries[src]))
				ਮੁਕਤ(dir->entries[src++]);

			/* compensate for the outer loop's loop control */
			src--;
		}
	}
	dir->nr = dst;
}

ਪੂਰਨ cmd_clean(ਪੂਰਨ argc,
	      ਅਚਲ ਅੱਖਰ **argv,
	      ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	      ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ i, res;
	ਪੂਰਨ dry_ਲੜੀਵਾਰ = 0, remove_directories = 0, quiet = 0, ignored = 0;
	ਪੂਰਨ ignored_only = 0, force = 0, errors = 0, gone = 1;
	ਪੂਰਨ rm_flags = REMOVE_DIR_KEEP_NESTED_GIT;
	ਬਣਤਰ strbuf abs_ਰਸਤਾ = STRBUF_INIT;
	ਬਣਤਰ dir_struct dir = DIR_INIT;
	ਬਣਤਰ pathspec pathspec;
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ string_list exclude_list = STRING_LIST_INIT_NODUP;
	ਬਣਤਰ ਨਮੂਨਾ_list *pl;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;
	ਅਚਲ ਅੱਖਰ *qname;
	ਬਣਤਰ option options[] = {
		OPT__QUIET(&quiet, N_("do not print names of files removed")),
		OPT_DRY_ਲੜੀਵਾਰ(&dry_ਲੜੀਵਾਰ, N_("dry run")),
		OPT__FORCE(&force, N_("force"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL('i', "interactive", &interactive, N_("interactive cleaning")),
		OPT_BOOL('d', ਨਿਰਾ, &remove_directories,
				N_("remove whole directories")),
		OPT_CALLBACK_F('e', "exclude", &exclude_list, N_("pattern"),
		  N_("add <pattern> to ignore rules"), PARSE_OPT_NONEG, exclude_cb),
		OPT_BOOL('x', ਨਿਰਾ, &ignored, N_("remove ignored files, too")),
		OPT_BOOL('X', ਨਿਰਾ, &ignored_only,
				N_("remove only ignored files")),
		OPT_ਸਿਰਾ()
	};

	repo_config(the_repository, git_clean_config, ਨਿਰਾ);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, builtin_clean_usage,
			     0);

	ਜੇ (require_force != 0 && !force && !interactive && !dry_ਲੜੀਵਾਰ)
		die(_("clean.requireForce is true and -f not given: refusing to clean"));

	ਜੇ (force > 1)
		rm_flags = 0;
	ਨਹੀਂਤਾਂ
		dir.flags |= DIR_SKIP_NESTED_GIT;

	dir.flags |= DIR_SHOW_OTHER_DIRECTORIES;

	ਜੇ (ignored && ignored_only)
		die(_("options '%s' and '%s' cannot be used together"), "-x", "-X");
	ਜੇ (!ignored)
		ਤਿਆਰੀ_standard_excludes(&dir);
	ਜੇ (ignored_only)
		dir.flags |= DIR_SHOW_IGNORED;

	ਜੇ (argc) {
		/*
		 * Remaining args implies pathspecs specified, and we should
		 * recurse within those.
		 */
		remove_directories = 1;
	}

	ਜੇ (remove_directories && !ignored_only) {
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
		ਜੇ (!ignored)
			dir.flags |= DIR_SHOW_IGNORED_TOO_ਬਹੁਲਕ_MATCHING;

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
	the_repository->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;

	ਜੇ (repo_read_ਸੂਚਕਾਂਕ(the_repository) < 0)
		die(_("index file corrupt"));

	pl = ਜੋੜੋ_ਨਮੂਨਾ_list(&dir, EXC_CMDL, "--exclude option");
	ਲਈ (i = 0; i < exclude_list.nr; i++)
		ਜੋੜੋ_ਨਮੂਨਾ(exclude_list.ਚੀਜ਼ਾਂ[i].string, "", 0, pl, -(i+1));

	parse_pathspec(&pathspec, 0,
		       PATHSPEC_PREFER_CWD,
		       ਅਗੇਤਰ, argv);

	fill_directory(&dir, the_repository->ਸੂਚਕਾਂਕ, &pathspec);
	correct_untracked_entries(&dir);

	ਲਈ (i = 0; i < dir.nr; i++) {
		ਬਣਤਰ dir_ਇੰਦਰਾਜ *ent = dir.entries[i];
		ਬਣਤਰ stat st;
		ਅਚਲ ਅੱਖਰ *rel;

		ਜੇ (!ਸੂਚਕਾਂਕ_name_is_other(the_repository->ਸੂਚਕਾਂਕ, ent->name, ent->len))
			ਜਾਰੀ;

		ਜੇ (lstat(ent->name, &st))
			die_errno("Cannot lstat '%s'", ent->name);

		ਜੇ (S_ISDIR(st.st_ਬਹੁਲਕ) && !remove_directories)
			ਜਾਰੀ;

		rel = relative_ਰਸਤਾ(ent->name, ਅਗੇਤਰ, &buf);
		string_list_append(&del_list, rel);
	}

	dir_clear(&dir);

	ਜੇ (interactive && del_list.nr > 0)
		interactive_main_loop();

	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &del_list) {
		ਬਣਤਰ stat st;

		strbuf_reset(&abs_ਰਸਤਾ);
		ਜੇ (ਅਗੇਤਰ)
			strbuf_addstr(&abs_ਰਸਤਾ, ਅਗੇਤਰ);

		strbuf_addstr(&abs_ਰਸਤਾ, ਵਸਤੂ->string);

		/*
		 * we might have removed this as part of earlier
		 * recursive directory removal, so lstat() here could
		 * fail with ENOENT.
		 */
		ਜੇ (lstat(abs_ਰਸਤਾ.buf, &st))
			ਜਾਰੀ;

		ਜੇ (S_ISDIR(st.st_ਬਹੁਲਕ)) {
			ਜੇ (remove_dirs(&abs_ਰਸਤਾ, ਅਗੇਤਰ, rm_flags, dry_ਲੜੀਵਾਰ, quiet, &gone))
				errors++;
			ਜੇ (gone && !quiet) {
				qname = quote_ਰਸਤਾ(ਵਸਤੂ->string, ਨਿਰਾ, &buf, 0);
				ਲਿਖੋ(dry_ਲੜੀਵਾਰ ? _(ਸੁਨੇਹਾ_would_remove) : _(ਸੁਨੇਹਾ_remove), qname);
			}
		} ਨਹੀਂਤਾਂ {
			res = dry_ਲੜੀਵਾਰ ? 0 : unlink(abs_ਰਸਤਾ.buf);
			ਜੇ (res) {
				ਪੂਰਨ saved_errno = errno;
				qname = quote_ਰਸਤਾ(ਵਸਤੂ->string, ਨਿਰਾ, &buf, 0);
				errno = saved_errno;
				warning_errno(_(ਸੁਨੇਹਾ_warn_remove_failed), qname);
				errors++;
			} ਨਹੀਂਤਾਂ ਜੇ (!quiet) {
				qname = quote_ਰਸਤਾ(ਵਸਤੂ->string, ਨਿਰਾ, &buf, 0);
				ਲਿਖੋ(dry_ਲੜੀਵਾਰ ? _(ਸੁਨੇਹਾ_would_remove) : _(ਸੁਨੇਹਾ_remove), qname);
			}
		}
	}

	strbuf_release(&abs_ਰਸਤਾ);
	strbuf_release(&buf);
	string_list_clear(&del_list, 0);
	string_list_clear(&exclude_list, 0);
	clear_pathspec(&pathspec);
	ਵਾਪਸ (errors != 0);
}
