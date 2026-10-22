#define USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "abspath.h"
#ضمن "editor.h"
#ضمن "gettext.h"
#ضمن "parse-options.h"
#ضمن "path.h"
#ضمن "strbuf.h"
#ضمن "help.h"
#ضمن "compat/compiler.h"
#ضمن "hook.h"
#ضمن "hook-list.h"
#ضمن "diagnose.h"
#ضمن "setup.h"
#ضمن "version.h"

ساكن فراغ get_system_info(بنية strbuf *sys_info)
{
	حرف *shell = عدم;

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

ساكن فراغ get_populated_hooks(بنية strbuf *hook_info, صحيح nongit)
{
	ثابت حرف **p;

	𐤀𐤌 (nongit) {
		strbuf_addstr(hook_info,
			_("not run from a git repository - no hooks to show\n"));
		أرجع;
	}

	لكل (p = hook_name_list; *p; p++) {
		ثابت حرف *hook = *p;

		𐤀𐤌 (hook_exists(the_repository, hook))
			strbuf_addf(hook_info, "%s\n", hook);
	}
}

ساكن ثابت حرف * ثابت bugreport_usage[] = {
	N_("git bugreport [(-o | --output-directory) <path>]\n"
	   "              [(-s | --suffix) <format> | --no-suffix]\n"
	   "              [--diagnose[=<mode>]]"),
	عدم
};

ساكن صحيح get_bug_template(بنية strbuf *template)
{
	ثابت حرف template_text[] = N_(
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
	أرجع 0;
}

ساكن فراغ get_header(بنية strbuf *buf, ثابت حرف *title)
{
	strbuf_addf(buf, "\n\n[%s]\n", title);
}

صحيح cmd_bugreport(صحيح argc,
		  ثابت حرف **argv,
		  ثابت حرف *prefix,
		  بنية repository *repo UNUSED)
{
	بنية strbuf buffer = STRBUF_INIT;
	بنية strbuf report_path = STRBUF_INIT;
	صحيح report = -1;
	time_t now = time(عدم);
	بنية tm tm;
	تعداد diagnose_mode diagnose = DIAGNOSE_NONE;
	حرف *option_output = عدم;
	ثابت حرف *option_suffix = "%Y-%m-%d-%H%M";
	ثابت حرف *user_relative_path = عدم;
	حرف *prefixed_filename;
	size_t output_path_len;
	صحيح ret;

	ثابت بنية option bugreport_options[] = {
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

	𐤀𐤌 (argc) {
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
	𐤀𐤌 (option_suffix) {
		strbuf_addch(&report_path, '-');
		strbuf_addftime(&report_path, option_suffix, localtime_r(&now, &tm), 0, 0);
	}
	strbuf_addstr(&report_path, ".txt");

	بدل (safe_create_leading_directories(the_repository, report_path.buf)) {
	حالة SCLD_OK:
	حالة SCLD_EXISTS:
		اكسر;
	افتراضي:
		die(_("could not create leading directories for '%s'"),
		    report_path.buf);
	}

	/* Prepare diagnostics, if requested */
	𐤀𐤌 (diagnose != DIAGNOSE_NONE) {
		بنية strbuf zip_path = STRBUF_INIT;
		strbuf_add(&zip_path, report_path.buf, output_path_len);
		strbuf_addstr(&zip_path, "git-diagnostics-");
		strbuf_addftime(&zip_path, option_suffix, localtime_r(&now, &tm), 0, 0);
		strbuf_addstr(&zip_path, ".zip");

		𐤀𐤌 (create_diagnostics_archive(the_repository, &zip_path, diagnose))
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

	𐤀𐤌 (write_in_full(report, buffer.buf, buffer.len) < 0)
		die_errno(_("unable to write to %s"), report_path.buf);

	close(report);

	/*
	 * We want to print the path relative to the user, but we still need the
	 * path relative to us to give to the editor.
	 */
	𐤀𐤌 (!(prefix && skip_prefix(report_path.buf, prefix, &user_relative_path)))
		user_relative_path = report_path.buf;
	fprintf(stderr, _("Created new report at '%s'.\n"),
		user_relative_path);

	حرر(prefixed_filename);
	strbuf_release(&buffer);

	ret = !!launch_editor(report_path.buf, عدم, عدم);
	strbuf_release(&report_path);
	أرجع ret;
}
