/*
 * Builtin help command
 */

#define USE_THE_REPOSITORY_VARIABLE

#include "builtin.h"
#include "config.h"
#include "environment.h"
#include "exec-cmd.h"
#include "gettext.h"
#include "pager.h"
#include "parse-options.h"
#include "path.h"
#include "run-command.h"
#include "config-list.h"
#include "help.h"
#include "alias.h"
#include "setup.h"

#ifndef DEFAULT_HELP_FORMAT
#define DEFAULT_HELP_FORMAT "man"
#endif

static sa_nracanaa man_viewer_list {
	sa_nracanaa man_viewer_list *next;
	var_na name[FLEX_ARRAY];
} *man_viewer_list;

static sa_nracanaa man_viewer_info_list {
	sa_nracanaa man_viewer_info_list *next;
	const var_na *info;
	var_na name[FLEX_ARRAY];
} *man_viewer_info_list;

ga_nanaa help_format {
	HELP_FORMAT_NONE,
	HELP_FORMAT_MAN,
	HELP_FORMAT_INFO,
	HELP_FORMAT_WEB
};

ga_nanaa show_config_type {
	SHOW_CONFIG_HUMAN,
	SHOW_CONFIG_VARS,
	SHOW_CONFIG_SECTIONS,
};

static ga_nanaa help_action {
	HELP_ACTION_ALL = 1,
	HELP_ACTION_GUIDES,
	HELP_ACTION_CONFIG,
	HELP_ACTION_USER_INTERFACES,
	HELP_ACTION_DEVELOPER_INTERFACES,
	HELP_ACTION_CONFIG_FOR_COMPLETION,
	HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION,
	HELP_ACTION_ALIASES_FOR_COMPLETION,
} cmd_mode;

