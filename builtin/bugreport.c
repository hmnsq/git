#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "editor.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "help.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "compat/compiler.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hook.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hook-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diagnose.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "version.h"

ਸਥਿਰ ਖਾਲੀ get_system_info(ਬਣਤਰ strbuf *sys_info)
{
	ਅੱਖਰ *shell = ਨਿਰਾ;

	/* get git version from native cmd */
	strbuf_addstr(sys_info, _("git version:\n"));
	get_ਸੰਸਕਰਣ_info(sys_info, 1);

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

ਸਥਿਰ ਖਾਲੀ get_populated_hooks(ਬਣਤਰ strbuf *hook_info, ਪੂਰਨ nongit)
{
	ਅਚਲ ਅੱਖਰ **p;

	ਜੇ (nongit) {
		strbuf_addstr(hook_info,
			_("not run from a git repository - no hooks to show\n"));
		ਵਾਪਸ;
	}

	ਲਈ (p = hook_name_list; *p; p++) {
		ਅਚਲ ਅੱਖਰ *hook = *p;

		ਜੇ (hook_exists(the_repository, hook))
			strbuf_addf(hook_info, "%s\n", hook);
	}
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ bugreport_usage[] = {
	N_("git bugreport [(-o | --output-directory) <path>]\n"
	   "              [(-s | --suffix) <format> | --no-suffix]\n"
	   "              [--diagnose[=<mode>]]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ get_bug_template(ਬਣਤਰ strbuf *ਸਾਂਚਾ)
{
	ਅਚਲ ਅੱਖਰ template_ਪਾਠ[] = N_(
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

	strbuf_addstr(ਸਾਂਚਾ, _(template_ਪਾਠ));
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ get_ਸਿਰਲੇਖ(ਬਣਤਰ strbuf *buf, ਅਚਲ ਅੱਖਰ *title)
{
	strbuf_addf(buf, "\n\n[%s]\n", title);
}

ਪੂਰਨ cmd_bugreport(ਪੂਰਨ argc,
		  ਅਚਲ ਅੱਖਰ **argv,
		  ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		  ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ strbuf ਬਫਰ = STRBUF_INIT;
	ਬਣਤਰ strbuf report_ਰਸਤਾ = STRBUF_INIT;
	ਪੂਰਨ report = -1;
	ਸਮਾਂ_t now = ਸਮਾਂ(ਨਿਰਾ);
	ਬਣਤਰ tm tm;
	ਗਿਣਤੀ diagnose_ਬਹੁਲਕ diagnose = DIAGNOSE_NONE;
	ਅੱਖਰ *option_output = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *option_suffix = "%Y-%m-%d-%H%M";
	ਅਚਲ ਅੱਖਰ *user_relative_ਰਸਤਾ = ਨਿਰਾ;
	ਅੱਖਰ *prefixed_filename;
	ਮਾਪ_t output_ਰਸਤਾ_len;
	ਪੂਰਨ ret;

	ਅਚਲ ਬਣਤਰ option bugreport_options[] = {
		OPT_CALLBACK_F(0, "diagnose", &diagnose, N_("mode"),
			       N_("create an additional zip archive of detailed diagnostics (default 'stats')"),
			       PARSE_OPT_OPTARG, option_parse_diagnose),
		OPT_STRING('o', "output-directory", &option_output, N_("path"),
			   N_("specify a destination for the bugreport file(s)")),
		OPT_STRING('s', "suffix", &option_suffix, N_("format"),
			   N_("specify a strftime format suffix for the filename(s)")),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, bugreport_options,
			     bugreport_usage, 0);

	ਜੇ (argc) {
		error(_("unknown argument `%s'"), argv[0]);
		usage(bugreport_usage[0]);
	}

	/* Prepare the path to put the result */
	prefixed_filename = ਅਗੇਤਰ_filename(ਅਗੇਤਰ,
					    option_output ? option_output : "");
	strbuf_addstr(&report_ਰਸਤਾ, prefixed_filename);
	strbuf_complete(&report_ਰਸਤਾ, '/');
	output_ਰਸਤਾ_len = report_ਰਸਤਾ.len;

	strbuf_addstr(&report_ਰਸਤਾ, "git-bugreport");
	ਜੇ (option_suffix) {
		strbuf_addch(&report_ਰਸਤਾ, '-');
		strbuf_addftime(&report_ਰਸਤਾ, option_suffix, localtime_r(&now, &tm), 0, 0);
	}
	strbuf_addstr(&report_ਰਸਤਾ, ".txt");

	ਚੋਣ (ਸੁਰੱਖਿਅਤ_create_leading_directories(the_repository, report_ਰਸਤਾ.buf)) {
	ਹਾਲਤ SCLD_OK:
	ਹਾਲਤ SCLD_EXISTS:
		ਰੋਕੋ;
	ਮੂਲ:
		die(_("could not create leading directories for '%s'"),
		    report_ਰਸਤਾ.buf);
	}

	/* Prepare diagnostics, if requested */
	ਜੇ (diagnose != DIAGNOSE_NONE) {
		ਬਣਤਰ strbuf zip_ਰਸਤਾ = STRBUF_INIT;
		strbuf_ਜੋੜੋ(&zip_ਰਸਤਾ, report_ਰਸਤਾ.buf, output_ਰਸਤਾ_len);
		strbuf_addstr(&zip_ਰਸਤਾ, "git-diagnostics-");
		strbuf_addftime(&zip_ਰਸਤਾ, option_suffix, localtime_r(&now, &tm), 0, 0);
		strbuf_addstr(&zip_ਰਸਤਾ, ".zip");

		ਜੇ (create_diagnostics_archive(the_repository, &zip_ਰਸਤਾ, diagnose))
			die_errno(_("unable to create diagnostics archive %s"), zip_ਰਸਤਾ.buf);

		strbuf_release(&zip_ਰਸਤਾ);
	}

	/* Prepare the report contents */
	get_bug_template(&ਬਫਰ);

	get_ਸਿਰਲੇਖ(&ਬਫਰ, _("System Info"));
	get_system_info(&ਬਫਰ);

	get_ਸਿਰਲੇਖ(&ਬਫਰ, _("Enabled Hooks"));
	get_populated_hooks(&ਬਫਰ, !startup_info->have_repository);

	/* fopen doesn't offer us an O_EXCL alternative, except with glibc. */
	report = xopen(report_ਰਸਤਾ.buf, O_CREAT | O_EXCL | O_WRONLY, 0666);

	ਜੇ (write_ਵਿੱਚ_ਭਰਿਆ(report, ਬਫਰ.buf, ਬਫਰ.len) < 0)
		die_errno(_("unable to write to %s"), report_ਰਸਤਾ.buf);

	close(report);

	/*
	 * We want to print the path relative to the user, but we still need the
	 * path relative to us to give to the editor.
	 */
	ਜੇ (!(ਅਗੇਤਰ && skip_ਅਗੇਤਰ(report_ਰਸਤਾ.buf, ਅਗੇਤਰ, &user_relative_ਰਸਤਾ)))
		user_relative_ਰਸਤਾ = report_ਰਸਤਾ.buf;
	fprintf(stderr, _("Created new report at '%s'.\n"),
		user_relative_ਰਸਤਾ);

	ਮੁਕਤ(prefixed_filename);
	strbuf_release(&ਬਫਰ);

	ret = !!launch_editor(report_ਰਸਤਾ.buf, ਨਿਰਾ, ਨਿਰਾ);
	strbuf_release(&report_ਰਸਤਾ);
	ਵਾਪਸ ret;
}
