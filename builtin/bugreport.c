#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "abspath.h"
#অন্তর্ভাব "editor.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "strbuf.h"
#অন্তর্ভাব "help.h"
#অন্তর্ভাব "compat/compiler.h"
#অন্তর্ভাব "hook.h"
#অন্তর্ভাব "hook-list.h"
#অন্তর্ভাব "diagnose.h"
#অন্তর্ভাব "setup.h"
#অন্তর্ভাব "version.h"

স্থির শূন্য get_system_info(কাঠামো strbuf *sys_info)
{
	অক্ষর *shell = ফাঁকা;

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

স্থির শূন্য get_populated_hooks(কাঠামো strbuf *hook_info, পূর্ণসংখ্যা nongit)
{
	ধ্রুবক অক্ষর **p;

	যদি (nongit) {
		strbuf_addstr(hook_info,
			_("not run from a git repository - no hooks to show\n"));
		ফেরত;
	}

	জন্য (p = hook_name_list; *p; p++) {
		ধ্রুবক অক্ষর *hook = *p;

		যদি (hook_exists(the_repository, hook))
			strbuf_addf(hook_info, "%s\n", hook);
	}
}

স্থির ধ্রুবক অক্ষর * ধ্রুবক bugreport_usage[] = {
	N_("git bugreport [(-o | --output-directory) <path>]\n"
	   "              [(-s | --suffix) <format> | --no-suffix]\n"
	   "              [--diagnose[=<mode>]]"),
	ফাঁকা
};

স্থির পূর্ণসংখ্যা get_bug_template(কাঠামো strbuf *template)
{
	ধ্রুবক অক্ষর template_text[] = N_(
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
	ফেরত 0;
}

স্থির শূন্য get_header(কাঠামো strbuf *buf, ধ্রুবক অক্ষর *title)
{
	strbuf_addf(buf, "\n\n[%s]\n", title);
}

পূর্ণসংখ্যা cmd_bugreport(পূর্ণসংখ্যা argc,
		  ধ্রুবক অক্ষর **argv,
		  ধ্রুবক অক্ষর *prefix,
		  কাঠামো repository *repo UNUSED)
{
	কাঠামো strbuf বাফার = STRBUF_INIT;
	কাঠামো strbuf report_path = STRBUF_INIT;
	পূর্ণসংখ্যা report = -1;
	time_t now = time(ফাঁকা);
	কাঠামো tm tm;
	গণনা diagnose_mode diagnose = DIAGNOSE_NONE;
	অক্ষর *option_output = ফাঁকা;
	ধ্রুবক অক্ষর *option_suffix = "%Y-%m-%d-%H%M";
	ধ্রুবক অক্ষর *user_relative_path = ফাঁকা;
	অক্ষর *prefixed_filename;
	আকার_t output_path_len;
	পূর্ণসংখ্যা ret;

	ধ্রুবক কাঠামো option bugreport_options[] = {
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

	যদি (argc) {
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
	যদি (option_suffix) {
		strbuf_addch(&report_path, '-');
		strbuf_addftime(&report_path, option_suffix, localtime_r(&now, &tm), 0, 0);
	}
	strbuf_addstr(&report_path, ".txt");

	নির্বাচন (safe_create_leading_directories(the_repository, report_path.buf)) {
	ক্ষেত্র SCLD_OK:
	ক্ষেত্র SCLD_EXISTS:
		বিরতি;
	পূর্বনির্ধারিত:
		die(_("could not create leading directories for '%s'"),
		    report_path.buf);
	}

	/* Prepare diagnostics, if requested */
	যদি (diagnose != DIAGNOSE_NONE) {
		কাঠামো strbuf zip_path = STRBUF_INIT;
		strbuf_add(&zip_path, report_path.buf, output_path_len);
		strbuf_addstr(&zip_path, "git-diagnostics-");
		strbuf_addftime(&zip_path, option_suffix, localtime_r(&now, &tm), 0, 0);
		strbuf_addstr(&zip_path, ".zip");

		যদি (create_diagnostics_archive(the_repository, &zip_path, diagnose))
			die_errno(_("unable to create diagnostics archive %s"), zip_path.buf);

		strbuf_release(&zip_path);
	}

	/* Prepare the report contents */
	get_bug_template(&বাফার);

	get_header(&বাফার, _("System Info"));
	get_system_info(&বাফার);

	get_header(&বাফার, _("Enabled Hooks"));
	get_populated_hooks(&বাফার, !startup_info->have_repository);

	/* fopen doesn't offer us an O_EXCL alternative, except with glibc. */
	report = xopen(report_path.buf, O_CREAT | O_EXCL | O_WRONLY, 0666);

	যদি (write_in_full(report, বাফার.buf, বাফার.len) < 0)
		die_errno(_("unable to write to %s"), report_path.buf);

	close(report);

	/*
	 * We want to print the path relative to the user, but we still need the
	 * path relative to us to give to the editor.
	 */
	যদি (!(prefix && skip_prefix(report_path.buf, prefix, &user_relative_path)))
		user_relative_path = report_path.buf;
	fprintf(stderr, _("Created new report at '%s'.\n"),
		user_relative_path);

	মুক্ত(prefixed_filename);
	strbuf_release(&বাফার);

	ret = !!launch_editor(report_path.buf, ফাঁকা, ফাঁকা);
	strbuf_release(&report_path);
	ফেরত ret;
}
