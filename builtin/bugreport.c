#define USE_THE_REPOSITORY_VARIABLE
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

ಸ್ಥಿರ ಶೂನ್ಯ get_system_info(ಸಂರಚನಾ strbuf *sys_info)
{
	ವರ್ಣ *shell = ರಿಕ್ತ;

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

ಸ್ಥಿರ ಶೂನ್ಯ get_populated_hooks(ಸಂರಚನಾ strbuf *hook_info, ಪೂರ್ಣಾಙ್ಕ nongit)
{
	ಧ್ರುವ ವರ್ಣ **p;

	ಯದಿ (nongit) {
		strbuf_addstr(hook_info,
			_("not run from a git repository - no hooks to show\n"));
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಪ್ರತಿ (p = hook_name_list; *p; p++) {
		ಧ್ರುವ ವರ್ಣ *hook = *p;

		ಯದಿ (hook_exists(the_repository, hook))
			strbuf_addf(hook_info, "%s\n", hook);
	}
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ bugreport_usage[] = {
	N_("git bugreport [(-o | --output-directory) <path>]\n"
	   "              [(-s | --suffix) <format> | --no-suffix]\n"
	   "              [--diagnose[=<mode>]]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_bug_template(ಸಂರಚನಾ strbuf *template)
{
	ಧ್ರುವ ವರ್ಣ template_text[] = N_(
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
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ get_header(ಸಂರಚನಾ strbuf *buf, ಧ್ರುವ ವರ್ಣ *title)
{
	strbuf_addf(buf, "\n\n[%s]\n", title);
}

ಪೂರ್ಣಾಙ್ಕ cmd_bugreport(ಪೂರ್ಣಾಙ್ಕ argc,
		  ಧ್ರುವ ವರ್ಣ **argv,
		  ಧ್ರುವ ವರ್ಣ *prefix,
		  ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ strbuf buffer = STRBUF_INIT;
	ಸಂರಚನಾ strbuf report_path = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ report = -1;
	time_t now = time(ರಿಕ್ತ);
	ಸಂರಚನಾ tm tm;
	ಗಣನಾ diagnose_mode diagnose = DIAGNOSE_NONE;
	ವರ್ಣ *option_output = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *option_suffix = "%Y-%m-%d-%H%M";
	ಧ್ರುವ ವರ್ಣ *user_relative_path = ರಿಕ್ತ;
	ವರ್ಣ *prefixed_filename;
	size_t output_path_len;
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಧ್ರುವ ಸಂರಚನಾ option bugreport_options[] = {
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

	ಯದಿ (argc) {
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
	ಯದಿ (option_suffix) {
		strbuf_addch(&report_path, '-');
		strbuf_addftime(&report_path, option_suffix, localtime_r(&now, &tm), 0, 0);
	}
	strbuf_addstr(&report_path, ".txt");

	ವಿಕಲ್ಪ (safe_create_leading_directories(the_repository, report_path.buf)) {
	ಪಕ್ಷ SCLD_OK:
	ಪಕ್ಷ SCLD_EXISTS:
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		die(_("could not create leading directories for '%s'"),
		    report_path.buf);
	}

	/* Prepare diagnostics, if requested */
	ಯದಿ (diagnose != DIAGNOSE_NONE) {
		ಸಂರಚನಾ strbuf zip_path = STRBUF_INIT;
		strbuf_add(&zip_path, report_path.buf, output_path_len);
		strbuf_addstr(&zip_path, "git-diagnostics-");
		strbuf_addftime(&zip_path, option_suffix, localtime_r(&now, &tm), 0, 0);
		strbuf_addstr(&zip_path, ".zip");

		ಯದಿ (create_diagnostics_archive(the_repository, &zip_path, diagnose))
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

	ಯದಿ (write_in_full(report, buffer.buf, buffer.len) < 0)
		die_errno(_("unable to write to %s"), report_path.buf);

	close(report);

	/*
	 * We want to print the path relative to the user, but we still need the
	 * path relative to us to give to the editor.
	 */
	ಯದಿ (!(prefix && skip_prefix(report_path.buf, prefix, &user_relative_path)))
		user_relative_path = report_path.buf;
	fprintf(stderr, _("Created new report at '%s'.\n"),
		user_relative_path);

	ಮುಕ್ತ(prefixed_filename);
	strbuf_release(&buffer);

	ret = !!launch_editor(report_path.buf, ರಿಕ್ತ, ರಿಕ್ತ);
	strbuf_release(&report_path);
	ಪ್ರತ್ಯಾಗಮ ret;
}
