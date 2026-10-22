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
static ціле verbose = 1;
static enum help_format help_format = HELP_FORMAT_NONE;
static ціле exclude_guides;
static ціле show_external_commands = -1;
static ціле show_aliases = -1;
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
	ціле found;
};

static void set_config_vars(struct string_list *keys_uniq, struct string_list_item *var)
{
	struct strbuf sb = STRBUF_INIT;
	const char *str = var->string;
	const char *wildcard = strchr(str, '*');
	const char *tag = strchr(str, '<');
	const char *cut;

	якщо (wildcard && tag)
		cut = wildcard < tag ? wildcard : tag;
	інакше якщо (wildcard)
		cut = wildcard;
	інакше якщо (tag)
		cut = tag;
	інакше {
		string_list_append(keys_uniq, str);
		повернути;
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

	якщо (dot)
		cut = dot;
	інакше {
		set_config_vars(keys_uniq, var);
		повернути;
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

	для (p = config_name_list; *p; p++) {
		const char *var = *p;

		для (e = slot_expansions; e->prefix; e++) {
			strbuf_reset(&sb);
			strbuf_addf(&sb, "%s.%s", e->prefix, e->placeholder);
			якщо (!strcasecmp(var, sb.buf)) {
				e->fn(&keys, e->prefix);
				e->found++;
				break;
			}
		}

		якщо (!e->prefix)
			string_list_append(&keys, var);
	}

	strbuf_release(&sb);

	для (e = slot_expansions; e->prefix; e++)
		якщо (!e->found)
			BUG("slot_expansion %s.%s is not used",
			    e->prefix, e->placeholder);

	для (size_t i = 0; i < keys.nr; i++) {
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
	якщо (!strcmp(format, "man"))
		повернути HELP_FORMAT_MAN;
	якщо (!strcmp(format, "info"))
		повернути HELP_FORMAT_INFO;
	якщо (!strcmp(format, "web") || !strcmp(format, "html"))
		повернути HELP_FORMAT_WEB;
	/*
	 * Please update _repo_config() in git-completion.bash when you
	 * add new help formats.
	 */
	die(_("unrecognized help format '%s'"), format);
}

static const char *get_man_viewer_info(const char *name)
{
	struct man_viewer_info_list *viewer;

	для (viewer = man_viewer_info_list; viewer; viewer = viewer->next)
	{
		якщо (!strcasecmp(name, viewer->name))
			повернути viewer->info;
	}
	повернути NULL;
}

static ціле check_emacsclient_version(void)
{
	struct strbuf buffer = STRBUF_INIT;
	struct child_process ec_process = CHILD_PROCESS_INIT;
	ціле version;

	/* emacsclient prints its version number on stderr */
	strvec_pushl(&ec_process.args, "emacsclient", "--version", NULL);
	ec_process.err = -1;
	ec_process.stdout_to_stderr = 1;
	якщо (start_command(&ec_process))
		повернути error(_("Failed to start emacsclient."));

	strbuf_read(&buffer, ec_process.err, 20);
	close(ec_process.err);

	/*
	 * Don't bother checking return value, because "emacsclient --version"
	 * seems to always exits with code 1.
	 */
	finish_command(&ec_process);

	якщо (!starts_with(buffer.buf, "emacsclient")) {
		strbuf_release(&buffer);
		повернути error(_("Failed to parse emacsclient version."));
	}

	strbuf_remove(&buffer, 0, strlen("emacsclient"));
	version = atoi(buffer.buf);

	якщо (version < 22) {
		strbuf_release(&buffer);
		повернути error(_("emacsclient version '%d' too old (< 22)."),
			version);
	}

	strbuf_release(&buffer);
	повернути 0;
}

static void exec_woman_emacs(const char *path, const char *page)
{
	якщо (!check_emacsclient_version()) {
		/* This works only with emacsclient version >= 22. */
		struct strbuf man_page = STRBUF_INIT;

		якщо (!path)
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
	якщо (display && *display) {
		struct strbuf man_page = STRBUF_INIT;
		const char *filename = "kfmclient";

		/* It's simpler to launch konqueror using kfmclient. */
		якщо (path) {
			size_t len;
			якщо (strip_suffix(path, "/konqueror", &len))
				path = xstrfmt("%.*s/kfmclient", (ціле)len, path);
			filename = basename((char *)path);
		} інакше
			path = "kfmclient";
		strbuf_addf(&man_page, "man:%s(1)", page);
		execlp(path, filename, "newTab", man_page.buf, (char *)NULL);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

static void exec_man_man(const char *path, const char *page)
{
	якщо (!path)
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

	поки (*p)
		p = &((*p)->next);
	FLEX_ALLOC_STR(*p, name, name);
}

static ціле supported_man_viewer(const char *name, size_t len)
{
	повернути (!strncasecmp("man", name, len) ||
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

static ціле add_man_viewer_path(const char *name,
			       size_t len,
			       const char *value)
{
	якщо (supported_man_viewer(name, len))
		do_add_man_viewer_info(name, len, value);
	інакше
		warning(_("'%s': path for unsupported man viewer.\n"
			  "Please consider using 'man.<tool>.cmd' instead."),
			name);

	повернути 0;
}

static ціле add_man_viewer_cmd(const char *name,
			      size_t len,
			      const char *value)
{
	якщо (supported_man_viewer(name, len))
		warning(_("'%s': cmd for supported man viewer.\n"
			  "Please consider using 'man.<tool>.path' instead."),
			name);
	інакше
		do_add_man_viewer_info(name, len, value);

	повернути 0;
}

static ціле add_man_viewer_info(const char *var, const char *value)
{
	const char *name, *subkey;
	size_t namelen;

	якщо (parse_config_key(var, "man", &name, &namelen, &subkey) < 0 || !name)
		повернути 0;

	якщо (!strcmp(subkey, "path")) {
		якщо (!value)
			повернути config_error_nonbool(var);
		повернути add_man_viewer_path(name, namelen, value);
	}
	якщо (!strcmp(subkey, "cmd")) {
		якщо (!value)
			повернути config_error_nonbool(var);
		повернути add_man_viewer_cmd(name, namelen, value);
	}

	повернути 0;
}

static ціле git_help_config(const char *var, const char *value,
			   const struct config_context *ctx, void *cb)
{
	якщо (!strcmp(var, "help.format")) {
		якщо (!value)
			повернути config_error_nonbool(var);
		help_format = parse_help_format(value);
		повернути 0;
	}
	якщо (!strcmp(var, "help.htmlpath")) {
		якщо (!value)
			повернути config_error_nonbool(var);
		free(html_path);
		html_path = xstrdup(value);
		повернути 0;
	}
	якщо (!strcmp(var, "man.viewer")) {
		якщо (!value)
			повернути config_error_nonbool(var);
		add_man_viewer(value);
		повернути 0;
	}
	якщо (starts_with(var, "man."))
		повернути add_man_viewer_info(var, value);

	повернути git_default_config(var, value, ctx, cb);
}

static struct cmdnames main_cmds, other_cmds;

static ціле is_git_command(const char *s)
{
	якщо (is_builtin(s))
		повернути 1;

	load_command_list("git-", &main_cmds, &other_cmds);
	повернути is_in_cmdlist(&main_cmds, s) ||
		is_in_cmdlist(&other_cmds, s);
}

static const char *cmd_to_page(const char *git_cmd)
{
	якщо (!git_cmd)
		повернути "git";
	інакше якщо (starts_with(git_cmd, "git"))
		повернути git_cmd;
	інакше якщо (is_git_command(git_cmd))
		повернути xstrfmt("git-%s", git_cmd);
	інакше якщо (!strcmp("scalar", git_cmd))
		повернути xstrdup(git_cmd);
	інакше
		повернути xstrfmt("git%s", git_cmd);
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
	якщо (old_path)
		strbuf_addstr(&new_path, old_path);

	free(git_man_path);
	setenv("MANPATH", new_path.buf, 1);

	strbuf_release(&new_path);
}

static void exec_viewer(const char *name, const char *page)
{
	const char *info = get_man_viewer_info(name);

	якщо (!strcasecmp(name, "man"))
		exec_man_man(info, page);
	інакше якщо (!strcasecmp(name, "woman"))
		exec_woman_emacs(info, page);
	інакше якщо (!strcasecmp(name, "konqueror"))
		exec_man_konqueror(info, page);
	інакше якщо (info)
		exec_man_cmd(info, page);
	інакше
		warning(_("'%s': unknown man viewer."), name);
}

static void show_man_page(const char *page)
{
	struct man_viewer_list *viewer;
	const char *fallback = getenv("GIT_MAN_VIEWER");

	setup_man_path();
	для (viewer = man_viewer_list; viewer; viewer = viewer->next)
	{
		exec_viewer(viewer->name, page); /* will return when unable */
	}
	якщо (fallback)
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

	якщо (!path)
		path = to_free = system_path(GIT_HTML_PATH);

	/*
	 * Check that the page we're looking for exists.
	 */
	якщо (!strstr(path, "://")) {
		якщо (stat(mkpath("%s/%s.html", path, page), &st)
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

	якщо (is_git_command(cmd))
		повернути xstrdup(cmd);

	alias = alias_lookup(cmd);
	якщо (alias) {
		const char **argv;
		ціле count;

		/*
		 * handle_builtin() in git.c rewrites "git cmd --help"
		 * to "git help --exclude-guides cmd", so we can use
		 * exclude_guides to distinguish "git cmd --help" from
		 * "git help cmd". In the latter case, or if cmd is an
		 * alias for a shell command, just print the alias
		 * definition.
		 */
		якщо (!exclude_guides || alias[0] == '!') {
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
		якщо (count < 0)
			die(_("bad alias.%s string: %s"), cmd,
			    split_cmdline_strerror(count));
		free(argv);
		повернути alias;
	}

	якщо (exclude_guides)
		повернути help_unknown_cmd(cmd);

	повернути xstrdup(cmd);
}

static void no_help_format(const char *opt_mode, enum help_format fmt)
{
	const char *opt_fmt;

	switch (fmt) {
	case HELP_FORMAT_NONE:
		повернути;
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

static void opt_mode_usage(ціле argc, const char *opt_mode,
			   enum help_format fmt)
{
	якщо (argc)
		usage_msg_optf(_("the '%s' option doesn't take any non-option arguments"),
			       builtin_help_usage, builtin_help_options,
			       opt_mode);

	no_help_format(opt_mode, fmt);
}

ціле cmd_help(ціле argc,
	     const char **argv,
	     const char *prefix,
	     struct repository *repo UNUSED)
{
	ціле nongit;
	enum help_format parsed_help_format;
	char *command = NULL;
	const char *page;

	argc = parse_options(argc, argv, prefix, builtin_help_options,
			builtin_help_usage, 0);
	parsed_help_format = help_format;

	якщо (cmd_mode != HELP_ACTION_ALL &&
	    (show_external_commands >= 0 ||
	     show_aliases >= 0))
		usage_msg_opt(_("the '--no-[external-commands|aliases]' options can only be used with '--all'"),
			      builtin_help_usage, builtin_help_options);

	switch (cmd_mode) {
	case HELP_ACTION_ALL:
		opt_mode_usage(argc, "--all", help_format);
		якщо (verbose) {
			setup_pager(the_repository);
			list_all_cmds_help(show_external_commands,
					   show_aliases);
			повернути 0;
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
		повернути 0;
	case HELP_ACTION_CONFIG_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-for-completion", help_format);
		list_config_help(SHOW_CONFIG_VARS);
		повернути 0;
	case HELP_ACTION_USER_INTERFACES:
		opt_mode_usage(argc, "--user-interfaces", help_format);
		list_user_interfaces_help();
		повернути 0;
	case HELP_ACTION_DEVELOPER_INTERFACES:
		opt_mode_usage(argc, "--developer-interfaces", help_format);
		list_developer_interfaces_help();
		повернути 0;
	case HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-sections-for-completion",
			       help_format);
		list_config_help(SHOW_CONFIG_SECTIONS);
		повернути 0;
	case HELP_ACTION_ALIASES_FOR_COMPLETION: {
		struct string_list alias_list = STRING_LIST_INIT_DUP;
		opt_mode_usage(argc, "--aliases-for-completion", help_format);
		list_aliases(&alias_list);
		для (size_t i = 0; i < alias_list.nr; i++)
			printf("%s%c%s%c", alias_list.items[i].string, '\n',
			       (char *)alias_list.items[i].util, '\0');
		string_list_clear(&alias_list, 1);
		повернути 0;
	}
	case HELP_ACTION_CONFIG:
		opt_mode_usage(argc, "--config", help_format);
		setup_pager(the_repository);
		list_config_help(SHOW_CONFIG_HUMAN);
		printf("\n%s\n", _("'git help config' for more information"));
		повернути 0;
	}

	якщо (!argv[0]) {
		printf(_("usage: %s%s"), _(git_usage_string), "\n\n");
		list_common_cmds_help();
		printf("\n%s\n", _(git_more_info_string));
		повернути 0;
	}

	setup_git_directory_gently(the_repository, &nongit);
	repo_config(the_repository, git_help_config, NULL);

	якщо (parsed_help_format != HELP_FORMAT_NONE)
		help_format = parsed_help_format;
	якщо (help_format == HELP_FORMAT_NONE)
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
	повернути 0;
}
