/*
 * Builtin help command
 */

#define USE_THE_REPOSITORY_VARIABLE

#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "exec-cmd.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "pager.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "config-list.h"
#ଅନ୍ତର୍ଭାଵ "help.h"
#ଅନ୍ତର୍ଭାଵ "alias.h"
#ଅନ୍ତର୍ଭାଵ "setup.h"

#ifndef DEFAULT_HELP_FORMAT
#define DEFAULT_HELP_FORMAT "man"
#endif

ସ୍ଥିର ସଂରଚନା man_viewer_list {
	ସଂରଚନା man_viewer_list *next;
	ଅକ୍ଷର name[FLEX_ARRAY];
} *man_viewer_list;

ସ୍ଥିର ସଂରଚନା man_viewer_info_list {
	ସଂରଚନା man_viewer_info_list *next;
	ଅଚଳ ଅକ୍ଷର *info;
	ଅକ୍ଷର name[FLEX_ARRAY];
} *man_viewer_info_list;

ଗଣନା help_format {
	HELP_FORMAT_NONE,
	HELP_FORMAT_MAN,
	HELP_FORMAT_INFO,
	HELP_FORMAT_WEB
};

ଗଣନା show_config_type {
	SHOW_CONFIG_HUMAN,
	SHOW_CONFIG_VARS,
	SHOW_CONFIG_SECTIONS,
};

ସ୍ଥିର ଗଣନା help_action {
	HELP_ACTION_ALL = 1,
	HELP_ACTION_GUIDES,
	HELP_ACTION_CONFIG,
	HELP_ACTION_USER_INTERFACES,
	HELP_ACTION_DEVELOPER_INTERFACES,
	HELP_ACTION_CONFIG_FOR_COMPLETION,
	HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION,
	HELP_ACTION_ALIASES_FOR_COMPLETION,
} cmd_mode;