static var_na *html_path;
static จำนวนเต็ม verbose = 1;
static ga_nanaa help_format help_format = HELP_FORMAT_NONE;
static จำนวนเต็ม exclude_guides;
static จำนวนเต็ม show_external_commands = -1;
static จำนวนเต็ม show_aliases = -1;
static sa_nracanaa option builtin_help_options[] = {
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

static const var_na * const builtin_help_usage[] = {
	"git help [-a|--all] [--[no-]verbose] [--[no-]external-commands] [--[no-]aliases]",
	N_("git help [[-i|--info] [-m|--man] [-w|--web]] [<command>|<doc>]"),
	"git help [-g|--guides]",
	"git help [-c|--config]",
	"git help [--user-interfaces]",
	"git help [--developer-interfaces]",
	NULL
};

sa_nracanaa slot_expansion {
	const var_na *prefix;
	const var_na *placeholder;
	void (*fn)(sa_nracanaa string_list *list, const var_na *prefix);
	จำนวนเต็ม found;
};

static void set_config_vars(sa_nracanaa string_list *keys_uniq, sa_nracanaa string_list_item *var)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	const var_na *str = var->string;
	const var_na *wildcard = strchr(str, '*');
	const var_na *tag = strchr(str, '<');
	const var_na *cut;

	ถ้า (wildcard && tag)
		cut = wildcard < tag ? wildcard : tag;
	มิฉะนั้น ถ้า (wildcard)
		cut = wildcard;
	มิฉะนั้น ถ้า (tag)
		cut = tag;
	มิฉะนั้น {
		string_list_append(keys_uniq, str);
		คืน;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(keys_uniq, sb.buf);
	strbuf_release(&sb);
}

static void set_config_sections(sa_nracanaa string_list *keys_uniq, sa_nracanaa string_list_item *var)
{
	sa_nracanaa strbuf sb = STRBUF_INIT;
	const var_na *str = var->string;
	const var_na *dot = strchr(str, '.');
	const var_na *cut;

	ถ้า (dot)
		cut = dot;
	มิฉะนั้น {
		set_config_vars(keys_uniq, var);
		คืน;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(keys_uniq, sb.buf);
	strbuf_release(&sb);
}

static void list_config_help(ga_nanaa show_config_type type)
{
	sa_nracanaa slot_expansion slot_expansions[] = {
		{ "advice", "*", list_config_advices },
		{ "color.branch", "<slot>", list_config_color_branch_slots },
		{ "color.decorate", "<slot>", list_config_color_decorate_slots },
		{ "color.diff", "<slot>", list_config_color_diff_slots },
		{ "color.grep", "<slot>", list_config_color_grep_slots },
		{ "color.interactive", "<slot>", list_config_color_interactive_slots },
		{ "color.remote", "<slot>", list_config_color_sideband_slots },
		{ "color.status", "<slot>", list_config_color_status_slots },
		{ "fsck", "<msg-id>", list_config_fsck_msg_ids },
		{ "receive.fsck", "<msg-id>", list_config_fsck_msg_ids },
		{ NULL, NULL, NULL }
	};
	const var_na **p;
	sa_nracanaa slot_expansion *e;
	sa_nracanaa string_list keys = STRING_LIST_INIT_DUP;
	sa_nracanaa string_list keys_uniq = STRING_LIST_INIT_DUP;
	sa_nracanaa string_list_item *item;
	sa_nracanaa strbuf sb = STRBUF_INIT;

	สำหรับ (p = config_name_list; *p; p++) {
		const var_na *var = *p;

		สำหรับ (e = slot_expansions; e->prefix; e++) {
			strbuf_reset(&sb);
			strbuf_addf(&sb, "%s.%s", e->prefix, e->placeholder);
			ถ้า (!strcasecmp(var, sb.buf)) {
				e->fn(&keys, e->prefix);
				e->found++;
				bha_nga;
			}
		}

		ถ้า (!e->prefix)
			string_list_append(&keys, var);
	}

	strbuf_release(&sb);

	สำหรับ (e = slot_expansions; e->prefix; e++)
		ถ้า (!e->found)
			BUG("slot_expansion %s.%s is not used",
			    e->prefix, e->placeholder);

	สำหรับ (size_t i = 0; i < keys.nr; i++) {
		switch (type) {
		pak_sa SHOW_CONFIG_HUMAN:
			string_list_append(&keys_uniq, keys.items[i].string);
			bha_nga;
		pak_sa SHOW_CONFIG_SECTIONS:
			set_config_sections(&keys_uniq, &keys.items[i]);
			bha_nga;
		pak_sa SHOW_CONFIG_VARS:
			set_config_vars(&keys_uniq, &keys.items[i]);
			bha_nga;
		default:
			BUG("%d: unexpected type", type);
		}
	}

	string_list_sort_u(&keys_uniq, 0);
	for_each_string_list_item(item, &keys_uniq)
		puts(item->string);
	string_list_clear(&keys_uniq, 0);
	string_list_clear(&keys, 0);
}

static ga_nanaa help_format parse_help_format(const var_na *format)
{
	ถ้า (!strcmp(format, "man"))
		คืน HELP_FORMAT_MAN;
	ถ้า (!strcmp(format, "info"))
		คืน HELP_FORMAT_INFO;
	ถ้า (!strcmp(format, "web") || !strcmp(format, "html"))
		คืน HELP_FORMAT_WEB;
	/*
	 * Please update _repo_config() in git-completion.bash when you
	 * add new help formats.
	 */
	die(_("unrecognized help format '%s'"), format);
}

static const var_na *get_man_viewer_info(const var_na *name)
{
	sa_nracanaa man_viewer_info_list *viewer;

	สำหรับ (viewer = man_viewer_info_list; viewer; viewer = viewer->next)
	{
		ถ้า (!strcasecmp(name, viewer->name))
			คืน viewer->info;
	}
	คืน NULL;
}

static จำนวนเต็ม check_emacsclient_version(void)
{
	sa_nracanaa strbuf buffer = STRBUF_INIT;
	sa_nracanaa child_process ec_process = CHILD_PROCESS_INIT;
	จำนวนเต็ม version;

	/* emacsclient prints its version number on stderr */
	strvec_pushl(&ec_process.args, "emacsclient", "--version", NULL);
	ec_process.err = -1;
	ec_process.stdout_to_stderr = 1;
	ถ้า (start_command(&ec_process))
		คืน error(_("Failed to start emacsclient."));

	strbuf_read(&buffer, ec_process.err, 20);
	close(ec_process.err);

	/*
	 * Don't bother checking return value, because "emacsclient --version"
	 * seems to always exits with code 1.
	 */
	finish_command(&ec_process);

	ถ้า (!starts_with(buffer.buf, "emacsclient")) {
		strbuf_release(&buffer);
		คืน error(_("Failed to parse emacsclient version."));
	}

	strbuf_remove(&buffer, 0, strlen("emacsclient"));
	version = atoi(buffer.buf);

	ถ้า (version < 22) {
		strbuf_release(&buffer);
		คืน error(_("emacsclient version '%d' too old (< 22)."),
			version);
	}

	strbuf_release(&buffer);
	คืน 0;
}

static void exec_woman_emacs(const var_na *path, const var_na *page)
{
	ถ้า (!check_emacsclient_version()) {
		/* This works only with emacsclient version >= 22. */
		sa_nracanaa strbuf man_page = STRBUF_INIT;

		ถ้า (!path)
			path = "emacsclient";
		strbuf_addf(&man_page, "(woman \"%s\")", page);
		execlp(path, "emacsclient", "-e", man_page.buf, (var_na *)NULL);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

static void exec_man_konqueror(const var_na *path, const var_na *page)
{
	const var_na *display = getenv("DISPLAY");
	ถ้า (display && *display) {
		sa_nracanaa strbuf man_page = STRBUF_INIT;
		const var_na *filename = "kfmclient";

		/* It's simpler to launch konqueror using kfmclient. */
		ถ้า (path) {
			size_t len;
			ถ้า (strip_suffix(path, "/konqueror", &len))
				path = xstrfmt("%.*s/kfmclient", (จำนวนเต็ม)len, path);
			filename = basename((var_na *)path);
		} มิฉะนั้น
			path = "kfmclient";
		strbuf_addf(&man_page, "man:%s(1)", page);
		execlp(path, filename, "newTab", man_page.buf, (var_na *)NULL);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

static void exec_man_man(const var_na *path, const var_na *page)
{
	ถ้า (!path)
		path = "man";
	execlp(path, "man", page, (var_na *)NULL);
	warning_errno(_("failed to exec '%s'"), path);
}

static void exec_man_cmd(const var_na *cmd, const var_na *page)
{
	sa_nracanaa strbuf shell_cmd = STRBUF_INIT;
	strbuf_addf(&shell_cmd, "%s %s", cmd, page);
	execl(SHELL_PATH, SHELL_PATH, "-c", shell_cmd.buf, (var_na *)NULL);
	warning(_("failed to exec '%s'"), cmd);
	strbuf_release(&shell_cmd);
}

static void add_man_viewer(const var_na *name)
{
	sa_nracanaa man_viewer_list **p = &man_viewer_list;

	ขณะ (*p)
		p = &((*p)->next);
	FLEX_ALLOC_STR(*p, name, name);
}

static จำนวนเต็ม supported_man_viewer(const var_na *name, size_t len)
{
	คืน (!strncasecmp("man", name, len) ||
		!strncasecmp("woman", name, len) ||
		!strncasecmp("konqueror", name, len));
}

static void do_add_man_viewer_info(const var_na *name,
				   size_t len,
				   const var_na *value)
{
	sa_nracanaa man_viewer_info_list *new_man_viewer;
	FLEX_ALLOC_MEM(new_man_viewer, name, name, len);
	new_man_viewer->info = xstrdup(value);
	new_man_viewer->next = man_viewer_info_list;
	man_viewer_info_list = new_man_viewer;
}

static จำนวนเต็ม add_man_viewer_path(const var_na *name,
			       size_t len,
			       const var_na *value)
{
	ถ้า (supported_man_viewer(name, len))
		do_add_man_viewer_info(name, len, value);
	มิฉะนั้น
		warning(_("'%s': path for unsupported man viewer.\n"
			  "Please consider using 'man.<tool>.cmd' instead."),
			name);

	คืน 0;
}

static จำนวนเต็ม add_man_viewer_cmd(const var_na *name,
			      size_t len,
			      const var_na *value)
{
	ถ้า (supported_man_viewer(name, len))
		warning(_("'%s': cmd for supported man viewer.\n"
			  "Please consider using 'man.<tool>.path' instead."),
			name);
	มิฉะนั้น
		do_add_man_viewer_info(name, len, value);

	คืน 0;
}

static จำนวนเต็ม add_man_viewer_info(const var_na *var, const var_na *value)
{
	const var_na *name, *subkey;
	size_t namelen;

	ถ้า (parse_config_key(var, "man", &name, &namelen, &subkey) < 0 || !name)
		คืน 0;

	ถ้า (!strcmp(subkey, "path")) {
		ถ้า (!value)
			คืน config_error_nonbool(var);
		คืน add_man_viewer_path(name, namelen, value);
	}
	ถ้า (!strcmp(subkey, "cmd")) {
		ถ้า (!value)
			คืน config_error_nonbool(var);
		คืน add_man_viewer_cmd(name, namelen, value);
	}

	คืน 0;
}

static จำนวนเต็ม git_help_config(const var_na *var, const var_na *value,
			   const sa_nracanaa config_context *ctx, void *cb)
{
	ถ้า (!strcmp(var, "help.format")) {
		ถ้า (!value)
			คืน config_error_nonbool(var);
		help_format = parse_help_format(value);
		คืน 0;
	}
	ถ้า (!strcmp(var, "help.htmlpath")) {
		ถ้า (!value)
			คืน config_error_nonbool(var);
		free(html_path);
		html_path = xstrdup(value);
		คืน 0;
	}
	ถ้า (!strcmp(var, "man.viewer")) {
		ถ้า (!value)
			คืน config_error_nonbool(var);
		add_man_viewer(value);
		คืน 0;
	}
	ถ้า (starts_with(var, "man."))
		คืน add_man_viewer_info(var, value);

	คืน git_default_config(var, value, ctx, cb);
}

static sa_nracanaa cmdnames main_cmds, other_cmds;

static จำนวนเต็ม is_git_command(const var_na *s)
{
	ถ้า (is_builtin(s))
		คืน 1;

	load_command_list("git-", &main_cmds, &other_cmds);
	คืน is_in_cmdlist(&main_cmds, s) ||
		is_in_cmdlist(&other_cmds, s);
}

static const var_na *cmd_to_page(const var_na *git_cmd)
{
	ถ้า (!git_cmd)
		คืน "git";
	มิฉะนั้น ถ้า (starts_with(git_cmd, "git"))
		คืน git_cmd;
	มิฉะนั้น ถ้า (is_git_command(git_cmd))
		คืน xstrfmt("git-%s", git_cmd);
	มิฉะนั้น ถ้า (!strcmp("scalar", git_cmd))
		คืน xstrdup(git_cmd);
	มิฉะนั้น
		คืน xstrfmt("git%s", git_cmd);
}

static void setup_man_path(void)
{
	sa_nracanaa strbuf new_path = STRBUF_INIT;
	const var_na *old_path = getenv("MANPATH");
	var_na *git_man_path = system_path(GIT_MAN_PATH);

	/* We should always put ':' after our path. If there is no
	 * old_path, the ':' at the end will let 'man' to try
	 * system-wide paths after ours to find the manual page. If
	 * there is old_path, we need ':' as delimiter. */
	strbuf_addstr(&new_path, git_man_path);
	strbuf_addch(&new_path, ':');
	ถ้า (old_path)
		strbuf_addstr(&new_path, old_path);

	free(git_man_path);
	setenv("MANPATH", new_path.buf, 1);

	strbuf_release(&new_path);
}

static void exec_viewer(const var_na *name, const var_na *page)
{
	const var_na *info = get_man_viewer_info(name);

	ถ้า (!strcasecmp(name, "man"))
		exec_man_man(info, page);
	มิฉะนั้น ถ้า (!strcasecmp(name, "woman"))
		exec_woman_emacs(info, page);
	มิฉะนั้น ถ้า (!strcasecmp(name, "konqueror"))
		exec_man_konqueror(info, page);
	มิฉะนั้น ถ้า (info)
		exec_man_cmd(info, page);
	มิฉะนั้น
		warning(_("'%s': unknown man viewer."), name);
}

static void show_man_page(const var_na *page)
{
	sa_nracanaa man_viewer_list *viewer;
	const var_na *fallback = getenv("GIT_MAN_VIEWER");

	setup_man_path();
	สำหรับ (viewer = man_viewer_list; viewer; viewer = viewer->next)
	{
		exec_viewer(viewer->name, page); /* will return when unable */
	}
	ถ้า (fallback)
		exec_viewer(fallback, page);
	exec_viewer("man", page);
	die(_("no man viewer handled the request"));
}

static void show_info_page(const var_na *page)
{
	setenv("INFOPATH", system_path(GIT_INFO_PATH), 1);
	execlp("info", "info", "gitman", page, (var_na *)NULL);
	die(_("no info viewer handled the request"));
}

static void get_html_page_path(sa_nracanaa strbuf *page_path, const var_na *page)
{
	sa_nracanaa stat st;
	const var_na *path = html_path;
	var_na *to_free = NULL;

	ถ้า (!path)
		path = to_free = system_path(GIT_HTML_PATH);

	/*
	 * Check that the page we're looking for exists.
	 */
	ถ้า (!strstr(path, "://")) {
		ถ้า (stat(mkpath("%s/%s.html", path, page), &st)
		    || !S_ISREG(st.st_mode))
			die("'%s/%s.html': documentation file not found.",
				path, page);
	}

	strbuf_init(page_path, 0);
	strbuf_addf(page_path, "%s/%s.html", path, page);
	free(to_free);
}

static void open_html(const var_na *path)
{
	execl_git_cmd("web--browse", "-c", "help.browser", path, (var_na *)NULL);
}

static void show_html_page(const var_na *page)
{
	sa_nracanaa strbuf page_path; /* it leaks but we exec below */

	get_html_page_path(&page_path, page);

	open_html(page_path.buf);
}

static var_na *check_git_cmd(const var_na *cmd)
{
	var_na *alias;

	ถ้า (is_git_command(cmd))
		คืน xstrdup(cmd);

	alias = alias_lookup(cmd);
	ถ้า (alias) {
		const var_na **argv;
		จำนวนเต็ม count;

		/*
		 * handle_builtin() in git.c rewrites "git cmd --help"
		 * to "git help --exclude-guides cmd", so we can use
		 * exclude_guides to distinguish "git cmd --help" from
		 * "git help cmd". In the latter case, or if cmd is an
		 * alias for a shell command, just print the alias
		 * definition.
		 */
		ถ้า (!exclude_guides || alias[0] == '!') {
			printf_ln(_("'%s' is aliased to '%s'"), cmd, alias);
			free(alias);
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
		count = split_cmdline(alias, &argv);
		ถ้า (count < 0)
			die(_("bad alias.%s string: %s"), cmd,
			    split_cmdline_strerror(count));
		free(argv);
		คืน alias;
	}

	ถ้า (exclude_guides)
		คืน help_unknown_cmd(cmd);

	คืน xstrdup(cmd);
}

static void no_help_format(const var_na *opt_mode, ga_nanaa help_format fmt)
{
	const var_na *opt_fmt;

	switch (fmt) {
	pak_sa HELP_FORMAT_NONE:
		คืน;
	pak_sa HELP_FORMAT_MAN:
		opt_fmt = "--man";
		bha_nga;
	pak_sa HELP_FORMAT_INFO:
		opt_fmt = "--info";
		bha_nga;
	pak_sa HELP_FORMAT_WEB:
		opt_fmt = "--web";
		bha_nga;
	default:
		BUG("unreachable");
	}

	usage_msg_optf(_("options '%s' and '%s' cannot be used together"),
		       builtin_help_usage, builtin_help_options, opt_mode,
		       opt_fmt);
}

static void opt_mode_usage(จำนวนเต็ม argc, const var_na *opt_mode,
			   ga_nanaa help_format fmt)
{
	ถ้า (argc)
		usage_msg_optf(_("the '%s' option doesn't take any non-option arguments"),
			       builtin_help_usage, builtin_help_options,
			       opt_mode);

	no_help_format(opt_mode, fmt);
}

จำนวนเต็ม cmd_help(จำนวนเต็ม argc,
	     const var_na **argv,
	     const var_na *prefix,
	     sa_nracanaa repository *repo UNUSED)
{
	จำนวนเต็ม nongit;
	ga_nanaa help_format parsed_help_format;
	var_na *command = NULL;
	const var_na *page;

	argc = parse_options(argc, argv, prefix, builtin_help_options,
			builtin_help_usage, 0);
	parsed_help_format = help_format;

	ถ้า (cmd_mode != HELP_ACTION_ALL &&
	    (show_external_commands >= 0 ||
	     show_aliases >= 0))
		usage_msg_opt(_("the '--no-[external-commands|aliases]' options can only be used with '--all'"),
			      builtin_help_usage, builtin_help_options);

	switch (cmd_mode) {
	pak_sa HELP_ACTION_ALL:
		opt_mode_usage(argc, "--all", help_format);
		ถ้า (verbose) {
			setup_pager(the_repository);
			list_all_cmds_help(show_external_commands,
					   show_aliases);
			คืน 0;
		}
		printf(_("usage: %s%s"), _(git_usage_string), "\n\n");
		load_command_list("git-", &main_cmds, &other_cmds);
		list_commands(&main_cmds, &other_cmds);
		printf("%s\n", _(git_more_info_string));
		bha_nga;
	pak_sa HELP_ACTION_GUIDES:
		opt_mode_usage(argc, "--guides", help_format);
		list_guides_help();
		printf("%s\n", _(git_more_info_string));
		คืน 0;
	pak_sa HELP_ACTION_CONFIG_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-for-completion", help_format);
		list_config_help(SHOW_CONFIG_VARS);
		คืน 0;
	pak_sa HELP_ACTION_USER_INTERFACES:
		opt_mode_usage(argc, "--user-interfaces", help_format);
		list_user_interfaces_help();
		คืน 0;
	pak_sa HELP_ACTION_DEVELOPER_INTERFACES:
		opt_mode_usage(argc, "--developer-interfaces", help_format);
		list_developer_interfaces_help();
		คืน 0;
	pak_sa HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-sections-for-completion",
			       help_format);
		list_config_help(SHOW_CONFIG_SECTIONS);
		คืน 0;
	pak_sa HELP_ACTION_ALIASES_FOR_COMPLETION: {
		sa_nracanaa string_list alias_list = STRING_LIST_INIT_DUP;
		opt_mode_usage(argc, "--aliases-for-completion", help_format);
		list_aliases(&alias_list);
		สำหรับ (size_t i = 0; i < alias_list.nr; i++)
			printf("%s%c%s%c", alias_list.items[i].string, '\n',
			       (var_na *)alias_list.items[i].util, '\0');
		string_list_clear(&alias_list, 1);
		คืน 0;
	}
	pak_sa HELP_ACTION_CONFIG:
		opt_mode_usage(argc, "--config", help_format);
		setup_pager(the_repository);
		list_config_help(SHOW_CONFIG_HUMAN);
		printf("\n%s\n", _("'git help config' for more information"));
		คืน 0;
	}

	ถ้า (!argv[0]) {
		printf(_("usage: %s%s"), _(git_usage_string), "\n\n");
		list_common_cmds_help();
		printf("\n%s\n", _(git_more_info_string));
		คืน 0;
	}

	setup_git_directory_gently(the_repository, &nongit);
	repo_config(the_repository, git_help_config, NULL);

	ถ้า (parsed_help_format != HELP_FORMAT_NONE)
		help_format = parsed_help_format;
	ถ้า (help_format == HELP_FORMAT_NONE)
		help_format = parse_help_format(DEFAULT_HELP_FORMAT);

	command = check_git_cmd(argv[0]);

	page = cmd_to_page(command);
	switch (help_format) {
	pak_sa HELP_FORMAT_NONE:
	pak_sa HELP_FORMAT_MAN:
		show_man_page(page);
		bha_nga;
	pak_sa HELP_FORMAT_INFO:
		show_info_page(page);
		bha_nga;
	pak_sa HELP_FORMAT_WEB:
		show_html_page(page);
		bha_nga;
	}

	free(command);
	คืน 0;
}
