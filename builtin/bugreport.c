#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "abspath.h"
#شامل "editor.h"
#شامل "gettext.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "strbuf.h"
#شامل "help.h"
#شامل "compat/compiler.h"
#شامل "hook.h"
#شامل "hook-list.h"
#شامل "diagnose.h"
#شامل "setup.h"
#شامل "version.h"

ساکن خالی get_system_info(ساخت strbuf *sys_info)
{
	حرف *shell = تہی;

	/* get git version from native cmd */
	strbuf_addstr(sys_info, _("git version:\n"));
	get_نسخہ_info(sys_info, 1);

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

ساکن خالی get_populated_hooks(ساخت strbuf *hook_info, صحیح nongit)
{
	ثابت حرف **p;

	اگر (nongit) {
		strbuf_addstr(hook_info,
			_("not run from a git repository - no hooks to show\n"));
		واپس;
	}

	برائے (p = hook_name_list; *p; p++) {
		ثابت حرف *hook = *p;

		اگر (hook_exists(the_repository, hook))
			strbuf_addf(hook_info, "%s\n", hook);
	}
}

ساکن ثابت حرف * ثابت bugreport_usage[] = {
	N_("git bugreport [(-o | --output-directory) <path>]\n"
	   "              [(-s | --suffix) <format> | --no-suffix]\n"
	   "              [--diagnose[=<mode>]]"),
	تہی
};

ساکن صحیح get_bug_template(ساخت strbuf *سانچہ)
{
	ثابت حرف template_متن[] = N_(
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

	strbuf_addstr(سانچہ, _(template_متن));
	واپس 0;
}

ساکن خالی get_سرخی(ساخت strbuf *buf, ثابت حرف *title)
{
	strbuf_addf(buf, "\n\n[%s]\n", title);
}

صحیح cmd_bugreport(صحیح argc,
		  ثابت حرف **argv,
		  ثابت حرف *سابقہ,
		  ساخت repository *repo غیر_مستعمل)
{
	ساخت strbuf بفر = STRBUF_INIT;
	ساخت strbuf report_راستہ = STRBUF_INIT;
	صحیح report = -1;
	وقت_t now = وقت(تہی);
	ساخت tm tm;
	شمار diagnose_منوال diagnose = DIAGNOSE_NONE;
	حرف *option_output = تہی;
	ثابت حرف *option_suffix = "%Y-%m-%d-%H%M";
	ثابت حرف *user_relative_راستہ = تہی;
	حرف *prefixed_filename;
	سائز_t output_راستہ_len;
	صحیح ret;

	ثابت ساخت option bugreport_options[] = {
		OPT_CALLBACK_F(0, "diagnose", &diagnose, N_("mode"),
			       N_("create an additional zip archive of detailed diagnostics (default 'stats')"),
			       PARSE_OPT_OPTARG, option_parse_diagnose),
		OPT_STRING('o', "output-directory", &option_output, N_("path"),
			   N_("specify a destination for the bugreport file(s)")),
		OPT_STRING('s', "suffix", &option_suffix, N_("format"),
			   N_("specify a strftime format suffix for the filename(s)")),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, bugreport_options,
			     bugreport_usage, 0);

	اگر (argc) {
		error(_("unknown argument `%s'"), argv[0]);
		usage(bugreport_usage[0]);
	}

	/* Prepare the path to put the result */
	prefixed_filename = سابقہ_filename(سابقہ,
					    option_output ? option_output : "");
	strbuf_addstr(&report_راستہ, prefixed_filename);
	strbuf_complete(&report_راستہ, '/');
	output_راستہ_len = report_راستہ.len;

	strbuf_addstr(&report_راستہ, "git-bugreport");
	اگر (option_suffix) {
		strbuf_addch(&report_راستہ, '-');
		strbuf_addftime(&report_راستہ, option_suffix, localtime_r(&now, &tm), 0, 0);
	}
	strbuf_addstr(&report_راستہ, ".txt");

	انتخاب (محفوظ_create_leading_directories(the_repository, report_راستہ.buf)) {
	حالت SCLD_OK:
	حالت SCLD_EXISTS:
		توڑو;
	طےشدہ:
		die(_("could not create leading directories for '%s'"),
		    report_راستہ.buf);
	}

	/* Prepare diagnostics, if requested */
	اگر (diagnose != DIAGNOSE_NONE) {
		ساخت strbuf zip_راستہ = STRBUF_INIT;
		strbuf_جوڑو(&zip_راستہ, report_راستہ.buf, output_راستہ_len);
		strbuf_addstr(&zip_راستہ, "git-diagnostics-");
		strbuf_addftime(&zip_راستہ, option_suffix, localtime_r(&now, &tm), 0, 0);
		strbuf_addstr(&zip_راستہ, ".zip");

		اگر (create_diagnostics_archive(the_repository, &zip_راستہ, diagnose))
			die_errno(_("unable to create diagnostics archive %s"), zip_راستہ.buf);

		strbuf_release(&zip_راستہ);
	}

	/* Prepare the report contents */
	get_bug_template(&بفر);

	get_سرخی(&بفر, _("System Info"));
	get_system_info(&بفر);

	get_سرخی(&بفر, _("Enabled Hooks"));
	get_populated_hooks(&بفر, !startup_info->have_repository);

	/* fopen doesn't offer us an O_EXCL alternative, except with glibc. */
	report = xopen(report_راستہ.buf, O_CREAT | O_EXCL | O_WRONLY, 0666);

	اگر (write_اندر_بھرا(report, بفر.buf, بفر.len) < 0)
		die_errno(_("unable to write to %s"), report_راستہ.buf);

	close(report);

	/*
	 * We want to print the path relative to the user, but we still need the
	 * path relative to us to give to the editor.
	 */
	اگر (!(سابقہ && skip_سابقہ(report_راستہ.buf, سابقہ, &user_relative_راستہ)))
		user_relative_راستہ = report_راستہ.buf;
	fprintf(stderr, _("Created new report at '%s'.\n"),
		user_relative_راستہ);

	آزاد(prefixed_filename);
	strbuf_release(&بفر);

	ret = !!launch_editor(report_راستہ.buf, تہی, تہی);
	strbuf_release(&report_راستہ);
	واپس ret;
}
