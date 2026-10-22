#define USE_THE_REPOSITORY_VARIABLE
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

ایستا تهی get_system_info(ساختار strbuf *sys_info)
{
	نویسه *shell = پوچ;

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

ایستا تهی get_populated_hooks(ساختار strbuf *hook_info, صحیح nongit)
{
	ثابت نویسه **p;

	اگر (nongit) {
		strbuf_addstr(hook_info,
			_("not run from a git repository - no hooks to show\n"));
		بازگشت;
	}

	برای (p = hook_name_list; *p; p++) {
		ثابت نویسه *hook = *p;

		اگر (hook_exists(the_repository, hook))
			strbuf_addf(hook_info, "%s\n", hook);
	}
}

ایستا ثابت نویسه * ثابت bugreport_usage[] = {
	N_("git bugreport [(-o | --output-directory) <path>]\n"
	   "              [(-s | --suffix) <format> | --no-suffix]\n"
	   "              [--diagnose[=<mode>]]"),
	پوچ
};

ایستا صحیح get_bug_template(ساختار strbuf *template)
{
	ثابت نویسه template_text[] = N_(
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
	بازگشت 0;
}

ایستا تهی get_header(ساختار strbuf *buf, ثابت نویسه *title)
{
	strbuf_addf(buf, "\n\n[%s]\n", title);
}

صحیح cmd_bugreport(صحیح argc,
		  ثابت نویسه **argv,
		  ثابت نویسه *prefix,
		  ساختار repository *repo UNUSED)
{
	ساختار strbuf buffer = STRBUF_INIT;
	ساختار strbuf report_path = STRBUF_INIT;
	صحیح report = -1;
	time_t now = time(پوچ);
	ساختار tm tm;
	شمارش diagnose_mode diagnose = DIAGNOSE_NONE;
	نویسه *option_output = پوچ;
	ثابت نویسه *option_suffix = "%Y-%m-%d-%H%M";
	ثابت نویسه *user_relative_path = پوچ;
	نویسه *prefixed_filename;
	size_t output_path_len;
	صحیح ret;

	ثابت ساختار option bugreport_options[] = {
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

	اگر (argc) {
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
	اگر (option_suffix) {
		strbuf_addch(&report_path, '-');
		strbuf_addftime(&report_path, option_suffix, localtime_r(&now, &tm), 0, 0);
	}
	strbuf_addstr(&report_path, ".txt");

	انتخاب (safe_create_leading_directories(the_repository, report_path.buf)) {
	حالت SCLD_OK:
	حالت SCLD_EXISTS:
		بشکن;
	پیشفرض:
		die(_("could not create leading directories for '%s'"),
		    report_path.buf);
	}

	/* Prepare diagnostics, if requested */
	اگر (diagnose != DIAGNOSE_NONE) {
		ساختار strbuf zip_path = STRBUF_INIT;
		strbuf_add(&zip_path, report_path.buf, output_path_len);
		strbuf_addstr(&zip_path, "git-diagnostics-");
		strbuf_addftime(&zip_path, option_suffix, localtime_r(&now, &tm), 0, 0);
		strbuf_addstr(&zip_path, ".zip");

		اگر (create_diagnostics_archive(the_repository, &zip_path, diagnose))
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

	اگر (write_in_full(report, buffer.buf, buffer.len) < 0)
		die_errno(_("unable to write to %s"), report_path.buf);

	close(report);

	/*
	 * We want to print the path relative to the user, but we still need the
	 * path relative to us to give to the editor.
	 */
	اگر (!(prefix && skip_prefix(report_path.buf, prefix, &user_relative_path)))
		user_relative_path = report_path.buf;
	fprintf(stderr, _("Created new report at '%s'.\n"),
		user_relative_path);

	آزاد(prefixed_filename);
	strbuf_release(&buffer);

	ret = !!launch_editor(report_path.buf, پوچ, پوچ);
	strbuf_release(&report_path);
	بازگشت ret;
}
