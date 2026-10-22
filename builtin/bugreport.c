#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "abspath.h"
#అన్తర్భావ "editor.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "strbuf.h"
#అన్తర్భావ "help.h"
#అన్తర్భావ "compat/compiler.h"
#అన్తర్భావ "hook.h"
#అన్తర్భావ "hook-list.h"
#అన్తర్భావ "diagnose.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "version.h"

స్థిర శూన్య get_system_info(నిర్మాణం strbuf *sys_info)
{
	అక్షరం *shell = ఖాళీ;

	/* get git version from native cmd */
	strbuf_addstr(sys_info, _("git version:\n"));
	get_సంస్కరణ_info(sys_info, 1);

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

స్థిర శూన్య get_populated_hooks(నిర్మాణం strbuf *hook_info, పూర్ణాంకం nongit)
{
	స్థిరరాశి అక్షరం **p;

	ఒకవేళ (nongit) {
		strbuf_addstr(hook_info,
			_("not run from a git repository - no hooks to show\n"));
		తిరిగి;
	}

	కొరకు (p = hook_name_list; *p; p++) {
		స్థిరరాశి అక్షరం *hook = *p;

		ఒకవేళ (hook_exists(the_repository, hook))
			strbuf_addf(hook_info, "%s\n", hook);
	}
}

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి bugreport_usage[] = {
	N_("git bugreport [(-o | --output-directory) <path>]\n"
	   "              [(-s | --suffix) <format> | --no-suffix]\n"
	   "              [--diagnose[=<mode>]]"),
	ఖాళీ
};

స్థిర పూర్ణాంకం get_bug_template(నిర్మాణం strbuf *మూస)
{
	స్థిరరాశి అక్షరం template_పాఠ్యం[] = N_(
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

	strbuf_addstr(మూస, _(template_పాఠ్యం));
	తిరిగి 0;
}

స్థిర శూన్య get_శీర్షిక(నిర్మాణం strbuf *buf, స్థిరరాశి అక్షరం *title)
{
	strbuf_addf(buf, "\n\n[%s]\n", title);
}

పూర్ణాంకం cmd_bugreport(పూర్ణాంకం argc,
		  స్థిరరాశి అక్షరం **argv,
		  స్థిరరాశి అక్షరం *ఉపసర్గ,
		  నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం strbuf బఫరు = STRBUF_INIT;
	నిర్మాణం strbuf report_మార్గం = STRBUF_INIT;
	పూర్ణాంకం report = -1;
	సమయం_t now = సమయం(ఖాళీ);
	నిర్మాణం tm tm;
	గణన diagnose_బహుళకం diagnose = DIAGNOSE_NONE;
	అక్షరం *option_output = ఖాళీ;
	స్థిరరాశి అక్షరం *option_suffix = "%Y-%m-%d-%H%M";
	స్థిరరాశి అక్షరం *user_relative_మార్గం = ఖాళీ;
	అక్షరం *prefixed_filename;
	కొలత_t output_మార్గం_len;
	పూర్ణాంకం ret;

	స్థిరరాశి నిర్మాణం option bugreport_options[] = {
		OPT_CALLBACK_F(0, "diagnose", &diagnose, N_("mode"),
			       N_("create an additional zip archive of detailed diagnostics (default 'stats')"),
			       PARSE_OPT_OPTARG, option_parse_diagnose),
		OPT_STRING('o', "output-directory", &option_output, N_("path"),
			   N_("specify a destination for the bugreport file(s)")),
		OPT_STRING('s', "suffix", &option_suffix, N_("format"),
			   N_("specify a strftime format suffix for the filename(s)")),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, bugreport_options,
			     bugreport_usage, 0);

	ఒకవేళ (argc) {
		error(_("unknown argument `%s'"), argv[0]);
		usage(bugreport_usage[0]);
	}

	/* Prepare the path to put the result */
	prefixed_filename = ఉపసర్గ_filename(ఉపసర్గ,
					    option_output ? option_output : "");
	strbuf_addstr(&report_మార్గం, prefixed_filename);
	strbuf_complete(&report_మార్గం, '/');
	output_మార్గం_len = report_మార్గం.len;

	strbuf_addstr(&report_మార్గం, "git-bugreport");
	ఒకవేళ (option_suffix) {
		strbuf_addch(&report_మార్గం, '-');
		strbuf_addftime(&report_మార్గం, option_suffix, localtime_r(&now, &tm), 0, 0);
	}
	strbuf_addstr(&report_మార్గం, ".txt");

	ఎంపిక (సురక్షిత_create_leading_directories(the_repository, report_మార్గం.buf)) {
	స్థితి SCLD_OK:
	స్థితి SCLD_EXISTS:
		ఆపు;
	అప్రమేయ:
		die(_("could not create leading directories for '%s'"),
		    report_మార్గం.buf);
	}

	/* Prepare diagnostics, if requested */
	ఒకవేళ (diagnose != DIAGNOSE_NONE) {
		నిర్మాణం strbuf zip_మార్గం = STRBUF_INIT;
		strbuf_కూడు(&zip_మార్గం, report_మార్గం.buf, output_మార్గం_len);
		strbuf_addstr(&zip_మార్గం, "git-diagnostics-");
		strbuf_addftime(&zip_మార్గం, option_suffix, localtime_r(&now, &tm), 0, 0);
		strbuf_addstr(&zip_మార్గం, ".zip");

		ఒకవేళ (create_diagnostics_archive(the_repository, &zip_మార్గం, diagnose))
			die_errno(_("unable to create diagnostics archive %s"), zip_మార్గం.buf);

		strbuf_release(&zip_మార్గం);
	}

	/* Prepare the report contents */
	get_bug_template(&బఫరు);

	get_శీర్షిక(&బఫరు, _("System Info"));
	get_system_info(&బఫరు);

	get_శీర్షిక(&బఫరు, _("Enabled Hooks"));
	get_populated_hooks(&బఫరు, !startup_info->have_repository);

	/* fopen doesn't offer us an O_EXCL alternative, except with glibc. */
	report = xopen(report_మార్గం.buf, O_CREAT | O_EXCL | O_WRONLY, 0666);

	ఒకవేళ (write_లోపల_నిండిన(report, బఫరు.buf, బఫరు.len) < 0)
		die_errno(_("unable to write to %s"), report_మార్గం.buf);

	close(report);

	/*
	 * We want to print the path relative to the user, but we still need the
	 * path relative to us to give to the editor.
	 */
	ఒకవేళ (!(ఉపసర్గ && skip_ఉపసర్గ(report_మార్గం.buf, ఉపసర్గ, &user_relative_మార్గం)))
		user_relative_మార్గం = report_మార్గం.buf;
	fprintf(stderr, _("Created new report at '%s'.\n"),
		user_relative_మార్గం);

	విడుదల(prefixed_filename);
	strbuf_release(&బఫరు);

	ret = !!launch_editor(report_మార్గం.buf, ఖాళీ, ఖాళీ);
	strbuf_release(&report_మార్గం);
	తిరిగి ret;
}