ସ୍ଥିର ଅକ୍ଷର *html_path;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbose = 1;
ସ୍ଥିର ଗଣନା help_format help_format = HELP_FORMAT_NONE;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା exclude_guides;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_external_commands = -1;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_aliases = -1;
ସ୍ଥିର ସଂରଚନା option builtin_help_options[] = {
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

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ builtin_help_usage[] = {
	"git help [-a|--all] [--[no-]verbose] [--[no-]external-commands] [--[no-]aliases]",
	N_("git help [[-i|--info] [-m|--man] [-w|--web]] [<command>|<doc>]"),
	"git help [-g|--guides]",
	"git help [-c|--config]",
	"git help [--user-interfaces]",
	"git help [--developer-interfaces]",
	ଖାଲି
};

ସଂରଚନା slot_expansion {
	ଅଚଳ ଅକ୍ଷର *prefix;
	ଅଚଳ ଅକ୍ଷର *placeholder;
	ଶୂନ୍ୟ (*fn)(ସଂରଚନା string_list *list, ଅଚଳ ଅକ୍ଷର *prefix);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା found;
};

ସ୍ଥିର ଶୂନ୍ୟ set_config_vars(ସଂରଚନା string_list *ଚାବିଗୁଡ଼ିକ_uniq, ସଂରଚନା string_list_item *var)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *str = var->string;
	ଅଚଳ ଅକ୍ଷର *wildcard = strchr(str, '*');
	ଅଚଳ ଅକ୍ଷର *tag = strchr(str, '<');
	ଅଚଳ ଅକ୍ଷର *cut;

	ଯଦି (wildcard && tag)
		cut = wildcard < tag ? wildcard : tag;
	ଅନ୍ୟଥା ଯଦି (wildcard)
		cut = wildcard;
	ଅନ୍ୟଥା ଯଦି (tag)
		cut = tag;
	ଅନ୍ୟଥା {
		string_list_append(ଚାବିଗୁଡ଼ିକ_uniq, str);
		ଫେରନ୍ତୁ;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(ଚାବିଗୁଡ଼ିକ_uniq, sb.buf);
	strbuf_release(&sb);
}

ସ୍ଥିର ଶୂନ୍ୟ set_config_sections(ସଂରଚନା string_list *ଚାବିଗୁଡ଼ିକ_uniq, ସଂରଚନା string_list_item *var)
{
	ସଂରଚନା strbuf sb = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *str = var->string;
	ଅଚଳ ଅକ୍ଷର *dot = strchr(str, '.');
	ଅଚଳ ଅକ୍ଷର *cut;

	ଯଦି (dot)
		cut = dot;
	ଅନ୍ୟଥା {
		set_config_vars(ଚାବିଗୁଡ଼ିକ_uniq, var);
		ଫେରନ୍ତୁ;
	}

	strbuf_add(&sb, str, cut - str);
	string_list_append(ଚାବିଗୁଡ଼ିକ_uniq, sb.buf);
	strbuf_release(&sb);
}

ସ୍ଥିର ଶୂନ୍ୟ list_config_help(ଗଣନା show_config_type type)
{
	ସଂରଚନା slot_expansion slot_expansions[] = {
		{ "advice", "*", list_config_advices },
		{ "color.branch", "<slot>", list_config_color_branch_slots },
		{ "color.decorate", "<slot>", list_config_color_decorate_slots },
		{ "color.diff", "<slot>", list_config_color_diff_slots },
		{ "color.grep", "<slot>", list_config_color_grep_slots },
		{ "color.interactive", "<slot>", list_config_color_interactive_slots },
		{ "color.remote", "<slot>", list_config_color_sideband_slots },
		{ "color.status", "<slot>", list_config_color_status_slots },
		{ "fsck", "<msg-id>", list_config_fsck_ସନ୍ଦେଶ_ids },
		{ "receive.fsck", "<msg-id>", list_config_fsck_ସନ୍ଦେଶ_ids },
		{ ଖାଲି, ଖାଲି, ଖାଲି }
	};
	ଅଚଳ ଅକ୍ଷର **p;
	ସଂରଚନା slot_expansion *e;
	ସଂରଚନା string_list ଚାବିଗୁଡ଼ିକ = STRING_LIST_INIT_DUP;
	ସଂରଚନା string_list ଚାବିଗୁଡ଼ିକ_uniq = STRING_LIST_INIT_DUP;
	ସଂରଚନା string_list_item *item;
	ସଂରଚନା strbuf sb = STRBUF_INIT;

	ପାଇଁ (p = config_name_list; *p; p++) {
		ଅଚଳ ଅକ୍ଷର *var = *p;

		ପାଇଁ (e = slot_expansions; e->prefix; e++) {
			strbuf_reset(&sb);
			strbuf_addf(&sb, "%s.%s", e->prefix, e->placeholder);
			ଯଦି (!strcasecmp(var, sb.buf)) {
				e->fn(&ଚାବିଗୁଡ଼ିକ, e->prefix);
				e->found++;
				ଭାଙ୍ଗ;
			}
		}

		ଯଦି (!e->prefix)
			string_list_append(&ଚାବିଗୁଡ଼ିକ, var);
	}

	strbuf_release(&sb);

	ପାଇଁ (e = slot_expansions; e->prefix; e++)
		ଯଦି (!e->found)
			BUG("slot_expansion %s.%s is not used",
			    e->prefix, e->placeholder);

	ପାଇଁ (size_t i = 0; i < ଚାବିଗୁଡ଼ିକ.nr; i++) {
		ବାଛ (type) {
		ସ୍ଥିତି SHOW_CONFIG_HUMAN:
			string_list_append(&ଚାବିଗୁଡ଼ିକ_uniq, ଚାବିଗୁଡ଼ିକ.ବସ୍ତୁଗୁଡ଼ିକ[i].string);
			ଭାଙ୍ଗ;
		ସ୍ଥିତି SHOW_CONFIG_SECTIONS:
			set_config_sections(&ଚାବିଗୁଡ଼ିକ_uniq, &ଚାବିଗୁଡ଼ିକ.ବସ୍ତୁଗୁଡ଼ିକ[i]);
			ଭାଙ୍ଗ;
		ସ୍ଥିତି SHOW_CONFIG_VARS:
			set_config_vars(&ଚାବିଗୁଡ଼ିକ_uniq, &ଚାବିଗୁଡ଼ିକ.ବସ୍ତୁଗୁଡ଼ିକ[i]);
			ଭାଙ୍ଗ;
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			BUG("%d: unexpected type", type);
		}
	}

	string_list_sort_u(&ଚାବିଗୁଡ଼ିକ_uniq, 0);
	for_each_string_list_item(item, &ଚାବିଗୁଡ଼ିକ_uniq)
		puts(item->string);
	string_list_clear(&ଚାବିଗୁଡ଼ିକ_uniq, 0);
	string_list_clear(&ଚାବିଗୁଡ଼ିକ, 0);
}

ସ୍ଥିର ଗଣନା help_format parse_help_format(ଅଚଳ ଅକ୍ଷର *format)
{
	ଯଦି (!strcmp(format, "man"))
		ଫେରନ୍ତୁ HELP_FORMAT_MAN;
	ଯଦି (!strcmp(format, "info"))
		ଫେରନ୍ତୁ HELP_FORMAT_INFO;
	ଯଦି (!strcmp(format, "web") || !strcmp(format, "html"))
		ଫେରନ୍ତୁ HELP_FORMAT_WEB;
	/*
	 * Please update _repo_config() in git-completion.bash when you
	 * add new help formats.
	 */
	die(_("unrecognized help format '%s'"), format);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *get_man_viewer_info(ଅଚଳ ଅକ୍ଷର *name)
{
	ସଂରଚନା man_viewer_info_list *viewer;

	ପାଇଁ (viewer = man_viewer_info_list; viewer; viewer = viewer->next)
	{
		ଯଦି (!strcasecmp(name, viewer->name))
			ଫେରନ୍ତୁ viewer->info;
	}
	ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_emacsclient_version(ଶୂନ୍ୟ)
{
	ସଂରଚନା strbuf buffer = STRBUF_INIT;
	ସଂରଚନା child_process ec_process = CHILD_PROCESS_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା version;

	/* emacsclient prints its version number on stderr */
	strvec_pushl(&ec_process.args, "emacsclient", "--version", ଖାଲି);
	ec_process.err = -1;
	ec_process.stdout_to_stderr = 1;
	ଯଦି (start_command(&ec_process))
		ଫେରନ୍ତୁ error(_("Failed to start emacsclient."));

	strbuf_read(&buffer, ec_process.err, 20);
	close(ec_process.err);

	/*
	 * Don't bother checking return value, because "emacsclient --version"
	 * seems to always exits with code 1.
	 */
	finish_command(&ec_process);

	ଯଦି (!starts_with(buffer.buf, "emacsclient")) {
		strbuf_release(&buffer);
		ଫେରନ୍ତୁ error(_("Failed to parse emacsclient version."));
	}

	strbuf_remove(&buffer, 0, strlen("emacsclient"));
	version = atoi(buffer.buf);

	ଯଦି (version < 22) {
		strbuf_release(&buffer);
		ଫେରନ୍ତୁ error(_("emacsclient version '%d' too old (< 22)."),
			version);
	}

	strbuf_release(&buffer);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ exec_woman_emacs(ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *page)
{
	ଯଦି (!check_emacsclient_version()) {
		/* This works only with emacsclient version >= 22. */
		ସଂରଚନା strbuf man_page = STRBUF_INIT;

		ଯଦି (!path)
			path = "emacsclient";
		strbuf_addf(&man_page, "(woman \"%s\")", page);
		execlp(path, "emacsclient", "-e", man_page.buf, (ଅକ୍ଷର *)ଖାଲି);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ exec_man_konqueror(ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *page)
{
	ଅଚଳ ଅକ୍ଷର *display = getenv("DISPLAY");
	ଯଦି (display && *display) {
		ସଂରଚନା strbuf man_page = STRBUF_INIT;
		ଅଚଳ ଅକ୍ଷର *filename = "kfmclient";

		/* It's simpler to launch konqueror using kfmclient. */
		ଯଦି (path) {
			size_t len;
			ଯଦି (strip_suffix(path, "/konqueror", &len))
				path = xstrfmt("%.*s/kfmclient", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)len, path);
			filename = basename((ଅକ୍ଷର *)path);
		} ଅନ୍ୟଥା
			path = "kfmclient";
		strbuf_addf(&man_page, "man:%s(1)", page);
		execlp(path, filename, "newTab", man_page.buf, (ଅକ୍ଷର *)ଖାଲି);
		warning_errno(_("failed to exec '%s'"), path);
		strbuf_release(&man_page);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ exec_man_man(ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *page)
{
	ଯଦି (!path)
		path = "man";
	execlp(path, "man", page, (ଅକ୍ଷର *)ଖାଲି);
	warning_errno(_("failed to exec '%s'"), path);
}

ସ୍ଥିର ଶୂନ୍ୟ exec_man_cmd(ଅଚଳ ଅକ୍ଷର *cmd, ଅଚଳ ଅକ୍ଷର *page)
{
	ସଂରଚନା strbuf shell_cmd = STRBUF_INIT;
	strbuf_addf(&shell_cmd, "%s %s", cmd, page);
	execl(SHELL_PATH, SHELL_PATH, "-c", shell_cmd.buf, (ଅକ୍ଷର *)ଖାଲି);
	warning(_("failed to exec '%s'"), cmd);
	strbuf_release(&shell_cmd);
}

ସ୍ଥିର ଶୂନ୍ୟ add_man_viewer(ଅଚଳ ଅକ୍ଷର *name)
{
	ସଂରଚନା man_viewer_list **p = &man_viewer_list;

	ଯେତେବେଳେ (*p)
		p = &((*p)->next);
	FLEX_ALLOC_STR(*p, name, name);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା supported_man_viewer(ଅଚଳ ଅକ୍ଷର *name, size_t len)
{
	ଫେରନ୍ତୁ (!strncasecmp("man", name, len) ||
		!strncasecmp("woman", name, len) ||
		!strncasecmp("konqueror", name, len));
}

ସ୍ଥିର ଶୂନ୍ୟ do_add_man_viewer_info(ଅଚଳ ଅକ୍ଷର *name,
				   size_t len,
				   ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ)
{
	ସଂରଚନା man_viewer_info_list *new_man_viewer;
	FLEX_ALLOC_MEM(new_man_viewer, name, name, len);
	new_man_viewer->info = xstrdup(ମୂଲ୍ୟ);
	new_man_viewer->next = man_viewer_info_list;
	man_viewer_info_list = new_man_viewer;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_man_viewer_path(ଅଚଳ ଅକ୍ଷର *name,
			       size_t len,
			       ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ)
{
	ଯଦି (supported_man_viewer(name, len))
		do_add_man_viewer_info(name, len, ମୂଲ୍ୟ);
	ଅନ୍ୟଥା
		warning(_("'%s': path for unsupported man viewer.\n"
			  "Please consider using 'man.<tool>.cmd' instead."),
			name);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_man_viewer_cmd(ଅଚଳ ଅକ୍ଷର *name,
			      size_t len,
			      ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ)
{
	ଯଦି (supported_man_viewer(name, len))
		warning(_("'%s': cmd for supported man viewer.\n"
			  "Please consider using 'man.<tool>.path' instead."),
			name);
	ଅନ୍ୟଥା
		do_add_man_viewer_info(name, len, ମୂଲ୍ୟ);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_man_viewer_info(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ)
{
	ଅଚଳ ଅକ୍ଷର *name, *subkey;
	size_t namelen;

	ଯଦି (parse_config_ଚାବି(var, "man", &name, &namelen, &subkey) < 0 || !name)
		ଫେରନ୍ତୁ 0;

	ଯଦି (!strcmp(subkey, "path")) {
		ଯଦି (!ମୂଲ୍ୟ)
			ଫେରନ୍ତୁ config_error_nonbool(var);
		ଫେରନ୍ତୁ add_man_viewer_path(name, namelen, ମୂଲ୍ୟ);
	}
	ଯଦି (!strcmp(subkey, "cmd")) {
		ଯଦି (!ମୂଲ୍ୟ)
			ଫେରନ୍ତୁ config_error_nonbool(var);
		ଫେରନ୍ତୁ add_man_viewer_cmd(name, namelen, ମୂଲ୍ୟ);
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_help_config(ଅଚଳ ଅକ୍ଷର *var, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
			   ଅଚଳ ସଂରଚନା config_context *ctx, ଶୂନ୍ୟ *cb)
{
	ଯଦି (!strcmp(var, "help.format")) {
		ଯଦି (!ମୂଲ୍ୟ)
			ଫେରନ୍ତୁ config_error_nonbool(var);
		help_format = parse_help_format(ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "help.htmlpath")) {
		ଯଦି (!ମୂଲ୍ୟ)
			ଫେରନ୍ତୁ config_error_nonbool(var);
		ମୁକ୍ତ(html_path);
		html_path = xstrdup(ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!strcmp(var, "man.viewer")) {
		ଯଦି (!ମୂଲ୍ୟ)
			ଫେରନ୍ତୁ config_error_nonbool(var);
		add_man_viewer(ମୂଲ୍ୟ);
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (starts_with(var, "man."))
		ଫେରନ୍ତୁ add_man_viewer_info(var, ମୂଲ୍ୟ);

	ଫେରନ୍ତୁ git_default_config(var, ମୂଲ୍ୟ, ctx, cb);
}

ସ୍ଥିର ସଂରଚନା cmdnames main_cmds, other_cmds;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_git_command(ଅଚଳ ଅକ୍ଷର *s)
{
	ଯଦି (is_builtin(s))
		ଫେରନ୍ତୁ 1;

	load_command_list("git-", &main_cmds, &other_cmds);
	ଫେରନ୍ତୁ is_in_cmdlist(&main_cmds, s) ||
		is_in_cmdlist(&other_cmds, s);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *cmd_to_page(ଅଚଳ ଅକ୍ଷର *git_cmd)
{
	ଯଦି (!git_cmd)
		ଫେରନ୍ତୁ "git";
	ଅନ୍ୟଥା ଯଦି (starts_with(git_cmd, "git"))
		ଫେରନ୍ତୁ git_cmd;
	ଅନ୍ୟଥା ଯଦି (is_git_command(git_cmd))
		ଫେରନ୍ତୁ xstrfmt("git-%s", git_cmd);
	ଅନ୍ୟଥା ଯଦି (!strcmp("scalar", git_cmd))
		ଫେରନ୍ତୁ xstrdup(git_cmd);
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ xstrfmt("git%s", git_cmd);
}

ସ୍ଥିର ଶୂନ୍ୟ setup_man_path(ଶୂନ୍ୟ)
{
	ସଂରଚନା strbuf new_path = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *old_path = getenv("MANPATH");
	ଅକ୍ଷର *git_man_path = system_path(GIT_MAN_PATH);

	/* We should always put ':' after our path. If there is no
	 * old_path, the ':' at the end will let 'man' to try
	 * system-wide paths after ours to find the manual page. If
	 * there is old_path, we need ':' as delimiter. */
	strbuf_addstr(&new_path, git_man_path);
	strbuf_addch(&new_path, ':');
	ଯଦି (old_path)
		strbuf_addstr(&new_path, old_path);

	ମୁକ୍ତ(git_man_path);
	setenv("MANPATH", new_path.buf, 1);

	strbuf_release(&new_path);
}

ସ୍ଥିର ଶୂନ୍ୟ exec_viewer(ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *page)
{
	ଅଚଳ ଅକ୍ଷର *info = get_man_viewer_info(name);

	ଯଦି (!strcasecmp(name, "man"))
		exec_man_man(info, page);
	ଅନ୍ୟଥା ଯଦି (!strcasecmp(name, "woman"))
		exec_woman_emacs(info, page);
	ଅନ୍ୟଥା ଯଦି (!strcasecmp(name, "konqueror"))
		exec_man_konqueror(info, page);
	ଅନ୍ୟଥା ଯଦି (info)
		exec_man_cmd(info, page);
	ଅନ୍ୟଥା
		warning(_("'%s': unknown man viewer."), name);
}

ସ୍ଥିର ଶୂନ୍ୟ show_man_page(ଅଚଳ ଅକ୍ଷର *page)
{
	ସଂରଚନା man_viewer_list *viewer;
	ଅଚଳ ଅକ୍ଷର *fallback = getenv("GIT_MAN_VIEWER");

	setup_man_path();
	ପାଇଁ (viewer = man_viewer_list; viewer; viewer = viewer->next)
	{
		exec_viewer(viewer->name, page); /* will return when unable */
	}
	ଯଦି (fallback)
		exec_viewer(fallback, page);
	exec_viewer("man", page);
	die(_("no man viewer handled the request"));
}

ସ୍ଥିର ଶୂନ୍ୟ show_info_page(ଅଚଳ ଅକ୍ଷର *page)
{
	setenv("INFOPATH", system_path(GIT_INFO_PATH), 1);
	execlp("info", "info", "gitman", page, (ଅକ୍ଷର *)ଖାଲି);
	die(_("no info viewer handled the request"));
}

ସ୍ଥିର ଶୂନ୍ୟ get_html_page_path(ସଂରଚନା strbuf *page_path, ଅଚଳ ଅକ୍ଷର *page)
{
	ସଂରଚନା stat st;
	ଅଚଳ ଅକ୍ଷର *path = html_path;
	ଅକ୍ଷର *to_free = ଖାଲି;

	ଯଦି (!path)
		path = to_free = system_path(GIT_HTML_PATH);

	/*
	 * Check that the page we're looking for exists.
	 */
	ଯଦି (!strstr(path, "://")) {
		ଯଦି (stat(mkpath("%s/%s.html", path, page), &st)
		    || !S_ISREG(st.st_mode))
			die("'%s/%s.html': documentation file not found.",
				path, page);
	}

	strbuf_init(page_path, 0);
	strbuf_addf(page_path, "%s/%s.html", path, page);
	ମୁକ୍ତ(to_free);
}

ସ୍ଥିର ଶୂନ୍ୟ open_html(ଅଚଳ ଅକ୍ଷର *path)
{
	execl_git_cmd("web--browse", "-c", "help.browser", path, (ଅକ୍ଷର *)ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ show_html_page(ଅଚଳ ଅକ୍ଷର *page)
{
	ସଂରଚନା strbuf page_path; /* it leaks but we exec below */

	get_html_page_path(&page_path, page);

	open_html(page_path.buf);
}

ସ୍ଥିର ଅକ୍ଷର *check_git_cmd(ଅଚଳ ଅକ୍ଷର *cmd)
{
	ଅକ୍ଷର *alias;

	ଯଦି (is_git_command(cmd))
		ଫେରନ୍ତୁ xstrdup(cmd);

	alias = alias_lookup(cmd);
	ଯଦି (alias) {
		ଅଚଳ ଅକ୍ଷର **argv;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2;

		/*
		 * handle_builtin() in git.c rewrites "git cmd --help"
		 * to "git help --exclude-guides cmd", so we can use
		 * exclude_guides to distinguish "git cmd --help" from
		 * "git help cmd". In the latter case, or if cmd is an
		 * alias for a shell command, just print the alias
		 * definition.
		 */
		ଯଦି (!exclude_guides || alias[0] == '!') {
			printf_ln(_("'%s' is aliased to '%s'"), cmd, alias);
			ମୁକ୍ତ(alias);
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
		ଗଣନା_2 = split_cmdline(alias, &argv);
		ଯଦି (ଗଣନା_2 < 0)
			die(_("bad alias.%s string: %s"), cmd,
			    split_cmdline_strerror(ଗଣନା_2));
		ମୁକ୍ତ(argv);
		ଫେରନ୍ତୁ alias;
	}

	ଯଦି (exclude_guides)
		ଫେରନ୍ତୁ help_unknown_cmd(cmd);

	ଫେରନ୍ତୁ xstrdup(cmd);
}

ସ୍ଥିର ଶୂନ୍ୟ no_help_format(ଅଚଳ ଅକ୍ଷର *opt_mode, ଗଣନା help_format fmt)
{
	ଅଚଳ ଅକ୍ଷର *opt_fmt;

	ବାଛ (fmt) {
	ସ୍ଥିତି HELP_FORMAT_NONE:
		ଫେରନ୍ତୁ;
	ସ୍ଥିତି HELP_FORMAT_MAN:
		opt_fmt = "--man";
		ଭାଙ୍ଗ;
	ସ୍ଥିତି HELP_FORMAT_INFO:
		opt_fmt = "--info";
		ଭାଙ୍ଗ;
	ସ୍ଥିତି HELP_FORMAT_WEB:
		opt_fmt = "--web";
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("unreachable");
	}

	usage_ସନ୍ଦେଶ_optf(_("options '%s' and '%s' cannot be used together"),
		       builtin_help_usage, builtin_help_options, opt_mode,
		       opt_fmt);
}

ସ୍ଥିର ଶୂନ୍ୟ opt_mode_usage(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର *opt_mode,
			   ଗଣନା help_format fmt)
{
	ଯଦି (argc)
		usage_ସନ୍ଦେଶ_optf(_("the '%s' option doesn't take any non-option arguments"),
			       builtin_help_usage, builtin_help_options,
			       opt_mode);

	no_help_format(opt_mode, fmt);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_help(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	     ଅଚଳ ଅକ୍ଷର **argv,
	     ଅଚଳ ଅକ୍ଷର *prefix,
	     ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nongit;
	ଗଣନା help_format parsed_help_format;
	ଅକ୍ଷର *command = ଖାଲି;
	ଅଚଳ ଅକ୍ଷର *page;

	argc = parse_options(argc, argv, prefix, builtin_help_options,
			builtin_help_usage, 0);
	parsed_help_format = help_format;

	ଯଦି (cmd_mode != HELP_ACTION_ALL &&
	    (show_external_commands >= 0 ||
	     show_aliases >= 0))
		usage_ସନ୍ଦେଶ_opt(_("the '--no-[external-commands|aliases]' options can only be used with '--all'"),
			      builtin_help_usage, builtin_help_options);

	ବାଛ (cmd_mode) {
	ସ୍ଥିତି HELP_ACTION_ALL:
		opt_mode_usage(argc, "--all", help_format);
		ଯଦି (verbose) {
			setup_pager(the_repository);
			list_all_cmds_help(show_external_commands,
					   show_aliases);
			ଫେରନ୍ତୁ 0;
		}
		printf(_("usage: %s%s"), _(git_usage_string), "\n\n");
		load_command_list("git-", &main_cmds, &other_cmds);
		list_commands(&main_cmds, &other_cmds);
		printf("%s\n", _(git_more_info_string));
		ଭାଙ୍ଗ;
	ସ୍ଥିତି HELP_ACTION_GUIDES:
		opt_mode_usage(argc, "--guides", help_format);
		list_guides_help();
		printf("%s\n", _(git_more_info_string));
		ଫେରନ୍ତୁ 0;
	ସ୍ଥିତି HELP_ACTION_CONFIG_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-for-completion", help_format);
		list_config_help(SHOW_CONFIG_VARS);
		ଫେରନ୍ତୁ 0;
	ସ୍ଥିତି HELP_ACTION_USER_INTERFACES:
		opt_mode_usage(argc, "--user-interfaces", help_format);
		list_user_interfaces_help();
		ଫେରନ୍ତୁ 0;
	ସ୍ଥିତି HELP_ACTION_DEVELOPER_INTERFACES:
		opt_mode_usage(argc, "--developer-interfaces", help_format);
		list_developer_interfaces_help();
		ଫେରନ୍ତୁ 0;
	ସ୍ଥିତି HELP_ACTION_CONFIG_SECTIONS_FOR_COMPLETION:
		opt_mode_usage(argc, "--config-sections-for-completion",
			       help_format);
		list_config_help(SHOW_CONFIG_SECTIONS);
		ଫେରନ୍ତୁ 0;
	ସ୍ଥିତି HELP_ACTION_ALIASES_FOR_COMPLETION: {
		ସଂରଚନା string_list alias_list = STRING_LIST_INIT_DUP;
		opt_mode_usage(argc, "--aliases-for-completion", help_format);
		list_aliases(&alias_list);
		ପାଇଁ (size_t i = 0; i < alias_list.nr; i++)
			printf("%s%c%s%c", alias_list.ବସ୍ତୁଗୁଡ଼ିକ[i].string, '\n',
			       (ଅକ୍ଷର *)alias_list.ବସ୍ତୁଗୁଡ଼ିକ[i].util, '\0');
		string_list_clear(&alias_list, 1);
		ଫେରନ୍ତୁ 0;
	}
	ସ୍ଥିତି HELP_ACTION_CONFIG:
		opt_mode_usage(argc, "--config", help_format);
		setup_pager(the_repository);
		list_config_help(SHOW_CONFIG_HUMAN);
		printf("\n%s\n", _("'git help config' for more information"));
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!argv[0]) {
		printf(_("usage: %s%s"), _(git_usage_string), "\n\n");
		list_common_cmds_help();
		printf("\n%s\n", _(git_more_info_string));
		ଫେରନ୍ତୁ 0;
	}

	setup_git_directory_gently(the_repository, &nongit);
	repo_config(the_repository, git_help_config, ଖାଲି);

	ଯଦି (parsed_help_format != HELP_FORMAT_NONE)
		help_format = parsed_help_format;
	ଯଦି (help_format == HELP_FORMAT_NONE)
		help_format = parse_help_format(DEFAULT_HELP_FORMAT);

	command = check_git_cmd(argv[0]);

	page = cmd_to_page(command);
	ବାଛ (help_format) {
	ସ୍ଥିତି HELP_FORMAT_NONE:
	ସ୍ଥିତି HELP_FORMAT_MAN:
		show_man_page(page);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି HELP_FORMAT_INFO:
		show_info_page(page);
		ଭାଙ୍ଗ;
	ସ୍ଥିତି HELP_FORMAT_WEB:
		show_html_page(page);
		ଭାଙ୍ଗ;
	}

	ମୁକ୍ତ(command);
	ଫେରନ୍ତୁ 0;
}
