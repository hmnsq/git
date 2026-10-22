/*
 * Builtin help command
 */

#define USE_THE_REPOSITORY_VARIABLE

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "exec-cmd.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "pager.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "config-list.h"
#અન્તર્ભાવ "help.h"
#અન્તર્ભાવ "alias.h"
#અન્તર્ભાવ "setup.h"

#ifndef DEFAULT_HELP_FORMAT
#define DEFAULT_HELP_FORMAT "man"
#endif

સ્થિર સંરચના man_viewer_list {
	સંરચના man_viewer_list *next;
	અક્ષર name[FLEX_ARRAY];
} *man_viewer_list;

સ્થિર સંરચના man_viewer_info_list {
	સંરચના man_viewer_info_list *next;
	અચલ અક્ષર *info;
	અક્ષર name[FLEX_ARRAY];
} *man_viewer_info_list;

ગણના help_format {
	HELP_FORMAT_NONE,
	HELP_FORMAT_MAN,
	HELP_FORMAT_INFO,
	HELP_FORMAT_WEB
};

ગણના show_config_type {
	SHOW_CONFIG_HUMAN,
	SHOW_CONFIG_VARS,
	SHOW_CONFIG_SECTIONS,
};

સ્થિર ગણના help_action {
	HELP_ACTION_ALL = 1,
	HELP_ACTION_GUIDES,
	HELP_ACTION_CONFIG,
	HELP_ACTION_USER_INTERFACES,
	HELP_ACTION_DEVELOPER_INTERFACES,
	HELP_ACTION_CONFIG_FOR_COMPLETION,
	HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION,
	HELP_ACTION_ALIASES_FOR_COMPLETION,
} cmd_mode;

સ્થિર અક્ષર *html_path;
સ્થિર પૂર્ણાંક verbose = 1;
સ્થિર ગણના help_format help_format = HELP_FORMAT_NONE;
સ્થિર પૂર્ણાંક exclude_guides;
સ્થિર પૂર્ણાંક show_external_commands = -1;
સ્થિર પૂર્ણાંક show_aliases = -1;
સ્થિર સંરચના option builtin_help_options[] = {
	OPT_CMDMODE('a', "all", &cmd_mode, N_("print all available commands"),
		    HELP_ACTION_ALL),
	OPT_BOOL(0, "external-commands", &show_external_commands,
		 N_("show external commands in --all")),
	OPT_BOOL(0, "aliases", &show_aliases, N_("show aliases in --all")),
	OPT_HIDDEN_BOOL(0, "exclude-guides", &exclude_guides, N_("exclude guides")),
	OPT_SET_INT('m', "man", &help_format, N_("show man page"), HELP_FORMAT_MAN),
	OPT_SET_INT('w', "web", &help_format, N_("show manual in web browser"),
			HELP_FORMAT_WEB),
	OPT_SET_INT('i', "info", &help_format, N_("show info page"),
			HELP_FORMAT_INFO),
	OPT__VERBOSE(&verbose, N_("print command description")),

	OPT_CMDMODE('g', "guides", &cmd_mode, N_("print list of useful guides"),
		    HELP_ACTION_GUIDES),
	OPT_CMDMODE(0, "user-interfaces", &cmd_mode,
		    N_("print list of user-facing repository, command and file interfaces"),
		    HELP_ACTION_USER_INTERFACES),
	OPT_CMDMODE(0, "developer-interfaces", &cmd_mode,
		    N_("print list of file formats, protocols and other developer interfaces"),
		    HELP_ACTION_DEVELOPER_INTERFACES),
	OPT_CMDMODE('c', "config", &cmd_mode, N_("print all configuration variable names"),
		    HELP_ACTION_CONFIG),
	OPT_CMDMODE_F(0, "config-for-completion", &cmd_mode, "",
		    HELP_ACTION_CONFIG_FOR_COMPLETION, PARSE_OPT_HIDDEN),
	OPT_CMDMODE_F(0, "config-sections-for-completion", &cmd_mode, "",
		    HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION, PARSE_OPT_HIDDEN),
	OPT_CMDMODE_F(0, "aliases-for-completion", &cmd_mode, "",
		    HELP_ACTION_ALIASES_FOR_COMPLETION, PARSE_OPT_HIDDEN),

	OPT_END(),
};

