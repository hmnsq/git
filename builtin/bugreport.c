#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "abspath.h"
#அந்தர்பாவ "editor.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "strbuf.h"
#அந்தர்பாவ "help.h"
#அந்தர்பாவ "compat/compiler.h"
#அந்தர்பாவ "hook.h"
#அந்தர்பாவ "hook-list.h"
#அந்தர்பாவ "diagnose.h"
#அந்தர்பாவ "setup.h"
#அந்தர்பாவ "version.h"

நிலையான வெறுமை get_system_info(கட்டமைப்பு strbuf *sys_info)
{
	எழுத்து *shell = வெற்று;

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

நிலையான வெறுமை get_populated_hooks(கட்டமைப்பு strbuf *hook_info, முழு nongit)
{
	மாறிலி எழுத்து **p;

	எனில் (nongit) {
		strbuf_addstr(hook_info,
			_("not run from a git repository - no hooks to show\n"));
		திரும்பு;
	}

	க்கு (p = hook_name_list; *p; p++) {
		மாறிலி எழுத்து *hook = *p;

		எனில் (hook_exists(the_repository, hook))
			strbuf_addf(hook_info, "%s\n", hook);
	}
}

நிலையான மாறிலி எழுத்து * மாறிலி bugreport_usage[] = {
	N_("git bugreport [(-o | --output-directory) <path>]\n"
	   "              [(-s | --suffix) <format> | --no-suffix]\n"
	   "              [--diagnose[=<mode>]]"),
	வெற்று
};

நிலையான முழு get_bug_template(கட்டமைப்பு strbuf *template)
{
	மாறிலி எழுத்து template_text[] = N_(
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
	திரும்பு 0;
}

நிலையான வெறுமை get_header(கட்டமைப்பு strbuf *buf, மாறிலி எழுத்து *title)
{
	strbuf_addf(buf, "\n\n[%s]\n", title);
}

முழு cmd_bugreport(முழு argc,
		  மாறிலி எழுத்து **argv,
		  மாறிலி எழுத்து *prefix,
		  கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு strbuf buffer = STRBUF_INIT;
	கட்டமைப்பு strbuf report_path = STRBUF_INIT;
	முழு report = -1;
	time_t now = time(வெற்று);
	கட்டமைப்பு tm tm;
	எண்ணிக்கை diagnose_mode diagnose = DIAGNOSE_NONE;
	எழுத்து *option_output = வெற்று;
	மாறிலி எழுத்து *option_suffix = "%Y-%m-%d-%H%M";
	மாறிலி எழுத்து *user_relative_path = வெற்று;
	எழுத்து *prefixed_filename;
	size_t output_path_len;
	முழு ret;

	மாறிலி கட்டமைப்பு option bugreport_options[] = {
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

	எனில் (argc) {
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
	எனில் (option_suffix) {
		strbuf_addch(&report_path, '-');
		strbuf_addftime(&report_path, option_suffix, localtime_r(&now, &tm), 0, 0);
	}
	strbuf_addstr(&report_path, ".txt");

	தேர்வு (safe_create_leading_directories(the_repository, report_path.buf)) {
	நிலை SCLD_OK:
	நிலை SCLD_EXISTS:
		நிறுத்து;
	இயல்பு:
		die(_("could not create leading directories for '%s'"),
		    report_path.buf);
	}

	/* Prepare diagnostics, if requested */
	எனில் (diagnose != DIAGNOSE_NONE) {
		கட்டமைப்பு strbuf zip_path = STRBUF_INIT;
		strbuf_add(&zip_path, report_path.buf, output_path_len);
		strbuf_addstr(&zip_path, "git-diagnostics-");
		strbuf_addftime(&zip_path, option_suffix, localtime_r(&now, &tm), 0, 0);
		strbuf_addstr(&zip_path, ".zip");

		எனில் (create_diagnostics_archive(the_repository, &zip_path, diagnose))
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

	எனில் (write_in_full(report, buffer.buf, buffer.len) < 0)
		die_errno(_("unable to write to %s"), report_path.buf);

	close(report);

	/*
	 * We want to print the path relative to the user, but we still need the
	 * path relative to us to give to the editor.
	 */
	எனில் (!(prefix && skip_prefix(report_path.buf, prefix, &user_relative_path)))
		user_relative_path = report_path.buf;
	fprintf(stderr, _("Created new report at '%s'.\n"),
		user_relative_path);

	விடுவி(prefixed_filename);
	strbuf_release(&buffer);

	ret = !!launch_editor(report_path.buf, வெற்று, வெற்று);
	strbuf_release(&report_path);
	திரும்பு ret;
}
