#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "abspath.h"
#ಅನ್ತರ್ಭಾವ "editor.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "strbuf.h"
#ಅನ್ತರ್ಭಾವ "help.h"
#ಅನ್ತರ್ಭಾವ "compat/compiler.h"
#ಅನ್ತರ್ಭಾವ "hook.h"
#ಅನ್ತರ್ಭಾವ "hook-list.h"
#ಅನ್ತರ್ಭಾವ "diagnose.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "version.h"

ಸ್ಥಿರ ಶೂನ್ಯ get_system_info(ರಚನೆ strbuf *sys_info)
{
	ಅಕ್ಷರ *shell = ಖಾಲಿ;

	/* get git version from native cmd */
	strbuf_addstr(sys_info, _("git version:\n"));
	get_ಆವೃತ್ತಿ_info(sys_info, 1);

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

ಸ್ಥಿರ ಶೂನ್ಯ get_populated_hooks(ರಚನೆ strbuf *hook_info, ಪೂರ್ಣಾಂಕ nongit)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **p;

	ಆದರೆ (nongit) {
		strbuf_addstr(hook_info,
			_("not run from a git repository - no hooks to show\n"));
		ಹಿಂತಿರುಗಿ;
	}

	ಗಾಗಿ (p = hook_name_list; *p; p++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *hook = *p;

		ಆದರೆ (hook_exists(the_repository, hook))
			strbuf_addf(hook_info, "%s\n", hook);
	}
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ bugreport_usage[] = {
	N_("git bugreport [(-o | --output-directory) <path>]\n"
	   "              [(-s | --suffix) <format> | --no-suffix]\n"
	   "              [--diagnose[=<mode>]]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_bug_template(ರಚನೆ strbuf *ಮಾದರಿ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ template_ಪಠ್ಯ[] = N_(
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

	strbuf_addstr(ಮಾದರಿ, _(template_ಪಠ್ಯ));
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ get_ಶೀರ್ಷಿಕೆ(ರಚನೆ strbuf *buf, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *title)
{
	strbuf_addf(buf, "\n\n[%s]\n", title);
}

ಪೂರ್ಣಾಂಕ cmd_bugreport(ಪೂರ್ಣಾಂಕ argc,
		  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		  ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ strbuf ಬಫರ್ = STRBUF_INIT;
	ರಚನೆ strbuf report_ಮಾರ್ಗ = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ report = -1;
	ಸಮಯ_t now = ಸಮಯ(ಖಾಲಿ);
	ರಚನೆ tm tm;
	ಎಣಿಕೆ diagnose_ಬಹುಲಕ diagnose = DIAGNOSE_NONE;
	ಅಕ್ಷರ *option_output = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *option_suffix = "%Y-%m-%d-%H%M";
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *user_relative_ಮಾರ್ಗ = ಖಾಲಿ;
	ಅಕ್ಷರ *prefixed_filename;
	ಗಾತ್ರ_t output_ಮಾರ್ಗ_len;
	ಪೂರ್ಣಾಂಕ ret;

	ಸ್ಥಿರಾಂಕ ರಚನೆ option bugreport_options[] = {
		OPT_CALLBACK_F(0, "diagnose", &diagnose, N_("mode"),
			       N_("create an additional zip archive of detailed diagnostics (default 'stats')"),
			       PARSE_OPT_OPTARG, option_parse_diagnose),
		OPT_STRING('o', "output-directory", &option_output, N_("path"),
			   N_("specify a destination for the bugreport file(s)")),
		OPT_STRING('s', "suffix", &option_suffix, N_("format"),
			   N_("specify a strftime format suffix for the filename(s)")),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, bugreport_options,
			     bugreport_usage, 0);

	ಆದರೆ (argc) {
		error(_("unknown argument `%s'"), argv[0]);
		usage(bugreport_usage[0]);
	}

	/* Prepare the path to put the result */
	prefixed_filename = ಪೂರ್ವಪ್ರತ್ಯಯ_filename(ಪೂರ್ವಪ್ರತ್ಯಯ,
					    option_output ? option_output : "");
	strbuf_addstr(&report_ಮಾರ್ಗ, prefixed_filename);
	strbuf_complete(&report_ಮಾರ್ಗ, '/');
	output_ಮಾರ್ಗ_len = report_ಮಾರ್ಗ.len;

	strbuf_addstr(&report_ಮಾರ್ಗ, "git-bugreport");
	ಆದರೆ (option_suffix) {
		strbuf_addch(&report_ಮಾರ್ಗ, '-');
		strbuf_addftime(&report_ಮಾರ್ಗ, option_suffix, localtime_r(&now, &tm), 0, 0);
	}
	strbuf_addstr(&report_ಮಾರ್ಗ, ".txt");

	ಆಯ್ಕೆ (ಸುರಕ್ಷಿತ_create_leading_directories(the_repository, report_ಮಾರ್ಗ.buf)) {
	ಸ್ಥಿತಿ SCLD_OK:
	ಸ್ಥಿತಿ SCLD_EXISTS:
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		die(_("could not create leading directories for '%s'"),
		    report_ಮಾರ್ಗ.buf);
	}

	/* Prepare diagnostics, if requested */
	ಆದರೆ (diagnose != DIAGNOSE_NONE) {
		ರಚನೆ strbuf zip_ಮಾರ್ಗ = STRBUF_INIT;
		strbuf_ಕೂಡಿಸು(&zip_ಮಾರ್ಗ, report_ಮಾರ್ಗ.buf, output_ಮಾರ್ಗ_len);
		strbuf_addstr(&zip_ಮಾರ್ಗ, "git-diagnostics-");
		strbuf_addftime(&zip_ಮಾರ್ಗ, option_suffix, localtime_r(&now, &tm), 0, 0);
		strbuf_addstr(&zip_ಮಾರ್ಗ, ".zip");

		ಆದರೆ (create_diagnostics_archive(the_repository, &zip_ಮಾರ್ಗ, diagnose))
			die_errno(_("unable to create diagnostics archive %s"), zip_ಮಾರ್ಗ.buf);

		strbuf_release(&zip_ಮಾರ್ಗ);
	}

	/* Prepare the report contents */
	get_bug_template(&ಬಫರ್);

	get_ಶೀರ್ಷಿಕೆ(&ಬಫರ್, _("System Info"));
	get_system_info(&ಬಫರ್);

	get_ಶೀರ್ಷಿಕೆ(&ಬಫರ್, _("Enabled Hooks"));
	get_populated_hooks(&ಬಫರ್, !startup_info->have_repository);

	/* fopen doesn't offer us an O_EXCL alternative, except with glibc. */
	report = xopen(report_ಮಾರ್ಗ.buf, O_CREAT | O_EXCL | O_WRONLY, 0666);

	ಆದರೆ (write_ಒಳ_ತುಂಬಿದ(report, ಬಫರ್.buf, ಬಫರ್.len) < 0)
		die_errno(_("unable to write to %s"), report_ಮಾರ್ಗ.buf);

	close(report);

	/*
	 * We want to print the path relative to the user, but we still need the
	 * path relative to us to give to the editor.
	 */
	ಆದರೆ (!(ಪೂರ್ವಪ್ರತ್ಯಯ && skip_ಪೂರ್ವಪ್ರತ್ಯಯ(report_ಮಾರ್ಗ.buf, ಪೂರ್ವಪ್ರತ್ಯಯ, &user_relative_ಮಾರ್ಗ)))
		user_relative_ಮಾರ್ಗ = report_ಮಾರ್ಗ.buf;
	fprintf(stderr, _("Created new report at '%s'.\n"),
		user_relative_ಮಾರ್ಗ);

	ಬಿಡುಗಡೆ(prefixed_filename);
	strbuf_release(&ಬಫರ್);

	ret = !!launch_editor(report_ಮಾರ್ಗ.buf, ಖಾಲಿ, ಖಾಲಿ);
	strbuf_release(&report_ಮಾರ್ಗ);
	ಹಿಂತಿರುಗಿ ret;
}