સ્થિર અચલ અક્ષર * અચલ builtin_help_usage[] = {
	"git help [-a|--all] [--[no-]verbose] [--[no-]external-commands] [--[no-]aliases]",
	N_("git help [[-i|--info] [-m|--man] [-w|--web]] [<command>|<doc>]"),
	"git help [-g|--guides]",
	"git help [-c|--config]",
	"git help [--user-interfaces]",
	"git help [--developer-interfaces]",
	ખાલી
};

સંરચના slot_expansion {
	અચલ અક્ષર *prefix;
	અચલ અક્ષર *placeholder;
	શૂન્ય (*fn)(સંરચના string_list *list, અચલ અક્ષર *prefix);
	પૂર્ણાંક found;
};

સ્થિર શૂન્ય set_config_vars(સંરચના string_list *ચાવીઓ_uniq, સંરચના string_list_item *var)
{
	સંરચના strbuf sb = STRBUF_INIT;
	અચલ અક્ષર *str = var->string;
	અચલ અક્ષર *wildcard = strchr(str, '*');
	અચલ અક્ષર *tag = strchr(str, '<');
	અચલ અક્ષર *cut;

	જો (wildcard && tag)
		cut = wildcard < tag ? wildcard : tag;
	નહીંતર જો (wildcard)
		cut = wildcard;
	નહીંતર જો (tag)
		cut = tag;
	નહીંતર {
		string_list_append(ચાવીઓ_uniq, str);
		પાછા;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(ચાવીઓ_uniq, sb.buf);
	strbuf_release(&sb);
}

સ્થિર શૂન્ય set_config_sections(સંરચના string_list *ચાવીઓ_uniq, સંરચના string_list_item *var)
{
	સંરચના strbuf sb = STRBUF_INIT;
	અચલ અક્ષર *str = var->string;
	અચલ અક્ષર *dot = strchr(str, '.');
	અચલ અક્ષર *cut;

	જો (dot)
		cut = dot;
	નહીંતર {
		set_config_vars(ચાવીઓ_uniq, var);
		પાછા;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(ચાવીઓ_uniq, sb.buf);
	strbuf_release(&sb);
}

સ્થિર શૂન્ય list_config_help(ગણના show_config_type type)
{
	સંરચના slot_expansion slot_expansions[] = {
		{ "advice", "*", list_config_advices },
		{ "color.branch", "<slot>", list_config_color_branch_slots },
		{ "color.decorate", "<slot>", list_config_color_decorate_slots },
		{ "color.diff", "<slot>", list_config_color_diff_slots },
		{ "color.grep", "<slot>", list_config_color_grep_slots },
		{ "color.interactive", "<slot>", list_config_color_interactive_slots },
		{ "color.remote", "<slot>", list_config_color_sideband_slots },
		{ "color.status", "<slot>", list_config_color_status_slots },
		{ "fsck", "<msg-id>", list_config_fsck_સંદેશ_ids },
		{ "receive.fsck", "<msg-id>", list_config_fsck_સંદેશ_ids },
		{ ખાલી, ખાલી, ખાલી }
	};
	અચલ અક્ષર **p;
	સંરચના slot_expansion *e;
	સંરચના string_list ચાવીઓ = STRING_LIST_INIT_DUP;
	સંરચના string_list ચાવીઓ_uniq = STRING_LIST_INIT_DUP;
	સંરચના string_list_item *item;
	સંરચના strbuf sb = STRBUF_INIT;

	માટે (p = config_name_list; *p; p++) {
		અચલ અક્ષર *var = *p;

		માટે (e = slot_expansions; e->prefix; e++) {
			strbuf_reset(&sb);
			strbuf_addf(&sb, "%s.%s", e->prefix, e->placeholder);
			જો (!strcasecmp(var, sb.buf)) {
				e->fn(&ચાવીઓ, e->prefix);
				e->found++;
				તોડો;
			}
		}

		જો (!e->prefix)
			string_list_append(&ચાવીઓ, var);
	}

	strbuf_release(&sb);

	માટે (e = slot_expansions; e->prefix; e++)
		જો (!e->found)
			BUG("slot_expansion %s.%s is not used",
			    e->prefix, e->placeholder);

	માટે (size_t i = 0; i < ચાવીઓ.nr; i++) {
		પસંદગી (type) {
		સ્થિતિ SHOW_CONFIG_HUMAN:
			string_list_append(&ચાવીઓ_uniq, ચાવીઓ.વસ્તુઓ[i].string);
			તોડો;
		સ્થિતિ SHOW_CONFIG_SECTIONS:
			set_config_sections(&ચાવીઓ_uniq, &ચાવીઓ.વસ્તુઓ[i]);
			તોડો;
		સ્થિતિ SHOW_CONFIG_VARS:
			set_config_vars(&ચાવીઓ_uniq, &ચાવીઓ.વસ્તુઓ[i]);
			તોડો;
		મૂળભૂત:
			BUG("%d: unexpected type", type);
		}
	}

	string_list_sort_u(&ચાવીઓ_uniq, 0);
	for_each_string_list_item(item, &ચાવીઓ_uniq)
		puts(item->string);
	string_list_clear(&ચાવીઓ_uniq, 0);
	string_list_clear(&ચાવીઓ, 0);
}

સ્થિર ગણના help_format parse_help_format(અચલ અક્ષર *format)
{
	જો (!strcmp(format, "man"))
		પાછા HELP_FORMAT_MAN;
	જો (!strcmp(format, "info"))
		પાછા HELP_FORMAT_INFO;
	જો (!strcmp(format, "web") || !strcmp(format, "html"))
		પાછા HELP_FORMAT_WEB;
	/*
	 * Please update _repo_config() in git-completion.bash when you
	 * add new help formats.
	 */
	die(_("unrecognized help format '%s'"), format);
}

સ્થિર અચલ અક્ષર *get_man_viewer_info(અચલ અક્ષર *name)
{
	સંરચના man_viewer_info_list *viewer;

	માટે (viewer = man_viewer_info_list; viewer; viewer = viewer->next)
	{
		જો (!strcasecmp(name, viewer->name))
			પાછા viewer->info;
	}
	પાછા ખાલી;
}

સ્થિર પૂર્ણાંક check_emacsclient_version(શૂન્ય)
{
	સંરચના strbuf buffer = STRBUF_INIT;
	સંરચના child_process ec_process = CHILD_PROCESS_INIT;
	પૂર્ણાંક version;

	/* emacsclient prints its version number on stderr */
	strvec_pushl(&ec_process.args, "emacsclient", "--version", ખાલી);
	ec_process.err = -1;
	ec_process.stdout_to_stderr = 1;
	જો (start_command(&ec_process))
		પાછા error(_("Failed to start emacsclient."));

	strbuf_read(&buffer, ec_process.err, 20);
	close(ec_process.err);

	/*
	 * Don't bother checking return value, because "emacsclient --version"
	 * seems to always exits with code 1.
	 */
	finish_command(&ec_process);

	જો (!starts_with(buffer.buf, "emacsclient")) {
		strbuf_release(&buffer);
		પાછા error(_("Failed to parse emacsclient version."));
	}

	strbuf_remove(&buffer, 0, strlen("emacsclient"));
	version = atoi(buffer.buf);

	જો (version < 22) {
		strbuf_release(&buffer);
		પાછા error(_("emacsclient version '%d' too old (< 22)."),
			version);
	}

	strbuf_release(&buffer);
	પાછા 0;
}

સ્થિર શૂન્ય exec_woman_emacs(અચલ અક્ષર *path, અચલ અક્ષર *page)
{
	જો (!check_emacsclient_version()) {
		/* This works only with emacsclient version >= 22. */
		સંરચના strbuf man_page = STRBUF_INIT;

		જો (!path)
			path = "emacsclient";
		strbuf_addf(&man_page, "(woman \"%s\")", page);
		execlp(path, "emacsclient", "-e", man_page.buf, (અક્ષર *)ખાલી);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

સ્થિર શૂન્ય exec_man_konqueror(અચલ અક્ષર *path, અચલ અક્ષર *page)
{
	અચલ અક્ષર *display = getenv("DISPLAY");
	જો (display && *display) {
		સંરચના strbuf man_page = STRBUF_INIT;
		અચલ અક્ષર *filename = "kfmclient";

		/* It's simpler to launch konqueror using kfmclient. */
		જો (path) {
			size_t len;
			જો (strip_suffix(path, "/konqueror", &len))
				path = xstrfmt("%.*s/kfmclient", (પૂર્ણાંક)len, path);
			filename = basename((અક્ષર *)path);
		} નહીંતર
			path = "kfmclient";
		strbuf_addf(&man_page, "man:%s(1)", page);
		execlp(path, filename, "newTab", man_page.buf, (અક્ષર *)ખાલી);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

સ્થિર શૂન્ય exec_man_man(અચલ અક્ષર *path, અચલ અક્ષર *page)
{
	જો (!path)
		path = "man";
	execlp(path, "man", page, (અક્ષર *)ખાલી);
	warning_errno(_("failed to exec '%s'"), path);
}

સ્થિર શૂન્ય exec_man_cmd(અચલ અક્ષર *cmd, અચલ અક્ષર *page)
{
	સંરચના strbuf shell_cmd = STRBUF_INIT;
	strbuf_addf(&shell_cmd, "%s %s", cmd, page);
	execl(SHELL_PATH, SHELL_PATH, "-c", shell_cmd.buf, (અક્ષર *)ખાલી);
	warning(_("failed to exec '%s'"), cmd);
	strbuf_release(&shell_cmd);
}

સ્થિર શૂન્ય add_man_viewer(અચલ અક્ષર *name)
{
	સંરચના man_viewer_list **p = &man_viewer_list;

	જ્યારે (*p)
		p = &((*p)->next);
	FLEX_ALLOC_STR(*p, name, name);
}

સ્થિર પૂર્ણાંક supported_man_viewer(અચલ અક્ષર *name, size_t len)
{
	પાછા (!strncasecmp("man", name, len) ||
		!strncasecmp("woman", name, len) ||
		!strncasecmp("konqueror", name, len));
}

સ્થિર શૂન્ય do_add_man_viewer_info(અચલ અક્ષર *name,
				   size_t len,
				   અચલ અક્ષર *કિંમત)
{
	સંરચના man_viewer_info_list *new_man_viewer;
	FLEX_ALLOC_MEM(new_man_viewer, name, name, len);
	new_man_viewer->info = xstrdup(કિંમત);
	new_man_viewer->next = man_viewer_info_list;
	man_viewer_info_list = new_man_viewer;
}

સ્થિર પૂર્ણાંક add_man_viewer_path(અચલ અક્ષર *name,
			       size_t len,
			       અચલ અક્ષર *કિંમત)
{
	જો (supported_man_viewer(name, len))
		do_add_man_viewer_info(name, len, કિંમત);
	નહીંતર
		warning(_("'%s': path for unsupported man viewer.\n"
			  "Please consider using 'man.<tool>.cmd' instead."),
			name);

	પાછા 0;
}

સ્થિર પૂર્ણાંક add_man_viewer_cmd(અચલ અક્ષર *name,
			      size_t len,
			      અચલ અક્ષર *કિંમત)
{
	જો (supported_man_viewer(name, len))
		warning(_("'%s': cmd for supported man viewer.\n"
			  "Please consider using 'man.<tool>.path' instead."),
			name);
	નહીંતર
		do_add_man_viewer_info(name, len, કિંમત);

	પાછા 0;
}

સ્થિર પૂર્ણાંક add_man_viewer_info(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત)
{
	અચલ અક્ષર *name, *subkey;
	size_t namelen;

	જો (parse_config_ચાવી(var, "man", &name, &namelen, &subkey) < 0 || !name)
		પાછા 0;

	જો (!strcmp(subkey, "path")) {
		જો (!કિંમત)
			પાછા config_error_nonbool(var);
		પાછા add_man_viewer_path(name, namelen, કિંમત);
	}
	જો (!strcmp(subkey, "cmd")) {
		જો (!કિંમત)
			પાછા config_error_nonbool(var);
		પાછા add_man_viewer_cmd(name, namelen, કિંમત);
	}

	પાછા 0;
}

સ્થિર પૂર્ણાંક git_help_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
			   અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	જો (!strcmp(var, "help.format")) {
		જો (!કિંમત)
			પાછા config_error_nonbool(var);
		help_format = parse_help_format(કિંમત);
		પાછા 0;
	}
	જો (!strcmp(var, "help.htmlpath")) {
		જો (!કિંમત)
			પાછા config_error_nonbool(var);
		મુક્ત(html_path);
		html_path = xstrdup(કિંમત);
		પાછા 0;
	}
	જો (!strcmp(var, "man.viewer")) {
		જો (!કિંમત)
			પાછા config_error_nonbool(var);
		add_man_viewer(કિંમત);
		પાછા 0;
	}
	જો (starts_with(var, "man."))
		પાછા add_man_viewer_info(var, કિંમત);

	પાછા git_default_config(var, કિંમત, ctx, cb);
}

સ્થિર સંરચના cmdnames main_cmds, other_cmds;

સ્થિર પૂર્ણાંક is_git_command(અચલ અક્ષર *s)
{
	જો (is_builtin(s))
		પાછા 1;

	load_command_list("git-", &main_cmds, &other_cmds);
	પાછા is_in_cmdlist(&main_cmds, s) ||
		is_in_cmdlist(&other_cmds, s);
}

સ્થિર અચલ અક્ષર *cmd_to_page(અચલ અક્ષર *git_cmd)
{
	જો (!git_cmd)
		પાછા "git";
	નહીંતર જો (starts_with(git_cmd, "git"))
		પાછા git_cmd;
	નહીંતર જો (is_git_command(git_cmd))
		પાછા xstrfmt("git-%s", git_cmd);
	નહીંતર જો (!strcmp("scalar", git_cmd))
		પાછા xstrdup(git_cmd);
	નહીંતર
		પાછા xstrfmt("git%s", git_cmd);
}

સ્થિર શૂન્ય setup_man_path(શૂન્ય)
{
	સંરચના strbuf new_path = STRBUF_INIT;
	અચલ અક્ષર *old_path = getenv("MANPATH");
	અક્ષર *git_man_path = system_path(GIT_MAN_PATH);

	/* We should always put ':' after our path. If there is no
	 * old_path, the ':' at the end will let 'man' to try
	 * system-wide paths after ours to find the manual page. If
	 * there is old_path, we need ':' as delimiter. */
	strbuf_addstr(&new_path, git_man_path);
	strbuf_addch(&new_path, ':');
	જો (old_path)
		strbuf_addstr(&new_path, old_path);

	મુક્ત(git_man_path);
	setenv("MANPATH", new_path.buf, 1);

	strbuf_release(&new_path);
}

સ્થિર શૂન્ય exec_viewer(અચલ અક્ષર *name, અચલ અક્ષર *page)
{
	અચલ અક્ષર *info = get_man_viewer_info(name);

	જો (!strcasecmp(name, "man"))
		exec_man_man(info, page);
	નહીંતર જો (!strcasecmp(name, "woman"))
		exec_woman_emacs(info, page);
	નહીંતર જો (!strcasecmp(name, "konqueror"))
		exec_man_konqueror(info, page);
	નહીંતર જો (info)
		exec_man_cmd(info, page);
	નહીંતર
		warning(_("'%s': unknown man viewer."), name);
}

સ્થિર શૂન્ય show_man_page(અચલ અક્ષર *page)
{
	સંરચના man_viewer_list *viewer;
	અચલ અક્ષર *fallback = getenv("GIT_MAN_VIEWER");

	setup_man_path();
	માટે (viewer = man_viewer_list; viewer; viewer = viewer->next)
	{
		exec_viewer(viewer->name, page); /* will return when unable */
	}
	જો (fallback)
		exec_viewer(fallback, page);
	exec_viewer("man", page);
	die(_("no man viewer handled the request"));
}

સ્થિર શૂન્ય show_info_page(અચલ અક્ષર *page)
{
	setenv("INFOPATH", system_path(GIT_INFO_PATH), 1);
	execlp("info", "info", "gitman", page, (અક્ષર *)ખાલી);
	die(_("no info viewer handled the request"));
}

સ્થિર શૂન્ય get_html_page_path(સંરચના strbuf *page_path, અચલ અક્ષર *page)
{
	સંરચના stat st;
	અચલ અક્ષર *path = html_path;
	અક્ષર *to_free = ખાલી;

	જો (!path)
		path = to_free = system_path(GIT_HTML_PATH);

	/*
	 * Check that the page we're looking for exists.
	 */
	જો (!strstr(path, "://")) {
		જો (stat(mkpath("%s/%s.html", path, page), &st)
		    || !S_ISREG(st.st_mode))
			die("'%s/%s.html': documentation file not found.",
				path, page);
	}

	strbuf_init(page_path, 0);
	strbuf_addf(page_path, "%s/%s.html", path, page);
	મુક્ત(to_free);
}

સ્થિર શૂન્ય open_html(અચલ અક્ષર *path)
{
	execl_git_cmd("web--browse", "-c", "help.browser", path, (અક્ષર *)ખાલી);
}

સ્થિર શૂન્ય show_html_page(અચલ અક્ષર *page)
{
	સંરચના strbuf page_path; /* it leaks but we exec below */

	get_html_page_path(&page_path, page);

	open_html(page_path.buf);
}

સ્થિર અક્ષર *check_git_cmd(અચલ અક્ષર *cmd)
{
	અક્ષર *alias;

	જો (is_git_command(cmd))
		પાછા xstrdup(cmd);

	alias = alias_lookup(cmd);
	જો (alias) {
		અચલ અક્ષર **argv;
		પૂર્ણાંક ગણતરી;

		/*
		 * handle_builtin() in git.c rewrites "git cmd --help"
		 * to "git help --exclude-guides cmd", so we can use
		 * exclude_guides to distinguish "git cmd --help" from
		 * "git help cmd". In the latter case, or if cmd is an
		 * alias for a shell command, just print the alias
		 * definition.
		 */
		જો (!exclude_guides || alias[0] == '!') {
			printf_ln(_("'%s' is aliased to '%s'"), cmd, alias);
			મુક્ત(alias);
			exit(0);
		}
		/*
		 * Otherwise, we pretend that the command was "git
		 * word0 --help". We use split_cmdline() to get the
		 * first word of the alias, to ensure that we use the
		 * same rules as when the alias is actually
		 * used. split_cmdline() modifies alias in-place.
		 */
		fprintf_ln(stderr, _("'%s' is aliased to '%s'"), cmd, alias);
		ગણતરી = split_cmdline(alias, &argv);
		જો (ગણતરી < 0)
			die(_("bad alias.%s string: %s"), cmd,
			    split_cmdline_strerror(ગણતરી));
		મુક્ત(argv);
		પાછા alias;
	}

	જો (exclude_guides)
		પાછા help_unknown_cmd(cmd);

	પાછા xstrdup(cmd);
}

સ્થિર શૂન્ય no_help_format(અચલ અક્ષર *opt_mode, ગણના help_format fmt)
{
	અચલ અક્ષર *opt_fmt;

	પસંદગી (fmt) {
	સ્થિતિ HELP_FORMAT_NONE:
		પાછા;
	સ્થિતિ HELP_FORMAT_MAN:
		opt_fmt = "--man";
		તોડો;
	સ્થિતિ HELP_FORMAT_INFO:
		opt_fmt = "--info";
		તોડો;
	સ્થિતિ HELP_FORMAT_WEB:
		opt_fmt = "--web";
		તોડો;
	મૂળભૂત:
		BUG("unreachable");
	}

	usage_સંદેશ_optf(_("options '%s' and '%s' cannot be used together"),
		       builtin_help_usage, builtin_help_options, opt_mode,
		       opt_fmt);
}

સ્થિર શૂન્ય opt_mode_usage(પૂર્ણાંક argc, અચલ અક્ષર *opt_mode,
			   ગણના help_format fmt)
{
	જો (argc)
		usage_સંદેશ_optf(_("the '%s' option doesn't take any non-option arguments"),
			       builtin_help_usage, builtin_help_options,
			       opt_mode);

	no_help_format(opt_mode, fmt);
}

પૂર્ણાંક cmd_help(પૂર્ણાંક argc,
	     અચલ અક્ષર **argv,
	     અચલ અક્ષર *prefix,
	     સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક nongit;
	ગણના help_format parsed_help_format;
	અક્ષર *command = ખાલી;
	અચલ અક્ષર *page;

	argc = parse_options(argc, argv, prefix, builtin_help_options,
			builtin_help_usage, 0);
	parsed_help_format = help_format;

	જો (cmd_mode != HELP_ACTION_ALL &&
	    (show_external_commands >= 0 ||
	     show_aliases >= 0))
		usage_સંદેશ_opt(_("the '--no-[external-commands|aliases]' options can only be used with '--all'"),
			      builtin_help_usage, builtin_help_options);

	પસંદગી (cmd_mode) {
	સ્થિતિ HELP_ACTION_ALL:
		opt_mode_usage(argc, "--all", help_format);
		જો (verbose) {
			setup_pager(the_repository);
			list_all_cmds_help(show_external_commands,
					   show_aliases);
			પાછા 0;
		}
		printf(_("usage: %s%s"), _(git_usage_string), "\n\n");
		load_command_list("git-", &main_cmds, &other_cmds);
		list_commands(&main_cmds, &other_cmds);
		printf("%s\n", _(git_more_info_string));
		તોડો;
	સ્થિતિ HELP_ACTION_GUIDES:
		opt_mode_usage(argc, "--guides", help_format);
		list_guides_help();
		printf("%s\n", _(git_more_info_string));
		પાછા 0;
	સ્થિતિ HELP_ACTION_CONFIG_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-for-completion", help_format);
		list_config_help(SHOW_CONFIG_VARS);
		પાછા 0;
	સ્થિતિ HELP_ACTION_USER_INTERFACES:
		opt_mode_usage(argc, "--user-interfaces", help_format);
		list_user_interfaces_help();
		પાછા 0;
	સ્થિતિ HELP_ACTION_DEVELOPER_INTERFACES:
		opt_mode_usage(argc, "--developer-interfaces", help_format);
		list_developer_interfaces_help();
		પાછા 0;
	સ્થિતિ HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-sections-for-completion",
			       help_format);
		list_config_help(SHOW_CONFIG_SECTIONS);
		પાછા 0;
	સ્થિતિ HELP_ACTION_ALIASES_FOR_COMPLETION: {
		સંરચના string_list alias_list = STRING_LIST_INIT_DUP;
		opt_mode_usage(argc, "--aliases-for-completion", help_format);
		list_aliases(&alias_list);
		માટે (size_t i = 0; i < alias_list.nr; i++)
			printf("%s%c%s%c", alias_list.વસ્તુઓ[i].string, '\n',
			       (અક્ષર *)alias_list.વસ્તુઓ[i].util, '\0');
		string_list_clear(&alias_list, 1);
		પાછા 0;
	}
	સ્થિતિ HELP_ACTION_CONFIG:
		opt_mode_usage(argc, "--config", help_format);
		setup_pager(the_repository);
		list_config_help(SHOW_CONFIG_HUMAN);
		printf("\n%s\n", _("'git help config' for more information"));
		પાછા 0;
	}

	જો (!argv[0]) {
		printf(_("usage: %s%s"), _(git_usage_string), "\n\n");
		list_common_cmds_help();
		printf("\n%s\n", _(git_more_info_string));
		પાછા 0;
	}

	setup_git_directory_gently(the_repository, &nongit);
	repo_config(the_repository, git_help_config, ખાલી);

	જો (parsed_help_format != HELP_FORMAT_NONE)
		help_format = parsed_help_format;
	જો (help_format == HELP_FORMAT_NONE)
		help_format = parse_help_format(DEFAULT_HELP_FORMAT);

	command = check_git_cmd(argv[0]);

	page = cmd_to_page(command);
	પસંદગી (help_format) {
	સ્થિતિ HELP_FORMAT_NONE:
	સ્થિતિ HELP_FORMAT_MAN:
		show_man_page(page);
		તોડો;
	સ્થિતિ HELP_FORMAT_INFO:
		show_info_page(page);
		તોડો;
	સ્થિતિ HELP_FORMAT_WEB:
		show_html_page(page);
		તોડો;
	}

	મુક્ત(command);
	પાછા 0;
}
