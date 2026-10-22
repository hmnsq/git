#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "abspath.h"
#അന്തര്ഭാവ "editor.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "strbuf.h"
#അന്തര്ഭാവ "help.h"
#അന്തര്ഭാവ "compat/compiler.h"
#അന്തര്ഭാവ "hook.h"
#അന്തര്ഭാവ "hook-list.h"
#അന്തര്ഭാവ "diagnose.h"
#അന്തര്ഭാവ "setup.h"
#അന്തര്ഭാവ "version.h"

സ്ഥിതം ശൂന്യം get_system_info(ഘടന strbuf *sys_info)
{
	അക്ഷരം *shell = ശൂന്യ;

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

സ്ഥിതം ശൂന്യം get_populated_hooks(ഘടന strbuf *hook_info, പൂർണ്ണസംഖ്യ nongit)
{
	സ്ഥിരം അക്ഷരം **p;

	എങ്കിൽ (nongit) {
		strbuf_addstr(hook_info,
			_("not run from a git repository - no hooks to show\n"));
		തിരികെ;
	}

	വേണ്ടി (p = hook_name_list; *p; p++) {
		സ്ഥിരം അക്ഷരം *hook = *p;

		എങ്കിൽ (hook_exists(the_repository, hook))
			strbuf_addf(hook_info, "%s\n", hook);
	}
}

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം bugreport_usage[] = {
	N_("git bugreport [(-o | --output-directory) <path>]\n"
	   "              [(-s | --suffix) <format> | --no-suffix]\n"
	   "              [--diagnose[=<mode>]]"),
	ശൂന്യ
};

സ്ഥിതം പൂർണ്ണസംഖ്യ get_bug_template(ഘടന strbuf *template)
{
	സ്ഥിരം അക്ഷരം template_text[] = N_(
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
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം get_header(ഘടന strbuf *buf, സ്ഥിരം അക്ഷരം *title)
{
	strbuf_addf(buf, "\n\n[%s]\n", title);
}

പൂർണ്ണസംഖ്യ cmd_bugreport(പൂർണ്ണസംഖ്യ argc,
		  സ്ഥിരം അക്ഷരം **argv,
		  സ്ഥിരം അക്ഷരം *prefix,
		  ഘടന repository *repo UNUSED)
{
	ഘടന strbuf buffer = STRBUF_INIT;
	ഘടന strbuf report_path = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ report = -1;
	time_t now = time(ശൂന്യ);
	ഘടന tm tm;
	എണ്ണൽ diagnose_mode diagnose = DIAGNOSE_NONE;
	അക്ഷരം *option_output = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *option_suffix = "%Y-%m-%d-%H%M";
	സ്ഥിരം അക്ഷരം *user_relative_path = ശൂന്യ;
	അക്ഷരം *prefixed_filename;
	size_t output_path_len;
	പൂർണ്ണസംഖ്യ ret;

	സ്ഥിരം ഘടന option bugreport_options[] = {
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

	എങ്കിൽ (argc) {
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
	എങ്കിൽ (option_suffix) {
		strbuf_addch(&report_path, '-');
		strbuf_addftime(&report_path, option_suffix, localtime_r(&now, &tm), 0, 0);
	}
	strbuf_addstr(&report_path, ".txt");

	തിരഞ്ഞെടുപ്പ് (safe_create_leading_directories(the_repository, report_path.buf)) {
	അവസ്ഥ SCLD_OK:
	അവസ്ഥ SCLD_EXISTS:
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		die(_("could not create leading directories for '%s'"),
		    report_path.buf);
	}

	/* Prepare diagnostics, if requested */
	എങ്കിൽ (diagnose != DIAGNOSE_NONE) {
		ഘടന strbuf zip_path = STRBUF_INIT;
		strbuf_add(&zip_path, report_path.buf, output_path_len);
		strbuf_addstr(&zip_path, "git-diagnostics-");
		strbuf_addftime(&zip_path, option_suffix, localtime_r(&now, &tm), 0, 0);
		strbuf_addstr(&zip_path, ".zip");

		എങ്കിൽ (create_diagnostics_archive(the_repository, &zip_path, diagnose))
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

	എങ്കിൽ (write_in_full(report, buffer.buf, buffer.len) < 0)
		die_errno(_("unable to write to %s"), report_path.buf);

	close(report);

	/*
	 * We want to print the path relative to the user, but we still need the
	 * path relative to us to give to the editor.
	 */
	എങ്കിൽ (!(prefix && skip_prefix(report_path.buf, prefix, &user_relative_path)))
		user_relative_path = report_path.buf;
	fprintf(stderr, _("Created new report at '%s'.\n"),
		user_relative_path);

	സ്വതന്ത്രം(prefixed_filename);
	strbuf_release(&buffer);

	ret = !!launch_editor(report_path.buf, ശൂന്യ, ശൂന്യ);
	strbuf_release(&report_path);
	തിരികെ ret;
}
