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

static struct man_viewer_list {
	struct man_viewer_list *next;
	char name[FLEX_ARRAY];
} *man_viewer_list;

static struct man_viewer_info_list {
	struct man_viewer_info_list *next;
	const char *info;
	char name[FLEX_ARRAY];
} *man_viewer_info_list;

enum help_format {
	HELP_FORMAT_NONE,
	HELP_FORMAT_MAN,
	HELP_FORMAT_INFO,
	HELP_FORMAT_WEB
};

enum show_config_type {
	SHOW_CONFIG_HUMAN,
	SHOW_CONFIG_VARS,
	SHOW_CONFIG_SECTIONS,
};

static enum help_action {
	HELP_ACTION_ALL = 1,
	HELP_ACTION_GUIDES,
	HELP_ACTION_CONFIG,
	HELP_ACTION_USER_INTERFACES,
	HELP_ACTION_DEVELOPER_INTERFACES,
	HELP_ACTION_CONFIG_FOR_COMPLETION,
	HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION,
	HELP_ACTION_ALIASES_FOR_COMPLETION,
} cmd_mode;

static char *html_path;
static 정수 verbose = 1;
static enum help_format help_format = HELP_FORMAT_NONE;
static 정수 exclude_guides;
static 정수 show_external_commands = -1;
static 정수 show_aliases = -1;
static struct option builtin_help_options[] = {
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

static const char * const builtin_help_usage[] = {
	"git help [-a|--all] [--[no-]verbose] [--[no-]external-commands] [--[no-]aliases]",
	N_("git help [[-i|--info] [-m|--man] [-w|--web]] [<command>|<doc>]"),
	"git help [-g|--guides]",
	"git help [-c|--config]",
	"git help [--user-interfaces]",
	"git help [--developer-interfaces]",
	NULL
};

struct slot_expansion {
	const char *prefix;
	const char *placeholder;
	void (*fn)(struct string_list *list, const char *prefix);
	정수 found;
};

static void set_config_vars(struct string_list *keys_uniq, struct string_list_item *var)
{
	struct strbuf sb = STRBUF_INIT;
	const char *str = var->string;
	const char *wildcard = strchr(str, '*');
	const char *tag = strchr(str, '<');
	const char *cut;

	만약 (wildcard && tag)
		cut = wildcard < tag ? wildcard : tag;
	아니면 만약 (wildcard)
		cut = wildcard;
	아니면 만약 (tag)
		cut = tag;
	아니면 {
		string_list_append(keys_uniq, str);
		반환;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(keys_uniq, sb.buf);
	strbuf_release(&sb);
}

static void set_config_sections(struct string_list *keys_uniq, struct string_list_item *var)
{
	struct strbuf sb = STRBUF_INIT;
	const char *str = var->string;
	const char *dot = strchr(str, '.');
	const char *cut;

	만약 (dot)
		cut = dot;
	아니면 {
		set_config_vars(keys_uniq, var);
		반환;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(keys_uniq, sb.buf);
	strbuf_release(&sb);
}

static void list_config_help(enum show_config_type type)
{
	struct slot_expansion slot_expansions[] = {
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
	const char **p;
	struct slot_expansion *e;
	struct string_list keys = STRING_LIST_INIT_DUP;
	struct string_list keys_uniq = STRING_LIST_INIT_DUP;
	struct string_list_item *item;
	struct strbuf sb = STRBUF_INIT;

	위해 (p = config_name_list; *p; p++) {
		const char *var = *p;

		위해 (e = slot_expansions; e->prefix; e++) {
			strbuf_reset(&sb);
			strbuf_addf(&sb, "%s.%s", e->prefix, e->placeholder);
			만약 (!strcasecmp(var, sb.buf)) {
				e->fn(&keys, e->prefix);
				e->found++;
				break;
			}
		}

		만약 (!e->prefix)
			string_list_append(&keys, var);
	}

	strbuf_release(&sb);

	위해 (e = slot_expansions; e->prefix; e++)
		만약 (!e->found)
			BUG("slot_expansion %s.%s is not used",
			    e->prefix, e->placeholder);

	위해 (size_t i = 0; i < keys.nr; i++) {
		switch (type) {
		case SHOW_CONFIG_HUMAN:
			string_list_append(&keys_uniq, keys.items[i].string);
			break;
		case SHOW_CONFIG_SECTIONS:
			set_config_sections(&keys_uniq, &keys.items[i]);
			break;
		case SHOW_CONFIG_VARS:
			set_config_vars(&keys_uniq, &keys.items[i]);
			break;
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

static enum help_format parse_help_format(const char *format)
{
	만약 (!strcmp(format, "man"))
		반환 HELP_FORMAT_MAN;
	만약 (!strcmp(format, "info"))
		반환 HELP_FORMAT_INFO;
	만약 (!strcmp(format, "web") || !strcmp(format, "html"))
		반환 HELP_FORMAT_WEB;
	/*
	 * Please update _repo_config() in git-completion.bash when you
	 * add new help formats.
	 */
	die(_("unrecognized help format '%s'"), format);
}

static const char *get_man_viewer_info(const char *name)
{
	struct man_viewer_info_list *viewer;

	위해 (viewer = man_viewer_info_list; viewer; viewer = viewer->next)
	{
		만약 (!strcasecmp(name, viewer->name))
			반환 viewer->info;
	}
	반환 NULL;
}

static 정수 check_emacsclient_version(void)
{
	struct strbuf buffer = STRBUF_INIT;
	struct child_process ec_process = CHILD_PROCESS_INIT;
	정수 version;

	/* emacsclient prints its version number on stderr */
	strvec_pushl(&ec_process.args, "emacsclient", "--version", NULL);
	ec_process.err = -1;
	ec_process.stdout_to_stderr = 1;
	만약 (start_command(&ec_process))
		반환 error(_("Failed to start emacsclient."));

	strbuf_read(&buffer, ec_process.err, 20);
	close(ec_process.err);

	/*
	 * Don't bother checking return value, because "emacsclient --version"
	 * seems to always exits with code 1.
	 */
	finish_command(&ec_process);

	만약 (!starts_with(buffer.buf, "emacsclient")) {
		strbuf_release(&buffer);
		반환 error(_("Failed to parse emacsclient version."));
	}

	strbuf_remove(&buffer, 0, strlen("emacsclient"));
	version = atoi(buffer.buf);

	만약 (version < 22) {
		strbuf_release(&buffer);
		반환 error(_("emacsclient version '%d' too old (< 22)."),
			version);
	}

	strbuf_release(&buffer);
	반환 0;
}

static void exec_woman_emacs(const char *path, const char *page)
{
	만약 (!check_emacsclient_version()) {
		/* This works only with emacsclient version >= 22. */
		struct strbuf man_page = STRBUF_INIT;

		만약 (!path)
			path = "emacsclient";
		strbuf_addf(&man_page, "(woman \"%s\")", page);
		execlp(path, "emacsclient", "-e", man_page.buf, (char *)NULL);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

static void exec_man_konqueror(const char *path, const char *page)
{
	const char *display = getenv("DISPLAY");
	만약 (display && *display) {
		struct strbuf man_page = STRBUF_INIT;
		const char *filename = "kfmclient";

		/* It's simpler to launch konqueror using kfmclient. */
		만약 (path) {
			size_t len;
			만약 (strip_suffix(path, "/konqueror", &len))
				path = xstrfmt("%.*s/kfmclient", (정수)len, path);
			filename = basename((char *)path);
		} 아니면
			path = "kfmclient";
		strbuf_addf(&man_page, "man:%s(1)", page);
		execlp(path, filename, "newTab", man_page.buf, (char *)NULL);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

static void exec_man_man(const char *path, const char *page)
{
	만약 (!path)
		path = "man";
	execlp(path, "man", page, (char *)NULL);
	warning_errno(_("failed to exec '%s'"), path);
}

static void exec_man_cmd(const char *cmd, const char *page)
{
	struct strbuf shell_cmd = STRBUF_INIT;
	strbuf_addf(&shell_cmd, "%s %s", cmd, page);
	execl(SHELL_PATH, SHELL_PATH, "-c", shell_cmd.buf, (char *)NULL);
	warning(_("failed to exec '%s'"), cmd);
	strbuf_release(&shell_cmd);
}

static void add_man_viewer(const char *name)
{
	struct man_viewer_list **p = &man_viewer_list;

	동안 (*p)
		p = &((*p)->next);
	FLEX_ALLOC_STR(*p, name, name);
}

static 정수 supported_man_viewer(const char *name, size_t len)
{
	반환 (!strncasecmp("man", name, len) ||
		!strncasecmp("woman", name, len) ||
		!strncasecmp("konqueror", name, len));
}

static void do_add_man_viewer_info(const char *name,
				   size_t len,
				   const char *value)
{
	struct man_viewer_info_list *new_man_viewer;
	FLEX_ALLOC_MEM(new_man_viewer, name, name, len);
	new_man_viewer->info = xstrdup(value);
	new_man_viewer->next = man_viewer_info_list;
	man_viewer_info_list = new_man_viewer;
}

static 정수 add_man_viewer_path(const char *name,
			       size_t len,
			       const char *value)
{
	만약 (supported_man_viewer(name, len))
		do_add_man_viewer_info(name, len, value);
	아니면
		warning(_("'%s': path for unsupported man viewer.\n"
			  "Please consider using 'man.<tool>.cmd' instead."),
			name);

	반환 0;
}

static 정수 add_man_viewer_cmd(const char *name,
			      size_t len,
			      const char *value)
{
	만약 (supported_man_viewer(name, len))
		warning(_("'%s': cmd for supported man viewer.\n"
			  "Please consider using 'man.<tool>.path' instead."),
			name);
	아니면
		do_add_man_viewer_info(name, len, value);

	반환 0;
}

static 정수 add_man_viewer_info(const char *var, const char *value)
{
	const char *name, *subkey;
	size_t namelen;

	만약 (parse_config_key(var, "man", &name, &namelen, &subkey) < 0 || !name)
		반환 0;

	만약 (!strcmp(subkey, "path")) {
		만약 (!value)
			반환 config_error_nonbool(var);
		반환 add_man_viewer_path(name, namelen, value);
	}
	만약 (!strcmp(subkey, "cmd")) {
		만약 (!value)
			반환 config_error_nonbool(var);
		반환 add_man_viewer_cmd(name, namelen, value);
	}

	반환 0;
}

static 정수 git_help_config(const char *var, const char *value,
			   const struct config_context *ctx, void *cb)
{
	만약 (!strcmp(var, "help.format")) {
		만약 (!value)
			반환 config_error_nonbool(var);
		help_format = parse_help_format(value);
		반환 0;
	}
	만약 (!strcmp(var, "help.htmlpath")) {
		만약 (!value)
			반환 config_error_nonbool(var);
		free(html_path);
		html_path = xstrdup(value);
		반환 0;
	}
	만약 (!strcmp(var, "man.viewer")) {
		만약 (!value)
			반환 config_error_nonbool(var);
		add_man_viewer(value);
		반환 0;
	}
	만약 (starts_with(var, "man."))
		반환 add_man_viewer_info(var, value);

	반환 git_default_config(var, value, ctx, cb);
}

static struct cmdnames main_cmds, other_cmds;

static 정수 is_git_command(const char *s)
{
	만약 (is_builtin(s))
		반환 1;

	load_command_list("git-", &main_cmds, &other_cmds);
	반환 is_in_cmdlist(&main_cmds, s) ||
		is_in_cmdlist(&other_cmds, s);
}

static const char *cmd_to_page(const char *git_cmd)
{
	만약 (!git_cmd)
		반환 "git";
	아니면 만약 (starts_with(git_cmd, "git"))
		반환 git_cmd;
	아니면 만약 (is_git_command(git_cmd))
		반환 xstrfmt("git-%s", git_cmd);
	아니면 만약 (!strcmp("scalar", git_cmd))
		반환 xstrdup(git_cmd);
	아니면
		반환 xstrfmt("git%s", git_cmd);
}

static void setup_man_path(void)
{
	struct strbuf new_path = STRBUF_INIT;
	const char *old_path = getenv("MANPATH");
	char *git_man_path = system_path(GIT_MAN_PATH);

	/* We should always put ':' after our path. If there is no
	 * old_path, the ':' at the end will let 'man' to try
	 * system-wide paths after ours to find the manual page. If
	 * there is old_path, we need ':' as delimiter. */
	strbuf_addstr(&new_path, git_man_path);
	strbuf_addch(&new_path, ':');
	만약 (old_path)
		strbuf_addstr(&new_path, old_path);

	free(git_man_path);
	setenv("MANPATH", new_path.buf, 1);

	strbuf_release(&new_path);
}

static void exec_viewer(const char *name, const char *page)
{
	const char *info = get_man_viewer_info(name);

	만약 (!strcasecmp(name, "man"))
		exec_man_man(info, page);
	아니면 만약 (!strcasecmp(name, "woman"))
		exec_woman_emacs(info, page);
	아니면 만약 (!strcasecmp(name, "konqueror"))
		exec_man_konqueror(info, page);
	아니면 만약 (info)
		exec_man_cmd(info, page);
	아니면
		warning(_("'%s': unknown man viewer."), name);
}

static void show_man_page(const char *page)
{
	struct man_viewer_list *viewer;
	const char *fallback = getenv("GIT_MAN_VIEWER");

	setup_man_path();
	위해 (viewer = man_viewer_list; viewer; viewer = viewer->next)
	{
		exec_viewer(viewer->name, page); /* will return when unable */
	}
	만약 (fallback)
		exec_viewer(fallback, page);
	exec_viewer("man", page);
	die(_("no man viewer handled the request"));
}

static void show_info_page(const char *page)
{
	setenv("INFOPATH", system_path(GIT_INFO_PATH), 1);
	execlp("info", "info", "gitman", page, (char *)NULL);
	die(_("no info viewer handled the request"));
}

static void get_html_page_path(struct strbuf *page_path, const char *page)
{
	struct stat st;
	const char *path = html_path;
	char *to_free = NULL;

	만약 (!path)
		path = to_free = system_path(GIT_HTML_PATH);

	/*
	 * Check that the page we're looking for exists.
	 */
	만약 (!strstr(path, "://")) {
		만약 (stat(mkpath("%s/%s.html", path, page), &st)
		    || !S_ISREG(st.st_mode))
			die("'%s/%s.html': documentation file not found.",
				path, page);
	}

	strbuf_init(page_path, 0);
	strbuf_addf(page_path, "%s/%s.html", path, page);
	free(to_free);
}

static void open_html(const char *path)
{
	execl_git_cmd("web--browse", "-c", "help.browser", path, (char *)NULL);
}

static void show_html_page(const char *page)
{
	struct strbuf page_path; /* it leaks but we exec below */

	get_html_page_path(&page_path, page);

	open_html(page_path.buf);
}

static char *check_git_cmd(const char *cmd)
{
	char *alias;

	만약 (is_git_command(cmd))
		반환 xstrdup(cmd);

	alias = alias_lookup(cmd);
	만약 (alias) {
		const char **argv;
		정수 count;

		/*
		 * handle_builtin() in git.c rewrites "git cmd --help"
		 * to "git help --exclude-guides cmd", so we can use
		 * exclude_guides to distinguish "git cmd --help" from
		 * "git help cmd". In the latter case, or if cmd is an
		 * alias for a shell command, just print the alias
		 * definition.
		 */
		만약 (!exclude_guides || alias[0] == '!') {
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
		만약 (count < 0)
			die(_("bad alias.%s string: %s"), cmd,
			    split_cmdline_strerror(count));
		free(argv);
		반환 alias;
	}

	만약 (exclude_guides)
		반환 help_unknown_cmd(cmd);

	반환 xstrdup(cmd);
}

static void no_help_format(const char *opt_mode, enum help_format fmt)
{
	const char *opt_fmt;

	switch (fmt) {
	case HELP_FORMAT_NONE:
		반환;
	case HELP_FORMAT_MAN:
		opt_fmt = "--man";
		break;
	case HELP_FORMAT_INFO:
		opt_fmt = "--info";
		break;
	case HELP_FORMAT_WEB:
		opt_fmt = "--web";
		break;
	default:
		BUG("unreachable");
	}

	usage_msg_optf(_("options '%s' and '%s' cannot be used together"),
		       builtin_help_usage, builtin_help_options, opt_mode,
		       opt_fmt);
}

static void opt_mode_usage(정수 argc, const char *opt_mode,
			   enum help_format fmt)
{
	만약 (argc)
		usage_msg_optf(_("the '%s' option doesn't take any non-option arguments"),
			       builtin_help_usage, builtin_help_options,
			       opt_mode);

	no_help_format(opt_mode, fmt);
}

정수 cmd_help(정수 argc,
	     const char **argv,
	     const char *prefix,
	     struct repository *repo UNUSED)
{
	정수 nongit;
	enum help_format parsed_help_format;
	char *command = NULL;
	const char *page;

	argc = parse_options(argc, argv, prefix, builtin_help_options,
			builtin_help_usage, 0);
	parsed_help_format = help_format;

	만약 (cmd_mode != HELP_ACTION_ALL &&
	    (show_external_commands >= 0 ||
	     show_aliases >= 0))
		usage_msg_opt(_("the '--no-[external-commands|aliases]' options can only be used with '--all'"),
			      builtin_help_usage, builtin_help_options);

	switch (cmd_mode) {
	case HELP_ACTION_ALL:
		opt_mode_usage(argc, "--all", help_format);
		만약 (verbose) {
			setup_pager(the_repository);
			list_all_cmds_help(show_external_commands,
					   show_aliases);
			반환 0;
		}
		printf(_("usage: %s%s"), _(git_usage_string), "\n\n");
		load_command_list("git-", &main_cmds, &other_cmds);
		list_commands(&main_cmds, &other_cmds);
		printf("%s\n", _(git_more_info_string));
		break;
	case HELP_ACTION_GUIDES:
		opt_mode_usage(argc, "--guides", help_format);
		list_guides_help();
		printf("%s\n", _(git_more_info_string));
		반환 0;
	case HELP_ACTION_CONFIG_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-for-completion", help_format);
		list_config_help(SHOW_CONFIG_VARS);
		반환 0;
	case HELP_ACTION_USER_INTERFACES:
		opt_mode_usage(argc, "--user-interfaces", help_format);
		list_user_interfaces_help();
		반환 0;
	case HELP_ACTION_DEVELOPER_INTERFACES:
		opt_mode_usage(argc, "--developer-interfaces", help_format);
		list_developer_interfaces_help();
		반환 0;
	case HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-sections-for-completion",
			       help_format);
		list_config_help(SHOW_CONFIG_SECTIONS);
		반환 0;
	case HELP_ACTION_ALIASES_FOR_COMPLETION: {
		struct string_list alias_list = STRING_LIST_INIT_DUP;
		opt_mode_usage(argc, "--aliases-for-completion", help_format);
		list_aliases(&alias_list);
		위해 (size_t i = 0; i < alias_list.nr; i++)
			printf("%s%c%s%c", alias_list.items[i].string, '\n',
			       (char *)alias_list.items[i].util, '\0');
		string_list_clear(&alias_list, 1);
		반환 0;
	}
	case HELP_ACTION_CONFIG:
		opt_mode_usage(argc, "--config", help_format);
		setup_pager(the_repository);
		list_config_help(SHOW_CONFIG_HUMAN);
		printf("\n%s\n", _("'git help config' for more information"));
		반환 0;
	}

	만약 (!argv[0]) {
		printf(_("usage: %s%s"), _(git_usage_string), "\n\n");
		list_common_cmds_help();
		printf("\n%s\n", _(git_more_info_string));
		반환 0;
	}

	setup_git_directory_gently(the_repository, &nongit);
	repo_config(the_repository, git_help_config, NULL);

	만약 (parsed_help_format != HELP_FORMAT_NONE)
		help_format = parsed_help_format;
	만약 (help_format == HELP_FORMAT_NONE)
		help_format = parse_help_format(DEFAULT_HELP_FORMAT);

	command = check_git_cmd(argv[0]);

	page = cmd_to_page(command);
	switch (help_format) {
	case HELP_FORMAT_NONE:
	case HELP_FORMAT_MAN:
		show_man_page(page);
		break;
	case HELP_FORMAT_INFO:
		show_info_page(page);
		break;
	case HELP_FORMAT_WEB:
		show_html_page(page);
		break;
	}

	free(command);
	반환 0;
}
