#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"
#समावेश "abspath.h"
#समावेश "editor.h"
#समावेश "gettext.h"
#समावेश "parse-options.h"
#समावेश "path.h"
#समावेश "strbuf.h"
#समावेश "help.h"
#समावेश "compat/compiler.h"
#समावेश "hook.h"
#समावेश "hook-list.h"
#समावेश "diagnose.h"
#समावेश "setup.h"
#समावेश "version.h"

अटल व्योम get_system_info(काष्ठा strbuf *sys_info)
{
	अक्षर *shell = शून्य;

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

	shell = दो_पर्या("SHELL");
	strbuf_addf(sys_info, "$SHELL (typically, interactive shell): %s\n",
		    shell ? shell : "<unset>");
}

अटल व्योम get_populated_hooks(काष्ठा strbuf *hook_info, पूर्णांक nongit)
{
	स्थिर अक्षर **p;

	अगर (nongit) {
		strbuf_addstr(hook_info,
			_("not run from a git repository - no hooks to show\n"));
		वापस;
	}

	क्रम (p = hook_name_list; *p; p++) {
		स्थिर अक्षर *hook = *p;

		अगर (hook_exists(the_repository, hook))
			strbuf_addf(hook_info, "%s\n", hook);
	}
}

अटल स्थिर अक्षर * स्थिर bugreport_usage[] = {
	N_("git bugreport [(-o | --output-directory) <path>]\n"
	   "              [(-s | --suffix) <format> | --no-suffix]\n"
	   "              [--diagnose[=<mode>]]"),
	शून्य
};

अटल पूर्णांक get_bug_template(काष्ठा strbuf *ढाँचा)
{
	स्थिर अक्षर template_text[] = N_(
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

	strbuf_addstr(ढाँचा, _(template_text));
	वापस 0;
}

अटल व्योम get_header(काष्ठा strbuf *buf, स्थिर अक्षर *title)
{
	strbuf_addf(buf, "\n\n[%s]\n", title);
}

पूर्णांक cmd_bugreport(पूर्णांक argc,
		  स्थिर अक्षर **argv,
		  स्थिर अक्षर *prefix,
		  काष्ठा repository *repo UNUSED)
{
	काष्ठा strbuf बफर = STRBUF_INIT;
	काष्ठा strbuf report_path = STRBUF_INIT;
	पूर्णांक report = -1;
	समय_प्रकार now = समय(शून्य);
	काष्ठा पंचांग पंचांग;
	क्रमागत diagnose_mode diagnose = DIAGNOSE_NONE;
	अक्षर *option_output = शून्य;
	स्थिर अक्षर *option_suffix = "%Y-%m-%d-%H%M";
	स्थिर अक्षर *user_relative_path = शून्य;
	अक्षर *prefixed_filename;
	माप_प्रकार output_path_len;
	पूर्णांक ret;

	स्थिर काष्ठा option bugreport_options[] = {
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

	अगर (argc) {
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
	अगर (option_suffix) {
		strbuf_addch(&report_path, '-');
		strbuf_addftime(&report_path, option_suffix, localtime_r(&now, &पंचांग), 0, 0);
	}
	strbuf_addstr(&report_path, ".txt");

	चयन (safe_create_leading_directories(the_repository, report_path.buf)) {
	हाल SCLD_OK:
	हाल SCLD_EXISTS:
		अवरोध;
	शेष:
		die(_("could not create leading directories for '%s'"),
		    report_path.buf);
	}

	/* Prepare diagnostics, if requested */
	अगर (diagnose != DIAGNOSE_NONE) {
		काष्ठा strbuf zip_path = STRBUF_INIT;
		strbuf_add(&zip_path, report_path.buf, output_path_len);
		strbuf_addstr(&zip_path, "git-diagnostics-");
		strbuf_addftime(&zip_path, option_suffix, localtime_r(&now, &पंचांग), 0, 0);
		strbuf_addstr(&zip_path, ".zip");

		अगर (create_diagnostics_archive(the_repository, &zip_path, diagnose))
			die_errno(_("unable to create diagnostics archive %s"), zip_path.buf);

		strbuf_release(&zip_path);
	}

	/* Prepare the report contents */
	get_bug_template(&बफर);

	get_header(&बफर, _("System Info"));
	get_system_info(&बफर);

	get_header(&बफर, _("Enabled Hooks"));
	get_populated_hooks(&बफर, !startup_info->have_repository);

	/* fopen doesn't offer us an O_EXCL alternative, except with glibc. */
	report = xopen(report_path.buf, O_CREAT | O_EXCL | O_WRONLY, 0666);

	अगर (write_in_full(report, बफर.buf, बफर.len) < 0)
		die_errno(_("unable to write to %s"), report_path.buf);

	बंद(report);

	/*
	 * We want to print the path relative to the user, but we still need the
	 * path relative to us to give to the editor.
	 */
	अगर (!(prefix && skip_prefix(report_path.buf, prefix, &user_relative_path)))
		user_relative_path = report_path.buf;
	ख_लिखो(मानक_त्रुटि, _("Created new report at '%s'.\n"),
		user_relative_path);

	मुक्त(prefixed_filename);
	strbuf_release(&बफर);

	ret = !!launch_editor(report_path.buf, शून्य, शून्य);
	strbuf_release(&report_path);
	वापस ret;
}
