#define USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "abspath.h"
#अन्तर्भाव "editor.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "help.h"
#अन्तर्भाव "compat/compiler.h"
#अन्तर्भाव "hook.h"
#अन्तर्भाव "hook-list.h"
#अन्तर्भाव "diagnose.h"
#अन्तर्भाव "setup.h"
#अन्तर्भाव "version.h"

स्थिर शून्य get_system_info(संरचना strbuf *sys_info)
{
	वर्ण *shell = रिक्त;

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

स्थिर शून्य get_populated_hooks(संरचना strbuf *hook_info, पूर्णाङ्क nongit)
{
	ध्रुव वर्ण **p;

	यदि (nongit) {
		strbuf_addstr(hook_info,
			_("not run from a git repository - no hooks to show\n"));
		प्रत्यागम;
	}

	प्रति (p = hook_name_list; *p; p++) {
		ध्रुव वर्ण *hook = *p;

		यदि (hook_exists(the_repository, hook))
			strbuf_addf(hook_info, "%s\n", hook);
	}
}

स्थिर ध्रुव वर्ण * ध्रुव bugreport_usage[] = {
	N_("git bugreport [(-o | --output-directory) <path>]\n"
	   "              [(-s | --suffix) <format> | --no-suffix]\n"
	   "              [--diagnose[=<mode>]]"),
	रिक्त
};

स्थिर पूर्णाङ्क get_bug_template(संरचना strbuf *template)
{
	ध्रुव वर्ण template_text[] = N_(
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
	प्रत्यागम 0;
}

स्थिर शून्य get_header(संरचना strbuf *buf, ध्रुव वर्ण *title)
{
	strbuf_addf(buf, "\n\n[%s]\n", title);
}

पूर्णाङ्क cmd_bugreport(पूर्णाङ्क argc,
		  ध्रुव वर्ण **argv,
		  ध्रुव वर्ण *prefix,
		  संरचना repository *repo UNUSED)
{
	संरचना strbuf buffer = STRBUF_INIT;
	संरचना strbuf report_path = STRBUF_INIT;
	पूर्णाङ्क report = -1;
	time_t now = time(रिक्त);
	संरचना tm tm;
	गणना diagnose_mode diagnose = DIAGNOSE_NONE;
	वर्ण *option_output = रिक्त;
	ध्रुव वर्ण *option_suffix = "%Y-%m-%d-%H%M";
	ध्रुव वर्ण *user_relative_path = रिक्त;
	वर्ण *prefixed_filename;
	size_t output_path_len;
	पूर्णाङ्क ret;

	ध्रुव संरचना option bugreport_options[] = {
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

	यदि (argc) {
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
	यदि (option_suffix) {
		strbuf_addch(&report_path, '-');
		strbuf_addftime(&report_path, option_suffix, localtime_r(&now, &tm), 0, 0);
	}
	strbuf_addstr(&report_path, ".txt");

	विकल्प (safe_create_leading_directories(the_repository, report_path.buf)) {
	पक्ष SCLD_OK:
	पक्ष SCLD_EXISTS:
		भङ्ग;
	स्वभाव:
		die(_("could not create leading directories for '%s'"),
		    report_path.buf);
	}

	/* Prepare diagnostics, if requested */
	यदि (diagnose != DIAGNOSE_NONE) {
		संरचना strbuf zip_path = STRBUF_INIT;
		strbuf_add(&zip_path, report_path.buf, output_path_len);
		strbuf_addstr(&zip_path, "git-diagnostics-");
		strbuf_addftime(&zip_path, option_suffix, localtime_r(&now, &tm), 0, 0);
		strbuf_addstr(&zip_path, ".zip");

		यदि (create_diagnostics_archive(the_repository, &zip_path, diagnose))
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

	यदि (write_in_full(report, buffer.buf, buffer.len) < 0)
		die_errno(_("unable to write to %s"), report_path.buf);

	close(report);

	/*
	 * We want to print the path relative to the user, but we still need the
	 * path relative to us to give to the editor.
	 */
	यदि (!(prefix && skip_prefix(report_path.buf, prefix, &user_relative_path)))
		user_relative_path = report_path.buf;
	fprintf(stderr, _("Created new report at '%s'.\n"),
		user_relative_path);

	मुक्त(prefixed_filename);
	strbuf_release(&buffer);

	ret = !!launch_editor(report_path.buf, रिक्त, रिक्त);
	strbuf_release(&report_path);
	प्रत्यागम ret;
}
