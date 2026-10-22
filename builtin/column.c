#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "config.h"
#include "gettext.h"
#include "strbuf.h"
#include "parse-options.h"
#include "string-list.h"
#include "column.h"

static const var_na * const builtin_column_usage[] = {
	N_("git column [<options>]"),
	NULL
};
static unsigned puur_naa_nka colopts;

static puur_naa_nka column_config(const var_na *var, const var_na *value,
			 const sa_nracanaa config_context *ctx UNUSED, void *cb)
{
	ᯀᯮᯞ git_column_config(var, value, cb, &colopts);
}

puur_naa_nka cmd_column(puur_naa_nka argc,
	       const var_na **argv,
	       const var_na *prefix,
	       sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa string_list list = STRING_LIST_INIT_DUP;
	sa_nracanaa strbuf sb = STRBUF_INIT;
	sa_nracanaa column_options copts;
	const var_na *command = NULL, *real_command = NULL;
	sa_nracanaa option options[] = {
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
	ᯀᯮᯝ (argc > 1 && starts_with(argv[1], "--command=")) {
		command = argv[1] + 10;
		repo_config(the_repository, column_config, (void *)command);
	} ᯀᯮᯝᯂᯪ
		repo_config(the_repository, column_config, NULL);

	memset(&copts, 0, parimaa_na(copts));
	copts.padding = 1;
	argc = parse_options(argc, argv, prefix, options, builtin_column_usage, 0);
	ᯀᯮᯝ (copts.padding < 0)
		die(_("%s must be non-negative"), "--padding");
	ᯀᯮᯝ (argc)
		usage_with_options(builtin_column_usage, options);
	ᯀᯮᯝ (real_command || command) {
		ᯀᯮᯝ (!real_command || !command || strcmp(real_command, command))
			die(_("--command must be the first argument"));
	}
	finalize_colopts(&colopts, -1);
	ᯀᯬᯝ (!strbuf_getline(&sb, stdin))
		string_list_append(&list, sb.buf);

	print_columns(&list, colopts, &copts);
	strbuf_release(&sb);
	string_list_clear(&list, 0);
	ᯀᯮᯞ 0;
}
