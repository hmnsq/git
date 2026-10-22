#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "abspath.h"
#અન્તર્ભાવ "editor.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "strbuf.h"
#અન્તર્ભાવ "help.h"
#અન્તર્ભાવ "compat/compiler.h"
#અન્તર્ભાવ "hook.h"
#અન્તર્ભાવ "hook-list.h"
#અન્તર્ભાવ "diagnose.h"
#અન્તર્ભાવ "setup.h"
#અન્તર્ભાવ "version.h"

સ્થિર શૂન્ય get_system_info(સંરચના strbuf *sys_info)
{
	અક્ષર *shell = ખાલી;

	/* get git version from native cmd */
	strbuf_addstr(sys_info, _("git version:\n"));
	get_version_info(sys_info, 1);

	/* system call for other version info */
	strbuf_addstr(sys_info, "uname: ");
	get_uname_info(sys_info, 1);

	strbuf_addstr(sys_info, _("compiler info: "));
	get_compiler_info(sys_info);

	strbuf_addstr(sys_info, _("libc info: "));
	get_libc_info(sys_info);

	shell = getenv("SHELL");
	strbuf_addf(sys_info, "$SHELL (typically, interactive shell): %s\n",
		    shell ? shell : "<unset>");
}

સ્થિર શૂન્ય get_populated_hooks(સંરચના strbuf *hook_info, પૂર્ણાંક nongit)
{
	અચલ અક્ષર **p;

	જો (nongit) {
		strbuf_addstr(hook_info,
			_("not run from a git repository - no hooks to show\n"));
		પાછા;
	}

	માટે (p = hook_name_list; *p; p++) {
		અચલ અક્ષર *hook = *p;

		જો (hook_exists(the_repository, hook))
			strbuf_addf(hook_info, "%s\n", hook);
	}
}

સ્થિર અચલ અક્ષર * અચલ bugreport_usage[] = {
	N_("git bugreport [(-o | --output-directory) <path>]\n"
	   "              [(-s | --suffix) <format> | --no-suffix]\n"
	   "              [--diagnose[=<mode>]]"),
	ખાલી
};

સ્થિર પૂર્ણાંક get_bug_template(સંરચના strbuf *template)
{
	અચલ અક્ષર template_text[] = N_(
"Thank you for filling out a Git bug report!\n"
"Please answer the following questions to help us understand your issue.\n"
"\n"
"What did you do before the bug happened? (Steps to reproduce your issue)\n"
"\n"
"What did you expect to happen? (Expected behavior)\n"
"\n"
"What happened instead? (Actual behavior)\n"
"\n"
"What's different between what you expected and what actually happened?\n"
"\n"
"Anything else you want to add:\n"
"\n"
"Please review the rest of the bug report below.\n"
"You can delete any lines you don't wish to share.\n");

	strbuf_addstr(template, _(template_text));
	પાછા 0;
}

સ્થિર શૂન્ય get_header(સંરચના strbuf *buf, અચલ અક્ષર *title)
{
	strbuf_addf(buf, "\n\n[%s]\n", title);
}

પૂર્ણાંક cmd_bugreport(પૂર્ણાંક argc,
		  અચલ અક્ષર **argv,
		  અચલ અક્ષર *prefix,
		  સંરચના repository *repo UNUSED)
{
	સંરચના strbuf buffer = STRBUF_INIT;
	સંરચના strbuf report_path = STRBUF_INIT;
	પૂર્ણાંક report = -1;
	time_t now = time(ખાલી);
	સંરચના tm tm;
	ગણના diagnose_mode diagnose = DIAGNOSE_NONE;
	અક્ષર *option_output = ખાલી;
	અચલ અક્ષર *option_suffix = "%Y-%m-%d-%H%M";
	અચલ અક્ષર *user_relative_path = ખાલી;
	અક્ષર *prefixed_filename;
	size_t output_path_len;
	પૂર્ણાંક ret;

	અચલ સંરચના option bugreport_options[] = {
		OPT_CALLBACK_F(0, "diagnose", &diagnose, N_("mode"),
			       N_("create an additional zip archive of detailed diagnostics (default 'stats')"),
			       PARSE_OPT_OPTARG, option_parse_diagnose),
		OPT_STRING('o', "output-directory", &option_output, N_("path"),
			   N_("specify a destination for the bugreport file(s)")),
		OPT_STRING('s', "suffix", &option_suffix, N_("format"),
			   N_("specify a strftime format suffix for the filename(s)")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, bugreport_options,
			     bugreport_usage, 0);

	જો (argc) {
		error(_("unknown argument `%s'"), argv[0]);
		usage(bugreport_usage[0]);
	}

	/* Prepare the path to put the result */
	prefixed_filename = prefix_filename(prefix,
					    option_output ? option_output : "");
	strbuf_addstr(&report_path, prefixed_filename);
	strbuf_complete(&report_path, '/');
	output_path_len = report_path.len;

	strbuf_addstr(&report_path, "git-bugreport");
	જો (option_suffix) {
		strbuf_addch(&report_path, '-');
		strbuf_addftime(&report_path, option_suffix, localtime_r(&now, &tm), 0, 0);
	}
	strbuf_addstr(&report_path, ".txt");

	પસંદગી (safe_create_leading_directories(the_repository, report_path.buf)) {
	સ્થિતિ SCLD_OK:
	સ્થિતિ SCLD_EXISTS:
		તોડો;
	મૂળભૂત:
		die(_("could not create leading directories for '%s'"),
		    report_path.buf);
	}

	/* Prepare diagnostics, if requested */
	જો (diagnose != DIAGNOSE_NONE) {
		સંરચના strbuf zip_path = STRBUF_INIT;
		strbuf_add(&zip_path, report_path.buf, output_path_len);
		strbuf_addstr(&zip_path, "git-diagnostics-");
		strbuf_addftime(&zip_path, option_suffix, localtime_r(&now, &tm), 0, 0);
		strbuf_addstr(&zip_path, ".zip");

		જો (create_diagnostics_archive(the_repository, &zip_path, diagnose))
			die_errno(_("unable to create diagnostics archive %s"), zip_path.buf);

		strbuf_release(&zip_path);
	}

	/* Prepare the report contents */
	get_bug_template(&buffer);

	get_header(&buffer, _("System Info"));
	get_system_info(&buffer);

	get_header(&buffer, _("Enabled Hooks"));
	get_populated_hooks(&buffer, !startup_info->have_repository);

	/* fopen doesn't offer us an O_EXCL alternative, except with glibc. */
	report = xopen(report_path.buf, O_CREAT | O_EXCL | O_WRONLY, 0666);

	જો (write_in_full(report, buffer.buf, buffer.len) < 0)
		die_errno(_("unable to write to %s"), report_path.buf);

	close(report);

	/*
	 * We want to print the path relative to the user, but we still need the
	 * path relative to us to give to the editor.
	 */
	જો (!(prefix && skip_prefix(report_path.buf, prefix, &user_relative_path)))
		user_relative_path = report_path.buf;
	fprintf(stderr, _("Created new report at '%s'.\n"),
		user_relative_path);

	મુક્ત(prefixed_filename);
	strbuf_release(&buffer);

	ret = !!launch_editor(report_path.buf, ખાલી, ખાલી);
	strbuf_release(&report_path);
	પાછા ret;
}
