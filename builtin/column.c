#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "config.h"
#include "gettext.h"
#include "strbuf.h"
#include "parse-options.h"
#include "string-list.h"
#include "column.h"

static const char * const builtin_column_usage[] = {
	N_("git column [<options>]"),
	NULL
};
static unsigned цео colopts;

static цео column_config(const char *var, const char *value,
			 const struct config_context *ctx UNUSED, void *cb)
{
	врати git_column_config(var, value, cb, &colopts);
}

цео cmd_column(цео argc,
	       const char **argv,
	       const char *prefix,
	       struct repository *repo UNUSED)
{
	struct string_list list = STRING_LIST_INIT_DUP;
	struct strbuf sb = STRBUF_INIT;
	struct column_options copts;
	const char *command = NULL, *real_command = NULL;
	struct option options[] = {
		OPT_STRING(0, "command", &real_command, N_("name"), N_("lookup config vars")),
		OPT_COLUMN(0, "mode", &colopts, N_("layout to use")),
		OPT_UNSIGNED(0, "raw-mode", &colopts, N_("layout to use")),
		OPT_INTEGER(0, "width", &copts.width, N_("maximum width")),
		OPT_STRING(0, "indent", &copts.indent, N_("string"), N_("padding space on left border")),
		OPT_STRING(0, "nl", &copts.nl, N_("string"), N_("padding space on right border")),
		OPT_INTEGER(0, "padding", &copts.padding, N_("padding space between columns")),
		OPT_END()
	};

	/* This one is special and must be the first one */
	ако (argc > 1 && starts_with(argv[1], "--command=")) {
		command = argv[1] + 10;
		repo_config(the_repository, column_config, (void *)command);
	} инако
		repo_config(the_repository, column_config, NULL);

	memset(&copts, 0, sizeof(copts));
	copts.padding = 1;
	argc = parse_options(argc, argv, prefix, options, builtin_column_usage, 0);
	ако (copts.padding < 0)
		die(_("%s must be non-negative"), "--padding");
	ако (argc)
		usage_with_options(builtin_column_usage, options);
	ако (real_command || command) {
		ако (!real_command || !command || strcmp(real_command, command))
			die(_("--command must be the first argument"));
	}
	finalize_colopts(&colopts, -1);
	док (!strbuf_getline(&sb, stdin))
		string_list_append(&list, sb.buf);

	print_columns(&list, colopts, &copts);
	strbuf_release(&sb);
	string_list_clear(&list, 0);
	врати 0;
}
