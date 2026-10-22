#تعریف_کن USE_THE_REPOSITORY_VARIABLE

#ضمن "git-compat-util.h"
#ضمن "credential.h"
#ضمن "builtin.h"
#ضمن "environment.h"
#ضمن "config.h"

ساكن ثابت حرف usage_پیام[] =
	"git credential (fill|approve|reject)";

صحيح cmd_credential(صحيح argc,
		   ثابت حرف **argv,
		   ثابت حرف *پیشوند استفادهنشده,
		   بنية repository *repo استفادهنشده)
{
	ثابت حرف *op;
	بنية credential c = CREDENTIAL_INIT;

	repo_config(the_repository, git_default_config, عدم);

	show_usage_if_asked(argc, argv, usage_پیام);
	إذا (argc != 2)
		usage(usage_پیام);
	op = argv[1];

	إذا (!strcmp(op, "capability")) {
		credential_مجموعه_all_capabilities(&c, CREDENTIAL_OP_INITIAL);
		credential_announce_capabilities(&c, stdout);
		أرجع 0;
	}

	إذا (credential_read(&c, stdin, CREDENTIAL_OP_INITIAL) < 0)
		die("unable to read credential from stdin");

	إذا (!strcmp(op, "fill")) {
		credential_fill(the_repository, &c, 0);
		credential_بعدی_state(&c);
		credential_write(&c, stdout, CREDENTIAL_OP_RESPONSE);
	} وإلا إذا (!strcmp(op, "approve")) {
		credential_مجموعه_all_capabilities(&c, CREDENTIAL_OP_HELPER);
		credential_approve(the_repository, &c);
	} وإلا إذا (!strcmp(op, "reject")) {
		credential_مجموعه_all_capabilities(&c, CREDENTIAL_OP_HELPER);
		credential_reject(the_repository, &c);
	} وإلا {
		usage(usage_پیام);
	}

	credential_clear(&c);
	أرجع 0;
}
